#ifndef ZUN_H
#define ZUN_H 1

#pragma clang diagnostic ignored "-Wc++20-extensions"

#include <stdint.h>
#include <stdlib.h>

#include "util.h"

static inline constexpr size_t game_version = GAME_VERSION;

enum ZunResult : int32_t {
    ZUN_SUCCESS = 0,
    ZUN_ERROR = -1,
    ZUN_ERROR2 = -2
};

#define ZUN_SUCCEEDED(result) (((ZunResult)(result)) == ZUN_SUCCESS)
#define ZUN_FAILED(result) (((ZunResult)(result)) != ZUN_SUCCESS)

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

#include "zun_math.h"
#include "zun_types.h"

#endif