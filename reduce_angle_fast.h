#pragma once
#include "zero/FloatConstants.h"
#include "zero/util.h"
dllexport inline float reduce_angle_fast(float value) {
	vec<double, 2> temp = { (double)value * ONE_OVER_TWO_PI_d };
//#if __SSE4_1__
	//vec<double, 2> temp2 = bitcast<vec<double, 2>>(_mm_round_sd(*(__m128d*) & temp, *(__m128d*) & temp, 0x8));
//#else
	vec<double, 2> temp2 = {};
//#endif
	temp[0] -= temp2[0];
	return temp[0] * TWO_PI_d;
};