#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <string_view>
#include <vector>
#include "zero/util.h"
#include "zero/custom_intrin.h"

#define STRING_ALIGNMENT 16
#define BUFFER_PADDING 64

enum ZsonType : uint8_t {
	End = 0,
	Double = 1,
	String = 2,
	Object = 3,
	Array = 4,
	Blob = 5,
	Undefined = 6,
	ObjectID = 7,
	Bool = 8,
	Time = 9,
	Null = 10,
	Regex = 11,
	DBPointer = 12,
	JS = 13,
	Symbol = 14,
	JSScope = 15,
	Int32 = 16,
	Timestamp = 17,
	Int64 = 18,
	Decimal128 = 19
};
enum ZsonBlobType : uint8_t {

};

union ZsonHeaderMeta {
	uint16_t raw; // 0x0
	struct {
		union {
			uint8_t flags; // 0x0
			struct {
				bool aligned : 1;
				bool is_array_member : 1;
				bool : 1;
				bool : 1;
				bool : 1;
				bool needs_free : 1;
				bool dead : 1;
				bool in_memory_format : 1;
			};
		};
		ZsonType type; // 0x1
	};
	struct {
		uint16_t : 7;
		uint16_t size_lookup : 9;
	};
};
ValidateFieldOffset(0x0, ZsonHeaderMeta, raw);
ValidateFieldOffset(0x0, ZsonHeaderMeta, flags);
ValidateFieldOffset(0x1, ZsonHeaderMeta, type);
struct /*alignas(16)*/ ZsonHeader {
	ZsonHeaderMeta meta; // 0x0
	uint8_t __byte_2; // 0x2
	uint8_t __byte_3; // 0x3
	uint32_t key_index; // 0x4
	// 0x8
};
ValidateFieldOffset(0x0, ZsonHeader, meta);
ValidateFieldOffset(0x2, ZsonHeader, __byte_2);
ValidateFieldOffset(0x3, ZsonHeader, __byte_3);
ValidateFieldOffset(0x4, ZsonHeader, key_index);

struct alignas(16) ZsonHeaderAligned : ZsonHeader {};

struct ZsonStringChunk {
	char8_t data[16];
};

struct ZsonString {

};

struct ZsonValue {
	ZsonType type; // 0x0
	uint8_t key_length_in_dwords; // 0x1
	uint16_t data_length_hint;
};

struct ZsonStringPoolEntry {
	uint32_t offset; // 0x0
	uint32_t length; // 0x4
	// 0x8
};
ValidateFieldOffset(0x0, ZsonStringPoolEntry, offset);
ValidateFieldOffset(0x4, ZsonStringPoolEntry, length);
ValidateStructSize(0x8, ZsonStringPoolEntry);

struct ZsonRuntimeStringPool;

struct ZsonStringPool {
	uint32_t count; // 0x0
	uint32_t buffer_size; // 0x4
	ZsonStringPoolEntry entries[]; // 0x8

	dllexport ZsonRuntimeStringPool* make_runtime_pool() const;
};
ValidateFieldOffset(0x0, ZsonStringPool, count);
ValidateFieldOffset(0x4, ZsonStringPool, buffer_size);
ValidateFieldOffset(0x8, ZsonStringPool, entries);

struct ZsonPackedStringPool {
	uint32_t total_size;
	ZsonStringPool pool;
};

struct ZsonRuntimeStringPool {
	uint32_t count; // 0x0
	uint32_t buffer_size; // 0x4
	std::string_view views[]; // 0x8

	dllexport ZsonPackedStringPool* make_packed_pool() const;
};
ValidateFieldOffset(0x0, ZsonRuntimeStringPool, count);
ValidateFieldOffset(0x4, ZsonRuntimeStringPool, buffer_size);
ValidateFieldOffset(0x8, ZsonRuntimeStringPool, views);

dllexport ZsonRuntimeStringPool* ZsonStringPool::make_runtime_pool() const {
	size_t count = (size_t)this->count;
	size_t buffer_size = (size_t)this->buffer_size;
	size_t offset_to_buffer = AlignUpToMultipleOf2(sizeof(ZsonRuntimeStringPool) + sizeof(std::string_view[count]), STRING_ALIGNMENT);
	ZsonRuntimeStringPool* ret = (ZsonRuntimeStringPool*)_aligned_malloc(offset_to_buffer + buffer_size, STRING_ALIGNMENT);
	if (expect(ret != NULL, true)) {
		ret->count = count;
		ret->buffer_size = buffer_size;
		std::string_view* views = ret->views;
		char* buffer_write = based_pointer<char>(ret, offset_to_buffer);
		for (size_t i = 0; i < count; ++i) {
			const ZsonStringPoolEntry& entry = this->entries[i];
			size_t string_length = entry.length;
			views[i] = std::string_view(buffer_write, string_length);
			buffer_write = rep_movsb(buffer_write, based_pointer<char>(this, entry.offset), string_length);
			buffer_write = rep_stosb(buffer_write, 0, AlignUpToMultipleOf2(entry.length, STRING_ALIGNMENT));
			/*
			size_t string_length2 = string_length;
			__asm__ volatile (
				"rep movsb"
				: "+D"(buffer_write), "+c"(string_length)
				: "S"(based_pointer<char>(this, entry.offset))
				: "memory"
			);
			__asm__ volatile (
				"rep stosb"
				: "+D"(buffer_write)
				: "a"(0), "c"(AlignUpToMultipleOf2(string_length, STRING_ALIGNMENT))
				: "memory"
			);
			*/
		}
	}
	return ret;
}

dllexport ZsonPackedStringPool* ZsonRuntimeStringPool::make_packed_pool() const {
	size_t count = (size_t)this->count;
	size_t packed_buffer_size = 0;
	nounroll novectorize for (size_t i = count; --i != SIZE_MAX;) {
		packed_buffer_size += this->views[i].length();
	}
	size_t offset_to_buffer = AlignUpToMultipleOf2(sizeof(ZsonStringPool) + sizeof(ZsonStringPoolEntry[count]), STRING_ALIGNMENT);
	size_t pool_size = offset_to_buffer + packed_buffer_size;
	// This uses offsetof to avoid needing to care about padding/alignment as much
	ZsonPackedStringPool* ret = (ZsonPackedStringPool*)malloc(offsetof(ZsonPackedStringPool, pool) + pool_size);
	if (expect(ret != NULL, true)) {
		ret->total_size = pool_size;
		ZsonStringPool* pool = &ret->pool;
		pool->count = count;
		pool->buffer_size = (size_t)this->buffer_size;
		char* buffer_write = based_pointer<char>(pool, offset_to_buffer);
		for (size_t i = 0; i < count; ++i) {
			const std::string_view& view = this->views[i];
			size_t string_length = view.length();
			pool->entries[i].length = string_length;
			size_t offset = uintptr_t(buffer_write) - offsetof(ZsonPackedStringPool, pool);
			__asm__ volatile (""::"r"(offset));
			pool->entries[i].offset = uint32_t(offset - uintptr_t(ret));
			buffer_write = rep_movsb(buffer_write, view.data(), string_length);
		}
	}
	return ret;
}

//dllexport ZsonPackedStringPool* ZsonRuntimeStringPool::make_packed_pool() const {
//	size_t count = (size_t)this->count;
//	size_t buffer_size = (size_t)this->buffer_size;
//	size_t offset_to_buffer = AlignUpToMultipleOf2(sizeof(ZsonStringPool) + sizeof(ZsonStringPoolEntry[count]), STRING_ALIGNMENT);
//	size_t packed_buffer_size = 0;
//	nounroll novectorize for (size_t i = 0; i < count; ++i) {
//		packed_buffer_size += this->views[i].length();
//	}
//	size_t pool_size = offset_to_buffer + packed_buffer_size;
//	// This uses offsetof to avoid needing to care about padding/alignment as much
//	ZsonPackedStringPool* ret = (ZsonPackedStringPool*)malloc(offsetof(ZsonPackedStringPool, pool) + pool_size);
//	if (expect(ret != NULL, true)) {
//		ret->total_size = pool_size;
//		ZsonStringPool* pool = &ret->pool;
//		pool->count = count;
//		pool->buffer_size = buffer_size;
//		char* buffer_write = based_pointer<char>(pool, offset_to_buffer);
//		for (size_t i = 0; i < count; ++i) {
//			const std::string_view& view = this->views[i];
//			size_t string_length = view.length();
//			pool->entries[i].length = string_length;
//			pool->entries[i].offset = (uint32_t)((uintptr_t)buffer_write - (uintptr_t)pool);
//			buffer_write = rep_movsb(buffer_write, view.data(), string_length);
//		}
//	}
//	return ret;
//}

struct ZsonFile {
	uint32_t file_size; // 0x0
	uint32_t string_pool_offset; // 0x4
	union {
		uint32_t file_meta; // 0x8
		struct {
			uint8_t header_size; // 0x8
			union {
				uint8_t flags; // 0x9
				struct {
					bool contains_inline_keys : 1;
				};
			};
			uint8_t version; // 0xA
		};
	};
	uint32_t top_level_value_count; // 0xC
	ZsonValue values[]; // 0x10
} gnu_packed;
ValidateFieldOffset(0x0, ZsonFile, file_size);
ValidateFieldOffset(0x4, ZsonFile, string_pool_offset);
ValidateFieldOffset(0x8, ZsonFile, file_meta);
ValidateFieldOffset(0x8, ZsonFile, header_size);
ValidateFieldOffset(0x9, ZsonFile, flags);
ValidateFieldOffset(0xA, ZsonFile, version);
ValidateFieldOffset(0xC, ZsonFile, top_level_value_count);
ValidateFieldOffset(0x10, ZsonFile, values);

#define ZSON_SIZE_INDEX(type, in_memory) (type << 1 | in_memory)
static uint32_t zson_key_size_lookup[] = {
	[ZSON_SIZE_INDEX(ZsonType::End, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::End, true)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::Double, false)] = sizeof(ZsonHeader) + sizeof(double),
	[ZSON_SIZE_INDEX(ZsonType::Double, true)] = sizeof(ZsonHeader) + sizeof(double),
	[ZSON_SIZE_INDEX(ZsonType::String, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::String, true)] = sizeof(ZsonHeader) + sizeof(void*),
	[ZSON_SIZE_INDEX(ZsonType::Object, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::Object, true)] = sizeof(ZsonHeader) + sizeof(void*),
	[ZSON_SIZE_INDEX(ZsonType::Array, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::Array, true)] = sizeof(ZsonHeader) + sizeof(void*),
	[ZSON_SIZE_INDEX(ZsonType::Blob, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::Blob, true)] = sizeof(ZsonHeader) + sizeof(void*),
	[ZSON_SIZE_INDEX(ZsonType::Undefined, false)] = sizeof(ZsonHeader),
	[ZSON_SIZE_INDEX(ZsonType::Undefined, true)] = sizeof(ZsonHeader),
	[ZSON_SIZE_INDEX(ZsonType::ObjectID, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::ObjectID, true)] = sizeof(ZsonHeader) + sizeof(void*),
	[ZSON_SIZE_INDEX(ZsonType::DBPointer, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::DBPointer, true)] = sizeof(ZsonHeader) + sizeof(void*),
	[ZSON_SIZE_INDEX(ZsonType::JS, false)] = 0,
	[ZSON_SIZE_INDEX(ZsonType::JS, true)] = sizeof(ZsonHeader) + sizeof(void*),

};

dllexport void* read_zson_file_to_memory2(const char* path) {
	if (FILE* file = fopen(path, "rb");
		expect(file != NULL, true)
	) {
		if (uint32_t file_size;
			expect(_fread_nolock(&file_size, sizeof(file_size), 1, file), true)
		) {
			file_size -= sizeof(file_size);
			if (size_t buffer_size = AlignUpToMultipleOf2(file_size, BUFFER_PADDING);
				expect(buffer_size != 0, true)
			) {
				if (uint8_t* buffer = malloc_array_aligned<uint8_t, STRING_ALIGNMENT>(buffer_size);
					expect(buffer != NULL, true)
				) {
					__builtin_memset(buffer + buffer_size - BUFFER_PADDING, 0, BUFFER_PADDING);
					if (expect(_fread_nolock(buffer + sizeof(uint64_t), file_size, 1, file), true)) {
						_fclose_nolock(file);

						return buffer;
					}
					_aligned_free(buffer);
				}
			}
		}
		_fclose_nolock(file);
	}
	return NULL;
}

dllexport void* read_zson_file_to_memory3(const char* path) {
	if (FILE* file = fopen(path, "rb");
		expect(file != NULL, true)
	) {
		if (uint32_t file_size;
			expect(_fread_nolock(&file_size, sizeof(file_size), 1, file), true)
		) {
			if (size_t buffer_size = AlignUpToMultipleOf2(file_size, 64);
				!(sizeof(size_t) > sizeof(uint32_t)) || expect(buffer_size != 0, true)
			) {
				if (uint8_t* buffer = malloc_array_aligned<uint8_t, 16>(buffer_size);
					expect(buffer != NULL, true)
				) {
					*(uint32_t*)buffer = file_size;
					__builtin_memset(buffer + buffer_size - 64, 0, 64);
					if (expect(_fread_nolock(buffer + sizeof(uint64_t), file_size - sizeof(uint32_t), 1, file), true)) {



						_fclose_nolock(file);
						return buffer;
					}
					_aligned_free(buffer);
				}
			}
		}
		_fclose_nolock(file);
	}
	return NULL;
}


dllexport void* get_zson_key(ZsonHeaderAligned* zson_values, const char* name) {
	ZsonHeader* zson_ptr = zson_values;
	ZsonHeaderMeta meta;

	while ((meta = zson_ptr->meta).type) {
		if (expect(!meta.dead, true)) {
		}
		uintptr_t offset = zson_key_size_lookup[meta.size_lookup];
		zson_values = based_pointer(zson_values, offset);
	}
}

#include <map>

//static std::set<
#include <string>

dllexport void* parse_key_string(const char *const restrict str) {
	std::string test;
	//std::hash<std::string_view>
}