#include "force_macros.h"
#include <stdio.h>
#include <math.h>

#include "util.h"

#pragma clang diagnostic ignored "-Wunused-variable"

gnu_noinline void assume_non_vector_registers_volatile() {
	_mm_empty();
	static volatile double yeetus = 1.0f;
	static volatile char deleetus[512];
	static volatile int your_registers = 5;
}

gnu_noinline void assume_all_registers_volatile() {
	//_mm256_zeroall();
	static volatile double yeetus = 1.0f;
	static volatile char deleetus[512];
	static volatile int your_registers = 5;
	yeetus = sin(cos(tan(yeetus)));
	your_registers = your_registers + snprintf((char*)deleetus, countof(deleetus), "%f", yeetus);
}

gnu_noinline void assume_all_registers_volatile(void* use_arg) {
	//_mm256_zeroall();
	static volatile double yeetus = 1.0f;
	static volatile char deleetus[512];
	static volatile int your_registers = 5;
	yeetus = sin(cos(tan(yeetus)));
	your_registers = your_registers + snprintf((char*)deleetus, countof(deleetus), "%f%p", yeetus, use_arg);
}