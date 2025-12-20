#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include "windows.h"

#include "last_branch_enable.h"

static const wchar_t NTDLL_NAME[] = L"ntdll.dll";
static bool last_branch_initialized = false;
static bool last_branch_functional = false;

#if !_M_X64 && !__x86_64__
static const wchar_t WOW64_NAME[] = L"wow64.dll";
static const char PREPARE_EXCEPTION_NAME[] = "Wow64PrepareForException";
static const char PROTECT_MEMORY_NAME[] = "NtProtectVirtualMemory";

#define static
struct TEB_dummy_t {
    unsigned char pad0[0xE10];
    uint32_t TlsSlots[32];
    unsigned char pad1[0x84];
    uint32_t* TlsExpansionSlots;
};
inline static uintptr_t get_tls_slot(struct TEB_dummy_t* teb, DWORD slot) {
    if (slot < 64u) {
        return teb->TlsSlots[slot];
    } else {
        return teb->TlsExpansionSlots[slot];
    }
}
struct THREAD_BASIC_INFORMATION_t {
    LONG ExitStatus;
    struct TEB_dummy_t* TebBaseAddress;
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
    SIZE_T AffinityMask;
    LONG Priority;
    LONG BasePriority;
};
typedef LONG WINAPI NtQueryInformationThread_t(HANDLE ThreadHandle, int ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);

static uint32_t exception_to_tls = 0;
static uint32_t exception_from_tls = 0;
static NtQueryInformationThread_t* NtQueryInformationThread_ptr = NULL;
static uint64_t original_prepare_addr = 0;
static uint64_t nt_virtual_protect_addr = 0;
static uint64_t ntdll_pointer_addr = 0;

#if __GNUC__ || __clang__
#define FS_OVERRIDE ".byte 0x64\n "
#define GS_OVERRIDE ".byte 0x65\n "
#define DATASIZE ".byte 0x66\n"
#define ADDRSIZE ".byte 0x67\n"
#define REX "incl %%eax\n"
#define REX_B "incl %%ecx\n"
#define REX_X "incl %%edx\n"
#define REX_XB "incl %%ebx\n"
#define REX_R "incl %%esp\n"
#define REX_RB "incl %%ebp\n"
#define REX_RX "incl %%esi\n"
#define REX_RXB "incl %%edi\n"
#define REX_W "decl %%eax\n"
#define REX_WB "decl %%ecx\n"
#define REX_WX "decl %%edx\n"
#define REX_WXB "decl %%ebx\n"
#define REX_WR "decl %%esp\n"
#define REX_WRB "decl %%ebp\n"
#define REX_WRX "decl %%esi\n"
#define REX_WRXB "decl %%edi\n"
#define rax " %%eax "
#define eax " %%eax "
#define rcx " %%ecx "
#define ecx " %%ecx "
#define cl " %%cl "
#define rdx " %%edx "
#define edx " %%edx "
#define rbx " %%ebx "
#define ebx " %%ebx "
#define rsp " %%esp "
#define esp " %%esp "
#define rsi " %%esi "
#define esi " %%esi "
#define rdi " %%edi "
#define edi " %%edi "
#define r8 " %%eax "
#define r8d " %%eax "
#define r9 " %%ecx "
#define r9d " %%ecx "
#define r10 " %%edx "
#define r10d " %%edx "
#define r11 " %%ebx "
#define r11d " %%ebx "
__attribute__((naked)) static void prepare_for_exception_hook() {
    __asm__ volatile(
        ".byte 0x8B, 0x04, 0x25 \n .int %c[except_to_slot]\n"
        REX_R "movl 0x4C0(" rdx ")," r8d "\n"
        "cmpl $0x40," eax "\n"
        "jnb 1f\n"
        FS_OVERRIDE REX_R "movl" r8d ",0xE10(," rax ",4)\n"
        "jmp 2f\n"
    "1:\n"
        FS_OVERRIDE REX_R ".byte 0x8B, 0x0C, 0x25 \n .int 0x0F94\n"
        REX_RB "movl" r8d ", -0x100(" r9 "," rax ",4)\n"
    "2:\n"
        ".byte 0x8B, 0x04, 0x25 \n .int %c[except_from_slot]\n"
        REX_R "movl 0x4C8(" rdx ")," r8d "\n"
        "cmpl $0x40," eax "\n"
        "jnb 1f\n"
        FS_OVERRIDE REX_R "movl" r8d ", 0xE10(," rax ",4)\n"
        "jmp 2f\n"
    "1:\n"
        FS_OVERRIDE REX_R ".byte 0x8B, 0x0C, 0x25 \n .int 0x0F94\n"
        REX_RB "movl" r8d ", -0x100(" r9 "," rax ",4)\n"
    "2: \n"
        ".byte 0xFF, 0x24, 0x25 \n .int %c[func_ptr] \n"
        "int3 \n"
        :
        :
        [except_to_slot]"i"(&exception_to_tls),
        [except_from_slot]"i"(&exception_from_tls),
        [func_ptr]"i"(&original_prepare_addr)
    );
}
__attribute__((naked)) static void initialize_wow_exception_hooks() {
    __asm__ volatile (
        GS_OVERRIDE REX_W ".byte 0x8B, 0x04, 0x25 \n .int 0x60\n"
        REX_WR "movl 0x18(" rax ")," r9 "\n"
        REX_WB "movl 0x10(" r9 ")," rdx "\n"
        REX_WB "addl $0x10," r9 "\n"
        "xorl" eax "," eax "\n"
        REX_RB "xorl" r8d "," r8d "\n"
        REX_RB "xorl" r11d "," r11d "\n"
        "jmp 1f\n"
        "int3\n"
    "3:\n"
        REX_W "movl 0x60(" rdx ")," rdi "\n"
        "movl $18," ecx "\n"
        "movl %[ntdll_name]," esi "\n"
        "repe cmpsb\n"
        "jne 3f\n"
        REX_WR "movl 0x30(" rdx ")," r8 "\n"
        REX_W "testl" rax "," rax "\n"
        "jnz 4f\n"
    ".nops 5\n"
    "2:\n"
        REX_W "movl (" rdx ")," rdx "\n"
        REX_WR "cmpl" r9 "," rdx "\n"
        "je 9f\n"
    "1:\n"
        "cmpw $18, 0x58(" rdx ")\n"
        "jne 2b\n"
        REX_WRB "testl" r8 "," r8 "\n"
        "jz 3b\n"
    "3:\n"
        REX_W "testl" rax "," rax "\n"
        "jnz 2b\n"
        REX_W "movl 0x60(" rdx ")," rdi "\n"
        "movl $18," ecx "\n"
        "movl %[wow64_name]," esi "\n"
        "repe cmpsb\n"
        "jne 2b\n"
        REX_W "movl 0x30(" rdx ")," rax "\n"
        REX_WRB "testl" r8 "," r8 "\n"
        "jz 2b\n"
    "4:\n"
        "movl 0x3C(" rax ")," ecx "\n"
        REX_R "movl 0x88(" rcx "," rax ")," r9d "\n"
        REX_B "movl 0x18(" r9 "," rax ")," edx "\n"
        REX_RB "movl 0x20(" r9 "," rax ")," r10d "\n"
        REX_WB "addl" rax "," r10 "\n"
    "1:\n"
        "subl $1," edx "\n"
        "jb 9f\n"
        REX_B "movl (" r10 "," rdx ",4)," edi "\n"
        REX_W "addl" rax "," rdi "\n"
        "movl $25," ecx "\n"
        "movl %[prepare_exception_name]," esi "\n"
        "repe cmpsb\n"
        "jne 1b\n"
        REX_B "movl 0x1C(" r9 "," rax ")," ecx "\n"
        REX_B "movl 0x24(" r9 "," rax ")," esi "\n"
        REX_W "addl" rax "," rsi "\n"
        "movzwl (" rsi "," rdx ",2)," edx "\n"
        REX_W "addl" rax "," rcx "\n"
        "movl (" rcx "," rdx ",4)," edi "\n"
        REX_W "addl" rdi "," rax "\n"
        REX_B "movl 0x3C(" r8 ")," edx "\n"
        REX_RX "movl 0x88(" rdx "," r8 ")," r9d "\n"
        REX_WR "addl" r8 "," rdx "\n"
        REX_RXB "movl 0x18(" r9 "," r8 ")," r11d "\n"
        REX_RXB "movl 0x20(" r9 "," r8 ")," r10d "\n"
        REX_WRB "addl" r8 "," r10 "\n"
    ".nops 6\n"
    ".nops 6\n"
    "1:\n"
        REX_B "subl $1," r11d "\n"
        "jb 9f\n"
        REX_XB "movl (" r10 "," r11 ",4)," edi "\n"
        REX_WR "addl" r8 "," rdi "\n"
        "movl $23," ecx "\n"
        "movl %[protect_memory_name]," esi "\n"
        "repe cmpsb\n"
        "jne 1b\n"
        REX_XB "movl 0x1C(" r9 "," r8 ")," ecx "\n"
        REX_XB "movl 0x24(" r9 "," r8 ")," edi "\n"
        REX_WR "addl" r8 "," rdi "\n"
        REX_X "movzwl (" rdi "," r11 ",2)," esi "\n"
        REX_WR "addl" r8 "," rcx "\n"
        REX_R "movl (" rcx "," rsi ",4)," r9d "\n"
        REX_WRB "addl" r8 "," r9 "\n"
        "movzwl 0x14(" rdx ")," esi "\n"
        REX_W "addl" rdx "," rsi "\n"
        "movzwl 0x6(" rdx ")," edx "\n"
        REX_RB "xorl" r11d "," r11d "\n"
        "jmp 1f\n"
        "int3\n"
        "int3\n"
        "int3\n"
        "int3\n"
        "int3\n"
    "2:\n"
        REX_W "addl $0x28," rsi "\n"
        ".byte 0xFF, 0xCA \n"
        "jz 9f\n"
    "1:\n"
        "movzbl 0x3F(" rsi ")," ecx "\n"
        "andb $0x60," cl "\n"
        "cmpb $0x40," cl "\n"
        "jne 2b\n"
        "movl 0x20(" rsi ")," ecx "\n"
        "movl 0x24(" rsi ")," edi "\n"
        REX_WR "addl" r8 "," rdi "\n"
        "shrl $3," ecx "\n"
        ".byte 0xF2, 0x48, 0xAF\n"
        "jne 2b\n"
        REX_W ".byte 0x89, 0x04, 0x25 \n .int %c[prepare_addr]\n"
        REX_WR ".byte 0x89, 0x0C, 0x25 \n .int %c[protect_addr]\n"
        REX_W "addl $-8," rdi "\n"
        REX_W ".byte 0x89, 0x3C, 0x25 \n .int %c[func_ptr_addr]\n"
        REX_B "movl $1," r11d "\n"
    "9: \n"
        REX_R "movl" r11d "," eax "\n"
        "lret\n"
        "int3\n"
        :
        :
        [ntdll_name]"i"(&NTDLL_NAME),
        [wow64_name]"i"(&WOW64_NAME),
        [prepare_exception_name]"i"(&PREPARE_EXCEPTION_NAME),
        [protect_memory_name]"i"(&PROTECT_MEMORY_NAME),
        [prepare_addr]"i"(&original_prepare_addr),
        [protect_addr]"i"(&nt_virtual_protect_addr),
        [func_ptr_addr]"i"(&ntdll_pointer_addr)
    );
}
__attribute__((always_inline)) static inline int32_t initialize_wow_exception_hooks_thunk() {
    int32_t ret;
    __asm__ volatile (
        "lcall %[Seg],%[Addr]"
        : "=a"(ret), "=m"(original_prepare_addr), "=m"(nt_virtual_protect_addr), "=m"(ntdll_pointer_addr)
        : [Seg]"i"(0x33), [Addr]"i"(&initialize_wow_exception_hooks)
        : "ecx", "edx", "esi", "edi", "flags"
    );
    return ret;
}
__attribute__((naked)) static void hook_prepare_pointer() {
    __asm__ volatile(
        "movl" esp "," ebx "\n"
        "subl $0x40," esp "\n"
        "andl $-0x10," esp "\n"
        REX_W ".byte 0x8B, 0x3C, 0x25 \n .int %c[func_ptr_addr]\n"
        REX_W "movl" rdi ", 0x38(" rsp ")\n"
        REX_W "movl $8, 0x30(" rsp ")\n"
        REX_W ".byte 0x8B, 0x34, 0x25 \n .int %c[protect_addr]\n"
        "leal 0x2C(" rsp ")," eax "\n"
        REX_W "movl" rax ", 0x20(" rsp ")\n"
        "leal 0x38(" rsp ")," edx "\n"
        REX_R "leal 0x30(" rsp ")," r8d "\n"
        REX_W ".byte 0xC7, 0xC1 \n .int -1\n"
        REX_B "movl $4," rcx "\n"
        "calll *" rsi "\n"
        "testl" eax "," eax "\n"
        "js 1f\n"
        "movl %[new_func]," eax "\n"
        REX_W "xchgl" rax ", (" rdi ")\n"
        REX_R "movl 0x2C(" rsp ")," r9d "\n"
        "leal 0x2C(" rsp ")," eax "\n"
        REX_W "movl" rax ", 0x20(" rsp ")\n"
        "leal 0x38(" rsp ")," edx "\n"
        REX_R "leal 0x30(" rsp ")," r8d "\n"
        REX_W ".byte 0xC7, 0xC1 \n .int -1\n"
        "calll *" rsi "\n"
        "xorl" eax "," eax "\n"
    "1:\n"
        "movl" ebx "," esp "\n"
        "lret\n"
        "int3\n"
        :
        :
        [func_ptr_addr]"i"(&ntdll_pointer_addr),
        [protect_addr]"i"(&nt_virtual_protect_addr),
        [new_func]"i"(&prepare_for_exception_hook)
    );
}
__attribute__((always_inline)) static inline int32_t hook_prepare_pointer_thunk() {
    int32_t ret;
    __asm__ volatile (
        "lcall %[Seg],%[Addr]"
        : "=a"(ret)
        : [Seg]"i"(0x33), [Addr]"i"(&hook_prepare_pointer)
        : "ecx", "edx", "ebx", "esi", "edi", "flags"
    );
    return ret;
}
__attribute__((naked)) static void unhook_prepare_pointer() {
    __asm__ volatile(
        "movl" esp "," ebx "\n"
        "subl $0x40," esp "\n"
        "andl $-0x10," esp "\n"
        REX_W ".byte 0x8B, 0x3C, 0x25 \n .int %c[func_ptr_addr]\n"
        REX_W "movl" rdi ", 0x38(" rsp ")\n"
        REX_W "movl $8, 0x30(" rsp ")\n"
        REX_W ".byte 0x8B, 0x34, 0x25 \n .int %c[protect_addr]\n"
        "leal 0x2C(" rsp ")," eax "\n"
        REX_W "movl" rax ", 0x20(" rsp ")\n"
        "leal 0x38(" rsp ")," edx "\n"
        REX_R "leal 0x30(" rsp ")," r8d "\n"
        REX_W ".byte 0xC7, 0xC1 \n .int -1\n"
        REX_B "movl $4," r9d "\n"
        "calll *" rsi "\n"
        "testl" eax "," eax "\n"
        "js 1f\n"
        ".byte 0x8B, 0x04, 0x25 \n .int %c[original_func]\n"
        REX_W "xchgl" rax ", (" rdi ")\n"
        REX_R "movl 0x2C(" rsp ")," r9d "\n"
        "leal 0x2C(" rsp ")," eax "\n"
        REX_W "movl" rax ", 0x20(" rsp ")\n"
        "leal 0x38(" rsp ")," edx "\n"
        REX_R "leal 0x30(" rsp ")," r8d "\n"
        REX_W ".byte 0xC7, 0xC1 \n .int -1\n"
        "calll *" rsi "\n"
        "xorl" eax "," eax "\n"
    "1:\n"
        "movl" ebx "," esp "\n"
        "lret\n"
        "int3\n"
        :
        :
        [func_ptr_addr]"i"(&ntdll_pointer_addr),
        [protect_addr]"i"(&nt_virtual_protect_addr),
        [original_func]"i"(&original_prepare_addr)
    );
}
__attribute__((always_inline)) static inline int32_t unhook_prepare_pointer_thunk() {
    int32_t ret;
    __asm__ volatile (
        "lcall %[Seg],%[Addr]"
        : "=a"(ret)
        : [Seg]"i"(0x33), [Addr]"i"(&unhook_prepare_pointer)
        : "ecx", "edx", "ebx", "esi", "edi", "flags"
    );
    return ret;
}
#elif _MSC_VER
struct FarPointer {
    void* addr;
    uint16_t segment;
};
#define FS_OVERRIDE __asm _emit 0x64 __asm
#define GS_OVERRIDE __asm _emit 0x65 __asm
#define DATASIZE __asm _emit 0x66 __asm
#define ADDRSIZE __asm _emit 0x67 __asm
#define REX __asm INC EAX __asm
#define REX_B __asm INC ECX __asm
#define REX_X __asm INC EDX __asm
#define REX_XB __asm INC EBX __asm
#define REX_R __asm INC ESP __asm
#define REX_RB __asm INC EBP __asm
#define REX_RX __asm INC ESI __asm
#define REX_RXB __asm INC EDI __asm
#define REX_W __asm DEC EAX __asm
#define REX_WB __asm DEC ECX __asm
#define REX_WX __asm DEC EDX __asm
#define REX_WXB __asm DEC EBX __asm
#define REX_WR __asm DEC ESP __asm
#define REX_WRB __asm DEC EBP __asm
#define REX_WRX __asm DEC ESI __asm
#define REX_WRXB __asm DEC EDI __asm
#define RAX EAX
#define RCX ECX
#define RDX EDX
#define RSP ESP
#define RSI ESI
#define RDI EDI
#define R8 EAX
#define R8D EAX
#define R9 ECX
#define R9D ECX
#define R10 EDX
#define R10D EDX
#define R11 EBX
#define R11D EBX
#define QWORD DWORD
__declspec(naked) static void prepare_for_exception_hook() {
    __asm {
        ADDRSIZE MOV EAX, DWORD PTR [exception_to_tls]
        REX_R MOV R8D, DWORD PTR [RDX+0x4C0]
        CMP EAX, 64
        JNB ext_tls_except_to
        FS_OVERRIDE REX_R MOV DWORD PTR [RAX*4+0xE10], R8D
        JMP tls_except_from
    ext_tls_except_to:
        FS_OVERRIDE REX_R
        _emit 0x8B
        _emit 0x0C
        _emit 0x25
        _emit 0x94
        _emit 0x0F
        _emit 0x00
        _emit 0x00
        REX_RB MOV DWORD PTR [RAX*4+R9-0x100], R8D
    tls_except_from:
        ADDRSIZE MOV EAX, DWORD PTR [exception_from_tls]
        REX_R MOV R8D, DWORD PTR [RDX+0x4C8]
        CMP EAX, 64
        JNB ext_tls_except_from
        FS_OVERRIDE REX_R MOV DWORD PTR [RAX*4+0xE10], R8D
        JMP call_original_prepare
    ext_tls_except_from:
        FS_OVERRIDE REX_R
        _emit 0x8B
        _emit 0x0C
        _emit 0x25
        _emit 0x94
        _emit 0x0F
        _emit 0x00
        _emit 0x00
        REX_RB MOV DWORD PTR [RAX*4+R9-0x100], R8D
    call_original_prepare:
        ADDRSIZE REX_W MOV RAX, QWORD PTR [original_prepare_addr]
        JMP RAX
    }
}
__declspec(naked) static void initialize_wow_exception_hooks() {
    __asm {
        GS_OVERRIDE REX_W
        _emit 0x8B
        _emit 0x04
        _emit 0x25
        _emit 0x60
        _emit 0x00
        _emit 0x00
        _emit 0x00
        REX_WR MOV R9, QWORD PTR [RAX+0x18]
        REX_WB MOV RDX, QWORD PTR [R9+0x10]
        REX_WB ADD R9, 0x10
        XOR EAX, EAX
        REX_RB XOR R8D, R8D
        REX_RB XOR R11D, R11D
        JMP start_module_loop
        INT 3
    check_ntdll:
        REX_W MOV RDI, QWORD PTR [RDX+0x60]
        MOV ECX, 18
        MOV ESI, OFFSET NTDLL_NAME
        REPE CMPSB
        JNE not_ntdll
        REX_WR MOV R8, QWORD PTR [RDX+0x30]
        REX_W TEST RAX, RAX
        JNZ both_modules_found
        _emit 0x0F
        _emit 0x1F
        _emit 0x44
        _emit 0x00
        _emit 0x00
    next_module:
        REX_W MOV RDX, QWORD PTR [RDX]
        REX_WR CMP RDX, R9
        JE initialize_hook_fail
    start_module_loop:
        CMP WORD PTR [RDX+0x58], 18
        JNE next_module
        REX_WRB TEST R8, R8
        JZ check_ntdll
    not_ntdll:
        REX_W TEST RAX, RAX
        JNZ next_module
        REX_W MOV RDI, QWORD PTR [RDX+0x60]
        MOV ECX, 18
        MOV ESI, OFFSET WOW64_NAME
        REPE CMPSB
        JNE next_module
    both_modules_found:
        MOV ECX, DWORD PTR [RAX+0x3C]
        REX_R MOV R9D, DWORD PTR [RAX*1+RCX+0x88]
        REX_B MOV EDX, DWORD PTR [RAX*1+R9+0x18]
        REX_RB MOV R10D, DWORD PTR [RAX*1+R9+0x20]
        REX_WB ADD R10, RAX
    next_wow64_export:
        SUB EDX, 1
        JC initialize_hook_fail
        REX_B MOV EDI, DWORD PTR [RDX*4+R10]
        REX_W ADD RDI, RAX
        MOV ECX, 25
        MOV ESI, OFFSET PREPARE_EXCEPTION_NAME
        REPE CMPSB
        JNE next_wow64_export
        REX_B MOV ECX, DWORD PTR [RAX*1+R9+0x1C]
        REX_B MOV ESI, DWORD PTR [RAX*1+R9+0x24]
        REX_W ADD RSI, RAX
        MOVZX EDX, WORD PTR [RDX*2+RSI]
        REX_W ADD RCX, RAX
        MOV EDI, DWORD PTR [RDX*4+RCX]
        REX_W ADD RAX, RDI
        REX_B MOV EDX, DWORD PTR [R8+0x3C]
        REX_RX MOV R9D, DWORD PTR [R8*1+RDX+0x88]
        REX_WR ADD RDX, R8
        REX_RXB MOV R11D, DWORD PTR [R8*1+R9+0x18]
        REX_RXB MOV R10D, DWORD PTR [R8*1+R9+0x20]
        REX_WRB ADD R10, R8
        _emit 0x66
        _emit 0x0F
        _emit 0x1F
        _emit 0x44
        _emit 0x00
        _emit 0x00
        _emit 0x66
        _emit 0x0F
        _emit 0x1F
        _emit 0x44
        _emit 0x00
        _emit 0x00
    next_ntdll_export:
        REX_B SUB R11D, 1
        JC initialize_hook_fail
        REX_XB MOV EDI, DWORD PTR [R11*4+R10]
        REX_WR ADD RDI, R8
        MOV ECX, 23
        MOV ESI, OFFSET PROTECT_MEMORY_NAME
        REPE CMPSB
        JNE next_ntdll_export
        REX_XB MOV ECX, DWORD PTR [R8*1+R9+0x1C]
        REX_XB MOV EDI, DWORD PTR [R8*1+R9+0x24]
        REX_WR ADD RDI, R8
        REX_X MOVZX ESI, WORD PTR [R11*2+RDI]
        REX_WR ADD RCX, R8
        REX_R MOV R9D, DWORD PTR [RSI*4+RCX]
        REX_WRB ADD R9, R8
        MOVZX ESI, WORD PTR [RDX+0x14]
        REX_W ADD RSI, RDX
        MOVZX EDX, WORD PTR [RDX+0x6]
        REX_RB XOR R11D, R11D
        JMP start_section_loop
        INT 3
        INT 3
        INT 3
        INT 3
        INT 3
    next_section:
        REX_W ADD RSI, 0x28
        _emit 0xFF
        _emit 0xCA
        JZ initialize_hook_fail
    start_section_loop:
        MOVZX ECX, BYTE PTR [RSI+0x3F]
        AND CL, 0x60
        CMP CL, 0x40
        JNE next_section
        MOV ECX, DWORD PTR [RSI+0x20]
        MOV EDI, DWORD PTR [RSI+0x24]
        REX_WR ADD RDI, R8
        SHR ECX, 3
        _emit 0xF2
        _emit 0x48
        _emit 0xAF
        JNE next_section
        MOV EDX, OFFSET original_prepare_addr
        REX_W MOV QWORD PTR [RDX], RAX
        MOV ECX, OFFSET nt_virtual_protect_addr
        REX_WR MOV QWORD PTR [RCX], R9
        REX_W ADD RDI, -8
        MOV EAX, OFFSET ntdll_pointer_addr
        REX_W MOV QWORD PTR [RAX], RDI
        REX_B MOV R11D, 1
    initialize_hook_fail:
        REX_R MOV EAX, R11D
        RETF
    }
}
static const struct FarPointer INITIALIZE_THUNK = { (void*)&initialize_wow_exception_hooks, 0x33 };
__forceinline static int32_t initialize_wow_exception_hooks_thunk() {
    __asm {
        MOV ESI, ESI
        MOV EDI, EDI
        CALL FWORD PTR [INITIALIZE_THUNK]
    }
}
__declspec(naked) static void hook_prepare_pointer() {
    __asm {
        MOV EBX, ESP
        SUB ESP, 0x40
        AND ESP, -16
        ADDRSIZE REX_W MOV RAX, QWORD PTR [ntdll_pointer_addr]
        REX_W MOV RDI, RAX
        REX_W MOV QWORD PTR [RSP+0x38], RDI
        REX_W MOV QWORD PTR [RSP+0x30], 8
        ADDRSIZE REX_W MOV RAX, QWORD PTR [nt_virtual_protect_addr]
        REX_W MOV RSI, RAX
        LEA EAX, [RSP+0x2C]
        REX_W MOV QWORD PTR [RSP+0x20], RAX
        LEA EDX, [RSP+0x38]
        REX_R LEA R8D, [RSP+0x30]
        REX_W
        _emit 0xC7
        _emit 0xC1
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        REX_B MOV R9D, 4
        CALL RSI
        TEST EAX, EAX
        JS hook_fail
        MOV EAX, OFFSET prepare_for_exception_hook
        REX_W XCHG QWORD PTR [RDI], RAX
        REX_R MOV R9D, DWORD PTR [RSP+0x2C]
        LEA EAX, [RSP+0x2C]
        REX_W MOV QWORD PTR [RSP+0x20], RAX
        LEA EDX, [RSP+0x38]
        REX_R LEA R8D, [RSP+0x30]
        REX_W
        _emit 0xC7
        _emit 0xC1
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        CALL RSI
        XOR EAX, EAX
    hook_fail:
        MOV ESP, EBX
        RETF
    }
}
static const struct FarPointer HOOK_THUNK = { (void*)&hook_prepare_pointer, 0x33 };
__forceinline static int32_t hook_prepare_pointer_thunk() {
    __asm {
        MOV ESI, ESI
        MOV EDI, EDI
        MOV EBX, EBX
        CALL FWORD PTR [HOOK_THUNK]
    }
}
__declspec(naked) static void unhook_prepare_pointer() {
    __asm {
        MOV EBX, ESP
        SUB ESP, 0x40
        AND ESP, -16
        ADDRSIZE REX_W MOV RAX, QWORD PTR [ntdll_pointer_addr]
        REX_W MOV RDI, RAX
        REX_W MOV QWORD PTR [RSP+0x38], RDI
        REX_W MOV QWORD PTR [RSP+0x30], 8
        ADDRSIZE REX_W MOV RAX, QWORD PTR [nt_virtual_protect_addr]
        REX_W MOV RSI, RAX
        LEA EAX, [RSP+0x2C]
        REX_W MOV QWORD PTR [RSP+0x20], RAX
        LEA EDX, [RSP+0x38]
        REX_R LEA R8D, [RSP+0x30]
        REX_W
        _emit 0xC7
        _emit 0xC1
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        REX_B MOV R9D, 4
        CALL RSI
        TEST EAX, EAX
        JS hook_fail
        ADDRSIZE REX_W MOV RAX, QWORD PTR [original_prepare_addr]
        REX_W XCHG QWORD PTR [RDI], RAX
        REX_R MOV R9D, DWORD PTR [RSP+0x2C]
        LEA EAX, [RSP+0x2C]
        REX_W MOV QWORD PTR [RSP+0x20], RAX
        LEA EDX, [RSP+0x38]
        REX_R LEA R8D, [RSP+0x30]
        REX_W
        _emit 0xC7
        _emit 0xC1
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        _emit 0xFF
        CALL RSI
        XOR EAX, EAX
    hook_fail:
        MOV ESP, EBX
        RETF
    }
}
static const struct FarPointer UNHOOK_THUNK = { (void*)&unhook_prepare_pointer, 0x33 };
__forceinline static int32_t unhook_prepare_pointer_thunk() {
    __asm {
        MOV ESI, ESI
        MOV EDI, EDI
        MOV EBX, EBX
        CALL FWORD PTR [UNHOOK_THUNK]
    }
}
#endif
#endif

static bool initialize_last_branch_tracking() {
    if (last_branch_initialized) {
        return last_branch_functional;
    }
    last_branch_initialized = true;
    HMODULE ntdll = GetModuleHandleW(NTDLL_NAME);
    // Wine does not support the necessary undocumented behavior of DR7 bits
    if (!GetProcAddress(ntdll, "wine_get_version")) {
#if !_M_X64 && !__x86_64__
        typedef BOOL WINAPI IsWow64Process_t(HANDLE hProcess, PBOOL Wow64Process);
        IsWow64Process_t* IsWow64Process_ptr = (IsWow64Process_t*)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process");
        BOOL is_wow64;
        if (
            IsWow64Process_ptr &&
            IsWow64Process_ptr((HANDLE)~(uintptr_t)0, &is_wow64) &&
            is_wow64
        ) {
            NtQueryInformationThread_t* NtQueryInformationThread_temp = (NtQueryInformationThread_t*)GetProcAddress(ntdll, "NtQueryInformationThread");
            if (NtQueryInformationThread_temp) {
                NtQueryInformationThread_ptr = NtQueryInformationThread_temp;
                DWORD exception_to_tls_temp = TlsAlloc();
                if (exception_to_tls_temp != TLS_OUT_OF_INDEXES) {
                    DWORD exception_from_tls_temp = TlsAlloc();
                    if (exception_from_tls_temp != TLS_OUT_OF_INDEXES) {
                        if (initialize_wow_exception_hooks_thunk() > 0) {
                            exception_to_tls = exception_to_tls_temp;
                            exception_from_tls = exception_from_tls_temp;
#endif
                            {
                                last_branch_functional = true;
                                return true;
                            }
#if !_M_X64 && !__x86_64__
                        }
                        TlsFree(exception_from_tls_temp);
                    }
                    TlsFree(exception_to_tls_temp);
                }
            }
        }
#endif
    }
    return false;
}

bool last_branch_tracking_hook() {
    if (initialize_last_branch_tracking()) {
#if !_M_X64 && !__x86_64__
        if (hook_prepare_pointer_thunk() >= 0)
#endif
        {
            return true;
        }
    }
    return false;
}

bool last_branch_tracking_unhook() {
    if (last_branch_functional) {
#if !_M_X64 && !__x86_64__
        if (unhook_prepare_pointer_thunk() >= 0)
#endif
        {
            return true;
        }
    }
    return false;
}

bool last_branch_tracking_start() {
    if (last_branch_functional) {
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        GetThreadContext((HANDLE)~(uintptr_t)1, &context);
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        context.Dr7 |= 0x100;
        SetThreadContext((HANDLE)~(uintptr_t)1, &context);
        return true;
    }
    return false;
}

bool last_branch_tracking_stop() {
    if (last_branch_functional) {
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        GetThreadContext((HANDLE)~(uintptr_t)1, &context);
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        context.Dr7 &= ~0x100;
        SetThreadContext((HANDLE)~(uintptr_t)1, &context);
        return true;
    }
    return false;
}

uintptr_t last_branch_exception_to(HANDLE thread) {
    if (last_branch_functional) {
#if _M_X64 || __x86_64__
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        GetThreadContext(thread, &context);
        return context.LastExceptionToRip;
#else
        struct THREAD_BASIC_INFORMATION_t thread_info;
        if (NtQueryInformationThread_ptr(thread, 0, &thread_info, sizeof(thread_info), NULL) >= 0) {
            return get_tls_slot(thread_info.TebBaseAddress, exception_to_tls);
        }
#endif
    }
    return 0;
}

uintptr_t last_branch_exception_from(HANDLE thread) {
    if (last_branch_functional) {
#if _M_X64 || __x86_64__
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        GetThreadContext(thread, &context);
        return context.LastExceptionFromRip;
#else
        struct THREAD_BASIC_INFORMATION_t thread_info;
        if (NtQueryInformationThread_ptr(thread, 0, &thread_info, sizeof(thread_info), NULL) >= 0) {
            return get_tls_slot(thread_info.TebBaseAddress, exception_from_tls);
        }
#endif
    }
    return 0;
}

LastBranchPair_t last_branch_get_exceptions(HANDLE thread) {
    if (last_branch_functional) {
#if _M_X64 || __x86_64__
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        GetThreadContext(thread, &context);
        return {
            context.LastExceptionToRip,
            context.LastExceptionFromRip
        };
#else
        struct THREAD_BASIC_INFORMATION_t thread_info;
        if (NtQueryInformationThread_ptr(thread, 0, &thread_info, sizeof(thread_info), NULL) >= 0) {
            return {
                get_tls_slot(thread_info.TebBaseAddress, exception_to_tls),
                get_tls_slot(thread_info.TebBaseAddress, exception_from_tls)
            };
        }
#endif
    }
    return { 0, 0 };
}
