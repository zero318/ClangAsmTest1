#pragma once

#ifndef Z86_CORE_INTERNAL_PRE_H
#define Z86_CORE_INTERNAL_PRE_H 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <limits>
#include <algorithm>
#include <utility>
#include <type_traits>

#include "../zero/util.h"

static inline constexpr unsigned long long operator ""_KB(unsigned long long value) {
    return value * 1024ull;
}

static inline constexpr unsigned long long operator ""_KB(long double value) {
    return value * 1024.0L;
}

static inline constexpr unsigned long long operator ""_MB(unsigned long long value) {
    return value * 1048576ull;
}

static inline constexpr unsigned long long operator ""_MB(long double value) {
    return value * 1048576.0L;
}

static inline constexpr unsigned long long operator ""_GB(unsigned long long value) {
    return value * 1073741824ull;
}

static inline constexpr unsigned long long operator ""_GB(long double value) {
    return value * 1073741824.0L;
}

// Code shared between x86 cores

template <size_t bytes>
struct z86Memory {
    unsigned char raw[bytes];

    template <typename T = uint8_t>
    inline T* ptr(size_t offset) {
        return (T*)&this->raw[offset];
    }

    template <typename T = uint8_t>
    inline const T* ptr(size_t offset) const {
        return (const T*)&this->raw[offset];
    }

    template <typename T = uint8_t>
    inline T& ref(size_t offset) {
        return *this->ptr<T>(offset);
    }

    template <typename T = uint8_t>
    inline const T& ref(size_t offset) const {
        return *this->ptr<T>(offset);
    }

    template <typename T = uint8_t>
    inline T read(size_t offset) const {
        return this->ref<T>(offset);
    }

    template <typename T = uint8_t>
    inline void write(size_t offset, const T& value) {
        if constexpr (!std::is_array_v<std::remove_reference_t<T>>) {
            this->ref<T>(offset) = value;
        } else {
            memcpy(this->ptr(offset), &value, sizeof(T));
        }
    }

    inline uint8_t& operator[](size_t offset) {
        return this->read(offset);
    }

    inline const uint8_t& operator[](size_t offset) const {
        return this->read(offset);
    }

    inline size_t read(void* dst, size_t src, size_t length) const {
        if (src < bytes) {
            length = (std::min)(bytes - src, length);
            memcpy(dst, &this->raw[src], length);
            return length;
        }
        return 0;
    }

    inline size_t write(size_t dst, const void* src, size_t length) {
        if (dst < bytes) {
            length = (std::min)(bytes - dst, length);
            memcpy(&this->raw[dst], src, length);
            return length;
        }
        return 0;
    }
};

template <size_t bits>
struct GPR;

template <>
struct GPR<16> {
    union {
        uint16_t qword;
        uint16_t dword;
        uint16_t word;
        struct {
            uint8_t byte;
            uint8_t hbyte;
        };
    };
};

template <>
struct GPR<32> {
    union {
        uint32_t qword;
        uint32_t dword;
        uint16_t word;
        struct {
            uint8_t byte;
            uint8_t hbyte;
        };
    };
};

template <>
struct GPR<64> {
    union {
        uint64_t qword;
        uint32_t dword;
        uint16_t word;
        struct {
            uint8_t byte;
            uint8_t hbyte;
        };
    };
};

#define DS DS_SEG

enum REG_INDEX : uint8_t {
    RAX  =  0, EAX  =  0, AX   =  0, AL   =  0, XMM0  =  0, CR0 = 0, ST0 = 0, MM0 = 0, DR0 = 0, ES = 0,
    RCX  =  1, ECX  =  1, CX   =  1, CL   =  1, XMM1  =  1, CR1 = 1, ST1 = 1, MM1 = 1, DR1 = 1, CS = 1,
    RDX  =  2, EDX  =  2, DX   =  2, DL   =  2, XMM2  =  2, CR2 = 2, ST2 = 2, MM2 = 2, DR2 = 2, SS = 2,
    RBX  =  3, EBX  =  3, BX   =  3, BL   =  3, XMM3  =  3, CR3 = 3, ST3 = 3, MM3 = 3, DR3 = 3, DS = 3,
    RSP  =  4, ESP  =  4, SP   =  4, AH   =  4, XMM4  =  4, CR4 = 4, ST4 = 4, MM4 = 4, DR4 = 4, FS = 4,
    RBP  =  5, EBP  =  5, BP   =  5, CH   =  5, XMM5  =  5, CR5 = 5, ST5 = 5, MM5 = 5, DR5 = 5, GS = 5,
    RSI  =  6, ESI  =  6, SI   =  6, DH   =  6, XMM6  =  6, CR6 = 6, ST6 = 6, MM6 = 6, DR6 = 6,
    RDI  =  7, EDI  =  7, DI   =  7, BH   =  7, XMM7  =  7, CR7 = 7, ST7 = 7, MM7 = 7, DR7 = 7,
    R8   =  8, R8D  =  8, R8W  =  8, R8B  =  8, XMM8  =  8, CR8 = 8,
    R9   =  9, R9D  =  9, R9W  =  9, R9B  =  9, XMM9  =  9,
    R10  = 10, R10D = 10, R10W = 10, R10B = 10, XMM10 = 10,
    R11  = 11, R11D = 11, R11W = 11, R11B = 11, XMM11 = 11,
    R12  = 12, R12D = 12, R12W = 12, R12B = 12, XMM12 = 12,
    R13  = 13, R13D = 13, R13W = 13, R13B = 13, XMM13 = 13,
    R14  = 14, R14D = 14, R14W = 14, R14B = 14, XMM14 = 14,
    R15  = 15, R15D = 15, R15W = 15, R15B = 15, XMM15 = 15
};

enum REP_STATE : int8_t {
    NO_REP = -1,
    REP_NE = 0,
    REP_E = 1
};

template<size_t max_bits>
struct z86BaseGPRs;

// size: 0x12
template<>
struct z86BaseGPRs<16> {

    using HT = uint8_t; // Half Reg Type
    using RT = uint16_t; // Reg Type
    using DT = uint32_t; // Double Reg Type

    union {
        GPR<16> gpr[8];
        struct {
            union {
                uint16_t rax;
                uint16_t eax;
                uint16_t ax;
                struct {
                    uint8_t al;
                    uint8_t ah;
                };
            };
            union {
                uint16_t rcx;
                uint16_t ecx;
                uint16_t cx;
                struct {
                    uint8_t cl;
                    uint8_t ch;
                };
            };
            union {
                uint16_t rdx;
                uint16_t edx;
                uint16_t dx;
                struct {
                    uint8_t dl;
                    uint8_t dh;
                };
            };
            union {
                uint16_t rbx;
                uint16_t ebx;
                uint16_t bx;
                struct {
                    uint8_t bl;
                    uint8_t bh;
                };
            };
            union {
                uint16_t rsp;
                uint16_t esp;
                uint16_t sp;
                uint8_t spl;
            };
            union {
                uint16_t rbp;
                uint16_t ebp;
                uint16_t bp;
                uint8_t bpl;
            };
            union {
                uint16_t rsi;
                uint16_t esi;
                uint16_t si;
                uint8_t sil;
            };
            union {
                uint16_t rdi;
                uint16_t edi;
                uint16_t di;
                uint8_t dil;
            };
        };
    };
    union {
        uint16_t rip;
        uint16_t eip;
        uint16_t ip;
    };

    static constexpr inline int8_t data_size = -1;
    static constexpr inline int8_t addr_size = -1;
    static constexpr inline int8_t stack_size = -1;

    inline constexpr uint8_t& index_byte_reg(uint8_t index) {
        return *(&this->gpr[index & 3].byte + (index > 3));
    }
};

// size: 0x24
template<>
struct z86BaseGPRs<32> {

    using HT = uint16_t; // Half Reg Type
    using RT = uint32_t; // Reg Type
    using DT = uint64_t; // Double Reg Type

    union {
        GPR<32> gpr[8];
        struct {
            union {
                uint32_t rax;
                uint32_t eax;
                uint16_t ax;
                struct {
                    uint8_t al;
                    uint8_t ah;
                };
            };
            union {
                uint32_t rcx;
                uint32_t ecx;
                uint16_t cx;
                struct {
                    uint8_t cl;
                    uint8_t ch;
                };
            };
            union {
                uint32_t rdx;
                uint32_t edx;
                uint16_t dx;
                struct {
                    uint8_t dl;
                    uint8_t dh;
                };
            };
            union {
                uint32_t rbx;
                uint32_t ebx;
                uint16_t bx;
                struct {
                    uint8_t bl;
                    uint8_t bh;
                };
            };
            union {
                uint32_t rsp;
                uint32_t esp;
                uint16_t sp;
                uint8_t spl;
            };
            union {
                uint32_t rbp;
                uint32_t ebp;
                uint16_t bp;
                uint8_t bpl;
            };
            union {
                uint32_t rsi;
                uint32_t esi;
                uint16_t si;
                uint8_t sil;
            };
            union {
                uint32_t rdi;
                uint32_t edi;
                uint16_t di;
                uint8_t dil;
            };
        };
    };
    union {
        uint32_t rip;
        uint32_t eip;
        uint16_t ip;
    };

    inline constexpr uint8_t& index_byte_reg(uint8_t index) {
        return *(&this->gpr[index & 3].byte + (index > 3));
    }
};

// size: 0x48 + 1
template <>
struct z86BaseGPRs<64> {

    using HT = uint32_t; // Half Reg Type
    using RT = uint64_t; // Reg Type
    using DT = uint128_t; // Double Reg Type

    union {
        GPR<64> gpr[16];
        struct {
            union {
                uint64_t rax;
                uint32_t eax;
                uint16_t ax;
                struct {
                    uint8_t al;
                    uint8_t ah;
                };
            };
            union {
                uint64_t rcx;
                uint32_t ecx;
                uint16_t cx;
                struct {
                    uint8_t cl;
                    uint8_t ch;
                };
            };
            union {
                uint64_t rdx;
                uint32_t edx;
                uint16_t dx;
                struct {
                    uint8_t dl;
                    uint8_t dh;
                };
            };
            union {
                uint64_t rbx;
                uint32_t ebx;
                uint16_t bx;
                struct {
                    uint8_t bl;
                    uint8_t bh;
                };
            };
            union {
                uint64_t rsp;
                uint32_t esp;
                uint16_t sp;
                uint8_t spl;
            };
            union {
                uint64_t rbp;
                uint32_t ebp;
                uint16_t bp;
                uint8_t bpl;
            };
            union {
                uint64_t rsi;
                uint32_t esi;
                uint16_t si;
                uint8_t sil;
            };
            union {
                uint64_t rdi;
                uint32_t edi;
                uint16_t di;
                uint8_t dil;
            };
            union {
                uint64_t r8;
                uint32_t r8d;
                uint16_t r8w;
                uint8_t r8b;
            };
            union {
                uint64_t r9;
                uint32_t r9d;
                uint16_t r9w;
                uint8_t r9b;
            };
            union {
                uint64_t r10;
                uint32_t r10d;
                uint16_t r10w;
                uint8_t r10b;
            };
            union {
                uint64_t r11;
                uint32_t r11d;
                uint16_t r11w;
                uint8_t r11b;
            };
            union {
                uint64_t r12;
                uint32_t r12d;
                uint16_t r12w;
                uint8_t r12b;
            };
            union {
                uint64_t r13;
                uint32_t r13d;
                uint16_t r13w;
                uint8_t r13b;
            };
            union {
                uint64_t r14;
                uint32_t r14d;
                uint16_t r14w;
                uint8_t r14b;
            };
            union {
                uint64_t r15;
                uint32_t r15d;
                uint16_t r15w;
                uint8_t r15b;
            };
        };
    };
    union {
        uint64_t rip;
        uint32_t eip;
        uint16_t ip;
    };

    int8_t rex_bits;

    inline constexpr uint8_t& index_byte_reg(uint8_t index) {
        if (!rex_bits) {
            return *(&this->gpr[index & 3].byte + (index > 3));
        } else {
            return this->gpr[index].byte;
        }
    }
};

template <size_t bits>
struct z86AddrBase;

template <>
struct z86AddrBase<16> {
    using OT = uint16_t; // Offset Type
    using FT = uint32_t; // Far Type
    using MT = uint32_t; // Memory Addr Type

    union {
        uint32_t raw;
        struct {
            uint16_t offset;
            uint16_t segment;
        };
    };

    inline constexpr z86AddrBase() : raw(0) {}
    inline constexpr z86AddrBase(FT raw) : raw(raw) {}
    inline constexpr z86AddrBase(uint16_t segment, OT offset) : offset(offset), segment(segment) {}
    inline constexpr z86AddrBase(const z86AddrBase&) = default;
};

template <>
struct z86AddrBase<32> {
    using OT = uint32_t; // Offset Type
    using FT = uint64_t; // Far Type
    using MT = uint32_t; // Memory Addr Type

    union {
        uint64_t raw;
        struct {
            uint32_t offset;
            uint16_t segment;
        };
    };

    inline constexpr z86AddrBase() : raw(0) {}
    inline constexpr z86AddrBase(FT raw) : raw(raw) {}
    inline constexpr z86AddrBase(uint16_t segment, OT offset) : offset(offset), segment(segment) {}
    inline constexpr z86AddrBase(const z86AddrBase&) = default;
};

template <>
struct z86AddrBase<64> {
    using OT = uint64_t; // Offset Type
    using FT = uint128_t; // Far Type
    using MT = uint64_t; // Memory Addr Type

    union {
        uint128_t raw;
        struct {
            uint64_t offset;
            uint16_t segment;
        };
    };

    inline constexpr z86AddrBase() : raw(0) {}
    inline constexpr z86AddrBase(FT raw) : raw(raw) {}
    inline constexpr z86AddrBase(uint16_t segment, OT offset) : offset(offset), segment(segment) {}
    inline constexpr z86AddrBase(const z86AddrBase&) = default;
};

template <size_t bits>
struct z86AddrImpl : z86AddrBase<bits> {

    using OT = z86AddrBase<bits>::OT;
    using FT = z86AddrBase<bits>::FT;
    using MT = z86AddrBase<bits>::MT;

    inline constexpr z86AddrImpl() : z86AddrBase<bits>::z86AddrBase() {}
    inline constexpr z86AddrImpl(FT raw) : z86AddrBase<bits>::z86AddrBase(raw) {}
    inline constexpr z86AddrImpl(uint16_t segment, OT offset) : z86AddrBase<bits>::z86AddrBase(segment, offset) {}
    inline constexpr z86AddrImpl(const z86AddrImpl&) = default;

    inline constexpr size_t real_addr(size_t offset = 0) const {
        return ((size_t)this->segment << 4) + (uint16_t)(this->offset + offset);
    }

    inline constexpr z86AddrImpl& operator+=(ssize_t offset) {
        this->offset += offset;
        return *this;
    }

    inline constexpr z86AddrImpl& operator-=(ssize_t offset) {
        this->offset -= offset;
        return *this;
    }

    inline constexpr z86AddrImpl& operator++() {
        ++this->offset;
        return *this;
    }

    inline constexpr z86AddrImpl operator++(int) {
        return z86Addr(this->segment, this->offset++);
    }

    inline constexpr z86AddrImpl& operator--() {
        --this->offset;
        return *this;
    }

    inline constexpr z86AddrImpl operator--(int) {
        return z86AddrImpl(this->segment, this->offset--);
    }

    inline constexpr FT operator+(ssize_t offset) const {
        z86AddrImpl temp = this->raw;
        temp.offset += offset;
        return temp.raw;
    }

    inline constexpr FT operator-(ssize_t offset) const {
        z86AddrImpl temp = this->raw;
        temp.offset -= offset;
        return temp.raw;
    }

    template <typename T = uint8_t>
    inline void write(const T& value, intptr_t offset = 0);

    template <typename T = uint8_t>
    inline T read(intptr_t offset = 0) const;

    template <typename T = uint8_t>
    inline void write_advance(const T& value, intptr_t index = sizeof(T)) {
        this->write(value);
        this->offset += index;
    }

    template <typename T = uint8_t>
    inline T read_advance(intptr_t index = sizeof(T)) {
        T ret = this->read<T>();
        this->offset += index;
        return ret;
    }
};

struct DataSize {};
struct AddrSize {};

template <auto& ctx>
struct ModRMBase {
#if USE_BITFIELDS
    union {
        uint8_t raw;
        struct {
            uint8_t m : 3;
            uint8_t r : 3;
            uint8_t mod : 2;
        };
    };

    inline constexpr uint8_t Mod() const {
        return this->mod;
    }

    inline constexpr uint8_t R() const {
        return this->r;
    }

    inline constexpr uint8_t M() const {
        return this->m;
    }
#else
    uint8_t raw;

    inline constexpr uint8_t Mod() const {
        return this->raw >> 6;
    }

    inline constexpr uint8_t R() const {
        return (this->raw & 0070) >> 3;
    }

    inline constexpr uint8_t M() const {
        return this->raw & 0007;
    }
#endif

    inline constexpr bool is_mem() const {
        return this->Mod() != 3;
    }

    uint32_t extra_length() const {
        switch (this->Mod()) {
            case 0:
                return (this->M() == 6) * 2;
            case 1:
                return 1;
            case 2:
                return 2;
            case 3:
                return 0;
            default:
                unreachable;
        }
    }

    template <typename T>
    uint32_t parse_memM(T& pc) const {
        uint8_t default_segment = DS;
        size_t offset;
        uint8_t m = this->M();
        switch (m) {
            case 0:
                offset = ctx.bx + ctx.si;
                break;
            case 1:
                offset = ctx.bx + ctx.di;
                break;
            case 2:
                offset = ctx.bp + ctx.si;
                default_segment = SS;
                break;
            case 3:
                offset = ctx.bp + ctx.di;
                default_segment = SS;
                break;
            case 4:
                offset = ctx.si;
                break;
            case 5:
                offset = ctx.di;
                break;
            case 6:
                offset = ctx.bp;
                default_segment = SS;
                break;
            case 7:
                offset = ctx.bx;
                break;
            default:
                unreachable;
        }
        switch (this->Mod()) {
            case 0:
                if (m == 6) {
                    offset = pc.read_advance<int16_t>();
                    default_segment = DS;
                }
                break;
            case 1:
                offset += pc.read_advance<int8_t>();
                break;
            case 2:
                offset += pc.read_advance<int16_t>();
                break;
            default:
                unreachable;
        }
        return ctx.addr(default_segment, offset);
    }
};

template <size_t bits>
struct z86Base : z86BaseGPRs<bits> {

    using HT = z86BaseGPRs<bits>::HT;
    using RT = z86BaseGPRs<bits>::RT;
    using DT = z86BaseGPRs<bits>::DT;

    using SRT = std::make_signed_t<RT>;

    inline constexpr auto& index_word_reg(uint8_t index) {
        return this->gpr[index].word;
    }

    inline constexpr auto& index_dword_reg(uint8_t index) {
        return this->gpr[index].dword;
    }

    inline constexpr auto& index_qword_reg(uint8_t index) {
        return this->gpr[index].qword;
    }

    template <typename T>
    inline constexpr auto& index_reg(uint8_t index) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->index_byte_reg(index);
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->index_word_reg(index);
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->index_dword_reg(index);
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->index_qword_reg(index);
        }
    }

    template <typename T>
    inline constexpr auto& A() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->al;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->ax;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->eax;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rax;
        }
    }

    template <typename T>
    inline constexpr const auto& A() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->al;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->ax;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->eax;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rax;
        }
    }

    template <typename T>
    inline constexpr auto& C() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->cl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->cx;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->ecx;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rcx;
        }
    }

    template <typename T>
    inline constexpr const auto& C() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->cl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->cx;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->ecx;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rcx;
        }
    }

    template <typename T>
    inline constexpr auto& D() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->dl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->dx;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->edx;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rdx;
        }
    }

    template <typename T>
    inline constexpr const auto& D() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->dl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->dx;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->edx;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rdx;
        }
    }

    template <typename T>
    inline constexpr auto& B() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->bl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->bx;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->ebx;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rbx;
        }
    }

    template <typename T>
    inline constexpr const auto& B() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->bl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->bx;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->ebx;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rbx;
        }
    }

    template <typename T>
    inline constexpr auto& SP() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->spl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->sp;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->esp;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rsp;
        }
    }

    template <typename T>
    inline constexpr const auto& SP() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->spl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->sp;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->esp;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rsp;
        }
    }

    template <typename T>
    inline constexpr auto& BP() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->bpl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->bp;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->ebp;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rbp;
        }
    }

    template <typename T>
    inline constexpr const auto& BP() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->bpl;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->bp;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->ebp;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rbp;
        }
    }

    template <typename T>
    inline constexpr auto& SI() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->sil;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->si;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->esi;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rsi;
        }
    }

    template <typename T>
    inline constexpr const auto& SI() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->sil;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->si;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->esi;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rsi;
        }
    }

    template <typename T>
    inline constexpr auto& DI() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->dil;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->di;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->edi;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rdi;
        }
    }

    template <typename T>
    inline constexpr const auto& DI() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->dil;
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->di;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->edi;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rdi;
        }
    }

    template <typename T>
    inline constexpr auto& IP() {
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->ip;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->eip;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rip;
        }
    }

    template <typename T>
    inline constexpr const auto& IP() const {
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->ip;
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->eip;
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->rip;
        }
    }

    union {
        uint16_t seg[8];
        struct {
            uint16_t es;
            uint16_t cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
        };
    };

    inline constexpr uint16_t& index_seg(uint8_t index) {
        return this->seg[index];
    }

    // Flags
    bool carry;
    bool parity;
    bool auxiliary;
    bool zero;
    bool sign;
    bool trap;
    bool interrupt;
    bool direction;
    bool overflow;

    inline constexpr bool cond_O() const { return this->overflow; }
    inline constexpr bool cond_NO() const { return !this->overflow; }
    inline constexpr bool cond_C() const { return this->carry; }
    inline constexpr bool cond_B() const { return this->cond_C(); }
    inline constexpr bool cond_NAE() const { return this->cond_C(); }
    inline constexpr bool cond_NC() const { return !this->carry; }
    inline constexpr bool cond_NB() const { return this->cond_NC(); }
    inline constexpr bool cond_AE() const { return this->cond_NC(); }
    inline constexpr bool cond_Z() const { return this->zero; }
    inline constexpr bool cond_E() const { return this->cond_Z(); }
    inline constexpr bool cond_NZ() const { return !this->zero; }
    inline constexpr bool cond_NE() const { return this->cond_NZ(); }
    inline constexpr bool cond_BE() const { return this->carry || this->zero; }
    inline constexpr bool cond_NA() const { return this->cond_BE(); }
    inline constexpr bool cond_A() const { return !this->carry && !this->zero; }
    inline constexpr bool cond_NBE() const { return this->cond_A(); }
    inline constexpr bool cond_S() const { return this->sign; }
    inline constexpr bool cond_NS() const { return !this->sign; }
    inline constexpr bool cond_P() const { return this->parity; }
    inline constexpr bool cond_PE() const { return this->cond_P(); }
    inline constexpr bool cond_NP() const { return !this->parity; }
    inline constexpr bool cond_PO() const { return this->cond_NP(); }
    inline constexpr bool cond_L() const { return this->sign != this->overflow; }
    inline constexpr bool cond_NGE() const { return this->cond_L(); }
    inline constexpr bool cond_GE() const { return this->sign == this->overflow; }
    inline constexpr bool cond_NL() const { return this->cond_GE(); }
    inline constexpr bool cond_LE() const { return this->zero || this->sign != this->overflow; }
    inline constexpr bool cond_NG() const { return this->cond_LE(); }
    inline constexpr bool cond_G() const { return !this->zero && this->sign == this->overflow; }
    inline constexpr bool cond_NLE() const { return this->cond_G(); }


    bool lock;

    int8_t seg_override;
    int8_t rep_type;
    int16_t pending_sinterrupt;

    inline constexpr void set_seg_override(uint8_t seg) {
        this->seg_override = seg;
    }

    inline constexpr uint16_t segment(uint8_t default_seg) const {
        return this->seg[this->seg_override < 0 ? default_seg : this->seg_override];
    }

    inline constexpr DT addr(uint8_t default_seg, RT offset) const {
        return (DT)this->segment(default_seg) << bitsof(RT) | offset;
    }

    inline constexpr DT addr_force(uint8_t seg, RT offset) const {
        return (DT)this->seg[seg] << bitsof(RT) | offset;
    }

    template <typename P = RT>
    inline constexpr DT pc() const {
        return this->addr_force(CS, this->IP<P>());
    }

    template <typename P = RT>
    inline constexpr DT stack() const {
        return this->addr_force(SS, this->SP<P>());
    }

    template <typename P = RT>
    inline constexpr DT str_src() const {
        return this->addr(DS, this->SI<P>());
    }

    template <typename P = RT>
    inline constexpr DT str_dst() const {
        return this->addr_force(ES, this->DI<P>());
    }

    // Read A double
    template <typename T>
    inline constexpr auto read_AD() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->ax;
        }
        else {
            using U = std::make_unsigned_t<dbl_int_t<T>>;
            return (U)this->D<T>() << bitsof(T) | this->A<T>();
        }
    }

    // Write A double
    template <typename T>
    inline constexpr void write_AD(T value) {
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            this->ax = value;
        }
        else {
            using U = std::make_unsigned_t<T>;
            this->A<T>() = value;
            this->D<T>() = (U)value >> bitsof(hlf_int_t<T>);
        }
    }

    // A double high
    template <typename T>
    inline constexpr auto& ADH() {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->ah;
        }
        else {
            return this->D<T>();
        }
    }

    // A double high
    template <typename T>
    inline constexpr const auto& ADH() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->ah;
        } else {
            return this->D<T>();
        }
    }

    inline void update_parity(uint8_t val) {
        this->parity = !__builtin_parity(val);
    }

    template <typename T>
    inline void update_pzs(T val) {
        using S = std::make_signed_t<T>;
        this->update_parity(val);
        this->zero = !val;
        this->sign = (S)val > 0;
    }

    template <typename T = RT>
    inline void JMPABS(T new_ip) {
        this->rip = new_ip;
    }

    template <typename T = RT>
    inline void JMPFABS(T new_ip, uint16_t new_cs) {
        this->JMPABS(new_ip);
        this->cs = new_cs;
    }

    template <typename T = SRT>
    inline void PUSH(T src);

    template <typename T = RT>
    inline T POP();

    template <typename T = RT>
    inline void CALLABS(T next_ip, T new_ip) {
        this->PUSH(next_ip);
        this->JMPABS(new_ip);
    }

    template <typename T = RT>
    inline void CALLFABS(T next_ip, T new_ip, uint16_t new_cs) {
        this->PUSH<T>(this->cs);
        this->PUSH(next_ip);
        this->JMPFABS(new_ip, new_cs);
    }

    template <typename T>
    inline void ADD(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        this->carry = add_would_overflow<U>(dst, src);
        this->overflow = add_would_overflow<S>(dst, src);
        T res = dst + src;
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        dst = res;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void OR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst |= src;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void ADC(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = carry_add((U)dst, (U)src, this->carry);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->overflow = (S)(~(dst ^ src) & (dst ^ res)) < 0;
        dst = res;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void SBB(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = carry_sub<U>(dst, src, this->carry);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->overflow = (S)(~(dst ^ src) & (dst ^ res)) < 0;
        dst = res;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void AND(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst &= src;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void SUB(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        this->carry = sub_would_overflow<U>(dst, src);
        this->overflow = sub_would_overflow<S>(dst, src);
        T res = dst - src;
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        dst = res;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void XOR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst ^= src;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void CMP(T dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        this->carry = sub_would_overflow<U>(dst, src);
        this->overflow = sub_would_overflow<S>(dst, src);
        T res = dst - src;
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->update_pzs(res);
    }

    template <typename T>
    inline void TEST(T dst, T src) {
        this->carry = false;
        this->overflow = false;
        this->update_pzs((T)(dst & src));
    }

    template <typename T>
    inline void INC(T& dst) {
        using S = std::make_signed_t<T>;
        this->overflow = dst == (std::numeric_limits<S>::max)();
        this->auxiliary = (dst ^ 1 ^ dst + 1) & 0x10; // BLCMSK
        this->update_pzs(++dst);
    }

    template <typename T>
    inline void DEC(T& dst) {
        using S = std::make_signed_t<T>;
        this->overflow = dst == (std::numeric_limits<S>::max)();
        this->auxiliary = (dst ^ 1 ^ dst - 1) & 0x10; // BLSMSK
        this->update_pzs(--dst);
    }

    template <typename T>
    inline void NOT(T& dst) {
        dst = ~dst;
    }

    template <typename T>
    inline void NEG(T& dst) {
        using S = std::make_signed_t<T>;
        this->carry = dst;
        this->overflow = (S)dst == (std::numeric_limits<S>::min)();
        this->auxiliary = dst & 0xF;
        dst = -dst;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void XCHG(T& dst, T& src) {
        std::swap(dst, src);
    }

    template <typename T>
    inline void MUL(T src) {
        using UD = std::make_unsigned_t<dbl_int_t<T>>;

        UD temp = this->A<T>();
        temp *= src;
        this->write_AD(temp);
        this->carry = this->overflow = temp >> bitsof(T);
    }

    template <typename T>
    inline void IMUL(T src) {
        using SD = std::make_signed_t<dbl_int_t<T>>;
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;

        SD temp = this->A<T>();
        temp *= src;
        this->write_AD(temp);
        this->carry = this->overflow = (U)(temp >> bitsof(T)) != (S)temp >> bitsof(T) - 1;
    }

    template <typename T>
    inline void DIV(T src) {
        if (src) {
            using UD = std::make_unsigned_t<dbl_int_t<T>>;
            using U = std::make_unsigned_t<T>;
            UD temp = this->read_AD<T>();
            UD quot = temp / src;

            this->A<T>() = quot;
            this->ADH<T>() = temp % src;
            if (quot > (std::numeric_limits<U>::max)()) {
                this->software_interrupt(IntDE);
            }
        } else {
            this->software_interrupt(IntDE);
        }
    }

    template <typename T>
    inline void IDIV(T src) {
        if (src) {
            using SD = std::make_signed_t<dbl_int_t<T>>;
            using S = std::make_signed_t<T>;
            using U = std::make_unsigned_t<T>;

            SD temp = this->read_AD<T>();
            SD quot = temp / src;

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

    template <bool mask_count = true, typename T>
    inline void ROL(T& dst, uint8_t count) {

        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;
            
            T res = std::rotl<U>(dst, count);
            this->carry = res & 1;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
        }
    }

    template <bool mask_count = true, typename T>
    inline void ROR(T& dst, uint8_t count) {

        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            T res = std::rotr<U>(dst, count);
            this->carry = res & 1;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
        }
    }

    template <bool mask_count = true, typename T>
    inline void RCL(T& dst, uint8_t count) {

        constexpr size_t bits = bitsof(T) + 1;
        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint32_t)) {
                count = (count & 0x1F) % bits;
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                count &= 0x1F;
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            constexpr size_t bits = bitsof(T) + 1;

            UBitInt(bits) temp = dst;
            if (this->carry) {
                temp += (U)(std::numeric_limits<S>::min)();
                temp += (U)(std::numeric_limits<S>::min)();
            }

            if constexpr (!mask_count) {
                count %= bits;
            }

            temp = temp << count | temp >> bits - count;
            this->carry = temp > (std::numeric_limits<U>::max)();
            dst = (U)temp;
            this->overflow = this->carry == (S)dst < 0;
        }
    }

    template <bool mask_count = true, typename T>
    inline void RCR(T& dst, uint8_t count) {

        constexpr size_t bits = bitsof(T) + 1;
        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint32_t)) {
                count = (count & 0x1F) % bits;
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                count &= 0x1F;
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            UBitInt(bits) temp = dst;
            if (this->carry) {
                temp += (U)(std::numeric_limits<S>::min)();
                temp += (U)(std::numeric_limits<S>::min)();
            }

            if constexpr (!mask_count) {
                count %= bits;
            }

            temp = temp >> count | temp << bits - count;
            this->carry = temp > (std::numeric_limits<U>::max)();
            dst = (U)temp;
            this->overflow = __builtin_parity(dst & 3u << bitsof(T) - 2);
        }
    }

    template <bool mask_count = true, typename T>
    inline void SHL(T& dst, uint8_t count) {
        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }
        if (count) {
            using S = std::make_signed_t<T>;

            this->carry = ((size_t)dst >> bitsof(T) - count) & 1;
            T res = (size_t)dst << count;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
            this->update_pzs(dst);
        }
    }

    template <bool mask_count = true, typename T>
    inline void SHR(T& dst, uint8_t count) {
        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using S = std::make_signed_t<T>;

            this->carry = ((size_t)dst >> 1 - count) & 1;
            this->overflow = (S)dst < 0;
            dst = (size_t)dst >> count;
            this->update_pzs(dst);
        }
    }

    // Yay, jank
    template <bool mask_count = true, typename T>
    inline void SETMO(T& dst, uint8_t count) {
        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            this->OR<T>(dst, (T)-1);
        }
    }

    template <bool mask_count = true, typename T>
    inline void SAR(T& dst, uint8_t count) {
        if constexpr (mask_count) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            this->carry = ((size_t)dst >> 1 - count) & 1;
            this->overflow = false;
            dst = (ssize_t)dst >> count;
            this->update_pzs(dst);
        }
    }

    template <typename T, typename P>
    inline void LODS_impl();

    template <bool is_byte = false>
    inline void LODS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size == 0) {
                    return this->LODS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size > 0) {
                        return this->LODS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->LODS_impl<uint8_t, uint16_t>();
        }
        else {
            if constexpr (bits > 16) {
                if (this->addr_size == 0) {
                    if (this->data_size == 0) {
                        return this->LODS_impl<uint32_t, uint32_t>();
                    }
                    if constexpr (bits == 64) {
                        if (data_size > 0) {
                            return this->LODS_impl<uint64_t, uint32_t>();
                        }
                    }
                    return this->LODS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size > 0) {
                        if (this->data_size == 0) {
                            return this->LODS_impl<uint32_t, uint64_t>();
                        }
                        if constexpr (bits == 64) {
                            if (this->data_size > 0) {
                                return this->LODS_impl<uint64_t, uint64_t>();
                            }
                        }
                        return this->LODS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size == 0) {
                    return this->LODS_impl<uint32_t, uint16_t>();
                }
                if constexpr (bits == 64) {
                    if (this->data_size > 0) {
                        return this->LODS_impl<uint64_t, uint16_t>();
                    }
                }
            }
            return this->LODS_impl<uint16_t, uint16_t>();
        }
    }

    template <typename T, typename P>
    inline void MOVS_impl();

    template <bool is_byte = false>
    inline void MOVS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size == 0) {
                    return this->MOVS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size > 0) {
                        return this->MOVS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->MOVS_impl<uint8_t, uint16_t>();
        }
        else {
            if constexpr (bits > 16) {
                if (this->addr_size == 0) {
                    if (this->data_size == 0) {
                        return this->MOVS_impl<uint32_t, uint32_t>();
                    }
                    if constexpr (bits == 64) {
                        if (data_size > 0) {
                            return this->MOVS_impl<uint64_t, uint32_t>();
                        }
                    }
                    return this->MOVS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size > 0) {
                        if (this->data_size == 0) {
                            return this->MOVS_impl<uint32_t, uint64_t>();
                        }
                        if constexpr (bits == 64) {
                            if (this->data_size > 0) {
                                return this->MOVS_impl<uint64_t, uint64_t>();
                            }
                        }
                        return this->MOVS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size == 0) {
                    return this->MOVS_impl<uint32_t, uint16_t>();
                }
                if constexpr (bits == 64) {
                    if (this->data_size > 0) {
                        return this->MOVS_impl<uint64_t, uint16_t>();
                    }
                }
            }
            return this->MOVS_impl<uint16_t, uint16_t>();
        }
    }

    // TODO: Read microcode dump to confirm accurate behavior of BCD, there's reason to doubt official docs here
    inline void AAA() {
        if (this->auxiliary || (this->al & 0xF) > 9) {
            this->auxiliary = this->carry = true;
            this->ax += 0x106;
        } else {
            this->carry = false;
        }
        this->al &= 0xF;
    }

    inline void AAS() {
        if (this->auxiliary || (this->al & 0xF) > 9) {
            this->auxiliary = this->carry = true;
            this->ax -= 0x106;
        } else {
            this->carry = false;
        }
        this->al &= 0xF;
    }

    // Supposedly better implementation than official docs: https://www.righto.com/2023/01/understanding-x86s-decimal-adjust-after.html
    inline void DAA() {
        uint8_t temp = this->al;
        if (this->auxiliary || (temp & 0xF) > 9) {
            this->auxiliary = true;
            this->al -= 0x06;
        }
        if (this->carry || temp > 0x99) {
            this->carry = true;
            this->al -= 0x60;
        }
    }

    inline void DAS() {
        uint8_t temp = this->al;
        if (this->auxiliary || (temp & 0xF) > 9) {
            this->auxiliary = true;
            this->al += 0x06;
        }
        if (this->carry || temp > 0x99) {
            this->carry = true;
            this->al += 0x60;
        }
    }

    inline void AAM(uint8_t imm) {
        if (imm) {
            this->ah = this->al / imm;
            this->al %= imm;
            this->update_pzs(this->al);
        }
        else {
            this->software_interrupt(IntDE);
        }
    }

    inline void AAD(uint8_t imm) {
        uint8_t temp = this->al + this->ah * imm;
        this->ax = temp;
        this->update_pzs(temp);
    }

    inline void software_interrupt(uint8_t number) {
        this->pending_sinterrupt = number;
    }

    template <typename T, typename P, typename L>
    inline void binopMR_impl(P& pc, const L& lambda);

    template <bool is_byte = false, typename P, typename L>
    inline void binopMR(P& pc, const L& lambda) {
        if constexpr (is_byte) {
            return this->binopMR_impl<uint8_t>(pc, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size == 0) {
                    return this->binopMR_impl<uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size > 0) {
                        return this->binopMR_impl<uint64_t>(pc, lambda);
                    }
                }
            }
            return this->binopMR_impl<uint16_t>(pc, lambda);
        }
    }

    template <typename T, typename P, typename L>
    inline void binopRM_impl(P& pc, const L& lambda);

    template <bool is_byte = false, typename P, typename L>
    inline void binopRM(P& pc, const L& lambda) {
        if constexpr (is_byte) {
            return this->binopRM_impl<uint8_t>(pc, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size == 0) {
                    return this->binopRM_impl<uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size > 0) {
                        return this->binopRM_impl<uint64_t>(pc, lambda);
                    }
                }
            }
            return this->binopRM_impl<uint16_t>(pc, lambda);
        }
    }

    template <typename T, typename P, typename L>
    inline void binopRM2_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline void binopRM2(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size == 0) {
                return this->binopRM2_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size > 0) {
                    return this->binopRM2_impl<uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopRM2_impl<uint16_t>(pc, lambda);
    }

    template <typename T, typename P, typename L>
    inline void binopMS_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline void binopMS(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size == 0) {
                return this->binopMS_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size > 0) {
                    return this->binopMS_impl<uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopMS_impl<uint16_t>(pc, lambda);
    }

    template <typename T, typename P, typename L>
    inline void binopSM_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline void binopSM(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size == 0) {
                return this->binopSM_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size > 0) {
                    return this->binopSM_impl<uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopSM_impl<uint16_t>(pc, lambda);
    }

    template <typename T, typename P, typename L>
    inline void unopM_impl(P& pc, const L& lambda);

    template <bool is_byte = false, typename P, typename L>
    inline void unopM(P& pc, const L& lambda) {
        if constexpr (is_byte) {
            return this->unopM_impl<uint8_t>(pc, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size == 0) {
                    return this->unopM_impl<uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size > 0) {
                        return this->unopM_impl<uint64_t>(pc, lambda);
                    }
                }
            }
            return this->unopM_impl<uint16_t>(pc, lambda);
        }
    }

    template <typename T, typename P>
    inline bool unopMS_impl(P& pc);

    template <bool is_byte = false, typename P>
    inline bool unopMS(P& pc) {
        if constexpr (is_byte) {
            return this->unopMS_impl<uint8_t>(pc);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size == 0) {
                    return this->unopMS_impl<uint32_t>(pc);
                }
                if constexpr (bits == 64) {
                    if (this->data_size > 0) {
                        return this->unopMS_impl<uint64_t>(pc);
                    }
                }
            }
            return this->unopMS_impl<uint16_t>(pc);
        }
    }
};

#endif