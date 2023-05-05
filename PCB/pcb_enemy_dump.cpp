#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <array>
#include <type_traits>

#include <intrin.h>

template<typename T, size_t count>
using vec = T __attribute__((__vector_size__(count * sizeof(T))));

template<typename T, size_t X, size_t Y>
using matrix = T __attribute__((matrix_type(X, Y)));

#define _MACRO_CAT(arg1, arg2) arg1 ## arg2
#define MACRO_CAT(arg1, arg2) _MACRO_CAT(arg1, arg2)
#define MACRO_CAT2(arg1, arg2, ...) _MACRO_CAT(arg1, arg2) __VA_ARGS__
#define _MACRO_STR(arg) #arg
#define MACRO_STR(arg) _MACRO_STR(arg)
#define MACRO_EVAL(...) __VA_ARGS__

#define unique_name(prefix) \
MACRO_CAT(prefix, __COUNTER__)

#define unknown_name \
unique_name(unknown_)

#define padding_name \
unique_name(padding_)

#define irrelevant_name \
unique_name(irrelevant_)

#define used_name \
unique_name(used_)

#define unknown_fields(size) \
unsigned char unknown_name[size]

#define padding_bytes(size) \
unsigned char padding_name[size]

#define probably_padding_bytes(size) \
padding_bytes(size)

#define irrelevant_bytes(size) \
unsigned char irrelevant_name[size]

#define ValidateFieldOffset(offset, struct_type, member_name) \
static_assert((offset) == __builtin_offsetof(struct_type, member_name), "Incorrect struct offset! Offset of " MACRO_STR(struct_type) "."#member_name" is not "#offset)

struct __attribute__((packed)) Timer {
    int32_t previous;
    float subframe;
    int32_t current;
};

// size: 0x68
struct __attribute__((packed)) EclContextVars { // EclContextVars, EclContext, Enemy
    int32_t int_vars[4]; // 0x0, 0x18, 0x6FC
    float float_vars[8]; // 0x10, 0x28, 0x70C
    int32_t counter_vars[4]; // 0x30, 0x48, 0x72C
    float more_float_vars[2]; // 0x40, 0x58, 0x73C
    int32_t int_params[4]; // 0x48, 0x60, 0x744
    float float_params[4]; // 0x58, 0x70, 0x754
	// 0x68, 0x80, 0x764
};

struct __attribute__((packed)) EclInstruction {
    uint32_t time;
    int16_t opcode;
    int16_t offset_to_next;
    uint8_t unused;
    uint8_t difficulty_mask;
    uint16_t param_mask;
    unsigned char args[];
};

struct __attribute__((packed)) EnemyFloatInterp {
    //unknown_fields(0x4);
    Timer timer;
    //unknown_fields(0x20);
};

// size: 0x218
struct __attribute__((packed)) EclContext {
    EclInstruction* current_instr; // 0x0
    Timer time; // 0x4
    void* func_set; // 0x10
    EclInstruction* func_set_instr; // 0x14
    EclContextVars vars; // 0x18
    Timer wait_timer; // 0x80
    EnemyFloatInterp float_interpolators[8]; // 0x8C
    int32_t compare; // 0x20C
    int32_t async_id; // 0x210
    uint16_t sub_id; // 0x214
    //padding_bytes(0x2);
};

struct __attribute__((packed)) Enemy {
    //irrelevant_bytes(0x24C * 3); // 0x0
    EclContext current_context; // 0x6E4
    EclContext context_stack[16]; // 0x8FC
    int32_t stack_depth; // 0x2A7C
    //unknown_fields(0x4); // 0x2A80
    int32_t death_callback_sub; // 0x2A84
    int32_t interrupts[32]; // 0x2A88
    int32_t run_interrupt; // 0x2B08
    //irrelevant_bytes(0x30B); // 0x2B0C
    uint8_t boss_id; // 0x2E17
    //irrelevant_bytes(0x10); // 0x2E18
    /*
    struct { //
        uint8_t move_mode : 2;
        uint8_t interp_mode : 3;
        uint8_t shoot_disable : 1;
        uint8_t mirrored : 1;
        uint8_t active : 1;
    };
    struct { // 0x2E29
        uint8_t can_interact : 1;
        uint8_t contact_damage : 1;
        uint8_t invulnerable : 1;
        uint8_t invisible : 1;
        uint8_t can_take_damage : 1;
        uint8_t grazeable : 1;
        uint8_t is_boss : 1;
        uint8_t move_limit : 1;
    };
    struct { // 0x2E2A
        uint8_t death_flags : 3;
        uint8_t __is_visible : 1;
        uint8_t anm_rotate : 1;
        uint8_t disable_call_stack : 1;
        uint8_t timeout_spell : 1;
        uint8_t oob_immune : 1;
    };
    struct { // 0x2E2B
        uint8_t __chen_jank_enable : 1;
        uint8_t spell_effect_visible : 1;
        uint8_t __yukari_jank_enable : 1;
        uint8_t armored : 1;
        uint8_t : 4;
    };
    */
    uint32_t flags; // 0x2E28
    //irrelevant_bytes(0x90); // 0x2E2C
    int32_t life_callback_thresholds[4]; // 0x2EBC
    int32_t life_callback_subs[4]; // 0x2ECC
    int32_t timer_callback_threshold; // 0x2EDC
    int32_t timer_callback_sub; // 0x2EE0
    int32_t call_repeat_sub; // 0x2EE4
    EclContextVars call_repeat_context; // 0x2EE8
    Timer call_repeat_timer; // 0x2F50
    //unknown_fields(0x1FEC); // 0x2F5C
};

/*

static constexpr auto kqwebjhq3vb = offsetof(Enemy, current_context);

ValidateFieldOffset(0x6E4, Enemy, current_context);
ValidateFieldOffset(0x8FC, Enemy, context_stack);
ValidateFieldOffset(0x2A7C, Enemy, stack_depth);
ValidateFieldOffset(0x2A84, Enemy, death_callback_sub);
ValidateFieldOffset(0x2A88, Enemy, interrupts);
ValidateFieldOffset(0x2B08, Enemy, run_interrupt);
ValidateFieldOffset(0x2E17, Enemy, boss_id);
ValidateFieldOffset(0x2E28, Enemy, flags);
ValidateFieldOffset(0x2EBC, Enemy, life_callback_thresholds);
ValidateFieldOffset(0x2ECC, Enemy, life_callback_subs);
ValidateFieldOffset(0x2EDC, Enemy, timer_callback_threshold);
ValidateFieldOffset(0x2EE0, Enemy, timer_callback_sub);
ValidateFieldOffset(0x2EE4, Enemy, call_repeat_sub);
ValidateFieldOffset(0x2EE8, Enemy, call_repeat_context);
ValidateFieldOffset(0x2F50, Enemy, call_repeat_timer);
*/

__declspec(dllexport, noinline) int log_printf(const char *format, ...) {
    va_list va;
    va_start(va, format);
    vprintf(format, va);
    va_end(va);
    return 0;
}

__declspec(dllexport, noinline) void dump_enemy(Enemy* enemy) {
    __builtin_dump_struct(enemy, &log_printf);
}
