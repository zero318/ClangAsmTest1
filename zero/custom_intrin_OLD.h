#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma once


#include <tuple>
#include <initializer_list>
#include <algorithm>
#include "util.h"

#if __INTELLISENSE__
// This prevents a bunch of stupid "invalid register" errors
#define clobber_list(...)
// GCC extensions give the register keyword special meaning, so shut up about it
#define register
#else
#define clobber_list(...) __VA_ARGS__
#endif

#define asm_arg(constraint, name) \
[name] constraint (name)

#define asm_flags(flag, name) \
MACRO_STR(MACRO_CAT(=@cc, flag)) (name)

#define read_asm_flags(condition, expr) \
[&](void) { \
	int flag; \
	__asm__ __volatile__( \
		"" \
		: asm_flags(condition, flag) \
		: "X"(expr) \
	); \
	return flag; \
}()

#define read_parity_flag(expr)		read_asm_flags(p, expr)
#define read_overflow_flag(expr)	read_asm_flags(o, expr)
#define read_sign_flag(expr)		read_asm_flags(s, expr)
#define read_zero_flag(expr)		read_asm_flags(z, expr)
#define read_carry_flag(expr)		read_asm_flags(c, expr)


#define PTR_REG(name) "q" MACRO_STR(name)


typedef __int128_t int128_t;
typedef __uint128_t uint128_t;
//register int16_t sp_reg asm("sp");
//register int16_t bp_reg asm("bp");
register int32_t esp_reg asm("esp");
register int32_t ebp_reg asm("ebp");
#ifdef _M_IX86
typedef int32_t sreg_t;
typedef uint32_t usreg_t;
typedef int64_t dreg_t;
typedef uint64_t udreg_t;
#define J_CXZ "JECXZ"
#define rsp_reg esp_reg
#define rbp_reg ebp_reg
#else
typedef int64_t sreg_t;
typedef uint64_t usreg_t;
typedef int128_t dreg_t;
typedef uint128_t udreg_t;
#define J_CXZ "JRCXZ"
register int64_t rsp_reg asm("rsp");
register int64_t rbp_reg asm("rbp");
#endif

template <typename T = uint8_t, sfinae_enable(sizeof(T) == sizeof(uint8_t))>
static forceinline T read_al() { T ret; asm volatile("":"=a"(ret)); return ret; }
template <typename T = uint8_t, sfinae_enable(sizeof(T) == sizeof(uint8_t))>
static forceinline T read_cl() { T ret; asm volatile("":"=c"(ret)); return ret; }
template <typename T = uint8_t, sfinae_enable(sizeof(T) == sizeof(uint8_t))>
static forceinline T read_dl() { T ret; asm volatile("":"=d"(ret)); return ret; }
template <typename T = uint8_t, sfinae_enable(sizeof(T) == sizeof(uint8_t))>
static forceinline T read_bl() { T ret; asm volatile("":"=b"(ret)); return ret; }
template <typename T = uint16_t, sfinae_enable(sizeof(T) == sizeof(uint16_t))>
static forceinline T read_ax() { T ret; asm volatile("":"=a"(ret)); return ret; }
template <typename T = uint16_t, sfinae_enable(sizeof(T) == sizeof(uint16_t))>
static forceinline T read_cx() { T ret; asm volatile("":"=c"(ret)); return ret; }
template <typename T = uint16_t, sfinae_enable(sizeof(T) == sizeof(uint16_t))>
static forceinline T read_dx() { T ret; asm volatile("":"=d"(ret)); return ret; }
template <typename T = uint16_t, sfinae_enable(sizeof(T) == sizeof(uint16_t))>
static forceinline T read_bx() { T ret; asm volatile("":"=b"(ret)); return ret; }
template <typename T = uint16_t, sfinae_enable(sizeof(T) == sizeof(uint16_t))>
static forceinline T read_si() { T ret; asm volatile("":"=S"(ret)); return ret; }
template <typename T = uint16_t, sfinae_enable(sizeof(T) == sizeof(uint16_t))>
static forceinline T read_di() { T ret; asm volatile("":"=D"(ret)); return ret; }
template <typename T = uint32_t, sfinae_enable(sizeof(T) == sizeof(uint32_t))>
static forceinline T read_eax() { T ret; asm volatile("":"=a"(ret)); return ret; }
template <typename T = uint32_t, sfinae_enable(sizeof(T) == sizeof(uint32_t))>
static forceinline T read_ecx() { T ret; asm volatile("":"=c"(ret)); return ret; }
template <typename T = uint32_t, sfinae_enable(sizeof(T) == sizeof(uint32_t))>
static forceinline T read_edx() { T ret; asm volatile("":"=d"(ret)); return ret; }
template <typename T = uint32_t, sfinae_enable(sizeof(T) == sizeof(uint32_t))>
static forceinline T read_ebx() { T ret; asm volatile("":"=b"(ret)); return ret; }
template <typename T = uint32_t, sfinae_enable(sizeof(T) == sizeof(uint32_t))>
static forceinline T read_esi() { T ret; asm volatile("":"=S"(ret)); return ret; }
template <typename T = uint32_t, sfinae_enable(sizeof(T) == sizeof(uint32_t))>
static forceinline T read_edi() { T ret; asm volatile("":"=D"(ret)); return ret; }

#define make_register_writes(reg_name, reg_constraint, signed_type, unsigned_type) \
template <typename T = signed_type, sfinae_enable(std::is_integral_v<T> && std::is_signed_v<T>)> \
static forceinline void MACRO_CAT(write_,reg_name)(const T& value) { signed_type temp; asm volatile("":reg_constraint(temp) : "0"((signed_type)value)); } \
template <typename T = unsigned_type, sfinae_enable(std::is_integral_v<T> && std::is_unsigned_v<T>)> \
static forceinline void MACRO_CAT(write_,reg_name)(const T& value) { unsigned_type temp; asm volatile("":reg_constraint(temp) : "0"((unsigned_type)value)); } \
template <typename T, sfinae_enable(sizeof(T) == sizeof(unsigned_type) && !(std::is_convertible_v<T, signed_type> || std::is_convertible_v<T, unsigned_type>))> \
static forceinline void MACRO_CAT(write_, reg_name)(const T& value) { T temp; asm volatile("":reg_constraint(temp) : "0"((T)value)); } \
template <typename T = signed_type, sfinae_enable(std::is_convertible_v<T, signed_type>)> \
static forceinline void MACRO_CAT(MACRO_CAT(write_,reg_name),_signed)(const T& value) { signed_type temp; asm volatile("":reg_constraint(temp) : "0"((signed_type)value)); } \
template <typename T = unsigned_type, sfinae_enable(std::is_convertible_v<T, unsigned_type>)> \
static forceinline void MACRO_CAT(MACRO_CAT(write_,reg_name),_unsigned)(const T& value) { unsigned_type temp; asm volatile("":reg_constraint(temp) : "0"((unsigned_type)value)); } \
template <typename T, sfinae_enable(sizeof(T) == sizeof(unsigned_type) && !std::is_integral_v<T>)> \
static forceinline void MACRO_CAT(MACRO_CAT(write_,reg_name),_raw)(const T& value) { T temp; asm volatile("":reg_constraint(temp) : "0"((T)value)); }

make_register_writes(al, "=a", int8_t, uint8_t)
make_register_writes(cl, "=c", int8_t, uint8_t)
make_register_writes(dl, "=d", int8_t, uint8_t)
make_register_writes(bl, "=b", int8_t, uint8_t)
make_register_writes(ax, "=a", int16_t, uint16_t)
make_register_writes(cx, "=c", int16_t, uint16_t)
make_register_writes(dx, "=d", int16_t, uint16_t)
make_register_writes(bx, "=b", int16_t, uint16_t)
make_register_writes(si, "=S", int16_t, uint16_t)
make_register_writes(di, "=D", int16_t, uint16_t)
make_register_writes(eax, "=a", int32_t, uint32_t)
make_register_writes(ecx, "=c", int32_t, uint32_t)
make_register_writes(edx, "=d", int32_t, uint32_t)
make_register_writes(ebx, "=b", int32_t, uint32_t)
make_register_writes(esi, "=S", int32_t, uint32_t)
make_register_writes(edi, "=D", int32_t, uint32_t)

template <typename T>
static forceinline T& ref_eax() {
	T ret;
	asm volatile("":"=a"(ret));
	return (T&)std::move(ret);
}

//template <typename T = int32_t, sfinae_enable(std::is_integral_v<T> && std::is_signed_v<T>)>
//static forceinline void write_ecx(T value) { int32_t temp; asm volatile("":"=c"(temp) : "0"((int32_t)value)); }
//template <typename T = uint32_t, sfinae_enable(std::is_integral_v<T> && std::is_unsigned_v<T>)>
//static forceinline void write_ecx(T value) { uint32_t temp; asm volatile("":"=c"(temp) : "0"((uint32_t)value)); }
//template <typename T = int32_t, sfinae_enable(std::is_integral_v<T> && std::is_signed_v<T>)>
//static forceinline void write_edx(T value) { int32_t temp; asm volatile("":"=d"(temp) : "0"((int32_t)value)); }
//template <typename T = uint32_t, sfinae_enable(std::is_integral_v<T> && std::is_unsigned_v<T>)>
//static forceinline void write_edx(T value) { uint32_t temp; asm volatile("":"=d"(temp) : "0"((uint32_t)value)); }
//template <typename T = int32_t, sfinae_enable(std::is_integral_v<T> && std::is_signed_v<T>)>
//static forceinline void write_ebx(T value) { int32_t temp; asm volatile("":"=b"(temp) : "0"((int32_t)value)); }
//template <typename T = uint32_t, sfinae_enable(std::is_integral_v<T> && std::is_unsigned_v<T>)>
//static forceinline void write_ebx(T value) { uint32_t temp; asm volatile("":"=b"(temp) : "0"((uint32_t)value)); }
//template <typename T = int32_t, sfinae_enable(std::is_integral_v<T> && std::is_signed_v<T>)>
//static forceinline void write_esi(T value) { int32_t temp; asm volatile("":"=S"(temp) : "0"((int32_t)value)); }
//template <typename T = uint32_t, sfinae_enable(std::is_integral_v<T> && std::is_unsigned_v<T>)>
//static forceinline void write_esi(T value) { uint32_t temp; asm volatile("":"=S"(temp) : "0"((uint32_t)value)); }
//template <typename T = int32_t, sfinae_enable(std::is_integral_v<T> && std::is_signed_v<T>)>
//static forceinline void write_edi(T value) { int32_t temp; asm volatile("":"=D"(temp) : "0"((int32_t)value)); }
//template <typename T = uint32_t, sfinae_enable(std::is_integral_v<T> && std::is_unsigned_v<T>)>
//static forceinline void write_edi(T value) { uint32_t temp; asm volatile("":"=D"(temp) : "0"((uint32_t)value)); }

static forceinline uint32_t read_esp() { return (uint32_t)esp_reg; }
//static forceinline uint32_t read_ebp() { return (uint32_t)ebp_reg; }
template <typename T = uint32_t, sfinae_enable(sizeof(T) == sizeof(uint32_t))>
static forceinline T read_ebp_old() {
	register T reg asm("ebp") = GARBAGE_VALUE(int32_t);
	T ret;
	asm volatile ("":"=r"(ret) : "r"(reg));
	return ret;
}
static forceinline void write_esp(uint32_t value) { esp_reg = value; }
static forceinline void write_ebp(uint32_t value) { ebp_reg = value; }
static forceinline udreg_t read_ax_dx() { udreg_t ret; asm volatile("":"=A"(ret)); return ret; }
static forceinline void write_ax_dx(udreg_t value) { udreg_t temp; asm volatile("":"=A"(temp) : "0"(value)); }

using ssize_t = std::make_signed_t<size_t>;


template <typename T>
struct bit_count : std::integral_constant<size_t, sizeof(T) * CHAR_BIT> {};
template <typename T>
inline constexpr size_t bit_count_v = sizeof(T) * CHAR_BIT;

template <typename Find, typename Tuple>
struct find_type_in_tuple;
template <typename Find, typename... TupleTypes>
struct find_type_in_tuple<Find, std::tuple<TupleTypes...>> {
private:
	template <typename>
	struct impl;
	template <size_t... idx>
	struct impl <std::integer_sequence<size_t, idx...>> : std::integral_constant<ssize_t, std::max({ static_cast<ssize_t>(std::is_same_v<Find, TupleTypes> ? idx : -1)... })>{};
public:
	static constexpr ssize_t value = impl<std::index_sequence_for<TupleTypes...>>::value;
};
template<typename Find, typename Tuple>
inline constexpr ssize_t find_type_in_tuple_v = find_type_in_tuple<Find, Tuple>::value;


using int_types = std::tuple<int8_t, int16_t, int32_t, int64_t>;
using uint_types = std::tuple<uint8_t, uint16_t, uint32_t, uint64_t>;

using int_fast_types = std::tuple<int_fast8_t, int_fast16_t, int_fast32_t, int_fast64_t>;
using uint_fast_types = std::tuple<uint_fast8_t, uint_fast16_t, uint_fast32_t, uint_fast64_t>;

using reg_fast_types = std::tuple<int_fast32_t, int_fast32_t, int_fast32_t, int_fast64_t>;
using ureg_fast_types = std::tuple<uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast64_t>;

template <typename T>
using int_fast_t = std::tuple_element_t<find_type_in_tuple_v<T, int_types>, int_fast_types>;
template <typename T>
using uint_fast_t = std::tuple_element_t<find_type_in_tuple_v<T, uint_types>, uint_fast_types>;
template <typename T>
using reg_fast_t = std::tuple_element_t<find_type_in_tuple_v<T, int_types>, reg_fast_types>;
template <typename T>
using ureg_fast_t = std::tuple_element_t<find_type_in_tuple_v<T, uint_types>, ureg_fast_types>;

static forceinline void* alloc_vla(size_t size) {
	rsp_reg -= AlignUpToMultipleOf2(size, 16);
	return (void*)rsp_reg;
}

static forceinline void* alloc_vla_aligned(size_t size) {
	return alloc_vla(AlignUpToMultipleOf2(size, 16));
}


static forceinline void* chkstk(size_t size) {
	size = AlignUpToMultipleOf2(size, 4);
	volatile uint8_t* current = (uint8_t*)rsp_reg;
	size >>= 12;
	for (; size--; current += 0x1000) {
		(void)(*current & 0);
	}
	rsp_reg = (uintptr_t)current;
	return (void*)current;
}

//static forceinline 

static forceinline int32_t sign_extend_eax_mask(int32_t eax_value) {
	int32_t ret;
	__asm__(
		"CDQ"
		: asm_arg("=d", ret)
		: asm_arg("a", eax_value)
	);
	return ret;
}

static forceinline uint64_t rdtsc(void) {

	return __builtin_ia32_rdtsc();

	/*
	uintptr_t tickl, tickh;
	__asm__ volatile (
		"RDTSC \n"
		: asm_arg("=a", tickl), asm_arg("=d", tickh)
	);
	return (uint64_t)tickl + ((uint64_t)tickh << 32);
	*/
}

#ifdef _M_IX86
static forceinline uint32_t push_cs() {
	asm volatile(
		"PUSH CS"
	);
	return GARBAGE_VALUE(uint32_t);
}
#endif

template<typename T>
requires (std::is_integral_v<T> && sizeof(T) <= sizeof(usreg_t))
static forceinline T FarReturn64To32(T retval) {
    asm (
        "lret"
        :
        : "a"(retval)
    );
	//unreachable;
}
template<typename T>
requires (std::is_integral_v<T> && sizeof(T) > sizeof(usreg_t))
static forceinline T FarReturn64To32(T retval) {
    asm (
        "lret"
        :
        : "A"(retval)
    );
	//unreachable;
}
template<typename T>
requires (!std::is_integral_v<T> && sizeof(T) > sizeof(usreg_t))
static forceinline T FarReturn64To32(T retval) {
    asm (
        "lret"
        :
        : "x"(retval)
    );
	//unreachable;
}
static forceinline void FarReturn64To32() {
    asm (
        "lret"
    );
	//unreachable;
}

template<auto func>
struct FarCall32To64 {
	template <typename... Args>
	requires (std::is_invocable_v<decltype(func), Args...> && std::is_same_v<std::invoke_result_t<decltype(func), Args...>, void>)
	forceinline void operator()(Args... args) {
		asm(
			"lcall $0x33, %[func]"
			:
			: asm_arg("i", func)
		);
	}
	template <typename... Args>
	requires (std::is_invocable_v<decltype(func), Args...> && std::is_integral_v<std::invoke_result_t<decltype(func), Args...>> && sizeof(std::invoke_result_t<decltype(func), Args...>) <= sizeof(usreg_t))
	forceinline std::invoke_result_t<decltype(func), Args...> operator()(Args... args) {
		uint32_t ret;
		asm(
			"lcall $0x33, %[func]"
			: asm_arg("=a", ret)
			: asm_arg("i", func)
		);
		return ret;
	}
	template <typename... Args>
	requires (std::is_invocable_v<decltype(func), Args...> && std::is_integral_v<std::invoke_result_t<decltype(func), Args...>> && sizeof(std::invoke_result_t<decltype(func), Args...>) > sizeof(usreg_t))
	forceinline std::invoke_result_t<decltype(func), Args...> operator()(Args... args) {
		uint64_t ret;
		asm(
			"lcall $0x33, %[func]"
			: asm_arg("=A", ret)
			: asm_arg("i", func)
		);
		return ret;
	}
    template <typename... Args>
	requires (std::is_invocable_v<decltype(func), Args...> && !std::is_integral_v<std::invoke_result_t<decltype(func), Args...>> && sizeof(std::invoke_result_t<decltype(func), Args...>) > sizeof(usreg_t))
	forceinline std::invoke_result_t<decltype(func), Args...> operator()(Args... args) {
		register __m128 ret asm("xmm0");
		asm (
            "lcall $0x33, %[func]"
            : asm_arg("=x", ret)
            : asm_arg("i", func)
        );
		return bitcast<std::invoke_result_t<decltype(func), Args...>>(ret);
	}
};

static inline bool repne_scasd(const uint32_t value, const uint32_t*& array_ptr, size_t& array_len) {
	bool ret;
	__asm__ volatile (
		"REPNE SCASD"
		: asm_arg("+c", array_len), asm_arg("+D", array_ptr), asm_flags(z, ret)
		: asm_arg("a", value)
	);
	return ret;
}

template<size_t array_len>
static inline bool repne_scasd(const uint32_t value, const uint32_t(&array_ref)[array_len]) {
	size_t length = array_len;
	const uint32_t* array_ptr = array_ref;
	return repne_scasd(value, array_ptr, length);
}
template<size_t array_len>
static inline bool repne_scasd(const uint32_t value, const uint32_t(*array_ptr)[array_len]) {
	size_t length = array_len;
	const uint32_t* array_ptr = array_ref;
	return repne_scasd(value, array_ref, length);
}
static inline bool repne_scasd(const uint32_t value, const uint32_t* array_ref) {
	size_t fake_length = -1;
	return repne_scasd(value, array_ref, fake_length);
}

static inline void rep_movsd(void*restrict dst, const void*restrict src, size_t dword_len) {
	__asm__ volatile (
		"rep movsl"
		: asm_arg("+c", dword_len), asm_arg("+S", src), asm_arg("+D", dst)
		:
		: "memory"
	);
}

static inline dreg_t pack_dreg(const sreg_t low, const sreg_t high) {
	dreg_t ret;
	__asm__(
		""
		: asm_arg("=A", ret)
		: asm_arg("a", low), asm_arg("d", high)
	);
	return ret;
}

static inline udreg_t pack_udreg(const usreg_t low, const usreg_t high) {
	udreg_t ret;
	__asm__(
		""
		: asm_arg("=A", ret)
		: asm_arg("a", low), asm_arg("d", high)
	);
	return ret;
}

static inline void unpack_dreg(const dreg_t input, sreg_t& low, sreg_t& high) {
	__asm__(
		""
		: asm_arg("=a", low), asm_arg("=d", high)
		: asm_arg("A", input)
	);
}

static inline void unpack_udreg(const udreg_t input, usreg_t& low, usreg_t& high) {
	__asm__(
		""
		: asm_arg("=a", low), asm_arg("=d", high)
		: asm_arg("A", input)
	);
}


#ifdef _M_IX86
static gnu_noinline uint32_t LMrdfsbase32() {
	uint32_t ret;
	asm(
		".byte 0xF3, 0x0F, 0xAE, 0xC0"
		: asm_arg("=a", ret)
	);
	FarReturn64To32(ret);
}
#define rdfsbase32() []() { \
	FarCall32To64<LMrdfsbase32> call; \
	return call(); \
}()
static gnu_noinline vec<uint64_t, 2> LMrdfsbase64() {
	register vec<uint64_t, 2> ret asm("xmm0");
	asm(
		".byte 0xF3, 0x48, 0x0F, 0xAE, 0xC0 \n"
		".byte 0x66, 0x48, 0x0F, 0x6E, 0xC0"
		: asm_arg("=x", ret)
	);
	FarReturn64To32(ret);
}
#define rdfsbase64() []() -> uint64_t { \
	FarCall32To64<LMrdfsbase64> call; \
	return call()[0]; \
}()
static gnu_noinline uint32_t LMrdgsbase32() {
	uint32_t ret;
	asm(
		".byte 0xF3, 0x0F, 0xAE, 0xC8"
		: asm_arg("=a", ret)
	);
	FarReturn64To32(ret);
}
#define rdgsbase32() []() { \
	FarCall32To64<LMrdgsbase32> call; \
	return call(); \
}()
static gnu_noinline vec<uint64_t, 2> LMrdgsbase64() {
	register vec<uint64_t, 2> ret asm("xmm0");
	asm(
		".byte 0xF3, 0x48, 0x0F, 0xAE, 0xC8 \n"
		".byte 0x66, 0x48, 0x0F, 0x6E, 0xC0"
		: asm_arg("=x", ret)
	);
	FarReturn64To32(ret);
}
#define rdgsbase64() []() { \
	FarCall32To64<LMrdgsbase64> call; \
	return call()[0]; \
}()
static gnu_noinline void fastcall LMwrfsbase32(uint32_t addr) {
	asm(
		".byte 0xF3, 0x0F, 0xAR, 0xD1"
		:
		: asm_arg("c", addr)
	);
	FarReturn64To32();
}
#define wrfsbase32(...) [](uint32_t addr) { \
	FarCall32To64<LMwrfsbase32> call; \
	call(addr); \
}(__VA_ARGS__)
static gnu_noinline void fastcall LMwrfsbase64(uint32_t addr_low, uint32_t addr_high) {
	asm(
		".byte 0x48 \n"
		"shl $0x20, %[addr_high] \n"
		".byte 0x48 \n"
		"or %[addr_high], %[addr_low] \n"
		".byte 0xF3, 0x48, 0x0F, 0xAR, 0xD1"
		:
		: asm_arg("c", addr_low), asm_arg("d", addr_high)
	);
	FarReturn64To32();
}
#define wrfsbase64(...) [](uint64_t addr) { \
	FarCall32To64<LMwrfsbase64> call; \
	call(addr, addr >> 32); \
}(__VA_ARGS__)
static gnu_noinline void fastcall LMwrgsbase32(uint32_t addr) {
	asm(
		".byte 0xF3, 0x0F, 0xAR, 0xD1"
		:
		: asm_arg("r", addr)
	);
	FarReturn64To32();
}
#define wrgsbase32(...) [](uint32_t addr) { \
	FarCall32To64<LMwrgsbase32> call; \
	call(addr); \
}(__VA_ARGS__)
static gnu_noinline void fastcall LMwrgsbase64(uint32_t addr_low, uint32_t addr_high) {
	asm(
		".byte 0x48 \n"
		"shl $0x20, %[addr_high] \n"
		".byte 0x48 \n"
		"or %[addr_high], %[addr_low] \n"
		".byte 0xF3, 0x48, 0x0F, 0xAR, 0xD1"
		:
		: asm_arg("c", addr_low), asm_arg("d", addr_high)
	);
	FarReturn64To32();
}
#define wrgsbase64(...) [](uint64_t addr) { \
	FarCall32To64<LMwrgsbase64> call; \
	call(addr, addr >> 32); \
}(__VA_ARGS__)
#define rdfsbase rdfsbase32
#define rdgsbase rdgsbase32
#define wrfsbase wrfsbase32
#define wrgsbase wrgsbase32

#define rdzsbase32 rdgsbase32
#define rdzsbase64 rdgsbase64
#define wrzsbase32 wrgsbase32
#define wrzsbase64 wrgsbase64
#define rdzsbase rdgsbase
#define wrzsbase wrgsbase
#define ZS_RELATIVE GS_RELATIVE
#else
static inline uint32_t rdfsbase32() {
	uint32_t ret;
	asm(
		"RDFSBASE %[ret]"
		: asm_arg("=r", ret)
	);
	return ret;
}
static inline uint64_t rdfsbase64() {
	uint64_t ret;
	asm(
		"RDFSBASE %[ret]"
		: asm_arg("=r", ret)
	);
	return ret;
}
static inline uint32_t rdgsbase32() {
	uint32_t ret;
	asm(
		"RDGSBASE %[ret]"
		: asm_arg("=r", ret)
	);
	return ret;
}
static inline uint64_t rdgsbase64() {
	uint64_t ret;
	asm(
		"RDGSBASE %[ret]"
		: asm_arg("=r", ret)
	);
	return ret;
}
static inline void wrfsbase32(uint32_t addr) {
	asm(
		"WRFSBASE %[addr]"
		:
		: asm_arg("r", addr)
	);
}
static inline void wrfsbase64(uint64_t addr) {
	asm(
		"WRFSBASE %[addr]"
		:
		: asm_arg("r", addr)
	);
}
static inline void wrgsbase32(uint32_t addr) {
	asm(
		"WRGSBASE %[addr]"
		:
		: asm_arg("r", addr)
	);
}
static inline void wrgsbase64(uint64_t addr) {
	asm(
		"WRGSBASE %[addr]"
		:
		: asm_arg("r", addr)
	);
}
#define rdfsbase rdfsbase64
#define rdgsbase rdgsbase64
#define wrfsbase wrfsbase64
#define wrgsbase wrgsbase64

#define rdzsbase32 rdfsbase32
#define rdzsbase64 rdfsbase64
#define wrzsbase32 wrfsbase32
#define wrzsbase64 wrfsbase64
#define rdzsbase rdfsbase
#define wrzsbase wrfsbase
#define ZS_RELATIVE FS_RELATIVE
#endif

template <typename T>
static inline bool read_least_significant_bit(const T& value) {
	int carry_flag;
	__asm__(
		"ROL %[shift_count], %[value]"
		: asm_arg("+X", value), asm_flags(c, carry_flag)
		: [shift_count]"J"(sizeof(T))
	);
	return carry_flag;
}

template <typename T>
static inline bool read_most_significant_bit(const T& value) {
	int carry_flag;
	__asm__(
		"ROR %[shift_count], %[value]"
		: asm_arg("+X", value), asm_flags(c, carry_flag)
		: [shift_count] "J"(sizeof(T))
	);
	return carry_flag;
}

template <typename T>
static inline bool bit_test(const T& value, std::make_unsigned_t<std::remove_volatile_t<T>> bit_offset) {
	int carry_flag;
	if constexpr (std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {
		__asm__(
			"BTW %[bit_offset], %[value]"
			: asm_flags(c, carry_flag)
			: asm_arg("X", value), asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>) {
		__asm__(
			"BTL %[bit_offset], %[value]"
			: asm_flags(c, carry_flag)
			: asm_arg("X", value), asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
		__asm__(
			"BTQ %[bit_offset], %[value]"
			: asm_flags(c, carry_flag)
			: asm_arg("X", value), asm_arg("rNs", bit_offset)
		);
	}
	return carry_flag;
}

template <typename T>
static inline bool bit_test_set(T& value, std::make_unsigned_t<std::remove_volatile_t<T>> bit_offset) {
	int carry_flag;
	if constexpr (std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {
		__asm__(
			"BTSW %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>) {
		__asm__(
			"BTSL %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
		__asm__(
			"BTSQ %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	}
	return carry_flag;
}

template <typename T>
static inline bool bit_test_reset(T& value, std::make_unsigned_t<std::remove_volatile_t<T>> bit_offset) {
	int carry_flag;
	if constexpr (std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {
		__asm__(
			"BTRW %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>) {
		__asm__(
			"BTRL %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
		__asm__(
			"BTRQ %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	}
	return carry_flag;
}

template <typename T>
static inline bool bit_test_complement(T& value, std::make_unsigned_t<std::remove_volatile_t<T>> bit_offset) {
	int carry_flag;
	if constexpr (std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {
		__asm__(
			"BTCW %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>) {
		__asm__(
			"BTCL %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	} else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
		__asm__(
			"BTCQ %[bit_offset], %[value]"
			: asm_arg("+rm", value), asm_flags(c, carry_flag)
			: asm_arg("rNs", bit_offset)
		);
	}
	return carry_flag;
}

template <typename T>
static inline std::make_unsigned<std::remove_cvref_t<T>> bit_scan_forward(const T& value) {
	std::make_unsigned<std::remove_cvref_t<T>> ret;
	int zero_flag;
	if constexpr (std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {
		__asm__(
			"BSFW %[value], %[ret]"
			: asm_arg("=r", ret), asm_flags(z, zero_flag)
			: asm_arg("rm", value)
		);
	} else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>) {
		__asm__(
			"BSFL %[value], %[ret]"
			: asm_arg("=r", ret), asm_flags(z, zero_flag)
			: asm_arg("rm", value)
		);
	} else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
		__asm__(
			"BSFQ %[value], %[ret]"
			: asm_arg("=r", ret), asm_flags(z, zero_flag)
			: asm_arg("rm", value)
		);
	}
	return zero_flag ? ret : 0;
}

template <typename T>
static inline std::make_unsigned<std::remove_cvref_t<T>> bit_scan_reverse(const T& value) {
	std::make_unsigned<std::remove_cvref_t<T>> ret;
	int zero_flag;
	if constexpr (std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {
		__asm__(
			"BSRW %[value], %[ret]"
			: asm_arg("=r", ret), asm_flags(z, zero_flag)
			: asm_arg("rm", value)
		);
	} else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>) {
		__asm__(
			"BSRL %[value], %[ret]"
			: asm_arg("=r", ret), asm_flags(z, zero_flag)
			: asm_arg("rm", value)
		);
	} else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
		__asm__(
			"BSRQ %[value], %[ret]"
			: asm_arg("=r", ret), asm_flags(z, zero_flag)
			: asm_arg("rm", value)
		);
	}
	return zero_flag ? ret : 0;
}

#define HAS_BMI1 (__BMI__ || COMPILER_IS_MSVC && __AVX2__)
#define HAS_BMI1_NOT_MSVC (__BMI__ && !COMPILER_IS_MSVC)

template<typename T> requires(std::is_integral_v<T>)
static inline T sbb_bool(T value) {
	using U = std::make_unsigned_t<T>;
	return (T)((U)value < (U)value - 1 ? -1 : 0);
}

template<typename T> requires(std::is_integral_v<T>)
static inline T blsi(T value) {
#if HAS_BMI1
	if constexpr (sizeof(T) < sizeof(uint32_t)) {
		uint32_t value_wide = value;
		return (T)(value_wide & -value_wide);
	}
#endif
	return value & -value;
}

template<typename T> requires(std::is_integral_v<T>)
static inline T blsr(T value) {
#if HAS_BMI1
	if constexpr (sizeof(T) < sizeof(uint32_t)) {
		uint32_t value_wide = value;
		return (T)(value_wide & value_wide - 1);
	}
#endif
	return value & value - 1;
}

template<typename T> requires(std::is_integral_v<T>)
static inline T blsmsk(T value) {
#if HAS_BMI1
	if constexpr (sizeof(T) < sizeof(uint32_t)) {
		uint32_t value_wide = value;
		return (T)(value_wide ^ value_wide - 1);
	}
#endif
	return value ^ value - 1;
}

template<typename T> requires(std::is_integral_v<T>)
static inline T blsmskz(const T& value) {
#if HAS_BMI1_NOT_MSVC
	using R = std::conditional_t <sizeof(T) < sizeof(uint64_t), uint32_t, uint64_t>;
	R temp = value;
	R ret;
	bool carry_flag;
	asm(
		"BLSMSK %[temp], %[ret]"
		: asm_arg("=r", ret), asm_flags(c, carry_flag)
		: asm_arg("X", temp)
	);
	return ret + carry_flag;
#else
#if HAS_BMI1
	if constexpr (sizeof(T) < sizeof(uint32_t)) {
		uint32_t value_wide = value;
		return (T)(blsmsk(value_wide) - sbb_bool(value));
	}
#endif
	return blsmsk(value) - sbb_bool(value);
#endif
}

template<typename T> requires(std::is_integral_v<T>)
static inline T tzmsk(T value) {
#if HAS_BMI1
	return ~value & value - 1;
#else
	return (std::make_signed_t<T>)blsmsk(value) >> 1;
#endif
}

template <typename T> requires(std::is_integral_v<T>)
static inline T tzmskz(const T& value) {
#if !COMPILER_IS_MSVC
#if __TBM__ || __BMI__
	using R = std::conditional_t<sizeof(T) < sizeof(uint64_t), uint32_t, uint64_t>;
	R temp = value;
	R ret;
	bool carry_flag;
	asm(
#if __TBM__
		"TZMSK %[temp], %[ret]"
#elif __BMI__
		"BLSI %[temp], %[ret]"
#endif
		: asm_arg("=r", ret), asm_flags(c, carry_flag)
		: asm_arg("X", temp)
	);
#if __TBM__
		ret += carry_flag;
#elif __BMI__
		ret -= carry_flag;
#endif
	return ret;
#else
	T ret = value;
	return blsi(ret) + sbb_bool(ret);
#endif
#else
	T ret = value;
	return tzmsk(ret) - sbb_bool(ret);
#endif
}

static inline int8_t& aad_math(int8_t& out, const int8_t mul, int8_t add) {
	uint16_t temp = out | add << 8;
	__asm__(
		"AAD %[mul]"
		: asm_arg("=a", out)
		: asm_arg("a", temp), asm_arg("K", mul)
	);
	return out;
}

static inline void aam_math(int8_t in, const int8_t div, int8_t& div_out, int8_t& mod_out) {
	union {
		int16_t word;
		struct {
			int8_t low;
			int8_t high;
		};
	} temp;
	temp.low = in;
	temp.high = temp.high;
	__asm__(
		"AAM %[div]"
		: "+A" (temp.word)
		: asm_arg("K", div)
	);
	div_out = temp.low;
	mod_out = temp.high;
}

static inline bool complement_carry(void) {
	int carry_flag;
	__asm__(
		"CMC"
		: asm_flags(c, carry_flag)
	);
	return carry_flag;
}

static inline void clear_dir_flag() {
	asm(
		"CLD"
		:
		:
		: clobber_list("dirflag")
	);
}
static inline void set_dir_flag() {
	asm(
		"STD"
		:
		:
		: clobber_list("dirflag")
	);
}

template <bool wait = false>
static inline void clear_x87_exceptions() {
	if constexpr (!wait) {
		asm(
			"FNCLEX"
			:
			:
			: "fpsr"
		);
	} else {
		asm(
			"FCLEX"
			:
			:
			: "fpsr"
		);
	}
}

template <bool wait = false>
static inline uint16_t store_x87_cw(uint16_t& cw) {
	if constexpr (!wait) {
		asm(
			"FNSTCW %[cw]"
			: asm_arg("=m", cw)
		);
	} else {
		asm(
			"FSTCW %[cw]"
			: asm_arg("=m", cw)
		);
	}
	return cw;
}
template <bool wait = false>
static inline uint16_t store_x87_cw() {
	uint16_t cw;
	return store_x87_cw<wait>(cw);
}

template <bool wait = false>
static inline void load_x87_cw(uint16_t cw) {
	if constexpr (!wait) {
		asm(
			"FNLDCW %[cw]"
			:
			: asm_arg("m", cw)
		);
	} else {
		asm(
			"FLDCW %[cw]"
			:
			: asm_arg("m", cw)
		);
	}
}

template <bool wait = false>
static inline uint16_t store_x87_sw() {
	uint16_t sw;
	if constexpr (!wait) {
		asm(
			"FNSTSW ax"
			: asm_arg("=a", sw)
		);
	} else {
		asm(
			"FSTSW ax"
			: asm_arg("=a", sw)
		);
	}
	return sw;
}
template <bool wait = false>
static inline uint16_t store_x87_sw(uint16_t& sw) {
	if constexpr (!wait) {
		asm(
			"FNSTSW ax"
			: asm_arg("=m", sw)
		);
	} else {
		asm(
			"FSTSW ax"
			: asm_arg("=m", sw)
		);
	}
	return sw;
}

template <bool wait = false>
static inline void store_x87_env(void* env) {
	if constexpr (!wait) {
		asm(
			"FNSTENV %[env]"
			: asm_arg("=m", env)
			:
			: "fpcr"
		);
	} else {
		asm(
			"FSTENV %[env]"
			: asm_arg("=m", env)
			:
			: "fpcr"
		);
	}
}

static inline void load_x87_env(void* env) {
	asm(
		"FLDENV %[env]"
		: asm_arg("=m", env)
	);
}

static inline void get_cpuid(uint32_t page_num, uint32_t& eax_out, uint32_t& ebx_out, uint32_t& ecx_out, uint32_t& edx_out) {
	asm(
		"cpuid"
		: "=a"(eax_out), "=b"(ebx_out), "=c"(ecx_out), "=d"(edx_out)
		: "a"(page_num)
	);
}
static inline void get_cpuid_ex(uint32_t page_num, uint32_t subpage_num, uint32_t& eax_out, uint32_t& ebx_out, uint32_t& ecx_out, uint32_t& edx_out) {
	asm(
		"cpuid"
		: "=a"(eax_out), "=b"(ebx_out), "=c"(ecx_out), "=d"(edx_out)
		: "a"(page_num), "c"(subpage_num)
	);
}
static inline void get_cpuid2(uint32_t page_num, uint64_t& eax_edx_out, uint32_t& ebx_out, uint32_t& ecx_out) {
	asm(
		"cpuid"
		: "=A"(eax_edx_out), "=b"(ebx_out), "=c"(ecx_out)
		: "a"(page_num)
	);
}
static inline void get_cpuid_ex2(uint32_t page_num, uint32_t subpage_num, uint64_t& eax_edx_out, uint32_t& ebx_out, uint32_t& ecx_out) {
	asm(
		"cpuid"
		: "=A"(eax_edx_out), "=b"(ebx_out), "=c"(ecx_out)
		: "a"(page_num), "c"(subpage_num)
	);
}