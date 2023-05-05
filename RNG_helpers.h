#pragma once

#include "zero/util.h"

#define INLINE_RNG

#ifndef INLINE_RNG

#define RngRet(type) \
dllexport noinline type noinline

#define RngFloatRet(type) \
[[clang::optnone, gnu::no_caller_saved_registers]] dllexport noinline type __attribute__((target("no-sse"))) thiscall

#else

#define RngRet(type) type

#define RngFloatRet(type) type

#endif

typedef struct Rng Rng;
struct Rng {
    uint16_t rng_value;
    uint32_t rng_index;

    /*RngRet(uint16_t) rand_ushort(void) asm("{[0x41E780]}") {
        uint16_t temp = (this->rng_value ^ 0x9630) - 0x6553;
        this->rng_value = ((temp & 0xC000) >> 14) + (temp << 2) & 0xFFFF;
        this->rng_index++;
        return this->rng_value;
    }*/

    // 0x41E780
    dllexport forceinline uint16_t rand_ushort(void) asm("{[codecave:RNG_rand_ushort]}") {
        ++this->rng_index;
        return this->rng_value = __rolw((this->rng_value ^ 0x9630) - 0x6553, 2);
    }

    // 0x41E7F0
    dllexport forceinline uint32_t rand_uint(void) asm("{[codecave:RNG_rand_uint]}") {
        return (this->rand_ushort() << 16) | this->rand_ushort();
    }

    // 0x41E820
    dllexport forceinline float rand_float(void) asm("{[codecave:RNG_rand_float]}") {
        float temp = (int64_t)this->rand_uint();
        return temp / (float)UINT32_MAX;
    }

    inline uint16_t rand_ushort_mod(uint16_t bound) {
        return expect(bound != 0, true) ? this->rand_ushort() % bound : 0;
    }

    inline uint32_t rand_uint_mod(uint32_t bound) {
        return expect(bound != 0, true) ? this->rand_uint() % bound : 0;
    }
};

#undef RngRet
#undef RngFloatRet

//static ExternalGlobal<Rng, 0x69D8F8> RNG;
extern "C" {
    extern Rng RNG asm("_RNG");
}