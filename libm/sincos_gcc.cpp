#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#define _USE_MATH_DEFINES 1
#include <math.h>

#include <bit>

#define _MACRO_STR(arg) #arg
#define MACRO_STR(arg) _MACRO_STR(arg)

#if __clang__
#define vectorcall __vectorcall
#define float2vec vec<float, 2>
#else

#if __FAST_MATH__
#error "Do not compile this with fast math on GCC!"
#endif

#include <immintrin.h>
#define vectorcall __attribute__((sseregparm))
#define float2vec vec<float, 4>
#endif
#define gnu_noinline __attribute__((noinline))

#define shufflevec __builtin_shufflevector
#define convertvec __builtin_convertvector

#define expect __builtin_expect
#define expect_chance __builtin_expect_with_probability
#define unpredictable(condition) __builtin_unpredictable(condition)
#define unreachable __builtin_unreachable()

#define CACHE_LINE_SIZE 64
#define cache_align alignas(CACHE_LINE_SIZE)

template <typename T, size_t count, bool is_aligned>
struct $vec_impl {
    using type __attribute__((__vector_size__(count * sizeof(T)), __aligned__(alignof(T)))) = T;
};

template <typename T, size_t count>
struct $vec_impl<T, count, true> {
    using type __attribute__((__vector_size__(count * sizeof(T)))) = T;
};

template <typename T, size_t count, bool is_aligned = false>
using vec = typename $vec_impl<T, count, is_aligned>::type;

static inline constexpr float INDEFINITE_FLOAT = std::bit_cast<float>(0xFFC00000);
static inline constexpr double INDEFINITE_DOUBLE = std::bit_cast<double>(0xFFF8000000000000);
static inline constexpr float PINFINITY_FLOAT = std::bit_cast<float>(0x7F800000);
static inline constexpr float NINFINITY_FLOAT = std::bit_cast<float>(0xFF800000);
static inline constexpr float PNAN_FLOAT = std::bit_cast<float>(0x7FC00000);
static inline constexpr float NNAN_FLOAT = std::bit_cast<float>(0xFFC00000);
static inline constexpr double PINFINITY_DOUBLE = std::bit_cast<double>(0x7FF0000000000000);
static inline constexpr double NINFINITY_DOUBLE = std::bit_cast<double>(0xFFF0000000000000);
static inline constexpr double PNAN_DOUBLE = std::bit_cast<double>(0x7FF8000000000000);
static inline constexpr double NNAN_DOUBLE = std::bit_cast<double>(0xFFF8000000000000);

static inline constexpr float minimum_negative_float_value = -0x1.FFFFFEp127f;
static inline constexpr float maximum_negative_float_value = -0x0.0p0f;
static inline constexpr float minimum_positive_float_value = 0x0.0p0f;
static inline constexpr float maximum_positive_float_value = 0x1.FFFFFEp127f;

#if __x86_64__
#define ASM_STACK_ENTER4
#define ASM_STACK_ENTER8
#define ASM_STACK_ENTER16
#define ASM_STACK_LEAVE4
#define ASM_STACK_LEAVE8
#define ASM_STACK_LEAVE16
#if __AVX__
#define ASM_SSE_WRITE_FLT "VMOVSS %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_FLD2 "VMOVSS %[y], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL "VMOVSD %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL2 "VMOVSD %[y], 8(%%RSP) \n"
#define ASM_SSE_READ_FLT "VMOVSS 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_DBL "VMOVSD 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_2DBL "VMOVAPD 8(%%RSP), %[ret] \n"
#else
#define ASM_SSE_WRITE_FLT "MOVSS %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_FLT2 "MOVSS %[y], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL "MOVSD %[x], 8(%%RSP) \n"
#define ASM_SSE_WRITE_DBL2 "MOVSD %[y], 8(%%RSP) \n"
#define ASM_SSE_READ_FLT "MOVSS 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_DBL "MOVSD 8(%%RSP), %[ret] \n"
#define ASM_SSE_READ_2DBL "MOVAPD 8(%%RSP), %[ret] \n"
#endif
#define ASM_X87_READ_FLT "FLDS 8(%%RSP) \n"
#define ASM_X87_READ_DBL "FLDL 8(%%RSP) \n"
#define ASM_X87_WRITE_FLT "FSTPS 8(%%RSP) \n"
#define ASM_X87_WRITE_DBL "FSTPL 8(%%RSP) \n"
#define ASM_X87_WRTIE_FLT2 "FSTPS 12(%%RSP) \n"
#define ASM_X87_WRITE_DBL2 "FSTPL 16(%%RSP) \n"
#else
#define ASM_STACK_ENTER4 "PUSH %%EAX \n"
#define ASM_STACK_ENTER8 "SUB $8, %%ESP \n"
#define ASM_STACK_ENTER16 "SUB $16, %%ESP \n"
#define ASM_STACK_LEAVE4 "POP %%EAX \n"
#define ASM_STACK_LEAVE8 "ADD $8, %%ESP \n"
#define ASM_STACK_LEAVE16 "ADD $16, %%ESP \n"
#if __AVX__
#define ASM_SSE_WRITE_FLT "VMOVSS %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_FLT2 "VMOVSS %[y], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL "VMOVSD %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL2 "VMOVSD %[y], (%%ESP) \n"
#define ASM_SSE_READ_FLT "VMOVSS (%%ESP), %[ret] \n"
#define ASM_SSE_READ_DBL "VMOVSD (%%ESP), %[ret] \n"
#define ASM_SSE_READ_2DBL "VMOVUPD (%%ESP), %[ret] \n"
#else
#define ASM_SSE_WRITE_FLT "MOVSS %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_FLT2 "MOVSS %[y], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL "MOVSD %[x], (%%ESP) \n"
#define ASM_SSE_WRITE_DBL2 "MOVSD %[y], (%%ESP) \n"
#define ASM_SSE_READ_FLT "MOVSS (%%ESP), %[ret] \n"
#define ASM_SSE_READ_DBL "MOVSD (%%ESP), %[ret] \n"
#define ASM_SSE_READ_2DBL "MOVUPD (%%ESP), %[ret] \n"
#endif
#define ASM_X87_READ_FLT "FLDS (%%ESP) \n"
#define ASM_X87_READ_DBL "FLDL (%%ESP) \n"
#define ASM_X87_WRITE_FLT "FSTPS (%%ESP) \n"
#define ASM_X87_WRITE_DBL "FSTPL (%%ESP) \n"
#define ASM_X87_WRTIE_FLT2 "FSTPS 4(%%ESP) \n"
#define ASM_X87_WRITE_DBL2 "FSTPL 8(%%ESP) \n"
#endif
#define ASM_SSE_READ_2FLT ASM_SSE_READ_DBL

#define ASM_FPREM1_LOOP \
    "1: \n" \
        "FPREM1 \n" \
        "FNSTSW %%AX \n" \
        "TEST $4, %%AH \n" \
        "JNZ 1b \n"

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
        "FSTP %%ST(1) \n" \
    "2: \n"

// Value is 2**63.
// Running FSIN with a values >= 2**63
// or <= -2**63 causes it to give up
// and do nothing.
static const uint32_t trig_limit = 0x5F000000;

// Value is Pi * 2**62
static const uint64_t trig_reduce[2] = { 0xC90FDAA22168C235, 0x403E };

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

inline vec<double, 2> vectorcall sincos_x87(double x) {
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

inline vec<double, 2> vectorcall sincos_sse2(double x) {
#pragma clang fp contract(off)

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
            vec<double, 2> ret = { sin_ret, cos_ret };
            return ret;
        }
        return sincos_x87(x);
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

gnu_noinline float2vec vectorcall sincosf_sse2(float x) {
#pragma clang fp contract(off)

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
            vec<double, 2> dbl_vec = { sine_x, dbl_x };
#if __clang__
            return convertvec(dbl_vec, vec<float, 2>);
#else
            return _mm_cvtpd_ps(dbl_vec);
#endif
        }
        if (expect((~int_x & 0x7F800000) != 0, true)) {
            // Nasty hack to correct an error in the last sine
            // bit caused by x87 extended precision rounding
            // differently for direct conversion to float
            // instead of intermediate double.
            if (expect((int_x & 0x7FFFFFFF) == 0x5D3DC2AF, false)) {
#if __clang__
                return { std::bit_cast<float>(0x3F2A4A27 | (~int_x & 0x80000000)), std::bit_cast<float>(0x3F3F25CD) };
#else
                vec<float, 4> ret = { std::bit_cast<float>(0x3F2A4A27 | (~int_x & 0x80000000)), std::bit_cast<float>(0x3F3F25CD) };
                return ret;
#endif
            }
#if __clang__
            return convertvec(sincos_sse2(x), vec<float, 2>);
#else
            return _mm_cvtpd_ps(sincos_sse2(x));
#endif
        }
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
#if __clang__
        return { x, x };
#else
        vec<float, 4> ret = { x, x };
        return ret;
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

#if __clang__
    return convertvec(ret, vec<float, 2>);
#else
    return _mm_cvtpd_ps(ret);
#endif
}