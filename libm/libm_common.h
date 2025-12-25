#pragma once

#ifndef LIBM_COMMON_H
#define LIBM_COMMON_H 1

#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#define _USE_MATH_DEFINES 1
#include <math.h>

#include <bit>

#define _MACRO_CAT(arg1, arg2) arg1 ## arg2
#define MACRO_CAT(arg1, arg2) _MACRO_CAT(arg1, arg2)
#define _MACRO_STR(arg) #arg
#define MACRO_STR(arg) _MACRO_STR(arg)

#define cdecl __cdecl
#define vectorcall __vectorcall
#define regcall __attribute__((regcall))
#define dllexport __declspec(dllexport)
#define forceinline __forceinline
#define gnu_noinline __attribute__((noinline))

#define shufflevec __builtin_shufflevector
#define convertvec __builtin_convertvector

#define expect __builtin_expect
#define expect_chance __builtin_expect_with_probability
#define unpredictable(condition) __builtin_unpredictable(condition)
#define unreachable __builtin_unreachable()

#define CACHE_LINE_SIZE 64
#define cache_align alignas(CACHE_LINE_SIZE)

__if_not_exists(vec) {
template <typename T, size_t count, bool is_aligned>
struct $vec_impl {
    using type __attribute__((__vector_size__(count * sizeof(T)), __aligned__(alignof(T)))) = T;
};

template <typename T, size_t count>
struct $vec_impl<T, count, true> {
    using type __attribute__((__vector_size__(count * sizeof(T)))) = T;
};

template <typename T, size_t count, bool is_aligned = false>
using vec = typename $vec_impl<T, count, is_aligned>::type;
}

static inline constexpr float INDEFINITE_FLOAT = std::bit_cast<float>(0xFFC00000);
static inline constexpr double INDEFINITE_DOUBLE = std::bit_cast<double>(0xFFF8000000000000);
static inline constexpr float PINFINITY_FLOAT = std::bit_cast<float>(0x7F800000);
static inline constexpr float NINFINITY_FLOAT = std::bit_cast<float>(0xFF800000);
static inline constexpr float PNAN_FLOAT = std::bit_cast<float>(0x7FC00000);
static inline constexpr float NNAN_FLOAT = std::bit_cast<float>(0xFFC00000);
static inline constexpr double PINFINITY_DOUBLE = std::bit_cast<double>(0x7FF0000000000000);
static inline constexpr double NINFINITY_DOUBLE = std::bit_cast<double>(0xFFF0000000000000);
static inline constexpr double PNAN_DOUBLE = std::bit_cast<double>(0x7FF8000000000000);
static inline constexpr double NNAN_DOUBLE = std::bit_cast<double>(0xFFF8000000000000);

static inline constexpr float minimum_negative_float_value = -0x1.FFFFFEp127f;
static inline constexpr float maximum_negative_float_value = -0x0.0p0f;
static inline constexpr float minimum_positive_float_value = 0x0.0p0f;
static inline constexpr float maximum_positive_float_value = 0x1.FFFFFEp127f;

#if __x86_64__
#define ASM_STACK_ENTER4
#define ASM_STACK_ENTER8
#define ASM_STACK_ENTER16
#define ASM_STACK_LEAVE4
#define ASM_STACK_LEAVE8
#define ASM_STACK_LEAVE16
#if __AVX__
#define ASM_SSE_WRITE_FLT "VMOVSS %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_FLD2 "VMOVSS %[y], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL "VMOVSD %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL2 "VMOVSD %[y], 8(%%RSP) \n"
#define ASM_SSE_READ_FLT "VMOVSS 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_DBL "VMOVSD 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_2DBL "VMOVAPD 8(%%RSP), %[ret] \n"
#else
#define ASM_SSE_WRITE_FLT "MOVSS %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_FLT2 "MOVSS %[y], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL "MOVSD %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL2 "MOVSD %[y], 8(%%RSP) \n"
#define ASM_SSE_READ_FLT "MOVSS 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_DBL "MOVSD 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_2DBL "MOVAPD 8(%%RSP), %[ret] \n"
#endif
#define ASM_X87_READ_FLT "FLDS 8(%%RSP) \n"
#define ASM_X87_READ_DBL "FLDL 8(%%RSP) \n"
#define ASM_X87_WRITE_FLT "FSTPS 8(%%RSP) \n"
#define ASM_X87_WRITE_DBL "FSTPL 8(%%RSP) \n"
#define ASM_X87_WRTIE_FLT2 "FSTPS 12(%%RSP) \n"
#define ASM_X87_WRITE_DBL2 "FSTPL 16(%%RSP) \n"
#else
#define ASM_STACK_ENTER4 "PUSH %%EAX \n"
#define ASM_STACK_ENTER8 "SUB $8, %%ESP \n"
#define ASM_STACK_ENTER16 "SUB $16, %%ESP \n"
#define ASM_STACK_LEAVE4 "POP %%EAX \n"
#define ASM_STACK_LEAVE8 "ADD $8, %%ESP \n"
#define ASM_STACK_LEAVE16 "ADD $16, %%ESP \n"
#if __AVX__
#define ASM_SSE_WRITE_FLT "VMOVSS %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_FLT2 "VMOVSS %[y], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL "VMOVSD %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL2 "VMOVSD %[y], (%%ESP) \n"
#define ASM_SSE_READ_FLT "VMOVSS (%%ESP), %[ret] \n"
#define ASM_SSE_READ_DBL "VMOVSD (%%ESP), %[ret] \n"
#define ASM_SSE_READ_2DBL "VMOVUPD (%%ESP), %[ret] \n"
#else
#define ASM_SSE_WRITE_FLT "MOVSS %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_FLT2 "MOVSS %[y], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL "MOVSD %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL2 "MOVSD %[y], (%%ESP) \n"
#define ASM_SSE_READ_FLT "MOVSS (%%ESP), %[ret] \n"
#define ASM_SSE_READ_DBL "MOVSD (%%ESP), %[ret] \n"
#define ASM_SSE_READ_2DBL "MOVUPD (%%ESP), %[ret] \n"
#endif
#define ASM_X87_READ_FLT "FLDS (%%ESP) \n"
#define ASM_X87_READ_DBL "FLDL (%%ESP) \n"
#define ASM_X87_WRITE_FLT "FSTPS (%%ESP) \n"
#define ASM_X87_WRITE_DBL "FSTPL (%%ESP) \n"
#define ASM_X87_WRTIE_FLT2 "FSTPS 4(%%ESP) \n"
#define ASM_X87_WRITE_DBL2 "FSTPL 8(%%ESP) \n"
#endif
#define ASM_SSE_READ_2FLT ASM_SSE_READ_DBL

#define ASM_FPREM1_LOOP \
    "1: \n" \
        "FPREM1 \n" \
        "FNSTSW %%AX \n" \
        "TEST $4, %%AH \n" \
        "JNZ 1b \n"

#define ASM_FPREM_LOOP \
    "1: \n" \
        "FPREM \n" \
        "FNSTSW %%AX \n" \
        "TEST $4, %%AH \n" \
        "JNZ 1b \n"

#endif