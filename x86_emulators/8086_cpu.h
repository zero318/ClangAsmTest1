#pragma once

#ifndef Z86_CPU
#define Z86_CPU 1

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <type_traits>

struct PortByteDevice {
    // Receive data from CPU.
    // Returns true if this device handled the port
    virtual bool out_byte(uint32_t port, uint8_t value) {
        return false;
    }

    // Send data to CPU
    // Returns true if this device handled the port
    virtual bool in_byte(uint8_t& value, uint32_t port) {
        return false;
    }
};

struct PortWordDevice {
    // Receive data from CPU.
    // Returns true if this device handled the port
    virtual bool out_word(uint32_t port, uint16_t value) {
        return false;
    }
    virtual bool out_byte(uint32_t port, uint8_t value) {
        return false;
    }

    // Send data to CPU
    // Returns true if this device handled the port
    virtual bool in_word(uint16_t& value, uint32_t port) {
        return false;
    }
    virtual bool in_byte(uint8_t& value, uint32_t port) {
        return false;
    }
};

struct PortDwordDevice {
    // Receive data from CPU.
    // Returns true if this device handled the port
    virtual bool out_dword(uint32_t port, uint32_t value) {
        return false;
    }
    virtual bool out_word(uint32_t port, uint16_t value) {
        return false;
    }
    virtual bool out_byte(uint32_t port, uint8_t value) {
        return false;
    }

    // Send data to CPU
    // Returns true if this device handled the port
    virtual bool in_dword(uint32_t& value, uint32_t port) {
        return false;
    }
    virtual bool in_word(uint16_t& value, uint32_t port) {
        return false;
    }
    virtual bool in_byte(uint8_t& value, uint32_t port) {
        return false;
    }
};

enum Interrupt : uint8_t {
    // 8086
    IntDE = 0,
    IntDB = 1,
    IntNMI = 2,
    IntBP = 3,
    IntOF = 4,
    // 80186
    IntBR = 5,
    IntUD = 6
};

void z86_execute();
void z86_interrupt(uint8_t number);
void z86_cancel_interrupt();
void z86_nmi();
void z86_reset();

void z86_add_dword_device(PortDwordDevice* device);
void z86_add_word_device(PortWordDevice* device);
void z86_add_byte_device(PortByteDevice* device);

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