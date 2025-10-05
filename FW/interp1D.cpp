#include <stdint.h>
#define _USE_MATH_DEFINES 1
#include <math.h>

// I've commented out the modes that are handled by different functions
enum InterpMode : int32_t {
    Linear = 0,
    AccelerateSlow = 1,
    Accelerate = 2,
    AccelerateFast = 3,
    DecelerateSlow = 4,
    Decelerate = 5,
    DecelerateFast = 6,
    //ConstantVelocity = 7,
    //Bezier = 8,
    AccelDecelSlow = 9,
    AccelDecel = 10,
    AccelDecelFast = 11,
    DecelAccelSlow = 12,
    DecelAccel = 13,
    DecelAccelFast = 14,
    ForceInitial = 15,
    ForceFinal = 16,
    //ConstantAccel = 17,
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

const float PI_f = (float)M_PI;
#define HALF_PI PI_f / 2.0f

#define pow_2(x) (x)*(x)
#define pow_3(x) (x)*(x)*(x)
#define pow_4(x) (x)*(x)*(x)*(x)

// becomes asymptotic at b=0.5 and b=1
#define parabola(a, b) \
    (((pow_2(a - b)) / (pow_2(1.0f - b)) - (pow_2(b)) / (pow_2(1.0f - b))) / (1.0f - (pow_2(b)) / (pow_2(1.0f - b))))

float interpolate1D(int32_t mode, float current_time, float end_time) {
    if (!(end_time != 0.0f)) {
        return 1.0f;
    }
    float value = current_time / end_time;
    switch (mode) {
        case AccelerateSlow: // 1
            return pow_2(value);
        case DecelerateSlow: // 4
            return 1.0f - pow_2(1.0f - value);
        case Accelerate: // 2
            return pow_3(value);
        case Decelerate: // 5
            return 1.0f - pow_3(1.0f - value);
        case AccelerateFast: // 3
            return pow_4(value);
        case DecelerateFast: // 6
            return 1.0f - pow_4(1.0f - value);
        case AccelDecelSlow: { // 9
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = pow_2(value) / 2.0f;
            } else {
                ret = (2.0f - pow_2(2.0f - value)) / 2.0f;
            }
            return ret;
        }
        case DecelAccelSlow: { // 12
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = 0.5f - pow_2(1.0f - value) / 2.0f;
            } else {
                ret = 0.5f + pow_2(value - 1.0f) / 2.0f;
            }
            return ret;
        }
        case AccelDecel: { // 10
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = pow_3(value) / 2.0f;
            } else {
                ret = (2.0f - pow_3(2.0f - value)) / 2.0f;
            }
            return ret;
        }
        case DecelAccel: { // 13
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = 0.5f - pow_3(1.0f - value) / 2.0f;
            } else {
                ret = 0.5f + pow_3(value - 1.0f) / 2.0f;
            }
            return ret;
        }
        case AccelDecelFast: { // 11
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = pow_4(value) / 2.0f;
            } else {
                ret = (2.0f - pow_4(2.0f - value)) / 2.0f;
            }
            return ret;
        }
        case DecelAccelFast: { // 14
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = 0.5f - pow_4(1.0f - value) / 2.0f;
            } else {
                ret = 0.5f + pow_4(value - 1.0f) / 2.0f;
            }
        }
        case ForceInitial: // 15
            return 0.0f;
        case ForceFinal: // 16
            return 1.0f;
        case DecelerateSine: // 18
            return sinf(value * HALF_PI);
        case AccelerateSine: // 19
            return 1.0f - sinf(value * HALF_PI + HALF_PI);
        case DecelAccelSine: { // 20
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = sinf(value * HALF_PI) / 2.0f;
            } else {
                ret = (1.0f - sinf(value * HALF_PI)) / 2.0f + 0.5f;
            }
            return ret;
        }
        case AccelDecelSine: { // 21
            value *= 2.0f;
            float ret;
            if (value < 1.0f) {
                ret = (1.0f - sinf(value * HALF_PI + HALF_PI)) / 2.0f;
            } else {
                ret = sinf((value - 1.0f) * HALF_PI) / 2.0f + 0.5f;
            }
            return ret;
        }
        case AccelParabolaA: // 22
            return value = parabola(value, 0.25f);
        case AccelParabolaB: // 23
            return value = parabola(value, 0.30f);
        case AccelParabolaC: // 24
            return value = parabola(value, 0.35f);
        case AccelParabolaD: // 25
            return value = parabola(value, 0.38f);
        case AccelParabolaE: // 26
            return value = parabola(value, 0.40f);
        case DecelParabolaA: // 27
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.25f);
        case DecelParabolaB: // 28
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.30f);
        case DecelParabolaC: // 29
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.35f);
        case DecelParabolaD: // 30
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.38f);
        case DecelParabolaE: // 31
            value = 1.0f - value;
            return value = 1.0f - parabola(value, 0.40f);
        case Linear: // 0
        default:
            return value;
    }
}