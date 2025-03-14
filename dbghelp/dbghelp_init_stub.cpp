#include <Windows.h>

extern "C" {

BOOL __stdcall SymInitializeW(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess) {
	return TRUE;
}

}