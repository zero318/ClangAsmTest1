
#include "Windows.h"

#define PY_SSIZE_T_CLEAN
#include "C:\Users\zero318\AppData\Local\Programs\Python\Python37-32\include\Python.h"

static bool python_is_initialized = false;

extern "C" {

__declspec(dllexport) int __fastcall run_string(const char* command) asm("run_string");
__declspec(dllexport) int __fastcall run_string(const char* command) {
	if (__builtin_expect(!python_is_initialized, false)) {
		python_is_initialized = true;
		Py_Initialize();
	}
	return PyRun_SimpleString(command);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	return TRUE;
}

}