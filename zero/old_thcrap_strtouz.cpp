#ifndef TH_X64
uint64_t
#else
std::pair<size_t, const char*>
#endif
TH_VECTORCALL str_to_num(size_t base, const char* str) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint8_t number_base = 10u;
	size_t ret_max = SIZE_MAX / 10u;
	uint8_t flags = 0;
	constexpr uint8_t is_negate = 0x01;
	constexpr uint8_t is_relative = 0x02;
	constexpr uint8_t is_max = 0x80;
	uint8_t c;
	union {
		uint64_t wide;
		struct {
			size_t number;
			const char* end_str;
		};
	} wide_ret;
//pre_number_loop:
	switch (c = *str_read) {
		//case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
			//++str_read;
			//goto pre_number_loop;
		//case '-':
			//flags = is_negate;
			//TH_FALLTHROUGH;
		//case '+':
			c = *++str_read;
			if ((uint8_t)(c - '1') < 9u) {
				break;
			} else if (c == '0') {
				goto is_leading_zero;
			} else if ((uint8_t)(c | 0x20u) != 'r') {
		default: fail:
#ifndef TH_X64
				wide_ret.number = SIZE_MAX;
				wide_ret.end_str = str;
				return wide_ret.wide;
				//return ((uint64_t)(uint32_t)str << 32) | SIZE_MAX;
#else
				return std::make_pair(SIZE_MAX, str);
#endif
			}
			TH_FALLTHROUGH;
		case 'r': case 'R':
			flags |= is_relative;
			TH_FALLTHROUGH;
		case '0': is_leading_zero:
			c = *++str_read;
			if ((uint8_t)(c - '0') >= 10u) {
				uint8_t temp = c | 0x20u;
				if (temp == 'x') {
					number_base = 16u;
					ret_max = SIZE_MAX / 16u;
				} else if (temp == 'b') {
					number_base = 2u;
					ret_max = SIZE_MAX / 2u;
				} else {
					goto fail;
				}
				c = *++str_read;
			}
			TH_FALLTHROUGH;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			TH_FALLTHROUGH;
	}
	size_t ret = 0;
	str = (const char*)str_read;
	for (
		uint8_t digit = c;
		;
		digit = (uint8_t)*++str
	) {
		digit -= '0';
		if (digit >= 10u) {
			digit -= 'A' - '0';
			if (digit >= 6u) {
				digit -= 'a' - 'A';
				if (digit >= 6u) {
					break;
				}
			}
		}
		if ((uint8_t)digit >= number_base) {
			break;
		}
		if (!(flags & is_max)) {
			if (ret <= ret_max) {
				ret *= number_base;
				if (size_t temp = ret; temp <= (ret += digit)) {
					continue;
				}
			}
			flags = is_max;
		}
	}
end_parse:
	if (!(flags & is_max)) {
		if (flags & is_negate) {
			ret = -ret;
		}
#ifndef TH_X64
		ret += (flags << 30) >> 31 & base;
#else
		ret += ((int64_t)flags << 62) >> 63 & base;
#endif
		//if (flags & is_relative) {
			//ret += base;
		//}
#ifndef TH_X64
		//uint64_t wide_ret_high = (uint64_t)(uint32_t)str_read << 32;
		//TH_ASSUME(wide_ret_high > UINT32_MAX);
		//return wide_ret_high | ret;
		wide_ret.number = ret;
		wide_ret.end_str = (const char*)str;
		return wide_ret.wide;
		//return ((uint64_t)(uint32_t)str_read << 32) + ret;
#else
		return std::make_pair(ret, (const char*)str_read);
#endif
	}
}