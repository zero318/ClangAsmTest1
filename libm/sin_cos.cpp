#include "libm_common.h"

// Required to make a single double precision sine value
// round to the correct float, but generates worse code.
// Prefer the double->float or float->float versions
// and leave this off. Effects on double->double with
// non-float arguments is unverified.

// double to double
#define CONFORM_TO_TRASH_ABI 1
// double to float
#define CONFORM_TO_TRASH_ABI2 1
// float to float
#define CONFORM_TO_TRASH_ABI3 1

// Fallback to the double->float calls instead of double->double
#define LOWER_PRECISION_DOUBLES 1

#define USE_CDECL_CONVENTION 1
#define USE_VECTORS_FOR_DOUBLES 1

#define INLINE_NON_FLOAT_ARGS 1

#if USE_CDECL_CONVENTION
#define libmcc cdecl
#else
#define libmcc vectorcall
#endif

#if USE_VECTORS_FOR_DOUBLES
#define doublev_arg(name) vec<double, 1> MACRO_CAT(name,_v)
#define doublev_param(val) (vec<double,1>){val}
#else
#define doublev_arg(name) double name
#define doublev_param(val) val
#endif

#if INLINE_NON_FLOAT_ARGS
#define double_inline forceinline
#else
#define double_inline gnu_noinline
#endif

#if __AVX__
#define CVTSS2SI(ret, flt) __asm__("VCVTSS2SI %[val], %[out]" : [out]"=r"(ret) : [val]"x"(flt))
#define CVTSD2SI(ret, dbl) __asm__("VCVTSD2SI %[val], %[out]" : [out]"=r"(ret) : [val]"x"(dbl))
#else
#define CVTSS2SI(ret, flt) __asm__("CVTSS2SI %[val], %[out]" : [out]"=r"(ret) : [val]"x"(flt))
#define CVTSD2SI(ret, dbl) __asm__("CVTSD2SI %[val], %[out]" : [out]"=r"(ret) : [val]"x"(dbl))
#endif

// .intel_syntax keeps emitting an extra
// % for the x condition and killing itself
#define ASM_TRIG_MAIN \
        "FLDS %[trig_limit] \n" \
        "FLD %%ST(1) \n" \
        "FABS \n" \
        "FUCOMIP %%ST(1), %%ST(0) \n" \
        "FFREEP %%ST(0) \n" \
        "JB 2f \n" \
        "FLDT %[trig_reduce] \n" \
        "FXCH %%ST(1) \n" \
        ASM_FPREM1_LOOP \
        "FFREE %%ST(1) \n" \
    "2: \n"

// Value is 2**63.
// Running FSIN with a values >= 2**63
// or <= -2**63 causes it to give up
// and do nothing.
static const uint32_t trig_limit = 0x5F000000;

// Value is Pi * 2**62
static const uint64_t trig_reduce[2] = { 0xC90FDAA22168C235, 0x403E };

/// ====================
/// float trigf_x87(float)
/// ====================

float vectorcall sinf_x87(float x) {
#pragma clang fp exceptions(ignore)
    float ret;
    __asm__(
        ASM_STACK_ENTER4
        ASM_SSE_WRITE_FLT
        ASM_X87_READ_FLT
        ASM_TRIG_MAIN
        "FSIN \n"
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_FLT
        ASM_STACK_LEAVE4
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
#if __x86_64__
        : "ax", "st", "st(1)", "st(2)"
#else
        : "st", "st(1)", "st(2)"
#endif
    );
    return ret;
}

float vectorcall cosf_x87(float x) {
#pragma clang fp exceptions(ignore)
    float ret;
    __asm__(
        ASM_STACK_ENTER4
        ASM_SSE_WRITE_FLT
        ASM_X87_READ_FLT
        ASM_TRIG_MAIN
        "FCOS \n"
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_FLT
        ASM_STACK_LEAVE4
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
#if __x86_64__
        : "ax", "st", "st(1)", "st(2)"
#else
        : "st", "st(1)", "st(2)"
#endif
    );
    return ret;
}

vec<float, 2> vectorcall sincosf_x87(float x) {
#pragma clang fp exceptions(ignore)
    double ret;
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_FLT
        ASM_X87_READ_FLT
        ASM_TRIG_MAIN
        "FSINCOS \n"
        ASM_X87_WRTIE_FLT2
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_2FLT
        ASM_STACK_LEAVE8
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax", "st", "st(1)", "st(2)"
    );
    return std::bit_cast<vec<float, 2>>(ret);
}

/// ====================
/// float trigf_x87(double)
/// ====================

float vectorcall sinf_x87(double x) {
#pragma clang fp exceptions(ignore)
    float ret;
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        ASM_TRIG_MAIN
        "FSIN \n"
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_FLT
        ASM_STACK_LEAVE8
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax", "st", "st(1)", "st(2)"
    );
    return ret;
}

float vectorcall cosf_x87(double x) {
#pragma clang fp exceptions(ignore)
    float ret;
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        ASM_TRIG_MAIN
        "FCOS \n"
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_FLT
        ASM_STACK_LEAVE8
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax", "st", "st(1)", "st(2)"
    );
    return ret;
}

vec<float, 2> vectorcall sincosf_x87(double x) {
#pragma clang fp exceptions(ignore)
    double ret;
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        ASM_TRIG_MAIN
        "FSINCOS \n"
        ASM_X87_WRTIE_FLT2
        ASM_X87_WRITE_FLT
        ASM_SSE_READ_2FLT
        ASM_STACK_LEAVE8
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax", "st", "st(1)", "st(2)"
    );
    return std::bit_cast<vec<float, 2>>(ret);
}

/// ====================
/// double trig_x87(double)
/// ====================

double vectorcall sin_x87(double x) {
#pragma clang fp exceptions(ignore)
    double ret;
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        ASM_TRIG_MAIN
        "FSIN \n"
        ASM_X87_WRITE_DBL
        ASM_SSE_READ_DBL
        ASM_STACK_LEAVE8
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax", "st", "st(1)", "st(2)"
    );
    return ret;
}

double vectorcall cos_x87(double x) {
#pragma clang fp exceptions(ignore)
    double ret;
    __asm__(
        ASM_STACK_ENTER8
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        ASM_TRIG_MAIN
        "FCOS \n"
        ASM_X87_WRITE_DBL
        ASM_SSE_READ_DBL
        ASM_STACK_LEAVE8
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax", "st", "st(1)", "st(2)"
    );
    return ret;
}

vec<double, 2> vectorcall sincos_x87(double x) {
#pragma clang fp exceptions(ignore)
    vec<double, 2> ret;
    __asm__(
        ASM_STACK_ENTER16
        ASM_SSE_WRITE_DBL
        ASM_X87_READ_DBL
        ASM_TRIG_MAIN
        "FSINCOS \n"
        ASM_X87_WRITE_DBL2
        ASM_X87_WRITE_DBL
        ASM_SSE_READ_2DBL
        ASM_STACK_LEAVE16
        : [ret]"=x"(ret)
        : [x]"x"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax", "st", "st(1)", "st(2)"
    );
    return ret;
}

/// ====================
/// long double trigl_x87(long double)
/// ====================

long double regcall sinl_x87(long double x) {
#pragma clang fp exceptions(ignore)
    long double ret;
    __asm__(
        ASM_TRIG_MAIN
        "FSIN \n"
        : [ret]"=t"(ret)
        : [x]"0"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax"
    );
    return ret;
}

long double regcall cosl_x87(long double x) {
#pragma clang fp exceptions(ignore)
    long double ret;
    __asm__(
        ASM_TRIG_MAIN
        "FCOS \n"
        : [ret]"=t"(ret)
        : [x]"0"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax"
    );
    return ret;
}

vec<long double, 2> regcall sincosl_x87(long double x) {
#pragma clang fp exceptions(ignore)
    long double sin_ret;
    long double cos_ret;
    __asm__(
        ASM_TRIG_MAIN
        "FSINCOS \n"
        "FXCH %%ST(1) \n"
        : [cos_ret]"=t"(sin_ret), [sin_ret]"=u"(cos_ret)
        : [x]"0"(x), [trig_reduce]"m"(trig_reduce), [trig_limit]"m"(trig_limit)
        : "ax"
    );
    return { sin_ret, cos_ret };
}

/// ====================
/// Lookup tables
/// ====================

cache_align static const vec<double, 2, true> cos_sin_ctable[128] = {
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0x0000000000000000ll) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0xBF73B92E176D6D31), std::bit_cast<double>(0x3FB917A6BC29B42C) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0xBC3E2718E0000000) },
    { std::bit_cast<double>(0xBF93AD06011469FB), std::bit_cast<double>(0x3FC8F8B83C69A60B) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0xBC626D19C0000000) },
    { std::bit_cast<double>(0xBFA60BEA939D225A), std::bit_cast<double>(0x3FD294062ED59F06) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0xBC75D28DA0000000) },
    { std::bit_cast<double>(0xBFB37CA1866B95CF), std::bit_cast<double>(0x3FD87DE2A6AEA963) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0xBC672CEDE0000000) },
    { std::bit_cast<double>(0xBFBE3A6873FA1279), std::bit_cast<double>(0x3FDE2B5D3806F63B) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x3C5E0D8920000000) },
    { std::bit_cast<double>(0xBFC592675BC57974), std::bit_cast<double>(0x3FE1C73B39AE68C8) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x3C8B25DD20000000) },
    { std::bit_cast<double>(0xBFCD0DFE53ABA2FD), std::bit_cast<double>(0x3FE44CF325091DD6) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x3C68076A20000000) },
    { std::bit_cast<double>(0x3FCA827999FCEF32), std::bit_cast<double>(0x3FE6A09E667F3BCD) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0xBC8BDD3420000000) },
    { std::bit_cast<double>(0x3FC133CC94247758), std::bit_cast<double>(0x3FE8BC806B151741) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0xBC82C5E120000000) },
    { std::bit_cast<double>(0x3FAC73B39AE68C87), std::bit_cast<double>(0x3FEA9B66290EA1A3) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0x3C39F630E0000000) },
    { std::bit_cast<double>(0xBF9D4A2C7F909C4E), std::bit_cast<double>(0x3FEC38B2F180BDB1) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0xBC76E0B180000000) },
    { std::bit_cast<double>(0xBFBE087565455A75), std::bit_cast<double>(0x3FED906BCF328D46) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0x3C7457E620000000) },
    { std::bit_cast<double>(0x3FA4A03176ACF82D), std::bit_cast<double>(0x3FEE9F4156C62DDA) }, { std::bit_cast<double>(0x3FD0000000000000), std::bit_cast<double>(0x3C8760B1E0000000) },
    { std::bit_cast<double>(0xBFAC1D1F0E5967D5), std::bit_cast<double>(0x3FEF6297CFF75CB0) }, { std::bit_cast<double>(0x3FD0000000000000), std::bit_cast<double>(0x3C75621720000000) },
    { std::bit_cast<double>(0xBF9BA1650F592F50), std::bit_cast<double>(0x3FEFD88DA3D12526) }, { std::bit_cast<double>(0x3FC0000000000000), std::bit_cast<double>(0xBC887DF640000000) },
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0x3FF0000000000000) }, { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0x3F9BA1650F592F50), std::bit_cast<double>(0x3FEFD88DA3D12526) }, { std::bit_cast<double>(0xBFC0000000000000), std::bit_cast<double>(0xBC887DF640000000) },
    { std::bit_cast<double>(0x3FAC1D1F0E5967D5), std::bit_cast<double>(0x3FEF6297CFF75CB0) }, { std::bit_cast<double>(0xBFD0000000000000), std::bit_cast<double>(0x3C75621720000000) },
    { std::bit_cast<double>(0xBFA4A03176ACF82D), std::bit_cast<double>(0x3FEE9F4156C62DDA) }, { std::bit_cast<double>(0xBFD0000000000000), std::bit_cast<double>(0x3C8760B1E0000000) },
    { std::bit_cast<double>(0x3FBE087565455A75), std::bit_cast<double>(0x3FED906BCF328D46) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0x3C7457E620000000) },
    { std::bit_cast<double>(0x3F9D4A2C7F909C4E), std::bit_cast<double>(0x3FEC38B2F180BDB1) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0xBC76E0B180000000) },
    { std::bit_cast<double>(0xBFAC73B39AE68C87), std::bit_cast<double>(0x3FEA9B66290EA1A3) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0x3C39F630E0000000) },
    { std::bit_cast<double>(0xBFC133CC94247758), std::bit_cast<double>(0x3FE8BC806B151741) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0xBC82C5E120000000) },
    { std::bit_cast<double>(0xBFCA827999FCEF32), std::bit_cast<double>(0x3FE6A09E667F3BCD) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0xBC8BDD3420000000) },
    { std::bit_cast<double>(0x3FCD0DFE53ABA2FD), std::bit_cast<double>(0x3FE44CF325091DD6) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x3C68076A20000000) },
    { std::bit_cast<double>(0x3FC592675BC57974), std::bit_cast<double>(0x3FE1C73B39AE68C8) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x3C8B25DD20000000) },
    { std::bit_cast<double>(0x3FBE3A6873FA1279), std::bit_cast<double>(0x3FDE2B5D3806F63B) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x3C5E0D8920000000) },
    { std::bit_cast<double>(0x3FB37CA1866B95CF), std::bit_cast<double>(0x3FD87DE2A6AEA963) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0xBC672CEDE0000000) },
    { std::bit_cast<double>(0x3FA60BEA939D225A), std::bit_cast<double>(0x3FD294062ED59F06) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0xBC75D28DA0000000) },
    { std::bit_cast<double>(0x3F93AD06011469FB), std::bit_cast<double>(0x3FC8F8B83C69A60B) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0xBC626D19C0000000) },
    { std::bit_cast<double>(0x3F73B92E176D6D31), std::bit_cast<double>(0x3FB917A6BC29B42C) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0xBC3E2718E0000000) },
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0x0000000000000000ll) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0x3F73B92E176D6D31), std::bit_cast<double>(0xBFB917A6BC29B42C) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x3C3E2718E0000000) },
    { std::bit_cast<double>(0x3F93AD06011469FB), std::bit_cast<double>(0xBFC8F8B83C69A60B) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x3C626D19C0000000) },
    { std::bit_cast<double>(0x3FA60BEA939D225A), std::bit_cast<double>(0xBFD294062ED59F06) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x3C75D28DA0000000) },
    { std::bit_cast<double>(0x3FB37CA1866B95CF), std::bit_cast<double>(0xBFD87DE2A6AEA963) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x3C672CEDE0000000) },
    { std::bit_cast<double>(0x3FBE3A6873FA1279), std::bit_cast<double>(0xBFDE2B5D3806F63B) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0xBC5E0D8920000000) },
    { std::bit_cast<double>(0x3FC592675BC57974), std::bit_cast<double>(0xBFE1C73B39AE68C8) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0xBC8B25DD20000000) },
    { std::bit_cast<double>(0x3FCD0DFE53ABA2FD), std::bit_cast<double>(0xBFE44CF325091DD6) }, { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0xBC68076A20000000) },
    { std::bit_cast<double>(0xBFCA827999FCEF32), std::bit_cast<double>(0xBFE6A09E667F3BCD) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0x3C8BDD3420000000) },
    { std::bit_cast<double>(0xBFC133CC94247758), std::bit_cast<double>(0xBFE8BC806B151741) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0x3C82C5E120000000) },
    { std::bit_cast<double>(0xBFAC73B39AE68C87), std::bit_cast<double>(0xBFEA9B66290EA1A3) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0xBC39F630E0000000) },
    { std::bit_cast<double>(0x3F9D4A2C7F909C4E), std::bit_cast<double>(0xBFEC38B2F180BDB1) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0x3C76E0B180000000) },
    { std::bit_cast<double>(0x3FBE087565455A75), std::bit_cast<double>(0xBFED906BCF328D46) }, { std::bit_cast<double>(0xBFE0000000000000), std::bit_cast<double>(0xBC7457E620000000) },
    { std::bit_cast<double>(0xBFA4A03176ACF82D), std::bit_cast<double>(0xBFEE9F4156C62DDA) }, { std::bit_cast<double>(0xBFD0000000000000), std::bit_cast<double>(0xBC8760B1E0000000) },
    { std::bit_cast<double>(0x3FAC1D1F0E5967D5), std::bit_cast<double>(0xBFEF6297CFF75CB0) }, { std::bit_cast<double>(0xBFD0000000000000), std::bit_cast<double>(0xBC75621720000000) },
    { std::bit_cast<double>(0x3F9BA1650F592F50), std::bit_cast<double>(0xBFEFD88DA3D12526) }, { std::bit_cast<double>(0xBFC0000000000000), std::bit_cast<double>(0x3C887DF640000000) },
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0xBFF0000000000000) }, { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0xBF9BA1650F592F50), std::bit_cast<double>(0xBFEFD88DA3D12526) }, { std::bit_cast<double>(0x3FC0000000000000), std::bit_cast<double>(0x3C887DF640000000) },
    { std::bit_cast<double>(0xBFAC1D1F0E5967D5), std::bit_cast<double>(0xBFEF6297CFF75CB0) }, { std::bit_cast<double>(0x3FD0000000000000), std::bit_cast<double>(0xBC75621720000000) },
    { std::bit_cast<double>(0x3FA4A03176ACF82D), std::bit_cast<double>(0xBFEE9F4156C62DDA) }, { std::bit_cast<double>(0x3FD0000000000000), std::bit_cast<double>(0xBC8760B1E0000000) },
    { std::bit_cast<double>(0xBFBE087565455A75), std::bit_cast<double>(0xBFED906BCF328D46) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0xBC7457E620000000) },
    { std::bit_cast<double>(0xBF9D4A2C7F909C4E), std::bit_cast<double>(0xBFEC38B2F180BDB1) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0x3C76E0B180000000) },
    { std::bit_cast<double>(0x3FAC73B39AE68C87), std::bit_cast<double>(0xBFEA9B66290EA1A3) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0xBC39F630E0000000) },
    { std::bit_cast<double>(0x3FC133CC94247758), std::bit_cast<double>(0xBFE8BC806B151741) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0x3C82C5E120000000) },
    { std::bit_cast<double>(0x3FCA827999FCEF32), std::bit_cast<double>(0xBFE6A09E667F3BCD) }, { std::bit_cast<double>(0x3FE0000000000000), std::bit_cast<double>(0x3C8BDD3420000000) },
    { std::bit_cast<double>(0xBFCD0DFE53ABA2FD), std::bit_cast<double>(0xBFE44CF325091DD6) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0xBC68076A20000000) },
    { std::bit_cast<double>(0xBFC592675BC57974), std::bit_cast<double>(0xBFE1C73B39AE68C8) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0xBC8B25DD20000000) },
    { std::bit_cast<double>(0xBFBE3A6873FA1279), std::bit_cast<double>(0xBFDE2B5D3806F63B) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0xBC5E0D8920000000) },
    { std::bit_cast<double>(0xBFB37CA1866B95CF), std::bit_cast<double>(0xBFD87DE2A6AEA963) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x3C672CEDE0000000) },
    { std::bit_cast<double>(0xBFA60BEA939D225A), std::bit_cast<double>(0xBFD294062ED59F06) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x3C75D28DA0000000) },
    { std::bit_cast<double>(0xBF93AD06011469FB), std::bit_cast<double>(0xBFC8F8B83C69A60B) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x3C626D19C0000000) },
    { std::bit_cast<double>(0xBF73B92E176D6D31), std::bit_cast<double>(0xBFB917A6BC29B42C) }, { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x3C3E2718E0000000) },
};

cache_align static const vec<double, 2, true> cosf_sinf_ctable[64] = {
    { std::bit_cast<double>(0x3FF0000000000000), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0x3FEFD88DA3D12526), std::bit_cast<double>(0x3FB917A6BC29B42C) },
    { std::bit_cast<double>(0x3FEF6297CFF75CB0), std::bit_cast<double>(0x3FC8F8B83C69A60B) },
    { std::bit_cast<double>(0x3FEE9F4156C62DDA), std::bit_cast<double>(0x3FD294062ED59F06) },
    { std::bit_cast<double>(0x3FED906BCF328D46), std::bit_cast<double>(0x3FD87DE2A6AEA963) },
    { std::bit_cast<double>(0x3FEC38B2F180BDB1), std::bit_cast<double>(0x3FDE2B5D3806F63B) },
    { std::bit_cast<double>(0x3FEA9B66290EA1A3), std::bit_cast<double>(0x3FE1C73B39AE68C8) },
    { std::bit_cast<double>(0x3FE8BC806B151741), std::bit_cast<double>(0x3FE44CF325091DD6) },
    { std::bit_cast<double>(0x3FE6A09E667F3BCD), std::bit_cast<double>(0x3FE6A09E667F3BCD) },
    { std::bit_cast<double>(0x3FE44CF325091DD6), std::bit_cast<double>(0x3FE8BC806B151741) },
    { std::bit_cast<double>(0x3FE1C73B39AE68C8), std::bit_cast<double>(0x3FEA9B66290EA1A3) },
    { std::bit_cast<double>(0x3FDE2B5D3806F63B), std::bit_cast<double>(0x3FEC38B2F180BDB1) },
    { std::bit_cast<double>(0x3FD87DE2A6AEA963), std::bit_cast<double>(0x3FED906BCF328D46) },
    { std::bit_cast<double>(0x3FD294062ED59F06), std::bit_cast<double>(0x3FEE9F4156C62DDA) },
    { std::bit_cast<double>(0x3FC8F8B83C69A60B), std::bit_cast<double>(0x3FEF6297CFF75CB0) },
    { std::bit_cast<double>(0x3FB917A6BC29B42C), std::bit_cast<double>(0x3FEFD88DA3D12526) },
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0x3FF0000000000000) },
    { std::bit_cast<double>(0xBFB917A6BC29B42C), std::bit_cast<double>(0x3FEFD88DA3D12526) },
    { std::bit_cast<double>(0xBFC8F8B83C69A60B), std::bit_cast<double>(0x3FEF6297CFF75CB0) },
    { std::bit_cast<double>(0xBFD294062ED59F06), std::bit_cast<double>(0x3FEE9F4156C62DDA) },
    { std::bit_cast<double>(0xBFD87DE2A6AEA963), std::bit_cast<double>(0x3FED906BCF328D46) },
    { std::bit_cast<double>(0xBFDE2B5D3806F63B), std::bit_cast<double>(0x3FEC38B2F180BDB1) },
    { std::bit_cast<double>(0xBFE1C73B39AE68C8), std::bit_cast<double>(0x3FEA9B66290EA1A3) },
    { std::bit_cast<double>(0xBFE44CF325091DD6), std::bit_cast<double>(0x3FE8BC806B151741) },
    { std::bit_cast<double>(0xBFE6A09E667F3BCD), std::bit_cast<double>(0x3FE6A09E667F3BCD) },
    { std::bit_cast<double>(0xBFE8BC806B151741), std::bit_cast<double>(0x3FE44CF325091DD6) },
    { std::bit_cast<double>(0xBFEA9B66290EA1A3), std::bit_cast<double>(0x3FE1C73B39AE68C8) },
    { std::bit_cast<double>(0xBFEC38B2F180BDB1), std::bit_cast<double>(0x3FDE2B5D3806F63B) },
    { std::bit_cast<double>(0xBFED906BCF328D46), std::bit_cast<double>(0x3FD87DE2A6AEA963) },
    { std::bit_cast<double>(0xBFEE9F4156C62DDA), std::bit_cast<double>(0x3FD294062ED59F06) },
    { std::bit_cast<double>(0xBFEF6297CFF75CB0), std::bit_cast<double>(0x3FC8F8B83C69A60B) },
    { std::bit_cast<double>(0xBFEFD88DA3D12526), std::bit_cast<double>(0x3FB917A6BC29B42C) },
    { std::bit_cast<double>(0xBFF0000000000000), std::bit_cast<double>(0x0000000000000000ll) },
    { std::bit_cast<double>(0xBFEFD88DA3D12526), std::bit_cast<double>(0xBFB917A6BC29B42C) },
    { std::bit_cast<double>(0xBFEF6297CFF75CB0), std::bit_cast<double>(0xBFC8F8B83C69A60B) },
    { std::bit_cast<double>(0xBFEE9F4156C62DDA), std::bit_cast<double>(0xBFD294062ED59F06) },
    { std::bit_cast<double>(0xBFED906BCF328D46), std::bit_cast<double>(0xBFD87DE2A6AEA963) },
    { std::bit_cast<double>(0xBFEC38B2F180BDB1), std::bit_cast<double>(0xBFDE2B5D3806F63B) },
    { std::bit_cast<double>(0xBFEA9B66290EA1A3), std::bit_cast<double>(0xBFE1C73B39AE68C8) },
    { std::bit_cast<double>(0xBFE8BC806B151741), std::bit_cast<double>(0xBFE44CF325091DD6) },
    { std::bit_cast<double>(0xBFE6A09E667F3BCD), std::bit_cast<double>(0xBFE6A09E667F3BCD) },
    { std::bit_cast<double>(0xBFE44CF325091DD6), std::bit_cast<double>(0xBFE8BC806B151741) },
    { std::bit_cast<double>(0xBFE1C73B39AE68C8), std::bit_cast<double>(0xBFEA9B66290EA1A3) },
    { std::bit_cast<double>(0xBFDE2B5D3806F63B), std::bit_cast<double>(0xBFEC38B2F180BDB1) },
    { std::bit_cast<double>(0xBFD87DE2A6AEA963), std::bit_cast<double>(0xBFED906BCF328D46) },
    { std::bit_cast<double>(0xBFD294062ED59F06), std::bit_cast<double>(0xBFEE9F4156C62DDA) },
    { std::bit_cast<double>(0xBFC8F8B83C69A60B), std::bit_cast<double>(0xBFEF6297CFF75CB0) },
    { std::bit_cast<double>(0xBFB917A6BC29B42C), std::bit_cast<double>(0xBFEFD88DA3D12526) },
    { std::bit_cast<double>(0x0000000000000000ll), std::bit_cast<double>(0xBFF0000000000000) },
    { std::bit_cast<double>(0x3FB917A6BC29B42C), std::bit_cast<double>(0xBFEFD88DA3D12526) },
    { std::bit_cast<double>(0x3FC8F8B83C69A60B), std::bit_cast<double>(0xBFEF6297CFF75CB0) },
    { std::bit_cast<double>(0x3FD294062ED59F06), std::bit_cast<double>(0xBFEE9F4156C62DDA) },
    { std::bit_cast<double>(0x3FD87DE2A6AEA963), std::bit_cast<double>(0xBFED906BCF328D46) },
    { std::bit_cast<double>(0x3FDE2B5D3806F63B), std::bit_cast<double>(0xBFEC38B2F180BDB1) },
    { std::bit_cast<double>(0x3FE1C73B39AE68C8), std::bit_cast<double>(0xBFEA9B66290EA1A3) },
    { std::bit_cast<double>(0x3FE44CF325091DD6), std::bit_cast<double>(0xBFE8BC806B151741) },
    { std::bit_cast<double>(0x3FE6A09E667F3BCD), std::bit_cast<double>(0xBFE6A09E667F3BCD) },
    { std::bit_cast<double>(0x3FE8BC806B151741), std::bit_cast<double>(0xBFE44CF325091DD6) },
    { std::bit_cast<double>(0x3FEA9B66290EA1A3), std::bit_cast<double>(0xBFE1C73B39AE68C8) },
    { std::bit_cast<double>(0x3FEC38B2F180BDB1), std::bit_cast<double>(0xBFDE2B5D3806F63B) },
    { std::bit_cast<double>(0x3FED906BCF328D46), std::bit_cast<double>(0xBFD87DE2A6AEA963) },
    { std::bit_cast<double>(0x3FEE9F4156C62DDA), std::bit_cast<double>(0xBFD294062ED59F06) },
    { std::bit_cast<double>(0x3FEF6297CFF75CB0), std::bit_cast<double>(0xBFC8F8B83C69A60B) },
    { std::bit_cast<double>(0x3FEFD88DA3D12526), std::bit_cast<double>(0xBFB917A6BC29B42C) },
};

alignas(16) static const vec<double, 2, true> sinf_cosf_ptable[2] = {
    { std::bit_cast<double>(0x7532E7B9676733AF), std::bit_cast<double>(0x77B921FB54444000) },
    { std::bit_cast<double>(0xE532E7B9676733AF), std::bit_cast<double>(0xE7B921FB54444000) },
};

/// ====================
/// double trig_sse2(double)
/// ====================

#if CONFORM_TO_TRASH_ABI
dllexport double_inline long double libmcc sin_sse2(doublev_arg(x)) asm("sin_sse2_DtoD");
dllexport double_inline long double libmcc sin_sse2(doublev_arg(x)) {
#else
dllexport double_inline double libmcc sin_sse2(doublev_arg(x)) asm("sin_sse2_DtoD");
dllexport double_inline double libmcc sin_sse2(doublev_arg(x)) {
#endif
#pragma clang fp contract(off)

#if USE_VECTORS_FOR_DOUBLES
    double x = x_v[0];
#endif

    vec<uint16_t, 4> tempA = std::bit_cast<vec<uint16_t, 4>>(x);
    uint32_t exp = tempA[3];

    exp = (uint16_t)((exp & 0x7FFF) - 0x3030);

    if (expect(exp > 0x10C5, false)) {
        if ((int16_t)exp <= 0x10C5) {
            if (exp >> 4 == 0xCFD) {
                // Value named ALL_ONES
                // Value is ~0.9999999999999999
                return x * std::bit_cast<double>(0x3FEFFFFFFFFFFFFF);
            }
            // Value named TWO_POW_55.
            // Value is 2**55
            double ret = std::bit_cast<double>(0x4360000000000000);
            ret *= x;
            ret -= x;
            // Value named TWO_POW_M55
            // Value is 1/2**55
            ret *= std::bit_cast<double>(0x3C80000000000000);
            return ret;
        }
#if CONFORM_TO_TRASH_ABI
        return sinl_x87(x);
#else
        return sin_x87(x);
#endif
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double A = std::bit_cast<double>(0x40245F306DC9C883);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, A);

    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    vec<double, 2> B = { A, A };

    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    vec<double, 2> C = { std::bit_cast<double>(0x3FB921FB54400000), std::bit_cast<double>(0x3D90B4611A600000) };

    B *= C;

    x -= B[0];

    // XMM2
    B = shufflevec(B, B, 1, 1);

    lookup_index &= 0x3F;

    // P_3
    // ~1.263916405497469e-22
    A *= std::bit_cast<double>(0x3B63198A2E037073);

    // XMM0
    vec<double, 2> D = { x, x };

    // XMM5
    // SC_4
    // ~2.755731922398589e-06, ~2.480158730158730e-05
    vec<double, 2> E = { std::bit_cast<double>(0x3EC71DE3A556C734), std::bit_cast<double>(0x3EFA01A01A01A01A) };

    E *= D;
    D -= B;
    E *= D;
    x -= D[0];
    x -= B[0];
    A -= x;

    // XMM4
    vec<double, 2> F = D;

    // XMM2
    vec<double, 2> G = cos_sin_ctable[lookup_index * 2];
    // XMM3
    vec<double, 2> H = cos_sin_ctable[lookup_index * 2 + 1];

    F *= G;
    G[0] += H[0];
    H[0] *= D[0];

    // XMM7
    vec<double, 2> I = shufflevec(F, G, 1, 3);
    I[0] -= G[0];
    A *= I[0];

    G[0] *= D[0];

    // XMM6
    vec<double, 2> J = shufflevec(H, H, 1, 0);
    A += J[0];

    J[0] = F[0];

    I = shufflevec(I, I, 1, 1);
    H[0] += I[0];
    I[0] = H[0];
    F[0] += H[0];

    F = shufflevec(F, H, 0, 2);

    I -= F;
    I += J;

    A += I[1];
    A += I[0];

    // XMM6
    // SC_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> K = { 1.0/120.0, 1.0/24.0 };

    D *= D;
    K *= D;
    G *= D;
    D *= D;

    // SC_3
    // ~-0.0001984126984126984, ~-0.001388888888888889
    vec<double, 2> L = { std::bit_cast<double>(0xBF2A01A01A01A01A), std::bit_cast<double>(0xBF56C16C16C16C17) };
    E += L;

    // SC_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> M = { -1.0/6.0, -0.5 };
    K += M;

    E *= D;
    K += E;
    K *= G;

    A += K[0];
    A += K[1];

    A += F[0];

    return A;
}

#if CONFORM_TO_TRASH_ABI
dllexport double_inline long double libmcc cos_sse2(doublev_arg(x)) asm("cos_sse2_DtoD");
dllexport double_inline long double libmcc cos_sse2(doublev_arg(x)) {
#else
dllexport double_inline double libmcc cos_sse2(doublev_arg(x)) asm("cos_sse2_DtoD");
dllexport double_inline double libmcc cos_sse2(doublev_arg(x)) {
#endif
#pragma clang fp contract(off)
#if USE_VECTORS_FOR_DOUBLES
    double x = x_v[0];
#endif

    vec<uint16_t, 4> tempA = std::bit_cast<vec<uint16_t, 4>>(x);
    uint32_t exp = tempA[3];

    exp = (uint16_t)((exp & 0x7FFF) - 0x3030);

    if (expect(exp > 0x10C5, false)) {
        if ((int16_t)exp <= 0x10C5) {
            return 1.0 - fabs(x);
        }
#if CONFORM_TO_TRASH_ABI
        return cosl_x87(x);
#else
        return cos_x87(x);
#endif
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double A = std::bit_cast<double>(0x40245F306DC9C883);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, A);

    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    vec<double, 2> B = { A, A };

    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    vec<double, 2> C = { std::bit_cast<double>(0x3FB921FB54400000), std::bit_cast<double>(0x3D90B4611A600000) };

    B *= C;

    x -= B[0];

    // XMM2
    B = shufflevec(B, B, 1, 1);

    lookup_index += 0x10;
    lookup_index &= 0x3F;

    // P_3
    // ~1.263916405497469e-22
    A *= std::bit_cast<double>(0x3B63198A2E037073);

    // XMM0
    vec<double, 2> D = { x, x };

    // XMM5
    // SC_4
    // ~2.755731922398589e-06, ~2.480158730158730e-05
    vec<double, 2> E = { std::bit_cast<double>(0x3EC71DE3A556C734), std::bit_cast<double>(0x3EFA01A01A01A01A) };

    E *= D;
    D -= B;
    E *= D;
    x -= D[0];
    x -= B[0];
    A -= x;

    // XMM4
    vec<double, 2> F = D;

    // XMM2
    vec<double, 2> G = cos_sin_ctable[lookup_index * 2];
    // XMM3
    vec<double, 2> H = cos_sin_ctable[lookup_index * 2 + 1];

    F *= G;
    G[0] += H[0];
    H[0] *= D[0];

    // XMM7
    vec<double, 2> I = shufflevec(F, G, 1, 3);
    I[0] -= G[0];
    A *= I[0];

    G[0] *= D[0];

    // XMM6
    vec<double, 2> J = shufflevec(H, H, 1, 0);
    A += J[0];

    J[0] = F[0];

    I = shufflevec(I, I, 1, 1);
    H[0] += I[0];
    I[0] = H[0];
    F[0] += H[0];

    F = shufflevec(F, H, 0, 2);

    I -= F;
    I += J;

    A += I[1];
    A += I[0];

    // XMM6
    // SC_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> K = { 1.0/120.0, 1.0/24.0 };

    D *= D;
    K *= D;
    G *= D;
    D *= D;

    // SC_3
    // ~-0.0001984126984126984, ~-0.001388888888888889
    vec<double, 2> L = { std::bit_cast<double>(0xBF2A01A01A01A01A), std::bit_cast<double>(0xBF56C16C16C16C17) };
    E += L;

    // SC_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> M = { -1.0/6.0, -0.5 };
    K += M;

    E *= D;
    K += E;
    K *= G;

    A += K[0];
    A += K[1];

    A += F[0];

    return A;
}

#if CONFORM_TO_TRASH_ABI
dllexport double_inline vec<long double, 2> libmcc sincos_sse2(doublev_arg(x)) asm("sincos_sse2_DtoD");
dllexport double_inline vec<long double, 2> libmcc sincos_sse2(doublev_arg(x)) {
#else
dllexport double_inline vec<double, 2> libmcc sincos_sse2(doublev_arg(x)) asm("sincos_sse2_DtoD");
dllexport double_inline vec<double, 2> libmcc sincos_sse2(doublev_arg(x)) {
#endif
#pragma clang fp contract(off)
#if USE_VECTORS_FOR_DOUBLES
    double x = x_v[0];
#endif

    vec<uint16_t, 4> tempA = std::bit_cast<vec<uint16_t, 4>>(x);
    uint32_t exp = tempA[3];

    exp = (uint16_t)((exp & 0x7FFF) - 0x3030);

    if (expect(exp > 0x10C5, false)) {
        if ((int16_t)exp <= 0x10C5) {

            double cos_ret = 1.0 - fabs(x);

            double sin_ret;
            if (exp >> 4 == 0xCFD) {
                // Value named ALL_ONES
                // Value is ~0.9999999999999999
                sin_ret = x * std::bit_cast<double>(0x3FEFFFFFFFFFFFFF);
            }
            else {
                // Value named TWO_POW_55.
                // Value is 2**55
                sin_ret = std::bit_cast<double>(0x4360000000000000);
                sin_ret *= x;
                sin_ret -= x;
                // Value named TWO_POW_M55
                // Value is 1/2**55
                sin_ret *= std::bit_cast<double>(0x3C80000000000000);
            }
            return { sin_ret, cos_ret };
        }
#if CONFORM_TO_TRASH_ABI
        return sincosl_x87(x);
#else
        return sincos_x87(x);
#endif
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double A = std::bit_cast<double>(0x40245F306DC9C883);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, A);

    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    vec<double, 2> B = { A, A };

    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    vec<double, 2> C = { std::bit_cast<double>(0x3FB921FB54400000), std::bit_cast<double>(0x3D90B4611A600000) };

    B *= C;

    x -= B[0];

    // XMM2
    B = shufflevec(B, B, 1, 1);

    int32_t sine_lookup_index = lookup_index;
    int32_t cosine_lookup_index = lookup_index + 0x10;
    sine_lookup_index &= 0x3F;
    cosine_lookup_index &= 0x3F;

    // P_3
    // ~1.263916405497469e-22
    A *= std::bit_cast<double>(0x3B63198A2E037073);

    // XMM0
    vec<double, 2> D = { x, x };

    // XMM5
    // SC_4
    // ~2.755731922398589e-06, ~2.480158730158730e-05
    vec<double, 2> E = { std::bit_cast<double>(0x3EC71DE3A556C734), std::bit_cast<double>(0x3EFA01A01A01A01A) };

    E *= D;
    D -= B;
    E *= D;
    x -= D[0];
    x -= B[0];
    A -= x;

    // XMM1
    vec<double, 2> N = { A, A };

    // XMM4
    vec<double, 2> F = D;

    // XMM2
    vec<double, 2> GS = cos_sin_ctable[sine_lookup_index * 2];
    // XMM3
    vec<double, 2> HS = cos_sin_ctable[sine_lookup_index * 2 + 1];

    vec<double, 2> GC = cos_sin_ctable[cosine_lookup_index * 2];
    vec<double, 2> HC = cos_sin_ctable[cosine_lookup_index * 2 + 1];

    vec<double, 2> FS = F * GS;
    vec<double, 2> FC = F * GC;

    vec<double, 2> GL = shufflevec(GS, GC, 0, 2);
    vec<double, 2> GH = shufflevec(GS, GC, 1, 3);
    vec<double, 2> HL = shufflevec(HS, HC, 0, 2);
    vec<double, 2> HH = shufflevec(HS, HC, 1, 3);

    GL += HL;
    HL *= D;

    // XMM7
    vec<double, 2> IL = shufflevec(FS, FC, 1, 3);
    vec<double, 2> FL = shufflevec(FS, FC, 0, 2);

    IL -= GL;
    N *= IL;

    GL *= D;

    N += HH;

    vec<double, 2> JH = HL;
    vec<double, 2> JL = FL;

    // I0 = HL, I1 = GH
    // F0 = FL, F1 = HL
    // J0 = JL, J1 = JH

    HL += GH;

    FL += HL;

    vec<double, 2> IH = GH;

    IH -= HL;
    HL -= FL;
    HL += JL;
    IH += JH;

    N += IH;
    N += HL;

    // XMM6
    // SC_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> K = { 1.0/120.0, 1.0/24.0 };

    D *= D;
    K *= D;
    GL *= D;
    GH *= D;
    D *= D;

    // SC_3
    // ~-0.0001984126984126984, ~-0.001388888888888889
    vec<double, 2> L = { std::bit_cast<double>(0xBF2A01A01A01A01A), std::bit_cast<double>(0xBF56C16C16C16C17) };
    E += L;

    // SC_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> M = { -1.0/6.0, -0.5 };
    K += M;

    E *= D;
    K += E;
    GL *= shufflevec(K, K, 0, 0);
    GH *= shufflevec(K, K, 1, 1);

    N += GL;
    N += GH;

    N += FL;

#if !CONFORM_TO_TRASH_ABI
    return N;
#else
    return convertvec(N, vec<long double, 2>);
#endif
}

/// ====================
/// float trigf_sse2(double)
/// ====================

#if CONFORM_TO_TRASH_ABI2
dllexport double_inline long double libmcc sinf_sse2(doublev_arg(x)) asm("sinf_sse2_DtoF");
dllexport double_inline long double libmcc sinf_sse2(doublev_arg(x)) {
#else
dllexport double_inline float libmcc sinf_sse2(doublev_arg(x)) asm("sinf_sse2_DtoF");
dllexport double_inline float libmcc sinf_sse2(doublev_arg(x)) {
#endif
#pragma clang fp contract(on)
#if USE_VECTORS_FOR_DOUBLES
    double x = x_v[0];
#endif

    vec<uint16_t, 4> tempA = std::bit_cast<vec<uint16_t, 4>>(x);
    uint32_t exp = tempA[3];

    exp = (uint16_t)((exp & 0x7FFF) - 0x3030);

    if (expect(exp > 0x10C5, false)) {
        if ((int16_t)exp <= 0x10C5) {
            if (exp >> 4 == 0xCFD) {
                // Value named ALL_ONES
                // Value is ~0.9999999999999999
                return x * std::bit_cast<double>(0x3FEFFFFFFFFFFFFF);
            }
            // Value named TWO_POW_55.
            // Value is 2**55
            double ret = std::bit_cast<double>(0x4360000000000000);
            ret = (ret * x) - x;
            // Value named TWO_POW_M55
            // Value is 1/2**55
            ret *= std::bit_cast<double>(0x3C80000000000000);
            return ret;
        }
#if !CONFORM_TO_TRASH_ABI2
        return sinf_x87(x);
#else
        return sinl_x87(x);
#endif
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double A = std::bit_cast<double>(0x40245F306DC9C883);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, A);

#if __SSE4_1__
    A = __builtin_roundeven(A);
#else
    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }
#endif

    vec<double, 2> B = { A, A };

    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    vec<double, 2> C = { std::bit_cast<double>(0x3FB921FB54400000), std::bit_cast<double>(0x3D90B4611A600000) };

    B *= C;

    x -= B[0];

    // XMM2
    B = shufflevec(B, B, 1, 1);

    lookup_index &= 0x3F;

    // XMM0
    vec<double, 2> D = { x, x };

    // XMM5
    // SC_4
    // ~2.755731922398589e-06, ~2.480158730158730e-05
    vec<double, 2> E = { std::bit_cast<double>(0x3EC71DE3A556C734), std::bit_cast<double>(0x3EFA01A01A01A01A) };

    E *= D;
    D -= B;

    // SC_3
    // ~-0.0001984126984126984, ~-0.001388888888888889
    vec<double, 2> L = { std::bit_cast<double>(0xBF2A01A01A01A01A), std::bit_cast<double>(0xBF56C16C16C16C17) };

    E = (E * D) + L;
    x -= D[0];
    x -= B[0];
    
    // P_3
    // ~1.263916405497469e-22
    A = (A * std::bit_cast<double>(0x3B63198A2E037073)) - x;

    // XMM4
    vec<double, 2> F = D;

    // XMM2
    vec<double, 2> G = cos_sin_ctable[lookup_index * 2];
    // XMM3
    vec<double, 2> H = cos_sin_ctable[lookup_index * 2 + 1];

    F *= G;
    G[0] += H[0];
    H[0] *= D[0];

    // XMM7
    vec<double, 2> I = shufflevec(F, G, 1, 3);
    I[0] -= G[0];
    A *= I[0];

    G[0] *= D[0];

    // XMM6
    vec<double, 2> J = shufflevec(H, H, 1, 0);
    A += J[0];

    J[0] = F[0];

    I = shufflevec(I, I, 1, 1);
    H[0] += I[0];
    I[0] = H[0];
    F[0] += H[0];

    F = shufflevec(F, H, 0, 2);

    I -= F;
    I += J;

    A += I[1];
    A += I[0];

    D *= D;

    // XMM6
    // SC_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> K = { 1.0/120.0, 1.0/24.0 };

    // SC_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> M = { -1.0/6.0, -0.5 };

    K = (K * D) + M;
    G *= D;
    D *= D;

    K = (E * D) + K;

    K *= G;

    A += K[0];
    A += K[1];

    A += F[0];

    return A;
}

#if CONFORM_TO_TRASH_ABI2
dllexport double_inline long double libmcc cosf_sse2(doublev_arg(x)) asm("cosf_sse2_DtoF");
dllexport double_inline long double libmcc cosf_sse2(doublev_arg(x)) {
#else
dllexport double_inline float libmcc cosf_sse2(doublev_arg(x)) asm("cosf_sse2_DtoF");
dllexport double_inline float libmcc cosf_sse2(doublev_arg(x)) {
#endif
#pragma clang fp contract(on)
#if USE_VECTORS_FOR_DOUBLES
    double x = x_v[0];
#endif

    vec<uint16_t, 4> tempA = std::bit_cast<vec<uint16_t, 4>>(x);
    uint32_t exp = tempA[3];

    exp = (uint16_t)((exp & 0x7FFF) - 0x3030);

    if (expect(exp > 0x10C5, false)) {
        if ((int16_t)exp <= 0x10C5) {
            return 1.0 - fabs(x);
        }
#if !CONFORM_TO_TRASH_ABI2
        return cosf_x87(x);
#else
        return cosl_x87(x);
#endif
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double A = std::bit_cast<double>(0x40245F306DC9C883);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, A);

#if __SSE4_1__
    A = __builtin_roundeven(A);
#else
    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }
#endif

    vec<double, 2> B = { A, A };

    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    vec<double, 2> C = { std::bit_cast<double>(0x3FB921FB54400000), std::bit_cast<double>(0x3D90B4611A600000) };

    B *= C;

    x -= B[0];

    // XMM2
    B = shufflevec(B, B, 1, 1);

    lookup_index += 0x10;
    lookup_index &= 0x3F;

    // XMM0
    vec<double, 2> D = { x, x };

    // XMM5
    // SC_4
    // ~2.755731922398589e-06, ~2.480158730158730e-05
    vec<double, 2> E = { std::bit_cast<double>(0x3EC71DE3A556C734), std::bit_cast<double>(0x3EFA01A01A01A01A) };

    E *= D;
    D -= B;

    // SC_3
    // ~-0.0001984126984126984, ~-0.001388888888888889
    vec<double, 2> L = { std::bit_cast<double>(0xBF2A01A01A01A01A), std::bit_cast<double>(0xBF56C16C16C16C17) };

    E = (E * D) + L;
    x -= D[0];
    x -= B[0];

    // P_3
    // ~1.263916405497469e-22
    A = (A * std::bit_cast<double>(0x3B63198A2E037073)) - x;

    // XMM4
    vec<double, 2> F = D;

    // XMM2
    vec<double, 2> G = cos_sin_ctable[lookup_index * 2];
    // XMM3
    vec<double, 2> H = cos_sin_ctable[lookup_index * 2 + 1];

    F *= G;
    G[0] += H[0];
    H[0] *= D[0];

    // XMM7
    vec<double, 2> I = shufflevec(F, G, 1, 3);
    I[0] -= G[0];
    A *= I[0];

    G[0] *= D[0];

    // XMM6
    vec<double, 2> J = shufflevec(H, H, 1, 0);
    A += J[0];

    J[0] = F[0];

    I = shufflevec(I, I, 1, 1);
    H[0] += I[0];
    I[0] = H[0];
    F[0] += H[0];

    F = shufflevec(F, H, 0, 2);

    I -= F;
    I += J;

    A += I[1];
    A += I[0];

    D *= D;

    // XMM6
    // SC_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> K = { 1.0/120.0, 1.0/24.0 };

    // SC_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> M = { -1.0/6.0, -0.5 };

    K = (K * D) + M;
    G *= D;
    D *= D;

    K = (E * D) + K;

    K *= G;

    A += K[0];
    A += K[1];

    A += F[0];

    return A;
}

#if CONFORM_TO_TRASH_ABI2
dllexport double_inline vec<long double, 2> libmcc sincosf_sse2(doublev_arg(x)) asm("sincosf_sse2_DtoF");
dllexport double_inline vec<long double, 2> libmcc sincosf_sse2(doublev_arg(x)) {
#else
dllexport double_inline vec<float, 2> libmcc sincosf_sse2(doublev_arg(x)) asm("sincosf_sse2_DtoF");
dllexport double_inline vec<float, 2> libmcc sincosf_sse2(doublev_arg(x)) {
#endif
#pragma clang fp contract(on)
#if USE_VECTORS_FOR_DOUBLES
    double x = x_v[0];
#endif

    vec<uint16_t, 4> tempA = std::bit_cast<vec<uint16_t, 4>>(x);
    uint32_t exp = tempA[3];

    exp = (uint16_t)((exp & 0x7FFF) - 0x3030);

    if (expect(exp > 0x10C5, false)) {
        if ((int16_t)exp <= 0x10C5) {

            double cos_ret = 1.0 - fabs(x);

            double sin_ret;
            if (exp >> 4 == 0xCFD) {
                // Value named ALL_ONES
                // Value is ~0.9999999999999999
                sin_ret = x * std::bit_cast<double>(0x3FEFFFFFFFFFFFFF);
            }
            else {
                // Value named TWO_POW_55.
                // Value is 2**55
                sin_ret = std::bit_cast<double>(0x4360000000000000);
                sin_ret = (sin_ret * x) - x;
                // Value named TWO_POW_M55
                // Value is 1/2**55
                sin_ret *= std::bit_cast<double>(0x3C80000000000000);
            }
#if !CONFORM_TO_TRASH_ABI2
            vec<double, 2> ret = { sin_ret, cos_ret };
            return convertvec(ret, vec<float, 2>);
#else
            vec<long double, 2> ret = { sin_ret, cos_ret };
            return ret;
#endif
        }
#if !CONFORM_TO_TRASH_ABI2
        return sincosf_x87(x);
#else
        return sincosl_x87(x);
#endif
    }

    // XMM1
    // Value named PI32INV.
    // ~10.18591635788130
    double A = std::bit_cast<double>(0x40245F306DC9C883);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSD2SI(lookup_index, A);

#if __SSE4_1__
    A = __builtin_roundeven(A);
#else
    // 0x4338000000000000
    double SHIFTER = 6755399441055744.0;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }
#endif

    vec<double, 2> B = { A, A };

    // P_1, P_2
    // ~0.09817477042088285, ~3.798187816439979e-12
    vec<double, 2> C = { std::bit_cast<double>(0x3FB921FB54400000), std::bit_cast<double>(0x3D90B4611A600000) };

    B *= C;

    x -= B[0];

    // XMM2
    B = shufflevec(B, B, 1, 1);

    int32_t sine_lookup_index = lookup_index;
    int32_t cosine_lookup_index = lookup_index + 0x10;
    sine_lookup_index &= 0x3F;
    cosine_lookup_index &= 0x3F;

    // XMM0
    vec<double, 2> D = { x, x };

    // XMM5
    // SC_4
    // ~2.755731922398589e-06, ~2.480158730158730e-05
    vec<double, 2> E = { std::bit_cast<double>(0x3EC71DE3A556C734), std::bit_cast<double>(0x3EFA01A01A01A01A) };

    E *= D;
    D -= B;

    // SC_3
    // ~-0.0001984126984126984, ~-0.001388888888888889
    vec<double, 2> L = { std::bit_cast<double>(0xBF2A01A01A01A01A), std::bit_cast<double>(0xBF56C16C16C16C17) };

    E = (E * D) + L;
    x -= D[0];
    x -= B[0];

    // P_3
    // ~1.263916405497469e-22
    A = (A * std::bit_cast<double>(0x3B63198A2E037073)) - x;

    // XMM1
    vec<double, 2> N = { A, A };

    // XMM4
    vec<double, 2> F = D;

    // XMM2
    vec<double, 2> GS = cos_sin_ctable[sine_lookup_index * 2];
    // XMM3
    vec<double, 2> HS = cos_sin_ctable[sine_lookup_index * 2 + 1];

    vec<double, 2> GC = cos_sin_ctable[cosine_lookup_index * 2];
    vec<double, 2> HC = cos_sin_ctable[cosine_lookup_index * 2 + 1];

    vec<double, 2> FS = F * GS;
    vec<double, 2> FC = F * GC;

    vec<double, 2> GL = shufflevec(GS, GC, 0, 2);
    vec<double, 2> GH = shufflevec(GS, GC, 1, 3);
    vec<double, 2> HL = shufflevec(HS, HC, 0, 2);
    vec<double, 2> HH = shufflevec(HS, HC, 1, 3);

    GL += HL;
    HL *= D;

    // XMM7
    vec<double, 2> IL = shufflevec(FS, FC, 1, 3);
    vec<double, 2> FL = shufflevec(FS, FC, 0, 2);

    IL -= GL;
    N *= IL;

    GL *= D;

    N += HH;

    vec<double, 2> JH = HL;
    vec<double, 2> JL = FL;

    // I0 = HL, I1 = GH
    // F0 = FL, F1 = HL
    // J0 = JL, J1 = JH

    HL += GH;

    FL += HL;

    vec<double, 2> IH = GH;

    IH -= HL;
    HL -= FL;
    HL += JL;
    IH += JH;

    N += IH;
    N += HL;

    D *= D;

    // XMM6
    // SC_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> K = { 1.0/120.0, 1.0/24.0 };

    // SC_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> M = { -1.0/6.0, -0.5 };
    
    K = (K * D) + M;
    GL *= D;
    GH *= D;
    D *= D;

    K = (E * D) + K;

    N = (GL * shufflevec(K, K, 0, 0)) + N;
    N = (GH * shufflevec(K, K, 1, 1)) + N;

    N += FL;

#if !CONFORM_TO_TRASH_ABI2
    return convertvec(N, vec<float, 2>);
#else
    return convertvec(N, vec<long double, 2>);
#endif
}

/// ====================
/// float trigf_sse2(float)
/// ====================

#if CONFORM_TO_TRASH_ABI3
dllexport gnu_noinline long double libmcc sinf_sse2(float x) asm("sinf_sse2_FtoF");
dllexport gnu_noinline long double libmcc sinf_sse2(float x) {
#else
dllexport gnu_noinline float libmcc sinf_sse2(float x) asm("sinf_sse2_FtoF");
dllexport gnu_noinline float libmcc sinf_sse2(float x) {
#endif
#pragma clang fp contract(off)
    __asm__ volatile ("":"+Yz"(x));

    uint32_t int_x = std::bit_cast<uint32_t>(x);

    // Magic bit manipulation :D
    uint32_t exp = (int_x & 0x7FFF0000) - 0x00800000;
    if (expect((uint16_t)(exp >> 16) > 0x457F, false)) {
        if ((int16_t)(exp >> 16) <= 0x457F) {
            // Value named ONE_M230
            // Value is ~0.9999999990686774
            float ret = x * std::bit_cast<double>(0x3FEFFFFFFF800000);
            {
#if CONFORM_TO_TRASH_ABI3
#pragma clang fp exceptions(strict)
#endif
                return ret;
            }
        }
        if (expect((~int_x & 0x7F800000) != 0, true)) {
#if !CONFORM_TO_TRASH_ABI
            // Nasty hack to correct an error in the last sine
            // bit caused by x87 extended precision rounding
            // differently for direct conversion to float
            // instead of intermediate double.
            if (expect((int_x & 0x7FFFFFFF) == 0x5D3DC2AF, false)) {
                return std::bit_cast<float>(0x3F2A4A27 | (~int_x & 0x80000000));
            }
#endif
#if !LOWER_PRECISION_DOUBLES
            return sin_sse2(doublev_param(x));
#else
            return sinf_sse2(doublev_param(x));
#endif
        }
#if !CONFORM_TO_TRASH_ABI3
        // Original code always gives -NaN here?
        // But it uses SUBSS?
        //return std::bit_cast<float>(0xFFC00000);
        {
#pragma clang fp exceptions(strict)
            // Presumably it's faster
            // to generate a NaN via subtraction
            // instead of reading uncached memory
            return x - x;
        }
#else
        return (long double)std::bit_cast<float>(0xFFC00000);
#endif
    }

    // Value named PI_32_RECIP.
    // ~10.18591
    float A = std::bit_cast<float>(0x4122F983);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSS2SI(lookup_index, A);
    double B = x;

    // 0x4B400000
    float SHIFTER = 12582912.0f;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    // Why TF is this in the original here?
    lookup_index &= 0x3F;

    // XMM3, XMM2
    // P_2,  P_1
    // Magic BS numbers.
    // The only difference between the two
    // sets is ^0x9000000000000000
    vec<double, 2> C = sinf_cosf_ptable[int_x >> 31];

    // Magic bit shift in the middle???
    vec<float, 4> D = { A, 0.0f };
    vec<uint64_t, 2> E = (vec<uint64_t, 2>)D << 29;
    vec<double, 2> F = (vec<double, 2>)E;

    F[1] = F[0];
    C *= F;

    B -= C[1];
    C[0] += B;

    B *= B;

    vec<double, 2> G = { B, B };

    // XMM5, XMM4
    // S_1,  C_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> H = { -1.0/6.0, -0.5 };

    H = (H * G) + 1.0;

    G *= G;

    // XMM7, XMM6
    // S_2,  C_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> I = { 1.0/120.0, 1.0/24.0 };

    H = (I * G) + H;
    
    vec<double, 2> J = cosf_sinf_ctable[lookup_index];
    J[0] *= C[0];

    J *= H;

    J[0] += J[1];

    float ret = J[0];
    {
#if CONFORM_TO_TRASH_ABI3
#pragma clang fp exceptions(strict)
#endif
        return ret;
    }
}

#if CONFORM_TO_TRASH_ABI3
dllexport gnu_noinline long double libmcc cosf_sse2(float x) asm("cosf_sse2_FtoF");
dllexport gnu_noinline long double libmcc cosf_sse2(float x) {
#else
dllexport gnu_noinline float libmcc cosf_sse2(float x) asm("cosf_sse2_FtoF");
dllexport gnu_noinline float libmcc cosf_sse2(float x) {
#endif
#pragma clang fp contract(off)
    __asm__ volatile ("":"+Yz"(x));

    uint32_t int_x = std::bit_cast<uint32_t>(x);

    // Magic bit manipulation :D
    uint32_t exp = (int_x & 0x7FFF0000) - 0x00800000;
    if (expect((uint16_t)(exp >> 16) > 0x457F, false)) {
        if ((int16_t)(exp >> 16) <= 0x457F) {
            double dbl_x = x;
            float ret = 1.0 - (dbl_x * dbl_x);
            {
#if CONFORM_TO_TRASH_ABI3
#pragma clang fp exceptions(strict)
#endif
                return ret;
            }
        }
        if (expect((int_x & 0x7F800000) != 0x7F800000, true)) {
#if !LOWER_PRECISION_DOUBLES
            return cos_sse2(doublev_param(x));
#else
            return cosf_sse2(doublev_param(x));
#endif
        }
#if !CONFORM_TO_TRASH_ABI3
        // Original code always gives -NaN here?
        // But it uses SUBSS?
        //return std::bit_cast<float>(0xFFC00000);
        {
#pragma clang fp exceptions(strict)
            // Presumably it's faster
            // to generate a NaN via subtraction
            // instead of reading uncached memory
            return x - x;
        }
#else
        return (long double)std::bit_cast<float>(0xFFC00000);
#endif
    }

    // Value named PI_32_RECIP.
    // ~10.18591
    float A = std::bit_cast<float>(0x4122F983);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSS2SI(lookup_index, A);
    double B = x;

    // 0x4B400000
    float SHIFTER = 12582912.0f;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    lookup_index += 0x10;
    lookup_index &= 0x3F;

    // XMM3, XMM2
    // P_2,  P_1
    // Magic BS numbers.
    // The only difference between the two
    // sets is ^0x9000000000000000
    vec<double, 2> C = sinf_cosf_ptable[int_x >> 31];

    // Magic bit shift in the middle???
    vec<float, 4> D = { A, 0.0f };
    vec<uint64_t, 2> E = (vec<uint64_t, 2>)D << 29;
    vec<double, 2> F = (vec<double, 2>)E;

    F[1] = F[0];
    C *= F;

    B -= C[1];
    C[0] += B;

    B *= B;

    vec<double, 2> G = { B, B };

    // XMM5, XMM4
    // S_1,  C_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> H = { -1.0/6.0, -0.5 };

    H = (H * G) + 1.0;

    G *= G;

    // XMM7, XMM6
    // S_2,  C_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> I = { 1.0/120.0, 1.0/24.0 };

    H = (I * G) + H;
    
    vec<double, 2> J = cosf_sinf_ctable[lookup_index];
    J[0] *= C[0];

    J *= H;

    J[0] += J[1];

    float ret = J[0];
    {
#if CONFORM_TO_TRASH_ABI3
#pragma clang fp exceptions(strict)
#endif
        return ret;
    }
}

#if CONFORM_TO_TRASH_ABI3
dllexport gnu_noinline vec<long double, 2> libmcc sincosf_sse2(float x) asm("sincosf_sse2_FtoF");
dllexport gnu_noinline vec<long double, 2> libmcc sincosf_sse2(float x) {
#else
dllexport gnu_noinline vec<float, 2> libmcc sincosf_sse2(float x) asm("sincosf_sse2_FtoF");
dllexport gnu_noinline vec<float, 2> libmcc sincosf_sse2(float x) {
#endif
#pragma clang fp contract(off)
    __asm__ volatile ("":"+Yz"(x));

    uint32_t int_x = std::bit_cast<uint32_t>(x);

    // Magic bit manipulation :D
    uint32_t exp = (int_x & 0x7FFF0000) - 0x00800000;
    if (expect((uint16_t)(exp >> 16) > 0x457F, false)) {
        if ((int16_t)(exp >> 16) <= 0x457F) {
            // Value named ONE_M230
            // Value is ~0.9999999990686774
            double dbl_x = x;
            double sine_x = dbl_x * std::bit_cast<double>(0x3FEFFFFFFF800000);
            dbl_x = 1.0 - (dbl_x * dbl_x);
#if !CONFORM_TO_TRASH_ABI3
            vec<double, 2> dbl_vec = { sine_x, dbl_x };
            return convertvec(dbl_vec, vec<float, 2>);
#else
            float ret1 = sine_x;
            float ret2 = dbl_x;
            {
#pragma clang fp exceptions(strict)
                return { ret1, ret2 };
            }
#endif
        }
        if (expect((~int_x & 0x7F800000) != 0, true)) {
#if !CONFORM_TO_TRASH_ABI
            // Nasty hack to correct an error in the last sine
            // bit caused by x87 extended precision rounding
            // differently for direct conversion to float
            // instead of intermediate double.
            if (expect((int_x & 0x7FFFFFFF) == 0x5D3DC2AF, false)) {
                return { std::bit_cast<float>(0x3F2A4A27 | (~int_x & 0x80000000)), std::bit_cast<float>(0x3F3F25CD) };
            }
#endif
#if !LOWER_PRECISION_DOUBLES
#if !CONFORM_TO_TRASH_ABI3
            return convertvec(sincos_sse2(doublev_param(x)), vec<float, 2>);
#else
            return sincos_sse2(doublev_param(x));
#endif
#else
#if !CONFORM_TO_TRASH_ABI3
            return convertvec(sincosf_sse2(doublev_param(x)), vec<float, 2>);
#else
            return sincosf_sse2(doublev_param(x));
#endif
#endif
        }
#if !CONFORM_TO_TRASH_ABI3
        // Original code always gives -NaN here?
        // But it uses SUBSS?
        //return { std::bit_cast<float>(0xFFC00000), std::bit_cast<float>(0xFFC00000) };
        {
#pragma clang fp exceptions(strict)
            // Presumably it's faster
            // to generate a NaN via subtraction
            // instead of reading uncached memory
            x -= x;
        }
        return { x, x };
#else
        return { (long double)std::bit_cast<float>(0xFFC00000), (long double)std::bit_cast<float>(0xFFC00000) };
#endif
    }

    // Value named PI_32_RECIP.
    // ~10.18591
    float A = std::bit_cast<float>(0x4122F983);
    A *= x;

    // The default rounding mode is important here!
    // And intrinsics/builtins confuse the compiler
    // about the state of the upper 3 floats being 0
    int32_t lookup_index;
    CVTSS2SI(lookup_index, A);
    double B = x;

    // 0x4B400000
    float SHIFTER = 12582912.0f;
    {
#pragma clang fp exceptions(strict)
        A += SHIFTER;
        A -= SHIFTER;
    }

    int32_t sine_lookup_index = lookup_index;
    int32_t cosine_lookup_index = lookup_index + 0x10;
    sine_lookup_index &= 0x3F;
    cosine_lookup_index &= 0x3F;

    // XMM3, XMM2
    // P_2,  P_1
    // Magic BS numbers.
    // The only difference between the two
    // sets is ^0x9000000000000000
    vec<double, 2> C = sinf_cosf_ptable[int_x >> 31];

    // Magic bit shift in the middle???
    vec<float, 4> D = { A, 0.0f };
    vec<uint64_t, 2> E = (vec<uint64_t, 2>)D << 29;
    vec<double, 2> F = (vec<double, 2>)E;

    F[1] = F[0];
    C *= F;

    B -= C[1];
    C[0] += B;

    B *= B;

    vec<double, 2> G = { B, B };

    // XMM5, XMM4
    // S_1,  C_1
    // BFC5555555555555, BFE0000000000000
    vec<double, 2> H = { -1.0/6.0, -0.5 };

    H = (H * G) + 1.0;

    G *= G;

    // XMM7, XMM6
    // S_2,  C_2
    // 3F81111111111111, 3FA5555555555555
    vec<double, 2> I = { 1.0/120.0, 1.0/24.0 };

    H = (I * G) + H;
    
    vec<double, 2> J_sin = cosf_sinf_ctable[sine_lookup_index];
    vec<double, 2> J_cos = cosf_sinf_ctable[cosine_lookup_index];

    J_sin[0] *= C[0];
    J_cos[0] *= C[0];

    J_sin *= H;
    J_cos *= H;

    vec<double, 2> ret = shufflevec(J_sin, J_cos, 0, 2);
    vec<double, 2> add = shufflevec(J_sin, J_cos, 1, 3);

    ret += add;

#if !CONFORM_TO_TRASH_ABI3
    return convertvec(ret, vec<float, 2>);
#else
    {
#pragma clang fp exceptions(strict)
        float ret1 = ret[0];
        float ret2 = ret[1];
        return { ret1, ret2 };
    }
#endif
}

/// ====================
/// Library builtins
/// ====================

extern "C" {
    extern float __vectorcall libm_sse2_sinf(float x) asm("___libm_sse2_sinf");
    extern float __vectorcall libm_sse2_cosf(float x) asm("___libm_sse2_cosf");
    extern vec<float, 2> __vectorcall libm_sse2_sincosf(float x)  asm("___libm_sse2_sincosf_");
    extern double __vectorcall libm_sse2_sin(double x) asm("___libm_sse2_sin");
    extern double __vectorcall libm_sse2_cos(double x) asm("___libm_sse2_cos");
    extern vec<double, 2> __vectorcall libm_sse2_sincos(double x) asm("___libm_sse2_sincos_");
}

#define TEST_FULL_RANGE 0

#if TEST_FULL_RANGE
#define TOTAL_TESTS 4294967296
#define TEST_FORMAT "%" PRIu64
using test_count_t = uint64_t;
#else
#define TOTAL_TESTS 4278190080
#define TEST_FORMAT "%" PRIu32
using test_count_t = uint32_t;
#endif

int main() {

// Test the correctness of individual functions
// by comparing them to the library equivalents
#define TEST_SINF 1
#define TEST_COSF 1
#define TEST_SIN 0
#define TEST_COS 0
#define TEST_SIND 0
#define TEST_COSD 0
// Test the correctness of combined functions by
// comparing them to the implementations of the
// separate functions (not library equivalents)
#define TEST_SINCOSF 1
#define TEST_SINCOS 0
#define TEST_SINCOSD 0

#if TEST_SINF
    test_count_t correct_sinf = 0;
#endif
#if TEST_COSF
    test_count_t correct_cosf = 0;
#endif
#if TEST_SIN
    test_count_t correct_sin = 0;
#endif
#if TEST_COS
    test_count_t correct_cos = 0;
#endif
#if TEST_SIND
    test_count_t correct_sind = 0;
#endif
#if TEST_COSD
    test_count_t correct_cosd = 0;
#endif
#if TEST_SINCOSF
    test_count_t correct_sincosf = 0;
#endif
#if TEST_SINCOS
    test_count_t correct_sincos = 0;
#endif
#if TEST_SINCOSD
    test_count_t correct_sincosd = 0;
#endif

    auto test_float = [&](uint32_t value) {
        float x = std::bit_cast<float>(value);

#if TEST_SINF || TEST_SINCOSF
        float test_sinf = sinf_sse2(x);
        uint32_t test_sinf_bits = std::bit_cast<uint32_t>(test_sinf);
#endif
#if TEST_SINF
        float lib_sinf = libm_sse2_sinf(x);
        uint32_t lib_sinf_bits = std::bit_cast<uint32_t>(lib_sinf);
        if (lib_sinf_bits != test_sinf_bits) __asm int3
        correct_sinf += lib_sinf_bits == test_sinf_bits;
#endif
#if TEST_COSF || TEST_SINCOSF
        float test_cosf = cosf_sse2(x);
        uint32_t test_cosf_bits = std::bit_cast<uint32_t>(test_cosf);
#endif
#if TEST_COSF
        float lib_cosf = libm_sse2_cosf(x);
        uint32_t lib_cosf_bits = std::bit_cast<uint32_t>(lib_cosf);
        if (lib_cosf_bits != test_cosf_bits) __asm int3
        correct_cosf += lib_cosf_bits == test_cosf_bits;
#endif
#if TEST_SINCOSF
#if !CONFORM_TO_TRASH_ABI3
        vec<float, 2> test_sincosf = sincosf_sse2(x);
#else
        vec<float, 2> test_sincosf = convertvec(sincosf_sse2(x), vec<float, 2>);
#endif
        uint64_t test_sincosf_bitsA = std::bit_cast<uint64_t>(test_sincosf);
        uint64_t test_sincosf_bitsB = (uint64_t)test_sinf_bits | (uint64_t)test_cosf_bits << 32;
        if (test_sincosf_bitsA != test_sincosf_bitsB) __asm int3
        correct_sincosf += test_sincosf_bitsA == test_sincosf_bitsB;
#endif
#if TEST_SIN || TEST_SINCOS
        float test_sin = sinf_sse2((double)x);
        uint32_t test_sin_bits = std::bit_cast<uint32_t>(test_sin);
#endif
#if TEST_SIN || TEST_SIND
        float lib_sin = sin(x);
        uint32_t lib_sin_bits = std::bit_cast<uint32_t>(lib_sin);
#endif
#if TEST_SIN
        if (lib_sin_bits != test_sin_bits) __asm int3
        correct_sin += lib_sin_bits == test_sin_bits;
#endif
#if TEST_SIND || TEST_SINCOSD
        float test_sind = sin_sse2(x);
        uint32_t test_sind_bits = std::bit_cast<uint32_t>(test_sind);
#endif
#if TEST_SIND
        //if (lib_sin_bits != test_sind_bits) __asm int3
        correct_sind += lib_sin_bits == test_sind_bits;
#endif
#if TEST_COS || TEST_SINCOS
        float test_cos = cosf_sse2((double)x);
        uint32_t test_cos_bits = std::bit_cast<uint32_t>(test_cos);
#endif
#if TEST_COS || TEST_COSD
        float lib_cos = cos(x);
        uint32_t lib_cos_bits = std::bit_cast<uint32_t>(lib_cos);
#endif
#if TEST_COS
        if (lib_cos_bits != test_cos_bits) __asm int3
        correct_cos += lib_cos_bits == test_cos_bits;
#endif
#if TEST_COSD || TEST_SINCOSD
        float test_cosd = cos_sse2(x);
        uint32_t test_cosd_bits = std::bit_cast<uint32_t>(test_cosd);
#endif
#if TEST_COSD
        //if (lib_cos_bits != test_cosd_bits) __asm int3
        correct_cosd += lib_cos_bits == test_cosd_bits;
#endif
#if TEST_SINCOS
#if !CONFORM_TO_TRASH_ABI2
        vec<float, 2> test_sincos = sincosf_sse2((double)x);
#else
        vec<float, 2> test_sincos = convertvec(sincosf_sse2((double)x), vec<float, 2>)
#endif
        uint64_t test_sincos_bitsA = std::bit_cast<uint64_t>(test_sincos);
        uint64_t test_sincos_bitsB = (uint64_t)test_sin_bits | (uint64_t)test_cos_bits << 32;
        //if (test_sincos_bitsA != test_sincos_bitsB) __asm int3
        correct_sincos += test_sincos_bitsA == test_sincos_bitsB;
#endif
#if TEST_SINCOSD
        vec<float, 2> test_sincosd = convertvec(sincos_sse2(x), vec<float, 2>);
        uint64_t test_sincosd_bitsA = std::bit_cast<uint64_t>(test_sincosd);
        uint64_t test_sincosd_bitsB = (uint64_t)test_sind_bits | (uint64_t)test_cosd_bits << 32;
        //if (test_sincosd_bitsA != test_sincosd_bitsB) __asm int3
        correct_sincosd += test_sincosd_bitsA == test_sincosd_bitsB;
#endif
    };

#if TEST_FULL_RANGE
    uint32_t value = 0;
    do {
        test_float(value);
    } while (++value != 0);
#else
    for (
        uint32_t value = std::bit_cast<uint32_t>(minimum_negative_float_value);
        value >= std::bit_cast<uint32_t>(maximum_negative_float_value);
        --value
    ) {
        test_float(value);
    }
    for (
        uint32_t value = std::bit_cast<uint32_t>(minimum_positive_float_value);
        value <= std::bit_cast<uint32_t>(maximum_positive_float_value);
        ++value
    ) {
        test_float(value);
    }
#endif
    
    printf(
        "Total:  " MACRO_STR(TOTAL_TESTS) "\n"
#if TEST_SINF
        "sinf :  " TEST_FORMAT "\n"
#endif
#if TEST_COSF
        "cosf :  " TEST_FORMAT "\n"
#endif
#if TEST_SIN
        "sin  :  " TEST_FORMAT "\n"
#endif
#if TEST_COS
        "cos  :  " TEST_FORMAT "\n"
#endif
#if TEST_SIND
        "sind :  " TEST_FORMAT "\n"
#endif
#if TEST_COSD
        "cosd :  " TEST_FORMAT "\n"
#endif
#if TEST_SINCOSF
        "sincosf:" TEST_FORMAT "\n"
#endif
#if TEST_SINCOS
        "sincos: " TEST_FORMAT "\n"
#endif
#if TEST_SINCOSD
        "sincosd:" TEST_FORMAT "\n"
#endif
#if TEST_SINF
        , (test_count_t)TOTAL_TESTS - correct_sinf
#endif
#if TEST_COSF
        , (test_count_t)TOTAL_TESTS - correct_cosf
#endif
#if TEST_SIN
        , (test_count_t)TOTAL_TESTS - correct_sin
#endif
#if TEST_COS
        , (test_count_t)TOTAL_TESTS - correct_cos
#endif
#if TEST_SIND
        , (test_count_t)TOTAL_TESTS - correct_sind
#endif
#if TEST_COSD
        , (test_count_t)TOTAL_TESTS - correct_cosd
#endif
#if TEST_SINCOSF
        , (test_count_t)TOTAL_TESTS - correct_sincosf
#endif
#if TEST_SINCOS
        , (test_count_t)TOTAL_TESTS - correct_sincos
#endif
#if TEST_SINCOSD
        , (test_count_t)TOTAL_TESTS - correct_sincosd
#endif
    );

    return 0;
}

// Code used to dump the sin/cos tables
/*
void dump_tables() {
    if (HMODULE handle = LoadLibraryExW(L"F:\\Touhou_Stuff_2\\disassembly_stuff\\19\\th19\\th19.exe", NULL, LOAD_LIBRARY_AS_IMAGE_RESOURCE)) {

        //vec<uint64_t, 2>* sinf_data_ptr = based_pointer<vec<uint64_t, 2>>(handle, 0x1897C0 - 2);
        vec<uint64_t, 4>* sin_data_ptr = based_pointer<vec<uint64_t, 4>>(handle, 0x189DE0 - 2);

        // v1.00a exe
        FILE* temp = fopen("F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\temp.txt", "wb");

        for (size_t i = 0; i < 64; ++i) {
            //fprintf(temp,
                //"{ std::bit_cast<double>(0x%llX), std::bit_cast<double>(0x%llX) },\n"
                //, sinf_data_ptr[i][1], sinf_data_ptr[i][0]
            //);

            fprintf(temp,
                "{ std::bit_cast<double>(0x%llX), std::bit_cast<double>(0x%llX) }, { std::bit_cast<double>(0x%llX), std::bit_cast<double>(0x%llX) },\n"
                , sin_data_ptr[i][0], sin_data_ptr[i][1], sin_data_ptr[i][3], sin_data_ptr[i][2]
            );
        }

        fclose(temp);
        FreeLibrary(handle);
    }
}
*/