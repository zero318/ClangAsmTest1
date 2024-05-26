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

#include <atomic>

#include "../zero/util.h"

#include "../windows_structs.h"

#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")

#define PLACE_IN_SECTION(name, size, ...) \
__declspec(allocate(MACRO_STR(name))) __attribute__((used, retain)) volatile const unsigned char MACRO_CAT(name,_section)[size]

#define PLACE_IN_SECTION_WITH_DOT(name, size, ...) \
__declspec(allocate(MACRO_STR(.name))) __attribute__((used,retain)) volatile const unsigned char MACRO_CAT(name,_section)[size]

#define MAKE_SECTION(name, size, ...) \
/*__pragma(section(MACRO_STR(name) __VA_OPT__(,) __VA_ARGS__))*/ PLACE_IN_SECTION(name, size)
//__declspec(allocate(MACRO_STR(name))) __attribute__((used,retain)) volatile const char MACRO_CAT(name,_section)[size]

#define MAKE_SECTION_WITH_DOT(name, size, ...) \
/*__pragma(section(MACRO_STR(.name) __VA_OPT__(,) __VA_ARGS__))*/ PLACE_IN_SECTION_WITH_DOT(name, size)


#pragma section("yeet",read,write,shared)

__declspec(allocate("yeet")) __attribute__((used, retain))
std::atomic<HANDLE> waiting_on_pid;
__declspec(allocate("yeet")) __attribute__((used, retain))
std::atomic<void*> address;

extern "C" {

dllexport void config(HANDLE id) {
	HANDLE dummy;
	do dummy = NULL;
	while (!waiting_on_pid.compare_exchange_strong(dummy, id));
	address = NULL;
}
dllexport void* wait() {
	void* inject_address = NULL;
	while (address.compare_exchange_strong(inject_address, NULL));
	waiting_on_pid = NULL;
	return inject_address;
}

__attribute__((used, retain)) DWORD WINAPI injector_init(void* module_base) {
	MessageBoxA(NULL, "yo what up it's ya boi injected DLL", "why did I write this", 0);
	return 0;
}

}

void write_address(HANDLE self_id) {
	while (waiting_on_pid != self_id);
	address = &injector_init;
}

extern "C" {

dllexport BOOL APIENTRY _DllMainCRTStartup(HMODULE hDll, DWORD ulReasonForCall, LPVOID lpvReserved) {
	switch (ulReasonForCall) {
		case DLL_PROCESS_ATTACH:
			if (lpvReserved == NULL) {
				write_address(teb64->ClientId.UniqueProcess);
			}
	}
	return TRUE;
}

}