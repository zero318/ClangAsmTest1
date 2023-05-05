#pragma once
#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#include "force_macros.h"

#ifdef __STDC__
#define C89 1
#define C90 1
#endif
#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= 199409L
#define C95 1
#endif
#if __STDC_VERSION__ >= 199901L
#define C99 1
#endif
#if __STDC_VERSION__ >= 201112L
#define C11 1
#endif
#if __STDC_VERSION__ >= 201710L
#define C17 1
#endif
#if __STDC_VERSION__ > 201710L
#define C2X 1
#endif
#endif

#ifdef __cplusplus
#if __cplusplus >= 199711L
#define CPP98
#endif
#if __cplusplus >= 201103L
#define CPP11 1
#endif
#if __cplusplus >= 201402L
#define CPP14 1
#endif
#if __cplusplus >= 201703L
#define CPP17 1
#endif
#if __cplusplus >= 202002L
#define CPP20 1
#endif
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

#define USE_ALL_FEATURES
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

#define __SHA__

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

//#undef __AVX__

#if __INTELLISENSE__
#define __building_module(e) 1
#endif

#include <intrin.h>

#define MACRO_VOID(...)

#if __INTELLISENSE__
#define INTELLISENSE_TYPENAME typename
//#define requires(...) MACRO_EVAL(MACRO_VOID(__VA_ARGS__))
#else
#define INTELLISENSE_TYPENAME
//#define requires(...) requires (__VA_ARGS__)
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
#define MACRO_EVAL(...) __VA_ARGS__

#define MACRO_FIRST(arg1, ...) arg1
//#define MACRO_FIRST_EVAL(...) MACRO_EVAL(MACRO_FIRST(__VA_ARGS__))
#define MACRO_SECOND(arg1, arg2, ...) arg2
#define MACRO_SECOND_EVAL(...) MACRO_EVAL(MACRO_SECOND(__VA_ARGS__))
#define MACRO_THIRD(arg1, arg2, arg3, ...) arg3
#define MACRO_THIRD_EVAL(...) MACRO_EVAL(MACRO_THIRD(__VA_ARGS__))

//#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND(__VA_OPT__(,) __VA_ARGS__, (default_arg))

#define MACRO_EMPTY
#define _EMPTY(...) _EMPTY __VA_ARGS__
#define _MACRO_DEPAREN(...) MACRO ## __VA_ARGS__
#define MACRO_DEPAREN(...) _MACRO_DEPAREN(MACRO_EVAL(_EMPTY __VA_ARGS__))

//#define MACRO_EMPTY
//#define _EMPTY(...) _EMPTY __VA_ARGS__
//#define MACRO_DEPAREN(...) MACRO_CAT(MACRO, MACRO_EVAL(_EMPTY __VA_ARGS__))
//#define MACRO_DEPAREN(...) MACRO_CAT(MACRO, _EMPTY __VA_ARGS__)

//#define MACRO_EMPTY
//#define _EMPTY(...) _EMPTY __VA_ARGS__
//#define _MACRO_DEPAREN(...) MACRO ## __VA_ARGS__
//#define MACRO_DEPAREN(...) _MACRO_DEPAREN(_EMPTY __VA_ARGS__)

//MACRO_DEPAREN((2, 3))

//#define _NOT(...) 1
//#define MACRO_EMPTY_NOT(...) 2

//#define MACRO_DUMMY_ARGS() 0,0
//#define _MACRO_DEFAULT_ARG(...) MACRO_CAT(MACRO_DUMMY_ARGS, MACRO_FIRST_EVAL(MACRO_DEPAREN(__VA_ARGS__)))()
//#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_THIRD_EVAL(_MACRO_DEFAULT_ARG(__VA_ARGS__), default_arg, __VA_ARGS__)


#define MACRO_COMMA() ,

//#define _MACRO_DEFAULT_ARG(...) MACRO_EVAL(MACRO_COMMA MACRO_FIRST_EVAL(MACRO_DEPAREN(__VA_ARGS__)) ())
//#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND_EVAL(_MACRO_DEFAULT_ARG(__VA_ARGS__) default_arg, __VA_ARGS__, 0)

//#define _MACRO_DEFAULT_ARG(...) MACRO_EVAL(MACRO_COMMA __VA_ARGS__ ())
//#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND_EVAL(_MACRO_DEFAULT_ARG(MACRO_FIRST_EVAL(MACRO_DEPAREN(__VA_ARGS__))) default_arg, __VA_ARGS__, 0)
//#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND_EVAL(MACRO_EVAL(MACRO_COMMA MACRO_FIRST_EVAL(MACRO_DEPAREN(__VA_ARGS__)) ()) default_arg, __VA_ARGS__, 0)


//#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND_EVAL(MACRO_EVAL(MACRO_COMMA MACRO_FIRST_EVAL(MACRO_DEPAREN(__VA_ARGS__))())default_arg,__VA_ARGS__,0)

#define MACRO_EVAL(...) __VA_ARGS__

#define MACRO_FIRST(arg1, ...) arg1
#define MACRO_FIRST_EVAL(...) MACRO_EVAL(MACRO_FIRST(__VA_ARGS__))
#define MACRO_SECOND(arg1, arg2, ...) arg2
#define MACRO_SECOND_EVAL(...) MACRO_EVAL(MACRO_SECOND(__VA_ARGS__))

#define MACRO_COMMA() ,

#define MACRO_EMPTY
#define _EMPTY(...) _EMPTY __VA_ARGS__
#define _MACRO_DEPAREN(...) MACRO ## __VA_ARGS__
#define MACRO_DEPAREN(...) _MACRO_DEPAREN(MACRO_EVAL(_EMPTY __VA_ARGS__))

#define _VA_OPT_SUPPORTED(...) __VA_OPT__(,)
#define VA_OPT_SUPPORTED MACRO_SECOND_EVAL(_VA_OPT_SUPPORTED(yeet) true,false)

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

// Black magic precessor voodoo

#define MACRO_DEFAULT_ARG(default_arg, ...) MACRO_SECOND_EVAL(MACRO_EVAL(MACRO_COMMA MACRO_FIRST_EVAL(MACRO_DEPAREN(__VA_ARGS__))()) default_arg, __VA_ARGS__, 0)

#endif

using ssize_t = std::make_signed_t<size_t>;

#if !_HAS_CXX20
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

#define ENUM_MAX_VALUE(enum_name) MACRO_CAT(enum_name,_MAX_VALUE)

#define CACHE_LINE_SIZE 64
#define cache_align alignas(CACHE_LINE_SIZE)

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
#define unused_field_type(type, ...)	type unused_name MACRO_DEFAULT_ARG(,[__VA_ARGS__])
#endif

#define cache_alignment_padding		alignment_padding(CACHE_LINE_SIZE)

// This makes C++ allow an always-failing static assert
// in the else block of an if-constexpr statement.
template<typename> inline constexpr bool dependent_false_v = false;

#define sfinae_enable(...) std::enable_if_t<(__VA_ARGS__), bool> = true

#define SFINAE_FAIL() static_assert(dependent_false_v<T>)

#define LogValidationOffsetFails 1

#if !LogValidationOffsetFails

#ifdef _M_IX86

//#define ValidateFieldOffset(offset, struct_type, member_name) 
#define ValidateFieldOffset(offset, struct_type, member_name) \
static_assert((offset) == __builtin_offsetof(struct_type, member_name), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset)
#define ValidateStructSize(size, struct_type) \
static_assert((size) == sizeof(struct_type), "Incorrect struct size! Size of " MACRO_STR(struct_type) " is not "#size)
#if __INTELLISENSE__
#define ValidateVirtualFieldOffset(offset, struct_type, member_name) \
static_assert(((offset) - sizeof(void*)) == __builtin_offsetof(struct_type, member_name), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset)
#else
#define ValidateVirtualFieldOffset ValidateFieldOffset
#endif
#define ValidateStructAlignment(align, struct_type) \
static_assert((align) == alignof(struct_type), "Incorrect struct alignment! Alignment of " MACRO_STR(struct_type) " is not"#align)
#else
#define ValidateFieldOffset(offset, struct_type, member_name)
#define ValidateStructSize(size, struct_type)
#define ValidateVirtualFieldOffset(offset, struct_type, member_name)
#endif

#else
#if COMPILER_IS_GCC

#define ValidateFieldOffset(offset, struct_type, member_name) \
static_assert((offset) == __builtin_offsetof(struct_type, member_name), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset)
#define ValidateStructSize(size, struct_type) \
static_assert((size) == sizeof(struct_type), "Incorrect struct size! Size of " MACRO_STR(struct_type) " is not "#size)
#define ValidateStructAlignment(align, struct_type) \
static_assert((align) == alignof(struct_type), "Incorrect struct alignment! Alignment of " MACRO_STR(struct_type) " is not"#align)

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

#define ValidateStructSize(size, struct_type) \
struct { \
    template<size_t = 0> \
    static constexpr bool vss() { \
        constexpr size_t test_value = sizeof(struct_type); \
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
    static_assert(vss(), "Incorrect struct size! Size of " MACRO_STR(struct_type) " is not "#size); \
}

#define ValidateStructAlignment(align, struct_type) \
struct { \
    template<size_t = 0> \
    static constexpr bool val() { \
        constexpr size_t test_value = sizeof(struct_type); \
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
    static_assert(val(), "Incorrect struct alignment! Alignment of " MACRO_STR(struct_type) " is not"#align); \
}

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
#define ValidateStructSize(size, struct_type) \
struct MACRO_CAT(vss,__COUNTER__) { \
    static inline constexpr bool vss = validate_impl<(size), sizeof(struct_type)>(); \
    static_assert(vss, "Incorrect struct size! Size of " MACRO_STR(struct_type) " is not "#size); \
}
#define ValidateStructAlignment(align, struct_type) \
struct MACRO_CAT(val,__COUNTER__) { \
    static inline constexpr bool val = validate_impl<(align), alignof(struct_type)>(); \
    static_assert(val, "Incorrect struct alignment! Alignment of " MACRO_STR(struct_type) " is not"#align); \
}

#endif
#endif

#define gnu_attr(...) __attribute__((__VA_ARGS__))

#define shuffle_vec __builtin_shufflevector
#define shufflevec __builtin_shufflevector
#define convert_vec __builtin_convertvector
#define convertvec __builtin_convertvector

// For struct members, the alignment attribute can only
// decrease alignment if the packed attribute is also used
#define gnu_aligned(alignment) gnu_attr(aligned(alignment))
#define unaligned __unaligned

#define bool_str(...) ((bool)(__VA_ARGS__) ? "true" : "false")

/*
template<typename T, size_t count, bool is_aligned = false>
using vec = std::conditional_t<is_aligned, \
	T gnu_attr(__vector_size__(count * sizeof(T))), \
	unaligned T gnu_attr(__vector_size__(count * sizeof(T))) \
> gnu_attr(__aligned__(is_aligned ? count * sizeof(T) : 1));
*/

template<typename T, size_t count>
using vec = T gnu_attr(__vector_size__(count * sizeof(T)), __aligned__(count * sizeof(T)));
//> gnu_attr(__aligned__(is_aligned ? count * sizeof(T) : 1));

#if !CPP20 && !(defined(__cpp_consteval) && __cpp_consteval >= 201811L)
#define consteval constexpr
#pragma message("consteval not supported, using constexpr instead")
#endif
#if !CPP20 && !(defined(__cpp_constinit) && __cpp_constinit >= 201907L)
#define constinit constexpr
#pragma message("constinit not supported, using constexpr instead")
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

#define assume(condition) __builtin_assume(condition)

#define ZUNListPlayNiceWithIntellisense

#ifdef ZUNListIdkDefault
#define ZUNListIdkDefaultValue = ZUNListIdkDefault
#else
#define ZUNListIdkDefaultValue
#endif

#ifndef ZUNListPlayNiceWithIntellisense
#define ZUNListNCast(ptr) (ptr)

template <typename T, bool has_idk>
struct ZUNLinkedListBase;

template <typename T, bool has_idk>
struct ZUNLinkedListData {
	using N = ZUNLinkedListBase<T, false>;
	T* data;
	N* next;
	N* prev;
};
template <typename T>
struct ZUNLinkedListData<T, true> {
	using N = ZUNLinkedListBase<T, true>;
	T* data;
	N* next;
	N* prev;
	N* idk;
};

template <typename T, bool has_idk ZUNListIdkDefaultValue>
struct ZUNLinkedListBase : ZUNLinkedListData<T, has_idk> {
	using N = ZUNLinkedListBase;

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
#define ZUNListNCast(ptr) ((N*)ptr)

template <typename T, bool has_idk ZUNListIdkDefaultValue, bool link_type = has_idk>
struct ZUNLinkedListBase {
	using N = ZUNLinkedListBase<T, link_type, link_type>;
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

#ifndef ZUNListPlayNiceWithIntellisense
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

	inline void delete_each() {
		for (N *node = ZUNListNCast(this), *next_node; node; node = next_node) {
			next_node = node->next;
			assume(node->data != NULL);
			delete node->data;
			delete node;
		}
	}
	inline void head_delete_each() {
		return this->next->delete_each();
	}
	template <typename L>
	inline void for_each(const L& lambda) {
		for (N* node = ZUNListNCast(this); node; node = node->next) {
			lambda(node->data);
		}
	}
	template <typename L>
	inline void head_for_each(const L& lambda) {
		return this->next->for_each(lambda);
	}
	template <typename L>
	inline T* find_if(const L& lambda) {
		for (N* node = ZUNListNCast(this); node; node = node->next) {
			if (T* data = node->data; lambda(data)) return data;
		}
		return NULL;
	}
	template <typename L>
	inline T* head_find_if(const L& lambda) {
		return this->next->find_if(lambda);
	}
	template <typename L>
	inline T* find_if_not(const L& lambda) {
		for (N* node = ZUNListNCast(this); node; node = node->next) {
			if (T* data = node->data; !lambda(data)) return data;
		}
		return NULL;
	}
	template <typename L>
	inline T* head_find_if_not(const L& lambda) {
		return this->next->find_if_not(lambda);
	}
	template <typename L>
	inline N* find_node_if(const L& lambda) {
		N* node = ZUNListNCast(this);
		for (; node; node = node->next) {
			if (T* data = node->data; lambda(data)) break;
		}
		return node;
	}
	template <typename L>
	inline N* head_find_node_if(const L& lambda) {
		return this->next->find_node_if(lambda);
	}
	template <typename L>
	inline N* find_node_before(const L& lambda) {
		N* node = ZUNListNCast(this);
		for (N* next_node; (next_node = node->next); node = next_node) {
			if (T* data = next_node->data; lambda(data)) break;
		}
		return node;
	}
	template <typename L>
	inline N* head_find_node_before(const L& lambda) {
		return this->next->find_node_before(lambda);
	}
	template <typename L>
	inline int32_t count_if(const L& lambda) {
		int32_t ret = 0;
		for (N *node = ZUNListNCast(this), *next_node; node; node = next_node) {
			next_node = node->next;
			if (T* data = node->data; lambda(data)) ++ret;
		}
		return ret;
	}
	template <typename L>
	inline int32_t head_count_if(const L& lambda) {
		return this->next->count_if(lambda);
	}
	template <typename L>
	inline int32_t count_if_not(const L& lambda) {
		int32_t ret = 0;
		for (N *node = ZUNListNCast(this), *next_node; node; node = next_node) {
			next_node = node->next;
			if (T* data = node->data; !lambda(data)) ++ret;
		}
		return ret;
	}
	template <typename L>
	inline int32_t head_count_if_not(const L& lambda) {
		return this->next->count_if_not(lambda);
	}
};

#ifdef ZUNListPlayNiceWithIntellisense
template <typename T>
struct ZUNLinkedListBase<T, true, true> : ZUNLinkedListBase<T, false, true> {
	using N = ZUNLinkedListBase<T, true, true>;
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
struct ZUNLinkedListHeadDummyBase : ZUNLinkedListBase<T, has_idk> {
	using N = ZUNLinkedListBase<T, has_idk>;

	inline void delete_each() {
		return this->head_delete_each();
	}
	template <typename L>
	inline void for_each(const L& lambda) {
		return this->head_for_each(lambda);
	}
	template <typename L>
	inline T* find_if(const L& lambda) {
		return this->head_find_if(lambda);
	}
	template <typename L>
	inline T* find_if_not(const L& lambda) {
		return this->head_find_if_not(lambda);
	}
	template <typename L>
	inline N* find_node_if(const L& lambda) {
		return this->head_find_node_if(lambda);
	}
	template <typename L>
	inline N* find_node_before(const L& lambda) {
		return this->head_find_node_before(lambda);
	}
	template <typename L>
	inline int32_t count_if(const L& lambda) {
		return this->head_count_if(lambda);
	}
	template <typename L>
	inline int32_t count_if_not(const L& lambda) {
		return this->head_count_if_not(lambda);
	}
};

#ifdef ZUNListIdkDefault
template <typename T>
using ZUNLinkedList = ZUNLinkedListBase<T, ZUNListIdkDefault>;
template <typename T>
using ZUNLinkedListHeadDummy = ZUNLinkedListHeadDummyBase<T, ZUNListIdkDefault>;
#endif

static constexpr auto qkjbe = sizeof(ZUNLinkedListBase<int, false>);
static constexpr auto wkjrb = sizeof(ZUNLinkedListBase<int, true>);

/*
template <typename T, bool has_idk = false, bool link_type = has_idk>
struct ZUNLinkedListBase {

	using N = ZUNLinkedListBase<T, link_type, link_type>;

	T* data;
	N* next;
	N* prev;

	inline void initialize_with(T* data) {
		this->data = data;
		this->next = NULL;
		this->prev = NULL;
	}

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
		new_node->next = (decltype(new_node->next))this;
		this->prev = (decltype(this->prev))new_node;
	}

	inline void append(N* new_node) {
		if (N* next_node = this->next) {
			next_node->prepend(new_node);
		}
		new_node->prepend(this);
	}

private:
	template <typename L>
	static inline void for_each(N* node, const L& lambda) {
		for (; node; node = node->next) {
			lambda(node->data);
		}
	}
	template <typename L>
	static inline T* find_if(N* node, const L& lambda) {
		for (; node; node = node->next) {
			if (T* data = node->data; lambda(data)) return data;
		}
		return NULL;
	}
public:
	template <typename L>
	inline void for_each(const L& lambda) & {
		return for_each(this, lambda);
	}
	template <typename L>
	inline T* find_if(const L& lambda) & {
		return find_if(this, lambda);
	}
	template <typename L>
	inline void for_each(const L& lambda) const& {
		return for_each(this->next, lambda);
	}
	template <typename L>
	inline T* find_if(const L& lambda) const& {
		return find_if(this->next, lambda);
	}
};

template <typename T>
struct ZUNLinkedListBase<T, true> : ZUNLinkedListBase<T, false, true> {
	using N = ZUNLinkedListBase<T, true, true>;
	N* idk;

	inline void initialize_with(T* data) {
		this->data = data;
		this->next = NULL;
		this->prev = NULL;
		this->idk = NULL;
		//(*(unaligned vec<uintptr_t, 4>*)this) = { (uintptr_t)data, 0, 0, 0 };
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

namespace ZUNLinkedListIter {
#pragma region // ZUNLinkedListIter::delete_each
	template <typename T, bool has_idk>
	inline void delete_each(ZUNLinkedListBase<T, has_idk>* node) {
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			delete node->data;
			delete node;
		}
	}
	template <typename T, bool has_idk>
	inline void delete_each(const ZUNLinkedListBase<T, has_idk>& head_node) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)head_node.next;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			delete node->data;
			delete node;
		}
	}
#pragma endregion

#pragma region // ZUNLinkedListIter::for_each
	template <typename T, typename L, bool has_idk>
	inline void for_each(ZUNLinkedListBase<T, has_idk>* node, const L& lambda) {
		for (; node; node = node->next) {
			lambda(node->data);
		}
	}
	template <typename T, typename L, bool has_idk>
	inline void for_each(const ZUNLinkedListBase<T, has_idk>& head_node, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			lambda(node->data);
		}
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline void for_each(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		for (; node; node = node->next) {
			lambda(copy, node->data);
		}
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline void for_each(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			lambda(copy, node->data);
		}
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline void for_each_this(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		for (; node; node = node->next) {
			lambda(copy, GARBAGE_ARG(DWORD), node->data);
		}
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline void for_each_this(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			lambda(copy, GARBAGE_ARG(DWORD), node->data);
		}
	}
#pragma endregion

#pragma region // ZUNLinkedListIter::find_if
	template <typename T, typename L, bool has_idk>
	inline T* find_if(ZUNLinkedListBase<T, has_idk>* node, const L& lambda) {
		for (; node; node = node->next) {
			if (T* data = node->data; lambda(data)) return data;
		}
		return NULL;
	}
	template <typename T, typename L, bool has_idk>
	inline T* find_if(const ZUNLinkedListBase<T, has_idk>& head_node, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			if (T* data = node->data; lambda(data)) return data;
		}
		return NULL;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline T* find_if(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		for (; node; node = node->next) {
			if (T* data = node->data; lambda(copy, data)) return data;
		}
		return NULL;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline T* find_if(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			if (T* data = node->data; lambda(copy, data)) return data;
		}
		return NULL;
	}
#pragma endregion

#pragma region // ZUNLinkedListIter::find_if_not
	template <typename T, typename L, bool has_idk>
	inline T* find_if_not(ZUNLinkedListBase<T, has_idk>* node, const L& lambda) {
		for (; node; node = node->next) {
			if (T* data = node->data; !lambda(data)) return data;
		}
		return NULL;
	}
	template <typename T, typename L, bool has_idk>
	inline T* find_if_not(const ZUNLinkedListBase<T, has_idk>& head_node, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			if (T* data = node->data; !lambda(data)) return data;
		}
		return NULL;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline T* find_if_not(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		for (; node; node = node->next) {
			if (T* data = node->data; !lambda(copy, data)) return data;
		}
		return NULL;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline T* find_if_not(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			if (T* data = node->data; !lambda(copy, data)) return data;
		}
		return NULL;
	}
#pragma endregion

#pragma region // ZUNLinkedListIter::find_node_if
	template <typename T, typename L, bool has_idk>
	inline ZUNLinkedListBase<T, has_idk>* find_node_if(ZUNLinkedListBase<T, has_idk>* node, const L& lambda) {
		for (; node; node = node->next) {
			if (T* data = node->data; lambda(data)) break;
		}
		return node;
	}
	template <typename T, typename L, bool has_idk>
	inline ZUNLinkedListBase<T, has_idk>* find_node_if(const ZUNLinkedListBase<T, has_idk>& head_node, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			if (T* data = node->data; lambda(data)) break;
		}
		return node;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline ZUNLinkedListBase<T, has_idk>* find_node_if(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		for (; node; node = node->next) {
			if (T* data = node->data; lambda(copy, data)) break;
		}
		return node;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline ZUNLinkedListBase<T, has_idk>* find_node_if(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		while ((node = node->next)) {
			if (T* data = node->data; lambda(copy, data)) break;
		}
		return node;
	}
#pragma endregion

#pragma region // ZUNLinkedListIter::find_node_before
	template <typename T, typename L, bool has_idk>
	inline ZUNLinkedListBase<T, has_idk>* find_node_before(ZUNLinkedListBase<T, has_idk>* node, const L& lambda) {
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			(next_node = node->next);
			node = next_node
		) {
			if (T* data = next_node->data; lambda(data)) break;
		}
		return node;
	}
	template <typename T, typename L, bool has_idk>
	inline ZUNLinkedListBase<T, has_idk>* find_node_before(const ZUNLinkedListBase<T, has_idk>& head_node, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		for (
			ZUNLinkedListBase<T, has_idk> *next_node;
			(next_node = node->next);
			node = next_node
		) {
			if (T* data = next_node->data; lambda(data)) break;
		}
		return node;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline ZUNLinkedListBase<T, has_idk>* find_node_before(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			(next_node = node->next);
			node = next_node
		) {
			if (T* data = next_node->data; lambda(copy, data)) break;
		}
		return node;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline ZUNLinkedListBase<T, has_idk>* find_node_before(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		auto* node = (ZUNLinkedListBase<T, has_idk>*)&head_node;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			(next_node = node->next);
			node = next_node
		) {
			if (T* data = next_node->data; lambda(copy, data)) break;
		}
		return node;
	}
#pragma endregion

#pragma region // ZUNLinkedListIter::count_if
	template <typename T, typename L, bool has_idk>
	inline int32_t count_if(ZUNLinkedListBase<T, has_idk>* node, const L& lambda) {
		int32_t ret = 0;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; lambda(data)) ++ret;
		}
		return ret;
	}
	template <typename T, typename L, bool has_idk>
	inline int32_t count_if(const ZUNLinkedListBase<T, has_idk>& head_node, const L& lambda) {
		int32_t ret = 0;
		auto* node = (ZUNLinkedListBase<T, has_idk>*)head_node.next;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; lambda(data)) ++ret;
		}
		return ret;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline int32_t count_if(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		int32_t ret = 0;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; lambda(copy, data)) ++ret;
		}
		return ret;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline int32_t count_if(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		int32_t ret = 0;
		auto* node = (ZUNLinkedListBase<T, has_idk>*)head_node.next;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; lambda(copy, data)) ++ret;
		}
		return ret;
	}
#pragma endregion

#pragma region // ZUNLinkedListIter::count_if_not
	template <typename T, typename L, bool has_idk>
	inline int32_t count_if_not(ZUNLinkedListBase<T, has_idk>* node, const L& lambda) {
		int32_t ret = 0;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; !lambda(data)) ++ret;
		}
		return ret;
	}
	template <typename T, typename L, bool has_idk>
	inline int32_t count_if_not(const ZUNLinkedListBase<T, has_idk>& head_node, const L& lambda) {
		int32_t ret = 0;
		auto* node = (ZUNLinkedListBase<T, has_idk>*)head_node.next;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; !lambda(data)) ++ret;
		}
		return ret;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline int32_t count_if_not(ZUNLinkedListBase<T, has_idk>* node, const C& copy, const L& lambda) {
		int32_t ret = 0;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; !lambda(copy, data)) ++ret;
		}
		return ret;
	}
	template <typename T, typename L, bool has_idk, typename C>
	inline int32_t count_if_not(const ZUNLinkedListBase<T, has_idk>& head_node, const C& copy, const L& lambda) {
		int32_t ret = 0;
		auto* node = (ZUNLinkedListBase<T, has_idk>*)head_node.next;
		for (
			ZUNLinkedListBase<T, has_idk>* next_node;
			node;
			node = next_node
		) {
			next_node = node->next;
			if (T* data = node->data; !lambda(copy, data)) ++ret;
		}
		return ret;
	}
#pragma endregion
}
*/

union ZUNMagic {
	char text[4];
	int32_t as_int;
	uint32_t as_uint;
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
    T temp;
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
#define AlignUpToMultipleOf22(val, mul) AlignDownToMultipleOf2((val) + (mul)-1,(mul))
#endif

#define AlignDownToMultipleOf(val, mul) ((val) - (val) % (mul))
#define AlignUpToMultipleOf(val, mul) (AlignDownToMultipleOf((val),(mul)) + (mul))

#define AlignPtrDownToMultipleOf2(val, mul) ((decltype(val))AlignDownToMultipleOf2((uintptr_t)(val),(mul)))
#define AlignPtrUpToMultipleOf2(val, mul) ((decltype(val))AlignUpToMultipleOf2((uintptr_t)(val),(mul)))
#define AlignPtrDownToMultipleOf(val, mul) ((decltype(val))AlignDownToMultipleOf((uintptr_t)(val),(mul)))
#define AlignPtrUpToMultipleOf(val, mul) ((decltype(val))AlignUpToMultipleOf((uintptr_t)(val),(mul)))

#define dword_align(val) (size_t)AlignUpToMultipleOf2((size_t)(val), 4)
#define ptr_dword_align(val) (BYTE*)dword_align((uintptr_t)(val))

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
#define gnu_always_inline gnu_attr(always_inline)
#define restrict __restrict

#define naked __declspec(naked)

//#define fastcall __fastcall
#define fastcall gnu_attr(fastcall)
#ifdef cdecl
#undef cdecl
#endif
#define cdecl __cdecl

//#define thiscall __thiscall
#define thiscall gnu_attr(thiscall)
//#define stdcall __stdcall
#define stdcall gnu_attr(stdcall)
#define vectorcall __vectorcall
#define dllexport __declspec(dllexport)
//#define dllimport __declspec(dllimport)
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

#define sysv_abi gnu_attr(sysv_abi)
#define ms_abi gnu_attr(ms_abi)

//#define noreturn [[noreturn]]
#define no_stack_protector gnu_attr(no_stack_protector)
#define expect __builtin_expect

#define GS_RELATIVE gnu_attr(address_space(256))
#define FS_RELATIVE gnu_attr(address_space(257))
#define SS_RELATIVE gnu_attr(address_space(258))

#if !__has_builtin(__builtin_expect_with_probability)
#define __builtin_expect_with_probability(cond, ...) (cond)
#endif
#define expect_chance __builtin_expect_with_probability

#define unpredictable(condition) __builtin_unpredictable(condition)
#define not_tail_called gnu_attr(not_tail_called)
#define disable_tail_calls gnu_attr(disable_tail_calls)
#define closed_enum gnu_attr(enum_extensibility(closed))
#define gnu_used gnu_attr(used)
#define gnu_retain gnu_attr(retain)

#define use_var(var) \
gnu_used static volatile auto used_name = var

#define use_variable(var) use_var(var)

#define unreachable __builtin_unreachable()

// These functions are useful to make the compiler
// not optimize out calls to stub functions.
gnu_noinline void assume_non_vector_registers_volatile();
gnu_noinline void assume_all_registers_volatile();
gnu_noinline void assume_all_registers_volatile(void* use_arg);

template <typename T>
inline T confine_to_range(T min, T input, T max) {
	T temp = __max(min, input);
	return __min(max, temp);
}

// Packs the bytes [c1], [c2], [c3], and [c4] together as a little endian integer
constexpr int32_t PackInt(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
	return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}

// Packs the bytes [c1], [c2], [c3], and [c4] together as a little endian integer
constexpr int64_t PackInt64(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0, uint8_t c5 = 0, uint8_t c6 = 0, uint8_t c7 = 0, uint8_t c8 = 0) {
	return (uint64_t)c8 << 56 | (uint64_t)c7 << 48 | (uint64_t)c6 << 40 | (uint64_t)c5 << 32 | c4 << 24 | c3 << 16 | c2 << 8 | c1;
}

// Packs the bytes [c1], [c2], [c3], and [c4] together as a little endian integer
constexpr uint32_t PackUInt(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
	return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}

// Packs the bytes [c1], [c2], [c3], and [c4] together as a little endian integer
constexpr uint64_t PackUInt64(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0, uint8_t c5 = 0, uint8_t c6 = 0, uint8_t c7 = 0, uint8_t c8 = 0) {
	return (uint64_t)c8 << 56 | (uint64_t)c7 << 48 | (uint64_t)c6 << 40 | (uint64_t)c5 << 32 | c4 << 24 | c3 << 16 | c2 << 8 | c1;
}

template<typename T = void>
using PTR32Z = T * __ptr32 __uptr;
template<typename T = void>
using PTR32S = T * __ptr32 __sptr;
template<typename T = void>
using PTR64 = T * __ptr64;

template<size_t pointer_size, typename T = void>
using PTRZ = std::conditional_t<pointer_size == 32, PTR32Z<T>,
			 std::conditional_t<pointer_size == 64, PTR64<T>,
			 void>>;
template<size_t pointer_size, typename T = void>
using PTRS = std::conditional_t<pointer_size == 32, PTR32S<T>,
			 std::conditional_t<pointer_size == 64, PTR64<T>,
			 void>>;

template<typename T = void>
static forceinline T* based_pointer(void* base, size_t offset) {
	return (T*)(uintptr_t(base) + offset);
}

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

#define align_statement(value) __asm__ volatile (".align " MACRO_STR(value) :)

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

template <size_t byte_count>
using SByteIntType = SBitIntType<byte_count * CHAR_BIT>;
template <size_t byte_count>
using UByteIntType = UBitIntType<byte_count * CHAR_BIT>;

#if __INTELLISENSE__
#define SBitInt(bit_count) SBitIntType<bit_count>
#define UBitInt(bit_count) UBitIntType<bit_count>
#define _BitInt(bit_count) SBitInt(bit_count)
#else
#define SBitInt(bit_count) signed _BitInt(bit_count)
#define UBitInt(bit_count) unsigned _BitInt(bit_count)
#endif

#include "custom_intrin.h"

inline int byteloop_strcmp(const char* restrict strA, const char* restrict strB) {
	auto A = (const volatile uint8_t * restrict)strA;
	auto B = (const volatile uint8_t * restrict)strB;
	char D;
	for (uint8_t C; !(D = (C = *A++) - *B++);) if (!C) return 0;
	return D;
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
inline char* byteloop_strcat(char*restrict dst, const char (&src)[size]) {
	char* original_dst = dst;
	while (*dst) ++dst;
	__builtin_memcpy(dst, src, sizeof(src));
	return original_dst;
}

template <typename T>
inline char* byteloop_strcat(char*restrict dst, const T src) {
	char* original_dst = dst;
	while (*dst) ++dst;
	byteloop_strcpy(dst, src);
	return original_dst;
}


#define NoUnroll _Pragma("clang loop unroll(disable)")
#define nounroll NoUnroll
#define NoVectorize _Pragma("clang loop vectorize(disable)")
#define novectorize NoVectorize
#define NoDistribute _Pragma("clang loop distribute(disable)")
#define nodistribute NoDistribute

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