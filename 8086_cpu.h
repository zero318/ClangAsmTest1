#pragma once

#ifndef Z86_CPU
#define Z86_CPU 1

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <type_traits>

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

void execute_z86();

#endif