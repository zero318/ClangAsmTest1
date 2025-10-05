#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <utility>
#include <tuple>

#include <Windows.h>

#include "zero/util.h"

#define TH_VER FW
#define VEC_DEFINED 1
#define PackUInt32 PackUInt32_
#define PackD3DCOLOR PackD3DCOLOR_

#include "zero/th_common.h"

#undef PackUInt32

struct Float4 : Float3 {
    float w;

    inline constexpr Float4() {};
    inline constexpr Float4(float X, float Y, float Z, float W = 0.0f) : Float3(X, Y, Z), w(W) {}
};
ValidateStructSize(0x10, Float4);

#define MEM_BARRIER __asm__ volatile ("":::"memory")

#define EXPORT_TESTS 0

#if EXPORT_TESTS
#define test_export test_export
#else
#define test_export static
#endif

#define TEST_USE_ASM 1

#if TEST_USE_ASM
#define test_inline inline
#else
#define test_inline gnu_noinline
#endif

#define TEST_USE_FLOAT4 1

#if TEST_USE_FLOAT4
using FloatT = Float4;
#define element_size 16
#else
using FloatT = Float3;
#define element_size 12
#endif

/*
Variant A1: Scalar
Variant A2: Scalar [load then store]
Variant B1: Split vector PS (low2 + high)
Variant B2: Split vector PS (low2 + high) [load then store]
Variant B3: Split vector SD (low2 + high)
Variant B4: Split vector SD (low2 + high) [load then store]
Variant C1: Split vector PS (low + high2)
Variant C2: Split vector PS (low + high2) [load then store]
Variant C3: Split vector SD (low + high2)
Variant C4: Split vector SD (low + high2) [load then store]
Variant D1: Shuffle vector PS (low2 + high)
Variant D2: Shuffle vector SD (low2 + high)
Variant E1: Shuffle vector PS (low + high2)
Variant F1: Half vector PS (low2 + high)
Variant F2: Half vector SD (low2 + high)
Variant G1: Half vector PS (low + high2)
Variant H1: Direct vector PS (low + high2)
Variant I1: Masked vector MMA
Variant I2: Masked vector MA
*/

#define for_each_vec_type_no_avx512(...) \
__VA_ARGS__(A1) \
__VA_ARGS__(A2) \
__VA_ARGS__(B1) \
__VA_ARGS__(B2) \
__VA_ARGS__(B3) \
__VA_ARGS__(B4) \
__VA_ARGS__(C1) \
__VA_ARGS__(C2) \
__VA_ARGS__(C3) \
__VA_ARGS__(C4) \
__VA_ARGS__(D1) \
__VA_ARGS__(D2) \
__VA_ARGS__(E1) \
__VA_ARGS__(F1) \
__VA_ARGS__(F2) \
__VA_ARGS__(G1) \
__VA_ARGS__(H1)

#define for_each_vec_type_avx512(...) \
__VA_ARGS__(I1) \
__VA_ARGS__(I2)

#define for_each_vec_type(...) \
for_each_vec_type_no_avx512(__VA_ARGS__) \
for_each_vec_type_avx512(__VA_ARGS__)

#define for_each_vec_type_control(...) \
__VA_ARGS__(V1) \
__VA_ARGS__(V2) \
__VA_ARGS__(V4)

#define PACK_RETURNS \
do { \
    auto temp = std::tuple(i, rhs, lhs, out); \
    return *(uint128_t*)&temp; \
} while (0)

// Variant A1: Scalar
test_export test_inline uint128_t regcall test_vec3_A1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "ADDSS %[rhsX], %%XMM0 \n"
        "MOVSS %%XMM0, %[outX] \n"
        "MOVSS %[lhsY], %%XMM0 \n"
        "ADDSS %[rhsY], %%XMM0 \n"
        "MOVSS %%XMM0, %[outY] \n"
        "MOVSS %[lhsZ], %%XMM0 \n"
        "ADDSS %[rhsZ], %%XMM0 \n"
        "MOVSS %%XMM0, %[outZ] \n"
        : [outX]"=m"(out->x), [outY]"=m"(out->y), [outZ]"=m"(out->z)
        : [lhsX]"m"(lhs->x), [lhsY]"m"(lhs->y), [lhsZ]"m"(lhs->z),
          [rhsX]"m"(rhs->x), [rhsY]"m"(rhs->y), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0")
    );
#else
    MEM_BARRIER;
    out->x = lhs->x + rhs->x;
    MEM_BARRIER;
    out->y = lhs->y + rhs->y;
    MEM_BARRIER;
    out->z = lhs->z + rhs->z;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant A2: Scalar [load then store]
test_export test_inline uint128_t regcall test_vec3_A2(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "MOVSS %[lhsY], %%XMM1 \n"
        "MOVSS %[lhsZ], %%XMM2 \n"
        "ADDSS %[rhsX], %%XMM0 \n"
        "ADDSS %[rhsY], %%XMM1 \n"
        "ADDSS %[rhsZ], %%XMM2 \n"
        "MOVSS %%XMM0, %[outX] \n"
        "MOVSS %%XMM1, %[outY] \n"
        "MOVSS %%XMM2, %[outZ] \n"
        : [outX]"=m"(out->x), [outY]"=m"(out->y), [outZ]"=m"(out->z)
        : [lhsX]"m"(lhs->x), [lhsY]"m"(lhs->y), [lhsZ]"m"(lhs->z),
          [rhsX]"m"(rhs->x), [rhsY]"m"(rhs->y), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    float x = lhs->x;
    MEM_BARRIER;
    float y = lhs->y;
    MEM_BARRIER;
    float z = lhs->z;
    MEM_BARRIER;
    x += rhs->x;
    MEM_BARRIER;
    y += rhs->y;
    MEM_BARRIER;
    z += rhs->z;
    MEM_BARRIER;
    out->x = x;
    MEM_BARRIER;
    out->y = y;
    MEM_BARRIER;
    out->z = z;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant B1: Split vector PS (low2 + high)
test_export test_inline uint128_t regcall test_vec3_B1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSD %[rhsXY], %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVLPS %%XMM1, %[outXY] \n"
        "MOVSS %[lhsZ], %%XMM0 \n"
        "ADDSS %[rhsZ], %%XMM0 \n"
        "MOVSS %%XMM0, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1")
    );
#else
    MEM_BARRIER;
    *(vec<float, 2>*)out = *(vec<float, 2>*)lhs + *(vec<float, 2>*)rhs;
    MEM_BARRIER;
    out->z = lhs->z + rhs->z;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant B2: Split vector PS (low2 + high) [load then store]
test_export test_inline uint128_t regcall test_vec3_B2(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSS %[lhsZ], %%XMM1 \n"
        "MOVSD %[rhsXY], %%XMM2 \n"
        "ADDPS %%XMM0, %%XMM2 \n"
        "ADDSS %[rhsZ], %%XMM1 \n"
        "MOVLPS %%XMM2, %[outXY] \n"
        "MOVSS %%XMM1, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 2> low = *(vec<float, 2>*)lhs;
    MEM_BARRIER;
    float z = lhs->z;
    MEM_BARRIER;
    low += *(vec<float, 2>*)rhs;
    MEM_BARRIER;
    z += rhs->z;
    MEM_BARRIER;
    *(vec<float, 2>*)out = low;
    MEM_BARRIER;
    out->z = z;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant B3: Split vector SD (low2 + high)
test_export test_inline uint128_t regcall test_vec3_B3(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSD %[rhsXY], %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSD %%XMM1, %[outXY] \n"
        "MOVSS %[lhsZ], %%XMM0 \n"
        "ADDSS %[rhsZ], %%XMM0 \n"
        "MOVSS %%XMM0, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1")
    );
#else
    MEM_BARRIER;
    *(double*)out = bitcast<double>(*(vec<float, 2>*)lhs + *(vec<float, 2>*)rhs);
    MEM_BARRIER;
    out->z = lhs->z + rhs->z;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant B4: Split vector SD (low2 + high) [load then store]
test_export test_inline uint128_t regcall test_vec3_B4(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSS %[lhsZ], %%XMM1 \n"
        "MOVSD %[rhsXY], %%XMM2 \n"
        "ADDPS %%XMM0, %%XMM2 \n"
        "ADDSS %[rhsZ], %%XMM1 \n"
        "MOVSD %%XMM2, %[outXY] \n"
        "MOVSS %%XMM1, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 2> low = *(vec<float, 2>*)lhs;
    MEM_BARRIER;
    float z = lhs->z;
    MEM_BARRIER;
    low += *(vec<float, 2>*)rhs;
    MEM_BARRIER;
    z += rhs->z;
    MEM_BARRIER;
    *(double*)out = bitcast<double>(low);
    MEM_BARRIER;
    out->z = z;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant C1: Split vector PS (low + high2)
test_export test_inline uint128_t regcall test_vec3_C1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "ADDSS %[rhsX], %%XMM0 \n"
        "MOVSS %%XMM0, %[outX] \n"
        "MOVSD %[lhsYZ], %%XMM0 \n"
        "MOVSD %[rhsYZ], %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVLPS %%XMM1, %[outYZ] \n"
        : [outX]"=m"(out->x), [outYZ]"=m"(*(double*)&out->y)
        : [lhsX]"m"(lhs->x), [lhsYZ]"m"(*(double*)&lhs->y),
          [rhsX]"m"(rhs->x), [rhsYZ]"m"(*(double*)&rhs->y)
        : clobber_list("xmm0", "xmm1")
    );
#else
    MEM_BARRIER;
    out->x = lhs->x + rhs->x;
    MEM_BARRIER;
    *(vec<float, 2>*)&out->y = *(vec<float, 2>*)&lhs->y + *(vec<float, 2>*)&rhs->y;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant C2: Split vector PS (low + high2) [load then store]
test_export test_inline uint128_t regcall test_vec3_C2(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "MOVSD %[lhsYZ], %%XMM1 \n"
        "ADDSS %[rhsX], %%XMM0 \n"
        "MOVSD %[rhsYZ], %%XMM2 \n"
        "ADDPS %%XMM1, %%XMM2 \n"
        "MOVSS %%XMM0, %[outX] \n"
        "MOVLPS %%XMM2, %[outYZ] \n"
        : [outX]"=m"(out->x), [outYZ]"=m"(*(double*)&out->y)
        : [lhsX]"m"(lhs->x), [lhsYZ]"m"(*(double*)&lhs->y),
          [rhsX]"m"(rhs->x), [rhsYZ]"m"(*(double*)&rhs->y)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    float x = lhs->x;
    MEM_BARRIER;
    vec<float, 2> high = *(vec<float, 2>*)&lhs->y;
    MEM_BARRIER;
    x += rhs->x;
    MEM_BARRIER;
    high += *(vec<float, 2>*)&rhs->y;
    MEM_BARRIER;
    out->x = x;
    MEM_BARRIER;
    *(vec<float, 2>*)&out->y = high;
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant C3: Split vector SD (low + high2)
test_export test_inline uint128_t regcall test_vec3_C3(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "ADDSS %[rhsX], %%XMM0 \n"
        "MOVSS %%XMM0, %[outX] \n"
        "MOVSD %[lhsYZ], %%XMM0 \n"
        "MOVSD %[rhsYZ], %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSD %%XMM1, %[outYZ] \n"
        : [outX]"=m"(out->x), [outYZ]"=m"(*(double*)&out->y)
        : [lhsX]"m"(lhs->x), [lhsYZ]"m"(*(double*)&lhs->y),
          [rhsX]"m"(rhs->x), [rhsYZ]"m"(*(double*)&rhs->y)
        : clobber_list("xmm0", "xmm1")
    );
#else
    MEM_BARRIER;
    out->x = lhs->x + rhs->x;
    MEM_BARRIER;
    *(double*)&out->y = bitcast<double>(*(vec<float, 2>*)&lhs->y + *(vec<float, 2>*)&rhs->y);
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant C4: Split vector SD (low + high2) [load then store]
test_export test_inline uint128_t regcall test_vec3_C4(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "MOVSD %[lhsYZ], %%XMM1 \n"
        "ADDSS %[rhsX], %%XMM0 \n"
        "MOVSD %[rhsYZ], %%XMM2 \n"
        "ADDPS %%XMM1, %%XMM2 \n"
        "MOVSS %%XMM0, %[outX] \n"
        "MOVSD %%XMM2, %[outYZ] \n"
        : [outX]"=m"(out->x), [outYZ]"=m"(*(double*)&out->y)
        : [lhsX]"m"(lhs->x), [lhsYZ]"m"(*(double*)&lhs->y),
          [rhsX]"m"(rhs->x), [rhsYZ]"m"(*(double*)&rhs->y)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    float x = lhs->x;
    MEM_BARRIER;
    vec<float, 2> high = *(vec<float, 2>*)&lhs->y;
    MEM_BARRIER;
    x += rhs->x;
    MEM_BARRIER;
    high += *(vec<float, 2>*)&rhs->y;
    MEM_BARRIER;
    out->x = x;
    MEM_BARRIER;
    *(double*)&out->y = bitcast<double>(high);
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant D1: Shuffle vector PS (low2 + high)
test_export test_inline uint128_t regcall test_vec3_D1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSS %[lhsZ], %%XMM1 \n"
        "SHUFPS $0x44, %%XMM1, %%XMM0 \n"
        "MOVSD %[rhsXY], %%XMM1 \n"
        "MOVSS %[rhsZ], %%XMM2 \n"
        "SHUFPS $0x44, %%XMM2, %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVLPS %%XMM1, %[outXY] \n"
        "MOVHLPS %%XMM1, %%XMM1 \n"
        "MOVSS %%XMM1, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 4> leftA = { lhs->x, lhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftB = { lhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftC = shufflevec(leftA, leftB, 0, 1, 4, 5);
    MEM_BARRIER;
    vec<float, 4> rightA = { rhs->x, rhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightB = { rhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightC = shufflevec(rightA, rightB, 0, 1, 4, 5);
    MEM_BARRIER;
    leftC += rightC;
    MEM_BARRIER;
    out->x = leftC[0];
    out->y = leftC[1];
    MEM_BARRIER;
    out->z = leftC[2];
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant D2: Shuffle vector SD (low2 + high)
test_export test_inline uint128_t regcall test_vec3_D2(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSS %[lhsZ], %%XMM1 \n"
        "SHUFPS $0x44, %%XMM1, %%XMM0 \n"
        "MOVSD %[rhsXY], %%XMM1 \n"
        "MOVSS %[rhsZ], %%XMM2 \n"
        "SHUFPS $0x44, %%XMM2, %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSD %%XMM1, %[outXY] \n"
        "MOVHLPS %%XMM1, %%XMM1 \n"
        "MOVSS %%XMM1, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 4> leftA = { lhs->x, lhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftB = { lhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftC = shufflevec(leftA, leftB, 0, 1, 4, 5);
    MEM_BARRIER;
    vec<float, 4> rightA = { rhs->x, rhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightB = { rhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightC = shufflevec(rightA, rightB, 0, 1, 4, 5);
    MEM_BARRIER;
    leftC += rightC;
    MEM_BARRIER;
    *(double*)&out->x = ((vec<double, 2>)leftC)[0];
    MEM_BARRIER;
    out->z = leftC[2];
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant E1: Shuffle vector PS (low + high2)
test_export test_inline uint128_t regcall test_vec3_E1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "MOVSD %[lhsYZ], %%XMM1 \n"
        "SHUFPS $0x44, %%XMM1, %%XMM0 \n"
        "MOVSS %[rhsX], %%XMM1 \n"
        "MOVSD %[rhsYZ], %%XMM2 \n"
        "SHUFPS $0x44, %%XMM2, %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSS %%XMM1, %[outX] \n"
        "MOVHPS %%XMM1, %[outYZ] \n"
        : [outX]"=m"(out->x), [outYZ]"=m"(*(double*)&out->y)
        : [lhsX]"m"(lhs->x), [lhsYZ]"m"(*(double*)&lhs->y),
          [rhsX]"m"(rhs->x), [rhsYZ]"m"(*(double*)&rhs->y)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 4> leftA = { lhs->x, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftB = { lhs->y, lhs->z, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftC = shufflevec(leftA, leftB, 0, 1, 4, 5);
    MEM_BARRIER;
    vec<float, 4> rightA = { rhs->x, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightB = { rhs->y, rhs->z, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightC = shufflevec(rightA, rightB, 0, 1, 4, 5);
    MEM_BARRIER;
    leftC += rightC;
    MEM_BARRIER;
    out->x = leftC[0];
    MEM_BARRIER;
    out->y = leftC[2];
    out->z = leftC[3];
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant F1: Half vector PS (low2 + high)
test_export test_inline uint128_t regcall test_vec3_F1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSS %[lhsZ], %%XMM1 \n"
        "MOVLHPS %%XMM1, %%XMM0 \n"
        "MOVSD %[rhsXY], %%XMM1 \n"
        "MOVSS %[rhsZ], %%XMM2 \n"
        "MOVLHPS %%XMM2, %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVLPS %%XMM1, %[outXY] \n"
        "MOVHLPS %%XMM1, %%XMM1 \n"
        "MOVSS %%XMM1, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 4> leftA = { lhs->x, lhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftB = { lhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftC = shufflevec(leftA, leftB, 0, 1, 4, 5);
    MEM_BARRIER;
    vec<float, 4> rightA = { rhs->x, rhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightB = { rhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightC = shufflevec(rightA, rightB, 0, 1, 4, 5);
    MEM_BARRIER;
    leftC += rightC;
    MEM_BARRIER;
    out->x = leftC[0];
    out->y = leftC[1];
    MEM_BARRIER;
    out->z = leftC[2];
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant F2: Half vector SD (low2 + high)
test_export test_inline uint128_t regcall test_vec3_F2(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhsXY], %%XMM0 \n"
        "MOVSS %[lhsZ], %%XMM1 \n"
        "MOVLHPS %%XMM1, %%XMM0 \n"
        "MOVSD %[rhsXY], %%XMM1 \n"
        "MOVSS %[rhsZ], %%XMM2 \n"
        "MOVLHPS %%XMM2, %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSD %%XMM1, %[outXY] \n"
        "MOVHLPS %%XMM1, %%XMM1 \n"
        "MOVSS %%XMM1, %[outZ] \n"
        : [outXY]"=m"(*(double*)&out->x), [outZ]"=m"(out->z)
        : [lhsXY]"m"(*(double*)&lhs->x), [lhsZ]"m"(lhs->z),
          [rhsXY]"m"(*(double*)&rhs->x), [rhsZ]"m"(rhs->z)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 4> leftA = { lhs->x, lhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftB = { lhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftC = shufflevec(leftA, leftB, 0, 1, 4, 5);
    MEM_BARRIER;
    vec<float, 4> rightA = { rhs->x, rhs->y, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightB = { rhs->z, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightC = shufflevec(rightA, rightB, 0, 1, 4, 5);
    MEM_BARRIER;
    leftC += rightC;
    MEM_BARRIER;
    *(double*)&out->x = ((vec<double, 2>)leftC)[0];
    MEM_BARRIER;
    out->z = leftC[2];
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant G1: Half vector PS (low + high2)
test_export test_inline uint128_t regcall test_vec3_G1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "MOVSD %[lhsYZ], %%XMM1 \n"
        "MOVLHPS %%XMM1, %%XMM0 \n"
        "MOVSS %[rhsX], %%XMM1 \n"
        "MOVSD %[rhsYZ], %%XMM2 \n"
        "MOVLHPS %%XMM2, %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSS %%XMM1, %[outX] \n"
        "MOVHPS %%XMM1, %[outYZ] \n"
        : [outX]"=m"(out->x), [outYZ]"=m"(*(double*)&out->y)
        : [lhsX]"m"(lhs->x), [lhsYZ]"m"(*(double*)&lhs->y),
          [rhsX]"m"(rhs->x), [rhsYZ]"m"(*(double*)&rhs->y)
        : clobber_list("xmm0", "xmm1", "xmm2")
    );
#else
    MEM_BARRIER;
    vec<float, 4> leftA = { lhs->x, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftB = { lhs->y, lhs->z, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftC = shufflevec(leftA, leftB, 0, 1, 4, 5);
    MEM_BARRIER;
    vec<float, 4> rightA = { rhs->x, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightB = { rhs->y, rhs->z, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightC = shufflevec(rightA, rightB, 0, 1, 4, 5);
    MEM_BARRIER;
    leftC += rightC;
    MEM_BARRIER;
    out->x = leftC[0];
    MEM_BARRIER;
    out->y = leftC[2];
    out->z = leftC[3];
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant H1: Direct vector PS (low + high2)
test_export test_inline uint128_t regcall test_vec3_H1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhsX], %%XMM0 \n"
        "MOVHPS %[lhsYZ], %%XMM0 \n"
        "MOVSS %[rhsX], %%XMM1 \n"
        "MOVHPS %[rhsYZ], %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSS %%XMM1, %[outX] \n"
        "MOVHPS %%XMM1, %[outYZ] \n"
        : [outX]"=m"(out->x), [outYZ]"=m"(*(double*)&out->y)
        : [lhsX]"m"(lhs->x), [lhsYZ]"m"(*(double*)&lhs->y),
          [rhsX]"m"(rhs->x), [rhsYZ]"m"(*(double*)&rhs->y)
        : clobber_list("xmm0", "xmm1")
    );
#else
    MEM_BARRIER;
    vec<float, 4> leftA = { lhs->x, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftB = { lhs->y, lhs->z, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> leftC = shufflevec(leftA, leftB, 0, 1, 4, 5);
    MEM_BARRIER;
    vec<float, 4> rightA = { rhs->x, 0.0f, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightB = { rhs->y, rhs->z, 0.0f, 0.0f };
    MEM_BARRIER;
    vec<float, 4> rightC = shufflevec(rightA, rightB, 0, 1, 4, 5);
    MEM_BARRIER;
    leftC += rightC;
    MEM_BARRIER;
    out->x = leftC[0];
    MEM_BARRIER;
    out->y = leftC[2];
    out->z = leftC[3];
    MEM_BARRIER;
#endif
    PACK_RETURNS;
}

// Variant I1: Masked vector MMA
test_export test_inline uint128_t regcall test_vec3_I1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
    __asm__(
        "VMOVUPS %[lhs], %%XMM0%{%%k1%} \n"
        "VMOVUPS %[rhs], %%XMM1%{%%k1%} \n"
        "VADDPS %%XMM1, %%XMM0, %%XMM0 \n"
        "VMOVUPS %%XMM0, %[out]%{%%k1%} \n"
        : [out]"=m"(*out)
        : [lhs]"m"(*lhs), [rhs]"m"(*rhs)
        : clobber_list("xmm0", "xmm1")
    );
    PACK_RETURNS;
}

// Variant I2: Masked vector MA
test_export test_inline uint128_t regcall test_vec3_I2(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
    __asm__(
        "VMOVUPS %[lhs], %%XMM0%{%%k1%} \n"
        "VADDPS %[rhs], %%XMM0, %%XMM0%{%%k1%}%{z%} \n"
        "VMOVUPS %%XMM0, %[out]%{%%k1%} \n"
        : [out]"=m"(*out)
        : [lhs]"m"(*lhs), [rhs]"m"(*rhs)
        : clobber_list("xmm0")
    );
    PACK_RETURNS;
}

// Variant V1: Vec1 baseline
test_export test_inline uint128_t regcall test_vec3_V1(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSS %[lhs], %%XMM0 \n"
        "ADDSS %[rhs], %%XMM0 \n"
        "MOVSS %%XMM0, %[out] \n"
        /*
        "MOVSS %[lhs], %%XMM0 \n"
        "MOVSS %[rhs], %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSS %%XMM1, %[out] \n"
        */
        : [out]"=m"(out->x)
        : [lhs]"m"(lhs->x), [rhs]"m"(rhs->x)
        : clobber_list("xmm0")
    );
#else
    float val = lhs->x;
    val += rhs->x;
    out->x = val;
#endif
    PACK_RETURNS;
}

// Variant V2: Vec2 baseline
test_export test_inline uint128_t regcall test_vec3_V2(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVSD %[lhs], %%XMM0 \n"
        "MOVSD %[rhs], %%XMM1 \n"
        "ADDPS %%XMM0, %%XMM1 \n"
        "MOVSD %%XMM1, %[out] \n"
        : [out]"=m"(*(double*)out)
        : [lhs]"m"(*(double*)lhs), [rhs]"m"(*(double*)rhs)
        : clobber_list("xmm0", "xmm1")
    );
#else
    vec<float, 2> left = *(vec<float, 2>*)lhs;
    vec<float, 2> right = *(vec<float, 2>*)rhs;
    left += right;
    *(vec<float, 2>*)out = left;
#endif
    PACK_RETURNS;
}

// Variant V4: Vec4 baseline
test_export test_inline uint128_t regcall test_vec3_V4(Float3* out, const Float3* lhs, const Float3* rhs, size_t i = GARBAGE_ARG(int32_t)) {
#if TEST_USE_ASM
    __asm__(
        "MOVUPS %[lhs], %%XMM0 \n"
        "ADDPS %[rhs], %%XMM0 \n"
        "MOVUPS %%XMM0, %[out] \n"
        : [out] "=m"(*out)
        : [lhs] "m"(*lhs), [rhs]"m"(*rhs)
        : clobber_list("xmm0")
    );
#else
    vec<float, 4> left = *(vec<float, 4>*)lhs;
    vec<float, 4> right = *(vec<float, 4>*)rhs;
    left += right;
    *(vec<float, 4>*)out = left;
#endif
    PACK_RETURNS;
}

#define TEST_CONTROLS_FIRST 1

#define make_test_timers(variant) \
static volatile uint64_t MACRO_CAT(start_,variant); \
static volatile uint64_t MACRO_CAT(end_,variant); \
static uint64_t MACRO_CAT(time_,variant);

#if TEST_CONTROLS_FIRST
for_each_vec_type_control(make_test_timers);
#endif
for_each_vec_type(make_test_timers);
#if !TEST_CONTROLS_FIRST
for_each_vec_type_control(make_test_timers);
#endif

#define cache_touch_arrays \
nounroll for (size_t i = 0; i < element_count; ++i) { \
    *(volatile float*)&lhs[i].x = *(volatile float*)&lhs[i].x; \
    *(volatile float*)&lhs[i].y = *(volatile float*)&lhs[i].y; \
    *(volatile float*)&lhs[i].z = *(volatile float*)&lhs[i].z; \
    *(volatile float*)&rhs[i].x = *(volatile float*)&rhs[i].x; \
    *(volatile float*)&rhs[i].y = *(volatile float*)&rhs[i].y; \
    *(volatile float*)&rhs[i].z = *(volatile float*)&rhs[i].z; \
    *(volatile float*)&out[i].x = *(volatile float*)&out[i].x; \
    *(volatile float*)&out[i].y = *(volatile float*)&out[i].y; \
    *(volatile float*)&out[i].z = *(volatile float*)&out[i].z; \
}

#define cache_touch_arrays1 \
nounroll for (size_t i = 0; i < element_count; ++i) { \
    (void)*(volatile float*)&lhs[i].x; \
    (void)*(volatile float*)&lhs[i].y; \
    (void)*(volatile float*)&lhs[i].z; \
    (void)*(volatile float*)&rhs[i].x; \
    (void)*(volatile float*)&rhs[i].y; \
    (void)*(volatile float*)&rhs[i].z; \
    (void)*(volatile float*)&out[i].x; \
    (void)*(volatile float*)&out[i].y; \
    (void)*(volatile float*)&out[i].z; \
}

#define cache_touch_arrays2 \
rep_movsb(lhs, lhs, allocation_size); \
rep_movsb(rhs, rhs, allocation_size); \
rep_movsb(out, out, allocation_size)

#define cache_touch_arrays3 \
rep_movsd(lhs, lhs, allocation_size / 4); \
rep_movsd(rhs, rhs, allocation_size / 4); \
rep_movsd(out, out, allocation_size / 4)

#define cache_touch_arrays4 \
repne_cmpsb(lhs, lhs, allocation_size); \
repne_cmpsb(rhs, rhs, allocation_size); \
repne_cmpsb(out, out, allocation_size)

#define cache_touch_arrays5 \
repne_cmpsd(lhs, lhs, allocation_size / 4); \
repne_cmpsd(rhs, rhs, allocation_size / 4); \
repne_cmpsd(out, out, allocation_size / 4)

/*
#if TEST_USE_ASM
#define test_force_registersA __asm__ volatile ("":"+S"(out),"+D"(lhs),"+b"(rhs))
#define test_force_registersB do; while(0)
#else
#define test_force_registersA do; while(0)
#define test_force_registersB __asm__ volatile ("":"+a"(out),"+c"(lhs),"+d"(rhs),"+D"(j))
#endif
*/

#define cache_touch() cache_touch_arrays2

#define CACHE_TOUCH_EACH_TEST 0

#if CACHE_TOUCH_EACH_TEST
#define test_cache_touch() cache_touch()
#define startup_cache_touch()
#define wait_for_cache_touch "MFENCE \n"
#else
#define test_cache_touch()
#define startup_cache_touch() cache_touch()
#define wait_for_cache_touch "MFENCE \n"
#endif

static forceinline uint64_t rdtsc_max_serialize(void) {
    uint64_t tick;
    __asm__ volatile (
        "MFENCE \n"
        "LFENCE \n"
        "RDTSC \n"
        "LFENCE \n"
        : "=A"(tick)
    );
    return tick;
}

#define TEST_COUNT 64

#if TEST_COUNT > 1
#define force_reg_pattern "+r"(count), "+S"(lhs), "+D"(rhs), "+b"(out)
#define test_vec3(variant) test_vec3_repeat(variant)
#else
#define force_reg_pattern "+r"(offset), "+r"(lhs), "+r"(rhs), "+r"(out)
#define test_vec3(variant) test_vec3_no_repeat(variant)
#endif

#define FENCE_SPECULATION 1

#if FENCE_SPECULATION
#define speculative_fence "LFENCE \n"
#else
#define speculative_fence ""
#endif

#define SCHEDULER_TYPE_NONE 0
#define SCHEDULER_TYPE_SLEEP 1
#define SCHEDULER_TYPE_SWITCH 2

#define SCHEDULER_VARIANT SCHEDULER_TYPE_NONE

#if SCHEDULER_VARIANT == SCHEDULER_TYPE_NONE
#define test_sleep() do; while(0)
#elif SCHEDULER_VARIANT == SCHEDULER_TYPE_SLEEP
#define test_sleep() Sleep(0)
#elif SCHEDULER_VARIANT == SCHEDULER_TYPE_SWITCH
#define test_sleep() SwitchToThread()
#endif

#define TIMESTAMP_MEM_BEFORE 0
#define TIMESTAMP_MEM_AFTER 1
#define TIMESTAMP_MEM_AFTER_NTI 2

#define TIMESTAMP_TYPE TIMESTAMP_MEM_AFTER

#if TIMESTAMP_TYPE == TIMESTAMP_MEM_BEFORE

#define start_timestamps(variant) \
uint64_t time; \
__asm__ volatile ( \
    wait_for_cache_touch \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    : "=A"(time), force_reg_pattern \
)

#define write_timestamps(variant) \
__asm__ volatile ( \
    speculative_fence \
    : "+A"(time) \
); \
MACRO_CAT(start_,variant) = time; \
__asm__ volatile ( \
    "LFENCE \n" \
    "MFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    : "=A"(time) \
); \
MACRO_CAT(end_,variant) = time

#elif TIMESTAMP_TYPE == TIMESTAMP_MEM_AFTER

#define start_timestamps(variant) \
uint64_t time; \
__asm__ volatile ( \
    wait_for_cache_touch \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    : "=A"(time), force_reg_pattern \
)

#define write_timestamps(variant) \
register uint32_t low asm("ebp"); \
uint32_t high; \
__asm__ volatile ( \
    speculative_fence \
    "MOV %%EAX, %[low] \n" \
    "MOV %%EDX, %[high] \n" \
    "MFENCE \n" \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    "MOV %[low], %[L1] \n" \
    "MOV %[high], %[H1] \n" \
    "MOV %%EAX, %[L2] \n" \
    "MOV %%EDX, %[H2] \n" \
    : "+A"(time), asm_arg("=r", low), asm_arg("=r", high), [L1]"=m"(MACRO_CAT(start_,variant)), [H1]"=m"(((uint32_t*)&MACRO_CAT(start_,variant))[1]), [L2]"=m"(MACRO_CAT(end_,variant)), [H2]"=m"(((uint32_t*)&MACRO_CAT(end_,variant))[1]) \
)

#elif TIMESTAMP_TYPE == TIMESTAMP_MEM_AFTER_NTI

#define start_timestamps(variant) \
uint64_t time; \
__asm__ volatile ( \
    wait_for_cache_touch \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    : "=A"(time), force_reg_pattern \
)

#define write_timestamps(variant) \
register uint32_t low asm("ebp"); \
uint32_t high; \
__asm__ volatile ( \
    speculative_fence \
    "MOV %%EAX, %[low] \n" \
    "MOV %%EDX, %[high] \n" \
    "MFENCE \n" \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    "MOVNTI %[low], %[L1] \n" \
    "MOVNTI %[high], %[H1] \n" \
    "MOVNTI %%EAX, %[L2] \n" \
    "MOVNTI %%EDX, %[H2] \n" \
    : "+A"(time), asm_arg("=r", low), asm_arg("=r", high), [L1]"=m"(MACRO_CAT(start_,variant)), [H1]"=m"(((uint32_t*)&MACRO_CAT(start_,variant))[1]), [L2]"=m"(MACRO_CAT(end_,variant)), [H2]"=m"(((uint32_t*)&MACRO_CAT(end_,variant))[1]) \
)

#endif

#define test_vec3_repeat(variant) \
{ \
    __asm__ volatile ("" : "+r"(lhs), "+r"(rhs), "+r"(out)); \
    test_sleep(); \
    test_cache_touch(); \
    size_t count = TEST_COUNT; \
    start_timestamps(variant); \
    nounroll do { \
        register size_t offset asm("ebp") = 0; \
        nounroll do { \
            __asm__(""::"r"(offset),"A"(time)); \
            MACRO_CAT(test_vec3_,variant)(based_pointer(out, offset), based_pointer(lhs, offset), based_pointer(rhs, offset)); \
            offset += sizeof(FloatT); \
        } while (offset != allocation_size); \
    } while (--count); \
    write_timestamps(variant); \
}

#define test_vec3_no_repeat(variant) \
{ \
    __asm__ volatile ("" : "+r"(lhs), "+r"(rhs), "+r"(out)); \
    test_sleep(); \    
    test_cache_touch(); \
    register size_t offset asm("ebp") = 0; \
    start_timestamps(variant); \
    nounroll do { \
        __asm__(""::"r"(offset),"A"(time)); \
        MACRO_CAT(test_vec3_,variant)(based_pointer(out, offset), based_pointer(lhs, offset), based_pointer(rhs, offset)); \
        offset += sizeof(FloatT); \
    } while (offset != allocation_size); \
    write_timestamps(variant); \
}


/*
#undef test_vec3

#define test_vec3(variant, ...) \
do { \
    __asm__ volatile ("":asm_arg("+r", array_lhs), asm_arg("+r", array_rhs), asm_arg("+r", array_out)); \
    MACRO_DEFAULT_ARG(CACHE_TOUCH_VARIANT,__VA_ARGS__); \
    register size_t offset asm("ebp") = 0; \
    uint64_t time; \
    __asm__ volatile ( \
        "MFENCE \n" \
        "LFENCE \n" \
        "RDTSC \n" \
        "LFENCE \n" \
        : asm_arg("+r", offset), asm_arg("=A", time), asm_arg("+r", array_lhs), asm_arg("+r", array_rhs), asm_arg("+r", array_out) \
    ); \
    nounroll do { \
        __asm__(""::"r"(offset),"A"(time)); \
        auto ret = MACRO_CAT(test_vec3_,variant)( \
            based_pointer(array_out, offset), \
            based_pointer(array_lhs, offset), \
            based_pointer(array_rhs, offset), \
            GARBAGE_ARG(int32_t) \
        ); \
        offset += sizeof(Float3); \
    } while (offset != allocation_size); \
    uint32_t high; \
    __asm__ volatile ( \
        "LFENCE \n" \
        "MOV %%EAX, %[offset] \n" \
        "MOV %%EDX, %[high] \n" \
        "MFENCE \n" \
        "LFENCE \n" \
        "RDTSC \n" \
        "LFENCE \n" \
        "MOVNTI %[offset], %[L1] \n" \
        "MOVNTI %[high], %[H1] \n" \
        "MOVNTI %%EAX, %[L2] \n" \
        "MOVNTI %%EDX, %[H2] \n" \
        : "+A"(time), asm_arg("=r", offset), asm_arg("=r", high), [L1]"=m"(MACRO_CAT(start_, variant)), [H1]"=m"(((uint32_t*)&MACRO_CAT(start_, variant))[1]), [L2]"=m"(MACRO_CAT(end_, variant)), [H2]"=m"(((uint32_t*)&MACRO_CAT(end_, variant))[1]) \
    ); \
} while (0)
*/
/*
#define test_vec3(variant) \
cache_touch_arrays2; \
MACRO_CAT(start_,variant) = rdtsc_serialize(); \
{ \
    size_t i = test_count; \
    nounroll do { \
        test_force_registersA; \
        out = array_out; \
        lhs = array_lhs; \
        rhs = array_rhs; \
        size_t j = element_count; \
        test_force_registersA; \
        test_force_registersB; \
        nounroll do { \
            auto ret = MACRO_CAT(test_vec3_,variant)(out, lhs, rhs, j); \
            out = (decltype(out))ret; \
            lhs = (decltype(lhs))(ret >> 32); \
            rhs = (decltype(rhs))(ret >> 64); \
            j = (decltype(j))(ret >> 96); \
            ++out; \
            ++lhs; \
            ++rhs; \
        } while (--j); \
    } while (--i); \
} \
MACRO_CAT(end_,variant) = rdtsc_serialize()
*/

/*
#define test_vec3(variant) \
out = array_out; \
lhs = array_lhs; \
rhs = array_rhs; \
MACRO_CAT(start_,variant) = rdtsc_serialize(); \
{ \
    size_t i = test_count; \
    nounroll do { \
        nounroll for ( \
            register size_t offset asm("ebp") = 0; \
            offset != (element_count * sizeof(Float3)); \
            offset += sizeof(Float3) \
        ) { \
            __asm__ volatile (""::"r"(offset)); \
            auto ret = MACRO_CAT(test_vec3_,variant)( \
                based_pointer(out,offset), \
                based_pointer(lhs,offset), \
                based_pointer(rhs,offset) \
            ); \
            out = (decltype(out))ret; \
            lhs = (decltype(lhs))(ret >> 32); \
            rhs = (decltype(rhs))(ret >> 64); \
            i = (decltype(i))(ret >> 96); \
        } \
    } while (--i); \
} \
MACRO_CAT(end_,variant) = rdtsc_serialize()
*/

gnu_noinline void fastcall deal_with_64_bit_stuff(bool has_avx512) {
    __asm__ volatile (
        INTEL_64_DIRECTIVE
        "TEST ECX, ECX \n"
        "JZ 1f \n"
        "VPBROADCASTD ZMM0, ECX \n"
        "VPADDD ZMM0, ZMM0, ZMM0 \n"
    "1: \n"
        "VZEROUPPER \n"
        "RETF \n"
    );
    unreachable;
}

#define CurrentProcessPseudoHandle() ((HANDLE)-1)
#define CurrentThreadPseudoHandle() ((HANDLE)-2)

#define RECORD_OVERALL_TEST_TIME 1

#define USE_ASM_JUMP 0

#define FREE_ALLOCATIONS 0

int main(int argc, char* argv[]) {

    // Half MB of data
#define element_count 16384

    char manufacturer[12];
    uint32_t eax, ebx, ecx, edx;
    get_cpuid(0, eax, ebx, ecx, edx);
    *(uint32_t*)&manufacturer[0] = ebx;
    *(uint32_t*)&manufacturer[4] = edx;
    *(uint32_t*)&manufacturer[8] = ecx;

    bool can_test_masks = false;
    if (eax >= 7) {
        get_cpuid_ex(7, 0, eax, ebx, ecx, edx);
        can_test_masks = (ebx & 1 << 16) && (ebx & 1 << 31);
    }

    get_cpuid(1, eax, ebx, ecx, edx);

    bool run_zeroall = edx & 1 << 28;

    uint8_t stepping = eax & 0xF;
    uint8_t model = eax >> 4 & 0xF;
    uint8_t extended_model = 0;
    uint32_t family = eax >> 8 & 0xF;
    switch (family) {
        case 15:
            family += eax >> 20 & 0xFF;
        case 6:
            extended_model = eax >> 16 & 0xF;
    }
    uint32_t cache_line_size = (edx & 1 << 19) ? (ebx >> 8 & 0xFF) * 8 : 64;

#if TEST_COUNT > 1
#define REPEAT_COUNT " x " MACRO_STR(TEST_COUNT) " iters"
#else
#define REPEAT_COUNT
#endif

#define TEST_HEADER "Vec3 Testing: " MACRO_STR(element_count) "x" MACRO_STR(element_size) " bytes" REPEAT_COUNT "\n"

    if (
        *(uint32_t*)&manufacturer[0] == PackUInt32('G', 'e', 'n', 'u') &&
        *(uint32_t*)&manufacturer[4] == PackUInt32('i', 'n', 'e', 'I') &&
        *(uint32_t*)&manufacturer[8] == PackUInt32('n', 't', 'e', 'l')
    ) {
        printf(
            TEST_HEADER
            "Brand:      Intel\n"
            "Model:      %u.0x%X.0x%X.0x%X\n"
            "CacheLine:  %u\n"
            "AVX512:     %s\n"
            , family, extended_model, model, stepping
            , cache_line_size
            , bool_str(can_test_masks)
        );
    } else if (
        *(uint32_t*)&manufacturer[0] == PackUInt32('A', 'u', 't', 'h') &&
        *(uint32_t*)&manufacturer[4] == PackUInt32('e', 'n', 't', 'i') &&
        *(uint32_t*)&manufacturer[8] == PackUInt32('c', 'A', 'M', 'D')
    ) {
        printf(
            TEST_HEADER
            "Brand:      AMD\n"
            "Model:      0x%X.0x%X.0x%X.0x%X\n"
            "CacheLine:  %u\n"
            "AVX512:     %s\n"
            , family, extended_model, model, stepping
            , cache_line_size
            , bool_str(can_test_masks)
        );
    } else {
        printf(
            TEST_HEADER
            "Brand:      %.12s\n"
            "Model:      0x%X.0x%X.0x%X.0x%X\n"
            "CacheLine:  %u\n"
            "AVX512:     %s\n"
            , manufacturer
            , family, extended_model, model, stepping
            , cache_line_size
            , bool_str(can_test_masks)
        );
    }

    srand(0);

    SetPriorityClass(CurrentProcessPseudoHandle(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(CurrentThreadPseudoHandle(), THREAD_PRIORITY_TIME_CRITICAL);

    SwitchToThread();

    if (run_zeroall) {
        decltype(&IsWow64Process) IsWow64ProcessVar;
        BOOL IsX64;
        if (
            (IsWow64ProcessVar = (decltype(&IsWow64Process))GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process")) &&
            IsWow64ProcessVar(GetCurrentProcess(), &IsX64) &&
            IsX64 == TRUE
        ) {
            FAR_CALL_IMM(0x33, deal_with_64_bit_stuff,
                         ,
                         "c"((int)can_test_masks)
                         : clobber_list("xmm0")
            );
        }
    }

    SetProcessAffinityMask(CurrentProcessPseudoHandle(), 1 << GetCurrentProcessorNumber());

    Sleep(5000);

    constexpr size_t allocation_size = sizeof(FloatT) * element_count;

    FloatT* lhs = (FloatT*)_aligned_malloc(allocation_size, cache_line_size);
    FloatT* rhs = (FloatT*)_aligned_malloc(allocation_size, cache_line_size);
    FloatT* out = (FloatT*)_aligned_malloc(allocation_size, cache_line_size);

    // Fill input arrays and load memory into cache
    for (size_t i = 0; i < element_count; ++i) {
        lhs[i] = { rand(), rand(), rand(), rand() };
        rhs[i] = { rand(), rand(), rand(), rand() };
        out[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    if (run_zeroall) {
        __asm__ volatile (
            "VZEROALL"
            :
        );
    }

#if RECORD_OVERALL_TEST_TIME
    union {
        struct {
            uint64_t test_start packed_field;
            uint64_t test_end packed_field;
            uint64_t qpc_freq packed_field;
            uint64_t qpc_start packed_field;
            uint64_t qpc_end packed_field;
        };
    };
    QueryPerformanceCounter((LARGE_INTEGER*)&qpc_start);
    test_start = rdtsc_max_serialize();
#endif

#if TEST_CONTROLS_FIRST
    for_each_vec_type_control(test_vec3);
#endif
    for_each_vec_type_no_avx512(test_vec3);

#if USE_ASM_JUMP
    __asm__ (
        "CMPB $0, %[can_test_masks] \n"
        "JE skip_mask_test"
        :
        : asm_arg("m", can_test_masks)
    );
#else
    if (expect(can_test_masks, true))
#endif
    {
        register uint32_t mask asm("ebp") = 0b111;
        __asm__ volatile (
            "KMOVW %[mask], %%k1 \n"
            :
            : asm_arg("r", mask)
        );

        for_each_vec_type_avx512(test_vec3);
    }
#if USE_ASM_JUMP
    __asm__(
        "skip_mask_test:"
    );
#endif
#if !TEST_CONTROLS_FIRST
    for_each_vec_type_control(test_vec3);
#endif

#if RECORD_OVERALL_TEST_TIME
    test_end = rdtsc_max_serialize();
    QueryPerformanceCounter((LARGE_INTEGER*)&qpc_end);
    test_end -= test_start;
    qpc_end -= qpc_start;
    QueryPerformanceFrequency((LARGE_INTEGER*)&qpc_freq);
#endif

#if FREE_ALLOCATIONS
    _aligned_free(lhs);
    _aligned_free(rhs);
    _aligned_free(out);
#endif

    uint64_t total = 0;

#define calculate_vec3_time(variant) \
    total += (MACRO_CAT(time_,variant) = (MACRO_CAT(end_,variant) - MACRO_CAT(start_,variant)));
#define calculate_vec3_time_no_total(variant) \
    MACRO_CAT(time_,variant) = (MACRO_CAT(end_,variant) - MACRO_CAT(start_,variant));

#if TEST_CONTROLS_FIRST
    for_each_vec_type_control(calculate_vec3_time);
#endif
    for_each_vec_type(calculate_vec3_time);
#if !TEST_CONTROLS_FIRST
    for_each_vec_type_control(calculate_vec3_time);
#endif

#if RECORD_OVERALL_TEST_TIME
#define RT \
        " [%*.3fus]"
#define print_vec3_time(variant) \
        , max_width, MACRO_CAT(time_,variant) \
        , time_width, qpc_us * ((double)MACRO_CAT(time_,variant) * total_double), (double)MACRO_CAT(time_,variant) * (100.0 * total_double) \
        , -100.0 * (1.0 - ((double)MACRO_CAT(time_, variant) / float3_estimate))
#define print_time_estimate \
        , max_width, float3_estimate \
        , time_width, qpc_us * float3_estimate * total_double

    int max_width = snprintf(NULL, 0, "%llu", test_end);
    double qpc_us = 1000000.0 * ((double)qpc_end / (double)qpc_freq);
    int time_width = snprintf(NULL, 0, "%.3f", qpc_us);
#else
#define RT \
        ""
#define print_vec3_time(variant) \
        , max_width, MACRO_CAT(time_,variant) \
        , (double)MACRO_CAT(time_,variant) * (100.0 * total_double) \
        , -100.0 * (1.0 - ((double)MACRO_CAT(time_, variant) / float3_estimate))
#define print_time_estimate \
        , max_width, float3_estimate

    int max_width = snprintf(NULL, 0, "%llu", total);
#endif
    double total_double = 1.0 / (double)total;

    double best_fit_slope = ((double)time_V4 - (double)time_V1) / 3.0;
    double best_fit_offset = ((double)time_V1 * 6.0 + (double)time_V2 * 3.0 - (double)time_V4 * 3.0) / 6.0;
    double float3_estimate = best_fit_slope * 3.0 + best_fit_offset;

#define vec3_format_str(variant) \
        MACRO_CAT(desc_,variant) "%*llu" RT " (%.3f%%) {%+.3f%%}\n"

#define desc_A1 "Scalar:                  "
#define desc_A2 "Scalar LS:               "
#define desc_B1 "Split vector PS L2H:     "
#define desc_B2 "Split vector PS L2H LS:  "
#define desc_B3 "Split vector SD L2H:     "
#define desc_B4 "Split vector SD L2H LS:  "
#define desc_C1 "Split vector PS LH2:     "
#define desc_C2 "Split vector PS LH2 LS:  "
#define desc_C3 "Split vector SD LH2:     "
#define desc_C4 "Split vector SD LH2 LS:  "
#define desc_D1 "Shuffle vector PS L2H:   "
#define desc_D2 "Shuffle vector SD L2H:   "
#define desc_E1 "Shuffle vector PS LH2:   "
#define desc_F1 "Half vector PS L2H:      "
#define desc_F2 "Half vector SD L2H:      "
#define desc_G1 "Half vector PS LH2:      "
#define desc_H1 "Direct vector PS LH2:    "
#define desc_I1 "Masked vector MMA:       "
#define desc_I2 "Masked vector MA:        "
#define desc_V1 "Float1 reference:        "
#define desc_V2 "Float2 reference:        "
#define desc_V4 "Float4 reference:        "

    printf(
#if RECORD_OVERALL_TEST_TIME
        "Realtime:                %llu [%.3fus] (+5s wait)\n"
        "Timestamp total:         %*llu [%*.3fus]\n"
#else
        "Timestamp total:         %llu\n"
#endif
#if TEST_CONTROLS_FIRST
        for_each_vec_type_control(vec3_format_str)
        "Float3 ideal estimate:   %*.0f" RT "\n"
#endif
        for_each_vec_type_no_avx512(vec3_format_str)
#if RECORD_OVERALL_TEST_TIME
        , test_end, qpc_us
        , max_width, total, time_width, qpc_us * ((double)total / (double)test_end)
#else
        , total
#endif
#if TEST_CONTROLS_FIRST
        for_each_vec_type_control(print_vec3_time)
        print_time_estimate
#endif
        for_each_vec_type_no_avx512(print_vec3_time)
    );

    if (can_test_masks)
    printf(
        for_each_vec_type_avx512(vec3_format_str)
        for_each_vec_type_avx512(print_vec3_time)
    );

#if !TEST_CONTROLS_FIRST
    printf(
        for_each_vec_type_control(vec3_format_str)
        "Float3 ideal estimate:   %*.0f\n"
        for_each_vec_type_control(print_vec3_time)
        print_time_estimate
    );
#endif

    return 0;
}