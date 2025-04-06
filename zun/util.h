#ifndef ZUN_UTIL_H
#define ZUN_UTIL_H 1

#include <stdint.h>
#include <stdlib.h>

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
#else
#error "GAME_VERSION not set"
#endif

enum ZUNResult : int32_t {
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

// Packs the bytes [b], [g], [r], and [a] together as a D3DCOLOR integer
static constexpr inline uint32_t PackD3DCOLOR(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return PackUInt(b, g, r, a);
}

#include "funcs.h"

#ifdef OLD_ENGINE
#include "old_engine_util.h"
#else
#include "new_engine_util.h"
#endif

#endif