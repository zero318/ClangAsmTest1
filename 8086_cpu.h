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

struct RAM {
    uint8_t raw[0x110000];

    template <typename T = uint8_t>
    inline T* ptr(size_t offset) {
        return (T*)&this->raw[offset];
    }

    template <typename T = uint8_t>
    inline const T* ptr(size_t offset) const {
        return (const T*)&this->raw[offset];
    }

    template <typename T = uint8_t>
    inline T& ref(size_t offset) {
        return *this->ptr<T>(offset);
    }

    template <typename T = uint8_t>
    inline const T& ref(size_t offset) const {
        return *this->ptr<T>(offset);
    }

    template <typename T = uint8_t>
    inline T read(size_t offset) const {
        return this->ref<T>(offset);
    }

    template <typename T = uint8_t>
    inline void write(size_t offset, const T& value) {
        if constexpr (!std::is_array_v<std::remove_reference_t<T>>) {
            this->ref<T>(offset) = value;
        }
        else {
            memcpy(this->ptr(offset), &value, sizeof(T));
        }
    }

    inline uint8_t operator[](size_t offset) const {
        return this->read(offset);
    }
};

extern RAM mem;

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

#endif