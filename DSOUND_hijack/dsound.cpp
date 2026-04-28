#include <windows.h>

#pragma comment(linker, "/export:DllMain=C:\\\\Windows\\\\System32\\\\DSOUND.dll.#11,@11")

extern "C" {

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	return TRUE;
}

}