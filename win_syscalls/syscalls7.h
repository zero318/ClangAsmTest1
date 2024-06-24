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

#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")

enum SyscallIndex : uint32_t {
	NtSetInformationThreadID = 0x0A,
	NtWaitForMultipleObjects32ID = 0x17,
	NtQueryInformationThreadID = 0x22,
	NtAccessCheckAndAuditAlarmID = 0x26, // TODO
	NtTerminateProcessID = 0x29,
	NtWriteVirtualMemoryID = 0x37,
	NtAddAtomID = 0x44,
	NtProtectVirtualMemoryID = 0x4D,
	NtAccessCheckByTypeAndAuditAlarmID = 0x56, // TODO
	NtAcceptConnectPortID = 0x60, // TODO
	NtAccessCheckID = 0x61, // TODO
	NtAccessCheckByTypeID = 0x62, // TODO
	NtAccessCheckByTypeResultListID = 0x63, // TODO
	NtAccessCheckByTypeResultListAndAuditAlarmID = 0x64, // TODO
	NtAccessCheckByTypeResultListAndAuditAlarmByHandleID = 0x65, // TODO

	NtFlushInstructionCacheID = 0xC2,

	NtUserGetThreadStateID = 0x1000,
	NtUserPeekMessageID = 0x1001,
	NtUserCallOneParamID = 0x1002,
	NtUserGetKeyStateID = 0x1003,
};

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

#include "common.h"

#endif