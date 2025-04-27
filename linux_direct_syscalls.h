#pragma once
#ifndef LINUX_DIRECT_SYSCALL_H
#define LINUX_DIRECT_SYSCALL_H 1
#include <stdint.h>
#include <type_traits>

#define _MACRO_STR(arg) #arg
#define MACRO_STR(arg) _MACRO_STR(arg)
#define MACRO_EVAL(...) __VA_ARGS__

#if SYSCALL_ENABLE_WIDE_RET
#define SYSCALL_WIDE_RET_MUL 2
#else
#define SYSCALL_WIDE_RET_MUL 1
#endif

#if __x86_64__
#define SYSCALL_INSTR "syscall"
#define SYSCALL_ARG0D
#define SYSCALL_ARG1D
#define SYSCALL_ARG2D
#define SYSCALL_ARG3D
#define SYSCALL_ARG4D register A4 a4b asm("r10") = a4
#define SYSCALL_ARG5D register A4 a4b asm("r10") = a4; register A5 a5b asm("r8") = a5
#define SYSCALL_ARG6D register A4 a4b asm("r10") = a4; register A5 a5b asm("r8") = a5; register A6 a6b asm("r9") = a6
#define SYSCALL_ARG0 "a"(number)
#define SYSCALL_ARG1 "a"(number),"D"(a1)
#define SYSCALL_ARG2 "a"(number),"D"(a1),"S"(a2)
#define SYSCALL_ARG3 "a"(number),"D"(a1),"S"(a2),"d"(a3)
#define SYSCALL_ARG4 "a"(number),"D"(a1),"S"(a2),"d"(a3),"r"(a4b)
#define SYSCALL_ARG5 "a"(number),"D"(a1),"S"(a2),"d"(a3),"r"(a4b),"r"(a5b)
#define SYSCALL_ARG6 "a"(number),"D"(a1),"S"(a2),"d"(a3),"r"(a4b),"r"(a5b),"r"(a6b)
#else
#define SYSCALL_INSTR "int $0x80"
#define SYSCALL_ARG0D
#define SYSCALL_ARG1D
#define SYSCALL_ARG2D
#define SYSCALL_ARG3D
#define SYSCALL_ARG4D
#define SYSCALL_ARG5D
#define SYSCALL_ARG6D register A6 a6b asm("ebp") = a6
#define SYSCALL_ARG0 "a"(number)
#define SYSCALL_ARG1 "a"(number),"b"(a1)
#define SYSCALL_ARG2 "a"(number),"b"(a1),"c"(a2)
#define SYSCALL_ARG3 "a"(number),"b"(a1),"c"(a2),"d"(a3)
#define SYSCALL_ARG4 "a"(number),"b"(a1),"c"(a2),"d"(a3),"S"(a4)
#define SYSCALL_ARG5 "a"(number),"b"(a1),"c"(a2),"d"(a3),"S"(a4),"D"(a5)
#define SYSCALL_ARG6 "a"(number),"b"(a1),"c"(a2),"d"(a3),"S"(a4),"D"(a5),"R"(a6b)
#endif

template<typename R, uintptr_t number>
static inline R $syscall_impl() noexcept(true) {
    SYSCALL_ARG0D;
    if constexpr (!std::is_same_v<R,void>) {
        R ret;
        if constexpr (sizeof(R) <= sizeof(uintptr_t)) __asm__ volatile (SYSCALL_INSTR:"=a"(ret):SYSCALL_ARG0);
        else __asm__ volatile (SYSCALL_INSTR:"=A"(ret):SYSCALL_ARG0);
        return ret;
    } else __asm__ volatile (SYSCALL_INSTR::SYSCALL_ARG0);
}
template<typename R, uintptr_t number, typename A1>
static inline R $syscall_impl(A1 a1) noexcept(true) {
    SYSCALL_ARG1D;
    if constexpr (!std::is_same_v<R,void>) {
        R ret;
        if constexpr (sizeof(R) <= sizeof(uintptr_t)) __asm__ volatile (SYSCALL_INSTR:"=a"(ret):SYSCALL_ARG1);
        else  __asm__ volatile (SYSCALL_INSTR:"=A"(ret):SYSCALL_ARG1);
        return ret;
    } else __asm__ volatile (SYSCALL_INSTR::SYSCALL_ARG1);
}
template<typename R, uintptr_t number, typename A1, typename A2>
static inline R $syscall_impl(A1 a1, A2 a2) noexcept(true) {
    SYSCALL_ARG2D;
    if constexpr (!std::is_same_v<R,void>) {
        R ret;
        if constexpr (sizeof(R) <= sizeof(uintptr_t)) __asm__ volatile (SYSCALL_INSTR:"=a"(ret):SYSCALL_ARG2);
        else __asm__ volatile (SYSCALL_INSTR:"=A"(ret):SYSCALL_ARG2);
        return ret;
    } else __asm__ volatile (SYSCALL_INSTR::SYSCALL_ARG2);
}
template<typename R, uintptr_t number, typename A1, typename A2, typename A3>
static inline R $syscall_impl(A1 a1, A2 a2, A3 a3) noexcept(true) {
    SYSCALL_ARG3D;
    if constexpr (!std::is_same_v<R,void>) {
        R ret;
        if constexpr (sizeof(R) <= sizeof(uintptr_t)) __asm__ volatile (SYSCALL_INSTR:"=a"(ret):SYSCALL_ARG3);
        else __asm__ volatile (SYSCALL_INSTR:"=A"(ret):SYSCALL_ARG3);
        return ret;
    } else __asm__ volatile (SYSCALL_INSTR::SYSCALL_ARG3);
}
template<typename R, uintptr_t number, typename A1, typename A2, typename A3, typename A4>
static inline R $syscall_impl(A1 a1, A2 a2, A3 a3, A4 a4) noexcept(true) {
    SYSCALL_ARG4D;
    if constexpr (!std::is_same_v<R,void>) {
        R ret;
        if constexpr (sizeof(R) <= sizeof(uintptr_t)) __asm__ volatile (SYSCALL_INSTR:"=a"(ret):SYSCALL_ARG4);
        else __asm__ volatile (SYSCALL_INSTR:"=A"(ret):SYSCALL_ARG4);
        return ret;
    } else __asm__ volatile (SYSCALL_INSTR::SYSCALL_ARG4);
}
template<typename R, uintptr_t number, typename A1, typename A2, typename A3, typename A4, typename A5>
static inline R $syscall_impl(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) noexcept(true) {
    SYSCALL_ARG5D;
    if constexpr (!std::is_same_v<R,void>) {
        R ret;
        if constexpr (sizeof(R) <= sizeof(uintptr_t)) __asm__ volatile (SYSCALL_INSTR:"=a"(ret):SYSCALL_ARG5);
        else __asm__ volatile (SYSCALL_INSTR:"=A"(ret):SYSCALL_ARG5);
        return ret;
    } else __asm__ volatile (SYSCALL_INSTR::SYSCALL_ARG5);
}
template<typename R, uintptr_t number, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
static inline R $syscall_impl(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) noexcept(true) {
    SYSCALL_ARG6D;
    if constexpr (!std::is_same_v<R,void>) {
        R ret;
        if constexpr (sizeof(R) <= sizeof(uintptr_t)) __asm__ volatile (SYSCALL_INSTR:"=a"(ret):SYSCALL_ARG6);
        else __asm__ volatile (SYSCALL_INSTR:"=A"(ret):SYSCALL_ARG6);
        return ret;
    } else __asm__ volatile (SYSCALL_INSTR::SYSCALL_ARG6);
}
template<typename R, uintptr_t number, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename ... Args>
static inline R $syscall_impl(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, Args... args) noexcept(true) {
    static_assert(false, "Way too many arguments in syscall declaration! SysV ABI only supports 6 arguments.");
}

#define SYSCALL_CONCAT(...) __VA_OPT__(,)__VA_ARGS__
#define SYSCALL_ARGS(T1,A1,T2,A2,T3,A3,T4,A4,T5,A5,T6,A6,T7,A7,...)\
MACRO_EVAL(T1 A1)\
SYSCALL_CONCAT(T2 A2)\
SYSCALL_CONCAT(T3 A3)\
SYSCALL_CONCAT(T4 A4)\
SYSCALL_CONCAT(T5 A5)\
SYSCALL_CONCAT(T6 A6)\
SYSCALL_CONCAT(T7 A7)
#define SYSCALL_VALUES(T1,A1,T2,A2,T3,A3,T4,A4,T5,A5,T6,A6,T7,A7,...)\
MACRO_EVAL(A1)\
SYSCALL_CONCAT(A2)\
SYSCALL_CONCAT(A3)\
SYSCALL_CONCAT(A4)\
SYSCALL_CONCAT(A5)\
SYSCALL_CONCAT(A6)\
SYSCALL_CONCAT(A7)

#define DECLARE_SYSCALL(ret, name, number, ...) \
static inline ret name(SYSCALL_ARGS(__VA_ARGS__,,,,,,,,,,,,,)) noexcept(true) { \
    static_assert(sizeof(std::conditional_t<std::is_same_v<ret,void>,unsigned char,ret>) <= sizeof(uintptr_t) * SYSCALL_WIDE_RET_MUL, "Return type \"" #ret "\" too large! SysV ABI only supports " MACRO_STR(SYSCALL_WIDE_RET_MUL) " register(s) for return values."); \
    return $syscall_impl<ret, (uintptr_t)number>(SYSCALL_VALUES(__VA_ARGS__,,,,,,,,,,,,,)); \
}

#endif