#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <vector>

#include <winsock2.h>
#include <Windows.h>

#include "..\zero\util.h"

struct TF4UDP {
    dummy_fields(0x234); // 0x0
    std::vector<uint8_t> recv_data; // 0x234
    void* __ptr_240; // 0x240
    int __dword_244; // 0x244
    void* __manbow_network_impl; // 0x248
    // 0x24C
};

using VirtualProtect_t = decltype(VirtualProtect);
using WSASendTo_t = decltype(WSASendTo);
using SetWindowTextA_t = decltype(SetWindowTextA);
using GetWindowTextA_t = decltype(GetWindowTextA);
using GetClassNameA_t = decltype(GetClassNameA);

dllexport gnu_noinline int detour_chain(const char *dll_name, int flags, ...) {
    va_list va;
    va_start(va, flags);
    use_var(dll_name);
    use_var(flags);
    va_end(va);
    return rand();
}

dllexport gnu_noinline const char* runconfig_title_get() {
    return (const char*)rand();
}

dllexport gnu_noinline HMODULE stdcall th_GetModuleHandleW(const wchar_t* str) {
    return GetModuleHandleW(str);
}

dllexport gnu_noinline void* stdcall th_GetProcAddress(HMODULE mod, const char* str) {
    return (void*)GetProcAddress(mod, str);
}

dllexport gnu_noinline BOOL WINAPI SetWindowTextU(HWND hWnd, LPCSTR lpString) {
    use_var(hWnd);
    use_var(lpString);
    return (BOOL)rand();
}

dllexport WSASendTo_t* wsa_send_to = NULL; // <codecave:netplayz_data>
dllexport VirtualProtect_t* virtual_protect = NULL; // (<codecave:netplayz_data>+4)

dllexport SetWindowTextA_t* set_window_text = NULL;

void mem_write(void* address, const void* data, size_t size, DWORD& protect_temp, VirtualProtect_t* vp_func = virtual_protect) {
    if (vp_func(address, size, PAGE_READWRITE, &protect_temp)) {
        memcpy(address, data, size);
        vp_func(address, size, protect_temp, &protect_temp);
    }
}

void mem_write(void* address, const void* data, size_t size, VirtualProtect_t* vp_func = virtual_protect) {
    DWORD protect_temp;
    return mem_write(address, data, size, protect_temp, vp_func);
}

template <typename T>
static forceinline void mem_write(T address, const void* data, size_t size, DWORD& protect_temp, VirtualProtect_t* vp_func = virtual_protect) {
    return mem_write((void*)address, data, size, protect_temp, vp_func);
}

template <typename T>
static forceinline void mem_write(T address, const void* data, size_t size, VirtualProtect_t* vp_func = virtual_protect) {
    DWORD protect_temp;
    return mem_write((void*)address, data, size, protect_temp, vp_func);
}

template <typename T, typename D>
static forceinline void mem_write(T address, const D& data, DWORD& protect_temp, VirtualProtect_t* vp_func = virtual_protect) {
    return mem_write((void*)address, (const void*)&data, sizeof(D), protect_temp, vp_func);
}

template <typename T, typename D>
static forceinline void mem_write(T address, const D& data, VirtualProtect_t* vp_func = virtual_protect) {
    DWORD protect_temp;
    return mem_write((void*)address, (const void*)&data, sizeof(D), protect_temp, vp_func);
}

struct PacketLayout {
    uint8_t type;
    unsigned char data[];
};

dllexport uint64_t timestamp = {}; // (<codecave:netplayz_data>+8)
dllexport size_t title_buf_len = 0; // (<codecave:netplayz_data>+16)
dllexport uint8_t lag_packets = 0; // (<codecave:netplayz_data>+20)
dllexport bool resyncing = false; // (<codecave:netplayz_data>+21)
dllexport bool not_in_match = false; // (<codecave:netplayz_data>+22)
dllexport char title_buf[512] = {}; // (<codecave:netplayz_data>+32)

dllexport HWND hwnd = (HWND)rand();

#define LOW_QUAD(val) ((uint32_t)(val))
#define HIGH_QUAD(val) ((uint32_t)(((uint64_t)(val))>>32))

void __fastcall __apply_patch_B(uint8_t value) {
    static constexpr uint8_t value_table[] = {
        5, 10, 15, 20, 25, 30, 35, INT8_MAX
    };
    mem_write(0xE364C, value_table[value]);
}

template<size_t N>
void update_window_text(const char(&text)[N]) {
    if (size_t length = title_buf_len) {
        if (length + N <= sizeof(title_buf)) {
            memcpy(title_buf + length, text, N);
            set_window_text(hwnd, title_buf);
        }
    }
}

void run_resync_logic(uint64_t new_timestamp) {
    // WTF is this supposed to do?
    // Looks like it might not actually be a 64 bit timestamp...
    uint32_t new_tsc = abs((int32_t)(LOW_QUAD(timestamp) - HIGH_QUAD(timestamp)));
    new_tsc += 4;
    if (new_tsc > INT8_MAX) {
        new_tsc = INT8_MAX;
    }
    if (!resyncing) {
        if (LOW_QUAD(timestamp) == LOW_QUAD(new_timestamp) &&
            HIGH_QUAD(timestamp) == HIGH_QUAD(new_timestamp)
        ) {
            ++lag_packets;
        }
        else {
            lag_packets = 0;
        }
        if (lag_packets >= new_tsc) {
            resyncing = true;
            lag_packets = 0;
            update_window_text("[Resyncing]");
        }
        timestamp = new_timestamp;
    }
    else {
        uint32_t lag = lag_packets;
        new_tsc *= 2;
        if (lag < new_tsc) {
            __apply_patch_B((lag << 3) / new_tsc);
            ++lag_packets;
        }
        else {
            resyncing = false;
            lag_packets = 0;
            update_window_text("");
        }
    }
}

dllexport int WSAAPI WSASendTo_hook(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const sockaddr* lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {

    PacketLayout* packet = (PacketLayout*)lpBuffers[0].buf;

    uint64_t timestamp;
    switch (packet->type) {
        default:
            break;
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
        if (0) {
        case 18:
            if (lpBuffers[0].len < 25) {
                break;
            }
            timestamp = *(uint64_t*)&packet->data[16];
        } else {
        case 19:
            if (lpBuffers[0].len < 26) {
                break;
            }
            timestamp = *(uint64_t*)&packet->data[17];
        }
            run_resync_logic(timestamp);
            break;
    }

    return wsa_send_to(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iTolen, lpOverlapped, lpCompletionRoutine);
}

typedef void thiscall packet_parser_t(
    TF4UDP* self,
    size_t packet_size
);

dllexport gnu_noinline void thiscall packet_parser(TF4UDP* self, size_t packet_size) {
    use_var(self);
    use_var(packet_size);
}

dllexport void thiscall packet_parser_hook(
    TF4UDP* self,
    size_t packet_size
) {

    PacketLayout* packet = (PacketLayout*)self->recv_data.data();

    switch (packet->type) {
        default:
            break;
        case 0:
        case 11: case 12: case 13:
        case 14: case 15: case 16:
        case 17: case 18: case 19:
            not_in_match = false;
            break;
    }

    return packet_parser(self, packet_size);
}

#define TITLE_VERSION_STR " + N 1.04z "

dllexport void netplayz_patch_post_init() {
	if (
		!th_GetModuleHandleW(L"Netplay.dll") &&
		!th_GetModuleHandleW(L"Netcode.dll")
	) {

        const char* game_title = runconfig_title_get();
        size_t title_len = strlen(game_title);
        if (title_len < sizeof(title_buf)) {
            memcpy(title_buf, game_title, title_len);
            if (title_len + sizeof(TITLE_VERSION_STR) <= sizeof(title_buf)) {
                memcpy(title_buf + title_len, TITLE_VERSION_STR, sizeof(TITLE_VERSION_STR));
                title_len += sizeof(TITLE_VERSION_STR) - 1;
            }
            title_buf_len = title_len;
        }

        HMODULE winsock_handle = th_GetModuleHandleW(L"WS2_32.dll");
        wsa_send_to = (WSASendTo_t*)th_GetProcAddress(winsock_handle, "WSASendTo");
        HMODULE kernel32_handle = th_GetModuleHandleW(L"kernel32.dll");
        VirtualProtect_t* virtual_protect_local = (VirtualProtect_t*)th_GetProcAddress(kernel32_handle, "VirtualProtect");
        virtual_protect = virtual_protect_local;

        DWORD protect_temp;
        mem_write(0x3884D4, &WSASendTo_hook, protect_temp, virtual_protect_local);
        mem_write(0x176B8A, &packet_parser_hook, protect_temp, virtual_protect_local);
        static constexpr uint8_t patch[5] = { 0x68, 0x1, 0x2, 0x3, 0x4 };
        mem_write(0xEA51, patch, protect_temp, virtual_protect_local);
	}
}

int main() {

}