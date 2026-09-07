#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <type_traits>
#include "Windows.h"

inline void wmemcpy_fast(void* dst, const void* src, size_t count) {
	memcpy(dst, src, count * 2);
}

#define LDR_IMAGE_DATAFILE_TO_VIEW(x) ((PVOID)(((ULONG_PTR)(x)) & ~(ULONG_PTR)3))

struct IMPORT_TABLE_ENTRY {
	uint32_t import_lookup_table_rva;
	uint32_t time_date_stamp;
	uint32_t forwarder_chain;
	uint32_t dll_name_rva;
	uint32_t import_address_table_rva;
};

template<size_t bits>
bool iterate_imports(uintptr_t module_base, PIMAGE_NT_HEADERS nt_header_generic, const char* dll_name, const char* func_name) {
	using NT = std::conditional_t<bits == 32, IMAGE_NT_HEADERS32, IMAGE_NT_HEADERS64>;
	using ENTRY = std::conditional_t<bits == 32, int32_t, int64_t>;

	NT* nt_header = (NT*)nt_header_generic;
	DWORD imports_offset = nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD imports_size = nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
	if (imports_offset && imports_size) {
		for (
			IMPORT_TABLE_ENTRY* import_entries = (IMPORT_TABLE_ENTRY*)(module_base + imports_offset);
			uint32_t dll_name_rva = import_entries->dll_name_rva;
			++import_entries
		) {
			const char* import_dll_name = (const char*)(module_base + (uint32_t)dll_name_rva);
			if (!stricmp(import_dll_name, dll_name)) {
				for (
					ENTRY* lookup_entries = (ENTRY*)(module_base + import_entries->import_lookup_table_rva);
					ENTRY entry = *lookup_entries; // final entry is 0
					++lookup_entries
				) {
					if (entry >= 0) { // check ordinal bit
						const char* import_name = (const char*)(module_base + (uint32_t)entry + 2); // +2 to ignore the export table hint
						if (!strcmp(import_name, func_name)) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void scan_imports(const wchar_t* filename, uint64_t filesize, const char* dll_name, const char* func_name) {
	if (HMODULE handle = LoadLibraryExW(filename, NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_IMAGE_RESOURCE | LOAD_LIBRARY_AS_DATAFILE)) {
		//wprintf(L"Scanning %ls...\n", filename);
		PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)LDR_IMAGE_DATAFILE_TO_VIEW(handle);
		if (dos_header->e_magic == IMAGE_DOS_SIGNATURE) {
			PIMAGE_NT_HEADERS nt_header_generic = (PIMAGE_NT_HEADERS)((uintptr_t)dos_header + dos_header->e_lfanew);
			switch (nt_header_generic->FileHeader.Machine) {
				default: goto not_found;
				case IMAGE_FILE_MACHINE_I386:
					if (!iterate_imports<32>((uintptr_t)dos_header, nt_header_generic, dll_name, func_name)) {
						goto not_found;
					}
					break;
				case IMAGE_FILE_MACHINE_AMD64:
					if (!iterate_imports<64>((uintptr_t)dos_header, nt_header_generic, dll_name, func_name)) {
						goto not_found;
					}
					break;
			}
			wprintf(
				L"IMPORT FOUND IN: %ls\n"
				, filename
			);
		}
	not_found:
		FreeLibrary(handle);
	}
}

void scan_directory(const wchar_t* directory, const wchar_t* extension, const char* dll_name, const char* func_name) {

	size_t directory_len = wcslen(directory);
	if (wchar_t* search = (wchar_t*)malloc(sizeof(wchar_t) * (directory_len + 3))) {
		wmemcpy_fast(search, directory, directory_len);
		search[directory_len] = L'\\';
		search[directory_len + 1] = L'*';
		search[directory_len + 2] = L'\0';
		//wprintf(L"Search: %ls\n", search);

		directory_len += 1; // save the \ as part of length

		WIN32_FIND_DATAW find_data;
		HANDLE find = FindFirstFileW(search, &find_data);
		if (find != INVALID_HANDLE_VALUE) {
			do {
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (find_data.cFileName[0] != L'.') {
						size_t folder_name_len = wcslen(find_data.cFileName) + 1;
						if (wchar_t* new_search = (wchar_t*)malloc(sizeof(wchar_t) * (directory_len + folder_name_len))) {
							wmemcpy_fast(new_search, search, directory_len);
							wmemcpy_fast(new_search + directory_len, find_data.cFileName, folder_name_len);
							scan_directory(new_search, extension, dll_name, func_name);
							free(new_search);
						}
					}
				} else if (uint64_t file_size = find_data.nFileSizeLow | (uint64_t)find_data.nFileSizeHigh << 32) {
					wchar_t* find_extension = wcsrchr(find_data.cFileName, L'.');
					if (find_extension && !wcsicmp(find_extension + 1, extension)) {
						size_t file_name_len = wcslen(find_data.cFileName) + 1;
						if (wchar_t* filename = (wchar_t*)malloc(sizeof(wchar_t) * (directory_len + file_name_len))) {
							wmemcpy_fast(filename, search, directory_len);
							wmemcpy_fast(filename + directory_len, find_data.cFileName, file_name_len);
							scan_imports(filename, file_size, dll_name, func_name);
							free(filename);
						}
					}
				}
			} while (FindNextFileW(find, &find_data));
			FindClose(find);
		}
		free(search);
	}
}

int main(int argc, char* argv[]) {

	size_t arg_index = 0;

	auto next_arg = [&]() -> const char* {
		if (arg_index < argc) {
			return argv[arg_index++];
		}
		return NULL;
	};

	const char* dll_name = NULL;
	const char* func_name = NULL;

	while (const char* arg = next_arg()) {
		if (!strcmp(arg, "-d")) {
			const char* arg = next_arg();
			if (!arg) break;
			dll_name = arg;
		}
		else if (!strcmp(arg, "-f")) {
			const char* arg = next_arg();
			if (!arg) break;
			func_name = arg;
		}
	}
	if (
		!dll_name ||
		!func_name
	) {
		printf("Invalid args\n");
		return -1;
	}

	printf(
		"Searching for %s imported from %s\n"
		, func_name
		, dll_name
	);

	scan_directory(L".", L"exe", dll_name, func_name);
	scan_directory(L".", L"dll", dll_name, func_name);

	return 0;
}