#pragma once
#if !LAST_BRANCH_ENABLE_H
#define LAST_BRANCH_ENABLE_H 1

#include <stdint.h>
#include <stdbool.h>

#include "windows.h"

typedef struct LastBranchPair_t {
    uintptr_t to;
    uintptr_t from;
} LastBranchPair_t;

bool last_branch_tracking_hook();
bool last_branch_tracking_unhook();
bool last_branch_tracking_start();
bool last_branch_tracking_stop();

uintptr_t last_branch_exception_to(HANDLE thread);
uintptr_t last_branch_exception_from(HANDLE thread);
LastBranchPair_t last_branch_get_exceptions(HANDLE thread);

#endif