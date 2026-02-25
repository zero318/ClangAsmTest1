#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

#define POINTER_64 __ptr64

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Windows.h"

#include "zero/util.h"
#include "windows_structs.h"
#include "x86_msrs.h"

#include <stdnoreturn.h>


extern "C" {

	static KPRCBX<64> static_kprcb;

	static void (*HalRequestSoftwareInterrupt)(uint8_t) = (void(*)(uint8_t))rand();
	static uint64_t MmUserProbeAddress = rand();
	static uint8_t KiKvaShadow = rand();
	static uint32_t __some_random_flag_dword_idk = rand();
	static uint32_t HvlLongSpinCountMask = rand();
	static uint32_t HvlEnlightenments = rand();
	static uint64_t KeFeatureBits = rand();
	static uint64_t VgaRegisterBase = rand();

gnu_noinline void KiBeginCounterAccumulation(PTR64Z<KTHREADX<64>> thread, uint8_t arg2) {
	use_var(thread);
	use_var(arg2);
	rand();
}

gnu_noinline void PsCheckThreadCpuQuota(PTR64Z<KPRCBX<64>> prcb_ptr, uint8_t arg2, PTR64Z<KTHREADX<64>> thread) {
	use_var(prcb_ptr);
	use_var(arg2);
	use_var(thread);
	rand();
}

gnu_noinline void KeInterlockedClearProcessorAffinityEx(volatile KAFFINITY_EXX<64>* affinity, uint32_t number) {
	use_var(affinity);
	use_var(number);
	rand();
}
gnu_noinline void KeInterlockedSetProcessorAffinityEx(volatile KAFFINITY_EXX<64>* affinity, uint32_t number) {
	use_var(affinity);
	use_var(number);
	rand();
}

gnu_noinline void KiUpdateSpeculationControl(KPROCESSX<64>* process) {
	use_var(process);
	rand();
}

gnu_noinline void HvlNotifyLongSpinWait(uint32_t count) {
	use_var(count);
	rand();
}

gnu_noinline void EtwTraceContextSwap(
	PTR64Z<KTHREADX<64>> prev_thread,
	PTR64Z<KTHREADX<64>> next_thread
) {
	use_var(prev_thread);
	use_var(next_thread);
	rand();
}

noreturn gnu_noinline void KeBugCheckEx(
	ULONG     BugCheckCode,
	ULONG_PTR BugCheckParameter1,
	ULONG_PTR BugCheckParameter2,
	ULONG_PTR BugCheckParameter3,
	ULONG_PTR BugCheckParameter4
) {
	use_var(BugCheckCode);
	use_var(BugCheckParameter1);
	use_var(BugCheckParameter2);
	use_var(BugCheckParameter3);
	use_var(BugCheckParameter4);
	rand();
	__asm int 3
}

extern uint8_t SwapContext_PatchXSave;
extern uint8_t SwapContext_PatchXRstor;
extern uint8_t EnlightenedSwapContext_PatchXSave;
extern uint8_t EnlightenedSwapContext_PatchXRstor;

uint8_t* KiXSaveOptPatchPointers[2] = {
	&SwapContext_PatchXSave,
	&EnlightenedSwapContext_PatchXSave
};
uint8_t* KiXRstorPatchPointers[2] = {
	&SwapContext_PatchXRstor,
	&EnlightenedSwapContext_PatchXRstor
};

}

#include "x86_msrs.h"

#define STATUS_SUCCESS ((DWORD)0L)

dllexport gnu_noinline NTSTATUS KiPatchModRMReg(
	uint8_t* address,
	uint16_t opcode, // unused
	uint8_t patch
) {
	// Try to skip a REX byte
	if (*address & 0x40) {
		++address;
	}
	// Nice hardcoded opcode
	if (*(uint16_t*)address != 0xAE0F) {
		return STATUS_INVALID_PARAMETER;
	}
	address[2] &= 0xC7;
	address[2] |= patch & 0x38;
	return STATUS_SUCCESS;
}

dllexport NTSTATUS KiPatchForXSave() {
	if (KeFeatureBits & 0x800000) {
		// heck is this address
		uint8_t patch = (*(uint8_t*)0xFFFFF780000003EC & 1) ? 0x30 : 0x20;
		uint16_t opcode = 0xAE0F;

		NTSTATUS status;
		uint8_t** address_ptr = KiXSaveOptPatchPointers;
		while (*address_ptr) {
			if (FAILED(status = KiPatchModRMReg(*address_ptr, opcode, patch))) {
				return status;
			}
			++address_ptr;
		}
		address_ptr = KiXRstorPatchPointers;
		while (*address_ptr) {
			if (FAILED(status = KiPatchModRMReg(*address_ptr, opcode, 0x28))) {
				return status;
			}
			++address_ptr;
		}
	}
	return STATUS_SUCCESS;
}

dllexport uint8_t regcall swap_context_test(
	int, // RAX
	KIRQL irql, // RCX
	int, // RDX
	PTR64Z<KTHREADX<64>> prev_thread, // RDI
	PTR64Z<KTHREADX<64>> next_thread, // RSI
	int, // R8
	int, // R9
	int, // R10
	int, // R11
	int, // R12
	int, // R14
	int // R15
) {
	volatile KIRQL irql_stack = irql;

	register PTR64Z<KPRCBX<64>> kprcb_ptr asm("rbx");
	__asm__("":"=b"(kprcb_ptr));

	_m_prefetchw(&next_thread->Running);
	while (expect(next_thread->Running, false)) {
		_mm_pause();
	}
	next_thread->Running = true;
	__asm CLI
	uint64_t timestamp = rdtsc();
	timestamp -= kprcb_ptr->StartCycles;
	kprcb_ptr->CycleTime += timestamp;
	kprcb_ptr->StartCycles += timestamp;
	if (kprcb_ptr->InterruptRequest != 0xFF) {
		kprcb_ptr->InterruptRequest = 0;
		if (next_thread != kprcb_ptr->IdleThread) {
			HalRequestSoftwareInterrupt(2);
		}
	}
	if (expect((bool)(next_thread->Header.CpuThrottled | next_thread->Header.CounterProfiling), false)) {
		if (next_thread->Header.CounterProfiling) {
			KiBeginCounterAccumulation(next_thread, 1);
		}
		if (next_thread->Header.CpuThrottled) {
			--kprcb_ptr->NestingLevel;
			__asm STI
			PsCheckThreadCpuQuota(kprcb_ptr, 1, next_thread);
			goto throttled_jump;
		}
	}
	--kprcb_ptr->NestingLevel;
	__asm STI
throttled_jump:
	++kprcb_ptr->KeContextSwitches;

	// Handle state saving
	int32_t prev_xsave_mask = prev_thread->NpxState; 
	prev_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (prev_xsave_mask) {
		XSAVE_FORMATX<64>* state_save = prev_thread->StateSaveArea;
		__asm__(
		"SwapContext_PatchXSave: \n"
			"FXSAVE (%[ptr]) \n"
			:
			: "a"(prev_xsave_mask), "d"(prev_xsave_mask >> 31), [ptr]"r"(state_save)
		);
		// x87 bit
		if (prev_xsave_mask & 1) {
			if (state_save->StatusWord.exception_summary) {
				__asm FNCLEX
			}
			__asm__(
				"FFREE %%ST(7) \n"
				"FILDL (%[ptr])"
				:
				: [ptr]"r"(state_save)
			);
		}
	}

	// Swap thread stacks
	prev_thread->KernelStack = (void*)rsp_reg;
	rsp_reg = (uint64_t)next_thread->KernelStack;

	if (prev_thread->Header.UmsPrimary) {
		uintptr_t usermode_gs_base = (uintptr_t)MSR_KERNEL_GS_BASE::read().address;
		prev_thread->Ucb->UmsTeb = (void*)__min(usermode_gs_base, MmUserProbeAddress);
	}

	KPROCESSX<64>* new_process = next_thread->Process;
	if (new_process != prev_thread->Process) {
		KiUpdateSpeculationControl(new_process);
	}

	if (!KiKvaShadow) {
		KPROCESSX<64>* new_process = next_thread->Process;
		if (new_process->LdtBaseAddress) {
			kprcb_ptr->kpcr()->GdtBase[7] = new_process->LdtSystemDescriptor;
			uint64_t selector = 0x70;
			__asm__(
				"LLDT %w[selector]"
				:
				: [selector]"r"(selector)
			);
		}
	}

	KPROCESSX<64>* new_apc_process = next_thread->ApcState.Process;
	if (new_apc_process != prev_thread->ApcState.Process) {
		KeInterlockedClearProcessorAffinityEx(&prev_thread->ApcState.Process->ActiveProcessors, kprcb_ptr->Number);
		KeInterlockedSetProcessorAffinityEx(&new_apc_process->ActiveProcessors, kprcb_ptr->Number);

		// CR3 value
		uintptr_t new_directory_table = new_apc_process->DirectoryTableBase;

		if (KiKvaShadow & 1) {
			__asm CLI
			if (new_directory_table & 2) {
				new_directory_table |= 0x8000000000000000;
				kprcb_ptr->__shadow_flag_A = true;
			}
			kprcb_ptr->KernelDirectoryTableBase = new_directory_table;
			new_directory_table &= 0x7FFFFFFFFFFFFFFF;
			kprcb_ptr->__shadow_flag_B = false;
			if (new_apc_process->__flip_shadow_flags) {
				kprcb_ptr->__shadow_flag_A ^= true;
				kprcb_ptr->__shadow_flag_B ^= true;
			}
			__asm STI
		}

		write_cr3(new_directory_table);

		if (
			(KiKvaShadow & 1) &&
			!(new_directory_table & 2)
		) {
			uintptr_t cr4 = read_cr4();
			cr4 ^= 0x80;
			write_cr4(cr4);
			cr4 ^= 0x80;
			write_cr4(cr4);
		}
	}

	uint64_t new_stack = (uint64_t)next_thread->InitialStack;
	if (!(KiKvaShadow & 1)) {
		kprcb_ptr->kpcr()->TssBase->Rsp0 = new_stack;
	}
	else {
		kprcb_ptr->RspBaseShadow = new_stack;
	}
	kprcb_ptr->RspBase = new_stack;

	if (expect(__some_random_flag_dword_idk & 4, false)) {
		EtwTraceContextSwap(prev_thread, next_thread);
	}

	prev_thread->Running = false;

	// Handle state loading
	int32_t new_xsave_mask = next_thread->NpxState;
	new_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (new_xsave_mask) {
		const XSAVE_FORMATX<64>* state_save = next_thread->StateSaveArea;
		__asm__(
		"SwapContext_PatchXRstor: \n"
			"FXRSTOR (%[ptr]) \n"
			:
			: "a"(new_xsave_mask), "d"(new_xsave_mask >> 31), [ptr]"r"(state_save)
		);
	}

	if (!next_thread->SystemThread) {
		uint32_t low_teb = (uint32_t)next_thread->Teb + 0x2000;
		kprcb_ptr->kpcr()->Gdt32Base[10].set_base(low_teb);

		uint32_t constant_segments = read_ds() & read_es() & read_gs();
		if ((uint16_t)constant_segments != 0x2B) {
			uint32_t data_segment = 0x2B;
			write_ds(data_segment);
			write_es(data_segment);
			__asm__ volatile (
				"CLI \n"
				"SWAPGS \n"
			);
			write_gs(data_segment);
			__asm__ volatile (
				"SWAPGS \n"
				"STI \n"
			);
		}
		write_fs(0x53);

		uint64_t teb = (uint64_t)next_thread->Teb;
		kprcb_ptr->kpcr()->Used_Self = (TEBX<64>*)teb;

		if (next_thread->Header.UmsPrimary) {
			teb = (uint64_t)next_thread->Ucb->UmsTeb;
		}
		MSR_KERNEL_GS_BASE::write_raw(teb);
	}

	if (expect(kprcb_ptr->DpcRoutineActive, false)) {
		KeBugCheckEx(0xB8, (uintptr_t)prev_thread, (uintptr_t)next_thread, 0, 0);
	}

	++next_thread->ContextSwitches;

	if (next_thread->ApcState.KernelApcPending == true) {
		if (next_thread->SpecialApcDisable | irql_stack) {
			HalRequestSoftwareInterrupt(1);
		}
		else {
			return true;
		}
	}
	return false;
}

dllexport uint8_t regcall enlightened_swap_context_test(
	int, // RAX
	KIRQL irql, // RCX
	int, // RDX
	PTR64Z<KTHREADX<64>> prev_thread, // RDI
	PTR64Z<KTHREADX<64>> next_thread, // RSI
	int, // R8
	int, // R9
	int, // R10
	int, // R11
	int, // R12
	int, // R14
	int // R15
) {
	volatile KIRQL irql_stack = irql;

	register PTR64Z<KPRCBX<64>> kprcb_ptr asm("rbx");
	__asm__("":"=b"(kprcb_ptr));

	_m_prefetchw(&next_thread->Running);
	if (expect(next_thread->Running, false)) {
		uint32_t count = 0;
		do {
			++count;
			if (
				!(HvlLongSpinCountMask & count) &&
				(HvlEnlightenments & 0x40)
			) {
				HvlNotifyLongSpinWait(count);
			}
			_mm_pause();
		} while (expect(next_thread->Running, true));
	}
	next_thread->Running = true;
	__asm CLI
	uint64_t timestamp = rdtsc();
	timestamp -= kprcb_ptr->StartCycles;
	kprcb_ptr->CycleTime += timestamp;
	kprcb_ptr->StartCycles += timestamp;
	if (kprcb_ptr->InterruptRequest != 0xFF) {
		kprcb_ptr->InterruptRequest = 0;
		if (next_thread != kprcb_ptr->IdleThread) {
			HalRequestSoftwareInterrupt(2);
		}
	}
	if (expect((bool)(next_thread->Header.CpuThrottled | next_thread->Header.CounterProfiling), false)) {
		if (next_thread->Header.CounterProfiling) {
			KiBeginCounterAccumulation(next_thread, 1);
		}
		if (next_thread->Header.CpuThrottled) {
			--kprcb_ptr->NestingLevel;
			__asm STI
			PsCheckThreadCpuQuota(kprcb_ptr, 1, next_thread);
			goto throttled_jump;
		}
	}
	--kprcb_ptr->NestingLevel;
	__asm STI
throttled_jump:
	++kprcb_ptr->KeContextSwitches;

	// Handle state saving
	int32_t prev_xsave_mask = prev_thread->NpxState; 
	prev_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (prev_xsave_mask) {
		XSAVE_FORMATX<64>* state_save = prev_thread->StateSaveArea;
		__asm__(
		"EnlightenedSwapContext_PatchXSave: \n"
			"FXSAVE (%[ptr]) \n"
			:
			: "a"(prev_xsave_mask), "d"(prev_xsave_mask >> 31), [ptr]"r"(state_save)
		);
		// x87 bit
		if (prev_xsave_mask & 1) {
			if (state_save->StatusWord.exception_summary) {
				__asm FNCLEX
			}
			__asm__(
				"FFREE %%ST(7) \n"
				"FILDL (%[ptr])"
				:
				: [ptr]"r"(state_save)
			);
		}
	}

	// Swap thread stacks
	prev_thread->KernelStack = (void*)rsp_reg;
	rsp_reg = (uint64_t)next_thread->KernelStack;

	if (prev_thread->Header.UmsPrimary) {
		uintptr_t usermode_gs_base = (uintptr_t)MSR_KERNEL_GS_BASE::read().address;
		prev_thread->Ucb->UmsTeb = (void*)__min(usermode_gs_base, MmUserProbeAddress);
	}

	KPROCESSX<64>* new_process = next_thread->Process;
	if (new_process != prev_thread->Process) {
		KiUpdateSpeculationControl(new_process);
	}

	if (!KiKvaShadow) {
		KPROCESSX<64>* new_process = next_thread->Process;
		if (new_process->LdtBaseAddress) {
			kprcb_ptr->kpcr()->GdtBase[7] = new_process->LdtSystemDescriptor;
			uint64_t selector = 0x70;
			__asm__(
				"LLDT %w[selector]"
				:
				: [selector]"r"(selector)
			);
		}
	}

	KPROCESSX<64>* new_apc_process = next_thread->ApcState.Process;
	if (new_apc_process != prev_thread->ApcState.Process) {
		KeInterlockedClearProcessorAffinityEx(&prev_thread->ApcState.Process->ActiveProcessors, kprcb_ptr->Number);
		KeInterlockedSetProcessorAffinityEx(&new_apc_process->ActiveProcessors, kprcb_ptr->Number);

		// CR3 value
		uintptr_t new_directory_table = new_apc_process->DirectoryTableBase;

		if (KiKvaShadow & 1) {
			__asm CLI
			if (new_directory_table & 2) {
				new_directory_table |= 0x8000000000000000;
				kprcb_ptr->__shadow_flag_A = true;
			}
			kprcb_ptr->KernelDirectoryTableBase = new_directory_table;
			new_directory_table &= 0x7FFFFFFFFFFFFFFF;
			kprcb_ptr->__shadow_flag_B = false;
			if (new_apc_process->__flip_shadow_flags) {
				kprcb_ptr->__shadow_flag_A ^= true;
				kprcb_ptr->__shadow_flag_B ^= true;
			}
			__asm STI
		}

		write_cr3(new_directory_table);

		if (
			(KiKvaShadow & 1) &&
			!(new_directory_table & 2)
		) {
			uintptr_t cr4 = read_cr4();
			cr4 ^= 0x80;
			write_cr4(cr4);
			cr4 ^= 0x80;
			write_cr4(cr4);
		}
	}

	uint64_t new_stack = (uint64_t)next_thread->InitialStack;
	if (!(KiKvaShadow & 1)) {
		kprcb_ptr->kpcr()->TssBase->Rsp0 = new_stack;
	}
	else {
		kprcb_ptr->RspBaseShadow = new_stack;
	}
	kprcb_ptr->RspBase = new_stack;

	if (expect(__some_random_flag_dword_idk & 4, false)) {
		EtwTraceContextSwap(prev_thread, next_thread);
	}

	prev_thread->Running = false;

	// Handle state loading
	int32_t new_xsave_mask = next_thread->NpxState;
	new_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (new_xsave_mask) {
		const XSAVE_FORMATX<64>* state_save = next_thread->StateSaveArea;
		__asm__(
		"EnlightenedSwapContext_PatchXRstor: \n"
			"FXRSTOR (%[ptr]) \n"
			:
			: "a"(new_xsave_mask), "d"(new_xsave_mask >> 31), [ptr]"r"(state_save)
		);
	}

	if (!next_thread->SystemThread) {
		uint32_t low_teb = (uint32_t)next_thread->Teb + 0x2000;
		kprcb_ptr->kpcr()->Gdt32Base[10].set_base(low_teb);

		uint32_t constant_segments = read_ds() & read_es() & read_gs();
		if ((uint16_t)constant_segments != 0x2B) {
			uint32_t data_segment = 0x2B;
			write_ds(data_segment);
			write_es(data_segment);
			__asm__ volatile (
				"CLI \n"
				"SWAPGS \n"
			);
			write_gs(data_segment);
			__asm__ volatile (
				"SWAPGS \n"
				"STI \n"
			);
		}
		write_fs(0x53);

		uint64_t teb = (uint64_t)next_thread->Teb;
		kprcb_ptr->kpcr()->Used_Self = (TEBX<64>*)teb;

		if (next_thread->Header.UmsPrimary) {
			teb = (uint64_t)next_thread->Ucb->UmsTeb;
		}
		MSR_KERNEL_GS_BASE::write_raw(teb);
	}

	if (expect(kprcb_ptr->DpcRoutineActive, false)) {
		KeBugCheckEx(0xB8, (uintptr_t)prev_thread, (uintptr_t)next_thread, 0, 0);
	}

	++next_thread->ContextSwitches;

	if (next_thread->ApcState.KernelApcPending == true) {
		if (next_thread->SpecialApcDisable | irql_stack) {
			HalRequestSoftwareInterrupt(1);
		}
		else {
			return true;
		}
	}
	return false;
}

dllexport uint8_t regcall swap_context_new(
	int, // RAX
	KIRQL irql, // RCX
	int, // RDX
	PTR64Z<KTHREADX<64>> prev_thread, // RDI
	PTR64Z<KTHREADX<64>> next_thread, // RSI
	int, // R8
	int, // R9
	int, // R10
	int, // R11
	int, // R12
	int, // R14
	int // R15
) {
	volatile KIRQL irql_stack = irql;

	register PTR64Z<KPRCBX<64>> kprcb_ptr asm("rbx");
	__asm__("":"=b"(kprcb_ptr));

	uint8_t* thread_running = (uint8_t*)&next_thread->Running;

	_m_prefetchw(thread_running);
	while (expect(*thread_running, false)) {
		_mm_pause();
	}
	*thread_running = true;

	__asm CLI
	uint64_t timestamp = rdtsc();
	timestamp -= kprcb_ptr->StartCycles;
	kprcb_ptr->StartCycles += timestamp;
	kprcb_ptr->CycleTime += timestamp;
	if (kprcb_ptr->InterruptRequest != 0xFF) {
		kprcb_ptr->InterruptRequest = 0;
		if (next_thread != kprcb_ptr->IdleThread) {
			HalRequestSoftwareInterrupt(2);
		}
	}
	uint8_t thread_control_flags = next_thread->Header.ThreadControlFlags;
	if (expect(thread_control_flags & 5, false)) {
		if (thread_control_flags & 4) {
			KiBeginCounterAccumulation(next_thread, 1);
		}
		if (thread_control_flags & 1) {
			--kprcb_ptr->NestingLevel;
			__asm STI
			PsCheckThreadCpuQuota(kprcb_ptr, 1, next_thread);
			goto throttled_jump;
		}
	}
	--kprcb_ptr->NestingLevel;
	__asm STI
throttled_jump:
	++kprcb_ptr->KeContextSwitches;

	// Handle state saving
	int32_t prev_xsave_mask = prev_thread->NpxState; 
	prev_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (prev_xsave_mask) {
		XSAVE_FORMATX<64>* state_save = prev_thread->StateSaveArea;
		__asm__(
			"CDQ \n"
			"FXSAVE (%[ptr]) \n"
			:
			: "a"(prev_xsave_mask), [ptr]"r"(state_save)
			: clobber_list("rdx")
		);
		// x87 bit
		if (prev_xsave_mask & 1) {
			if (state_save->StatusWord.exception_summary) {
				__asm FNCLEX
			}
			__asm__(
				"FFREE %%ST(7) \n"
				"FILDL (%[ptr])"
				:
				: [ptr]"r"(state_save)
			);
		}
	}

	if (expect(!next_thread->SystemThread, true)) {
		uint64_t fs_base = rdfsbase64();
		*based_pointer<uint32_t>(prev_thread, 0x84) = fs_base;
		prev_thread->Spare0 = fs_base >> 32;
	}

	// Swap thread stacks
	prev_thread->KernelStack = (void*)rsp_reg;
	rsp_reg = (uint64_t)next_thread->KernelStack;

	if (prev_thread->Header.UmsPrimary) {
		uintptr_t usermode_gs_base = (uintptr_t)MSR_KERNEL_GS_BASE::read().address;
		prev_thread->Ucb->UmsTeb = (void*)__min(usermode_gs_base, MmUserProbeAddress);
	}

	KPROCESSX<64>* new_process = next_thread->Process;
	if (new_process != prev_thread->Process) {
		KiUpdateSpeculationControl(new_process);
	}

	uint8_t KVA_SHADOW = KiKvaShadow;

	if (!KVA_SHADOW) {
		if (new_process->LdtBaseAddress) {
			kprcb_ptr->kpcr()->GdtBase[7] = new_process->LdtSystemDescriptor;
			uint32_t selector = 0x70;
			__asm__(
				"LLDT %w[selector]"
				:
				: [selector]"r"(selector)
			);
		}
	}

	bool KVA_SHADOW_FLAG = KVA_SHADOW & 1;

	uint64_t new_stack;

	KPROCESSX<64>* new_apc_process = next_thread->ApcState.Process;
	KPROCESSX<64>* prev_apc_process = prev_thread->ApcState.Process;
	if (new_apc_process != prev_apc_process) {
		uint32_t number = kprcb_ptr->Number;
		KeInterlockedClearProcessorAffinityEx(&prev_apc_process->ActiveProcessors, number);
		KeInterlockedSetProcessorAffinityEx(&new_apc_process->ActiveProcessors, number);

		// CR3 value
		uintptr_t new_directory_table = new_apc_process->DirectoryTableBase;

		if (expect_chance(KVA_SHADOW_FLAG, true, 0.6)) {
			__asm CLI
			uint32_t new_shadow_flags = 0;
			uintptr_t kernel_directory_table = new_directory_table;
			if (new_directory_table & 2) {
				kernel_directory_table |= 0x8000000000000000;
				new_shadow_flags = 1;
			}
			kprcb_ptr->KernelDirectoryTableBase = kernel_directory_table;
			new_directory_table &= 0x7FFFFFFFFFFFFFFF;

			new_shadow_flags |= (kprcb_ptr->ShadowFlags & 0xFFFFFFFD);
			if (new_apc_process->__flip_shadow_flags) {
				new_shadow_flags ^= 3;
			}
			kprcb_ptr->ShadowFlagsByte = new_shadow_flags;
			__asm STI
		}

		write_cr3(new_directory_table);

		new_stack = (uint64_t)next_thread->InitialStack;
		if (expect_chance(KVA_SHADOW_FLAG, true, 0.6)) {
			if (!(new_directory_table & 2)) {
				uintptr_t cr4 = read_cr4();
				cr4 ^= 0x80;
				write_cr4(cr4);
				cr4 ^= 0x80;
				write_cr4(cr4);
			}
			goto set_stackB;
		}
		goto set_stackA;
	}
	new_stack = (uint64_t)next_thread->InitialStack;

	if (expect_chance(!KVA_SHADOW_FLAG, true, 0.6)) {
set_stackA:
		kprcb_ptr->kpcr()->TssBase->Rsp0 = new_stack;
	}
	else {
set_stackB:
		kprcb_ptr->RspBaseShadow = new_stack;
	}
	kprcb_ptr->RspBase = new_stack;

	if (expect(__some_random_flag_dword_idk & 4, false)) {
		EtwTraceContextSwap(prev_thread, next_thread);
	}

	prev_thread->Running = false;

	// Handle state loading
	int32_t new_xsave_mask = next_thread->NpxState;
	new_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (new_xsave_mask) {
		const XSAVE_FORMATX<64>* state_save = next_thread->StateSaveArea;
		__asm__(
			"CDQ \n"
			"FXRSTOR (%[ptr]) \n"
			:
			: "a"(new_xsave_mask), [ptr]"r"(state_save)
			: clobber_list("rdx")
		);
	}

	if (expect(!next_thread->SystemThread, true)) {
		uint64_t teb = (uint64_t)next_thread->Teb;
		uint32_t low_teb = (uint32_t)teb + 0x2000;
		kprcb_ptr->kpcr()->Gdt32Base[10].set_base(low_teb);

		uint32_t data_segments = read_ds() & read_es() & read_gs();
		uint32_t data_segment_val = 0x2B;
		if (expect((uint16_t)data_segments != data_segment_val, false)) {
			write_ds(data_segment_val);
			write_es(data_segment_val);
			__asm__ volatile (
				"CLI \n"
				"SWAPGS \n"
			);
			write_gs(data_segment_val);
			__asm__ volatile (
				"SWAPGS \n"
				"STI \n"
			);
		}

		uint32_t teb_segment = read_fs();
		uint32_t teb_segment_val = 0x53;
		if (expect((uint16_t)teb_segment != teb_segment_val, false)) {
			write_fs(teb_segment_val);
		}

		uint64_t fs_base = *based_pointer<uint32_t>(next_thread, 0x84);
		fs_base |= (uint64_t)next_thread->Spare0 << 32;

		if (fs_base != low_teb) {
			wrfsbase64(fs_base);
		}

		kprcb_ptr->kpcr()->Used_Self = (TEBX<64>*)teb;

		if (next_thread->Header.UmsPrimary) {
			teb = (uint64_t)next_thread->Ucb->UmsTeb;
		}
		MSR_KERNEL_GS_BASE::write_raw(teb);
	}

	uint8_t* DpcRoutineActive = (uint8_t*)&kprcb_ptr->DpcRoutineActive;
	if (expect(*DpcRoutineActive, false)) {
		KeBugCheckEx(0xB8, (uintptr_t)prev_thread, (uintptr_t)next_thread, 0, 0);
	}

	++next_thread->ContextSwitches;

	if (next_thread->ApcState.KernelApcPending == true) {
		if (next_thread->SpecialApcDisable | irql_stack) {
			HalRequestSoftwareInterrupt(1);
		}
		else {
			return true;
		}
	}
	return false;
}

dllexport uint8_t regcall swap_context_new2(
	int, // RAX
	KIRQL irql, // RCX
	int, // RDX
	PTR64Z<KTHREADX<64>> prev_thread, // RDI
	PTR64Z<KTHREADX<64>> next_thread, // RSI
	int, // R8
	int, // R9
	int, // R10
	int, // R11
	int, // R12
	int, // R14
	int // R15
) {
	volatile KIRQL irql_stack = irql;

	register PTR64Z<KPRCBX<64>> kprcb_ptr asm("rbp");
	__asm__ volatile ("":"=r"(kprcb_ptr));

	uint8_t* thread_running = (uint8_t*)&next_thread->Running;

	_m_prefetchw(thread_running);
	while (expect(*thread_running, false)) {
		_mm_pause();
	}
	*thread_running = true;

	__asm CLI
	uint64_t timestamp = rdtsc();
	timestamp -= kprcb_ptr->StartCycles;
	kprcb_ptr->StartCycles += timestamp;
	kprcb_ptr->CycleTime += timestamp;
	if (kprcb_ptr->InterruptRequest != 0xFF) {
		kprcb_ptr->InterruptRequest = 0;
		if (next_thread != kprcb_ptr->IdleThread) {
			HalRequestSoftwareInterrupt(2);
		}
	}
	uint8_t thread_control_flags = next_thread->Header.ThreadControlFlags;
	if (expect(thread_control_flags & 5, false)) {
		if (thread_control_flags & 4) {
			KiBeginCounterAccumulation(next_thread, 1);
		}
		if (thread_control_flags & 1) {
			--kprcb_ptr->NestingLevel;
			__asm STI
			PsCheckThreadCpuQuota(kprcb_ptr, 1, next_thread);
			goto throttled_jump;
		}
	}
	--kprcb_ptr->NestingLevel;
	__asm STI
throttled_jump:
	++kprcb_ptr->KeContextSwitches;

	// Handle state saving
	int32_t prev_xsave_mask = prev_thread->NpxState; 
	prev_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (prev_xsave_mask) {
		XSAVE_FORMATX<64>* state_save = prev_thread->StateSaveArea;
		__asm__(
			"CDQ \n"
			"FXSAVE (%[ptr]) \n"
			:
			: "a"(prev_xsave_mask), [ptr]"r"(state_save)
			: clobber_list("rdx")
		);
		// x87 bit
		if (prev_xsave_mask & 1) {
			if (state_save->StatusWord.exception_summary) {
				__asm FNCLEX
			}
			__asm__(
				"FFREE %%ST(7) \n"
				"FILDL (%[ptr])"
				:
				: [ptr]"r"(state_save)
			);
		}
	}

	if (expect(!next_thread->SystemThread, true)) {
		uint64_t fs_base = rdfsbase64();
		*based_pointer<uint32_t>(prev_thread, 0x84) = fs_base;
		prev_thread->Spare0 = fs_base >> 32;
	}

	// Swap thread stacks
	prev_thread->KernelStack = (void*)rsp_reg;
	rsp_reg = (uint64_t)next_thread->KernelStack;

	if (prev_thread->Header.UmsPrimary) {
		uintptr_t usermode_gs_base = (uintptr_t)MSR_KERNEL_GS_BASE::read().address;
		prev_thread->Ucb->UmsTeb = (void*)__min(usermode_gs_base, MmUserProbeAddress);
	}

	KPROCESSX<64>* new_process = next_thread->Process;
	if (new_process != prev_thread->Process) {
		KiUpdateSpeculationControl(new_process);
	}

	uint8_t KVA_SHADOW = KiKvaShadow;

	if (!KVA_SHADOW) {
		if (new_process->LdtBaseAddress) {
			kprcb_ptr->kpcr()->GdtBase[7] = new_process->LdtSystemDescriptor;
			write_ldtr(0x70);
		}
	}

	bool KVA_SHADOW_FLAG = KVA_SHADOW & 1;

	uint64_t new_stack;

	KPROCESSX<64>* new_apc_process = next_thread->ApcState.Process;
	KPROCESSX<64>* prev_apc_process = prev_thread->ApcState.Process;
	if (new_apc_process != prev_apc_process) {
		uint32_t number = kprcb_ptr->Number;
		KeInterlockedClearProcessorAffinityEx(&prev_apc_process->ActiveProcessors, number);
		KeInterlockedSetProcessorAffinityEx(&new_apc_process->ActiveProcessors, number);

		// CR3 value
		uintptr_t new_directory_table = new_apc_process->DirectoryTableBase;

		if (expect_chance(KVA_SHADOW_FLAG, true, 0.4)) {
			__asm CLI
			uint32_t new_shadow_flags = 0;
			uintptr_t kernel_directory_table = new_directory_table;
			if (new_directory_table & 2) {
				kernel_directory_table |= 0x8000000000000000;
				new_shadow_flags = 1;
			}
			kprcb_ptr->KernelDirectoryTableBase = kernel_directory_table;
			new_directory_table &= 0x7FFFFFFFFFFFFFFF;

			new_shadow_flags |= (kprcb_ptr->ShadowFlags & 0xFFFFFFFD);
			if (new_apc_process->__flip_shadow_flags) {
				new_shadow_flags ^= 3;
			}
			kprcb_ptr->ShadowFlags = new_shadow_flags;
			__asm STI

			write_cr3(new_directory_table);
			//new_stack = (uint64_t)next_thread->InitialStack;

			if (!(new_directory_table & 2)) {
				uintptr_t cr4 = read_cr4();
				cr4 ^= 0x80;
				write_cr4(cr4);
				cr4 ^= 0x80;
				write_cr4(cr4);
			}
			goto set_stackB;
		}
		else {
			write_cr3(new_directory_table);
			//new_stack = (uint64_t)next_thread->InitialStack;
			goto set_stackA;
		}
	}

	if (expect_chance(!KVA_SHADOW_FLAG, true, 0.4)) {
set_stackA:
		new_stack = (uint64_t)next_thread->InitialStack;
		kprcb_ptr->kpcr()->TssBase->Rsp0 = new_stack;
	}
	else {
set_stackB:
		new_stack = (uint64_t)next_thread->InitialStack;
		kprcb_ptr->RspBaseShadow = new_stack;
	}
	kprcb_ptr->RspBase = new_stack;

	if (expect(__some_random_flag_dword_idk & 4, false)) {
		EtwTraceContextSwap(prev_thread, next_thread);
	}

	prev_thread->Running = false;

	// Handle state loading
	int32_t new_xsave_mask = next_thread->NpxState;
	new_xsave_mask &= 0xFFFFFFFD; // Ignore SSE bit?
	if (new_xsave_mask) {
		const XSAVE_FORMATX<64>* state_save = next_thread->StateSaveArea;
		__asm__(
			"CDQ \n"
			"FXRSTOR (%[ptr]) \n"
			:
			: "a"(new_xsave_mask), [ptr]"r"(state_save)
			: clobber_list("rdx")
		);
	}

	if (expect(!next_thread->SystemThread, true)) {
		uint64_t teb = (uint64_t)next_thread->Teb;
		uint32_t low_teb = (uint32_t)teb + 0x2000;
		kprcb_ptr->kpcr()->Gdt32Base[10].set_base(low_teb);

		uint32_t teb_segment = read_fs();
		uint32_t teb_segment_val = 0x53;
		if (expect((uint16_t)teb_segment != teb_segment_val, false)) {
			write_fs(teb_segment_val);
		}

		uint64_t fs_base = *based_pointer<uint32_t>(next_thread, 0x84);
		fs_base |= (uint64_t)next_thread->Spare0 << 32;

		if (fs_base != low_teb) {
			wrfsbase64(fs_base);
		}

		kprcb_ptr->kpcr()->Used_Self = (TEBX<64>*)teb;

		if (next_thread->Header.UmsPrimary) {
			teb = (uint64_t)next_thread->Ucb->UmsTeb;
		}

		uint32_t data_segments = read_ds();
		data_segments &= read_es();
		data_segments &= read_gs();
		uint32_t data_segment_val = 0x2B;
		if (expect((uint16_t)data_segments != data_segment_val, false)) {
			write_ds(data_segment_val);
			write_es(data_segment_val);
			__asm__ volatile (
				"CLI \n"
				"SWAPGS \n"
			);
			write_gs(data_segment_val);
			wrgsbase64(teb);
			__asm__ volatile (
				"SWAPGS \n"
				"STI \n"
			);
		}
		else {
			MSR_KERNEL_GS_BASE::write_raw(teb);
		}
	}

	uint8_t* DpcRoutineActive = (uint8_t*)&kprcb_ptr->DpcRoutineActive;
	if (expect(*DpcRoutineActive, false)) {
		KeBugCheckEx(0xB8, (uintptr_t)prev_thread, (uintptr_t)next_thread, 0, 0);
	}

	++next_thread->ContextSwitches;

	if (next_thread->ApcState.KernelApcPending == true) {
		if (next_thread->SpecialApcDisable | irql_stack) {
			HalRequestSoftwareInterrupt(1);
		}
		else {
			return true;
		}
	}
	return false;
}

naked uint8_t swap_context_asm() {
	__asm__(
		INTEL_64_DIRECTIVE
		"PUSH RBP \n"
		"PUSH RCX \n"
		"SUB RSP, 0x28 \n"
		"MOV RBP, RBX \n"
		"LEA RAX, [RSI + 0x49] \n"
		"PREFETCHW [RSI + 0x49] \n"
		"CMP BYTE PTR [RSI + 0x49], 0 \n"
		"JNE 1f \n"
	"2: \n"
		"MOV BYTE PTR [RAX], 1 \n"
		"CLI \n"
		"RDTSC \n"
		"SHL RDX, 32 \n"
		"OR RDX, RAX \n"
		"MOV RAX, RDX \n"
		"SUB RAX, [RBP + 0x4F40] \n"
		"MOV [RBP + 0x4F40], RDX \n"
		"ADD [RBP + 0x4F78], RAX \n"
		"CMP BYTE PTR [RBP + 6], 0xFF \n"
		"JE 3f \n"
		"MOV BYTE PTR [RBP + 6], 0 \n"
		"CMP RSI, [RBP + 0x18] \n"
		"JE 3f \n"
		"MOV ECX, 2 \n"
		"CALL QWORD PTR [RIP - HalRequestSoftwareInterrupt] \n"
	"3: \n"
		"MOVZX EBX, BYTE PTR [RSI + 2] \n"
		"TEST BL, 5 \n"
		"JNZ 3f \n"
	"4: \n"
		"DEC BYTE PTR [RBP + 0x20] \n"
		"STI \n"
	"5: \n"
		"INC DWORD PTR [RBP + 0x20BC] \n"
		"MOVSX EAX, BYTE PTR [RDI + 0x165] \n"
		"AND EAX, 0xFFFFFFFD \n"
		"JZ 6f \n"
		"MOV RCX, [RDI + 0x358] \n"
		"CDQ \n"
		"FXSAVE [RCX] \n"
		"TEST AL, 1 \n"
		"JZ 6f \n"
		"TEST BYTE PTR [RCX + 2], 0x80 \n"
		"JZ 7f \n"
		"FNCLEX \n"
	"7: \n"
		"FFREE ST(7) \n"
		"FILD DWORD PTR [RCX] \n"
	"6: \n"
		"TEST BYTE PTR [RDI + 0x4D], 0x20 \n"
		"JNZ 6f \n"
		"RDFSBASE RAX \n"
		"MOV [RDI + 0x84], EAX \n"
		"SHR RAX, 32 \n"
		"MOV [RDI + 0x104], EAX \n"
	"6: \n"
		"MOV [RDI + 0x38], RSP \n"
		"MOV RSP, [RSI + 0x38] \n"
		"CMP BYTE PTR [RDI + 3], 0 \n"
		"JNS 6f \n"
		"MOV ECX, 0xC0000102 \n"
		"RDMSR \n"
		"SHL RDX, 32 \n"
		"OR RDX, RAX \n"
		"MOV RAX, [RIP - MmUserProbeAddress] \n"
		"CMP RAX, RDX \n"
		"CMOVB RDX, RAX \n"
		"MOV RAX, [RDI + 0x1B8] \n"
		"MOV [RAX + 0x80], RDX \n"
	"6: \n"
		"MOV R14, [RSI + 0x210] \n"
		"CMP R14, [RDI + 0x210] \n"
		"JE 6f \n"
		"MOV RCX, R14 \n"
		"CALL KiUpdateSpeculationControl \n"
	"6: \n"
		"MOVZX EBX, BYTE PTR [RIP - KiKvaShadow] \n"
		"TEST BL, BL \n"
		"JNZ 6f \n"
		"CMP QWORD PTR [R14 + 0x120], 0 \n"
		"JE 6f \n"
		"MOV RAX, [RBP - 0x180] \n"
		"MOVUPS XMM6, [R14 + 0x110] \n"
		"MOVUPS [RAX + 0x70], XMM6 \n"
		"MOV EAX, 0x70 \n"
		"LLDT AX \n"
	"6: \n"
		"MOV R15, [RSI + 0x70] \n"
		"MOV RCX, [RDI + 0x70] \n"
		"CMP R15, RCX \n"
		"JE 6f \n"
		"MOV EDX, [RBP + 0x24] \n"
		"ADD RCX, 0x88 \n"
		"MOV R14D, EDX \n"
		"CALL KeInterlockedClearProcessorAffinityEx \n"
		"LEA RCX, [R15 + 0x88] \n"
		"MOV EDX, R14D \n"
		"CALL KeInterlockedSetProcessorAffinityEx \n"
		"MOV RAX, [R15 + 0x28] \n"
		"TEST BL, 1 \n"
		"JNZ 7f \n"
		"MOV CR3, RAX \n"
		"JMP 8f \n"
		"INT3 \n"
		"INT3 \n"
		"INT3 \n"

	//".align 16, 0xCC \n"
	"6: \n"
		"TEST BL, 1 \n"
		"JNZ 9f \n"
	"8: \n"
		"MOV RAX, [RSI + 0x28] \n"
		"MOV RCX, [RBP - 0x178] \n"
		"MOV [RCX + 4], RAX \n"
		"MOV [RBP + 0x28], RAX \n"
		"TEST BYTE PTR [RIP - __some_random_flag_dword_idk], 4 \n"
		"JNZ 10f \n"
	"8: \n"
		"MOV BYTE PTR [RDI + 0x49], 0 \n"
		"MOVSX EAX, BYTE PTR [RSI + 0x165] \n"
		"AND EAX, 0xFFFFFFFD \n"
		"JZ 6f \n"
		"MOV RCX, [RSI + 0x358] \n"
		"CDQ \n"
		"FXRSTOR [RCX] \n"
	"6: \n"
		"TEST BYTE PTR [RSI + 0x4D], 0x20 \n"
		"JNZ 6f \n"
		"MOV RAX, [RSI + 0xB8] \n"
		"LEA EBX, [RAX + 0x2000] \n"
		"MOV RDX, [RBP - 0x180] \n"
		"MOV ECX, EBX \n"
		"MOV [RDX + 0x52], CX \n"
		"SHR ECX, 16 \n"
		"MOV [RDX + 0x54], CL \n"
		"MOV [RDX + 0x57], CH \n"

		"MOV EDX, FS \n"
		"CMP EDX, 0x53 \n"
		"JNE bad_tls_seg \n"
	"good_tls_seg: \n"

		"MOV ECX, [RSI + 0x84] \n"
		"MOV EDX, [RSI + 0x104] \n"
		"SHL RDX, 32 \n"
		"OR RDX, RCX \n"
		"CMP RDX, RBX \n"
		"JE skip_fs_base \n"
		"WRFSBASE RDX \n"
	"skip_fs_base: \n"
		
		"MOV [RBP - 0x150], RAX \n"
		"CMP BYTE PTR [RSI + 3], 0 \n"
		"JNS new_thread_not_ums \n"
		"MOV RAX, [RSI + 0x1B8] \n"
		"MOV RAX, [RAX + 0x80] \n"
	"new_thread_not_ums: \n"

		"MOV EDX, DS \n"
		"MOV ECX, ES \n"
		"AND EDX, ECX \n"
		"MOV ECX, GS \n"
		"AND EDX, ECX \n"
		"CMP EDX, 0x2B \n"
		"JNE bad_data_seg \n"
		"MOV RDX, RAX \n"
		"SHR RDX, 32 \n"
		"MOV ECX, 0xC0000102 \n"
		"WRMSR \n"
	"6: \n"
		"CMP BYTE PTR [RBP + 0x21DA], 0 \n"
		"JNE bugcheck \n"
	"no_bugcheck: \n"
		"INC DWORD PTR [RSI + 0x134] \n"
		"XOR EAX, EAX \n"
		"CMP BYTE PTR [RSI + 0x79], 1 \n"
		"JNE return \n"
		"MOV AL, 1 \n"
		"MOVZX EDX, BYTE PTR [RSP + 28] \n"
		"OR DX, WORD PTR [RSI + 0x1C6] \n"
		"JZ return \n"
		"MOV ECX, 1 \n"
		"CALL QWORD PTR [RIP - HalRequestSoftwareInterrupt] \n"
		"XOR EAX, EAX \n"
	"return: \n"
		"MOV RBX, RBP \n"
		"ADD RSP, 0x30 \n"
		"POP RBP \n"
		"RET \n"
		"INT3 \n"

	".align 16, 0xCC \n"
	"7: \n"
		"CLI \n"
		"MOV ECX, EAX \n"
		"AND ECX, 2 \n"
		"MOV EDX, ECX \n"
		"SHL RDX, 62 \n"
		"OR RDX, RAX \n"
		"SHR ECX, 1 \n"
		"MOV [RBP + 0x5E80], RDX \n"
		//"MOV RDX, 0x7FFFFFFFFFFFFFFF \n"
		//"AND RAX, RDX \n"
		"BTR RAX, 63 \n"
		"MOVZX EDX, BYTE PTR [RBP + 0x5E98] \n"
		"AND EDX, 0xFFFFFFFD \n"
		"OR EDX, ECX \n"
		"MOV R8D, EDX \n"
		"XOR EDX, 3 \n"
		"TEST BYTE PTR [R14 + 0x118], 1 \n"
		"CMOVZ EDX, R8D \n"
		"MOV [RBP + 0x5E98], DL \n"
		"STI \n"
		"MOV CR3, RAX \n"
		"TEST CL, CL \n"
		"JNZ 9f \n"
		"MOV RAX, CR4 \n"
		"MOV RCX, RAX \n"
		"XOR RCX, 0x80 \n"
		"MOV CR4, RCX \n"
		"MOV CR4, RAX \n"
	"9: \n"
		"MOV RAX, [RSI + 0x28] \n"
		"MOV [RBP + 0x5E88], RAX \n"
		"MOV [RBP + 0x28], RAX \n"
		"TEST BYTE PTR [RIP - __some_random_flag_dword_idk], 4 \n"
		"JZ 8b \n"
	"10: \n"
		"MOV RCX, RDI \n"
		"MOV RDX, RSI \n"
		"CALL EtwTraceContextSwap \n"
		"JMP 8b \n"
		"INT3 \n"

	".align 16, 0xCC \n"
	"1: \n"
		"PAUSE \n"
		"CMP BYTE PTR [RAX], 0 \n"
		"JE 2b \n"
		"JMP 1b \n"
		"INT3 \n"

	".align 16, 0xCC \n"
	"3: \n"
		"TEST BL, 4 \n"
		"JZ 1f \n"
		"MOV RCX, RSI \n"
		"MOV DL, 1 \n"
		"CALL KiBeginCounterAccumulation \n"
	"1: \n"
		"TEST BL, 1 \n"
		"JZ 4b \n"
		"DEC BYTE PTR [RBP + 0x20] \n"
		"STI \n"
		"MOV RCX, RBP \n"
		"MOV DL, 1 \n"
		"MOV R8, RSI \n"
		"CALL PsCheckThreadCpuQuota \n"
		"JMP 5b \n"
		"INT3 \n"

	".align 16, 0xCC \n"
	"bad_tls_seg: \n"
		"MOV EDX, 0x53 \n"
		"MOV FS, EDX \n"
		"JMP good_tls_seg \n"
		"INT3 \n"

	".align 16, 0xCC \n"
	"bad_data_seg: \n"
		"MOV EDX, 0x2B \n"
		"MOV DS, EDX \n"
		"MOV ES, EDX \n"
		"CLI \n"
		"SWAPGS \n"
		"MOV GS, EDX \n"
		"WRGSBASE RAX \n"
		"SWAPGS \n"
		"STI \n"
		"CMP BYTE PTR [RBP + 0x21DA], 0 \n"
		"JE no_bugcheck \n"
	"bugcheck: \n"
		"XOR R9D, R9D \n"
		"MOV [RSP + 0x20], R9 \n"
		"MOV ECX, 0xB8 \n"
		"MOV RDX, RDI \n"
		"MOV R8, RSI \n"
		"CALL KeBugCheckEx \n"
		"INT3 \n"
	);
}

void ki_swap_context(
	PTR64Z<KTHREADX<64>> prev_thread, // RCX
	PTR64Z<KTHREADX<64>> next_thread // RDX
) {

}

dllexport void KiSystemStartup(
	PTR64Z<LOADER_PARAMETER_BLOCKX<64>> loader_block
) {
	KPRCBX<64>* kprcb_ptr = (KPRCBX<64>*)loader_block->Prcb;
	kprcb_ptr = kprcb_ptr ? kprcb_ptr : &static_kprcb;
	loader_block->Prcb = (uintptr_t)kprcb_ptr;
	KPCRX<64>* kpcr_ptr = kprcb_ptr->kpcr();

	kpcr_ptr->Self = kpcr_ptr;
	kpcr_ptr->CurrentPrcb = kprcb_ptr;

	store_cr0(kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Cr0);
	store_cr2(kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Cr2);
	store_cr3(kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Cr3);
	store_cr4(kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Cr4);

	store_gdtr(&kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Gdtr.Limit);
	KGDTENTRY64* gdt_ptr = (KGDTENTRY64*)kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Gdtr.Base;
	kpcr_ptr->GdtBase = gdt_ptr;
	store_idtr(&kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Idtr.Limit);
	KIDTENTRY64* idt_ptr = (KIDTENTRY64*)kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Idtr.Base;
	kpcr_ptr->IdtBase = idt_ptr;
	store_tr(kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Tr);
	store_ldtr(kpcr_ptr->Prcb.ProcessorState.SpecialRegisters.Ldtr);

	kpcr_ptr->Prcb.MxCsr = 0x1F80;
	load_mxcsr<SSE_Encoding>(kpcr_ptr->Prcb.MxCsr);
	if (kprcb_ptr->Number == 0) {
		gdt_ptr[5].LimitLow = 0x3C00;
	}

	seg_t data_segment = 0x2B;
	write_ds(data_segment);
	write_es(data_segment);
	seg_t tls_segment = 0x53;
	write_fs(tls_segment);
	write_ldtr(0);

	// Get TSS ptr from GDT
	((uint16_t*)&kpcr_ptr->TssBase)[0] = gdt_ptr[4].BaseLow;
	((uint8_t*)&kpcr_ptr->TssBase)[2] = gdt_ptr[4].BaseMiddle;
	((uint8_t*)&kpcr_ptr->TssBase)[3] = gdt_ptr[4].BaseHigh;
	((uint32_t*)&kpcr_ptr->TssBase)[1] = gdt_ptr[4].BaseUpper;
	
	MSR_GS_BASE::write_raw((uint64_t)kpcr_ptr);
	MSR_KERNEL_GS_BASE::write_raw((uint64_t)kpcr_ptr);


}


dllexport bool VgaIsPresent() {
	uint32_t vga_register_base = VgaRegisterBase;
	uint8_t A = port_in<uint8_t>(vga_register_base + 0x3CE);
	port_out<uint8_t>(4, vga_register_base + 0x3CE);
	uint8_t B = port_in<uint8_t>(vga_register_base + 0x3CE);
	if ((B & 0xF) != 4) {
		return false;
	}
	uint8_t C = port_in<uint8_t>(vga_register_base + 0x3CF);
	port_out<uint8_t>(3, vga_register_base + 0x3CF);
	uint8_t D = port_in<uint8_t>(vga_register_base + 0x3CF);
	if (D != 3) {
		port_out<uint8_t>(0, vga_register_base + 0x3CF);
		return false;
	}
	port_out<uint8_t>(8, vga_register_base + 0x3CE);
	uint8_t E = port_in<uint8_t>(vga_register_base + 0x3CE);
	if ((E & 0xF) != 8) {
		port_out<uint8_t>(4, vga_register_base + 0x3CE);
		port_out<uint8_t>(0, vga_register_base + 0x3CF);
		return false;
	}
	uint8_t F = port_in<uint8_t>(vga_register_base + 0x3CF);
	uint8_t G = 0xBB;
	do {
		port_out<uint8_t>(G, vga_register_base + 0x3CF);
		uint8_t H = port_in<uint8_t>(vga_register_base + 0x3CF);
		if (H != G) {
			port_out<uint8_t>(0xFF, vga_register_base + 0x3CF);
			port_out<uint8_t>(4, vga_register_base + 0x3CE);
			port_out<uint8_t>(0, vga_register_base + 0x3CF);
			return false;
		}
	} while (G >>= 1);
	port_out<uint8_t>(4, vga_register_base + 0x3CE);
	uint8_t I = port_in<uint8_t>(vga_register_base + 0x3CF);
	if (I != 3) {
		port_out<uint8_t>(0, vga_register_base + 0x3CF);
		port_out<uint8_t>(8, vga_register_base + 0x3CE);
		port_out<uint8_t>(0, vga_register_base + 0x3CF);
		return false;
	}
	port_out<uint8_t>(C, vga_register_base + 0x3CF);
	port_out<uint8_t>(8, vga_register_base + 0x3CE);
	port_out<uint8_t>(F, vga_register_base + 0x3CF);
	port_out<uint8_t>(A, vga_register_base + 0x3CE);
	uint8_t J = port_in<uint8_t>(vga_register_base + 0x3C4);
	port_out<uint8_t>(4, vga_register_base + 0x3C4);
	uint8_t K = port_in<uint8_t>(vga_register_base + 0x3C4);
	if ((K & 7) != 4) {
		return false;
	}
	uint8_t L = port_in<uint8_t>(vga_register_base + 0x3C5);
	port_out<uint16_t>(0x100, vga_register_base + 0x3C4);
	port_out<uint8_t>(4, vga_register_base + 0x3C4);
	uint8_t M = L ^ 8;
	port_out<uint8_t>(M, vga_register_base + 0x3C5);
	uint8_t N = port_in<uint8_t>(vga_register_base + 0x3C5);
	if (N != M) {
		port_out<uint8_t>(2, vga_register_base + 0x3C5);
		port_out<uint16_t>(0x300, vga_register_base + 0x3C4);
		return false;
	}
	port_out<uint8_t>(L, vga_register_base + 0x3C5);
	port_out<uint16_t>(0x300, vga_register_base + 0x3C4);
	port_out<uint8_t>(J, vga_register_base + 0x3C4);
	return true;
}