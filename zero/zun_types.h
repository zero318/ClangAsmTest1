#ifndef ZUN_TYPES_H
#define ZUN_TYPES_H 1

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

#define DECLARED_FLOAT_STUCTS 1

// size: 0x4
struct Int1 {
    int32_t x; // 0x0
    // 0x4
};

// size: 0x8
struct Int2 : Int1 {
    int32_t y; // 0x4
    // 0x8
};

// size: 0xC
struct Int3 : Int2 {
    int32_t z; // 0x8
    // 0xC
};

// size: 0x10
struct Int4 : Int3 {
    int32_t w; // 0xC
    // 0x10
};

#if GAME_VERSION == EoSD_VER
#define zun_operator_inline forceinline
#define zun_make_from_vector_inline forceinline
#elif GAME_VERSION == PCB_VER || GAME_VERSION == IN_VER
#define zun_operator_inline gnu_noinline
#define zun_make_from_vector_inline forceinline
#else
#define zun_operator_inline inline
#define zun_make_from_vector_inline gnu_noinline
#endif

// size: 0x4
struct Float1 {
    float x; // 0x0
    // 0x4

    inline constexpr Float1() {};
    inline constexpr Float1(const float& X) : x(X) {}
};

// size: 0x8
struct Float2 : Float1 {
    float y; // 0x4

    using Float1::Float1;
    inline constexpr Float2() {};
    inline constexpr Float2(const float& X, const float& Y) : Float1(X), y(Y) {}
    
    // th18 A: 0x404DD0
    // th18 H: 0x429BC0
    dllexport zun_make_from_vector_inline Float2& thiscall make_from_vector(float angle, float magnitude) {
#ifndef __x86_64__
        __asm {
            MOV EAX, this;
            FLD angle;
            FSINCOS;
            FMUL magnitude;
            FSTP DWORD PTR [EAX]
            FMUL magnitude;
            FSTP DWORD PTR [EAX+4]
        };
#else
        __asm {
            MOV RAX, this;
            FLD angle;
            FSINCOS;
            FMUL magnitude;
            FSTP DWORD PTR [RAX]
            FMUL magnitude;
            FSTP DWORD PTR [RAX+4]
        };
#endif
        return *this;
    }
    
    inline float angle_to(const Float2& coord) {
        return zatan2f(this->y - coord.y, this->x - coord.x);
    }
    
#pragma region // Float2 Operators

#define Float2BinOp(op) \
    zun_operator_inline Float2 operator op(const Float2& value) { \
        return { \
            this->x op value.x, \
            this->y op value.y \
        }; \
    }
    Float2BinOp(+);
    Float2BinOp(-);
    Float2BinOp(*);
    Float2BinOp(/);
#undef Float2BinOp
#define Float2BinOpScalar(op) \
    zun_operator_inline Float2 operator op(const float& value) { \
        return { \
            this->x op value, \
            this->y op value \
        }; \
    }
    Float2BinOpScalar(+);
    Float2BinOpScalar(-);
    Float2BinOpScalar(*);
    Float2BinOpScalar(/);
#undef Float2BinOpScalar
#define Float2UnOp(op) \
    zun_operator_inline Float2 operator op() { \
        return { \
            op this->x, \
            op this->y \
        }; \
    }
    Float2UnOp(+);
    Float2UnOp(-);
#undef Float2UnOp
#define Float2AssignOp(op) \
    zun_operator_inline Float2& operator op(const Float2& value) {\
        this->x op value.x; \
        this->y op value.y; \
        return *this; \
    }
    Float2AssignOp(+=);
    Float2AssignOp(-=);
    Float2AssignOp(*=);
    Float2AssignOp(/=);
#undef Float2AssignOp
#define Float2AssignOpScalar(op) \
    zun_operator_inline Float2& operator op(const float& value) {\
        this->x op value; \
        this->y op value; \
        return *this; \
    }
    Float2AssignOpScalar(+=);
    Float2AssignOpScalar(-=);
    Float2AssignOpScalar(*=);
    Float2AssignOpScalar(/=);
#undef Float2AssignOpScalar

#pragma endregion
};
#pragma region // Float2 Validation
ValidateFieldOffset32(0x0, Float2, x);
ValidateFieldOffset32(0x4, Float2, y);
ValidateStructSize32(0x8, Float2);
#pragma endregion

// size: 0xC
struct Float3 : Float2 {
    float z; // 0x8

    using Float2::Float2;
    inline constexpr Float3() {};
    inline constexpr Float3(const float& X, const float& Y, const float& Z) : Float2(X, Y), z(Z) {}
    
    inline Float2& as2() {
        return *(Float2*)this;
    }
    
    // 0x425B40
#if GAME_VERSION == UM_VER
    dllexport gnu_noinline float vectorcall __bullet_effect_angle_jank(float angle, float arg2, float arg3);
#endif

    void safe_copy(Float3* src) {
        if (!src) {
            *this = { 0.0f, 0.0f, 0.0f };
        } else {
            *this = *src;
        }
    }

#pragma region // Float3 Operators

#define Float3BinOp(op) \
    zun_operator_inline Float3 operator op(const Float3& value) { \
        return { \
            this->x op value.x, \
            this->y op value.y, \
            this->z op value.z \
        }; \
    } \
    zun_operator_inline Float3 operator op(const Float2& value) { \
        return { \
            this->x op value.x, \
            this->y op value.y, \
            this->z op 0.0f \
        }; \
    }
    Float3BinOp(+);
    Float3BinOp(-);
    Float3BinOp(*);
    Float3BinOp(/);
#undef Float3BinOp
#define Float3BinOpScalar(op) \
    zun_operator_inline Float3 operator op(const float& value) { \
        return { \
            this->x op value, \
            this->y op value, \
            this->z op value \
        }; \
    }
    Float3BinOpScalar(+);
    Float3BinOpScalar(-);
    Float3BinOpScalar(*);
    Float3BinOpScalar(/);
#undef Float3BinOpScalar
#define Float3UnOp(op) \
    zun_operator_inline Float3 operator op() { \
        return { \
            op this->x, \
            op this->y, \
            op this->z \
        }; \
    }
    Float3UnOp(+);
    Float3UnOp(-);
#undef Float3UnOp
#define Float3AssignOp(op) \
    zun_operator_inline Float3& operator op(const Float3& value) {\
        this->x op value.x; \
        this->y op value.y; \
        this->z op value.z; \
        return *this; \
    } \
    zun_operator_inline Float3& operator op(const Float2& value) {\
        this->x op value.x; \
        this->y op value.y; \
        this->z op 0.0f; \
        return *this; \
    }
    Float3AssignOp(+=);
    Float3AssignOp(-=);
    Float3AssignOp(*=);
    Float3AssignOp(/=);
#undef Float3AssignOp
#define Float3AssignOpScalar(op) \
    zun_operator_inline Float3& operator op(const float& value) {\
        this->x op value; \
        this->y op value; \
        this->z op value; \
        return *this; \
    }
    Float3AssignOpScalar(+=);
    Float3AssignOpScalar(-=);
    Float3AssignOpScalar(*=);
    Float3AssignOpScalar(/=);
#undef Float3AssignOpScalar

#pragma endregion

};
#pragma region // Float3 Validation
ValidateFieldOffset32(0x0, Float3, x);
ValidateFieldOffset32(0x4, Float3, y);
ValidateFieldOffset32(0x8, Float3, z);
ValidateStructSize32(0xC, Float3);
#pragma endregion

#undef zun_operator_inline

// size: 0x10
struct Float4 {
    float x; // 0x0
    float y; // 0x4
    float z; // 0x8
    float w; // 0xC
};
#pragma region // Float4 Validation
ValidateFieldOffset32(0x0, Float4, x);
ValidateFieldOffset32(0x4, Float4, y);
ValidateFieldOffset32(0x8, Float4, z);
ValidateFieldOffset32(0xC, Float4, w);
ValidateStructSize32(0x10, Float4);
#pragma endregion

#endif