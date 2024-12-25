#pragma once

#ifndef Z86_CPU
#define Z86_CPU 1

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <type_traits>

struct PortDevice {
    // Receive data from CPU.
    // Returns true if this device handled the port
    virtual bool out(uint16_t port, uint16_t value) = NULL;

    // Send data to CPU
    // Returns true if this device handled the port
    virtual bool in(uint16_t& value, uint16_t port) = NULL;
};

enum Interrupt : uint8_t {
    IntDE = 0,
    IntDB = 1,
    IntNMI = 2,
    IntBP = 3,
    IntOF = 4
};

void z86_execute();
void z86_interrupt(uint8_t number);
void z86_cancel_interrupt();
void z86_nmi();
void z86_reset();

void z86_add_device(PortDevice* device);

size_t z86_mem_write(size_t dst, const void* src, size_t size);

template <typename T>
static inline size_t z86_mem_write(size_t dst, const T& src) {
    return z86_mem_write(dst, &src, sizeof(T));
}

size_t z86_mem_read(void* dst, size_t src, size_t size);

template <typename T>
static inline size_t z86_mem_read(T& dst, size_t src) {
    return z86_mem_read(&dst, src, sizeof(T));
}

#endif