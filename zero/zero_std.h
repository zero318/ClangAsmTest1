
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <type_traits>
#include <utility>

#ifndef shufflevec
#define shufflevec __builtin_shufflevector
#endif

#ifndef convertvec
#define convertvec __builtin_convertvector
#endif

#ifndef expect
#define expect __builtin_expect
#endif

#ifndef unreachable
#define unreachable __builtin_unreachable()
#endif

#ifndef GS_RELATIVE
#define GS_RELATIVE __attribute__((address_space(256)))
#endif

#ifndef FS_RELATIVE
#define FS_RELATIVE __attribute__((address_space(257)))
#endif

#ifndef SS_RELATIVE
#define SS_RELATIVE __attribute__((address_space(258)))
#endif

namespace zerostd {

typedef __int128_t int128_t;
typedef __uint128_t uint128_t;

using ssize_t = std::make_signed_t<size_t>;

template <typename T, size_t count, bool is_aligned>
struct $vec_impl {
    using type __attribute__((__vector_size__(count * sizeof(T)), __aligned__(alignof(T)))) = T;
};

template <typename T, size_t count>
struct $vec_impl<T, count, true> {
    using type __attribute__((__vector_size__(count * sizeof(T)))) = T;
};

template <typename T, size_t count, bool is_aligned = false>
using vec = $vec_impl<T, count, is_aligned>::type;

template <typename T, typename = void>
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

template <typename T, typename = void>
struct vector_type : std::type_identity<T> {};

template <typename T>
struct vector_type<T, std::void_t<decltype(std::declval<T>()[0])>> {
    using type = std::remove_reference_t<decltype(std::declval<T>()[0])>;
};

template <typename T>
using vector_type_t = vector_type<T>::type;

template <typename T, typename = void>
struct vector_length : std::integral_constant<size_t, 0> {};

template <typename T>
struct vector_length<T, std::void_t<decltype(std::declval<T>()[0])>> {
    static constexpr size_t value = sizeof(T) / sizeof(vector_type_t<T>);
};

template <typename T>
inline constexpr size_t vector_length_v = vector_length<T>::value;


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
using FSPTR = T FS_RELATIVE*;
template<typename T = void>
using GSPTR = T GS_RELATIVE*;
template<typename T = void>
using SSPTR = T SS_RELATIVE*;

template<typename T = void>
static __forceinline T* based_pointer(void* base, size_t offset) {
    return (T*)((uintptr_t)base + offset);
}
template<typename T = void>
static __forceinline const T* based_pointer(const void* base, size_t offset) {
    return (const T*)((uintptr_t)base + offset);
}
template<typename T, typename T2 = T>
static __forceinline T2* based_pointer(T* base, size_t offset) {
    return (T*)((uintptr_t)base + offset);
}
template<typename T = void>
static __forceinline T* based_pointer(void* base, void* offset) {
    return (T*)((uintptr_t)base + (uintptr_t)offset);
}
template<typename T = void>
static __forceinline const T* based_pointer(const void* base, void* offset) {
    return (const T*)((uintptr_t)base + (uintptr_t)offset);
}

#define PtrDiffStrlen(end_ptr, start_ptr) ((size_t)((end_ptr) - (start_ptr)))

template<typename T1, typename T2>
static __forceinline constexpr T1 bitcast(const T2& value) {
    return __builtin_bit_cast(T1, value);
}

static __forceinline uint64_t rdtsc(void) {
#ifdef _M_IX86
    // This actually lets the compiler be
    // smarter than the builtin sometimes
    uint64_t tick;
    __asm__ volatile (
        "rdtsc"
        : "=A"(tick)
    );
    return tick;
#else
    return __builtin_ia32_rdtsc();
#endif
}

static __forceinline uint64_t rdtsc_benchmark(void) {
    __asm__ volatile("lfence":::"memory");
    uint64_t ret = rdtsc();
    __asm__ volatile("lfence":::"memory");
    return ret;
}

}