#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>

#include "../zero/util.h"

#include "../windows_structs.h"

BOOL APIENTRY DllMain(HMODULE hDll, DWORD ulReasonForCall, LPVOID);

using DllEntry = decltype(DllMain);

_Noreturn sysv_abi void yeet() {

	wchar_t* command_line = GetCommandLineW();
	
	wchar_t c;
	
	if (*command_line == L'"') {
		++command_line;
		do {
			c = *command_line++;
			if (expect(!c, false)) {
				ExitProcess(1);
				unreachable;
			}
		} while (c != L'"');
	}
	
	do {
		c = *command_line++;
		if (expect(!c, false)) {
			ExitProcess(1);
			unreachable;
		}
	} while (c != L' ');

	wchar_t* dll_name = command_line;
	if (*command_line++ == L'"') {
		++dll_name;
		
		do {
			c = *command_line;
			if (c == L'"') {
				*command_line = L'\0';
				break;
			}
			++command_line;
		} while (c != L'\0');
	}

	HMODULE dll_handle = LoadLibraryExW(dll_name, NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
	if (expect(dll_handle == NULL, false)) {
		ExitProcess(1);
		unreachable;
	}
	void* dll_base = (void*)((uintptr_t)dll_handle & ~3);

	IMAGE_DOS_HEADER* dll_header = (IMAGE_DOS_HEADER*)dll_base;


	IMAGE_NT_HEADERS* nt_header = based_pointer<IMAGE_NT_HEADERS>(dll_base, dll_header->e_lfanew);

	DWORD entry_offset = nt_header->OptionalHeader.AddressOfEntryPoint;
	DWORD code_offset = nt_header->OptionalHeader.BaseOfCode;

	if (nt_header->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) {

		auto& user_shared_data = USER_SHARED_DATAR();

		if (
			(nt_header->OptionalHeader.MajorOperatingSystemVersion < user_shared_data.NtMajorVersion || (
				nt_header->OptionalHeader.MajorOperatingSystemVersion == user_shared_data.NtMajorVersion &&
				nt_header->OptionalHeader.MinorOperatingSystemVersion <= user_shared_data.NtMinorVersion
			)) &&
			(nt_header->OptionalHeader.MajorImageVersion < user_shared_data.NtMajorVersion || (
				nt_header->OptionalHeader.MajorImageVersion == user_shared_data.NtMajorVersion &&
				nt_header->OptionalHeader.MinorImageVersion <= user_shared_data.NtMinorVersion
			)) &&
			(nt_header->OptionalHeader.MajorSubsystemVersion < user_shared_data.NtMajorVersion || (
				nt_header->OptionalHeader.MajorSubsystemVersion == user_shared_data.NtMajorVersion &&
				nt_header->OptionalHeader.MinorSubsystemVersion <= user_shared_data.NtMinorVersion
			))
		) {
			FreeLibrary(dll_handle);
			dll_handle = LoadLibraryW(dll_name);
			if (expect(dll_handle == NULL, false)) {
				ExitProcess(1);
				unreachable;
			}

			dll_base = (void*)dll_handle;
		}
	}

	if (expect(entry_offset != 0, true)) {
		DllEntry* entry_func = based_pointer<DllEntry>(dll_base, entry_offset);
		
		register LPVOID lpvReserved asm("r8") = NULL;

		__asm__ volatile (
			INTEL_SYNTAX_DIRECTIVE
			"PUSHF \n"
			"POP RAX \n"
			"OR RAX, 0x100 \n"
			"PUSH RAX \n"
			"POPF \n"
			ATT_SYNTAX_DIRECTIVE
			"call %A0\n"
			:
			: "r"(entry_func), "c"(dll_base), "d"(DLL_PROCESS_ATTACH), "r"(lpvReserved)
			: clobber_list("rax")
		);
	}
	else {

		void* code = based_pointer<void>(dll_base, code_offset);

		__asm__ volatile (
			INTEL_SYNTAX_DIRECTIVE
			"PUSHF \n"
			"POP RAX \n"
			"OR RAX, 0x100 \n"
			"PUSH RAX \n"
			"POPF \n"
			ATT_SYNTAX_DIRECTIVE
			"jmp %A0 \n"
			:
			: "r"(code)
			: clobber_list("rax")
		);
		unreachable;
	}

	ExitProcess(0);
	unreachable;
}