#ifndef ZUN_TYPES_H
#define ZUN_TYPES_H 1

#include <stdint.h>
#include <stdlib.h>
//#include <math.h>

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

    inline Float1() = default;
    inline constexpr Float1(const float& X) : x(X) {}
};

// size: 0x8
struct Float2 : Float1 {
    float y; // 0x4

    using Float1::Float1;
    inline Float2() = default;
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
        clang_noinline return zatan2f(this->y - coord.y, this->x - coord.x);
    }

    // th14: 0x4143B0, 0x415410, 0x42E070, 0x454460
    inline float angle_to(const Float2* coord) {
        return zatan2f(this->y - coord->y, this->x - coord->x);
    }

    // th14: 0x407090, 0x415590, 0x42E270, 0x443FA0
    inline float direction() {
        return zatan2f(this->y, this->x);
    }

    inline float dot_product(const Float2& value) {
        return this->x * value.x + this->y * value.y;
    }

    inline float hypot_squared() {
        return this->dot_product(*this);
    }

    inline float hypot() {
        return zsqrt(this->hypot_squared());
    }

    inline Float2 normalize() {
        float scale_squared = this->hypot_squared();
        Float2 ret = *this;
        if (scale_squared >= 0.1f) {
            ret /= zsqrt(scale_squared);
        }
        return ret;
    }

    inline float length_squared() {
        float x = this->x;
        float y = this->y;
        return x * x + y * y;
    }
    inline float length() {
        float x = this->x;
        float y = this->y;
        return zsqrtf(x * x + y * y);
    }

    inline float distance_squared(const Float2* value) {
        float x_diff = this->x - value->x;
        float y_diff = this->y - value->y;
        return x_diff * x_diff + y_diff * y_diff;
    }

    inline float distance(const Float2* value) {
        float x_diff = this->x - value->x;
        float y_diff = this->y - value->y;
        return zsqrtf(x_diff * x_diff + y_diff * y_diff);
    }

    inline Float2 rotate_around_origin(float angle) const {
        float y_unit = zsinf(angle);
        float x_unit = zcosf(angle);
        return {
            (x_unit * this->x) - (y_unit * this->y),
            (x_unit * this->y) + (y_unit * this->x)
        };
    }
    
#pragma region // Float2 Operators

#define Float2BinOp(op) \
    zun_operator_inline Float2 operator op(const Float2& value) const { \
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
    zun_operator_inline Float2 operator op(float value) const { \
        return { \
            this->x op value, \
            this->y op value \
        }; \
    } \
    friend zun_operator_inline Float2 operator op(float lhs, const Float2& rhs) { \
        return { \
            lhs op rhs.x, \
            lhs op rhs.y \
        }; \
    }
    Float2BinOpScalar(+);
    Float2BinOpScalar(-);
    Float2BinOpScalar(*);
    Float2BinOpScalar(/);
#undef Float2BinOpScalar
#define Float2UnOp(op) \
    zun_operator_inline Float2 operator op() const { \
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
    zun_operator_inline Float2& operator op(float value) {\
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
    //inline constexpr Float3() {};
    inline Float3() = default;
    inline constexpr Float3(const Float2& v) : Float2(v), z(0.0f) {}
    inline constexpr Float3(const Float2& v, float Z) : Float2(v), z(Z) {}
    inline constexpr Float3(const float& X, const float& Y, const float& Z) : Float2(X, Y), z(Z) {}
    
    inline Float2& as2() {
        return *(Float2*)this;
    }
    
    // 0x425B40
#if GAME_VERSION == UM_VER
    dllexport gnu_noinline float vectorcall __bullet_effect_angle_jank(float angle, float arg2, float arg3);
#endif

    void set(const Float2& src) {
        *(double*)&this->x = *(double*)&src.x;
        this->z = 0.0f;
    }

    void set(const Float3& src) {
        *(double*)&this->x = *(double*)&src.x;
        this->z = src.z;
    }

    void safe_copy(Float3* src) {
        if (!src) {
            *this = { 0.0f, 0.0f, 0.0f };
        } else {
            this->set(*src);
        }
    }

    /*
    inline float length_squared() {
        float x = this->x;
        float y = this->y;
        float z = this->z;
        return x * x + y * y + z * z;
    }
    inline float length() {
        float x = this->x;
        float y = this->y;
        float z = this->z;
        return zsqrtf(x * x + y * y + z * z);
    }

    inline float distance_squared(const Float3* value) {
        float x_diff = this->x - value->x;
        float y_diff = this->y - value->y;
        float z_diff = this->z - value->z;
        return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
    }

    inline float distance(const Float3* value) {
        float x_diff = this->x - value->x;
        float y_diff = this->y - value->y;
        float z_diff = this->z - value->z;
        return zsqrtf(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
    }
    */

#pragma region // Float3 Operators

#define Float3BinOp(op) \
    zun_operator_inline Float3 operator op(const Float3& value) const { \
        return { \
            this->x op value.x, \
            this->y op value.y, \
            this->z op value.z \
        }; \
    } \
    zun_operator_inline Float3 operator op(const Float2& value) const { \
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
    zun_operator_inline Float3 operator op(float value) const { \
        return { \
            this->x op value, \
            this->y op value, \
            this->z op value \
        }; \
    } \
    friend zun_operator_inline Float3 operator op(float lhs, const Float3& rhs) { \
        return { \
            lhs op rhs.x, \
            lhs op rhs.y, \
            lhs op rhs.z \
        }; \
    }
    Float3BinOpScalar(+);
    Float3BinOpScalar(-);
    Float3BinOpScalar(*);
    Float3BinOpScalar(/);
#undef Float3BinOpScalar
#define Float3UnOp(op) \
    zun_operator_inline Float3 operator op() const { \
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
    zun_operator_inline Float3& operator op(float value) {\
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

#if 0 && GAME_VERSION >= MoF_VER

// size: 0x8
struct GameSpeed {
    float value = 1.0f; // 0x0
    int __counter_4; // 0x4
    // 0x8

    // UM: 0x43A200
    dllexport gnu_noinline void vectorcall set(float new_speed);
};
#pragma region // GameSpeed Validation
ValidateFieldOffset32(0x0, GameSpeed, value);
ValidateFieldOffset32(0x4, GameSpeed, __counter_4);
ValidateStructSize32(0x8, GameSpeed);
#pragma endregion

struct Timer {
    int32_t prev; // 0x0
    int32_t current; // 0x4
    float current_f; // 0x8
    GameSpeed* time_scale; // 0xC
    struct {
        uint32_t flags; // 0x10
        struct {
            uint32_t initialized : 1;
        };
    };

    // DDC: 0x4038C0
    inline Timer() : initialized(false) {};

    // MoF: 0x401F40
    // DDC: 0x408A90
    inline void default_values() {
        this->current = 0;
        this->previous = -999999;
        this->current_f = 0.0f;
    }

    inline void initialize() {
        if (!this->initialized) {
            this->default_values();
            this->time_scale = 0;
            this->initialized = true;
        }
    }

    inline void set_raw(int32_t time) {
        float float_time = time;
        this->current = time;
        this->current_f = float_time;
        this->previous = time - 1;
    }

    inline void set_raw(float time) {
        int32_t int_time = time;
        this->current_f = time;
        this->current = int_time;
        this->previous = int_time - 1;
    }

    inline void reset() {
        this->set(0);
    }
    inline void initialize_and_reset() {
        this->initialize();
        this->set_raw(0);
    }
    inline void initialize_and_set(int32_t time) {
        this->initialize();
        this->set_raw(time);
    }

    // UM: 0x405D10
    dllexport void thiscall set(int32_t time) {
        this->initialize_important();
        this->set_raw(time);
    }

    void thiscall set(float time) {
        this->initialize_important();
        this->set_raw(time);
    }

    // MoF: 0x44BF30
    // DDC: 0x4038D0
    dllexport void thiscall set_time_scale(GameSpeed* new_time_scale) {
        if (GameSpeed* time_scale = this->time_scale) {
            --time_scale->__counter_4;
        }
        this->time_scale = new_time_scale;
    }

    inline GameSpeed* get_time_scale_ptr() const {
        return this->time_scale;
    }

    // DDC: 0x4039C0
    float thiscall get_time_scale() const {
        return this->time_scale->value;
    }

    forceinline bool les(int32_t time) const {
        return this->current < time;
    }
    forceinline bool leq(int32_t time) const {
        return this->current <= time;
    }
    forceinline bool gre(int32_t time) const {
        return this->current > time;
    }
    forceinline bool geq(int32_t time) const {
        return this->current >= time;
    }
    forceinline bool equ(int32_t time) const {
        return this->current == time;
    }
    forceinline bool neq(int32_t time) const {
        return this->current != time;
    }
    forceinline bool is_zero() const {
        return this->equ(0);
    }
    forceinline bool is_not_zero() const {
        return this->neq(0);
    }

    // DDC: 0x403940
    // UM: 0x402A60
    int32_t vectorcall add_raw(float amount) {
        this->previous = this->current;
        return this->set_current(this->current_f + amount);
    }

    // MoF: 0x44BF40
    int32_t stdcall add(float amount) {
        GameSpeed* time_scale = this->get_time_scale_ptr();
        int32_t current_time = this->current;
        this->prev = current_time;
        if (time_scale->value > 0.99f && time_scale->value < 1.01f) {
            this->current_f += amount;
            return this->current = CRT::ftol2(this->current_f);
        }
        this->current_f += amount * time_scale->value;
        return this->current = CRT::ftol2(this->current_f);
    }

    inline int32_t thiscall add(int32_t amount) {
        return this->add((float)amount);
    }

    int32_t vectorcall sub(float amount) {
        GameSpeed* time_scale = this->get_time_scale_ptr();
        int32_t current_time = this->current;
        this->prev = current_time;
        if (time_scale->value > 0.99f && time_scale->value < 1.01f) {
            this->current_f -= amount;
            return this->current = CRT::ftol2(this->current_f);
        }
        this->current_f -= amount * time_scale->value;
        return this->current = CRT::ftol2(this->current_f);
    }

    inline int32_t thiscall sub(int32_t amount) {
        return this->add((float)(-amount));
    }
    
    int32_t thiscall increment() {
        GameSpeed* time_scale = this->get_time_scale();
        int32_t current_time = this->current;
        this->prev = current_time;
        if (time_scale->value > 0.99f && time_scale->value < 1.01f) {
            this->current_f += 1.0f;
            return this->current = CRT::ftol2(this->current_f);
        }
        this->current_f += time_scale->value;
        return this->current = CRT::ftol2(this->current_f);
    }

    int32_t thiscall decrement() {
        return this->sub(1.0f);
    }

    // DDC: 0x408AB0
    thiscall Timer(int32_t time) {
        this->initialize_and_set(time);
    }

    // DDC: 0x443E10
    thiscall Timer(float time) {
        this->initialize_and_set(time);
    }

    // DDC: 0x408B00
    int32_t thiscall operator=(int32_t time) {
        return this->initialize_and_set(time);
    }

    int32_t thiscall operator=(float time) {
        return this->initialize_and_set(time);
    }

    // UM: 0x418C40
    int32_t thiscall operator=(Timer timer) {
        return this->set(timer.current);
    }

    // MoF: 0x413070
    // DDC: 0x4085B0
    int32_t thiscall operator+=(int32_t amount) {
        return this->add(amount);
    }

    // DDC: 0x4038F0
    int32_t vectorcall operator+=(float amount) {
        return this->add(amount);
    }

    // DDC: 0x411530, 0x414420
    int32_t thiscall operator-=(int32_t amount) {
        return this->sub(amount);
    }

    int32_t vectorcall operator-=(float amount) {
        return this->sub(amount);
    }

    // MoF: 0x404ED0
    // DDC: 0x403960
    // UM: 0x402A80
    int32_t thiscall operator++() {
        return this->increment();
    }

    // DDC: 0x408690
    // UM: 0x405990
    int32_t thiscall operator++(int) {
        return this->increment();
    }

    int32_t thiscall operator--() {
        return this->decrement();
    }

    // UM: 0x409750
    int32_t thiscall operator--(int) {
        return this->decrement();
    }

    // MoF: 0x413050
    // DDC: 0x408A20
    bool thiscall operator<(int32_t time) const {
        return this->les(time);
    }

    // MoF: 0x405280
    // DDC: 0x411510
    bool thiscall operator<=(int32_t time) const {
        return this->leq(time);
    }

    // MoF: 0x413080
    // DDC: 0x412E00
    bool thiscall operator>(int32_t time) const {
        return this->gre(time);
    }

    // MoF: 0x404E90
    // DDC: 0x409FA0
    bool thiscall operator>=(int32_t time) const {
        return this->geq(time);
    }

    // MoF: 0x418A50
    // DDC: 0x413190
    bool thiscall operator==(int32_t time) const {
        return this->equ(time);
    }

    bool thiscall operator!=(int32_t time) const {
        return this->neq(time);
    }

    bool thiscall operator!() const {
        return this->is_zero();
    }

    bool thiscall operator bool() const {
        return this->is_not_zero();
    }
};

template <typename T, size_t modes = 1>
struct ZUNInterp { //       0x58    0x44    0x30
    //                      T3      T2      T1
    T initial_value; //     0x0     0x0     0x0
    T final_value; //       0xC     0x8     0x4
    T bezier1; //           0x18    0x10    0x8
    T bezier2; //           0x24    0x18    0xC
    T current; //           0x30    0x20    0x10
    Timer time; //          0x3C    0x28    0x14
    int32_t end_time; //    0x50    0x3C    0x28
    int32_t mode[modes]; // 0x54    0x40    0x2C

    inline ZUNInterp<T, modes>() : end_time(0) {}

    // UM float: 0x41F600
    // UM Float2: 0x439600
    // UM Float3: 0x405A00
    // UM Float3Ex: 0x439940
    // UM ZUNAngle: 0x47CBF0
    dllexport gnu_noinline void set_end_time(int32_t time) {
        this->end_time = time;
    }
    
    // UM Float3Ex: 0x439950
    dllexport gnu_noinline void set_current(const T& value) {
        this->current = value;
    }
    
    // UM float: 0x41B790
    // UM Float2: 0x4395C0
    // UM Float3Ex: 0x439900
    dllexport gnu_noinline void reset_end_time() {
        this->end_time = 0;
    }

    // UM float: 0x41F630
    // UM Float2: 0x439690
    // UM Float3: 0x405A10
    // UM ZUNAngle: 0x47CC40
    dllexport gnu_noinline void set_mode(int32_t mode) {
        this->mode[0] = mode;
    }

    // UM float: 0x41F620
    // UM Float2: 0x439670
    // UM Float3: 0x405A20
    // UM ZUNAngle: 0x47CC30
    dllexport gnu_noinline void set_initial_value(const T& value) {
        this->initial_value = value;
    }

    // UM float: 0x41F610
    // UM Float2: 0x439650
    // UM Float3: 0x405A40
    // UM Float3Ex: 0x4399B0
    // UM ZUNAngle: 0x47CC20
    dllexport gnu_noinline void set_final_value(const T& value) {
        this->final_value = value;
    }

    // UM float: 0x429A70
    // UM Float3: 0x405A60
    // UM Float3Ex: 0x439990
    // UM ZUNAngle: 0x47CC10
    dllexport gnu_noinline void set_bezier1(const T& value) {
        this->bezier1 = value;
    }

    // UM float: 0x429A60
    // UM Float3: 0x405A80
    // UM Float3Ex: 0x439970
    // UM ZUNAngle: 0x47CC00
    dllexport gnu_noinline void set_bezier2(const T& value) {
        this->bezier2 = value;
    }

    // UM float: 0x41F5D0
    // UM Float2: 0x4395D0
    // UM Float3: 0x405AA0
    // UM Float3Ex: 0x439910
    // UM ZUNAngle: 0x47CBC0
    dllexport gnu_noinline void reset_timer() {
        this->time.reset();
    }
    
    // UM int: 0x47CF40
    // UM float: 0x41F640
    // UM Float2: 0x4396A0
    // UM Float3Ex: 0x439A10
    // UM ZUNAngle: 0x47CC50
    dllexport T vectorcall step() {
        int32_t end_time = this->end_time;
        if (end_time > 0) {
            this->time.increment();
            end_time = this->end_time;
            if (this->time.current >= end_time) {
                this->time.set(end_time);
                goto TimeEnd;
            }
        } else if (end_time == 0) {
TimeEnd:
            int32_t mode = this->mode[0];
            if (mode != ConstantVelocity && mode != 17) {
                return this->final_value;
            } else {
                return this->initial_value;
            }
        }
        T initial = this->initial_value;
        int32_t mode = this->mode[0];
        if (mode == ConstantVelocity) { // 7
            return this->current = this->initial_value = initial + this->final_value;
        }
        if (mode == 17) {
            T temp = this->bezier2;
            this->bezier2 = temp + this->final_value;
            return this->current = this->initial_value = initial + temp;
        }
        T current_time = this->time.current_f;
        float end_time_f = end_time;
        if (mode == Bezier) { // 8
            current_time /= end_time_f;
            float XMM3 = 1.0f - current_time;
            float XMM2 = current_time + current_time;
            float XMM5 = current_time - 1.0f;
            XMM3 *= XMM3;
            initial *= XMM5 * XMM5 * (XMM2 + 1.0f);
            initial += (3.0f - XMM2) * this->final_value * current_time * current_time;
            initial += XMM3 * this->bezier1 * current_time;
            initial += XMM5 * this->bezier2 * current_time * current_time;
            return this->current = initial;
        }
        else {
            
        }
    }
};
#pragma region // ZUNInterp Validation
ValidateFieldOffset32(0x0, ZUNInterp<float>, initial_value);
ValidateFieldOffset32(0x4, ZUNInterp<float>, final_value);
ValidateFieldOffset32(0x8, ZUNInterp<float>, bezier1);
ValidateFieldOffset32(0xC, ZUNInterp<float>, bezier2);
ValidateFieldOffset32(0x10, ZUNInterp<float>, current);
ValidateFieldOffset32(0x14, ZUNInterp<float>, time);
ValidateFieldOffset32(0x28, ZUNInterp<float>, end_time);
ValidateFieldOffset32(0x2C, ZUNInterp<float>, mode);
ValidateStructSize32(0x30, ZUNInterp<float>);
ValidateFieldOffset32(0x0, ZUNInterp<Float2>, initial_value);
ValidateFieldOffset32(0x8, ZUNInterp<Float2>, final_value);
ValidateFieldOffset32(0x10, ZUNInterp<Float2>, bezier1);
ValidateFieldOffset32(0x18, ZUNInterp<Float2>, bezier2);
ValidateFieldOffset32(0x20, ZUNInterp<Float2>, current);
ValidateFieldOffset32(0x28, ZUNInterp<Float2>, time);
ValidateFieldOffset32(0x3C, ZUNInterp<Float2>, end_time);
ValidateFieldOffset32(0x40, ZUNInterp<Float2>, mode);
ValidateStructSize32(0x44, ZUNInterp<Float2>);
ValidateFieldOffset32(0x0, ZUNInterp<Float3>, initial_value);
ValidateFieldOffset32(0xC, ZUNInterp<Float3>, final_value);
ValidateFieldOffset32(0x18, ZUNInterp<Float3>, bezier1);
ValidateFieldOffset32(0x24, ZUNInterp<Float3>, bezier2);
ValidateFieldOffset32(0x30, ZUNInterp<Float3>, current);
ValidateFieldOffset32(0x3C, ZUNInterp<Float3>, time);
ValidateFieldOffset32(0x50, ZUNInterp<Float3>, end_time);
ValidateFieldOffset32(0x54, ZUNInterp<Float3>, mode);
ValidateStructSize32(0x58, ZUNInterp<Float3>);
#pragma endregion

#endif

#endif