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

#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")

enum SyscallIndex : uint32_t {
	NtSetInformationThreadID = 0x0A,
	NtQueryInformationThreadID = 0x22,
	NtTerminateProcessID = 0x29,
	NtWriteVirtualMemoryID = 0x37,
	NtProtectVirtualMemoryID = 0x4D,
	NtFlushInstructionCacheID = 0xC2
};

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

inline NTSTATUS NtSetInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength) {
	return syscall4<NTSTATUS>(NtSetInformationThreadID, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
}

inline NTSTATUS NtQueryInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength) {
	return syscall5<NTSTATUS>(NtQueryInformationThreadID, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength);
}

inline NTSTATUS NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus) {
	return syscall2<NTSTATUS>(NtTerminateProcessID, ProcessHandle, ExitStatus);
}

inline NTSTATUS NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten) {
	return syscall5<NTSTATUS>(NtWriteVirtualMemoryID, ProcessHandle, BaseAddress, Buffer, NumberOfBytesToWrite, NumberOfBytesWritten);
}

inline NTSTATUS NtProtectVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, PULONG NumberOfBytesToProtect, ULONG NewAccessProtection, ULONG OldAccessProtection) {
	return syscall5<NTSTATUS>(NtProtectVirtualMemoryID, ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
}

inline NTSTATUS NtFlushInstructionCache(HANDLE ProcessHandle, PVOID BaseAddress, ULONG NumberOfBytesToFlush) {
	return syscall3<NTSTATUS>(NtFlushInstructionCacheID, ProcessHandle, BaseAddress, NumberOfBytesToFlush);
}

struct UnknownA {
	int __dword_0;
	CONTEXTX<32> context32;
	int __dword_2D0;
};

template<typename T>
bool has_bits_set(T value, T bits) {
	return (value & bits) == bits;
}

NTSTATUS RtlpWow64GetContextOnAmd64(const CONTEXTX<32>* context_in, CONTEXTX<32>* context_out) {

	uint32_t requested_data = context_out->ContextFlags;

	if (expect(
		!(requested_data & CONTEXTX_32) ||
		//(requested_data & ~(CONTEXTX_CONTROL | CONTEXTX_INTEGER | CONTEXTX_SEGMENTS | CONTEXTX_FLOATING_POINT | CONTEXTX_DEBUG_REGISTERS | CONTEXTX_EXTENDED_REGISTERS | CONTEXTX_XSTATE)) ||
		//(requested_data & CONTEXTX_64)
		(requested_data & ~(CONTEXTX_32 | CONTEXTX_CONTROL | CONTEXTX_INTEGER | CONTEXTX_SEGMENTS))
	, false)) {
		return STATUS_INVALID_PARAMETER;
	}

	if (expect(requested_data & CONTEXTX_SEGMENTS, true)) {
		context_out->SegGs = context_in->SegGs;
		context_out->SegFs = 0x53; // context_in->SegFs;
		__builtin_memcpy(&context_out->SegEs, &context_in->SegEs, sizeof(uint32_t[2]));
		//context_out->SegEs = context_in->SegEs;
		//context_out->SegDs = context_in->SegDs;
	}

	if (expect(requested_data & CONTEXTX_INTEGER, true)) {
		__builtin_memcpy(&context_out->Edi, &context_in->Edi, sizeof(uint32_t[6]));
		//context_out->Edi = context_in->Edi;
		//context_out->Esi = context_in->Esi;
		//context_out->Ebx = context_in->Ebx;
		//context_out->Edx = context_in->Edx;
		//context_out->Ecx = context_in->Ecx;
		//context_out->Eax = context_in->Eax;
	}

	if (expect(requested_data & CONTEXTX_CONTROL, true)) {
		__builtin_memcpy(&context_out->Ebp, &context_in->Ebp, sizeof(uint32_t[2]));
		//context_out->Ebp = context_in->Ebp;
		//context_out->Eip = context_in->Eip;
		context_out->SegCs = 0x23; // context_in->SegCs;
		context_out->EFlags = (context_in->EFlags & 0x3F0DD5) | 0x202; // context_in->EFlags;
		context_out->Esp = context_in->Esp;
		context_out->SegSs = 0x2B; // context_in->SegSs;
	}

	/*
	if (expect(requested_data & CONTEXTX_EXTENDED_REGISTERS, true)) {
		context_out->ExtendedRegisters.XmmRegisters[0] = context_in->ExtendedRegisters.XmmRegisters[0];
		context_out->ExtendedRegisters.XmmRegisters[1] = context_in->ExtendedRegisters.XmmRegisters[1];
		context_out->ExtendedRegisters.XmmRegisters[2] = context_in->ExtendedRegisters.XmmRegisters[2];
		context_out->ExtendedRegisters.XmmRegisters[3] = context_in->ExtendedRegisters.XmmRegisters[3];
		context_out->ExtendedRegisters.XmmRegisters[4] = context_in->ExtendedRegisters.XmmRegisters[4];
		context_out->ExtendedRegisters.XmmRegisters[5] = context_in->ExtendedRegisters.XmmRegisters[5];
	}
	*/

	return 0;
}

NTSTATUS RtlpWow64SetContextOnAmd64(CONTEXTX<32>* context_out, int, const CONTEXTX<32>* context_in, uint8_t* wtf) {
	
	uint32_t incoming_data = context_in->ContextFlags;

	context_out->ContextFlags = incoming_data;
		
	if (expect(has_bits_set(incoming_data, CONTEXTX_32 | CONTEXTX_INTEGER), true)) {
		__builtin_memcpy(&context_out->Edi, &context_in->Edi, sizeof(uint32_t[6]));
		//context_out->Edi = context_in->Edi;
		//context_out->Esi = context_in->Esi;
		//context_out->Ebx = context_in->Ebx;
		//context_out->Edx = context_in->Edx;
		//context_out->Ecx = context_in->Ecx;
		//context_out->Eax = context_in->Eax;
	}

	if (expect(has_bits_set(incoming_data, CONTEXTX_32 | CONTEXTX_CONTROL), true)) {
		__builtin_memcpy(&context_out->Ebp, &context_in->Ebp, sizeof(uint32_t[2]));
		//context_out->Ebp = context_in->Ebp;
		//context_out->Eip = context_in->Eip;
		context_out->SegCs = 0x23; // context_in->SegCs;
		context_out->EFlags = (context_in->EFlags & 0x3F0DD5) | 0x202; // context_in->EFlags;
		context_out->Esp = context_in->Esp;
		context_out->SegSs = 0x2B; // context_in->SegSs;
	}

	/*
	if (expect(has_bits_set(requested_data, CONTEXTX_32 | CONTEXTX_EXTENDED_REGISTERS), true)) {
		context_out->ExtendedRegisters.XmmRegisters[0] = context_in->ExtendedRegisters.XmmRegisters[0];
		context_out->ExtendedRegisters.XmmRegisters[1] = context_in->ExtendedRegisters.XmmRegisters[1];
		context_out->ExtendedRegisters.XmmRegisters[2] = context_in->ExtendedRegisters.XmmRegisters[2];
		context_out->ExtendedRegisters.XmmRegisters[3] = context_in->ExtendedRegisters.XmmRegisters[3];
		context_out->ExtendedRegisters.XmmRegisters[4] = context_in->ExtendedRegisters.XmmRegisters[4];
		context_out->ExtendedRegisters.XmmRegisters[5] = context_in->ExtendedRegisters.XmmRegisters[5];
	}
	*/
	return 0;
}

#define TEB_UNKNOWNA (*(UnknownA* GS_RELATIVE*)&teb->TlsSlots[1])

static constexpr auto kwjrbkrb = sizeof(UnknownA);

extern "C" {

extern NTSYSAPI NTSTATUS LdrDisableThreadCalloutsForDll(PVOID DllHandle);


// Export #1
dllexport void CpuFlushInstructionCache(HANDLE ProcessHandle, PVOID BaseAddress, ULONG NumberOfBytesToFlush, BOOL skip) {
	if (expect(!skip, true)) {
		NtFlushInstructionCache(ProcessHandle, BaseAddress, NumberOfBytesToFlush);
	}
}

// Export #2
dllexport NTSTATUS CpuGetContext(HANDLE ThreadHandle, int, int, CONTEXTX<32>* context_out) {
	if (expect(
		((int64_t)ThreadHandle == -2) &&
		!(context_out->ContextFlags & ~(CONTEXTX_32 | CONTEXTX_CONTROL | CONTEXTX_INTEGER | CONTEXTX_SEGMENTS))
	, true)) {
		return RtlpWow64GetContextOnAmd64(&TEB_UNKNOWNA->context32, context_out);
	}
	else {
		return NtQueryInformationThread(ThreadHandle, (THREADINFOCLASS)29, context_out, sizeof(CONTEXTX<32>), 0);
	}
}

// Export #3
dllexport PTR32Z<> CpuGetStackPointer() {
	return (PTR32Z<>)TEB_UNKNOWNA->context32.Esp;
}

// Export #4
dllexport void CpuInitializeStartupContext() {
	// TODO
}

// Export #5, 6, 7, 8, 9, 10, 14
dllexport void dummy_ret() {
}

// Export #11
dllexport bool CpuProcessDebugEvent() {
	return false;
}

// Export #12
dllexport NTSTATUS CpuProcessInit(int, int) {
	// TODO
}

// Export #13, 21, 23
dllexport int dummy_ret0() {
	return 0;
}

// Export #15
dllexport void CpuResetToConsistentState() {
	// TODO
}

// Export #16
dllexport NTSTATUS CpuSetContext(HANDLE ThreadHandle, int, int, const CONTEXTX<32>* context_in) {
	UnknownA* idk = TEB_UNKNOWNA;
	bool write_wtf = false;
	NTSTATUS status;
	if (expect(
		((int64_t)ThreadHandle == -2) &&
		((write_wtf = (
			(bool)(context_in->ContextFlags & CONTEXTX_32) &
			(bool)(context_in->ContextFlags & (CONTEXTX_INTEGER | CONTEXTX_EXTENDED_REGISTERS))
		)),
		!(context_in->ContextFlags & ~(CONTEXTX_32 | CONTEXTX_CONTROL | CONTEXTX_INTEGER | CONTEXTX_SEGMENTS)))
	, true)) {
		uint8_t stupid;
		status = RtlpWow64SetContextOnAmd64(&idk->context32, 0, context_in, &stupid);
	}
	else {
		status = NtSetInformationThread(ThreadHandle, (THREADINFOCLASS)29, (PVOID)context_in, sizeof(CONTEXTX<32>));
	}
	if (SUCCEEDED(status) && write_wtf) {
		idk->__dword_2D0 = 1;
	}
	return status;
}

// Export #17
dllexport void CpuSetInstructionPointer(PTR32Z<> ptr) {
	TEB_UNKNOWNA->context32.Eip = (uint32_t)ptr;
}

// Export #18
dllexport void CpuSetStackPointer(PTR32Z<> ptr) {
	TEB_UNKNOWNA->context32.Esp = (uint32_t)ptr;
}

// Export #19
dllexport void CpuSimulate() {
	// TODO
}

// Export #20
dllexport NTSTATUS CpuSuspendLocalThread() {
	return STATUS_UNSUCCESSFUL;
}

gnu_used void X86SwitchTo64BitMode() {
	__asm__ volatile (
		CODE_32_DIRECTIVE
		"ljmp %[Seg],%[Addr]"
		:
		: [Seg]"i"(0x33), [Addr]"i"(0)
	);
	unreachable;
}

// Export #22
dllexport NTSTATUS CpuThreadInit(UnknownA* unknownA) {
	*(uint32_t GS_RELATIVE*)&teb->arbitrary_offset[0x20C0] = (uint32_t)&X86SwitchTo64BitMode;
	if (expect(!TEB_UNKNOWNA, true)) {
		UnknownA* temp = AlignPtrUpToMultipleOf2(unknownA, 16);
		TEB_UNKNOWNA = temp;
		unknownA->context32.SegGs = 0x2B;
		unknownA->context32.SegFs = 0x53;
		unknownA->context32.SegEs = 0x2B;
		unknownA->context32.SegDs = 0x2B;
		unknownA->context32.SegCs = 0x23;
		unknownA->context32.EFlags = 0x202;
		unknownA->context32.SegSs = 0x2B;
		uint32_t initial_esp = *(uint32_t GS_RELATIVE*)&teb->arbitrary_offset[0x2004];
		if (teb->ProcessEnvironmentBlock->IsImageDynamicallyRelocated) {
			initial_esp -= rdtsc_low() & 0x7FC;
		}
		unknownA->context32.Esp = initial_esp;
	}
	return 0;
}

//BOOL APIENTRY DllMain(HMODULE hDll, DWORD ulReasonForCall, LPVOID) {
dllexport BOOL APIENTRY _DllMainCRTStartup(HMODULE hDll, DWORD ulReasonForCall, LPVOID) {
	if (expect(ulReasonForCall == DLL_PROCESS_ATTACH, true)) {
		LdrDisableThreadCalloutsForDll(hDll);
	}
	return TRUE;
}

}