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

//struct z8086Context : z86Core<z80286, FLAG_CPUID_MMX | FLAG_CPUID_SSE | FLAG_CPUID_SSE2 | FLAG_CPUID_SSE3 /*, FLAG_OPCODES_80186 | FLAG_OPCODES_80286 | FLAG_OPCODES_80386 | FLAG_OPCODES_80486 | FLAG_CPUID_CMOV*/> {
struct z8086Context : z86Core<z8086, FLAG_XBTS_IBTS> {

    z8086Context() = default;

    // Internal state
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;
    std::atomic<int16_t> pending_einterrupt;
    size_t clock;

    inline void init() {
        //memset(this, 0, sizeof(*this));
        new (this) z8086Context();
        this->reset_descriptors();
        this->reset_ip();
        this->pending_einterrupt = -1;
    }

    inline constexpr void reset() {
        this->init();
    }

    inline void check_for_software_interrupt() {
        int16_t pending_software = this->pending_sinterrupt;
        if (pending_software > 0) {
        //if (expect_chance(pending_software > 0, true, 0.1)) {
            this->pending_sinterrupt = -1;
            this->call_interrupt(pending_software);
        }
    }

    template <bool set_halt = false>
    inline void check_for_nmi() {
        if (this->pending_nmi.exchange(false)) {
        //if (expect_chance(this->pending_nmi.exchange(false), true, 0.1)) {
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
            //if (expect_chance(external > 0, true, 0.1)) {
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
        this->ip = mem.read<uint16_t>(interrupt_addr);
        this->cs = mem.read<uint16_t>(interrupt_addr + 2);

        this->check_for_nmi();
        if (prev_trap) {
        //if (expect(prev_trap, false)) {
            this->call_interrupt(IntDB);
        }
    }

    inline void HLT() {
        this->halted = true;
        this->check_for_software_interrupt();
        do {
            this->check_for_nmi<true>();
            this->check_for_external_interrupt<true>();
        } while (this->halted);
        if (this->trap) {
        //if (expect(this->trap, false)) {
            this->call_interrupt(IntDB);
        }
    }

    inline void execute_pending_interrupts() {
        this->check_for_software_interrupt();
        this->check_for_nmi();
        this->check_for_external_interrupt();
        if (this->trap) {
        //if (expect(this->trap, false)) {
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
dllexport size_t z86_file_read(size_t dst, FILE* src, size_t size) {
    return mem.read_file(dst, src, size);
}
dllexport size_t z86_mem_read(void* dst, size_t src, size_t size) {
    return mem.read(dst, src, size);
}
dllexport size_t z86_file_write(FILE* dst, size_t src, size_t size) {
    return mem.write_file(dst, src, size);
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

#define ALWAYS_UD() { ctx.set_fault(IntUD); goto fault; }
#define ALWAYS_UD_GRP() { ctx.set_fault(IntUD); return OP_FAULT; }
#define THROW_UD() if constexpr (!ctx.NO_UD) { ALWAYS_UD(); } else
#define THROW_UD_GRP() if constexpr (!ctx.NO_UD) { ALWAYS_UD_GRP(); } else

#define ALWAYS_UD_WITHOUT_FLAG(...) if constexpr (!(__VA_ARGS__)) { ALWAYS_UD(); } else
#define ALWAYS_UD_WITHOUT_FLAG_GRP(...) if constexpr (!(__VA_ARGS__)) { ALWAYS_UD_GRP(); } else
#define THROW_UD_WITHOUT_FLAG(...) if constexpr (!(__VA_ARGS__)) { THROW_UD(); } else
#define THROW_UD_WITHOUT_FLAG_GRP(...) if constexpr (!(__VA_ARGS__)) { THROW_UD_GRP(); } else

#define ALWAYS_UD_WITHOUT_X87_REGS() ALWAYS_UD_WITHOUT_FLAG(ctx.CPUID_X87 || ctx.CPUID_MMX || ctx.CPUID_3DNOW)
#define ALWAYS_UD_WITHOUT_MMX_REGS() ALWAYS_UD_WITHOUT_FLAG(ctx.CPUID_X87 || ctx.CPUID_MMX || ctx.CPUID_3DNOW)
#define ALWAYS_UD_WITHOUT_SSE_REGS() ALWAYS_UD_WITHOUT_FLAG(ctx.CPUID_SSE || ctx.CPUID_SSE2)
#define ALWAYS_UD_WITHOUT_MMX_SSE_REGS() ALWAYS_UD_WITHOUT_FLAG((ctx.CPUID_X87 || ctx.CPUID_MMX || ctx.CPUID_3DNOW) && (ctx.CPUID_SSE || ctx.CPUID_SSE2))

#define ALWAYS_GP() { ctx.set_fault(IntGP); goto fault; }
#define ALWAYS_GP_GRP() { ctx.set_fault(IntGP); return OP_FAULT; }
#define GP_WITHOUT_CPL0() if (ctx.current_privilege_level() != 0) { ALWAYS_GP(); } else
#define GP_WITHOUT_CPL0_GRP() if (ctx.current_privilege_level() != 0) { ALWAYS_GP_GRP(); } else

#define GP_WITHOUT_IOPL() if (ctx.current_privilege_level() > ctx.get_iopl()) { ALWAYS_GP(); } else
#define GP_WITHOUT_IOPL_GRP() if (ctx.current_privilege_level() > ctx.get_iopl()) { ALWAYS_GP_GRP(); } else

#define FAULT_CHECK(...) if (expect((__VA_ARGS__), false)) { goto fault; }

#define FAULT_CHECK_X87(...) ALWAYS_UD_WITHOUT_X87_REGS() { FAULT_CHECK(__VA_ARGS__); }
#define FAULT_CHECK_MMX(...) ALWAYS_UD_WITHOUT_MMX_REGS() { FAULT_CHECK(__VA_ARGS__); }
#define FAULT_CHECK_SSE(...) ALWAYS_UD_WITHOUT_SSE_REGS() { FAULT_CHECK(__VA_ARGS__); }
#define FAULT_CHECK_MMX_SSE(...) ALWAYS_UD_WITHOUT_MMX_SSE_REGS() { FAULT_CHECK(__VA_ARGS__); }

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
        //uint32_t opcode = opcode_byte | (uint32_t)map << 8;
        //assume(opcode <= UINT16_MAX);
        //switch (opcode) {
        switch (opcode_byte | (uint32_t)map << 8) {
            case 0x00: // ADD Mb, Rb
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.PUSH(ctx.get_seg(opcode_byte >> 3)));
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
                FAULT_CHECK(ctx.write_seg(opcode_byte >> 3, ctx.POP()));
                break;
            case 0x08: // OR Mb, Rb
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                });
                break;
            case 0x0D: // OR AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.OR(dst, src);
                });
                break;
            case 0x10: // ADC Mb, Rb
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                });
                break;
            case 0x15: // ADC AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.ADC(dst, src);
                });
                break;
            case 0x18: // SBB Mb, Rb
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                });
                break;
            case 0x1D: // SBB AX, Is
                ctx.binopAI(pc, [](auto& dst, auto src) regcall {
                    ctx.SBB(dst, src);
                });
                break;
            case 0x20: // AND Mb, Rb
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto& dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopRM<OP_BYTE>(pc, [](auto dst, auto src) regcall {
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
                ctx.binopAI<OP_BYTE>(pc, [](auto dst, auto src) regcall {
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
                        ctx.set_rex_bits(opcode_byte);
                        goto prefix_byte;
                    }
                }
                FAULT_CHECK(ctx.unopR<OP_NO_REX>(pc, opcode_byte & 7, [](auto& dst) regcall {
                    ctx.INC(dst);
                    return OP_WRITE;
                }));
                break;
            case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: // DEC reg
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        ctx.set_rex_bits(opcode_byte);
                        goto prefix_byte;
                    }
                }
                FAULT_CHECK(ctx.unopR<OP_NO_REX>(pc, opcode_byte & 7, [](auto& dst) regcall {
                    ctx.DEC(dst);
                    return OP_WRITE;
                }));
                break;
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: // PUSH reg
                FAULT_CHECK(ctx.unopR(pc, opcode_byte & 7, [](auto& dst) regcall {
                    if constexpr (ctx.OLD_PUSH_SP) {
                        return ctx.PUSH(dst) ? OP_FAULT : OP_NO_WRITE; // Stack is written, not dst
                    }
                    else {
                        auto temp = dst;
                        return ctx.PUSH(temp) ? OP_FAULT : OP_NO_WRITE; // Stack is written, not dst
                    }
                }));
                break;
            case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: // POP reg
                FAULT_CHECK(ctx.unopR<OP_NO_READ>(pc, opcode_byte & 7, [](auto& dst) regcall {
                    dst = ctx.POP();
                    return OP_WRITE;
                }));
                break;
            case 0x60: // PUSHA
                if constexpr (ctx.OPCODES_80186) {
                    if constexpr (ctx.LONG_MODE) {
                        if (ctx.is_long_mode()) {
                            THROW_UD();
                        }
                    }
                    FAULT_CHECK(ctx.PUSHA());
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
                    FAULT_CHECK(ctx.POPA());
                    break;
                }
                THROW_UD();
            case 0x70: // JO Jb
            case 0x71: // JNO Jb
                FAULT_CHECK(ctx.JCC<CondNO, OP_BYTE>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x62: // BOUND Rv, Mv2
                if constexpr (ctx.OPCODES_80186) {
                    if constexpr (ctx.LONG_MODE) {
                        if (ctx.is_long_mode()) {
                            // Parse EVEX
                            THROW_UD();
                        }
                    }
                    FAULT_CHECK(ctx.binopRM2<OP_NO_REX>(pc, [](auto index, auto lower, auto upper) regcall {
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
                            FAULT_CHECK(ctx.MOVX<int32_t>(pc, [](auto& dst, auto src) regcall {
                                using S = decltype(src);
                                using D = std::remove_reference_t<decltype(dst)>;
                                dst = (D)(S)src;
                                return OP_NOT_MEM;
                            }));
                            break;
                        }
                    }
                    FAULT_CHECK(ctx.binopMR_impl<OP_NO_REX, uint16_t>(pc, [](auto& dst, auto src) regcall {
                        return ctx.ARPL(dst, src);
                    }));
                    break;
                }
                else if constexpr (ctx.OPCODES_V20) {
                    // ???
                }
                THROW_UD();
            case 0x72: // JC Jb
            case 0x73: // JNC Jb
                FAULT_CHECK(ctx.JCC<CondNC, OP_BYTE>(pc, opcode_byte & 1));
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
                FAULT_CHECK(ctx.JCC<CondNZ, OP_BYTE>(pc, opcode_byte & 1));
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
                FAULT_CHECK(ctx.JCC<CondA, OP_BYTE>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x68: // PUSH Is
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.PUSHI(pc.read_advance_Is()));
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
                FAULT_CHECK(ctx.JCC<CondNS, OP_BYTE>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x6A: // PUSH Ib
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.PUSHI(pc.read_advance<int8_t>()));
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
                FAULT_CHECK(ctx.JCC<CondNP, OP_BYTE>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x6C: // INSB
                if constexpr (ctx.OPCODES_80186) {
                    GP_WITHOUT_IOPL();
                    FAULT_CHECK(ctx.INS<OP_BYTE>());
                    break;
                }
                THROW_UD();
            case 0x6D: // INS
                if constexpr (ctx.OPCODES_80186) {
                    GP_WITHOUT_IOPL();
                    FAULT_CHECK(ctx.INS());
                    break;
                }
                THROW_UD();
            case 0x7C: // JL Jb
            case 0x7D: // JGE Jb
                FAULT_CHECK(ctx.JCC<CondGE, OP_BYTE>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x6E: // OUTSB
                if constexpr (ctx.OPCODES_80186) {
                    GP_WITHOUT_IOPL();
                    FAULT_CHECK(ctx.OUTS<OP_BYTE>());
                    break;
                }
                THROW_UD();
            case 0x6F: // OUTS
                if constexpr (ctx.OPCODES_80186) {
                    GP_WITHOUT_IOPL();
                    FAULT_CHECK(ctx.OUTS());
                    break;
                }
                THROW_UD();
            case 0x7E: // JLE Jb
            case 0x7F: // JG Jb
                FAULT_CHECK(ctx.JCC<CondG, OP_BYTE>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x82:
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
            case 0x80: // GRP1 Mb, Ib
                FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto& dst, uint8_t r) regcall {
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
            case 0x81: // GRP1 Mv, Is
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
            case 0x83: // GRP1 Mv, Ib
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
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto dst, auto src) regcall {
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
                FAULT_CHECK(ctx.binopMR<OP_BYTE>(pc, [](auto& dst, auto& src) regcall {
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
            mov_mrb:
                FAULT_CHECK(ctx.binopMR<OP_BYTE | OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x89: // MOV Mv, Rv
            mov_mr:
                FAULT_CHECK(ctx.binopMR<OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8A: // MOV Rb, Mb
            mov_rmb:
                FAULT_CHECK(ctx.binopRM<OP_BYTE | OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8B: // MOV Rv, Mv
            mov_rm:
                FAULT_CHECK(ctx.binopRM<OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8C: // MOV M, seg
                FAULT_CHECK(ctx.binopMS<OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8D: // LEA Rv, M
                FAULT_CHECK(ctx.LEA(pc));
                break;
            case 0x8E: // MOV seg, M
                FAULT_CHECK(ctx.binopSM<OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_WRITE;
                }));
                break;
            case 0x8F: // GRP1A (Supposedly this does mystery jank if R != 0)
                FAULT_CHECK(ctx.unopM(pc, [](auto& src, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                            THROW_UD_GRP();
                            // TODO: ???
                        case 0:
                            src = ctx.POP();
                            return OP_WRITE;
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
                FAULT_CHECK(ctx.CALLFABS(pc));
                goto next_instr;
            case 0x9B: // WAIT
                // NOP :D
                break;
            case 0x9C: // PUSHF
                FAULT_CHECK(ctx.PUSHF());
                break;
            case 0x9D: // POPF
                FAULT_CHECK(ctx.POPF());
                break;
            case 0x9E: // SAHF
                ctx.set_flags<uint8_t>(ctx.ah);
                break;
            case 0x9F: // LAHF
                ctx.ah = ctx.get_flags<uint8_t>();
                break;
            case 0xA0: // MOV AL, mem
                FAULT_CHECK(ctx.binopAO<OP_BYTE>(pc, [](auto& dst, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    dst = addr.read<decltype(dst)>();
                    return NO_FAULT;
                }));
                break;
            case 0xA1: // MOV AX, mem
                FAULT_CHECK(ctx.binopAO(pc, [](auto& dst, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    dst = addr.read<decltype(dst)>();
                    return NO_FAULT;
                }));
                break;
            case 0xA2: // MOV mem, AL
                FAULT_CHECK(ctx.binopAO<OP_BYTE>(pc, [](auto src, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    addr.write(src);
                    return NO_FAULT;
                }));
                break;
            case 0xA3: // MOV mem, AX
                FAULT_CHECK(ctx.binopAO(pc, [](auto src, auto offset) regcall {
                    z86Addr addr = ctx.addr(DS, offset);
                    addr.write(src);
                    return NO_FAULT;
                }));
                break;
            case 0xA4: // MOVSB
                FAULT_CHECK(ctx.MOVS<OP_BYTE>());
                break;
            case 0xA5: // MOVSW
                FAULT_CHECK(ctx.MOVS());
                break;
            case 0xA6: // CMPSB
                FAULT_CHECK(ctx.CMPS<OP_BYTE>());
                break;
            case 0xA7: // CMPSW
                FAULT_CHECK(ctx.CMPS());
                break;
            case 0xA8: // TEST AL, Ib
                ctx.binopAI<OP_BYTE>(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                });
                break;
            case 0xA9: // TEST AX, Is
                ctx.binopAI(pc, [](auto dst, auto src) regcall {
                    ctx.TEST(dst, src);
                });
                break;
            case 0xAA: // STOSB
                FAULT_CHECK(ctx.STOS<OP_BYTE>());
                break;
            case 0xAB: // STOSW
                FAULT_CHECK(ctx.STOS());
                break;
            case 0xAC: // LODSB
                FAULT_CHECK(ctx.LODS<OP_BYTE>());
                break;
            case 0xAD: // LODSW
                FAULT_CHECK(ctx.LODS());
                break;
            case 0xAE: // SCASB
                FAULT_CHECK(ctx.SCAS<OP_BYTE>());
                break;
            case 0xAF: // SCASW
                FAULT_CHECK(ctx.SCAS());
                break;
            case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: // MOV reg8, Ib
                ctx.MOV_RI<OP_BYTE>(pc, opcode_byte & 7);
                break;
            case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: // MOV reg, Iv
                ctx.MOV_RI(pc, opcode_byte & 7);
                break;
            case 0xC0: // GRP2 Mb, Ib
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto& dst, uint8_t r) regcall {
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
                FAULT_CHECK(ctx.RETI(pc));
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
                FAULT_CHECK(ctx.RET());
                goto next_instr;
            case 0xC4: // LES Rv, Mf
                FAULT_CHECK(ctx.binopRMF<OP_NO_REX>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.es = src >> (bitsof(src) >> 1);
                    return true;
                }));
                break;
            case 0xC5: // LDS Rv, Mf
                FAULT_CHECK(ctx.binopRMF<OP_NO_REX>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    ctx.ds = src >> (bitsof(src) >> 1);
                    return true;
                }));
                break;
            case 0xC6: // GRP11 Ib (Supposedly this just ignores R bits)
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [&](auto& dst, uint8_t r) regcall {
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
                FAULT_CHECK(ctx.unopM<OP_NO_READ>(pc, [&](auto& dst, uint8_t r) regcall {
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
                    FAULT_CHECK(ctx.ENTER(pc.read<uint16_t>(), pc.read<uint8_t>(2)));
                    break;
                }
                THROW_UD();
            case 0xCA: // RETF imm
                FAULT_CHECK(ctx.RETFI(pc));
                goto next_instr;
            case 0xC9: // LEAVE
                if constexpr (ctx.OPCODES_80186) {
                    FAULT_CHECK(ctx.LEAVE());
                    break;
                }
                THROW_UD();
            case 0xCB: // RETF
                FAULT_CHECK(ctx.RETF());
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
                FAULT_CHECK(ctx.IRET());
                continue; // Using continues delays execution deliberately
            case 0xD0: // GRP2 Mb, 1
                FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto& dst, uint8_t r) regcall {
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
                FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto& dst, uint8_t r) regcall {
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
                    ctx.fop_set(opcode_byte, modrm.raw);
                    uint8_t r = modrm.R();
                    uint8_t m;
                    long double* lhs;
                    long double rhs;
                    if (modrm.is_mem()) {
                        z86Addr data_addr = modrm.parse_memM(pc);
                        switch (opcode_byte & 7) {
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
                        switch (opcode_byte & 7) {
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
                                        ctx.FCMOVCC<CondNB>(ctx.index_st_reg(m), opcode_byte & 1);
                                        break;
                                    case 1: fcmove: // FCMOVE ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                                        ctx.FCMOVCC<CondNE>(ctx.index_st_reg(m), opcode_byte & 1);
                                        break;
                                    case 2: fcmovbe: // FCMOVBE ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                                        ctx.FCMOVCC<CondNBE>(ctx.index_st_reg(m), opcode_byte & 1);
                                        break;
                                    case 3: fcmovu: // FCMOVU ST(0), ST(m)
                                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                                        ctx.FCMOVCC<CondNP>(ctx.index_st_reg(m), opcode_byte & 1);
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
                FAULT_CHECK(ctx.LOOPCC(pc, opcode_byte & 1));
                goto next_instr;
            case 0xE2: // LOOP Jb
                FAULT_CHECK(ctx.LOOP(pc));
                goto next_instr;
            case 0xE3: // JCXZ Jb
                FAULT_CHECK(ctx.JCXZ(pc));
                goto next_instr;
            case 0xE4: // IN AL, Ib
                GP_WITHOUT_IOPL();
                ctx.port_in<OP_BYTE>(pc.read_advance<uint8_t>());
                break;
            case 0xE5: // IN AX, Ib
                GP_WITHOUT_IOPL();
                ctx.port_in(pc.read_advance<uint8_t>());
                break;
            case 0xE6: // OUT Ib, AL
                GP_WITHOUT_IOPL();
                ctx.port_out<OP_BYTE>(pc.read_advance<uint8_t>());
                break;
            case 0xE7: // OUT Ib, AX
                GP_WITHOUT_IOPL();
                ctx.port_out(pc.read_advance<uint8_t>());
                break;
            case 0xE8: // CALL Jz
                FAULT_CHECK(ctx.CALL(pc));
                goto next_instr;
            case 0xE9: // JMP Jz
                FAULT_CHECK(ctx.JMP(pc));
                goto next_instr;
            case 0xEA: // JMP far abs
                if constexpr (ctx.LONG_MODE) {
                    if (ctx.is_long_mode()) {
                        THROW_UD();
                    }
                }
                FAULT_CHECK(ctx.JMPFABS(pc));
                goto next_instr;
            case 0xEB: // JMP Jb
                FAULT_CHECK(ctx.JMP<OP_BYTE>(pc));
                goto next_instr;
            case 0xEC: // IN AL, DX
                GP_WITHOUT_IOPL();
                ctx.port_in<OP_BYTE>(ctx.dx);
                break;
            case 0xED: // IN AX, DX
                GP_WITHOUT_IOPL();
                ctx.port_in(ctx.dx);
                break;
            case 0xEE: // OUT DX, AL
                GP_WITHOUT_IOPL();
                ctx.port_out<OP_BYTE>(ctx.dx);
                break;
            case 0xEF: // OUT DX, AX
                GP_WITHOUT_IOPL();
                ctx.port_out(ctx.dx);
                break;
            case 0xF1:
                if constexpr (ctx.OPCODES_80386) { // INT1
                    ctx.set_trap(IntDB);
                    goto trap;
                }
                if constexpr (ctx.OPCODES_80286) {
                    // Stupid STOREALL prefix, just going to ignore this and pretend it's still lock
                    goto lock;
                }
                if constexpr (!ctx.OPCODES_V20) {
                    THROW_UD();
                }
            case 0xF0: lock: // LOCK
                ctx.set_lock();
                goto prefix_byte;
            case 0xF2: case 0xF3: // REPNE, REP
                ctx.set_rep_type(opcode_byte);
                goto prefix_byte;
            case 0xF4: // HLT
                GP_WITHOUT_CPL0();
            hlt:
                ctx.HLT();
                continue;
            case 0xF5: // CMC
                ctx.carry ^= 1;
                break;
            case 0xF6: // GRP3 Mb
                FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto& val, uint8_t r) regcall {
                    switch (r) {
                        default: unreachable;
                        case 0: case 1: // TEST Mb, Ib
                            ctx.TEST(val, pc.read_advance<uint8_t>());
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
                break;
            case 0xF7: // GRP3 Mv
                FAULT_CHECK(ctx.unopM(pc, [&](auto& val, uint8_t r) fastcall {
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
                GP_WITHOUT_IOPL();
                ctx.interrupt = opcode_byte & 1;
                break;
            case 0xFC: case 0xFD: // CLD, STD
                ctx.direction = opcode_byte & 1;
                break;

                // TODO: Fault support
            case 0xFE: // GRP4 Mb
                if (ctx.unopMS<OP_BYTE>(pc)) {
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
                            THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                            dst = ctx.get_control_seg(r & 1);
                            return OP_WRITE;
                        case 2: // LLDT Mw
                            THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                            GP_WITHOUT_CPL0_GRP();
                            return ctx.write_ldtr(dst) ? OP_FAULT : OP_NO_WRITE;
                        case 3: // LTR Mw
                            THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                            GP_WITHOUT_CPL0_GRP();
                            return ctx.write_tr(dst) ? OP_FAULT : OP_NO_WRITE;
                        case 4: // VERR Mw
                            THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                            return ctx.VERR(dst) ? OP_FAULT : OP_NO_WRITE;
                        case 5: // VERW Mw
                            THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                            return ctx.VERW(dst) ? OP_FAULT : OP_NO_WRITE;
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
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
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
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                                GP_WITHOUT_CPL0_GRP();
                                uint16_t limit = data_addr.read<uint16_t>();
                                decltype(ctx.get_descriptor_table_base(0)) base;
                                if constexpr (ctx.LONG_MODE) {
                                    if (ctx.is_long_mode()) {
                                        base = data_addr.read<uint64_t>(2);
                                        goto load_table;
                                    }
                                }
                                base = data_addr.read<uint32_t>(2);
                                if constexpr (ctx.max_bits > 16) {
                                    if (ctx.data_size_32()) {
                                        goto load_table;
                                    }
                                }
                                base &= 0x00FFFFFF;
                            load_table:
                                ctx.load_descriptor_table(r & 1, limit, base);
                                return OP_NO_FAULT;
                            }
                            case 4: // SMSW Mw
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                                data_addr.write<uint16_t>(ctx.get_machine_status_word());
                                return OP_NO_FAULT;
                            case 5:
                                ALWAYS_UD_GRP();
                            case 6: // LMSW Mw
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                                GP_WITHOUT_CPL0_GRP();
                                ctx.set_machine_status_word(data_addr.read<uint16_t>());
                                return OP_NO_FAULT;
                            case 7: // INVLPG M
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.OPCODES_80486);
                                return OP_NO_FAULT;
                        }
                    },
                    [](auto& src, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                                ALWAYS_UD_GRP();
                            case 4: // SMSW Rv
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                                src = ctx.get_machine_status_word();
                                return OP_NO_FAULT;
                            case 5:
                                ALWAYS_UD_GRP();
                            case 6: // LMSW Rv
                                THROW_UD_WITHOUT_FLAG_GRP(ctx.PROTECTED_MODE);
                                ctx.set_machine_status_word(src);
                                return OP_NO_FAULT;
                            case 7:
                                ALWAYS_UD_GRP();
                        }
                    }
                ));
                break;
            case 0x102: // LAR Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.PROTECTED_MODE);
                break;
            case 0x103: // LSL Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.PROTECTED_MODE);
                break;
            case 0x104: // STOREALL
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80286 && !ctx.OPCODES_80386);
                // We're just going to pretend that LOCK is the ICE prefix
                if (!ctx.lock) {
                    ALWAYS_UD();
                }
                ctx.STOREALL();
                ctx.interrupt = false;
                goto hlt;
            case 0x105: // SYSCALL
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80286 && !ctx.OPCODES_80386);
                // LOADALL2
                GP_WITHOUT_CPL0();
                ctx.LOADALL2();
                break;
            case 0x106: // CLTS
                GP_WITHOUT_CPL0();

                break;
            case 0x107: // SYSRET
                // Technically LOADALL is still here on the 80486,
                // but can only be used from within ICE mode.
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386 && !ctx.OPCODES_80486);
                // LOADALL3
                GP_WITHOUT_CPL0();
                ctx.LOADALL3();
                break;
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
            case 0x10C:
            case 0x125: case 0x127:
            case 0x136: case 0x137:
            case 0x139: case 0x13B: case 0x13C: case 0x13E: case 0x13F:
            case 0x256: case 0x257:
            case 0x25D:
            case 0x260: case 0x261:
            case 0x269: case 0x26A:
            case 0x284: case 0x285: case 0x286: case 0x287:
            case 0x294: case 0x295:
            case 0x2A4: case 0x2A5:
            case 0x2B2: case 0x2B3:
            case 0x2C0: case 0x2C1: case 0x2C2: case 0x2C3: case 0x2C5:
            case 0x2CE:
            case 0x2D0: case 0x2D1: case 0x2D4: case 0x2D5: case 0x2D6: case 0x2D7:
            case 0x2D9:
            case 0x2F4:
            case 0x2FD: case 0x2FE: case 0x2FF:
                THROW_UD();
                break;
            case 0x10D: // PREFETCHx
            case 0x10E: // FEMMS
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_3DNOW);
            femms:
                break;
            case 0x10F: { // 3DNow!
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_3DNOW);
                /*
                FAULT_CHECK_MMX(ctx.binopRM_MM(pc, [&](auto& dst, auto src) regcall{
                    switch (pc.read<uint8_t>()) {
                        default: unreachable;
                        case 0x0C: // PI2FW Rm, Mm
                        case 0x0D: // PI2FD Rm, Mm
                        case 0x1C: // PF2IW Rm, Mm
                        case 0x1D: // PF2ID Rm, Mm

                        case 0x00: // TFADDS
                        case 0x01: // TFADDD
                        case 0x02: // TFSUBS
                        case 0x03: // TFSUBD
                        case 0x04: // TFMULS
                        case 0x05: // TFMULD
                        case 0x06: // TFDIVS
                        case 0x07: // TFDIVD
                        case 0x10: // TFSQRTS
                        case 0x11: // TFSQRTD
                        case 0x12: // TFPREMS
                        case 0x13: // TFPREMD
                        case 0x14: // TFCOMI
                        case 0x15: // TFUCOMI
                        case 0x20: // TFCMOVB
                        case 0x21: // TFCMOVE
                        case 0x22: // TFCMOVBE
                        case 0x23: // TFCMOVU
                        case 0x24: // TFCMOVNB
                        case 0x25: // TFCMOVNE
                        case 0x26: // TFCMOVNBE
                        case 0x27: // TFCMOVNU
                        case 0x30: // TFABS
                        case 0x31: // TFCPYS
                        case 0x32: // TFCPYSN
                        case 0x33: // TFCPYSE
                        case 0x40: // TFCVT32D
                        case 0x41: // TFCVT32S
                        case 0x42: // TFCVT64D
                        case 0x43: // TFCVT64S
                        case 0x44: // TFCVTF64
                        case 0x45: // TFCVTF32
                        case 0x46: // TFCVTDS
                        case 0x50: // TFLD32
                        case 0x51: // TFLD64
                        case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0E: case 0x0F:
                        case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1E: case 0x1F:
                        case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F:
                        case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
                        case 0x47: case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
                    }
                    return OP_WRITE;
                }));
                ++pc;
                */
                break;
            }
            case 0x110:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // TEST1 Mb, CL
                                ctx.TEST1(dst, ctx.cl);
                                return OP_NO_WRITE;
                        }
                    }));
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVUPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                    movups_rm:
                        FAULT_CHECK_SSE(ctx.binopRM_XX(pc, [](auto& dst, auto src) regcall {
                            dst = src;
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // MOVUPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_rm;
                    case OpcodeF3Prefix: // MOVSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MOVSD Rx, MxQ
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x111:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
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
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVUPS Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                    movups_mr:
                        FAULT_CHECK_SSE(ctx.binopMR_XX<float, OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                            dst = src;
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // MOVUPD Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_mr;
                    case OpcodeF3Prefix: // MOVSS MxD, Rx (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MOVSD MxQ, Rx (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x112:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2 && !ctx.CPUID_SSE3) {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // CLR1 Mb, CL
                                ctx.CLR1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVLPS Rx, MxQ
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVLPD Rx, MxQ
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVSLDUP Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<float>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.MOVLDUP(src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF2Prefix: // MOVDDUP Rx, MxQ
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        break;
                }
                break;
            case 0x113:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
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
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVLPS MxQ, Rx (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVLPD MxQ, Rx (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x114:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // SET1 Mb, CL
                                ctx.SET1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // UNPCKLPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                    unpcklps:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<float>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKL(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // UNPCKLPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                    unpcklpd:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<double>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKL(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x115:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
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
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // UNPCKHPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                    unpckhps:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<float>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKH(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // UNPCKHPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                    unpckhpd:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<double>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKH(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x116:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2 && !ctx.CPUID_SSE3) {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // NOT1 Mb, CL
                                ctx.NOT1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVHPS Rx, MxQ
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVHPD Rx, MxQ
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVSHDUP Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<float>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.MOVHDUP(src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x117:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM(pc, [](auto& dst, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // NOT1 Mv, CL
                                ctx.NOT1(dst, ctx.cl);
                                return OP_WRITE;
                        }
                    }));
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVHPS MxQ, Rx (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case Opcode66Prefix: // MOVHPD MxQ, Rx (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x118:
                if constexpr (ctx.OPCODES_V20) {
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto dst, uint8_t r) regcall {
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
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto& dst, uint8_t r) regcall {
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
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto& dst, uint8_t r) regcall {
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
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto& dst, uint8_t r) regcall {
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
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [&](auto& dst, uint8_t r) regcall {
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
                    GP_WITHOUT_CPL0();

                }
                else { // ADD4S
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);

                }
                break;
            case 0x121:
                if constexpr (ctx.OPCODES_80386) { // MOV M, DR
                    GP_WITHOUT_CPL0();

                }
                THROW_UD();
                break;
            case 0x122:
                if constexpr (ctx.OPCODES_80386) { // MOV CR, M
                    GP_WITHOUT_CPL0();

                }
                else { // SUB4S
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);

                }
                break;
            case 0x123:
                if constexpr (ctx.OPCODES_80386) { // MOV DR, M
                    GP_WITHOUT_CPL0();

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
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto& src, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // ROL4 Mb
                                ctx.ROL4(src);
                                return OP_WRITE;
                        }
                    }));
                    break;
                }
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVAPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        goto movups_rm;
                    case Opcode66Prefix: // MOVAPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_rm;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x129:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVAPS Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        goto movups_mr;
                    case Opcode66Prefix: // MOVAPD Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_mr;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x12A:
                if constexpr (!ctx.CPUID_SSE && !ctx.CPUID_SSE2) {
                    THROW_UD_WITHOUT_FLAG(ctx.OPCODES_V20);
                    FAULT_CHECK(ctx.unopM<OP_BYTE>(pc, [](auto& src, uint8_t r) regcall {
                        switch (r) {
                            default: unreachable;
                            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                                THROW_UD_GRP();
                            case 0: // ROR4 Mb
                                ctx.ROR4(src);
                                return OP_WRITE;
                        }
                    }));
                    break;
                }
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
                break;
            case 0x12B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVNTPS Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        goto movups_mr;
                    case Opcode66Prefix: // MOVNTPD Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_mr;
                    case OpcodeF3Prefix: // MOVNTSS MxD, Rx (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                    case OpcodeF2Prefix: // MOVNTSD MxQ, Rx (no read)
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
                    case OpcodeF3Prefix: // VUCOMXSS Rx, Mx
                    case OpcodeF2Prefix: // VUCOMXSD Rx, Mx
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
                    case OpcodeF3Prefix: // COMXSS Rx, Mx
                    case OpcodeF2Prefix: // COMXSD Rx, Mx
                        ALWAYS_UD();
                }
                break;
            case 0x130: // WRMSR
                GP_WITHOUT_CPL0();
                break;
            case 0x131: // RDTSC
                break;
            case 0x132: // RDMSR
                GP_WITHOUT_CPL0();
                break;
            case 0x133: // RDPMC
                break;
            case 0x134: // SYSENTER
            case 0x135: // SYSEXIT
            [[unlikely]] case 0x138: // Three byte opcodes A
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                map = 2;
                goto next_byte;
            [[unlikely]] case 0x13A: // Three byte opcodes B
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                map = 3;
                goto next_byte;
            case 0x141: // CMOVNO Rv, Mv
                // KAND Rk, Vk, Mk (VEX)
            case 0x140: // CMOVO Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondNO>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x142: // CMOVC Rv, Mv
                // KANDN Rk, Vk, Mk (VEX)
            case 0x143: // CMOVNC Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondNC>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x144: // CMOVZ Rv, Mv
                // KNOT Rk, Mk (VEX)
            case 0x145: // CMOVNZ Rv, Mv
                // KOR Rk, Vk, Mk (VEX)
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondNZ>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x146: // CMOVBE Rv, Mv
                // KXNOR Rk, Vk, Mk (VEX)
            case 0x147: // CMOVA Rv, Mv
                // KXOR Rk, Vk, Mk (VEX)
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondA>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x148: // CMOVS Rv, Mv
            case 0x149: // CMOVNS Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondNS>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x14A: // CMOVP Rv, Mv
                // KADD Rk, Vk, Mk (VEX)
            case 0x14B: // CMOVNP Rv, Mv
                // KUNPCK Rk, Vk, Mk (VEX)
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondNP>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x14C: // CMOVL Rv, Mv
            case 0x14D: // CMOVGE Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondGE>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x14E: // CMOVLE Rv, Mv
            case 0x14F: // CMOVG Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.CPUID_CMOV);
                FAULT_CHECK(ctx.binopRM(pc, [=](auto& dst, auto src) regcall {
                    ctx.CMOVCC<CondG>(dst, src, opcode_byte & 1);
                    return OP_WRITE;
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
                    case OpcodeF3Prefix: // SQRTSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // SQRTSD Rx, MxQ
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
                    case OpcodeF3Prefix: // RSQRTSS Rx, MxD
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
                    case OpcodeF3Prefix: // RCPSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x154:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // ANDPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        goto pand;
                    case Opcode66Prefix: // ANDPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto pand;
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
                        goto pandn;
                    case Opcode66Prefix: // ANDNPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto pandn;
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
                        goto por;
                    case Opcode66Prefix: // ORPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto por;
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
                        goto pxor;
                    case Opcode66Prefix: // XORPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto pxor;
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
                    case OpcodeF3Prefix: // ADDSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // ADDSD Rx, MxQ
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
                    case OpcodeF3Prefix: // MULSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MULSD Rx, MxQ
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
                    case OpcodeF3Prefix: // CVTSS2SD Rx, MxD
                        break;
                    case OpcodeF2Prefix: // CVTSD2SS Rx, MxQ
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
                    case OpcodeF3Prefix: // SUBSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // SUBSD Rx, MxQ
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
                    case OpcodeF3Prefix: // MINSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MINSD Rx, MxQ
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
                    case OpcodeF3Prefix: // DIVSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // DIVSD Rx, MxQ
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
                    case OpcodeF3Prefix: // MAXSS Rx, MxD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE);
                        break;
                    case OpcodeF2Prefix: // MAXSD Rx, MxQ
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                }
                break;
            case 0x160:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKLBW Rm, MmD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        /*
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKL(dst, src);
                            return OP_WRITE;
                        }));
                        */
                        break;
                    case Opcode66Prefix: // PUNPCKLBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKL(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x161:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKLWD Rm, MmD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        /*
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKL(dst, src);
                            return OP_WRITE;
                        }));
                        */
                        break;
                    case Opcode66Prefix: // PUNPCKLWD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKL(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x162:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PUNPCKLDQ Rm, MmD
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        /*
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKL(dst, src);
                            return OP_WRITE;
                        }));
                        */
                        break;
                    case Opcode66Prefix: // PUNPCKLDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto unpcklps;
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            return ctx.PACKSS(dst, src);
                        }));
                        break;
                    case Opcode66Prefix: // PACKSSWB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            return ctx.PACKSS(dst, src);
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPGT(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PCMPGTB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPGT(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPGT(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PCMPGTW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPGT(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPGT(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PCMPGTD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPGT(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            return ctx.PACKUS(dst, src);
                        }));
                        break;
                    case Opcode66Prefix: // PACKUSWB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            return ctx.PACKUS(dst, src);
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKH(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PUNPCKHBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKH(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKH(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PUNPCKHWD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKH(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PUNPCKH(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PUNPCKHDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto unpckhps;
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int32_t>(pc, [](auto& dst, auto src) regcall {
                            return ctx.PACKSS(dst, src);
                        }));
                        break;
                    case Opcode66Prefix: // PACKSSDW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            return ctx.PACKSS(dst, src);
                        }));
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
                        goto unpcklpd;
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
                        goto unpckhpd;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x16E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVD Rm, Mv (REXW = MOVQ Rm, Mv)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // MOVD Rx, Mv (REXW = MOVQ Rx, Mv)
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM(pc, [](auto& dst, auto src) regcall {
                            dst = src;
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // MOVDQA Rx, Mx
                    case OpcodeF3Prefix: // MOVDQU Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_rm;
                    case OpcodeF2Prefix: // VMOVDQU8 Rx, Mx
                        ALWAYS_UD();
                        goto movups_rm;
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPEQ(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PCMPEQB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPEQ(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPEQ(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PCMPEQW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPEQ(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPEQ(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PCMPEQD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPEQ(dst, src);
                            return OP_WRITE;
                        }));
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
                        // VZEROUPPER, VZEROALL (VEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        goto femms;
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
                        // SVDC M, Rs
                        // VCVTTPS2UDQ (EVEX)
                        ALWAYS_UD();
                    case Opcode66Prefix: // EXTRQ Mx, Ib, Ib
                        // VCVTTPS2UQQ (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                    case OpcodeF3Prefix:
                        // VCVTTSS2USI Rv, Mx (EVEX)
                        ALWAYS_UD();
                    case OpcodeF2Prefix: // INSERTQ Rx, Mx, Ib, Ib
                        // VCVTTSD2USI Rv, Mx (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                }
                break;
            case 0x179:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VMWRITE Rv, Mv
                        // RSDC Rs, M
                        // VCVTPS2UDQ (EVEX)
                        ALWAYS_UD();
                    case Opcode66Prefix: // EXTRQ Rx, Mx
                        // VCVTPS2UQQ (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                    case OpcodeF3Prefix:
                        // VCVTSS2USI Rv, Mx (EVEX)
                        ALWAYS_UD();
                    case OpcodeF2Prefix: // INSERTQ Rx, Mx
                        // VCVTSD2USI Rv, Mx (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE4A);
                        break;
                }
                break;
            case 0x17A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SVLDT M
                        ALWAYS_UD();
                    case Opcode66Prefix: // VCVTTPS2QQ (EVEX)
                    case OpcodeF3Prefix: // VCVTUDQ2PD (EVEX)
                    case OpcodeF2Prefix: // VCVTUDQ2PS (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x17B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // RSLDT M
                        ALWAYS_UD();
                    case Opcode66Prefix: // VCVTPS2QQ (EVEX)
                    case OpcodeF3Prefix: // VCVTUSI2SS (EVEX)
                    case OpcodeF2Prefix: // VCVTUSI2SD (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x17C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SVTS M
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
                    case OpcodeNoPrefix: // RSTS M
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
                    case OpcodeNoPrefix: // MOVD Mv, Rm (REXW = MOVQ Mv, Rm) (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        break;
                    case Opcode66Prefix: // MOVD Mv, Rx (REXW = MOVQ Mv, Rx) (no read)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVQ Rx, MxQ
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
                    movq_mmx:
                        FAULT_CHECK_MMX(ctx.binopMR_MM<uint64_t, OP_NO_READ>(pc, [](auto& dst, auto src) regcall {
                            dst = src;
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // MOVDQA Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_mr;
                    case OpcodeF3Prefix: // MOVDQU Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_mr;
                    case OpcodeF2Prefix: // VMOVDQU8 Mx, Rx
                        ALWAYS_UD();
                        goto movups_mr;
                }
                break;
            case 0x180: // JO Jz
            case 0x181: // JNO Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondNO>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x182: // JC Jz
            case 0x183: // JNC Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondNC>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x184: // JZ Jz
            case 0x185: // JNZ Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondNZ>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x186: // JBE Jz
            case 0x187: // JA Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondA>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x188: // JS Jz
            case 0x189: // JNS Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondNS>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x18A: // JP Jz
            case 0x18B: // JNP Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondNP>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x18C: // JL Jz
            case 0x18D: // JGE Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondGE>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x18E: // JLE Jz
            case 0x18F: // JG Jz
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.JCC<CondG>(pc, opcode_byte & 1));
                goto next_instr;
            case 0x190: // SETO Mb
                // KMOV Rk, Mk (VEX)
            case 0x191: // SETNO Mb
                // KMOV Mk, Rk (VEX)
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNO>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x192: // SETC Mb
                // KMOV Rk, Mv
            case 0x193: // SETNC Mb
                // KMOV Rv, Mk
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNC>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x194: // SETZ Mb
            case 0x195: // SETNZ Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNZ>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x196: // SETBE Mb
            case 0x197: // SETA Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondA>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x198: // SETS Mb
                // KORTEST Rk, Mk (VEX)
            case 0x199: // SETNS Mb
                // KTEST Rk, Mk (VEX)
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNS>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x19A: // SETP Mb
            case 0x19B: // SETNP Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondNP>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x19C: // SETL Mb
            case 0x19D: // SETGE Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondGE>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x19E: // SETLE Mb
            case 0x19F: // SETG Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.unopM<OP_BYTE | OP_NO_READ>(pc, [=](auto& dst, uint8_t r) regcall {
                    ctx.SETCC<CondG>(dst, opcode_byte & 1);
                    return OP_WRITE;
                }));
                break;
            case 0x1A0: case 0x1A8: // PUSH FS/GS
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.PUSH(ctx.get_seg(FS + (bool)(opcode_byte & 8))));
                break;
            case 0x1A1: case 0x1A9: // POP FS/GS
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.write_seg(FS + (bool)(opcode_byte & 8), ctx.POP()));
                break;
            case 0x1A2: // CPUID
                // TODO
                break;
            case 0x1A3: // BT Mv, Gv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMRB(pc, [](auto dst, auto src) regcall {
                    ctx.BT(dst, src);
                    return OP_NO_WRITE;
                }));
                break;
            case 0x1A4: // SHLD Mv, Gv, Ib
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [&](auto& dst, auto src) regcall {
                    ctx.SHLD(dst, src, pc.read<uint8_t>());
                    return OP_WRITE;
                }));
                ++pc;
                break;
            case 0x1A5: // SHLD Mv, Gv, CL
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [](auto& dst, auto src) regcall {
                    ctx.SHLD(dst, src, ctx.cl);
                    return OP_WRITE;
                }));
                break;
            case 0x1A6:
                if constexpr (ctx.HAS_OLD_CMPXCHG) {
                    goto cmpxchgb;
                }
                else { // XBTS Rv, Mv, AX, CL
                    THROW_UD_WITHOUT_FLAG(ctx.HAS_XBTS_IBTS);
                    FAULT_CHECK(ctx.XBTS(pc));
                }
                break;
            case 0x1A7:
                if constexpr (ctx.HAS_OLD_CMPXCHG) {
                    goto cmpxchgv;
                }
                else { // IBTS Mv, AX, CL, Rv
                    THROW_UD_WITHOUT_FLAG(ctx.HAS_XBTS_IBTS);
                    FAULT_CHECK(ctx.IBTS(pc));
                }
                break;
            case 0x1AA: // RSM
                // TODO
                break;
            case 0x1AB: // BTS Mv, Rv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMRB(pc, [](auto& dst, auto src) regcall {
                    ctx.BTS(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1AC: // SHRD Mv, Rv, Ib
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [&](auto dst, auto src) regcall {
                    ctx.SHRD(dst, src, pc.read<uint8_t>());
                    return OP_WRITE;
                }));
                ++pc;
                break;
            case 0x1AD: // SHRD Mv, Rv, CL
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMR(pc, [](auto dst, auto src) regcall {
                    ctx.SHRD(dst, src, ctx.cl);
                    return OP_WRITE;
                }));
                break;
            case 0x1AE: // GRP15
                // TODO
                break;
            case 0x1AF: // IMUL Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.IMUL(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1B0: // CMPXCHG Mb, Rb
                THROW_UD_WITHOUT_FLAG(!ctx.HAS_OLD_CMPXCHG);
            cmpxchgb:
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80486);
                break;
            case 0x1B1: // CMPXCHG Mv, Rb
                THROW_UD_WITHOUT_FLAG(!ctx.HAS_OLD_CMPXCHG);
            cmpxchgv:
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80486);
                // TODO
                break;
            case 0x1B2: // LSS Rv, M
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopRMF(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return ctx.write_seg(SS, src >> (bitsof(src) >> 1)) ? OP_FAULT : OP_WRITE;
                }));
                break;
            case 0x1B3: // BTR Mv, Rv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopMRB(pc, [](auto& dst, auto src) regcall {
                    ctx.BTR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1B4: case 0x1B5: // LFS/LGS Rv, M
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.binopRMF(pc, [=](auto& dst, auto src) regcall {
                    dst = src;
                    return ctx.write_seg(FS + (opcode_byte & 1), src >> (bitsof(src) >> 1)) ? OP_FAULT : OP_WRITE;
                }));
                break;
            case 0x1B6: // MOVZX Rv, Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.MOVX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_NOT_MEM;
                }));
                break;
            case 0x1B7: // MOVZX Rv, Mw
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.MOVX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                    dst = src;
                    return OP_NOT_MEM;
                }));
                break;
            case 0x1B8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case Opcode66Prefix: // POPCNT Rv, Mv
                        if (ctx.rep_type != REP_E) {
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
                FAULT_CHECK(ctx.binopMRB(pc, [](auto& dst, auto src) regcall {
                    ctx.BTC(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1BC: // BSF Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                if (ctx.rep_type > 0) { // TZCNT Rv, Mv
                    // TODO
                }
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.BSF(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1BD: // BSR Rv, Mv
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                if (ctx.rep_type > 0) { // LZCNT Rv, Mv
                    // TODO
                }
                FAULT_CHECK(ctx.binopRM(pc, [](auto& dst, auto src) regcall {
                    ctx.BSR(dst, src);
                    return OP_WRITE;
                }));
                break;
            case 0x1BE: // MOVSX Rv, Mb
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.MOVX<int8_t>(pc, [](auto& dst, auto src) regcall {
                    using S = decltype(src);
                    using D = std::remove_reference_t<decltype(dst)>;
                    dst = (D)(S)src;
                    return OP_NOT_MEM;
                }));
                break;
            case 0x1BF: // MOVSX Rv, Mw
                THROW_UD_WITHOUT_FLAG(ctx.OPCODES_80386);
                FAULT_CHECK(ctx.MOVX<int16_t>(pc, [](auto& dst, auto src) regcall {
                    using S = decltype(src);
                    using D = std::remove_reference_t<decltype(dst)>;
                    dst = (D)(S)src;
                    return OP_NOT_MEM;
                }));
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
                FAULT_CHECK(ctx.unopR(pc, opcode_byte & 7, [](auto& dst) regcall {
                    ctx.BSWAP(dst);
                    return OP_WRITE;
                }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSRLW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSRLD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSRLQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1D5:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMULLW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMULL(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMULLW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMULL(dst, src);
                            return OP_WRITE;
                        }));
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
                    case Opcode66Prefix: // MOVQ MxQ, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        break;
                    case OpcodeF3Prefix: // MOVQ2DQ Rx, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        break;
                    case OpcodeF2Prefix: // MOVDQ2Q Rm, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSE2);
                        FAULT_CHECK_MMX_SSE(ctx.binopRM_MX<uint64_t>(pc, [](auto& dst, auto src) {
                            dst[0] = src[0];
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSUBS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBUSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBUSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMINUB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PAND(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PAND Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                    pand:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PAND(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDUSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDUSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMAXUB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PANDN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PANDN Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                    pandn:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PANDN(dst, src);
                            return OP_WRITE;
                        }));
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
                    pavgb_mmx:
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PAVG(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PAVGB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PAVG(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSRAW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSRAD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHR(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PAVG(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PAVGW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PAVG(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMULH(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMULHUW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMULH(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMULH(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMULHW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMULH(dst, src);
                            return OP_WRITE;
                        }));
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
                        goto movq_mmx;
                    case Opcode66Prefix: // MOVNTDQ Mx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        goto movups_mr;
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSUBS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMINSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.POR(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // POR Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                    por:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.POR(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADDS(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMAXSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PXOR(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PXOR Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                    pxor:
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PXOR(dst, src);
                            return OP_WRITE;
                        }));
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
                        goto movups_rm;
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x1F1:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSLLW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHL(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSLLW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHL(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHL(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSLLD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHL(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHL(dst, std::bit_cast<uint64_t>(src));
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSLLQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSHL(dst, std::bit_cast<vec<uint64_t, sizeof(src) / sizeof(uint64_t)>>(src)[0]);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMUL(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMULUDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMUL(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PMADDWD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMADD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSAD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSADBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSAD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int8_t>(pc, [](auto dst, auto src) regcall {
                            return ctx.MASKMOV(dst, src);
                        }));
                        break;
                    case Opcode66Prefix: // MASKMOVDQU Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto dst, auto src) regcall {
                            return ctx.MASKMOV(dst, src);
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PSUB(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint64_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSUBQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint64_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint8_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint8_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PADDD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE2);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall{
                            dst = ctx.PADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
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
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PHADDW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x202:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PHADDD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PHADDD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHADD(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x203:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PHADDSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PHADDSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHADDS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x204:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMADDUBSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        break;
                    case Opcode66Prefix: // PMADDUBSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x205:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PHSUBW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHSUB(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PHSUBW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHSUB(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x206:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PHSUBD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHSUB(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PHSUBD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHSUB(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x207:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PHSUBSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHSUBS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PHSUBSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PHSUBS(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x208:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSIGNB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSIGN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSIGNB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSIGN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x209:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSIGNW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSIGN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSIGNW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSIGN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x20A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PSIGND Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSIGN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PSIGND Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PSIGN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x20B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PMULHRSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        break;
                    case Opcode66Prefix: // PMULHRSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x20C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPERMILPS Rx, Vx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x20D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPERMILPD Rx, Vx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x20E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VTESTPS Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x20F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VTESTPD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x210:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PBLENDVB Rx, Mx, XMM0
                        // VPSRLVW Rx, Vx, Mx (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVUSWB Mx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x211:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSRAVW Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix: // VPMOVUSDB Mx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x212:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSLLVW Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix: // VPMOVUSQB Mx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x213:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VCVTPH2PS Rx, Mx (F16C)
                    case OpcodeF3Prefix: // VPMOVUSDW Mx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x214:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // BLENDVPS Rx, Mx, XMM0
                        // VPRORVD Rx, Vx, Mx (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVUSDW Mx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x215:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // BLENDVPD Rx, Mx, XMM0
                        // VPROLVD Rx, Vx, Mx (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVUSQD Mx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x216:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPERMPS Rx, Vx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x217:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PTEST Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x218:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VBROADCASTSS Rx, M
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x219:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VBROADCASTSD Rx, M
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x21A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VBROADCASTF128 Rx, M
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x21B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VBROADCASTF32X8 Rx, M (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x21C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PABSB Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PABSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x21D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PABSW Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PABSW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x21E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // PABSD Rm, Mm
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_MMX && ctx.CPUID_SSSE3);
                        FAULT_CHECK_MMX(ctx.binopRM_MM<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case Opcode66Prefix: // PABSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSSE3);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x21F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPABSQ Rx, Mx (EVEX)
                        ALWAYS_UD();
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x220:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVSXBW Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVSWB Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x221:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVSXBD Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVSDB Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x222:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVSXBQ Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVSQB Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x223:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVSXWD Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVSDW Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x224:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVSXWQ Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVSQW Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x225:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVSXDQ Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVSQD Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x226:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPTESTMB Rk, Vx, Mx (EVEX)
                    case OpcodeF3Prefix: // VPTESTNMB Rk, Vx, Mx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x227:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPTESTMD Rk, Vx, Mx (EVEX)
                    case OpcodeF3Prefix: // VPTESTNMD Rk, Vx, Mx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x228:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMULDQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PABS(src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix: // VPMOVM2B Rx, Mk (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x229:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PCMPEQQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPEQ(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix: // VPMOVM2B Rx, Mk (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x22A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // MOVNTDQA Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        goto movups_rm;
                    case OpcodeF3Prefix: // VPBROADCASTMB2Q Rx, Mk (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x22B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PACKUSDW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            return ctx.PACKUS(dst, src);
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x22C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VMASKMOVPS Rx, Vx, Mx
                        // VSCALEFPS Rx, Vx, Mx (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x22D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VMASKMOVPD Rx, Vx, Mx
                        // VSCALEFSS Rx, Vx, Mx (EVEX)
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x22E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VMASKMOVPS Mx, Vx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x22F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VMASKMOVPD Mx, Vx, Rx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x230:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVZXBW Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVWB Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x231:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVZXBD Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVDB Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x232:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVZXBQ Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVQB Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x233:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVZXWD Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVDW Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x234:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVZXWQ Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVQW Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x235:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // PMOVZXDQ Rx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix: // VPMOVQD Wx, Rx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x236:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPERMD Rx, Vx, Wx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x237:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PCMPGTQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int64_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PCMPGT(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x238:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMINSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix: // VPMOVM2D Rx, Mk (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x239:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMINSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix: // VPMOVD2M Rx, Mk (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x23A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMINUW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix: // VPBROADCASTMW2D Rx, Mk (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x23B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMINUD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMIN(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x23C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMAXSB Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int8_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x23D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMAXSD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<int32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x23E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMAXUW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint16_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x23F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMAXUD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMAX(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x240:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PMULLD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        FAULT_CHECK_SSE(ctx.binopRM_XX<uint32_t>(pc, [](auto& dst, auto src) regcall {
                            dst = ctx.PMULL(dst, src);
                            return OP_WRITE;
                        }));
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x241:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // PHMINPOSUW Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_SSE41);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x242:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VGETEXPPS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x243:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VGETEXPSS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x244:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VPLZCNTB Rx, Mx (EVEX)
                    case Opcode66Prefix: // VPLZCNTD Rx, Mx (EVEX)
                    case OpcodeF3Prefix: // VPTZCNTB Rx, Mx (EVEX)
                    case OpcodeF2Prefix: // VPTZCNTD Rx, Mx (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x245:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPSRLVD Rx, Vx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x246:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPSRAVD Rx, Vx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x247:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPSLLVD Rx, Vx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x248:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // TTMMULTF32PS Rt, Mt, Vt (AMX)
                    case Opcode66Prefix: // TMMULTF32PS Rt, Mt, Vt (AMX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x249:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // LDTILECFG M, TILERELEASE (AMX)
                    case Opcode66Prefix: // STTILECFG M (AMX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix: // TILEZERO Rt
                        ALWAYS_UD();
                }
                break;
            case 0x24A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // TILELOADDRST1 Rt, M (AMX)
                        // TILEMOVROW Rx, Mt, Vv (AMX, EVEX)
                    case OpcodeF2Prefix: // TILELOADDRS Rt, M (AMX)
                    case OpcodeF3Prefix: // TCVTROWD2PS Rx, Mt, Vv (AMX, EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x24B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // TILELOADDT1 Rt, M (AMX)
                        // TILEMOVROW Rx, Mt, Vv (AMX, EVEX)
                    case OpcodeF2Prefix: // TILELOADD Rt, M (AMX)
                    case OpcodeF3Prefix: // TILESTORED M, Rt
                        ALWAYS_UD();
                }
                break;
            case 0x24C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VRCP14PS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x24D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VRCP14SS Rx, MxD (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x24E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VRSQRT14PS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x24F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VRSQRT14SS Rx, MxD (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x250:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VPDPBUUD Rx, Vx, Mx (AVX VNNI)
                    case Opcode66Prefix: // VPDPBUSD Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF3Prefix: // VPDPBSUD Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF2Prefix: // VPDPBSSD Rx, Vx, Mx (AVX VNNI)
                        ALWAYS_UD();
                }
                break;
            case 0x251:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VPDPBUUDS Rx, Vx, Mx (AVX VNNI)
                    case Opcode66Prefix: // VPDPBUSDS Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF3Prefix: // VPDPBSUDS Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF2Prefix: // VPDPBSSDS Rx, Vx, Mx (AVX VNNI)
                        ALWAYS_UD();
                }
                break;
            case 0x252:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VDPPHPS Rx, Vx, Mx (AVX 10.2)
                    case Opcode66Prefix: // VPDPWSSD Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF3Prefix: // VDPBF16PS Rx, Vx, Mx (AVX 10.2)
                    case OpcodeF2Prefix: // VP4DPWSSD Rx, Vx, Mx (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x253:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPDPWSSDS Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix: // VP4DPWSSDS Rx, Vx, Mx (EVX)
                        ALWAYS_UD();
                }
                break;
            case 0x254:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPOPCNTB Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x255:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPOPCNTD Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x258:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBROADCASTD Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x259:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBROADCASTQ Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x25A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VBROADCASTI128 Rx, Mx
                        THROW_UD_WITHOUT_FLAG(ctx.CPUID_AVX2);
                        break;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x25B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VBROADCASTI32X8 Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x25C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix:
                    case OpcodeF3Prefix: // TDPBF16PS Rt, Mt, Vt (AMX)
                    case OpcodeF2Prefix: // TDPFP16PS Rt, Mt, Vt (AMX)
                        ALWAYS_UD();
                }
                break;
            case 0x25E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // TDPBUUD Rt, Mt, Vt (AMX)
                    case Opcode66Prefix: // TDPBUSD Rt, Mt, Vt (AMX)
                    case OpcodeF3Prefix: // TDPBSUD Rt, Mt, Vt (AMX)
                    case OpcodeF2Prefix: // TDPBSSD Rt, Mt, Vt (AMX)
                        ALWAYS_UD();
                }
                break;
            case 0x25F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix:
                    case OpcodeF3Prefix: // TTRANSPOSED Rt, Mt (AMX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x262:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPEXPANDB Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x263:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPCOMPRESSB Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x264:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBLENDMD Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x265:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VBLENDMPS Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x266:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBLENDMB Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x267:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VCVT2PS2PHX Rx, Vx, Mx (AVX 10.2)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x268:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix: // VP2INTERSECTD (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x26B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // TCONJTCMMIMFP16PS Rt, Mt, Vt (AMX)
                    case Opcode66Prefix: // TCONJTFP16 Rt, Mt (AMX)
                    case OpcodeF3Prefix: // TTCMMRLFP16PS Rt, Mt, Vt (AMX)
                    case OpcodeF2Prefix: // TTCMMIMFP16PS Rt, Mt, Vt (AMX)
                        ALWAYS_UD();
                }
                break;
            case 0x26C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // TCMMRLFP16PS Rt, Mt, Vt (AMX)
                    case Opcode66Prefix: // TCMMIMFP16PS Rt, Mt, Vt (AMX)
                    case OpcodeF3Prefix: // TTDPBF16PS Rt, Mt, Vt (AMX)
                    case OpcodeF2Prefix: // TTDPFP16PS Rt, Mt, Vt (AMX)
                        ALWAYS_UD();
                }
                break;
            case 0x26D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // TCVTROWPS2PHH Rx, Mt, Vv (AMX, EVEX)
                    case Opcode66Prefix: // TCVTROWPS2PHL Rx, Mt, Vv (AMX, EVEX)
                    case OpcodeF3Prefix: // TCVTROWPS2PBF16L Rt, Mt, Vt (AMX, EVEX)
                    case OpcodeF2Prefix: // TCVTROWPS2PBF16H Rt, Mt, Vt (AMX, EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x26E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // T2RPNTLVWZ0 Rt, M (AMX)
                t2rpntlvwz0:
                    case Opcode66Prefix: // T2RPNTLVWZ1 Rt, M (AMX)
                t2rpntlvwz1:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x26F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // T2RPNTLVWZ0 Rt, M (AMX)
                        goto t2rpntlvwz0;
                    case Opcode66Prefix: // T2RPNTLVWZ1 Rt, M (AMX)
                        goto t2rpntlvwz1;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x270:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSHLDVW Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x271:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSHLDVD Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x272:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSHRDVW Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix: // VCVTNEPS2BF16 Rx, Mx (AVX)
                    case OpcodeF2Prefix: // VCVTNE2PS2BF16 Rx, Vx, Mx (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x273:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSHRDVD Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x274:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VCVTBIASPH2BF8 Rx, Vx, Mx (EVEX, AVX 10.2)
                    case Opcode66Prefix:
                    case OpcodeF3Prefix: // VCVTNEPH2BF8 Rx, Mx (EVEX, AVX 10.2)
                    case OpcodeF2Prefix: // VCVTNE2PH2BF8 Rx, Vx, Mx (EVEX, AVX 10.2)
                        ALWAYS_UD();
                }
                break;
            case 0x275:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPERMI2B Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x276:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                        ALWAYS_UD();
                    case Opcode66Prefix: // VPERMI2D Rx, Vx, Mx (EVEX)
                        goto vpermi2ps;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x277:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPERMI2PS Rx, Vx, Mx (EVEX)
                vpermi2ps:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x278:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBROADCASTB Rx, M (AVX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x279:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBROADCASTW Rx, M (AVX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x27A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBROADCASTB Rx, Mb (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x27B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBROADCASTW Rx, Mw (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x27C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPBROADCASTD Rx, Mv (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x27D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPERMT2B Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x27E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPERMT2D Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x27F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPERMT2PS Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x280:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // INVEPT Rv, M
                    case OpcodeF3Prefix: // VPADDSD Rx, Vx, Mx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x281:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // INVVPID Rv, M
                    case OpcodeF3Prefix: // VPADDUSD Rx, Vx, Mx (EVEX)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x282:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // INVPCID Rv, M
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x283:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPMULTISHIFTQB Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x288:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VEXPANDPS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x289:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPEXPANDD Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x28A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVRS Rb, Mb
                        ALWAYS_UD();
                        goto mov_rmb;
                    case Opcode66Prefix: // VCOMPRESSPS Mx, Rx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x28B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // MOVRS Rv, Mv
                    case Opcode66Prefix:
                        // VPCOMPRESSD Mx, Rx (EVEX)
                        ALWAYS_UD();
                        goto mov_rm;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x28C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPMASKMOVD Rx, Vx, Mx (AVX2)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x28D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPERMB Rx, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x28E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPMASKMOVD Mx, Vx, Rx (AVX2)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x28F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSHUFBITQMB Rk, Vx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x290:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPGATHERDD Rx, Mx, Vx (AVX2)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x291:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPGATHERQD Rx, Mx, Vx (AVX2)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x292:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VGATHERDPS Rx, Mx, Vx (AVX2)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x293:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VGATHERQPS Rx, Mx, Vx (AVX2)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x296:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADDSUB132PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x297:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUBADD132PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x298:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADD132PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x299:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADD132SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x29A:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUB132PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix: // V4FMADDPS (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x29B:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUB132SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix: // V4FMADDSS (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x29C:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMADD132PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x29D:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMADD132SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x29E:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMSUB132PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x29F:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMSUB132SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A0:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSCATTERDD M, Rx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A1:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPSCATTERQD M, Rx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A2:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VSCATTERDPS M, Rx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A3:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VSCATTERQPS M, Rx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A6:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADDSUB213PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A7:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUBADD213PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADD213PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2A9:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADD213SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2AA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUB213PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix: // V4FNMADDPS (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x2AB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUB213SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix: // V4FNMADDSS (EVEX)
                        ALWAYS_UD();
                }
                break;
            case 0x2AC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMADD213PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2AD:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMADD213SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2AE:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMSUB213PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2AF:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMSUB213SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2B0:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VCVTNEOPH2PS Rx, Mx (AVX NE CONVERT)
                    case Opcode66Prefix: // VCVTNEEPH2PS Rx, Mx (AVX NE CONVERT)
                    case OpcodeF3Prefix: // VCVTNEEBF162PS Rx, Mx (AVX NE CONVERT)
                    case OpcodeF2Prefix: // VCVTNEOBF162PS Rx, Mx (AVX NE CONVERT)
                        ALWAYS_UD();
                }
                break;
            case 0x2B1:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VBCSTNEPH2PS Rx, Mw (AVX NE CONVERT)
                    case OpcodeF3Prefix: // VBCSTNEBF162PS Rx, Mw (AVX NE CONVERT)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2B4:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPMADD52LUQ Rx, Vx, Mx (AVX IFMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2B5:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPMADD52HUQ Rx, Vx, Mx (AVX IFMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2B6:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADDSUB231PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2B7:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUBADD231PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2B8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADD231PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2B9:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMADD231SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2BA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUB231PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2BB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFMSUB231SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2BC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMADD231PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2BD:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMADD231SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2BE:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMSUB231PS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2BF:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VFNMSUB231SS Rx, Vx, Mx (FMA)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2C4:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // VPCONFLICTD Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2C6: // GRP18
            case 0x2C7: // GRP18
                ALWAYS_UD();
            case 0x2C8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SHA1NEXTE Rx, Mx (SHA)
                    case Opcode66Prefix: // VEXP2PS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2C9:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SHA1MSG1 Rx, Mx (SHA)
                    case Opcode66Prefix:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2CA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SHA1MSG2 Rx, Mx (SHA)
                    case Opcode66Prefix: // VRCP28PS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2CB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SHA256RNDS2 Rx, Mx (SHA)
                    case Opcode66Prefix: // VRCP28SS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2CC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SHA256MSG1 Rx, Mx (SHA)
                    case Opcode66Prefix: // VRSQRT28PS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2CD:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // SHA256MSG2 Rx, Mx (SHA)
                    case Opcode66Prefix: // VRSQRT28SS Rx, Mx (EVEX)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2CF:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // GF2P8MULB Rx, Mx (GFNI)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2D2:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VPDPWUUD Rx, Vx, Mx (AVX VNNI)
                    case Opcode66Prefix: // VPDPWUSD Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF3Prefix: // VPDPWSUD Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2D3:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VPDPWUUDS Rx, Vx, Mx (AVX VNNI)
                    case Opcode66Prefix: // VPDPWUSDS Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF3Prefix: // VPDPWSUDS Rx, Vx, Mx (AVX VNNI)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2D8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix:
                    case OpcodeF3Prefix: // GRP19
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2DA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // VSM3MSG1 Rx, Vx, Mx (SM3)
                    case Opcode66Prefix: // VSM3MSG2 Rx, Vx, Mx (SM3)
                    case OpcodeF3Prefix: // VSM4KEY4 Rx, Vx, Mx (SM4)
                    case OpcodeF2Prefix: // VSM4RNDS4 Rx, Vx, Mx (SM4)
                        ALWAYS_UD();
                }
                break;
            case 0x2DB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // AESIMC Rx, Mx (AES)
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2DC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // AESENC Rx, Mx (AES)
                    case OpcodeF3Prefix: // AESENC128KL Rx, M (KL)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2DD:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // AESENCLAST Rx, Mx (AES)
                    case OpcodeF3Prefix: // AESDEC128KL Rx, M (KL)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2DE:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // AESDEC Rx, Mx (AES)
                    case OpcodeF3Prefix: // AESENC256KL Rx, M (KL)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2DF:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // AESDECLAST Rx, Mx (AES)
                    case OpcodeF3Prefix: // AESDEC256KL Rx, M (KL)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2E0: // CMPOXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2E1: // CMPNOXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2E2: // CMPCXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2E3: // CMPNCXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2E4: // CMPZXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2E5: // CMPNZXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2E6: // CMPBEXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2E7: // CMPAXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2E8: // CMPSXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2E9: // CMPNSXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2EA: // CMPPXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2EB: // CMPNPXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2EC: // CMPLXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2ED: // CMPGEXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2EE: // CMPLEXADD Mv, Rv, Vv (CMPccXADD)
            case 0x2EF: // CMPGXADD Mv, Rv, Vv (CMPccXADD)
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // 
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2F0:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case Opcode66Prefix:
                        if (ctx.rep_type == REP_NE) {
                    case OpcodeF2Prefix: // CRC32 Rv, Mb
                        }
                        else {
                    case OpcodeNoPrefix: // MOVBE Rv, Mv
                        }
                        ALWAYS_UD();
                        break;
                    case OpcodeF3Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2F1:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case Opcode66Prefix:
                        if (ctx.rep_type == REP_NE) {
                    case OpcodeF2Prefix: // CRC32 Rv, Mv
                        } else {
                    case OpcodeNoPrefix: // MOVBE Rv, Mv
                        }
                        ALWAYS_UD();
                        break;
                    case OpcodeF3Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2F2:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // ANDN Rv, Mv, Vv (BMI)
                    case Opcode66Prefix:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2F3:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // GRP17
                    case Opcode66Prefix:
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2F5:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // BZHI Rv, Mv, Vv (BMI2)
                    case Opcode66Prefix: // WRUSSD Mv, Rv (CET)
                    case OpcodeF3Prefix: // PEXT Rv, Vv, Mv (BMI2)
                    case OpcodeF2Prefix: // PDEP Rv, Vv, Mv (BMI2)
                        ALWAYS_UD();
                }
                break;
            case 0x2F6:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // WRSSD Mv, Rv (CET)
                    case Opcode66Prefix: // ADCX Rv, Mv (ADX)
                    case OpcodeF3Prefix: // ADOX Rv, Mv (ADX)
                    case OpcodeF2Prefix: // MULX Rv, Vv, Mv (BMI2)
                        ALWAYS_UD();
                }
                break;
            case 0x2F7:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // BEXTR Rv, Mv, Vv (BMI)
                    case Opcode66Prefix: // SHLX Rv, Mv, Vv (BMI2)
                    case OpcodeF3Prefix: // SARX Rv, Mv, Vv (BMI2)
                    case OpcodeF2Prefix: // SHRX Rv, Vv, Mv (BMI2)
                        ALWAYS_UD();
                }
                break;
            case 0x2F8:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // MOVDIR64B Rv, M
                    case OpcodeF3Prefix: // (ENQCMDS Rv, M), UWRMSR
                    case OpcodeF2Prefix: // (ENQCMD Rv, M), URDMSR
                        ALWAYS_UD();
                }
                break;
            case 0x2F9:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix: // MOVDIRI Mv, Rv
                        ALWAYS_UD();
                        goto mov_mr;
                    case OpcodeF3Prefix:
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2FA:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix:
                    case OpcodeF3Prefix: // ENCODEKEY128 (KL)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2FB:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix:
                    case Opcode66Prefix:
                    case OpcodeF3Prefix: // ENCODEKEY256 (KL)
                    case OpcodeF2Prefix:
                        ALWAYS_UD();
                }
                break;
            case 0x2FC:
                switch (ctx.opcode_select()) {
                    default: unreachable;
                    case OpcodeNoPrefix: // AADD Mv, Rv (RAO INT)
                    case Opcode66Prefix: // AAND Mv, Rv (RAO INT)
                    case OpcodeF3Prefix: // AXOR Mv, Rv (RAO INT)
                    case OpcodeF2Prefix: // AOR Mv, Rv (RAO INT)
                        ALWAYS_UD();
                }
                break;

            default:
                //ALWAYS_UD();
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