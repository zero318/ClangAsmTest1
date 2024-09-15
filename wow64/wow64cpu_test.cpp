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

#include "..\win_syscalls\syscalls7.h"

#include "wow64_common.h"

static constexpr uint32_t default_eflags_value			= 0x202;
static constexpr uint16_t default_64_bit_code_segment	= 0x33;
static constexpr uint16_t default_data_segment			= default_64_bit_code_segment - 0x8;
static constexpr uint16_t default_32_bit_code_segment	= default_64_bit_code_segment - 0x10;
static constexpr uint16_t teb_segment					= 0x53;

template<typename T>
bool has_bits_set(T value, T bits) {
	return (value & bits) == bits;
}

static constexpr inline uint32_t mask_flags_value(uint32_t value) {
	return (value & 0x3F0DD5) | default_eflags_value;
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
		context_out->SegFs = teb_segment; // context_in->SegFs;
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
		context_out->SegCs = default_32_bit_code_segment; // context_in->SegCs;
		context_out->EFlags = mask_flags_value(context_in->EFlags); // context_in->EFlags;
		context_out->Esp = context_in->Esp;
		context_out->SegSs = default_data_segment; // context_in->SegSs;
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
		context_out->SegCs = default_32_bit_code_segment; // context_in->SegCs;
		context_out->EFlags = mask_flags_value(context_in->EFlags); // context_in->EFlags;
		context_out->Esp = context_in->Esp;
		context_out->SegSs = default_data_segment; // context_in->SegSs;
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

NTSTATUS RtlpWow64CtxFromAmd64(uint32_t context_flags, const CONTEXTX<64>* context64, CONTEXTX<32>* context32) {
	context32->ContextFlags = context_flags;
	if (has_bits_set(context_flags, CONTEXTX_32)) {
		if (has_bits_set(context_flags, CONTEXTX_CONTROL)) {
			context32->Ebp = context64->Rbp;
			context32->Eip = context64->Rip;
			context32->SegCs = default_32_bit_code_segment;
			context32->EFlags = context64->EFlags;
			context32->Esp = context64->Rsp;
			context32->SegSs = default_data_segment;
		}
		if (has_bits_set(context_flags, CONTEXTX_INTEGER)) {
			context32->Edi = context64->Rdi;
			context32->Esi = context64->Rsi;
			context32->Ebx = context64->Rbx;
			context32->Edx = context64->Rdx;
			context32->Ecx = context64->Rcx;
			context32->Eax = context64->Rax;
		}
		if (has_bits_set(context_flags, CONTEXTX_SEGMENTS)) {
			context32->SegGs = default_data_segment;
			context32->SegFs = teb_segment;
			context32->SegEs = default_data_segment;
			context32->SegDs = default_data_segment;
		}
		if (has_bits_set(context_flags, CONTEXTX_EXTENDED_REGISTERS)) {
			/*
			context32->ExtendedRegisters = {};
			context32->ExtendedRegisters.ControlWord = context64->FltSave.ControlWord;
			context32->ExtendedRegisters.StatusWord = context64->FltSave.StatusWord;
			*(uint16_t*)&context32->ExtendedRegisters.TagWord = *(uint16_t*)&context64->FltSave.TagWord;
			context32->ExtendedRegisters.ErrorOpcode = context64->FltSave.ErrorOpcode;
			*(uint64_t*)&context32->ExtendedRegisters.ErrorOffset = context64->FltSave.ErrorOffset;
			*(uint64_t*)&context32->ExtendedRegisters.DataOffset = context64->FltSave.DataOffset;
			context32->ExtendedRegisters.MxCsr = context64->FltSave.MxCsr;
			context32->ExtendedRegisters.MxCsr_Mask = context64->FltSave.MxCsr_Mask;
			*/
			__builtin_memcpy(&context32->ExtendedRegisters, &context64->FltSave, 0x20);
			__builtin_memset(context32->ExtendedRegisters.FloatRegisters, 0, sizeof(context32->ExtendedRegisters.FloatRegisters));
			if (!has_bits_set(context_flags, CONTEXTX_FLOATING_POINT)) {
				for (size_t i = 0; i < 8; ++i) {
					__builtin_memcpy(&context32->FloatSave.RegisterArea[i], &context64->FltSave.FloatRegisters[i], sizeof(PackedLongDouble));
				}
			}
			for (size_t i = 0; i < 8; ++i) {
				context32->ExtendedRegisters.XmmRegisters[i] = context64->FltSave.XmmRegisters[i];
			}
			__builtin_memset(&context32->ExtendedRegisters.XmmRegisters[8], 0, sizeof(context32->ExtendedRegisters) - offsetof(decltype(context32->ExtendedRegisters), XmmRegisters[8]));
		}
		if (has_bits_set(context_flags, CONTEXTX_FLOATING_POINT)) {
			context32->FloatSave.ControlWord.raw_wide = context64->FltSave.ControlWord.raw;
			FSW status_word = context64->FltSave.StatusWord;
			context32->FloatSave.StatusWord.raw_wide = status_word.raw;
			context32->FloatSave.ErrorOffset = context64->FltSave.ErrorOffset;
			context32->FloatSave.ErrorSelector = context64->FltSave.ErrorSelector;
			context32->FloatSave.ErrorOpcode = context64->FltSave.ErrorOpcode;
			context32->FloatSave.DataOffset = context64->FltSave.DataOffset;
			context32->FloatSave.DataSelector = context64->FltSave.DataSelector;
			for (size_t i = 0; i < 8; ++i) {
				__builtin_memcpy(&context32->FloatSave.RegisterArea[i], &context64->FltSave.FloatRegisters[i], sizeof(PackedLongDouble));
			}
			size_t stack_index = 7 - status_word.stack_top;
			uint32_t tag_word = 0;
			int8_t compressed_tag_word = context64->FltSave.TagWord;
			for (size_t i = 0; i < 8; ++i) {
				tag_word <<= 2;
				if (compressed_tag_word < 0) {
					switch (context64->FltSave.FloatRegisters[stack_index].raw[4] & 0x7FFF) {
						case 0:
							tag_word |= 2 >> !*(uint64_t*)&context64->FltSave.FloatRegisters[stack_index];
							break;
						default:
							if (*(int64_t*)&context64->FltSave.FloatRegisters[stack_index] < 0) {
						case 0x7FFF:
							tag_word |= 2;
							}
							break;
					}
				} else {
					tag_word |= 3;
				}
				compressed_tag_word <<= 1;
				stack_index = stack_index - 1 & 7;
			}
			context32->FloatSave.TagWord = tag_word;
		}
		if (expect(has_bits_set(context_flags, CONTEXTX_DEBUG_REGISTERS), false)) {
			uint32_t dr7 = context64->Dr7;
			if (!(dr7 & 0x355)) {
				__builtin_memset(&context32->Dr0, 0, sizeof(uint32_t[6]));
				//context32->Dr0 = 0;
				//context32->Dr1 = 0;
				//context32->Dr2 = 0;
				//context32->Dr3 = 0;
				//context32->Dr6 = 0;
				//context32->Dr7 = 0;
			} else {
				context32->Dr0 = context64->Dr0;
				context32->Dr1 = context64->Dr1;
				context32->Dr2 = context64->Dr2;
				context32->Dr3 = context64->Dr3;
				context32->Dr6 = context64->Dr6;
				context32->Dr7 = dr7 & 0xFFFF0155;
			}
		}
	}
	return 0;
}

bool CpupIsInterixProcess() {
	uint8_t* base_address = (uint8_t*)teb->ProcessEnvironmentBlock->ImageBaseAddress;
	IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)base_address;
	IMAGE_NT_HEADERS32* nt_header = based_pointer<IMAGE_NT_HEADERS32>(base_address, dos_header->e_lfanew);
	switch (nt_header->OptionalHeader.Subsystem) {
		case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		case IMAGE_SUBSYSTEM_WINDOWS_CUI: {
			size_t section_count = nt_header->FileHeader.NumberOfSections;
			IMAGE_SECTION_HEADER* section = based_pointer<IMAGE_SECTION_HEADER>(&nt_header->OptionalHeader, nt_header->FileHeader.SizeOfOptionalHeader);
			while (section_count--) {
				if (expect(!__builtin_memcmp(section->Name, ".mixcrt", sizeof(".mixcrt")), false)) {
					return true;
				}
				++section;
			}
		}
		default:
			return false;
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			return true;
	}
}

extern "C" {

//extern char CpupReturnFromSimulatedCode_addr[] asm("CpupReturnFromSimulatedCode");

extern NTSYSAPI NTSTATUS LdrDisableThreadCalloutsForDll(PVOID DllHandle);

void X86SwitchTo64BitMode() {
	__asm__ volatile (
		CODE_32_DIRECTIVE
		//"ljmpl %[Seg], CpupReturnFromSimulatedCode"
		".byte 0xEA \n"
	"far_jump_addr: \n"
		".int CpupReturnFromSimulatedCode \n"
	"long_mode_segment: \n"
		".word %c[Seg]"
		:
		: [Seg]"i"(default_64_bit_code_segment)
	);
	unreachable;
}

extern PTR32Z<> CpupReturnFromSimulatedCode;
extern PTR32Z<> far_jump_addr;
extern uint16_t long_mode_segment;
extern uint64_t turbo_thunk_jump;

WOW64CPU_EXPORT extern void* TurboDispatchJumpAddressEnd;
WOW64CPU_EXPORT extern void* TurboDispatchJumpAddressStart;

// For now I guess we're just going to hope this is correct
static constexpr size_t turbo_dispatch_length = 8;

// Export #1
WOW64CPU_EXPORT void CpuFlushInstructionCache(HANDLE ProcessHandle, void* BaseAddress, uint32_t NumberOfBytesToFlush, WOW64_FLUSH_REASON Reason) {
	if (expect(Reason != WOW64_FLUSH_FORCE, true)) {
		NtFlushInstructionCache(ProcessHandle, BaseAddress, NumberOfBytesToFlush);
	}
}

// Export #2
WOW64CPU_EXPORT NTSTATUS CpuGetContext(HANDLE ThreadHandle, UNUSED_ARG(HANDLE ProcessHandle), UNUSED_ARG(TEBX<64>* teb_ptr), CONTEXTX<32>* context_out) {
	if (expect(
		(ThreadHandle == CURRENT_THREAD_HANDLE) &&
		!(context_out->ContextFlags & ~(CONTEXTX_32 | CONTEXTX_CONTROL | CONTEXTX_INTEGER | CONTEXTX_SEGMENTS))
	, true)) {
		return RtlpWow64GetContextOnAmd64(&TEB_CPUCONTEXT->context32, context_out);
	}
	else {
		return NtQueryInformationThread(ThreadHandle, (THREADINFOCLASS)29, context_out, sizeof(CONTEXTX<32>), 0);
	}
}

// Export #3
WOW64CPU_EXPORT PTR32Z<> CpuGetStackPointer() {
	return (PTR32Z<>)TEB_CPUCONTEXT->context32.Esp;
}

// Export #4
WOW64CPU_EXPORT void CpuInitializeStartupContext(HANDLE ProcessHandle, int, TEBX<64>* teb_ptr, CPUCONTEXT* context_out, const CONTEXTX<32>* context_in) {
	CPUCONTEXT virt_context = {};
	__builtin_memcpy(&virt_context.context32.Edi, &context_in->Edi, sizeof(uint32_t[8]));
	//virt_context.context32.Edi = context_in->Edi;
	//virt_context.context32.Esi = context_in->Esi;
	//virt_context.context32.Ebx = context_in->Ebx;
	//virt_context.context32.Edx = context_in->Edx;
	//virt_context.context32.Ecx = context_in->Ecx;
	//virt_context.context32.Eax = context_in->Eax;
	//virt_context.context32.Ebp = context_in->Ebp;
	//virt_context.context32.Eip = context_in->Eip;
	virt_context.context32.EFlags = mask_flags_value(context_in->EFlags);
	virt_context.context32.Esp = context_in->Esp;
	if (expect(SUCCEEDED(NtWriteVirtualMemory(ProcessHandle, context_out, &virt_context, sizeof(CPUCONTEXT), NULL)), true)) {
		NtWriteVirtualMemory(ProcessHandle, CPUCONTEXT_FOR_TEB(teb_ptr), &context_out, sizeof(CPUCONTEXT*), NULL);
	}
}

#ifdef COMPILE_WOW64_CPU
// Export #5, 6, 7, 8, 9, 10, 14
// dummy_ret
WOW64CPU_EXPORT void CpuNotifyAffinityChange() {
}
#endif

// Export #12
WOW64CPU_EXPORT NTSTATUS CpuProcessInit(UNUSED_ARG(wchar_t* ImageName), size_t* cpu_size) {
	NTSTATUS ret = 0;
	TEB_WOW64INFO->CpuFlags = 1;
	*cpu_size = sizeof(CPUCONTEXT);
	if (expect(long_mode_segment != default_64_bit_code_segment, false)) {
		PVOID addr = &X86SwitchTo64BitMode;
		ULONG length = 9;
		ULONG old_protection;
		if (expect(SUCCEEDED(ret = NtProtectVirtualMemory(CURRENT_PROCESS_HANDLE, &addr, &length, PAGE_EXECUTE_READWRITE, &old_protection)), true)) {
			far_jump_addr = CpupReturnFromSimulatedCode;
			long_mode_segment = default_64_bit_code_segment;
			ret = NtProtectVirtualMemory(CURRENT_PROCESS_HANDLE, &addr, &length, old_protection, &old_protection);
		}
	}
	if (expect(TEB_WOW64INFO->__unknown_flag_A, false)) {
		if (expect(CpupIsInterixProcess(), false)) {
			PVOID addr = &turbo_thunk_jump;
			ULONG length = turbo_dispatch_length;
			ULONG old_protection;
			if (expect(SUCCEEDED(ret = NtProtectVirtualMemory(CURRENT_PROCESS_HANDLE, &addr, &length, PAGE_EXECUTE_READWRITE, &old_protection)), true)) {
				turbo_thunk_jump = 0x0F1F840000000000u;
				NtProtectVirtualMemory(CURRENT_PROCESS_HANDLE, &addr, &length, old_protection, &old_protection);
				NtFlushInstructionCache(CURRENT_PROCESS_HANDLE, &turbo_thunk_jump, turbo_dispatch_length);
			}
		}
	}
	return ret;
}

#ifdef COMPILE_WOW64_CPU
// Export #11, 13, 21, 23
WOW64CPU_EXPORT int CpuProcessTerm() {
	return 0;
}
#endif

static EXCEPTION_RECORDX<64> RecoverException64;
static CONTEXTX<64> RecoverContext64;

struct IRETQ_Data {
	uint64_t retaddr; // 0x0
	uint64_t cs; // 0x8
	uint64_t rflags; // 0x10
	uint64_t rsp; // 0x18
	uint64_t ss; // 0x20
};

// Export #15
WOW64CPU_EXPORT void CpuResetToConsistentState(EXCEPTION_POINTERSX<64>* exception_pointers) {
	EXCEPTION_RECORDX<64>* record = exception_pointers->ExceptionRecord;
	//RecoverException64 = *record;
	CONTEXTX<64>* context = exception_pointers->ContextRecord;
	//RecoverContext64 = *context;
	void* saved_rsp = TEB_SAVED_RSP;
	TEB_SAVED_RSP = NULL;
	if (context->SegCs == default_32_bit_code_segment) {
		context->SegCs = default_64_bit_code_segment;
		context->Rsp = (uintptr_t)saved_rsp;
		context->Rip = (uintptr_t)CpupReturnFromSimulatedCode; // TODO

		IRETQ_Data* return_data = (IRETQ_Data*)&record[1].ExceptionRecord;
		return_data->retaddr = (uintptr_t)CpupReturnFromSimulatedCode; // TODO
		return_data->cs = default_64_bit_code_segment;
		return_data->rsp = (uint64_t)saved_rsp;
		return_data->ss = default_data_segment;

		RtlpWow64CtxFromAmd64(CONTEXTX_32 | CONTEXTX_CONTROL | CONTEXTX_INTEGER | CONTEXTX_SEGMENTS | CONTEXTX_FLOATING_POINT | CONTEXTX_EXTENDED_REGISTERS, context, &TEB_CPUCONTEXT->context32);
	}
}

// Export #16
WOW64CPU_EXPORT NTSTATUS CpuSetContext(HANDLE ThreadHandle, UNUSED_ARG(HANDLE ProcessHandle), UNUSED_ARG(TEBX<64>* teb_ptr), const CONTEXTX<32>* context_in) {
	CPUCONTEXT* virt_context = TEB_CPUCONTEXT;
	bool iret_return = false;
	NTSTATUS status;
	if (expect(
		((int64_t)ThreadHandle == -2) &&
		((iret_return = (
			(bool)(context_in->ContextFlags & CONTEXTX_32) &
			(bool)(context_in->ContextFlags & (CONTEXTX_INTEGER | CONTEXTX_EXTENDED_REGISTERS))
		)),
		!(context_in->ContextFlags & ~(CONTEXTX_32 | CONTEXTX_CONTROL | CONTEXTX_INTEGER | CONTEXTX_SEGMENTS)))
	, true)) {
		uint8_t stupid;
		status = RtlpWow64SetContextOnAmd64(&virt_context->context32, 0, context_in, &stupid);
	}
	else {
		status = NtSetInformationThread(ThreadHandle, (THREADINFOCLASSX)29, (PVOID)context_in, sizeof(CONTEXTX<32>));
	}
	if (SUCCEEDED(status) && iret_return) {
		//virt_context->flags = 1;
		virt_context->return_with_iret = true;
	}
	return status;
}

// Export #17
WOW64CPU_EXPORT void CpuSetInstructionPointer(PTR32Z<> ptr) {
	TEB_CPUCONTEXT->context32.Eip = (uint32_t)ptr;
}

// Export #18
WOW64CPU_EXPORT void CpuSetStackPointer(PTR32Z<> ptr) {
	TEB_CPUCONTEXT->context32.Esp = (uint32_t)ptr;
}

static int yeetus = 7;
gnu_noinline int system_service_ex(int arg) {
	return yeetus = yeetus * 6372654 + 1236 + arg;
}

#define USE_LSS 1

// Export #19
WOW64CPU_EXPORT void CpuSimulate() {
	// TODO
	CPUCONTEXT* context = TEB_CPUCONTEXT;

	//FarPtr32<> far_jump;
	//far_jump.seg = default_32_bit_code_segment;

	static constexpr void* turbo_thunk_table[] = {
		&&turbo_dispatch_end,
		&&Thunk0Arg,
		&&Thunk0ArgReloadState,
		&&WaitForMultipleObjects,
		&&WaitForMultipleObjects32
	};

#if !USE_LSS
	uint32_t ss_data_segment = default_data_segment;
	__asm__ volatile (
		""
		: "+r"(ss_data_segment)
		:
		//: clobber_list("eax", "edx", "ecx", "r8d", "r9d", "r10d", "r11d")
	);
#endif

	for (;;) {

		register uint32_t edi asm("edi") = context->context32.Edi;
		register uint32_t esi asm("esi") = context->context32.Esi;
		register uint32_t ebx asm("ebx") = context->context32.Ebx;
		union {
			uint64_t rdx;
			uint32_t edx;
		};
		union {
			uint64_t rcx;
			uint32_t ecx;
		};
		register uint32_t eax asm("eax") = context->context32.Eax;
		register uint32_t ebp asm("ebp") = context->context32.Ebp;
		uint64_t r8;
		uint64_t r9;
		if (context->return_with_iret) {
			register __m128 xmm0 asm("xmm0") = context->context32.ExtendedRegisters.XmmRegisters[0];
			register __m128 xmm1 asm("xmm1") = context->context32.ExtendedRegisters.XmmRegisters[1];
			register __m128 xmm2 asm("xmm2") = context->context32.ExtendedRegisters.XmmRegisters[2];
			register __m128 xmm3 asm("xmm3") = context->context32.ExtendedRegisters.XmmRegisters[3];
			register __m128 xmm4 asm("xmm4") = context->context32.ExtendedRegisters.XmmRegisters[4];
			register __m128 xmm5 asm("xmm5") = context->context32.ExtendedRegisters.XmmRegisters[5];
iretq_back:
			uint32_t flags = context->context32.EFlags;
			context->context32.EFlags &= ~0x100;
			TEB_SAVED_RSP = (void*)rsp_reg;
			*(IRETQ_Data*)rsp_reg = {
				.retaddr = context->context32.Eip,
				.cs = default_32_bit_code_segment,
				.rflags = flags,
				.rsp = context->context32.Esp,
				.ss = default_data_segment
			};
			__asm__ volatile (
				"IRETQ"
				:
				:
				"c"(context->context32.Ecx),
				"d"(context->context32.Edx),
				"a"(eax), "b"(ebx), "r"(ebp), "S"(esi), "D"(edi),
				"x"(xmm0), "x"(xmm1), "x"(xmm2), "x"(xmm3), "x"(xmm4), "x"(xmm5)
			);
			unreachable;
		}

#if USE_LSS
	#define FAR_JUMP_BACK_MACRO() \
		TEB_SAVED_RSP = (void*)rsp_reg; \
		context->context32.SegCs = default_32_bit_code_segment; \
		context->context32.SegSs = default_data_segment; \
		__asm__ volatile ( \
			"lssl %[stack], %%esp \n" \
			"ljmp *%[far_jump]" \
			: \
			: [stack]"m"(context->context32.Esp), [far_jump]"m"(context->context32.Eip) \
			, "a"(eax), "b"(ebx), "r"(ebp), "S"(esi), "D"(edi) \
			: clobber_list("ecx", "edx") \
		)
#else

#define FAR_JUMP_BACK_MACRO() \
		TEB_SAVED_RSP = (void*)rsp_reg; \
		write_ss(ss_data_segment); \
		context->context32.SegCs = default_32_bit_code_segment; \
		esp_reg = context->context32.Esp; \
		__asm__ volatile ( \
			"ljmp *%[far_jump]" \
			: \
			: [far_jump]"m"(context->context32.Eip), "a"(eax), "b"(ebx), "r"(ebp), "S"(esi), "D"(edi) \
			: clobber_list("ecx", "edx") \
		)
#endif
		

far_jump_back:
		
#if USE_LSS
		TEB_SAVED_RSP = (void*)rsp_reg;
		//far_jump.addr = (PTR32Z<>)context->context32.Eip;
		context->context32.SegCs = default_32_bit_code_segment;
		context->context32.SegSs = default_data_segment; \
		__asm__ volatile (
			"lssl %[stack], %%esp \n" \
			"ljmp *%[far_jump]"
			:
			//: [far_jump]"r"(&far_jump), "a"(eax), "b"(ebx), "r"(ebp), "S"(esi), "D"(edi)
			: [stack] "m"(context->context32.Esp), [far_jump]"m"(context->context32.Eip)
			, "a"(eax), "b"(ebx), "r"(ebp), "S"(esi), "D"(edi)
			: clobber_list("ecx", "edx")
		);
#else
		TEB_SAVED_RSP = (void*)rsp_reg;
		//far_jump.addr = (PTR32Z<>)context->context32.Eip;
		write_ss(ss_data_segment);
		context->context32.SegCs = default_32_bit_code_segment;
		esp_reg = context->context32.Esp;
		__asm__ volatile (
			//"ljmp *(%[far_jump])"
			"ljmp *%[far_jump]"
			:
			//: [far_jump]"r"(&far_jump), "a"(eax), "b"(ebx), "r"(ebp), "S"(esi), "D"(edi)
			: [far_jump]"m"(context->context32.Eip), "a"(eax), "b"(ebx), "r"(ebp), "S"(esi), "D"(edi)
			: clobber_list("ecx", "edx")
		);
#endif
		

		__asm__ volatile(
			".align 16, 0xCC \n"
		"CpupReturnFromSimulatedCode: \n"
			: "=r"(esp_reg), "=a"(eax), "=c"(rcx), "=d"(rdx), "=b"(ebx), "=r"(ebp), "=S"(esi), "=D"(edi)
		);
		// EAX = Syscall index
		// ECX = Turbo thunk index
		// EDX = Stack arg ptr

		context->context32.Eip = *(uint32_t*)esp_reg;
		context->context32.Esp = esp_reg;
		rsp_reg = (uint64_t)TEB_SAVED_RSP;
		TEB_SAVED_RSP = NULL;

		//goto *(turbo_thunk_lookup[rcx]);
		__asm__ volatile goto (
			".global TurboDispatchJumpAddressStart \n"
		"TurboDispatchJumpAddressStart: \n"
		"turbo_thunk_jump: \n"
			INTEL_SYNTAX_DIRECTIVE
			"JMP QWORD PTR [%V[rcx]*8+%c[turbo_thunk_table]] \n"
			ATT_SYNTAX_DIRECTIVE
			".byte 0xCC \n"
			:
			: asm_arg("r", rcx), asm_arg("i", turbo_thunk_table)
			:
			: turbo_dispatch_end,
			Thunk0Arg, Thunk0ArgReloadState,
			WaitForMultipleObjects, WaitForMultipleObjects32
		);


#define SAVE_STATE() \
		context->context32.Edi = edi; \
		context->context32.Esi = esi; \
		context->context32.Ebx = ebx; \
		context->context32.Ebp = ebp; \
		context->context32.EFlags = __readeflags()

turbo_dispatch_end:
		__asm__ volatile (
			".global TurboDispatchJumpAddressEnd \n"
			"TurboDispatchJumpAddressEnd:"
		);

		SAVE_STATE();
		context->context32.Eax = system_service_ex(eax);
		continue;

GetCurrentProcessorNumber:
		eax = load_segment_limit(0x53) >> 14 & 0x1F;
		goto far_jump_back;

QuerySystemTime:
		*(int64_t*)rdx = USER_SHARED_DATA64.SystemTime.QuadPart;
		goto far_jump_back;

WaitForMultipleObjects:
WaitForMultipleObjects32:
		{
			SAVE_STATE();
			uint32_t* args = (uint32_t*)rdx;
			eax = NtWaitForMultipleObjects32(args[0], (PHANDLE)args[1], (OBJECT_WAIT_TYPE)args[2], args[3], (PLARGE_INTEGER)args[4]);
			goto state_reload;
		}

Thunk0Arg:
		context->context32.Ebp = ebp;
		eax = syscall0<int>((SyscallIndex)eax);
		goto far_jump_back;

state_reload:
		edi = context->context32.Edi;
		esi = context->context32.Esi;
		ebx = context->context32.Ebx;
		ebp = context->context32.Ebp;
		if (!context->return_with_iret) goto far_jump_back;
		goto iretq_back;

Thunk0ArgReloadState:
		SAVE_STATE();
		eax = syscall0<int>((SyscallIndex)eax);
		goto state_reload;

Thunk1ArgSp:

	}
}

// Export #20
WOW64CPU_EXPORT NTSTATUS CpuSuspendLocalThread() {
	return STATUS_UNSUCCESSFUL;
}

// Export #22
WOW64CPU_EXPORT NTSTATUS CpuThreadInit(CPUCONTEXT* virt_context) {
	//*(uint32_t GS_RELATIVE*)&teb->arbitrary_offset[0x20C0] = (uint32_t)&X86SwitchTo64BitMode;
	write_teb_offset(0x20C0, (uint32_t)&X86SwitchTo64BitMode);
	if (expect(!TEB_CPUCONTEXT, true)) {
		CPUCONTEXT* temp = AlignPtrUpToMultipleOf2(virt_context, 16);
		TEB_CPUCONTEXT = temp;
		virt_context->context32.SegGs = default_data_segment;
		virt_context->context32.SegFs = teb_segment;
		virt_context->context32.SegEs = default_data_segment;
		virt_context->context32.SegDs = default_data_segment;
		virt_context->context32.SegCs = default_32_bit_code_segment;
		virt_context->context32.EFlags = default_eflags_value;
		virt_context->context32.SegSs = default_data_segment;
		//uint32_t initial_esp = *(uint32_t GS_RELATIVE*)&teb->arbitrary_offset[0x2004];
		uint32_t initial_esp = read_teb_offset<uint32_t>(0x2004);
		if (teb->ProcessEnvironmentBlock->IsImageDynamicallyRelocated) {
			initial_esp -= rdtsc_low() << 2 & 0x7FC;
		}
		virt_context->context32.Esp = initial_esp;
	}
	return 0;
}


#ifdef COMPILE_WOW64_CPU
//BOOL APIENTRY DllMain(HMODULE hDll, DWORD ulReasonForCall, LPVOID) {
BOOL APIENTRY _DllMainCRTStartup(HMODULE hDll, DWORD ulReasonForCall, LPVOID) {
	if (expect(ulReasonForCall == DLL_PROCESS_ATTACH, true)) {
		LdrDisableThreadCalloutsForDll(hDll);
	}
	return TRUE;
}
#endif

}