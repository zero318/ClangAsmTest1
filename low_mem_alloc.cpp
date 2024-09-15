
#include <stdlib.h>
#include <stdint.h>

#include "Windows.h"
#include <winternl.h>

#include "zero/util.h"

//#include <ntifs.h>

//typedef MEMORY_BASIC_INFORMATION MEMORY_INFORMATION_CLASS;

//#define INCLUDE_WTF_MEMORY_INFO 1

static constexpr size_t page_size = 4096;

enum MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation,				// 0, 0
#if INCLUDE_WTF_MEMORY_INFO
	MemoryWorkingSetInformation,		//  , 1
#endif
	MemoryMappedFilenameInformation,	// 1, 2
	MemoryRegionInformation,			// 2, 3
	MemoryWorkingSetExInformation		// 3, 4
};

static NTSTATUS NTAPI NtAllocateVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect);
static NTSTATUS NTAPI NtFreeVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T RegionSize, ULONG FreeType);
static NTSTATUS NTAPI NtProtectVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection);
static NTSTATUS NTAPI NtQueryVirtualMemoryShim(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID Buffer, ULONG Length, PULONG ResultLength);
static NTSTATUS NTAPI NtLockVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T NumberOfBytesToLock, ULONG LockOption);
static NTSTATUS NTAPI NtUnlockVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T NumberOfBytesToUnlock, ULONG LockType);
static BOOLEAN NTAPI RtlFlushSecureMemoryCacheShim(PVOID MemoryCache, SIZE_T MemoryLength);

typedef decltype(&NtAllocateVirtualMemoryShim) NtAllocateVirtualMemoryPtr;
typedef decltype(&NtFreeVirtualMemoryShim) NtFreeVirtualMemoryPtr;
typedef decltype(&NtProtectVirtualMemoryShim) NtProtectVirtualMemoryPtr;
typedef decltype(&NtQueryVirtualMemoryShim) NtQueryVirtualMemoryPtr;
typedef decltype(&NtLockVirtualMemoryShim) NtLockVirtualMemoryPtr;
typedef decltype(&NtUnlockVirtualMemoryShim) NtUnlockVirtualMemoryPtr;

static NtAllocateVirtualMemoryPtr NtAllocateVirtualMemory = &NtAllocateVirtualMemoryShim;
static NtFreeVirtualMemoryPtr NtFreeVirtualMemory = &NtFreeVirtualMemoryShim;
static NtProtectVirtualMemoryPtr NtProtectVirtualMemory = &NtProtectVirtualMemoryShim;
static NtQueryVirtualMemoryPtr NtQueryVirtualMemory = &NtQueryVirtualMemoryShim;
static NtLockVirtualMemoryPtr NtLockVirtualMemory = &NtLockVirtualMemoryShim;
static NtUnlockVirtualMemoryPtr NtUnlockVirtualMemory = &NtUnlockVirtualMemoryShim;

static void resolve_nt_virtual_memory_funcs();

static NTSTATUS NTAPI NtAllocateVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect) {
	resolve_nt_virtual_memory_funcs();
	return NtAllocateVirtualMemory(ProcessHandle, BaseAddress, ZeroBits, RegionSize, AllocationType, Protect);
}
static NTSTATUS NTAPI NtFreeVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T RegionSize, ULONG FreeType) {
	resolve_nt_virtual_memory_funcs();
	return NtFreeVirtualMemory(ProcessHandle, BaseAddress, RegionSize, FreeType);
}
static NTSTATUS NTAPI NtProtectVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection) {
	resolve_nt_virtual_memory_funcs();
	return NtProtectVirtualMemory(ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
}
static NTSTATUS NTAPI NtQueryVirtualMemoryShim(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID Buffer, ULONG Length, PULONG ResultLength) {
	resolve_nt_virtual_memory_funcs();
	return NtQueryVirtualMemory(ProcessHandle, BaseAddress, MemoryInformationClass, Buffer, Length, ResultLength);
}
static NTSTATUS NTAPI NtLockVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T NumberOfBytesToLock, ULONG LockOption) {
	resolve_nt_virtual_memory_funcs();
	return NtLockVirtualMemory(ProcessHandle, BaseAddress, NumberOfBytesToLock, LockOption);
}
static NTSTATUS NTAPI NtUnlockVirtualMemoryShim(HANDLE ProcessHandle, PVOID *BaseAddress, PSIZE_T NumberOfBytesToUnlock, ULONG LockType) {
	resolve_nt_virtual_memory_funcs();
	return NtUnlockVirtualMemory(ProcessHandle, BaseAddress, NumberOfBytesToUnlock, LockType);
}

static void resolve_nt_virtual_memory_funcs() {
	const HMODULE ntdll_module = (HMODULE)GetModuleHandleA("ntdll.dll");
	NtAllocateVirtualMemory = (NtAllocateVirtualMemoryPtr)GetProcAddress(ntdll_module, "NtAllocateVirtualMemory");
	NtFreeVirtualMemory = (NtFreeVirtualMemoryPtr)GetProcAddress(ntdll_module, "NtFreeVirtualMemory");
	NtProtectVirtualMemory = (NtProtectVirtualMemoryPtr)GetProcAddress(ntdll_module, "NtProtectVirtualMemory");
	NtQueryVirtualMemory = (NtQueryVirtualMemoryPtr)GetProcAddress(ntdll_module, "NtQueryVirtualMemory");
	NtLockVirtualMemory = (NtLockVirtualMemoryPtr)GetProcAddress(ntdll_module, "NtLockVirtualMemory");
	NtUnlockVirtualMemory = (NtUnlockVirtualMemoryPtr)GetProcAddress(ntdll_module, "NtUnlockVirtualMemory");
}

static constexpr size_t low_mem_zero_mask = 0x7FFFFFFF;
static constexpr uintptr_t null_region_size = 0x10000;
static constexpr uint32_t max_numa_node = 0x3F;
static constexpr uint8_t* temp = NULL;
static constexpr HANDLE current_process = NULL; // This is the same as NtCurrentProcess()


#define STATUS_SUCCESS					((DWORD)0x00000000)
#define STATUS_ACCESS_DENIED			((DWORD)0xC0000022)
#define STATUS_ALREADY_COMMITTED		((DWORD)0xC0000021)
#define STATUS_COMMITMENT_LIMIT			((DWORD)0xC000012D)
#define STATUS_CONFLICTING_ADDRESSES	((DWORD)0xC0000018)
#define STATUS_INSUFFICIENT_RESOURCES	((DWORD)0xC000009A)
//#define STATUS_INVALID_HANDLE			((DWORD)0xC0000008)
#define STATUS_INVALID_PAGE_PROTECTION	((DWORD)0xC0000045)
//#define STATUS_NO_MEMORY				((DWORD)0xC0000017)
#define STATUS_OBJECT_TYPE_MISMATCH		((DWORD)0xC0000024)
#define STATUS_PROCESS_IS_TERMINATING	((DWORD)0xC000010A)
//#define STATUS_INVALID_PARAMETER		((DWORD)0xC000000D)

#define INVALID_PARAMETER_VALUE 0x57

/*
#define PAGE_NOACCESS			0x001
#define PAGE_READONLY			0x002
#define PAGE_READWRITE			0x004
#define PAGE_WRITECOPY			0x008
#define PAGE_EXECUTE			0x010
#define PAGE_EXECUTE_READ		0x020
#define PAGE_EXECUTE_READWRITE	0x040
#define PAGE_EXECUTE_WRITECOPY	0x080
*/
#define PAGE_GUARD				0x100
#define PAGE_NOCACHE			0x200
#define PAGE_WRITECOMBINE		0x400

#define MEM_COMMIT			0x00001000
#define MEM_RESERVE			0x00002000
#define MEM_DECOMMIT		0x00004000
#define MEM_RELEASE			0x00008000
#define MEM_FREE			0x00010000
#define MEM_PRIVATE			0x00020000
#define MEM_MAPPED			0x00040000
#define MEM_RESET			0x00080000
#define MEM_TOP_DOWN		0x00100000
#define MEM_WRITE_WATCH		0x00200000
#define MEM_PHYSICAL		0x00400000
#define MEM_IMAGE			0x01000000 // MEM_RESET_UNDO since windows 8
#define MEM_LARGE_PAGES		0x20000000
#define MEM_DOS_LIM			0x40000000
#define MEM_4MB_PAGES		0x80000000

// size: 0x4
union MEMORY_WORKING_SET_BLOCK32 {
	uint32_t raw; // 0x0
	struct {
		uint32_t Protection : 5;
		uint32_t ShareCount : 3;
		uint32_t Shared : 1;
		uint32_t Node : 3;
		uint32_t VirtualPage : 20;
	};
	// 0x4
};

// size: 0x8
union MEMORY_WORKING_SET_BLOCK64 {
	uint64_t raw; // 0x0
	struct {
		uint64_t Protection : 5;
		uint64_t ShareCount : 3;
		uint64_t Shared : 1;
		uint64_t Node : 3;
		uint64_t VirtualPage : 52;
	};
	// 0x8
};

// size: 0x4
union MEMORY_WORKING_SET_EX_BLOCK32 {
	uint32_t raw; // 0x0
	struct {
		uint32_t Valid : 1;
		uint32_t ShareCount : 3;
		uint32_t Win32Protection : 11;
		uint32_t Shared : 1;
		uint32_t Node : 6;
		uint32_t Locked : 1;
		uint32_t LargePage : 1;
		uint32_t Priority : 3;
		uint32_t Reserved : 5;
	};
	// 0x4
};

// size: 0x8
union MEMORY_WORKING_SET_EX_BLOCK64 {
	uint64_t raw; // 0x0
	struct {
		uint64_t Valid : 1;
		uint64_t ShareCount : 3;
		uint64_t Win32Protection : 11;
		uint64_t Shared : 1;
		uint64_t Node : 6;
		uint64_t Locked : 1;
		uint64_t LargePage : 1;
		uint64_t Priority : 3;
		uint64_t ReservedA : 5;
		uint64_t ReservedB : 32;
	};
	// 0x8
};

// size: 0x8
struct MEMORY_WORKING_SET_EX_INFORMATION32 {
	PTR32Z<void> VirtualAddress; // 0x0
	MEMORY_WORKING_SET_EX_BLOCK32 VirtualAttributes; // 0x4
	// 0x8
};

// size: 0x10
struct MEMORY_WORKING_SET_EX_INFORMATION64 {
	PTR64<void> VirtualAddress; // 0x0
	MEMORY_WORKING_SET_EX_BLOCK64 VirtualAttributes; // 0x8
	// 0x10
};

// size: 0x8
union MEMORY_FRAME_INFORMATION {
	uint64_t raw; // 0x0
	struct {
		uint64_t UseDescription : 4;
		uint64_t ListDescription : 3;
		uint64_t ReservedA : 1;
		uint64_t Pinned : 1;
		uint64_t ReservedB : 48;
		uint64_t ReservedC : 7;
	};
	// 0x8
};

// size: 0x8
union FILEOFFSET_INFORMATION {
	uint64_t raw; // 0x0
	struct {
		uint64_t ReservedA : 9;
		uint64_t Offset : 48;
		uint64_t ReservedB : 7;
	};
	// 0x8
};

// size: 0x8
union PAGEDIR_INFORMATION {
	uint64_t raw; // 0x0
	struct {
		uint64_t ReservedA : 9;
		uint64_t PageDirectoryBase : 48;
		uint64_t ReservedB : 7;
	};
	// 0x8
};

static DWORD set_nt_error_from_nt_status(NTSTATUS status) {
	//DWORD ret = RtlNtStatusToDosError(status);
	DWORD ret;
	SetLastError(ret);
	return ret;
}

LPVOID VirtualAllocLowEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect) {
	if (lpAddress && (uintptr_t)lpAddress < null_region_size) {
		SetLastError(INVALID_PARAMETER_VALUE);
		return NULL;
	}
	flAllocationType &= ~max_numa_node; // Make sure we don't do anything weird anyway...
	NTSTATUS status = NtAllocateVirtualMemory(hProcess, &lpAddress, low_mem_zero_mask, &dwSize, flAllocationType, flProtect);
	if (status < STATUS_SUCCESS) {
		return lpAddress;
	} else {
		set_nt_error_from_nt_status(status);
		return NULL;
	}
}

LPVOID VirtualAllocLow(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect) {
	return VirtualAllocLowEx(current_process, lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL VirtualFreeLowEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType) {
	if ((dwFreeType & MEM_RELEASE) && !dwSize) {
		SetLastError(INVALID_PARAMETER_VALUE);
		return false;
	}
	NTSTATUS status = NtFreeVirtualMemory(hProcess, &lpAddress, &dwSize, dwFreeType);
	if (status < STATUS_SUCCESS) {
		return true;
	}
	else if (status == STATUS_INVALID_PAGE_PROTECTION) {
		set_nt_error_from_nt_status(status);
		return false;
	}
}

BOOL VirtualFreeLow(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType) {
	return VirtualFreeLowEx(current_process, lpAddress, dwSize, dwFreeType);
}

struct TEB64;
struct TEB64 {
	PTR64<> ExceptionList; // 0x0
	PTR64<> StackBase; // 0x8
	PTR64<> StackLimit; // 0x10
	PTR64<> SubSystemTib; // 0x18
	union {
		PTR64<> FiberData; // 0x20
		uint32_t Version; // 0x20
	};
	PTR64<> ArbitraryUserPointer; // 0x28
	PTR64<TEB64> Self; // 0x30
};

template<typename T>
struct LIST_ENTRY64B {
	PTR64<T> Flink; // 0x0
	PTR64<T> Blink; // 0x8
	// 0x10
};


template<typename T>
static inline constexpr T unpack_hilo(uint64_t low, uint64_t high) {
	union Unpack {
		struct {
			uint32_t low;
			uint32_t high;
		};
		T ret;
	};
	return ((Unpack) { { low, high } }).ret;
}

#define ASSUME_HIGH_REGS_ARE_ZERO 1

#ifdef _M_IX86
using IsWow64ProcessPtrT = decltype(&IsWow64Process);
static constexpr const bool is_x64 = false;
static const bool is_wow_64 = []() -> bool {
	BOOL ret = false;
	if (auto IsWow64ProcessPtr = (IsWow64ProcessPtrT)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process")) {
		IsWow64ProcessPtr(GetModuleHandleA(NULL), &ret);
	}
	return ret;
}();

dllexport gnu_noinline naked
	HANDLE						// EDX:EAX, XMM0
regcall GetModuleHandleW64_old(
	PTR64<const wchar_t> name
) {
	__asm__ volatile (
		"xorps %xmm0, %xmm0 \n"
		"movl %edi, %edi \n"
		"testl %edi, %edi \n"
		"jz 3f \n"
		GS_OVERRIDE REX_W ".byte 0x8B, 0x14, 0x25, 0x60, 0x00, 0x00, 0x00 \n" // "movq %gs:0x60, %rdx \n"
		REX_WR "movl 0x18(%edx), %ecx \n" // "movq 0x18(%rdx), %r9 \n"
		REX_WRB "movl 0x10(%ecx), %eax \n" // "movq 0x10(%r9), %r8 \n"
		REX_WB "addl $0x10, %ecx \n" // "addq $0x10, %r9 \n"
		".align 16 \n"
	"1: \n"
		REX_B "movzwl 0x58(%eax), %ecx \n" // "movzwl 0x58(%r8), %ecx \n"
		REX_WB "movl 0x60(%eax), %esi \n" // "movq 0x60(%r8), %rsi \n"
		"movl %edi, %eax \n" // "movl %edi, %eax \n"
		REX_W "subl %edi, %esi \n" // "subq %rdi, %rsi \n"
		REX_W "addl %edi, %ecx \n" // "addq %rdi, %rcx \n"
	"jmp 1f \n"
	"2: \n"
		REX_WRB "movl (%eax), %eax \n" // "movq (%r8), %r8 \n"
		REX_WRB "cmpl %ecx, %eax \n" // "cmpq %r9, %r8 \n"
	"jne 1b \n"
	"3: \n"
		"xorl %eax, %eax \n"
		"xorl %edx, %edx \n"
		"lret \n"
		".align 16, 0xCC \n"
	"3: \n"
		REX_R "movzbl (%eax), %edx \n" // "movzwl (%rax), %r10d \n"
		REX_W "leal 1(%eax), %edx \n" // "leaq 1(%rax), %rdx \n"
		REX_R "cmpb (%esi,%eax), %dl \n" // "cmpb (%rsi,%rax), %r10b \n"
	"jne 2b \n"
		REX_W "movl %edx, %eax \n" // "movq %rdx, %rax \n"
	"1: \n"
		REX_W "cmpl %ecx, %eax \n" // "cmpq %rcx, %rax \n"
	"jne 3b \n"
		DATASIZE REX_WB "movd 0x30(%eax), %mm0 \n" // "movq 0x30(%r8), %xmm0 \n"
		REX_WB "movl 0x30(%eax), %eax \n" // "movq 0x30(%r8), %rax \n"
		REX_W "movl %eax, %edx \n" // "movq %rax, %rdx \n"
		REX_W ".byte 0xC1, 0xEA, 0x20 \n" // "shrq $0x20, %rdx \n"
		"lret \n"
	);
}

struct alignas(8) ExtMem32 {
	PTR32Z<> buffer; // 0x0
	uint32_t size; // 0x8
};
struct alignas(8) ExtMem64 {
	PTR64<> buffer; // 0x0
	uint64_t size; // 0x8
};

//((val) + 15) & -15)


template<typename T>
static forceinline auto handle_to_base_ptr(T handle) {
	if constexpr (std::is_same_v<T, HMODULE>) {
		return (T)((uintptr_t)handle & ~(uintptr_t)3);
	}
#if NATIVE_BITS == 32
	if constexpr (std::is_same_v<T, PTR64<>>) {
		return (T)((uint64_t)handle & ~(uint64_t)3);
	}
#elif NATIVE_BITS == 64
	if constexpr (std::is_same_v<T, PTR32Z<>> || std::is_same_v<T, PTR32S<>>) {
		return (T)((uint32_t)handle & ~(uint32_t)3);
	}
#endif
}

/*

Adapter API Rules:
1. 64 bit functions may only be called from 32 bit code via an adapter

Adapter ABI Rules:
1. The stack does not need to be aligned before calling a 64 bit adapter
2. The upper 32 bits of all GPRs are undefined when calling an adapter (maybe even RSP)
3. Adapters do not need stack space allocated for home parameters

Adapter Argument ABI:
1. General registers will be used in the order: EAX, EDX, ECX, EDI, ESI, EBX, EBP
2. Vector registers will be used in the order: XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7
3. x87 registers will only be used for 80-bit long double arguments (but there probably won't be any)
4. Any of the EBX/EBP/ESI/EDI registers not used for passing arguments will be preserved
5. EAX/ECX/EDX will *not* be preserved
6. XMM6/XMM7 will *not* be preserved
7. Arguments are never passed on the stack

Adapter Return Value ABI:
1. Gereral registers will be used in the order: EAX, EDX, ECX
2. Vector registers will be used in the order: XMM0

*/

#define CALL_PTR_LOW(sym) ".byte 0xFF, 0x14, 0x25 \n .long " MACRO_STR(sym) " \n"
//#define UNALIGNED_X64_ENTRY(stack_size) "pushl %ebp \n movl %esp, %ebp \n subl $0x30 + $" MACRO_STR(stack_size) ", %esp \n andl $-0x10, %esp \n"
//#define ALIGNED_X64_ENTRY(stack_size) "subl $0x20 + $" MACRO_STR(stack_size) ", %esp \n"
//#define UNALIGNED_X64_EXIT(stack_size) "leavel \n"
//#define ALIGNED_X64_EXIT(stack_size) "addl $0x20 + $" MACRO_STR(stack_size) ", %esp \n"
#define BIG_SHL_RAX(count) REX_W ".byte 0xC1, 0xE0, " MACRO_STR(count) " \n"
#define BIG_SHL_RCX(count) REX_W ".byte 0xC1, 0xE1, " MACRO_STR(count) " \n"
#define BIG_SHL_RDX(count) REX_W ".byte 0xC1, 0xE2, " MACRO_STR(count) " \n"
#define BIG_SHL_RBX(count) REX_W ".byte 0xC1, 0xE3, " MACRO_STR(count) " \n"
#define BIG_SHL_RSP(count) REX_W ".byte 0xC1, 0xE4, " MACRO_STR(count) " \n"
#define BIG_SHL_RBP(count) REX_W ".byte 0xC1, 0xE5, " MACRO_STR(count) " \n"
#define BIG_SHL_RSI(count) REX_W ".byte 0xC1, 0xE6, " MACRO_STR(count) " \n"
#define BIG_SHL_RDI(count) REX_W ".byte 0xC1, 0xE7, " MACRO_STR(count) " \n"
#define BIG_SHL_R8(count)  REX_WB ".byte 0xC1, 0xE0, " MACRO_STR(count) " \n"
#define BIG_SHL_R9(count)  REX_WB ".byte 0xC1, 0xE1, " MACRO_STR(count) " \n"
#define BIG_SHL_R10(count) REX_WB ".byte 0xC1, 0xE2, " MACRO_STR(count) " \n"
#define BIG_SHL_R11(count) REX_WB ".byte 0xC1, 0xE3, " MACRO_STR(count) " \n"
#define BIG_SHL_R12(count) REX_WB ".byte 0xC1, 0xE4, " MACRO_STR(count) " \n"
#define BIG_SHL_R13(count) REX_WB ".byte 0xC1, 0xE5, " MACRO_STR(count) " \n"
#define BIG_SHL_R14(count) REX_WB ".byte 0xC1, 0xE6, " MACRO_STR(count) " \n"
#define BIG_SHL_R15(count) REX_WB ".byte 0xC1, 0xE7, " MACRO_STR(count) " \n"
#define BIG_SHR_RAX(count) REX_W ".byte 0xC1, 0xE8, " MACRO_STR(count) " \n"
#define BIG_SHR_RCX(count) REX_W ".byte 0xC1, 0xE9, " MACRO_STR(count) " \n"
#define BIG_SHR_RDX(count) REX_W ".byte 0xC1, 0xEA, " MACRO_STR(count) " \n"
#define BIG_SHR_RBX(count) REX_W ".byte 0xC1, 0xEB, " MACRO_STR(count) " \n"
#define BIG_SHR_RSP(count) REX_W ".byte 0xC1, 0xEC, " MACRO_STR(count) " \n"
#define BIG_SHR_RBP(count) REX_W ".byte 0xC1, 0xED, " MACRO_STR(count) " \n"
#define BIG_SHR_RSI(count) REX_W ".byte 0xC1, 0xEE, " MACRO_STR(count) " \n"
#define BIG_SHR_RDI(count) REX_W ".byte 0xC1, 0xEF, " MACRO_STR(count) " \n"
#define BIG_SHR_R8(count)  REX_WB ".byte 0xC1, 0xE8, " MACRO_STR(count) " \n"
#define BIG_SHR_R9(count)  REX_WB ".byte 0xC1, 0xE9, " MACRO_STR(count) " \n"
#define BIG_SHR_R10(count) REX_WB ".byte 0xC1, 0xEA, " MACRO_STR(count) " \n"
#define BIG_SHR_R11(count) REX_WB ".byte 0xC1, 0xEB, " MACRO_STR(count) " \n"
#define BIG_SHR_R12(count) REX_WB ".byte 0xC1, 0xEC, " MACRO_STR(count) " \n"
#define BIG_SHR_R13(count) REX_WB ".byte 0xC1, 0xED, " MACRO_STR(count) " \n"
#define BIG_SHR_R14(count) REX_WB ".byte 0xC1, 0xEE, " MACRO_STR(count) " \n"
#define BIG_SHR_R15(count) REX_WB ".byte 0xC1, 0xEF, " MACRO_STR(count) " \n"
#define INC_EAX() ".byte 0xFF, 0xC0 \n"
#define INC_ECX() ".byte 0xFF, 0xC1 \n"
#define INC_EDX() ".byte 0xFF, 0xC2 \n"
#define INC_EBX() ".byte 0xFF, 0xC3 \n"
#define INC_ESP() ".byte 0xFF, 0xC4 \n"
#define INC_EBP() ".byte 0xFF, 0xC5 \n"
#define INC_ESI() ".byte 0xFF, 0xC6 \n"
#define INC_EDI() ".byte 0xFF, 0xC7 \n"
#define DEC_EAX() ".byte 0xFF, 0xC8 \n"
#define DEC_ECX() ".byte 0xFF, 0xC9 \n"
#define DEC_EDX() ".byte 0xFF, 0xCA \n"
#define DEC_EBX() ".byte 0xFF, 0xCB \n"
#define DEC_ESP() ".byte 0xFF, 0xCC \n"
#define DEC_EBP() ".byte 0xFF, 0xCD \n"
#define DEC_ESI() ".byte 0xFF, 0xCE \n"
#define DEC_EDI() ".byte 0xFF, 0xCF \n"
#define MOVSX_RAX_IMM32(value) REX_W ".byte 0xC7, 0xC0 \n .long " MACRO_STR(value) " \n"
#define MOVSX_RCX_IMM32(value) REX_W ".byte 0xC7, 0xC1 \n .long " MACRO_STR(value) " \n"
#define MOVSX_RDX_IMM32(value) REX_W ".byte 0xC7, 0xC2 \n .long " MACRO_STR(value) " \n"
#define MOVSX_RBX_IMM32(value) REX_W ".byte 0xC7, 0xC3 \n .long " MACRO_STR(value) " \n"
#define MOVSX_RSP_IMM32(value) REX_W ".byte 0xC7, 0xC4 \n .long " MACRO_STR(value) " \n"
#define MOVSX_RBP_IMM32(value) REX_W ".byte 0xC7, 0xC5 \n .long " MACRO_STR(value) " \n"
#define MOVSX_RSI_IMM32(value) REX_W ".byte 0xC7, 0xC6 \n .long " MACRO_STR(value) " \n"
#define MOVSX_RDI_IMM32(value) REX_W ".byte 0xC7, 0xC7 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R8_IMM32(value) REX_WB ".byte 0xC7, 0xC0 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R9_IMM32(value) REX_WB ".byte 0xC7, 0xC1 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R10_IMM32(value) REX_WB ".byte 0xC7, 0xC2 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R11_IMM32(value) REX_WB ".byte 0xC7, 0xC3 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R12_IMM32(value) REX_WB ".byte 0xC7, 0xC4 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R13_IMM32(value) REX_WB ".byte 0xC7, 0xC5 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R14_IMM32(value) REX_WB ".byte 0xC7, 0xC6 \n .long " MACRO_STR(value) " \n"
#define MOVSX_R15_IMM32(value) REX_WB ".byte 0xC7, 0xC7 \n .long " MACRO_STR(value) " \n"

static forceinline vec<uint32_t, 2> vec_arg_adapter(uint32_t low, uint32_t high) {
	return { low, high };
}

static forceinline uint64_t vec_ret_adapter(vec<uint32_t, 2> value) {
	return (uint64_t)((uint64_t)value[1] << 32 | value[0]);
}

//template<typename T>
dllexport gnu_noinline void regparm(3) write64_32(
	PTR64<> dst, uint32_t value
) {
	__asm__ volatile (
		//REX_W ".byte 0xC1, 0xE2, 0x20 \n" // "shl $0x20, %%rdx \n"
		BIG_SHL_RDX(0x20)
		"movl %%eax, %%eax \n"
		"movl %%ecx, (%%edx,%%eax) \n"
		"lret"
		:
	);
	unreachable;
}

dllexport gnu_noinline void regcall memcpy64_MMM(
	uint32_t, uint32_t, uint32_t,
	PTR64<> dst, PTR64<> src, uint64_t size
) {
	__asm__ volatile (
		REX_W "mov 24(%%esp), %%ecx \n" // "movq 24(%%rsp), %%rcx \n"
		REX_W "mov 8(%%esp), %%edi \n" // "movq 8(%%rsp), %%rdi \n"
		REX_W "mov 16(%%esp), %%esi \n" // "movq 16(%%rsp), %%rsi \n"
		"rep movsb \n"
		"lret $0x18"
		:::clobber_list("ecx" , "esi", "edi")
	);
	unreachable;
}
dllexport gnu_noinline void regcall memcpy64_XXX(
	PTR64<> dst, PTR64<> src, uint64_t size, uint32_t, uint32_t
) {
	__asm__ volatile (
		DATASIZE REX_W "movd %%mm2, %%ecx \n" // "movq %%xmm2, %%rcx \n"
		DATASIZE REX_W "movd %%mm1, %%esi \n" // "movq %%xmm2, %%rcx \n"
		DATASIZE REX_W "movd %%mm0, %%edi \n" // "movq %%xmm2, %%rcx \n"
		"rep movsb \n"
		"lret"
		:
	);
	unreachable;
}
dllexport gnu_noinline void regcall memcpy64_IRR(
	uint32_t, uint32_t size, uint32_t,
	PTR32Z<PTR64<>> dst, PTR32Z<> src
) {
	__asm__ volatile(
		ADDRSIZE REX_W "movl (%%edi), %%edi \n"
		"movl %%esi, %%esi \n"
		"movl %%ecx, %%ecx \n"
		"rep movsb \n"
		"lret"
		:
	);
	unreachable;
}
dllexport gnu_noinline void regcall memcpy64_RIR(
	uint32_t, uint32_t size, uint32_t,
	PTR32Z<> dst, PTR32Z<PTR64<>> src
) {
	__asm__ volatile(
		"movl %%edi, %%edi \n"
		ADDRSIZE REX_W "movl (%%esi), %%esi \n"
		"movl %%ecx, %%ecx \n"
		"rep movsb \n"
		"lret"
		:
	);
}

static seg_t x32_code_seg;
static seg_t x64_code_seg;

template<typename T = void>
struct FarPtr64Ex : FarPtr64<T> {

	template<typename T>
	gnu_noinline auto&& operator=(const T& value) && {

	}

	forceinline auto&& operator*() {
		return *this;
	}
	
	template<auto func, typename ... Args> requires std::invocable<std::decay_t<decltype(func)>, Args...>
	gnu_noinline auto operator() (Args&&... args) -> decltype(func(perfect_forward(Args, args))) {
		__asm__ volatile goto (
			"ljmp %l[func]"
			:
			:
			:
			: func
		);
		unreachable;
	}
};

template<typename T = void>
struct FarPtr32Ex : FarPtr32<T> {

	template<typename T>
	gnu_noinline auto&& operator=(const T& value) && {

	}

	forceinline auto&& operator*() {
		return *this;
	}
	
	template<typename ... Args> requires std::invocable<std::decay_t<T>, Args...>
	dllexport gnu_noinline auto operator() (Args... args) -> decltype(std::declval<T>()(args...)) {
		void* ret;
		__asm__ volatile (
			"lcall *%[dest]"
			: "=a"(ret)
			: [dest]"m"(*this)
		);
		return ret;
	}
};

struct NtDll64Data {
	PTR64<> ldr_get_procedure_addresss;
	PTR64<> ldr_load_dll_address;
	PTR64<> nt_allocate_virtual_memory;
	PTR64<> nt_query_virtual_memory;
	PTR64<> nt_free_virtual_memory;
	PTR64<> nt_protect_virtual_memory;
	PTR64<> nt_read_virtual_memory;
	PTR64<> nt_write_virtual_memory;
	HMODULE handle;
};
struct Kernel3264Data {
	PTR64<> get_proc_addresss;
	PTR64<> virtual_alloc_64;
	HMODULE handle;
};

extern "C" {
	dllexport NtDll64Data ntdll64_data asm("ntdll64_data");
	dllexport Kernel3264Data kernel3264_data asm("kernel3264_data");
}

dllexport gnu_noinline naked
	PTR64<>						// EDX:EAX, XMM0
regcall GetModuleHandleW64Adapter(
	PTR64<const wchar_t> name	// EDX:EAX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"SHL RDX, 32 \n"
#if !ASSUME_HIGH_REGS_ARE_ZERO
		"MOV EAX, EAX \n"
#endif
		"OR RAX, RDX \n"
		"JZ 3f \n"
		"MOV RCX, QWORD PTR GS:[0x60] \n"
		"MOV R8, QWORD PTR [RCX+0x18] \n"
		"MOV RDI, QWORD PTR [R8+0x10] \n"
		"ADD R8, 0x10 \n"
	"1: \n"
		"MOVZX ESI, WORD PTR [RDI+0x58] \n"
		"TEST ESI, ESI \n"
		"JNZ 1f \n"
	"2: \n"
		"MOV RDI, QWORD PTR [RDI] \n"
		"CMP RDI, R8 \n"
		"JNE 1b \n"
		"XOR EAX, EAX \n"
	"3: \n"
		"XORPS XMM0, XMM0 \n"
		"RETF \n"
	"1: \n"
		"MOV R9, QWORD PTR [RDI+0x60] \n"
		"MOV RDX, RAX \n"
		"SUB R9, RAX \n"
	"1: \n"
		"MOVZX ECX, BYTE PTR [RDX] \n"
		"CMP CL, BYTE PTR [RDX*1+R9] \n"
		"JNE 2b \n"
		"INC RDX \n"
		"DEC ESI \n"
		"JNZ 1b \n"
		"MOVQ XMM0, QWORD PTR [RDI+0x30] \n"
		"MOV RDX, QWORD PTR [RDI+0x30] \n"
		"MOV EAX, EDX \n"
		"SHR RDX, 32 \n"
		"RETF"
	);
}

template<typename T> requires(std::is_pointer_v<T> && std::is_same_v<remove_pointer_t<T>, const wchar_t>)
inline PTR64<> regparm(2) GetModuleHandleW64(T name) {
	PTR64<> ret;
	if constexpr (std::is_same_v<T, PTR64<const wchar_t>>) {
		FAR_CALL_IMM(0x33, GetModuleHandleW64Adapter,
					 "=A"(ret),
					 "a"((uint32_t)name), "d"((uint32_t)((uint64_t)name >> 32))
					 : clobber_list("ecx", "esi", "edi", "xmm0")
		);
	} else {
		FAR_CALL_IMM(0x33, GetModuleHandleW64Adapter,
					 "=A"(ret),
					 "a"((uint32_t)name), "d"(0)
					 : clobber_list("ecx", "esi", "edi", "xmm0")
		);
	}
	return ret;
}

static constexpr auto kjbsf = offsetof(IMAGE_DOS_HEADER, e_lfanew);
static constexpr auto wjklrkb = offsetof(IMAGE_NT_HEADERS64, FileHeader.Machine);
static constexpr auto wkjbwkjrb = sizeof(IMAGE_FILE_HEADER);
static constexpr auto kwjbrkwrb = offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);

dllexport gnu_noinline naked
	PTR64<>			// EDX:EAX, XMM0
regcall memset64Adapter(
	PTR64<> dst,	// EDX:EDI
	int val,		// EAX
	uint64_t size	// ESI:ECX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"SHL RSI, 32 \n"
		"MOV ECX, ECX \n"
		"OR RCX, RSI \n"
		"MOV EDI, EDI \n"
		"MOV ESI, EDX \n"
		"SHL RSI, 32 \n"
		"OR RDI, RSI \n"
		"MOVQ XMM0, RDI \n"
		"MOV ESI, EDI \n"
		"REP stosb \n"
		"MOV EAX, ESI \n"
		"RETF"
	);
}

template <typename T> requires(std::is_pointer_v<T> && std::is_same_v<remove_pointer_t<T>, void>)
inline PTR64<> regcall memset64(T dst, int val, uint64_t size) {
	PTR64<> ret;
	if constexpr (std::is_same_v<T, PTR64<>>) {
		FAR_CALL_IMM(0x33, memset64Adapter,
					 "=A"(ret),
					 "D"((uint32_t)dst), "d"((uint32_t)((uint64_t)dst >> 32)),
					 "a"(val),
					 "c"((uint32_t)size), "S"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else {
		FAR_CALL_IMM(0x33, memset64Adapter,
					 "=A"(ret),
					 "D"((uint32_t)dst), "d"(0),
					 "a"(val),
					 "c"((uint32_t)size), "S"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	}
	__asm__ volatile ("":::clobber_list("ecx", "esi", "edi"));
	return ret;
}

dllexport gnu_noinline naked
	PTR64<>			// EDX:EAX, XMM0
regcall memcpy64Adapter(
	PTR64<> dst,	// EDX:EAX
	PTR64<> src,	// EDI:ESI
	uint64_t size	// EBX:ECX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV ESI, ESI \n"
		"SHL RDI, 32 \n"
		"OR RSI, RDI \n"
		"SHL RBX, 32 \n"
		"MOV ECX, ECX \n"
		"OR RCX, RBX \n"
		"MOV EDI, EAX \n"
		"MOV EBX, EDX \n"
		"SHL RBX, 32 \n"
		"OR RDI, RBX \n"
		"MOVQ XMM0, RDI \n"
		"REP movsb \n"
		"RETF"
	);
}

template <typename T1, typename T2> requires(std::is_pointer_v<T1> && std::is_same_v<remove_pointer_t<T1>, void> && std::is_pointer_v<T2> && std::is_same_v<remove_pointer_t<T2>, void>)
inline PTR64<> regcall memcpy64(T1 dst, T2 src, uint64_t size) {
	PTR64<> ret;
	if constexpr (std::is_same_v<T1, PTR64<>> && std::is_same_v<T2, PTR64<>>) {
		FAR_CALL_IMM(0x33, memcpy64Adapter,
					 "=A"(ret),
					 "A"(dst),
					 "S"((uint32_t)src), "D"((uint32_t)((uint64_t)src >> 32)),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else if constexpr (std::is_same_v<T1, PTR64<>>) {
		FAR_CALL_IMM(0x33, memcpy64Adapter,
					 "=A"(ret),
					 "A"(dst),
					 "S"((uint32_t)src), "D"(0),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else if constexpr (std::is_same_v<T2, PTR64<>>) {
		FAR_CALL_IMM(0x33, memcpy64Adapter,
					 "=A"(ret),
					 "a"((uint32_t)dst), "d"(0)
					 "S"((uint32_t)src), "D"((uint32_t)((uint64_t)src >> 32)),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else {
		FAR_CALL_IMM(0x33, memcpy64Adapter,
					 "=A"(ret),
					 "a"((uint32_t)dst), "d"(0)
					 "S"((uint32_t)src), "D"(0),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	}
	__asm__ volatile ("":::clobber_list("ecx", "ebx", "esi", "edi"));
	return ret;
}

dllexport gnu_noinline naked
	PTR64<>			// EDX:EAX, XMM0
regcall memmove64Adapter(
	PTR64<> dst,	// EDX:EAX
	PTR64<> src,	// EDI:ESI
	uint64_t size	// EBX:ECX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV ESI, ESI \n"
		"SHL RDI, 32 \n"
		"OR RSI, RDI \n"
		"SHL RBX, 32 \n"
		"MOV ECX, ECX \n"
		"OR RCX, RBX \n"
		"MOV EDI, EAX \n"
		"MOV EBX, EDX \n"
		"SHL RBX, 32 \n"
		"OR RDI, RBX \n"
		"MOVQ XMM0, RDI \n"
		"CMP RDI, RSI \n"
		"JBE 1f \n"
		"STD \n"
		"LEA RDI, [RDI*1+RCX-1] \n"
		"LEA RSI, [RSI*1+RCX-1] \n"
	"1: \n"
		"REP movsb \n"
		"CLD \n"
		"RETF"
	);
}

template <typename T1, typename T2> requires(std::is_pointer_v<T1> && std::is_same_v<remove_pointer_t<T1>, void> && std::is_pointer_v<T2> && std::is_same_v<remove_pointer_t<T2>, void>)
inline PTR64<> regcall memmove64(T1 dst, T2 src, uint64_t size) {
	PTR64<> ret;
	if constexpr (std::is_same_v<T1, PTR64<>> && std::is_same_v<T2, PTR64<>>) {
		FAR_CALL_IMM(0x33, memmove64Adapter,
					 "=A"(ret),
					 "A"(dst),
					 "S"((uint32_t)src), "D"((uint32_t)((uint64_t)src >> 32)),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else if constexpr (std::is_same_v<T1, PTR64<>>) {
		FAR_CALL_IMM(0x33, memmove64Adapter,
					 "=A"(ret),
					 "A"(dst),
					 "S"((uint32_t)src), "D"(0),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else if constexpr (std::is_same_v<T2, PTR64<>>) {
		FAR_CALL_IMM(0x33, memmove64Adapter,
					 "=A"(ret),
					 "a"((uint32_t)dst), "d"(0)
					 "S"((uint32_t)src), "D"((uint32_t)((uint64_t)src >> 32)),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else {
		FAR_CALL_IMM(0x33, memmove64Adapter,
					 "=A"(ret),
					 "a"((uint32_t)dst), "d"(0)
					 "S"((uint32_t)src), "D"(0),
					 "c"((uint32_t)size), "b"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	}
	__asm__ volatile ("":::clobber_list("ecx", "ebx", "esi", "edi"));
	return ret;
}

dllexport gnu_noinline naked
	int				// EAX
regcall memcmp64Adapter(
	PTR64<> lhs,	// ESI:EAX
	PTR64<> rhs,	// EDI:EDX
	uint64_t size	// ECX:EBX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV EAX, EAX \n"
		"SHL RSI, 32 \n"
		"OR RSI, RAX \n"
		"MOV EDX, EDX \n"
		"SHL RDI, 32 \n"
		"OR RDI, RDX \n"
		"MOV EBX, EBX \n"
		"SHL RCX, 32 \n"
		"OR RCX, RBX \n"
		"REPE cmpsb \n"
		"SBB EAX, EAX \n"
		"SBB EAX, -1 \n"
		"TEST RCX, RCX \n"
		"CMOVZ EAX, ECX \n"
		"RETF"
	);
}

template <typename T1, typename T2> requires(std::is_pointer_v<T1> && std::is_same_v<remove_pointer_t<T1>, void> && std::is_pointer_v<T2> && std::is_same_v<remove_pointer_t<T2>, void>)
inline int regcall memcmp64(T1 lhs, T2 rhs, uint64_t size) {
	int ret;
	if constexpr (std::is_same_v<T1, PTR64<>> && std::is_same_v<T2, PTR64<>>) {
		FAR_CALL_IMM(0x33, memcmp64Adapter,
					 "=a"(ret),
					 "a"((uint32_t)lhs), "S"((uint32_t)((uint64_t)lhs >> 32)),
					 "d"((uint32_t)rhs), "D"((uint32_t)((uint64_t)rhs >> 32)),
					 "b"((uint32_t)size), "c"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else if constexpr (std::is_same_v<T1, PTR64<>>) {
		FAR_CALL_IMM(0x33, memcpy64Adapter,
					 "=a"(ret),
					 "a"((uint32_t)lhs), "S"((uint32_t)((uint64_t)lhs >> 32)),
					 "d"((uint32_t)rhs), "D"(0),
					 "b"((uint32_t)size), "c"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else if constexpr (std::is_same_v<T2, PTR64<>>) {
		FAR_CALL_IMM(0x33, memcpy64Adapter,
					 "=a"(ret),
					 "a"((uint32_t)lhs), "S"(0)
					 "d"((uint32_t)rhs), "D"((uint32_t)((uint64_t)rhs >> 32)),
					 "b"((uint32_t)size), "c"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	} else {
		FAR_CALL_IMM(0x33, memcpy64Adapter,
					 "=a"(ret),
					 "a"((uint32_t)lhs), "S"(0)
					 "d"((uint32_t)rhs), "D"(0),
					 "b"((uint32_t)size), "c"((uint32_t)(size >> 32))
					 : clobber_list("xmm0")
		);
	}
	__asm__ volatile ("":::clobber_list("ecx", "esi", "edi"));
	return ret;
}

dllexport gnu_noinline naked
	PTR64<>							// EDX:EAX, XMM0
regcall GetProcAddress64Adapter(
	PTR64<> module_handle,			// EDX:EAX
	PTR64<const char> lookup_name	// EDI:ECX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"XORPS XMM0, XMM0 \n"
		"SHL RDX, 32 \n"
		"MOV EAX, EAX \n"
		"LEA RSI, [RAX*1+RDX] \n"
		"MOV EAX, DWORD PTR [RAX*1+RDX+0x3C] \n"
		"MOV EDX, DWORD PTR [RSI*1+RAX+0x88] \n"
		"TEST RAX, RAX \n"
		"JZ 1f \n"
		"SHL RDI, 32 \n"
		"MOV ECX, ECX \n"
		"OR RCX, RDI \n"
		"CMP RCX, 0x10000 \n"
		"JB 2f \n"
		"MOV EDI, DWORD PTR [RDX*1+RSI+0x18] \n"
		"MOV R8D, DWORD PTR [RDX*1+RSI+0x20] \n"
		"ADD R8, RSI \n"
		"JMP 3f \n"
		".align 16, 0xCC \n"
	"4: \n"
		"CMP AL, R11B \n"
		"JE 4f \n"
	"3: \n"
		".byte 0x48, 0x81, 0xEF, 0x01, 0x00, 0x00, 0x00 \n" // "SUB RDI, 1 \n" (wide encoding)
		"JB 1f \n"
		"MOV R9D, DWORD PTR [RDI*4+R8] \n"
		"ADD RCX, RSI \n"
		"XOR R10D, R10D \n"
		".align 16 \n"
	"5: \n"
		"MOVZX R11D, BYTE PTR [R10*1+RCX] \n"
		"INC R10 \n"
		"MOVZX EAX, BYTE PTR [R9*1+R10-1] \n"
		"TEST R11B, R11B \n"
		"JZ 4b \n"
		"CMP AL, R11B \n"
		"JE 5b \n"
		"JMP 3b \n"
	"2: \n"
		"CMP ECX, DWORD PTR [RDX*1+RSI+0x14] \n"
		"JB 2f \n"
	"1: \n"
		"XOR EAX, EAX \n"
		"XOR EDX, EDX \n"
		"RETF \n"
	"4: \n"
		"MOV EAX, DWORD PTR [RDX*1+RSI+0x24] \n"
		"ADD RAX, RSI \n"
		"MOVZX ECX, WORD PTR [RDI*2+RAX] \n"
	"2: \n"
		"MOV EAX, DWORD PTR [RDX*1+RSI+0x1C] \n"
		"ADD RAX, RSI \n"
		"MOV EAX, DWORD PTR [RCX*4+RAX] \n"
		"ADD RAX, RSI \n"
		"MOVQ XMM0, RAX \n"
		"MOV RDX, RAX \n"
		"SHR RDX, 32 \n"
		"RETF"
	);
}

template <typename T> requires(std::is_pointer_v<T> && std::is_same_v<remove_pointer_t<T>, const char>)
inline PTR64<> regcall GetProcAddress64(PTR64<> module_handle, T search_name) {
	PTR64<> ret;
	if constexpr (std::is_same_v<T, PTR64<const char>>) {
		FAR_CALL_IMM(0x33, GetProcAddress64Adapter,
					 "=A"(ret),
					 "A"(handle_to_base_ptr(module_handle)),
					 "c"((uint32_t)search_name), "D"((uint32_t)((uint64_t)search_name >> 32))
					 : clobber_list("esi", "xmm0")
		);
	} else {
		FAR_CALL_IMM(0x33, GetProcAddress64Adapter,
					 "=A"(ret),
					 "A"(handle_to_base_ptr(module_handle)),
					 "c"((uint32_t)search_name), "D"(0)
					 : clobber_list("esi", "xmm0")
		);
	}
	__asm__ volatile ("":::clobber_list("ecx", "edi"));
	return ret;
}

/*
dllexport gnu_noinline naked
	PTR64<>						// EDX:EAX, XMM0
	// NTSTATUS					// ECX
regcall VirtualAlloc64(
	PTR64<> address,			// EDX:EAX
	uint64_t size,				// EDI:ECX
	uint32_t allocation_type,	// ESI
	uint32_t protection			// EBX
) {
	__asm__ volatile (
		//DATASIZE REX_W "movd %ebp, %mm6 \n" // "movq %rbp, %xmm6 \n"
		//DATASIZE REX_W "movd %esp, %mm7 \n" // "movq %rsp, %xmm7 \n"
		"movl %ebp, %ebp \n"
		"pushl %ebp \n" // "pushq %rbp \n"
		"movl %esp, %ebp \n"
		"subl $0x40, %esp \n"
		"andl $-0x10, %esp \n"
		//REX_W ".byte 0xC1, 0xE2, 0x20 \n" // "shlq $0x20, %rdx \n"
		BIG_SHL_RDX(0x20)
		"movl %eax, %eax \n"
		REX_W "orl %edx, %eax \n" // "orq %rdx, %rax \n"
		REX_W "movl %eax, 0x30(%esp) \n" // "movq %rax, 0x30(%rsp) \n"
		//REX_W ".byte 0xC1, 0xE7, 0x20 \n" // "shlq $0x20, %rdi \n"
		BIG_SHL_RDI(0x20)
		"movl %ecx, %eax \n"
		REX_W "orl %edi, %eax \n" // "orq %rdi, %rax \n"
		REX_W "movl %eax, 0x38(%esp) \n" // "movq %rax, 0x38(%rsp) \n"
		"movl %ebx, 0x28(%esp) \n" // "movl %ebx, 0x28(%rsp) \n"
		"movl %esi, 0x20(%esp) \n" // "movl %esi, 0x20(%rsp) \n"
		"xorl %esi, %esi \n"
		REX_W "leal 0x30(%esp), %edx \n" // "leaq 0x30(%rsp), %rdx \n"
		REX_WR "leal 0x38(%esp), %ecx \n" // "leaq 0x38(%rsp), %r9 \n"
		//REX_W ".byte 0xC7, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF \n" // "movq -1, %rcx \n"
		MOVSX_RCX_IMM32(-1)
		REX_RB "xorl %eax, %eax \n" // "xorl %r8d, r8d \n"
		CALL_PTR_LOW(ntdll64_data + 0x10)
		"movl %eax, %ecx \n"
		"testl %eax, %eax \n"
		REX_W "cmovnsl 0x30(%esp), %esi \n" // "cmovnsq 0x30(%rsp), %rsi \n"
		DATASIZE REX_W "movd %esi, %mm0 \n" // "movq %rsi, %xmm0 \n"
		"movl %esi, %eax \n"
		REX_W "movl %esi, %edx \n" // "movq %rsi, %rdx \n"
		//REX_W ".byte 0xC1, 0xEA, 0x20 \n" // "shrq $0x20, %rdx \n"
		BIG_SHR_RDX(0x20)
		"leavel \n" // "leaveq \n"
		"lret \n"
	);
}
*/

dllexport gnu_noinline naked
	PTR64<>						// EDX:EAX, XMM0
	// NTSTATUS					// ECX
regcall VirtualAlloc64Adapter(
	PTR64<> base_address,		// EDX:EAX
	uint64_t size,				// EDI:ECX
	uint32_t allocation_type,	// ESI
	uint32_t protection			// EBX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV R8D, ESI \n"
		"MOV ESI, ESP \n"
		"SUB ESP, 0x48 \n"
		"AND ESP, -0x10 \n"
		"MOV EAX, EAX \n"
		"SHL RDX, 32 \n"
		"OR RDX, RAX \n"
		"MOV QWORD PTR [RSP+0x38], RDX \n"
		"SHL RDI, 32 \n"
		"MOV EAX, ECX \n"
		"OR RAX, RDI \n"
		"MOV QWORD PTR [RSP+0x40], RAX \n"
		"MOV DWORD PTR [RSP+0x28], EBX \n"
		"MOV DWORD PTR [RSP+0x20], R8D \n"
		"XOR EDI, EDI \n"
		"LEA RDX, [RSP+0x38] \n"
		"LEA R9, [RSP+0x40] \n"
		"MOV RCX, -1 \n"
		"XOR R8D, R8D \n"
		"CALL QWORD PTR [ntdll64_data + 0x10] \n"
		"MOV ECX, EAX \n"
		"TEST EAX, EAX \n"
		"CMOVNS RDI, QWORD PTR [RSP+0x38] \n"
		"MOV RDX, RDI \n"
		"SHR RDX, 32 \n"
		"MOVQ XMM0, RDI \n"
		"MOV RAX, RDI \n"
		"MOV ESP, ESI \n"
		"RETF"
	);
}

template<bool return_status = false, typename R = std::conditional_t<return_status, std::pair<PTR64<>,NTSTATUS>, PTR64<>>>
inline R
VirtualAlloc64(
	PTR64<> address,
	uint64_t size,
	uint32_t allocation_type,
	uint32_t protection
) {
	PTR64<> ret;
	NTSTATUS status;
	FAR_CALL_IMM(
		0x33, VirtualAlloc64Adapter,
		MACRO_EVAL("=A"(ret), "=c"(status)),
		"A"(address),
		"c"((uint32_t)size), "D"((uint32_t)(size >> 32)),
		"S"(allocation_type),
		"b"(protection)
		: clobber_list("xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5")
	);
	__asm__ volatile ("":::clobber_list("esi", "edi"));
	if constexpr (return_status) {
		return std::make_pair(ret, status);
	} else {
		return ret;
	}
}

/*
struct VirtualQueryInfoBuffer {
	uint64_t size;
	unsigned char data[];
};
dllexport gnu_noinline naked
	uint64_t								// EDX:EAX, XMM0
	// NTSTATUS								// ECX
regcall VirtualQuery64(
	PTR64<> base_address,					// EDX:EAX
	MEMORY_INFORMATION_CLASS info_class,	// ECX
	PTR64<VirtualQueryInfoBuffer> info_ptr,	// ESI:EDI
	PTR64<uint64_t> return_length			// EBP:EBX
) {
	__asm__ volatile (
		REX_B "movl %ebx, %eax \n" // "movl %ebx, %r8d \n"
		"movl %esp, %ebx \n"
		"subl $0x50, %esp \n"
		"andl $-0x10, %esp \n"
		//REX_W ".byte 0xC1, 0xE2, 0x20 \n" // "shlq $0x20, %rdx \n"
		BIG_SHL_RDX(0x20)
		"movl %eax, %eax \n"
		REX_W "orl %eax, %edx \n" // "orq %rax, %rdx \n"
		//REX_W ".byte 0xC1, 0xE6, 0x20 \n" // "shlq $0x20, %rsi \n"
		BIG_SHL_RSI(0x20)
		REX_B "movl %edi, %ecx \n" // "movl %edi, %r9d \n"
		REX_WB "orl %esi, %ecx \n" // "orq %rsi, %rcx \n"
		//REX_W ".byte 0xC1, 0xE5, 0x20 \n" // "shlq $0x20, %rbp \n"
		BIG_SHL_RBP(0x20)
		REX_WR "addl %eax, %ebp \n" // "addq %r8, %rbp \n"
		REX_B "movl %ecx, %eax \n" // "movl %ecx, %r8d \n"
		REX_W "leal 0x30(%esp), %ecx \n" // "leaq 0x30(%rsp), %rcx \n"
		REX_W "movl %ecx, 0x28(%esp) \n" // "movq %rcx, 0x28(%rsp) \n"
		REX_W "movl %ebp, 0x20(%esp) \n" // "movq %rbp, 0x20(%rsp) \n"
		//REX_W ".byte 0xC7, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF \n" // "movq -1, %rcx \n"
		MOVSX_RCX_IMM32(-1)
		CALL_PTR_LOW(ntdll64_data + 0x18)
		"movl %eax, %ecx \n"
		"testl %ecx, %ecx \n"
		REX_W "cmovnsl 0x30(%esp), %eax \n" // "cmovnsq 0x30(%rsp), %rax \n"
		DATASIZE REX_W "movd %eax, %mm0 \n" // "movq %rax, %xmm0 \n"
		REX_W "movl %eax, %edx \n" // "movq %rax, %rdx \n"
		//REX_W ".byte 0xC1, 0xEA, 0x20 \n" // "shrq $0x20, %rdx \n"
		BIG_SHR_RDX(0x20)
		"movl %ebx, %esp \n"
		"lret"
	);
}
*/

dllexport gnu_noinline naked
	size_t								// EDX:EAX, XMM0
	// NTSTATUS							// ECX
regcall VirtualQuery64Adapter(
	PTR64<> address,					// EDX:EAX
	PTR64<> query_info,					// EDI:ECX
	uint64_t query_info_size,			// EBX:ESI
	MEMORY_INFORMATION_CLASS info_class	// EBP
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV R8D, EBP \n"
		"MOV EBP, ESP \n"
		"SUB ESP, 0x38 \n"
		"AND ESP, -0x10 \n"
		"MOV EAX, EAX \n"
		"SHL RDX, 32 \n"
		"OR RDX, RAX \n"
		"MOV R9D, ECX \n"
		"SHL RDI, 32 \n"
		"OR R9, RDI \n"
		"SHL RBX, 32 \n"
		"MOV EAX, ESI \n"
		"OR RAX, RBX \n"
		"XOR EDI, EDI \n"
		"LEA RCX, [RSP+0x30] \n"
		"MOV QWORD PTR [RSP+0x28], RCX \n"
		"MOV QWORD PTR [RSP+0x20], RAX \n"
		"MOV RCX, -1 \n"
		"CALL QWORD PTR [ntdll64_data + 0x18] \n"
		"MOV ECX, EAX \n"
		"TEST EAX, EAX \n"
		"CMOVNS RDI, QWORD PTR [RSP+0x30] \n"
		"MOV RDX, RDI \n"
		"SHR RDX, 32 \n"
		"MOVQ XMM0, RDI \n"
		"MOV RAX, RDI \n"
		"MOV ESP, EBP \n"
		"RETF"
	);
}

template<bool return_status = false, typename T, typename R = std::conditional_t<return_status, std::pair<uint64_t, NTSTATUS>, uint64_t>> requires(std::is_pointer_v<T> && std::is_same_v<remove_pointer_t<T>, void>)
inline R
VirtualQuery64(
	PTR64<> address,
	T query_info,
	uint64_t query_info_size,
	MEMORY_INFORMATION_CLASS info_class_in = MemoryBasicInformation
) {
	uint64_t ret;
	NTSTATUS status;
	register MEMORY_INFORMATION_CLASS info_class asm("ebp") = info_class_in;
	if constexpr (std::is_same_v<T, PTR64<>>) {
		FAR_CALL_IMM(
			0x33, VirtualQuery64Adapter,
			MACRO_EVAL("=A"(ret), "=c"(status)),
			"A"(address),
			"c"((uint32_t)query_info), "D"((uint32_t)((uint64_t)query_info >> 32)),
			"S"((uint32_t)query_info_size), "b"((uint32_t)(query_info_size >> 32)),
			"R"(info_class)
			: clobber_list("xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5")
		);
	} else {
		FAR_CALL_IMM(
			0x33, VirtualQuery64Adapter,
			MACRO_EVAL("=A"(ret), "=c"(status)),
			"A"(address),
			"c"((uint32_t)query_info), "D"(0),
			"S"((uint32_t)query_info_size), "b"((uint32_t)(query_info_size >> 32)),
			"R"(info_class)
			: clobber_list("xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5")
		);
	}
	__asm__ volatile ("":::clobber_list("ebx", "ebp", "edi"));
	if constexpr (return_status) {
		return std::make_pair(ret, status);
	} else {
		return ret;
	}
}

/*
dllexport gnu_noinline naked
	NTSTATUS						// EAX
regcall VirtualFree64(
	PTR64<PTR64<>> base_address,	// EDX:EAX
	PTR64<uint64_t> size,			// EDI:ECX
	uint32_t free_type				// ESI
) {
	__asm__ volatile (
		//REX_W ".byte 0xC1, 0xE7, 0x20 \n" // "shlq $0x20, %rdi \n"
		BIG_SHL_RDI(0x20)
		"movl %ecx, %ecx \n"
		REX_W "orl %edi, %ecx \n" // "orq %rdi, %rcx \n"
		"testw %si, %si \n"
		"js 1f \n"
	"2: \n"
		"movl %ebp, %ebp \n"
		"pushl %ebp \n" // "pushq %rbp \n"
		"movl %esp, %ebp \n"
		"subl $0x40, %esp \n"
		"andl $-0x10, %esp \n"
		//REX_W ".byte 0xC1, 0xE2, 0x20 \n" // "shlq $0x20, %rdx \n"
		BIG_SHL_RDX(0x20)
		"movl %eax, %eax \n"
		REX_W "orl %edx, %eax \n" // "orq %rdx, %rax \n"
		REX_W "movl %eax, 0x28(%esp) \n" // "movq %rax, 0x28(%rsp) \n"
		REX_W "movl %ecx, 0x20(%esp) \n" // "movq %rcx, 0x20(%rsp) \n"
		REX_W "leal 0x28(%esp), %edx \n" // "leaq 0x28(%rsp), %rdx \n"
		REX_WR "leal 0x20(%esp), %eax \n" // "leaq 0x20(%rsp), %r8 \n"
		//REX_W ".byte 0xC7, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF \n" // "movq -1, %rcx \n"
		MOVSX_RCX_IMM32(-1)
		REX_B "movl %esi, %ecx \n" // "movl %esi, %r9d \n"
		CALL_PTR_LOW(ntdll64_data + 0x20)
		"leavel \n" // "leaveq \n"
		"lret \n"
	"1: \n"
		REX_W "testl %ecx, %ecx \n" // "testq %rcx, %rcx \n"
		"jz 2b \n"
		"movl $0xC000000D, %eax \n"
		"lret"
	);
}
*/

dllexport gnu_noinline naked
	BOOL							// EAX
	// NTSTATUS						// ECX
regcall VirtualFree64Adapter(
	PTR64<PTR64<>> base_address,	// EDX:EAX
	PTR64<uint64_t> size,			// EDI:ECX
	uint32_t free_type				// ESI
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV R9D, ESI \n"
		"MOV ESI, ESP \n"
		"SUB ESP, 0x30 \n"
		"AND ESP, -0x10 \n"
		"MOV EAX, EAX \n"
		"SHL RDX, 32 \n"
		"OR RDX, RAX \n"
		"MOV QWORD PTR [RSP+0x28], RDX \n"
		"SHL RDI, 32 \n"
		"MOV EAX, ECX \n"
		"OR RAX, RDI \n"
		"MOV QWORD PTR [RSP+0x20], RAX \n"
		"MOV RCX, -1 \n"
		"CALL QWORD PTR [ntdll64_data + 0x20] \n"
		"MOV ECX, EAX \n"
		"NOT EAX \n"
		"SHR EAX, 31 \n"
		"MOV ESP, ESI \n"
		"RETF"
	);
}

template<bool return_status = false, typename R = std::conditional_t<return_status, std::pair<BOOL, NTSTATUS>, BOOL>>
inline R
VirtualFree64(
	PTR64<> base_address,
	uint64_t size,
	uint32_t free_type
) {
	BOOL ret;
	NTSTATUS status;
	FAR_CALL_IMM(
		0x33, VirtualFree64Adapter,
		MACRO_EVAL("=a"(ret), "=c"(status)),
		"A"(base_address),
		"c"((uint32_t)size), "D"((uint32_t)(size >> 32)),
		"S"(free_type)
		: clobber_list("xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5")
	);
	__asm__ volatile ("":::clobber_list("esi", "edi"));
	if constexpr (return_status) {
		return std::make_pair(ret, status);
	} else {
		return ret;
	}
}

dllexport gnu_noinline naked
	BOOL							// EAX
	// NTSTATUS						// ECX
regcall VirtualProtect64Adapter(
	PTR64<> base_address,			// EDX:EAX
	uint64_t size,					// EDI:ECX
	uint32_t new_protection,		// ESI
	PTR64<uint32_t> old_protection	// EBP:EBX
) {
	__asm__ volatile (
		INTEL_64_DIRECTIVE
		"MOV R9D, ESI \n"
		"MOV ESI, ESP \n"
		"SUB ESP, 0x38 \n"
		"AND ESP, -0x10 \n"
		"MOV EAX, EAX \n"
		"SHL RDX, 32 \n"
		"OR RDX, RAX \n"
		"MOV QWORD PTR [RSP+0x30], RDX \n"
		"SHL RDI, 32 \n"
		"MOV EAX, ECX \n"
		"OR RAX, RDI \n"
		"MOV QWORD PTR [RSP+0x28], RAX \n"
		"MOV EAX, EBX \n"
		"SHL RBP, 32 \n"
		"OR RAX, RBP \n"
		"MOV QWORD PTR [RSP+0x20], RAX \n"
		"LEA RDX, [RSP+0x30] \n"
		"LEA R8, [RSP+0x28] \n"
		"MOV RCX, -1 \n"
		"CALL QWORD PTR [ntdll64_data + 0x28] \n"
		"MOV ECX, EAX \n"
		"NOT EAX \n"
		"SHR EAX, 31 \n"
		"MOV ESP, ESI \n"
		"RETF"
	);
}

template<bool return_status = false, typename T, typename R = std::conditional_t<return_status, std::pair<BOOL, NTSTATUS>, BOOL>>  requires(std::is_pointer_v<T> && std::is_same_v<remove_pointer_t<T>, uint32_t>)
inline R
VirtualProtect64(
	PTR64<> base_address,
	uint64_t size,
	uint32_t new_protection,
	T old_protection
) {
	BOOL ret;
	NTSTATUS status;
	if constexpr (std::is_same_v<T, PTR64<uint32_t>>) {
		register uint32_t old_protection_high asm("ebp") = (uint64_t)old_protection >> 32;
		FAR_CALL_IMM(
			0x33, VirtualProtect64Adapter,
			MACRO_EVAL("=a"(ret), "=c"(status)),
			"A"(base_address),
			"c"((uint32_t)size), "D"((uint32_t)(size >> 32)),
			"S"(new_protection),
			"b"((uint32_t)old_protection), "R"(old_protection_high)
			: clobber_list("xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5")
		);
	} else {
		register uint32_t old_protection_high asm("ebp") = 0;
		FAR_CALL_IMM(
			0x33, VirtualProtect64Adapter,
			MACRO_EVAL("=a"(ret), "=c"(status)),
			"A"(base_address),
			"c"((uint32_t)size), "D"((uint32_t)(size >> 32)),
			"S"(new_protection),
			"b"((uint32_t)old_protection), "R"(old_protection_high)
			: clobber_list("xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5")
		);
	}
	__asm__ volatile ("":::clobber_list("edx", "ebp", "esi", "edi"));
	if constexpr (return_status) {
		return std::make_pair(ret, status);
	} else {
		return ret;
	}
}

dllexport inline bool init_unwow64_ntdll() {
	if (PTR64<> ntdll64_handle = GetModuleHandleW64(L"ntdll.dll")) {
		PTR64<> nt_allocate_virtual_memory = ntdll64_data.nt_allocate_virtual_memory = GetProcAddress64(ntdll64_handle, "NtAllocateVirtualMemory");
		PTR64<> nt_query_virtual_memory = ntdll64_data.nt_query_virtual_memory = GetProcAddress64(ntdll64_handle, "NtQueryVirtualMemory");
		PTR64<> nt_free_virtual_memory = ntdll64_data.nt_free_virtual_memory = GetProcAddress64(ntdll64_handle, "NtFreeVirtualMemory");
		PTR64<> nt_protect_virtual_memory = ntdll64_data.nt_protect_virtual_memory = GetProcAddress64(ntdll64_handle, "NtProtectVirtualMemory");
		PTR64<> nt_read_virtual_memory = ntdll64_data.nt_read_virtual_memory = GetProcAddress64(ntdll64_handle, "NtReadVirtualMemory");
		PTR64<> nt_write_virtual_memory = ntdll64_data.nt_write_virtual_memory = GetProcAddress64(ntdll64_handle, "NtWriteVirtualMemory");
		if (
			nt_allocate_virtual_memory &&
			nt_query_virtual_memory &&
			nt_free_virtual_memory &&
			nt_protect_virtual_memory &&
			nt_read_virtual_memory &&
			nt_write_virtual_memory
		) {
			return true;
		}
	}
	return false;
}
/*
extern "C" {
	dllexport PTR64<> virtual_alloc_64 asm("virtual_alloc_64");
}

dllexport gnu_noinline naked PTR64<> regcall VirtualAlloc64_old(PTR64<> addr, uint64_t size, uint32_t type, uint32_t protection) {
	__asm__ volatile (
		"pushl %ebp \n" // "pushq %rbp \n"
		"movl %esp, %ebp \n"
		"subl $0x30, %esp \n"
		"andl $-0x10, %esp \n"
		//REX_W ".byte 0xC1, 0xE3, 0x20 \n" // "shlq $0x20, %rbx \n"
		BIG_SHL_RBX(0x20)
		"movl %ecx, %ecx \n"
		REX_W "orl %ecx, %ebx \n" // "orq %rcx, %rbx \n"
		//REX_W ".byte 0xC1, 0xE2, 0x20 \n" // "shlq $0x20, %rdx \n"
		BIG_SHL_RDX(0x20)
		"movl %eax, %ecx \n"
		REX_W "orl %edx, %ecx \n" // "orq %rdx, %rcx \n"
		REX_W "movl %ebx, %edx \n" // "movq %rbx, %rdx \n"
		REX_B "movl %edi, %eax \n" // "movl %edi, %r8d \n"
		REX_B "movl %esi, %ecx \n" // "movl %esi, %r9d \n"
		CALL_PTR_LOW(virtual_alloc_64) // "CALL QWORD PTR %[virtual_alloc_64] \n"
		DATASIZE REX_W "movd %eax, %mm0 \n" // "movq %rax, %xmm0 \n"
		REX_W "movl %eax, %edx \n" // "movq %rax, %rdx \n"
		//REX_W ".byte 0xC1, 0xEA, 0x20 \n" // "shrq $0x20, %rdx \n"
		BIG_SHR_RDX(0x20)
		"leavel \n" // "leaveq \n"
		"lret"
	);
}
*/

#define STATUS_INVALID_PARAMETER_1 0xC00000EF
#define STATUS_INVALID_PARAMETER_5 0xC00000F3

static constexpr uint64_t low_48_bit_canonical_address_min =		0x0000000000000000ull;
static constexpr uint64_t low_48_bit_canonical_address_max =		0x00007FFFFFFFFFFFull;
static constexpr uint64_t invalid_48_bit_canonical_address_min =	0x0000800000000000ull;
static constexpr uint64_t invalid_48_bit_canonical_address_max =	0xFFFF7FFFFFFFFFFFull;
static constexpr uint64_t high_48_bit_canonical_address_min =		0xFFFF800000000000ull;
static constexpr uint64_t high_48_bit_canonical_address_max =		0xFFFFFFFFFFFFFFFFull;

static constexpr uint64_t low_57_bit_canonical_address_min =		0x0000000000000000ull;
static constexpr uint64_t low_57_bit_canonical_address_max =		0x00FFFFFFFFFFFFFFull;
static constexpr uint64_t invalid_57_bit_canonical_address_min =	0x0100000000000000ull;
static constexpr uint64_t invalid_57_bit_canonical_address_max =	0xFEFFFFFFFFFFFFFFull;
static constexpr uint64_t high_57_bit_canonical_address_min =		0xFF00000000000000ull;
static constexpr uint64_t high_57_bit_canonical_address_max =		0xFFFFFFFFFFFFFFFFull;

static constexpr uint64_t allocation_size = 0x100000000;
static constexpr uint64_t starting_allocation_addr = 0x100000000;

static constexpr uint32_t low_allocation_size = (uint32_t)allocation_size;
static constexpr uint32_t high_allocation_size = (uint32_t)(allocation_size >> 32);

dllexport gnu_noinline void regcall query_memory_jank(uint64_t base_address) {
	//MEMORY_BASIC_INFORMATION64 memory_info;
	//auto byte_count = VirtualQuery64((PTR64<>)base_address, (void*)&memory_info, sizeof(MEMORY_BASIC_INFORMATION64));

}

dllexport gnu_noinline bool bs_dll64() {
	if (init_unwow64_ntdll()) {
		for (
			uint64_t base_addr = starting_allocation_addr;
			base_addr < (invalid_48_bit_canonical_address_min - allocation_size);
			base_addr += page_size
		) {
			auto [huge_allocation_ptr, status] = VirtualAlloc64<true>((PTR64<>)base_addr, allocation_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			switch ((DWORD)status) {
				case STATUS_NO_MEMORY:
				case STATUS_INVALID_PARAMETER_1 ... STATUS_INVALID_PARAMETER_5:
					return false;
				case STATUS_CONFLICTING_ADDRESSES:
					continue;
				default:
					if (SUCCEEDED(status)) {
						PTR64<> temp = huge_allocation_ptr;
						memset64(temp, 69, allocation_size);
						return true;
					}
			}
		}
	}
	return false;
}

#elif 0
static constexpr const bool is_x64 = true;
static const bool is_wow_64 = false;


dllexport gnu_noinline int32_t nt_free_dummy_stub(
	int64_t handle,
	void** address,
	size_t* size,
	uint32_t free_type
) {
	use_var(handle);
	use_var(address);
	use_var(size);
	use_var(free_type);
	return rand();
}

dllexport gnu_noinline gnu_attr(force_align_arg_pointer) int regcall call_nt_free_dummy_stub(
	uint64_t address_low,
	uint64_t size_low,
	uint64_t address_high,
	uint64_t size_high,
	uint64_t free_type
) {
	void* address = unpack_hilo<void*>(address_low, address_high);
	size_t size = unpack_hilo<size_t>(size_low, size_high);
	int32_t status = nt_free_dummy_stub(-1, &address, &size, free_type);
	__asm__ volatile (""::"c"(status));
	return SUCCEEDED(status);
}

dllexport gnu_noinline int32_t nt_allocate_dummy_stub(
	int64_t handle,
	void** address,
	size_t zero_bits,
	size_t* size,
	uint32_t allocation_type,
	uint32_t protection
) {
	use_var(handle);
	use_var(address);
	use_var(zero_bits);
	use_var(size);
	use_var(allocation_type);
	use_var(protection);
	return rand();
}

dllexport gnu_noinline gnu_attr(force_align_arg_pointer) void* regcall call_nt_allocate_dummy_stub(
	uint64_t address_low,
	uint64_t size_low,
	uint64_t address_high,
	uint64_t size_high,
	uint64_t allocation_type,
	uint64_t protection
) {
	__asm__ volatile (
		""
		: "=b"(protection)
	);
	void* address = unpack_hilo<void*>(address_low, address_high);
	size_t size = unpack_hilo<size_t>(size_low, size_high);
	void* ret = NULL;
	int32_t status = nt_allocate_dummy_stub(-1, &address, 0, &size, allocation_type, protection);
	if (SUCCEEDED(status)) ret = address;
	__asm__ volatile (
		""
		:
		: "c"(status), "d"((uintptr_t)ret >> 32), "Yz"(ret)
	);
	return ret;
}

dllexport gnu_noinline int32_t nt_query_dummy_stub(
	int64_t handle,
	void* address,
	MEMORY_INFORMATION_CLASS info_class,
	void* query_info,
	size_t query_info_size,
	size_t* return_length
) {
	use_var(handle);
	use_var(address);
	use_var(info_class);
	use_var(query_info);
	use_var(query_info_size);
	use_var(return_length);
	return rand();
}

dllexport gnu_noinline gnu_attr(force_align_arg_pointer) size_t regcall call_nt_query_dummy_stub(
	uint64_t address_low,
	uint64_t query_info_low,
	uint64_t address_high,
	uint64_t query_info_high,
	uint64_t query_info_size_low,
	uint64_t query_info_size_high,
	MEMORY_INFORMATION_CLASS info_class_in = MemoryBasicInformation
) {
	register MEMORY_INFORMATION_CLASS info_class asm("r8d") = info_class_in;
	__asm__ volatile (
		""
		: "=b"(query_info_size_high), "=r"(info_class)
	);
	void* address = unpack_hilo<void*>(address_low, address_high);
	void* query_info = unpack_hilo<void*>(query_info_low, query_info_high);
	size_t query_info_size = unpack_hilo<size_t>(query_info_size_low, query_info_size_high);
	size_t ret_mem;
	size_t ret = 0;
	__asm__ volatile ("":"+r"(ret));
	int32_t status = nt_query_dummy_stub(-1, address, info_class, query_info, query_info_size, &ret_mem);
	if (SUCCEEDED(status)) ret = ret_mem;
	__asm__ volatile (
		""
		:
		: "c"(status), "d"(ret >> 32), "Yz"(ret)
	);
	return ret;
}

dllexport gnu_noinline int32_t nt_protect_dummy_stub(
	int64_t handle,
	void** base_address,
	size_t* size,
	uint32_t new_protection,
	uint32_t* old_protection
) {
	use_var(handle);
	use_var(base_address);
	use_var(size);
	use_var(new_protection);
	use_var(old_protection);
	return rand();
}

dllexport gnu_noinline gnu_attr(force_align_arg_pointer) int32_t regcall call_nt_protect_dummy_stub(
	uint64_t address_low,
	uint64_t size_low,
	uint64_t address_high,
	uint64_t size_high,
	uint64_t new_protection,
	uint64_t old_protection_low,
	uint64_t old_protection_high_in
) {
	register uint64_t old_protection_high asm("rbp") = old_protection_high_in;
	__asm__ volatile (
		""
		: "=b"(old_protection_low), "=R"(old_protection_high)
	);
	void* address = unpack_hilo<void*>(address_low, address_high);
	size_t size = unpack_hilo<size_t>(size_low, size_high);
	uint32_t* old_protection = unpack_hilo<uint32_t*>(old_protection_low, old_protection_high);
	int32_t status = nt_protect_dummy_stub(-1, &address, &size, new_protection, old_protection);
	__asm__ volatile (""::"c"(status));
	return SUCCEEDED(status);
}

template<typename T>
struct LIST_ENTRY_NEW {
	T* Flink; // 0x0
	T* Blink; // 0x8
	// 0x10
};

struct LDR_DATA_TABLE_ENTRY_NEW;
struct LDR_DATA_TABLE_ENTRY_NEW {
	LIST_ENTRY_NEW<LDR_DATA_TABLE_ENTRY_NEW> InLoadOrderLinks;
	LIST_ENTRY_NEW<LDR_DATA_TABLE_ENTRY_NEW> InMemoryOrderLinks;
	LIST_ENTRY_NEW<LDR_DATA_TABLE_ENTRY_NEW> InInitializationOrderLinks;
	void* DllBase;
	void* EntryPoint;
	uint32_t SizeOfImage;
	uint32_t dummyA;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
};

struct PEB_LDR_DATA_NEW {
	ULONG Length; // 0x0
	BOOLEAN Initialized; // 0x4
	padding_bytes(3);
	PVOID SsHandle; // 0x8
	LIST_ENTRY_NEW<LDR_DATA_TABLE_ENTRY_NEW> InLoadOrderModuleList; // 0x10
};

#define USE_JANK_MODULE_LOOP 2

static inline gnu_attr(target("no-sse3")) bool byteloop_memcmp(
	const uint8_t *restrict lhs,
	const uint8_t *restrict rhs,
	const size_t size
) {
	size_t true_size = size + (uintptr_t)rhs;
	const uint8_t *restrict lhs2 = based_pointer< const uint8_t>(lhs, -(uintptr_t)rhs);
	while ((size_t)rhs != true_size) {
		if (*rhs != *lhs2) return false;
		++rhs;
		++lhs2;
	}
	return true;
}
static inline gnu_attr(target("no-sse3")) bool byteloop_memcmp2(
	const uint8_t *restrict lhs,
	const uint8_t *restrict rhs,
	uint32_t size
) {
	const uint8_t *restrict lhs2 = based_pointer< const uint8_t>(lhs, -(uintptr_t)rhs);
	__asm__ volatile (""::"R"(lhs2));
	if (expect(size, false)) {
		//__asm__ volatile (""::"R"(size)/*,"R"(lhs2),"R"(rhs)*/);
		bool not_zero;
		do {
			uint8_t c = *rhs;
			if (expect(c != lhs2[(uintptr_t)rhs], false)) return false;
			//++rhs;
			__asm__ volatile (
				"inc %[rhs] \n"
				"dec %[size] \n"
				: asm_arg("+R", rhs), asm_arg("+R", size), asm_flags(nz, not_zero)
			);
			//--size;
		} while (expect(not_zero, true));
		return true;
	}
	return false;
}

dllexport gnu_noinline gnu_attr(target("no-sse3")) sysv_abi HMODULE get_module_handle_w64(
	uint64_t name_low,
	uint64_t name_high
) {
	//const auto *restrict name = unpack_hilo<const uint8_t *restrict>(name_low, name_high);
	const auto *restrict name = (const uint8_t *restrict)(name_high << 32 | (uint32_t)name_low);
	if (expect(name != NULL, true)) {
		const LIST_ENTRY_NEW<LDR_DATA_TABLE_ENTRY_NEW> *restrict module_list_head = &((const PEB_LDR_DATA_NEW *restrict)((TEB GS_RELATIVE*)0)->ProcessEnvironmentBlock->Ldr)->InLoadOrderModuleList;
		const LDR_DATA_TABLE_ENTRY_NEW *restrict node = module_list_head->Flink;
		//__asm__ volatile ("":"=R"(module_list_head));
		assume(&node->InLoadOrderLinks != module_list_head);
		for (
			;
			expect(&node->InLoadOrderLinks != module_list_head, true);
			node = node->InLoadOrderLinks.Flink
		) {
#if USE_JANK_MODULE_LOOP == 1
			size_t buffer_size = node->BaseDllName.Length + (uintptr_t)name;
			const auto *restrict buffer_ptr = based_pointer<const uint8_t>(node->BaseDllName.Buffer, -(uintptr_t)name);
			const auto *restrict rhs = name;
			while ((size_t)rhs != buffer_size) {
				if (*rhs++ != *buffer_ptr++) goto continue_outer_loop;
			}
#elif USE_JANK_MODULE_LOOP == 2
			uint32_t buffer_size = node->BaseDllName.Length;
			if (expect(byteloop_memcmp2((const uint8_t *restrict)node->BaseDllName.Buffer, name, buffer_size), true)) {
#else
			const auto *restrict buffer_ptr = (const uint8_t *restrict)node->BaseDllName.Buffer;
			for (size_t i = 0; i < node->BaseDllName.Length; ++i) {
				if (name[i] != buffer_ptr[i]) goto continue_outer_loop;
			}
#endif
				void* ret = node->DllBase;
				__asm__ volatile (
					""
					:
					: "d"((uintptr_t)ret >> 32), "Yz"(ret)
				);
				return (HMODULE)ret;
#if USE_JANK_MODULE_LOOP != 2
		continue_outer_loop:
#else
			}
#endif
		}
	}
	__asm__ volatile (
		""
		:
		: "d"(NULL), "Yz"(NULL)
	);
	return NULL;
}

dllexport gnu_noinline void* regcall memset64Adapter(
	uint64_t val,		// EAX
	uint64_t size_low,	// ECX
	uint64_t dst_high,	// EDX
	uint64_t dst_low,	// EDI
	uint64_t size_high	// ESI
) {
	void *const dst_in = unpack_hilo<void *const>(dst_low, dst_high);
	void* dst = dst_in;
	size_t size = unpack_hilo<size_t>(size_low, size_high);
	__asm__ volatile (
		"rep stosb"
		: "=c"(size), "+D"(dst)
		: "0"(size), "a"(val)
		: "memory"
	);
	__asm__ volatile (
		""
		:
		: "d"((uintptr_t)dst_in >> 32), "Yz"(dst_in)
	);
	return dst_in;
}

dllexport gnu_noinline void* regcall memcpy64Adapter(
	uint64_t dst_low,	// EAX
	uint64_t size_low,	// ECX
	uint64_t dst_high,	// EDX
	uint64_t src_high,	// EDI
	uint64_t src_low,	// ESI
	uint64_t size_high	// EBX
) {
	__asm__ volatile (
		""
		: "=b"(size_high)
	);
	void *const dst_in = unpack_hilo<void *const>(dst_low, dst_high);
	void *src = unpack_hilo<void *const>(src_low, src_high);
	void* dst = dst_in;
	size_t size = unpack_hilo<size_t>(size_low, size_high);
	__asm__ volatile (
		"rep movsb"
		: "=c"(size), "+D"(dst), "+S"(src)
		: "0"(size)
		: "memory"
	);
	__asm__ volatile (
		""
		:
		: "d"((uintptr_t)dst_in >> 32), "Yz"(dst_in)
	);
	return dst_in;
}

dllexport gnu_noinline int regcall memcmp64Adapter(
	uint64_t rhs_high,	// EAX
	uint64_t size_low,	// ECX
	uint64_t lhs_high,	// EDX
	uint64_t rhs_low,	// EDI
	uint64_t lhs_low,	// ESI
	uint64_t size_high	// EBX
) {
	__asm__ volatile (
		""
		: "=b"(size_high)
	);
	void* lhs = unpack_hilo<void*>(lhs_low, lhs_high);
	void* rhs = unpack_hilo<void*>(rhs_low, rhs_high);
	size_t size = unpack_hilo<size_t>(size_low, size_high);
	int ret;
	__asm__ volatile (
		"repe cmpsb \n"
		"sbbl %[ret], %[ret] \n"
		"sbbl $1, %[ret] \n"
		: "=c"(size), asm_arg("=a", ret), "+D"(rhs), "+S"(lhs)
		: "0"(size)
	);
	return size ? ret : (uint32_t)size;
}


dllexport gnu_noinline void regcall custom_chkstk3(
	size_t allocation_size,
	size_t rcx_dummy,
	size_t rdx_dummy,
	size_t rdi_dummy,
	size_t rsi_dummy,
	size_t r8_dummy_in,
	size_t r9_dummy_in,
	size_t r10_dummy_in,
	size_t r11_dummy_in
) {
	register size_t r8_dummy asm("r8") = r8_dummy_in;
	register size_t r9_dummy asm("r9") = r9_dummy_in;
	register size_t r10_dummy asm("r10") = r10_dummy_in;
	__asm__ volatile (""::"c"(rcx_dummy), "d"(rdx_dummy), "D"(rdi_dummy), "S"(rsi_dummy), "r"(r8_dummy), "r"(r9_dummy), "r"(r10_dummy));


	uintptr_t temp_stack_pointer;
	register uint64_t new_stack_pointer asm("r11");
	__asm__ volatile (
		"movl %%esp, %k[new_stack_pointer] \n"
		"subl %k[allocation_size], %k[new_stack_pointer] \n"
		"leal %c[negative_page_size](%q[new_stack_pointer]), %k[temp_stack_pointer] \n"
		: asm_arg("=a", temp_stack_pointer), asm_arg("=r", new_stack_pointer)
		: [negative_page_size] "i"(-page_size), asm_arg("a", allocation_size)
	);
	for (
		;
		temp_stack_pointer > new_stack_pointer;
		temp_stack_pointer -= page_size
	) {
		__asm__ volatile (
			"testb $0, (%q[temp_stack_pointer])"
			:
			: asm_arg("a", temp_stack_pointer)
		);
	}
	__asm__ volatile (
		"testb $0, (%q[new_stack_pointer])"
		:
		: asm_arg("r", new_stack_pointer)
	);
	esp_reg = (uint32_t)new_stack_pointer;
	__asm__ volatile (""::"c"(rcx_dummy), "d"(rdx_dummy), "D"(rdi_dummy), "S"(rsi_dummy), "r"(r8_dummy), "r"(r9_dummy), "r"(r10_dummy));
	return;
}

extern "C" {
	//dllexport forceinline void regparm(1) custom_chkstk() asm("__chkstk");
	//dllexport forceinline void regparm(1) custom_chkstk() {

	//}
}

dllexport gnu_noinline void test_msr_dummy(uint32_t index) {
	union {
		uint64_t raw;
		struct {
			uint32_t low_half;
			uint32_t high_half;
		};
	} dummy;
	__asm__ volatile (
		"rdmsr"
		: "=a"(dummy.low_half), "=d"(dummy.high_half)
		: "c"(index)
	);
	dummy.raw += 1;
	__asm__ volatile (
		"wrmsr"
		:
		: "a"(dummy.low_half), "d"(dummy.high_half), "c"(index)
	);
}

dllexport gnu_noinline void test_msr_dummy2(uint32_t index) {
	uint32_t low_half;
	uint32_t high_half;
	__asm__ volatile (
		"rdmsr"
		: "=a"(low_half), "=d"(high_half)
		: "c"(index)
	);
	//assume(low_half <= UINT32_MAX && high_half <= UINT32_MAX);
	union {
		uint64_t raw;
		struct {
			uint32_t low_half;
			uint32_t high_half;
		};
	} dummy;
	dummy.low_half = low_half;
	dummy.high_half = high_half;

	dummy.raw += 1;
	__asm__ volatile (
		"wrmsr"
		:
		: "a"(dummy.low_half), "d"(dummy.high_half), "c"(index)
	);
}

static constexpr auto kjbwekbjtg = __builtin_offsetof(IMAGE_DOS_HEADER, e_lfanew);

#endif

dllexport gnu_noinline vec<uint32_t, 2> fastcall vec_arg_test(uint32_t low, uint32_t high) {
	return { low, high };
}

dllexport gnu_noinline uint64_t vec_ret_test(vec<uint32_t, 2> value) {
	return (uint64_t)((uint64_t)value[1] << 32 | value[0]);
}

/*
#define testcc gnu_attr(pascal)

dllexport gnu_noinline float testcc floats_test(float argA, float argB, float argC, float argD, float argE, float argF, float argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}

dllexport gnu_noinline gnu_attr(target("no-sse")) float testcc floats_test_no_sse(float argA, float argB, float argC, float argD, float argE, float argF, float argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}

dllexport gnu_noinline long double testcc long_doubles_test(long double argA, long double argB, long double argC, long double argD, long double argE, long double argF, long double argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}
dllexport gnu_noinline gnu_attr(target("no-sse")) long double testcc long_doubles_test_no_sse(long double argA, long double argB, long double argC, long double argD, long double argE, long double argF, long double argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}
dllexport gnu_noinline uint128_t testcc jumbo_int_test(uint128_t argA, uint128_t argB, uint32_t argC, uint32_t argD, uint32_t argE, uint32_t argF, uint32_t argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}
dllexport gnu_noinline gnu_attr(target("no-sse")) uint128_t testcc jumbo_int_test_no_sse(uint128_t argA, uint128_t argB, uint32_t argC, uint32_t argD, uint32_t argE, uint32_t argF, uint32_t argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}
dllexport gnu_noinline vec<uint64_t, 2> testcc vecs_test(vec<uint64_t, 2> argA, vec<uint64_t, 2> argB, vec<uint64_t, 2> argC, vec<uint64_t, 2> argD, vec<uint64_t, 2> argE, vec<uint64_t, 2> argF, vec<uint64_t, 2> argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}
dllexport gnu_noinline gnu_attr(target("no-sse")) vec<uint64_t, 2> testcc vecs_test_no_sse(vec<uint64_t, 2> argA, vec<uint64_t, 2> argB, vec<uint64_t, 2> argC, vec<uint64_t, 2> argD, vec<uint64_t, 2> argE, vec<uint64_t, 2> argF, vec<uint64_t, 2> argG) {
	return argA + argB + argC + argD + argE + argF + argG;
}
dllexport gnu_noinline gnu_attr(target("mmx")) __m64 testcc m64_test(__m64 argA, __m64 argB) {
	return argA ^ argB;
}
dllexport gnu_noinline gnu_attr(target("mmx")) __m64 testcc mmx_test(__m64 argA, __m64 argB) {
	return _mm_add_pi32(argA, argB);
}

dllexport gnu_noinline vec<float,2> testcc float2_vecs_test(float argA, float argB) {
	return { argA , argB };
}

dllexport gnu_noinline gnu_attr(target("no-sse")) vec<float, 2> testcc float2_vecs_test_no_sse(float argA, float argB) {
	return { argA , argB };
}

dllexport gnu_noinline vec<float, 4> testcc float4_vecs_test(float argA, float argB, float argC, float argD) {
	return { argA , argB, argC, argD };
}

dllexport gnu_noinline gnu_attr(target("no-sse")) vec<float, 4> testcc float4_vecs_test_no_sse(float argA, float argB, float argC, float argD) {
	return { argA , argB, argC, argD };
}


dllexport gnu_noinline vec<uint32_t, 2> testcc int2_vecs_test(uint32_t argA, uint32_t argB) {
	return { argA , argB };
}

dllexport gnu_noinline gnu_attr(target("no-sse")) vec<uint32_t, 2> testcc int2_vecs_test_no_sse(uint32_t argA, uint32_t argB) {
	return { argA , argB };
}

dllexport gnu_noinline vec<uint32_t, 4> testcc int4_vecs_test(uint32_t argA, uint32_t argB, uint32_t argC, uint32_t argD) {
	return { argA , argB, argC, argD };
}
dllexport gnu_noinline gnu_attr(target("no-sse")) vec<uint32_t, 4> testcc int4_vecs_test_no_sse(uint32_t argA, uint32_t argB, uint32_t argC, uint32_t argD) {
	return { argA , argB, argC, argD };
}
dllexport gnu_noinline vec<uint64_t, 2> testcc longlong2_vecs_test(uint64_t argA, uint64_t argB) {
	return { argA , argB };
}
dllexport gnu_noinline gnu_attr(target("no-sse")) vec<uint64_t, 2> testcc longlong2_vecs_test_no_sse(uint64_t argA, uint64_t argB) {
	return { argA , argB };
}

dllexport gnu_noinline _Complex long double testcc complex_long_doubles_test(_Complex long double argA, _Complex long double argB) {
	return argA + argB;
}
*/

/*
#include "zero/func_traits.h"

void stdcall yeetus_tester() {

}

using yeetus_traits = FunctionTraits<yeetus_tester>;

using wkrhwkrv = CallingConventionTemplates::STDCALL_PTR;

static constexpr bool pingy = __is_same(void(cdecl*)(), void(stdcall*)());
static_assert(!pingy);

using yeetus_tester_ret_type = yeetus_traits::ret_type;
using yeetus_tester_arg_types = yeetus_traits::arg_types;
static constexpr size_t yeetus_tester_arg_count = yeetus_traits::arg_count;
//using yeetus_tester_pointer = yeetus_traits::pointer;
static constexpr auto yeetus_tester_convention = yeetus_traits::calling_convention;
static_assert(yeetus_tester_convention == CC_STDCALL);
static constexpr bool yeetus_tester_noexcept = yeetus_traits::is_noexcept;
static constexpr bool yeetus_tester_variadic = yeetus_traits::is_variadic;
*/



dllexport gnu_noinline gnu_attr(target("no-sse")) uint32_t test_sib_jank() {
	uint32_t matches = 0;
	uintptr_t esp_compare = esp_reg;
	uintptr_t esp_val;
	__asm__ volatile (".byte 0x8D, 0x14, 0x24 \n" : asm_arg("=d", esp_val));
	matches |= (esp_val == esp_compare);
	__asm__ volatile (".byte 0x8D, 0x14, 0x64 \n" : asm_arg("=d", esp_val));
	matches |= (esp_val == esp_compare) << 1;
	__asm__ volatile (".byte 0x8D, 0x14, 0xA4 \n" : asm_arg("=d", esp_val));
	matches |= (esp_val == esp_compare) << 2;
	__asm__ volatile (".byte 0x8D, 0x14, 0xE4 \n" : asm_arg("=d", esp_val));
	matches |= (esp_val == esp_compare) << 3;
	__asm__ volatile (".byte 0x8D, 0x54, 0x24 \n .byte %c[offset] \n" : asm_arg("=d", esp_val) : [offset]"i"(69));
	matches |= (esp_val == esp_compare + 69) << 4;
	__asm__ volatile (".byte 0x8D, 0x54, 0x64 \n .byte %c[offset] \n" : asm_arg("=d", esp_val) : [offset]"i"(69));
	matches |= (esp_val == esp_compare + 69) << 5;
	__asm__ volatile (".byte 0x8D, 0x54, 0xA4 \n .byte %c[offset] \n" : asm_arg("=d", esp_val) : [offset]"i"(69));
	matches |= (esp_val == esp_compare + 69) << 6;
	__asm__ volatile (".byte 0x8D, 0x54, 0xE4 \n .byte %c[offset] \n" : asm_arg("=d", esp_val) : [offset]"i"(69));
	matches |= (esp_val == esp_compare + 69) << 7;
	__asm__ volatile (".byte 0x8D, 0x94, 0x24 \n .long %c[offset] \n" : asm_arg("=d", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 8;
	__asm__ volatile (".byte 0x8D, 0x94, 0x64 \n .long %c[offset] \n" : asm_arg("=d", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 9;
	__asm__ volatile (".byte 0x8D, 0x94, 0xA4 \n .long %c[offset] \n" : asm_arg("=d", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 10;
	__asm__ volatile (".byte 0x8D, 0x94, 0xE4 \n .long %c[offset] \n" : asm_arg("=d", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 11;
	return matches;
}

dllexport gnu_noinline gnu_attr(target("no-sse")) int log_mboxf(const char *caption, const UINT type, const char *format, ...) {
	use_var(caption);
	use_var(type);
	va_list va;
	va_start(va, format);
	vprintf(format, va);
	va_end(va);
	return rand();
}

dllexport gnu_noinline gnu_attr(target("no-sse")) void test_sib_jank2() {
	uint32_t matches = 0;
	uintptr_t esp_compare = esp_reg;
	__asm__ volatile (""::"r"(esp_compare));
	uintptr_t esp_val;
	__asm__ volatile ("lea (%%esp), %[esp_val] \n" : asm_arg("=r", esp_val));
	matches |= (esp_val == esp_compare);
	__asm__ volatile ("lea (%%esp), %[esp_val] \n" : asm_arg("=r", esp_val));
	matches |= (esp_val == esp_compare) << 1;
	__asm__ volatile ("lea (%%esp), %[esp_val] \n" : asm_arg("=r", esp_val));
	matches |= (esp_val == esp_compare) << 2;
	__asm__ volatile ("lea (%%esp), %[esp_val] \n" : asm_arg("=r", esp_val));
	matches |= (esp_val == esp_compare) << 3;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0x69));
	matches |= (esp_val == esp_compare + 0x69) << 4;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0x69));
	matches |= (esp_val == esp_compare + 0x69) << 5;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0x69));
	matches |= (esp_val == esp_compare + 0x69) << 6;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0x69));
	matches |= (esp_val == esp_compare + 0x69) << 7;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 8;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 9;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 10;
	__asm__ volatile ("lea %c[offset](%%esp), %[esp_val] \n" : asm_arg("=r", esp_val) : [offset] "i"(0xDEADBEEF));
	matches |= (esp_val == esp_compare + 0xDEADBEEF) << 11;
	log_mboxf("ESP SIB Test", 0x40, matches == 0xFFF ? "No ESP SIB errors" : "ESP SIB errors! %X", matches);
}

dllexport gnu_noinline void stdcall ret_jump(uintptr_t addr) {
	*(uintptr_t*)_AddressOfReturnAddress() = addr;
}


dllexport gnu_noinline void regcall custom_chkstk(size_t allocation_size) {
	if (expect(allocation_size <= page_size, true)) {
		__asm__ volatile (
			"sub %[allocation_size], " RSP_REG "\n"
			:
			: asm_arg("a", allocation_size)
		);
		*(uintptr_t*)_AddressOfReturnAddress() = *(uintptr_t*)(rsp_reg + allocation_size);
		return;
	}
	/*
	uintptr_t new_stack_pointer = rsp_reg + sizeof(void*);
	__asm__ volatile (
		""
		: "=b"(new_stack_pointer)
		: "0"(rsp_reg + sizeof(void*))
	);
	do {
		new_stack_pointer -= page_size;
		*(volatile uint8_t*)new_stack_pointer & 0;
	} while ((allocation_size -= page_size) > page_size);
	//} while (expect_chance((allocation_size -= page_size) > page_size, true, 0.125));
	new_stack_pointer -= allocation_size;
	*/

	//uintptr_t ebx_var = rsp_reg + sizeof(void*);
	uintptr_t ecx_var;
	__asm__ volatile (
		"push %[ecx_var] \n"
		"lea %c[push_offset](" RSP_REG "), %[ecx_var]"
		: asm_arg("=c", ecx_var)
		: [push_offset]"i"(sizeof(void*))
	);
	ecx_var -= allocation_size;
	do {
		allocation_size -= page_size;
		*(volatile uint8_t*)(ecx_var + allocation_size) = 0;
	} while (allocation_size > page_size);
	//rsp_reg = ecx_var;
	//ecx_var = *(uintptr_t*)allocation_size;
	__asm__ volatile (
		"mov %[ecx_var], %[allocation_size] \n"
		"pop %[ecx_var] \n"
		"mov %[allocation_size], " RSP_REG " \n"
		: asm_arg("=a", allocation_size), asm_arg("+c", ecx_var)
	);
	*(uintptr_t*)_AddressOfReturnAddress() = *(uintptr_t*)allocation_size;
	return;
}


dllexport gnu_noinline void regcall custom_chkstk2(size_t allocation_size) {
	if (expect(allocation_size <= page_size, true)) {
		__asm__ volatile (
			"testb $0, %c[page_size](" RSP_REG ") \n"
			"sub %[allocation_size], " RSP_REG " \n"
			"mov (" RSP_REG ",%[allocation_size]), %[allocation_size] \n"
			: asm_arg("+a", allocation_size)
			: [page_size]"i"(-page_size)
		);
		*(uintptr_t*)_AddressOfReturnAddress() = (uintptr_t)allocation_size;
		return;
	}
	uintptr_t new_stack_pointer;
	__asm__ volatile (
		//"push %[new_stack_pointer] \n"
		".byte 0xFF, 0xF1 \n"
		"lea %c[push_offset](" RSP_REG "), %[new_stack_pointer] \n"
		"sub %[allocation_size], %[new_stack_pointer] \n"
		: asm_arg("=c", new_stack_pointer)
		: [push_offset]"i"(sizeof(void*)), asm_arg("a", allocation_size)
	);
	/*
	bool carry;
	do {
		__asm__ volatile (
			"testb $0, -%c[page_size](%[allocation_size],%[new_stack_pointer]) \n"
			"sub %[page_size], %[allocation_size] \n"
			: asm_arg("+a", allocation_size), asm_flags(c, carry)
			: asm_arg("i", page_size), asm_arg("c", new_stack_pointer)
		);
	} while (!carry);
	*/
	do {
		__asm__ volatile (
			"testb $0, %c[page_size](%[allocation_size],%[new_stack_pointer]) \n"
			"add %[page_size], %[allocation_size] \n"
			: asm_arg("+a", allocation_size)
			: [page_size]"i"(-page_size), asm_arg("c", new_stack_pointer)
		);
	} while (allocation_size > page_size);
	__asm__ volatile (
		"testb $0, (%[new_stack_pointer]) \n"
		"mov " RSP_REG ", %[allocation_size] \n"
		"mov %[new_stack_pointer], " RSP_REG " \n"
		"mov (%[allocation_size]), %[new_stack_pointer] \n"
		"mov %c[push_size](%[allocation_size]), %[allocation_size] \n"
		: asm_arg("+c", new_stack_pointer), asm_arg("+a", allocation_size)
		: [push_size]"i"(sizeof(void*))
	);
	*(uintptr_t*)_AddressOfReturnAddress() = (uintptr_t)allocation_size;
	return;
}

dllexport gnu_noinline bool fastcall is_valid_decimal_char(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case '0' ... '9':
			return true;
		default:
			return false;
	}
}
dllexport gnu_noinline bool fastcall is_valid_hex_letter_char(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case 'a' ... 'f': case 'A' ... 'F':
			return true;
		default:
			return false;
	}
}
dllexport gnu_noinline bool fastcall is_valid_hex_letter_char2(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case 'a' ... 'f':
			return true;
		default:
			return false;
	}
}
dllexport gnu_noinline bool fastcall is_valid_hex_char(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case '0' ... '9': case 'a' ... 'f': case 'A' ... 'F':
			return true;
		default:
			return false;
	}
}
dllexport gnu_noinline bool fastcall is_valid_hex_char2(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case '0' ... '9': case 'a' ... 'f':
			return true;
		default:
			return false;
	}
}

dllexport gnu_noinline uint8_t fastcall valid_decimal_char_value(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		default: unreachable;
	}
}
dllexport gnu_noinline uint8_t fastcall valid_hex_letter_char_value(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case 'a': case 'A': return 10;
		case 'b': case 'B': return 11;
		case 'c': case 'C': return 12;
		case 'd': case 'D': return 13;
		case 'e': case 'E': return 14;
		case 'f': case 'F': return 15;
		default: unreachable;
	}
}
dllexport gnu_noinline uint8_t fastcall valid_hex_letter_char_value2(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		default: unreachable;
	}
}
dllexport gnu_noinline uint8_t fastcall valid_hex_char_value(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': case 'A': return 10;
		case 'b': case 'B': return 11;
		case 'c': case 'C': return 12;
		case 'd': case 'D': return 13;
		case 'e': case 'E': return 14;
		case 'f': case 'F': return 15;
		default: unreachable;
	}
}
dllexport gnu_noinline uint8_t fastcall valid_hex_char_value2(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		default: unreachable;
	}
}

static forceinline uint8_t valid_hex_char_value3(uint32_t c) {
	assume(c <= CHAR_MAX);
	switch (c) {
		case '0' ... '9':
			return c - '0';
		case 'a' ... 'f':
			return c - 'a';
		case 'A' ... 'F':
			return c - 'A';
		default:
			unreachable;
	}
}
static forceinline uint8_t valid_hex_char_value4(uint32_t c) {
	c = lowercase(c);
	assume(c >= lowercase_mask && c <= CHAR_MAX);
	switch (c) {
		case '0' ... '9':
			return c - '0';
		case 'a' ... 'f':
			return c - 'a';
		default:
			unreachable;
	}
}
static forceinline uint8_t valid_hex_char_value5(uint32_t c) {
	assume(c >= lowercase_mask && c <= CHAR_MAX);
	switch (c) {
		case '0' ... '9':
			return c - '0';
		case 'a' ... 'f':
			return c - 'a';
		default:
			unreachable;
	}
}


dllexport gnu_noinline auto fastcall strtouz(const char* str/*, uintptr_t rel_source*/) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint8_t number_base = 10;
	union {
		uint8_t raw;
		struct {
			bool negate : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool is_max : 1;
		};
	} flags = { 0 };
	//bool negate = false;
	for (;;) {
		switch (uint8_t c = *str_read++) {
			default:
				return std::pair((size_t)0, str);
			case '-':
				if (flags.negate) {
					return std::pair((size_t)0, str);
				}
				flags.negate = true;
				continue;
			case ' ': case '\t': case '\n': case '\r': case '\f': case '\v':
				if (flags.negate) {
					return std::pair((size_t)0, str);
				}
				continue;
			case '0':
				//rel_source = 0;
			case 'r': case 'R': relative_value:
				c = '0';
				switch (lowercase(*str_read)) {
					case 'x': /*case 'X':*/
						number_base = 16;
						++str_read;
						break;
					case 'b': /*case 'B':*/
						number_base = 2;
						++str_read;
						break;
				}
			case '1' ... '9':
				//bool is_max = false;
				size_t ret = 0;
				for (;;) {
					uint8_t digit;
					switch (c) {
						case '0' ... '9':
							digit = c - '0';
							break;
						case 'a' ... 'f':
							digit = c - 'a';
							break;
					}
					if (digit >= number_base) break;
					c = lowercase(*str_read++);
					if (flags.is_max) continue;
					// The short circuiting of || skipping the addition
					// doesn't matter of the multiplication overflows;
					//is_max = __builtin_mul_overflow(ret, number_base, &ret) || __builtin_add_overflow(ret, digit, &ret);
					if (
						__builtin_mul_overflow(ret, number_base, &ret) ||
						__builtin_add_overflow(ret, digit, &ret)
					) {
						flags.is_max = true;
						ret = SIZE_MAX;
					}
				}
				if (flags.negate) {
					ret = -ret;
				}
				return std::pair(ret, (const char*)str_read);
		}
	}
}

dllexport gnu_noinline auto fastcall strtouz2(const char* str/*, uintptr_t rel_source*/) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint32_t number_base = 10;
	struct {
		bool negate;
		bool is_max;
	} flags;
	flags.negate = false;
	for (;;) {
		switch (uint8_t c = *str_read++) {
			case '-':
				if (flags.negate) {
			default:
					return std::pair((size_t)0, str);
				}
				flags.negate = true;
				continue;
			case ' ': case '\t': case '\n': case '\r': case '\f': case '\v':
				if (flags.negate) {
					return std::pair((size_t)0, str);
				}
				continue;
			case '0':
				//rel_source = 0;
			case 'r': case 'R':
				c = '0';
				switch (lowercase(*str_read)) {
					case 'x': /*case 'X':*/
						number_base = 16;
						++str_read;
						break;
					case 'b': /*case 'B':*/
						number_base = 2;
						++str_read;
						break;
				}
			case '1' ... '9':
				//bool is_max = false;
				size_t ret = 0;
				for (flags.is_max = false;;) {
					uint32_t digit;
					switch (c) {
						case '0' ... '9':
							digit = c - '0';
							break;
						case 'a' ... 'f':
							digit = c - 'a';
							break;
						default:
							goto end_parse;
					}
					if (digit >= number_base) break;
					c = lowercase(*str_read++);
					if (flags.is_max) continue;
					// The short circuiting of || skipping the addition
					// doesn't matter of the multiplication overflows;
					if (
						__builtin_mul_overflow(ret, number_base, &ret) ||
						__builtin_add_overflow(ret, digit, &ret)
					) {
						flags.is_max = true;
						ret = SIZE_MAX;
					}
				}
			end_parse:
				if (flags.negate) {
					ret = -ret;
				}
				return std::pair(ret, (const char*)str_read);
		}
	}
}

dllexport gnu_noinline auto fastcall strtouz3(const char* str) {
	const uint8_t* str_read = (const uint8_t*)str;
	union {
		uint32_t w;
		struct {
			uint8_t c;
			int8_t flags;
		};
	} ebx;
	uint32_t number_base;
	for (;;) {
		//ebx.w = *str_read;
		__asm__ volatile (
			""
			: "=b"(ebx.w)
			: "0"((uint32_t)*str_read)
		);
		++str_read;
		switch (ebx.w) {
			default: error_ret:
				return std::pair((size_t)0, str);
			case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
				continue;
			case '-': {
				ebx.w = *str_read;
				++str_read;
				uint32_t temp = ebx.w - '1';
				__asm__ volatile ("":"+b"(ebx));
				ebx.w |= 0x0120;
				if (temp < 9) goto decimal_prefix;
				switch (ebx.c) {
					default:
						goto error_ret;
					case '0':
						goto zero_prefix_case;
					case 'r':
				}
			}
			case 'r': case 'R':
				ebx.c = '0';
				assume(ebx.flags != 0);
			case '0': zero_prefix_case:
				switch (*str_read) {
					case 'b':
						number_base = 2;
						++str_read;
						goto post_prefix;
					case 'x':
						number_base = 16;
						++str_read;
						goto post_prefix;
				}
			case '1' ... '9': decimal_prefix:
				number_base = 10;
			post_prefix:
				size_t ret = 0;
				for (;;) {
					uint32_t digit = ebx.c;
					switch (digit) {
						case '0' ... '9':
							digit -= '0';
							break;
						case 'a' ... 'f':
							digit -= 'a';
							break;
						default:
							goto end_parse;
					}
					if (digit >= number_base) break;
					ebx.c = lowercase(*str_read);
					++str_read;
					if (ebx.flags < 0) continue;
					if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
						if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
							continue;
						}
					}
					//ebx.w |= 0x8000;
					__asm__ volatile (
						".byte 0x80, 0xCF, 0x80"
					);
					//__asm__ volatile (
						//"orb $0x80, %h[flags]"
						//: [flags] "+Q"(ebx)
					//);
					ret = SIZE_MAX;
				}
			end_parse:
				if (ebx.flags & 1) {
					ret = -ret;
				}
				return std::pair(ret, (const char*)str_read);
		}
	}
}

dllexport gnu_noinline auto fastcall strtouz4(const char* str) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint32_t number_base;
	//++str_read;
	//for (;;) {
		//uint32_t ebx = *str_read;
	outer_loop:
		union {
			uint32_t w;
			_Atomic uint32_t aw;
			struct {
				uint8_t c;
				int8_t flags;
			};
			struct {
				_Atomic uint8_t ac;
				_Atomic int8_t aflags;
			};
		} ebx;
		ebx.w = *str_read++;
		switch (ebx.w) {
			default: error_ret:
				return std::pair((size_t)0, str);
			case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
				//continue;
				goto outer_loop;
			case '-': {
				ebx.w = *str_read++;
				uint32_t temp = ebx.w - '1';
				ebx.w |= 0x0120;
				if (temp < 9) goto decimal_prefix;
				switch ((uint8_t)ebx.w) {
					default:
						goto error_ret;
					case '0':
						goto zero_prefix_case;
					case 'r':
				}
			}
			case 'r': case 'R':
				//ebx.c = '0';
				ebx.w &= ~UINT8_MAX;
				ebx.w |= '0';
			case '0': zero_prefix_case:
				switch (*str_read) {
					case 'b':
						number_base = 2;
						++str_read;
						goto post_prefix;
					case 'x':
						number_base = 16;
						++str_read;
						goto post_prefix;
				}
			case '1' ... '9': decimal_prefix:
				number_base = 10;
			post_prefix:
				size_t ret = 0;
				for (;;) {
					uint32_t digit = (uint8_t)ebx.w;
					switch (digit) {
						case '0' ... '9':
							digit -= '0';
							break;
						case 'a' ... 'f':
							digit -= 'a';
							break;
						default:
							goto end_parse;
					}
					if (digit >= number_base) break;
					//ebx.ac = lowercase(*str_read);
					//ebx.w &= ~UINT8_MAX;
					//ebx.w |= lowercase(*str_read);
					//ebx.w = (ebx.w & ~UINT8_MAX) | lowercase(*str_read);
					__asm__ volatile (
						"movb (%[str_read]), %b[c]"
						: [c]"=q"(ebx.w)
						: asm_arg("r", str_read)
					);
					++str_read;
					if (!(ebx.w & 0x8000)) {
					//if (ebx.flags < 0) {
						if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
							if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
								continue;
							}
						}
						ebx.w |= 0x8000;
						//ebx.flags |= 0x80;
						ret = SIZE_MAX;
					}
				}
			end_parse:
				if (ebx.w & 0x0100) {
					ret = -ret;
				}
				return std::pair(ret, (const char*)str_read);
		}
	//}
}

dllexport gnu_noinline auto fastcall strtouz5(const char* str) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint32_t number_base;
	//++str_read;
	//for (;;) {
		//uint32_t ebx = *str_read;
	outer_loop:
		union {
			uint32_t w;
			_Atomic uint32_t aw;
			struct {
				uint8_t c;
				int8_t flags;
			};
			struct {
				_Atomic uint8_t ac;
				_Atomic int8_t aflags;
			};
		} ebx;
		//ebx.w = *str_read++;
		__asm__ volatile (
			"movzbl (%[str_read]), %[ebx]"
			: [ebx]"=Q"(ebx.w)
			: asm_arg("r", str_read)
		);
		++str_read;
		switch (ebx.w) {
			default: error_ret:
				return std::pair((size_t)0, str);
			case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
				//continue;
				goto outer_loop;
			case '-': {
				ebx.w = *str_read++;
				//uint32_t temp = ebx.w - '1';
				ebx.w |= 0x0120;
				//if (temp < 9) goto decimal_prefix;
				switch ((uint8_t)ebx.w) {
					case '1' ... '9':
						goto decimal_prefix;
					default:
						goto error_ret;
					case '0':
						goto zero_prefix_case;
					case 'r':
				}
			}
			case 'r': case 'R':
				ebx.c = '0';
				//ebx.w &= ~UINT8_MAX;
				//ebx.w |= '0';
			case '0': zero_prefix_case:
				switch (*str_read) {
					case 'b':
						number_base = 2;
						++str_read;
						goto post_prefix;
					case 'x':
						number_base = 16;
						++str_read;
						goto post_prefix;
				}
			case '1' ... '9': decimal_prefix:
				number_base = 10;
			post_prefix:
				size_t ret = 0;
				for (;;) {
					uint32_t digit = (uint8_t)ebx.w;
					switch (digit) {
						case '0' ... '9':
							digit -= '0';
							break;
						case 'a' ... 'f':
							digit -= 'a';
							break;
						default:
							goto end_parse;
					}
					if (digit >= number_base) break;
					//ebx.ac = lowercase(*str_read);
					//ebx.w &= ~UINT8_MAX;
					//ebx.w |= lowercase(*str_read);
					//ebx.w = (ebx.w & ~UINT8_MAX) | lowercase(*str_read);
					__asm__ volatile (
						"movb (%[str_read]), %b[c]"
						: [c]"=Q"(ebx.w)
						: asm_arg("r", str_read)
					);
					++str_read;
					if (!(ebx.w & 0x8000)) {
					//if (ebx.flags < 0) {
						if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
							if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
								continue;
							}
						}
						//ebx.w |= 0x8000;
						ebx.flags |= 0x80;
						ret = SIZE_MAX;
					}
				}
			end_parse:
				if (ebx.w & 0x0100) {
					ret = -ret;
				}
				return std::pair(ret, (const char*)str_read);
		}
	//}
}

dllexport gnu_noinline auto fastcall strtouz6(const char* str) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint32_t number_base;
outer_loop:
	union {
		uint32_t w;
		_Atomic uint32_t aw;
		struct {
			uint8_t c;
			int8_t flags;
		};
		struct {
			_Atomic uint8_t ac;
			_Atomic int8_t aflags;
		};
	} ebx;
	ebx.w = *str_read++;
	switch (ebx.w) {
		default: error_ret:
			return std::pair((size_t)0, str);
		case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
			//continue;
			goto outer_loop;
		case '-': {
			ebx.w = *str_read++;
			//uint32_t temp = ebx.w - '1';
			ebx.w |= 0x0120;
			//if (temp < 9) goto decimal_prefix;
			switch ((uint8_t)ebx.w) {
				case '1' ... '9':
					goto decimal_prefix;
				default:
					goto error_ret;
				case '0':
					goto zero_prefix_case;
				case 'r':
			}
		}
		case 'r': case 'R':
			//ebx.c = '0';
			//ebx.w &= ~UINT8_MAX;
			//ebx.w |= '0';
			__asm__ volatile (
				"movb %[value], %b[c]"
				: [c]"=q"(ebx.w)
				: [value]"i"('0')
			);
		case '0': zero_prefix_case:
			switch (*str_read) {
				case 'b':
					number_base = 2;
					++str_read;
					goto post_prefix;
				case 'x':
					number_base = 16;
					++str_read;
					goto post_prefix;
			}
		case '1' ... '9': decimal_prefix:
			number_base = 10;
		post_prefix:
			size_t ret = 0;
			for (;;) {
				uint32_t digit = (uint8_t)ebx.w;
				switch (digit) {
					case '0' ... '9':
						digit -= '0';
						break;
					case 'a' ... 'f':
						digit -= 'a';
						break;
					default:
						goto end_parse;
				}
				if (digit >= number_base) break;
				//ebx.ac = lowercase(*str_read);
				//ebx.w &= ~UINT8_MAX;
				//ebx.w |= lowercase(*str_read);
				//ebx.w = (ebx.w & ~UINT8_MAX) | lowercase(*str_read);
				__asm__ volatile (
					"movb (%[str_read]), %b[c]"
					: [c]"=q"(ebx.w)
					: asm_arg("r", str_read)
				);
				++str_read;
				if (!(ebx.w & 0x8000)) {
				//if (ebx.flags < 0) {
					if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
						if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
							continue;
						}
					}
					//ebx.w |= 0x8000;
					//ebx.flags |= 0x80;
					__asm__ volatile (
						"orb %[value], %h[flags]"
						: [flags]"+Q"(ebx.w)
						: [value]"i"(0x80)
					);
					ret = SIZE_MAX;
				}
			}
		end_parse:
			if (ebx.w & 0x0100) {
			//if (ebx.flags & 0x01) {
				ret = -ret;
			}
			return std::pair(ret, (const char*)str_read);
	}
}

dllexport gnu_noinline auto fastcall strtouz7(const char* str) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint32_t number_base;
outer_loop:
	union {
		uint32_t w;
		_Atomic uint32_t aw;
		struct {
			uint8_t c;
			int8_t flags;
		};
		struct {
			_Atomic uint8_t ac;
			_Atomic int8_t aflags;
		};
	} ebx;
	switch (ebx.w = *str_read++) {
		default: error_ret:
			return std::pair((size_t)0, str);
		case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
			//continue;
			goto outer_loop;
		case '-': {
			ebx.w = *str_read++;
			//uint32_t temp = ebx.w - '1';
			ebx.w |= 0x0120;
			//if (temp < 9) goto decimal_prefix;
			switch ((uint8_t)ebx.w) {
				case '1' ... '9':
					goto decimal_prefix;
				default:
					goto error_ret;
				case '0':
					goto zero_prefix_case;
				case 'r':
			}
		}
		case 'r': case 'R':
			//ebx.c = '0';
			//ebx.w &= ~UINT8_MAX;
			//ebx.w |= '0';
			__asm__ volatile (
				"movb %[value], %b[c]"
				: [c]"=q"(ebx.w)
				: [value]"i"('0')
			);
		case '0': zero_prefix_case:
			switch (*str_read) {
				case 'b':
					number_base = 2;
					++str_read;
					goto post_prefix;
				case 'x':
					number_base = 16;
					++str_read;
					goto post_prefix;
			}
		case '1' ... '9': decimal_prefix:
			number_base = 10;
		post_prefix:
			size_t ret = 0;
			for (;;) {
				uint32_t digit = (uint8_t)ebx.w;
				switch (digit) {
					case '0' ... '9':
						digit -= '0';
						break;
					case 'a' ... 'f':
						digit -= 'a';
						break;
					default:
						goto end_parse;
				}
				if (digit >= number_base) break;
				//ebx.ac = lowercase(*str_read);
				//ebx.w &= ~UINT8_MAX;
				//ebx.w |= lowercase(*str_read);
				//ebx.w = (ebx.w & ~UINT8_MAX) | lowercase(*str_read);
				__asm__ volatile (
					"movb (%[str_read]), %b[c]"
					: [c]"=q"(ebx.w)
					: asm_arg("r", str_read)
				);
				++str_read;
				if (!(ebx.w & 0x8000)) {
				//if (ebx.flags < 0) {
					if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
						if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
							continue;
						}
					}
					//ebx.w |= 0x8000;
					//ebx.flags |= 0x80;
					__asm__ volatile (
						"movb %[value], %h[flags]"
						: [flags]"+Q"(ebx.w)
						: [value]"i"(0x80)
					);
					ret = SIZE_MAX;
				}
			}
		end_parse:
			bool is_negate;
			__asm__ volatile (
				"testb %[value], %h[flags]"
				: asm_flags(s, is_negate)
				: [flags]"Q"(ebx.w), [value]"i"(0x01)
			);
			if (is_negate) {
			//if (ebx.w & 0x0100) {
			//if (ebx.flags & 0x01) {
				ret = -ret;
			}
			return std::pair(ret, (const char*)str_read);
	}
}

dllexport gnu_noinline auto fastcall strtouz8(const char* str) {
	const uint8_t* str_read = (const uint8_t*)str;
	uint32_t number_base = 10;
outer_loop:
	union {
		uint32_t w;
		struct {
			uint8_t c;
			int8_t flags;
		};
	} ebx;
	switch (ebx.w = *str_read++) {
		[[unlikely]] default: error_ret:
			return std::pair((size_t)0, str);
		case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
			goto outer_loop;
		case '-': {
			ebx.w = *str_read++ | 0x0120;
			switch ((uint8_t)ebx.w) {
				case '1' ... '9':
					goto post_prefix;
				[[unlikely]] default:
					goto error_ret;
				case '0':
					goto zero_prefix_case;
				case 'r':
			}
		}
		case 'r': case 'R':
			__asm__ volatile (
				"movb %[value], %b[c]"
				: [c]"=q"(ebx.w)
				: [value]"i"('0')
			);
		case '0': zero_prefix_case:
			assume((ebx.w & 0xFF) == '0');
			switch (lowercase(*str_read)) {
				case 'b':
					number_base = 2;
					++str_read;
					goto post_prefix;
				[[likely]] case 'x':
					number_base = 16;
					++str_read;
					goto post_prefix;
			}
		case '1' ... '9': post_prefix:
			size_t ret = 0;
			for (;;) {
				uint32_t digit = (uint8_t)ebx.w;
				switch (digit) {
					case '0' ... '9':
						digit -= '0';
						break;
					case 'a' ... 'f':
						digit -= 'a';
						break;
					default:
						goto end_parse;
				}
				if (digit >= number_base) break;
				__asm__ volatile (
					"movb (%[str_read]), %b[c]"
					: [c]"=q"(ebx.w)
					: asm_arg("r", str_read)
				);
				++str_read;
				bool is_max;
				__asm__ volatile (
					"testb %h[flags], %h[flags]"
					: asm_flags(s, is_max)
					: [flags]"Q"(ebx.w)
				);
				if (!is_max) {
				//if (!(ebx.w & 0x8000)) {
				//if (ebx.flags < 0) {
					if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
						if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
							continue;
						}
					}
					__asm__ volatile (
						"movb %[value], %h[flags]"
						: [flags]"+Q"(ebx.w)
						: [value]"i"(0x80)
					);
					ret = SIZE_MAX;
				}
			}
		end_parse:
			bool is_negate;
			__asm__ volatile (
				"testb %[value], %h[flags]"
				: asm_flags(s, is_negate)
				: [flags]"Q"(ebx.w), [value]"i"(0x01)
			);
			if (is_negate) {
			//if (ebx.w & 0x0100) {
			//if (ebx.flags & 0x01) {
				ret = -ret;
			}
			return std::pair(ret, (const char*)str_read);
	}
}

dllexport gnu_noinline auto fastcall strtouz9(const char* str) {
	const char* original_str = str;
	uint32_t number_base = 10;
outer_loop:
	union {
		uint32_t w;
		struct {
			uint8_t c;
			int8_t flags;
		};
	} ebx;
	switch (ebx.w = (uint8_t)*str++) {
		[[unlikely]] default: error_ret:
			return std::pair((size_t)0, original_str);
		case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
			goto outer_loop;
		case '-': {
			ebx.w = (uint8_t)*str++ | 0x0100;
			switch ((uint8_t)ebx.w) {
				case '1' ... '9':
					goto post_prefix;
				[[unlikely]] default:
					goto error_ret;
				case '0':
					goto zero_prefix_case;
				case 'r': case 'R':
			}
		}
		case 'r': case 'R':
			__asm__ volatile (
				"movb %[value], %b[c]"
				: [c]"=q"(ebx.w)
				: [value]"i"('0')
			);
		case '0': zero_prefix_case:
			assume((ebx.w & 0xFF) == '0');
			switch ((uint8_t)*str) {
				case 'b': case 'B':
					number_base = 2;
					++str;
					goto post_prefix;
				[[likely]] case 'x': case 'X':
					number_base = 16;
					++str;
					goto post_prefix;
			}
		case '1' ... '9': post_prefix:
			size_t ret = 0;
			for (;;) {
				uint32_t digit = (uint8_t)ebx.w;
				switch (digit) {
					case '0' ... '9':
						digit -= '0';
						break;
					case 'a' ... 'f':
						digit -= 'a' - 10;
						break;
					case 'A' ... 'F':
						digit -= 'A' - 10;
						break;
					default:
						goto end_parse;
				}
				if (digit >= number_base) break;
				__asm__ volatile (
					"movb (%[str]), %b[c]"
					: [c]"=q"(ebx.w)
					: asm_arg("r", str)
				);
				++str;
				bool is_max;
				__asm__ volatile (
					"testb %h[flags], %h[flags]"
					: asm_flags(s, is_max)
					: [flags]"Q"(ebx.w)
				);
				if (!is_max) {
				//if (!(ebx.w & 0x8000)) {
				//if (ebx.flags < 0) {
					if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
						if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
							continue;
						}
					}
					__asm__ volatile (
						"movb %[value], %h[flags]"
						: [flags]"+Q"(ebx.w)
						: [value]"i"(0x80)
					);
					ret = SIZE_MAX;
				}
			}
		end_parse:
			bool is_negate;
			__asm__ volatile (
				"testb %[value], %h[flags]"
				: asm_flags(s, is_negate)
				: [flags]"Q"(ebx.w), [value]"i"(0x01)
			);
			if (is_negate) {
			//if (ebx.w & 0x0100) {
			//if (ebx.flags & 0x01) {
				ret = -ret;
			}
			return std::pair(ret, str);
	}
}

dllexport gnu_noinline auto fastcall strtouz10(const char* str) {
	const char* original_str = str;
	uint32_t number_base = 10;
outer_loop:
	union {
		uint32_t w;
		struct {
			uint8_t c;
			int8_t flags;
		};
	} ebx;
	switch (ebx.w = (uint8_t)*str++) {
		[[unlikely]] default: error_ret:
			return std::pair((size_t)0, original_str);
		case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
			goto outer_loop;
		case '-': {
			ebx.w = (uint8_t)*str++ | 0x0100;
			switch ((uint8_t)ebx.w) {
				case '1' ... '9':
					goto post_prefix;
				[[unlikely]] default:
					goto error_ret;
				case '0':
					goto zero_prefix_case;
				case 'r': case 'R':
			}
		}
		case 'r': case 'R':
			__asm__ volatile (
				"movb %[value], %b[c]"
				: [c]"=q"(ebx.w)
				: [value]"i"('0')
			);
		case '0': zero_prefix_case:
			assume((ebx.w & 0xFF) == '0');
			{
				uint8_t temp = (uint8_t)*str;
				if (temp == 'x' || temp == 'X') {
					number_base = 16;
					temp = *++str;
				} else if (temp == 'b' || temp == 'B') {
					number_base = 2;
					temp = *++str;
				}
				__asm__ volatile (
					"movb %[temp], %b[c]"
					: [c]"=q"(ebx.w)
					: asm_arg("q", temp)
				);
			}
		case '1' ... '9':
		post_prefix:
			size_t ret = 0;
			for (;;) {
				uint32_t digit = (uint8_t)ebx.w;
				if (digit == '0') {

				} else if (digit == 'a') {

				}
				switch (digit) {
					case '0' ... '9':
						digit -= '0';
						break;
					case 'a' ... 'f':
						digit -= 'a' - 10;
						break;
					case 'A' ... 'F':
						digit -= 'A' - 10;
						break;
					default:
						goto end_parse;
				}
				if (digit >= number_base) break;
				__asm__ volatile (
					"movb (%[str]), %b[c]"
					: [c]"=q"(ebx.w)
					: asm_arg("r", str)
				);
				++str;
				bool is_max;
				__asm__ volatile (
					"testb %h[flags], %h[flags]"
					: asm_flags(s, is_max)
					: [flags]"Q"(ebx.w)
				);
				if (!is_max) {
				//if (!(ebx.w & 0x8000)) {
				//if (ebx.flags < 0) {
					if (expect(!__builtin_mul_overflow(ret, number_base, &ret), true)) {
						if (expect(!__builtin_add_overflow(ret, digit, &ret), true)) {
							continue;
						}
					}
					__asm__ volatile (
						"movb %[value], %h[flags]"
						: [flags]"+Q"(ebx.w)
						: [value]"i"(0x80)
					);
					ret = SIZE_MAX;
				}
			}
		end_parse:
			bool is_negate;
			__asm__ volatile (
				"testb %[value], %h[flags]"
				: asm_flags(s, is_negate)
				: [flags]"Q"(ebx.w), [value]"i"(0x01)
			);
			if (is_negate) {
			//if (ebx.w & 0x0100) {
			//if (ebx.flags & 0x01) {
				ret = -ret;
			}
			return std::pair(ret, str);
	}
}