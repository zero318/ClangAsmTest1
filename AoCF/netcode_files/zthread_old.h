#pragma once

#ifndef ZTHREAD_H
#define ZTHREAD_H 1

#include <stdlib.h>
#include <stdint.h>

#include <utility>
#include <thread>

struct zthread : std::thread {

    typedef void cleanup_func();

    cleanup_func *const cleanup;

    zthread() noexcept : cleanup(NULL), std::thread() {}

    template <typename F, typename ... Args>
    explicit zthread(cleanup_func* cleanup, F&& f, Args&&... args) : cleanup(cleanup), std::thread(std::forward<F>(f), std::forward<Args>(args)...) {}
    
    void join() {
        if (cleanup_func* cleanup = this->cleanup) {
            cleanup();
        }
        return ((std::thread*)this)->join();
    }

    ~zthread() {
        if (this->joinable()) {
            this->join();
        }
    }
};

#endif