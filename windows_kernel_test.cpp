#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "zero/util.h"
#include "zero/custom_intrin.h"

#include "x86_msrs.h"

#include "F:/My Hacking Stuff Expansion/windows/ntoskrnl_structs.h"

#include <initializer_list>

dllexport void msr_test() {
	MSR_STAR star_value_a;
	star_value_a.address = (void*)0xDEADBEEF;
	star_value_a.call_segment = 0x10;
	star_value_a.ret_segment = 0x23;
	MSR_STAR::write(star_value_a);

	MSR_STAR::write((void*)0xDEADBEEF, 0x10, 0x23);

	MSR_STAR::write_raw_wide(MSR_STAR::read_raw_wide());
}

#define SYSCALL_TABLE_SIZE 401

// Last 7 are unused
uint8_t SYSCALL_ARGUMENT_TABLE[] = {
	0x00, 0x00, 0x00, 0x14, 0x18, 0x14, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x00,
	0x08, 0x00, 0x00, 0x04, 0x08, 0x08, 0x04, 0x04, 0x14, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x08, 0x00, 0x04, 0x00, 0x04, 0x18, 0x1C, 0x00, 0x04, 0x00, 0x00, 0x14, 0x04, 0x00, 0x00, 0x08,
	0x08, 0x00, 0x1C, 0x00, 0x00, 0x04, 0x18, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x04, 0x00, 0x08, 0x08,
	0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 0x04, 0x0C, 0x00, 0x00, 0x14, 0x10, 0x00, 0x04, 0x04, 0x00,
	0x00, 0x08, 0x1C, 0x04, 0x08, 0x00, 0x30, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x0C,
	0x08, 0x10, 0x1C, 0x1C, 0x30, 0x34, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14,
	0x00, 0x1C, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x04,
	0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0x28, 0x00, 0x04, 0x00, 0x10,
	0x14, 0x18, 0x0C, 0x04, 0x00, 0x1C, 0x00, 0x24, 0x18, 0x08, 0x1C, 0x04, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08,
	0x04, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x14, 0x18, 0x20, 0x10, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x20, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0C, 0x00, 0x14, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x04, 0x04,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x14, 0x08, 0x04, 0x04, 0x00, 0x00, 0x04, 0x08, 0x00, 0x08, 0x00,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

enum FlagType {
	SetAlignmentFlag,
	ClearAlignmentFlag,
	SetInterruptFlag,
	ClearAlignmentFlag,

};
dllexport void syscall_flags_change() {

}

enum SystemRegisterType {

};
union SystemRegisterAction {
	uint16_t raw;
	struct {
		uint8_t register_number;
		uint8_t register_type;
	};
};
dllexport void syscall_system_register(
	SystemRegisterAction action,
	void* data,
	size_t data_size
) {
	uint64_t temp;
	switch (action.raw) {
		case 0:
			__asm__ volatile (
				"mov %[temp], %%cr0"
				: asm_arg("=r", temp)
			);
			if (data_size < sizeof(uint64_t)) break;
			*(uint64_t*)data = temp;
			return;
	}
}

dllexport uint128_t syscall_read_msr(
	uint32_t index
) {
	union {
		uint128_t wide;
		struct {
			uint32_t low;
			uint32_t mid;
			uint64_t high;
		};
	} raw;
	__asm__ volatile (
		"rdmsr"
		: "=a"(raw.low), "=d"(raw.high)
		: "c"(index)
	);
	raw.mid = raw.high;
	return raw.wide;
}

dllexport void syscall_write_msr(
	uint32_t index,
	uint64_t value
) {
	uint32_t low = (uint32_t)value;
	value <<= 32;
	__asm__ volatile (
		"wrmsr"
		:
		: "a"(low), "d"(value), "c"(index)
	);
}