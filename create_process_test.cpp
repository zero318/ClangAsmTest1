#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include <utility>
#include <tuple>

#include <Windows.h>

//#include <ntdef.h>
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define OBJ_INHERIT                         0x00000002L
#define OBJ_PERMANENT                       0x00000010L
#define OBJ_EXCLUSIVE                       0x00000020L
#define OBJ_CASE_INSENSITIVE                0x00000040L
#define OBJ_OPENIF                          0x00000080L
#define OBJ_OPENLINK                        0x00000100L
#define OBJ_KERNEL_HANDLE                   0x00000200L
#define OBJ_FORCE_ACCESS_CHECK              0x00000400L
#define OBJ_IGNORE_IMPERSONATED_DEVICEMAP   0x00000800L
#define OBJ_DONT_REPARSE                    0x00001000L
#define OBJ_VALID_ATTRIBUTES                0x00001FF2L

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }

#include "zero/util.h"

#include "windows_structs.h"

#undef CreateProcess

#define CSR_MAKE_API_NUMBER( DllIndex, ApiIndex ) \
    (CSR_API_NUMBER)(((DllIndex) << 16) | (ApiIndex))
#define BASESRV_SERVERDLL_INDEX         1

#define PROCESS_PRIORITY_CLASS_UNKNOWN      0
#define PROCESS_PRIORITY_CLASS_IDLE         1
#define PROCESS_PRIORITY_CLASS_NORMAL       2
#define PROCESS_PRIORITY_CLASS_HIGH         3
#define PROCESS_PRIORITY_CLASS_REALTIME     4
#define PROCESS_PRIORITY_CLASS_BELOW_NORMAL 5
#define PROCESS_PRIORITY_CLASS_ABOVE_NORMAL 6

#define PROCESS_CREATE_FLAGS_BREAKAWAY               0x00000001
#define PROCESS_CREATE_FLAGS_NO_DEBUG_INHERIT        0x00000002
#define PROCESS_CREATE_FLAGS_INHERIT_HANDLES         0x00000004
#define PROCESS_CREATE_FLAGS_OVERRIDE_ADDRESS_SPACE  0x00000008
#define PROCESS_CREATE_FLAGS_LEGAL_MASK              0x0000000f

#define PROCESSOR_ARCHITECTURE_INTEL            0
#define PROCESSOR_ARCHITECTURE_MIPS             1
#define PROCESSOR_ARCHITECTURE_ALPHA            2
#define PROCESSOR_ARCHITECTURE_PPC              3
#define PROCESSOR_ARCHITECTURE_SHX              4
#define PROCESSOR_ARCHITECTURE_ARM              5
#define PROCESSOR_ARCHITECTURE_IA64             6
#define PROCESSOR_ARCHITECTURE_ALPHA64          7
#define PROCESSOR_ARCHITECTURE_MSIL             8
#define PROCESSOR_ARCHITECTURE_AMD64            9
#define PROCESSOR_ARCHITECTURE_IA32_ON_WIN64    10

#define PROCESSOR_ARCHITECTURE_UNKNOWN 0xFFFF

#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008
#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080
#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800
#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000
#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000
#define FILE_COPY_STRUCTURED_STORAGE            0x00000041
#define FILE_STRUCTURED_STORAGE                 0x00000441
#define FILE_VALID_OPTION_FLAGS                 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS            0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS        0x00000032
#define FILE_VALID_SET_FLAGS                    0x00000036

#define RTL_USER_PROC_PARAMS_NORMALIZED     0x00000001
#define RTL_USER_PROC_PROFILE_USER          0x00000002
#define RTL_USER_PROC_PROFILE_KERNEL        0x00000004
#define RTL_USER_PROC_PROFILE_SERVER        0x00000008
#define RTL_USER_PROC_RESERVE_1MB           0x00000020
#define RTL_USER_PROC_RESERVE_16MB          0x00000040
#define RTL_USER_PROC_CASE_SENSITIVE        0x00000080
#define RTL_USER_PROC_DISABLE_HEAP_DECOMMIT 0x00000100
#define RTL_USER_PROC_DLL_REDIRECTION_LOCAL 0x00001000
#define RTL_USER_PROC_APP_MANIFEST_PRESENT  0x00002000

static void resolve_nt_funcs();

static VOID NTAPI RtlAcquirePebLockShim();
typedef decltype(&RtlAcquirePebLockShim) RtlAcquirePebLockPtr;
static RtlAcquirePebLockPtr RtlAcquirePebLock = &RtlAcquirePebLockShim;
static VOID NTAPI RtlAcquirePebLockShim() {
    resolve_nt_funcs();
    return RtlAcquirePebLock();
}

static VOID NTAPI RtlReleasePebLockShim();
typedef decltype(&RtlReleasePebLockShim) RtlReleasePebLockPtr;
static RtlReleasePebLockPtr RtlReleasePebLock = &RtlReleasePebLockShim;
static VOID NTAPI RtlReleasePebLockShim() {
    resolve_nt_funcs();
    return RtlReleasePebLock();
}

static BOOLEAN NTAPI RtlDosPathNameToNtPathName_UShim(
    PCWSTR DosFileName,
    PUNICODE_STRING NtFileName,
    PWSTR* FilePart,
    PRTL_RELATIVE_NAME RelativeName
);
typedef decltype(&RtlDosPathNameToNtPathName_UShim) RtlDosPathNameToNtPathName_UPtr;
static RtlDosPathNameToNtPathName_UPtr RtlDosPathNameToNtPathName_U = &RtlDosPathNameToNtPathName_UShim;
static BOOLEAN NTAPI RtlDosPathNameToNtPathName_UShim(
    PCWSTR DosFileName,
    PUNICODE_STRING NtFileName,
    PWSTR* FilePart,
    PRTL_RELATIVE_NAME RelativeName
) {
    resolve_nt_funcs();
    return RtlDosPathNameToNtPathName_U(DosFileName, NtFileName, FilePart, RelativeName);
}

static NTSTATUS NTAPI NtOpenFileShim(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG ShareAccess,
    ULONG OpenOptions
);
typedef decltype(&NtOpenFileShim) NtOpenFilePtr;
static NtOpenFilePtr NtOpenFile = &NtOpenFileShim;
static NTSTATUS NTAPI NtOpenFileShim(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG ShareAccess,
    ULONG OpenOptions
) {
    resolve_nt_funcs();
    return NtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
}

static NTSTATUS NTAPI NtCreateSectionShim(
    PHANDLE SectionHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PLARGE_INTEGER MaximumSize,
    ULONG SectionPageProtection,
    ULONG AllocationAttributes,
    HANDLE FileHandle
);
typedef decltype(&NtCreateSectionShim) NtCreateSectionPtr;
static NtCreateSectionPtr NtCreateSection = &NtCreateSectionShim;
static NTSTATUS NTAPI NtCreateSectionShim(
    PHANDLE SectionHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PLARGE_INTEGER MaximumSize,
    ULONG SectionPageProtection,
    ULONG AllocationAttributes,
    HANDLE FileHandle
) {
    resolve_nt_funcs();
    return NtCreateSection(SectionHandle, DesiredAccess, ObjectAttributes, MaximumSize, SectionPageProtection, AllocationAttributes, FileHandle);
}

static NTSTATUS NTAPI NtCloseShim(
    HANDLE Handle
);
typedef decltype(&NtCloseShim) NtClosePtr;
static NtClosePtr NtClose = &NtCloseShim;
static NTSTATUS NTAPI NtCloseShim(
    HANDLE Handle
) {
    resolve_nt_funcs();
    return NtClose(Handle);
}

static NTSTATUS NTAPI NtQuerySectionShim(
    HANDLE SectionHandle,
    SECTION_INFORMATION_CLASS SectionInformationClass,
    PVOID SectionInformation,
    SIZE_T SectionInformationLength,
    PSIZE_T ReturnLength
);
typedef decltype(&NtQuerySectionShim) NtQuerySectionPtr;
static NtQuerySectionPtr NtQuerySection = &NtQuerySectionShim;
static NTSTATUS NTAPI NtQuerySectionShim(
    HANDLE SectionHandle,
    SECTION_INFORMATION_CLASS SectionInformationClass,
    PVOID SectionInformation,
    SIZE_T SectionInformationLength,
    PSIZE_T ReturnLength
) {
    resolve_nt_funcs();
    return NtQuerySection(SectionHandle, SectionInformationClass, SectionInformation, SectionInformationLength, ReturnLength);
}

static NTSTATUS NTAPI NtCreateProcessExShim(
    PHANDLE ProcessHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    HANDLE ParentProcess,
    ULONG Flags, // PROCESS_CREATE_FLAGS_*
    HANDLE SectionHandle,
    HANDLE DebugPort,
    HANDLE TokenHandle,
    ULONG JobMemberLevel
);
typedef decltype(&NtCreateProcessExShim) NtCreateProcessExPtr;
static NtCreateProcessExPtr NtCreateProcessEx = &NtCreateProcessExShim;
static NTSTATUS NTAPI NtCreateProcessExShim(
    PHANDLE ProcessHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    HANDLE ParentProcess,
    ULONG Flags, // PROCESS_CREATE_FLAGS_*
    HANDLE SectionHandle,
    HANDLE DebugPort,
    HANDLE TokenHandle,
    ULONG JobMemberLevel
) {
    resolve_nt_funcs();
    return NtCreateProcessEx(ProcessHandle, DesiredAccess, ObjectAttributes, ParentProcess, Flags, SectionHandle, DebugPort, TokenHandle, JobMemberLevel);
}

static NTSTATUS NTAPI NtTerminateProcessShim(
    HANDLE ProcessHandle,
    NTSTATUS ExitStatus
);
typedef decltype(&NtTerminateProcessShim) NtTerminateProcessPtr;
static NtTerminateProcessPtr NtTerminateProcess = &NtTerminateProcessShim;
static NTSTATUS NTAPI NtTerminateProcessShim(
    HANDLE ProcessHandle,
    NTSTATUS ExitStatus
) {
    resolve_nt_funcs();
    return NtTerminateProcess(ProcessHandle, ExitStatus);
}

static NTSTATUS NTAPI NtQueryInformationProcessShim(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
);
typedef decltype(&NtQueryInformationProcessShim) NtQueryInformationProcessPtr;
static NtQueryInformationProcessPtr NtQueryInformationProcess = &NtQueryInformationProcessShim;
static NTSTATUS NTAPI NtQueryInformationProcessShim(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
) {
    resolve_nt_funcs();
    return NtQueryInformationProcess(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
}

static NTSTATUS NTAPI RtlCreateProcessParametersShim(
    PRTL_USER_PROCESS_PARAMETERS* ProcessParameters,
    PUNICODE_STRING ImagePathName,
    PUNICODE_STRING DllPath,
    PUNICODE_STRING CurrentDirectory,
    PUNICODE_STRING CommandLine,
    PWSTR Environment,
    PUNICODE_STRING WindowTitle,
    PUNICODE_STRING DesktopInfo,
    PUNICODE_STRING ShellInfo,
    PUNICODE_STRING RuntimeData
);
typedef decltype(&RtlCreateProcessParametersShim) RtlCreateProcessParametersPtr;
static RtlCreateProcessParametersPtr RtlCreateProcessParameters = &RtlCreateProcessParametersShim;
static NTSTATUS NTAPI RtlCreateProcessParametersShim(
    PRTL_USER_PROCESS_PARAMETERS* ProcessParameters,
    PUNICODE_STRING ImagePathName,
    PUNICODE_STRING DllPath,
    PUNICODE_STRING CurrentDirectory,
    PUNICODE_STRING CommandLine,
    PWSTR Environment,
    PUNICODE_STRING WindowTitle,
    PUNICODE_STRING DesktopInfo,
    PUNICODE_STRING ShellInfo,
    PUNICODE_STRING RuntimeData
) {
    resolve_nt_funcs();
    return RtlCreateProcessParameters(ProcessParameters, ImagePathName, DllPath, CurrentDirectory, CommandLine, Environment, WindowTitle, DesktopInfo, ShellInfo, RuntimeData);
}

static NTSTATUS NTAPI RtlDestroyProcessParametersShim(
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters
);
typedef decltype(&RtlDestroyProcessParametersShim) RtlDestroyProcessParametersPtr;
static RtlDestroyProcessParametersPtr RtlDestroyProcessParameters = &RtlDestroyProcessParametersShim;
static NTSTATUS NTAPI RtlDestroyProcessParametersShim(
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters
) {
    resolve_nt_funcs();
    return RtlDestroyProcessParameters(ProcessParameters);
}

static NTSTATUS NTAPI NtAllocateVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect
);
typedef decltype(&NtAllocateVirtualMemoryShim) NtAllocateVirtualMemoryPtr;
static NtAllocateVirtualMemoryPtr NtAllocateVirtualMemory = &NtAllocateVirtualMemoryShim;
static NTSTATUS NTAPI NtAllocateVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect
) {
    resolve_nt_funcs();
    return NtAllocateVirtualMemory(ProcessHandle, BaseAddress, ZeroBits, RegionSize, AllocationType, Protect);
}

static NTSTATUS NTAPI NtWriteVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    SIZE_T NumberOfBytesToWrite,
    PSIZE_T NumberOfBytesWritten
);
typedef decltype(&NtWriteVirtualMemoryShim) NtWriteVirtualMemoryPtr;
static NtWriteVirtualMemoryPtr NtWriteVirtualMemory = &NtWriteVirtualMemoryShim;
static NTSTATUS NTAPI NtWriteVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    SIZE_T NumberOfBytesToWrite,
    PSIZE_T NumberOfBytesWritten
) {
    resolve_nt_funcs();
    return NtWriteVirtualMemory(ProcessHandle, BaseAddress, Buffer, NumberOfBytesToWrite, NumberOfBytesWritten);
}

static NTSTATUS NTAPI NtFreeVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T RegionSize,
    ULONG FreeType
);
typedef decltype(&NtFreeVirtualMemoryShim) NtFreeVirtualMemoryPtr;
static NtFreeVirtualMemoryPtr NtFreeVirtualMemory = &NtFreeVirtualMemoryShim;
static NTSTATUS NTAPI NtFreeVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T RegionSize,
    ULONG FreeType
) {
    resolve_nt_funcs();
    return NtFreeVirtualMemory(ProcessHandle, BaseAddress, RegionSize, FreeType);
}

static NTSTATUS NTAPI NtProtectVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T RegionSize,
    ULONG NewProtection,
    PULONG OldProtection
);
typedef decltype(&NtProtectVirtualMemoryShim) NtProtectVirtualMemoryPtr;
static NtProtectVirtualMemoryPtr NtProtectVirtualMemory = &NtProtectVirtualMemoryShim;
static NTSTATUS NTAPI NtProtectVirtualMemoryShim(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T RegionSize,
    ULONG NewProtection,
    PULONG OldProtection
) {
    resolve_nt_funcs();
    return NtProtectVirtualMemory(ProcessHandle, BaseAddress, RegionSize, NewProtection, OldProtection);
}

static NTSTATUS NTAPI NtCreateThreadShim(
    PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    HANDLE ProcessHandle,
    PCLIENT_ID ClientId,
    PCONTEXT ThreadContext,
    PINITIAL_TEB InitialTeb,
    BOOLEAN CreateSuspended
);
typedef decltype(&NtCreateThreadShim) NtCreateThreadPtr;
static NtCreateThreadPtr NtCreateThread = &NtCreateThreadShim;
static NTSTATUS NTAPI NtCreateThreadShim(
    PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    HANDLE ProcessHandle,
    PCLIENT_ID ClientId,
    PCONTEXT ThreadContext,
    PINITIAL_TEB InitialTeb,
    BOOLEAN CreateSuspended
) {
    resolve_nt_funcs();
    return NtCreateThread(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, ClientId, ThreadContext, InitialTeb, CreateSuspended);
}

static NTSTATUS NTAPI NtResumeThreadShim(
    HANDLE ThreadHandle,
    PULONG PreviousSuspendCount
);
typedef decltype(&NtResumeThreadShim) NtResumeThreadPtr;
static NtResumeThreadPtr NtResumeThread = &NtResumeThreadShim;
static NTSTATUS NTAPI NtResumeThreadShim(
    HANDLE ThreadHandle,
    PULONG PreviousSuspendCount
) {
    resolve_nt_funcs();
    return NtResumeThread(ThreadHandle, PreviousSuspendCount);
}

static NTSTATUS NTAPI CsrClientCallServerShim(
    PCSR_API_MSG ApiMsg,
    PVOID CaptureBuffer,
    ULONG ApiNumber,
    LONG ApiMessageDataSize
);
typedef decltype(&CsrClientCallServerShim) CsrClientCallServerPtr;
static CsrClientCallServerPtr CsrClientCallServer = &CsrClientCallServerShim;
static NTSTATUS NTAPI CsrClientCallServerShim(
    PCSR_API_MSG ApiMsg,
    PVOID CaptureBuffer,
    ULONG ApiNumber,
    LONG ApiMessageDataSize
) {
    resolve_nt_funcs();
    return CsrClientCallServer(ApiMsg, CaptureBuffer, ApiNumber, ApiMessageDataSize);
}

static VOID RtlUserThreadStartShim();
typedef decltype(&RtlUserThreadStartShim) RtlUserThreadStartPtr;
static RtlUserThreadStartPtr RtlUserThreadStart = NULL;

static BOOL WINAPI IsWow64ProcessShim(
    HANDLE hProcess,
    PBOOL Wow64Process
);
typedef decltype(&IsWow64ProcessShim) IsWow64ProcessPtr;
static IsWow64ProcessPtr IsWow64ProcessFunc = &IsWow64ProcessShim;
static BOOL WINAPI IsWow64ProcessShim(
    HANDLE hProcess,
    PBOOL Wow64Process
) {
    resolve_nt_funcs();
    return IsWow64ProcessFunc(hProcess, Wow64Process);
}
static BOOL WINAPI IsWow64ProcessFalse(
    HANDLE hProcess,
    PBOOL Wow64Process
) {
    *Wow64Process = FALSE;
    return TRUE;
}

/*
static NTSTATUS NTAPI NtWow64CsrBasepCreateProcessShim(
    PBASE_CREATEPROCESS_MSG a
);
typedef decltype(&NtWow64CsrBasepCreateProcessShim) NtWow64CsrBasepCreateProcessPtr;
static NtWow64CsrBasepCreateProcessPtr NtWow64CsrBasepCreateProcess = &NtWow64CsrBasepCreateProcessShim;
static NTSTATUS NTAPI NtWow64CsrBasepCreateProcessShim(
    PBASE_CREATEPROCESS_MSG a
) {
    resolve_nt_funcs();
    return NtWow64CsrBasepCreateProcess(a);
}
*/

static bool is_wow64_process() {
    BOOL ret;
    if (IsWow64ProcessFunc((HANDLE)-1, &ret)) {
        return ret;
    }
    return false;
}

static void resolve_nt_funcs() {
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    RtlAcquirePebLock = (RtlAcquirePebLockPtr)GetProcAddress(ntdll, "RtlAcquirePebLock");
    RtlReleasePebLock = (RtlReleasePebLockPtr)GetProcAddress(ntdll, "RtlReleasePebLock");
    RtlDosPathNameToNtPathName_U = (RtlDosPathNameToNtPathName_UPtr)GetProcAddress(ntdll, "RtlDosPathNameToNtPathName_U");
    NtOpenFile = (NtOpenFilePtr)GetProcAddress(ntdll, "NtOpenFile");
    NtCreateSection = (NtCreateSectionPtr)GetProcAddress(ntdll, "NtCreateSection");
    NtClose = (NtClosePtr)GetProcAddress(ntdll, "NtClose");
    NtQuerySection = (NtQuerySectionPtr)GetProcAddress(ntdll, "NtQuerySection");
    NtCreateProcessEx = (NtCreateProcessExPtr)GetProcAddress(ntdll, "NtCreateProcessEx");
    NtTerminateProcess = (NtTerminateProcessPtr)GetProcAddress(ntdll, "NtTerminateProcess");
    NtQueryInformationProcess = (NtQueryInformationProcessPtr)GetProcAddress(ntdll, "NtQueryInformationProcess");
    RtlCreateProcessParameters = (RtlCreateProcessParametersPtr)GetProcAddress(ntdll, "RtlCreateProcessParameters");
    RtlDestroyProcessParameters = (RtlDestroyProcessParametersPtr)GetProcAddress(ntdll, "RtlDestroyProcessParameters");
    NtAllocateVirtualMemory = (NtAllocateVirtualMemoryPtr)GetProcAddress(ntdll, "NtAllocateVirtualMemory");
    NtWriteVirtualMemory = (NtWriteVirtualMemoryPtr)GetProcAddress(ntdll, "NtWriteVirtualMemory");
    NtFreeVirtualMemory = (NtFreeVirtualMemoryPtr)GetProcAddress(ntdll, "NtFreeVirtualMemory");
    NtProtectVirtualMemory = (NtProtectVirtualMemoryPtr)GetProcAddress(ntdll, "NtProtectVirtualMemory");
    NtCreateThread = (NtCreateThreadPtr)GetProcAddress(ntdll, "NtCreateThread");
    NtResumeThread = (NtResumeThreadPtr)GetProcAddress(ntdll, "NtResumeThread");
    CsrClientCallServer = (CsrClientCallServerPtr)GetProcAddress(ntdll, "CsrClientCallServer");
    RtlUserThreadStart = (RtlUserThreadStartPtr)GetProcAddress(ntdll, "RtlUserThreadStart");

    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    IsWow64ProcessPtr is_wow64 = (IsWow64ProcessPtr)GetProcAddress(kernel32, "IsWow64Process");
    IsWow64ProcessFunc = is_wow64 ? is_wow64 : &IsWow64ProcessFalse;
    //NtWow64CsrBasepCreateProcess = (NtWow64CsrBasepCreateProcessPtr)GetProcAddress(kernel32, "_NtWow64CsrBasepCreateProcess@4");
}

#include "win_syscalls/syscalls7.h"
#include "win_syscalls/common.h"

typedef enum {
    BasepSearchPathEnd,         // end of path
    BasepSearchPathDlldir,      // use the dll dir; fallback to nothing
    BasepSearchPathAppdir,      // use the exe dir; fallback to base exe dir
    BasepSearchPathDefaultDirs, // use the default system dirs
    BasepSearchPathEnvPath,     // use %PATH%
    BasepSearchPathCurdir,      // use "."
    MaxBasepSearchPath
} BASEP_SEARCH_PATH_ELEMENT;

dllexport
gnu_noinline
BOOL
WINAPI
CreateProcessW_test(
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
) asm("CreateProcessW_test");
dllexport
gnu_noinline
BOOL
WINAPI
CreateProcessW_test(
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
    *lpProcessInformation = {};

    dwCreationFlags &= (ULONG)~CREATE_NO_WINDOW;

    if (
        // Conflicting flags
        (dwCreationFlags & (DETACHED_PROCESS | CREATE_NEW_CONSOLE)) == (DETACHED_PROCESS | CREATE_NEW_CONSOLE) ||
        // Conflicting flags
        (dwCreationFlags & (CREATE_SEPARATE_WOW_VDM | CREATE_SHARED_WOW_VDM)) == (CREATE_SEPARATE_WOW_VDM | CREATE_SHARED_WOW_VDM) ||
        // Unsupported flags
        (dwCreationFlags & (DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS | CREATE_DEFAULT_ERROR_MODE | PROFILE_USER | PROFILE_KERNEL | PROFILE_SERVER))
    ) {
        goto invalid_parameter;
    }

    // No unicode environment support
    if (lpEnvironment && (dwCreationFlags & CREATE_UNICODE_ENVIRONMENT)) {
        goto invalid_parameter;
    }

    DWORD directory_length;
    if (lpCurrentDirectory) {
        wchar_t* directory = (wchar_t*)malloc((MAX_PATH + 1) * sizeof(wchar_t));
        DWORD attr;
        if (
            (directory_length = GetFullPathNameW(lpCurrentDirectory, MAX_PATH, directory, NULL)) > MAX_PATH ||
            (
                (attr = GetFileAttributesW(directory)) == INVALID_FILE_ATTRIBUTES ||
                !(attr & FILE_ATTRIBUTE_DIRECTORY)
            )
        ) {
            goto invalid_parameter_free1;
        }

        lpCurrentDirectory = (LPCWSTR)directory;
    }
    else {
        directory_length = GetCurrentDirectoryW(0, NULL);
        lpCurrentDirectory = (LPCWSTR)malloc(directory_length * sizeof(wchar_t));
        GetCurrentDirectoryW(directory_length, (LPWSTR)lpCurrentDirectory);
        --directory_length;
    }
    UNICODE_STRING directory;
    directory.Buffer = (LPWSTR)lpCurrentDirectory;
    directory.Length = directory_length * sizeof(wchar_t);
    directory.MaximumLength = (directory_length + 1) * sizeof(wchar_t);

    // ignore priority BS
    constexpr DWORD PRIORITY_CLASS_MASK = IDLE_PRIORITY_CLASS | BELOW_NORMAL_PRIORITY_CLASS | NORMAL_PRIORITY_CLASS | ABOVE_NORMAL_PRIORITY_CLASS | HIGH_PRIORITY_CLASS | REALTIME_PRIORITY_CLASS;
    /*
    PROCESS_PRIORITY_CLASS priority = {
        .Foreground = FALSE,
        .PriorityClass = PROCESS_PRIORITY_CLASS_UNKNOWN
    };

    DWORD priority_class = dwCreationFlags & PRIORITY_CLASS_MASK;
    if (priority_class) {
        priority.PriorityClass = _bit_scan_forward(priority_class) - (priority_class >= BELOW_NORMAL_PRIORITY_CLASS ? 9 : 4);
    }
    */
    dwCreationFlags &= ~PRIORITY_CLASS_MASK;

    wchar_t* wbuffer = NULL;

#define QUOTE_INSERT 1
    uint8_t flags = 0;
    wchar_t* command_line = NULL;
    UNICODE_STRINGX<> image;
    UNICODE_STRINGX<> cmdline;
    if (!lpApplicationName) {
        wbuffer = (wchar_t*)malloc(sizeof(wchar_t[MAX_PATH]));

        wchar_t* program_end = lpCommandLine;
        if (*(uint16_t*)program_end == L'"') {
            lpApplicationName = ++program_end;
            for (;;) {
                switch (*(uint16_t*)program_end) {
                    case L'"':
                    case L'\0':
                        goto end_program_search;
                }
                ++program_end;
            }
        } else {
            lpApplicationName = program_end;
        continue_search:
            for (;;) {
                switch (*(uint16_t*)program_end) {
                    case L' ': case L'\t':
                    case L'\0':
                        goto end_program_search;
                }
                ++program_end;
            }
        }
    end_program_search:

        wchar_t temp_char = *(uint16_t*)program_end;
        *program_end = L'\0';
        DWORD image_length = SearchPathW(NULL, lpApplicationName, L".exe", MAX_PATH, wbuffer, NULL);
        *program_end = temp_char;

        DWORD attr;
        if (
            !image_length ||
            image_length >= MAX_PATH ||
            (
                (attr = GetFileAttributesW(wbuffer)) != INVALID_FILE_ATTRIBUTES &&
                (attr & FILE_ATTRIBUTE_DIRECTORY)
            )
        ) {
            if (temp_char && *(uint16_t*)lpCommandLine != L'"') {
                flags = QUOTE_INSERT;
                goto continue_search;
            }
            return FALSE;
        }

        image.Buffer = wbuffer;
        image.Length = image_length * sizeof(wchar_t);
        image.MaximumLength = (image_length + 1) * sizeof(wchar_t);

        lpApplicationName = wbuffer;
        size_t command_line_length = byteloop_wcslen(lpCommandLine);
        if (flags) {
            command_line = (wchar_t*)malloc((command_line_length + 3) * sizeof(wchar_t));
            command_line[0] = L'"';
            command_line[command_line_length + 1] = L'"';
            command_line[command_line_length + 2] = L'\0';
            wchar_t* old_cmd = lpCommandLine;
            lpCommandLine = command_line;
            cmdline.Buffer = command_line;
            cmdline.Length = (command_line_length + 2) * sizeof(wchar_t);
            cmdline.MaximumLength = (command_line_length + 3) * sizeof(wchar_t);
            memcpy(command_line + 1, old_cmd, command_line_length * sizeof(wchar_t));
        }
        else {
            cmdline.Buffer = lpCommandLine;
            cmdline.Length = command_line_length * sizeof(wchar_t);
            cmdline.MaximumLength = command_line_length + sizeof(wchar_t);
        }
    }
    else if (!lpCommandLine || !*lpCommandLine) {
        //flags = QUOTE_CMDLINE;
        //lpCommandLine = (LPWSTR)lpApplicationName;
        size_t command_line_length = byteloop_wcslen(lpApplicationName);
        command_line = lpCommandLine = (wchar_t*)malloc((command_line_length + 3) * sizeof(wchar_t));
        command_line[0] = L'"';
        command_line[command_line_length + 1] = L'"';
        command_line[command_line_length + 2] = L'\0';
        cmdline.Buffer = command_line;
        cmdline.Length = (command_line_length + 2) * sizeof(wchar_t);
        cmdline.MaximumLength = (command_line_length + 3) * sizeof(wchar_t);
        image = cmdline;
        memcpy(command_line + 1, lpApplicationName, command_line_length * sizeof(wchar_t));
    }

    UNICODE_STRING path;
    RTL_RELATIVE_NAME relative;
    if (!RtlDosPathNameToNtPathName_U(lpApplicationName, &path, NULL, &relative)) {
        goto invalid_parameter_free3;
    }

    // wtf is an sxs, do I need to care?

    if (relative.RelativeName.Length) {
        path = *(UNICODE_STRING*)&relative.RelativeName;
    } else {
        relative.ContainingDirectory = NULL;
    }

    OBJECT_ATTRIBUTES object_attr;
    InitializeObjectAttributes(&object_attr, &path, OBJ_CASE_INSENSITIVE, relative.ContainingDirectory, NULL);

    HANDLE file;
    IO_STATUS_BLOCK io_status;
    if (NT_SUCCESS(NtOpenFile(
        &file,
        SYNCHRONIZE | FILE_EXECUTE | FILE_READ_ATTRIBUTES | FILE_READ_DATA,
        &object_attr,
        &io_status,
        FILE_SHARE_READ | FILE_SHARE_DELETE,
        FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE
    ))) {
        if (!lpStartupInfo->lpDesktop) {
            lpStartupInfo->lpDesktop = peb->ProcessParameters->DesktopInfo.Buffer;
        }

        HANDLE section;
        if (NT_SUCCESS(NtCreateSection(&section, SECTION_ALL_ACCESS, NULL, NULL, PAGE_EXECUTE, SEC_IMAGE, file))) {
            SECTION_IMAGE_INFORMATION image_info;
            if (
                NT_SUCCESS(NtQuerySection(section, SectionImageInformation, &image_info, sizeof(image_info), NULL)) &&
                !(image_info.ImageCharacteristics & IMAGE_FILE_DLL)
            ) {

                // just pretend the no isolation flag is set because it can skip stuff
                // CRAP that doesn't work

                bool is_wow64 = is_wow64_process();

                BASE_API_MSGX<32> csr_message;

                switch (image_info.Machine) {
                    default:
                        goto error_section_close;
                    case IMAGE_FILE_MACHINE_AMD64:
                        csr_message.u.CreateProcess.ProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;
                        break;
                    case IMAGE_FILE_MACHINE_I386:
                        //csr_message.u.CreateProcess.ProcessorArchitecture = PROCESSOR_ARCHITECTURE_INTEL;
                        //csr_message.u.CreateProcess.ProcessorArchitecture = PROCESSOR_ARCHITECTURE_IA32_ON_WIN64;
                        csr_message.u.CreateProcess.ProcessorArchitecture = is_wow64 ? PROCESSOR_ARCHITECTURE_IA32_ON_WIN64 : PROCESSOR_ARCHITECTURE_INTEL;
                        break;
                }
                csr_message.u.CreateProcess.ProcessHandle = NULL;
                switch (image_info.SubSystemType) {
                    case IMAGE_SUBSYSTEM_WINDOWS_GUI:
                        csr_message.u.CreateProcess.ProcessHandle = (void*)3;
                    case IMAGE_SUBSYSTEM_WINDOWS_CUI:
                        if (
                            image_info.SubSystemMajorVersion > 3 &&
                            (
                                image_info.SubSystemMajorVersion > USER_SHARED_DATAR().NtMajorVersion ||
                                (
                                    image_info.SubSystemMajorVersion == USER_SHARED_DATAR().NtMajorVersion &&
                                    image_info.SubSystemMinorVersion <= USER_SHARED_DATAR().NtMinorVersion
                                )
                            )
                        ) {
                            if (lpProcessAttributes) {
                                InitializeObjectAttributes(&object_attr, NULL, lpProcessAttributes->bInheritHandle ? OBJ_INHERIT : 0, NULL, lpProcessAttributes->lpSecurityDescriptor);
                                lpProcessAttributes = (LPSECURITY_ATTRIBUTES)&object_attr;
                            }

                            DWORD flags = PROCESS_CREATE_FLAGS_OVERRIDE_ADDRESS_SPACE;
                            if (dwCreationFlags & CREATE_BREAKAWAY_FROM_JOB) {
                                flags |= PROCESS_CREATE_FLAGS_BREAKAWAY;
                            }
                            if (bInheritHandles) {
                                flags |= PROCESS_CREATE_FLAGS_INHERIT_HANDLES;
                            }
                            HANDLE process;
                            if (NT_SUCCESS(NtCreateProcessEx(
                                &process,
                                PROCESS_ALL_ACCESS,
                                (POBJECT_ATTRIBUTES)lpProcessAttributes,
                                (HANDLE)-1,
                                flags,
                                section,
                                NULL,
                                NULL,
                                0
                            ))) {

                                // ignore hard error mode thing

                                PROCESS_BASIC_INFORMATION process_info;
                                if (NT_SUCCESS(NtQueryInformationProcess(
                                    process,
                                    ProcessBasicInformation,
                                    &process_info,
                                    sizeof(process_info),
                                    NULL
                                ))) {
                                    PEB* new_peb = process_info.PebBaseAddress;

                                    wchar_t* windows_folder_ptr;
                                    wchar_t* system32_folder_ptr;
                                    size_t windows_folder_length;
                                    size_t system32_folder_length;
                                    // Default directory
#if !_WIN64
                                    if (USER_SHARED_DATAR().ImageNumberLow != 0x8664) {
                                        const UNICODE_STRINGX<32>* system_paths = (UNICODE_STRINGX<32>*)peb->ReadOnlyStaticServerData;
                                        const UNICODE_STRINGX<32>& windows_folder = system_paths[0];
                                        const UNICODE_STRINGX<32>& system32_folder = system_paths[1];
                                        windows_folder_length = windows_folder.Length;
                                        windows_folder_ptr = windows_folder.Buffer;
                                        system32_folder_length = system32_folder.Length;
                                        system32_folder_ptr = system32_folder.Buffer;
                                    }
                                    else
#endif
                                    {
                                        const UNICODE_STRINGX<64>* system_paths = (UNICODE_STRINGX<64>*)based_pointer(peb->ReadOnlyStaticServerData, 0x30);
                                        const UNICODE_STRINGX<64>& windows_folder = system_paths[0];
                                        const UNICODE_STRINGX<64>& system32_folder = system_paths[1];
                                        windows_folder_length = windows_folder.Length;
                                        windows_folder_ptr = windows_folder.Buffer;
                                        system32_folder_length = system32_folder.Length;
                                        system32_folder_ptr = system32_folder.Buffer;
                                    }

                                    size_t dll_search_length =
                                        //image.Length + sizeof(L';') + // App directory
                                        system32_folder_length + sizeof(L';') + // system32
                                        windows_folder_length + sizeof(L"\\system") + sizeof(L';') + // system
                                        windows_folder_length + sizeof(L';') +
                                        sizeof(L'.'); // Current directory
                                        // Environment (skip for now?)

                                    wchar_t* dll_search_buffer = (wchar_t*)malloc(dll_search_length + sizeof(wchar_t));
                                    if (dll_search_buffer) {
                                        wchar_t* dll_search_write = dll_search_buffer;

                                        /*
                                        *based_pointer(dll_search_write, image.Length) = L';';
                                        memcpy(dll_search_write, image.Buffer, image.Length);
                                        dll_search_write = based_pointer(dll_search_write, image.Length + sizeof(L';'));
                                        */

                                        *based_pointer(dll_search_write, system32_folder_length) = L';';
                                        memcpy(dll_search_write, system32_folder_ptr, system32_folder_length);
                                        dll_search_write = based_pointer(dll_search_write, system32_folder_length + sizeof(L';'));

                                        memcpy(dll_search_write, windows_folder_ptr, windows_folder_length);
                                        dll_search_write = based_pointer(dll_search_write, windows_folder_length);
                                        memcpy(dll_search_write, L"\\system;", sizeof(L"\\system;") - sizeof(L'\0'));
                                        dll_search_write = based_pointer(dll_search_write, sizeof(L"\\system;") - sizeof(L'\0'));

                                        memcpy(dll_search_write, windows_folder_ptr, windows_folder_length);
                                        dll_search_write = based_pointer(dll_search_write, windows_folder_length);

                                        *dll_search_write++ = L';';
                                        *dll_search_write++ = L'.';
                                        *dll_search_write++ = L'\0';

                                        wprintf(
                                            L"DLL:  %s\n"
                                            , dll_search_buffer
                                        );

                                        UNICODE_STRINGX<> dll_search_path;
                                        dll_search_path.Buffer = dll_search_buffer;
                                        dll_search_path.Length = dll_search_length;
                                        dll_search_path.MaximumLength = dll_search_length + sizeof(wchar_t);

                                        UNICODE_STRINGX<> title;
                                        if (wchar_t* title_str = lpStartupInfo->lpTitle) {
                                            size_t title_length = byteloop_wcslen(title_str);
                                            title.Buffer = title_str;
                                            title.Length = title_length * sizeof(wchar_t);
                                            title.MaximumLength = (title_length + 1) * sizeof(wchar_t);
                                        } else {
                                            title = image;
                                        }
                                    
                                        UNICODE_STRINGX<> desktop;
                                        desktop.Buffer = (wchar_t*)L"";
                                        desktop.Length = 0;
                                        desktop.MaximumLength = sizeof(wchar_t);
                                        if (wchar_t* desktop_str = lpStartupInfo->lpDesktop) {
                                            size_t desktop_length = byteloop_wcslen(desktop_str);
                                            desktop.Buffer = desktop_str;
                                            desktop.Length = desktop_length * sizeof(wchar_t);
                                            desktop.MaximumLength = (desktop_length + 1) * sizeof(wchar_t);
                                        }

                                        UNICODE_STRINGX<> shell;
                                        shell.Buffer = (wchar_t*)L"";
                                        shell.Length = 0;
                                        shell.MaximumLength = sizeof(wchar_t);
                                        if (wchar_t* shell_str = lpStartupInfo->lpReserved) {
                                            size_t shell_length = byteloop_wcslen(shell_str);
                                            shell.Buffer = shell_str;
                                            shell.Length = shell_length * sizeof(wchar_t);
                                            shell.MaximumLength = (shell_length + 1) * sizeof(wchar_t);
                                        }

                                        UNICODE_STRINGX<> runtime;
                                        runtime.Buffer = (PWSTR)lpStartupInfo->lpReserved2;
                                        runtime.MaximumLength = runtime.Length = lpStartupInfo->cbReserved2;

                                        // Assume no manifest

                                        PRTL_USER_PROCESS_PARAMETERS process_parameters;
                                        if (NT_SUCCESS(RtlCreateProcessParameters(
                                            &process_parameters,
                                            (PUNICODE_STRING)&image,
                                            //(PUNICODE_STRING)&dll_search_path, // this isn't how this parameter is supposed to work
                                            (PUNICODE_STRING)&directory,
                                            (PUNICODE_STRING)&directory,
                                            (PUNICODE_STRING)&cmdline,
                                            (PWSTR)lpEnvironment,
                                            (PUNICODE_STRING)&title,
                                            (PUNICODE_STRING)&desktop,
                                            (PUNICODE_STRING)&shell,
                                            (PUNICODE_STRING)&runtime
                                        ))) {
                                            const wchar_t* environment = (wchar_t*)(lpEnvironment ? process_parameters->Environment : (RtlAcquirePebLock(), peb->ProcessParameters->Environment));
                                            process_parameters->Environment = NULL;
                                            SIZE_T region_size_environment;
                                            if (environment) {
                                                size_t environment_length = 0;
                                                while (environment[environment_length] || environment[environment_length + 1]) ++environment_length;
                                                environment_length += sizeof(wchar_t);

                                                region_size_environment = environment_length;
                                                if (
                                                    !NT_SUCCESS(NtAllocateVirtualMemory(process, (PVOID*)&process_parameters->Environment, 0, &region_size_environment, MEM_COMMIT, PAGE_READWRITE))
                                                ) {
                                                    if (lpEnvironment) goto error_destroy_parameters;
                                                    goto error_release_peb_lock;
                                                }
                                                if (
                                                    !NT_SUCCESS(NtWriteVirtualMemory(process, process_parameters->Environment, (PVOID)environment, environment_length, NULL))
                                                ) {
                                                    NtFreeVirtualMemory(process, (PVOID*)process_parameters->Environment, &region_size_environment, MEM_RELEASE);
                                                    if (lpEnvironment) goto error_destroy_parameters;
                                                    goto error_release_peb_lock;
                                                }
                                            }
                                            if (!lpEnvironment) {
                                                RtlReleasePebLock();
                                            }

                                            if (!bInheritHandles) {
                                                process_parameters->CurrentDirectory.Handle = NULL;
                                            }
                                            process_parameters->StartingX = lpStartupInfo->dwX;
                                            process_parameters->StartingY = lpStartupInfo->dwY;
                                            process_parameters->CountX = lpStartupInfo->dwXSize;
                                            process_parameters->CountY = lpStartupInfo->dwYSize;
                                            process_parameters->CountCharsX = lpStartupInfo->dwXCountChars;
                                            process_parameters->CountCharsY = lpStartupInfo->dwYCountChars;
                                            process_parameters->FillAttribute = lpStartupInfo->dwFillAttribute;
                                            process_parameters->WindowFlags = lpStartupInfo->dwFlags;
                                            process_parameters->ShowWindowFlags = lpStartupInfo->wShowWindow;

                                            process_parameters->ConsoleHandle = peb->ProcessParameters->ConsoleHandle;
                                            if (lpStartupInfo->dwFlags & STARTF_USESTDHANDLES) {
                                                process_parameters->StandardInput = lpStartupInfo->hStdInput;
                                                process_parameters->StandardOutput = lpStartupInfo->hStdOutput;
                                                process_parameters->StandardError = lpStartupInfo->hStdError;
                                            }
                                            // doesn't deal with console handles

                                            process_parameters->Flags |= (peb->ProcessParameters->Flags & RTL_USER_PROC_DISABLE_HEAP_DECOMMIT);
                                            SIZE_T region_size = process_parameters->Length;

                                            PRTL_USER_PROCESS_PARAMETERS new_params = NULL;
                                            if (NT_SUCCESS(NtAllocateVirtualMemory(process, (PVOID*)&new_params, 0, &region_size, MEM_COMMIT, PAGE_READWRITE))) {
                                                process_parameters->MaximumLength = region_size;
                                                if (
                                                    NT_SUCCESS(NtWriteVirtualMemory(process, new_params, process_parameters, process_parameters->Length, NULL)) &&
                                                    NT_SUCCESS(NtWriteVirtualMemory(process, &new_peb->ProcessParameters, &new_params, sizeof(new_params), NULL))
                                                ) {

                                                    SIZE_T stack_commit = image_info.CommittedStackSize;
                                                    SIZE_T stack_max = std::max((uintptr_t)image_info.MaximumStackSize, (uintptr_t)0x40000);
                                                    if (!stack_commit) {
                                                        void* self_image_base = peb->ImageBaseAddress;
                                                        stack_commit = based_pointer<IMAGE_NT_HEADERS>(self_image_base, ((PIMAGE_DOS_HEADER)self_image_base)->e_lfanew)->OptionalHeader.SizeOfStackCommit;
                                                        stack_commit = std::max(stack_commit, (SIZE_T)peb->MinimumStackCommit);
                                                        stack_max = AlignUpToMultipleOf2(stack_max, 4096);
                                                    }
                                                    else {
                                                        stack_commit = std::max(stack_commit, (SIZE_T)peb->MinimumStackCommit);
                                                        if (stack_commit >= stack_max) {
                                                            stack_max = AlignUpToMultipleOf2(stack_commit, 1048576);
                                                        }
                                                    }
                                                    stack_commit = AlignUpToMultipleOf2(stack_commit, 4096);

                                                    uint8_t* stack_top = NULL;
                                                    if (NT_SUCCESS(NtAllocateVirtualMemory(process, (PVOID*)&stack_top, 0, &stack_max, MEM_RESERVE, PAGE_READWRITE))) {
                                                        uint8_t* stack = stack_top;
                                                        uint8_t* stack_base = stack_top + stack_max;
                                                        uint8_t* stack_limit;
                                                        stack += stack_max - stack_commit;
                                                        if (stack_max > stack_commit) {
                                                            stack -= 4096;
                                                            stack_commit += 4096;
                                                            if (!NT_SUCCESS(NtAllocateVirtualMemory(process, (PVOID*)&stack, 0, &stack_commit, MEM_COMMIT, PAGE_READWRITE))) {
                                                                goto stack_fail;
                                                            }
                                                            stack_limit = stack;
                                                            SIZE_T size = 4096;
                                                            static ULONG idgaf;
                                                            if (NT_SUCCESS(NtProtectVirtualMemory(process, (PVOID*)&stack, &size, PAGE_GUARD | PAGE_READWRITE, &idgaf))) {
                                                                stack_limit += size;
                                                            }
                                                        } else {
                                                            if (!NT_SUCCESS(NtAllocateVirtualMemory(process, (PVOID*)&stack, 0, &stack_commit, MEM_COMMIT, PAGE_READWRITE))) {
                                                                goto stack_fail;
                                                            }
                                                            stack_limit = stack;
                                                        }

                                                        CONTEXT context = {};
                                                        context.ContextFlags = CONTEXT_FULL;
                                                        context.EFlags = 0x3000;
                                                        context.SegDs = context.SegEs = context.SegSs = context.SegGs = 0x2B;
                                                        context.SegFs = 0x53;
    #if _WIN64
                                                        context.Rcx = (DWORD64)image_info.TransferAddress;
                                                        context.Rdx = (DWORD64)new_peb;
                                                        context.SegCs = 0x33;
                                                        context.Rsp = (DWORD64)stack_base - sizeof(uintptr_t);
                                                        context.Rip = (DWORD64)&RtlUserThreadStart;
    #else
                                                        context.Eax = (DWORD)image_info.TransferAddress;
                                                        context.Ebx = (DWORD)new_peb;
                                                        context.SegCs = 0x23;
                                                        context.Esp = (DWORD)stack_base - sizeof(uintptr_t);
                                                        context.Eip = (DWORD)&RtlUserThreadStart;
    #endif

                                                        if (lpThreadAttributes) {
                                                            InitializeObjectAttributes(&object_attr, NULL, lpThreadAttributes->bInheritHandle ? OBJ_INHERIT : 0, NULL, lpThreadAttributes->lpSecurityDescriptor);
                                                            lpThreadAttributes = (LPSECURITY_ATTRIBUTES)&object_attr;
                                                        }
                                                            
                                                        INITIAL_TEB initial_teb;
                                                        initial_teb.OldInitialTeb = {};
                                                        initial_teb.StackBase = stack_base;
                                                        initial_teb.StackLimit = stack_limit;
                                                        initial_teb.StackAllocationBase = stack_top;

                                                        HANDLE thread;
                                                        CLIENT_ID client_id;
                                                        if (NT_SUCCESS(NtCreateThread(
                                                            &thread,
                                                            THREAD_ALL_ACCESS,
                                                            (POBJECT_ATTRIBUTES)lpThreadAttributes,
                                                            process,
                                                            &client_id,
                                                            &context,
                                                            &initial_teb,
                                                            TRUE
                                                        ))) {

                                                            csr_message.u.CreateProcess.Sxs = {};
                                                            csr_message.u.CreateProcess.Peb = (uint64_t)new_peb;
#if _WIN64
                                                            csr_message.u.CreateProcess.RealPeb = (uint64_t)new_peb;
#else
                                                            csr_message.u.CreateProcess.RealPeb = (uint32_t)NULL;
#endif
                                                            csr_message.u.CreateProcess.ProcessHandle = (void*)((uintptr_t)csr_message.u.CreateProcess.ProcessHandle | (uintptr_t)process);
                                                            csr_message.u.CreateProcess.ThreadHandle = thread;
                                                            //csr_message.u.CreateProcess.ClientId = client_id;
                                                            csr_message.u.CreateProcess.ClientId.UniqueProcess = client_id.UniqueProcess;
                                                            csr_message.u.CreateProcess.ClientId.UniqueThread = client_id.UniqueThread;
                                                            csr_message.u.CreateProcess.CreationFlags = dwCreationFlags;
                                                            csr_message.u.CreateProcess.VdmBinaryType = 0;
                                                        
                                                            constexpr auto wkjrbkwrjb32 = sizeof(BASE_CREATEPROCESS_MSGX<32>);
                                                            constexpr auto wkjrbkwrjb64 = sizeof(BASE_CREATEPROCESS_MSGX<64>);
                                                            constexpr auto wkjrbkwrjbRY = 0x110;
                                                            if (!is_wow64) {
                                                                CsrClientCallServer(
                                                                    (PCSR_API_MSG)&csr_message,
                                                                    NULL,
                                                                    CSR_MAKE_API_NUMBER(BASESRV_SERVERDLL_INDEX, BasepCreateProcess),
                                                                    sizeof(csr_message.u.CreateProcess)
                                                                );
                                                            } else {
                                                                csr_message.ReturnValue = NtWow64CsrBasepCreateProcess(&csr_message.u.CreateProcess);
                                                            }
                                                        
                                                            // This is wrong somehow, the struct def is bad
                                                            if (NT_SUCCESS((NTSTATUS)csr_message.ReturnValue))
                                                            {
                                                            
                                                                if (!(dwCreationFlags & CREATE_SUSPENDED)) {
                                                                    NtResumeThread(thread, NULL);
                                                                }

                                                                lpProcessInformation->hProcess = process;
                                                                lpProcessInformation->hThread = thread;
                                                                lpProcessInformation->dwProcessId = (ULONG)client_id.UniqueProcess;
                                                                lpProcessInformation->dwThreadId = (ULONG)client_id.UniqueThread;


                                                                RtlDestroyProcessParameters(process_parameters);
                                                                NtClose(file);
                                                                NtClose(section);
                                                                free(dll_search_buffer);
                                                                free(command_line);
                                                                free(wbuffer);
                                                                free((void*)lpCurrentDirectory);
                                                                return TRUE;
                                                            }
                                                            NtTerminateProcess(process, (NTSTATUS)csr_message.ReturnValue);
                                                            NtClose(thread);
                                                        }
                                                    }
                                                stack_fail:
                                                    NtFreeVirtualMemory(process, (PVOID*)&stack_top, &stack_max, MEM_RELEASE);
                                                }
                                                NtFreeVirtualMemory(process, (PVOID*)new_params, &region_size, MEM_RELEASE);
                                            }
                                            if (process_parameters->Environment) {
                                                NtFreeVirtualMemory(process, (PVOID*)process_parameters->Environment, &region_size_environment, MEM_RELEASE);
                                            }
                                            RtlDestroyProcessParameters(process_parameters);
                                        }
                                        else if (0) {
                                    error_release_peb_lock:
                                            RtlReleasePebLock();
                                    error_destroy_parameters:
                                            RtlDestroyProcessParameters(process_parameters);
                                        }
                                        free(dll_search_buffer);
                                    }
                                }
                            }
                        error_close_process:
                            NtTerminateProcess(process, (NTSTATUS)0);
                            NtClose(process);
                        }
                }
            }
        error_section_close:
            NtClose(section);
        }
        NtClose(file);
    }
invalid_parameter_free3:
    free(command_line);
    free(wbuffer);
invalid_parameter_free1:
    free((void*)lpCurrentDirectory);
invalid_parameter:
    SetLastError(ERROR_INVALID_PARAMETER);
    return FALSE;
}

#define HARDCODE_PATH 1

int main(int argc, char* argv[]) {

    int ret = EXIT_FAILURE;
#if !HARDCODE_PATH
    if (argc > 1)
#endif
    {
        const char* path;
#if !HARDCODE_PATH
        path = argv[1];
#else
        if constexpr (sizeof(void*) == 8) {
            path = "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\out\\build\\x64-Clang-Release\\smallest_pe_file.exe";
        }
        else {
            //path = "F:\\Touhou_Stuff_2\\disassembly_stuff\\18\\crack\\th18.exe.unvlv.exe";
            path = "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\out\\build\\x86-Clang-Release\\smallest_pe_file.exe";
        }
#endif

        size_t path_len = strlen(path);
        wchar_t* wpath = (wchar_t*)malloc(sizeof(wchar_t) * path_len * 2);
        if (wpath) {

            int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, wpath, path_len * 4);
            wchar_t* directory_end = NULL;
            wchar_t* temp = wpath;
            do {
                switch (*temp) {
                    case L'/':
                        *temp = L'\\';
                    case L'\\':
                        directory_end = temp;
                }
                ++temp;
            } while (--length);
            if (!directory_end) {
                directory_end = temp;
            }

            size_t directory_length = directory_end - wpath + 1;
            wchar_t* directory = (wchar_t*)malloc((directory_length + 1) * sizeof(wchar_t));
            if (directory) {
                directory[directory_length] = L'\0';
                memcpy(directory, wpath, directory_length * sizeof(wchar_t));

                wprintf(
                    L"Exe:  %s\n"
                    L"Path: %s\n"
                    , wpath
                    , directory
                );

                STARTUPINFOW si = { .cb = sizeof(STARTUPINFOW) };
                PROCESS_INFORMATION pi;

                if (CreateProcessW_test(wpath, NULL, NULL, NULL, FALSE, 0, NULL, directory, &si, &pi)) {
                    WaitForSingleObject(pi.hProcess, INFINITE);
                    DWORD exit_code;
                    GetExitCodeProcess(pi.hProcess, &exit_code);
                    printf(
                        "Exit: %X\n"
                        , exit_code
                    );
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                    ret = EXIT_SUCCESS;
                }
                free(directory);
            }
            free(wpath);
        }
    }

    return ret;
}