#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <atomic>

#include <Windows.h>
#include <ktmw32.h>

#include <winternl.h>

#include "zero/util.h"

#include "windows_structs.h"

using CreateFileA_t = decltype(CreateFileA);

#pragma comment (lib, "KtmW32.lib")
#pragma comment (lib, "F:/Users/zero318/Source/Repos/ClangAsmTest1/ntdll32.lib")

#define RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED 0x00000001
#define RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES 0x00000002
#define RTL_CLONE_PROCESS_FLAGS_NO_SYNCHRONIZE 0x00000004

extern "C" {
NTSYSAPI NTSTATUS NTAPI RtlCloneUserProcess(
    _In_ ULONG ProcessFlags,
    _In_opt_ PSECURITY_DESCRIPTOR ProcessSecurityDescriptor,
    _In_opt_ PSECURITY_DESCRIPTOR ThreadSecurityDescriptor,
    _In_opt_ HANDLE DebugPort,
    _Out_ PRTL_USER_PROCESS_INFORMATION ProcessInformation
) asm("_RtlCloneUserProcess");
NTSYSAPI NTSTATUS NTAPI NtTerminateProcess(
    HANDLE   ProcessHandle,
    NTSTATUS ExitStatus
) asm("_NtTerminateProcess");
NTSYSAPI NTSTATUS NTAPI NtWaitForSingleObject(
    HANDLE         Handle,
    BOOLEAN        Alertable,
    PLARGE_INTEGER Timeout
) asm("_NtWaitForSingleObject");
NTSYSAPI NTSTATUS NTAPI NtClose(
    HANDLE Handle
) asm("_NtClose");
}

static constexpr HANDLE current_process = NULL; // This is the same as NtCurrentProcess()

#define STATUS_PROCESS_CLONED ((DWORD)0x00000129)

char PRINT_BUFFER[64] = "Test failed\n";

#pragma section("yeet",read,write,shared)
#pragma comment(linker, "/SECTION:yeet,RWS")

__declspec(allocate("yeet")) __attribute__((used, retain))
std::atomic<bool> BACKUP_FLAG;

NTSTATUS fork() {
    RTL_USER_PROCESS_INFORMATION processInfo;

    NTSTATUS status = RtlCloneUserProcess(
        RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES,
        NULL,
        NULL,
        NULL,
        &processInfo
    );

    if (status == STATUS_PROCESS_CLONED) {

        infinite_loop();

        FreeConsole();
        AttachConsole(ATTACH_PARENT_PROCESS);

        //printf("New: %s", PRINT_BUFFER);
        HMODULE self = GetModuleHandleA(NULL);
        printf("ByteB: %hhX\n", ((uint8_t*)self)[0x14E]);
        __fastfail(0);
        //NtTerminateProcess(current_process, STATUS_PROCESS_CLONED);
    }
    else if (NT_SUCCESS(status)) {
        status = NtWaitForSingleObject(processInfo.ProcessHandle, FALSE, NULL);

        NtClose(processInfo.ProcessHandle);
        NtClose(processInfo.ThreadHandle);

        if (!NT_SUCCESS(status)) {
            return status;
        }
    }
    return 0;
}

uint8_t PATCH[] = { 0x22 };

void fork_test() {

    
    //SetFilePointer(file_handle, (uint8_t*)PRINT_BUFFER - (uint8_t*)self, NULL, FILE_BEGIN);
    //WriteFile(file_handle, "Test passed\n", sizeof("Test passed\n"), &idgaf, NULL);

    //fork();
    HMODULE self = GetModuleHandleA(NULL);

    if (!BACKUP_FLAG) {
        BACKUP_FLAG = true;


        printf("ByteA: %hhX\n", ((uint8_t*)self)[0x8E]);

        HANDLE transaction = CreateTransaction(NULL, NULL, 0, 0, 0, 0, NULL);

        HANDLE file_handle = CreateFileTransactedW(
            L"ClangAsmTest1.exe",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL,
            transaction,
            NULL,
            NULL
        );
        DWORD idgaf;
        SetFilePointer(file_handle, 0x8E, NULL, FILE_BEGIN);
        WriteFile(file_handle, PATCH, sizeof(PATCH), &idgaf, NULL);

        STARTUPINFOW si{
            .cb = sizeof(si)
        };
        PROCESS_INFORMATION pi{};
        CreateProcessW(L"ClangAsmTest1.exe", NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi);

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        CloseHandle(file_handle);
        CloseHandle(transaction);

        printf("ByteC: %hhX\n", ((uint8_t*)self)[0x8E]);
    }
    else {
        FreeConsole();
        AttachConsole(ATTACH_PARENT_PROCESS);
        printf("ByteB: %hhX\n", ((uint8_t*)self)[0x8E]);
    }
}