#ifndef ZUN_NEW_ENGINE_TYPES_H
#define ZUN_NEW_ENGINE_TYPES_H 1

#include <stdint.h>
#include <stdlib.h>

#include "../zero/util.h"
#include "zun.h"
#include "funcs.h"

// size: 0x8
struct GameSpeed {
    float value = 1.0f; // 0x0
    int __counter_4 = 0; // 0x4
    // 0x8

    // MoF: 0x44C620
    // SA: 0x459B00
    // UFO: 0x4653F0
    // DS: 0x4652F0
    // GFW: 0x46C250
    GameSpeed() {}

    // MoF: 0x44C5F0
    // SA: 0x459AC0
    // UFO: 0x4653B0
    // DS: 0x4652B0
    // GFW: 0x46C210
    float thiscall scale(float value) const {
        if constexpr (game_version < SA) {
            return this->value * value;
        } else if constexpr (game_version >= SA && game_version <= GFW) {
            double temp = this->value;
            temp *= value;
            return temp;
        }
    }

    // MoF: 0x44C600:
    // SA: 0x459AE0
    // UFO: 0x4653D0
    // DS: 0x4652D0
    // GFW: 0x46C230
    void __decrement_counter_4() {
        --this->__counter_4;
    }

    // UFO: 0x4066A0
    // UM: 0x43A200
    dllexport gnu_noinline void vectorcall set(float new_speed);
};
#pragma region // GameSpeed Validation
ValidateFieldOffset32(0x0, GameSpeed, value);
ValidateFieldOffset32(0x4, GameSpeed, __counter_4);
ValidateStructSize32(0x8, GameSpeed);
#pragma endregion

// size: 0x14
struct Timer {
    /* ==========
    Notes
    ========== */
    /*
    
    MoF, UB:
    -Uses game_speed directly as a pointer without null checking
    -Includes default values when setting
    -Returns void from most functions
    -Compares with float values for the time scale

    SA, UFO:
    -Mostly like MoF, but casts to double for some things
    -Returns int from most functions
    
    DS:
    -Adds a few more functions
    
    */

    /* ==========
    Janky Type Garbage
    ========== */

private:
    static inline constexpr bool skip_default_values_for_set = game_version >= UM;

    using lower_time_bound_t = std::conditional_t<game_version >= SA && game_version <= GFW, float, double>;
    using upper_time_bound_t = float;
    using increment_time_t = std::conditional_t<game_version >= SA && game_version <= GFW, float, double>;
    using decrement_time_t = float;
    static inline constexpr const lower_time_bound_t lower_time_bound = 0.99;
    static inline constexpr const upper_time_bound_t upper_time_bound = 1.01;
    static inline constexpr const increment_time_t increment_amount = 1.0;
    static inline constexpr const decrement_time_t decrement_amount = -1.0;

    //forceinline 

public:

    /* ==========
    Fields/Members
    ========== */
    int32_t previous; // 0x0
    int32_t current; // 0x4
    float current_f; // 0x8
    GameSpeed* time_scale; // 0xC
    struct {
        uint32_t flags; // 0x10
        struct {
            uint32_t initialized : 1;
        };
    };
    // 0x14

    /* ==========
    Default Constructor
    ========== */

    // MoF: 0x44BF20
    // UB: 0x448290
    // SA: 0x4591E0
    // UFO: 0x4649F0
    // DS: 0x463E30
    // GFW: 0x46A9F0
    // DDC: 0x4038C0
    
    // UDoALGtr2: Rx125800
    inline Timer() : initialized(false) {};

    /* ==========
    Assignment Functions
    ========== */

    // MoF: 0x401F40
    // UB: 0x402220
    // SA: 0x4022E0
    // UFO: 0x402850
    // DS: 0x402ED0
    // GFW: 0x403220
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
    inline void initialize_without_defaults() {
        if (!this->initialized) {
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
        int32_t int_time = zftol(time);
        this->current_f = time;
        this->current = int_time;
        this->previous = int_time - 1;
    }

    // UM: 0x405D10
    void thiscall set(int32_t time) {
        this->initialize();
        this->set_raw(time);
    }
    void thiscall set(float time) {
        this->initialize();
        this->set_raw(time);
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

    /* ==========
    Time scale crap
    ========== */

    // MoF: 0x44BF30
    // UB: 0x4482A0
    // SA: 0x4591F0
    // UFO: 0x464A00
    // DS: 0x463E40
    // GFW: 0x46AA00
    // DDC: 0x4038D0
    void thiscall set_time_scale(GameSpeed* new_time_scale) {
        if (GameSpeed* time_scale = this->time_scale) {
            time_scale->__decrement_counter_4();
        }
        this->time_scale = new_time_scale;
    }

    inline GameSpeed* get_time_scale_ptr() const {
        return this->time_scale;
    }

    // MoF: 0x44BFF0
    // UB: 0x448360
    // SA: 0x4592D0
    // UFO: 0x464AE0
    // DS: 0x463F20
    // GFW: 0x46AAF0
    // DDC: 0x4039C0
    float thiscall get_time_scale() const {
        return this->time_scale->value;
    }

    /* ==========
    Logical Functions
    ========== */

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

    /* ==========
    Arithmetic Functions
    ========== */

    // DDC: 0x403940
    // UM: 0x402A60
    int32_t vectorcall add_raw(float amount) {
        this->previous = this->current;
        return this->set_current(this->current_f + amount);
    }

    int32_t thiscall add(float amount) {
        GameSpeed* time_scale = this->get_time_scale_ptr();
        int32_t current_time = this->current;
        this->previous = current_time;
        if (time_scale->value > lower_time_bound && time_scale->value < upper_time_bound) {
            this->current_f += amount;
            return this->current = zftol(this->current_f);
        }
        this->current_f += time_scale->scale(amount);
        return this->current = zftol(this->current_f);
    }

    inline int32_t thiscall add(int32_t amount) {
        return this->add((float)amount);
    }

    int32_t vectorcall sub(float amount) {
        GameSpeed* time_scale = this->get_time_scale_ptr();
        int32_t current_time = this->current;
        this->previous = current_time;
        if (time_scale->value > lower_time_bound && time_scale->value < upper_time_bound) {
            this->current_f -= amount;
            return this->current = zftol(this->current_f);
        }
        this->current_f -= time_scale->scale(amount);
        return this->current = zftol(this->current_f);
    }

    inline int32_t thiscall sub(int32_t amount) {
        return this->add((float)(-amount));
    }

    /* ==========
    Offset Functions
    ========== */
    
    int32_t thiscall increment() {
        GameSpeed* time_scale = this->get_time_scale();
        int32_t current_time = this->current;
        this->previous = current_time;
        if (time_scale->value > lower_time_bound && time_scale->value < upper_time_bound) {
            this->current_f += increment_amount;
            return this->current = zftol(this->current_f);
        }
        this->current_f += time_scale->value;
        return this->current = zftol(this->current_f);
    }

    int32_t thiscall decrement() {
        if constexpr (game_version <= UFO) {
            return this->add(decrement_amount);
        } else {
            return this->sub(increment_amount);
        }
    }

    /* ==========
    Non-default Constructors
    ========== */

    // DDC: 0x408AB0
    thiscall Timer(int32_t time) {
        this->initialize_and_set(time);
    }

    // DDC: 0x443E10
    thiscall Timer(float time) {
        this->initialize_and_set(time);
    }

    /* ==========
    Assignment Operators
    ========== */

    // MoF: 0x405410, 0x405450
    // UB: 0x405720, 0x405760
    // SA: 0x4060C0, 0x406100
    // UFO: 0x4067A0, 0x4067E0
    // DS: 0x402CB0, 0x402CF0
    // GFW: 0x403240, 0x403280
    // DDC: 0x408B00
    int32_t thiscall operator=(int32_t time) {
        return this->initialize_and_set(time);
    }

    // DS: 0x428280, 0x4282C0
    // GFW: 0x432C40, 0x432C80
    int32_t thiscall operator=(float time) {
        return this->initialize_and_set(time);
    }

    // DS: 0x458640
    // GFW: 0x45E5E0
    // UM: 0x418C40
    int32_t thiscall operator=(Timer timer) {
        return this->set(timer.current);
    }

    /* ==========
    Arithmetic Operators
    ========== */

    // MoF: 0x4052D0, 0x413070
    // UB: 0x405610, 0x413FC0
    // SA: 0x405E20
    // UFO: 0x4064F0
    // DS: 0x406F90
    // GFW: 0x407150
    // DDC: 0x4085B0
    int32_t thiscall operator+=(int32_t amount) {
        return this->add(amount);
    }

    // MoF: 0x44BF40
    // UB: 0x4482B0
    // SA: 0x459210
    // UFO: 0x464A20
    // DS: 0x463E60
    // GFW: 0x46AA20
    // DDC: 0x4038F0
    int32_t vectorcall operator+=(float amount) {
        return this->add(amount);
    }

    // MoF: 0x4052B0, 0x428DD0 (screwy calling convention)
    // UB: 0x4055F0, 0x428B40 (screwy calling convention)
    // SA: 0x405E30, 0x4355C0 (screwy calling convention)
    // UFO: 0x406500, 0x43ADB0 (screwy calling convention)
    // DS: 0x406FA0, 0x428150 (screwy calling convention)
    // GFW: 0x407170, 0x432B10 (screwy calling convention)
    // DDC: 0x411530, 0x414420
    int32_t thiscall operator-=(int32_t amount) {
        return this->sub(amount);
    }

    int32_t vectorcall operator-=(float amount) {
        return this->sub(amount);
    }

    /* ==========
    Offset Operators
    ========== */

    // MoF: 0x404ED0, 0x44BFA0
    // UB: 0x405250, 0x448310
    // SA: 0x405AD0, 0x459270
    // UFO: 0x4061A0, 0x464A80
    // DS: 0x402B40, 0x463EC0
    // GFW: 0x402AC0, 0x46AA90
    // DDC: 0x403960
    // UM: 0x402A80
    
    // UDoALGtr2: Rx125810
    int32_t thiscall operator++() {
        return this->increment();
    }

    // DDC: 0x408690
    // UM: 0x405990
    int32_t thiscall operator++(int) {
        return this->increment();
    }

    // SA: 0x405E50
    // UFO: 0x406520
    // DS: 0x406FC0
    // GFW: 0x407190
    int32_t thiscall operator--() {
        return this->decrement();
    }

    // UM: 0x409750
    int32_t thiscall operator--(int) {
        return this->decrement();
    }

    /* ==========
    Logical Operators
    ========== */

    // MoF: 0x413050
    // UB: 0x4194F0
    // SA: 0x407600
    // UFO: 0x407B20
    // DS: 0x402C60
    // GFW: 0x402C40
    // DDC: 0x408A20
    bool thiscall operator<(int32_t time) const {
        return this->les(time);
    }

    // MoF: 0x405280
    // UB: 0x4055C0
    // SA: 0x405E10
    // UFO: 0x4064E0
    // DS: 0x406F80
    // GFW: 0x407130
    // DDC: 0x411510
    // UDoALG: Rx2CF0
    bool thiscall operator<=(int32_t time) const {
        return this->leq(time);
    }

    // MoF: 0x413080
    // UB: 0x406260
    // SA: 0x407090
    // UFO: 0x407720
    // DS: 0x41A980
    // GFW: 0x410CD0
    // DDC: 0x412E00
    bool thiscall operator>(int32_t time) const {
        return this->gre(time);
    }

    // MoF: 0x404E90
    // UB: 0x405210
    // SA: 0x405AB0
    // UFO: 0x406740
    // DS: 0x4071E0
    // GFW: 0x4073B0
    // DDC: 0x409FA0
    // UDoALG: Rx2DC0
    bool thiscall operator>=(int32_t time) const {
        return this->geq(time);
    }

    // MoF: 0x418A50
    // UB: 0x4195C0
    // SA: 0x407A20
    // UFO: 0x408940
    // DS: 0x40C720
    // GFW: 0x407E70
    // DDC: 0x413190
    bool thiscall operator==(int32_t time) const {
        return this->equ(time);
    }

    // Not in anything but UB? Why?
    // UB: 0x428BA0
    bool thiscall operator!=(int32_t time) const {
        return this->neq(time);
    }

    bool thiscall operator!() const {
        return this->is_zero();
    }

    // SA: 0x419B90
    // UFO: 0x41C9B0
    // DS: 0x43B230
    // GFW: 0x432E10
    thiscall operator bool() const {
        return this->is_not_zero();
    }
};
#pragma region // Timer Validation
ValidateFieldOffset32(0x0, Timer, prev);
ValidateFieldOffset32(0x4, Timer, current_f);
ValidateFieldOffset32(0x8, Timer, current);
ValidateFieldOffset32(0xC, Timer, time_scale);
ValidateFieldOffset32(0x10, Timer, flags);
ValidateStructSize32(0x14, Timer);
#pragma endregion

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