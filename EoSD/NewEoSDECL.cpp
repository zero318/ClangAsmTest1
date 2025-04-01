#pragma clang diagnostic ignored "-Wc++11-narrowing"

#include <stdio.h>

#include "../ClangAsmTest1.h"

#include "../zero/util.h"

#include <math.h>

#include <stdint.h>

#include "../RNG_helpers.h"

#include "../zero/LinkerCheeseValues.h"
#include "../zero/FloatConstants.h"

#undef WIN32_LEAN_AND_MEAN

#include <Windows.h>

//#include <process.h>
//#include <WinUser.h>
//#include <wbemidl.h>
//#include <winnls32.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>
#include <mmeapi.h>
#include <dsound.h>
#define UNICODE
//#include <Core/DXUT.h>
#include <Optional/SDKsound.h>
#undef UNICODE

#include "../zero/custom_intrin.h"

#define GAME_VERSION EoSD_VER
#include "../zero/zun.h"
#include "../zero/BoundingBox.h"

//#include <d3d.h>

//#define SqueezeStack

dllexport gnu_noinline int32_t cdecl vsprintf_helper(char* buffer, const char* format, va_list va) asm_symbol_rel(0x45B9E0);
dllexport gnu_noinline int32_t cdecl vsprintf_helper(char* buffer, const char* format, va_list va) {
    return vsprintf(buffer, format, va_use(va));
}

dllexport gnu_noinline const char* cdecl strings_vsprintf(const size_t slot, const char* format, va_list va) asm_symbol_rel(strings_vsprintf);
dllexport gnu_noinline const char* cdecl strings_vsprintf(const size_t slot, const char* format, va_list va) {
    gnu_used static volatile auto tempA = slot;
    gnu_used static volatile auto tempB = format;
    gnu_used static volatile auto tempC = va;
    return format;
}

dllexport gnu_noinline auto thcrap_malloc(size_t size) asm_symbol_rel(th_malloc);
dllexport gnu_noinline auto thcrap_malloc(size_t size) {
    return malloc(size);
}

dllexport gnu_noinline auto thcrap_free(void* memory) asm_symbol_rel(th_free);
dllexport gnu_noinline auto thcrap_free(void* memory) {
    return free(memory);
}

typedef int cdecl ascii_put_func_t(void* classptr, Float3* position, const char* text);
dllexport gnu_noinline int stdcall ascii_vpatchf(ascii_put_func_t* putfunc, Float3* position, const char* fmt, va_list va) asm_symbol_rel(ascii_vpatchf);
dllexport gnu_noinline int stdcall ascii_vpatchf(ascii_put_func_t* putfunc, Float3* position, const char* fmt, va_list va) {
    return putfunc((void*)putfunc, position, fmt + (intptr_t)&va);
}

// 0x403670
dllexport gnu_noinline D3DMATRIX& cdecl matrix_set_identity(D3DMATRIX& matrix) asm_symbol_rel(codecave:D3DMATRIX_set_identity);
dllexport gnu_noinline D3DMATRIX& cdecl matrix_set_identity(D3DMATRIX& matrix) {
    matrix.m[3][2] = 0.0f;
    matrix.m[3][1] = 0.0f;
    matrix.m[3][0] = 0.0f;
    matrix.m[2][3] = 0.0f;
    matrix.m[2][1] = 0.0f;
    matrix.m[2][0] = 0.0f;
    matrix.m[1][3] = 0.0f;
    matrix.m[1][2] = 0.0f;
    matrix.m[1][0] = 0.0f;
    matrix.m[0][3] = 0.0f;
    matrix.m[0][2] = 0.0f;
    matrix.m[0][1] = 0.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[0][0] = 1.0f;
    return matrix;
}

// size: 0x10
struct Custom_RGBAQUAD {
    uint32_t red_mask; // 0x0
    uint32_t green_mask; // 0x4
    uint32_t blue_mask; // 0x8
    uint32_t alpha_mask; // 0xC
    // 0x10
};

#define LogStub(...) __noop(__VA_ARGS__)
#define DEBUG_FLAGS 1
#define ReadDebugFlags(mask) (DEBUG_FLAGS & mask)

typedef float __v2sf __attribute__((__vector_size__(8)));
typedef float __v2sf_u __attribute__((__vector_size__(8), __aligned__(1)));
typedef float __v4sf_u __attribute__((__vector_size__(16), __aligned__(1)));

dllexport void float3_angle_reduce(Float3* angles) {
    unaligned vec<float, 4>& angles_ref = *(unaligned vec<float, 4>*)angles;
    vec<double, 4> angles_temp = convertvec(angles_ref, vec<double, 4>);
    angles_temp *= ONE_OVER_TWO_PI_d;
    vec<double, 4> angles_temp2 = _mm256_round_pd(angles_temp, 0x8);
    angles_temp -= angles_temp2;
    angles_temp *= TWO_PI_d;
    vec<float, 4> angles_temp3 = convertvec(angles_temp, vec<float, 4>);
    angles_ref[0] = angles_temp3[0];
    angles_ref[1] = angles_temp3[1];
    angles_ref[2] = angles_temp3[2];
}

// ====================
// Supervisor
// ====================

// size: 0x10
struct Input {
    alignas(DWORD) uint16_t __word_0; // 0x0
    alignas(DWORD) uint16_t __word_4; // 0x4
    alignas(DWORD) uint16_t __word_8; // 0x8
    alignas(DWORD) uint16_t __word_C; // 0xC
    // 0x10
};

extern "C" {
    extern Input INPUTS asm("_INPUT");
}

// size: 0x38
struct Config {
    int16_t __input_related_0; // 0x0
    int16_t __input_related_2; // 0x2
    int16_t __input_related_4; // 0x4
    int16_t __input_related_6; // 0x6
    int16_t __input_related_8; // 0x8
    int16_t __input_related_A; // 0xA
    int16_t __input_related_C; // 0xC
    int16_t __input_related_E; // 0xE
    int16_t __input_related_10; // 0x10
    probably_padding_bytes(0x2);
    uint32_t version; // 0x14
    uint8_t starting_lives; // 0x18
    uint8_t starting_bombs; // 0x19
    uint8_t graphic_format; // 0x1A
    uint8_t bgm_format; // 0x1B
    uint8_t __byte_1C; // 0x1C
    uint8_t __byte_1D; // 0x1D
    uint8_t fullscreen; // 0x1E
    uint8_t __frameskip_setting; // 0x1F
    uint16_t deadzone_x; // 0x20
    uint16_t deadzone_y; // 0x22
    unknown_fields(0x10); // 0x24
    union {
        uint32_t flags; // 0x34
        struct {
            uint32_t __unknown_flag_A : 1;
            uint32_t __unknown_flag_C : 1;
            uint32_t : 4;
            uint32_t __unknown_flag_D : 1;
            uint32_t : 1;
            uint32_t __unknown_flag_B : 1;
        };
    };
    // 0x38
};

struct MidiManager {

    unknown_fields(0x2C8); // 0x0
    int __dword_2C8; // 0x2C8
    unknown_fields(0x14); // 0x2CC
    int __dword_2DC; // 0x2DC
    int __dword_2E0; // 0x2E0
    int __dword_2E4; // 0x2E4
    int __dword_2E8; // 0x2E8
    // 0x2EC

    // 0x4224E0
    dllexport gnu_noinline int32_t thiscall __sub_4224E0() asm_symbol_rel(0x4224E0) {
        gnu_used static volatile auto tempA = this;
        assume_all_registers_volatile();
        return 0;
    }

    // 0x422140
    dllexport gnu_noinline int32_t thiscall __sub_422140(int32_t music_index) asm_symbol_rel(0x422140) {
        gnu_used static volatile auto tempA = this;
        gnu_used static volatile auto tempB = music_index;
        assume_all_registers_volatile();
        return 0;
    }

    // 0x422490
    dllexport gnu_noinline int32_t thiscall __sub_422490() asm_symbol_rel(0x422490) {
        gnu_used static volatile auto tempA = this;
        assume_all_registers_volatile();
        return 0;
    }

    // 0x422630
    dllexport int32_t thiscall __sub_422630(int32_t arg) asm_symbol_rel(codecave:MIDI_MANAGER_sub_422630) {
        this->__dword_2C8 = 0;
        this->__dword_2E4 = arg;
        this->__dword_2E8 = 0;
        this->__dword_2DC = 0;
        this->__dword_2E0 = 1;
        return 0;
    }
};

typedef struct Supervisor Supervisor;
// size: 0x4D8
struct Supervisor {
    HINSTANCE current_instance; // 0x0
    LPDIRECT3D8 d3d; // 0x4
    LPDIRECT3DDEVICE8 d3d_device; // 0x8
    LPDIRECTINPUT8 dinput; // 0xC
    LPDIRECTINPUTDEVICE8 dinput_device; // 0x10
    void* __dinput_related_ptr_14; // 0x14
    unknown_fields(0x2C); // 0x18
    HWND main_window; // 0x44
    D3DMATRIX view_matrix; // 0x48
    D3DMATRIX projection_matrix; // 0x88
    D3DVIEWPORT8 viewport; // 0xC8
    D3DPRESENT_PARAMETERS present_params; // 0xE0
    Config config; // 0x118
    Config default_config; // 0x14C
    int __dword_184; // 0x184
    int __dword_188; // 0x188
    int __dword_18C; // 0x18C
    int __dword_190; // 0x190
    unknown_fields(0x4); // 0x194
    uint32_t __dword_198; // 0x198
    int __dword_19C; // 0x19C
    int __dword_1A0; // 0x1A0
    int __dword_1A4; // 0x1A4
    float game_speed; // 0x1A8
    float slowdown; // 0x1AC
    MidiManager* midi_manager_ptr; // 0x1B0
    float __float_1B4; // 0x1B4
    float __float_1B8; // 0x1B8
    unknown_fields(0x244); // 0x1BC
    int __dword_400; // 0x400
    D3DCAPS8 device_caps; // 0x404
    // 0x4D8

    // 0x424285
    dllexport gnu_noinline void thiscall tick_timer(int32_t* current, float* subframe) const asm_symbol_rel(codecave:SUPERVISOR_tick_timer) {
        if (this->slowdown <= 0.99f) {
            *subframe += this->game_speed;
            if (*subframe >= 1.0f) {
                *current++;
                *subframe -= 1.0f;
            }
        } else {
            *current++;
        }
    }

    inline void set_fog_data(D3DCOLOR color, float near_plane, float far_plane) {
        IDirect3DDevice8 *restrict d3d_device = this->d3d_device;
        d3d_device->SetRenderState(D3DRS_FOGCOLOR, color);
        d3d_device->SetRenderState(D3DRS_FOGSTART, bitcast<DWORD>(near_plane));
        d3d_device->SetRenderState(D3DRS_FOGEND, bitcast<DWORD>(far_plane));
    }

    // 0x424AE4
    dllexport int32_t thiscall change_midi_music(int32_t music_index) asm_symbol_rel(codecave:SUPERVISOR_change_midi_music) {
        if (this->config.bgm_format == 2) {
            if (this->midi_manager_ptr) {
                MidiManager* midi_manager = this->midi_manager_ptr;
                midi_manager->__sub_4224E0();
                midi_manager->__sub_422140(music_index);
                midi_manager->__sub_422490();
            }
            return 0;
        }
        return 1;
    }

    // 0x424B5D
    dllexport gnu_noinline int32_t thiscall load_bgm_file(const char filename[0x80]) asm_symbol_rel(0x424B5D) {
        gnu_used static volatile auto tempA = this;
        gnu_used static volatile auto tempB = filename;
        assume_all_registers_volatile();
        return 0;
    }

    // 0x424D82
    dllexport int32_t thiscall fade_music(float arg) asm_symbol_rel(codecave:SUPERVISOR_fade_music);

};  

extern "C" {
    // 0x6C6D18
    extern Supervisor SUPERVISOR asm("_SUPERVISOR");
}

typedef struct Timer Timer;
// size: 0xC
struct Timer {
    int32_t previous; // 0x0
    float subframe; // 0x4
    int32_t current; // 0x8
    // 0xC

    inline void default_values() {
        this->current = 0;
        this->previous = -1;
        this->subframe = 0.0f;
    }

    inline operator float() {
        return (float)this->current + this->subframe;
    }

    // 0x41B5AF
    dllexport inline int32_t thiscall tick() asm_symbol_rel(codecave:TIMER_tick) {
        this->previous = this->current;
        SUPERVISOR.tick_timer(&this->current, &this->subframe);
        return this->current;
    }

    forceinline void set_default(int32_t current = 0, int32_t previous = -999) {
        this->current = current;
        this->subframe = 0.0f;
        this->previous = previous;
    }

    // 0x424127
    dllexport Timer* thiscall initialize() asm_symbol_rel(codecave:TIMER_initialize) {
        this->default_values();
        return this;
    }

    // 0x424154
    dllexport gnu_noinline void thiscall increment(int32_t count) asm_symbol_rel(codecave:TIMER_increment);
    // 0x4241E5
    dllexport gnu_noinline void thiscall decrement(int32_t count) asm_symbol_rel(codecave:TIMER_decrement);
};

template <int32_t current, int32_t prev = -999>
static inline constexpr Timer static_default_timer = (Timer){ prev, 0.0f, current };

// 0x424145
dllexport gnu_noinline void thiscall Timer::increment(int32_t count) {
    if (SUPERVISOR.slowdown > 0.99f) {
        this->current += count;
    } else if (count < 0) {
        this->decrement(-count);
    } else {
        this->previous = this->current;
        this->subframe += (float)count * SUPERVISOR.game_speed;
        while (this->subframe >= 1.0f) {
            this->current++;
            this->subframe -= 1.0f;
        }
    }
}

// 0x4241E5
dllexport gnu_noinline void thiscall Timer::decrement(int32_t count) {
    if (SUPERVISOR.slowdown > 0.99f) {
        this->current -= count;
    } else if (count < 0) {
        this->increment(-count);
    } else {
        this->previous = this->current;
        this->subframe -= (float)count * SUPERVISOR.game_speed;
        while (this->subframe < 0.0f) {
            this->current--;
            this->subframe += 1.0f;
        }
    }
}

forceinline void float3_make_from_vector(Float3* self, float angle, float magnitude) {
#ifndef __x86_64__
    __asm {
        MOV EAX, self;
        FLD angle;
        FSINCOS;
        FMUL magnitude;
        FSTP DWORD PTR [EAX]
        FMUL magnitude;
        FSTP DWORD PTR [EAX+4]
    };
#else
    __asm {
        MOV RAX, self;
        FLD angle;
        FSINCOS;
        FMUL magnitude;
        FSTP DWORD PTR [RAX]
        FMUL magnitude;
        FSTP DWORD PTR [RAX+4]
    };
#endif
}

#define read_float3_vec(float3) \
*(vec<float, 4>*)&(float3)
#define write_float3_vec(float3, val) \
shufflevec((float3), (val), 0, 1, 2, 7)


static constexpr __v4su Float3VecMask = { UINT32_MAX, UINT32_MAX, UINT32_MAX, 0 };

//float reduce_angle(float angle) {
//    int count = 0;
//    while (angle > PI_f) {
//        angle -= TWO_PI_f;
//        if (count++ > 16) break;
//    }
//    while (angle < NPI_f) {
//        angle += TWO_PI_f;
//        if (count++ > 16) break;
//    }
//    return angle;
//}

#define float_wrap_neg_n_to_n(_value, _bounds) \
([](float value) -> float { \
    if constexpr ((_bounds) == PI_f) { \
        __asm__( \
            "FLDS %[TWO_PI] \n" \
            "FLDS %[VALUE] \n" \
            "FPREM1 \n" \
            "FSTPS %[VALUE] \n" \
            "FFREEP %%st(0) \n" \
            : [VALUE]"+m"(value) \
            : [TWO_PI]"m"(TWO_PI_f) \
        ); \
    } else { \
        float bounds = _bounds; \
        bounds += bounds; \
        __asm__( \
            "FLDS %[BOUNDS] \n" \
            "FLDS %[VALUE] \n" \
            "FPREM1 \n" \
            "FSTPS %[VALUE] \n" \
            "FFREEP %%st(0) \n" \
            : [VALUE]"+m"(value) \
            : [BOUNDS]"m"(bounds) \
        ); \
    } \
    return value; \
}(_value))

#define pre_lite_reduce_angle() \
__asm__ ("FLDS %[TWO_PI]" ::[TWO_PI]"m"(TWO_PI_f))

#define lite_reduce_angle(_value) \
([](float value) -> float { \
    __asm__( \
        "FLDS %[ANGLE] \n" \
        "FPREM1 \n" \
        "FSTPS %[ANGLE] \n" \
        : [ANGLE]"+m"(value) \
    ); \
    return value; \
}(_value))

#define post_lite_reduce_angle() \
__asm__ ("FFREEP %%st(0)":)

//dllexport float reduce_angle(float value, float initial_add) asm("{[codecave:reduce_angle]}");
//dllexport float reduce_angle(float value, float initial_add) {
//    vec<float, 4> temp = { value + initial_add, 0.0f, 0.0f, 0.0f };
//    temp[0] *= RECIP_TWO_PI_f;
//    temp = _mm_round_ss(temp, temp, 8);
//    return value + temp[0] * NTWO_PI_f;
//}

dllexport gnu_attr(target("no-sse")) float cdecl reduce_angle(float value, float initial_add) asm("{[codecave:reduce_angle]}");
dllexport gnu_attr(target("no-sse")) float cdecl reduce_angle(float value, float initial_add) {
    float ret;
    float no;
    __asm__(
        "FLDS %[TWO_PI] \n"
        "FLDS %[ANGLE] \n"
        "FADDS %[INITIAL_ADD] \n"
        "FPREM1 \n"
        : "=t"(ret), "=u"(no)
        : [ANGLE] "m"(value), [INITIAL_ADD]"m"(initial_add), [TWO_PI]"m"(TWO_PI_f)
    );
    return ret;
    /*__asm__(
        "FLDS %2 \n"
        "FLDS %0 \n"
        "FADDS %1 \n"
        "FPREM1 \n"
        "FSTP %%st(1) \n"
        :
        : "m"(value), "m"(initial_add), "m"(TWO_PI_f)
    );*/
}

dllexport inline float cdecl reduce_angle_new(float value, float initial_add) {
    value += initial_add;
    vec<double, 2> temp = { (double)value * ONE_OVER_TWO_PI_d };
    vec<double, 2> temp2 = _mm_round_sd(temp, temp, 0x8);
    temp[0] -= temp2[0];
    return temp[0] * TWO_PI_d;
};

// size: 0x1C
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 (0x144)
struct SpriteVertex {
    Float4 position; // 0x0
    D3DCOLOR diffuse; // 0x10
    Float2 texture_uv; // 0x14
    // 0x1C
    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};
// size: 0x18
// D3DFVF_XYZRHW | D3DFVF_TEX1 (0x104)
struct SpriteVertexB {
    Float4 position; // 0x0
    Float2 texture_uv; // 0x10
    // 0x18
    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_TEX1;
};
// size: 0x18
// D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 (0x142)
struct SpriteVertexC {
    Float3 position; // 0x0
    D3DCOLOR diffuse; // 0xC
    Float2 texture_uv; // 0x10
    // 0x18
    static constexpr DWORD FVF_TYPE = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};

extern "C" {
    // 0x6D4590
    extern SpriteVertex SPRITE_VERTEX_BUFFER_A[4] asm("_SPRITE_VERTEX_BUFFER_A");
    // 0x6D4600
    extern SpriteVertexB SPRITE_VERTEX_BUFFER_B[4] asm("_SPRITE_VERTEX_BUFFER_B");
    // 0x6D4660
    extern SpriteVertexC SPRITE_VERTEX_BUFFER_C[4] asm("_SPRITE_VERTEX_BUFFER_C");
};

// ====================
// UpdateFunc
// ====================

typedef struct UpdateFunc UpdateFunc;

#define SetInstr(value) \
current_instruction = (decltype(current_instruction))(value)

#define IndexInstr(offset) \
SetInstr((intptr_t)current_instruction + (offset))

#define ByteArg(number) \
(((uint8_t*)current_instruction->args)[(number)])

#define OneBitArg(number) \
(ByteArg(number) & 0b1)
#define ThreeBitArg(number) \
(ByteArg(number) & 0b111)

#define ShortArg(number) \
(((int16_t*)current_instruction->args)[(number)])

#define RawArg(number) \
(((EclArg*)current_instruction->args)[(number)])

#define IntArg(number) \
(((int32_t*)current_instruction->args)[(number)])

#define UIntArg(number) \
(((uint32_t*)current_instruction->args)[(number)])

#define FloatArg(number) \
(((float*)current_instruction->args)[(number)])

#define Float2Arg(number) \
(((Float2*)current_instruction->args)[(number)])

#define Float2ArgVec(number) \
(((vec<float, 2>*)current_instruction->args)[(number)])

#define Float3Arg(number) \
(((Float3*)current_instruction->args)[(number)])

#define StringArg(offset) \
((const char*)(current_instruction->args + (offset)))

#define TypeArg(type, number) \
(((type*)current_instruction->args)[(number)])

// ====================
// ANM, SOUND
// ====================

// size: 0x8
struct SoundEffectData {
    int32_t id; // 0x0
    int16_t sample_count; // 0x4
    int16_t __short_6; // 0x6
    // 0x8
};

extern "C" {
    // 0x478528
    extern SoundEffectData SOUND_EFFECT_DATA[32] asm("_UnknownSoundData_Table");
}

struct CWaveFileB {
    void* __ptr_0; // 0x0
    int __dword_4; // 0x4
    unknown_fields(0x28); // 0x8
    int __dword_30; // 0x30
    unknown_fields(0x48); // 0x34
    int __dword_7C; // 0x7C
    int __dword_80; // 0x80
    unknown_fields(0xC); // 0x84
    int __dword_90; // 0x90
    int __dword_94; // 0x94
    // 0x98
};

struct CSoundB {
    void* __ptr_0; // 0x0
    void* __ptr_4; // 0x4
    CWaveFile* m_pWaveFile; // 0x8
    unknown_fields(0x8); // 0xC
    int __dword_14; // 0x14
    int __dword_18; // 0x18
    int __dword_1C; // 0x1C
    // 0x20
};
ValidateFieldOffset(0x0, CSoundB, __ptr_0);
ValidateFieldOffset(0x4, CSoundB, __ptr_4);
ValidateFieldOffset(0x8, CSoundB, m_pWaveFile);
ValidateFieldOffset(0x14, CSoundB, __dword_14);
ValidateFieldOffset(0x18, CSoundB, __dword_18);
ValidateFieldOffset(0x1C, CSoundB, __dword_1C);

// size: 0x638
struct SoundManager {
    LPDIRECTSOUND dsound_api; // 0x0
    unknown_fields(0x4); // 0x4
    void* __ptrs_8[128]; // 0x8
    void* __ptrs_208[128]; // 0x208
    int32_t __array_408[128]; // 0x408
    void* __ptr_608; // 0x608
    HWND window; // 0x60C
    LPDIRECTSOUND* dsound; // 0x610
    int __dword_614; // 0x614
    int __dword_618; // 0x618
    unknown_fields(0x4); // 0x61C
    int32_t __sound_index_array[3]; // 0x620
    CSoundB* c_sound_ptr; // 0x62C
    int __dword_630; // 0x630
    unknown_fields(0x4); // 0x634
    // 0x638

    // 0x4311E0
    dllexport gnu_noinline void thiscall play_sound_centered(int32_t sound_index, int32_t = 0) asm_symbol_rel(codecave:SOUND_MANAGER_play_sound_centered) {
        int32_t intA = SOUND_EFFECT_DATA[sound_index].__short_6;
        int32_t i = 0;
        for (; i < countof(this->__sound_index_array); ++i) {
            if (this->__sound_index_array[i] < 0) {
                break;
            }
            if (this->__sound_index_array[i] == sound_index) {
                return;
            }
        }
        if (i >= countof(this->__sound_index_array)) {
            return;
        }
        this->__sound_index_array[i] = sound_index;
        this->__array_408[sound_index] = intA;
    }

    // 0x424E41
    dllexport gnu_noinline void thiscall __sub_424E41(float arg1) asm_symbol_rel(codecave:SOUND_MANAGER_sub_424E41) {
        if (this->c_sound_ptr) {
            CSoundB* c_sound = this->c_sound_ptr;
            c_sound->__dword_1C = 1;
            c_sound->__dword_14 = arg1 * 60.0f;
            c_sound->__dword_18 = c_sound->__dword_14;
        }
    }

};

extern "C" {
    // 0x6D3F50
    extern SoundManager SOUND_MANAGER asm("_SOUND_MANAGER");
}

// size: 0x38
struct AnmLoadedSprite {
    int32_t source_file_index; // 0x0
    Float2 start_pixel_inclusive; // 0x4
    Float2 end_pixel_inclusive; // 0xC
    union {
        Float2 texture_size;
        struct {
            float texture_height; // 0x14
            float texture_width; // 0x18
        };
    };
    Float2 uv_start; // 0x1C
    Float2 uv_end; // 0x24
    union {
        Float2 sprite_size;
        struct {
            float height_px; // 0x2C
            float width_px; // 0x30
        };
    };
    int32_t sprite_id; // 0x34
    // 0x38
};  

// size: 0x2C
struct AnmInstruction {
    int16_t time; // 0x0
    uint8_t opcode; // 0x2
    uint8_t argsize; // 0x3
    unsigned char args[]; // 0x4
    // 0x2C
}; 

// size: 0x8
struct AnmScript {
    uint32_t id; // 0x0
    AnmInstruction* first_instr; // 0x4
    // 0x8
};  

// size: 0x14
struct AnmSprite {
    uint32_t id; // 0x0
    Float2 offset; // 0x4
    Float2 size; // 0xC
    // 0x14
};

// size: 0x10
struct AnmTexture {
    ZUNMagic magic; // 0x0 (THTX)
    uint16_t __zero; // 0x4
    uint16_t format; // 0x6
    uint16_t width; // 0x8
    uint16_t height; // 0xA
    uint32_t num_bytes; // 0xC
    unsigned char data[]; // 0x10
};


struct AnmEntry {
    int32_t num_sprites; // 0x0
    int32_t num_scripts; // 0x4
    int32_t rt_texture_slot; // 0x8
    uint32_t width; // 0xC
    uint32_t height; // 0x10
    uint32_t format; // 0x14
    D3DCOLOR colorkey; // 0x18
    char* path_1; // 0x1C
    uint32_t __unknown__used_at_runtime; // 0x20
    char* path_2; // 0x24
    uint32_t version; // 0x28
    uint32_t memory_priority; // 0x2C
    AnmTexture* texture; // 0x30
    uint16_t __has_data__unused_in_EoSD; // 0x34
    uint16_t __unused_2; // 0x36
    uint32_t offset_to_next; // 0x38
    uint32_t __unused_3; // 0x3C
    AnmSprite* sprites[10]; // 0x40
    AnmScript scripts[10]; // 0x68
    // 0xB8
};

// size: 0x110
struct AnmVM {
    Float3 rotation; // 0x0
    Float3 angular_velocity; // 0xC
    union {
        Float2 scale; // 0x18
        struct {
            float scale_y; // 0x18
            float scale_x; // 0x1C
        };
    };
    union {
        Float2 scale_interp_final; // 0x20
        struct {
            float scale_interp_final_y; // 0x20
            float scale_interp_final_x; // 0x24
        };
    };
    Float2 uv_scroll_pos; // 0x28
    Timer script_time; // 0x30
    D3DMATRIX __matrix; // 0x3C
    union {
        D3DCOLOR color; // 0x7C
        uint8_t color_non_aplha[3];
        uint8_t color_all[4];
        vec<uint8_t, 4> color_vec gnu_attr(packed);
        struct {
            uint8_t blue;
            uint8_t green;
            uint8_t red;
            uint8_t alpha;
        };
    };
    union {
        uint16_t flags; // 0x80
        struct {
            union {
                uint8_t flags_80; // 0x80
                struct {
                    uint8_t visible : 1; // 0x0001
                    uint8_t __unknown_flag_B : 1; // 0x0002
                    uint8_t additive_blending : 1; // 0x0004
                    uint8_t __unknown_flag_D : 1; // 0x0008
                    uint8_t __unknown_flag_E: 1; // 0x0010
                    uint8_t position_mode : 1; // 0x0020
                    uint8_t flip_x : 1; // 0x0040
                    uint8_t flip_y : 1; // 0x0080
                };
            };
            union {
                uint8_t flags_81; // 0x81
                struct {
                    uint8_t anchor_mode : 2; // 0x0100-0x0200
                    uint8_t move_mode : 2; // 0x0400-0x0800
                    uint8_t z_write_disable : 1; // 0x1000
                    uint8_t stop_enable : 1; // 0x2000
                };
            };
        };
    };
    probably_padding_bytes(0x2); // 0x82
    int16_t alpha_interp_end_time; // 0x84
    int16_t scale_interp_end_time; // 0x86
    uint16_t __auto_rotate; // 0x88
    int16_t pending_interrupt_label; // 0x8A
    int16_t position_interp_end_time; // 0x8C
    unknown_fields(0x2); // 0x8E
    Float3 position; // 0x90
    union {
        Float2 scale_interp_initial; // 0x9C
        struct {
            float scale_interp_initial_y; // 0x9C
            float scale_interp_initial_x; // 0xA0
        };
    };
    Timer scale_interp_timer; // 0xA4
    int16_t sprite_number; // 0xB0
    int16_t base_sprite_number; // 0xB2
    int16_t script_number; // 0xB4
    int16_t base_script_number; // 0xB6
    AnmInstruction* beginning_of_script; // 0xB8
    AnmInstruction* current_instr; // 0xBC
    AnmLoadedSprite* sprite; // 0xC0
    union {
        D3DCOLOR alpha_interp_initial; // 0xC4
        uint8_t alpha_interp_initial_non_alpha[3];
        uint8_t alpha_interp_initial_all[4];
        vec<uint8_t, 4> alpha_interp_initial_vec gnu_attr(packed);
        struct {
            uint8_t alpha_interp_initial_blue;
            uint8_t alpha_interp_initial_green;
            uint8_t alpha_interp_initial_red;
            uint8_t alpha_interp_initial_alpha;
        };
    };
    union {
        D3DCOLOR alpha_interp_final; // 0xC8
        uint8_t alpha_interp_final_non_alpha[3];
        uint8_t alpha_interp_final_all[4];
        vec<uint8_t, 4> alpha_interp_final_vec gnu_attr(packed);
        struct {
            uint8_t alpha_interp_final_blue;
            uint8_t alpha_interp_final_green;
            uint8_t alpha_interp_final_red;
            uint8_t alpha_interp_final_alpha;
        };
    };
    Float3 position_interp_initial; // 0xCC
    Float3 position_interp_final; // 0xD8
    Float3 __position_2; // 0xE4
    Timer position_interp_timer; // 0xF0
    int32_t time_of_last_sprite_set; // 0xFC
    Timer alpha_interp_timer; // 0x100
    uint8_t font_width; // 0x10C
    uint8_t font_height; // 0x10D
    probably_padding_bytes(0x2); // 0x10E
    // 0x110

    // 0x403580
    dllexport void thiscall initialize() asm("{[codecave:ANM_VM_initialize]}") {
        this->uv_scroll_pos = { 0.0f, 0.0f };
        this->scale_interp_final = { 0.0f, 0.0f };
        this->angular_velocity = { 0.0f, 0.0f, 0.0f };
        this->rotation = { 0.0f, 0.0f, 0.0f };
        this->scale = { 1.0f, 1.0f };
        this->scale_interp_end_time = 0;
        this->alpha_interp_end_time = 0;
        this->color = PackD3DCOLOR(255, 255, 255, 255);
        matrix_set_identity(this->__matrix);
        //this->flags = 3;
        this->visible = true;
        this->__unknown_flag_B = true;
        this->__auto_rotate = 0;
        this->pending_interrupt_label = 0;
        this->position_interp_end_time = 0;
        this->script_time.initialize();
    }

    inline void set_script(int32_t index);
};

dllexport gnu_noinline void cdecl __sub_41F050(
    int32_t x_pos, int32_t y_pos, int32_t width, int32_t height,
    int32_t font_width, int32_t font_height, D3DCOLOR colorA, D3DCOLOR colorB, const char *restrict text,
    IDirect3DTexture8 *restrict texture
) asm_symbol_rel(0x41F050_old);
dllexport gnu_noinline void cdecl __sub_41F050(
    int32_t x_pos, int32_t y_pos, int32_t width, int32_t height,
    int32_t font_width, int32_t font_height, D3DCOLOR colorA, D3DCOLOR colorB, const char *restrict text,
    IDirect3DTexture8 *restrict texture
) {
    gnu_used static volatile auto tempA = x_pos;
    gnu_used static volatile auto tempB = y_pos;
    gnu_used static volatile auto tempC = width;
    gnu_used static volatile auto tempD = height;
    gnu_used static volatile auto tempE = font_width;
    gnu_used static volatile auto tempF = font_height;
    gnu_used static volatile auto tempG = colorA;
    gnu_used static volatile auto tempH = colorB;
    gnu_used static volatile auto tempJ = text;
    gnu_used static volatile auto tempI = texture;
    assume_all_registers_volatile();
}

extern "C" {
    extern const char SingleSpaceString[2] asm("_SingleSpaceString");
}

// size: 0x18
struct RenderDataFormatLookupData {
    D3DFORMAT format; // 0x0
    int32_t bits_per_pixel; // 0x4
    uint32_t alpha_mask; // 0x8
    uint32_t red_mask; // 0xC
    uint32_t green_mask; // 0x10
    uint32_t blue_mask; // 0x14
    // 0x18
};

extern "C" {
    extern RenderDataFormatLookupData RenderDataFormatLookupDataTable[7] asm("_RenderDataFormatLookupDataTable");
}

// size: 0x6C
struct Custom_BITMAPINFO {
    BITMAPINFOHEADER bmiHeader; // 0x0
    Custom_RGBAQUAD  bmiColors[1]; // 0x28
    unknown_fields(0x34); // 0x38
    // 0x6C
};

static constexpr auto wkrbwejkrb = offsetof(Custom_BITMAPINFO, bmiColors);

struct BitmapRenderData {
    D3DFORMAT format; // 0x0
    int32_t width; // 0x4
    int32_t height; // 0x8
    size_t raw_bitmap_size; // 0xC
    int32_t bytes_per_row; // 0x10
    HDC device_context; // 0x14
    HGDIOBJ screen_bitmap_object; // 0x18
    HBITMAP bitmap_handle; // 0x1C
    void* raw_bitmap_bits; // 0x20
    // 0x24
    
    inline void initialize() {
        this->format = (D3DFORMAT)-1;
        this->width = 0;
        this->height = 0;
        this->bitmap_handle = NULL;
        this->screen_bitmap_object = NULL;
        this->raw_bitmap_bits = 0;
    }
    
    BitmapRenderData() {
        this->initialize();
    }
    
    // 0x41E992
    dllexport gnu_noinline bool thiscall reset() asm_symbol_rel(0x41E992) {
        if (this->device_context) {
            SelectObject(this->device_context, this->screen_bitmap_object);
            DeleteDC(this->device_context);
            DeleteObject(this->bitmap_handle);
            this->initialize();
            return true;
        }
        return false;
    }
    
    // 0x41E981
    dllexport gnu_noinline ~BitmapRenderData() {
        this->reset();
    }
    
    // 0x41EC22
    dllexport gnu_noinline RenderDataFormatLookupData* thiscall __sub_41EC22(D3DFORMAT format) asm_symbol_rel(0x41EC22) {
        int32_t i = 0;
        for (;
            RenderDataFormatLookupDataTable[i].format != (D3DFORMAT)-1 &&
            RenderDataFormatLookupDataTable[i].format != format;
            ++i
        );
        if (format == (D3DFORMAT)-1) {
            return NULL;
        }
        return &RenderDataFormatLookupDataTable[i];
    }
    
    // 0x41EA63
    dllexport gnu_noinline bool thiscall __sub_41EA63(int32_t width, int32_t height, D3DFORMAT format) asm_symbol_rel(0x41EA63) {
        this->reset();
        Custom_BITMAPINFO bitmap_info;
        memset(&bitmap_info, 0, sizeof(Custom_BITMAPINFO));
        RenderDataFormatLookupData* render_data_ptr = this->__sub_41EC22(format);
        if (!render_data_ptr) {
            return false;
        }
        int32_t bytes_per_row = dword_align(width * render_data_ptr->bits_per_pixel / CHAR_BIT) * sizeof(DWORD);
        bitmap_info.bmiHeader.biSize = sizeof(Custom_BITMAPINFO);
        bitmap_info.bmiHeader.biWidth = width;
        bitmap_info.bmiHeader.biHeight = -(height + 1); // Some kind of crappy ~?
        bitmap_info.bmiHeader.biPlanes = 1;
        bitmap_info.bmiHeader.biBitCount = render_data_ptr->bits_per_pixel;
        bitmap_info.bmiHeader.biSizeImage = height * bytes_per_row;
        if (format != D3DFMT_X1R5G5B5 && format != D3DFMT_X8R8G8B8) {
            bitmap_info.bmiHeader.biCompression = BI_BITFIELDS;
            bitmap_info.bmiColors[0].red_mask = render_data_ptr->red_mask;
            bitmap_info.bmiColors[0].green_mask = render_data_ptr->green_mask;
            bitmap_info.bmiColors[0].blue_mask = render_data_ptr->blue_mask;
            bitmap_info.bmiColors[0].alpha_mask = render_data_ptr->alpha_mask;
        }
        void* raw_bitmap_bits;
        HBITMAP bitmap_handle = CreateDIBSection(NULL, (const BITMAPINFO*)&bitmap_info, DIB_RGB_COLORS, &raw_bitmap_bits, NULL, 0);
        if (!bitmap_handle) {
            return false;
        }
        memset(raw_bitmap_bits, 0, bitmap_info.bmiHeader.biSizeImage);
        HDC device_context = CreateCompatibleDC(NULL);
        HGDIOBJ screen_bitmap_object = SelectObject(device_context, bitmap_handle);
        this->device_context = device_context;
        this->bitmap_handle = bitmap_handle;
        this->raw_bitmap_bits = raw_bitmap_bits;
        this->raw_bitmap_size = bitmap_info.bmiHeader.biSizeImage;
        this->screen_bitmap_object = screen_bitmap_object;
        this->width = width;
        this->height = height;
        this->format = format;
        this->bytes_per_row = bytes_per_row;
        return true;
    }
    
    // 0x41EA04
    dllexport gnu_noinline bool thiscall __sub_41EA04(int32_t width, int32_t height, D3DFORMAT format) asm_symbol_rel(0x41EA04) {
        if (this->__sub_41EA63(width, height, format)) {
            return true;
        }
        if (format == D3DFMT_A1R5G5B5 || format == D3DFMT_A4R4G4B4) {
            return this->__sub_41EA63(width, height, D3DFMT_A8R8G8B8);
        }
        if (format == D3DFMT_R5G6B5) {
            return this->__sub_41EA63(width, height, D3DFMT_X8R8G8B8);
        }
        return false;
    }
    
    // 0x41EC72
    dllexport gnu_noinline bool thiscall __sub_41EC72(int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) asm_symbol_rel(0x41EC72) {
        gnu_used static volatile auto tempA = arg1;
        gnu_used static volatile auto tempB = arg2;
        gnu_used static volatile auto tempC = arg3;
        gnu_used static volatile auto tempD = arg4;
        return 0;
    }
    
    // 0x41EF11
    dllexport gnu_noinline bool thiscall __sub_41EF11(LPDIRECT3DSURFACE8 surface) asm_symbol_rel(0x41EF11) {
        if (!(bool)(this->bitmap_handle != NULL)) {
            return false;
        }
        D3DSURFACE_DESC surface_desc; // EBP-40
        surface->GetDesc(&surface_desc);

        RECT rect; // EBP-18
        rect.left = 0;
        rect.top = 0;
        int32_t width = this->width; // EBP-50
        rect.right = width;
        int32_t height = this->height; // EBP-54
        rect.bottom = height;

        D3DLOCKED_RECT locked_rect; // EBP-4C
        if (surface->LockRect(&locked_rect, &rect, 0) != D3D_OK) {
            return false;
        }
        int32_t pitch = locked_rect.Pitch; // EBP-8
        int32_t bytes_per_row = this->bytes_per_row; // EBP-20
        uint8_t* raw_bitmap_bits = (uint8_t*)this->raw_bitmap_bits; // EBP-44
        uint8_t* locked_bits = (uint8_t*)locked_rect.pBits; // EBP-4
        D3DFORMAT format = this->format; // EBP-58
        if (surface_desc.Format == format) {
            int32_t height; // EBP-5C
            for (int32_t i = 0; i < (height = this->height); i++) {
                memcpy(locked_bits, raw_bitmap_bits, bytes_per_row);
                raw_bitmap_bits += bytes_per_row;
                locked_bits += pitch;
            }
        }
        surface->UnlockRect();
    }
};

extern "C" {
    // 0x69E230
    extern LPDIRECT3DSURFACE8 UNKNOWN_SURFACE_A asm("_UNKNOWN_SURFACE_A");
}

// 0x41F008
dllexport gnu_noinline void create_global_surface() asm_symbol_rel(codecave:create_global_surface);
dllexport gnu_noinline void create_global_surface() {
    SUPERVISOR.d3d_device->CreateImageSurface(640, 64, D3DFMT_A1R5G5B5, &UNKNOWN_SURFACE_A);
}

// 0x41F02B
dllexport gnu_noinline void release_global_surface() asm_symbol_rel(codecave:release_global_surface);
dllexport gnu_noinline void release_global_surface() {
    SAFE_RELEASE(UNKNOWN_SURFACE_A);
}

// 0x41F050
dllexport gnu_noinline void cdecl render_text(int32_t x_pos, int32_t y_pos, int32_t width, int32_t height, int32_t font_width, int32_t font_height, D3DCOLOR colorA, D3DCOLOR colorB, char* text, IDirect3DTexture8* texture) asm_symbol_rel(0x41F050);
dllexport gnu_noinline void cdecl render_text(
    int32_t x_pos, // 0x8
    int32_t y_pos, // 0xC
    int32_t width, // 0x10
    int32_t height, // 0x14
    int32_t font_width, // 0x18
    int32_t font_height, // 0x1C
    D3DCOLOR colorA, // 0x20
    D3DCOLOR colorB, // 0x24
    char* text, // 0x28
    LPDIRECT3DTEXTURE8 texture // 0x2C
) {
    // EBP-14
    HFONT font_handle = CreateFontA(font_width * 2, 0, 0, 0, FW_BOLD, false, false, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_ROMAN, "‚l‚r ƒSƒVƒbƒN");
    BitmapRenderData bitmap_data; // EBP-5C
    bitmap_data.initialize();
    D3DSURFACE_DESC surface_desc; // EBP-34
    UNKNOWN_SURFACE_A->GetDesc(&surface_desc);
    bitmap_data.__sub_41EA04(surface_desc.Width, surface_desc.Height, surface_desc.Format);
    HDC screen_context = bitmap_data.device_context; // EBP-10
    HGDIOBJ screen_font_object = SelectObject(screen_context, font_handle); // EBP-38
    bitmap_data.__sub_41EC72(0, 0, width * 2, font_width * 2 + 6);
    SetBkMode(screen_context, TRANSPARENT);
    if (colorB != PackD3DCOLOR(255, 255, 255, 255)) {
        SetTextColor(screen_context, colorB);
        TextOutA(screen_context, x_pos * 2 + 6, 2, text, strlen(text));
    }
    SetTextColor(screen_context, colorA);
    TextOutA(screen_context, x_pos * 2, 0, text, strlen(text));
    (void)SelectObject(screen_context, screen_font_object);
    bitmap_data.__sub_41EC72(0, 0, width * 2, font_width * 2 + 6);
    bitmap_data.__sub_41EF11(UNKNOWN_SURFACE_A);
    (void)SelectObject(screen_context, screen_font_object);
    DeleteObject(font_handle);
    RECT dest_rect; // EBP-7C
    dest_rect.left = 0;
    dest_rect.top = y_pos;
    dest_rect.right = width;
    dest_rect.bottom = y_pos + 10;
    RECT src_rect; // EBP-6C
    src_rect.left = 0;
    src_rect.top = 0;
    src_rect.right = width * 2 - 2;
    src_rect.bottom = font_width * 2 - 2;
    LPDIRECT3DSURFACE8 dest_surface; // EBP-80
    texture->GetSurfaceLevel(0, &dest_surface);
    D3DXLoadSurfaceFromSurface(dest_surface, NULL, &dest_rect, UNKNOWN_SURFACE_A, NULL, &src_rect, D3DX_FILTER_TRIANGLE, 0);
    SAFE_RELEASE(dest_surface);
    int32_t wtf = -1;
}


// size: 0x2112C
struct AnmManager {
    AnmLoadedSprite sprites[0x800]; // 0x0
    AnmVM __vm_1C000; // 0x1C000
    LPDIRECT3DTEXTURE8 textures[0x108]; // 0x1C110
    void* image_data_array[0x100]; // 0x1C530
    int32_t __maybe_loaded_sprite_count; // 0x1C930
    AnmInstruction* scripts[0x800]; // 0x1C934
    int32_t sprite_indices[0x800]; // 0x1E934
    AnmEntry* anm_files[0x80]; // 0x20934
    uint32_t anm_file_sprite_index_offsets[0x80]; // 0x20B34
    LPDIRECT3DSURFACE8 surfaces_a[0x20]; // 0x20D34
    LPDIRECT3DSURFACE8 surfaces_b[0x20]; // 0x20DB4
    D3DXIMAGE_INFO surface_source_info[0x20]; // 0x20E34
    D3DCOLOR current_texture_factor; // 0x210B4
    LPDIRECT3DTEXTURE8 current_texture; // 0x210B8
    uint8_t current_blend_mode; // 0x210BC
    uint8_t current_colorop; // 0x210BD
    uint8_t current___uhhhhh; // 0x210BE
    uint8_t current_zwrite_disable; // 0x210BF
    AnmLoadedSprite* current_sprite; // 0x210C0
    LPDIRECT3DVERTEXBUFFER8 stream_source; // 0x210C4
    SpriteVertexC __some_vertex_buffer[0x4]; // 0x210C8
    int32_t __weird_texture_height; // 0x21128
    // 0x2112C

    // 0x432730
    dllexport gnu_noinline ZunResult thiscall __sub_432730(AnmVM* VM, bool round_inputs) asm_symbol_rel(0x432730) {
        if (round_inputs) {
            long double A = CRT::rint_asm(SPRITE_VERTEX_BUFFER_B[0].position.x) - 0.5f;
            long double B = CRT::rint_asm(SPRITE_VERTEX_BUFFER_B[1].position.x) - 0.5f;
            long double C = CRT::rint_asm(SPRITE_VERTEX_BUFFER_B[0].position.y) - 0.5f;
            long double D = CRT::rint_asm(SPRITE_VERTEX_BUFFER_B[2].position.y) - 0.5f;
            SPRITE_VERTEX_BUFFER_B[2].position.y = D;
            SPRITE_VERTEX_BUFFER_B[3].position.y = D;
            SPRITE_VERTEX_BUFFER_B[0].position.y = C;
            SPRITE_VERTEX_BUFFER_B[1].position.y = C;
            SPRITE_VERTEX_BUFFER_B[1].position.x = B;
            SPRITE_VERTEX_BUFFER_B[3].position.x = B;
            SPRITE_VERTEX_BUFFER_B[0].position.x = A;
            SPRITE_VERTEX_BUFFER_B[2].position.x = A;
        }
        SPRITE_VERTEX_BUFFER_B[3].position.z = VM->position.z;
        SPRITE_VERTEX_BUFFER_B[2].position.z = SPRITE_VERTEX_BUFFER_B[3].position.z;
        SPRITE_VERTEX_BUFFER_B[1].position.z = SPRITE_VERTEX_BUFFER_B[2].position.z;
        SPRITE_VERTEX_BUFFER_B[0].position.z = SPRITE_VERTEX_BUFFER_B[1].position.z;
        if (this->current_sprite != VM->sprite) {
            this->current_sprite = VM->sprite;
            SPRITE_VERTEX_BUFFER_B[2].texture_uv.x = VM->sprite->uv_start.x + VM->uv_scroll_pos.x;
            SPRITE_VERTEX_BUFFER_B[0].texture_uv.x = SPRITE_VERTEX_BUFFER_B[2].texture_uv.x;
            SPRITE_VERTEX_BUFFER_B[3].texture_uv.x = VM->sprite->uv_end.x + VM->uv_scroll_pos.x;
            SPRITE_VERTEX_BUFFER_B[1].texture_uv.x = SPRITE_VERTEX_BUFFER_B[3].texture_uv.x;
            SPRITE_VERTEX_BUFFER_B[1].texture_uv.y = VM->sprite->uv_start.y + VM->uv_scroll_pos.y;
            SPRITE_VERTEX_BUFFER_B[0].texture_uv.y = SPRITE_VERTEX_BUFFER_B[1].texture_uv.y;
            SPRITE_VERTEX_BUFFER_B[3].texture_uv.y = VM->sprite->uv_end.y + VM->uv_scroll_pos.y;
            SPRITE_VERTEX_BUFFER_B[2].texture_uv.y = SPRITE_VERTEX_BUFFER_B[3].texture_uv.y;
            if (this->current_texture != this->textures[VM->sprite->source_file_index]) {
                this->current_texture = this->textures[VM->sprite->source_file_index];
                SUPERVISOR.d3d_device->SetTexture(0, this->current_texture);
            }
        }
        if (this->current___uhhhhh != 2) {
            if (!SUPERVISOR.config.__unknown_flag_C) {
                SUPERVISOR.d3d_device->SetVertexShader(SpriteVertexB::FVF_TYPE);
            } else {
                SUPERVISOR.d3d_device->SetVertexShader(SpriteVertex::FVF_TYPE);
            }
            this->current___uhhhhh = 2;
        }
        this->set_render_state_for_vm(VM);
        if (!SUPERVISOR.config.__unknown_flag_C) {
            SUPERVISOR.d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, SPRITE_VERTEX_BUFFER_B, sizeof(SpriteVertexB));
        } else {
            SPRITE_VERTEX_BUFFER_A[0].position.x = SPRITE_VERTEX_BUFFER_B[0].position.x;
            SPRITE_VERTEX_BUFFER_A[0].position.y = SPRITE_VERTEX_BUFFER_B[0].position.y;
            SPRITE_VERTEX_BUFFER_A[0].position.z = SPRITE_VERTEX_BUFFER_B[0].position.z;
            SPRITE_VERTEX_BUFFER_A[1].position.x = SPRITE_VERTEX_BUFFER_B[1].position.x;
            SPRITE_VERTEX_BUFFER_A[1].position.y = SPRITE_VERTEX_BUFFER_B[1].position.y;
            SPRITE_VERTEX_BUFFER_A[1].position.z = SPRITE_VERTEX_BUFFER_B[1].position.z;
            SPRITE_VERTEX_BUFFER_A[2].position.x = SPRITE_VERTEX_BUFFER_B[2].position.x;
            SPRITE_VERTEX_BUFFER_A[2].position.y = SPRITE_VERTEX_BUFFER_B[2].position.y;
            SPRITE_VERTEX_BUFFER_A[2].position.z = SPRITE_VERTEX_BUFFER_B[2].position.z;
            SPRITE_VERTEX_BUFFER_A[3].position.x = SPRITE_VERTEX_BUFFER_B[3].position.x;
            SPRITE_VERTEX_BUFFER_A[3].position.y = SPRITE_VERTEX_BUFFER_B[3].position.y;
            SPRITE_VERTEX_BUFFER_A[3].position.z = SPRITE_VERTEX_BUFFER_B[3].position.z;
            SPRITE_VERTEX_BUFFER_A[2].texture_uv.x = VM->sprite->uv_start.x + VM->uv_scroll_pos.x;
            SPRITE_VERTEX_BUFFER_A[0].texture_uv.x = SPRITE_VERTEX_BUFFER_A[2].texture_uv.x;
            SPRITE_VERTEX_BUFFER_A[3].texture_uv.x = VM->sprite->uv_end.x + VM->uv_scroll_pos.x;
            SPRITE_VERTEX_BUFFER_A[1].texture_uv.x = SPRITE_VERTEX_BUFFER_A[3].texture_uv.x;
            SPRITE_VERTEX_BUFFER_A[1].texture_uv.y = VM->sprite->uv_start.y + VM->uv_scroll_pos.y;
            SPRITE_VERTEX_BUFFER_A[0].texture_uv.y = SPRITE_VERTEX_BUFFER_A[1].texture_uv.y;
            SPRITE_VERTEX_BUFFER_A[3].texture_uv.y = VM->sprite->uv_end.y + VM->uv_scroll_pos.y;
            SPRITE_VERTEX_BUFFER_A[2].texture_uv.y = SPRITE_VERTEX_BUFFER_A[3].texture_uv.y;
            SUPERVISOR.d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, SPRITE_VERTEX_BUFFER_A, sizeof(SpriteVertex));
        }
        return ZUN_SUCCESS;
    }

    // 0x432AD0
    dllexport gnu_noinline ZunResult __draw_vm_1(AnmVM* vm) asm_symbol_rel(0x432AD0) {
        if (!vm->visible) {
            return ZUN_ERROR;
        }
        if (!vm->__unknown_flag_B) {
            return ZUN_ERROR;
        }
        if (!vm->color) {
            return ZUN_ERROR;
        }
        float scaled_half_width = vm->sprite->width_px * vm->scale_x / 2.0f;
        float scaled_half_height = vm->sprite->height_px * vm->scale_y / 2.0f;
        if (!(vm->anchor_mode & 0b01)) {
            SPRITE_VERTEX_BUFFER_B[2].position.x = vm->position.x - scaled_half_width;
            SPRITE_VERTEX_BUFFER_B[0].position.x = SPRITE_VERTEX_BUFFER_B[2].position.x;
            SPRITE_VERTEX_BUFFER_B[3].position.x = vm->position.x + scaled_half_width;
            SPRITE_VERTEX_BUFFER_B[1].position.x = SPRITE_VERTEX_BUFFER_B[3].position.x;
        } else {
            SPRITE_VERTEX_BUFFER_B[2].position.x = vm->position.x;
            SPRITE_VERTEX_BUFFER_B[0].position.x = SPRITE_VERTEX_BUFFER_B[2].position.x;
            SPRITE_VERTEX_BUFFER_B[3].position.x = vm->position.x + scaled_half_width + scaled_half_width;
            SPRITE_VERTEX_BUFFER_B[1].position.x = SPRITE_VERTEX_BUFFER_B[3].position.x;
        }
        if (!(vm->anchor_mode & 0b10)) {
            SPRITE_VERTEX_BUFFER_B[1].position.y = vm->position.y - scaled_half_height;
            SPRITE_VERTEX_BUFFER_B[0].position.y = SPRITE_VERTEX_BUFFER_B[1].position.y;
            SPRITE_VERTEX_BUFFER_B[3].position.y = vm->position.y + scaled_half_height;
            SPRITE_VERTEX_BUFFER_B[2].position.y = SPRITE_VERTEX_BUFFER_B[3].position.y;
        } else {
            SPRITE_VERTEX_BUFFER_B[1].position.y = vm->position.y;
            SPRITE_VERTEX_BUFFER_B[0].position.y = SPRITE_VERTEX_BUFFER_B[1].position.y;
            SPRITE_VERTEX_BUFFER_B[3].position.y = vm->position.y + scaled_half_height + scaled_half_height;
            SPRITE_VERTEX_BUFFER_B[2].position.y = SPRITE_VERTEX_BUFFER_B[3].position.y;
        }
        return this->__sub_432730(vm, true);
    }

    dllexport void string_test(AnmVM* vm, ...) {
        va_list va;
        va_start(va, vm);
        unsigned char buffer[21];
        vsprintf((char*)buffer, "Continues Used: %hhu", va_use(va));
        va_end(va);
        const unsigned char* buffer_read = buffer;
        float initial_x_pos = vm->position.x;
        while (true) {
            switch (unsigned char c = *buffer_read++) {
                case '\0':
                    return;
                case '\n':
                    vm->position.x = initial_x_pos;
                    vm->position.y += 16.0f;
                    break;

                default:
                    assume(c < 128);
                    vm->sprite = &this->sprites[c - 10];
                    vm->color = PackD3DCOLOR(255, 255, 255, 255);
                    this->__draw_vm_1(vm);
                case ' ':
                    vm->position.x += 14.0f;
                    break;
            }
        }
    }

    // 0x432430
    dllexport void thiscall set_vm_script(AnmVM* vm, AnmInstruction* script_start) asm_symbol_rel(codecave:ANM_MANAGER_set_vm_script);

    // 0x4323A0
    dllexport gnu_noinline ZunResult thiscall set_vm_sprite(AnmVM* vm, int32_t sprite_number) asm_symbol_rel(codecave:ANM_MANAGER_set_vm_sprite) {
        if (this->sprites[sprite_number].source_file_index < 0) {
            return ZUN_ERROR;
        }
        vm->sprite_number = sprite_number;
        vm->sprite = &this->sprites[sprite_number];
        matrix_set_identity(vm->__matrix);
        vm->__matrix.m[0][0] = vm->sprite->width_px / vm->sprite->texture_width;
        vm->__matrix.m[1][1] = vm->sprite->height_px / vm->sprite->texture_height;
        return ZUN_SUCCESS;
    }

    // 0x4321E0
    dllexport gnu_noinline void thiscall free_texture(int32_t texture_index) asm_symbol_rel(codecave:ANM_MANAGER_free_texture) {
        SAFE_RELEASE(this->textures[texture_index]);
        free(this->image_data_array[texture_index]);
        this->image_data_array[texture_index] = NULL;
    }

    // 0x432030
    dllexport gnu_noinline void thiscall free_anm_file(int32_t file_index) asm_symbol_rel(codecave:ANM_MANAGER_free_anm_file) {
        if (this->anm_files[file_index]) {
            uint32_t sprite_index_offset = this->anm_file_sprite_index_offsets[file_index];
            AnmSprite* current_sprite = *this->anm_files[file_index]->sprites;

            AnmEntry* anm_file = this->anm_files[file_index]; // TODO: fix

            uint32_t sprite_count = anm_file->num_sprites;
            uint32_t *restrict current_sprite_offset = (uint32_t*)&anm_file->sprites;
            for (size_t ii = 0; ii < sprite_count; ++ii) {
                AnmLoadedSprite *restrict loaded_sprite = &this->sprites[sprite_index_offset + ((AnmSprite *restrict)((uintptr_t)anm_file + *current_sprite_offset++))->id];
                memset(loaded_sprite, 0, sizeof(AnmLoadedSprite));
                loaded_sprite->source_file_index = -1;
            }
            AnmScript *restrict current_script_offset = (AnmScript*)current_sprite_offset;
            uint32_t script_count = anm_file->num_scripts;
            for (size_t ii = 0; ii < script_count; ++ii) {
                int32_t script_index = current_script_offset++->id;
                this->scripts[script_index] = NULL;
                this->sprite_indices[script_index] = 0;
            }
            this->free_texture(anm_file->rt_texture_slot);
            thcrap_free(anm_file);
            this->current_blend_mode = -1;
            this->current_colorop = -1;
            this->current___uhhhhh = -1;
        }
    }

    // 0x431DC0
    dllexport gnu_noinline ZunResult thiscall load_anm_file(int32_t file_index, char *restrict filename, int32_t starting_sprite_index) asm_symbol_rel(0x431DC0) {
        gnu_used static volatile auto tempA = this;
        gnu_used static volatile auto tempB = file_index;
        gnu_used static volatile auto tempC = filename;
        gnu_used static volatile auto tempD = starting_sprite_index;
        assume_all_registers_volatile();
        return (ZunResult)file_index;
    }

    // 0x434AF0
    dllexport inline void thiscall __draw_text_to_texture(
        uint32_t texture_index,
        int32_t x_pos, int32_t y_pos, int32_t width, int32_t height,
        uint32_t font_height, uint32_t arg_10D,
        D3DCOLOR colorA, D3DCOLOR colorB, const char *restrict text
    ) asm("{[codecave:ANM_MANAGER_draw_text_to_texture]}")
    {
        __sub_41F050(
            x_pos, y_pos, width, height,
            (font_height > 0 ? font_height : 15), (arg_10D > 0 ? arg_10D : 15),
            colorA, colorB,
            text, this->textures[texture_index]
        );
    }

    // 0x434B60
    dllexport void __draw_text_to_vm_left(AnmVM *restrict vm, D3DCOLOR colorA, D3DCOLOR colorB, const char *restrict format, ...) asm("{[codecave:ANM_MANAGER_draw_text_to_vm_left]}") {
        int32_t font_width = vm->font_width;
        char buffer[64];
        va_list va;
        va_start(va, format);
        vsprintf_helper(buffer, format, va);
        va_end(va);
        const AnmLoadedSprite *restrict sprite = vm->sprite;
        this->__draw_text_to_texture(
            sprite->source_file_index,
            sprite->start_pixel_inclusive.x, sprite->start_pixel_inclusive.y,
            sprite->texture_width, sprite->texture_height,
            font_width, vm->font_height,
            colorA, colorB,
            buffer
        );
        vm->visible = true;
    }

    // 0x434C40
    dllexport void __draw_text_to_vm_right(AnmVM *restrict vm, D3DCOLOR colorA, D3DCOLOR colorB, const char *restrict format, ...) asm("{[codecave:ANM_MANAGER_draw_text_to_vm_right]}") {
        int32_t font_width = vm->font_width > 0 ? vm->font_width : 15;
        const AnmLoadedSprite *restrict sprite = vm->sprite;
        this->__draw_text_to_texture(
            sprite->source_file_index,
            sprite->start_pixel_inclusive.x, sprite->start_pixel_inclusive.y,
            sprite->texture_width, sprite->texture_height,
            font_width, vm->font_height,
            colorA, colorB,
            SingleSpaceString
        );
        va_list va;
        va_start(va, format);
        char buffer[64]; // No! Bad ZUN! *bops with newspaper*
        int32_t buffer_written = vsprintf_helper(buffer, format, va);
        va_end(va);
        this->__draw_text_to_texture(
            sprite->source_file_index,
            sprite->start_pixel_inclusive.x + sprite->texture_width - (buffer_written * (font_width + 1) / 2.0f),
            //sprite->start_pixel_inclusive.x + sprite->texture_width - ((float)buffer_written * (float)(font_width + 1) / 2.0f),
            sprite->start_pixel_inclusive.y,
            sprite->texture_width, sprite->texture_height,
            font_width, vm->font_height,
            colorA, colorB,
            buffer
        );
        vm->visible = true;

        // Original code:
        
        //int32_t temp = vm->__byte_10C > 0 ? vm->__byte_10C : 15;
        //va_list va;
        //va_start(va, format);
        //char buffer[64]; // No! Bad ZUN! *bops with newspaper*
        //vsprintf_indirect(buffer, format, va);
        //const AnmLoadedSprite *restrict sprite = vm->sprite;
        //this->__draw_text_to_texture(
        //    sprite->source_file_index,
        //    sprite->start_pixel_inclusive.x,
        //    sprite->start_pixel_inclusive.y,
        //    sprite->texture_width,
        //    sprite->texture_height,
        //    temp,
        //    vm->__byte_10D,
        //    colorA,
        //    colorB,
        //    /* */ (char*)0x46AC10
        //);
        //char* temp_str = buffer;
        //while (*temp_str) ++temp_str;
        //size_t buffer_written = temp_str - buffer;
        //this->__draw_text_to_texture(
        //    sprite->source_file_index,
        //    sprite->start_pixel_inclusive.x + sprite->texture_width - ((float)buffer_written * (float)(temp + 1) / 2.0f),
        //    sprite->start_pixel_inclusive.y,
        //    sprite->texture_width,
        //    sprite->texture_height,
        //    temp,
        //    vm->__byte_10D,
        //    colorA,
        //    colorB,
        //    buffer
        //);
        //vm->visible = true;
    }

    // 0x434E20
    dllexport void __draw_text_to_vm_center(AnmVM *restrict vm, D3DCOLOR colorA, D3DCOLOR colorB, const char *restrict format, ...) asm("{[codecave:ANM_MANAGER_draw_text_to_vm_center]}") {
        int32_t font_width = vm->font_width > 0 ? vm->font_width : 15;
        const AnmLoadedSprite *restrict sprite = vm->sprite;
        this->__draw_text_to_texture(
            sprite->source_file_index,
            sprite->start_pixel_inclusive.x, sprite->start_pixel_inclusive.y,
            sprite->texture_width, sprite->texture_height,
            font_width, vm->font_height,
            colorA, colorB,
            SingleSpaceString
        );
        va_list va;
        va_start(va, format);
        char buffer[64];
        int32_t buffer_written = vsprintf_helper(buffer, format, va);
        va_end(va);
        this->__draw_text_to_texture(
            sprite->source_file_index,
            sprite->start_pixel_inclusive.x + (sprite->texture_width / 2.0f) - (buffer_written * (font_width + 1) / 4.0f),
            sprite->start_pixel_inclusive.y,
            sprite->texture_width, sprite->texture_height,
            font_width, vm->font_height,
            colorA, colorB,
            buffer
        );
        vm->visible = true;
    }

    // 0x433960
    dllexport int32_t thiscall run_anm(AnmVM *restrict vm) asm("{[codecave:ANM_MANAGER_run_anm]}") {
        AnmInstruction *restrict current_instruction = vm->current_instr;
        if (expect(!current_instruction, false)) {
            return 1;
        }
        uint8_t op_index;
        int32_t current_time;
        int32_t instruction_time;
        if (int32_t pending_interrupt = vm->pending_interrupt_label; 
            expect(pending_interrupt, false)
        ) {
            vm->pending_interrupt_label = 0;
            vm->stop_enable = false;
            current_instruction = vm->beginning_of_script;
            for (
                AnmInstruction *restrict default_interrupt = NULL;
                ;
                IndexInstr(current_instruction->argsize + sizeof(AnmInstruction))
            ) {
                switch (current_instruction->opcode) {
                    case 22: { // interrupt_label
                        if (IntArg(0) == pending_interrupt) {
                            goto StopInterruptSearch;
                        }
                        else if (IntArg(0) == -1) {
                            default_interrupt = current_instruction;
                        }
                        break;
                    }
                    case 0: // delete
                    case 15: // static
                        if (expect_chance(default_interrupt == NULL, true, 0.0)) {
                            vm->script_time.decrement(1);
                            asm_jump(BreakInsLoop3);
                            unreachable;
                            //goto BreakInsLoop3;
                        } else {
                            current_instruction = default_interrupt;
                        }
                        goto StopInterruptSearch;
                }
            }
        StopInterruptSearch:
            IndexInstr(current_instruction->argsize + sizeof(AnmInstruction));
            vm->visible = true;
            vm->script_time = (Timer){ -999, 0.0f, instruction_time = current_time = current_instruction->time };
            goto NextInstr;
            //goto FirstInstr;
        }
        current_time = vm->script_time.current;
        instruction_time = current_instruction->time;
        assume(instruction_time <= current_time);
        goto FirstInstr;
        for (
            ;;
        ) {
            gnu_used static constexpr void* im_sorry = &&InstrLoop;
        InstrLoop:
            IndexInstr(current_instruction->argsize + sizeof(AnmInstruction));
            instruction_time = current_instruction->time;
        FirstInstr:
            if (!expect(instruction_time <= current_time, true)) break;
            //if (!expect(current_instruction->time <= current_time, true)) break;
        NextInstr:
            //switch (expect_chance(op_index = current_instruction->opcode, 6, 0.99)) {
            //switch (unpredictable(op_index = current_instruction->opcode)) {
            switch (expect_chance(op_index = current_instruction->opcode, 5, 0.0)) {
                default:
                    break;
                case 0: // delete
                    vm->visible = false;
                    [[fallthrough]];
                case 15: // static
                    vm->current_instr = NULL;
                    return 1;
                case 1: // sprite
                    vm->visible = true;
                    this->set_vm_sprite(vm, this->sprite_indices[vm->script_number] + IntArg(0));
                    vm->time_of_last_sprite_set = current_time;
                    align_statement(16);
                    break;
                case 16: // sprite_rand
                    vm->visible = true;
                    this->set_vm_sprite(vm, RNG.rand_ushort_mod(ShortArg(2)) + this->sprite_indices[vm->script_number] + IntArg(0));
                    vm->time_of_last_sprite_set = current_time;
                    break;
                case 2: // scale
                {
                    vec<float, 2> temp = Float2ArgVec(0);
                    vec<float, 2>& scale_vec = *(vec<float, 2>*)&vm->scale;
                    scale_vec[1] = temp[0];
                    scale_vec[0] = temp[1];
                }
                    //vm->scale_x = FloatArg(0);
                    //vm->scale_y = FloatArg(1);
                    break;
                case 3: // alpha
                    vm->alpha = ByteArg(0);
                    break;
                case 4: // color
                    // color_non_aplha is an array of 3 uint8_t
                    // in a union with the standard D3DCOLOR.
                    // Memcpy is used to let the compiler move
                    // the 3 bytes however it wants without me
                    // needing to give a crap.
                    memcpy(vm->color_non_aplha, current_instruction->args, sizeof(vm->color_non_aplha));
                    break;
                case 5: // jump
                    SetInstr((intptr_t)vm->beginning_of_script + IntArg(0));
                    current_time = current_instruction->time;
                    goto NextInstr;
                case 7: // flip_x
                    vm->flip_x ^= 1;
                    vm->scale_x = -vm->scale_x;
                    break;
                case 25: // position_mode
                    vm->position_mode = OneBitArg(0);
                    break;
                case 8: // flip_y
                    vm->flip_y ^= 1;
                    vm->scale_y = -vm->scale_y;
                    break;
                case 9: // rotate
                    vm->rotation = Float3Arg(0);
                    break;
                case 10: // angular_velocity
                    vm->angular_velocity = Float3Arg(0);
                    break;
                case 11: // scale_growth
                {
                    vec<float, 2> temp = Float2ArgVec(0);
                    vec<float, 2>& scale_interp_final_vec = *(vec<float, 2>*)&vm->scale_interp_final;
                    scale_interp_final_vec[1] = temp[0];
                    scale_interp_final_vec[0] = temp[1];
                }
                    //vm->scale_interp_final_x = FloatArg(0);
                    //vm->scale_interp_final_y = FloatArg(1);
                    vm->scale_interp_end_time = 0;
                    break;
                case 30: // scale_time_linear
                {
                    vec<float, 2> temp = Float2ArgVec(0);
                    vec<float, 2>& scale_interp_final_vec = *(vec<float, 2>*) & vm->scale_interp_final;
                    scale_interp_final_vec[1] = temp[0];
                    scale_interp_final_vec[0] = temp[1];
                }
                    //vm->scale_interp_final_x = FloatArg(0);
                    //vm->scale_interp_final_y = FloatArg(1);
                    vm->scale_interp_end_time = ShortArg(4);
                    //vm->scale_interp_timer = static_default_timer<0>;
                    vm->scale_interp_timer.set_default();
                    vm->scale_interp_initial = vm->scale;
                    break;
                case 12: { // alpha_time_linear
                    D3DCOLOR color = vm->color;
                    vm->alpha_interp_initial = color;
                    vm->alpha_interp_final = (color & 0x00FFFFFF) | (ByteArg(0) << 24);
                    vm->alpha_interp_end_time = ShortArg(2);
                    //vm->alpha_interp_timer = static_default_timer<0>;
                    vm->alpha_interp_timer.set_default();
                    break;
                }
                case 13: // blend_additive
                    vm->additive_blending = true;
                    break;
                case 14: // blend_default
                    vm->additive_blending = false;
                    break;
                case 17: // position
                    (!vm->position_mode ? vm->position : vm->__position_2) = Float3Arg(0);
                    break;
                case 18: // position_time_linear
                case 19: // position_time_quadratic
                case 20: // position_time_quartic
                    vm->move_mode = op_index - 18;
                    vm->position_interp_initial = (!vm->position_mode ? vm->position : vm->__position_2);
                    vm->position_interp_final = Float3Arg(0);
                    vm->position_interp_end_time = IntArg(3);
                    //vm->position_interp_timer = static_default_timer<0>;
                    vm->position_interp_timer.set_default();
                    break;
                case 24: // stop_hide
                    vm->visible = false;
                    [[fallthrough]];
                case 21: { // stop
                    //if (!vm->pending_interrupt_label) {
                        vm->stop_enable = true;
                        vm->script_time.current = current_time;
                        vm->script_time.decrement(1);
                        goto BreakInsLoop2;
                    //}
            //AnmInterrupt:
            //        int32_t pending_interrupt = vm->pending_interrupt_label;
            //        AnmInstruction *restrict default_interrupt = NULL;
            //        SetInstr(vm->beginning_of_script);
            //        for (
            //            ;
            //            ;
            //            IndexInstr(current_instruction->argsize + sizeof(AnmInstruction))
            //        ) {
            //            switch (current_instruction->opcode) {
            //                case 22: // interrupt_label
            //                    if (int32_t found_interrupt = IntArg(0);
            //                        found_interrupt == pending_interrupt
            //                    ) {
            //                        goto StopInterruptSearch;
            //                    } else if (found_interrupt == -1) {
            //                        default_interrupt = current_instruction;
            //                    }
            //                    break;
            //                case 0: // delete
            //                case 15: // static
            //                    if (expect(default_interrupt == NULL, true)) {
            //                        vm->pending_interrupt_label = 0;
            //                        vm->stop_enable = false;
            //                        vm->script_time.current = current_time;
            //                        vm->script_time.decrement(1);
            //                        goto BreakInsLoop;
            //                    } else {
            //                        SetInstr(default_interrupt);
            //                    }
            //                    goto StopInterruptSearch;
            //            }
            //        }
            //    StopInterruptSearch:
            //        vm->pending_interrupt_label = 0;
            //        vm->stop_enable = false;
            //        IndexInstr(current_instruction->argsize + sizeof(AnmInstruction));
            //        vm->script_time = (Timer){ -999, 0.0f, current_time = current_instruction->time };
            //        vm->visible = true;
            //        goto NextInstr;
                }
                case 29: // visible
                    vm->visible = OneBitArg(0);
                    break;
                case 23: // anchor_top_left
                    vm->anchor_mode = 3;
                    break;
                case 26: // 
                    vm->__auto_rotate = ShortArg(0);
                    break;
                case 27: { // scroll_now_x
                    float temp = vm->uv_scroll_pos.x + FloatArg(0);
                    if (temp >= 1.0f) temp -= 1.0f;
                    if (temp < 0.0f) temp += 1.0f;
                    vm->uv_scroll_pos.x = temp;
                    break;
                }
                case 28: { // scroll_now_y
                    float temp = vm->uv_scroll_pos.y + FloatArg(0);
                    if (temp >= 1.0f) temp -= 1.0f;
                    if (temp < 0.0f) temp += 1.0f;
                    vm->uv_scroll_pos.y = temp;
                    break;
                }
                case 31: // z_write_disable
                    vm->z_write_disable = OneBitArg(0);
                    break;
            }
        }
    BreakInsLoop:
        vm->script_time.current = current_time;
    BreakInsLoop2:
        vm->current_instr = current_instruction;
    BreakInsLoop3:
        /*{
            vec<float, 4> rotational_velocity = *(unaligned vec<float, 4>*)&vm->angle_vel;
            rotational_velocity *= { RECIP_TWO_PI_f, RECIP_TWO_PI_f, RECIP_TWO_PI_f, 0.0f };
            rotational_velocity *= SUPERVISOR.game_speed;
            unaligned vec<float, 4>& rotation = *(unaligned vec<float, 4>*)&vm->rotation;
            rotational_velocity += rotation;
            rotational_velocity = _mm_round_ps(rotational_velocity, 0b1000);
            rotation[0] = rotational_velocity[0];
            rotation[1] = rotational_velocity[1];
            rotation[2] = rotational_velocity[2];
        }*/
        // Yes, this is how I've been implementing reduce_angle.
        // I didn't feel like trying to use GetProcAddress to find
        // remainderf at runtime and then do manual fixups on all
        // my codecaves. Besides, the precision of a single FPREM1
        // still beats the original function by a mile. :P
        // Real shame that there's no SSE version of FPREM1.
        //__asm__(
        //    "FLDZ \n" // ST(2)
        //    "FLDS %[TWO_PI] \n" // ST(1)
        //    "FLDS %[X_ROT_VEL] \n" // ST(0)
        //    "FUCOMI %%st(2) \n"
        //    "JZ skip_x \n"
        //    "FMULS %[GAME_SPEED] \n"
        //    "FADDS %[X_ROT] \n"
        //    "FPREM1 \n"
        //    "FSTS %[X_ROT] \n"
        //"skip_x: \n"
        //    "FFREEP %%st(0) \n" // FFREEP ST(0) is cheaper to pop the stack that FSTP ST(0)
        //    "FLDS %[Y_ROT_VEL] \n"
        //    "FUCOMI %%st(2) \n"
        //    "JZ skip_y \n"
        //    "FMULS %[GAME_SPEED] \n"
        //    "FADDS %[Y_ROT] \n"
        //    "FPREM1 \n"
        //    "FSTS %[Y_ROT] \n"
        //"skip_y: \n"
        //    "FFREEP %%st(0) \n"
        //    "FLDS %[Z_ROT_VEL] \n"
        //    "FUCOMI %%st(2) \n"
        //    "JZ skip_z \n"
        //    "FMULS %[GAME_SPEED] \n"
        //    "FADDS %[Z_ROT] \n"
        //    "FPREM1 \n"
        //    "FSTS %[Z_ROT] \n"
        //"skip_z: \n"
        //    "FFREEP %%st(0) \n"
        //    "FFREEP %%st(1) \n"
        //    "FINCSTP \n"
        //    : [Z_ROT] "+m"(vm->rotation.z), [Y_ROT]"+m"(vm->rotation.y), [X_ROT]"+m"(vm->rotation.x)
        //    : [TWO_PI]"m"(TWO_PI_f), [GAME_SPEED]"m"(SUPERVISOR.game_speed),
        //    [Z_ROT_VEL]"m"(vm->angle_vel.z), [Y_ROT_VEL]"m"(vm->angle_vel.y), [X_ROT_VEL]"m"(vm->angle_vel.x)
        //);
        {
            vec<double, 4> angles_temp = convertvec(*(unaligned vec<float, 4>*)&vm->angular_velocity, vec<double, 4>);
            angles_temp *= ONE_OVER_TWO_PI_d;
            vec<double, 4> angles_temp2 = _mm256_round_pd(angles_temp, 0x8);
            angles_temp -= angles_temp2;
            angles_temp *= TWO_PI_d;
            vec<float, 4> angles_temp3 = convertvec(angles_temp, vec<float, 4>);
            unaligned vec<float, 4>& rotation_ref = *(unaligned vec<float, 4>*)&vm->rotation;
            angles_temp3 *= SUPERVISOR.game_speed;
            angles_temp3 += rotation_ref;
            rotation_ref[0] = angles_temp3[0];
            rotation_ref[1] = angles_temp3[1];
            rotation_ref[2] = angles_temp3[2];
        }
        if (int32_t scale_end_time = vm->scale_interp_end_time;
            scale_end_time > 0
        ) {
            vm->scale_interp_timer.tick();
            //vm->scale_interp_timer.previous = vm->scale_interp_timer.current;
            //SUPERVISOR.tick_timer(&vm->scale_interp_timer.current, &vm->scale_interp_timer.subframe);
            vec<float, 2> scale;
            if (vm->scale_interp_timer.current >= scale_end_time) {
                //vm->scale = vm->scale_interp_final;
                scale = *(vec<float, 2>*)&vm->scale_interp_final;
                vm->scale_interp_end_time = 0;
                vm->scale_interp_final_y = 0.0f;
                vm->scale_interp_final_x = 0.0f;
            } else {
                float interp = ((float)vm->scale_interp_timer.current + vm->scale_interp_timer.subframe) / (float)scale_end_time;
                //vec<float, 2> interp_mul = (vec<float, 2>){ interp, interp };
                vec<float, 2> scale_initial = *(vec<float, 2>*)&vm->scale_interp_initial;
                vec<float, 2> scale_final = *(vec<float, 2>*)&vm->scale_interp_final;
                scale = scale_initial + (scale_final - scale_initial) * interp;
                //vm->scale.as_vec = scale_initial + (scale_final - scale_initial) * interp;
                //*(vec<float, 2, 1>*)&vm->scale = scale_initial + (scale_final - scale_initial) * interp_mul;

                /*float scale_x = vm->scale_interp_initial_x + ((vm->scale_interp_final_x - vm->scale_interp_initial_x) * interp);
                float scale_y = vm->scale_interp_initial_y + ((vm->scale_interp_final_y - vm->scale_interp_initial_y) * interp);
                vm->scale_x = (!vm->flip_x ? scale_x : -scale_x);
                vm->scale_y = (!vm->flip_y ? scale_y : -scale_y);*/
            }
            vec<float, 2>& scale_vec = *(vec<float, 2>*)&vm->scale;
            scale_vec[0] = (!vm->flip_y ? scale[0] : -scale[0]);
            scale_vec[1] = (!vm->flip_x ? scale[1] : -scale[1]);
        } else {
            vec<float, 2>& scale_vec = *(vec<float, 2>*)&vm->scale;
            //vm->scale_y += vm->scale_interp_final_y * SUPERVISOR.game_speed;
            //vm->scale_x += vm->scale_interp_final_x * SUPERVISOR.game_speed;
            scale_vec += *(vec<float, 2>*)&vm->scale_interp_final * SUPERVISOR.game_speed;
        }
        if (int32_t alpha_end_time = vm->alpha_interp_end_time;
            alpha_end_time > 0
        ) {
            vm->alpha_interp_timer.tick();
            //vm->alpha_interp_timer.previous = vm->alpha_interp_timer.current;
            //SUPERVISOR.tick_timer(&vm->alpha_interp_timer.current, &vm->alpha_interp_timer.subframe);
            if (vm->alpha_interp_timer.current >= alpha_end_time) {
                vm->alpha_interp_end_time = 0;
            }
            float interp = ((float)vm->alpha_interp_timer.current + vm->alpha_interp_timer.subframe) / (float)alpha_end_time;
            interp = __min(interp, 1.0f);
            
            //vec<float, 4> interp_mul = (vec<float, 4>){ interp, interp, interp, interp };
            //vec<float, 4> interp_initial = convertvec(shufflevec((vec<uint8_t, 4>)vm->alpha_interp_initial, (vec<uint8_t, 4>)vm->alpha_interp_initial, 0, 1, 2, 3), vec<float, 4>);
            //vec<float, 4> interp_final = convertvec(shufflevec((vec<uint8_t, 4>)vm->alpha_interp_final, (vec<uint8_t, 4>)vm->alpha_interp_final, 0, 1, 2, 3), vec<float, 4>);
            //interp_initial += (interp_final - interp_initial) * interp_mul;
            //vm->color = bitcast(uint32_t, convertvec(interp_initial, vec<uint8_t, 4>));

            vec<float, 4> interp_initial = convertvec(vm->alpha_interp_initial_vec, vec<float, 4>);
            vec<float, 4> interp_final = convertvec(vm->alpha_interp_final_vec, vec<float, 4>);
            vm->color_vec = convertvec(interp_initial + (interp_final - interp_initial) * interp, vec<uint8_t, 4>);

            //for (size_t ii = 0; ii < 4; ++ii) {
                //float temp_val = (float)vm->alpha_interp_initial_all[ii] + ((float)vm->alpha_interp_final_all[ii] - (float)vm->alpha_interp_initial_all[ii]) * interp;
                //vm->color_all[ii] = __min(__max(temp_val, 0.0f), 255.0f);
                //vm->color_all[ii] = (uint8_t)((vm->alpha_interp_final_all[ii] - vm->alpha_interp_initial_all[ii]) * interp) + vm->alpha_interp_final_all[ii];
            //}
            /*if (vm->alpha_interp_timer.current >= alpha_end_time) {
                vm->alpha_interp_end_time = 0;
            }*/
        }
        if (int32_t position_end_time = vm->position_interp_end_time) {
            float interp = ((float)vm->position_interp_timer.current + vm->position_interp_timer.subframe) / (float)position_end_time;
            interp = __min(interp, 1.0f);
            //switch (vm->move_mode) {
            switch (vm->flags_81 & 0xC) {
                case 0x4: //case 1:
                    interp = 1.0f - interp;
                    interp *= interp;
                    interp = 1.0f - interp;
                    break;
                case 0x8: //case 2:
                    interp = 1.0f - interp;
                    interp *= interp;
                    interp *= interp;
                    interp = 1.0f - interp;
                    break;
            }

            vec<float, 4> interp_mul = (vec<float, 4>){ interp, interp, interp, interp };
            unaligned vec<float, 4>& position = *(unaligned vec<float, 4>*)(!vm->position_mode ? &vm->position : &vm->__position_2);
            vec<float, 4> position_interp_final = *(unaligned vec<float, 4>*)&vm->position_interp_final;
            vec<float, 4> position_interp_initial = *(unaligned vec<float, 4>*)&vm->position_interp_initial;
            vec<float, 4> temp_position = (interp_mul * position_interp_final) + ((vec<float, 4>) { 1.0f, 1.0f, 1.0f, 1.0f } - interp_mul) * position_interp_initial;
            position[0] = temp_position[0];
            position[1] = temp_position[1];
            position[2] = temp_position[2];

            //Float3 *restrict position = (!vm->position_mode ? &vm->position : &vm->__position_2);
            //position->x = (temp * vm->position_interp_final.x) + (1.0f - interp) * vm->position_interp_initial.x;
            //position->y = (temp * vm->position_interp_final.y) + (1.0f - interp) * vm->position_interp_initial.y;
            //position->z = (temp * vm->position_interp_final.z) + (1.0f - interp) * vm->position_interp_initial.z;
            if (vm->position_interp_timer.current >= position_end_time) {
                vm->position_interp_end_time = 0;
            }
            vm->position_interp_timer.tick();
            //vm->position_interp_timer.previous = vm->position_interp_timer.current;
            //SUPERVISOR.tick_timer(&vm->position_interp_timer.current, &vm->position_interp_timer.subframe);
        }
        //vm->script_time.current = current_time;
        vm->script_time.tick();
        //vm->script_time.previous = vm->script_time.current;
        //SUPERVISOR.tick_timer(&vm->script_time.current, &vm->script_time.subframe);
        return 0;
    }

    // 0x4324D0
    dllexport void thiscall set_render_state_for_vm(AnmVM* VM) asm_symbol_rel(codecave:ANM_MANAGER_set_render_state_for_vm) {
        if (this->current_blend_mode != VM->additive_blending) {
            this->current_blend_mode = VM->additive_blending;
            if (!this->current_blend_mode) {
                SUPERVISOR.d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            } else {
                SUPERVISOR.d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            }
        }
        if (
            !SUPERVISOR.config.__unknown_flag_A &&
            !SUPERVISOR.config.__unknown_flag_B &&
            this->current_colorop != VM->__unknown_flag_D
        ) {
            this->current_colorop = VM->__unknown_flag_D;
            if (!this->current_colorop) {
                SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            } else {
                SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
            }
        }
        if (!SUPERVISOR.config.__unknown_flag_B) {
            if (this->current_texture_factor != VM->color) {
                this->current_texture_factor = VM->color;
                SUPERVISOR.d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, this->current_texture_factor);
            }
        } else {
            SPRITE_VERTEX_BUFFER_A[0].diffuse = VM->color;
            SPRITE_VERTEX_BUFFER_A[1].diffuse = VM->color;
            SPRITE_VERTEX_BUFFER_A[2].diffuse = VM->color;
            SPRITE_VERTEX_BUFFER_A[3].diffuse = VM->color;
            SPRITE_VERTEX_BUFFER_C[0].diffuse = VM->color;
            SPRITE_VERTEX_BUFFER_C[1].diffuse = VM->color;
            SPRITE_VERTEX_BUFFER_C[2].diffuse = VM->color;
            SPRITE_VERTEX_BUFFER_C[3].diffuse = VM->color;
        }
        if (
            !SUPERVISOR.config.__unknown_flag_D &&
            this->current_zwrite_disable != VM->z_write_disable
        ) {
            this->current_zwrite_disable = VM->z_write_disable;
            if (!this->current_zwrite_disable) {
                SUPERVISOR.d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
            } else {
                SUPERVISOR.d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
            }
        }
    }
};

dllexport void thiscall AnmManager::set_vm_script(AnmVM *restrict vm, AnmInstruction *restrict script_start) {
    vm->flip_x = false;
    vm->flip_y = false;
    vm->position_mode = 0;
    vm->initialize();
    vm->beginning_of_script = script_start;
    vm->current_instr = script_start;
    //vm->script_time = static_default_timer<0>;
    vm->script_time.set_default();
    vm->visible = false;
    if (script_start) {
        this->run_anm(vm);
    }
}

extern "C" {
    // 0x6D4588
    extern AnmManager *restrict ANM_MANAGER_PTR asm("_ANM_MANAGER_PTR");
}

void AnmVM::set_script(int32_t index) {
    this->script_number = index;
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    anm_manager->set_vm_script(this, anm_manager->scripts[index]);
}

typedef struct AsciiManagerString AsciiManagerString;
struct AsciiManagerString {
    char text[0x40];  // 0x0
    Float3 position;  // 0x40
    D3DCOLOR color;  // 0x4c
    Float2 scale;  // 0x50
    int __dword_58;  // 0x58
    int __dword_5C;  // 0x5C
};  // 0x60
ValidateFieldOffset(0x0, AsciiManagerString, text);
ValidateFieldOffset(0x40, AsciiManagerString, position);
ValidateFieldOffset(0x4C, AsciiManagerString, color);
ValidateFieldOffset(0x50, AsciiManagerString, scale);
ValidateFieldOffset(0x58, AsciiManagerString, __dword_58);
ValidateFieldOffset(0x5C, AsciiManagerString, __dword_5C);

typedef struct AsciiManagerPopup AsciiManagerPopup;
struct AsciiManagerPopup {
    char digits[0x8];  // 0x0
    Float3 position;  // 0x8
    D3DCOLOR color;  // 0x14
    Timer timer;  // 0x18
    uint8_t in_use;  // 0x24
    uint8_t characters;  // 0x25
    unknown_fields(0x2);  // 0x26
};  // 0x28
ValidateFieldOffset(0x0, AsciiManagerPopup, digits);
ValidateFieldOffset(0x8, AsciiManagerPopup, position);
ValidateFieldOffset(0x14, AsciiManagerPopup, color);
ValidateFieldOffset(0x18, AsciiManagerPopup, timer);
ValidateFieldOffset(0x24, AsciiManagerPopup, in_use);
ValidateFieldOffset(0x25, AsciiManagerPopup, characters);

typedef struct AsciiManagerBigChild AsciiManagerBigChild;
struct AsciiManagerBigChild {
    int32_t __index_0;  // 0x0
    int __dword_4;  // 0x4
    AnmVM __vms_8[0x6];  // 0x8
    AnmVM __vm_668;  // 0x668 // capture.anm VM
};  // 0x778
ValidateFieldOffset(0x8, AsciiManagerBigChild, __vms_8);
ValidateFieldOffset(0x668, AsciiManagerBigChild, __vm_668);

typedef struct AsciiManager AsciiManager;
struct AsciiManager {
    AnmVM __vm_0;  // 0x0
    AnmVM __vm_110;  // 0x110
    AsciiManagerString strings[256];  // 0x220
    int32_t num_strings;  // 0x6220
    D3DCOLOR color;  // 0x6224
    Float2 scale;  // 0x6228
    int __dword_6230;  // 0x6230
    int __dword_6234;  // 0x6234
    int32_t next_popupA_index;  // 0x6238
    int32_t next_popupB_index;  // 0x623c
    unknown_fields(0x4);  // 0x6240
    AsciiManagerBigChild __child_6244;  // 0x6244
    AsciiManagerBigChild __child_69BC;  // 0x69bc
    // Technically the original game was written with an array
    // of 515 popups and ZUN just looped the first 512 and the
    // last 3 separately. This is evident from the +512 found at
    // address 0x401A8B.
    // Screw that though, I'm doing it the sane way.
    AsciiManagerPopup popupsA_7134[512];  // 0x7134
    AsciiManagerPopup popupsB_C134[3];  // 0xc134

    // 0x401940
    dllexport void thiscall make_popup_A(Float3 *restrict position, int32_t number, D3DCOLOR color) asm("{[codecave:ASCII_MANAGER_make_popup_A]}") {
        int32_t popup_index = this->next_popupA_index;
        AsciiManagerPopup *restrict new_popup = &this->popupsA_7134[popup_index];
        this->next_popupA_index = ++popup_index >= countof(this->popupsA_7134) ? 0 : popup_index;
        new_popup->in_use = true;
        new_popup->color = color;
        //new_popup->timer = static_default_timer<0>;
        new_popup->timer.set_default();
        new_popup->position = *position;
        if (expect(number > 0, true)) {
            size_t index = 0;
            while (number > 0) {
                new_popup->digits[index++] = number % 10;
                number /= 10;
            }
            new_popup->characters = index;
        } else {
            new_popup->digits[0] = number < 0 ? 10 : 0;
            new_popup->characters = 1;
        }
        

        //if (this->next_popupA_index >= countof(this->popupsA_7134)) {
        //    this->next_popupA_index = 0;
        //}
        //AsciiManagerPopup *restrict new_popup = &this->popupsA_7134[this->next_popupA_index];
        //new_popup->in_use = true;
        //size_t index = 0;
        //if (number >= 0) {
        //    while (number != 0) {
        //        new_popup->digits[index++] = number % 10;
        //        number /= 10;
        //    }
        //} else {
        //    new_popup->digits[index++] = 10;
        //}
        //if (index == 0) {
        //    new_popup->digits[index++] = 0;
        //}
        //new_popup->characters = index;
        //new_popup->color = color;
        ////new_popup->timer = static_default_timer<0>;
        //new_popup->timer.set_default();
        //new_popup->position = *position;
        //++this->next_popupA_index;
    }

    // 0x401A60
    dllexport void thiscall make_popup_B(Float3 *restrict position, int32_t number, D3DCOLOR color) asm("{[codecave:ASCII_MANAGER_make_popup_B]}") {
        int32_t popup_index = this->next_popupB_index;
        AsciiManagerPopup *restrict new_popup = &this->popupsB_C134[popup_index];
        this->next_popupB_index = ++popup_index >= countof(this->popupsB_C134) ? 0 : popup_index;
        new_popup->in_use = true;
        new_popup->color = color;
        //new_popup->timer = static_default_timer<0>;
        new_popup->timer.set_default();
        new_popup->position = *position;
        if (expect(number > 0, true)) {
            size_t index = 0;
            while (number > 0) {
                new_popup->digits[index++] = number % 10;
                number /= 10;
            }
            new_popup->characters = index;
        } else {
            new_popup->digits[0] = number < 0 ? 10 : 0;
            new_popup->characters = 1;
        }
    }
    
    // 0x401530
    dllexport gnu_noinline void thiscall draw(Float3 *restrict position, const char *restrict text) asm("{[codecave:ASCII_MANAGER_draw]}") {
        if (this->num_strings >= countof(this->strings)) {
            return;
        }
        AsciiManagerString *restrict string = &this->strings[this->num_strings++];
        byteloop_strcpy(string->text, text);
        string->position = *position;
#ifdef __AVX__
        unaligned vec<float, 8>& output = *(unaligned vec<float, 8>*)&string->color;
        unaligned vec<float, 8> temp;
        auto temp2 = shufflevec(*(unaligned vec<float, 8>*) & this->color, temp, 0, 1, 2, 4, 3);
        output[0] = temp2[0];
        output[1] = temp2[1];
        output[2] = temp2[2];
        output[3] = (SUPERVISOR.config.flags & 0x101) ? temp2[3] : 0.0f;
        output[4] = temp2[4];
#elif 0//defined(__SSE4_1__)
        string->color = this->color;
        //unaligned vec<float, 4>& output = *(unaligned vec<float, 4>*)&string->scale;
        vec<int32_t, 4> mask1 = { 0x80000000, 0x80000000, 0x80000000, (SUPERVISOR.config.flags & 0x101) ? 0x80000000 : 0 };
        vec<float, 4> temp = *(unaligned vec<float, 4>*)&this->scale;
        temp = shufflevec(temp, temp, 0, 1, 3, 2);
        *(unaligned vec<float, 4>*)&string->scale = _mm_blendv_ps(mask1, temp, mask1);
#else
        string->color = this->color;
        string->scale = this->scale;
        string->__dword_5C = this->__dword_6230;
        string->__dword_58 = (SUPERVISOR.config.flags & 0x101) ? this->__dword_6234 : 0;
        /*if (SUPERVISOR.config.__unknown_flag_A || SUPERVISOR.config.__unknown_flag_B) {
            string->__dword_58 = this->__dword_6234;
        } else {
            string->__dword_58 = 0;
        }*/
#endif
    }

    dllexport void drawf(Float3 *restrict position, const char *restrict format, ...)  asm("{[codecave:ASCII_MANAGER_drawf]}") {
    //void AsciiManager_drawf(AsciiManager *restrict ascii_manager, Float3 *restrict position, const char *restrict format, ...) {
        char buffer[512];
        va_list va;
        va_start(va, format);
        vsprintf_helper(buffer, format, va);
        this->draw(position, buffer);
        va_end(va);
    }

    dllexport auto drawf_thcrap(Float3 *restrict position, const char *restrict format, va_list va, ...)  asm("{[codecave:ASCII_MANAGER_drawf_thcrap]}") {
        return ascii_vpatchf((ascii_put_func_t*)this, position, format, va);
    }

};  // 0xc1ac
ValidateFieldOffset(0x0, AsciiManager, __vm_0);
ValidateFieldOffset(0x110, AsciiManager, __vm_110);
ValidateFieldOffset(0x220, AsciiManager, strings);
ValidateFieldOffset(0x6220, AsciiManager, num_strings);
ValidateFieldOffset(0x6224, AsciiManager, color);
ValidateFieldOffset(0x6228, AsciiManager, scale);
ValidateFieldOffset(0x6230, AsciiManager, __dword_6230);
ValidateFieldOffset(0x6234, AsciiManager, __dword_6234);
ValidateFieldOffset(0x6238, AsciiManager, next_popupA_index);
ValidateFieldOffset(0x623C, AsciiManager, next_popupB_index);
ValidateFieldOffset(0x6244, AsciiManager, __child_6244);
ValidateFieldOffset(0x69BC, AsciiManager, __child_69BC);
ValidateFieldOffset(0x7134, AsciiManager, popupsA_7134);
ValidateFieldOffset(0xC134, AsciiManager, popupsB_C134);

dllexport auto cdecl draw_stub(AsciiManager* ascii_manager, Float3 *restrict position, const char *restrict text) {
    return ascii_manager->draw(position, text);
}

extern "C" {
    extern AsciiManager ASCII_MANAGER asm("_ASCII_MANAGER");
}

typedef struct Effect Effect;

typedef int(*cdecl effect_func)(Effect* restrict effect);

typedef struct EffectData EffectData;
struct EffectData {
    int32_t anm_script_index;
    effect_func on_tick;
};

extern "C" {
    extern EffectData EffectData_Table[20] asm("_EffectData_Table");
}

struct Effect {
    AnmVM primary_vm;  // 0x0
    Float3 __float3_110;  // 0x110
    unknown_fields(0x18); // 0x11C
    Float3 __float3_134;  // 0x134
    Float3 position;  // 0x140
    unknown_fields(0x10);  // 0x14C
    float __float_15C;  // 0x15c
    float __float_160;  // 0x160
    Timer timer;  // 0x164
    unknown_fields(0x4);  // 0x170
    effect_func on_tick_func;  // 0x174
    int8_t in_use_flag;  // 0x178
    int8_t effect_id;  // 0x179
    char __byte_17A;  // 0x17a
    char __byte_17B;  // 0x17b
};  // 0x17c

typedef struct EffectManager EffectManager;
struct EffectManager {
    int32_t next_index;  // 0x0
    int __index_4;  // 0x4
    Effect effects[512];  // 0x8
    Effect dummy_effect_for_failed_spawns;  // 0x2f808

    // 0x40EF50
    dllexport Effect* thiscall spawn_effect(int32_t EffectId, Float3 *restrict position, int32_t count, D3DCOLOR color) asm("{[codecave:EFFECT_MANAGER_spawn_effect]}") {
        Effect *restrict new_effect = &this->effects[this->next_index];
        Effect *const starting_effect = new_effect;
        do {
            do {
                if (!new_effect->in_use_flag) goto FoundUsable;
                ++new_effect;
                if (new_effect == array_end_addr(this->effects)) {
                    new_effect = this->effects;
                }
            } while (new_effect != starting_effect);
            break;
        FoundUsable:
            new_effect->in_use_flag = true;
            new_effect->effect_id = EffectId;
            new_effect->__float3_110 = *position;
            int32_t script_index = EffectData_Table[EffectId].anm_script_index;
            new_effect->primary_vm.script_number = script_index;
            ANM_MANAGER_PTR->set_vm_script(&new_effect->primary_vm, ANM_MANAGER_PTR->scripts[script_index]);
            new_effect->primary_vm.color = color;
            new_effect->on_tick_func = EffectData_Table[EffectId].on_tick;
            //new_effect->timer = static_default_timer<0>;
            new_effect->timer.set_default();
            new_effect->__byte_17A = 0;
            new_effect->__byte_17B = 0;
        } while (--count);
        if (Effect *restrict next_effect = new_effect + 1;
            expect(next_effect != array_end_addr(this->effects), true)) {
            this->next_index = next_effect - &this->effects[0];
        } else {
            this->next_index = 0;
        }
        return new_effect;
    }

};  // 0x2f984

extern "C" {
    // 0x487FE0
    extern EffectManager EFFECT_MANAGER asm("_EFFECT_MANAGER");
    // 0x476438
    extern D3DCOLOR(*EffectColorTablePtr)[] asm("_EffectColorTablePtr");
}

// ====================
// Player
// ====================

typedef struct PlayerRect PlayerRect;
struct PlayerRect {
    Float2 position; // 0x0
    Float2 size; // 0x8
};

typedef struct ShottypeData ShottypeData;
struct ShottypeData {
    float __float_0;
    float __float_4;
    float __float_8;
    float __float_C;
    void *restrict __shot_func_A;
    void *restrict __shot_func_B;
};

typedef struct PlayerBullet PlayerBullet;
struct PlayerBullet {
    AnmVM primary_vm;  // 0x0
    Float3 position;  // 0x110
    Float3 size;  // 0x11c
    float __float_128;  // 0x128
    float __float_12C;  // 0x12c
    float __float_130;  // 0x130
    float __float_134;  // 0x134
    float __float_138;  // 0x138
    float __float_13C;  // 0x13c
    Timer __timer_140;  // 0x140
    int16_t __short_14C;  // 0x14c
    int16_t state;  // 0x14e
    int16_t __short_150;  // 0x150
    short __word_152;  // 0x152
    short __word_154;  // 0x154
    probably_padding_bytes(0x2);
};

typedef struct PlayerInnerD PlayerInnerD;
struct PlayerInnerD {
    int __dword_0;
    int __dword_4;
    Timer __timer_8;
    void *restrict __bomb_func_14;
    void *restrict __bomb_func_18;
    unknown_fields(0x44);
    int __dword_array_5C[8];
    unknown_fields(0x40);
    int __dword_array_BC[8];
    unknown_fields(0x40);
    AnmVM __vms_11C[32];
};

typedef struct Player Player;
struct Player {
    AnmVM vm_0;  // 0x0
    AnmVM vms_1[0x3];  // 0x110
    Float3 position;  // 0x440
    unknown_fields(0xC);  // 0x44c
    Float3 __float3_458;  // 0x458
    Float3 __float3_464;  // 0x464
    Float3 __float3_470;  // 0x470
    Float3 __float3_47C;  // 0x47c
    Float3 __float3_488;  // 0x488
    Float3 __float3_494;  // 0x494
    Float3 __float3_array_4A0[2];  // 0x4A0
    Float3 __position_array_4B8[32];  // 0x4B8
    Float3 __size_array_638[32];  // 0x638
    int __dword_array_7B8[32];  // 0x7b8
    int __dword_array_838[32];  // 0x838
    PlayerRect __player_rect_array_8B8[16];  // 0x8b8
    Timer __timer_array_9B8[2];  // 0x9b8
    float __float_9D0;  // 0x9d0
    float __float_9D4;  // 0x9d4
    int __dword_9D8;  // 0x9d8
    int __dword_9DC;  // 0x9dc
    char __byte_9E0;  // 0x9e0
    char __byte_9E1;  // 0x9e1
    char __byte_9E2;  // 0x9e2
    char __byte_9E3;  // 0x9e3
    uint8_t __byte_9E4;  // 0x9e4
    unknown_fields(0x3);  // 0x9e5
    Timer __timer_9E8;  // 0x9e8
    ShottypeData __shottype_data;  // 0x9f4
    int __dword_A0C;  // 0xa0c
    float __float_A10;  // 0xa10
    int __dword_A14;  // 0xa14
    short __word_A18;  // 0xa18
    unknown_fields(0x2);  // 0xa1a
    Float3 position_of_last_enemy_hit;  // 0xa1c
    PlayerBullet bullets[80];  // 0xa28
    Timer __timer_75A8;  // 0x75a8
    Timer __timer_75B4;  // 0x75b4
    void *restrict __shot_func_75C0;  // 0x75c0
    void *restrict __shot_func_75C4;  // 0x75c4
    PlayerInnerD __player_inner_d_75C8;
    UpdateFunc *restrict on_tick;  // 0x98e4
    UpdateFunc *restrict on_draw_1;  // 0x98e8
    UpdateFunc *restrict on_draw_2;  // 0x98ec

    // 0x427770
    dllexport void die(void) asm("{[codecave:PLAYER_die]}");

    // 0x427190
    dllexport gnu_noinline int32_t thiscall check_laser_collision(Float3* arg1, Float3* arg2, Float3* coord, float angle, bool allow_graze) asm_symbol_rel(0x427190) {
        // TODO
        use_var(arg1);
        use_var(arg2);
        use_var(angle);
        use_var(allow_graze);
        return rand();
    }

    // 0x4264B0
    dllexport forceinline int32_t check_bullet_collisions(Float3* restrict position, Float3* restrict size, int32_t *restrict idk) asm("{[codecave:PLAYER_check_bullet_collisions]}");

    //// 0x426C40
    //dllexport int32_t check_collision_2(Float3* restrict position, Float3* restrict size)  asm("{[codecave:PLAYER_check_collision_original]}") {
    //    PlayerRect* restrict player_rect = this->__player_rect_array_8B8; // Local11
    //    float x_min = position->x - size->x / 2.0f; // Local10
    //    float y_min = position->y - size->y / 2.0f; // Local9
    //    float x_max = position->x + size->x / 2.0f; // Local7
    //    float y_max = position->y + size->y / 2.0f; // Local6
    //    for (int i = 0 /*Local4*/ ; i < countof(this->__player_rect_array_8B8); ++i, ++player_rect) {
    //        if (player_rect->size.x == 0.0f) continue;
    //        float rect_x_min = player_rect->position.x - player_rect->size.x / 2.0f; // Local3
    //        float rect_y_min = player_rect->position.y - player_rect->size.y / 2.0f; // Local2
    //        float rect_x_max = player_rect->position.x + player_rect->size.x / 2.0f; // Local14
    //        float rect_y_max = player_rect->position.y + player_rect->size.y / 2.0f; // Local13
    //        if (!(rect_x_min > x_max || rect_x_max < x_min || rect_y_min > y_max || rect_y_max < y_min)) {
    //            return 2;
    //        }
    //    }
    //    if (this->__float3_458.x > x_max || this->__float3_458.y > y_max || this->__float3_464.x < x_min || this->__float3_464.y < y_min) {
    //        return 0;
    //    } else {
    //        if (!this->__byte_9E0) {
    //            return 1;
    //        } else {
    //            this->die();
    //            return 1;
    //        }
    //    }
    //}

    /*dllexport inline int32_t check_collision2(Float3* restrict position, Float3* restrict size) asm("{[codecave:PLAYER_check_collision2]}") {
        vec<float, 4> sign_maskA = { 0.0f, 0.0f, -0.0f, -0.0f };
        vec<float, 4> sign_maskB = { -0.0f, -0.0f, 0.0f, 0.0f };
        vec<float, 4> player_box = {
            position->x,
            position->y,
            position->x,
            position->y
        };
        vec<float, 4> player_size = {
            size->x,
            size->y,
            size->x,
            size->y
        };
        player_size /= 2.0f;
        player_size = _mm_xor_ps(player_size, sign_maskA);
        player_box -= player_size;
        player_box = _mm_xor_ps(player_box, sign_maskA);

        const PlayerRect *restrict player_rect = this->__player_rect_array_8B8;
        for (size_t ii = 0; ii < countof(this->__player_rect_array_8B8); ++ii, ++player_rect) {
            if (!player_rect->size.x) continue;
            vec<float, 4> rect_box = {
                player_rect->position.x,
                player_rect->position.y,
                player_rect->position.x,
                player_rect->position.y
            };
            vec<float, 4> rect_size = {
                player_rect->size.x,
                player_rect->size.y,
                player_rect->size.x,
                player_rect->size.y
            };
            rect_size /= 2.0f;
            rect_size = _mm_xor_ps(rect_size, sign_maskB);
            rect_box -= rect_size;
            rect_box = _mm_xor_ps(rect_box, sign_maskA);
            if (!_mm_movemask_ps(player_box < rect_box)) {
                return 2;
            }
        }
        vec<float, 4> rect_box = bitcast(vec<float, 4>, (vec<double, 2>){ 
            *(double*)&this->__float3_464,
            *(double*)&this->__float3_458
        });
        rect_box = _mm_xor_ps(rect_box, sign_maskA);
        if (!_mm_movemask_ps(player_box >= rect_box)) {
            if (!this->__byte_9E0) {
                this->die();
            }
            return 1;
        }
        return 0;
    }*/

    dllexport inline int32_t check_collision(Float3* restrict position, Float3* restrict size) asm("{[codecave:PLAYER_check_collision]}") {
        BoundingBoxSource source_box;
        source_box.make_from_pos_size(*position, *size);
        for (
            const PlayerRect *restrict player_rect = this->__player_rect_array_8B8;
            player_rect != array_end_addr(this->__player_rect_array_8B8);
            ++player_rect
        ) {
            if (!player_rect->size.x) continue;
            BoundingBoxDest dest_box;
            dest_box.make_from_pos_size(player_rect->position, player_rect->size);
            //if (source_box.test_not_intersect(dest_box)) {
            if (source_box.test_intersect(dest_box)) {
                return 2;
            }
        }
        BoundingBoxDest dest_box;
        dest_box.make_from_min_max(this->__float3_464, this->__float3_458);
        if (source_box.test_intersect(dest_box)) {
            if (!this->__byte_9E0) {
                this->die();
            }
            return 1;
        }
        return 0;
    }

    //// 0x426C40
    //dllexport inline int32_t check_collision(Float3* restrict position, Float3* restrict size) asm("{[codecave:PLAYER_check_collision]}") {
    //    vec<float, 2> temp_size = *(unaligned vec<float, 2>*)size;
    //    temp_size /= 2.0f;
    //    vec<float, 2> temp_pos = *(unaligned vec<float, 2>*)position;
    //    vec<float, 2> min_pos = temp_pos - temp_size;
    //    vec<float, 2> max_pos = temp_pos + temp_size;
    //
    //    const PlayerRect *restrict player_rect = this->__player_rect_array_8B8;
    //    for (size_t ii = 0; ii < countof(this->__player_rect_array_8B8); ++ii, ++player_rect) {
    //        vec<float, 2> temp_rect_size = player_rect->size.as_vec;
    //        if (!temp_rect_size[0]) continue;
    //        temp_rect_size /= 2.0f;
    //        vec<float, 2> temp_rect_pos = player_rect->position.as_vec;
    //        vec<float, 2> rect_min = temp_rect_pos - temp_rect_size;
    //        vec<float, 2> rect_max = temp_rect_pos + temp_rect_size;
    //
    //
    //        if (vec<uint32_t, 2> compare = (rect_min > max_pos) | (rect_max < min_pos);
    //            bitcast(double, compare) == 0.0
    //        ) {
    //            return 2;
    //        }
    //    }
    //    //if (vec<uint32_t, 2> compare = (*(unaligned vec<float, 2>*)&this->__float3_458 <= max_pos) & (*(unaligned vec<float, 2>*)&this->__float3_464 >= min_pos);
    //    vec<uint32_t, 2> compare = ((*(unaligned vec<float, 2>*)&this->__float3_458 > max_pos) | (*(unaligned vec<float, 2>*)&this->__float3_464 < min_pos));
    //    compare ^= {0xFFFFFFFF, 0xFFFFFFFF};
    //    if (
    //        bitcast(double, compare) == 0.0
    //    ) {
    //        return 0;
    //    } else {
    //        if (!this->__byte_9E0) {
    //            this->die();
    //        }
    //        return 1;
    //    }
    //    
    //    //__v4sf div_by_two = { 0.5f, 0.5f, 0.0f, 0.0f };
    //    ////__v4sf div_by_two = { 0.5f, 0.5f };
    //    //__v4sf temp_size = *(__m128_u*)size;
    //    //temp_size *= div_by_two;
    //    //__v4sf temp_pos = *(__m128_u*)position;
    //    //__v4sf min_pos = temp_pos - temp_size;
    //    //__v4sf max_pos = temp_pos + temp_size;
    //
    //    //const PlayerRect *restrict player_rect = this->__player_rect_array_8B8;
    //    //for (size_t ii = 0; ii < countof(this->__player_rect_array_8B8); ++ii, ++player_rect) {
    //    //    __v4sf temp_rect_size = *((__m128_u*)&player_rect->size);
    //    //    if (!temp_rect_size[0]) continue;
    //    //    temp_rect_size *= div_by_two;
    //    //    __v4sf temp_rect_pos = *((__m128_u*)&player_rect->position);
    //    //    __v4sf rect_min = temp_rect_pos - temp_rect_size;
    //    //    __v4sf rect_max = temp_rect_pos + temp_rect_size;
    //    //    if (_mm_movemask_ps(_mm_cmple_ps(bitcast(__m128, rect_min), bitcast(__m128, max_pos))) &
    //    //        _mm_movemask_ps(_mm_cmpge_ps(bitcast(__m128, rect_max), bitcast(__m128, min_pos)))
    //    //        & 0b11) {
    //    //        return 2;
    //    //    }
    //    //}
    //    //if ((_mm_movemask_ps(_mm_cmpgt_ps(bitcast(__m128, *(__m128_u*)&this->__float3_458), bitcast(__m128, max_pos))) |
    //    //    _mm_movemask_ps(_mm_cmplt_ps(bitcast(__m128, *(__m128_u*)&this->__float3_464), bitcast(__m128, min_pos))))
    //    //    & 0b11) {
    //    //    return 0;
    //    //} else {
    //    //    if (!this->__byte_9E0) {
    //    //        this->die();
    //    //    }
    //    //    return 1;
    //    //}
    //}

    //// 0x426C40
    //dllexport int32_t check_collision(Float3* restrict position, Float3* restrict size) asm("{[codecave:PLAYER_check_collision_old1]}") {
    //    float temp_width = size->x / 2.0f;
    //    float temp_x_pos = position->x;
    //    float x_min = temp_x_pos - temp_width;
    //    float x_max = temp_x_pos + temp_width;
    //
    //    float temp_height = size->y / 2.0f;
    //    float temp_y_pos = position->y;
    //    float y_min = temp_y_pos - temp_height;
    //    float y_max = temp_y_pos + temp_height;
    //
    //    PlayerRect* restrict player_rect = this->__player_rect_array_8B8;
    //    for (size_t ii = 0; ii < countof(this->__player_rect_array_8B8); ++ii, ++player_rect) {
    //        float rect_width = player_rect->size.x;
    //        if (rect_width == 0.0f) continue;
    //        rect_width /= 2.0f;
    //
    //        float rect_x_pos = player_rect->position.x;
    //        if ((rect_x_pos - rect_width) > x_max || (rect_x_pos + rect_width) < x_min) continue;
    //
    //        float rect_height = player_rect->size.y / 2.0f;
    //        float rect_y_pos = player_rect->position.y;
    //        if ((rect_y_pos - rect_height) > y_max || (rect_y_pos + rect_height) < y_min) continue;
    //        return 2;
    //    }
    //    if (this->__float3_458.x <= x_max && this->__float3_458.y <= y_max &&
    //        this->__float3_464.x >= x_min && this->__float3_464.y >= y_min) {
    //        if (!this->__byte_9E0) {
    //            this->die();
    //        }
    //        return 1;
    //    }
    //    return 0;
    //}

    dllexport float angle_to_player_from_point(const Float3 *const restrict point) asm("{[codecave:PLAYER_angle_to_player_from_point]}") {
        vec<float, 2> temp = *(unaligned vec<float, 2>*)&this->position - *(unaligned vec<float, 2>*)point;
        if ((temp[0] != 0.0f) & (temp[1] != 0.0f)) {
            vec<double, 2> temp2 = convertvec(temp, vec<double, 2>);
            return zatan2f(temp2[1], temp2[0]);
        } else {
            return HALF_PI_f;
        }
    }

    dllexport float angle_to_player_from_point_fast(const Float3 *const restrict point) {
        vec<float, 4> temp = { this->position.x, this->position.y, 0.0f, 0.0f };
        temp -= *(unaligned vec<float, 4>*)point;
        vec<float, 4> temp2 = { 0.0f, 0.0f, 0.0f, 0.0f };
        temp2 = _mm_cmpneq_ps(temp, temp2);
        temp2 = _mm_cvtepi32_epi64((__m128i)temp2);
        if (!_mm_testc_pd((__m128d)temp2, (__m128d)temp2)) {
            __asm__ volatile ("SUB $0x10, %%ESP":);
            vec<double, 2> temp2 = _mm_cvtps_pd(temp);
            temp2 = { temp2[1], temp2[0] };
            /*
            *(unaligned vec<double, 2>*)esp_reg = temp2;
            __asm__ volatile (
                "fstps (%%ESP)"
                :
                : "X"(atan2_impl_manual(), NULL)
                : clobber_list("eax", "ecx", "edx")
            );
            float ret = *(volatile float*)esp_reg;
            __asm__ volatile ("ADD $0x10, %%ESP":);
            return ret;
            */
            return zatan2f(temp2[1], temp2[0]);
        } else {
            return HALF_PI_f;
        }
        /*vec<float, 2> temp = *(unaligned vec<float, 2>*)this->position.as_array - *(unaligned vec<float, 2>*)point;
        if ((temp[0] != 0.0f) & (temp[1] != 0.0f)) {
            vec<double, 2> temp2 = convertvec(temp, vec<double, 2>);
            return zatan2f(temp2[1], temp2[0]);
        } else {
            return HALF_PI_f;
        }*/
    }

    dllexport float distance_to_player_from_point(Float3 *restrict point_in) asm("{[codecave:PLAYER_distance_to_player_from_point]}") {
        
        unaligned vec<float, 4>& position = *(unaligned vec<float, 4>*)&this->position;
        unaligned vec<float, 4>& point = *(unaligned vec<float, 4>*)point_in;
        vec<float, 4> temp;
        /*temp[0] = position[0] - point[0];
        temp[1] = position[1] - point[1];
        temp[2] = position[2] - point[2];*/
        temp = position - point;
        temp *= temp;
        float calc = temp[0] + temp[1] + temp[2];
        __asm__(
            "VSQRTSS %[calc], %[calc], %[calc]"
            : [calc] "+x"(calc)
        );
        //return __builtin_sqrtf(calc);
        return calc;
        

        
        /*float ret;
        float scratch;
        __asm__(
            "MOVUPS %[position], %[scratch] \n"
            "SUBPS %[point], %[scratch] \n"
            "ANDPS %[mask], %[scratch] \n"
            "MULPS %[scratch], %[scratch] \n"
            "MOVSHDUP %[scratch], %[ret]\n"
            "ADDPS %[scratch], %[ret] \n"
            "MOVHLPS %[ret], %[scratch] \n"
            "ADDSS %[scratch], %[ret] \n"
            "SQRTSS %[ret], %[ret] \n"
            : [ret]"+x"(ret), [scratch]"=x"(scratch)
            : [position]"X"(this->position), [point]"X"(*point_in), [mask]"m"(Float3VecMask)
        );
        scratch = scratch;
        return ret;*/

        /*float ret;
        __asm__(
            "MOVUPS %1, %%xmm0 \n"
            "SUBPS %2, %%xmm0 \n"
            "ANDPS %3, %%xmm0 \n"
            "MULPS %%xmm0, %%xmm0 \n"
            "MOVSHDUP %%xmm0, %%xmm1\n"
            "ADDPS %%xmm1, %%xmm0 \n"
            "MOVHLPS %%xmm0, %%xmm1 \n"
            "ADDSS %%xmm1, %%xmm0 \n"
            "SQRTSS %%xmm0, %%xmm0 \n"
            "MOVD %%xmm0, %0"
            : "=r" (ret)
            : "m" (this->position), "m" (*point), "m" (Float3VecMask)
        );
        return ret;*/

        //return sqrtf(powf(this->position.x - point->x, 2.0f) + powf(this->position.y - point->y, 2.0f) + powf(this->position.z - point->z, 2.0f));
        
        /*float temp = this->position.x - point->x;
        float total = temp * temp;
        temp = this->position.y - point->y;
        total += temp * temp;
        temp = this->position.z - point->z;
        total += temp * temp;
        assume(total >= 0.0f);
        return sqrtf(total);*/
    }

};  // 0x98f0

extern "C" {
    // 0x6CA628
    extern Player PLAYER asm("_PLAYER");
}

// ====================
// Items
// ====================

enum ItemID {
    PowerItem = 0,
    PointItem = 1,
    BigPowerItem = 2,
    BombItem = 3,
    FItem = 4,
    LifeItem = 5,
    StarItem = 6
};

typedef struct Item Item;
struct Item {
    AnmVM vm;  // 0x0
    Float3 position;  // 0x110
    Float3 velocity;  // 0x11c
    Float3 __float3_128;  // 0x128
    Timer __timer_134;  // 0x134
    uint8_t item_type;  // 0x140
    int8_t in_use_flag;  // 0x141
    uint8_t __byte_142;  // 0x142
    uint8_t state;  // 0x143
};  // 0x144

typedef struct ItemManager ItemManager;
struct ItemManager {
    Item items[512];  // 0x0
    Item dummy_item_for_failed_spawns;  // 0x28800
    int32_t next_index;  // 0x28944
    int32_t item_count;  // 0x28948

    // 0x41F290
    dllexport void thiscall spawn_item(Float3* position, int32_t item_type, int32_t item_state) asm("{[codecave:ITEM_MANAGER_spawn_item]}") {
        Item *restrict new_item = &this->items[this->next_index];
        Item *const starting_item = new_item;
        do {
            if (!new_item->in_use_flag) break;
            ++new_item;
            if (new_item == array_end_addr(this->items)) {
                new_item = this->items;
            }
        } while (new_item != starting_item);
        if (Item *restrict next_item = new_item + 1;
            expect(next_item != array_end_addr(this->items), true)) {
            this->next_index = next_item - &this->items[0];
        } else {
            this->next_index = 0;
        }
        
        /*int32_t next_index = this->next_index;
        Item *restrict new_item = &this->items[next_index];
        for (size_t ii = 0; ii < countof(this->items); ++ii) {
            ++next_index;
            if (!new_item->in_use_flag) break;
            if (next_index < countof(this->items)) {
                ++new_item;
            } else {
                next_index = 0;
                new_item = this->items;
            }
        }
        if (next_index >= countof(this->items)) {
            next_index = 0;
        }
        this->next_index = next_index;*/

        new_item->in_use_flag = true;
        new_item->item_type = item_type;
        new_item->state = item_state;
        //new_item->__timer_134 = static_default_timer<0>;
        new_item->__timer_134.set_default();
        new_item->position = *position;
        if (expect(item_state != 2, true)) {
            new_item->velocity = (Float3){ 0.0f, -2.2f, 0.0f };
        } else {
            new_item->velocity = *position;
            new_item->__float3_128.x = RNG.rand_float() * 288.0f + 48.0f;
            new_item->__float3_128.y = RNG.rand_float() * 192.0f - 64.0f;
            new_item->__float3_128.z = 0.0f;
        }
        int32_t item_script_index = item_type + 533;
        new_item->vm.script_number = item_script_index;
        ANM_MANAGER_PTR->set_vm_script(&new_item->vm, ANM_MANAGER_PTR->scripts[item_script_index]);
        new_item->vm.color = PackD3DCOLOR(255, 255, 255, 255);
        new_item->__byte_142 = 1;
    }
    
    // 0x41F4A0
    dllexport gnu_noinline void thiscall on_tick() {
        
    }

};  // 0x2894c

extern "C" {
    // 0x69E268
    extern ItemManager ITEM_MANAGER asm("_ITEM_MANAGER");
}

// ====================
// ECL
// ====================

typedef struct Enemy Enemy;

typedef struct EclInstruction EclInstruction;
struct EclInstruction {
    uint32_t time;
    int16_t opcode;
    int16_t offset_to_next;
    uint8_t unused;
    uint8_t difficulty_mask;
    uint16_t param_mask;
    unsigned char args[];
};
ValidateFieldOffset(0x0, EclInstruction, time);
ValidateFieldOffset(0x4, EclInstruction, opcode);
ValidateFieldOffset(0x6, EclInstruction, offset_to_next);
ValidateFieldOffset(0x8, EclInstruction, unused);
ValidateFieldOffset(0x9, EclInstruction, difficulty_mask);
ValidateFieldOffset(0xA, EclInstruction, param_mask);
ValidateFieldOffset(0xC, EclInstruction, args);

typedef void(*cdecl ins_call)(Enemy* restrict enemy, EclInstruction* restrict current_instruction);

typedef struct EclContext EclContext;
struct EclContext {
    EclInstruction *restrict current_instr;  // 0x0
    Timer time;  // 0x4
    ins_call func;  // 0x10
    int32_t int_vars[4];  // 0x14
    float float_vars[4];  // 0x24
    int32_t counter_vars[4];  // 0x34
    int32_t compare_register;  // 0x44
    uint16_t sub_id;  // 0x48
    probably_padding_bytes(0x2);  // 0x4A
};  // 0x4c
ValidateFieldOffset(0x0, EclContext, current_instr);
ValidateFieldOffset(0x4, EclContext, time);
ValidateFieldOffset(0x4, EclContext, time.previous);
ValidateFieldOffset(0x8, EclContext, time.subframe);
ValidateFieldOffset(0xC, EclContext, time.current);
ValidateFieldOffset(0x10, EclContext, func);
ValidateFieldOffset(0x14, EclContext, int_vars);
ValidateFieldOffset(0x24, EclContext, float_vars);
ValidateFieldOffset(0x34, EclContext, counter_vars);
ValidateFieldOffset(0x44, EclContext, compare_register);
ValidateFieldOffset(0x48, EclContext, sub_id);

typedef union EclValue EclValue;
union EclValue {
    void* address;
    int32_t* integer;
    float* real;
    int16_t* word;
    unsigned char* raw;
};

typedef union EclArg EclArg;
union EclArg {
    int32_t integer;
    uint32_t uinteger;
    float real;

    /*forceinline operator int32_t() const {
        return this->integer;
    }

    forceinline operator float() const {
        return this->real;
    }*/
};

// ====================
// Others
// ====================

typedef struct ScorefileCATK ScorefileCATK;
struct ScorefileCATK {
    char magic_CATK[0x4];  // 0x0
    uint16_t size_1;  // 0x4
    uint16_t size_2;  // 0x6
    uint8_t __byte_8;  // 0x8
    probably_padding_bytes(0x3);  // 0x9
    int32_t highscore;  // 0xc
    short __word_10;  // 0x10
    uint8_t spell_name_hash;  // 0x12
    uint8_t most_recent_capture_shottype;  // 0x13
    unknown_fields(0x4);  // 0x14
    char spell_name[36];  // 0x18
    short attempts;  // 0x3c
    short captures;  // 0x3e
};  // 0x40

typedef struct ScorefileCLRD ScorefileCLRD;
struct ScorefileCLRD {
    char magic_CLRD[0x4];  // 0x0
    uint16_t size_1;  // 0x4
    uint16_t size_2;  // 0x6
    uint8_t __byte_8;  // 0x8
    probably_padding_bytes(0x3);  // 0x9
    uint8_t clear_counts[0x5];  // 0xc
    uint8_t continue_counts[0x5];  // 0x11
    uint8_t character_shot_id;  // 0x16
    probably_padding_bytes(0x1);  // 0x17
};  // 0x18

typedef struct ScorefilePSCR ScorefilePSCR;
struct ScorefilePSCR {
    char magic_PSCR[0x4];  // 0x0
    uint16_t size_1;  // 0x4
    uint16_t size_2;  // 0x6
    uint8_t __byte_8;  // 0x8
    probably_padding_bytes(0x3);  // 0x9
    int32_t score;  // 0xc
    uint8_t character;  // 0x10
    uint8_t difficulty;  // 0x11
    uint8_t stage;  // 0x12
    probably_padding_bytes(0x1);  // 0x13
};  // 0x14

typedef struct GameManager GameManager;
struct GameManager  {
    int __dword_0;  // 0x0
    int32_t score;  // 0x4
    int __dword_8;  // 0x8
    int32_t high_score;  // 0xc
    int32_t difficulty;  // 0x10
    int32_t graze_in_stage;  // 0x14
    int __dword_18;  // 0x18
    int32_t __spellcard_related_1;  // 0x1c
    int __dword_20;  // 0x20
    int __dword_24;  // 0x24
    int __dword_28;  // 0x28
    char __byte_2C;  // 0x2c
    probably_padding_bytes(0x3);  // 0x2d
    ScorefileCATK catk_data[0x40];  // 0x30
    ScorefileCLRD clrd_data[0x4];  // 0x1030
    ScorefilePSCR pscr_data[0x60];  // 0x1090
    int16_t current_power;  // 0x1810
    probably_padding_bytes(0x2);  // 0x1812
    uint16_t point_items_collected_in_stage;  // 0x1814
    short __word_1816;  // 0x1816
    char __byte_1818;  // 0x1818 // continue count?
    int8_t power_item_count_for_score;  // 0x1819
    int8_t lives_remaining;  // 0x181a
    int8_t bombs_remaining;  // 0x181b
    char __byte_181C;  // 0x181c
    int8_t character;  // 0x181d
    int8_t shottype;  // 0x181e
    char __byte_181F;  // 0x181f
    char __byte_1820;  // 0x1820 // show continue menu?
    char __byte_1821;  // 0x1821
    char __byte_1822;  // 0x1822
    uint8_t __byte_1823;  // 0x1823
    char demoplay_mode;  // 0x1824
    unknown_fields(0x7);  // 0x1825
    char replay_filename[0x100];  // 0x182c
    char __gap_9[0x100];  // 0x192c
    short __word_1A2C;  // 0x1a2c
    unknown_fields(0x6);  // 0x1a2e
    int32_t current_stage;  // 0x1a34
    int __dword_1A38;  // 0x1a38
    Float2 arcade_region_top_left_pos;  // 0x1a3c
    Float2 arcade_region_size;  // 0x1a44
    Float2 __float2_1A4C;  // 0x1a4c
    Float2 __float2_1A54;  // 0x1a54
    float __float_1A5C;  // 0x1a5c
    Float3 stage_camera_facing_dir;  // 0x1a64
    int32_t __int_1A6C;  // 0x1a6c
    struct {
        int32_t current;  // 0x1a70
        int32_t max;  // 0x1a74
        int32_t min;  // 0x1a78
        int32_t subrank;  // 0x1a7c
    } rank;

    // 0x41C57E
    dllexport void increase_rank(int32_t amount) asm("{[codecave:GAME_MANAGER_increase_rank]}") {
        int32_t subrank_temp = this->rank.subrank + amount;
        if (subrank_temp >= 100) {
            int32_t rank_temp = 0;
            while (subrank_temp >= 100) {
                ++rank_temp;
                subrank_temp -= 100;
            }
            rank_temp += this->rank.current;
            this->rank.current = rank_temp <= this->rank.max ? rank_temp : this->rank.max;
        }
        this->rank.subrank = subrank_temp;
    }

    // 0x41C5FA
    dllexport void decrease_rank(int32_t amount) asm("{[codecave:GAME_MANAGER_decrease_rank]}") {
        int32_t subrank_temp = this->rank.subrank - amount;
        if (subrank_temp < 0) {
            int32_t rank_temp = 0;
            while (subrank_temp < 0) {
                --rank_temp;
                subrank_temp += 100;
            }
            rank_temp += this->rank.current;
            this->rank.current = rank_temp >= this->rank.min ? rank_temp : this->rank.min;
        }
        this->rank.subrank = subrank_temp;
    }

    dllexport void modify_rank(int32_t amount) asm("{[codecave:GAME_MANAGER_modify_rank]}") {
        if (amount >= 0) {
            this->increase_rank(amount);
        } else {
            this->decrease_rank(-amount);
        }
    }

    // 0x41B5E1
    dllexport uint32_t thiscall check_visibility(float X, float Y, float W, float H) asm("{[codecave:GAME_MANAGER_check_visibility]}") {
        
        /*W *= -0.5f;
        H *= -0.5f;
        Float2 bounds = this->arcade_region_size;
        bounds.x -= W;
        bounds.y -= H;
        return expect(X >= W && Y >= H && bounds.x >= X && bounds.y >= Y, true);*/

        BoundingBoxSource input_box;
        input_box.make_from_raw_pos_size(X, Y, W, H);

        BoundingBoxDest screen_box;
        screen_box.make_from_max(this->arcade_region_size);

        return input_box.test_intersect(screen_box);

        /*vec<float, 2> pos = { X, Y };
        vec<float, 2> size = { W, H };
        size *= -0.5f;
        vec<float, 2> bounds = this->arcade_region_size.as_vec;
        bounds -= size;

        vec<uint32_t, 2> compare = (bounds < pos) | (pos < size);
        return bitcast(double, compare) == 0.0;*/


        //return compare[0] == 0 && compare[1] == 0;
        //return _mm_ucomieq_sd(bitcast(__m128d, compare), bitcast(__m128d, true_compare));

        //vec<uint32_t, 2> compare = (bounds >= pos) & (pos >= size);
        //const vec<uint32_t, 2> true_compare = { 0xFFFFFFFF, 0xFFFFFFFF };
        //return compare[0] == true_compare[0] && compare[1] == true_compare[1];

        //return _mm_ucomieq_sd(((bounds >= pos) & (pos >= size)), true_compare);
        //return bitcast(vec<double, 1>, ((bounds >= pos) & (pos >= size)))[0] == bitcast(vec<double, 1>, (vec<int, 2>) { -1, -1 })[0];
        //return (vec<int, 2>)((bounds >= pos) & (pos >= size)) == (vec<int, 2>) { -1, -1 };
        //return pos[0] >= size[0] & pos[1] >= size[1] & bounds[0] >= pos[0] & bounds[1] >= pos[1];
        
        
        /*W *= 0.5f;
        H *= 0.5f;
        float X_max = X + W;
        float Y_max = Y + H;
        float X_min = X - W;
        float Y_min = Y - H;
        return X_max >= 0.0f & Y_max >= 0.0f & X_min <= this->arcade_region_size.x & Y_min <= this->arcade_region_size.y;*/
        /*__v4sf zero = { 0.0f, 0.0f, 0.0f, 0.0f };
        __v4sf div_by_two = { 0.5f, 0.5f, 0.0f, 0.0f };
        __v4sf size = *(__v4sf_u*)&W;
        size *= div_by_two;
        __v4sf position = *(__v4sf_u*)&X;
        __v4sf max_pos = position + size;
        __v4sf min_pos = position - size;
        __v4sf max_pos_temp = _mm_max_ps(max_pos, zero);
        __v4sf min_pos_temp = _mm_min_ps(min_pos, *(__v4sf_u*)&this->arcade_region_size);*/
        /*__v4sf div_by_neg_2 = { -0.5f, -0.5f, 0.0f, 0.0f };
        __v4sf size = { W, H };
        size *= div_by_neg_2;
        __v4sf bounds = *(__v4sf_u*)&this->arcade_region_size - size;
        __v4sf position = { X, Y };
        return !(_mm_movemask_ps(_mm_and_ps(_mm_cmplt_ps(position, size), _mm_cmpgt_ps(position, bounds))) & 0b11);*/
        /*__v4sf zero = { 0.0f, 0.0f, 0.0f, 0.0f };
        __v4sf div_by_two = { 0.5f, 0.5f, 0.0f, 0.0f };
        __v4sf size = *(__v4sf_u*)&W;
        size *= div_by_two;
        __v4sf position = *(__v4sf_u*)&X;
        __v4sf max_pos = position + size;
        __v4sf min_pos = position - size;
        return _mm_movemask_pd(_mm_cvtepi32_epi64((__m128i)_mm_and_ps(_mm_cmpge_ps(max_pos, zero), _mm_cmple_ps(min_pos, *(__v4sf_u*)&this->arcade_region_size)))) == 0b11;*/
        /*W /= 2.0f;
        H /= 2.0f;
        float x_max = X + W;
        float y_max = Y + H;
        float x_min = X - W;
        float y_min = Y - H;
        return (x_max >= 0.0f & y_max >= 0.0f) && (x_min <= this->arcade_region_size.x & y_min <= this->arcade_region_size.y);*/
        /*H *= -0.5f;
        W *= -0.5f;
        float temp_x = X - W;
        float temp_y = Y - H;
        float x_bounds = this->arcade_region_size.x;
        float y_bounds = this->arcade_region_size.y;
        return X >= W && Y >= H;*/
        /*W /= 2.0f;
        H /= 2.0f;
        float x_max = X + W;
        float y_max = Y + H;
        float x_max_temp = __max(x_max, 0.0f);
        float y_max_temp = __max(y_max, 0.0f);
        float x_min = X - W;
        float y_min = Y - H;
        float x_min_temp = __min(x_min, this->arcade_region_size.x);
        float y_min_temp = __min(y_min, this->arcade_region_size.y);
        return x_max == x_max_temp & y_max == y_max_temp & x_min == x_min_temp & y_min == y_min_temp;*/
        /*W /= 2.0f;
        H /= 2.0f;
        float x_center = X + W;
        float y_center = Y + H;
        float x_temp = __min(x_center, 0.0f);
        float y_temp = __min(y_center, 0.0f);
        x_temp = __max(x_temp, this->arcade_region_size.x);
        y_temp = __max(y_temp, this->arcade_region_size.y);
        return x_center == x_temp & y_center == y_temp;*/
    }

};  // 0x1a80

extern "C" {
    // 0x69BCA0
    extern GameManager GAME_MANAGER asm("_GAME_MANAGER");
}

typedef struct MsgInstruction MsgInstruction;
struct MsgInstruction {
    uint16_t time;  // 0x0
    uint8_t opcode;  // 0x2
    uint8_t args_size;  // 0x3
    unsigned char args[];
};  // 0x4

typedef struct MsgHeader MsgHeader;
struct MsgHeader {
    uint32_t num_entries;  // 0x0
    MsgInstruction *restrict entries[10];  // 0x4
};  // 0x2c

struct GuiMsgVM {
    MsgHeader *restrict msg_file;  // 0x0, 0x2534
    MsgInstruction *restrict current_instr;  // 0x4, 0x2538
    int32_t current_msg_index;  // 0x8, 0x253C
    Timer script_time;  // 0xc, 0x2B40
    int32_t frames_elapsed_during_pause;  // 0x18, 0x254C
    AnmVM portraits[2];  // 0x1c, 0x2550
    AnmVM dialogue_lines[2];  // 0x23c, 0x2770
    AnmVM intro_lines[2];  // 0x45c, 0x2990
    D3DCOLOR text_colors_A[4];  // 0x67c, 0x2BB0
    D3DCOLOR text_colors_B[4];  // 0x68c, 0x2BC0
    uint32_t font_size;  // 0x69c, 0x2BD0
    uint32_t ignore_wait_counter;  // 0x6a0, 0x2BD4
    uint8_t dialog_skippable;  // 0x6a4, 0x2BD8
    probably_padding_bytes(0x3);  // 0x6a5
};  // 0x6A8
ValidateFieldOffset(0x0, GuiMsgVM, msg_file);
ValidateFieldOffset(0x4, GuiMsgVM, current_instr);
ValidateFieldOffset(0x8, GuiMsgVM, current_msg_index);
ValidateFieldOffset(0xC, GuiMsgVM, script_time);
ValidateFieldOffset(0x18, GuiMsgVM, frames_elapsed_during_pause);
ValidateFieldOffset(0x1C, GuiMsgVM, portraits);
ValidateFieldOffset(0x23C, GuiMsgVM, dialogue_lines);
ValidateFieldOffset(0x45C, GuiMsgVM, intro_lines);
ValidateFieldOffset(0x67C, GuiMsgVM, text_colors_A);
ValidateFieldOffset(0x68C, GuiMsgVM, text_colors_B);
ValidateFieldOffset(0x69C, GuiMsgVM, font_size);
ValidateFieldOffset(0x6A0, GuiMsgVM, ignore_wait_counter);
ValidateFieldOffset(0x6A4, GuiMsgVM, dialog_skippable);
ValidateStructSize(0x6A8, GuiMsgVM);

typedef struct GuiImplChildB GuiImplChildB;
struct GuiImplChildB {
    Float3 __float3_0;  // 0x0
    int32_t __dword_C;  // 0xc
    int32_t __dword_10;  // 0x10
    Timer __timer_14;  // 0x14
};  // 0x20

extern "C" {
    extern const char MusicFormatString[] asm("_MusicFormatString");
}

typedef struct GuiImpl GuiImpl;
struct GuiImpl {
    AnmVM __vms_0[26];  // 0x0
    uint8_t __byte_1BA0;  // 0x1ba0
    unknown_fields(0x3);  // 0x1ba1
    AnmVM __vm_1BA4;  // 0x1ba4
    AnmVM __vm_1CB4;  // 0x1cb4
    AnmVM __vm_1DC4;  // 0x1dc4
    AnmVM __vm_1ED4;  // 0x1ed4
    AnmVM __vm_1FE4;  // 0x1fe4
    AnmVM __vm_20F4;  // 0x20f4
    AnmVM __vm_2204;  // 0x2204
    AnmVM __vm_2314;  // 0x2314
    AnmVM __vm_2424;  // 0x2424

    GuiMsgVM msg;  // 0x2534

    int __dword_2BDC;  // 0x2bdc
    int32_t stage_score;  // 0x2be0
    GuiImplChildB __child_b_2BE4;  // 0x2be4
    GuiImplChildB __child_b_2C04;  // 0x2c04
    GuiImplChildB __child_b_2C24;  // 0x2c24

    // 0x41878B
    dllexport void thiscall read_msg(int32_t msg_index) asm("{[codecave:GUI_IMPL_read_msg]}") {
        MsgHeader *const restrict msg_file = this->msg.msg_file;
        if (msg_index < msg_file->num_entries) {
            this->msg.current_msg_index = msg_index;
            this->msg.current_instr = msg_file->entries[msg_index];
            __builtin_memset(&this->msg.script_time, 0, sizeof(this->msg) - 0xC);
            this->msg.dialogue_lines[0].script_number = -1;
            this->msg.dialogue_lines[1].script_number = -1;
            this->msg.font_size = 15;
            this->msg.text_colors_A[0] = PackD3DCOLOR(0, 232, 240, 255);
            this->msg.text_colors_A[1] = PackD3DCOLOR(0, 255, 232, 240);
            this->msg.text_colors_B[0] = PackD3DCOLOR(0, 0, 0, 0);
            this->msg.text_colors_B[1] = PackD3DCOLOR(0, 0, 0, 0);
            this->msg.dialog_skippable = 1;
            switch (expect(GAME_MANAGER.current_stage, 0)) {
                case 6:
                    ANM_MANAGER_PTR->load_anm_file(11, /* data/eff06.anm*/ (char*)0x46AC00, 691);
                    break;
                case 7:
                    ANM_MANAGER_PTR->load_anm_file(11, /* data/eff07.anm*/ (char*)0x46ABF0, 691);
                    ANM_MANAGER_PTR->load_anm_file(18, /* data/face12c.anm*/ (char*)0x46ABDC, 1192);
                    break;
                default:
                    break;
            }
        }
        return;
        //if (msg_index < this->msg.msg_file->num_entries) {
        //    MsgHeader* temp_msg_file = this->msg.msg_file;
        //    memset(&this->msg, 0, sizeof(this->msg));
        //    this->msg.current_msg_index = msg_index;
        //    this->msg.msg_file = temp_msg_file;
        //    this->msg.current_instr = this->msg.msg_file->entries[msg_index];
        //    this->msg.dialogue_lines[0].script_number = -1;
        //    this->msg.dialogue_lines[1].script_number = -1;
        //    this->msg.__dword_69C = 15;
        //    this->msg.enemy_text_color = PackD3DCOLOR(0, 232, 240, 255);
        //    this->msg.player_text_color = PackD3DCOLOR(0, 255, 232, 240);
        //    this->msg.__dword_68C = 0;
        //    this->msg.__dword_690 = 0;
        //    this->msg.dialog_skippable = 1;
        //    if (GAME_MANAGER.current_stage == 6 && (msg_index == 0 || msg_index == 10)) {
        //        ANM_MANAGER_PTR->load_anm_file(11, /* data/eff06.anm*/ (char*)0x46AC00, 691);
        //    } else if (GAME_MANAGER.current_stage == 7 && (msg_index == 0 || msg_index == 10)) {
        //        ANM_MANAGER_PTR->load_anm_file(11, /* data/eff07.anm*/ (char*)0x46ABF0, 691);
        //        ANM_MANAGER_PTR->load_anm_file(18, /* data/face12c.anm*/ (char*)0x46ABDC, 1192);
        //    }
        //}
    }

    // 0x4188C9
    dllexport int32_t thiscall run_msg() asm("{[codecave:GUI_IMPL_run_msg]}");

};  // 0x2c44
ValidateFieldOffset(0x1fe4, GuiImpl, __vm_1FE4);

typedef struct Gui Gui;
struct Gui {
    union {
        uint32_t __flags_0;  // 0x0
    };
    GuiImpl *restrict impl;  // 0x4
    float __float_8;  // 0x8
    float blue_spellcard_bar_length;  // 0xc
    uint32_t __uint_10;  // 0x10
    int32_t __ecl_set_lives;  // 0x14
    int32_t __read_by_spellcard_ecl;  // 0x18
    unknown_fields(0x4);  // 0x1c
    bool boss_present;  // 0x20
    unknown_fields(0x3);  // 0x21
    float __boss_health_bar_24;  // 0x24
    float __boss_health_bar_28;  // 0x28

    // 0x41735A
    dllexport void thiscall __sub_41735A(int32_t arg1) asm("{[codecave:GUI_sub_41735A]}") {
        GuiImpl *restrict impl = this->impl;
        impl->__child_b_2BE4.__float3_0 = (Float3){ 416.0f, 32.0f, 0.0f };
        impl->__child_b_2BE4.__dword_10 = 1;
        //impl->__child_b_2BE4.__timer_14 = static_default_timer<0>;
        impl->__child_b_2BE4.__timer_14.set_default();
        impl->__child_b_2BE4.__dword_C = arg1;
    }

    // 0x4172D3
    dllexport int32_t thiscall __sub_4172D3(void) asm("{[codecave:GUI_sub_4172D3]}") {
        AnmVM *const restrict vm = &this->impl->__vm_2424;
        return vm->sprite_number >= 0 && vm->stop_enable;
    }

    // 0x417314
    dllexport void thiscall vm5_auto_rotate(void) asm("{[codecave:GUI_vm5_auto_rotate]}") {
        this->impl->__vm_1FE4.__auto_rotate = 1;
    }

    // 0x41732C
    dllexport void thiscall vm6_auto_rotate(void) asm("{[codecave:GUI_vm6_auto_rotate]}") {
        this->impl->__vm_20F4.__auto_rotate = 1;
    }

    // 0x417344
    dllexport uint32_t thiscall get_dialog_skippable(void) asm("{[codecave:GUI_get_dialog_skippable]}") {
        return this->impl->msg.dialog_skippable;
    }

    // 0x417BFD
    dllexport void thiscall spawn_spellcard_effects(int32_t face_arg, char *restrict name_str) asm("{[codecave:GUI_spawn_spellcard_effects]}") {
        GuiImpl *const restrict impl = this->impl;
        impl->__vm_1ED4.script_number = 1187;
        ANM_MANAGER_PTR->set_vm_script(&impl->__vm_1ED4, ANM_MANAGER_PTR->scripts[1187]);
        ANM_MANAGER_PTR->set_vm_sprite(&impl->__vm_1ED4, face_arg + 1192);
        impl->__vm_20F4.script_number = 1799;
        ANM_MANAGER_PTR->set_vm_script(&impl->__vm_20F4, ANM_MANAGER_PTR->scripts[1799]);
        ANM_MANAGER_PTR->__draw_text_to_vm_right(&impl->__vm_20F4, PackD3DCOLOR(0, 255, 240, 240), PackD3DCOLOR(0, 0, 0, 0), name_str);
        this->blue_spellcard_bar_length = (int32_t)byteloop_strlen(name_str) * 7.5f + 16.0f;
        //this->blue_spellcard_bar_length = ((int32_t)byteloop_strlen(name_str) * 15) / 2.0f + 16.0f;
        SOUND_MANAGER.play_sound_centered(14);
    }

    // 0x417458
    dllexport gnu_noinline void thiscall __sub_417458(int32_t score) asm("{[0x417458]}") {
        gnu_used static volatile auto tempA = this;
        gnu_used static volatile auto tempB = score;
        assume_all_registers_volatile();
    }

    // 0x4195A2
    dllexport int32_t msg_is_active(void) asm("{[codecave:GUI_msg_is_active]}") {
        return this->impl->msg.current_msg_index >= 0;
    }

    // 0x418768
    dllexport void __read_msg(int32_t msg_index) asm("{[codecave:GUI_read_msg]}") {
        this->impl->read_msg(msg_index);
        SUPERVISOR.__dword_198 = 3;
    }

    // 0x419572
    dllexport int32_t msg_is_waiting(void) asm("{[codecave:GUI_msg_is_waiting]}") {
        if (this->impl->msg.ignore_wait_counter > 0) {
            return false;
        } else {
            return this->msg_is_active();
        }
    }

    // 0x4195BF
    dllexport void thiscall on_tick_impl() asm("{[codecave:GUI_on_tick_impl]}") {
        GuiImpl *restrict gui_impl = this->impl;
        for (size_t ii = 0; ii < 19; ++ii) {
            ANM_MANAGER_PTR->run_anm(&gui_impl->__vms_0[ii]);
        }
        if (expect(gui_impl->msg.current_msg_index < 0, false)) {
            if (this->boss_present) {
                if (!gui_impl->__byte_1BA0) {
                    gui_impl->__byte_1BA0 = 1;
                    gui_impl->__vms_0[19].script_number = 1555;
                    ANM_MANAGER_PTR->set_vm_script(&gui_impl->__vms_0[19], ANM_MANAGER_PTR->scripts[1555]);
                    this->__uint_10 = 0;
                } else {
                    if (ANM_MANAGER_PTR->run_anm(&gui_impl->__vms_0[19])) {
                        gui_impl->__byte_1BA0 = 2;
                    }
                    if (this->__uint_10 < 252) {
                        this->__uint_10 += 4;
                    } else {
                        this->__uint_10 = 255;
                    }
                }
            } else {
                switch (gui_impl->__byte_1BA0) {
                    case 1: case 2:
                        gui_impl->__vms_0[19].script_number = 1556;
                        ANM_MANAGER_PTR->set_vm_script(&gui_impl->__vms_0[19], ANM_MANAGER_PTR->scripts[1556]);
                        gui_impl->__byte_1BA0 = 3;
                    default:
                        if (this->__uint_10 > 0) {
                            this->__uint_10 -= 4;
                        } else {
                            this->__uint_10 = 0;
                        }
                        if (ANM_MANAGER_PTR->run_anm(&gui_impl->__vms_0[19])) {
                            gui_impl->__byte_1BA0 = 0;
                            this->__boss_health_bar_28 = 0.0f;
                            this->__uint_10 = 0;
                        }
                    case 0:
                }
            }
            if (gui_impl->__byte_1BA0 >= 2) {
                float floatA = this->__boss_health_bar_24;
                float floatB = this->__boss_health_bar_28;
                if (floatA > floatB) {
                    floatB += 0.01f;
                    this->__boss_health_bar_28 = __min(floatA, floatB);
                } else if (floatA < floatB) {
                    floatB -= 0.02f;
                    this->__boss_health_bar_28 = __max(floatA, floatB);
                }
            }
        } else {
            ANM_MANAGER_PTR->run_anm(&gui_impl->__vms_0[19]);
        }
        for (size_t ii = 20; ii < countof(gui_impl->__vms_0); ++ii) {
            ANM_MANAGER_PTR->run_anm(&gui_impl->__vms_0[ii]);
        }
        /*for (size_t ii = 0; ii < countof(gui_impl->__vms_0); ++ii) {
            AnmVM *restrict current_vm = &gui_impl->__vms_0[ii];
            if (expect(ii == 19 && gui_impl->msg.current_msg_index < 0, false)) {
                if (this->boss_present) {
                    if (!gui_impl->__byte_1BA0) {
                        gui_impl->__byte_1BA0 = 1;
                        current_vm->script_number = 1555;
                        ANM_MANAGER_PTR->set_vm_script(current_vm, ANM_MANAGER_PTR->scripts[1555]);
                        this->__uint_10 = 0;
                    } else {
                        if (ANM_MANAGER_PTR->run_anm(current_vm)) {
                            gui_impl->__byte_1BA0 = 2;
                        }
                        if (this->__uint_10 < 252) {
                            this->__uint_10 += 4;
                        } else {
                            this->__uint_10 = 255;
                        }
                    }
                } else {
                    switch (gui_impl->__byte_1BA0) {
                        case 1: case 2:
                            current_vm->script_number = 1556;
                            ANM_MANAGER_PTR->set_vm_script(current_vm, ANM_MANAGER_PTR->scripts[1556]);
                            gui_impl->__byte_1BA0 = 3;
                        default:
                            if (this->__uint_10 > 0) {
                                this->__uint_10 -= 4;
                            } else {
                                this->__uint_10 = 0;
                            }
                            if (ANM_MANAGER_PTR->run_anm(current_vm)) {
                                gui_impl->__byte_1BA0 = 0;
                                this->__float_28 = 0.0f;
                                this->__uint_10 = 0;
                            }
                        case 0:
                    }
                }
                if (gui_impl->__byte_1BA0 >= 2) {
                    if (this->__float_24 > this->__float_28) {
                        this->__float_28 += 0.01f;
                        if (this->__float_24 < this->__float_28) {
                            this->__float_28 = this->__float_24;
                        }
                    } else if (this->__float_24 < this->__float_28) {
                        this->__float_28 -= 0.02f;
                        if (this->__float_24 > this->__float_28) {
                            this->__float_28 = this->__float_24;
                        }
                    }
                }
            } else {
                ANM_MANAGER_PTR->run_anm(&gui_impl->__vms_0[ii]);
            }
        }*/
        ANM_MANAGER_PTR->run_anm(&gui_impl->__vm_1BA4);
        ANM_MANAGER_PTR->run_anm(&gui_impl->__vm_1CB4);
        ANM_MANAGER_PTR->run_anm(&gui_impl->__vm_1DC4);
        ANM_MANAGER_PTR->run_anm(&gui_impl->__vm_1FE4);
        ANM_MANAGER_PTR->run_anm(&gui_impl->__vm_1ED4);
        ANM_MANAGER_PTR->run_anm(&gui_impl->__vm_20F4);
        if (gui_impl->__vm_2424.sprite_number >= 0) {
            if (ANM_MANAGER_PTR->run_anm(&gui_impl->__vm_2424)) {
                gui_impl->__vm_2424.sprite_number = -1;
            }
        }
        if (gui_impl->__child_b_2BE4.__dword_10) {
            if (gui_impl->__child_b_2BE4.__timer_14.current < 30) {
                gui_impl->__child_b_2BE4.__float3_0.x = ((float)gui_impl->__child_b_2BE4.__timer_14.current + gui_impl->__child_b_2BE4.__timer_14.subframe) * -312.0f / 30.0f + 416.0f;
            } else {
                gui_impl->__child_b_2BE4.__float3_0.x = 104.0f;
            }
            if (gui_impl->__child_b_2BE4.__timer_14.current >= 250) {
                gui_impl->__child_b_2BE4.__dword_10 = 0;
            }
            gui_impl->__child_b_2BE4.__timer_14.tick();
        }
        if (gui_impl->__child_b_2C04.__dword_10) {
            if (gui_impl->__child_b_2C04.__timer_14.current < 30) {
                gui_impl->__child_b_2C04.__float3_0.x = ((float)gui_impl->__child_b_2C04.__timer_14.current + gui_impl->__child_b_2C04.__timer_14.subframe) * -312.0f / 30.0f + 416.0f;
            } else {
                gui_impl->__child_b_2C04.__float3_0.x = 104.0f;
            }
            if (gui_impl->__child_b_2C04.__timer_14.current >= 180) {
                gui_impl->__child_b_2C04.__dword_10 = 0;
            }
            gui_impl->__child_b_2C04.__timer_14.tick();
        }
        if (gui_impl->__child_b_2C24.__dword_10) {
            if (gui_impl->__child_b_2C24.__timer_14.current >= 280) {
                gui_impl->__child_b_2C24.__dword_10 = 0;
            }
            gui_impl->__child_b_2C24.__timer_14.tick();
        }
        if (gui_impl->__dword_2BDC == 1) {
            int32_t stage_score_bonus = 0;
            stage_score_bonus += GAME_MANAGER.current_stage * 1000;
            stage_score_bonus += GAME_MANAGER.graze_in_stage * 10;
            stage_score_bonus += GAME_MANAGER.current_power * 100;
            stage_score_bonus *= GAME_MANAGER.point_items_collected_in_stage;
            if (GAME_MANAGER.current_stage >= 6) {
                stage_score_bonus += GAME_MANAGER.lives_remaining * 3000000;
                stage_score_bonus += GAME_MANAGER.bombs_remaining * 1000000;
            }
            switch (GAME_MANAGER.difficulty) {
                case 0:
                    stage_score_bonus /= 2;
                    stage_score_bonus -= stage_score_bonus % 10;
                    break;
                case 2:
                    stage_score_bonus *= 12;
                    stage_score_bonus /= 10;
                    stage_score_bonus -= stage_score_bonus % 10;
                    break;
                case 3:
                    stage_score_bonus *= 15;
                    stage_score_bonus /= 10;
                    stage_score_bonus -= stage_score_bonus % 10;
                    break;
                case 4:
                    stage_score_bonus *= 2;
                    stage_score_bonus -= stage_score_bonus % 10;
                    break;
            }
            switch (SUPERVISOR.default_config.starting_lives) {
                case 3:
                    stage_score_bonus *= 5;
                    stage_score_bonus /= 10;
                    stage_score_bonus -= stage_score_bonus % 10;
                    break;
                case 4:
                    stage_score_bonus *= 2;
                    stage_score_bonus /= 10;
                    stage_score_bonus -= stage_score_bonus % 10;
                    break;
            }
            gui_impl->stage_score = stage_score_bonus;
            GAME_MANAGER.score += stage_score_bonus;
            ++gui_impl->__dword_2BDC;
        }
    }

};  // 0x2C

extern "C" {
    // 0x69BC30
    extern Gui GUI asm("_GUI");
}

// 0x4174D7
dllexport int32_t cdecl gui_on_tick(Gui *restrict gui_arg) asm("{[codecave:GUI_on_tick]}");
dllexport int32_t cdecl gui_on_tick(Gui *restrict gui_arg) {
    if (!GAME_MANAGER.__byte_2C) {
        gui_arg->on_tick_impl();
        gui_arg->impl->run_msg();
    }
    return 1;
}

typedef struct StdQuadBasic StdQuadBasic;
struct StdQuadBasic {
    int16_t type;  // 0x0
    int16_t offset_to_next;  // 0x2
    uint16_t anm_script;  // 0x4
    int16_t vm_index;  // 0x6
    Float3 position;  // 0x8
    Float2 size;  // 0x14
};  // 0x1c

typedef struct StdObject StdObject;
struct StdObject {
    int16_t id;  // 0x0
    uint8_t __unknown_always_zero;  // 0x2
    union {
        int8_t flags;  // 0x3
        struct {
            int8_t active : 1;
        };
    };
    Float3 posisiton;  // 0x4
    Float3 __size;  // 0x10
    StdQuadBasic first_quad;  // 0x1c
    unknown_fields(0x4);  // 0x38
};  // 0x3c

typedef struct StdInstruction StdInstruction;
struct StdInstruction {
    int32_t time;  // 0x0
    int16_t opcode;  // 0x4
    uint16_t args_size;  // 0x6
#ifndef UseStdArgSize
    int32_t args[3];  // 0x8
#else
    unsigned char args[];
#endif
};  // 0x58

typedef struct StdInstance StdInstance;
struct StdInstance {
    uint16_t object_id;  // 0x0
    uint16_t unknown;  // 0x2
    Float3 position;  // 0x4
};  // 0x10

typedef struct StdHeader StdHeader;
struct StdHeader {
    uint16_t num_objects;  // 0x0
    uint16_t num_quads;  // 0x2
    uint32_t offset_to_instances;  // 0x4
    uint32_t offset_to_script;  // 0x8
    uint32_t __dword_C;  // 0xc
    char stage_name[0x80];  // 0x10
    char bgm_names[4][0x80];  // 0x90
    char bgm_paths[4][0x80];  // 0x290
    StdObject *restrict objects[0x10];  // 0x490
};  // 0x4d0

typedef struct StageCameraSky StageCameraSky;
struct StageCameraSky {
    union {
        Float2 planes;
        vec<float, 2> planes_as_vec gnu_attr(packed);
        struct {
            union {
                float near_plane;  // 0x0
                DWORD near_plane_dword;
            };
            union {
                float far_plane;  // 0x4
                DWORD far_plane_dword;
            };
        };
    };
    union {
        D3DCOLOR color;  // 0x8
        uint8_t color_as_array[4];
        vec<uint8_t, 4> color_as_vec;
    };
};  // 0xc

typedef struct Stage Stage;
struct Stage {
    AnmVM *restrict quad_vms;  // 0x0
    StdHeader *restrict std_file;  // 0x4
    int32_t num_quads;  // 0x8
    int32_t num_objects;  // 0xc
    StdObject *restrict *restrict objects;  // 0x10
    StdInstance *restrict object_instances;  // 0x14
    StdInstruction *restrict beginning_of_script;  // 0x18
    Timer script_time;  // 0x1c
    union {
        int32_t instr_index;  // 0x28
        StdInstruction *restrict current_instr;
    };
    Timer __timer_2C;  // 0x2c
    unknown_fields(0x4);  // 0x38
    Float3 position;  // 0x3c
    StageCameraSky sky_fog;  // 0x48
    StageCameraSky sky_fog_interp_initial;  // 0x54
    StageCameraSky sky_fog_interp_final;  // 0x60
    int32_t sky_fog_interp_duration;  // 0x6c
    Timer sky_fog_interp_timer;  // 0x70
    unknown_fields(0x4);  // 0x7c
    int32_t __set_by_spellcard_ecl_80;  // 0x80
    int32_t __set_by_spellcard_ecl_84;  // 0x84
    AnmVM __vm_88;  // 0x88
    AnmVM __vm_198;  // 0x198
    uint8_t unpause_flag;  // 0x2a8
    unknown_fields(0x3);  // 0x2a9
    Float3 facing_dir_interp_initial;  // 0x2ac
    Float3 facing_dir_interp_final;  // 0x2b8
    int32_t facing_dir_interp_duration;  // 0x2c4
    Timer facing_dir_interp_timer;  // 0x2c8
    Float3 position_interp_final;  // 0x2d4
    int32_t position_interp_end_time;  // 0x2e0
    Float3 position_interp_initial;  // 0x2e4
    int32_t position_interp_start_time;  // 0x2f0

    // 0x404860
    dllexport int32_t thiscall tick_std_objects() asm("{[codecave:STAGE_tick_std_objects]}") {
        uint32_t object_count = this->num_objects;
        StdObject *restrict *restrict current_object_ptr = this->objects;
        for (size_t ii = 0; ii < object_count; ++ii, ++current_object_ptr) {
            StdObject *restrict current_object = *current_object_ptr;
            if (current_object->active) {
                bool has_visible_quads = false;
                for (
                    StdQuadBasic *restrict current_quad = &current_object->first_quad;
                    current_quad->type >= 0;
                    current_quad = (StdQuadBasic*)((uintptr_t)current_quad + current_quad->offset_to_next)
                ) {
                    AnmVM *restrict current_vm = &this->quad_vms[current_quad->vm_index];
                    switch (current_quad->type) {
                        case 1: {
                            StdQuadBasic *restrict lol_unused_variable = current_quad;
                        }
                        case 0:
                            ANM_MANAGER_PTR->run_anm(current_vm);
                            break;
                    }
                    has_visible_quads |= current_vm->current_instr != NULL;
                }
                if (!has_visible_quads) {
                    current_object->active = false;
                }
            }
        }
        return 0;
        
        /*uint32_t object_count = this->num_objects;
        StdObject *restrict current_object = (*this->objects)[0];
        for (size_t ii = 0; ii < object_count; ++ii, ++current_object) {
            if (current_object->active) {
                size_t processed_quads = 0;
                for (
                    StdQuadBasic *restrict current_quad = &current_object->first_quad;
                    current_quad->type >= 0;
                    current_quad = (StdQuadBasic*)((uintptr_t)current_quad + current_quad->offset_to_next)
                ) {
                    AnmVM *restrict current_vm = &this->quad_vms[current_quad->vm_index];
                    switch (current_quad->type) {
                        case 0:
                            ANM_MANAGER_PTR->run_anm(current_vm);
                            break;
                        case 1:
                            StdQuadBasic *restrict lol_unused_variable = current_quad;
                            ANM_MANAGER_PTR->run_anm(current_vm);
                            break;
                    }
                    if (current_vm->current_instr != NULL) {
                        ++processed_quads;
                    }
                }
                if (processed_quads == 0) {
                    current_object->active = false;
                }
            }
        }
        return 0;*/
    }

};  // 0x2f4

extern "C" {
    // 0x487B10
    extern Stage STAGE asm("_STAGE");
}

// 0x403810
dllexport int32_t cdecl stage_on_tick(Stage *const restrict stage_arg) asm("{[codecave:STAGE_on_tick]}");
dllexport int32_t cdecl stage_on_tick(Stage *const restrict stage_arg) {
    Stage *const restrict stage = stage_arg;
    if (expect(stage->std_file == NULL, false)) {
        return 1;
    }
    if (expect(GAME_MANAGER.__byte_2C, false)) {
        stage->__vm_88.color = PackD3DCOLOR(96, 128, 48, 48);
        return 1;
    }
#ifndef UseStdArgSize
    StdInstruction *restrict current_instruction = &stage->beginning_of_script[stage->instr_index];
    int32_t instruction_time = current_instruction->time;
    for (
        ;
        ;
        ++stage->instr_index, instruction_time = (++current_instruction)->time
    ) {
#else
    StdInstruction *restrict current_instruction = stage->current_instr;
    int32_t instruction_time = current_instruction->time;
    for (
        ;
        ;
        IndexInstr(current_instruction->args_size + sizeof(StdInstruction))
    ) {
#endif
        switch (expect_chance(current_instruction->opcode, 0, 0.75)) {
        //switch (expect_chance(current_instruction->opcode, 0, 0.4)) {
        //switch (expect_chance(current_instruction->opcode, 0, 0.2)) {
        //switch (current_instruction->opcode) {
            case 0:
                if (instruction_time == -1) {
                    stage->position = stage->position_interp_initial = Float3Arg(0);
                } else if (instruction_time < stage->script_time.current) {
                    stage->position = stage->position_interp_initial = Float3Arg(0);
                    stage->position_interp_start_time = instruction_time;
#ifndef UseStdArgSize
                    ++stage->instr_index;
                    while ((++current_instruction)->opcode != 0);
#else
                    stage->current_instr = IndexInstr(current_instruction->args_size + sizeof(StdInstruction));
                    for (
                        ;
                        current_instruction->opcode != 0;
                        IndexInstr(current_instruction->args_size + sizeof(StdInstruction))
                    );
#endif
                    stage->position_interp_end_time = current_instruction->time;
                    stage->position_interp_final = Float3Arg(0);
                }
                [[fallthrough]];
            default:
                goto BreakInsLoop;
            case 1:
                if (instruction_time < stage->script_time.current) {
                    D3DCOLOR color = UIntArg(0);
                    stage->sky_fog_interp_final.color = stage->sky_fog.color = color;
                    float near_plane = FloatArg(1);
                    stage->sky_fog_interp_final.near_plane = stage->sky_fog.near_plane = near_plane;
                    float far_plane = FloatArg(2);
                    stage->sky_fog_interp_final.far_plane = stage->sky_fog.far_plane = far_plane;
                    if (!stage->sky_fog_interp_duration) {
                        SUPERVISOR.set_fog_data(color, near_plane, far_plane);
                    }
                    continue;
                }
                goto BreakInsLoop;
            case 4:
                if (instruction_time < stage->script_time.current) {
                    stage->sky_fog_interp_initial = stage->sky_fog;
                    stage->sky_fog_interp_duration = IntArg(0);
                    //stage->sky_fog_interp_timer = static_default_timer<0>;
                    stage->sky_fog_interp_timer.set_default();
                    continue;
                }
                goto BreakInsLoop;
            case 2:
                if (instruction_time < stage->script_time.current) {
                    stage->facing_dir_interp_initial = stage->facing_dir_interp_final;
                    stage->facing_dir_interp_final = Float3Arg(0);
                    continue;
                }
                goto BreakInsLoop;
            case 3:
                if (instruction_time < stage->script_time.current) {
                    stage->facing_dir_interp_duration = IntArg(0);
                    //stage->facing_dir_interp_timer = static_default_timer<0>;
                    stage->facing_dir_interp_timer.set_default();
                    continue;
                }
                goto BreakInsLoop;
            case 5:
                if (stage->unpause_flag) {
                    stage->unpause_flag = false;
                    continue;
                }
                goto BreakInsLoop;
        }
    }
BreakInsLoop:
    if (instruction_time != -1) {
        float interp = ((float)stage->script_time.current + stage->script_time.subframe) - (float)stage->position_interp_start_time;
        interp /= (float)(stage->position_interp_end_time - stage->position_interp_start_time);
        //stage->position.x = stage->position_interp_initial.x + interp * (stage->position_interp_final.x - stage->position_interp_initial.x);
        //stage->position.y = stage->position_interp_initial.y + interp * (stage->position_interp_final.y - stage->position_interp_initial.y);
        //stage->position.z = stage->position_interp_initial.z + interp * (stage->position_interp_final.z - stage->position_interp_initial.z);
        
        unaligned vec<float, 4>& position = *(unaligned vec<float, 4>*)&stage->position;
        vec<float, 4> position_interp_final = *(unaligned vec<float, 4>*)&stage->position_interp_final;
        vec<float, 4> position_interp_initial = *(unaligned vec<float, 4>*)&stage->position_interp_initial;
        position_interp_initial += (position_interp_final - position_interp_initial) * interp;
        position[0] = position_interp_initial[0];
        position[1] = position_interp_initial[1];
        position[2] = position_interp_initial[2];
        
    }
    if (int32_t facing_dir_duration = stage->facing_dir_interp_duration) {
        if (stage->facing_dir_interp_timer.current < facing_dir_duration) {
            stage->facing_dir_interp_timer.tick();
        } else {
            stage->facing_dir_interp_timer = (Timer){ -999, 0.0f, facing_dir_duration };
        }
        //float temp_z = stage->facing_dir_interp_final.z - stage->facing_dir_interp_initial.z;
        //float temp_y = stage->facing_dir_interp_final.y - stage->facing_dir_interp_initial.y;
        //float temp_x = stage->facing_dir_interp_final.x - stage->facing_dir_interp_initial.x;
        float interp = ((float)stage->facing_dir_interp_timer.current + stage->facing_dir_interp_timer.subframe) / (float)stage->facing_dir_interp_duration;
        //GAME_MANAGER.stage_camera_facing_dir.x = stage->facing_dir_interp_initial.x + temp_x * interp;
        //GAME_MANAGER.stage_camera_facing_dir.y = stage->facing_dir_interp_initial.y + temp_y * interp;
        //GAME_MANAGER.stage_camera_facing_dir.z = stage->facing_dir_interp_initial.z + temp_z * interp;

        unaligned vec<float, 4>& facing_dir = *(unaligned vec<float, 4>*)&GAME_MANAGER.stage_camera_facing_dir;
        vec<float, 4> facing_dir_interp_final = *(unaligned vec<float, 4>*)&stage->facing_dir_interp_final;
        vec<float, 4> facing_dir_interp_initial = *(unaligned vec<float, 4>*)&stage->facing_dir_interp_initial;
        facing_dir_interp_initial += (facing_dir_interp_final - facing_dir_interp_initial) * interp;
        facing_dir[0] = facing_dir_interp_initial[0];
        facing_dir[1] = facing_dir_interp_initial[1];
        facing_dir[2] = facing_dir_interp_initial[2];
    }
    if (int32_t fog_duration = stage->sky_fog_interp_duration) {
        stage->sky_fog_interp_timer.tick();
        if (stage->sky_fog_interp_timer.current >= fog_duration) {
            stage->sky_fog_interp_duration = 0;
        }
        float interp = ((float)stage->sky_fog_interp_timer.current + stage->sky_fog_interp_timer.subframe) / (float)fog_duration;
        interp = __min(interp, 1.0f);
        
        /*for (size_t ii = 0; ii < 4; ++ii) {
            float final_color = stage->sky_fog_interp_final.color_as_array[ii];
            float initial_color = stage->sky_fog_interp_initial.color_as_array[ii];
            stage->sky_fog.color_as_array[ii] = (float)stage->sky_fog_interp_initial.color_as_array[ii] + (final_color - initial_color) * interp;
        }*/

        vec<float, 4> color_interp_final = convertvec(stage->sky_fog_interp_final.color_as_vec, vec<float, 4>);
        vec<float, 4> color_interp_initial = convertvec(stage->sky_fog_interp_initial.color_as_vec, vec<float, 4>);
        D3DCOLOR color = bitcast<D3DCOLOR>(convertvec(color_interp_initial + (color_interp_final - color_interp_initial) * interp, vec<uint8_t, 4>));
        stage->sky_fog.color = color;

        //stage->sky_fog.near_plane = stage->sky_fog_interp_initial.near_plane + (stage->sky_fog_interp_final.near_plane - stage->sky_fog_interp_initial.near_plane) * interp;
        //stage->sky_fog.far_plane = stage->sky_fog_interp_initial.far_plane + (stage->sky_fog_interp_final.far_plane - stage->sky_fog_interp_initial.far_plane) * interp;

        vec<float, 2> planes_interp_final = stage->sky_fog_interp_final.planes_as_vec;
        vec<float, 2> planes_interp_initial = stage->sky_fog_interp_initial.planes_as_vec;
        planes_interp_initial += (planes_interp_final - planes_interp_initial) * interp;
        stage->sky_fog.planes_as_vec = planes_interp_initial;

        //SUPERVISOR.set_fog_data(stage->sky_fog.color, stage->sky_fog.near_plane, stage->sky_fog.far_plane);
        SUPERVISOR.set_fog_data(color, planes_interp_initial[0], planes_interp_initial[1]);
    }
    if (current_instruction->opcode != 5) {
        stage->script_time.tick();
    }
    stage->tick_std_objects();
    if (stage->__set_by_spellcard_ecl_80 >= 1) {
        if (stage->__set_by_spellcard_ecl_84 == 60) {
            stage->__set_by_spellcard_ecl_80++;
        }
        stage->__set_by_spellcard_ecl_84++;
        ANM_MANAGER_PTR->run_anm(&stage->__vm_88);
    }
    return 1;
}

//// 0x403810
//dllexport int32_t cdecl stage_on_tick(Stage* restrict stage_arg) asm("{[codecave:STAGE_on_tick]}");
//dllexport int32_t cdecl stage_on_tick(Stage* restrict stage_arg) {
//    if (expect(stage_arg == NULL, false)) {
//        return 1;
//    }
//    if (expect(GAME_MANAGER.__byte_2C, false)) {
//        stage_arg->__vm_88.color = PackD3DCOLOR(96, 128, 48, 48);
//        return 1;
//    }
//    StdInstruction *restrict current_instruction = &(*stage_arg->beginning_of_script)[stage_arg->instr_index];
//    for (
//        ;
//        ;
//    ) {
//        switch (current_instruction->opcode) {
//            default:
//                goto BreakInsLoop;
//            case 0:
//                if (expect(current_instruction->time == -1, false)) {
//                    stage_arg->position = stage_arg->position_interp_initial = Float3Arg(0);
//                } else if (current_instruction->time < stage_arg->script_time.current) {
//                    stage_arg->position = stage_arg->position_interp_initial = Float3Arg(0);
//                    stage_arg->position_interp_start_time = current_instruction->time;
//                    ++stage_arg->instr_index;
//                    while (++current_instruction->opcode != 0);
//                    stage_arg->position_interp_end_time = current_instruction->time;
//                    stage_arg->position_interp_final = Float3Arg(0);
//                }
//                goto BreakInsLoop;
//            case 1:
//                if (current_instruction->time < stage_arg->script_time.current) {
//                    uint32_t color = UIntArg(0);
//                    stage_arg->sky_fog.color = color;
//                    float near_plane = FloatArg(1);
//                    stage_arg->sky_fog.near_plane = near_plane;
//                    float far_plane = FloatArg(2);
//                    stage_arg->sky_fog.far_plane = far_plane;
//                    if (stage_arg->sky_fog_interp_duration) {
//                        SUPERVISOR.set_fog_data(color, near_plane, far_plane);
//                        //IDirect3DDevice8 *restrict d3d_device = SUPERVISOR.d3d_device;
//                        //d3d_device->SetRenderState(D3DRS_FOGCOLOR, color);
//                        //d3d_device->SetRenderState(D3DRS_FOGSTART, bitcast(DWORD, near_plane));
//                        //d3d_device->SetRenderState(D3DRS_FOGEND, bitcast(DWORD, far_plane));
//                    }
//                    ++stage_arg->instr_index;
//                    stage_arg->sky_fog_interp_final = stage_arg->sky_fog;
//                    continue;
//                }
//                goto BreakInsLoop;
//            case 4:
//                if (current_instruction->time < stage_arg->script_time.current) {
//                    stage_arg->sky_fog_interp_initial = stage_arg->sky_fog;
//                    stage_arg->sky_fog_interp_duration = IntArg(0);
//                    stage_arg->sky_fog_interp_timer = static_default_timer<0>;
//                    ++stage_arg->instr_index;
//                    continue;
//                }
//                goto BreakInsLoop;
//            case 2:
//                if (current_instruction->time < stage_arg->script_time.current) {
//                    stage_arg->facing_dir_interp_initial = stage_arg->facing_dir_interp_final;
//                    stage_arg->facing_dir_interp_final = Float3Arg(0);
//                    ++stage_arg->instr_index;
//                    continue;
//                }
//                goto BreakInsLoop;
//            case 3:
//                if (current_instruction->time < stage_arg->script_time.current) {
//                    stage_arg->facing_dir_interp_duration = IntArg(0);
//                    stage_arg->facing_dir_interp_timer = static_default_timer<0>;
//                    ++stage_arg->instr_index;
//                    continue;
//                }
//                goto BreakInsLoop;
//            case 5:
//                if (!stage_arg->unpause_flag) {
//                    stage_arg->unpause_flag = false;
//                    ++stage_arg->instr_index;
//                    continue;
//                }
//                goto BreakInsLoop;
//        }
//    }
//BreakInsLoop:
//    if (current_instruction->time != -1) {
//        float interp = ((float)stage_arg->script_time.current + stage_arg->script_time.subframe) - (float)stage_arg->position_interp_start_time;
//        interp /= (float)(stage_arg->position_interp_end_time - stage_arg->position_interp_start_time);
//        stage_arg->position.x = stage_arg->position_interp_initial.x + interp * (stage_arg->position_interp_final.x - stage_arg->position_interp_initial.x);
//        stage_arg->position.y = stage_arg->position_interp_initial.y + interp * (stage_arg->position_interp_final.y - stage_arg->position_interp_initial.y);
//        stage_arg->position.z = stage_arg->position_interp_initial.z + interp * (stage_arg->position_interp_final.z - stage_arg->position_interp_initial.z);
//    }
//    if (stage_arg->facing_dir_interp_duration) {
//        if (stage_arg->facing_dir_interp_timer.current < stage_arg->facing_dir_interp_duration) {
//            stage_arg->facing_dir_interp_timer.tick();
//        } else {
//            stage_arg->facing_dir_interp_timer = (Timer){ -999, 0.0f, stage_arg->facing_dir_interp_duration };
//        }
//        float temp_z = stage_arg->facing_dir_interp_final.z - stage_arg->facing_dir_interp_initial.z;
//        float temp_y = stage_arg->facing_dir_interp_final.y - stage_arg->facing_dir_interp_initial.y;
//        float temp_x = stage_arg->facing_dir_interp_final.x - stage_arg->facing_dir_interp_initial.x;
//        float interp = ((float)stage_arg->facing_dir_interp_timer.current + stage_arg->facing_dir_interp_timer.subframe) / (float)stage_arg->facing_dir_interp_duration;
//        GAME_MANAGER.__float3_1A60.x = stage_arg->facing_dir_interp_initial.x + temp_x * interp;
//        GAME_MANAGER.__float3_1A60.y = stage_arg->facing_dir_interp_initial.y + temp_y * interp;
//        GAME_MANAGER.__float3_1A60.z = stage_arg->facing_dir_interp_initial.z + temp_z * interp;
//    }
//    if (stage_arg->sky_fog_interp_duration) {
//        stage_arg->sky_fog_interp_timer.tick();
//        float interp = ((float)stage_arg->sky_fog_interp_timer.current + stage_arg->sky_fog_interp_timer.subframe) / (float)stage_arg->sky_fog_interp_duration;
//        interp = __min(interp, 1.0f);
//        for (size_t ii = 0; ii < 4; ++ii) {
//            float final_color = stage_arg->sky_fog_interp_final.color_as_array[ii];
//            float initial_color = stage_arg->sky_fog_interp_initial.color_as_array[ii];
//            stage_arg->sky_fog.color_as_array[ii] = (float)stage_arg->sky_fog_interp_initial.color_as_array[ii] + (final_color - initial_color) * interp;
//        }
//        stage_arg->sky_fog.near_plane = stage_arg->sky_fog_interp_initial.near_plane + (stage_arg->sky_fog_interp_final.near_plane - stage_arg->sky_fog_interp_initial.near_plane) * interp;
//        stage_arg->sky_fog.far_plane = stage_arg->sky_fog_interp_initial.far_plane + (stage_arg->sky_fog_interp_final.far_plane - stage_arg->sky_fog_interp_initial.far_plane) * interp;
//        SUPERVISOR.set_fog_data(stage_arg->sky_fog.color, stage_arg->sky_fog.near_plane, stage_arg->sky_fog.far_plane);
//        if (stage_arg->sky_fog_interp_timer.current >= stage_arg->sky_fog_interp_duration) {
//            stage_arg->sky_fog_interp_duration = 0;
//        }
//    }
//    if (current_instruction->opcode != 5) {
//        stage_arg->script_time.tick();
//    }
//    stage_arg->tick_std_objects();
//    if (stage_arg->__set_by_spellcard_ecl_80 >= 1) {
//        if (stage_arg->__set_by_spellcard_ecl_84 == 60) {
//            ++stage_arg->__set_by_spellcard_ecl_80;
//        }
//        ++stage_arg->__set_by_spellcard_ecl_84;
//        ANM_MANAGER_PTR->run_anm(&stage_arg->__vm_88);
//    }
//    return 1;
//}

enum BulletType {
    Pellet = 0,
    RingBall = 1,
    Rice = 2,
    Ball = 3,
    Kunai = 4,
    Shard = 5,
    BigBall = 6,
    Fireball = 7,
    Dagger = 8,
    Bubble = 9
};

static inline constexpr int32_t etama3_scripts_base_index = 512;
static inline constexpr int32_t etama4_scripts_base_index = 666;

// size: 0x560
struct BulletTemplateData {
    AnmVM vm; // 0x0
    AnmVM spawn_effect_short_vm; // 0x110
    AnmVM spawn_effect_medium_vm; // 0x220
    AnmVM spawn_effect_long_vm; // 0x330
    AnmVM despawn_effect_vm; // 0x440
    Float3 hitbox_size; // 0x550
    uint8_t bullet_width_pixels; // 0x55C
    uint8_t bullet_height_pixels; // 0x55D
    unknown_fields(0x2);  // 0x55E
    // 0x560
};  

typedef struct Bullet Bullet;
struct Bullet {
    BulletTemplateData data;  // 0x0
    Float3 position;  // 0x560
    Float3 velocity;  // 0x56C
    Float3 acceleration_vec;  // 0x578
    float speed;  // 0x584
    float velocity_scalar;  // 0x588
    float ex_angle_speed;  // 0x58C
    float angle;  // 0x590
    float angular_velocity;  // 0x594
    float ex_angle_offset;  // 0x598
    Timer timer;  // 0x59C
    int32_t acceleration_duration;  // 0x5A8
    int32_t ex_angle_interval;  // 0x5AC
    int32_t ex_angle_count;  // 0x5B0
    int32_t ex_angle_max_count;  // 0x5B4
    union {
        uint16_t flags;  // 0x5B8
        struct {
            uint16_t __unknown_flag_A : 1; // 1
            uint16_t has_spawn_effect_short : 1; // 2
            uint16_t has_spawn_effect_medium : 1; // 3
            uint16_t has_spawn_effect_long : 1; // 4
            uint16_t __unknown_flag_B : 1; // 5
            uint16_t __unknown_flag_C : 1; // 6
            uint16_t __unknown_flag_D : 1; // 7
        };
    };
    uint16_t color;  // 0x5BA
    unknown_fields(0x2);  // 0x5BC
    uint16_t state;  // 0x5BE
    uint16_t __field_5c0;  // 0x5C0
    uint8_t __field_5c2;  // 0x5C2
    uint8_t __field_5c3;  // 0x5C3
};  // 0x5C4

typedef struct BulletShooter BulletShooter;
struct BulletShooter {
    uint16_t sprite;  // 0x0
    uint16_t color;  // 0x2
    Float3 position;  // 0x4
    union {
        struct {
            float angle1;  // 0x10
            float angle2;  // 0x14
            float speed1;  // 0x18
            float speed2;  // 0x1c
        };
    };
    float float_data[0x4];  // 0x20
    int32_t int_data[0x4];  // 0x30
    unknown_fields(0x4);  // 0x40
    int16_t count_1;  // 0x44
    int16_t count_2;  // 0x46
    uint16_t aim_mode;  // 0x48
    short __word_4A;  // 0x4a
    union {
        uint32_t flags;  // 0x4c
        struct {
            unknown_bitfields(uint32_t, 0x1);
            uint32_t play_sound : 1;
            unknown_bitfields(uint32_t, 0x15);
            uint32_t play_sound_on_spawn : 1;
        };
    };
    int32_t sfx;  // 0x50
};  // 0x54

typedef struct LaserShooter LaserShooter;
struct LaserShooter {
    uint16_t sprite;  // 0x0
    uint16_t color; // 0x2
    Float3 position;  // 0x4
    float angle;  // 0x10
    unknown_fields(0x4);  // 0x14
    float speed;  // 0x18
    unknown_fields(0x4);  // 0x1c
    float start_offset;  // 0x20
    float end_offset;  // 0x24
    float start_length;  // 0x28
    float width;  // 0x2c
    int32_t start_time;  // 0x30
    int32_t duration;  // 0x34
    int32_t stop_time;  // 0x38
    int32_t graze_delay;  // 0x3c
    int32_t graze_distance;  // 0x40
    unknown_fields(0x4);  // 0x44
    uint16_t type;  // 0x48
    unknown_fields(0x2);  // 0x4a
    union {
        uint32_t flags_dword;  // 0x4c
        uint16_t flags;  // 0x4c
    };
    unknown_fields(0x4);  // 0x50
};  // 0x54

struct GenericShooter {
    uint16_t sprite;  // 0x0
    uint16_t color;  // 0x2
    Float3 position;  // 0x4
    float angle1;  // 0x10
    float angle2;  // 0x14
    float speed1;  // 0x18
    float speed2;  // 0x1c
    float start_offset;  // 0x20
    float end_offset;  // 0x24
    float start_length;  // 0x28
    float width;  // 0x2c
    int32_t start_time;  // 0x30
    int32_t duration;  // 0x34
    int32_t stop_time;  // 0x38
    int32_t graze_delay;  // 0x3c
    int32_t graze_distance;  // 0x40
    int16_t count_1;  // 0x44
    int16_t count_2;  // 0x46
    uint16_t aim_mode;  // 0x48
    short __word_4A;  // 0x4a
    uint32_t flags;  // 0x4c
    int32_t sfx;  // 0x50
};

typedef struct Laser Laser;
struct Laser {
    AnmVM vm_0;  // 0x0
    AnmVM vm_1;  // 0x110
    Float3 position;  // 0x220
    float angle;  // 0x22C
    float start_offset;  // 0x230
    float end_offset;  // 0x234
    float start_length;  // 0x238
    float width;  // 0x23c
    float speed;  // 0x240
    int32_t start_time;  // 0x244
    int32_t graze_delay;  // 0x248
    int32_t duration;  // 0x24c
    int32_t end_time;  // 0x250
    int32_t graze_interval;  // 0x254
    int32_t in_use;  // 0x258
    Timer timer;  // 0x25c
    union {
        uint16_t flags; // 0x268
        struct {
            uint16_t fade_appear_style : 1;
        };
    };
    uint16_t color;  // 0x26a
    uint8_t state;  // 0x26c
    probably_padding_bytes(0x3);  // 0x26d
};  // 0x270

// size: 0x14
struct BulletTypeAnmData {
    int32_t main_script; // 0x0
    int32_t spawn_effect_short_script; // 0x4
    int32_t spawn_effect_medium_script; // 0x8
    int32_t spawn_effect_long_script; // 0xC
    int32_t despawn_effect_script; // 0x10
    // 0x14
};

extern "C" {
    // 0x46A6E8
    extern BulletTypeAnmData BULLET_TYPE_ANM_DATA[10] asm("_BULLET_TYPE_ANM_DATA");
}

typedef struct BulletManager BulletManager;
// size: 0xF5C18
struct BulletManager {
    BulletTemplateData bullet_templates[16]; // 0x0
    Bullet bullets[640]; // 0x5600
    Laser lasers[64]; // 0xEC000
    int32_t next_bullet_index; // 0xF5C00
    int32_t bullet_count; // 0xF5C04
    Timer time; // 0xF5C08
    char* etama_anm_filename; // 0xF5C14
    // 0xF5C18

    // 0x4135B0
    dllexport int32_t thiscall shoot_one_bullet(const BulletShooter *restrict bullet_data, int32_t count_1, int32_t count_2, float angle_to_player) {
        Bullet *restrict new_bullet = &this->bullets[this->next_bullet_index];
        Bullet *const starting_bullet = new_bullet;
        do {
            if (!new_bullet->state) break;
            ++new_bullet;
            if (new_bullet == array_end_addr(this->bullets)) {
                new_bullet = this->bullets;
            }
        } while (new_bullet != starting_bullet);
        if (Bullet *restrict next_bullet = new_bullet + 1;
            expect(next_bullet != array_end_addr(this->bullets), true)
            ) {
            this->next_bullet_index = next_bullet - this->bullets;
        } else {
            return 1;
        }
        //float speed = bullet_data->speed1 - bullet_data->speed2;
        return 0;
    }

    // 0x4145C0
    dllexport int32_t thiscall shoot_bullets(const BulletShooter *restrict bullet_data) asm("{[codecave:BULLET_MANAGER_shoot_bullets]}") {
        float angle_to_player = PLAYER.angle_to_player_from_point(&bullet_data->position);
        for (int32_t ii = 0; ii < bullet_data->count_2; ++ii) {
            for (int32_t jj = 0; jj < bullet_data->count_1; ++jj) {
                if (this->shoot_one_bullet(bullet_data, jj, ii, angle_to_player)) break;
            }
        }
        if (bullet_data->play_sound_on_spawn) {
            SOUND_MANAGER.play_sound_centered(bullet_data->sfx);
        }
        return 0;
    }

    //// 0x414160
    //dllexport void thiscall cancel_bullets_original(bool spawn_star_items) asm("{[codecave:BULLET_MANAGER_cancel_bullets_original]}") {
    //    Bullet *restrict current_bullet = this->bullets;
    //    for (; current_bullet != &this->bullets[countof(this->bullets)]; ++current_bullet) {
    //        switch (current_bullet->state) {
    //            case 0: case 5:
    //                continue;
    //        }
    //        if (spawn_star_items) {
    //            ITEM_MANAGER.spawn_item(&current_bullet->position, 6, 1);
    //            memset(current_bullet, 0, sizeof(Bullet));
    //        } else {
    //            current_bullet->state = 5;
    //        }
    //    }
    //    Laser *restrict current_laser = reinterpret_cast<Laser*>(current_bullet);
    //    for (; current_laser != &this->lasers[countof(this->lasers)]; ++current_laser) {
    //        if (!current_laser->in_use) {
    //            continue;
    //        }
    //        if (current_laser->state < 2) {
    //            current_laser->state = 2;
    //            current_laser->timer = static_default_timer<0>;
    //            if (spawn_star_items) {
    //                float laser_angle = current_laser->angle;
    //                vec<float, 2> angles = { zcosf(laser_angle), zsinf(laser_angle) };
    //                vec<float, 2> laser_position = *(unaligned vec<float, 2>*)&current_laser->position;
    //                Float3 item_position;
    //                item_position.z = 0.0f;
    //                for (
    //                    float current_offset = current_laser->start_offset;
    //                    current_laser->end_offset > current_offset;
    //                    current_offset += 32.0f
    //                ) {
    //                    *(unaligned vec<float, 2>*)&item_position = angles * current_offset + laser_position;
    //                    ITEM_MANAGER.spawn_item(&item_position, 6, 1);
    //                }
    //            }
    //        }
    //        current_laser->graze_interval = 0;
    //    }
    //}

    // 0x414160
    dllexport forceinline void thiscall cancel_bullets() asm("{[codecave:BULLET_MANAGER_cancel_bullets]}") {
        Bullet *restrict current_bullet = this->bullets;
        for (; current_bullet != array_end_addr(this->bullets); ++current_bullet) {
            switch (current_bullet->state) {
                case 0: case 5:
                    continue;
            }
            current_bullet->state = 5;
        }
        Laser *restrict current_laser = reinterpret_cast<Laser*>(current_bullet);
        for (; current_laser != array_end_addr(this->lasers); ++current_laser) {
            if (!current_laser->in_use) {
                continue;
            }
            if (current_laser->state < 2) {
                current_laser->state = 2;
                //current_laser->timer = static_default_timer<0>;
                current_laser->timer.set_default();
            }
            current_laser->graze_interval = 0;
        }
    }

    // 0x414340
    dllexport void thiscall cancel_bullets_to_star_items(void) asm("{[codecave:BULLET_MANAGER_cancel_bullets_to_star_items]}") {
        for (Bullet *restrict current_bullet = this->bullets; current_bullet != array_end_addr(this->bullets); ++current_bullet) {
            switch (current_bullet->state) {
                case 0: case 5:
                    continue;
            }
            ITEM_MANAGER.spawn_item(&current_bullet->position, StarItem, 1);
            __builtin_memset(current_bullet, 0, sizeof(Bullet));
        }
        for (Laser *restrict current_laser = this->lasers; current_laser != array_end_addr(this->lasers); ++current_laser) {
            if (!current_laser->in_use) {
                continue;
            }
            if (current_laser->state < 2) {
                current_laser->state = 2;
                //current_laser->timer = static_default_timer<0>;
                current_laser->timer.set_default();

                /*float laser_angle = current_laser->angle;
                float x_add = zcosf(laser_angle);
                float y_add = zsinf(laser_angle);
                Float3 item_position;
                for (
                    float current_offset = current_laser->start_offset;
                    current_laser->end_offset > current_offset;
                    current_offset += 32.0f
                ) {
                    item_position = {
                        .x = x_add * current_offset + current_laser->position.x,
                        .y = y_add * current_offset + current_laser->position.y,
                        .z = 0.0f
                    };
                    ITEM_MANAGER.spawn_item(&item_position, 6, 1);
                }*/

                float current_offset = current_laser->start_offset;

                float laser_angle = current_laser->angle;
#ifndef SqueezeStack
                float x_add = zcosf(laser_angle);
                float y_add = zsinf(laser_angle);
#else
                //esp_reg -= sizeof(double);
                __asm__ volatile (
                    "SUB $8, %%ESP":
                );
                *(double*)esp_reg = laser_angle;
                __asm__ volatile (
                    "fstps (%%ESP)"
                    :
                    : "X"(cosf_impl_manual(), NULL)
                    : clobber_list("eax", "ecx", "edx")
                );
                float x_add = *(volatile float*)esp_reg;
                *(double*)esp_reg = laser_angle;
                __asm__ volatile (
                    "fstps (%%ESP)"
                    :
                    : "X"(sinf_impl_manual(), NULL)
                    : clobber_list("eax", "ecx", "edx")
                );
                float y_add = *(volatile float*)esp_reg;
                __asm__ volatile (
                    "ADD $8, %%ESP":
                );
                //esp_reg += sizeof(double);
#endif
                Float3 item_position;
                item_position.x = current_laser->position.x + x_add * current_offset;
                item_position.y = current_laser->position.y + y_add * current_offset;
                item_position.z = 0.0f;
                x_add *= 32.0f;
                y_add *= 32.0f;
                for (
                    ;
                    current_laser->end_offset > current_offset;
                    current_offset += 32.0f
                    ) {
                    ITEM_MANAGER.spawn_item(&item_position, StarItem, 1);
                    item_position.x += x_add;
                    item_position.y += y_add;
                }

                /*for (
                    float current_offset = current_laser->start_offset;
                    current_laser->end_offset > current_offset;
                    current_offset += 32.0f
                ) {
                    *(unaligned vec<float, 2>*)&item_position = angles * current_offset + laser_position;
                    ITEM_MANAGER.spawn_item(&item_position, 6, 1);
                }*/
            }
            current_laser->graze_interval = 0;
        }
    }

    // 0x414670
    dllexport gnu_noinline Laser* thiscall shoot_lasers(LaserShooter *restrict laser_data) asm("{[0x414670]}") {
        gnu_used static volatile auto tempA = this;
        gnu_used static volatile auto tempB = laser_data;
        assume_all_registers_volatile();
        return &this->lasers[0];
    }

    // 0x414360
    dllexport int32_t thiscall cancel_bullets_spell_bonus(int32_t score_threshold) asm("{[codecave:BULLET_MANAGER_cancel_bullets_spell_bonus]}") {
        int32_t value = 2000;
        int32_t score_bonus = 0;
        for (
            Bullet *restrict current_bullet = this->bullets;
            current_bullet != array_end_addr(this->bullets);
            ++current_bullet
            ) {
            if (!current_bullet->state) continue;
            ASCII_MANAGER.make_popup_A(
                &current_bullet->position,
                value,
                (value < score_threshold) ? PackD3DCOLOR(0, 0, 0, 0) : PackD3DCOLOR(255, 255, 255, 255)
            );
            score_bonus += value;
            value += 10;
            if (value > score_threshold) {
                value = score_threshold;
            }
            current_bullet->state = 5;
        }
        for (
            Laser *restrict current_laser = this->lasers;
            current_laser != array_end_addr(this->lasers);
            ++current_laser
            ) {
            if (!current_laser->in_use) continue;
            if (current_laser->state < 2) {
                current_laser->state = 2;
                //current_laser->timer = static_default_timer<0>;
                current_laser->timer.set_default();
            }
            current_laser->graze_interval = 0;
        }
        if (score_bonus) {
            GAME_MANAGER.score += score_bonus;
            GUI.__sub_41735A(score_bonus);
        }
        return score_bonus;
    }

    // 0x414360
    dllexport int32_t thiscall cancel_bullets_to_star_items_spell_bonus(int32_t score_threshold) asm("{[codecave:BULLET_MANAGER_cancel_bullets_to_star_items_spell_bonus]}") {
        int32_t value = 2000;
        int32_t score_bonus = 0;
        for (
            Bullet *restrict current_bullet = this->bullets;
            current_bullet != array_end_addr(this->bullets);
            ++current_bullet
            ) {
            if (!current_bullet->state) continue;
            ITEM_MANAGER.spawn_item(&current_bullet->position, StarItem, 1);
            ASCII_MANAGER.make_popup_A(
                &current_bullet->position,
                value,
                (value < score_threshold) ? PackD3DCOLOR(0, 0, 0, 0) : PackD3DCOLOR(255, 255, 255, 255)
            );
            score_bonus += value;
            value += 10;
            if (value > score_threshold) {
                value = score_threshold;
            }
            current_bullet->state = 5;
        }
        for (
            Laser *restrict current_laser = this->lasers;
            current_laser != array_end_addr(this->lasers);
            ++current_laser
            ) {
            if (!current_laser->in_use) continue;
            if (current_laser->state < 2) {
                current_laser->state = 2;
                //current_laser->timer = static_default_timer<0>;
                current_laser->timer.set_default();
            }
            float current_offset = current_laser->start_offset;

            float laser_angle = current_laser->angle;
            float x_add = zcosf(laser_angle);
            float y_add = zsinf(laser_angle);
            Float3 item_position;
            item_position.x = current_laser->position.x + x_add * current_offset;
            item_position.y = current_laser->position.y + y_add * current_offset;
            item_position.z = 0.0f;
            x_add *= 32.0f;
            y_add *= 32.0f;
            for (
                ;
                current_laser->end_offset > current_offset;
                current_offset += 32.0f
                ) {
                ITEM_MANAGER.spawn_item(&item_position, StarItem, 1);
                item_position.x += x_add;
                item_position.y += y_add;
            }
            current_laser->graze_interval = 0;
        }
        if (score_bonus) {
            GAME_MANAGER.score += score_bonus;
            GUI.__sub_41735A(score_bonus);
        }
        return score_bonus;
    }

    //// 0x414360
    //dllexport int32_t thiscall cancel_bullets_spell_bonus(int32_t score_threshold, bool spawn_star_items) asm("{[codecave:BULLET_MANAGER_cancel_related_2]}") {
    //    int32_t value = 2000;
    //    int32_t score_bonus = 0;
    //    int32_t cancel_count = 0;
    //    for (
    //        Bullet *restrict current_bullet = this->bullets;
    //        current_bullet != array_end_addr(this->bullets);
    //        ++current_bullet
    //    ) {
    //        if (!current_bullet->state) continue;
    //        if (spawn_star_items) {
    //            ITEM_MANAGER.spawn_item(&current_bullet->position, StarItem, 1);
    //        }
    //        ASCII_MANAGER.make_popup_A(
    //            &current_bullet->position,
    //            value,
    //            (value < score_threshold) ? PackD3DCOLOR(0, 0, 0, 0) : PackD3DCOLOR(255, 255, 255, 255)
    //        );
    //        score_bonus += value;
    //        ++cancel_count;
    //        value += 10;
    //        if (value > score_threshold) {
    //            value = score_threshold;
    //        }
    //        current_bullet->state = 5;
    //    }
    //    for (
    //        Laser *restrict current_laser = this->lasers;
    //        current_laser != array_end_addr(this->lasers);
    //        ++current_laser
    //    ) {
    //        if (!current_laser->in_use) continue;
    //        if (current_laser->state < 2) {
    //            current_laser->state = 2;
    //            current_laser->timer = static_default_timer<0>;
    //        }
    //        if (spawn_star_items) {
    //            float current_offset = current_laser->start_offset;
    //
    //            float laser_angle = current_laser->angle;
    //            float x_add = zcosf(laser_angle);
    //            float y_add = zsinf(laser_angle);
    //            Float3 item_position;
    //            item_position.x = current_laser->position.x + x_add * current_offset;
    //            item_position.y = current_laser->position.y + y_add * current_offset;
    //            item_position.z = 0.0f;
    //            x_add *= 32.0f;
    //            y_add *= 32.0f;
    //            for (
    //                ;
    //                current_laser->end_offset > current_offset;
    //                current_offset += 32.0f
    //            ) {
    //                ITEM_MANAGER.spawn_item(&item_position, StarItem, 1);
    //                item_position.x += x_add;
    //                item_position.y += y_add;
    //            }
    //        }
    //        current_laser->graze_interval = 0;
    //    }
    //    GAME_MANAGER.score += score_bonus;
    //    if (score_bonus) {
    //        GUI.__sub_41735A(score_bonus);
    //    }
    //}


    // 0x416D60
    dllexport static gnu_noinline ZunResult cdecl bullet_manager_on_registration(BulletManager* self) asm_symbol_rel(codecave:BULLET_MANAGER_on_registration) {
        if ((bool)(SUPERVISOR.__dword_18C != 3)) {
            if (ZUN_FAILED(ANM_MANAGER_PTR->load_anm_file(6, "data/etama3.anm", etama3_scripts_base_index))) {
                return ZUN_ERROR;
            }
            if (ZUN_FAILED(ANM_MANAGER_PTR->load_anm_file(7, "data/etama4.anm", etama4_scripts_base_index))) {
                return ZUN_ERROR;
            }
        }
        for (int32_t i = 0; i < countof(BULLET_TYPE_ANM_DATA); i++) {
            self->bullet_templates[i].vm.set_script(BULLET_TYPE_ANM_DATA[i].main_script);
            self->bullet_templates[i].spawn_effect_short_vm.set_script(BULLET_TYPE_ANM_DATA[i].spawn_effect_short_script);
            self->bullet_templates[i].spawn_effect_medium_vm.set_script(BULLET_TYPE_ANM_DATA[i].spawn_effect_medium_script);
            self->bullet_templates[i].spawn_effect_long_vm.set_script(BULLET_TYPE_ANM_DATA[i].spawn_effect_long_script);
            self->bullet_templates[i].despawn_effect_vm.set_script(BULLET_TYPE_ANM_DATA[i].despawn_effect_script);
            self->bullet_templates[i].vm.base_sprite_number = self->bullet_templates[i].vm.sprite_number;
            self->bullet_templates[i].bullet_height_pixels = (int32_t)self->bullet_templates[i].vm.sprite->height_px;
            if (self->bullet_templates[i].vm.sprite->height_px <= 8.0f) { // Pellet
                self->bullet_templates[i].hitbox_size.x = 4.0f;
                self->bullet_templates[i].hitbox_size.y = 4.0f;
            }
            else if (self->bullet_templates[i].vm.sprite->height_px <= 16.0f) { // RingBall, Rice, Ball, Kunai, Shard
                switch (BULLET_TYPE_ANM_DATA[i].main_script) {
                    case etama3_scripts_base_index + Rice:
                        self->bullet_templates[i].hitbox_size.x = 4.0f;
                        self->bullet_templates[i].hitbox_size.y = 4.0f;
                        break;
                    case etama3_scripts_base_index + Kunai:
                        self->bullet_templates[i].hitbox_size.x = 5.0f;
                        self->bullet_templates[i].hitbox_size.y = 5.0f;
                        break;
                    case etama3_scripts_base_index + Shard:
                        self->bullet_templates[i].hitbox_size.x = 4.0f;
                        self->bullet_templates[i].hitbox_size.y = 4.0f;
                        break;
                    default:
                        self->bullet_templates[i].hitbox_size.x = 6.0f;
                        self->bullet_templates[i].hitbox_size.y = 6.0f;
                        break;
                }
            }
            else if (self->bullet_templates[i].vm.sprite->height_px <= 32.0f) { // BigBall, Fireball, Dagger
                switch (BULLET_TYPE_ANM_DATA[i].main_script) {
                    case etama3_scripts_base_index + Fireball:
                        self->bullet_templates[i].hitbox_size.x = 11.0f;
                        self->bullet_templates[i].hitbox_size.y = 11.0f;
                        break;
                    case etama3_scripts_base_index + Dagger:
                        self->bullet_templates[i].hitbox_size.x = 9.0f;
                        self->bullet_templates[i].hitbox_size.y = 9.0f;
                        break;
                    default:
                        self->bullet_templates[i].hitbox_size.x = 16.0f;
                        self->bullet_templates[i].hitbox_size.y = 16.0f;
                        break;
                }
            }
            else { // Bubble
                self->bullet_templates[i].hitbox_size.x = 32.0f;
                self->bullet_templates[i].hitbox_size.y = 32.0f;
            }
        }
        memset(&ITEM_MANAGER, 0, sizeof(ITEM_MANAGER));
        return ZUN_SUCCESS;
    }
};
ValidateFieldOffset(0x0, BulletManager, bullet_templates);
ValidateFieldOffset(0x5600, BulletManager, bullets);
ValidateFieldOffset(0xEC000, BulletManager, lasers);
ValidateFieldOffset(0xF5C00, BulletManager, next_bullet_index);


// 0x4149D0
dllexport int32_t cdecl bullet_manager_on_tick(BulletManager* bullet_manager_arg) asm("{[codecave:BULLET_MANAGER_on_tick]}");
dllexport int32_t cdecl bullet_manager_on_tick(BulletManager* bullet_manager_arg) {
    Bullet* current_bullet = bullet_manager_arg->bullets;
    if (GAME_MANAGER.__byte_2C) {
        return 1;
    }
    ITEM_MANAGER.on_tick();
    bullet_manager_arg->bullet_count = 0;
    int i;
    for (i = 0; i < countof(bullet_manager_arg->bullets); i++, current_bullet++) {
        if (!current_bullet->state) continue;
        bullet_manager_arg->bullet_count++;
        switch (current_bullet->state) {
            case 2: {
                Float3* velocity = &current_bullet->velocity; // EBP-C4
                float multiplierA = 1.0f / 2.0f; // EBP-B4
                Float3 tempA; // EBP-C0
                tempA.z = velocity->z * multiplierA;
                tempA.y = velocity->y * multiplierA;
                tempA.x = velocity->x * multiplierA;
                Float3 tempB = tempA; // EBP-44
                float multiplierB = SUPERVISOR.game_speed; // EBP-D4
                Float3 tempC; // EBP-D0
                tempC.z = tempB.z * multiplierB;
                tempC.y = tempB.y * multiplierB;
                tempC.x = tempB.x * multiplierB;
                Float3 tempD = tempC; // EBP-50
                Float3* position = &current_bullet->position; // EBP-D8
                position->x += tempD.x;
                position->y += tempD.y;
                position->z += tempD.z;
                if (!ANM_MANAGER_PTR->run_anm(&current_bullet->data.spawn_effect_short_vm)) {
                    break;
                }
                goto bullet_state_jump;
            }
            case 3: {
                Float3* velocity = &current_bullet->velocity; // EBP-EC
                float multiplierA = 1.0f / 2.5f; // EBP-DC
                Float3 tempA; // EBP-E8
                tempA.z = velocity->z * multiplierA;
                tempA.y = velocity->y * multiplierA;
                tempA.x = velocity->x * multiplierA;
                Float3 tempB = tempA; // EBP-5C
                float multiplierB = SUPERVISOR.game_speed; // EBP-D4
                Float3 tempC; // EBP-F8
                tempC.z = tempB.z * multiplierB;
                tempC.y = tempB.y * multiplierB;
                tempC.x = tempB.x * multiplierB;
                Float3 tempD = tempC; // EBP-68
                Float3* position = &current_bullet->position; // EBP-100
                position->x += tempD.x;
                position->y += tempD.y;
                position->z += tempD.z;
                if (!ANM_MANAGER_PTR->run_anm(&current_bullet->data.spawn_effect_medium_vm)) {
                    break;
                }
                goto bullet_state_jump;
            }
            case 4: {
                Float3* velocity = &current_bullet->velocity; // EBP-114
                float multiplierA = 1.0f / 3.0f; // EBP-104
                Float3 tempA; // EBP-110
                tempA.z = velocity->z * multiplierA;
                tempA.y = velocity->y * multiplierA;
                tempA.x = velocity->x * multiplierA;
                Float3 tempB = tempA; // EBP-74
                float multiplierB = SUPERVISOR.game_speed; // EBP-124
                Float3 tempC; // EBP-120
                tempC.z = tempB.z * multiplierB;
                tempC.y = tempB.y * multiplierB;
                tempC.x = tempB.x * multiplierB;
                Float3 tempD = tempC; // EBP-80
                Float3* position = &current_bullet->position; // EBP-128
                position->x += tempD.x;
                position->y += tempD.y;
                position->z += tempD.z;
                if (!ANM_MANAGER_PTR->run_anm(&current_bullet->data.spawn_effect_long_vm)) {
                    break;
                }
            }
        bullet_state_jump:
                current_bullet->state = 1;
                current_bullet->timer.set_default();
            case 1: {
                if (current_bullet->flags) {
                    float floatA; // EBP-C
                    if (current_bullet->__unknown_flag_A) {
                        if ((bool)(current_bullet->timer.current <= 16)) {
                            Timer* timer = &current_bullet->timer; // EBP-130
                            floatA = 5.0f - ((float)timer->current + timer->subframe) * 5.0f / 16.0f; // EBP-C
                            float magnitude = floatA + current_bullet->speed; // EBP-134
                            float angle = current_bullet->angle; // EBP-138
                            Float3* velocity = &current_bullet->velocity; // EBP-13C
                            float3_make_from_vector(velocity, angle, magnitude);
                        } else {
                            current_bullet->__unknown_flag_A ^= true;
                        }
                    }
                    else if (current_bullet->__unknown_flag_B) {
                        if ((bool)(current_bullet->timer.current >= current_bullet->acceleration_duration)) {
                            current_bullet->__unknown_flag_B = false;
                        } else {
                            float multiplierA = SUPERVISOR.game_speed; // EBP-14C
                            Float3* acceleration_vec = &current_bullet->acceleration_vec; // EBP-150
                            Float3 tempA; // EBP-148
                            tempA.z = acceleration_vec->z * multiplierA;
                            tempA.y = acceleration_vec->y * multiplierA;
                            tempA.x = acceleration_vec->x * multiplierA;
                            Float3 tempB = tempA; // EBP-8C
                            Float3* velocity = &current_bullet->velocity; // EBP-154
                            velocity->x += tempB.x;
                            velocity->y += tempB.y;
                            velocity->z += tempB.z;
                            current_bullet->angle = zatan2f(velocity->y, velocity->x);
                        }
                    }
                    else if (current_bullet->__unknown_flag_C) {
                        if ((bool)(current_bullet->timer.current >= current_bullet->acceleration_duration)) {
                            current_bullet->__unknown_flag_C = false;
                        } else {
                            current_bullet->angle = reduce_angle_new(current_bullet->angle, current_bullet->angular_velocity);
                            current_bullet->speed += current_bullet->velocity_scalar;
                            float magnitude = current_bullet->speed; // EBP-160
                            float angle = current_bullet->angle; // EBP-164
                            Float3* velocity = &current_bullet->velocity; // EBP-168
                            float3_make_from_vector(velocity, angle, magnitude);
                        }
                    }
                    if (current_bullet->__unknown_flag_D) {
                        if ((bool)(current_bullet->timer.current >= current_bullet->ex_angle_interval * (current_bullet->ex_angle_count + 1))) {
                            current_bullet->ex_angle_count++;
                            if (current_bullet->ex_angle_count >= current_bullet->ex_angle_max_count) {
                                current_bullet->__unknown_flag_D = false;
                            }
                            current_bullet->angle += current_bullet->ex_angle_offset;
                            current_bullet->speed = current_bullet->ex_angle_speed;
                            floatA = current_bullet->speed;
                        } else {
                            Timer* timer = &current_bullet->timer; // EBP-16C
                            float floatB = (float)(current_bullet->ex_angle_interval * current_bullet->ex_angle_count); // EBP-224
                            floatA = current_bullet->speed - (((float)current_bullet->timer.current + current_bullet->timer.subframe) - floatB) * current_bullet->speed / (float)current_bullet->ex_angle_interval;
                        }
                        
                    }
                }
            }
            case 5:
        }
        current_bullet->timer.tick();
    }
    // LASER_PINGAS
    Laser* current_laser = bullet_manager_arg->lasers;
    for (i = 0; i < countof(bullet_manager_arg->lasers); i++, current_laser++) {
        if (!current_laser->in_use) continue;
        current_laser->end_offset += current_laser->speed * SUPERVISOR.game_speed;
        if (current_laser->end_offset - current_laser->start_offset > current_laser->start_length) {
            current_laser->start_offset = current_laser->end_offset - current_laser->start_length;
        }
        if (current_laser->start_offset < 0.0f) {
            current_laser->start_offset = 0.0f;
        }
        Float3 float3A;
        float3A.y = current_laser->width / 2.0f; // EBP-18
        float3A.x = current_laser->end_offset - current_laser->start_offset; // EBP-1C
        Float3 float3B;
        float3B.x = current_laser->position.x + current_laser->start_offset + (current_laser->end_offset - current_laser->start_offset) / 2.0f; // EBP-34
        float3B.y = current_laser->position.y; // EBP-30
        current_laser->vm_0.scale_x = current_laser->width / current_laser->vm_0.sprite->width_px;
        float floatE = current_laser->end_offset - current_laser->start_offset; // EBP-10
        current_laser->vm_0.scale_y = floatE / current_laser->vm_0.sprite->height_px;
        current_laser->vm_0.rotation.z = TWO_PI_f - current_laser->angle;
        switch (current_laser->state) {
            case 0:
                if (current_laser->fade_appear_style) {
                    int32_t alpha = (float)current_laser->timer * 255.0f / (float)current_laser->start_time;
                    if (alpha > 255) {
                        alpha = 255;
                    }
                    current_laser->vm_0.color = alpha << 24;
                } else {
                    int32_t start_timeB = __max(current_laser->start_time, 30);
                    int32_t current_time = current_laser->timer.current;
                    if (current_laser->start_time - start_timeB < current_time) {
                        floatE = (float)current_laser->timer * current_laser->width / (float)current_laser->start_time;
                    } else {
                        floatE = 1.2f;
                    }
                    current_laser->vm_0.scale_x = floatE / 16.0f;
                    float3A.x = floatE / 2.0f;
                }
                if ((bool)(current_laser->timer.current >= current_laser->graze_delay)) {
                    PLAYER.check_laser_collision(&float3B, &float3A, &current_laser->position, current_laser->angle, current_laser->timer.current % 12);
                }
                if ((bool)(current_laser->timer.current < current_laser->start_time)) {
                    break;
                }
                current_laser->timer.set_default();
                current_laser->state++;
            case 1:
                PLAYER.check_laser_collision(&float3B, &float3A, &current_laser->position, current_laser->angle, current_laser->timer.current % 12);
                if ((bool)(current_laser->timer.current < current_laser->duration)) {
                    break;
                }
                current_laser->timer.set_default();
                current_laser->state++;
                if (!current_laser->end_time) {
                    current_laser->in_use = false;
                    continue;
                }
            case 2:
                if (current_laser->fade_appear_style) {
                    int32_t alpha = (float)current_laser->timer * 255.0f / (float)current_laser->start_time;
                    if (alpha > 255) {
                        alpha = 255;
                    }
                    current_laser->vm_0.color = alpha << 24;
                } else {
                    if (current_laser->end_time > 0) {
                        floatE = current_laser->width - (float)current_laser->timer * current_laser->width / (float)current_laser->end_time;
                        current_laser->vm_0.scale_x = floatE / 16.0f;
                        float3A.x = floatE / 2.0f;
                    }
                }
                if ((bool)(current_laser->timer.current < current_laser->graze_interval)) {
                    PLAYER.check_laser_collision(&float3B, &float3A, &current_laser->position, current_laser->angle, current_laser->timer.current % 12);
                }
                if ((bool)(current_laser->timer.current < current_laser->end_time)) {
                    break;
                }
                current_laser->in_use = false;
                continue;
        }
        if (current_laser->start_offset >= 640.0f) {
            current_laser->in_use = false;
        }
        current_laser->timer.tick();
        ANM_MANAGER_PTR->run_anm(&current_laser->vm_0);
    }
    bullet_manager_arg->time.tick();
    return 1;
}

extern "C" {
    // 0x5A5FF8
    extern BulletManager BULLET_MANAGER asm("_BULLET_MANAGER");
}

// ====================
// Enemy
// ====================

enum MoveMode {
    PerAxisSpeed = 0,
    Velocity = 1,
    Interpolate = 2
};

enum InterpMode {
    Constant = 0,
    Decelerate = 1,
    DecelerateFast = 2,
    Accelerate = 3,
    AccelerateFast = 4
};

struct Enemy {
    AnmVM primary_vm;  // 0x0
    AnmVM vms[0x8];  // 0x110
    EclContext current_context;  // 0x990
    EclContext saved_context_stack[0x8];  // 0x9dc
    int32_t stack_depth;  // 0xc3c
    unknown_fields(0x4);  // 0xc40
    int32_t death_callback_sub;  // 0xc44
    int32_t interrupts[0x8];  // 0xc48
    int32_t run_interrupt;  // 0xc68
    Float3 position;  // 0xc6c
    Float3 hitbox_dimensions;  // 0xc78
    Float3 axis_speed;  // 0xc84
    float direction;  // 0xc90
    float rotation;  // 0xc94
    float speed;  // 0xc98
    float acceleration;  // 0xc9c
    Float3 shoot_offset;  // 0xca0
    Float3 move_interp;  // 0xcac
    Float3 move_interp_start_pos;  // 0xcb8
    Timer move_interp_timer;  // 0xcc4
    int32_t move_interp_start_time;  // 0xcd0
    struct {
        float speed_low;  // 0xcd4
        float speed_high;  // 0xcd8
        uint16_t amount1_low;  // 0xcdc
        uint16_t amount1_high;  // 0xcde
        uint16_t amount2_low;  // 0xce0
        uint16_t amount2_high;  // 0xce2
    } bullet_rank;
    int32_t life;  // 0xce4
    int32_t max_life;  // 0xce8
    int32_t score;  // 0xcec
    Timer boss_timer;  // 0xcf0
    D3DCOLOR __color_CFC;  // 0xcfc
    BulletShooter bullet_properties;  // 0xd00
    int32_t shoot_interval;  // 0xd54
    Timer shoot_interval_timer;  // 0xd58
    LaserShooter laser_properties;  // 0xd64
    Laser* lasers[0x20];  // 0xdb8
    int32_t laser_store;  // 0xe38
    union {
        struct {
            uint8_t one;  // 0xe3c
            uint8_t two;  // 0xe3d
            uint8_t three;  // 0xe3e
        };
        uint8_t as_array[3];
    } death_anms;
    uint8_t item_drop;  // 0xe3f
    int8_t boss_id;  // 0xe40
    uint8_t __byte_E41;  // 0xe41
    unknown_fields(0x2);  // 0xe42
    Timer __timer_e44;  // 0xe44
    union {
        uint8_t flags_E50;  // 0xe50
        struct {
            uint8_t move_mode : 2;  // 0, 0x03, 0xFC
            uint8_t interp_mode : 3;  // 2, 0x1C, 0xE3
            uint8_t cannot_shoot : 1;  // 5, 0x20, 0xDF
            uint8_t mirrored : 1;  // 6, 0x40, 0xBF
            uint8_t active : 1;  // 7, 0x80, 0x7F
        };
    };
    union {
        uint8_t flags_E51;  // 0xe51
        struct {
            uint8_t can_interact : 1;  // 0, 0x01, 0xFE
            uint8_t contact_damage : 1;  // 1, 0x02, 0xFD
            uint8_t __offscreen_related : 1;  // 2, 0x04, 0xFB
            uint8_t is_boss : 1;  // 3, 0x08, 0xF7
            uint8_t can_take_damage : 1;  // 4, 0x10, 0xEF
            uint8_t death_flags : 3;  // 5, 0xE0, 0x1F
        };
    };
    union {
        uint8_t flags_E52;  // 0xe52
        struct {
            uint8_t move_limit : 1;  // 0, 0x01, 0xFE
            uint8_t anm_rotate : 1;  // 1, 0x02, 0xFD
            uint8_t disable_call_stack : 1;  // 2, 0x04, 0xFB
            uint8_t invisible : 1;  // 3, 0x08, 0xF7
            uint8_t timeout_spell : 1;  // 4, 0x10, 0xEF
        };
    };
    int8_t anm_ex_flags;  // 0xe53
    union {
        struct {
            int16_t Default;  // 0xe54
            int16_t FarLeft;  // 0xe56
            int16_t FarRight;  // 0xe58
            int16_t Left;  // 0xe5a
            int16_t Right;  // 0xe5c
        };
        int16_t as_array[5];
    } anm_ex;
    probably_padding_bytes(0x2);  // 0xe5e
    union {
        struct {
            float x_lower;  // 0xe60
            float y_lower;  // 0xe64
            float x_upper;  // 0xe68
            float y_upper;  // 0xe6c
        };
        struct {
            Float2 lower;
            Float2 upper;
        };
        float as_array[4];
    } move_limits;
    Effect* __effect_array[12];  // 0xe70
    int32_t __effect_index;  // 0xea0
    float __effect_distance;  // 0xea4
    int32_t life_callback_threshold;  // 0xea8
    int32_t life_callback_sub;  // 0xeac
    int32_t timer_callback_threshold;  // 0xeb0
    int32_t timer_callback_sub;  // 0xeb4
    float __float_eb8;  // 0xeb8
    Timer __timer_ebc;  // 0xebc

    inline void reset_bullet_rank(void) {
        this->bullet_rank.speed_low = -0.5f;
        this->bullet_rank.speed_high = 1.0f;
        memset(&this->bullet_rank.amount1_low, 0, sizeof(uint16_t) * 4);
    }

    forceinline void generic_callback(void);

    // 0x411F40
    dllexport forceinline int32_t check_timer_callback(void) asm("{[codecave:ENEMY_check_timer_callback]}");

    // 0x411DA0
    dllexport int32_t check_life_callback(void) asm("{[codecave:ENEMY_check_life_callback]}");

    // 0x4114C0
    dllexport void __deactivate_effects(void) asm("{[codecave:ENEMY_deactivate_effects]}") {
        for (size_t ii = 0, max_index = this->__effect_index; ii < max_index; ++ii) {
            Effect*& effect = this->__effect_array[ii];
            if (effect == NULL) continue;
            effect->__byte_17A = 1;
            effect = NULL;
        }
        this->__effect_index = 0;
    }

    // Seems to be related to those squares orbitting bosses
    // 0x412E50
    dllexport void ENEMY_spin_effects(void) asm("{[codecave:ENEMY_spin_effects]}") {
        size_t max_index = this->__effect_index;
        if (max_index) {
            /*__asm__(
                "FLDS %1 \n"
                "FLDS %0 \n"
                :
                : "m"(TWO_PI_f), "m"(HUNDREDTH_PI_f)
            );*/
            size_t ii = 0;
            assume(ii < max_index);
            for (; ii < max_index; ++ii) {
                if (Effect* restrict effect = this->__effect_array[ii]) {
                    effect->__float3_134 = this->position;
                    if (effect->__float_15C < this->__effect_distance) {
                        effect->__float_15C += 0.3f;
                    }
                    effect->__float_160 = reduce_angle_new(effect->__float_160, HUNDREDTH_PI_f);
                    /*__asm__(\
                        "FLDS %0 \n" \
                        "FADD %%st(2) \n"
                        "FPREM1 \n" \
                        "FSTPS %0 \n" \
                        : "+m"(effect->__float_160) \
                    ); \*/
                }
            }
            /*__asm__(
                "FFREEP %%st(1) \n"
                "FINCSTP \n"
                :
            );*/
        }
        
        ////pre_lite_reduce_angle();
        //__asm__(
        //    "FLDS %1 \n"
        //    "FLDS %0 \n"
        //    :
        //    : "m"(TWO_PI_f), "m"(HUNDREDTH_PI_f)
        //);
        //for (size_t ii = 0, max_index = this->__effect_index; ii < max_index; ++ii) {
        //    if (Effect* restrict effect = this->__effect_array[ii]) {
        //        effect->__float3_134 = this->position;
        //        if (effect->__float_15C < this->__effect_distance) {
        //            effect->__float_15C += 0.3f;
        //        }
        //        __asm__(\
        //            "FLDS %0 \n" \
        //            "FADD %%st(2) \n"
        //            "FPREM1 \n" \
        //            "FSTPS %0 \n" \
        //            : "+m"(effect->__float_160) \
        //        ); \
        //        //effect->__float_160 = lite_reduce_angle(effect->__float_160 + HUNDREDTH_PI_f);
        //    }
        //}
        ////post_lite_reduce_angle();
        //__asm__(
        //    "FFREEP %%st(1) \n"
        //    "FINCSTP \n"
        //    :
        //);
    }

    // 0x4121B0
    dllexport forceinline void kill(void) asm("{[codecave:ENEMY_kill]}") {
        if (this->can_take_damage) {
            this->active = false;
        } else {
            this->can_interact = false;
        }
        if (this->is_boss) {
            GUI.boss_present = false;
        }
        if (this->__effect_index) {
            this->__deactivate_effects();
        }
    }

    // 0x413380
    dllexport void main_movement(void) asm("{[codecave:ENEMY_main_movement]}") {
        vec<float, 4> axis_speed = *(unaligned vec<float, 4>*)&this->axis_speed;
        axis_speed *= SUPERVISOR.game_speed;
        if (this->mirrored) {
            axis_speed = _mm_xor_ps(axis_speed, (vec<float, 4>) { -0.0f, 0.0f, 0.0f, 0.0f });
            //axis_speed  (vec<float, 4>){-0.0f, 0.0f, 0.0f, 0.0f};
        }
        unaligned vec<float, 4>& position = *(unaligned vec<float, 4>*) & this->position;
        position[0] += axis_speed[0];
        position[1] += axis_speed[1];
        position[2] += axis_speed[2];

        
        /*float game_speed = SUPERVISOR.game_speed;
        if (float temp = this->axis_speed.x * game_speed; !this->mirrored) {
            this->position.x += temp;
        } else {
            this->position.x -= temp;
        }
        this->position.y += this->axis_speed.y * game_speed;
        this->position.z += this->axis_speed.z * game_speed;*/
    }

    //dllexport void write_to_var(int32_t var, EclArg val) asm("{[codecave:ENEMY_write_to_var]}") {
    //    switch (int32_t index = -var - 10001) {
    //        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
    //            this->current_context.int_vars[index] = val.integer;
    //            break;
    //        case 14: case 15: case 16:
    //            this->position.as_array[index - 14] = val.real;
    //            break;
    //        /*case 14:
    //            this->position.x = val.real;
    //            break;
    //        case 15:
    //            this->position.y = val.real;
    //            break;
    //        case 16:
    //            this->position.z = val.real;
    //            break;*/
    //        case 21:
    //            this->boss_timer.current = val.integer;
    //            break;
    //        case 23:
    //            this->life = val.integer;
    //            break;
    //    }
    //}

    dllexport void write_to_var(int32_t var, EclArg val) asm("{[codecave:ENEMY_write_to_var]}") {
        switch (int32_t index = var + 10001) {
            case 0: case -1: case -2: case -3: case -4: case -5: case -6: case -7: case -8: case -9: case -10: case -11:
                this->current_context.int_vars[-index] = val.integer;
                break;
            case -14: case -15: case -16:
                ((float*)&this->position)[-index - 14] = val.real;
                break;
            /*case 14:
                this->position.x = val.real;
                break;
            case 15:
                this->position.y = val.real;
                break;
            case 16:
                this->position.z = val.real;
                break;*/
            case -21:
                this->boss_timer.current = val.integer;
                break;
            case -23:
                this->life = val.integer;
                break;
        }
    }

    //dllexport EclArg* get_var_ptr(int32_t* var) asm("{[codecave:ENEMY_get_var_ptr]}") {
    //    switch (int32_t index = -*var - 10001) {
    //        default:
    //            return (EclArg*)var;
    //        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
    //            return (EclArg*)&this->current_context.int_vars[index];
    //        case 14: case 15: case 16:
    //            return (EclArg*)&this->position.as_array[index - 14];
    //        /*case 14:
    //            return (EclArg*)&this->position.x;
    //        case 15:
    //            return (EclArg*)&this->position.y;
    //        case 16:
    //            return (EclArg*)&this->position.z;*/
    //        case 21:
    //            return (EclArg*)&this->boss_timer.current;
    //        case 23:
    //            return (EclArg*)&this->life;
    //    }
    //}

#define macro_get_var_ptr(enemy, var) \
({ \
    EclArg *restrict ret = (EclArg*)(var); \
    switch (int32_t index = ret->integer + 10001) { \
        case 0: case -1: case -2: case -3: case -4: case -5: case -6: case -7: case -8: case -9: case -10: case -11: \
            ret = (EclArg*)&(enemy)->current_context.int_vars[-index]; \
            break; \
        case -14: case -15: case -16: \
            ret = (EclArg*)&(enemy)->position.as_array[-index - 14]; \
            break; \
        case -21: \
            ret = (EclArg*)&(enemy)->boss_timer.current; \
            break; \
        case -23: \
            ret = (EclArg*)&(enemy)->life; \
            break; \
    } \
    ret; \
})

    dllexport EclArg* get_var_ptr(int32_t* var) const asm("{[codecave:ENEMY_get_var_ptr]}") {
        switch (int32_t index = *var + 10001) {
            default:
                return (EclArg*)var;
            case 0: case -1: case -2: case -3: case -4: case -5: case -6: case -7: case -8: case -9: case -10: case -11:
                return (EclArg*)&this->current_context.int_vars[-index];
            case -14: case -15: case -16:
                return (EclArg*)&((float*)&this->position)[-index - 14];
            /*case 14:
                return (EclArg*)&this->position.x;
            case 15:
                return (EclArg*)&this->position.y;
            case 16:
                return (EclArg*)&this->position.z;*/
            case -21:
                return (EclArg*)&this->boss_timer.current;
            case -23:
                return (EclArg*)&this->life;
        }
    }

    // 0x412240
    dllexport void enfore_move_limit(void) asm("{[codecave:ENEMY_enforce_move_limit]}") {
        if (this->move_limit) {
            //float x_position = this->position.x;
            //this->position.x = __max(__min(x_position, this->move_limits.x_upper), this->move_limits.x_lower);
            //float y_position = this->position.y;
            //this->position.y = __max(__min(y_position, this->move_limits.y_upper), this->move_limits.y_lower);
            unaligned vec<float, 2>& position = *(unaligned vec<float, 2>*)&this->position;
            position = __max(*(vec<float, 2>*)&this->move_limits.lower, __min(*(vec<float, 2>*)&this->move_limits.upper, position));
            
            //__v2sf position = *(__v2sf_u*)&this->position;
            //__v2sf lower_limit = *(__v2sf_u*)&this->move_limits.x_lower;
            //__v2sf upper_limit = *(__v2sf_u*)&this->move_limits.x_upper;
            //*(__v2sf_u*)&this->position = __max(__min(position, upper_limit), lower_limit);
        }
        
        /*if (this->move_limit) {
            if (this->position.x > this->move_limits.x_upper) {
                this->position.x = this->move_limits.x_upper;
            } else if (this->position.x < this->move_limits.x_lower) {
                this->position.x = this->move_limits.x_lower;
            }
            if (this->position.y > this->move_limits.y_upper) {
                this->position.y = this->move_limits.y_upper;
            } else if (this->position.y < this->move_limits.y_lower) {
                this->position.y = this->move_limits.y_lower;
            }
        }*/
    }

    inline void finalize_interp_ecl(int32_t op_index, int32_t time) {
        /*static constexpr uint8_t interp_table[] = {
            Constant << 2,
            Decelerate << 2,
            DecelerateFast << 2,
            Accelerate << 2,
            AccelerateFast << 2
        };*/
        //this->interp_mode = 0;
        //this->flags_E50 |= (op_index - 51) << 2;
        this->interp_mode = (op_index - 51) << 2;
        this->move_interp_start_pos = this->position;
        this->move_interp_start_time = time;
        this->move_interp_timer = (Timer){ -999, 0.0f, time };
        this->move_mode = Interpolate;
    }

    inline void set_main_anm(int32_t index) {
        this->primary_vm.script_number = index;
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        anm_manager->set_vm_script(&this->primary_vm, anm_manager->scripts[index]);
    }

};  // 0xec8

//EclArg regparm(2) no_stack_protector parse_int_as_arg(int32_t arg_val, Enemy* restrict enemy) asm("{[codecave:parse_int_as_arg]}");
//EclArg regparm(2) no_stack_protector parse_int_as_arg(int32_t arg_val, Enemy *restrict enemy) {
//    switch (int32_t index = -arg_val - 10001) {
//        default:
//            return (EclArg) { .integer = arg_val };
//        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
//            return (EclArg) { .integer = enemy->current_context.int_vars[index] };
//        case 12:
//            return (EclArg) { .integer = GAME_MANAGER.difficulty };
//        case 13:
//            return (EclArg) { .integer = GAME_MANAGER.rank.current };
//        case 14: case 15: case 16:
//            return (EclArg) { .real = enemy->position.as_array[index - 14] };
//        /*case 14:
//            return (EclArg){ .real = enemy->position.x };
//        case 15:
//            return (EclArg){ .real = enemy->position.y };
//        case 16:
//            return (EclArg){ .real = enemy->position.z };*/
//        case 17: case 18: case 19:
//            return (EclArg) { .real = PLAYER.position.as_array[index - 17] };
//        /*case 17:
//            return (EclArg){ .real = PLAYER.position.x };
//        case 18:
//            return (EclArg){ .real = PLAYER.position.y };
//        case 19:
//            return (EclArg){ .real = PLAYER.position.z };*/
//        case 20:
//            return (EclArg) { .real = PLAYER.angle_to_player_from_point(&enemy->position) };
//        case 21:
//            return (EclArg) { .integer = enemy->boss_timer.current };
//        case 22:
//            return (EclArg) { .real = PLAYER.distance_to_player_from_point(&enemy->position) };
//        case 23:
//            return (EclArg) { .integer = enemy->life };
//        case 24:
//            return (EclArg) { .integer = GAME_MANAGER.character * 2 + GAME_MANAGER.shottype };
//    }
//}

#define macro_parse_int_as_arg(arg_val, enemy) \
({ \
EclArg ret; \
switch (int32_t index = (arg_val) + 10001) { \
    default: \
        ret = (EclArg) { .integer = arg_val }; \
        break; \
    case 0: case -1: case -2: case -3: case -4: case -5: case -6: case -7: case -8: case -9: case -10: case -11: \
        ret = (EclArg) { .integer = enemy->current_context.int_vars[-index] }; \
        break; \
    case -12: \
        ret = (EclArg) { .integer = GAME_MANAGER.difficulty }; \
        break; \
    case -13: \
        ret = (EclArg) { .integer = GAME_MANAGER.rank.current }; \
        break; \
    case -14: case -15: case -16: \
        ret = (EclArg) { .real = enemy->position.as_array[-index - 14] }; \
        break; \
    case -17: case -18: case -19: \
        ret = (EclArg) { .real = PLAYER.position.as_array[-index - 17] }; \
        break; \
    case -20: \
        ret = (EclArg) { .real = PLAYER.angle_to_player_from_point(&enemy->position) }; \
        break; \
    case -21: \
        ret = (EclArg) { .integer = enemy->boss_timer.current }; \
        break; \
    case -22: \
        ret = (EclArg) { .real = PLAYER.distance_to_player_from_point(&enemy->position) }; \
        break; \
    case -23: \
        ret = (EclArg) { .integer = enemy->life }; \
        break; \
    case -24: \
        ret = (EclArg) { .integer = GAME_MANAGER.character * 2 + GAME_MANAGER.shottype }; \
        break; \
} \
ret; \
})

//EclArg gnu_noinline regparm(2) no_stack_protector parse_int_as_arg(int32_t arg_val, Enemy*volatile restrict enemy) asm("{[codecave:parse_int_as_arg]}");
//EclArg gnu_noinline regparm(2) no_stack_protector parse_int_as_arg(int32_t arg_val, Enemy*volatile restrict enemy) {

dllexport EclArg gnu_noinline no_stack_protector regparm(2) parse_int_as_arg(int32_t arg_val, Enemy*const restrict enemy) asm("{[codecave:parse_int_as_arg]}");
dllexport EclArg gnu_noinline no_stack_protector regparm(2) parse_int_as_arg(int32_t arg_val, Enemy*const restrict enemy) {
    
    switch (int32_t index = arg_val + 10001; expect_chance(index, -20, 0.25)) {
        default:
            return (EclArg) { .integer = arg_val };
        case 0: case -1: case -2: case -3: case -4: case -5: case -6: case -7: case -8: case -9: case -10: case -11:
            return (EclArg) { .integer = enemy->current_context.int_vars[-index] };
        case -12:
            return (EclArg) { .integer = GAME_MANAGER.difficulty };
        case -13:
            return (EclArg) { .integer = GAME_MANAGER.rank.current };
        case -14: case -15: case -16:
            return (EclArg) { .real = ((float*)&enemy->position)[-index - 14] };
        /*case 14:
            return (EclArg){ .real = enemy->position.x };
        case 15:
            return (EclArg){ .real = enemy->position.y };
        case 16:
            return (EclArg){ .real = enemy->position.z };*/
        case -17: case -18: case -19:
            return (EclArg) { .real = ((float*)&PLAYER.position)[-index - 17] };
        /*case 17:
            return (EclArg){ .real = PLAYER.position.x };
        case 18:
            return (EclArg){ .real = PLAYER.position.y };
        case 19:
            return (EclArg){ .real = PLAYER.position.z };*/
        case -20:
            return (EclArg) { .real = PLAYER.angle_to_player_from_point_fast(&enemy->position) };
        case -21:
            return (EclArg) { .integer = enemy->boss_timer.current };
        case -22:
            return (EclArg) { .real = PLAYER.distance_to_player_from_point(&enemy->position) };
        case -23:
            return (EclArg) { .integer = enemy->life };
        case -24:
            return (EclArg) { .integer = GAME_MANAGER.character * 2 + GAME_MANAGER.shottype };
    }
}

dllexport EclArg no_stack_protector parse_float_as_arg(Enemy*const restrict enemy, EclArg value) asm("{[codecave:parse_float_as_arg]}");
EclArg no_stack_protector parse_float_as_arg(Enemy*const restrict enemy, EclArg value) {
    int32_t temp = value.real;
    EclArg ret = parse_int_as_arg(temp, enemy);
    if (ret.integer == temp) ret.integer = value.integer;
    return ret;
}

//inline EclArg no_stack_protector vectorcall parse_float_as_arg(Enemy* restrict enemy, float value) asm("[codecave:parse_float_as_arg]");
//inline EclArg no_stack_protector vectorcall parse_float_as_arg(Enemy* restrict enemy, float value) {
//    int32_t temp = value;
//    EclArg ret = parse_int_as_arg(temp, enemy);
//    if (ret.integer == temp) ret.real = value;
//    return ret;
//}

typedef struct TimelineInstruction TimelineInstruction;
struct TimelineInstruction {
    int16_t time;
    int16_t first_arg;
    int16_t opcode;
    int16_t offset_to_next;
    unsigned char args[];
};

typedef struct EnemyController EnemyController;

typedef struct EnemyManager EnemyManager;

struct EnemyManager {
    char *restrict stg_enm_anm_filename;  // 0x0
    char *restrict stg_enm2_anm_filename;  // 0x4
    Enemy template_enemy;  // 0x8
    Enemy enemies[256];  // 0xED0
    Enemy dummy_enemy_for_failed_spawns; // 0xED6D0
    Enemy *restrict bosses[8];  // 0xEE598
    uint16_t random_item_spawn_index;  // 0xEE5B8
    uint16_t random_item_table_index;  // 0xEE5BA
    int32_t enemy_count_real;  // 0xEE5BC
    unknown_fields(0x4);  // 0xEE5C0
    int __dword_EE5C4;  // 0xEE5C4
    int32_t __spellcard_capture;  // 0xEE5C8
    int32_t __spellcard_bonus;  // 0xEE5CC
    int32_t __spellcard_number;  // 0xEE5D0
    int __dword_EE5D4;  // 0xEE5D4
    unknown_fields(0x4);  // 0xEE5D8
    TimelineInstruction *restrict timeline_instr;  // 0xEE5DC
    Timer timeline_time;  // 0xEE5E0
    // 0xEE5EC

    // 0x411390
    dllexport Enemy* create_enemy(int16_t SubID, Float3* position, int16_t life, int8_t item, int32_t score) asm("{[codecave:ENEMY_MANAGER_create_enemy]}");

    // 0x411530
    dllexport void run_timeline(void) asm("{[codecave:ENEMY_MANAGER_run_timeline]}");

    forceinline void wtf_zun_why(void);

};  // 0xee5ec

extern "C" {
    // 0x4B79C8
    extern EnemyManager ENEMY_MANAGER asm("_ENEMY_MANAGER");
    // 0x476338
    extern uint32_t RandomItemIDs_LookupTable[32] asm("_RandomItemIDs_LookupTable");
    // 0x476120
    extern int32_t SpellcardPointsTable[64] asm("_SpellcardPointsTable");
}

// ====================
// Funcs
// ====================

inline float __attribute__((target("no-sse"))) rand_float_helper(void) {
    RNG.rand_float();
    float ret;
    __asm__(
        "fstps %0"
        : "=m"(ret)
    );
    return ret;
}

dllexport gnu_noinline void cdecl hardcoded_func_16_stageEx(Enemy* restrict enemy, EclInstruction* restrict current_instruction) {
    int32_t intA = enemy->life;
    if ((bool)(enemy->boss_timer.current >= 7200)) {
        intA = 0;
    }
    if (IntArg(0) == 0) {
        enemy->current_context.float_vars[3] = 2.0f - (float)intA * 1.0f / 6000.0f;
        enemy->current_context.counter_vars[1] = 40 + intA * 240 / 6000;
    } else {
        float floatA = 320.0f - (float)intA * 160.0f / 6000.0f;
        float floatB = 192.0f - floatA / 2.0f;
        enemy->current_context.float_vars[2] = RNG.rand_float() * floatA + floatB;
        floatA = 128.0f - (float)intA * 64.0f / 6000.0f;
        float floatC = 96.0f - floatA / 2.0f;
        enemy->current_context.float_vars[3] = RNG.rand_float() * floatA + floatC;
    }
}

extern "C" {
    // 0x476220
    extern ins_call FuncCall_PtrTable[17] asm("_FuncCall_PtrTable");
}

dllexport gnu_noinline void dummy_ins(Enemy* restrict enemy, EclInstruction* restrict current_instruction) {
    gnu_used static volatile auto tempA = enemy;
    gnu_used static volatile auto tempB = current_instruction;
}

static const ins_call extension_ins[] asm("{<codecave:placeholder>}") = { dummy_ins, NULL };
static constexpr auto func_size = countof(FuncCall_PtrTable);


struct EnemyController {

    void *restrict ecl_file_buffer;
    EclInstruction** sub_table;
    TimelineInstruction *restrict current_timeline_instruction;

    forceinline ins_call get_extension_ins(int32_t func_index) {
        if (expect(func_index < countof(FuncCall_PtrTable), true)) {
            return FuncCall_PtrTable[func_index];
        } else if (func_index < 0) {
            return NULL;
        } else {
            return extension_ins[func_index - countof(FuncCall_PtrTable)];
        }
    }

    // 0x407440
    dllexport void init_sub_context(EclContext& current_context, int16_t sub_id) asm("{[codecave:ENEMY_CONTROLLER_init_sub_context]}") {
        current_context.current_instr = this->sub_table[sub_id];
        //current_context.time = (Timer){ -999, 0.0f, 0 };
        //current_context.time = static_default_timer<0>;
        current_context.time.set_default();
        current_context.sub_id = sub_id;
    }

#define MakeArg(arg) \
_Generic((arg), \
    EclArg      : (arg), \
    int32_t     : (EclArg){ .integer = (arg) }, \
    uint32_t    : (EclArg){ .uinteger = (arg) }, \
    float       : (EclArg){ .real = (arg) } \
)


#define ParseArg(number) \
(parse_int_as_arg(IntArg(number), enemy))

#define ParseShortArg(number) \
(parse_int_as_arg(ShortArg(number), enemy).integer)

#define ParseIntArg(number) \
(parse_int_as_arg(IntArg(number), enemy).integer)

#define ParseFloatArg(number) \
(parse_float_as_arg(enemy, RawArg(number)).real)

#define WriteVar(number, val) \
enemy->write_to_var(IntArg(number), MakeArg(val))

    // 0x4074A0
    dllexport int32_t thiscall run_ecl(Enemy *const restrict enemy) asm("{[codecave:ENEMY_CONTROLLER_run_ecl]}") {
        EclContext& current_context = enemy->current_context;
        EclInstruction *restrict current_instruction = current_context.current_instr;

        int32_t op_index;
        if (expect(enemy->run_interrupt >= 0, false)) {
            goto RunInterrupt;
        }
        goto NextInstr;
        for (;;) {
            gnu_used static constexpr void* im_sorry = &&InstrLoop;
        InstrLoop:
            IndexInstr(current_instruction->offset_to_next);
        NextInstr:
            if (current_context.time.current != current_instruction->time) break;
        JumpInstr:
            if (expect(!(current_instruction->difficulty_mask >> GAME_MANAGER.difficulty & 1), false)) continue;
            //switch (expect_chance(op_index = current_instruction->opcode, 2, 0.0)) {
            switch (op_index = current_instruction->opcode) {
                case 0: // nop
                case 127: // debug_watch
                    break;
                case 1: // enemy_delete
                    return -1;
                case 3: // loop
                    if (
                        EclArg* restrict loop_ptr = enemy->get_var_ptr(&IntArg(2));
                        expect(--loop_ptr->integer <= 0, false)
                    ) break;
                    [[fallthrough]];
                case 2: JumpLabel: // jump
                    current_context.time.current = IntArg(0);
                    IndexInstr(IntArg(1));
                    //goto NextInstr;
                    goto JumpInstr;
                case 4: case 5: // set_int, set_float
                    enemy->write_to_var(IntArg(0), MakeArg(ParseIntArg(1)));
                    break;
                {
                    int32_t rng_min;
                case 6: // set_int_rand_bound
                    rng_min = 0;
                    goto RunIntRNG;
                case 7: // set_int_rand_bound_min
                    rng_min = ParseIntArg(2);
                RunIntRNG:
                    int32_t temp = ParseIntArg(1);
                    enemy->write_to_var(IntArg(0), MakeArg(RNG.rand_uint_mod(temp) + rng_min));
                    break;
                }
                {
                    float rng_min;
                case 8: // set_float_rand_bound
                    rng_min = 0.0f;
                    goto RunFloatRNG;
                case 9: // set_float_rand_bound_min
                    rng_min = ParseFloatArg(2);
                RunFloatRNG:
                    enemy->write_to_var(IntArg(0), MakeArg(RNG.rand_float() * ParseFloatArg(1) + rng_min));
                    break;
                }
                case 10: // set_var_self_x
                case 11: // set_var_self_y
                case 12: // set_var_self_z
                    enemy->write_to_var(IntArg(0), MakeArg(((float*)&enemy->position)[op_index - 10]));
                    break;
                //{
                //case 13: // math_int_add
                //case 20: // math_float_add
                //    EclArg val1 = parse_int_as_arg(1, enemy);
                //    EclArg val2 = parse_int_as_arg(2, enemy);
                //    enemy->write_to_var_math(IntArg(0), MakeArg(val1.integer + val2.integer), MakeArg(val1.real + val2.real));
                //    break;
                //}
                //{
                //case 14: // math_int_sub
                //case 21: // math_float_sub
                //    EclArg val1 = parse_int_as_arg(1, enemy);
                //    EclArg val2 = parse_int_as_arg(2, enemy);
                //    enemy->write_to_var_math(IntArg(0), MakeArg(val1.integer - val2.integer), MakeArg(val1.real - val2.real));
                //    break;
                //}
                //{
                //case 15: // math_int_mul
                //case 22: // math_float_mul
                //    EclArg val1 = parse_int_as_arg(1, enemy);
                //    EclArg val2 = parse_int_as_arg(2, enemy);
                //    enemy->write_to_var_math(IntArg(0), MakeArg(val1.integer * val2.integer), MakeArg(val1.real * val2.real));
                //    break;
                //}
                //{
                //case 16: // math_int_div
                //case 23: // math_float_div
                //    EclArg val1 = parse_int_as_arg(1, enemy);
                //    EclArg val2 = parse_int_as_arg(2, enemy);
                //    enemy->write_to_var_math(IntArg(0), MakeArg(val1.integer / val2.integer), MakeArg(val1.real / val2.real));
                //    break;
                //}
                //{
                //case 17: // math_int_mod
                //case 24: // math_float_mod
                //    EclArg val1 = parse_int_as_arg(1, enemy);
                //    EclArg val2 = parse_int_as_arg(2, enemy);
                //    enemy->write_to_var_math(IntArg(0), MakeArg(val1.integer + val2.integer), MakeArg(fmodf(val1.real, val2.real)));
                //    break;
                //}
                case 13: // math_int_add
                    enemy->write_to_var(IntArg(0), MakeArg(ParseIntArg(1) + ParseIntArg(2)));
                    break;
                case 14: // math_int_sub
                    enemy->write_to_var(IntArg(0), MakeArg(ParseIntArg(1) - ParseIntArg(2)));
                    break;
                case 15: // math_int_mul
                    enemy->write_to_var(IntArg(0), MakeArg(ParseIntArg(1) * ParseIntArg(2)));
                    break;
                case 16: // math_int_add
                    enemy->write_to_var(IntArg(0), MakeArg(ParseIntArg(1) / ParseIntArg(2)));
                    break;
                case 17: // math_int_add
                    enemy->write_to_var(IntArg(0), MakeArg(ParseIntArg(1) % ParseIntArg(2)));
                    break;
                case 18: // math_inc
                    ++enemy->get_var_ptr(&IntArg(0))->integer;
                    break;
                case 19: // math_dec
                    --enemy->get_var_ptr(&IntArg(0))->integer;
                    break;
                case 20: // math_float_add
                    enemy->write_to_var(IntArg(0), MakeArg(ParseFloatArg(1) + ParseFloatArg(2)));
                    break;
                case 21: // math_float_sub
                    enemy->write_to_var(IntArg(0), MakeArg(ParseFloatArg(1) - ParseFloatArg(2)));
                    break;
                case 22: // math_float_mul
                    enemy->write_to_var(IntArg(0), MakeArg(ParseFloatArg(1) * ParseFloatArg(2)));
                    break;
                case 23: // math_float_div
                    enemy->write_to_var(IntArg(0), MakeArg(ParseFloatArg(1) / ParseFloatArg(2)));
                    break;
                case 24: // math_float_mod
                    enemy->write_to_var(IntArg(0), MakeArg(zfmodf(ParseFloatArg(1), ParseFloatArg(2))));
                    break;
                case 25: // math_atan2
                    enemy->write_to_var(IntArg(0), MakeArg(zatan2f(ParseFloatArg(3) - ParseFloatArg(1), ParseFloatArg(4) - ParseFloatArg(2))));
                    break;
                case 26: { // math_reduce_angle
                    EclArg *restrict temp = enemy->get_var_ptr(&IntArg(0));
                    //EclArg *restrict temp = macro_get_var_ptr(enemy, &IntArg(0));
                    /*float temp2;
                    __asm__(
                        "FLDS %[TWO_PI] \n"
                        : "=t"(temp2)
                        : [TWO_PI] "m"(TWO_PI_f)
                    );
                    __asm__(
                        "FPREM1 \n"
                        : "+t"(temp->real)
                        : "u"(temp2)
                    );
                    __asm__(
                        "FLDS %[TWO_PI] \n"
                        "FLDS %[ANGLE] \n"
                        "FPREM1 \n"
                        "FSTPS %[ANGLE] \n"
                        "FFREEP %%st(0) \n"
                        : [ANGLE]"+m"(temp->real)
                        : [TWO_PI]"m"(TWO_PI_f)
                    );*/
                    temp->real = reduce_angle_new(temp->real, 0.0f);
                    break;
                }
                case 27: { // cmp_int
                    int32_t int1 = ParseIntArg(0);
                    int32_t int2 = ParseIntArg(1);
                    int compare;
                    if (int1 == int2) {
                        compare = 0;
                    } else {
                        compare = int1 >= int2 ? 1 : -1;
                    }
                    current_context.compare_register = compare;
                    break;
                }
                case 28: { // cmp_float
                    float float1 = ParseFloatArg(0);
                    float float2 = ParseFloatArg(1);
                    int compare;
                    if (float1 == float2) {
                        compare = 0;
                    } else {
                        compare = float1 > float2 ? 1 : -1;
                    }
                    current_context.compare_register = compare;
                    break;
                }
                [[unlikely]] case 29: [[unlikely]] // jump_lss
                    if (current_context.compare_register < 0) goto JumpLabel;
                    break;
                [[unlikely]] case 30: [[unlikely]] // jump_leq
                    if (current_context.compare_register <= 0) goto JumpLabel;
                    break;
                [[unlikely]] case 31: [[unlikely]] // jump_equ
                    if (current_context.compare_register == 0) goto JumpLabel;
                    break;
                [[unlikely]] case 32: [[unlikely]] // jump_gre
                    if (current_context.compare_register > 0) goto JumpLabel;
                    break;
                [[unlikely]] case 33: [[unlikely]] // jump_geq
                    if (current_context.compare_register >= 0) goto JumpLabel;
                    break;
                [[unlikely]] case 34: [[unlikely]] // jump_neq
                    if (current_context.compare_register != 0) goto JumpLabel;
                    break;
                case 35: CallLabel: // call
                    IndexInstr(current_instruction->offset_to_next);
                    if (expect(!enemy->disable_call_stack, true)) {
                        int32_t context_depth = enemy->stack_depth;
                        enemy->saved_context_stack[context_depth] = current_context;
                        if (expect(context_depth < 7, true)) ++enemy->stack_depth;
                    }
                    this->init_sub_context(current_context, IntArg(0));
                    current_context.int_vars[0] = IntArg(1);
                    current_context.float_vars[0] = FloatArg(2);
                    break;
                case 36: // return
                    if (expect(!enemy->disable_call_stack, true)) {
                        int32_t context_depth = enemy->stack_depth;
                        enemy->saved_context_stack[context_depth] = current_context;
                        if (expect(context_depth > 0, true)) --enemy->stack_depth;
                    }
                    break;
                case 37: // call_lss
                    if (ParseIntArg(3) < IntArg(4)) goto CallLabel;
                    break;
                case 38: // call_leq
                    if (ParseIntArg(3) <= IntArg(4)) goto CallLabel;
                    break;
                case 39: // call_equ
                    if (ParseIntArg(3) == IntArg(4)) goto CallLabel;
                    break;
                case 40: // call_gre
                    if (ParseIntArg(3) > IntArg(4)) goto CallLabel;
                    break;
                case 41: // call_geq
                    if (ParseIntArg(3) >= IntArg(4)) goto CallLabel;
                    break;
                case 42: // call_neq
                    if (ParseIntArg(3) != IntArg(4)) goto CallLabel;
                    break;
                case 43: // move_position
                    enemy->position.x = ParseFloatArg(0);
                    enemy->position.y = ParseFloatArg(1);
                    enemy->position.z = ParseFloatArg(2);
                    enemy->enfore_move_limit();
                    break;
                case 44: // move_axis_speed
                    enemy->axis_speed.x = ParseFloatArg(0);
                    enemy->axis_speed.y = ParseFloatArg(1);
                    enemy->axis_speed.z = ParseFloatArg(2);
                    enemy->move_mode = PerAxisSpeed;
                    break;
                case 45: // move_velocity
                    enemy->direction = ParseFloatArg(0);
                    enemy->speed = ParseFloatArg(1);
                    enemy->move_mode = Velocity;
                    break;
                case 46: // move_angular_velocity
                    enemy->rotation = ParseFloatArg(0);
                    enemy->move_mode = Velocity;
                    break;
                case 47: // move_speed
                    enemy->speed = ParseFloatArg(0);
                    enemy->move_mode = Velocity;
                    break;
                case 48: // move_acceleration
                    enemy->acceleration = ParseFloatArg(0);
                    enemy->move_mode = Velocity;
                case 49: { // move_rand
                    float min = FloatArg(0);
                    enemy->direction = RNG.rand_float() * (FloatArg(1) - min) + min;
                    break;
                }
                case 50: { // move_rand_in_bounds
                    float min = FloatArg(0);
                    float new_direction = RNG.rand_float() * (FloatArg(1) - min) + min;

                    const float x_position = enemy->position.x;
                    if ((enemy->move_limits.x_upper + 96.0f) > x_position) {
                        if (new_direction > HALF_PI_f) {
                            new_direction = PI_f - new_direction;
                        } else if (new_direction < NHALF_PI_f) {
                            new_direction = NPI_f - new_direction;
                        }
                    }
                    if ((enemy->move_limits.x_lower - 96.0f) < x_position) {
                        if (new_direction < HALF_PI_f && new_direction >= 0.0f) {
                            new_direction = PI_f - new_direction;
                        } else if (new_direction > NHALF_PI_f && new_direction <= 0.0f) {
                            new_direction = NPI_f - new_direction;
                        }
                    }
                    const float y_position = enemy->position.y;
                    if ((enemy->move_limits.y_upper + 48.0f) > y_position && new_direction < 0.0f) {
                        new_direction = -new_direction;
                    }
                    if ((enemy->move_limits.y_lower - 48.0f) < y_position && new_direction > 0.0f) {
                        new_direction = -new_direction;
                    }
                    enemy->direction = new_direction;
                    break;
                }
                case 51: // move_at_player
                    enemy->direction = PLAYER.angle_to_player_from_point(&enemy->position) + FloatArg(0);
                    enemy->speed = ParseFloatArg(1);
                    enemy->move_mode = Velocity;
                    break;
                case 56: // move_position_time_linear
                case 57: // move_position_time_decelerate
                case 58: // move_position_time_decelerate_fast
                case 59: // move_position_time_accelerate
                case 60: // move_position_time_accelerate_fast
                    enemy->move_interp.x = ParseFloatArg(1) - enemy->position.x;
                    enemy->move_interp.y = ParseFloatArg(2) - enemy->position.y;
                    enemy->move_interp.z = ParseFloatArg(3) - enemy->position.z;
                    memset(&enemy->axis_speed, 0, sizeof(enemy->axis_speed));
                    //enemy->axis_speed = (Float3){ 0.0f, 0.0f, 0.0f };
                    enemy->finalize_interp_ecl(op_index - 5, IntArg(0));
                    break;
                {
                    float angle;
                    float scale;
                case 61: // move_time_decelerate
                case 62: // move_time_decelerate_fast
                case 63: // move_time_accelerate
                case 64: // move_time_accelerate_fast
                    op_index -= 9;
                    angle = enemy->direction;
                    scale = enemy->speed;
                    goto ParseInterpLabel;
                case 52: // move_dir_time_decelerate
                case 53: // move_dir_time_decelerate_fast
                case 54: // move_dir_time_accelerate
                case 55: // move_dir_time_accelerate_fast
                    angle = FloatArg(1);
                    scale = FloatArg(2);
                ParseInterpLabel:
                    angle = parse_float_as_arg(enemy, MakeArg(angle)).real;
                    int32_t time = IntArg(0);
                    scale = scale * time / 2.0f;
                    enemy->move_interp.x = zcosf(angle) * scale;
                    enemy->move_interp.y = zsinf(angle) * scale;
                    enemy->move_interp.z = 0.0f;
                    enemy->finalize_interp_ecl(op_index, time);
                    break;
                }
                case 65: // move_bounds_set
                    *(__m128_u*)enemy->move_limits.as_array = *(__m128_u*)current_instruction->args;
                    enemy->move_limit = true;
                    break;
                case 66: // move_bounds_disable
                    enemy->move_limit = false;
                    break;
                {
                case 67: // bullet_fan_aimed
                case 68: // bullet_fan
                case 69: // bullet_circle_aimed
                case 70: // bullet_circle
                case 71: // bullet_offset_circle_aimed
                case 72: // bullet_offset_circle
                case 73: // bullet_random_angle
                case 74: // bullet_random_speed
                case 75: // bullet_random

                    BulletShooter *restrict bullet_properties = &enemy->bullet_properties;

                    bullet_properties->sprite = ShortArg(0);
                    bullet_properties->aim_mode = op_index - 67;

                    //int32_t rank = GAME_MANAGER.rank.current;
                    {
                        int32_t count_temp = enemy->bullet_rank.amount1_low;
                        int32_t rank_temp = ((enemy->bullet_rank.amount1_high - count_temp) * GAME_MANAGER.rank.current) / 32;
                        count_temp += rank_temp + ParseIntArg(1);
                        bullet_properties->count_1 = __max(count_temp, 1);
                    }
                    {
                        int32_t count_temp = enemy->bullet_rank.amount2_low;
                        int32_t rank_temp = ((enemy->bullet_rank.amount2_high - count_temp) * GAME_MANAGER.rank.current) / 32;
                        count_temp += rank_temp + ParseIntArg(2);
                        bullet_properties->count_2 = __max(count_temp, 1);
                    }

                    // Yes, this overwrites the angle1 field after
                    // the position Float3 with meaningless garbage.
                    // However, that field is going to get overwritten in a moment anyway.
                    *(__m128_u*)&bullet_properties->position = *(__m128_u*)&enemy->position + *(__m128_u*)&enemy->shoot_offset;

                    //bullet_properties->angle1 = reduce_angle(ParseFloatArg(5));
                    /*float temp = ParseFloatArg(5);
                    __asm__(
                        "FLDS %[TWO_PI] \n"
                        "FLDS %[ANGLE_IN] \n"
                        "FPREM1 \n"
                        "FSTPS %[ANGLE_OUT] \n"
                        "FFREEP %%st(0) \n"
                        : [ANGLE_OUT]"=m"(bullet_properties->angle1)
                        : [ANGLE_IN]"m"(temp), [TWO_PI]"m"(TWO_PI_f)
                    );*/
                    bullet_properties->angle1 = reduce_angle_new(ParseFloatArg(5), 0.0f);
                    
                    float low_rank_temp = enemy->bullet_rank.speed_low;
                    float rank_temp = (enemy->bullet_rank.speed_high - low_rank_temp) * GAME_MANAGER.rank.current / 32.0f + low_rank_temp;

                    {
                        float speed_temp = ParseFloatArg(3);
                        if (expect(speed_temp != 0.0f, true)) {
                            speed_temp += rank_temp;
                            speed_temp = __max(speed_temp, 0.3f);
                        }
                        bullet_properties->speed1 = speed_temp;
                    }
                    
                    bullet_properties->angle2 = ParseFloatArg(6);

                    {
                        float speed_temp = ParseFloatArg(4);
                        speed_temp += rank_temp / 2.0f;
                        speed_temp = __max(speed_temp, 0.3f);
                        bullet_properties->speed2 = speed_temp;
                    }

                    bullet_properties->__word_4A = 0;
                    bullet_properties->flags = IntArg(7);
                    bullet_properties->color = ParseShortArg(1);
                    if (!enemy->cannot_shoot) {
                        BULLET_MANAGER.shoot_bullets(bullet_properties);
                    }
                    break;
                }
                case 76: { // shoot_interval
                    int32_t temp_interval = IntArg(0);
                    int32_t temp_interval_2 = temp_interval / 5;
                    temp_interval += -temp_interval_2 * GAME_MANAGER.rank.current / 32 + temp_interval_2;
                    enemy->shoot_interval = temp_interval;
                    //enemy->shoot_interval_timer = (Timer){ -999, 0.0f, 0 };
                    //enemy->shoot_interval_timer = static_default_timer<0>;
                    enemy->shoot_interval_timer.set_default();
                    break;
                }
                case 77: { // shoot_interval_delay
                    int32_t temp_interval = IntArg(0);
                    int32_t temp_interval_2 = temp_interval / 5;
                    temp_interval += -temp_interval_2 * GAME_MANAGER.rank.current / 32 + temp_interval_2;
                    enemy->shoot_interval = temp_interval;
                    if (temp_interval) {
                        enemy->shoot_interval_timer = (Timer){ -999, 0.0f, (int32_t)(RNG.rand_uint() % temp_interval) };
                    }
                    break;
                }
                case 78: // shoot_disable
                    enemy->cannot_shoot = true;
                    break;
                case 79: // shoot_enable
                    enemy->cannot_shoot = false;
                    break;
                case 80: // shoot_now
                    enemy->bullet_properties.position.x = enemy->position.x + enemy->shoot_offset.x;
                    enemy->bullet_properties.position.y = enemy->position.y + enemy->shoot_offset.y;
                    enemy->bullet_properties.position.z = enemy->position.z + enemy->shoot_offset.z;
                    BULLET_MANAGER.shoot_bullets(&enemy->bullet_properties);
                    break;
                case 81: // shoot_offset
                    enemy->shoot_offset.x = ParseFloatArg(0);
                    enemy->shoot_offset.y = ParseFloatArg(1);
                    enemy->shoot_offset.z = ParseFloatArg(2);
                    break;
                case 82: // bullet_effects
                    for (uint8_t ii = 0; ii < 4; ++ii) {
                        enemy->bullet_properties.int_data[ii] = ParseIntArg(ii);
                    }
                    for (uint8_t ii = 0; ii < 4; ++ii) {
                        enemy->bullet_properties.float_data[ii] = ParseFloatArg(4 + ii);
                    }
                    break;
                case 83: // bullet_cancel
                    BULLET_MANAGER.cancel_bullets_to_star_items();
                    break;
                case 84: // bullet_sound
                    if (int32_t sound_id = IntArg(0); expect(enemy->bullet_properties.play_sound = sound_id >= 0, true)) {
                        enemy->bullet_properties.sfx = sound_id;
                    }
                    break;
                {
                case 85: // laser_create
                case 86: // laser_create_aimed

                    LaserShooter *restrict laser_properties = &enemy->laser_properties;

                    laser_properties->type = op_index == 86;
                    
                    *(__m128_u*)&laser_properties->position = *(__m128_u*)&enemy->position + *(__m128_u*)&enemy->shoot_offset;

                    memcpy(&laser_properties->sprite, &ShortArg(0), sizeof(int32_t));
                    //laser_properties->sprite = ShortArg(0);
                    //laser_properties->color = ShortArg(1);

                    laser_properties->angle = ParseFloatArg(1);
                    laser_properties->speed = ParseFloatArg(2);
                    laser_properties->start_offset = ParseFloatArg(3);
                    laser_properties->end_offset = ParseFloatArg(4);
                    laser_properties->start_length = ParseFloatArg(5);

                    memcpy(&laser_properties->width, &FloatArg(6), sizeof(int32_t) * 6);
                    /*laser_properties->width = FloatArg(6);
                    laser_properties->start_time = IntArg(7);
                    laser_properties->duration = IntArg(8);
                    laser_properties->stop_time = IntArg(9);
                    laser_properties->graze_delay = IntArg(10);
                    laser_properties->graze_distance = IntArg(11);*/

                    laser_properties->flags_dword = IntArg(12);
                    enemy->lasers[enemy->laser_store] = BULLET_MANAGER.shoot_lasers(laser_properties);
                    break;
                }
                case 87: // laser_index
                    enemy->laser_store = ParseIntArg(0);
                    break;
                case 88: // laser_rotate
                    if (
                        Laser* restrict laser = enemy->lasers[IntArg(0)];
                        expect(laser != NULL, true)
                    ) {
                        laser->angle += ParseFloatArg(1);
                    }
                    break;
                case 89: // laser_rotate_from_player
                    if (
                        Laser* restrict laser = enemy->lasers[IntArg(0)];
                        expect(laser != NULL, true)
                    ) {
                        laser->angle = ParseFloatArg(1) + PLAYER.angle_to_player_from_point(&laser->position);
                    }
                    break;
                case 90: // laser_offset
                    if (
                        Laser* restrict laser = enemy->lasers[IntArg(0)];
                        expect(laser != NULL, true)
                    ) {
                        laser->position.x += FloatArg(1);
                        laser->position.y += FloatArg(2);
                        laser->position.z += FloatArg(3);
                    }
                    break;
                case 91: // laser_test
                    if (
                        Laser* restrict laser = enemy->lasers[IntArg(0)];
                        expect(laser != NULL, true)
                    ) {
                        current_context.compare_register = !laser->in_use;
                    }
                    break;
                case 92: // laser_cancel
                    if (
                        Laser* restrict laser = enemy->lasers[IntArg(0)];
                        expect(laser != NULL, true)
                    ) {
                        if (laser->in_use && laser->state < 2) {
                            laser->state = 2;
                            //laser->timer = (Timer){ -999, 0.0f, 0 };
                            //laser->timer = static_default_timer<0>;
                            laser->timer.set_default();
                        }
                    }
                    break;
                case 93: { // spellcard_start
                    char *restrict spell_name = (char* restrict) & IntArg(1);
                    GUI.spawn_spellcard_effects(ShortArg(0), spell_name);
                    ENEMY_MANAGER.__dword_EE5C4 = 1;
                    ENEMY_MANAGER.__spellcard_capture = 1;
                    int32_t card_number = ShortArg(1);
                    ENEMY_MANAGER.__spellcard_number = card_number;
                    int32_t spell_bonus = SpellcardPointsTable[card_number];
                    ENEMY_MANAGER.__spellcard_bonus = spell_bonus;
                    BULLET_MANAGER.cancel_bullets_to_star_items();
                    STAGE.__set_by_spellcard_ecl_80 = 1;
                    STAGE.__set_by_spellcard_ecl_84 = 0;
                    enemy->bullet_rank.speed_low = -0.5f;
                    enemy->bullet_rank.speed_high = 0.5f;
                    enemy->bullet_rank.amount1_low = 0;
                    enemy->bullet_rank.amount1_high = 0;
                    enemy->bullet_rank.amount2_low = 0;
                    enemy->bullet_rank.amount2_high = 0;

                    if (GAME_MANAGER.__spellcard_related_1) break;

                    ScorefileCATK *restrict catk = &GAME_MANAGER.catk_data[card_number];
                    uint8_t trash_hash = 0;
                    char* restrict scorefile_spell_name = catk->spell_name;
                    char c;
                    do {
                        c = *spell_name++;
                        trash_hash += c;
                        *scorefile_spell_name++ = c;
                    } while (c);
                    if (expect(trash_hash != catk->spell_name_hash, false)) {
                        catk->spell_name_hash = trash_hash;
                        memset(&catk->attempts, 0, sizeof(int32_t));
                    }
                    catk->highscore = spell_bonus;
                    if (expect(catk->attempts < 9999, true)) ++catk->attempts;
                    break;
                }
                case 94: { // spellcard_end
                    int32_t capture = ENEMY_MANAGER.__spellcard_capture;
                    if (capture) {
                        GUI.vm6_auto_rotate();
                        if (capture == 1) {
                            BULLET_MANAGER.cancel_bullets_to_star_items_spell_bonus(3200);
                            //BULLET_MANAGER.cancel_bullets_spell_bonus(3200, 1);
                            if (ENEMY_MANAGER.__dword_EE5C4) {
                                ScorefileCATK *restrict catk = &GAME_MANAGER.catk_data[ENEMY_MANAGER.__spellcard_number];
                                int32_t spell_score = ENEMY_MANAGER.__spellcard_bonus;
                                spell_score += spell_score * GUI.__read_by_spellcard_ecl / 10;
                                GUI.__sub_417458(spell_score);
                                GAME_MANAGER.score += spell_score;
                                if (!GAME_MANAGER.__spellcard_related_1) {
                                    ++catk->captures;
                                    catk->most_recent_capture_shottype = GAME_MANAGER.character * 2 + GAME_MANAGER.shottype;
                                }
                                ++GAME_MANAGER.__dword_28;
                            }
                        }
                        ENEMY_MANAGER.__spellcard_capture = 0;
                    }
                    STAGE.__set_by_spellcard_ecl_80 = 0;
                    break;
                }
                case 95: { // enemy_create
                    Float3 spawn_position;
                    spawn_position.x = ParseFloatArg(1);
                    spawn_position.y = ParseFloatArg(2);
                    spawn_position.z = ParseFloatArg(3);
                    ENEMY_MANAGER.create_enemy(IntArg(0), &spawn_position, ShortArg(8), ShortArg(9), IntArg(5));
                    break;
                }
                case 96: { // enemy_kill_all
                    Enemy *restrict enemy_ptr = ENEMY_MANAGER.enemies;
                    for (size_t ii = 0; ii < countof(ENEMY_MANAGER.enemies) - 1; ++ii, ++enemy_ptr) {
                        if (!enemy_ptr->active) continue;
                        if (enemy_ptr->is_boss) continue;
                        enemy_ptr->life = 0;
                        if (enemy_ptr->can_interact) continue; // wtf?
                        if (int32_t death_sub = enemy_ptr->death_callback_sub; death_sub >= 0) {
                            this->init_sub_context(enemy_ptr->current_context, death_sub);
                            enemy_ptr->death_callback_sub = -1;
                        }
                    }
                    break;
                }
                case 97: { // anm_set_main
                    enemy->set_main_anm(IntArg(0) + 256);
                    break;
                }
                case 98: // anm_set_poses
                    memcpy(enemy->anm_ex.as_array, current_instruction->args, sizeof(enemy->anm_ex.as_array));
                    enemy->anm_ex_flags = -1;
                    break;
                case 99: { // anm_set_slot
                    AnmVM *restrict vm = &enemy->vms[IntArg(0)];
                    int32_t anm_index = IntArg(1) + 256;
                    vm->script_number = anm_index;
                    ANM_MANAGER_PTR->set_vm_script(vm, ANM_MANAGER_PTR->scripts[anm_index]);
                    break;
                }
                case 100: // anm_death_effects
                    __builtin_memcpy(enemy->death_anms.as_array, current_instruction->args, sizeof(enemy->death_anms.as_array));
                    break;
                case 101: // boss_set
                    if (
                        int32_t boss_id = IntArg(0);
                        expect(enemy->is_boss = GUI.boss_present = boss_id >= 0, true)
                    ) {
                        enemy->boss_id = boss_id;
                        ENEMY_MANAGER.bosses[boss_id] = enemy;
                        GUI.__boss_health_bar_24 = 1.0f;
                    } else {
                        ENEMY_MANAGER.bosses[enemy->boss_id] = NULL;
                    }
                    break;
                case 102: { // spellcard_effect
                    int32_t effect_id = IntArg(0);
                    Effect*& new_effect = enemy->__effect_array[enemy->__effect_index++];
                    new_effect = EFFECT_MANAGER.spawn_effect(13, &enemy->position, 1, (*EffectColorTablePtr)[effect_id]);
                    new_effect->position = *(Float3*)&FloatArg(1);
                    enemy->__effect_distance = FloatArg(4);
                    break;
                }
                case 103: // enemy_set_hitbox
                    enemy->hitbox_dimensions = *(Float3*)current_instruction->args;
                    break;
                case 104: // enemy_flag_collision
                    enemy->contact_damage = OneBitArg(0);
                    break;
                case 105: // enemy_flag_can_take_damage
                    enemy->can_take_damage = OneBitArg(0);
                    break;
                case 106: // effect_sound
                    SOUND_MANAGER.play_sound_centered(IntArg(0));
                    break;
                case 107: // enemy_flag_death
                    enemy->death_flags = ThreeBitArg(0);
                    break;
                case 108: // death_callback_sub
                    enemy->death_callback_sub = IntArg(0);
                    break;
                case 109: // enemy_interrupt_set
                    enemy->interrupts[IntArg(1)] = IntArg(0);
                    break;
                case 110: // enemy_interrupt
                    enemy->run_interrupt = IntArg(0);
                RunInterrupt:
                    IndexInstr(current_instruction->offset_to_next);
                    if (expect(!enemy->disable_call_stack, true)) {
                        int32_t context_depth = enemy->stack_depth;
                        enemy->saved_context_stack[context_depth] = current_context;
                        if (expect(context_depth < 7, true)) ++enemy->stack_depth;
                    }
                    this->init_sub_context(current_context, enemy->interrupts[enemy->run_interrupt]);
                    enemy->run_interrupt = -1;
                    break;
                case 111: // enemy_life_set
                    enemy->life = enemy->max_life = IntArg(0);
                    break;
                case 112: // boss_timer_set
                    enemy->boss_timer = (Timer){ -999, 0.0f, IntArg(0) };
                    break;
                case 113: // life_callback_threshold
                    enemy->life_callback_threshold = IntArg(0);
                    break;
                case 114: // life_callback_sub
                    enemy->life_callback_sub = IntArg(0);
                    break;
                case 115: // timer_callback_threshold
                    enemy->timer_callback_threshold = IntArg(0);
                    //enemy->boss_timer = (Timer){ -999, 0.0f, 0 };
                    //enemy->boss_timer = static_default_timer<0>;
                    enemy->boss_timer.set_default();
                    break;
                case 116: // timer_callback_sub
                    enemy->timer_callback_sub = IntArg(0);
                    break;
                case 117: // enemy_flag_interactable
                    enemy->can_interact = OneBitArg(0);
                    break;
                case 118: // effect_particle
                    //EFFECT_MANAGER.spawn_effect(IntArg(0), &enemy->position, IntArg(1), (D3DCOLOR) { .as_int = IntArg(2) });
                    EFFECT_MANAGER.spawn_effect(IntArg(0), &enemy->position, IntArg(1), IntArg(2));
                    break;
                case 119: { // drop_items
                    const bool spawn_power = GAME_MANAGER.current_power < 128;
                    for (size_t ii = 0, ii_max = IntArg(0); ii < ii_max; ++ii) {
                        Float3 spawn_position = enemy->position;
                        spawn_position.x += RNG.rand_float() * 144.0f - 72.0f;
                        spawn_position.y += RNG.rand_float() * 144.0f - 72.0f;
                        int32_t item_type = spawn_power ? (ii == 0 ? BigPowerItem : PowerItem) : PointItem;
                        ITEM_MANAGER.spawn_item(&spawn_position, item_type, 0);
                    }
                    break;
                }
                case 120: // anm_flag_rotation
                    enemy->anm_rotate = OneBitArg(0);
                    break;
                case 121: // ins_call
                    if (ins_call func = this->get_extension_ins(IntArg(0))) {
                        func(enemy, current_instruction);
                    }
                    break;
                case 122: // ins_repeat
                    current_context.func = this->get_extension_ins(IntArg(0));
                    break;
                case 123: // time_set
                    current_context.time.increment(ParseIntArg(0));
                    break;
                case 124: // drop_item_id
                    ITEM_MANAGER.spawn_item(&enemy->position, IntArg(0), 0);
                    break;
                case 125: // std_unpause
                    STAGE.unpause_flag = true;
                    break;
                case 126: // boss_set_life_count
                    GUI.__ecl_set_lives = IntArg(0);
                    GAME_MANAGER.__int_1A6C += 708; // ???
                    break;
                case 128: // anm_interrupt_main
                    enemy->primary_vm.pending_interrupt_label = ShortArg(0);
                    break;
                case 129: // anm_interrupt_slot
                    enemy->vms[IntArg(0)].pending_interrupt_label = ShortArg(3);
                    break;
                case 130: // enemy_flag_disable_call_stack
                    enemy->disable_call_stack = OneBitArg(0);
                    break;
                case 131: { // bullet_rank_influence
                    /*enemy->bullet_rank.speed_low = FloatArg(0);
                    enemy->bullet_rank.speed_high = FloatArg(1);
                    enemy->bullet_rank.amount1_low = ShortArg(4);
                    enemy->bullet_rank.amount1_high = ShortArg(6);
                    enemy->bullet_rank.amount2_low = ShortArg(8);
                    enemy->bullet_rank.amount2_high = ShortArg(10);*/
                    __m128i amount_args = *(__m128i*)&ShortArg(4);
                    amount_args = _mm_packs_epi32(amount_args, amount_args);
                    _mm_storel_pi((__m64*)current_instruction->args, bitcast<__m128>(amount_args));
                    *(__m128i*)&enemy->bullet_rank = amount_args;
                    break;
                }
                case 132: // enemy_flag_invisible
                    enemy->invisible = OneBitArg(0);
                    break;
                case 133: // boss_timer_clear
                    enemy->timer_callback_sub = enemy->death_callback_sub;
                    //enemy->boss_timer = (Timer){ -999, 0.0f, 0 };
                    //enemy->boss_timer = static_default_timer<0>;
                    enemy->boss_timer.set_default();
                    break;
                case 134: // laser_clear_all
                    memset(enemy->lasers, 0, sizeof(enemy->lasers));
                    break;
                case 135: // spellcard_flag_timeout
                    enemy->timeout_spell = OneBitArg(0);
                    break;
            }
        }
        float game_speed = SUPERVISOR.game_speed;
        switch (enemy->move_mode) {
            default: unreachable;
            case Velocity: {
                /*float angle = enemy->rotation * game_speed + enemy->direction;
                __asm__(
                    "FLDS %[TWO_PI] \n"
                    "FLDS %[ANGLE] \n"
                    "FPREM1 \n"
                    "FSTPS %[ANGLE] \n"
                    "FFREEP %%st(0) \n"
                    : [ANGLE] "+m"(angle)
                    : [TWO_PI] "m"(TWO_PI_f)
                );*/
                float angle = reduce_angle_new(enemy->direction, enemy->rotation * game_speed);
                enemy->direction = angle;
                float velocity = enemy->acceleration * game_speed + enemy->speed;
                enemy->speed = velocity;
                enemy->axis_speed.x = zcosf(angle) * velocity;
                enemy->axis_speed.y = zsinf(angle) * velocity;
                enemy->axis_speed.z = 0.0f;
                break;
            }
            case Interpolate: {
                Timer *restrict interp_timer = &enemy->move_interp_timer;
                interp_timer->decrement(1);
                float interp_speed = (interp_timer->subframe + (float)interp_timer->current) / (float)enemy->move_interp_start_time;
                interp_speed = __min(interp_speed, 1.0f);
                switch (enemy->interp_mode) {
                    default: unreachable;
                    case DecelerateFast:
                        interp_speed *= interp_speed;
                        [[fallthrough]];
                    case Decelerate:
                        interp_speed *= interp_speed;
                        [[fallthrough]];
                    case Constant:
                        interp_speed = 1.0f - interp_speed;
                        break;
                    case Accelerate:
                        interp_speed = 1.0f - interp_speed;
                        interp_speed *= interp_speed;
                        break;
                    case AccelerateFast:
                        interp_speed = 1.0f - interp_speed;
                        interp_speed *= interp_speed;
                        interp_speed *= interp_speed;
                        break;
                }
                vec<float, 4> interp_temp = *(unaligned vec<float, 4>*)&enemy->move_interp;
                vec<float, 4> interp_start = *(unaligned vec<float, 4>*)&enemy->move_interp_start_pos;
                vec<float, 4> interp_temp2 = (interp_temp * interp_speed) + interp_start - interp_temp;
                *(unaligned vec<float, 4>*)&enemy->axis_speed = interp_temp2;
                vec<float, 2> interp_temp3;
                interp_temp3[0] = interp_temp2[0];
                interp_temp3[1] = interp_temp2[1];
                vec<double, 2> interp_temp4 = convertvec(interp_temp3, vec<double, 2>);
                enemy->direction = zatan2f(interp_temp4[1], interp_temp4[0]);
                if (enemy->move_interp_timer.current > 0) {
                    interp_start += interp_temp;
                    enemy->position.x = interp_start[0];
                    enemy->position.y = interp_start[1];
                    enemy->position.z = interp_start[2];
                    __builtin_memset(&enemy->axis_speed, 0, sizeof(enemy->axis_speed));
                    //enemy->axis_speed = (Float3){ 0.0f, 0.0f, 0.0f };
                }
                break;

                //__v4sf interp_temp = *(__m128_u*)&enemy->move_interp;
                //__v4sf interp_start = *(__m128_u*)&enemy->move_interp_start_pos;
                //__v4sf interp_temp2 = (interp_temp * interp_speed) + interp_start - interp_temp;
                //*(__m128_u*)&enemy->axis_speed = interp_temp2;
                //enemy->direction = zatan2f(interp_temp2[1], interp_temp2[0]);
                //if (enemy->move_interp_timer.current > 0) {
                //    interp_start += interp_temp;
                //    enemy->position.x = interp_start[0];
                //    enemy->position.y = interp_start[1];
                //    enemy->position.z = interp_start[2];
                //    memset(enemy->axis_speed.as_array, 0, sizeof(enemy->axis_speed.as_array));
                //    //enemy->axis_speed = (Float3){ 0.0f, 0.0f, 0.0f };
                //}
                //break;
            }
            case PerAxisSpeed:
                break;
        }
        if (enemy->life > 0) {
            if (enemy->shoot_interval > 0) {
                //enemy->shoot_interval_timer.previous = enemy->shoot_interval_timer.current;
                //SUPERVISOR.tick_timer(&enemy->shoot_interval_timer.current, &enemy->shoot_interval_timer.subframe);
                //if (enemy->shoot_interval_timer.current >= enemy->shoot_interval) {
                if (enemy->shoot_interval_timer.tick() >= enemy->shoot_interval) {
                    vec<float, 4> position = *(unaligned vec<float, 4>*)&enemy->position;
                    position += *(unaligned vec<float, 4>*)&enemy->shoot_offset;
                    unaligned vec<float, 4>& bullet_position = *(unaligned vec<float, 4>*)&enemy->bullet_properties.position;
                    bullet_position[0] = position[0];
                    bullet_position[1] = position[1];
                    bullet_position[2] = position[2];
                    //enemy->bullet_properties.position.x = enemy->position.x + enemy->shoot_offset.x;
                    //enemy->bullet_properties.position.y = enemy->position.y + enemy->shoot_offset.y;
                    //enemy->bullet_properties.position.z = enemy->position.z + enemy->shoot_offset.z;
                    BULLET_MANAGER.shoot_bullets(&enemy->bullet_properties);
                    //enemy->shoot_interval_timer = (Timer){ -999, 0.0f, 0 };
                    //enemy->shoot_interval_timer = static_default_timer<0>;
                    enemy->shoot_interval_timer.set_default();
                }
            }
            if (enemy->anm_ex.Left >= 0) {
                int8_t anm_ex_pose = 1 + (enemy->axis_speed.x > 0.0f) - (enemy->axis_speed.x == 0.0f);
                int8_t anm_ex_flags = enemy->anm_ex_flags;
                if (anm_ex_flags != anm_ex_pose) {
                    int32_t pose;
                    switch (anm_ex_pose) {
                        default: unreachable;
                        case 0: // default
                            switch (anm_ex_flags) {
                                case -1:
                                    pose = enemy->anm_ex.Default;
                                    break;
                                case 1:
                                    pose = enemy->anm_ex.FarLeft;
                                    break;
                                default:
                                    pose = enemy->anm_ex.FarRight;
                                    break;
                            }
                            break;
                        case 1: // left
                            pose = enemy->anm_ex.Left;
                            break;
                        case 2: // right
                            pose = enemy->anm_ex.Right;
                            break;
                    }
                    enemy->set_main_anm(pose + 256);
                    enemy->anm_ex_flags = anm_ex_pose;
                }
            }
            if (ins_call func = current_context.func) {
                func(enemy, NULL);
            }
        }
        current_context.current_instr = current_instruction;
        //current_context.time.previous = current_instruction->time;
        //SUPERVISOR.tick_timer(&current_context.time.current, &current_context.time.subframe);
        current_context.time.tick();
        return 0;
    }
};

extern "C" {
    // 0x487E50
    extern EnemyController ENEMY_CONTROLLER asm("_ENEMY_CONTROLLER");
}

// 0x4123E0
dllexport int32_t cdecl enemy_manager_on_tick(EnemyManager *const restrict enemy_manager_arg) asm("{[codecave:ENEMY_MANAGER_on_tick]}");
dllexport int32_t cdecl enemy_manager_on_tick(EnemyManager *const restrict enemy_manager_arg) {
    enemy_manager_arg->run_timeline();
    enemy_manager_arg->enemy_count_real = 0;
    //Enemy* restrict current_enemy = enemy_manager_arg->enemies;
    //for (size_t ii = 0; ii < countof(enemy_manager_arg->enemies); ++ii, ++current_enemy) {
    bool odd_index = true;
    /*for (
        Enemy* restrict current_enemy = enemy_manager_arg->enemies;
        current_enemy != array_end_addr(enemy_manager_arg->enemies);
        ++current_enemy
    ) {*/
    //Enemy* restrict current_enemy = enemy_manager_arg->enemies;
    uintptr_t ii = (uintptr_t)&enemy_manager_arg->enemies;
    assume(ii < offsetof(EnemyManager, dummy_enemy_for_failed_spawns));
    for (
        ;
        ii < offsetof(EnemyManager, dummy_enemy_for_failed_spawns);
        ii += sizeof(Enemy)
    ) {
        Enemy* restrict current_enemy = (Enemy*)((uintptr_t)enemy_manager_arg + ii);
        if (!current_enemy->active) continue;
        ++enemy_manager_arg->enemy_count_real;
        current_enemy->main_movement();
        current_enemy->enfore_move_limit();
        {
            AnmLoadedSprite *restrict sprite = current_enemy->primary_vm.sprite;
            bool is_visible = GAME_MANAGER.check_visibility(current_enemy->position.x, current_enemy->position.y, sprite->width_px, sprite->height_px);
            if (current_enemy->__offscreen_related != is_visible) {
                if (!(current_enemy->__offscreen_related = is_visible)) {
                    current_enemy->kill();
                    continue;
                }
            }
        }
        if (current_enemy->life_callback_threshold >= 0) {
            current_enemy->check_life_callback();
        }
        if (current_enemy->timer_callback_threshold >= 0) {
            current_enemy->check_timer_callback();
        }
        if (expect(ENEMY_CONTROLLER.run_ecl(current_enemy) == -1, false)) {
            current_enemy->active = false;
            current_enemy->kill();
            continue;
        }
        current_enemy->enfore_move_limit();
        current_enemy->primary_vm.color = current_enemy->__color_CFC;
        ANM_MANAGER_PTR->run_anm(&current_enemy->primary_vm);
        current_enemy->__color_CFC = current_enemy->primary_vm.color;
        for (size_t jj = 0; jj < countof(current_enemy->vms); ++jj) {
            AnmVM* restrict slotted_vm = &current_enemy->vms[jj];
            if (slotted_vm->script_number >= 0) {
                if (ANM_MANAGER_PTR->run_anm(slotted_vm)) {
                    slotted_vm->script_number = -1;
                }
            }
        }
        int32_t idk = 0;
        if (current_enemy->__offscreen_related && !current_enemy->invisible) {
            int32_t starting_life = current_enemy->life;
            if (current_enemy->contact_damage && current_enemy->can_interact) {
                vec<float, 4> size = *(unaligned vec<float, 4>*)&current_enemy->hitbox_dimensions;
                size *= TWO_THIRDS_f;
                if (PLAYER.check_collision(&current_enemy->position, (Float3*)&size) == 1) {
                    if (current_enemy->can_interact && !current_enemy->is_boss) {
                        current_enemy->life -= 10;
                    }
                }
                /*Float3 size;
                size.x = current_enemy->hitbox_dimensions.x * TWO_THIRDS_f;
                size.y = current_enemy->hitbox_dimensions.y * TWO_THIRDS_f;
                size.z = current_enemy->hitbox_dimensions.z * TWO_THIRDS_f;
                if (PLAYER.check_collision(&current_enemy->position, &size) == 1) {
                    if (current_enemy->can_interact && !current_enemy->is_boss) {
                        current_enemy->life -= 10;
                    }
                }*/
            }
            if (current_enemy->can_interact) {
                int32_t damage = PLAYER.check_bullet_collisions(&current_enemy->position, &current_enemy->hitbox_dimensions, &idk);
                damage = __max(damage, 70);
                GAME_MANAGER.score += (damage / 5) * 10;
                if (enemy_manager_arg->__spellcard_capture) {
                    if (idk == 0) {
                        if (damage > 7) {
                            damage /= 7;
                        } else if (damage != 0) {
                            damage = 1;
                        }
                    } else if (enemy_manager_arg->__dword_EE5C4) {
                        if (damage > 3) {
                            damage /= 3;
                        } else if (damage != 0) {
                            damage = 1;
                        }
                    } else {
                        damage = 0;
                    }
                }
                if (current_enemy->can_take_damage) {
                    current_enemy->life -= damage;
                }
                if (PLAYER.position_of_last_enemy_hit.y < current_enemy->position.y) {
                    PLAYER.position_of_last_enemy_hit = current_enemy->position;
                }
            }
            if (current_enemy->life < 0 && current_enemy->can_interact) {
                current_enemy->timer_callback_threshold = current_enemy->life_callback_threshold = -1;
                switch (current_enemy->death_flags) {
                    case 3:
                        current_enemy->life = 1;
                        current_enemy->can_take_damage = false;
                        current_enemy->death_flags = 0;
                        GUI.boss_present = 0;
                        EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.one, &current_enemy->position, 3, PackD3DCOLOR(255, 255, 255, 255));
                        //EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.one, &current_enemy->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                        //EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.one, &current_enemy->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                        //EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.one, &current_enemy->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                        break;
                    case 1:
                        current_enemy->can_interact = false;
                        goto IncScore;
                    case 0:
                        current_enemy->active = false;
                    IncScore:
                        GAME_MANAGER.score += current_enemy->score;
                        if (current_enemy->is_boss) {
                            GUI.boss_present = 0;
                            current_enemy->__deactivate_effects();
                        }
                        [[fallthrough]];
                    case 2:
                        int16_t item_drop = current_enemy->item_drop;
                        if (item_drop >= 0) {
                            EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.two + 4, &current_enemy->position, 3, PackD3DCOLOR(255, 255, 255, 255));
                            ITEM_MANAGER.spawn_item(&current_enemy->position, item_drop, idk);
                        } else if (item_drop == -1) {
                            if (!(enemy_manager_arg->random_item_spawn_index++ % 3)) {
                                uint16_t table_index = enemy_manager_arg->random_item_table_index;
                                EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.two + 4, &current_enemy->position, 6, PackD3DCOLOR(255, 255, 255, 255));
                                ITEM_MANAGER.spawn_item(&current_enemy->position, RandomItemIDs_LookupTable[table_index++], idk);
                                if (table_index >= countof(RandomItemIDs_LookupTable)) table_index = 0;
                            }
                        }
                        if (current_enemy->is_boss && enemy_manager_arg->__spellcard_capture) {
                            BULLET_MANAGER.cancel_bullets_spell_bonus(12800);
                            //BULLET_MANAGER.cancel_bullets_spell_bonus(12800, 0);
                        }
                        current_enemy->life = 0;
                        break;
                }
                //SOUND_MANAGER.play_sound_centered((ii & 1) + 2);
                SOUND_MANAGER.play_sound_centered((odd_index ^= true) + 2);
                EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.one, &current_enemy->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                EFFECT_MANAGER.spawn_effect(current_enemy->death_anms.two, &current_enemy->position, 4, PackD3DCOLOR(255, 255, 255, 255));
                int16_t death_callback_sub = current_enemy->death_callback_sub;
                if (death_callback_sub >= 0) {
                    current_enemy->reset_bullet_rank();
                    current_enemy->stack_depth = 0;
                    ENEMY_CONTROLLER.init_sub_context(current_enemy->current_context, death_callback_sub);
                    current_enemy->death_callback_sub = -1;
                }
            }
            if (current_enemy->is_boss) {
                if (!GUI.msg_is_active()) {
                    GUI.__boss_health_bar_24 = (float)current_enemy->max_life / (float)current_enemy->life;
                }
            }
            if (current_enemy->__byte_E41) {
                --current_enemy->__byte_E41;
                current_enemy->primary_vm.__unknown_flag_D = false;
            } else if (starting_life <= current_enemy->life) {
                SOUND_MANAGER.play_sound_centered(20);
                current_enemy->primary_vm.__unknown_flag_D = true;
                current_enemy->__byte_E41 = 4;
            } else {
                current_enemy->primary_vm.__unknown_flag_D = false;
            }
        }
        current_enemy->ENEMY_spin_effects();
        if (!GAME_MANAGER.__byte_2C) {
            current_enemy->boss_timer.tick();
            //current_enemy->boss_timer.previous = current_enemy->boss_timer.current;
            //SUPERVISOR.tick_timer(&current_enemy->boss_timer.current, &current_enemy->boss_timer.subframe);
        }
    }
    enemy_manager_arg->timeline_time.tick();
    //enemy_manager->timeline_time.previous = enemy_manager->timeline_time.current;
    //SUPERVISOR.tick_timer(&enemy_manager->timeline_time.current, &enemy_manager->timeline_time.subframe);
    return 1;
}

forceinline void EnemyManager::wtf_zun_why(void) {
    Enemy* restrict enemy = this->enemies;
    for (size_t ii = 0; ii < countof(this->enemies); ++ii, ++enemy) {
        if (!enemy->active) continue;
        if (enemy->is_boss) continue;
        enemy->life = 0;
        if (enemy->can_interact) continue;
        int32_t death_callback_sub = enemy->death_callback_sub;
        if (death_callback_sub >= 0) {
            ENEMY_CONTROLLER.init_sub_context(enemy->current_context, death_callback_sub);
            enemy->death_callback_sub = -1;
        }
    }
}

forceinline void Enemy::generic_callback(void) {
    this->timer_callback_sub = this->death_callback_sub;
    this->reset_bullet_rank();
    this->stack_depth = 0;
    ENEMY_MANAGER.wtf_zun_why();
}

int32_t Enemy::check_life_callback(void) {
    if (this->life >= this->life_callback_threshold) {
        return false;
    } else {
        this->life = this->life_callback_threshold;
        ENEMY_CONTROLLER.init_sub_context(this->current_context, this->life_callback_sub);
        this->life_callback_threshold = -1;
        this->generic_callback();
        return true;
    }
}

forceinline int32_t Enemy::check_timer_callback(void) {
    int32_t timer_callback_threshold = this->timer_callback_threshold;
    int32_t current_boss_timer = this->boss_timer.current;
    if (this->is_boss) {
        GUI.__read_by_spellcard_ecl = (timer_callback_threshold - current_boss_timer) / 60;
    }
    if (expect(current_boss_timer < timer_callback_threshold, true)) return 0;
    if (int32_t life_callback_threshold = this->life_callback_threshold;
        life_callback_threshold > 0
    ) {
        this->life = life_callback_threshold;
        this->life_callback_threshold = 0;
    }
    ENEMY_CONTROLLER.init_sub_context(this->current_context, this->timer_callback_sub);
    this->timer_callback_threshold = -1;
    this->timer_callback_sub = this->death_callback_sub;
    //this->boss_timer = (Timer){ -999, 0.0f, 0 };
    //this->boss_timer = static_default_timer<0>;
    this->boss_timer.set_default();
    if (this->timeout_spell) {
        ENEMY_MANAGER.__dword_EE5C4 = 0;
        ENEMY_MANAGER.__spellcard_capture += ENEMY_MANAGER.__spellcard_capture != 0;
        //BULLET_MANAGER.cancel_bullets(false);
        BULLET_MANAGER.cancel_bullets();
    }
    this->generic_callback();
    return 1;
}

Enemy* EnemyManager::create_enemy(int16_t SubID, Float3* position, int16_t life, int8_t item, int32_t score) {
    Enemy* restrict new_enemy = this->enemies;
    for (size_t ii = 0; ii < countof(this->enemies); ++ii, ++new_enemy) {
        if (!new_enemy->active) {
            *new_enemy = this->template_enemy;
            if (life >= 0) new_enemy->life = life;
            new_enemy->position = *position;
            ENEMY_CONTROLLER.init_sub_context(new_enemy->current_context, SubID);
            ENEMY_CONTROLLER.run_ecl(new_enemy);
            new_enemy->__color_CFC = new_enemy->primary_vm.color;
            new_enemy->item_drop = item;
            if (life >= 0) new_enemy->life = life;
            if (score >= 0) new_enemy->score = score;
            new_enemy->max_life = new_enemy->life;
            break;
        }
    }
    return new_enemy;
}

void EnemyManager::run_timeline(void) {
    TimelineInstruction* restrict current_instruction = this->timeline_instr;
    if (current_instruction == NULL) {
        current_instruction = ENEMY_CONTROLLER.current_timeline_instruction;
    }
    if (!GUI.msg_is_active()) {
        int32_t current_time = this->timeline_time.current;
        if (current_time != this->timeline_time.previous) {
            if (current_time % (2400 - GAME_MANAGER.lives_remaining * 240) == 0) {
                GAME_MANAGER.modify_rank(100);
            }
        }
    }
    for (
        ;
        current_instruction->time >= 0;
        IndexInstr(current_instruction->offset_to_next)
    ) {
        if (current_instruction->time == this->timeline_time.current) {
            switch (auto op_index = current_instruction->opcode) {
                {
                    int32_t score;
                    int32_t item;
                    int32_t life;
                case 0: // enemy_create
                case 2: // enemy_create_mirror
                case 4: // enemy_create_random
                case 6: // enemy_create_mirror_random
                    if (GUI.boss_present) continue;
                    score = IntArg(4);
                    item = ShortArg(7);
                    life = ShortArg(6);
                    goto CreateEnemy;
                case 1: // enemy_create_blank
                case 3: // enemy_create_blank_mirror
                case 5: // enemy_create_blank_random
                case 7: // enemy_create_blank_mirror_random
                    if (GUI.boss_present) continue;
                    score = -1;
                    item = -1;
                    life = -1;
                CreateEnemy:
                    Float3 position = Float3Arg(0);
                    if (expect(op_index & 0b100, false)) {
                        if (position.x <= -990.0f) position.x = RNG.rand_float() * GAME_MANAGER.__float2_1A54.x;
                        if (position.y <= -990.0f) position.y = RNG.rand_float() * GAME_MANAGER.__float2_1A54.y;
                        if (position.z <= -990.0f) position.z = RNG.rand_float() * 800.0f;
                    }
                    Enemy* restrict new_enemy = this->create_enemy(current_instruction->first_arg, &position, life, item, score);
                    if (op_index & 0b010) new_enemy->mirrored = true;
                    break;
                    /*Float3 *restrict position_ptr = &Float3Arg(0);
                    Float3 position;
                    if (op_index & 0b100) {
                        position = *position_ptr;
                        if (position.x <= -990.0f) position.x = RNG.rand_float() * GAME_MANAGER.__float2_1A54.x;
                        if (position.y <= -990.0f) position.y = RNG.rand_float() * GAME_MANAGER.__float2_1A54.y;
                        if (position.z <= -990.0f) position.z = RNG.rand_float() * 800.0f;
                        position_ptr = &position;
                    }
                    Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, position_ptr, life, item, score);
                    if (op_index & 0b010) new_enemy->mirrored = true;
                    break;*/
                }
                //case 0: // enemy_create
                //    if (!GUI.boss_present) {
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &Float3Arg(0), ShortArg(6), ShortArg(7), IntArg(5));
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                //case 1: // enemy_create_blank
                //    if (!GUI.boss_present) {
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &Float3Arg(0), -1, -1, -1);
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                //case 2: // enemy_create_mirror
                //    if (!GUI.boss_present) {
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &Float3Arg(0), ShortArg(6), ShortArg(7), IntArg(5));
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                //case 3: // enemy_create_blank_mirror
                //    if (!GUI.boss_present) {
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &Float3Arg(0), -1, -1, -1);
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                //case 4: // enemy_create_random
                //    if (!GUI.boss_present) {
                //        Float3 position;
                //        float temp = FloatArg(0);
                //        position.x = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.x);
                //        temp = FloatArg(1);
                //        position.y = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.y);
                //        temp = FloatArg(2);
                //        position.z = temp > -990.0f ? temp : (RNG.rand_float() * 800.0f);
                //
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &position, ShortArg(6), ShortArg(7), IntArg(5));
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                //case 5: // enemy_create_blank_random
                //    if (!GUI.boss_present) {
                //        Float3 position;
                //        float temp = FloatArg(0);
                //        position.x = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.x);
                //        temp = FloatArg(1);
                //        position.y = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.y);
                //        temp = FloatArg(2);
                //        position.z = temp > -990.0f ? temp : (RNG.rand_float() * 800.0f);
                //
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &position, -1, -1, -1);
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                //case 6: // enemy_create_mirror_random
                //    if (!GUI.boss_present) {
                //        Float3 position;
                //        float temp = FloatArg(0);
                //        position.x = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.x);
                //        temp = FloatArg(1);
                //        position.y = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.y);
                //        temp = FloatArg(2);
                //        position.z = temp > -990.0f ? temp : (RNG.rand_float() * 800.0f);
                //
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &position, ShortArg(6), ShortArg(7), IntArg(5));
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                //case 7: // enemy_create_blank_mirror_random
                //    if (!GUI.boss_present) {
                //        Float3 position;
                //        float temp = FloatArg(0);
                //        position.x = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.x);
                //        temp = FloatArg(1);
                //        position.y = temp > -990.0f ? temp : (RNG.rand_float() * GAME_MANAGER.__float2_1A54.y);
                //        temp = FloatArg(2);
                //        position.z = temp > -990.0f ? temp : (RNG.rand_float() * 800.0f);
                //
                //        Enemy* restrict new_enemy = ENEMY_MANAGER.create_enemy(current_instruction->first_arg, &position, -1, -1, -1);
                //        if (op_index & 1) new_enemy->mirrored = true;
                //    }
                //    break;
                case 8: { // read_msg
                    int32_t msg_index = current_instruction->first_arg;
                    if (expect(msg_index == 1 &&
                        GAME_MANAGER.difficulty == 0 &&
                        GAME_MANAGER.current_stage == 5
                    , false)) {
                        msg_index = 3;
                    }
                    msg_index += GAME_MANAGER.character * 10;
                    GUI.__read_msg(msg_index);
                    break;
                }
                case 9: // wait_msg
                    if (!GUI.msg_is_waiting()) {
                        this->timeline_time.decrement(1);
                        this->timeline_instr = current_instruction;
                        return;
                    }
                    break;
                case 10: // boss_interrupt
                    this->bosses[IntArg(0)]->run_interrupt = IntArg(1);
                    break;
                case 11: // player_power
                    GAME_MANAGER.current_power = current_instruction->first_arg;
                    break;
                case 12: { // boss_wait
                    Enemy* boss = this->bosses[current_instruction->first_arg];
                    if (boss && boss->active) {
                        this->timeline_time.decrement(1);
                        this->timeline_instr = current_instruction;
                        return;
                    }
                    break;
                }
            }
        } else if (current_instruction->time > this->timeline_time.current) {
            break;
        }
    }
    if (!GUI.msg_is_active()) {
        ++GAME_MANAGER.__int_1A6C;
    }
}

void Player::die(void) {
    ENEMY_MANAGER.__dword_EE5C4 = 0;
    EFFECT_MANAGER.spawn_effect(12, &this->position, 1, PackD3DCOLOR(255, 64, 64, 255));
    EFFECT_MANAGER.spawn_effect(6, &this->position, 16, PackD3DCOLOR(255, 255, 255, 255));
    this->__byte_9E0 = 2;
    //this->__timer_75B4 = (Timer){ -999, 0, 0 };
    //this->__timer_75B4 = static_default_timer<0>;
    this->__timer_75B4.set_default();
    SOUND_MANAGER.play_sound_centered(4);
    ++GAME_MANAGER.__dword_20;
    for (size_t ii = 0; ii < countof(this->__timer_array_9B8); ++ii) {
        //this->__timer_array_9B8[ii] = (Timer){ -999, 0, 2 };
        //this->__timer_array_9B8[ii] = static_default_timer<2>;
        this->__timer_array_9B8[ii].set_default(2);
    }
}

forceinline int32_t Player::check_bullet_collisions(Float3* restrict position, Float3* restrict size, int32_t* restrict hit_by_bomb_out) {
    
    int32_t counter = 0;

    BoundingBoxSource bullet_box;
    bullet_box.make_from_pos_size(*position, *size);

    if (hit_by_bomb_out != NULL) *hit_by_bomb_out = false;
    //PlayerBullet *restrict player_bullet = this->bullets;
    for (
        //size_t ii = 0;
        PlayerBullet *restrict player_bullet = this->bullets;
        //uintptr_t ii = 0;
        
        //ii < countof(this->bullets);
        player_bullet != array_end_addr(this->bullets);
        //expect(player_bullet != array_end_addr(this->bullets), true);
        //ii < (offsetof(Player, __timer_75A8) - offsetof(Player, bullets));
        
        //++ii, ++player_bullet
        ++player_bullet
        //ii += sizeof(PlayerBullet)
    ) {
        //PlayerBullet *restrict player_bullet = (PlayerBullet*)((uintptr_t)this->bullets + ii);
        switch (int16_t state = player_bullet->state;
            expect_chance(state, 0, 0.5)
        ) {
            case 0:
                continue;
            default:
                if (player_bullet->__short_150 != 2) {
                    continue;
                }
                [[fallthrough]];
            case 1:
                BoundingBoxDest dest_box;
                dest_box.make_from_pos_size(player_bullet->position, player_bullet->size);

                if (bullet_box.test_not_intersect(dest_box)) {
                    continue;
                }

                int16_t temp_short_14C = player_bullet->__short_14C;
                {
                    int16_t temp_short_14C_2 = temp_short_14C;
                    if (this->__player_inner_d_75C8.__dword_0) {
                        //temp_short_14C_2 /= 3;
                        //if (!temp_short_14C_2) temp_short_14C_2 = 1;
                        //temp_short_14C_2 = __max(temp_short_14C_2 / 3, 1);
                        temp_short_14C_2 = temp_short_14C_2 / 3 ? temp_short_14C_2 / 3 : 1;
                    }
                    counter += temp_short_14C_2;
                }
                switch (player_bullet->__short_150) {
                    case 2: {
                        //player_bullet->__short_14C = __max(temp_short_14C / 4, 1);
                        player_bullet->__short_14C = temp_short_14C / 4 ? temp_short_14C / 4 : 1;
                        switch (int32_t size_index = player_bullet->primary_vm.script_number - 1089) {
                            case 0: case 1: case 2: case 3:
                                static constexpr vec<float, 2> bullet_size_lookup_table[] = {
                                    { 32.0f, 32.0f },
                                    { 42.0f, 42.0f },
                                    { 48.0f, 48.0f },
                                    { 48.0f, 48.0f }
                                };
                                *(vec<float, 2>*)&player_bullet->size = bullet_size_lookup_table[size_index];
                        }
                        if (player_bullet->__timer_140.current % 6 == 0) {
                            EFFECT_MANAGER.spawn_effect(5, &player_bullet->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                        }
                    }
                        [[fallthrough]];
                    default:
                        if (state == 1) {
                            ANM_MANAGER_PTR->set_vm_script(&player_bullet->primary_vm, ANM_MANAGER_PTR->scripts[player_bullet->primary_vm.script_number += 32]);
                            EFFECT_MANAGER.spawn_effect(5, &player_bullet->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                            player_bullet->position.z = 0.1f;
                        }
                        player_bullet->state = 2;
                        *(vec<float, 2>*)&player_bullet->__float_128 /= 8.0f;
                        continue;
                    case 3:
                        if (++this->__byte_9E4 % 8 == 0) {
                            Float3 temp_position = {
                                player_bullet->position.x,
                                position->y,
                                position->z
                            };
                            EFFECT_MANAGER.spawn_effect(5, &temp_position, 1, PackD3DCOLOR(255, 255, 255, 255));
                        }
                        continue;
                }
        }
    }
    for (size_t ii = 0; ii < countof(this->__size_array_638); ++ii) {
        if (this->__size_array_638[ii].x <= 0.0f) continue;
        BoundingBoxDest dest_box;
        dest_box.make_from_pos_size(this->__position_array_4B8[ii], this->__size_array_638[ii]);

        if (bullet_box.test_not_intersect(dest_box)) {
            continue;
        }
        {
            int32_t temp_int = this->__dword_array_7B8[ii];
            counter += temp_int;
            this->__dword_array_838[ii] += temp_int;
        }
        if (++this->__byte_9E4 % 4 == 0) {
            EFFECT_MANAGER.spawn_effect(3, position, 1, PackD3DCOLOR(255, 255, 255, 255));
        }
        if (this->__player_inner_d_75C8.__dword_0 && hit_by_bomb_out) {
            *hit_by_bomb_out = true;
        }
    }
    return counter;
    
    /*int32_t counter = 0;
    
    float temp_width = size->x / 2.0f;
    float temp_x_pos = position->x;
    float x_min = temp_x_pos - temp_width;
    float x_max = temp_x_pos + temp_width;

    float temp_height = size->y / 2.0f;
    float temp_y_pos = position->y;
    float y_min = temp_y_pos - temp_height;
    float y_max = temp_y_pos + temp_height;

    PlayerBullet* restrict player_bullet = this->bullets;
    if (idk != NULL) *idk = 0;
    for (size_t ii = 0; ii < countof(this->bullets); ++ii, ++player_bullet) {
        switch (int16_t state = player_bullet->state) {
            default:
                if (player_bullet->__short_150 != 2) {
            case 0:
                    continue;
                }
                [[fallthrough]];
            case 1:
                float rect_width = player_bullet->size.x / 2.0f;
                float rect_x_pos = player_bullet->position.x;
                if ((rect_x_pos - rect_width) > x_max || (rect_x_pos + rect_width) < x_min) continue;

                float rect_height = player_bullet->size.y / 2.0f;
                float rect_y_pos = player_bullet->position.y;
                if ((rect_y_pos - rect_height) > y_max || (rect_y_pos + rect_height) < y_min) continue;

                int16_t temp_short_14C = player_bullet->__short_14C;
                {
                    int16_t temp_short_14C_2 = temp_short_14C;
                    if (this->__player_inner_d_75C8.__dword_0) {
                        temp_short_14C_2 /= 3;
                        if (!temp_short_14C_2) temp_short_14C_2 = 1;
                    }
                    counter += temp_short_14C_2;
                }
                switch (player_bullet->__short_150) {
                    case 2:
                        temp_short_14C /= 4;
                        if (!temp_short_14C) temp_short_14C = 1;
                        player_bullet->__short_14C = temp_short_14C;
                        switch (float new_size = 32.0f; player_bullet->primary_vm.script_number - 1089) {
                            case 2: case 3:
                                new_size += 6.0f;
                            case 1:
                                new_size += 10.0f;
                            case 0:
                                player_bullet->size.x = player_bullet->size.y = 32.0f;
                        }
                        if (player_bullet->__timer_140.current % 6) {
                            EFFECT_MANAGER.spawn_effect(5, &player_bullet->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                        }
                        [[fallthrough]];
                    default:
                        if (state == 1) {
                            ANM_MANAGER_PTR->set_vm_script(&player_bullet->primary_vm, ANM_MANAGER_PTR->scripts[player_bullet->primary_vm.script_number += 32]);
                            EFFECT_MANAGER.spawn_effect(5, &player_bullet->position, 1, PackD3DCOLOR(255, 255, 255, 255));
                            player_bullet->position.z = 0.1f;
                        }
                        player_bullet->state = 2;
                        player_bullet->__float_128 /= 8.0f;
                        player_bullet->__float_12C /= 8.0f;
                        continue;
                    case 3:
                        if (!(++this->__byte_9E4 & 0b111)) {
                            Float3 temp_position = *position;
                            temp_position.x = player_bullet->position.x;
                            EFFECT_MANAGER.spawn_effect(5, &temp_position, 1, PackD3DCOLOR(255, 255, 255, 255));
                        }
                        continue;
                }
        }
    }
    for (size_t ii = 0; ii < countof(this->__size_array_638); ++ii) {
        float rect_width = this->__size_array_638[ii].x;
        if (rect_width <= 0.0f) continue;
        rect_width /= 2.0f;

        float rect_x_pos = this->__position_array_4B8[ii].x;
        if ((rect_x_pos - rect_width) > x_max || (rect_x_pos + rect_width) < x_min) continue;

        float rect_height = this->__size_array_638[ii].y / 2.0f;
        float rect_y_pos = this->__position_array_4B8[ii].y;
        if ((rect_y_pos - rect_height) > y_max || (rect_y_pos + rect_height) < y_min) continue;


        int32_t temp_int = this->__dword_array_7B8[ii];
        counter += temp_int;
        this->__dword_array_838[ii] += temp_int;

        if (!(++this->__byte_9E4 & 0b11)) {
            EFFECT_MANAGER.spawn_effect(3, position, 1, PackD3DCOLOR(255, 255, 255, 255));
        }
        if (this->__player_inner_d_75C8.__dword_0 && idk) {
            ++(*idk);
        }
    }
    return counter;*/
}

#define ScrewyField(pointer, field) \
(*((std::remove_reference_t<decltype((pointer)->field)>*)((uintptr_t)(pointer) + offsetof(std::remove_reference_t<decltype(*(pointer))>, field))))
//(*((decltype((pointer)->field)*)((uintptr_t)(pointer) + offsetof(std::remove_reference_t<decltype(*pointer)>, field))))

extern "C" {
    extern uint8_t LinkerCheeseA[0] asm("_LinkerCheeseA");
}

dllexport int32_t thiscall GuiImpl::run_msg() {
    if (this->msg.current_msg_index < 0) {
        return -1;
    }
    if (this->msg.ignore_wait_counter > 0) {
        --this->msg.ignore_wait_counter;
    }
    MsgInstruction *restrict current_instruction = this->msg.current_instr;
    if (this->msg.dialog_skippable && (INPUTS.__word_0 & 0x100)) {
        this->msg.script_time = (Timer){ -999, 0.0f, current_instruction->time };
    }
    int32_t current_time = this->msg.script_time.current;
    for (
        ;
        //expect_chance(current_instruction->time < current_time, true, 0.4);
        //current_instruction->time < current_time;
        current_instruction->time <= this->msg.script_time.current;
        IndexInstr(current_instruction->args_size + sizeof(MsgInstruction))
    ) {
        //switch (expect_chance(current_instruction->opcode, 3, 0.4)) {
        switch (current_instruction->opcode) {
            case 0: // end
                this->msg.current_msg_index = -1;
                return -1;
            case 1: { // set_script
                int32_t portrait_index = ShortArg(0);
                AnmVM *restrict portrait_vm = &this->msg.portraits[portrait_index];
                int32_t script_index = ShortArg(1) + (portrait_index ? 2 : 0) + 1184;
                portrait_vm->script_number = script_index;
                ANM_MANAGER_PTR->set_vm_script(portrait_vm, ANM_MANAGER_PTR->scripts[script_index]);
                break;
            }
            case 2: { // set_sprite
                int32_t portrait_index = ShortArg(0);
                AnmVM *restrict portrait_vm = &this->msg.portraits[portrait_index];
                int32_t sprite_index = ShortArg(1) + (portrait_index ? 8 : 0) + 1184;
                ANM_MANAGER_PTR->set_vm_sprite(portrait_vm, sprite_index);
                break;
            }
            case 3: { // text_set
                //int32_t color_index = ShortArg(0);
                //D3DCOLOR colorA = this->msg.text_colors_A[color_index];
                //D3DCOLOR colorB = this->msg.text_colors_B[color_index];
                int32_t line_index = ShortArg(1);
                if (line_index == 0 && this->msg.dialogue_lines[1].script_number >= 0) {
                    //ANM_MANAGER_PTR->__draw_text_to_vm_left(&this->msg.dialogue_lines[1], colorA, colorB, SingleSpaceString);
                    int32_t color_index = ShortArg(0);
                    ANM_MANAGER_PTR->__draw_text_to_vm_left(&this->msg.dialogue_lines[1], this->msg.text_colors_A[color_index], this->msg.text_colors_B[color_index], SingleSpaceString);
                }
                AnmVM *restrict dialogue_vm = &this->msg.dialogue_lines[line_index];
                int32_t script_index = line_index + 1794;
                dialogue_vm->script_number = script_index;
                ANM_MANAGER_PTR->set_vm_script(dialogue_vm, ANM_MANAGER_PTR->scripts[script_index]);
                dialogue_vm->font_width = this->msg.font_size;
                dialogue_vm->font_height = this->msg.font_size;
                //ANM_MANAGER_PTR->__draw_text_to_vm_left(dialogue_vm, colorA, colorB, StringArg(8));
                int32_t color_index = ShortArg(0);
                ANM_MANAGER_PTR->__draw_text_to_vm_left(dialogue_vm, this->msg.text_colors_A[color_index], this->msg.text_colors_B[color_index], StringArg(4));
                this->msg.frames_elapsed_during_pause = 0;
                break;
            }
            case 4: // text_pause
                if ((!this->msg.dialog_skippable || !(INPUTS.__word_0 & 0x100)) &&
                    (!(INPUTS.__word_0 & 0x1) || INPUTS.__word_4 & 0x1 || this->msg.frames_elapsed_during_pause < 8) &&
                    this->msg.frames_elapsed_during_pause < IntArg(0)
                ) {
                    ++this->msg.frames_elapsed_during_pause;
                    goto BreakInsLoop;
                }
                /*if ((!this->msg.dialog_skippable || !(INPUTS.__word_0 & 0x100)) &&
                    (!(INPUTS.__word_0 & 0x1) || ((INPUTS.__word_0 & 0x1) == (INPUTS.__word_4 & 0x1)) || this->msg.frames_elapsed_during_pause < 8) &&
                    this->msg.frames_elapsed_during_pause < IntArg(0)
                    ) {
                    ++this->msg.frames_elapsed_during_pause;
                    goto BreakInsLoop;
                }*/
                break;
            case 5: { // anm_interrupt
                this->msg.portraits[ShortArg(0)].pending_interrupt_label = ByteArg(2);
                /*int32_t vm_index = ShortArg(0);
                uint16_t interrupt = ByteArg(2);
                if (vm_index < 2) {
                    this->msg.portraits[vm_index].pending_interrupt_label = interrupt;
                } else {
                    this->msg.dialogue_lines[vm_index - 2].pending_interrupt_label = interrupt;
                }*/
                break;
            }
            case 6: // ecl_resume
                ++this->msg.ignore_wait_counter;
                break;
            case 7: { // music
                //AnmVM *restrict music_vm = (AnmVM*)((uintptr_t)this + 0x1CB5 + LinkerZero);
                AnmVM *restrict music_vm = &this->__vm_1CB4;

                music_vm->script_number = 1793;
                ANM_MANAGER_PTR->set_vm_script(music_vm, ANM_MANAGER_PTR->scripts[1793]);
                music_vm->font_width = 16;
                music_vm->font_height = 16;
                int32_t music_index = IntArg(0);
                char(*bgm_buffer)[0x80] = &STAGE.std_file->bgm_names[music_index];
                ANM_MANAGER_PTR->__draw_text_to_vm_right(music_vm, PackD3DCOLOR(0, 224, 255, 255), PackD3DCOLOR(0, 0, 0, 0), MusicFormatString, bgm_buffer);
                if (expect(SUPERVISOR.change_midi_music(music_index), true)) {
                    SUPERVISOR.load_bgm_file((char*)(bgm_buffer + countof(STAGE.std_file->bgm_names)));
                }

                /*StdHeader *restrict std_file = STAGE.std_file;
                ANM_MANAGER_PTR->__draw_text_to_vm_right(music_vm, PackD3DCOLOR(0, 224, 255, 255), PackD3DCOLOR(0, 0, 0, 0), MusicFormatString, std_file->bgm_names[music_index]);
                if (expect(SUPERVISOR.change_midi_music(music_index), true)) {
                    SUPERVISOR.load_bgm_file(std_file->bgm_paths[music_index]);
                }*/
                break;

                /*this->__vm_1CB4.script_number = 1793;
                ANM_MANAGER_PTR->set_vm_script(&this->__vm_1CB4, ANM_MANAGER_PTR->scripts[1793]);
                this->__vm_1CB4.font_width = 16;
                this->__vm_1CB4.font_height = 16;
                int32_t music_index = IntArg(0);
                StdHeader *restrict std_file = STAGE.std_file;
                ANM_MANAGER_PTR->__draw_text_to_vm_right(&this->__vm_1CB4, PackD3DCOLOR(0, 224, 255, 255), PackD3DCOLOR(0, 0, 0, 0), MusicFormatString, std_file->bgm_names[music_index]);
                if (expect(SUPERVISOR.change_midi_music(music_index), true)) {
                    SUPERVISOR.load_bgm_file(std_file->bgm_paths[music_index]);
                }
                break;*/

                /*AnmVM *restrict music_vm = &this->__vm_1CB4;
                music_vm->script_number = 1793;
                ANM_MANAGER_PTR->set_vm_script(music_vm, ANM_MANAGER_PTR->scripts[1793]);
                music_vm->font_width = 16;
                music_vm->font_height = 16;
                int32_t music_index = IntArg(0);
                StdHeader *restrict std_file = STAGE.std_file;
                ANM_MANAGER_PTR->__draw_text_to_vm_right(music_vm, PackD3DCOLOR(0, 224, 255, 255), PackD3DCOLOR(0, 0, 0, 0), MusicFormatString, std_file->bgm_names[music_index]);
                if (SUPERVISOR.change_midi_music(music_index)) {
                    SUPERVISOR.load_bgm_file(std_file->bgm_paths[music_index]);
                }
                break;*/
            }
            case 8: { // intro
                int32_t line_index = ShortArg(1);
                int32_t script_index = line_index + 1796;
                AnmVM *restrict intro_vm = &this->msg.intro_lines[line_index];
                intro_vm->script_number = script_index;
                ANM_MANAGER_PTR->set_vm_script(intro_vm, ANM_MANAGER_PTR->scripts[script_index]);
                int32_t color_index = ShortArg(0);
                ANM_MANAGER_PTR->__draw_text_to_vm_right(intro_vm, this->msg.text_colors_A[color_index], this->msg.text_colors_B[color_index], StringArg(4));
                this->msg.frames_elapsed_during_pause = 0;
                break;
            }
            case 9: // stage_results
                this->__dword_2BDC = 1;
                if (expect(GAME_MANAGER.current_stage < 6, true)) {
                    //AnmVM *restrict results_vm = (AnmVM*)((uintptr_t)this + 0x2424 + LinkerZero);
                    AnmVM *restrict results_vm = &this->__vm_2424;

                    results_vm->script_number = 1561;
                    ANM_MANAGER_PTR->set_vm_script(results_vm, ANM_MANAGER_PTR->scripts[1561]);

                    //this->__vm_2424.script_number = 1561;
                    //ANM_MANAGER_PTR->set_vm_script(&this->__vm_2424, ANM_MANAGER_PTR->scripts[1561]);
                } else {
                    GAME_MANAGER.__byte_181C = 0xFF;
                }
                break;
            case 10: // nop
                goto BreakInsLoop;
            case 12: // music_end
                SUPERVISOR.fade_music(4.0f);
                break;
            case 11: // stage_end
                GAME_MANAGER.__dword_0 = GAME_MANAGER.score;
                if (GAME_MANAGER.__byte_1823) {
                    //GAME_MANAGER.__dword_0 = GAME_MANAGER.score;
                    SUPERVISOR.__dword_18C = 7;
                } else if (GAME_MANAGER.current_stage < 5 || (GAME_MANAGER.difficulty != 0 && GAME_MANAGER.current_stage == 5)) {
                    SUPERVISOR.__dword_18C = 3;
                } else if (!GAME_MANAGER.__spellcard_related_1) {
                    SUPERVISOR.__dword_18C = 8;
                } else if (GAME_MANAGER.difficulty != 4) {
                    SUPERVISOR.__dword_18C = 10;
                } else {
                    GAME_MANAGER.__byte_1822 = 1;
                    //GAME_MANAGER.__dword_0 = GAME_MANAGER.score;
                    SUPERVISOR.__dword_18C = 7;
                }
                goto BreakInsLoop;
            case 13: // text_skippable
                this->msg.dialog_skippable = ByteArg(0);
                break;
        }
    }
    this->msg.script_time.tick();
BreakInsLoop:
    this->msg.current_instr = current_instruction;
    ANM_MANAGER_PTR->run_anm(&this->msg.portraits[0]);
    ANM_MANAGER_PTR->run_anm(&this->msg.portraits[1]);
    ANM_MANAGER_PTR->run_anm(&this->msg.dialogue_lines[0]);
    ANM_MANAGER_PTR->run_anm(&this->msg.dialogue_lines[1]);
    ANM_MANAGER_PTR->run_anm(&this->msg.intro_lines[0]);
    ANM_MANAGER_PTR->run_anm(&this->msg.intro_lines[1]);
    if (this->msg.script_time.current < 60 && this->msg.dialog_skippable && (INPUTS.__word_0 & 0x100)) {
        this->msg.script_time = (Timer){ -999, 0.0f, 60 };
    }
    return 0;
}

dllexport int32_t thiscall Supervisor::fade_music(float arg) {
    if (expect(this->config.bgm_format == 2, false)) {
        if (MidiManager *restrict midi_manager = this->midi_manager_ptr) {
            midi_manager->__sub_422630(arg);
        }
    } else if (expect(this->config.bgm_format == 1, true)) {
        if (this->game_speed == 0.0f) {
            SOUND_MANAGER.__sub_424E41(arg);
        } else if (this->game_speed > 1.0f) {
            SOUND_MANAGER.__sub_424E41(arg);
        } else {
            SOUND_MANAGER.__sub_424E41(arg / this->game_speed);
        }
    } else {
        return -1;
    }
    return 0;
}

Enemy& deleeto = *(Enemy*)malloc(sizeof(Enemy));

dllexport void force_export_funcs() {
    gnu_used static volatile auto tempA = ENEMY_CONTROLLER.run_ecl(&deleeto);
}


typedef struct UpdateFunc UpdateFunc;

struct UpdateFunc {
    int16_t priority;  // 0x0
    union {
        uint16_t flags;  // 0x2
        struct {
            uint16_t __flag_A : 1;
        };
    };
    int32_t (cdecl *function_ptr)(void *const restrict update_data);  // 0x4
    int32_t (cdecl *on_registration)(void *const restrict update_data);  // 0x8
    void (cdecl *on_cleanup)(void *const restrict update_data);  // 0xc
    UpdateFunc *restrict prev;  // 0x10
    UpdateFunc *restrict next;  // 0x14
    UpdateFunc* self;  // 0x18
    void *restrict data_ptr;  // 0x1c

    // 0x41C790
    dllexport inline void thiscall cleanup() asm("{[codecave:UPDATE_FUNC_cleanup]}") {
        if (this->on_cleanup != NULL) {
            this->on_cleanup(this->data_ptr);
        }
        this->prev = NULL;
        this->next = NULL;
        this->function_ptr = NULL;
        this->on_registration = NULL;
        this->on_cleanup = NULL;
    }
};
ValidateFieldOffset(0x0, UpdateFunc, priority);
ValidateFieldOffset(0x2, UpdateFunc, flags);
ValidateFieldOffset(0x4, UpdateFunc, function_ptr);
ValidateFieldOffset(0x8, UpdateFunc, on_registration);
ValidateFieldOffset(0xC, UpdateFunc, on_cleanup);
ValidateFieldOffset(0x10, UpdateFunc, prev);
ValidateFieldOffset(0x14, UpdateFunc, next);
ValidateFieldOffset(0x1C, UpdateFunc, data_ptr);

typedef struct UpdateFuncRegistry UpdateFuncRegistry;
struct UpdateFuncRegistry {
    UpdateFunc on_tick_list_head;
    UpdateFunc on_draw_list_head;
    uint32_t midi_outputs_count;
    uint32_t __dword_44;

    // 0x41C860
    dllexport int32_t thiscall register_on_tick(UpdateFunc *restrict func, int16_t priority) asm("{[codecave:UPDATE_FUNC_REGISTRY_register_on_tick]}") {

        UpdateFunc* restrict on_tick_list_node = &this->on_tick_list_head;
        LogStub("add calc chain (pri = %d)\n", priority);
        func->priority = priority;
        while (on_tick_list_node->next != NULL) {
            if (on_tick_list_node->priority > priority) break;
            on_tick_list_node = on_tick_list_node->next;
        }
        if (on_tick_list_node->priority > priority) {
            func->next = on_tick_list_node;
            func->prev = on_tick_list_node->prev;
            if (func->prev != NULL) {
                func->prev->next = func;
            }
            on_tick_list_node->prev = func;
        } else {
            func->next = NULL;
            func->prev = on_tick_list_node;
            on_tick_list_node->next = func;
        }
        /*int32_t ret = 0;
        if (func->on_registration != NULL) {
            ret = func->on_registration(func->data_ptr);
            func->on_registration = NULL;
        }
        return ret;*/
        if (expect(func->on_registration != NULL, true)) {
            int32_t ret = func->on_registration(func->data_ptr);
            func->on_registration = NULL;
            return ret;
        } else {
            return 0;
        }
    }

    // 0x41C940
    dllexport int32_t thiscall register_on_draw(UpdateFunc *restrict func, int16_t priority) asm("{[codecave:UPDATE_FUNC_REGISTRY_register_on_draw]}") {
        
        UpdateFunc* restrict on_draw_list_node = &this->on_draw_list_head;
        LogStub("add draw chain (pri = %d)\n", priority);
        func->priority = priority;
        while (on_draw_list_node->next != NULL) {
            if (on_draw_list_node->priority > priority) break;
            on_draw_list_node = on_draw_list_node->next;
        }
        if (on_draw_list_node->priority > priority) {
            func->next = on_draw_list_node;
            func->prev = on_draw_list_node->prev;
            if (func->prev != NULL) {
                func->prev->next = func;
            }
            on_draw_list_node->prev = func;
        } else {
            func->next = NULL;
            func->prev = on_draw_list_node;
            on_draw_list_node->next = func;
        }
        /*int32_t ret = 0;
        if (func->on_registration != NULL) {
            ret = func->on_registration(func->data_ptr);
        }
        return ret;*/
        if (expect(func->on_registration != NULL, true)) {
            return func->on_registration(func->data_ptr);
        } else {
            return 0;
        }
    }

    // 0x41CDE0
    dllexport void thiscall unregister_func(UpdateFunc *restrict func) asm("{[codecave:UPDATE_FUNC_REGISTRY_unregister_func]}") {
        int32_t func_type = 0;
        if (func != NULL) {

            /*UpdateFunc* restrict list_node = &this->on_tick_list_head;
            assume(list_node != NULL);
            while (expect(list_node != NULL, false)) {
                if (expect(list_node == func, false)) goto FoundFunc;
                list_node = list_node->next;
            }
            func_type = 1;
            list_node = &this->on_draw_list_head;
            assume(list_node != NULL);
            while (expect(list_node != NULL, true)) {
                if (expect(list_node == func, true)) goto FoundFunc;
                list_node = list_node->next;
            }*/

            UpdateFunc* restrict list_node = &this->on_tick_list_head;
            assume(list_node != NULL);
            while (list_node != NULL) {
                if (list_node == func) goto FoundFunc;
                list_node = list_node->next;
            }
            func_type = 1;
            list_node = &this->on_draw_list_head;
            assume(list_node != NULL);
            while (list_node != NULL) {
                if (list_node == func) goto FoundFunc;
                list_node = list_node->next;
            }
            return;
        FoundFunc:
            if (func_type == 0) {
                LogStub("calc out Chain (Pri = %d)\n", func->priority);
            } else {
                LogStub("draw out Chain (Pri = %d)\n", func->priority);
            }
            if (expect(func->prev != NULL, true)) {
                func->prev->next = func->next;
                if (func->next != NULL) {
                    func->next->prev = func->prev;
                }
                func->prev = NULL;
                func->next = NULL;
                if (func->__flag_A == true) {
                    // No idea what this ZUN jank was originally supposed to be. �P\_(�c)_/�P
                    UpdateFunc* tempA = func;
                    UpdateFunc* tempB = tempA;
                    UpdateFunc* tempC;
                    if (tempB != NULL) {
                        tempB->cleanup();
                        if (!ReadDebugFlags(1)) {
                            delete tempB;
                        }
                        tempC = tempB;
                    } else {
                        tempC = NULL;
                        func = NULL;
                    }
                } else {
                    if (func->on_cleanup != NULL) {
                        auto cleanup_func = func->on_cleanup;
                        func->on_cleanup = NULL;
                        cleanup_func(func->data_ptr);
                    }
                }
            }
        }
    }

    // 0x41CA10
    dllexport int32_t thiscall run_all_on_tick(void) asm("{[codecave:UPDATE_FUNC_REGISTRY_run_all_on_tick]}") {

        
    ReInitTickLoop:
        int32_t func_count = 0;
        UpdateFunc* restrict on_tick_list_node = &this->on_tick_list_head;
        assume(on_tick_list_node != NULL);
        for (
            UpdateFunc* restrict next_node;
            on_tick_list_node != NULL;
            on_tick_list_node = next_node
        ) {
            next_node = on_tick_list_node->next;

            if (auto func_ptr = on_tick_list_node->function_ptr; func_ptr != NULL) {
                
                int32_t case_index;
                while (expect((case_index = func_ptr(on_tick_list_node->data_ptr)) == 2, false));

                if (!expect(case_index == 1, true)) {
                    if (expect(case_index == 0, false)) {
                        this->unregister_func(on_tick_list_node);
                    }
                    else if (expect(case_index == 6, false)) {
                        goto ReInitTickLoop;
                    }
                    else if (expect((uint32_t)case_index < 6, true)) {
                        return 4 - case_index;
                    }
                }
                ++func_count;
            }

            /*if (on_tick_list_node->function_ptr != NULL) {
            ReRunTickFunc:
                switch (int32_t case_index = on_tick_list_node->function_ptr(on_tick_list_node->data_ptr)) {
                    case 0: {
                        UpdateFunc *restrict old_node = on_tick_list_node;
                        on_tick_list_node = on_tick_list_node->next;
                        this->unregister_func(old_node);
                        ++func_count;
                        continue;
                    }
                    case 2:
                        goto ReRunTickFunc;
                    case 3: case 4: case 5:
                        return 4 - case_index;
                    case 6:
                        goto ReInitTickLoop;
                    default:
                        ++func_count;
                }
            }
            on_tick_list_node = on_tick_list_node->next;*/
        }
        return func_count;
    }

    // 0041CAD0
    dllexport int32_t run_all_on_draw(void) asm("{[codecave:UPDATE_FUNC_REGISTRY_run_all_on_draw]}") {

        int32_t func_count = 0;
        UpdateFunc* restrict on_draw_list_node = &this->on_draw_list_head;
        assume(on_draw_list_node != NULL);
        for (
            UpdateFunc* restrict next_node;
            on_draw_list_node != NULL;
            on_draw_list_node = next_node
        ) {
            next_node = on_draw_list_node->next;

            if (auto func_ptr = on_draw_list_node->function_ptr; func_ptr != NULL) {
                int32_t case_index;

                while (expect((case_index = func_ptr(on_draw_list_node->data_ptr)) == 2, false));

                if (!expect(case_index == 1, true)) {
                    if (expect(case_index == 0, false)) {
                        this->unregister_func(on_draw_list_node);
                    }
                    else if (expect((uint32_t)case_index < 6, true)) {
                        return 4 - case_index;
                    }
                }
                ++func_count;
            }
        }
        return func_count;
    }
};
ValidateFieldOffset(0x0, UpdateFuncRegistry, on_tick_list_head);
ValidateFieldOffset(0x20, UpdateFuncRegistry, on_draw_list_head);
ValidateFieldOffset(0x40, UpdateFuncRegistry, midi_outputs_count);
ValidateFieldOffset(0x44, UpdateFuncRegistry, __dword_44);