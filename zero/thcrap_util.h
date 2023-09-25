#pragma once
#include "force_macros.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "util.h"
//#undef _WIN64
#include "Windows.h"
#include <utility>

/*
template<typename F>
struct function_traits;
template<typename R, typename... Args>
struct function_traits<R(Args...)> {
	using ret_type = R;

};
*/

template<typename... Ts> struct Types {};

#ifdef cdecl
#undef cdecl
#endif
#define cdecl __cdecl
#ifdef pascal
#undef pascal
#endif

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

#ifndef __x86_64__
#define thcrap_builtin(func_name, ...) \
extern "C++" { \
using MACRO_CAT(func_name,T) = INTELLISENSE_TYPENAME typeof(MACRO_DEFAULT_ARG(func_name,__VA_ARGS__)); \
using MACRO_CAT(func_name,Ptr) = MACRO_CAT(func_name,T)*; \
dllexport gnu_used struct dummy_name { \
private: \
	using F = decltype(MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)); \
	using FTypes = decltype(get_func_types_of(std::declval<decltype(MACRO_DEFAULT_ARG(func_name, __VA_ARGS__))>())); \
	using FArgs = FTypes::Types; \
	using FRet = FTypes::R; \
	static constexpr bool FVariadic = FTypes::is_variadic; \
	template <typename> \
	struct CallFunc {}; \
	template <typename... Args> \
	struct CallFunc<Types<Args...>> { \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(cdecl*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto cdecl call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(cdecl*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto cdecl addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(cdecl*)(Args..., ...), F*> \
		dllexport gnu_used gnu_noinline static auto cdecl call(Args... args, ...) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			asm_func_jump(MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)); \
			return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(cdecl*)(Args..., ...), F*> \
		dllexport gnu_used gnu_noinline static auto cdecl addr(Args... args, ...) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			asm_func_jump(MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)); \
			return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(stdcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto stdcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(stdcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto stdcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(fastcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto fastcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(fastcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto fastcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(thiscall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto thiscall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(thiscall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto thiscall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(vectorcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto vectorcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(vectorcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto vectorcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		/*template <typename... Fargs> \
		requires std::is_same_v<FRet(regcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(0)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(0) call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(0)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(0) addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(1)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(1) call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(1)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(1) addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(2)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(2) call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(2)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(2) addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(3)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(3) call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(regparm(3)*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto regparm(3) addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(pascalcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto pascalcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(pascalcall*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto pascalcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		}*/ \
	}; \
public: \
	inline operator F*() { \
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	inline F* operator&() { \
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	template <typename... Args> \
	requires std::is_invocable_v<F*, Args...> \
	inline auto operator()(Args... args) { \
		return CallFunc<FArgs>::call(args...); \
	} \
} MACRO_CAT(th_,func_name); \
}
#else
#define thcrap_builtin(func_name, ...) \
extern "C++" { \
using MACRO_CAT(func_name,T) = INTELLISENSE_TYPENAME typeof(MACRO_DEFAULT_ARG(func_name,__VA_ARGS__)); \
using MACRO_CAT(func_name,Ptr) = MACRO_CAT(func_name,T)*; \
dllexport gnu_used struct dummy_name { \
private: \
	using F = decltype(MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)); \
	using FTypes = decltype(get_func_types_of(std::declval<MACRO_CAT(func_name,T)>())); \
	using FArgs = FTypes::Types; \
	using FRet = FTypes::R; \
	template <typename> \
	struct CallFunc {}; \
	template <typename... Args> \
	struct CallFunc<Types<Args...>> { \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(*)(Args...), F*> \
		dllexport gnu_used gnu_noinline static auto addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
	}; \
public: \
	inline operator F*() { \
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	inline F* operator&() { \
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	template <typename... Args> \
	requires std::is_invocable_v<F*, Args...> \
	inline auto operator()(Args... args) { \
		return CallFunc<FArgs>::call(args...); \
	} \
} MACRO_CAT(th_,func_name); \
}
#endif

#define thcrap_import(func_name, ...) \
using MACRO_CAT(func_name,Ptr) = decltype(&MACRO_DEFAULT_ARG(func_name,__VA_ARGS__)); \
dllexport gnu_used MACRO_CAT(func_name,Ptr) MACRO_CAT(func_name,_ptr) asm_symbol_abs(option:MACRO_CAT(func_name,_ptr))

#define thcrap_string(option_name, text) \
dllexport char option_name[] asm_symbol_abs(option:option_name) = text

#define thcrap_wstring(option_name, text) \
dllexport wchar_t option_name[] asm_symbol_abs(option:option_name) = text

#define thcrap_utf8_string(option_name, text) \
dllexport char8_t option_name[] asm_symbol_abs(option:option_name) = text

#define thcrap_code_string_ref(option_name, type, var_name) \
dllexport type var_name asm_symbol_abs(option:option_name)

#define thcrap_expr(expr_name, value) \
dllexport gnu_used struct dummy_name { \
	inline operator usreg_t() { \
		return bitcast<usreg_t>(this); \
	} \
} expr_name asm_symbol_expr(value)

extern "C" {
	thcrap_builtin(malloc);
	thcrap_builtin(calloc);
	thcrap_builtin(realloc);
	thcrap_builtin(free);
	thcrap_builtin(msize, _msize);
	thcrap_builtin(expand, _expand);
	thcrap_builtin(aligned_malloc, _aligned_malloc);
	thcrap_builtin(aligned_realloc, _aligned_realloc);
	thcrap_builtin(aligned_free, _aligned_free);
	thcrap_builtin(aligned_msize, _aligned_msize);
	thcrap_builtin(memcpy);
	thcrap_builtin(memmove);
	thcrap_builtin(memcmp);
	thcrap_builtin(memset);
	thcrap_builtin(memccpy);
	thcrap_builtin(strdup);
	thcrap_builtin(strcmp);
	thcrap_builtin(strncmp);
	thcrap_builtin(stricmp);
	thcrap_builtin(strnicmp);
	thcrap_builtin(strcpy);
	thcrap_builtin(strncpy);
	thcrap_builtin(strcat);
	thcrap_builtin(strncat);
	thcrap_builtin(strlen);
	thcrap_builtin(sprintf);
	thcrap_builtin(snprintf);
	thcrap_builtin(sscanf);
	thcrap_builtin(GetLastError);
	thcrap_builtin(GetProcAddress);
	thcrap_builtin(GetModuleHandleA);
	thcrap_builtin(GetModuleHandleW);
}

dllexport gnu_noinline const char* runconfig_game_get() asm_symbol_rel(runconfig_game_get);
dllexport gnu_noinline const char* runconfig_game_get() {
	return (const char*)(uintptr_t)rand();
}
dllexport gnu_noinline const char* runconfig_thcrap_dir_get() asm_symbol_rel(runconfig_thcrap_dir_get);
dllexport gnu_noinline const char* runconfig_thcrap_dir_get() {
	return (const char*)(uintptr_t)rand();
}
dllexport gnu_noinline const char* runconfig_runcfg_fn_get() asm_symbol_rel(runconfig_runcfg_fn_get);
dllexport gnu_noinline const char* runconfig_runcfg_fn_get() {
	return (const char*)(uintptr_t)rand();
}

dllexport gnu_noinline int log_mbox(const char* caption, const UINT type, const char* text) asm_symbol_rel(log_mbox);
dllexport gnu_noinline int log_mbox(const char* caption, const UINT type, const char* text) {
	use_var(caption);
	use_var(type);
	use_var(text);
	return rand();
}

dllexport gnu_noinline int log_vmboxf(const char* caption, const UINT type, const char* format, va_list va) asm_symbol_rel(log_vmboxf);
dllexport gnu_noinline int log_vmboxf(const char* caption, const UINT type, const char* format, va_list va) {
	use_var(caption);
	use_var(type);
	use_var(format);
	use_var(va);
	return rand();
}

dllexport gnu_noinline int log_mboxf(const char* caption, const UINT type, const char* format, ...) asm_symbol_rel(log_mboxf);
dllexport gnu_noinline int log_mboxf(const char* caption, const UINT type, const char* format, ...) {
	use_var(caption);
	use_var(type);
	va_list va;
	va_start(va, format);
	int ret = vsnprintf(NULL, 0, format, va_use(va));
	va_end(va);
	return ret + rand();
}
