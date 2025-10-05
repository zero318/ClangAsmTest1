float interpolate1D_fast(int32_t mode, float current_time, float end_time) {
    if (end_time == 0.0f) {
        return 1.0f;
    }
    float value = current_time / end_time;
    float valueR = 1.0f - value;
    vec<float, 2> temp = { valueR, value };
    temp += temp;
    vec<float, 2> temp2 = temp;
    float ret = value;
    float retR = valueR;
    switch (expect_chance(mode, 0, 0.5)) {
        default:
            unreachable;
        case Linear: // 0
            return ret;
        case ForceInitial: // 15
            return 0.0f;
        case ForceFinal: // 16
            return 1.0f;
        case AccelerateFast: // 3
            ret *= value;
        case Accelerate: // 2
            ret *= value;
        case AccelerateSlow: // 1
            ret *= value;
            return ret;
        case DecelerateFast: // 6
            retR *= valueR;
        case Decelerate: // 5
            retR *= valueR;
        case DecelerateSlow: // 4
            retR *= valueR;
            return 1.0f - retR;
        case AccelDecelFast: { // 11
            temp2 *= temp;
        case AccelDecel: // 10
            temp2 *= temp;
        case AccelDecelSlow: // 9
            temp2 *= temp;
            float high = temp2[0];
            float low = temp2[1];
            high = 2.0f - high;
            float ret = value < 0.5f ? low : high;
            return ret * 0.5f;
        }
        case DecelAccelFast: // 14
            value += value;
            if (value < 1.0f) {
                ret = pow_4(1.0f - value) * -0.5f;
            } else {
                ret = pow_4(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelAccel: // 13
            value += value;
            if (value < 1.0f) {
                ret = pow_3(1.0f - value) * -0.5f;
            } else {
                ret = pow_3(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelAccelSlow: // 12
            value += value;
            if (value < 1.0f) {
                ret = pow_2(1.0f - value) * -0.5f;
            } else {
                ret = pow_2(value - 1.0f) * 0.5f;
            }
            return ret + 0.5f;
        case DecelerateSine: // 18
            return sinf(value * HALF_PI_f);
        case AccelerateSine: // 19
            return 1.0f - sinf(value * HALF_PI + HALF_PI);
        case DecelAccelSine: // 20
            ret = sinf(value * PI_f);
            if (value < 0.5f) {
                return ret * 0.5f;
            } else {
                return (1.0f - ret) * 0.5f + 0.5f;
            }
        case AccelDecelSine: // 21
            value += value;
            if (value < 1.0f) {
                ret = (1.0f - sinf(value * HALF_PI + HALF_PI)) * 0.5f;
            } else {
                ret = sinf((value - 1.0f) * HALF_PI_f) * 0.5f + 0.5f;
            }
            return ret;
        case AccelParabolaA: // 22
            return parabola(value, 0.25f);
        case AccelParabolaB: // 23
            return parabola(value, 0.30f);
        case AccelParabolaC: // 24
            return parabola(value, 0.35f);
        case AccelParabolaD: // 25
            return parabola(value, 0.38f);
        case AccelParabolaE: // 26
            return parabola(value, 0.40f);
        case DecelParabolaA: // 27
            return 1.0f - parabola(valueR, 0.25f);
        case DecelParabolaB: // 28
            return 1.0f - parabola(valueR, 0.30f);
        case DecelParabolaC: // 29
            return 1.0f - parabola(valueR, 0.35f);
        case DecelParabolaD: // 30
            return 1.0f - parabola(valueR, 0.38f);
        case DecelParabolaE: // 31
            return 1.0f - parabola(valueR, 0.40f);
    }
}