#ifndef CUSTOM_INTRIN_H
#define CUSTOM_INTRIN_H 1
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

#define FIX_LONG_DOUBLE_X87_ARGS(st0, ...) __asm__ volatile("":"=t"(st0) __VA_OPT__(,"=u"(__VA_ARGS__)))

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

typedef uint16_t seg_t;
struct FarPtr16 {
	uint16_t addr;
	seg_t seg;
};
template<typename T = void>
struct FarPtr32 {
	PTR32Z<T> addr;
	seg_t seg;
};
template<typename T = void>
struct FarPtr64 {
	PTR64<T> addr;
	seg_t seg;
};
struct AbsFarOpcode16 {
	uint8_t opcode gnu_packed; // 0x0
	uint16_t addr gnu_packed; // 0x1
	seg_t seg gnu_packed; // 0x3
	// 0x5
} gnu_packed;
struct AbsFarOpcode32 {
	uint8_t opcode gnu_packed; // 0x0
	uint32_t addr gnu_packed; // 0x1
	seg_t seg gnu_packed; // 0x5
	// 0x7
} gnu_packed;

#define ATT_SYNTAX_DIRECTIVE ".att_syntax \n"
#define INTEL_SYNTAX_DIRECTIVE ".intel_syntax \n"

#define CODE_16_DIRECTIVE ".code16 \n"
#define CODE_32_DIRECTIVE ".code32 \n"
#define CODE_64_DIRECTIVE ".code64 \n"

#define INTEL_16_DIRECTIVE INTEL_SYNTAX_DIRECTIVE CODE_16_DIRECTIVE
#define INTEL_32_DIRECTIVE INTEL_SYNTAX_DIRECTIVE CODE_32_DIRECTIVE
#define INTEL_64_DIRECTIVE INTEL_SYNTAX_DIRECTIVE CODE_64_DIRECTIVE
#define ATT_16_DIRECTIVE ATT_SYNTAX_DIRECTIVE CODE_16_DIRECTIVE
#define ATT_32_DIRECTIVE ATT_SYNTAX_DIRECTIVE CODE_32_DIRECTIVE
#define ATT_64_DIRECTIVE ATT_SYNTAX_DIRECTIVE CODE_64_DIRECTIVE

#define DATASIZE " .byte 0x66 \n "
#define ADDRSIZE " .byte 0x67 \n "

#define ES_OVERRIDE " .byte 0x26 \n "
#define CS_OVERRIDE " .byte 0x2E \n "
#define SS_OVERRIDE " .byte 0x36 \n "
#define DS_OVERRIDE " .byte 0x3E \n "
#define FS_OVERRIDE " .byte 0x64 \n "
#define GS_OVERRIDE " .byte 0x65 \n "

#define REX " .byte 0x40 \n "
#define REX_B " .byte 0x41 \n "
#define REX_X " .byte 0x42 \n "
#define REX_XB " .byte 0x43 \n "
#define REX_R " .byte 0x44 \n "
#define REX_RB " .byte 0x45 \n "
#define REX_RX " .byte 0x46 \n "
#define REX_RXB " .byte 0x47 \n "
#define REX_W " .byte 0x48 \n "
#define REX_WB " .byte 0x49 \n "
#define REX_WX " .byte 0x4A \n "
#define REX_WXB " .byte 0x4B \n "
#define REX_WR " .byte 0x4C \n "
#define REX_WRB " .byte 0x4D \n "
#define REX_WRX " .byte 0x4E \n "
#define REX_WRXB " .byte 0x4F \n "

#define LOCK " .byte 0xF0 \n "
#define REP " .byte 0xF3 \n "
#define REPE REP
#define REPZ REP
#define REPNE " .byte 0xF2 \n "
#define REPNZ REPNE
#define BND REPNE

typedef __int128_t int128_t;
typedef __uint128_t uint128_t;
//register int16_t sp_reg asm("sp");
//register int16_t bp_reg asm("bp");
register int32_t esp_reg asm("esp");
register int32_t ebp_reg asm("ebp");
#define ESP_REG "%%esp"
#define ESP_REG_NAME "esp"
#define EBP_REG "%%ebp"
#define EBP_REG_NAME "ebp"
#ifdef _M_IX86
typedef int32_t sreg_t;
typedef uint32_t usreg_t;
typedef int64_t dreg_t;
typedef uint64_t udreg_t;
#define JHCXZ "jcxz "
#define JPCXZ "jecxz "
#define INCSSP "incsspd "
#define RDSSP "rdsspd "
#define WRSS "wrssd "
#define WRUSS "wrussd "
#define rsp_reg esp_reg
#define rbp_reg ebp_reg
#define RSP_REG ESP_REG
#define RSP_REG_NAME ESP_REG_NAME
#define RBP_REG EBP_REG
#define RBP_REG_NAME EBP_REG_NAME
#define CODE_DEFAULT_DIRECTIVE CODE_32_DIRECTIVE
#else
typedef int64_t sreg_t;
typedef uint64_t usreg_t;
typedef int128_t dreg_t;
typedef uint128_t udreg_t;
#define JHCXZ "jecxz "
#define JPCXZ "jrcxz "
#define INCSSP "incsspq "
#define RDSSP "rdsspq "
#define WRSS "wrssq "
#define WRUSS "wrussq "
register int64_t rsp_reg asm("rsp");
register int64_t rbp_reg asm("rbp");
#define RSP_REG "%%rsp"
#define RSP_REG_NAME "rsp"
#define RBP_REG "%%rbp"
#define RBP_REG_NAME "rbp"
#define CODE_DEFAULT_DIRECTIVE CODE_64_DIRECTIVE
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

static forceinline int32_t sign_extend_eax_mask(int32_t eax_value) {
	int32_t ret;
	__asm__(
		"CDQ"
		: asm_arg("=d", ret)
		: asm_arg("a", eax_value)
	);
	return ret;
}

static forceinline uint32_t rdtsc_low(void) {
	uint32_t tick;
	__asm__ volatile(
		"RDTSC"
		: asm_arg("=a", tick)
		:
		: clobber_list("edx")
	);
	return tick;
}

static forceinline uint64_t rdtsc(void) {
#ifdef _M_IX86
	uint64_t tick;
	__asm__ volatile (
		"RDTSC"
		: asm_arg("=A", tick)
	);
	return tick;
#else
	return __builtin_ia32_rdtsc();

	/*
	uintptr_t tickl, tickh;
	__asm__ volatile (
		"RDTSC"
		: asm_arg("=a", tickl), asm_arg("=d", tickh)
	);
	return (uint64_t)tickl + ((uint64_t)tickh << 32);
	*/
#endif
}

static forceinline uint64_t rdtscp(uint32_t& tsc_aux) {
#ifdef _M_IX86
	uint64_t tick;
	__asm__ volatile (
		"RDTSCP"
		: asm_arg("=A", tick), asm_arg("=c", tsc_aux)
	);
	return tick;
#else
	return __builtin_ia32_rdtscp(&tsc_aux);

	/*
	uintptr_t tickl, tickh;
	__asm__ volatile (
		"RDTSCP"
		: asm_arg("=a", tickl), asm_arg("=d", tickh), asm_arg("=c", tsc_aux)
	);
	return (uint64_t)tickl + ((uint64_t)tickh << 32);
	*/
#endif
}

static forceinline uint64_t rdtsc_serialize() {
	uint32_t idgaf;
	return rdtscp(idgaf);
}

static forceinline void serialize_instructions() {
	rdtsc_serialize(); // Surely there's a better way...
}

static uint32_t load_segment_limit(uint32_t value) {
	uint32_t ret;
	__asm__ volatile (
		"LSL %[ret], %[value]"
		: asm_arg("=r", ret)
		: asm_arg("r", value)
	);
	return ret;
}

#ifdef _M_IX86
static forceinline uint32_t push_cs() {
	asm volatile(
		"PUSH CS"
	);
	return GARBAGE_VALUE(uint32_t);
}
#endif

static forceinline void read_cs(uint16_t& out) {
	__asm__ volatile(
		"MOV %[out], CS"
		: asm_arg("=rm", out)
	);
}
static forceinline uint16_t read_cs() {
	uint16_t temp;
	read_cs(temp);
	return temp;
}

static forceinline void write_ss(const uint32_t& value) {
	__asm__ volatile(
		"MOV %[value], %%SS"
		:
		: [value]"r,m"(value)
	);
}
static forceinline void store_ss(uint32_t& value) {
	__asm__ volatile(
		"MOV %%SS, %[value]"
		: [value]"=r"(value)
	);
}
static forceinline uint32_t store_ss() {
	uint32_t temp;
	store_ss(temp);
	return temp;
}
static forceinline void write_ds(const uint16_t& value) {
	__asm__ volatile(
		"MOV DS, %[value]"
		:
		: [value] "r,m"((uint32_t)value)
	);
}
static forceinline void write_es(const uint16_t& value) {
	__asm__ volatile(
		"MOV ES, %[value]"
		:
		: [value] "r,m"((uint32_t)value)
	);
}
static forceinline void write_fs(const uint16_t& value) {
	__asm__ volatile(
		"MOV FS, %[value]"
		:
		: [value] "r,m"((uint32_t)value)
	);
}
static forceinline void write_gs(const uint16_t& value) {
	__asm__ volatile(
		"MOV GS, %[value]"
		:
		: [value] "r,m"((uint32_t)value)
	);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"

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

#pragma clang diagnostic pop

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
	const uint32_t* array_ptrB = array_ptr;
	return repne_scasd(value, array_ptrB, length);
}
static inline bool repne_scasd(const uint32_t value, const uint32_t* array_ref) {
	size_t fake_length = -1;
	return repne_scasd(value, array_ref, fake_length);
}

static inline void rep_movsd(void *restrict dst, const void *restrict src, size_t dword_len) {
	__asm__ volatile (
		"rep movsl"
		: asm_arg("+c", dword_len), asm_arg("+S", src), asm_arg("+D", dst)
		:
		: "memory"
	);
}
template<typename T = void, typename T2 = T>
static inline T *restrict rep_movsb(T *restrict dst, const T2 *restrict src, size_t byte_len) {
	__asm__ volatile (
		"rep movsb"
		: "=c"(byte_len), "+D"(dst), "+S"(src)
		: "0"(byte_len)
		: "memory"
	);
	//assume(byte_len == 0);
	return dst;
}
template<typename T = void>
static inline T *restrict rep_stosb(T *restrict dst, uint8_t value, size_t byte_len) {
	__asm__ volatile (
		"rep stosb"
		: "=c"(byte_len), "+D"(dst)
		: "0"(byte_len), "a"(value)
		: "memory"
	);
	//assume(byte_len == 0);
	return dst;
}
template<typename T = void>
static inline bool repe_cmpsb(T *restrict dst, T *restrict src, size_t byte_len) {
	__asm__ volatile (
		"repe cmpsb"
		: "=c"(byte_len), "+D"(dst), "+S"(src)
		: "0"(byte_len)
		: "memory"
	);
}
template<typename T = void>
static inline bool repne_scasb(T *restrict dst, int val, size_t byte_len) {
	__asm__ volatile (
		"repne scasb"
		: "=c"(byte_len), "+D"(dst)
		: "0"(byte_len), "a"(val)
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"

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

#pragma clang diagnostic pop

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

#define swapgs() __asm__ volatile (".byte 0x0F, 0x01, 0xF8 \n")

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

#define swapgs() __asm__ volatile ("swapgs \n")

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
static inline std::make_unsigned_t<std::remove_cvref_t<T>> bit_scan_forward(const T& value) {
	std::make_unsigned_t<std::remove_cvref_t<T>> ret;
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
static inline std::make_unsigned_t<std::remove_cvref_t<T>> bit_scan_reverse(const T& value) {
	std::make_unsigned_t<std::remove_cvref_t<T>> ret;
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

template<typename T> requires(std::is_integral_v<T>)
static forceinline bool umul_overflow(T left, T right, T* out) {
	bool carry;
#if IS_X64

#else
	if constexpr (std::is_same_v<T, uint32_t>) {
		uint64_t value;
		__asm__ volatile (
			"mul %[right]"
			: asm_arg("=A", value), asm_flags(c, carry)
			: asm_arg("%a", left), asm_arg("%r", right)
		);
		if (out) *out = (uint32_t)value;
	}
#endif
	return carry;
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

#ifndef __x86_64__
static inline uint16_t aad_math(uint16_t in, const uint8_t mul = 10u) {
	__asm__(
		"AAD %[mul]"
		: asm_arg("+a", in)
		: asm_arg("N", mul)
		: "cc"
	);
	return in;
}
static inline uint16_t aad_math(uint8_t low, uint8_t high, const uint8_t mul = 10u) {
	return aad_math(PackUInt16(low, high), mul);
}

struct aam_ret {
	uint8_t remainder;
	uint8_t quotient;
};
static inline aam_ret aam_math(uint8_t dividend, const uint8_t divisor = 10u) {
	register uint8_t remainder asm("al");
	register uint8_t quotient asm("ah");
	__asm__(
		"AAM %[divisor]"
		: asm_arg("=a", remainder), asm_arg("=a", quotient)
		: asm_arg("a", dividend), asm_arg("N", divisor)
		: "cc"
	);
	return { remainder, quotient };
}
#endif

template<typename T>
size_t fastcall get_hex_digit_count(T value) {
    if constexpr (sizeof(T) == sizeof(uint8_t)) {
        return value > 0xF;
    }
    else {
        T ret = 0;
#if __LZCNT__
        if constexpr (sizeof(T) == sizeof(uint16_t)) {
            int carry_flag;
            __asm__(
                "lzcntw %[value], %[ret]"
                : asm_arg("=r", ret), asm_flags(c, carry_flag)
                : asm_arg("r", value)
            );
            ret -= carry_flag;
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t)) {
            int carry_flag;
            __asm__(
                "lzcntl %[value], %[ret]"
				: asm_arg("=r", ret), asm_flags(c, carry_flag)
				: asm_arg("r", value)
            );
            ret -= carry_flag;
        }
#if __x86_64__
        else if constexpr (sizeof(T) == sizeof(uint64_t)) {
            int carry_flag;
            __asm__(
                "lzcntq %[value], %[ret]"
				: asm_arg("=r", ret), asm_flags(c, carry_flag)
				: asm_arg("r", value)
            );
            ret -= carry_flag;
        }
        else
#endif
#endif
        {
            ret = std::countl_zero<T>(value | 1u);
        }
        return (ret ^ bitsof(T) - 1) >> 2;
    }
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

enum x86FloatPrecision : uint8_t {
	SinglePrecision = 0,
	DoublePrecision = 2,
	ExtendedPrecision = 3
};
enum x86FloatRounding : uint8_t {
	RoundToNearest = 0,
	RoundDown = 1,
	RoundUp = 2,
	RoundTowardsZero = 3
};
union FCW {
	uint16_t raw;
	struct {
		uint8_t low_byte;
		uint8_t high_byte;
	};
	struct {
		uint16_t invalid_operation_mask : 1; // 0
		uint16_t denormal_operand_mask : 1; // 1
		uint16_t divide_by_zero_mask : 1; // 2
		uint16_t overflow_mask : 1; // 3
		uint16_t underflow_mask : 1; // 4
		uint16_t precision_mask : 1; // 5
		uint16_t : 2; // 6-7
		uint16_t precision : 2; // 8-9
		uint16_t rounding : 2; // 10-11
		uint16_t infinity_control : 1; // 12
		uint16_t : 3; // 13-15
	};
	struct {
		uint16_t exception_masks : 6;
	};
};
ValidateStructSize(0x2, FCW);
union FCWW {
	uint32_t raw_wide;
	uint16_t raw;
	struct {
		uint8_t low_byte;
		uint8_t high_byte;
	};
	struct {
		uint32_t invalid_operation_mask : 1; // 0
		uint32_t denormal_operand_mask : 1; // 1
		uint32_t divide_by_zero_mask : 1; // 2
		uint32_t overflow_mask : 1; // 3
		uint32_t underflow_mask : 1; // 4
		uint32_t precision_mask : 1; // 5
		uint32_t : 2; // 6-7
		uint32_t precision : 2; // 8-9
		uint32_t rounding : 2; // 10-11
		uint32_t infinity_control : 1; // 12
		uint32_t : 19; // 13-31
	};
	struct {
		uint32_t exception_masks : 6;
	};
};
ValidateStructSize(0x4, FCWW);

union FSW {
	uint16_t raw;
	struct {
		uint16_t invalid_operation_exception : 1; // 0
		uint16_t denormal_operand_exception : 1; // 1
		uint16_t divide_by_zero_exception : 1; // 2
		uint16_t overflow_exception : 1; // 3
		uint16_t underflow_exception : 1; // 4
		uint16_t precision_exception : 1; // 5
		uint16_t stack_fault : 1; // 6
		uint16_t exception_summary : 1; // 7
		uint16_t c0 : 1; // 8
		uint16_t c1 : 1; // 9
		uint16_t c2 : 1; // 10
		uint16_t stack_top : 3; // 11-13
		uint16_t c3 : 1; // 14
		uint16_t busy : 1; // 15
	};
	struct {
		uint16_t exceptions : 6;
	};
};
ValidateStructSize(0x2, FSW);
union FSWW {
	uint32_t raw_wide;
	uint16_t raw;
	struct {
		uint32_t invalid_operation_exception : 1;
		uint32_t denormal_operand_exception : 1;
		uint32_t divide_by_zero_exception : 1;
		uint32_t overflow_exception : 1;
		uint32_t underflow_exception : 1;
		uint32_t precision_exception : 1;
		uint32_t stack_fault : 1;
		uint32_t exception_summary : 1;
		uint32_t c0 : 1;
		uint32_t c1 : 1;
		uint32_t c2 : 1;
		uint32_t stack_top : 3;
		uint32_t c3 : 1;
		uint32_t busy : 1;
		uint32_t : 16;
	};
	struct {
		uint32_t exceptions : 6;
	};
};
ValidateStructSize(0x4, FSWW);

union MXCSR {
	uint32_t raw;
	struct {
		uint32_t invalid_operation_exception : 1; // 0
		uint32_t denormal_operand_exception : 1; // 1
		uint32_t divide_by_zero_exception : 1; // 2
		uint32_t overflow_exception : 1; // 3
		uint32_t underflow_exception : 1; // 4
		uint32_t precision_exception : 1; // 5
		uint32_t denormals_are_zeros : 1; // 6
		uint32_t invalid_operation_mask : 1; // 7
		uint32_t denormal_operand_mask : 1; // 8
		uint32_t divide_by_zero_mask : 1; // 9
		uint32_t overflow_mask : 1; // 10
		uint32_t underflow_mask : 1; // 11
		uint32_t precision_mask : 1; // 12
		uint32_t rounding : 2; // 13-14
		uint32_t flush_to_zero : 1; // 15
		uint32_t : 1; // 16
		uint32_t misaligned_exception_mask : 1; // 17
		uint32_t : 2; // 18-19
		uint32_t __unknown_k10m_bit_A : 1; // 20
		uint32_t disable_unmasked_exceptions : 1; // 21
		uint32_t : 10; // 22-31
	};
	struct {
		uint32_t exceptions : 6;
		uint32_t : 1;
		uint32_t exception_masks : 6;
	};
};
ValidateStructSize(0x4, MXCSR);

enum VexState {
	SSE_Encoding,
	VEX_Encoding,
	EVEX_Encoding
};

template <bool wait = false>
static inline void clear_x87_exceptions() {
	if constexpr (!wait) {
		__asm__ volatile (
			"FNCLEX"
			:
			:
			: "fpsr"
		);
	} else {
		__asm__ volatile (
			"FCLEX"
			:
			:
			: "fpsr"
		);
	}
}

template <bool wait = false>
static inline FCW store_x87_cw(FCW& cw) {
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTCW %[cw]"
			: asm_arg("=m", cw)
		);
	} else {
		__asm__ volatile (
			"FSTCW %[cw]"
			: asm_arg("=m", cw)
		);
	}
	return cw;
}
template <bool wait = false>
static inline FCW store_x87_cw() {
	FCW cw;
	return store_x87_cw<wait>(cw);
}
template <bool wait = false>
static inline FCWW store_x87_cw(FCWW& cww) {
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTCW %[cww]"
			: asm_arg("=m", cww)
		);
	} else {
		__asm__ volatile (
			"FSTCW %[cww]"
			: asm_arg("=m", cww)
		);
	}
	return cww;
}
#define store_fcw(...) (store_x87_cw(__VA_ARGS__))
#define current_fcw() (store_fcw())

template <bool wait = false>
static inline FCWW store_x87_cww(FCWW& cww) {
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTCW %[cww]"
			: asm_arg("=m", cww)
		);
	} else {
		__asm__ volatile (
			"FSTCW %[cww]"
			: asm_arg("=m", cww)
		);
	}
	return cww;
}
template <bool wait = false>
static inline FCWW store_x87_cww() {
	FCWW cww;
	return store_x87_cww<wait>(cww);
}
#define store_fcww(...) (store_x87_cww(__VA_ARGS__))
#define current_fcww() (store_fcww())

static inline void load_x87_cw(const FCW& cw) {
	__asm__ volatile (
		"FLDCW %[cw]"
		:
		: asm_arg("m", cw)
	);
}
static inline void load_x87_cw(const FCWW& cw) {
	__asm__ volatile (
		"FLDCW %[cw]"
		:
		: asm_arg("m", cw)
	);
}
#define load_fcw(cw) (load_x87_cw(cw))
#define load_fcww(cww) (load_x87_cw(cww))


template <bool wait = false>
static inline FSW store_x87_sw() {
	FSW sw;
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTSW %[sw]"
			: asm_arg("=a", sw)
		);
	} else {
		__asm__ volatile (
			"FSTSW %[sw]"
			: asm_arg("=a", sw)
		);
	}
	return sw;
}
template <bool wait = false>
static inline FSW store_x87_sw(FSW& sw) {
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTSW %[sw]"
			: asm_arg("=m", sw)
		);
	} else {
		__asm__ volatile (
			"FSTSW %[sw]"
			: asm_arg("=m", sw)
		);
	}
	return sw;
}
#define store_fsw(...) (store_x87_sw(__VA_ARGS__))
#define current_fsw() (store_fsw())

template <bool wait = false>
static inline FSWW store_x87_sww() {
	FSWW sww;
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTSW %[sww]"
			: asm_arg("=a", sww)
		);
	} else {
		__asm__ volatile (
			"FSTSW %[sww]"
			: asm_arg("=a", sww)
		);
	}
	return sww;
}
template <bool wait = false>
static inline FSWW store_x87_sww(FSWW& sww) {
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTSW %[sww]"
			: asm_arg("=m", sww)
		);
	} else {
		__asm__ volatile (
			"FSTSW %[sww]"
			: asm_arg("=m", sww)
		);
	}
	return sww;
}
#define store_fsww(...) (store_x87_sww(__VA_ARGS__))
#define current_fsww() (store_fsww())

template <bool wait = false>
static inline void store_x87_env(void* env) {
	if constexpr (!wait) {
		__asm__ volatile (
			"FNSTENV %[env]"
			: asm_arg("=m", env)
			:
			: "fpcr"
		);
	} else {
		__asm__ volatile (
			"FSTENV %[env]"
			: asm_arg("=m", env)
			:
			: "fpcr"
		);
	}
}

#if __AVX__
static inline constexpr VexState default_mxcsr_encoding = VEX_Encoding;
#else
static inline constexpr VexState default_mxcsr_encoding = SSE_Encoding;
#endif

template <VexState encoding = default_mxcsr_encoding>
static inline MXCSR store_mxcsr(MXCSR& mxcsr) {
	if constexpr (encoding == SSE_Encoding) {
		__asm__ volatile (
			"stmxcsr %[mxcsr]"
			: asm_arg("=m", mxcsr)
		);
	} else {
		__asm__ volatile (
			"vstmxcsr %[mxcsr]"
			: asm_arg("=m", mxcsr)
		);
	}
	return mxcsr;
}
template <VexState encoding = default_mxcsr_encoding>
static inline MXCSR store_mxcsr() {
	MXCSR ret;
	return store_mxcsr<encoding>(ret);
}
template <VexState encoding = default_mxcsr_encoding>
static inline MXCSR current_mxcsr() {
	return store_mxcsr<encoding>();
}

template <VexState encoding = default_mxcsr_encoding>
static inline void load_mxcsr(const MXCSR& mxcsr) {
	if constexpr (encoding == SSE_Encoding) {
		__asm__ volatile (
			"ldmxcsr %[mxcsr]"
			:
			: asm_arg("m", mxcsr)
		);
	} else {
		__asm__ volatile (
			"vldmxcsr %[mxcsr]"
			:
			: asm_arg("m", mxcsr)
		);
	}
}

static inline void load_x87_env(void* env) {
	__asm__ volatile (
		"FLDENV %[env]"
		: asm_arg("=m", env)
	);
}

static inline void get_cpuid(uint32_t page_num, uint32_t& eax_out, uint32_t& ebx_out, uint32_t& ecx_out, uint32_t& edx_out) {
	__asm__ volatile (
		"cpuid"
		: "=a"(eax_out), "=b"(ebx_out), "=c"(ecx_out), "=d"(edx_out)
		: "a"(page_num)
	);
}
static inline void get_cpuid_ex(uint32_t page_num, uint32_t subpage_num, uint32_t& eax_out, uint32_t& ebx_out, uint32_t& ecx_out, uint32_t& edx_out) {
	__asm__ volatile (
		"cpuid"
		: "=a"(eax_out), "=b"(ebx_out), "=c"(ecx_out), "=d"(edx_out)
		: "a"(page_num), "c"(subpage_num)
	);
}
static inline void get_cpuid2(uint32_t page_num, uint64_t& eax_edx_out, uint32_t& ebx_out, uint32_t& ecx_out) {
	__asm__ volatile (
		"cpuid"
		: "=A"(eax_edx_out), "=b"(ebx_out), "=c"(ecx_out)
		: "a"(page_num)
	);
}
static inline void get_cpuid_ex2(uint32_t page_num, uint32_t subpage_num, uint64_t& eax_edx_out, uint32_t& ebx_out, uint32_t& ecx_out) {
	__asm__ volatile (
		"cpuid"
		: "=A"(eax_edx_out), "=b"(ebx_out), "=c"(ecx_out)
		: "a"(page_num), "c"(subpage_num)
	);
}

template<uint8_t index, typename T>
struct CR {

	static inline void write_raw(usreg_t value) {
		if constexpr (index == 0) {
			__asm__ volatile (
				"mov %[value], %%cr0"
				:
				: asm_arg("r", value)
			);
		} else if constexpr (index == 2) {
			__asm__ volatile (
				"mov %[value], %%cr2"
				:
				: asm_arg("r", value)
			);
		} else if constexpr (index == 3) {

		}
	}

};

template<uint32_t index, typename T>
struct MSR {

	static inline void regparm(2) write_raw_wide(udreg_t value) {
		__asm__ volatile (
			"wrmsr"
			:
			: "A"(value), "c"(index)
		);
	}

	static inline void regparm(2) write_raw_halves(const uint32_t low, const uint32_t high) {
		__asm__ volatile (
			"wrmsr"
			:
			: "a"(low), "d"(high), "c"(index)
		);
	}

	static inline void regparm(2) write_raw(const uint64_t value) {
#ifndef __x86_64__
		__asm__ volatile (
			"wrmsr"
			:
			: "A"(value), "c"(index)
		);
#else
		write_raw_halves(value, value >> 32);
#endif
	}
    
    static inline void write(T& value) {
        write_raw(bitcast<uint64_t>(value));
    }
	
    template<bool nest = true> requires(nest)
    static inline void write(const T&& value) {
		write_raw(bitcast<uint64_t>(value));
    }
    
	template<typename ... Args, bool nest = false> requires(!nest)
    static inline void write(Args&&... args) {
		write<true>({{}, std::forward<Args>(args)... });
    }

	static inline udreg_t read_raw_wide() {
		udreg_t ret;
		__asm__ volatile (
			"rdmsr"
			: "=A"(ret)
			: "c"(index)
		);
		return ret;
	}
    
    static inline uint64_t read_raw() {
#ifndef __x86_64__
		uint64_t ret;
#else
		uint32_t ret_low;
		uint32_t ret_high;
#endif
        __asm__ volatile (
            "rdmsr"
#ifndef __x86_64__
            : "=A"(ret)
#else
            : "=a"(ret_low), "=d"(ret_high)
#endif
            : "c"(index)
        );
#ifndef __x86_64__
		return ret;
#else
		return (uint64_t)ret_low + ((uint64_t)ret_high << 32);
#endif
    }

    static inline T read() {
        return bitcast<T>(read_raw());
    }
};

#define RSB_COUNT 32
#define RSB_LOOP_SIZE 2
template<size_t rsb_count = RSB_COUNT, size_t rsb_loop_size = RSB_LOOP_SIZE, bool inc_shadow_stack = false>
static forceinline void stuff_rsb() {
	uint32_t loop_counter = rsb_count / rsb_loop_size;
	nounroll do {
		__asm__ volatile (
			".rept %c[rsb_loop_size] \n"
				"call 1f \n"
				"int3 \n"
			"1: \n"
			".endr \n"
#ifdef __x86_64__
				"addq %[rsb_loop_offset], %%rsp \n"
#else
				"addl %[rsb_loop_offset], %%esp \n"
#endif
			:
			: asm_arg("i", rsb_loop_size), [rsb_loop_offset]"i"(rsb_loop_size * sizeof(void*))
		);
		if constexpr (inc_shadow_stack) {
			__asm__ volatile (
				INCSSP "%[rsb_loop_size] \n"
				:
				: asm_arg("r", rsb_loop_size)
			);
		}
	} while (--loop_counter);
	if constexpr (constexpr size_t rsb_extra_count = rsb_count % rsb_loop_size) {
		__asm__ volatile (
			".rept %c[rsb_extra_count] \n"
				"call 1f \n"
				"int3 \n"
			"1: \n"
			".endr \n"
#ifdef __x86_64__
				"addq %[rsb_extra_offset], %%rsp \n"
#else
				"addl %[rsb_extra_offset], %%esp \n"
#endif
			:
			: asm_arg("i", rsb_extra_count), [rsb_extra_offset]"i"(rsb_extra_count * sizeof(void*))
		);
		if constexpr (inc_shadow_stack) {
			__asm__ volatile (
				INCSSP "%[rsb_extra_count] \n"
				:
				: asm_arg("r", rsb_extra_count)
			);
		}
	}
}

#if !__has_builtin(__builtin_add_overflow)
#define __builtin_add_overflow __builtin_add_overflow_impl
// Return result if not pointer null
template<typename T>
static inline constexpr bool __builtin_add_overflow_impl(T a, T b, T* res) {
    if constexpr (std::is_signed_v<T>) {
        using U = std::make_unsigned_t<T>;
        U result = (U)a + (U)b;
        *res = (T)result;
        if (a > 0) {
            return b <= (std::numeric_limits<T>::max)() - a;
        } else {
            return b >= (std::numeric_limits<T>::min)() - a;
        }
    } else {
        return (*res = a + b) >= a;
    }
}
// Ignore result if pointer is null
template<typename T>
static inline constexpr bool __builtin_add_overflow_impl(T a, T b, std::nullptr_t res) {
    if constexpr (std::is_signed_v<T>) {
        if (a > 0) {
            return b <= std::numeric_limits<T>::max() - a;
        } else {
            return b >= std::numeric_limits<T>::min() - a;
        }
    } else {
        return a + b >= a;
    }
}
#endif

#if !__has_builtin(__builtin_sub_overflow)
#define __builtin_sub_overflow __builtin_sub_overflow_impl
// Return result if not pointer null
template<typename T>
static inline constexpr bool __builtin_sub_overflow_impl(T a, T b, T* res) {
    if constexpr (std::is_signed_v<T>) {
        using U = std::make_unsigned_t<T>;
        U result = (U)a - (U)b;
        *res = (T)result;
        if (a > 0) {
            return b >= (std::numeric_limits<T>::max)() - a;
        } else {
            return b <= (std::numeric_limits<T>::min)() - a;
        }
    } else {
        return (*res = a - b) <= a;
    }
}
// Ignore result if pointer is null
template<typename T>
static inline constexpr bool __builtin_sub_overflow_impl(T a, T b, std::nullptr_t res) {
    if constexpr (std::is_signed_v<T>) {
        if (a > 0) {
            return b >= std::numeric_limits<T>::max() - a;
        } else {
            return b <= std::numeric_limits<T>::min() - a;
        }
    } else {
        return a - b <= a;
    }
}
#endif

#if !__has_builtin(__builtin_addcb)
#define __builtin_addcb __builtin_addcb_impl
static inline constexpr unsigned char __builtin_addcb_impl(unsigned char lhs, unsigned char rhs, unsigned char carry_in, unsigned char* carry_out) {
	unsigned char s = 0;
	unsigned char c1 = __builtin_add_overflow(lhs, rhs, &s);
	unsigned char c2 = __builtin_add_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_subcb)
#define __builtin_subcb __builtin_subcb_impl
static inline constexpr unsigned char __builtin_subcb_impl(unsigned char lhs, unsigned char rhs, unsigned char carry_in, unsigned char* carry_out) {
	unsigned char s = 0;
	unsigned char c1 = __builtin_sub_overflow(lhs, rhs, &s);
	unsigned char c2 = __builtin_sub_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_addcs)
#define __builtin_addcs __builtin_addcs_impl
static inline constexpr unsigned short __builtin_addcs_impl(unsigned short lhs, unsigned short rhs, unsigned short carry_in, unsigned short* carry_out) {
	unsigned short s = 0;
	unsigned short c1 = __builtin_add_overflow(lhs, rhs, &s);
	unsigned short c2 = __builtin_add_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_subcs)
#define __builtin_subcs __builtin_subcs_impl
static inline constexpr unsigned short __builtin_subcs_impl(unsigned short lhs, unsigned short rhs, unsigned short carry_in, unsigned short* carry_out) {
	unsigned short s = 0;
	unsigned short c1 = __builtin_sub_overflow(lhs, rhs, &s);
	unsigned short c2 = __builtin_sub_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_addc)
#define __builtin_addc __builtin_addc_impl
static inline constexpr unsigned int __builtin_addc_impl(unsigned int lhs, unsigned int rhs, unsigned int carry_in, unsigned int* carry_out) {
	unsigned int s = 0;
	unsigned int c1 = __builtin_add_overflow(lhs, rhs, &s);
	unsigned int c2 = __builtin_add_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_subc)
#define __builtin_subc __builtin_subc_impl
static inline constexpr unsigned int __builtin_subc_impl(unsigned int lhs, unsigned int rhs, unsigned int carry_in, unsigned int* carry_out) {
	unsigned int s = 0;
	unsigned int c1 = __builtin_sub_overflow(lhs, rhs, &s);
	unsigned int c2 = __builtin_sub_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_addcl)
#define __builtin_addcl __builtin_addcl_impl
static inline constexpr unsigned long __builtin_addcl_impl(unsigned long lhs, unsigned long rhs, unsigned long carry_in, unsigned long* carry_out) {
	unsigned long s = 0;
	unsigned long c1 = __builtin_add_overflow(lhs, rhs, &s);
	unsigned long c2 = __builtin_add_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_subcl)
#define __builtin_subcl __builtin_subcl_impl
static inline constexpr unsigned long __builtin_subcl_impl(unsigned long lhs, unsigned long rhs, unsigned long carry_in, unsigned long* carry_out) {
	unsigned long s = 0;
	unsigned long c1 = __builtin_sub_overflow(lhs, rhs, &s);
	unsigned long c2 = __builtin_sub_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_addcll)
#define __builtin_addcll __builtin_addcll_impl
static inline constexpr unsigned long long __builtin_addcl_impl(unsigned long long lhs, unsigned long long rhs, unsigned long long carry_in, unsigned long long* carry_out) {
	unsigned long long s = 0;
	unsigned long long c1 = __builtin_add_overflow(lhs, rhs, &s);
	unsigned long long c2 = __builtin_add_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

#if !__has_builtin(__builtin_subcll)
#define __builtin_subcll __builtin_subcll_impl
static inline constexpr unsigned long long __builtin_subcl_impl(unsigned long long lhs, unsigned long long rhs, unsigned long long carry_in, unsigned long long* carry_out) {
	unsigned long long s = 0;
	unsigned long long c1 = __builtin_sub_overflow(lhs, rhs, &s);
	unsigned long long c2 = __builtin_sub_overflow(s, carry_in, &s);
	*carry_out = c1 | c2;
	return s;
}
#endif

template<typename T>
static inline constexpr T saturate_add(T lhs, T rhs) {
	if constexpr (std::is_signed_v<T>) {
		using U = std::make_unsigned_t<T>;
		T ret;
		if (!__builtin_add_overflow(lhs, rhs, &ret)) {
			return ret;
		}
		return ((U)rhs >> (bitsof(T) - 1)) + (U)(std::numeric_limits<T>::max)();
	} else {
		T ret = lhs + rhs;
		return ret >= lhs ? ret : (std::numeric_limits<T>::max)();
	}
}

template<typename T>
static inline constexpr T saturate_sub(T lhs, T rhs) {
	if constexpr (std::is_signed_v<T>) {
		using U = std::make_unsigned_t<T>;
		T ret;
		if (!__builtin_sub_overflow(lhs, rhs, &ret)) {
			return ret;
		}
		return (U)(std::numeric_limits<T>::min)() - ((U)rhs >> (bitsof(T) - 1));
	} else {
		T ret = lhs - rhs;
		return ret <= lhs ? ret : (std::numeric_limits<T>::min)();
	}
}

template<typename T>
static inline constexpr T carry_add(T lhs, T rhs, bool& carry) {
	if constexpr (sizeof(T) == sizeof(unsigned char)) {
		unsigned char carry_temp = carry;
		T ret = __builtin_addcb(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned short)) {
		unsigned short carry_temp = carry;
		T ret = __builtin_addcs(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned int)) {
		unsigned int carry_temp = carry;
		T ret = __builtin_addc(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned long)) {
		unsigned long carry_temp = carry;
		T ret = __builtin_addcl(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned long long)) {
		unsigned long long carry_temp = carry;
		T ret = __builtin_addcll(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
}

template<typename T>
static inline constexpr T carry_add(T lhs, T rhs, bool* carry) {
	return carry_add(lhs, rhs, *carry);
}

template<typename T>
static inline constexpr T carry_sub(T lhs, T rhs, bool& carry) {
	if constexpr (sizeof(T) == sizeof(unsigned char)) {
		unsigned char carry_temp = carry;
		T ret = __builtin_subcb(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned short)) {
		unsigned short carry_temp = carry;
		T ret = __builtin_subcs(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned int)) {
		unsigned int carry_temp = carry;
		T ret = __builtin_subc(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned long)) {
		unsigned long carry_temp = carry;
		T ret = __builtin_subcl(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
	else if constexpr (sizeof(T) == sizeof(unsigned long long)) {
		unsigned long long carry_temp = carry;
		T ret = __builtin_subcll(lhs, rhs, carry_temp, &carry_temp);
		carry = carry_temp;
		return ret;
	}
}

template<typename T>
static inline constexpr T carry_sub(T lhs, T rhs, bool* carry) {
	return carry_sub(lhs, rhs, *carry);
}

#endif