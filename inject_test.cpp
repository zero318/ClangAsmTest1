#include "zero/util.h"
//#include "zero/BoundingBox.h"
#undef _WIN64
//#undef WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <winternl.h>


typedef struct _RTL_DRIVE_LETTER_CURDIR {
	USHORT                  Flags;
	USHORT                  Length;
	ULONG                   TimeStamp;
	UNICODE_STRING          DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	ULONG                   MaximumLength;
	ULONG                   Length;
	ULONG                   Flags;
	ULONG                   DebugFlags;
	PVOID                   ConsoleHandle;
	ULONG                   ConsoleFlags;
	HANDLE                  StdInputHandle;
	HANDLE                  StdOutputHandle;
	HANDLE                  StdErrorHandle;
	UNICODE_STRING          CurrentDirectoryPath;
	HANDLE                  CurrentDirectoryHandle;
	UNICODE_STRING          DllPath;
	UNICODE_STRING          ImagePathName;
	UNICODE_STRING          CommandLine;
	PVOID                   Environment;
	ULONG                   StartingPositionLeft;
	ULONG                   StartingPositionTop;
	ULONG                   Width;
	ULONG                   Height;
	ULONG                   CharWidth;
	ULONG                   CharHeight;
	ULONG                   ConsoleTextAttributes;
	ULONG                   WindowFlags;
	ULONG                   ShowWindowFlags;
	UNICODE_STRING          WindowTitle;
	UNICODE_STRING          DesktopName;
	UNICODE_STRING          ShellInfo;
	UNICODE_STRING          RuntimeData;
	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

int Inject(const HANDLE hProcess, const wchar_t *const dll_dir, const wchar_t *const dll_fn, const char *const func_name, const void *const param, const size_t param_size) {
	PROCESS_BASIC_INFORMATION   pbi;
	RTL_USER_PROCESS_PARAMETERS upp;
	PEB   peb;
	SIZE_T len;
	HANDLE hThread;

	typedef NTSTATUS NTAPI NtQueryInformationProcess_t(
		HANDLE ProcessHandle,
		PROCESSINFOCLASS ProcessInformationClass,
		PVOID ProcessInformation,
		ULONG ProcessInformationLength,
		PULONG ReturnLength
	);
	auto NtQueryInformationProcess = (NtQueryInformationProcess_t*)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryInformationProcess");
	NTSTATUS ret = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len);
	ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), &len);
	ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), &len);
	WCHAR path[upp.CurrentDirectoryPath.Length + 1];
	ReadProcessMemory(hProcess, upp.CurrentDirectoryPath.Buffer, path, upp.CurrentDirectoryPath.Length, &len);
	path[upp.CurrentDirectoryPath.Length / sizeof(WCHAR)] = 0;

	SIZE_T rBuf_len = wcslen(dll_dir) * sizeof(wchar_t) + wcslen(dll_fn) * sizeof(wchar_t) + param_size + strlen(func_name) + 8;
	LPVOID rBuf = VirtualAllocEx(hProcess, 0, rBuf_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hProcess, rBuf, dll_dir, wcslen(dll_dir) * sizeof(wchar_t), &len);
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SetCurrentDirectoryW, rBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	WriteProcessMemory(hProcess, rBuf, dll_fn, wcslen(dll_fn) * sizeof(wchar_t), &len);
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, rBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	uintptr_t hSelfRemote;
	GetExitCodeThread(hThread, (LPDWORD)&hSelfRemote);
	CloseHandle(hThread);

	WriteProcessMemory(hProcess, rBuf, path, upp.CurrentDirectoryPath.Length + sizeof(wchar_t), &len);
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)SetCurrentDirectoryW, rBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	HMODULE hSelf = GetModuleHandleW(dll_fn);
	uintptr_t func = (uintptr_t)GetProcAddress(hSelf, func_name);

	func = func - (uintptr_t)hSelf + hSelfRemote;
	WriteProcessMemory(hProcess, rBuf, param, param_size, &len);
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)func, rBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, rBuf, rBuf_len, MEM_DECOMMIT | MEM_RELEASE);

	return 0;
}