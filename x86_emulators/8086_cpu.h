#pragma once

#ifndef Z86_CPU
#define Z86_CPU 1

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <type_traits>

static inline constexpr unsigned long long operator ""_KB(unsigned long long value) {
    return value * 1024ull;
}

static inline constexpr unsigned long long operator ""_KB(long double value) {
    return value * 1024.0L;
}

static inline constexpr unsigned long long operator ""_MB(unsigned long long value) {
    return value * 1048576ull;
}

static inline constexpr unsigned long long operator ""_MB(long double value) {
    return value * 1048576.0L;
}

static inline constexpr unsigned long long operator ""_GB(unsigned long long value) {
    return value * 1073741824ull;
}

static inline constexpr unsigned long long operator ""_GB(long double value) {
    return value * 1073741824.0L;
}

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

    uint8_t temp_value;
    virtual bool out_byte(uint32_t port, uint8_t value) {
        if (port & 1) {
            return this->out_word(port & ~1, value << 8 | this->temp_value);
        }
        this->temp_value = value;
        return true;
    }

    // Send data to CPU
    // Returns true if this device handled the port
    virtual bool in_word(uint16_t& value, uint32_t port) {
        return false;
    }
    virtual bool in_byte(uint8_t& value, uint32_t port) {
        uint16_t temp = 0;
        if (this->in_word(temp, port & ~1)) {
            value = temp >> (port & 1) * 8;
            return true;
        }
        return false;
    }
};

struct PortDwordDevice {
    // Receive data from CPU.
    // Returns true if this device handled the port
    virtual bool out_dword(uint32_t port, uint32_t value) {
        return false;
    }

    uint32_t temp_value;
    virtual bool out_word(uint32_t port, uint16_t value) {
        if (port & 3) {
            return this->out_dword(port & ~3, value << 16 | (uint16_t)this->temp_value);
        }
        this->temp_value = value;
        return false;
    }
    virtual bool out_byte(uint32_t port, uint8_t value) {
        uint32_t full_value = this->temp_value >> 8 | value << 24;
        if ((port & 3) == 3) {
            return this->out_dword(port & ~3, full_value);
        }
        this->temp_value = full_value;
        return false;
    }

    // Send data to CPU
    // Returns true if this device handled the port
    virtual bool in_dword(uint32_t& value, uint32_t port) {
        return false;
    }
    virtual bool in_word(uint16_t& value, uint32_t port) {
        uint32_t temp = 0;
        if (this->in_dword(temp, port & ~3)) {
            value = temp >> (port & 2) * 8;
            return true;
        }
        return false;
    }
    virtual bool in_byte(uint8_t& value, uint32_t port) {
        uint32_t temp = 0;
        if (this->in_dword(temp, port & ~3)) {
            value = temp >> (port & 3) * 8;
            return true;
        }
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
    IntUD = 6,
    // 80286
    IntNM = 7,
    IntDF = 8,
    IntCSO = 9, // Coprocessor segment overrun
    IntTS = 10,
    IntNP = 11,
    IntSS = 12,
    IntGP = 13,
    // ???
    IntPF = 14,
    IntMF = 16,
    IntAC = 17,
    IntMC = 18,
    IntXM = 19,
    IntVE = 20,
    IntCP = 21
};

void z86_execute();
void z86_interrupt(uint8_t number);
void z86_cancel_interrupt();
void z86_nmi();
void z86_reset(); // hard reset
//void z86_init(); // soft reset?

void z86_add_dword_device(PortDwordDevice* device);
void z86_add_word_device(PortWordDevice* device);
void z86_add_byte_device(PortByteDevice* device);

size_t z86_mem_write(size_t dst, const void* src, size_t size);
size_t z86_file_read(size_t dst, FILE* src, size_t size);

template <typename T>
static inline size_t z86_mem_write(size_t dst, const T& src) {
    return z86_mem_write(dst, &src, sizeof(T));
}

size_t z86_mem_read(void* dst, size_t src, size_t size);
size_t z86_file_write(FILE* dst, size_t src, size_t size);

template <typename T>
static inline size_t z86_mem_read(T& dst, size_t src) {
    return z86_mem_read(&dst, src, sizeof(T));
}

#endif