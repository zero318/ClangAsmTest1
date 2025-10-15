#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

#ifndef SYSCALLS_7_H
#define SYSCALLS_7_H 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>

#include "../zero/util.h"

#include "../windows_structs.h"

#if __x86_64__
#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")
#else
//#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll32.lib")
#endif

enum SyscallIndex : uint32_t {
	NtCallbackReturnID = 0x2,
	NtSetInformationThreadID = 0xA,
	NtCloseID = 0xC,
	NtOpenKeyID = 0xF,
	NtEnumerateValueKeyID = 0x10,
	NtQueryKeyID = 0x13,
	NtQueryValueKeyID = 0x14,
	NtAllocateVirtualMemoryID = 0x15,
	NtQueryInformationProcessID = 0x16,
	NtWaitForMultipleObjects32ID = 0x17,
	NtCreateKeyID = 0x1A,
	NtFreeVirtualMemoryID = 0x1B,
	NtQueryVirtualMemoryID = 0x20,
	NtQueryInformationThreadID = 0x22,
	NtAccessCheckAndAuditAlarmID = 0x26, // TODO
	NtTerminateProcessID = 0x29,
	NtEnumerateKeyID = 0x2F,
	NtQuerySystemInformationID = 0x33,
	NtWriteVirtualMemoryID = 0x37,
	NtContinueID = 0x40,
	NtAddAtomID = 0x44,
	NtProtectVirtualMemoryID = 0x4D,
	NtAccessCheckByTypeAndAuditAlarmID = 0x56, // TODO
	NtSetValueKeyID = 0x5D,
	NtAcceptConnectPortID = 0x60, // TODO
	NtAccessCheckID = 0x61, // TODO
	NtAccessCheckByTypeID = 0x62, // TODO
	NtAccessCheckByTypeResultListID = 0x63, // TODO
	NtAccessCheckByTypeResultListAndAuditAlarmID = 0x64, // TODO
	NtAccessCheckByTypeResultListAndAuditAlarmByHandleID = 0x65, // TODO

	NtCompactKeysID = 0x8B,
	NtCompressKeyID = 0x8E,
	NtCreateProcessID = 0x9F,
	NtDeleteKeyID = 0xB3,
	NtDeleteValueKeyID = 0xB6,
	NtFlushInstructionCacheID = 0xC2,
	NtFlushKeyID = 0xC3,
	NtLoadKeyID = 0xDD,
	NtLoadKey2ID = 0xDE,
	NtLoadKeyExID = 0xDF, // TODO
	NtLockVirtualMemoryID = 0xE3,
	NtOpenKeyExID = 0xF2,
	NtRenameKeyID = 0x13B,
	NtReplaceKeyID = 0x13D,
	NtRestoreKeyID = 0x143,
	NtSaveKeyID = 0x149,
	NtSaveKeyExID = 0x14A,
	NtSaveMergedKeysID = 0x14B,
	NtSetInformationKeyID = 0x15C,
	NtUnloadKeyID = 0x185,
	NtUnloadKey2ID = 0x186,
	NtUnloadKeyExID = 0x187,
	NtUnlockVirtualMemoryID = 0x189,

	NtUserGetThreadStateID = 0x1000,
	NtUserPeekMessageID = 0x1001,
	NtUserCallOneParamID = 0x1002,
	NtUserGetKeyStateID = 0x1003,

	NtWow64CsrBasepCreateProcessID = 0x3003,
};

template<typename R = uint64_t>
static R syscall32() {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
	);
}

template<typename R = uint64_t>
static R syscall0(SyscallIndex index) {
	R ret;
	__asm__ volatile(
		"syscall"
		: "=a"(ret)
		: "a"(index)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1>
static R syscall1(SyscallIndex index, A1 arg1) {
	register A1 temp1 asm("r10") = arg1;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2>
static R syscall2(SyscallIndex index, A1 arg1, A2 arg2) {
	register A1 temp1 asm("r10") = arg1;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3>
static R syscall3(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3, typename A4>
static R syscall4(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3, A4 arg4) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	register A4 temp4 asm("r9") = arg4;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3), "r"(temp4)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3, typename A4, typename A5>
static R syscall5(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	register A4 temp4 asm("r9") = arg4;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3), "r"(temp4), "m"(arg5)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
static R syscall6(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5, A6 arg6) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	register A4 temp4 asm("r9") = arg4;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3), "r"(temp4), "m"(arg5), "m"(arg6)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
static R syscall7(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5, A6 arg6, A7 arg7) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	register A4 temp4 asm("r9") = arg4;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3), "r"(temp4), "m"(arg5), "m"(arg6), "m"(arg7)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
static R syscall8(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5, A6 arg6, A7 arg7, A8 arg8) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	register A4 temp4 asm("r9") = arg4;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3), "r"(temp4), "m"(arg5), "m"(arg6), "m"(arg7), "m"(arg8)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
static R syscall9(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5, A6 arg6, A7 arg7, A8 arg8, A9 arg9) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	register A4 temp4 asm("r9") = arg4;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3), "r"(temp4), "m"(arg5), "m"(arg6), "m"(arg7), "m"(arg8), "m"(arg9)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
static R syscall10(SyscallIndex index, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5, A6 arg6, A7 arg7, A8 arg8, A9 arg9, A10 arg10) {
	register A1 temp1 asm("r10") = arg1;
	register A3 temp3 asm("r8") = arg3;
	register A4 temp4 asm("r9") = arg4;
	R ret;
	__asm__ volatile (
		"syscall"
		: "=a"(ret)
		: "a"(index), "r"(temp1), "d"(arg2), "r"(temp3), "r"(temp4), "m"(arg5), "m"(arg6), "m"(arg7), "m"(arg8), "m"(arg9), "m"(arg10)
		: clobber_list("rcx", "r11")
	);
	__asm__ volatile ("":::clobber_list("r10"));
	return ret;
}
template<typename R = uint64_t, typename ... Args>
static inline R syscall(SyscallIndex index, Args... args) {
	if constexpr (sizeof...(Args) == 0) {
		return syscall0<R>(index);
	} else if constexpr (sizeof...(Args) == 1) {
		return syscall1<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 2) {
		return syscall2<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 3) {
		return syscall3<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 4) {
		return syscall4<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 5) {
		return syscall5<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 6) {
		return syscall6<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 7) {
		return syscall7<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 8) {
		return syscall8<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 9) {
		return syscall9<R>(index, args...);
	} else if constexpr (sizeof...(Args) == 10) {
		return syscall10<R>(index, args...);
	}
}

template<SyscallIndex index, uint32_t wow_table, typename R = uint32_t>
naked static gnu_noinline R stdcall syscallWoW() {
	__asm__(
		"CALLL *%%FS:0xC0 \n"
		"ADDL $4, %%ESP \n"
		"RETL \n"
		:
		: "a"(index), "c"(wow_table), "d"(esp_reg + 4)
	);
	/*
	__asm__(
		"PUSHL $1f \n"
		"JMPL *%%FS:0xC0 \n"
		"1: \n"
		"ADDL $4, %%ESP \n"
		"RETL \n"
		:
		: "a"(index), "c"(wow_table), "d"(esp_reg + 4)
	);
	*/
}

template<SyscallIndex index, uint32_t wow_table, typename R = uint32_t, typename ... Args>
naked static gnu_noinline R stdcall syscallWoW(gnu_used Args... args) {
	__asm__(
		"CALLL *%%FS:0xC0 \n"
		"ADDL $4, %%ESP \n"
		"RETL %[ret_size] \n"
		:
		: "a"(index), "c"(wow_table), "d"(esp_reg + 4), [ret_size]"i"(sizeof_pack<Args...>())
	);
	/*
	__asm__(
		"PUSHL $1f \n"
		"JMPL *%%FS:0xC0 \n"
	"1: \n"
		"ADDL $4, %%ESP \n"
		"RETL %[ret_size] \n"
		:
		: "a"(index), "c"(wow_table), "d"(esp_reg + 4), [ret_size]"i"(sizeof_pack<Args...>())
	);
	*/
}

#include "common.h"

#endif