#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

#include "dbghelp.h"
#include "windows.h"

#pragma comment(lib, "Dbghelp.lib")

#ifndef STATUS_INVALID_LOCK_SEQUENCE
#define STATUS_INVALID_LOCK_SEQUENCE		((DWORD)0xC000001EL)
#endif
#ifndef STATUS_HEAP_CORRUPTION
#define STATUS_HEAP_CORRUPTION				((DWORD)0xC0000374L)
#endif
#ifndef STATUS_DATATYPE_MISALIGNMENT_ERROR
#define STATUS_DATATYPE_MISALIGNMENT_ERROR	((DWORD)0xC00002C5L)
#endif

#if _M_X64
#define ADDR_WIDTH_FORMAT L"16"
#else
#define ADDR_WIDTH_FORMAT L"8"
#endif
static const wchar_t *const FORMATS[] = {
	// No symbol, no module, not segfault
	L"Exception %08X at 0x%p\n",
	// Symbol, no module, not segfault
	L"Exception %08X at 0x%p\n"
	L"FuncName: %s\n"
	L"FuncAddr: 0x%0" ADDR_WIDTH_FORMAT L"zX + 0x%zX\n",
	// No symbol, module, not segfault
	L"Exception %1$08X at 0x%2$p \n"
	L"Module: %7$s (Rx%6$zX)\n",
	// Symbol, module, not segfault
	L"Exception %1$08X at 0x%2$p\n"
	L"Module: %7$s (Rx%6$zX)\n"
	L"FuncName: %3$s\n"
	L"FuncAddr: 0x%4$0" ADDR_WIDTH_FORMAT L"zX + 0x%5$zX\n",
	// No symbol, no module, segfault
	L"Exception %1$08X at 0x%2$p\n"
	L"The instruction referenced memory at 0x%8$0" ADDR_WIDTH_FORMAT "zX. The memory could not be %9$s.\n",
	// Symbol, no module, segfault
	L"Exception %1$08X at 0x%2$p\n"
	L"FuncName: %3$s\n"
	L"FuncAddr: 0x%4$0" ADDR_WIDTH_FORMAT L"zX + 0x%5$zX\n"
	L"The instruction referenced memory at 0x%8$0" ADDR_WIDTH_FORMAT "zX. The memory could not be %9$s.\n",
	// No symbol, module, segfault
	L"Exception %1$08X at 0x%2$p\n"
	L"Module: %7$s (Rx%6$zX)\n"
	L"The instruction referenced memory at 0x%8$0" ADDR_WIDTH_FORMAT "zX. The memory could not be %9$s.\n",
	// Symbol, module, segfault
	L"Exception %1$08X at 0x%2$p\n"
	L"Module: %7$s (Rx%6$zX)\n"
	L"FuncName: %3$s\n"
	L"FuncAddr: 0x%4$0" ADDR_WIDTH_FORMAT L"zX + 0x%5$zX\n"
	L"The instruction referenced memory at 0x%8$0" ADDR_WIDTH_FORMAT "zX. The memory could not be %9$s.\n"
};

static const wchar_t *const ACCESS_TYPES[] = {
	L"read", L"written", NULL, L"executed"
};

void exception_mbox(const wchar_t* format, ...) {
	va_list va, va2;
	va_start(va, format);
	va_copy(va2, va);
	int length = _vscwprintf_p(format, va) + 1;
	va_end(va);
	wchar_t* buffer = (wchar_t*)malloc(length * sizeof(wchar_t));
	if (buffer) {
		_vswprintf_p(buffer, length, format, va2);
		_putws(buffer);
		MessageBoxW(NULL, buffer, L":NotLikeAlice:", MB_ICONERROR | MB_SYSTEMMODAL);
		free(buffer);
	}
	va_end(va2);
}

LONG WINAPI exception_filter(LPEXCEPTION_POINTERS lpEI) {

#define HAVE_SYMBOL 1
#define HAVE_MODULE 2
#define IS_SEGFAULT 4

	LPEXCEPTION_RECORD lpER = lpEI->ExceptionRecord;

	unsigned char print_type;

	switch ((DWORD)lpER->ExceptionCode) {
		case (DWORD)EXCEPTION_SINGLE_STEP:
		case (DWORD)EXCEPTION_BREAKPOINT:
			if (IsDebuggerPresent()) {
		default:
				return EXCEPTION_CONTINUE_SEARCH;
			}
		case (DWORD)STATUS_INTEGER_DIVIDE_BY_ZERO:
		case (DWORD)STATUS_ASSERTION_FAILURE:
		case (DWORD)STATUS_STACK_OVERFLOW:
		case (DWORD)STATUS_STACK_BUFFER_OVERRUN:
		case (DWORD)STATUS_BAD_STACK:
		case (DWORD)STATUS_HEAP_CORRUPTION:

		// These are only here to catch
		// jumps/calls to garbage data
		case (DWORD)STATUS_ILLEGAL_INSTRUCTION:
		case (DWORD)STATUS_PRIVILEGED_INSTRUCTION:
		case (DWORD)STATUS_INVALID_LOCK_SEQUENCE:
		case (DWORD)STATUS_ARRAY_BOUNDS_EXCEEDED:
		case (DWORD)STATUS_INTEGER_OVERFLOW:
		case (DWORD)STATUS_DATATYPE_MISALIGNMENT_ERROR:
		case (DWORD)STATUS_FLOAT_DENORMAL_OPERAND:
		case (DWORD)STATUS_FLOAT_DIVIDE_BY_ZERO:
		case (DWORD)STATUS_FLOAT_INEXACT_RESULT:
		case (DWORD)STATUS_FLOAT_INVALID_OPERATION:
		case (DWORD)STATUS_FLOAT_OVERFLOW:
		case (DWORD)STATUS_FLOAT_STACK_CHECK:
		case (DWORD)STATUS_FLOAT_UNDERFLOW:
		case (DWORD)STATUS_FLOAT_MULTIPLE_FAULTS:
		case (DWORD)STATUS_FLOAT_MULTIPLE_TRAPS:
			print_type = 0;
			break;

		case (DWORD)STATUS_ACCESS_VIOLATION:
		case (DWORD)EXCEPTION_IN_PAGE_ERROR:
			print_type = IS_SEGFAULT;
	}

	unsigned char symbol_info[sizeof(SYMBOL_INFOW) + sizeof(WCHAR[MAX_SYM_NAME])];
	DWORD64 offset;
	size_t module_addr = 0;

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	if (process) {
		if (SymInitializeW(process, NULL, TRUE)) {
			((SYMBOL_INFOW*)symbol_info)->SizeOfStruct = sizeof(SYMBOL_INFOW);
			((SYMBOL_INFOW*)symbol_info)->MaxNameLen = MAX_SYM_NAME;
			if (SymFromAddrW(process, (DWORD64)lpER->ExceptionAddress, &offset, (SYMBOL_INFOW*)symbol_info)) {
				print_type |= HAVE_SYMBOL;
				module_addr = (size_t)((SYMBOL_INFOW*)symbol_info)->ModBase;
			}
			SymCleanup(process);
		}
		CloseHandle(process);
	}

	if (!module_addr) {
		GetModuleHandleExW(
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
			(LPCWSTR)lpER->ExceptionAddress,
			(HMODULE*)&module_addr
		);
	}

	WCHAR module_path[MAX_PATH];
	wchar_t* module_name;
	if (
		module_addr &&
		GetModuleFileNameW((HMODULE)module_addr, module_path, MAX_PATH) != MAX_PATH &&
		((module_name = wcsrchr(module_path, L'\\')) || (module_name = wcsrchr(module_path, L'/')))
	) {
		++module_name;
		print_type |= HAVE_MODULE;
		wchar_t* extension = wcschr(module_name, L'.');
		if (extension) {
			*extension = L'\0';
		}
	}

	exception_mbox(
		FORMATS[print_type]
		, (unsigned int)lpER->ExceptionCode // 1
		, lpER->ExceptionAddress // 2
		, (size_t)((SYMBOL_INFOW*)symbol_info)->Name // 3
		, (size_t)((SYMBOL_INFOW*)symbol_info)->Address // 4
		, (size_t)offset // 5
		, (size_t)lpER->ExceptionAddress - module_addr // 6
		, module_name // 7
		, (size_t)lpER->ExceptionInformation[1] // 8
		, ACCESS_TYPES[lpER->ExceptionInformation[0] & 3] // 9
	);

	return EXCEPTION_CONTINUE_SEARCH;
}

int add_exception_filter() {
	AddVectoredExceptionHandler(0, exception_filter);
	return 0;
}
int exception_dummy = add_exception_filter();
