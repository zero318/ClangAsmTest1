#pragma once
#ifndef TH_COMMON_H
#define TH_COMMON_H 1
#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <type_traits>
#include <utility>
#include <bit>

#define _USE_MATH_DEFINES 1
#include <math.h>

#define EoSD_VER 60
#define PCB_VER 70
#define IN_VER 80
#define PoFV_VER 90
#define StB_VER 95
#define MoF_VER 100
#define UB_VER 103
#define SA_VER 110
#define UFO_VER 120
#define DS_VER 125
#define GFW_VER 128
#define TD_VER 130
#define DDC_VER 140
#define ISC_VER 143
#define LoLK_VER 150
#define HSiFS_VER 160
#define VD_VER 165
#define WBaWC_VER 170
#define UM_VER 180
#define HBM_VER 185
#define UDoALG_VER 190


#define _MACRO_CAT(arg1, arg2) arg1 ## arg2
#define MACRO_CAT(arg1, arg2) _MACRO_CAT(arg1, arg2)
#define MACRO_EVAL(...) __VA_ARGS__

#ifndef TH_VER

// Just pick UM as the default
#define TH_VER UM_VER

#else

#define _THV_EoSD EoSD_VER
#define _THV_EOSD EoSD_VER
#define _THV_eosd EoSD_VER
#define _THV_th6 EoSD_VER
#define _THV_th06 EoSD_VER
#define _THV_6 EoSD_VER
#define _THV_06 EoSD_VER
#define _THV_PCB PCB_VER
#define _THV_pcb PCB_VER
#define _THV_th7 PCB_VER
#define _THV_th07 PCB_VER
#define _THV_7 PCB_VER
#define _THV_07 PCB_VER
#define _THV_IN IN_VER
#define _THV_in IN_VER
#define _THV_th8 IN_VER
#define _THV_th08 IN_VER
#define _THV_8 IN_VER
#define _THV_08 IN_VER
#define _THV_PoFV PoFV_VER
#define _THV_POFV PoFV_VER
#define _THV_pofv PoFV_VER
#define _THV_th9 PoFV_VER
#define _THV_th09 PoFV_VER
#define _THV_9 PoFV_VER
#define _THV_09 PoFV_VER
#define _THV_StB StB_VER
#define _THV_STB StB_VER
#define _THV_stb StB_VER
#define _THV_th95 StB_VER
#define _THV_th095 StB_VER
#define _THV_95 StB_VER
#define _THV_095 StB_VER
#define _THV_MoF MoF_VER
#define _THV_MOF MoF_VER
#define _THV_mof MoF_VER
#define _THV_th10 MoF_VER
#define _THV_10 MoF_VER
#define _THV_UB UB_VER
#define _THV_ub UB_VER
#define _THV_ALCOSTG UB_VER
#define _THV_alcostg UB_VER
#define _THV_th103 UB_VER
#define _THV_103 UB_VER
#define _THV_SA SA_VER
#define _THV_sa SA_VER
#define _THV_th11 SA_VER
#define _THV_11 SA_VER
#define _THV_UFO UFO_VER
#define _THV_ufo UFO_VER
#define _THV_th12 UFO_VER
#define _THV_12 UFO_VER
#define _THV_DS DS_VER
#define _THV_ds DS_VER
#define _THV_th125 DS_VER
#define _THV_125 DS_VER
#define _THV_GFW GFW_VER
#define _THV_gfw GFW_VER
#define _THV_FW GFW_VER
#define _THV_fw GFW_VER
#define _THV_th128 GFW_VER
#define _THV_128 GFW_VER
#define _THV_TD TD_VER
#define _THV_td TD_VER
#define _THV_th13 TD_VER
#define _THV_13 TD_VER
#define _THV_DDC DDC_VER
#define _THV_ddc DDC_VER
#define _THV_th14 DDC_VER
#define _THV_14 DDC_VER
#define _THV_ISC ISC_VER
#define _THV_isc ISC_VER
#define _THV_th143 ISC_VER
#define _THV_143 ISC_VER
#define _THV_LoLK LoLK_VER
#define _THV_LOLK LoLK_VER
#define _THV_lolk LoLK_VER
#define _THV_th15 LoLK_VER
#define _THV_15 LoLK_VER
#define _THV_HSiFS HSiFS_VER
#define _THV_HSIFS HSiFS_VER
#define _THV_hsifs HSiFS_VER
#define _THV_th16 HSiFS_VER
#define _THV_16 HSiFS_VER
#define _THV_VD VD_VER
#define _THV_vd VD_VER
#define _THV_th165 VD_VER
#define _THV_165 VD_VER
#define _THV_WBaWC WBaWC_VER
#define _THV_WBAWC WBaWC_VER
#define _THV_wbawc WBaWC_VER
#define _THV_th17 WBaWC_VER
#define _THV_17 WBaWC_VER
#define _THV_UM UM_VER
#define _THV_um UM_VER
#define _THV_th18 UM_VER
#define _THV_18 UM_VER
#define _THV_HBM HBM_VER
#define _THV_hbm HBM_VER
#define _THV_BM HBM_VER
#define _THV_bm HBM_VER
#define _THV_th185 HBM_VER
#define _THV_185 HBM_VER
#define _THV_UDoALG UDoALG_VER
#define _THV_UDoaLG UDoALG_VER
#define _THV_UDOALG UDoALG_VER
#define _THV_udoalg UDoALG_VER
#define _THV_th19 UDoALG_VER
#define _THV_19 UDoALG_VER

#define PARSE_TH_VER(...) MACRO_EVAL(MACRO_CAT(_THV_,__VA_ARGS__))

#if PARSE_TH_VER(TH_VER) == EoSD_VER
#undef TH_VER
#define TH_VER EoSD_VER
#elif PARSE_TH_VER(TH_VER) == PCB_VER
#undef TH_VER
#define TH_VER PCB_VER
#elif PARSE_TH_VER(TH_VER) == IN_VER
#undef TH_VER
#define TH_VER IN_VER
#elif PARSE_TH_VER(TH_VER) == PoFV_VER
#undef TH_VER
#define TH_VER PoFV_VER
#elif PARSE_TH_VER(TH_VER) == StB_VER
#undef TH_VER
#define TH_VER StB_VER
#elif PARSE_TH_VER(TH_VER) == MoF_VER
#undef TH_VER
#define TH_VER MoF_VER
#elif PARSE_TH_VER(TH_VER) == UB_VER
#undef TH_VER
#define TH_VER UB_VER
#elif PARSE_TH_VER(TH_VER) == SA_VER
#undef TH_VER
#define TH_VER SA_VER
#elif PARSE_TH_VER(TH_VER) == UFO_VER
#undef TH_VER
#define TH_VER UFO_VER
#elif PARSE_TH_VER(TH_VER) == DS_VER
#undef TH_VER
#define TH_VER DS_VER
#elif PARSE_TH_VER(TH_VER) == GFW_VER
#undef TH_VER
#define TH_VER GFW_VER
#elif PARSE_TH_VER(TH_VER) == TD_VER
#undef TH_VER
#define TH_VER TD_VER
#elif PARSE_TH_VER(TH_VER) == DDC_VER
#undef TH_VER
#define TH_VER DDC_VER
#elif PARSE_TH_VER(TH_VER) == ISC_VER
#undef TH_VER
#define TH_VER ISC_VER
#elif PARSE_TH_VER(TH_VER) == LoLK_VER
#undef TH_VER
#define TH_VER LoLK_VER
#elif PARSE_TH_VER(TH_VER) == HSiFS_VER
#undef TH_VER
#define TH_VER HSiFS_VER
#elif PARSE_TH_VER(TH_VER) == VD_VER
#undef TH_VER
#define TH_VER VD_VER
#elif PARSE_TH_VER(TH_VER) == WBaWC_VER
#undef TH_VER
#define TH_VER WBaWC_VER
#elif PARSE_TH_VER(TH_VER) == UM_VER
#undef TH_VER
#define TH_VER UM_VER
#elif PARSE_TH_VER(TH_VER) == HBM_VER
#undef TH_VER
#define TH_VER HBM_VER
#elif PARSE_TH_VER(TH_VER) == UDoALG_VER
#undef TH_VER
#define TH_VER UDoALG_VER
#else
#error Unrecognized version
#endif

#endif

#if TH_VER < MoF_VER
#define OLD_ENGINE 1
#elif TH_VER < LoLK_VER
#define NEW_ENGINE 1
#define NEW_ENGINE_A 1
#else
#define NEW_ENGINE 1
#define NEW_ENGINE_B 1
#endif

#define sfinae_enable(...) std::enable_if_t<(__VA_ARGS__), bool> = true

enum ZUNResult : int32_t {
    ZUN_SUCCESS2 = 1,
    ZUN_SUCCESS = 0,
    ZUN_ERROR = -1,
    ZUN_ERROR2 = -2
};

#define ZUN_SUCCEEDED(result) (((ZunResult)(result)) >= ZUN_SUCCESS)
#define ZUN_FAILED(result) (((ZunResult)(result)) < ZUN_SUCCESS)

// Packs the bytes [c1], [c2], [c3], and [c4] together as a 32 bit little endian unsigned integer
static inline constexpr uint32_t PackUInt32(uint8_t c1, uint8_t c2 = 0, uint8_t c3 = 0, uint8_t c4 = 0) {
	return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}
// Packs the bytes [b], [g], [r], and [a] together as a D3DCOLOR integer
static inline constexpr uint32_t PackD3DCOLOR(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return PackUInt32(b, g, r, a);
}

#define gnu_attr(...) __attribute__((__VA_ARGS__))

#define vectorcall __vectorcall
#define fastcall __fastcall
#define regcall gnu_attr(regcall)

#define shufflevec __builtin_shufflevector
#define convertvec __builtin_convertvector

#pragma push_macro("forceinline")
#ifdef forceinline
#undef forceinline
#endif

#define forceinline [[clang::always_inline]]

template <typename T, size_t count, bool is_aligned>
struct _vec_impl {
    using type gnu_attr(__vector_size__(count * sizeof(T)), __aligned__(alignof(T))) = T;
};

template <typename T, size_t count>
struct _vec_impl<T, count, true> {
    using type gnu_attr(__vector_size__(count * sizeof(T))) = T;
};

template <typename T, size_t count, bool is_aligned = false>
using vec = typename _vec_impl<T, count, is_aligned>::type;

template <typename T, typename=void>
struct is_vector : std::false_type {};

template <typename T>
struct is_vector<T, std::void_t<decltype(std::declval<T>()[0])>> {
private:
    using B = std::remove_reference_t<decltype(std::declval<T>()[0])>;
public:
    static constexpr bool value = !(std::is_pointer_v<T> || std::is_array_v<T> || std::is_class_v<T> || std::is_union_v<T>) && sizeof(B) <= sizeof(T);
};

template <typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

//template <typename T>
//concept Vector = is_vector_v<T>;

template <typename T, typename=void>
struct vector_type {
    using type = void;
};

template <typename T>
struct vector_type<T, std::void_t<decltype(std::declval<T>()[0])>> {
    using type = std::remove_reference_t<decltype(std::declval<T>()[0])>;
};

template <typename T>
using vector_type_t = typename vector_type<T>::type;

template <typename T, typename=void>
struct vector_length {
    static constexpr size_t value = 0;
};

template <typename T>
struct vector_length<T, std::void_t<decltype(std::declval<T>()[0])>> {
    static constexpr size_t value = sizeof(T) / sizeof(vector_type_t<T>);
};

template <typename T>
inline constexpr size_t vector_length_v = vector_length<T>::value;

#if __SSSE3__
static inline constexpr bool has_ssse3 = true;
#else
static inline constexpr bool has_ssse3 = false;
#endif

#if __SSE4_1__
#define forceinlineSSE41 forceinline
static inline constexpr bool has_sse41 = true;
#else
#define forceinlineSSE41
static inline constexpr bool has_sse41 = false;
#endif

#if defined(__FMA__) || defined(__FMA4__)
#define forceinlineFMA forceinline
#else
#define forceinlineFMA
#endif

#if !__has_builtin(__builtin_elementwise_abs)
// Just make GCC shut up
#define __builtin_elementwise_abs(...) __VA_ARGS__
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Walign-mismatch" // This warning is BS, the alignment is specified
#pragma clang diagnostic ignored "-Wclass-conversion" // They're redundant because MACRO MAGIC and IDGAF
#pragma clang diagnostic ignored "-Wnarrowing" // why was this ever a warning
#pragma clang diagnostic ignored "-Winvalid-constexpr" // I'd rather just spam constexpr and let the compiler figure it out, especially as they update what is/isn't valid
#pragma clang diagnostic ignored "-Wmicrosoft-template"

// size: 0x4
struct Float1 {
    float x; // 0x0
    // 0x4

    inline constexpr Float1() {};
    inline constexpr Float1(float X) : x(X) {}
    template<typename T, sfinae_enable(is_vector_v<T> && std::is_same_v<float, vector_type_t<T>>)>
    inline constexpr Float1(const T& v) : x(v[0]) {}
};

#define Int1Declare(Name, Type) \
struct Name { \
    Type x; \
    \
    inline constexpr Name() {}; \
    inline constexpr Name(Type X) : x(X) {} \
    template<typename T, sfinae_enable(is_vector_v<T> && (std::is_same_v<int32_t, vector_type_t<T>> || std::is_same_v<uint32_t, vector_type_t<T>>))> \
    inline constexpr Name(const T& v) : x(v[0]) {} \
};
Int1Declare(Int1, int32_t);
Int1Declare(UInt1, uint32_t);
#undef Int1Declare

// size: 0x8
struct Float2 : Float1 {
    float y; // 0x4

    inline constexpr Float2() {};
    inline constexpr Float2(float X, float Y) : Float1(X), y(Y) {}
    template<typename T, sfinae_enable(is_vector_v<T> && std::is_same_v<float, vector_type_t<T>> && vector_length_v<T> >= 2)>
    inline constexpr Float2(const T& v) : Float1(v[0]), y(v[1]) {}

    inline constexpr vec<float, 2>& regcall as_vec2() const {
        return *(vec<float, 2>*)this;
    }

    inline constexpr Float2& vectorcall make_from_vector(float angle, float magnitude) {
        this->x = cosf(angle) * magnitude;
        this->y = sinf(angle) * magnitude;
        return *this;
    }

    inline constexpr float vectorcall angle_to() const {
        return atan2f(this->y, this->x);
    }

    inline constexpr float vectorcall angle_to(const Float2& coord) const {
#if __SSE3__
        return (*this - coord).angle_to();
#else
        return atan2f(this->y - coord.y, this->x - coord.x);
#endif
    }
    
    inline constexpr float regcall dot_product(const Float2& value) const {
#if __SSE3__
        vec<float, 2> temp = this->as_vec2() * value.as_vec2();
        return temp[0] + temp[1];
#else
        return this->x * value.x + this->y * value.y;
#endif
    }

    inline constexpr float regcall hypot_squared() const {
        return this->dot_product(*this);
    }

    inline constexpr float regcall hypot() const {
        return __builtin_sqrtf(this->hypot_squared());
    }

    forceinline constexpr Float2 regcall normalize() const {
        float scale_squared = this->hypot_squared();
        Float2 ret = *this;
        if (scale_squared >= 0.1f) {
#if !__FAST_MATH__
            ret /= __builtin_sqrtf(scale_squared);
#else
            ret *= 1.0f / __builtin_sqrtf(scale_squared);
#endif
        }
        return ret;
    }

    inline constexpr float regcall distance_squared(const Float2& value) const {
#if __SSE3__
        return (*this - value).hypot_squared();
#else
        float x_diff = this->x - value.x;
        float y_diff = this->y - value.y;
        return x_diff * x_diff + y_diff * y_diff;
#endif
    }

    inline constexpr float regcall distance(const Float2& value) const {
        return __builtin_sqrtf(this->distance_squared(value));
    }

    forceinline friend constexpr Float2 regcall fabs(const Float2& value) {
#if __has_builtin(__builtin_elementwise_abs)
        return __builtin_elementwise_abs(value.as_vec2());
#else
        return {
            __builtin_fabsf(value.x),
            __builtin_fabsf(value.y)
        };
#endif
    }
    forceinline friend constexpr Float2 regcall abs(const Float2& value) {
        return fabs(value);
    }
    forceinlineFMA friend constexpr Float2 regcall fma(const Float2& a, const Float2& b, const Float2& c) {
#if (defined(__FMA__) || defined(__FMA4__)) && __has_builtin(__builtin_elementwise_fma)
        return __builtin_elementwise_fma(a.as_vec2(), b.as_vec2(), c.as_vec2());
#else
        return {
            __builtin_fmaf(a.x, b.x, c.x),
            __builtin_fmaf(a.y, b.y, c.y)
        };
#endif
    }
    forceinline friend constexpr Float2 regcall fmax(const Float2& a, const Float2& b) {
        vec<float, 2> temp1 = a.as_vec2();
        vec<float, 2> temp2 = b.as_vec2();
        return temp1 > temp2 ? temp1 : temp2;
    }
    forceinline friend constexpr Float2 regcall fmin(const Float2& a, const Float2& b) {
        vec<float, 2> temp1 = a.as_vec2();
        vec<float, 2> temp2 = b.as_vec2();
        return temp1 < temp2 ? temp1 : temp2;
    }
    forceinline friend constexpr Float2 regcall (max)(const Float2& a, const Float2& b) {
        return fmax(a, b);
    }
    forceinline friend constexpr Float2 regcall (min)(const Float2& a, const Float2& b) {
        return fmin(a, b);
    }
    forceinline friend constexpr Float2 regcall sqrt(const Float2& value) {
#if __has_builtin(__builtin_elementwise_sqrt)
        return __builtin_elementwise_sqrt(value.as_vec2());
#else
        return {
            __builtin_sqrtf(value.x),
            __builtin_sqrtf(value.y)
        };
#endif
    }
    forceinline friend constexpr float regcall hypot(const Float2& a, const Float2& b) {
        return a.distance(b);
    }
    forceinlineSSE41 friend constexpr Float2 regcall ceil(const Float2& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_ceil)
        return __builtin_elementwise_ceil(value.as_vec2());
#else
        return {
            __builtin_ceilf(value.x),
            __builtin_ceilf(value.y)
        };
#endif
    }
    forceinlineSSE41 friend constexpr Float2 regcall floor(const Float2& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_floor)
        return __builtin_elementwise_floor(value.as_vec2());
#else
        return {
            __builtin_floorf(value.x),
            __builtin_floorf(value.y)
        };
#endif
    }
    forceinlineSSE41 friend constexpr Float2 regcall trunc(const Float2& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_trunc)
        return __builtin_elementwise_trunc(value.as_vec2());
#else
        return {
            __builtin_truncf(value.x),
            __builtin_truncf(value.y)
        };
#endif
    }
    forceinlineSSE41 friend constexpr Float2 regcall round(const Float2& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_round)
        return __builtin_elementwise_round(value.as_vec2());
#else
        return {
            __builtin_roundf(value.x),
            __builtin_roundf(value.y)
        };
#endif
    }
    forceinline friend constexpr Float2 regcall lerp(const Float2& v0, const Float2& v1, float t) {
#if defined(__FMA__) || defined(__FMA4__)
        return fma(v1 - v0, { t, t }, v0);
#else
        return v0 * (1.0f - t) + v1 * t;
#endif
    }

#define Float2BinOp(op) \
    forceinline constexpr Float2 regcall operator op(const Float2& value) const { \
        return this->as_vec2() op value.as_vec2(); \
    } \
    forceinline constexpr Float2 regcall operator op(float value) const { \
        return this->as_vec2() op value; \
    }
    Float2BinOp(+);
    Float2BinOp(-);
    Float2BinOp(*);
    Float2BinOp(/);
#undef Float2BinOp
#define Float2UnOp(op) \
    forceinline constexpr Float2 regcall operator op() const { \
        return op this->as_vec2(); \
    }
    Float2UnOp(+);
    Float2UnOp(-);
#undef Float2UnOp
#define Float2AssignOp(op) \
    inline constexpr Float2& regcall operator op(const Float2& value) {\
        this->as_vec2() op value.as_vec2(); \
        return *this; \
    } \
    inline constexpr Float2& regcall operator op(float value) {\
        this->as_vec2() op value; \
        return *this; \
    }
    Float2AssignOp(+=);
    Float2AssignOp(-=);
    Float2AssignOp(*=);
    Float2AssignOp(/=);
#undef Float2AssignOp
};

#define Int2BinOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op(const Name& value) const { \
        return this->as_vec2() op value.as_vec2(); \
    } \
    forceinline constexpr Name regcall operator op(Type value) const { \
        return this->as_vec2() op value; \
    }
#define Int2BinNoVecOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op(const Name& value) const { \
        return { \
            this->x op value.x, \
            this->y op value.y \
        }; \
    } \
    forceinline constexpr Name regcall operator op(Type value) const { \
        return { \
            this->x op value, \
            this->y op value \
        }; \
    }
#define Int2UnOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op() const { \
        return op this->as_vec2(); \
    }
#define Int2AssignOp(op, Name, Type, Base) \
    inline constexpr Name& regcall operator op(const Name& value) { \
        this->as_vec2() op value.as_vec2(); \
        return *this; \
    } \
    inline constexpr Name& regcall operator op(Type value) { \
        this->as_vec2() op value; \
        return *this; \
    }
#define Int2AssignNoVecOp(op, Name, Type, Base) \
    inline constexpr Name& regcall operator op(const Name& value) { \
        this->x op value.x; \
        this->y op value.y; \
        return *this; \
    } \
    inline constexpr Name& regcall operator op(Type value) { \
        this->x op value; \
        this->y op value; \
        return *this; \
    }
#if defined(__AVX2__) || defined(__XOP__)
#define Int2ShiftOp(op, Name, Type, Base) Int2BinOp(op, Name, Type, Base)
#define Int2ShiftAssignOp(op, Name, Type, Base) Int2AssignOp(op, Name, Type, Base)
#else
#define Int2ShiftOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op(const Name& value) const { \
        return { \
            this->x op value.x, \
            this->y op value.y \
        }; \
    } \
    forceinline constexpr Name regcall operator op(Type value) const { \
        return this->as_vec2() op value; \
    }
#define Int2ShiftAssignOp(op, Name, Type, Base) \
    inline constexpr Name& regcall operator op(const Name& value) { \
        this->x op value.x; \
        this->y op value.y; \
        return *this; \
    } \
    inline constexpr Name& regcall operator op(Type value) { \
        this->as_vec2() op value; \
        return *this; \
    }
#endif
#define Int2Abs() \
    forceinline friend constexpr Int2 regcall abs(const Int2& value) { \
        if constexpr (__has_builtin(__builtin_elementwise_abs)) { \
            return __builtin_elementwise_abs(value.as_vec2()); \
        } else { \
            return { \
                __builtin_abs(value.x), \
                __builtin_abs(value.y) \
            }; \
        }\
    }

#define Int2Declare(Name, Type, Base) \
struct Name : Base { \
    Type y; /* 0x4 */ \
    \
    inline constexpr Name() {}; \
    inline constexpr Name(Type X, Type Y) : Base(X), y(Y) {} \
    template<typename T, sfinae_enable(is_vector_v<T> && (std::is_same_v<int32_t, vector_type_t<T>> || std::is_same_v<uint32_t, vector_type_t<T>>) && vector_length_v<T> >= 2)> \
    inline constexpr Name(const T& v) : Base(v[0]), y(v[1]) {} \
    template<typename T, sfinae_enable(is_vector_v<T> && std::is_same_v<float, vector_type_t<T>> && vector_length_v<T> >= 2)> \
    inline constexpr Name(const T& v) : Name(convertvec(v, vec<Type, vector_length_v<T>>)) {} \
    \
    inline constexpr vec<Type, 2>& regcall as_vec2() const { \
        return *(vec<Type, 2>*)this; \
    } \
    inline constexpr vec<int32_t, 2>& regcall as_signed_vec2() const { \
        return *(vec<int32_t, 2>*)this; \
    } \
    inline constexpr vec<uint32_t, 2>& regcall as_unsigned_vec2() const { \
        return *(vec<uint32_t, 2>*)this; \
    } \
    forceinline constexpr regcall operator Int2() const; \
    forceinline constexpr regcall operator UInt2() const; \
    \
    Int2Abs(); \
    forceinline friend constexpr Name regcall (max)(const Name& a, const Name& b) { \
        vec<Type, 2> temp1 = a.as_vec2(); \
        vec<Type, 2> temp2 = b.as_vec2(); \
        return temp1 > temp2 ? temp1 : temp2; \
    } \
    forceinline friend constexpr Name regcall (min)(const Name& a, const Name& b) { \
        vec<Type, 2> temp1 = a.as_vec2(); \
        vec<Type, 2> temp2 = b.as_vec2(); \
        return temp1 < temp2 ? temp1 : temp2; \
    } \
    \
    Int2BinOp(+, Name, Type, Base); \
    Int2BinOp(-, Name, Type, Base); \
    Int2BinOp(*, Name, Type, Base); \
    Int2BinNoVecOp(/, Name, Type, Base); \
    Int2BinNoVecOp(%, Name, Type, Base); \
    Int2BinOp(&, Name, Type, Base); \
    Int2BinOp(|, Name, Type, Base); \
    Int2BinOp(^, Name, Type, Base); \
    Int2ShiftOp(<<, Name, Type, Base); \
    Int2ShiftOp(>>, Name, Type, Base); \
    Int2UnOp(+, Name, Type, Base); \
    Int2UnOp(-, Name, Type, Base); \
    Int2UnOp(~, Name, Type, Base); \
    Int2AssignOp(+=, Name, Type, Base); \
    Int2AssignOp(-=, Name, Type, Base); \
    Int2AssignOp(*=, Name, Type, Base); \
    Int2AssignNoVecOp(/=, Name, Type, Base); \
    Int2AssignNoVecOp(%=, Name, Type, Base); \
    Int2AssignOp(&=, Name, Type, Base); \
    Int2AssignOp(|=, Name, Type, Base); \
    Int2AssignOp(^=, Name, Type, Base); \
    Int2ShiftAssignOp(<<=, Name, Type, Base); \
    Int2ShiftAssignOp(>>=, Name, Type, Base); \
};

struct Int2;
struct UInt2;

Int2Declare(Int2, int32_t, Int1);
#undef Int2Abs
#define Int2Abs()
Int2Declare(UInt2, uint32_t, UInt1);

forceinline constexpr regcall Int2::operator UInt2() const {
    return *(UInt2*)this;
}
forceinline constexpr regcall UInt2::operator Int2() const {
    return *(Int2*)this;
}

#undef Int2Declare
#undef Int2BinOp
#undef Int2BinNoVecOp
#undef Int2ShiftOp
#undef Int2UnOp
#undef Int2AssignOp
#undef Int2AssignNoVecOp
#undef Int2ShiftAssignOp
#undef Int2Abs

// size: 0xC
struct Float3 : Float2 {
    float z; // 0x8

    inline constexpr Float3() {};
    inline constexpr Float3(float X, float Y, float Z) : Float2(X, Y), z(Z) {}
    inline constexpr Float3(const Float2& v) : Float2(v), z(0.0f) {}
    inline constexpr Float3(const Float2& v, float Z) : Float2(v), z(Z) {}
    inline constexpr Float3(const vec<float, 2>& v) : Float2(v), z(0.0f) {}
    inline constexpr Float3(const vec<float, 2>& v, float Z) : Float2(v), z(Z) {}
    template<typename T, sfinae_enable(is_vector_v<T> && std::is_same_v<float, vector_type_t<T>> && vector_length_v<T> >= 3)>
    inline constexpr Float3(const T& v) : Float2(v), z(v[2]) {}

    inline constexpr Float2& regcall as2() const {
        return *(Float2*)this;
    }

    forceinline constexpr vec<float, 4> regcall as_vec4() const {
        vec<float, 4> temp1 = { this->x, this->y };
        vec<float, 4> temp2 = { this->z, 0.0f };
        return shufflevec(temp1, temp2, 0, 1, 4, 5);
    }
    
    inline constexpr float regcall dot_product(const Float3& value) const {
// If fast math is off then this is better than scalar code.
// But if fast math is on then it tends to figure out something better.
#if !__FAST_MATH__ && __SSE3__
        vec<float, 4> temp = this->as_vec4() * value.as_vec4();
        return temp[0] + temp[1] + temp[2];
#else
        return this->x * value.x + this->y * value.y + this->z * value.z;
#endif
    }

    inline constexpr float regcall hypot_squared() const {
        return this->dot_product(*this);
    }

    inline constexpr float regcall hypot() const {
        return __builtin_sqrtf(this->hypot_squared());
    }

    forceinline constexpr Float3 regcall normalize() const {
        float scale_squared = this->hypot_squared();
        Float3 ret = *this;
        if (scale_squared >= 0.1f) {
#if !__FAST_MATH__
            ret *= 1.0f / __builtin_sqrtf(scale_squared);
#else
            ret /= __builtin_sqrtf(scale_squared);
#endif
        }
        return ret;
    }

    inline constexpr float regcall distance_squared(const Float3& value) const {
#if !__FAST_MATH__ && __SSE3__
        return (*this - value).hypot_squared();
#else
        float x_diff = this->x - value.x;
        float y_diff = this->y - value.y;
        float z_diff = this->z - value.z;
        return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
#endif
    }

    inline constexpr float regcall distance(const Float3& value) const {
        return __builtin_sqrtf(this->distance_squared(value));
    }


    forceinline friend constexpr Float3 regcall fabs(const Float3& value) {
#if !__FAST_MATH__ && __has_builtin(__builtin_elementwise_abs)
        return __builtin_elementwise_abs(value.as_vec4());
#else
        return {
            __builtin_fabsf(value.x),
            __builtin_fabsf(value.y),
            __builtin_fabsf(value.z)
        };
#endif
    }
    forceinline friend constexpr Float3 regcall abs(const Float3& value) {
        return fabs(value);
    }
    forceinlineFMA friend constexpr Float3 regcall fma(const Float3& a, const Float3& b, const Float3& c) {
#if (defined(__FMA__) || defined(__FMA4__)) && __has_builtin(__builtin_elementwise_fma)
        return __builtin_elementwise_fma(a.as_vec4(), b.as_vec4(), c.as_vec4());
#else
        return {
            __builtin_fmaf(a.x, b.x, c.x),
            __builtin_fmaf(a.y, b.y, c.y),
            __builtin_fmaf(a.z, b.z, c.z)
        };
#endif
    }
    forceinline friend constexpr Float3 regcall fmax(const Float3& a, const Float3& b) {
        vec<float, 4> temp1 = a.as_vec4();
        vec<float, 4> temp2 = b.as_vec4();
        return temp1 > temp2 ? temp1 : temp2;
    }
    forceinline friend constexpr Float3 regcall fmin(const Float3& a, const Float3& b) {
        vec<float, 4> temp1 = a.as_vec4();
        vec<float, 4> temp2 = b.as_vec4();
        return temp1 < temp2 ? temp1 : temp2;
    }
    forceinline friend constexpr Float3 regcall (max)(const Float3& a, const Float3& b) {
        return fmax(a, b);
    }
    forceinline friend constexpr Float3 regcall (min)(const Float3& a, const Float3& b) {
        return fmin(a, b);
    }
    forceinline friend constexpr Float3 regcall sqrt(const Float3& value) {
#if __has_builtin(__builtin_elementwise_sqrt)
        return __builtin_elementwise_sqrt(value.as_vec4());
#else
        return {
            __builtin_sqrtf(value.x),
            __builtin_sqrtf(value.y),
            __builtin_sqrtf(value.z)
        };
#endif
    }
    forceinline friend constexpr float regcall hypot(const Float3& a, const Float3& b) {
        return a.distance(b);
    }
    forceinlineSSE41 friend constexpr Float3 regcall ceil(const Float3& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_ceil)
        return __builtin_elementwise_ceil(value.as_vec4());
#else
        return {
            __builtin_ceilf(value.x),
            __builtin_ceilf(value.y),
            __builtin_ceilf(value.z)
        };
#endif
    }
    forceinlineSSE41 friend constexpr Float3 regcall floor(const Float3& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_floor)
        return __builtin_elementwise_floor(value.as_vec4());
#else
        return {
            __builtin_floorf(value.x),
            __builtin_floorf(value.y),
            __builtin_floorf(value.z)
        };
#endif
    }
    forceinlineSSE41 friend constexpr Float3 regcall trunc(const Float3& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_trunc)
        return __builtin_elementwise_trunc(value.as_vec4());
#else
        return {
            __builtin_truncf(value.x),
            __builtin_truncf(value.y),
            __builtin_truncf(value.z)
        };
#endif
    }
    forceinlineSSE41 friend constexpr Float3 regcall round(const Float3& value) {
#if defined(__SSE4_1__) && __has_builtin(__builtin_elementwise_round)
        return __builtin_elementwise_round(value.as_vec4());
#else
        return {
            __builtin_roundf(value.x),
            __builtin_roundf(value.y),
            __builtin_roundf(value.z)
        };
#endif
    }
    forceinline friend constexpr Float3 regcall lerp(const Float3& v0, const Float3& v1, float t) {
#if defined(__FMA__) || defined(__FMA4__)
        vec<float, 4> v0_vec = v0.as_vec4();
        return __builtin_elementwise_fma(v1.as_vec4() - v0_vec, (vec<float, 4>){ t, t, t, t }, v0_vec);
#else
        return v0 * (1.0f - t) + v1 * t;
#endif
    }
    
#define Float3BinOp(op) \
    forceinline constexpr Float3 regcall operator op(const Float3& value) const { \
        if constexpr (has_sse41) { \
            return this->as_vec4() op value.as_vec4(); \
        } else { \
            return { \
                this->as_vec2() op value.as_vec2(), \
                this->z op value.z \
            }; \
        } \
    } \
    template <typename T, sfinae_enable(std::is_same_v<T, Float2>)> \
    forceinline constexpr Float3 regcall operator op(const T& value) const { \
        return { \
            this->as_vec2() op value.as_vec2(), \
            this->z \
        }; \
    } \
    forceinline constexpr Float3 regcall operator op(float value) const { \
        if constexpr (has_sse41) { \
            return this->as_vec4() op value; \
        } else { \
            return { \
                this->as_vec2() op value, \
                this->z op value \
            }; \
        } \
    }
    Float3BinOp(+);
    Float3BinOp(-);
    Float3BinOp(*);
    Float3BinOp(/);
#undef Float3BinOp
#define Float3UnOp(op) \
    forceinline constexpr Float3 regcall operator op() const { \
        if constexpr (has_sse41) { \
            return op this->as_vec4(); \
        } else { \
            return { \
                op this->as_vec2(), \
                op this->z \
            }; \
        } \
    }
    Float3UnOp(+);
    Float3UnOp(-);
#undef Float3UnOp
#define Float3AssignOp(op1, op2) \
    inline constexpr Float3& regcall operator op1(const Float3& value) { \
        if constexpr (has_sse41) { \
            return *this = this->as_vec4() op2 value.as_vec4(); \
        } else { \
            this->as_vec2() op1 value.as_vec2(); \
            this->z op1 value.z; \
            return *this; \
        } \
    } \
    template<typename T, sfinae_enable(std::is_same_v<T, Float2>)> \
    inline Float3& regcall operator op1(const T& value) {\
        this->as_vec2() op1 value.as_vec2(); \
        return *this; \
    } \
    inline Float3& regcall operator op1(float value) {\
        if constexpr (has_sse41) { \
            return *this = this->as_vec4() op2 value; \
        } else { \
            this->as_vec2() op1 value; \
            this->z op1 value; \
            return *this; \
        } \
    }
    Float3AssignOp(+=, +);
    Float3AssignOp(-=, -);
    Float3AssignOp(*=, *);
    Float3AssignOp(/=, /);
#undef Float3AssignOp
};

#define Int3BinOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op(const Name& value) const { \
        if constexpr (has_sse41) { \
            return this->as_vec4() op value.as_vec4(); \
        } else { \
            return { \
                this->as_vec2() op value.as_vec2(), \
                this->z op value.z \
            }; \
        } \
    } \
    forceinline constexpr Name regcall operator op(const Base& value) const { \
        return { \
            this->as_vec2() op value.as_vec2(), \
            this->z \
        }; \
    } \
    forceinline constexpr Name regcall operator op(Type value) const { \
        if constexpr (has_sse41) { \
            return this->as_vec4() op value; \
        } else { \
            return { \
                this->as_vec2() op value, \
                this->z op value \
            }; \
        } \
    }
#define Int3BinNoVecOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op(const Name& value) const { \
        return { \
            this->x op value.x, \
            this->y op value.y, \
            this->z op value.z \
        }; \
    } \
    forceinline constexpr Name regcall operator op(const Base& value) const { \
        return { \
            this->x op value.x, \
            this->y op value.y, \
            this->z \
        }; \
    } \
    forceinline constexpr Name regcall operator op(Type value) const { \
        return { \
            this->x op value, \
            this->y op value, \
            this->z op value \
        }; \
    }
#define Int3UnOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op() const { \
        if constexpr (has_sse41) { \
            return op this->as_vec4(); \
        } else { \
            return { \
                op this->as_vec2(), \
                op this->z \
            }; \
        } \
    }
#define Int3AssignOp(op1, op2, Name, Type, Base) \
    inline constexpr Name& regcall operator op1(const Name& value) { \
        if constexpr (has_sse41) { \
            return *this = this->as_vec4() op2 value.as_vec4(); \
        } else { \
            this->as_vec2() op1 value.as_vec2(); \
            this->z op1 value.z; \
            return *this; \
        } \
    } \
    inline constexpr Name& regcall operator op1(const Base& value) { \
        this->as_vec2() op1 value.as_vec2(); \
        return *this; \
    } \
    inline constexpr Name& regcall operator op1(Type value) { \
        if constexpr (has_sse41) { \
            return *this = this->as_vec4() op2 value; \
        } else { \
            this->as_vec2() op1 value; \
            this->z op1 value; \
            return *this; \
        } \
    }
#define Int3AssignNoVecOp(op, Name, Type, Base) \
    inline constexpr Name& regcall operator op(const Name& value) { \
        this->x op value.x; \
        this->y op value.y; \
        this->z op value.z; \
        return *this; \
    } \
    inline constexpr Name& regcall operator op(const Base& value) { \
        this->x op value.x; \
        this->y op value.y; \
        return *this; \
    } \
    inline constexpr Name& regcall operator op(Type value) { \
        this->x op value; \
        this->y op value; \
        this->z op value; \
        return *this; \
    }
#if defined(__AVX2__) || defined(__XOP__)
#define Int3ShiftOp(op, Name, Type, Base) Int3BinOp(op, Name, Type, Base)
#define Int3ShiftAssignOp(op1, op2, Name, Type, Base) Int3AssignOp(op1, op2, Name, Type, Base)
#else
#define Int3ShiftOp(op, Name, Type, Base) \
    forceinline constexpr Name regcall operator op(const Name& value) const { \
        return { \
            this->x op value.x, \
            this->y op value.y, \
            this->z op value.z \
        }; \
    } \
    forceinline constexpr Name regcall operator op(const Base& value) const { \
        return { \
            this->x op value.x, \
            this->y op value.y, \
            this->z \
        }; \
    } \
    forceinline constexpr Name regcall operator op(Type value) const { \
        return this->as_vec4() op value; \
    }
#define Int3ShiftAssignOp(op1, op2, Name, Type, Base) \
    inline constexpr Name& regcall operator op1(const Name& value) { \
        this->x op1 value.x; \
        this->y op1 value.y; \
        this->z op1 value.z; \
        return *this; \
    } \
    inline constexpr Name& regcall operator op1(const Base& value) { \
        this->as_vec2() op1 value.as_vec2(); \
        return *this; \
    } \
    inline constexpr Name& regcall operator op1(Type value) { \
        if constexpr (has_sse41) { \
            return *this = this->as_vec4() op2 value; \
        } else { \
            this->as_vec2() op1 value; \
            this->z op1 value; \
            return *this; \
        } \
    }
#endif
#define Int3Abs() \
    forceinline friend constexpr Int3 regcall abs(const Int3& value) { \
        if constexpr (__has_builtin(__builtin_elementwise_abs)) { \
            return __builtin_elementwise_abs(value.as_vec2()); \
        } else { \
            return { \
                __builtin_abs(value.x), \
                __builtin_abs(value.y), \
                __builtin_abs(value.z) \
            }; \
        }\
    }

#define Int3Declare(Name, Type, Base) \
struct Name : Base { \
    Type z; /* 0x8 */ \
    \
    inline constexpr Name() {}; \
    inline constexpr Name(Type X, Type Y, Type Z) : Base(X, Y), z(Z) {} \
    inline constexpr Name(const Base& v) : Base(v), z(0) {} \
    inline constexpr Name(const Base& v, Type Z) : Base(v), z(Z) {} \
    inline constexpr Name(const vec<Type, 2>& v) : Base(v), z(0) {} \
    inline constexpr Name(const vec<Type, 2>& v, Type Z) : Base(v), z(Z) {} \
    template<typename T, sfinae_enable(is_vector_v<T> && (std::is_same_v<int32_t, vector_type_t<T>> || std::is_same_v<uint32_t, vector_type_t<T>>) && vector_length_v<T> >= 3)> \
    inline constexpr Name(const T& v) : Base(v), z(v[2]) {} \
    template<typename T, sfinae_enable(is_vector_v<T> && std::is_same_v<float, vector_type_t<T>> && vector_length_v<T> >= 3)> \
    inline constexpr Name(const T& v) : Name(convertvec(v, vec<Type, vector_length_v<T>>)) {} \
    \
    inline constexpr Base& regcall as2() const { \
        return *(Base*)this; \
    } \
    forceinline constexpr vec<Type, 4> regcall as_vec4() const { \
        vec<Type, 4> temp1 = { this->x, this->y }; \
        vec<Type, 4> temp2 = { this->z, 0 }; \
        return shufflevec(temp1, temp2, 0, 1, 4, 5); \
    } \
    \
    forceinline constexpr regcall operator Int3() const; \
    forceinline constexpr regcall operator UInt3() const; \
    \
    Int3Abs(); \
    forceinline friend constexpr Name regcall (max)(const Name& a, const Name& b) { \
        vec<Type, 2> temp1 = a.as_vec2(); \
        vec<Type, 2> temp2 = b.as_vec2(); \
        return temp1 > temp2 ? temp1 : temp2; \
    } \
    forceinline friend constexpr Name regcall (min)(const Name& a, const Name& b) { \
        vec<Type, 2> temp1 = a.as_vec2(); \
        vec<Type, 2> temp2 = b.as_vec2(); \
        return temp1 < temp2 ? temp1 : temp2; \
    } \
    \
    Int3BinOp(+, Name, Type, Base); \
    Int3BinOp(-, Name, Type, Base); \
    Int3BinOp(*, Name, Type, Base); \
    Int3BinNoVecOp(/, Name, Type, Base); \
    Int3BinNoVecOp(%, Name, Type, Base); \
    Int3BinOp(&, Name, Type, Base); \
    Int3BinOp(|, Name, Type, Base); \
    Int3BinOp(^, Name, Type, Base); \
    Int3ShiftOp(<<, Name, Type, Base); \
    Int3ShiftOp(>>, Name, Type, Base); \
    Int3UnOp(+, Name, Type, Base); \
    Int3UnOp(-, Name, Type, Base); \
    Int3UnOp(~, Name, Type, Base); \
    Int3AssignOp(+=, +, Name, Type, Base); \
    Int3AssignOp(-=, -, Name, Type, Base); \
    Int3AssignOp(*=, *, Name, Type, Base); \
    Int3AssignNoVecOp(/=, Name, Type, Base); \
    Int3AssignNoVecOp(%=, Name, Type, Base); \
    Int3AssignOp(&=, &, Name, Type, Base); \
    Int3AssignOp(|=, |, Name, Type, Base); \
    Int3AssignOp(^=, ^, Name, Type, Base); \
    Int3ShiftAssignOp(<<=, <<, Name, Type, Base); \
    Int3ShiftAssignOp(>>=, >>, Name, Type, Base); \
};

struct Int3;
struct UInt3;

Int3Declare(Int3, int32_t, Int2);
#undef Int3Abs
#define Int3Abs()
Int3Declare(UInt3, uint32_t, UInt2);

forceinline constexpr regcall Int3::operator UInt3() const {
    return *(UInt3*)this;
}
forceinline constexpr regcall UInt3::operator Int3() const {
    return *(Int3*)this;
}

#undef Int3Declare
#undef Int3BinOp
#undef Int3BinNoVecOp
#undef Int3ShiftOp
#undef Int3UnOp
#undef Int3AssignOp
#undef Int3AssignNoVecOp
#undef Int3ShiftAssignOp
#undef Int3Abs


__asm__(
#if TH_VER == EoSD_VER
"_GAME_SPEED = 0x6C6EC0 \n"
"_SLOWDOWN = 0x6C6EC4"
#elif TH_VER == PCB_VER
"_GAME_SPEED = 0x575AC8 \n"
"_SUPERVISOR_FLAGS = 0x575ADC"
#elif TH_VER == IN_VER
"_GAME_SPEED = 0x17CE8E0 \n"
"_SUPERVISOR_FLAGS = 0x17CE8FC"
#elif TH_VER == PoFV_VER
"_GAME_SPEED = 0x4B36B8 \n"
"_SUPERVISOR_FLAGS = 0x4B36D4"
#elif TH_VER == StB_VER
"_GAME_SPEED = 0x4BDED8"
#elif TH_VER == MoF_VER
"_GAME_SPEED = 0x476F78"
#elif TH_VER == UB_VER
"_GAME_SPEED = 0x4738E8"
#elif TH_VER == SA_VER
"_GAME_SPEED = 0x4A7948"
#elif TH_VER == UFO_VER
"_GAME_SPEED = 0x4B2ED0"
#elif TH_VER == DS_VER
"_GAME_SPEED = 0x4B5308"
#elif TH_VER == GFW_VER
"_GAME_SPEED = 0x4B6F58"
#elif TH_VER == TD_VER
"_GAME_SPEED = 0x4C0A28"
#elif TH_VER == DDC_VER
"_GAME_SPEED = 0x4D8F58"
#elif TH_VER == ISC_VER
"_GAME_SPEED = 0x4E40B0"
#elif TH_VER == LoLK_VER
"_GAME_SPEED = 0x4E73E8"
#elif TH_VER == HSiFS_VER
"_GAME_SPEED = 0x4A5788"
#elif TH_VER == VD_VER
"_GAME_SPEED = 0x4B39D0"
#elif TH_VER == WBaWC_VER
"_GAME_SPEED = 0x4B5918"
#elif TH_VER == UM_VER
"_GAME_SPEED = 0x4CCBF0"
#elif TH_VER == HBM_VER
"_GAME_SPEED = 0x4D0FE0"
#elif TH_VER == UDoALG_VER
""
#endif
);
extern "C" {
#if TH_VER != UDoALG_VER
    extern float GAME_SPEED asm("_GAME_SPEED");
#endif
#if TH_VER == EoSD_VER
    extern float SLOWDOWN asm("_SLOWDOWN");
#elif TH_VER < StB_VER
    extern struct {
        uint32_t : 5;
        uint32_t lag_frame : 1; // I'm just guessing at this name tbh
        uint32_t : 0;
    } SUPERVISOR_FLAGS asm("_SUPERVISOR_FLAGS");
#endif
}

#if TH_VER == UDoALG_VER
// Janky way of parsing a base relocation
static inline float& GAME_SPEED = *(float*)(((uintptr_t* gnu_attr(address_space(257))*)0)[12][2] + 0x1A356C);
#endif

#if __SSE4_1__
static inline constexpr float regcall fracf(float value) {
    return value - __builtin_truncf(value);
}
#else
static inline constexpr float regcall fracf(float value) {
    uint32_t bits = std::bit_cast<uint32_t>(value);
    uint8_t exp = bits >> 23;
    if (__builtin_expect(exp < 127, false)) {
        return value;
    }
    if (__builtin_expect(exp > 149, false)) {
        return 0.0f;
    }
    uint32_t shift = 150 - exp;
    //__asm__(""::"r"(shift));
    return value - std::bit_cast<float>(bits >> shift << shift);
}
#endif

struct Timer {
#if OLD_ENGINE
    // size: 0xC
    int32_t previous; // 0x0
#if TH_VER > IN_VER
    float current_f; // 0x4
#else
    float subframe; // 0x4
#endif
    int32_t current; // 0x8
    // 0xC
#else
    // size: 0x14
    int32_t previous; // 0x0
    int32_t current; // 0x4
    float current_f; // 0x8
#if NEW_ENGINE_A
    float* time_scale; // 0xC
#elif NEW_ENGINE_B
    uint32_t scale_table_index; // 0xC
#endif
    union {
        uint32_t flags; // 0x10
        struct {
            uint32_t initialized : 1;
        };
    };
    // 0x14
#endif

    inline constexpr Timer()
#if NEW_ENGINE
        : initialized(false)
#endif
    {}
    inline constexpr Timer(int32_t time) {
        this->set_raw(time);
#if NEW_ENGINE_A
        this->time_scale = &GAME_SPEED;
        this->initialized = true;
#elif NEW_ENGINE_B
        this->scale_table_index = 0;
        this->initialized = true;
#endif
    }
    inline constexpr Timer(float time) {
        this->set_raw(time);
#if NEW_ENGINE_A
        this->time_scale = &GAME_SPEED;
        this->initialized = true;
#elif NEW_ENGINE_B
        this->scale_table_index = 0;
        this->initialized = true;
#endif
    }

    inline constexpr Timer& regcall default_values() {
#if TH_VER > IN_VER
        this->previous = -999999;
        this->current = 0;
        this->current_f = 0.0f;
#else
        this->previous = -999;
        this->subframe = 0.0f;
        this->current = 0;
#endif
        return *this;
    }

    inline constexpr Timer& regcall initialize() {
#if OLD_ENGINE
        return this->default_values();
#else
        if (!this->initialized) {
            this->previous = -999999;
#if NEW_ENGINE_A
            this->time_scale = &GAME_SPEED;
            this->initialized = true;
#elif NEW_ENGINE_B
            this->scale_table_index = 0;
            this->initialized = true;
#endif
        }
        return *this;
#endif
    }

    inline constexpr Timer& regcall initialize_important() {
#if NEW_ENGINE_A
        this->time_scale = &GAME_SPEED;
        this->initialized = true;
#elif NEW_ENGINE_B
        this->scale_table_index = 0;
        this->initialized = true;
#endif
        return *this;
    }

    inline constexpr Timer& regcall set_raw(int32_t time) {
#if TH_VER > IN_VER
        this->current_f = this->current = time;
        this->previous = time - 1;
#else
        this->previous = (this->current = time) - 1;
        this->subframe = 0.0f;
#endif
        return *this;
    }

    inline constexpr Timer& regcall set_raw(float time) {
#if TH_VER > IN_VER
        this->previous = (this->current = this->current_f = time) - 1;
#else
        this->previous = (this->current = time) - 1;
        this->subframe = fracf(time);
#endif
        return *this;
    }

    gnu_attr(reinitializes) inline constexpr Timer& regcall set(int32_t time) {
        this->initialize_important();
        return this->set_raw(time);
    }
    gnu_attr(reinitializes) inline constexpr Timer& regcall set(float time) {
        this->initialize_important();
        return this->set_raw(time);
    }

    gnu_attr(reinitializes) inline constexpr Timer& regcall reset() {
        return this->set(0);
    }

protected:
#if TH_VER == PCB_VER || TH_VER == IN_VER || TH_VER == PoFV_VER
    // This is locked behind this purely so it doesn't clutter
    // the auto-complete menu with per-game stupid crap
    inline void handle_lag() {
        if (__builtin_expect(SUPERVISOR_FLAGS.lag_frame, false)) {
#if TH_VER == PCB_VER || TH_VER == IN_VER
            ++this->current;
            this->subframe = 0.0f;
#else
            this->current_f += this->current; // why?
#endif
        }
    }
#endif
public:

    inline constexpr Timer& regcall add_raw(float amount) {
#if TH_VER > IN_VER
        this->previous = this->current;
        this->current = this->current_f += amount;
#else
        float total = this->subframe + amount;
        this->current += (int32_t)total;
        this->subframe = fracf(total);
#endif
        return *this;
    }
    inline constexpr Timer& regcall sub_raw(float amount) {
#if TH_VER > IN_VER
        this->previous = this->current;
        this->current = this->current_f -= amount;
#else
        float total = this->subframe - amount;
        this->current += (int32_t)total;
        this->subframe = 1.0f + fracf(total); // This might be wrong
#endif
        return *this;
    }

    inline Timer& regcall add(float amount) {
#if TH_VER == PCB_VER || TH_VER == IN_VER
        this->handle_lag();
#endif
        float game_speed = GAME_SPEED;
        if (
#if TH_VER == EoSD_VER
            __builtin_expect(!(SLOWDOWN <= 0.99f), false)
#elif OLD_ENGINE
            __builtin_expect(!(game_speed <= 0.99f), false)
#else
            __builtin_expect(!(game_speed > 0.99f) || !(game_speed < 1.01f), false)
#endif
        ) {
            amount *= game_speed;
        }
        this->add_raw(amount);
#if TH_VER == PoFV_VER
        this->handle_lag();
#endif
        return *this;
    }

    inline Timer& regcall add(int32_t amount) {
#if TH_VER > IN_VER
        return this->add((float)amount);
#else
#if TH_VER == PCB_VER || TH_VER == IN_VER
        this->handle_lag();
#endif
        float game_speed = GAME_SPEED;
        if (
#if TH_VER == EoSD_VER
            __builtin_expect(!(SLOWDOWN <= 0.99f), true)
#else
            __builtin_expect(!(game_speed <= 0.99f), true)
#endif
        ) {
            this->current += amount;
            return *this;
        }
        return this->add_raw((float)amount * game_speed);
#endif
    }

    inline Timer& regcall tick() {
#if TH_VER > IN_VER
        return this->add(1.0f);
#else
        // Notice that there's no lag frame handling
        float game_speed = GAME_SPEED;
        if (
#if TH_VER == EoSD_VER
            __builtin_expect(!(SLOWDOWN <= 0.99f), true)
#else
            __builtin_expect(!(game_speed <= 0.99f), true)
#endif
        ) {
            ++this->current;
            return *this;
        }
        return this->add_raw(game_speed);
#endif
    }


    inline Timer& regcall sub(float amount) {
#if TH_VER == PCB_VER || TH_VER == IN_VER
        this->handle_lag();
#endif
        float game_speed = GAME_SPEED;
        if (
#if TH_VER == EoSD_VER
            __builtin_expect(!(SLOWDOWN <= 0.99f), false)
#elif OLD_ENGINE
            __builtin_expect(!(game_speed <= 0.99f), false)
#else
            __builtin_expect(!(game_speed > 0.99f) || !(game_speed < 1.01f), false)
#endif
        ) {
            amount *= game_speed;
        }
        return this->sub_raw(amount);
    }

    inline Timer& regcall sub(int32_t amount) {
#if TH_VER > IN_VER
        return this->sub((float)amount);
#else
#if TH_VER == PCB_VER || TH_VER == IN_VER
        this->handle_lag();
#endif
        float game_speed = GAME_SPEED;
        if (
#if TH_VER == EoSD_VER
            __builtin_expect(!(SLOWDOWN <= 0.99f), true)
#else
            __builtin_expect(!(game_speed <= 0.99f), true)
#endif
        ) {
            this->current -= amount;
            return *this;
        }
        return this->sub_raw((float)amount * game_speed);
#endif
    }

    inline constexpr bool vectorcall is_multiple_of(int32_t amount) const {
        return !(this->current % amount);
    }
    inline constexpr bool vectorcall operator%(int32_t amount) const {
        return this->is_multiple_of(amount);
    }

    inline constexpr bool vectorcall les(int32_t time) const {
        return this->current < time;
    }
    inline constexpr bool vectorcall les(float time) const {
        return this->operator float() < time;
    }
    inline constexpr bool vectorcall leq(int32_t time) const {
        return this->current <= time;
    }
    inline constexpr bool vectorcall leq(float time) const {
        return this->operator float() <= time;
    }
    inline constexpr bool vectorcall gre(int32_t time) const {
        return this->current > time;
    }
    inline constexpr bool vectorcall gre(float time) const {
        return this->operator float() > time;
    }
    inline constexpr bool vectorcall geq(int32_t time) const {
        return this->current >= time;
    }
    inline constexpr bool vectorcall geq(float time) const {
        return this->operator float() >= time;
    }
    inline constexpr bool vectorcall equ(int32_t time) const {
        return this->current == time;
    }
    inline constexpr bool vectorcall equ(float time) const {
        return this->operator float() == time;
    }
    inline constexpr bool vectorcall neq(int32_t time) const {
        return this->current != time;
    }
    inline constexpr bool vectorcall neq(float time) const {
        return this->operator float() != time;
    }
    inline constexpr bool vectorcall is_zero() const {
        return this->equ(0);
    }
    inline constexpr bool vectorcall is_not_zero() const {
        return this->neq(0);
    }

    inline constexpr vectorcall operator int32_t() const {
        return this->current;
    }
    inline constexpr vectorcall operator float() const {
#if TH_VER > IN_VER
        return this->current_f;
#else
        return this->current + this->subframe;
#endif
    }

    gnu_attr(reinitializes) inline constexpr Timer& regcall operator=(int32_t time) {
        return this->set(time);
    }
    gnu_attr(reinitializes) inline constexpr Timer& regcall operator=(float time) {
        return this->set(time);
    }
    inline Timer& regcall operator+=(int32_t time) {
        return this->add(time);
    }
    inline Timer& regcall operator+=(float time) {
        return this->add(time);
    }
    inline Timer& regcall operator-=(int32_t time) {
        return this->sub(time);
    }
    inline Timer& regcall operator-=(float time) {
        return this->sub(time);
    }

    // Old engine games *don't* handle lag frames ++
    inline Timer& regcall operator++() {
        return this->tick();
    }
    inline void regcall operator++(int) {
        this->operator++();
    }

    // Old engines games *do* handle lag frames for --
    inline Timer& regcall operator--() {
#if TH_VER > IN_VER
        return this->sub(1.0f);
#else
        return this->sub(1);
#endif
    }
    inline void regcall operator--(int) {
        this->operator--();
    }


    inline constexpr bool vectorcall operator<(int32_t time) const {
        return this->les(time);
    }
    inline constexpr bool vectorcall operator<(float time) const {
        return this->les(time);
    }
    inline constexpr bool vectorcall operator<=(int32_t time) const {
        return this->leq(time);
    }
    inline constexpr bool vectorcall operator<=(float time) const {
        return this->leq(time);
    }
    inline constexpr bool vectorcall operator>(int32_t time) const {
        return this->gre(time);
    }
    inline constexpr bool vectorcall operator>(float time) const {
        return this->gre(time);
    }
    inline constexpr bool vectorcall operator>=(int32_t time) const {
        return this->geq(time);
    }
    inline constexpr bool vectorcall operator>=(float time) const {
        return this->geq(time);
    }
    inline constexpr bool vectorcall operator==(int32_t time) const {
        return this->equ(time);
    }
    inline constexpr bool vectorcall operator==(float time) const {
        return this->equ(time);
    }
    inline constexpr bool vectorcall operator!=(int32_t time) const {
        return this->neq(time);
    }
    inline constexpr bool vectorcall operator!=(float time) const {
        return this->neq(time);
    }
    inline constexpr bool vectorcall operator!() const {
        return this->is_zero();
    }
    inline constexpr vectorcall operator bool() const {
        return this->is_not_zero();
    }
};

// Interpolator structs didn't exist in old engine games, but this should work anyway
//#if NEW_ENGINE

enum InterpMode : int32_t {
    Linear = 0,
    AccelerateSlow = 1,
    Accelerate = 2,
    AccelerateFast = 3,
    DecelerateSlow = 4,
    Decelerate = 5,
    DecelerateFast = 6,
    ConstantVelocity = 7,
    Bezier = 8,
    AccelDecelSlow = 9,
    AccelDecel = 10,
    AccelDecelFast = 11,
    DecelAccelSlow = 12,
    DecelAccel = 13,
    DecelAccelFast = 14,
    ForceInitial = 15,
    ForceFinal = 16,
    ConstantAccel = 17,
    DecelerateSine = 18,
    AccelerateSine = 19,
    DecelAccelSine = 20,
    AccelDecelSine = 21,
    AccelParabolaA = 22,
    AccelParabolaB = 23,
    AccelParabolaC = 24,
    AccelParabolaD = 25,
    AccelParabolaE = 26,
    DecelParabolaA = 27,
    DecelParabolaB = 28,
    DecelParabolaC = 29,
    DecelParabolaD = 30,
    DecelParabolaE = 31
};

static inline constexpr const float PI_f = M_PI;
static inline constexpr const float HALF_PI_f = M_PI_2;
static inline constexpr const float TWO_PI_f = M_PI * 2.0;

static
#if __cpp_constexpr >= 202110L
constexpr
#endif
float vectorcall interpolate1D(InterpMode mode, float current_time, float end_time) {
    if (__builtin_expect(!(end_time == 0.0f), true)) {
        float value = current_time / end_time;
        switch (mode) {
            default:
                __builtin_unreachable();
            case Linear:
                return value;
            case AccelDecelSlow:
                if (!(value < 0.5f)) goto decelerate_slow;
                [[fallthrough]];
            case AccelerateSlow: accelerate_slow:
                return value * value;
            case AccelDecel:
                if (!(value < 0.5f)) goto decelerate;
                [[fallthrough]];
            case Accelerate: accelerate:
                return value * value * value;
            case AccelDecelFast:
                if (!(value < 0.5f)) goto decelerate_fast;
                [[fallthrough]];
            case AccelerateFast: accelerate_fast:
                return value * value * value * value;
            case DecelAccelSlow:
                if (!(value < 0.5f)) goto accelerate_slow;
                [[fallthrough]];
            case DecelerateSlow: decelerate_slow:
                value = 1.0f - value;
                value = value * value;
                return 1.0f - value;
            case DecelAccel:
                if (!(value < 0.5f)) goto accelerate;
                [[fallthrough]];
            case Decelerate: decelerate:
                value = 1.0f - value;
                value = value * value * value;
                return 1.0f - value;
            case DecelAccelFast:
                if (!(value < 0.5f)) goto accelerate_fast;
                [[fallthrough]];
            case DecelerateFast: decelerate_fast:
                value = 1.0f - value;
                value = value * value * value * value;
                return 1.0f - value;
            case ForceInitial:
                return 0.0f;
            case ForceFinal:
                break;
            case DecelerateSine:
                return sinf(value * HALF_PI_f);
            case AccelerateSine:
                return 1.0f - cosf(value * HALF_PI_f);
            
            // These look like they were probably meant to be simple back-forth switches between
            // the two sine modes, but ZUN didn't write them correctly because of how the original
            // halfway point test was structured.
            case DecelAccelSine: {
                float temp = sinf((value + value) * HALF_PI_f);
                if (!(value < 0.5f)) {
                    temp = 1.0f - temp + 1.0f;
                }
                return temp * 0.5f;
            }
            case AccelDecelSine:
                return (1.0f - cosf((value + value) * HALF_PI_f)) * 0.5f;

            case AccelParabolaA:
                value -= 0.25f;
                value *= value;
                return value + value - 0.125f;
            case AccelParabolaB:
                value -= 0.3f;
                return value * value * 2.5f - 0.225f;
            case AccelParabolaC:
                value -= 0.35f;
                return value * value * 3.333333f - 0.4083333f;
            case AccelParabolaD:
                value -= 0.38f;
                return value * value * 4.166666f - 0.6016666f;
            case AccelParabolaE:
                value -= 0.4f;
                return value * value * 5.0f - 0.8f;
            case DecelParabolaA:
                value = 0.75f - value;
                value *= value;
                return 1.0f - (value + value - 0.125f);
            case DecelParabolaB:
                value = 0.7f - value;
                return 1.0f - (value * value * 2.5f - 0.225f);
            case DecelParabolaC:
                value = 0.65f - value;
                return 1.0f - (value * value * 3.333333f - 0.4083333f);
            case DecelParabolaD:
                value = 0.62f - value;
                return 1.0f - (value * value * 4.166666f - 0.6016666f);
            case DecelParabolaE:
                value = 0.6f - value;
                return 1.0f - (value * value * 5.0f - 0.8f);
        }
    }
    return 1.0f;
}

template<typename T>
forceinline static constexpr T regcall ZUNlerp(const T& initial_val, const T& final_val, float time) {
    if constexpr (std::is_same_v<T, Int2> || std::is_same_v<T, UInt2>) {
        return convertvec(initial_val.as_vec2(), vec<float, 2>) + convertvec((final_val - initial_val).as_vec2(), vec<float, 2>) * time;
    }
    else if constexpr (std::is_same_v<T, Int3> || std::is_same_v<T, UInt3>) {
        return convertvec(initial_val.as_vec4(), vec<float, 4>) + convertvec((final_val - initial_val).as_vec4(), vec<float, 4>) * time;
    }
    else {
        return initial_val + (final_val - initial_val) * time;
    }
}

template<typename T>
forceinline static constexpr T regcall ZUNbezier(const T& initial_val, const T& final_val, const T& bezier1, const T& bezier2, float progress) {
    float A = 1.0f - progress;
    float B = progress - 1.0f;
    float bezier1_factor = A * A * progress;
    float bezier2_factor = B * progress * progress;
    float final_factor = progress * progress * (3.0f - (progress + progress));
    float initial_factor = B * B * (1.0f + progress + progress);

    if constexpr (std::is_same_v<T, Int2> || std::is_same_v<T, UInt2>) {
        return convertvec(initial_val.as_vec2(), vec<float, 2>) * initial_factor +
               convertvec(final_val.as_vec2(), vec<float, 2>) * final_factor +
               convertvec(bezier1.as_vec2(), vec<float, 2>) * bezier1_factor +
               convertvec(bezier2.as_vec2(), vec<float, 2>) * bezier2_factor;
    }
    else if constexpr (std::is_same_v<T, Int3> || std::is_same_v<T, UInt3>) {
        return convertvec(initial_val.as_vec4(), vec<float, 4>) * initial_factor +
               convertvec(final_val.as_vec4(), vec<float, 4>) * final_factor +
               convertvec(bezier1.as_vec4(), vec<float, 4>) * bezier1_factor +
               convertvec(bezier2.as_vec4(), vec<float, 4>) * bezier2_factor;
    }
    else {
        return initial_val * initial_factor + final_val * final_factor + bezier1 * bezier1_factor + bezier2 * bezier2_factor;
    }
}

template <typename T, size_t modes = 1>
struct ZUNInterp { //           0x58    0x44    0x30
    //                          T3      T2      T1
    T initial_value; //         0x0     0x0     0x0
    T final_value; //           0xC     0x8     0x4
    T bezier1; //               0x18    0x10    0x8
    T bezier2; //               0x24    0x18    0xC
    T current; //               0x30    0x20    0x10
    Timer time; //              0x3C    0x28    0x14
    int32_t end_time; //        0x50    0x3C    0x28
    InterpMode mode[modes]; //  0x54    0x40    0x2C

    gnu_attr(reinitializes) constexpr ZUNInterp& regcall initialize(int32_t end_time, InterpMode mode, const T& initial_value, const T& final_value) {
        this->current = this->initial_value = initial_value;
        this->final_value = final_value;
        this->bezier1 = this->bezier2 = {};
        this->time.reset();
        this->end_time = end_time;
        this->mode[0] = mode;
        return *this;
    }

    gnu_attr(reinitializes) constexpr ZUNInterp& regcall initialize_bezier(int32_t end_time, const T& initial_value, const T& final_value, const T& bezier1_value, const T& bezier2_value) {
        this->current = this->initial_value = initial_value;
        this->final_value = final_value;
        this->bezier1 = bezier1_value;
        this->bezier2 = bezier2_value;
        this->time.reset();
        this->end_time = end_time;
        this->mode[0] = Bezier;
        return *this;
    }

    gnu_attr(reinitializes) constexpr ZUNInterp& regcall initialize_constant_velocity(int32_t end_time, const T& initial_value, const T& velocity) {
        this->current = this->initial_value = initial_value;
        this->final_value = velocity;
        this->bezier1 = this->bezier2 = {};
        this->time.reset();
        this->end_time = end_time;
        this->mode[0] = ConstantVelocity;
        return *this;
    }

    gnu_attr(reinitializes) constexpr ZUNInterp& regcall initialize_constant_accel(int32_t end_time, const T& initial_value, const T& initial_velocity, const T& acceleration) {
        this->current = this->initial_value = initial_value;
        this->final_value = acceleration;
        this->bezier1 = {};
        this->bezier2 = initial_velocity;
        this->time.reset();
        this->end_time = end_time;
        this->mode[0] = ConstantAccel;
        return *this;
    }

    inline constexpr ZUNInterp& regcall set_initial_value(const T& value) {
        this->initial_value = value;
        return *this;
    }
    inline constexpr ZUNInterp& regcall set_final_value(const T& value) {
        this->final_value = value;
        return *this;
    }
    inline constexpr ZUNInterp& regcall set_bezier1(const T& value) {
        this->bezier1 = value;
        return *this;
    }
    inline constexpr ZUNInterp& regcall set_bezier2(const T& value) {
        this->bezier2 = value;
        return *this;
    }
    inline constexpr ZUNInterp& regcall set_current(const T& value) {
        this->current = value;
        return *this;
    }
    inline constexpr ZUNInterp& regcall reset_timer() {
        this->time.reset();
        return *this;
    }
    inline constexpr ZUNInterp& regcall set_end_time(int32_t time) {
        this->end_time = time;
        return *this;
    }
    inline constexpr ZUNInterp& regcall reset_end_time() {
        return this->set_end_time(0);
    }
    inline constexpr ZUNInterp& regcall set_mode(InterpMode mode) {
        this->mode[0] = mode;
        return *this;
    }

    T vectorcall step() {
        int32_t end_time = this->end_time;
        switch (end_time) {
            case 1 ... INT32_MAX:
                if (__builtin_expect(++this->time < end_time, true)) break;
                this->time = end_time;
                [[fallthrough]];
            case 0:
                switch (__builtin_expect(this->mode[0], Linear)) {
                    default:
                        return this->final_value;
                    case ConstantVelocity:
                    case ConstantAccel:
                        return this->initial_value;
                }
        }
        switch (InterpMode mode; __builtin_expect(mode = this->mode[0], Linear)) {
            default:
                return this->current = ZUNlerp(this->initial_value, this->final_value, interpolate1D(mode, (float)this->time, (float)end_time));
            case ConstantVelocity:
                return this->current = this->initial_value += this->final_value;
            case ConstantAccel:
                return this->current = this->initial_value += this->bezier2 += this->final_value;
            case Bezier:
                return this->current = ZUNbezier(this->initial_value, this->final_value, this->bezier1, this->bezier2, (float)this->time / (float)end_time);
        }
    }
};

//#endif

template <typename T>
struct ZUNList {
    T* data; // 0x0
    ZUNList* next; // 0x4
    ZUNList* prev; // 0x8
#if NEW_ENGINE
    ZUNList* __ptr_C; // 0xC
#endif
    // 0x10

    inline constexpr ZUNList() {}
    inline constexpr ZUNList(T* data)
        : data(data)
        , next(NULL)
        , prev(NULL)
#if NEW_ENGINE
        , __ptr_C(NULL)
#endif
    {}

    gnu_attr(reinitializes) inline constexpr void initialize_with(T* data) {
        this->data = data;
        this->next = NULL;
        this->prev = NULL;
#if NEW_ENGINE
        this->__ptr_C = NULL;
#endif
    }

    inline constexpr void unlink_from_next() const {
		if (ZUNList* next_node = this->next) {
			next_node->prev = this->prev;
		}
	}
	inline constexpr void unlink_from_prev() const {
		if (ZUNList* prev_node = this->prev) {
			prev_node->next = this->next;
		}
	}

	inline constexpr void unlink() {
        this->unlink_from_next();
        this->unlink_from_prev();
		this->next = NULL;
		this->prev = NULL;
	}

	inline constexpr void unlink_from_head(ZUNList*& head_node) const {
		if (this == head_node) {
			head_node = this->next;
		}
	}

	inline constexpr void unlink_from_tail(ZUNList*& tail_node) const {
		if (this == tail_node) {
			tail_node = this->prev;
		}
	}

	inline constexpr void prepend(ZUNList* new_node) {
		new_node->next = this;
		this->prev = new_node;
	}

    inline constexpr void append(ZUNList* new_node) {
		if (ZUNList* next_node = this->next) {
			next_node->prepend(new_node);
		}
#if NEW_ENGINE
        // __ptr_C operation would go here but nobody cares
#endif
		new_node->prepend(this);
	}

    inline constexpr ZUNList* find_end_node(ZUNList* node) const {
        while (ZUNList* next_node = node->next) node = next_node;
        return node;
    }

protected:
    // The iterators are split like this because clang
    // assumes that "this" can never be NULL when calling
    // a function, so the base variant of the list node
    // needs a non-member function to call when passing in
    // the "next" pointer.
    static inline constexpr void delete_each_impl(ZUNList* node) {
		for (ZUNList* next_node; node; node = next_node) {
			next_node = node->next;
			delete node->data;
			delete node;
		}
	}
	template <typename L>
	static inline constexpr void for_each_impl(const L& lambda, ZUNList* node) {
		for (; node; node = node->next) {
            if constexpr (std::is_invocable_v<L, T*, ZUNList*>) {
				lambda(node->data, node);
			} else {
				lambda(node->data);
			}
		}
	}
	template <typename L>
	static inline constexpr bool do_while_impl(const L& lambda, ZUNList* node) {
		for (; node; node = node->next) {
            if constexpr (std::is_invocable_v<L, T*, ZUNList*>) {
                if (!lambda(node->data, node)) return false;
            } else {
                if (!lambda(node->data)) return false;
            }
		}
		return true;
	}
	template <typename L>
	static inline constexpr T* find_if_impl(const L& lambda, ZUNList* node) {
		for (; node; node = node->next) {
            if constexpr (std::is_invocable_v<L, T*, ZUNList*>) {
                if (T* data = node->data; lambda(data, node)) return data;
            } else {
                if (T* data = node->data; lambda(data)) return data;
            }
		}
		return NULL;
	}
	template <typename L>
	static inline constexpr T* find_if_not_impl(const L& lambda, ZUNList* node) {
		for (; node; node = node->next) {
            if constexpr (std::is_invocable_v<L, T*, ZUNList*>) {
                if (T* data = node->data; !lambda(data, node)) return data;
            } else {
                if (T* data = node->data; !lambda(data)) return data;
            }
		}
		return NULL;
	}
	template <typename L>
	static inline constexpr ZUNList* find_node_if_impl(const L& lambda, ZUNList* node) {
		for (; node; node = node->next) {
            if constexpr (std::is_invocable_v<L, T*, ZUNList*>) {
                if (T* data = node->data; lambda(data, node)) break;
            } else {
                if (T* data = node->data; lambda(data)) break;
            }
		}
		return node;
	}
	template <typename L>
	static inline constexpr size_t count_if_impl(const L& lambda, ZUNList* node) {
		size_t ret = 0;
		for (; node; node = node->next) {
            if constexpr (std::is_invocable_v<L, T*, ZUNList*>) {
                if (T* data = node->data; lambda(data, node)) ++ret;
            } else {
                if (T* data = node->data; lambda(data)) ++ret;
            }
		}
		return ret;
	}
	template <typename L>
	static inline constexpr size_t count_if_not_impl(const L& lambda, ZUNList* node) {
		size_t ret = 0;
		for (; node; node = node->next) {
            if constexpr (std::is_invocable_v<L, T*, ZUNList*>) {
                if (T* data = node->data; !lambda(data, node)) ++ret;
            } else {
                if (T* data = node->data; !lambda(data)) ++ret;
            }
		}
		return ret;
	}
public:
	inline constexpr void delete_each() {
		return delete_each_impl(this);
	}
	template <typename L>
	inline constexpr void for_each(const L& lambda) {
		return for_each_impl(lambda, this);
	}
	template <typename L>
	inline constexpr bool do_while(const L& lambda) {
		return do_while_impl(lambda, this);
	}
	template <typename L>
	inline constexpr T* find_if(const L& lambda) {
		return find_if_impl(lambda, this);
	}
	template <typename L>
	inline constexpr T* find_if_not(const L& lambda) {
		return find_if_not_impl(lambda, this);
	}
	template <typename L>
	inline ZUNList* find_node_if(const L& lambda) {
		return find_node_if_impl(lambda, this);
	}
	template <typename L>
	inline constexpr size_t count_if(const L& lambda) {
		return count_if_impl(lambda, this);
	}
	template <typename L>
	inline constexpr size_t count_if_not(const L& lambda) {
		return count_if_not_impl(lambda, this);
	}
};

template <typename T>
struct ZUNListRoot : ZUNList<T> {

    inline constexpr void append(ZUNList<T>* new_node) {
        this->find_end_node()->append(new_node);
    }

    inline constexpr void delete_each() const {
		return delete_each_impl(this->next);
	}
	template <typename L>
	inline constexpr void for_each(const L& lambda) const {
		return for_each_impl(lambda, this->next);
	}
	template <typename L>
	inline constexpr bool do_while(const L& lambda) const {
		return do_while_impl(lambda, this->next);
	}
	template <typename L>
	inline constexpr T* find_if(const L& lambda) const {
		return find_if_impl(lambda, this->next);
	}
	template <typename L>
	inline constexpr T* find_if_not(const L& lambda) const {
		return find_if_not_impl(lambda, this->next);
	}
	template <typename L>
	inline constexpr ZUNList<T>* find_node_if(const L& lambda) const {
		return find_node_if_impl(lambda, this->next);
	}
	template <typename L>
	inline constexpr size_t count_if(const L& lambda) const {
		return count_if_impl(lambda, this->next);
	}
	template <typename L>
	inline constexpr size_t count_if_not(const L& lambda) const {
		return count_if_not_impl(lambda, this->next);
	}
};

#pragma clang diagnostic pop

#pragma pop_macro("forceinline")

#endif