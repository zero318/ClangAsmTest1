#ifndef SYSCALLS_COMMON_H
#define SYSCALLS_COMMON_H 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>

#include "../zero/util.h"

#include "../windows_structs.h"

#define INLINE_SYSCALLS 1
#define INLINE_SHORT_SYSCALLS 1
#define INLINE_LONG_SYSCALLS 1

#if !INLINE_SYSCALLS
#undef INLINE_SHORT_SYSCALLS
#undef INLINE_LONG_SYSCALLS
#endif

//extern "C" {

#if INLINE_SHORT_SYSCALLS
__if_exists(NtCallbackReturnID) {
	// 0x0002
	inline NTSTATUS NtCallbackReturn(PVOID Result, ULONG ResultLength, NTSTATUS Status) {
		return syscall<NTSTATUS>(NtCallbackReturnID, Result, ResultLength, Status);
	}
}
__if_not_exists(NtCallbackReturnID) {
#endif
	dll_import NTSTATUS NtCallbackReturn(PVOID Result, ULONG ResultLength, NTSTATUS Status);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtSetInformationThreadID) {
	// 0x000A
	inline NTSTATUS NtSetInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength) {
		return syscall<NTSTATUS>(NtSetInformationThreadID, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
	}
}
__if_not_exists(NtSetInformationThreadID) {
#endif
	dll_import NTSTATUS NtSetInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtCloseID) {
	// 0x000C
	inline NTSTATUS NtClose(HANDLE Handle) {
		return syscall<NTSTATUS>(NtCloseID, Handle);
	}
}
__if_not_exists(NtCloseID) {
#endif
	dll_import NTSTATUS NtClose(HANDLE Handle);
#if INLINE_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtOpenKeyID) {
	// 0x000F
	inline NTSTATUS NtOpenKey(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes) {
		return syscall<NTSTATUS>(NtOpenKeyID, KeyHandle, DesiredAccess, ObjectAttributes);
	}
}
__if_not_exists(NtOpenKeyID) {
#endif
	dll_import NTSTATUS NtOpenKey(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtEnumerateValueKeyID) {
	// 0x0010
	inline NTSTATUS NtEnumerateValueKey(HANDLE KeyHandle, ULONG Index, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, PVOID KeyValueInformation, ULONG Length, PULONG ReturnLength) {
		return syscall<NTSTATUS>(NtEnumerateValueKeyID, KeyHandle, Index, KeyValueInformationClass, KeyValueInformation, Length, ReturnLength);
	}
}
__if_not_exists(NtEnumerateValueKeyID) {
#endif
	dll_import NTSTATUS NtEnumerateValueKey(HANDLE KeyHandle, ULONG Index, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, PVOID KeyValueInformation, ULONG Length, PULONG ReturnLength);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtQueryKeyID) {
	// 0x0014
	inline NTSTATUS NtQueryKey(HANDLE KeyHandle, KEY_INFORMATION_CLASS KeyInformationClass, PVOID KeyInformation, ULONG Length, PULONG ReturnLength) {
		return syscall<NTSTATUS>(NtQueryKeyID, KeyHandle, KeyInformationClass, KeyInformation, Length, ReturnLength);
	}
}
__if_not_exists(NtQueryKeyID) {
#endif
	dll_import NTSTATUS NtQueryKey(HANDLE KeyHandle, KEY_INFORMATION_CLASS KeyInformationClass, PVOID KeyInformation, ULONG Length, PULONG ReturnLength);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtQueryValueKeyID) {
	// 0x0014
	inline NTSTATUS NtQueryValueKey(HANDLE KeyHandle, PUNICODE_STRING ValueName, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, PVOID KeyValueInformation, ULONG Length, PULONG ReturnLength) {
		return syscall<NTSTATUS>(NtQueryValueKeyID, KeyHandle, ValueName, KeyValueInformationClass, KeyValueInformation, Length, ReturnLength);
	}
}
__if_not_exists(NtQueryValueKeyID) {
#endif
	dll_import NTSTATUS NtQueryValueKey(HANDLE KeyHandle, PUNICODE_STRING ValueName, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, PVOID KeyValueInformation, ULONG Length, PULONG ReturnLength);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtAllocateVirtualMemoryID) {
	// 0x0015
	inline NTSTATUS NtAllocateVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect) {
		return syscall<NTSTATUS>(NtAllocateVirtualMemoryID, ProcessHandle, BaseAddress, ZeroBits, RegionSize, AllocationType, Protect);
	}
}
__if_not_exists(NtAllocateVirtualMemoryID) {
#endif
	dll_import NTSTATUS NtAllocateVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtQueryInformationProcessID) {
	// 0x0016
	inline NTSTATUS NtQueryInformationProcess(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength) {
		return syscall<NTSTATUS>(NtQueryInformationProcessID, ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
	}
}
__if_not_exists(NtQueryInformationProcessID) {
#endif
	dll_import NTSTATUS NtQueryInformationProcess(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtWaitForMultipleObjects32ID) {
	// 0x0017
	inline NTSTATUS NtWaitForMultipleObjects32(ULONG ObjectCount, PHANDLE ObjectsArray, OBJECT_WAIT_TYPE WaitType, BOOLEAN Alertable, PLARGE_INTEGER TimeOut) {
		return syscall<NTSTATUS>(NtWaitForMultipleObjects32ID, ObjectCount, ObjectsArray, WaitType, Alertable, TimeOut);
	}
}
__if_not_exists(NtWaitForMultipleObjects32ID) {
#endif
	dll_import NTSTATUS NtWaitForMultipleObjects32(ULONG ObjectCount, PHANDLE ObjectsArray, OBJECT_WAIT_TYPE WaitType, BOOLEAN Alertable, PLARGE_INTEGER TimeOut);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtCreateKeyID) {
	// 0x001A
	inline NTSTATUS NtCreateKey(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG TitleIndex, PUNICODE_STRING Class, ULONG CreateOptions, PULONG Disposition) {
		return syscall<NTSTATUS>(NtCreateKeyID, KeyHandle, DesiredAccess, ObjectAttributes, TitleIndex, Class, CreateOptions, Disposition);
	}
}
__if_not_exists(NtCreateKeyID) {
#endif
	dll_import NTSTATUS NtCreateKey(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG TitleIndex, PUNICODE_STRING Class, ULONG CreateOptions, PULONG Disposition);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtFreeVirtualMemoryID) {
	// 0x001B
	inline NTSTATUS NtFreeVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, PSIZE_T RegionSize, ULONG FreeType) {
		return syscall<NTSTATUS>(NtFreeVirtualMemoryID, ProcessHandle, BaseAddress, RegionSize, FreeType);
	}
}
__if_not_exists(NtFreeVirtualMemoryID) {
#endif
	dll_import NTSTATUS NtFreeVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, PSIZE_T RegionSize, ULONG FreeType);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtQueryVirtualMemoryID) {
	// 0x0020
	inline NTSTATUS NtQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength) {
		return syscall<NTSTATUS>(NtQueryVirtualMemoryID, ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
	}
}
__if_not_exists(NtQueryVirtualMemoryID) {
#endif
	dll_import NTSTATUS NtQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtQueryInformationThreadID) {
	// 0x0022
	inline NTSTATUS NtQueryInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength) {
		return syscall<NTSTATUS>(NtQueryInformationThreadID, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength);
	}
}
__if_not_exists(NtQueryInformationThreadID) {
#endif
	dll_import NTSTATUS NtQueryInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtTerminateProcessID) {
	// 0x0029
	inline NTSTATUS NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus) {
		return syscall<NTSTATUS>(NtTerminateProcessID, ProcessHandle, ExitStatus);
	}
}
__if_not_exists(NtTerminateProcessID) {
#endif
	dll_import NTSTATUS NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtEnumerateKeyID) {
	// 0x002F
	inline NTSTATUS NtEnumerateKey(HANDLE KeyHandle, ULONG Index, KEY_INFORMATION_CLASS KeyInformationClass, PVOID KeyInformation, ULONG Length, PULONG ReturnLength) {
		return syscall<NTSTATUS>(NtEnumerateKeyID, KeyHandle, Index, KeyInformationClass, KeyInformation, Length, ReturnLength);
	}
}
__if_not_exists(NtEnumerateKeyID) {
#endif
	dll_import NTSTATUS NtEnumerateKey(HANDLE KeyHandle, ULONG Index, KEY_INFORMATION_CLASS KeyInformationClass, PVOID KeyInformation, ULONG Length, PULONG ReturnLength);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtQuerySystemInformationID) {
	// 0x0033
	inline NTSTATUS NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength) {
		return syscall<NTSTATUS>(NtQuerySystemInformationID, SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	}
}
__if_not_exists(NtQuerySystemInformationID) {
#endif
	dll_import NTSTATUS NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtWriteVirtualMemoryID) {
	// 0x0037
	inline NTSTATUS NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten) {
		return syscall<NTSTATUS>(NtWriteVirtualMemoryID, ProcessHandle, BaseAddress, Buffer, NumberOfBytesToWrite, NumberOfBytesWritten);
	}
}
__if_not_exists(NtWriteVirtualMemoryID) {
#endif
	dll_import NTSTATUS NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtContinueID) {
	// 0x0040
	inline NTSTATUS NtContinue(PCONTEXT ThreadContext, BOOLEAN RaiseAlert) {
		return syscall<NTSTATUS>(NtContinueID, ThreadContext, RaiseAlert);
	}
}
__if_not_exists(NtContinueID) {
#endif
	dll_import NTSTATUS NtContinue(PCONTEXT ThreadContext, BOOLEAN RaiseAlert);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtAddAtomID) {
	// 0x0044
	inline NTSTATUS NtAddAtom(PWCHAR AtomName, PRTL_ATOM Atom) {
		return syscall<NTSTATUS>(NtAddAtomID, AtomName, Atom);
	}
}
__if_not_exists(NtAddAtomID) {
#endif
	dll_import NTSTATUS NtAddAtom(PWCHAR AtomName, PRTL_ATOM Atom);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtProtectVirtualMemoryID) {
	// 0x004D
	inline NTSTATUS NtProtectVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, PULONG NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection) {
		return syscall<NTSTATUS>(NtProtectVirtualMemoryID, ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
	}
}
__if_not_exists(NtProtectVirtualMemoryID) {
#endif
	dll_import NTSTATUS NtProtectVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, PULONG NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtSetValueKeyID) {
	// 0x005D
	inline NTSTATUS NtSetValueKey(HANDLE KeyHandle, PUNICODE_STRING ValueName, ULONG TitleIndex, ULONG Type, PVOID Data, ULONG DataSize) {
		return syscall<NTSTATUS>(NtSetValueKeyID, KeyHandle, ValueName, TitleIndex, Type, Data, DataSize);
	}
}
__if_not_exists(NtSetValueKeyID) {
#endif
	dll_import NTSTATUS NtSetValueKey(HANDLE KeyHandle, PUNICODE_STRING ValueName, ULONG TitleIndex, ULONG Type, PVOID Data, ULONG DataSize);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtCompactKeysID) {
	// 0x008B
	inline NTSTATUS NtCompactKeys(ULONG NumberOfKeys, HANDLE KeysArray[]) {
		return syscall<NTSTATUS>(NtCompactKeysID, NumberOfKeys, KeysArray);
	}
}
__if_not_exists(NtCompactKeysID) {
#endif
	dll_import NTSTATUS NtCompactKeys(ULONG NumberOfKeys, HANDLE KeysArray[]);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtCompressKeyID) {
	// 0x008E
	inline NTSTATUS NtCompressKey(HANDLE KeyHandle) {
		return syscall<NTSTATUS>(NtCompressKeyID, KeyHandle);
	}
}
__if_not_exists(NtCompressKeyID) {
#endif
	dll_import NTSTATUS NtCompressKey(HANDLE KeyHandle);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_LONG_SYSCALLS
__if_exists(NtCreateProcessID) {
	// 0x009F
	inline NTSTATUS NtCreateProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ParentProcess, BOOLEAN InheritObjectTable, HANDLE SectionHandle, HANDLE DebugPort, HANDLE ExceptionPort) {
		return syscall<NTSTATUS>(NtCreateProcessID, ProcessHandle, DesiredAccess, ObjectAttributes, ParentProcess, InheritObjectTable, SectionHandle, DebugPort, ExceptionPort);
	}
}
__if_not_exists(NtCreateProcessID) {
#endif
	dll_import NTSTATUS NtCreateProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ParentProcess, BOOLEAN InheritObjectTable, HANDLE SectionHandle, HANDLE DebugPort, HANDLE ExceptionPort);
#if INLINE_LONG_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtDeleteKeyID) {
	// 0x00B3
	inline NTSTATUS NtDeleteKey(HANDLE KeyHandle) {
		return syscall<NTSTATUS>(NtDeleteKeyID, KeyHandle);
	}
}
__if_not_exists(NtDeleteKeyID) {
#endif
	dll_import NTSTATUS NtDeleteKey(HANDLE KeyHandle);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtDeleteValueKeyID) {
	// 0x00B6
	inline NTSTATUS NtDeleteValueKey(HANDLE KeyHandle, PUNICODE_STRING ValueName) {
		return syscall<NTSTATUS>(NtDeleteValueKeyID, KeyHandle, ValueName);
	}
}
__if_not_exists(NtDeleteValueKeyID) {
#endif
	dll_import NTSTATUS NtDeleteValueKey(HANDLE KeyHandle, PUNICODE_STRING ValueName);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtFlushInstructionCacheID) {
	// 0x00C2
	inline NTSTATUS NtFlushInstructionCache(HANDLE ProcessHandle, PVOID BaseAddress, ULONG NumberOfBytesToFlush) {
		return syscall<NTSTATUS>(NtFlushInstructionCacheID, ProcessHandle, BaseAddress, NumberOfBytesToFlush);
	}
}
__if_not_exists(NtFlushInstructionCacheID) {
#endif
	dll_import NTSTATUS NtFlushInstructionCache(HANDLE ProcessHandle, PVOID BaseAddress, ULONG NumberOfBytesToFlush);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtFlushKeyID) {
	// 0x00C3
	inline NTSTATUS NtFlushKey(HANDLE KeyHandle) {
		return syscall<NTSTATUS>(NtFlushKeyID, KeyHandle);
	}
}
__if_not_exists(NtFlushKeyID) {
#endif
	dll_import NTSTATUS NtFlushKey(HANDLE KeyHandle);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtLoadKeyID) {
	// 0x00DD
	inline NTSTATUS NtLoadKey(POBJECT_ATTRIBUTES DestinationKeyName, POBJECT_ATTRIBUTES HiveFileName) {
		return syscall<NTSTATUS>(NtLoadKeyID, DestinationKeyName, HiveFileName);
	}
}
__if_not_exists(NtLoadKeyID) {
#endif
	dll_import NTSTATUS NtLoadKey(POBJECT_ATTRIBUTES DestinationKeyName, POBJECT_ATTRIBUTES HiveFileName);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtLoadKey2ID) {
	// 0x00DE
	inline NTSTATUS NtLoadKey2(POBJECT_ATTRIBUTES DestinationKeyName, POBJECT_ATTRIBUTES HiveFileName, ULONG Flags) {
		return syscall<NTSTATUS>(NtLoadKey2ID, DestinationKeyName, HiveFileName, Flags);
	}
}
__if_not_exists(NtLoadKeyID) {
#endif
	dll_import NTSTATUS NtLoadKey2(POBJECT_ATTRIBUTES DestinationKeyName, POBJECT_ATTRIBUTES HiveFileName, ULONG Flags);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtLockVirtualMemoryID) {
	// 0x00E3
	inline NTSTATUS NtLockVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, PSIZE_T NumberOfBytesToLock, ULONG LockOption) {
		return syscall<NTSTATUS>(NtLockVirtualMemoryID, ProcessHandle, BaseAddress, NumberOfBytesToLock, LockOption);
	}
}
__if_not_exists(NtLockVirtualMemoryID) {
#endif
	dll_import NTSTATUS NtLockVirtualMemory(HANDLE ProcessHandle, PVOID * BaseAddress, PSIZE_T NumberOfBytesToLock, ULONG LockOption);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtOpenKeyExID) {
	// 0x00F2
	inline NTSTATUS NtOpenKeyEx(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG OpenOptions) {
		return syscall<NTSTATUS>(NtOpenKeyExID, KeyHandle, DesiredAccess, ObjectAttributes, OpenOptions);
	}
}
__if_not_exists(NtOpenKeyExID) {
#endif
	dll_import NTSTATUS NtOpenKeyEx(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG OpenOptions);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtRenameKeyID) {
	// 0x013D
	inline NTSTATUS NtRenameKey(HANDLE KeyHandle, PUNICODE_STRING NewName) {
		return syscall<NTSTATUS>(NtRenameKeyID, KeyHandle, NewName);
	}
}
__if_not_exists(NtRenameKeyID) {
#endif
	dll_import NTSTATUS NtRenameKey(HANDLE KeyHandle, PUNICODE_STRING NewName);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtReplaceKeyID) {
	// 0x013D
	inline NTSTATUS NtReplaceKey(POBJECT_ATTRIBUTES NewHiveFileName, HANDLE KeyHandle, POBJECT_ATTRIBUTES BackupHiveFileName) {
		return syscall<NTSTATUS>(NtReplaceKeyID, NewHiveFileName, KeyHandle, BackupHiveFileName);
	}
}
__if_not_exists(NtReplaceKeyID) {
#endif
	dll_import NTSTATUS NtReplaceKey(POBJECT_ATTRIBUTES NewHiveFileName, HANDLE KeyHandle, POBJECT_ATTRIBUTES BackupHiveFileName);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtRestoreKeyID) {
	// 0x0143
	inline NTSTATUS NtRestoreKey(HANDLE KeyHandle, HANDLE FileHandle, ULONG RestoreOption) {
		return syscall<NTSTATUS>(NtRestoreKeyID, KeyHandle, FileHandle, RestoreOption);
	}
}
__if_not_exists(NtRestoreKeyID) {
#endif
	dll_import NTSTATUS NtRestoreKey(HANDLE KeyHandle, HANDLE FileHandle, ULONG RestoreOption);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtSaveKeyID) {
	// 0x0149
	inline NTSTATUS NtSaveKey(HANDLE KeyHandle, HANDLE FileHandle) {
		return syscall<NTSTATUS>(NtSaveKeyID, KeyHandle, FileHandle);
	}
}
__if_not_exists(NtSaveKeyID) {
#endif
	dll_import NTSTATUS NtSaveKey(HANDLE KeyHandle, HANDLE FileHandle);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtSaveKeyExID) {
	// 0x014A
	inline NTSTATUS NtSaveKeyEx(HANDLE KeyHandle, HANDLE FileHandle, ULONG SaveOptions) {
		return syscall<NTSTATUS>(NtSaveKeyExID, KeyHandle, FileHandle, SaveOptions);
	}
}
__if_not_exists(NtSaveKeyExID) {
#endif
	dll_import NTSTATUS NtSaveKeyEx(HANDLE KeyHandle, HANDLE FileHandle, ULONG SaveOptions);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtSaveMergedKeysID) {
	// 0x014B
	inline NTSTATUS NtSaveMergedKeys(HANDLE KeyHandle1, HANDLE KeyHandle2, HANDLE FileHandle) {
		return syscall<NTSTATUS>(NtSaveMergedKeysID, KeyHandle1, KeyHandle2, FileHandle);
	}
}
__if_not_exists(NtSaveMergedKeysID) {
#endif
	dll_import NTSTATUS NtSaveMergedKeys(HANDLE KeyHandle1, HANDLE KeyHandle2, HANDLE FileHandle);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtSetInformationKeyID) {
	// 0x015C
	inline NTSTATUS NtSetInformationKey(HANDLE KeyHandle, KEY_SET_INFORMATION_CLASS KeySetInformationClass, PVOID KeySetInformation, ULONG KeySetInformationLength) {
		return syscall<NTSTATUS>(NtSetInformationKeyID, KeyHandle, KeySetInformationClass, KeySetInformation, KeySetInformationLength);
	}
}
__if_not_exists(NtSetInformationKeyID) {
#endif
	dll_import NTSTATUS NtSetInformationKey(HANDLE KeyHandle, KEY_SET_INFORMATION_CLASS KeySetInformationClass, PVOID KeySetInformation, ULONG KeySetInformationLength);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUnloadKeyID) {
	// 0x0185
	inline NTSTATUS NtUnloadKey(POBJECT_ATTRIBUTES DestinationKeyName) {
		return syscall<NTSTATUS>(NtUnloadKeyID, DestinationKeyName);
	}
}
__if_not_exists(NtUnloadKeyID) {
#endif
	dll_import NTSTATUS NtUnloadKey(POBJECT_ATTRIBUTES DestinationKeyName);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUnloadKey2ID) {
	// 0x0186
	inline NTSTATUS NtUnloadKey2(POBJECT_ATTRIBUTES DestinationKeyName, ULONG Flags) {
		return syscall<NTSTATUS>(NtUnloadKey2ID, DestinationKeyName, Flags);
	}
}
__if_not_exists(NtUnloadKey2ID) {
#endif
	dll_import NTSTATUS NtUnloadKey2(POBJECT_ATTRIBUTES DestinationKeyName, ULONG Flags);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUnloadKeyExID) {
	// 0x0187
	inline NTSTATUS NtUnloadKeyEx(POBJECT_ATTRIBUTES DestinationKeyName, HANDLE Event) {
		return syscall<NTSTATUS>(NtUnloadKeyExID, DestinationKeyName, Event);
	}
}
__if_not_exists(NtUnloadKeyExID) {
#endif
	dll_import NTSTATUS NtUnloadKeyEx(POBJECT_ATTRIBUTES DestinationKeyName, HANDLE Event);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUnlockVirtualMemoryID) {
	// 0x0189
	inline NTSTATUS NtUnlockVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, PSIZE_T NumberOfBytesToUnlock, ULONG LockType) {
		return syscall<NTSTATUS>(NtUnlockVirtualMemoryID, ProcessHandle, BaseAddress, NumberOfBytesToUnlock, LockType);
	}
}
__if_not_exists(NtUnlockVirtualMemoryID) {
#endif
	dll_import NTSTATUS NtUnlockVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, PSIZE_T NumberOfBytesToUnlock, ULONG LockType);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUserGetThreadStateID) {
	// 0x1000
	inline DWORD_PTR NtUserGetThreadState(DWORD Routine) {
		return syscall<DWORD_PTR>(NtUserGetThreadStateID, Routine);
	}
}
__if_not_exists(NtUserGetThreadStateID) {
#endif
	dll_import DWORD_PTR NtUserGetThreadState(DWORD Routine);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUserPeekMessageID) {
	// 0x1001
	inline BOOL NtUserPeekMessage(HWND hWnd, UINT MsgFilterMin, UINT MsgFilterMax, UINT RemoveMsg) {
		return syscall<BOOL>(NtUserPeekMessageID, hWnd, MsgFilterMin, MsgFilterMax, RemoveMsg);
	}
}
__if_not_exists(NtUserPeekMessageID) {
#endif
	dll_import BOOL NtUserPeekMessage(HWND hWnd, UINT MsgFilterMin, UINT MsgFilterMax, UINT RemoveMsg);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUserCallOneParamID) {
	// 0x1002
	inline DWORD_PTR NtUserCallOneParam(DWORD_PTR Param, DWORD Routine) {
		return syscall<DWORD_PTR>(NtUserCallOneParamID, Param, Routine);
	}
}
__if_not_exists(NtUserCallOneParamID) {
#endif
	dll_import DWORD_PTR NtUserCallOneParam(DWORD_PTR Param, DWORD Routine);
#if INLINE_SHORT_SYSCALLS
}
#endif

#if INLINE_SHORT_SYSCALLS
__if_exists(NtUserGetKeyStateID) {
	// 0x1003
	inline SHORT NtUserGetKeyState(INT VirtKey) {
		return syscall<SHORT>(NtUserGetKeyStateID, VirtKey);
	}
}
__if_not_exists(NtUserGetKeyStateID) {
#endif
	dll_import SHORT NtUserGetKeyState(INT VirtKey);
#if INLINE_SHORT_SYSCALLS
}
#endif

//}

#endif