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
#include <ctype.h>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <deque>
#include <stack>
#include <queue>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#include <Windows.h>
#include <timeapi.h>

using namespace std::literals::string_literals;

#include <dinput.h>

#pragma comment (lib, "Ws2_32.lib")

#include "..\zero\util.h"

// size: 0x18
struct CRITICAL_SECTION_SMALL {
    dummy_fields(0x18);
};

// boost::system::error_code
struct UnknownA {
    int wsa_last_error;
    void* __ptr_4;
};

struct UnknownC {
    SOCKET socket;
	unsigned char __byte_4;
};

// This does something with TLS and is called everywhere. Seems to be something about error handling
dllexport gnu_noinline void* __sub_r2DBEE0() {
    // I haven't reversed this yet, returning rand just makes it compile
    return (void*)rand();
};

dllexport gnu_noinline void* __sub_r16F330() {
    // I haven't reversed this yet, returning rand just makes it compile
    return (void*)rand();
}

// socket_ops::socket
// https://github.com/boostorg/asio/blob/d6e7b5a547daaddfd19c548d2f602cb5b15361df/include/boost/asio/detail/impl/socket_ops.ipp#L1874C13-L1874C19
dllexport gnu_noinline SOCKET __create_socket_r170620(int af, int type, int protocol, UnknownA* unknown_a) {
    WSASetLastError(0);
    SOCKET sock = WSASocketW(af, type, protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
    void* local1 = __sub_r2DBEE0();
    int last_error = WSAGetLastError();
    unknown_a->wsa_last_error = last_error;
    unknown_a->__ptr_4 = local1;
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    if (af == AF_INET6) {
        uint32_t opt_value = 0;
		// 41 = IPPROTO_IPV6
		// 27 = IPV6_V6ONLY
        setsockopt(sock, 41, 27, (char*)&opt_value, sizeof(opt_value));
    }
    unknown_a->__ptr_4 = __sub_r2DBEE0();
    unknown_a->wsa_last_error = ERROR_SUCCESS;
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

	dllexport gnu_noinline UnknownA* __calls_create_socket_r170C70(
		UnknownA* unknown_a1,	// EBP+8
		UnknownC* unknown_c,	// EBP+C
		int af,	// EBP+10
		int type,	// EBP+14
		int protocol,	// EBP+18
		UnknownA* unknown_a2	// EBP+1C
	) {
		if (unknown_c->socket != INVALID_SOCKET) {
			void* idk = __sub_r16F330();
			unknown_a1->wsa_last_error = 1;
			unknown_a1->__ptr_4 = idk;
			unknown_a2->wsa_last_error = 1;
			unknown_a2->__ptr_4 = idk;
			return unknown_a1;
		}
		SOCKET socket = __create_socket_r170620(af, type, protocol, unknown_a2);
		if (socket == INVALID_SOCKET) {
			*unknown_a1 = *unknown_a2;
			return unknown_a1;
		}
		if (!CreateIoCompletionPort((HANDLE)socket, this->__handle_14, 0, 0)) {
			unknown_a2->wsa_last_error = GetLastError();
		} else {
			unknown_a2->wsa_last_error = 0;
		}
		unknown_a2->__ptr_4 = __sub_r2DBEE0();
		if (unknown_a2->wsa_last_error != 0) {
			*unknown_a1 = *unknown_a2;
			// TODO
			return unknown_a1;
		}
		unknown_c->socket = socket;
		switch (type) {
			default:
				unknown_c->__byte_4 = 0x00;
				break;
			case SOCK_STREAM:
				unknown_c->__byte_4 = 0x10;
				break;
			case SOCK_DGRAM:
				unknown_c->__byte_4 = 0x20;
				break;
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


// Rx12E4F0 (thiscall function, unknown type)
void* __sub_r12E4F0(
	HMODULE module, // EBP+8
	const char* func_name // EBP+C
) {
	
	IMAGE_DOS_HEADER* dos_header = NULL; // EBP-1C
	IMAGE_FILE_HEADER* file_header = NULL; // EBP-20
	IMAGE_OPTIONAL_HEADER* optional_header = NULL; // EBP-24
	
	dos_header = (IMAGE_DOS_HEADER*)module;
	file_header = based_pointer<IMAGE_FILE_HEADER>(module, dos_header->e_lfanew + offsetof(IMAGE_NT_HEADERS, FileHeader));
	optional_header = based_pointer<IMAGE_OPTIONAL_HEADER>(file_header, sizeof(IMAGE_FILE_HEADER));
	
	DWORD export_directory_rva = optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress; // EBP-2C
	
	void* base_address = (void*)module; // EBP-8
	
	IMAGE_EXPORT_DIRECTORY* export_directory = based_pointer<IMAGE_EXPORT_DIRECTORY>(base_address, export_directory_rva); // EBP-10
	
	DWORD* name_pointer_table = based_pointer<DWORD>(base_address, export_directory->AddressOfNames); // EBP-34
	
	DWORD* export_address_table = based_pointer<DWORD>(base_address, export_directory->AddressOfFunctions); // EBP-44
	
	WORD* ordinal_table = based_pointer<WORD>(base_address, export_directory->AddressOfNameOrdinals); // EBP-40
	
	DWORD number_of_names = export_directory->NumberOfNames; // EBP-30
	
	for (
		DWORD i = 0; // EBP-C
		i < number_of_names;
		++i
	) {
		const char* export_name = based_pointer<const char>(base_address, name_pointer_table[i]); // EBP-38
		int cmp_ret = strcmp(export_name, func_name); // EBP-3C
		if (cmp_ret == 0) {
			return based_pointer(base_address, export_address_table[ordinal_table[i]]);
		}
	}
	return NULL;
}

/*
se_libact.dll, se_trust.dll:
- init_instance_v2
- release_instance
- render
- render_preproc
- update_frame

se_lobby.dll, se_upnp.dll, se_information.dll:
- init_instance_v2
- release_instance
- update_frame
*/

typedef int stdcall release_instance_t();
typedef int stdcall update_frame_t();
typedef int stdcall render_preproc_t();
typedef int stdcall render_t(void* arg1, void* arg2);
typedef int stdcall init_instance_v2_t(void* arg1);

// size: 0x24
struct ActPluginData {
    unknown_fields(0x1); // 0x0, 0x8
    bool is_from_dat_archive; // 0x1, 0x9
    unknown_fields(0x2); // 0x2, 0xA
    void* init_instance; // 0x4, 0xC
    release_instance_t* release_instance; // 0x8, 0x10
    update_frame_t* update_frame; // 0xC, 0x14
    render_preproc_t* render_preproc; // 0x10, 0x18
    render_t* render; // 0x14, 0x1C
    init_instance_v2_t* init_instance_v2; // 0x18, 0x20
    unknown_fields(0x8); // 0x1C, 0x24
    // 0x24, 0x2C
};

// size: 0x2C
struct ActPlugin {
    // void* vtable; // 0x0
    void* base_address; // 0x4
    ActPluginData data; // 0x8
    // 0x2C
};

// size: 0x264
struct KiteSquirrelAPI {
    // void* vtable; // 0x0
    int __dword_4; // 0x4
    void* sq_open; // 0x8
    void* __func_C; // 0xC
    void* sq_seterrorhandler; // 0x10
    void* sq_close; // 0x14
    void* sq_setforeignptr; // 0x18
    void* sq_getforeignptr; // 0x1C
    int __dword_20; // 0x20
    int __dword_24; // 0x24
    int __dword_28; // 0x28
    int __dword_2C; // 0x2C
    int __dword_30; // 0x30
    int __dword_34; // 0x34
    void* sq_setprintfunc; // 0x38
    void* sq_getprintfunc; // 0x3C
    void* sq_geterrorfunc; // 0x40
    void* __func_44; // 0x44
    void* __func_48; // 0x48
    void* sq_getvmstate; // 0x4C
    void* __func_50; // 0x50
    void* __func_54; // 0x54
    void* sq_compilebuffer; // 0x58
    void* sq_enabledebuginfo; // 0x5C
    void* sq_notifyallexceptions; // 0x60
    void* sq_setcompilererrorhandler; // 0x64
    void* sq_push; // 0x68
    void* sq_pop; // 0x6C
    void* __func_70; // 0x70
    void* sq_remove; // 0x74
    void* sq_gettop; // 0x78
    void* sq_settop; // 0x7C
    void* __func_80; // 0x80
    void* sq_cmp; // 0x84
    void* sq_move; // 0x88
    void* __func_8C; // 0x8C
    void* sq_newtable; // 0x90
    void* __func_94; // 0x94
    void* __func_98; // 0x98
    void* sq_newclosure; // 0x9C
    void* sq_setparamscheck; // 0xA0
    void* __func_A4; // 0xA4
    int __dword_A8; // 0xA8
    int __dword_AC; // 0xAC
    void* sq_pushstring; // 0xB0
    void* sq_pushfloat; // 0xB4
    void* sq_pushinteger; // 0xB8
    void* sq_pushbool; // 0xBC
    void* sq_pushuserpointer; // 0xC0
    void* sq_pushnull; // 0xC4
    int __dword_C8; // 0xC8
    void* sq_gettype; // 0xCC
    void* __func_D0; // 0xD0
    void* sq_getsize; // 0xD4
    void* sq_gethash; // 0xD8
    void* sq_getbase; // 0xDC
    void* sq_instanceof; // 0xE0
    void* __func_E4; // 0xE4
    void* sq_tobool; // 0xE8
    void* sq_getstring; // 0xEC
    void* sq_getinteger; // 0xF0
    void* sq_getfloat; // 0xF4
    void* sq_getbool; // 0xF8
    void* sq_getthread; // 0xFC
    void* sq_getuserpointer; // 0x100
    void* sq_getuserdata; // 0x104
    void* sq_settypetag; // 0x108
    void* sq_gettypetag; // 0x10C
    void* sq_setreleasehook; // 0x110
    int __dword_114; // 0x114
    void* sq_getscratchpad; // 0x118
    void* __func_11C; // 0x11C
    void* __func_120; // 0x120
    void* __func_124; // 0x124
    void* sq_setnativeclosurename; // 0x128
    void* sq_setinstanceup; // 0x12C
    void* sq_getinstanceup; // 0x130
    void* __func_134; // 0x134
    void* sq_newclass; // 0x138
    void* __func_13C; // 0x13C
    void* __func_140; // 0x140
    void* __func_144; // 0x144
    void* __func_148; // 0x148
    void* __func_14C; // 0x14C
    void* __func_150; // 0x150
    void* __func_154; // 0x154
    void* __func_158; // 0x158
    void* __func_15C; // 0x15C
    void* sq_pushroottable; // 0x160
    void* sq_pushregistrytable; // 0x164
    void* sq_pushconsttable; // 0x168
    void* sq_setroottable; // 0x16C
    void* sq_setconsttable; // 0x170
    void* sq_newslot; // 0x174
    void* __func_178; // 0x178
    void* sq_set; // 0x17C
    void* sq_get; // 0x180
    void* sq_rawget; // 0x184
    void* __func_188; // 0x188
    void* __func_18C; // 0x18C
    void* __func_190; // 0x190
    void* __func_194; // 0x194
    void* __func_198; // 0x198
    void* __func_19C; // 0x19C
    void* sq_arraysize; // 0x1A0
    void* __func_1A4; // 0x1A4
    void* __func_1A8; // 0x1A8
    void* sq_arrayinsert; // 0x1AC
    void* sq_setdelegate; // 0x1B0
    void* sq_getdelegate; // 0x1B4
    void* sq_clone; // 0x1B8
    void* __func_1BC; // 0x1BC
    void* __func_1C0; // 0x1C0
    void* sq_getweakrefval; // 0x1C4
    void* __func_1C8; // 0x1C8
    void* sq_call; // 0x1CC
    void* sq_resume; // 0x1D0
    void* sq_getlocal; // 0x1D4
    void* sq_getcallee; // 0x1D8
    void* sq_getfreevariable; // 0x1DC
    void* sq_throwerror; // 0x1E0
    void* __func_1E4; // 0x1E4
    void* sq_reseterror; // 0x1E8
    void* sq_getlasterror; // 0x1EC
    void* __func_1F0; // 0x1F0
    void* sq_pushobject; // 0x1F4
    void* sq_addref; // 0x1F8
    void* sq_release; // 0x1FC
    void* sq_getrefcount; // 0x200
    void* sq_resetobject; // 0x204
    void* sq_objtostring; // 0x208
    void* sq_objtobool; // 0x20C
    void* sq_objtointeger; // 0x210
    void* sq_objtofloat; // 0x214
    void* sq_objtouserpointer; // 0x218
    void* __func_21C; // 0x21C
    int __dword_220; // 0x220
    void* sq_collectgarbage; // 0x224
    void* __func_228; // 0x228
    void* sq_writeclosure; // 0x22C
    void* __func_230; // 0x230
    void* sq_vm_malloc; // 0x234
    void* sq_vm_realloc; // 0x238
    void* sq_vm_free; // 0x23C
    void* sq_stackinfos; // 0x240
    void* __func_244; // 0x244
    void* sq_setnativedebughook; // 0x248
    int __dword_24C; // 0x24C
    int __dword_250; // 0x250
    int __dword_254; // 0x254
    void* sqstd_register_iolib; // 0x258
    int __dword_25C; // 0x25C
    void* sqstd_seterrorhandlers; // 0x260
    void* sqstd_printcallstack; // 0x264
    // 0x268
};


namespace TF4 {

    // Rx2EA00
    static inline size_t stdcall file_tell(const HANDLE& file) {
        return SetFilePointer(file, 0, NULL, FILE_CURRENT);
    }

    // Rx2EA50
    static inline size_t stdcall file_size(const HANDLE& file) {
        return GetFileSize(file, NULL);
    }

    // Rx2EAC0
    static size_t stdcall file_seek(const HANDLE& file, const ssize_t& offset, const int& origin) {
        switch (origin) {
            default:
                return 0;
            case SEEK_SET:
                return SetFilePointer(file, offset, NULL, FILE_BEGIN);
            case SEEK_CUR:
                return SetFilePointer(file, offset, NULL, FILE_CURRENT);
            case SEEK_END:
                return SetFilePointer(file, offset, NULL, FILE_END);
        }
    }

    // Rx2EB70
    static inline BOOL stdcall file_read(const HANDLE& file, void *const& buffer, const size_t& size, size_t *const& bytes_read) {
        return ReadFile(file, buffer, size, (LPDWORD)bytes_read, NULL) != FALSE;
    }

    // Rx2EC60
    static inline HANDLE stdcall file_open(const char *const& path) {
        return CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    }

    // This is some custom version of std::hash that I don't feel like figuring out
    static gnu_noinline uint32_t fastcall hash_path(const char* path, size_t length, uint32_t hash_constant = 2166136261) {
        return (uintptr_t)path * (uintptr_t)length * hash_constant * rand();
    }

    // size: 8
    struct PakDataInnerA {
        int __dword_0; // 0x0
        int __dword_8; // 0x4
        // 0x8

        // Rx24410
        void thiscall __sub_r24410(int* arg1, PakDataInnerA* self) {
            
        }

        // Rx244A0
        void* thiscall __sub_r244A0() {

        }
    };

    // size: 0x18
    struct PakDataInnerB {
        std::string __string_0; // 0x0
        unknown_fields(0x8); // 0x10
        // 0x18
    };
    
    // size: 0xC
    struct PakDataInnerC {
        int __dword_0; // 0x0
        int __dword_4; // 0x4
        int __dword_8; // 0x8
        // 0xC
    };
    
    // size: 8
    struct PakDataInnerD {
        int __dword_0; // 0x0
        int __dword_4; // 0x4
        // 0x8
    };
    
    // size: 0xC
    struct PakDataInnerE {
        int __dword_0; // 0x0
        int __dword_4; // 0x4
        int __dword_8; // 0x8
        // 0xC
    };

    struct PakDataInfo {
        size_t __innerB_index; // 0x0
        uint32_t file_name_hash; // 0x4
        uint32_t __int_8; // 0x8
        uint32_t __int_C; // 0xC
        size_t file_size; // 0x10
        uint32_t key[4]; // 0x14
        // 0x24
    };

    struct PakData {

        PakDataInnerB* __pak_innerB_ptr_0; // 0x0
        unknown_fields(0x8); // 0x4
        PakDataInnerC __pak_innerC_C; // 0xC
        size_t __size_18; // 0x18
        size_t __size_1C; // 0x1C
        PakDataInnerA __pak_innerA_20; // 0x20
        PakDataInnerD __pak_innerD_28; // 0x28
        PakDataInnerE __pak_innerE_30; // 0x30
        // 0x3C

        // Rx243B0
        gnu_noinline PakDataInfo* thiscall __sub_r243B0(const char* path) {
            -this->__sub_r26300(path);

            return (PakDataInfo*)rand() + (uintptr_t)path;
        }

        // Rx26120
        gnu_noinline bool thiscall __sub_r26120(const char* path, size_t length) {
            // This is another hash related thing maybe?
            return (uintptr_t)path * length != rand();
        }

        // Rx26300
        int32_t thiscall __sub_r26300(const char* path) {
            if (!path) {
                return 0;
            }

            size_t path_length = strlen(path);

            if (!path_length) {
                return 0;
            }

            if (
                path_length > this->__size_1C &&
                (
                    path[1] == ':' ||
                    (path[0] == '\\' && path[1] == '\\')
                )
            ) {
                path_length -= this->__size_1C;
                path += this->__size_1C;
                return hash_path(path, path_length);
            }
            if (path[0] == '.' && path[1] == '\\') {
                path_length -= this->__size_18 + 2;
                path += this->__size_18 + 2;
            }
            return hash_path(path, path_length);
        }
    };

    // Rx4DAD18
    static PakData* PAK_DATA_PTR;

    // size: 0x10014
    struct FileReader {
        // void* vftable; // 0x0
        HANDLE file; // 0x4
        uint8_t buffer[0x10000]; // 0x8
        size_t buffer_filled; // 0x10008
        size_t buffer_offset; // 0x1000C
        size_t __int_10010; // 0x10010
        // 0x10014

        // Method 0
        // Rx2ED70
        virtual ~FileReader() {
            if (this->file != INVALID_HANDLE_VALUE) {
                CloseHandle(this->file);
            }
        }

        // Method 4
        // Rx2EE00
        virtual bool thiscall open(const std::string& path) {
            return this->open(path.c_str());
        }

        // Method 8
        // Rx2DE10
        virtual bool thiscall open(const char* path) {
            if (!path) {
                return false;
            }
            HANDLE handle = file_open(path);
            this->buffer_filled = 0;
            this->file = handle;
            this->buffer_offset = 0;
            return handle != INVALID_HANDLE_VALUE;
        }

        // Method C
        // Rx2DEE0
        virtual void thiscall close() {
            if (this->file != INVALID_HANDLE_VALUE) {
                CloseHandle(this->file);
                this->file = INVALID_HANDLE_VALUE;
            }
        }

        // Method 10
        // Rx2DF30
        virtual bool thiscall read(void* out, size_t size) {
            uint8_t* out_write = (uint8_t*)out;

            if (this->buffer_offset + size > this->buffer_filled) {

                size_t size_from_buffer = this->buffer_filled - this->buffer_offset;
                if (size_from_buffer) {
                    memcpy(out_write, &this->buffer[this->buffer_offset], size_from_buffer);
                }

                size_t remaining_size = size - size_from_buffer;
                this->buffer_offset = remaining_size;

                if (remaining_size >= sizeof(this->buffer)) {
                    file_read(this->file, &out_write[size_from_buffer], remaining_size, &this->__int_10010);
                    this->__int_10010 += size_from_buffer;
                    this->buffer_offset = 0;
                    this->buffer_filled = 0;
                    return this->__int_10010 == size;
                }
                else {
                    // Shouldn't this be passing remaining_size instead of buffer size?
                    file_read(this->file, this->buffer, sizeof(this->buffer), &this->buffer_filled);

                    // buffer_offset currently holds remaining_size
                    if (this->buffer_offset > this->buffer_filled) {
                        memcpy(&out_write[size_from_buffer], this->buffer, this->buffer_filled);
                        this->buffer_offset = this->buffer_filled;
                        this->__int_10010 = size_from_buffer + this->buffer_filled;
                        return false;
                    }
                    else {
                        memcpy(&out_write[size_from_buffer], this->buffer, this->buffer_offset);

                        // buffer_offset doesn't get fixed here?
                        this->__int_10010 = size;
                        return true;
                    }
                }
            }
            else {
                memcpy(out_write, &this->buffer[this->buffer_offset], size);
                this->__int_10010 = size;
                this->buffer_offset += size;
                return true;
            }
        }

        inline size_t seek_impl(ssize_t offset, int origin) {
            this->buffer_offset = 0;
            this->buffer_filled = 0;
            return file_seek(this->file, offset, origin);
        }

        // Method 14
        // Rx2E090
        virtual size_t thiscall seek(ssize_t offset, int origin) {
            if (origin == SEEK_CUR) {
                ssize_t combined_offset = this->buffer_offset - this->buffer_filled + offset;
                return this->seek_impl(combined_offset, SEEK_CUR);
            }
            else {
                return this->seek_impl(offset, origin);
            }
        }
    };

    // size: 0x10044
    struct PackageReader : FileReader {
        // FileReader base; // 0x0
        uint32_t __int_10014; // 0x10014
        size_t file_size; // 0x10018
        uint32_t file_name_hash; // 0x1001C
        uint32_t __int_10020; // 0x10020
        size_t offset; // 0x10024
        union {
            uint32_t key[5]; // 0x10028
            uint8_t key_bytes[20];
        };
        union {
            uint32_t aux; // 0x1003C
            uint8_t aux_bytes[4];
            uint16_t aux_words[2];
        };
        uint32_t aux_mask; // 0x10040
        // 0x10044

        // Method 0
        // RxAD90
        virtual ~PackageReader() {
        }

        static uint32_t fastcall hash_file_path(const char* path) {
            if (!path) {
                return 0;
            }
            size_t length = strlen(path);
            if (
                length >= 2 &&
                path[0] == '.' &&
                path[1] == '/' || path[1] == '\\'
            ) {
                length -= 2;
                path += 2;
            }
            return hash_path(path, length);
        }

        // Rx23E50
        bool thiscall open(const char* path, PakData* pak_data) {
            if (pak_data) {
                PakDataInfo* pak_info = pak_data->__sub_r243B0(path);
                if (!pak_info) {
                    if (
                        path &&
                        pak_data->__sub_r26120(path, strlen(path)) &&
                        this->FileReader::open(path)
                    ) {
                        this->__int_10014 = 0;
                        // this->file_size = IDK
                        this->file_name_hash = hash_file_path(path);
                        this->__int_10020 = 0;
                        memset(this->key, 0, sizeof(this->key));
                        this->aux = 0;
                        this->offset = 0;
                        this->FileReader::seek(this->__int_10014, SEEK_SET);
                        this->aux_mask = 0;
                        return true;
                    }
                    return false;
                }
                this->FileReader::open(pak_data->__pak_innerB_ptr_0[pak_info->__innerB_index].__string_0.c_str());
                if (this->file == INVALID_HANDLE_VALUE) {
                    return false;
                }
                this->__int_10014 = pak_info->__int_C;
                this->file_size = pak_info->file_size;
                this->file_name_hash = pak_info->file_name_hash;
                this->__int_10020 = pak_info->__int_8;
                memcpy(this->key, pak_info->key, sizeof(pak_info->key));
                this->aux = this->key[4] = this->key[0];
                this->aux_mask = ~0;
            }
            else {
                this->FileReader::open(path);
                if (this->file == INVALID_HANDLE_VALUE) {
                    return false;
                }
                // this->file_size = IDK
                this->file_name_hash = hash_file_path(path);
                this->__int_10020 = 0;
                memset(this->key, 0, sizeof(this->key));
                this->aux = 0;
                this->aux_mask = 0;
            }
            this->offset = 0;
            this->seek_impl(this->__int_10014, SEEK_SET);
            this->key[0] = -this->key[0];
            this->key[1] = -this->key[1];
            this->key[2] = -this->key[2];
            this->aux = this->key[3] = -this->key[3];
            return true;
        }

        // Method 4
        // RxAD30
        virtual bool thiscall open(const std::string& path) {
            return this->open(path.c_str(), PAK_DATA_PTR);
        }

        // Method 8
        // RxAD10
        virtual bool thiscall open(const char* path) {
            return this->open(path, PAK_DATA_PTR);
        }

        // Method 10
        // Rx24100
        virtual bool thiscall read(void* out, size_t size) {
            size_t remaining_size = this->file_size - this->offset;

            size_t requested_end = this->offset + size;

            size_t read_size = requested_end <= this->file_size ? size : remaining_size;

            if (!read_size) {
                this->__int_10010 = 0;
                return false;
            }

            this->FileReader::read(out, read_size);

            uint8_t* decrypt = ((uint8_t*)out) - this->offset;
            size_t end_offset = this->offset + read_size;

            if (!is_byte_aligned_to_word(read_size)) {
                uint8_t temp = decrypt[this->offset] & this->aux_mask;

                decrypt[this->offset] ^= this->key_bytes[this->offset & 0xF] ^ this->aux;

                ++this->offset;
                this->aux >>= bitsof(uint8_t);
                this->aux_bytes[3] = temp;
            }

            if (!is_word_aligned_to_dword(read_size)) {
                uint16_t temp = *(uint16_t*)&decrypt[this->offset] & this->aux_mask;

                *(uint16_t*)&decrypt[this->offset] ^= *(uint16_t*)&this->key_bytes[this->offset & 0xF] ^ this->aux;

                this->offset += 2;
                this->aux >>= bitsof(uint16_t);
                this->aux_words[1] = temp;
            }

            while (this->offset < end_offset) {
                uint32_t temp = *(uint32_t*)&decrypt[this->offset] & this->aux_mask;

                *(uint32_t*)&decrypt[this->offset] ^= *(uint32_t*)&this->key_bytes[this->offset & 0xF] ^ this->aux;

                this->offset += 4;
                this->aux = temp;
            }

            this->__int_10010 = read_size;
            return true;
        }

        template <typename T>
        inline T read() {
            T temp{};
            this->PackageReader::read(&temp, sizeof(temp));
            return temp;
        }

        // Method 14
        // Rx24250
        virtual size_t thiscall seek(ssize_t offset, int origin) {
            switch (origin) {
                default:
                    return SIZE_MAX;
                case SEEK_CUR:
                    this->offset += offset;
                    break;
                case SEEK_END:
                    offset += this->file_size;
                case SEEK_SET:
                    this->offset = offset;
            }
            origin = SEEK_SET;


            if (this->offset >= sizeof(uint32_t)) {
                offset = this->offset - sizeof(uint32_t) + this->__int_10014;

                size_t ret = this->seek_impl(offset, origin);

                this->FileReader::read(&this->aux, sizeof(uint32_t));

                this->aux &= this->aux_mask;

                return ret + sizeof(uint32_t);
            }
            else {
                this->aux = this->key[0] >> (this->offset << 3 & 0x1F);

                size_t ret = this->seek_impl(this->__int_10014, origin);

                this->FileReader::read(&this->aux_bytes[4 - this->offset], this->offset);

                this->aux &= this->aux_mask;

                return ret + this->offset;
            }
        }

        // Method 18
        // RxAD50
        virtual size_t thiscall get_file_size() {
            return this->file_size;
        }

        // Method 1C
        // RxAD60
        virtual size_t thiscall get_offset() {
            return this->offset;
        }

        uint32_t thiscall get_file_hash_impl() {
            if (this->file == INVALID_HANDLE_VALUE) {
                return 0;
            }
            size_t old_offset = this->get_offset();
            this->seek(0, SEEK_SET);

            uint8_t data[1024]{};
            this->read(data, sizeof(data));

            // This is probably also an std::hash variant
            uint32_t hash = 2166136261;
            while (this->__int_10010) {
                for (size_t i = 0; i < this->__int_10010; ++i) {
                    hash ^= data[i];
                    hash *= 16777619u;
                }
                this->read(data, sizeof(data));
            }

            this->seek(old_offset, SEEK_SET);

            return hash;
        }

        // Method 20
        // RxAD70
        virtual uint32_t thiscall get_file_hash() {
            if (!this->__int_10020) {
                return this->get_file_hash_impl();
            }
            return this->__int_10020;
        }

        // Method 24
        // RxAD80
        virtual uint32_t thiscall get_file_name_hash() {
            return this->file_name_hash;
        }

        // Rx42300
        int32_t thiscall read_int() {
            return this->read<int32_t>();
        }

        // Rx42320
        float thiscall read_float() {
            return this->read<float>();
        }

        // Rx42340
        bool thiscall read_bool() {
            return this->read<bool>();
        }
    };
}

namespace Manbow {
    struct SqFileReader : TF4::PackageReader {

        // Method 8
        // Rx422E0
        virtual bool thiscall open(const char* path) {
            return this->TF4::PackageReader::open(path, NULL);
        }
    };
}

#include <mutex>

enum SocketState : int32_t {
    SOCKET_CLOSED = 0,
    SOCKET_STATE_1 = 1,
    SOCKET_CONNECTED = 2,
    SOCKET_STATE_3 = 3,
    SOCKET_STATE_4 = 4
};

enum LobbyState : int32_t {
    LOBBY_CLOSED = 0,
    LOBBY_NO_OPERATION = 0,
    LOBBY_CONNECTING = 1,
    LOBBY_CONNECTED = 2,
    LOBBY_WAIT_INCOMING = 100,
    LOBBY_BUSY = 101,
    LOBBY_STATE_102 = 102,
    LOBBY_MATCHING = 200,
};

// size: 0x8
struct AsyncTcpSSLClientInnerA {
    int __dword_0; // 0x0
    int __dword_4; // 0x4
    // 0x8
};

struct AsyncTcpSSLClientInnerB {

};

// size: 0x298
struct AsyncTcpSSLClient {
    // void* vftable; // 0x0
    // 0x4
    SocketState socket_state; // 0x8
    SOCKET socket; // 0xC
	// 0x10
	
    uint32_t __uint_114; // 0x114
    SOCKET __socket_array_118[64]; // 0x118
    AsyncTcpSSLClientInnerA* __innerA_ptr_218; // 0x218
    bool __enable_ssl; // 0x21C
	// 0x21D
    uint32_t __time_220; // 0x220
    // 0x224
    long long __longlong_228; // 0x228
    long long __longlong_230; // 0x230
    std::string server_string; // 0x238
    std::string port_string; // 0x250
    std::mutex __mutex_268; // 0x268
    // 0x298

    AsyncTcpSSLClient() {
        this->socket_state = SOCKET_CLOSED;
        this->__time_220 = 0;
        this->__innerA_ptr_218 = new AsyncTcpSSLClientInnerA();
        this->__innerA_ptr_218->__dword_0 = 0;
        this->__innerA_ptr_218->__dword_4 = 0;
        this->__longlong_230 = 10;
    }

    // Method 0
    // Rx1FF30
    virtual ~AsyncTcpSSLClient() {
        if (this->__innerA_ptr_218) {
            delete this->__innerA_ptr_218;
        }
    }

    // Method 4
    // Rx20560
    virtual SocketState __method_4() {
        return this->socket_state;
    }

    // Method 8
    // Rx1FF70
    virtual int close() {
        if (this->socket_state == SOCKET_CLOSED) {
            return 0;
        }
        if (this->socket_state == SOCKET_STATE_1) {
            this->socket_state = SOCKET_STATE_4;
            return 0;
        }
        this->__time_220 = timeGetTime();
        this->socket_state = SOCKET_STATE_3;
        if (this->__innerA_ptr_218) {

        }
        closesocket(this->socket);
        this->socket_state = SOCKET_CLOSED;
        return 0;
    }

    // Method C
    // Rx15B0
    virtual void __method_C() {
    }

    // Rx20060
    void __sub_r20060(const char* server, const char* port, bool arg3) {

        if (this->socket_state != SOCKET_CLOSED) {
            this->close();
        }
        this->socket_state = SOCKET_STATE_1;
        this->__enable_ssl = false;
        this->server_string = server;
        this->port_string = port;
    }

    // Rx20160
    void __sub_r20160(const char* server, const char* port) {
        // lock mutex
        server = this->server_string.c_str();
        port = this->port_string.c_str();
        this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
        hostent* host = gethostbyname(server);
        if (!host) {
            this->close();
            // unlock mutex
            return;
        }
        sockaddr_in addr = {};

        addr.sin_family = AF_INET;
        addr.sin_port = htons(atoi(port));
        ((uint8_t*)&addr.sin_addr)[0] = host->h_addr[0];
        ((uint8_t*)&addr.sin_addr)[1] = host->h_addr[1];
        ((uint8_t*)&addr.sin_addr)[2] = host->h_addr[2];
        ((uint8_t*)&addr.sin_addr)[3] = host->h_addr[3];
        if (FAILED(connect(this->socket, (const sockaddr*)&addr, sizeof(sockaddr_in)))) {
            this->close();
            // unlock mutex
            return;
        }

        // this loop looks broken...
        this->__uint_114 = 0;
        do {
            size_t i = 0;
            for (; i < this->__uint_114; ++i) {
                if (this->__socket_array_118[i] == this->socket) {
                    break;
                }
            }
            if (
                i != this->__uint_114 &&
                this->__uint_114 < countof(this->__socket_array_118)
            ) {
                this->__socket_array_118[i] = this->socket;
                ++this->__uint_114;
            }
        } while (false);

        // this->__longlong_228 = ??? + this->__longlong_230;

        this->__enable_ssl = false;

        // SSL init


    }

    // Rx207F0
    void __sub_r207F0(long long arg1) {
        this->__longlong_230 = arg1;
    }
};

// size: 0x8
struct AsyncLobbyClientInnerA {
    int __dword_0; // 0x0
    int __dword_4; // 0x4
    // 0x8
};

// size: 0x8
struct AsyncLobbyClientInnerB {
    int __dword_0; // 0x0
    int __dword_4; // 0x4
    // 0x8
};

static std::string HOST_STRING;
static std::string MATCH_USER_DATA_STRING;

static inline constexpr int RPL_ENDOFMOTD = 376;
static inline constexpr int ERR_NOMOTD = 422;
static inline constexpr int ERR_NICKNAMEINUSE = 433;

struct StatParams {
    int peer_state; // 0x0
    int __param_2; // 0x4
    int __time_8; // 0x8
    int __param_3; // 0xC
    // 0x10
    std::string nick_string; // 0x14
    std::string host_string; // 0x2C
    int __int_44; // 0x44
    // 0x48
};

// size: 0x4F8
struct AsyncLobbyClient : AsyncTcpSSLClient {
    // AsyncTcpSSLClient base; // 0x0
    // 0x298
    AsyncLobbyClientInnerA __innerA_298; // 0x298
    CRITICAL_SECTION_SMALL __crit_section_2A0; // 0x2A0
    AsyncLobbyClientInnerB __innerB_2B8; // 0x2B8
    CRITICAL_SECTION_SMALL __crit_section_2C0; // 0x2C0
    std::string __last_host_used; // 0x2D8
    std::string lobby_prefix; // 0x2F0
    std::string current_nick; // 0x308
    std::string __string_320; // 0x320
    std::string current_channel; // 0x338
    std::string __lobby_nameA; // 0x350
    std::string password; // 0x368
    std::string __lobby_nameB; // 0x380
    std::string version_signature; // 0x398
    std::string user_data_str; // 0x3B0 external_port as string
    std::string match_host; // 0x3C8
    std::string __match_user_data_str; // 0x3E0 opponent port?
    uint32_t external_port; // 0x3F8
    int __dword_3FC; // 0x3FC
    int __int_400; // 0x400
    int __int_404; // 0x404
    int __dword_408; // 0x408
    AsyncLobbyClientInnerB __innerB_40C; // 0x40C
    int __dword_414; // 0x414
    uint32_t __time_418; // 0x418
    int __dword_41C; // 0x41C
    uint8_t __byte_420; // 0x420
    // 0x421
    std::string __string_424; // 0x424 opponent nick?
    char __byte_43C; // 0x43C
    // 0x43D
    std::string __recv_str; // 0x440
    //AsyncLobbyClientInnerC __innerC_458; // 0x458
    std::queue<std::string> __queue_458; // 0x458
    // 0x470

    LobbyState __lobby_state; // 0x4EC
    int32_t max_nick_length; // 0x4F0
    // 0x4F4

    AsyncLobbyClient() {
        this->lobby_prefix = "irc_";
        this->version_signature = "-";
        this->external_port = 10800;
        this->set_lobby_user_state(LOBBY_CLOSED);
        this->__dword_3FC = 0;
        this->__dword_41C = 0;
        this->__byte_420 = 0;
        this->__time_418 = 0;
        this->__dword_414 = 0;
        this->__byte_43C = 0;
        this->max_nick_length = 10;
        this->set_strike_factor(1000, 1000);
    }

    // Method 8
    virtual int close() {
        this->AsyncTcpSSLClient::close();
        this->current_nick = "";
        this->__string_320 = "";
        this->current_channel = "";
        this->password = "";
        this->__lobby_nameA = "";
        this->__lobby_nameB = "";
        this->match_host = "";
        this->__match_user_data_str = "";
        // more random crap

        this->set_lobby_user_state(LOBBY_CLOSED);
    }

    // Method 10
    // Rx4740
    virtual LobbyState get_lobby_user_state() {
        if (this->socket_state != SOCKET_CONNECTED) {
            return LOBBY_CLOSED;
        }
        if (
            this->__lobby_state == LOBBY_WAIT_INCOMING &&
            this->__byte_420 != 0
        ) {
            return LOBBY_STATE_102;
        }
        return this->__lobby_state;
    }

    // Method 14
    // Rx4790
    virtual void set_lobby_user_state(LobbyState arg) {
        this->__lobby_state = arg;
        this->__dword_408 = 0;
        this->__time_418 = 0;
        if (arg == LOBBY_WAIT_INCOMING) {
            this->__dword_3FC = 0;
            this->__dword_41C = 0;
            this->__byte_420 = 0;
        }
        if (arg == LOBBY_MATCHING) {
            this->match_host = "";
            this->__match_user_data_str = "";
            this->__dword_414 = 0;
        }
    }

    // Method 18
    // Rx4830
    virtual void set_strike_factor(int arg1, int arg2) {
        this->__int_400 = arg1;
        this->__int_404 = arg2;
    }

    // Rx4720
    void set_external_port(uint32_t port) {
        this->external_port = port;
    }

    // Rx46F0
    void set_prefix(const char* str) {
        if (!str) {
            str = "irc_";
        }
        this->lobby_prefix = str;
    }

    // Rx4860
    const char* get_match_host() {
        HOST_STRING = this->match_host;
        this->match_host = "";
    }

    // Rx49A0
    const char* get_current_channel() {
        if (
            this->current_channel[0] == '#' ||
            this->current_channel[0] == '@'
        ) {
            return this->current_channel.c_str() + this->lobby_prefix.length() + 1;
        }
        else {
            return this->current_channel.c_str();
        }
    }

    // Rx4A10
    const char* get_current_nick() {
        return this->current_nick.c_str() + this->lobby_prefix.length();
    }

    // Rx4A40
    int count_other_user() {

    }

    // Rx4B70
    void join_channel() {

    }

    // Rx4670
    void set_max_nick_length(int length) {
        this->max_nick_length = length;
        if (this->max_nick_length > 32) {
            this->max_nick_length = 32;
        }
        if (this->max_nick_length < 9) {
            this->max_nick_length = 9;
        }
    }

    // Rx46C0
    void set_version_sig(const char* str) {
        this->version_signature = str;
    }

    // Rx8A50
    void set_user_data(const char* str) {
        this->user_data_str = str;
    }

    // Rx4900
    void get_match_user_data() {
        MATCH_USER_DATA_STRING = this->__match_user_data_str;
        this->__match_user_data_str = "";
    }

    // Rx53B0
    int connect(const char* server, const char* port, const char* password, const char* lobby_nameA, const char* lobby_nameB) {
        if (this->socket_state != SOCKET_CLOSED) {
            this->close();
        }
        this->current_nick = "";
        this->__string_320 = "";
        this->password = password;
        this->current_channel = "";
        this->__lobby_nameA = lobby_nameA;
        this->__lobby_nameB = lobby_nameB;
        this->match_host = "";
        this->__match_user_data_str = "";
        this->__byte_43C = 0;
        this->set_lobby_user_state(LOBBY_CLOSED);
        // crit section things

        if (
            this->__lobby_nameA[0] != '#' &&
            this->__lobby_nameA[0] != '@'
        ) {

        }
        else {

        }

        if (
            this->__lobby_nameA[0] != '#' &&
            this->__lobby_nameA[0] != '@'
        ) {

        } else {

        }

        this->__sub_r207F0(600);

        this->__sub_r20060(server, port, true);
    }
	
    // Rx69C0
    void __recv() {

        char bufferA[4096];
        char bufferB[4096];
        while (this->socket_state == SOCKET_CONNECTED) {
            memset(bufferA, 0, 4095);

            int recv_length = recv(this->socket, bufferA, 4095, 0); // real call is more complicated
            if (recv_length >= sizeof(bufferA)) {
                this->close();
            }
            if (recv_length > 0) {
                memcpy(bufferB, bufferA, recv_length);
                if (recv_length > sizeof(bufferB)) {
                    // abort or something?
                }
                bufferB[recv_length] = '\0';
                this->__recv_str += bufferB;

                while (true) {
                    size_t i = 0; // LOCAL 2069
                    const char* recv_read = this->__recv_str.c_str(); // LOCAL 2068

                    while (
                        *recv_read != '\0' &&
                        *recv_read != '\n'
                    ) {
                        ++i;
                        ++recv_read;
                    }

                    if (*recv_read == '\n') {
                        std::string message = this->__recv_str.substr(0, i);
                        this->__recv_str = this->__recv_str.substr(i + 1);
                        if (isprint(*message.c_str())) {
                            this->__queue_458.push(message);
                        }
                        this->__parse_recv();
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }

	// Rx6C40
	void __parse_recv() {
        if (this->socket_state == SOCKET_CONNECTED) {
            char* buffer = new char[655535]; // This looks like an extra 5 as a typo

            char*& buffer_ref = buffer; // LOCAL 81

            memset(buffer, 0, 65535);

            while (this->__queue_458.size() > 0) {

                std::string local11{ this->__queue_458.front() };
                this->__queue_458.pop();

                memcpy(buffer_ref, local11.c_str(), local11.length());

                if (this->__string_320 == "") {
                    // send NICK/PASS/USER
                }

                char* buffer_ptrA = buffer_ref; // local82
                char* buffer_ptrB = buffer_ref; // local80

                while (*buffer_ptrA != '\0') {
                    if (
                        *buffer_ptrB == '\r' ||
                        *buffer_ptrB == '\n' ||
                        *buffer_ptrB == '\0'
                    ) {
                        *buffer_ptrB = '\0';
                        this->__parse_message(buffer_ptrB);
                        buffer_ptrA = buffer_ptrB + 1;
                    }
                    ++buffer_ptrA;


                    if (this->socket_state != SOCKET_CONNECTED) {
                        delete buffer_ref; // clean up LOCAL 81
                        return;
                    }
                }
                if (this->socket_state != SOCKET_CONNECTED) {
                    delete buffer_ref; // clean up LOCAL 81
                    return;
                }
            }
            delete buffer_ref; // clean up LOCAL 81
        }
	}

    // Rx7230
    void __parse_message(const char* message) {
        if (!message || !message[0]) {
            return;
        }
        if (this->socket_state != SOCKET_CONNECTED) {
            return;
        }

        std::vector<std::string> message_parts;

        // make stringstream from message
        for (;;) {
            // call getline? and append strings to vector
        }

        if (
            message_parts.size() > 3 &&
            message_parts[0].length() > 0 &&
            *message_parts[0].c_str() == ':'
        ) {
            int code = atoi(message_parts[1].c_str());
            switch (code) {
                case ERR_NICKNAMEINUSE: {
                    // std::string nick_str = "NICK " + ???
                    // send str
                    break;
                }
                case RPL_ENDOFMOTD: case ERR_NOMOTD: {
                    if (this->__lobby_nameA != "") {
                        std::string join_str = "JOIN " + this->__lobby_nameA + (this->__lobby_nameB == "" ? ""s : " "s + this->__lobby_nameB) + "\r\n";
                        // send str

                        // ???
                    }
                    break;
                }
                default:
                    if (code >= 400 && code < 500) {
                        this->__byte_43C = 1;
                        this->socket_state = SOCKET_STATE_4;
                        return;
                    }
                    break;
            }
        }

        std::string message_nick; // LOCAL 10
        std::string message_host; // LOCAL 16
        if (message_parts.size() > 0) {
            message_nick = message_parts[0];
            size_t offset = message_nick.find("!");
            if (offset != std::string::npos) {
                message_nick = message_nick.substr(message_nick[0] == ':', offset);
            }
            else {
                message_nick = "";
            }
            message_host = message_parts[0];
            offset = message_host.find("@");
            if (offset != std::string::npos) {
                message_host = message_host.substr(offset + 1, message_host.length());
            }
            else {
                message_host = "";
            }
        }

        if (
            message_parts.size() > 2 &&
            message_parts[1] == "JOIN" &&
            message_nick == this->current_nick
        ) {
            this->current_channel = message_parts[2];
            this->__lobby_nameA = "";

            if (this->current_channel[0] == ':') {
                this->current_channel = this->current_channel.substr(1, this->current_channel.length());
            }
        }

        if (
            message_parts.size() > 2 &&
            message_parts[0].find("!") == std::string::npos &&
            message_parts[0].find("@") == std::string::npos &&
            message_parts[1] == "MODE" &&
            message_parts[2] == this->current_channel
        ) {
            std::string mode_str = "MODE" + this->current_channel + " +snt" + (this->__lobby_nameB == "" ? ""s : "+k " + this->__lobby_nameB) + "\r\n";
            // send str
        }

        if (
            message_parts.size() > 0 &&
            message_parts[0] == "PING"
        ) {
            // send "PONG 0\r\n"
        }

        if (
            message_parts.size() > 0 &&
            message_parts[0] == "ERROR"
        ) {
            this->socket_state = SOCKET_STATE_4;
            return;
        }

        if (
            message_parts.size() > 2 &&
            message_parts[1] == "PRIVMSG" &&
            (
                message_parts[2] == this->current_channel ||
                message_parts[2] == ":" + this->current_channel
            )
        ) {
            std::string temp{ message };

            size_t offset = 0;
            offset += message_parts[0].length() + 1;
            offset += message_parts[1].length() + 1;
            offset += message_parts[2].length() + 1;

            if (temp[offset] == ':') {
                ++offset;
            }

            temp = temp.substr(offset, temp.length());

            this->__parse_privmsg(message_nick.c_str(), message_host.c_str(), temp.c_str());
        }
    }

    // Rx804A
    void __parse_privmsg(const char* nick, const char* host, const char* message) {
        if (!nick || !nick[0]) {
            return;
        }
        if (!host || !host[0]) {
            return;
        }
        if (!message || !message[0]) {
            return;
        }

        std::vector<std::string> message_parts;

        // make stringstream from message
        for (;;) {
            // call getline? and append strings to vector
        }

        if (
            message_parts.size() > 4 &&
            message_parts[0] == "STAT" &&
            message_parts[4] == this->version_signature
        ) {
            StatParams stat_params; // LOCAL 540
            stat_params.peer_state = atoi(message_parts[1].c_str());
            stat_params.__param_2 = atoi(message_parts[2].c_str());
            stat_params.__param_3 = atoi(message_parts[3].c_str());
            stat_params.__time_8 = timeGetTime();
            stat_params.nick_string = nick;
            stat_params.host_string = host;
            stat_params.__int_44 = 1000;
            if (message_parts.size() > 5) {
                stat_params.__int_44 = atoi(message_parts[5].c_str());
            }
        }

        if (
            this->get_lobby_user_state() == LOBBY_WAIT_INCOMING &&
            message_parts.size() > 2 &&
            message_parts[0] == "REQUEST_MATCH" &&
            message_parts[1] == this->current_nick
        ) {
            int current_time = timeGetTime();
            int time_since_last = current_time - this->__time_418;
            if (!this->__byte_420) {

                char buffer[2048] = "";

                sprintf(buffer,
                    "PRIVMSG %s : WELCOME %s %d %s\r\n",
                    this->current_channel.c_str(), nick, this->external_port, this->user_data_str.c_str()
                );

                // send buffer

                this->__time_418 = current_time;
                this->__byte_420 = 1;

                this->__string_424 = nick;
                this->__match_user_data_str = message_parts[2];
            }
        }

        if (
            this->get_lobby_user_state() == LOBBY_MATCHING &&
            this->match_host != "" &&
            message_parts.size() > 3 &&
            message_parts[0] == "WELCOME" &&
            message_parts[1] == this->current_nick
        ) {
            this->match_host = std::string(host) + ":" + message_parts[2];
            this->__match_user_data_str = message_parts[3];
            this->set_lobby_user_state(LOBBY_CLOSED);
            this->__last_host_used = host;
        }

        if (
            this->get_lobby_user_state() == LOBBY_WAIT_INCOMING &&
            this->match_host != "" &&
            message_parts.size() > 3 &&
            message_parts[0] == "WELCOME" &&
            message_parts[1] == this->current_nick
        ) {
            this->match_host = std::string(host) + ":" + message_parts[2];
            this->__match_user_data_str = message_parts[3];
            this->__last_host_used = host;
        }
    }

    // Rx5B00
    void __sub_r5B00() {
        while (this->socket_state == SOCKET_CONNECTED) {
            if (this->current_channel != "") {

                LobbyState state = this->get_lobby_user_state();

                // a whole bunch of conditions

                std::string message;
                message += "PRIVMSG";
                message += " " + this->current_channel;
                message += " :STAT";
                message += " "s + std::to_string(state);
                message += " "s + std::to_string(0); // IDK what this value actually is

            }
            else {
                Sleep(50);
            }
        }
    }
};