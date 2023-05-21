#ifndef ZUN_MATH_H
#define ZUN_MATH_H 1

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "util.h"
#include "custom_intrin.h"

#undef IN

extern "C" {

    extern int64_t vectorcall _ftol(long double value) asm("__ftol");
    extern int64_t vectorcall _ftol2(long double value) asm("__ftol2");

    extern long double vectorcall _CIacos(long double value) asm("__CIacos");
    extern long double vectorcall _CIasin(long double value) asm("__CIasin");
    extern long double vectorcall _CIatan(long double value) asm("__CIatan");
    extern long double vectorcall _CIatan2(long double X, long double Y) asm("__CIatan2"); // ARGS ARE FLIPPED
    extern long double vectorcall _CIcos(long double angle) asm("__CIcos");
    extern long double vectorcall _CIcosh(long double angle) asm("__CIcosh");
    extern long double vectorcall _CIexp(long double value) asm("__CIexp");
    extern long double vectorcall _CIfmod(long double Y, long double X) asm("__CIfmod"); // ARGS ARE FLIPPED
    extern long double vectorcall _CIlog(long double value) asm("__CIlog");
    extern long double vectorcall _CIlog10(long double value) asm("__CIlog10");
    extern long double vectorcall _CIpow(long double exponent, long double base) asm("__CIpow"); // ARGS ARE FLIPPED
    extern long double vectorcall _CIsin(long double angle) asm("__CIsin");
    extern long double vectorcall _CIsinh(long double angle) asm("__CIsinh");
    extern long double vectorcall _CIsqrt(long double value) asm("__CIsqrt");
    extern long double vectorcall _CItan(long double angle) asm("__CItan");
    extern long double vectorcall _CItanh(long double angle) asm("__CItanh");

    extern double cdecl _acos(double value) asm("_acos");
    extern double cdecl _asin(double value) asm("_asin");
    extern double cdecl _atan(double value) asm("_atan");
    extern double cdecl _atan2(double Y, double X) asm("_atan2");
    extern double cdecl _cos(double angle) asm("_cos");
    extern double cdecl _cosh(double angle) asm("_cosh");
    extern double cdecl _exp(double value) asm("_exp");
    extern double cdecl _fmod(double X, double Y) asm("_fmod");
    extern double cdecl _log(double value) asm("_log");
    extern double cdecl _log10(double value) asm("_log10");
    extern double cdecl _pow(double base, double exponent) asm("_pow");
    extern double cdecl _sin(double angle) asm("_sin");
    extern double cdecl _sinh(double angle) asm("_sinh");
    extern double cdecl _sqrt(double value) asm("_sqrt");
    extern double cdecl _tan(double angle) asm("_tan");
    extern double cdecl _tanh(double angle) asm("_tanh");

    extern double vectorcall __libm_sse2_acos(double value) asm("___libm_sse2_acos");
    extern float vectorcall __libm_sse2_acosf(float value) asm("___libm_sse2_acosf");
    extern double vectorcall __libm_sse2_asin(double value) asm("___libm_sse2_asin");
    extern float vectorcall __libm_sse2_asinf(float value) asm("___libm_sse2_asinf");
    extern double vectorcall __libm_sse2_atan(double value) asm("___libm_sse2_atan");
    extern float vectorcall __libm_sse2_atanf(float value) asm("___libm_sse2_atanf");
    extern double vectorcall __libm_sse2_atan2(double Y, double X) asm("___libm_sse2_atan2");
    extern double vectorcall __libm_sse2_cos(double angle) asm("___libm_sse2_cos");
    extern float vectorcall __libm_sse2_cosf(float angle) asm("___libm_sse2_cosf");
    extern double vectorcall __libm_sse2_exp(double value) asm("___libm_sse2_exp");
    extern float vectorcall __libm_sse2_expf(float value) asm("___libm_sse2_expf");
    extern double vectorcall __libm_sse2_log(double value) asm("___libm_sse2_log");
    extern float vectorcall __libm_sse2_logf(float value) asm("___libm_sse2_logf");
    extern double vectorcall __libm_sse2_log10(double value) asm("___libm_sse2_log10");
    extern float vectorcall __libm_sse2_log10f(float value) asm("___libm_sse2_log10f");
    extern double vectorcall __libm_sse2_pow(double base, double exponent) asm("___libm_sse2_pow");
    extern float vectorcall __libm_sse2_powf(float base, float exponent) asm("___libm_sse2_powf");
    extern double vectorcall __libm_sse2_sin(double angle) asm("___libm_sse2_sin");
    extern float vectorcall __libm_sse2_sinf(float angle) asm("___libm_sse2_sinf");
    extern double vectorcall __libm_sse2_sqrt(double value) asm("___libm_sse2_sqrt");
    extern double vectorcall __libm_sse2_tan(double angle) asm("___libm_sse2_tan");
    extern float vectorcall __libm_sse2_tanf(float angle) asm("___libm_sse2_tanf");

    extern double cdecl _fabs(double value) asm("_fabs");
    extern double cdecl _floor(double value) asm("_floor");
    
}

static forceinline long double regcall rint_asm(long double value) {
    long double ret;
    __asm__ volatile ("frndint" : asm_arg("=t", ret) : asm_arg("0", value));
    return ret;
}
static forceinline long double regcall atan_asm(long double value) {
    long double ret, one;
    __asm__ volatile ("fld1" : asm_arg("=t", one));
    __asm__ volatile ("fpatan" : asm_arg("=t", ret) : asm_arg("0", one), asm_arg("u", value) : clobber_list("st(1)"));
    return ret;
}
static forceinline long double vectorcall atan2_asm(long double Y, long double X) {
    long double ret;
    __asm__ volatile ("fpatan" : asm_arg("=t", ret) : asm_arg("0", X), asm_arg("u", Y) : clobber_list("st(1)"));
    return ret;
}
static forceinline long double regcall cos_asm(long double angle) {
    long double cos;
    __asm__ volatile ("fcos" : asm_arg("=t", cos) : asm_arg("0", angle));
    return cos;
}
static forceinline long double regcall sin_asm(long double angle) {
    long double sin;
    __asm__ volatile ("fsin" : asm_arg("=t", sin) : asm_arg("0", angle));
    return sin;
}
static forceinline vec<long double, 2> regcall sincos_asm(long double angle) {
    long double cos, sin;
    __asm__ volatile ("fsincos" : asm_arg("=t", cos), asm_arg("=u", sin) : asm_arg("0", angle));
    return { cos, sin };
}
static forceinline long double regcall sqrt_asm(long double value) {
    long double ret;
    __asm__ volatile ("fsqrt" : asm_arg("=t", ret) : asm_arg("0", value));
    return ret;
}
static forceinline long double regcall tan_asm(long double angle) {
    long double ret, one;
    __asm__ volatile ("fptan" : asm_arg("=t", one), asm_arg("=u", ret) : asm_arg("0", angle));
    return ret;
}

static inline constexpr MXCSR default_mxcsr = {
    .exceptions = 0,
    .exception_masks = -1,
    .denormals_are_zeros = false,
    .rounding = RoundToNearest,
    .flush_to_zero = false,
    .misaligned_exception_mask = false,
    .disable_unmasked_exceptions = false
};
static inline constexpr MXCSR mxcsr_mask = {
    .exceptions = 0,
    .exception_masks = -1,
    .denormals_are_zeros = false,
    .rounding = RoundTowardsZero,
    .flush_to_zero = false,
    .misaligned_exception_mask = false,
    .disable_unmasked_exceptions = false
};
static inline constexpr FCW fcw_mask = {
    .low_byte = 0x7F,
    .high_byte = 0x00
};

// The "precise" variants of the libm functions aren't 
static forceinline bool control_words_are_normal() {
    if (expect((current_mxcsr<SSE_Encoding>().raw & mxcsr_mask.raw) == default_mxcsr.raw, true)) {
        if (expect((current_fcw().raw & fcw_mask.raw) == fcw_mask.raw, true)) {
            return true;
        }
    }
    return false;
}

static gnu_noinline double vectorcall __libm_sse2_acos_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return __libm_sse2_acos(value);
    }
    return _acos(value);
}
static gnu_noinline double vectorcall __libm_sse2_atan_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return __libm_sse2_atan(value);
    }
    return _atan(value);
}
static gnu_noinline double vectorcall __libm_sse2_cos_precise(double angle) {
    if (expect(control_words_are_normal(), true)) {
        return __libm_sse2_cos(angle);
    }
    return _cos(angle);
}
static gnu_noinline double vectorcall __libm_sse2_sin_precise(double angle) {
    if (expect(control_words_are_normal(), true)) {
        return __libm_sse2_sin(angle);
    }
    return _sin(angle);
}
static gnu_noinline double vectorcall __libm_sse2_sqrt_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return __libm_sse2_sqrt(value);
    }
    return _sqrt(value);
}
static gnu_noinline double vectorcall __libm_sse2_tan_precise(double angle) {
    if (expect(control_words_are_normal(), true)) {
        return __libm_sse2_tan(angle);
    }
    return _tan(angle);
}

static forceinline double zfabs(double value) {
    if constexpr (game_version == EoSD) {
        return _fabs(value);
    } else if constexpr (game_version >= PCB && game_version <= TD) {
        return __builtin_fabsl(value);
    } else if constexpr (game_version >= DDC) {
        return __builtin_fabs(value);
    }
}

static forceinline double zacos(double value) {
    if constexpr (game_version == EoSD) {
        return _acos(value); // Not present
    } else if constexpr (game_version >= PCB && game_version <= TD) {
        return _CIacos(value);
    } else if constexpr (game_version >= DDC) {
        return __libm_sse2_acos_precise(value);
    }
}
static forceinline double zatan(double value) {
    if constexpr (game_version == EoSD) {
        return _atan(value); // Not present
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return _CIatan(value);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return atan_asm(value);
    } else if constexpr (game_version >= DDC) {
        return __libm_sse2_atan_precise(value);
    }
}
static forceinline double zatan2(double Y, double X) {
    if constexpr (game_version == EoSD) {
        return _atan2(Y, X);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || game_version >= SA) {
        return _CIatan2(X, Y);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return atan2_asm(Y, X);
    }
}
static forceinline double zcos(double angle) {
    if constexpr (game_version == EoSD) {
        return _cos(angle);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return _CIcos(angle);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return cos_asm(angle);
    } else if constexpr (game_version >= DDC) {
        return __libm_sse2_cos_precise(angle);
    }
}
static forceinline double zfloor(double value) {
    return _floor(value);
}
static forceinline double zfmod(double X, double Y) {
    if constexpr (game_version == EoSD) {
        return _fmod(X, Y);
    } else {
        return _CIfmod(Y, X);
    }
}
static forceinline double zsin(double angle) {
    if constexpr (game_version == EoSD) {
        return _sin(angle);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return _CIsin(angle);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return sin_asm(angle);
    } else if constexpr (game_version >= DDC) {
        return __libm_sse2_sin_precise(angle);
    }
}
static forceinline double zsqrt(double value) {
    if constexpr (game_version == EoSD) {
        return _sqrt(value);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return _CIsqrt(value);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return sqrt_asm(value);
    } else if constexpr (game_version >= DDC && game_version <= WBaWC) {
        return __libm_sse2_sqrt_precise(value);
    } else if constexpr (game_version >= UM) {
        if (expect(!(value < 0.0), true)) {
            return __builtin_sqrt(value);
        } else {
            return __libm_sse2_sqrt_precise(value);
        }
    }
}
static forceinline double ztan(double angle) {
    if constexpr (game_version == EoSD || game_version == DDC) {
        return _tan(angle);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return _CItan(angle);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return tan_asm(angle);
    } else if constexpr (game_version >= DDC) {
        return __libm_sse2_tan_precise(angle);
    }
}

#if GAME_VERSION == EoSD_VER
#define float_inline forceinline
#define float_volatile /*volatile*/
#define float_convention stdcall
#elif GAME_VERSION == PCB_VER || (GAME_VERSION >= StB_VER && GAME_VERSION <= UB_VER)
#define float_inline forceinline
#define float_volatile
#define float_convention stdcall
#elif GAME_VERSION == IN_VER || GAME_VERSION == PoFV_VER
#define float_inline gnu_noinline
#define float_volatile
#define float_convention stdcall
#elif GAME_VERSION >= SA_VER && GAME_VERSION <= TD_VER
#define float_inline
#define float_volatile /*volatile*/
#define float_convention stdcall
#elif GAME_VERSION >= DDC_VER
#define float_inline
#define float_volatile
#define float_convention vectorcall
#else
#define float_inline
#define float_volatile
#define float_convention
#endif

static forceinline float float_convention zfabsf(float value) {
    if constexpr (game_version < DDC) {
        return zfabs(value);
    } else {
        return __builtin_fabsf(value);
    }
}

static inline constexpr bool game_has_dumb_float = game_version == PCB || game_version == IN || game_version == StB;
static forceinline void dumb_float(long double value) {
    float dumb;
    __asm__ volatile ("fsts %[dumb]" : asm_arg("=m", dumb) : asm_arg("t", value));
}

static float_inline float float_convention zacosf(float value) {
    float_volatile float ret = zacos(value);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention zatanf(float value) {
    float_volatile float ret = zatan(value);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention zatan2f(float Y, float X) {
    float_volatile float ret = zatan2(Y, X);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention zcosf(float angle) {
    float_volatile float ret = zcos(angle);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention zfloorf(float value) {
    float_volatile float ret = zfloor(value);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention zfmodf(float X, float Y) {
    float_volatile float ret = zfmod(X, Y);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention zsinf(float angle) {
    float_volatile float ret = zsin(angle);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention zsqrtf(float value) {
    float_volatile float ret = zsqrt(value);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}
static float_inline float float_convention ztanf(float angle) {
    float_volatile float ret = ztan(angle);
    if constexpr (game_has_dumb_float) dumb_float(ret);
    return ret;
}

#undef float_inline
#undef float_volatile
#undef float_convention

#endif