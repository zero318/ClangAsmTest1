#pragma once
#ifndef FUNC_TRAITS_H
#define FUNC_TRAITS_H

#include "zero/util.h"
//#include "compiler_support.h"
#include <tuple>
#include <type_traits>

#if NATIVE_BITS == 16
#error "WTF it's 16 bit garbo"
#endif

/*

__cdecl, _cdecl, __attribute__((cdecl)), [[gnu::cdecl]]
__fastcall, _fastcall, __attribute__((fastcall)), [[gnu::fastcall]]
__stdcall, _stdcall, __attribute__((stdcall)), [[gnu::stdcall]]
__thiscall, _thiscall, __attribute__((thiscall)), [[gnu::thiscall]]
__vectorcall, _vectorcall, __attribute__((vectorcall)), [[clang::vectorcall]]

__regcall, __attribute__((regcall)), [[gnu::regcall]]
__pascal, _pascal, __attribute__((pascal)), [[clang::pascal]]
__attribute__((regparm(X))), [[gnu::regparm(X)]]
__attribute__((intel_ocl_bicc)), [[clang::intel_ocl_bicc]]
__attribute__((swiftcall)), [[clang::swiftcall]]

__attribute__((ms_abi)), [[gnu::ms_abi]]
__attribute__((sysv_abi)), [[gnu::sysv_abi]]

__attribute__((interrupt)), [[gnu::interrupt]]
__attribute__((no_caller_saved_registers)), [[gnu::no_caller_saved_registers]]

Old BS:
__fortran
__syscall

Extra Obscure BS:
__watcall
_System

*/

/*

MSVC __cdecl Win32: _func
Int Args:       Stack
Float Args:     Stack
LongDoubleArgs: Stack
MMX Args:       EDX:EAX, Stack:ECX, Stack
Vector Args:    XMM0, XMM1, XMM2, Stack
Int Ret:        EDX:EAX
Float Ret:      ST(0)
LongDouble Ret: ST(0)
MMX Ret:        EDX:EAX
Vector Ret:     XMM0
IVec Fback Ret: EDX:EAX
FVec Fback Ret: ST(1):ST(0)
Preserved:      EBX, EBP, ESI, EDI
Cleanup:        Caller
Arg Order:      Right to Left
Decoration:     '_' prefix
Notes:          Supports va-args. Can be un-prototyped.

MSVC __stdcall Win32: _func@0
Int Args:       Stack
Float Args:     Stack
LongDoubleArgs: Stack
MMX Args:       EDX:EAX, Stack:ECX, Stack
Vector Args:    XMM0, XMM1, XMM2, Stack
Int Ret:        EDX:EAX
Float Ret:      ST(0)
LongDouble Ret: ST(0)
MMX Ret:        EDX:EAX
Vector Ret:     XMM0
IVec Fback Ret: EDX:EAX
FVec Fback Ret: ST(1):ST(0)
Preserved:      EBX, EBP, ESI, EDI
Cleanup:        Callee
Arg Order:      Right to Left
Decoration:     '_' prefix and '@bytes' suffix

MSVC __fastcall Win32: @func@0
Int Args:       ECX, EDX, Stack
Float Args:     Stack
LongDoubleArgs: Stack
MMX Args:       EDX:ECX, Stack
Vector Args:    XMM0, XMM1, XMM2, Stack
Int Ret:        EDX:EAX
Float Ret:      ST(0)
LongDouble Ret: ST(0)
MMX Ret:        EDX:EAX
Vector Ret:     XMM0
IVec Fback Ret: EDX:EAX
FVec Fback Ret: ST(1):ST(0)
Preserved:      EBX, EBP, ESI, EDI
Cleanup:        Callee
Arg Order:      Right to Left
Decoration:     '@' prefix and '@bytes' suffix

MSVC __thiscall Win32:
Int Args:       ECX, Stack
Float Args:     Stack
LongDoubleArgs: Stack
MMX Args:       Stack:ECX, Stack
Vector Args:    XMM0, XMM1, XMM2, Stack
Int Ret:        EDX:EAX
Float Ret:      ST(0)
LongDouble Ret: ST(0)
MMX Ret:        EDX:EAX
Vector Ret:     XMM0
IVec Fback Ret: EDX:EAX
FVec Fback Ret: ST(1):ST(0)
Preserved:      EBX, EBP, ESI, EDI
Cleanup:        Callee
Arg Order:      Right to Left
Decoration:     

MSVC __vectorcall Win32: func@@0
Int Args:       ECX, EDX, Stack
Float Args:     XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, Stack
LongDoubleArgs: ST(0), ST(1), ST(2), ST(3), ST(4), ST(5), Compiler Crash
MMX Args:       EDX:ECX, Stack
Vector Args:    ZMM0, ZMM1, ZMM2, ZMM3, ZMM4, ZMM5, Stack
Int Ret:        EDX:EAX
Float Ret:      XMM0, XMM1, XMM2, XMM3
LongDouble Ret: ST(0)
MMX Ret:        EDX:EAX
Vector Ret:     ZMM0, ???
IVec Fback Ret: EDX:EAX
FVec Fback Ret: Compile Error
Preserved:      EBX, EBP, ESI, EDI
Cleanup:        Callee
Arg Order:      Right to Left
Decoration:     '@@bytes' suffix
Notes:          Clang generates incorrect code for actually reading more than 1 long double arg!
                This can be semi-fixed via inline assembly for <=2 args, but still sucks.
                Also crashes the compiler

Clang __regcall Win32:
Int Args:       EAX, ECX, EDX, EDI, ESI, Stack
Float Args:     XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, ???
LongDoubleArgs: ST(0), Stack
MMX Args:       ECX:EAX, EDI:EDX, ???
Vector Args:    ZMM0, ZMM1, ZMM2, ZMM3, ZMM4, ZMM5, ZMM6, ???
Int Ret:        EDI:EDX:ECX:EAX
Float Ret:      XMM0
LongDouble Ret: ST(0)
MMX Ret:        ECX:EAX
Vector Ret:     ZMM0, ???
IVec Fback Ret: ECX:EAX
FVec Fback Ret: ST(1):ST(0)
Preserved:      EBX, EBP
Cleanup:        Caller
Arg Order:      
Decoration:     

Clang [[clang::intel_ocl_bicc]] Win32:
Int Args:       EAX, EDX, ECX, Stack (Only for vector fallback or something?)
Float Args:     Stack
LongDoubleArgs: Stack
MMX Args:       EDX:EAX, Stack:ECX, Stack
Vector Args:    XMM0, XMM1, XMM2, Stack
Int Ret:        EDX:EAX
Float Ret:      ST(0)
LongDouble Ret: ST(0)
MMX Ret:        EDX:EAX
Vector Ret:     XMM0
IVec Fback Ret: EDX:EAX
FVec Fback Ret: ST(1):ST(0)
Preserved:      EBX, EBP, ESI, EDI
Cleanup:        Caller
Arg Order:      
Decoration:     

GCC __atttibute__((regparm(3))) Win32:
Int Args:       EAX, EDX, ECX, Stack
Float Args:     Stack
LongDoubleArgs: Stack
MMX Args:       Stack
Vector Args:    Stack
Int Ret:        EDX:EAX
Float Ret:      ST(0)
LongDouble Ret: ST(0)
MMX Ret:        EDX:EAX
Vector Ret:     XMM0
IVec Fback Ret: EDX:EAX
FVec Fback Ret: ST(1):ST(0)
Preserved:      EBX, EBP, ESI, EDI
Cleanup:        Caller
Arg Order:      
Decoration:     

MSVC default Win64: func
Int Args:       RCX, RDX, R8, R9, Stack
Float Args:     XMM0, XMM1, XMM2, XMM3, Stack
LongDoubleArgs: Stack
MMX Args:       RCX, RDX, R8, R9, Stack
Vector Args:    Stack
Int Ret:        RDX:RAX
Float Ret:      XMM0
LongDouble Ret: RCX Ptr
MMX Ret:        RAX
Vector Ret:     XMM0
IVec Fback Ret: EDX:EAX
FVec Fback Ret: EDX:EAX
Preserved:      RBX, RBP, RSI, RDI, R12, R13, R14, R15, XMM6-XMM15 (Not YMM6-YMM15), FCW, MXCSR
Cleanup:        Callee
Arg Order:      Right to Left
Decoration:     None
Notes:          Supports va-args. Can be un-prototyped.

MSVC __vectorcall Win64:
Int Args:       RCX, RDX, R8, R9, Stack
Float Args:     XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, Stack
LongDoubleArgs: ST(0), ST(1), ST(2), ST(3), ST(4), ST(5), Stack
MMX Args:       RCX, RDX, R8, R9, Stack
Vector Args:    ZMM0, ZMM1, ZMM2, ZMM3, ZMM4, ZMM5, Stack
Int Ret:        RAX
Float Ret:      XMM0
LongDouble Ret: RCX Ptr
MMX Ret:        RAX
Vector Ret:     ZMM0
IVec Fback Ret: RDX:RAX
FVec Fback Ret: Compiler Crash
Preserved:      RBX, RBP, RSI, RDI, R12-R15, XMM6-XMM15 (Not YMM6-YMM15), FCW, MXCSR
Cleanup:        Caller
Arg Order:      Right to Left
Decoration:     

Clang __regcall Win64:
Int Args:       RAX, RCX, RDX, RDI, RSI, R8, R9, R10, R11, R12, R14, R15, Stack
Float Args:     XMM0-XMM15, Stack
LongDoubleArgs: ST(0), Stack
MMX Args:       
Vector Args:    ZMM0-ZMM15, Stack
Int Ret:        RCX:RAX
Float Ret:      XMM0
LongDouble Ret: ST(0)
MMX Ret:        RAX
Vector Ret:     ZMM0
IVec Fback Ret: 
FVec Fback Ret: 
Preserved:      RBX, RBP
Cleanup:        
Arg Order:      
Decoration:     

Clang default sysv64:
Int Args:       RDI, RSI, RDX, RCX, R8, R9, Stack
Float Args:     XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7, Stack
LongDoubleArgs: Stack
MMX Args:       XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7, Stack
Vector Args:    ZMM0, ZMM1, ZMM2, ZMM3, ZMM4, ZMM5, ZMM6, ZMM7, Stack
Int Ret:        RDX:RAX
Float Ret:      XMM1:XMM0
LongDouble Ret: ST(1):ST(0)
MMX Ret:        XMM0
Vector Ret:     ZMM0
IVec Fback Ret: 
FVec Fback Ret: 
Preserved:      RBX, RBP, R12, R13, R14, R15
Cleanup:        
Arg Order:      Right to Left
Decoration:     
Notes:          

*/

enum CallingConvention {
    CC_CDECL,
    CC_STDCALL,
    CC_FASTCALL,
    CC_THISCALL,
    CC_VECTORCALL,
    CC_REGCALL,
    CC_REGPARM0,
    CC_REGPARM1,
    CC_REGPARM2,
    CC_REGPARM3,
    CC_INTELOCL,
    CC_SWIFTCALL,
    CC_PASCAL,
    CC_SYSV_ABI,
    CC_MS_ABI
};

#ifdef cdecl
#undef cdecl
#endif
#ifdef pascal
#undef pascal
#endif

#if MSVC_COMPAT

#define cdecl __cdecl
#define stdcall __stdcall
#define fastcall __fastcall
#define thiscall __thiscall
#define vectorcall __vectorcall

#if COMPILER_IS_CLANG
#define regcall __attribute__((regcall))
#define regparm(count) __attribute__((regparm(count)))
#define intelocl __attribute__((intel_ocl_bicc))
#define swiftcall __attribute__((swiftcall))
#define pascal __attribute__((pascal))
#define sysv_abi __attribute__((sysv_abi))
#define ms_abi __attribute__((ms_abi))
#else
#define regcall
#define regparm(count)
#define intelocl
#define swiftcall
#define pascal stdcall
#define sysv_abi
#define ms_abi
#endif

#elif GCC_COMPAT

#define cdecl __attribute__((cdecl))
#define stdcall __attribute__((stdcall))
#define fastcall __attribute__((fastcall))
#define thiscall __attribute__((thiscall))
#define regcall
#define regparm(count) __attribute__((regparm(count)))
#define intelocl
#define pascal stdcall
#define sysv_abi __attribute__((sysv_abi))
#define ms_abi __attribute__((ms_abi))

#endif

struct CallingConventionTemplates {
    using CDECL_PTR = void(cdecl*)(void);
    using STDCALL_PTR = void(stdcall*)(void);
    using FASTCALL_PTR = void(fastcall*)(void);
    using THISCALL_PTR = void(thiscall*)(void);
    using VECTORCALL_PTR = void(vectorcall*)(void);
    using REGCALL_PTR = void(regcall*)(void);
    using REGPARM0_PTR = void(regparm(0)*)(void);
    using REGPARM1_PTR = void(regparm(1)*)(void);
    using REGPARM2_PTR = void(regparm(2)*)(void);
    using REGPARM3_PTR = void(regparm(3)*)(void);
    using INTELOCL_PTR = void(intelocl*)(void);
    using PASCAL_PTR = void(pascal*)(void);
    using SYSV_ABI_PTR = void(sysv_abi*)(void);
    using MS_ABI_PTR = void(ms_abi*)(void);
};

/*
template<typename R, typename ... Args>
struct FunctionTypeTraitsBase {
    using ret_type = R;
    using arg_types = std::tuple<Args...>;
    static inline constexpr auto arg_count = sizeof...(Args);

    template<size_t N>
    using nth_arg_type = std::tuple_element_t<N, arg_types>;
};

template<typename F, typename = void>
struct FunctionTypeTraits;

#define SPECIALIZE_FUNC_TRAITS_INNER(convention, convention_enum, convention_ptr, noexcept_state, arg_list) \
template<typename R, typename ... Args> requires((CallingConvention::convention_enum == CallingConvention::CC_CDECL) || !std::is_same_v<CallingConventionTemplates::CDECL_PTR,CallingConventionTemplates::convention_ptr>)\
struct FunctionTypeTraits<R(convention*)arg_list noexcept(noexcept_state), \
    std::enable_if_t<sizeof(std::decay<R>**)>> : FunctionTypeTraitsBase<R, Args...> { \
    typedef R(convention* pointer)arg_list noexcept(noexcept_state); \
    static inline constexpr auto calling_convention = CallingConvention::convention_enum; \
    static inline constexpr bool is_noexcept = noexcept_state; \
    static inline constexpr bool is_variadic = !std::is_same_v<void(*)arg_list, void(*)(Args...)>; \
}
#define SPECIALIZE_FUNC_TRAITS(convention, convention_enum, convention_ptr, ...) \
SPECIALIZE_FUNC_TRAITS_INNER(convention, convention_enum, convention_ptr, true, (Args...)); \
SPECIALIZE_FUNC_TRAITS_INNER(convention, convention_enum, convention_ptr, false, (Args...)) \
__VA_OPT__(; \
SPECIALIZE_FUNC_TRAITS_INNER(convention, convention_enum, convention_ptr, true, (Args...,...)); \
SPECIALIZE_FUNC_TRAITS_INNER(convention, convention_enum, convention_ptr, false, (Args...,...)) \
)

#define FunctionTraits(func) FunctionTypeTraits<decltype(&func)>

SPECIALIZE_FUNC_TRAITS(cdecl, CC_CDECL, CDECL_PTR, true);
SPECIALIZE_FUNC_TRAITS(stdcall, CC_STDCALL, STDCALL_PTR, true);
SPECIALIZE_FUNC_TRAITS(fastcall, CC_FASTCALL, FASTCALL_PTR, true);
SPECIALIZE_FUNC_TRAITS(thiscall, CC_THISCALL, THISCALL_PTR);
SPECIALIZE_FUNC_TRAITS(vectorcall, CC_VECTORCALL, VECTORCALL_PTR);
SPECIALIZE_FUNC_TRAITS(regcall, CC_REGCALL, REGCALL_PTR);
SPECIALIZE_FUNC_TRAITS(regparm(0), CC_REGPARM0, REGPARM0_PTR, true);
SPECIALIZE_FUNC_TRAITS(regparm(1), CC_REGPARM1, REGPARM1_PTR, true);
SPECIALIZE_FUNC_TRAITS(regparm(2), CC_REGPARM2, REGPARM2_PTR, true);
SPECIALIZE_FUNC_TRAITS(regparm(3), CC_REGPARM3, REGPARM3_PTR, true);
SPECIALIZE_FUNC_TRAITS(intelocl, CC_INTELOCL, INTELOCL_PTR, true);
SPECIALIZE_FUNC_TRAITS(pascal, CC_PASCAL, PASCAL_PTR);
SPECIALIZE_FUNC_TRAITS(sysv_abi, CC_SYSV_ABI, SYSV_ABI_PTR, true);
SPECIALIZE_FUNC_TRAITS(ms_abi, CC_MS_ABI, MS_ABI_PTR, true);
*/

template <typename R, CallingConvention convention, bool _is_variadic, bool _is_noexcept, typename ... Args>
struct FunctionTraitsBase {
    using ret_type = R;
    using arg_types = std::tuple<Args...>;
    static inline constexpr auto arg_count = sizeof...(Args);

    template<size_t N>
    using nth_arg_type = std::tuple_element_t<N, arg_types>;

    static inline constexpr auto calling_convention = convention;
    static inline constexpr bool is_noexcept = _is_noexcept;
    static inline constexpr bool is_variadic = _is_variadic;
};

#define SPECIALIZE_FUNC_TRAITS(convention, convention_enum, ...) \
template <typename R, typename ... Args> constexpr FunctionTraitsBase<R, convention_enum, false, false, Args...> get_func_types_of2(R(convention*)(Args...) noexcept(false)); \
template <typename R, typename ... Args> constexpr FunctionTraitsBase<R, convention_enum, false, true, Args...> get_func_types_of2(R(convention*)(Args...) noexcept(true)) \
__VA_OPT__(; \
template <typename R, typename ... Args> constexpr FunctionTraitsBase<R, convention_enum, true, false, Args...> get_func_types_of2(R(convention*)(Args...,...) noexcept(false)); \
template <typename R, typename ... Args> constexpr FunctionTraitsBase<R, convention_enum, true, true, Args...> get_func_types_of2(R(convention*)(Args...,...) noexcept(true)) \
)

SPECIALIZE_FUNC_TRAITS(cdecl, CC_CDECL, true);
SPECIALIZE_FUNC_TRAITS(stdcall, CC_STDCALL, true);
SPECIALIZE_FUNC_TRAITS(fastcall, CC_FASTCALL, true);
SPECIALIZE_FUNC_TRAITS(thiscall, CC_THISCALL);
SPECIALIZE_FUNC_TRAITS(vectorcall, CC_VECTORCALL);
SPECIALIZE_FUNC_TRAITS(regcall, CC_REGCALL);
SPECIALIZE_FUNC_TRAITS(regparm(0), CC_REGPARM0, true);
SPECIALIZE_FUNC_TRAITS(regparm(1), CC_REGPARM1, true);
SPECIALIZE_FUNC_TRAITS(regparm(2), CC_REGPARM2, true);
SPECIALIZE_FUNC_TRAITS(regparm(3), CC_REGPARM3, true);
SPECIALIZE_FUNC_TRAITS(intelocl, CC_INTELOCL, true);
SPECIALIZE_FUNC_TRAITS(pascal, CC_PASCAL);
SPECIALIZE_FUNC_TRAITS(sysv_abi, CC_SYSV_ABI, true);
SPECIALIZE_FUNC_TRAITS(ms_abi, CC_MS_ABI, true);

//requires(std::is_function_v<decltype(func)>)

template<auto func>
struct FunctionTraits : decltype(get_func_types_of2(func)) {

};


#endif