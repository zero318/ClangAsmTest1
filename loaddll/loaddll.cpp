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

static LARGE_INTEGER qpc_frequency;

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

BOOL APIENTRY DllMain(HMODULE hDll, DWORD ulReasonForCall, LPVOID);

using DllEntry = decltype(DllMain);

_Noreturn void yeet() {

	//HMODULE self_handle = (HMODULE)teb->ProcessEnvironmentBlock.ImageBaseAddress;
	
	//QueryPerformanceFrequency(&qpc_frequency);
	
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

	HMODULE dll_handle = LoadLibraryA(dll_name);
	if (expect(dll_handle == NULL, false)) {
		ExitProcess(1);
		unreachable;
	}

	//__asm int3
	IMAGE_DOS_HEADER* dll_header = (IMAGE_DOS_HEADER*)dll_handle;

	// 

	IMAGE_NT_HEADERS* nt_header = based_pointer<IMAGE_NT_HEADERS>(dll_handle, dll_header->e_lfanew);

	DWORD entry_offset = nt_header->OptionalHeader.AddressOfEntryPoint;
	if (expect(entry_offset != 0, true)) {
		DllEntry* entry_func = based_pointer<DllEntry>(dll_handle, entry_offset);
		
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
			: "r"(entry_func), "c"(dll_handle), "d"(DLL_PROCESS_ATTACH), "r"(lpvReserved)
			: clobber_list("rax")
		);
	}
	else {

		void* code = based_pointer<void>(dll_handle, nt_header->OptionalHeader.BaseOfCode);

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