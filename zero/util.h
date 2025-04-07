#pragma once
#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#include "force_macros.h"

#define POINTER_64 __ptr64

#define C95_VERSION 199409L
#define C99_VERSION 199901L
#define C11_VERSION 201112L
#define C17_VERSION 201710L

#ifdef __STDC__
#define C89 1
#define C90 1
#endif
#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= C95_VERSION
#define C95 1
#endif
#if __STDC_VERSION__ >= C99_VERSION
#define C99 1
#endif
#if __STDC_VERSION__ >= C11_VERSION
#define C11 1
#endif
#if __STDC_VERSION__ >= C17_VERSION
#define C17 1
#endif
#if __STDC_VERSION__ > 201710L
#define C2X 1
#endif
#endif

#define CPP98_VERSION 199711L
#define CPP11_VERSION 201103L
#define CPP14_VERSION 201402L
#define CPP17_VERSION 201703L
#define CPP20_VERSION 202002L
#define CPP23_VERSION 999999L

#ifdef __cplusplus
#if __cplusplus >= CPP98_VERSION
#define CPP98 1
#endif
#if __cplusplus >= CPP11_VERSION
#define CPP11 1
#endif
#if __cplusplus >= CPP14_VERSION
#define CPP14 1
#endif
#if __cplusplus >= CPP17_VERSION
#define CPP17 1
#endif
#if __cplusplus >= CPP20_VERSION
#define CPP20 1
#endif
#define CPP23 0
#endif

#ifdef __clang__
#define CLANG_COMPAT 1
#define COMPILER_IS_CLANG 1
#endif
#ifdef _MSC_VER
#define MSVC_COMPAT 1
#ifndef COMPILER_IS_CLANG
#define COMPILER_IS_MSVC 1
#endif
#endif
#ifdef __GNUC__
#define GCC_COMPAT 1
#ifndef COMPILER_IS_CLANG
#define COMPILER_IS_GCC 1
#endif
#endif
#ifdef __MINGW32__
#define MINGW_COMPAT 1
#endif
#ifdef __INTEL_COMPILER
#define ICC_OLD_COMPAT 1
#define ICC_COMPAT 1
#endif
#ifdef __INTEL_LLVM_COMPILER
#define ICC_NEW_COMPAT 1
#define ICC_COMPAT 1
#endif

#ifdef _M_IX86
#define NATIVE_BITS 32
#else
#define NATIVE_BITS 64
#endif

typedef char* MS_va_list;
#if __INTELLISENSE__
#define va_use(va) (MS_va_list)(va)
#else
#define va_use(va) (va)
#endif

#include <stdarg.h>

#include <stddef.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <type_traits>
#include <new>
#include <utility>
#include <tuple>

//#define USE_ALL_FEATURES
//#define USE_AMD_FEATURES

#ifndef __SSE__
#define __SSE__ 1
#endif
#ifndef __SSE2__
#define __SSE2__ 1
#endif
#ifndef __SSSE3__
#define __SSSE3__ 1
#endif
#ifndef __SSE3__
#define __SSE3__ 1
#endif
#ifndef __SSE4_1__
#define __SSE4_1__ 1
#endif

//#define __SHA__

#ifdef USE_ALL_FEATURES
#ifndef __SSE4_2__
#define __SSE4_2__ 1
#endif
#ifndef __AVX__
#define __AVX__ 1
#endif
#ifndef __BMI__
#define __BMI__ 1
#endif
#ifndef __FMA__
#define __FMA__ 1
#endif
#ifndef __LZCNT__
#define __LZCNT__ 1
#endif
#ifndef __POPCNT__
#define __POPCNT__ 1
#endif
#ifndef __F16C__
#define __F16C__ 1
#endif
#endif

#ifdef USE_AMD_FEATURES
#ifndef __FMA4__
#define __FMA4__ 1
#endif
#ifndef __SSE4A__
#define __SSE4A__ 1
#endif
#ifndef __XOP__
#define __XOP__ 1
#endif
#ifndef __TBM__
#define __TBM__ 1
#endif
#endif

enum SseTier_t : uint8_t {
    IA32 = 0,
    MMX = 1,
    _3DNOW = 2,
    SSE = 3,
    SSE2 = 4,
    SSE3 = 5,
    SSSE3 = 6,
    SSE41 = 7,
    SSE42 = 8,
    AVX = 9,
    XOP = 10,
    AVX2 = 11,
    AVX512_basic = 12,
    AVX512_wide = 13,
    AVX512_full = 14,
    AVX512_fast = 15
};

#if __AVX512BF16__ && __AVX512IFMA__
static inline constexpr SseTier_t SSE_TIER = AVX512_fast;
#elif __AVX512BITALG__
static inline constexpr SseTier_t SSE_TIER = AVX512_full;
#elif __AVX512VL__ && __AVX512DQ__ && __AVX512BW__
static inline constexpr SseTier_t SSE_TIER = AVX512_wide;
#elif __AVX512F__
static inline constexpr SseTier_t SSE_TIER = AVX512_basic;
#elif __AVX2__
static inline constexpr SseTier_t SSE_TIER = AVX2;
#elif __XOP__
static inline constexpr SseTier_t SSE_TIER = XOP;
#elif __AVX__
static inline constexpr SseTier_t SSE_TIER = AVX;
#elif __SSE4_2__
static inline constexpr SseTier_t SSE_TIER = SSE42;
#elif __SSE4_1__
static inline constexpr SseTier_t SSE_TIER = SSE41;
#elif __SSSE3__
static inline constexpr SseTier_t SSE_TIER = SSSE3;
#elif __SSE3__
static inline constexpr SseTier_t SSE_TIER = SSE3;
#elif __SSE2__
static inline constexpr SseTier_t SSE_TIER = SSE2;
#elif __SSE__
static inline constexpr SseTier_t SSE_TIER = SSE;
#elif __3dNOW__
static inline constexpr SseTier_t SSE_TIER = _3DNOW;
#elif __MMX__
static inline constexpr SseTier_t SSE_TIER = MMX;
#else
static inline constexpr SseTier_t SSE_TIER = IA32;
#endif

//#undef __AVX__

#if __INTELLISENSE__
#define __building_module(e) 1
#endif

#include <intrin.h>

#define MACRO_VOID(...)

#if __INTELLISENSE__
// Attempt to make intellisense shut up about C++ features it doesn't understand...
#define INTELLISENSE_TYPENAME typename
#define requires(...) MACRO_EVAL(MACRO_VOID(__VA_ARGS__))
#define concept static inline constexpr bool
#define static_lambda
#define constexpr_lambda
#define consteval_lambda
#define ethis_template()
#define ethis_params(...) __VA_ARGS__
#define ethis_args(...) __VA_ARGS__
#define ethis_type std::remove_cvref_t<std::remove_pointer_t<decltype(this)>>
#define ethis (*this)
#else
#define INTELLISENSE_TYPENAME
#define requires(...) requires (__VA_ARGS__)
#define static_lambda static
#define constexpr_lambda constexpr
#define consteval_lambda consteval
#define ethis_template() template <typename Self>
#define ethis_params(...) __VA_ARGS__ __VA_OPT__(,) typename Self = void
#define ethis_args(...) this Self&& self __VA_OPT__(,) __VA_ARGS__
#define ethis_type Self
#define ethis self
#endif
#define typeof(type) std::remove_reference_t<decltype(type)>

//#pragma clang diagnostic error "-Winvalid-token-paste"

#define _MACRO_CAT(arg1, arg2) arg1 ## arg2
#define MACRO_CAT(arg1, arg2) _MACRO_CAT(arg1, arg2)
#define MACRO_CAT2(arg1, arg2, ...) _MACRO_CAT(arg1, arg2) __VA_ARGS__
#define MACRO_CATW_RAW(arg1, arg2, arg3) arg1 ## arg2 ## arg3
#define MACRO_CATW(arg1, arg2, arg3) MACRO_CATW_RAW(arg1, arg2, arg3)
#define _MACRO_STR(arg) #arg
#define MACRO_STR(arg) _MACRO_STR(arg)
#define MACRO_STRV(...) #__VA_ARGS__
#define MACRO_EVAL(...) __VA_ARGS__

// EoSD
#define MACRO_FIRST(arg1, ...) arg1
#define MACRO_FIRST_EVAL(...) MACRO_EVAL(MACRO_FIRST(__VA_ARGS__))
// PCB
#define MACRO_SECOND(arg1, arg2, ...) arg2
#define MACRO_SECOND_EVAL(...) MACRO_EVAL(MACRO_SECOND(__VA_ARGS__))
// IN
#define MACRO_THIRD(arg1, arg2, arg3, ...) arg3
#define MACRO_THIRD_EVAL(...) MACRO_EVAL(MACRO_THIRD(__VA_ARGS__))
// PoFV
#define MACRO_FOURTH(arg1, arg2, arg3, arg4, ...) arg4
#define MACRO_FOURTH_EVAL(...) MACRO_EVAL(MACRO_FOURTH(__VA_ARGS__))
// StB
#define MACRO_FIFTH(arg1, arg2, arg3, arg4, arg5, ...) arg5
#define MACRO_FIFTH_EVAL(...) MACRO_EVAL(MACRO_FIFTH(__VA_ARGS__))
// MoF
#define MACRO_SIXTH(arg1, arg2, arg3, arg4, arg5, arg6, ...) arg6
#define MACRO_SIXTH_EVAL(...) MACRO_EVAL(MACRO_SIXTH(__VA_ARGS__))
// UB
#define MACRO_SEVENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, ...) arg7
#define MACRO_SEVENTH_EVAL(...) MACRO_EVAL(MACRO_SEVENTH(__VA_ARGS__))
// SA
#define MACRO_EIGTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ...) arg8
#define MACRO_EIGTH_EVAL(...) MACRO_EVAL(MACRO_EIGTH(__VA_ARGS__))
// UFO
#define MACRO_NINTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ...) arg9
#define MACRO_NINTH_EVAL(...) MACRO_EVAL(MACRO_NINTH(__VA_ARGS__))
// DS
#define MACRO_TENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, ...) arg10
#define MACRO_TENTH_EVAL(...) MACRO_EVAL(MACRO_TENTH(__VA_ARGS__))
// GFW
#define MACRO_ELEVENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, ...) arg11
#define MACRO_ELEVENTH_EVAL(...) MACRO_EVAL(MACRO_ELEVENTH(__VA_ARGS__))
// TD
#define MACRO_TWELFTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, ...) arg12
#define MACRO_TWELFTH_EVAL(...) MACRO_EVAL(MACRO_TWELFTH(__VA_ARGS__))
// DDC
#define MACRO_THIRTEENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, ...) arg13
#define MACRO_THIRTEENTH_EVAL(...) MACRO_EVAL(MACRO_THIRTEENTH(__VA_ARGS__))
// ISC
#define MACRO_FOURTEENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, ...) arg14
#define MACRO_FOURTEENTH_EVAL(...) MACRO_EVAL(MACRO_FOURTEENTH(__VA_ARGS__))
// LoLK
#define MACRO_FIFTEENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, ...) arg15
#define MACRO_FIFTEENTH_EVAL(...) MACRO_EVAL(MACRO_FIFTEENTH(__VA_ARGS__))
// HSiFS
#define MACRO_SIXTEENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, ...) arg16
#define MACRO_SIXTEENTH_EVAL(...) MACRO_EVAL(MACRO_SIXTEENTH(__VA_ARGS__))
// VD
#define MACRO_SEVENTEENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, ...) arg17
#define MACRO_SEVENTEENTH_EVAL(...) MACRO_EVAL(MACRO_SEVENTEENTH(__VA_ARGS__))
// WBaWC
#define MACRO_EIGHTEENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, ...) arg18
#define MACRO_EIGHTEENTH_EVAL(...) MACRO_EVAL(MACRO_EIGHTEENTH(__VA_ARGS__))
// UM
#define MACRO_NINETEENTH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, ...) arg19
#define MACRO_NINETEENTH_EVAL(...) MACRO_EVAL(MACRO_NINETEENTH(__VA_ARGS__))
// HBM
#define MACRO_TWENTIETH(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, ...) arg20
#define MACRO_TWENTIETH_EVAL(...) MACRO_EVAL(MACRO_TWENTIETH(__VA_ARGS__))
// UDoALG
#define MACRO_TWENTY_FIRST(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, ...) arg21
#define MACRO_TWENTY_FIRST_EVAL(...) MACRO_EVAL(MACRO_TWENTY_FIRST(__VA_ARGS__))


//#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND(__VA_OPT__(,) __VA_ARGS__, (default_arg))

#define MACRO_COMMA ,

#define MACRO_EMPTY
#define _EMPTY(...) _EMPTY __VA_ARGS__
#define _MACRO_DEPAREN_OLD(...) MACRO ## __VA_ARGS__
#define MACRO_DEPAREN_OLD(...) _MACRO_DEPAREN_OLD(MACRO_EVAL(_EMPTY __VA_ARGS__))

#define __MACRO_DEPAREN(arg1, ...) arg1##__VA_ARGS__
#define _MACRO_DEPAREN(...) __MACRO_DEPAREN(MACRO,__VA_ARGS__)
#define MACRO_DEPAREN(...) MACRO_EVAL(_MACRO_DEPAREN(_EMPTY __VA_ARGS__))

#define _VA_OPT_SUPPORTED(...) __VA_OPT__(,)
#define VA_OPT_SUPPORTED MACRO_SECOND_EVAL(_VA_OPT_SUPPORTED(yeet) true,false)

// This is broken
#define _GCC_VA_OPT_SUPPORTED(...) ,__VA_ARGS__##__VA_ARGS__
#define GCC_VA_OPT_SUPPORTED MACRO_SECOND(_GCC_VA_OPT_SUPPORTED() false,true)

#define _IMPLICIT_VA_OPT_SUPPORTED(...) , __VA_ARGS__
#define IMPLICIT_VA_OPT_SUPPORTED MACRO_SECOND(_IMPLICIT_VA_OPT_SUPPORTED() false,true)

#define MACRO_IS_BLANK(arg) (!defined(arg) || ((MACRO_CAT(arg, 1)) == 1))

#if VA_OPT_SUPPORTED

// The sane and standardized way of doing it
// Unfortunately requires C++20

#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND(__VA_OPT__(,) __VA_ARGS__, default_arg)

#elif GCC_VA_OPT_SUPPORTED

// Many compilers accept this GCC extension

#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND(,##__VA_ARGS__ , default_arg)

#elif IMPLICIT_VA_OPT_SUPPORTED

// I have no idea why this works, but
// for whatever reason clang allows it

#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND(,__VA_ARGS__ , default_arg)

#else

// This is broken
// Black magic precessor voodoo

#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND_EVAL(MACRO_EVAL(MACRO_COMMA MACRO_FIRST_EVAL(MACRO_DEPAREN_OLD(__VA_ARGS__))()) default_arg, __VA_ARGS__, 0)

#endif

using ssize_t = std::make_signed_t<size_t>;

#if _WIN32 && !_HAS_CXX20
namespace std {
template <class _Ty>
using remove_cvref_t = _Remove_cvref_t<_Ty>;

template <class _Ty>
struct remove_cvref {
    using type = remove_cvref_t<_Ty>;
};
template <class _Callable, class... _Args>
inline constexpr bool is_invocable_v = _Select_invoke_traits<_Callable, _Args...>::_Is_invocable::value;
}
#endif // _HAS_CXX20

#define ENUM_MAX_VALUE_DECLARE(enum_name) \
MACRO_CATW(_,enum_name,_MAX_VALUE_DUMMY), \
MACRO_CAT(enum_name,_MAX_VALUE) = MACRO_CATW(_,enum_name,_MAX_VALUE_DUMMY) - 1

#define ENUM_VALUE_COUNT_DECLARE(enum_name) \
MACRO_CAT(enum_name,_VALUE_COUNT)

#define ENUM_MAX_VALUE(enum_name) MACRO_CAT(enum_name,_MAX_VALUE)
#define ENUM_VALUE_COUNT(enum_name) MACRO_CAT(enum_name,_VALUE_COUNT)

#define CACHE_LINE_SIZE 64
#define cache_align alignas(CACHE_LINE_SIZE)

#define gnu_attr(...) __attribute__((__VA_ARGS__))

#if COMPILER_IS_GCC
template<size_t bytes = 1, size_t alignment = 1>
struct gnu_attr(aligned(alignment),packed) PaddingBytes {
    unsigned char : bytes * sizeof(unsigned char) * CHAR_BIT;
};
#else
template<size_t bytes = 1, size_t alignment = 1>
struct alignas(alignment) PaddingBytes {
    _Pragma("pack(push, 1)");
    struct alignas(1) {
        unsigned char : CHAR_BIT;
    } PaddingBytesInner[bytes];
    _Pragma("pack(pop)");
};
#endif

#define unique_name(prefix)			MACRO_CAT(prefix, __COUNTER__)

#define unknown_name				unique_name(unknown_)
#define used_name					unique_name(used_)
#define dummy_name					unique_name(dummy_)
#define padding_name				unique_name(padding_)
#define unused_name					unique_name(unused_)
#define alignment_name				unique_name(alignment_)
#define alignment_separator_name	unique_name(alignment_separator_)
#define reserved_name				unique_name(reserved_)

#define unknown_fields(size)		unsigned char unknown_name[size]
#define unknown_bytes				unknown_fields
#define dummy_fields(size)			unsigned char dummy_name[size]
#define padding_bytes(size)			unsigned char padding_name[size]
#define unused_bytes(size)			unsigned char unused_name[size]
#define probably_padding_bytes		padding_bytes
#define alignment_bytes(size)		unsigned char alignment_name[size]
#define reserved_bytes(size)		unsigned char reserved_name[size]

#define unknown_bitfields(type, size) \
type unknown_name : size

#define padding_bitfields(type, size) \
unknown_bitfields((type), (size))

#define probably_padding_bitfields(type, size) \
padding_bitfields((type), (size))

#if MSVC_COMPAT
#define alignment_padding(align)	alignas(align) /##/
#elif GCC_COMPAT
#define alignment_padding(align)	alignas(align) unsigned char : 0
#endif

#define aligned_separator(align)	struct {}; alignment_padding(align)

#define PreferAlignmentBytesForUnusedFieldType 0

#if PreferAlignmentBytesForUnusedFieldType
#define unused_field_type(type, ...)	alignment_bytes(sizeof(type MACRO_DEFAULT_ARG(,[__VA_ARGS__])))
#else
#define unused_field_type(type, ...)	type unused_name MACRO_DEFAULT_ARG(,__VA_OPT__([__VA_ARGS__]))
#endif

#define cache_alignment_padding		alignment_padding(CACHE_LINE_SIZE)

// This makes C++ allow an always-failing static assert
// in the else block of an if-constexpr statement.
template<typename> inline constexpr bool dependent_false_v = false;

#define sfinae_enable(...) std::enable_if_t<(__VA_ARGS__), bool> = true

#define SFINAE_FAIL() static_assert(dependent_false_v<T>)

#define LogValidationOffsetFails 1

#if !LogValidationOffsetFails || COMPILER_IS_GCC || (COMPILER_IS_CLANG && __clang_major__ >= 16)

#define ValidateFieldOffset(offset, struct_type, member_name) \
static_assert((offset) == __builtin_offsetof(struct_type, member_name), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset)
#define ValidateStructSize(size, ...) \
static_assert((size) == sizeof(__VA_ARGS__), "Incorrect struct size! Size of " MACRO_STRV(__VA_ARGS__) " is not "#size)
#define ValidateStructAlignment(align, ...) \
static_assert((align) == alignof(__VA_ARGS__), "Incorrect struct alignment! Alignment of " MACRO_STRV(__VA_ARGS__) " is not "#align)

#if __INTELLISENSE__
#define ValidateVirtualFieldOffset(offset, struct_type, member_name) \
static_assert(((offset) - sizeof(void*)) == __builtin_offsetof(struct_type, member_name), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset)
#endif

#elif COMPILER_IS_MSVC

#define ValidateFieldOffset(offset, struct_type, member_name) \
struct { \
    template<size_t = 0> \
    static constexpr bool vfo() { \
        constexpr size_t test_value = __builtin_offsetof(struct_type, member_name); \
        constexpr size_t expected_value = (offset); \
        if constexpr (expected_value != test_value) { \
            using expected_val = char; \
            using test_val = char; \
            struct { test_val a[test_value]; } A; \
            struct { expected_val b[expected_value]; } B; \
            decltype(B.b)* b; \
            b = &A.a; \
        } \
        return expected_value == test_value; \
    } \
    static_assert(vfo(), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset); \
}

#define ValidateStructSize(size, ...) \
struct { \
    template<size_t = 0> \
    static constexpr bool vss() { \
        constexpr size_t test_value = sizeof(__VA_ARGS__); \
        constexpr size_t expected_value = (size); \
        if constexpr (expected_value != test_value) { \
            using expected_val = char; \
            using test_val = char; \
            struct { test_val a[test_value]; } A; \
            struct { expected_val b[expected_value]; } B; \
            decltype(B.b)* b; \
            b = &A.a; \
        } \
        return expected_value == test_value; \
    } \
    static_assert(vss(), "Incorrect struct size! Size of " MACRO_STRV(__VA_ARGS__) " is not "#size); \
}

#define ValidateStructAlignment(align, ...) \
struct { \
    template<size_t = 0> \
    static constexpr bool val() { \
        constexpr size_t test_value = alignof(__VA_ARGS__); \
        constexpr size_t expected_value = (align); \
        if constexpr (expected_value != test_value) { \
            using expected_val = char; \
            using test_val = char; \
            struct { test_val a[test_value]; } A; \
            struct { expected_val b[expected_value]; } B; \
            decltype(B.b)* b; \
            b = &A.a; \
        } \
        return expected_value == test_value; \
    } \
    static_assert(val(), "Incorrect struct alignment! Alignment of " MACRO_STRV(__VA_ARGS__) " is not "#align); \
}

#if __INTELLISENSE__
#define ValidateVirtualFieldOffset(offset, struct_type, member_name) \
struct { \
    template<size_t = 0> \
    static constexpr bool vvfo() { \
        constexpr size_t test_value = __builtin_offsetof(struct_type, member_name); \
        constexpr size_t expected_value = (offset)-sizeof(void*); \
        if constexpr (expected_value != test_value) { \
            using expected_val = char; \
            using test_val = char; \
            struct { test_val a[test_value]; } A; \
            struct { expected_val b[expected_value]; } B; \
            decltype(B.b)* b; \
            b = &A.a; \
        } \
        return expected_value == test_value; \
    } \
    static_assert(vvfo(), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset); \
}
#endif

#elif COMPILER_IS_CLANG

template<size_t expected_value, size_t test_value, sfinae_enable(expected_value == test_value)>
static inline constexpr bool validate_impl() {
    return true;
}
template<size_t expected_value, size_t test_value, sfinae_enable(expected_value != test_value)>
static inline constexpr bool validate_impl() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvexing-parse"
    char checker(char(*)[expected_value]);
#pragma clang diagnostic pop
    char (*B)[test_value] = {checker(+B)};
    return false;
}

#define ValidateFieldOffset(offset, struct_type, member_name) \
struct MACRO_CAT(vfo,__COUNTER__) { \
    static inline constexpr bool vfo = validate_impl<(offset), __builtin_offsetof(struct_type, member_name)>(); \
    static_assert(vfo, "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset); \
}
#define ValidateStructSize(size, ...) \
struct MACRO_CAT(vss,__COUNTER__) { \
    static inline constexpr bool vss = validate_impl<(size), sizeof(__VA_ARGS__)>(); \
    static_assert(vss, "Incorrect struct size! Size of " MACRO_STRV(__VA_ARGS__) " is not "#size); \
}
#define ValidateStructAlignment(align, ...) \
struct MACRO_CAT(val,__COUNTER__) { \
    static inline constexpr bool val = validate_impl<(align), alignof(__VA_ARGS__)>(); \
    static_assert(val, "Incorrect struct alignment! Alignment of " MACRO_STRV(__VA_ARGS__) " is not "#align); \
}

#if __INTELLISENSE__
#define ValidateVirtualFieldOffset(offset, struct_type, member_name) \
struct MACRO_CAT(vfo,__COUNTER__) { \
    static inline constexpr bool vvfo = validate_impl<(offset) - sizeof(void*), __builtin_offsetof(struct_type, member_name)>(); \
    static_assert(vvfo, "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset); \
}
#endif

#endif

#if !__INTELLISENSE__
#define ValidateVirtualFieldOffset ValidateFieldOffset
#endif

#ifdef _M_IX86
#define ValidateFieldOffset32(offset, struct_type, member_name) ValidateFieldOffset(offset, struct_type, member_name)
#define ValidateVirtualFieldOffset32(offset, struct_type, member_name) ValidateVirtualFieldOffset(offset, struct_type, member_name)
#define ValidateStructSize32(size, ...) ValidateStructSize(size, __VA_ARGS__)
#define ValidateStructAlignment32(align, ...) ValidateStructAlignment(align, __VA_ARGS__)
#define ValidateFieldOffset64(offset, struct_type, member_name)
#define ValidateVirtualFieldOffset64(offset, struct_type, member_name)
#define ValidateStructSize64(size, ...)
#define ValidateStructAlignment64(align, ...)
#else
#define ValidateFieldOffset32(offset, struct_type, member_name)
#define ValidateVirtualFieldOffset32(offset, struct_type, member_name)
#define ValidateStructSize32(size, ...)
#define ValidateStructAlignment32(align, ...)
#define ValidateFieldOffset64(offset, struct_type, member_name) ValidateFieldOffset(offset, struct_type, member_name)
#define ValidateVirtualFieldOffset64(offset, struct_type, member_name) ValidateVirtualFieldOffset(offset, struct_type, member_name)
#define ValidateStructSize64(size, ...) ValidateStructSize(size, __VA_ARGS__)
#define ValidateStructAlignment64(align, ...) ValidateStructAlignment(align, __VA_ARGS__)
#endif

#define ValidateFieldOffsetX(offset32, offset64, struct_type, member_name) \
ValidateFieldOffset(offset32, struct_type<32>, member_name); \
ValidateFieldOffset(offset64, struct_type<64>, member_name)
#define ValidateVirtualFieldOffsetX(offset32, offset64, struct_type, member_name) \
ValidateVirtualFieldOffset(offset32, struct_type<32>, member_name); \
ValidateVirtualFieldOffset(offset64, struct_type<64>, member_name)
#define ValidateStructSizeX(size32, size64, struct_type) \
ValidateStructSize(size32, struct_type<32>); \
ValidateStructSize(size64, struct_type<64>)
#define ValidateStructAlignmentX(align32, align64, struct_type) \
ValidateStructAlignment(align32, struct_type<32>); \
ValidateStructAlignment(align64, struct_type<64>)

#define shuffle_vec __builtin_shufflevector
#define shufflevec __builtin_shufflevector
#define convert_vec __builtin_convertvector
#define convertvec __builtin_convertvector

// For struct members, the alignment attribute can only
// decrease alignment if the packed attribute is also used
#define gnu_aligned(alignment) gnu_attr(aligned(alignment))
#define unaligned __unaligned

#define bool_str(...) ((bool)(__VA_ARGS__) ? "true" : "false")

template <size_t bytes>
inline constexpr size_t align_mask = bytes - 1;

template <typename T, typename P>
static inline constexpr bool is_aligned(const P& value) {
    return !(value & sizeof(T) - 1);
}

/*
template<typename T, size_t count, bool is_aligned = false>
using vec = std::conditional_t<is_aligned, \
    T gnu_attr(__vector_size__(count * sizeof(T))), \
    unaligned T gnu_attr(__vector_size__(count * sizeof(T))) \
> gnu_attr(__aligned__(is_aligned ? count * sizeof(T) : 1));
*/

//template<typename T, size_t count>
//using vec = T gnu_attr(__vector_size__(count * sizeof(T)), __aligned__(count * sizeof(T)));

template <typename T, size_t count, bool is_aligned>
struct $vec_impl {
    using type gnu_attr(__vector_size__(count * sizeof(T)), __aligned__(alignof(T))) = T;
};

template <typename T, size_t count>
struct $vec_impl<T, count, true> {
    using type gnu_attr(__vector_size__(count * sizeof(T))) = T;
};

template <typename T, size_t count, bool is_aligned = false>
using vec = $vec_impl<T, count, is_aligned>::type;

template <typename T, typename=void>
struct is_vector : std::false_type {};

template <typename T>
struct is_vector<T, std::void_t<decltype(std::declval<T>()[0])>> {
private:
    using B = std::remove_reference_t<decltype(std::declval<T>()[0])>;
public:
    static constexpr bool value = !(std::is_pointer_v<T> || std::is_array_v<T> || std::is_class_v<T> || std::is_union_v<T>) && sizeof(B) <= sizeof(T);
};

template <typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template <typename T, typename=void>
struct vector_type : std::type_identity<T> {};

template <typename T>
struct vector_type<T, std::void_t<decltype(std::declval<T>()[0])>> {
    using type = std::remove_reference_t<decltype(std::declval<T>()[0])>;
};

template <typename T>
using vector_type_t = vector_type<T>::type;

template <typename T, typename=void>
struct vector_length : std::integral_constant<size_t, 0> {};

template <typename T>
struct vector_length<T, std::void_t<decltype(std::declval<T>()[0])>> {
    static constexpr size_t value = sizeof(T) / sizeof(vector_type_t<T>);
};

template <typename T>
inline constexpr size_t vector_length_v = vector_length<T>::value;

template <typename T, typename... U>
struct is_all_same {
    static constexpr bool value = (... && std::is_same_v<T, U>);
};

template <typename T, typename... U>
inline constexpr bool is_all_same_v = is_all_same<T, U...>::value;

template <typename T, typename... U>
struct is_all_same_size {
    static constexpr bool value = (... && (sizeof(T) == sizeof(U)));
};

template <typename T, typename... U>
inline constexpr bool is_all_same_size_v = is_all_same_size<T, U...>::value;

enum InlineState {
    DefaultInline,
    ForceInline,
    NoInline
};

#if !CPP20 && !(defined(__cpp_consteval) && __cpp_consteval >= 201811L)
#define consteval constexpr
#pragma message("consteval not supported, using constexpr instead")
#endif
#if !CPP20 && !(defined(__cpp_constinit) && __cpp_constinit >= 201907L)
#define constinit static
#pragma message("constinit not supported")
#endif
#if __INTELLISENSE__
#ifndef consteval
#define consteval constexpr
#endif
#ifndef constinit
#define constinit static
#endif
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuninitialized"

template <typename T>
static inline constexpr T garbage_value(void) {
    T garbage;
    return garbage;
}

#define GARBAGE_VALUE(type) garbage_value<type>()
#define GARBAGE_ARG(type) GARBAGE_VALUE(type)

#pragma clang diagnostic pop

#define UNUSED_ARG(...) [[maybe_unused]] __VA_ARGS__

#define assume(condition) __builtin_assume(condition)


#undef IN

#define EoSD_VER 60
#define PCB_VER 70
#define IN_VER 80
#define PoFV_VER 90
#define StB_VER 95
#define MoF_VER 100
#define UB_VER 103
#define SA_VER 110
#define UFO_VER 120
#define DS_VER 125
#define GFW_VER 128
#define TD_VER 130
#define DDC_VER 140
#define ISC_VER 143
#define LoLK_VER 150
#define HSiFS_VER 160
#define VS_VER 165
#define WBaWC_VER 170
#define UM_VER 180
#define HBM_VER 185
#define UDoALG_VER 190
enum GameVersion : size_t {
    EoSD = EoSD_VER,
    PCB = PCB_VER,
    IN = IN_VER,
    PoFV = PoFV_VER,
    StB = StB_VER,
    MoF = MoF_VER,
    UB = UB_VER,
    SA = SA_VER,
    UFO = UFO_VER,
    DS = DS_VER,
    GFW = GFW_VER,
    TD = TD_VER,
    DDC = DDC_VER,
    ISC = ISC_VER,
    LoLK = LoLK_VER,
    HSiFS = HSiFS_VER,
    VD = VS_VER,
    WBaWC = WBaWC_VER,
    UM = UM_VER,
    HBM = HBM_VER,
    UDoALG = UDoALG_VER
};

#define IN

#define ZUNListPlayNiceWithIntellisense 1

#if !ZUNListPlayNiceWithIntellisense
#define ZUNListNCast(ptr) (ptr)
#else
#define ZUNListNCast(ptr) ((N*)ptr)
#endif

#ifdef ZUNListIdkDefault
#define ZUNListIdkDefaultValue = ZUNListIdkDefault
#else
#define ZUNListIdkDefaultValue
#endif

template <typename T>
struct ZUNEmbeddedList {
    T* next;
    T* prev;
};

#define embedded_node embedded_node

template <typename T>
struct ZUNEmbeddedListR {
    using N = ZUNEmbeddedListR<T>;

    T* prev;
    T* next;

    inline T* data_ptr() {
        return (T*)((uintptr_t)this - offsetof(T, embedded_node));
    }

    inline void unlink_from_next() {
        if (T* next_node = this->next) {
            next_node->embedded_node.prev = this->prev;
        }
    }
    inline void unlink_from_prev() {
        this->prev->embedded_node.next = this->next;
    }

    inline void unlink() {
        this->prev->embedded_node.next = this->next;
        if (T* next_node = this->next) {
            next_node->embedded_node.prev = this->prev;
        }
    }

    inline void append(T* new_node) {
        new_node->embedded_node.prev = this->data_ptr();
        this->next = new_node;
    }

    inline void append(N* new_node) {
        new_node->prev = this->data_ptr();
        this->next = new_node->data_ptr();
    }

protected:
    template <typename L>
    static inline void for_each_impl(const L& lambda, T* data) {
        do {
            lambda(data);
            data = data->embedded_node.next;
        } while (data);
    }
    template <typename L>
    static inline void for_each_safe_impl(const L& lambda, T* data) {
        do {
            T* next_data = data->embedded_node.next;
            lambda(data);
            data = next_data;
        } while (data);
    }
    template <typename L>
    static inline T* find_if_impl(const L& lambda, T* data) {
        do {
            if (lambda(data)) return data;
            data = data->embedded_node.next;
        } while (data);
        return NULL;
    }
public:
    template <typename L>
    inline void for_each(const L& lambda) {
        return for_each_impl(lambda, this->data_ptr());
    }
    template <typename L>
    inline void for_each_safe(const L& lambda) {
        return for_each_safe_impl(lambda, this->data_ptr());
    }
    template <typename L>
    inline T* find_if(const L& lambda) {
        return find_if_impl(lambda, this->data_ptr());
    }
};

template <typename T>
struct ZUNEmbeddedListRHead : ZUNEmbeddedListR<T> {
    using N = ZUNEmbeddedListR<T>;

    template <typename L>
    inline void for_each(const L& lambda) {
        if (T* data = this->next) {
            return for_each_impl(lambda, data);
        }
    }
    template <typename L>
    inline void for_each_safe(const L& lambda) {
        if (T* data = this->next) {
            return for_each_safe_impl(lambda, data);
        }
    }
    template <typename L>
    inline T* find_if(const L& lambda) {
        if (T* data = this->next) {
            return find_if_impl(lambda, data);
        }
        return NULL;
    }
};

#if !ZUNListPlayNiceWithIntellisense

template <typename T, bool has_idk>
struct ZUNListBase;

template <typename T, bool has_idk>
struct ZUNListData {
    using N = ZUNListBase<T, false>;
    T* data;
    N* next;
    N* prev;
};
template <typename T>
struct ZUNListData<T, true> {
    using N = ZUNListBase<T, true>;
    T* data;
    N* next;
    N* prev;
    N* idk;
};

template <typename T, bool has_idk ZUNListIdkDefaultValue>
struct ZUNListBase : ZUNListData<T, has_idk> {
    using N = ZUNListBase;

    inline void initialize_with(T* data) requires (!has_idk) {
        this->data = data;
        this->next = NULL;
        this->prev = NULL;
    }

    inline void initialize_with(T* data) requires has_idk {
        this->data = data;
        this->next = NULL;
        this->prev = NULL;
        this->idk = NULL;
    }
#else

template <typename T, bool has_idk ZUNListIdkDefaultValue, bool link_type = has_idk>
struct ZUNListBase {
    using N = ZUNListBase<T, link_type, link_type>;
    T* data;
    N* next;
    N* prev;
    
    inline void initialize_with(T* data) {
        this->data = data;
        this->next = NULL;
        this->prev = NULL;
    }
#endif

    inline void unlink_from_next() {
        if (N* next_node = this->next) {
            next_node->prev = this->prev;
        }
    }
    inline void unlink_from_prev() {
        if (N* prev_node = this->prev) {
            prev_node->next = this->next;
        }
    }

    inline void unlink() {
        if (N* next_node = this->next) {
            next_node->prev = this->prev;
        }
        if (N* prev_node = this->prev) {
            prev_node->next = this->next;
        }
        this->next = NULL;
        this->prev = NULL;
    }

    inline void unlink_from_head(N*& head_node) {
        if (this == head_node) {
            head_node = this->next;
        }
    }

    inline void unlink_from_tail(N*& tail_node) {
        if (this == tail_node) {
            tail_node = this->prev;
        }
    }

    inline void prepend(N* new_node) {
        new_node->next = ZUNListNCast(this);
        this->prev = new_node;
    }

#if !ZUNListPlayNiceWithIntellisense
    inline void append(N* new_node) requires (!has_idk) {
        if (N* next_node = this->next) {
            next_node->prepend(new_node);
        }
        new_node->prepend(this);
    }

    inline void append(N* new_node) requires has_idk {
        if (N* next_node = this->next) {
            next_node->prepend(new_node);
        }
        if (this->idk) {
            this->idk = new_node;
        }
        new_node->prepend(this);
    }
#else
    inline void append(N* new_node) {
        if (N* next_node = this->next) {
            next_node->prepend(new_node);
        }
        new_node->prepend(this);
    }
#endif

protected:
    static inline void delete_each_impl(N* node) {
        for (N* next_node; node; node = next_node) {
            next_node = node->next;
            assume(node->data != NULL);
            delete node->data;
            delete node;
        }
    }
    template <typename L>
    static inline void for_each_node_impl(const L& lambda, N* node) {
        for (; node; node = node->next) {
            lambda(node);
        }
    }
    template <typename L>
    static inline void for_each_node_safe_impl(const L& lambda, N* node) {
        for (N* next_node; node; node = next_node) {
            next_node = node->next;
            lambda(node);
        }
    }
    template <typename L>
    static inline void for_each_impl(const L& lambda, N* node) {
        for (; node; node = node->next) {
            T* data = node->data;
            lambda(data);
        }
    }
    template <typename L>
    static inline void for_each_safe_impl(const L& lambda, N* node) {
        for (N* next_node; node; node = next_node) {
            next_node = node->next;
            T* data = node->data;
            if constexpr (std::is_invocable_v<L, T*, N*>) {
                lambda(data, node);
            } else {
                lambda(data);
            }
        }
    }
    template <typename L>
    static inline void for_each_safeB_impl(const L& lambda, N* node) {
        for (N* next_node; node; node = next_node) {
            T* data = node->data;
            next_node = node->next;
            if constexpr (std::is_invocable_v<L, T*, N*>) {
                lambda(data, node);
            } else {
                lambda(data);
            }
        }
    }
    template <typename L>
    static inline bool do_while_impl(const L& lambda, N* node) {
        for (; node; node = node->next) {
            T* data = node->data;
            if (!lambda(data)) return false;
        }
        return true;
    }
    template <typename L>
    static inline bool do_while_safe_impl(const L& lambda, N* node) {
        for (N* next_node; node; node = next_node) {
            next_node = node->next;
            T* data = node->data;
            if constexpr (std::is_invocable_v<L, T*, N*>) {
                if (!lambda(data, node)) return false;
            } else {
                if (!lambda(data)) return false;
            }
        }
        return true;
    }
    template <typename L>
    static inline bool do_while_safeB_impl(const L& lambda, N* node) {
        for (N* next_node; node; node = next_node) {
            T* data = node->data;
            next_node = node->next;
            if constexpr (std::is_invocable_v<L, T*, N*>) {
                if (!lambda(data, node)) return false;
            } else {
                if (!lambda(data)) return false;
            }
        }
        return true;
    }
    template <typename L>
    static inline T* find_if_impl(const L& lambda, N* node) {
        for (; node; node = node->next) {
            T* data = node->data;
            if (lambda(data)) return data;
        }
        return NULL;
    }
    template <typename L>
    static inline T* find_if_not_impl(const L& lambda, N* node) {
        for (; node; node = node->next) {
            T* data = node->data;
            if (!lambda(data)) return data;
        }
        return NULL;
    }
    template <typename L>
    static inline N* find_node_if_impl(const L& lambda, N* node) {
        for (; node; node = node->next) {
            T* data = node->data;
            if (lambda(data)) break;
        }
        return node;
    }
    template <typename L>
    static inline N* find_node_before_impl(const L& lambda, N* node) {
        for (N* next_node; (next_node = node->next); node = next_node) {
            T* data = next_node->data;
            if (lambda(data)) break;
        }
        return node;
    }
    template <typename L>
    static inline int32_t count_if_impl(const L& lambda, N* node) {
        int32_t ret = 0;
        for (; node; node = node->next) {
            T* data = node->data;
            if (lambda(data)) ++ret;
        }
        return ret;
    }
    template <typename L>
    static inline int32_t count_if_safe_impl(const L& lambda, N* node) {
        int32_t ret = 0;
        for (N* next_node; node; node = next_node) {
            next_node = node->next;
            T* data = node->data;
            if (lambda(data)) ++ret;
        }
        return ret;
    }
    template <typename L>
    static inline int32_t count_if_not_impl(const L& lambda, N* node) {
        int32_t ret = 0;
        for (N* next_node; node; node = next_node) {
            next_node = node->next;
            T* data = node->data;
            if (!lambda(data)) ++ret;
        }
        return ret;
    }
public:
    inline void delete_each() {
        return delete_each_impl((N*)this);
    }
    template <typename L>
    inline void for_each_node(const L& lambda) {
        return for_each_node_impl(lambda, (N*)this);
    }
    template <typename L>
    inline void for_each_node_safe(const L& lambda) {
        return for_each_node_safe_impl(lambda, (N*)this);
    }
    template <typename L>
    inline void for_each(const L& lambda) {
        return for_each_impl(lambda, (N*)this);
    }
    template <typename L>
    inline void for_each_safe(const L& lambda) {
        return for_each_safe_impl(lambda, (N*)this);
    }
    template <typename L>
    inline void for_each_safeB(const L& lambda) {
        return for_each_safeB_impl(lambda, (N*)this);
    }
    template <typename L>
    inline bool do_while(const L& lambda) {
        return do_while_impl(lambda, (N*)this);
    }
    template <typename L>
    inline bool do_while_safe(const L& lambda) {
        return do_while_safe_impl(lambda, (N*)this);
    }
    template <typename L>
    inline bool do_while_safeB(const L& lambda) {
        return do_while_safeB_impl(lambda, (N*)this);
    }
    template <typename L>
    inline T* find_if(const L& lambda) {
        return find_if_impl(lambda, (N*)this);
    }
    template <typename L>
    inline T* find_if_not(const L& lambda) {
        return find_if_not_impl(lambda, (N*)this);
    }
    template <typename L>
    inline N* find_node_if(const L& lambda) {
        return find_node_if_impl(lambda, (N*)this);
    }
    template <typename L>
    inline N* find_node_before(const L& lambda) {
        return find_node_before_impl(lambda, (N*)this);
    }
    template <typename L>
    inline int32_t count_if(const L& lambda) {
        return count_if_impl(lambda, (N*)this);
    }
    template <typename L>
    inline int32_t count_if_safe(const L& lambda) {
        return count_if_safe_impl(lambda, (N*)this);
    }
    template <typename L>
    inline int32_t count_if_not(const L& lambda) {
        return count_if_not_impl(lambda, (N*)this);
    }
};

#if ZUNListPlayNiceWithIntellisense
template <typename T>
struct ZUNListBase<T, true, true> : ZUNListBase<T, false, true> {
    using N = ZUNListBase<T, true, true>;
    N* idk;

    inline void initialize_with(T* data) {
        this->data = data;
        this->next = NULL;
        this->prev = NULL;
        this->idk = NULL;
    }

    inline void append(N* new_node) {
        if (N* next_node = this->next) {
            next_node->prepend(new_node);
        }
        if (this->idk) {
            this->idk = new_node;
        }
        new_node->prepend(this);
    }
};
#endif

template <typename T, bool has_idk ZUNListIdkDefaultValue>
struct ZUNListHeadDummyBase : ZUNListBase<T, has_idk> {
    using N = ZUNListBase<T, has_idk>;

    inline void delete_each() {
        return delete_each_impl(this->next);
    }
    template <typename L>
    inline void for_each_node(const L& lambda) {
        return for_each_node_impl(lambda, this->next);
    }
    template <typename L>
    inline void for_each_node_safe(const L& lambda) {
        return for_each_node_safe_impl(lambda, this->next);
    }
    template <typename L>
    inline void for_each(const L& lambda) {
        return for_each_impl(lambda, this->next);
    }
    template <typename L>
    inline void for_each_safe(const L& lambda) {
        return for_each_impl(lambda, this->next);
    }
    template <typename L>
    inline void for_each_safeB(const L& lambda) {
        return for_each_safeB_impl(lambda, this->next);
    }
    template <typename L>
    inline bool do_while(const L& lambda) {
        return do_while_impl(lambda, this->next);
    }
    template <typename L>
    inline bool do_while_safe(const L& lambda) {
        return do_while_safe_impl(lambda, this->next);
    }
    template <typename L>
    inline bool do_while_safeB(const L& lambda) {
        return do_while_safeB_impl(lambda, this->next);
    }
    template <typename L>
    inline T* find_if(const L& lambda) {
        return find_if_impl(lambda, this->next);
    }
    template <typename L>
    inline T* find_if_not(const L& lambda) {
        return find_if_not_impl(lambda, this->next);
    }
    template <typename L>
    inline N* find_node_if(const L& lambda) {
        return find_node_if_impl(lambda, this->next);
    }
    template <typename L>
    inline N* find_node_before(const L& lambda) {
        return find_node_before_impl(lambda, this->next);
    }
    template <typename L>
    inline int32_t count_if(const L& lambda) {
        return count_if_impl(lambda, this->next);
    }
    template <typename L>
    inline int32_t count_if_safe(const L& lambda) {
        return count_if_safe_impl(lambda, this->next);
    }
    template <typename L>
    inline int32_t count_if_not(const L& lambda) {
        return count_if_not_impl(lambda, this->next);
    }
};

#ifdef ZUNListIdkDefault
template <typename T>
using ZUNList = ZUNListBase<T, ZUNListIdkDefault>;
template <typename T>
using ZUNListHeadDummy = ZUNListHeadDummyBase<T, ZUNListIdkDefault>;
#endif

union ZUNMagic {
    char text[4];
    int32_t as_int;
    uint32_t as_uint;
};

union ZUNMagic16 {
    char text[2];
    int16_t as_int;
    uint16_t as_uint;
};

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

#define bitsof(type) \
(sizeof(type) * CHAR_BIT)

#define array_end_addr(array_var) \
(&((array_var)[countof(array_var)]))

template<typename T, size_t size>
constexpr size_t GetArrLength(T(&)[size]) { return size; }

template <typename T>
static consteval size_t sizeof_template_impl() {
    return sizeof(T);
}
template <template <typename...> typename T>
static consteval size_t sizeof_template_impl() {
    T<> temp;
    return sizeof(temp);
};

#define sizeof_template(...) (sizeof_template_impl<__VA_ARGS__>())

#if __has_builtin(__builtin_align_down)
#define AlignDownToMultipleOf2(val, mul) (__builtin_align_down((val),(mul)))
#else
#define AlignDownToMultipleOf2(val, mul) ((val) & -(mul))
#endif
#if __has_builtin(__builtin_align_up)
#define AlignUpToMultipleOf2(val, mul) (__builtin_align_up((val),(mul)))
#else
//#define AlignUpToMultipleOf2(val, mul) (((val) + (mul) - 1) & -(mul))
#define AlignUpToMultipleOf2(val, mul) AlignDownToMultipleOf2((val) + (mul)-1,(mul))
#endif

#define AlignDownToMultipleOf(val, mul) ((val) - ((val) % (mul)))
#define AlignUpToMultipleOf(val, mul) (AlignDownToMultipleOf((val),(mul)) + (mul))

#define AlignPtrDownToMultipleOf2(val, mul) ((decltype(val))AlignDownToMultipleOf2((uintptr_t)(val),(mul)))
#define AlignPtrUpToMultipleOf2(val, mul) ((decltype(val))AlignUpToMultipleOf2((uintptr_t)(val),(mul)))
#define AlignPtrDownToMultipleOf(val, mul) ((decltype(val))AlignDownToMultipleOf((uintptr_t)(val),(mul)))
#define AlignPtrUpToMultipleOf(val, mul) ((decltype(val))AlignUpToMultipleOf((uintptr_t)(val),(mul)))

#define dword_align(val) (size_t)AlignUpToMultipleOf2((size_t)(val), 4)
#define ptr_dword_align(val) (BYTE*)dword_align((uintptr_t)(val))

#define is_word_aligned(val) (!((uintptr_t)val & 1))
#define is_dword_aligned(val) (!((uintptr_t)val & 3))
#define is_qword_aligned(val) (!((uintptr_t)val & 7))

#define is_byte_aligned_to_word(val) is_word_aligned(val)
#define is_word_aligned_to_dword(val) (!((uintptr_t)val & 2))

template <typename P, typename O>
static inline P* pointer_raw_offset(P* pointer, O offset) {
    return (P*)(((intptr_t)pointer) + offset);
}

#define _identifier(name) __identifier(MACRO_STR(name))

#define gnu_packed gnu_attr(packed)
#define packed_field gnu_packed

#define clang_noinline [[clang::noinline]]
//#define noinline __declspec(noinline)
#define gnu_noinline gnu_attr(noinline)
#define forceinline __forceinline
#define clang_always_inline [[clang::always_inline]]
#define clang_forceinline clang_always_inline
#define gnu_always_inline gnu_attr(always_inline)
#define gnu_forceinline gnu_always_inline
#define restrict __restrict

#define naked __declspec(naked)

//#define fastcall __fastcall
#define fastcall gnu_attr(fastcall)
#ifdef cdecl
#undef cdecl
#endif
#ifdef _cdecl
#undef _cdecl
#endif
#ifdef __cdecl
#undef __cdecl
#endif
#define cdecl __cdecl

//#define thiscall __thiscall
#define thiscall gnu_attr(thiscall)
//#define stdcall __stdcall
#define stdcall gnu_attr(stdcall)
#define vectorcall gnu_attr(vectorcall)
#define dllexport __declspec(dllexport)
//#define dllimport __declspec(dllimport)
#define dll_export dllexport
#define dll_import __declspec(dllimport)
#define debug_export extern "C"
#define regparm(count) gnu_attr(regparm(count))
#define regcall gnu_attr(regcall)
#ifdef __pascal
#undef __pascal
#endif
#ifdef pascal
#undef pascal
#endif
#define pascalcall gnu_attr(pascal)
#define preserve_none gnu_attr(preserve_none)
#define preserve_most gnu_attr(preserve_most)
#define preserve_all gnu_attr(preserve_all)

#define sysv_abi gnu_attr(sysv_abi)
#define ms_abi gnu_attr(ms_abi)

//#define noreturn [[noreturn]]
#define no_stack_protector gnu_attr(no_stack_protector)
#define expect __builtin_expect

#define GS_RELATIVE gnu_attr(address_space(256))
#define FS_RELATIVE gnu_attr(address_space(257))
#define SS_RELATIVE gnu_attr(address_space(258))
#if CUSTOM_CLANG
#define CS_RELATIVE gnu_attr(address_space(259))
#define DS_RELATIVE gnu_attr(address_space(260))
#define ES_RELATIVE gnu_attr(address_space(261))
#endif

#if !__has_builtin(__builtin_expect_with_probability)
#define __builtin_expect_with_probability(cond, ...) (cond)
#endif
#define expect_chance __builtin_expect_with_probability

#define unpredictable(condition) __builtin_unpredictable(condition)
#define not_tail_called gnu_attr(not_tail_called)
#define disable_tail_calls gnu_attr(disable_tail_calls)
//#define no_caller_saved_registers [[gnu::no_caller_saved_registers]] 
#define closed_enum gnu_attr(enum_extensibility(closed))
#define gnu_used gnu_attr(used)
#define gnu_retain gnu_attr(retain)

#define use_var(var) \
gnu_used static volatile auto used_name = var

#define use_variable(var) use_var(var)

#define unreachable __builtin_unreachable()

#ifndef DONT_COMPILE_REGISTER_ASSUME_JANK
// These functions are useful to make the compiler
// not optimize out calls to stub functions.
gnu_noinline void assume_non_vector_registers_volatile();
gnu_noinline void assume_all_registers_volatile();
gnu_noinline void assume_all_registers_volatile(void* use_arg);
#else
static gnu_noinline void assume_non_vector_registers_volatile() {}
static gnu_noinline void assume_all_registers_volatile() {}
static gnu_noinline void assume_all_registers_volatile(void* use_arg) {}
#endif

template<typename T, uint8_t known_base = 0, bool enable_relative = false, bool skip_whitespace = true, bool check_sign = true>
static gnu_noinline std::pair<T, const char*> regcall strtonum(const char* str, size_t base) {
    const uint8_t* str_read = (const uint8_t*)str;
    uint8_t number_base;
    if constexpr (known_base == 0 || known_base == 10) {
        number_base = 10;
    } else if constexpr (known_base == 2 || known_base == 16) {
        number_base = known_base;
    }
    uint8_t flags = 0;
    const uint8_t is_negate = 0x01;
    const uint8_t is_relative = 0x02;
    const uint8_t is_max = 0x80;
    uint32_t c;
    if constexpr (skip_whitespace || check_sign) {
    pre_number_loop:
        switch (c = *str_read) {
            case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
                if constexpr (!skip_whitespace) {
                    ++str_read;
                    goto pre_number_loop;
                } else {
                    goto fail;
                }
            case '-':
                if constexpr (check_sign) {
                    flags |= is_negate;
                    break;
                } else {
                    goto fail;
                }
            case '+':
                if constexpr (check_sign) {
                    break;
                } else {
                    goto fail;
                }
            default: [[unlikely]];
                goto fail;
            case 'r': case 'R':
                if constexpr (enable_relative) {
                    goto is_leading_r;
                } else {
                    goto fail;
                }
            case '0':
                goto is_leading_zero;
            case '1' ... '9':
                goto start_number;
        }
        ++str_read;
    }
    switch (c = *str_read) {
        default:
            goto fail;
        case 'r': case 'R': is_leading_r:
            if constexpr (!enable_relative) {
                goto fail;
            } else {
                flags |= is_relative;
            }
        case '0': is_leading_zero:
            if constexpr (known_base == 16) {
                switch (*++str_read) {
                    case 'x': case 'X':
                        c = *++str_read;
                        break;
                    default: [[unlikely]];
                        goto fail;
                }
                break;
            } else if constexpr (known_base == 2) {
                switch (*++str_read) {
                    case 'b': case 'B':
                        c = *++str_read;
                        break;
                    default: [[unlikely]];
                        goto fail;
                }
                break;
            } else if constexpr (known_base == 10) {
                switch (c = *++str_read) {
                    case '0' ... '9':
                        break;
                    default: [[unlikely]];
                        goto fail;
                }
                break;
            } else {
                switch (c = *++str_read) {
                    default: [[unlikely]];
                        goto fail;
                    case 'x': case 'X':
                        number_base = 16;
                        c = *++str_read;
                        break;
                    case 'b': case 'B':
                        number_base = 2;
                        c = *++str_read;
                        break;
                    case '0' ... '9':
                        break;
                }
                break;
            }
        case '1' ... '9':
            break;
    }
    {
start_number:
    T ret = 0;
    for (;; c = *++str_read) {
        uint32_t digit = c;
        switch (digit) {
            case '0' ... '9':
                digit -= '0';
                break;
            case 'a' ... 'f':
                digit -= 'a' - 10;
                break;
            case 'A' ... 'F':
                digit -= 'A' - 10;
                break;
            default:
                goto end_parse;
        }
        if (digit >= number_base) {
            break;
        }
        if (expect(!(flags & is_max), true)) {
            if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
                if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
                    continue;
                }
            }
            flags |= is_max;
        }
    }
end_parse:
    if (!(flags & is_max)) {
        if constexpr (check_sign) {
            if (flags & is_negate) {
                ret = -ret;
            }
        }
        if constexpr (enable_relative) {
            if (flags & is_relative) {
                ret += base;
            }
        }
        return std::make_pair(ret, (const char*)str_read);
    }
    }
fail:
    return std::make_pair(std::numeric_limits<T>::max(), str);
}

template <typename T>
inline T confine_to_range(T min, T input, T max) {
    T temp = __max(min, input);
    return __min(max, temp);
}

// Efficiently tests if [value] is within the range [min, max)
template <typename T> requires(std::is_integral_v<T>)
static inline constexpr bool in_range_exclusive(T value, T min, T max) {
    return (std::make_unsigned_t<T>)(value - min) < (std::make_unsigned_t<T>)(max - min);
}
// Efficiently tests if [value] is within the range [min, max]
// Valid for both signed and unsigned integers
template <typename T> requires(std::is_integral_v<T>)
static inline constexpr bool in_range_inclusive(T value, T min, T max) {
    return (std::make_unsigned_t<T>)(value - min) <= (std::make_unsigned_t<T>)(max - min);
}

template <typename T1, typename T2>
static inline constexpr T1 lerp(const T1& initial_val, const T1& final_val, const T2& time) {
    if constexpr (std::is_integral_v<T1>) {
        return (float)initial_val + time * (final_val - initial_val);
    } else {
        return initial_val + time * (final_val - initial_val);
    }
}

// Packs the bytes [c1] and [c2] together as a 16 bit little endian signed integer
static inline constexpr int16_t PackSInt16(uint8_t c1, uint8_t c2 = 0) {
    return c2 << 8 | c1;
}
// Packs the bytes [c1], [c2], [c3], and [c4] together as a 32 bit little endian signed integer
static inline constexpr int32_t PackSInt32(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
    return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}
// Packs the bytes [c1], [c2], [c3], [c4], [c5], [c6], [c7], and [c8] together as a 64 bit little endian signed integer
static inline constexpr int64_t PackSInt64(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0, uint8_t c5 = 0, uint8_t c6 = 0, uint8_t c7 = 0, uint8_t c8 = 0) {
    return (uint64_t)c8 << 56 | (uint64_t)c7 << 48 | (uint64_t)c6 << 40 | (uint64_t)c5 << 32 | c4 << 24 | c3 << 16 | c2 << 8 | c1;
}
// Packs the bytes [c1] and [c2] together as a 16 bit little endian unsigned integer
static inline constexpr uint16_t PackUInt16(uint8_t c1, uint8_t c2 = 0) {
    return c2 << 8 | c1;
}
// Packs the bytes [c1], [c2], [c3], and [c4] together as a 32 bit little endian unsigned integer
static inline constexpr uint32_t PackUInt32(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
    return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}
// Packs the bytes [c1], [c2], [c3], [c4], [c5], [c6], [c7], and [c8] together as a 64 bit little endian unsigned integer
static inline constexpr uint64_t PackUInt64(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0, uint8_t c5 = 0, uint8_t c6 = 0, uint8_t c7 = 0, uint8_t c8 = 0) {
    return (uint64_t)c8 << 56 | (uint64_t)c7 << 48 | (uint64_t)c6 << 40 | (uint64_t)c5 << 32 | c4 << 24 | c3 << 16 | c2 << 8 | c1;
}
// Packs the bytes [c1] and [c2] together as a 16 bit little endian signed integer
static inline constexpr int16_t PackInt16(uint8_t c1, uint8_t c2 = 0) {
    return PackSInt16(c1, c2);
}
// Packs the bytes [c1], [c2], [c3], and [c4] together as a 32 bit little endian signed integer
static inline constexpr int32_t PackInt32(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
    return PackSInt32(c1, c2, c3, c4);
}
// Packs the bytes [c1], [c2], [c3], [c4], [c5], [c6], [c7], and [c8] together as a 64 bit little endian signed integer
static inline constexpr int64_t PackInt64(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0, uint8_t c5 = 0, uint8_t c6 = 0, uint8_t c7 = 0, uint8_t c8 = 0) {
    return PackSInt64(c1, c2, c3, c4, c5, c6, c7, c8);
}
// Packs the bytes [c1], [c2], [c3], and [c4] together as a 32 bit little endian signed integer
static inline constexpr int32_t PackInt(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
    return PackSInt32(c1, c2, c3, c4);
}
// Packs the bytes [c1], [c2], [c3], and [c4] together as a 32 bit little endian unsigned integer
static inline constexpr uint32_t PackUInt(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
    return PackUInt32(c1, c2, c3, c4);
}

// Packs the nibbles [n1], [n2], [n3], [n4], [n5], [n6], [n7], and [n8] together as a 32 bit little endian unsigned integer
static inline constexpr uint32_t PackUIntNibbles32(uint8_t n1, uint8_t n2 = 0, uint8_t n3 = 0, uint8_t n4 = 0, uint8_t n5 = 0, uint8_t n6 = 0, uint8_t n7 = 0, uint8_t n8 = 0) {
    return (uint32_t)n8 << 28 | (uint32_t)n7 << 24 | (uint32_t)n6 << 20 | (uint32_t)n5 << 16 | (uint32_t)n4 << 12 | (uint32_t)n3 << 8 | n2 << 4 | n1;
}
// Packs the nibbles [n1], [n2], [n3], [n4], [n5], [n6], [n7], and [n8] together as a 32 bit little endian unsigned integer
static inline constexpr uint32_t PackUIntOctets32(uint8_t n1, uint8_t n2 = 0, uint8_t n3 = 0, uint8_t n4 = 0, uint8_t n5 = 0, uint8_t n6 = 0, uint8_t n7 = 0, uint8_t n8 = 0) {
    return (uint32_t)n8 << 21 | (uint32_t)n7 << 18 | (uint32_t)n6 << 15 | (uint32_t)n5 << 12 | (uint32_t)n4 << 9 | (uint32_t)n3 << 6 | n2 << 3 | n1;
}

#ifndef far
#define far
#endif
#ifndef near
#define near
#endif

enum PointerType {
    Near32Z			= 0b0001,
    Near64Z			= 0b0010,
    Near16Z			= 0b0011,
    SegmentPointer	= 0b0100,
    Far32Z			= 0b0101,
    Far64Z			= 0b0110,
    Far16Z			= 0b0111,
    SignedPointer	= 0b1000,
    Near32S			= 0b1001,
    Near64S			= 0b1010,
    Near16S			= 0b1011,
    Far32S			= 0b1101,
    Far64S			= 0b1110,
    Far16S			= 0b1111,
    Near32 = Near32Z,
    Near64 = Near64Z,
    Near16 = Near16Z,
    Pointer32 = Near32,
    Pointer64 = Near64,
    Pointer16 = Near16
};

static inline constexpr size_t native_bits = NATIVE_BITS;
static inline constexpr PointerType native_pointer = native_bits == 32 ? Near32 : Near64;

template<typename T = void>
using PTR32 = T * __ptr32;
template<typename T = void>
using PTR32Z = T * __ptr32 __uptr;
template<typename T = void>
using PTR32S = T * __ptr32 __sptr;
template<typename T = void>
using PTR64 = T * __ptr64;
template<typename T = void>
using PTR64Z = T * __ptr64 __uptr;
template<typename T = void>
using PTR64S = T * __ptr64 __sptr;

template<typename T = void>
using SPTR32 = T near * __ptr32;
template<typename T = void>
using SPTR32Z = T near * __ptr32 __uptr;
template<typename T = void>
using SPTR32S = T near * __ptr32 __sptr;
template<typename T = void>
using SPTR64 = T near * __ptr64;
template<typename T = void>
using SPTR64Z = T near * __ptr64 __uptr;
template<typename T = void>
using SPTR64S = T near * __ptr64 __sptr;

template<typename T = void>
using LPTR32 = T far * __ptr32;
template<typename T = void>
using LPTR32Z = T far * __ptr32 __uptr;
template<typename T = void>
using LPTR32S = T far * __ptr32 __sptr;
template<typename T = void>
using LPTR64 = T far * __ptr64;
template<typename T = void>
using LPTR64Z = T far * __ptr64 __uptr;
template<typename T = void>
using LPTR64S = T far * __ptr64 __sptr;

template<typename T = void>
using FSPTR = T FS_RELATIVE*;
template<typename T = void>
using GSPTR = T GS_RELATIVE*;
template<typename T = void>
using SSPTR = T SS_RELATIVE*;
#if CUSTOM_CLANG
template<typename T = void>
using CSPTR = T CS_RELATIVE*;
template<typename T = void>
using DSPTR = T DS_RELATIVE*;
template<typename T = void>
using ESPTR = T ES_RELATIVE*;
#endif

template <typename T> struct remove_pointer { using type = T; };
template <typename T> struct remove_pointer<T*> { using type = T; };
template <typename T> struct remove_pointer<T *const> { using type = T; };
template <typename T> struct remove_pointer<T *volatile> { using type = T; };
template <typename T> struct remove_pointer<T *const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *restrict> { using type = T; };
template <typename T> struct remove_pointer<T *restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *restrict const volatile> { using type = T; };
/*
template <typename T> struct remove_pointer<T *__uptr const> { using type = T; };
template <typename T> struct remove_pointer<T *__uptr volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__uptr const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__uptr restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__uptr restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__uptr restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__uptr restrict const volatile> { using type = T; };
*/
//template <typename T> struct remove_pointer<T *__sptr> { using type = T; };
/*
template <typename T> struct remove_pointer<T *__sptr const> { using type = T; };
template <typename T> struct remove_pointer<T *__sptr volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__sptr const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__sptr restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__sptr restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__sptr restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__sptr restrict const volatile> { using type = T; };
*/
#if NATIVE_BITS == 64
template <typename T> struct remove_pointer<T *__ptr32> { using type = T; };
#endif
/*
template <typename T> struct remove_pointer<T *__ptr32 const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 restrict const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __uptr restrict const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr32 __sptr restrict const volatile> { using type = T; };
*/
#if NATIVE_BITS == 32
template <typename T> struct remove_pointer<T *__uptr> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64> { using type = T; };
#endif
/*
template <typename T> struct remove_pointer<T *__ptr64 const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 restrict const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __uptr restrict const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr const volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr restrict> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr restrict const> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr restrict volatile> { using type = T; };
template <typename T> struct remove_pointer<T *__ptr64 __sptr restrict const volatile> { using type = T; };
*/

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template<typename T>
static inline constexpr bool ptr_is_64 = std::is_same_v<T, PTR64<remove_pointer_t<T>>>;

template<size_t pointer_size, typename T = void>
using PTRNZ = std::conditional_t<pointer_size == 32 || pointer_size == 16, PTR32Z<T>, // TODO: 16
              std::conditional_t<pointer_size == 64, PTR64Z<T>,
              void>>;
template<size_t pointer_size, typename T = void>
using PTRNS = std::conditional_t<pointer_size == 32 || pointer_size == 16, PTR32S<T>, // TODO: 16
              std::conditional_t<pointer_size == 64, PTR64S<T>,
              void>>;

template<size_t pointer_size, typename T = void>
using SPTRNZ = std::conditional_t<pointer_size == 32 || pointer_size == 16, SPTR32Z<T>, // TODO: 16
               std::conditional_t<pointer_size == 64, SPTR64Z<T>,
               void>>;
template<size_t pointer_size, typename T = void>
using SPTRNS = std::conditional_t<pointer_size == 32 || pointer_size == 16, SPTR32S<T>, // TODO: 16
               std::conditional_t<pointer_size == 64, SPTR64S<T>,
               void>>;

template<size_t pointer_size, typename T = void>
using LPTRNZ = std::conditional_t<pointer_size == 32 || pointer_size == 16, LPTR32Z<T>,
               std::conditional_t<pointer_size == 64, LPTR64Z<T>,
               void>>;
template<size_t pointer_size, typename T = void>
using LPTRNS = std::conditional_t<pointer_size == 32 || pointer_size == 16, LPTR32S<T>,
               std::conditional_t<pointer_size == 64, LPTR64S<T>,
               void>>;

template<size_t pointer_size>
using uintptr_tx = std::conditional_t<pointer_size == 32 || pointer_size == 16, uint32_t,
                   std::conditional_t<pointer_size == 64, uint64_t,
                   void>>;

template<size_t pointer_size>
using intptr_tx = std::conditional_t<pointer_size == 32 || pointer_size == 16, int32_t,
                  std::conditional_t<pointer_size == 64, int64_t,
                  void>>;

template<size_t pointer_size>
using size_tx = std::conditional_t<pointer_size == 32 || pointer_size == 16, uint32_t,
                std::conditional_t<pointer_size == 64, uint64_t,
                void>>;

template<size_t pointer_size>
using ssize_tx = std::conditional_t<pointer_size == 32 || pointer_size == 16, int32_t,
                 std::conditional_t<pointer_size == 64, int64_t,
                 void>>;

template<size_t pointer_size, typename T = void>
using PTRZX = PTRNZ<pointer_size, T>;

template<size_t pointer_size, typename T = void>
using PTRSX = PTRNS<pointer_size, T>;

template<size_t pointer_size, typename T = void>
using SPTRZX = SPTRNZ<pointer_size, T>;

template<size_t pointer_size, typename T = void>
using SPTRSX = SPTRNS<pointer_size, T>;

template<size_t pointer_size, typename T = void>
using LPTRZX = PTRNZ<pointer_size, T>;

template<size_t pointer_size, typename T = void>
using LPTRSX = LPTRNS<pointer_size, T>;

template<typename T>
using PTRZ = PTRNZ<bitsof(void*), T>;
template<typename T>
using PTRS = PTRNS<bitsof(void*), T>;

template<typename T>
using SPTRZ = SPTRNZ<bitsof(void*), T>;
template<typename T>
using SPTRS = SPTRNS<bitsof(void*), T>;

template<typename T>
using LPTRZ = LPTRNZ<bitsof(void*), T>;
template<typename T>
using LPTRS = LPTRNS<bitsof(void*), T>;

template<typename T = void>
static forceinline T* based_pointer(void* base, size_t offset) {
    return (T*)(uintptr_t(base) + offset);
}
template<typename T = void>
static forceinline const T* based_pointer(const void* base, size_t offset) {
    return (const T*)(uintptr_t(base) + offset);
}
template<typename T, typename T2 = T>
static forceinline T2* based_pointer(T* base, size_t offset) {
    return (T*)(uintptr_t(base) + offset);
}
template<typename T = void>
static forceinline T* based_pointer(void* base, void* offset) {
    return (T*)(uintptr_t(base) + uintptr_t(offset));
}
template<typename T = void>
static forceinline const T* based_pointer(const void* base, void* offset) {
    return (const T*)(uintptr_t(base) + uintptr_t(offset));
}

#define PtrDiffStrlen(end_ptr, start_ptr) ((size_t)((end_ptr) - (start_ptr)))

constexpr inline size_t byteloop_strlen(const char *const restrict str) {
    const char *restrict temp = str;
    while (*temp) ++temp;
    return temp - str;
}

constexpr inline char* byteloop_strcpy(char *const restrict dest, const char *restrict src) {
    char *restrict temp = dest;
    while ((*temp++ = *src++));
    return dest;
}

constexpr inline const char *restrict byteloop_strchr(const char *const restrict str, char ch) {
    const char *restrict temp = str;
    unsigned char c = 0;
    for (
        ;
        (c = *temp) && c != ch;
        ++temp
    );
    return c ? temp : NULL;
}

constexpr inline size_t byteloop_wcslen(const wchar_t *const restrict str) {
    const wchar_t *restrict temp = str;
    //while (*temp) ++temp;
    //return temp - str;
    size_t length = 0;
    while (*temp++) ++length;
    return length;
}

constexpr inline size_t byteloop_wcslen_raw(const wchar_t *const restrict str) {
    const wchar_t *restrict temp = str;
    //while (*temp) ++temp;
    //return temp - str;
    size_t length = 0;
    while (*temp) {
        ++temp;
        length += 2;
    }
    return length;
}

template<typename T = size_t>
constexpr inline T mask_width(uint8_t bit_count) {
    T ret = 0;
    while (bit_count--) ret = ret << 1 | 1;
    return ret;
}
constexpr inline size_t sint_width_max(size_t bit_count) {
    size_t ret = 0;
    while (--bit_count) ret = ret << 1 | 1;
    return ret;
}
constexpr inline size_t uint_width_max(size_t bit_count) {
    size_t ret = 0;
    while (bit_count--) ret = ret << 1 | 1;
    return ret;
}

template<typename T1, typename T2>
struct pack_reverse_impl;

template<typename ... TArgs>
struct pack_reverse_impl<std::tuple<>, std::tuple<TArgs...>> {
    using types = std::tuple<TArgs...>;
};

template<typename A, typename ... Args, typename ... TArgs>
struct pack_reverse_impl<std::tuple<A, Args...>, std::tuple<TArgs...>> : pack_reverse_impl<std::tuple<Args...>, std::tuple<A, TArgs...>> {
};

template<typename A, typename ... Args>
struct pack_reverse_impl<std::tuple<A, Args...>, std::tuple<>> : pack_reverse_impl<std::tuple<Args...>, std::tuple<A>> {
};

template<typename ... Args>
struct pack_reverse : pack_reverse_impl<std::tuple<Args...>, std::tuple<>> {};

template<typename ... Args>
using pack_reverse_t = typename pack_reverse<Args...>::types;

//using vec = T __attribute__((__vector_size__(count * sizeof(T)),__aligned__(alignment ? alignment : (count * sizeof(T)))));

/*
template<typename T, size_t count, bool is_aligned = false>
using vec = std::conditional_t<is_aligned, \
    T __attribute__((__vector_size__(count * sizeof(T)))), \
    unaligned T __attribute__((__vector_size__(count * sizeof(T)))) \
> __attribute__((__aligned__(is_aligned ? count * sizeof(T) : 1)));
*/

// Minimum and maximum macros
#define __max2(a,b) (((a) >= (b)) ? (a) : (b))
#define __min2(a,b) (((a) <= (b)) ? (a) : (b))

#if __INTELLISENSE__

// Intellisense doesn't understand a lot of the builtins...

#if !__has_builtin(__builtin_dump_struct)
#define __builtin_dump_struct(struct_ptr, print_func, ...) __noop(struct_ptr __VA_OPT__(,) __VA_ARGS__)
#endif

#if !__has_builtin(__builtin_bit_cast)
#define __builtin_bit_cast(type, value) ((type)(value))
#endif

#if !__has_builtin(__builtin_ia32_tzcnt_u32)
#define __builtin_ia32_tzcnt_u32(value) (value)
#endif

#if !__has_builtin(__builtin_rotateleft64)
#define __builtin_rotateleft64(value,amount) (value)
#endif
#if !__has_builtin(__builtin_rotateleft32)
#define __builtin_rotateleft32(value,amount) (value)
#endif
#if !__has_builtin(__builtin_rotateleft16)
#define __builtin_rotateleft16(value,amount) (value)
#endif
#if !__has_builtin(__builtin_rotateleft8)
#define __builtin_rotateleft8(value,amount) (value)
#endif
#if !__has_builtin(__builtin_rotateright64)
#define __builtin_rotateright64(value,amount) (value)
#endif
#if !__has_builtin(__builtin_rotateright32)
#define __builtin_rotateright32(value,amount) (value)
#endif
#if !__has_builtin(__builtin_rotateright16)
#define __builtin_rotateright16(value,amount) (value)
#endif
#if !__has_builtin(__builtin_rotateright8)
#define __builtin_rotateright8(value,amount) (value)
#endif

#endif

//#define bit_cast __builtin_bit_cast
//#define bitcast __builtin_bit_cast

template<typename T1, typename T2>
static forceinline constexpr T1 bitcast(const T2& value) {
    return __builtin_bit_cast(T1, value);
}

//#define bitcast(type, value) ((bitcast<type>)(value))

#define asm_arg(constraint, name) \
[name] constraint (name)

#define asm_flags(flag, name) \
MACRO_STR(MACRO_CAT(=@cc, flag)) (name)

#define asm_jump(label) \
__asm__ goto ("JMP %l0"::::label)

#define asm_func_jump(func) \
__asm__ __volatile__ ("JMP %P[" MACRO_STR(func) "]"::asm_arg("i", func))

#define asm_call(func) \
__asm__ __volatile__ ("CALL %P[" MACRO_STR(func) "]"::asm_arg("i", func))

#if __x86_64__
#define asm64(symbol) asm(symbol)
#define asm32(symbol)
#define asm3264(symbol32, symbol64) asm(symbol64)
#else
#define asm64(symbol)
#define asm32(symbol) asm(symbol)
#define asm3264(symbol32, symbol64) asm(symbol32)
#endif

#ifndef NO_ASM_SYMBOLS
#define asm_symbol_raw(symbol) asm(symbol)
#else
#define asm_symbol_raw(symbol)
#endif

#define asm_symbol(symbol) \
asm_symbol_raw("{" MACRO_STR(symbol) "}")

#define asm_symbol_rel(symbol) \
asm_symbol_raw("{[" MACRO_STR(symbol) "]}")

#define asm_symbol_abs(symbol) \
asm_symbol_raw("{<" MACRO_STR(symbol) ">}")

#define asm_symbol_expr(symbol) \
asm_symbol_raw("{(" MACRO_STR(symbol) ")}")

#define align_statement(...) __asm__ volatile (".align " #__VA_ARGS__ :)

#define infinite_loop() __asm__ volatile (".byte 0xEB, 0xFE")

template <size_t bit_count>
using SBitIntType = std::conditional_t<bit_count <= 8, int8_t,
                    std::conditional_t<bit_count <= 16, int16_t,
                    std::conditional_t<bit_count <= 32, int32_t,
                    std::conditional_t<bit_count <= 64, int64_t,
                    void>>>>;
template <size_t bit_count>
using UBitIntType = std::conditional_t<bit_count <= 8, uint8_t,
                    std::conditional_t<bit_count <= 16, uint16_t,
                    std::conditional_t<bit_count <= 32, uint32_t,
                    std::conditional_t<bit_count <= 64, uint64_t,
                    void>>>>;
template <size_t bit_count>
using BitFloatType = std::conditional_t<bit_count <= 32, float,
                     std::conditional_t<bit_count <= 64, double,
                     std::conditional_t<bit_count <= 80, long double,
                     void>>>;

template <size_t byte_count>
using SByteIntType = SBitIntType<byte_count * CHAR_BIT>;
template <size_t byte_count>
using UByteIntType = UBitIntType<byte_count * CHAR_BIT>;
template <size_t byte_count>
using ByteFloatType = BitFloatType<byte_count * CHAR_BIT>;

#if __INTELLISENSE__
#define SBitInt(bit_count) SBitIntType<bit_count>
#define UBitInt(bit_count) UBitIntType<bit_count>
#define _BitInt(bit_count) SBitInt(bit_count)
#else
#define SBitInt(bit_count) signed _BitInt(bit_count)
#define UBitInt(bit_count) unsigned _BitInt(bit_count)
#endif

#define NoUnroll _Pragma("clang loop unroll(disable)")
#define nounroll NoUnroll
#define NoVectorize _Pragma("clang loop vectorize(disable)")
#define novectorize NoVectorize
#define NoDistribute _Pragma("clang loop distribute(disable)")
#define nodistribute NoDistribute

#include "custom_intrin.h"

template <typename T>
static inline constexpr auto vec_as_int(T vector) {
    using V = vec<SBitIntType<bitsof(vector_type_t<T>)>, vector_length_v<T>>;
    return __builtin_bit_cast(V, vector);
}

template <typename T>
static inline constexpr auto vec_as_uint(T vector) {
    using V = vec<UBitIntType<bitsof(vector_type_t<T>)>, vector_length_v<T>>;
    return __builtin_bit_cast(V, vector);
}

template <typename T>
static inline constexpr auto vec_as_float(T vector) {
    using V = vec<BitFloatType<bitsof(vector_type_t<T>)>, vector_length_v<T>>;
    return __builtin_bit_cast(V, vector);
}

template <typename T1, typename T2> requires(is_vector_v<T2>)
static inline constexpr T1 vec_xor(T1 vector, T2 mask) {
    using V1 = vec<SBitIntType<bitsof(vector_type_t<T1>)>, vector_length_v<T1>>;
    using V2 = vec<SBitIntType<bitsof(vector_type_t<T2>)>, vector_length_v<T2>>;
    return (T1)((V1)vector ^ (V2)mask);
}

template <typename T, typename ... Args>
requires(sizeof...(Args) != 0 && is_all_same_size_v<Args...> && (... + sizeof(Args)) <= sizeof(T) && !(sizeof(T) % sizeof(std::tuple_element_t<0, std::tuple<Args...>>)))
static inline constexpr T vec_xor(T vector, Args... mask) {
    using E = std::tuple_element_t<0, std::tuple<Args...>>;
    constexpr size_t L = sizeof(T) / sizeof(E);
    return vec_xor(vector, (vec<E, L>) { __builtin_bit_cast(E, mask)... });
}

template <typename T1, typename T2> requires(is_vector_v<T2>)
static inline constexpr T1 vec_and(T1 vector, T2 mask) {
    using V1 = vec<SBitIntType<bitsof(vector_type_t<T1>)>, vector_length_v<T1>>;
    using V2 = vec<SBitIntType<bitsof(vector_type_t<T2>)>, vector_length_v<T2>>;
    return (T1)((V1)vector & (V2)mask);
}

template <typename T, typename ... Args>
requires(sizeof...(Args) != 0 && is_all_same_size_v<Args...> && (... + sizeof(Args)) <= sizeof(T) && !(sizeof(T) % sizeof(std::tuple_element_t<0, std::tuple<Args...>>)))
static inline constexpr T vec_and(T vector, Args... mask) {
    using E = std::tuple_element_t<0, std::tuple<Args...>>;
    constexpr size_t L = sizeof(T) / sizeof(E);
    return vec_and(vector, (vec<E, L>) { __builtin_bit_cast(E, mask)... });
}

template <typename T1, typename T2> requires(is_vector_v<T2>)
static inline constexpr T1 vec_andnot(T1 vector, T2 mask) {
    using V1 = vec<SBitIntType<bitsof(vector_type_t<T1>)>, vector_length_v<T1>>;
    using V2 = vec<SBitIntType<bitsof(vector_type_t<T2>)>, vector_length_v<T2>>;
    return (T1)(~(V1)vector & (V2)mask);
}

template <typename T, typename ... Args>
requires(sizeof...(Args) != 0 && is_all_same_size_v<Args...> && (... + sizeof(Args)) <= sizeof(T) && !(sizeof(T) % sizeof(std::tuple_element_t<0, std::tuple<Args...>>)))
static inline constexpr T vec_andnot(T vector, Args... mask) {
    using E = std::tuple_element_t<0, std::tuple<Args...>>;
    constexpr size_t L = sizeof(T) / sizeof(E);
    return vec_andnot(vector, (vec<E, L>) { __builtin_bit_cast(E, mask)... });
}

template <typename T1, typename T2> requires(is_vector_v<T2>)
static inline constexpr T1 vec_or(T1 vector, T2 mask) {
    using V1 = vec<SBitIntType<bitsof(vector_type_t<T1>)>, vector_length_v<T1>>;
    using V2 = vec<SBitIntType<bitsof(vector_type_t<T2>)>, vector_length_v<T2>>;
    return (T1)((V1)vector | (V2)mask);
}

template <typename T, typename ... Args>
requires(sizeof...(Args) != 0 && is_all_same_size_v<Args...> && (... + sizeof(Args)) <= sizeof(T) && !(sizeof(T) % sizeof(std::tuple_element_t<0, std::tuple<Args...>>)))
static inline constexpr T vec_or(T vector, Args... mask) {
    using E = std::tuple_element_t<0, std::tuple<Args...>>;
    constexpr size_t L = sizeof(T) / sizeof(E);
    return vec_or(vector, (vec<E, L>) { __builtin_bit_cast(E, mask)... });
}

template <typename T, size_t count = vector_length_v<T>, typename E>
static inline constexpr auto vec_broadcast(E value) {
    using V = vector_type_t<T>;
    vec<V, count> vector = {};
    vector += (V)value; // Technically this is constexpr
    //for (size_t i = 0; i < count; ++i) {
        //vector[i] = (V)value;
    //}
    return vector;
}

template <typename T>
static inline constexpr T vec_reverse(T vector) {
    constexpr size_t vec_length = vector_length_v<T>;
    if constexpr (vec_length == 1) {
        return vector;
    }
    else if constexpr (vec_length == 2) {
        return shufflevec(vector, vector, 1, 0);
    }
    else if constexpr (vec_length == 4) {
        return shufflevec(vector, vector, 3, 2, 1, 0);
    }
    else if constexpr (vec_length == 8) {
        return shufflevec(vector, vector, 7, 6, 5, 4, 3, 2, 1, 0);
    }
    else if constexpr (vec_length == 16) {
        return shufflevec(vector, vector, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    }
    else if constexpr (vec_length == 32) {
        return shufflevec(vector, vector, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    }
    else if constexpr (vec_length == 64) {
        return shufflevec(vector, vector, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    }
    else {
        constexpr size_t half_vec_length = vec_length / 2;
        for (size_t i = 0; i < half_vec_length; ++i) {
            auto temp = vector[i];
            vector[i] = vector[(vec_length - 1) - i];
            vector[(vec_length - 1) - i] = temp;
        }
        return vector;
    }
}

template <typename T>
static inline constexpr T vec_odd_interleave(T lower, T upper) {
    constexpr size_t vec_length = vector_length_v<T>;
    if constexpr (vec_length == 1) {
        return lower;
    }
    else if constexpr (vec_length == 2) {
        return shufflevec(lower, upper, 0, 2);
    }
    else if constexpr (vec_length == 4) {
        return shufflevec(lower, upper, 0, 2, 4, 6);
    }
    else if constexpr (vec_length == 8) {
        return shufflevec(lower, upper, 0, 2, 4, 6, 8, 10, 12, 14);
    }
    else if constexpr (vec_length == 16) {
        return shufflevec(lower, upper, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30);
    }
    else if constexpr (vec_length == 32) {
        return shufflevec(lower, upper, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62);
    }
    else if constexpr (vec_length == 64) {
        return shufflevec(lower, upper, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126);
    }
    else {
        constexpr size_t half_vec_length = vec_length / 2;
        T ret = {};
        for (size_t i = 0; i < half_vec_length; ++i) {
            ret[i] = lower[i * 2];
            ret[i + 1] = upper[i * 2];
        }
        return ret;
    }
}

template <typename T>
static inline constexpr T vec_even_interleave(T lower, T upper) {
    constexpr size_t vec_length = vector_length_v<T>;
    if constexpr (vec_length == 1) {
        return lower;
    }
    else if constexpr (vec_length == 2) {
        return shufflevec(lower, upper, 1, 3);
    }
    else if constexpr (vec_length == 4) {
        return shufflevec(lower, upper, 1, 3, 5, 7);
    }
    else if constexpr (vec_length == 8) {
        return shufflevec(lower, upper, 1, 3, 5, 7, 9, 11, 13, 15);
    }
    else if constexpr (vec_length == 16) {
        return shufflevec(lower, upper, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31);
    }
    else if constexpr (vec_length == 32) {
        return shufflevec(lower, upper, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63);
    }
    else if constexpr (vec_length == 64) {
        return shufflevec(lower, upper, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95, 96, 97, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125, 127);
    }
    else {
        constexpr size_t half_vec_length = vec_length / 2;
        T ret = {};
        for (size_t i = 0; i < half_vec_length; ++i) {
            ret[i] = lower[i * 2];
            ret[i + 1] = upper[i * 2];
        }
        return ret;
    }
}

template <typename T>
static inline constexpr auto vec_movmsk(T vec) {
    constexpr size_t vec_length = vector_length_v<T>;
    using V = vector_type_t<T>;

    if constexpr (sizeof(V) == sizeof(int8_t)) {
        if constexpr (vec_length <= 16) {
            return _mm_movemask_epi8((__m128i)vec);
        }
        else if constexpr (vec_length <= 32) {
            return _mm256_movemask_epi8((__m256i)vec);
        }
        else {
            static_assert(false);
        }
    }
    else if constexpr (sizeof(V) == sizeof(int32_t)) {
        if constexpr (vec_length <= 4) {
            return _mm_movemask_ps((__m128)vec);
        }
        else if constexpr (vec_length <= 8) {
            return _mm256_movemask_ps((__m256)vec);
        }
        else {
            static_assert(false);
        }
    }
    else if constexpr (sizeof(V) == sizeof(int64_t)) {
        if constexpr (vec_length <= 2) {
            return _mm_movemask_pd((__m128d)vec);
        }
        else if constexpr (vec_length <= 4) {
            return _mm256_movemask_pd((__m256d)vec);
        }
        else {
            static_assert(false);
        }
    }
    else {
        static_assert(false);
    }
}

#define FAR_CALL_IMM(seg, addr, ret, ...) __asm__ volatile (CODE_32_DIRECTIVE "lcall %[Seg],%[Addr]":ret: [Seg]"i"(seg), [Addr]"i"(addr) __VA_OPT__(,) __VA_ARGS__)
#define FAR_JUMP_IMM(seg, addr, ret, ...) __asm__ volatile (CODE_32_DIRECTIVE "ljmp %[Seg],%[Addr]":ret: [Seg]"i"(seg), [Addr]"i"(addr) __VA_OPT__(,) __VA_ARGS__)

template<size_t max_float_width, size_t max_double_width, size_t max_byte_width, size_t max_word_width, size_t max_dword_width, size_t max_qword_width, size_t max_oword_width,
         size_t prefer_float_width, size_t prefer_double_width, size_t prefer_byte_width, size_t prefer_word_width, size_t prefer_dword_width, size_t prefer_qword_width, size_t prefer_oword_width>
struct VEC_TRAITS_BASE {
    static inline constexpr size_t MAX_FLOAT_VEC_WIDTH = max_float_width;
    static inline constexpr size_t MAX_DOUBLE_VEC_WIDTH = max_double_width;
    static inline constexpr size_t MAX_OWORD_VEC_WIDTH = max_oword_width;
    static inline constexpr size_t MAX_QWORD_VEC_WIDTH = max_qword_width;
    static inline constexpr size_t MAX_DWORD_VEC_WIDTH = max_dword_width;
    static inline constexpr size_t MAX_WORD_VEC_WIDTH = max_word_width;
    static inline constexpr size_t MAX_BYTE_VEC_WIDTH = max_byte_width;
    static inline constexpr size_t PREFER_FLOAT_VEC_WIDTH = prefer_float_width;
    static inline constexpr size_t PREFER_DOUBLE_VEC_WIDTH = prefer_double_width;
    static inline constexpr size_t PREFER_OWORD_VEC_WIDTH = prefer_oword_width;
    static inline constexpr size_t PREFER_QWORD_VEC_WIDTH = prefer_qword_width;
    static inline constexpr size_t PREFER_DWORD_VEC_WIDTH = prefer_dword_width;
    static inline constexpr size_t PREFER_WORD_VEC_WIDTH = prefer_word_width;
    static inline constexpr size_t PREFER_BYTE_VEC_WIDTH = prefer_byte_width;

    template <typename T> requires(std::is_integral_v<T>)
    static inline constexpr size_t max_integral_width_v = sizeof(T) == sizeof(uint8_t) ? MAX_BYTE_VEC_WIDTH :
                                                          sizeof(T) == sizeof(uint16_t) ? MAX_WORD_VEC_WIDTH :
                                                          sizeof(T) == sizeof(uint32_t) ? MAX_DWORD_VEC_WIDTH :
                                                          sizeof(T) == sizeof(uint64_t) ? MAX_QWORD_VEC_WIDTH :
                                                          sizeof(T) == sizeof(__uint128_t) ? MAX_OWORD_VEC_WIDTH :
                                                          0;
    template <typename T> requires(std::is_integral_v<T>)
    static inline constexpr size_t prefer_integral_width_v = sizeof(T) == sizeof(uint8_t) ? PREFER_BYTE_VEC_WIDTH :
                                                             sizeof(T) == sizeof(uint16_t) ? PREFER_WORD_VEC_WIDTH :
                                                             sizeof(T) == sizeof(uint32_t) ? PREFER_DWORD_VEC_WIDTH :
                                                             sizeof(T) == sizeof(uint64_t) ? PREFER_QWORD_VEC_WIDTH :
                                                             sizeof(T) == sizeof(__uint128_t) ? PREFER_OWORD_VEC_WIDTH :
                                                             0;
    template <typename T> requires(std::is_floating_point_v<T>)
    static inline constexpr size_t max_float_width_v = std::is_same_v<T, float> ? MAX_FLOAT_VEC_WIDTH :
                                                       std::is_same_v<T, double> ? MAX_DOUBLE_VEC_WIDTH :
                                                       0;
    template <typename T> requires(std::is_floating_point_v<T>)
    static inline constexpr size_t prefer_float_width_v = std::is_same_v<T, float> ? PREFER_FLOAT_VEC_WIDTH :
                                                          std::is_same_v<T, double> ? PREFER_DOUBLE_VEC_WIDTH :
                                                          0;
};

template<SseTier_t tier> struct VEC_TRAITS;
template<> struct VEC_TRAITS<AVX512_fast> : VEC_TRAITS_BASE </*MAX*/ 16, 8, 64, 32, 16, 8, 2, /*PREFER*/ 16, 8, 64, 32, 16, 8, 0> {};
template<> struct VEC_TRAITS<AVX512_full> : VEC_TRAITS_BASE </*MAX*/ 16, 8, 64, 32, 16, 8, 2, /*PREFER*/ 8, 4, 32, 16, 8, 4, 0> {};
template<> struct VEC_TRAITS<AVX512_wide> : VEC_TRAITS_BASE </*MAX*/ 16, 8, 64, 32, 16, 8, 2, /*PREFER*/ 8, 4, 32, 16, 8, 4, 0> {};
template<> struct VEC_TRAITS<AVX512_basic> : VEC_TRAITS_BASE </*MAX*/ 8, 4, 32, 16, 8, 4, 1, /*PREFER*/ 8, 4, 32, 16, 8, 4, 0> {};
template<> struct VEC_TRAITS<AVX2> : VEC_TRAITS_BASE </*MAX*/ 8, 4, 32, 16, 8, 4, 1, /*PREFER*/ 8, 4, 32, 16, 8, 4, 0> {};
template<> struct VEC_TRAITS<XOP> : VEC_TRAITS_BASE </*MAX*/ 8, 4, 16, 8, 4, 2, 1, /*PREFER*/ 8, 4, 16, 8, 4, 2, 0> {};
template<> struct VEC_TRAITS<AVX> : VEC_TRAITS_BASE </*MAX*/ 8, 4, 16, 8, 4, 2, 1, /*PREFER*/ 8, 4, 16, 8, 4, 2, 0> {};
template<> struct VEC_TRAITS<SSE42> : VEC_TRAITS_BASE </*MAX*/ 4, 2, 16, 8, 4, 2, 0, /*PREFER*/ 4, 2, 16, 8, 4, 2, 0> {};
template<> struct VEC_TRAITS<SSE41> : VEC_TRAITS_BASE </*MAX*/ 4, 2, 16, 8, 4, 2, 0, /*PREFER*/ 4, 2, 16, 8, 4, 2, 0> {};
template<> struct VEC_TRAITS<SSSE3> : VEC_TRAITS_BASE </*MAX*/ 4, 2, 16, 8, 4, 2, 0, /*PREFER*/ 4, 2, 16, 8, 4, 2, 0> {};
template<> struct VEC_TRAITS<SSE3> : VEC_TRAITS_BASE </*MAX*/ 4, 2, 16, 8, 4, 2, 0, /*PREFER*/ 4, 2, 16, 8, 4, 2, 0> {};
template<> struct VEC_TRAITS<SSE2> : VEC_TRAITS_BASE </*MAX*/ 4, 2, 16, 8, 4, 2, 0, /*PREFER*/ 4, 2, 16, 8, 4, 0, 0> {};
template<> struct VEC_TRAITS<SSE> : VEC_TRAITS_BASE </*MAX*/ 4, 2, 8, 4, 2, 1, 0, /*PREFER*/ 4, 2, 8, 4, 2, 0, 0> {};
template<> struct VEC_TRAITS<_3DNOW> : VEC_TRAITS_BASE </*MAX*/ 2, 0, 8, 4, 2, 1, 0, /*PREFER*/ 0, 0, 0, 0, 0, 0, 0> {};
template<> struct VEC_TRAITS<MMX> : VEC_TRAITS_BASE </*MAX*/ 0, 0, 8, 4, 2, 1, 0, /*PREFER*/ 0, 0, 0, 0, 0, 0, 0> {};
template<> struct VEC_TRAITS<IA32> : VEC_TRAITS_BASE </*MAX*/ 0, 0, 0, 0, 0, 0, 0, /*PREFER*/ 0, 0, 0, 0, 0, 0, 0> {};

using CURRENT_VEC_TRAITS = VEC_TRAITS<SSE_TIER>;

template <typename T> requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
using DoubleSizeUintT = std::conditional_t<sizeof(T) == sizeof(uint8_t), uint16_t,
                        std::conditional_t<sizeof(T) == sizeof(uint16_t), uint32_t,
                        std::conditional_t<sizeof(T) == sizeof(uint32_t), uint64_t,
                        std::conditional_t<sizeof(T) == sizeof(uint64_t), __uint128_t,
                        void>>>>;

#define uppercase_mask 0xDFu
#define lowercase_mask 0x20u

#define lowercase(c) ((c)|lowercase_mask)
#define uppercase(c) ((c)&uppercase_mask)

//#define is_valid_decimal(c) ((c)-'0'<10u)
//#define is_valid_hex_letter(c) (lowercase(c)-'a'<6u)
//#define is_valid_hex(c) 

//static forceinline bool is_valid_decimal(unsigned char c) {
//	return c - '0' < 10u;
//}
//template<bool assume_lowercase = false>
//static forceinline bool is_valid_hex_letter(unsigned char c) {
//	if constexpr (!assume_lowercase) {
//		c = lowercase(c);
//	}
//	return c - 'a' < 6u;
//}
//template<bool assume_lowercase = false>
//static forceinline bool is_valid_hex(unsigned char c) {
//	return is_valid_decimal(c) || is_valid_hex_letter(c);
//}

inline int byteloop_strcmp(const char* restrict strA, const char* restrict strB) {
    auto A = (const volatile uint8_t * restrict)strA;
    auto B = (const volatile uint8_t * restrict)strB;
    char D;
    for (uint8_t C; !(D = (C = *A++) - *B++);) if (!C) return 0;
    return D;
}

inline bool byteloop_strmatch(const char *restrict strA, const char *restrict strB) {
    bool ret;
    for (char c; (ret = (c = *strA++) == *strB++) && c;);
    return ret;
}

inline int byteloop_strcmp_fancy(const uint8_t* restrict strA, const uint8_t* restrict strB) {
    uint8_t ret;
    for (uint8_t c, diff; (ret = __builtin_sub_overflow((c = *strA++), *strB++, &diff)), diff;) if (!c) return 0;
    return (ret - 1) | 1;
}

inline int8_t strcmp_asm(const char* restrict strA, const char* restrict strB) {
    int32_t ret;
    int8_t carry, zero;
    __asm__(
        ".align 16 \n"
        "0: \n"
        "	MOVZB (%[strA]), %[ret] \n"
        "	MOVB (%[strB]), %h[ret] \n"
        "	SUBB %h[ret], %b[ret] \n"
        "   JNE 1f \n"
        //J_CXZ" strcmp_break_label \n"
        "   INC %[strA] \n"
        "   INC %[strB] \n"
        //"   LEA 1(%[strB]), %[strB] \n"
        "   TEST %h[ret], %h[ret] \n"
        "   JE 0b \n"
        //"strcmp_break_label2: \n"
        //"   SBB %[ret], %[ret] \n"
        //"   OR $1, %[ret] \n"
        "1: \n"
        //"   MOVSB %b[ret], %[ret] \n"
        : asm_arg("=&Q", ret), asm_arg("+r", strA), asm_arg("+r", strB)
    );
    return ret;
}

inline bool byteloop_strucmp(const volatile uint8_t* restrict strA, const volatile uint8_t* restrict strB) {
    for (uint8_t c; (c = *strA++) == *strB++;) if (!c) return false;
    return true;
}

template <size_t size>
inline char* byteloop_strcat(char* restrict dst, const char (&src)[size]) {
    char* original_dst = dst;
    while (*dst) ++dst;
    __builtin_memcpy(dst, src, sizeof(src));
    return original_dst;
}

/*
template <typename T>
inline char* byteloop_strcat(char*restrict dst, const T src) {
    char* original_dst = dst;
    while (*dst) ++dst;
    byteloop_strcpy(dst, src);
    return original_dst;
}
*/

template <typename T>
inline char* byteloop_strcat(char* restrict dst, T src) {
    const char* original_src = src;
    while (*src++);
    size_t src_length = PtrDiffStrlen(src, original_src);
    char* dst_end = dst - 1;
    while (*++dst_end);
    __builtin_memcpy(dst_end, src, src_length);
    return dst;
}

#define is_valid_decimal(c) ((uint8_t)((c) - '0') < 10)

#define expect_char(c, letter) (expect(c, letter) == letter)
#define expect_char_chance(c, letter, chance) (expect_chance(c, letter, chance) == letter)

#define jump_only_block if (1); else

template<typename F>
struct function_traits;
template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    using ret_type = R;

};

template<typename... Ts> struct Types {};

#ifndef __x86_64__
struct CdeclT {};
struct StdcallT {};
struct FastcallT {};
struct ThiscallT {};
struct VectorcallT {};
struct RegCallT {};
struct RegParm0T {};
struct RegParm1T {};
struct RegParm2T {};
struct RegParm3T {};
struct PascalCallT {};
template <typename RIn, typename CCIn, typename TypesIn, bool variadic>
struct FuncTypes {
    using R = RIn;
    using CC = CCIn;
    using Types = TypesIn;
    static constexpr bool is_variadic = variadic;
};
template<typename R, typename CC = CdeclT, typename... Args>
constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(cdecl*)(Args...));
template<typename R, typename CC = CdeclT, typename... Args>
constexpr FuncTypes<R, CC, Types<Args...>, true> get_func_types_of(R(cdecl*)(Args..., ...));
template<typename R, typename CC = StdcallT, typename... Args>
constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(stdcall*)(Args...));
template<typename R, typename CC = FastcallT, typename... Args>
constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(fastcall*)(Args...));
template<typename R, typename CC = ThiscallT, typename... Args>
constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(thiscall*)(Args...));
template<typename R, typename CC = VectorcallT, typename... Args>
constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(vectorcall*)(Args...));
//template<typename R, typename CC = RegCallT, typename... Args>
//constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(regcall*)(Args...));
//template<typename R, typename CC = RegParm0T, typename... Args>
//constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(regparm(0)*)(Args...));
//template<typename R, typename CC = RegParm1T, typename... Args>
//constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(regparm(1)*)(Args...));
//template<typename R, typename CC = RegParm2T, typename... Args>
//constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(regparm(2)*)(Args...));
//template<typename R, typename CC = RegParm3T, typename... Args>
//constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(regparm(3)*)(Args...));
//template<typename R, typename CC = PascalCallT, typename... Args>
//constexpr FuncTypes<R, CC, Types<Args...>, false> get_func_types_of(R(pascalcall*)(Args...));
#else
template <typename RIn, typename TypesIn, bool variadic>
struct FuncTypes {
    using R = RIn;
    using Types = TypesIn;
    static constexpr bool is_variadic = variadic;
};
template<typename R, typename... Args>
constexpr FuncTypes<R, Types<Args...>, false> get_func_types_of(R(*)(Args...));
template<typename R, typename... Args>
constexpr FuncTypes<R, Types<Args...>, true> get_func_types_of(R(*)(Args..., ...));
#endif


#define perfect_forward(type, var) std::forward<type>(var)...

template<typename T>
[[nodiscard]] static inline gnu_attr(malloc) T* malloc_array(size_t count) {
    return (T*)malloc(sizeof(T[count]));
}

template<typename T, size_t alignment>
[[nodiscard]] static inline gnu_attr(malloc) T* malloc_array_aligned(size_t count) {
#if _WIN32
    return (T*)_aligned_malloc(sizeof(T[count]), alignment);
#else
    return (T*)aligned_alloc(alignment, sizeof(T[count]));
#endif
}

/*

#include <string>

struct FuncDoesExist {
    void dump_struct() {

    }
};
struct FuncDoesNotExist {

};
struct FuncDoesExistStatic {
    static void dump_struct() {

    }
};

template<typename T, typename = void>
struct has_member_dump_func : std::false_type {};
template<typename T>
struct has_member_dump_func<T, std::enable_if_t<std::is_member_function_pointer_v<decltype(&T::dump_struct)>>> : std::true_type {};
template<typename T>
static inline constexpr bool has_member_dump_func_v = has_member_dump_func<T>::value;
template<typename T, typename = void>
struct has_static_dump_func : std::false_type {};
template<typename T>
struct has_static_dump_func <T, std::enable_if_t<std::is_function_v<decltype(T::dump_struct)>>> : std::true_type {};
template<typename T>
static inline constexpr bool has_static_dump_func_v = has_static_dump_func<T>::value;
template<typename T>
static inline constexpr bool has_dump_func_v = has_member_dump_func_v<T> || has_static_dump_func_v<T>;

template<typename T, typename = void, typename ... ExtraArgs>
struct has_member_dump_print_func : std::false_type {};
template<typename T, typename ... ExtraArgs>
struct has_member_dump_print_func<T,
    std::enable_if_t<
        std::is_member_function_pointer_v<decltype(&T::dump_struct_print)> &&
        std::is_invocable_v<decltype(&T::dump_struct_print), T&, ExtraArgs..., const char*>
    >,
    ExtraArgs...
> : std::true_type {};
template<typename T, typename ... ExtraArgs>
static inline constexpr bool has_member_dump_print_func_v = has_member_dump_print_func<T>::value;
template<typename T, typename = void, typename ... ExtraArgs>
struct has_static_dump_print_func : std::false_type {};
template<typename T, typename ... ExtraArgs>
struct has_static_dump_print_func<T,
    std::enable_if_t<
        std::is_function_v<decltype(T::dump_struct_print)> &&
        std::is_invocable_v<decltype(T::dump_struct_print), ExtraArgs..., const char*>
    >,
    ExtraArgs...
> : std::true_type {};
template<typename T, typename ... ExtraArgs>
static inline constexpr bool has_static_dump_print_func_v = has_static_dump_print_func<T>::value;
template<typename T, typename ... ExtraArgs>
static inline constexpr bool has_dump_print_func_v = has_member_dump_print_func_v<T, ExtraArgs...> || has_static_dump_print_func_v<T, ExtraArgs...>;

static constexpr inline bool test_member_dump_func_true = has_member_dump_func_v<FuncDoesExist>;
static constexpr inline bool test_member_dump_func_false_not_exist = has_member_dump_func_v<FuncDoesNotExist>;
static constexpr inline bool test_member_dump_func_false_static = has_member_dump_func_v<FuncDoesExistStatic>;
static constexpr inline bool test_static_dump_func_true = has_static_dump_func_v<FuncDoesExistStatic>;
static constexpr inline bool test_static_dump_func_false_not_exist = has_static_dump_func_v<FuncDoesNotExist>;
static constexpr inline bool test_static_dump_func_false_not_static = has_static_dump_func_v<FuncDoesExist>;
static constexpr inline bool test_dump_func_true_member = has_dump_func_v<FuncDoesExist>;
static constexpr inline bool test_dump_func_true_static = has_dump_func_v<FuncDoesExistStatic>;
static constexpr inline bool test_dump_func_false_not_exist = has_dump_func_v<FuncDoesNotExist>;

using wkjbrwkjrb = decltype(FuncDoesExist::dump_struct);

static_assert(test_member_dump_func_true);
static_assert(!test_member_dump_func_false_not_exist);
static_assert(!test_member_dump_func_false_static);
static_assert(test_static_dump_func_true);
static_assert(!test_static_dump_func_false_not_exist);
static_assert(!test_static_dump_func_false_not_static);
static_assert(test_dump_func_true_member);
static_assert(test_dump_func_true_static);
static_assert(!test_dump_func_false_not_exist);

template<typename T, typename F, typename ... ExtraArgs> requires std::is_invocable_v<F, ExtraArgs..., const char*>
static void dump_type(const T& dump_data, F* print_func, ExtraArgs&&... extra_args) {
    __builtin_dump_struct(&dump_data, print_func, std::forward<ExtraArgs>(extra_args)...);
}
template<typename T, typename ... ExtraArgs> requires has_static_dump_func_v<T>
static inline void dump_type(const T& dump_data, ExtraArgs&&... extra_args) {
    dump_type(dump_data, T::dump_struct, std::forward<ExtraArgs>(extra_args)...);
}

void dump_type_test() {
    struct pingas_yeet {
        int test;
        double dumb;
        static void dump_struct(const char*) {

        }
    };
    pingas_yeet wtf;
    char buffer[69];
    dump_type(wtf);
}
*/