#pragma once

#include <immintrin.h>

__attribute__((noinline, sseregparm)) __m128 sincosf_sse2(float x);