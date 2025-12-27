#pragma once
#if !LAST_BRANCH_ENABLE_H
#define LAST_BRANCH_ENABLE_H 1

#include <stdint.h>
#include <stdbool.h>

#if _WIN32
#include "windows.h"
typedef HANDLE thread_id_t;
#define CURRENT_THREAD ((HANDLE)(intptr_t)-2)
#else
#include <unistd.h>
typedef pid_t thread_id_t;
#define CURRENT_THREAD (gettid())
#endif

#if __cplusplus
#define DEFAULT_CURRENT_THREAD = CURRENT_THREAD
#else
#define DEFAULT_CURRENT_THREAD
#endif

typedef struct LastBranchPair_t {
    uintptr_t to;
    uintptr_t from;
} LastBranchPair_t;

bool last_branch_tracking_hook(void);
bool last_branch_tracking_unhook(void);

bool last_branch_tracking_start(thread_id_t thread DEFAULT_CURRENT_THREAD);
bool last_branch_tracking_stop(thread_id_t thread DEFAULT_CURRENT_THREAD);
uintptr_t last_branch_exception_to(thread_id_t thread DEFAULT_CURRENT_THREAD);
uintptr_t last_branch_exception_from(thread_id_t thread DEFAULT_CURRENT_THREAD);
LastBranchPair_t last_branch_get_exceptions(thread_id_t thread DEFAULT_CURRENT_THREAD);

#endif