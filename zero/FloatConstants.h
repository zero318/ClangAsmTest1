#pragma once

#pragma clang diagnostic ignored "-Wc++17-extensions"

#include <math.h>

static inline constexpr double ONE_OVER_TWO_PI_d = 1.0 / (2.0 * M_PI);
static inline constexpr double TWO_PI_d = 2.0 * M_PI;

static inline constexpr const float PI_f = M_PI;
static inline constexpr const float HALF_PI_f asm("{(<codecave:float_constants> + 0x10)}") = M_PI_2;
static inline constexpr const float TWO_PI_f asm("{(<codecave:float_constants> + 0x18)}") = 2.0f * M_PI;
static inline constexpr const float NPI_f = -M_PI;
static inline constexpr const float NHALF_PI_f = -M_PI_2;
static inline constexpr const float HUNDREDTH_PI_f asm("{(<codecave:float_constants> + 0x1C)}") = M_PI / 100.0f;
static inline constexpr const float ONE_THIRD_f = 1.0f / 3.0f;
static inline constexpr const float TWO_THIRDS_f = 2.0f / 3.0f;
static inline constexpr const float NTWO_PI_f = -2.0f * M_PI;
static inline constexpr const float ONE_OVER_TWO_PI_f = 1.0f / (2.0f * M_PI);
static inline constexpr const float NONE_OVER_TWO_PI_f = -1.0f / (2.0f * M_PI);
static inline constexpr const float REASONABLY_ABOVE_NPI_f = -3.141592;
static inline constexpr const float RECIP_TWO_PI_f = 1.0f / (2.0f * M_PI);

static inline constexpr const float HALF_POW_IDK asm("{(<codecave:float_constants> + 0x0)}") = 0x1p-32f;

static inline constexpr const float POINT_NINE_NINE asm("{(<codecave:float_constants> + 0x4)}") = 0.99f;
static inline constexpr const float ONE asm("{(<codecave:float_constants> + 0x8)}") = 1.0f;
static inline constexpr const float NEG_ONE asm("{(<codecave:float_constants> + 0xC)}") = -1.0f;
static inline constexpr const float NEG_POINT_FIVE asm("{(<codecave:float_constants> + 0x14)}") = -0.5f;
static inline constexpr const float POINT_THREE asm("{(<codecave:float_constants> + 0x20)}") = 0.3f;
static inline constexpr const float FORTY_EIGHT asm("{(<codecave:float_constants> + 0x28)}") = 48.0f;
static inline constexpr const float NEG_SIXTY_FOUR asm("{(<codecave:float_constants> + 0x30)}") = -64.0f;
static inline constexpr const float SEVEN_POINT_FIVE asm("{(<codecave:float_constants> + 0x34)}") = 7.5f;
static inline constexpr const float SIXTEEN asm("{(<codecave:float_constants> + 0x38)}") = 16.0f;
static inline constexpr const float POINT_FIVE asm("{(<codecave:float_constants> + 0x3C)}") = 0.5f;
static inline constexpr const float NEG_POINT_TWO_FIVE asm("{(<codecave:float_constants> + 0x40)}") = -0.25f;
static inline constexpr const float TWO_FORTY asm("{(<codecave:float_constants> + 0x44)}") = 240.0f;
static inline constexpr const float NEG_TEN_POINT_FOUR asm("{(<codecave:float_constants> + 0x48)}") = -10.4f;
static inline constexpr const float FOUR_SIXTEEN asm("{(<codecave:float_constants> + 0x4C)}") = 416.0f;
static inline constexpr const float POINT_ZERO_ONE asm("{(<codecave:float_constants> + 0x50)}") = 0.01f;
static inline constexpr const float NEG_POINT_ZERO_TWO asm("{(<codecave:float_constants> + 0x54)}") = -0.02f;
static inline constexpr const float SIXTY asm("{(<codecave:float_constants> + 0x58)}") = 60.0f;