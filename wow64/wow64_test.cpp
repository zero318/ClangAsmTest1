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

/*
BOOL WINAPI CreateProcessZWInner(
    HANDLE hUserToken,
    LPCWSTR lpApplicationName,
    LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation,
    PHANDLE hRestrictedUserToken
) {

}

BOOL WINAPI CreateProcessZW(
    LPCWSTR lpApplicationName,
    LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
) {
    return CreateProcessZWInner(
        NULL, // Create new process with the token on the creator process
        lpApplicationName,
        lpCommandLine,
        lpProcessAttributes,
        lpThreadAttributes,
        bInheritHandles,
        dwCreationFlags,
        lpEnvironment,
        lpCurrentDirectory,
        lpStartupInfo,
        lpProcessInformation,
        NULL  // Do not return the restricted token
    );
}
*/


enum Wow64TLSSlot {
    WOW64_TLS_STACKPTR64,
    WOW64_TLS_CPURESERVED,
    WOW64_TLS_INCPUSIMULATION,
    WOW64_TLS_TEMPLIST,
    WOW64_TLS_EXCEPTIONADDR,
    WOW64_TLS_USERCALLBACKDATA,
    WOW64_TLS_EXTENDED_FLOAT,
    WOW64_TLS_APCLIST,
    WOW64_TLS_FILESYSREDIR,
    WOW64_TLS_LASTWOWCALL,
    WOW64_TLS_WOW64INFO,
    WOW64_TLS_INITIAL_TEB32
};

void Wow64ApcRoutine(uintptr_t ApcContext, uintptr_t Arg2, uintptr_t Arg3);

void ThunkContext32TO64(CONTEXTX<32>* Context32, CONTEXTX<64>* Context64, uintptr_t StackBase) {

}


NTSTATUS Wow64pThunkProcessParameters(PEBX<32>* Peb32, PEBX<64>* Peb) {
    _AddressOfReturnAddress()
}

NTSTATUS ProcessInit(size_t* pCpuThreadSize) {

}