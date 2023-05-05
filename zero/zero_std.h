
#include <stdlib.h>
#include <stdint.h>
#include <util.h>
#include <custom_intrin.h>

template <typename T>
struct zalloc_no_size {
	T *restrict buffer;
};

template <typename T, typename S = size_t>
struct zalloc_sized : zalloc_no_size<T> {
	S size;
};


namespace std {
	template <typename T, typename S = size_t>
	struct hash<zalloc_sized<T,S>> {

	};
}