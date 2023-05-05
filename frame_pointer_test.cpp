#include <stdint.h>

extern "C" {
    extern inline uint32_t get_ebp_test() {
        register uint32_t ebp_reg asm("ebp");
        register uint32_t ret asm("eax");
        __asm__ volatile (
            ""
            : "=r"(ret)
        );
        return ret;
    }
}