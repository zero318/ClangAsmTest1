
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <type_traits>

#include <bit>

#include <intrin.h>
#include <uchar.h>

#include "zero/util.h"


#include "zero/custom_intrin.h"



dllexport char16_t* utf8_to_utf16(const char* utf8_str) {
	if (utf8_str) {
		size_t utf8_len = strlen(utf8_str) + 1;
		char16_t* utf16_str = (char16_t*)malloc(utf8_len * sizeof(char16_t));
		mbstate_t state;
		memset(&state, 0, sizeof(state));
		char16_t* utf16_write = utf16_str - 1;
		size_t c;
		while (1) {
			switch (c = mbrtoc16(++utf16_write, utf8_str, utf8_len, &state)) {
				case 0:								return utf16_str;
				case (size_t)-1: case (size_t)-2:	goto BreakAll;
				case (size_t)-3:					continue;
				default:							utf8_str += c; utf8_len -= c;
			}
		}
	BreakAll:
		free(utf16_str);
	}
	return NULL;
}

dllexport char16_t* utf8_to_utf16_bit_test_reset(const char* utf8_str) {
	if (utf8_str) {
		char16_t* utf16_str = (char16_t*)malloc((strlen(utf8_str) + 1) * sizeof(char16_t));
		char16_t* utf16_write = utf16_str;

		uint32_t codepoint = 0;
		int8_t multibyte_index = -1; // Signed = Single byte, Unsigned = Multibyte
		bool is_four_bytes = false;
		while (1) {
			uint32_t cur_byte = (uint8_t)*utf8_str++; // Zero Extend
			if (!_bittestandreset((long*)&cur_byte, 7)) {
				*utf16_write++ = cur_byte; // Single byte character
				if (cur_byte != '\0') continue;
				if (multibyte_index >= 0) break; // ERROR: Unfinished multi-byte sequence
				return utf16_str;
			}
			if (_bittestandreset((long*)&cur_byte, 6)) { // Leading byte
				if (multibyte_index >= 0) break; // ERROR: Leading byte before end of previous sequence
				multibyte_index = 1 + _bittestandreset((long*)&cur_byte, 5); // Test 3 byte bit
				if (multibyte_index == 2) {
					is_four_bytes = _bittestandreset((long*)&cur_byte, 4); // Test 4 byte bit
					if (is_four_bytes & (bool)_bittest((long*)&cur_byte, 3)) break; // ERROR: No 5 byte sequences
					multibyte_index += is_four_bytes;
				}
			}
			if (multibyte_index < 0) break; // ERROR: Trailing byte before leading byte
			codepoint |= cur_byte << 6 * (uint8_t)multibyte_index; // Accumulate to codepoint
			if (--multibyte_index < 0) { // Sequence is finished
				if (!is_four_bytes) {
					*utf16_write++ = codepoint;
					codepoint = 0;
				} else {
					is_four_bytes = false;
					*(uint32_t*)utf16_write = 0xDC00D800 | // Surrogate Base
						(codepoint & ~0x103FF) >> 10 | // High Surrogate
						(codepoint & 0x3FF) << 16; // Low Surrogate
					utf16_write += 2;
					codepoint = 0;
				}
			}
		}
		free(utf16_str);
	}
	return NULL;
}

#if __INTELLISENSE__
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif
#include <bit>

dllexport char16_t* fastcall utf8_to_utf16_new_test(const char* utf8_str) {

	if (expect_chance(utf8_str != NULL, true, 1.0)) {
		char16_t* utf16_str = (char16_t*)malloc(sizeof(char16_t[byteloop_strlen(utf8_str) + 1]));
		//char16_t* utf16_write = utf16_str;
		char16_t* utf16_write;
		__asm__ volatile (
			""
			: "=b"(utf16_write)
			: "0"(utf16_str)
		);

		for (;;) {
			int32_t codepoint = *utf8_str++; // Zero Extend
			if (expect_chance(codepoint < 0, true, 0.1)) {
				uint32_t leading_bits = std::countl_one((uint32_t)codepoint);
				size_t index = leading_bits - 26;
				if (expect_chance(index >= 2, false, 1.0)) break;
				codepoint &= (uint32_t)-1 >> leading_bits;
				//codepoint &= mask_table[index];
				do {
					switch (int8_t next_byte = *utf8_str++) {
						case -128 ... - 65:
							next_byte &= 0x3F;
							codepoint = codepoint << 6 | next_byte;
							break;
						default:
							goto error_break;
					}
				} while (((ssize_t)(--index) >= 0));
			}
			*utf16_write++ = codepoint; // Single byte character
			if (expect_chance(codepoint == '\0', true, 0.05)) {
				return utf16_str;
			}
		}
error_break:
		free(utf16_str);
	}
	return NULL;
}

dllexport char16_t* utf8_to_utf16_bit_scan(const char* utf8_str) {
	if (utf8_str) {
		char16_t* utf16_str = (char16_t*)malloc((strlen(utf8_str) + 1) * sizeof(char16_t));
		char16_t* utf16_write = utf16_str;

		uint32_t codepoint = 0;
		int8_t multibyte_index = -1; // Signed = Single byte, Unsigned = Multibyte
		bool is_four_bytes = false;
		while (1) {
			int8_t cur_byte = *utf8_str++;
			uint32_t pad_cur_byte = (uint8_t)cur_byte; // Zero Extend
			if (cur_byte >= 0) {
				*utf16_write++ = pad_cur_byte; // Single byte character
				if (cur_byte != '\0') continue;
				if (multibyte_index >= 0) break; // ERROR: Unfinished multi-byte sequence
				return utf16_str;
			}
			uint32_t BitScan = (uint8_t)~cur_byte; // Flip bits THEN Zero Extend
			(void)_BitScanReverse((unsigned long*)&BitScan, BitScan); // Count leading zero bits
			uint8_t BitIndex = 6 - BitScan; // Convert to multibyte index
			pad_cur_byte &= 0b01111111u >> BitIndex;
			if (BitIndex) { // Leading byte
				if (multibyte_index >= 0) break; // ERROR: Leading byte before end of previous sequence
				if (BitIndex > 3) break; // ERROR: No 5 byte sequences
				is_four_bytes = BitIndex == 3;
				multibyte_index = BitIndex;
			}
			if (multibyte_index < 0) break; // ERROR: Trailing byte before leading byte
			codepoint |= pad_cur_byte << 6 * (uint8_t)multibyte_index; // Accumulate to codepoint
			if (--multibyte_index < 0) { // Sequence is finished
				if (!is_four_bytes) {
					*utf16_write++ = codepoint;
					codepoint = 0;
				} else {
					is_four_bytes = false;
					*(uint32_t*)utf16_write = 0xDC00D800 | // Surrogate Base
						(codepoint & ~0x103FF) >> 10 | // High Surrogate
						(codepoint & 0x3FF) << 16; // Low Surrogate
					utf16_write += 2;
					codepoint = 0;
				}
			}
		}
		free(utf16_str);
	}
	return NULL;
}

dllexport char16_t* utf8_to_utf16_masking(const char* utf8_str) {
	if (utf8_str) {
		char16_t* utf16_str = (char16_t*)malloc((strlen(utf8_str) + 1) * sizeof(char16_t));
		char16_t* utf16_write = utf16_str;

		uint32_t temp;
#define BitMask(value, mask) \
		((bool)((value) & (mask)))
#define BitMaskAndReset(value, mask) \
		(temp = (value), temp != ((value) &= ~(mask)))
#define BitMaskAndSet(value, mask) \
		(temp = (value), temp != ((value) |= (mask)))
#define BitMaskAndComplement(value, mask) \
		(temp = (value), temp != ((value) ^= (mask)))

		uint32_t codepoint = 0;
		int8_t multibyte_index = -1; // Signed = Single byte, Unsigned = Multibyte
		bool is_four_bytes = false;
		while (1) {
			int32_t cur_byte = *utf8_str++; // Sign Extend
			if (cur_byte >= 0) {
				*utf16_write++ = cur_byte; // Single byte character
				if (cur_byte != '\0') continue;
				if (multibyte_index >= 0) break; // ERROR: Unfinished multibyte sequence
				return utf16_str;
			}
			cur_byte &= 0b01111111; // Get rid of the sign extended bits
			if (BitMaskAndReset(cur_byte, 0b01000000)) { // Leading byte
				if (multibyte_index >= 0) break;  // ERROR: Leading byte before end of previous sequence
				multibyte_index = 1 + BitMaskAndReset(cur_byte, 0b00100000); // Test 3 byte bit
				if (multibyte_index == 2) {
					is_four_bytes = BitMaskAndReset(cur_byte, 0b00010000); // Test 4 byte bit
					if (is_four_bytes & BitMask(cur_byte, 0b00001000)) break; // ERROR: No 5 byte sequences
					multibyte_index += is_four_bytes;
				}
			}
			if (multibyte_index < 0) break; // ERROR: Trailing byte before leading byte
			codepoint |= cur_byte << 6 * (uint8_t)multibyte_index; // Accumulate to codepoint
			if (--multibyte_index < 0) { // Sequence is finished
				if (!is_four_bytes) {
					*utf16_write++ = codepoint;
					codepoint = 0;
				} else {
					is_four_bytes = false;
					*(uint32_t*)utf16_write = 0xDC00D800 | // Surrogate Base
						(codepoint & ~0x103FF) >> 10 | // High Surrogate
						(codepoint & 0x3FF) << 16; // Low Surrogate
					utf16_write += 2;
					codepoint = 0;
				}
			}
		}
		free(utf16_str);
	}
	return NULL;
}

dllexport char32_t* utf8_to_utf32(const char* utf8_str) {
	if (utf8_str) {
		size_t utf8_len = strlen(utf8_str) + 1;
		char32_t* utf32_str = (char32_t*)malloc(utf8_len * sizeof(char32_t));
		mbstate_t state;
		memset(&state, 0, sizeof(state));
		char32_t* utf32_write = utf32_str - 1;
		size_t c;
		while (1) {
			switch (c = mbrtoc32(++utf32_write, utf8_str, utf8_len, &state)) {
				case 0:								return utf32_str;
				case (size_t)-1: case (size_t)-2:	goto BreakAll;
				case (size_t)-3:					continue;
				default:							utf8_str += c; utf8_len -= c;
			}
		}
	BreakAll:
		free(utf32_str);
	}
	return NULL;
}

#ifdef _M_IX86
extern "C" {
	//extern char32_t* fastcall utf8_to_utf32_asm(const char8_t* input_string);
}

//dllexport char32_t* fastcall utf8_to_utf32_asm_wrap(const char8_t* input_string) {
	//return utf8_to_utf32_asm(input_string);
//}
#endif

#define char8_t char

dllexport char32_t* utf8_to_utf32_new(const char* utf8_str) {
	
	union U8Decode_t {
		char8_t raw_value;
		struct {
			char8_t data : 6;
			char8_t header : 2;
		} continuation;
		// first byte
		union {
			char8_t four_byte_data : 3;
			char8_t three_byte_data : 4;
			char8_t two_byte_data : 5;
			char8_t one_byte_data : 7;
			struct {
				char8_t : 3;
				char8_t has_error : 1;
				char8_t has_four_bytes : 1;
				char8_t has_three_bytes : 1;
				char8_t has_two_bytes : 1;
				char8_t is_multibyte : 1;
			} header;
		};
	};
	
	const U8Decode_t* utf8_read = (U8Decode_t*)utf8_str;
	
	size_t utf8_len = strlen(utf8_str);
	//char32_t utf32_str[utf8_len * 4];
	char32_t* utf32_str = (char32_t*)malloc(utf8_len * sizeof(char32_t) + sizeof(char32_t));
	utf32_str[utf8_len++] = U'\0';
	char32_t* utf32_write = utf32_str;
	uint32_t utf32_temp = 0;

#define TestAndSet(value, expect_val) \
bool temp = (value); ((value) = true), expect(temp, (expect_val))
#define TestAndReset(value, expect_val) \
bool temp = (value); ((value) = false), expect(temp, (expect_val))

	uint8_t multibyte_len = 0;
	do {
		U8Decode_t utf8_temp = *utf8_read++;
		if (TestAndReset(utf8_temp.header.is_multibyte, false)) {
			if (TestAndReset(utf8_temp.header.has_two_bytes, false)) {
				++multibyte_len;
				if (TestAndReset(utf8_temp.header.has_three_bytes, false)) {
					++multibyte_len;
					if (TestAndReset(utf8_temp.header.has_four_bytes, false)) {
						++multibyte_len;
					}
				}
			}
		}
		utf32_temp = utf32_temp << 7 | utf8_temp.raw_value;
		if (expect(!multibyte_len, true)) {
			*utf32_write++ = utf32_temp;
			utf32_temp = 0;
		}
	} while (--utf8_len);

	return utf32_str;
}

//#undef _BIT_
/*
#include <bit>
dllexport char8_t* fastcall utf32_to_utf8(const char32_t* input_string, char8_t* out) {
	
#define UTF8_CONT (0x80)
#define UTF8_CONT_MASK (0x3F)
	uint32_t temp, temp2;
	do {
		temp = temp2 = *input_string++;
		uint32_t byte_count = 0;
		switch (uint32_t count = __lzcnt32(temp)) {
			default: unreachable;
			case 1 ... 24: {
				byte_count = count = (count + 4) / 5;
				int8_t byte_mask = -128;
				byte_mask >>= count;
				do {
					out[count] = UTF8_CONT | UTF8_CONT_MASK & temp;
					temp >>= 6;
				} while (--count);
				temp |= byte_mask;
			}
			case 25 ... 32:
				out[0] = temp;
		}
		out += byte_count + 1;
	} while (temp2);
	return out;
}
*/