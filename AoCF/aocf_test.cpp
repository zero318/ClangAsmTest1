#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"

#if !CLANG_CL && __INTELLISENSE__
#define _HAS_CXX17 0
#define _HAS_CXX20 0
#endif

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <memory>
#include <atomic>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#include <Windows.h>

#include <dinput.h>

#pragma comment (lib, "Ws2_32.lib")

#include "..\zero\util.h"

struct UnknownA {
    int wsa_last_error;
    void* __ptr_4;
};

struct UnknownC {
    SOCKET socket;
};

// This does something with TLS and is called everywhere. Seems to be something about error handling
dllexport gnu_noinline void* __sub_r2DBEE0() {
    // I haven't reversed this yet, returning rand just makes it compile
    return (void*)rand();
};

dllexport gnu_noinline SOCKET __create_socket_r170620(int af, int type, int protocol, UnknownA* arg4) {
    WSASetLastError(0);
    SOCKET sock = WSASocketW(af, type, protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
    void* local1 = __sub_r2DBEE0();
    int last_error = WSAGetLastError();
    arg4->wsa_last_error = last_error;
    arg4->__ptr_4 = local1;
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    if (af == AF_INET6) {
        uint32_t opt_value = 0;
        setsockopt(sock, 41, 27, (char*)&opt_value, sizeof(opt_value));
    }
    arg4->__ptr_4 = __sub_r2DBEE0();
    arg4->wsa_last_error = ERROR_SUCCESS;
    return sock;
}

template<typename T = uint32_t>
struct JANK_CRITICAL_SECTION {
    union {
        CRITICAL_SECTION crit_section;
        struct {
            uint32_t : 32;
            uint32_t : 32;
            uint32_t : 32;
            uint32_t : 32;
            uint32_t : 32;
            uint32_t : 32;
            uint32_t : 32;
            T __padding_reuse_1C;
        };
    };
};

struct UnknownF {
    union {
        WSAOVERLAPPED __overlapped_0; // 0x0
        struct {
            void* __ptr_0; // 0x0
            unknown_fields(0x4); // 0x4
            int error; // 0x8
            int __int_C; // 0xC
            unknown_fields(0x4); // 0x10
            UnknownF* __unknownF_ptr_14; // 0x14
        };
    };
    unknown_fields(0x4); // 0x18
    std::atomic<int> __atomic_int_1C; // 0x1C
    // 0x20
};

struct UnknownD {
    std::atomic<int> __atomic_int_0;
    unknown_fields(0x10);
    HANDLE __handle_14; // 0x14
    unknown_fields(0x1C); // 0x18
    std::atomic<int> __atomic_int_34; // 0x34
    JANK_CRITICAL_SECTION<UnknownF*> __jank_crit_section_38; // 0x38
    UnknownF* __unknownF_ptr_58; // 0x58
    // 0x5C

    dllexport gnu_noinline void __sub_r16FDE0(UnknownF* unknown_f) {
        int value = 0;
        unknown_f->__atomic_int_1C.compare_exchange_strong(value, 1);
        if (value == 1) {
            if (!PostQueuedCompletionStatus(this->__handle_14, 0, 2, &unknown_f->__overlapped_0)) {
                CRITICAL_SECTION* crit_section = &this->__jank_crit_section_38.crit_section;
                EnterCriticalSection(crit_section);
                unknown_f->__unknownF_ptr_14 = NULL;
                if (UnknownF* local_ptr = this->__unknownF_ptr_58) {
                    local_ptr->__unknownF_ptr_14 = unknown_f;
                } else {
                    this->__jank_crit_section_38.__padding_reuse_1C = unknown_f;
                }
                this->__unknownF_ptr_58 = unknown_f;
                this->__atomic_int_34 = 1;
                LeaveCriticalSection(crit_section);
            }
        }
    }

    dllexport gnu_noinline void __sub_r16FE50(UnknownF* unknown_f, int error, int arg3) {
        unknown_f->__ptr_0 = __sub_r2DBEE0();
        unknown_f->error = error;
        unknown_f->__int_C = arg3;
        if (!PostQueuedCompletionStatus(this->__handle_14, 0, 2, (LPOVERLAPPED)unknown_f)) {
            CRITICAL_SECTION* crit_section = &this->__jank_crit_section_38.crit_section;
            EnterCriticalSection(crit_section);
            unknown_f->__unknownF_ptr_14 = NULL;
            if (UnknownF* local_ptr = this->__unknownF_ptr_58) {
                local_ptr->__unknownF_ptr_14 = unknown_f;
            } else {
                this->__jank_crit_section_38.__padding_reuse_1C = unknown_f;
            }
            this->__unknownF_ptr_58 = unknown_f;
            this->__atomic_int_34 = 1;
            LeaveCriticalSection(crit_section);
        }
    }
};

struct UnknownE {
    unknown_fields(0x4);
    UnknownD* __unknownD_ptr_4; // 0x4


    dllexport gnu_noinline void __sub_r170E00(UnknownC* unknown_c, LPWSABUF buffers, DWORD bufferCount, sockaddr* from, DWORD flags, LPINT from_len, UnknownF* unknown_f) {

        ++this->__unknownD_ptr_4->__atomic_int_0;

        SOCKET socket = unknown_c->socket;
        if (socket == INVALID_SOCKET) {
            this->__unknownD_ptr_4->__sub_r16FE50(unknown_f, WSAEBADF, 0);
            return;
        }
        flags = flags;
        DWORD received = 0;
        int result = WSARecvFrom(socket, buffers, bufferCount, &received, &flags, from, from_len, &unknown_f->__overlapped_0, NULL);
        int error = WSAGetLastError();
        if (error == ERROR_PORT_UNREACHABLE) {
            error = WSAECONNREFUSED;
        }
        if (result != 0 && error != ERROR_IO_PENDING) {
            this->__unknownD_ptr_4->__sub_r16FE50(unknown_f, error, received);
        } else {
            this->__unknownD_ptr_4->__sub_r16FDE0(unknown_f);
        }
    }
};


// Rx4DAD30
static HWND __window_handle;
// Rx4DAD34
static HINSTANCE __instance_handle;

// Rx4DAEB8
static DIMOUSESTATE2 MOUSE_STATE;
// Rx4DAECC
static LPDIRECTINPUTDEVICE8 KEYBOARD_DEVICE;
// Rx4DAEE8
static LPDIRECTINPUTDEVICE8 MOUSE_DEVICE;
// Rx4DAEEC
static LPDIRECTINPUT8 DINPUT;
// Rx4DAEF0
static DWORD XINPUT_PACKET_NUMBERS[4];
// Rx4DAF00
static BYTE KEYBOARD_STATE[256];

bool __keyboard_device_initialize();
bool __mouse_device_initialize();
BOOL PASCAL EnumDevicesCallback(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvref);
BOOL PASCAL EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef);

// Rx3B680
bool __input_initialize() {
    if (!DINPUT) {
        if (FAILED(DirectInput8Create(__instance_handle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&DINPUT, NULL))) {
            return false;
        }
        memset(KEYBOARD_STATE, 0, sizeof(KEYBOARD_STATE));
        MOUSE_STATE = {};
        XINPUT_PACKET_NUMBERS[0] = -1;
        XINPUT_PACKET_NUMBERS[1] = -1;
        XINPUT_PACKET_NUMBERS[2] = -1;
        XINPUT_PACKET_NUMBERS[3] = -1;
        __keyboard_device_initialize();
        __joystick_device_initialize();
        __mouse_device_initialize();
        // not sure about the rest
    }
    return true;
}

// Rx3B720
void __input_cleanup() {
    if (LPDIRECTINPUTDEVICE8 mouse_device = MOUSE_DEVICE) {
        mouse_device->Unacquire();
        MOUSE_DEVICE->Release();
        MOUSE_DEVICE = NULL;
    }
    if (LPDIRECTINPUTDEVICE8 keyboard_device = KEYBOARD_DEVICE) {
        keyboard_device->Unacquire();
        KEYBOARD_DEVICE->Release();
        KEYBOARD_DEVICE = NULL;
    }
    // not sure about the rest
    if (LPDIRECTINPUT8 dinput = DINPUT) {
        dinput->Release();
        DINPUT = NULL;
    }
}

// Rx3B7F0
bool __keyboard_device_initialize() {
    if (!KEYBOARD_DEVICE) {
        if (FAILED(DINPUT->CreateDevice(GUID_SysKeyboard, &KEYBOARD_DEVICE, NULL))) {
            return false;
        }
        if (FAILED(KEYBOARD_DEVICE->SetDataFormat(&c_dfDIKeyboard))) {
            return false;
        }
        if (FAILED(KEYBOARD_DEVICE->SetCooperativeLevel(__window_handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY))) {
            return false;
        }
        KEYBOARD_DEVICE->Acquire();
    }
    return true;
}

// Rx3B850
void __keyboard_device_get_state() {
    if (LPDIRECTINPUTDEVICE8 keyboard_device = KEYBOARD_DEVICE) {
        if (FAILED(keyboard_device->GetDeviceState(sizeof(KEYBOARD_STATE), KEYBOARD_STATE))) {
            KEYBOARD_DEVICE->Acquire();
            memset(KEYBOARD_STATE, 0, sizeof(KEYBOARD_STATE));
            return;
        }
        if (KEYBOARD_STATE[DIK_LALT] || KEYBOARD_STATE[DIK_RALT]) {
            KEYBOARD_STATE[DIK_RETURN] = 0;
        }
    }
}

// Rx3B8B0
bool __joystick_device_initialize() {
    if (LPDIRECTINPUT8 dinput = DINPUT) {
        if (FAILED(dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, &EnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY))) {
            return false;
        }
        // Not sure about the rest
    }
    return true;
}

// Rx3B930
void __joystick_device_get_state() {
    // idfk
}

// Rx3BF10
bool __mouse_device_initialize() {
    if (!MOUSE_DEVICE) {
        if (FAILED(DINPUT->CreateDevice(GUID_SysMouse, &MOUSE_DEVICE, NULL))) {
            return false;
        }
        if (FAILED(MOUSE_DEVICE->SetDataFormat(&c_dfDIMouse2))) {
            return false;
        }
        if (FAILED(MOUSE_DEVICE->SetCooperativeLevel(__window_handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
            return false;
        }
        DIPROPDWORD property = {
            .diph = {
                .dwSize = sizeof(DIPROPDWORD),
                .dwHeaderSize = sizeof(DIPROPHEADER),
                .dwObj = 0,
                .dwHow = DIPH_DEVICE
            },
            .dwData = DIPROPAXISMODE_REL
        };
        if (FAILED(MOUSE_DEVICE->SetProperty(DIPROP_AXISMODE, (LPCDIPROPHEADER)&property))) {
            return false;
        }
        MOUSE_DEVICE->Acquire();
    }
    return true;
}

inline void __mouse_device_get_state() {
    if (LPDIRECTINPUTDEVICE8 mouse_device = MOUSE_DEVICE) {
        if (FAILED(mouse_device->GetDeviceState(sizeof(MOUSE_STATE), &MOUSE_STATE))) {
            MOUSE_DEVICE->Acquire();
        }
    }
}

struct UnknownB {
    int __dword_0;
    int __dword_4;
    int __dword_8;

    // RxF1F0
    void thiscall __sub_rF1F0(std::string& arg1) {

    }
};

// Rx4DB61C
static UnknownB UNKNOWN_B;

// Rx3BFE0
BOOL PASCAL EnumDevicesCallback(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvref) {
    LPDIRECTINPUTDEVICE8 device = NULL;
    if (FAILED(DINPUT->CreateDevice(lpddi->guidInstance, &device, NULL))) {
        return DIENUM_STOP;
    }
    device->SetDataFormat(&c_dfDIJoystick);
    device->SetCooperativeLevel(__window_handle, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    DIDEVCAPS capabilities = {
        .dwSize = sizeof(DIDEVCAPS)
    };
    device->GetCapabilities(&capabilities);
    // Some unknown method call
    device->EnumObjects(&EnumDeviceObjectsCallback, device, DIDFT_AXIS);
    DIDEVICEINSTANCE device_info = {
        .dwSize = sizeof(DIDEVICEINSTANCE)
    };
    device->GetDeviceInfo(&device_info);
    std::string product_name = device_info.tszProductName;
    UNKNOWN_B.__sub_rF1F0(product_name);
    // Not sure about the rest
}

static constexpr auto kwjbrkwjb = offsetof(DIDEVICEINSTANCE, tszProductName);

// Rx3C320
BOOL PASCAL EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef) {

}

// Rx3D390
void __input_get_states() {
    __keyboard_device_get_state();
    __joystick_device_get_state();
    __mouse_device_get_state(); // this one got inlined
}