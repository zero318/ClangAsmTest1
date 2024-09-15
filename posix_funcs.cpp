size_t strnlen(const char* str, size_t count) {
	size_t length = 0;
	if (count) {
		while (str[length] && ++length < count);
	}
	return length;
}

size_t strlcpy(char* dst, const char* src, size_t count) {
	size_t src_length = strlen(src);
	if (count) {
		size_t copy_length = count - 1;
		if (src_length <= copy_length) {
			copy_length = src_length;
		}
		dst[copy_length] = '\0';
		memcpy(dst, src, copy_length);
	}
	return src_length;
}

size_t strlcat(char* dst, const char* src, size_t count) {
	size_t dst_length = strnlen(dst, count);
	size_t src_length = strlen(src);
	
	if (src_length) {
		size_t remaining_length = count - dst_length;
		if (remaining_length > 1) {
			dst += dst_length;
			size_t copy_length = remaining_length - 1;
			if (src_length <= copy_length) {
				copy_length = src_length;
			}
			dst[copy_length] = '\0';
			memcpy(dst, src, copy_length);
		}
	}
	return dst_length + src_length;
}

char* stpcpy(char* dst, const char* src) {
	src -= dst;
	while ((*dst = *(src + dst)) ++dst;
	return dst;
}

char* stpncpy(char* dst, const char* src, size_t count) {
	if (count) {
		do {
			char c = *src++;
			if (c) {
				*dst++ = c;
			} else {
				memset(dst, 0, count);
				break;
			}
		} while (--count);
	}
	return dst;
}