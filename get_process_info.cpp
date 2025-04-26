#include <utility>

struct ProcessInfo {
    HANDLE handle;
    DWORD pid;
    DWORD path_length;
    const wchar_t* path;
};

std::pair<DWORD*, size_t> get_all_pids() {
    DWORD size = sizeof(DWORD[1024]);
    if (DWORD* pids = (DWORD*)malloc(size)) {
        for (;;) {
            DWORD written;
            if (!EnumProcesses(pids, size, &written)) {
                break;
            }
            if (written == size) {
                size += sizeof(DWORD[1024]);
                if (DWORD* new_pids = (DWORD*)realloc(pids, size)) {
                    pids = new_pids;
                    continue;
                }
                break;
            }
            return { pids, written / sizeof(DWORD) };
        }
        free(pids);
    }
    return { NULL, 0 };
}

std::pair<wchar_t*, size_t> get_process_path(HANDLE handle) {
    size_t length = MAX_PATH;
    if (wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t[length]))) {
        for (;;) {
            DWORD written_length = GetProcessImageFileNameW(handle, buffer, length);
            if (!written_length) {
                length += MAX_PATH;
                if (wchar_t* new_buffer = (wchar_t*)realloc(buffer, length)) {
                    buffer = new_buffer;
                    continue;
                }
                return { NULL, 0 };
            }
            return { buffer, written_length };
        }
    }
    return { NULL, 0 };
}

std::pair<ProcessInfo*, size_t> get_all_process_handles() {
    auto [pids, count] = get_all_pids();
    if (pids) {
        if (ProcessInfo* info = (ProcessInfo*)malloc(sizeof(ProcessInfo[count]))) {
            size_t successes = 0;
            for (size_t i = 0; i < count; ++i) {
                DWORD pid = pids[i];
                if (HANDLE handle = OpenHandle(PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION , FALSE, pid)) {
                    auto [name, name_length] = get_process_path(handle);
                    if (name) {
                        info[successes++] = { handle, pid, name_length, name };
                        continue;
                    }
                }
            }
            free(pids);
            return { info, successes };
        }
        free(pids);
    }
    return { NULL, 0 };
}