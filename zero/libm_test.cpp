#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdint.h>
#include <direct.h>

#include <string.h>

#define _USE_MATH_DEFINES 1
#include <math.h>

#include <bit>

#include "FloatConstants.h"

#include "custom_intrin.h"
#include "util.h"

dllexport gnu_noinline double vectorcall sin_precise(double arg) {
	if (expect((_mm_getcsr() & _MM_MASK_MASK) == _MM_MASK_MASK, true) && expect((store_x87_cw().raw & 0x7F) == 0x7F, true)) {
		
	}
	else {
		unreachable;
	}
}