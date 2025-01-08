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
//#include <new>
//#include <memory>

// TODO: Investigate these:
// https://gist.github.com/TomHarte/f9c89349f676ebbe5bc92e52af77b0a1
// https://fd.lod.bz/rbil/opcode/other.html

// Random 80186 jank: https://news.ycombinator.com/item?id=34334799

#include "../zero/util.h"

#define USE_BITFIELDS 1
#define USE_VECTORS 1

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

template <typename T, typename ... Args>
static inline constexpr void reconstruct_at(T* p, Args&&... args) {
    std::destroy_at(p);
    new (p) T(std::forward<Args>(args)...);
}

// Avoid the values 8, 16, 32, and 64 so that template values
// can be differentiated from bit values.
// THESE VALUES WILL LIKELY CHANGE
enum z86CoreType : size_t {
    z8086 = 0,
    z80186 = 1,
    z80286 = 2,
    zNV30 = 3,
    z80386 = 4,
    z80486
};

// V20 series:
// TEST1, CLR1, SET1, NOT1, ADD4S, SUB4S, CMP4S, ROL4, ROR4, EXT, INS, REPC, REPNC, FPO2
// BRKEM

// Feature Flags
enum z86FeatureTier {
    FEATURES_8086, // PUSH CS, No #UD, unmasked shifts
    FEATURES_80186, // #UD, PUSHA, POPA, BOUND, (PUSH Is), (PUSH Ib), (IMUL Rv, Mv, Is), (IMUL Rb, Mb, Ib), INS, OUTS, ENTER, LEAVE
    FEATURES_80286, // Protected mode, ARPL, 2 byte opcodes
};

static inline constexpr uint64_t QUIRK_FLAGS_A_START = __COUNTER__ + 1;

// I got tired of rewriting the flag shifts
#define QUIRK_FLAG(...) __VA_ARGS__ = 1ull << (__COUNTER__ - QUIRK_FLAGS_A_START)

// QUIRK DEFS
enum z86FeatureFlagsA : uint64_t {
    QUIRK_FLAG(FLAG_PUSH_CS),               // 8086
    QUIRK_FLAG(FLAG_SAL_IS_SETMO),          // 8086
    QUIRK_FLAG(FLAG_REP_INVERT_MUL),        // 8086
    QUIRK_FLAG(FLAG_REP_INVERT_IDIV),       // 8086, 80186
    QUIRK_FLAG(FLAG_FAULTS_ARE_TRAPS),      // 8086
    QUIRK_FLAG(FLAG_NO_UD),                 // 8086
    QUIRK_FLAG(FLAG_SINGLE_MEM_WRAPS),      // 8086, 80186
    QUIRK_FLAG(FLAG_UNMASK_SHIFTS),         // 8086, 80186, v20
    QUIRK_FLAG(FLAG_OLD_PUSH_SP),           // 8086, 80186, v20
    QUIRK_FLAG(FLAG_OLD_RESET_PC),          // 8086, 80186, v20
    QUIRK_FLAG(FLAG_OLD_AAA),               // 8086, 80186 (v20 unknown)
    QUIRK_FLAG(FLAG_WRAP_SEGMENT_MODRM),    // 8086, 80186
    QUIRK_FLAG(FLAG_AAM_NO_DE),             // 80186
    QUIRK_FLAG(FLAG_UNMASK_ENTER),          // 80186, v20
    QUIRK_FLAG(FLAG_REP_BOUND),             // 80186
    QUIRK_FLAG(FLAG_REP_MUL_MISSTORE),      // 80186
    QUIRK_FLAG(FLAG_PROTECTED_MODE),        // 80286+
    QUIRK_FLAG(FLAG_PAGING),                // 80386+
    QUIRK_FLAG(FLAG_LONG_MODE),
    QUIRK_FLAG(FLAG_HAS_TEST_REGS),         // 80386, 80486
    QUIRK_FLAG(FLAG_OPCODES_80186),
    QUIRK_FLAG(FLAG_OPCODES_80286),
    QUIRK_FLAG(FLAG_OPCODES_V20),
    QUIRK_FLAG(FLAG_OPCODES_80386),
    QUIRK_FLAG(FLAG_OPCODES_80486),
    QUIRK_FLAG(FLAG_OPCODES_P5),
    QUIRK_FLAG(FLAG_OPCODES_P6),
    QUIRK_FLAG(FLAG_HAS_CPUID),
    QUIRK_FLAG(FLAG_HAS_LONG_NOP),
    QUIRK_FLAG(FLAG_CPUID_X87),
    QUIRK_FLAG(FLAG_CPUID_CMOV),
    QUIRK_FLAG(FLAG_CPUID_MMX),
    QUIRK_FLAG(FLAG_CPUID_3DNOW),
    QUIRK_FLAG(FLAG_CPUID_SSE),
    QUIRK_FLAG(FLAG_CPUID_SSE2),
    QUIRK_FLAG(FLAG_CPUID_SSE3),
    QUIRK_FLAG(FLAG_CPUID_SSSE3),
    QUIRK_FLAG(FLAG_CPUID_SSE41),
    QUIRK_FLAG(FLAG_CPUID_SSE42),
    QUIRK_FLAG(FLAG_CPUID_SSE4A),
    QUIRK_FLAG(FLAG_CPUID_AVX),
    QUIRK_FLAG(FLAG_CPUID_AVX2),
    QUIRK_FLAG(FLAG_CPUID_XOP),
    QUIRK_FLAG(FLAG_CPUID_FMA3),
    QUIRK_FLAG(FLAG_CPUID_POPCNT),
    QUIRK_FLAG(FLAG_CPUID_BMI1),
    QUIRK_FLAG(FLAG_CPUID_BMI2),
};

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
    inline void regcall write(size_t offset, const T& value) {
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

    inline void* read_movsb(void* dst, size_t src, size_t length) const {
        return rep_movsb(dst, &this->raw[src], length);
    }

    inline size_t write(size_t dst, const void* src, size_t length) {
        if (dst < bytes) {
            length = (std::min)(bytes - dst, length);
            memcpy(&this->raw[dst], src, length);
            return length;
        }
        return 0;
    }

    inline const void* write_movsb(size_t dst, const void* src, size_t length) {
        return rep_movsbS(&this->raw[dst], src, length);
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

struct MMXREG {
    union {
        vec<float, 2> f32;
        double f64;
        vec<uint8_t, 8> byte;
        vec<int8_t, 8> sbyte;
        vec<uint16_t, 4> word;
        vec<int16_t, 4> sword;
        vec<uint32_t, 2> dword;
        vec<int32_t, 2> sdword;
        vec<uint64_t, 1> qword;
        vec<int64_t, 1> sqword;
    };

    template <typename T>
    static inline constexpr size_t vec_size() {
        return 8 / sizeof(T);
    }
};

template <typename T>
using MMXT = std::conditional_t<std::is_same_v<T, void>, MMXREG,
             std::conditional_t<MMXREG::vec_size<T>() != 0,
                                vec<T, MMXREG::vec_size<T>()>,
                                void>>;

struct FPUREG {
    union {
        long double st;
        MMXREG mmx;
    };
};

struct SSEREG {
    union {
        vec<float, 4> f32;
        vec<double, 2> f64;
        vec<uint8_t, 16> byte;
        vec<int8_t, 16> sbyte;
        vec<uint16_t, 8> word;
        vec<int16_t, 8> sword;
        vec<uint32_t, 4> dword;
        vec<int32_t, 4> sdword;
        vec<uint64_t, 2> qword;
        vec<int64_t, 2> sqword;
        uint128_t oword;
        int128_t soword;
    };

    template <typename T>
    static inline constexpr size_t vec_size() {
        return 16 / sizeof(T);
    }
};

template <typename T>
using SSET = std::conditional_t<std::is_same_v<T, void>, SSEREG,
             std::conditional_t<SSEREG::vec_size<T>() != 0,
                 std::conditional_t<SSEREG::vec_size<T>() != 1,
                                        vec<T, SSEREG::vec_size<T>()>,
                                        T>,
                                    void>>;

struct AVXREG {
    union {
        vec<float, 8> f32;
        vec<double, 4> f64;
        vec<uint8_t, 32> byte;
        vec<int8_t, 32> sbyte;
        vec<uint16_t, 16> word;
        vec<int16_t, 16> sword;
        vec<uint32_t, 8> dword;
        vec<int32_t, 8> sdword;
        vec<uint64_t, 4> qword;
        vec<int64_t, 4> sqword;
        vec<uint128_t, 2> oword;
        vec<int128_t, 2> soword;
    };

    template <typename T>
    static inline constexpr size_t vec_size() {
        return 32 / sizeof(T);
    }
};

template <typename T>
using AVXT = std::conditional_t<std::is_same_v<T, void>, AVXREG,
             std::conditional_t<AVXREG::vec_size<T>() != 0,
                 std::conditional_t<AVXREG::vec_size<T>() != 1,
                                        vec<T, AVXREG::vec_size<T>()>,
                                        T>,
                                    void>>;

struct AVX512REG {
    union {
        vec<float, 16> f32;
        vec<double, 8> f64;
        vec<uint8_t, 64> byte;
        vec<int8_t, 64> sbyte;
        vec<uint16_t, 32> word;
        vec<int16_t, 32> sword;
        vec<uint32_t, 16> dword;
        vec<int32_t, 16> sdword;
        vec<uint64_t, 8> qword;
        vec<int64_t, 8> sqword;
        vec<uint128_t, 4> oword;
        vec<int128_t, 4> soword;
    };

    template <typename T>
    static inline constexpr size_t vec_size() {
        return 64 / sizeof(T);
    }
};

template <typename T>
using AVX512T = std::conditional_t<std::is_same_v<T, void>, AVX512REG,
                std::conditional_t<AVX512REG::vec_size<T>() != 0,
                    std::conditional_t<AVX512REG::vec_size<T>() != 1,
                                           vec<T, AVX512REG::vec_size<T>()>,
                                           T>,
                                       void>>;

template <size_t bits>
struct SEG_DESCRIPTOR {
    union {
        uint16_t limit_low;
        uint16_t gate_ip;
    };
    union {
        uint16_t base_low;
        uint16_t gate_cs;
    };
    union {
        uint8_t base_middle;
        uint8_t gate_params : 5;
    };
    union {
        uint8_t flags1;
        struct { // Generic flags
            uint8_t type : 5;
            uint8_t dpl : 2;
            uint8_t present : 1;
        };
        struct {
            uint8_t accessed : 1;
            uint8_t readable : 1;
            uint8_t conforming : 1;
        } code;
        struct {
            uint8_t accessed : 1;
            uint8_t writable : 1;
            uint8_t expand_down : 1;
        } data;
        struct {
            uint8_t type : 4;
        } system;
    };

    inline constexpr bool is_system() const {
        return this->type < 0b10000;
    }

    inline constexpr bool is_code() const {
        return this->type >= 0b11000;
    }

    inline constexpr bool is_data() const {
        return this->type - 0b10000u < 0b1000u;
    }

    inline constexpr bool is_gate() const {
        return (this->type & 0b10111) - 4 < 3;
    }
};

template <>
struct SEG_DESCRIPTOR<16> : SEG_DESCRIPTOR<0> {
    uint16_t must_be_zero;

    inline uint32_t base() const {
        return *(uint32_t*)&this->base_low & 0xFFFFFF;
    }

    inline constexpr uint16_t limit() const {
        return this->limit_low;
    }

    inline constexpr uint16_t ip() const {
        return this->gate_ip;
    }
};

template <>
struct SEG_DESCRIPTOR<32> : SEG_DESCRIPTOR<0> {
    union {
        struct {
            union {
                uint8_t flags2;
                struct {
                    uint8_t limit_high : 4;
                    uint8_t system_use : 1;
                    uint8_t long_mode : 1;
                    uint8_t big : 1;
                    uint8_t granularity : 1;
                };
            };
            uint8_t base_high;
        };
        uint16_t gate_ip_high;
    };

    inline uint32_t base() const {
#if USE_VECTORS
        const vec<uint8_t, 8> seg = *(vec<uint8_t, 8>*)this;
        const vec<uint8_t, 8> zero = {};
        vec<uint8_t, 8> ret = shufflevec(seg, zero, 2, 3, 4, 7, 8, 8, 8, 8);
        return std::bit_cast<vec<uint32_t, 2>>(ret)[0];
#else
        return (*(uint32_t*)&this->base_low & 0xFFFFFF) | (uint32_t)this->base_high << 24;
#endif
    }

    inline constexpr uint32_t limit() const {
        uint32_t limit = (uint32_t)this->limit_low | (uint32_t)this->limit_high << 16;
        if (this->granularity) {
            limit <<= 12;
        }
        return limit;
    }

    inline constexpr uint32_t ip() const {
        return (uint32_t)this->gate_ip | (uint32_t)this->gate_ip_high << 16;
    }
};

template <>
struct SEG_DESCRIPTOR<64> : SEG_DESCRIPTOR<32> {
    union {
        uint32_t base_upper;
        uint32_t gate_ip_upper;
    };
    uint32_t must_be_zero;

    inline uint64_t base() const {
#if USE_VECTORS
        if constexpr (sizeof(void*) == sizeof(uint64_t)) {
            const vec<uint8_t, 16> seg = *(vec<uint8_t, 16>*)this;
            const vec<uint8_t, 16> zero = {};
            vec<uint8_t, 16> ret = shufflevec(seg, zero, 2, 3, 4, 7, 8, 9, 10, 11, 16, 16, 16, 16, 16, 16, 16, 16);
            return std::bit_cast<vec<uint64_t, 2>>(ret)[0];
        }
        else {
            const vec<uint8_t, 8> seg = *(vec<uint8_t, 8>*)this;
            const vec<uint8_t, 8> zero = {};
            vec<uint8_t, 8> ret = shufflevec(seg, zero, 2, 3, 4, 7, 8, 8, 8, 8);
            return (uint64_t)std::bit_cast<vec<uint32_t, 2>>(ret)[0] | (uint64_t)this->base_upper << 32;
        }
#else
        uint64_t ret = *(uint32_t*)&seg->base_low & 0xFFFFFF | (uint32_t)seg->base_high << 24;
        return ret | (uint64_t)seg->base_upper << 32;
#endif
    }

    inline constexpr uint64_t ip() const {
        uint64_t ret = (uint32_t)this->gate_ip | (uint32_t)this->gate_ip_high << 16;
        return ret | (uint64_t)this->gate_ip_upper << 32;
    }
};

#define DS DS_SEG

enum REG_INDEX : uint8_t {
    ZMM0  =  0, YMM0  =  0, XMM0  =  0, RAX  =  0, EAX  =  0, AX   =  0, AL   =  0, ES  =  0, CR0 = 0, K0 = 0, ST0 = 0, MM0 = 0, DR0 = 0,
    ZMM1  =  1, YMM1  =  1, XMM1  =  1, RCX  =  1, ECX  =  1, CX   =  1, CL   =  1, CS  =  1, CR1 = 1, K1 = 1, ST1 = 1, MM1 = 1, DR1 = 1,
    ZMM2  =  2, YMM2  =  2, XMM2  =  2, RDX  =  2, EDX  =  2, DX   =  2, DL   =  2, SS  =  2, CR2 = 2, K2 = 2, ST2 = 2, MM2 = 2, DR2 = 2,
    ZMM3  =  3, YMM3  =  3, XMM3  =  3, RBX  =  3, EBX  =  3, BX   =  3, BL   =  3, DS  =  3, CR3 = 3, K3 = 3, ST3 = 3, MM3 = 3, DR3 = 3,
    ZMM4  =  4, YMM4  =  4, XMM4  =  4, RSP  =  4, ESP  =  4, SP   =  4, AH   =  4, FS  =  4, CR4 = 4, K4 = 4, ST4 = 4, MM4 = 4, DR4 = 4,
    ZMM5  =  5, YMM5  =  5, XMM5  =  5, RBP  =  5, EBP  =  5, BP   =  5, CH   =  5, GS  =  5, CR5 = 5, K5 = 5, ST5 = 5, MM5 = 5, DR5 = 5,
    ZMM6  =  6, YMM6  =  6, XMM6  =  6, RSI  =  6, ESI  =  6, SI   =  6, DH   =  6, DS3 =  6, CR6 = 6, K6 = 6, ST6 = 6, MM6 = 6, DR6 = 6,
    ZMM7  =  7, YMM7  =  7, XMM7  =  7, RDI  =  7, EDI  =  7, DI   =  7, BH   =  7, DS2 =  7, CR7 = 7, K7 = 7, ST7 = 7, MM7 = 7, DR7 = 7,
    ZMM8  =  8, YMM8  =  8, XMM8  =  8, R8   =  8, R8D  =  8, R8W  =  8, R8B  =  8, LDT =  8, CR8 = 8,
    ZMM9  =  9, YMM9  =  9, XMM9  =  9, R9   =  9, R9D  =  9, R9W  =  9, R9B  =  9, TSS =  9,
    ZMM10 = 10, YMM10 = 10, XMM10 = 10, R10  = 10, R10D = 10, R10W = 10, R10B = 10, GDT = 10,
    ZMM11 = 11, YMM11 = 11, XMM11 = 11, R11  = 11, R11D = 11, R11W = 11, R11B = 11, IDT = 11,
    ZMM12 = 12, YMM12 = 12, XMM12 = 12, R12  = 12, R12D = 12, R12W = 12, R12B = 12,
    ZMM13 = 13, YMM13 = 13, XMM13 = 13, R13  = 13, R13D = 13, R13W = 13, R13B = 13,
    ZMM14 = 14, YMM14 = 14, XMM14 = 14, R14  = 14, R14D = 14, R14W = 14, R14B = 14,
    ZMM15 = 15, YMM15 = 15, XMM15 = 15, R15  = 15, R15D = 15, R15W = 15, R15B = 15,
    ZMM16 = 16, YMM16 = 16, XMM16 = 16, R16  = 16, R16D = 16, R16W = 16, R16B = 16,
    ZMM17 = 17, YMM17 = 17, XMM17 = 17, R17  = 17, R17D = 17, R17W = 17, R17B = 17,
    ZMM18 = 18, YMM18 = 18, XMM18 = 18, R18  = 18, R18D = 18, R18W = 18, R18B = 18,
    ZMM19 = 19, YMM19 = 19, XMM19 = 19, R19  = 19, R19D = 19, R19W = 19, R19B = 19,
    ZMM20 = 20, YMM20 = 20, XMM20 = 20, R20  = 20, R20D = 20, R20W = 20, R20B = 20,
    ZMM21 = 21, YMM21 = 21, XMM21 = 21, R21  = 21, R21D = 21, R21W = 21, R21B = 21,
    ZMM22 = 22, YMM22 = 22, XMM22 = 22, R22  = 22, R22D = 22, R22W = 22, R22B = 22,
    ZMM23 = 23, YMM23 = 23, XMM23 = 23, R23  = 23, R23D = 23, R23W = 23, R23B = 23,
    ZMM24 = 24, YMM24 = 24, XMM24 = 24, R24  = 24, R24D = 24, R24W = 24, R24B = 24,
    ZMM25 = 25, YMM25 = 25, XMM25 = 25, R25  = 25, R25D = 25, R25W = 25, R25B = 25,
    ZMM26 = 26, YMM26 = 26, XMM26 = 26, R26  = 26, R26D = 26, R26W = 26, R26B = 26,
    ZMM27 = 27, YMM27 = 27, XMM27 = 27, R27  = 27, R27D = 27, R27W = 27, R27B = 27,
    ZMM28 = 28, YMM28 = 28, XMM28 = 28, R28  = 28, R28D = 28, R28W = 28, R28B = 28,
    ZMM29 = 29, YMM29 = 29, XMM29 = 29, R29  = 29, R29D = 29, R29W = 29, R29B = 29,
    ZMM30 = 30, YMM30 = 30, XMM30 = 30, R30  = 30, R30D = 30, R30W = 30, R30B = 30,
    ZMM31 = 31, YMM31 = 31, XMM31 = 31, R31  = 31, R31D = 31, R31W = 31, R31B = 31
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
    REP_NZ = 0, REP_NE = REP_NZ,
    REP_Z = 1, REP_E = REP_Z,
    REP_NC = 2,
    REP_C = 3
};

enum OPCODE_PREFIX_TYPE : uint8_t {
    OpcodeNoPrefix = 0,
    Opcode66Prefix = 1,
    OpcodeF2Prefix = 2,
    OpcodeF3Prefix = 3
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
    inline constexpr REX(uint8_t raw) : raw(raw) {}

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

template <size_t max_bits>
struct z86BaseGPRs;

// size: 0x10
template <>
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
};

// size: 0x20
template <>
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
};

// size: 0x40
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
};

template <bool has_x87>
struct z86BaseFPU {
};

// size: 0x80
template <>
struct z86BaseFPU<true> {
    union {
        alignas(16) long double st[8];
        alignas(16) MMXREG mm[8];
        struct {
            union {
                alignas(16) long double st0;
                alignas(16) MMXREG mm0;
            };
            union {
                alignas(16) long double st1;
                alignas(16) MMXREG mm1;
            };
            union {
                alignas(16) long double st2;
                alignas(16) MMXREG mm2;
            };
            union {
                alignas(16) long double st3;
                alignas(16) MMXREG mm3;
            };
            union {
                alignas(16) long double st4;
                alignas(16) MMXREG mm4;
            };
            union {
                alignas(16) long double st5;
                alignas(16) MMXREG mm5;
            };
            union {
                alignas(16) long double st6;
                alignas(16) MMXREG mm6;
            };
            union {
                alignas(16) long double st7;
                alignas(16) MMXREG mm7;
            };
        };
    };
};

template <size_t max_bits, size_t reg_count>
struct z86BaseSSE;

template <>
struct z86BaseSSE<0, 0> {
};

template <>
struct z86BaseSSE<128, 8> {
    union {
        SSEREG xmm[8];
        SSEREG ymm[8];
        SSEREG zmm[8];
        struct {
            union {
                SSEREG xmm0;
                SSEREG ymm0;
                SSEREG zmm0;
            };
            union {
                SSEREG xmm1;
                SSEREG ymm1;
                SSEREG zmm1;
            };
            union {
                SSEREG xmm2;
                SSEREG ymm2;
                SSEREG zmm2;
            };
            union {
                SSEREG xmm3;
                SSEREG ymm3;
                SSEREG zmm3;
            };
            union {
                SSEREG xmm4;
                SSEREG ymm4;
                SSEREG zmm4;
            };
            union {
                SSEREG xmm5;
                SSEREG ymm5;
                SSEREG zmm5;
            };
            union {
                SSEREG xmm6;
                SSEREG ymm6;
                SSEREG zmm6;
            };
            union {
                SSEREG xmm7;
                SSEREG ymm7;
                SSEREG zmm7;
            };
        };
    };
};

template <>
struct z86BaseSSE<128, 16> {
    union {
        SSEREG xmm[16];
        SSEREG ymm[16];
        SSEREG zmm[16];
        struct {
            union {
                SSEREG xmm0;
                SSEREG ymm0;
                SSEREG zmm0;
            };
            union {
                SSEREG xmm1;
                SSEREG ymm1;
                SSEREG zmm1;
            };
            union {
                SSEREG xmm2;
                SSEREG ymm2;
                SSEREG zmm2;
            };
            union {
                SSEREG xmm3;
                SSEREG ymm3;
                SSEREG zmm3;
            };
            union {
                SSEREG xmm4;
                SSEREG ymm4;
                SSEREG zmm4;
            };
            union {
                SSEREG xmm5;
                SSEREG ymm5;
                SSEREG zmm5;
            };
            union {
                SSEREG xmm6;
                SSEREG ymm6;
                SSEREG zmm6;
            };
            union {
                SSEREG xmm7;
                SSEREG ymm7;
                SSEREG zmm7;
            };
            union {
                SSEREG xmm8;
                SSEREG ymm8;
                SSEREG zmm8;
            };
            union {
                SSEREG xmm9;
                SSEREG ymm9;
                SSEREG zmm9;
            };
            union {
                SSEREG xmm10;
                SSEREG ymm10;
                SSEREG zmm10;
            };
            union {
                SSEREG xmm11;
                SSEREG ymm11;
                SSEREG zmm11;
            };
            union {
                SSEREG xmm12;
                SSEREG ymm12;
                SSEREG zmm12;
            };
            union {
                SSEREG xmm13;
                SSEREG ymm13;
                SSEREG zmm13;
            };
            union {
                SSEREG xmm14;
                SSEREG ymm14;
                SSEREG zmm14;
            };
            union {
                SSEREG xmm15;
                SSEREG ymm15;
                SSEREG zmm15;
            };
        };
    };
};

template <>
struct z86BaseSSE<256, 16> {
    union {
        SSEREG xmm[16];
        AVXREG ymm[16];
        AVXREG zmm[16];
        struct {
            union {
                SSEREG xmm0;
                AVXREG ymm0;
                AVXREG zmm0;
            };
            union {
                SSEREG xmm1;
                AVXREG ymm1;
                AVXREG zmm1;
            };
            union {
                SSEREG xmm2;
                AVXREG ymm2;
                AVXREG zmm2;
            };
            union {
                SSEREG xmm3;
                AVXREG ymm3;
                AVXREG zmm3;
            };
            union {
                SSEREG xmm4;
                AVXREG ymm4;
                AVXREG zmm4;
            };
            union {
                SSEREG xmm5;
                AVXREG ymm5;
                AVXREG zmm5;
            };
            union {
                SSEREG xmm6;
                AVXREG ymm6;
                AVXREG zmm6;
            };
            union {
                SSEREG xmm7;
                AVXREG ymm7;
                AVXREG zmm7;
            };
            union {
                SSEREG xmm8;
                AVXREG ymm8;
                AVXREG zmm8;
            };
            union {
                SSEREG xmm9;
                AVXREG ymm9;
                AVXREG zmm9;
            };
            union {
                SSEREG xmm10;
                AVXREG ymm10;
                AVXREG zmm10;
            };
            union {
                SSEREG xmm11;
                AVXREG ymm11;
                AVXREG zmm11;
            };
            union {
                SSEREG xmm12;
                AVXREG ymm12;
                AVXREG zmm12;
            };
            union {
                SSEREG xmm13;
                AVXREG ymm13;
                AVXREG zmm13;
            };
            union {
                SSEREG xmm14;
                AVXREG ymm14;
                AVXREG zmm14;
            };
            union {
                SSEREG xmm15;
                AVXREG ymm15;
                AVXREG zmm15;
            };
        };
    };
};

template <>
struct z86BaseSSE<512, 32> {
    union {
        SSEREG xmm[32];
        AVXREG ymm[32];
        AVX512REG zmm[32];
        struct {
            union {
                SSEREG xmm0;
                AVXREG ymm0;
                AVX512REG zmm0;
            };
            union {
                SSEREG xmm1;
                AVXREG ymm1;
                AVX512REG zmm1;
            };
            union {
                SSEREG xmm2;
                AVXREG ymm2;
                AVX512REG zmm2;
            };
            union {
                SSEREG xmm3;
                AVXREG ymm3;
                AVX512REG zmm3;
            };
            union {
                SSEREG xmm4;
                AVXREG ymm4;
                AVX512REG zmm4;
            };
            union {
                SSEREG xmm5;
                AVXREG ymm5;
                AVX512REG zmm5;
            };
            union {
                SSEREG xmm6;
                AVXREG ymm6;
                AVX512REG zmm6;
            };
            union {
                SSEREG xmm7;
                AVXREG ymm7;
                AVX512REG zmm7;
            };
            union {
                SSEREG xmm8;
                AVXREG ymm8;
                AVX512REG zmm8;
            };
            union {
                SSEREG xmm9;
                AVXREG ymm9;
                AVX512REG zmm9;
            };
            union {
                SSEREG xmm10;
                AVXREG ymm10;
                AVX512REG zmm10;
            };
            union {
                SSEREG xmm11;
                AVXREG ymm11;
                AVX512REG zmm11;
            };
            union {
                SSEREG xmm12;
                AVXREG ymm12;
                AVX512REG zmm12;
            };
            union {
                SSEREG xmm13;
                AVXREG ymm13;
                AVX512REG zmm13;
            };
            union {
                SSEREG xmm14;
                AVXREG ymm14;
                AVX512REG zmm14;
            };
            union {
                SSEREG xmm15;
                AVXREG ymm15;
                AVX512REG zmm15;
            };
            union {
                SSEREG xmm16;
                AVXREG ymm16;
                AVX512REG zmm16;
            };
            union {
                SSEREG xmm17;
                AVXREG ymm17;
                AVX512REG zmm17;
            };
            union {
                SSEREG xmm18;
                AVXREG ymm18;
                AVX512REG zmm18;
            };
            union {
                SSEREG xmm19;
                AVXREG ymm19;
                AVX512REG zmm19;
            };
            union {
                SSEREG xmm20;
                AVXREG ymm20;
                AVX512REG zmm20;
            };
            union {
                SSEREG xmm21;
                AVXREG ymm21;
                AVX512REG zmm21;
            };
            union {
                SSEREG xmm22;
                AVXREG ymm22;
                AVX512REG zmm22;
            };
            union {
                SSEREG xmm23;
                AVXREG ymm23;
                AVX512REG zmm23;
            };
            union {
                SSEREG xmm24;
                AVXREG ymm24;
                AVX512REG zmm24;
            };
            union {
                SSEREG xmm25;
                AVXREG ymm25;
                AVX512REG zmm25;
            };
            union {
                SSEREG xmm26;
                AVXREG ymm26;
                AVX512REG zmm26;
            };
            union {
                SSEREG xmm27;
                AVXREG ymm27;
                AVX512REG zmm27;
            };
            union {
                SSEREG xmm28;
                AVXREG ymm28;
                AVX512REG zmm28;
            };
            union {
                SSEREG xmm29;
                AVXREG ymm29;
                AVX512REG zmm29;
            };
            union {
                SSEREG xmm30;
                AVXREG ymm30;
                AVX512REG zmm30;
            };
            union {
                SSEREG xmm31;
                AVXREG ymm31;
                AVX512REG zmm31;
            };
        };
    };
    union {
        uint64_t k[8];
        struct {
            uint64_t k0;
            uint64_t k1;
            uint64_t k2;
            uint64_t k3;
            uint64_t k4;
            uint64_t k5;
            uint64_t k6;
            uint64_t k7;
        };
    };
};

template <size_t max_bits, bool has_x87, bool has_sse>
struct z86BaseFPUControl {};

// size: 0x4
template <size_t max_bits>
struct z86BaseFPUControl<max_bits, false, true> {
    MXCSR mxcsr; // 0x0
    // 0x4
};

// size: 0x14
template <>
struct z86BaseFPUControl<16, true, false> {
    FCW fcw; // 0x0
    FSW fsw; // 0x2
    uint16_t ftw; // 0x4
    uint16_t fop; // 0x6
    uint32_t fip; // 0x8
    uint32_t fdp; // 0xC
    uint32_t stack_top; // 0x10
    // 0x14
};

// size: 0x18
template <>
struct z86BaseFPUControl<16, true, true> {
    FCW fcw; // 0x0
    FSW fsw; // 0x2
    uint16_t ftw; // 0x4
    uint16_t fop; // 0x6
    uint32_t fip; // 0x8
    uint32_t fdp; // 0xC
    uint32_t stack_top; // 0x10
    MXCSR mxcsr; // 0x14
    // 0x18
};

// size: 0x18
template <>
struct z86BaseFPUControl<32, true, false> {
    FCW fcw; // 0x0
    FSW fsw; // 0x2
    uint16_t ftw; // 0x4
    uint16_t fop; // 0x6
    uint32_t fip; // 0x8
    uint32_t fdp; // 0xC
    uint16_t fcs; // 0x10
    uint16_t fds; // 0x12
    uint32_t stack_top; // 0x14
    // 0x18
};

// size: 0x1C
template <>
struct z86BaseFPUControl<32, true, true> {
    FCW fcw; // 0x0
    FSW fsw; // 0x2
    uint16_t ftw; // 0x4
    uint16_t fop; // 0x6
    uint32_t fip; // 0x8
    uint32_t fdp; // 0xC
    uint16_t fcs; // 0x10
    uint16_t fds; // 0x12
    uint32_t stack_top; // 0x14
    MXCSR mxcsr; // 0x18
    // 0x1C
};

// size: 0x20
template <>
struct z86BaseFPUControl<64, true, false> {
    FCW fcw; // 0x0
    FSW fsw; // 0x2
    uint16_t ftw; // 0x4
    uint16_t fop; // 0x6
    uint64_t fip; // 0x8
    uint64_t fdp; // 0x10
    uint16_t fcs; // 0x18
    uint16_t fds; // 0x1A
    uint32_t stack_top; // 0x1C
    // 0x20
};

// size: 0x28 (because of alignment)
template <>
struct z86BaseFPUControl<64, true, true> {
    FCW fcw; // 0x0
    FSW fsw; // 0x2
    uint16_t ftw; // 0x4
    uint16_t fop; // 0x6
    uint64_t fip; // 0x8
    uint64_t fdp; // 0x10
    uint16_t fcs; // 0x18
    uint16_t fds; // 0x1A
    uint32_t stack_top; // 0x1C
    MXCSR mxcsr; // 0x20
    // 0x24
};

struct z86DescriptorCache80286 {
    uint16_t base_low;
    uint8_t base_high;
    uint8_t access_rights;
    uint16_t limit;
};

template <size_t max_bits>
struct z86DescriptorCacheBase;
// Seg Defaults:
// - Base: 0
// - Limit: 0xFFFF
// - Access: 0x93
// Table Defaults:
// - Base: 0
// - Limit: 0xFFFF
// - Access: 0x82

// Size: 0x8
template <>
struct z86DescriptorCacheBase<16> {
    using BT = uint32_t; // Base Type
    using LT = uint16_t; // Limit type

    const uint32_t base = 0; // 0x0
    const uint16_t limit = 0; // 0x4
    const uint8_t type = 0; // 0x6
    const uint8_t privilege = 0; // 0x7
    // 0x8

    inline constexpr z86DescriptorCacheBase() = default;
    inline constexpr z86DescriptorCacheBase(uint16_t limit, uint32_t base) : base(base), limit(limit), type(0), privilege(0) {}
    inline constexpr z86DescriptorCacheBase(uint16_t limit, uint32_t base, uint8_t type, uint8_t privilege) : base(base), limit(limit), type(type), privilege(privilege) {}

    inline constexpr z86DescriptorCacheBase(SEG_DESCRIPTOR<16>* descriptor)
        : base(descriptor->base()), limit(descriptor->limit()), type(0), privilege(0)
    {}
};

// Size: 0xC
template <>
struct z86DescriptorCacheBase<32> {
    using BT = uint32_t; // Base Type
    using LT = uint32_t; // Limit type

    const uint32_t base = 0; // 0x0
    const uint32_t limit = 0; // 0x4
    const uint8_t type = 0; // 0x8
    const uint8_t privilege = 0; // 0x9
    // 0xA

    inline constexpr z86DescriptorCacheBase() = default;
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint32_t base) : base(base), limit(limit), type(0), privilege(0) {}
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint32_t base, uint8_t type, uint8_t privilege) : base(base), limit(limit), type(type), privilege(privilege) {}

    inline constexpr z86DescriptorCacheBase(SEG_DESCRIPTOR<32>* descriptor)
        : base(descriptor->base()), limit(descriptor->limit()), type(0), privilege(0)
    {}
};

// Size: 0x10
template <>
struct z86DescriptorCacheBase<64> {
    using BT = uint64_t; // Base Type
    using LT = uint32_t; // Limit type

    const uint64_t base = 0; // 0x0
    const uint32_t limit = 0; // 0x8
    const uint8_t type = 0; // 0xC
    const uint8_t privilege = 0; // 0xD
    // 0xE

    inline constexpr z86DescriptorCacheBase() = default;
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint64_t base) : base(base), limit(limit), type(0), privilege(0) {}
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint64_t base, uint8_t type, uint8_t privilege) : base(base), limit(limit), type(type), privilege(privilege) {}
    
    inline constexpr z86DescriptorCacheBase(SEG_DESCRIPTOR<64>* descriptor)
        : base(descriptor->base()), limit(descriptor->limit()), type(0), privilege(0)
    {}
};

template <size_t max_bits>
struct z86DescriptorCache : z86DescriptorCacheBase<max_bits> {
    using BT = z86DescriptorCacheBase<max_bits>::BT;
    using LT = z86DescriptorCacheBase<max_bits>::LT;

    inline constexpr z86DescriptorCache() = default;
    inline constexpr z86DescriptorCache(LT limit, BT base) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(limit, base) {}
    inline constexpr z86DescriptorCache(LT limit, BT base, uint8_t type, uint8_t privilege) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(limit, base, type, privilege) {}
    inline constexpr z86DescriptorCache(SEG_DESCRIPTOR<max_bits>* descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}

    /*
    inline constexpr void load_table(LT limit, BT base) {
        this->base = base;
        this->limit = limit;
    }

    inline constexpr void load_descriptor(SEG_DESCRIPTOR<max_bits>* descriptor) {
        this->base = descriptor->base();
        this->limit = descriptor->limit();
    }
    */

    // Invoked on GDT/LDT
    inline constexpr SEG_DESCRIPTOR<max_bits>* load_selector(uint16_t selector) const;
};

struct z86Loadall2Frame {
    uint16_t x0;
    uint16_t x1;
    uint16_t x2;
    uint16_t msw;
    uint16_t x3;
    uint16_t x4;
    uint16_t x5;
    uint16_t x6;
    uint16_t x7;
    uint16_t x8;
    uint16_t x9;
    uint16_t tr;
    uint16_t flags;
    uint16_t ip;
    uint16_t ldtr;
    uint16_t ds;
    uint16_t ss;
    uint16_t cs;
    uint16_t es;
    uint16_t di;
    uint16_t si;
    uint16_t bp;
    uint16_t sp;
    uint16_t bx;
    uint16_t dx;
    uint16_t cx;
    uint16_t ax;
    z86DescriptorCache80286 es_descriptor;
    z86DescriptorCache80286 cs_descriptor;
    z86DescriptorCache80286 ss_descriptor;
    z86DescriptorCache80286 ds_descriptor;
    z86DescriptorCache80286 gdt_descriptor;
    z86DescriptorCache80286 ldt_descriptor;
    z86DescriptorCache80286 idt_descriptor;
    z86DescriptorCache80286 tss_descriptor;
};

template <size_t max_bits, bool protected_mode>
struct z86BaseControlBase;

// size: 0x16
template <size_t max_bits>
struct z86BaseControlBase<max_bits, false> {

    static inline constexpr z86DescriptorCache<max_bits> descriptors[12] = {};

    union {
        uint16_t seg[8];
        struct {
            uint16_t es;
            uint16_t cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
            uint16_t ds3;
            uint16_t ds2;
        };
    };
    static inline constexpr uint16_t ldtr = 0;
    static inline constexpr uint16_t tr = 0;

    static inline constexpr uint8_t cpl = 0;
};

template <>
struct z86BaseControlBase<16, true> {
    union {
        z86DescriptorCache<16> descriptors[12] = {};
        struct {
            z86DescriptorCache<16> es_descriptor;
            z86DescriptorCache<16> cs_descriptor;
            z86DescriptorCache<16> ss_descriptor;
            z86DescriptorCache<16> ds_descriptor;
            z86DescriptorCache<16> fs_descriptor;
            z86DescriptorCache<16> gs_descriptor;
            z86DescriptorCache<16> ds3_descriptor;
            z86DescriptorCache<16> ds2_descriptor;
            z86DescriptorCache<16> ldt_descriptor;
            z86DescriptorCache<16> tss_descriptor;
            z86DescriptorCache<16> gdt_descriptor;
            z86DescriptorCache<16> idt_descriptor;
        };
    };
    union {
        uint16_t msw;
        uint16_t cr0;
        union {
            uint8_t protected_mode : 1;
        };
    };
    union {
        uint16_t seg[10];
        struct {
            uint16_t es;
            uint16_t cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
            uint16_t ds3;
            uint16_t ds2;
            uint16_t ldtr;
            uint16_t tr;
        };
    };
    uint8_t cpl;
};

template <>
struct z86BaseControlBase<32, true> {
    union {
        z86DescriptorCache<32> descriptors[12] = {};
        struct {
            z86DescriptorCache<32> es_descriptor;
            z86DescriptorCache<32> cs_descriptor;
            z86DescriptorCache<32> ss_descriptor;
            z86DescriptorCache<32> ds_descriptor;
            z86DescriptorCache<32> fs_descriptor;
            z86DescriptorCache<32> gs_descriptor;
            z86DescriptorCache<32> ds3_descriptor;
            z86DescriptorCache<32> ds2_descriptor;
            z86DescriptorCache<32> ldt_descriptor;
            z86DescriptorCache<32> tss_descriptor;
            z86DescriptorCache<32> gdt_descriptor;
            z86DescriptorCache<32> idt_descriptor;
        };
    };
    union {
        uint32_t cr[8];
        struct {
            union {
                uint16_t msw;
                uint32_t cr0;
                union {
                    uint8_t protected_mode : 1;
                };
            };
            uint32_t cr1;
            uint32_t cr2;
            uint32_t cr3;
            uint32_t cr4;
            uint32_t cr5;
            uint32_t cr6;
            uint32_t cr7;
        };
    };
    union {
        uint16_t seg[10];
        struct {
            uint16_t es;
            uint16_t cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
            uint16_t ds3;
            uint16_t ds2;
            uint16_t ldtr;
            uint16_t tr;
        };
    };
    uint8_t cpl;
};

template <>
struct z86BaseControlBase<64, true> {
    union {
        z86DescriptorCache<64> descriptors[12] = {};
        struct {
            z86DescriptorCache<64> es_descriptor;
            z86DescriptorCache<64> cs_descriptor;
            z86DescriptorCache<64> ss_descriptor;
            z86DescriptorCache<64> ds_descriptor;
            z86DescriptorCache<64> fs_descriptor;
            z86DescriptorCache<64> gs_descriptor;
            z86DescriptorCache<64> ds3_descriptor;
            z86DescriptorCache<64> ds2_descriptor;
            z86DescriptorCache<64> ldt_descriptor;
            z86DescriptorCache<64> tss_descriptor;
            z86DescriptorCache<64> gdt_descriptor;
            z86DescriptorCache<64> idt_descriptor;
        };
    };
    union {
        uint64_t cr[16];
        struct {
            union {
                uint16_t msw;
                uint64_t cr0;
                union {
                    uint8_t protected_mode : 1;
                };
            };
            uint64_t cr1;
            uint64_t cr2;
            uint64_t cr3;
            uint64_t cr4;
            uint64_t cr5;
            uint64_t cr6;
            uint64_t cr7;
            uint64_t cr8;
        };
    };
    union {
        uint16_t seg[10];
        struct {
            uint16_t es;
            uint16_t cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
            uint16_t ds3;
            uint16_t ds2;
            uint16_t ldtr;
            uint16_t tr;
        };
    };
    uint8_t cpl;
};

template <size_t max_bits, bool use_old_reset, bool has_protected_mode>
struct z86BaseControl;

template <size_t max_bits, bool use_old_reset>
struct z86BaseControl<max_bits, use_old_reset, false> : z86BaseControlBase<max_bits, false> {
    using BT = z86DescriptorCache<max_bits>::BT;
    using LT = z86DescriptorCache<max_bits>::LT;

    inline constexpr const uint16_t& get_seg_impl(uint8_t index) {
        return this->seg[index];
    }

    inline constexpr uint16_t get_control_seg(uint8_t index) const {
        return 0;
    }

    inline constexpr void write_seg_impl(uint8_t index, uint16_t value) {
        this->seg[index] = value;
    }

    inline constexpr void write_control_seg(uint8_t index, uint16_t value) {
    }

    // Assuming a previous memset of full context
    inline constexpr void reset_descriptors() {
        if constexpr (!use_old_reset) {
            this->cs = 0xF000;
        }
        else {
            this->cs = 0xFFFF;
        }
    }

    inline constexpr void load_descriptor_table(uint8_t index, LT limit, BT base) {
    }
    inline constexpr LT get_descriptor_table_limit(uint8_t index) const {
        return ~(LT)0;
    }
    inline constexpr BT get_descriptor_table_base(uint8_t index) const {
        return 0;
    }

    inline constexpr uint8_t current_privilege_level() const {
        return 0;
    }

    inline constexpr uint16_t get_machine_status_word() const {
        return 0;
    }
    inline constexpr void set_machine_status_word(uint16_t msw) {
    }
};

// Various notes about 80286 descriptor caches, LOADALL, etc.:
// https://www.rcollins.org/articles/loadall/
// https://gist.github.com/luelista/557dd8f7f5b28cc1f9c28776c88ec347
// https://www.pcjs.org/documents/manuals/intel/80386/loadall/
// https://www.vogons.org/viewtopic.php?t=65223&start=580
// https://forum.vcfed.org/index.php?threads/i-found-the-saveall-opcode.71519/
// https://rep-lodsb.mataroa.blog/blog/the-286s-internal-registers/
template <size_t max_bits, bool use_old_reset>
struct z86BaseControl<max_bits, use_old_reset, true> : z86BaseControlBase<max_bits, true> {
    using BT = z86DescriptorCache<max_bits>::BT;
    using LT = z86DescriptorCache<max_bits>::LT;

    inline constexpr const uint16_t& get_seg_impl(uint8_t index) {
        return this->seg[index];
    }

    inline constexpr uint16_t get_control_seg(uint8_t index) const {
        return this->seg[LDT + index];
    }

    inline constexpr void write_seg_impl(uint8_t index, uint16_t selector) {
        if (this->protected_mode) {
            //this->descriptors[index].load_descriptor(this->descriptors[GDT + (selector >> 2 & 1)].load_selector(selector));
            auto* new_descriptor = this->descriptors[GDT + (selector >> 2 & 1)].load_selector(selector);

            // CHECK FOR DANG GATES
            
            //std::destroy_at(&this->descriptors[index]);
            //new (&this->descriptors[index]) z86DescriptorCache<max_bits>(new_descriptor);
            reconstruct_at(&this->descriptors[index], new_descriptor);
        } else {
            //this->descriptors[index].base = (size_t)selector << 4;
            reconstruct_at(&this->descriptors[index], this->descriptors[index].limit, (size_t)selector << 4, this->descriptors[index].type, this->descriptors[index].privilege);
        }
        this->seg[index] = selector;
    }

    inline constexpr void write_control_seg(uint8_t index, uint16_t value) {
        this->write_seg_impl(LDT + index, value);
    }

    // Used for control flow specifically
    inline constexpr void write_cs(uint16_t value) {

    }

    // Assuming a previous memset of full context
    inline constexpr void reset_descriptors() {
        if constexpr (!use_old_reset) {
            this->cs = 0xF000;
            //this->cs_descriptor.base = 0xFFFF0000;
            reconstruct_at(&this->cs_descriptor, this->cs_descriptor.limit, 0xFFFF0000, this->cs_descriptor.type, this->cs_descriptor.privilege);
        }
        else {
            this->cs = 0xFFFF;
            //this->cs_descriptor.base = 0xFFFFFFF0;
            reconstruct_at(&this->cs_descriptor, this->cs_descriptor.limit, 0xFFFFFFF0, this->cs_descriptor.type, this->cs_descriptor.privilege);
        }
        for (size_t i = 0; i < 8; ++i) {
            //this->descriptors[i].limit = 0xFFFF;
            //std::destroy_at(&this->descriptors[i]);
            //new (&this->descriptors[i]) z86DescriptorCache<max_bits>((LT)0xFFFF, (BT)0);
            reconstruct_at(&this->descriptors[i], 0xFFFF, this->descriptors[i].base, this->descriptors[i].type, this->descriptors[i].privilege);
        }
        //std::destroy_at(&this->cs_descriptor);
        //new (&this->cs_descriptor) z86DescriptorCache<max_bits>((LT)0xFFFF, (BT)0);
    }

    inline constexpr void load_descriptor_table(uint8_t index, LT limit, BT base) {
        reconstruct_at(&this->descriptors[GDT + index], limit, base, this->descriptors[GDT + index].type, this->descriptors[GDT + index].privilege);
    }

    inline constexpr LT get_descriptor_table_limit(uint8_t index) const {
        return this->descriptors[GDT + index].limit;
    }
    inline constexpr BT get_descriptor_table_base(uint8_t index) const {
        return this->descriptors[GDT + index].base;
    }

    inline constexpr uint8_t current_privilege_level() const {
        return this->cpl;
    }

    inline constexpr auto get_machine_status_word() const {
        return this->cr0;
    }
    inline constexpr void set_machine_status_word(uint16_t msw) {
        // TODO: filter bits
        this->msw = msw;
    }
};

template <size_t max_bits, bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase;

template <bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase<16, use_old_reset, has_protected_mode, has_x87, max_sse_bits, sse_reg_count> :
    z86BaseSSE<max_sse_bits, sse_reg_count>,
    z86BaseFPU<has_x87>,
    z86BaseGPRs<16>,
    z86BaseControl<16, use_old_reset, has_protected_mode>,
    z86BaseFPUControl<16, has_x87, max_sse_bits != 0>
{
    union {
        uint16_t rip;
        uint16_t eip;
        uint16_t ip;
    };

    static constexpr inline int8_t data_size = 1;
    static constexpr inline int8_t addr_size = 1;
    static constexpr inline int8_t stack_size = 1;
    static constexpr inline int8_t default_data_size = 1;
    static constexpr inline int8_t default_addr_size = 1;
    static constexpr inline int8_t mode = 1;
    static constexpr inline uint8_t opcode_prefix = 0;
    static constexpr inline REX rex_bits = {};

    inline constexpr void reset_prefixes() {
    }

    inline constexpr void set_opcode_select(uint8_t type) {
    }

    inline constexpr void data_size_prefix() {
    }

    inline constexpr void addr_size_prefix() {
    }

    inline constexpr uint8_t opcode_select() const {
        return opcode_prefix;
    }

    inline constexpr bool data_size_16() const {
        return true;
    }
    inline constexpr bool data_size_32() const {
        return false;
    }
    inline constexpr bool data_size_64() const {
        return false;
    }
    inline constexpr bool addr_size_16() const {
        return true;
    }
    inline constexpr bool addr_size_32() const {
        return false;
    }
    inline constexpr bool addr_size_64() const {
        return false;
    }
    inline constexpr bool stack_size_16() const {
        return true;
    }
    inline constexpr bool stack_size_32() const {
        return true;
    }
    inline constexpr bool stack_size_64() const {
        return true;
    }
    inline constexpr REX get_rex_bits() const {
        return {};
    }
    inline constexpr void set_rex_bits(uint8_t bits) {
    }
    inline constexpr void reset_rex_bits() {
    }
    inline constexpr bool is_real_mode() const {
        return true;
    }
    inline constexpr bool is_protected_mode() const {
        return false;
    }
    inline constexpr bool is_long_mode() const {
        return false;
    }
};

template <bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase<32, use_old_reset, has_protected_mode, has_x87, max_sse_bits, sse_reg_count> :
    z86BaseSSE<max_sse_bits, sse_reg_count>,
    z86BaseFPU<has_x87>,
    z86BaseGPRs<32>,
    z86BaseControl<32, use_old_reset, has_protected_mode>,
    z86BaseFPUControl<32, has_x87, max_sse_bits != 0>
{
    union {
        uint32_t rip;
        uint32_t eip;
        uint16_t ip;
    };

    int8_t data_size = 1;
    int8_t addr_size = 1;
    int8_t stack_size = 1;
    int8_t default_data_size = 1;
    int8_t default_addr_size = 1;
    int8_t mode = 1;
    uint8_t opcode_prefix = 0;
    static constexpr inline REX rex_bits = {};
    
    inline constexpr void reset_prefixes() {
        this->data_size = this->default_data_size;
        this->addr_size = this->default_addr_size;
        this->opcode_prefix = 0;
    }

    inline constexpr void set_opcode_select(uint8_t type) {
        this->opcode_prefix = type;
    }

    inline constexpr void data_size_prefix() {
        this->data_size = !this->default_data_size;
        this->set_opcode_select(1);
    }

    inline constexpr void addr_size_prefix() {
        this->addr_size = !this->default_addr_size;
    }

    inline constexpr uint8_t opcode_select() const {
        return this->opcode_prefix;
    }

    inline constexpr bool data_size_16() const {
        return this->data_size != 0;
    }
    inline constexpr bool data_size_32() const {
        return this->data_size == 0;
    }
    inline constexpr bool data_size_64() const {
        return false;
    }
    inline constexpr bool addr_size_16() const {
        return this->addr_size != 0;
    }
    inline constexpr bool addr_size_32() const {
        return this->addr_size == 0;
    }
    inline constexpr bool addr_size_64() const {
        return false;
    }
    inline constexpr bool stack_size_16() const {
        return this->stack_size != 0;
    }
    inline constexpr bool stack_size_32() const {
        return this->stack_size == 0;
    }
    inline constexpr bool stack_size_64() const {
        return false;
    }
    inline constexpr REX get_rex_bits() const {
        return {};
    }
    inline constexpr void set_rex_bits(uint8_t bits) {
    }
    inline constexpr void reset_rex_bits() {
    }
    inline constexpr bool is_real_mode() const {
        return this->mode != 0;
    }
    inline constexpr bool is_protected_mode() const {
        return this->mode == 0;
    }
    inline constexpr bool is_long_mode() const {
        return false;
    }

};

template <bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase<64, use_old_reset, has_protected_mode, has_x87, max_sse_bits, sse_reg_count> :
    z86BaseSSE<max_sse_bits, sse_reg_count>,
    z86BaseFPU<has_x87>,
    z86BaseGPRs<64>,
    z86BaseControl<64, use_old_reset, has_protected_mode>,
    z86BaseFPUControl<64, has_x87, max_sse_bits != 0>
{
    union {
        uint64_t rip;
        uint32_t eip;
        uint16_t ip;
    };

    int8_t data_size = 1;
    int8_t addr_size = 1;
    int8_t stack_size = 1;
    int8_t default_data_size = 1;
    int8_t default_addr_size = 1;
    int8_t mode = 1;
    uint8_t opcode_prefix = 0;
    REX rex_bits = {};

    inline constexpr void reset_prefixes() {
        this->data_size = this->default_data_size;
        this->addr_size = this->default_addr_size;
        this->opcode_prefix = 0;
        this->reset_rex_bits();
    }

    inline constexpr void set_opcode_select(uint8_t type) {
        this->opcode_prefix = type;
    }

    inline constexpr void data_size_prefix() {
        this->data_size = this->default_data_size <= 0;
        this->set_opcode_select(1);
    }

    inline constexpr void addr_size_prefix() {
        this->addr_size = this->default_addr_size <= 0;
    }

    inline constexpr uint8_t opcode_select() const {
        return this->opcode_prefix;
    }

    inline constexpr bool data_size_16() const {
        return this->data_size > 0;
    }
    inline constexpr bool data_size_32() const {
        return this->data_size == 0;
    }
    inline constexpr bool data_size_64() const {
        return this->data_size < 0;
    }
    inline constexpr bool addr_size_16() const {
        return this->addr_size > 0;
    }
    inline constexpr bool addr_size_32() const {
        return this->addr_size == 0;
    }
    inline constexpr bool addr_size_64() const {
        return this->addr_size < 0;
    }
    inline constexpr bool stack_size_16() const {
        return this->stack_size > 0;
    }
    inline constexpr bool stack_size_32() const {
        return this->stack_size == 0;
    }
    inline constexpr bool stack_size_64() const {
        return this->stack_size < 0;
    }
    inline constexpr REX get_rex_bits() const {
        return this->rex_bits;
    }
    inline constexpr void set_rex_bits(uint8_t bits) {
        this->rex_bits.raw = bits;
        if (bits & 4) {
            this->data_size = -1;
        }
    }
    inline constexpr void reset_rex_bits() {
        this->rex_bits = {};
    }
    inline constexpr bool is_real_mode() const {
        return this->mode > 0;
    }
    inline constexpr bool is_protected_mode() const {
        return this->mode == 0;
    }
    inline constexpr bool is_long_mode() const {
        return this->mode < 0;
    }

};

struct z86DataProperitesImpl {
    template <typename T>
    static inline constexpr size_t size() {
        if constexpr (is_vector_v<T>) {
            return sizeof(vector_type_t<T>) * vector_length_v<T>;
        }
        else if constexpr (std::is_same_v<T, float>) {
            return 4;
        }
        else if constexpr (std::is_same_v<T, double>) {
            return 8;
        }
        else if constexpr (std::is_same_v<T, long double>) {
            return 10;
        }
        else if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return 1;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return 2;
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return 4;
        }
        else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return 8;
        }
        else if constexpr (sizeof(T) == sizeof(uint128_t)) {
            return 16;
        }
    }

    template <typename T>
    static inline constexpr size_t align() {
        if constexpr (is_vector_v<T>) {
            return sizeof(vector_type_t<T>) * vector_length_v<T>;
        }
        else if constexpr (std::is_same_v<T, float>) {
            return 4;
        }
        else if constexpr (std::is_same_v<T, double>) {
            return 8;
        }
        else if constexpr (std::is_same_v<T, long double>) {
            return 8;
        }
        else if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return 1;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            return 2;
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            return 4;
        }
        else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            return 8;
        }
        else if constexpr (sizeof(T) == sizeof(uint128_t)) {
            return 16;
        }
    }
};

template <typename T>
struct z86DataProperites {
    static inline constexpr size_t size = z86DataProperitesImpl::size<T>();
    static inline constexpr size_t align = z86DataProperitesImpl::align<T>();
};

struct z86AddrSharedFuncs {
    template <typename T>
    static inline constexpr bool addr_fits_on_bus(size_t addr);

    template <typename T>
    static inline constexpr bool addr_crosses_page(size_t addr);

    static inline constexpr size_t regcall virt_to_phys(size_t addr);

    template <typename T, typename P>
    static inline void regcall write(P* self, const T& value, ssize_t offset);

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>, typename P>
    static inline V read(const P* self, ssize_t offset = 0);

    template <typename P>
    static inline uint32_t read_Iz(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint32_t read_advance_Iz(P* self);

    template <typename P>
    static inline int32_t read_Is(const P* self, ssize_t index = 0);

    template <typename P>
    static inline int32_t read_advance_Is(P* self);

    template <typename P>
    static inline uint64_t read_Iv(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint64_t read_advance_Iv(P* self);

    template <typename P>
    static inline uint64_t read_O(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint64_t read_advance_O(P* self);
};

template <size_t bits, bool protected_mode>
struct z86AddrBase;

template <>
struct z86AddrBase<16, false> {
    // Offset Type
    using OT = uint16_t;
    // Far Type
    using FT = uint32_t;
    // Physical Addr Type
    using MT = uint32_t;

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
struct z86AddrBase<16, true> {
    // Offset Type
    using OT = uint16_t;
    // Far Type
    using FT = uint32_t;
    // Physical Addr Type
    using MT = uint32_t;

    union {
        uint32_t raw;
        struct {
            uint16_t offset;
            uint8_t segment;
        };
    };

    inline constexpr z86AddrBase() : raw(0) {}
    inline constexpr z86AddrBase(FT raw) : raw(raw) {}
    inline constexpr z86AddrBase(uint8_t segment, OT offset) : offset(offset), segment(segment) {}
    inline constexpr z86AddrBase(const z86AddrBase&) = default;
};

template <>
struct z86AddrBase<32, false> {
    // Offset Type
    using OT = uint32_t;
    // Far Type
    using FT = uint64_t;
    // Physical Addr Type
    using MT = uint32_t;

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
struct z86AddrBase<32, true> {
    // Offset Type
    using OT = uint32_t;
    // Far Type
    using FT = uint64_t;
    // Physical Addr Type
    using MT = uint32_t;

    union {
        uint64_t raw;
        struct {
            uint32_t offset;
            uint8_t segment;
        };
    };

    inline constexpr z86AddrBase() : raw(0) {}
    inline constexpr z86AddrBase(FT raw) : raw(raw) {}
    inline constexpr z86AddrBase(uint8_t segment, OT offset) : offset(offset), segment(segment) {}
    inline constexpr z86AddrBase(const z86AddrBase&) = default;
};

template <>
struct z86AddrBase<64, false> {
    // Offset Type
    using OT = uint64_t;
    // Far Type
    using FT = uint128_t;
    // Physical Addr Type
    using MT = uint64_t;

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

template <>
struct z86AddrBase<64, true> {
    // Offset Type
    using OT = uint64_t;
    // Far Type
    using FT = uint128_t;
    // Physical Addr Type
    using MT = uint64_t;

    union {
        uint128_t raw;
        struct {
            uint64_t offset;
            uint8_t segment;
        };
    };

    inline constexpr z86AddrBase() : raw(0) {}
    inline constexpr z86AddrBase(FT raw) : raw(raw) {}
    inline constexpr z86AddrBase(uint8_t segment, OT offset) : offset(offset), segment(segment) {}
    inline constexpr z86AddrBase(const z86AddrBase&) = default;
};

template <size_t bits, bool protected_mode>
struct z86AddrImpl : z86AddrBase<bits, protected_mode> {

    // Offset Type
    using OT = z86AddrBase<bits, protected_mode>::OT;
    // Far Type
    using FT = z86AddrBase<bits, protected_mode>::FT;
    // Physical Addr Type
    using MT = z86AddrBase<bits, protected_mode>::MT;

    inline constexpr z86AddrImpl() : z86AddrBase<bits, protected_mode>::z86AddrBase() {}
    inline constexpr z86AddrImpl(FT raw) : z86AddrBase<bits, protected_mode>::z86AddrBase(raw) {}
    inline constexpr z86AddrImpl(uint16_t segment, OT offset) : z86AddrBase<bits, protected_mode>::z86AddrBase(segment, offset) {}
    inline constexpr z86AddrImpl(const z86AddrImpl&) = default;

    template <size_t other_bits, bool other_protection>
    inline constexpr z86AddrImpl(const z86AddrImpl<other_bits, other_protection>& addr) : z86AddrBase<bits, protected_mode>::z86AddrBase(addr.segment, addr.offset) {}

    inline constexpr size_t ptr(ssize_t offset = 0) const {
        return (OT)(this->offset + offset);
    }

    inline constexpr size_t seg() const;

    inline constexpr size_t addr(ssize_t offset = 0) const {
        return this->seg() + this->ptr(offset);
    }

    template <typename T>
    inline constexpr size_t offset_wrap_sub(size_t wrap) const {
        return (std::numeric_limits<OT>::max)() - (wrap - 1);
    }

    template <typename T>
    inline constexpr size_t offset_wrap(ssize_t offset = 0) const {
        OT base = this->offset + offset;
        constexpr size_t non_wrapping_limit = (std::numeric_limits<OT>::max)() - (z86DataProperites<T>::size - 1);
        if (base <= non_wrapping_limit) {
            return 0;
        }
        return base - non_wrapping_limit;
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
        return z86AddrImpl(this->segment, this->offset++);
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
    inline void regcall write(const T& value, ssize_t offset = 0) {
        return z86AddrSharedFuncs::write<T>(this, value, offset);
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read(ssize_t offset = 0) const {
        return z86AddrSharedFuncs::read<T>(this, offset);
    }

    template <typename T = uint8_t>
    inline void regcall write_advance(const T& value, ssize_t index = sizeof(T)) {
        this->write(value);
        this->offset += index;
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read_advance(ssize_t index = sizeof(V)) {
        V ret = this->read<V>();
        this->offset += index;
        return ret;
    }

    inline uint32_t read_Iz(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iz(this, index);
    }

    inline uint32_t read_advance_Iz() {
        return z86AddrSharedFuncs::read_advance_Iz(this);
    }

    inline int32_t read_Is(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Is(this, index);
    }

    inline int32_t read_advance_Is() {
        return z86AddrSharedFuncs::read_advance_Is(this);
    }

    inline uint64_t read_Iv(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iv(this, index);
    }

    inline uint64_t read_advance_Iv() {
        return z86AddrSharedFuncs::read_advance_Iv(this);
    }

    inline uint64_t read_O(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_O(this, index);
    }

    inline uint64_t read_advance_O() {
        return z86AddrSharedFuncs::read_advance_O(this);
    }
};

template <size_t max_bits>
struct z86AddrFixedBase;

template <>
struct z86AddrFixedBase<16> {
    using OT = uint16_t; // Offset Type
    using FT = uint32_t; // Far Type
    using MT = uint32_t; // Memory Addr Type

    uint16_t offset;

    inline constexpr z86AddrFixedBase() : offset(0) {}
    inline constexpr z86AddrFixedBase(OT offset) : offset(offset) {}
    inline constexpr z86AddrFixedBase(FT offset) : offset(offset) {}
    inline constexpr z86AddrFixedBase(const z86AddrFixedBase&) = default;
};

template <>
struct z86AddrFixedBase<32> {
    using OT = uint32_t; // Offset Type
    using FT = uint64_t; // Far Type
    using MT = uint32_t; // Memory Addr Type

    uint32_t offset;

    inline constexpr z86AddrFixedBase() : offset(0) {}
    inline constexpr z86AddrFixedBase(OT offset) : offset(offset) {}
    inline constexpr z86AddrFixedBase(FT offset) : offset(offset) {}
    inline constexpr z86AddrFixedBase(const z86AddrFixedBase&) = default;
};

template <>
struct z86AddrFixedBase<64> {
    using OT = uint64_t; // Offset Type
    using FT = uint128_t; // Far Type
    using MT = uint64_t; // Memory Addr Type

    uint64_t offset;

    inline constexpr z86AddrFixedBase() : offset(0) {}
    inline constexpr z86AddrFixedBase(OT offset) : offset(offset) {}
    inline constexpr z86AddrFixedBase(FT offset) : offset(offset) {}
    inline constexpr z86AddrFixedBase(const z86AddrFixedBase&) = default;
};

template <size_t max_bits, uint8_t descriptor_index>
struct z86AddrFixedImpl : z86AddrFixedBase<max_bits> {

    using OT = z86AddrFixedBase<max_bits>::OT; // Offset Type
    using FT = z86AddrFixedBase<max_bits>::FT; // Far Type
    using MT = z86AddrFixedBase<max_bits>::MT; // Memory Addr Type

    inline constexpr z86AddrFixedImpl() : z86AddrFixedBase<max_bits>::z86AddrFixedBase() {}
    inline constexpr z86AddrFixedImpl(OT offset) : z86AddrFixedBase<max_bits>::z86AddrFixedBase(offset) {}
    inline constexpr z86AddrFixedImpl(FT offset) : z86AddrFixedBase<max_bits>::z86AddrFixedBase(offset) {}
    inline constexpr z86AddrFixedImpl(const z86AddrFixedImpl&) = default;

    inline constexpr size_t ptr(ssize_t offset = 0) const {
        return (OT)(this->offset + offset);
    }

    inline constexpr size_t seg() const;

    inline constexpr size_t addr(ssize_t offset = 0) const {
        return this->seg() + this->ptr(offset);
    }
    
    template <typename T>
    inline constexpr size_t offset_wrap_sub(size_t wrap) const {
        return (std::numeric_limits<OT>::max)() - (wrap - 1);
    }

    template <typename T>
    inline constexpr size_t offset_wrap(ssize_t offset = 0) const {
        OT base = this->offset + offset;
        constexpr size_t non_wrapping_limit = (std::numeric_limits<OT>::max)() - (z86DataProperites<T>::size - 1);
        if (base <= non_wrapping_limit) {
            return 0;
        }
        return base - non_wrapping_limit;
    }

    inline constexpr z86AddrFixedImpl& operator+=(ssize_t offset) {
        this->offset += offset;
        return *this;
    }

    inline constexpr z86AddrFixedImpl& operator-=(ssize_t offset) {
        this->offset -= offset;
        return *this;
    }

    inline constexpr z86AddrFixedImpl& operator++() {
        ++this->offset;
        return *this;
    }

    inline constexpr z86AddrFixedImpl operator++(int) {
        return z86AddrFixedImpl(this->offset++);
    }

    inline constexpr z86AddrFixedImpl& operator--() {
        --this->offset;
        return *this;
    }

    inline constexpr z86AddrFixedImpl operator--(int) {
        return z86AddrFixedImpl(this->offset--);
    }

    inline constexpr OT operator+(ssize_t offset) const {
        return this->offset + offset;
    }

    inline constexpr OT operator-(ssize_t offset) const {
        return this->offset - offset;
    }

    template <typename T = uint8_t>
    inline void regcall write(const T& value, ssize_t offset = 0) {
        return z86AddrSharedFuncs::write<T>(this, value, offset);
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read(ssize_t offset = 0) const {
        return z86AddrSharedFuncs::read<T>(this, offset);
    }

    template <typename T = uint8_t>
    inline void regcall write_advance(const T& value, ssize_t index = sizeof(T)) {
        this->write(value);
        this->offset += index;
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read_advance(ssize_t index = sizeof(V)) {
        V ret = this->read<V>();
        this->offset += index;
        return ret;
    }

    inline uint32_t read_Iz(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iz(this, index);
    }

    inline uint32_t read_advance_Iz() {
        return z86AddrSharedFuncs::read_advance_Iz(this);
    }

    inline int32_t read_Is(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Is(this, index);
    }

    inline int32_t read_advance_Is() {
        return z86AddrSharedFuncs::read_advance_Is(this);
    }

    inline uint64_t read_Iv(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iv(this, index);
    }

    inline uint64_t read_advance_Iv() {
        return z86AddrSharedFuncs::read_advance_Iv(this);
    }

    inline uint64_t read_O(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_O(this, index);
    }

    inline uint64_t read_advance_O() {
        return z86AddrSharedFuncs::read_advance_O(this);
    }
};

template <size_t max_bits, bool protected_mode>
struct z86AddrESImpl;

template <size_t max_bits>
struct z86AddrESImpl<max_bits, false> {
    using type = z86AddrImpl<max_bits, false>;
};

template <size_t max_bits, bool protected_mode>
struct z86AddrCSImpl;

template <size_t max_bits>
struct z86AddrCSImpl<max_bits, false> {
    using type = z86AddrImpl<max_bits, false>;
};

template <size_t max_bits, bool protected_mode>
struct z86AddrSSImpl;

template <size_t max_bits>
struct z86AddrSSImpl<max_bits, false> {
    using type = z86AddrImpl<max_bits, false>;
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
    inline uint32_t length(const P& pc) const {
        return 1 + this->extra_length(pc);
    }

    template <typename P>
    auto parse_memM(P& pc) const;
};

#define z86BaseTemplate size_t bits, size_t bus, uint64_t flagsA
#define z86BaseDefault z86Base<bits, bus, flagsA>

template <size_t bits, size_t bus = bits, uint64_t flagsA = 0>
struct z86Base :
    z86RegBase<bits, flagsA & FLAG_OLD_RESET_PC, flagsA & FLAG_PROTECTED_MODE,
        flagsA & (FLAG_CPUID_X87 | FLAG_CPUID_MMX | FLAG_CPUID_3DNOW),
        (flagsA & FLAG_CPUID_SSE) ? (flagsA & FLAG_CPUID_AVX) ? 256 : 128 : 0,
        (flagsA & FLAG_CPUID_SSE) ? (flagsA & FLAG_LONG_MODE) ? 16 : 8 : 0
    >
{

    static inline constexpr size_t max_bits = bits;
    static inline constexpr size_t bus_width = bus;
    static inline constexpr size_t bus_bytes = bus / 8;

    static inline constexpr z86FeatureTier tier = FEATURES_8086;

    // QUIRK FLAGS
    static inline constexpr bool PUSH_CS = flagsA & FLAG_PUSH_CS;
    static inline constexpr bool SAL_IS_SETMO = flagsA & FLAG_SAL_IS_SETMO;
    static inline constexpr bool NO_UD = flagsA & FLAG_NO_UD;
    static inline constexpr bool SINGLE_MEM_WRAPS = flagsA & FLAG_SINGLE_MEM_WRAPS;
    static inline constexpr bool SHIFT_MASKING = !(flagsA & FLAG_UNMASK_SHIFTS);
    static inline constexpr bool REP_INVERT_MUL = flagsA & FLAG_REP_INVERT_MUL;
    static inline constexpr bool REP_INVERT_IDIV = flagsA & FLAG_REP_INVERT_IDIV;
    static inline constexpr bool FAULTS_ARE_TRAPS = flagsA & FLAG_FAULTS_ARE_TRAPS;
    static inline constexpr bool OLD_PUSH_SP = flagsA & FLAG_OLD_PUSH_SP;
    static inline constexpr bool OLD_RESET_PC = flagsA & FLAG_OLD_RESET_PC;
    static inline constexpr bool OLD_AAA = flagsA & FLAG_OLD_AAA;
    static inline constexpr bool WRAP_SEGMENT_MODRM = flagsA & FLAG_WRAP_SEGMENT_MODRM;
    static inline constexpr bool AAM_NO_DE = flagsA & FLAG_AAM_NO_DE;
    static inline constexpr bool UNMASK_ENTER = flagsA & FLAG_UNMASK_ENTER;
    static inline constexpr bool REP_BOUND = flagsA & FLAG_REP_BOUND;
    static inline constexpr bool REP_MUL_MISSTORE = flagsA & FLAG_REP_MUL_MISSTORE;
    static inline constexpr bool PROTECTED_MODE = flagsA & FLAG_PROTECTED_MODE;
    static inline constexpr bool PAGING = flagsA & FLAG_PAGING;
    static inline constexpr bool LONG_MODE = flagsA & FLAG_LONG_MODE;
    static inline constexpr bool HAS_TEST_REGS = flagsA & FLAG_HAS_TEST_REGS;
    static inline constexpr bool OPCODES_80186 = flagsA & FLAG_OPCODES_80186;
    static inline constexpr bool OPCODES_V20 = flagsA & FLAG_OPCODES_V20;
    static inline constexpr bool OPCODES_80286 = flagsA & FLAG_OPCODES_80286;
    static inline constexpr bool OPCODES_80386 = flagsA & FLAG_OPCODES_80386;
    static inline constexpr bool OPCODES_80486 = flagsA & FLAG_OPCODES_80486;
    static inline constexpr bool OPCODES_P5 = flagsA & FLAG_OPCODES_P5;
    static inline constexpr bool OPCODES_P6 = flagsA & FLAG_OPCODES_P6;
    static inline constexpr bool HAS_CPUID = flagsA & FLAG_HAS_CPUID;
    static inline constexpr bool HAS_LONG_NOP = flagsA & FLAG_HAS_LONG_NOP;
    static inline constexpr bool CPUID_X87 = flagsA & FLAG_CPUID_X87;
    static inline constexpr bool CPUID_CMOV = flagsA & FLAG_CPUID_CMOV;
    static inline constexpr bool CPUID_MMX = flagsA & FLAG_CPUID_MMX;
    static inline constexpr bool CPUID_3DNOW = flagsA & FLAG_CPUID_3DNOW;
    static inline constexpr bool CPUID_SSE = flagsA & FLAG_CPUID_SSE;
    static inline constexpr bool CPUID_SSE2 = flagsA & FLAG_CPUID_SSE2;
    static inline constexpr bool CPUID_SSE3 = flagsA & FLAG_CPUID_SSE3;
    static inline constexpr bool CPUID_SSSE3 = flagsA & FLAG_CPUID_SSSE3;
    static inline constexpr bool CPUID_SSE41 = flagsA & FLAG_CPUID_SSE41;
    static inline constexpr bool CPUID_SSE42 = flagsA & FLAG_CPUID_SSE42;
    static inline constexpr bool CPUID_SSE4A = flagsA & FLAG_CPUID_SSE4A;
    static inline constexpr bool CPUID_AVX = flagsA & FLAG_CPUID_AVX;
    static inline constexpr bool CPUID_AVX2 = flagsA & FLAG_CPUID_AVX2;
    static inline constexpr bool CPUID_XOP = flagsA & FLAG_CPUID_XOP;
    static inline constexpr bool CPUID_FMA3 = flagsA & FLAG_CPUID_FMA3;
    static inline constexpr bool CPUID_POPCNT = flagsA & FLAG_CPUID_POPCNT;
    static inline constexpr bool CPUID_BMI1 = flagsA & FLAG_CPUID_BMI1;
    static inline constexpr bool CPUID_BMI2 = flagsA & FLAG_CPUID_BMI2;
    static inline constexpr bool MADE_UP_INSTRUCTIONS = false;

    // Assuming a previous memset of full context
    inline constexpr void reset_ip() {
        if constexpr (!OLD_RESET_PC) {
            this->rip = 0xFFF0;
        }
    }

    using HT = z86BaseGPRs<bits>::HT;
    using RT = z86BaseGPRs<bits>::RT;
    using DT = z86BaseGPRs<bits>::DT;

    using SRT = std::make_signed_t<RT>;
    
    inline constexpr const uint16_t& get_seg(uint8_t index) {
        if constexpr (WRAP_SEGMENT_MODRM) {
            index &= 3;
        }
        return this->get_seg_impl(index);
    }

    inline constexpr void write_seg(uint8_t index, uint16_t selector) {
        if constexpr (WRAP_SEGMENT_MODRM) {
            index &= 3;
        }
        return this->write_seg_impl(index, selector);
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t full_indexR(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (REX rex = this->get_rex_bits()) {
                index |= rex.R();
                assume(index < 16);
            }
        }
        return index;
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t full_indexI(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (REX rex = this->get_rex_bits()) {
                index |= rex.X();
                assume(index < 16);
            }
        }
        return index;
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t full_indexMB(uint8_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (REX rex = this->get_rex_bits()) {
                index |= rex.B();
                assume(index < 16);
            }
        }
        return index;
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t& index_byte_regR(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (REX rex = this->get_rex_bits()) {
                return this->gpr[index | rex.R()].byte;
            }
        }
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t& index_byte_regI(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (REX rex = this->get_rex_bits()) {
                return this->gpr[index | rex.X()].byte;
            }
        }
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    template <bool ignore_rex = false>
    inline constexpr uint8_t& index_byte_regMB(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            if (REX rex = this->get_rex_bits()) {
                return this->gpr[index | rex.B()].byte;
            }
        }
        return *(&this->gpr[index & 3].byte + (index > 3));
    }

    inline constexpr auto& index_word_reg_raw(uint32_t index) {
        return this->gpr[index].word;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_word_regR(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().R();
        }
        return this->gpr[index].word;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_word_regI(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().X();
        }
        return this->gpr[index].word;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_word_regMB(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().B();
        }
        return this->gpr[index].word;
    }

    inline constexpr auto& index_dword_reg_raw(uint32_t index) {
        return this->gpr[index].dword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_dword_regR(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().R();
        }
        return this->gpr[index].dword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_dword_regI(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().X();
        }
        return this->gpr[index].dword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_dword_regMB(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().B();
        }
        return this->gpr[index].dword;
    }

    inline constexpr auto& index_qword_reg_raw(uint32_t index) {
        return this->gpr[index].qword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_qword_regR(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().R();
        }
        return this->gpr[index].qword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_qword_regI(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().X();
        }
        return this->gpr[index].qword;
    }

    template <bool ignore_rex = false>
    inline constexpr auto& index_qword_regMB(uint32_t index) {
        assume(index < 8);
        if constexpr (!ignore_rex) {
            index |= this->get_rex_bits().B();
        }
        return this->gpr[index].qword;
    }

    inline constexpr long double& index_st_reg(uint32_t index) {
        assume(index < 8);
        if constexpr (CPUID_X87) {
            return this->st[this->stack_top + index & 7];
        }
    }

    template <typename T = void>
    inline constexpr auto& index_mmx_reg(uint32_t index) {
        assume(index < 8);
        if constexpr (CPUID_MMX || CPUID_X87) {
            if constexpr (std::is_same_v<T, void>) {
                return this->mm[index];
            }
            else if constexpr (std::is_same_v<T, uint8_t>) {
                return this->mm[index].byte;
            }
            else if constexpr (std::is_same_v<T, int8_t>) {
                return this->mm[index].sbyte;
            }
            else if constexpr (std::is_same_v<T, uint16_t>) {
                return this->mm[index].word;
            }
            else if constexpr (std::is_same_v<T, int16_t>) {
                return this->mm[index].sword;
            }
            else if constexpr (std::is_same_v<T, uint32_t>) {
                return this->mm[index].dword;
            }
            else if constexpr (std::is_same_v<T, int32_t>) {
                return this->mm[index].sdword;
            }
            else if constexpr (std::is_same_v<T, uint64_t>) {
                return this->mm[index].qword;
            }
            else if constexpr (std::is_same_v<T, int64_t>) {
                return this->mm[index].sqword;
            }
            else if constexpr (std::is_same_v<T, float>) {
                return this->mm[index].f32;
            }
            else if constexpr (std::is_same_v<T, double>) {
                return this->mm[index].f64;
            }
        }
    }

    template <typename T = void, bool ignore_rex = false>
    inline constexpr auto& index_xmm_regR(uint32_t index) {
        assume(index < 8);
        if constexpr (CPUID_SSE) {
            if constexpr (!ignore_rex) {
                index |= this->get_rex_bits().R();
            }
            if constexpr (std::is_same_v<T, void>) {
                return this->xmm[index];
            }
            else if constexpr (std::is_same_v<T, uint8_t>) {
                return this->xmm[index].byte;
            }
            else if constexpr (std::is_same_v<T, int8_t>) {
                return this->xmm[index].sbyte;
            }
            else if constexpr (std::is_same_v<T, uint16_t>) {
                return this->xmm[index].word;
            }
            else if constexpr (std::is_same_v<T, int16_t>) {
                return this->xmm[index].sword;
            }
            else if constexpr (std::is_same_v<T, uint32_t>) {
                return this->xmm[index].dword;
            }
            else if constexpr (std::is_same_v<T, int32_t>) {
                return this->xmm[index].sdword;
            }
            else if constexpr (std::is_same_v<T, uint64_t>) {
                return this->xmm[index].qword;
            }
            else if constexpr (std::is_same_v<T, int64_t>) {
                return this->xmm[index].sqword;
            }
            else if constexpr (std::is_same_v<T, uint128_t>) {
                return this->xmm[index].oword;
            }
            else if constexpr (std::is_same_v<T, uint128_t>) {
                return this->xmm[index].soword;
            }
            else if constexpr (std::is_same_v<T, float>) {
                return this->xmm[index].f32;
            }
            else if constexpr (std::is_same_v<T, double>) {
                return this->xmm[index].f64;
            }
        }
    }

    template <typename T = void, bool ignore_rex = false>
    inline constexpr auto& index_xmm_regMB(uint32_t index) {
        assume(index < 8);
        if constexpr (CPUID_SSE) {
            if constexpr (!ignore_rex) {
                index |= this->get_rex_bits().B();
            }
            if constexpr (std::is_same_v<T, void>) {
                return this->xmm[index];
            }
            else if constexpr (std::is_same_v<T, uint8_t>) {
                return this->xmm[index].byte;
            }
            else if constexpr (std::is_same_v<T, int8_t>) {
                return this->xmm[index].sbyte;
            }
            else if constexpr (std::is_same_v<T, uint16_t>) {
                return this->xmm[index].word;
            }
            else if constexpr (std::is_same_v<T, int16_t>) {
                return this->xmm[index].sword;
            }
            else if constexpr (std::is_same_v<T, uint32_t>) {
                return this->xmm[index].dword;
            }
            else if constexpr (std::is_same_v<T, int32_t>) {
                return this->xmm[index].sdword;
            }
            else if constexpr (std::is_same_v<T, uint64_t>) {
                return this->xmm[index].qword;
            }
            else if constexpr (std::is_same_v<T, int64_t>) {
                return this->xmm[index].sqword;
            }
            else if constexpr (std::is_same_v<T, uint128_t>) {
                return this->xmm[index].oword;
            }
            else if constexpr (std::is_same_v<T, uint128_t>) {
                return this->xmm[index].soword;
            }
            else if constexpr (std::is_same_v<T, float>) {
                return this->xmm[index].f32;
            }
            else if constexpr (std::is_same_v<T, double>) {
                return this->xmm[index].f64;
            }
        }
    }

    template <typename T, bool ignore_rex = false>
    inline constexpr auto& index_regR(uint32_t index) {
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
    inline constexpr auto& index_regI(uint32_t index) {
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
    inline constexpr auto& index_regMB(uint32_t index) {
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
    inline constexpr bool cond_BE(bool val = true) const { return (this->carry | this->zero) == val; }
    inline constexpr bool cond_NA(bool val = true) const { return this->cond_BE(val); }
    inline constexpr bool cond_A(bool val = true) const { return (this->carry | this->zero) != val; }
    inline constexpr bool cond_NBE(bool val = true) const { return this->cond_A(val); }
    inline constexpr bool cond_S(bool val = true) const { return this->sign == val; }
    inline constexpr bool cond_NS(bool val = true) const { return this->sign != val; }
    inline constexpr bool cond_P(bool val = true) const { return this->parity == val; }
    inline constexpr bool cond_PE(bool val = true) const { return this->cond_P(val); }
    inline constexpr bool cond_NP(bool val = true) const { return this->parity != val; }
    inline constexpr bool cond_PO(bool val = true) const { return this->cond_NP(val); }
    inline constexpr bool cond_L(bool val = true) const { return (this->sign ^ this->overflow) == val; }
    inline constexpr bool cond_NGE(bool val = true) const { return this->cond_L(val); }
    inline constexpr bool cond_GE(bool val = true) const { return (this->sign ^ this->overflow) != val; }
    inline constexpr bool cond_NL(bool val = true) const { return this->cond_GE(val); }
    inline constexpr bool cond_LE(bool val = true) const { return (this->zero | (this->sign ^ this->overflow)) == val; }
    inline constexpr bool cond_NG(bool val = true) const { return this->cond_LE(val); }
    inline constexpr bool cond_G(bool val = true) const { return (this->zero | (this->sign ^ this->overflow)) != val; }
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

    inline constexpr void set_lock() {
        this->lock = true;
        if constexpr (LONG_MODE) {
            if (this->is_long_mode()) {
                this->reset_rex_bits();
            }
        }
    }

    inline constexpr void set_rep_type(uint8_t type) {
        type &= 1;
        this->rep_type = type;
        this->set_opcode_select(type + 2);
        if constexpr (LONG_MODE) {
            if (this->is_long_mode()) {
                this->reset_rex_bits();
            }
        }
    }

    inline constexpr void set_repc_type(uint8_t type) {
        if constexpr (OPCODES_V20 && !OPCODES_80386) {
            this->rep_type = REP_NC + (type & 1);
            if constexpr (LONG_MODE) {
                if (this->is_long_mode()) {
                    this->reset_rex_bits();
                }
            }
        }
    }

    inline constexpr bool has_rep() const {
        return this->rep_type >= NO_REP;
    }

    inline constexpr bool is_repc() const {
        if constexpr (OPCODES_V20 && !OPCODES_80386) {
            return this->rep_type >= REP_NC;
        }
        else {
            return false;
        }
    }

    inline constexpr void set_seg_override(uint8_t seg) {
        this->seg_override = seg;
        if constexpr (LONG_MODE) {
            if (this->is_long_mode()) {
                this->reset_rex_bits();
            }
        }
    }

    inline constexpr uint16_t segment(uint8_t default_seg) const {
        if constexpr (PROTECTED_MODE) {
            return this->seg_override < 0 ? default_seg : this->seg_override;
        } else {
            return this->seg[this->seg_override < 0 ? default_seg : this->seg_override];
        }
    }

    inline constexpr DT addr(uint8_t default_seg, RT offset) const {
        return (DT)this->segment(default_seg) << bitsof(RT) | offset;
    }

    inline constexpr DT addr_force(uint8_t seg, RT offset) const {
        if constexpr (PROTECTED_MODE) {
            return (DT)seg << bitsof(RT) | offset;
        } else {
            return (DT)this->seg[seg] << bitsof(RT) | offset;
        }
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

    inline void regcall update_parity(uint8_t val) {
        this->parity = !__builtin_parity(val);
    }

    template <typename T>
    inline void regcall update_pzs(T val) {
        using S = std::make_signed_t<T>;
        this->update_parity(val);
        this->zero = !val;
        this->sign = (S)val > 0;
    }

    template <typename T = RT>
    inline void regcall JMPABS(T new_ip) {
        this->rip = new_ip;
    }

    template <typename T = RT>
    inline void regcall JMPFABS(T new_ip, uint16_t new_cs) {
        this->JMPABS(new_ip);
        this->cs = new_cs;
    }

    template <typename P>
    inline void regcall JMPFABS(const P& pc) {
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
    inline void regcall PUSH_impl(const T& src);

    template <typename T>
    inline void regcall PUSH16(const T& src) {
        return this->PUSH_impl<uint16_t>(src);
    }

    template <typename T>
    inline void regcall PUSH32(const T& src) {
        return this->PUSH_impl<uint32_t>(src);
    }

    template <typename T>
    inline void regcall PUSH64(const T& src) {
        return this->PUSH_impl<uint64_t>(src);
    }

    template <typename T = SRT>
    inline void regcall PUSH(const T& src) {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            if constexpr (bits == 64) {
                // 64 bit values can only be pushed in long
                // mode, where the stack is always 64 bit
                return this->PUSH64(src);
            }
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            if constexpr (bits > 16) {
                // No need to check for 64 bit stack size
                // because a 32 bit push can't be encoded
                // when running in long mode (hopefully)
                if (this->stack_size_32()) {
                    return this->PUSH32(src);
                }
            }
        }
        else {
            // 16 bit pushes are horrible and exist
            // in all modes
            if constexpr (bits > 16) {
                if (this->stack_size_32()) {
                    return this->PUSH32(src);
                }
                if constexpr (bits == 64) {
                    if (this->stack_size_64()) {
                        return this->PUSH64(src);
                    }
                }
            }
        }
        // This is doubling as a backup case for the constexprs
        return this->PUSH16(src);
    }

    void PUSHA() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                uint32_t temp = this->esp;
                if (this->stack_size_32()) {
                    this->PUSH32(this->eax);
                    this->PUSH32(this->ecx);
                    this->PUSH32(this->edx);
                    this->PUSH32(this->ebx);
                    this->PUSH32(temp);
                    this->PUSH32(this->ebp);
                    this->PUSH32(this->esi);
                    this->PUSH32(this->edi);
                    return;
                }
                else {
                    this->PUSH16(this->eax);
                    this->PUSH16(this->ecx);
                    this->PUSH16(this->edx);
                    this->PUSH16(this->ebx);
                    this->PUSH16(temp);
                    this->PUSH16(this->ebp);
                    this->PUSH16(this->esi);
                    this->PUSH16(this->edi);
                    return;
                }
            }
        }
        uint16_t temp = this->sp;
        if constexpr (bits > 16) {
            if (this->stack_size_32()) {
                this->PUSH32(this->ax);
                this->PUSH32(this->cx);
                this->PUSH32(this->dx);
                this->PUSH32(this->bx);
                this->PUSH32(temp);
                this->PUSH32(this->bp);
                this->PUSH32(this->si);
                this->PUSH32(this->di);
                return;
            }
        }
        this->PUSH_impl<uint16_t>(this->ax);
        this->PUSH_impl<uint16_t>(this->cx);
        this->PUSH_impl<uint16_t>(this->dx);
        this->PUSH_impl<uint16_t>(this->bx);
        this->PUSH_impl<uint16_t>(temp);
        this->PUSH_impl<uint16_t>(this->bp);
        this->PUSH_impl<uint16_t>(this->si);
        this->PUSH_impl<uint16_t>(this->di);
    }

    inline void regcall PUSHI(SRT val) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->PUSH<int32_t>(val);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->PUSH<int64_t>(val);
                }
            }
        }
        return this->PUSH<int16_t>(val);
    }

    template <typename P, typename T>
    inline T regcall POP_impl();

    template <typename T>
    inline T regcall POP16() {
        return this->POP_impl<uint16_t, T>();
    }

    template <typename T>
    inline T regcall POP32() {
        return this->POP_impl<uint32_t, T>();
    }

    template <typename T>
    inline T regcall POP64() {
        return this->POP_impl<uint64_t, T>();
    }

    template <typename T = RT>
    inline T regcall POP() {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            if constexpr (bits == 64) {
                // 64 bit values can only be popped in long
                // mode, where the stack is always 64 bit
                return this->POP64<T>();
            }
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            if constexpr (bits > 16) {
                // No need to check for 64 bit stack size
                // because a 32 bit pop can't be encoded
                // when running in long mode (hopefully)
                if (this->stack_size_32()) {
                    return this->POP32<T>();
                }
            }
        }
        else {
            // 16 bit pops are horrible and exist
            // in all modes
            if constexpr (bits > 16) {
                if (this->stack_size_32()) {
                    return this->POP32<T>();
                }
                if constexpr (bits == 64) {
                    if (this->stack_size_64()) {
                        return this->POP64<T>();
                    }
                }
            }
        }
        // This is doubling as a backup case for the constexprs
        return this->POP16<T>();
    }

    void POPA() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                if (this->stack_size_32()) {
                    this->edi = this->POP32<uint32_t>();
                    this->esi = this->POP32<uint32_t>();
                    this->ebp = this->POP32<uint32_t>();
                    (void)this->POP32<uint32_t>();
                    this->ebx = this->POP32<uint32_t>();
                    this->edx = this->POP32<uint32_t>();
                    this->ecx = this->POP32<uint32_t>();
                    this->eax = this->POP32<uint32_t>();
                    return;
                } else {
                    this->edi = this->POP16<uint32_t>();
                    this->esi = this->POP16<uint32_t>();
                    this->ebp = this->POP16<uint32_t>();
                    (void)this->POP16<uint32_t>();
                    this->ebx = this->POP16<uint32_t>();
                    this->edx = this->POP16<uint32_t>();
                    this->ecx = this->POP16<uint32_t>();
                    this->eax = this->POP16<uint32_t>();
                    return;
                }
            }
            if (this->stack_size_32()) {
                this->di = this->POP32<uint16_t>();
                this->si = this->POP32<uint16_t>();
                this->bp = this->POP32<uint16_t>();
                (void)this->POP32<uint16_t>();
                this->bx = this->POP32<uint16_t>();
                this->dx = this->POP32<uint16_t>();
                this->cx = this->POP32<uint16_t>();
                this->ax = this->POP32<uint16_t>();
                return;
            }
        }
        this->di = this->POP16<uint16_t>();
        this->si = this->POP16<uint16_t>();
        this->bp = this->POP16<uint16_t>();
        (void)this->POP16<uint16_t>();
        this->bx = this->POP16<uint16_t>();
        this->dx = this->POP16<uint16_t>();
        this->cx = this->POP16<uint16_t>();
        this->ax = this->POP16<uint16_t>();
    }

    template <typename T>
    gnu_attr(minsize) inline void regcall ENTER_impl(uint16_t alloc, uint8_t nesting);

    // http://www.os2museum.com/wp/if-you-enter-you-might-not-leave/
    inline void regcall ENTER(uint16_t alloc, uint8_t nesting) {
        if constexpr (!UNMASK_ENTER) {
            nesting &= 0x1F;
        }
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->ENTER_impl<uint32_t>(alloc, nesting);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->ENTER_impl<uint64_t>(alloc, nesting);
                }
            }
        }
        return this->ENTER_impl<uint16_t>(alloc, nesting);
    }

    inline void regcall LEAVE() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                if (this->stack_size_32()) {
                    this->esp = this->ebp;
                    this->ebp = this->POP32<uint32_t>();
                    return;
                }
                else {
                    this->sp = this->bp;
                    this->ebp = this->POP16<uint32_t>();
                    return;
                }
            }
            if constexpr (bits == 64) {
                if (this->stack_size_64()) {
                    this->rsp = this->rbp;
                    if (this->data_size_64()) {
                        this->rbp = this->POP64<uint64_t>();
                    }
                    else {
                        this->bp = this->POP64<uint16_t>();
                    }
                    return;
                }
            }
            if (this->stack_size_32()) {
                this->esp = this->ebp;
                this->bp = this->POP32<uint16_t>();
                return;
            }
        }
        this->sp = this->bp;
        this->bp = this->POP16<uint16_t>();
    }

    template <typename P>
    inline void regcall CALL(const P& pc) {
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

    inline void regcall RET() {
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

    inline void regcall RETF() {
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
    inline void regcall RETI(const P& pc) {
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
    inline void regcall RETFI(const P& pc) {
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
    inline void regcall JMP(const P& pc) {
        if constexpr (is_byte) {
            auto new_ip = pc.offset + 1 + pc.read<int8_t>();
            if constexpr (bits > 16) {
                if (this->data_size_16()) {
                    new_ip = (uint16_t)new_ip;
                }
            }
            this->rip = new_ip;
        }
        else {
            if constexpr (bits > 16) {
                if (!this->data_size_16()) {
                    this->rip = pc.offset + 4 + pc.read<int32_t>();
                    return;
                }
            }
            this->rip = (uint16_t)(pc.offset + 2 + pc.read<int16_t>());
        }
    }

    template <CONDITION_CODE cc, typename T>
    inline void regcall CMOVCC(T& dst, T src, bool val = true) {
        if (this->cond<cc>(val)) {
            dst = src;
        }
    }

    template <CONDITION_CODE cc, typename T>
    inline void regcall SETCC(T& dst, bool val = true) {
        dst = this->cond<cc>(val);
    }

    template <CONDITION_CODE cc, bool is_byte = false, typename P>
    inline void regcall JCC(const P& pc, bool val = true) {
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
            this->rip = new_ip;
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
            this->rip = new_ip;
        }
    }

    template <typename T, typename P>
    inline void regcall LOOP_impl(const P& pc, T& index) {
        auto new_ip = pc.offset + 1;
        if (--index) {
            new_ip += pc.read<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        this->rip = new_ip;
    }

    template <typename P>
    inline void regcall LOOP(const P& pc) {
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
    inline void regcall LOOPCC_impl(const P& pc, T& index, bool val) {
        auto new_ip = pc.offset + 1;
        if (--index || this->cond_Z(val)) {
            new_ip += pc.read<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        this->rip = new_ip;
    }

    template <typename P>
    inline void regcall LOOPCC(const P& pc, bool val) {
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
    inline void regcall JCXZ_impl(const P& pc, const T& index) {
        auto new_ip = pc.offset + 1;
        if (!index) {
            new_ip += pc.read<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        this->rip = new_ip;
    }

    template <typename P>
    inline void regcall JCXZ(const P& pc) {
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
    inline void regcall CALLABS(T next_ip, T new_ip) {
        this->PUSH(next_ip);
        this->JMPABS(new_ip);
    }

    template <typename T = RT>
    inline void regcall CALLFABS(T next_ip, T new_ip, uint16_t new_cs) {
        this->PUSH<T>(this->cs);
        this->PUSH(next_ip);
        this->JMPFABS(new_ip, new_cs);
    }

    template <typename P>
    inline void regcall CALLFABS(const P& pc) {
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
    inline void regcall ADD(T& dst, T src) {
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
    inline void regcall ADD(T1& dst, T2 src) {
        return this->ADD<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall OR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst |= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void regcall OR(T1& dst, T2 src) {
        return this->OR<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall ADC(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = carry_add((U)dst, (U)src, this->carry);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->overflow = (S)(~(dst ^ src) & (dst ^ res)) < 0;
        dst = res;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void regcall ADC(T1& dst, T2 src) {
        return this->ADC<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall SBB(T& dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        T res = carry_sub<U>(dst, src, this->carry);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->overflow = (S)(~(dst ^ src) & (dst ^ res)) < 0;
        dst = res;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void regcall SBB(T1& dst, T2 src) {
        return this->SBB<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall AND(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst &= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void regcall AND(T1& dst, T2 src) {
        return this->AND<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall SUB(T& dst, T src) {
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
    inline void regcall SUB(T1& dst, T2 src) {
        return this->SUB<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall XOR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst ^= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2>
    inline void regcall XOR(T1& dst, T2 src) {
        return this->XOR<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall CMP(T dst, T src) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;
        this->carry = sub_would_overflow<U>(dst, src);
        this->overflow = sub_would_overflow<S>(dst, src);
        T res = dst - src;
        this->auxiliary = (dst ^ src ^ res) & 0x10;
        this->update_pzs(res);
    }

    template <typename T1, typename T2>
    inline void regcall CMP(T1 dst, T2 src) {
        return this->CMP<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall TEST(T dst, T src) {
        this->carry = false;
        this->overflow = false;
        this->update_pzs((T)(dst & src));
    }

    template <typename T1, typename T2>
    inline void regcall TEST(T1 dst, T2 src) {
        return this->TEST<T1>(dst, (std::make_signed_t<T1>)src);
    }

    template <typename T>
    inline void regcall INC(T& dst) {
        using S = std::make_signed_t<T>;
        this->overflow = dst == (std::numeric_limits<S>::max)();
        this->auxiliary = (dst ^ 1 ^ dst + 1) & 0x10; // BLCMSK
        this->update_pzs(++dst);
    }

    template <typename T>
    inline void regcall DEC(T& dst) {
        using S = std::make_signed_t<T>;
        this->overflow = dst == (std::numeric_limits<S>::max)();
        this->auxiliary = (dst ^ 1 ^ dst - 1) & 0x10; // BLSMSK
        this->update_pzs(--dst);
    }

    template <typename T>
    inline void regcall NOT(T& dst) {
        dst = ~dst;
    }

    template <typename T>
    inline void regcall NEG(T& dst) {
        using S = std::make_signed_t<T>;
        this->carry = dst;
        this->overflow = (S)dst == (std::numeric_limits<S>::min)();
        this->auxiliary = dst & 0xF;
        dst = -dst;
        this->update_pzs(dst);
    }

    template <typename T>
    inline void regcall XCHG(T& dst, T& src) {
        std::swap(dst, src);
    }

    inline void regcall CBW() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                // CWDE
                this->eax = (int32_t)(int16_t)this->ax;
                return;
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    // CDQE
                    this->rax = (int64_t)(int32_t)this->eax;
                    return;
                }
            }
        }
        // CBW
        this->ax = (int16_t)(int8_t)this->al;
    }

    inline void regcall CWD() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                // CDQ
                this->edx = (int32_t)this->eax >> 31;
                return;
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    // CDO
                    this->rdx = (int64_t)this->rax >> 63;
                    return;
                }
            }
        }
        // CWD
        this->dx = (int16_t)this->ax >> 15;
    }

    template <typename T, typename R = std::make_unsigned_t<dbl_int_t<T>>>
    inline R MUL_impl(T lhs, T rhs) {
        using U = std::make_unsigned_t<T>;

        R ret = lhs;
        ret *= (U)rhs;

        // Is this correct here?
        if constexpr (REP_INVERT_MUL) {
            if (expect(this->has_rep(), false)) {
                ret = -ret;
            }
        }

        this->carry = this->overflow = ret >> bitsof(T);
        return ret;
    }

    template <typename T>
    inline void regcall MUL(T src) {
        auto value = this->MUL_impl(this->A<T>(), src);
        if constexpr (REP_MUL_MISSTORE) {
            if (expect(this->has_rep(), false)) {
                if constexpr (sizeof(T) == sizeof(uint8_t)) {
                    this->index_byte_regR(4) = value; // AH
                }
                else if constexpr (sizeof(T) == sizeof(uint16_t)) {
                    this->index_word_regR(4) = value; // SP
                }
                else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                    this->index_dword_regR(4) = value; // ESP
                }
                else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                    this->index_qword_regR(4) = value; // RSP
                }
                return;
            }
        }
        return this->write_AD(value);
    }

    template <typename T, typename R = std::make_signed_t<dbl_int_t<T>>>
    inline R IMUL_impl(T lhs, T rhs) {
        using U = std::make_unsigned_t<T>;
        using S = std::make_signed_t<T>;

        R ret = lhs;
        ret *= (S)rhs;

        // Supposedly there's 80186 rep jank too:
        // https://forum.vcfed.org/index.php?threads/8088-8086-microcode-disassembly.77933/page-2#post-951895
        if constexpr (REP_INVERT_MUL) {
            if (expect(this->has_rep(), false)) {
                ret = -ret;
            }
        }
        this->carry = this->overflow = (U)(ret >> bitsof(T)) != (S)ret >> (bitsof(T) - 1);

        return ret;
    }

    template <typename T>
    inline void regcall IMUL(T src) {
        auto value = this->IMUL_impl(this->A<T>(), src);
        if constexpr (REP_MUL_MISSTORE) {
            if (expect(this->has_rep(), false)) {
                if constexpr (sizeof(T) == sizeof(uint8_t)) {
                    this->index_byte_regR(5) = value; // CH
                }
                else if constexpr (sizeof(T) == sizeof(uint16_t)) {
                    this->index_word_regR(5) = value; // BP
                }
                else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                    this->index_dword_regR(5) = value; // EBP
                }
                else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                    this->index_qword_regR(5) = value; // RBP
                }
                return;
            }
        }
        return this->write_AD(value);
    }

    template <typename T>
    inline void regcall IMUL(T& dst, T src) {
        dst = this->IMUL_impl(dst, src);
    }

    template <typename T, typename I>
    inline void regcall IMUL(T& dst, T src, I val) {
        using S = std::make_signed_t<T>;
        dst = this->IMUL_impl(src, (T)(S)val);
    }

    template <typename T>
    inline bool regcall DIV(T src) {
        if (src) {
            using UD = std::make_unsigned_t<dbl_int_t<T>>;
            using U = std::make_unsigned_t<T>;
            UD temp = this->read_AD<T>();
            UD quot = temp / src;

            this->A<T>() = quot;
            this->ADH<T>() = temp % src;
            if (quot > (std::numeric_limits<U>::max)()) {
                return this->set_fault(IntDE);
            }
        }
        else {
            return this->set_fault(IntDE);
        }
        return false;
    }

    template <typename T>
    inline bool regcall IDIV(T src) {
        if (src) {
            using SD = std::make_signed_t<dbl_int_t<T>>;
            using S = std::make_signed_t<T>;
            using U = std::make_unsigned_t<T>;

            SD temp = this->read_AD<T>();
            SD quot = temp / src;

            if constexpr (REP_INVERT_IDIV) {
                if (expect(this->has_rep(), false)) {
                    quot = -quot;
                }
            }

            this->A<T>() = quot;
            this->ADH<T>() = temp % src;
            if ((U)(quot - (std::numeric_limits<S>::min)()) > (std::numeric_limits<U>::max)()) {
                return this->set_fault(IntDE);
            }
        }
        else {
            return this->set_fault(IntDE);
        }
        return false;
    }

    inline void regcall ROL4(uint8_t& dst) {
        // HACK: ROL4 AL just rotates by 4
        if (&dst != &this->al) {
            uint8_t temp = dst;
            dst = temp << 4 | this->al & 0xF;
            this->al <<= 4;
            this->al |= temp >> 4;
        }
        else {
            dst = std::rotl(dst, 4);
        }
    }

    inline void regcall ROR4(uint8_t& dst) {
        // HACK: ROR4 AL does nothing
        if (&dst != &this->al) {
            uint8_t temp = dst;
            dst = this->al << 4 | temp >> 4;
            this->al = temp;
        }
    }

    template <typename T>
    inline void regcall ROL(T& dst, uint8_t count) {

        if constexpr (SHIFT_MASKING) {
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

    template <typename T>
    inline void regcall ROR(T& dst, uint8_t count) {

        if constexpr (SHIFT_MASKING) {
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

    template <typename T>
    inline void regcall RCL(T& dst, uint8_t count) {

        constexpr size_t total_bits = bitsof(T) + 1;
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint32_t)) {
                count = (count & 0x1F) % total_bits;
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                count &= 0x1F;
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            UBitInt(total_bits) temp = dst;
            if (this->carry) {
                temp += (U)(std::numeric_limits<S>::min)();
                temp += (U)(std::numeric_limits<S>::min)();
            }

            if constexpr (!SHIFT_MASKING) {
                count %= total_bits;
            }

            temp = temp << count | temp >> (total_bits - count);
            this->carry = temp > (std::numeric_limits<U>::max)();
            dst = (U)temp;
            this->overflow = this->carry == (S)dst < 0;
        }
    }

    template <typename T>
    inline void regcall RCR(T& dst, uint8_t count) {

        constexpr size_t total_bits = bitsof(T) + 1;
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint32_t)) {
                count = (count & 0x1F) % total_bits;
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                count &= 0x1F;
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = std::make_unsigned_t<T>;
            using S = std::make_signed_t<T>;

            UBitInt(total_bits) temp = dst;
            if (this->carry) {
                temp += (U)(std::numeric_limits<S>::min)();
                temp += (U)(std::numeric_limits<S>::min)();
            }

            if constexpr (!SHIFT_MASKING) {
                count %= total_bits;
            }

            temp = temp >> count | temp << total_bits - count;
            this->carry = temp > (std::numeric_limits<U>::max)();
            dst = (U)temp;
            this->overflow = __builtin_parity(dst & 3u << bitsof(T) - 2);
        }
    }

    template <typename T>
    inline void regcall SHL(T& dst, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }
        if (count) {
            using S = std::make_signed_t<T>;

            this->carry = ((size_t)dst >> (bitsof(T) - count)) & 1;
            T res = (size_t)dst << count;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
            this->update_pzs(dst);
        }
    }

    template <typename T>
    inline void regcall SHR(T& dst, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using S = std::make_signed_t<T>;

            this->carry = ((size_t)dst >> (count - 1)) & 1;
            this->overflow = (S)dst < 0;
            dst = (size_t)dst >> count;
            this->update_pzs(dst);
        }
    }

    // Yay, jank
    template <typename T>
    inline void regcall SETMO(T& dst, uint8_t count) {
        if constexpr (!SAL_IS_SETMO) {
            return this->SHL(dst, count);
        }
        else {
            if constexpr (SHIFT_MASKING) {
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
    }

    template <typename T>
    inline void regcall SAR(T& dst, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            this->carry = ((size_t)dst >> (count - 1)) & 1;
            this->overflow = false;
            dst = (ssize_t)dst >> count;
            this->update_pzs(dst);
        }
    }

    template <typename T>
    inline void regcall SHLD(T& dst, T src, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using DU = dbl_int_t<std::make_unsigned_t<T>>;
            using S = std::make_signed_t<T>;

            DU temp = src;
            temp |= (DU)dst << bitsof(T);
            this->carry = ((size_t)temp >> (bitsof(DU) - count)) & 1;
            dst = (size_t)temp << count;
            this->overflow = this->carry == (S)dst < 0;
            this->update_pzs(dst);
        }
    }

    template <typename T>
    inline void regcall SHRD(T& dst, T src, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = std::make_unsigned_t<T>;
            using DU = dbl_int_t<U>;

            DU temp = dst;
            temp |= (DU)src << bitsof(T);
            this->carry = ((size_t)temp >> (count - 1)) & 1;
            dst = (size_t)temp >> count;
            this->overflow = __builtin_parity((U)dst >> (bitsof(T) - 2) & 3);
            this->update_pzs(dst);
        }
    }

    template <typename T>
    inline void regcall BT(T dst, T src) {
        assume(src < bitsof(T));
        using U = std::make_unsigned_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
    }

    template <typename T>
    inline void regcall BTS(T& dst, T src) {
        assume(src < bitsof(T));
        using U = std::make_unsigned_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
        dst |= mask;
    }
    template <typename T>
    inline void regcall BTR(T& dst, T src) {
        assume(src < bitsof(T));
        using U = std::make_unsigned_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
        dst &= ~mask;
    }

    template <typename T>
    inline void regcall BTC(T& dst, T src) {
        assume(src < bitsof(T));
        using U = std::make_unsigned_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
        dst ^= mask;
    }

    template <typename T>
    inline void regcall BSF(T& dst, T src) {
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
    inline void regcall BSR(T& dst, T src) {
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
    inline void regcall TEST1(T dst, uint8_t count) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            count &= 0x7;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            count &= 0xF;
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            count &= 0x1F;
        }
        else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            count &= 0x3F;
        }
        using U = std::make_unsigned_t<T>;

        this->zero = dst & (U)1 << count;
        // Some docs say carry/overflow are set to 0
        this->carry = this->overflow = false;
    }

    template <typename T>
    inline void regcall SET1(T& dst, uint8_t count) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            count &= 0x7;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            count &= 0xF;
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            count &= 0x1F;
        }
        else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            count &= 0x3F;
        }
        using U = std::make_unsigned_t<T>;

        const U mask = (U)1 << count;
        dst |= mask;
    }


    template <typename T>
    inline void regcall CLR1(T& dst, uint8_t count) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            count &= 0x7;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            count &= 0xF;
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            count &= 0x1F;
        }
        else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            count &= 0x3F;
        }
        using U = std::make_unsigned_t<T>;

        const U mask = (U)1 << count;
        dst &= ~mask;
    }

    template <typename T>
    inline void regcall NOT1(T& dst, uint8_t count) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            count &= 0x7;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t)) {
            count &= 0xF;
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            count &= 0x1F;
        }
        else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            count &= 0x3F;
        }
        using U = std::make_unsigned_t<T>;

        const U mask = (U)1 << count;
        dst ^= mask;
    }

    template <typename T>
    inline void regcall BSWAP(T& src) {
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
    inline bool regcall LODS_impl();

    template <bool is_byte = false>
    inline bool regcall LODS() {
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
                        if (this->data_size_64()) {
                            return this->LODS_impl<uint64_t, uint64_t>();
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
    inline bool regcall MOVS_impl();

    template <bool is_byte = false>
    inline bool regcall MOVS() {
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
                        if (this->data_size_64()) {
                            return this->MOVS_impl<uint64_t, uint64_t>();
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
    inline bool regcall STOS_impl();

    template <bool is_byte = false>
    inline bool regcall STOS() {
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
                        if (this->data_size_64()) {
                            return this->STOS_impl<uint64_t, uint64_t>();
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
    inline bool regcall SCAS_impl();

    template <bool is_byte = false>
    inline bool regcall SCAS() {
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
                        if (this->data_size_64()) {
                            return this->SCAS_impl<uint64_t, uint64_t>();
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
    inline bool regcall CMPS_impl();

    template <bool is_byte = false>
    inline bool regcall CMPS() {
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
                        if (this->data_size_64()) {
                            return this->CMPS_impl<uint64_t, uint64_t>();
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

    template <typename P>
    inline bool regcall ADD4S_impl();

    inline bool regcall ADD4S() {
        if constexpr (bits > 16) {
            if (this->addr_size_32()) {
                return this->ADD4S_impl<uint32_t>();
            }
            if constexpr (bits == 64) {
                if (this->addr_size_64()) {
                    return this->ADD4S_impl<uint64_t>();
                }
            }
        }
        return this->ADD4S_impl<uint16_t>();
    }

    template <typename T>
    inline void regcall port_out_impl(uint16_t port, T value) const;

    template <bool is_byte = false>
    inline void regcall port_out(uint16_t port) const {
        if constexpr (is_byte) {
            return this->port_out_impl<uint8_t>(port, this->al);
        }
        else {
            if constexpr (bits > 16) {
                if (!this->data_size_16()) {
                    return this->port_out_impl<uint32_t>(port, this->eax);
                }
            }
            return this->port_out_impl<uint16_t>(port, this->ax);
        }
    }

    template <typename T>
    inline T regcall port_in_impl(uint16_t port);

    template <bool is_byte = false>
    inline void regcall port_in(uint16_t port) {
        if constexpr (is_byte) {
            this->al = this->port_in_impl<uint8_t>(port);
        }
        else {
            if constexpr (bits > 16) {
                if (!this->data_size_16()) {
                    this->eax = this->port_in_impl<uint32_t>(port);
                    return;
                }
            }
            this->ax = this->port_in_impl<uint16_t>(port);
        }
    }

    template <typename T, typename P>
    inline bool regcall OUTS_impl();

    template <bool is_byte = false>
    gnu_attr(minsize) gnu_noinline bool OUTS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    return this->OUTS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        return this->OUTS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->OUTS_impl<uint8_t, uint16_t>();
        } else {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    if (this->data_size_32()) {
                        return this->OUTS_impl<uint32_t, uint32_t>();
                    }
                    return this->OUTS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        if (this->data_size_32()) {
                            return this->OUTS_impl<uint32_t, uint64_t>();
                        }
                        return this->OUTS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size_32()) {
                    return this->OUTS_impl<uint32_t, uint16_t>();
                }
            }
            return this->OUTS_impl<uint16_t, uint16_t>();
        }
    }

    template <typename T, typename P>
    inline bool regcall INS_impl();

    template <bool is_byte = false>
    gnu_attr(minsize) gnu_noinline bool INS() {
        if constexpr (is_byte) {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    return this->INS_impl<uint8_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        return this->INS_impl<uint8_t, uint64_t>();
                    }
                }
            }
            return this->INS_impl<uint8_t, uint16_t>();
        } else {
            if constexpr (bits > 16) {
                if (this->addr_size_32()) {
                    if (this->data_size_32()) {
                        return this->INS_impl<uint32_t, uint32_t>();
                    }
                    return this->INS_impl<uint16_t, uint32_t>();
                }
                if constexpr (bits == 64) {
                    if (this->addr_size_64()) {
                        if (this->data_size_32()) {
                            return this->INS_impl<uint32_t, uint64_t>();
                        }
                        return this->INS_impl<uint16_t, uint64_t>();
                    }
                }
                if (this->data_size_32()) {
                    return this->INS_impl<uint32_t, uint16_t>();
                }
            }
            return this->INS_impl<uint16_t, uint16_t>();
        }
    }

    inline void regcall fop_set(uint8_t opcode, uint8_t modrm) {
        if constexpr (CPUID_X87) {
            this->fop = (uint16_t)opcode << 8 | modrm;
        }
    }

    inline void regcall FINCSTP() {
        if constexpr (CPUID_X87) {
            ++this->stack_top;
        }
    }

    inline void regcall FDECSTP() {
        if constexpr (CPUID_X87) {
            --this->stack_top;
        }
    }

    inline constexpr long double& regcall FTOP() {
        if constexpr (CPUID_X87) {
            return this->st[this->stack_top & 7];
        }
    }

    inline constexpr void regcall FPUSH(long double value) {
        if constexpr (CPUID_X87) {
            this->st[--this->stack_top & 7] = value;
        }
    }

    inline constexpr long double regcall FPOP() {
        if constexpr (CPUID_X87) {
            return this->st[this->stack_top++ & 7];
        }
    }

    inline void regcall FXCH(long double& value) {
        if constexpr (CPUID_X87) {
            std::swap(this->FTOP(), value);
        }
    }

    template <CONDITION_CODE cc>
    inline void regcall FCMOVCC(long double rhs, bool val = true) {
        if constexpr (CPUID_X87) {
            if (this->cond<cc>(val)) {
                this->FTOP() = rhs;
            }
        }
    }

    inline void regcall FADD(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs += rhs;
        }
    }

    inline void regcall FADD(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FADD(this->FTOP(), rhs);
        }
    }

    inline void regcall FMUL(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs *= rhs;
        }
    }

    inline void regcall FMUL(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FMUL(this->FTOP(), rhs);
        }
    }

    inline void regcall FCOM(long double lhs, long double rhs) {
        if constexpr (CPUID_X87) {

        }
    }

    inline void regcall FCOM(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FCOM(this->FTOP(), rhs);
        }
    }

    inline void regcall FSUB(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs -= rhs;
        }
    }

    inline void regcall FSUB(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FSUB(this->FTOP(), rhs);
        }
    }

    inline void regcall FSUBR(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs = rhs - lhs;
        }
    }

    inline void regcall FSUBR(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FSUBR(this->FTOP(), rhs);
        }
    }

    inline void regcall FDIV(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs /= rhs;
        }
    }

    inline void regcall FDIV(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FDIV(this->FTOP(), rhs);
        }
    }

    inline void regcall FDIVR(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs = rhs / lhs;
        }
    }

    inline void regcall FDIVR(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FDIVR(this->FTOP(), rhs);
        }
    }

    inline void regcall FUCOM(long double& lhs, long double rhs) {

    }

    inline void regcall FUCOM(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FUCOM(this->FTOP(), rhs);
        }
    }

    inline void regcall FCOMI(long double& lhs, long double rhs) {

    }

    inline void regcall FCOMI(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FCOMI(this->FTOP(), rhs);
        }
    }

    inline void regcall FUCOMI(long double& lhs, long double rhs) {

    }
    
    inline void regcall FUCOMI(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FUCOMI(this->FTOP(), rhs);
        }
    }

    inline void regcall FFREE(uint8_t index) {

    }

    // TODO: Read microcode dump to confirm accurate behavior of BCD, there's reason to doubt official docs here
    inline void regcall AAA() {
        if (this->auxiliary || (this->al & 0xF) > 9) {
            this->auxiliary = this->carry = true;
            if constexpr (!OLD_AAA) {
                this->ax += 0x106;
            } else {
                this->al += 6;
                this->ah += 1;
            }
        } else {
            this->carry = false;
        }
        this->al &= 0xF;
    }

    inline void regcall AAS() {
        if (this->auxiliary || (this->al & 0xF) > 9) {
            this->auxiliary = this->carry = true;
            this->ax -= 0x106; // Was this different on 8086 too?
        } else {
            this->carry = false;
        }
        this->al &= 0xF;
    }

    // Supposedly better implementation than official docs: https://www.righto.com/2023/01/understanding-x86s-decimal-adjust-after.html
    inline void regcall DAA() {
        uint8_t temp = this->al;
        if (this->auxiliary || (temp & 0xF) > 9) {
            this->auxiliary = true;
            this->al -= 0x06;
        }
        if (this->carry || temp > 0x99) {
            this->carry = true;
            this->al -= 0x60;
        }
        this->update_pzs(this->al);
    }

    inline void regcall DAS() {
        uint8_t temp = this->al;
        if (this->auxiliary || (temp & 0xF) > 9) {
            this->auxiliary = true;
            this->al += 0x06;
        }
        if (this->carry || temp > 0x99) {
            this->carry = true;
            this->al += 0x60;
        }
        this->update_pzs(this->al);
    }

    // Note: NEC only handles default imm of 10
    inline bool regcall AAM(uint8_t imm) {
        if (imm) {
            this->ah = this->al / imm;
            this->al %= imm;
        }
        else {
            if constexpr (!AAM_NO_DE) {
                return this->set_fault(IntDE);
            }
            else {
                // https://news.ycombinator.com/item?id=34334799
                this->ah = 0xFF;
                // Assuming v20 replicates 80186
            }
        }
        this->update_pzs(this->al);
        return false;
    }

    inline void regcall AAD(uint8_t imm) {
        uint8_t temp = this->al + this->ah * imm;
        this->ax = temp;
        this->update_pzs(temp);
    }

    template <typename T>
    inline bool regcall BOUND(T index, T lower, T upper) {
        using S = std::make_signed_t<T>;

        // (S)index < (S)lower || (S)index > (S)upper
        if constexpr (REP_BOUND) {
            if (
                (S)index < (S)lower ||
                (
                    expect(!this->has_rep(), true) &&
                    (S)index > (S)upper
                )
            ) {
                this->set_fault(IntBR);
                return true;
            }
        }
        else {
            if (!in_range_inclusive<S>(index, lower, upper)) {
                this->set_fault(IntBR);
                return true;
            }
        }
        return false;
    }

    template <typename T, typename P>
    inline bool regcall MASKMOV_impl(T& src, T mask);

    template <typename T>
    inline bool regcall MASKMOV(T& src, T mask) {
        if constexpr (bits > 16) {
            if (this->addr_size_32()) {
                return this->MASKMOV_impl<T, uint32_t>(src, mask);
            }
            if constexpr (bits == 64) {
                if (this->addr_size_64()) {
                    return this->MASKMOV_impl<T, uint64_t>(src, mask);
                }
            }
        }
        return this->MASKMOV_impl<T, uint16_t>(src, mask);
    }

    template <typename T>
    bool regcall PACKSS(T& dst, T src) {
        constexpr size_t src_vec_length = vector_length_v<T>;
        constexpr size_t dst_vec_length = src_vec_length * 2;
        using src_int = vector_type_t<T>;
        using dst_int = hlf_int_t<src_int>;

        vec<src_int, dst_vec_length> temp;
        for (size_t i = 0; i < src_vec_length; ++i) {
            temp[i] = dst[i];
            temp[i + src_vec_length] = src[i];
        }
        
        const vec<src_int, dst_vec_length> min_vec = vec_broadcast<src_int, dst_vec_length>((std::numeric_limits<dst_int>::min)());
        const vec<src_int, dst_vec_length> max_vec = vec_broadcast<src_int, dst_vec_length>((std::numeric_limits<dst_int>::max)());
        temp = __builtin_elementwise_min(temp, max_vec);
        temp = __builtin_elementwise_max(temp, min_vec);
        dst = std::bit_cast<T>(convertvec(temp, vec<dst_int, dst_vec_length>));
        return false;
    }

    template <typename T>
    bool regcall PACKUS(T& dst, T src) {
        constexpr size_t src_vec_length = vector_length_v<T>;
        constexpr size_t dst_vec_length = src_vec_length * 2;
        using src_int = vector_type_t<T>;
        using dst_int = std::make_unsigned_t<hlf_int_t<src_int>>;

        vec<src_int, dst_vec_length> temp;
        for (size_t i = 0; i < src_vec_length; ++i) {
            temp[i] = dst[i];
            temp[i + src_vec_length] = src[i];
        }
        
        const vec<src_int, dst_vec_length> min_vec = {};
        const vec<src_int, dst_vec_length> max_vec = vec_broadcast<src_int, dst_vec_length>((std::numeric_limits<dst_int>::max)());
        temp = __builtin_elementwise_min(temp, max_vec);
        temp = __builtin_elementwise_max(temp, min_vec);
        dst = std::bit_cast<T>(convertvec(temp, vec<dst_int, dst_vec_length>));
        return false;
    }

    template <typename T>
    T regcall PUNPCKL(T dst, T src) {
        constexpr size_t vec_length = vector_length_v<T>;
        constexpr size_t half_vec_length = vec_length / 2;
        using src_int = vector_type_t<T>;

        vec<src_int, vec_length> ret;
        for (size_t i = 0; i < half_vec_length; ++i) {
            ret[i * 2] = dst[i];
            ret[i * 2 + 1] = src[i];
        }
        return ret;
    }

    template <typename T>
    T regcall PUNPCKH(T dst, T src) {
        constexpr size_t vec_length = vector_length_v<T>;
        constexpr size_t half_vec_length = vec_length / 2;
        using src_int = vector_type_t<T>;

        vec<src_int, vec_length> ret;
        for (size_t i = 0; i < half_vec_length; ++i) {
            ret[i * 2] = dst[half_vec_length + i];
            ret[i * 2 + 1] = src[half_vec_length + i];
        }
        return ret;
    }

    template <typename T>
    T regcall PAVG(T dst, T src) {
        constexpr size_t vec_length = vector_length_v<T>;
        using src_int = vector_type_t<T>;
        using tmp_int = dbl_int_t<src_int>;

        vec<tmp_int, vec_length> temp = convertvec(dst, vec<tmp_int, vec_length>);
        temp += convertvec(src, vec<tmp_int, vec_length>);
        temp += 1;
        temp >>= 1;
        return convertvec(temp, T);
    }

    template <typename T>
    T regcall PABS(T src) {
        return __builtin_elementwise_abs(src);
    }

    template <typename T>
    T regcall PADD(T dst, T src) {
        return dst + src;
    }

    template <typename T>
    T regcall PADDS(T dst, T src) {
        return __builtin_elementwise_add_sat(dst, src);
    }

    template <typename T>
    T regcall PHADD(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return odds + evens;
    }

    template <typename T>
    T regcall PHADDS(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return __builtin_elementwise_add_sat(odds, evens);
    }

    template <typename T>
    T regcall PSAD(T dst, T src) {
        constexpr size_t src_vec_length = vector_length_v<T>;
        constexpr size_t dst_vec_length = src_vec_length / 2;
        using src_int = vector_type_t<T>;
        using dst_int = dbl_int_t<src_int>;

        T temp = __builtin_elementwise_abs(dst - src);

        vec<dst_int, dst_vec_length> ret = {};
        for (size_t i = 0; i < dst_vec_length / 4; ++i) {
            ret[i * 4] = (dst_int)temp[i * 4] + (dst_int)temp[i * 4 + 1] + (dst_int)temp[i * 4 + 2] + (dst_int)temp[i * 4 + 3];
        }
        return std::bit_cast<T>(ret);
    }

    template <typename T>
    T regcall PSUB(T dst, T src) {
        return dst - src;
    }

    template <typename T>
    T regcall PSUBS(T dst, T src) {
        return __builtin_elementwise_sub_sat(dst, src);
    }

    template <typename T>
    T regcall PHSUB(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return odds - evens;
    }

    template <typename T>
    T regcall PHSUBS(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return __builtin_elementwise_sub_sat(odds, evens);
    }

    template <typename T>
    T regcall PMADD(T dst, T src) {
        constexpr size_t src_vec_length = vector_length_v<T>;
        constexpr size_t dst_vec_length = src_vec_length / 2;
        using src_int = vector_type_t<T>;
        using dst_int = dbl_int_t<src_int>;

        vec<dst_int, src_vec_length> temp = convertvec(dst, vec<dst_int, src_vec_length>);
        temp *= convertvec(src, vec<dst_int, src_vec_length>);
        
        vec<dst_int, dst_vec_length> ret;
        for (size_t i = 0; i < dst_vec_length; ++i) {
            ret[i] = temp[i * 2] + temp[i * 2 + 1];
        }
        return std::bit_cast<T>(ret);
    }

    template <typename T>
    T regcall PMULL(T dst, T src) {
        return dst * src;
    }

    template <typename T>
    T regcall PMULH(T dst, T src) {
        constexpr size_t vec_length = vector_length_v<T>;
        using src_int = vector_type_t<T>;
        using tmp_int = dbl_int_t<src_int>;

        vec<tmp_int, vec_length> temp = convertvec(dst, vec<tmp_int, vec_length>);
        temp *= convertvec(src, vec<tmp_int, vec_length>);
        temp >>= bitsof(src_int);
        return convertvec(temp, T);
    }

    template <typename T>
    T regcall PMUL(T dst, T src) {
        constexpr size_t src_vec_length = vector_length_v<T>;
        constexpr size_t dst_vec_length = src_vec_length / 2;
        using src_int = vector_type_t<T>;
        using dst_int = dbl_int_t<src_int>;

        vec<dst_int, dst_vec_length> ret = {};
        for (size_t i = 0; i < dst_vec_length; ++i) {
            ret[i] = (dst_int)dst[i * 2] * (dst_int)src[i * 2];
        }
        return std::bit_cast<T>(ret);
    }
    
    template <typename T1, typename T2>
    T1 regcall PSHL(T1 dst, T2 src) {
        return dst << src;
    }

    template <typename T1, typename T2>
    T1 regcall PSHR(T1 dst, T2 src) {
        return dst >> src;
    }

    template <typename T>
    T regcall PAND(T dst, T src) {
        return dst & src;
    }

    template <typename T>
    T regcall PANDN(T dst, T src) {
        return ~dst & src;
    }

    template <typename T>
    T regcall POR(T dst, T src) {
        return dst | src;
    }

    template <typename T>
    T regcall PXOR(T dst, T src) {
        return dst ^ src;
    }

    template <typename T>
    T regcall PMAX(T dst, T src) {
        return __builtin_elementwise_max(dst, src);
    }

    template <typename T>
    T regcall PMIN(T dst, T src) {
        return __builtin_elementwise_max(dst, src);
    }

    template <typename T>
    T regcall PCMPEQ(T dst, T src) {
        return dst == src;
    }

    template <typename T>
    T regcall PCMPGT(T dst, T src) {
        return dst > src;
    }

    template <typename T>
    T regcall PSIGN(T dst, T src) {
        const T zero = {};
        return src == 0 ? zero : src > 0 ? dst : -dst;
    }

    inline void regcall software_interrupt(uint8_t number) {
        this->pending_sinterrupt = number;
    }

    inline bool regcall set_fault(uint8_t number) {
        this->software_interrupt(number);
        return !FAULTS_ARE_TRAPS;
    }

    inline void regcall set_trap(uint8_t number) {
        return this->software_interrupt(number);
    }

// Constants for returning from op wrappers
#define OP_NOT_MEM  ((uint8_t)0)
#define OP_NO_WRITE ((uint8_t)0)
#define OP_WRITE    ((uint8_t)1)
#define OP_NO_FAULT ((uint8_t)0)
#define OP_FAULT    ((uint8_t)2)

#define OP_NEEDS_WRITE(...) ((__VA_ARGS__)&1)
#define OP_HAD_FAULT(...)   ((__VA_ARGS__)&2)

    template <bool is_byte = false, typename L>
    inline void regcall binopAR(uint32_t index, const L& lambda) {
        assume(index < 8);
        if constexpr (is_byte) {
            return lambda(this->al, this->index_byte_regMB(index));
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return lambda(this->eax, this->index_dword_regMB(index));
                }
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return lambda(this->rax, this->index_qword_regMB(index));
                }
            }
            return lambda(this->ax, this->index_word_regMB(index));
        }
    }

    template <bool is_byte = false, typename P, typename L>
    inline void regcall binopAI(P& pc, const L& lambda) {
        if constexpr (is_byte) {
            return lambda(this->al, pc.read_advance<uint8_t>());
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return lambda(this->eax, pc.read_advance<uint32_t>());
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return lambda(this->rax, (uint64_t)(int64_t)pc.read_advance<int32_t>());
                    }
                }
            }
            return lambda(this->ax, pc.read_advance<uint16_t>());
        }
    }

    template <bool is_byte = false, typename P, typename L>
    inline void regcall binopAO(P& pc, const L& lambda) {
        auto offset = pc.read_advance_O();
        if constexpr (is_byte) {
            return lambda(this->al, offset);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return lambda(this->eax, offset);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return lambda(this->rax, offset);
                    }
                }
            }
            return lambda(this->ax, offset);
        }
    }

    template <bool is_byte = false, typename P>
    inline void regcall MOV_RI(P& pc, uint32_t index) {
        assume(index < 8);
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

    template <typename T1, typename T2 = T1, typename P, typename L>
    inline bool regcall binopMR_impl(P& pc, const L& lambda);

    template <typename T, typename P, typename L>
    inline bool regcall MOVX(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopRM_impl<uint32_t, T>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopRM_impl<uint64_t, T>(pc, lambda);
                }
            }
        }
        return this->binopRM_impl<uint16_t, T>(pc, lambda);
    }

    template <bool is_byte = false, typename P, typename L>
    inline bool regcall binopMR(P& pc, const L& lambda) {
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

    template <typename T1, typename T2 = T1, typename P, typename L>
    inline bool regcall binopRM_impl(P& pc, const L& lambda);

    template <bool is_byte = false, typename P, typename L>
    inline bool regcall binopRM(P& pc, const L& lambda) {
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
    inline bool regcall binopMRB_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline bool regcall binopMRB(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopMRB_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopMRB_impl<uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopMRB_impl<uint16_t>(pc, lambda);
    }

    template <typename T, typename P, typename L>
    inline bool regcall binopRMF_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline bool regcall binopRMF(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopRMF_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopRMF_impl<uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopRMF_impl<uint16_t>(pc, lambda);
    }

    template <typename T, typename P, typename L>
    inline bool regcall binopRM2_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline bool regcall binopRM2(P& pc, const L& lambda) {
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
    inline bool regcall binopMS_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline bool regcall binopMS(P& pc, const L& lambda) {
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
    inline bool regcall binopSM_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline bool regcall binopSM(P& pc, const L& lambda) {
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

    // Mm <- Rm
    template <typename T = uint64_t, typename P, typename L>
    inline bool regcall binopMR_MM(P& pc, const L& lambda);

    // Rm <- Mm
    template <typename T = uint64_t, typename P, typename L>
    inline bool regcall binopRM_MM(P& pc, const L& lambda);

    // Mx <- Rx
    template <typename T = uint128_t, typename P, typename L>
    inline bool regcall binopMR_XX(P& pc, const L& lambda);

    // Rx <- Mx
    template <typename T = uint128_t, typename P, typename L>
    inline bool regcall binopRM_XX(P& pc, const L& lambda);

    // Rm <- Mx
    template <typename T, typename P, typename L>
    inline bool regcall binopRM_MX(P& pc, const L& lambda);

    // Rx <- Mm
    template <typename T, typename P, typename L>
    inline bool regcall binopRM_XM(P& pc, const L& lambda);

    template <typename T, typename P, typename L>
    inline bool regcall unopM_impl(P& pc, const L& lambda);

    template <bool is_byte = false, typename P, typename L>
    inline bool regcall unopM(P& pc, const L& lambda) {
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
    inline bool regcall unopMS_impl(P& pc);

    template <bool is_byte = false, typename P>
    inline bool regcall unopMS(P& pc) {
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

    template <typename T, typename P, typename L>
    inline bool regcall unopMW_impl(P& pc, const L& lambda);

    template <typename P, typename L>
    inline bool regcall unopMW(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->unopMW_impl<uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->unopMW_impl<uint64_t>(pc, lambda);
                }
            }
        }
        return this->unopMW_impl<uint16_t>(pc, lambda);
    }

    template <typename T, typename P, typename LM, typename LR>
    inline bool regcall unopMM_impl(P& pc, const LM& lambdaM, const LR& lambdaR);

    template <bool is_byte = false, typename P, typename LM, typename LR>
    inline bool regcall unopMM(P& pc, const LM& lambdaM, const LR& lambdaR) {
        if constexpr (is_byte) {
            return this->unopMM_impl<uint8_t>(pc, lambdaM, lambdaR);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->unopMM_impl<uint32_t>(pc, lambdaM, lambdaR);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->unopMM_impl<uint64_t>(pc, lambdaM, lambdaR);
                    }
                }
            }
            return this->unopMM_impl<uint16_t>(pc, lambdaM, lambdaR);
        }
    }
};

template <z86CoreType core_type, uint64_t flagsA = 0>
struct z86Core;

// Software exceptions
// NMI
// Maskable interrupts
// Trap flag
template <uint64_t flagsA>
struct z86Core<z8086, flagsA> :
    z86Base<
        16, 16, flagsA |
        FLAG_PUSH_CS | FLAG_SAL_IS_SETMO | FLAG_REP_INVERT_MUL | FLAG_REP_INVERT_IDIV | FLAG_FAULTS_ARE_TRAPS | FLAG_NO_UD | FLAG_SINGLE_MEM_WRAPS | FLAG_UNMASK_SHIFTS | FLAG_OLD_PUSH_SP | FLAG_OLD_RESET_PC | FLAG_OLD_AAA | FLAG_WRAP_SEGMENT_MODRM
    > {};

template <uint64_t flagsA>
struct z86Core<z80186, flagsA> :
    z86Base<16, 16, flagsA |
        FLAG_REP_INVERT_IDIV | FLAG_SINGLE_MEM_WRAPS | FLAG_UNMASK_SHIFTS | FLAG_OLD_PUSH_SP | FLAG_OLD_RESET_PC | FLAG_OLD_AAA | FLAG_AAM_NO_DE | FLAG_UNMASK_ENTER | FLAG_REP_BOUND | FLAG_REP_MUL_MISSTORE |
        FLAG_OPCODES_80186
    > {};

// Software exceptions
// Trap flag
// NMI
// Coprocessor overrun
// Maskable interrupts
// INT instruction
template <uint64_t flagsA>
struct z86Core<z80286, flagsA> :
    z86Base<
        16, 16, flagsA |
        FLAG_PROTECTED_MODE |
        FLAG_OPCODES_80186 | FLAG_OPCODES_80286
    > {};

template <uint64_t flagsA>
struct z86Core<zNV30, flagsA> :
    z86Base<
        16, 16, flagsA |
        FLAG_UNMASK_SHIFTS | FLAG_OLD_PUSH_SP | FLAG_OLD_RESET_PC | FLAG_UNMASK_ENTER |
        FLAG_OPCODES_80186 | FLAG_OPCODES_80286 | FLAG_OPCODES_V20
    > {};

template <uint64_t flagsA>
struct z86Core<z80386, flagsA> :
    z86Base<
        32, 32, flagsA |
        FLAG_PROTECTED_MODE |
        FLAG_OPCODES_80186 | FLAG_OPCODES_80286 | FLAG_OPCODES_80386
    > {};

#endif