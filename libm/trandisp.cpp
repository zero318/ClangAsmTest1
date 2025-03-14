#include <Windows.h>

#if __INTELLISENSE__
#define _HAS_CXX17 1
#define _HAS_CXX20 0
#endif

#include <type_traits>

#if __INTELLISENSE__
#define _HAS_CXX17 1
#define _HAS_CXX20 1
#endif

#include <bit>
#include <thread>

#if __INTELLISENSE__
#define _HAS_CXX17 1
#define _HAS_CXX20 0
#endif

#include "../znet/zthread.h"
using namespace zero::thread;

#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "../zero/util.h"
#include "libm_common.h"

double vectorcall fmod_x87(double x, double y) {
	double ret;
	long double junk;
	__asm__(
		ASM_STACK_ENTER8
		ASM_SSE_WRITE_DBL2
		ASM_X87_READ_DBL
		ASM_SSE_WRITE_DBL
		ASM_X87_READ_DBL
		ASM_FPREM_LOOP
		ASM_X87_WRITE_DBL
		ASM_SSE_READ_DBL
		"FFREEP %%ST(0) \n"
		ASM_STACK_LEAVE8
		: [ret]"=x"(ret)
		: [x]"x"(x), [y]"x"(y)
		: "ax", "st", "st(1)"
	);
	return ret;
}

double vectorcall remainder_x87(double x, double y) {
	double ret;
	long double junk;
	__asm__(
		ASM_STACK_ENTER8
		ASM_SSE_WRITE_DBL2
		ASM_X87_READ_DBL
		ASM_SSE_WRITE_DBL
		ASM_X87_READ_DBL
		ASM_FPREM1_LOOP
		ASM_X87_WRITE_DBL
		ASM_SSE_READ_DBL
		"FFREEP %%ST(0) \n"
		ASM_STACK_LEAVE8
		: [ret]"=x"(ret)
		: [x]"x"(x), [y]"x"(y)
		: "ax", "st", "st(1)"
	);
	return ret;
}

double vectorcall remquo_x87(double x, double y, int* quo) {
	double ret;
	long double junk;
	__asm__(
		ASM_STACK_ENTER8
		ASM_SSE_WRITE_DBL2
		ASM_X87_READ_DBL
		ASM_SSE_WRITE_DBL
		ASM_X87_READ_DBL
		ASM_FPREM1_LOOP

		ASM_X87_WRITE_DBL
		ASM_SSE_READ_DBL
		"FFREEP %%ST(0) \n"
		ASM_STACK_LEAVE8
		: [ret] "=x"(ret)
		: [x] "x"(x), [y]"x"(y), [quo]"c"(quo)
		: "ax", "st", "st(1)"
	);
	return ret;
}

dllexport gnu_noinline double vectorcall fmod_sse2(double x, double y) {
	if (expect(!__builtin_isnan(y) && !__builtin_isnan(x), true)) {
		if (expect(fabs(x) == PINFINITY_DOUBLE || y == 0.0, false)) {
			return INDEFINITE_DOUBLE;
		}
		if (expect(fabs(y) == PINFINITY_DOUBLE, false)) {
			return __builtin_isnormal(x) ? x : INDEFINITE_DOUBLE;
		}
		if (expect(x == 0.0, false)) {
			return 0.0;
		}
		return fmod_x87(x, y);
	}
	return y + x;
}

dllexport gnu_noinline double vectorcall remainder_sse2(double x, double y) {
	if (expect(!__builtin_isnan(y) && !__builtin_isnan(x), true)) {
		if (expect(fabs(x) == PINFINITY_DOUBLE || y == 0.0, false)) {
			return PNAN_DOUBLE;
		}
		if (expect(fabs(y) == PINFINITY_DOUBLE, false)) {
			return __builtin_isnormal(x) ? x : PNAN_DOUBLE;
		}
		if (expect(x == 0.0, false)) {
			return 0.0;
		}
		return remainder_x87(x, y);
	}
	return y + x;
}

dllexport gnu_noinline double vectorcall remquo_sse2(double x, double y, int* quo) {
	if (expect(!__builtin_isnan(y) && !__builtin_isnan(x), true)) {
		if (expect(fabs(x) == PINFINITY_DOUBLE || y == 0.0, false)) {
			return PNAN_DOUBLE;
		}
		if (expect(fabs(y) == PINFINITY_DOUBLE, false)) {
			return __builtin_isnormal(x) ? x : PNAN_DOUBLE;
		}
		if (expect(x == 0.0, false)) {
			return 0.0;
		}
		return remainder_x87(x, y);
	}
	return y + x;
}

#define TOTAL_TESTS 4278190080
#define TEST_FORMAT "%" PRIu32
using test_count_t = uint32_t;

#define FOUR_TEST_THREADS 1

#if FOUR_TEST_THREADS
#define PP_INDEX 0
#define PN_INDEX 1
#define NP_INDEX 2
#define NN_INDEX 3
#else
#define PP_INDEX 0
#define PN_INDEX 0
#define NP_INDEX 1
#define NN_INDEX 1
#endif

#define TEST_PRECISION 1

#define SINGLE_X_VALUE 0
#define SINGLE_X_BITS 0x47245551
#define SINGLE_Y_VALUE 1
#define SINGLE_Y_BITS 0x47245551


uint32_t counters[4] = {};

void test_float(uint32_t& counter, uint32_t x_bits, uint32_t y_bits) {
	float x = std::bit_cast<float>(x_bits);
	float y = std::bit_cast<float>(y_bits);

	float correct_remainder = remainder(x, y);
	//float correct_remainder = fmod(x, y);
	uint32_t correct_remainder_bits = std::bit_cast<uint32_t>(correct_remainder);

	float test_remainder = remainder_sse2(x, y);
	//float test_remainder = fmod_sse2(x, y);
	uint32_t test_remainder_bits = std::bit_cast<uint32_t>(test_remainder);

	if (correct_remainder_bits != test_remainder_bits) __asm int3
	counter += correct_remainder_bits != test_remainder_bits;
}

void test_two_float_funcs() {
	zjthread pp_thread([](const std::atomic<uint8_t>& stop) {
#if !SINGLE_X_VALUE
		for (
			uint32_t valueX = std::bit_cast<uint32_t>(minimum_positive_float_value);
			valueX <= std::bit_cast<uint32_t>(maximum_positive_float_value);
			valueX += TEST_PRECISION
		)
#else
		uint32_t valueX = SINGLE_X_BITS;
#endif
		{
			{
#if !SINGLE_Y_VALUE
				for (
					uint32_t valueY = std::bit_cast<uint32_t>(minimum_positive_float_value);
					valueY <= std::bit_cast<uint32_t>(maximum_positive_float_value);
					valueY += TEST_PRECISION
				)
#else
				uint32_t valueY = SINGLE_Y_BITS;
#endif
				{
					test_float(counters[PP_INDEX], valueX, valueY);
				}
			}
#if !FOUR_TEST_THREADS
#if SINGLE_Y_VALUE
			continue;
#endif
#else
		}
	});
	zjthread pn_thread([](const std::atomic<uint8_t>& stop) {
#if !SINGLE_X_VALUE
		for (
			uint32_t valueX = std::bit_cast<uint32_t>(minimum_positive_float_value);
			valueX <= std::bit_cast<uint32_t>(maximum_positive_float_value);
			valueX += TEST_PRECISION
		)
#else
		uint32_t valueX = SINGLE_X_BITS;
#endif
		{
#endif
			{
#if !SINGLE_Y_VALUE
				for (
					uint32_t valueY = std::bit_cast<uint32_t>(minimum_negative_float_value);
					valueY >= std::bit_cast<uint32_t>(maximum_negative_float_value);
					valueY -= TEST_PRECISION
				)
#else
				uint32_t valueY = SINGLE_Y_BITS;
#endif
				{
					test_float(counters[PN_INDEX], valueX, valueY);
				}
			}
		}
	});
	zjthread np_thread([](const std::atomic<uint8_t>& stop) {
#if !SINGLE_X_VALUE
		for (
			uint32_t valueX = std::bit_cast<uint32_t>(minimum_negative_float_value);
			valueX >= std::bit_cast<uint32_t>(maximum_negative_float_value);
			valueX -= TEST_PRECISION
		)
#else
		uint32_t valueX = SINGLE_X_BITS;
#endif
		{
			{
#if !SINGLE_Y_VALUE
				for (
					uint32_t valueY = std::bit_cast<uint32_t>(minimum_positive_float_value);
					valueY <= std::bit_cast<uint32_t>(maximum_positive_float_value);
					valueY += TEST_PRECISION
				)
#else
				uint32_t valueY = SINGLE_Y_BITS;
#endif
				{
					test_float(counters[NP_INDEX], valueX, valueY);
				}
			}
#if !FOUR_TEST_THREADS
#if SINGLE_Y_VALUE
			continue;
#endif
#else
		}
	});
	zjthread nn_thread([](const std::atomic<uint8_t>& stop) {
#if !SINGLE_X_VALUE
		for (
			uint32_t valueX = std::bit_cast<uint32_t>(minimum_negative_float_value);
			valueX >= std::bit_cast<uint32_t>(maximum_negative_float_value);
			valueX -= TEST_PRECISION
		)
#else
		uint32_t valueX = SINGLE_X_BITS;
#endif
		{
#endif
			{
#if !SINGLE_Y_VALUE
				for (
					uint32_t valueY = std::bit_cast<uint32_t>(minimum_negative_float_value);
					valueY >= std::bit_cast<uint32_t>(maximum_negative_float_value);
					valueY -= TEST_PRECISION
				)
#else
				uint32_t valueY = SINGLE_Y_BITS;
#endif
				{
					test_float(counters[NN_INDEX], valueX, valueY);
				}
			}
		}
	});


	while (
		!pp_thread.stopped() ||
#if FOUR_TEST_THREADS
		!pn_thread.stopped() ||
		!nn_thread.stopped() ||
#endif
		!np_thread.stopped()
	) {
		Sleep(10000);
	}

	test_count_t incorrect_count = (test_count_t)counters[0] + (test_count_t)counters[1];
#if FOUR_TEST_THREADS
	incorrect_count += (test_count_t)counters[2] + (test_count_t)counters[3];
#endif

	printf(
		"Total: " MACRO_STR(TOTAL_TESTS) "\n"
		"Fails: " TEST_FORMAT "\n"
		, incorrect_count
	);
}


dllexport double vectorcall fma_test(double x, double y, double z) {
	return fma(x, y, z);
}