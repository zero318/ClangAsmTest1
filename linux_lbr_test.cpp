#define _GNU_SOURCE
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>
#include <sys/ucontext.h>

// idk why the gcc on my desktop doesn't support this
#define thread_local __thread

__attribute__((always_inline)) static inline ssize_t pread_direct(int fd, void *buf, size_t count, off_t offset) {
    ssize_t ret;
    register off_t offset_local asm("r10");
    offset_local = offset;
    __asm__ volatile (
        "syscall"
        : "=a"(ret)
        : "a"(17), "D"(fd), "S"(buf), "d"(count), "r"(offset_local)
    );
    return ret;
}
__attribute__((always_inline)) static inline ssize_t pwrite_direct(int fd, const void *buf, size_t count, off_t offset) {
    ssize_t ret;
    register off_t offset_local asm("r10");
    offset_local = offset;
    __asm__ volatile (
        "syscall"
        : "=a"(ret)
        : "a"(18), "D"(fd), "S"(buf), "d"(count), "r"(offset_local)
    );
    return ret;
}

__attribute__((always_inline)) static inline void get_cpuid(uint32_t page_num, uint32_t* eax_out, uint32_t* ebx_out, uint32_t* ecx_out, uint32_t* edx_out) {
    uint32_t eax_val;
    uint32_t ebx_val;
    uint32_t ecx_val;
    uint32_t edx_val;
    __asm__ volatile (
        "cpuid"
        : "=a"(eax_val), "=b"(ebx_val), "=c"(ecx_val), "=d"(edx_val)
        : "a"(page_num)
    );
    if (eax_out) *eax_out = eax_val;
    if (ebx_out) *ebx_out = ebx_val;
    if (ecx_out) *ecx_out = ecx_val;
    if (edx_out) *edx_out = edx_val;
}

thread_local int msr_fd;
thread_local uintptr_t branch_from_tls;
thread_local uintptr_t branch_to_tls;
thread_local uintptr_t exception_from_tls;
thread_local uintptr_t exception_to_tls;

extern void* TrapAddr;

void handle_trap_disable(int sig, siginfo_t* info, void* raw_context) {
    ucontext_t* context = (ucontext_t*)raw_context;
    if ((void*)context->uc_mcontext.gregs[REG_RIP] == &TrapAddr) {
        return;
    }
    // do whatever calls the previous handler...?
    __asm__("int3");
}

void handle_segfault_amd(int sig, siginfo_t* info, void* raw_context) {
    // Force a #DB to disable LBR
    __asm__ volatile (
        "int1\n"
    ".global TrapAddr \n"
    "TrapAddr: \n"
    );

    int fd = msr_fd;
    uint64_t val;
    pread_direct(fd, (void*)&val, 8, 0x1DB);
    branch_from_tls = val;
    pread_direct(fd, (void*)&val, 8, 0x1DC);
    branch_to_tls = val;
    pread_direct(fd, (void*)&val, 8, 0x1DD);
    exception_from_tls = val;
    pread_direct(fd, (void*)&val, 8, 0x1DE);
    exception_to_tls = val;

    // Re-enable LBR
    pread_direct(fd, (void*)&val, 8, 0x1D9);
    val |= 1;
    pwrite_direct(fd, (void*)&val, 8, 0x1D9);


    ucontext_t* context = (ucontext_t*)raw_context;
    context->uc_mcontext.gregs[REG_RIP] += 7;
}

void handle_segfault_intel(int sig, siginfo_t* info, void* raw_context) {
    int fd = msr_fd;
    uint64_t val;
    pread_direct(fd, (void*)&val, 8, 0x1DB);
    branch_from_tls = val;
    pread_direct(fd, (void*)&val, 8, 0x1DC);
    branch_to_tls = val;
    pread_direct(fd, (void*)&val, 8, 0x1DD);
    exception_from_tls = val;
    pread_direct(fd, (void*)&val, 8, 0x1DE);
    exception_to_tls = val;

    ucontext_t* context = (ucontext_t*)raw_context;
    context->uc_mcontext.gregs[REG_RIP] += 7;
}

int main() {
    unsigned int cpu;
    getcpu(&cpu, NULL);

    char msr_path[64] = "/dev/cpu/";
    sprintf(&msr_path[sizeof("/dev/cpu/") - 1], "%d/msr", cpu);
    int fd = open(msr_path, O_RDONLY);
    if (fd >= 0) {
        msr_fd = fd;

        struct sigaction sa = {};
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);

        uint32_t brand1;
        uint32_t brand2;
        uint32_t brand3;
        get_cpuid(0, NULL, &brand1, &brand3, &brand2);
        if (
            brand1 == 0x756E6547 ||
            brand2 == 0x49656E69 ||
            brand3 == 0x6C65746E
        ) {
            sa.sa_sigaction = &handle_segfault_intel;
        } else {
            sa.sa_sigaction = &handle_trap_disable;
            sigaction(SIGTRAP, &sa, NULL);
            sa.sa_sigaction = &handle_segfault_amd;
        }
        sigaction(SIGSEGV, &sa, NULL);

bs_label_testA:
        __asm__ volatile(
            "jmp 0f \n"
            "int3 \n"
        "0: \n"
        );
bs_label_testB:
        __asm__ volatile(
            "NOP \n"
            ".byte 0x8B,0x04,0x25,0,0,0,0" // MOV EAX, [0]
        );

        close(fd);
        printf(
            "BranchTo:   %zX\n"
            "BranchFrom: %zX\n"
            "ExceptTo:   %zX\n"
            "ExceptFrom: %zX\n"
            "LabelTestA: %zX\n"
            "LabelTestB: %zX\n"
            , branch_to_tls
            , branch_from_tls
            , exception_to_tls
            , exception_from_tls
            , (size_t)&&bs_label_testA
            , (size_t)&&bs_label_testB
        );
    } else {
        puts("Failed to open MSR file");
    }

    return 0;
}