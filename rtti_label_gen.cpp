
#include <stdlib.h>
#include <stdint.h>

#include <Windows.h>

#include "zero/util.h"

#define _ThrowInfo ThrowInfo
#include <rttidata.h>

#include <typeinfo>


template<typename T, size_t bits = native_bits, typename U = std::type_info>
struct RttiTypeDescriptorX;

template<typename T, size_t bits = native_bits>
struct RttiClassHierarchyDescriptorX;

template<typename T, size_t bits = native_bits>
struct RttiBaseClassDescriptorX {
	PTRZX<32, RttiClassHierarchyDescriptorX<T, bits>> pTypeDescriptor; // 0x0, 0x0
	uint32_t numContainedBases; // 0x4, 0x4
	PMD where; // 0x8, 0x8
	union {
		uint32_t attributes; // 0xC, 0xC
		struct {
			uint32_t not_visible : 1;
			uint32_t ambiguous : 1;
			uint32_t is_private_inheritance : 1;
			uint32_t is_virtual_inheritance : 1;
			uint32_t not_polymorphic : 1;
			uint32_t has_class_hierarchy_descriptor : 1;
		};
	};
	PTRZX<32, const RttiClassHierarchyDescriptorX<T, bits>> pClassDescriptor[]; // 0x10, 0x10
};
//ValidateStructSize(0x10, (RttiBaseClassDescriptorX<void, 32>));
//ValidateStructAlignment(0x4, (RttiBaseClassDescriptorX<void, 32>));
//ValidateStructSize(0x10, (RttiBaseClassDescriptorX<void, 64>));
//ValidateStructAlignment(0x4, (RttiBaseClassDescriptorX<void, 64>));

template<typename T, size_t bits>
struct RttiBaseClassArrayX {
	PTRZX<32, const RttiBaseClassDescriptorX<T, bits>> main_descriptor; // 0x0, 0x0
	PTRZX<32, const void> arrayOfBaseClassDescriptors[]; // 0x4, 0x4

	template<typename U>
	inline PTRZX<32, const RttiBaseClassDescriptorX<U, bits>>& operator[](size_t index) {
		return ((PTRZX<32, const RttiBaseClassDescriptorX<U, bits>>*)&this->main_descriptor)[index];
	}
};
//ValidateStructSize(0x4, (RttiBaseClassArrayX<void, 32>));
//ValidateStructAlignment(0x4, (RttiBaseClassArrayX<void, 32>));
//ValidateStructSize(0x4, (RttiBaseClassArrayX<void, 64>));
//ValidateStructAlignment(0x4, (RttiBaseClassArrayX<void, 64>));

template<typename T, size_t bits>
struct RttiClassHierarchyDescriptorX {
	uint32_t signature; // 0x0, 0x0
	union {
		uint32_t attributes; // 0x4, 0x4
		struct {
			uint32_t has_multiple_inheritance : 1;
			uint32_t has_virtual_inheritance : 1;
			uint32_t has_ambiguous_base : 1;
		};
	};
	uint32_t numBaseClasses; // 0x8, 0x8
	PTRZX<32, const RttiBaseClassArrayX<T, bits>> pBaseClassArray; // 0xC, 0xC
	// 0x10, 0x10
};
//ValidateStructSize(0x10, (RttiClassHierarchyDescriptorX<void, 32>));
//ValidateStructAlignment(0x4, (RttiClassHierarchyDescriptorX<void, 32>));
//ValidateStructSize(0x10, (RttiClassHierarchyDescriptorX<void, 64>));
//ValidateStructAlignment(0x4, (RttiClassHierarchyDescriptorX<void, 64>));

template<typename T, size_t bits = native_bits>
struct RttiCompleteObjectLocatorX;

template<typename T>
struct RttiCompleteObjectLocatorX<T, 32> {
	uint32_t signature; // 0x0
	uint32_t offset; // 0x4
	uint32_t cdOffset; // 0x8
	PTRZX<32, RttiTypeDescriptorX<T, 32>> pTypeDescriptor; // 0xC
	PTRZX<32, const RttiClassHierarchyDescriptorX<T, 32>> pClassDescriptor; // 0x10
	// 0x14
};

template<typename T>
struct RttiCompleteObjectLocatorX<T, 64> {
	uint32_t signature; // 0x0
	uint32_t offset; // 0x4
	uint32_t cdOffset; // 0x8
	PTRZX<32, RttiTypeDescriptorX<T, 64>> pTypeDescriptor; // 0xC
	PTRZX<32, const RttiClassHierarchyDescriptorX<T, 64>> pClassDescriptor; // 0x10
	PTRZX<32, const RttiCompleteObjectLocatorX<T, 64>> pSelf; // 0x14
	// 0x18
};
//ValidateStructSize(0x14, (RttiCompleteObjectLocatorX<void, 32>));
//ValidateStructAlignment(0x4, (RttiCompleteObjectLocatorX<void, 32>));
//ValidateStructSize(0x18, (RttiCompleteObjectLocatorX<void, 64>));
//ValidateStructAlignment(0x4, (RttiCompleteObjectLocatorX<void, 64>));

template<typename T, size_t bits = native_bits>
struct VFTableX {
	PTRZX<bits, void()> funcs[]; // 0x0

	PTRZX<bits, const RttiCompleteObjectLocatorX<T, bits>> rtti() {
		return (PTRZX<bits, const RttiCompleteObjectLocatorX<T, bits>>)this->funcs[-1];
	}
};
//ValidateStructAlignment(0x4, (VFTableX<void, 32>));
//ValidateStructAlignment(0x8, (VFTableX<void, 64>));


template<typename T, size_t bits, typename U>
struct RttiTypeDescriptorX {
	PTRZX<bits, VFTableX<U, bits>> pVFTable; // 0x0, 0x0
	PTRZX<bits, char> undecorated_name; // 0x4, 0x8
	char name[]; // 0x8, 0x10
};
//ValidateStructSize(0x8, (RttiTypeDescriptorX<void, 32>));
//ValidateStructAlignment(0x4, (RttiTypeDescriptorX<void, 32>));
//ValidateStructSize(0x10, (RttiTypeDescriptorX<void, 64>));
//ValidateStructAlignment(0x8, (RttiTypeDescriptorX<void, 64>));

#include <vector>

dllexport bool parse_vtable_inputs(std::vector<uint32_t>& values, const char* input_path, size_t base) {
	if (FILE* input_file = fopen(input_path, "rb")) {
		fseek(input_file, 0, SEEK_END);
		size_t file_size = ftell(input_file);
		if (char* file = (char*)malloc(file_size+1)) {
			file[file_size] = '\0';
			rewind(input_file);
			fread(file, file_size, 1, input_file);
			const char* file_read = file;
			for (;;) {
				auto [number, end_str] = decltype(strtonum<uint32_t, 16, true, false, false>(file_read, base))();
				values.emplace_back(number);
				if (file_read != end_str) {
					file_read = end_str;
					if (*file_read++ == ',') {
						continue;
					}
					free(file);
					fclose(input_file);
					return true;
				}
				break;
			}
			free(file);
		}
		fclose(input_file);
	}
	return false;
}

dllexport gnu_noinline void generate_rtti_labels32(const wchar_t* path, const char* output_path, const char* input_path) {
	if (FILE* output_file = fopen(output_path, "w")) {
		output_file = freopen(output_path, "a", output_file);
		if (HMODULE exe_handle = LoadLibraryExW(path, NULL, LOAD_LIBRARY_AS_DATAFILE)) {
			void* exe_base = based_pointer<void>(exe_handle, -1);
			std::vector<uint32_t> values;
			if (parse_vtable_inputs(values, input_path, (size_t)exe_base)) {			
				for (auto value : values) {
					fprintf(output_file, "0x%X,", value);
				}
			}
			FreeLibrary(exe_handle);
		}
		fclose(output_file);
	}
}

dllexport gnu_noinline void generate_rtti_labels64() {

}