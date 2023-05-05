#pragma once
#ifndef COMPILER_SUPPORT_H
#define COMPILER_SUPPORT_H 1

#include <stdint.h>

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

#ifdef MSVC_COMPAT
#if _M_I86 && !defined(_M_I386)
#define NATIVE_BITS 16
#elif _M_IX86
#define NATIVE_BITS 32
#else
#define NATIVE_BITS 64
#endif
#elif GCC_COMPAT
#if __x86_64__
#define NATIVE_BITS 64
#elif __i386__
#define NATIVE_BITS 32
#else
#define NATIVE_BITS 16
#endif
#endif

static inline constexpr const uint8_t native_bits = NATIVE_BITS;

#endif