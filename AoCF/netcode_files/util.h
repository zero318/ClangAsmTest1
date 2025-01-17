#pragma once

#ifndef UTIL_H
#define UTIL_H 1

#include <stdlib.h>
#include <stdint.h>
#include <type_traits>

#define _MACRO_CAT(arg1, arg2) arg1 ## arg2
#define MACRO_CAT(arg1, arg2) _MACRO_CAT(arg1, arg2)
#define _MACRO_CATW(arg1, arg2, arg3) arg1 ## arg2 ## arg3
#define MACRO_CATW(arg1, arg2, arg3) _MACRO_CATW(arg1, arg2, arg3)

#define MACRO_FIRST(arg1, ...) arg1

#if defined(_MSC_VER) && !defined(MSVC_COMPAT)
#define MSVC_COMPAT 1
#endif
#if defined(__GNUC__) && !defined(GCC_COMPAT)
#define GCC_COMPAT 1
#endif
#if defined(__clang__) && !defined(CLANG_COMPAT)
#define CLANG_COMPAT 1
#endif

#ifdef cdecl
#undef cdecl
#endif
#ifdef stdcall
#undef stdcall
#endif
#ifdef fastcall
#undef fastcall
#endif
#ifdef thiscall
#undef thiscall
#endif

#if GCC_COMPAT
#define cdecl __attribute__((cdecl))
#define stdcall __attribute__((stdcall))
#define fastcall __attribute__((fastcall))
#define thiscall __attribute__((thiscall))
#elif MSVC_COMPAT || CLANG_COMPAT
#define cdecl __cdecl
#define stdcall __stdcall
#define fastcall __fastcall
#define thiscall __thiscall
#else
#define cdecl
#define stdcall 
#define fastcall
#define thiscall
#endif

#if GCC_COMPAT || CLANG_COMPAT
#define thisfastcall thiscall
#define thisfastcall_edx(...)
#else
#define thisfastcall fastcall
#define thisfastcall_edx(...) __VA_ARGS__
#endif

#ifdef forceinline
#undef forceinline
#endif

#ifdef neverinline
#undef neverinline
#endif

#ifdef naked
#undef naked
#endif

#if MSVC_COMPAT || CLANG_COMPAT
#define forceinline __forceinline
#define neverinline __declspec(noinline)
#define naked __declspec(naked)
#else
#define forceinline __attribute__((always_inline)) inline
#define neverinline __attribute__((noinline))
#define naked __attribute__((naked))
#endif

#ifdef EVAL_NOOP
#undef EVAL_NOOP
#endif

#if MSVC_COMPAT
#define EVAL_NOOP(...) __noop(__VA_ARGS__)
#else
#define EVAL_NOOP(...) (0)
#endif

#ifdef dll_export
#undef dll_export
#endif

#define dll_export __declspec(dllexport)

#if GCC_COMPAT || CLANG_COMPAT
#define expect(...) __builtin_expect(__VA_ARGS__)
#else
#define expect(...) MACRO_FIRST(__VA_ARGS__)
#endif

#ifdef unreachable
#undef unreachable
#endif

#if GCC_COMPAT || CLANG_COMPAT
#define unreachable __builtin_unreachable()
#elif MSVC_COMPAT
#define unreachable __assume(0)
#else
#define unreachable do; while(0)
#endif

/*
#if GCC_COMPAT || CLANG_COMPAT
#define stack_return_offset ((uintptr_t*)__builtin_return_address(0))
#elif MSVC_COMPAT
#include <intrin.h>
#define stack_return_offset ((uintptr_t*)_AddressOfReturnAddress())
#else
#error "Unknown stack offset format"
#endif
*/

#define declare_has_func(name) \
template <typename T, typename = void> \
struct MACRO_CAT(has_,name) : std::false_type {}; \
template <typename T> \
struct MACRO_CAT(has_,name)<T, std::void_t<decltype(&T::name)>> : std::true_type {}; \
template <typename T> \
static inline constexpr bool MACRO_CATW(has_,name,_v) = MACRO_CAT(has_,name)<T>::value

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

extern const uintptr_t base_address;

forceinline uintptr_t operator ""_R(unsigned long long int addr) {
    return (uintptr_t)addr + base_address;
}

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
                    
template <typename R, typename B, typename O> requires(!std::is_same_v<R, B>)
static forceinline R* based_pointer(B* base, O offset) {
    return (R*)((uintptr_t)base + (uintptr_t)offset);
}

template <typename P, typename O>
static forceinline P* based_pointer(P* base, O offset) {
    return (P*)((uintptr_t)base + (uintptr_t)offset);
}

template <typename R, typename B, typename O> requires(!std::is_pointer_v<B>)
static forceinline R* based_pointer(B base, O offset) {
    return (R*)((uintptr_t)base + (uintptr_t)offset);
}

template <typename B, typename O> requires(!std::is_pointer_v<B>)
static forceinline B based_pointer(B base, O offset) {
    return (B)((uintptr_t)base + (uintptr_t)offset);
}

template <typename T>
static inline size_t uint16_to_strbuf(uint16_t value, T* text_buffer) {
	size_t digit_offset = 4;
	if (value <= 9) {
		digit_offset = 0;
	}
	else if (value <= 99) {
		digit_offset = 1;
	}
	else if (value <= 999) {
		digit_offset = 2;
	}
	else if (value <= 9999) {
		digit_offset = 3;
	}
	size_t ret = digit_offset + 1;
	do {
		uint16_t digit = value % 10;
		value /= 10;
		text_buffer[digit_offset] = ((T)'0') + digit;
	} while (digit_offset--);
	return ret;
}

#endif