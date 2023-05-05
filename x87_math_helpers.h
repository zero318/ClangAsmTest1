#pragma once

#include "zero/util.h"

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl sinf_impl_manual();// asm("{[0x45BCF4]}");

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl cosf_impl_manual();// asm("{[0x45BDA4]}");

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl atan2_impl_manual();// asm("{[0x45BE40]}");

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl fmod_impl_manual();// asm("{[0x45BE60]}");

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl sinf_impl(double angle);// asm("{[0x45BCF4]}");

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl cosf_impl(double angle);// asm("{[0x45BDA4]}");

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl atan2_impl(double Y, double X);// asm("{[0x45BE40]}");

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl fmod_impl(double X, double Y);// asm("{[0x45BE60]}");