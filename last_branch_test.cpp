#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "windows.h"
#include "windows_structs.h"

#include "zero/util.h"

#define MakeOffsetPtr(ptr, type, base)		based_pointer<type>((ptr),offsetof(type,base))
#define OffsetTypePtr(ptr, type, base)      based_pointer<type>((ptr),-offsetof(type,base))
#define OffsetPtr(ptr, type, field, base)   based_pointer<typeof(type::field)>((ptr),offsetof(type,field)-offsetof(type,base))
#define OffsetField(ptr, type, field, base) (*OffsetPtr(ptr,type,field,base))

static uint64_t last_branch_to;
static uint64_t last_branch_from;
static uint64_t last_exception_to;
static uint64_t last_exception_from;

LONG WINAPI log_branch_records(LPEXCEPTION_POINTERS lpEI) {
#if __x86_64__
	last_branch_to = lpEI->ContextRecord->LastBranchToRip;
	last_branch_from = lpEI->ContextRecord->LastBranchFromRip;
	last_exception_to = lpEI->ContextRecord->LastExceptionToRip;
	last_exception_from = lpEI->ContextRecord->LastExceptionFromRip;

	lpEI->ContextRecord->Rip += 7;
#else
	/*
	CONTEXTX<64>* x64_context = (CONTEXTX<64>*)0x731B4690;
	last_branch_to = x64_context->LastBranchToRip;
	last_branch_from = x64_context->LastBranchFromRip;
	last_exception_to = x64_context->LastExceptionToRip;
	last_exception_from = x64_context->LastExceptionFromRip;
	*/
	lpEI->ContextRecord->Eip += 7;
#endif

	return EXCEPTION_CONTINUE_EXECUTION;
}

gnu_noinline void debug_test_func() {
bs_label_testA:
	__asm__ volatile(
		"jmp 0f \n"
		"int3 \n"
	"0: \n"
		//"nop"
	);
bs_label_testB:
	__asm {
		_emit 0x8B
		_emit 0x04
		_emit 0x25
		_emit 0
		_emit 0
		_emit 0
		_emit 0
	}

	printf(
		"BranchTo:   %llX\n"
		"BranchFrom: %llX\n"
		"ExceptTo:   %llX\n"
		"ExceptFrom: %llX\n"
		"LabelTestA: %zX\n"
		"LabelTestB: %zX\n"
		, last_branch_to
		, last_branch_from
		, last_exception_to
		, last_exception_from
		, &&bs_label_testA
		, &&bs_label_testB
	);
}

void last_branch_testing() {
	AddVectoredExceptionHandler(0, log_branch_records);

	CONTEXT context;
	context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(GetCurrentThread(), &context);

	printf("Testing without DR7 bits... %zX\n", context.Dr7);
	debug_test_func();


	context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	context.Dr7 |= 0x100;

	SetThreadContext(GetCurrentThread(), &context);

	context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(GetCurrentThread(), &context);

	if (context.Dr7 & 0x100) {
		printf("\nTesting with DR7 bits... %zX\n", context.Dr7);
		debug_test_func();
	} else {
		printf("\nSetting DR7 failed %zX", context.Dr7);
	}
}


constexpr wchar_t NTDLL_NAME[] asm("NTDLL_NAME") = L"ntdll.dll";
constexpr wchar_t WOW64_NAME[] asm("WOW64_NAME") = L"wow64.dll";
constexpr char FUNC_NAME[] asm("FUNC_NAME") = "Wow64PrepareForException";
constexpr char PROTECT_NAME[] asm("PROTECT_NAME") = "NtProtectVirtualMemory";

void prepare_for_exception_hook_original(EXCEPTION_RECORDX<64>* exception_record, CONTEXTX<64>* context);
NTSTATUS NTAPI NtProtectVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection);
using prepare_t = decltype(prepare_for_exception_hook_original);
using protect_t = decltype(NtProtectVirtualMemoryShim);
prepare_t* original_exception_prepare;

dllexport gnu_noinline naked
void prepare_for_exception_hook_asm(
	EXCEPTION_RECORDX<64>* exception_record,
	CONTEXTX<64>* context
) {
	__asm__ volatile(
		INTEL_64_DIRECTIVE
		"MOV RAX, [RDX+0x4B0] \n"
		"MOV [%c[branch_to]], RAX \n"
		"MOV RAX, [RDX+0x4B8] \n"
		"MOV [%c[branch_from]], RAX \n"
		"MOV RAX, [RDX+0x4C0] \n"
		"MOV [%c[except_to]], RAX \n"
		"MOV RAX, [RDX+0x4C8] \n"
		"MOV [%c[except_from]], RAX \n"
		"JMP QWORD PTR [%c[func_ptr]] \n"
		:
		:
			[branch_to]"i"(&last_branch_to),
			[branch_from]"i"(&last_branch_from),
			[except_to]"i"(&last_exception_to),
			[except_from]"i"(&last_exception_from),
			[func_ptr]"i"(&original_exception_prepare)
	);
}

/*
dllexport gnu_noinline naked
void hook_wow_exception_addr_asm(
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV RAX, GS:[0x60] \n"
		"MOV RAX, [RAX+0x18] \n"
		"MOV R9, [RAX+0x10] \n"
		"ADD RAX, 0x10 \n"
		"XOR R8D, R8D \n"
		"XOR EDX, EDX \n"
		"JMP 1f \n"
	"3: \n"
		"MOV RDI, [R9+0x60] \n"
		"MOV ECX, 18 \n"
		"MOV ESI, OFFSET %c[ntdll_name] \n"
		"repe cmpsb \n"
		"JNE 3f \n"
		"MOV R8, [R9+0x30] \n"
		"TEST RDX, RDX \n"
		"JNZ 4f \n"
	".align 16 \n"
	"2: \n"
		"MOV R9, [R9] \n"
		"CMP R9, RAX \n"
		"JE 9f \n"
	"1: \n"
		"CMP WORD PTR [R9+0x58], 18 \n"
		"JNE 2b \n"
		"TEST R8, R8 \n"
		"JZ 3b \n"
	"3: \n"
		"TEST RDX, RDX \n"
		"JNZ 2b \n"
		"MOV RDI, [R9+0x60] \n"
		"MOV ESI, OFFSET %c[wow64_name] \n"
		"MOV ECX, 18 \n"
		"repe cmpsb \n"
		"JNE 2b \n"
		"MOV RDX, [R9+0x30] \n"
		"TEST R8, R8 \n"
		"JZ 2b \n"
	"4: \n"
		"MOV EAX, [RDX+0x3C] \n"
		"MOV EAX, [RDX*1+RAX+0x88] \n"
		"MOV R9D, [RDX*1+RAX+0x18] \n"
		"MOV R10D, [RDX*1+RAX+0x20] \n"
		"ADD R10, RDX \n"

	".align 16 \n"
	"1: \n"
		"SUB R9D, 1 \n"
		"JB 9f \n"
		"MOV EDI, [R9*4+R10] \n"
		"ADD RDI, RDX \n"
		"MOV ESI, OFFSET %c[func_name] \n"
		"MOV ECX, 25 \n"
		"repe cmpsb \n"
		"JNE 1b \n"

		"MOV ECX, [RAX*1+RDX+0x1C] \n"
		"MOV EAX, [RAX*1+RDX+0x24] \n"
		"ADD RAX, RDX \n"
		"MOVZX EAX, WORD PTR [R9*2+RAX] \n"
		"ADD RCX, RDX \n"
		"MOV EAX, [RAX*4+RCX] \n"
		"ADD RAX, RDX \n"
		"MOV ECX, [R8+0x3C] \n"
		"LEA RDI, [R8*1+RCX] \n"
		"MOVZX EDX, WORD PTR [R8*1+RCX+0x14] \n"
		"MOVZX ESI, WORD PTR [R8*1+RCX+0x6] \n"
		"LEA RDX, [RDI*1+RDX+0x20] \n"
		"JMP 1f \n"

	".align 16 \n"
	"2: \n"
		"ADD RDX, 0x28 \n"
		"DEC ESI \n"
		"JZ 9f \n"
	"1: \n"
		"MOV ECX, [RDX+0x1C] \n"
		"AND ECX, 0xE0000000 \n"
		"CMP ECX, 0xC0000000 \n"
		"JNE 2b \n"
		"MOV ECX, [RDX] \n"
		"MOV EDI, [RDX+4] \n"
		"ADD RDI, R8 \n"
		"SHR ECX, 3 \n"
		"repne scasq \n"
		"JNE 2b \n"
		"MOV [%c[func_ptr]], RAX \n"
		"MOV QWORD PTR [RDI-8], OFFSET %c[new_func] \n"
	"9: \n"
		"RETF"
		:
		:
		[ntdll_name]"i"(&NTDLL_NAME),
		[wow64_name]"i"(&WOW64_NAME),
		[func_name]"i"(&FUNC_NAME),
		[func_ptr]"i"(&original_exception_prepare),
		[new_func]"i"(&prepare_for_exception_hook_asm)
	);
}
*/

dllexport gnu_noinline naked
void hook_wow_exception_addr_asm2(
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"PUSH RSI \n"
		"PUSH RDI \n"
		"PUSH RBX \n"
		"PUSH RBP \n"
		"MOV EBP, ESP \n"
		"SUB ESP, 0x40 \n"
		"AND ESP, -0x10 \n"
		"MOV RAX, GS:[0x60] \n"
		"MOV R8, [RAX+0x18] \n"
		"MOV RBX, [R8+0x10] \n"
		"ADD R8, 0x10 \n"
		"XOR EAX, EAX \n"
		"XOR EDX, EDX \n"
		"MOV R11D, -1 \n"
		"JMP 1f \n"
	"3: \n"
		"MOV RDI, [RBX+0x60] \n"
		"MOV ECX, 18 \n"
		"MOV ESI, OFFSET %c[ntdll_name] \n"
		"repe cmpsb \n"
		"JNE 3f \n"
		"MOV RDX, [RBX+0x30] \n"
		"TEST RAX, RAX \n"
		"JNZ 4f \n"
	".align 16 \n"
	"2: \n"
		"MOV RBX, [RBX] \n"
		"CMP RBX, R8 \n"
		"JE 9f \n"
	"1: \n"
		"CMP WORD PTR [RBX+0x58], 18 \n"
		"JNE 2b \n"
		"TEST RDX, RDX \n"
		"JZ 3b \n"
	"3: \n"
		"TEST RAX, RAX \n"
		"JNZ 2b \n"
		"MOV RDI, [RBX+0x60] \n"
		"MOV ECX, 18 \n"
		"MOV ESI, OFFSET %c[wow64_name] \n"
		"repe cmpsb \n"
		"JNE 2b \n"
		"MOV RAX, [RBX+0x30] \n"
		"TEST RDX, RDX \n"
		"JZ 2b \n"
	"4: \n"

		"MOV ECX, [RAX+0x3C] \n"
		"MOV EBX, [RAX*1+RCX+0x88] \n"
		"MOV R8D, [RAX*1+RBX+0x18] \n"
		"MOV R9D, [RAX*1+RBX+0x20] \n"
		"ADD R9, RAX \n"

	".align 16 \n"
	"1: \n"
		"SUB R8D, 1 \n"
		"JB 9f \n"
		"MOV EDI, [R8*4+R9] \n"
		"ADD RDI, RAX \n"
		"MOV ECX, 25 \n"
		"MOV ESI, OFFSET %c[func_name] \n"
		"repe cmpsb \n"
		"JNE 1b \n"

		"MOV ECX, [RAX*1+RBX+0x1C] \n"
		"MOV ESI, [RAX*1+RBX+0x24] \n"
		"ADD RSI, RAX \n"
		"MOVZX ESI, WORD PTR [R8*2+RSI] \n"
		"ADD RCX, RAX \n"
		"MOV ESI, [RSI*4+RCX] \n"
		"ADD RAX, RSI \n"

		"MOV EBX, [RDX+0x3C] \n"
		"MOV R10D, [RDX*1+RBX+0x88] \n"
		"ADD RBX, RDX \n"
		"MOV R8D, [RDX*1+R10+0x18] \n"
		"MOV R9D, [RDX*1+R10+0x20] \n"
		"ADD R9, RDX \n"

	".align 16 \n"
	"1: \n"
		"SUB R8D, 1 \n"
		"JB 9f \n"
		"MOV EDI, [R8*4+R9] \n"
		"ADD RDI, RDX \n"
		"MOV ECX, 23 \n"
		"MOV ESI, OFFSET %c[protect_name] \n"
		"repe cmpsb \n"
		"JNE 1b \n"

		"MOV ECX, [RDX*1+R10+0x1C] \n"
		"MOV R9D, [RDX*1+R10+0x24] \n"
		"ADD R9, RDX \n"
		"MOVZX R9D, WORD PTR [R8*2+R9] \n"
		"ADD RCX, RDX \n"
		"MOV R10D, [R9*4+RCX] \n"
		"ADD R10, RDX \n"

		"MOVZX ECX, WORD PTR [RBX+0x14] \n"
		"MOVZX ESI, WORD PTR [RBX+0x6] \n"
		"LEA RBX, [RCX*1+RBX+0x20] \n"
		"JMP 1f \n"
		"INT3 \n"
		"INT3 \n"
		"INT3 \n"

	".align 16 \n"
	"2: \n"
		"ADD RBX, 0x28 \n"
		"DEC ESI \n"
		"JZ 9f \n"
	"1: \n"
		"MOVZX ECX, BYTE PTR [RBX+0x1F] \n"
		"AND CL, 0x60 \n"
		"CMP CL, 0x40 \n"
		"JNE 2b \n"
		"MOV ECX, [RBX] \n"
		"MOV EDI, [RBX+4] \n"
		"ADD RDI, RDX \n"
		"SHR ECX, 3 \n"
		"repne scasq \n"
		"JNE 2b \n"
		"MOV RBX, RAX \n"
		"ADD RDI, -8 \n"
		"MOV RSI, R10 \n"
		"MOV [RSP+0x38], RDI \n"
		"MOV QWORD PTR [RSP+0x30], 8 \n"
		"LEA EAX, [RSP+0x2C] \n"
		"MOV [RSP+0x20], RAX \n"
		"LEA EDX, [RSP+0x38] \n"
		"LEA R8D, [RSP+0x30] \n"
		"MOV RCX, -1 \n"
		"MOV R9D, 4 \n"
		"CALL RSI \n"
		"TEST EAX, EAX \n"
		"JS 8f \n"
		"MOV [%c[func_ptr]], RBX \n"
		"MOV QWORD PTR [RDI], OFFSET %c[new_func] \n"
		"MOV R9D, [RSP+0x2C] \n"
		"LEA EAX, [RSP+0x2C] \n"
		"MOV [RSP+0x20], RAX \n"
		"LEA EDX, [RSP+0x38] \n"
		"LEA R8D, [RSP+0x30] \n"
		"MOV RCX, -1 \n"
		"CALL RSI \n"
		"XOR R11D, R11D \n"
	"9: \n"
		"MOV EAX, R11D \n"
	"8: \n"
		"LEAVE \n"
		"POP RBX \n"
		"POP RDI \n"
		"POP RSI \n"
		"RETF \n"
		:
		:
		[ntdll_name]"i"(&NTDLL_NAME),
		[wow64_name]"i"(&WOW64_NAME),
		[func_name]"i"(&FUNC_NAME),
		[protect_name]"i"(&PROTECT_NAME),
		[func_ptr]"i"(&original_exception_prepare),
		[new_func]"i"(&prepare_for_exception_hook_asm)
	);
}

uint64_t original_prepare_addr;
uint64_t nt_virtual_protect_addr;
uint64_t ntdll_pointer_addr;

dllexport gnu_noinline naked
void hook_wow_exception_addr_asm3A(
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV RAX, GS:[0x60] \n"
		"MOV R9, [RAX+0x18] \n"
		"MOV RDX, [R9+0x10] \n"
		"ADD R9, 0x10 \n"
		"XOR EAX, EAX \n"
		"XOR R8D, R8D \n"
		"XOR R11D, R11D \n"
		"JMP 1f \n"
		"INT3 \n"
	"3: \n"
		"MOV RDI, [RDX+0x60] \n"
		"MOV ECX, 18 \n"
		"MOV ESI, OFFSET %c[ntdll_name] \n"
		"repe cmpsb \n"
		"JNE 3f \n"
		"MOV R8, [RDX+0x30] \n"
		"TEST RAX, RAX \n"
		"JNZ 4f \n"
	".nops 5 \n"
	"2: \n"
		"MOV RDX, [RDX] \n"
		"CMP RDX, R9 \n"
		"JE 9f \n"
	"1: \n"
		"CMP WORD PTR [RDX+0x58], 18 \n"
		"JNE 2b \n"
		"TEST R8, R8 \n"
		"JZ 3b \n"
	"3: \n"
		"TEST RAX, RAX \n"
		"JNZ 2b \n"
		"MOV RDI, [RDX+0x60] \n"
		"MOV ECX, 18 \n"
		"MOV ESI, OFFSET %c[wow64_name] \n"
		"repe cmpsb \n"
		"JNE 2b \n"
		"MOV RAX, [RDX+0x30] \n"
		"TEST R8, R8 \n"
		"JZ 2b \n"
	"4: \n"

		"MOV ECX, [RAX+0x3C] \n"
		"MOV R9D, [RAX*1+RCX+0x88] \n"
		"MOV EDX, [RAX*1+R9+0x18] \n"
		"MOV R10D, [RAX*1+R9+0x20] \n"
		"ADD R10, RAX \n"

	//".align 16 \n"
	"1: \n"
		"SUB EDX, 1 \n"
		"JB 9f \n"
		"MOV EDI, [RDX*4+R10] \n"
		"ADD RDI, RAX \n"
		"MOV ECX, 25 \n"
		"MOV ESI, OFFSET %c[func_name] \n"
		"repe cmpsb \n"
		"JNE 1b \n"

		"MOV ECX, [RAX*1+R9+0x1C] \n"
		"MOV ESI, [RAX*1+R9+0x24] \n"
		"ADD RSI, RAX \n"
		"MOVZX EDX, WORD PTR [RDX*2+RSI] \n"
		"ADD RCX, RAX \n"
		"MOV EDI, [RDX*4+RCX] \n"
		"ADD RAX, RDI \n"

		"MOV EDX, [R8+0x3C] \n"
		"MOV R9D, [R8*1+RDX+0x88] \n"
		"ADD RDX, R8 \n"
		"MOV R11D, [R8*1+R9+0x18] \n"
		"MOV R10D, [R8*1+R9+0x20] \n"
		"ADD R10, R8 \n"

	//".align 16 \n"
	".nops 6 \n"
	".nops 6 \n"
	"1: \n"
		"SUB R11D, 1 \n"
		"JB 9f \n"
		"MOV EDI, [R11*4+R10] \n"
		"ADD RDI, R8 \n"
		"MOV ECX, 23 \n"
		"MOV ESI, OFFSET %c[protect_name] \n"
		"repe cmpsb \n"
		"JNE 1b \n"

		"MOV ECX, [R8*1+R9+0x1C] \n"
		"MOV EDI, [R8*1+R9+0x24] \n"
		"ADD RDI, R8 \n"
		"MOVZX ESI, WORD PTR [R11*2+RDI] \n"
		"ADD RCX, R8 \n"
		"MOV R9D, [RSI*4+RCX] \n"
		"ADD R9, R8 \n"

		"MOVZX ESI, WORD PTR [RDX+0x14] \n"
		"ADD RSI, RDX \n"
		"MOVZX EDX, WORD PTR [RDX+0x6] \n"
		"XOR R11D, R11D \n"
		"JMP 1f \n"

	//".align 16, 0xCC \n"
		"INT3 \n"
		"INT3 \n"
		"INT3 \n"
		"INT3 \n"
		"INT3 \n"
	"2: \n"
		"ADD RSI, 0x28 \n"
		"DEC EDX \n"
		"JZ 9f \n"
	"1: \n"
		"MOVZX ECX, BYTE PTR [RSI+0x3F] \n"
		"AND CL, 0x60 \n"
		"CMP CL, 0x40 \n"
		"JNE 2b \n"
		"MOV ECX, [RSI+0x20] \n"
		"MOV EDI, [RSI+0x24] \n"
		"ADD RDI, R8 \n"
		"SHR ECX, 3 \n"
		"repne scasq \n"
		"JNE 2b \n"
		"MOV [%c[prepare_addr]], RAX \n"
		"MOV [%c[protect_addr]], R9 \n"
		"ADD RDI, -8 \n"
		"MOV [%c[func_ptr_addr]], RDI \n"
		"MOV R11D, 1 \n"
	"9: \n"
		"MOV EAX, R11D \n"
		"RETF \n"
		:
		:
		[ntdll_name]"i"(&NTDLL_NAME),
		[wow64_name]"i"(&WOW64_NAME),
		[func_name]"i"(&FUNC_NAME),
		[protect_name]"i"(&PROTECT_NAME),
		[prepare_addr]"i"(&original_prepare_addr),
		[protect_addr]"i"(&nt_virtual_protect_addr),
		[func_ptr_addr]"i"(&ntdll_pointer_addr)
	);
}

dllexport gnu_noinline naked
void hook_wow_exception_addr_asm3B(
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV EBX, ESP \n"
		"SUB ESP, 0x40 \n"
		"AND ESP, -0x10 \n"
		"MOV RDI, [%c[func_ptr_addr]] \n"
		"MOV [RSP+0x38], RDI \n"
		"MOV QWORD PTR [RSP+0x30], 8 \n"
		"MOV RSI, [%c[protect_addr]] \n"
		"LEA EAX, [RSP+0x2C] \n"
		"MOV [RSP+0x20], RAX \n"
		"LEA EDX, [RSP+0x38] \n"
		"LEA R8D, [RSP+0x30] \n"
		"MOV RCX, -1 \n"
		"MOV R9D, 4 \n"
		"CALL RSI \n"
		"TEST EAX, EAX \n"
		"JS 1f \n"
		"MOV EAX, OFFSET %c[new_func] \n"
		"XCHG [RDI], RAX \n"
		"MOV R9D, [RSP+0x2C] \n"
		"LEA EAX, [RSP+0x2C] \n"
		"MOV [RSP+0x20], RAX \n"
		"LEA EDX, [RSP+0x38] \n"
		"LEA R8D, [RSP+0x30] \n"
		"MOV RCX, -1 \n"
		"CALL RSI \n"
		"XOR EAX, EAX \n"
	"1: \n"
		"MOV ESP, EBX \n"
		"RETF \n"
		:
		:
		[protect_addr] "i"(&nt_virtual_protect_addr),
		[func_ptr_addr]"i"(&ntdll_pointer_addr),
		[new_func]"i"(&prepare_for_exception_hook_asm)
	);
}

dllexport gnu_noinline naked
void hook_wow_exception_addr_asm3C(
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV EBX, ESP \n"
		"SUB ESP, 0x40 \n"
		"AND ESP, -0x10 \n"
		"MOV RDI, [%c[func_ptr_addr]] \n"
		"MOV [RSP+0x38], RDI \n"
		"MOV QWORD PTR [RSP+0x30], 8 \n"
		"MOV RSI, [%c[protect_addr]] \n"
		"LEA EAX, [RSP+0x2C] \n"
		"MOV [RSP+0x20], RAX \n"
		"LEA EDX, [RSP+0x38] \n"
		"LEA R8D, [RSP+0x30] \n"
		"MOV RCX, -1 \n"
		"MOV R9D, 4 \n"
		"CALL RSI \n"
		"TEST EAX, EAX \n"
		"JS 1f \n"
		"MOV RAX, [%c[original_func]] \n"
		"XCHG [RDI], RAX \n"
		"MOV R9D, [RSP+0x2C] \n"
		"LEA EAX, [RSP+0x2C] \n"
		"MOV [RSP+0x20], RAX \n"
		"LEA EDX, [RSP+0x38] \n"
		"LEA R8D, [RSP+0x30] \n"
		"MOV RCX, -1 \n"
		"CALL RSI \n"
		"XOR EAX, EAX \n"
	"1: \n"
		"MOV ESP, EBX \n"
		"RETF \n"
		:
		:
		[protect_addr]"i"(&nt_virtual_protect_addr),
		[func_ptr_addr]"i"(&ntdll_pointer_addr),
		[original_func]"i"(&original_prepare_addr)
	);
}

dllexport gnu_noinline naked
void hook_wow_exception_addr_asm3A2(
) {
	__asm__ volatile (
		".code64 \n"
		"movq %%gs:0x60, %%rax \n"
		"movq 0x18(%%rax), %%r9 \n"
		"movq 0x10(%%r9), %%rdx \n"
		"addq $0x10, %%r9 \n"
		"xorl %%eax, %%eax \n"
		"xorl %%r8d, %%r8d \n"
		"xorl %%r11d, %%r11d \n"
		"jmp 1f \n"
		"int3 \n"
	"3: \n"
		"movq 0x60(%%rdx), %%rdi \n"
		"movl $18, %%ecx \n"
		"movl $%c[ntdll_name], %%esi \n"
		"repe cmpsb \n"
		"jne 3f \n"
		"movq 0x30(%%rdx), %%r8 \n"
		"testq %%rax, %%rax \n"
		"jnz 4f \n"
	".nops 5 \n"
	"2: \n"
		"movq (%%rdx), %%rdx \n"
		"cmpq %%r9, %%rdx \n"
		"je 9f \n"
	"1: \n"
		"cmpw $18, 0x58(%%rdx) \n"
		"jne 2b \n"
		"testq %%r8, %%r8 \n"
		"jz 3b \n"
	"3: \n"
		"testq %%rax, %%rax \n"
		"jnz 2b \n"
		"movq 0x60(%%rdx), %%rdi \n"
		"movl $18, %%ecx \n"
		"movl $%c[wow64_name], %%esi \n"
		"repe cmpsb \n"
		"jne 2b \n"
		"movq 0x30(%%rdx), %%rax \n"
		"testq %%r8, %%r8 \n"
		"jz 2b \n"
	"4: \n"
		"movl 0x3C(%%rax), %%ecx \n"
		"movl 0x88(%%rcx,%%rax), %%r9d \n"
		"movl 0x18(%%r9,%%rax), %%edx \n"
		"movl 0x20(%%r9,%%rax), %%r10d \n"
		"addq %%rax, %%r10 \n"
	"1: \n"
		"subl $1, %%edx \n"
		"jb 9f \n"
		"movl (%%r10,%%rdx,4), %%edi \n"
		"addq %%rax, %%rdi \n"
		"movl $25, %%ecx \n"
		"movl $%c[func_name], %%esi \n"
		"repe cmpsb \n"
		"jne 1b \n"

		"movl 0x1C(%%r9,%%rax), %%ecx \n"
		"movl 0x24(%%r9,%%rax), %%esi \n"
		"addq %%rax, %%rsi \n"
		"movzwl (%%rsi,%%rdx,2), %%edx \n"
		"addq %%rax, %%rcx \n"
		"movl (%%rcx,%%rdx,4), %%edi \n"
		"addq %%rdi, %%rax \n"

		"movl 0x3C(%%r8), %%edx \n"
		"movl 0x88(%%rdx,%%r8), %%r9d \n"
		"addq %%r8, %%rdx \n"
		"movl 0x18(%%r9,%%r8), %%r11d \n"
		"movl 0x20(%%r9,%%r8), %%r10d \n"
		"addq %%r8, %%r10 \n"

	".nops 6 \n"
	".nops 6 \n"
	"1: \n"
		"subl $1, %%r11d \n"
		"jb 9f \n"
		"movl (%%r10,%%r11,4), %%edi \n"
		"addq %%r8, %%rdi \n"
		"movl $23, %%ecx \n"
		"movl $%c[protect_name], %%esi \n"
		"repe cmpsb \n"
		"jne 1b \n"

		"movl 0x1C(%%r9,%%r8), %%ecx \n"
		"movl 0x24(%%r9,%%r8), %%edi \n"
		"addq %%r8, %%rdi \n"
		"movzwl (%%rdi,%%r11,2), %%esi \n"
		"addq %%r8, %%rcx \n"
		"movl (%%rcx,%%rsi,4), %%r9d \n"
		"addq %%r8, %%r9 \n"

		"movzwl 0x14(%%rdx), %%esi \n"
		"addq %%rdx, %%rsi \n"
		"movzwl 0x6(%%rdx), %%edx \n"
		"xorl %%r11d, %%r11d \n"
		"jmp 1f \n"
		"int3 \n"
		"int3 \n"
		"int3 \n"
		"int3 \n"
		"int3 \n"
	"2: \n"
		"addq $0x28, %%rsi \n"
		"decl %%edx \n"
		"jz 9f \n"
	"1: \n"
		"movzbl 0x3F(%%rsi), %%ecx \n"
		"andb $0x60, %%cl \n"
		"cmpb $0x40, %%cl \n"
		"jne 2b \n"
		"movl 0x20(%%rsi), %%ecx \n"
		"movl 0x24(%%rsi), %%edi \n"
		"addq %%r8, %%rdi \n"
		"shrl $3, %%ecx \n"
		"repne scasq \n"
		"jne 2b \n"
		"movq %%rax, %c[prepare_addr] \n"
		"movq %%r9, %c[protect_addr] \n"
		"addq $-8, %%rdi \n"
		"movq %%rdi, %c[func_ptr_addr] \n"
		"movl $1, %%r11d \n"
	"9: \n"
		"movl %%r11d, %%eax \n"
		"lret \n"
		:
		"=m"(nt_virtual_protect_addr),
		"=m"(ntdll_pointer_addr),
		"=m"(original_prepare_addr)
		:
		[ntdll_name]"i"(&NTDLL_NAME),
		[wow64_name]"i"(&WOW64_NAME),
		[func_name]"i"(&FUNC_NAME),
		[protect_name]"i"(&PROTECT_NAME),
		[prepare_addr]"i"(&original_prepare_addr),
		[protect_addr]"i"(&nt_virtual_protect_addr),
		[func_ptr_addr]"i"(&ntdll_pointer_addr)
	);
}


dllexport gnu_noinline naked
void hook_wow_exception_addr_asm3B2(
) {
	__asm__ volatile (
		".code64 \n"
		"movl %%esp, %%ebx \n"
		"subl $0x40, %%esp \n"
		"andl $-0x10, %%esp \n"
		"movq %c[func_ptr_addr], %%rdi \n"
		"movq %%rdi, 0x38(%%rsp) \n"
		"movq $8, 0x30(%%rsp) \n"
		"movq %c[protect_addr], %%rsi \n"
		"leal 0x2C(%%rsp), %%eax \n"
		"movq %%rax, 0x20(%%rsp) \n"
		"leal 0x38(%%rsp), %%edx \n"
		"leal 0x30(%%rsp), %%r8d \n"
		"movq $-1, %%rcx \n"
		"movl $4, %%r9d \n"
		"callq *%%rsi \n"
		"testl %%eax, %%eax \n"
		"js 1f \n"
		"movl $%c[new_func], %%eax \n"
		"xchgq %%rax, (%%rdi) \n"
		"movl 0x2C(%%rsp), %%r9d \n"
		"leal 0x2C(%%rsp), %%eax \n"
		"movq %%rax, 0x20(%%rsp) \n"
		"leal 0x38(%%rsp), %%edx \n"
		"leal 0x30(%%rsp), %%r8d \n"
		"movq $-1, %%rcx \n"
		"callq *%%rsi \n"
		"xorl %%eax, %%eax \n"
	"1: \n"
		"movl %%ebx, %%esp \n"
		"lret \n"
		:
		:
		[protect_addr] "i"(&nt_virtual_protect_addr),
		[func_ptr_addr]"i"(&ntdll_pointer_addr),
		[new_func]"i"(&prepare_for_exception_hook_asm)
	);
}

dllexport gnu_noinline naked
void hook_wow_exception_addr_asm3C2(
) {
	__asm__ volatile (
		".code64 \n"
		"movl %%esp, %%ebx \n"
		"subl $0x40, %%esp \n"
		"andl $-0x10, %%esp \n"
		"movq %c[func_ptr_addr], %%rdi \n"
		"movq %%rdi, 0x38(%%rsp) \n"
		"movq $8, 0x30(%%rsp) \n"
		"movq %c[protect_addr], %%rsi \n"
		"leal 0x2C(%%rsp), %%eax \n"
		"movq %%rax, 0x20(%%rsp) \n"
		"leal 0x38(%%rsp), %%edx \n"
		"leal 0x30(%%rsp), %%r8d \n"
		"movq $-1, %%rcx \n"
		"movl $4, %%r9d \n"
		"callq *%%rsi \n"
		"testl %%eax, %%eax \n"
		"js 1f \n"
		"movq %c[original_func], %%rax \n"
		"xchgq %%rax, (%%rdi) \n"
		"movl 0x2C(%%rsp), %%r9d \n"
		"leal 0x2C(%%rsp), %%eax \n"
		"movq %%rax, 0x20(%%rsp) \n"
		"leal 0x38(%%rsp), %%edx \n"
		"leal 0x30(%%rsp), %%r8d \n"
		"movq $-1, %%rcx \n"
		"callq *%%rsi \n"
		"xorl %%eax, %%eax \n"
	"1: \n"
		"movl %%ebx, %%esp \n"
		"lret \n"
		:
		:
		[protect_addr] "i"(&nt_virtual_protect_addr),
		[func_ptr_addr]"i"(&ntdll_pointer_addr),
		[original_func]"i"(&original_prepare_addr)
	);
}

#if __x86_64__

void prepare_for_exception_hook(EXCEPTION_RECORDX<64>* exception_record, CONTEXTX<64>* context) {
	last_branch_to = context->LastBranchToRip;
	last_branch_from = context->LastBranchFromRip;
	last_exception_to = context->LastExceptionToRip;
	last_exception_from = context->LastExceptionFromRip;
	return original_exception_prepare(exception_record, context);
}

dllexport sysv_abi void hook_wow_exception_addr(int, int) {
	auto loader_data = peb->Ldr;
	LIST_ENTRYX<64, LDR_DATA_TABLE_ENTRYX<64>>* module_list = &loader_data->InLoadOrderModuleList;
	LDR_DATA_TABLE_ENTRYX<64>* module_data = module_list->Flink;
	HMODULE ntdll = NULL;
	HMODULE wow64 = NULL;
	do {
		uint64_t name_length = module_data->BaseDllName.Length;
		if (name_length == 18) {
			if (!ntdll) {
				if (repe_cmpsb(module_data->BaseDllName.Buffer, (void*)0xEFEFEFEF, name_length)) {
					ntdll = (HMODULE)module_data->DllBase;
					if (wow64) goto found_modules;
					continue;
				}
			}
			if (!wow64) {
				if (repe_cmpsb(module_data->BaseDllName.Buffer, (void*)0xCDCDCDCD, name_length)) {
					wow64 = (HMODULE)module_data->DllBase;
					if (ntdll) goto found_modules;
					continue;
				}
			}
		}
	} while ((void*)(module_data = module_data->InLoadOrderLinks.Flink) != (void*)module_list);
	return;

found_modules:
	IMAGE_NT_HEADERS* wow64_header = based_pointer<IMAGE_NT_HEADERS>(wow64, (uint32_t)((IMAGE_DOS_HEADER*)wow64)->e_lfanew);
	IMAGE_EXPORT_DIRECTORY* wow64_exports = based_pointer<IMAGE_EXPORT_DIRECTORY>(wow64, wow64_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	uint32_t export_name_count = wow64_exports->NumberOfNames;
	uint32_t* export_names = based_pointer<uint32_t>(wow64, wow64_exports->AddressOfNames);
	uint32_t i = export_name_count;
	while (!__builtin_sub_overflow(i, 1, &i)) {
		__asm__("":"+r"(i));
		const char* export_name = based_pointer<const char>(wow64, export_names[i]);
		if (repe_cmpsb(export_name, (void*)0xABABABAB, sizeof(FUNC_NAME))) {
			goto found_func;
		}
	}
	return;

found_func:
	uint16_t* export_ordinals = based_pointer<uint16_t>(wow64, wow64_exports->AddressOfNameOrdinals);

	uint32_t ordinal = export_ordinals[i];

	uint32_t* export_rvas = based_pointer<uint32_t>(wow64, wow64_exports->AddressOfFunctions);

	prepare_t* Wow64PrepareForException = based_pointer<prepare_t>(wow64, export_rvas[ordinal]);

	IMAGE_NT_HEADERS* ntdll_header = based_pointer<IMAGE_NT_HEADERS>(ntdll, (uint32_t)((IMAGE_DOS_HEADER*)ntdll)->e_lfanew);
	IMAGE_SECTION_HEADER* ntdll_sections = IMAGE_FIRST_SECTION(ntdll_header);

	uint32_t ntdll_section_count = ntdll_header->FileHeader.NumberOfSections;
	i = 0;
	do {	
		DWORD characteristics = ntdll_sections[i].Characteristics;
		if ((characteristics & 0xE0000000) == (IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)) {
			uintptr_t* section_data = based_pointer<uintptr_t>(ntdll, ntdll_sections[i].VirtualAddress);
			DWORD section_size = ntdll_sections[i].Misc.VirtualSize;
			DWORD section_search_size = section_size / sizeof(void*);
			bool match;
			__asm__ volatile (
				"repne scasq"
				: asm_arg("+c", section_search_size), asm_arg("+D", section_data), asm_flags(z, match)
				: asm_arg("a", Wow64PrepareForException)
			);
			if (match) {
				*(prepare_t**)0x89898989 = Wow64PrepareForException;
				section_data[-1] = (uintptr_t)0x67676767;
				break;
			}
		}
	} while (++i != ntdll_section_count);

	return;
}

dllexport void hook_wow_exception_addr2() {
	auto loader_data = peb->Ldr;
	LIST_ENTRYX<64, LDR_DATA_TABLE_ENTRYX<64>>* module_list = &loader_data->InLoadOrderModuleList;
	LDR_DATA_TABLE_ENTRYX<64>* module_data = module_list->Flink;
	HMODULE ntdll = NULL;
	HMODULE wow64 = NULL;
	do {
		uint64_t name_length = module_data->BaseDllName.Length;
		if (name_length == 18) {
			if (!ntdll) {
				if (repe_cmpsb(module_data->BaseDllName.Buffer, (void*)0xEFEFEFEF, name_length)) {
					ntdll = (HMODULE)module_data->DllBase;
					if (wow64) goto found_modules;
					continue;
				}
			}
			if (!wow64) {
				if (repe_cmpsb(module_data->BaseDllName.Buffer, (void*)0xCDCDCDCD, name_length)) {
					wow64 = (HMODULE)module_data->DllBase;
					if (ntdll) goto found_modules;
					continue;
				}
			}
		}
	} while ((void*)(module_data = module_data->InLoadOrderLinks.Flink) != (void*)module_list);
	return;

	uint32_t export_name_count;
	uint32_t* export_names;
	uint32_t i;
	uint16_t* export_ordinals;
	uint32_t ordinal;
	uint32_t* export_rvas;

found_modules:

	IMAGE_NT_HEADERS* wow64_header = based_pointer<IMAGE_NT_HEADERS>(wow64, (uint32_t)((IMAGE_DOS_HEADER*)wow64)->e_lfanew);
	IMAGE_EXPORT_DIRECTORY* wow64_exports = based_pointer<IMAGE_EXPORT_DIRECTORY>(wow64, wow64_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	export_name_count = wow64_exports->NumberOfNames;
	export_names = based_pointer<uint32_t>(wow64, wow64_exports->AddressOfNames);
	i = export_name_count;
	while (!__builtin_sub_overflow(i, 1, &i)) {
		__asm__("":"+r"(i));
		const char* export_name = based_pointer<const char>(wow64, export_names[i]);
		if (repe_cmpsb(export_name, (void*)0xABABABAB, sizeof(FUNC_NAME))) {
			goto found_funcA;
		}
	}
	return;

found_funcA:
	export_ordinals = based_pointer<uint16_t>(wow64, wow64_exports->AddressOfNameOrdinals);

	ordinal = export_ordinals[i];

	export_rvas = based_pointer<uint32_t>(wow64, wow64_exports->AddressOfFunctions);

	prepare_t* Wow64PrepareForException = based_pointer<prepare_t>(wow64, export_rvas[ordinal]);
	IMAGE_NT_HEADERS* ntdll_header = based_pointer<IMAGE_NT_HEADERS>(ntdll, (uint32_t)((IMAGE_DOS_HEADER*)ntdll)->e_lfanew);
	IMAGE_EXPORT_DIRECTORY* ntdll_exports = based_pointer<IMAGE_EXPORT_DIRECTORY>(ntdll, ntdll_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	export_name_count = ntdll_exports->NumberOfNames;
	export_names = based_pointer<uint32_t>(ntdll, ntdll_exports->AddressOfNames);
	i = export_name_count;
	while (!__builtin_sub_overflow(i, 1, &i)) {
		__asm__("":"+r"(i));
		const char* export_name = based_pointer<const char>(ntdll, export_names[i]);
		if (repe_cmpsb(export_name, (void*)0x45454545, sizeof(PROTECT_NAME))) {
			goto found_funcB;
		}
	}
	return;

found_funcB:
	export_ordinals = based_pointer<uint16_t>(ntdll, ntdll_exports->AddressOfNameOrdinals);

	ordinal = export_ordinals[i];

	export_rvas = based_pointer<uint32_t>(ntdll, ntdll_exports->AddressOfFunctions);

	protect_t* nt_virtual_protect = based_pointer<protect_t>(ntdll, export_rvas[ordinal]);

	IMAGE_SECTION_HEADER* ntdll_sections = IMAGE_FIRST_SECTION(ntdll_header);

	uint32_t ntdll_section_count = ntdll_header->FileHeader.NumberOfSections;
	i = 0;
	do {	
		DWORD characteristics = ntdll_sections[i].Characteristics;
		if ((characteristics & IMAGE_SCN_MEM_READ) && !(characteristics & IMAGE_SCN_MEM_EXECUTE)) {
			uintptr_t* section_data = based_pointer<uintptr_t>(ntdll, ntdll_sections[i].VirtualAddress);
			DWORD section_size = ntdll_sections[i].Misc.VirtualSize;
			DWORD section_search_size = section_size / sizeof(void*);
			bool match;
			__asm__ volatile (
				"repne scasq"
				: asm_arg("+c", section_search_size), asm_arg("+D", section_data), asm_flags(z, match)
				: asm_arg("a", Wow64PrepareForException)
			);
			if (match) {
				PVOID pointer = &section_data[-1];
				SIZE_T size = 8;
				ULONG old_access;
				NTSTATUS status = nt_virtual_protect((HANDLE)~(uintptr_t)0, &pointer, &size, PAGE_READWRITE, &old_access);
				if (status >= 0) {
					*(prepare_t**)0x89898989 = Wow64PrepareForException;
					section_data[-1] = (uintptr_t)0x67676767;
					nt_virtual_protect((HANDLE)~(uintptr_t)0, &pointer, &size, old_access, &old_access);
				}
				/*
				*(prepare_t**)0x89898989 = Wow64PrepareForException;
				section_data[-1] = (uintptr_t)0x67676767;
				*/
				break;
			}
		}
	} while (++i != ntdll_section_count);

	return;
}

dllexport sysv_abi void hook_wow_exception_addr3A(int, int) {
	PTRZX<64, PEB_LDR_DATAX<64>> loader_data = peb->Ldr;
	//__asm__ volatile ("":"+r"(loader_data));
	LIST_ENTRYX<64, LDR_DATA_TABLE_ENTRYX<64>>* module_list = &loader_data->InLoadOrderModuleList;
	register LDR_DATA_TABLE_ENTRYX<64>* module_data asm("rdx") = module_list->Flink;
	//__asm__ volatile ("":"+r"(module_data));
	register HMODULE ntdll asm("r8") = NULL;
	register HMODULE wow64 asm("rax") = NULL;
	__asm__ volatile ("":"+r"(ntdll), "+r"(wow64), "+r"(module_data));
	do {
		uint64_t name_length = module_data->BaseDllName.Length;
		if (name_length == 18) {
			if ((int64_t)ntdll >= 0) {
				if (repe_cmpsb(module_data->BaseDllName.Buffer, (void*)0xEFEFEFEF, name_length)) {
					ntdll = (HMODULE)module_data->DllBase;
					if ((int64_t)wow64 < 0) goto found_modules;
					continue;
				}
			}
			if ((int64_t)wow64 >= 0) {
				if (repe_cmpsb(module_data->BaseDllName.Buffer, (void*)0xCDCDCDCD, name_length)) {
					wow64 = (HMODULE)module_data->DllBase;
					if ((int64_t)ntdll < 0) goto found_modules;
					continue;
				}
			}
		}
	} while ((void*)(module_data = module_data->InLoadOrderLinks.Flink) != (void*)module_list);
	return;

	uint32_t export_name_count;
	uint32_t* export_names;
	uint64_t i;
	uint16_t* export_ordinals;
	uint32_t ordinal;
	uint32_t* export_rvas;
	uint64_t name_offset;
found_modules:
	//__asm__ volatile ("":"+r"(ntdll), "+r"(wow64));

	IMAGE_NT_HEADERS* wow64_header = based_pointer<IMAGE_NT_HEADERS>(wow64, (uint32_t)((IMAGE_DOS_HEADER*)wow64)->e_lfanew);
	IMAGE_EXPORT_DIRECTORY* wow64_exports = based_pointer<IMAGE_EXPORT_DIRECTORY>(wow64, wow64_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	//__asm__ volatile ("":"+r"(ntdll), "+r"(wow64));

	export_name_count = wow64_exports->NumberOfNames;
	export_names = based_pointer<uint32_t>(wow64, wow64_exports->AddressOfNames);
	i = export_name_count;
	for (;;) {
		__asm__ volatile goto (
			"SUB $1, %k[i] \n"
			"JB %l[failA] \n"
			"MOV (%q[export_names],%q[i],4), %k[name_offset] \n"
			: asm_arg("+r", i), asm_arg("=D", name_offset)
			: asm_arg("r", export_names)
			:
			: failA
		);
		const char* export_name = based_pointer<const char>(wow64, name_offset);
		if (repe_cmpsb(export_name, (void*)0xABABABAB, sizeof(FUNC_NAME))) {
			goto found_funcA;
		}
	}
failA:
	return;

found_funcA:
	export_ordinals = based_pointer<uint16_t>(wow64, wow64_exports->AddressOfNameOrdinals);

	ordinal = export_ordinals[i];

	export_rvas = based_pointer<uint32_t>(wow64, wow64_exports->AddressOfFunctions);

	uint32_t export_offset = export_rvas[ordinal];
	prepare_t* Wow64PrepareForException;
	//register prepare_t* Wow64PrepareForException asm("rax") = (prepare_t*)((uintptr_t)wow64 + export_offset);
	//__asm__ volatile ("":"+r"(Wow64PrepareForException));
	__asm__ volatile (
		"ADD %q[export_offset], %[wow64] \n"
		: "=a"(Wow64PrepareForException)
		: asm_arg("0", wow64), asm_arg("r", export_offset)
	);

	IMAGE_NT_HEADERS* ntdll_header = based_pointer<IMAGE_NT_HEADERS>(ntdll, (uint32_t)((IMAGE_DOS_HEADER*)ntdll)->e_lfanew);
	IMAGE_EXPORT_DIRECTORY* ntdll_exports = based_pointer<IMAGE_EXPORT_DIRECTORY>(ntdll, ntdll_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	export_name_count = ntdll_exports->NumberOfNames;
	export_names = based_pointer<uint32_t>(ntdll, ntdll_exports->AddressOfNames);
	i = export_name_count;
	for (;;) {
		__asm__ volatile goto (
			"SUB $1, %k[i] \n"
			"JB %l[failB] \n"
			"MOV (%q[export_names],%q[i],4), %k[name_offset] \n"
			: asm_arg("+r", i), asm_arg("=D", name_offset)
			: asm_arg("r", export_names)
			:
			: failB
		);
		const char* export_name = based_pointer<const char>(ntdll, name_offset);
		if (repe_cmpsb(export_name, (void*)0x45454545, sizeof(PROTECT_NAME))) {
			goto found_funcB;
		}
	}
failB:
	return;

found_funcB:
	export_ordinals = based_pointer<uint16_t>(ntdll, ntdll_exports->AddressOfNameOrdinals);

	ordinal = export_ordinals[i];

	export_rvas = based_pointer<uint32_t>(ntdll, ntdll_exports->AddressOfFunctions);

	protect_t* nt_virtual_protect = based_pointer<protect_t>(ntdll, export_rvas[ordinal]);

	/*
	((PIMAGE_SECTION_HEADER)        \
	 ((ULONG_PTR)(ntheader)+\
	  FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + \
	  ((ntheader))->FileHeader.SizeOfOptionalHeader   \
	  ))
	*/

	//IMAGE_SECTION_HEADER* ntdll_sections = IMAGE_FIRST_SECTION(ntdll_header);
	//register uint8_t* cur_section asm("rsi") = based_pointer<uint8_t>(IMAGE_FIRST_SECTION(ntdll_header), 0x8);
	register uint8_t* cur_section asm("rsi") = based_pointer<uint8_t>(ntdll_header, ntdll_header->FileHeader.SizeOfOptionalHeader);
	uint32_t ntdll_section_count = ntdll_header->FileHeader.NumberOfSections;
	//register IMAGE_SECTION_HEADER* cur_section asm("rsi") = MakeOffsetPtr(ntdll_sections, IMAGE_SECTION_HEADER, Misc);
	//IMAGE_SECTION_HEADER e;
	do {
		__asm__ volatile ("":"+r"(cur_section));
		//DWORD characteristics = ntdll_sections[j].Characteristics;
		//uint8_t characteristics_byte = ((uint8_t*)&ntdll_sections->Characteristics)[3];
		//uint8_t characteristics_byte = ((uint8_t*)&OffsetField(cur_section, IMAGE_SECTION_HEADER, Characteristics, Misc))[3];
		uint8_t characteristics_byte = cur_section[FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + FIELD_OFFSET(IMAGE_SECTION_HEADER, Characteristics) + 3];
		characteristics_byte &= 0x60; // IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE
		if (
			characteristics_byte == 0x40 // IMAGE_SCN_MEM_READ
		) {
			//uintptr_t* section_data = based_pointer<uintptr_t>(ntdll, ntdll_sections->VirtualAddress);
			//DWORD section_size = ntdll_sections->Misc.VirtualSize;
			//uintptr_t* section_data = based_pointer<uintptr_t>(ntdll, OffsetField(cur_section, IMAGE_SECTION_HEADER, VirtualAddress, Misc));
			//DWORD section_size = OffsetField(cur_section, IMAGE_SECTION_HEADER, Misc, Misc).VirtualSize;
			//DWORD section_size = ((DWORD*)cur_section)[0];
			//uintptr_t* section_data = based_pointer<uintptr_t>(ntdll, ((DWORD*)cur_section)[1]);
			DWORD section_size = *(DWORD*)&cur_section[FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + FIELD_OFFSET(IMAGE_SECTION_HEADER, Misc)];
			DWORD section_offset = *(DWORD*)&cur_section[FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + FIELD_OFFSET(IMAGE_SECTION_HEADER, VirtualAddress)];
			uintptr_t* section_data = based_pointer<uintptr_t>(ntdll, section_offset);
			DWORD section_search_size = section_size / sizeof(void*);
			bool match;
			__asm__ volatile (
				"repne scasq"
				: asm_arg("+c", section_search_size), asm_arg("+D", section_data), asm_flags(z, match)
				: asm_arg("a", Wow64PrepareForException)
			);
			if (match) {

				*(uint64_t*)0x67676767 = (uint64_t)Wow64PrepareForException;
				*(uint64_t*)0x45454545 = (uint64_t)nt_virtual_protect;
				PVOID pointer = &section_data[-1];
				*(uint64_t*)0x78787878 = (uint64_t)pointer;
				/*
				SIZE_T size = 8;
				ULONG old_access;
				NTSTATUS status = nt_virtual_protect((HANDLE)~(uintptr_t)0, &pointer, &size, PAGE_READWRITE, &old_access);
				if (status >= 0) {
					*(prepare_t**)0x89898989 = Wow64PrepareForException;
					section_data[-1] = (uintptr_t)0x67676767;
					nt_virtual_protect((HANDLE)~(uintptr_t)0, &pointer, &size, old_access, &old_access);
				}
				*/
				/*
				*(prepare_t**)0x89898989 = Wow64PrepareForException;
				section_data[-1] = (uintptr_t)0x67676767;
				*/
				break;
			}
		}
	} while ((cur_section += 0x28), --ntdll_section_count);

	return;
}
dllexport int32_t hook_wow_exception_addr3B() {
	uint64_t* func_ptr = *(uint64_t**)0x78787878;
	PVOID pointer = (PVOID)func_ptr;
	SIZE_T size = 8;
	ULONG old_access;
	protect_t* protect = *(protect_t**)0x45454545;
	NTSTATUS status = protect((HANDLE)~(uintptr_t)0, &pointer, &size, PAGE_READWRITE, &old_access);
	if (status >= 0) {
		*(_Atomic(uint64_t)*)func_ptr = (uint64_t)0x67676767;
		protect((HANDLE)~(uintptr_t)0, &pointer, &size, old_access, &old_access);
		status = 0;
	}
	return status;
}
dllexport int32_t hook_wow_exception_addr3C() {
	uint64_t* func_ptr = *(uint64_t**)0x78787878;
	PVOID pointer = (PVOID)func_ptr;
	SIZE_T size = 8;
	ULONG old_access;
	protect_t* protect = *(protect_t**)0x45454545;
	NTSTATUS status = protect((HANDLE)~(uintptr_t)0, &pointer, &size, PAGE_READWRITE, &old_access);
	if (status >= 0) {
		*(_Atomic(uint64_t)*)func_ptr = *(uint64_t*)0x67676767;
		protect((HANDLE)~(uintptr_t)0, &pointer, &size, old_access, &old_access);
		status = 0;
	}
	return status;
}

#endif

//#if !__x86_64__
static DWORD branch_to_tls = rand();
static DWORD branch_from_tls = rand();
//#endif

dllexport gnu_noinline naked
void prepare_for_exception_hook_tls_asm(
	EXCEPTION_RECORDX<64>* exception_record,
	CONTEXTX<64>* context
) {
	__asm__ volatile(
		INTEL_64_DIRECTIVE
		"MOV EAX, [%c[except_to_slot]] \n"
		"CMP EAX, 64 - 1 \n"
		"JA 1f \n"
		"MOV R8D, [RDX+0x4C0] \n"
		"MOV FS:[RAX*4+0xE10], R8D \n"
		"JMP 2f \n"
	"1: \n"
		"CMP EAX, 64 + 1024 - 1 \n"
		"JA 2f \n"
		"MOV R8D, [RDX+0x4C0] \n"
		"MOV R9D, FS:[0xF94] \n"
		"MOV [RAX*4+R9-0x100], R8D \n"
	"2: \n"
		"MOV EAX, [%c[except_from_slot]] \n"
		"CMP EAX, 64 - 1 \n"
		"JA 1f \n"
		"MOV R8D, [RDX+0x4C8] \n"
		"MOV FS:[RAX*4+0xE10], R8D \n"
		"JMP 2f \n"
	"1: \n"
		"CMP EAX, 64 + 1024 - 1 \n"
		"JA 2f \n"
		"MOV R8D, [RDX+0x4C8] \n"
		"MOV R9D, FS:[0xF94] \n"
		"MOV [RAX*4+R9-0x100], R8D \n"
	"2: \n"
		"JMP QWORD PTR [%c[func_ptr]] \n"
		:
		:
			[except_to_slot]"i"(&branch_to_tls),
			[except_from_slot]"i"(&branch_from_tls),
			[func_ptr]"i"(&original_exception_prepare)
	);
}

dllexport gnu_noinline naked
void prepare_for_exception_hook_tls_asm2(
	EXCEPTION_RECORDX<64>* exception_record,
	CONTEXTX<64>* context
) {
	__asm__ volatile(
		INTEL_64_DIRECTIVE
		"MOV EAX, [%c[except_to_slot]] \n"
		"MOV R8D, [RDX+0x4C0] \n"
		"CMP EAX, 64 - 1 \n"
		"JA 1f \n"
		"MOV FS:[RAX*4+0xE10], R8D \n"
		"JMP 2f \n"
	"1: \n"
		"MOV R9D, FS:[0xF94] \n"
		"MOV [RAX*4+R9-0x100], R8D \n"
	"2: \n"
		"MOV EAX, [%c[except_from_slot]] \n"
		"MOV R8D, [RDX+0x4C8] \n"
		"CMP EAX, 64 - 1 \n"
		"JA 1f \n"
		"MOV FS:[RAX*4+0xE10], R8D \n"
		"JMP 2f \n"
	"1: \n"
		"MOV R9D, FS:[0xF94] \n"
		"MOV [RAX*4+R9-0x100], R8D \n"
	"2: \n"
		"JMP QWORD PTR [%c[func_ptr]] \n"
		:
		:
			[except_to_slot]"i"(&branch_to_tls),
			[except_from_slot]"i"(&branch_from_tls),
			[func_ptr]"i"(&original_exception_prepare)
	);
}

dllexport gnu_noinline naked
void prepare_for_exception_hook_tls_asm2B(
	EXCEPTION_RECORDX<64>* exception_record,
	CONTEXTX<64>* context
) {
	__asm__ volatile(
		".code64 \n"
		"movl %c[except_to_slot], %%eax \n"
		"movl 0x4C0(%%rdx), %%r8d \n"
		"cmpl $0x40, %%eax \n"
		"jnb 1f \n"
		"movl %%r8d, %%fs:0xE10(,%%rax,4) \n"
		"jmp 2f \n"
	"1: \n"
		"movl %%fs:0xF94, %%r9d \n"
		"movl %%r8d, -0x100(%%r9,%%rax,4) \n"
	"2: \n"
		"movl %c[except_from_slot], %%eax \n"
		"movl 0x4C8(%%rdx), %%r8d \n"
		"cmpl $0x40, %%eax \n"
		"jnb 1f \n"
		"movl %%r8d, %%fs:0xE10(,%%rax,4) \n"
		"jmp 2f \n"
	"1: \n"
		"movl %%fs:0xF94, %%r9d \n"
		"movl %%r8d, -0x100(%%r9,%%rax,4) \n"
	"2: \n"
		"jmpq *%c[func_ptr] \n"
		:
		:
		[except_to_slot]"i"(&branch_to_tls),
		[except_from_slot]"i"(&branch_from_tls),
		[func_ptr]"i"(&original_exception_prepare)
	);
}

#if __x86_64__
dllexport void prepare_for_exception_hook_tls(EXCEPTION_RECORDX<64>* exception_record, CONTEXTX<64>* context) {
	//last_branch_to = context->LastBranchToRip;
	//last_branch_from = context->LastBranchFromRip;
	//last_exception_to = context->LastExceptionToRip;
	//last_exception_from = context->LastExceptionFromRip;
	DWORD branch_to_slot = branch_to_tls;
	if (branch_to_slot < 64) {
		teb32->TlsSlots[branch_to_slot] = (PTRZX<32, UNKNOWN_TYPE>)context->LastExceptionToRip;
	} else //if (branch_to_slot < 64 + 1024)
	{
		teb32->TlsExpansionSlots[branch_to_slot - 64] = (PTRZX<32, UNKNOWN_TYPE>)context->LastExceptionToRip;
	}
	DWORD branch_from_slot = branch_from_tls;
	if (branch_from_slot < 64) {
		teb32->TlsSlots[branch_from_slot] = (PTRZX<32, UNKNOWN_TYPE>)context->LastExceptionFromRip;
	} else //if (branch_from_slot < 64 + 1024)
	{
		teb32->TlsExpansionSlots[branch_from_slot - 64] = (PTRZX<32, UNKNOWN_TYPE>)context->LastExceptionFromRip;
	}
	return original_exception_prepare(exception_record, context);
}
#endif

bool enable_last_branch_record() {
	// Wine does not support the necessary undocumented behavior of DR7 bits
	if (!GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "wine_get_version")) {
#if !__x86_64__
		typedef BOOL WINAPI IsWow64Process_t(HANDLE hProcess, PBOOL Wow64Process);
		IsWow64Process_t* IsWow64Process_ptr = (IsWow64Process_t*)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process");
		BOOL is_wow64;
		if (
			!IsWow64Process_ptr ||
			!IsWow64Process_ptr((HANDLE)-1, &is_wow64) ||
			!is_wow64
		) {
			return false;
		}
		DWORD branch_to_tls_temp = TlsAlloc();
		if (branch_to_tls_temp == TLS_OUT_OF_INDEXES) {
			return false;
		}
		DWORD branch_from_tls_temp = TlsAlloc();
		if (branch_from_tls_temp == TLS_OUT_OF_INDEXES) {
			TlsFree(branch_to_tls_temp);
			return false;
		}
		branch_to_tls = branch_to_tls_temp;
		branch_from_tls = branch_from_tls_temp;

		int32_t success;
		__asm__ volatile (
			CODE_32_DIRECTIVE
			"lcall %[Seg],%[Addr]"
			: "=a"(success), "=m"(original_exception_prepare)
			: [Seg]"i"(0x33), [Addr]"i"(hook_wow_exception_addr_asm2)
		);
		if (success < 0) {
			TlsFree(branch_to_tls_temp);
			TlsFree(branch_from_tls_temp);
			return false;
		}
#endif

	}
	return false;
}

int main() {
	//PTR64<> ntdll64_handle = GetModuleHandleW64(L"ntdll.dll");
	//thread_test = rand();
	
	int32_t success;
	FAR_CALL_IMM(0x33, hook_wow_exception_addr_asm2,
				 "=a"(success),
				 "i"(0)
				 //: clobber_list("esi", "edi", "ebx", "ebp")
	);
	last_branch_testing();
	return 0;
}