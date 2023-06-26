#ifndef ZUN_H
#define ZUN_H 1

#pragma clang diagnostic ignored "-Wc++20-extensions"

#include <stdint.h>
#include <stdlib.h>

#include "../zero/util.h"

static inline constexpr size_t game_version = GAME_VERSION;

#if GAME_VERSION < MoF
#define OLD_ENGINE 1
#else
#define NEW_ENGINE 1
#endif

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
    ForceFinal = 16
};
#endif

#include "util.h"
#include "funcs.h"
#include "types.h"

#endif