#include "force_macros.h"
#include <stdio.h>
//#include <math.h>

#include <limits>

#include "util.h"

#pragma clang diagnostic ignored "-Wunused-variable"

#ifndef DONT_COMPILE_REGISTER_ASSUME_JANK

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
#ifndef STOP_LINKING_DEFAULT_LIB_PLZ
	//yeetus = sin(cos(tan(yeetus)));
	your_registers = your_registers + snprintf((char*)deleetus, countof(deleetus), "%f", yeetus);
#endif
}

gnu_noinline void assume_all_registers_volatile(void* use_arg) {
	//_mm256_zeroall();
	static volatile double yeetus = 1.0f;
	static volatile char deleetus[512];
	static volatile int your_registers = 5;
#ifndef STOP_LINKING_DEFAULT_LIB_PLZ
	//yeetus = sin(cos(tan(yeetus)));
	your_registers = your_registers + snprintf((char*)deleetus, countof(deleetus), "%f%p", yeetus, use_arg);
#endif
}

#endif

/*

struct StrConvFlags {
	uint8_t known_base = 0;
	bool skip_whitespace = true;
	bool skip_sign = false;
};

template<typename T, uint8_t known_base, bool enable_relative, bool skip_whitespace, bool check_sign>
static std::pair<T, const char*> regcall strtonum_bad(const char* str, size_t base) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint8_t number_base = 10;
	bool negate = false;
	bool is_relative = false;
	bool is_max = false;
	uint8_t c;
	if constexpr (skip_whitespace || check_sign) {
	pre_number_loop:
		switch (c = *str_read) {
			case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
				if constexpr (!skip_whitespace) {
					++str_read;
					goto pre_number_loop;
				} else {
					goto fail;
				}
			case '-':
				if constexpr (check_sign) {
					negate = true;
					break;
				} else {
					goto fail;
				}
			case '+':
				if constexpr (check_sign) {
					break;
				} else {
					goto fail;
				}
			default:
				goto fail;
			case 'r': case 'R':
				if constexpr (enable_relative) {
					goto is_leading_r;
				} else {
					goto fail;
				}
			case '0':
				goto is_leading_zero;
			case '1' ... '9':
				goto start_number;
		}
	}
	switch (c = *++str_read) {
		case 'r': case 'R': is_leading_r:
	if constexpr (!enable_relative) {
		goto fail;
	}
	is_relative = true;
		case '0': is_leading_zero:
	if constexpr (known_base == 16) {
		switch (*++str_read) {
			case 'x': case 'X':
				c = *++str_read;
				break;
			default:
				goto fail;
		}
		break;
	} else if constexpr (known_base == 2) {
		switch (*++str_read) {
			case 'b': case 'B':
				c = *++str_read;
				break;
			default:
				goto fail;
		}
		break;
	} else if constexpr (known_base == 10) {
		switch (c = *++str_read) {
			case '0' ... '9':
				break;
			default:
				goto fail;
		}
		break;
	} else {
		switch (c = *++str_read) {
			default:
				goto fail;
			case 'x': case 'X':
				number_base = 16;
				c = *++str_read;
				break;
			case 'b': case 'B':
				number_base = 2;
				c = *++str_read;
				break;
			case '0' ... '9':
				break;
		}
		break;
	}
		case '1' ... '9':
			break;
	}
start_number:
	T ret = 0;
	for (;;) {
		uint32_t digit = c;
		switch (digit) {
			case '0' ... '9':
				digit -= '0';
				break;
			case 'a' ... 'f':
				digit -= 'a' - 10;
				break;
			case 'A' ... 'F':
				digit -= 'A' - 10;
				break;
			default:
				goto end_parse;
		}
		if (digit >= number_base) {
			break;
		}
		c = *++str_read;
		if (!is_max) {
			if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
				if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
					continue;
				}
			}
			is_max = true;
		}
	}
end_parse:
	if (!is_max) {
		if constexpr (check_sign) {
			if (negate) {
				ret = -ret;
			}
		}
		if constexpr (enable_relative) {
			if (is_relative) {
				ret += base;
			}
		}
		return { ret, str_read };
	}
fail:
	return { std::numeric_limits<T>::max, str };
}

*/