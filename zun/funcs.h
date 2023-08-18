#ifndef ZUN_FUNCS_H
#define ZUN_FUNCS_H 1

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../zero/util.h"
#include "../zero/custom_intrin.h"

#undef IN

// CRT namespace: CRT crap that *doesn't* change depending on game version
namespace CRT {

extern "C" {
    extern int64_t vectorcall ftol(long double value) asm("__ftol");
    extern int64_t vectorcall ftol2(long double value) asm("__ftol2");

    extern long double vectorcall CIacos(long double value) asm("__CIacos");
    extern long double vectorcall CIasin(long double value) asm("__CIasin");
    extern long double vectorcall CIatan(long double value) asm("__CIatan");
    extern long double vectorcall CIatan2(long double X, long double Y) asm("__CIatan2"); // ARGS ARE FLIPPED
    extern long double vectorcall CIcos(long double value) asm("__CIcos");
    extern long double vectorcall CIcosh(long double value) asm("__CIcosh");
    extern long double vectorcall CIexp(long double value) asm("__CIexp");
    extern long double vectorcall CIfmod(long double Y, long double X) asm("__CIfmod"); // ARGS ARE FLIPPED
    extern long double vectorcall CIlog(long double value) asm("__CIlog");
    extern long double vectorcall CIlog10(long double value) asm("__CIlog10");
    extern long double vectorcall CIpow(long double exponent, long double base) asm("__CIpow"); // ARGS ARE FLIPPED
    extern long double vectorcall CIsin(long double value) asm("__CIsin");
    extern long double vectorcall CIsinh(long double value) asm("__CIsinh");
    extern long double vectorcall CIsqrt(long double value) asm("__CIsqrt");
    extern long double vectorcall CItan(long double value) asm("__CItan");
    extern long double vectorcall CItanh(long double value) asm("__CItanh");

    extern double cdecl acos(double value) asm64("_acos");
    extern double cdecl asin(double value) asm64("_asin");
    extern double cdecl atan(double value) asm64("_atan");
    extern double cdecl atan2(double Y, double X) asm64("_atan2");
    extern double cdecl cos(double value) asm64("_cos");
    extern double cdecl cosh(double value) asm64("_cosh");
    extern double cdecl exp(double value) asm64("_exp");
    extern double cdecl fmod(double X, double Y) asm64("_fmod");
    extern double cdecl log(double value) asm64("_log");
    extern double cdecl log10(double value) asm64("_log10");
    extern double cdecl pow(double base, double exponent) asm64("_pow");
    extern double cdecl sin(double value) asm64("_sin");
    extern double cdecl sinh(double value) asm64("_sinh");
    extern double cdecl sqrt(double value) asm64("_sqrt");
    extern double cdecl tan(double value) asm64("_tan");
    extern double cdecl tanh(double value) asm64("_tanh");

    extern double vectorcall libm_sse2_acos(double value) asm("___libm_sse2_acos");
    extern float vectorcall libm_sse2_acosf(float value) asm("___libm_sse2_acosf");
    extern double vectorcall libm_sse2_asin(double value) asm("___libm_sse2_asin");
    extern float vectorcall libm_sse2_asinf(float value) asm("___libm_sse2_asinf");
    extern double vectorcall libm_sse2_atan(double value) asm("___libm_sse2_atan");
    extern float vectorcall libm_sse2_atanf(float value) asm("___libm_sse2_atanf");
    extern double vectorcall libm_sse2_atan2(double Y, double X) asm("___libm_sse2_atan2");
    extern double vectorcall libm_sse2_cos(double value) asm("___libm_sse2_cos");
    extern float vectorcall libm_sse2_cosf(float value) asm("___libm_sse2_cosf");
    extern double vectorcall libm_sse2_exp(double value) asm("___libm_sse2_exp");
    extern float vectorcall libm_sse2_expf(float value) asm("___libm_sse2_expf");
    extern double vectorcall libm_sse2_log(double value) asm("___libm_sse2_log");
    extern float vectorcall libm_sse2_logf(float value) asm("___libm_sse2_logf");
    extern double vectorcall libm_sse2_log10(double value) asm("___libm_sse2_log10");
    extern float vectorcall libm_sse2_log10f(float value) asm("___libm_sse2_log10f");
    extern double vectorcall libm_sse2_pow(double base, double exponent) asm("___libm_sse2_pow");
    extern float vectorcall libm_sse2_powf(float base, float exponent) asm("___libm_sse2_powf");
    extern double vectorcall libm_sse2_sin(double value) asm("___libm_sse2_sin");
    extern float vectorcall libm_sse2_sinf(float value) asm("___libm_sse2_sinf");
    extern double vectorcall libm_sse2_sqrt(double value) asm("___libm_sse2_sqrt");
    extern double vectorcall libm_sse2_tan(double value) asm("___libm_sse2_tan");
    extern float vectorcall libm_sse2_tanf(float value) asm("___libm_sse2_tanf");

    extern double cdecl fabs(double value) asm64("_fabs");
    extern double cdecl floor(double value) asm64("_floor");
}

static forceinline long double acosl(long double value) {
    return CRT::CIacos(value);
}
static forceinline long double asinl(long double value) {
    return CRT::CIasin(value);
}
static forceinline long double atanl(long double value) {
    return CRT::CIatan(value);
}
static forceinline long double atan2l(long double Y, long double X) {
    return CRT::CIatan2(X, Y);
}
static forceinline long double cosl(long double value) {
    return CRT::CIcos(value);
}
static forceinline long double coshl(long double value) {
    return CRT::CIcosh(value);
}
static forceinline long double expl(long double value) {
    return CRT::CIexp(value);
}
static forceinline long double fmodl(long double X, long double Y) {
    return CRT::CIfmod(Y, X);
}
static forceinline long double logl(long double value) {
    return CRT::CIlog(value);
}
static forceinline long double log10l(long double value) {
    return CRT::CIlog10(value);
}
static forceinline long double powl(long double base, long double exponent) {
    return CRT::CIpow(exponent, base);
}
static forceinline long double sinl(long double value) {
    return CRT::CIsin(value);
}
static forceinline long double sinhl(long double value) {
    return CRT::CIsinh(value);
}
static forceinline long double sqrtl(long double value) {
    return CRT::CIsqrt(value);
}
static forceinline long double tanl(long double value) {
    return CRT::CItan(value);
}
static forceinline long double tanhl(long double value) {
    return CRT::CItanh(value);
}

static forceinline long double vectorcall rint_asm(long double value) {
    long double ret;
    __asm__ volatile ("frndint" : asm_arg("=t", ret) : asm_arg("0", value));
    return ret;
}
static forceinline long double vectorcall atan_asm(long double value) {
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
static forceinline long double vectorcall cos_asm(long double value) {
    long double cos;
    __asm__ volatile ("fcos" : asm_arg("=t", cos) : asm_arg("0", value));
    return cos;
}
static forceinline long double vectorcall sin_asm(long double value) {
    long double sin;
    __asm__ volatile ("fsin" : asm_arg("=t", sin) : asm_arg("0", value));
    return sin;
}
static forceinline vec<long double, 2> regcall sincos_asm(long double value) {
    long double cos, sin;
    __asm__ volatile ("fsincos" : asm_arg("=t", cos), asm_arg("=u", sin) : asm_arg("0", value));
    return { cos, sin };
}
static forceinline long double vectorcall sqrt_asm(long double value) {
    long double ret;
    __asm__ volatile ("fsqrt" : asm_arg("=t", ret) : asm_arg("0", value));
    return ret;
}
static forceinline long double vectorcall tan_asm(long double value) {
    long double ret, one;
    __asm__ volatile ("fptan" : asm_arg("=t", one), asm_arg("=u", ret) : asm_arg("0", value));
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

// The "precise" variants of the libm functions aren't included in the CRT .lib file
static forceinline bool control_words_are_normal() {
    if (expect((current_mxcsr<SSE_Encoding>().raw & mxcsr_mask.raw) == default_mxcsr.raw, true)) {
        if (expect((current_fcw().raw & fcw_mask.raw) == fcw_mask.raw, true)) {
            return true;
        }
    }
    return false;
}

static gnu_noinline double vectorcall libm_sse2_acos_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return CRT::libm_sse2_acos(value);
    }
    return CRT::acos(value);
}
static gnu_noinline double vectorcall libm_sse2_atan_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return CRT::libm_sse2_atan(value);
    }
    return CRT::atan(value);
}
static gnu_noinline double vectorcall libm_sse2_cos_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return CRT::libm_sse2_cos(value);
    }
    return CRT::cos(value);
}
static gnu_noinline double vectorcall libm_sse2_sin_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return CRT::libm_sse2_sin(value);
    }
    return CRT::sin(value);
}
static gnu_noinline double vectorcall libm_sse2_sqrt_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return CRT::libm_sse2_sqrt(value);
    }
    return CRT::sqrt(value);
}
static gnu_noinline double vectorcall libm_sse2_tan_precise(double value) {
    if (expect(control_words_are_normal(), true)) {
        return CRT::libm_sse2_tan(value);
    }
    return CRT::tan(value);
}

} // namespace CRT

// zCRT namespace: CRT crap that *does* change depending on game version
namespace ZCRT {

static forceinline double fabs(double value) {
    if constexpr (game_version == EoSD) {
        return CRT::fabs(value);
    } else if constexpr (game_version >= PCB && game_version <= TD) {
        return __builtin_fabsl(value);
    } else if constexpr (game_version >= DDC) {
        return __builtin_fabs(value);
    }
}

static forceinline double acos(double value) {
    if constexpr (game_version == EoSD) {
        return CRT::acos(value); // Not present
    } else if constexpr (game_version >= PCB && game_version <= TD) {
        return CRT::acosl(value);
    } else if constexpr (game_version >= DDC) {
        return CRT::libm_sse2_acos_precise(value);
    }
}
static forceinline double atan(double value) {
    if constexpr (game_version == EoSD) {
        return CRT::atan(value); // Not present
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return CRT::atanl(value);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return CRT::atan_asm(value);
    } else if constexpr (game_version >= DDC) {
        return CRT::libm_sse2_atan_precise(value);
    }
}
static forceinline double atan2(double Y, double X) {
    if constexpr (game_version == EoSD) {
        return CRT::atan2(Y, X);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || game_version >= SA) {
        return CRT::atan2l(Y, X);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return CRT::atan2_asm(Y, X);
    }
}
static forceinline double cos(double value) {
    if constexpr (game_version == EoSD) {
        return CRT::cos(value);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return CRT::cosl(value);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return CRT::cos_asm(value);
    } else if constexpr (game_version >= DDC) {
        return CRT::libm_sse2_cos_precise(value);
    }
}
static forceinline double floor(double value) {
    return CRT::floor(value);
}
static forceinline double fmod(double X, double Y) {
    if constexpr (game_version == EoSD) {
        return CRT::fmod(X, Y);
    } else {
        return CRT::fmodl(X, Y);
    }
}
static forceinline double sin(double value) {
    if constexpr (game_version == EoSD) {
        return CRT::sin(value);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return CRT::sinl(value);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return CRT::sin_asm(value);
    } else if constexpr (game_version >= DDC) {
        return CRT::libm_sse2_sin_precise(value);
    }
}
static forceinline double sqrt(double value) {
    if constexpr (game_version == EoSD) {
        return CRT::sqrt(value);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return CRT::sqrtl(value);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return CRT::sqrt_asm(value);
    } else if constexpr (game_version >= DDC && game_version <= WBaWC) {
        return CRT::libm_sse2_sqrt_precise(value);
    } else if constexpr (game_version >= UM) {
        if (expect(!(value < 0.0), true)) {
            return __builtin_sqrt(value);
        } else {
            return CRT::libm_sse2_sqrt_precise(value);
        }
    }
}
static forceinline double tan(double value) {
    if constexpr (game_version == EoSD || game_version == DDC) {
        return CRT::tan(value);
    } else if constexpr (game_version == PCB || game_version == IN || game_version == StB || (game_version >= SA && game_version <= TD)) {
        return CRT::tanl(value);
    } else if constexpr (game_version == PoFV || game_version == MoF || game_version == UB) {
        return CRT::tan_asm(value);
    } else if constexpr (game_version >= DDC) {
        return CRT::libm_sse2_tan_precise(value);
    }
}

static forceinline int32_t ftol(float value) {
    if constexpr (game_version < DDC) {
        return CRT::ftol2(value);
    } else {
        return value;
    }
}

#if GAME_VERSION == EoSD_VER
#define float_inline_state ForceInline
#define float_volatile /*volatile*/
#define float_convention stdcall
#elif GAME_VERSION == PCB_VER || (GAME_VERSION >= StB_VER && GAME_VERSION <= UB_VER)
#define float_inline_state ForceInline
#define float_volatile
#define float_convention stdcall
#elif GAME_VERSION == IN_VER || GAME_VERSION == PoFV_VER
#define float_inline_state NoInline
#define float_volatile
#define float_convention stdcall
#elif GAME_VERSION >= SA_VER && GAME_VERSION <= TD_VER
#define float_inline_state DefaultInline
#define float_volatile /*volatile*/
#define float_convention stdcall
#elif GAME_VERSION >= DDC_VER
#define float_inline_state NoInline
#define float_volatile
#define float_convention vectorcall
#else
#define float_inline_state DefaultInline
#define float_volatile
#define float_convention
#endif

static forceinline float float_convention fabsf(float value) {
    if constexpr (game_version < DDC) {
        return ZCRT::fabs(value);
    } else {
        return __builtin_fabsf(value);
    }
}

static inline constexpr bool has_dumb_float = game_version == PCB || game_version == IN || game_version == StB;
static forceinline void dumb_float(long double value) {
    float dumb;
    __asm__ volatile ("fsts %[dumb]" : asm_arg("=m", dumb) : asm_arg("t", value));
}

static float float_convention acosf(float value) {
    float_volatile float ret = ZCRT::acos(value);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention atanf(float value) {
    float_volatile float ret = ZCRT::atan(value);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention atan2f(float Y, float X) {
    float_volatile float ret = ZCRT::atan2(Y, X);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention cosf(float value) {
    float_volatile float ret = ZCRT::cos(value);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention floorf(float value) {
    float_volatile float ret = ZCRT::floor(value);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention fmodf(float X, float Y) {
    float_volatile float ret = ZCRT::fmod(X, Y);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention sinf(float value) {
    float_volatile float ret = ZCRT::sin(value);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention sqrtf(float value) {
    float_volatile float ret = ZCRT::sqrt(value);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}
static float float_convention tanf(float value) {
    float_volatile float ret = ZCRT::tan(value);
    if constexpr (has_dumb_float) ZCRT::dumb_float(ret);
    return ret;
}

} // namespace ZCRT

// ZUN namespace: function wrappers for each game
inline namespace ZUN {

static forceinline long double zacosl(long double value) {
    return CRT::acosl(value);
}
static forceinline double zacos(double value) {
    return ZCRT::acos(value);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zacosf(float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::acosf(value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::acosf(value);
    } else {
        return ZCRT::acosf(value);
    }
}

static forceinline long double zatanl(long double value) {
    return CRT::atanl(value);
}
static forceinline double zatan(double value) {
    return ZCRT::atan(value);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zatanf(float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::atanf(value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::atanf(value);
    } else {
        return ZCRT::atanf(value);
    }
}

static forceinline long double zatan2l(long double Y, long double X) {
    return CRT::atan2l(Y, X);
}
static forceinline double zatan2(double Y, double X) {
    return ZCRT::atan2(Y, X);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zatan2f(float Y, float X) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::atan2f(Y, X);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::atan2f(Y, X);
    } else {
        return ZCRT::atan2f(Y, X);
    }
}

static forceinline long double zcosl(long double value) {
    return CRT::cosl(value);
}
static forceinline double zcos(double value) {
    return ZCRT::cos(value);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zcosf(float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::cosf(value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::cosf(value);
    } else {
        return ZCRT::cosf(value);
    }
}

static forceinline long double zfloorl(long double value) {
    return __builtin_floorl(value);
}
static forceinline double zfloor(double value) {
    return ZCRT::floor(value);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zfloorf(float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::floorf(value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::floorf(value);
    } else {
        return ZCRT::floorf(value);
    }
}

static forceinline long double zfmodl(long double X, long double Y) {
    return CRT::fmodl(X, Y);
}
static forceinline double zfmod(double X, double Y) {
    return ZCRT::fmod(X, Y);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zfmodf(float X, float Y) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::fmodf(X, Y);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::fmodf(X, Y);
    } else {
        return ZCRT::fmodf(X, Y);
    }
}

static forceinline long double zsinl(long double value) {
    return CRT::sinl(value);
}
static forceinline double zsin(double value) {
    return ZCRT::sin(value);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zsinf(float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::sinf(value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::sinf(value);
    } else {
        return ZCRT::sinf(value);
    }
}

static forceinline long double zsqrtl(long double value) {
    return CRT::sqrtl(value);
}
static forceinline double zsqrt(double value) {
    return ZCRT::sqrt(value);
}
template <InlineState inline_state = float_inline_state>
static forceinline float zsqrtf(float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::sqrtf(value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::sqrtf(value);
    } else {
        return ZCRT::sqrtf(value);
    }
}

static forceinline long double ztanl(long double value) {
    return CRT::tanl(value);
}
static forceinline double ztan(double value) {
    return ZCRT::tan(value);
}
template <InlineState inline_state = float_inline_state>
static forceinline float ztanf(float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZCRT::tanf(value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZCRT::tanf(value);
    } else {
        return ZCRT::tanf(value);
    }
}


#undef float_inline_state
#undef float_volatile
#undef float_convention

static forceinline int32_t zftol(float value) {
    return ZCRT::ftol(value);
}

} // namespace ZUN

// ZUN::impl namespace: function implementations for each game
namespace ZUN::impl {

#if GAME_VERSION == EoSD_VER
#define reduce_angle_linkage            static forceinline
#define reduce_angle_convention         
#define reduce_angle_add_linkage        dllexport gnu_noinline
#define reduce_angle_add_convention     cdecl
#define reduced_angle_diff_linkage      static forceinline
#define reduced_angle_diff_convention   
#elif GAME_VERSION >= PCB && GAME_VERSION <= PoFV
#define reduce_angle_linkage            static forceinline
#define reduce_angle_convention     
#define reduce_angle_add_linkage        dllexport gnu_noinline
#define reduce_angle_add_convention     stdcall
#define reduced_angle_diff_linkage      static forceinline
#define reduced_angle_diff_convention   
#elif GAME_VERSION == StB
#define reduce_angle_linkage            dllexport gnu_noinline
#define reduce_angle_convention         stdcall
#define reduce_angle_add_linkage        dllexport gnu_noinline
#define reduce_angle_add_convention     stdcall
#define reduced_angle_diff_linkage      static forceinline
#define reduced_angle_diff_convention   
#elif GAME_VERSION == MoF
#define reduce_angle_linkage            dllexport
#define reduce_angle_convention         stdcall
#define reduce_angle_add_linkage        dllexport
#define reduce_angle_add_convention     stdcall
#define reduced_angle_diff_linkage      dllexport
#define reduced_angle_diff_convention   stdcall
#else
#define reduce_angle_linkage            static
#define reduce_angle_convention         
#define reduce_angle_add_linkage        static
#define reduce_angle_add_convention     
#define reduced_angle_diff_linkage      static
#define reduced_angle_diff_convention   
#endif

static inline constexpr int32_t reduce_angle_loop_count = game_version < StB ? 16 : 32;

extern "C" {
    // EoSD: 0x41E850
    // PCB: 0x431930
    // IN: 0x43EDB0
    // PoFV: 0x42AED0
    // StB: 0x41B500
    // MoF: 0x44BC10
    reduce_angle_add_linkage float reduce_angle_add_convention reduce_angle_add(float angle, float value) {
        int32_t counter = 0;
        angle += value;
        while (angle > PI_f) {
            angle -= TWO_PI_f;
            if (counter++ > reduce_angle_loop_count) break;
        }
        while (-PI_f > angle) {
            angle += TWO_PI_f;
            if (counter++ > reduce_angle_loop_count) break;
        }
        return angle;
    }

    // StB: 0x41B580
    // MoF: 0x44BC70
    reduce_angle_linkage float reduce_angle_convention reduce_angle(float angle) {
        if constexpr (game_version < StB) {
            clang_forceinline return reduce_angle_add(angle, 0.0f);
        } else {
            int32_t counter = 0;
            while (angle > PI_f) {
                angle -= TWO_PI_f;
                if (counter++ > reduce_angle_loop_count) break;
            }
            while (-PI_f > angle) {
                angle += TWO_PI_f;
                if (counter++ > reduce_angle_loop_count) break;
            }
            return angle;
        }
    }

    // MoF: 0x408660, 0x428CE0
    reduced_angle_diff_linkage float reduced_angle_diff_convention reduced_angle_diff(float angle, float value) {
        float diff = angle - value;
        if (diff > PI_f) {
            return angle - (value + TWO_PI_f);
        } else if (value - angle > PI_f) {
            return angle - (value - TWO_PI_f);
        } else {
            return diff;
        }
    }

} // extern "C"

} // namespace ZUN::impl

#undef reduce_angle_add_linkage
#undef reduce_angle_add_convention
#undef reduce_angle_linkage
#undef reduce_angle_convention
#undef reduced_angle_diff_linkage
#undef reduced_angle_diff_convention

inline namespace ZUN {

template <InlineState inline_state = DefaultInline>
static forceinline float reduce_angle_add(float angle, float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZUN::impl::reduce_angle_add(angle, value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZUN::impl::reduce_angle_add(angle, value);
    } else {
        return ZUN::impl::reduce_angle_add(angle, value);
    }
}

template <InlineState inline_state = DefaultInline>
static forceinline float reduce_angle(float angle) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZUN::impl::reduce_angle(angle);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZUN::impl::reduce_angle(angle);
    } else {
        return ZUN::impl::reduce_angle(angle);
    }
}

template <InlineState inline_state = DefaultInline>
static forceinline float reduced_angle_diff(float angle, float value) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return ZUN::impl::reduced_angle_diff(angle, value);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return ZUN::impl::reduced_angle_diff(angle, value);
    } else {
        return ZUN::impl::reduced_angle_diff(angle, value);
    }
}

} // inline namespace ZUN

#endif