#pragma once

#ifndef Z86_CORE_INTERNAL_PRE_H
#define Z86_CORE_INTERNAL_PRE_H 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

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

// Is there any way to convince clang to generate LAHF to get the aux flag?

/*

Things that I don't want/plan to implement:
- Shadow stacks

*/

#include "../zero/util.h"

#define TYPEOF(...) std::remove_reference_t<decltype(__VA_ARGS__)>
#define STYPEOF(...) make_signed_ex_t<TYPEOF(__VA_ARGS__)>
#define UTYPEOF(...) make_signed_ex_t<TYPEOF(__VA_ARGS__)>

template <typename T>
using IMMS32 = make_signed_ex_t<std::conditional_t<sizeof(T) != sizeof(uint64_t), T, uint32_t>>;
template <typename T>
using IMMZ32 = make_unsigned_ex_t<std::conditional_t<sizeof(T) != sizeof(uint64_t), T, uint32_t>>;

// Signed word immediate for type
#define TYPE_IMMs(...) IMMS32<TYPEOF(__VA_ARGS__)>

#define FAULT_CHECK(...) if (expect((__VA_ARGS__), false)) { goto fault; }

#if __cpp_if_consteval
#define constevalexpr constexpr
#else
#define constevalexpr
#endif

// This fixes debug builds not compiling
#if NDEBUG
#define z86call regcall
#else
#define z86call fastcall
#endif

#define USE_BITFIELDS 1
#define USE_VECTORS 1

#if _M_IX86 || __x86_64__
#define USE_NATIVE_INSTRS 1
#else
#define USE_NATIVE_INSTRS 0
#endif

#define MORE_RELIABLE_BREAKPOINTS 0

#undef REX

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
    z80486 = 5,
    z86_64
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
    QUIRK_FLAG(FLAG_MIN_DIV_FAULT),         // 8086
    QUIRK_FLAG(FLAG_FAULTS_ARE_TRAPS),      // 8086
    QUIRK_FLAG(FLAG_DE_IS_TRAP),            // 8086, 80186
    QUIRK_FLAG(FLAG_NO_UD),                 // 8086
    QUIRK_FLAG(FLAG_SINGLE_MEM_WRAPS),      // 8086, 80186
    QUIRK_FLAG(FLAG_UNMASK_SHIFTS),         // 8086, v20
    QUIRK_FLAG(FLAG_OLD_PUSH_SP),           // 8086, 80186, v20
    QUIRK_FLAG(FLAG_OLD_RESET_PC),          // 8086, 80186, v20
    QUIRK_FLAG(FLAG_OLD_AAA),               // 8086, 80186 (v20 unknown)
    QUIRK_FLAG(FLAG_WRAP_SEGMENT_MODRM),    // 8086, 80186
    QUIRK_FLAG(FLAG_AAM_NO_DE),             // 80186
    QUIRK_FLAG(FLAG_UNMASK_ENTER),          // 80186, v20
    QUIRK_FLAG(FLAG_REP_BOUND),             // 80186
    QUIRK_FLAG(FLAG_REP_MUL_MISSTORE),      // 80186
    QUIRK_FLAG(FLAG_PROTECTED_MODE),        // 80286+
    QUIRK_FLAG(FLAG_XBTS_IBTS),             // 80386
    QUIRK_FLAG(FLAG_PAGING),                // 80386+
    QUIRK_FLAG(FLAG_OLD_CMPXCHG),           // 80486
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

template <size_t bytes, bool needs_a20 = (bytes > 1_MB)>
struct z86Memory {
    cache_align unsigned char raw[bytes];

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
    inline void z86call write(size_t offset, const T& value) {
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

    inline size_t write_file(FILE* dst, size_t src, size_t length) const {
        if (src < bytes) {
            length = (std::min)(bytes - src, length);
            size_t success_length = fwrite(&this->raw[src], 1, length, dst);
            return (std::min)(length, success_length);
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

    inline size_t read_file(size_t dst, FILE* src, size_t length) {
        if (dst < bytes) {
            length = (std::min)(bytes - dst, length);
            size_t success_length = fread(&this->raw[dst], 1, length, src);
            return (std::min)(length, success_length);
        }
        return 0;
    }

    /*
    inline size_t read_file(size_t dst, FILE* src) {
        long current_pos = ftell(src);
        fseek(src, 0, SEEK_END);
        long remaining_size = ftell(src) - current_pos;
        fseek(src, current_pos, SEEK_SET);
        return this->read_file(dst, src, remaining_size);
    }
    */
};

// TODO: Investigate whether or not it's worth just compiling
// a whole separate core with/without the A20 gate and switching
// between them at runtime rather than branching based on the gate.
// That way extra expensive jank can be enabled if it's ever necessary
// and everyone else can just enable the dang gate in the first
// five seconds and pretend this abomination doesn't exist.
//
// Until the core is split, I'm *not* implementing anything to
// split a single memory access across the A20 gate. It's either
// going to be all below or all wrapping. Because screw that noise.
template <size_t bytes>
struct z86Memory<bytes, true> : z86Memory<bytes, false> {
    static inline constexpr size_t A20_MASK = ~1_MB;

    //bool a20;
    size_t a20 = A20_MASK;


    using z86Memory<bytes, false>::write;
    using z86Memory<bytes, false>::read;


    inline size_t a20_mask(size_t offset) const {
        return offset & this->a20;
        /*
        if (expect(!this->a20, false)) {
            offset &= A20_MASK;
        }
        return offset;
        */
    }

    template <typename T = uint8_t>
    inline T* ptr(size_t offset) {
        return this->z86Memory<bytes, false>::ptr<T>(this->a20_mask(offset));
    }

    template <typename T = uint8_t>
    inline const T* ptr(size_t offset) const {
        return this->z86Memory<bytes, false>::ptr<T>(this->a20_mask(offset));
    }

    template <typename T = uint8_t>
    inline T& ref(size_t offset) {
        return this->z86Memory<bytes, false>::ref<T>(this->a20_mask(offset));
    }

    template <typename T = uint8_t>
    inline const T& ref(size_t offset) const {
        return this->z86Memory<bytes, false>::ref<T>(this->a20_mask(offset));
    }

    template <typename T = uint8_t>
    inline T read(size_t offset) const {
        return this->z86Memory<bytes, false>::read<T>(this->a20_mask(offset));
    }

    template <typename T = uint8_t>
    inline void z86call write(size_t offset, const T& value) {
        return this->z86Memory<bytes, false>::write<T>(this->a20_mask(offset), value);
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

using EXCEPTION = bool;
static inline constexpr EXCEPTION FAULT = true;
static inline constexpr EXCEPTION NO_FAULT = false;
//#define FAULTED(...) (__VA_ARGS__)
static inline constexpr bool FAULTED(EXCEPTION state) {
    return state;
}

enum GATE_TYPE : int8_t {
    CALL_GATE = 0,
    TASK_GATE = 1,
    INTERRUPT_GATE = 2,
    TRAP_GATE = 3
};

enum DESCRIPTOR_TYPE5 : uint8_t {
    DESCRIPTOR_EMPTY            = 0b00000,
    DESCRIPTOR_TSS16            = 0b00001,
    DESCRIPTOR_LDT              = 0b00010,
    DESCRIPTOR_TSS16_BUSY       = 0b00011,
    DESCRIPTOR_CALL_GATE16      = 0b00100,
    DESCRIPTOR_TASK_GATE        = 0b00101,
    DESCRIPTOR_INTERRUPT_GATE16 = 0b00110,
    DESCRIPTOR_TRAP_GATE16      = 0b00111,
    DESCRIPTOR_UNUSED_8         = 0b01000,
    DESCRIPTOR_TSS              = 0b01001,
    DESCRIPTOR_UNUSED_10        = 0b01010,
    DESCRIPTOR_TSS_BUSY         = 0b01011,
    DESCRIPTOR_CALL_GATE        = 0b01100,
    DESCRIPTOR_UNUSED_13        = 0b01101,
    DESCRIPTOR_INTERRUPT_GATE   = 0b01110,
    DESCRIPTOR_TRAP_GATE        = 0b01111,
    DESCRIPTOR_R_DATA           = 0b10000,
    DESCRIPTOR_R_DATAA          = 0b10001,
    DESCRIPTOR_RW_DATA          = 0b10010,
    DESCRIPTOR_RW_DATAA         = 0b10011,
    DESCRIPTOR_R_EDATA          = 0b10100,
    DESCRIPTOR_R_EDATAA         = 0b10101,
    DESCRIPTOR_RW_EDATA         = 0b10110,
    DESCRIPTOR_RW_EDATAA        = 0b10111,
    DESCRIPTOR_E_CODE           = 0b11000,
    DESCRIPTOR_E_CODEA          = 0b11001,
    DESCRIPTOR_ER_CODE          = 0b11010,
    DESCRIPTOR_ER_CODEA         = 0b11011,
    DESCRIPTOR_E_CCODE          = 0b11100,
    DESCRIPTOR_E_CCODEA         = 0b11101,
    DESCRIPTOR_ER_CCODE         = 0b11110,
    DESCRIPTOR_ER_CCODEA        = 0b11111,
};

enum DESCRIPTOR_MASK : uint32_t {
    DESCRIPTORM_EMPTY            = 1u << 0,
    DESCRIPTORM_TSS16            = 1u << 1,
    DESCRIPTORM_LDT              = 1u << 2,
    DESCRIPTORM_TSS16_BUSY       = 1u << 3,
    DESCRIPTORM_CALL_GATE16      = 1u << 4,
    DESCRIPTORM_TASK_GATE        = 1u << 5,
    DESCRIPTORM_INTERRUPT_GATE16 = 1u << 6,
    DESCRIPTORM_TRAP_GATE16      = 1u << 7,
    DESCRIPTORM_UNUSED_8         = 1u << 8,
    DESCRIPTORM_TSS              = 1u << 9,
    DESCRIPTORM_UNUSED_10        = 1u << 10,
    DESCRIPTORM_TSS_BUSY         = 1u << 11,
    DESCRIPTORM_CALL_GATE        = 1u << 12,
    DESCRIPTORM_UNUSED_13        = 1u << 13,
    DESCRIPTORM_INTERRUPT_GATE   = 1u << 14,
    DESCRIPTORM_TRAP_GATE        = 1u << 15,
    DESCRIPTORM_R_DATA           = 1u << 16,
    DESCRIPTORM_R_DATAA          = 1u << 17,
    DESCRIPTORM_RW_DATA          = 1u << 18,
    DESCRIPTORM_RW_DATAA         = 1u << 19,
    DESCRIPTORM_R_EDATA          = 1u << 20,
    DESCRIPTORM_R_EDATAA         = 1u << 21,
    DESCRIPTORM_RW_EDATA         = 1u << 22,
    DESCRIPTORM_RW_EDATAA        = 1u << 23,
    DESCRIPTORM_E_CODE           = 1u << 24,
    DESCRIPTORM_E_CODEA          = 1u << 25,
    DESCRIPTORM_ER_CODE          = 1u << 26,
    DESCRIPTORM_ER_CODEA         = 1u << 27,
    DESCRIPTORM_E_CCODE          = 1u << 28,
    DESCRIPTORM_E_CCODEA         = 1u << 29,
    DESCRIPTORM_ER_CCODE         = 1u << 30,
    DESCRIPTORM_ER_CCODEA        = 1u << 31,
};

template <size_t bits>
struct SEG_DESCRIPTOR {
    union {
        uint16_t limit_low;
        uint16_t gate_ip;
    };
    union {
        uint16_t base_low;
        uint16_t gate_seg;
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
        struct {
            uint8_t : 1;
            uint8_t busy : 1;
        } tss;
    };

    inline constexpr uint16_t segment() const {
        return this->gate_seg;
    }

    inline constexpr uint16_t ip() const {
        return this->gate_ip;
    }

    inline constexpr void set_accessed() {
        this->data.accessed = true;
    }

    inline constexpr void set_busy() {
        this->tss.busy = true;
    }

    inline constexpr uint8_t get_dpl() const {
        return this->dpl;
    }

    inline constexpr uint8_t get_full_type() const {
        return this->type;
    }

    inline constexpr bool is_system() const {
        return this->type < 0b10000;
    }

    inline constexpr bool is_code() const {
        return this->type >= 0b11000;
    }

    inline constexpr bool is_data() const {
        return this->type - 0b10000u < 0b1000u;
    }

    inline constexpr int8_t gate_type() const {
        return (this->type & 0b10111) - 4;
    }

    inline constexpr bool is_gate() const {
        return (uint8_t)this->gate_type() < 3;
    }

    inline constexpr bool is_present() const {
        return this->present;
    }

    inline constexpr bool is_long_mode() const {
        return false;
    }
    inline constexpr bool is_big() const {
        return false;
    }
    inline constexpr bool is_invalid_long_bits() const {
        return false;
    }
    inline constexpr bool is_valid_long_bits() const {
        return false;
    }
    inline constexpr uint8_t get_long_bits() const {
        return 0;
    }
    inline constexpr bool is_valid_for_stack() const {
        switch (this->type) {
            default:
                return false;
            case DESCRIPTOR_RW_DATA: case DESCRIPTOR_RW_DATAA:
                return true;
        }
    }
};

template <>
struct SEG_DESCRIPTOR<16> : SEG_DESCRIPTOR<0> {
    uint16_t must_be_zero;

    inline constevalexpr uint32_t base() const {
#if __cpp_if_consteval
        if consteval {
            return this->base_low | this->base_middle << 16;
        } else
#endif
        {
            return *(uint32_t*)&this->base_low & 0xFFFFFF;
        }
    }

    inline constexpr uint16_t limit() const {
        return this->limit_low;
    }

    inline constexpr uint16_t attributes(uint16_t extra = 0) const {
        return this->flags1 | extra;
    }

    inline constexpr uint8_t access_rights() const {
        return this->flags1;
    }

    inline constexpr uint16_t eip() const {
        return this->ip();
    }
    inline constexpr uint16_t rip() const {
        return this->ip();
    }
};
static_assert(sizeof(SEG_DESCRIPTOR<16>) == 0x8);

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
                struct {
                    uint8_t : 5;
                    uint8_t long_bits : 2;
                };
            };
            uint8_t base_high;
        };
        uint16_t gate_ip_high;
    };

    inline constevalexpr uint32_t base() const {
#if __cpp_if_consteval
        if consteval {
            return this->base_low | this->base_middle << 16 | this->base_high << 24;
        } else
#endif
        {
#if USE_VECTORS
            const vec<uint8_t, 8> seg = *(vec<uint8_t, 8>*)this;
            const vec<uint8_t, 8> zero = {};
            vec<uint8_t, 8> ret = shufflevec(seg, zero, 2, 3, 4, 7, 8, 8, 8, 8);
            return std::bit_cast<vec<uint32_t, 2>>(ret)[0];
#else
            return (*(uint32_t*)&this->base_low & 0xFFFFFF) | (uint32_t)this->base_high << 24;
#endif
        }
    }

    inline constexpr uint32_t limit() const {
        uint32_t limit = (uint32_t)this->limit_low | (uint32_t)this->limit_high << 16;
        if (this->granularity) {
            limit <<= 12;
            limit |= 0xFFF;
        }
        return limit;
    }

    inline constevalexpr uint16_t attributes(uint16_t extra = 0) const {
#if __cpp_if_consteval
        if consteval {
            return this->flags1 | (this->flags2 & 0xF0) << 16 | extra;
        } else
#endif
        {
            return (*(uint16_t*)&this->flags1 & 0xF0FF) | extra;
        }
    }

    inline constevalexpr uint16_t access_rights() const {
#if __cpp_if_consteval
        if consteval {
            return this->flags1 | this->flags2 << 16;
        } else
#endif
        {
            return *(uint16_t*)&this->flags1;
        }
    }

    inline constexpr uint32_t eip() const {
        return (uint32_t)this->gate_ip | (uint32_t)this->gate_ip_high << 16;
    }
    inline constexpr uint32_t rip() const {
        return this->eip();
    }

    inline constexpr bool is_long_mode() const {
        return false;
    }
    inline constexpr bool is_big() const {
        return this->big;
    }
    inline constexpr bool is_invalid_long_bits() const {
        return false;
    }
    inline constexpr bool is_valid_long_bits() const {
        return true;
    }
    inline constexpr uint8_t get_long_bits() const {
        return this->big;
    }
};
static_assert(sizeof(SEG_DESCRIPTOR<32>) == 0x8);

template <>
struct SEG_DESCRIPTOR<64> : SEG_DESCRIPTOR<32> {
    union {
        uint32_t base_upper;
        uint32_t gate_ip_upper;
    };
    uint32_t must_be_zero;

    inline constevalexpr uint64_t base() const {
#if __cpp_if_consteval
        if consteval {
            uint64_t ret = this->base_low | this->base_middle << 16 | this->base_high << 24;
            return ret | (uint64_t)this->base_upper << 32;
        } else
#endif
        {
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
            uint64_t ret = *(uint32_t*)&this->base_low & 0xFFFFFF | (uint32_t)this->base_high << 24;
            return ret | (uint64_t)this->base_upper << 32;
#endif
        }
    }

    inline constexpr uint64_t rip() const {
        uint64_t ret = (uint32_t)this->gate_ip | (uint32_t)this->gate_ip_high << 16;
        return ret | (uint64_t)this->gate_ip_upper << 32;
    }

    inline constexpr bool is_long_mode() const {
        return this->long_mode;
    }
    inline constexpr bool is_invalid_long_bits() const {
        return this->long_bits == 3;
    }
    inline constexpr bool is_valid_long_bits() const {
        return this->long_bits == 1;
    }
    inline constexpr uint8_t get_long_bits() const {
        return this->long_bits;
    }
};
static_assert(sizeof(SEG_DESCRIPTOR<64>) == 0x10);

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

enum ACCESS_FLAGS : uint8_t {
    ACCESS_READ     = 0b001,
    ACCESS_WRITE    = 0b010,
    ACCESS_EXEC     = 0b100,
};

static inline constexpr uint16_t ATTRIBUTES_VALID   = 0b0000000010000000;
static inline constexpr uint16_t ATTRIBUTES_NULL    = 0b0000000000000000;
static inline constexpr uint16_t ATTRIBUTES_R       = 0b0000000100000000;
static inline constexpr uint16_t ATTRIBUTES_W       = 0b0000001000000000;
static inline constexpr uint16_t ATTRIBUTES_RW      = 0b0000001100000000;
static inline constexpr uint16_t ATTRIBUTES_E       = 0b0000010000000000;
static inline constexpr uint16_t ATTRIBUTES_RE      = 0b0000010100000000;
static inline constexpr uint16_t ATTRIBUTES_WE      = 0b0000011000000000;
static inline constexpr uint16_t ATTRIBUTES_RWE     = 0b0000011100000000;
static inline constexpr uint16_t ATTRIBUTES_XDOWN   = 0b0000100000000000;

struct z86DescriptorAttributes {
    union {
        uint16_t raw;
        uint8_t flags[2];
        struct {
            union {
                uint8_t flags1;
                struct {
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
            union {
                uint8_t flags2;
                struct {
                    // Internal flag bits
                    uint8_t read : 1;
                    uint8_t write : 1;
                    uint8_t execute : 1;
                    uint8_t expand_down : 1;
                    // Architecture bits
                    uint8_t system_use : 1; // Can I get away with 
                    uint8_t long_mode : 1;
                    uint8_t big : 1;
                    uint8_t granularity : 1;
                };
                struct {
                    uint8_t : 5;
                    uint8_t long_bits : 2;
                };
            };
        };
    };

    inline constexpr bool can_read() const {
        return this->read;
    }
    inline constexpr bool can_write() const {
        return this->write;
    }
    inline constexpr bool can_execute() const {
        return this->execute;
    }
    template <ACCESS_FLAGS access>
    inline constexpr bool check_access() const {
        return (this->flags2 & access) == access;
    }
    inline constexpr bool is_valid() const {
        return this->present;
    }

    template <bool is_code = true>
    static inline constexpr uint16_t make_full_attributes(uint32_t attributes) {
        switch (attributes & 0b11111) {
            default: unreachable;
            // System descriptors, behavior unknown.
            // Currently assuming RW data
            case DESCRIPTOR_EMPTY: case DESCRIPTOR_TSS16: case DESCRIPTOR_LDT: case DESCRIPTOR_TSS16_BUSY:
            case DESCRIPTOR_CALL_GATE16: case DESCRIPTOR_TASK_GATE: case DESCRIPTOR_INTERRUPT_GATE16: case DESCRIPTOR_TRAP_GATE16:
            case DESCRIPTOR_UNUSED_8: case DESCRIPTOR_TSS: case DESCRIPTOR_UNUSED_10: case DESCRIPTOR_TSS_BUSY:
            case DESCRIPTOR_CALL_GATE: case DESCRIPTOR_UNUSED_13: case DESCRIPTOR_INTERRUPT_GATE: case DESCRIPTOR_TRAP_GATE:
            // RW data descriptors
            case DESCRIPTOR_RW_DATA: case DESCRIPTOR_RW_DATAA:
            case DESCRIPTOR_RW_EDATA: case DESCRIPTOR_RW_EDATAA:
                attributes |= ATTRIBUTES_RW;
                break;
            // R data descriptors
            // RE code descriptors
            case DESCRIPTOR_R_DATA: case DESCRIPTOR_R_DATAA:
            case DESCRIPTOR_R_EDATA: case DESCRIPTOR_R_EDATAA:
            case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
            case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                attributes |= ATTRIBUTES_R;
                break;
            // E code descriptors
            case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
            case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                break;
        }
        if constexpr (is_code) {
            attributes |= ATTRIBUTES_E;
        }
        return attributes;
    }
};
static_assert(sizeof(z86DescriptorAttributes) == 0x2);

struct z86DescriptorCacheHardware {};

struct z86DescriptorCache80286 : z86DescriptorCacheHardware {
    uint16_t base_low;
    uint8_t base_high;
    uint8_t attributes_field;
    uint16_t limit_field;

    inline constevalexpr uint32_t base() const {
#if __cpp_if_consteval
        if consteval {
            return this->base_low | this->base_high << 16;
        } else
#endif
        {
            return *(uint32_t*)&this->base_low & 0xFFFFFF;
        }
    }

    inline constexpr uint16_t limit() const {
        return this->limit_field;
    }

    template <bool is_code = true>
    static inline constexpr uint16_t make_full_attributes(uint8_t attributes) {
        return z86DescriptorAttributes::make_full_attributes<is_code>(attributes);
    }

    template <bool is_code = true>
    inline constexpr uint16_t attributes() const {
        return make_full_attributes<is_code>(this->attributes_field);
    }
};
static_assert(sizeof(z86DescriptorCache80286) == 0x6);

struct z86DescriptorCache80386 : z86DescriptorCacheHardware {
    uint8_t : 8;
    union {
        uint8_t flags2;
    };
    uint8_t flags1;
    uint8_t : 8;
    uint32_t base_field;
    uint32_t limit_field;

    inline constexpr uint32_t base() const {
        return this->base_field;
    }

    inline constexpr uint32_t limit() const {
        return this->limit_field;
    }

    template <bool is_code = true>
    static inline constexpr uint16_t make_full_attributes(uint16_t attributes) {
        return z86DescriptorAttributes::make_full_attributes<is_code>(0b0100000011111111 & attributes);
    }

    template <bool is_code = true>
    inline constevalexpr uint16_t attributes() const {
#if __cpp_if_consteval
        if consteval {
            return make_full_attributes<is_code>(this->flags2 << 8 | this->flags1);
        } else
#endif
        {
            return make_full_attributes<is_code>(__builtin_bswap16(*(uint16_t*)&this->flags2));
        }
    }
};
static_assert(sizeof(z86DescriptorCache80386) == 0xC);

struct z86DescriptorCachePentium : z86DescriptorCacheHardware {
    uint32_t limit_field;
    uint32_t base_field;
    uint16_t attributes_field;

    inline constexpr uint32_t base() const {
        return this->base_field;
    }

    inline constexpr uint32_t limit() const {
        return this->limit_field;
    }

    template <bool is_code = true>
    static inline constexpr uint16_t make_full_attributes(uint16_t attributes) {
        return z86DescriptorAttributes::make_full_attributes<is_code>(0b0100000011111111 & attributes);
    }

    template <bool is_code = true>
    inline constexpr uint16_t attributes() const {
        return make_full_attributes<is_code>(this->attributes_field);
    }
};
static_assert(sizeof(z86DescriptorCachePentium) == 0xC);

struct z86DescriptorCachePentiumPro : z86DescriptorCacheHardware {
    uint16_t selector_field;
    uint16_t attributes_field;
    uint32_t limit_field;
    uint32_t base_field;

    inline constexpr uint32_t base() const {
        return this->base_field;
    }

    inline constexpr uint32_t limit() const {
        return this->limit_field;
    }

    template <bool is_code = true>
    static inline constexpr uint16_t make_full_attributes(uint16_t attributes) {
        return z86DescriptorAttributes::make_full_attributes<is_code>(0b0100000011111111 & attributes);
    }

    template <bool is_code = true>
    inline constexpr uint16_t attributes() const {
        return make_full_attributes<is_code>(this->attributes_field);
    }
};
static_assert(sizeof(z86DescriptorCachePentiumPro) == 0xC);

struct z86DescriptorCacheAMD64 : z86DescriptorCacheHardware {
    uint16_t selector_field;
    uint16_t attributes_field;
    uint32_t limit_field;
    uint64_t base_field;

    inline constexpr uint64_t base() const {
        return this->base_field;
    }

    inline constexpr uint32_t limit() const {
        return this->limit_field;
    }

    template <bool is_code = true>
    static inline constexpr uint16_t make_full_attributes(uint16_t attributes) {
        return z86DescriptorAttributes::make_full_attributes<is_code>(0b0100000011111111 & attributes);
    }

    template <bool is_code = true>
    inline constexpr uint16_t attributes() const {
        return make_full_attributes<is_code>(this->attributes_field);
    }
};
static_assert(sizeof(z86DescriptorCacheAMD64) == 0x10);

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
    const z86DescriptorAttributes attributes = {}; // 0x6
    // 0x8

    inline constexpr z86DescriptorCacheBase() = default;
    inline constexpr z86DescriptorCacheBase(uint16_t limit, uint32_t base) : base(base), limit(limit), attributes{} {}
    inline constexpr z86DescriptorCacheBase(uint16_t limit, uint32_t base, uint16_t attributes) : base(base), limit(limit), attributes{ attributes } {}
    inline constexpr z86DescriptorCacheBase(uint16_t limit, uint32_t base, const z86DescriptorAttributes& attributes) : base(base), limit(limit), attributes(attributes) {}

    template <size_t bits>
    inline constexpr z86DescriptorCacheBase(const SEG_DESCRIPTOR<bits>* descriptor)
        : base(descriptor->base()), limit(descriptor->limit()), attributes{ descriptor->attributes() }
    {}
    template <size_t bits>
    inline constexpr z86DescriptorCacheBase(const SEG_DESCRIPTOR<bits>* descriptor, uint16_t extra)
        : base(descriptor->base()), limit(descriptor->limit()), attributes{ descriptor->attributes(extra) }
    {}

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCacheBase(const T& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{ descriptor.attributes() }
    {}

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCacheBase(const T& descriptor, uint16_t attributes)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{ attributes }
    {}

    /*
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCache80286& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCache80386& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCachePentium& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCachePentiumPro& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCacheAMD64& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    */
};

// Size: 0xC
template <>
struct z86DescriptorCacheBase<32> {
    using BT = uint32_t; // Base Type
    using LT = uint32_t; // Limit type

    const uint32_t base = 0; // 0x0
    const uint32_t limit = 0; // 0x4
    const z86DescriptorAttributes attributes = {}; // 0x8
    // 0xA

    inline constexpr z86DescriptorCacheBase() = default;
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint32_t base) : base(base), limit(limit), attributes{} {}
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint32_t base, uint16_t attributes) : base(base), limit(limit), attributes{ attributes } {}
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint32_t base, const z86DescriptorAttributes& attributes) : base(base), limit(limit), attributes(attributes) {}

    template <size_t bits>
    inline constexpr z86DescriptorCacheBase(const SEG_DESCRIPTOR<bits>* descriptor)
        : base(descriptor->base()), limit(descriptor->limit()), attributes{ descriptor->attributes() }
    {}
    template <size_t bits>
    inline constexpr z86DescriptorCacheBase(const SEG_DESCRIPTOR<bits>* descriptor, uint16_t extra)
        : base(descriptor->base()), limit(descriptor->limit()), attributes{ descriptor->attributes(extra) }
    {}

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCacheBase(const T& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{ descriptor.attributes() }
    {}

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCacheBase(const T& descriptor, uint16_t attributes)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{ attributes }
    {}

    /*
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCache80286& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCache80386& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCachePentium& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCachePentiumPro& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCacheAMD64& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    */
};

// Size: 0x10
template <>
struct z86DescriptorCacheBase<64> {
    using BT = uint64_t; // Base Type
    using LT = uint32_t; // Limit type

    const uint64_t base = 0; // 0x0
    const uint32_t limit = 0; // 0x8
    const z86DescriptorAttributes attributes = {}; // 0xC
    // 0xE

    inline constexpr z86DescriptorCacheBase() = default;
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint64_t base) : base(base), limit(limit), attributes{} {}
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint64_t base, uint16_t attributes) : base(base), limit(limit), attributes{ attributes } {}
    inline constexpr z86DescriptorCacheBase(uint32_t limit, uint64_t base, const z86DescriptorAttributes& attributes) : base(base), limit(limit), attributes(attributes) {}
    
    template <size_t bits>
    inline constexpr z86DescriptorCacheBase(const SEG_DESCRIPTOR<bits>* descriptor)
        : base(descriptor->base()), limit(descriptor->limit()), attributes{ descriptor->attributes() }
    {}
    template <size_t bits>
    inline constexpr z86DescriptorCacheBase(const SEG_DESCRIPTOR<bits>* descriptor, uint16_t extra)
        : base(descriptor->base()), limit(descriptor->limit()), attributes{ descriptor->attributes(extra) }
    {}

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCacheBase(const T& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{ descriptor.attributes() }
    {}

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCacheBase(const T& descriptor, uint16_t attributes)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{ attributes }
    {}

    /*
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCache80286& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCache80386& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCachePentium& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCachePentiumPro& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    inline constexpr z86DescriptorCacheBase(const z86DescriptorCacheAMD64& descriptor)
        : base(descriptor.base()), limit(descriptor.limit()), attributes{}
    {}
    */
};

template <size_t max_bits>
struct z86DescriptorCache : z86DescriptorCacheBase<max_bits> {
    using BT = z86DescriptorCacheBase<max_bits>::BT;
    using LT = z86DescriptorCacheBase<max_bits>::LT;

    inline constexpr z86DescriptorCache() = default;
    inline constexpr z86DescriptorCache(LT limit, BT base) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(limit, base) {}
    inline constexpr z86DescriptorCache(LT limit, BT base, uint16_t attributes) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(limit, base, attributes) {}
    inline constexpr z86DescriptorCache(LT limit, BT base, const z86DescriptorAttributes& attributes) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(limit, base, attributes) {}
    template <size_t bits>
    inline constexpr z86DescriptorCache(const SEG_DESCRIPTOR<bits>* descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}
    template <size_t bits>
    inline constexpr z86DescriptorCache(const SEG_DESCRIPTOR<bits>* descriptor, uint16_t extra) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor, extra) {}
    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCache(const T& descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}
    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline constexpr z86DescriptorCache(const T& descriptor, uint16_t attributes) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor, attributes) {}

    /*
    inline constexpr z86DescriptorCache(const z86DescriptorCache80286& descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}
    inline constexpr z86DescriptorCache(const z86DescriptorCache80386& descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}
    inline constexpr z86DescriptorCache(const z86DescriptorCachePentium& descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}
    inline constexpr z86DescriptorCache(const z86DescriptorCachePentiumPro& descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}
    inline constexpr z86DescriptorCache(const z86DescriptorCacheAMD64& descriptor) : z86DescriptorCacheBase<max_bits>::z86DescriptorCacheBase(descriptor) {}
    */

    template <typename T>
    inline constexpr T* get_ptr() const;

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
    inline constexpr SEG_DESCRIPTOR<max_bits>* get_descriptor(uint16_t selector) const;

    inline constexpr void fill_descriptor(z86DescriptorCache80286& out) const {
        out.attributes_field = this->attributes.flags1;
        out.base_low = this->base;
        out.base_high = this->base >> 16;
        out.limit_field = this->limit;
    }
};

// 80286 format
struct z86LoadallFrame80286 {
    uint16_t x0; // 0x0
    uint16_t x1; // 0x2
    uint16_t x2; // 0x4
    uint16_t msw; // 0x6
    uint16_t x3; // 0x8
    uint16_t x4; // 0xA
    uint16_t x5; // 0xC
    uint16_t x6; // 0xE
    uint16_t x7; // 0x10
    uint16_t x8; // 0x12
    uint16_t x9; // 0x14
    uint16_t tr; // 0x16
    uint16_t flags; // 0x18
    uint16_t ip; // 0x1A
    uint16_t ldtr; // 0x1C
    uint16_t ds; // 0x1E
    uint16_t ss; // 0x20
    uint16_t cs; // 0x22
    uint16_t es; // 0x24
    uint16_t di; // 0x26
    uint16_t si; // 0x28
    uint16_t bp; // 0x2A
    uint16_t sp; // 0x2C
    uint16_t bx; // 0x2E
    uint16_t dx; // 0x30
    uint16_t cx; // 0x32
    uint16_t ax; // 0x34
    z86DescriptorCache80286 es_descriptor; // 0x36
    z86DescriptorCache80286 cs_descriptor; // 0x3C
    z86DescriptorCache80286 ss_descriptor; // 0x42
    z86DescriptorCache80286 ds_descriptor; // 0x48
    z86DescriptorCache80286 gdt_descriptor; // 0x4E
    z86DescriptorCache80286 ldt_descriptor; // 0x54
    z86DescriptorCache80286 idt_descriptor; // 0x5A
    z86DescriptorCache80286 tss_descriptor; // 0x60
    // 0x66
};
static_assert(sizeof(z86LoadallFrame80286) == 0x66);

// 80386, 80486 format
struct z86LoadallFrame80386 {
    uint32_t cr0; // 0x0
    uint32_t eflags; // 0x4
    uint32_t eip; // 0x8
    uint32_t edi; // 0xC
    uint32_t esi; // 0x10
    uint32_t ebp; // 0x14
    uint32_t esp; // 0x18
    uint32_t ebx; // 0x1C
    uint32_t edx; // 0x20
    uint32_t ecx; // 0x24
    uint32_t eax; // 0x28
    uint32_t dr6; // 0x2C
    uint32_t dr7; // 0x30
    alignas(4) uint16_t tr; // 0x34
    alignas(4) uint16_t ldtr; // 0x38
    alignas(4) uint16_t gs; // 0x3C
    alignas(4) uint16_t fs; // 0x40
    alignas(4) uint16_t ds; // 0x44
    alignas(4) uint16_t ss; // 0x48
    alignas(4) uint16_t cs; // 0x4C
    alignas(4) uint16_t es; // 0x50
    z86DescriptorCache80386 tss_descriptor; // 0x54
    z86DescriptorCache80386 idt_descriptor; // 0x60
    z86DescriptorCache80386 gdt_descriptor; // 0x6C
    z86DescriptorCache80386 ldt_descriptor; // 0x78
    z86DescriptorCache80386 gs_descriptor; // 0x84
    z86DescriptorCache80386 fs_descriptor; // 0x90
    z86DescriptorCache80386 ds_descriptor; // 0x9C
    z86DescriptorCache80386 ss_descriptor; // 0xA8
    z86DescriptorCache80386 cs_descriptor; // 0xB4
    z86DescriptorCache80386 es_descriptor; // 0xC0
    /*
    // Are these real?
    // Source: http://phg.chat.ru/opcode.txt
    uint32_t : 32; // 0xCC
    uint32_t : 32; // 0xD0
    uint32_t : 32; // 0xD4
    uint32_t : 32; // 0xD8
    uint32_t : 32; // 0xDC
    uint32_t : 32; // 0xE0
    uint32_t : 32; // 0xE4
    uint32_t : 32; // 0xE8
    uint32_t : 32; // 0xEC
    uint32_t : 32; // 0xF0
    uint32_t : 32; // 0xF4
    uint32_t : 32; // 0xF8
    uint32_t : 32; // 0xFC
    uint32_t x0; // 0x100
    uint32_t x1; // 0x104
    uint32_t x2; // 0x108
    uint32_t x3; // 0x10C
    uint32_t x4; // 0x110
    uint32_t x5; // 0x114
    uint32_t x6; // 0x118
    uint32_t x7; // 0x11C
    uint32_t x8; // 0x120
    uint32_t x9; // 0x124
    // 0x128
    */
};
static_assert(sizeof(z86LoadallFrame80386) == 0xCC);

struct z86RsmFramePentium {
    uint32_t smm_base;
    uint32_t smm_revision;
    uint16_t io_restart_flag;
    uint16_t halt_restart_flag;
    uint32_t io_restart_edi;
    uint32_t io_restart_ecx;
    uint32_t io_restart_esi;
    uint32_t io_restart_eip;
    uint32_t : 32;
    uint32_t : 32;
    uint32_t : 32;
    uint32_t : 32;
    uint16_t dr6_alt;
    uint16_t rsm_control;
    uint32_t cr4;
    uint32_t : 32;
    z86DescriptorCachePentium es_descriptor;
    z86DescriptorCachePentium cs_descriptor;
    z86DescriptorCachePentium ss_descriptor;
    z86DescriptorCachePentium ds_descriptor;
    z86DescriptorCachePentium fs_descriptor;
    z86DescriptorCachePentium gs_descriptor;
    z86DescriptorCachePentium ldt_descriptor;
    z86DescriptorCachePentium gdt_descriptor;
    z86DescriptorCachePentium idt_descriptor;
    z86DescriptorCachePentium tss_descriptor;
    alignas(4) uint16_t es;
    alignas(4) uint16_t cs;
    alignas(4) uint16_t ss;
    alignas(4) uint16_t ds;
    alignas(4) uint16_t fs;
    alignas(4) uint16_t gs;
    alignas(4) uint16_t ldtr;
    alignas(4) uint16_t tr;
    uint32_t dr7;
    uint32_t dr6;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t eip;
    uint32_t eflags;
    uint32_t cr3;
    uint32_t cr0;
};

struct z86RsmFrameIntel64 {
    // "reserved"
    uint64_t r15 packed_field;
    uint64_t r14 packed_field;
    uint64_t r13 packed_field;
    uint64_t r12 packed_field;
    uint64_t r11 packed_field;
    uint64_t r10 packed_field;
    uint64_t r9 packed_field;
    uint64_t r8 packed_field;
    uint64_t rax packed_field;
    uint64_t rcx packed_field;
    uint64_t rdx packed_field;
    uint64_t rbx packed_field;
    uint64_t rsp packed_field;
    uint64_t rbp packed_field;
    uint64_t rsi packed_field;
    uint64_t rdi packed_field;
    uint64_t io_mem_addr packed_field;
    uint32_t io_misc;
    alignas(4) uint16_t es;
    alignas(4) uint16_t cs;
    alignas(4) uint16_t ss;
    alignas(4) uint16_t ds;
    alignas(4) uint16_t fs;
    alignas(4) uint16_t gs;
    alignas(4) uint16_t ldtr;
    alignas(4) uint16_t tr;
    uint64_t dr7;
    uint64_t dr6;
    uint64_t rip;
    uint64_t efer;
    uint64_t rflags;
    uint64_t cr3;
    uint64_t cr0;
};

struct z86RsmFrameAMD64 {
    z86DescriptorCacheAMD64 es_descriptor;
    z86DescriptorCacheAMD64 cs_descriptor;
    z86DescriptorCacheAMD64 ss_descriptor;
    z86DescriptorCacheAMD64 ds_descriptor;
    z86DescriptorCacheAMD64 fs_descriptor;
    z86DescriptorCacheAMD64 gs_descriptor;
    z86DescriptorCacheAMD64 gdt_descriptor;
    z86DescriptorCacheAMD64 ldt_descriptor;
    z86DescriptorCacheAMD64 idt_descriptor;
    z86DescriptorCacheAMD64 tss_descriptor;
    uint64_t io_restart_rip;
    uint64_t io_restart_rcx;
    uint64_t io_restart_rsi;
    uint64_t io_restart_rdi;
    uint32_t io_restart_dword;
    uint32_t : 32;
    uint8_t io_restart_flag;
    uint8_t halt_restart_flag;
    uint16_t : 16;
    uint32_t : 32;
    uint64_t efer;
    uint64_t guest;
    uint64_t guest_vmcb_physical_addr;
    uint64_t guest_virtual_interrupt;
    uint64_t : 64;
    uint32_t : 32;
    uint32_t smm_revision;
    uint32_t smm_base;
    uint32_t : 32;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t ssp;
    uint64_t guest_pat;
    uint64_t host_efer;
    uint64_t host_cr4;
    uint64_t host_cr3;
    uint64_t host_cr0;
    uint64_t cr4;
    uint64_t cr3;
    uint64_t cr0;
    uint64_t dr7;
    uint64_t dr6;
    uint64_t rflags;
    uint64_t rip;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rbx;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rax;
};
static_assert(sizeof(z86RsmFrameAMD64) == 0x200);

template <size_t bits>
struct z86TSSStack;

template <>
struct z86TSSStack<16> {
    uint16_t sp;
    uint16_t ss;
};

template <>
struct z86TSSStack<32> {
    uint32_t sp;
    alignas(4) uint16_t ss;
};

template <>
struct z86TSSStack<64> {
    uint64_t sp;
};

template <size_t bits>
struct z86TSS;

template <>
struct z86TSS<16> {
    uint16_t link;
    union {
        z86TSSStack<16> stacks[3];
        struct {
            uint16_t sp0;
            uint16_t ss0;
            uint16_t sp1;
            uint16_t ss1;
            uint16_t sp2;
            uint16_t ss2;
        };
    };
    uint16_t ip;
    uint16_t flags;
    uint16_t ax;
    uint16_t cx;
    uint16_t dx;
    uint16_t bx;
    uint16_t sp;
    uint16_t bp;
    uint16_t si;
    uint16_t di;
    uint16_t es;
    uint16_t cs;
    uint16_t ss;
    uint16_t ds;
    uint16_t ldtr;
};

template <>
struct z86TSS<32> {
    alignas(4) uint16_t link;
    union {
        z86TSSStack<32> stacks[3];
        struct {
            uint32_t sp0;
            alignas(4) uint16_t ss0;
            uint32_t sp1;
            alignas(4) uint16_t ss1;
            uint32_t sp2;
            alignas(4) uint16_t ss2;
        };
    };
    uint32_t cr3;
    uint32_t ip;
    uint32_t flags;
    uint32_t ax;
    uint32_t cx;
    uint32_t dx;
    uint32_t bx;
    uint32_t sp;
    uint32_t bp;
    uint32_t si;
    uint32_t di;
    alignas(4) uint16_t es;
    alignas(4) uint16_t cs;
    alignas(4) uint16_t ss;
    alignas(4) uint16_t ds;
    alignas(4) uint16_t fs;
    alignas(4) uint16_t gs;
    alignas(4) uint16_t ldtr;
    uint16_t task_trap : 1;
    uint16_t : 15;
    uint16_t io_map_base;
    uint32_t ssp;
};
static_assert(sizeof(z86TSS<32>) == 108);

template <>
struct z86TSS<64> {
    uint32_t : 32;
    union {
        z86TSSStack<64> stacks[3] packed_field;
        struct {
            uint64_t sp0 packed_field;
            uint64_t sp1 packed_field;
            uint64_t sp2 packed_field;
        };
    };
    union {
        uint64_t ist[8] packed_field;
        struct {
            uint64_t ist0 packed_field;
            uint64_t ist1 packed_field;
            uint64_t ist2 packed_field;
            uint64_t ist3 packed_field;
            uint64_t ist4 packed_field;
            uint64_t ist5 packed_field;
            uint64_t ist6 packed_field;
            uint64_t ist7 packed_field;
        };
    };
    uint32_t : 32;
    uint32_t : 32;
    uint16_t : 16;
    uint16_t io_map_base;
};
static_assert(sizeof(z86TSS<64>) == 104);

template <size_t max_bits, bool protected_mode, bool use_old_reset>
struct z86BaseControlBase;

// size: 0x12
template <bool use_old_reset>
struct z86BaseControlBase<16, false, use_old_reset> {

    static inline constexpr z86DescriptorCache<16> descriptors[12] = {};

    static inline constexpr uint16_t reset_ip = !use_old_reset ? 0xFFF0 : 0x0000;
    static inline constexpr uint16_t reset_cs = !use_old_reset ? 0xF000 : 0xFFFF;

    union {
        uint16_t rip = reset_ip;
        uint16_t eip;
        uint16_t ip;
    };
    union {
        uint16_t seg[8];
        struct {
            uint16_t es;
            uint16_t cs = reset_cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
            uint16_t ds3;
            uint16_t ds2;
        };
    };
    int16_t pending_sinterrupt = -1;
    std::atomic<int16_t> pending_einterrupt = -1;
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;

    static inline constexpr uint16_t ldtr = 0;
    static inline constexpr uint16_t tr = 0;

    static inline constexpr uint8_t cpl = 0;
    static inline constexpr uint8_t iopl = 0;
    static inline constexpr uint8_t protected_mode = 0;
    static inline constexpr bool long_mode = false;
};

template <bool use_old_reset>
struct z86BaseControlBase<32, false, use_old_reset> {

    static inline constexpr z86DescriptorCache<32> descriptors[12] = {};

    static inline constexpr uint32_t reset_ip = !use_old_reset ? 0xFFF0 : 0x0000;
    static inline constexpr uint16_t reset_cs = !use_old_reset ? 0xF000 : 0xFFFF;

    union {
        uint32_t rip = reset_ip;
        uint32_t eip;
        uint16_t ip;
    };
    union {
        uint16_t seg[8];
        struct {
            uint16_t es;
            uint16_t cs = reset_cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
            uint16_t ds3;
            uint16_t ds2;
        };
    };
    int16_t pending_sinterrupt = -1;
    std::atomic<int16_t> pending_einterrupt = -1;
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;

    static inline constexpr uint16_t ldtr = 0;
    static inline constexpr uint16_t tr = 0;

    static inline constexpr uint8_t cpl = 0;
    static inline constexpr uint8_t iopl = 0;
    static inline constexpr uint8_t protected_mode = 0;
    static inline constexpr bool long_mode = false;
};

template <bool use_old_reset>
struct z86BaseControlBase<64, false, use_old_reset> {

    static inline constexpr z86DescriptorCache<64> descriptors[12] = {};

    static inline constexpr uint64_t reset_ip = !use_old_reset ? 0xFFF0 : 0x0000;
    static inline constexpr uint16_t reset_cs = !use_old_reset ? 0xF000 : 0xFFFF;

    union {
        uint64_t rip = reset_ip;
        uint32_t eip;
        uint16_t ip;
    };
    union {
        uint16_t seg[8];
        struct {
            uint16_t es;
            uint16_t cs = reset_cs;
            uint16_t ss;
            uint16_t ds;
            uint16_t fs;
            uint16_t gs;
            uint16_t ds3;
            uint16_t ds2;
        };
    };
    int16_t pending_sinterrupt = -1;
    std::atomic<int16_t> pending_einterrupt = -1;
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;

    static inline constexpr uint16_t ldtr = 0;
    static inline constexpr uint16_t tr = 0;

    static inline constexpr uint8_t cpl = 0;
    static inline constexpr uint8_t iopl = 0;
    static inline constexpr int8_t tss_size = 1;
    static inline constexpr uint8_t protected_mode = 0;
    static inline constexpr bool long_mode = false;
};

template <bool use_old_reset>
struct z86BaseControlBase<16, true, use_old_reset> {

    static inline constexpr uint16_t reset_ip = !use_old_reset ? 0xFFF0 : 0x0000;
    static inline constexpr uint16_t reset_cs = !use_old_reset ? 0xF000 : 0xFFFF;

    union {
        z86DescriptorCache<16> descriptors[12] = {
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, reset_cs << 4, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
        };
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
        uint16_t rip = reset_ip;
        uint16_t eip;
        uint16_t ip;
    };
    union {
        uint16_t cr[1];
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
            uint16_t cs = reset_cs;
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
    int16_t pending_sinterrupt = -1;
    std::atomic<int16_t> pending_einterrupt = -1;
    uint16_t error_code;
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;
    uint8_t cpl;
    uint8_t iopl;
    int8_t tss_size = 1;
    static inline constexpr bool long_mode = false;
};

template <bool use_old_reset>
struct z86BaseControlBase<32, true, use_old_reset> {

    static inline constexpr uint32_t reset_ip = !use_old_reset ? 0xFFF0 : 0x0000;
    static inline constexpr uint16_t reset_cs = !use_old_reset ? 0xF000 : 0xFFFF;

    union {
        z86DescriptorCache<32> descriptors[12] = {
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, reset_cs << 4, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
        };
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
        uint32_t rip = reset_ip;
        uint32_t eip;
        uint16_t ip;
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
            uint16_t cs = reset_cs;
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
    int16_t pending_sinterrupt = -1;
    std::atomic<int16_t> pending_einterrupt = -1;
    uint16_t error_code;
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;
    uint8_t cpl;
    uint8_t iopl;
    int8_t tss_size = 1;
    static inline constexpr bool long_mode = false;
};

template <bool use_old_reset>
struct z86BaseControlBase<64, true, use_old_reset> {

    static inline constexpr uint64_t reset_ip = !use_old_reset ? 0xFFF0 : 0x0000;
    static inline constexpr uint16_t reset_cs = !use_old_reset ? 0xF000 : 0xFFFF;

    union {
        z86DescriptorCache<64> descriptors[12] = {
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, reset_cs << 4, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_RW_DATAA | ATTRIBUTES_RWE | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
            { 0xFFFF, 0x00000000, DESCRIPTOR_LDT | ATTRIBUTES_R | ATTRIBUTES_VALID },
        };
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
        uint64_t rip = reset_ip;
        uint32_t eip;
        uint16_t ip;
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
            uint16_t cs = reset_cs;
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
    int16_t pending_sinterrupt = -1;
    std::atomic<int16_t> pending_einterrupt = -1;
    uint16_t error_code;
    std::atomic<bool> pending_nmi;
    std::atomic<bool> halted;
    uint8_t cpl;
    uint8_t iopl;
    int8_t tss_size = 1;
    bool long_mode;
};

struct z86PageFaultErrorCode {
    union {
        uint16_t raw;
        struct {
            uint8_t p : 1; // 0 = not present, 1 = protection
            uint8_t rw : 1; // 0 = read, 1 = write
            uint8_t su : 1; // 0 = user, 1 = supervisor
            uint8_t rsvd : 1; // 0 = normal, 1 = reserved bits
            uint8_t di : 1; // 0 = data, 1 = instruction
            uint8_t pk : 1; // 0 = normal, 1 = protection keys
            uint8_t ss : 1; // 0 = normal, 1 = shadow stack
            uint8_t hlat : 1; // 0 = normal, 1 = ???
            uint8_t : 7;
            uint8_t sgx : 1; // 0 = normal, 1 = sgx
        };
    };
};

template <size_t max_bits, bool use_old_reset, bool has_protected_mode>
struct z86BaseControl;

template <size_t max_bits, bool use_old_reset>
struct z86BaseControl<max_bits, use_old_reset, false> : z86BaseControlBase<max_bits, false, use_old_reset> {

    inline jmp_buf& get_triple_fault_buf() const {
        return std::declval<jmp_buf>();
    }
    inline constexpr void triple_fault() const {
    }

    using BT = z86DescriptorCache<max_bits>::BT;
    using LT = z86DescriptorCache<max_bits>::LT;
    using RT = z86BaseGPRs<max_bits>::RT;

    template <typename T>
    inline void set_descriptor_base(uint8_t index, const T& base) {
    }

    inline void load_descriptor(uint8_t index, BT base, LT limit, uint16_t attributes) {
    }

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline void load_descriptor(uint8_t index, const T& descriptor) {
    }

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline void load_descriptor(uint8_t index, const T& descriptor, uint16_t attributes) {
    }

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline void fill_descriptor(uint8_t index, T& descriptor) {
    }

    inline void z86call software_interrupt(uint8_t number) {
        this->pending_sinterrupt = number;
    }

    inline void z86call software_interrupt(uint8_t number, uint16_t error_code) {
        this->pending_sinterrupt = number;
    }

    inline constexpr const uint16_t& get_seg_impl(uint8_t index) {
        return this->seg[index];
    }

    inline constexpr uint16_t get_control_seg(uint8_t index) const {
        return 0;
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
    inline constexpr void set_privilege_level(uint8_t privilege) {
    }

    inline constexpr uint16_t get_machine_status_word() const {
        return 0;
    }

    inline constexpr void set_machine_status_word(uint16_t msw) {
    }

    inline constexpr RT get_control_reg(uint8_t index) const {
        return 0;
    }
    inline constexpr void set_control_reg(uint8_t index, RT value) {
    }

    inline constexpr void set_iopl(uint8_t value) {
    }
    inline constexpr uint8_t get_iopl() const {
        return 3;
    }
    inline constexpr bool io_is_allowed() const {
        return true;
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
    inline constexpr bool long_mode_enabled() const {
        return false;
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
struct z86BaseControl<max_bits, use_old_reset, true> : z86BaseControlBase<max_bits, true, use_old_reset> {

    static inline jmp_buf TRIPLE_FAULT_BUF;
    inline jmp_buf& get_triple_fault_buf() const {
        return TRIPLE_FAULT_BUF;
    }
    [[noreturn]] gnu_noinline void triple_fault() const {
        longjmp(TRIPLE_FAULT_BUF, 1);
    }

    using BT = z86DescriptorCache<max_bits>::BT;
    using LT = z86DescriptorCache<max_bits>::LT;
    using RT = z86BaseGPRs<max_bits>::RT;

    inline void set_descriptor_base(uint8_t index, BT base) {
        //this->descriptors[index].base = base;
        reconstruct_at(&this->descriptors[index], this->descriptors[index].limit, base, this->descriptors[index].attributes);
    }

    inline void set_descriptor_limit(uint8_t index, LT limit) {
        //this->descriptors[index].limit = limit;
        reconstruct_at(&this->descriptors[index], limit, this->descriptors[index].base, this->descriptors[index].attributes);
    }

    inline void set_descriptor_attributes(uint8_t index, uint16_t attributes) {
        //this->descriptors[index].attributes.raw = attributes;
        reconstruct_at(&this->descriptors[index], this->descriptors[index].limit, this->descriptors[index].base, attributes);
    }

    inline void set_descriptor_attributes(uint8_t index, const z86DescriptorAttributes& attributes) {
        //this->descriptors[index].attributes.raw = attributes;
        reconstruct_at(&this->descriptors[index], this->descriptors[index].limit, this->descriptors[index].base, attributes);
    }

    inline void load_descriptor(uint8_t index, BT base, LT limit, uint16_t attributes) {
        //this->descriptors[index].base = base;
        //this->descriptors[index].limit = limit;
        //this->descriptors[index].attributes.raw = attributes;
        reconstruct_at(&this->descriptors[index], base, limit, attributes);
    }

    template <size_t bits>
    inline void load_descriptor(uint8_t index, const SEG_DESCRIPTOR<bits>* descriptor) {
        //this->descriptors[index].base = descriptor->base();
        //this->descriptors[index].limit = descriptor->limit();
        //this->descriptors[index].attributes.raw = descriptor->attributes();
        reconstruct_at(&this->descriptors[index], descriptor);
    }

    template <size_t bits>
    inline void load_descriptor(uint8_t index, const SEG_DESCRIPTOR<bits>* descriptor, uint16_t extra) {
        //this->descriptors[index].base = descriptor->base();
        //this->descriptors[index].limit = descriptor->limit();
        //this->descriptors[index].attributes.raw = descriptor->attributes(extra);
        reconstruct_at(&this->descriptors[index], descriptor, extra);
    }

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline void load_descriptor(uint8_t index, const T& descriptor) {
        reconstruct_at(&this->descriptors[index], descriptor);
    }

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline void load_descriptor(uint8_t index, const T& descriptor, uint16_t attributes) {
        reconstruct_at(&this->descriptors[index], descriptor, attributes);
    }

    template <typename T> requires(std::is_base_of_v<z86DescriptorCacheHardware, T>)
    inline void fill_descriptor(uint8_t index, T& descriptor) {
        this->descriptors[index].fill_descriptor(descriptor);
    }

    // INT n does not push error codes
    // External interrupts do not push error codes
    static inline constexpr bool interrupt_uses_error_code(uint32_t number) {
        if (expect(number < 32, true)) {
            //return 0b01100000001001100111110100000010 & 1 << number;
            return 0b01100000001000100011110100000000 & 1 << number;
        }
        return false;
    }

    inline void z86call software_interrupt(uint8_t number) {
        this->pending_sinterrupt = number;
    }

    inline void z86call software_interrupt(uint8_t number, uint16_t error_code) {
        this->pending_sinterrupt = number;
        this->error_code = error_code;
    }

    inline constexpr const uint16_t& get_seg_impl(uint8_t index) {
        return this->seg[index];
    }

    inline constexpr uint16_t get_control_seg(uint8_t index) const {
        return this->seg[LDT + index];
    }

    inline constexpr void load_descriptor_table(uint8_t index, LT limit, BT base) {
        reconstruct_at(&this->descriptors[GDT + index], limit, base, this->descriptors[GDT + index].attributes);
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
    inline constexpr void set_privilege_level(uint8_t privilege) {
        this->cpl = privilege;
    }

    inline constexpr auto get_machine_status_word() const {
        return this->cr0;
    }

    inline constexpr void set_machine_status_word(uint16_t msw) {
        // TODO: filter bits
        this->msw = msw;
    }

    inline constexpr RT get_control_reg(uint8_t index) const {
        return this->cr[index];
    }
    inline constexpr void set_control_reg(uint8_t index, RT value) {
        this->cr[index] = value;
    }

    inline constexpr void set_iopl(uint8_t value) {
        this->iopl = value;
    }
    inline constexpr uint8_t get_iopl() const {
        return this->iopl;
    }
    inline constexpr bool io_is_allowed() const {
        return this->cpl <= this->iopl;
    }

    inline constexpr bool is_real_mode() const {
        return !this->protected_mode;
    }
    inline constexpr bool is_protected_mode() const {
        return this->protected_mode;
    }
    inline constexpr bool is_long_mode() const {
        if constexpr (max_bits == 64) {
            return this->long_mode;
        }
        return false;
    }
    //inline constexpr bool long_mode_enabled() const {

    //}
};

template <bool has_protected_mode>
struct z86Flags;

template <>
struct z86Flags<false> {
    // Flags
    bool carry;
    bool parity;
    bool auxiliary;
    bool zero;
    bool sign;
    bool overflow;
    /*
    union {
        bool trap : 1;
        bool interrupt : 1;
        bool direction : 1;
    };
    */
    bool trap;
    bool interrupt;
    bool direction;
};

template <>
struct z86Flags<true> {
    // Flags
    bool carry;
    bool parity;
    bool auxiliary;
    bool zero;
    bool sign;
    bool overflow;
    /*
    union {
        bool trap : 1;
        bool interrupt : 1;
        bool direction : 1;
        bool nested_task : 1;
    };
    */
    bool trap;
    bool interrupt;
    bool direction;
    bool nested_task;
};

template <size_t max_bits, bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase;

template <bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase<16, use_old_reset, has_protected_mode, has_x87, max_sse_bits, sse_reg_count> :
    z86BaseSSE<max_sse_bits, sse_reg_count>,
    z86BaseFPU<has_x87>,
    z86BaseGPRs<16>,
    z86BaseControl<16, use_old_reset, has_protected_mode>,
    z86BaseFPUControl<16, has_x87, max_sse_bits != 0>,
    z86Flags<has_protected_mode>
{
    static constexpr inline int8_t data_size = 1;
    static constexpr inline int8_t addr_size = 1;
    static constexpr inline int8_t default_data_size = 1;
    static constexpr inline int8_t default_addr_size = 1;
    static constexpr inline bool stack_size = true;
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
    inline constexpr void change_data_size(uint8_t size) {
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
    inline constexpr void change_addr_size(uint8_t size) {
    }
    inline constexpr bool stack_size_16() const {
        return true;
    }
    inline constexpr bool stack_size_32() const {
        return false;
    }
    inline constexpr bool stack_size_64() const {
        return false;
    }
    inline constexpr void change_stack_size(bool size) {
    }
    inline constexpr bool tss_size_16() const {
        return true;
    }
    inline constexpr bool tss_size_32() const {
        return false;
    }
    inline constexpr bool tss_size_64() const {
        return false;
    }
    inline constexpr void change_tss_size(uint8_t size) {
    }
    inline constexpr REX get_rex_bits() const {
        return {};
    }
    inline constexpr void set_rex_bits(uint8_t bits) {
    }
    inline constexpr void reset_rex_bits() {
    }
};

template <bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase<32, use_old_reset, has_protected_mode, has_x87, max_sse_bits, sse_reg_count> :
    z86BaseSSE<max_sse_bits, sse_reg_count>,
    z86BaseFPU<has_x87>,
    z86BaseGPRs<32>,
    z86BaseControl<32, use_old_reset, has_protected_mode>,
    z86BaseFPUControl<32, has_x87, max_sse_bits != 0>,
    z86Flags<has_protected_mode>
{
    // TODO: How bad is volatile here?
    int8_t data_size = 1;
    int8_t addr_size = 1;
    int8_t default_data_size = 1;
    int8_t default_addr_size = 1;
    bool stack_size = true;
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
    inline constexpr void change_data_size(bool size) {
        this->default_data_size = !size;
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
    inline constexpr void change_addr_size(bool size) {
        this->default_addr_size = !size;
    }
    inline constexpr bool stack_size_16() const {
        return this->stack_size;
    }
    inline constexpr bool stack_size_32() const {
        return !this->stack_size;
    }
    inline constexpr bool stack_size_64() const {
        return false;
    }
    inline constexpr void change_stack_size(bool size) {
        this->stack_size = !size;
    }
    inline constexpr bool tss_size_16() const {
        return this->tss_size != 0;
    }
    inline constexpr bool tss_size_32() const {
        return this->tss_size == 0;
    }
    inline constexpr bool tss_size_64() const {
        return false;
    }
    inline constexpr void change_tss_size(bool size) {
    }
    inline constexpr REX get_rex_bits() const {
        return {};
    }
    inline constexpr void set_rex_bits(uint8_t bits) {
    }
    inline constexpr void reset_rex_bits() {
    }
};

template <bool use_old_reset, bool has_protected_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegBase<64, use_old_reset, has_protected_mode, has_x87, max_sse_bits, sse_reg_count> :
    z86BaseSSE<max_sse_bits, sse_reg_count>,
    z86BaseFPU<has_x87>,
    z86BaseGPRs<64>,
    z86BaseControl<64, use_old_reset, has_protected_mode>,
    z86BaseFPUControl<64, has_x87, max_sse_bits != 0>,
    z86Flags<has_protected_mode>
{
    int8_t data_size = 1;
    int8_t addr_size = 1;
    int8_t default_data_size = 1;
    int8_t default_addr_size = 1;
    bool stack_size = true; // long mode overrides this, always check that first
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
    inline constexpr void change_data_size(uint8_t size) {
        this->default_data_size = !size;
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
    inline constexpr void change_addr_size(uint8_t size) {
        switch (size) {
            default: unreachable;
            case 0b00:
                this->default_addr_size = 1;
                break;
            case 0b10:
                this->default_addr_size = 0;
                break;
            case 0b01:
                this->default_addr_size = -1;
                break;
        }
    }
    inline constexpr bool stack_size_16() const {
        return this->stack_size;
    }
    inline constexpr bool stack_size_32() const {
        return !this->stack_size;
    }
    inline constexpr bool stack_size_64() const {
        return this->is_long_mode();
    }
    inline constexpr void change_stack_size(bool size) {
        this->stack_size = !size;
    }
    inline constexpr bool tss_size_16() const {
        return this->tss_size > 0;
    }
    inline constexpr bool tss_size_32() const {
        return this->tss_size == 0;
    }
    inline constexpr bool tss_size_64() const {
        return this->tss_size < 0;
    }
    inline constexpr void change_tss_size(int8_t size) {
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
};

#define z86RegCommonTemplate size_t max_bits, bool use_old_reset, bool has_protected_mode, bool has_long_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count
#define z86RegCommonDefault z86RegCommon<max_bits, use_old_reset, has_protected_mode, has_long_mode, has_x87, max_sse_bits, sse_reg_count>

template <size_t max_bits, bool use_old_reset, bool has_protected_mode, bool has_long_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86RegCommon : z86RegBase<max_bits, use_old_reset, has_protected_mode, has_x87, max_sse_bits, sse_reg_count> {

    using HT = z86BaseGPRs<max_bits>::HT;
    using RT = z86BaseGPRs<max_bits>::RT;
    using DT = z86BaseGPRs<max_bits>::DT;

    using SRT = make_signed_ex_t<RT>;
    int8_t seg_override = -1;

    inline constexpr void set_seg_override(uint8_t seg) {
        this->seg_override = seg;
        if constexpr (has_long_mode) {
            if (this->is_long_mode()) {
                this->reset_rex_bits();
            }
        }
    }

    inline constexpr uint16_t segment(uint8_t default_seg) const {
        // TODO: Check if addition an expect hint is smart here
        if constexpr (has_protected_mode) {
            return this->seg_override < 0 ? default_seg : this->seg_override;
        } else {
            return this->seg[this->seg_override < 0 ? default_seg : this->seg_override];
        }
    }

    inline constexpr DT addr(uint8_t default_seg, RT offset) const {
        return (DT)this->segment(default_seg) << bitsof(RT) | offset;
    }

    inline constexpr DT addr_force(uint8_t seg, RT offset) const {
        if constexpr (has_protected_mode) {
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

    template <typename P = RT>
    inline constexpr DT str_src() const {
        return this->addr(DS, this->SI<P>());
    }

    template <typename P = RT>
    inline constexpr DT str_dst() const {
        return this->addr_force(ES, this->DI<P>());
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

    // Read A double
    template <typename T>
    inline constexpr auto read_AD() const {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            return this->ax;
        }
        else {
            using U = make_unsigned_ex_t<dbl_int_t<T>>;
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
            using U = make_unsigned_ex_t<T>;
            this->A<hlf_int_t<T>>() = value;
            this->D<hlf_int_t<T>>() = (U)value >> bitsof(hlf_int_t<T>);
        }
    }

    template <typename T>
    inline constexpr void write_AD(T lvalue, T hvalue) {
        if constexpr (sizeof(T) == sizeof(uint8_t)) {
            this->al = lvalue;
            this->ah = hvalue;
        }
        else {
            this->A<T>() = lvalue;
            this->D<T>() = hvalue;
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


    template <typename T = uint16_t>
    inline T get_flags() const {
        T base = 0b110000000000010;
        base |= this->carry;
        base |= (uint32_t)this->parity << 2;
        base |= (uint32_t)this->auxiliary << 4;
        base |= (uint32_t)this->zero << 6;
        base |= (uint32_t)this->sign << 7;
        if constexpr (sizeof(T) >= sizeof(uint16_t)) {
            base |= (uint32_t)this->trap << 8;
            base |= (uint32_t)this->interrupt << 9;
            base |= (uint32_t)this->direction << 10;
            base |= (uint32_t)this->overflow << 11;
            base |= (uint32_t)this->get_iopl() << 12;
            if constexpr (sizeof(T) >= sizeof(uint32_t)) {

            }
        }
        return base;
    }

    template <typename T = uint16_t, bool ignore_checks = false>
    inline void set_flags(T src) {
        this->carry = src & 0x01;
        this->parity = src & 0x04;
        this->auxiliary = src & 0x10;
        this->zero = src & 0x40;
        this->sign = src & 0x80;
        if constexpr (sizeof(T) >= sizeof(uint16_t)) {
            this->trap = src & 0x0100;
            uint8_t cpl = this->current_privilege_level();
            uint8_t iopl = this->get_iopl();
            if (ignore_checks || cpl <= iopl) {
                this->interrupt = src & 0x0200;
            }
            this->direction = src & 0x0400;
            this->overflow = src & 0x0800;
            if (ignore_checks || cpl == 0) {
                this->set_iopl(src >> 12 & 3);
            }
            if constexpr (sizeof(T) >= sizeof(uint32_t)) {

            }
        }
    }

    template <typename P, typename T>
    inline EXCEPTION z86call PUSH_impl(const T& src);

    template <typename T>
    inline EXCEPTION z86call PUSH16(const T& src) {
        return this->PUSH_impl<uint16_t>(src);
    }

    template <typename T>
    inline EXCEPTION z86call PUSH32(const T& src) {
        return this->PUSH_impl<uint32_t>(src);
    }

    template <typename T>
    inline EXCEPTION z86call PUSH64(const T& src) {
        return this->PUSH_impl<uint64_t>(src);
    }

    template <typename T = SRT>
    inline EXCEPTION z86call PUSH(const T& src) {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            if constexpr (max_bits == 64) {
                // 64 bit values can only be pushed in long
                // mode, where the stack is always 64 bit
                return this->PUSH64(src);
            }
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            if constexpr (max_bits > 16) {
                // No need to check for 64 bit stack size
                // because a 32 bit push can't be encoded
                // when running in long mode (hopefully)
                if (expect(this->stack_size_32(), true)) {
                    return this->PUSH32(src);
                }
            }
        }
        else {
            // 16 bit pushes are horrible and exist
            // in all modes
            if constexpr (max_bits > 16) {
                if constexpr (max_bits == 64) {
                    if (expect(this->stack_size_64(), false)) {
                        return this->PUSH64(src);
                    }
                }
                if (expect(this->stack_size_32(), false)) {
                    return this->PUSH32(src);
                }
            }
        }
        // This is doubling as a backup case for the constexprs
        return this->PUSH16(src);
    }

    template <typename T = SRT, typename ... Args> requires(sizeof...(Args) > 1)
    inline EXCEPTION z86call PUSH(Args&&... args) {
        if constexpr (sizeof(T) == sizeof(uint64_t)) {
            if constexpr (max_bits == 64) {
                // 64 bit values can only be pushed in long
                // mode, where the stack is always 64 bit
                return (this->PUSH64<T>(std::forward<Args>(args)) || ...);
            }
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            if constexpr (max_bits > 16) {
                // No need to check for 64 bit stack size
                // because a 32 bit push can't be encoded
                // when running in long mode (hopefully)
                if (expect(this->stack_size_32(), true)) {
                    return (this->PUSH32<T>(std::forward<Args>(args)) || ...);
                }
            }
        }
        else {
            // 16 bit pushes are horrible and exist
            // in all modes
            if constexpr (max_bits > 16) {
                if constexpr (max_bits == 64) {
                    if (expect(this->stack_size_64(), false)) {
                        return (this->PUSH64<T>(std::forward<Args>(args)) || ...);
                    }
                }
                if (expect(this->stack_size_32(), false)) {
                    return (this->PUSH32<T>(std::forward<Args>(args)) || ...);
                }
            }
        }
        // This is doubling as a backup case for the constexprs
        return (this->PUSH16<T>(std::forward<Args>(args)) || ...);
    }

    /*
        POPX<T>(D)
        X = Stack size (void = dynamic)
        T = SP offset size (void = D)
        D = Data size
        R = Read size (min(T, D))
    */

    template <typename P, typename R, typename T>
    inline EXCEPTION z86call POP_impl(T& dst);

    template <typename R = void, typename T>
    inline EXCEPTION z86call POP16(T& dst) {
        using R2 = std::conditional_t<std::is_same_v<R, void>, T, R>;
        return this->POP_impl<uint16_t, R2>(dst);
    }

    template <typename R = void, typename T>
    inline EXCEPTION z86call POP32(T& dst) {
        using R2 = std::conditional_t<std::is_same_v<R, void>, T, R>;
        return this->POP_impl<uint32_t, R2>(dst);
    }

    template <typename R = void, typename T>
    inline EXCEPTION z86call POP64(T& dst) {
        using R2 = std::conditional_t<std::is_same_v<R, void>, T, R>;
        return this->POP_impl<uint64_t, R2>(dst);
    }

    template <typename R = void, typename T>
    inline EXCEPTION z86call POP(T& dst) {
        using R2 = std::conditional_t<std::is_same_v<R, void>, T, R>;
        if constexpr (sizeof(R2) == sizeof(uint64_t)) {
            if constexpr (max_bits == 64) {
                // 64 bit values can only be popped in long
                // mode, where the stack is always 64 bit
                return this->POP64<R2>(dst);
            }
        }
        else if constexpr (sizeof(R2) == sizeof(uint32_t)) {
            if constexpr (max_bits > 16) {
                // No need to check for 64 bit stack size
                // because a 32 bit pop can't be encoded
                // when running in long mode (hopefully)
                if (expect(this->stack_size_32(), true)) {
                    return this->POP32<R2>(dst);
                }
            }
        }
        else {
            // 16 bit pops are horrible and exist
            // in all modes
            if constexpr (max_bits > 16) {
                if constexpr (max_bits == 64) {
                    if (expect(this->stack_size_64(), false)) {
                        return this->POP64<R2>(dst);
                    }
                }
                if (expect(this->stack_size_32(), false)) {
                    return this->POP32<R2>(dst);
                }
            }
        }
        // This is doubling as a backup case for the constexprs
        return this->POP16<R2>(dst);
    }

    template <typename P = void>
    inline constexpr RT stack_size_bp() const {
        if constexpr (std::is_same_v<P, void>) {
            // No size specified, calculate it
            if constexpr (bits > 16) {
                if constexpr (bits == 64) {
                    if (this->stack_size_64()) {
                        return this->rbp;
                    }
                }
                if (this->stack_size_32()) {
                    return this->ebp;
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

    template <typename T = RT>
    inline EXCEPTION z86call JMPABS(T new_ip) {
        this->rip = new_ip;
        return NO_FAULT;
    }

    template <typename T = RT>
    inline EXCEPTION z86call CALLABS(T next_ip, T new_ip) {
        this->PUSH(next_ip);
        return this->JMPABS(new_ip);
    }
};

template <size_t max_bits, bool use_old_reset, bool has_protected_mode, bool has_long_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86Reg;

#define z86RegRealOnlyTemplate size_t max_bits, bool use_old_reset, bool has_x87, size_t max_sse_bits, size_t sse_reg_count
#define z86RegRealOnlyDefault z86Reg<max_bits, use_old_reset, false, false, has_x87, max_sse_bits, sse_reg_count>

template <size_t max_bits, bool use_old_reset, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86Reg<max_bits, use_old_reset, false, false, has_x87, max_sse_bits, sse_reg_count> :
    z86RegCommon<max_bits, use_old_reset, false, false, has_x87, max_sse_bits, sse_reg_count>
{
    using HT = z86BaseGPRs<max_bits>::HT;
    using RT = z86BaseGPRs<max_bits>::RT;
    using DT = z86BaseGPRs<max_bits>::DT;

    using SRT = make_signed_ex_t<RT>;

    inline constexpr EXCEPTION write_seg_impl(uint8_t index, uint16_t value) {
        this->seg[index] = value;
        return NO_FAULT;
    }

    template <typename T = RT>
    inline EXCEPTION z86call JMPFABS_impl(T new_ip, uint16_t new_cs) {
        this->cs = new_cs;
        return this->JMPABS(new_ip);
    }

    template <typename T = RT>
    inline EXCEPTION z86call CALLFABS_impl(T next_ip, T new_ip, uint16_t new_cs) {
        this->PUSH<T>(this->cs);
        this->PUSH(next_ip);
        return this->JMPFABS_impl(new_ip, new_cs);
    }

    inline EXCEPTION z86call RETF() {
        if constexpr (max_bits > 16) {
            if (this->data_size_32()) {
                return this->POP<uint32_t>(this->rip) ||
                       this->POP<uint32_t>(this->cs);
            }
            if constexpr (max_bits == 64) {
                if (this->data_size_64()) {
                    return this->POP<uint64_t>(this->rip) ||
                           this->POP<uint64_t>(this->cs);
                }
            }
        }
        return this->POP<uint16_t>(this->rip) ||
               this->POP<uint16_t>(this->cs);
    }

    template <typename P> requires (!std::is_integral_v<P>)
    inline EXCEPTION z86call RETFI(const P& pc) {
        this->RETF();
        if constexpr (max_bits > 16) {
            if constexpr (max_bits == 64) {
                if (this->stack_size_64()) {
                    this->rsp += pc.read<uint16_t>();
                    return NO_FAULT;
                }
            }
            if (this->stack_size_32()) {
                this->esp += pc.read<uint16_t>();
                return NO_FAULT;
            }
        }
        this->sp += pc.read<uint16_t>();
        return NO_FAULT;
    }

    inline EXCEPTION z86call IRET() {
        uint32_t flags;
        if constexpr (max_bits > 16) {
            if (this->data_size_32()) {
                FAULT_CHECK(this->POP<uint32_t>(this->rip));
                FAULT_CHECK(this->POP<uint32_t>(this->cs));
                FAULT_CHECK(this->POP<uint32_t>(flags));
            }
            else {
                if constexpr (max_bits == 64) {
                    if (this->data_size_64()) {
                        FAULT_CHECK(this->POP<uint64_t>(this->rip));
                        FAULT_CHECK(this->POP<uint64_t>(this->cs));
                        FAULT_CHECK(this->POP<uint64_t>(flags));
                    }
                    else goto iret16;
                }
                else goto iret16;
            }
            this->set_flags(flags);
            return NO_FAULT;
        }
    iret16:
        FAULT_CHECK(this->POP<uint16_t>(this->rip));
        FAULT_CHECK(this->POP<uint16_t>(this->cs));
        FAULT_CHECK(this->POP<uint16_t>(flags));
        this->set_flags<uint16_t>(flags);
        return NO_FAULT;
    fault:
        return FAULT;
    }

    template <typename P>
    EXCEPTION z86call int_software_interrupt(const P& pc, uint8_t number, bool is_int1) {
        this->software_interrupt(number);
        return NO_FAULT;
    }

    void handle_pending_interrupt(uint8_t number) {
        this->PUSH(this->get_flags<uint16_t>());
        this->interrupt = false;
        bool prev_trap = this->trap;
        this->trap = false;
        this->PUSH(this->cs);
        this->PUSH(this->ip);

        size_t interrupt_addr = (size_t)number << 2;
        this->rip = mem.read<uint16_t>(interrupt_addr);
        this->cs = mem.read<uint16_t>(interrupt_addr + 2);
    }

    inline EXCEPTION VERR(uint16_t selector) {
        return FAULT;
    }
    inline EXCEPTION VERW(uint16_t selector) {
        return FAULT;
    }
    template <typename T>
    inline EXCEPTION LAR(T& dst, uint16_t selector) {
        return FAULT;
    }
    template <typename T>
    inline EXCEPTION LSL(T& dst, uint16_t selector) {
        return FAULT;
    }

    inline EXCEPTION write_tr(uint16_t selector) {
        return FAULT;
    }
    inline EXCEPTION write_ldtr(uint16_t selector) {
        return FAULT;
    }
};

#define z86RegProtectedTemplate size_t max_bits, bool use_old_reset, bool has_long_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count
#define z86RegProtectedDefault z86Reg<max_bits, use_old_reset, true, has_long_mode, has_x87, max_sse_bits, sse_reg_count>

template <size_t max_bits, bool use_old_reset, bool has_long_mode, bool has_x87, size_t max_sse_bits, size_t sse_reg_count>
struct z86Reg<max_bits, use_old_reset, true, has_long_mode, has_x87, max_sse_bits, sse_reg_count> :
    z86RegCommon<max_bits, use_old_reset, true, has_long_mode, has_x87, max_sse_bits, sse_reg_count>
{
    using HT = z86BaseGPRs<max_bits>::HT;
    using RT = z86BaseGPRs<max_bits>::RT;
    using DT = z86BaseGPRs<max_bits>::DT;

    using SRT = make_signed_ex_t<RT>;

    inline constexpr SEG_DESCRIPTOR<max_bits>* get_descriptor(uint16_t selector) const {
        return this->descriptors[GDT + (bool)(selector & 4)].get_descriptor(selector);
    }

    inline constexpr SEG_DESCRIPTOR<max_bits>* get_gdt_descriptor(uint16_t selector) const {
        return this->gdt_descriptor.get_descriptor(selector);
    }

    inline constexpr SEG_DESCRIPTOR<max_bits>* get_idt_descriptor(uint8_t selector) const {
        return this->idt_descriptor.get_descriptor(selector);
    }

    // Allowed segment types:
    // screw this though
    // CS: E code, ER code
    // SS: RW data
    // Other: R data, RW data, ER code

    // x64: POP DS/ES/SS not valid, LDS/LES not valid
    constexpr EXCEPTION write_seg_impl(uint8_t index, uint16_t selector) {
        if (this->protected_mode) {
            using T = SEG_DESCRIPTOR<(std::min)((size_t)32, max_bits)>;
            T* new_descriptor;
            uint8_t descriptor_type;
            if (index == SS) {
                // Null check
                if (expect(!(selector & ~3), false)) {
                    if constexpr (max_bits == 64) {
                        if (this->is_long_mode()) {
                            // Todo: Jank
                            if (this->cpl == 3) goto null_segment_set;
                            if (this->cpl == (selector & 3)) goto null_segment_set;
                        }
                    }
                    goto throw_gp_selector;
                }
                // RPL check
                if (expect(this->cpl != (selector & 3), false)) goto throw_gp_selector;
                new_descriptor = (T*)this->get_descriptor(selector);
                // Table limit check
                if (expect(!new_descriptor, false)) goto throw_gp_selector;
                // DPL check
                if (expect(this->cpl != new_descriptor->get_dpl(), false)) goto throw_gp_selector;
                // Type check
                constexpr uint32_t allowed_typesA = DESCRIPTORM_RW_DATA | DESCRIPTORM_RW_DATAA | DESCRIPTORM_RW_EDATA | DESCRIPTORM_RW_EDATAA;
                descriptor_type = new_descriptor->get_full_type();
                if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_gp_selector;
                // Present check
                if (expect(!new_descriptor->is_present(), false)) goto throw_ss_selector;
                // Suppress exceptions
                this->change_stack_size(new_descriptor->is_big());
            }
            else {
                // Null check
                if (expect((selector & ~3) != 0, true)) {
                    // RPL check
                    if (expect(this->cpl > (selector & 3), false)) goto throw_gp_selector;
                    new_descriptor = (T*)this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!new_descriptor, false)) goto throw_gp_selector;
                    // DPL check
                    if (expect(this->cpl > new_descriptor->get_dpl(), false)) goto throw_gp_selector;
                    // Type check
                    constexpr uint32_t allowed_typesB = DESCRIPTORM_R_DATA | DESCRIPTORM_R_DATAA | DESCRIPTORM_RW_DATA | DESCRIPTORM_RW_DATAA | DESCRIPTORM_R_EDATA | DESCRIPTORM_R_EDATAA | DESCRIPTORM_RW_EDATA | DESCRIPTORM_RW_EDATAA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                    descriptor_type = new_descriptor->get_full_type();
                    if (expect(!(allowed_typesB & 1 << descriptor_type), false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) {
                        this->software_interrupt(IntNP, selector);
                        return FAULT;
                    }
                }
                else {
                null_segment_set:
                    // Todo: mark invalid somehow
                    goto raw_segment_set;
                }
            }
            new_descriptor->set_accessed();
            //constexpr uint32_t access_mask = PackUIntOctets32(ACCESS_READ, ACCESS_READ | ACCESS_WRITE, ACCESS_READ, ACCESS_READ | ACCESS_WRITE, 0, ACCESS_READ, 0, ACCESS_READ);
            this->load_descriptor(index, new_descriptor, __builtin_parity((uint8_t)(descriptor_type & 0b1010)) ? ATTRIBUTES_RW : ATTRIBUTES_R);
        }
        else {
            this->set_descriptor_base(index, (size_t)selector << 4);
        }
    raw_segment_set:
        this->seg[index] = selector;
        return NO_FAULT;
    throw_gp_selector:
        this->software_interrupt(IntGP, selector & ~3);
        return FAULT;
    throw_ss_selector:
        this->software_interrupt(IntSS, selector & ~3);
        return FAULT;
    }

    constexpr EXCEPTION write_cs_jmp(RT& new_ip, uint16_t selector) {
        if (this->protected_mode) {
            // Null check
            if (expect(!(selector & ~3), false)) goto throw_gp_selector;
            auto* new_descriptor = this->get_descriptor(selector);
            // Table limit check
            if (expect(!new_descriptor, false)) goto throw_gp_selector;

            uint32_t allowed_typesA;
            if constexpr (max_bits == 64) {
                if (this->is_long_mode()) {
                    allowed_typesA = DESCRIPTORM_CALL_GATE | DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                    goto check_type;
                }
            }
            if constexpr (max_bits == 32) {
                allowed_typesA = DESCRIPTORM_TSS16 | DESCRIPTORM_CALL_GATE16 | DESCRIPTORM_TASK_GATE | DESCRIPTORM_TSS | DESCRIPTORM_CALL_GATE | DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
            }
            else {
                allowed_typesA = DESCRIPTORM_TSS16 | DESCRIPTORM_CALL_GATE16 | DESCRIPTORM_TASK_GATE | DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
            }
        check_type:
            uint8_t descriptor_type = new_descriptor->get_full_type();
            if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_gp_selector;
            switch (descriptor_type) {
                default: unreachable;
                // Task gate
                case DESCRIPTOR_TASK_GATE: {
                    uint8_t new_dpl = new_descriptor->get_dpl();
                    // DPL check
                    if (expect(this->cpl > new_dpl, false)) goto throw_gp_selector;
                    // RPL check
                    if (expect(new_dpl < (selector & 3), false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    selector = new_descriptor->segment();
                    // Null check?
                    // Local bit check
                    if (expect((bool)(selector & 4), false)) goto throw_np_selector;
                    new_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!new_descriptor, false)) goto throw_np_selector;
                    // Type check
                    constexpr uint32_t allowed_typesB = max_bits == 32 ? DESCRIPTORM_TSS16 | DESCRIPTORM_TSS : DESCRIPTORM_TSS16;
                    if (expect(!(allowed_typesB & 1 << new_descriptor->get_full_type()), false)) goto throw_gp_selector;
                    goto task_switch_common;
                }
                // Tss
                case DESCRIPTOR_TSS:
                    if constexpr (max_bits < 32) {
                        unreachable;
                    }
                case DESCRIPTOR_TSS16: {
                    uint8_t new_dpl = new_descriptor->get_dpl();
                    // DPL check
                    if (expect(this->cpl > new_dpl, false)) goto throw_gp_selector;
                    // RPL check
                    if (expect(new_dpl < (selector & 3), false)) goto throw_gp_selector;
                task_switch_common:
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    // Task switch
                    return NO_FAULT;
                }
                // Call gates
                case DESCRIPTOR_CALL_GATE:
                    if constexpr (max_bits < 32) {
                        unreachable;
                    }
                case DESCRIPTOR_CALL_GATE16: {
                    uint8_t new_dpl = new_descriptor->get_dpl();
                    // DPL check
                    if (expect(this->cpl > new_dpl, false)) goto throw_gp_selector;
                    // RPL check
                    if (expect(new_dpl < (selector & 3), false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    selector = new_descriptor->segment();
                    // Null check
                    if (expect(!(selector & ~3), false)) goto throw_gp_selector;
                    auto* dest_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!dest_descriptor, false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!dest_descriptor->is_present(), false)) goto throw_np_selector;
                    // DPL check
                    new_dpl = dest_descriptor->get_dpl();
                    if (expect(this->cpl < new_dpl, false)) goto throw_gp_selector;
                    // L+D check
                    if constexpr (max_bits == 64) {
                        if (this->is_long_mode()) {
                            if (expect(descriptor_type == DESCRIPTOR_CALL_GATE16, false)) goto throw_gp_selector;
                            if (expect(!dest_descriptor->is_valid_long_bits(), false)) goto throw_gp_selector;
                        }
                    }
                    // Type check
                    uint8_t gate_descriptor_type = descriptor_type;
                    constexpr uint32_t allowed_typesC = DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                    descriptor_type = dest_descriptor->get_full_type();
                    if (expect(!(allowed_typesC & 1 << descriptor_type), false)) goto throw_gp_selector;
                    switch (descriptor_type) {
                        default: unreachable;
                        // Code Segment
                        case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                        case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                            // This is the only difference from CALL...
                            if (expect(new_dpl != this->cpl, false)) goto throw_gp_selector;
                        // Conforming code segment
                        case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                        case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                            uint8_t param_count = 0;
                            if constexpr (max_bits >= 32) {
                                if (gate_descriptor_type == DESCRIPTOR_CALL_GATE) {
                                    if constexpr (max_bits == 64) {
                                        if (this->is_long_mode()) {
                                            // Call gate 64
                                            new_ip = new_descriptor->rip();
                                            break;
                                        }
                                    }
                                    // Call gate 32
                                    new_ip = new_descriptor->eip();
                                    for (size_t i = 0; i < param_count; ++i) {
                                        this->PUSH<uint32_t>(0); // TODO: params
                                    }
                                    break;
                                }
                            }
                            // Call gate 16
                            new_ip = new_descriptor->ip();
                        call_gate_params16:
                            for (size_t i = 0; i < param_count; ++i) {
                                this->PUSH<uint16_t>(0); // TODO: params
                            }
                            break;
                    }
                    new_descriptor = dest_descriptor;
                    // END CHECKS
                    // Set RPL to CPL
                    selector &= ~3;
                    selector |= this->cpl;
                    break;
                }
                // Conforming code segment
                case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                    // DPL check
                    if (expect(this->cpl < new_descriptor->get_dpl(), false)) goto throw_gp_selector;
                    goto main_code_segment_checks;
                // Code Segment
                case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                    // RPL check
                    if (expect(this->cpl < (selector & 3), false)) goto throw_gp_selector;
                    // DPL check
                    if (expect(this->cpl != new_descriptor->get_dpl(), false)) goto throw_gp_selector;
                main_code_segment_checks:
                    // L+D check
                    if constexpr (max_bits == 64) {
                        if (this->is_long_mode()) {
                            if (expect(new_descriptor->is_invalid_long_bits(), false)) goto throw_gp_selector;
                        }
                    }
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    // END CHECKS
                    // Set RPL to CPL
                    selector &= ~3;
                    selector |= this->cpl;
                    break;
            }
            new_descriptor->set_accessed();
            if constexpr (max_bits >= 32) {
                uint8_t new_size = new_descriptor->get_long_bits();
                this->change_data_size(new_size);
                this->change_addr_size(new_size);
            }
            this->load_descriptor(CS, new_descriptor, descriptor_type & 2 ? ATTRIBUTES_RE : ATTRIBUTES_E);
        }
        else {
            this->set_descriptor_base(CS, (size_t)selector << 4);
        }
        this->cs = selector;
        return NO_FAULT;
    throw_gp_selector:
        this->software_interrupt(IntGP, selector & ~3);
        return FAULT;
    throw_np_selector:
        this->software_interrupt(IntNP, selector & ~3);
        return FAULT;
    throw_ts_selector:
        this->software_interrupt(IntTS, selector & ~3);
        return FAULT;
    throw_ss_selector:
        this->software_interrupt(IntSS, selector & ~3);
        return FAULT;
    }

    template <typename T = RT>
    inline EXCEPTION z86call JMPFABS_impl(T new_ip, uint16_t new_cs) {
        RT full_ip = new_ip;
        if (!FAULTED(this->write_cs_jmp(full_ip, new_cs))) {
            return this->JMPABS<RT>(full_ip);
        }
        return FAULT;
    }

    constexpr EXCEPTION write_cs_call(RT& new_ip, uint16_t selector) {
        if (this->protected_mode) {
            // Null check
            if (expect(!(selector & ~3), false)) goto throw_gp_selector;
            auto* new_descriptor = this->get_descriptor(selector);
            // Table limit check
            if (expect(!new_descriptor, false)) goto throw_gp_selector;

            uint32_t allowed_typesA;
            if constexpr (max_bits == 64) {
                if (this->is_long_mode()) {
                    allowed_typesA = DESCRIPTORM_CALL_GATE | DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                    goto check_type;
                }
            }
            if constexpr (max_bits == 32) {
                allowed_typesA = DESCRIPTORM_TSS16 | DESCRIPTORM_CALL_GATE16 | DESCRIPTORM_TASK_GATE | DESCRIPTORM_TSS | DESCRIPTORM_CALL_GATE | DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
            }
            else {
                allowed_typesA = DESCRIPTORM_TSS16 | DESCRIPTORM_CALL_GATE16 | DESCRIPTORM_TASK_GATE | DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
            }
        check_type:
            uint8_t descriptor_type = new_descriptor->get_full_type();
            if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_gp_selector;
            switch (descriptor_type) {
                default: unreachable;
                // Task gate
                case DESCRIPTOR_TASK_GATE: {
                    uint8_t new_dpl = new_descriptor->get_dpl();
                    // DPL check
                    if (expect(this->cpl > new_dpl, false)) goto throw_gp_selector;
                    // RPL check
                    if (expect(new_dpl < (selector & 3), false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    selector = new_descriptor->segment();
                    // Null check?
                    // Local bit check
                    if (expect((bool)(selector & 4), false)) goto throw_np_selector;
                    new_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!new_descriptor, false)) goto throw_np_selector;
                    // Type check
                    constexpr uint32_t allowed_typesB = max_bits == 32 ? DESCRIPTORM_TSS16 | DESCRIPTORM_TSS : DESCRIPTORM_TSS16;
                    if (expect(!(allowed_typesB & 1 << new_descriptor->get_full_type()), false)) goto throw_gp_selector;
                    goto task_switch_common;
                }
                // Tss
                case DESCRIPTOR_TSS:
                    if constexpr (max_bits < 32) {
                        unreachable;
                    }
                case DESCRIPTOR_TSS16: {
                    uint8_t new_dpl = new_descriptor->get_dpl();
                    // DPL check
                    if (expect(this->cpl > new_dpl, false)) goto throw_gp_selector;
                    // RPL check
                    if (expect(new_dpl < (selector & 3), false)) goto throw_gp_selector;
                task_switch_common:
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    // Task switch
                    return NO_FAULT;
                }
                // Call gates
                case DESCRIPTOR_CALL_GATE:
                    if constexpr (max_bits < 32) {
                        unreachable;
                    }
                case DESCRIPTOR_CALL_GATE16: {
                    uint8_t new_dpl = new_descriptor->get_dpl();
                    // DPL check
                    if (expect(this->cpl > new_dpl, false)) goto throw_gp_selector;
                    // RPL check
                    if (expect(new_dpl < (selector & 3), false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    selector = new_descriptor->segment();
                    // Null check
                    if (expect(!(selector & ~3), false)) goto throw_gp_selector;
                    auto* dest_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!dest_descriptor, false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!dest_descriptor->is_present(), false)) goto throw_np_selector;
                    // DPL check
                    new_dpl = dest_descriptor->get_dpl();
                    if (expect(this->cpl < new_dpl, false)) goto throw_gp_selector;
                    // L+D check
                    if constexpr (max_bits == 64) {
                        if (this->is_long_mode()) {
                            if (expect(descriptor_type == DESCRIPTOR_CALL_GATE16, false)) goto throw_gp_selector;
                            if (expect(!dest_descriptor->is_valid_long_bits(), false)) goto throw_gp_selector;
                        }
                    }
                    // Type check
                    uint8_t gate_descriptor_type = descriptor_type;
                    constexpr uint32_t allowed_typesC = DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                    descriptor_type = dest_descriptor->get_full_type();
                    if (expect(!(allowed_typesC & 1 << descriptor_type), false)) goto throw_gp_selector;
                    uint8_t param_count;
                    switch (descriptor_type) {
                        default: unreachable;
                        // Code Segment
                        case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                        case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                            if (new_dpl < this->cpl) {
                                auto old_sp = this->rsp;
                                uint16_t old_ss = this->ss;
                                {
                                    uint16_t temp_cs = selector;
                                    RT new_sp;
                                    if constexpr (max_bits >= 32) {
                                        if (!this->tss_size_16()) {
                                            if constexpr (max_bits == 64) {
                                                if (this->tss_size_64()) {
                                                    // Load TSS 64
                                                    selector = new_dpl;
                                                    this->rsp = this->tss_descriptor.get_ptr<z86TSS<64>>()->stacks[new_dpl].sp;
                                                    goto skip_gate_tss_checks;
                                                }
                                            }
                                            // Load TSS 32
                                            const auto* tss32 = this->tss_descriptor.get_ptr<z86TSS<32>>();
                                            new_sp = tss32->stacks[new_dpl].sp;
                                            selector = tss32->stacks[new_dpl].ss;
                                            goto gate_tss_checks;
                                        }
                                    }
                                    // Load TSS 16
                                    {
                                        const auto* tss16 = this->tss_descriptor.get_ptr<z86TSS<16>>();
                                        new_sp = tss16->stacks[new_dpl].sp;
                                        selector = tss16->stacks[new_dpl].ss;
                                    }
                                gate_tss_checks:
                                    // Null check
                                    if (expect(!(selector & ~3), false)) goto throw_ts_selector;
                                    // RPL check
                                    if (expect(new_dpl != (selector & 3), false)) goto throw_ts_selector;
                                    auto* ss_descriptor = this->get_descriptor(selector);
                                    // Table limit check
                                    if (expect(!ss_descriptor, false)) goto throw_ts_selector;
                                    // DPL check
                                    if (expect(new_dpl != ss_descriptor->get_dpl(), false)) goto throw_ts_selector;
                                    // Type check
                                    if (expect(!ss_descriptor->is_valid_for_stack(), false)) goto throw_ts_selector;
                                    // Present check
                                    if (expect(!ss_descriptor->is_present(), false)) goto throw_ss_selector;
                                    this->set_privilege_level(new_dpl);
                                    this->change_stack_size(ss_descriptor->is_big());
                                    ss_descriptor->set_accessed();
                                    this->load_descriptor(SS, ss_descriptor, ATTRIBUTES_RW);
                                    this->rsp = new_sp;
                                    this->ss = selector;
                                    selector = temp_cs;
                                }
                            skip_gate_tss_checks:
                                param_count = 0;
                                if constexpr (max_bits >= 32) {
                                    if (gate_descriptor_type == DESCRIPTOR_CALL_GATE) {
                                        if constexpr (max_bits == 64) {
                                            if (this->is_long_mode()) {
                                                // Call gate 64
                                                new_ip = new_descriptor->rip();
                                                this->PUSH<uint64_t>(old_ss);
                                                this->PUSH<uint64_t>(old_sp);
                                                break;
                                            }
                                        }
                                        // Call gate 32
                                        new_ip = new_descriptor->eip();
                                        this->PUSH<uint32_t>(old_ss);
                                        this->PUSH<uint32_t>(old_sp);
                                        goto call_gate_params32;
                                    }
                                }
                                // Call gate 16
                                new_ip = new_descriptor->ip();
                                this->PUSH<uint16_t>(old_ss);
                                this->PUSH<uint16_t>(old_sp);
                                goto call_gate_params16;
                            }
                        // Conforming code segment
                        case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                        case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                            param_count = 0;
                            if constexpr (max_bits >= 32) {
                                if (gate_descriptor_type == DESCRIPTOR_CALL_GATE) {
                                    if constexpr (max_bits == 64) {
                                        if (this->is_long_mode()) {
                                            // Call gate 64
                                            new_ip = new_descriptor->rip();
                                            break;
                                        }
                                    }
                                    // Call gate 32
                                    new_ip = new_descriptor->eip();
                                    goto call_gate_params32;
                                }
                            }
                            // Call gate 16
                            new_ip = new_descriptor->ip();
                        call_gate_params16:
                            for (size_t i = 0; i < param_count; ++i) {
                                this->PUSH<uint16_t>(0); // TODO: params
                            }
                            break;
                        call_gate_params32:
                            for (size_t i = 0; i < param_count; ++i) {
                                this->PUSH<uint32_t>(0); // TODO: params
                            }
                            break;
                    }
                    // END CHECKS
                    // Set RPL to CPL
                    selector &= ~3;
                    selector |= this->cpl;
                    break;
                }
                // Conforming code segment
                case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                    // DPL check
                    if (expect(this->cpl < new_descriptor->get_dpl(), false)) goto throw_gp_selector;
                    goto main_code_segment_checks;
                // Code Segment
                case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                    // RPL check
                    if (expect(this->cpl < (selector & 3), false)) goto throw_gp_selector;
                    // DPL check
                    if (expect(this->cpl != new_descriptor->get_dpl(), false)) goto throw_gp_selector;
                main_code_segment_checks:
                    // L+D check
                    if constexpr (max_bits == 64) {
                        if (this->is_long_mode()) {
                            if (expect(new_descriptor->is_invalid_long_bits(), false)) goto throw_gp_selector;
                        }
                    }
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    // END CHECKS
                    // Set RPL to CPL
                    selector &= ~3;
                    selector |= this->cpl;
                    break;
            }
            new_descriptor->set_accessed();
            if constexpr (max_bits >= 32) {
                uint8_t new_size = new_descriptor->get_long_bits();
                this->change_data_size(new_size);
                this->change_addr_size(new_size);
            }
            this->load_descriptor(CS, new_descriptor, descriptor_type & 2 ? ATTRIBUTES_RE : ATTRIBUTES_E);
        }
        else {
            this->set_descriptor_base(CS, (size_t)selector << 4);
        }
        this->cs = selector;
        return NO_FAULT;
    throw_gp_selector:
        this->software_interrupt(IntGP, selector & ~3);
        return FAULT;
    throw_np_selector:
        this->software_interrupt(IntNP, selector & ~3);
        return FAULT;
    throw_ts_selector:
        this->software_interrupt(IntTS, selector & ~3);
        return FAULT;
    throw_ss_selector:
        this->software_interrupt(IntSS, selector & ~3);
        return FAULT;
    }

    template <typename T = RT>
    inline EXCEPTION z86call CALLFABS_impl(T next_ip, T new_ip, uint16_t new_cs) {
        RT full_ip = new_ip;
        uint16_t old_cs = this->cs;
        if (!FAULTED(this->write_cs_call(full_ip, new_cs))) {
            this->PUSH<T>(old_cs);
            this->PUSH(next_ip);
            return this->JMPABS(full_ip);
        }
        return FAULT;
    }

    // Load DS/ES with NULL when returning to lower privilege
    // What about FS/GS?
    constexpr EXCEPTION write_cs_ret(RT& new_ip, uint16_t selector, uint16_t imm) {
        if (this->protected_mode) {
            // Null check
            if (expect(!(selector & ~3), false)) goto throw_gp_selector;
            // RPL check
            if (expect(this->cpl > (selector & 3), false)) goto throw_gp_selector;
            auto* new_descriptor = this->get_descriptor(selector);
            // Table limit check
            if (expect(!new_descriptor, false)) goto throw_gp_selector;
            // Type check
            constexpr uint32_t allowed_typesA = DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
            uint8_t descriptor_type = new_descriptor->get_full_type();
            if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_gp_selector;
            uint8_t new_dpl = new_descriptor->get_dpl();
            switch (descriptor_type) {
                default: unreachable;
                // Code Segment
                case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                    // DPL check
                    if (expect(new_dpl != (selector & 3), false)) goto throw_gp_selector;
                    break;
                // Conforming code segment
                case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                    // DPL check
                    if (expect(new_dpl > (selector & 3), false)) goto throw_gp_selector;
                    break;
            }
            // Present check
            if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
            if (this->cpl < (selector & 3)) {
                uint16_t temp_cs = selector;
                RT new_sp;
                // Privilege change
                if constexpr (max_bits > 16) {
                    if (this->data_size_32()) {
                        this->esp += imm;
                        this->POP<uint32_t>(new_sp);
                        this->POP<uint32_t>(selector);
                        goto start_checks;
                    }
                    if constexpr (max_bits == 64) {
                        if (this->data_size_64()) {
                            this->rsp += imm;
                            this->POP<uint64_t>(new_sp);
                            this->POP<uint64_t>(selector);
                            goto start_checks;
                        }
                    }
                }
                this->sp += imm;
                this->POP<uint16_t>(new_sp);
                this->POP<uint16_t>(selector);
            start_checks:
                // Null check
                if (expect(!(selector & ~3), false)) goto throw_gp_selector;
                // RPL check
                if (expect((temp_cs & 3) != (selector & 3), false)) goto throw_gp_selector;
                auto* ss_descriptor = this->get_descriptor(selector);
                // Table limit check
                if (expect(!ss_descriptor, false)) goto throw_gp_selector;
                // DPL check
                new_dpl = ss_descriptor->get_dpl();
                if (expect((temp_cs & 3) != new_dpl, false)) goto throw_gp_selector;
                // Type check
                if (expect(!ss_descriptor->is_valid_for_stack(), false)) goto throw_gp_selector;
                // Present check
                if (expect(!ss_descriptor->is_present(), false)) goto throw_ss_selector;
                this->set_privilege_level(new_dpl);
                this->change_stack_size(ss_descriptor->is_big());
                ss_descriptor->set_accessed();
                this->load_descriptor(SS, ss_descriptor, ATTRIBUTES_RW);
                this->rsp = new_sp;
                this->ss = selector;
                selector = temp_cs;
                // Protect data descriptors...
            }
            if constexpr (max_bits > 16) {
                if constexpr (max_bits == 64) {
                    if (this->stack_size_64()) {
                        this->rsp += imm;
                        goto after_stack_imm;
                    }
                }
                if (this->stack_size_32()) {
                    this->esp += imm;
                    goto after_stack_imm;
                }
            }
            this->sp += imm;
        after_stack_imm:
            new_descriptor->set_accessed();
            if constexpr (max_bits >= 32) {
                uint8_t new_size = new_descriptor->get_long_bits();
                this->change_data_size(new_size);
                this->change_addr_size(new_size);
            }
            this->load_descriptor(CS, new_descriptor, descriptor_type & 2 ? ATTRIBUTES_RE : ATTRIBUTES_E);
            // Protect data descriptors...
        }
        else {
            this->set_descriptor_base(CS, (size_t)selector << 4);
            this->sp += imm;
        }
        this->cs = selector;
        return NO_FAULT;
    throw_gp_selector:
        this->software_interrupt(IntGP, selector & ~3);
        return FAULT;
    throw_np_selector:
        this->software_interrupt(IntNP, selector & ~3);
        return FAULT;
    throw_ss_selector:
        this->software_interrupt(IntSS, selector & ~3);
        return FAULT;
    }

    // TODO: Make this only read the params, pop inside write_cs_ret
    inline constexpr EXCEPTION RETFI(uint16_t imm) {
        RT new_ip;
        uint16_t new_cs;
        if constexpr (max_bits > 16) {
            if (this->data_size_32()) {
                FAULT_CHECK(this->POP<uint32_t>(new_ip));
                FAULT_CHECK(this->POP<uint32_t>(new_cs));
                goto start_checks;
            }
            if constexpr (max_bits == 64) {
                if (this->data_size_64()) {
                    FAULT_CHECK(this->POP<uint64_t>(new_ip));
                    FAULT_CHECK(this->POP<uint64_t>(new_cs));
                    goto start_checks;
                }
            }
        }
        FAULT_CHECK(this->POP<uint16_t>(new_ip));
        FAULT_CHECK(this->POP<uint16_t>(new_cs));
    start_checks:
        if (!FAULTED(this->write_cs_ret(new_ip, new_cs, imm))) {
            return this->JMPABS(new_ip);
        }
    fault:
        return FAULT;
    }

    inline EXCEPTION z86call RETF() {
        return this->RETFI(0);
    }

    template <typename P> requires (!std::is_integral_v<P>)
    inline EXCEPTION z86call RETFI(const P& pc) {
        return this->RETFI(pc.read<uint16_t>());
    }

    inline constexpr EXCEPTION write_cs_iret(RT& new_ip, uint16_t selector, uint32_t flags) {
        if (this->protected_mode) {
            if constexpr (max_bits == 64) {
                if (this->is_long_mode()) {
                    // NT check
                    if (expect(this->nested_task, false)) goto throw_gp_zero;
                    goto not_nested_task;
                }
            }
            if (expect(!this->nested_task, true)) {
                // Check v86 mode flag
            not_nested_task:
                // Null check
                if (expect(!(selector & ~3), false)) goto throw_gp_selector;
                // RPL check
                if (expect(this->cpl > (selector & 3), false)) goto throw_gp_selector;
                auto* new_descriptor = this->get_descriptor(selector);
                // Table limit check
                if (expect(!new_descriptor, false)) goto throw_gp_selector;
                // Type check
                constexpr uint32_t allowed_typesA = DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                uint8_t descriptor_type = new_descriptor->get_full_type();
                if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_gp_selector;
                uint8_t new_dpl = new_descriptor->get_dpl();
                switch (descriptor_type) {
                    default: unreachable;
                    // Code Segment
                    case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                    case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                        // DPL check
                        if (expect(new_dpl != (selector & 3), false)) goto throw_gp_selector;
                        break;
                    // Conforming code segment
                    case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                    case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                        // DPL check
                        if (expect(new_dpl > (selector & 3), false)) goto throw_gp_selector;
                        break;
                }
                if constexpr (max_bits == 64) {
                    if (this->is_long_mode()) {
                        goto always_pop_ss;
                    }
                }
                if (this->cpl < (selector & 3)) {
                always_pop_ss:
                    uint16_t temp_cs = selector;
                    RT new_sp;
                    // Privilege change
                    if constexpr (max_bits > 16) {
                        if (this->data_size_32()) {
                            this->POP<uint32_t>(new_sp);
                            this->POP<uint32_t>(selector);
                            goto start_checks;
                        }
                        if constexpr (max_bits == 64) {
                            if (this->data_size_64()) {
                                this->POP<uint64_t>(new_sp);
                                this->POP<uint64_t>(selector);
                                goto start_checks;
                            }
                        }
                    }
                    this->POP<uint16_t>(new_sp);
                    this->POP<uint16_t>(selector);
                start_checks:
                    // NULL is valid in long mode...
                    // Null check
                    if (expect(!(selector & ~3), false)) goto throw_gp_selector;
                    // RPL check
                    if (expect((temp_cs & 3) != (selector & 3), false)) goto throw_gp_selector;
                    auto* ss_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!ss_descriptor, false)) goto throw_gp_selector;
                    // DPL check
                    new_dpl = ss_descriptor->get_dpl();
                    if (expect((temp_cs & 3) != new_dpl, false)) goto throw_gp_selector;
                    // Type check
                    if (expect(!ss_descriptor->is_valid_for_stack(), false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!ss_descriptor->is_present(), false)) goto throw_ss_selector;
                    this->set_privilege_level(new_dpl);
                    this->change_stack_size(ss_descriptor->is_big());
                    ss_descriptor->set_accessed();
                    this->load_descriptor(SS, ss_descriptor, ATTRIBUTES_RW);
                    this->rsp = new_sp;
                    this->ss = selector;
                    selector = temp_cs;
                    // Protect data descriptors...
                }
                if constexpr (max_bits > 16) {
                    if (!this->data_size_16()) {
                        this->set_flags(flags);
                        goto after_flags;
                    }
                }
                this->set_flags<uint16_t>(flags);
            after_flags:
                new_descriptor->set_accessed();
                if constexpr (max_bits >= 32) {
                    uint8_t new_size = new_descriptor->get_long_bits();
                    this->change_data_size(new_size);
                    this->change_addr_size(new_size);
                }
                this->load_descriptor(CS, new_descriptor, descriptor_type & 2 ? ATTRIBUTES_RE : ATTRIBUTES_E);
            }
            else {
                // Task return
                // WTF?
            }
        }
        else {
            this->set_descriptor_base(CS, (size_t)selector << 4);
            if constexpr (max_bits > 16) {
                if (!this->data_size_16()) {
                    this->set_flags<uint32_t>(flags);
                    goto ret;
                }
            }
            this->set_flags<uint16_t>(flags);
        }
    ret:
        this->cs = selector;
        return NO_FAULT;
    throw_gp_zero:
        selector = 0;
    throw_gp_selector:
        this->software_interrupt(IntGP, selector & ~3);
        return FAULT;
    throw_np_selector:
        this->software_interrupt(IntNP, selector & ~3);
        return FAULT;
    throw_ss_selector:
        this->software_interrupt(IntSS, selector & ~3);
        return FAULT;
    }

    // TODO: Make this only read the params, pop inside write_cs_iret
    inline EXCEPTION z86call IRET() {
        RT new_ip;
        uint16_t new_cs;
        uint32_t new_flags;
        if constexpr (max_bits > 16) {
            if (this->data_size_32()) {
                FAULT_CHECK(this->POP<uint32_t>(new_ip));
                FAULT_CHECK(this->POP<uint32_t>(new_cs));
                FAULT_CHECK(this->POP<uint32_t>(new_flags));
                goto start_checks;
            }
            if constexpr (max_bits == 64) {
                if (this->data_size_64()) {
                    FAULT_CHECK(this->POP<uint64_t>(new_ip));
                    FAULT_CHECK(this->POP<uint64_t>(new_cs));
                    FAULT_CHECK(this->POP<uint64_t>(new_flags));
                    goto start_checks;
                }
            }
        }
        FAULT_CHECK(this->POP<uint16_t>(new_ip));
        FAULT_CHECK(this->POP<uint16_t>(new_cs));
        FAULT_CHECK(this->POP<uint16_t>(new_flags));
    start_checks:
        if (!FAULTED(this->write_cs_iret(new_ip, new_cs, new_flags))) {
            return this->JMPABS(new_ip);
        }
    fault:
        return FAULT;
    }

    inline EXCEPTION z86call real_mode_interrupt(uint8_t number) {
        // TODO check limit
        auto idt_base = this->idt_descriptor.base;
        size_t interrupt_addr = idt_base + (size_t)number << 2;

        this->PUSH(this->get_flags<uint16_t>());
        this->interrupt = false;
        bool prev_trap = this->trap;
        this->trap = false;
        this->PUSH(this->cs);
        this->PUSH(this->ip);

        this->rip = mem.read<uint16_t>(interrupt_addr);
        uint16_t new_cs = mem.read<uint16_t>(interrupt_addr + 2);
        this->cs = new_cs;
        this->set_descriptor_base(CS, (size_t)new_cs << 4);
        return NO_FAULT;
    }

    template <typename P>
    EXCEPTION z86call int_software_interrupt(const P& pc, uint8_t number, bool is_int1) {
        uint16_t selector;
        if (this->protected_mode) {
            // Supposedly these aren't supposed to be #DF,
            // but I don't see how that's possible.

            auto* new_descriptor = this->get_idt_descriptor(number);
            // Table limit check
            if (expect(!new_descriptor, false)) goto throw_gp_vector;
            // Type check
            uint32_t allowed_typesA;
            if constexpr (max_bits == 64) {
                if (this->is_long_mode()) {
                    allowed_typesA = DESCRIPTORM_INTERRUPT_GATE | DESCRIPTORM_TRAP_GATE;
                    goto check_type;
                }
            }
            if constexpr (max_bits == 32) {
                allowed_typesA = DESCRIPTORM_TASK_GATE | DESCRIPTORM_INTERRUPT_GATE16 | DESCRIPTORM_TRAP_GATE16 | DESCRIPTORM_INTERRUPT_GATE | DESCRIPTORM_TRAP_GATE;
            }
            else {
                allowed_typesA = DESCRIPTORM_TASK_GATE | DESCRIPTORM_INTERRUPT_GATE16 | DESCRIPTORM_TRAP_GATE16;
            }
        check_type:
            uint8_t descriptor_type = new_descriptor->get_full_type();
            if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_gp_vector;

            if (expect(!is_int1, true)) {
                // DPL check
                uint8_t gate_dpl = new_descriptor->get_dpl();
                if (expect(gate_dpl < this->cpl, false)) goto throw_gp_vector;
            }

            // Present check
            if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;

            switch (descriptor_type) {
                default: unreachable;
                case DESCRIPTOR_TASK_GATE:
                {
                    selector = new_descriptor->segment();
                    // Null check?
                    // Local bit check
                    if (expect((bool)(selector & 4), false)) goto throw_np_selector;
                    new_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!new_descriptor, false)) goto throw_np_selector;
                    // Type check
                    constexpr uint32_t allowed_typesB = max_bits == 32 ? DESCRIPTORM_TSS16 | DESCRIPTORM_TSS : DESCRIPTORM_TSS16;
                    if (expect(!(allowed_typesB & 1 << new_descriptor->get_full_type()), false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                    // Task switch
                    // TODO
                    return NO_FAULT;
                }
                case DESCRIPTOR_INTERRUPT_GATE16:
                case DESCRIPTOR_TRAP_GATE16:
                case DESCRIPTOR_INTERRUPT_GATE:
                case DESCRIPTOR_TRAP_GATE:
                {
                    selector = new_descriptor->segment();
                    // Null check
                    if (expect(!(selector & ~3), false)) goto throw_gp_selector;
                    auto* dest_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!dest_descriptor, false)) goto throw_gp_selector;
                    // Type check
                    uint8_t gate_descriptor_type = descriptor_type;
                    constexpr uint32_t allowed_typesB = DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                    descriptor_type = dest_descriptor->get_full_type();
                    if (expect(!(allowed_typesB & 1 << descriptor_type), false)) goto throw_gp_selector;
                    // DPL check
                    uint8_t new_dpl = dest_descriptor->get_dpl();
                    if (expect(this->cpl < new_dpl, false)) goto throw_gp_selector;
                    // Present check
                    if (expect(!dest_descriptor->is_present(), false)) goto throw_np_selector;

                    // WTF do I do with IST jank
                    switch (descriptor_type) {
                        default: unreachable;
                        case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                        case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                            if (new_dpl < this->cpl) {
                                auto old_sp = this->rsp;
                                uint16_t old_ss = this->ss;
                                {
                                    uint16_t temp_cs = selector;
                                    RT new_sp;
                                    if constexpr (max_bits >= 32) {
                                        if (!this->tss_size_16()) {
                                            if constexpr (max_bits == 64) {
                                                if (this->tss_size_64()) {
                                                    // Load TSS 64
                                                    selector = new_dpl;
                                                    this->rsp = this->tss_descriptor.get_ptr<z86TSS<64>>()->stacks[new_dpl].sp;
                                                    goto skip_gate_tss_checks;
                                                }
                                            }
                                            // Load TSS 32
                                            const auto* tss32 = this->tss_descriptor.get_ptr<z86TSS<32>>();
                                            new_sp = tss32->stacks[new_dpl].sp;
                                            selector = tss32->stacks[new_dpl].ss;
                                            goto gate_tss_checks;
                                        }
                                    }
                                    // Load TSS 16
                                    {
                                        const auto* tss16 = this->tss_descriptor.get_ptr<z86TSS<16>>();
                                        new_sp = tss16->stacks[new_dpl].sp;
                                        selector = tss16->stacks[new_dpl].ss;
                                    }
                                gate_tss_checks:
                                    // Null check
                                    if (expect(!(selector & ~3), false)) goto throw_ts_selector;
                                    // RPL check
                                    if (expect(new_dpl != (selector & 3), false)) goto throw_ts_selector;
                                    auto* ss_descriptor = this->get_descriptor(selector);
                                    // Table limit check
                                    if (expect(!ss_descriptor, false)) goto throw_ts_selector;
                                    // DPL check
                                    if (expect(new_dpl != ss_descriptor->get_dpl(), false)) goto throw_ts_selector;
                                    // Type check
                                    if (expect(!ss_descriptor->is_valid_for_stack(), false)) goto throw_ts_selector;
                                    // Present check
                                    if (expect(!ss_descriptor->is_present(), false)) goto throw_ss_selector;
                                    this->set_privilege_level(new_dpl);
                                    this->change_stack_size(ss_descriptor->is_big());
                                    ss_descriptor->set_accessed();
                                    this->load_descriptor(SS, ss_descriptor, ATTRIBUTES_RW);
                                    this->rsp = new_sp;
                                    this->ss = selector;
                                    selector = temp_cs;
                                }
                            skip_gate_tss_checks:
                                if constexpr (max_bits >= 32) {
                                    switch (gate_descriptor_type) {
                                        default: unreachable;
                                        case DESCRIPTOR_TRAP_GATE: case DESCRIPTOR_INTERRUPT_GATE:
                                            if constexpr (max_bits == 64) {
                                                if (this->is_long_mode()) {
                                                    this->PUSH<uint64_t>(old_ss);
                                                    this->PUSH<uint64_t>(old_sp);
                                                    goto after_sp_push;
                                                }
                                            }
                                            this->PUSH<uint32_t>(old_ss);
                                            this->PUSH<uint32_t>(old_sp);
                                            goto after_sp_push;
                                        case DESCRIPTOR_TRAP_GATE16: case DESCRIPTOR_INTERRUPT_GATE16:
                                            break;
                                    }
                                }
                                this->PUSH<uint16_t>(old_ss);
                                this->PUSH<uint16_t>(old_sp);
                            after_sp_push:;
                            }
                        case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                        case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                            uint32_t flags = this->get_flags<uint32_t>();
                            if constexpr (max_bits >= 32) {
                                switch (gate_descriptor_type) {
                                    default: unreachable;
                                    case DESCRIPTOR_INTERRUPT_GATE:
                                        this->interrupt = false;
                                    case DESCRIPTOR_TRAP_GATE:
                                        if constexpr (max_bits == 64) {
                                            if (this->is_long_mode()) {
                                                this->rip = new_descriptor->rip();
                                                this->PUSH<uint64_t>(flags);
                                                this->PUSH<uint64_t>(this->cs);
                                                this->PUSH<uint64_t>(pc.offset);
                                                if (interrupt_uses_error_code(number)) {
                                                    this->PUSH<uint64_t>(this->error_code);
                                                }
                                                goto after_stack_push;
                                            }
                                        }
                                        this->rip = new_descriptor->eip();
                                        this->PUSH<uint32_t>(flags);
                                        this->PUSH<uint32_t>(this->cs);
                                        this->PUSH<uint32_t>(pc.offset);
                                        if (interrupt_uses_error_code(number)) {
                                            this->PUSH<uint32_t>(this->error_code);
                                        }
                                        goto after_stack_push;
                                    case DESCRIPTOR_INTERRUPT_GATE16:
                                        this->interrupt = false;
                                    case DESCRIPTOR_TRAP_GATE16:
                                        break;
                                }
                            }
                            this->rip = new_descriptor->ip();
                            this->PUSH<uint16_t>(flags);
                            this->PUSH<uint16_t>(this->cs);
                            this->PUSH<uint16_t>(pc.offset);
                            if (interrupt_uses_error_code(number)) {
                                this->PUSH<uint16_t>(this->error_code);
                            }
                        after_stack_push:;
                    }
                    this->trap = false;
                    // NT, VM, RF also set to 0
                    new_descriptor = dest_descriptor;
                    // END CHECKS
                    // Set RPL to CPL
                    selector &= ~3;
                    selector |= this->cpl;
                    break;
                }
            }
            this->cs = selector;
            new_descriptor->set_accessed();
            if constexpr (max_bits >= 32) {
                uint8_t new_size = new_descriptor->get_long_bits();
                this->change_data_size(new_size);
                this->change_addr_size(new_size);
            }
            this->load_descriptor(CS, new_descriptor, descriptor_type & 2 ? ATTRIBUTES_RE : ATTRIBUTES_E);
            return NO_FAULT;
        }
        else {
            return this->real_mode_interrupt(selector);
        }

    throw_gp_vector:
        this->software_interrupt(IntGP, number << 3 | 2 | is_int1);
        return FAULT;
    throw_gp_selector:
        this->software_interrupt(IntGP, (selector & ~7) | is_int1);
        return FAULT;
    throw_np_vector:
        this->software_interrupt(IntNP, number << 3 | 2 | is_int1);
        return FAULT;
    throw_np_selector:
        this->software_interrupt(IntNP, (selector & ~7) | is_int1);
        return FAULT;
    throw_ss_selector:
        this->software_interrupt(IntSS, (selector & ~7) | is_int1);
        return FAULT;
    throw_ts_selector:
        this->software_interrupt(IntTS, (selector & ~7) | is_int1);
        return FAULT;
    }

    void handle_pending_interrupt(uint8_t number) {
        uint16_t selector;
        if (this->protected_mode) {
            uint16_t error_code = this->error_code;
            // Supposedly these aren't supposed to be #DF,
            // but I don't see how that's possible.

            auto* new_descriptor = this->get_idt_descriptor(number);
            // Table limit check
            if (expect(!new_descriptor, false)) goto throw_df;
            // Type check
            uint32_t allowed_typesA;
            if constexpr (max_bits == 64) {
                if (this->is_long_mode()) {
                    allowed_typesA = DESCRIPTORM_INTERRUPT_GATE | DESCRIPTORM_TRAP_GATE;
                    goto check_type;
                }
            }
            if constexpr (max_bits == 32) {
                allowed_typesA = DESCRIPTORM_TASK_GATE | DESCRIPTORM_INTERRUPT_GATE16 | DESCRIPTORM_TRAP_GATE16 | DESCRIPTORM_INTERRUPT_GATE | DESCRIPTORM_TRAP_GATE;
            }
            else {
                allowed_typesA = DESCRIPTORM_TASK_GATE | DESCRIPTORM_INTERRUPT_GATE16 | DESCRIPTORM_TRAP_GATE16;
            }
        check_type:
            uint8_t descriptor_type = new_descriptor->get_full_type();
            if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_df;

            // Present check
            if (expect(!new_descriptor->is_present(), false)) goto throw_df;

            switch (descriptor_type) {
                default: unreachable;
                case DESCRIPTOR_TASK_GATE:
                {
                    selector = new_descriptor->segment();
                    // Null check?
                    // Local bit check
                    if (expect((bool)(selector & 4), false)) goto throw_df;
                    new_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!new_descriptor, false)) goto throw_df;
                    // Type check
                    constexpr uint32_t allowed_typesB = max_bits == 32 ? DESCRIPTORM_TSS16 | DESCRIPTORM_TSS : DESCRIPTORM_TSS16;
                    if (expect(!(allowed_typesB & 1 << new_descriptor->get_full_type()), false)) goto throw_df;
                    // Present check
                    if (expect(!new_descriptor->is_present(), false)) goto throw_df;
                    // Task switch
                    // TODO
                    return;
                }
                case DESCRIPTOR_INTERRUPT_GATE16:
                case DESCRIPTOR_TRAP_GATE16:
                case DESCRIPTOR_INTERRUPT_GATE:
                case DESCRIPTOR_TRAP_GATE:
                {
                    selector = new_descriptor->segment();
                    // Null check
                    if (expect(!(selector & ~3), false)) goto throw_df;
                    auto* dest_descriptor = this->get_descriptor(selector);
                    // Table limit check
                    if (expect(!dest_descriptor, false)) goto throw_df;
                    // Type check
                    uint8_t gate_descriptor_type = descriptor_type;
                    constexpr uint32_t allowed_typesB = DESCRIPTORM_E_CODE | DESCRIPTORM_E_CODEA | DESCRIPTORM_ER_CODE | DESCRIPTORM_ER_CODEA | DESCRIPTORM_E_CCODE | DESCRIPTORM_E_CCODEA | DESCRIPTORM_ER_CCODE | DESCRIPTORM_ER_CCODEA;
                    descriptor_type = dest_descriptor->get_full_type();
                    if (expect(!(allowed_typesB & 1 << descriptor_type), false)) goto throw_df;
                    // Present check
                    if (expect(!dest_descriptor->is_present(), false)) goto throw_df;

                    // WTF do I do with IST jank
                    switch (descriptor_type) {
                        default: unreachable;
                        case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                        case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                        {
                            uint8_t new_dpl = dest_descriptor->get_dpl();
                            if (new_dpl < this->cpl) {
                                auto old_sp = this->rsp;
                                uint16_t old_ss = this->ss;
                                {
                                    uint16_t temp_cs = selector;
                                    RT new_sp;
                                    if constexpr (max_bits >= 32) {
                                        if (!this->tss_size_16()) {
                                            if constexpr (max_bits == 64) {
                                                if (this->tss_size_64()) {
                                                    // Load TSS 64
                                                    selector = new_dpl;
                                                    this->rsp = this->tss_descriptor.get_ptr<z86TSS<64>>()->stacks[new_dpl].sp;
                                                    goto skip_gate_tss_checks;
                                                }
                                            }
                                            // Load TSS 32
                                            const auto* tss32 = this->tss_descriptor.get_ptr<z86TSS<32>>();
                                            new_sp = tss32->stacks[new_dpl].sp;
                                            selector = tss32->stacks[new_dpl].ss;
                                            goto gate_tss_checks;
                                        }
                                    }
                                    // Load TSS 16
                                    {
                                        const auto* tss16 = this->tss_descriptor.get_ptr<z86TSS<16>>();
                                        new_sp = tss16->stacks[new_dpl].sp;
                                        selector = tss16->stacks[new_dpl].ss;
                                    }
                                gate_tss_checks:
                                    // Null check
                                    if (expect(!(selector & ~3), false)) goto throw_df;
                                    // RPL check
                                    if (expect(new_dpl != (selector & 3), false)) goto throw_df;
                                    auto* ss_descriptor = this->get_descriptor(selector);
                                    // Table limit check
                                    if (expect(!ss_descriptor, false)) goto throw_df;
                                    // DPL check
                                    if (expect(new_dpl != ss_descriptor->get_dpl(), false)) goto throw_df;
                                    // Type check
                                    if (expect(!ss_descriptor->is_valid_for_stack(), false)) goto throw_df;
                                    // Present check
                                    if (expect(!ss_descriptor->is_present(), false)) goto throw_df;
                                    this->set_privilege_level(new_dpl);
                                    this->change_stack_size(ss_descriptor->is_big());
                                    ss_descriptor->set_accessed();
                                    this->load_descriptor(SS, ss_descriptor, ATTRIBUTES_RW);
                                    this->rsp = new_sp;
                                    this->ss = selector;
                                    selector = temp_cs;
                                }
                            skip_gate_tss_checks:
                                if constexpr (max_bits >= 32) {
                                    switch (gate_descriptor_type) {
                                        default: unreachable;
                                        case DESCRIPTOR_TRAP_GATE: case DESCRIPTOR_INTERRUPT_GATE:
                                            if constexpr (max_bits == 64) {
                                                if (this->is_long_mode()) {
                                                    this->PUSH<uint64_t>(old_ss);
                                                    this->PUSH<uint64_t>(old_sp);
                                                    goto after_sp_push;
                                                }
                                            }
                                            this->PUSH<uint32_t>(old_ss);
                                            this->PUSH<uint32_t>(old_sp);
                                            goto after_sp_push;
                                        case DESCRIPTOR_TRAP_GATE16: case DESCRIPTOR_INTERRUPT_GATE16:
                                            break;
                                    }
                                }
                                this->PUSH<uint16_t>(old_ss);
                                this->PUSH<uint16_t>(old_sp);
                            after_sp_push:;
                            }
                        }
                        case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                        case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                            uint32_t flags = this->get_flags<uint32_t>();
                            if constexpr (max_bits >= 32) {
                                switch (gate_descriptor_type) {
                                    default: unreachable;
                                    case DESCRIPTOR_INTERRUPT_GATE:
                                        this->interrupt = false;
                                    case DESCRIPTOR_TRAP_GATE:
                                        if constexpr (max_bits == 64) {
                                            if (this->is_long_mode()) {
                                                this->PUSH<uint64_t>(flags);
                                                this->PUSH<uint64_t>(this->cs);
                                                this->PUSH<uint64_t>(this->rip);
                                                this->rip = new_descriptor->rip();
                                                if (interrupt_uses_error_code(number)) {
                                                    this->PUSH<uint64_t>(error_code);
                                                }
                                                goto after_stack_push;
                                            }
                                        }
                                        this->PUSH<uint32_t>(flags);
                                        this->PUSH<uint32_t>(this->cs);
                                        this->PUSH<uint32_t>(this->eip);
                                        this->rip = new_descriptor->eip();
                                        if (interrupt_uses_error_code(number)) {
                                            this->PUSH<uint32_t>(error_code);
                                        }
                                        goto after_stack_push;
                                    case DESCRIPTOR_INTERRUPT_GATE16:
                                        this->interrupt = false;
                                    case DESCRIPTOR_TRAP_GATE16:
                                        break;
                                }
                            }
                            this->PUSH<uint16_t>(flags);
                            this->PUSH<uint16_t>(this->cs);
                            this->PUSH<uint16_t>(this->ip);
                            this->rip = new_descriptor->ip();
                            if (interrupt_uses_error_code(number)) {
                                this->PUSH<uint16_t>(error_code);
                            }
                        after_stack_push:;
                    }
                    this->trap = false;
                    // NT, VM, RF also set to 0
                    new_descriptor = dest_descriptor;
                    // END CHECKS
                    // Set RPL to CPL
                    selector &= ~3;
                    selector |= this->cpl;
                    break;
                }
            }
            this->cs = selector;
            new_descriptor->set_accessed();
            if constexpr (max_bits >= 32) {
                uint8_t new_size = new_descriptor->get_long_bits();
                this->change_data_size(new_size);
                this->change_addr_size(new_size);
            }
            this->load_descriptor(CS, new_descriptor, descriptor_type & 2 ? ATTRIBUTES_RE : ATTRIBUTES_E);
        }
        else {
            if (expect(this->real_mode_interrupt(selector), false)) goto throw_df;
        }
        return;

    throw_df:
        if (number == IntDF) {
            this->triple_fault();
            unreachable;
        }
        this->software_interrupt(IntDF, 0);
        return this->handle_pending_interrupt(IntDF);
    }

    inline EXCEPTION VERR(uint16_t selector) {
        if (expect(this->protected_mode, true)) {
            // Null check
            if (expect(!(selector & ~3), false)) goto clear_zero;
            auto* descriptor = this->get_descriptor(selector);
            // Table limit check
            if (expect(!descriptor, false)) goto clear_zero;
            switch (descriptor->get_full_type()) {
                default: unreachable;
                case DESCRIPTOR_EMPTY: case DESCRIPTOR_TSS16: case DESCRIPTOR_LDT: case DESCRIPTOR_TSS16_BUSY:
                case DESCRIPTOR_CALL_GATE16: case DESCRIPTOR_TASK_GATE: case DESCRIPTOR_INTERRUPT_GATE16: case DESCRIPTOR_TRAP_GATE16:
                case DESCRIPTOR_UNUSED_8: case DESCRIPTOR_TSS: case DESCRIPTOR_UNUSED_10: case DESCRIPTOR_TSS_BUSY:
                case DESCRIPTOR_CALL_GATE: case DESCRIPTOR_UNUSED_13: case DESCRIPTOR_INTERRUPT_GATE: case DESCRIPTOR_TRAP_GATE:
                case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                    goto clear_zero;
                case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                case DESCRIPTOR_R_DATA: case DESCRIPTOR_R_DATAA:
                case DESCRIPTOR_RW_DATA: case DESCRIPTOR_RW_DATAA:
                case DESCRIPTOR_R_EDATA: case DESCRIPTOR_R_EDATAA:
                case DESCRIPTOR_RW_EDATA: case DESCRIPTOR_RW_EDATAA: {
                    uint8_t dpl = descriptor->get_dpl();
                    // CPL check
                    if (dpl < this->cpl || dpl < (selector & ~3)) {
                        goto clear_zero;
                    }
                }
                case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                    this->zero = true;
            }
            return NO_FAULT;
        clear_zero:
            this->zero = false;
            return NO_FAULT;
        }
        this->software_interrupt(IntUD);
        return FAULT;
    }

    inline EXCEPTION VERW(uint16_t selector) {
        if (expect(this->protected_mode, true)) {
            // Null check
            if (expect(!(selector & ~3), false)) goto clear_zero;
            auto* descriptor = this->get_descriptor(selector);
            // Table limit check
            if (expect(!descriptor, false)) goto clear_zero;
            switch (descriptor->get_full_type()) {
                default: unreachable;
                case DESCRIPTOR_EMPTY: case DESCRIPTOR_TSS16: case DESCRIPTOR_LDT: case DESCRIPTOR_TSS16_BUSY:
                case DESCRIPTOR_CALL_GATE16: case DESCRIPTOR_TASK_GATE: case DESCRIPTOR_INTERRUPT_GATE16: case DESCRIPTOR_TRAP_GATE16:
                case DESCRIPTOR_UNUSED_8: case DESCRIPTOR_TSS: case DESCRIPTOR_UNUSED_10: case DESCRIPTOR_TSS_BUSY:
                case DESCRIPTOR_CALL_GATE: case DESCRIPTOR_UNUSED_13: case DESCRIPTOR_INTERRUPT_GATE: case DESCRIPTOR_TRAP_GATE:
                case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                case DESCRIPTOR_R_DATA: case DESCRIPTOR_R_DATAA:
                case DESCRIPTOR_R_EDATA: case DESCRIPTOR_R_EDATAA:
                    goto clear_zero;
                case DESCRIPTOR_RW_DATA: case DESCRIPTOR_RW_DATAA:
                case DESCRIPTOR_RW_EDATA: case DESCRIPTOR_RW_EDATAA:
                    uint8_t dpl = descriptor->get_dpl();
                    // CPL check
                    if (dpl < this->cpl || dpl < (selector & ~3)) {
                        goto clear_zero;
                    }
                    this->zero = true;
            }
            return NO_FAULT;
        clear_zero:
            this->zero = false;
            return NO_FAULT;
        }
        this->software_interrupt(IntUD);
        return FAULT;
    }

    template <typename T>
    inline EXCEPTION LAR(T& dst, uint16_t selector) {
        if (expect(this->protected_mode, true)) {
            // Null check
            if (expect(!(selector & ~3), false)) goto clear_zero;
            auto* descriptor = this->get_descriptor(selector);
            // Table limit check
            if (expect(!descriptor, false)) goto clear_zero;
            switch (descriptor->get_full_type()) {
                default: unreachable;
                case DESCRIPTOR_TSS16: case DESCRIPTOR_TSS16_BUSY:
                case DESCRIPTOR_CALL_GATE16: case DESCRIPTOR_TASK_GATE:
                    if constexpr (max_bits == 64) {
                        if (this->is_long_mode()) {
                            goto clear_zero;
                        }
                    }
                    goto cpl_check;
                case DESCRIPTOR_TSS: case DESCRIPTOR_TSS_BUSY:
                case DESCRIPTOR_CALL_GATE:
                    if constexpr (max_bits > 16) {
                        goto cpl_check;
                    }
                case DESCRIPTOR_EMPTY: case DESCRIPTOR_UNUSED_8: case DESCRIPTOR_UNUSED_10: case DESCRIPTOR_UNUSED_13:
                case DESCRIPTOR_INTERRUPT_GATE16: case DESCRIPTOR_TRAP_GATE16:
                case DESCRIPTOR_INTERRUPT_GATE: case DESCRIPTOR_TRAP_GATE:
                    goto clear_zero;
                case DESCRIPTOR_LDT:
                case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                case DESCRIPTOR_R_DATA: case DESCRIPTOR_R_DATAA:
                case DESCRIPTOR_R_EDATA: case DESCRIPTOR_R_EDATAA:
                case DESCRIPTOR_RW_DATA: case DESCRIPTOR_RW_DATAA:
                case DESCRIPTOR_RW_EDATA: case DESCRIPTOR_RW_EDATAA:
                cpl_check:
                    uint8_t dpl = descriptor->get_dpl();
                    // CPL check
                    if (dpl < this->cpl || dpl < (selector & ~3)) {
                        goto clear_zero;
                    }
                    this->zero = true;
            }
            dst = descriptor->access_rights() << 8;
            return NO_FAULT;
        clear_zero:
            this->zero = false;
            return NO_FAULT;
        }
        this->software_interrupt(IntUD);
        return FAULT;
    }

    template <typename T>
    inline EXCEPTION LSL(T& dst, uint16_t selector) {
        if (expect(this->protected_mode, true)) {
            // Null check
            if (expect(!(selector & ~3), false)) goto clear_zero;
            auto* descriptor = this->get_descriptor(selector);
            // Table limit check
            if (expect(!descriptor, false)) goto clear_zero;
            switch (descriptor->get_full_type()) {
                default: unreachable;
                case DESCRIPTOR_TSS16: case DESCRIPTOR_TSS16_BUSY:
                    if constexpr (max_bits == 64) {
                        if (this->is_long_mode()) {
                            goto clear_zero;
                        }
                    }
                    goto cpl_check;
                case DESCRIPTOR_TSS: case DESCRIPTOR_TSS_BUSY:
                    if constexpr (max_bits > 16) {
                        goto cpl_check;
                    }
                case DESCRIPTOR_EMPTY: case DESCRIPTOR_UNUSED_8: case DESCRIPTOR_UNUSED_10: case DESCRIPTOR_UNUSED_13:
                case DESCRIPTOR_INTERRUPT_GATE16: case DESCRIPTOR_TRAP_GATE16:
                case DESCRIPTOR_INTERRUPT_GATE: case DESCRIPTOR_TRAP_GATE:
                case DESCRIPTOR_CALL_GATE16:
                case DESCRIPTOR_CALL_GATE: case DESCRIPTOR_TASK_GATE:
                    goto clear_zero;
                case DESCRIPTOR_LDT:
                case DESCRIPTOR_E_CODE: case DESCRIPTOR_E_CODEA:
                case DESCRIPTOR_ER_CODE: case DESCRIPTOR_ER_CODEA:
                case DESCRIPTOR_E_CCODE: case DESCRIPTOR_E_CCODEA:
                case DESCRIPTOR_ER_CCODE: case DESCRIPTOR_ER_CCODEA:
                case DESCRIPTOR_R_DATA: case DESCRIPTOR_R_DATAA:
                case DESCRIPTOR_R_EDATA: case DESCRIPTOR_R_EDATAA:
                case DESCRIPTOR_RW_DATA: case DESCRIPTOR_RW_DATAA:
                case DESCRIPTOR_RW_EDATA: case DESCRIPTOR_RW_EDATAA:
                cpl_check:
                    uint8_t dpl = descriptor->get_dpl();
                    // CPL check
                    if (dpl < this->cpl || dpl < (selector & ~3)) {
                        goto clear_zero;
                    }
                    this->zero = true;
            }
            dst = descriptor->limit();
            return NO_FAULT;
        clear_zero:
            this->zero = false;
            return NO_FAULT;
        }
        this->software_interrupt(IntUD);
        return FAULT;
    }

    inline EXCEPTION write_tr(uint16_t selector) {
        if (expect(this->protected_mode, true)) {
            // LDT check
            if (expect(selector & 4, false)) goto throw_gp_zero;
            if (expect(selector <= 3, false)) goto throw_gp_selector;
            auto* new_descriptor = this->get_gdt_descriptor(selector);
            // Table limit check
            if (expect(!new_descriptor, false)) goto throw_gp_selector;
            // Type check
            uint32_t allowed_typesA;
            if constexpr (max_bits == 64) {
                if (this->is_long_mode()) {
                    allowed_typesA = DESCRIPTORM_TSS;
                    goto check_type;
                }
            }
            if constexpr (max_bits == 32) {
                allowed_typesA = DESCRIPTORM_TSS16 | DESCRIPTORM_TSS;
            }
            else {
                allowed_typesA = DESCRIPTORM_TSS16;
            }
        check_type:
            uint8_t descriptor_type = new_descriptor->get_full_type();
            if (expect(!(allowed_typesA & 1 << descriptor_type), false)) goto throw_gp_selector;
            // Present check
            if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
            // END CHECKS
            new_descriptor->set_busy();
            this->load_descriptor(TSS, new_descriptor);
            return NO_FAULT;
        }
        this->software_interrupt(IntUD);
        return FAULT;
    throw_gp_zero:
        selector = 0;
    throw_gp_selector:
        this->software_interrupt(IntGP, selector);
        return FAULT;
    throw_np_selector:
        this->software_interrupt(IntNP, selector);
        return FAULT;
    }

    inline EXCEPTION write_ldtr(uint16_t selector) {
        if (expect(this->protected_mode, true)) {
            // Recursion check
            if (expect(selector & 4, false)) goto throw_gp_zero;
            if (selector > 3) {
                auto* new_descriptor = this->get_gdt_descriptor(selector);
                // Table limit check
                if (expect(!new_descriptor, false)) goto throw_gp_selector;
                // Type check
                if (expect(new_descriptor->get_full_type() != DESCRIPTOR_LDT, false)) goto throw_gp_selector;
                // Present check
                if (expect(!new_descriptor->is_present(), false)) goto throw_np_selector;
                if constexpr (max_bits == 64) {
                    if (!this->is_long_mode()) {
                        this->load_descriptor(LDT, (SEG_DESCRIPTOR<32>*)new_descriptor);
                        goto ret;
                    }
                }
                this->load_descriptor(LDT, new_descriptor);
            }
            else {
                // Marks the LDT as invalid
                this->set_descriptor_limit(LDT, 0);
            }
        ret:
            return NO_FAULT;
        }
        this->software_interrupt(IntUD);
        return FAULT;
    throw_gp_zero:
        selector = 0;
    throw_gp_selector:
        this->software_interrupt(IntGP, selector);
        return FAULT;
    throw_np_selector:
        this->software_interrupt(IntNP, selector);
        return FAULT;
    }
};

struct z86AddrSharedFuncs {
    template <typename T>
    static inline constexpr bool addr_fits_on_bus(size_t addr);

    template <typename T>
    static inline constexpr bool addr_crosses_page(size_t addr);

    static inline constexpr size_t z86call virt_to_phys(size_t addr);

    template <typename T, typename P>
    static inline EXCEPTION z86call write(P* self, const T& value, ssize_t offset);

    template <typename T = uint8_t, bool is_execute = false, typename V = std::remove_reference_t<T>, typename P>
    static inline V read(const P* self, ssize_t offset = 0);

    template <typename P>
    static inline uint32_t read_Iz(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint32_t read_advance_Iz(P* self);

    template <typename P>
    static inline uint32_t execute_Iz(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint32_t execute_advance_Iz(P* self);

    template <typename P>
    static inline int32_t read_Is(const P* self, ssize_t index = 0);

    template <typename P>
    static inline int32_t read_advance_Is(P* self);

    template <typename P>
    static inline int32_t execute_Is(const P* self, ssize_t index = 0);

    template <typename P>
    static inline int32_t execute_advance_Is(P* self);

    template <typename P>
    static inline uint64_t read_Iv(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint64_t read_advance_Iv(P* self);

    template <typename P>
    static inline uint64_t execute_Iv(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint64_t execute_advance_Iv(P* self);

    template <typename P>
    static inline uint64_t read_O(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint64_t read_advance_O(P* self);

    template <typename P>
    static inline uint64_t execute_O(const P* self, ssize_t index = 0);

    template <typename P>
    static inline uint64_t execute_advance_O(P* self);
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

    inline constexpr uint32_t as_raw() const {
        return this->raw;
    }
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

    inline constexpr uint32_t as_raw() const {
        return this->raw;
    }
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

    inline constexpr uint64_t as_raw() const {
        return this->raw;
    }
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

    inline constexpr uint64_t as_raw() const {
        return this->raw;
    }
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

    inline constexpr uint128_t as_raw() const {
        return this->raw;
    }
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

    inline constexpr uint128_t as_raw() const {
        return this->raw;
    }
};

template <size_t bits, bool protected_mode>
struct z86AddrImpl : z86AddrBase<bits, protected_mode> {

    // Offset Type
    using OT = z86AddrBase<bits, protected_mode>::OT;
    // Signed Offset Type
    using ST = make_signed_ex_t<OT>;
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

    inline constexpr const z86DescriptorCache<bits>& descriptor() const;

    inline constexpr size_t seg() const;

    inline constexpr size_t limit() const;

    inline constexpr size_t addr(ssize_t offset = 0) const {
        return this->seg() + this->ptr(offset);
    }

    template <typename T>
    inline constexpr size_t offset_wrap_sub(size_t wrap) const {
        return this->limit() - (wrap - 1);
    }

    template <typename T>
    inline constexpr size_t offset_wrap(ssize_t offset = 0) const {
        OT base = -(ST)(this->offset + offset);
        return base >= z86DataProperites<T>::size ? 0 : base;
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
    inline EXCEPTION z86call write(const T& value, ssize_t offset = 0) {
        return z86AddrSharedFuncs::write<T>(this, value, offset);
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read(ssize_t offset = 0) const {
        return z86AddrSharedFuncs::read<T, false>(this, offset);
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V execute(ssize_t offset = 0) const {
        return z86AddrSharedFuncs::read<T, protected_mode>(this, offset);
    }

    template <typename T = uint8_t>
    inline EXCEPTION z86call write_advance(const T& value, ssize_t index = sizeof(T)) {
        if (!FAULTED(this->write(value))) {
            this->offset += index;
            return NO_FAULT;
        }
        return FAULT;
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read_advance(ssize_t index = sizeof(V)) {
        V ret = this->read<V>();
        this->offset += index;
        return ret;
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V execute_advance(ssize_t index = sizeof(V)) {
        V ret = this->execute<V>();
        this->offset += index;
        return ret;
    }

    inline uint32_t read_Iz(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iz(this, index);
    }

    inline uint32_t read_advance_Iz() {
        return z86AddrSharedFuncs::read_advance_Iz(this);
    }

    inline uint32_t execute_Iz(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_Iz(this, index);
    }

    inline uint32_t execute_advance_Iz() {
        return z86AddrSharedFuncs::execute_advance_Iz(this);
    }

    inline int32_t read_Is(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Is(this, index);
    }

    inline int32_t read_advance_Is() {
        return z86AddrSharedFuncs::read_advance_Is(this);
    }

    inline int32_t execute_Is(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_Is(this, index);
    }

    inline int32_t execute_advance_Is() {
        return z86AddrSharedFuncs::execute_advance_Is(this);
    }

    inline uint64_t read_Iv(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iv(this, index);
    }

    inline uint64_t read_advance_Iv() {
        return z86AddrSharedFuncs::read_advance_Iv(this);
    }

    inline uint64_t execute_Iv(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_Iv(this, index);
    }

    inline uint64_t execute_advance_Iv() {
        return z86AddrSharedFuncs::execute_advance_Iv(this);
    }

    inline uint64_t read_O(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_O(this, index);
    }

    inline uint64_t read_advance_O() {
        return z86AddrSharedFuncs::read_advance_O(this);
    }

    inline uint64_t execute_O(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_O(this, index);
    }

    inline uint64_t execute_advance_O() {
        return z86AddrSharedFuncs::execute_advance_O(this);
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

    inline constexpr uint16_t as_raw() const {
        return this->offset;
    }
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

    inline constexpr uint32_t as_raw() const {
        return this->offset;
    }
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

    inline constexpr uint64_t as_raw() const {
        return this->offset;
    }
};

template <size_t max_bits, uint8_t descriptor_index>
struct z86AddrFixedImpl : z86AddrFixedBase<max_bits> {

    // Offset Type
    using OT = z86AddrFixedBase<max_bits>::OT;
    // Signed Offset Type
    using ST = make_signed_ex_t<OT>;
    // Far Type
    using FT = z86AddrFixedBase<max_bits>::FT;
    // Memory Addr Type
    using MT = z86AddrFixedBase<max_bits>::MT;

    inline constexpr z86AddrFixedImpl() : z86AddrFixedBase<max_bits>::z86AddrFixedBase() {}
    inline constexpr z86AddrFixedImpl(OT offset) : z86AddrFixedBase<max_bits>::z86AddrFixedBase(offset) {}
    inline constexpr z86AddrFixedImpl(FT offset) : z86AddrFixedBase<max_bits>::z86AddrFixedBase(offset) {}
    inline constexpr z86AddrFixedImpl(const z86AddrFixedImpl&) = default;

    inline constexpr size_t ptr(ssize_t offset = 0) const {
        return (OT)(this->offset + offset);
    }

    inline constexpr const z86DescriptorCache<max_bits>& descriptor() const;

    inline constexpr size_t seg() const;

    inline constexpr size_t limit() const;

    inline constexpr size_t addr(ssize_t offset = 0) const {
        return this->seg() + this->ptr(offset);
    }
    
    template <typename T>
    inline constexpr size_t offset_wrap_sub(size_t wrap) const {
        return this->limit() - (wrap - 1);
    }

    template <typename T>
    inline constexpr size_t offset_wrap(ssize_t offset = 0) const {
        OT base = -(ST)(this->offset + offset);
        /*
        const size_t non_wrapping_limit = this->limit() - (z86DataProperites<T>::size - 1);
        if (base <= non_wrapping_limit) {
            return 0;
        }
        return base - non_wrapping_limit;
        */
        return base >= z86DataProperites<T>::size ? 0 : base;
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
    inline EXCEPTION z86call write(const T& value, ssize_t offset = 0) {
        return z86AddrSharedFuncs::write<T>(this, value, offset);
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read(ssize_t offset = 0) const {
        return z86AddrSharedFuncs::read<T>(this, offset);
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V execute(ssize_t offset = 0) const {
        return z86AddrSharedFuncs::read<T, true>(this, offset);
    }

    template <typename T = uint8_t>
    inline EXCEPTION z86call write_advance(const T& value, ssize_t index = sizeof(T)) {
        if (!FAULTED(this->write(value))) {
            this->offset += index;
            return NO_FAULT;
        }
        return FAULT;
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V read_advance(ssize_t index = sizeof(V)) {
        V ret = this->read<V>();
        this->offset += index;
        return ret;
    }

    template <typename T = uint8_t, typename V = std::remove_reference_t<T>>
    inline V execute_advance(ssize_t index = sizeof(V)) {
        V ret = this->execute<V>();
        this->offset += index;
        return ret;
    }

    inline uint32_t read_Iz(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iz(this, index);
    }

    inline uint32_t read_advance_Iz() {
        return z86AddrSharedFuncs::read_advance_Iz(this);
    }

    inline uint32_t execute_Iz(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_Iz(this, index);
    }

    inline uint32_t execute_advance_Iz() {
        return z86AddrSharedFuncs::execute_advance_Iz(this);
    }

    inline int32_t read_Is(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Is(this, index);
    }

    inline int32_t read_advance_Is() {
        return z86AddrSharedFuncs::read_advance_Is(this);
    }

    inline int32_t execute_Is(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_Is(this, index);
    }

    inline int32_t execute_advance_Is() {
        return z86AddrSharedFuncs::execute_advance_Is(this);
    }

    inline uint64_t read_Iv(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_Iv(this, index);
    }

    inline uint64_t read_advance_Iv() {
        return z86AddrSharedFuncs::read_advance_Iv(this);
    }

    inline uint64_t execute_Iv(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_Iv(this, index);
    }

    inline uint64_t execute_advance_Iv() {
        return z86AddrSharedFuncs::execute_advance_Iv(this);
    }

    inline uint64_t read_O(ssize_t index = 0) const {
        return z86AddrSharedFuncs::read_O(this, index);
    }

    inline uint64_t read_advance_O() {
        return z86AddrSharedFuncs::read_advance_O(this);
    }

    inline uint64_t execute_O(ssize_t index = 0) const {
        return z86AddrSharedFuncs::execute_O(this, index);
    }

    inline uint64_t execute_advance_O() {
        return z86AddrSharedFuncs::execute_advance_O(this);
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

enum ModRMSpecialFlags {
    MODRM_DEFAULT = 0,
    MODRM_NO64    = 1,
    MODRM_NO16    = 2
};

enum OP_FLAGS : uint8_t {
    OP_DEFAULT      = 0b0000000,
    OP_BYTE         = 0b0000001, // Op is always byte regardless of prefix
    OP_NO_READ      = 0b0000010, // Op can write to memory without reading first
    OP_NO_REX       = 0b0000100, // Op cannot be encoded with a rex prefix
    OP_NO_ADDR16    = 0b0001000, // Op cannot be encoded with 16 bit addressing
    OP_NO_ADDR64    = 0b0010000, // Op cannot be encoded with 64 bit addressing
    OP_NO_DATA16    = 0b0100000, // Op cannot be encoded with 16 bit data
    OP_NO_DATA64    = 0b1000000, // Op cannot be encoded with 64 bit data
};

#define OP_IS_BYTE(...) ((bool)((__VA_ARGS__) & OP_BYTE))
#define OP_SKIPS_READ(...) ((bool)((__VA_ARGS__) & OP_NO_READ))
#define OP_IGNORES_REX(...) ((bool)((__VA_ARGS__) & OP_NO_REX))
#define OP_CANNOT_ADDR16(...) ((bool)((__VA_ARGS__) & OP_NO_ADDR16))
#define OP_CANNOT_ADDR64(...) ((bool)((__VA_ARGS__) & OP_NO_ADDR64))
#define OP_CANNOT_DATA16(...) ((bool)((__VA_ARGS__) & OP_NO_DATA16))
#define OP_CANNOT_DATA64(...) ((bool)((__VA_ARGS__) & OP_NO_DATA64))

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

    template <OP_FLAGS op_flags = OP_DEFAULT, typename P>
    uint32_t extra_length(const P& pc) const;

    template <OP_FLAGS op_flags = OP_DEFAULT, typename P>
    inline uint32_t length(const P& pc) const {
        return 1 + this->extra_length<op_flags>(pc);
    }

    template <OP_FLAGS op_flags = OP_DEFAULT, size_t imm_offset = 0, typename P>
    auto parse_memM(P& pc) const;
};

#define z86BaseTemplate size_t bits, size_t bus, uint64_t flagsA
#define z86BaseDefault z86Base<bits, bus, flagsA>

template <size_t bits, size_t bus = bits, uint64_t flagsA = 0>
struct z86Base :
    z86Reg<bits,
        (bool)(flagsA & FLAG_OLD_RESET_PC),
        (bool)(flagsA & FLAG_PROTECTED_MODE),
        (bool)(flagsA & FLAG_LONG_MODE),
        (bool)(flagsA & (FLAG_CPUID_X87 | FLAG_CPUID_MMX | FLAG_CPUID_3DNOW)),
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
    static inline constexpr bool MIN_DIV_FAULT = flagsA & FLAG_MIN_DIV_FAULT;
    static inline constexpr bool FAULTS_ARE_TRAPS = flagsA & FLAG_FAULTS_ARE_TRAPS;
    static inline constexpr bool DE_IS_TRAP = flagsA & FLAG_DE_IS_TRAP;
    static inline constexpr bool OLD_PUSH_SP = flagsA & FLAG_OLD_PUSH_SP;
    static inline constexpr bool OLD_RESET_PC = flagsA & FLAG_OLD_RESET_PC;
    static inline constexpr bool OLD_AAA = flagsA & FLAG_OLD_AAA;
    static inline constexpr bool WRAP_SEGMENT_MODRM = flagsA & FLAG_WRAP_SEGMENT_MODRM;
    static inline constexpr bool AAM_NO_DE = flagsA & FLAG_AAM_NO_DE;
    static inline constexpr bool UNMASK_ENTER = flagsA & FLAG_UNMASK_ENTER;
    static inline constexpr bool REP_BOUND = flagsA & FLAG_REP_BOUND;
    static inline constexpr bool REP_MUL_MISSTORE = flagsA & FLAG_REP_MUL_MISSTORE;
    static inline constexpr bool PROTECTED_MODE = flagsA & FLAG_PROTECTED_MODE;
    static inline constexpr bool HAS_XBTS_IBTS = flagsA & FLAG_XBTS_IBTS;
    static inline constexpr bool PAGING = flagsA & FLAG_PAGING;
    static inline constexpr bool HAS_OLD_CMPXCHG = flagsA & FLAG_OLD_CMPXCHG;
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

    using HT = z86BaseGPRs<bits>::HT;
    using RT = z86BaseGPRs<bits>::RT;
    using DT = z86BaseGPRs<bits>::DT;

    using SRT = make_signed_ex_t<RT>;
    
    inline constexpr const uint16_t& get_seg(uint8_t index) {
        if constexpr (WRAP_SEGMENT_MODRM) {
            index &= 3;
        }
        return this->get_seg_impl(index);
    }

    inline constexpr EXCEPTION write_seg(uint8_t index, uint16_t selector) {
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

    int8_t rep_type = NO_REP;

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
        return this->rep_type > NO_REP;
    }

    inline constexpr bool is_repc() const {
        if constexpr (OPCODES_V20 && !OPCODES_80386) {
            return this->rep_type >= REP_NC;
        }
        else {
            return false;
        }
    }

    inline void z86call update_parity(uint8_t val) {
        this->parity = !__builtin_parity(val);
    }

    template <typename T>
    inline void z86call update_pzs(T val) {
        using S = make_signed_ex_t<T>;
        this->update_parity(val);
        this->zero = !val;
        this->sign = (S)val > 0;
    }

    template <typename T = RT>
    inline EXCEPTION z86call JMPFABS(T new_ip, uint16_t new_cs) {
        return this->JMPFABS_impl(new_ip, new_cs);
    }

    template <typename P>
    inline EXCEPTION z86call JMPFABS(const P& pc) {
        if constexpr (bits == 64) {
            // TODO: Exception conditions
        }
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->JMPFABS(pc.execute<uint32_t>(), pc.execute<uint16_t>(4));
            }
        }
        return this->JMPFABS(pc.execute<uint16_t>(), pc.execute<uint16_t>(2));
    }

    template <OP_FLAGS op_flags = OP_DEFAULT>
    EXCEPTION PUSHS(uint8_t seg_index) {
        uint16_t seg = get_seg(seg_index);
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->PUSH<uint32_t>(seg);
            }
            if constexpr (!OP_CANNOT_DATA64(op_flags) && bits == 64) {
                if (this->data_size_64()) {
                    return this->PUSH<uint64_t>(seg);
                }
            }
        }
        return this->PUSH<uint16_t>(seg);
    }

    EXCEPTION PUSHA() {
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
                    return NO_FAULT;
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
                    return NO_FAULT;
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
                return NO_FAULT;
            }
        }
        this->PUSH16(this->ax);
        this->PUSH16(this->cx);
        this->PUSH16(this->dx);
        this->PUSH16(this->bx);
        this->PUSH16(temp);
        this->PUSH16(this->bp);
        this->PUSH16(this->si);
        this->PUSH16(this->di);
        return NO_FAULT;
    }

    template <OP_FLAGS op_flags = OP_DEFAULT, typename P>
    inline EXCEPTION z86call PUSHI(P& pc) {
        if constexpr (bits > 16) {
            if (!this->data_size_16()) {
                int32_t imm = pc.execute_advance<uint32_t>();
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->PUSH<int64_t>(imm);
                    }
                }
                return this->PUSH<int32_t>(imm);
            }
        }
        return this->PUSH<int16_t>(pc.execute_advance<int16_t>());
    }

    inline EXCEPTION z86call PUSHI(SRT val) {
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

    EXCEPTION z86call PUSHF() {
        if constexpr (bits > 16) {
            if (!this->data_size_16()) {
                uint32_t flags = this->get_flags<uint32_t>();
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->PUSH<uint64_t>(flags);
                    }
                }
                return this->PUSH<uint32_t>(flags);
            }
        }
        return this->PUSH(this->get_flags<uint16_t>());
    }

    template <OP_FLAGS op_flags = OP_DEFAULT>
    EXCEPTION POPS(uint8_t seg_index) {
        uint16_t seg;
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                FAULT_CHECK(this->POP<uint32_t>(seg));
                goto set_seg;
            }
            if constexpr (!OP_CANNOT_DATA64(op_flags) && bits == 64) {
                if (this->data_size_64()) {
                    FAULT_CHECK(this->POP<uint64_t>(seg));
                    goto set_seg;
                }
            }
        }
        FAULT_CHECK(this->POP(seg));
    set_seg:
        return this->write_seg(seg_index, seg);
    fault:
        return FAULT;
    }

    EXCEPTION POPA() {
        static union {
            uint32_t dont_care32;
            uint16_t dont_care16;
        };
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                if (this->stack_size_32()) {
                    return this->POP32<uint32_t>(this->rdi) ||
                           this->POP32<uint32_t>(this->rsi) ||
                           this->POP32<uint32_t>(this->rbp) ||
                           this->POP32<uint32_t>(dont_care32) ||
                           this->POP32<uint32_t>(this->rbx) ||
                           this->POP32<uint32_t>(this->rdx) ||
                           this->POP32<uint32_t>(this->rcx) ||
                           this->POP32<uint32_t>(this->rax);
                }
                else {
                    return this->POP16<uint32_t>(this->rdi) ||
                           this->POP16<uint32_t>(this->rsi) ||
                           this->POP16<uint32_t>(this->rbp) ||
                           this->POP16<uint32_t>(dont_care32) ||
                           this->POP16<uint32_t>(this->rbx) ||
                           this->POP16<uint32_t>(this->rdx) ||
                           this->POP16<uint32_t>(this->rcx) ||
                           this->POP16<uint32_t>(this->rax);
                }
            }
            if (this->stack_size_32()) {
                return this->POP32<uint16_t>(this->di) ||
                       this->POP32<uint16_t>(this->si) ||
                       this->POP32<uint16_t>(this->bp) ||
                       this->POP32<uint16_t>(dont_care16) ||
                       this->POP32<uint16_t>(this->bx) ||
                       this->POP32<uint16_t>(this->dx) ||
                       this->POP32<uint16_t>(this->cx) ||
                       this->POP32<uint16_t>(this->ax);
            }
        }
        return this->POP16<uint16_t>(this->di) ||
               this->POP16<uint16_t>(this->si) ||
               this->POP16<uint16_t>(this->bp) ||
               this->POP16<uint16_t>(dont_care16) ||
               this->POP16<uint16_t>(this->bx) ||
               this->POP16<uint16_t>(this->dx) ||
               this->POP16<uint16_t>(this->cx) ||
               this->POP16<uint16_t>(this->ax);
    }

    EXCEPTION POPF() {
        /*
        auto flags = this->POP();
        if constexpr (bits > 16) {
            if (!this->data_size_16()) {
                this->set_flags<uint32_t>(flags);
                return NO_FAULT;
            }
        }
        this->set_flags<uint16_t>(flags);
        return NO_FAULT;
        */
        uint32_t flags;
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                FAULT_CHECK(this->POP(flags));
            }
            else {
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        FAULT_CHECK(this->POP<uint64_t>(flags));
                    }
                    else goto popf16;
                }
                else goto popf16;
            }
            this->set_flags(flags);
            return NO_FAULT;
        }
    popf16:
        FAULT_CHECK(this->POP<uint16_t>(flags));
        this->set_flags<uint16_t>(flags);
        return NO_FAULT;
    fault:
        return FAULT;
    }

    void LOADALL2();
    void STOREALL();
    EXCEPTION LOADALL3();

    gnu_attr(minsize) bool z86call ARPL(uint16_t& dst, uint16_t src) {
        uint32_t temp = std::rotr(dst, 2);
        src <<= 14;
        bool ret = temp < src;
        this->zero = ret;
        if (ret) {
            dst = temp << 2 | src >> 14;
            //dst = shld(temp, src, 2);
        }
        return ret;
    }

    template <typename T>
    gnu_attr(minsize) inline EXCEPTION z86call ENTER_impl(uint16_t alloc, uint8_t nesting);

    // http://www.os2museum.com/wp/if-you-enter-you-might-not-leave/
    inline EXCEPTION z86call ENTER(uint16_t alloc, uint8_t nesting) {
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

    inline EXCEPTION z86call LEAVE() {
        if constexpr (bits > 16) {
            if constexpr (bits == 64) {
                if (this->stack_size_64()) {
                    this->rsp = this->rbp;
                    if (this->data_size_64()) {
                        return this->POP64(this->rbp);
                    }
                    else {
                        return this->POP64<uint64_t>(this->bp);
                    }
                }
            }
            if (this->data_size_32()) {
                if (this->stack_size_32()) {
                    this->esp = this->ebp;
                    return this->POP32(this->ebp);
                }
                else {
                    this->sp = this->bp;
                    return this->POP16(this->ebp);
                }
            }
            if (this->stack_size_32()) {
                this->esp = this->ebp;
                return this->POP32<uint32_t>(this->bp);
            }
        }
        this->sp = this->bp;
        return this->POP16(this->bp);
    }

    template <typename P>
    inline EXCEPTION z86call CALL(const P& pc) {
        auto next_ip = pc.offset;
        if constexpr (bits > 16) {
            if (!this->data_size_16()) {
                next_ip += 4;
                auto dest_ip = next_ip + pc.execute<int32_t>();
                if (this->data_size_32()) {
                    return this->CALLABS<uint32_t>(next_ip, dest_ip);
                }
                else {
                    return this->CALLABS<uint64_t>(next_ip, dest_ip);
                }
            }
        }
        next_ip += 2;
        return this->CALLABS<uint16_t>(next_ip, next_ip + pc.execute<int16_t>());
    }

    inline EXCEPTION z86call RET() {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->POP<uint32_t>(this->rip);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->POP(this->rip);
                }
            }
        }
        return this->POP<uint16_t>(this->rip);
    }

    template <typename P>
    inline EXCEPTION z86call RETI(const P& pc) {
        FAULT_CHECK(this->RET());
        if constexpr (bits > 16) {
            if constexpr (bits == 64) {
                if (this->stack_size_64()) {
                    this->rsp += pc.execute<uint16_t>();
                    return NO_FAULT;
                }
            }
            if (this->stack_size_32()) {
                this->esp += pc.execute<uint16_t>();
                return NO_FAULT;
            }
        }
        this->sp += pc.execute<uint16_t>();
        return NO_FAULT;
    fault:
        return FAULT;
    }

    template <uint8_t op_flags = OP_DEFAULT, typename P>
    inline EXCEPTION z86call JMP(const P& pc) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            auto new_ip = pc.offset + 1 + pc.execute<int8_t>();
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
                    this->rip = pc.offset + 4 + pc.execute<int32_t>();
                    return NO_FAULT;
                }
            }
            this->rip = (uint16_t)(pc.offset + 2 + pc.execute<int16_t>());
        }
        return NO_FAULT;
    }

    template <CONDITION_CODE cc, typename T>
    inline void z86call CMOVCC(T& dst, T src, bool val = true) {
        if (this->cond<cc>(val)) {
            dst = src;
        }
    }

    template <CONDITION_CODE cc, typename T>
    inline void z86call SETCC(T& dst, bool val = true) {
        dst = this->cond<cc>(val);
    }

    template <CONDITION_CODE cc, uint8_t op_flags = OP_DEFAULT, typename P>
    inline EXCEPTION z86call JCC(const P& pc, bool val = true) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            auto new_ip = pc.offset + 1;
            if (this->cond<cc>(val)) {
                new_ip += pc.execute<int8_t>();
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
                        new_ip += pc.execute<int32_t>();
                        goto set_rip;
                    }
                }
                new_ip += pc.execute<int16_t>();
            }
        set_rip:
            if constexpr (bits > 16) {
                if (this->data_size_16()) {
                    new_ip = (uint16_t)new_ip;
                }
            }
            this->rip = new_ip;
        }
        return NO_FAULT;
    }

    template <typename T, typename P>
    inline EXCEPTION z86call LOOP_impl(const P& pc, T& index) {
        auto new_ip = pc.offset + 1;
        if (--index) {
            new_ip += pc.execute<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        this->rip = new_ip;
        return NO_FAULT;
    }

    template <typename P>
    inline EXCEPTION z86call LOOP(const P& pc) {
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
    inline EXCEPTION z86call LOOPCC_impl(const P& pc, T& index, bool val) {
        auto new_ip = pc.offset + 1;
        if (--index || this->cond_Z(val)) {
            new_ip += pc.execute<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        this->rip = new_ip;
        return NO_FAULT;
    }

    template <typename P>
    inline EXCEPTION z86call LOOPCC(const P& pc, bool val) {
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
    inline EXCEPTION z86call JCXZ_impl(const P& pc, const T& index) {
        auto new_ip = pc.offset + 1;
        if (!index) {
            new_ip += pc.execute<int8_t>();
        }
        if constexpr (bits > 16) {
            if (this->data_size_16()) {
                new_ip = (uint16_t)new_ip;
            }
        }
        this->rip = new_ip;
        return NO_FAULT;
    }

    template <typename P>
    inline EXCEPTION z86call JCXZ(const P& pc) {
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
    inline EXCEPTION z86call CALLFABS(T next_ip, T new_ip, uint16_t new_cs) {
        return this->CALLFABS_impl(next_ip, new_ip, new_cs);
    }

    template <typename P>
    inline EXCEPTION z86call CALLFABS(const P& pc) {
        auto next_ip = pc.offset + 4;
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->CALLFABS<uint32_t>(next_ip + 2, pc.execute<uint32_t>(), pc.execute<uint16_t>(4));
            }
        }
        return this->CALLFABS<uint16_t>(next_ip, pc.execute<uint16_t>(), pc.execute<uint16_t>(2));
    }

    template <typename T>
    inline void z86call ADD(T& dst, T src) {
        using U = make_unsigned_ex_t<T>;
        using S = make_signed_ex_t<T>;
        T temp = dst;
        this->carry = add_would_overflow<U>(temp, src);
        this->overflow = add_would_overflow<S>(temp, src);
        T res = temp + src;
        this->update_pzs(res);
        this->auxiliary = (temp ^ src ^ res) & 0x10;
        dst = res;
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call ADD(T1& dst, T2 src) {
        return this->ADD<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call OR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst |= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call OR(T1& dst, T2 src) {
        return this->OR<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call ADC(T& dst, T src) {
        using U = make_unsigned_ex_t<T>;
        using S = make_signed_ex_t<T>;
        T temp = dst;
        T res = carry_add((U)temp, (U)src, this->carry);
        this->update_pzs(res);
        this->auxiliary = (temp ^ src ^ res) & 0x10;
        this->overflow = (S)(~(temp ^ src) & (temp ^ res)) < 0;
        dst = res;
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call ADC(T1& dst, T2 src) {
        return this->ADC<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call SBB(T& dst, T src) {
        using U = make_unsigned_ex_t<T>;
        using S = make_signed_ex_t<T>;
        T temp = dst;
        T res = carry_sub<U>(temp, src, this->carry);
        this->update_pzs(res);
        this->auxiliary = (temp ^ src ^ res) & 0x10;
        this->overflow = (S)(~(temp ^ src) & (temp ^ res)) < 0;
        dst = res;
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call SBB(T1& dst, T2 src) {
        return this->SBB<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call AND(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst &= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call AND(T1& dst, T2 src) {
        return this->AND<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call SUB(T& dst, T src) {
        using U = make_unsigned_ex_t<T>;
        using S = make_signed_ex_t<T>;
        T temp = dst;
        this->carry = sub_would_overflow<U>(temp, src);
        this->overflow = sub_would_overflow<S>(temp, src);
        T res = temp - src;
        this->update_pzs(res);
        this->auxiliary = (temp ^ src ^ res) & 0x10;
        dst = res;
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call SUB(T1& dst, T2 src) {
        return this->SUB<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call XOR(T& dst, T src) {
        this->carry = false;
        this->overflow = false;
        dst ^= src;
        this->update_pzs(dst);
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call XOR(T1& dst, T2 src) {
        return this->XOR<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call CMP(T dst, T src) {
        using U = make_unsigned_ex_t<T>;
        using S = make_signed_ex_t<T>;
        this->carry = sub_would_overflow<U>(dst, src);
        this->overflow = sub_would_overflow<S>(dst, src);
        T res = dst - src;
        this->update_pzs(res);
        this->auxiliary = (dst ^ src ^ res) & 0x10;
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call CMP(T1 dst, T2 src) {
        return this->CMP<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call TEST(T dst, T src) {
        this->carry = false;
        this->overflow = false;
        this->update_pzs((T)(dst & src));
    }

    template <typename T1, typename T2> requires(!std::is_same_v<T1, T2>)
    inline void z86call TEST(T1 dst, T2 src) {
        return this->TEST<T1>(dst, (T1)(make_signed_ex_t<T1>)src);
    }

    template <typename T>
    inline void z86call INC(T& dst) {
        using S = make_signed_ex_t<T>;
        T temp = dst;
        this->overflow = temp == (std::numeric_limits<S>::max)();
        this->auxiliary = (temp ^ 1 ^ temp + 1) & 0x10; // BLCMSK
        this->update_pzs(++temp);
        dst = temp;
    }

    template <typename T>
    inline void z86call DEC(T& dst) {
        using S = make_signed_ex_t<T>;
        T temp = dst;
        this->overflow = temp == (std::numeric_limits<S>::max)();
        this->auxiliary = (temp ^ 1 ^ temp - 1) & 0x10; // BLSMSK
        this->update_pzs(--temp);
        dst = temp;
    }

    template <typename T>
    inline void z86call NOT(T& dst) {
        dst = ~dst;
    }

    template <typename T>
    inline void z86call NEG(T& dst) {
        using S = make_signed_ex_t<T>;
        T temp = dst;
        this->carry = temp;
        this->overflow = (S)temp == (std::numeric_limits<S>::min)();
        this->auxiliary = temp & 0xF;
        temp = -(S)temp;
        this->update_pzs(temp);
        dst = temp;
    }

    template <typename T>
    inline void z86call XCHG(T& dst, T& src) {
        std::swap(dst, src);
    }

    inline void z86call CBW() {
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

    inline void z86call CWD() {
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

    template <typename T>
    inline auto MUL_impl(T lhs, T rhs) {
        using U = make_unsigned_ex_t<T>;
        using S = make_signed_ex_t<T>;
        using R = dbl_int_t<U>;

        R ret = lhs;
        ret *= (U)rhs;

        // Is this correct here?
        if constexpr (REP_INVERT_MUL) {
            if (expect(this->has_rep(), false)) {
                ret = -(S)ret;
            }
        }

        this->carry = this->overflow = ret >> bitsof(T);
        return ret;
    }

    template <typename T>
    inline void z86call MUL(T src) {
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

    template <typename T>
    inline auto IMUL_impl(T lhs, T rhs) {
        using U = make_unsigned_ex_t<T>;
        using S = make_signed_ex_t<T>;
        using R = dbl_int_t<S>;

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
    inline void z86call IMUL(T src) {
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
    inline void z86call IMUL(T& dst, T src) {
        dst = this->IMUL_impl(dst, src);
    }

    template <typename T, typename I>
    inline void z86call IMUL(T& dst, T src, I val) {
        using S = make_signed_ex_t<T>;
        dst = this->IMUL_impl(src, (T)(S)val);
    }

    template <typename T>
    inline EXCEPTION z86call DIV(T src) {
        if (expect(src != 0, true)) {
            using U = make_unsigned_ex_t<T>;
            using UD = dbl_int_t<U>;
            UD temp = this->read_AD<T>();
            UD quot = temp / src;

            if constexpr (DE_IS_TRAP) {
                this->A<T>() = quot;
                this->ADH<T>() = temp % src;
            }
            if (expect(quot > (std::numeric_limits<U>::max)(), false)) {
                return this->set_fault(IntDE);
            }
            if constexpr (!DE_IS_TRAP) {
                this->A<T>() = quot;
                this->ADH<T>() = temp % src;
            }
        }
        else {
            return this->set_fault(IntDE);
        }
        return NO_FAULT;
    }

    template <typename T>
    inline EXCEPTION z86call IDIV(T src) {
        if (expect(src != 0, true)) {
            using S = make_signed_ex_t<T>;
            //using U = make_unsigned_ex_t<T>;
            using SD = dbl_int_t<S>;

            SD temp = this->read_AD<T>();
            SD quot = temp / src;

            if constexpr (REP_INVERT_IDIV) {
                if (expect(this->has_rep(), false)) {
                    quot = -quot;
                }
            }

            if constexpr (DE_IS_TRAP) {
                this->A<T>() = quot;
                this->ADH<T>() = temp % src;
            }
            if constexpr (!MIN_DIV_FAULT) {
                //if ((U)(quot - (std::numeric_limits<S>::min)()) > (std::numeric_limits<U>::max)()) {
                if (expect(quot != (S)quot, false)) {
                    return this->set_fault(IntDE);
                }
            }
            else {
                if (expect(quot != (quot & (std::numeric_limits<S>::max)()), false)) {
                    return this->set_fault(IntDE);
                }
            }
            if constexpr (!DE_IS_TRAP) {
                this->A<T>() = quot;
                this->ADH<T>() = temp % src;
            }
        }
        else {
            return this->set_fault(IntDE);
        }
        return NO_FAULT;
    }

    inline void z86call ROL4(uint8_t& dst) {
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

    inline void z86call ROR4(uint8_t& dst) {
        // HACK: ROR4 AL does nothing
        if (&dst != &this->al) {
            uint8_t temp = dst;
            dst = this->al << 4 | temp >> 4;
            this->al = temp;
        }
    }

    template <typename T>
    inline void z86call ROL(T& dst, uint8_t count) {

        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = make_unsigned_ex_t<T>;
            using S = make_signed_ex_t<T>;
            
            T res = std::rotl<U>(dst, count);
            this->carry = res & 1;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
        }
    }

    template <typename T>
    inline void z86call ROR(T& dst, uint8_t count) {

        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = make_unsigned_ex_t<T>;
            using S = make_signed_ex_t<T>;

            T res = std::rotr<U>(dst, count);
            this->carry = res & 1;
            this->overflow = this->carry == (S)res < 0;
            dst = res;
        }
    }

    template <typename T>
    inline void z86call RCL(T& dst, uint8_t count) {

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
            using U = make_unsigned_ex_t<T>;
            using S = make_signed_ex_t<T>;

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
    inline void z86call RCR(T& dst, uint8_t count) {

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
            using U = make_unsigned_ex_t<T>;
            using S = make_signed_ex_t<T>;

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
    inline void z86call SHL(T& dst, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }
        if (count) {
            using S = make_signed_ex_t<T>;

            T temp = dst;
            this->carry = ((size_t)temp >> (bitsof(T) - count)) & 1;
            T res = (size_t)temp << count;
            this->update_pzs(res);
            this->overflow = this->carry == (S)res < 0;
            dst = res;
        }
    }

    template <typename T>
    inline void z86call SHR(T& dst, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using S = make_signed_ex_t<T>;

            T temp = dst;
            this->carry = ((size_t)temp >> (count - 1)) & 1;
            this->overflow = (S)temp < 0;
            dst = (size_t)temp >> count;
            this->update_pzs(dst);
        }
    }

    // Yay, jank
    template <typename T>
    inline void z86call SETMO(T& dst, uint8_t count) {
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
    inline void z86call SAR(T& dst, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            T temp = dst;
            this->carry = ((size_t)temp >> (count - 1)) & 1;
            this->overflow = false;
            dst = (ssize_t)temp >> count;
            this->update_pzs(dst);
        }
    }

    template <typename T>
    inline void z86call SHLD(T& dst, T src, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using DU = dbl_int_t<make_unsigned_ex_t<T>>;
            using S = make_signed_ex_t<T>;

            DU temp = src;
            temp |= (DU)dst << bitsof(T);
            this->carry = ((size_t)temp >> (bitsof(DU) - count)) & 1;
            dst = (size_t)temp << count;
            this->update_pzs(dst);
            this->overflow = this->carry == (S)dst < 0;
        }
    }

    template <typename T>
    inline void z86call SHRD(T& dst, T src, uint8_t count) {
        if constexpr (SHIFT_MASKING) {
            if constexpr (sizeof(T) < sizeof(uint64_t)) {
                count &= 0x1F;
            } else {
                count &= 0x3F;
            }
        }

        if (count) {
            using U = make_unsigned_ex_t<T>;
            using DU = dbl_int_t<U>;

            DU temp = dst;
            temp |= (DU)src << bitsof(T);
            this->carry = ((size_t)temp >> (count - 1)) & 1;
            dst = (size_t)temp >> count;
            this->update_pzs(dst);
            this->overflow = __builtin_parity((U)dst >> (bitsof(T) - 2) & 3);
        }
    }

    template <typename T>
    inline void z86call BT(T dst, T src) {
        assume(src < bitsof(T));
        using U = make_unsigned_ex_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
    }

    template <typename T>
    inline void z86call BTS(T& dst, T src) {
        assume(src < bitsof(T));
        using U = make_unsigned_ex_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
        dst |= mask;
    }
    template <typename T>
    inline void z86call BTR(T& dst, T src) {
        assume(src < bitsof(T));
        using U = make_unsigned_ex_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
        dst &= ~mask;
    }

    template <typename T>
    inline void z86call BTC(T& dst, T src) {
        assume(src < bitsof(T));
        using U = make_unsigned_ex_t<T>;

        const U mask = (U)1 << src;
        this->carry = dst & mask;
        dst ^= mask;
    }

    // TODO: More research
    template <typename T1, typename T2>
    inline void z86call IBTS(T1& dst, T2 src, uint8_t offset, uint8_t count) {
        assume(offset < bitsof(T2) && count < bitsof(T2));

        const T2 low_mask = ((T2)1 << count) - 1;
        src &= low_mask;
        const T1 high_mask = (T1)low_mask << offset;
        dst = (dst & ~high_mask) | (T1)src << offset;
    }

    // TODO: More research.
    // Offset is handled in the function responsible for calling this.
    template <typename T>
    inline void z86call XBTS(T& dst, T src, uint8_t count) {
        assume(count < bitsof(T));
        dst = src & (((T)1 << count) - 1);
    }

    template <typename T>
    inline void z86call BSF(T& dst, T src) {
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
    inline void z86call BSR(T& dst, T src) {
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
    inline void z86call TEST1(T dst, uint8_t count) {
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
        using U = make_unsigned_ex_t<T>;

        this->zero = dst & (U)1 << count;
        // Some docs say carry/overflow are set to 0
        this->carry = this->overflow = false;
    }

    template <typename T>
    inline void z86call SET1(T& dst, uint8_t count) {
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
        using U = make_unsigned_ex_t<T>;

        const U mask = (U)1 << count;
        dst |= mask;
    }


    template <typename T>
    inline void z86call CLR1(T& dst, uint8_t count) {
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
        using U = make_unsigned_ex_t<T>;

        const U mask = (U)1 << count;
        dst &= ~mask;
    }

    template <typename T>
    inline void z86call NOT1(T& dst, uint8_t count) {
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
        using U = make_unsigned_ex_t<T>;

        const U mask = (U)1 << count;
        dst ^= mask;
    }

    template <typename T>
    inline void z86call BSWAP(T& src) {
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
    inline EXCEPTION z86call LODS_impl();

    template <uint8_t op_flags = OP_DEFAULT>
    inline EXCEPTION z86call LODS() {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline EXCEPTION z86call MOVS_impl();

    template <uint8_t op_flags = OP_DEFAULT>
    inline EXCEPTION z86call MOVS() {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline EXCEPTION z86call STOS_impl();

    template <uint8_t op_flags = OP_DEFAULT>
    inline EXCEPTION z86call STOS() {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline EXCEPTION z86call SCAS_impl();

    template <uint8_t op_flags = OP_DEFAULT>
    inline EXCEPTION z86call SCAS() {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline EXCEPTION z86call CMPS_impl();

    template <uint8_t op_flags = OP_DEFAULT>
    inline EXCEPTION z86call CMPS() {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline EXCEPTION z86call ADD4S_impl();

    inline EXCEPTION z86call ADD4S() {
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
    inline void z86call port_out_impl(uint16_t port, T value) const;

    template <uint8_t op_flags = OP_DEFAULT>
    inline void z86call port_out(uint16_t port) const {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline T z86call port_in_impl(uint16_t port);

    template <uint8_t op_flags = OP_DEFAULT>
    inline void z86call port_in(uint16_t port) {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline EXCEPTION z86call OUTS_impl();

    template <uint8_t op_flags = OP_DEFAULT>
    gnu_attr(minsize) gnu_noinline EXCEPTION OUTS() {
        if constexpr (OP_IS_BYTE(op_flags)) {
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
    inline EXCEPTION z86call INS_impl();

    template <uint8_t op_flags = OP_DEFAULT>
    gnu_attr(minsize) gnu_noinline EXCEPTION INS() {
        if constexpr (OP_IS_BYTE(op_flags)) {
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

    inline void z86call fop_set(uint8_t opcode, uint8_t modrm) {
        if constexpr (CPUID_X87) {
            this->fop = (uint16_t)opcode << 8 | modrm;
        }
    }

    inline void z86call FINCSTP() {
        if constexpr (CPUID_X87) {
            ++this->stack_top;
        }
    }

    inline void z86call FDECSTP() {
        if constexpr (CPUID_X87) {
            --this->stack_top;
        }
    }

    inline constexpr long double& z86call FTOP() {
        if constexpr (CPUID_X87) {
            return this->st[this->stack_top & 7];
        }
    }

    inline constexpr void z86call FPUSH(long double value) {
        if constexpr (CPUID_X87) {
            this->st[--this->stack_top & 7] = value;
        }
    }

    inline constexpr long double z86call FPOP() {
        if constexpr (CPUID_X87) {
            return this->st[this->stack_top++ & 7];
        }
    }

    inline void z86call FXCH(long double& value) {
        if constexpr (CPUID_X87) {
            std::swap(this->FTOP(), value);
        }
    }

    template <CONDITION_CODE cc>
    inline void z86call FCMOVCC(long double rhs, bool val = true) {
        if constexpr (CPUID_X87) {
            if (this->cond<cc>(val)) {
                this->FTOP() = rhs;
            }
        }
    }

    inline void z86call FADD(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs += rhs;
        }
    }

    inline void z86call FADD(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FADD(this->FTOP(), rhs);
        }
    }

    inline void z86call FMUL(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs *= rhs;
        }
    }

    inline void z86call FMUL(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FMUL(this->FTOP(), rhs);
        }
    }

    inline void z86call FCOM(long double lhs, long double rhs) {
        if constexpr (CPUID_X87) {

        }
    }

    inline void z86call FCOM(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FCOM(this->FTOP(), rhs);
        }
    }

    inline void z86call FSUB(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs -= rhs;
        }
    }

    inline void z86call FSUB(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FSUB(this->FTOP(), rhs);
        }
    }

    inline void z86call FSUBR(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs = rhs - lhs;
        }
    }

    inline void z86call FSUBR(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FSUBR(this->FTOP(), rhs);
        }
    }

    inline void z86call FDIV(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs /= rhs;
        }
    }

    inline void z86call FDIV(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FDIV(this->FTOP(), rhs);
        }
    }

    inline void z86call FDIVR(long double& lhs, long double rhs) {
        if constexpr (CPUID_X87) {
            lhs = rhs / lhs;
        }
    }

    inline void z86call FDIVR(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FDIVR(this->FTOP(), rhs);
        }
    }

    inline void z86call FUCOM(long double& lhs, long double rhs) {

    }

    inline void z86call FUCOM(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FUCOM(this->FTOP(), rhs);
        }
    }

    inline void z86call FCOMI(long double& lhs, long double rhs) {

    }

    inline void z86call FCOMI(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FCOMI(this->FTOP(), rhs);
        }
    }

    inline void z86call FUCOMI(long double& lhs, long double rhs) {

    }
    
    inline void z86call FUCOMI(long double rhs) {
        if constexpr (CPUID_X87) {
            return this->FUCOMI(this->FTOP(), rhs);
        }
    }

    inline void z86call FFREE(uint8_t index) {

    }

    // TODO: Read microcode dump to confirm accurate behavior of BCD, there's reason to doubt official docs here
    inline void z86call AAA() {
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

    inline void z86call AAS() {
        if (this->auxiliary || (this->al & 0xF) > 9) {
            this->auxiliary = this->carry = true;
            this->ax -= 0x106; // Was this different on 8086 too?
        } else {
            this->carry = false;
        }
        this->al &= 0xF;
    }

    // Supposedly better implementation than official docs: https://www.righto.com/2023/01/understanding-x86s-decimal-adjust-after.html
    inline void z86call DAA() {
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

    inline void z86call DAS() {
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
    inline bool z86call AAM(uint8_t imm) {
        if (expect(imm != 0, true)) {
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

    inline void z86call AAD(uint8_t imm) {
        uint8_t temp = this->al + this->ah * imm;
        this->ax = temp;
        this->update_pzs(temp);
    }

    template <typename T>
    inline EXCEPTION z86call BOUND(T index, T lower, T upper) {
        using S = make_signed_ex_t<T>;

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
                return FAULT;
            }
        }
        else {
            if (!in_range_inclusive<S>(index, lower, upper)) {
                this->set_fault(IntBR);
                return FAULT;
            }
        }
        return NO_FAULT;
    }

    template <typename T, typename P>
    inline EXCEPTION z86call MASKMOV_impl(T& src, T mask);

    template <typename T>
    inline EXCEPTION z86call MASKMOV(T& src, T mask) {
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
    bool z86call PACKSS(T& dst, T src) {
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
    bool z86call PACKUS(T& dst, T src) {
        constexpr size_t src_vec_length = vector_length_v<T>;
        constexpr size_t dst_vec_length = src_vec_length * 2;
        using src_int = vector_type_t<T>;
        using dst_int = make_unsigned_ex_t<hlf_int_t<src_int>>;

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
    T z86call PUNPCKL(T dst, T src) {
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
    T z86call PUNPCKH(T dst, T src) {
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
    T z86call MOVLDUP(T src) {
        constexpr size_t vec_length = vector_length_v<T>;
        using src_int = vector_type_t<T>;

        vec<src_int, vec_length> ret;
        for (size_t i = 0; i < vec_length; i += 2) {
            ret[i] = src[i];
            ret[i + 1] = src[i];
        }
        return ret;
    }

    template <typename T>
    T z86call MOVHDUP(T src) {
        constexpr size_t vec_length = vector_length_v<T>;
        using src_int = vector_type_t<T>;

        vec<src_int, vec_length> ret;
        for (size_t i = 0; i < vec_length; i += 2) {
            ret[i] = src[i + 1];
            ret[i + 1] = src[i + 1];
        }
        return ret;
    }

    template <typename T>
    T z86call PAVG(T dst, T src) {
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
    T z86call PABS(T src) {
        return __builtin_elementwise_abs(src);
    }

    template <typename T>
    T z86call PADD(T dst, T src) {
        return dst + src;
    }

    template <typename T>
    T z86call PADDS(T dst, T src) {
        return __builtin_elementwise_add_sat(dst, src);
    }

    template <typename T>
    T z86call PHADD(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return odds + evens;
    }

    template <typename T>
    T z86call PHADDS(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return __builtin_elementwise_add_sat(odds, evens);
    }

    template <typename T>
    T z86call PSAD(T dst, T src) {
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
    T z86call PSUB(T dst, T src) {
        return dst - src;
    }

    template <typename T>
    T z86call PSUBS(T dst, T src) {
        return __builtin_elementwise_sub_sat(dst, src);
    }

    template <typename T>
    T z86call PHSUB(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return odds - evens;
    }

    template <typename T>
    T z86call PHSUBS(T dst, T src) {
        T odds = vec_odd_interleave(dst, src);
        T evens = vec_even_interleave(dst, src);
        return __builtin_elementwise_sub_sat(odds, evens);
    }

    template <typename T>
    T z86call PMADD(T dst, T src) {
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
    T z86call PMULL(T dst, T src) {
        return dst * src;
    }

    template <typename T>
    T z86call PMULH(T dst, T src) {
        constexpr size_t vec_length = vector_length_v<T>;
        using src_int = vector_type_t<T>;
        using tmp_int = dbl_int_t<src_int>;

        vec<tmp_int, vec_length> temp = convertvec(dst, vec<tmp_int, vec_length>);
        temp *= convertvec(src, vec<tmp_int, vec_length>);
        temp >>= bitsof(src_int);
        return convertvec(temp, T);
    }

    template <typename T>
    T z86call PMUL(T dst, T src) {
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
    T1 z86call PSHL(T1 dst, T2 src) {
        return dst << src;
    }

    template <typename T1, typename T2>
    T1 z86call PSHR(T1 dst, T2 src) {
        return dst >> src;
    }

    template <typename T>
    T z86call PAND(T dst, T src) {
        return dst & src;
    }

    template <typename T>
    T z86call PANDN(T dst, T src) {
        return ~dst & src;
    }

    template <typename T>
    T z86call POR(T dst, T src) {
        return dst | src;
    }

    template <typename T>
    T z86call PXOR(T dst, T src) {
        return dst ^ src;
    }

    template <typename T>
    T z86call PMAX(T dst, T src) {
        return __builtin_elementwise_max(dst, src);
    }

    template <typename T>
    T z86call PMIN(T dst, T src) {
        return __builtin_elementwise_max(dst, src);
    }

    template <typename T>
    T z86call PCMPEQ(T dst, T src) {
        return dst == src;
    }

    template <typename T>
    T z86call PCMPGT(T dst, T src) {
        return dst > src;
    }

    template <typename T>
    T z86call PSIGN(T dst, T src) {
        const T zero = {};
        return src == 0 ? zero : src > 0 ? dst : -dst;
    }

    inline EXCEPTION z86call set_fault(uint8_t number) {
        this->software_interrupt(number);
        return !FAULTS_ARE_TRAPS;
    }

    inline void z86call set_trap(uint8_t number) {
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

    template <uint8_t op_flags = OP_DEFAULT, typename T, typename P>
    inline EXCEPTION z86call LEA_impl(P& pc);

    template <uint8_t op_flags = OP_DEFAULT, typename P>
    inline EXCEPTION z86call LEA(P& pc) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->LEA_impl<op_flags, uint32_t>(pc);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->LEA_impl<op_flags, uint64_t>(pc);
                }
            }
        }
        return this->LEA_impl<op_flags, uint16_t>(pc);
    }

    template <uint8_t op_flags = OP_DEFAULT, typename L>
    inline void z86call binopAR(uint32_t index, const L& lambda) {
        assume(index < 8);
        if constexpr (OP_IS_BYTE(op_flags)) {
            return lambda(this->al, this->index_byte_regMB(index));
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return lambda(this->eax, this->index_dword_regMB(index));
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return lambda(this->rax, this->index_qword_regMB(index));
                    }
                }
            }
            return lambda(this->ax, this->index_word_regMB(index));
        }
    }

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline void z86call binopAI(P& pc, const L& lambda) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return lambda(this->al, pc.execute_advance<uint8_t>());
        }
        else {
            if constexpr (bits > 16) {
                if (!this->data_size_16()) {
                    int32_t imm = pc.execute_advance<int32_t>();
                    if constexpr (bits == 64) {
                        if (this->data_size_64()) {
                            return lambda(this->rax, (uint64_t)(int64_t)imm);
                        }
                    }
                    return lambda(this->eax, (uint32_t)imm);
                }
            }
            return lambda(this->ax, pc.execute_advance<uint16_t>());
        }
    }

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopAO(P& pc, const L& lambda) {
        auto offset = pc.execute_advance_O();
        if constexpr (OP_IS_BYTE(op_flags)) {
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

    template <uint8_t op_flags = OP_DEFAULT, typename P>
    inline void z86call MOV_RI(P& pc, uint32_t index) {
        assume(index < 8);
        if constexpr (OP_IS_BYTE(op_flags)) {
            this->index_byte_regMB(index) = pc.execute_advance<uint8_t>();
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    this->index_dword_regMB(index) = pc.execute_advance<uint32_t>();
                    return;
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        this->index_qword_regMB(index) = pc.execute_advance<uint64_t>();
                        return;
                    }
                }
            }
            this->index_word_regMB(index) = pc.execute_advance<uint16_t>();
        }
    }

    template <typename T, typename P, typename L>
    inline EXCEPTION z86call MOVX(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopRM_impl<OP_NO_READ, uint32_t, T>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopRM_impl<OP_NO_READ, uint64_t, T>(pc, lambda);
                }
            }
        }
        return this->binopRM_impl<OP_NO_READ, uint16_t, T>(pc, lambda);
    }

    template <uint8_t op_flags, typename T1, typename T2 = T1, typename P, typename L>
    inline EXCEPTION z86call binopMR_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopMR(P& pc, const L& lambda) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return this->binopMR_impl<op_flags, uint8_t>(pc, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->binopMR_impl<op_flags, uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->binopMR_impl<op_flags, uint64_t>(pc, lambda);
                    }
                }
            }
            return this->binopMR_impl<op_flags, uint16_t>(pc, lambda);
        }
    }

    template <uint8_t op_flags, typename T1, typename T2 = T1, typename P, typename L>
    inline EXCEPTION z86call binopRM_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRM(P& pc, const L& lambda) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return this->binopRM_impl<op_flags, uint8_t>(pc, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->binopRM_impl<op_flags, uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->binopRM_impl<op_flags, uint64_t>(pc, lambda);
                    }
                }
            }
            return this->binopRM_impl<op_flags, uint16_t>(pc, lambda);
        }
    }

    template <uint8_t op_flags, typename T, typename P, typename L>
    inline EXCEPTION z86call binopRMW_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRMW(P& pc, const L& lambda) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return this->binopRMW_impl<op_flags, uint8_t>(pc, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->binopRMW_impl<op_flags, uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->binopRMW_impl<op_flags, uint64_t>(pc, lambda);
                    }
                }
            }
            return this->binopRMW_impl<op_flags, uint16_t>(pc, lambda);
        }
    }

    template <uint8_t op_flags, typename T, typename P, typename L>
    inline EXCEPTION z86call binopMRB_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopMRB(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopMRB_impl<op_flags, uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopMRB_impl<op_flags, uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopMRB_impl<op_flags, uint16_t>(pc, lambda);
    }

    template <uint8_t op_flags, typename T, typename P>
    inline EXCEPTION z86call IBTS_impl(P& pc);

    template <uint8_t op_flags = OP_DEFAULT, typename P>
    inline EXCEPTION z86call IBTS(P& pc) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->IBTS_impl<op_flags, uint32_t>(pc);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->IBTS_impl<op_flags, uint64_t>(pc);
                }
            }
        }
        return this->IBTS_impl<op_flags, uint16_t>(pc);
    }

    template <uint8_t op_flags, typename T, typename P>
    inline EXCEPTION z86call XBTS_impl(P& pc);

    template <uint8_t op_flags = OP_DEFAULT, typename P>
    inline EXCEPTION z86call XBTS(P& pc) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->XBTS_impl<op_flags, uint32_t>(pc);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->XBTS_impl<op_flags, uint64_t>(pc);
                }
            }
        }
        return this->XBTS_impl<op_flags, uint16_t>(pc);
    }

    template <uint8_t op_flags, typename T, typename P, typename L>
    inline EXCEPTION z86call binopRMF_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRMF(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopRMF_impl<op_flags, uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopRMF_impl<op_flags, uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopRMF_impl<op_flags, uint16_t>(pc, lambda);
    }

    template <uint8_t op_flags, typename T, typename P, typename L>
    inline EXCEPTION z86call binopRM2_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRM2(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopRM2_impl<op_flags, uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopRM2_impl<op_flags, uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopRM2_impl<op_flags, uint16_t>(pc, lambda);
    }

    template <uint8_t op_flags, typename T, typename P, typename L>
    inline EXCEPTION z86call binopMS_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopMS(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopMS_impl<op_flags, uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopMS_impl<op_flags, uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopMS_impl<op_flags, uint16_t>(pc, lambda);
    }

    template <uint8_t op_flags, typename T, typename P, typename L>
    inline EXCEPTION z86call binopSM_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopSM(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->binopSM_impl<op_flags, uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->binopSM_impl<op_flags, uint64_t>(pc, lambda);
                }
            }
        }
        return this->binopSM_impl<op_flags, uint16_t>(pc, lambda);
    }

    // Mm <- Rm
    template <typename T = uint64_t, uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopMR_MM(P& pc, const L& lambda);

    // Rm <- Mm
    template <typename T = uint64_t, uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRM_MM(P& pc, const L& lambda);

    // Mx <- Rx
    template <typename T = uint128_t, uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopMR_XX(P& pc, const L& lambda);

    // Rx <- Mx
    template <typename T = uint128_t, uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRM_XX(P& pc, const L& lambda);

    // Rm <- Mx
    template <typename T, uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRM_MX(P& pc, const L& lambda);

    // Rx <- Mm
    template <typename T, uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call binopRM_XM(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename T, typename P, typename L>
    inline EXCEPTION z86call unopR_impl(P& pc, uint8_t reg, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call unopR(P& pc, uint8_t reg, const L& lambda) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return this->unopR_impl<op_flags, uint8_t>(pc, reg, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->unopR_impl<op_flags, uint32_t>(pc, reg, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->unopR_impl<op_flags, uint64_t>(pc, reg, lambda);
                    }
                }
            }
            return this->unopR_impl<op_flags, uint16_t>(pc, reg, lambda);
        }
    }

    template <uint8_t op_flags = OP_DEFAULT, typename T, typename P, typename L>
    inline EXCEPTION z86call unopM_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call unopM(P& pc, const L& lambda) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return this->unopM_impl<op_flags, uint8_t>(pc, lambda);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->unopM_impl<op_flags, uint32_t>(pc, lambda);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->unopM_impl<op_flags, uint64_t>(pc, lambda);
                    }
                }
            }
            return this->unopM_impl<op_flags, uint16_t>(pc, lambda);
        }
    }

    template <uint8_t op_flags, typename T, typename P>
    inline bool z86call unopMS_impl(P& pc);

    template <uint8_t op_flags = OP_DEFAULT, typename P>
    inline bool z86call unopMS(P& pc) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return this->unopMS_impl<op_flags, uint8_t>(pc);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->unopMS_impl<op_flags, uint32_t>(pc);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->unopMS_impl<op_flags, uint64_t>(pc);
                    }
                }
            }
            return this->unopMS_impl<op_flags, uint16_t>(pc);
        }
    }

    template <uint8_t op_flags, typename T, typename P, typename L>
    inline EXCEPTION z86call unopMW_impl(P& pc, const L& lambda);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename L>
    inline EXCEPTION z86call unopMW(P& pc, const L& lambda) {
        if constexpr (bits > 16) {
            if (this->data_size_32()) {
                return this->unopMW_impl<op_flags, uint32_t>(pc, lambda);
            }
            if constexpr (bits == 64) {
                if (this->data_size_64()) {
                    return this->unopMW_impl<op_flags, uint64_t>(pc, lambda);
                }
            }
        }
        return this->unopMW_impl<op_flags, uint16_t>(pc, lambda);
    }

    template <uint8_t op_flags, typename T, typename P, typename LM, typename LR>
    inline EXCEPTION z86call unopMM_impl(P& pc, const LM& lambdaM, const LR& lambdaR);

    template <uint8_t op_flags = OP_DEFAULT, typename P, typename LM, typename LR>
    inline EXCEPTION z86call unopMM(P& pc, const LM& lambdaM, const LR& lambdaR) {
        if constexpr (OP_IS_BYTE(op_flags)) {
            return this->unopMM_impl<op_flags, uint8_t>(pc, lambdaM, lambdaR);
        }
        else {
            if constexpr (bits > 16) {
                if (this->data_size_32()) {
                    return this->unopMM_impl<op_flags, uint32_t>(pc, lambdaM, lambdaR);
                }
                if constexpr (bits == 64) {
                    if (this->data_size_64()) {
                        return this->unopMM_impl<op_flags, uint64_t>(pc, lambdaM, lambdaR);
                    }
                }
            }
            return this->unopMM_impl<op_flags, uint16_t>(pc, lambdaM, lambdaR);
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
        FLAG_PUSH_CS | FLAG_SAL_IS_SETMO | FLAG_REP_INVERT_MUL | FLAG_REP_INVERT_IDIV | FLAG_MIN_DIV_FAULT | FLAG_FAULTS_ARE_TRAPS | FLAG_DE_IS_TRAP | FLAG_NO_UD | FLAG_SINGLE_MEM_WRAPS | FLAG_UNMASK_SHIFTS | FLAG_OLD_PUSH_SP | FLAG_OLD_RESET_PC | FLAG_OLD_AAA | FLAG_WRAP_SEGMENT_MODRM
    > {};

template <uint64_t flagsA>
struct z86Core<z80186, flagsA> :
    z86Base<16, 16, flagsA |
        FLAG_REP_INVERT_IDIV | FLAG_DE_IS_TRAP | FLAG_SINGLE_MEM_WRAPS | FLAG_OLD_PUSH_SP | FLAG_OLD_RESET_PC | FLAG_OLD_AAA | FLAG_AAM_NO_DE | FLAG_UNMASK_ENTER | FLAG_REP_BOUND | FLAG_REP_MUL_MISSTORE |
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

template <uint64_t flagsA>
struct z86Core<z86_64, flagsA> :
    z86Base<
        64, 64, flagsA |
        FLAG_PROTECTED_MODE | FLAG_PAGING | FLAG_LONG_MODE |
        FLAG_OPCODES_80186 | FLAG_OPCODES_80286 | FLAG_OPCODES_80386 | FLAG_OPCODES_80486 | FLAG_OPCODES_P5 | FLAG_OPCODES_P6 | FLAG_HAS_CPUID | FLAG_HAS_LONG_NOP |
        FLAG_CPUID_X87 | FLAG_CPUID_CMOV | FLAG_CPUID_MMX | FLAG_CPUID_SSE | FLAG_CPUID_SSE2 | FLAG_CPUID_SSE3 | FLAG_CPUID_SSSE3 | FLAG_CPUID_SSE41 | FLAG_CPUID_SSE42 |
        FLAG_CPUID_SSE4A | FLAG_CPUID_3DNOW
    > {};

#endif