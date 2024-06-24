#ifndef SYSCALLS_COMMON_H
#define SYSCALLS_COMMON_H 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>

#include "../zero/util.h"

#include "../windows_structs.h"

__if_exists(NtSetInformationThreadID) {
	// 0x000A
	inline NTSTATUS NtSetInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength) {
		return syscall4<NTSTATUS>(NtSetInformationThreadID, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
	}
}

__if_exists(NtWaitForMultipleObjects32ID) {
	// 0x0017
	inline NTSTATUS NtWaitForMultipleObjects32(ULONG ObjectCount, PHANDLE ObjectsArray, OBJECT_WAIT_TYPE WaitType, BOOLEAN Alertable, PLARGE_INTEGER TimeOut) {
		return syscall5<NTSTATUS>(NtWaitForMultipleObjects32ID, ObjectCount, ObjectsArray, WaitType, Alertable, TimeOut);
	}
}

__if_exists(NtQueryInformationThreadID) {
	// 0x0022
	inline NTSTATUS NtQueryInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength) {
		return syscall5<NTSTATUS>(NtQueryInformationThreadID, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength);
	}
}

__if_exists(NtTerminateProcessID) {
	// 0x0029
	inline NTSTATUS NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus) {
		return syscall2<NTSTATUS>(NtTerminateProcessID, ProcessHandle, ExitStatus);
	}
}

__if_exists(NtWriteVirtualMemoryID) {
	// 0x0037
	inline NTSTATUS NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten) {
		return syscall5<NTSTATUS>(NtWriteVirtualMemoryID, ProcessHandle, BaseAddress, Buffer, NumberOfBytesToWrite, NumberOfBytesWritten);
	}
}

__if_exists(NtAddAtomID) {
	// 0x0044
	inline NTSTATUS NtAddAtom(PWCHAR AtomName, PRTL_ATOM Atom) {
		return syscall2<NTSTATUS>(NtAddAtomID, AtomName, Atom);
	}
}

__if_exists(NtProtectVirtualMemoryID) {
	// 0x004D
	inline NTSTATUS NtProtectVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, PULONG NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection) {
		return syscall5<NTSTATUS>(NtProtectVirtualMemoryID, ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
	}
}

__if_exists(NtFlushInstructionCacheID) {
	// 0x00C2
	inline NTSTATUS NtFlushInstructionCache(HANDLE ProcessHandle, PVOID BaseAddress, ULONG NumberOfBytesToFlush) {
		return syscall3<NTSTATUS>(NtFlushInstructionCacheID, ProcessHandle, BaseAddress, NumberOfBytesToFlush);
	}
}

__if_exists(NtUserGetThreadStateID) {
	// 0x1000
	inline DWORD_PTR NtUserGetThreadState(DWORD Routine) {
		return syscall1<DWORD_PTR>(NtUserGetThreadStateID, Routine);
	}
}

__if_exists(NtUserPeekMessageID) {
	// 0x1001
	inline BOOL NtUserPeekMessage(HWND hWnd, UINT MsgFilterMin, UINT MsgFilterMax, UINT RemoveMsg) {
		return syscall4<BOOL>(NtUserPeekMessageID, hWnd, MsgFilterMin, MsgFilterMax, RemoveMsg);
	}
}

__if_exists(NtUserCallOneParamID) {
	// 0x1002
	inline DWORD_PTR NtUserCallOneParam(DWORD_PTR Param, DWORD Routine) {
		return syscall2<DWORD_PTR>(NtUserCallOneParamID, Param, Routine);
	}
}

__if_exists(NtUserGetKeyStateID) {
	// 0x1003
	inline SHORT NtUserGetKeyState(INT VirtKey) {
		return syscall1<SHORT>(NtUserGetKeyStateID, VirtKey);
	}
}

#endif