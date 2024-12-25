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

struct x86Context : z86Base<16> {

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

    inline constexpr void set_seg_override(uint8_t seg) {
        this->seg_override = seg & 3;
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

    // TODO: rep prefix negating inputs of imul/idiv

    template <typename T>
    inline void IMUL(T src) {
        using SD = std::make_signed_t<dbl_int_t<T>>;
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;

        SD temp = this->A<T>();
        if (this->rep_type > NO_REP) {
            temp = -temp;
        }
        temp *= src;
        this->write_AD(temp);
        this->carry = this->overflow = (U)(temp >> bitsof(T)) != (S)temp >> bitsof(T) - 1;
    }

    template <typename T>
    inline void IDIV(T src) {
        if (src) {
            using SD = std::make_signed_t<dbl_int_t<T>>;
            using S = std::make_signed_t<T>;
            using U = std::make_unsigned_t<T>;

            SD temp = this->read_AD<T>();
            SD quot = temp / src;

            if (this->rep_type > NO_REP) {
                quot = -quot;
            }

            this->A<T>() = quot;
            this->ADH<T>() = temp % src;
            if ((U)(quot - (std::numeric_limits<S>::min)()) > (std::numeric_limits<U>::max)()) {
                this->software_interrupt(IntDE);
            }
        }
        else {
            this->software_interrupt(IntDE);
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
        this->PUSH(this->ip);

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

static x86Context ctx;

using z86Addr = z86AddrImpl<16>;
using ModRM = ModRMBase<ctx>;

#include "z86_core_internal_post.h"

dllexport size_t z86_mem_write(size_t dst, const void* src, size_t size) {
    return mem.write(dst, src, size);
}
dllexport size_t z86_mem_read(void* dst, size_t src, size_t size) {
    return mem.read(dst, src, size);
}

static std::vector<PortDevice*> io_devices;

static void port_out(uint16_t port, uint16_t value) {
    const std::vector<PortDevice*>& devices = io_devices;
    for (auto device : devices) {
        if (device->out(port, value)) {
            return;
        }
    }
    printf("Unhandled: OUT %X, %04X\n", port, value);
}

static void port_in(uint16_t& value, uint16_t port) {
    // TODO: Check default value
    value = 0;

    const std::vector<PortDevice*>& devices = io_devices;
    for (auto device : devices) {
        if (device->in(value, port)) {
            return;
        }
    }
    printf("Unhandled: IN %04X, %X\n", value, port);
}

dllexport void z86_add_device(PortDevice* device) {
    io_devices.push_back(device);
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

    for (;;) {
        // Reset per-instruction states
        ctx.seg_override = -1;
        ctx.rep_type = NO_REP;
        ctx.lock = false;


        z86Addr pc = ctx.pc();
        // TODO: The 6 byte prefetch cache
        // TODO: Clock cycles
    next_byte:
        uint8_t opcode = pc.read_advance();
        switch (opcode) {
            case 0x00: // ADD Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    ctx.ADD(dst, src);
                    return true;
                });
                break;
            case 0x01: // ADD Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.ADD(dst, src);
                    return true;
                });
                break;
            case 0x02: // ADD Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    ctx.ADD(dst, src);
                    return true;
                });
                break;
            case 0x03: // ADD Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.ADD(dst, src);
                    return true;
                });
                break;
            case 0x04: // ADD AL, Ib
                ctx.ADD(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x05: // ADD AX, Iz
                ctx.ADD(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x06: case 0x0E: case 0x16: case 0x1E: // PUSH seg
                ctx.PUSH(ctx.index_seg(opcode >> 3));
                break;
            case 0x07: case 0x0F: case 0x17: case 0x1F: // POP seg
                ctx.index_seg(opcode >> 3) = ctx.POP();
                break;
            case 0x08: // OR Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    ctx.OR(dst, src);
                    return true;
                });
                break;
            case 0x09: // OR Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.OR(dst, src);
                    return true;
                });
                break;
            case 0x0A: // OR Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    ctx.OR(dst, src);
                    return true;
                });
                break;
            case 0x0B: // OR Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.OR(dst, src);
                    return true;
                });
                break;
            case 0x0C: // OR AL, Ib
                ctx.OR(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x0D: // OR AX, Iz
                ctx.OR(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x10: // ADC Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    ctx.ADC(dst, src);
                    return true;
                });
                break;
            case 0x11: // ADC Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.ADC(dst, src);
                    return true;
                });
                break;
            case 0x12: // ADC Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    ctx.ADC(dst, src);
                    return true;
                });
                break;
            case 0x13: // ADC Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.ADC(dst, src);
                    return true;
                });
                break;
            case 0x14: // ADC AL, Ib
                ctx.ADC(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x15: // ADC AX, Iz
                ctx.ADC(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x18: // SBB Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    ctx.SBB(dst, src);
                    return true;
                });
                break;
            case 0x19: // SBB Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.SBB(dst, src);
                    return true;
                });
                break;
            case 0x1A: // SBB Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    ctx.SBB(dst, src);
                    return true;
                });
                break;
            case 0x1B: // SBB Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.SBB(dst, src);
                    return true;
                });
                break;
            case 0x1C: // SBB AL, Ib
                ctx.SBB(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x1D: // SBB AX, Iz
                ctx.SBB(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x20: // AND Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    ctx.AND(dst, src);
                    return true;
                });
                break;
            case 0x21: // AND Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.AND(dst, src);
                    return true;
                });
                break;
            case 0x22: // AND Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    ctx.AND(dst, src);
                    return true;
                });
                break;
            case 0x23: // AND Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.AND(dst, src);
                    return true;
                });
                break;
            case 0x24: // AND AL, Ib
                ctx.AND(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x25: // AND AX, Iz
                ctx.AND(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x26: case 0x2E: case 0x36: case 0x3E: // SEG:
                ctx.set_seg_override(opcode >> 3);
                goto next_byte;
            case 0x27: // DAA
                ctx.DAA();
                break;
            case 0x28: // SUB Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    ctx.SUB(dst, src);
                    return true;
                });
                break;
            case 0x29: // SUB Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.SUB(dst, src);
                    return true;
                });
                break;
            case 0x2A: // SUB Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    ctx.SUB(dst, src);
                    return true;
                });
                break;
            case 0x2B: // SUB Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.SUB(dst, src);
                    return true;
                });
                break;
            case 0x2C: // SUB AL, Ib
                ctx.SUB(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x2D: // SUB AX, Iz
                ctx.SUB(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x2F: // DAS
                ctx.DAS();
                break;
            case 0x30: // XOR Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    ctx.XOR(dst, src);
                    return true;
                });
                break;
            case 0x31: // XOR Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    ctx.XOR(dst, src);
                    return true;
                });
                break;
            case 0x32: // XOR Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    ctx.XOR(dst, src);
                    return true;
                });
                break;
            case 0x33: // XOR Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    ctx.XOR(dst, src);
                    return true;
                });
                break;
            case 0x34: // XOR AL, Ib
                ctx.XOR(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x35: // XOR AX, Iz
                ctx.XOR(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x37: // AAA
                ctx.AAA();
                break;
            case 0x38: // CMP Mb, Rb
                ctx.binopMR<true>(pc, [](auto dst, auto src) {
                    ctx.CMP(dst, src);
                    return false;
                });
                break;
            case 0x39: // CMP Mv, Rv
                ctx.binopMR(pc, [](auto dst, auto src) {
                    ctx.CMP(dst, src);
                    return false;
                });
                break;
            case 0x3A: // CMP Rb, Mb
                ctx.binopRM<true>(pc, [](auto dst, auto src) {
                    ctx.CMP(dst, src);
                    return false;
                });
                break;
            case 0x3B: // CMP Rv, Mv
                ctx.binopRM(pc, [](auto dst, auto src) {
                    ctx.CMP(dst, src);
                    return false;
                });
                break;
            case 0x3C: // CMP AL, Ib
                ctx.CMP(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0x3D: // CMP AX, Iz
                ctx.CMP(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0x3F: // AAS
                ctx.AAS();
                break;
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: // INC reg
                ctx.INC(ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: // DEC reg
                ctx.DEC(ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: // PUSH reg
                ctx.PUSH(ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: // POP reg
                ctx.index_reg<uint16_t>(opcode & 7) = ctx.POP();
                break;
            case 0x60: case 0x70: // JO Jb
            case 0x61: case 0x71: // JNO Jb
                ctx.ip = pc.offset + 1;
                if (ctx.overflow != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x62: case 0x72: // JC Jb
            case 0x63: case 0x73: // JNC Jb
                ctx.ip = pc.offset + 1;
                if (ctx.carry != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x64: case 0x74: // JZ Jb
            case 0x65: case 0x75: // JNZ Jb
                ctx.ip = pc.offset + 1;
                if (ctx.zero != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x66: case 0x76: // JBE Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_BE()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x67: case 0x77: // JA Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_A()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x68: case 0x78: // JS Jb
            case 0x69: case 0x79: // JNS Jb
                ctx.ip = pc.offset + 1;
                if (ctx.sign != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x6A: case 0x7A: // JP Jb
            case 0x6B: case 0x7B: // JNP Jb
                ctx.ip = pc.offset + 1;
                if (ctx.parity != (opcode & 1)) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x6C: case 0x7C: // JL Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_L()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x6D: case 0x7D: // JGE Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_GE()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x6E: case 0x7E: // JLE Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_LE()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x6F: case 0x7F: // JG Jb
                ctx.ip = pc.offset + 1;
                if (ctx.cond_G()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0x80: case 0x82: // GRP1 Mb, Ib
                ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) {
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
                });
                ++pc;
                break;
            case 0x81: // GRP1 Ev, Iz
                ctx.unopM(pc, [&](auto& dst, uint8_t r) {
                    uint16_t val = pc.read<uint16_t>();
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
                });
                pc += 2;
                break;
            case 0x83: // GRP1 Ev, Ib
                ctx.unopM(pc, [&](auto& dst, uint8_t r) {
                    uint16_t val = (int16_t)pc.read<int8_t>();
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
                });
                ++pc;
                break;
            case 0x84: // TEST Mb, Rb
                ctx.binopMR<true>(pc, [](auto dst, auto src) {
                    ctx.TEST(dst, src);
                    return false;
                });
                break;
            case 0x85: // TEST Mv, Rv
                ctx.binopMR(pc, [](auto dst, auto src) {
                    ctx.TEST(dst, src);
                    return false;
                });
                break;
            case 0x86: // XCHG Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto& src) {
                    ctx.XCHG(dst, src);
                    return true;
                });
                break;
            case 0x87: // XCHG Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto& src) {
                    ctx.XCHG(dst, src);
                    return true;
                });
                break;
            case 0x88: // MOV Mb, Rb
                ctx.binopMR<true>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x89: // MOV Mv, Rv
                ctx.binopMR(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8A: // MOV Rb, Mb
                ctx.binopRM<true>(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8B: // MOV Rv, Mv
                ctx.binopRM(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8C: // MOV M, seg
                ctx.binopMS(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8D: { // LEA
                ModRM modrm = pc.read_advance<ModRM>();
                if (modrm.is_mem()) {
                    z86Addr addr = modrm.parse_memM(pc);
                    ctx.index_reg<uint16_t>(modrm.R()) = addr.offset;
                }
                else {
                    // TODO: jank
                }
                break;
            }
            case 0x8E: // MOV seg, M
                ctx.binopSM(pc, [](auto& dst, auto src) {
                    dst = src;
                    return true;
                });
                break;
            case 0x8F: // GRP1A (Supposedly this does mystery jank if R != 0)
                ctx.unopM(pc, [](auto src, uint8_t r) {
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
                });
                break;
            case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // XCHG AX, reg
                ctx.XCHG(ctx.ax, ctx.index_reg<uint16_t>(opcode & 7));
                break;
            case 0x98: // CBW
                ctx.ax = (int16_t)(int8_t)ctx.al;
                break;
            case 0x99: // CWD
                ctx.dx = (int16_t)ctx.ax >> 15;
                break;
            case 0x9A: // CALL far abs
                ctx.PUSH(ctx.cs);
                ctx.PUSH(pc.offset + 4);
                ctx.ip = pc.read<uint16_t>();
                ctx.cs = pc.read<uint16_t>(2);
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
            case 0xA0: { // MOV AL, mem
                z86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                ctx.al = addr.read<uint8_t>();
                break;
            }
            case 0xA1: { // MOV AX, mem
                z86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                ctx.ax = addr.read<uint16_t>();
                break;
            }
            case 0xA2: { // MOV mem, AL
                z86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                addr.write(ctx.al);
                break;
            }
            case 0xA3: { // MOV mem, AX
                z86Addr addr = ctx.addr(DS, pc.read_advance<uint16_t>());
                addr.write(ctx.ax);
                break;
            }
            case 0xA4: // MOVSB
                ctx.MOVS<true>();
                break;
            case 0xA5: // MOVSW
                ctx.MOVS();
                break;
            case 0xA6: // CMPSB
                ctx.CMPS<true>();
                break;
            case 0xA7: // CMPSW
                ctx.CMPS();
                break;
            case 0xA8: // TEST AL, Ib
                ctx.TEST(ctx.al, pc.read_advance<uint8_t>());
                break;
            case 0xA9: // TEST AX, Iz
                ctx.TEST(ctx.ax, pc.read_advance<uint16_t>());
                break;
            case 0xAA: // STOSB
                ctx.STOS<true>();
                break;
            case 0xAB: // STOSW
                ctx.STOS();
                break;
            case 0xAC: // LODSB
                ctx.LODS<true>();
                break;
            case 0xAD: // LODSW
                ctx.LODS();
                break;
            case 0xAE: // SCASB
                ctx.SCAS<true>();
                break;
            case 0xAF: // SCASW
                ctx.SCAS();
                break;
            case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: // MOV reg8, Ib
                ctx.index_reg<uint8_t>(opcode & 7) = pc.read_advance<int8_t>();
                break;
            case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: // MOV reg, Iv
                ctx.index_reg<uint16_t>(opcode & 7) = pc.read_advance<uint16_t>();
                break;
            case 0xC0: case 0xC2: // RET imm
                ctx.ip = ctx.POP();
                ctx.sp += pc.read<int16_t>();
                goto next_instr;
            case 0xC1: case 0xC3: // RET
                ctx.ip = ctx.POP();
                goto next_instr;
            case 0xC4: // LES
                ctx.binopRM2(pc, [](auto& dst, uint32_t src) {
                    dst = src;
                    ctx.es = src >> 16;
                    return true;
                });
                break;
            case 0xC5: // LDS
                ctx.binopRM2(pc, [](auto& dst, uint32_t src) {
                    dst = src;
                    ctx.ds = src >> 16;
                    return true;
                });
                break;
            case 0xC6: // GRP11 Ib (Supposedly this just ignores R bits)
                ctx.unopM<true>(pc, [&](auto& dst, uint8_t r) {
                    dst = pc.read<int8_t>();
                    return true;
                });
                ++pc;
                break;
            case 0xC7: // GRP11 Iz (Supposedly this just ignores R bits)
                ctx.unopM(pc, [&](auto& dst, uint8_t r) {
                    dst = pc.read<uint16_t>();
                    return true;
                });
                pc += 2;
                break;
            case 0xC8: case 0xCA: // RETF imm
                ctx.ip = ctx.POP();
                ctx.cs = ctx.POP();
                ctx.sp += pc.read<int16_t>();
                goto next_instr;
            case 0xC9: case 0xCB: // RETF
                ctx.ip = ctx.POP();
                ctx.cs = ctx.POP();
                goto next_instr;
            case 0xCC: // INT3
                ctx.software_interrupt(IntBP);
                break;
            case 0xCD: // INT Ib
                ctx.software_interrupt(pc.read_advance<uint8_t>());
                break;
            case 0xCE: // INTO
                if (ctx.overflow) {
                    ctx.software_interrupt(IntOF);
                }
                break;
            case 0xCF: // IRET
                ctx.ip = ctx.POP();
                ctx.cs = ctx.POP();
                ctx.set_flags(ctx.POP());
                continue; // Using continues delays execution deliberately
            case 0xD0: // GRP2 Mb, 1
                ctx.unopM<true>(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.ROL<false>(dst, 1); return true;
                        case 1: ctx.ROR<false>(dst, 1); return true;
                        case 2: ctx.RCL<false>(dst, 1); return true;
                        case 3: ctx.RCR<false>(dst, 1); return true;
                        case 4: ctx.SHL<false>(dst, 1); return true;
                        case 5: ctx.SHR<false>(dst, 1); return true;
                        case 6: ctx.SETMO<false>(dst, 1); return true;
                        case 7: ctx.SAR<false>(dst, 1); return true;
                        default: unreachable;
                    }
                });
                break;
            case 0xD1: // GRP2 Mv, 1
                ctx.unopM(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.ROL<false>(dst, 1); return true;
                        case 1: ctx.ROR<false>(dst, 1); return true;
                        case 2: ctx.RCL<false>(dst, 1); return true;
                        case 3: ctx.RCR<false>(dst, 1); return true;
                        case 4: ctx.SHL<false>(dst, 1); return true;
                        case 5: ctx.SHR<false>(dst, 1); return true;
                        case 6: ctx.SETMO<false>(dst, 1); return true;
                        case 7: ctx.SAR<false>(dst, 1); return true;
                        default: unreachable;
                    }
                });
                break;
            case 0xD2: // GRP2 Mb, CL
                ctx.unopM<true>(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.ROL<false>(dst, ctx.cl); return true;
                        case 1: ctx.ROR<false>(dst, ctx.cl); return true;
                        case 2: ctx.RCL<false>(dst, ctx.cl); return true;
                        case 3: ctx.RCR<false>(dst, ctx.cl); return true;
                        case 4: ctx.SHL<false>(dst, ctx.cl); return true;
                        case 5: ctx.SHR<false>(dst, ctx.cl); return true;
                        case 6: ctx.SETMO<false>(dst, ctx.cl); return true;
                        case 7: ctx.SAR<false>(dst, ctx.cl); return true;
                        default: unreachable;
                    }
                });
                break;
            case 0xD3: // GRP2 Mv, CL
                ctx.unopM(pc, [](auto& dst, uint8_t r) {
                    switch (r) {
                        case 0: ctx.ROL<false>(dst, ctx.cl); return true;
                        case 1: ctx.ROR<false>(dst, ctx.cl); return true;
                        case 2: ctx.RCL<false>(dst, ctx.cl); return true;
                        case 3: ctx.RCR<false>(dst, ctx.cl); return true;
                        case 4: ctx.SHL<false>(dst, ctx.cl); return true;
                        case 5: ctx.SHR<false>(dst, ctx.cl); return true;
                        case 6: ctx.SETMO<false>(dst, ctx.cl); return true;
                        case 7: ctx.SAR<false>(dst, ctx.cl); return true;
                        default: unreachable;
                    }
                });
                break;
                break;
            case 0xD4: // AAM Ib
                ctx.AAM(pc.read_advance<uint8_t>());
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
                pc += 1 + pc.read<ModRM>().extra_length();
                break;
            }
            case 0xE0: // LOOPNZ Jb
                ctx.ip = pc.offset + 1;
                if (--ctx.cx || ctx.cond_NZ()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0xE1: // LOOPZ Jb
                ctx.ip = pc.offset + 1;
                if (--ctx.cx || ctx.cond_Z()) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0xE2: // LOOP Jb
                ctx.ip = pc.offset + 1;
                if (--ctx.cx) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0xE3: // JCXZ Jb
                ctx.ip = pc.offset + 1;
                if (!ctx.cx) {
                    ctx.ip += pc.read<int8_t>();
                }
                goto next_instr;
            case 0xE4: { // IN AL, Ib
                uint16_t temp;
                port_in(temp, pc.read_advance<uint8_t>());
                ctx.al = temp;
                break;
            }
            case 0xE5: // IN AX, Ib
                port_in(ctx.ax, pc.read_advance<uint8_t>());
                break;
            case 0xE6: // OUT Ib, AL
                port_out(pc.read_advance<uint8_t>(), ctx.al);
                break;
            case 0xE7: // OUT Ib, AX
                port_out(pc.read_advance<uint8_t>(), ctx.ax);
                break;
                break;
            case 0xE8: // CALL Jz
                ctx.PUSH(pc.offset + 2);
                ctx.ip = pc.offset + 2 + pc.read<int16_t>();
                goto next_instr;
            case 0xE9: // JMP Jz
                ctx.ip = pc.offset + 2 + pc.read<int16_t>();
                goto next_instr;
            case 0xEA: // JMP far abs
                ctx.ip = pc.read<uint16_t>();
                ctx.cs = pc.read<uint16_t>(2);
                goto next_instr;
            case 0xEB: // JMP Jb
                ctx.ip = pc.offset + 1 + pc.read<int8_t>();
                goto next_instr;
            case 0xEC: { // IN AL, DX
                uint16_t temp;
                port_in(temp, ctx.dx);
                ctx.al = temp;
                break;
            }
            case 0xED: // IN AX, DX
                port_in(ctx.ax, ctx.dx);
                break;
            case 0xEE: // OUT DX, AL
                port_out(ctx.dx, ctx.al);
                break;
            case 0xEF: // OUT DX, AX
                port_out(ctx.dx, ctx.ax);
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
                ctx.unopM<true>(pc, [&](auto& val, uint8_t r) {
                    switch (r) {
                        case 0: case 1: // TEST Mb, Ib
                            ctx.TEST(val, pc.read_advance<uint8_t>());
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
                            ctx.DIV(val);
                            return false;
                        case 7: // IDIV Mb
                            ctx.IDIV(val);
                            return false;
                        default:
                            unreachable;
                    }
                });
                break;
            case 0xF7: // GRP3 Mv
                ctx.unopM(pc, [&](auto& val, uint8_t r) {
                    switch (r) {
                        case 0: case 1: // TEST Mv, Iz
                            ctx.TEST(val, pc.read_advance<uint16_t>());
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
                            ctx.DIV(val);
                            return false;
                        case 7: // IDIV Mv
                            ctx.IDIV(val);
                            return false;
                        default:
                            unreachable;
                    }
                });
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
        }
        ctx.ip = pc.offset;
    next_instr:
        ctx.execute_pending_interrupts();
    }
}