#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <type_traits>
#include <utility>
#include <tuple>
#include <limits>
#include <atomic>
#include <vector>

#if __INTELLISENSE__ && !_HAS_CXX20
#define TEMP_DEF_CXX20 1
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif
#include <bit>
#if __INTELLISENSE__ && TEMP_DEF_CXX20
#undef _HAS_CXX20
#define _HAS_CXX20 0
#endif

#include "8086_cpu.h"

#include "../zero/util.h"

#define USE_BITFIELDS 1

#include "z86_core_internal_pre.h"

static z86Memory<1_MB> mem;

struct z8086Context : z86Core<z8086 /*, FLAG_OPCODES_80186 | FLAG_OPCODES_80286 | FLAG_OPCODES_80386 | FLAG_OPCODES_80486 | FLAG_CPUID_CMOV*/> {

    // Internal state
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;
    std::atomic<int16_t> pending_einterrupt;
    size_t clock;

    inline constexpr void init() {
        memset(this, 0, sizeof(*this));
        this->cs = 0xF000;
        this->ip = 0xFFF0;
        this->pending_einterrupt = -1;
        this->pending_sinterrupt = -1;
    }

    inline constexpr void reset() {
        this->init();
        this->seg_override = -1;
        this->rep_type = NO_REP;
        this->halted = false;
    }

    inline constexpr uint16_t& index_seg(uint8_t index) {
        return this->seg[index & 3];
    }

    // Instruction implementations
    template <typename T = uint16_t>
    inline T get_flags() const {
        uint16_t base = 0b111100000000010;
        base |= this->carry;
        base |= (uint32_t)this->parity << 2;
        base |= (uint32_t)this->auxiliary << 4;
        base |= (uint32_t)this->zero << 6;
        base |= (uint32_t)this->sign << 7;
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            base |= (uint32_t)this->trap << 8;
            base |= (uint32_t)this->interrupt << 9;
            base |= (uint32_t)this->direction << 10;
            base |= (uint32_t)this->overflow << 11;
        }
        return base;
    }

    template <typename T = uint16_t>
    inline void set_flags(T src) {
        this->carry = src & 0x01;
        this->parity = src & 0x04;
        this->auxiliary = src & 0x10;
        this->zero = src & 0x40;
        this->sign = src & 0x80;
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            this->trap = src & 0x0100;
            this->interrupt = src & 0x0200;
            this->direction = src & 0x0400;
            this->overflow = src & 0x0800;
        }
    }

    inline void check_for_software_interrupt() {
        int16_t pending_software = this->pending_sinterrupt;
        if (pending_software > 0) {
            this->pending_sinterrupt = -1;
            this->call_interrupt(pending_software);
        }
    }

    template <bool set_halt = false>
    inline void check_for_nmi() {
        if (this->pending_nmi.exchange(false)) {
            if constexpr (set_halt) {
                this->halted = false;
            }
            this->call_interrupt(IntNMI);
        }
    }

    template <bool set_halt = false>
    inline void check_for_external_interrupt() {
        if (this->interrupt) {
            int16_t external = this->pending_einterrupt.exchange(-1);
            if (external > 0) {
                if constexpr (set_halt) {
                    this->halted = false;
                }
                this->call_interrupt(external);
            }
        }
    }

    inline void call_interrupt(uint8_t number) {
        this->PUSH(this->get_flags());
        this->interrupt = false;
        bool prev_trap = this->trap;
        this->trap = false;
        this->PUSH(this->cs);
        this->PUSH(this->rip);

        size_t interrupt_addr = (size_t)number << 2;
        this->cs = mem.read<uint16_t>(interrupt_addr);
        this->ip = mem.read<uint16_t>(interrupt_addr + 2);

        this->check_for_nmi();
        if (prev_trap) {
            this->call_interrupt(IntDB);
        }
    }

    inline void execute_pending_interrupts() {
        this->check_for_software_interrupt();
        do {
            this->check_for_nmi<true>();
            this->check_for_external_interrupt<true>();
        } while (this->halted);
        if (this->trap) {
            this->call_interrupt(IntDB);
        }
    }

    inline void external_interrupt(uint8_t number) {
        this->pending_einterrupt = number;
    }

    inline void nmi() {
        this->pending_nmi = true;
    }

    inline void cancel_interrupt() {
        this->pending_einterrupt = -1;
    }
};

static z8086Context ctx;

using z86Addr = z86AddrImpl<16>;

static std::vector<PortDwordDevice*> io_dword_devices;
static std::vector<PortWordDevice*> io_word_devices;
static std::vector<PortByteDevice*> io_byte_devices;

#include "z86_core_internal_post.h"

dllexport size_t z86_mem_write(size_t dst, const void* src, size_t size) {
    return mem.write(dst, src, size);
}
dllexport size_t z86_mem_read(void* dst, size_t src, size_t size) {
    return mem.read(dst, src, size);
}

dllexport void z86_add_dword_device(PortDwordDevice* device) {
    //io_dword_devices.push_back(device);
}
dllexport void z86_add_word_device(PortWordDevice* device) {
    io_word_devices.push_back(device);
}
dllexport void z86_add_byte_device(PortByteDevice* device) {
    io_byte_devices.push_back(device);
}

dllexport void z86_reset() {
    ctx.reset();
}

dllexport void z86_nmi() {
    ctx.nmi();
}

dllexport void z86_interrupt(uint8_t number) {
    ctx.external_interrupt(number);
}

dllexport void z86_cancel_interrupt() {
    ctx.cancel_interrupt();
}

dllexport void z86_execute() {
    ctx.init();

#define FAULT_CHECK(...) if (expect(__VA_ARGS__, false)) { goto fault; }

#define THROW_UD() if constexpr (!ctx.NO_UD) { ctx.set_fault(IntUD); goto fault; }

    for (;;) {
        // Reset per-instruction states
        ctx.seg_override = -1;
        ctx.rep_type = NO_REP;
        ctx.lock = false;

        z86Addr pc = ctx.pc();
        // TODO: The 6 byte prefetch cache
        // TODO: Clock cycles
    next_byte:
        uint32_t opcode = pc.read_advance();
        switch (opcode) {
            case 0x00: // ADD Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return true;
                }));
                break;
            case 0x01: // ADD Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return true;
                }));
                break;
            case 0x02: // ADD Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return true;
                }));
                break;
            case 0x03: // ADD Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return true;
                }));
                break;
            case 0x04: // ADD AL, Ib
                ctx.binopAI<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                });
                break;
            case 0x05: // ADD AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                });
                break;
            case 0x08: // OR Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return true;
                }));
                break;
            case 0x09: // OR Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return true;
                }));
                break;
            case 0x0A: // OR Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return true;
                }));
                break;
            case 0x0B: // OR Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return true;
                }));
                break;
            case 0x0C: // OR AL, Ib
                ctx.binopAI<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                });
                break;
            case 0x0D: // OR AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                });
                break;
            case 0x10: // ADC Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                    return true;
                }));
                break;
            case 0x11: // ADC Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                    return true;
                }));
                break;
            case 0x12: // ADC Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                    return true;
                }));
                break;
            case 0x13: // ADC Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                    return true;
                }));
                break;
            case 0x14: // ADC AL, Ib
                ctx.binopAI<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                });
                break;
            case 0x15: // ADC AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                });
                break;
            case 0x18: // SBB Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                    return true;
                }));
                break;
            case 0x19: // SBB Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                    return true;
                }));
                break;
            case 0x1A: // SBB Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                    return true;
                }));
                break;
            case 0x1B: // SBB Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                    return true;
                }));
                break;
            case 0x1C: // SBB AL, Ib
                ctx.binopAI<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                });
                break;
            case 0x1D: // SBB AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                });
                break;
            case 0x20: // AND Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                    return true;
                }));
                break;
            case 0x21: // AND Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                    return true;
                }));
                break;
            case 0x22: // AND Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                    return true;
                }));
                break;
            case 0x23: // AND Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                    return true;
                }));
                break;
            case 0x24: // AND AL, Ib
                ctx.binopAI<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                });
                break;
            case 0x25: // AND AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                });
                break;
            case 0x26: case 0x2E: case 0x36: case 0x3E: // SEG:
                ctx.set_seg_override((opcode >> 3) & 3);
                goto next_byte;
            case 0x27: // DAA
                ctx.DAA();
                break;
            case 0x28: // SUB Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return true;
                }));
                break;
            case 0x29: // SUB Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return true;
                }));
                break;
            case 0x2A: // SUB Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return true;
                }));
                break;
            case 0x2B: // SUB Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return true;
                }));
                break;
            case 0x2C: // SUB AL, Ib
                ctx.binopAI<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                });
                break;
            case 0x2D: // SUB AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                });
                break;
            case 0x2F: // DAS
                ctx.DAS();
                break;
            case 0x30: // XOR Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return true;
                }));
                break;
            case 0x31: // XOR Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return true;
                }));
                break;
            case 0x32: // XOR Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return true;
                }));
                break;
            case 0x33: // XOR Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return true;
                }));
                break;
            case 0x34: // XOR AL, Ib
                ctx.binopAI<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                });
                break;
            case 0x35: // XOR AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                });
                break;
            case 0x37: // AAA
                ctx.AAA();
                break;
            case 0x38: // CMP Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return false;
                }));
                break;
            case 0x39: // CMP Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return false;
                }));
                break;
            case 0x3A: // CMP Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return false;
                }));
                break;
            case 0x3B: // CMP Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return false;
                }));
                break;
            case 0x3C: // CMP AL, Ib
                ctx.binopAI<true>(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                });
                break;
            case 0x3D: // CMP AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.CMP(dst, src);
                });
                break;
            case 0x3F: // AAS
                ctx.AAS();
                break;
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: // INC reg
                ctx.INC(ctx.index_regMB<uint16_t>(opcode & 7));
                break;
            case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: // DEC reg
                ctx.DEC(ctx.index_regMB<uint16_t>(opcode & 7));
                break;
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: // PUSH reg
                if constexpr (ctx.OLD_PUSH_SP) {
                    ctx.PUSH(ctx.index_regMB<uint16_t>(opcode & 7));
                }
                else {
                    auto temp = ctx.index_regMB<uint16_t>(opcode & 7);
                    ctx.PUSH(temp);
                }
                break;
            case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: // POP reg
                ctx.index_regMB<uint16_t>(opcode & 7) = ctx.POP();
                break;
            case 0x60: // PUSHA
                if constexpr (ctx.OPCODES_80186) {
                    ctx.PUSHA();
                    break;
                }
                THROW_UD();
            case 0x61:
                if constexpr (ctx.OPCODES_80186) {
                    ctx.POPA();
                    break;
                }
                THROW_UD();
            case 0x70: // JO Jb
            case 0x71: // JNO Jb
                ctx.JCC<CondNO, true>(pc, opcode & 1);
                goto next_instr;
            case 0x62: // BOUND Rv, Mv2
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.binopRM2(pc, [](auto index, auto lower, auto upper) regcall{
                        return ctx.BOUND(index, lower, upper);
                    }));
                    break;
                }
                THROW_UD();
            case 0x63: // ARPL
                if constexpr (ctx.OPCODES_80286) {
                    // Not valid in real mode apparently
                    // TODO
                }
                THROW_UD();
            case 0x72: // JC Jb
            case 0x73: // JNC Jb
                ctx.JCC<CondNC, true>(pc, opcode & 1);
                goto next_instr;
            case 0x64: case 0x65: // FS/GS prefixes
                if constexpr (ctx.OPCODES_80386) {
                    ctx.set_seg_override(opcode & 0xF);
                    goto next_byte;
                }
                THROW_UD();
            case 0x74: // JZ Jb
            case 0x75: // JNZ Jb
                ctx.JCC<CondNZ, true>(pc, opcode & 1);
                goto next_instr;
            case 0x66: // Data size prefix
                if constexpr (ctx.max_bits > 16) {
                    // TODO: Temporary override
                    goto next_byte;
                }
                THROW_UD();
            case 0x67: // Addr size prefix
                if constexpr (ctx.max_bits > 16) {
                    // TODO: Temporary override
                    goto next_byte;
                }
                THROW_UD();
            case 0x76: // JBE Jb
            case 0x77: // JA Jb
                ctx.JCC<CondA, true>(pc, opcode & 1);
                goto next_instr;
            case 0x68: // PUSH Is
                if constexpr (ctx.OPCODES_80186) {
                    ctx.PUSHI(pc.read_advance_Is());
                    break;
                }
                THROW_UD();
            case 0x69: // IMUL Rv, Mv, Is
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.binopRM(pc, [&](auto& dst, auto src) regcall {
                        ctx.IMUL(dst, src, pc.read_advance_Is());
                    }));
                    break;
                }
                THROW_UD();
            case 0x78: // JS Jb
            case 0x79: // JNS Jb
                ctx.JCC<CondNS, true>(pc, opcode & 1);
                goto next_instr;
            case 0x6A: // PUSH Ib
                if constexpr (ctx.OPCODES_80186) {
                    ctx.PUSHI(pc.read_advance<int8_t>());
                    break;
                }
                THROW_UD();
            case 0x6B: // IMUL Rv, Mv, Ib
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.binopRM(pc, [&](auto& dst, auto src) regcall {
                        ctx.IMUL(dst, src, pc.read<int8_t>());
                    }));
                    ++pc;
                    break;
                }
                THROW_UD();
            case 0x7A: // JP Jb
            case 0x7B: // JNP Jb
                ctx.JCC<CondNP, true>(pc, opcode & 1);
                goto next_instr;
            case 0x6C: // INSB
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.INS<true>());
                    break;
                }
                THROW_UD();
            case 0x6D: // INS
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.INS());
                    break;
                }
                THROW_UD();
            case 0x7C: // JL Jb
            case 0x7D: // JGE Jb
                ctx.JCC<CondGE, true>(pc, opcode & 1);
                goto next_instr;
            case 0x6E: // OUTSB
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.OUTS<true>());
                    break;
                }
                THROW_UD();
            case 0x6F: // OUTS
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.OUTS());
                    break;
                }
                THROW_UD();
            case 0x7E: // JLE Jb
            case 0x7F: // JG Jb
                ctx.JCC<CondG, true>(pc, opcode & 1);
                goto next_instr;
            case 0x80: case 0x82: // GRP1 Mb, Ib
                FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                    uint8_t val = pc.read<int8_t>();
                    switch (r) {
                        case 0: ctx.ADD(dst, val); return true;
                        case 1: ctx.OR(dst, val); return true;
                        case 2: ctx.ADC(dst, val); return true;
                        case 3: ctx.SBB(dst, val); return true;
                        case 4: ctx.AND(dst, val); return true;
                        case 5: ctx.SUB(dst, val); return true;
                        case 6: ctx.XOR(dst, val); return true;
                        case 7: ctx.CMP(dst, val); return false;
                        default: unreachable;
                    }
                }));
                ++pc;
                break;
            case 0x81: // GRP1 Ev, Is
                FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                    int32_t val = pc.read_advance_Is();
                    switch (r) {
                        case 0: ctx.ADD(dst, val); return true;
                        case 1: ctx.OR(dst, val); return true;
                        case 2: ctx.ADC(dst, val); return true;
                        case 3: ctx.SBB(dst, val); return true;
                        case 4: ctx.AND(dst, val); return true;
                        case 5: ctx.SUB(dst, val); return true;
                        case 6: ctx.XOR(dst, val); return true;
                        case 7: ctx.CMP(dst, val); return false;
                        default: unreachable;
                    }
                }));
                break;
            case 0x83: // GRP1 Ev, Ib
                FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                    int32_t val = pc.read<int8_t>();
                    switch (r) {
                        case 0: ctx.ADD(dst, val); return true;
                        case 1: ctx.OR(dst, val); return true;
                        case 2: ctx.ADC(dst, val); return true;
                        case 3: ctx.SBB(dst, val); return true;
                        case 4: ctx.AND(dst, val); return true;
                        case 5: ctx.SUB(dst, val); return true;
                        case 6: ctx.XOR(dst, val); return true;
                        case 7: ctx.CMP(dst, val); return false;
                        default: unreachable;
                    }
                }));
                ++pc;
                break;
            case 0x84: // TEST Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                    return false;
                }));
                break;
            case 0x85: // TEST Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                    return false;
                }));
                break;
            case 0x86: // XCHG Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto& src) regcall {
                    ctx.XCHG(dst, src);
                    return true;
                }));
                break;
            case 0x87: // XCHG Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto& src) regcall {
                    ctx.XCHG(dst, src);
                    return true;
                }));
                break;
            case 0x88: // MOV Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return true;
                }));
                break;
            case 0x89: // MOV Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return true;
                }));
                break;
            case 0x8A: // MOV Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return true;
                }));
                break;
            case 0x8B: // MOV Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return true;
                }));
                break;
            case 0x8C: // MOV M, seg
                FAULT_CHECK(ctx.binopMS(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return true;
                }));
                break;
            case 0x8D: { // LEA
                ModRM modrm = pc.read_advance<ModRM>();
                if (modrm.is_mem()) {
                    z86Addr addr = modrm.parse_memM(pc);
                    ctx.index_regR<uint16_t>(modrm.R()) = addr.offset;
                }
                else {
                    THROW_UD();
                    // TODO: jank
                }
                break;
            }
            case 0x8E: // MOV seg, M
                FAULT_CHECK(ctx.binopSM(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return true;
                }));
                break;
            case 0x8F: // GRP1A (Supposedly this does mystery jank if R != 0)
                FAULT_CHECK(ctx.unopM(pc, [](auto src, uint8_t r) regcall {
                    switch (r) {
                        case 0:
                            ctx.PUSH(src);
                            return false;
                        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                            // TODO: ???
                            return false;
                        default:
                            unreachable;
                    }
                }));
                break;
            case 0x90: // NOP, XCHG RAX, R8
                if (!ctx.rex_bits.B()) {
                    // NOP
                    break;
                }
            case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // XCHG AX, reg
                ctx.binopAR(opcode & 7, [](auto& dst, auto& src) regcall {
                    ctx.XCHG(dst, src);
                });
                break;
            case 0x98: // CBW
                ctx.CBW();
                break;
            case 0x99: // CWD
                ctx.CWD();
                break;
            case 0x9A: // CALL far abs
                ctx.CALLFABS(pc);
                goto next_instr;
            case 0x9B: // WAIT
                // NOP :D
                break;
            case 0x9C: // PUSHF
                ctx.PUSH(ctx.get_flags<uint16_t>());
                break;
            case 0x9D: // POPF
                ctx.set_flags<uint16_t>(ctx.POP());
                break;
            case 0x9E: // SAHF
                ctx.set_flags<uint8_t>(ctx.ah);
                break;
            case 0x9F: // LAHF
                ctx.ah = ctx.get_flags<uint8_t>();
                break;
            case 0xA0: // MOV AL, mem
                ctx.binopAO<true>(pc, [](auto& dst, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    dst = addr.read<decltype(dst)>();
                });
                break;
            case 0xA1: // MOV AX, mem
                ctx.binopAO(pc, [](auto& dst, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    dst = addr.read<decltype(dst)>();
                });
                break;
            case 0xA2: // MOV mem, AL
                ctx.binopAO<true>(pc, [](auto src, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    addr.write(src);
                });
                break;
            case 0xA3: // MOV mem, AX
                ctx.binopAO(pc, [](auto src, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    addr.write(src);
                });
                break;
            case 0xA4: // MOVSB
                FAULT_CHECK(ctx.MOVS<true>());
                break;
            case 0xA5: // MOVSW
                FAULT_CHECK(ctx.MOVS());
                break;
            case 0xA6: // CMPSB
                FAULT_CHECK(ctx.CMPS<true>());
                break;
            case 0xA7: // CMPSW
                FAULT_CHECK(ctx.CMPS());
                break;
            case 0xA8: // TEST AL, Ib
                ctx.binopAI<true>(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                });
                break;
            case 0xA9: // TEST AX, Is
                ctx.binopAI(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                });
                break;
            case 0xAA: // STOSB
                FAULT_CHECK(ctx.STOS<true>());
                break;
            case 0xAB: // STOSW
                FAULT_CHECK(ctx.STOS());
                break;
            case 0xAC: // LODSB
                FAULT_CHECK(ctx.LODS<true>());
                break;
            case 0xAD: // LODSW
                FAULT_CHECK(ctx.LODS());
                break;
            case 0xAE: // SCASB
                FAULT_CHECK(ctx.SCAS<true>());
                break;
            case 0xAF: // SCASW
                FAULT_CHECK(ctx.SCAS());
                break;
            case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: // MOV reg8, Ib
                ctx.MOV_RI<true>(pc, opcode & 7);
                break;
            case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: // MOV reg, Iv
                ctx.MOV_RI(pc, opcode & 7);
                break;
            case 0xC0: // GRP2 Mb, Ib
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall{
                        uint8_t count = pc.read<uint8_t>();
                        switch (r) {
                            case 0: ctx.ROL(dst, count); return true;
                            case 1: ctx.ROR(dst, count); return true;
                            case 2: ctx.RCL(dst, count); return true;
                            case 3: ctx.RCR(dst, count); return true;
                            case 4: ctx.SHL(dst, count); return true;
                            case 5: ctx.SHR(dst, count); return true;
                            case 6: ctx.SETMO(dst, count); return true;
                            case 7: ctx.SAR(dst, count); return true;
                            default: unreachable;
                        }
                    }));
                    ++pc;
                    break;
                }
                THROW_UD();
            case 0xC2: // RET imm
                ctx.RETI(pc);
                goto next_instr;
            case 0xC1: // GRP2 Mv, Ib
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall{
                        uint8_t count = pc.read<uint8_t>();
                        switch (r) {
                            case 0: ctx.ROL(dst, count); return true;
                            case 1: ctx.ROR(dst, count); return true;
                            case 2: ctx.RCL(dst, count); return true;
                            case 3: ctx.RCR(dst, count); return true;
                            case 4: ctx.SHL(dst, count); return true;
                            case 5: ctx.SHR(dst, count); return true;
                            case 6: ctx.SETMO(dst, count); return true;
                            case 7: ctx.SAR(dst, count); return true;
                            default: unreachable;
                        }
                    }));
                    ++pc;
                    break;
                }
                THROW_UD();
            case 0xC3: // RET
                ctx.RET();
                goto next_instr;
            case 0xC4: // LES
                FAULT_CHECK(ctx.binopRMF(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.es = src >> (bitsof(src) >> 1);
                    return true;
                }));
                break;
            case 0xC5: // LDS
                FAULT_CHECK(ctx.binopRMF(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.ds = src >> (bitsof(src) >> 1);
                    return true;
                }));
                break;
            case 0xC6: // GRP11 Ib (Supposedly this just ignores R bits)
                FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                    dst = pc.read<int8_t>();
                    return true;
                }));
                ++pc;
                break;
            case 0xC7: // GRP11 Is (Supposedly this just ignores R bits)
                FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                    dst = pc.read_advance_Is();
                    return true;
                }));
                break;
            case 0xC8: // ENTER Iw, Ib
                if constexpr (ctx.OPCODES_80186) {
                    ctx.ENTER(pc.read<uint16_t>(), pc.read<uint8_t>(2));
                    break;
                }
                THROW_UD();
            case 0xCA: // RETF imm
                ctx.RETFI(pc);
                goto next_instr;
            case 0xC9: // LEAVE
                if constexpr (ctx.OPCODES_80186) {
                    ctx.LEAVE();
                    break;
                }
                THROW_UD();
            case 0xCB: // RETF
                ctx.RETF();
                goto next_instr;
            case 0xCC: // INT3
                ctx.set_trap(IntBP);
                goto trap;
            case 0xCD: // INT Ib
                ctx.set_trap(pc.read_advance<uint8_t>());
                goto trap;
            case 0xCE: // INTO
                if (ctx.overflow) {
                    ctx.set_trap(IntOF);
                    goto trap;
                }
                break;
            case 0xCF: // IRET
                ctx.ip = ctx.POP();
                ctx.cs = ctx.POP();
                ctx.set_flags(ctx.POP());
                continue; // Using continues delays execution deliberately
            case 0xD0: // GRP2 Mb, 1
                FAULT_CHECK(ctx.unopM<true>(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        case 0: ctx.ROL(dst, 1); return true;
                        case 1: ctx.ROR(dst, 1); return true;
                        case 2: ctx.RCL(dst, 1); return true;
                        case 3: ctx.RCR(dst, 1); return true;
                        case 4: ctx.SHL(dst, 1); return true;
                        case 5: ctx.SHR(dst, 1); return true;
                        case 6: ctx.SETMO(dst, 1); return true;
                        case 7: ctx.SAR(dst, 1); return true;
                        default: unreachable;
                    }
                }));
                break;
            case 0xD1: // GRP2 Mv, 1
                FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        case 0: ctx.ROL(dst, 1); return true;
                        case 1: ctx.ROR(dst, 1); return true;
                        case 2: ctx.RCL(dst, 1); return true;
                        case 3: ctx.RCR(dst, 1); return true;
                        case 4: ctx.SHL(dst, 1); return true;
                        case 5: ctx.SHR(dst, 1); return true;
                        case 6: ctx.SETMO(dst, 1); return true;
                        case 7: ctx.SAR(dst, 1); return true;
                        default: unreachable;
                    }
                }));
                break;
            case 0xD2: // GRP2 Mb, CL
                FAULT_CHECK(ctx.unopM<true>(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        case 0: ctx.ROL(dst, ctx.cl); return true;
                        case 1: ctx.ROR(dst, ctx.cl); return true;
                        case 2: ctx.RCL(dst, ctx.cl); return true;
                        case 3: ctx.RCR(dst, ctx.cl); return true;
                        case 4: ctx.SHL(dst, ctx.cl); return true;
                        case 5: ctx.SHR(dst, ctx.cl); return true;
                        case 6: ctx.SETMO(dst, ctx.cl); return true;
                        case 7: ctx.SAR(dst, ctx.cl); return true;
                        default: unreachable;
                    }
                }));
                break;
            case 0xD3: // GRP2 Mv, CL
                FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        case 0: ctx.ROL(dst, ctx.cl); return true;
                        case 1: ctx.ROR(dst, ctx.cl); return true;
                        case 2: ctx.RCL(dst, ctx.cl); return true;
                        case 3: ctx.RCR(dst, ctx.cl); return true;
                        case 4: ctx.SHL(dst, ctx.cl); return true;
                        case 5: ctx.SHR(dst, ctx.cl); return true;
                        case 6: ctx.SETMO(dst, ctx.cl); return true;
                        case 7: ctx.SAR(dst, ctx.cl); return true;
                        default: unreachable;
                    }
                }));
                break;
            case 0xD4: // AAM Ib
                FAULT_CHECK(ctx.AAM(pc.read_advance<uint8_t>()));
                break;
            case 0xD5: // AAD Ib
                ctx.AAD(pc.read_advance<uint8_t>());
                break;
            case 0xD6: // SALC
                ctx.al = ctx.carry ? -1 : 0;
                break;
            case 0xD7: { // XLAT
                z86Addr addr = ctx.addr(DS, ctx.bx + ctx.al);
                ctx.al = addr.read<uint8_t>();
                break;
            }
            case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF: { // ESC x87
                // NOP :D
                pc += 1 + pc.read<ModRM>().extra_length(pc);
                break;
            }
            case 0xE0: // LOOPNZ Jb
            case 0xE1: // LOOPZ Jb
                ctx.LOOPCC(pc, opcode & 1);
                goto next_instr;
            case 0xE2: // LOOP Jb
                ctx.LOOP(pc);
                goto next_instr;
            case 0xE3: // JCXZ Jb
                ctx.JCXZ(pc);
                goto next_instr;
            case 0xE4: // IN AL, Ib
                ctx.port_in<true>(pc.read_advance<uint8_t>());
                break;
            case 0xE5: // IN AX, Ib
                ctx.port_in(pc.read_advance<uint8_t>());
                break;
            case 0xE6: // OUT Ib, AL
                ctx.port_out<true>(pc.read_advance<uint8_t>());
                break;
            case 0xE7: // OUT Ib, AX
                ctx.port_out(pc.read_advance<uint8_t>());
                break;
            case 0xE8: // CALL Jz
                ctx.CALL(pc);
                goto next_instr;
            case 0xE9: // JMP Jz
                ctx.JMP(pc);
                goto next_instr;
            case 0xEA: // JMP far abs
                ctx.JMPFABS(pc);
                goto next_instr;
            case 0xEB: // JMP Jb
                ctx.JMP<true>(pc);
                goto next_instr;
            case 0xEC: // IN AL, DX
                ctx.port_in<true>(ctx.dx);
                break;
            case 0xED: // IN AX, DX
                ctx.port_in(ctx.dx);
                break;
            case 0xEE: // OUT DX, AL
                ctx.port_out<true>(ctx.dx);
                break;
            case 0xEF: // OUT DX, AX
                ctx.port_out(ctx.dx);
                break;
            case 0xF0: case 0xF1: // LOCK
                ctx.lock = true;
                goto next_byte;
            case 0xF2: case 0xF3: // REPNE, REP
                ctx.rep_type = opcode & 1;
                goto next_byte;
            case 0xF4: // HLT
                ctx.halted = true;
                break;
            case 0xF5: // CMC
                ctx.carry ^= 1;
                break;
            case 0xF6: // GRP3 Mb
                FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& val, uint8_t r) regcall -> uint8_t {
                    switch (r) {
                        case 0: case 1: // TEST Mb, Ib
                            ctx.TEST(val, pc.read<uint8_t>());
                            return false;
                        case 2: // NOT Mb
                            ctx.NOT(val);
                            return true;
                        case 3: // NEG Mb
                            ctx.NEG(val);
                            return true;
                        case 4: // MUL Mb
                            ctx.MUL(val);
                            return false;
                        case 5: // IMUL Mb
                            ctx.IMUL(val);
                            return false;
                        case 6: // DIV Mb
                            return ctx.DIV(val) ? 2 : 0;
                        case 7: // IDIV Mb
                            return ctx.IDIV(val) ? 2 : 0;
                        default:
                            unreachable;
                    }
                }));
                ++pc;
                break;
            case 0xF7: // GRP3 Mv
                FAULT_CHECK(ctx.unopM(pc, [&](auto& val, uint8_t r) regcall -> uint8_t {
                    switch (r) {
                        case 0: case 1: // TEST Mv, Is
                            ctx.TEST(val, pc.read_advance_Is());
                            return false;
                        case 2: // NOT Mv
                            ctx.NOT(val);
                            return true;
                        case 3: // NEG Mv
                            ctx.NEG(val);
                            return true;
                        case 4: // MUL Mv
                            ctx.MUL(val);
                            return false;
                        case 5: // IMUL Mv
                            ctx.IMUL(val);
                            return false;
                        case 6: // DIV Mv
                            return ctx.DIV(val) ? 2 : 0;
                        case 7: // IDIV Mv
                            return ctx.IDIV(val) ? 2: 0;
                        default:
                            unreachable;
                    }
                }));
                break;
            case 0xF8: case 0xF9: // CLC, STC
                ctx.carry = opcode & 1;
                break;
            case 0xFA: case 0xFB: // CLI, STI
                ctx.interrupt = opcode & 1;
                break;
            case 0xFC: case 0xFD: // CLD, STD
                ctx.direction = opcode & 1;
                break;

                // TODO: Fault support
            case 0xFE: // GRP4 Mb
                if (ctx.unopMS<true>(pc)) {
                    goto next_instr;
                }
                break;
            case 0xFF: // GRP5 Mv
                if (ctx.unopMS(pc)) {
                    goto next_instr;
                }
                break;
            case 0x06: case 0x0E: case 0x16: case 0x1E: // PUSH seg
                ctx.PUSH(ctx.index_seg(opcode >> 3));
                break;
            case 0x0F:
                if constexpr (ctx.OPCODES_80286) {
                    // TODO: crap
                    opcode = pc.read_advance();
                    switch (opcode) {
                        case 0x00: // GRP6
                        case 0x01: // GRP7
                        case 0x02: // LAR Rv, Mv
                        case 0x03: // LSL Rv, Mv
                        case 0x04: // SAVEALL
                        case 0x05: // SYSCALL, LOADALL2
                        case 0x06: // CLTS
                        case 0x07: // SYSRET, LOADALL3
                        case 0x08: // INVD (486)
                        case 0x09: // WBINVD (486)
                        case 0x0A: // CL1INVMB (wtf)
                            break;
                        case 0x0B: // UD2
                        case 0xB9: // UD1
                        case 0xFF: // UD0
                            ctx.set_fault(IntUD);
                            goto fault;
                        case 0x24: // MOV M, TR
                        case 0x26: // MOV TR, M
                        case 0xA6: // XBTS
                        case 0xA7: // IBTS
                        case 0x0C: case 0x25: case 0x27: case 0x36: case 0x37:
                        case 0x39: case 0x3B: case 0x3C: case 0x3E: case 0x3F:
                        case 0x7A: case 0x7B:
                            THROW_UD();
                            break;
                        case 0x0D: // PREFETCHx
                        case 0x0E: // FEMMS
                        case 0x0F: // 3DNow!
                        case 0x10: // MOVUPS/MOVUPD/MOVSS/MOVSD Rx, Mx
                        case 0x11: // MOVUPS/MOVUPD/MOVSS/MOVSD Mx, Rx
                        case 0x12: // (MOVLPS/MOVLPD Rx, Mx), (MOVSLDUP Rx, Mx), (MOVDDUP Rx, Mx)
                        case 0x13: // MOVLPS/MOVLPD Mx, Rx
                        case 0x14: // UNPCKLPS/UNPCKLPD Rx, Mx
                        case 0x15: // UNPCKLPS/UNPCKLPD Mx, Rx
                        case 0x16: // (MOVHPS/MOVHPD Rx, Mx), (MOVSHDUP Rx, Mx)
                        case 0x17: // MOVHPS/MOVHPD Mx, Rx
                        case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F: // HINT_NOP
                        case 0x20: // MOV M, CR
                        case 0x21: // MOV M, DR
                        case 0x22: // MOV CR, M
                        case 0x23: // MOV DR, M
                        case 0x28: // MOVAPS/MOVAPD Rx, Mx
                        case 0x29: // MOVAPS/MOVAPD Mx, Rx
                        case 0x2A: // CVTPI2PS/CVTPI2PD/CVTSI2SS/CVTSI2SD Rx, Mx
                        case 0x2B: // MOVNTPS/MOVNTPD/MOVNTSS/MOVNTSD Mx, Rx
                        case 0x2C: // (CVTTPS2PI/CVTTPD2PI Rm, Mx), (CVTTSS2SI/CVTTSD2SI Rv, Mx)
                        case 0x2D: // (CVTPS2PI/CVTPD2PI Rm, Mx), (CVTSS2SI/CVTSD2SI Rv, Mx)
                        case 0x2E: // UCOMISS/UCOMISD Rx, Mx
                        case 0x2F: // COMISS/COMISD Rx, Mx
                        case 0x30: // WRMSR
                        case 0x31: // RDTSC
                        case 0x32: // RDMSR
                        case 0x33: // RDPMC
                        case 0x34: // SYSENTER
                        case 0x35: // SYSEXIT
                        case 0x38: // Three byte opcodes A
                        case 0x3A: // Three byte opcodes B
                            break;
                        case 0x40: // CMOVO Rv, Mv
                        case 0x41: // CMOVNO Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondNO>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x42: // CMOVC Rv, Mv
                        case 0x43: // CMOVNC Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondNC>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x44: // CMOVZ Rv, Mv
                        case 0x45: // CMOVNZ Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondNZ>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x46: // CMOVBE Rv, Mv
                        case 0x47: // CMOVA Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondA>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x48: // CMOVS Rv, Mv
                        case 0x49: // CMOVNS Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondNS>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x4A: // CMOVP Rv, Mv
                        case 0x4B: // CMOVNP Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondNP>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x4C: // CMOVL Rv, Mv
                        case 0x4D: // CMOVGE Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondGE>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x4E: // CMOVLE Rv, Mv
                        case 0x4F: // CMOVG Rv, Mv
                            if constexpr (!ctx.CPUID_CMOV) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                                ctx.CMOVCC<CondG>(dst, src, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x50: // MOVMSKPS/MOVMSKPD Rv, Mx
                        case 0x51: // SQRTPS/SQRTPD/SQRTSS/SQRTSD Rx, Mx
                        case 0x52: // RSQRTPS/RSQRTSS Rx, Mx
                        case 0x53: // RCPPS/RCPSS Rx, Mx
                        case 0x54: // ANDPS/ANDPD Rx, Mx
                        case 0x55: // ANDNPS/ANDNPD Rx, Mx
                        case 0x56: // ORPS/ORPD Rx, Mx
                        case 0x57: // XORPS/XORPD Rx, Mx
                        case 0x58: // ADDPS/ADDPD/ADDSS/ADDSD Rx, Mx
                        case 0x59: // MULPS/MULPD/MULSS/MULSD Rx, Mx
                        case 0x5A: // CVTPS2PD/CVTPD2PS/CVTSS2SD/CVTSD2SS Rx, Mx
                        case 0x5B: // CVTDQ2PS/CVTPS2DQ/CVTTPS2DQ Rx, Mx
                        case 0x5C: // SUBPS/SUBPD/SUBSS/SUBSD Rx, Mx
                        case 0x5D: // MINPS/MINPD/MINSS/MINSD Rx, Mx
                        case 0x5E: // DIVPS/DIVPD/DIVSS/DIVSD Rx, Mx
                        case 0x5F: // MAXPS/MAXPD/MAXSS/MAXSD Rx, Mx
                        case 0x60: // PUNPCKLBW R, M
                        case 0x61: // PUNPCKLWD R, M
                        case 0x62: // PUNPCKLDQ R, M
                        case 0x63: // PACKSSWB R, M
                        case 0x64: // PCMPGTB R, M
                        case 0x65: // PCMPGTW R, M
                        case 0x66: // PCMPGTD R, M
                        case 0x67: // PACKUSWB R, M
                        case 0x68: // PUNPCKHBW R, M
                        case 0x69: // PUNPCKHWD R, M
                        case 0x6A: // PUNPCKHDQ R, M
                        case 0x6B: // PACKSSDW R, M
                        case 0x6C: // PUNPCKLQDQ R, M
                        case 0x6D: // PUNPCKHQDQ R, M
                        case 0x6E: // MOVD R, M
                        case 0x6F: // MOVQ/MOVDQA/MOVDQU R, M
                        case 0x70: // PSHUFW/PSHUFD/PSHUFHW/PSHUFLW R, M, Ib
                        case 0x71: // GRP12
                        case 0x72: // GRP13
                        case 0x73: // GRP14
                        case 0x74: // PCMPEQB Rx, Mx
                        case 0x75: // PCMPEQW Rx, Mx
                        case 0x76: // PCMPEQD Rx, Mx
                        case 0x77: // EMMS
                        case 0x78: // EXTRQ/INSERTQ
                        case 0x79: // EXTRQ/INSERTQ
                        case 0x7C: // HADDPD/HADDPS Rx, Mx
                        case 0x7D: // HSUBPD/HSUBPS Rx, Mx
                        case 0x7E: // MOVD/MOVQ
                        case 0x7F: // MOVQ/MOVDQA/MOVDQU M, R
                            break;
                        case 0x80: // JO Jz
                        case 0x81: // JNO Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondNO>(pc, opcode & 1);
                            goto next_instr;
                        case 0x82: // JC Jz
                        case 0x83: // JNC Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondNC>(pc, opcode & 1);
                            goto next_instr;
                        case 0x84: // JZ Jz
                        case 0x85: // JNZ Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondNZ>(pc, opcode & 1);
                            goto next_instr;
                        case 0x86: // JBE Jz
                        case 0x87: // JA Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondA>(pc, opcode & 1);
                            goto next_instr;
                        case 0x88: // JS Jz
                        case 0x89: // JNS Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondNS>(pc, opcode & 1);
                            goto next_instr;
                        case 0x8A: // JP Jz
                        case 0x8B: // JNP Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondNP>(pc, opcode & 1);
                            goto next_instr;
                        case 0x8C: // JL Jz
                        case 0x8D: // JGE Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondGE>(pc, opcode & 1);
                            goto next_instr;
                        case 0x8E: // JLE Jz
                        case 0x8F: // JG Jz
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.JCC<CondG>(pc, opcode & 1);
                            goto next_instr;
                        case 0x90: // SETO Mb
                        case 0x91: // SETNO Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondNO>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x92: // SETC Mb
                        case 0x93: // SETNC Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondNC>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x94: // SETZ Mb
                        case 0x95: // SETNZ Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondNZ>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x96: // SETBE Mb
                        case 0x97: // SETA Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondA>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x98: // SETS Mb
                        case 0x99: // SETNS Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondNS>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x9A: // SETP Mb
                        case 0x9B: // SETNP Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondNP>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x9C: // SETL Mb
                        case 0x9D: // SETGE Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondGE>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0x9E: // SETLE Mb
                        case 0x9F: // SETG Mb
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                                ctx.SETCC<CondG>(dst, opcode & 1);
                                return true;
                            }));
                            break;
                        case 0xA0: // PUSH FS
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.PUSH(ctx.fs);
                            break;
                        case 0xA1: // POP FS
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.fs = ctx.POP<uint16_t>();
                            break;
                        case 0xA2: // CPUID
                            break;
                        case 0xA3: // BT Mv, Gv
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                                // TODO
                                return false;
                            }));
                            break;
                        case 0xA4: // SHLD Mv, Gv, Ib
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [&](auto& dst, auto src) {
                                ctx.SHLD(dst, src, pc.read<uint8_t>());
                                return true;
                            }));
                            ++pc;
                            break;
                        case 0xA5: // SHLD Mv, Gv, CL
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                                ctx.SHLD(dst, src, ctx.cl);
                                return true;
                            }));
                            break;
                        case 0xA8: // PUSH GS
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.PUSH(ctx.gs);
                            break;
                        case 0xA9: // POP GS
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            ctx.gs = ctx.POP<uint16_t>();
                            break;
                        case 0xAA: // RSM
                            break;
                        case 0xAB: // BTS Mv, Rv
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                                // TODO
                                return true;
                            }));
                            break;
                        case 0xAC: // SHRD Mv, Rv, Ib
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [&](auto dst, auto src) {
                                ctx.SHRD(dst, src, pc.read<uint8_t>());
                                return true;
                            }));
                            ++pc;
                            break;
                        case 0xAD: // SHRD Mv, Rv, CL
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [](auto dst, auto src) {
                                ctx.SHRD(dst, src, ctx.cl);
                                return true;
                            }));
                            break;
                        case 0xAE: // GRP15
                            break;
                        case 0xAF: // IMUL Rv, Mv
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) {
                                ctx.IMUL(dst, src);
                                return true;
                            }));
                            break;
                        case 0xB0: // CMPXCHG Mb, Rb
                        case 0xB1: // CMPXCHG Mv, Rb
                            break;
                        case 0xB2: // LSS Rv, M
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRMF(pc, [](auto& dst, auto src) regcall {
                                dst = src;
                                ctx.ss = src >> (bitsof(src) >> 1);
                                return true;
                            }));
                            break;
                        case 0xB3: // BTR Mv, Rv
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                                // TODO
                                return true;
                            }));
                            break;
                        case 0xB4: case 0xB5: // LFS/LGS Rv, M
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRMF(pc, [=](auto& dst, auto src) regcall {
                                dst = src;
                                ctx.index_seg(FS + (opcode & 1)) = src >> (bitsof(src) >> 1);
                                return true;
                            }));
                            break;
                        case 0xB6: // MOVZX Rv, Mb
                        case 0xB7: // MOVZX Rv, Mv
                        case 0xB8: // JMPE, POPCNT Rv, Mv
                        case 0xBA: // GRP8
                            break;
                        case 0xBB: // BTC Mv, Gv
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                                // TODO
                                return true;
                            }));
                            break;
                        case 0xBC: // BSF/TZCNT Rv, Mv
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) {
                                ctx.BSF(dst, src);
                                return true;
                            }));
                            break;
                        case 0xBD: // BSR/LZCNT Rv, Mv
                            if constexpr (!ctx.OPCODES_80386) {
                                THROW_UD();
                            }
                            FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) {
                                ctx.BSR(dst, src);
                                return true;
                            }));
                            break;
                        case 0xBE: // MOVSX Rv, Mb
                        case 0xBF: // MOVSX Rv, Mv
                        case 0xC0: // XADD Mb, Rb
                        case 0xC1: // XADD Mv, Rv
                        case 0xC2: // CMPccPS/CMPccPD/CMPccSS/CMPccSD Rx, Mx
                        case 0xC3: // MOVNTI Mv, Rv
                        case 0xC4: // PINSRW Rx, Mx, Ib
                        case 0xC5: // PEXTRW Rx, Mx, Ib
                        case 0xC6: // SHUFPS/SHUFPD Rx, Mx, Ib
                        case 0xC7: // GRP9
                            break;
                        case 0xC8: case 0xC9: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCE: case 0xCF: // BSWAP reg
                            if constexpr (!ctx.OPCODES_80486) {
                                THROW_UD();
                            }
                            ctx.BSWAP(ctx.index_regMB<uint16_t>(opcode & 7));
                            break;
                        case 0xD0: // ADDSUBPD/ADDSUBPS Rx, Mx
                        case 0xD1: // PSRLW Rx, Mx
                        case 0xD2: // PSRLD Rx, Mx
                        case 0xD3: // PSRLQ Rx, Mx
                        case 0xD4: // PADDQ Rx, Mx
                        case 0xD5: // MULLW Rx, Mx
                        case 0xD6: // MOVQ/MOVQ2DQ/MOVDQ2W
                        case 0xD7: // PMOVMSKB Rx, Mx
                        case 0xD8: // PSUBUSB Rx, Mx
                        case 0xD9: // PSUBUSW Rx, Mx
                        case 0xDA: // PMINUB Rx, Mx
                        case 0xDB: // PAND Rx, Mx
                        case 0xDC: // PADDUSB Rx, Mx
                        case 0xDD: // PADDUSW Rx, Mx
                        case 0xDE: // PMAXUB Rx, Mx
                        case 0xDF: // PANDN Rx, Mx
                        case 0xE0: // PAVGB Rx, Mx
                        case 0xE1: // PSRAW Rx, Mx
                        case 0xE2: // PSRAS Rx, Mx
                        case 0xE3: // PAVGW Rx, Mx
                        case 0xE4: // PMULHUW Rx, Mx
                        case 0xE5: // PMULHW Rx, Mx
                        case 0xE6: // CVTTPD2DQ/CVTDQ2PD/CVTPD2DQ Rx, Mx
                        case 0xE7: // MOVNTQ/MOVNTDQ Mx, Rx
                        case 0xE8: // PSUBSB Rx, Mx
                        case 0xE9: // PSUBSW Rx, Mx
                        case 0xEA: // PMINSW Rx, Mx
                        case 0xEB: // POR Rx, Mx
                        case 0xEC: // PADDSB Rx, Mx
                        case 0xED: // PADDSW Rx, Mx
                        case 0xEE: // PMAXSW Rx, Mx
                        case 0xEF: // PXOR Rx, Mx
                        case 0xF0: // LDDQU Rx, Mx
                        case 0xF1: // PSLLW Rx, Mx
                        case 0xF2: // PSLLD Rx, Mx
                        case 0xF3: // PSLLQ Rx, Mx
                        case 0xF4: // PMULUDQ Rx, Mx
                        case 0xF5: // PMADDWD Rx, Mx
                        case 0xF6: // PSADBW Rx, Mx
                        case 0xF7: // MASKMOVQ/MASKMOVDQU Rx, Mx
                        case 0xF8: // PSUBB Rx, Mx
                        case 0xF9: // PSUBW Rx, Mx
                        case 0xFA: // PSUBD Rx, Mx
                        case 0xFB: // PSUBQ Rx, Mx
                        case 0xFC: // PADDB Rx, Mx
                        case 0xFD: // PADDW Rx, Mx
                        case 0xFE: // PADDD Rx, Mx
                            break;
                        default:
                            unreachable;
                    }
                    break;
                }
                THROW_UD();
            case 0x07: case 0x17: case 0x1F: // POP seg
                ctx.index_seg(opcode >> 3) = ctx.POP();
                break;
            default:
                unreachable;
            fault:
                if constexpr (ctx.FAULTS_ARE_TRAPS) {
                    goto trap;
                }
                goto next_instr;
        }
    trap:
        ctx.ip = pc.offset;
    next_instr:
        ctx.execute_pending_interrupts();
    }
}