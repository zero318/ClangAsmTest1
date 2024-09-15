#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <windows.h>

#include <WinSock2.h>

#include <SoftPub.h>

#pragma comment (lib, "Ws2_32.lib")

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

#define uppercase_mask 0xDFu
#define lowercase_mask 0x20u

#define lowercase(c) ((c)|lowercase_mask)
#define uppercase(c) ((c)&uppercase_mask)

#if STOP_LINKING_DEFAULT_LIB_PLZ

// Replacement functions for use when linking without the
// standard library (compiling closer to the original DLL)
// 
// None of the implementations are based directly on the
// original DLL, but there's only so different they can be
// while sticking to the standard.

char* np_strrchr(const char* str, int ch) {
    const char* ret = NULL;

    while (char c = *str) {
        if (c == ch) {
            ret = str;
        }
        ++str;
    }

    return (char*)ret;
}
#define strrchr np_strrchr

constexpr size_t np_strlen(const char* str) {
    const char* temp = str;
    while (*temp) ++temp;
    return temp - str;
}
#define strlen np_strlen

char* np_strncpy(char* dst, const char* src, size_t count) {
    if (count) {
        
        char* dst_write = dst;
        do {
            char c = *src++;
            if (c) {
                *dst_write++ = c;
            } else {
                do {
                    *(volatile char*)(dst_write++) = 0;
                } while (--count);
                break;
            }
        } while (--count);
    }
    return dst;
}
#define strncpy np_strncpy

char* np_strncat(char* dst, const char* src, size_t count) {
    char* dst_write = dst;
    while (*dst_write) ++dst_write;

    if (count) {
        do {
            if (!(*dst_write++ = *src++)) {
                break;
            }
        } while (--count);
    }

    *dst_write = '\0';
    return dst;
}
#define strncat np_strncat

int np_strcmp(const char* lhs, const char* rhs) {
    uint8_t c;
    while ((c = *lhs++) == *rhs++) {
        if (!c) {
            return 0;
        }
    }
    return 1;
}
#define strcmp np_strcmp

int np_stricmp(const char* lhs, const char* rhs) {
    uint8_t c;
    while (lowercase(c = *lhs++) == lowercase(*rhs++)) {
        if (!c) {
            return 0;
        }
    }
    return 1;
}
#define _stricmp np_stricmp

int np_strnicmp(const char* lhs, const char* rhs, size_t count) {
    if (count) {
        size_t i = 0;
        uint8_t c;
        do {
            if (lowercase(c = lhs[i]) != lowercase(rhs[i])) {
                return 1;
            }
        } while (c && ++i < count);
    }
    return 0;
}
#define _strnicmp np_strnicmp

#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuninitialized"

template <typename T>
static inline constexpr T uninitialized_value(void) {
    T garbage;
    return garbage;
}

#define UNINITIALIZED_VALUE(type) uninitialized_value<type>()
#define UNINITIALIZED_ARG(type) UNINITIALIZED_VALUE(type)

// Used to represent an argument that the compiler fully
// optimized out but still needs to be present for the
// calling convention.
#define UNUSED_HMODULE (UNINITIALIZED_ARG(HMODULE))

#pragma clang diagnostic pop

#define MASK_TRUE(value, mask) ((bool)((value) & (mask)))
#define MASK_FALSE(value, mask) ((bool)(~(value) & (mask)))

using WinVerifyTrust_t = decltype(WinVerifyTrust);
using CreateMutexA_t = decltype(CreateMutexA);
using SetWindowTextA_t = decltype(SetWindowTextA);
using WSASendTo_t = decltype(WSASendTo);
using WSARecvFrom_t = decltype(WSARecvFrom);

#define DETOUR_CHAIN_RETURN_OLD_POINTERS 1
int __cdecl detour_chain(const char *dll_name, int flags, ...);

const char* runconfig_game_get();

using detour_chain_t = decltype(detour_chain);
using runconfig_game_get_t = decltype(runconfig_game_get);

struct WinFuncs {
    WinVerifyTrust_t* WinVerifyTrust_func;
    CreateMutexA_t* CreateMutexA_func;
    SetWindowTextA_t* SetWindowTextA_func;
};

struct NetFuncs {
    void* __unknown_net_funcA;
    void* __unknown_net_funcB;
    WSASendTo_t* WSASendTo_func;
    WSARecvFrom_t* WSARecvFrom_func;
};

// Rx4030
// Dummy variable for proprietary code blob
static uint8_t __patch_B_data[1];

// Rx40B0
// Dummy variable for proprietary code blob
static uint8_t __patch_A_data[1];

// Rx42F4
static CreateMutexA_t* CreateMutexA_func_temp = CreateMutexA;

// Rx42F8
static SetWindowTextA_t* SetWindowTextA_func_temp = SetWindowTextA;

// Rx4358
static WinFuncs win_funcs = {};

// Rx4364
static char title_suffix_buf[32] = {};

// Rx4384
static WSARecvFrom_t* WSARecvFrom_func_temp = NULL;
// Rx4388
static WSASendTo_t* WSASendTo_func_temp = NULL;

// Rx438C
static bool resyncing = false;
// Rx438D
static bool not_in_match = false;
// Rx438E
static uint8_t lag_packets = 0;

// Rx4390
static NetFuncs net_funcs = {};

// Rx43A0
static ULARGE_INTEGER prev_timestamp = {};

// Rx43AC
static HMODULE thcrap_handle = NULL;

// Rx1BE4
IMAGE_IMPORT_DESCRIPTOR* __fastcall get_import_table(HMODULE module);

// Rx1030 (Not quite fastcall, has caller cleanup)
BOOL __fastcall patch_import(
    HMODULE module,
    IMAGE_IMPORT_DESCRIPTOR* import_descriptor,
    void* old_func,
    void* new_func
) {
    for (
        void** iat = (void**)((uintptr_t)module + import_descriptor->FirstThunk);
        void* iat_func = *iat;
        ++iat
    ) {
        if (iat_func == old_func) {
            MEMORY_BASIC_INFORMATION mem;
            VirtualQuery(iat, &mem, sizeof(mem));

            DWORD old_protect;
            VirtualProtect(mem.BaseAddress, mem.RegionSize, PAGE_READWRITE, &old_protect);

            *iat = new_func;

            VirtualProtect(mem.BaseAddress, mem.RegionSize, old_protect, &old_protect);

            return TRUE;
        }
    }
    return FALSE;
}

// Rx1116 (Not quite fastcall, has caller cleanup)
bool __fastcall replace_function(
    HMODULE module,
    const char* dll_name,
    const char* func_name,
    void* replacement,
    void** old_func
) {
    if (HMODULE dll_handle = GetModuleHandleA(dll_name)) {
        void* func_addr = (void*)GetProcAddress(dll_handle, func_name);
        *old_func = func_addr;

        IMAGE_IMPORT_DESCRIPTOR* import_descriptors = get_import_table(module);

        while (DWORD name_offset = import_descriptors->Name) {

            const char* name = (const char*)((uintptr_t)module + name_offset);

            size_t name_length;
            if (const char* dot_ptr = strrchr(name, '.')) {
                name_length = dot_ptr - name;
            } else {
                name_length = strlen(name);
            }

            if (!_strnicmp(name, dll_name, name_length)) {
                return patch_import(module, import_descriptors, *old_func, replacement);
            }

            ++import_descriptors;
        }
    }
    return false;
}

// Rx1196
bool __fastcall is_memory_range_valid(void* address, size_t size) {
    MEMORY_BASIC_INFORMATION mem;
    if (VirtualQuery(address, &mem, sizeof(mem))) {
        // NOTE: The access type isn't really a
        // bit flag, but I guess this works?
        if (MASK_FALSE(mem.Protect, PAGE_NOACCESS)) {
            uintptr_t mem_end = (uintptr_t)mem.BaseAddress + mem.RegionSize;
            uintptr_t query_end = (uintptr_t)address + size;
            if (mem_end >= query_end) {
                return true;
            }
        }
    }
    return false;
}

// Rx11D3
void __fastcall set_title_with_suffix(const char* suffix) {
    char title_buf[256];

    if (!suffix) {
        title_suffix_buf[0] = '\0';
    } else {
        // NOTE: The buffer is 0 initialized by default and
        // only 17 characters will ever be copied with the
        // current uses, so this is safe.
        strncpy(title_suffix_buf, suffix, sizeof(title_suffix_buf) - 1);
    }

    if (HWND window = FindWindowA("th155", NULL)) {
        GetWindowTextA(window, title_buf, sizeof(title_buf));

        // NOTE: Assumes strrchr does not return NULL
        strrchr(title_buf, '+')[-1] = '\0';

        // BUG: The size parameter of strncat controls how many characters
        // are copied from the source, not the max length of the destination.
        // This does nothing to prevent buffer overflows.
        strncat(title_buf, " + N 1.03f ", sizeof(title_buf));
        strncat(title_buf, title_suffix_buf, sizeof(title_buf));

        SetWindowTextA(window, title_buf);
    }
}

// Rx1266
LONG __stdcall WinVerifyTrust_dummy(HWND hwnd, GUID* pgActionID, LPVOID pWVTData) {
    return 0;
}

// Rx126B
HANDLE __stdcall CreateMutexA_replacement(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) {
    return win_funcs.CreateMutexA_func(lpMutexAttributes, bInitialOwner, NULL);
}

// Rx1280
BOOL __stdcall SetWindowTextA_replacement(HWND hWnd, LPCSTR lpString) {
    char class_buf[sizeof("th155")];
    char title_buf[256];

    GetClassNameA(hWnd, class_buf, sizeof(class_buf));

    if (!strcmp(class_buf, "th155")) {
        // BUG:
        // strncpy does not add a null terminator if it copies the specified
        // number of bytes into the buffer. Subtracting 1 from the buffer size
        // without also writing a null terminator to the end of the buffer
        // does not prevent buffer overflows.
        strncpy(title_buf, lpString, sizeof(title_buf) - 1);

        // BUG: The size parameter of strncat controls how many characters
        // are copied from the source, not the max length of the destination.
        // This does nothing to prevent buffer overflows.
        strncat(title_buf, " + N 1.03f ", sizeof(title_buf));
        strncat(title_buf, title_suffix_buf, sizeof(title_buf));

        return win_funcs.SetWindowTextA_func(hWnd, title_buf);
    } else {
        return win_funcs.SetWindowTextA_func(hWnd, lpString);
    }
}

// Rx14D4
int WSAAPI WSASendTo_replacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const sockaddr* lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

// Rx1563
int WSAAPI WSARecvFrom_replacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, sockaddr* lpFrom, LPINT lpFromLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

void thcrap_apply_net_detours() {
    HMODULE winsock_handle = GetModuleHandleA("WS2_32.DLL");

    WSASendTo_func_temp = (WSASendTo_t*)GetProcAddress(winsock_handle, "WSASendTo");
    WSARecvFrom_func_temp = (WSARecvFrom_t*)GetProcAddress(winsock_handle, "WSARecvFrom");

    if (
        WSASendTo_func_temp &&
        WSARecvFrom_func_temp
    ) {
        if (detour_chain_t* detour_chain = (detour_chain_t*)GetProcAddress(thcrap_handle, "detour_chain")) {

            detour_chain("WS2_32.DLL", DETOUR_CHAIN_RETURN_OLD_POINTERS,
                         "WSASendTo", WSASendTo_replacement, &WSASendTo_func_temp,
                         "WSARecvFrom", WSARecvFrom_replacement, &WSARecvFrom_func_temp,
                         NULL
            );

            NetFuncs net_funcs_temp;

            net_funcs_temp.WSASendTo_func = WSASendTo_func_temp;
            net_funcs_temp.WSARecvFrom_func = WSARecvFrom_func_temp;

            net_funcs = net_funcs_temp;
        }
    }
}

void __fastcall __apply_patch_B(HMODULE module, void* patch_data, uint8_t value);

// Rx1432
// Argument is likely a uint64_t based on the calling convention,
// but all handling is done similar to ULARGE_INTEGER
void __fastcall run_resync_logic(ULARGE_INTEGER new_timestamp) {
    if (!resyncing) {
        if (
            // BUG (that probably isn't important):
            // Timestamp comparison should be checking QuadPart.
            // If the LowPart happens to be the same but not HighPart
            // then this will be detected as a duplicate timestamp
            prev_timestamp.LowPart != new_timestamp.LowPart &&
            prev_timestamp.HighPart != new_timestamp.HighPart
        ) {
            lag_packets = 0;
        }
        else {
            if (++lag_packets >= UINT8_MAX) {
                resyncing = true;
                lag_packets = 0;
                set_title_with_suffix("[Resynchronizing]");
            }
        }
        prev_timestamp = new_timestamp;
    } else {
        if (lag_packets > INT8_MAX) {
            resyncing = false;
            lag_packets = 0;
            set_title_with_suffix(NULL);
        }
        else {
            // NOTE: This return value of this handle call is completely unused
            (void)GetModuleHandleA(NULL);
            __apply_patch_B(UNUSED_HMODULE, __patch_B_data, lag_packets);
            ++lag_packets;
        }
    }
}

struct PacketLayout {
    int8_t type;
    unsigned char data[];
};

// Rx14D4
int WSAAPI WSASendTo_replacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const sockaddr* lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
    PacketLayout* packet = (PacketLayout*)lpBuffers[0].buf;

    switch (packet->type) {
        case 9:
            if (not_in_match) {
                if (lpNumberOfBytesSent) {
                    *lpNumberOfBytesSent = 1;
                }
                return 0;
            }
            not_in_match = true;
            break;
        case 11:
            not_in_match = false;
            break;
        case 18:
            if (lpBuffers[0].len >= 25) {
                run_resync_logic(*(ULARGE_INTEGER*)&packet->data[16]);
            }
            break;
        case 19:
            if (lpBuffers[0].len >= 26) {
                run_resync_logic(*(ULARGE_INTEGER*)&packet->data[17]);
            }
            break;
    }

    return net_funcs.WSASendTo_func(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iTolen, lpOverlapped, lpCompletionRoutine);
}

// Rx1563
int WSAAPI WSARecvFrom_replacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, sockaddr* lpFrom, LPINT lpFromLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
    int ret = net_funcs.WSARecvFrom_func(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromLen, lpOverlapped, lpCompletionRoutine);

    PacketLayout* packet = (PacketLayout*)lpBuffers[0].buf;

    switch (packet->type) {
        case 0: default:
            not_in_match = false;
        case 1: case 2: case 3: case 4: case 5:
        case 6: case 7: case 8: case 9: case 10:
        case 11:
            break;
    }

    return ret;
}

// Rx15CA
// Simplified reimplementation of __apply_patch_A to remove proprietary code
void __fastcall __apply_patch_A(HMODULE module, void* patch_data, uint8_t value) {

    struct PatchAData {
        uint32_t offset;
        uint8_t byte;
    };

    static constexpr PatchAData patches[] = {
        // The standard 3 anti tamper patches
        { 0x12E820, 0xC3 },
        { 0x130630, 0xC3 },
        { 0x132AF0, 0xC3 },

        // ???
        { 0xE357A, 0x7F },
    };

    for (size_t i = 0; i < countof(patches); ++i) {
        uint8_t* patch_addr = (uint8_t*)((uintptr_t)module + patches[i].offset);

        DWORD old_protect;
        if (VirtualProtect(patch_addr, 1, PAGE_READWRITE, &old_protect)) {

            *patch_addr = patches[i].byte;

            VirtualProtect(patch_addr, 1, old_protect, &old_protect);

            FlushInstructionCache(GetCurrentProcess(), patch_addr, 1);
        }
    }
}

// Rx178A
// Simplified reimplementation of __apply_patch_B to remove proprietary code
void __fastcall __apply_patch_B(HMODULE module, void* patch_data, uint8_t value) {
    // NOTE: module argument in ECX is not used
    uint8_t* patch_addr = (uint8_t*)((uintptr_t)GetModuleHandleA(NULL) + 0xE364C);

    DWORD old_protect;
    if (VirtualProtect(patch_addr, 1, PAGE_READWRITE, &old_protect)) {

        static constexpr uint8_t value_table[] = {
            5, 10, 15, INT8_MAX
        };

        *patch_addr = value_table[value / 32];

        VirtualProtect(patch_addr, 1, old_protect, &old_protect);

        FlushInstructionCache(GetCurrentProcess(), patch_addr, 1);
    }
}

extern "C" {

    // Rx18CF
    __declspec(dllexport) void __cdecl InitializeAt(HMODULE module) {

        WinFuncs original_win_funcs;

        replace_function(module, "WINTRUST", "WinVerifyTrust", WinVerifyTrust_dummy, (void**)&original_win_funcs.WinVerifyTrust_func);
        replace_function(module, "KERNEL32", "CreateMutexA", CreateMutexA_replacement, (void**)&original_win_funcs.CreateMutexA_func);
        replace_function(module, "USER32", "SetWindowTextA", SetWindowTextA_replacement, (void**)&original_win_funcs.SetWindowTextA_func);

        NetFuncs original_net_funcs;

        replace_function(module, "WS2_32", "WSASendTo", WSASendTo_replacement, (void**)&original_net_funcs.WSASendTo_func);
        replace_function(module, "WS2_32", "WSARecvFrom", WSARecvFrom_replacement, (void**)&original_net_funcs.WSARecvFrom_func);

        win_funcs = original_win_funcs;
        net_funcs = original_net_funcs;

        __apply_patch_A(module, __patch_A_data, 0x60);
        __apply_patch_B(UNUSED_HMODULE, __patch_B_data, INT8_MAX); 
    }

    // Rx1B90
    __declspec(dllexport) void Initialize() {
        InitializeAt(GetModuleHandleA(NULL));
    }

}

// Rx1BA0
IMAGE_NT_HEADERS32* __fastcall get_nt_header(HMODULE module) {
    if (module) {
        IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)module;
        if (is_memory_range_valid(dos_header, sizeof(IMAGE_DOS_HEADER))) {
            if (dos_header->e_magic == IMAGE_DOS_SIGNATURE) {
                IMAGE_NT_HEADERS32* nt_header = (IMAGE_NT_HEADERS32*)((uintptr_t)module + dos_header->e_lfanew);
                if (is_memory_range_valid(nt_header, sizeof(IMAGE_NT_HEADERS32))) {
                    if (nt_header->Signature == IMAGE_NT_SIGNATURE) {
                        return nt_header;
                    }
                }
            }
        }
    }
    return NULL;
}

// Rx1BE4
IMAGE_IMPORT_DESCRIPTOR* __fastcall get_import_table(HMODULE module) {
    if (IMAGE_NT_HEADERS32* nt_header = get_nt_header(module)) {
        if (DWORD import_table_offset = nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) {
            return (IMAGE_IMPORT_DESCRIPTOR*)((uintptr_t)module + import_table_offset);
        }
    }
    return NULL;
}

// Rx1C0E
void thcrap_apply_win_detours() {
    detour_chain_t* detour_chain = (detour_chain_t*)GetProcAddress(thcrap_handle, "detour_chain");

    WinFuncs original_win_funcs;

    detour_chain("KERNEL32.DLL", DETOUR_CHAIN_RETURN_OLD_POINTERS,
                 "CreateMutexA", CreateMutexA_replacement, &CreateMutexA_func_temp,
                 NULL
    );

    detour_chain("USER32.DLL", DETOUR_CHAIN_RETURN_OLD_POINTERS,
                 "SetWindowTextA", SetWindowTextA_replacement, &SetWindowTextA_func_temp,
                 NULL
    );

    original_win_funcs.CreateMutexA_func = CreateMutexA_func_temp;
    original_win_funcs.SetWindowTextA_func = SetWindowTextA_func_temp;

    win_funcs = original_win_funcs;
}

extern "C" {

    __declspec(dllexport) bool net_hlp_isaocf() {
        // BUG: thcrap_handle will always be NULL if this
        // export is called when not loaded via thcrap
        if (runconfig_game_get_t* runconfig_game_get = (runconfig_game_get_t*)GetProcAddress(thcrap_handle, "runconfig_game_get")) {
            if (const char* game_id = runconfig_game_get()) {
                if (!strcmp(game_id, "th155")) {
                    return true;
                }
            }
        }
        else {
            const char* command_line = GetCommandLineA();

            // Rx31C0
            static constexpr const char search_patterns[][16] = {
                "th155",
                "Antinomy",
                "Flowers",
                "15.5",
                ""
            };

            size_t i = 0;
            const char* search_pattern = search_patterns[0];
            do {
                if (!_stricmp(command_line, search_pattern)) {
                    return true;
                }
                search_pattern = search_patterns[++i];
            } while (*search_pattern);
        }

        return false;
    }

    __declspec(dllexport) void __cdecl net_mod_detour(void*) {
        HMODULE current_module = GetModuleHandleA(NULL);

        thcrap_apply_win_detours();
        thcrap_apply_net_detours();

        __apply_patch_A(current_module, __patch_A_data, 0x40);
        __apply_patch_B(UNUSED_HMODULE, __patch_B_data, INT8_MAX); 
    }

    __declspec(dllexport) int __stdcall thcrap_plugin_init() {
        // NOTE: This return value of this handle call is completely unused
        (void)GetModuleHandleA(NULL);

        // BUG: thcrap_handle isn't set before calling
        // a helper function that requires it
        if (net_hlp_isaocf()) {
            thcrap_handle = GetModuleHandleA("thcrap.dll");
            return 0;
        }

        return 155; // Any non-zero value will unload the plugin
    }

    __declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
        return TRUE;
    }
}