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
        this->reset_descriptors();
        this->reset_ip();
        this->pending_einterrupt = -1;
        this->pending_sinterrupt = -1;
    }

    inline constexpr void reset() {
        this->init();
        this->seg_override = -1;
        this->rep_type = NO_REP;
        this->halted = false;
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

#define ALWAYS_UD() { ctx.set_fault(IntUD); goto fault; }
#define THROW_UD() if constexpr (!ctx.NO_UD) { ctx.set_fault(IntUD); goto fault; } else
#define ALWAYS_UD_GRP() { ctx.set_fault(IntUD); return OP_FAULT; }
#define THROW_UD_GRP() if constexpr (!ctx.NO_UD) { ctx.set_fault(IntUD); return OP_FAULT; } else

#define ALWAYS_UD_WITHOUT_FLAG(...) if constexpr (!(__VA_ARGS__)) { ALWAYS_UD(); }
#define THROW_UD_WITHOUT_FLAG(...) if constexpr (!(__VA_ARGS__)) { THROW_UD(); } else
#define ALWAYS_UD_WITHOUT_FLAG_GRP(...) if constexpr (!(__VA_ARGS__)) { ALWAYS_UD_GRP(); }
#define THROW_UD_WITHOUT_FLAG_GRP(...) if constexpr (!(__VA_ARGS__)) { THROW_UD_GRP(); } else

#define ALWAYS_GP() { ctx.set_fault(IntGP); goto fault; }
#define ALWAYS_GP_GRP() { ctx.set_fault(IntGP); return OP_FAULT; }
#define GP_WITHOUT_CPL0() if (ctx.current_privilege_level() != 0) { ALWAYS_GP(); } else
#define GP_WITHOUT_CPL0_GRP() if (ctx.current_privilege_level() != 0) { ALWAYS_GP_GRP(); } else

    for (;;) {
        // Reset per-instruction states
        ctx.seg_override = -1;
        ctx.rep_type = NO_REP;
        ctx.lock = false;
        ctx.reset_prefixes();

        z86AddrCS pc = ctx.pc();
        uint8_t map = 0;
        // TODO: The 6 byte prefetch cache
        // TODO: Clock cycles
    prefix_byte:
        assume(map == 0);
    next_byte:
        uint8_t opcode_byte = pc.read_advance();
        //assume(!(map & 0xFF));
        uint32_t opcode = opcode_byte | (uint32_t)map << 8;
        assume(opcode <= UINT16_MAX);
        switch (opcode) {
            case 0x00: // ADD Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x01: // ADD Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x02: // ADD Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x03: // ADD Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.ADD(dst, src);
                    return OP_WRITE;
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
            case 0x06: case 0x0E: case 0x16: case 0x1E: // PUSH seg
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                ctx.PUSH(ctx.get_seg(opcode_byte >> 3));
                break;
            case 0x0F:
                if constexpr (ctx.OPCODES_80286) {
                    map = 1;
                    goto next_byte;
                }
                THROW_UD();
            case 0x07: case 0x17: case 0x1F: // POP seg
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                ctx.write_seg(opcode_byte >> 3, ctx.POP());
                break;
            case 0x08: // OR Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x09: // OR Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x0A: // OR Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x0B: // OR Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                    return OP_WRITE;
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
                    return OP_WRITE;
                }));
                break;
            case 0x11: // ADC Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x12: // ADC Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x13: // ADC Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                    return OP_WRITE;
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
                    return OP_WRITE;
                }));
                break;
            case 0x19: // SBB Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1A: // SBB Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1B: // SBB Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                    return OP_WRITE;
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
                    return OP_WRITE;
                }));
                break;
            case 0x21: // AND Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x22: // AND Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x23: // AND Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.AND(dst, src);
                    return OP_WRITE;
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
                ctx.set_seg_override((opcode_byte >> 3) & 3);
                goto prefix_byte;
            case 0x27: // DAA
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                ctx.DAA();
                break;
            case 0x28: // SUB Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x29: // SUB Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x2A: // SUB Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x2B: // SUB Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.SUB(dst, src);
                    return OP_WRITE;
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
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                ctx.DAS();
                break;
            case 0x30: // XOR Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x31: // XOR Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x32: // XOR Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x33: // XOR Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.XOR(dst, src);
                    return OP_WRITE;
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
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                ctx.AAA();
                break;
            case 0x38: // CMP Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return OP_NO_WRITE;
                }));
                break;
            case 0x39: // CMP Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return OP_NO_WRITE;
                }));
                break;
            case 0x3A: // CMP Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return OP_NO_WRITE;
                }));
                break;
            case 0x3B: // CMP Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto dst, auto src) regcall {
                    ctx.CMP(dst, src);
                    return OP_NO_WRITE;
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
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                ctx.AAS();
                break;
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: // INC reg
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        ctx.set_rex_bits(opcode);
                        goto prefix_byte;
                    }
                }
                ctx.INC(ctx.index_regMB<uint16_t>(opcode_byte & 7));
                break;
            case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: // DEC reg
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        ctx.set_rex_bits(opcode);
                        goto prefix_byte;
                    }
                }
                ctx.DEC(ctx.index_regMB<uint16_t>(opcode_byte & 7));
                break;
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: // PUSH reg
                if constexpr (ctx.OLD_PUSH_SP) {
                    ctx.PUSH(ctx.index_regMB<uint16_t>(opcode_byte & 7));
                }
                else {
                    auto temp = ctx.index_regMB<uint16_t>(opcode_byte & 7);
                    ctx.PUSH(temp);
                }
                break;
            case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: // POP reg
                ctx.index_regMB<uint16_t>(opcode_byte & 7) = ctx.POP();
                break;
            case 0x60: // PUSHA
                if constexpr (ctx.OPCODES_80186) {
                    if constexpr (ctx.LONG_MODE) {
                        if (ctx.is_long_mode()) {
                            THROW_UD();
                        }
                    }
                    ctx.PUSHA();
                    break;
                }
                THROW_UD();
            case 0x61:
                if constexpr (ctx.OPCODES_80186) {
                    if constexpr (ctx.LONG_MODE) {
                        if (ctx.is_long_mode()) {
                            THROW_UD();
                        }
                    }
                    ctx.POPA();
                    break;
                }
                THROW_UD();
            case 0x70: // JO Jb
            case 0x71: // JNO Jb
                ctx.JCC<CondNO, true>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x62: // BOUND Rv, Mv2
                if constexpr (ctx.OPCODES_80186) {
                    if constexpr (ctx.LONG_MODE) {
                        if (ctx.is_long_mode()) {
                            // Parse EVEX
                            THROW_UD();
                        }
                    }
                    FAULT_CHECK(ctx.binopRM2(pc, [](auto index, auto lower, auto upper) regcall {
                        return ctx.BOUND(index, lower, upper);
                    }));
                    break;
                }
                THROW_UD();
            case 0x63: // ARPL Mw, Rw
                if constexpr (ctx.PROTECTED_MODE && ctx.OPCODES_80286) {
                    // Not valid in real mode apparently
                    if (ctx.is_real_mode()) {
                        THROW_UD();
                    }
                    if constexpr (ctx.LONG_MODE) {
                        if (ctx.is_long_mode()) {
                            // MOVSXD Rv, Mv
                            THROW_UD();
                        }
                    }
                    // TODO
                }
                else if constexpr (ctx.OPCODES_V20) {
                    // ???
                }
                THROW_UD();
            case 0x72: // JC Jb
            case 0x73: // JNC Jb
                ctx.JCC<CondNC, true>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x64: case 0x65: // FS/GS prefixes
                if constexpr (ctx.OPCODES_80386) {
                    ctx.set_seg_override(opcode_byte & 0xF);
                    goto prefix_byte;
                }
                else if constexpr (ctx.OPCODES_V20) {
                    ctx.set_repc_type(opcode_byte);
                    goto prefix_byte;
                }
                THROW_UD();
            case 0x74: // JZ Jb
            case 0x75: // JNZ Jb
                ctx.JCC<CondNZ, true>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x66: // Data size prefix
                if constexpr (ctx.max_bits > 16) {
                    ctx.data_size_prefix();
                    goto prefix_byte;
                }
                if constexpr (ctx.OPCODES_V20) {
                    goto modrm_nop;
                }
                THROW_UD();
            case 0x67: // Addr size prefix
                if constexpr (ctx.max_bits > 16) {
                    ctx.addr_size_prefix();
                    goto prefix_byte;
                }
                if constexpr (ctx.OPCODES_V20) {
                    goto modrm_nop;
                }
                THROW_UD();
            case 0x76: // JBE Jb
            case 0x77: // JA Jb
                ctx.JCC<CondA, true>(pc, opcode_byte & 1);
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
                ctx.JCC<CondNS, true>(pc, opcode_byte & 1);
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
                ctx.JCC<CondNP, true>(pc, opcode_byte & 1);
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
                ctx.JCC<CondGE, true>(pc, opcode_byte & 1);
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
                ctx.JCC<CondG, true>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x82:
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
            case 0x80: // GRP1 Mb, Ib
                FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                    uint8_t val = pc.read<int8_t>();
                    switch (r) {
                        default: unreachable;
                        case 0: ctx.ADD(dst, val); return OP_WRITE;
                        case 1: ctx.OR(dst, val); return OP_WRITE;
                        case 2: ctx.ADC(dst, val); return OP_WRITE;
                        case 3: ctx.SBB(dst, val); return OP_WRITE;
                        case 4: ctx.AND(dst, val); return OP_WRITE;
                        case 5: ctx.SUB(dst, val); return OP_WRITE;
                        case 6: ctx.XOR(dst, val); return OP_WRITE;
                        case 7: ctx.CMP(dst, val); return OP_NO_WRITE;
                    }
                }));
                ++pc;
                break;
            case 0x81: // GRP1 Ev, Is
                FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                    int32_t val = pc.read_advance_Is();
                    switch (r) {
                        default: unreachable;
                        case 0: ctx.ADD(dst, val); return OP_WRITE;
                        case 1: ctx.OR(dst, val); return OP_WRITE;
                        case 2: ctx.ADC(dst, val); return OP_WRITE;
                        case 3: ctx.SBB(dst, val); return OP_WRITE;
                        case 4: ctx.AND(dst, val); return OP_WRITE;
                        case 5: ctx.SUB(dst, val); return OP_WRITE;
                        case 6: ctx.XOR(dst, val); return OP_WRITE;
                        case 7: ctx.CMP(dst, val); return OP_NO_WRITE;
                    }
                }));
                break;
            case 0x83: // GRP1 Ev, Ib
                FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                    int32_t val = pc.read<int8_t>();
                    switch (r) {
                        default: unreachable;
                        case 0: ctx.ADD(dst, val); return OP_WRITE;
                        case 1: ctx.OR(dst, val); return OP_WRITE;
                        case 2: ctx.ADC(dst, val); return OP_WRITE;
                        case 3: ctx.SBB(dst, val); return OP_WRITE;
                        case 4: ctx.AND(dst, val); return OP_WRITE;
                        case 5: ctx.SUB(dst, val); return OP_WRITE;
                        case 6: ctx.XOR(dst, val); return OP_WRITE;
                        case 7: ctx.CMP(dst, val); return OP_NO_WRITE;
                    }
                }));
                ++pc;
                break;
            case 0x84: // TEST Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                    return OP_NO_WRITE;
                }));
                break;
            case 0x85: // TEST Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                    return OP_NO_WRITE;
                }));
                break;
            case 0x86: // XCHG Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto& src) regcall {
                    ctx.XCHG(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x87: // XCHG Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto& src) regcall {
                    ctx.XCHG(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x88: // MOV Mb, Rb
                FAULT_CHECK(ctx.binopMR<true>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x89: // MOV Mv, Rv
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8A: // MOV Rb, Mb
                FAULT_CHECK(ctx.binopRM<true>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8B: // MOV Rv, Mv
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8C: // MOV M, seg
                FAULT_CHECK(ctx.binopMS(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
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
                    return OP_WRITE;
                }));
                break;
            case 0x8F: // GRP1A (Supposedly this does mystery jank if R != 0)
                FAULT_CHECK(ctx.unopM(pc, [](auto src, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                            THROW_UD_GRP();
                            // TODO: ???
                        case 0:
                            ctx.PUSH(src);
                            return OP_NO_WRITE; // Writes to stack, not src
                    }
                }));
                break;
            case 0x90: // NOP, XCHG RAX, R8
                if (!ctx.rex_bits.B()) {
                    // NOP
                    break;
                }
            case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // XCHG AX, reg
                ctx.binopAR(opcode_byte & 7, [](auto& dst, auto& src) regcall {
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
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
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
                ctx.MOV_RI<true>(pc, opcode_byte & 7);
                break;
            case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: // MOV reg, Iv
                ctx.MOV_RI(pc, opcode_byte & 7);
                break;
            case 0xC0: // GRP2 Mb, Ib
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                        uint8_t count = pc.read<uint8_t>();
                        switch (r) {
                            default: unreachable;
                            case 0: ctx.ROL(dst, count); return OP_WRITE;
                            case 1: ctx.ROR(dst, count); return OP_WRITE;
                            case 2: ctx.RCL(dst, count); return OP_WRITE;
                            case 3: ctx.RCR(dst, count); return OP_WRITE;
                            case 4: ctx.SHL(dst, count); return OP_WRITE;
                            case 5: ctx.SHR(dst, count); return OP_WRITE;
                            case 6: ctx.SETMO(dst, count); return OP_WRITE;
                            case 7: ctx.SAR(dst, count); return OP_WRITE;
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
                    FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                        uint8_t count = pc.read<uint8_t>();
                        switch (r) {
                            default: unreachable;
                            case 0: ctx.ROL(dst, count); return OP_WRITE;
                            case 1: ctx.ROR(dst, count); return OP_WRITE;
                            case 2: ctx.RCL(dst, count); return OP_WRITE;
                            case 3: ctx.RCR(dst, count); return OP_WRITE;
                            case 4: ctx.SHL(dst, count); return OP_WRITE;
                            case 5: ctx.SHR(dst, count); return OP_WRITE;
                            case 6: ctx.SETMO(dst, count); return OP_WRITE;
                            case 7: ctx.SAR(dst, count); return OP_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                THROW_UD();
            case 0xC3: // RET
                ctx.RET();
                goto next_instr;
            case 0xC4: // LES Rv, Mf
                FAULT_CHECK(ctx.binopRMF(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.es = src >> (bitsof(src) >> 1);
                    return true;
                }));
                break;
            case 0xC5: // LDS Rv, Mf
                FAULT_CHECK(ctx.binopRMF(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.ds = src >> (bitsof(src) >> 1);
                    return true;
                }));
                break;
            case 0xC6: // GRP11 Ib (Supposedly this just ignores R bits)
                FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                            THROW_UD_GRP();
                        case 0: // MOV Mb, Ib
                            dst = pc.read<int8_t>();
                            return OP_WRITE;
                        default:
                            unreachable;
                    }
                }));
                ++pc;
                break;
            case 0xC7: // GRP11 Is (Supposedly this just ignores R bits)
                FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                            THROW_UD_GRP();
                        case 0: // MOV Mv, Is
                            dst = pc.read_advance_Is();
                            return OP_WRITE;
                        default:
                            unreachable;
                    }
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
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
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
                        default: unreachable;
                        case 0: ctx.ROL(dst, 1); return OP_WRITE;
                        case 1: ctx.ROR(dst, 1); return OP_WRITE;
                        case 2: ctx.RCL(dst, 1); return OP_WRITE;
                        case 3: ctx.RCR(dst, 1); return OP_WRITE;
                        case 4: ctx.SHL(dst, 1); return OP_WRITE;
                        case 5: ctx.SHR(dst, 1); return OP_WRITE;
                        case 6: ctx.SETMO(dst, 1); return OP_WRITE;
                        case 7: ctx.SAR(dst, 1); return OP_WRITE;
                    }
                }));
                break;
            case 0xD1: // GRP2 Mv, 1
                FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 0: ctx.ROL(dst, 1); return OP_WRITE;
                        case 1: ctx.ROR(dst, 1); return OP_WRITE;
                        case 2: ctx.RCL(dst, 1); return OP_WRITE;
                        case 3: ctx.RCR(dst, 1); return OP_WRITE;
                        case 4: ctx.SHL(dst, 1); return OP_WRITE;
                        case 5: ctx.SHR(dst, 1); return OP_WRITE;
                        case 6: ctx.SETMO(dst, 1); return OP_WRITE;
                        case 7: ctx.SAR(dst, 1); return OP_WRITE;
                    }
                }));
                break;
            case 0xD2: // GRP2 Mb, CL
                FAULT_CHECK(ctx.unopM<true>(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 0: ctx.ROL(dst, ctx.cl); return OP_WRITE;
                        case 1: ctx.ROR(dst, ctx.cl); return OP_WRITE;
                        case 2: ctx.RCL(dst, ctx.cl); return OP_WRITE;
                        case 3: ctx.RCR(dst, ctx.cl); return OP_WRITE;
                        case 4: ctx.SHL(dst, ctx.cl); return OP_WRITE;
                        case 5: ctx.SHR(dst, ctx.cl); return OP_WRITE;
                        case 6: ctx.SETMO(dst, ctx.cl); return OP_WRITE;
                        case 7: ctx.SAR(dst, ctx.cl); return OP_WRITE;
                    }
                }));
                break;
            case 0xD3: // GRP2 Mv, CL
                FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 0: ctx.ROL(dst, ctx.cl); return OP_WRITE;
                        case 1: ctx.ROR(dst, ctx.cl); return OP_WRITE;
                        case 2: ctx.RCL(dst, ctx.cl); return OP_WRITE;
                        case 3: ctx.RCR(dst, ctx.cl); return OP_WRITE;
                        case 4: ctx.SHL(dst, ctx.cl); return OP_WRITE;
                        case 5: ctx.SHR(dst, ctx.cl); return OP_WRITE;
                        case 6: ctx.SETMO(dst, ctx.cl); return OP_WRITE;
                        case 7: ctx.SAR(dst, ctx.cl); return OP_WRITE;
                    }
                }));
                break;
            case 0xD4: // AAM Ib
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                FAULT_CHECK(ctx.AAM(pc.read_advance<uint8_t>()));
                break;
            case 0xD5: // AAD Ib
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                ctx.AAD(pc.read_advance<uint8_t>());
                break;
            case 0xD6: // SALC
                if constexpr (!ctx.OPCODES_V20) {
                    if constexpr (ctx.LONG_MODE) {
                        if (ctx.is_long_mode()) {
                            // Screw larabee junk
                            THROW_UD();
                        }
                    }
                    ctx.al = ctx.carry ? -1 : 0;
                    break;
                }
            case 0xD7: { // XLAT
                z86Addr addr = ctx.addr(DS, ctx.bx + ctx.al);
                ctx.al = addr.read<uint8_t>();
                break;
            }
            case 0xD8: case 0xDA: case 0xDC: case 0xDE:
            case 0xD9: case 0xDB: case 0xDD: case 0xDF:
                if constexpr (!ctx.CPUID_X87) {
                    goto modrm_nop;
                }
                else {
                    ModRM modrm = pc.read_advance<ModRM>();
                    ctx.fop_set(opcode, modrm.raw);
                    uint8_t r = modrm.R();
                    uint8_t m;
                    long double* lhs;
                    long double rhs;
                    if (modrm.is_mem()) {
                        z86Addr data_addr = modrm.parse_memM(pc);
                        switch (opcode & 7) {
                            default: unreachable;
                            case 0: // D8 ALU mem f32
                                rhs = data_addr.read<float>();
                                goto x87_math_op;
                            case 1: // D9 MOV mem
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FLD f32
                                        rhs = data_addr.read<float>();
                                        goto fld;
                                    case 1:
                                        ALWAYS_UD();
                                    case 2: // FST f32
                                        data_addr.write<float>(ctx.FTOP());
                                        break;
                                    case 3: // FSTP f32
                                        data_addr.write<float>(ctx.FPOP());
                                        break;
                                    case 4: // FLDENV
                                        break;
                                    case 5: // FLDCW Mw
                                        break;
                                    case 6: // FSTENV
                                        break;
                                    case 7: // FSTCW Mw
                                        break;
                                }
                                break;
                            case 2: // DA ALU mem i32
                                rhs = data_addr.read<int32_t>();
                                goto x87_math_op;
                            case 3: // DB MOV mem
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FILD i32
                                        rhs = data_addr.read<int32_t>();
                                        goto fld;
                                    case 2: // FIST i32
                                        data_addr.write<int32_t>(ctx.FTOP());
                                        break;
                                    case 1: // FISTTP i32
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                                    case 3: // FISTP i32
                                        data_addr.write<int32_t>(ctx.FPOP());
                                        break;
                                    case 4:
                                        ALWAYS_UD();
                                    case 5:
                                        rhs = data_addr.read<long double>();
                                        goto fld;
                                    case 6:
                                        ALWAYS_UD();
                                    case 7:
                                        data_addr.write<long double>(ctx.FPOP());
                                        break;
                                }
                                break;
                            case 4: // DC ALU mem f64
                                rhs = data_addr.read<double>();
                                goto x87_math_op;
                            case 5: // DD MOV mem
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FLD f64
                                        rhs = data_addr.read<double>();
                                        goto fld;
                                    case 1: // FISTTP i64
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                                        goto fistp64;
                                    case 2: // FST f64
                                        data_addr.write<double>(ctx.FTOP());
                                        break;
                                    case 3: // FSTP f64
                                        data_addr.write<double>(ctx.FPOP());
                                        break;
                                    case 4: // FRSTOR
                                    case 5:
                                        ALWAYS_UD();
                                    case 6: // FSAVE
                                    case 7: // FSTSW Mw
                                        break;
                                }
                                break;
                            case 6: // DE ALU mem i16
                                rhs = data_addr.read<int16_t>();
                                goto x87_math_op;
                            case 7: // DF MOV mem
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FILD i16
                                        rhs = data_addr.read<int16_t>();
                                        goto fld;
                                    case 2: // FIST i16
                                        data_addr.write<int16_t>(ctx.FTOP());
                                        break;
                                    case 1: // FISTTP i16
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                                    case 3: // FISTP i16
                                        data_addr.write<int16_t>(ctx.FPOP());
                                        break;
                                    case 4: // FBLD
                                        break;
                                    case 5: // FILD i64
                                        rhs = data_addr.read<int64_t>();
                                        goto fld;
                                    case 6: // FBSTP
                                        break;
                                    case 7: fistp64: // FISTP i64
                                        data_addr.write<int64_t>(ctx.FPOP());
                                        break;
                                }
                                break;
                        }
                    }
                    else {
                        m = modrm.M();
                        switch (opcode & 7) {
                            default: unreachable;
                            case 0: // D8 ALU reg
                                rhs = ctx.index_st_reg(m);
                            x87_math_op:
                                lhs = &ctx.FTOP();
                                switch (r) {
                                    default: unreachable;
                                    case 0: fadd: // FADD ST(0), rhs
                                        ctx.FADD(*lhs, rhs);
                                        break;
                                    case 1: fmul: // FMUL ST(0), rhs
                                        ctx.FMUL(*lhs, rhs);
                                        break;
                                    case 2: fcom: // FCOM ST(0), rhs
                                        ctx.FCOM(*lhs, rhs);
                                        break;
                                    case 3: fcomp: // FCOMP ST(0), rhs
                                        ctx.FCOM(*lhs, rhs);
                                        ctx.FINCSTP();
                                        break;
                                    case 4: fsub: // FSUB ST(0), rhs
                                        ctx.FSUB(*lhs, rhs);
                                        break;
                                    case 5: fsubr: // FSUBR ST(0), rhs
                                        ctx.FSUBR(*lhs, rhs);
                                        break;
                                    case 6: fdiv: // FDIV ST(0), rhs
                                        ctx.FDIV(*lhs, rhs);
                                        break;
                                    case 7: fdivr: // FDIVR ST(0), rhs
                                        ctx.FDIVR(*lhs, rhs);
                                        break;
                                }
                                break;
                            case 1: // D9 BS
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FLD ST(m)
                                        rhs = ctx.index_st_reg(m);
                                    fld:
                                        ctx.FPUSH(rhs);
                                        break;
                                    case 1: fxch: // FXCH ST(0), ST(m)
                                        ctx.FXCH(ctx.index_st_reg(m));
                                        break;
                                    case 2:
                                        switch (m) {
                                            default: unreachable;
                                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                                THROW_UD();
                                            case 0: // FNOP
                                                break;
                                        }
                                        break;
                                    case 3: // FSTP1 ST(m)
                                        goto fstp;
                                    case 4:
                                        switch (m) {
                                            default: unreachable;
                                            case 0: // FCHS
                                                break;
                                            case 1: // FABS
                                                break;
                                            case 2: // Cyrix?
                                            case 3:
                                                ALWAYS_UD();
                                            case 4: // FTST
                                                break;
                                            case 5: // FXAM
                                                break;
                                            case 6: // FTSTP
                                            case 7:
                                                ALWAYS_UD();
                                        }
                                        break;
                                    case 5:
                                        switch (m) {
                                            default: unreachable;
                                            case 0: // FLD1
                                                rhs = 1.0L;
                                                goto fld;
                                            case 1: // FLDL2T
                                            case 2: // FLDL2E
                                            case 3: // FLDPI
                                            case 4: // FLDLG2
                                            case 5: // FLDLN2
                                                break;
                                            case 6: // FLDZ
                                                rhs = 0.0L;
                                                goto fld;
                                            case 7:
                                                ALWAYS_UD();
                                        }
                                        break;
                                    case 6:
                                        switch (m) {
                                            default: unreachable;
                                            case 0: // F2XM1
                                            case 1: // FYL2X
                                            case 2: // FPTAN
                                            case 3: // FPATAN
                                            case 4: // FXTRACT
                                                break;
                                            case 5: // FPREM1
                                                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                                                break;
                                            case 6: // FDECSTP
                                                ctx.FDECSTP();
                                                break;
                                            case 7: // FINCSTP
                                                ctx.FINCSTP();
                                                break;
                                        }
                                        break;
                                    case 7:
                                        switch (m) {
                                            default: unreachable;
                                            case 0: // FPREM
                                            case 1: // FYL2XP1
                                            case 2: // FSQRT
                                                break;
                                            case 3: // FSINCOS
                                                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                                                break;
                                            case 4: // FRNDINT
                                            case 5: // FSCALE
                                                break;
                                            case 6: // FSIN
                                                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                                                break;
                                            case 7: // FCOS
                                                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                                                break;
                                        }
                                        break;
                                }
                                break;
                            case 2: // DA FCMOV + FUCOMPP
                                switch (r) {
                                    default: unreachable;
                                    case 0: fcmovb: // FCMOVB ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                                        ctx.FCMOVCC<CondNB>(ctx.index_st_reg(m), opcode & 1);
                                        break;
                                    case 1: fcmove: // FCMOVE ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                                        ctx.FCMOVCC<CondNE>(ctx.index_st_reg(m), opcode & 1);
                                        break;
                                    case 2: fcmovbe: // FCMOVBE ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                                        ctx.FCMOVCC<CondNBE>(ctx.index_st_reg(m), opcode & 1);
                                        break;
                                    case 3: fcmovu: // FCMOVU ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                                        ctx.FCMOVCC<CondNP>(ctx.index_st_reg(m), opcode & 1);
                                        break;
                                    case 4:
                                        ALWAYS_UD();
                                    case 5:
                                        if (r == 1) { // FUCOMPP
                                            THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                                            ctx.FUCOM(ctx.FTOP(), ctx.index_st_reg(1));
                                            ctx.FINCSTP();
                                            ctx.FINCSTP();
                                            break;
                                        }
                                    case 6:
                                    case 7:
                                        ALWAYS_UD();
                                }
                                break;
                            case 3: // DB FCMOV + BS
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FCMOVNB ST(0), ST(m)
                                        goto fcmovb;
                                    case 1: // FCMOVNE ST(0), ST(m)
                                        goto fcmove;
                                    case 2: // FCMOVNBE ST(0), ST(m)
                                        goto fcmovbe;
                                    case 3: // FCMOVNU ST(0), ST(m)
                                        goto fcmovu;
                                    case 4:
                                        switch (m) {
                                            default: unreachable;
                                            case 0: // FENI
                                                if constexpr (!ctx.OPCODES_80286) {

                                                }
                                                break;
                                            case 1: // FDISI
                                                if constexpr (!ctx.OPCODES_80286) {

                                                }
                                                break;
                                            case 2: // FCLEX
                                            case 3: // FINIT
                                                break;
                                            case 4: // FSETPM
                                                if constexpr (!ctx.OPCODES_80386) {

                                                }
                                                break;
                                            case 5: // FRSTPM
                                                if constexpr (!ctx.OPCODES_80386) {

                                                }
                                                break;
                                            case 6:
                                            case 7:
                                                ALWAYS_UD();
                                        }
                                        break;
                                    case 5: // FUCOMI ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.OPCODES_P6);
                                        ctx.FUCOMI(ctx.index_st_reg(m));
                                        break;
                                    case 6: // FCMOI ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.OPCODES_P6);
                                        ctx.FCOMI(ctx.index_st_reg(m));
                                        break;
                                    case 7:
                                        if (m == 4) { // FRINT2

                                            break;
                                        }
                                        ALWAYS_UD();
                                }
                                break;
                            case 4: // DC ALU reg reverse
                                lhs = &ctx.index_st_reg(m);
                                rhs = ctx.FTOP();
                                switch (r) {
                                    default: unreachable;
                                    case 0: goto fadd;
                                    case 1: goto fmul;
                                    case 2: goto fcom;
                                    case 3: goto fcomp;
                                    case 4: goto fsubr;
                                    case 5: goto fsub;
                                    case 6: goto fdivr;
                                    case 7: goto fdiv;
                                }
                            case 5: // DD misc
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FFREE ST(m)
                                        ctx.FFREE(m);
                                        break;
                                    case 1: // FXCH4 ST(0), ST(m)
                                        goto fxch;
                                    case 2: fst: // FST ST(m)
                                        ctx.index_st_reg(m) = ctx.FTOP();
                                        break;
                                    case 3: fstp: // FSTP ST(m)
                                        ctx.index_st_reg(m) = ctx.FTOP();
                                        ctx.FINCSTP();
                                        break;
                                    case 4: // FUCOM ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                                        ctx.FUCOM(ctx.index_st_reg(m));
                                        break;
                                    case 5: // FUCOMP ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                                        ctx.FUCOM(ctx.index_st_reg(m));
                                        ctx.FINCSTP();
                                        break;
                                    case 6:
                                        ALWAYS_UD();
                                    case 7:
                                        if (m == 4) { // FRICHOP

                                            break;
                                        }
                                        ALWAYS_UD();
                                }
                                break;
                            case 6: // DE ALU reg pop
                                lhs = &ctx.index_st_reg(m);
                                rhs = ctx.FTOP();
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FADDP ST(m), ST(0)
                                        ctx.FADD(*lhs, rhs);
                                        break;
                                    case 1: // FMULP ST(m), ST(0)
                                        ctx.FMUL(*lhs, rhs);
                                        break;
                                    case 2: // FCOMP ST(m), ST(0)
                                        ctx.FCOM(*lhs, rhs);
                                        break;
                                    case 3:
                                        switch (m) {
                                            default: unreachable;
                                            case 0: case 2: case 3: case 4: case 5: case 6: case 7:
                                                THROW_UD();
                                            case 1: // FCOMPP
                                                ctx.FCOM(*lhs, rhs);
                                                ctx.FINCSTP();
                                                break;
                                        }
                                        break;
                                    case 4: // FSUBRP ST(m), ST(0)
                                        ctx.FSUBR(*lhs, rhs);
                                        break;
                                    case 5: // FSUB ST(m), ST(0)
                                        ctx.FSUB(*lhs, rhs);
                                        break;
                                    case 6: // FDIVRP ST(m), ST(0)
                                        ctx.FDIVR(*lhs, rhs);
                                        break;
                                    case 7: // FDIVP ST(m), ST(0)
                                        ctx.FDIV(*lhs, rhs);
                                        break;
                                }
                                ctx.FINCSTP();
                                break;
                            case 7: // DF misc
                                switch (r) {
                                    default: unreachable;
                                    case 0: // FFREEP ST(m)
                                        ctx.FFREE(m);
                                        ctx.FINCSTP();
                                        break;
                                    case 1: // FXCH7 ST(0), ST(m)
                                        goto fxch;
                                    case 2: // FSTP8 ST(m)
                                    case 3: // FSTP9 ST(m)
                                        goto fstp;
                                    case 4:
                                        switch (m) {
                                            default: unreachable;
                                            case 0: // FSTSW AX
                                                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80286);
                                                break;
                                            case 1: // FSTDW AX
                                            case 2: // FSTSG AX
                                            case 3:
                                            case 4:
                                            case 5:
                                            case 6:
                                            case 7:
                                                ALWAYS_UD();
                                        }
                                        break;
                                    case 5: // FUCOMIP ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.OPCODES_P6);
                                        ctx.FUCOMI(ctx.index_st_reg(m));
                                        ctx.FINCSTP();
                                        break;
                                    case 6: // FCOMIP ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.OPCODES_P6);
                                        ctx.FCOMI(ctx.index_st_reg(m));
                                        ctx.FINCSTP();
                                        break;
                                    case 7:
                                        if (m == 4) { // FRINEAR

                                            break;
                                        }
                                        ALWAYS_UD();
                                }
                                break;
                        }
                    }
                }
                break;
            x87: // ESC x87
                // NOP :D
                pc += pc.read<ModRM>().length(pc);
                break;
            case 0xE0: // LOOPNZ Jb
            case 0xE1: // LOOPZ Jb
                ctx.LOOPCC(pc, opcode_byte & 1);
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
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
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
            case 0xF1:
                if constexpr (ctx.OPCODES_80386) { // INT1
                    ctx.set_trap(IntDB);
                    goto trap;
                }
                if constexpr (ctx.OPCODES_80286) {
                    // Stupid STOREALL prefix, just going to ignore this
                    goto prefix_byte;
                }
                if constexpr (!ctx.OPCODES_V20) {
                    THROW_UD();
                }
            case 0xF0: // LOCK
                ctx.set_lock();
                goto prefix_byte;
            case 0xF2: case 0xF3: // REPNE, REP
                ctx.set_rep_type(opcode_byte);
                goto prefix_byte;
            case 0xF4: // HLT
                GP_WITHOUT_CPL0();
                ctx.halted = true;
                break;
            case 0xF5: // CMC
                ctx.carry ^= 1;
                break;
            case 0xF6: // GRP3 Mb
                FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& val, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 0: case 1: // TEST Mb, Ib
                            ctx.TEST(val, pc.read<uint8_t>());
                            return OP_NO_WRITE;
                        case 2: // NOT Mb
                            ctx.NOT(val);
                            return OP_WRITE;
                        case 3: // NEG Mb
                            ctx.NEG(val);
                            return OP_WRITE;
                        case 4: // MUL Mb
                            ctx.MUL(val);
                            return OP_NOT_MEM;
                        case 5: // IMUL Mb
                            ctx.IMUL(val);
                            return OP_NOT_MEM;
                        case 6: // DIV Mb
                            return ctx.DIV(val) ? OP_FAULT : OP_NOT_MEM;
                        case 7: // IDIV Mb
                            return ctx.IDIV(val) ? OP_FAULT : OP_NOT_MEM;
                    }
                }));
                ++pc;
                break;
            case 0xF7: // GRP3 Mv
                FAULT_CHECK(ctx.unopM(pc, [&](auto& val, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 0: case 1: // TEST Mv, Is
                            ctx.TEST(val, pc.read_advance_Is());
                            return OP_NO_WRITE;
                        case 2: // NOT Mv
                            ctx.NOT(val);
                            return OP_WRITE;
                        case 3: // NEG Mv
                            ctx.NEG(val);
                            return OP_WRITE;
                        case 4: // MUL Mv
                            ctx.MUL(val);
                            return OP_NOT_MEM;
                        case 5: // IMUL Mv
                            ctx.IMUL(val);
                            return OP_NOT_MEM;
                        case 6: // DIV Mv
                            return ctx.DIV(val) ? OP_FAULT : OP_NOT_MEM;
                        case 7: // IDIV Mv
                            return ctx.IDIV(val) ? OP_FAULT : OP_NOT_MEM;
                    }
                }));
                break;
            case 0xF8: case 0xF9: // CLC, STC
                ctx.carry = opcode_byte & 1;
                break;
            case 0xFA: case 0xFB: // CLI, STI
                ctx.interrupt = opcode_byte & 1;
                break;
            case 0xFC: case 0xFD: // CLD, STD
                ctx.direction = opcode_byte & 1;
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
            case 0x100: // GRP6
                FAULT_CHECK(ctx.unopMW(pc, [](auto& dst, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 0: // SLDT Mw
                        case 1: // STR Mw
                            dst = ctx.get_control_seg(r & 1);
                            return OP_WRITE;
                        case 2: // LLDT Mw
                        case 3: // LTR Mw
                            ctx.write_control_seg(r & 1, dst);
                            return OP_NO_WRITE;
                        case 4: // VERR Mw
                        case 5: // VERW Mw
                            return OP_NO_WRITE;
                        case 6:
                        case 7:
                            ALWAYS_UD_GRP();
                    }
                }));
                break;
            case 0x101: // GRP7
                FAULT_CHECK(ctx.unopMM(pc,
                    [](auto data_addr_raw, uint8_t r) regcall {
                        z86Addr data_addr = data_addr_raw;
                        switch (r) {
                            default: unreachable;
                            case 0: // SGDT M
                            case 1: // SIDT M
                            {
                                data_addr.write<uint16_t>(ctx.get_descriptor_table_limit(r & 1));
                                auto base = ctx.get_descriptor_table_base(r & 1);
                                if constexpr (ctx.LONG_MODE) {
                                    if (ctx.is_long_mode()) {
                                        data_addr.write<uint64_t>(base, 2);
                                        return OP_NO_FAULT;
                                    }
                                }
                                data_addr.write<uint32_t>(base, 2);
                                return OP_NO_FAULT;
                            }
                            case 2: // LGDT M
                            case 3: // LIDT M
                            {
                                uint16_t limit = data_addr.read<uint16_t>();
                                decltype(ctx.get_descriptor_table_base(0)) base;
                                if constexpr (ctx.LONG_MODE) {
                                    if (ctx.is_long_mode()) {
                                        base = data_addr.read<uint64_t>(2);
                                        goto load_table;
                                    }
                                }
                                base = data_addr.read<uint32_t>(2);
                            load_table:
                                ctx.load_descriptor_table(r & 1, limit, base);
                                return OP_NO_FAULT;
                            }
                            case 4: // SMSW Mw
                                data_addr.write<uint16_t>(ctx.get_machine_status_word());
                                return OP_NO_FAULT;
                            case 5:
                                ALWAYS_UD_GRP();
                            case 6: // LMSW Mw
                                ctx.set_machine_status_word(data_addr.read<uint16_t>());
                                return OP_NO_FAULT;
                            case 7: // INVLPG M
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.OPCODES_80486);
                                return OP_NO_FAULT;
                        }
                    },
                    [](auto& src, uint8_t r) regcall{
                        switch (r) {
                            default: unreachable;
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                                ALWAYS_UD_GRP();
                            case 4: // SMSW Rv
                                src = ctx.get_machine_status_word();
                                return OP_NO_FAULT;
                            case 5:
                                ALWAYS_UD_GRP();
                            case 6: // LMSW Rv
                                ctx.set_machine_status_word(src);
                                return OP_NO_FAULT;
                            case 7:
                                ALWAYS_UD_GRP();
                        }
                    }
                ));
                break;
            case 0x102: // LAR Rv, Mv
            case 0x103: // LSL Rv, Mv
            case 0x104: // STOREALL
            case 0x105: // SYSCALL, LOADALL2
                break;
            case 0x106: // CLTS
                GP_WITHOUT_CPL0();

                break;
            case 0x107: // SYSRET, LOADALL3
            case 0x108: // INVD (486)
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80486);
                GP_WITHOUT_CPL0();
                break;
            case 0x109: // WBINVD (486)
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80486);
                GP_WITHOUT_CPL0();
                break;
            case 0x10A: // CL1INVMB (wtf)
                break;
            case 0x10B: // UD2
            case 0x1B9: // UD1
            case 0x1FF: // UD0
                ALWAYS_UD();
            case 0x1A6: // XBTS
            case 0x1A7: // IBTS
            case 0x10C: case 0x125: case 0x127: case 0x136: case 0x137:
            case 0x139: case 0x13B: case 0x13C: case 0x13E: case 0x13F:
            case 0x17A: case 0x17B:
                THROW_UD();
                break;
            case 0x10D: // PREFETCHx
            case 0x10E: // FEMMS
            case 0x10F: // 3DNow!
                break;
            case 0x110:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // MOVUPS Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // MOVUPD Rx, Mx (SSE2)
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix: // MOVSS Rx, Mx (SSE)
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case OpcodeF2Prefix: // MOVSD Rx, Mx (SSE2)
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<true>(pc, [](auto dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // TEST1 Mb, CL
                                ctx.TEST1(dst, ctx.cl);
                                return OP_NO_WRITE;
                        }
                    }));
                }
                break;
            case 0x111:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // MOVUPS Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // MOVUPD Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix: // MOVSS Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case OpcodeF2Prefix: // MOVSD Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM(pc, [](auto dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // TEST1 Mv, CL
                                ctx.TEST1(dst, ctx.cl);
                                return OP_NO_WRITE;
                        }
                    }));
                }
                break;
            case 0x112:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2 || ctx.CPUID_SSE3) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // MOVLPS Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // MOVLPD Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix: // MOVSLDUP Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                            break;
                        case OpcodeF2Prefix: // MOVDDUP Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                            break;
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<true>(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // CLR1 Mb, CL
                                ctx.CLR1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x113:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // MOVLPS Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // MOVLPD Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix:
                        case OpcodeF2Prefix:
                            ALWAYS_UD();
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // CLR1 Mv, CL
                                ctx.CLR1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x114:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // UNPCKLPS Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // UNPCKLPD Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix:
                        case OpcodeF2Prefix:
                            ALWAYS_UD();
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<true>(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // SET1 Mb, CL
                                ctx.SET1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x115:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // UNPCKLPS Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // UNPCKLPD Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix:
                        case OpcodeF2Prefix:
                            ALWAYS_UD();
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // SET1 Mv, CL
                                ctx.SET1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x116:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2 || ctx.CPUID_SSE3) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // MOVHPS Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // MOVHPD Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix: // MOVSHDUP Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                            break;
                        case OpcodeF2Prefix:
                            ALWAYS_UD();
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<true>(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // NOT1 Mb, CL
                                ctx.NOT1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x117:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // MOVHPS Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // MOVHPD Mx, Rx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix:
                        case OpcodeF2Prefix:
                            ALWAYS_UD();
                    }
                }
                else { // NOT1 Mv, CL
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0:
                                ctx.NOT1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x118:
                if constexpr (ctx.OPCODES_V20) {
                    FAULT_CHECK(ctx.unopM<true>(pc, [&](auto dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // TEST1 Mb, Ib
                                ctx.TEST1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                goto hint_nop;
            case 0x119:
                if constexpr (ctx.OPCODES_V20) {
                    FAULT_CHECK(ctx.unopM(pc, [&](auto dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // TEST1 Mv, Ib
                                ctx.TEST1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                goto hint_nop;
            case 0x11A:
                if constexpr (ctx.OPCODES_V20) { 
                    FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // CLR1 Mb, Ib
                                ctx.CLR1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                goto hint_nop;
            case 0x11B:
                if constexpr (ctx.OPCODES_V20) { 
                    FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // CLR1 Mv, Ib
                                ctx.CLR1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                goto hint_nop;
            case 0x11C:
                if constexpr (ctx.OPCODES_V20) { 
                    FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // SET1 Mb, Ib
                                ctx.SET1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                goto hint_nop;
            case 0x11D:
                if constexpr (ctx.OPCODES_V20) { 
                    FAULT_CHECK(ctx.unopM(pc, [&](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // SET1 Mv, Ib
                                ctx.SET1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                goto hint_nop;
            case 0x11E:
                if constexpr (ctx.OPCODES_V20) { 
                    FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // NOT1 Mb, Ib
                                ctx.NOT1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
                goto hint_nop;
            case 0x11F:
                if constexpr (ctx.OPCODES_V20) { 
                    FAULT_CHECK(ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // NOT1 Mv, Ib
                                ctx.NOT1(dst, pc.read<uint8_t>());
                                return OP_NO_WRITE;
                        }
                    }));
                    ++pc;
                    break;
                }
            hint_nop: // HINT_NOP
                THROW_UD_WITHOUT_FLAG(ctx.HAS_LONG_NOP);
            modrm_nop:
                pc += pc.read<ModRM>().length(pc);
                break;
            case 0x120:
                if constexpr (ctx.OPCODES_80386) { // MOV M, CR

                }
                else { // ADD4S
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);

                }
                break;
            case 0x121:
                if constexpr (ctx.OPCODES_80386) { // MOV M, DR

                }
                THROW_UD();
                break;
            case 0x122:
                if constexpr (ctx.OPCODES_80386) { // MOV CR, M

                }
                else { // SUB4S
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);

                }
                break;
            case 0x123:
                if constexpr (ctx.OPCODES_80386) { // MOV DR, M

                }
                THROW_UD();
                break;
            case 0x124: // MOV M, TR
                THROW_UD_WITHOUT_FLAG(ctx.HAS_TEST_REGS);
                
                break;
            case 0x126:
                if constexpr (ctx.HAS_TEST_REGS) { // MOV TR, M

                }
                else { // CMP4S
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);

                }
                break;
            case 0x128:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // MOVAPS Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // MOVAPD Rx, Mx
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix:
                        case OpcodeF2Prefix:
                            ALWAYS_UD();
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<true>(pc, [](auto& src, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // ROL4 Mb
                                ctx.ROL4(src);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x129:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVAPS Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVAPD Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x12A:
                if constexpr (ctx.CPUID_SSE || ctx.CPUID_SSE2) {
                    switch (ctx.opcode_select()) {
                        default: unreachable;
                        case OpcodeNoPrefix: // CVTPI2PS Rx, Mm
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                            break;
                        case Opcode66Prefix: // CVTPI2PD Rx, Mm
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                            break;
                        case OpcodeF3Prefix: // CVTSI2SS Rx, Mv
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                            break;
                        case OpcodeF2Prefix: // CVTSI2SD Rx, Mv
                            THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                            break;
                    }
                }
                else {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<true>(pc, [](auto& src, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // ROR4 Mb
                                ctx.ROR4(src);
                                return OP_WRITE;
                        }
                    }));
                }
                break;
            case 0x12B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVNTPS Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVNTPD Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVNTSS Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                    case OpcodeF2Prefix: // MOVNTSD Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                }
                break;
            case 0x12C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // CVTTPS2PI Rm, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // CVTTPD2PI Rm, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // CVTTSS2SI Rv, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // CVTTSD2SI Rv, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x12D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // CVTPS2PI Rm, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // CVTPD2PI Rm, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // CVTSS2SI Rv, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // CVTSD2SI Rv, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x12E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // UCOMISS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // UCOMISD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x12F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // COMISS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // COMISD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x130: // WRMSR
            case 0x131: // RDTSC
            case 0x132: // RDMSR
            case 0x133: // RDPMC
            case 0x134: // SYSENTER
            case 0x135: // SYSEXIT
            case 0x138: // Three byte opcodes A
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                map = 2;
                goto next_byte;
            case 0x13A: // Three byte opcodes B
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                map = 3;
                goto next_byte;
            case 0x140: // CMOVO Rv, Mv
            case 0x141: // CMOVNO Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondNO>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x142: // CMOVC Rv, Mv
            case 0x143: // CMOVNC Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondNC>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x144: // CMOVZ Rv, Mv
            case 0x145: // CMOVNZ Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondNZ>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x146: // CMOVBE Rv, Mv
            case 0x147: // CMOVA Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondA>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x148: // CMOVS Rv, Mv
            case 0x149: // CMOVNS Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondNS>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x14A: // CMOVP Rv, Mv
            case 0x14B: // CMOVNP Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondNP>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x14C: // CMOVL Rv, Mv
            case 0x14D: // CMOVGE Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondGE>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x14E: // CMOVLE Rv, Mv
            case 0x14F: // CMOVG Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) {
                    ctx.CMOVCC<CondG>(dst, src, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x150:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVMSKPS Rv, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVMSKPD Rv, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x151:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SQRTPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // SQRTPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // SQRTSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // SQRTSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x152:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // RSQRTPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix:
                        ALWAYS_UD();
                    case OpcodeF3Prefix: // RSQRTSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x153: // RCPPS/RCPSS Rx, Mx
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // RCPPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix:
                        ALWAYS_UD();
                    case OpcodeF3Prefix: // RCPSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x154: // ANDPS/ANDPD Rx, Mx
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // ANDPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // ANDPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x155:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // ANDNPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // ANDNPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x156:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // ORPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // ORPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x157:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // XORPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // XORPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x158:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // ADDPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // ADDPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // ADDSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // ADDSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x159:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MULPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MULPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MULSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MULSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x15A:
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // CVTPS2PD Rx, Mx
                        break;
                    case Opcode66Prefix: // CVTPD2PS Rx, Mx
                        break;
                    case OpcodeF3Prefix: // CVTSS2SD Rx, Mx
                        break;
                    case OpcodeF2Prefix: // CVTSD2SS Rx, Mx
                        break;
                }
                break;
            case 0x15B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // CVTDQ2PS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case Opcode66Prefix: // CVTPS2DQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // CVTTPS2DQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x15C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SUBPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // SUBPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // SUBSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // SUBSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x15D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MINPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MINPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MINSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MINSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x15E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // DIVPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // DIVPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // DIVSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // DIVSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x15F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MAXPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MAXPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MAXSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MAXSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x160:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKLBW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PUNPCKLBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x161:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKLWD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PUNPCKLWD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x162:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKLDQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PUNPCKLDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x163:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PACKSSWB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PACKSSWB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x164:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PCMPGTB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PCMPGTB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x165:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PCMPGTW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PCMPGTW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x166:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PCMPGTD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PCMPGTD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x167:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PACKUSWB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PACKUSWB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x168:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKHBW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PUNPCKHBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x169:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKHWD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PUNPCKHWD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x16A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKHDQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PUNPCKHDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x16B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PACKSSDW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PACKSSDW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x16C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PUNPCKLQDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x16D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PUNPCKHQDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x16E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVD Rm, Mv
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // MOVD Rx, Mv
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x16F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // MOVDQA Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVDQU Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x170:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSHUFW Rm, Mm, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PSHUFD Rx, Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // PSHUFHW Rx, Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix: // PSHUFLW Rx, Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x171:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // GRP12 Mm, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // GRP12 Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x172:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // GRP13 Mm, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // GRP13 Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x173:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // GRP14 Mm, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // GRP14 Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x174:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PCMPEQB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PCMPEQB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x175:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PCMPEQW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PCMPEQW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x176:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PCMPEQD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PCMPEQD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x177:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // EMMS
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x178:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VMREAD Mv, Rv
                        ALWAYS_UD();
                    case Opcode66Prefix: // EXTRQ Mx, Ib, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                    case OpcodeF3Prefix:
                        ALWAYS_UD();
                    case OpcodeF2Prefix: // INSERTQ Rx, Mx, Ib, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                }
                break;
            case 0x179:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VMWRITE Rv, Mv
                        ALWAYS_UD();
                    case Opcode66Prefix: // EXTRQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                    case OpcodeF3Prefix:
                        ALWAYS_UD();
                    case OpcodeF2Prefix: // INSERTQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                }
                break;
            case 0x17C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // HADDPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                    case OpcodeF3Prefix:
                        ALWAYS_UD();
                    case OpcodeF2Prefix: // HADDPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                }
                break;
            case 0x17D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // HSUBPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                    case OpcodeF3Prefix:
                        ALWAYS_UD();
                    case OpcodeF2Prefix: // HSUBPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                }
                break;
            case 0x17E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVD Mv, Rm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // MOVD Mm, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x17F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVQ Mm, Rm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // MOVDQA Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVDQU Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x180: // JO Jz
            case 0x181: // JNO Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondNO>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x182: // JC Jz
            case 0x183: // JNC Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondNC>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x184: // JZ Jz
            case 0x185: // JNZ Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondNZ>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x186: // JBE Jz
            case 0x187: // JA Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondA>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x188: // JS Jz
            case 0x189: // JNS Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondNS>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x18A: // JP Jz
            case 0x18B: // JNP Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondNP>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x18C: // JL Jz
            case 0x18D: // JGE Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondGE>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x18E: // JLE Jz
            case 0x18F: // JG Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.JCC<CondG>(pc, opcode_byte & 1);
                goto next_instr;
            case 0x190: // SETO Mb
            case 0x191: // SETNO Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNO>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x192: // SETC Mb
            case 0x193: // SETNC Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNC>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x194: // SETZ Mb
            case 0x195: // SETNZ Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNZ>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x196: // SETBE Mb
            case 0x197: // SETA Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondA>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x198: // SETS Mb
            case 0x199: // SETNS Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNS>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x19A: // SETP Mb
            case 0x19B: // SETNP Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNP>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x19C: // SETL Mb
            case 0x19D: // SETGE Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondGE>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x19E: // SETLE Mb
            case 0x19F: // SETG Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<true>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondG>(dst, opcode_byte & 1);
                    return true;
                }));
                break;
            case 0x1A0: // PUSH FS
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.PUSH(ctx.fs);
                break;
            case 0x1A1: // POP FS
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.fs = ctx.POP<uint16_t>();
                break;
            case 0x1A2: // CPUID
                // TODO
                break;
            case 0x1A3: // BT Mv, Gv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMRB(pc, [](auto dst, auto src) {
                    ctx.BT(dst, src);
                    return false;
                }));
                break;
            case 0x1A4: // SHLD Mv, Gv, Ib
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [&](auto& dst, auto src) {
                    ctx.SHLD(dst, src, pc.read<uint8_t>());
                    return true;
                }));
                ++pc;
                break;
            case 0x1A5: // SHLD Mv, Gv, CL
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.SHLD(dst, src, ctx.cl);
                    return true;
                }));
                break;
            case 0x1A8: // PUSH GS
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.PUSH(ctx.gs);
                break;
            case 0x1A9: // POP GS
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                ctx.gs = ctx.POP<uint16_t>();
                break;
            case 0x1AA: // RSM
                // TODO
                break;
            case 0x1AB: // BTS Mv, Rv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.BTS(dst, src);
                    return true;
                }));
                break;
            case 0x1AC: // SHRD Mv, Rv, Ib
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [&](auto dst, auto src) {
                    ctx.SHRD(dst, src, pc.read<uint8_t>());
                    return true;
                }));
                ++pc;
                break;
            case 0x1AD: // SHRD Mv, Rv, CL
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [](auto dst, auto src) {
                    ctx.SHRD(dst, src, ctx.cl);
                    return true;
                }));
                break;
            case 0x1AE: // GRP15
                // TODO
                break;
            case 0x1AF: // IMUL Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.IMUL(dst, src);
                    return true;
                }));
                break;
            case 0x1B0: // CMPXCHG Mb, Rb
            case 0x1B1: // CMPXCHG Mv, Rb
                // TODO
                break;
            case 0x1B2: // LSS Rv, M
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopRMF(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.ss = src >> (bitsof(src) >> 1);
                    return true;
                }));
                break;
            case 0x1B3: // BTR Mv, Rv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.BTR(dst, src);
                    return true;
                }));
                break;
            case 0x1B4: case 0x1B5: // LFS/LGS Rv, M
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopRMF(pc, [=](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.write_seg(FS + (opcode_byte & 1), src >> (bitsof(src) >> 1));
                    return true;
                }));
                break;
            case 0x1B6: // MOVZX Rv, Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                // TODO
                break;
            case 0x1B7: // MOVZX Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                // TODO
                break;
            case 0x1B8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case Opcode66Prefix: // POPCNT Rv, Mv
                        if (ctx.rep_type > 0) {
                    case OpcodeF3Prefix: // POPCNT Rv, Mv
                            // TODO
                            break;
                        }
                        else {
                    case OpcodeNoPrefix: // JMPE Jx
                            ALWAYS_UD();
                        }
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1BA: // GRP8 Mv, Ib
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                // TODO
                break;
            case 0x1BB: // BTC Mv, Gv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.BTC(dst, src);
                    return true;
                }));
                break;
            case 0x1BC: // BSF Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                if (ctx.rep_type > 0) { // TZCNT Rv, Mv
                    // TODO
                }
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.BSF(dst, src);
                    return true;
                }));
                break;
            case 0x1BD: // BSR Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                if (ctx.rep_type > 0) { // LZCNT Rv, Mv
                    // TODO
                }
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.BSR(dst, src);
                    return true;
                }));
                break;
            case 0x1BE: // MOVSX Rv, Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                // TODO
                break;
            case 0x1BF: // MOVSX Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                // TODO
                break;
            case 0x1C0: // XADD Mb, Rb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80486);
                // TODO
                break;
            case 0x1C1: // XADD Mv, Rv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80486);
                // TODO
                break;
            case 0x1C2:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // CMPccPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // CMPccPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // CMPccSS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // CMPccSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x1C3:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVNTI Mv, Rv
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case Opcode66Prefix:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1C4:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PINSRW Rm, Mw, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PINSRW Rx, Mw, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1C5:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PEXTRW Rv, Mm, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PEXTRW Rv, Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1C6:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SHUFPS Rx, Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // SHUFPD Rx, Mx, Ib
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1C7: // GRP9
                // TODO
                break;
            case 0x1C8: case 0x1C9: case 0x1CA: case 0x1CB: case 0x1CC: case 0x1CD: case 0x1CE: case 0x1CF: // BSWAP reg
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80486);
                ctx.BSWAP(ctx.index_regMB<uint16_t>(opcode_byte & 7));
                break;
            case 0x1D0:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // ADDSUBPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                    case OpcodeF3Prefix:
                        ALWAYS_UD();
                    case OpcodeF2Prefix: // ADDSUBPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                }
                break;
            case 0x1D1:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSRLW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSRLW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D2:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSRLD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSRLD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D3:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSRLQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSRLQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D4:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                    case Opcode66Prefix: // PADDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D5:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MULLW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // MULLW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D6:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // MOVQ Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVQ2DQ Rx, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix: // MOVDQ2Q Rm, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x1D7:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMOVMSKB Rv, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PMOVMSKB Rv, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBUSB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSUBUSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D9:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBUSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSUBUSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1DA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMINUB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PMINUB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1DB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PAND Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PAND Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1DC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDUSB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PADDUSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1DD:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDUSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PADDUSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1DE:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMAXUB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PMAXUB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1DF:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PANDN Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PANDN Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E0:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PAVGB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PAVGB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E1:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSRAW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSRAW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E2:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSRAD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSRAD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E3:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PAVGW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PAVGW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E4:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMULHUW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PMULHUW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E5:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMULHW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PMULHW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E6:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // CVTTPD2DQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // CVTDQ2PD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix: // CVTDQ2PD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x1E7:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVNTQ Mm, Rm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVNTDQ Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBSB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSUBSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1E9:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSUBSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1EA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMINSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PMINSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1EB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // POR Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // POR Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1EC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDSB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PADDSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1ED:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PADDSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1EE:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMAXSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PMAXSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1EF:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PXOR Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PXOR Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F0:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix:
                        ALWAYS_UD();
                    case OpcodeF3Prefix: // LDDQU Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F1:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSLLW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSLLW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F2:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSLLD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSLLD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F3:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSLLQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSLLQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F4:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMULUDQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                    case Opcode66Prefix: // PMULUDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F5:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMADDWD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PMADDWD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F6:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSADBW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // PSADBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F7:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MASKMOVQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MASKMOVDQU Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSUBB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F9:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSUBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1FA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PSUBD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1FB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSUBQ Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                    case Opcode66Prefix: // PSUBQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1FC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PADDB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1FD:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PADDW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1FE:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PADDD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // PADDD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
                /*
            case 0x200:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSHUFB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        break;
                    case Opcode66Prefix: // PSHUFB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x201:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PHADDW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        break;
                    case Opcode66Prefix: // PHADDW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
                */
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