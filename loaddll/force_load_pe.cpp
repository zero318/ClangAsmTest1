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

static inline constexpr bool skip_to_char(char*& str, char end) {
	for (;;) {
		char c = *str;
		if (expect(!c, false)) {
			return false;
		}
		if (c == end) {
			return true;
		}
		++str;
	}
}

int mainCRTStartup(void);

BOOL APIENTRY DllMain(HMODULE hDll, DWORD ulReasonForCall, LPVOID);

using ExeEntry = decltype(mainCRTStartup);
using DllEntry = decltype(DllMain);

_Noreturn void yeet() {
	char* command_line = GetCommandLineA();

	if (*command_line == '"') {
		++command_line;
		if (expect(!skip_to_char(command_line, '"'), false)) {
			ExitProcess(1);
			unreachable;
		}
		++command_line;
	}

	if (expect(!skip_to_char(command_line, ' '), false)) {
		ExitProcess(1);
		unreachable;
	}
	++command_line;

	char end_char = ' ';
	if (*command_line == '"') {
		++command_line;
		end_char = '"';
	}
	char* dll_name = command_line;

	if (skip_to_char(command_line, end_char)) {
		*command_line = '\0';
	}


	HMODULE dll_handle = LoadLibraryExA(dll_name, NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
	if (expect(dll_handle == NULL, false)) {
		ExitProcess(1);
		unreachable;
	}

	void* dll_base = (void*)((uintptr_t)dll_handle & ~3);

	IMAGE_DOS_HEADER* dll_header = (IMAGE_DOS_HEADER*)((uintptr_t)dll_base & ~3);

	// 

	IMAGE_NT_HEADERS* nt_header = based_pointer<IMAGE_NT_HEADERS>(dll_base, dll_header->e_lfanew);

	DWORD entry_offset = nt_header->OptionalHeader.AddressOfEntryPoint;
	if (expect(entry_offset != 0, true)) {
		void* entry_func = based_pointer<void*>(dll_base, entry_offset);

#if IS_X64
		register LPVOID lpvReserved asm("r8") = NULL;
#endif

		__asm__ volatile (

			INTEL_SYNTAX_DIRECTIVE
			"PUSHF \n"
#if IS_X64
			"POP RAX \n"
			"OR RAX, 0x100 \n"
			"PUSH RAX \n"
#else
			"POP EAX \n"
			"OR EAX, 0x100 \n"
			"PUSH EAX \n"
#endif
			"POPF \n"
			ATT_SYNTAX_DIRECTIVE
			"call %A0\n"
			:
#if IS_X64
			: "r"(entry_func)
			: clobber_list("rax")
#else
			: "r"(entry_func), "c"(dll_handle), "d"(DLL_PROCESS_ATTACH), "r"(lpvReserved)
			: clobber_list("eax")
#endif
		);
	} else {
		void* code = based_pointer<void>(dll_base, nt_header->OptionalHeader.BaseOfCode);

		__asm__ volatile (
			INTEL_SYNTAX_DIRECTIVE
			"PUSHF \n"
#if IS_X64
			"POP RAX \n"
			"OR RAX, 0x100 \n"
			"PUSH RAX \n"
#else
			"POP EAX \n"
			"OR EAX, 0x100 \n"
			"PUSH EAX \n"
#endif
			"POPF \n"
			ATT_SYNTAX_DIRECTIVE
			"jmp %A0 \n"
			:
			: "r"(code)
#if IS_X64
			: clobber_list("rax")
#else
			: clobber_list("eax")
#endif
		);
		unreachable;
	}

	ExitProcess(0);
	unreachable;
}