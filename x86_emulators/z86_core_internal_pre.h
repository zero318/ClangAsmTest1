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

#undef REX

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

enum CONDITION_CODE : uint8_t {
    CondO = 0,
    CondNO = 1,
    CondC = 2, CondB = CondC, CondNAE = CondC,
    CondNC = 3, CondNB = CondNC, CondAE = CondNC,
    CondZ = 4, CondE = CondZ,
    CondNZ = 5, CondNE = CondNZ,
    CondBE = 6, CondNA = CondBE,
    CondA = 7, CondNBE = CondA,
    CondS = 8,
    CondNS = 9,
    CondP = 10, CondPE = CondP,
    CondNP = 11, CondPO = CondP,
    CondL = 12, CondNGE = CondL,
    CondGE = 13, CondNL = CondGE,
    CondLE = 14, CondNG = CondLE,
    CondG = 15, CondNLE = CondG
};

enum DATA_SIZE : int8_t {
    DataSize64 = -1,
    DataSize32 = 0,
    DataSize16 = 1
};

enum ADDR_SIZE : int8_t {
    AddrSize64 = -1,
    AddrSize32 = 0,
    AddrSize16 = 1
};

enum REP_STATE : int8_t {
    NO_REP = -1,
    REP_NE = 0,
    REP_E = 1
};



struct REX {
#if USE_BITFIELDS
    union {
        uint8_t raw;
        struct {
            uint8_t b : 1;
            uint8_t x : 1;
            uint8_t r : 1;
            uint8_t w : 1;
            uint8_t pre : 4;
        };
    };

    inline constexpr REX() : raw(0) {}

    inline constexpr uint8_t B() const {
        return this->b << 3;;
    }

    inline constexpr uint8_t X() const {
        return this->x << 3;
    }

    inline constexpr uint8_t R() const {
        return this->r << 3;
    }

    inline constexpr bool W() const {
        return this->w;
    }
#else
    uint8_t raw;

    inline constexpr uint8_t B() const {
        return (this->raw & 1) << 3;
    }

    inline constexpr uint8_t X() const {
        return (this->raw & 2) << 2;
    }

    inline constexpr uint8_t R() const {
        return (this->raw & 4) << 1;
    }

    inline constexpr bool W() const {
        return this->raw & 8;
    }
#endif

    inline constexpr bool is_present() const {
        return this->raw;
    }

    inline constexpr operator bool() const {
        return this->raw;
    }

    inline constexpr bool operator!() const {
        return !this->raw;
    }
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

    static constexpr inline int8_t data_size = 1;
    static constexpr inline int8_t addr_size = 1;
    static constexpr inline int8_t stack_size = 1;
    static constexpr inline int8_t mode = 1;
    static constexpr inline REX rex_bits = {};
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

    int8_t data_size = 1;
    int8_t addr_size = 1;
    int8_t stack_size = 1;
    int8_t mode = 1;
    static constexpr inline REX rex_bits = {};

    inline constexpr uint8_t& index_byte_reg(uint8_t index) {
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    inline constexpr auto& index_word_reg(uint8_t index) {
        return this->gpr[index].word;
    }

    inline constexpr auto& index_dword_reg(uint8_t index) {
        return this->gpr[index].dword;
    }

    inline constexpr auto& index_qword_reg(uint8_t index) {
        return this->gpr[index].qword;
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


    int8_t data_size = 1;
    int8_t addr_size = 1;
    int8_t stack_size = 1;
    int8_t mode = 1;
    REX rex_bits = {};

    

    inline constexpr auto& index_word_reg(uint8_t index) {
        return this->gpr[index].word;
    }

    inline constexpr auto& index_dword_reg(uint8_t index) {
        return this->gpr[index].dword;
    }

    inline constexpr auto& index_qword_reg(uint8_t index) {
        return this->gpr[index].qword;
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

    template <size_t other_bits>
    inline constexpr z86AddrImpl(const z86AddrImpl<other_bits>& addr) : z86AddrBase<bits>::z86AddrBase(addr.segment, addr.offset) {}

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
    inline void write(const T& value, ssize_t offset = 0);

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read(ssize_t offset = 0) const;

    template <typename T = uint8_t>
    inline void write_advance(const T& value, ssize_t index = sizeof(T)) {
        this->write(value);
        this->offset += index;
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read_advance(ssize_t index = sizeof(V)) {
        V ret = this->read<V>();
        this->offset += index;
        return ret;
    }

    inline uint32_t read_Iz(ssize_t index = 0) const;

    inline uint32_t read_advance_Iz();

    inline int32_t read_Is(ssize_t index = 0) const;

    inline int32_t read_advance_Is();

    inline uint64_t read_Iv(ssize_t index = 0) const;

    inline uint64_t read_advance_Iv();

    inline uint64_t read_O(ssize_t index = 0) const;

    inline uint64_t read_advance_O();
};

struct SIB {
#if USE_BITFIELDS
    union {
        uint8_t raw;
        struct {
            uint8_t b : 3;
            uint8_t i : 3;
            uint8_t s : 2;
        };
    };

    inline constexpr uint8_t S() const {
        return this->s;
    }

    inline constexpr uint8_t I() const {
        return this->i;
    }

    inline constexpr uint8_t B() const {
        return this->b;
    }
#else
    uint8_t raw;

    inline constexpr uint8_t S() const {
        return this->raw >> 6;
}

    inline constexpr uint8_t I() const {
        return (this->raw & 0070) >> 3;
    }

    inline constexpr uint8_t B() const {
        return this->raw & 0007;
    }
#endif
};

struct ModRM {
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

    template <typename P>
    uint32_t extra_length(const P& pc) const;

    template <typename P>
    auto parse_memM(P& pc) const;
};

template <size_t bits, size_t bus>
struct z86Base : z86BaseGPRs<bits> {

    static inline constexpr size_t max_bits = bits;
    static inline constexpr size_t bus_width = bus;

    using HT = z86BaseGPRs<bits>::HT;
    using RT = z86BaseGPRs<bits>::RT;
    using DT = z86BaseGPRs<bits>::DT;

    using SRT = std::make_signed_t<RT>;

    inline constexpr bool data_size_16() const {
        return data_size > 0;
    }
    inline constexpr bool data_size_32() const {
        return data_size == 0;
    }
    inline constexpr bool data_size_64() const {
        return data_size < 0;
    }
    inline constexpr bool addr_size_16() const {
        return addr_size > 0;
    }
    inline constexpr bool addr_size_32() const {
        return addr_size == 0;
    }
    inline constexpr bool addr_size_64() const {
        return addr_size < 0;
    }
    inline constexpr bool stack_size_16() const {
        return stack_size > 0;
    }
    inline constexpr bool stack_size_32() const {
        return stack_size == 0;
    }
    inline constexpr bool stack_size_64() const {
        return stack_size < 0;
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t& index_byte_regR(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (rex_bits) {
                return this->gpr[index | rex_bits.R()].byte;
            }
        }
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t& index_byte_regI(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (rex_bits) {
                return this->gpr[index | rex_bits.X()].byte;
            }
        }
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t& index_byte_regMB(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (rex_bits) {
                return this->gpr[index | rex_bits.B()].byte;
            }
        }
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_word_regR(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.R();
        }
        return this->gpr[index].word;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_word_regI(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.X();
        }
        return this->gpr[index].word;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_word_regMB(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.B();
        }
        return this->gpr[index].word;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_dword_regR(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.R();
        }
        return this->gpr[index].dword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_dword_regI(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.X();
        }
        return this->gpr[index].dword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_dword_regMB(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.B();
        }
        return this->gpr[index].dword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_qword_regR(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.R();
        }
        return this->gpr[index].qword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_qword_regI(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.X();
        }
        return this->gpr[index].qword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_qword_regMB(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= rex_bits.B();
        }
        return this->gpr[index].qword;
    }

    template <typename T, bool ignore_rex = false>
    inline constexpr auto& index_regR(uint8_t index) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->index_byte_regR<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->index_word_regR<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->index_dword_regR<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->index_qword_regR<ignore_rex>(index);
        }
    }

    template <typename T, bool ignore_rex = false>
    inline constexpr auto& index_regI(uint8_t index) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->index_byte_regI<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->index_word_regI<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->index_dword_regI<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->index_qword_regI<ignore_rex>(index);
        }
    }

    template <typename T, bool ignore_rex = false>
    inline constexpr auto& index_regMB(uint8_t index) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->index_byte_regMB<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return this->index_word_regMB<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return this->index_dword_regMB<ignore_rex>(index);
        } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return this->index_qword_regMB<ignore_rex>(index);
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

    inline constexpr bool cond_O(bool val = true) const { return this->overflow == val; }
    inline constexpr bool cond_NO(bool val = true) const { return this->overflow != val; }
    inline constexpr bool cond_C(bool val = true) const { return this->carry == val; }
    inline constexpr bool cond_B(bool val = true) const { return this->cond_C(val); }
    inline constexpr bool cond_NAE(bool val = true) const { return this->cond_C(val); }
    inline constexpr bool cond_NC(bool val = true) const { return this->carry != val; }
    inline constexpr bool cond_NB(bool val = true) const { return this->cond_NC(val); }
    inline constexpr bool cond_AE(bool val = true) const { return this->cond_NC(val); }
    inline constexpr bool cond_Z(bool val = true) const { return this->zero == val; }
    inline constexpr bool cond_E(bool val = true) const { return this->cond_Z(val); }
    inline constexpr bool cond_NZ(bool val = true) const { return this->zero != val; }
    inline constexpr bool cond_NE(bool val = true) const { return this->cond_NZ(val); }
    inline constexpr bool cond_BE(bool val = true) const { return (this->carry || this->zero) == val; }
    inline constexpr bool cond_NA(bool val = true) const { return this->cond_BE(val); }
    inline constexpr bool cond_A(bool val = true) const { return (this->carry || this->zero) != val; }
    inline constexpr bool cond_NBE(bool val = true) const { return this->cond_A(val); }
    inline constexpr bool cond_S(bool val = true) const { return this->sign == val; }
    inline constexpr bool cond_NS(bool val = true) const { return this->sign != val; }
    inline constexpr bool cond_P(bool val = true) const { return this->parity == val; }
    inline constexpr bool cond_PE(bool val = true) const { return this->cond_P(val); }
    inline constexpr bool cond_NP(bool val = true) const { return this->parity != val; }
    inline constexpr bool cond_PO(bool val = true) const { return this->cond_NP(val); }
    inline constexpr bool cond_L(bool val = true) const { return (this->sign != this->overflow) == val; }
    inline constexpr bool cond_NGE(bool val = true) const { return this->cond_L(val); }
    inline constexpr bool cond_GE(bool val = true) const { return (this->sign != this->overflow) != val; }
    inline constexpr bool cond_NL(bool val = true) const { return this->cond_GE(val); }
    inline constexpr bool cond_LE(bool val = true) const { return (this->zero || this->sign != this->overflow) == val; }
    inline constexpr bool cond_NG(bool val = true) const { return this->cond_LE(val); }
    inline constexpr bool cond_G(bool val = true) const { return (this->zero || this->sign != this->overflow) != val; }
    inline constexpr bool cond_NLE(bool val = true) const { return this->cond_G(val); }

    template <CONDITION_CODE cc>
    inline constexpr bool cond(bool val = true) const {
        if constexpr (cc == CondO) return this->cond_O(val);
        else if constexpr (cc == CondNO) return this->cond_NO(val);
        else if constexpr (cc == CondC) return this->cond_C(val);
        else if constexpr (cc == CondNC) return this->cond_NC(val);
        else if constexpr (cc == CondZ) return this->cond_Z(val);
        else if constexpr (cc == CondNZ) return this->cond_NZ(val);
        else if constexpr (cc == CondBE) return this->cond_BE(val);
        else if constexpr (cc == CondA) return this->cond_A(val);
        else if constexpr (cc == CondS) return this->cond_S(val);
        else if constexpr (cc == CondNS) return this->cond_NS(val);
        else if constexpr (cc == CondP) return this->cond_P(val);
        else if constexpr (cc == CondNP) return this->cond_NP(val);
        else if constexpr (cc == CondL) return this->cond_L(val);
        else if constexpr (cc == CondGE) return this->cond_L(val);
        else if constexpr (cc == CondLE) return this->cond_LE(val);
        else if constexpr (cc == CondG) return this->cond_G(val);
    }

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

    template <typename P = void>
    inline constexpr RT stack_size_bp() const {
        if constexpr (std::is_same_v<P, void>) {
            // No size specified, calculate it
            if constexpr (bits > 16) {
                if (this->stack_size_32()) {
                    return this->ebp;
                }
                if constexpr (bits == 64) {
                    if (this->stack_size_64()) {
                        return this->rbp;
                    }
                }
            }
        }
        else if constexpr (sizeof(P) == sizeof(uint64_t)) {
            return this->rbp;
        }
        else if constexpr (sizeof(P) == sizeof(uint32_t)) {
            return this->ebp;
        }
        return this->bp;
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

    template <typename P>
    inline void JMPFABS(const P& pc) {
        if constexpr (bits == 64) {
            // TODO: Exception conditions
        }
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->JMPFABS(pc.read<uint32_t>(), pc.read<uint16_t>(4));
            }
        }
        return this->JMPFABS(pc.read<uint16_t>(), pc.read<uint16_t>(2));
    }

    template <typename P, typename T>
    inline void PUSH_impl(T src);

    template <typename T = SRT>
    inline void PUSH(T src) {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            if constexpr (bits == 64) {
                // 64 bit values can only be pushed in long
                // mode, where the stack is always 64 bit
                return this->PUSH_impl<uint64_t>(src);
            }
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            if constexpr (bits > 16) {
                // No need to check for 64 bit stack size
                // because a 32 bit push can't be encoded
                // when running in long mode (hopefully)
                if (this->stack_size_32()) {
                    return this->PUSH_impl<uint32_t>(src);
                }
            }
        }
        else {
            // 16 bit pushes are horrible and exist
            // in all modes
            if constexpr (bits > 16) {
                if (this->stack_size_32()) {
                    return this->PUSH_impl<uint32_t>(src);
                }
                if constexpr (bits == 64) {
                    if (this->stack_size_64()) {
                        return this->PUSH_impl<uint64_t>(src);
                    }
                }
            }
        }
        // This is doubling as a backup case for the constexprs
        return this->PUSH_impl<uint16_t>(src);
    }

    template <typename P, typename T>
    inline T POP_impl();

    template <typename T = RT>
    inline T POP() {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            if constexpr (bits == 64) {
                // 64 bit values can only be popped in long
                // mode, where the stack is always 64 bit
                return this->POP_impl<uint64_t, T>();
            }
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            if constexpr (bits > 16) {
                // No need to check for 64 bit stack size
                // because a 32 bit pop can't be encoded
                // when running in long mode (hopefully)
                if (this->stack_size_32()) {
                    return this->POP_impl<uint32_t, T>();
                }
            }
        }
        else {
            // 16 bit pops are horrible and exist
            // in all modes
            if constexpr (bits > 16) {
                if (this->stack_size_32()) {
                    return this->POP_impl<uint32_t, T>();
                }
                if constexpr (bits == 64) {
                    if (this->stack_size_64()) {
                        return this->POP_impl<uint64_t, T>();
                    }
                }
            }
        }
        // This is doubling as a backup case for the constexprs
        return this->POP_impl<uint16_t, T>();
    }

    template <typename T>
    inline void ENTER_impl(uint16_t alloc, uint8_t nesting);


    // http://www.os2museum.com/wp/if-you-enter-you-might-not-leave/
    inline void ENTER(uint16_t alloc, uint8_t nesting) {
        nesting &= 0x1F;
    }

    template <typename P>
    inline void CALL(const P& pc) {
        auto next_ip = pc.offset;
        if constexpr (bits > 16) {
            if (!this->data_size_16()) {
                next_ip += 4;
                auto dest_ip = next_ip + pc.read<int32_t>();
                if (this->data_size_32()) {
                    return this->CALLABS<uint32_t>(next_ip, dest_ip);
                }
                else {
                    return this->CALLABS<uint64_t>(next_ip, dest_ip);
                }
            }
        }
        next_ip += 2;
        return this->CALLABS<uint16_t>(next_ip, next_ip + pc.read<int16_t>());
    }

    inline void RET() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                this->rip = this->POP<uint32_t>();
                return;
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    this->rip = this->POP<uint64_t>();
                    return;
                }
            }
        }
        this->rip = this->POP<uint16_t>();
    }

    inline void RETF() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                this->rip = this->POP<uint32_t>();
                this->cs = this->POP<uint32_t>();
                return;
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    this->rip = this->POP<uint64_t>();
                    this->cs = this->POP<uint64_t>();
                    return;
                }
            }
        }
        this->rip = this->POP<uint16_t>();
        this->cs = this->POP<uint16_t>();
    }

    template <typename P>
    inline void RETI(const P& pc) {
        this->RET();
        if constexpr (bits > 16) {
            if (this->stack_size_32()) {
                this->esp += pc.read<uint16_t>();
                return;
            }
            if constexpr (bits == 64) {
                if (this->stack_size_64()) {
                    this->rsp += pc.read<uint16_t>();
                    return;
                }
            }
        }
        this->sp += pc.read<uint16_t>();
    }

    template <typename P>
    inline void RETFI(const P& pc) {
        this->RETF();
        if constexpr (bits > 16) {
            if (this->stack_size_32()) {
                this->esp += pc.read<uint16_t>();
                return;
            }
            if constexpr (bits == 64) {
                if (this->stack_size_64()) {
                    this->rsp += pc.read<uint16_t>();
                    return;
                }
            }
        }
        this->sp += pc.read<uint16_t>();
    }

    template <bool is_byte = false, typename P>
    inline void JMP(const P& pc) {
        if constexpr (is_byte) {
            auto new_ip = pc.offset + 1 + pc.read<int8_t>();
            if constexpr (bits > 16) {
                if (this->data_size_16()) {
                    new_ip = (uint16_t)new_ip;
                }
            }
            ctx.rip = new_ip;
        }
        else {
            if constexpr (bits > 16) {
                if (!this->data_size_16()) {
                    ctx.rip = pc.offset + 4 + pc.read<int32_t>();
                    return;
                }
            }
            ctx.rip = (uint16_t)(pc.offset + 2 + pc.read<int16_t>());
        }
    }

    template <CONDITION_CODE cc, bool is_byte = false, typename P>
    inline void JCC(const P& pc, bool val = true) {
        if constexpr (is_byte) {
            auto new_ip = pc.offset + 1;
            if (this->cond<cc>(val)) {
                new_ip += pc.read<int8_t>();
            }
            if constexpr (bits > 16) {
                if (this->data_size_16()) {
                    new_ip = (uint16_t)new_ip;
                }
            }
            ctx.rip = new_ip;
        }
        else {
            auto new_ip = pc.offset + 2;
            if constexpr (bits > 16) {
                new_ip += !this->data_size_16() * 2;
            }
            if (this->cond<cc>(val)) {
                if constexpr (bits > 16) {
                    if (!this->data_size_16()) {
                        new_ip += pc.read<int32_t>();
                        goto set_rip;
                    }
                }
                new_ip += pc.read<int16_t>();
            }
        set_rip:
            if constexpr (bits > 16) {
                if (this->data_size_16()) {
                    new_ip = (uint16_t)new_ip;
                }
            }
            ctx.rip = new_ip;
        }
    }

    template <typename T, typename P>
    inline void LOOP_impl(const P& pc, T& index) {
        auto new_ip = pc.offset + 1;
        if (--index) {
            new_ip += pc.read<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        ctx.rip = new_ip;
    }

    template <typename P>
    inline void LOOP(const P& pc) {
        if constexpr (bits > 16) {
            if (this->addr_size_32()) {
                return this->LOOP_impl(pc, this->ecx);
            }
            if constexpr (bits == 64) {
                if (this->addr_size_64()) {
                    return this->LOOP_impl(pc, this->rcx);
                }
            }
        }
        return this->LOOP_impl(pc, this->cx);
    }

    template <typename T, typename P>
    inline void LOOPCC_impl(const P& pc, T& index, bool val) {
        auto new_ip = pc.offset + 1;
        if (--index || this->cond_Z(val)) {
            new_ip += pc.read<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        ctx.rip = new_ip;
    }

    template <typename P>
    inline void LOOPCC(const P& pc, bool val) {
        if constexpr (bits > 16) {
            if (this->addr_size_32()) {
                return this->LOOPCC_impl(pc, this->ecx, val);
            }
            if constexpr (bits == 64) {
                if (this->addr_size_64()) {
                    return this->LOOPCC_impl(pc, this->rcx, val);
                }
            }
        }
        return this->LOOPCC_impl(pc, this->cx, val);
    }

    template <typename T, typename P>
    inline void JCXZ_impl(const P& pc, const T& index) {
        auto new_ip = pc.offset + 1;
        if (!index) {
            new_ip += pc.read<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        ctx.rip = new_ip;
    }

    template <typename P>
    inline void JCXZ(const P& pc) {
        if constexpr (bits > 16) {
            if (this->addr_size_32()) {
                return this->JCXZ_impl(pc, this->ecx);
            }
            if constexpr (bits == 64) {
                if (this->addr_size_64()) {
                    return this->JCXZ_impl(pc, this->rcx);
                }
            }
        }
        return this->JCXZ_impl(pc, this->cx);
    }

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

    template <typename P>
    inline void CALLFABS(const P& pc) {
        if constexpr (bits == 64) {
            // TODO: Exception condition
        }
        auto next_ip = pc.offset + 4;
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->CALLFABS<uint32_t>(next_ip + 2, pc.read<uint32_t>(), pc.read<uint16_t>(4));
            }
        }
        return this->CALLFABS<uint16_t>(next_ip, pc.read<uint16_t>(), pc.read<uint16_t>(2));
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

    template <typename T1, typename T2>
    inline void ADD(T1& dst, T2 src) {
        return this->ADD<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void OR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst |= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void OR(T1& dst, T2 src) {
        return this->OR<T1>(dst, (std::make_signed_t<T1>)src);
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

    template <typename T1, typename T2>
    inline void ADC(T1& dst, T2 src) {
        return this->ADC<T1>(dst, (std::make_signed_t<T1>)src);
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

    template <typename T1, typename T2>
    inline void SBB(T1& dst, T2 src) {
        return this->SBB<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void AND(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst &= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void AND(T1& dst, T2 src) {
        return this->AND<T1>(dst, (std::make_signed_t<T1>)src);
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

    template <typename T1, typename T2>
    inline void SUB(T1& dst, T2 src) {
        return this->SUB<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void XOR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst ^= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void XOR(T1& dst, T2 src) {
        return this->XOR<T1>(dst, (std::make_signed_t<T1>)src);
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

    template <typename T1, typename T2>
    inline void CMP(T1 dst, T2 src) {
        return this->CMP<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void TEST(T dst, T src) {
        this->carry = false;
        this->overflow = false;
        this->update_pzs((T)(dst & src));
    }

    template <typename T1, typename T2>
    inline void TEST(T1 dst, T2 src) {
        return this->TEST<T1>(dst, (std::make_signed_t<T1>)src);
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

    inline void CBW() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                // CWDE
                ctx.eax = (int32_t)(int16_t)ctx.ax;
                return;
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    // CDQE
                    ctx.rax = (int64_t)(int32_t)ctx.eax;
                    return;
                }
            }
        }
        // CBW
        ctx.ax = (int16_t)(int8_t)ctx.al;
    }

    inline void CWD() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                // CDQ
                ctx.edx = (int32_t)ctx.eax >> 31;
                return;
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    // CDO
                    ctx.rdx = (int64_t)ctx.rax >> 63;
                    return;
                }
            }
        }
        // CWD
        ctx.dx = (int16_t)ctx.ax >> 15;
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

    template <typename T>
    inline void BSF(T& dst, T src) {
        if (!(this->zero = !src)) {
            for (size_t i = 0; i < bitsof(T); ++i) {
                if (src >> i & 1) {
                    dst = i;
                    return;
                }
            }
        }
    }

    template <typename T>
    inline void BSR(T& dst, T src) {
        if (!(this->zero = !src)) {
            size_t i = bitsof(T) - 1;
            do {
                if (src >> i & 1) {
                    dst = i;
                    return;
                }
            } while (i--);
        }
    }

    template <typename T>
    inline void BSWAP(T& src) {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            src = __builtin_bswap64(src);
        }
        else {
            // 16 bit operands result in a 0 output
            // because of internl extension to 32 bit
            src = __builtin_bswap32(src);
        }
    }

    template <typename T, typename P>
    inline void LODS_impl();

    template <bool is_byte = false>
    inline void LODS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    return this->LODS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        return this->LODS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->LODS_impl<uint8_t, uint16_t>();
        }
        else {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    if (this->data_size_32()) {
                        return this->LODS_impl<uint32_t, uint32_t>();
                    }
                    if constexpr (bits == 64) {
                        if (this->data_size_64()) {
                            return this->LODS_impl<uint64_t, uint32_t>();
                        }
                    }
                    return this->LODS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        if (this->data_size_32()) {
                            return this->LODS_impl<uint32_t, uint64_t>();
                        }
                        if constexpr (bits == 64) {
                            if (this->data_size_64()) {
                                return this->LODS_impl<uint64_t, uint64_t>();
                            }
                        }
                        return this->LODS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size_32()) {
                    return this->LODS_impl<uint32_t, uint16_t>();
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
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
                if (this->addr_size_32()) {
                    return this->MOVS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        return this->MOVS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->MOVS_impl<uint8_t, uint16_t>();
        }
        else {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    if (this->data_size_32()) {
                        return this->MOVS_impl<uint32_t, uint32_t>();
                    }
                    if constexpr (bits == 64) {
                        if (this->data_size_64()) {
                            return this->MOVS_impl<uint64_t, uint32_t>();
                        }
                    }
                    return this->MOVS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        if (this->data_size_32()) {
                            return this->MOVS_impl<uint32_t, uint64_t>();
                        }
                        if constexpr (bits == 64) {
                            if (this->data_size_64()) {
                                return this->MOVS_impl<uint64_t, uint64_t>();
                            }
                        }
                        return this->MOVS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size_32()) {
                    return this->MOVS_impl<uint32_t, uint16_t>();
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->MOVS_impl<uint64_t, uint16_t>();
                    }
                }
            }
            return this->MOVS_impl<uint16_t, uint16_t>();
        }
    }

    template <typename T, typename P>
    inline void STOS_impl();

    template <bool is_byte = false>
    inline void STOS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    return this->STOS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        return this->STOS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->STOS_impl<uint8_t, uint16_t>();
        }
        else {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    if (this->data_size_32()) {
                        return this->STOS_impl<uint32_t, uint32_t>();
                    }
                    if constexpr (bits == 64) {
                        if (this->data_size_64()) {
                            return this->STOS_impl<uint64_t, uint32_t>();
                        }
                    }
                    return this->STOS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        if (this->data_size_32()) {
                            return this->STOS_impl<uint32_t, uint64_t>();
                        }
                        if constexpr (bits == 64) {
                            if (this->data_size_64()) {
                                return this->STOS_impl<uint64_t, uint64_t>();
                            }
                        }
                        return this->STOS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size_32()) {
                    return this->STOS_impl<uint32_t, uint16_t>();
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->STOS_impl<uint64_t, uint16_t>();
                    }
                }
            }
            return this->STOS_impl<uint16_t, uint16_t>();
        }
    }

    template <typename T, typename P>
    inline void SCAS_impl();

    template <bool is_byte = false>
    inline void SCAS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    return this->SCAS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        return this->SCAS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->SCAS_impl<uint8_t, uint16_t>();
        } else {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    if (this->data_size_32()) {
                        return this->SCAS_impl<uint32_t, uint32_t>();
                    }
                    if constexpr (bits == 64) {
                        if (this->data_size_64()) {
                            return this->SCAS_impl<uint64_t, uint32_t>();
                        }
                    }
                    return this->SCAS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        if (this->data_size_32()) {
                            return this->SCAS_impl<uint32_t, uint64_t>();
                        }
                        if constexpr (bits == 64) {
                            if (this->data_size_64()) {
                                return this->SCAS_impl<uint64_t, uint64_t>();
                            }
                        }
                        return this->SCAS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size_32()) {
                    return this->SCAS_impl<uint32_t, uint16_t>();
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->SCAS_impl<uint64_t, uint16_t>();
                    }
                }
            }
            return this->SCAS_impl<uint16_t, uint16_t>();
        }
    }

    template <typename T, typename P>
    inline void CMPS_impl();

    template <bool is_byte = false>
    inline void CMPS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    return this->CMPS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        return this->CMPS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->CMPS_impl<uint8_t, uint16_t>();
        } else {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    if (this->data_size_32()) {
                        return this->CMPS_impl<uint32_t, uint32_t>();
                    }
                    if constexpr (bits == 64) {
                        if (this->data_size_64()) {
                            return this->CMPS_impl<uint64_t, uint32_t>();
                        }
                    }
                    return this->CMPS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        if (this->data_size_32()) {
                            return this->CMPS_impl<uint32_t, uint64_t>();
                        }
                        if constexpr (bits == 64) {
                            if (this->data_size_64()) {
                                return this->CMPS_impl<uint64_t, uint64_t>();
                            }
                        }
                        return this->CMPS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size_32()) {
                    return this->CMPS_impl<uint32_t, uint16_t>();
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->CMPS_impl<uint64_t, uint16_t>();
                    }
                }
            }
            return this->CMPS_impl<uint16_t, uint16_t>();
        }
    }

    template <typename T>
    inline void port_out_impl(uint16_t port) const;

    template <bool is_byte = false>
    inline void port_out(uint16_t port) const {
        if constexpr (is_byte) {
            return this->port_out_impl<uint8_t>(port);
        }
        else {
            if constexpr (bits > 16) {
                if (!this->data_size_16()) {
                    return this->port_out_impl<uint32_t>(port);
                }
            }
            return this->port_out_impl<uint16_t>(port);
        }
    }

    template <typename T>
    inline void port_in_impl(uint16_t port);

    template <bool is_byte = false>
    inline void port_in(uint16_t port) {
        if constexpr (is_byte) {
            return this->port_in_impl<uint8_t>(port);
        }
        else {
            if constexpr (bits > 16) {
                if (!this->data_size_16()) {
                    return this->port_in_impl<uint32_t>(port);
                }
            }
            return this->port_in_impl<uint16_t>(port);
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

    template <bool is_byte = false, typename L>
    inline void binopAR(uint8_t index, const L& lambda) {
        if constexpr (is_byte) {
            return lambda(ctx.al, this->index_byte_regMB(index));
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return lambda(ctx.eax, this->index_dword_regMB(index));
                }
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return lambda(ctx.rax, this->index_qword_regMB(index));
                }
            }
            return lambda(ctx.ax, this->index_word_regMB(index));
        }
    }

    template <bool is_byte = false, typename P, typename L>
    inline void binopAI(P& pc, const L& lambda) {
        if constexpr (is_byte) {
            return lambda(ctx.al, pc.read_advance<uint8_t>());
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return lambda(ctx.eax, pc.read_advance<uint32_t>());
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return lambda(ctx.rax, (uint64_t)(int64_t)pc.read_advance<int32_t>());
                    }
                }
            }
            return lambda(ctx.ax, pc.read_advance<uint16_t>());
        }
    }

    template <bool is_byte = false, typename P, typename L>
    inline void binopAO(P& pc, const L& lambda) {
        auto offset = pc.read_advance_O();
        if constexpr (is_byte) {
            return lambda(ctx.al, offset);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return lambda(ctx.eax, offset);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return lambda(ctx.rax, offset);
                    }
                }
            }
            return lambda(ctx.ax, offset);
        }
    }

    template <bool is_byte = false, typename P>
    inline void MOV_RI(P& pc, uint8_t index) {
        if constexpr (is_byte) {
            this->index_byte_regMB(index) = pc.read_advance<uint8_t>();
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    this->index_dword_regMB(index) = pc.read_advance<uint32_t>();
                    return;
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        this->index_qword_regMB(index) = pc.read_advance<uint64_t>();
                        return;
                    }
                }
            }
            this->index_word_regMB(index) = pc.read_advance<uint16_t>();
        }
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
                if (this->data_size_32()) {
                    return this->binopMR_impl<uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
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
                if (this->data_size_32()) {
                    return this->binopRM_impl<uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
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
            if (this->data_size_32()) {
                return this->binopRM2_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
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
            if (this->data_size_32()) {
                return this->binopMS_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
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
            if (this->data_size_32()) {
                return this->binopSM_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
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
                if (this->data_size_32()) {
                    return this->unopM_impl<uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
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
                if (this->data_size_32()) {
                    return this->unopMS_impl<uint32_t>(pc);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->unopMS_impl<uint64_t>(pc);
                    }
                }
            }
            return this->unopMS_impl<uint16_t>(pc);
        }
    }
};

#endif