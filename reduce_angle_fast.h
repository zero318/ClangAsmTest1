#pragma once
#include "zero/FloatConstants.h"
#include "zero/util.h"
dllexport inline float reduce_angle_fast(float value) {
	vec<double, 2> temp = { (double)value * ONE_OVER_TWO_PI_d };
	vec<double, 2> temp2 = bitcast<vec<double, 2>>(_mm_round_sd(*(__m128d*)&temp, *(__m128d*)&temp, 0x8));
	temp[0] -= temp2[0];
	return temp[0] * TWO_PI_d;
};