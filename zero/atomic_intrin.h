#ifndef ATOMIC_INTRIN_H
#define ATOMIC_INTRIN_H 1
#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma once


#include <atomic>

template <typename T>
static inline bool atomic_bit_set(std::atomic<T>& val, size_t bit) {
    const T mask = (T)1 << bit;
    return val.fetch_or(mask) & mask;
}

template <typename T>
static inline bool atomic_bit_clear(std::atomic<T>& val, size_t bit) {
    const T mask = (T)1 << bit;
    return val.fetch_and(~mask) & mask;
}

template <typename T>
static inline bool atomic_bit_flip(std::atomic<T>& val, size_t bit) {
    const T mask = (T)1 << bit;
    return val.fetch_xor(mask) & mask;
}


#endif