#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "windows.h"

#include "last_branch_enable.h"

LONG WINAPI just_dont_actually_crash(LPEXCEPTION_POINTERS lpEI) {
#if _M_X64 || __x86_64__
    lpEI->ContextRecord->Rip += 7;
#else
    lpEI->ContextRecord->Eip += 7;
#endif
    return EXCEPTION_CONTINUE_EXECUTION;
}

__declspec(noinline) void last_branch_testing() {
label_testA:
    __asm__ volatile(
        "jmp 0f \n"
        "int3 \n"
    "0: \n"
    );
label_testB:
    __asm__ volatile (
        "NOP \n"
        ".byte 0x8B,0x04,0x25,0,0,0,0" // MOV EAX, [0]
    );
    /*
    __asm {
        _emit 0x8B
        _emit 0x04
        _emit 0x25
        _emit 0
        _emit 0
        _emit 0
        _emit 0
    }
    */

    auto [last_exception_to, last_exception_from] = last_branch_get_exceptions();

    printf(
        //"BranchTo:   %llX\n"
        //"BranchFrom: %llX\n"
        "ExceptTo:   %zX\n"
        "ExceptFrom: %zX\n"
        "LabelTestA: %zX\n"
        "LabelTestB: %zX\n"
        //, last_branch_to
        //, last_branch_from
        , last_exception_to
        , last_exception_from
        , &&label_testA
        , &&label_testB
    );
}

int main() {
    SetProcessAffinityMask(GetCurrentProcess(), 1);
    Sleep(1000);
    AddVectoredExceptionHandler(0, just_dont_actually_crash);
    last_branch_tracking_hook();
    last_branch_tracking_start();
    last_branch_testing();
    return 0;
}