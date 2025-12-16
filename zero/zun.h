#ifndef ZUN_H
#define ZUN_H 1

#pragma clang diagnostic ignored "-Wc++20-extensions"

#include <stdint.h>
#include <stdlib.h>
#include <utility>

#include "util.h"

#define UNUSED_DWORD (GARBAGE_ARG(int32_t))
#define UNUSED_FLOAT (GARBAGE_ARG(float))

static inline constexpr size_t game_version = GAME_VERSION;

#define NA_TO_GAME

// WTF is wrong with me
#define PER_GAME_EoSD(...) MACRO_FIRST_EVAL(__VA_ARGS__)
#define PER_GAME_PCB(...) MACRO_DEFAULT_ARG(PER_GAME_EoSD(__VA_ARGS__), MACRO_SECOND(__VA_ARGS__))
#define PER_GAME_IN(...) MACRO_DEFAULT_ARG(PER_GAME_PCB(__VA_ARGS__), MACRO_THIRD(__VA_ARGS__))
#define PER_GAME_PoFV(...) MACRO_DEFAULT_ARG(PER_GAME_IN(__VA_ARGS__), MACRO_FOURTH(__VA_ARGS__))
#define PER_GAME_StB(...) MACRO_DEFAULT_ARG(PER_GAME_PoFV(__VA_ARGS__), MACRO_FIFTH(__VA_ARGS__))
#define PER_GAME_MoF(...) MACRO_DEFAULT_ARG(PER_GAME_StB(__VA_ARGS__), MACRO_SIXTH(__VA_ARGS__))
#define PER_GAME_UB(...) MACRO_DEFAULT_ARG(PER_GAME_MoF(__VA_ARGS__), MACRO_SEVENTH(__VA_ARGS__))
#define PER_GAME_SA(...) MACRO_DEFAULT_ARG(PER_GAME_UB(__VA_ARGS__), MACRO_EIGTH(__VA_ARGS__))
#define PER_GAME_UFO(...) MACRO_DEFAULT_ARG(PER_GAME_SA(__VA_ARGS__), MACRO_NINTH(__VA_ARGS__))
#define PER_GAME_DS(...) MACRO_DEFAULT_ARG(PER_GAME_UFO(__VA_ARGS__), MACRO_TENTH(__VA_ARGS__))
#define PER_GAME_GFW(...) MACRO_DEFAULT_ARG(PER_GAME_DS(__VA_ARGS__), MACRO_ELEVENTH(__VA_ARGS__))
#define PER_GAME_TD(...) MACRO_DEFAULT_ARG(PER_GAME_GFW(__VA_ARGS__), MACRO_TWELFTH(__VA_ARGS__))
#define PER_GAME_DDC(...) MACRO_DEFAULT_ARG(PER_GAME_TD(__VA_ARGS__), MACRO_THIRTEENTH(__VA_ARGS__))
#define PER_GAME_ISC(...) MACRO_DEFAULT_ARG(PER_GAME_DDC(__VA_ARGS__), MACRO_FOURTEENTH(__VA_ARGS__))
#define PER_GAME_LoLK(...) MACRO_DEFAULT_ARG(PER_GAME_ISC(__VA_ARGS__), MACRO_FIFTEENTH(__VA_ARGS__))
#define PER_GAME_HSiFS(...) MACRO_DEFAULT_ARG(PER_GAME_LoLK(__VA_ARGS__), MACRO_SIXTEENTH(__VA_ARGS__))
#define PER_GAME_VD(...) MACRO_DEFAULT_ARG(PER_GAME_HSiFS(__VA_ARGS__), MACRO_SEVENTEENTH(__VA_ARGS__))
#define PER_GAME_WBaWC(...) MACRO_DEFAULT_ARG(PER_GAME_VD(__VA_ARGS__), MACRO_EIGHTEENTH(__VA_ARGS__))
#define PER_GAME_UM(...) MACRO_DEFAULT_ARG(PER_GAME_WBaWC(__VA_ARGS__), MACRO_NINETEENTH(__VA_ARGS__))
#define PER_GAME_HBM(...) MACRO_DEFAULT_ARG(PER_GAME_UM(__VA_ARGS__), MACRO_TWENTIETH(__VA_ARGS__))
#define PER_GAME_UDoALG(...) MACRO_DEFAULT_ARG(PER_GAME_HBM(__VA_ARGS__), MACRO_TWENTY_FIRST(__VA_ARGS__))
#define PER_GAME_FW(...) MACRO_DEFAULT_ARG(PER_GAME_UDoALG(__VA_ARGS__), MACRO_TWENTY_SECOND(__VA_ARGS__))

#if GAME_VERSION == EoSD_VER
#define PER_GAME(...) PER_GAME_EoSD(__VA_ARGS__)
#elif GAME_VERSION == PCB_VER
#define PER_GAME(...) PER_GAME_PCB(__VA_ARGS__)
#elif GAME_VERSION == IN_VER
#define PER_GAME(...) PER_GAME_IN(__VA_ARGS__)
#elif GAME_VERSION == PoFV_VER
#define PER_GAME(...) PER_GAME_PoFV(__VA_ARGS__)
#elif GAME_VERSION == StB_VER
#define PER_GAME(...) PER_GAME_StB(__VA_ARGS__)
#elif GAME_VERSION == MoF_VER
#define PER_GAME(...) PER_GAME_MoF(__VA_ARGS__)
#elif GAME_VERSION == UB_VER
#define PER_GAME(...) PER_GAME_UB(__VA_ARGS__)
#elif GAME_VERSION == SA_VER
#define PER_GAME(...) PER_GAME_SA(__VA_ARGS__)
#elif GAME_VERSION == UFO_VER
#define PER_GAME(...) PER_GAME_UFO(__VA_ARGS__)
#elif GAME_VERSION == DS_VER
#define PER_GAME(...) PER_GAME_DS(__VA_ARGS__)
#elif GAME_VERSION == GFW_VER
#define PER_GAME(...) PER_GAME_GFW(__VA_ARGS__)
#elif GAME_VERSION == TD_VER
#define PER_GAME(...) PER_GAME_TD(__VA_ARGS__)
#elif GAME_VERSION == DDC_VER
#define PER_GAME(...) PER_GAME_DDC(__VA_ARGS__)
#elif GAME_VERSION == ISC_VER
#define PER_GAME(...) PER_GAME_ISC(__VA_ARGS__)
#elif GAME_VERSION == LoLK_VER
#define PER_GAME(...) PER_GAME_LoLK(__VA_ARGS__)
#elif GAME_VERSION == HSiFS_VER
#define PER_GAME(...) PER_GAME_HSiFS(__VA_ARGS__)
#elif GAME_VERSION == VD_VER
#define PER_GAME(...) PER_GAME_VD(__VA_ARGS__)
#elif GAME_VERSION == WBaWC_VER
#define PER_GAME(...) PER_GAME_WBaWC(__VA_ARGS__)
#elif GAME_VERSION == UM_VER
#define PER_GAME(...) PER_GAME_UM(__VA_ARGS__)
#elif GAME_VERSION == HBM_VER
#define PER_GAME(...) PER_GAME_HBM(__VA_ARGS__)
#elif GAME_VERSION == UDoALG_VER
#define PER_GAME(...) PER_GAME_UDoALG(__VA_ARGS__)
#elif GAME_VERSION == FW_VER
#define PER_GAME(...) PER_GAME_FW(__VA_ARGS__)
#else
#error "GAME_VERSION not set"
#endif

enum ZUNResult : int32_t {
    ZUN_SUCCESS2 = 1,
    ZUN_SUCCESS = 0,
    ZUN_ERROR = -1,
    ZUN_ERROR2 = -2
};

#define ZUN_SUCCEEDED(result) (((ZUNResult)(result)) == ZUN_SUCCESS)
#define ZUN_FAILED(result) (((ZUNResult)(result)) != ZUN_SUCCESS)

// ZUN probably wrote this himself to go with the other "SAFE" macros...
#ifndef SAFE_FREE
#define SAFE_FREE(p)       { if (p) { free ((void*)p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

// size: 0x4
union PixelA8R8G8B8 {
    uint32_t raw; // 0x0
    struct {
        uint8_t b; // 0x0
        uint8_t g; // 0x1
        uint8_t r; // 0x2
        uint8_t a; // 0x3
        // 0x4
    };

    static inline constexpr D3DFORMAT format = D3DFMT_A8R8G8B8;
    static inline constexpr bool has_a         = true,  has_r         = true,  has_g         = true,  has_b         = true;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = false, g_is_bitfield = false, b_is_bitfield = false;
    static inline constexpr uint8_t a_bits = 8, r_bits = 8, g_bits = 8, b_bits = 8;
    static inline constexpr uint32_t a_mask = 0xFF000000, r_mask = 0x00FF0000, g_mask = 0x0000FF00, b_mask = 0x000000FF;
    static inline constexpr uint8_t a_mask_shifted = 0xFF, r_mask_shifted = 0xFF, g_mask_shifted = 0xFF, b_mask_shifted = 0xFF;
    static inline constexpr uint8_t a_shift = 24, r_shift = 16, g_shift = 8, b_shift = 0;
};

// size: 0x4
union PixelX8R8G8B8 {
    uint32_t raw; // 0x0
    struct {
        uint8_t b; // 0x0
        uint8_t g; // 0x1
        uint8_t r; // 0x2
        // 0x3
    };

    static inline constexpr D3DFORMAT format = D3DFMT_X8R8G8B8;
    static inline constexpr bool has_a         = false, has_r         = true,  has_g         = true,  has_b         = true;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = false, g_is_bitfield = false, b_is_bitfield = false;
    static inline constexpr uint8_t a_bits = 0, r_bits = 8, g_bits = 8, b_bits = 8;
    static inline constexpr uint32_t a_mask = 0, r_mask = 0x00FF0000, g_mask = 0x0000FF00, b_mask = 0x000000FF;
    static inline constexpr uint8_t a_mask_shifted = 0, r_mask_shifted = 0xFF, g_mask_shifted = 0xFF, b_mask_shifted = 0xFF;
    static inline constexpr uint8_t a_shift = 0, r_shift = 16, g_shift = 8, b_shift = 0;
};

// size: 0x2
union PixelA1R5G5B5 {
    uint16_t raw; // 0x0
    struct {
        uint16_t b : 5;
        uint16_t g : 5;
        uint16_t r : 5;
        uint16_t a : 1;
    };

    static inline constexpr D3DFORMAT format = D3DFMT_A1R5G5B5;
    static inline constexpr bool has_a         = true, has_r         = true, has_g         = true, has_b         = true;
    static inline constexpr bool a_is_bitfield = true, r_is_bitfield = true, g_is_bitfield = true, b_is_bitfield = true;
    static inline constexpr uint8_t a_bits = 1, r_bits = 5, g_bits = 5, b_bits = 5;
    static inline constexpr uint16_t a_mask = 0x8000, r_mask = 0x7C00, g_mask = 0x03E0, b_mask = 0x001F;
    static inline constexpr uint8_t a_mask_shifted = 0x1, r_mask_shifted = 0x1F, g_mask_shifted = 0x1F, b_mask_shifted = 0x1F;
    static inline constexpr uint8_t a_shift = 11, r_shift = 10, g_shift = 5, b_shift = 0;
};

// size: 0x2
union PixelX1R5G5B5 {
    uint16_t raw; // 0x0
    struct {
        uint16_t b : 5;
        uint16_t g : 5;
        uint16_t r : 5;
        uint16_t : 1;
    };

    static inline constexpr D3DFORMAT format = D3DFMT_X1R5G5B5;
    static inline constexpr bool has_a         = false, has_r         = true, has_g         = true, has_b         = true;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = true, g_is_bitfield = true, b_is_bitfield = true;
    static inline constexpr uint8_t a_bits = 0, r_bits = 5, g_bits = 5, b_bits = 5;
    static inline constexpr uint16_t a_mask = 0, r_mask = 0x7C00, g_mask = 0x03E0, b_mask = 0x001F;
    static inline constexpr uint8_t a_mask_shifted = 0, r_mask_shifted = 0x1F, g_mask_shifted = 0x1F, b_mask_shifted = 0x1F;
    static inline constexpr uint8_t a_shift = 0, r_shift = 10, g_shift = 5, b_shift = 0;
};

// size: 0x2
union PixelR5G6B5 {
    uint16_t raw; // 0x0
    struct {
        uint16_t b : 5;
        uint16_t g : 6;
        uint16_t r : 5;
    };

    static inline constexpr D3DFORMAT format = D3DFMT_R5G6B5;
    static inline constexpr bool has_a         = false, has_r         = true, has_g         = true, has_b         = true;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = true, g_is_bitfield = true, b_is_bitfield = true;
    static inline constexpr uint8_t a_bits = 0, r_bits = 5, g_bits = 6, b_bits = 5;
    static inline constexpr uint16_t a_mask = 0, r_mask = 0xF800, g_mask = 0x07E0, b_mask = 0x001F;
    static inline constexpr uint8_t a_mask_shifted = 0, r_mask_shifted = 0x1F, g_mask_shifted = 0x3F, b_mask_shifted = 0x1F;
    static inline constexpr uint8_t a_shift = 0, r_shift = 11, g_shift = 5, b_shift = 0;
};

// size: 0x3
union PixelR8G8B8 {
    struct {
        uint8_t raw[3];
    };
    struct {
        uint8_t b; // 0x0
        uint8_t g; // 0x1
        uint8_t r; // 0x2
    };

    static inline constexpr D3DFORMAT format = D3DFMT_R8G8B8;
    static inline constexpr bool has_a         = false, has_r         = true, has_g         = true, has_b         = true;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = true, g_is_bitfield = true, b_is_bitfield = true;
    static inline constexpr uint8_t a_bits = 0, r_bits = 8, g_bits = 8, b_bits = 8;
    static inline constexpr uint32_t a_mask = 0, r_mask = 0xFF0000, g_mask = 0x00FF00, b_mask = 0x0000FF;
    static inline constexpr uint8_t a_mask_shifted = 0, r_mask_shifted = 0xFF, g_mask_shifted = 0xFF, b_mask_shifted = 0xFF;
    static inline constexpr uint8_t a_shift = 0, r_shift = 16, g_shift = 8, b_shift = 0;
};

// size: 0x2
union PixelA4R4G4B4 {
    uint16_t raw; // 0x0
    struct {
        uint8_t b : 4;
        uint8_t g : 4;
        uint8_t r : 4;
        uint8_t a : 4;
    };

    static inline constexpr D3DFORMAT format = D3DFMT_A4R4G4B4;
    static inline constexpr bool has_a         = true, has_r         = true, has_g         = true, has_b         = true;
    static inline constexpr bool a_is_bitfield = true, r_is_bitfield = true, g_is_bitfield = true, b_is_bitfield = true;
    static inline constexpr uint8_t a_bits = 4, r_bits = 4, g_bits = 4, b_bits = 4;
    static inline constexpr uint16_t a_mask = 0xF000, r_mask = 0x0F00, g_mask = 0x00F0, b_mask = 0x000F;
    static inline constexpr uint8_t a_mask_shifted = 0xF, r_mask_shifted = 0xF, g_mask_shifted = 0xF, b_mask_shifted = 0xF;
    static inline constexpr uint8_t a_shift = 12, r_shift = 8, g_shift = 4, b_shift = 0;
};

// size: 0x2
union PixelA8R3G3B2 {
    uint16_t raw; // 0x0
    struct {
        uint8_t b : 2;
        uint8_t g : 3;
        uint8_t r : 3;
        uint8_t a;
    };

    static inline constexpr D3DFORMAT format = D3DFMT_A8R3G3B2;
    static inline constexpr bool has_a         = true,  has_r         = true, has_g         = true, has_b         = true;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = true, g_is_bitfield = true, b_is_bitfield = true;
    static inline constexpr uint8_t a_bits = 8, r_bits = 3, g_bits = 3, b_bits = 2;
    static inline constexpr uint16_t a_mask = 0xFF00, r_mask = 0x00E0, g_mask = 0x001C, b_mask = 0x0003;
    static inline constexpr uint8_t a_mask_shifted = 0xFF, r_mask_shifted = 0x7, g_mask_shifted = 0x7, b_mask_shifted = 0x3;
    static inline constexpr uint8_t a_shift = 8, r_shift = 5, g_shift = 2, b_shift = 0;
};

// size: 0x1
union PixelA8 {
    uint8_t raw; // 0x0
    struct {
        uint8_t a;
    };

    static inline constexpr D3DFORMAT format = D3DFMT_A8;
    static inline constexpr bool has_a         = true,  has_r         = false, has_g         = false, has_b         = false;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = false, g_is_bitfield = false, b_is_bitfield = false;
    static inline constexpr uint8_t a_bits = 8, r_bits = 0, g_bits = 0, b_bits = 0;
    static inline constexpr uint8_t a_mask = 0xFF, r_mask = 0, g_mask = 0, b_mask = 0;
    static inline constexpr uint8_t a_mask_shifted = 0xFF, r_mask_shifted = 0, g_mask_shifted = 0, b_mask_shifted = 0;
    static inline constexpr uint8_t a_shift = 0, r_shift = 0, g_shift = 0, b_shift = 0;
};

// size: 0x1
union PixelR3G3B2 {
    uint8_t raw; // 0x0
    struct {
        uint8_t b : 2;
        uint8_t g : 3;
        uint8_t r : 3;
    };

    static inline constexpr D3DFORMAT format = D3DFMT_R3G3B2;
    static inline constexpr bool has_a         = false, has_r         = true, has_g         = true, has_b         = true;
    static inline constexpr bool a_is_bitfield = false, r_is_bitfield = true, g_is_bitfield = true, b_is_bitfield = true;
    static inline constexpr uint8_t a_bits = 0, r_bits = 3, g_bits = 3, b_bits = 2;
    static inline constexpr uint8_t a_mask = 0, r_mask = 0xE0, g_mask = 0x1C, b_mask = 0x03;
    static inline constexpr uint8_t a_mask_shifted = 0, r_mask_shifted = 0x7, g_mask_shifted = 0x7, b_mask_shifted = 0x3;
    static inline constexpr uint8_t a_shift = 0, r_shift = 5, g_shift = 2, b_shift = 0;
};

// Packs the bytes [b], [g], [r], and [a] together as a D3DCOLOR integer
static constexpr inline uint32_t PackD3DCOLOR(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return PackUInt(b, g, r, a);
}

#if GAME_VERSION >= MoF_VER
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
#endif

#include "zun_math.h"
#include "zun_types.h"

#endif