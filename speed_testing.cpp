#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <utility>
#include <tuple>

#include <Windows.h>

#include "zero/util.h"

#define NO_WOW64 0
#define OLD_WINDOWS_COMPAT 0

alignas(4096) uint8_t PAGE_BUFFER[32];

#define USE_CONTROLS 1
#define USE_CONTROLS_TO_ESTIMATE 0

#define CARE_ABOUT_AVX512 0

#define MEM_BARRIER __asm__ volatile ("":::"memory")

#define EXPORT_TESTS 0

#if EXPORT_TESTS
#define test_export test_export
#else
#define test_export static
#endif

#define TEST_USE_ASM 1

#if TEST_USE_ASM
#define test_inline inline
#else
#define test_inline gnu_noinline
#endif

#if !NO_WOW64
#define for_each_test_no_avx512(...) \
__VA_ARGS__(A) \
__VA_ARGS__(B) \
__VA_ARGS__(B2) \
__VA_ARGS__(C) \
__VA_ARGS__(D) \
__VA_ARGS__(E) \
__VA_ARGS__(H) \
__VA_ARGS__(H2) \
__VA_ARGS__(R1) \
__VA_ARGS__(R2) \
__VA_ARGS__(F) \
__VA_ARGS__(F2) \
__VA_ARGS__(F3) \
__VA_ARGS__(F4) \
__VA_ARGS__(F5) \
__VA_ARGS__(F6) \
__VA_ARGS__(F7) \
__VA_ARGS__(L) \
__VA_ARGS__(M) \
__VA_ARGS__(M2) \
__VA_ARGS__(G2) \
__VA_ARGS__(G) \
__VA_ARGS__(G3) \
__VA_ARGS__(O) \
__VA_ARGS__(O2) \
__VA_ARGS__(O3) \
__VA_ARGS__(Q) \
__VA_ARGS__(Q2) \
__VA_ARGS__(I) \
__VA_ARGS__(I2) \
__VA_ARGS__(J) \
__VA_ARGS__(K) \
__VA_ARGS__(N) \
__VA_ARGS__(P1) \
__VA_ARGS__(P2) \
__VA_ARGS__(P3) \
__VA_ARGS__(P4) \
__VA_ARGS__(P5)
#else
#define for_each_test_no_avx512(...) \
__VA_ARGS__(A) \
__VA_ARGS__(B) \
__VA_ARGS__(B2) \
__VA_ARGS__(C) \
__VA_ARGS__(D) \
__VA_ARGS__(E) \
__VA_ARGS__(H) \
__VA_ARGS__(H2) \
__VA_ARGS__(R1) \
__VA_ARGS__(R2) \
__VA_ARGS__(F2) \
__VA_ARGS__(F3) \
__VA_ARGS__(F4) \
__VA_ARGS__(F5) \
__VA_ARGS__(F6) \
__VA_ARGS__(F7) \
__VA_ARGS__(L) \
__VA_ARGS__(M) \
__VA_ARGS__(M2) \
__VA_ARGS__(G2) \
__VA_ARGS__(G) \
__VA_ARGS__(G3) \
__VA_ARGS__(O) \
__VA_ARGS__(O2) \
__VA_ARGS__(O3) \
__VA_ARGS__(Q) \
__VA_ARGS__(Q2) \
__VA_ARGS__(I) \
__VA_ARGS__(I2) \
__VA_ARGS__(J) \
__VA_ARGS__(K) \
__VA_ARGS__(N) \
__VA_ARGS__(P1) \
__VA_ARGS__(P2) \
__VA_ARGS__(P3) \
__VA_ARGS__(P4) \
__VA_ARGS__(P5)
#endif

#if CARE_ABOUT_AVX512
#define for_each_test_avx512(...) \

#else
#define for_each_test_avx512(...)
#endif

#define for_each_test(...) \
for_each_test_no_avx512(__VA_ARGS__) \
for_each_test_avx512(__VA_ARGS__)

#define for_each_test_control(...) \
__VA_ARGS__(V6) \
__VA_ARGS__(V7) \
__VA_ARGS__(V1) \
__VA_ARGS__(V9) \
__VA_ARGS__(V3) \
__VA_ARGS__(V2) \
__VA_ARGS__(V8) \
__VA_ARGS__(V4) \
__VA_ARGS__(V5)

// Variant V6: nothing
test_export test_inline void regcall test_V6() {
    __asm {}
}
#define args_V6
#define setup_V6
#define cleanup_V6

// Variant V7: nothing
test_export test_inline void regcall test_V7() {
    __asm {}
}
#define args_V7
#define setup_V7
#define cleanup_V7

// Variant V1: MOV r32, r32
test_export test_inline void regcall test_V1(uint32_t count) {
    uint32_t temp;
    __asm__ volatile (
        "MOV %[count], %[temp]"
        : asm_arg("=&r", temp)
        : asm_arg("r", count)
    );
}
#define args_V1 count
#define setup_V1
#define cleanup_V1

// Variant V9: MOV r32, m32
test_export test_inline void regcall test_V9() {
    uint32_t temp1, temp2;
    __asm__ volatile (
        "MOV %[temp1], %[temp2]"
        : asm_arg("=m", temp1), asm_arg("=r", temp2)
    );
}
#define args_V9
#define setup_V9
#define cleanup_V9

// Variant V3: MOV m32, r32
test_export test_inline void regcall test_V3() {
    uint32_t temp1, temp2;
    __asm__ volatile (
        "MOV %[temp2], %[temp1]"
        : asm_arg("=m", temp1), asm_arg("=r", temp2)
    );
}
#define args_V3
#define setup_V3
#define cleanup_V3

// Variant V2: ADD r32, imm
test_export test_inline void regcall test_V2() {
    uint32_t temp;
    __asm__ volatile (
        "ADD $67, %[temp]"
        : asm_arg("=r", temp)
    );
}
#define args_V2
#define setup_V2
#define cleanup_V2

// Variant V8: ADD m32, imm
test_export test_inline void regcall test_V8() {
    uint32_t temp;
    __asm__ volatile (
        "ADDL $67, %[temp]"
        : asm_arg("=m", temp)
    );
}
#define args_V8
#define setup_V8
#define cleanup_V8

// Variant V4: XOR r32, r32
test_export test_inline void regcall test_V4() {
    uint32_t temp;
    __asm__ volatile (
        "XOR %[temp], %[temp]"
        : asm_arg("=r", temp)
    );
}
#define args_V4
#define setup_V4
#define cleanup_V4

// Variant V5: NOP
test_export test_inline void regcall test_V5() {
    __asm__ volatile (
        "NOP"
        :
    );
}
#define args_V5
#define setup_V5
#define cleanup_V5

// Variant A: CPUID page 0
test_export test_inline void regcall test_A() {
    uint32_t temp = 0;
    __asm__ volatile (
        "cpuid"
        : "+a"(temp)
        :
        : clobber_list("ecx", "edx", "ebx")
    );
}
#define args_A
#define setup_A
#define cleanup_A

// Variant B: XCHG memory
test_export test_inline void regcall test_B() {
    uint32_t tempA, tempB;
    __asm__ volatile (
        "xchg %[tempB], %[tempA]"
        : asm_arg("=r", tempA), asm_arg("=m", tempB)
    );
}
#define args_B
#define setup_B
#define cleanup_B

// Variant B2: LOCK ADD m32, imm
test_export test_inline void regcall test_B2() {
    uint32_t temp;
    __asm__ volatile (
        "lock addl $67, %[temp]"
        : asm_arg("=m", temp)
    );
}
#define args_B2
#define setup_B2
#define cleanup_B2

// Variant C: FBSTP
test_export test_inline void regcall test_C(long double value) {
    long double buffer;
    __asm__ volatile (
        "FLD %%ST(0) \n"
        "FBSTP %[buffer] \n"
        : asm_arg("=m", buffer)
        : asm_arg("t", value)
    );
}
#define args_C rand_angle
#define setup_C \
    long double rand_angle; \
    __asm__ volatile ("FLDL %[RAND_ANGLE]": "=t"(rand_angle): asm_arg("m", RAND_ANGLE))
#define cleanup_C \
    __asm__ volatile ("FFREEP %%ST(0)"::"t"(rand_angle):clobber_list("st"))

// Variant D: FSINCOS
test_export test_inline void regcall test_D(long double value) {
    __asm__ volatile (
	    "FLD %%ST(0) \n"
	    "FSINCOS \n"
	    "FSTP %%ST(0) \n"
        "FSTP %%ST(0) \n"
	    :
	    : asm_arg("t", value)
    );
}
#define args_D rand_angle
#define setup_D \
    long double rand_angle; \
    __asm__ volatile ("FLDL %[RAND_ANGLE]": "=t"(rand_angle): asm_arg("m", RAND_ANGLE))
#define cleanup_D \
    __asm__ volatile ("FFREEP %%ST(0)"::"t"(rand_angle):clobber_list("st"))

// Variant E: FPATAN
test_export test_inline void regcall test_E(long double value) {
    __asm__ volatile (
        "FLD %%ST(0) \n"
        "FLD %%ST(0) \n"
        "FPATAN \n"
        "FSTP %%ST(0) \n"
        :
        : asm_arg("t", value)
    );
}
#define args_E rand_angle
#define setup_E \
    long double rand_angle; \
    __asm__ volatile ("FLDL %[RAND_ANGLE]": "=t"(rand_angle): asm_arg("m", RAND_ANGLE))
#define cleanup_E \
    __asm__ volatile ("FFREEP %%ST(0)"::"t"(rand_angle):clobber_list("st"))

naked void near_ret() {
    __asm RET
}

naked void far_ret() {
    __asm RETF
}

// Variant R1: CALL/RET
test_export test_inline void regcall test_R1() {
    near_ret();
}
#define args_R1
#define setup_R1
#define cleanup_R1

// Variant R2: JMP
test_export test_inline void regcall test_R2() {
    __asm__ volatile (
        "JMP 1f \n"
    "1: \n"
        :
    );
}
#define args_R2
#define setup_R2
#define cleanup_R2

// Variant F: CALL FAR/RETF (diff CS)
test_export test_inline void regcall test_F() {
    FAR_CALL_IMM(0x33, far_ret, );
}
#define args_F
#define setup_F
#define cleanup_F

// Variant F2: CALL FAR/RETF (same CS)
test_export test_inline void regcall test_F2() {
    //FAR_CALL_IMM(0x23, far_ret, );
    __asm__ volatile (
        //"lcall %[Seg],%[Addr]"
        ".byte 0x9A \n"
        ".int %c[Addr] \n"
    "self_modify_cs_call: \n"
        ".short 0x33 \n"
        :
        : [Addr]"i"(far_ret)
    );
}
#define args_F2
#define setup_F2
#define cleanup_F2

// Variant F3: JMP FAR (same CS)
test_export test_inline void regcall test_F3() {
    __asm__ volatile (
        ".byte 0xEA \n"
        ".int dummy_label \n"
    "self_modify_cs_jump: \n"
        ".short 0x23 \n"
    "dummy_label: \n"
    );
}
#define args_F3
#define setup_F3
#define cleanup_F3

extern "C" {
    extern uint16_t self_modify_cs_call asm("self_modify_cs_call");
    extern uint16_t self_modify_cs_jump asm("self_modify_cs_jump");
}

// Variant F4: MOV DS
test_export test_inline void regcall test_F4() {
    uint32_t temp;
    __asm__ volatile (
        "MOV %%DS, %[temp]\n"
        "MOV %[temp], %%DS \n"
        : asm_arg("=r", temp)
    );
}
#define args_F4
#define setup_F4
#define cleanup_F4

// Variant F5: MOV SS
test_export test_inline void regcall test_F5() {
    uint32_t temp;
    __asm__ volatile (
        "MOV %%SS, %[temp] \n"
        "MOV %[temp], %%SS \n"
        : asm_arg("=r", temp)
    );
}
#define args_F5
#define setup_F5
#define cleanup_F5

// Variant F6: PUSH/POP DS
test_export test_inline void regcall test_F6() {
    __asm__ volatile (
        "PUSH %%DS \n"
        "POP %%DS \n"
        :
    );
}
#define args_F6
#define setup_F6
#define cleanup_F6

// Variant F7: PUSH/POP SS
test_export test_inline void regcall test_F7() {
    __asm__ volatile (
        "PUSH %%SS \n"
        "POP %%SS \n"
        :
    );
}
#define args_F7
#define setup_F7
#define cleanup_F7

// Variant G: MASKMOVDQU/MOVDQA
test_export test_inline void regcall test_G(vec<uint32_t, 4> value) {
    alignas(16) char buffer[16];
    vec<uint32_t, 4> temp;
    __asm__ volatile (
        "MASKMOVDQU %[value], %[value] \n"
        "MOVDQA (%[buffer]), %[temp]"
        : asm_arg("=x", temp)
        : asm_arg("x", value), [buffer]"D"(&buffer)
    );
}
#define args_G xmm_rand
#define setup_G \
    vec<uint32_t, 4> xmm_rand = RAND_XMM; \
    __asm__ volatile("":"+x"(xmm_rand));
#define cleanup_G

// Variant G2: MOVDQA/MOVDQA
test_export test_inline void regcall test_G2(vec<uint32_t, 4> value) {
    alignas(16) char buffer[16];
    vec<uint32_t, 4> temp;
    __asm__ volatile (
        "MOVDQA %[value], %[buffer] \n"
        "MOVDQA %[buffer], %[temp]"
        : asm_arg("=x", temp), asm_arg("=m", buffer)
        : asm_arg("x", value)
    );
}
#define args_G2 xmm_rand
#define setup_G2 \
    vec<uint32_t, 4> xmm_rand = RAND_XMM; \
    __asm__ volatile("":"+x"(xmm_rand));
#define cleanup_G2

// Variant G3: MASKMOVDQU
test_export test_inline void regcall test_G3(vec<uint32_t, 4> value) {
    alignas(16) char buffer[16];
    vec<uint32_t, 4> temp;
    __asm__ volatile (
        "MASKMOVDQU %[value], %[value] \n"
        : asm_arg("=x", temp)
        : asm_arg("x", value), [buffer]"D"(&buffer)
    );
}
#define args_G3 xmm_rand
#define setup_G3 \
    vec<uint32_t, 4> xmm_rand = RAND_XMM; \
    __asm__ volatile("":"+x"(xmm_rand));
#define cleanup_G3

// Variant H: LOOP 32
test_export test_inline void regcall test_H() {
    size_t loop_count = 32;
    __asm__ volatile (
    "1: \n"
        "LOOP 1b \n"
        : "+c"(loop_count)
    );
}
#define args_H
#define setup_H
#define cleanup_H

// Variant H2: DEC/JNZ 32
test_export test_inline void regcall test_H2() {
    size_t loop_count = 32;
    __asm__ volatile (
    "1: \n"
        "DEC %[loop_count] \n"
        "JNZ 1b \n"
        : asm_arg("+r", loop_count)
    );
}
#define args_H2
#define setup_H2
#define cleanup_H2

// Variant I: ENTER 0, 0
test_export test_inline void regcall test_I() {
    __asm__ volatile (
        "ENTER $0, $0 \n"
        "LEAVE"
    );
}
#define args_I
#define setup_I
#define cleanup_I

// Variant I2: ENTER 0, 1
test_export test_inline void regcall test_I2() {
    __asm__ volatile (
        "ENTER $0, $1 \n"
        "LEAVE"
    );
}
#define args_I2
#define setup_I2
#define cleanup_I2

// Variant J: EMMS
test_export test_inline void regcall test_J() {
    __asm__ volatile (
        "EMMS"
        :
        :
        : clobber_list("st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)", "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7")
    );
}
#define args_J
#define setup_J
#define cleanup_J

// Variant K: BOUND
static const int32_t bounds[2] = { INT32_MIN, INT32_MAX };
test_export test_inline void regcall test_K(int32_t count) {
    __asm__ volatile (
        INTEL_SYNTAX_DIRECTIVE
        "BOUND %V[count], %[bounds]"
        :
        : asm_arg("r", count), asm_arg("m", bounds)
    );
}
#define args_K count
#define setup_K
#define cleanup_K

// Variant L: PUSHF/POPF
test_export test_inline void regcall test_L() {
    __asm__ volatile (
        "PUSHFL \n"
        "POPFL \n"
        :
    );
}
#define args_L
#define setup_L
#define cleanup_L

// Variant M: PUSHA/POPA
test_export test_inline void regcall test_M() {
    __asm__ volatile (
        "PUSHAL \n"
        "POPAL \n"
        :
    );
}
#define args_M
#define setup_M
#define cleanup_M

// Variant M2: PUSH/POP
test_export test_inline void regcall test_M2() {
    uint32_t temp;
    __asm__ volatile (
        "PUSH %[temp] \n"
        "POP %[temp] \n"
        : asm_arg("=r", temp)
    );
}
#define args_M2
#define setup_M2
#define cleanup_M2

// Variant N: RDTSC
test_export test_inline void regcall test_N() {
    __asm__ volatile (
        "RDTSC"
        :
        :
        : clobber_list("eax", "edx")
    );
}
#define args_N
#define setup_N
#define cleanup_N

// Variant O: RCR r32, 31
test_export test_inline void regcall test_O(uint32_t count) {
    uint32_t temp = count;
    __asm__ volatile (
        "RCR $31, %[temp]"
        : asm_arg("+r", temp)
    );
}
#define args_O count
#define setup_O
#define cleanup_O

// Variant O2: SHRD r32, r32, 31
test_export test_inline void regcall test_O2(uint32_t count) {
    uint32_t temp = count;
    __asm__ volatile (
        "SHRD $31, %[temp], %[temp]"
        : asm_arg("+r", temp)
    );
}
#define args_O2 count
#define setup_O2
#define cleanup_O2

// Variant O3: ROR r32, 31
test_export test_inline void regcall test_O3(uint32_t count) {
    uint32_t temp = count;
    __asm__ volatile (
        "ROR $31, %[temp]"
        : asm_arg("+r", temp)
    );
}
#define args_O3 count
#define setup_O3
#define cleanup_O3

// Variant P1: REP MOVSB
test_export test_inline void regcall test_P1() {
    uint8_t* ptr = PAGE_BUFFER;
    uint8_t* ptr2 = ptr;
    size_t count = sizeof(PAGE_BUFFER);
    __asm__ volatile (
        "rep movsb"
        : "+S"(ptr), "+D"(ptr2), "+c"(count)
    );
}
#define args_P1
#define setup_P1
#define cleanup_P1

// Variant P2: REP STOSB
test_export test_inline void regcall test_P2() {
    uint8_t* ptr = PAGE_BUFFER;
    size_t count = sizeof(PAGE_BUFFER);
    __asm__ volatile (
        "rep stosb"
        : "+D"(ptr), "+c"(count)
        : "a"(0)
    );
}
#define args_P2
#define setup_P2
#define cleanup_P2

// Variant P3: REPE SCASB
test_export test_inline void regcall test_P3() {
    uint8_t* ptr = PAGE_BUFFER;
    size_t count = sizeof(PAGE_BUFFER);
    __asm__ volatile (
        "repe scasb"
        : "+D"(ptr), "+c"(count)
        : "a"(0)
    );
}
#define args_P3
#define setup_P3
#define cleanup_P3

// Variant P4: REPE CMPSB
test_export test_inline void regcall test_P4() {
    uint8_t* ptr = PAGE_BUFFER;
    uint8_t* ptr2 = ptr;
    size_t count = sizeof(PAGE_BUFFER);
    __asm__ volatile (
        "repe cmpsb"
        : "+S"(ptr), "+D"(ptr2), "+c"(count)
    );
}
#define args_P4
#define setup_P4
#define cleanup_P4

// Variant P5: REP LODSB
test_export test_inline void regcall test_P5() {
    uint8_t* ptr = PAGE_BUFFER;
    size_t count = sizeof(PAGE_BUFFER);
    __asm__ volatile (
        "rep lodsb"
        : "+S"(ptr), "+c"(count)
        :
        : clobber_list("al")
    );
}
#define args_P5
#define setup_P5
#define cleanup_P5

// Variant Q: DIV
test_export test_inline void regcall test_Q() {
    uint64_t big_val = 0x7FFE8001;
    uint32_t div = 0x7FFFFFFF;
    __asm__ volatile (
        "DIV %[div]"
        : "+A"(big_val)
        : asm_arg("r", div)
    );
}
#define args_Q
#define setup_Q
#define cleanup_Q

// Variant Q2: IDIV
test_export test_inline void regcall test_Q2() {
    uint64_t big_val = 0x7FFE8001;
    uint32_t div = 0x7FFFFFFF;
    __asm__ volatile (
        "IDIV %[div]"
        : "+A"(big_val)
        : asm_arg("r", div)
    );
}
#define args_Q2
#define setup_Q2
#define cleanup_Q2

static forceinline uint64_t rdtsc_max_serialize(void) {
    uint64_t tick;
    __asm__ volatile (
        "MFENCE \n"
        "LFENCE \n"
        "RDTSC \n"
        "LFENCE \n"
        : "=A"(tick)
    );
    return tick;
}

#define TEST_COUNT 0x10000

#define force_reg_pattern "+r"(count)
#define test_funcs(variant) test_repeat(variant,MACRO_CAT(args_,variant))

#define test_setup(variant) MACRO_CAT(setup_,variant)
#define test_cleanup(variant) MACRO_CAT(cleanup_,variant)

#define FENCE_SPECULATION 1

#if FENCE_SPECULATION
#define speculative_fence "LFENCE \n"
#else
#define speculative_fence ""
#endif

#define SCHEDULER_TYPE_NONE 0
#define SCHEDULER_TYPE_SLEEP 1
#define SCHEDULER_TYPE_SWITCH 2

#define SCHEDULER_VARIANT SCHEDULER_TYPE_NONE

#if SCHEDULER_VARIANT == SCHEDULER_TYPE_NONE
#define test_sleep() do; while(0)
#elif SCHEDULER_VARIANT == SCHEDULER_TYPE_SLEEP
#define test_sleep() Sleep(0)
#elif SCHEDULER_VARIANT == SCHEDULER_TYPE_SWITCH
#define test_sleep() SwitchToThread()
#endif

#define TIMESTAMP_MEM_BEFORE 0
#define TIMESTAMP_MEM_AFTER 1
#define TIMESTAMP_MEM_AFTER_NTI 2

#define TIMESTAMP_TYPE TIMESTAMP_MEM_BEFORE

#if TIMESTAMP_TYPE == TIMESTAMP_MEM_BEFORE

#define start_timestamps(variant) \
uint64_t time; \
__asm__ volatile ( \
    /*wait_for_cache_touch*/ \
    "LFENCE \n" \
    "RDTSC \n" \
    "MOV %%EAX, %[L1] \n" \
    "MOV %%EDX, %[H1] \n" \
    "LFENCE \n" \
    : "=A"(time), [L1]"=m"(MACRO_CAT(time_,variant)), [H1]"=m"(((uint32_t*)&MACRO_CAT(time_,variant))[1]), force_reg_pattern \
)

#define write_timestamps(variant) \
__asm__ volatile ( \
    speculative_fence \
    "MFENCE \n" \
    "LFENCE \n" \
    "RDTSC \n" \
    "SUB %[L1], %%EAX \n" \
    "SBB %[H1], %%EDX \n" \
    "MOV %%EAX, %[L1] \n" \
    "MOV %%EDX, %[H1] \n" \
    "LFENCE \n" \
    : "=A"(time), [L1]"+m"(MACRO_CAT(time_,variant)), [H1]"+m"(((uint32_t*)&MACRO_CAT(time_,variant))[1]) \
)


#define make_test_timers(variant) \
static uint64_t MACRO_CAT(time_,variant);

#elif TIMESTAMP_TYPE == TIMESTAMP_MEM_AFTER

#define start_timestamps(variant) \
uint64_t time; \
__asm__ volatile ( \
    /*wait_for_cache_touch*/ \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    : "=A"(time), force_reg_pattern \
)

#define write_timestamps(variant) \
uint32_t low, high; \
__asm__ volatile ( \
    speculative_fence \
/*  ".ifnes " "\"" "%V[Ltime]" "\"" "," "\"" "eax" "\"" "\n" \
    "MOV %[Ltime], %[low] \n" \
    ".endif \n" \
    ".ifnes " "\"" "%V[Htime]" "\"" "," "\"" "edx" "\"" " \n" \
    "MOV %[Htime], %[high] \n" \
    ".endif \n" */\
    "MFENCE \n" \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    "MOV %[Ltime], %[L1] \n" \
    "MOV %[Htime], %[H1] \n" \
    "MOV %%EAX, %[L2] \n" \
    "MOV %%EDX, %[H2] \n" \
    : "=&a"(low), "=&d"(high), asm_arg("=r", low), asm_arg("=r", high), [L1]"=m"(MACRO_CAT(start_,variant)), [H1]"=m"(((uint32_t*)&MACRO_CAT(start_,variant))[1]), [L2]"=m"(MACRO_CAT(end_,variant)), [H2]"=m"(((uint32_t*)&MACRO_CAT(end_,variant))[1]) \
    : [Ltime]"r"((uint32_t)time), [Htime]"r"((uint32_t)(time >> 32)) \
)

#define make_test_timers(variant) \
static volatile uint64_t MACRO_CAT(start_,variant); \
static volatile uint64_t MACRO_CAT(end_,variant); \
static uint64_t MACRO_CAT(time_,variant);

#elif TIMESTAMP_TYPE == TIMESTAMP_MEM_AFTER_NTI

#define start_timestamps(variant) \
uint64_t time; \
__asm__ volatile ( \
    /*wait_for_cache_touch*/ \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    : "=A"(time), force_reg_pattern \
)

#define write_timestamps(variant) \
register uint32_t low asm("ebp"); \
uint32_t high; \
__asm__ volatile ( \
    speculative_fence \
    "MOV %%EAX, %[low] \n" \
    "MOV %%EDX, %[high] \n" \
    "MFENCE \n" \
    "LFENCE \n" \
    "RDTSC \n" \
    "LFENCE \n" \
    "MOVNTI %[low], %[L1] \n" \
    "MOVNTI %[high], %[H1] \n" \
    "MOVNTI %%EAX, %[L2] \n" \
    "MOVNTI %%EDX, %[H2] \n" \
    : "+A"(time), asm_arg("=r", low), asm_arg("=r", high), [L1]"=m"(MACRO_CAT(start_,variant)), [H1]"=m"(((uint32_t*)&MACRO_CAT(start_,variant))[1]), [L2]"=m"(MACRO_CAT(end_,variant)), [H2]"=m"(((uint32_t*)&MACRO_CAT(end_,variant))[1]) \
)

#define make_test_timers(variant) \
static volatile uint64_t MACRO_CAT(start_,variant); \
static volatile uint64_t MACRO_CAT(end_,variant); \
static uint64_t MACRO_CAT(time_,variant);

#endif


#define TEST_CONTROLS_FIRST 1

#if USE_CONTROLS && TEST_CONTROLS_FIRST
for_each_test_control(make_test_timers);
#endif
for_each_test(make_test_timers);
#if USE_CONTROLS && !TEST_CONTROLS_FIRST
for_each_test_control(make_test_timers);
#endif

#define test_repeat(variant,...) \
{ \
    test_sleep(); \
    size_t count = test_count; \
    test_setup(variant); \
    start_timestamps(variant); \
    nounroll do { \
        MACRO_CAT(test_,variant)(__VA_ARGS__); \
    } while (--count); \
    write_timestamps(variant); \
    test_cleanup(variant); \
}

gnu_noinline naked void fastcall deal_with_64_bit_stuff(
#if CARE_ABOUT_AVX512
    bool has_avx512
#endif
) {
    __asm__ volatile (
        INTEL_64_DIRECTIVE
#if CARE_ABOUT_AVX512
        "TEST ECX, ECX \n"
        "JZ 1f \n"
        "VPBROADCASTD ZMM0, ECX \n"
        "VPADDD ZMM0, ZMM0, ZMM0 \n"
    "1: \n"
#endif
        "VZEROUPPER \n"
        "RETF \n"
    );
}

#define CurrentProcessPseudoHandle() ((HANDLE)-1)
#define CurrentThreadPseudoHandle() ((HANDLE)-2)

#define RECORD_OVERALL_TEST_TIME 1

#define USE_ASM_JUMP 0

#define NEED_RAND_ANGLE 1
#define NEED_RAND_XMM 1
#define NEED_BIG_BUFFER 0

#if NEED_RAND_ANGLE
static double RAND_ANGLE;
#endif
#if NEED_RAND_XMM
static vec<uint32_t, 4> RAND_XMM;
#endif

#if RECORD_OVERALL_TEST_TIME
static union {
    struct {
        uint64_t test_start packed_field;
        uint64_t test_end packed_field;
        uint64_t qpc_freq packed_field;
        uint64_t qpc_start packed_field;
        uint64_t qpc_end packed_field;
    };
};
#endif

//gnu_noinline
forceinline
void fastcall run_test(uint32_t test_count) {
    char manufacturer[12];
    uint32_t eax, ebx, ecx, edx;
    get_cpuid(0, eax, ebx, ecx, edx);
    *(uint32_t*)&manufacturer[0] = ebx;
    *(uint32_t*)&manufacturer[4] = edx;
    *(uint32_t*)&manufacturer[8] = ecx;

#if CARE_ABOUT_AVX512
    bool can_test_masks = false;
    if (eax >= 7) {
        get_cpuid_ex(7, 0, eax, ebx, ecx, edx);
        can_test_masks = (ebx & 1 << 16) && (ebx & 1 << 31);
    }
#endif

    get_cpuid(1, eax, ebx, ecx, edx);

#if !NO_WOW64
    bool run_zeroall = edx & 1 << 28;
#endif

    uint8_t stepping = eax & 0xF;
    uint8_t model = eax >> 4 & 0xF;
    uint8_t extended_model = 0;
    uint32_t family = eax >> 8 & 0xF;
    switch (family) {
        case 15:
            family += eax >> 20 & 0xFF;
        case 6:
            extended_model = eax >> 16 & 0xF;
    }
    uint32_t cache_line_size = (edx & 1 << 19) ? (ebx >> 8 & 0xFF) * 8 : 64;

    if (
        *(uint32_t*)&manufacturer[0] == PackUInt32('G', 'e', 'n', 'u') &&
        *(uint32_t*)&manufacturer[4] == PackUInt32('i', 'n', 'e', 'I') &&
        *(uint32_t*)&manufacturer[8] == PackUInt32('n', 't', 'e', 'l')
    ) {
        printf(
            "Testing:    %llu iter%s\n"
            "Brand:      Intel\n"
            "Model:      %u.0x%X.0x%X.0x%X\n"
            "CacheLine:  %u\n"
#if CARE_ABOUT_AVX512
            "AVX512:     %s\n"
#endif
            , (test_count ? (uint64_t)test_count : 0x100000000u), test_count != 1 ? "s" : ""
            , family, extended_model, model, stepping
            , cache_line_size
#if CARE_ABOUT_AVX512
            , bool_str(can_test_masks)
#endif
        );
    } else if (
        *(uint32_t*)&manufacturer[0] == PackUInt32('A', 'u', 't', 'h') &&
        *(uint32_t*)&manufacturer[4] == PackUInt32('e', 'n', 't', 'i') &&
        *(uint32_t*)&manufacturer[8] == PackUInt32('c', 'A', 'M', 'D')
    ) {
        printf(
            "Testing:    %llu iter%s\n"
            "Brand:      AMD\n"
            "Model:      0x%X.0x%X.0x%X.0x%X\n"
            "CacheLine:  %u\n"
#if CARE_ABOUT_AVX512
            "AVX512:     %s\n"
#endif
            , (test_count ? (uint64_t)test_count : 0x100000000u), test_count != 1 ? "s" : ""
            , family, extended_model, model, stepping
            , cache_line_size
#if CARE_ABOUT_AVX512
            , bool_str(can_test_masks)
#endif
        );
    } else {
        printf(
            "Testing:    %llu iter%s\n"
            "Brand:      %.12s\n"
            "Model:      0x%X.0x%X.0x%X.0x%X\n"
            "CacheLine:  %u\n"
#if CARE_ABOUT_AVX512
            "AVX512:     %s\n"
#endif
            , (test_count ? (uint64_t)test_count : 0x100000000u), test_count != 1 ? "s" : ""
            , manufacturer
            , family, extended_model, model, stepping
            , cache_line_size
#if CARE_ABOUT_AVX512
            , bool_str(can_test_masks)
#endif
        );
    }

#if NO_WOW64
    DWORD old_protect;
    VirtualProtect(&self_modify_cs_call, 2, PAGE_EXECUTE_READWRITE, &old_protect);
    store_cs(self_modify_cs_call);
    VirtualProtect(&self_modify_cs_call, 2, old_protect, &old_protect);
    VirtualProtect(&self_modify_cs_jump, 2, PAGE_EXECUTE_READWRITE, &old_protect);
    store_cs(self_modify_cs_jump);
    VirtualProtect(&self_modify_cs_jump, 2, old_protect, &old_protect);
#endif

    SetPriorityClass(CurrentProcessPseudoHandle(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(CurrentThreadPseudoHandle(), THREAD_PRIORITY_TIME_CRITICAL);

    SwitchToThread();

#if !NO_WOW64
    if (run_zeroall) {
        decltype(&IsWow64Process) IsWow64ProcessVar;
        BOOL IsX64;
        if (
            (IsWow64ProcessVar = (decltype(&IsWow64Process))GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process")) &&
            IsWow64ProcessVar(GetCurrentProcess(), &IsX64) &&
            IsX64 == TRUE
        ) {
#if CARE_ABOUT_AVX512
            FAR_CALL_IMM(0x33, deal_with_64_bit_stuff,
                         , "c"((int)can_test_masks)
                         : clobber_list("xmm0")
            );
#else
            FAR_CALL_IMM(0x33, deal_with_64_bit_stuff,
                         ,
            );
#endif
        }
    }
#endif

#if !OLD_WINDOWS_COMPAT
    SetProcessAffinityMask(CurrentProcessPseudoHandle(), 1 << GetCurrentProcessorNumber());
#else
    static DWORD_PTR process_mask;
    static DWORD_PTR system_mask;
    GetProcessAffinityMask(CurrentProcessPseudoHandle(), &process_mask, &system_mask);
    SetProcessAffinityMask(CurrentProcessPseudoHandle(), process_mask & 1);
#endif

    Sleep(5000);

#if !NO_WOW64
    if (run_zeroall) {
        __asm__ volatile (
            "VZEROALL"
            :
        );
    }
#endif

#if NEED_BIG_BUFFER
    void* big_buffer = malloc(HALF_GIGABYTE);
#endif

#if NEED_RAND_ANGLE
    unsigned int rand_val;
    rand_s(&rand_val);
    RAND_ANGLE = ((double)(int32_t)rand_val / (double)INT32_MAX) * M_PI;
#endif
#if NEED_RAND_XMM
    rand_s(&((uint32_t*)&RAND_XMM)[0]);
    rand_s(&((uint32_t*)&RAND_XMM)[1]);
    rand_s(&((uint32_t*)&RAND_XMM)[2]);
    rand_s(&((uint32_t*)&RAND_XMM)[3]);
#endif

#if RECORD_OVERALL_TEST_TIME
    QueryPerformanceCounter((LARGE_INTEGER*)&qpc_start);
    test_start = rdtsc_max_serialize();
#endif

#if USE_CONTROLS && TEST_CONTROLS_FIRST
    for_each_test_control(test_funcs);
#endif
    for_each_test_no_avx512(test_funcs);

#if CARE_ABOUT_AVX512

#if USE_ASM_JUMP
    __asm__(
        "CMPB $0, %[can_test_masks] \n"
        "JE skip_mask_test"
        :
        : asm_arg("m", can_test_masks)
    );
#else
    if (expect(can_test_masks, true))
#endif
    {
        register uint32_t mask asm("ebp") = 0b111;
        __asm__ volatile (
            "KMOVW %[mask], %%k1 \n"
            :
            : asm_arg("r", mask)
        );

        for_each_test_avx512(test_funcs);
    }
#if USE_ASM_JUMP
    __asm__(
        "skip_mask_test:"
    );
#endif

#endif

#if USE_CONTROLS && !TEST_CONTROLS_FIRST
    for_each_test_control(test_funcs);
#endif

#if RECORD_OVERALL_TEST_TIME
    test_end = rdtsc_max_serialize();
#endif

    /*
#if NEED_RAND_ANGLE
    __asm__ volatile(
        ""
        : "+t"(rand_angle)
    );
#endif
    __asm__ volatile(
        ""
        : "+x"(xmm_rand)
    );
    */

#if RECORD_OVERALL_TEST_TIME
    QueryPerformanceCounter((LARGE_INTEGER*)&qpc_end);
    test_end -= test_start;
    qpc_end -= qpc_start;
    QueryPerformanceFrequency((LARGE_INTEGER*)&qpc_freq);
#endif

    uint64_t total = 0;

#if TIMESTAMP_TYPE != TIMESTAMP_MEM_BEFORE
#define calculate_time(variant) \
    total += (MACRO_CAT(time_,variant) = (MACRO_CAT(end_,variant) - MACRO_CAT(start_,variant)));
#define calculate_time_no_total(variant) \
    MACRO_CAT(time_,variant) = (MACRO_CAT(end_,variant) - MACRO_CAT(start_,variant));
#else
#define calculate_time(variant) \
    total += MACRO_CAT(time_,variant);
#define calculate_time_no_total(variant)
#endif

#if USE_CONTROLS && TEST_CONTROLS_FIRST
    for_each_test_control(calculate_time);
#endif
    for_each_test(calculate_time);
#if USE_CONTROLS && !TEST_CONTROLS_FIRST
    for_each_test_control(calculate_time);
#endif

#if USE_CONTROLS && USE_CONTROLS_TO_ESTIMATE
    double best_fit_slope = ((double)time_V4 - (double)time_V1) / 3.0;
    double best_fit_offset = ((double)time_V1 * 6.0 + (double)time_V2 * 3.0 - (double)time_V4 * 3.0) / 6.0;
    double ideal_estimate = best_fit_slope * 3.0 + best_fit_offset;
#define test_format_str(variant) \
        MACRO_CAT(desc_,variant) "%*llu" RT " (%.3f%%) {%+.3f%%}\n"
#define estimate_format_str(variant) \
        , -100.0 * (1.0 - ((double)MACRO_CAT(time_, variant) / ideal_estimate))


#if RECORD_OVERALL_TEST_TIME
#define print_test_time(variant) \
        , max_width, MACRO_CAT(time_,variant) \
        , time_width, qpc_us * ((double)MACRO_CAT(time_,variant) * total_double), (double)MACRO_CAT(time_,variant) * (100.0 * total_double) \
        , -100.0 * (1.0 - ((double)MACRO_CAT(time_,variant) / ideal_estimate))
#define print_time_estimate \
        , max_width, ideal_estimate \
        , time_width, qpc_us * ideal_estimate * total_double

#else
#define print_test_time(variant) \
        , max_width, MACRO_CAT(time_,variant) \
        , (double)MACRO_CAT(time_,variant) * (100.0 * total_double) \
        , -100.0 * (1.0 - ((double)MACRO_CAT(time_,variant) / ideal_estimate))
#define print_time_estimate \
        , max_width, ideal_estimate
#endif
#else
#define test_format_str(variant) \
        MACRO_CAT(desc_,variant) "%*llu" RT " (%.3f%%)\n"

#if RECORD_OVERALL_TEST_TIME
#define print_test_time(variant) \
        , max_width, MACRO_CAT(time_,variant) \
        , time_width, qpc_us * ((double)MACRO_CAT(time_,variant) * total_double), (double)MACRO_CAT(time_,variant) * (100.0 * total_double)
#else
#define print_test_time(variant) \
        , max_width, MACRO_CAT(time_,variant) \
        , (double)MACRO_CAT(time_,variant) * (100.0 * total_double)
#endif
#define print_time_estimate

#endif

#if RECORD_OVERALL_TEST_TIME
#define RT \
        " [%*.3fus]"

    int max_width = snprintf(NULL, 0, "%llu", test_end);
    double qpc_us = 1000000.0 * ((double)qpc_end / (double)qpc_freq);
    int time_width = snprintf(NULL, 0, "%.3f", qpc_us);
#else
#define RT \
        ""

    int max_width = snprintf(NULL, 0, "%llu", total);
#endif
    double total_double = 1.0 / (double)total;


#define desc_V6 "Nothing #1:              "
#define desc_V7 "Nothing #2:              "
#define desc_V1 "MOV r32, r32:            "
#define desc_V9 "MOV r32, m32:            "
#define desc_V3 "MOV m32, r32:            "
#define desc_V2 "ADD r32, imm:            "
#define desc_V8 "ADD m32, imm:            "
#define desc_V4 "XOR r32, r32:            "
#define desc_V5 "NOP:                     "

#define desc_A  "CPUID:                   "
#define desc_B  "XCHG r32, m32:           "
#define desc_B2 "LOCK ADD m32, imm:       "
#define desc_C  "FBSTP:                   "
#define desc_D  "FSINCOS:                 "
#define desc_E  "FPATAN:                  "
#define desc_H  "LOOP x32:                "
#define desc_H2 "DEC/JNZ x32:             "
#define desc_R1 "CALL/RET:                "
#define desc_R2 "JMP:                     "
#define desc_F  "CALL FAR/RETF (diff CS): "
#define desc_F2 "CALL FAR/RETF (same CS): "
#define desc_F3 "JMP FAR (same CS):       "
#define desc_F4 "MOV DS:                  "
#define desc_F5 "MOV SS:                  "
#define desc_F6 "PUSH/POP DS:             "
#define desc_F7 "PUSH/POP SS:             "
#define desc_L  "PUSHFD/POPFD:            "
#define desc_M  "PUSHAD/POPAD:            "
#define desc_M2 "PUSH/POP r32:            "
#define desc_G2 "MOVDQA/MOVDQA:           "
#define desc_G  "MASKMOVDQU/MOVDQA:       "
#define desc_G3 "MASKMOVDQU:              "
#define desc_O  "RCR r32, 31:             "
#define desc_O2 "SHRD r32, r32, 31:       "
#define desc_O3 "ROR r32, 31:             "
#define desc_O4 "SHR r32, 31:             "
#define desc_Q  "DIV:                     "
#define desc_Q2 "IDIV:                    "
#define desc_I  "ENTER 0, 0/LEAVE:        "
#define desc_I2 "ENTER 0, 1/LEAVE:        "
#define desc_J  "EMMS:                    "
#define desc_K  "BOUND:                   "
#define desc_N  "RDTSC:                   "
#define desc_P1 "REP MOVSB 32 bytes:      "
#define desc_P2 "REP STOSB 32 bytes:      "
#define desc_P3 "REP SCASB 32 bytes:      "
#define desc_P4 "REP CMPSB 32 bytes:      "
#define desc_P5 "REP LODSB 32 bytes:      "

    printf(
#if NEED_RAND_ANGLE || NEED_RAND_XMM
        "\n"
#if NEED_RAND_ANGLE
        "Rand angle for x87 Tests: %f\n"
#endif
#if NEED_RAND_XMM
        "Rand XMM for MASKMOVDQU:  %X%X%X%X\n"
#endif
        "\n"
#endif
#if RECORD_OVERALL_TEST_TIME
        "Realtime:                %llu [%.3fus] (+5s wait)\n"
        "Timestamp total:         %*llu [%*.3fus]\n"
#else
        "Timestamp total:         %llu\n"
#endif
#if USE_CONTROLS && TEST_CONTROLS_FIRST
        for_each_test_control(test_format_str)
#if USE_CONTROLS_TO_ESTIMATE
        "Ideal estimate:          %*.0f" RT "\n"
#endif
#endif
        for_each_test_no_avx512(test_format_str)
#if NEED_RAND_ANGLE
        , RAND_ANGLE
#endif
#if NEED_RAND_XMM
        , RAND_XMM[3], RAND_XMM[2], RAND_XMM[1], RAND_XMM[0]
#endif
#if RECORD_OVERALL_TEST_TIME
        , test_end, qpc_us
        , max_width, total, time_width, qpc_us * ((double)total / (double)test_end)
#else
        , total
#endif
#if USE_CONTROLS && TEST_CONTROLS_FIRST
        for_each_test_control(print_test_time)
#if USE_CONTROLS_TO_ESTIMATE
        print_time_estimate
#endif
#endif
        for_each_test_no_avx512(print_test_time)
    );

#if CARE_ABOUT_AVX512
    if (can_test_masks) {
        printf(
            ""
            for_each_test_avx512(test_format_str)
            for_each_test_avx512(print_test_time)
        );
    }
#endif

#if USE_CONTROLS && !TEST_CONTROLS_FIRST
    printf(
        for_each_test_control(test_format_str)
#if USE_CONTROLS_TO_ESTIMATE
        "Ideal estimate:          %*.0f" RT "\n"
#endif
        for_each_test_control(print_test_time)
#if USE_CONTROLS_TO_ESTIMATE
        print_time_estimate
#endif
    );
#endif

    exit(0);
}

int main(int argc, char* argv[]) {
    uint32_t test_count = TEST_COUNT;

    if (argc > 1) {
        test_count = strtoul(argv[1], NULL, 0);
    }

    run_test(test_count);
    return 0;
}