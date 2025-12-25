#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

#include "windows.h"

#include "last_branch_enable.h"

// The DDK also defines these as macros, so they're likely to remain stable
#define CURRENT_PROCESS_HANDLE ((HANDLE)(intptr_t)-1)
#define CURRENT_THREAD_HANDLE ((HANDLE)(intptr_t)-2)

static const wchar_t NTDLL_NAME[] = L"ntdll.dll";
static bool last_branch_initialized = false;
static bool last_branch_functional = false;

struct TEB_dummy_t {
#if _M_X64 || __x86_64__
    unsigned char pad0[0x1480];
    uint64_t TlsSlots[64];
    unsigned char pad1[0x100];
    uint64_t* TlsExpansionSlots;
#else
    unsigned char pad0[0xE10];
    uint32_t TlsSlots[64];
    unsigned char pad1[0x84];
    uint32_t* TlsExpansionSlots;
#endif
};

inline static uintptr_t get_tls_slot(struct TEB_dummy_t* teb, uint32_t slot) {
    if (slot < 64u) {
        return teb->TlsSlots[slot];
    } else {
        return teb->TlsExpansionSlots[slot];
    }
}

struct THREAD_BASIC_INFORMATION_t {
    int32_t ExitStatus;
    struct TEB_dummy_t* TebBaseAddress;
    void* UniqueProcess;
    void* UniqueThread;
    size_t AffinityMask;
    int32_t Priority;
    int32_t BasePriority;
};

typedef LONG WINAPI NtQueryInformationThread_t(HANDLE ThreadHandle, int ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);

static uint32_t exception_to_tls = 0;
static uint32_t exception_from_tls = 0;
static NtQueryInformationThread_t* NtQueryInformationThread_ptr = NULL;

#if _M_X64 || __x86_64__
static PVOID vector_handle;
LONG WINAPI log_branch_records(LPEXCEPTION_POINTERS lpEI) {
    PCONTEXT context = lpEI->ContextRecord;
    LPVOID exception_to = (LPVOID)context->LastExceptionToRip;
    LPVOID exception_from = (LPVOID)context->LastExceptionFromRip;
    TlsSetValue(exception_to_tls, exception_to);
    TlsSetValue(exception_from_tls, exception_from);
    return EXCEPTION_CONTINUE_SEARCH;
}
#else
static const wchar_t WOW64_NAME[] = L"wow64.dll";
static const char PREPARE_EXCEPTION_NAME[] = "Wow64PrepareForException";
static const char PROTECT_MEMORY_NAME[] = "NtProtectVirtualMemory";

static uint64_t original_prepare_addr = 0;
static uint64_t nt_virtual_protect_addr = 0;
static uint64_t ntdll_pointer_addr = 0;

#define EMIT2(b1,b2) EB(b1) EB(b2)
#define EMIT3(b1,b2,b3) EMIT2(b1,b2) EB(b3)
#define EMIT4(b1,b2,b3,b4) EMIT3(b1,b2,b3) EB(b4)
#define EMIT5(b1,b2,b3,b4,b5) EMIT4(b1,b2,b3,b4) EB(b5)
#define EMIT6(b1,b2,b3,b4,b5,b6) EMIT5(b1,b2,b3,b4,b5) EB(b6)
#define EMIT7(b1,b2,b3,b4,b5,b6,b7) EMIT6(b1,b2,b3,b4,b5,b6) EB(b7)

#define FS_OVERRIDE EB(0x64)
#define GS_OVERRIDE EB(0x65)
#define DATASIZE EB(0x66)
#define ADDRSIZE EB(0x67)
#define REPNE EB(0xF2)
#define REX EB(0x40)
#define REX_B EB(0x41)
#define REX_X EB(0x42)
#define REX_XB EB(0x43)
#define REX_R EB(0x44)
#define REX_RB EB(0x45)
#define REX_RX EB(0x46)
#define REX_RXB EB(0x47)
#define REX_W EB(0x48)
#define REX_WB EB(0x49)
#define REX_WX EB(0x4A)
#define REX_WXB EB(0x4B)
#define REX_WR EB(0x4C)
#define REX_WRB EB(0x4D)
#define REX_WRX EB(0x4E)
#define REX_WRXB EB(0x4F)

#if __GNUC__ || __clang__
#define EB(b) ".byte " #b "\n"

#define eax " eax "
#define rax eax
#define ecx " ecx "
#define rcx ecx
#define cl " cl "
#define edx " edx "
#define rdx edx
#define ebx " ebx "
#define rbx ebx
#define esp " esp "
#define rsp esp
#define esi " esi "
#define rsi esi
#define edi " edi "
#define rdi edi
#define r8 eax
#define r8d eax
#define r9 ecx
#define r9d ecx
#define r10 edx
#define r10d edx
#define r11 ebx
#define r11d ebx

#define AR(r) "%%" r
#define IR(r) r
#define AI(i) "$" #i
#define II(i) #i
#define AO(a) #a
#define IO(a) "OFFSET " #a
#define AMOBIS(o,b,i,s) #o "(%%" b ",%%" i "," #s ")"
#define IMOBIS(o,b,i,s) "[" i "*" #s "+" b "+" #o "]"
#define AMOIS(o,i,s) #o "(,%%" i "," #s ")"
#define IMOIS(o,i,s) "[" i "*" #s "+" #o "]" 
#define AMOBI(o,b,i) #o "(%%" b ",%%" i ")"
#define IMOBI(o,b,i) "[" i "+" b "+" #o "]"
#define AMBIS(b,i,s) "(%%" b ",%%" i "," #s ")"
#define IMBIS(b,i,s) "[" i "*" #s "+" b "]"
#define AMOB(o,b) #o "(%%" b ")"
#define IMOB(o,b) "[" b "+" #o "]"
#define AMB(b) "(%%" b ")"
#define IMB(b) "[" b "]"
#define B(s,d) " {" A##s "," A##d "|" I##d "," I##s "}\n"
#define BBS(s,d) " {" A##s "," A##d "|" I##d ",BYTE PTR " I##s "}\n"
#define BBD(s,d) " {" A##s "," A##d "|BYTE PTR " I##d "," I##s "}\n"
#define BWS(s,d) " {" A##s "," A##d "|" I##d ",WORD PTR " I##s "}\n"
#define BWD(s,d) " {" A##s "," A##d "|WORD PTR " I##d "," I##s "}\n"
#define BLS(s,d) " {" A##s "," A##d "|" I##d ",DWORD PTR " I##s "}\n"
#define BLD(s,d) " {" A##s "," A##d "|DWORD PTR " I##d "," I##s "}\n"
#define BQS BLS
#define BQD BLD

#define mov_absmem_eax(addr) EMIT3(0x8B,0x04,0x25)".int " #addr "\n"
#define mov_eax_absmem(addr) EMIT3(0x89,0x04,0x25)".int " #addr "\n"
#define mov_absmem_rax(addr) REX_W mov_absmem_eax(addr)
#define mov_rax_absmem(addr) REX_W mov_eax_absmem(addr)
#define mov_absmem_ecx(addr) EMIT3(0x8B,0x0C,0x25)".int " #addr "\n"
#define mov_ecx_absmem(addr) EMIT3(0x89,0x0C,0x25)".int " #addr "\n"
#define mov_absmem_esi(addr) EMIT3(0x8B,0x34,0x25)".int " #addr "\n"
#define mov_esi_absmem(addr) EMIT3(0x89,0x0C,0x25)".int " #addr "\n"
#define mov_absmem_rsi(addr) REX_W mov_absmem_esi(addr)
#define mov_rsi_absmem(addr) REX_W mov_esi_absmem(addr)
#define mov_absmem_edi(addr) EMIT3(0x8B,0x3C,0x25)".int " #addr "\n"
#define mov_edi_absmem(addr) EMIT3(0x89,0x3C,0x25)".int " #addr "\n"
#define mov_absmem_rdi(addr) REX_W mov_absmem_edi(addr)
#define mov_rdi_absmem(addr) REX_W mov_edi_absmem(addr)
#define mov_absmem_r9d(addr) REX_R mov_absmem_ecx(addr)
#define mov_r9d_absmem(addr) REX_R mov_ecx_absmem(addr)
#define mov_absmem_r9(addr) REX_WR mov_absmem_ecx(addr)
#define mov_r9_absmem(addr) REX_WR mov_ecx_absmem(addr)
#define jmp_absmem(addr) EMIT3(0xFF,0x24,0x25)".int " #addr "\n"
#define mov_neg1_rcx REX_W EMIT2(0xC7,0xC1)".int -1\n"
#define call_x64_abs(addr) EB(0x9A) ".int " #addr "\n" EMIT2(0x33,0x0)

__attribute__((naked)) static void prepare_for_exception_hook() {
    __asm__ volatile(
        mov_absmem_eax(%c[except_to_slot])
        REX_R "mov" B(MOB(0x4C0,rdx), R(r8d))
        "cmp" B(I(0x40), R(eax))
        "jnb 1f \n"
        FS_OVERRIDE REX_R "mov" B(R(r8d), MOIS(0xE10,rax,4))
        "jmp 2f\n"
    "1:\n"
        FS_OVERRIDE mov_absmem_r9d(0xF94)
        REX_RB "mov" B(MOBIS(-0x100,r9,rax,4), R(r8d))
    "2:\n"
        mov_absmem_eax(%c[except_from_slot])
        REX_R "mov" B(MOB(0x4C8,rdx), R(r8d))
        "cmp" B(I(0x40), R(eax))
        "jnb 1f\n"
        FS_OVERRIDE REX_R "mov" B(R(r8d), MOIS(0xE10,rax,4))
        "jmp 2f\n"
    "1:\n"
        FS_OVERRIDE mov_absmem_r9d(0xF94)
        REX_RB "mov" B(MOBIS(-0x100,r9,rax,4), R(r8d))
    "2:\n"
        jmp_absmem(%c[func_ptr])
#if !__clang__
        "int3\n"
#endif
        :
        :
        [except_to_slot]"i"(&exception_to_tls),
        [except_from_slot]"i"(&exception_from_tls),
        [func_ptr]"i"(&original_prepare_addr)
    );
}
__attribute__((naked)) static void initialize_wow_exception_hooks() {
    __asm__ volatile (
        GS_OVERRIDE mov_absmem_rax(0x60)
        REX_WR "mov" B(MOB(0x18,rax), R(r9))
        REX_WB "mov" B(MOB(0x10,r9), R(rdx))
        REX_WB "add" B(I(0x10), R(r9))
        "xor" B(R(eax), R(eax))
        REX_RB "xor" B(R(r8d), R(r8d))
        REX_RB "xor" B(R(r11d), R(r11d))
        "jmp 1f\n"
        "int3\n"
    "3:\n"
        REX_W "mov" B(MOB(0x60,rdx), R(rdi))
        "mov" B(I(18), R(ecx))
        "mov" B(O(%[ntdll_name]), R(esi))
        "repe cmpsb\n"
        "jne 3f\n"
        REX_WR "mov" B(MOB(0x30,rdx), R(r8))
        REX_W "test" B(R(rax), R(rax))
        "jnz 4f\n"
    ".nops 5\n"
    "2:\n"
        REX_W "mov" B(MB(rdx), R(rdx))
        REX_WR "cmp" B(R(r9), R(rdx))
        "je 9f\n"
    "1:\n"
        "cmp{w|}" BWD(I(18), MOB(0x58,rdx))
        "jne 2b\n"
        REX_WRB "test" B(R(r8), R(r8))
        "jz 3b\n"
    "3:\n"
        REX_W "test" B(R(rax), R(rax))
        "jnz 2b\n"
        REX_W "mov" B(MOB(0x60,rdx), R(rdi))
        "mov" B(I(18), R(ecx))
        "mov" B(O(%[wow64_name]), R(esi))
        "repe cmpsb\n"
        "jne 2b\n"
        REX_W "mov" B(MOB(0x30,rdx), R(rax))
        REX_WRB "test" B(R(r8), R(r8))
        "jz 2b\n"
    "4:\n"
        "mov" B(MOB(0x3C,rax), R(ecx))
        REX_R "mov" B(MOBI(0x88,rcx,rax), R(r9d))
        REX_B "mov" B(MOBI(0x18,r9,rax), R(edx))
        REX_RB "mov" B(MOBI(0x20,r9,rax), R(r10d))
        REX_WB EMIT2(0x01,0xC2) // add %%rax, %%r10
    "1:\n"
        "sub" B(I(1), R(edx))
        "jb 9f\n"
        REX_B "mov" B(MBIS(r10,rdx,4), R(edi))
        REX_W "add" B(R(rax), R(rdi))
        "mov" B(I(25), R(ecx))
        "mov" B(O(%[prepare_exception_name]), R(esi))
        "repe cmpsb\n"
        "jne 1b\n"
        REX_B "mov" B(MOBI(0x1C,r9,rax), R(ecx))
        REX_B "mov" B(MOBI(0x24,r9,rax), R(esi))
        REX_W "add" B(R(rax), R(rsi))
        "movz{w|x}" BWS(MBIS(rsi,rdx,2), R(edx))
        REX_W "add" B(R(rax), R(rcx))
        "mov" B(MBIS(rcx,rdx,4), R(edi))
        REX_W "add" B(R(rdi), R(rax))
        REX_B "mov" B(MOB(0x3C,r8), R(edx))
        REX_RX "mov" B(MOBI(0x88,rdx,r8), R(r9d))
        REX_WR EMIT2(0x01,0xC2) // add %%r8, %%rdx
        REX_RXB "mov" B(MOBI(0x18,r9,r8), R(r11d))
        REX_RXB "mov" B(MOBI(0x20,r9,r8), R(r10d))
        REX_WRB "add" B(R(r8), R(r10))
    ".nops 6\n"
    ".nops 6\n"
    "1:\n"
        REX_B "sub" B(I(1), R(r11d))
        "jb 9f\n"
        REX_XB "mov" B(MBIS(r10,r11,4), R(edi))
        REX_WR EMIT2(0x01,0xC7) // add %%r8, %%rdi
        "mov" B(I(23), R(ecx))
        "mov" B(O(%[protect_memory_name]), R(esi))
        "repe cmpsb\n"
        "jne 1b\n"
        REX_XB "mov" B(MOBI(0x1C,r9,r8), R(ecx))
        REX_XB "mov" B(MOBI(0x24,r9,r8), R(edi))
        REX_WR EMIT2(0x01,0xC7) // add %%r8, %%rdi
        REX_X "movz{w|x}" BWS(MBIS(rdi,r11,2), R(esi))
        REX_WR EMIT2(0x01,0xC1) // add %%r8, %%rcx
        REX_R "mov" B(MBIS(rcx,rsi,4), R(r9d))
        REX_WRB "add" B(R(r8), R(r9))
        "movz{w|x}" BWS(MOB(0x14,rdx), R(esi))
        REX_W "add" B(R(rdx), R(rsi))
        "movz{w|x}" BWS(MOB(0x6,rdx), R(edx))
        REX_RB "xor" B(R(r11d), R(r11d))
        "jmp 1f\n"
        "int3\nint3\nint3\nint3\nint3\n"
    "2:\n"
        REX_W "add" B(I(0x28), R(rsi))
        EMIT2(0xFF,0xCA) // dec %%edx
        "jz 9f\n"
    "1:\n"
        "movz{b|x}" BBS(MOB(0x3F,rsi), R(ecx))
        "and" B(I(0x60), R(cl))
        "cmp" B(I(0x40), R(cl))
        "jne 2b\n"
        "mov" B(MOB(0x20,rsi), R(ecx))
        "mov" B(MOB(0x24,rsi), R(edi))
        REX_WR EMIT2(0x01,0xC7) // add %%r8, %%rdi
        "shr" B(I(3), R(ecx))
        REPNE REX_W "scas{l|d}\n"
        "jne 2b\n"
        mov_rax_absmem(%c[prepare_addr])
        mov_r9_absmem(%c[protect_addr])
        REX_W "add" B(I(-8), R(rdi))
        mov_rdi_absmem(%c[func_ptr_addr])
        REX_B "mov" B(I(1), R(r11d))
    "9:\n"
        REX_R "mov" B(R(r11d), R(eax))
        "lret\n"
#if !__clang__
        "int3\n"
#endif
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
        call_x64_abs(%c[Addr])
        : "=a"(ret), "=m"(original_prepare_addr), "=m"(nt_virtual_protect_addr), "=m"(ntdll_pointer_addr)
        : [Addr]"i"(&initialize_wow_exception_hooks)
        : "ecx", "edx", "esi", "edi", "flags"
    );
    return ret;
}
__attribute__((naked)) static void hook_prepare_pointer() {
    __asm__ volatile(
        "mov" B(R(esp), R(ebx))
        "sub" B(I(0x40), R(esp))
        "and" B(I(-0x10), R(esp))
        mov_absmem_rdi(%c[func_ptr_addr])
        REX_W "mov" B(R(rdi), MOB(0x38,rsp))
        REX_W "mov{l|}" BQD(I(8), MOB(0x30,rsp))
        mov_absmem_rsi(%c[protect_addr])
        "lea" B(MOB(0x2C,rsp), R(eax))
        REX_W "mov" B(R(rax), MOB(0x20,rsp))
        "lea" B(MOB(0x38,rsp), R(edx))
        REX_R "lea" B(MOB(0x30,rsp), R(r8d))
        mov_neg1_rcx
        REX_B "mov" B(I(4), R(r9d))
        "call{ *%%|} " rsi "\n"
        "test" B(R(eax), R(eax))
        "js 1f\n"
        "mov" B(O(%[new_func]), R(eax))
        REX_W "xchg" B(R(rax), MB(rdi))
        REX_R "mov" B(MOB(0x2C,rsp), R(r9d))
        "lea" B(MOB(0x2C,rsp), R(eax))
        REX_W "mov" B(R(rax), MOB(0x20,rsp))
        "lea" B(MOB(0x30,rsp), R(edx))
        REX_R "lea" B(MOB(0x30,rsp), R(r8d))
        mov_neg1_rcx
        "call{ *%%|} " rsi "\n"
        "xor" B(R(eax), R(eax))
    "1:\n"
        "mov" B(R(ebx), R(esp))
        "lret\n"
#if !__clang__
        "int3\n"
#endif
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
        call_x64_abs(%c[Addr])
        : "=a"(ret)
        : [Addr]"i"(&hook_prepare_pointer)
        : "ecx", "edx", "ebx", "esi", "edi", "flags"
    );
    return ret;
}
__attribute__((naked)) static void unhook_prepare_pointer() {
    __asm__ volatile(
        "mov" B(R(esp), R(ebx))
        "sub" B(I(0x40), R(esp))
        "and" B(I(-0x10), R(esp))
        mov_absmem_rdi(%c[func_ptr_addr])
        REX_W "mov" B(R(rdi), MOB(0x38,rsp))
        REX_W "mov{l|}" BQD(I(8), MOB(0x30,rsp))
        mov_absmem_rsi(%c[protect_addr])
        "lea" B(MOB(0x2C,rsp), R(eax))
        REX_W "mov" B(R(rax), MOB(0x20,rsp))
        "lea" B(MOB(0x38,rsp), R(edx))
        REX_R "lea" B(MOB(0x30,rsp), R(r8d))
        mov_neg1_rcx
        REX_B "mov" B(I(4), R(r9d))
        "call{ *%%|} " rsi "\n"
        "test" B(R(eax), R(eax))
        "js 1f\n"
        mov_absmem_rax(%c[original_func])
        REX_W "xchg" B(R(rax), MB(rdi))
        REX_R "mov" B(MOB(0x2C,rsp), R(r9d))
        "lea" B(MOB(0x2C,rsp), R(eax))
        REX_W "mov" B(R(rax), MOB(0x20,rsp))
        "lea" B(MOB(0x38,rsp), R(edx))
        REX_R "lea" B(MOB(0x30,rsp), R(r8d))
        mov_neg1_rcx
        "call{ *%%|} " rsi "\n"
        "xor" B(R(eax), R(eax))
    "1:\n"
        "mov" B(R(ebx), R(esp))
        "lret\n"
#if !__clang__
        "int3\n"
#endif
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
        call_x64_abs(%c[Addr])
        : "=a"(ret)
        : [Addr]"i"(&unhook_prepare_pointer)
        : "ecx", "edx", "ebx", "esi", "edi", "flags"
    );
    return ret;
}
#elif _MSC_VER
struct FarPointer {
    void* addr;
    uint16_t segment;
};
#define EB(b) __asm _emit b
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
        FS_OVERRIDE REX_R EMIT7(0x8B,0x0C,0x25,0x94,0x0F,0x00,0x00) // MOV R9D, DWORD PTR [0xF94]
        REX_RB MOV DWORD PTR [RAX*4+R9-0x100], R8D
    tls_except_from:
        ADDRSIZE MOV EAX, DWORD PTR [exception_from_tls]
        REX_R MOV R8D, DWORD PTR [RDX+0x4C8]
        CMP EAX, 64
        JNB ext_tls_except_from
        FS_OVERRIDE REX_R MOV DWORD PTR [RAX*4+0xE10], R8D
        JMP call_original_prepare
    ext_tls_except_from:
        FS_OVERRIDE REX_R EMIT7(0x8B,0x0C,0x25,0x94,0x0F,0x00,0x00) // MOV R9D, DWORD PTR [0xF94]
        REX_RB MOV DWORD PTR [RAX*4+R9-0x100], R8D
    call_original_prepare:
        ADDRSIZE REX_W MOV RAX, QWORD PTR [original_prepare_addr]
        JMP RAX
    }
}
__declspec(naked) static void initialize_wow_exception_hooks() {
    __asm {
        GS_OVERRIDE REX_W EMIT7(0x8B,0x04,0x25,0x60,0x00,0x00,0x00) // MOV RAX, QWORD PTR GS:[0x60]
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
        EMIT5(0x0F,0x1F,0x44,0x00,0x00) // NOP
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
        REX_W MOV RAX, QWORD PTR [RDX+0x30]
        REX_WRB TEST R8, R8
        JZ next_module
    both_modules_found:
        MOV ECX, DWORD PTR [RAX+0x3C]
        REX_R EMIT7(0x8B,0x8C,0x01,0x88,0x00,0x00,0x00) // MOV R9D, DWORD PTR [RAX*1+RCX+0x88]
        REX_B EMIT4(0x8B,0x54,0x01,0x18) // MOV EDX, DWORD PTR [RAX*1+R9+0x18]
        REX_RB EMIT4(0x8B,0x54,0x01,0x20) // MOV R10D, DWORD PTR [RAX*1+R9+0x20]
        REX_WB EMIT2(0x01,0xC2) // ADD R10, RAX
    next_wow64_export:
        SUB EDX, 1
        JC initialize_hook_fail
        REX_B EMIT3(0x8B,0x3C,0x92) // MOV EDI, DWORD PTR [RDX*4+R10]
        REX_W ADD RDI, RAX
        MOV ECX, 25
        MOV ESI, OFFSET PREPARE_EXCEPTION_NAME
        REPE CMPSB
        JNE next_wow64_export
        REX_B EMIT4(0x8B,0x4C,0x01,0x1C) // MOV ECX, DWORD PTR [RAX*1+R9+0x1C]
        REX_B EMIT4(0x8B,0x74,0x01,0x24) // MOV ESI, DWORD PTR [RAX*1+R9+0x1C]
        REX_W ADD RSI, RAX
        MOVZX EDX, WORD PTR [RDX*2+RSI]
        REX_W ADD RCX, RAX
        MOV EDI, DWORD PTR [RDX*4+RCX]
        REX_W ADD RAX, RDI
        REX_B MOV EDX, DWORD PTR [R8+0x3C]
        REX_RX EMIT7(0x8B,0x8C,0x02,0x88,0x00,0x00,0x00) // MOV R9D, DWORD PTR [R8*1+RDX+0x88]
        REX_WB EMIT2(0x01,0xC2) // ADD RDX, R8
        REX_RXB EMIT4(0x8B,0x5C,0x01,0x18) // MOV R11D, DWORD PTR [R8*1+R9+0x18]
        REX_RXB EMIT4(0x8B,0x54,0x01,0x20) // MOV R10D, DWORD PTR [R8*1+R9+0x20]
        REX_WRB ADD R10, R8
        EMIT6(0x66,0x0F,0x1F,0x44,0x00,0x00) // NOP
        EMIT6(0x66,0x0F,0x1F,0x44,0x00,0x00) // NOP
    next_ntdll_export:
        REX_B SUB R11D, 1
        JC initialize_hook_fail
        REX_XB MOV EDI, DWORD PTR [R11*4+R10]
        REX_WR EMIT2(0x01,0xC7) // ADD RDI, R8
        MOV ECX, 23
        MOV ESI, OFFSET PROTECT_MEMORY_NAME
        REPE CMPSB
        JNE next_ntdll_export
        REX_XB EMIT4(0x8B,0x4C,0x01,0x1C) // MOV ECX, DWORD PTR [R8*1+R9+0x1C]
        REX_XB EMIT4(0x8B,0x7C,0x01,0x24) // MOV EDI, DWORD PTR [R8*1+R9+0x24]
        REX_WR EMIT2(0x01,0xC7) // ADD RDI, R8
        REX_X MOVZX ESI, WORD PTR [R11*2+RDI]
        REX_WB EMIT2(0x01,0xC1) // ADD RCX, R8
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
        EMIT2(0xFF,0xCA) // DEC EDX
        JZ initialize_hook_fail
    start_section_loop:
        MOVZX ECX, BYTE PTR [RSI+0x3F]
        AND CL, 0x60
        CMP CL, 0x40
        JNE next_section
        MOV ECX, DWORD PTR [RSI+0x20]
        MOV EDI, DWORD PTR [RSI+0x24]
        REX_WB ADD RDI, R8
        SHR ECX, 3
        REPNE REX_W SCASD
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
        REX_W EMIT6(0xC7,0xC1,0xFF,0xFF,0xFF,0xFF) // MOV RCX, -1
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
        REX_W EMIT6(0xC7,0xC1,0xFF,0xFF,0xFF,0xFF) // MOV RCX, -1
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
        REX_W EMIT6(0xC7,0xC1,0xFF,0xFF,0xFF,0xFF) // MOV RCX, -1
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
        REX_W EMIT6(0xC7,0xC1,0xFF,0xFF,0xFF,0xFF) // MOV RCX, -1
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
            IsWow64Process_ptr(CURRENT_PROCESS_HANDLE, &is_wow64) &&
            is_wow64
        )
#endif
        {
            NtQueryInformationThread_t* NtQueryInformationThread_temp = (NtQueryInformationThread_t*)GetProcAddress(ntdll, "NtQueryInformationThread");
            if (NtQueryInformationThread_temp) {
                NtQueryInformationThread_ptr = NtQueryInformationThread_temp;
                uint32_t exception_to_tls_temp = TlsAlloc();
                if (exception_to_tls_temp != TLS_OUT_OF_INDEXES) {
                    uint32_t exception_from_tls_temp = TlsAlloc();
                    if (exception_from_tls_temp != TLS_OUT_OF_INDEXES) {
#if !_M_X64 && !__x86_64__
                        if (initialize_wow_exception_hooks_thunk() > 0)
#endif
                        {
                            exception_to_tls = exception_to_tls_temp;
                            exception_from_tls = exception_from_tls_temp;
                            last_branch_functional = true;
                            return true;
                        }
#if !_M_X64 && !__x86_64__
                        TlsFree(exception_from_tls_temp);
#endif
                    }
                    TlsFree(exception_to_tls_temp);
                }
            }
        }
    }
    return false;
}

static inline bool hook_exceptions() {
#if _M_X64 || __x86_64__
    PVOID handle = AddVectoredExceptionHandler(1, log_branch_records);
    vector_handle = handle;
    return handle != NULL;
#else
    return hook_prepare_pointer_thunk() >= 0;
#endif
}

bool last_branch_tracking_hook() {
    return initialize_last_branch_tracking() &&
           hook_exceptions();
}

static inline bool unhook_exceptions() {
#if _M_X64 || __x86_64__
    PVOID handle = vector_handle;
    vector_handle = NULL;
    return RemoveVectoredExceptionHandler(handle) != 0;
#else
    return hook_prepare_pointer_thunk() >= 0;
#endif
}

bool last_branch_tracking_unhook() {
    return last_branch_functional &&
           unhook_exceptions();
}

bool last_branch_tracking_start(HANDLE thread) {
    if (last_branch_functional) {
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        GetThreadContext(thread, &context);
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        context.Dr7 |= 0x100;
        SetThreadContext(thread, &context);
        return true;
    }
    return false;
}

bool last_branch_tracking_stop(HANDLE thread) {
    if (last_branch_functional) {
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        GetThreadContext(thread, &context);
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        context.Dr7 &= ~0x100;
        SetThreadContext(thread, &context);
        return true;
    }
    return false;
}

uintptr_t last_branch_exception_to(HANDLE thread) {
    if (last_branch_functional) {
        struct THREAD_BASIC_INFORMATION_t thread_info;
        if (NtQueryInformationThread_ptr(thread, 0, &thread_info, sizeof(thread_info), NULL) >= 0) {
            return get_tls_slot(thread_info.TebBaseAddress, exception_to_tls);
        }
    }
    return 0;
}

uintptr_t last_branch_exception_from(HANDLE thread) {
    if (last_branch_functional) {
        struct THREAD_BASIC_INFORMATION_t thread_info;
        if (NtQueryInformationThread_ptr(thread, 0, &thread_info, sizeof(thread_info), NULL) >= 0) {
            return get_tls_slot(thread_info.TebBaseAddress, exception_from_tls);
        }
    }
    return 0;
}

LastBranchPair_t last_branch_get_exceptions(HANDLE thread) {
    if (last_branch_functional) {
        struct THREAD_BASIC_INFORMATION_t thread_info;
        if (NtQueryInformationThread_ptr(thread, 0, &thread_info, sizeof(thread_info), NULL) >= 0) {
            return {
                get_tls_slot(thread_info.TebBaseAddress, exception_to_tls),
                get_tls_slot(thread_info.TebBaseAddress, exception_from_tls)
            };
        }
    }
    return { 0, 0 };
}
