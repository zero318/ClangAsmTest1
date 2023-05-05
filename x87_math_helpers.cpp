#include "x87_math_helpers.h"


[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl sinf_impl_manual() {
    //return angle;
}

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl cosf_impl_manual() {
    //return angle;
}

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl atan2_impl_manual() {
    //return Y + X;
}

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl fmod_impl_manual() {
    //return X + Y;
}

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl sinf_impl(double angle) {
    //return angle;
}

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl cosf_impl(double angle) {
    //return angle;
}

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl atan2_impl(double Y, double X) {
    //return Y + X;
}

[[clang::optnone, gnu::no_caller_saved_registers]] gnu_noinline void cdecl fmod_impl(double X, double Y) {
    //return X + Y;
}

#include <math.h>

dllexport int64_t ftoll(float input) asm("{[codecave:ftoll]}");
dllexport int64_t ftoll(float input) {
    return input;
}

dllexport int32_t ftol(float input) asm("{[codecave:ftol]}");
dllexport int32_t ftol(float input) {
    return input;
}