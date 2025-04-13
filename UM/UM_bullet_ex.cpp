#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

#if !CLANG_CL && __INTELLISENSE__
#define _HAS_CXX17 1
#define _HAS_CXX20 0
#endif

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdint.h>
#include <direct.h>
#include <time.h>

#include <string.h>

#define _USE_MATH_DEFINES 1
#include <math.h>

#include <bit>

#include "../zero/FloatConstants.h"

#include "../zero/util.h"
//#include "../zero/custom_intrin.h"
#include "../zero/func_traits_basic.h"

//#define USE_FAST_REDUCE_ANGLE
#include "../reduce_angle_fast.h"

template <typename T>
using ZUNList = ZUNListBase<T, true>;
template <typename T>
using ZUNListHead = ZUNListHeadBase<T, true>;
template <typename T>
using ZUNListIter = ZUNListIterBase<T, true>;
template <typename T>
using ZUNListEnds = ZUNListEndsBase<T, true>;

#define GAME_VERSION UM_VER

#define ENGLISH_STRINGS

#ifndef ENGLISH_STRINGS
#define JpEnStr(jstring, estring) jstring
#else
#define JpEnStr(jstring, estring) estring
#endif

#define INCLUDE_PATCH_CODE 0

//#define USE_VOLATILES_AND_BARRIERS_FOR_ORIGINAL_CODEGEN 1

#if USE_VOLATILES_AND_BARRIERS_FOR_ORIGINAL_CODEGEN
#define codegen_volatile volatile
#define codegen_barrier() _ReadWriteBarrier()
#else
#define codegen_volatile
#define codegen_barrier() do {} while(false)
#endif

#define CHEAT_THE_LOADER

#undef WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#include <WinUser.h>
#include <wbemidl.h>
#include <winnls32.h>
#include <d3d9.h>
#include <dinput.h>
#include <dsound.h>
#define UNICODE
#include <Core/DXUT.h>
#include <Optional/SDKsound.h>
#undef UNICODE

#include "../zero/zun.h"

#pragma comment (lib, "wbemuuid.lib")
#pragma comment (lib, "Xinput9_1_0.lib")


#ifndef SAFE_FREE
#define SAFE_FREE(p)       { if (p) { free ((void*)p);     (p)=NULL; } }
#endif

#ifndef SAFE_FREE_FAST
#define SAFE_FREE_FAST(p)       { if (auto ptr = (p)) { free ((void*)ptr);     (p)=NULL; } }
#endif

/*
#ifdef cdecl
#undef cdecl
#endif
#define cdecl __cdecl
*/


dllexport gnu_noinline void* cdecl memset_force(void* dst, int val, size_t size) {
    gnu_attr(musttail) return memset(dst, val, size);
}

#define zero_this() memset_force(this, 0, sizeof(*this));

#define zero_this_inline() __builtin_memset(this, 0, sizeof(*this));

struct D3DMATRIXZ : D3DMATRIX {
    // D3DXMatrixIdentity
    inline void set_identity() {
        this->m[0][0] = 1.0f;
        this->m[0][1] = 0.0f;
        this->m[0][2] = 0.0f;
        this->m[0][3] = 0.0f;
        this->m[1][0] = 0.0f;
        this->m[1][1] = 1.0f;
        this->m[1][2] = 0.0f;
        this->m[1][3] = 0.0f;
        this->m[2][0] = 0.0f;
        this->m[2][1] = 0.0f;
        this->m[2][2] = 1.0f;
        this->m[2][3] = 0.0f;
        this->m[3][0] = 0.0f;
        this->m[3][1] = 0.0f;
        this->m[3][2] = 0.0f;
        this->m[3][3] = 1.0f;
    }

    // D3DXMatrixScaling
    inline void set_scaled(float X, float Y, float Z = 1.0f) {
        this->m[0][0] = X;
        this->m[0][1] = 0.0f;
        this->m[0][2] = 0.0f;
        this->m[0][3] = 0.0f;
        this->m[1][0] = 0.0f;
        this->m[1][1] = Y;
        this->m[1][2] = 0.0f;
        this->m[1][3] = 0.0f;
        this->m[2][0] = 0.0f;
        this->m[2][1] = 0.0f;
        this->m[2][2] = Z;
        this->m[2][3] = 0.0f;
        this->m[3][0] = 0.0f;
        this->m[3][1] = 0.0f;
        this->m[3][2] = 0.0f;
        this->m[3][3] = 1.0f;
    }
};
#pragma region // D3DMATRIXZ Validation
ValidateStructSize32(0x40, D3DMATRIXZ);
#pragma endregion

/*
// 0x4028F0
dllexport float vectorcall reduce_angle(float angle) asm_symbol_rel(0x4028F0);
dllexport float vectorcall reduce_angle(float angle) {
    int32_t counter = 0;
    while (angle > PI_f) {
        angle -= TWO_PI_f;
        if (counter++ > 32) break;
    }
    while (-PI_f > angle) {
        angle += TWO_PI_f;
        if (counter++ > 32) break;
    }
    return angle;
}

template <InlineState inline_state = DefaultInline>
static auto reduce_angle(float angle) {
    if constexpr (inline_state == ForceInline) {
        clang_forceinline return reduce_angle(angle);
    } else if constexpr (inline_state == NoInline) {
        clang_noinline return reduce_angle(angle);
    } else {
        return reduce_angle(angle);
    }
}

// 0x402890
// ZUN has always had this function... IDK why
dllexport float vectorcall reduce_angle_add(float angle, float value) asm_symbol_rel(0x402890);
dllexport float vectorcall reduce_angle_add(float angle, float value) {
    return reduce_angle(angle + value);
}
*/

/*
// 0x404BC0
dllexport float& vectorcall reduce_angle_add_write(float& angle_ref, float& out, float value) asm_symbol_rel(0x404BC0);
dllexport float& vectorcall reduce_angle_add_write(float& angle_ref, float& out, float value) {
    return out = reduce_angle_add(angle_ref, value);
}
*/
/*
// 0x404D10
dllexport float& vectorcall reduce_angle_add_assign_write(float& angle_ref, float value) asm_symbol_rel(0x404D10) {
    return angle_ref = reduce_angle_add_ref(angle_ref, value);
}
*/

inline float vectorcall __angle_diffB(float angle, float value, float temp) {
    if (temp > PI_f) {
        return angle - (value + TWO_PI_f);
    } else if (value - angle > PI_f) {
        return angle - (value - TWO_PI_f);
    } else {
        return temp;
    }
}

// 0x439FE0
dllexport float vectorcall reduced_angle_diff(float angle, float value) asm_symbol_rel(0x439FE0);
dllexport float vectorcall reduced_angle_diff(float angle, float value) {
    float temp = angle - value;
    if (temp > PI_f) {
        return angle - (value + TWO_PI_f);
    }
    else if (value - angle > PI_f) {
        return angle - (value - TWO_PI_f);
    }
    else {
        return temp;
    }
}


// 0x46F150
dllexport char* stdcall pbg_strdup(const char* str) asm_symbol_rel(0x46F150);
dllexport char* stdcall pbg_strdup(const char* str) {
    char* ret = (char*)malloc(byteloop_strlen(str));
    if (ret) {
        byteloop_strcpy(ret, str);
    }
    return ret;
}

/*
// 0x404C70
dllexport float& vectorcall reduce_angle_diff_write(float& angle_ref, float& out, float& value_ref) asm_symbol_rel(0x404C70) {
    return reduce_angle_write(out, reduced_angle_diff(angle_ref, value_ref));
}
*/

// size: 0x4
// Of course this exists...
struct ZUNAngle {
    float value; // 0x0

    inline constexpr ZUNAngle() {}

private:
    // 0x404C20
    dllexport gnu_noinline void vectorcall constructor(float, const float value) asm_symbol_rel(0x404C20) {
        this->value = reduce_angle(value);
    }
public:
    inline ZUNAngle(const float value) {
        this->constructor(UNUSED_FLOAT, value);
    }

private:
    // 0x404D70
    dllexport ZUNAngle& vectorcall set(float, const float value) asm_symbol_rel(0x404D70) {
        this->value = reduce_angle(value);
        return *this;
    }
public:
    // 0x404D70
    inline ZUNAngle& vectorcall operator=(const float value) asm_symbol_rel(0x404D70) {
        return this->set(UNUSED_FLOAT, value);
    }

    /*
    inline ZUNAngle& operator=(const ZUNAngle& value) {
        this->value = value.value;
        return *this;
    }
    */

    // 0x404DC0
    dllexport vectorcall operator float() const asm_symbol_rel(0x404DC0) {
        return this->value;
    }

private:
    // 0x404BC0
    dllexport ZUNAngle& vectorcall add(int, float, float, ZUNAngle& out, const float value) asm_symbol_rel(0x404BC0) {
        return out = this->value + value;
    }
public:
    inline ZUNAngle operator+(const float value) {
        ZUNAngle dummy;
        return this->add(UNUSED_DWORD, UNUSED_FLOAT, UNUSED_FLOAT, dummy, value);
    }

private:
    // 0x45E2A0
    dllexport ZUNAngle& thiscall add(ZUNAngle& out, const ZUNAngle& angle) asm_symbol_rel(0x45E2A0) {
        return out = this->value + angle.value;
    }
public:
    inline ZUNAngle operator+(const ZUNAngle& angle) {
        ZUNAngle dummy;
        return this->add(dummy, angle);
    }

private:
    // 0x404D10
    dllexport gnu_noinline ZUNAngle& vectorcall add_assign(float, const float value) asm_symbol_rel(0x404D10) {
        return *this = this->value + value;
    }
public:
    // 0x404D10
    inline ZUNAngle& vectorcall operator+=(const float value) {
        return this->add_assign(UNUSED_FLOAT, value);
    }

    // th14: 0x443D60
    inline ZUNAngle& thiscall operator+=(const ZUNAngle& angle) {
        return *this = this->value + angle.value;
    }

    /*
    inline ZUNAngle operator-(const float value) const {
        return this->value - value;
    }
    */

private:
    // 0x404C70
    dllexport ZUNAngle& thiscall sub(ZUNAngle& out, const ZUNAngle& angle) asm_symbol_rel(0x404C70) {
        return out = reduced_angle_diff(this->value, angle.value);
    }
public:
    inline ZUNAngle thiscall operator-(const ZUNAngle& angle) {
        ZUNAngle dummy;
        return this->sub(dummy, angle);
    }
    
    /*
    inline ZUNAngle& diff(float value) const {
        ZUNAngle temp;
        temp.value = value;
        return *this - temp;
    }
    */
    /*
    inline ZUNAngle& operator-=(const float value) {
        return *this = *this - value;
    }
    */

private:
    // 0x461540
    dllexport ZUNAngle& vectorcall mul(int, float, float, ZUNAngle& out, const float value) asm_symbol_rel(0x404BC0) {
        return out = this->value * value;
    }
public:
    inline ZUNAngle operator*(const float value) {
        ZUNAngle dummy;
        return this->mul(UNUSED_DWORD, UNUSED_FLOAT, UNUSED_FLOAT, dummy, value);
    }

private:
    // th14: 0x443D00
    inline ZUNAngle& vectorcall div(ZUNAngle& out, const float value) {
        return out = this->value / value;
    }
public:
    inline ZUNAngle vectorcall operator/(const float value) {
        ZUNAngle dummy;
        return this->div(dummy, value);
    }
};

struct DUMB_FUNCTIONS {
private:
    // 0x403BC0
    dllexport static gnu_noinline int32_t vectorcall __sub_403BC0(float* arg1, float* arg2, float, float, float arg3, float arg4, float, float, float arg5, float arg6, float arg7, float arg8) {
        float A = reduce_angle(arg5);
        float B = reduce_angle(arg8);
        float C = reduced_angle_diff(A, B);
        float D = reduce_angle(C);
        if (!(fabsf(D) < 0.001f)) {
            float E = reduce_angle(__angle_diffB(A, B, C));
            float F = reduce_angle(reduced_angle_diff(E, PI_f));
            if (!(fabsf(F) < 0.001f)) {
                Float2 G;
                G.make_from_vector(arg5, 10.0f);
                float H = G.x;
                float I;
                float J;
                float K = 0.0f;
                BOOL L;
                if (fabsf(H) < 0.01f) {
                    I = arg3;
                    J = 0.0f;
                    L = true;
                } else {
                    L = false;
                    J = G.y / H;
                    I = arg4 - J * arg3;
                }
                G.make_from_vector(arg8, 10.0f);
                H = G.x;
                float M;
                BOOL N;
                if (fabsf(H) < 0.01f) {
                    M = arg6;
                    N = true;
                } else {
                    N = false;
                    K = G.y / H;
                    M = arg7 - K * arg6;
                }
                float O;
                float P;
                if (!L) {
                    if (!N) {
                        O = M - I;
                        float Q = J - K;
                        P = O * J / Q + I;
                        O /= Q;
                    } else {
                        O = arg6;
                        P = J * arg6 + I;
                    }
                } else {
                    if (!N) {
                        P = K * arg3 + M;
                        O = arg3;
                    } else {
                        P = arg3;
                        if (fabsf(P - arg6) < 0.001f) {
                            return 0;
                        }
                        *arg2 = arg4;
                        *arg1 = P;
                        return 1;
                    }
                }
                *arg1 = O;
                *arg2 = P;
                return 1;
            }
        }
        *arg2 = arg4;
        *arg1 = arg3;
        return 0;
    }
public:
    forceinline static int32_t __sub_403BC0(float* arg1, float* arg2, float arg3, float arg4, float arg5, float arg6, float arg7, float arg8) {
        return __sub_403BC0(arg1, arg2, UNUSED_FLOAT, UNUSED_FLOAT, arg3, arg4, UNUSED_FLOAT, UNUSED_FLOAT, arg5, arg6, arg7, arg8);
    }
};

forceinline int32_t __sub_403BC0(float* arg1, float* arg2, float arg3, float arg4, float arg5, float arg6, float arg7, float arg8) {
    return DUMB_FUNCTIONS::__sub_403BC0(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

enum CriticalSectionIndex {
    UpdateFuncRegistry_CS = 0,
    __unused_cs_1 = 1,
    FileIO_CS = 2,
    Logging_CS = 3,
    __unused_cs_2 = 4,
    Menu_CS = 5,
    __supervisor_thread_cs_1 = 6,
    __unused_cs_3 = 7,
    __unused_cs_4 = 8,
    AnmList_CS = 9,
    RNG_CS = 10,
    __unknown_cs_2 = 11,
    __unknown_cs_3 = 12,
    __unused_cs_5 = 13,
};

// size: 0x1D0
struct CriticalSectionManager {
    CRITICAL_SECTION sections[14]; // 0x0
    uint8_t section_depths[14]; // 0x1C0
    bool enable_sections; // 0x1CE

    // Pretty sure these aren't actually class methods, but whatever. Not the point.
    inline void initialize() {
        nounroll for (size_t i = 0; i < countof(this->sections); ++i) {
            InitializeCriticalSection(&this->sections[i]);
        }
        this->enable_sections = true;
    }

    // 0x404F60
    dllexport static inline void stdcall leave_section(CriticalSectionIndex index) asm_symbol_rel(0x404F60);
    
    // 0x404F90
    dllexport static inline void stdcall enter_section(CriticalSectionIndex index) asm_symbol_rel(0x404F90);

    static inline void leave_section_volatile(CriticalSectionIndex index);
    static inline void enter_section_volatile(CriticalSectionIndex index);
};
/*
#pragma region // CriticalSectionManager Validation
ValidateFieldOffset32(0x0, CriticalSectionManager, sections);
ValidateFieldOffset32(0x1C0, CriticalSectionManager, section_depths);
ValidateFieldOffset32(0x1CE, CriticalSectionManager, enable_sections);
ValidateStructSize32(0x1D0, CriticalSectionManager);
#pragma endregion
*/

extern "C" {
    // 0x521660
    extern CriticalSectionManager CRITICAL_SECTION_MANAGER asm("_CRITICAL_SECTION_MANAGER");
}

// 0x404F60
dllexport inline void stdcall CriticalSectionManager::leave_section(CriticalSectionIndex index) {
    if (CRITICAL_SECTION_MANAGER.enable_sections) {
        LeaveCriticalSection(&CRITICAL_SECTION_MANAGER.sections[index]);
        --CRITICAL_SECTION_MANAGER.section_depths[index];
    }
}
// 0x404F90
dllexport inline void stdcall CriticalSectionManager::enter_section(CriticalSectionIndex index) {
    if (CRITICAL_SECTION_MANAGER.enable_sections) {
        EnterCriticalSection(&CRITICAL_SECTION_MANAGER.sections[index]);
        ++CRITICAL_SECTION_MANAGER.section_depths[index];
    }
}
inline void CriticalSectionManager::leave_section_volatile(CriticalSectionIndex index) {
    if (*(volatile uint8_t*)&CRITICAL_SECTION_MANAGER.enable_sections) {
        LeaveCriticalSection(&CRITICAL_SECTION_MANAGER.sections[index]);
        --*(volatile uint8_t*)&CRITICAL_SECTION_MANAGER.section_depths[index];
    }
}
inline void CriticalSectionManager::enter_section_volatile(CriticalSectionIndex index) {
    if (*(volatile uint8_t*)&CRITICAL_SECTION_MANAGER.enable_sections) {
        EnterCriticalSection(&CRITICAL_SECTION_MANAGER.sections[index]);
        ++*(volatile uint8_t*)&CRITICAL_SECTION_MANAGER.section_depths[index];
    }
}

template<CriticalSectionIndex index>
struct ScopedCriticalSection {
    inline ScopedCriticalSection() {
        CRITICAL_SECTION_MANAGER.enter_section(index);
    }
    inline ~ScopedCriticalSection() {
        CRITICAL_SECTION_MANAGER.leave_section(index);
    }
};

static constexpr auto wkrwjher = sizeof(ScopedCriticalSection<UpdateFuncRegistry_CS>);

#define UniqueCriticalSectionLock(index) auto unique_name(critical_section_scope_guard_) = ScopedCriticalSection<index>()
#define CriticalSectionBlock(index) switch (UniqueCriticalSectionLock(index); 0) default:

// size: 0x4
struct DebugLogger {
    int __dword_0; // 0x0

    // 0x404E80
    dllexport static gnu_noinline void cdecl __debug_log_stub_4(const char* format, ...) asm_symbol_rel(0x404E80) {}

    // 0x404F50
    dllexport gnu_noinline int32_t thiscall __debug_log_stub_12(int32_t value, int, const char*) asm_symbol_rel(0x404F50) {
        return value;
    }

    // 0x43A280
    dllexport static gnu_noinline void cdecl __debug_log_stub_3(const char* format, ...) asm_symbol_rel(0x43A280) {}

    // 0x442440
    dllexport static gnu_noinline void cdecl __debug_log_stub_8(const char* format, ...) asm_symbol_rel(0x442440) {}

    // 0x4562A0
    dllexport static gnu_noinline void cdecl __debug_log_stub_7(const char* format, ...) asm_symbol_rel(0x4562A0) {}

    // 0x46F1D0
    dllexport static gnu_noinline void cdecl __debug_log_stub_2(const char* format, ...) asm_symbol_rel(0x46F1D0) {}

    // 0x471110
    dllexport static gnu_noinline void cdecl __debug_log_stub_11(const char* format, ...) asm_symbol_rel(0x471110) {}

    // 0x476310
    dllexport static gnu_noinline void cdecl __debug_log_stub_1(const char* format, ...) asm_symbol_rel(0x476310) {}

    // 0x477AF0
    dllexport static gnu_noinline void cdecl __debug_log_stub_10(const char* format, ...) asm_symbol_rel(0x477AF0) {}

    // 0x489590
    [[gnu::no_caller_saved_registers]] dllexport static gnu_noinline void cdecl __debug_log_stub_6(const char* format, ...) asm_symbol_rel(0x489590) {}

    // 0x48B010
    dllexport static gnu_noinline void cdecl __debug_log_stub_9(const char* format, ...) asm_symbol_rel(0x48B010) {}

    // 0x48B020
    dllexport static gnu_noinline void cdecl __debug_log_stub_5(const char* format, ...) asm_symbol_rel(0x48B020) {}
};

extern "C" {
    // 0x4CF290
    extern DebugLogger* DEBUG_LOG_PTR asm("_DEBUG_LOG_PTR");

    // 0x4CF2F0
    extern char UNKNOWN_TEXT_BUFFER_A[MAX_PATH] asm("_UNKNOWN_TEXT_BUFFER_A");
}

namespace Pbg {

    // size: 0x4
    struct IFile {
        // Method 0
        virtual gnu_noinline bool thiscall open_file(LPCSTR path, const char* mode) = 0;
        // Method 4
        virtual gnu_noinline void thiscall close_file() = 0;
        // Method 8
        virtual gnu_noinline DWORD thiscall read_file_to_buffer(LPVOID buffer, DWORD size) = 0;
        // Method C
        virtual gnu_noinline bool thiscall write_buffer_to_file(LPVOID buffer, DWORD size) = 0;
        // Method 10
        virtual gnu_noinline DWORD thiscall get_file_pointer() = 0;
        // Method 14
        virtual gnu_noinline DWORD thiscall get_file_size() = 0;
        // Method 18
        virtual gnu_noinline bool thiscall set_file_pointer(LONG offset, DWORD origin) = 0;
        
        // 0x46F580
        virtual ~IFile() {}
    };

    // size: 0xC
    struct File : IFile {

        HANDLE handle; // 0x4
        ACCESS_MASK access_mask; // 0x8
        // 0xC

        inline File() : handle(INVALID_HANDLE_VALUE), access_mask(0) {}

        // 0x46F230
        // Method 0
        virtual gnu_noinline bool thiscall open_file(LPCSTR path_in, const char* mode_str) asm_symbol_rel(0x46F230) {
            BOOL is_append = false;
            DWORD create_type = 0;
            this->close_file();
            const char* read_mode = mode_str;
            for (
                char c;
                (c = *read_mode);
                ++read_mode
            ) {
                switch (c) {
                    case 'r':
                        this->access_mask = GENERIC_READ;
                        create_type = OPEN_EXISTING;
                        goto valid_mode;
                    case 'w':
                        this->access_mask = GENERIC_WRITE;
                        create_type = CREATE_ALWAYS;
                        goto valid_mode;
                    case 'a':
                        is_append = true;
                        this->access_mask = GENERIC_WRITE;
                        create_type = OPEN_ALWAYS;
                        goto valid_mode;
                }
            }
            return false;
        valid_mode:
            if (!*read_mode) {
                return false;
            }
            const char* path_no_drive = strchr(path_in, ':');
            char path_buffer[MAX_PATH];
            if (path_no_drive) {
                byteloop_strcpy(path_buffer, path_in);
            }
            else {
                GetModuleFileNameA(NULL, path_buffer, MAX_PATH);
                char* filename = strrchr(path_buffer, '\\');
                path_buffer[0] = filename ? '\0' : path_buffer[0];
                filename[1] = '\0';
                byteloop_strcat(path_buffer, path_in);
            }
            HANDLE file_handle = CreateFileA(
                path_buffer,
                this->access_mask,
                FILE_SHARE_READ,
                NULL,
                create_type,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                NULL
            );
            this->handle = file_handle;
            if (file_handle == INVALID_HANDLE_VALUE) {
                return false;
            }
            if (is_append) {
                SetFilePointer(file_handle, 0, NULL, FILE_END);
            }
            return true;
        }

        // 0x46F3E0
        // Method 4
        virtual gnu_noinline void thiscall close_file() asm_symbol_rel(0x46F3E0) {
            HANDLE file_handle = this->handle;
            if (file_handle != INVALID_HANDLE_VALUE) {
                CloseHandle(file_handle);
            }
            this->handle = INVALID_HANDLE_VALUE;
            this->access_mask = 0;
        }

        // 0x46F410
        // Method 8
        virtual gnu_noinline DWORD thiscall read_file_to_buffer(LPVOID buffer, DWORD size) asm_symbol_rel(0x46F410) {
            DWORD read_size = 0;
            if (this->access_mask != GENERIC_READ) {
                return 0;
            }
            ReadFile(this->handle, buffer, size, &read_size, NULL);
            return read_size;
        }

        // 0x46F450
        // Method C
        virtual gnu_noinline bool thiscall write_buffer_to_file(LPVOID buffer, DWORD size) asm_symbol_rel(0x46F450) {
            DWORD read_size = 0;
            if (this->access_mask != GENERIC_WRITE) {
                return false;
            }
            WriteFile(this->handle, buffer, size, &read_size, NULL);
            return size == read_size;
        }

        // 0x46F490
        // Method 10
        virtual gnu_noinline DWORD thiscall get_file_pointer() asm_symbol_rel(0x46F490) {
            HANDLE file_handle = this->handle;
            if (file_handle == INVALID_HANDLE_VALUE) {
                return 0;
            }
            return SetFilePointer(file_handle, 0, NULL, FILE_CURRENT);
        }

        // 0x46F4B0
        // Method 14
        virtual gnu_noinline DWORD thiscall get_file_size() asm_symbol_rel(0x46F4B0) {
            HANDLE file_handle = this->handle;
            if (file_handle == INVALID_HANDLE_VALUE) {
                return 0;
            }
            return GetFileSize(file_handle, NULL);
        }

        // 0x46F4D0
        // Method 18
        virtual gnu_noinline bool thiscall set_file_pointer(LONG offset, DWORD origin) asm_symbol_rel(0x46F4D0) {
            HANDLE file_handle = this->handle;
            if (file_handle == INVALID_HANDLE_VALUE) {
                return false;
            }
            SetFilePointer(file_handle, offset, NULL, origin);
            return true;
        }

        // 0x46F1E0
        // Method 1C
        virtual ~File() {
            HANDLE file_handle = this->handle;
            if (file_handle != INVALID_HANDLE_VALUE) {
                CloseHandle(file_handle);
                this->handle = INVALID_HANDLE_VALUE;
                this->access_mask = 0;
            }
        }

        // 0x46F500
        // Method 20
        virtual gnu_noinline LPVOID copy_file_to_new_buffer(LONG file_offset) asm_symbol_rel(0x46F500) {
            if (this->access_mask == GENERIC_READ) {
                DWORD file_size = this->get_file_size();
                if ((DWORD)file_offset <= file_size) {
                    if (LPVOID buffer = malloc(file_size)) {
                        file_offset = this->get_file_pointer();
                        if (this->set_file_pointer(file_offset, FILE_BEGIN)) {
                            if (!this->read_file_to_buffer(buffer, file_size)) {
                                // Yes, this free is misplaced and can leak memory
                                free(buffer);
                            } else {
                                this->set_file_pointer(file_offset, FILE_BEGIN);
                                return buffer;
                            }
                        }
                    }
                }
            }
            return NULL;
        }
    };

};

// size: 0x10(?)
struct ArcFileInner {
    char* __string_0; // 0x0
    int __dword_4; // 0x4
    int32_t file_size; // 0x8
    int __dword_C; // 0xC
    // 0x10

    // 0x46F1C0
    ArcFileInner() : __string_0(NULL) {}

    // 0x46F1A0
    ~ArcFileInner() {
        SAFE_FREE(this->__string_0);
    }
};

// size: 0xC
struct UnknownP {
    uint8_t xor_mask; // 0x0
    uint8_t xor_accel; // 0x1
    uint8_t __ubyte_2; // 0x2
    probably_padding_bytes(0x1); // 0x3
    int32_t __int_4; // 0x4
    int32_t __int_8; // 0x8
    // 0xC
};

/*
extern "C" {
    // 0x4C5360
    extern UnknownP UNKNOWN_P[] asm("_UNKNOWN_P");
}
*/

// 0x4C5360
static UnknownP UNKNOWN_P[] = {
    [0] = {
        .xor_mask = 0x1B,
        .xor_accel = 0x73,
        .__ubyte_2 = 0xAA,
        .__int_4 = 256,
        .__int_8 = 0x3800
    },
    [1] = {
        .xor_mask = 0x12,
        .xor_accel = 0x43,
        .__ubyte_2 = 0xFF,
        .__int_4 = 512,
        .__int_8 = 0x3E00
    },
    [2] = {
        .xor_mask = 0x35,
        .xor_accel = 0x79,
        .__ubyte_2 = 0x11,
        .__int_4 = 1024,
        .__int_8 = 0x3C00
    },
    [3] = {
        .xor_mask = 0x03,
        .xor_accel = 0x91,
        .__ubyte_2 = 0xDD,
        .__int_4 = 128,
        .__int_8 = 0x6400
    },
    [4] = {
        .xor_mask = 0xAB,
        .xor_accel = 0xDC,
        .__ubyte_2 = 0xEE,
        .__int_4 = 128,
        .__int_8 = 0x7000
    },
    [5] = {
        .xor_mask = 0x51,
        .xor_accel = 0x9E,
        .__ubyte_2 = 0xBB,
        .__int_4 = 256,
        .__int_8 = 0x4000
    },
    [6] = {
        .xor_mask = 0xC1,
        .xor_accel = 0x15,
        .__ubyte_2 = 0xCC,
        .__int_4 = 1024,
        .__int_8 = 0x2C00
    },
    [7] = {
        .xor_mask = 0x99,
        .xor_accel = 0x7D,
        .__ubyte_2 = 0x77,
        .__int_4 = 128,
        .__int_8 = 0x4400
    }
};

// 0x401F50
dllexport gnu_noinline void* fastcall __crypt_buffer(void* buffer, int32_t buffer_size, uint8_t xor_mask, uint8_t xor_accel, int32_t arg3, int32_t arg4) asm_symbol_rel(0x401F50);
dllexport gnu_noinline void* fastcall __crypt_buffer(void* buffer, int32_t buffer_size, uint8_t xor_mask, uint8_t xor_accel, int32_t arg3, int32_t arg4) {
    use_var(buffer);
    use_var(buffer_size);
    use_var(xor_mask);
    use_var(xor_accel);
    use_var(arg3);
    use_var(arg4);
    return (void*)rand();
}

// 0x401E40
dllexport gnu_noinline void* fastcall __decrypt_buffer(void* buffer, int32_t buffer_size, uint8_t xor_mask, uint8_t xor_accel, int32_t arg3, int32_t arg4) asm_symbol_rel(0x401E40);
dllexport gnu_noinline void* fastcall __decrypt_buffer(void* buffer, int32_t buffer_size, uint8_t xor_mask, uint8_t xor_accel, int32_t arg3, int32_t arg4) {
    int32_t intA = buffer_size % arg3; // EBX
    uint8_t* input_buffer = (uint8_t*)buffer; // ESI
    uint8_t* buffer_write_offset = input_buffer; // EBP-4
    int32_t intB = arg3 / 4; // EBP-C
    // Definitely not a min func since those use <
    int32_t temp_size = arg4 > buffer_size ? buffer_size : arg4; // EBP-10
    void* temp_buffer = malloc(temp_size);
    input_buffer = (uint8_t*)temp_buffer;
    if (temp_buffer) {
        buffer_size = (buffer_size & ~1) - (intA >= intB ? 0 : intA);
        memcpy(temp_buffer, buffer, temp_size);
        while (buffer_size > 0) {
            if (arg4 <= 0) {
                break;
            }
            arg3 = buffer_size >= arg3 ? arg3 : buffer_size;
            buffer_write_offset += arg3;
            uint8_t* buffer_write = buffer_write_offset - 1;
            for (
                int32_t i = (arg3 + 1) / 2;
                i > 0;

            ) {
                uint8_t val = *input_buffer;
                buffer_write -= 2;
                val ^= xor_mask;
                --i;
                xor_mask += xor_accel;
                buffer_write[2] = val;
                ++input_buffer;
            }
            buffer_write = buffer_write_offset - 2;
            for (
                int32_t i = arg3 / 2;
                i > 0;

            ) {
                uint8_t val = *input_buffer;
                buffer_write -= 2;
                val ^= xor_mask;
                --i;
                xor_mask += xor_accel;
                buffer_write[2] = val;
                ++input_buffer;
            }
            buffer_size -= arg3;
            arg4 -= arg3;
        }
        free(temp_buffer);
    }
    return buffer;
}

extern "C" {
    // 0x51F660
    extern uint8_t DECOMPRESS_BUFFER[0x2000] asm("_DECOMPRESS_BUFFER");
}

// 0x46F5B0
dllexport gnu_noinline void* fastcall __compress_buffer(void* buffer_in, int32_t buffer_size, int32_t* out_buffer_size) asm_symbol_rel(0x46F5B0);
dllexport gnu_noinline void* fastcall __compress_buffer(void* buffer_in, int32_t buffer_size, int32_t* out_buffer_size) {
    use_var(buffer_in);
    use_var(buffer_size);
    *out_buffer_size = rand();
    return (void*)rand();
}

// 0x46F840
// This function looks like handwritten assembly. Compilers don't emit shift instructions that modify flags like this, right?
dllexport gnu_noinline void* fastcall __decompress_buffer(void* buffer_in, int32_t buffer_size, void* out_buffer, int32_t out_buffer_size) asm_symbol_rel(0x46F840);
dllexport gnu_noinline void* fastcall __decompress_buffer(void* buffer_in, int32_t buffer_size, void* out_buffer, int32_t out_buffer_size) {
    codegen_barrier();
    uint32_t uintA = 0; // EBP-8
    uint8_t* buffer = (uint8_t*)buffer_in;
    uint8_t byteA = 0x80;
    if (!out_buffer) {
        if (!(out_buffer = malloc(out_buffer_size))) {
            return NULL;
        }
        uintA = 0; // EBP-8
    }
    uint8_t* buffer_read = buffer; // EBP-1C
    uint8_t* out_buffer_write = (uint8_t*)out_buffer; // EBP-10
    uint32_t decompress_buffer_offset = 1; // EBP-14
    uint32_t uintC = 0x80; // EBP-20
    ptrdiff_t sizeA;

    auto common_loop = [&](uint32_t arg) {
        int32_t ret = 0;
        nounroll do {
            if (byteA == 0x80) {
                uintA = *buffer_read;
                if (sizeA >= buffer_size) {
                    uintA = 0;
                } else {
                    ++buffer_read;
                    ++sizeA;
                }
            }
            // The argument order gets flipped here without the cast
            if ((uint8_t)uintA & byteA) {
                codegen_barrier();
                ret |= arg;
            }
            if (!(byteA >>= 1)) {
                byteA = uintC;
            }
        } while (arg >>= 1);
        return ret;
    };

    for (;;) {
        uint8_t byteB = byteA >> 1; // EBP+F
        codegen_barrier();
        codegen_volatile uint8_t byteC = byteA; // EBP+1
        uint32_t uintB; // this was optimized out, see below
        if (byteA == 0x80) {
            uintA = *buffer_read;
            sizeA = buffer_read - buffer;
            byteA = byteB;
            if (sizeA >= buffer_size) {
                uintA = 0;
                uintB = 0;
                // the compiler figured out that the uintB != 0 branch
                // around loop A is impossible from here and skipped it
            }
            else {
                uintB = byteC;
                ++buffer_read;
                uintB &= uintA;
            }
        }
        else {
            uintB = byteA;
            byteA = byteB;
            uintB &= uintA;
            if (!byteA) {
                byteA = 0x80;
            }
        }
        sizeA = buffer_read - buffer;
        if (uintB) {
            // loop A
            uint32_t val = common_loop(0b10000000); // 0x80
            DECOMPRESS_BUFFER[decompress_buffer_offset++] = val;
            *out_buffer_write++ = val;
            decompress_buffer_offset %= countof(DECOMPRESS_BUFFER);
        }
        else {
            // loop B
            int32_t intD = common_loop(0b1000000000000); // 0x1000
            if (!intD) {
                return out_buffer;
            }
            sizeA = buffer_read - buffer;
            // loop C
            int32_t iters = common_loop(0b1000) + 2; // 0x8
            // loop D
            for (int32_t i = 0; i <= iters; ++i) {
                uint8_t val = DECOMPRESS_BUFFER[(intD + i) % countof(DECOMPRESS_BUFFER)];
                DECOMPRESS_BUFFER[decompress_buffer_offset++] = val;
                *out_buffer_write++ = val;
                decompress_buffer_offset %= countof(DECOMPRESS_BUFFER);
            }
        }
    }
}

// size: 0x10
struct ArcFile {
    ArcFileInner* __pointer_0; // 0x0
    int32_t __int_4; // 0x4
    char* __string_8; // 0x8
    Pbg::File* file; // 0xC
    // 0x10

    // 0x46EC50
    ~ArcFile() {
        if (char* str = this->__string_8) {
            DebugLogger::__debug_log_stub_2("info : %s close arcfile\r\n", str);
            free(this->__string_8);
        }
        this->__string_8 = NULL;
        SAFE_DELETE_ARRAY(this->__pointer_0);
        SAFE_DELETE(this->file);
        this->__int_4 = 0;
    }

    // 0x46EB80
    dllexport static gnu_noinline bool stdcall __sub_46EB80(int32_t = UNUSED_DWORD) asm_symbol_rel(0x46EB80);

    // 0x46EE90
    dllexport static gnu_noinline bool stdcall __sub_46EE90(int32_t = UNUSED_DWORD) asm_symbol_rel(0x46EE90);

    // 0x46ED10
    dllexport gnu_noinline void* thiscall __sub_46ED10(const char* filename, void* file_buffer) asm_symbol_rel(0x46ED10) {
        if (!this->file) {
            return NULL;
        }
        ArcFileInner* ptrA = this->__pointer_0;
        if (ptrA) {
            for (int32_t i = this->__int_4; i > 0; --i, ++ptrA) {
                if (!stricmp(filename, ptrA->__string_0)) {
                    goto found_in_cache;
                }
            }
        }
        DebugLogger::__debug_log_stub_2("info : %s error\r\n", this->__string_8);
        return NULL;
found_in_cache:
        int32_t new_file_size = (intptr_t)(ptrA + 1)->__string_0 - ptrA->__dword_4;
        size_t cached_file_size = ptrA->file_size;
        void* cached_file_buffer;
        if (new_file_size == cached_file_size && file_buffer) {
            cached_file_buffer = file_buffer;
        } else {
            cached_file_buffer = malloc(new_file_size);
        }
        if (!cached_file_buffer) {
            goto error_free;
        }
        if (!this->file->set_file_pointer(this->__int_4, FILE_BEGIN)) {
            goto error_free;
        }
        if (!this->file->read_file_to_buffer(cached_file_buffer, new_file_size)) {
            goto error_free;
        }
        const char* strA = ptrA->__string_0;
        uint8_t string_sum = 0;
        for (
            size_t string_length = byteloop_strlen(strA);
            string_length;
            --string_length
        ) {
            string_sum += *strA++;
        }
        UnknownP& xor_data = UNKNOWN_P[string_sum % countof(UNKNOWN_P)];
        __decrypt_buffer(cached_file_buffer, new_file_size, xor_data.xor_mask, xor_data.xor_accel, xor_data.__int_4, xor_data.__int_8);
        void* ret;
        if (cached_file_size != new_file_size) {
            ret = __decompress_buffer(cached_file_buffer, new_file_size, file_buffer, cached_file_size);
        } else {
            ret = cached_file_buffer;
        }
        if (ret != file_buffer && file_buffer) {
            free(file_buffer);
        }
        return ret;
error_free:
        DebugLogger::__debug_log_stub_2("info : %s error\r\n", this->__string_8);
        if (cached_file_buffer) {
            free(cached_file_buffer);
        }
        return NULL;
    }
};
extern "C" {
    // 0x5217C0
    extern ArcFile THDAT_ARCFILE asm("_THDAT_ARCFILE");
}

// size: 0x10
struct DatFileHeader {
    ZUNMagic magic; // 0x0
    uint32_t decompressed_size; // 0x4
    uint32_t compressed_size; // 0x8
    int __dword_C; // 0xC
    // 0x10
};
#pragma region // DatFileHeader Validation
ValidateFieldOffset32(0x0, DatFileHeader, magic);
ValidateFieldOffset32(0x4, DatFileHeader, decompressed_size);
ValidateFieldOffset32(0x8, DatFileHeader, compressed_size);
ValidateFieldOffset32(0xC, DatFileHeader, __dword_C);
ValidateStructSize32(0x10, DatFileHeader);
#pragma endregion

// 0x46EE90
dllexport gnu_noinline bool stdcall ArcFile::__sub_46EE90(int32_t) {
    if (Pbg::File* pbg_file = THDAT_ARCFILE.file) {
        if (pbg_file->open_file("th18.dat", "r")) {
            DatFileHeader dat_header;
            if (THDAT_ARCFILE.file->read_file_to_buffer(&dat_header, sizeof(DatFileHeader))) {
                __decrypt_buffer(&dat_header, sizeof(DatFileHeader), 0x1B, 0x37, 0x10, 0x10);
                if (dat_header.magic.as_uint == PackUInt('T', 'H', 'A', '1')) {
                    uint32_t uintA = dat_header.__dword_C + 0xF7E7F8AC; // IDK what this value is
                    dat_header.decompressed_size -= 123456789;
                    dat_header.compressed_size -= 987654321;
                    THDAT_ARCFILE.__int_4 = uintA;
                    int32_t data_offset = THDAT_ARCFILE.file->get_file_size() - dat_header.compressed_size;
                    THDAT_ARCFILE.file->set_file_pointer(data_offset, FILE_BEGIN);
                    size_t compressed_size = dat_header.compressed_size;
                    if (void* compressed_buffer = malloc(compressed_size)) {
                        void* buffer = NULL;
                        if (THDAT_ARCFILE.file->read_file_to_buffer(compressed_buffer, compressed_size)) {
                            __decrypt_buffer(compressed_buffer, compressed_size, 0x3E, 0x9B, 0x80, compressed_size);
                            buffer = __decompress_buffer(compressed_buffer, compressed_size, buffer, dat_header.decompressed_size);
                            if (buffer) {
                                int32_t intA = THDAT_ARCFILE.__int_4;
                                ArcFileInner* arcfile_inner_array = new ArcFileInner[intA + 1];
                                if (!arcfile_inner_array) {
                                    THDAT_ARCFILE.__pointer_0 = NULL;
                                    free(compressed_buffer);
                                    goto free_buffer;
                                }
                                char* buffer_read = (char*)buffer;
                                ArcFileInner* arcfile_inner_array_write = arcfile_inner_array;
                                for (int32_t i = intA; i > 0; --i) {
                                    arcfile_inner_array_write->__string_0 = pbg_strdup(buffer_read);
                                    int32_t length = byteloop_strlen(buffer_read) + 1;
                                    if (int32_t temp = length % 4) {
                                        length += 4 - temp;
                                    }
                                    buffer_read += length;
                                    arcfile_inner_array_write->__dword_4 = ((uint32_t*)buffer_read)[0];
                                    arcfile_inner_array_write->file_size = ((uint32_t*)buffer_read)[1];
                                    arcfile_inner_array_write->__dword_C = ((uint32_t*)buffer_read)[2];
                                    buffer_read += sizeof(uint32_t[3]);
                                    ++arcfile_inner_array_write;
                                }
                                arcfile_inner_array[intA].__dword_4 = data_offset;
                                arcfile_inner_array[intA].file_size = 0;
                                THDAT_ARCFILE.__pointer_0 = arcfile_inner_array;
                                free(compressed_buffer);
                                free(buffer);
                                return true;
                            }
                        }
                        free(compressed_buffer);
                        if (buffer) {
free_buffer:
                            free(buffer);
                        }
                    }
                }
            }
        }
        SAFE_DELETE(THDAT_ARCFILE.file);
    }
    return false;
}

// 0x46EB80
dllexport gnu_noinline bool stdcall ArcFile::__sub_46EB80(int32_t) {
    THDAT_ARCFILE.~ArcFile();
    char current_directory[512];
    GetCurrentDirectoryA(countof(current_directory), current_directory);
    DebugLogger::__debug_log_stub_2("info : CurrentDirectory %s \r\n", current_directory);
    Pbg::File* pbg_file = new Pbg::File();
    THDAT_ARCFILE.file = pbg_file;
    if (ArcFile::__sub_46EE90()) {
        char* str;
        clang_noinline str = pbg_strdup("th18.dat");
        THDAT_ARCFILE.__string_8 = str;
        if (str) {
            THDAT_ARCFILE.file->open_file(str, "r");
            return true;
        }
    }
    THDAT_ARCFILE.~ArcFile();
    return false;
}

// size: 0x10
struct ArcFileEx : ArcFile {
    // 0x46EB50
    dllexport gnu_noinline ArcFileEx() {
        this->__pointer_0 = NULL;
        this->__int_4 = 0;
        this->__string_8 = NULL;
        this->file = NULL;
    }

    // 0x46EB70
    dllexport gnu_noinline ~ArcFileEx() asm_symbol_rel(0x46EB70) {}
};

extern "C" {
    // 0x568AF0
    extern ArcFileEx ARCFILE_ARRAY[20] asm("_ARCFILE_ARRAY");
}

// 0x402060
dllexport gnu_noinline void* fastcall read_file_to_buffer(const char* path, int32_t* file_size_out, bool is_not_in_dat) {
    CRITICAL_SECTION_MANAGER.enter_section(FileIO_CS);
    DWORD file_size;
    void* ret = NULL;
    if (!is_not_in_dat) {
        const char* filename = strrchr(path, '\\');
        filename = strrchr(filename ? ++filename : path, '/');
        filename = filename ? ++filename : path;
        if (ArcFileInner* ptrA = THDAT_ARCFILE.__pointer_0) {
            for (int32_t i = THDAT_ARCFILE.__int_4; i > 0; --i, ++ptrA) {
                if (!stricmp(filename, ptrA->__string_0)) {
                    file_size = ptrA->file_size;
                    goto decode_file;
                }
            }
        }
        file_size = 0;
decode_file:
        if (file_size_out) {
            *file_size_out = file_size;
        }
        if (file_size) {
            DebugLogger::__debug_log_stub_4("%s Decode ... \r\n", filename);
            if ((ret = malloc(file_size))) {
                THDAT_ARCFILE.__sub_46ED10(filename, ret);
            }
        }
    } else {
        DebugLogger::__debug_log_stub_4("%s Load ... \r\n", path);
        HANDLE file_handle = CreateFileA(
            path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
            NULL
        );
        if (file_handle == INVALID_HANDLE_VALUE) {
            DebugLogger::__debug_log_stub_4("error : %s is not found.\r\n", path);
        }
        else {
            file_size = GetFileSize(file_handle, NULL);
            if (!(ret = malloc(file_size))) {
                DebugLogger::__debug_log_stub_4("error : %s allocation error.\r\n", path);
            }
            else {
                ReadFile(file_handle, ret, file_size, &file_size, NULL);
                if (file_size_out) {
                    *file_size_out = file_size;
                }
            }
            CloseHandle(file_handle);
        }
    }
    CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
    return ret;
}

extern "C" {
    // 0x570EA0
    extern char PATH_BUFFER[MAX_PATH] asm("_PATH_BUFFER");
}
inline void* read_file_from_dat(const char* path) {
    PATH_BUFFER[0] = '\0';
    byteloop_strcat(PATH_BUFFER, path);
    return read_file_to_buffer(PATH_BUFFER, NULL, false);
}

// 0x402220
dllexport gnu_noinline BOOL fastcall zun_file_exists(const char* filename) asm_symbol_rel(0x402220);
dllexport gnu_noinline BOOL fastcall zun_file_exists(const char* filename) {
    CRITICAL_SECTION_MANAGER.enter_section(FileIO_CS);
    HANDLE file_handle = CreateFileA(
        filename, // lpFileName
        GENERIC_READ, // dwDesiredAccess
        FILE_SHARE_READ, // dwShareMode
        NULL, // lpSecurityFeatures
        OPEN_EXISTING, // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, // dwFlagsAndAttributes
        NULL // hTemplateFile
    );
    if (file_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(file_handle);
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
        return TRUE;
    }
    else {
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
        return FALSE;
    }
}
// 0x4022A0
dllexport gnu_noinline ZUNResult fastcall __zun_create_new_file_from_buffer(const char* filename, void* buffer, size_t buffer_size) asm_symbol_rel(0x4022A0);
dllexport gnu_noinline ZUNResult fastcall __zun_create_new_file_from_buffer(const char* filename, void* buffer, size_t buffer_size) {
    CRITICAL_SECTION_MANAGER.enter_section(FileIO_CS);
    HANDLE file_handle = CreateFileA(
        filename, // lpFileName
        GENERIC_WRITE, // dwDesiredAccess
        FILE_SHARE_READ, // dwShareMode
        NULL, // lpSecurityFeatures
        CREATE_ALWAYS, // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
        NULL // hTemplateFile
    );
    if (file_handle == INVALID_HANDLE_VALUE) {
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, // dwFlags
            NULL, // lpSource
            GetLastError(), // dwMessageId,
            LANG_USER_DEFAULT, // dwLanguageId,
            (LPSTR)&buffer_size, // lpBuffer
            0, // nSize
            NULL // Arguments
        );
        DebugLogger::__debug_log_stub_4("error : %s write error %s\r\n", filename, (char*)buffer_size);
        LocalFree((HLOCAL)buffer_size);
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
        return ZUN_ERROR;
    }
    DWORD bytes_written;
    WriteFile(
        file_handle, // hFile
        buffer, // lpBuffer
        buffer_size, // nNumberOfBytesToWrite
        &bytes_written, // lpNumberOfBytesWritten
        NULL // lpOverlapped
    );
    if (buffer_size != bytes_written) {
        CloseHandle(file_handle);
        DebugLogger::__debug_log_stub_4("error : %s write error\r\n", filename);
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
        return ZUN_ERROR2;
    }
    CloseHandle(file_handle);
    DebugLogger::__debug_log_stub_4("%s write ... \r\n", filename);
    CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
    return ZUN_SUCCESS;
}

extern "C" {
    // 0x4CA96C
    extern HANDLE STATIC_FILE_HANDLE asm("_STATIC_FILE_HANDLE");
}

inline ZUNResult __zun_open_file(const char* filename) {
    CRITICAL_SECTION_MANAGER.enter_section(FileIO_CS);
    HANDLE file_handle = CreateFileA(
        filename, // lpFileName
        GENERIC_READ, // dwDesiredAccess
        FILE_SHARE_READ, // dwShareMode
        NULL, // lpSecurityFeatures
        OPEN_EXISTING, // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, // dwFlagsAndAttributes
        NULL // hTemplateFile
    );
    STATIC_FILE_HANDLE = file_handle;
    if (file_handle == INVALID_HANDLE_VALUE) {
        char* message;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, // dwFlags
            NULL, // lpSource
            GetLastError(), // dwMessageId,
            LANG_USER_DEFAULT, // dwLanguageId,
            (LPSTR)&message, // lpBuffer
            0, // nSize
            NULL // Arguments
        );
        DebugLogger::__debug_log_stub_4("error : %s write error %s\r\n", filename, message);
        LocalFree((HLOCAL)message);
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
        return ZUN_ERROR;
    }
    DebugLogger::__debug_log_stub_4("%s open ...\r\n", filename);
    return ZUN_SUCCESS;
}

// 0x4023E0
dllexport gnu_noinline ZUNResult fastcall __zun_open_new_file(const char* filename) asm_symbol_rel(0x4023E0);
dllexport gnu_noinline ZUNResult fastcall __zun_open_new_file(const char* filename) {
    CRITICAL_SECTION_MANAGER.enter_section(FileIO_CS);
    HANDLE file_handle = CreateFileA(
        filename, // lpFileName
        GENERIC_WRITE, // dwDesiredAccess
        FILE_SHARE_READ, // dwShareMode
        NULL, // lpSecurityFeatures
        CREATE_ALWAYS, // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
        NULL // hTemplateFile
    );
    STATIC_FILE_HANDLE = file_handle;
    if (file_handle == INVALID_HANDLE_VALUE) {
        char* message;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, // dwFlags
            NULL, // lpSource
            GetLastError(), // dwMessageId,
            LANG_USER_DEFAULT, // dwLanguageId,
            (LPSTR)&message, // lpBuffer
            0, // nSize
            NULL // Arguments
        );
        DebugLogger::__debug_log_stub_4("error : %s write error %s\r\n", filename, message);
        LocalFree((HLOCAL)message);
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
        return ZUN_ERROR;
    }
    DebugLogger::__debug_log_stub_4("%s open ...\r\n", filename);
    return ZUN_SUCCESS;
}

// 0x4024A0
dllexport gnu_noinline void* fastcall __zun_read_file(size_t bytes) asm_symbol_rel(0x4024A0);
dllexport gnu_noinline void* fastcall __zun_read_file(size_t bytes) {
    if (STATIC_FILE_HANDLE == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    void* buffer = malloc(bytes);
    if (!buffer) {
        CloseHandle(STATIC_FILE_HANDLE);
        return NULL;
    }
    DWORD idgaf;
    ReadFile(
        STATIC_FILE_HANDLE, // hFile
        buffer, // lpBuffer
        bytes, // nNumberOfBytesToRead
        &idgaf, // lpNumberOfBytesRead
        NULL // lpOverlapped
    );
    return buffer;
}

inline ZUNResult fastcall __zun_write_file(void* buffer, size_t bytes) {
    if (STATIC_FILE_HANDLE == INVALID_HANDLE_VALUE) {
        return ZUN_ERROR;
    }
    DWORD written;
    WriteFile(
        STATIC_FILE_HANDLE, // hFile
        buffer, // lpBuffer
        bytes, // nNumberOfBytesToRead
        &written, // lpNumberOfBytesRead
        NULL // lpOverlapped
    );
    if (bytes != written) {
        CloseHandle(STATIC_FILE_HANDLE);
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
        return ZUN_ERROR2;
    }
    return ZUN_SUCCESS;
}

// 0x402500
dllexport gnu_noinline ZUNResult fastcall __zun_close_file() asm_symbol_rel(0x402500);
dllexport gnu_noinline ZUNResult fastcall __zun_close_file() {
    HANDLE file_handle = STATIC_FILE_HANDLE;
    if (file_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(file_handle);
        CRITICAL_SECTION_MANAGER.leave_section(FileIO_CS);
    }
    return ZUN_SUCCESS;
}

// size: 0x2008
template <size_t buffer_size>
struct LogBuffer {
    char buffer[buffer_size]; // 0x0
    char* buffer_write; // 0x2000
    bool is_error; // 0x2004

    LogBuffer() {
        this->buffer_write = this->buffer;
    }

    // 0x402530
    dllexport gnu_noinline const char* cdecl write(const char* format, ...) asm_symbol_rel(0x402530) {
        char stupid_stack_buffer[buffer_size];

        CRITICAL_SECTION_MANAGER.enter_section(Logging_CS);
        {
            va_list va;
            va_start(va, format);
            vsprintf(stupid_stack_buffer, format, va_use(va));
            size_t written_length = byteloop_strlen(stupid_stack_buffer);
            char* buffer_write = this->buffer_write;
            if (&buffer_write[written_length] < array_end_addr(this->buffer)) {
                byteloop_strcpy(buffer_write, stupid_stack_buffer);
                this->buffer_write += written_length;
                *this->buffer_write = '\0';
            }
        }
        CRITICAL_SECTION_MANAGER.leave_section(Logging_CS);
        return format;
    }

    // 0x402600
    dllexport gnu_noinline const char* cdecl write_error(const char* format, ...) asm_symbol_rel(0x402600) {
        char stupid_stack_buffer[0x200];

        CRITICAL_SECTION_MANAGER.enter_section(Logging_CS);
        {
            va_list va;
            va_start(va, format);
            vsprintf(stupid_stack_buffer, format, va_use(va));
            size_t written_length = byteloop_strlen(stupid_stack_buffer);
            char* buffer_write = this->buffer_write;
            if (&buffer_write[written_length] < array_end_addr(this->buffer)) {
                byteloop_strcpy(buffer_write, stupid_stack_buffer);
                this->buffer_write += written_length;
                *this->buffer_write = '\0';
            }
            this->is_error = true;
        }
        CRITICAL_SECTION_MANAGER.leave_section(Logging_CS);
        return format;
    }
};
extern "C" {
    // 0x4CABE8
    extern LogBuffer<0x2000> LOG_BUFFER asm("_LOG_BUFFER");
}

enum UpdateFuncRet : int32_t {
    UpdateFuncDeleteCurrentThenNext = 0,
    UpdateFuncNext = 1,
    UpdateFuncRepeatCurrent = 2,
    UpdateFuncEnd1 = 3,
    UpdateFuncEnd0 = 4,
    UpdateFuncEndN1 = 5,
    UpdateFuncRestartTick = 6,
    UpdateFuncCleanupThenNext = 7,
    UpdateFuncEnd0Dupe = 8,
};

typedef UpdateFuncRet(fastcall UpdateFunction)(void*);
typedef ZUNResult(fastcall UpdateFuncInit)(void*);
typedef ZUNResult(fastcall UpdateFuncCleanup)(void*);

// size: 0x28
struct UpdateFunc {
    int32_t priority; // 0x0
    union {
        uint32_t flags; // 0x4
        struct {
            uint32_t initialized : 1; // 1
            uint32_t run_on_update : 1; // 2
        };
    };
    UpdateFunction* on_update_func; // 0x8
    UpdateFuncInit* on_init_func; // 0xC
    UpdateFuncCleanup* on_cleanup_func; // 0x10
    ZUNList<UpdateFunc> list_node; // 0x14
    void* func_arg; // 0x24
    // 0x28

    // 0x401530
    UpdateFunc(UpdateFunction* tick_func) : initialized(false), on_update_func(NULL), on_init_func(NULL), on_cleanup_func(NULL) {
        this->priority = 0;
        this->list_node.initialize_with(this);
        this->initialized = true;
        this->on_update_func = tick_func;
        this->on_init_func = NULL;
        this->on_cleanup_func = NULL;
    }

    inline UpdateFunc(UpdateFunction* tick_func, bool run_on_update, void* arg) : initialized(false), on_update_func(NULL), on_init_func(NULL), on_cleanup_func(NULL) {
        this->priority = 0;
        this->list_node.initialize_with(this);
        this->initialized = true;
        this->run_on_update = run_on_update;
        this->on_update_func = tick_func;
        this->on_init_func = NULL;
        this->on_cleanup_func = NULL;
        this->func_arg = arg;
    }

#define declare_on_func(ret, func_name, func_field, ...) \
    inline ret func_name() { \
        if (auto* func = this->func_field) { \
            return func(this->func_arg); \
        } else return (ret)0; \
    }
    declare_on_func(ZUNResult, run_init, on_init_func)
    declare_on_func(ZUNResult, run_cleanup, on_cleanup_func)

#undef declare_on_func

    inline UpdateFuncRet run_update() {
        UpdateFuncRet ret;
        CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
        ret = this->on_update_func(this->func_arg);
        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
        return ret;
    }
};
#pragma region // UpdateFunc Validation
ValidateFieldOffset32(0x0, UpdateFunc, priority);
ValidateFieldOffset32(0x4, UpdateFunc, flags);
ValidateFieldOffset32(0x8, UpdateFunc, on_update_func);
ValidateFieldOffset32(0xC, UpdateFunc, on_init_func);
ValidateFieldOffset32(0x10, UpdateFunc, on_cleanup_func);
ValidateFieldOffset32(0x14, UpdateFunc, list_node);
ValidateFieldOffset32(0x24, UpdateFunc, func_arg);
ValidateStructSize32(0x28, UpdateFunc);
#pragma endregion

typedef struct UpdateFuncRegistry UpdateFuncRegistry;
extern "C" {
    // 0x4CF294
    extern UpdateFuncRegistry* UPDATE_FUNC_REGISTRY_PTR asm("_UPDATE_FUNC_REGISTRY_PTR");
}
// size: 0x58
struct UpdateFuncRegistry {
    UpdateFunc on_tick_funcs; // 0x0
    UpdateFunc on_draw_funcs; // 0x28
    ZUNList<UpdateFunc>* __next_node; // 0x50
    int __dword_54; // 0x54
    // 0x58


    UpdateFuncRegistry() : on_tick_funcs(NULL), on_draw_funcs(NULL) {
        this->__next_node->data = NULL;
    }

    // 0x401180
    dllexport static gnu_noinline ZUNResult stdcall register_on_tick(UpdateFunc* update_tick, int32_t new_func_priority) asm_symbol_rel(0x401180) {
        ZUNResult ret = update_tick->run_init();
        UpdateFuncRegistry* update_func_registry = UPDATE_FUNC_REGISTRY_PTR;
        CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
        {
            update_tick->priority = new_func_priority;
            auto* prev_priority_node = update_func_registry->on_tick_funcs.list_node.as_head().find_node_before([=](UpdateFunc* update_func) {
                return update_func->priority >= new_func_priority;
            });
            prev_priority_node->append(&update_tick->list_node);
        }
        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
        return ret;
    }

    // 0x401230
    dllexport static gnu_noinline int32_t stdcall register_on_draw(UpdateFunc* update_draw, int32_t new_func_priority) asm_symbol_rel(0x401230) {
        int32_t ret = update_draw->run_init();
        UpdateFuncRegistry* update_func_registry = UPDATE_FUNC_REGISTRY_PTR;
        CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
        {
            update_draw->priority = new_func_priority;
            auto* prev_priority_node = update_func_registry->on_draw_funcs.list_node.as_head().find_node_before([=](UpdateFunc* update_func) {
                return update_func->priority >= new_func_priority;
            });
            prev_priority_node->append(&update_draw->list_node);
        }
        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
        return ret;
    }

    // 0x4015A0
    dllexport gnu_noinline void thiscall delete_func(UpdateFunc* update_func) asm_symbol_rel(0x4015A0) {
        if (update_func) {
            auto data_match = [=](UpdateFunc* data) {
                return data == update_func;
            };
            ZUNList<UpdateFunc>* update_func_node;
            if (!(update_func_node = this->on_tick_funcs.list_node.find_node_if(data_match)) &&
                !(update_func_node = this->on_draw_funcs.list_node.find_node_if(data_match))
            ) {
                return;
            }
            update_func_node->unlink_from_head(this->__next_node);
            if (update_func_node->prev) { // Don't try to delete the list heads
                update_func_node->unlink();
                update_func->on_update_func = NULL;
                if (update_func->initialized) {
                    update_func->on_init_func = NULL;
                    update_func->on_cleanup_func = NULL;
                    delete update_func;
                }
            }
        }
    }

#if GAME_VERSION == 190
    // 0x401650
    dllexport gnu_noinline void thiscall delete_func_locked(UpdateFunc* update_func) asm_symbol_rel(0x401650) {
        if (update_func) {
            CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
            {
                this->delete_func(update_func);
            }
            CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
        }
    }
#endif

    // 0x4012E0
    dllexport gnu_noinline int32_t thiscall run_all_on_tick() asm_symbol_rel(0x4012E0) {
        int32_t ret;
        CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
        {
RestartOnTick:
            ret = 0;
            ZUNList<UpdateFunc>* node = this->on_tick_funcs.list_node.next;
            while (node) {
                ZUNList<UpdateFunc>* next_node = node->next;
                this->__next_node = next_node;
                UpdateFunc* update_tick = node->data;
                node = next_node;
                if (!update_tick->on_update_func) {
                    continue;
                }
                if (update_tick->run_on_update) {
                    do {
                        if (this->__dword_54) {
                            goto CleanupThenNext;
                        }
                        int32_t update_ret;
                        CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
                        {
                            update_ret = update_tick->on_update_func(update_tick->func_arg);
                        }
                        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
                        switch (expect_chance(update_ret, UpdateFuncRepeatCurrent, 0.05)) {
                            case UpdateFuncDeleteCurrentThenNext:
                                goto DeleteCurrentThenNext;
                            case UpdateFuncNext: default:
                                goto NextFunc;
                            case UpdateFuncRepeatCurrent:
                                continue;
                            case UpdateFuncEnd0: case UpdateFuncEnd0Dupe:
                                ret = 0;
                                goto EndOnTick;
                            case UpdateFuncEnd1:
                                ret = 1;
                                goto EndOnTick;
                            case UpdateFuncEndN1:
                                ret = -1;
                                goto EndOnTick;
                            case UpdateFuncRestartTick:
                                goto RestartOnTick;
                            case UpdateFuncCleanupThenNext:
                                goto CleanupThenNext;
                        }
                    } while (update_tick->run_on_update);
                    jump_only_block{
DeleteCurrentThenNext:
                        this->delete_func(update_tick);
                    }
                    jump_only_block{
CleanupThenNext:
                        update_tick->run_cleanup();
                    }
                }
NextFunc:
                node = this->__next_node;
                ++ret;
            }
        }
EndOnTick:
        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
        return ret;
    }

    // 0x401420
    dllexport static gnu_noinline int32_t run_all_on_draw() asm_symbol_rel(0x401420) {
        UpdateFuncRegistry* update_func_registry = UPDATE_FUNC_REGISTRY_PTR;
        int32_t ret;
        CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
        {
//RestartOnTick:
            ret = 0;
            ZUNList<UpdateFunc>* node = update_func_registry->on_draw_funcs.list_node.next;
            while (expect(node != NULL, true)) {
                ZUNList<UpdateFunc>* next_node = node->next;
                update_func_registry->__next_node = next_node;
                UpdateFunc* update_draw = node->data;
                node = next_node;
                if (!update_draw->on_update_func) {
                    continue;
                }
                if (update_draw->run_on_update) {
                    do {
                        /*if (update_func_registry->__dword_54) {
                            goto CleanupThenNext;
                        }*/
                        int32_t update_ret;
                        CRITICAL_SECTION_MANAGER.enter_section_volatile(UpdateFuncRegistry_CS);
                        {
                            update_ret = update_draw->on_update_func(update_draw->func_arg);
                        }
                        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
                        switch (expect_chance(update_ret, UpdateFuncNext, 0.95)) {
                            case UpdateFuncDeleteCurrentThenNext:
                                goto DeleteCurrentThenNext;
                            case UpdateFuncNext: default:
                                goto NextFunc;
                            case UpdateFuncRepeatCurrent:
                                continue;
                            case UpdateFuncEnd0: //case UpdateFuncEnd0Dupe:
                                ret = 0;
                                goto EndOnDraw;
                            case UpdateFuncEnd1:
                                ret = 1;
                                goto EndOnDraw;
                            case UpdateFuncEndN1:
                                ret = -1;
                                goto EndOnDraw;
                            /*case UpdateFuncRestartTick:
                                goto RestartOnTick;
                            case UpdateFuncCleanupThenNext:
                                goto CleanupThenNext;*/
                        }
                    } while (update_draw->run_on_update);
                    jump_only_block{
DeleteCurrentThenNext:
                        update_func_registry->delete_func(update_draw);
                    }
                    /*jump_only_block{
CleanupThenNext:
                        update_tick->run_cleanup();
                    }*/
                }
            NextFunc:
                node = update_func_registry->__next_node;
                ++ret;
            }
        }
EndOnDraw:
        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
        return ret;
    }
};
#pragma region // UpdateFuncRegistry Validation
ValidateFieldOffset32(0x0, UpdateFuncRegistry, on_tick_funcs);
ValidateFieldOffset32(0x28, UpdateFuncRegistry, on_draw_funcs);
ValidateFieldOffset32(0x50, UpdateFuncRegistry, __next_node);
ValidateFieldOffset32(0x54, UpdateFuncRegistry, __dword_54);
ValidateStructSize32(0x58, UpdateFuncRegistry);
#pragma endregion

// size: 0x8
struct Rng {
    uint16_t value; // 0x0
    uint32_t index; // 0x4
    // 0x8
    
    // 0x4026D0
    dllexport gnu_noinline uint16_t thiscall rand_ushort() asm_symbol_rel(0x4026D0) {
        CRITICAL_SECTION_MANAGER.enter_section(RNG_CS);
        {
            this->index++;
            uint16_t temp = (this->value ^ 0x9630) - 0x6553;
            this->value = ((temp & 0xC000) >> 14) + (temp << 2) & 0xFFFF;
        }
        CRITICAL_SECTION_MANAGER.leave_section(RNG_CS);
        return this->value;
    }
    
    // 0x402740
    dllexport gnu_noinline uint32_t thiscall rand_uint() asm_symbol_rel(0x402740) {
        uint16_t tempA, tempB;
        CRITICAL_SECTION_MANAGER.enter_section(RNG_CS);
        {
            this->index += 2;
            tempA = (this->value ^ 0x9630) - 0x6553;
            tempB = ((tempA & 0xC000) >> 14) + (tempA << 2) & 0xFFFF;
            tempB = (tempB ^ 0x9630) - 0x6553;
            this->value = ((tempB & 0xC000) >> 14) + (tempB << 2) & 0xFFFF;
        }
        CRITICAL_SECTION_MANAGER.leave_section(RNG_CS);
        return (tempA << 16) | tempB;
    }
    
    // 0x4027D0
    dllexport gnu_noinline float thiscall rand_float() asm_symbol_rel(0x4027D0) {
        __asm FINIT;
        float temp = this->rand_uint();
        return temp / (float)UINT32_MAX; // float jank rounds this to UINT32_MAX+1
    }
    // 0x402810
    dllexport gnu_noinline float thiscall rand_float_signed() asm_symbol_rel(0x402810) {
        __asm FINIT;
        float temp = this->rand_uint();
        return temp / (float)INT32_MAX - 1.0f; // float jank rounds this to INT32_MAX+1
    }
    // 0x405B90
    dllexport gnu_noinline float vectorcall rand_angle() asm_symbol_rel(0x405B90) {
        return this->rand_float_signed() * PI_f;
    }
    // 0x402850
    dllexport gnu_noinline float thiscall rand_angle_2() asm_symbol_rel(0x402850) {
        __asm FINIT;
        float temp = this->rand_uint();
        return temp / TWO_PI_f - PI_f;
    }
    
    // 0x406AB0
    dllexport gnu_noinline float vectorcall rand_float_range(float range) asm_symbol_rel(0x406AB0) {
        return this->rand_float() * range;
    }
    
    inline float rand_float_signed_range(float range) {
        return this->rand_float_signed() * range;
    }
    // 0x4615A0
    dllexport gnu_noinline uint32_t thiscall rand_uint_range(uint32_t range) asm_symbol_rel(0x4615A0) {
        if (range != 0) {
            return this->rand_uint() % range;
        } else {
            return 0;
        }
    }
};
#pragma region // Rng Validation
ValidateFieldOffset32(0x0, Rng, value);
ValidateFieldOffset32(0x4, Rng, index);
ValidateStructSize32(0x8, Rng);
#pragma endregion

extern "C" {
    // 0x4CF280
    extern Rng RNG asm("_RNG");
    // 0x4CF288
    extern Rng REPLAY_RNG asm("_REPLAY_RNG");
}

enum Difficulty {
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
    LUNATIC = 3,
    EXTRA = 4,
    OVERDRIVE = 5
};

typedef struct Float3 Float3;
typedef struct Enemy Enemy;

// 0x4237F0
dllexport gnu_noinline Enemy* get_boss_by_index(int32_t boss_id) asm_symbol_rel(0x4237F0);

// size: 0x8
struct GameSpeed {
    float value = 1.0f; // 0x0
    int __counter_4; // 0x4
    // 0x8

    // 0x43A200
    dllexport gnu_noinline void vectorcall set(float new_speed) asm_symbol_rel(0x43A200);
};
#pragma region // GameSpeed Validation
ValidateFieldOffset32(0x0, GameSpeed, value);
ValidateFieldOffset32(0x4, GameSpeed, __counter_4);
ValidateStructSize32(0x8, GameSpeed);
#pragma endregion

extern "C" {
    // 0x4CCBF0
    extern GameSpeed GAME_SPEED asm("_GAME_SPEED");
}
// 0x43A200
dllexport gnu_noinline void vectorcall GameSpeed::set(float new_speed) {
    GAME_SPEED.value = new_speed;
}

static const float* TIME_SCALING_TABLE[] = {
    &GAME_SPEED.value
};

// size: 0x14
struct Timer {
    int32_t previous; // 0x0
    int32_t current; // 0x4
    float current_f; // 0x8
    uint32_t scale_table_index; // 0xC
    union {
        uint32_t flags; // 0x10
        struct {
            uint32_t initialized : 1;
        };
    };
    // 0x14

    inline operator int32_t() {
        return this->current;
    }
    inline operator float() {
        return this->current_f;
    }

    inline Timer() : initialized(false) {};

    inline void default_values() {
        this->current = 0;
        this->previous = -999999;
        this->current_f = 0.0f;
    }

private:
    inline void initialize_previous_force(int32_t value) volatile {
        this->previous = value;
    }
public:

    inline void initialize() {
        if (!this->initialized) {
            this->initialize_previous_force(-999999);
            this->scale_table_index = 0;
            this->initialized = true;
        }
    }

    inline void initialize_important() {
        if (!this->initialized) {
            this->scale_table_index = 0;
            this->initialized = true;
        }
    }
    inline void set_raw(int32_t time) {
        this->current = time;
        this->current_f = (float)time;
        this->previous = time - 1;
    }
    inline void set_raw(float time) {
        this->current_f = time;
        int32_t int_time = (int32_t)time;
        this->current = int_time;
        this->previous = int_time - 1;
    }
    // 0x405D10
    dllexport inline void thiscall set(int32_t time) asm_symbol_rel(0x405D10) {
        this->initialize_important();
        this->set_raw(time);
    }
    inline void set(float time) {
        this->initialize_important();
        this->set_raw(time);
    }
    inline void reset() {
        this->set(0);
    }

    inline void initialize_and_reset() {
        this->initialize();
        this->set_raw(0);
    }
    inline void initialize_and_set(int32_t time) {
        this->initialize();
        this->set_raw(time);
    }

    // 0x418C40
    // Yes, this really does copy a whole timer to the stack
    // Possibly an assignment operator...?
    dllexport gnu_noinline void set_from_timer(Timer timer) asm_symbol_rel(0x418C40) {
        this->set(timer.current);
    }

    // Just in case that is an assignment operator, check for warnings in current uses
    Timer& operator=(Timer timer) = delete;

    inline uint32_t get_scaling_index() {
        uint32_t time_scaling_index = this->scale_table_index;
        if (time_scaling_index >= countof(TIME_SCALING_TABLE)) {
            time_scaling_index = this->scale_table_index = 0;
        }
        return time_scaling_index;
    }

    inline float get_scale_unsafe() {
        return *TIME_SCALING_TABLE[this->scale_table_index];
    }

    // 0x402A60
    dllexport int32_t vectorcall add_raw(float value) asm_symbol_rel(0x402A60) {
        this->previous = this->current;
        return this->current = this->current_f += value;
    }

    inline int32_t vectorcall add_float(float amount) {
        const float* time_scale_ptr = TIME_SCALING_TABLE[this->get_scaling_index()];
        this->previous = this->current;
        if (time_scale_ptr) {
            float time_scale = *time_scale_ptr;
            if (!(time_scale > 0.99f) || !(time_scale < 1.01f)) {
                return this->current = this->current_f += time_scale * amount;
            }
        }
        return this->current = this->current_f += amount;
    }

    inline int32_t vectorcall sub_float(float amount) {
        const float* time_scale_ptr = TIME_SCALING_TABLE[this->get_scaling_index()];
        this->previous = this->current;
        if (time_scale_ptr) {
            float time_scale = *time_scale_ptr;
            if (!(time_scale > 0.99f) || !(time_scale < 1.01f)) {
                return this->current = this->current_f -= time_scale * amount;
            }
        }
        return this->current = this->current_f -= amount;
    }

    inline int32_t add_int(int32_t amount) {
        return this->add_float(amount);
    }

    // 0x452BF0
    dllexport int32_t thiscall sub_int(int32_t amount) asm_symbol_rel(0x452BF0) {
        return this->add_float(-amount);
    }
    
    inline int32_t increment() {
        const float* time_scale_ptr = TIME_SCALING_TABLE[this->get_scaling_index()];
        int32_t current = this->current;
        this->previous = current;
        if (time_scale_ptr) {
            float time_scale = *time_scale_ptr;
            if (!(time_scale > 0.99f) || !(time_scale < 1.01f)) {
                return this->current = this->current_f += time_scale;
            }
        }
        this->current_f += 1.0f;
        return this->current = ++current;
    }

    inline int32_t decrement() {
        return this->sub_float(1.0f);
    }
    
    // 0x405990
    dllexport int32_t thiscall operator++(int) asm_symbol_rel(0x405990) {
        clang_forceinline return this->increment();
    }

    // 0x402A80
    dllexport int32_t thiscall operator++() asm_symbol_rel(0x402A80) {
        clang_forceinline return this->increment();
    }

    // 0x409750
    dllexport int32_t thiscall operator--(int) asm_symbol_rel(0x409750) {
        clang_forceinline return this->decrement();
    }

    dllexport int32_t thiscall operator--() {
        clang_forceinline return this->decrement();
    }

    inline int32_t operator+=(int32_t amount) {
        return this->add_int(amount);
    }

    inline int32_t operator+=(float amount) {
        return this->add_float(amount);
    }

    inline int32_t operator-=(int32_t amount) {
        return this->sub_int(amount);
    }

    inline int32_t operator-=(float amount) {
        return this->sub_float(amount);
    }

    inline int32_t operator+(int32_t amount) {
        return this->current + amount;
    }
    inline float operator+(float amount) {
        return this->current_f + amount;
    }
    inline int32_t operator-(int32_t amount) {
        return this->current - amount;
    }
    inline float operator-(float amount) {
        return this->current_f - amount;
    }
    inline int32_t operator*(int32_t amount) {
        return this->current * amount;
    }
    inline float operator*(float amount) {
        return this->current_f * amount;
    }
    inline int32_t operator/(int32_t amount) {
        return this->current / amount;
    }
    inline float operator/(float amount) {
        return this->current_f / amount;
    }

    inline int32_t operator%(int32_t amount) {
        return this->current % amount;
    }

    friend inline int32_t operator-(int32_t amount, const Timer& self) {
        return amount - self.current;
    }
    friend inline float operator-(float amount, const Timer& self) {
        return amount - self.current_f;
    }
    friend inline int32_t operator/(int32_t amount, const Timer& self) {
        return amount / self.current;
    }
    friend inline float operator/(float amount, const Timer& self) {
        return amount / self.current_f;
    }

    friend inline int32_t operator%(int32_t amount, const Timer& self) {
        return amount % self.current;
    }

    // 0x4210A0
    dllexport BOOL thiscall __is_multiple_of_not_paused(int32_t value) asm_symbol_rel(0x4210A0) {
        int32_t current = this->current;
        if (
            current != this->previous &&
            !(current % value)
        ) {
            return TRUE;
        }
        return FALSE;
    }

    inline bool is_multiple_of(int32_t value) {
        return !(this->current % value);
    }

    inline bool les(int32_t time) {
        return this->current < time;
    }
    inline bool les(float time) {
        return this->current_f < time;
    }
    inline bool leq(int32_t time) {
        return this->current <= time;
    }
    inline bool leq(float time) {
        return this->current_f <= time;
    }
    inline bool gre(int32_t time) {
        return this->current > time;
    }
    inline bool gre(float time) {
        return this->current_f > time;
    }
    inline bool geq(int32_t time) {
        return this->current >= time;
    }
    inline bool geq(float time) {
        return this->current_f >= time;
    }
    inline bool equ(int32_t time) {
        return this->current == time;
    }
    inline bool equ(float time) {
        return this->current_f == time;
    }
    inline bool neq(int32_t time) {
        return this->current != time;
    }
    inline bool neq(float time) {
        return this->current_f != time;
    }
    inline bool is_zero() {
        return this->equ(0);
    }
    inline bool is_not_zero() {
        return this->neq(0);
    }

    inline bool operator<(int32_t time) {
        return this->les(time);
    }
    inline bool operator<(float time) {
        return this->les(time);
    }
    inline bool operator<=(int32_t time) {
        return this->leq(time);
    }
    inline bool operator<=(float time) {
        return this->leq(time);
    }
    inline bool operator>(int32_t time) {
        return this->gre(time);
    }
    inline bool operator>(float time) {
        return this->gre(time);
    }
    inline bool operator>=(int32_t time) {
        return this->geq(time);
    }
    inline bool operator>=(float time) {
        return this->geq(time);
    }
    inline bool operator==(int32_t time) {
        return this->equ(time);
    }
    inline bool operator==(float time) {
        return this->equ(time);
    }
    inline bool operator!=(int32_t time) {
        return this->neq(time);
    }
    inline bool operator!=(float time) {
        return this->neq(time);
    }
    inline bool operator!() {
        return this->is_zero();
    }
    inline operator bool() {
        return this->is_not_zero();
    }
};
#pragma region // Timer Validation
ValidateFieldOffset32(0x0, Timer, previous);
ValidateFieldOffset32(0x4, Timer, current);
ValidateFieldOffset32(0x8, Timer, current_f);
ValidateFieldOffset32(0xC, Timer, scale_table_index);
ValidateFieldOffset32(0x10, Timer, flags);
ValidateStructSize32(0x14, Timer);
#pragma endregion

// size: 0x14
struct InputMapping {
    int16_t shoot; // 0x0
    int16_t bomb; // 0x2
    int16_t focus; // 0x4
    int16_t pause; // 0x6
    int16_t __button4; // 0x8
    int16_t __button5; // 0xA
    int16_t __button6; // 0xC
    int16_t __button7; // 0xE
    int16_t use_card; // 0x10
    int16_t switch_card; // 0x12
    // 0x14
};
#pragma region // InputMapping Validation
ValidateFieldOffset32(0x0, InputMapping, shoot);
ValidateFieldOffset32(0x2, InputMapping, bomb);
ValidateFieldOffset32(0x4, InputMapping, focus);
ValidateFieldOffset32(0x6, InputMapping, pause);
ValidateFieldOffset32(0x10, InputMapping, use_card);
ValidateFieldOffset32(0x12, InputMapping, switch_card);
ValidateStructSize32(0x14, InputMapping);
#pragma endregion

static inline constexpr size_t BUTTON_COUNT = 32;

static inline constexpr uint32_t BUTTON_SHOOT       = 0x00000001; // 'Z'
static inline constexpr uint32_t BUTTON_BOMB        = 0x00000002; // 'X'

static inline constexpr uint32_t BUTTON_FOCUS       = 0x00000008; // VK_SHIFT
static inline constexpr uint32_t BUTTON_UP          = 0x00000010;
static inline constexpr uint32_t BUTTON_DOWN        = 0x00000020;
static inline constexpr uint32_t BUTTON_LEFT        = 0x00000040;
static inline constexpr uint32_t BUTTON_RIGHT       = 0x00000080;
static inline constexpr uint32_t BUTTON_PAUSE       = 0x00000100; // VK_ESCAPE
static inline constexpr uint32_t BUTTON_SKIP        = 0x00000200;
static inline constexpr uint32_t BUTTON_USE_CARD    = 0x00000400; // 'C'
static inline constexpr uint32_t BUTTON_SWITCH_CARD = 0x00000800; // 'D'

static inline constexpr uint32_t BUTTON_UNKNOWN_A   = 0x00010000;

static inline constexpr uint32_t BUTTON_SCREENSHOT  = 0x00040000; // 'P', VK_HOME
static inline constexpr uint32_t BUTTON_ENTER       = 0x00080000; // VK_RETURN

static inline constexpr uint32_t BUTTON_RESTART     = 0x00200000; // 'R', VK_END

static inline constexpr uint32_t BUTTON_SELECT      = BUTTON_SHOOT | BUTTON_ENTER; // 'Z', VK_RETURN
static inline constexpr uint32_t BUTTON_CANCEL      = BUTTON_BOMB; // 'X'

static inline constexpr uint32_t BUTTON_SHOOT_INDEX = 0;
static inline constexpr uint32_t BUTTON_BOMB_INDEX = 1;
static inline constexpr uint32_t BUTTON_FOCUS_INDEX = 3;
static inline constexpr uint32_t BUTTON_UP_INDEX = 4;
static inline constexpr uint32_t BUTTON_DOWN_INDEX = 5;
static inline constexpr uint32_t BUTTON_LEFT_INDEX = 6;
static inline constexpr uint32_t BUTTON_RIGHT_INDEX = 7;
static inline constexpr uint32_t BUTTON_PAUSE_INDEX = 8;
static inline constexpr uint32_t BUTTON_SKIP_INDEX = 9;
static inline constexpr uint32_t BUTTON_USE_CARD_INDEX = 10;
static inline constexpr uint32_t BUTTON_SWITCH_CARD_INDEX = 11;
static inline constexpr uint32_t BUTTON_SCREENSHOT_INDEX = 18;
static inline constexpr uint32_t BUTTON_ENTER_INDEX = 19;
static inline constexpr uint32_t BUTTON_RESTART_INDEX = 21;

#define XINPUT_GAMEPAD_LEFT_TRIGGER 0x10000
#define XINPUT_GAMEPAD_RIGHT_TRIGGER 0x20000

// 0x4B4320
// Is this actually an array of 20 with extra blanks?
static inline constexpr uint32_t XINPUT_PAD_MAPPINGS[] = {
    XINPUT_GAMEPAD_A,
    XINPUT_GAMEPAD_B,
    XINPUT_GAMEPAD_X,
    XINPUT_GAMEPAD_Y,
    XINPUT_GAMEPAD_LEFT_SHOULDER,
    XINPUT_GAMEPAD_RIGHT_SHOULDER,
    XINPUT_GAMEPAD_LEFT_TRIGGER,
    XINPUT_GAMEPAD_RIGHT_TRIGGER,
    XINPUT_GAMEPAD_LEFT_THUMB,
    XINPUT_GAMEPAD_RIGHT_THUMB,
    XINPUT_GAMEPAD_START,
    XINPUT_GAMEPAD_BACK
};

enum InputMode : int32_t {
    InputXInput = 0,
    InputJoypad = 1,
    InputKeyboard = 2,
};

// This matches UnknownV in the th19 repo
struct InputState {
    union {
        uint32_t hardware_inputs_current; // 0x0
        struct {

        };
    };
    union {
        uint32_t hardware_inputs_previous; // 0x4
        struct {

        };
    };
    union {
        uint32_t hardware_inputs_held_26_frames; // 0x8
        struct {

        };
    };
    union {
        uint32_t hardware_inputs_rising_edge; // 0xC
        struct {

        };
    };
    union {
        uint32_t hardware_inputs_falling_edge; // 0x10
        struct {

        };
    };
    uint32_t hardware_inputs_held_for_repeat[BUTTON_COUNT]; // 0x14
    uint32_t inputs_held_for_repeat[BUTTON_COUNT]; // 0x94
    uint32_t hardware_inputs_held[BUTTON_COUNT]; // 0x114
    uint32_t inputs_held[BUTTON_COUNT]; // 0x194
    int __dword_214; // 0x214
    union {
        uint32_t inputs_current; // 0x218
        struct {
            
        };
    };
    union {
        uint32_t inputs_previous; // 0x21C
        struct {
            
        };
    };
    union {
        uint32_t inputs_held_26_frames; // 0x220
        struct {
            
        };
    };
    union {
        uint32_t inputs_rising_edge; // 0x224
        struct {
            
        };
    };
    union {
        uint32_t inputs_falling_edge; // 0x228
        struct {
            
        };
    };
    union {
        uint32_t hardware_inputs_held_8_frames; // 0x22C
        struct {

        };
    };
    union {
        uint32_t inputs_held_8_frames; // 0x230
        struct {
            
        };
    };
    InputMode __device_type; // 0x234
    InputMapping joypad_mapping; // 0x238
    InputMapping xinput_mapping; // 0x24C
    InputMapping keyboard_mapping; // 0x260
    // 0x274

    // 0x418CE0
    dllexport gnu_noinline void thiscall __reset_inputs() asm_symbol_rel(0x418CE0) {
        memset(this->inputs_held_for_repeat, 0, sizeof(this->inputs_held_for_repeat));
        memset(this->inputs_held, 0, sizeof(this->inputs_held));
        this->__dword_214 = 0;
        this->inputs_current = 0;
        this->inputs_previous = 0;
        this->inputs_held_26_frames = 0;
        this->inputs_rising_edge = 0;
        this->inputs_falling_edge = 0;
        this->inputs_held_8_frames = 0;
    }

    // 0x404E90
    dllexport gnu_noinline void thiscall __update_hardware_input() asm_symbol_rel(0x404E90) {
        uint32_t current = this->hardware_inputs_current;
        uint32_t mask = 1;

        for (size_t i = 0; i < BUTTON_COUNT; ++i) {
            if (current & 1) {
                ++this->hardware_inputs_held_for_repeat[i];
                ++this->hardware_inputs_held[i];
                if (this->hardware_inputs_held_for_repeat[i] >= 8) {
                    this->hardware_inputs_held_8_frames |= mask;
                }
                if (this->hardware_inputs_held_for_repeat[i] >= 26) {
                    this->hardware_inputs_held_26_frames |= mask;
                    this->hardware_inputs_held_for_repeat[i] -= 8;
                }
            } else {
                this->hardware_inputs_held_for_repeat[i] = 0;
                this->hardware_inputs_held[i] = 0;
            }
            current >>= 1;
            mask <<= 1;
        }
        current = this->hardware_inputs_current;
        uint32_t changed = current ^ this->hardware_inputs_previous;
        this->hardware_inputs_rising_edge = current & changed;
        this->hardware_inputs_falling_edge = ~current & changed;
    }

    inline void __update_input() {
        uint32_t current = this->inputs_current;
        uint32_t mask = 1;

        for (size_t i = 0; i < BUTTON_COUNT; ++i) {
            if (current & 1) {
                ++this->inputs_held_for_repeat[i];
                ++this->inputs_held[i];
                if (this->inputs_held_for_repeat[i] >= 8) {
                    this->inputs_held_8_frames |= mask;
                }
                if (this->inputs_held_for_repeat[i] >= 26) {
                    this->inputs_held_26_frames |= mask;
                    this->inputs_held_for_repeat[i] -= 8;
                }
            } else {
                this->inputs_held_for_repeat[i] = 0;
                this->inputs_held[i] = 0;
            }
            current >>= 1;
            mask <<= 1;
        }
        current = this->inputs_current;
        uint32_t changed = current ^ this->inputs_previous;
        this->inputs_rising_edge = current & changed;
        this->inputs_falling_edge = ~current & changed;
    }

    inline BOOL check_inputs(uint32_t mask) {
        if (!(this->inputs_current & mask)) {
            return FALSE;
        }
        return TRUE;
    }

    inline BOOL check_inputs_no_repeat(uint32_t mask) {
        if (!(this->inputs_rising_edge & mask)) {
            return FALSE;
        }
        return TRUE;
    }

    inline BOOL check_inputs_repeating(uint32_t mask) {
        if (
            !(this->inputs_rising_edge & mask) &&
            !(this->inputs_held_26_frames & mask)
        ) {
            return FALSE;
        }
        return TRUE;
    }

    inline BOOL check_hardware_inputs(uint32_t mask) {
        if (!(this->hardware_inputs_current & mask)) {
            return FALSE;
        }
        return TRUE;
    }

    inline BOOL check_hardware_inputs_no_repeat(uint32_t mask) {
        if (!(this->hardware_inputs_rising_edge & mask)) {
            return FALSE;
        }
        return TRUE;
    }

    inline BOOL check_hardware_inputs_repeating(uint32_t mask) {
        if (
            !(this->hardware_inputs_rising_edge & mask) &&
            !(this->hardware_inputs_held_26_frames & mask)
        ) {
            return FALSE;
        }
        return TRUE;
    }

    inline uint32_t get_xinput(uint32_t buttons) {
        uint32_t prev_buttons = buttons;

        nounroll for (DWORD i = 0; i < 4; ++i) {
            XINPUT_STATE state = {};
            if (XInputGetState(i, &state) == ERROR_SUCCESS) {
                uint32_t pad_buttons = state.Gamepad.wButtons;
                if (state.Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
                    pad_buttons |= XINPUT_GAMEPAD_LEFT_TRIGGER;
                }
                if (state.Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
                    pad_buttons |= XINPUT_GAMEPAD_RIGHT_TRIGGER;
                }
                if (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                    pad_buttons |= XINPUT_GAMEPAD_DPAD_UP;
                }
                if (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                    pad_buttons |= XINPUT_GAMEPAD_DPAD_DOWN;
                }
                if (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                    pad_buttons |= XINPUT_GAMEPAD_DPAD_RIGHT;
                }
                if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                    pad_buttons |= XINPUT_GAMEPAD_DPAD_LEFT;
                }

                if (pad_buttons & XINPUT_GAMEPAD_DPAD_UP) {
                    buttons |= BUTTON_UP;
                }
                if (pad_buttons & XINPUT_GAMEPAD_DPAD_DOWN) {
                    buttons |= BUTTON_DOWN;
                }
                if (pad_buttons & XINPUT_GAMEPAD_DPAD_LEFT) {
                    buttons |= BUTTON_LEFT;
                }
                if (pad_buttons & XINPUT_GAMEPAD_DPAD_RIGHT) {
                    buttons |= BUTTON_RIGHT;
                }

                if (pad_buttons & XINPUT_PAD_MAPPINGS[this->xinput_mapping.shoot]) {
                    buttons |= BUTTON_SHOOT;
                }
                if (pad_buttons & XINPUT_PAD_MAPPINGS[this->xinput_mapping.bomb]) {
                    buttons |= BUTTON_BOMB;
                }
                if (pad_buttons & XINPUT_PAD_MAPPINGS[this->xinput_mapping.focus]) {
                    buttons |= BUTTON_FOCUS;
                }
                if (pad_buttons & XINPUT_PAD_MAPPINGS[this->xinput_mapping.use_card]) {
                    buttons |= BUTTON_USE_CARD;
                }
                if (pad_buttons & XINPUT_PAD_MAPPINGS[this->xinput_mapping.switch_card]) {
                    buttons |= BUTTON_SWITCH_CARD;
                }
                if (pad_buttons & XINPUT_PAD_MAPPINGS[this->xinput_mapping.pause]) {
                    buttons |= BUTTON_PAUSE;
                }
            }
        }

        if (buttons != prev_buttons) {
            this->__device_type = InputXInput;
        }
        return buttons;
    }

    inline uint32_t get_joypad(uint32_t buttons);

    inline uint32_t get_keyboard(uint32_t buttons) {
        uint32_t prev_buttons = buttons;

        BYTE keys[256];
        GetKeyboardState(keys);

        // Again, this original code is *bad*.
        // I'm not going to attempt to match it.

        int32_t bomb = this->keyboard_mapping.bomb;
        int32_t shoot = this->keyboard_mapping.shoot;

        uint32_t pressed = keys[shoot]; // becomes 0x1
        pressed >>= 1;

        pressed |= keys[bomb] & 0x80; // becomes 0x2
        pressed >>= 1;

        // mystery key
        pressed >>= 1;

        int32_t focus = this->keyboard_mapping.focus;
        pressed |= keys[focus] & 0x80; // becomes 0x8
        pressed >>= 1;

        pressed |= (keys[VK_UP] | keys[VK_NUMPAD8]) & 0x80; // becomes 0x10
        pressed >>= 1;
        pressed |= (keys[VK_DOWN] | keys[VK_NUMPAD2]) & 0x80; // becomes 0x20
        pressed >>= 1;
        pressed |= (keys[VK_LEFT] | keys[VK_NUMPAD4]) & 0x80; // becomes 0x40
        pressed >>= 1;

        uint32_t pressed2 = (keys[VK_END] | keys['R']) & 0x80; // becomes 0x200000
        pressed2 <<= 1;

        // mystery key
        pressed2 <<= 1;

        pressed2 |= keys[VK_RETURN] & 0x80; // becomes 0x80000
        pressed2 <<= 1;

        pressed2 |= (keys[VK_HOME] | keys['P']) & 0x80; // becomes 0x40000
        pressed2 <<= 7;

        int32_t switch_card = this->keyboard_mapping.switch_card;
        pressed2 |= keys[switch_card] & 0x80; // becomes 0x800
        pressed2 <<= 1;

        int32_t use_card = this->keyboard_mapping.use_card;
        pressed2 |= keys[use_card] & 0x80; // becomes 0x400
        pressed2 <<= 1;

        // mystery key
        pressed2 <<= 1;

        int32_t pause = this->keyboard_mapping.pause;
        pressed2 |= keys[pause] & 0x80; // becomes 0x100
        pressed2 <<= 1;

        pressed |= pressed2;

        pressed |= (keys[VK_RIGHT] | keys[VK_NUMPAD6]) & 0x80; // becomes 0x80

        pressed |= keys[VK_NUMPAD3] & 0x80 ? BUTTON_DOWN | BUTTON_RIGHT : 0;
        pressed |= keys[VK_NUMPAD9] & 0x80 ? BUTTON_UP | BUTTON_RIGHT : 0;
        pressed |= keys[VK_NUMPAD1] & 0x80 ? BUTTON_DOWN | BUTTON_LEFT : 0;
        pressed |= keys[VK_NUMPAD7] & 0x80 ? BUTTON_UP | BUTTON_LEFT : 0;

        buttons |= pressed;

        if (buttons != prev_buttons) {
            this->__device_type = InputKeyboard;
        }
        return buttons;
    }
};
#pragma region // InputState Validation
ValidateFieldOffset32(0x0, InputState, hardware_inputs_current);
ValidateFieldOffset32(0x4, InputState, hardware_inputs_previous);
ValidateFieldOffset32(0x8, InputState, hardware_inputs_held_26_frames);
ValidateFieldOffset32(0xC, InputState, hardware_inputs_rising_edge);
ValidateFieldOffset32(0x14, InputState, hardware_inputs_held_for_repeat);
ValidateFieldOffset32(0x94, InputState, inputs_held_for_repeat);
ValidateFieldOffset32(0x114, InputState, hardware_inputs_held);
ValidateFieldOffset32(0x194, InputState, inputs_held);
ValidateFieldOffset32(0x214, InputState, __dword_214);
ValidateFieldOffset32(0x218, InputState, inputs_current);
ValidateFieldOffset32(0x21C, InputState, inputs_previous);
ValidateFieldOffset32(0x220, InputState, inputs_held_26_frames);
ValidateFieldOffset32(0x224, InputState, inputs_rising_edge);
ValidateFieldOffset32(0x228, InputState, inputs_falling_edge);
ValidateFieldOffset32(0x22C, InputState, hardware_inputs_held_8_frames);
ValidateFieldOffset32(0x230, InputState, inputs_held_8_frames);
ValidateFieldOffset32(0x234, InputState, __device_type);
ValidateFieldOffset32(0x238, InputState, joypad_mapping);
ValidateFieldOffset32(0x24C, InputState, xinput_mapping);
ValidateFieldOffset32(0x260, InputState, keyboard_mapping);
ValidateStructSize32(0x274, InputState);
#pragma endregion


extern "C" {
    // 0x4CA210
    extern InputState INPUT_STATES[3] asm("_INPUT_STATES");
}

// 0x42ABC0
dllexport gnu_noinline void __update_input0() asm_symbol_rel(0x42ABC0);
dllexport gnu_noinline void __update_input0() {
    INPUT_STATES[0].__update_input();
}

// 0x416B70
dllexport gnu_noinline BOOL stdcall __hardware_input0_pressed_or_held(uint32_t mask) asm_symbol_rel(0x416B70);
dllexport gnu_noinline BOOL stdcall __hardware_input0_pressed_or_held(uint32_t mask) {
    return INPUT_STATES[0].check_hardware_inputs_repeating(mask);
}

// size : 0x88
struct Config {
    int __dword_0; // 0x0
    InputMapping joypad_mapping; // 0x4
    InputMapping xinput_mapping; // 0x18
    InputMapping keyboard_mapping; // 0x2C
    int16_t deadzone_x; // 0x40
    int16_t deadzone_y; // 0x42
    uint8_t __ubyte_44; // 0x44
    uint8_t __ubyte_45; // 0x45
    uint8_t __ubyte_46; // 0x46
    uint8_t __ubyte_47; // 0x47
    uint8_t __ubyte_48; // 0x48
    uint8_t __ubyte_49; // 0x49
    int8_t __sbyte_4A; // 0x4A
    int8_t __sbyte_4B; // 0x4B
    char __byte_4C; // 0x4C
    char __byte_4D; // 0x4D
    uint8_t __ubyte_4E; // 0x4E
    unknown_fields(0x1); // 0x4F
    union {
        int32_t flags; // 0x50
        struct {
            int32_t __unknown_flag_B : 1; // 1
            int32_t reference_rasterizer : 1; // 2
            int32_t disable_fog : 1; // 3
            int32_t disable_direct_input : 1; // 4
            int32_t preload_bgm : 1; // 5
            int32_t disable_vsync : 1; // 6
            int32_t __disable_locale_detection : 1; // 7
            int32_t : 1; // 8
            int32_t __unknown_flag_A : 1; // 9
            int32_t __unknown_flag_C : 1; // 10
        };
    };
    int32_t window_x; // 0x54
    int32_t window_y; // 0x58
    unknown_fields(0x2C); // 0x5C
    
    inline void zero_contents() {
        zero_this();
    }
    
    // 0x444DD0
    dllexport gnu_noinline void initialize() asm_symbol_rel(0x444DD0) {
        this->zero_contents();
        this->__unknown_flag_A = true;
        this->__dword_0 = 0x180002;
        this->deadzone_x = 0x258;
        this->deadzone_y = 0x258;
        this->__ubyte_45 = 1;
        this->__ubyte_46 = 1;
        this->__ubyte_47 = 8;
        this->joypad_mapping = INPUT_STATES[0].joypad_mapping;
        this->xinput_mapping = INPUT_STATES[0].xinput_mapping;
        this->keyboard_mapping = INPUT_STATES[0].keyboard_mapping;
        this->__ubyte_49 = 2;
        this->__sbyte_4A = 0x64;
        this->__byte_4C = 0;
        this->__byte_4D = 2;
        this->__sbyte_4B = 50;
        this->window_x = INT32_MIN;
        this->window_y = INT32_MIN;
    }

    inline Config() {
        this->initialize();
    }
};
#pragma region // Config Validation
ValidateFieldOffset32(0x0, Config, __dword_0);
ValidateFieldOffset32(0x4, Config, joypad_mapping);
ValidateFieldOffset32(0x18, Config, xinput_mapping);
ValidateFieldOffset32(0x2C, Config, keyboard_mapping);
ValidateFieldOffset32(0x40, Config, deadzone_x);
ValidateFieldOffset32(0x42, Config, deadzone_y);
ValidateFieldOffset32(0x44, Config, __ubyte_44);
ValidateFieldOffset32(0x45, Config, __ubyte_45);
ValidateFieldOffset32(0x46, Config, __ubyte_46);
ValidateFieldOffset32(0x47, Config, __ubyte_47);
ValidateFieldOffset32(0x48, Config, __ubyte_48);
ValidateFieldOffset32(0x49, Config, __ubyte_49);
ValidateFieldOffset32(0x4A, Config, __sbyte_4A);
ValidateFieldOffset32(0x4B, Config, __sbyte_4B);
ValidateFieldOffset32(0x4C, Config, __byte_4C);
ValidateFieldOffset32(0x4D, Config, __byte_4D);
ValidateFieldOffset32(0x4E, Config, __ubyte_4E);
ValidateFieldOffset32(0x50, Config, flags);
ValidateFieldOffset32(0x54, Config, window_x);
ValidateFieldOffset32(0x58, Config, window_y);
ValidateStructSize32(0x88, Config);
#pragma endregion


// size: 0x1C
struct StageSky {
    float begin_distance; // 0x0
    float end_distance; // 0x4
    D3DCOLORVALUE color_components; // 0x8
    D3DCOLOR color; // 0x18

private:
    // 0x41F830
    dllexport gnu_noinline StageSky& add(StageSky& out, const StageSky& value) asm_symbol_rel(0x41F830) {
        out.begin_distance = this->begin_distance + value.begin_distance;
        out.begin_distance = this->begin_distance + value.end_distance;
        out.color_components.r = this->color_components.r + value.color_components.r;
        out.color_components.g = this->color_components.g + value.color_components.g;
        out.color_components.b = this->color_components.b + value.color_components.b;
        out.color_components.a = this->color_components.a + value.color_components.a;
        for (size_t i = 0; i < 4; ++i) {
            ((int8_t*)&out.color)[i] = ((float*)&out.color_components)[i];
        }
        return out;
    }
public:
    inline StageSky operator+(const StageSky& value) {
        StageSky dummy;
        return this->add(dummy, value);
    }

private:
    inline StageSky& sub(StageSky& out, const StageSky& value) {
        out.begin_distance = this->begin_distance - value.begin_distance;
        out.begin_distance = this->begin_distance - value.end_distance;
        out.color_components.r = this->color_components.r - value.color_components.r;
        out.color_components.g = this->color_components.g - value.color_components.g;
        out.color_components.b = this->color_components.b - value.color_components.b;
        out.color_components.a = this->color_components.a - value.color_components.a;
        for (size_t i = 0; i < 4; ++i) {
            ((int8_t*)&out.color)[i] = ((float*)&this->color_components)[i] - ((float*)&out.color_components)[i];
        }
        return out;
    }
public:
    inline StageSky operator-(const StageSky& value) {
        StageSky dummy;
        return this->sub(dummy, value);
    }

private:
    inline StageSky& mul(StageSky& out, const float value) {
        out.begin_distance = this->begin_distance * value;
        out.begin_distance = this->begin_distance * value;
        out.color_components.r = this->color_components.r * value;
        out.color_components.g = this->color_components.g * value;
        out.color_components.b = this->color_components.b * value;
        out.color_components.a = this->color_components.a * value;
        for (size_t i = 0; i < 4; ++i) {
            ((int8_t*)&out.color)[i] = ((float*)&this->color_components)[i] * value;
        }
        return out;
    }
public:
    inline StageSky operator*(const float value) {
        StageSky dummy;
        return this->mul(dummy, value);
    }
};
#pragma region // StageSky Validation
ValidateFieldOffset32(0x0, StageSky, begin_distance);
ValidateFieldOffset32(0x4, StageSky, end_distance);
ValidateFieldOffset32(0x8, StageSky, color_components);
ValidateFieldOffset32(0x18, StageSky, color);
ValidateStructSize32(0x1C, StageSky);
#pragma endregion

// size: 0x164
struct StageCamera {
    Float3 position; // 0x0
    Float3 facing; // 0xC
    Float3 rotation; // 0x18
    Float3 facing_normalized; // 0x24
    Float3 __float3_30; // 0x30
    Float3 __shaking_float3_A; // 0x3C
    Float3 __shaking_float3_B; // 0x48
    float fov; // 0x54
    Int2 window_resolution; // 0x58
    D3DMATRIXZ view_matrix; // 0x60
    D3DMATRIXZ projection_matrix; // 0xA0
    D3DVIEWPORT9 viewport; // 0xE0
    int32_t camera_index; // 0xF8
    Int2 __int2_FC; // 0xFC
    Int2 __int2_104; // 0x104
    D3DVIEWPORT9 __viewport_10C; // 0x10C
    D3DVIEWPORT9 __viewport_124; // 0x124
    Float3 __float3_13C; // 0x13C
    StageSky sky; // 0x148
    // 0x164

    inline void __copy_int2_FC_to_anm_manager();
};
#pragma region // StageCamera Validation
ValidateFieldOffset32(0x0, StageCamera, position);
ValidateFieldOffset32(0xC, StageCamera, facing);
ValidateFieldOffset32(0x18, StageCamera, rotation);
ValidateFieldOffset32(0x24, StageCamera, facing_normalized);
ValidateFieldOffset32(0x30, StageCamera, __float3_30);
ValidateFieldOffset32(0x3C, StageCamera, __shaking_float3_A);
ValidateFieldOffset32(0x48, StageCamera, __shaking_float3_B);
ValidateFieldOffset32(0x54, StageCamera, fov);
ValidateFieldOffset32(0x58, StageCamera, window_resolution);
ValidateFieldOffset32(0x60, StageCamera, view_matrix);
ValidateFieldOffset32(0xA0, StageCamera, projection_matrix);
ValidateFieldOffset32(0xE0, StageCamera, viewport);
ValidateFieldOffset32(0xF8, StageCamera, camera_index);
ValidateFieldOffset32(0xFC, StageCamera, __int2_FC);
ValidateFieldOffset32(0x104, StageCamera, __int2_104);
ValidateFieldOffset32(0x10C, StageCamera, __viewport_10C);
ValidateFieldOffset32(0x124, StageCamera, __viewport_124);
ValidateFieldOffset32(0x13C, StageCamera, __float3_13C);
ValidateFieldOffset32(0x148, StageCamera, sky);
ValidateStructSize32(0x164, StageCamera);
#pragma endregion

// size: 0x124
struct ScreenshotManager {
    BITMAPFILEHEADER bmp_header; // 0x0
    padding_bytes(0x2); // 0xE
    LPBITMAPINFOHEADER info_header; // 0x10
    unknown_fields(0x8); // 0x14
    int32_t __int_1C; // 0x1C
    char filename[MAX_PATH]; // 0x20
};
#pragma region // ScreenshotManager Validation
ValidateFieldOffset32(0x0, ScreenshotManager, bmp_header);
ValidateFieldOffset32(0x10, ScreenshotManager, info_header);
ValidateFieldOffset32(0x1C, ScreenshotManager, __int_1C);
ValidateFieldOffset32(0x20, ScreenshotManager, filename);
ValidateStructSize32(0x124, ScreenshotManager);
#pragma endregion

typedef struct AnmVM AnmVM;
typedef struct AnmLoaded AnmLoaded;

// size: 0x1C
struct ZUNThread {
    //void* vtable; // 0x0
    HANDLE thread; // 0x4
    uint32_t tid; // 0x8
    BOOL __bool_C; // 0xC
    BOOL __bool_10; // 0x10
    HMODULE* phModule; // 0x14
    _beginthreadex_proc_type start_func; // 0x18
    // 0x1C

    // 0x402B30
    // Might be destructor?
    dllexport gnu_noinline void stop_and_cleanup() asm_symbol_rel(0x402B30) {
        if (HANDLE thread_handle = this->thread) {
            this->__bool_C = true;
            this->__bool_10 = false;
            if (WaitForSingleObject(thread_handle, 200) == WAIT_TIMEOUT) {
                do {
                    this->__bool_C = true;
                    this->__bool_10 = false;
                    Sleep(1);
                } while (WaitForSingleObject(this->thread, 200) == WAIT_TIMEOUT);
            }
            CloseHandle(this->thread);
            this->thread = NULL;
            this->start_func = NULL;
        }
    }

    // 0x402BB0
    dllexport gnu_noinline void start(_beginthreadex_proc_type func, void* arg_list) asm_symbol_rel(0x402BB0) {
        this->stop_and_cleanup();
        this->start_func = func;
        this->__bool_10 = true;
        this->__bool_C = false;
        this->thread = (HANDLE)_beginthreadex(NULL, 0, func, arg_list, 0, &this->tid);
    }

    virtual ~ZUNThread() {
        this->stop_and_cleanup();
    }
};
#pragma region // ZUNThread Validation
//ValidateVirtualFieldOffset32(0x0, ZUNThread, vtable);
ValidateVirtualFieldOffset32(0x4, ZUNThread, thread);
ValidateVirtualFieldOffset32(0x8, ZUNThread, tid);
ValidateVirtualFieldOffset32(0xC, ZUNThread, __bool_C);
ValidateVirtualFieldOffset32(0x10, ZUNThread, __bool_10);
ValidateVirtualFieldOffset32(0x14, ZUNThread, phModule);
ValidateVirtualFieldOffset32(0x18, ZUNThread, start_func);
ValidateStructSize32(0x1C, ZUNThread);
#pragma endregion

// Couldn't figure out how to get the constructor right otherwise...
struct ZUNThreadB : ZUNThread {
    inline ZUNThreadB() {
        this->thread = NULL;
        this->tid = 0;
        this->__bool_C = FALSE;
        this->__bool_10 = FALSE;
    }
};

// size: 0x58
struct D3DThread : ZUNThread {
    unknown_fields(0x3C); // 0x1C
    // 0x58

    // 0x46FDD0
    virtual ~D3DThread() {

    }
};
#pragma region // D3DThread Validation
ValidateStructSize32(0x58, D3DThread);
#pragma endregion

extern "C" {
    // 0x4CD950
    extern D3DThread D3D_THREAD_A asm("_D3D_THREAD_A");
    // 0x4CDAE0
    extern ZUNThread UNKNOWN_THREAD_A asm("_UNKNOWN_THREAD_A");
}

// size: 0xC
struct ZUNTask {
    union {
        uint32_t task_flags; // 0x0
        struct {
            uint32_t : 1;
            uint32_t __unknown_task_flag_A : 1;
        };
    };
    UpdateFunc* on_tick_func; // 0x4
    UpdateFunc* on_draw_func; // 0x8
    // 0xC

    // 0x42A9A0
    dllexport void enable_funcs() {
        if (UpdateFunc* on_tick = this->on_tick_func) {
            on_tick->run_on_update = true;
        }
        if (UpdateFunc* on_draw = this->on_draw_func) {
            on_draw->run_on_update = true;
        }
    }

    inline BOOL on_tick_enabled() {
        UpdateFunc* on_tick = this->on_tick_func;
        if (on_tick && on_tick->run_on_update) {
            return TRUE;
        }
        return FALSE;
    }

    inline BOOL on_draw_enabled() {
        UpdateFunc* on_draw = this->on_draw_func;
        if (on_draw && on_draw->run_on_update) {
            return TRUE;
        }
        return FALSE;
    }
};

extern "C" {
    // 0x57095C
    extern size_t JOYPAD_INDEX asm("_JOYPAD_INDEX");
}

static constexpr LONG JOYPAD_MIN_RANGE = -1000;
static constexpr LONG JOYPAD_MAX_RANGE = 1000;

// size: 0x38
struct FpsCounter : ZUNTask {
    //ZUNTask base; // 0x0
    int __dword_C; // 0xC
    unknown_fields(0x10); // 0x10
    double __double_20; // 0x20
    double __double_28; // 0x28
    float __float_30; // 0x30
    unknown_fields(0x4); // 0x34
    // 0x38

    inline void zero_contents() {
        zero_this();
    }

    inline FpsCounter() {
        this->zero_contents();
        this->__unknown_task_flag_A = true;

    }

    inline float calc_slowdown_rate() {
        return 100.0f - (float)(this->__double_20 / this->__double_28) * 100.0f;
    }

    // 0x43A2D0
    dllexport gnu_noinline FpsCounter* stdcall destroy() asm_symbol_rel(0x43A2D0);
};

extern "C" {
    // 0x4CF2DC
    extern FpsCounter* FPS_COUNTER_PTR asm("_FPS_COUNTER_PTR");
    // 0x507644
}

// 0x43A2D0
dllexport gnu_noinline FpsCounter* stdcall FpsCounter::destroy() {
    UpdateFuncRegistry* update_func_registry = UPDATE_FUNC_REGISTRY_PTR;
    if (UpdateFunc* update_func = this->on_draw_func) {
        CRITICAL_SECTION_MANAGER.enter_section(UpdateFuncRegistry_CS);
        update_func_registry->delete_func(update_func);
        CRITICAL_SECTION_MANAGER.leave_section(UpdateFuncRegistry_CS);
    }
    FPS_COUNTER_PTR = NULL;
    delete this;
    return this;
}

static inline constexpr size_t msg_variant_count = 4;

// size: 0x28
struct StageDataInner {
    int32_t __spell_anm_indexA; // 0x0 (0x34)
    int32_t __spell_anm_scriptA; // 0x4 (0x38)
    int32_t __spell_flag_state; // 0x8 (0x3C)
    int32_t __spell_anm_indexB; // 0xC (0x40)
    int32_t __spell_anm_scriptB; // 0x10 (0x44)
    int32_t __intro_anm_index; // 0x14 (0x48)
    int32_t __intro_anm_script; // 0x18 (0x4C)
    int32_t __portrait_anm_file_index; // 0x1C (0x50)
    int32_t __portrait_anm_script; // 0x20 (0x54)
    unknown_fields(0x4); // 0x24
    // 0x28
};

// size: 0xD4
struct StageData {
    int32_t stage_number; // 0x0
    const char* std_filename; // 0x4
    const char* ecl_filename; // 0x8
    const char* __string_array_C[2]; // 0xC
    const char* msg_filenames[msg_variant_count]; // 0x14
    const char* logo_anm_filename; // 0x24
    unknown_fields(0x4); // 0x28
    int32_t music_index; // 0x2C
    unknown_fields(0x4); // 0x30
    StageDataInner innner[1]; // 0x34 this may actually be shifted up by 1 dword

    int32_t __anm_file_index_78; // 0x78
    int32_t __anm_script_7C; // 0x7C
    int32_t __anm_script_80; // 0x80
    // 0x84
};

extern "C" {
    // 0x4CF428
    extern StageData* STAGE_DATA_PTR asm("_STAGE_DATA_PTR");
}

typedef struct AnmManager AnmManager;

#define ANM_FULL_ID_BITS (32)
#define ANM_FAST_ID_BITS (15)
#define ANM_SLOW_ID_BITS (ANM_FULL_ID_BITS - ANM_FAST_ID_BITS)
#define INVALID_FAST_ID uint_width_max(ANM_FAST_ID_BITS)

// size: 0x4
union AnmID {
    uint32_t full;
    struct {
        uint32_t fast_id : ANM_FAST_ID_BITS;
        uint32_t slow_id : ANM_SLOW_ID_BITS;
    };

    inline constexpr AnmID() : full(0) {}

    inline constexpr AnmID(const AnmID& id) : full(id.full) {}

    template<typename T> requires(std::is_integral_v<T>)
    inline constexpr AnmID(const T& raw) : full(raw) {}

    // 0x488E30
    dllexport AnmVM* get_vm_ptr() asm_symbol_rel(0x488E30);

    inline int32_t run_vm();

    // 0x488E50
    dllexport void interrupt_tree(int32_t interrupt_index) asm_symbol_rel(0x488E50);

    // 0x488E70
    dllexport void __unknown_tree_set_J() asm_symbol_rel(0x488E70);

    inline void __unknown_tree_set_J(AnmManager* anm_manager);

    // 0x488EB0
    dllexport void __unknown_tree_clear_J() asm_symbol_rel(0x488EB0);

    // 0x488F50
    dllexport void mark_tree_for_delete() asm_symbol_rel(0x488F50);

    inline void mark_tree_for_delete(AnmManager* anm_manager);

    inline void set_position(Float3* position);

    // 0x488F70
    dllexport void thiscall set_controller_position(Float3* position) asm_symbol_rel(0x488F70);

    // 0x488FD0
    dllexport void thiscall __sub_488FD0(int32_t script) asm_symbol_rel(0x488FD0);

    // 0x4892F0
    dllexport void thiscall set_color1(D3DCOLOR color) asm_symbol_rel(0x4892F0);

    // 0x489230
    dllexport gnu_noinline AnmVM* thiscall __wtf_child_list_jank_A(int32_t script, uint32_t arg2) asm_symbol_rel(0x489230);

    inline constexpr operator uint32_t() const {
        return this->full;
    }
    template<typename T> requires(std::is_integral_v<T>)
    inline constexpr AnmID& operator=(const T& raw) {
        this->full = raw;
        return *this;
    }

    AnmID thiscall __sub_489140();
};
#pragma region // AnmID Validation
ValidateFieldOffset32(0x0, AnmID, full);
ValidateStructSize32(0x4, AnmID);
#pragma endregion

extern "C" {
    // 0x4C5F90
    extern int32_t ACHIEVEMENT_MODE_STATE asm("_ACHIEVEMENT_MODE_STATE");
}

// size: 0xB60
struct Supervisor {
    HINSTANCE current_instance; // 0x0
    LPDIRECT3D9 d3d; // 0x4
    LPDIRECT3DDEVICE9 d3d_device; // 0x8
    LPDIRECTINPUT8 dinput; // 0xC
    RECT window_rect; // 0x10
    LPDIRECTINPUTDEVICE8 keyboard_device; // 0x20;
    LPDIRECTINPUTDEVICE8 joypad_devices[1]; // 0x24;
    unknown_fields(0x4); // 0x28
    DIDEVCAPS __joypad_caps; // 0x2C
    HWND main_window_handle; // 0x58
    D3DMATRIXZ __view_matrix_5C; // 0x5C
    D3DMATRIXZ __projection_matrix_9C; // 0x9C
    D3DVIEWPORT9 __viewport_DC; // 0xDC
    D3DPRESENT_PARAMETERS present_parameters; // 0xF4
    unknown_fields(0x70); // 0x12C
    D3DDISPLAYMODE display_mode; // 0x19C
    LPDIRECT3DSURFACE9 __surface_1AC; // 0x1AC
    LPDIRECT3DSURFACE9 __surface_1B0; // 0x1B0
    LPDIRECT3DSURFACE9 back_buffer; // 0x1B4
    unknown_fields(0x4); // 0x1B8
    AnmVM* __arcade_vm_ptr_A; // 0x1BC
    AnmVM* __arcade_vm_ptr_B; // 0x1C0
    AnmVM* __arcade_vm_ptr_C; // 0x1C4
    AnmVM* __arcade_vm_ptr_D; // 0x1C8
    unknown_fields(0x4); // 0x1CC
    AnmID __vm_id_1D0; // 0x1D0
    Config config; // 0x1D4
    StageCamera cameras[4]; // 0x25C
    StageCamera* current_camera_ptr; // 0x7EC
    int32_t current_camera_index; // 0x7F0
    int32_t gamemode_current; // 0x7F4
    int32_t gamemode_switch; // 0x7F8
    int32_t gamemode_previous; // 0x7FC
    int __dword_800; // 0x800
    int32_t __int_804; // 0x804
    unknown_fields(0x10); // 0x808
    int __int_818; // 0x818
    unknown_fields(0x4); // 0x81C
    BOOL disable_vsync; // 0x820
    int __dword_824; // 0x824
    int __dword_828; // 0x828
    AnmLoaded* text_anm; // 0x82C
    unknown_fields(0x4); // 0x830
    union {
        uint32_t flags; // 0x834
        struct {
            uint32_t __unknown_flag_D : 1; // 1
            uint32_t __unknown_flag_C : 1; // 2
            uint32_t __unknown_flag_E : 1; // 3
            uint32_t : 1; // 4
            uint32_t __unknown_flag_F : 1; // 5
            uint32_t : 1; // 6
            uint32_t __unknown_flag_A : 1; // 7
            uint32_t __unknown_bitfield_A : 2; // 8-9
            uint32_t : 4; // 10-13
            uint32_t __unknown_flag_B : 1; // 14
        };
    };
    uint32_t initial_rng_seed; // 0x838
    unknown_fields(0x4); // 0x83C
    D3DCAPS9 d3dcaps; // 0x840
    ScreenshotManager screenshot_manager; // 0x970
    ZUNThread __thread_A94; // 0xA94
    ZUNThread __thread_AB0; // 0xAB0
    unknown_fields(0x4); // 0xACC
    int32_t __int_AD0; // 0xAD0
    unknown_fields(0x54); // 0xAD4
    BOOL fog_enabled; // 0xB28
    BOOL zwrite_enabled; // 0xB2C
    int32_t game_exe_checksum; // 0xB30
    int32_t game_exe_file_size; // 0xB34
    int32_t ver_file_size; // 0xB38
    void* ver_file_buffer; // 0xB3C
    unknown_fields(0x10); // 0xB40
    double __double_B50; // 0xB50
    D3DCOLOR background_color; // 0xB58
    probably_padding_bytes(0x4); // 0xB5C
    // 0xB60

    // 0x456180
    dllexport static gnu_noinline void __release_rendering_surfaces() asm_symbol_rel(0x456180);
    
    // 0x453460
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_tick_A(void* self) asm_symbol_rel(0x453460) {

    }

    // 0x4553B0
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_A(void* self) asm_symbol_rel(0x4553B0) {

    }

    // 0x455610
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_B(void* self) asm_symbol_rel(0x455610) {

    }

    // 0x455530
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_C(void* self) asm_symbol_rel(0x455530) {

    }

    // 0x455A70
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_D(void* self) asm_symbol_rel(0x455A70) {

    }

    // 0x4559A0
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_E(void* self) asm_symbol_rel(0x4559A0) {

    }
    
    // 0x455BC0
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_F(void* self) asm_symbol_rel(0x455BC0) {

    }

    // 0x455B10
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_G(void* self) asm_symbol_rel(0x455B10) {

    }

    // 0x455CF0
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_H(void* self) asm_symbol_rel(0x455CF0) {

    }

    // 0x455C90
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_I(void* self) asm_symbol_rel(0x455C90) {

    }

    // 0x455D40
    dllexport static gnu_noinline UpdateFuncRet fastcall __unknown_on_draw_J(void* self) asm_symbol_rel(0x455D40) {

    }

    // 0x453640
    dllexport static gnu_noinline ZUNResult fastcall on_registration(void* self) asm_symbol_rel(0x453640);
    
    // 0x453DB0
    dllexport static gnu_noinline ZUNResult initialize() asm_symbol_rel(0x453DB0);

    // 0x4543A0
    dllexport gnu_noinline ZUNResult thiscall load_config_file(int) asm_symbol_rel(0x4543A0);

    // 0x454760
    dllexport HRESULT thiscall d3d_enable_fog() asm_symbol_rel(0x454760);

    // 0x4547A0
    dllexport HRESULT thiscall d3d_disable_fog() asm_symbol_rel(0x4547A0);

    // 0x4547E0
    dllexport HRESULT thiscall d3d_enable_zwrite() asm_symbol_rel(0x4547E0);

    // 0x454820
    dllexport HRESULT thiscall d3d_disable_zwrite() asm_symbol_rel(0x454820);

private:
    // 0x454860
    dllexport static gnu_noinline int32_t stdcall __start_thread_A94(_beginthreadex_proc_type func, int) asm_symbol_rel(0x454860);
public:
    inline static int32_t __start_thread_A94(_beginthreadex_proc_type func) {
        return __start_thread_A94(func, UNUSED_DWORD);
    }

    // 0x4548E0
    dllexport static gnu_noinline void stdcall __sub_4548E0(StageCamera* camera) asm_symbol_rel(0x4548E0);

    // 0x454950
    dllexport static gnu_noinline void stdcall __sub_454950(StageCamera* camera) asm_symbol_rel(0x454950) {
        uint32_t height = camera->viewport.Height;
        float x = (float)camera->viewport.X + (float)camera->viewport.Width * 0.5f;
        float viewport_height = (float)height;
        height /= 2;
        float y = (float)camera->viewport.Y + viewport_height * 0.5f;
        float eye_z = (float)height / ztanf(camera->fov * 0.5f);
        D3DXVECTOR3 eye;
        D3DXVECTOR3 at;
        D3DXVECTOR3 up;
        up.x = 0.0f;
        up.y = -1.0f;
        up.z = 0.0f;
        eye.z = eye_z;
        eye.x = x;
        eye.y = y;
        at.x = x;
        at.y = y;
        at.z = 0.0f;
        D3DXMatrixLookAtLH((D3DXMATRIX*)&camera->view_matrix, &eye, &at, &up);
        float aspect = (float)camera->viewport.Width / (float)camera->viewport.Height;
        D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)&camera->projection_matrix, camera->fov, aspect, 1.0f, 10000.0f);
    }

    // 0x454B20
    dllexport gnu_noinline void thiscall __initialize_cameras() asm_symbol_rel(0x454B20);

    // 0x454F50
    dllexport static gnu_noinline void __camera2_sub_454F50() asm_symbol_rel(0x454F50);

    // 0x41B330
    dllexport void thiscall set_camera_by_index(uint32_t index) asm_symbol_rel(0x41B330);
    // 0x41B3B0
    dllexport static gnu_noinline void stdcall set_camera2_alt(uint32_t = UNUSED_DWORD) asm_symbol_rel(0x41B3B0);

    inline void thiscall set_camera_by_index_disable_fog(uint32_t index);
    inline void thiscall set_camera_by_index_disable_zwrite(uint32_t index);
};
#pragma region // Supervisor Validation
ValidateFieldOffset32(0x0, Supervisor, current_instance);
ValidateFieldOffset32(0x4, Supervisor, d3d);
ValidateFieldOffset32(0x8, Supervisor, d3d_device);
ValidateFieldOffset32(0xC, Supervisor, dinput);
ValidateFieldOffset32(0x10, Supervisor, window_rect);
ValidateFieldOffset32(0x20, Supervisor, keyboard_device);
ValidateFieldOffset32(0x24, Supervisor, joypad_devices);
ValidateFieldOffset32(0x2C, Supervisor, __joypad_caps);
ValidateFieldOffset32(0x58, Supervisor, main_window_handle);
ValidateFieldOffset32(0x5C, Supervisor, __view_matrix_5C);
ValidateFieldOffset32(0x9C, Supervisor, __projection_matrix_9C);
ValidateFieldOffset32(0xDC, Supervisor, __viewport_DC);
ValidateFieldOffset32(0xF4, Supervisor, present_parameters);
ValidateFieldOffset32(0x19C, Supervisor, display_mode);
ValidateFieldOffset32(0x1AC, Supervisor, __surface_1AC);
ValidateFieldOffset32(0x1B0, Supervisor, __surface_1B0);
ValidateFieldOffset32(0x1B4, Supervisor, back_buffer);
ValidateFieldOffset32(0x1BC, Supervisor, __arcade_vm_ptr_A);
ValidateFieldOffset32(0x1C0, Supervisor, __arcade_vm_ptr_B);
ValidateFieldOffset32(0x1C4, Supervisor, __arcade_vm_ptr_C);
ValidateFieldOffset32(0x1C8, Supervisor, __arcade_vm_ptr_D);
ValidateFieldOffset32(0x1D0, Supervisor, __vm_id_1D0);
ValidateFieldOffset32(0x1D4, Supervisor, config);
ValidateFieldOffset32(0x25C, Supervisor, cameras);
ValidateFieldOffset32(0x7EC, Supervisor, current_camera_ptr);
ValidateFieldOffset32(0x7F0, Supervisor, current_camera_index);
ValidateFieldOffset32(0x7F4, Supervisor, gamemode_current);
ValidateFieldOffset32(0x7F8, Supervisor, gamemode_switch);
ValidateFieldOffset32(0x7FC, Supervisor, gamemode_previous);
ValidateFieldOffset32(0x800, Supervisor, __dword_800);
ValidateFieldOffset32(0x804, Supervisor, __int_804);
ValidateFieldOffset32(0x818, Supervisor, __int_818);
ValidateFieldOffset32(0x820, Supervisor, disable_vsync);
ValidateFieldOffset32(0x824, Supervisor, __dword_824);
ValidateFieldOffset32(0x828, Supervisor, __dword_828);
ValidateFieldOffset32(0x82C, Supervisor, text_anm);
ValidateFieldOffset32(0x834, Supervisor, flags);
ValidateFieldOffset32(0x838, Supervisor, initial_rng_seed);
ValidateFieldOffset32(0x840, Supervisor, d3dcaps);
ValidateFieldOffset32(0x970, Supervisor, screenshot_manager);
ValidateFieldOffset32(0xA94, Supervisor, __thread_A94);
ValidateFieldOffset32(0xAB0, Supervisor, __thread_AB0);
ValidateFieldOffset32(0xAD0, Supervisor, __int_AD0);
ValidateFieldOffset32(0xB28, Supervisor, fog_enabled);
ValidateFieldOffset32(0xB2C, Supervisor, zwrite_enabled);
ValidateFieldOffset32(0xB30, Supervisor, game_exe_checksum);
ValidateFieldOffset32(0xB34, Supervisor, game_exe_file_size);
ValidateFieldOffset32(0xB38, Supervisor, ver_file_size);
ValidateFieldOffset32(0xB3C, Supervisor, ver_file_buffer);
ValidateFieldOffset32(0xB50, Supervisor, __double_B50);
ValidateFieldOffset32(0xB58, Supervisor, background_color);
ValidateStructSize32(0xB60, Supervisor);
#pragma endregion

extern "C" {
    // 0x4CCDF0
    extern Supervisor SUPERVISOR asm("_SUPERVISOR");
}

// 0x456180
dllexport gnu_noinline void Supervisor::__release_rendering_surfaces() {
    SAFE_RELEASE(SUPERVISOR.__surface_1AC);
    SAFE_RELEASE(SUPERVISOR.__surface_1B0);
    SAFE_RELEASE(SUPERVISOR.back_buffer);
    SUPERVISOR.__surface_1AC = NULL; // why tho
}

// 0x453DB0
dllexport gnu_noinline ZUNResult Supervisor::initialize() {
    SUPERVISOR.gamemode_current = -2;
    SUPERVISOR.gamemode_switch = 0;
    SUPERVISOR.__dword_800 = 0;
    UpdateFunc* update_func = new UpdateFunc(&Supervisor::__unknown_on_tick_A, true, &SUPERVISOR);
    update_func->on_init_func = &Supervisor::on_registration;
    ZUNResult ret = UpdateFuncRegistry::register_on_tick(update_func, 1);
    if (ZUN_SUCCEEDED(ret)) {
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_A, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 1);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_B, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 14);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_C, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 15);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_D, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 25);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_E, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 26);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_F, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 44);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_G, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 45);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_H, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 58);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_I, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 59);
        update_func = new UpdateFunc(&Supervisor::__unknown_on_draw_J, true, &SUPERVISOR);
        UpdateFuncRegistry::register_on_draw(update_func, 92);
        SUPERVISOR.d3d_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &SUPERVISOR.back_buffer);
        ret = ZUN_SUCCESS;
    }
    return ret;
}

// 0x454860
dllexport gnu_noinline int32_t Supervisor::__start_thread_A94(_beginthreadex_proc_type func, int) {
    CRITICAL_SECTION_MANAGER.enter_section(__supervisor_thread_cs_1);
    {
        clang_forceinline SUPERVISOR.__thread_A94.start(func, NULL);
    }
    CRITICAL_SECTION_MANAGER.leave_section(__supervisor_thread_cs_1);
    return 0;
}

inline uint32_t InputState::get_joypad(uint32_t buttons) {
    uint32_t prev_buttons = buttons;
    if (SUPERVISOR.joypad_devices[0]) {
        if (FAILED(SUPERVISOR.joypad_devices[0]->Poll())) {
            for (size_t i = 0; i < 400; ++i) {
                // This isn't even listed as a valid return value in the docs?
                if (SUPERVISOR.joypad_devices[0]->Acquire() != DIERR_INPUTLOST) {
                    break;
                }
            }
        }
        else {
            DIJOYSTATE2 state = {};
            if (SUCCEEDED(SUPERVISOR.joypad_devices[0]->GetDeviceState(sizeof(state), &state))) {
                // This code is mangled AF to the point that no sane compiler will ever
                // output it, so I'm not even going to attempt to match the codegen.

                int32_t mapping;

                mapping = this->joypad_mapping.shoot;
                if (mapping >= 0 && (int8_t)state.rgbButtons[mapping] < 0) {
                    buttons |= BUTTON_SHOOT;
                }

                mapping = this->joypad_mapping.bomb;
                if (mapping >= 0 && (int8_t)state.rgbButtons[mapping] < 0) {
                    buttons |= BUTTON_BOMB;
                }

                mapping = this->joypad_mapping.pause;
                if (mapping >= 0 && (int8_t)state.rgbButtons[mapping] < 0) {
                    buttons |= BUTTON_PAUSE;
                }

                mapping = this->joypad_mapping.focus;
                if (mapping >= 0 && (int8_t)state.rgbButtons[mapping] < 0) {
                    buttons |= BUTTON_FOCUS;
                }

                mapping = this->joypad_mapping.use_card;
                if (mapping >= 0 && (int8_t)state.rgbButtons[mapping] < 0) {
                    buttons |= BUTTON_USE_CARD;
                }

                mapping = this->joypad_mapping.switch_card;
                if (mapping >= 0 && (int8_t)state.rgbButtons[mapping] < 0) {
                    buttons |= BUTTON_SWITCH_CARD;
                }

                uint32_t axes = 0;

                int32_t deadzone_x = SUPERVISOR.config.deadzone_x;
                int32_t deadzone_y = SUPERVISOR.config.deadzone_y;

                if (state.lX < -deadzone_x) {
                    axes |= BUTTON_LEFT;
                }
                if (state.lY < -deadzone_y) {
                    axes |= BUTTON_UP;
                }
                if (state.lX > deadzone_x) {
                    axes |= BUTTON_RIGHT;
                }
                if (state.lY > SUPERVISOR.config.deadzone_y) {
                    axes |= BUTTON_DOWN;
                }

                buttons |= axes;

                if (buttons != prev_buttons) {
                    this->__device_type = InputJoypad;
                }
            }
        }
    }
    return buttons;
}

static inline constexpr size_t CHARACTER_COUNT = 4;
static inline constexpr size_t SHOTTYPES_PER_CHARACTER = 1;
static inline constexpr size_t SHOTTYPE_COUNT = CHARACTER_COUNT * SHOTTYPES_PER_CHARACTER;
static inline constexpr size_t DIFFICULTY_COUNT = 6;
static inline constexpr int32_t STAGE_COUNT = 8;

static inline constexpr int32_t RANK_BOUND = 1024;
static inline constexpr int32_t MAX_RANK = +RANK_BOUND; // 1024
static inline constexpr int32_t MIN_RANK = -RANK_BOUND; // -1024
static inline constexpr int32_t RANK_RANGE = RANK_BOUND * 2; // 2048

// This is used as an index into the scorefile section B array
static inline constexpr size_t SCOREFILE_TOTALS = SHOTTYPE_COUNT;

static inline void __update_bomb_ui();

// size: 0xFC
struct Globals {
    int32_t current_stage; // 0x0
    int32_t __stage_number_related_4; // 0x4
    int32_t chapter; // 0x8
    int32_t __counter_C; // 0xC (Sound related?)
    int32_t __counter_10; // 0x10 (Sound related?)
    int32_t __counter_14; // 0x14 (Sound related?)
    int32_t character; // 0x18
    int32_t shottype; // 0x1C
    int32_t score; // 0x20
    int32_t difficulty; // 0x24
    int32_t continues; // 0x28
    int32_t rank; // 0x2C
    int32_t graze_in_stage; // 0x30
    int32_t graze; // 0x34
    int32_t __ecl_var_9907; // 0x38
    int32_t miss_count_in_game; // 0x3C
    int __dword_40; // 0x40 (Maybe point_items_collected_in_stage?)
    int32_t point_items_collected_in_game; // 0x44
    int32_t point_item_value; // 0x48
    int32_t min_point_item_value; // 0x4C
    int32_t max_point_item_value; // 0x50
    int32_t money_collected_in_game; // 0x54
    int32_t current_money; // 0x58
    int32_t current_power; // 0x5C
    int32_t max_power; // 0x60
    int32_t power_per_level; // 0x64
    int __dword_68; // 0x68
    int32_t life_stocks; // 0x6C
    int32_t life_fragments; // 0x70
    int32_t lives_added; // 0x74
    int32_t life_stock_max; // 0x78
    int32_t bomb_stocks; // 0x7C
    int32_t bomb_fragments; // 0x80
    int32_t bomb_stocks_for_new_life; // 0x84
    int32_t bomb_stock_max; // 0x88
    int32_t __int_8C; // 0x8C
    int32_t __int_90; // 0x90
    int __dword_94; // 0x94
    int __dword_98; // 0x98
    int __dword_9C; // 0x9C
    int __dword_A0; // 0xA0
    int __dword_A4; // 0xA4
    int __dword_A8; // 0xA8
    int __dword_AC; // 0xAC
    int __dword_B0; // 0xB0
    int __dword_B4; // 0xB4
    int __dword_B8; // 0xB8
    int __dword_BC; // 0xBC
    int __dword_C0; // 0xC0
    Timer __timer_C4; // 0xC4
    Timer __timer_D8; // 0xD8
    int __dword_EC; // 0xEC
    int __dword_F0; // 0xF0
    int __dword_F4; // 0xF4
    union {
        uint32_t flags; // 0xF8
        struct {
            uint32_t : 4; // 1-4
            uint32_t __unknown_field_A : 2; // 5-6
            uint32_t : 5; // 7-11
            uint32_t __unknown_flag_A : 1; // 12
        };
    };
    // 0xFC

    inline int32_t shottype_index() const {
        return this->character + this->shottype * SHOTTYPES_PER_CHARACTER;
    }

    // 0x417A60
    dllexport gnu_noinline Globals& thiscall operator=(const Globals& rhs) {
        this->current_stage = rhs.current_stage;
        this->__stage_number_related_4 = rhs.__stage_number_related_4;
        this->chapter = rhs.chapter;
        this->__counter_C = rhs.__counter_C;
        this->__counter_10 = rhs.__counter_10;
        this->__counter_14 = rhs.__counter_14;
        this->character = rhs.character;
        this->shottype = rhs.shottype;
        this->score = rhs.score;
        this->difficulty = rhs.difficulty;
        this->continues = rhs.continues;
        this->rank = rhs.rank;
        this->graze_in_stage = rhs.graze_in_stage;
        this->graze = rhs.graze;
        this->__ecl_var_9907 = rhs.__ecl_var_9907;
        this->miss_count_in_game = rhs.miss_count_in_game;
        this->__dword_40 = rhs.__dword_40;
        this->point_items_collected_in_game = rhs.point_items_collected_in_game;
        this->point_item_value = rhs.point_item_value;
        this->min_point_item_value = rhs.min_point_item_value;
        this->max_point_item_value = rhs.max_point_item_value;
        this->money_collected_in_game = rhs.money_collected_in_game;
        this->current_money = rhs.current_money;
        this->current_power = rhs.current_power;
        this->max_power = rhs.max_power;
        this->power_per_level = rhs.power_per_level;
        this->__dword_68 = rhs.__dword_68;
        this->life_stocks = rhs.life_stocks;
        this->life_fragments = rhs.life_fragments;
        this->lives_added = rhs.lives_added;
        this->life_stock_max = rhs.life_stock_max;
        this->bomb_stocks = rhs.bomb_stocks;
        this->bomb_fragments = rhs.bomb_fragments;
        this->bomb_stocks_for_new_life = rhs.bomb_stocks_for_new_life;
        this->bomb_stock_max = rhs.bomb_stock_max;
        this->__int_8C = rhs.__int_8C;
        this->__int_90 = rhs.__int_90;
        this->__dword_94 = rhs.__dword_94;
        this->__dword_98 = rhs.__dword_98;
        this->__dword_9C = rhs.__dword_9C;
        this->__dword_A0 = rhs.__dword_A0;
        this->__dword_A4 = rhs.__dword_A4;
        this->__dword_A8 = rhs.__dword_A8;
        this->__dword_AC = rhs.__dword_AC;
        this->__dword_B0 = rhs.__dword_B0;
        this->__dword_B4 = rhs.__dword_B4;
        this->__dword_B8 = rhs.__dword_B8;
        this->__dword_BC = rhs.__dword_BC;
        this->__dword_C0 = rhs.__dword_C0;
        this->__timer_C4.set_from_timer(rhs.__timer_C4);
        this->__timer_D8.set_from_timer(rhs.__timer_D8);
        this->__dword_EC = rhs.__dword_EC;
        this->__dword_F0 = rhs.__dword_F0;
        this->__dword_F4 = rhs.__dword_F4;
        this->flags = rhs.flags;
        return *this;
    }

    // 0x412FA0
    dllexport gnu_noinline void thiscall set_power(int32_t value) asm_symbol_rel(0x412FA0) {
        this->current_power = value;
        int32_t power = this->max_power;
        if (value <= power) {
            power = __max(power, this->power_per_level); // Effectively clamps to level 1 power
        }
        this->current_power = power;
    }

    // 0x42A970
    dllexport gnu_noinline void thiscall __set_unknown_flag_A(int32_t value) asm_symbol_rel(0x42A970) {
        this->__unknown_flag_A = value;
    }

    // 0x439EA0
    dllexport gnu_noinline int32_t thiscall get_rank() asm_symbol_rel(0x439EA0) {
        return this->rank;
    }

    template <typename T>
    inline T rank_lerp(const T& min_val, const T& max_val) {
        int32_t rank = this->get_rank();
        return min_val + ((rank + (T)RANK_BOUND) * (max_val - min_val)) / (T)RANK_RANGE;
    }

    // 0x439EB0
    dllexport gnu_noinline void thiscall __add_to_int_90(int32_t value) asm_symbol_rel(0x439EB0) {
        this->__int_90 += value;
    }

    // 00439EC0
    dllexport gnu_noinline void thiscall __add_to_int_8C(int32_t value) asm_symbol_rel(0x439EC0) {
        this->__int_8C += value;
    }

    // 0x4573F0
    dllexport gnu_noinline BOOL thiscall __add_power(int32_t value) asm_symbol_rel(0x4573F0);

private:
    // 0x457690
    dllexport gnu_noinline void thiscall add_bombs(int32_t) asm_symbol_rel(0x457690) {
        this->bomb_stocks += 1;
        int32_t max_bombs = this->bomb_stock_max;
        if (this->bomb_stocks >= max_bombs) {
            this->bomb_fragments = 0;
            if (this->bomb_stocks > max_bombs) {
                this->bomb_stocks = max_bombs;
            }
        }
        else {
            //SOUND_MANAGER.play_sound(46);
        }
        __update_bomb_ui();
    }
public:
    inline void add_bomb() {
        return this->add_bombs(UNUSED_DWORD);
    }

private:
    // 0x4576E0
    dllexport gnu_noinline void thiscall add_bomb_fragments(int32_t) asm_symbol_rel(0x4576E0) {
        if (this->bomb_stocks >= this->bomb_stock_max) {
            this->bomb_fragments = 0;
            return;
        }
        this->bomb_fragments += 1;
        if (this->bomb_fragments >= 3) {
            this->bomb_fragments = 0;
            this->add_bomb();
        }
        __update_bomb_ui();
    }
public:
    inline void add_bomb_fragment() {
        return this->add_bomb_fragments(UNUSED_DWORD);
    }

    // 0x457480
    dllexport gnu_noinline BOOL thiscall subtract_power(int32_t amount) asm_symbol_rel(0x457480) {
        int32_t power = this->current_power;
        int32_t min_power = this->power_per_level;
        if (power <= min_power) {
            return false;
        }
        power -= amount;
        this->current_power = power;
        if (power < min_power) {
            this->current_power = min_power;
            power = min_power;
        }

        int32_t prev_level = (power + amount) / min_power;
        int32_t new_level = power / min_power;
        return prev_level != new_level;
    }

    // 0x4574D0
    dllexport gnu_noinline void thiscall __subtract_bomb() asm_symbol_rel(0x4574D0) {
        if (--this->bomb_stocks < 0) {
            this->bomb_stocks = 0;
        }
        int32_t max_bombs = this->bomb_stock_max;
        if (this->bomb_stocks > max_bombs) {
            this->bomb_stocks = max_bombs;
        }
        __update_bomb_ui();
    }

    inline void add_graze() {
        int32_t stage_graze = this->graze_in_stage;
        int32_t total_graze = this->graze;
        ++stage_graze;
        ++total_graze;
        this->graze_in_stage = __min(stage_graze, 99999999);
        this->graze = __min(total_graze, 99999999);
    }
};

// 0x130
struct GameManager {
    int32_t __high_score; // 0x0
    int32_t __high_score_continues; // 0x4
    union {
        uint32_t flags; // 0x8
        struct {
            uint32_t __unknown_flag_A : 1; // 1
            uint32_t __unknown_flag_B : 1; // 2
            uint32_t __unknown_flag_C : 1; // 3
            uint32_t __unknown_flag_D : 1; // 4
            uint32_t __unknown_field_A : 2; // 5-6
            uint32_t __unknown_flag_E : 1; // 7
        };
    };
    int32_t __int_C; // 0xC
    int32_t continue_credits; // 0x10
    int32_t __int_14; // 0x14
    int32_t __int_18; // 0x18
    Globals globals; // 0x1C
    int32_t __int_118; // 0x118
    probably_padding_bytes(0x4); // 0x11C
    double game_time_double; // 0x120
    int __dword_128; // 0x128
    probably_padding_bytes(0x4); // 0x12C
    // 0x130

    // 0x406AA0
    dllexport gnu_noinline int32_t thiscall get_chapter() asm_symbol_rel(0x406AA0) {
        return this->globals.chapter;
    }

    // 0x418D60
    dllexport gnu_noinline int32_t thiscall get_current_stage() asm_symbol_rel(0x418D60) {
        return this->globals.current_stage;
    }

    // 0x42A990
    dllexport bool thiscall __unknown_field_A_is_2() asm_symbol_rel(0x42A990) {
        return this->__unknown_field_A == 2;
    }

    // 0x42AA20
    dllexport gnu_noinline int32_t thiscall get_difficulty() asm_symbol_rel(0x42AA20) {
        return this->globals.difficulty;
    }

    // 0x4630B0
    dllexport gnu_noinline void __set_unknown_field_A(int32_t value) asm_symbol_rel(0x4630B0) {
        if (this->__unknown_field_A != 2) {
            this->globals.__ecl_var_9907 = -1;
        }
        this->__unknown_field_A = value;
    }

    inline void add_to_score(uint32_t value) {
        uint32_t new_score = this->globals.score + value / 10;
        if (new_score > 1000000000) {
            new_score = 999999999;
        }
        this->globals.score = new_score;
    }
};

extern "C" {
    // 0x4CCCC0
    extern GameManager GAME_MANAGER asm("_GAME_MANAGER");
}

// size: 0x2108
struct WindowData {
    HWND window; // 0x0
    HWND resolution_dialogue; // 0x4
    int __dword_8; // 0x8
    HINSTANCE current_instance; // 0xC
    BOOL window_active; // 0x10
    int __dword_14; // 0x14
    int __dword_18; // 0x18
    int8_t __sbyte_1C; // 0x1C
    probably_padding_bytes(0x3); // 0x1D
    LARGE_INTEGER performance_counter_frequency; // 0x20
    LARGE_INTEGER startup_qpc_value; // 0x28
    bool __bool_30; // 0x30
    char appdata_path[0x1000]; // 0x31
    char exe_path[0x1000]; // 0x1031
    probably_padding_bytes(0x3); // 0x2031
    BOOL screen_saver_active; // 0x2034
    BOOL screen_saver_low_power_active; // 0x2038
    BOOL screen_saver_power_off_active; // 0x203C
    union {
        uint32_t flags; // 0x2040
        struct {
            uint32_t __unknown_flag_A : 1; // 1
            uint32_t __unknown_flag_B : 1; // 2
            uint32_t __unknown_bitfield_A : 5; // 3-7
            uint32_t __unknown_flag_C : 1; // 8
            uint32_t __unknown_bitfield_B : 2; // 9-10
        };
    };
    uint32_t __counter_2044; // 0x2044
    unknown_fields(0x8); // 0x2048
    int32_t __int_2050; // 0x2050 scaled_width
    int32_t __int_2054; // 0x2054 scaled_height
    int32_t window_width; // 0x2058
    int32_t window_height; // 0x205C
    int __dword_2060; // 0x2060 display_width
    int __dword_2064; // 0x2064 display_height
    int32_t __int_2068; // 0x2068 backbuffer_width
    int32_t __int_206C; // 0x206C backbuffer_height
    float __float_2070; // 0x2070 game_scale
    int32_t __int_2074; // 0x2074
    int32_t __int_2078; // 0x2078
    int32_t __int_207C; // 0x207C
    int32_t __int_2080; // 0x2080
    int32_t __int_2084; // 0x2084
    int32_t __int_2088; // 0x2088
    int32_t __int_208C; // 0x208C
    int32_t __int_2090; // 0x2090
    probably_padding_bytes(0x4); // 0x2094
    double __double_2098; // 0x2098
    double __double_20A0; // 0x20A0
    double __double_20A8; // 0x20A8
    double __double_20B0; // 0x20B0
    double __double_20B8; // 0x20B8
    double __double_20C0; // 0x20C0
    unknown_fields(0x4); // 0x20C8
    int __dword_20CC; // 0x20CC
    int __int_20D0; // 0x20D0
    int __dword_array_20D4[12]; // 0x20D4 (This might be an array of 4 Int3...?)
    probably_padding_bytes(0x4); // 0x2104
    // 0x2108

    // 0x4726A0
    dllexport gnu_noinline ZUNResult __save_properties_and_configure_paths() asm_symbol_rel(0x4726A0);

    template <typename L>
    inline int32_t update_window_common(const L& lambda);
    
    // 0x472DD0
    dllexport gnu_noinline int32_t thiscall update_window__normal_version() asm_symbol_rel(0x472DD0);

    // 0x472FD0
    dllexport gnu_noinline int32_t thiscall update_window__alt_version() asm_symbol_rel(0x472FD0);

    inline int32_t thiscall update_window__alt_version2();
    
    // 0x4734E0
    dllexport gnu_noinline void __sub_4734E0(int arg1) asm_symbol_rel(0x4734E0);
    
    // 0x473890
    dllexport gnu_noinline BOOL __create_window(HINSTANCE instance) asm_symbol_rel(0x473890);

    // 0x472B50
    dllexport gnu_noinline void thiscall __sub_472B50() asm_symbol_rel(0x472B50);

    // 0x4731B0
    dllexport gnu_noinline void thiscall __sub_4731B0() asm_symbol_rel(0x4731B0);
};
#pragma region // WindowData Verification
ValidateFieldOffset32(0x0, WindowData, window);
ValidateFieldOffset32(0x4, WindowData, resolution_dialogue);
ValidateFieldOffset32(0x8, WindowData, __dword_8);
ValidateFieldOffset32(0xC, WindowData, current_instance);
ValidateFieldOffset32(0x10, WindowData, window_active);
ValidateFieldOffset32(0x14, WindowData, __dword_14);
ValidateFieldOffset32(0x18, WindowData, __dword_18);
ValidateFieldOffset32(0x1C, WindowData, __sbyte_1C);
ValidateFieldOffset32(0x20, WindowData, performance_counter_frequency);
ValidateFieldOffset32(0x28, WindowData, startup_qpc_value);
ValidateFieldOffset32(0x30, WindowData, __bool_30);
ValidateFieldOffset32(0x31, WindowData, appdata_path);
ValidateFieldOffset32(0x1031, WindowData, exe_path);
ValidateFieldOffset32(0x2034, WindowData, screen_saver_active);
ValidateFieldOffset32(0x2038, WindowData, screen_saver_low_power_active);
ValidateFieldOffset32(0x203C, WindowData, screen_saver_power_off_active);
ValidateFieldOffset32(0x2040, WindowData, flags);
ValidateFieldOffset32(0x2044, WindowData, __counter_2044);

ValidateFieldOffset32(0x2050, WindowData, __int_2050);
ValidateFieldOffset32(0x2054, WindowData, __int_2054);
ValidateFieldOffset32(0x2058, WindowData, window_width);
ValidateFieldOffset32(0x205C, WindowData, window_height);
ValidateFieldOffset32(0x2060, WindowData, __dword_2060);
ValidateFieldOffset32(0x2064, WindowData, __dword_2064);
ValidateFieldOffset32(0x2068, WindowData, __int_2068);
ValidateFieldOffset32(0x206C, WindowData, __int_206C);
ValidateFieldOffset32(0x2070, WindowData, __float_2070);
ValidateFieldOffset32(0x2074, WindowData, __int_2074);
ValidateFieldOffset32(0x2078, WindowData, __int_2078);
ValidateFieldOffset32(0x207C, WindowData, __int_207C);
ValidateFieldOffset32(0x2080, WindowData, __int_2080);
ValidateFieldOffset32(0x2084, WindowData, __int_2084);
ValidateFieldOffset32(0x2088, WindowData, __int_2088);
ValidateFieldOffset32(0x208C, WindowData, __int_208C);
ValidateFieldOffset32(0x2090, WindowData, __int_2090);
ValidateFieldOffset32(0x2098, WindowData, __double_2098);
ValidateFieldOffset32(0x20A0, WindowData, __double_20A0);
ValidateFieldOffset32(0x20A8, WindowData, __double_20A8);
ValidateFieldOffset32(0x20B0, WindowData, __double_20B0);
ValidateFieldOffset32(0x20B8, WindowData, __double_20B8);
ValidateFieldOffset32(0x20C0, WindowData, __double_20C0);

ValidateFieldOffset32(0x20CC, WindowData, __dword_20CC);
ValidateFieldOffset32(0x20D0, WindowData, __int_20D0);
ValidateFieldOffset32(0x20D4, WindowData, __dword_array_20D4);
#pragma endregion

extern "C" {
    // 0x568C30
    extern WindowData WINDOW_DATA asm("_WINDOW_DATA");
}

// Yes, I know I don't need explicit enum values. But it makes a quick lookup table visually.
enum CardId : int32_t {
    BLANK_CARD = 0,
    EXTEND_CARD = 1,
    BOMB_CARD = 2,
    EXTEND2_CARD = 3,
    BOMB2_CARD = 4,
    PENDULUM_CARD = 5,
    DANGO_CARD = 6,
    MOKOU_CARD = 7,
    REIMU_OP_CARD = 8,
    REIMU_OP2_CARD = 9,
    MARISA_OP_CARD = 10,
    MARISA_OP2_CARD = 11,
    SAKUYA_OP_CARD = 12,
    SAKUYA_OP2_CARD = 13,
    SANAE_OP_CARD = 14,
    SANAE_OP2_CARD = 15,
    YOUMU_OP_CARD = 16,
    ALICE_OP_CARD = 17,
    CIRNO_OP_CARD = 18,
    OKINA_OP_CARD = 19,
    NUE_OP_CARD = 20,
    ITEM_CATCH_CARD = 21,
    ITEM_LINE_CARD = 22,
    AUTOBOMB_CARD = 23,
    DBOMBEXTEND_CARD = 24,
    MAINSHOT_PU_CARD = 25,
    MAGICSCROLL_CARD = 26,
    KOISHI_CARD = 27,
    MAINSHOT_SP_CARD = 28,
    SPEEDQUEEN_CARD = 29,
    OPTION_BR_CARD = 30,
    DEAD_SPELL_CARD = 31,
    POWERMAX_CARD = 32,
    YUYUKO_CARD = 33,
    MONEY_CARD = 34,
    ROKUMON_CARD = 35,
    NARUMI_CARD = 36,
    PACHE_CARD = 37,
    MANEKI_CARD = 38,
    YAMAWARO_CARD = 39,
    KISERU_CARD = 40,
    WARP_CARD = 41,
    KOZUCHI_CARD = 42,
    KANAME_CARD = 43,
    MOON_CARD = 44,
    MIKOFLASH_CARD = 45,
    VAMPIRE_CARD = 46,
    SUN_CARD = 47,
    LILY_CARD = 48,
    BASSDRUM_CARD = 49,
    PSYCHO_CARD = 50,
    MAGATAMA_CARD = 51,
    CYLINDER_CARD = 52,
    RICEBALL_CARD = 53,
    MUKADE_CARD = 54,
    MAGATAMA2_CARD = 55,
    NULL_CARD = 56,
    BACK_CARD = 57,

    ENUM_VALUE_COUNT_DECLARE(CardId)
};

static inline constexpr size_t INTERNAL_CARD_COUNT = ENUM_VALUE_COUNT(CardId);
static inline constexpr size_t CARD_COUNT = INTERNAL_CARD_COUNT - 1;

// size: 0x34
struct CardData {
    const char* name; // 0x0
    CardId id; // 0x4
    unknown_fields(0x10); // 0x8
    int __int_18; // 0x18
    unknown_fields(0x4); // 0x1C
    uint8_t __byte_20; // 0x20
    unknown_fields(0x7); // 0x21
    BOOL __render_passive_in_hud; // 0x28
    int32_t sprite_large; // 0x2C
    int32_t sprite_small; // 0x30
    // 0x34

    // 0x416E10
    dllexport gnu_noinline int thiscall __sub_416E10() asm_symbol_rel(0x416E10);
};
#pragma region // CardData Validation
ValidateFieldOffset32(0x0, CardData, name);
ValidateFieldOffset32(0x4, CardData, id);
ValidateFieldOffset32(0x18, CardData, __int_18);
ValidateFieldOffset32(0x20, CardData, __byte_20);
ValidateFieldOffset32(0x28, CardData, __render_passive_in_hud);
ValidateFieldOffset32(0x2C, CardData, sprite_large);
ValidateFieldOffset32(0x30, CardData, sprite_small);
ValidateStructSize32(0x34, CardData);
#pragma endregion

extern "C" {
    // 0x4C53C0
    extern CardData CARD_DATA_TABLE[INTERNAL_CARD_COUNT] asm("_CARD_DATA_TABLE");
}

template <typename L>
static inline constexpr const CardData& find_in_card_data(const L& lambda) {
    for (int32_t i = 0; i < countof(CARD_DATA_TABLE); ++i) {
        if (lambda(CARD_DATA_TABLE[i])) {
            return CARD_DATA_TABLE[i];
        }
    }
    return CARD_DATA_TABLE[NULL_CARD];
}

// 0x407D70
dllexport const CardData& find_id_in_card_data(int32_t id) {
    return find_in_card_data([=](const CardData& card) {
        return card.id == id;
    });
}

static inline constexpr uint32_t SCOREFILE_MAGIC = PackUInt32('T', 'H', '8', '1'); // Yup, it's backwards
static inline constexpr uint16_t SCOREFILE_VERSION_NUMBER = 6;

static inline constexpr int32_t SPELL_COUNT = 97;

// 0x4B3F30
static const int8_t SPELL_DIFFICULTY_TABLE[SPELL_COUNT] = {
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EASY, NORMAL, HARD, LUNATIC,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA,
    EXTRA
};

// size: 0x18
struct ScorefileHeader {
    ZUNMagic magic; // 0x0
    uint32_t file_size; // 0x4
    uint16_t version_number; // 0x8
    short __short_A; // 0xA
    int __int_C; // 0xC
    int32_t compressed_size; // 0x10
    int32_t decompressed_size; // 0x14
    // 0x18

    inline void zero_contents() {
        zero_this();
    }
};
#pragma region // ScorefileHeader Validation
ValidateFieldOffset32(0x0, ScorefileHeader, magic);
ValidateFieldOffset32(0x4, ScorefileHeader, file_size);
ValidateFieldOffset32(0x8, ScorefileHeader, version_number);
ValidateFieldOffset32(0xC, ScorefileHeader, __int_C);
ValidateFieldOffset32(0x10, ScorefileHeader, compressed_size);
ValidateFieldOffset32(0x14, ScorefileHeader, decompressed_size);
ValidateStructSize32(0x18, ScorefileHeader);
#pragma endregion

// size: 0x20
struct ScorefileRecord {
    uint32_t score; // 0x0
    uint8_t __byte_4; // 0x4
    int8_t continues; // 0x5
    char name[10]; // 0x6
    time_t time; // 0x10
    float slowdown_rate; // 0x18
    unknown_fields(0x4); // 0x1C
    // 0x20
};
#pragma region // ScorefileRecord Validation
ValidateFieldOffset32(0x0, ScorefileRecord, score);
ValidateFieldOffset32(0x4, ScorefileRecord, __byte_4);
ValidateFieldOffset32(0x5, ScorefileRecord, continues);
ValidateFieldOffset32(0x6, ScorefileRecord, name);
ValidateFieldOffset32(0x10, ScorefileRecord, time);
ValidateFieldOffset32(0x18, ScorefileRecord, slowdown_rate);
ValidateStructSize32(0x20, ScorefileRecord);
#pragma endregion

// size: 0xDC
struct ScorefileSpellcard {
    char name[0]; // 0x0, 0x8D0, 0x8D8
    unknown_fields(0xC0); // 0x0
    int32_t captures[2]; // 0xC0, 0x990, 0x998 0 = Spell record menu, Spell practice menu GAME MODE line, 1 = Spell practice menu
    int32_t attempts[2]; // 0xC8, 0x998, 0x9A0 0 = Spell record menu, Spell practice menu GAME MODE line, 1 = Spell practice menu
    int32_t id; // 0xD0, 0x9A0, 0x9A8
    int32_t difficulty; // 0xD0, 0x9A0, 0x9A8
    int32_t spell_practice_score; // 0xD0, 0x9A8, 0x9B0
    // 0xDC

    //inline void 
};
#pragma region // ScorefileSpellcard
ValidateFieldOffset32(0x0, ScorefileSpellcard, name);
ValidateStructSize32(0xDC, ScorefileSpellcard);
#pragma endregion

// size: 0x8
struct ScorefileStagePractice {
    uint32_t score; // 0x0, 0x12EF0, 0x12EF8
    bool __bool_4; // 0x4, 0x12EF4, 0x12EFC
    bool unlocked; // 0x5, 0x12EF5, 0x12EFD
    unknown_fields(0x2); // 0x6, 0x12EF6, 0x12EFE
    // 0x8, 0x12EF8, 0x12F00
};
#pragma region // ScorefileStagePractice Validation
ValidateFieldOffset32(0x0, ScorefileStagePractice, score);
ValidateFieldOffset32(0x4, ScorefileStagePractice, __bool_4);
ValidateFieldOffset32(0x5, ScorefileStagePractice, unlocked);
ValidateStructSize32(0x8, ScorefileStagePractice);
#pragma endregion

// size: 0xC
struct ScorefileSectionHeader {
    ZUNMagic16 magic; // 0x0
    uint16_t __version_number; // 0x2
    uint32_t checksum; // 0x4
    uint32_t size; // 0x8
    // 0xC
};
#pragma region // ScorefileSectionHeader Validation
ValidateFieldOffset32(0x0, ScorefileSectionHeader, magic);
ValidateFieldOffset32(0x2, ScorefileSectionHeader, __version_number);
ValidateFieldOffset32(0x4, ScorefileSectionHeader, checksum);
ValidateFieldOffset32(0x8, ScorefileSectionHeader, size);
ValidateStructSize32(0xC, ScorefileSectionHeader);
#pragma endregion


struct ScorefileSection : ScorefileSectionHeader {
    uint8_t data[]; // 0xC

    // 0x463250
    dllexport gnu_noinline uint32_t thiscall calculate_checksum(int32_t data_size) asm_symbol_rel(0x463250) {
        const uint8_t* read = this->data;
        data_size -= sizeof(ScorefileSectionHeader);
        
        // This really just looks like MSVC trying to optimize a normal loop
        uint32_t sum = 0;
        for (int32_t i = 0; i < data_size; ++i) {
            sum += read[i];
        }
        return sum;
    }
};

static inline constexpr uint16_t SCOREFILE_SECTION_B_MAGIC = PackUInt16('C', 'R');
static inline constexpr uint16_t SCOREFILE_SECTION_B_VERSION_NUMBER = 3;
static inline constexpr int32_t RECORDS_PER_DIFFICULTY = 10;

/*
uint32_t score; //
bool __idk; // 0x12EF4, 0x12EFC
bool __seen; // 0x12EF5, 0x12EFD
// 0x12EFE
// 0x12F00
bool __idkB; // 0x12F05
// 0x12F06
*/

static inline constexpr size_t wekjbkrb = sizeof(ScorefileStagePractice[DIFFICULTY_COUNT][STAGE_COUNT + 1]);

// size: 0x130F0
struct ScorefileSectionB : ScorefileSectionHeader {
    // ScorefileSectionHeader base; // 0x0, 0x8
    int32_t index; // 0xC, 0x14
    ScorefileRecord records[DIFFICULTY_COUNT][RECORDS_PER_DIFFICULTY]; // 0x10, 0x18
    unknown_fields(0x140); // 0x790, 0x798
    ScorefileSpellcard spells[SPELL_COUNT]; // 0x8D0, 0x8D8
    unknown_fields(0x898); // 0x5C2C, 0x5C34
    int32_t __int_64C4; // 0x64C4, 0x64CC
    uint64_t __ulonglong_64C8; // 0x64C8, 0x64D0
    int32_t __total_clears[DIFFICULTY_COUNT]; // 0x64D0, 0x64D8
    unknown_fields(0x4); // 0x64E8, 0x64F0
    int32_t __1cc_clears[DIFFICULTY_COUNT]; // 0x64EC, 0x64F4
    unknown_fields(0xC9EC); // 0x6504, 0x650C
    ScorefileStagePractice practice[DIFFICULTY_COUNT][STAGE_COUNT + 1]; // 0x12EF0, 0x12EF8
    unknown_fields(0x50); // 0x130A0, 0x130A8
    // 0x130F0, 0x130F8

    inline void zero_contents() {
        zero_this();
    }

    inline ScorefileSectionHeader* next_section() {
        return this + 1;
    }

    inline uint32_t calculate_checksum() {
        return ((ScorefileSection*)this)->calculate_checksum(sizeof(ScorefileSectionB));
    }

    inline void __add_play_count() {
        int32_t count = this->__int_64C4;
        if (count < 9999999) {
            this->__int_64C4 = count + 1;
        }
    }

    // 0x463350
    dllexport gnu_noinline void thiscall initialize() asm_symbol_rel(0x463350) {
        this->zero_contents();
        this->magic.as_uint = SCOREFILE_SECTION_B_MAGIC;
        this->__version_number = SCOREFILE_SECTION_B_VERSION_NUMBER;
        this->size = sizeof(ScorefileSectionB);

        for (size_t i = 0; i < DIFFICULTY_COUNT; ++i) {
            for (size_t j = 0; j < RECORDS_PER_DIFFICULTY; ++j) {
                this->records[i][j].score = 100000 - j * 10000;
                this->records[i][j].__byte_4 = 1;
                memcpy(this->records[i][j].name, "--------", sizeof("--------"));
                this->records[i][j].time = 0;
                this->records[i][j].continues = 0;
                this->records[i][j].slowdown_rate = 0.0f;
            }
        }

        for (int32_t i = 0; i < SPELL_COUNT; ++i) {
            int32_t difficulty = SPELL_DIFFICULTY_TABLE[i];
            this->spells[i].id = i;
            this->spells[i].difficulty = difficulty;
        }
    }

    // 0x457870
    dllexport gnu_noinline int32_t thiscall __sub_457870() asm_symbol_rel(0x457870) {
        ScorefileRecord* cur_record = &this->records[GAME_MANAGER.globals.difficulty][0];
        int32_t cur_score = GAME_MANAGER.globals.score;

        int32_t found_index;
        for (found_index = 0; found_index < RECORDS_PER_DIFFICULTY; ++found_index) {
            if (cur_score >= cur_record->score) {
                goto found_record;
            }
            ++cur_record;
        }
        return -1;
    found_record:
        for (size_t i = RECORDS_PER_DIFFICULTY - 1; found_index < i; --i) {
            this->records[GAME_MANAGER.globals.difficulty][i] = this->records[GAME_MANAGER.globals.difficulty][i - 1];
        }

        cur_record->score = GAME_MANAGER.globals.score;
        cur_record->continues = GAME_MANAGER.globals.continues;
        cur_record->__byte_4 = GAME_MANAGER.globals.current_stage;
        time(&cur_record->time);
        cur_record->slowdown_rate = FPS_COUNTER_PTR->calc_slowdown_rate();

        return found_index;
    }
};
#pragma region // ScorefileSectionB Validation
ValidateFieldOffset32(0x0, ScorefileSectionB, magic);
ValidateFieldOffset32(0x2, ScorefileSectionB, __version_number);
ValidateFieldOffset32(0x4, ScorefileSectionB, checksum);
ValidateFieldOffset32(0x8, ScorefileSectionB, size);
ValidateFieldOffset32(0xC, ScorefileSectionB, index);
ValidateFieldOffset32(0x10, ScorefileSectionB, records);
ValidateFieldOffset32(0x8D0, ScorefileSectionB, spells);
ValidateFieldOffset32(0x64C4, ScorefileSectionB, __int_64C4);
ValidateFieldOffset32(0x64C8, ScorefileSectionB, __ulonglong_64C8);
ValidateFieldOffset32(0x64D0, ScorefileSectionB, __total_clears);
ValidateFieldOffset32(0x64EC, ScorefileSectionB, __1cc_clears);
ValidateFieldOffset32(0x12EF0, ScorefileSectionB, practice);
ValidateStructSize32(0x130F0, ScorefileSectionB);
#pragma endregion

static inline constexpr uint16_t SCOREFILE_SECTION_A_MAGIC = PackUInt16('S', 'T');
static inline constexpr uint16_t SCOREFILE_SECTION_A_VERSION_NUMBER = 6;

// size: 0x3D0
struct ScorefileSectionA : ScorefileSectionHeader {
    // ScorefileSectionHeader base; // 0x0, 0x5F4B8
    char __text_buffer_C[10]; // 0xC, 0x5F4C4
    unknown_fields(0x32); // 0x16, 0x5F4CE
    uint64_t __ulonglong_48; // 0x48, 0x5F500
    bool trophies[30]; // 0x50, 0x5F508
    unknown_fields(0x62); // 0x6E, 0x5F526
    uint8_t __byte_array_D0[CARD_COUNT]; // 0xD0, 0x5F588
    unknown_fields(0x47); // 0x109, 0x5F5C1
    uint8_t __card_ids_150[SHOTTYPE_COUNT][16]; // 0x150, 0x5F608
    unknown_fields(0x30); // 0x190, 0x5F648
    int32_t __int_array_1C0[4]; // 0x1C0, 0x5F678
    short __short_array_1D0[0x100]; // 0x1D0, 0x5F688
    // 0x3D0, 0x5F888

    inline void zero_contents() {
        zero_this();
    }

    inline ScorefileSectionHeader* next_section() {
        return this + 1;
    }

    inline uint32_t calculate_checksum() {
        return ((ScorefileSection*)this)->calculate_checksum(sizeof(ScorefileSectionA));
    }

    // 0x463670
    dllexport gnu_noinline void thiscall initialize() asm_symbol_rel(0x463670) {
        this->zero_contents();
        this->magic.as_uint = SCOREFILE_SECTION_A_MAGIC;
        this->__version_number = SCOREFILE_SECTION_A_VERSION_NUMBER;
        this->size = sizeof(ScorefileSectionA);

        memcpy(this->__text_buffer_C, "        ", sizeof("        "));
        for (int32_t i = 0; i < CARD_COUNT; ++i) {
            this->__byte_array_D0[i] = find_id_in_card_data(i).__byte_20;
        }
        memset(this->__card_ids_150, NULL_CARD, sizeof(__card_ids_150));
        this->__card_ids_150[0][0] = KOZUCHI_CARD;
        this->__int_array_1C0[0] = 1;
        this->__int_array_1C0[1] = 3;
        this->__card_ids_150[1][0] = KOZUCHI_CARD;
        this->__card_ids_150[1][1] = YOUMU_OP_CARD;
        this->__card_ids_150[1][2] = DBOMBEXTEND_CARD;
        this->__int_array_1C0[2] = 5;
        this->__card_ids_150[2][0] = KOZUCHI_CARD;
        this->__int_array_1C0[3] = 0;
        for (size_t i = 0; i < countof(this->__short_array_1D0); ++i) {
            this->__short_array_1D0[i] = REPLAY_RNG.rand_ushort();
        }
    }
};
#pragma region // ScorefileInnerA Validation
ValidateFieldOffset32(0x0, ScorefileSectionA, magic);
ValidateFieldOffset32(0x2, ScorefileSectionA, __version_number);
ValidateFieldOffset32(0x4, ScorefileSectionA, checksum);
ValidateFieldOffset32(0x8, ScorefileSectionA, size);
ValidateFieldOffset32(0xC, ScorefileSectionA, __text_buffer_C);
ValidateFieldOffset32(0x48, ScorefileSectionA, __ulonglong_48);
ValidateFieldOffset32(0x50, ScorefileSectionA, trophies);
ValidateFieldOffset32(0xD0, ScorefileSectionA, __byte_array_D0);
ValidateFieldOffset32(0x150, ScorefileSectionA, __card_ids_150);
ValidateFieldOffset32(0x1C0, ScorefileSectionA, __int_array_1C0);
ValidateFieldOffset32(0x1D0, ScorefileSectionA, __short_array_1D0);
ValidateStructSize32(0x3D0, ScorefileSectionA);
#pragma endregion

struct ScorefileBuffer {
    ScorefileHeader header; // 0x0
    uint8_t file_body[]; // 0x18
};

// size: 0x5F888
struct Scorefile {
    ScorefileBuffer* buffer; // 0x0
    void* decompressed_buffer; // 0x4
    ScorefileSectionB shottypes[SHOTTYPE_COUNT + 1]; // 0x8, 0x130F8, 0x251E8, 0x392D8, 0x4C3C8
    ScorefileSectionA __sectionA; // 0x5F4B8
    // 0x5F888

private:
    // 0x463B30
    dllexport gnu_noinline ZUNResult stdcall save_to_file(const char* filename, Scorefile* scorefile) {
        if (!scorefile->buffer) {
            return ZUN_ERROR;
        }

        // bad ZUN, don't do BS like this!
        uint8_t* big_buffer = (uint8_t*)malloc(0x200000);
        uint32_t written_size = 0;

        *(ScorefileHeader*)big_buffer = scorefile->buffer->header;
        written_size += sizeof(ScorefileHeader);
        
        ScorefileSectionB* sectionB = scorefile->shottypes;
        for (size_t i = 0; i < countof(scorefile->shottypes); ++i) {
            if (sectionB->magic.as_uint == SCOREFILE_SECTION_B_MAGIC) {
                sectionB->index = i;
                clang_forceinline sectionB->checksum = sectionB->calculate_checksum();
                *(ScorefileSectionB*)&big_buffer[written_size] = *sectionB;
                written_size += sizeof(ScorefileSectionB);
            }
            ++sectionB;
        }

        ScorefileSectionA* sectionA = &scorefile->__sectionA;
        sectionA->checksum = sectionA->calculate_checksum();
        *(ScorefileSectionA*)&big_buffer[written_size] = *sectionA;
        written_size += sizeof(ScorefileSectionA) - sizeof(ScorefileHeader); // WTF ZUN

        scorefile->buffer->header.decompressed_size = written_size;
        void* compressed_buffer = __compress_buffer(&big_buffer[sizeof(ScorefileHeader)], scorefile->buffer->header.decompressed_size, &scorefile->buffer->header.compressed_size);
        scorefile->buffer->header.file_size = scorefile->buffer->header.compressed_size + sizeof(ScorefileHeader);
        __crypt_buffer(compressed_buffer, scorefile->buffer->header.compressed_size, 0xAC, 0x35, 0x10, scorefile->buffer->header.compressed_size);
        
        chdir(WINDOW_DATA.appdata_path);

        if (ZUN_FAILED(__zun_open_new_file(filename))) {
            LOG_BUFFER.write_error(JpEnStr("", "error : cannot write score file\n"));
            SAFE_FREE(compressed_buffer);
            free(big_buffer);
            chdir(WINDOW_DATA.exe_path);
            return ZUN_ERROR;
        }

        __zun_write_file(&scorefile->buffer->header, sizeof(ScorefileHeader));
        __zun_write_file(compressed_buffer, scorefile->buffer->header.compressed_size);
        __zun_close_file();

        SAFE_FREE(compressed_buffer);
        chdir(WINDOW_DATA.exe_path);
        free(big_buffer);
        return ZUN_SUCCESS;
    }

    // 0x4639B0
    dllexport gnu_noinline ZUNResult stdcall load_from_buffer(Scorefile* scorefile) {
        if (scorefile->buffer) {
            if (
                scorefile->buffer->header.magic.as_uint == SCOREFILE_MAGIC &&
                scorefile->buffer->header.version_number == SCOREFILE_VERSION_NUMBER
            ) {
                __decrypt_buffer(scorefile->buffer->file_body, scorefile->buffer->header.compressed_size, 0xAC, 0x35, 0x10, scorefile->buffer->header.compressed_size);
                scorefile->decompressed_buffer = malloc(scorefile->buffer->header.decompressed_size * 4);
                __decompress_buffer(scorefile->buffer->file_body, scorefile->buffer->header.compressed_size, scorefile->decompressed_buffer, scorefile->buffer->header.decompressed_size);

                ScorefileSectionHeader* section = (ScorefileSectionHeader*)scorefile->decompressed_buffer;
                int32_t remaining_size = scorefile->buffer->header.decompressed_size;
                while (remaining_size > 0) {
                    switch (section->magic.as_uint) {
                        case SCOREFILE_SECTION_B_MAGIC: {
                            ScorefileSectionB* sectionB = (ScorefileSectionB*)section;
                            if (
                                sectionB->__version_number == SCOREFILE_SECTION_B_VERSION_NUMBER &&
                                sectionB->checksum == sectionB->calculate_checksum() &&
                                sectionB->size == sizeof(ScorefileSectionB)
                            ) {
                                scorefile->shottypes[sectionB->index] = *sectionB;
                                remaining_size -= sectionB->size;
                                if (remaining_size < 0) break;
                                section = sectionB->next_section();
                            }
                            break;
                        }
                        case SCOREFILE_SECTION_A_MAGIC: {
                            ScorefileSectionA* sectionA = (ScorefileSectionA*)section;
                            if (
                                sectionA->__version_number == SCOREFILE_SECTION_A_VERSION_NUMBER &&
                                sectionA->checksum == sectionA->calculate_checksum() &&
                                sectionA->size == sizeof(ScorefileSectionA)
                            ) {

                            }
                            break;
                        }
                    }
                }
                return ZUN_SUCCESS;
            }
            SAFE_FREE(scorefile->buffer);
        }
        scorefile->buffer = (ScorefileBuffer*)malloc(sizeof(ScorefileBuffer));
        scorefile->buffer->header.zero_contents();
        scorefile->buffer->header.magic.as_uint = SCOREFILE_MAGIC;
        scorefile->buffer->header.version_number = SCOREFILE_VERSION_NUMBER;
        scorefile->buffer->header.__int_C = 256;
        return ZUN_SUCCESS;
    }

public:
    inline ZUNResult save_to_file(const char* filename) {
        return Scorefile::save_to_file(filename, this);
    }

    inline ZUNResult load_from_buffer() {
        return Scorefile::load_from_buffer(this);
    }

    inline void initialize(const char* filename) {
        chdir(WINDOW_DATA.appdata_path);
        int32_t bak_file_size;
        this->buffer = (ScorefileBuffer*)read_file_to_buffer(filename, &bak_file_size, true);
        chdir(WINDOW_DATA.exe_path);
        this->__sectionA.initialize();

        for (
            ScorefileSectionB* sectionB = this->shottypes;
            (int32_t)(sectionB - this->shottypes) < SHOTTYPE_COUNT + 1;
            ++sectionB
        ) {
            sectionB->initialize();
        }
    }
};
#pragma region // Scorefile Validation
ValidateFieldOffset32(0x0, Scorefile, buffer);
ValidateFieldOffset32(0x4, Scorefile, decompressed_buffer);
ValidateFieldOffset32(0x8, Scorefile, shottypes);
ValidateFieldOffset32(0x5F4B8, Scorefile, __sectionA);
ValidateStructSize32(0x5F888, Scorefile);
#pragma endregion

typedef struct ScorefileManager ScorefileManager;

extern "C" {
    // 0x4CF41C
    extern ScorefileManager* SCOREFILE_MANAGER_PTR asm("_SCOREFILE_MANAGER_PTR");
}

// size: 0xBF158
struct ScorefileManager {
    Scorefile primary_file; // 0x0
    Scorefile backup_file; // 0x5F888
    unknown_fields(0x48); // 0xBF110
    // 0xBF158

    inline void zero_contents() {
        zero_this();
    }

    ScorefileManager() {
        this->zero_contents();
        this->initialize();
    }

    inline void copy_backup_to_primary() {
        memcpy(this->primary_file.shottypes, this->backup_file.shottypes, sizeof(this->backup_file.shottypes));
        this->primary_file.__sectionA = this->backup_file.__sectionA;
    }

    inline void copy_primary_to_backup() {
        memcpy(this->backup_file.shottypes, this->primary_file.shottypes, sizeof(this->primary_file.shottypes));
        this->backup_file.__sectionA = this->primary_file.__sectionA;
    }

    inline void initialize() {
        this->backup_file.initialize("scoreth18bak.dat");
        this->backup_file.load_from_buffer();
        this->primary_file.initialize("scoreth18.dat");

        this->copy_backup_to_primary();

        this->primary_file.load_from_buffer();

        this->copy_primary_to_backup();
    }

    // 0x4637D0
    dllexport gnu_noinline static ScorefileManager* allocate() {
        ScorefileManager* scorefile_manager = new ScorefileManager();
        SCOREFILE_MANAGER_PTR = scorefile_manager;
        return scorefile_manager;
    }

    // 0x413510
    dllexport gnu_noinline static ZUNResult save_files() {
        ScorefileManager* scorefile_manager = SCOREFILE_MANAGER_PTR;
        int32_t ret = scorefile_manager->backup_file.save_to_file("scoreth18bak.dat");
        ret |= scorefile_manager->primary_file.save_to_file("scoreth18.dat");
        scorefile_manager->copy_primary_to_backup();
        return (ZUNResult)ret;
    }
};
#pragma region // ScorefileManager Validation
ValidateFieldOffset32(0x0, ScorefileManager, primary_file);
ValidateFieldOffset32(0x5F888, ScorefileManager, backup_file);
ValidateStructSize32(0xBF158, ScorefileManager);
#pragma endregion

// 0x416E10
dllexport gnu_noinline int thiscall CardData::__sub_416E10() {
    switch (this->__int_18) {
        case 0:
            return 1;
        case 1:
            if (GAME_MANAGER.globals.current_stage == 1) {
                return 2;
            }
            break;
        case 2:
            if (GAME_MANAGER.globals.current_stage == 2) {
                return 2;
            }
            break;
        case 3:
            if (GAME_MANAGER.globals.current_stage == 3) {
                return 2;
            }
            break;
        case 4:
            if (GAME_MANAGER.globals.current_stage == 4) {
                return 2;
            }
            break;
        case 5:
            if (GAME_MANAGER.globals.current_stage == 5) {
                return 2;
            }
            break;
        case 6:
            switch (GAME_MANAGER.globals.current_stage) {
                case 1: case 2:
                    return 1;
                default:
                    return !(RNG.rand_uint() % 5);
            }
        case 7:
            switch (GAME_MANAGER.globals.current_stage) {
                case 1: case 2: case 3:
                    return 1;
                default:
                    return !(RNG.rand_uint() % 5);
            }
        case 8:
            switch (GAME_MANAGER.globals.current_stage) {
                case 2: case 3: case 4:
                    return 1;
                default:
                    return !(RNG.rand_uint() % 5);
            }
        case 9:
            switch (GAME_MANAGER.globals.current_stage) {
                case 3: case 4: case 5:
                    return 1;
                default:
                    return !(RNG.rand_uint() % 5);
            }
        case 10:
            switch (GAME_MANAGER.globals.current_stage) {
                case 4: case 5:
                    return 1;
                default:
                    return !(RNG.rand_uint() % 5);
            }
        case 11:
            switch (GAME_MANAGER.globals.current_stage) {
                default:
                    return 0;
                case 1: case 2: case 3: case 4: case 5:
                    break;
            }
        case 12:
            break;
    }
    return SCOREFILE_MANAGER_PTR->primary_file.__sectionA.__byte_array_D0[this->id] != 0;
}

enum MotionMode : int32_t {
    AxisVelocityMovement = 0,
    NoMovement = 1,
    OrbitMovement = 2,
    EllipseMovement = 3,
    UnknownMovement = 4
};

// size: 0x44
struct MotionData {
    Float3 position; // 0x0
    Float3 position2; // 0xC
    float speed; // 0x18
    ZUNAngle angle; // 0x1C
    float orbit_radius; // 0x20
    float orbit_velocity; // 0x24
    ZUNAngle ellipse_angle; // 0x28
    float ellipse_ratio; // 0x2C
    float __angle_30; // 0x30
    union {
        Float3 misc_float3; // 0x34
        Float3 axis_velocity; // 0x34
        Float3 orbit_origin; // 0x34
    };
    union {
        uint32_t flags; // 0x40
        struct {
            uint32_t mode : 4; // 1-4
        };
    };

    inline void zero_contents() {
        zero_this();
    }
    
    // 0x402CC0
    dllexport gnu_noinline void update() asm_symbol_rel(0x402CC0) {
        Float3 float3B; // EBP-20, EBP-1C, EBP-18
        Float3 float3A; // EBP-14, EBP-10, EBP-C
        float floatA; // EBP-8
        float floatB; // EBP-4
        float floatC;
        switch (this->mode) {
            case AxisVelocityMovement:
                this->position += this->axis_velocity;
                break;
            case OrbitMovement:
                float3A.make_from_vector(this->angle, this->orbit_radius);
                this->position = this->orbit_origin + float3A.as2();
                break;
            case EllipseMovement: {
                ZUNAngle angle_temp = this->ellipse_angle;
                float3A.make_from_vector(reduce_angle<NoInline>(this->angle - angle_temp), this->orbit_radius);
                floatB = this->ellipse_ratio * float3A.x;
                floatA = zsinf(angle_temp);
                floatC = zcosf(angle_temp);
                Float2 float2A = { floatB, float3A.y };
                Float2 float2B = float2A;
                float2B *= floatA;
                float2A *= floatC;
                float2A.x -= float2B.y;
                float2A.y += float2B.x;
                this->position = this->orbit_origin + float2A;
                break;
            }
            case UnknownMovement: {
                float3A.as2() = this->position.as2();
                this->position2 += this->axis_velocity;
                floatB = this->ellipse_angle + HALF_PI_f;
                float3B.make_from_vector(reduce_angle<NoInline>(floatB), this->orbit_radius * zsinf(this->__angle_30) * GAME_SPEED.value);
                this->position = this->position2 + float3B.as2();
                float angle_temp = this->position.angle_to(float3A.as2());
                clang_noinline this->angle = angle_temp;
                break;
            }
        }
        this->position.x = zfloorf(this->position.x * 100.0f) / 100.0f;
        this->position.y = zfloorf(this->position.y * 100.0f) / 100.0f;
    }

    // This function doesn't exist, I merely wanted to write a simplified version
    inline void update_readable() {
        switch (this->mode) {
            case AxisVelocityMovement:
                this->position.x += this->axis_velocity.x;
                this->position.y += this->axis_velocity.y;
                this->position.z += this->axis_velocity.z;
                break;
            case OrbitMovement: {
                Float2 circle_pos;
                circle_pos.make_from_vector(this->angle, this->orbit_radius);
                this->position.x = this->orbit_origin.x + circle_pos.x;
                this->position.y = this->orbit_origin.y + circle_pos.y;
                this->position.z = this->orbit_origin.z;
                break;
            }
            case EllipseMovement: {
                Float2 ellipse_pos;
                ellipse_pos.make_from_vector(reduce_angle(this->angle - this->ellipse_angle), this->orbit_radius);
                ellipse_pos.x *= this->ellipse_ratio;
                Float2 temp_pos = ellipse_pos;
                ellipse_pos.x *= cosf(this->ellipse_angle);
                ellipse_pos.y *= cosf(this->ellipse_angle);
                temp_pos.x *= sinf(this->ellipse_angle);
                temp_pos.y *= sinf(this->ellipse_angle);
                ellipse_pos.x -= temp_pos.y;
                ellipse_pos.y += temp_pos.x;
                this->position.x = this->orbit_origin.x + ellipse_pos.x;
                this->position.y = this->orbit_origin.y + ellipse_pos.y;
                this->position.z = this->orbit_origin.z;
                break;
            }
            case UnknownMovement: {
                Float2 prev_position = this->position;
                this->position2.x += this->axis_velocity.x;
                this->position2.y += this->axis_velocity.y;
                this->position2.z += this->axis_velocity.z;
                Float2 offset;
                offset.make_from_vector(reduce_angle(this->ellipse_angle + HALF_PI_f), this->orbit_radius * sinf(this->__angle_30) * GAME_SPEED.value);
                this->position.x = this->position2.x + offset.x;
                this->position.y = this->position2.y + offset.y;
                this->position.z = this->position2.z;
                this->angle = this->position.angle_to(prev_position); // wrapper function for atan2
                break;
            }
        }
        this->position.x = floorf(this->position.x * 100.0f) / 100.0f;
        this->position.y = floorf(this->position.y * 100.0f) / 100.0f;
    }

    // 0x402F90
    dllexport gnu_noinline void update2() asm_symbol_rel(0x402F90) {
        if (this->mode == UnknownMovement) {
            this->position2 = this->position;
        }
        this->update();
    }
    
    // 0x412F60
    dllexport gnu_noinline void set_position(Float3* coord) asm_symbol_rel(0x412F60) {
        this->position = *coord;
    }
    // 0x43A290
    dllexport gnu_noinline void set_positionB(Float3* coord) asm_symbol_rel(0x43A290) {
        this->position = *coord;
    }
    
    // 0x412FD0
    dllexport gnu_noinline void vectorcall set_speed(float value) asm_symbol_rel(0x412FD0) {
        this->speed = value;
    }
    
    // 0x413280
    dllexport gnu_noinline Float3& thiscall get_position() asm_symbol_rel(0x413280) {
        return this->position;
    }
    
    // 0x422530
    dllexport gnu_noinline void vectorcall set_orbit_velocity(float value) asm_symbol_rel(0x422530) {
        this->orbit_velocity = value;
    }
    
    // 0x422540
    dllexport gnu_noinline void vectorcall set_orbit_radius(float value) asm_symbol_rel(0x422540) {
        this->orbit_radius = value;
    }
    
    // 0x422550
    dllexport gnu_noinline void set_orbit_mode() asm_symbol_rel(0x422550) {
        this->mode = OrbitMovement;
    }
    
    // 0x422560
    dllexport gnu_noinline void set_misc_float3(const Float3& value) asm_symbol_rel(0x422560) {
        this->misc_float3 = value;
    }

    forceinline void set_axis_velocity(const Float3& value) {
        this->set_misc_float3(value);
    }

    forceinline void set_orbit_origin(const Float3& value) {
        this->set_misc_float3(value);
    }

    // 0x422580
    dllexport gnu_noinline float vectorcall get_speed() asm_symbol_rel(0x422580) {
        return this->speed;
    }
    
    // 0x422590
    dllexport gnu_noinline ZUNAngle& get_angle() asm_symbol_rel(0x422590) {
        return this->angle;
    }
    
    // 0x4225A0
    dllexport gnu_noinline void vectorcall set_angle(float angle) asm_symbol_rel(0x4225A0) {
        this->angle = angle;
    }
    
    // 0x4225F0
    dllexport gnu_noinline void set_axis_velocity_mode() asm_symbol_rel(0x4225F0) {
        this->mode = AxisVelocityMovement;
    }
    
    // 0x43A210
    dllexport gnu_noinline void vectorcall set_position_y(float value) asm_symbol_rel(0x43A210) {
        this->position.y = value;
    }
    
    // 0x43A220
    dllexport gnu_noinline void vectorcall set_position_x(float value) asm_symbol_rel(0x43A220) {
        this->position.x = value;
    }
    
    // 0x43A230
    dllexport gnu_noinline float vectorcall get_position_y() asm_symbol_rel(0x43A230) {
        return this->position.y;
    }
    
    // 0x43A240
    dllexport gnu_noinline float vectorcall get_position_x() asm_symbol_rel(0x43A240) {
        return this->position.x;
    }

    // 0x439ED0
    dllexport gnu_noinline float vectorcall get_ellipse_angle() asm_symbol_rel(0x439ED0) {
        return this->ellipse_angle;
    }

    // 0x439EE0
    dllexport gnu_noinline float vectorcall get_ellipse_ratio() asm_symbol_rel(0x439EE0) {
        return this->ellipse_ratio;
    }
    
    // 0x439EF0
    dllexport gnu_noinline void set_ellipse_mode() asm_symbol_rel(0x439EF0) {
        this->mode = EllipseMovement;
    }
    
    // 0x439F00
    dllexport gnu_noinline void vectorcall set_ellipse_ratio(float ratio) asm_symbol_rel(0x439F00) {
        this->ellipse_ratio = ratio;
    }
    
    // 0x439F10
    dllexport gnu_noinline void vectorcall set_ellipse_angle(float angle) asm_symbol_rel(0x439F10) {
        this->ellipse_angle = angle;
    }

    // 0x439FA0
    dllexport gnu_noinline void vectorcall set_misc_float3_y(float value) asm_symbol_rel(0x439FA0) {
        this->misc_float3.y = value;
    }

    forceinline void vectorcall set_axis_velocity_y(float value) {
        this->set_misc_float3_y(value);
    }

    forceinline void vectorcall set_orbit_origin_y(float value) {
        this->set_misc_float3_y(value);
    }

    // 0x439FB0
    dllexport gnu_noinline void vectorcall set_misc_float3_x(float value) asm_symbol_rel(0x439FB0) {
        this->misc_float3.x = value;
    }

    forceinline void vectorcall set_axis_velocity_x(float value) {
        this->set_misc_float3_x(value);
    }

    forceinline void vectorcall set_orbit_origin_x(float value) {
        this->set_misc_float3_x(value);
    }
    
    // 0x439FC0
    dllexport gnu_noinline float vectorcall get_orbit_radius() asm_symbol_rel(0x439FD0) {
        return this->orbit_radius;
    }
    
    // 0x439FD0
    dllexport gnu_noinline float vectorcall get_orbit_velocity() asm_symbol_rel(0x439FD0) {
        return this->orbit_velocity;
    }
    
    // 0x43A270
    dllexport gnu_noinline bool mode_is_orbit() asm_symbol_rel(0x43A270) {
        return this->mode == OrbitMovement;
    }

    inline MotionData() {};
};
#pragma region // MotionData Validation
ValidateFieldOffset32(0x0, MotionData, position);
ValidateFieldOffset32(0xC, MotionData, position2);
ValidateFieldOffset32(0x18, MotionData, speed);
ValidateFieldOffset32(0x1C, MotionData, angle);
ValidateFieldOffset32(0x20, MotionData, orbit_radius);
ValidateFieldOffset32(0x24, MotionData, orbit_velocity);
ValidateFieldOffset32(0x28, MotionData, ellipse_angle);
ValidateFieldOffset32(0x2C, MotionData, ellipse_ratio);
ValidateFieldOffset32(0x30, MotionData, __angle_30);
ValidateFieldOffset32(0x34, MotionData, misc_float3);
ValidateFieldOffset32(0x40, MotionData, flags);
ValidateStructSize32(0x44, MotionData);
#pragma endregion

template <typename T>
struct ZUNAbsRel {
    T absolute;
    T relative;
};
/*
enum InterpMode {
    Linear = 0,
    AccelerateSlow = 1,
    Accelerate = 2,
    AccelerateFast = 3,
    DecelerateSlow = 4,
    Decelerate = 5,
    DecelerateFast = 6,
    ConstantVelocity = 7,
    Bezier = 8,
    AccelDecelSlow = 9,
    AccelDecel = 10,
    AccelDecelFast = 11,
    DecelAccelSlow = 12,
    DecelAccel = 13,
    DecelAccelFast = 14,
};
*/

// 0x402FB0
dllexport gnu_noinline float vectorcall __interp_inner_thing(int32_t mode, float current_time, float end_time) asm_symbol_rel(0x402FB0);
dllexport gnu_noinline float vectorcall __interp_inner_thing(int32_t mode, float current_time, float end_time) {
    float value = current_time;
    if (end_time == 0.0f) {
        goto interp_final;
    }
    value /= end_time;
    switch (mode) {
        case AccelerateSlow: // 1
            value = value * value;
        default: // linear
            return value;
        case DecelerateSlow: // 4
            value = 1.0f - value;
            value = value * value;
            value = 1.0f - value;
            return value;
        case Accelerate: // 2
            value = value * value * value;
            return value;
        case Decelerate: // 5
            value = 1.0f - value;
            value = value * value * value;
            value = 1.0f - value;
            return value;
        case AccelerateFast: // 3
            value = value * value * value * value;
            return value;
        case DecelerateFast: // 6
            value = 1.0f - value;
            value = value * value * value * value;
            value = 1.0f - value;
            return value;
        case AccelDecelSlow: // 9
            value += value;
            if (value < 1.0f) {
                value = value * value;
                return value * 0.5f;
            } else {
                value = 2.0f - value;
                value = value * value;
                value = 2.0f - value;
                return value * 0.5f;
            }
        case DecelAccelSlow: // 12
            value += value;
            if (value < 1.0f) {
                value = 1.0f - value;
                value = value * value;
                return 0.5f - value * 0.5f;
            } else {
                value = value - 1.0f;
                value = value * value;
                return 0.5f + value * 0.5f;
            }
        case AccelDecel: // 10
            value += value;
            if (value < 1.0f) {
                value = value * value * value;
                return value * 0.5f;
            } else {
                value = 2.0f - value;
                value = value * value * value;
                value = 2.0f - value;
                return value * 0.5f;
            }
        case DecelAccel: // 13
            value += value;
            if (value < 1.0f) {
                value = 1.0f - value;
                value = value * value * value;
                return 0.5f - value * 0.5f;
            } else {
                value = value - 1.0f;
                value = value * value * value;
                return 0.5f + value * 0.5f;
            }
        case AccelDecelFast: // 11
            value += value;
            if (value < 1.0f) {
                value = value * value * value * value;
                return value * 0.5f;
            } else {
                value = 2.0f - value;
                value = value * value * value * value;
                value = 2.0f - value;
                return value * 0.5f;
            }
        case DecelAccelFast: // 14
            value += value;
            if (value < 1.0f) {
                value = 1.0f - value;
                value = value * value * value * value;
                return 0.5f - value * 0.5f;
            } else {
                value = value - 1.0f;
                value = value * value * value * value;
                return 0.5f + value * 0.5f;
            }
        case DecelerateSine: // 18
            return zsinf(value * PI_f * 0.5f);
        case AccelerateSine: // 19
            return 1.0f - zsinf(value * PI_f * 0.5f + HALF_PI_f);
        case DecelAccelSine: // 20
            value += value;
            if (1.0f > value) {
                return zsinf(value * PI_f * 0.5f) * 0.5f;
            } else {
                return (1.0f - zsinf(value * PI_f * 0.5f)) * 0.5f + 0.5f;
            }
        case AccelDecelSine: // 21
            value += value;
            if (1.0f > value) {
                return (1.0f - zsinf(value * PI_f * 0.5f + HALF_PI_f)) * 0.5f;
            } else {
                value = value - 1.0f;
                return zsinf(value * PI_f * 0.5f) * 0.5f + 0.5f;
            }
        case AccelParabolaA: // 22
            value = value - 0.25f;
            value = (value * value / (9.0f / 16.0f) - 1.0f / 9.0f) / (8.0f / 9.0f);
            return value;
        case AccelParabolaB: // 23
            value = value - 0.3f;
            value = (value * value / 0.49f - 9.0f / 49.0f) / (40.0f / 49.0f);
            return value;
        case AccelParabolaC: // 24
            value = value - 0.35f;
            value = (value * value / 0.4225f - 49.0f / 169.0f) / (120.0f / 169.0f);
            return value;
        case AccelParabolaD: // 25
            value = value - 0.38f;
            value = (value * value / 0.3844f - 361.0f / 961.0f) / (600.0f / 961.0f);
            return value;
        case AccelParabolaE: // 26
            value = value - 0.4f;
            value = (value * value / 0.36f - 4.0f / 9.0f) / (5.0f / 9.0f);
            return value;
        case DecelParabolaA: // 27
            value = 1.0f - value;
            value = value - 0.25f;
            value = (value * value / (9.0f / 16.0f) - 1.0f / 9.0f) / (8.0f / 9.0f);
            value = 1.0f - value;
            return value;
        case DecelParabolaB: // 28
            value = 1.0f - value;
            value = value - 0.3f;
            value = (value * value / 0.49f - 9.0f / 49.0f) / (40.0f / 49.0f);
            value = 1.0f - value;
            return value;
        case DecelParabolaC: // 29
            value = 1.0f - value;
            value = value - 0.35f;
            value = (value * value / 0.4225f - 49.0f / 169.0f) / (120.0f / 169.0f);
            value = 1.0f - value;
            return value;
        case DecelParabolaD: // 30
            value = 1.0f - value;
            value = value - 0.38f;
            value = (value * value / 0.3844f - 361.0f / 961.0f) / (600.0f / 961.0f);
            value = 1.0f - value;
            return value;
        case DecelParabolaE: // 31
            value = 1.0f - value;
            value = value - 0.4f;
            value = (value * value / 0.36f - 4.0f / 9.0f) / (5.0f / 9.0f);
            value = 1.0f - value;
            return value;
        case ForceFinal: interp_final: // 16
            return 1.0f;
        case ForceInitial: // 15
            return 0.0f;
    }
}

template <typename T>
struct ZUNInterp { //       0x58    0x44    0x30
    //                      T3      T2      T1
    T initial_value; //     0x0     0x0     0x0
    T final_value; //       0xC     0x8     0x4
    T bezier1; //           0x18    0x10    0x8
    T bezier2; //           0x24    0x18    0xC
    T current; //           0x30    0x20    0x10
    Timer time; //          0x3C    0x28    0x14
    int32_t end_time; //    0x50    0x3C    0x28
    int32_t mode; //        0x54    0x40    0x2C

    inline ZUNInterp<T>() : end_time(0) {}

    // float: 0x41F600
    // Float2: 0x439600
    // Float3: 0x405A00
    // Float3Ex: 0x439940
    // ZUNAngle: 0x47CBF0
    dllexport gnu_noinline void set_end_time(int32_t time) {
        this->end_time = time;
    }
    
    // Float3Ex: 0x439950
    dllexport gnu_noinline void set_current(const T& value) {
        this->current = value;
    }
    
    // float: 0x41B790
    // Float2: 0x4395C0
    // Float3Ex: 0x439900
    dllexport gnu_noinline void reset_end_time() {
        this->end_time = 0;
    }

    // float: 0x41F630
    // Float2: 0x439690
    // Float3: 0x405A10
    // ZUNAngle: 0x47CC40
    dllexport gnu_noinline void set_mode(int32_t mode) {
        this->mode = mode;
    }

    // float: 0x41F620
    // Float2: 0x439670
    // Float3: 0x405A20
    // ZUNAngle: 0x47CC30
    dllexport gnu_noinline void set_initial_value(const T& value) {
        this->initial_value = value;
    }

    // float: 0x41F610
    // Float2: 0x439650
    // Float3: 0x405A40
    // Float3Ex: 0x4399B0
    // ZUNAngle: 0x47CC20
    dllexport gnu_noinline void set_final_value(const T& value) {
        this->final_value = value;
    }

    // float: 0x429A70
    // Float3: 0x405A60
    // Float3Ex: 0x439990
    // ZUNAngle: 0x47CC10
    dllexport gnu_noinline void set_bezier1(const T& value) {
        this->bezier1 = value;
    }

    // float: 0x429A60
    // Float3: 0x405A80
    // Float3Ex: 0x439970
    // ZUNAngle: 0x47CC00
    dllexport gnu_noinline void set_bezier2(const T& value) {
        this->bezier2 = value;
    }

    // float: 0x41F5D0
    // Float2: 0x4395D0
    // Float3: 0x405AA0
    // Float3Ex: 0x439910
    // ZUNAngle: 0x47CBC0
    dllexport gnu_noinline void reset_timer() {
        this->time.reset();
    }
    
    // int: 0x47CF40
    // float: 0x41F640
    // Float2: 0x4396A0
    // ZUNAngle: 0x47CC50
    // StageSky: 0x41EFA0
    dllexport T vectorcall step() {
        int32_t end_time = this->end_time;
        if (end_time > 0) {
            this->time.increment();
            end_time = this->end_time;
            if (this->time >= end_time) {
                this->time.set(end_time);
                goto TimeEnd;
            }
        } else if (end_time == 0) {
TimeEnd:
            int32_t mode = this->mode;
            if (mode != ConstantVelocity && mode != ConstantAccel) {
                return this->final_value;
            } else {
                return this->initial_value;
            }
        }
        T initial = this->initial_value;
        int32_t mode = this->mode;
        if (mode == ConstantVelocity) { // 7
            return this->current = this->initial_value = initial + this->final_value;
        }
        if (mode == ConstantAccel) { // 17
            T temp = this->bezier2;
            this->bezier2 = temp + this->final_value;
            return this->current = this->initial_value = initial + temp;
        }
        float current_time = this->time.current_f;
        float end_time_f = end_time;
        if (mode == Bezier) { // 8
            current_time /= end_time_f;
            float XMM3 = 1.0f - current_time;
            float XMM2 = current_time + current_time;
            float XMM5 = current_time - 1.0f;
            XMM3 *= XMM3;
            initial *= XMM5 * XMM5 * (XMM2 + 1.0f);
            initial += (3.0f - XMM2) * this->final_value * current_time * current_time;
            initial += XMM3 * this->bezier1 * current_time;
            initial += XMM5 * this->bezier2 * current_time * current_time;
            return this->current = initial;
        }
        else {
            float interp_value = __interp_inner_thing(mode, current_time, end_time_f);
            return this->current = lerp(initial, this->final_value, interp_value);
        }
    }

    forceinline void initialize(int32_t end_time, int32_t mode, const T& initial_value, const T& final_value) {
        const T zero = {};
        this->set_end_time(end_time);
        this->set_mode(mode);
        this->set_bezier1(zero);
        this->set_bezier2(zero);
        this->set_initial_value(initial_value);
        this->set_final_value(final_value);
        this->reset_timer();
    }

    forceinline void initialize_bezier(int32_t end_time, int32_t mode, const T& initial_value, const T& final_value, const T& bezier1_value, const T& bezier2_value) {
        this->set_end_time(end_time);
        this->set_mode(mode);
        this->set_bezier1(bezier1_value);
        this->set_bezier2(bezier2_value);
        this->set_initial_value(initial_value);
        this->set_final_value(final_value);
        this->reset_timer();
    }
};
#pragma region // ZUNInterp Validation
ValidateFieldOffset32(0x0, ZUNInterp<float>, initial_value);
ValidateFieldOffset32(0x4, ZUNInterp<float>, final_value);
ValidateFieldOffset32(0x8, ZUNInterp<float>, bezier1);
ValidateFieldOffset32(0xC, ZUNInterp<float>, bezier2);
ValidateFieldOffset32(0x10, ZUNInterp<float>, current);
ValidateFieldOffset32(0x14, ZUNInterp<float>, time);
ValidateFieldOffset32(0x28, ZUNInterp<float>, end_time);
ValidateFieldOffset32(0x2C, ZUNInterp<float>, mode);
ValidateStructSize32(0x30, ZUNInterp<float>);
ValidateFieldOffset32(0x0, ZUNInterp<Float2>, initial_value);
ValidateFieldOffset32(0x8, ZUNInterp<Float2>, final_value);
ValidateFieldOffset32(0x10, ZUNInterp<Float2>, bezier1);
ValidateFieldOffset32(0x18, ZUNInterp<Float2>, bezier2);
ValidateFieldOffset32(0x20, ZUNInterp<Float2>, current);
ValidateFieldOffset32(0x28, ZUNInterp<Float2>, time);
ValidateFieldOffset32(0x3C, ZUNInterp<Float2>, end_time);
ValidateFieldOffset32(0x40, ZUNInterp<Float2>, mode);
ValidateStructSize32(0x44, ZUNInterp<Float2>);
ValidateFieldOffset32(0x0, ZUNInterp<Float3>, initial_value);
ValidateFieldOffset32(0xC, ZUNInterp<Float3>, final_value);
ValidateFieldOffset32(0x18, ZUNInterp<Float3>, bezier1);
ValidateFieldOffset32(0x24, ZUNInterp<Float3>, bezier2);
ValidateFieldOffset32(0x30, ZUNInterp<Float3>, current);
ValidateFieldOffset32(0x3C, ZUNInterp<Float3>, time);
ValidateFieldOffset32(0x50, ZUNInterp<Float3>, end_time);
ValidateFieldOffset32(0x54, ZUNInterp<Float3>, mode);
ValidateStructSize32(0x58, ZUNInterp<Float3>);
#pragma endregion

template <typename T, typename E>
struct ZUNInterpExImpl { //                 0x68    0x50
    static inline constexpr size_t AXIS_COUNT = sizeof(T) / sizeof(E);
    T current; //                       0x0     0x0
    T initial_value; //                 0xC     0x8
    T final_value; //                   0x18    0x10
    T bezier1; //                       0x24    0x18
    T bezier2; //                       0x30    0x20
    Timer time; //                      0x3C    0x28
    int32_t end_time; //                0x50    0x3C
    int32_t axis_modes[AXIS_COUNT]; //  0x54    0x40
    int32_t combined_mode; //           0x60    0x48
    union {
        uint32_t flags; //              0x64    0x4C
        struct {
            uint32_t interp_per_axis : 1; // 1
        };
    };

    inline ZUNInterpExImpl<T, E>() : end_time(0) {}

    // Float3: 0x439940
    dllexport gnu_noinline void set_end_time(int32_t time) {
        this->end_time = time;
    }

    // Float3: 0x439900
    dllexport gnu_noinline void reset_end_time() {
        this->end_time = 0;
    }

    // Float3: 0x4399B0
    dllexport gnu_noinline void set_initial_value(const T& value) {
        this->initial_value = value;
    }

    // Float3: 0x439990
    dllexport gnu_noinline void set_final_value(const T& value) {
        this->final_value = value;
    }

    // Float3: 0x439970
    dllexport gnu_noinline void set_bezier1(const T& value) {
        this->bezier1 = value;
    }

    // Float3: 0x439950
    dllexport gnu_noinline void set_bezier2(const T& value) {
        this->bezier2 = value;
    }

    // Float3: 0x439910
    dllexport gnu_noinline void reset_timer() {
        this->time.reset();
    }
    
    // Float3: 0x4399D0
    dllexport gnu_noinline void set_axis_mode(int32_t axis, int32_t mode) {
        this->interp_per_axis = true;
        this->axis_modes[axis] = mode;
    }
    
    // Float3: 0x4399F0
    dllexport gnu_noinline void set_combined_mode(int32_t mode) {
        this->interp_per_axis = false;
        this->combined_mode = mode;
    }

    // Float3: 0x439A10
    dllexport T vectorcall step() {
        int32_t end_time = this->end_time;
        if (end_time > 0) {
            this->time.increment();
            end_time = this->end_time;
            if (this->time >= end_time) {
                this->time.set(end_time);
                goto TimeEnd;
            }
        } else if (end_time == 0) {
TimeEnd:
            int32_t mode = this->combined_mode;
            if (mode != ConstantVelocity && mode != ConstantAccel) {
                return this->final_value;
            } else {
                return this->initial_value;
            }
        }
        if (!this->interp_per_axis) {
            int32_t mode = this->combined_mode;
            if (mode == ConstantVelocity) { // 7
                T initial = this->initial_value;
                return this->current = this->initial_value = initial + this->final_value;
            }
            if (mode == ConstantAccel) { // 17
                T initial = this->initial_value;
                T temp = this->bezier2;
                this->bezier2 = temp + this->final_value;
                return this->current = this->initial_value = initial + temp;
            }
            float current_time = this->time.current_f;
            float end_time_f = end_time;
            if (mode == Bezier) { // 8
                current_time /= end_time_f;
                float XMM3 = 1.0f - current_time;
                float XMM2 = current_time + current_time;
                float XMM5 = current_time - 1.0f;
                XMM3 *= XMM3;
                T initial = this->initial_value;
                initial *= XMM5 * XMM5 * (XMM2 + 1.0f);
                initial += (3.0f - XMM2) * this->final_value * current_time * current_time;
                initial += XMM3 * this->bezier1 * current_time;
                initial += XMM5 * this->bezier2 * current_time * current_time;
                return this->current = initial;
            }
            else {
                float interp_value = __interp_inner_thing(mode, current_time, end_time_f);
                return this->current = lerp(this->initial_value, this->final_value, interp_value);
            }
        }
        else {
            for (size_t i = 0; i < AXIS_COUNT; ++i) {
                E& current = ((E*)&this->current)[i];
                E& initial_value = ((E*)&this->initial_value)[i];
                E& final_value = ((E*)&this->final_value)[i];
                E& bezier1 = ((E*)&this->bezier1)[i];
                E& bezier2 = ((E*)&this->bezier2)[i];

                int32_t mode = this->axis_modes[i];
                if (mode == ConstantVelocity) { // 7
                    current = initial_value = initial_value + final_value;
                }
                else if (mode == ConstantAccel) { // 17
                    E temp = bezier2;
                    bezier2 = temp + final_value;
                    current = initial_value = initial_value + temp;
                }
                else {
                    float end_time_f = end_time;
                    E final_val = final_value;
                    float current_time = this->time.current_f;
                    if (mode == Bezier) { // 8
                        current_time /= end_time_f;
                        float XMM3 = 1.0f - current_time;
                        float XMM2 = current_time + current_time;
                        float XMM5 = current_time - 1.0f;
                        XMM3 *= XMM3;
                        E initial = initial_value;
                        initial *= XMM5 * XMM5 * (XMM2 + 1.0f);
                        initial += (3.0f - XMM2) * final_val * current_time * current_time;
                        initial += XMM3 * bezier1 * current_time;
                        initial += XMM5 * bezier2 * current_time * current_time;
                        current = initial;
                    }
                    else {
                        float interp_value = __interp_inner_thing(mode, current_time, end_time_f);
                        current = lerp(initial_value, final_val, interp_value);
                    }
                }
            }
            return this->current;
        }
    }
};
template <typename T> struct ZUNInterpEx;
template <> struct ZUNInterpEx<Float2> : ZUNInterpExImpl<Float2, float> {};
template <> struct ZUNInterpEx<Float3> : ZUNInterpExImpl<Float3, float> {};
template <> struct ZUNInterpEx<Int2> : ZUNInterpExImpl<Int2, int32_t> {};
template <> struct ZUNInterpEx<Int3> : ZUNInterpExImpl<Int3, int32_t> {};
#pragma region // ZUNInterpEx Validation
ValidateFieldOffset32(0x0, ZUNInterpEx<Float2>, current);
ValidateFieldOffset32(0x8, ZUNInterpEx<Float2>, initial_value);
ValidateFieldOffset32(0x10, ZUNInterpEx<Float2>, final_value);
ValidateFieldOffset32(0x18, ZUNInterpEx<Float2>, bezier1);
ValidateFieldOffset32(0x20, ZUNInterpEx<Float2>, bezier2);
ValidateFieldOffset32(0x28, ZUNInterpEx<Float2>, time);
ValidateFieldOffset32(0x3C, ZUNInterpEx<Float2>, end_time);
ValidateFieldOffset32(0x40, ZUNInterpEx<Float2>, axis_modes);
ValidateFieldOffset32(0x48, ZUNInterpEx<Float2>, combined_mode);
ValidateFieldOffset32(0x4C, ZUNInterpEx<Float2>, flags);
ValidateStructSize32(0x50, ZUNInterpEx<Float2>);
ValidateFieldOffset32(0x0, ZUNInterpEx<Float3>, current);
ValidateFieldOffset32(0xC, ZUNInterpEx<Float3>, initial_value);
ValidateFieldOffset32(0x18, ZUNInterpEx<Float3>, final_value);
ValidateFieldOffset32(0x24, ZUNInterpEx<Float3>, bezier1);
ValidateFieldOffset32(0x30, ZUNInterpEx<Float3>, bezier2);
ValidateFieldOffset32(0x3C, ZUNInterpEx<Float3>, time);
ValidateFieldOffset32(0x50, ZUNInterpEx<Float3>, end_time);
ValidateFieldOffset32(0x54, ZUNInterpEx<Float3>, axis_modes);
ValidateFieldOffset32(0x60, ZUNInterpEx<Float3>, combined_mode);
ValidateFieldOffset32(0x64, ZUNInterpEx<Float3>, flags);
ValidateStructSize32(0x68, ZUNInterpEx<Float3>);
#pragma endregion

#define SetInstr(value) \
current_instruction = (decltype(current_instruction))(value)

#define IndexInstr(offset) \
SetInstr((intptr_t)current_instruction + (offset))

#define ByteArg(number) \
(((uint8_t*)current_instruction->args)[(number)])

#define OneBitArg(number) \
(ByteArg(number) & 0b1)

#define ShortArg(number) \
(((int16_t*)current_instruction->args)[(number)])

#define RawArg(number) \
(((EclArg*)current_instruction->args)[(number)])

#define IntArg(number) \
(((int32_t*)current_instruction->args)[(number)])

#define UIntArg(number) \
(((uint32_t*)current_instruction->args)[(number)])

#define FloatArg(number) \
(((float*)current_instruction->args)[(number)])

#define Float2Arg(number) \
(((Float2*)current_instruction->args)[(number)])

#define Float3Arg(number) \
(((Float3*)current_instruction->args)[(number)])

#define StringArg(offset) \
((const char*)(current_instruction->args + (offset)))

#define TypeArg(type, number) \
(((type*)current_instruction->args)[(number)])

typedef struct EclVM EclVM;

// size: 0x54
struct EnemyInitData {
    Float3 position; // 0x0
    int32_t score; // 0xC
    int32_t item_drop; // 0x10
    int32_t life; // 0x14
    int32_t mirrored; // 0x18
    int32_t __basic_anm_update; // 0x1C
    int32_t int_vars[4]; // 0x20
    float float_vars[8]; // 0x30
    int32_t parent_id; // 0x50
};
#pragma region // EnemyInitData Validation
ValidateFieldOffset32(0x0, EnemyInitData, position);
ValidateFieldOffset32(0xC, EnemyInitData, score);
ValidateFieldOffset32(0x10, EnemyInitData, item_drop);
ValidateFieldOffset32(0x14, EnemyInitData, life);
ValidateFieldOffset32(0x18, EnemyInitData, mirrored);
ValidateFieldOffset32(0x1C, EnemyInitData, __basic_anm_update);
ValidateFieldOffset32(0x20, EnemyInitData, int_vars);
ValidateFieldOffset32(0x30, EnemyInitData, float_vars);
ValidateFieldOffset32(0x50, EnemyInitData, parent_id);
ValidateStructSize32(0x54, EnemyInitData);
#pragma endregion

// size: 0x10
struct EclInstruction {
    int32_t time; // 0x0
    int16_t opcode; // 0x4
    uint16_t offset_to_next; // 0x6
    uint16_t param_mask; // 0x8
    uint8_t difficulty_mask; // 0xA
    uint8_t param_count; // 0xB
    uint8_t stack_adjust; // 0xC
    padding_bytes(0x3); // 0xD
    unsigned char args[]; // 0x10
};
#pragma region // EclInstruction Validation
ValidateFieldOffset32(0x0, EclInstruction, time);
ValidateFieldOffset32(0x4, EclInstruction, opcode);
ValidateFieldOffset32(0x6, EclInstruction, offset_to_next);
ValidateFieldOffset32(0x8, EclInstruction, param_mask);
ValidateFieldOffset32(0xA, EclInstruction, difficulty_mask);
ValidateFieldOffset32(0xB, EclInstruction, param_count);
ValidateFieldOffset32(0xC, EclInstruction, stack_adjust);
ValidateFieldOffset32(0x10, EclInstruction, args);
ValidateStructSize32(0x10, EclInstruction);
#pragma endregion

#define EclStackCount 0x400
#define EclStackSize sizeof(EclStackItem[EclStackCount])

// size: 0x8
struct EclLocation {
    int32_t sub_index; // 0x0
    int32_t instruction_offset; // 0x4

    inline void reset() {
        this->instruction_offset = -1;
        this->sub_index = -1;
    }
};
#pragma region // EclLocation Validation
ValidateFieldOffset32(0x0, EclLocation, sub_index);
ValidateFieldOffset32(0x4, EclLocation, instruction_offset);
ValidateStructSize32(0x8, EclLocation);
#pragma endregion

typedef char EclSubName[64];

union EclRawValue {
    int32_t integer;
    float real;
    struct {
        char type;
        char type2;
    };
};

struct EclValue {
    char type;
    char type2;
    EclRawValue value;

    template<typename T>
    static forceinline T cast_to(const int32_t& value, const char& type) {
        if constexpr (std::is_same_v<T, int32_t>) {
            if (type == 'f') {
                return bitcast<int32_t>((float)value);
            } else {
                return value;
            }
        } else if constexpr (std::is_same_v<T, float>) {
            if (type != 'f' && type == 'i') {
                return (float)value;
            } else {
                return bitcast<float>(value);
            }
        }
    }

    template<typename T>
    forceinline T read() {
        return this->cast_to<T>(this->value.integer, this->type);
    }
};

using EclStackItem = EclRawValue;

// size: 0x1008
struct EclStack {

    union { // 0x0
        EclStackItem data[EclStackCount];
        uint8_t raw[EclStackSize];
    };
    int32_t pointer; // 0x1000
    int32_t base; // 0x1004

    template<typename T>
    forceinline T read_offset(int32_t offset) {
        return *based_pointer<T>(this->raw, offset);
    }
    template<typename T>
    forceinline T read_typed_offset(int32_t offset) {
        return based_pointer<EclValue>(this->raw, offset)->read<T>();
    }
    template<typename T>
    forceinline T read_local(int32_t offset, int32_t base_offset) {
        return this->read_offset<T>(base_offset + offset);
    }
    template<typename T>
    forceinline T read_local(T offset) {
        return this->read_local<T>((int32_t)offset, this->base);
    }
    /*
    template<typename T>
    forceinline T read_temp(int32_t offset, int32_t top_offset) {
        return this->read_offset<T>(top_offset - offset);
    }
    template<typename T>
    forceinline T read_temp(int32_t offset) {
        return this->read_temp<T>(offset, this->pointer);
    }
    */
    template<typename T>
    forceinline T read_temp(T index, int32_t top_offset);
    template<>
    forceinline int32_t read_temp<int32_t>(int32_t index, int32_t top_offset) {
        return this->read_typed_offset<int32_t>(top_offset + index * sizeof(EclValue));
    }
    template<>
    forceinline float read_temp<float>(float index, int32_t top_offset) {
        index *= -(float)sizeof(EclValue);
        return this->read_typed_offset<float>(top_offset - (int32_t)index);
    }

    template<typename T>
    forceinline T read_temp(T index) {
        return this->read_temp<T>(index, this->pointer);
    }

    template<typename T>
    forceinline void write_offset(int32_t offset, const T& value) {
        *based_pointer<T>(this->raw, offset) = value;
    }
    template<typename T>
    forceinline void write_local(int32_t offset, const T& value, int32_t base_offset) {
        this->write_offset<T>(base_offset + offset, value);
    }
    template<typename T>
    forceinline void write_local(T offset, const T& value) {
        this->write_local<T>((int32_t)offset, value, this->base);
    }
    
    template<typename T>
    forceinline void write_temp(int32_t index, const T& value, int32_t top_offset) {
        this->write_offset<T>(top_offset - index * sizeof(int32_t), value);
    }
    template<typename T>
    forceinline void write_temp(int32_t index, const T& value) {
        this->write_temp<T>(index, value, this->pointer);
    }
    

    template<typename T>
    forceinline T& ref_offset(int32_t offset) {
        return *based_pointer<T>(this->raw, offset);
    }
    template<typename T>
    forceinline T& ref_local(int32_t offset, int32_t base_offset) {
        return this->ref_offset<T>(base_offset + offset);
    }
    template<typename T>
    forceinline T& ref_local(T offset) {
        return this->ref_local<T>((int32_t)offset, this->base);
    }
    
    template<typename T>
    forceinline T& ref_temp(int32_t index, int32_t top_offset) {
        return this->ref_offset<T>(top_offset - index * sizeof(int32_t));
    }
    template<typename T>
    forceinline T& ref_temp(int32_t index) {
        return this->ref_temp<T>(index, this->pointer);
    }
    

    template<typename T>
    forceinline void push(int32_t& starting_offset, const T& value) {
        this->write_offset(starting_offset, value);
        starting_offset += sizeof(int32_t);
    }

    template<typename T>
    forceinline void push(const T& value) {
        this->push(this->pointer, value);
    }

    template<typename T>
    forceinline T pop(int32_t& starting_offset) {
        starting_offset -= sizeof(int32_t);
        return this->read_offset<T>(starting_offset);
    }
    template<typename T>
    forceinline T pop() {
        return this->pop<T>(this->pointer);
    }

    template<typename T>
    forceinline T pop_cast(int32_t& starting_offset) {
        return EclValue::cast_to<T>(this->pop<int32_t>(), this->pop<char>());
    }
    template<typename T>
    forceinline T pop_cast() {
        return this->pop_cast<T>(this->pointer);
    }
    
    template<typename L> requires(FunctionTraitsType<L>::arg_count == 1)
    void unary_op(const L& func) {
        using T = typename FunctionTraitsType<L>::template nth_arg_type<0>;
        using P = typename FunctionTraitsType<L>::ret_type;
        this->push<P>(func(this->pop_cast<T>()));
    }

    template<typename L> requires(FunctionTraitsType<L>::arg_count == 2 && std::is_same_v<typename FunctionTraitsType<L>::template nth_arg_type<0>, typename FunctionTraitsType<L>::template nth_arg_type<1>>)
    void binary_op(const L& func) {
        using T = typename FunctionTraitsType<L>::template nth_arg_type<0>;
        using P = typename FunctionTraitsType<L>::ret_type;
        T right = this->pop_cast<T>();
        T left = this->pop_cast<T>();
        this->push<P>(func(left, right));
    }

    inline ZUNResult enter_frame(int32_t size) {
        int32_t prev_pointer = this->pointer;
        size += prev_pointer;
        if (size >= EclStackSize) {
            return ZUN_ERROR;
        } else {
            this->pointer = size;
            this->push(this->base);
            this->base = prev_pointer;
            return ZUN_SUCCESS;
        }
    }

    ZUNResult leave_frame(void) {
        int32_t new_base = this->pop<int32_t>();
        int32_t new_pointer = this->base;
        this->base = new_base;
        this->pointer = new_pointer;
        if (new_pointer) {
            return ZUN_SUCCESS;
        }
        else {
            return ZUN_ERROR;
        }
    }

    inline void zero_contents() {
        zero_this();
    }

    inline EclStack() {
        this->zero_contents();
    }
};
#pragma region // EclStack Validation
ValidateFieldOffset32(0x0, EclStack, data);
ValidateFieldOffset32(0x1000, EclStack, pointer);
ValidateFieldOffset32(0x1004, EclStack, base);
ValidateStructSize32(0x1008, EclStack);
#pragma endregion

// size: 0x1208
struct EclContext {
    float time; // 0x0
    EclLocation location; // 0x4
    EclStack stack; // 0xC
    int32_t async_id; // 0x1014
    EclVM* vm; // 0x1018
    int32_t __int_101C; // 0x101C
    uint8_t difficulty_mask; // 0x1020
    padding_bytes(3);
    ZUNInterp<float> float_interps[8]; // 0x1024
    int32_t float_interp_stack_offsets[8]; // 0x11A4
    EclLocation float_interp_locations[8]; // 0x11C4
    union {
        uint32_t flags; // 0x1204
        struct {
            uint32_t __unknown_flag_A : 1;
        };
    };
    // 0x1208

    inline void set_float_interp_times(int32_t value) {
        this->float_interps[0].end_time = value;
        this->float_interps[1].end_time = value;
        this->float_interps[2].end_time = value;
        this->float_interps[3].end_time = value;
        this->float_interps[4].end_time = value;
        this->float_interps[5].end_time = value;
        this->float_interps[6].end_time = value;
        this->float_interps[7].end_time = value;
    }

    inline EclInstruction* get_instruction(int32_t sub_index, int32_t instruction_offset);

    // 0x48DBE0
    dllexport EclInstruction* thiscall EclContext::get_current_instruction() asm_symbol_rel(0x48DBE0) {
        int32_t instr_offset = this->location.instruction_offset;
        if (instr_offset != -1) {
            int32_t sub_index = this->location.sub_index;
            if (sub_index != -1) {
                return this->get_instruction(sub_index, instr_offset);
            }
        }
        return NULL;
    }

    inline void step_float_interps();

    forceinline int32_t thiscall get_int_arg(int32_t index, EclInstruction* current_instruction);

    // 0x48D4F0
    dllexport gnu_noinline int32_t thiscall get_int_arg(int32_t index) asm_symbol_rel(0x48D4F0);

    forceinline float vectorcall get_float_arg(int32_t index, EclInstruction* current_instruction);

private:
    // 0x48D5A0
    dllexport gnu_noinline float vectorcall get_float_arg(int, int32_t index) asm_symbol_rel(0x48D5A0);
public:
    forceinline float get_float_arg(int32_t index) {
        return this->get_float_arg(UNUSED_DWORD, index);
    }

    forceinline int32_t thiscall get_int_arg_pop(int32_t index, EclInstruction* current_instruction);

    forceinline int32_t thiscall get_int_arg_pop(int32_t index);

    forceinline float vectorcall get_float_arg_pop(int32_t index, EclInstruction* current_instruction);

    forceinline float vectorcall get_float_arg_pop(int32_t index);

    forceinline int32_t thiscall parse_int_as_arg_pop(int32_t index, int32_t value, EclInstruction* current_instruction);

    // 0x48D690
    dllexport gnu_noinline int32_t thiscall parse_int_as_arg_pop(int32_t index, int32_t value) asm_symbol_rel(0x48D690);

    forceinline float vectorcall parse_float_as_arg_pop(int32_t index, float value, EclInstruction* current_instruction);

    inline float vectorcall parse_float_as_arg_pop(int32_t index, float value);

    forceinline int32_t* thiscall get_int_ptr_arg(int32_t index, EclInstruction* current_instruction);

    // 0x48D750
    dllexport gnu_noinline int32_t* thiscall get_int_ptr_arg(int32_t index = UNUSED_DWORD) asm_symbol_rel(0x48D750);

    forceinline float* thiscall get_float_ptr_arg(int32_t index, EclInstruction* current_instruction);

    // 0x48D7C0
    dllexport gnu_noinline float* thiscall get_float_ptr_arg(int32_t index) asm_symbol_rel(0x48D7C0);

    forceinline int32_t thiscall parse_int_as_arg(int32_t index, int32_t value, EclInstruction* current_instruction);

    forceinline int32_t thiscall parse_int_as_arg(int32_t index, int32_t value);

    forceinline float parse_float_as_arg(int32_t index, float value, EclInstruction* current_instruction);

    forceinline float parse_float_as_arg(int32_t index, float value);

    // 0x42CCC0
    dllexport gnu_noinline EclContext() noexcept(true) {}

    // 0x48B030
    dllexport gnu_noinline ZUNResult thiscall call(EclContext* new_context, int32_t va_index, int32_t = UNUSED_DWORD) asm_symbol_rel(0x48B030);

private:
    // 0x48B3A0
    dllexport gnu_noinline ZUNResult vectorcall low_ecl_run(float, float current_gamespeed) asm_symbol_rel(0x48B3A0);

public:
    forceinline ZUNResult low_ecl_run(float current_gamespeed) {
        return this->low_ecl_run(UNUSED_FLOAT, current_gamespeed);
    }
};
#pragma region // EclContext Validation
ValidateFieldOffset32(0x0, EclContext, time);
ValidateFieldOffset32(0x4, EclContext, location);
ValidateFieldOffset32(0xC, EclContext, stack);
ValidateFieldOffset32(0x1014, EclContext, async_id);
ValidateFieldOffset32(0x1018, EclContext, vm);
ValidateFieldOffset32(0x101C, EclContext, __int_101C);
ValidateFieldOffset32(0x1024, EclContext, float_interps);
ValidateFieldOffset32(0x11A4, EclContext, float_interp_stack_offsets);
ValidateFieldOffset32(0x11C4, EclContext, float_interp_locations);
ValidateFieldOffset32(0x1204, EclContext, flags);
ValidateStructSize32(0x1208, EclContext);
#pragma endregion

using BulletEffectType = enum BulletEffectType : uint32_t;

// size: 0x2C
struct BulletEffectArgs {
    float float_values[4]; // 0x0
    int32_t int_values[4]; // 0x10
    BulletEffectType type; // 0x20
    int32_t async; // 0x24
    const char* string; // 0x28
    // 0x2C
};
#pragma region // BulletEffectArgs Validation
ValidateFieldOffset32(0x0, BulletEffectArgs, float_values);
ValidateFieldOffset32(0x10, BulletEffectArgs, int_values);
ValidateFieldOffset32(0x20, BulletEffectArgs, type);
ValidateFieldOffset32(0x24, BulletEffectArgs, async);
ValidateFieldOffset32(0x28, BulletEffectArgs, string);
ValidateStructSize32(0x2C, BulletEffectArgs);
#pragma endregion

static inline constexpr int32_t BULLET_EFFECT_MAX = 24;

// size: 0x488
struct ShooterData {
    int32_t type; // 0x0
    int32_t color; // 0x4
    Float3 position; // 0x8
    float angle1; // 0x14
    float angle2; // 0x18
    float speed1; // 0x1C
    float speed2; // 0x20
    float distance; // 0x24
    BulletEffectArgs effects[BULLET_EFFECT_MAX]; // 0x28
    float width; // 0x448
    unknown_fields(0xC); // 0x44C
    int32_t start_time; // 0x458
    int32_t __transform_A; // 0x45C
    int32_t duration; // 0x460
    int32_t __transform_B; // 0x464
    int32_t __laser_flags; // 0x468
    int16_t count1; // 0x46C
    int16_t count2; // 0x46E
    int32_t aim_mode; // 0x470
    union {
        uint32_t flags; // 0x474
        struct {
            uint32_t : 5;
            uint32_t play_sound_on_spawn : 1;
        };
    };
    int32_t shoot_sound; // 0x478
    int32_t transform_sound; // 0x47C
    int32_t start_transform; // 0x480
    int __dword_484; // 0x484
    // 0x488

    inline void zero_contents() {
        zero_this();
    }

    inline ShooterData() {
        this->zero_contents();
        this->transform_sound = -1;
    }

    inline ShooterData(int) {}
};
#pragma region // ShooterData Validation
ValidateFieldOffset32(0x0, ShooterData, type);
ValidateFieldOffset32(0x4, ShooterData, color);
ValidateFieldOffset32(0x8, ShooterData, position);
ValidateFieldOffset32(0x14, ShooterData, angle1);
ValidateFieldOffset32(0x18, ShooterData, angle2);
ValidateFieldOffset32(0x1C, ShooterData, speed1);
ValidateFieldOffset32(0x20, ShooterData, speed2);
ValidateFieldOffset32(0x24, ShooterData, distance);
ValidateFieldOffset32(0x28, ShooterData, effects);
ValidateFieldOffset32(0x448, ShooterData, width);
ValidateFieldOffset32(0x458, ShooterData, start_time);
ValidateFieldOffset32(0x45C, ShooterData, __transform_A);
ValidateFieldOffset32(0x460, ShooterData, duration);
ValidateFieldOffset32(0x464, ShooterData, __transform_B);
ValidateFieldOffset32(0x468, ShooterData, __laser_flags);
ValidateFieldOffset32(0x46C, ShooterData, count1);
ValidateFieldOffset32(0x46E, ShooterData, count2);
ValidateFieldOffset32(0x470, ShooterData, aim_mode);
ValidateFieldOffset32(0x474, ShooterData, flags);
ValidateFieldOffset32(0x478, ShooterData, shoot_sound);
ValidateFieldOffset32(0x47C, ShooterData, transform_sound);
ValidateFieldOffset32(0x480, ShooterData, start_transform);
ValidateFieldOffset32(0x484, ShooterData, __dword_484);
ValidateStructSize32(0x488, ShooterData);
#pragma endregion

union EnemyID {
    uint32_t raw;

    inline constexpr operator uint32_t() const {
        return this->raw;
    }
    template<typename T> requires(std::is_integral_v<T>)
        inline constexpr EnemyID& operator=(const T& raw) {
        this->raw = raw;
        return *this;
    }

    // 0x42E0C0
    dllexport gnu_noinline Enemy* thiscall get_enemy_ptr() asm_symbol_rel(0x42E0C0);
};

static inline constexpr uint32_t MAX_CALLBACKS = 8;

// size: 0x88
struct EnemyCallback {
    int32_t life; // 0x0
    int32_t time; // 0x4
    EclSubName life_sub; // 0x8
    EclSubName time_sub; // 0x48

    inline void initialize() {
        this->life = -1;
        this->time = -1;
        this->life_sub[0] = '\0';
    }
};

// WEIRD CALLING CONVENTION
struct HitboxManager {
private:
    // 0x45F0F0
    static dllexport gnu_noinline int32_t vectorcall enm_compute_damage_sources(int32_t, int32_t, float, float, float, Float3* position, Float2* size, float rotation, float radius, int32_t* arg5, Float3* hit_position_out, BOOL arg7, int32_t enemy_id) asm_symbol_rel(0x45F0F0) {
        
    }
public:
    static forceinline int32_t enm_compute_damage_sources(Float3* position, Float2* size, float rotation, float radius, int32_t* arg5, Float3* hit_position_out, BOOL arg7, int32_t enemy_id) {
        return enm_compute_damage_sources(UNUSED_DWORD, UNUSED_DWORD, UNUSED_FLOAT, UNUSED_FLOAT, UNUSED_FLOAT, position, size, rotation, radius, arg5, hit_position_out, arg7, enemy_id);
    }
};

typedef struct AbilityShop AbilityShop;
extern "C" {
    // 0x4CF2A4
    extern AbilityShop* ABILITY_SHOP_PTR asm("_ABILITY_SHOP_PTR");
}

typedef struct BombBase BombBase;
extern "C" {
    // 0x4CF2B8
    extern BombBase* BOMB_PTR asm("_BOMB_PTR");
}

// size: 0xA8
struct BombBase : ZUNTask {
    // void* vftable; // 0x0
    // ZUNTask base; // 0x4
    unknown_fields(0x4); // 0x10
    Float3 __float3_14; // 0x14
    unknown_fields(0x4); // 0x20
    Float3 __float3_24; // 0x24 (only 0x2C is know so far, but it's Z rotation)
    BOOL bomb_active; // 0x30
    Timer __timer_34; // 0x34
    unknown_fields(0x14); // 0x48
    AnmID __vm_id_5C; // 0x5C
    AnmID __vm_id_60; // 0x60
    AnmID __vm_id_64; // 0x64
    unknown_fields(0x8); // 0x68
    void* __ptr_70; // 0x70
    float __float_74; // 0x74
    float __float_78; // 0x78
    Timer __timer_7C; // 0x7C
    int __int_90; // 0x90
    Float3 __float3_94; // 0x94
    int __dword_A0; // 0xA0
    int __int_A4; // 0xA4
    // 0xA8

    inline void zero_contents() {
        zero_this();
    }

    inline BombBase() {
        this->zero_contents();
        this->__unknown_task_flag_A = true;
    }

    // Method 0
    // 0x41FC10
    virtual ~BombBase() {
        this->__vm_id_5C.mark_tree_for_delete();
        this->__vm_id_60.mark_tree_for_delete();
        this->__vm_id_64.mark_tree_for_delete();
        // TODO
        SAFE_FREE(this->__ptr_70);
    }

    // Method 4
    // 0x41FB80
    dllexport gnu_noinline virtual int thiscall activate() {
        return 0;
    }
    // Method 8
    // 0x41FB90
    dllexport gnu_noinline virtual int thiscall on_tick_impl() {
        return 0;
    }
    // Method C
    // 0x41FBA0
    dllexport gnu_noinline virtual int thiscall on_draw_impl() {
        return 0;
    }
    // Method 10
    // 0x41FBB0
    dllexport gnu_noinline virtual int thiscall __method_10(int, int) {
        return 0;
    }
    // Method 14
    // 0x41FBC0
    dllexport gnu_noinline virtual int thiscall __method_14() {
        return 0;
    }
    // Method 18
    // 0x41FBD0
    dllexport gnu_noinline virtual void thiscall __method_18() {
    }

    // 0x41FD40
    dllexport gnu_noinline static void allocate();

    // 0x420040
    dllexport gnu_noinline static UpdateFuncRet fastcall on_tick(void* ptr) {
        BombBase* self = (BombBase*)ptr;
        if (!ABILITY_SHOP_PTR) {
            if (self->__timer_34 < 0) {
                ++self->__timer_34;
            }
            if (self->bomb_active) {
                if (!self->on_tick_impl()) {
                    self->bomb_active = FALSE;
                    return UpdateFuncNext;
                }
                ++self->__timer_34;
            }
        }
        return UpdateFuncNext;
    }

    // 0x420160
    dllexport gnu_noinline static UpdateFuncRet fastcall on_draw(void* ptr) {
        BombBase* self = (BombBase*)ptr;
        self->on_draw_impl();
        if (self->__timer_7C <= 0) {
            self->__float_78 = -1.0f;
            return UpdateFuncNext;
        }
        if (self->__timer_7C <= 60) {
            // TODO
            --self->__timer_7C;
        }
        return UpdateFuncNext;
    }
};

#pragma region // BombBase Validation
ValidateVirtualFieldOffset32(0x30, BombBase, bomb_active);
ValidateStructSize32(0xA8, BombBase);
#pragma endregion

// size: 0x14A0
struct BombReimuAData {
    unknown_fields(0x14A0); // 0x0
    // 0x14A0

    inline void zero_contents() {
        zero_this();
    }

    // 0x4212A0
    dllexport gnu_noinline void thiscall __sub_4212A0() asm_symbol_rel(0x4212A0) {
        // TODO
    }
};

struct BombReimuA : BombBase {
    // Method 4
    // 0x421420
    dllexport gnu_noinline virtual int thiscall activate() {
        // TODO
        //SOUND_MANAGER.play_sound(49);
        // TODO
        //++ENEMY_MANAGER_PTR->player_bomb_count;
        SAFE_FREE(this->__ptr_70);
        this->__ptr_70 = malloc(sizeof(BombReimuAData));
        memset(this->__ptr_70, 0, sizeof(BombReimuAData));
        // TODO
        return 0;
    }
    // Method 8
    // 0x421590
    dllexport gnu_noinline virtual int thiscall on_tick_impl() {
        // TODO
        if (this->__timer_34 >= 120) {
            // TODO
        }
        // TODO
        return 0;
    }
    // Method C
    // 0x422380
    dllexport gnu_noinline virtual int thiscall on_draw_impl() {
        return 1;
    }
    // Method 10
    // 0x422390
    dllexport gnu_noinline virtual int thiscall __method_10(int, int) {
        return 0;
    }
    // Method 14
    // 0x4223A0
    dllexport gnu_noinline virtual int thiscall __method_14() {
        // TODO
        return 0;
    }
    // Method 18
    // 0x41FBD0
    dllexport gnu_noinline virtual void thiscall __method_18() {
        ((BombReimuAData*)this->__ptr_70)->__sub_4212A0();
        this->__vm_id_64.interrupt_tree(1);
        SAFE_FREE(this->__ptr_70);
        // TODO
        this->bomb_active = FALSE;
    }
};

struct BombMarisaA : BombBase {

};

struct BombSakuya : BombBase {

};

struct BombSanaeA : BombBase {

};

// 0x41FD40
dllexport gnu_noinline void BombBase::allocate() {
    BombBase* bomb_ptr;
    switch (GAME_MANAGER.globals.shottype_index()) {
        default:
            bomb_ptr = new BombReimuA();
            break;
        case 1:
            bomb_ptr = new BombMarisaA();
            break;
        case 2:
            bomb_ptr = new BombSakuya();
            break;
        case 3:
            bomb_ptr = new BombSanaeA();
            break;
    }
    UpdateFunc* update_func = new UpdateFunc(&on_tick, true, bomb_ptr);
    UpdateFuncRegistry::register_on_tick(update_func, 25);
    bomb_ptr->on_tick_func = update_func;
    update_func = new UpdateFunc(&on_draw, true, bomb_ptr);
    UpdateFuncRegistry::register_on_draw(update_func, 41);
    bomb_ptr->on_draw_func = update_func;
    bomb_ptr->__timer_34.reset();
    bomb_ptr->__timer_7C.reset();
    bomb_ptr->__float_78 = -1.0f;
    bomb_ptr->__float_74 = 0.0f;
    bomb_ptr->__dword_A0 = 0;
    bomb_ptr->__int_A4 = 0;
    BOMB_PTR = bomb_ptr;
}

// size: 0xC4
struct EnemyDrops {
    int32_t main_id; // 0x0
    int32_t extra_ids[20]; // 0x4
    int32_t limited_extra_ids[20]; // 0x54
    int32_t limited_end_time; // 0xA4
    Timer limited_timer; // 0xA8
    float width; // 0xBC
    float height; // 0xC0
    // 0xC4

    inline void zero_contents() {
        zero_this();
    }

    // 0x42D010
    dllexport gnu_noinline void set_extra_drop(int32_t index, int32_t id) asm_symbol_rel(0x42D010) {
        this->extra_ids[index - 1] = id;
    }

    // 0x42D020
    dllexport gnu_noinline void set_timed_extra_drop(int32_t index, int32_t id) asm_symbol_rel(0x42D020) {
        this->limited_extra_ids[index - 1] = id;
    }

    // 0x42D040
    dllexport gnu_noinline void set_timed_drop_time(int32_t time) asm_symbol_rel(0x42D040) {
        this->limited_end_time = time;
        this->limited_timer.reset();
    }

private:
    // 0x42D090
    dllexport gnu_noinline void vectorcall set_area(float, float width, float height) asm_symbol_rel(0x42D090) {
        this->width = width;
        this->height = height;
    }
public:
    inline void vectorcall set_area(float width, float height) {
        return this->set_area(UNUSED_FLOAT, width, height);
    }

    // 0x42D0B0
    dllexport gnu_noinline void set_main_drop(int32_t id) asm_symbol_rel(0x42D0B0) {
        this->main_id = id;
    }

    // 0x430510
    dllexport gnu_noinline void spawn_extra_items(Float3* position) asm_symbol_rel(0x430510);

    // 0x42D0C0
    dllexport void spawn_items(Float3* position) asm_symbol_rel(0x42D0C0);

    // 0x42D110
    dllexport gnu_noinline void initialize() asm_symbol_rel(0x42D110) {
        this->zero_contents();
        this->height = 32.0f;
        this->width = 32.0f;
        this->limited_timer.reset();
    }

    inline void reset() {
        memset(this->extra_ids, 0, sizeof(EnemyDrops) - sizeof(this->main_id));
        this->height = 32.0f;
        this->width = 32.0f;
        this->main_id = 0;
        this->limited_timer.reset();
    }

    // 0x42D180
    dllexport gnu_noinline void reset_extra_drops() asm_symbol_rel(0x42D180) {
        memset(this->extra_ids, 0, sizeof(this->extra_ids) + sizeof(this->limited_extra_ids));
        this->limited_end_time = 0;
        this->limited_timer.reset();
    }
};

// size: 0x1C
struct EnemyLife {
    int32_t current; // 0x0, 0x4FF4, 0x6220
    int32_t maximum; // 0x4, 0x4FF8, 0x6224
    int32_t remaining_current_attack; // 0x8, 0x4FFC, 0x6228
    int32_t current_scaled_by_seven; // 0xC, 0x5000, 0x622C
    int32_t starting_value_for_next_attack; // 0x10, 0x5004, 0x6230
    int32_t total_damage_including_ignored; // 0x14, 0x5008, 0x6234
    union {
        uint32_t flags; // 0x18, 0x500C, 0x6238
        struct {
            uint32_t is_spell : 1; // 1
            uint32_t __unknown_flag_A : 1; // 2
        };
    };
    // 0x1C

    inline void initialize() {
        this->__unknown_flag_A = false;
        this->current = 0;
        this->maximum = 0;
        this->remaining_current_attack = 0;
        this->total_damage_including_ignored = 0;
    }

    // 0x42CF80
    dllexport gnu_noinline int32_t get_current() asm_symbol_rel(0x42CF80) {
        return this->current;
    }

    // 0x42CF90
    dllexport gnu_noinline void set_current_scaled(int32_t value) asm_symbol_rel(0x42CF90) {
        this->current_scaled_by_seven = value * 7;
    }

    // 0x42CFB0
    dllexport gnu_noinline void set_spell(int32_t value) asm_symbol_rel(0x42CFB0) {
        this->is_spell = value;
    }

    // 0x42CFD0
    dllexport gnu_noinline int32_t get_maximum() asm_symbol_rel(0x42CFD0) {
        return this->maximum;
    }

    // 0x42CFE0
    dllexport gnu_noinline void set_maximum(int32_t value) asm_symbol_rel(0x42CFE0) {
        this->maximum = value;
    }

    // 0x42CFF0
    dllexport gnu_noinline void set_current_remaining(int32_t value) asm_symbol_rel(0x42CFF0) {
        this->remaining_current_attack = value;
    }

    // 0x42D000
    dllexport gnu_noinline void set_current(int32_t value) asm_symbol_rel(0x42D000) {
        this->current = value;
    }

    inline void take_damage(int32_t damage, bool invulnerable) {
        if (!invulnerable) {
            this->total_damage_including_ignored += damage;
            if (this->is_spell) {
                this->current_scaled_by_seven -= damage;
                int32_t next_base = this->starting_value_for_next_attack;
                this->current = next_base + (this->current_scaled_by_seven - next_base * 7) / 7;
            } else {
                this->current -= damage;
            }
        } else {
            this->total_damage_including_ignored += damage;
        }
    }
};

// size: 0x1C
struct EnemyFog {
    void* fog_ptr; // 0x0
    unknown_fields(0x4); // 0x4
    float radius; // 0x8
    float __float_C; // 0xC
    D3DCOLOR color; // 0x10
    float __angle_14; // 0x14
    float __angle_18; // 0x18
    // 0x1C
};

typedef struct EnemyData EnemyData;

typedef int32_t fastcall ExtraDamageFunc(EnemyData* enemy_data, int32_t prev_damage);
typedef int32_t fastcall ExtraHitboxFunc(EnemyData* enemy_data);

// size: 0x5600
struct EnemyData {
    MotionData previous_motion; // 0x0, 0x122C
    MotionData current_motion; // 0x44, 0x1270
    ZUNAbsRel<MotionData> motion; // 0x88, 0x12B4
    Float2 hitbox_size; // 0x110, 0x133C
    Float2 collision_size; // 0x118, 0x1344
    float hitbox_rotation; // 0x120, 0x134C
    AnmID anm_vms[16]; // 0x124, 0x1350
    Float3 anm_positions[16]; // 0x164, 0x1390
    int32_t anm_vm_indices[16]; // 0x224, 0x1450
    int32_t anm_source_index; // 0x264, 0x1490
    int32_t anm_slot_0_source_index; // 0x268, 0x1494
    int32_t anm_slot_0_script; // 0x26C, 0x1498
    int32_t current_anm_script; // 0x270, 0x149C
    int32_t current_anm_pose; // 0x274, 0x14A0
    int32_t kill_id; // 0x278, 0x14A4
    int32_t anm_base_layer; // 0x27C, 0x14A8
    Float3 position_of_last_damage_source_to_hit; // 0x280, 0x14AC
    int32_t int_vars[4]; // 0x28C, 0x14B8
    float float_vars[8]; // 0x29C, 0x14C8
    Timer ecl_time; // 0x2BC, 0x14E8
    Timer boss_timer; // 0x2D0, 0x14FC
    float slowdown; // 0x2E4, 0x1510
    ZUNList<Enemy> global_list_node; // 0x2E8, 0x1514
    ZUNAbsRel<ZUNInterpEx<Float3>> position_interp; // 0x2F8, 0x1524
    ZUNInterp<float> angle_interp_absolute; // 0x3C8, 0x15F4
    ZUNInterp<float> speed_interp_absolute; // 0x3F8, 0x1624
    ZUNInterp<float> angle_interp_relative; // 0x428, 0x1654
    ZUNInterp<float> speed_interp_relative; // 0x458, 0x1684
    ZUNAbsRel<ZUNInterp<Float2>> orbit_radius_interp; // 0x488, 0x16B4
    ZUNAbsRel<ZUNInterp<Float2>> ellipse_interp; // 0x510, 0x173C
    ShooterData shooters[16]; // 0x598, 0x17C4
    int32_t bullet_effect_indices[16]; // 0x4E18, 0x6044
    Float3 shooter_offsets[16]; // 0x4E58, 0x6084
    Float3 shooter_origins[16]; // 0x4F18, 0x6144
    Float2 final_sprite_size; // 0x4FD8, 0x6204
    Float2 move_bounds_center; // 0x4FE0, 0x620C
    Float2 move_bounds_size; // 0x4FE8, 0x6214
    int32_t score; // 0x4FF0, 0x621C
    EnemyLife life; // 0x4FF4, 0x6220
    EnemyDrops drops; // 0x5010, 0x623C
    int32_t __int_50D4; // 0x50D4, 0x6300
    int32_t death_sound; // 0x50D8, 0x6304
    int32_t death_anm_script; // 0x50DC, 0x6308
    int32_t death_anm_index; // 0x50E0, 0x630C
    int32_t __int_50E4; // 0x50E4, 0x6310
    int __dword_50E8; // 0x50E8, 0x6314
    int32_t hit_sound; // 0x50EC, 0x6318
    Timer invulnerable_timer; // 0x50F0, 0x631C
    Timer no_collision_timer; // 0x5104, 0x6330
    Timer __timer_5118; // 0x5118, 0x6344
    float bomb_damage_multiplier; // 0x512C, 0x6358
    union {
        uint32_t flags_low; // 0x5130, 0x635C
        struct {
            uint32_t disable_hitbox : 1; // 1
            uint32_t disable_collision : 1; // 2
            uint32_t offscreen_immune_horizontal : 1; // 3
            uint32_t offscreen_immune_vertical : 1; // 4
            uint32_t invincible : 1; // 5
            uint32_t intangible : 1; // 6
            uint32_t __is_midboss : 1; // 7
            uint32_t no_delete : 1; // 8
            uint32_t always_delete : 1; // 9
            uint32_t grazable : 1; // 10
            uint32_t only_dialogue_delete : 1; // 11
            uint32_t delete_as_bullet : 1; // 12
            uint32_t rectangular_hitbox : 1; // 13
            uint32_t slowdown_immune : 1; // 14
            uint32_t __unknown_flag_E : 1; // 15
            uint32_t __unknown_flag_F : 1; // 16
            uint32_t __unknown_flag_G : 1; // 17
            uint32_t move_bounds_enable : 1; // 18
            uint32_t __unknown_flag_A : 1; // 19
            uint32_t mirrored : 1; // 20
            uint32_t __unknown_flag_I : 1; // 21
            uint32_t __damaged_this_frame : 1; // 22
            uint32_t __delete_related : 1; // 23 like is_boss, but skips something in the GUI code
            uint32_t is_boss : 1; // 24
            uint32_t __unknown_flag_L : 1; // 25
            uint32_t __unknown_flag_B : 1; // 26
            uint32_t __basic_anm_update : 1; // 27
            uint32_t homing_disable : 1; // 28
            uint32_t bomb_shield : 1; // 29
            uint32_t bomb_shield_active : 1; // 30
            uint32_t __unknown_flag_O : 1; // 31
            uint32_t __anm_related_flag_A : 1; // 32
        };
    };
    union {
        uint32_t flags_high; // 0x5134, 0x6360
        struct {
            uint32_t __anm_slowdown_immune : 1; // 1
            uint32_t : 1; // 2
            uint32_t __unknown_flag_P : 1; // 3
        };
    };
    int32_t bombshield_on_anm; // 0x5138, 0x6364
    int32_t bombshield_off_anm; // 0x513C, 0x6368
    int32_t boss_id; // 0x5140, 0x636C
    float player_protect_radius; // 0x5144, 0x6370
    EnemyCallback callbacks[MAX_CALLBACKS]; // 0x5148, 0x6374
    EclVM* vm; // 0x5588, 0x67B4
    EnemyFog fog; // 0x558C, 0x67B8
    EclSubName death_callback_sub; // 0x55A8, 0x67D4
    ZUNResult(thiscall *func_set_func)(EnemyData*); // 0x55E8, 0x6814
    uint32_t __is_func_set_2; // 0x55EC, 0x6818
    ExtraDamageFunc* extra_damage_func; // 0x55F0, 0x681C
    ExtraHitboxFunc* extra_hitbox_func; // 0x55F4, 0x6820
    int32_t chapter; // 0x55F8, 0x6824
    int32_t chapter_spawn_weight; // 0x55FC, 0x6828
    // 0x5600, 0x682C

    inline Enemy* enemy() {
        return (Enemy*)this->vm;
    }

    inline EnemyData() {
        zero_this();
    }

    inline void initialize_callbacks() {
        this->callbacks[0].initialize();
        this->callbacks[1].initialize();
        this->callbacks[2].initialize();
        this->callbacks[3].initialize();
        this->callbacks[4].initialize();
        this->callbacks[5].initialize();
        this->callbacks[6].initialize();
        this->callbacks[7].initialize();
    }

    inline void initialize_anm_vm_slots() {
        this->anm_vm_indices[0] = -1;
        this->anm_vm_indices[1] = -1;
        this->anm_vm_indices[2] = -1;
        this->anm_vm_indices[3] = -1;
        this->anm_vm_indices[4] = -1;
        this->anm_vm_indices[5] = -1;
        this->anm_vm_indices[6] = -1;
        this->anm_vm_indices[7] = -1;
        this->anm_vm_indices[8] = -1;
        this->anm_vm_indices[9] = -1;
        this->anm_vm_indices[10] = -1;
        this->anm_vm_indices[11] = -1;
        this->anm_vm_indices[12] = -1;
        this->anm_vm_indices[13] = -1;
        this->anm_vm_indices[14] = -1;
        this->anm_vm_indices[15] = -1;
    }

    inline void set_anm_vm_slowdowns(const float& slowdown_value);

    inline void initialize(Enemy* enemy);

    // 0x42D1E0
    dllexport gnu_noinline MotionData& get_current_motion() asm_symbol_rel(0x42D1E0) {
        return this->current_motion;
    }

    inline Float3& get_position() {
        return this->get_current_motion().position;
    }

    // 0x42D1F0
    dllexport gnu_noinline EnemyLife& get_life_data() asm_symbol_rel(0x42D1F0) {
        return this->life;
    }

    forceinline bool get_mirror_flag() {
        return this->mirrored;
    }

    inline bool flags_allow_kill() {
        return !(this->is_boss | this->__delete_related | this->intangible | this->no_delete | this->only_dialogue_delete) ||
                this->always_delete;
    }

    inline bool has_active_hitbox() {
        return !(this->disable_hitbox | this->intangible);
    }

    inline bool has_active_collision() {
        return !(this->disable_collision | this->intangible) && this->no_collision_timer <= 0;
    }

    inline bool is_invulnerable() {
        return this->invincible || this->invulnerable_timer > 0;
    }

    inline void enforce_move_bounds() {
        if (this->move_bounds_enable) {
            float x_limit_size = this->move_bounds_size.x * 0.5f;
            float x_limit_pos = this->move_bounds_center.x;
            float x_pos = this->current_motion.position.x;
            {
                float temp = x_limit_pos - x_limit_size;
                if (temp > x_pos) {
                    this->current_motion.position.x = temp;
                } else {
                    temp = x_limit_pos + x_limit_size;
                    if (x_pos > temp) {
                        this->current_motion.position.x = temp;
                    }
                }
            }
            float y_limit_size = this->move_bounds_size.y * 0.5f;
            float y_limit_pos = this->move_bounds_center.y;
            float y_pos = this->current_motion.position.y;
            {
                float temp = y_limit_pos - y_limit_size;
                if (temp > y_pos) {
                    this->current_motion.position.y = temp;
                } else {
                    temp = y_limit_pos + y_limit_size;
                    if (y_pos > temp) {
                        this->current_motion.position.y = temp;
                    }
                }
            }
            this->motion.absolute.position = this->current_motion.position - this->motion.relative.position;
        }
    }

    // 0x42EC00
    dllexport gnu_noinline void update_current_motion() asm_symbol_rel(0x42EC00) {
        this->current_motion.axis_velocity = this->motion.relative.position + this->motion.absolute.position - this->current_motion.position;
        this->current_motion.update();
        this->enforce_move_bounds();
    }

    inline ZUNResult run_ecl();

    // 0x430D40
    dllexport gnu_noinline int32_t thiscall high_ecl_run() asm_symbol_rel(0x430D40);

    // 0x438AA0
    dllexport gnu_noinline int32_t thiscall get_int_arg(int32_t index) asm_symbol_rel(0x438AA0);

    // 0x438AC0
    dllexport gnu_noinline int32_t* thiscall get_int_ptr_arg(int32_t index = UNUSED_DWORD) asm_symbol_rel(0x438AC0);

private:
    // 0x438AD0
    dllexport gnu_noinline float vectorcall get_float_arg(int32_t, int32_t index) asm_symbol_rel(0x438AD0);

public:
    forceinline float get_float_arg(int32_t index) {
        return this->get_float_arg(UNUSED_DWORD, index);
    }

    // 0x438AF0
    dllexport gnu_noinline float* thiscall get_float_ptr_arg(int32_t index) asm_symbol_rel(0x438AF0);

    // 0x438B10
    dllexport gnu_noinline int32_t thiscall parse_int_as_arg(int32_t index, int32_t value) asm_symbol_rel(0x438B10);

private:
    // 0x438BC0
    dllexport gnu_noinline float vectorcall parse_float_as_arg(int32_t, int32_t index, float, float, float value) asm_symbol_rel(0x438BC0);

public:
    forceinline float parse_float_as_arg(int32_t index, float value) {
        return this->parse_float_as_arg(UNUSED_DWORD, index, UNUSED_FLOAT, UNUSED_FLOAT, value);
    }

    // 0x4369E0
    dllexport gnu_noinline int32_t thiscall ecl_enm_create() asm_symbol_rel(0x4369E0);

    // 0x42E5A0
    dllexport gnu_noinline ZUNResult thiscall __move() asm_symbol_rel(0x42E5A0) {
        clang_noinline this->previous_motion = this->current_motion;
        // TODO: Finish this function and all the giga-jank in it

        return ZUN_SUCCESS;
    }

    forceinline ZUNResult thiscall update();

    // 0x42ED40
    dllexport gnu_noinline ZUNResult thiscall __update_state() asm_symbol_rel(0x42ED40);

    // 0x42F890
    dllexport gnu_noinline void thiscall __update_fog() asm_symbol_rel(0x42F890) {

    }

    // 0x42FF80
    dllexport gnu_noinline ZUNResult thiscall on_tick() asm_symbol_rel(0x42FF80);
    
    // 0x439480
    dllexport gnu_noinline static int32_t fastcall extra_damage_func1(EnemyData* enemy_data, int32_t prev_damage) asm_symbol_rel(0x439480) {
        int32_t new_damage = enemy_data->int_vars[3];
        if (new_damage > 0) {
            new_damage += prev_damage;
            enemy_data->int_vars[3] = 0;
            return new_damage;
        } else {
            return prev_damage;
        }
    }
    
    // 0x4394A0
    dllexport gnu_noinline static int32_t fastcall extra_damage_func2(EnemyData* enemy_data, int32_t prev_damage) asm_symbol_rel(0x4394A0);

    // 0x436BB0
    dllexport gnu_noinline void thiscall anm_set_slot_impl() asm_symbol_rel(0x436BB0);

    inline void thiscall anm_set_slot_main_impl();

    inline void thiscall anm_play_main_impl();

    inline void thiscall anm_play_attack_impl();

    inline void thiscall anm_play_attack_ex_impl();

    template <bool sanity_check_script>
    inline float kill_effects();

    template <bool inline_calls = true>
    inline void add_spawn_weight_to_chapter_destroy() {
        if constexpr (inline_calls) {
            if (
                this->chapter_spawn_weight &&
                this->chapter == GAME_MANAGER.globals.chapter
            ) {
                GAME_MANAGER.globals.__int_90 += this->chapter_spawn_weight;
                this->chapter_spawn_weight = 0;
            }
        }
        else {
            if (
                this->chapter_spawn_weight &&
                this->chapter == GAME_MANAGER.get_chapter()
            ) {
                GAME_MANAGER.globals.__add_to_int_90(this->chapter_spawn_weight);
                this->chapter_spawn_weight = 0;
            }
        }
    }

    inline void reinitialize_vm_with_sub(const char* sub_name);
};
#pragma region // EnemyData Field Validation
ValidateFieldOffset32(0x0, EnemyData, previous_motion);
ValidateFieldOffset32(0x44, EnemyData, current_motion);
ValidateFieldOffset32(0x88, EnemyData, motion);
ValidateFieldOffset32(0x110, EnemyData, hitbox_size);
ValidateFieldOffset32(0x118, EnemyData, collision_size);
ValidateFieldOffset32(0x120, EnemyData, hitbox_rotation);
ValidateFieldOffset32(0x124, EnemyData, anm_vms);
ValidateFieldOffset32(0x164, EnemyData, anm_positions);
ValidateFieldOffset32(0x224, EnemyData, anm_vm_indices);
ValidateFieldOffset32(0x264, EnemyData, anm_source_index);
ValidateFieldOffset32(0x268, EnemyData, anm_slot_0_source_index);
ValidateFieldOffset32(0x26C, EnemyData, anm_slot_0_script);
ValidateFieldOffset32(0x270, EnemyData, current_anm_script);
ValidateFieldOffset32(0x274, EnemyData, current_anm_pose);
ValidateFieldOffset32(0x278, EnemyData, kill_id);
ValidateFieldOffset32(0x27C, EnemyData, anm_base_layer);
ValidateFieldOffset32(0x280, EnemyData, position_of_last_damage_source_to_hit);
ValidateFieldOffset32(0x28C, EnemyData, int_vars);
ValidateFieldOffset32(0x29C, EnemyData, float_vars);
ValidateFieldOffset32(0x2BC, EnemyData, ecl_time);
ValidateFieldOffset32(0x2D0, EnemyData, boss_timer);
ValidateFieldOffset32(0x2E4, EnemyData, slowdown);
ValidateFieldOffset32(0x2E8, EnemyData, global_list_node);
ValidateFieldOffset32(0x2F8, EnemyData, position_interp);
ValidateFieldOffset32(0x3C8, EnemyData, angle_interp_absolute);
ValidateFieldOffset32(0x3F8, EnemyData, speed_interp_absolute);
ValidateFieldOffset32(0x428, EnemyData, angle_interp_relative);
ValidateFieldOffset32(0x458, EnemyData, speed_interp_relative);
ValidateFieldOffset32(0x488, EnemyData, orbit_radius_interp);
ValidateFieldOffset32(0x510, EnemyData, ellipse_interp);
ValidateFieldOffset32(0x598, EnemyData, shooters);
ValidateFieldOffset32(0x4E18, EnemyData, bullet_effect_indices);
ValidateFieldOffset32(0x4E58, EnemyData, shooter_offsets);
ValidateFieldOffset32(0x4F18, EnemyData, shooter_origins);
ValidateFieldOffset32(0x4FD8, EnemyData, final_sprite_size);
ValidateFieldOffset32(0x4FE0, EnemyData, move_bounds_center);
ValidateFieldOffset32(0x4FE8, EnemyData, move_bounds_size);
ValidateFieldOffset32(0x4FF0, EnemyData, score);
ValidateFieldOffset32(0x4FF4, EnemyData, life);
ValidateFieldOffset32(0x5010, EnemyData, drops);
ValidateFieldOffset32(0x50D4, EnemyData, __int_50D4);
ValidateFieldOffset32(0x50E8, EnemyData, __dword_50E8);
ValidateFieldOffset32(0x50EC, EnemyData, hit_sound);
ValidateFieldOffset32(0x50F0, EnemyData, invulnerable_timer);
ValidateFieldOffset32(0x5104, EnemyData, no_collision_timer);
ValidateFieldOffset32(0x5118, EnemyData, __timer_5118);
ValidateFieldOffset32(0x512C, EnemyData, bomb_damage_multiplier);
ValidateFieldOffset32(0x5130, EnemyData, flags_low);
ValidateFieldOffset32(0x5134, EnemyData, flags_high);
ValidateFieldOffset32(0x5138, EnemyData, bombshield_on_anm);
ValidateFieldOffset32(0x513C, EnemyData, bombshield_off_anm);
ValidateFieldOffset32(0x5140, EnemyData, boss_id);
ValidateFieldOffset32(0x5144, EnemyData, player_protect_radius);
ValidateFieldOffset32(0x5148, EnemyData, callbacks);
ValidateFieldOffset32(0x5588, EnemyData, vm);
ValidateFieldOffset32(0x558C, EnemyData, fog);
ValidateFieldOffset32(0x55A8, EnemyData, death_callback_sub);
ValidateFieldOffset32(0x55E8, EnemyData, func_set_func);
ValidateFieldOffset32(0x55EC, EnemyData, __is_func_set_2);
ValidateFieldOffset32(0x55F0, EnemyData, extra_damage_func);
ValidateFieldOffset32(0x55F4, EnemyData, extra_hitbox_func);
ValidateFieldOffset32(0x55F8, EnemyData, chapter);
ValidateFieldOffset32(0x55FC, EnemyData, chapter_spawn_weight);
ValidateStructSize32(0x5600, EnemyData);
#pragma endregion

// 0x4B36E4
static ExtraDamageFunc* EXTRA_DAMAGE_FUNC_TABLE[] = {
    NULL,
    &EnemyData::extra_damage_func1,
    &EnemyData::extra_damage_func2,
};
// 0x4CF2D8
static ExtraHitboxFunc* EXTRA_HITBOX_FUNC_TABLE[] = {
    NULL
};

#pragma region // IMPORTED_FROM_MOF_DATA_NEEDS_VALIDATION

// size: 0x10
struct EclSub {
    ZUNMagic magic; // 0x0
    uint32_t data_offset; // 0x4
    unknown_fields(0x8); // 0x8
    EclInstruction instructions[]; // 0x10
};

// size: 0x8
struct EclSubHeader {
    char* name; // 0x0
    EclSub* data; // 0x4
};

// size: 0x24
struct EclFileHeader {
    ZUNMagic magic; // 0x0
    uint16_t version; // 0x4
    uint16_t include_length; // 0x6 include_offset + ANIM+ECLI length
    uint32_t include_offset; // 0x8
    int __dword_C; // 0xC
    uint16_t sub_count; // 0x10

    int __dword_14; // 0x14
    int __dword_18; // 0x18
    int __dword_1C; // 0x1C
    int __dword_20; // 0x20
    // 0x24
};

struct EclFile {
    EclFileHeader header; // 0x0
    unsigned char data[]; // 0x24
};
#pragma endregion

struct SptResource {
    //void* vtable; // 0x0
    int32_t file_count; // 0x4
    int32_t sub_count; // 0x8
    EclFile* files[32]; // 0xC
    EclSubHeader(*subs)[]; // 0x8C

    // 0x48D9D0
    dllexport virtual gnu_noinline ZUNResult __method_0(const char* file_buffer) asm_symbol_rel(0x48D9D0) {
        this->files[this->file_count] = (EclFile*)file_buffer;
        EclFile* ecl_file = this->files[this->file_count];
        if (
            ecl_file->header.magic.as_uint == PackUInt('S', 'C', 'P', 'T') &&
            ecl_file->header.version == 1
        ) {
            char* includes = based_pointer<char>(ecl_file, ecl_file->header.include_length);
            this->sub_count += ecl_file->header.sub_count;
            EclSubHeader(*old_sub_array)[] = this->subs;
            void* file_data = ecl_file->data;
            // TODO
        }
        else {
            this->files[this->file_count] = NULL;
            return ZUN_ERROR;
        }
    }
};

// size: 0x1098
struct EnemyController {
    void* vtable; // 0x0
    int32_t file_count; // 0x4
    int32_t sub_count; // 0x8
    EclFile* files[32]; // 0xC
    EclSubHeader(*subs)[]; // 0x8C
    EclStack __wtf_stack_maybe; // 0x90
    // 0x1098

    inline void zero_contents() {
        zero_this();
    }

    inline EnemyController() {
        this->zero_contents();
    }
};

// size: 0x122C
struct EclVM {
    //void* vtable; // 0x0
    EclContext* next_context; // 0x4
    EclContext* prev_context; // 0x8
    EclContext* current_context; // 0xC
    EclContext context; // 0x10
    EnemyController* controller; // 0x1218
    ZUNList<EclContext> context_list; // 0x121C
    // 0x122C

private:
    // 0x42CDC0
    dllexport gnu_noinline void vectorcall subtract_time_float(float, float amount) asm_symbol_rel(0x42CDC0) {
        this->current_context->time -= amount;
    }
public:
    inline void subtract_time_float(float amount) {
        return this->subtract_time_float(UNUSED_FLOAT, amount);
    }

    // 0x42CE10
    dllexport gnu_noinline EclVM() {
        this->context.set_float_interp_times(0);
    }

    // 0x42CE80
    dllexport gnu_noinline void initialize_vm() asm_symbol_rel(0x42CE80) {
        this->context.__unknown_flag_A = false;
        this->context.time = 0.0f;
        this->context.location.reset();
        this->context.async_id = -1;
        this->context.vm = this;
        this->context.__int_101C = 0;
        this->context.set_float_interp_times(0);
        this->current_context = &this->context;
        this->context.stack.pointer = 0;
        this->context.stack.base = 0;
        this->context_list.initialize_with(&this->context);
    }

    // 0x42CD70
    dllexport gnu_noinline virtual int32_t high_ecl_run() asm_symbol_rel(0x42CD70) {
        return 0;
    }

    // 0x42CD80
    dllexport gnu_noinline virtual int32_t get_int_var(int32_t index) asm_symbol_rel(0x42CD80) {
        return 0;
    }

    // 0x42CD90
    dllexport gnu_noinline virtual int32_t* get_int_ptr(int32_t index) asm_symbol_rel(0x42CD90) {
        return NULL;
    }

    // 0x42CDA0
    dllexport gnu_noinline virtual float get_float_var(int32_t index) asm_symbol_rel(0x42CDA0) {
        return 0.0f;
    }

    // 0x42CDB0
    dllexport gnu_noinline virtual float* get_float_ptr(int32_t index) asm_symbol_rel(0x42CDB0) {
        return NULL;
    }

    // 0x42CDD0
    dllexport gnu_noinline void cleanup_vm() asm_symbol_rel(0x42CDD0) {
        this->context_list.as_head().delete_each();
    }

    // 0x42CF50
    dllexport inline virtual ~EclVM() asm_symbol_rel(0x42CF50) {
        clang_forceinline this->cleanup_vm();
    }

private:
    // 0x48D420
    dllexport gnu_noinline ZUNResult vectorcall run_ecl(float, float current_gamespeed) asm_symbol_rel(0x48D420) {
        BOOL is_primary_context = true;
        if (this->context_list.do_while_safe([=, &is_primary_context](EclContext* context, ZUNList<EclContext>* node) {
            this->current_context = context;
            if (is_primary_context) {
                if (ZUN_FAILED(context->low_ecl_run(current_gamespeed))) {
                    return false;
                }
                is_primary_context = false;
            } else {
                if (ZUN_FAILED(context->low_ecl_run(current_gamespeed))) {
                    delete context;
                    node->unlink();
                    delete node;
                }
            }
            return true;
        })) {
            this->current_context = &this->context;
            return ZUN_SUCCESS;
        } else {
            return ZUN_ERROR;
        }
    }

public:
    forceinline ZUNResult run_ecl(float time) {
        return this->run_ecl(UNUSED_FLOAT, time);
    }

    // 0x48D850
    dllexport gnu_noinline ZUNResult thiscall new_async(int32_t async_id, int32_t args_index) asm_symbol_rel(0x48D850) {
        EclContext* context = new EclContext();
        ZUNList<EclContext>* list_node = new ZUNList<EclContext>();
        context->async_id = async_id;
        context->vm = this;
        context->time = 0.0f;
        context->location.reset();
        context->difficulty_mask = this->current_context->difficulty_mask;
        list_node->initialize_with(context);
        this->context_list.append(list_node);
        return this->current_context->call(context, args_index);
    }

    // 0x48D920
    dllexport gnu_noinline void thiscall locate_sub(const char* sub_name) asm_symbol_rel(0x48D920) {
        EnemyController* enemy_controller = this->controller;
        int32_t left_index = 0;
        assume(sub_name[0] != '\0');
        int32_t right_index = enemy_controller->sub_count - 1;
        if (expect(right_index >= 0, true)) {
            EclSubHeader* subs = *enemy_controller->subs;
            do {
                int32_t index = right_index - left_index;
                const char* name = sub_name;
                index /= 2;
                index += left_index;
                int32_t cmp_value = strcmp_asm(name, subs[index].name);
                if (!cmp_value) {
                    this->current_context->location.sub_index = index;
                    return;
                } else if (cmp_value < 0) {
                    right_index = index - 1;
                } else {
                    left_index = index + 1;
                }
            } while (left_index <= right_index);
        }
        this->current_context->location.sub_index = -1;
    }

    // 0x48DC20
    dllexport gnu_noinline int32_t thiscall set_context_to_sub(const char* sub_name) asm_symbol_rel(0x48DC20) {
        this->locate_sub(sub_name);
        this->current_context->location.instruction_offset = 0;
        this->current_context->time = 0.0f;
        return 0;
    }

    inline void reinitialize_vm_with_sub(const char* sub_name) {
        this->cleanup_vm();
        this->initialize_vm();
        this->set_context_to_sub(sub_name);
    }
};
#pragma region // EclVM Validation
ValidateVirtualFieldOffset32(0x4, EclVM, next_context);
ValidateVirtualFieldOffset32(0x8, EclVM, prev_context);
ValidateVirtualFieldOffset32(0xC, EclVM, current_context);
ValidateVirtualFieldOffset32(0x10, EclVM, context);
ValidateVirtualFieldOffset32(0x1218, EclVM, controller);
ValidateVirtualFieldOffset32(0x121C, EclVM, context_list);
ValidateStructSize32(0x122C, EclVM);
#pragma endregion

inline ZUNResult EnemyData::run_ecl() {
    return this->vm->run_ecl(this->ecl_time.get_scale_unsafe());
}

inline void EnemyData::reinitialize_vm_with_sub(const char* sub_name) {
    this->vm->cleanup_vm();
    this->vm->initialize_vm();
    this->vm->set_context_to_sub(sub_name);
}

inline EclInstruction* EclContext::get_instruction(int32_t sub_index, int32_t instr_offset) {
    return based_pointer((*this->vm->controller->subs)[sub_index].data->instructions, instr_offset);
}

forceinline int32_t thiscall EclContext::get_int_arg(int32_t index, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        int32_t value = IntArg(index);
        if (value >= 0) {
            return this->stack.read_local(value);
        } else if (value <= -1 && value >= -100) {
            return this->stack.read_temp(value);
        } else {
            return this->vm->get_int_var(value);
        }
    } else {
        return IntArg(index);
    }
}

// 0x48D4F0
dllexport gnu_noinline int32_t thiscall EclContext::get_int_arg(int32_t index) {
    return this->get_int_arg(index, this->get_current_instruction());
}

forceinline float vectorcall EclContext::get_float_arg(int32_t index, EclInstruction* current_instruction) {
    float value = FloatArg(index);
    if (current_instruction->param_mask & (1 << index)) {
        if (value >= 0.0f) {
            return this->stack.read_local(value);
        } else if (value <= -1.0f && value >= -100.0f) {
            return this->stack.read_temp(value);
        } else {
            return this->vm->get_float_var((int32_t)value);
        }
    } else {
        return value;
    }
}

// 0x48D5A0
dllexport gnu_noinline float vectorcall EclContext::get_float_arg(int32_t, int32_t index) {
    return this->get_float_arg(index, this->get_current_instruction());
}

forceinline int32_t thiscall EclContext::get_int_arg_pop(int32_t index, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        int32_t value = IntArg(index);
        if (value >= 0) {
            return this->stack.read_local<int32_t>(value);
        } else if (value <= -1 && value >= -100) {
            return this->stack.pop_cast<int32_t>();
        } else {
            return this->vm->get_int_var(value);
        }
    } else {
        return IntArg(index);
    }
}

forceinline int32_t thiscall EclContext::get_int_arg_pop(int32_t index) {
    return this->get_int_arg_pop(index, this->get_current_instruction());
}

forceinline float vectorcall EclContext::get_float_arg_pop(int32_t index, EclInstruction* current_instruction) {
    float value = FloatArg(index);
    if (current_instruction->param_mask & (1 << index)) {
        if (value >= 0.0f) {
            return this->stack.read_local<float>(value);
        } else if (value <= -1.0f && value >= -100.0f) {
            return this->stack.pop_cast<float>();
        } else {
            return this->vm->get_float_var(value);
        }
    } else {
        return value;
    }
}

forceinline float vectorcall EclContext::get_float_arg_pop(int32_t index) {
    return this->get_float_arg_pop(index, this->get_current_instruction());
}

forceinline int32_t thiscall EclContext::parse_int_as_arg_pop(int32_t index, int32_t value, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        if (value >= 0) {
            return this->stack.read_local(value);
        } else if (value <= -1 && value >= -100) {
            return this->stack.pop_cast<int32_t>();
        } else {
            return this->vm->get_int_var(value);
        }
    } else {
        return value;
    }
}

// 0x48D690
dllexport gnu_noinline int32_t thiscall EclContext::parse_int_as_arg_pop(int32_t index, int32_t value) {
    return this->parse_int_as_arg_pop(index, value, this->get_current_instruction());
}

forceinline float vectorcall EclContext::parse_float_as_arg_pop(int32_t index, float value, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        if (value >= 0.0f) {
            return this->stack.read_local(value);
        } else if (value <= -1.0f && value >= -100.0f) {
            return this->stack.pop_cast<float>();
        } else {
            return this->vm->get_float_var(value);
        }
    } else {
        return value;
    }
}

inline float vectorcall EclContext::parse_float_as_arg_pop(int32_t index, float value) {
    return this->parse_float_as_arg_pop(index, value, this->get_current_instruction());
}

forceinline int32_t* thiscall EclContext::get_int_ptr_arg(int32_t index, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        int32_t value = IntArg(index);
        if (value >= 0) {
            return &this->stack.ref_local(value);
        } else {
            return this->vm->get_int_ptr(value);
        }
    } else {
        return NULL;
    }
}

// 0x48D750
dllexport gnu_noinline int32_t* thiscall EclContext::get_int_ptr_arg(int32_t index) {
    return this->get_int_ptr_arg(index, this->get_current_instruction());
}

forceinline float* thiscall EclContext::get_float_ptr_arg(int32_t index, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        float value = FloatArg(index);
        if (value >= 0.0f) {
            return &this->stack.ref_local(value);
        } else {
            return this->vm->get_float_ptr((int32_t)value);
        }
    } else {
        return NULL;
    }
}

// 0x48D7C0
dllexport gnu_noinline float* thiscall EclContext::get_float_ptr_arg(int32_t index) {
    return this->get_float_ptr_arg(index, this->get_current_instruction());
}

forceinline int32_t thiscall EclContext::parse_int_as_arg(int32_t index, int32_t value, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        if (value >= 0) {
            return this->stack.read_local(value);
        } else if (value <= -1 && value >= -100) {
            return this->stack.read_temp(value);
        } else {
            return this->vm->get_int_var(value);
        }
    } else {
        return IntArg(index);
    }
}

forceinline int32_t thiscall EclContext::parse_int_as_arg(int32_t index, int32_t value) {
    return this->parse_int_as_arg(index, value, this->get_current_instruction());
}

forceinline float EclContext::parse_float_as_arg(int32_t index, float value, EclInstruction* current_instruction) {
    if (current_instruction->param_mask & (1 << index)) {
        if (value >= 0.0f) {
            return this->stack.read_local(value);
        } else if (value <= -1.0f && value >= -100.0f) {
            return this->stack.read_temp(value);
        } else {
            return this->vm->get_float_var((int32_t)value);
        }
    } else {
        return value;
    }
}

forceinline float EclContext::parse_float_as_arg(int32_t index, float value) {
    return this->parse_float_as_arg(index, value, this->get_current_instruction());
}

// 0x438AA0
dllexport gnu_noinline int32_t thiscall EnemyData::get_int_arg(int32_t index) {
    return this->vm->current_context->get_int_arg(index);
}

// 0x438AC0
dllexport gnu_noinline int32_t* thiscall EnemyData::get_int_ptr_arg(int32_t index) {
    return this->vm->current_context->get_int_ptr_arg(index);
}

// 0x438AD0
dllexport gnu_noinline float vectorcall EnemyData::get_float_arg(int32_t, int32_t index) {
    return this->vm->current_context->get_float_arg(index);
}

// 0x438AF0
dllexport gnu_noinline float* thiscall EnemyData::get_float_ptr_arg(int32_t index) {
    return this->vm->current_context->get_float_ptr_arg(index);
}

// 0x438B10
dllexport gnu_noinline int32_t thiscall EnemyData::parse_int_as_arg(int32_t index, int32_t value) {
    return this->vm->current_context->parse_int_as_arg(index, value);
}

// 0x438BC0
dllexport gnu_noinline float vectorcall EnemyData::parse_float_as_arg(int32_t, int32_t index, float, float, float value) {
    return this->vm->current_context->parse_float_as_arg(index, value);
}

namespace Ecl {
enum Var : int32_t {
    RAND_INT = -10000,
    RAND_FLOAT = -9999,
    RAND_ANGLE = -9998,
    SELF_X = -9997,
    SELF_Y = -9996,
    SELF_X_ABS = -9995,
    SELF_Y_ABS = -9994,
    SELF_X_REL = -9993,
    SELF_Y_REL = -9992,

    SELF_X2 = -9977,
    SELF_Y2 = -9976,
    SELF_X2_ABS = -9975,
    SELF_Y2_ABS = -9974,
    SELF_X2_REL = -9973,
    SELF_Y2_REL = -9972
};
enum Opcode : uint16_t {
    // Section A
    nop = 0,
    enemy_delete = 1,
    ret = 10,
    call = 11,
    jump = 12,
    jump_equ = 13,
    jump_neq = 14,
    async_call = 15,
    async_call_id = 16,
    async_stop_id = 17,
    __async_unknown_flag_set = 18,
    __async_unknown_flag_clear = 19,
    __async_unknown_value = 20,
    async_stop_all = 21,
    __debug_unknown_A = 22,
    ecl_time_sub = 23,
    ecl_time_sub_float = 24,
    debug_print = 30,
    __debug_unknown_B = 31,
    frame_enter = 40,
    frame_leave = 41,
    push_int = 42,
    pop_int = 43,
    push_float = 44,
    pop_float = 45,
    math_int_add = 50,
    math_float_add = 51,
    math_int_sub = 52,
    math_float_sub = 53,
    math_int_mul = 54,
    math_float_mul = 55,
    math_int_div = 56,
    math_float_div = 57,
    math_int_mod = 58,
    cmp_int_equ = 59,
    cmp_float_equ = 60,
    cmp_int_neq = 61,
    cmp_float_neq = 62,
    cmp_int_les = 63,
    cmp_float_les = 64,
    cmp_int_leq = 65,
    cmp_float_leq = 66,
    cmp_int_gre = 67,
    cmp_float_gre = 68,
    cmp_int_geq = 69,
    cmp_float_geq = 70,
    cmp_int_not = 71,
    cmp_float_not = 72,
    cmp_or = 73,
    cmp_and = 74,
    math_bit_xor = 75,
    math_bit_or = 76,
    math_bit_and = 77,
    math_post_dec = 78,
    math_sin = 79,
    math_cos = 80,
    math_circle_pos = 81,
    math_reduce_angle = 82,
    math_int_neg = 83,
    math_float_neg = 84,
    math_hypot_squared = 85,
    math_hypot = 86,
    math_line_angle = 87,
    math_sqrt = 88,
    math_angle_diff = 89,
    math_point_rotate = 90,
    math_float_interp = 91,
    math_float_interp_bezier = 92,
    math_circle_pos_rand = 93,
    math_ellipse_pos = 94,
    __math_angle_95,
    __math_angle_96,
    __math_angle_97,

    // Section B
    enemy_create_rel = 300,
    enemy_create_abs,
    anm_source,
    anm_set_slot,
    enemy_create_rel_mirror,
    enemy_create_abs_mirror,
    anm_set_slot_main,
    anm_create_front,
    anm_create_zero_front,
    enemy_create_rel_stage,
    enemy_create_abs_stage,
    enemy_create_rel_stage_mirror,
    enemy_create_abs_stage_mirror,
    anm_play_attack,
    anm_create_back,
    anm_create_front_rotated,
    anm_play_attack_ex,
    anm_interrupt_slot,
    anm_play_main,
    anm_rotate_slot,
    anm_move_position_slot,
    __enemy_create_rel_2,
    __anm_set_slot_anchor_index,
    anm_death_effects,
    enemy_id_get_position,
    anm_color_slot,
    anm_color_slot_interp,
    anm_alpha_slot,
    anm_alpha_slot_interp,
    anm_scale_slot,
    anm_scale_slot_interp,
    anm_alpha2_slot,
    anm_alpha2_slot_interp,
    anm_move_position_slot_interp,
    __effect_create_special,
    anm_scale2_slot,
    __anm_layer_slot,
    anm_blend_mode_slot,
    anm_create_rel_front_rotated,
    __anm_create_zero_front_and_run,
    __enemy_id_set_flag_unknown_B,

    // Section C
    move_position_abs = 400,
    move_position_abs_interp,
    move_position_rel,
    move_position_rel_interp,
    move_velocity_abs,
    move_velocity_abs_interp,
    move_velocity_rel,
    move_velocity_rel_interp,
    move_orbit_abs,
    move_orbit_abs_interp,
    move_orbit_rel,
    move_orbit_rel_interp,
    move_rand_interp_abs,
    move_rand_interp_rel,
    move_to_boss0_abs,
    move_to_boss0_rel,
    move_position_add_abs,
    move_position_add_rel,
    move_origin_abs,
    move_origin_rel,
    move_ellipse_abs,
    move_ellipse_abs_interp,
    move_ellipse_rel,
    move_ellipse_rel_interp,
    enemy_flag_mirror,
    move_bezier_abs,
    move_bezier_rel,
    move_stop,
    move_velocity_no_mirror_abs,
    move_velocity_no_mirror_abs_interp,
    move_velocity_no_mirror_rel,
    move_velocity_no_mirror_rel_interp,
    move_to_enemy_id_abs,
    move_to_enemy_id_rel,
    move_axis_interp_abs,
    move_axis_interp_rel,
    __move_position_offset_abs_interp,
    __move_position_offset_rel_interp,
    __move_axis_offset_abs_interp,
    __move_axis_offset_rel_interp,
    move_angle_abs,
    move_angle_abs_interp,
    move_angle_rel,
    move_angle_rel_interp,
    move_speed_abs,
    move_speed_abs_interp,
    move_speed_rel,
    move_speed_rel_interp,

    // Section D
    enemy_set_hitbox = 500,
    enemy_set_collision,
    enemy_flags_set,
    enemy_flags_clear,
    move_bounds_set,
    move_bounds_disable,
    item_bonus_slots_reset,
    item_bonus_slot_set,
    item_drop_area,
    drop_item_rewards,
    item_reward_set,
    enemy_life_set,
    boss_set,
    boss_timer_clear,
    callback_ex,
    enemy_invincible_timer,
    effect_sound,
    effect_screen_shake,
    msg_read,
    msg_wait,
    boss_wait,
    timer_callback_sub,
    spellcard_start,
    spellcard_end,
    chapter_set,
    enemy_kill_all,
    player_protect_range,
    enemy_lifebar_color,
    spellcard_start_2,
    set_float_rank_3,
    set_float_rank_5,
    math_float_rank_lerp,
    set_int_rank_3,
    set_int_rank_5,
    math_int_rank_lerp,
    set_int_difficulty,
    set_float_difficulty,
    spellcard_start_difficulty,
    spellcard_start_difficulty_1,
    spellcard_start_difficulty_2,
    boss_set_life_count,
    enemy_no_collision_timer,
    spellcard_flag_timeout_set,
    __spellcard_flag_anm_unknown,
    enemy_flag_homing_disable,
    laser_clear_all,
    enemy_bomb_shield,
    game_speed_set,
    ecl_time_sub_difficulty,
    __enemy_flag_unknown_A,
    enemy_set_kill_id,
    enemy_kill_all_id,
    anm_layer_base,
    enemy_damage_sound,
    __stage_logo,
    enemy_id_exists,
    death_callback_sub,
    std_fog_interp,
    enemy_flag_mirror2,
    enemy_limit_set,
    bullet_bounce_bounds_set,
    __enemy_kill_effect_create,
    drop_item_rewards_force,
    enemy_flag_hitbox_shape,
    enemy_set_hitbox_rotation,
    enemy_bomb_shield_multiplier,
    enemy_kill,
    __spellcard_flag_unknown_A,
    __enemy_flag_armored,
    __enemy_set_chapter_spawn_weight,
    __enemy_add_spawn_weight_to_destroy,
    enemy_kill_all_no_callbacks,
    __enemy_life_set_current,
    item_timed_bonus_slot_set,
    item_timed_bonus_duration,

    // Section E
    shooter_reset = 600,
    shoot_now,
    bullet_sprite,
    shoot_offset,
    shoot_angle,
    bullet_speed,
    bullet_count,
    shoot_aim_mode,
    bullet_sound,
    bullet_effects,
    bullet_effects_ex,
    bullet_effects_add,
    bullet_effects_add_ex,
    bullet_cancel,
    shooter_copy,
    bullet_cancel_radius,
    bullet_clear_radius,
    bullet_speed_rank_3,
    bullet_speed_rank_5,
    bullet_speed_rank_lerp,
    bullet_count_rank_3,
    bullet_count_rank_5,
    bullet_count_rank_lerp,
    set_float_angle_to_player_from_point,
    bullet_speed_difficulty,
    bullet_count_difficulty,
    shoot_offset_circle,
    shoot_spawn_distance,
    __shoot_origin,
    enemy_fog_spawn,
    std_interrupt,
    __enemy_manager_flag_unknown_A,
    ex_ins_repeat,
    enemy_damage_ex,
    enemy_hitbox_ex,
    bullet_cancel_weak_radius,
    bullet_clear_weak_radius,
    ex_ins_call,
    score_add,
    __ex_ins_repeat2,
    bullet_effects_set_string,
    __bullet_effects_prev,

    // Section F
    laser_size_data = 700,
    laser_timing_data,
    laser_line_create,
    laser_infinite_create,
    laser_offset,
    laser_target,
    laser_speed,
    laser_width,
    laser_angle,
    laser_rotate,
    laser_clear,
    laser_curve_create,
    __bullet_cancel_weak_rectangle,
    laser_beam_create,
    __laser_beam_flag_unknown_A,

    // Section G
    enemy_id_change_sub = 800,
    enemy_id_get_position_crash,
    boss_callback,

    // Section H
    __debug_unknown_C = 901,
    __debug_unknown_D = 902,

    // Section I
    __globals_flag_unknown_A = 1001
};
}

using EnemyOnKillFunc = void fastcall (Enemy*);

// size: 0x683C
struct Enemy : EclVM {
    //EclVM vm; // 0x0
    EnemyData data; // 0x122C
    EnemyOnKillFunc* __on_kill_func; // 0x682C
    EnemyID id; // 0x6830
    EnemyID parent_id; // 0x6834
    int __dword_6838; // 0x6838
    // 0x683C

    // 0x42E100
    dllexport gnu_noinline Enemy(const char* sub_name);

    // 0x42D200
    dllexport gnu_noinline void __set_unknown_flag_B() asm_symbol_rel(0x42D200) {
        this->data.__unknown_flag_B = true;
    }

    // 0x42D210
    dllexport gnu_noinline EnemyData& get_data() asm_symbol_rel(0x42D210) {
        return this->data;
    }

    // 0x42D220
    dllexport gnu_noinline ~Enemy() asm_symbol_rel(0x42D220);

    // 0x42FE80
    dllexport gnu_noinline void thiscall on_tick() asm_symbol_rel(0x42FE80) {
        float enemy_slowdown = this->data.slowdown;
        if (enemy_slowdown <= 0.0f) {
            if (this->data.__anm_slowdown_immune) {
                this->data.set_anm_vm_slowdowns(0.0f);
            }
            this->data.on_tick();
        } else {
            float previous_gamespeed = GAME_SPEED.value;
            GAME_SPEED.value = confine_to_range(0.0f, previous_gamespeed - enemy_slowdown * previous_gamespeed, 1.0f);
            this->data.set_anm_vm_slowdowns(this->data.slowdown);
            this->data.on_tick();
            GAME_SPEED.value = previous_gamespeed;
            this->data.__anm_slowdown_immune = true; // Why?
        }
    }

    // 0x4389F0
    dllexport gnu_noinline void thiscall set_callback(int32_t index, int32_t life, int32_t time, const char* sub_name) asm_symbol_rel(0x4389F0) {
        EnemyCallback& callback = this->data.callbacks[index];
        callback.life = life;
        if (life >= 0) {
            callback.time = time;
            if (sub_name) {
                byteloop_strcpy(callback.life_sub, sub_name);
                byteloop_strcpy(callback.time_sub, sub_name);
            } else {
                callback.life_sub[0] = '\0';
                callback.time_sub[0] = '\0';
            }
        }
    }

    inline void set_callback_life_sub_name(int32_t index, const char* sub_name) {
        EnemyCallback& callback = this->data.callbacks[index];
        if (sub_name) {
            byteloop_strcpy(callback.life_sub, sub_name);
        } else {
            callback.life_sub[0] = '\0';
        }
    }

    // 0x438A60
    dllexport gnu_noinline void thiscall set_callback_time_sub_name(int32_t index, const char* sub_name) asm_symbol_rel(0x438A60) {
        EnemyCallback& callback = this->data.callbacks[index];
        if (sub_name) {
            byteloop_strcpy(callback.time_sub, sub_name);
        } else {
            callback.time_sub[0] = '\0';
        }
    }

    inline const char* check_timer_callbacks();

    inline const char* check_life_callbacks();

    // 0x409980
    dllexport gnu_noinline MotionData* thiscall get_current_motion() {
        return &this->data.current_motion;
    }

    // 0x430D30
    dllexport gnu_noinline virtual int32_t high_ecl_run() asm_symbol_rel(0x430D30) {
        return this->data.high_ecl_run();
    }

    // 0x437360
    dllexport gnu_noinline virtual int32_t get_int_var(int32_t index) asm_symbol_rel(0x437360) {
        switch (index) {

            default:
                return 0;
        }
    }

    // 0x437B20
    dllexport gnu_noinline virtual int32_t* get_int_ptr(int32_t index) asm_symbol_rel(0x437B20) {
        switch (index) {

            default:
                return NULL;
        }
    }

    // 0x437C90
    dllexport gnu_noinline virtual float get_float_var(int32_t index) asm_symbol_rel(0x437C90) {
        switch (index) {

            default:
                return 0.0f;
        }
    }

    // 0x4387E0
    dllexport gnu_noinline virtual float* get_float_ptr(int32_t index) asm_symbol_rel(0x4387E0) {
        switch (index) {

            default:
                return NULL;
        }
    }

    // 0x430240
    dllexport gnu_noinline int thiscall kill() asm_symbol_rel(0x430240);

protected:
    template <bool no_callbacks = false>
    forceinline void try_kill_success() {
        this->data.drops.reset();
        this->data.position_of_last_damage_source_to_hit.x = 0.0f;
        this->data.position_of_last_damage_source_to_hit.y = 192.0f;
        this->data.chapter_spawn_weight = 0;
        if constexpr (no_callbacks) {
            this->data.death_callback_sub[0] = '\0';
        }
        this->kill();
        nounroll for (size_t i = 0; i < 16; ++i) {
            this->data.anm_vms[i].mark_tree_for_delete();
        }
        this->data.__unknown_flag_B = true;
    }

public:
    template <bool no_callbacks = false>
    forceinline void try_kill() {
        if (this->data.flags_allow_kill()) {
            this->try_kill_success<no_callbacks>();
        }
    }

    template <bool no_callbacks = false>
    forceinline void try_kill_by_kill_id(int32_t kill_id) {
        if (
            this->data.flags_allow_kill() &&
            this->data.kill_id == kill_id
        ) {
            this->try_kill_success<no_callbacks>();
        }
    }
};
#pragma region // Enemy Validation
//ValidateFieldOffset32(0x0, Enemy, vm);
ValidateFieldOffset32(0x122C, Enemy, data);
ValidateFieldOffset32(0x682C, Enemy, __on_kill_func);
ValidateFieldOffset32(0x6830, Enemy, id);
ValidateFieldOffset32(0x6834, Enemy, parent_id);
ValidateFieldOffset32(0x6838, Enemy, __dword_6838);
ValidateStructSize32(0x683C, Enemy);
#pragma endregion

// size: 0xC
struct RGB {
    int32_t r; // 0x0
    int32_t g; // 0x4
    int32_t b; // 0x8
    // 0xC
};

typedef struct Gui Gui;
extern "C" {
    // 0x4CF2E0
    extern Gui* GUI_PTR asm("_GUI_PTR");
    // 0x570918
    extern void* CACHED_MSG_FILE_PTR asm("_CACHED_MSG_FILE_PTR");
}

static inline constexpr int32_t MENU_STACK_DEPTH = 16;

// size: 0xD8
struct MenuSelect {
    int32_t current_selection; // 0x0
    int32_t previous_selection; // 0x4
    int32_t menu_length; // 0x8
    int32_t selection_stack[MENU_STACK_DEPTH]; // 0xC
    int32_t menu_length_stack[MENU_STACK_DEPTH]; // 0x4C
    int32_t stack_index; // 0x8C
    int32_t disabled_selections[MENU_STACK_DEPTH]; // 0x90
    BOOL enable_wrap; // 0xD0
    int32_t disabled_selections_count; // 0xD4
    // 0xD8

    // 0x402940
    dllexport gnu_noinline void thiscall push_state() asm_symbol_rel(0x402940) {
        this->selection_stack[this->stack_index] = this->current_selection;
        this->menu_length_stack[this->stack_index] = this->menu_length;
        ++this->stack_index;
        if (this->stack_index >= MENU_STACK_DEPTH) {
            this->stack_index = MENU_STACK_DEPTH - 1;
        }
        this->disabled_selections_count = 0;
    }

    // 0x402990
    dllexport gnu_noinline void thiscall pop_state() asm_symbol_rel(0x402990) {
        if (--this->stack_index < 0) {
            this->stack_index = 0;
            this->current_selection = 0;
            this->menu_length = 0;
            this->disabled_selections_count = 0;
        }
        else {
            this->current_selection = this->selection_stack[this->stack_index];
            this->menu_length = this->menu_length_stack[this->stack_index];
            this->disabled_selections_count = 0;
        }
    }

    // 0x4029E0
    dllexport gnu_noinline int32_t thiscall __move_selection(int32_t offset) asm_symbol_rel(0x4029E0) {
        int32_t selection = this->current_selection;
        int32_t menu_length = this->menu_length;

        if (menu_length > 0) {
            int32_t disabled_count = this->disabled_selections_count;

        retry_selection:
            selection += offset;
            this->current_selection = selection;
            if (selection >= menu_length) {
                BOOL wrap = this->enable_wrap;
                do {
                    if (wrap) {
                        selection -= menu_length;
                    } else {
                        --selection;
                    }
                    this->current_selection = selection;
                } while (selection >= menu_length);
            }
            if (selection < 0) {
                BOOL wrap = this->enable_wrap;
                do {
                    if (wrap) {
                        selection += menu_length;
                    } else {
                        selection = 0;
                    }
                    this->current_selection = selection;
                } while (selection < 0);
            }

            for (int32_t i = 0; i < disabled_count; ++i) {
                if (this->current_selection == this->disabled_selections[i]) {
                    goto retry_selection;
                }
            }
        }

        return selection;
    }
};
#pragma region // MenuSelect Verification
ValidateFieldOffset32(0x0, MenuSelect, current_selection);
ValidateFieldOffset32(0x4, MenuSelect, previous_selection);
ValidateFieldOffset32(0x8, MenuSelect, menu_length);
ValidateFieldOffset32(0xC, MenuSelect, selection_stack);
ValidateFieldOffset32(0x4C, MenuSelect, menu_length_stack);
ValidateFieldOffset32(0x8C, MenuSelect, stack_index);
ValidateFieldOffset32(0x90, MenuSelect, disabled_selections);
ValidateFieldOffset32(0xD0, MenuSelect, enable_wrap);
ValidateFieldOffset32(0xD4, MenuSelect, disabled_selections_count);
ValidateStructSize32(0xD8, MenuSelect);
#pragma endregion

// size: 0x4
struct MsgInstruction {
    uint16_t time; // 0x0
    uint8_t opcode; // 0x2
    uint8_t args_size; // 0x3
    unsigned char args[]; // 0x4
};

namespace Msg {
enum Opcode : uint8_t {
    msg_delete = 0,
    initialize_player = 1,
    initialize_boss = 2,
    initialize_textbox = 3,
    delete_player = 4,
    delete_boss = 5,
    delete_textbox = 6,
    focus_player = 7,
    focus_boss = 8,
    focus_none = 9,
    msg_flag_wait_skippable = 10,
    wait = 11,
    ecl_resume = 12,
    portrait_player = 13,
    portrait_boss = 14,
    text_top_line = 15,
    text_bottom_line = 16,
    text_dialogue = 17,
    text_clear = 18,
    music_boss = 19,
    text_intro = 20,
    stage_end = 21,
    music_fade_out = 22,
    player_shake = 23,
    boss_shake = 24,
    text_offset = 25,
    __unknown_flag_set_A = 26,
    music_fade_out_time = 27,
    text_position = 28,
    text_type = 29,
    
};
}

// 0x570DA0
static char MSG_DECRYPT_BUFFER[0x100];

// 0x4419F0
dllexport gnu_noinline const char* fastcall __decrypt_related(const char* str) {
    uint8_t xor_mask = 0x77;
    uint8_t xor_accel = 7;

    char* buffer_write = MSG_DECRYPT_BUFFER;
    uint8_t c;
    do {
        c = *str++;
        c ^= xor_mask;
        xor_mask += xor_accel;
        xor_accel += 16;
        *buffer_write++ = c;
    } while (c);

    char* buffer_read = MSG_DECRYPT_BUFFER;

    while ((c = *buffer_read)) {
        if (
            (c <= 0x80 || 0xA0 <= c) &&
            (c + ' ' > 0x1C)
        ) {
            if (c == '_') {
                *buffer_read = ' ';
            }
        } else {
            ++buffer_read;
        }
        ++buffer_read;
    }

    return MSG_DECRYPT_BUFFER;
}

// 0x4B6620
static int32_t PLAYER_PORTRAIT_SCRIPT_TABLE[] = {
    40, 46, 29, 28
};

// size: 0x1D8
struct MsgVM {
    unknown_fields(0x4); // 0x0
    Timer __timer_4; // 0x4
    Timer script_time; // 0x18
    Timer pause_timer; // 0x2C
    AnmID player_portraits[2]; // 0x40
    AnmID enemy_portraits[4]; // 0x48
    AnmID __vm_id_58; // 0x58
    int __dword_5C; // 0x5C
    unknown_fields(0x4); // 0x60
    AnmID dialogue_lines[2]; // 0x64
    AnmID furigana_lines[2]; // 0x6C
    AnmID intro; // 0x74
    AnmID __textbox_related; // 0x78
    AnmID __vm_id_7C; // 0x7C
    int32_t menu_time; // 0x80
    int32_t menu_state; // 0x84
    MenuSelect menu_controller; // 0x88
    int __dword_160; // 0x160
    MsgInstruction* current_instr; // 0x164
    Float3 __float3_168; // 0x168
    Float3 __float3_174; // 0x174
    Float3 __float3_180; // 0x180
    Float3 __float3_18C; // 0x18C
    int32_t __enemy_appear_counter; // 0x198
    union {
        uint32_t flags; // 0x19C
        struct {
            uint32_t __unknown_flag_B : 1; // 1
            uint32_t __unknown_flag_C : 1; // 1
            uint32_t : 4; // 3-6
            uint32_t __unknown_flag_A : 1; // 7
        };
    };
    int32_t next_text_line; // 0x1A0
    int __dword_1A4; // 0x1A4
    int32_t __int_1A8; // 0x1A8
    int32_t active_portait; // 0x1AC
    D3DCOLOR __color_array_1B0[1]; // 0x1B0
    int __dword_1B4; // 0x1B4
    int __dword_1B8; // 0x1B8
    int __dword_1BC; // 0x1BC
    Float3 callout_position; // 0x1C0
    float __float_1CC; // 0x1CC
    unknown_fields(0x4); // 0x1D0
    int32_t __int_1D4; // 0x1D4
    // 0x1D8
    
private:
    // 0x4416D0
    dllexport gnu_noinline void vectorcall __sub_4416D0(int, float, float arg1, float arg2, float arg3, int arg4) asm_symbol_rel(0x4416D0) {
        this->__textbox_related.mark_tree_for_delete();
        // TODO
    }
public:
    inline void __sub_4416D0(float arg1, float arg2, float arg3, int arg4) {
        return this->__sub_4416D0(UNUSED_DWORD, UNUSED_FLOAT, arg1, arg2, arg3, arg4);
    }

    // 0x43E550
    dllexport gnu_noinline int thiscall run_msg() {
        using namespace Msg;

        if (this->__enemy_appear_counter > 0) {
            --this->__enemy_appear_counter;
        }
        if (this->__int_1A8 > 0) {
            --this->__int_1A8;
        } else {
            if (INPUT_STATES[0].check_inputs_no_repeat(BUTTON_SHOOT | BUTTON_SKIP)) {
                this->__unknown_flag_A = true;
            }
        }
        if (
            (this->__unknown_flag_A & this->__unknown_flag_B) &&
            (
                (INPUT_STATES[0].check_inputs(BUTTON_SKIP) && INPUT_STATES[0].inputs_held[BUTTON_SKIP_INDEX] >= 20) ||
                (INPUT_STATES[0].check_inputs(BUTTON_SHOOT) && INPUT_STATES[0].inputs_held[BUTTON_SHOOT_INDEX] >= 20)
            )
        ) {
            this->script_time.set(this->current_instr->time);
        }

        for (
            MsgInstruction* current_instruction = this->current_instr;
            this->script_time >= current_instruction->time;
            this->current_instr = IndexInstr(sizeof(MsgInstruction) + this->current_instr->args_size)
        ) {
            switch (current_instruction->opcode) {
                case __unknown_flag_set_A: // 36
                    this->__unknown_flag_C = true;
                    break;
                case text_top_line: { // 15
                    AnmVM* vm = this->dialogue_lines[0].get_vm_ptr();
                    const char* text = __decrypt_related(StringArg(0));
                    // ANM_MANAGER_PTR->draw_text_left(vm, this->__color_array_1B0[this->active_portait], 0, this->__unknown_flag_C + 4, 0, 0, text);
                    break;
                }
                case text_bottom_line: { // 16
                    AnmVM* vm = this->dialogue_lines[1].get_vm_ptr();
                    const char* text = __decrypt_related(StringArg(0));
                    // ANM_MANAGER_PTR->draw_text_left(vm, this->__color_array_1B0[this->active_portait], 0, this->__unknown_flag_C + 4, 0, 0, text);
                    break;
                }
                case text_position: // 28
                    this->callout_position.x = FloatArg(0) * 2;
                    this->callout_position.y = FloatArg(1) * 2;
                    break;
                case text_dialogue: { // 17
                    // TODO: A disgusting pile of inlined ANM code
                    break;
                }
                case text_clear: // 18
                    this->__textbox_related.mark_tree_for_delete();
                    this->dialogue_lines[0].interrupt_tree(3);
                    this->dialogue_lines[1].interrupt_tree(3);
                    this->furigana_lines[0].interrupt_tree(3);
                    this->furigana_lines[1].interrupt_tree(3);
                    break;
                case initialize_player: { // 1
                    int32_t who = IntArg(0);
                    if (who == 0) {

                    }
                    else {

                    }
                }
            }
        }
        this->script_time++;
        if (AnmVM* vm = this->__textbox_related.__wtf_child_list_jank_A(this->__int_1D4 + 170, 0)) {
            // TODO
        }
        return 0;
    }
};
#pragma region // MsgVM Validation
ValidateFieldOffset32(0x4, MsgVM, __timer_4);
ValidateFieldOffset32(0x18, MsgVM, script_time);
ValidateFieldOffset32(0x2C, MsgVM, pause_timer);
ValidateFieldOffset32(0x40, MsgVM, player_portraits);
ValidateFieldOffset32(0x48, MsgVM, enemy_portraits);
ValidateFieldOffset32(0x58, MsgVM, __vm_id_58);
ValidateFieldOffset32(0x5C, MsgVM, __dword_5C);
ValidateFieldOffset32(0x64, MsgVM, dialogue_lines);
ValidateFieldOffset32(0x6C, MsgVM, furigana_lines);
ValidateFieldOffset32(0x74, MsgVM, intro);
ValidateFieldOffset32(0x78, MsgVM, __textbox_related);
ValidateFieldOffset32(0x7C, MsgVM, __vm_id_7C);
ValidateFieldOffset32(0x80, MsgVM, menu_time);
ValidateFieldOffset32(0x84, MsgVM, menu_state);
ValidateFieldOffset32(0x88, MsgVM, menu_controller);
ValidateFieldOffset32(0x160, MsgVM, __dword_160);
ValidateFieldOffset32(0x164, MsgVM, current_instr);
ValidateFieldOffset32(0x168, MsgVM, __float3_168);
ValidateFieldOffset32(0x174, MsgVM, __float3_174);
ValidateFieldOffset32(0x180, MsgVM, __float3_180);
ValidateFieldOffset32(0x18C, MsgVM, __float3_18C);
ValidateFieldOffset32(0x198, MsgVM, __enemy_appear_counter);
ValidateFieldOffset32(0x19C, MsgVM, flags);
ValidateFieldOffset32(0x1A0, MsgVM, next_text_line);
ValidateFieldOffset32(0x1A4, MsgVM, __dword_1A4);
ValidateFieldOffset32(0x1A8, MsgVM, __int_1A8);
ValidateFieldOffset32(0x1AC, MsgVM, active_portait);
ValidateFieldOffset32(0x1B0, MsgVM, __color_array_1B0);
ValidateFieldOffset32(0x1B4, MsgVM, __dword_1B4);
ValidateFieldOffset32(0x1B8, MsgVM, __dword_1B8);
ValidateFieldOffset32(0x1BC, MsgVM, __dword_1BC);
ValidateFieldOffset32(0x1C0, MsgVM, callout_position);
ValidateFieldOffset32(0x1CC, MsgVM, __float_1CC);
ValidateFieldOffset32(0x1D4, MsgVM, __int_1D4);
ValidateStructSize32(0x1D8, MsgVM);
#pragma endregion

#pragma region IMPORTED_FROM_VD_DATA_NEEDS_VALIDATION
// size: 0x8
struct LifebarMarker {
    float bar_position; // 0x0
    D3DCOLOR section_color; // 0x4
    // 0x8
};
// size: 0x54
struct Lifebar {
    float bar_value; // 0x0
    float life_percentage; // 0x4
    int32_t current_life; // 0x8
    unknown_fields(0x4); // 0xC
    LifebarMarker markers[4]; // 0x10
    AnmID main_vm; // 0x30
    AnmID glowA_vm; // 0x34
    AnmID glowB_vm; // 0x38
    AnmID marker_vms[4]; // 0x3C
    int32_t vms_initialized; // 0x4C
    int __dword_50; // 0x50
    // 0x54
};
#pragma endregion

// size: 0x2CC
struct Gui : ZUNTask {
    // ZUNTask base; // 0x0
    unknown_fields(0x5C); // 0xC
    AnmVM* __anm_vm_ptr_array_68[7]; // 0x68
    AnmVM* __anm_vm_84; // 0x84
    AnmVM* __anm_vm_88; // 0x88
    AnmID __anm_id_8C; // 0x8C
    unknown_fields(0x28); // 0x90
    AnmID __anm_id_B8; // 0xB8
    AnmID __anm_id_BC; // 0xBC
    unknown_fields(0x50); // 0xC0
    AnmID __anm_id_110; // 0x110
    unknown_fields(0x20); // 0x104
    int __int_134; // 0x134
    unknown_fields(0x4); // 0x138
    int32_t __int_13C; // 0x13C
    Timer __timer_140; // 0x140
    unknown_fields(0x4); // 0x154
    int32_t __score; // 0x158
    int32_t __int_15C; // 0x15C
    AnmLoaded* stage_logo_anm; // 0x160
    unknown_fields(0xC); // 0x164
    int __dword_170; // 0x170
    unknown_fields(0x20); // 0x174
    union {
        uint32_t flags; // 0x194
        struct {
            uint32_t : 8; // 1-8
            uint32_t __unknown_flag_A : 1; // 9
        };
    };
    Timer __timer_198; // 0x198
    unknown_fields(0x4); // 0x1AC
    MsgVM* msg_vm; // 0x1B0
    void* msg_file; // 0x1B4
    int32_t spell_timer_seconds; // 0x1B8
    int32_t spell_timer_hundredths; // 0x1BC
    int32_t __int_1C0; // 0x1C0
    Lifebar lifebars[3]; // 0x1C4
    AnmLoaded* __anm_loaded_2C0; // 0x2C0
    unknown_fields(0x8); // 0x2C4
    // 0x2CC

    inline void update_spell_timer(int32_t time) {
        // I'm only ~80% sure I got this math right
        int32_t seconds = time / 60;
        int32_t frames = time % 60;

        int32_t hundredths = frames * 100;
        hundredths += (hundredths / 60);

        this->spell_timer_seconds = seconds > 99 ? seconds : 99;
        this->spell_timer_hundredths = seconds > 99 ? hundredths : 99; // Is this a bug?
    }

    // 0x4420E0
    dllexport gnu_noinline void thiscall __update_bomb_ui(int32_t bomb_count, int32_t bomb_fragments, int32_t bomb_max) asm_symbol_rel(0x4420E0);

    // 0x42D560
    dllexport gnu_noinline void __set_field_170(int value) asm_symbol_rel(0x42D560) {
        this->__dword_170 = value;
    }

    // 0x42D570
    dllexport gnu_noinline void vectorcall set_lifebar_marker(int32_t bar_index, int32_t marker_index, D3DCOLOR color, float position) asm_symbol_rel(0x42D570) {
        Gui* gui = GUI_PTR;
        gui->lifebars[bar_index].markers[marker_index].bar_position = position;
        gui->lifebars[bar_index].markers[marker_index].section_color = color;
    }

    // 0x42D5B0
    dllexport gnu_noinline int32_t __get_enemy_appear_counter() asm_symbol_rel(0x42D5B0) {
        return GUI_PTR->msg_vm->__enemy_appear_counter;
    }

    // 0x43A730
    dllexport gnu_noinline ZUNResult thiscall __sub_43A730() asm_symbol_rel(0x43A730);

    // 0x407D60
    dllexport bool thiscall msg_vm_active() {
        return this->msg_vm;
    }

    static inline bool msg_is_active() {
        if (Gui* gui = GUI_PTR) {
            return gui->msg_vm_active();
        }
        return false;
    }

    // 0x441ED0
    dllexport gnu_noinline void thiscall __display_stage_logo() asm_symbol_rel(0x441ED0);
};
#pragma region // Gui Validation
ValidateFieldOffset32(0x0, Gui, task_flags);
ValidateFieldOffset32(0x4, Gui, on_tick_func);
ValidateFieldOffset32(0x8, Gui, on_draw_func);
ValidateFieldOffset32(0x68, Gui, __anm_vm_ptr_array_68);
ValidateFieldOffset32(0x84, Gui, __anm_vm_84);
ValidateFieldOffset32(0x88, Gui, __anm_vm_88);
ValidateFieldOffset32(0x8C, Gui, __anm_id_8C);
ValidateFieldOffset32(0xB8, Gui, __anm_id_B8);
ValidateFieldOffset32(0xBC, Gui, __anm_id_BC);
ValidateFieldOffset32(0x110, Gui, __anm_id_110);
ValidateFieldOffset32(0x134, Gui, __int_134);
ValidateFieldOffset32(0x13C, Gui, __int_13C);
ValidateFieldOffset32(0x140, Gui, __timer_140);
ValidateFieldOffset32(0x158, Gui, __score);
ValidateFieldOffset32(0x15C, Gui, __int_15C);
ValidateFieldOffset32(0x170, Gui, __dword_170);
ValidateFieldOffset32(0x194, Gui, flags);
ValidateFieldOffset32(0x198, Gui, __timer_198);
ValidateFieldOffset32(0x1B0, Gui, msg_vm);
ValidateFieldOffset32(0x1B4, Gui, msg_file);
ValidateFieldOffset32(0x1B8, Gui, spell_timer_seconds);
ValidateFieldOffset32(0x1BC, Gui, spell_timer_hundredths);
ValidateFieldOffset32(0x1C0, Gui, __int_1C0);
ValidateFieldOffset32(0x1C4, Gui, lifebars);
ValidateFieldOffset32(0x2C0, Gui, __anm_loaded_2C0);
ValidateStructSize32(0x2CC, Gui);
#pragma endregion

static inline void __update_bomb_ui() {
    if (Gui* gui = GUI_PTR) {
        gui->__update_bomb_ui(GAME_MANAGER.globals.bomb_stocks, GAME_MANAGER.globals.bomb_fragments, GAME_MANAGER.globals.bomb_stock_max);
    }
}

#define SOUND_EFFECT_COUNT 84

// 0x4B47A0
const char* SOUND_EFFECT_FILENAMES[] = {
    "se_plst00.wav",
    "se_enep00.wav",
    "se_pldead00.wav",
    "se_power0.wav",
    "se_power1.wav",
    "se_tan00.wav",
    "se_tan01.wav",
    "se_tan02.wav",
    "se_ok00.wav",
    "se_cancel00.wav",
    "se_select00.wav",
    "se_gun00.wav",
    "se_cat00.wav",
    "se_lazer00.wav",
    "se_lazer01.wav",
    "se_enep01.wav",
    "se_damage00.wav",
    "se_item00.wav",
    "se_kira00.wav",
    "se_kira01.wav",
    "se_kira02.wav",
    "se_timeout.wav",
    "se_graze.wav",
    "se_powerup.wav",
    "se_pause.wav",
    "se_cardget.wav",
    "se_damage01.wav",
    "se_timeout2.wav",
    "se_invalid.wav",
    "se_slash.wav",
    "se_ch00.wav",
    "se_ch01.wav",
    "se_extend.wav",
    "se_cardget.wav", // Yes, this is a duplicate
    "se_nep00.wav",
    "se_bonus.wav",
    "se_bonus2.wav",
    "se_enep02.wav",
    "se_lazer02.wav",
    "se_nodamage.wav",
    "se_boon00.wav",
    "se_don00.wav",
    "se_boon01.wav",
    "se_ch02.wav",
    "se_ch03.wav",
    "se_extend2.wav",
    "se_pin00.wav",
    "se_pin01.wav",
    "se_lgods1.wav",
    "se_lgods2.wav",
    "se_lgods3.wav",
    "se_lgods4.wav",
    "se_lgodsget.wav",
    "se_msl.wav",
    "se_msl2.wav",
    "se_pldead01.wav",
    "se_heal.wav",
    "se_msl3.wav",
    "se_fault.wav",
    "se_noise.wav",
    "se_etbreak.wav",
    "se_tan03.wav",
    "se_wold.wav",
    "se_bonus4.wav",
    "se_big.wav",
    "se_item01.wav",
    "se_release.wav",
    "se_changeitem.wav",
    "se_trophy.wav",
    "se_warpl.wav",
    "se_warpr.wav",
    "se_notice.wav"
};

// size: 0x14
struct SoundData {
    int32_t id; // 0x0
    int32_t filename_index; // 0x4
    unknown_fields(0x2); // 0x8
    int16_t __short_A; // 0xA
    unknown_fields(0x8); // 0xC
};
#pragma region // SoundData Validation
ValidateFieldOffset32(0x0, SoundData, id);
ValidateFieldOffset32(0x4, SoundData, filename_index);
ValidateFieldOffset32(0xA, SoundData, __short_A);
ValidateStructSize32(0x14, SoundData);
#pragma endregion

// size: 0x34
struct ThBgmFormat {
    char filename[0x10]; // 0x0
    unknown_fields(0x4); // 0x10
    int __dword_14; // 0x14
    unknown_fields(0x1C); // 0x18
};
#pragma region // ThBgmFormat Validation
ValidateFieldOffset32(0x0, ThBgmFormat, filename);
ValidateFieldOffset32(0x14, ThBgmFormat, __dword_14);
ValidateStructSize32(0x34, ThBgmFormat);
#pragma endregion

struct WavFileHeader {
    char riff_text[4]; // 0x0
    uint32_t remaining_file_size; // 0x4
    char wave_text[4]; // 0x8
};
struct WavFileChunk {
    char id[4]; // 0x0
    uint32_t size; // 0x4
    unsigned char data[]; // 0x8
};

// 0x476830
dllexport gnu_noinline unsigned char* vectorcall get_wav_chunk_data(WavFileChunk* chunk, const char* chunk_id, int32_t* remaining_size_ptr, int32_t remaining_size) asm_symbol_rel(0x476830);
dllexport gnu_noinline unsigned char* vectorcall get_wav_chunk_data(WavFileChunk* chunk, const char* chunk_id, int32_t* remaining_size_ptr, int32_t remaining_size) {
    while (remaining_size) {
        *remaining_size_ptr = chunk->size;
        if (strncmp(chunk->id, chunk_id, sizeof(chunk->id)) == 0) {
            return chunk->data;
        }
        int32_t chunk_size = *remaining_size_ptr;
        chunk = pointer_raw_offset(chunk, sizeof(WavFileChunk) + chunk_size);
        remaining_size -= sizeof(WavFileChunk) + chunk_size;
    }
    return NULL;
}

struct WavFile {
    WavFileHeader header; // 0x0
    WavFileChunk chunks[]; // 0xC
};

/*
struct MMCKINFO {
    FOURCC ckid; // 0x0
    DWORD cksize; // 0x4
    FOURCC fccType; // 0x8
    DWORD dwDataOffset; // 0xC
    DWORD dwFlags; // 0x10
    // 0x14
};

class CWaveFile {
    WAVEFORMATEX* m_pwfx; // 0x0
    HMMIO m_hmmio; // 0x4
    MMCKINFO m_ck; // 0x8
    MMCKINFO m_ckRiff; // 0x1C
    DWORD m_dwSize; // 0x30
    MMIOINFO m_mmioinfoOut; // 0x34
    DWORD m_dwFlags; // 0x48
    BOOL m_bIsReadingFromMemory; // 0x4C
    BYTE* m_pbData; // 0x50
    BYTE* m_pbDataCur; // 0x54
    ULONG m_ulDataSize; // 0x58
    CHAR* m_pResourceBuffer; // 0x5C
};
*/

struct SoundManagerUnknownA {

};

// size: 0x18
struct SoundManagerUnknownB {
    LPDIRECTSOUNDBUFFER sound_buffer; // 0x0
    int32_t __int_4; // 0x4
    SoundData* data; // 0x8
    int32_t __int_C; // 0xC
    int32_t __int_10; // 0x10
    int __dword_14; // 0x14
    // 0x18

    // 0x4776F0
    dllexport gnu_noinline ZUNResult thiscall __sub_4776F0(const char* filename) asm_symbol_rel(0x4776F0);
};
#pragma region // SoundManagerUnknownB Validation
ValidateFieldOffset32(0x0, SoundManagerUnknownB, sound_buffer);
ValidateFieldOffset32(0x4, SoundManagerUnknownB, __int_4);
ValidateFieldOffset32(0x8, SoundManagerUnknownB, data);
ValidateFieldOffset32(0xC, SoundManagerUnknownB, __int_C);
ValidateFieldOffset32(0x10, SoundManagerUnknownB, __int_10);
ValidateFieldOffset32(0x14, SoundManagerUnknownB, __dword_14);
ValidateStructSize32(0x18, SoundManagerUnknownB);
#pragma endregion

// size: 0x10C
struct SoundManagerUnknownD {
    unknown_fields(0x10C); // 0x0
};
#pragma region // SoundManagerUnknownD Validation
ValidateStructSize32(0x10C, SoundManagerUnknownD);
#pragma endregion

struct SoundManagerUnknownG {
    unknown_fields(0x78); // 0x0
    int __dword_78; // 0x78
    unknown_fields(0x10); // 0x7C
    HANDLE __handle_8C; // 0x8C
    // 0x90
};
#pragma region // SoundManagerUnknownG Validation
ValidateFieldOffset32(0x78, SoundManagerUnknownG, __dword_78);
ValidateFieldOffset32(0x8C, SoundManagerUnknownG, __handle_8C);
#pragma endregion

// This seems to be another part of CSound that isn't in my version of it...
struct SoundManagerUnknownE {

    // vtable ptr
    unknown_fields(0x4); // 0x4
    LPDIRECTSOUNDBUFFER* sound_buffer_array; // 0x8
    unknown_fields(0x4); // 0xC
    SoundManagerUnknownG* __smg_ptr_10; // 0x10
    uint32_t sound_buffer_count; // 0x14
    unknown_fields(0x8); // 0x18
    int __dword_20; // 0x20
    unknown_fields(0x34); // 0x24
    int __dword_58; // 0x58
    int __dword_5C; // 0x5C
    // 0x60

    // 0x48A620
    dllexport gnu_noinline HRESULT thiscall __sub_48A620(BOOL Arg1) asm_symbol_rel(0x48A620) {
        if (!this->sound_buffer_array) {
            return CO_E_NOTINITIALIZED;
        }
        this->__dword_58 = 0;
        this->__dword_5C = 0;
        HRESULT ret = 0;
        for (size_t i = 0; i < this->sound_buffer_count; ++i) {
            ret |= this->sound_buffer_array[i]->Stop();
            ret |= this->sound_buffer_array[i]->SetCurrentPosition(0);
        }
        this->__dword_20 = 0;
        if (Arg1) {
            SoundManagerUnknownG* smg_ptr = this->__smg_ptr_10;
            if (smg_ptr->__dword_78 == 1) {
                CloseHandle(smg_ptr->__handle_8C);
                smg_ptr->__handle_8C = INVALID_HANDLE_VALUE;
            }
        }
        // TODO
    }

    // 0x48AF10
    dllexport gnu_noinline void vectorcall __sub_48AF10(double arg1) asm_symbol_rel(0x48AF10) {
        use_var(arg1);
    }

    // This is giga jank
    virtual ~SoundManagerUnknownE() {}
};
#pragma region // SoundManagerUnknownE Validation
ValidateVirtualFieldOffset32(0x8, SoundManagerUnknownE, sound_buffer_array);
ValidateVirtualFieldOffset32(0x10, SoundManagerUnknownE, __smg_ptr_10);
ValidateVirtualFieldOffset32(0x14, SoundManagerUnknownE, sound_buffer_count);
ValidateVirtualFieldOffset32(0x20, SoundManagerUnknownE, __dword_20);
ValidateVirtualFieldOffset32(0x58, SoundManagerUnknownE, __dword_58);
ValidateVirtualFieldOffset32(0x5C, SoundManagerUnknownE, __dword_5C);
#pragma endregion

// size: 0x200
struct SoundManagerUnknownF {
    int32_t __int_array_0[60]; // 0x0
    unknown_fields(0x110); // 0xF0
};
#pragma region // SoundManagerUnknownF Validation
ValidateFieldOffset32(0x0, SoundManagerUnknownF, __int_array_0);
ValidateStructSize32(0x200, SoundManagerUnknownF);
#pragma endregion

extern "C" {
    // 0x4C9B80
    extern SoundData SOUND_DATA[SOUND_EFFECT_COUNT] asm("_SOUND_DATA");
}

typedef struct SoundManager SoundManager;

// 0x4B47A0
static inline constexpr const char *const SoundEffectFilenames[] = {
    "se_plst00.wav",
    "se_enep00.wav",
    "se_pldead00.wav",
    "se_power0.wav",
    "se_power1.wav",
    "se_tan00.wav",
    "se_tan01.wav",
    "se_tan02.wav",
    "se_ok00.wav",
    "se_cancel00.wav",
    "se_select00.wav",
    "se_gun00.wav",
    "se_cat00.wav",
    "se_lazer00.wav",
    "se_lazer01.wav",
    "se_enep01.wav",
    "se_damage00.wav",
    "se_item00.wav",
    "se_kira00.wav",
    "se_kira01.wav",
    "se_kira02.wav",
    "se_timeout.wav",
    "se_graze.wav",
    "se_powerup.wav",
    "se_pause.wav",
    "se_cardget.wav",
    "se_damage01.wav",
    "se_timeout2.wav",
    "se_invalid.wav",
    "se_slash.wav",
    "se_ch00.wav",
    "se_ch01.wav",
    "se_extend.wav",
    "se_cardget.wav",
    "se_nep00.wav",
    "se_bonus.wav",
    "se_bonus2.wav",
    "se_enep02.wav",
    "se_lazer02.wav",
    "se_nodamage.wav",
    "se_boon00.wav",
    "se_don00.wav",
    "se_boon01.wav",
    "se_ch02.wav",
    "se_ch03.wav",
    "se_extend2.wav",
    "se_pin00.wav",
    "se_pin01.wav",
    "se_lgods1.wav",
    "se_lgods2.wav",
    "se_lgods3.wav",
    "se_lgods4.wav",
    "se_lgodsget.wav",
    "se_msl.wav",
    "se_msl2.wav",
    "se_pldead01.wav",
    "se_heal.wav",
    "se_msl3.wav",
    "se_fault.wav",
    "se_noise.wav",
    "se_etbreak.wav",
    "se_tan03.wav",
    "se_wolf.wav",
    "se_bonus4.wav",
    "se_big.wav",
    "se_item01.wav",
    "se_release.wav",
    "se_changeitem.wav",
    "se_trophy.wav",
    "se_warpl.wav",
    "se_warpr.wav",
    "se_notice.wav"
};

// size: 0x573C
struct SoundManager {
    LPDIRECTSOUND8 dsound; // 0x0
    LPDIRECTSOUNDBUFFER sound_buffer_ptr; // 0x4
    HWND timer_hwnd; // 0x8
    CSoundManager* csound_manager_ptr; // 0xC
    DWORD sound_thread_id; // 0x10
    HANDLE __handle_14; // 0x14
    unknown_fields(0x4); // 0x18
    int32_t active_sound_ids[12]; // 0x1C
    int32_t active_sound_id_counts[12]; // 0x4C
    SoundManagerUnknownF __unknown_smf_array_7C[12]; // 0x7C
    void* bgm_format_offsets[16]; // 0x187C
    SoundManagerUnknownA* __unknown_sma_ptr_array_18BC[16]; // 0x18BC
    SoundManagerUnknownA* __unknown_sma_ptr_array_18FC[16]; // 0x18FC
    int __bgm_format_14_array_193C[16]; // 0x193C
    int __dword_197C; // 0x197C
    union {
        void* bgm_format_file; // 0x1980
        ThBgmFormat* bgm_formats;
    };
    char __text_buffer_1984[0x100]; // 0x1984
    SoundManagerUnknownB __unknown_smb_array_1A84[SOUND_EFFECT_COUNT]; // 0x1A84
    void* sound_effect_files[countof(SOUND_EFFECT_FILENAMES)]; // 0x2264
    char __text_buffer_2384[0x100]; // 0x2384
    SoundManagerUnknownD __unknown_smd_array_2484[32]; // 0x2484 (only 31 are used properly)
    char __text_buffer_array_4604[16][0x100]; // 0x4604
    char thbgm_filename[0x100]; // 0x5604
    SoundManagerUnknownE* __unknown_sme_ptr_5704; // 0x5704
    unknown_fields(0x4); // 0x5708
    HANDLE __handle_570C; // 0x570C
    unknown_fields(0x4); // 0x5710
    int32_t file_pointer_offset; // 0x5714
    HANDLE __thread_5718; // 0x5718
    HANDLE __handle_571C; // 0x571C
    DWORD __thread_id_5720; // 0x5720
    int __dword_5724; // 0x5724
    HWND main_window_hwnd; // 0x5728
    int __dword_572C; // 0x572C
    int __dword_5730; // 0x5730
    int __dword_5734; // 0x5734
    int __dword_5738; // 0x5738
    // 0x573C

    inline void zero_contents() {
        zero_this();
    }

    inline void copy_sound_data() {
        SoundManagerUnknownB* unknown_b_ptr = this->__unknown_smb_array_1A84;
        for (size_t i = 0; i < countof(this->__unknown_smb_array_1A84); ++unknown_b_ptr, ++i) {
            unknown_b_ptr->__int_4 = -1;
            SoundData* sound_data = SOUND_DATA;
            while (sound_data->id != i) ++sound_data;
            unknown_b_ptr->__int_C = i;
            unknown_b_ptr->data = sound_data;
        }
    }

    inline void free_bgm_format_file() {
        SAFE_FREE_FAST(this->bgm_format_file);
    }

    inline void release_smb_array() {
        SoundManagerUnknownB* smb_ptr = this->__unknown_smb_array_1A84;
        for (size_t i = 0; i < countof(this->__unknown_smb_array_1A84); ++smb_ptr, ++i) {
            SAFE_RELEASE(smb_ptr->sound_buffer);
        }
    }

    inline void free_all_sound_effect_files() {
        void** sound_file_pointer = this->sound_effect_files;
        for (size_t i = 0; i < countof(this->sound_effect_files); ++sound_file_pointer, ++i) {
            SAFE_FREE_FAST(*sound_file_pointer);
        }
    }

    inline void free_first_sma_array() {
        SoundManagerUnknownA** sma_ptr = this->__unknown_sma_ptr_array_18BC;
        for (size_t i = 0; i < countof(this->__unknown_sma_ptr_array_18BC); ++sma_ptr, ++i) {
            SAFE_FREE_FAST(*sma_ptr);
        }
    }

    SoundManager() {
        this->copy_sound_data();
    }

    // 0x476320
    dllexport static gnu_noinline int32_t __wait_and_close_handles() asm_symbol_rel(0x476320);

    // 0x4763D0
    dllexport static gnu_noinline DWORD WINAPI sound_thread_func(void* self) asm_symbol_rel(0x4763D0);

    // 0x476410
    dllexport gnu_noinline ZUNResult thiscall __sub_476410(HWND window_hwnd_arg) asm_symbol_rel(0x476410);

    // 0x4767B0
    dllexport static gnu_noinline DWORD WINAPI load_sound_effects(void* self) asm_symbol_rel(0x4767B0);
    
    // 0x476890
    dllexport gnu_noinline int32_t thiscall __sub_476890(const char* filename) asm_symbol_rel(0x476890) {
        char buffer[128];
        const char* path;
        if (!(path = strrchr(filename, '/')) && !(path = strrchr(filename, '\\'))) {
            byteloop_strcpy(buffer, filename);
        } else {
            byteloop_strcpy(buffer, path + 1);
        }
        ThBgmFormat* bgm_formats = this->bgm_formats;
        size_t i = 0;
        for (; bgm_formats[i].filename[0] != '\0';) {
            // TODO
        }
    }
    
    // 0x476B40
    dllexport gnu_noinline void thiscall __sub_476B40() asm_symbol_rel(0x476B40);

    // 0x476BE0
    dllexport static gnu_noinline void stdcall play_sound_centered(int32_t sound_id, float) asm_symbol_rel(0x476BE0);

    // 0x476C70
    dllexport static gnu_noinline void vectorcall play_sound_positioned(int32_t sound_id, float position) asm_symbol_rel(0x476C70);

    static inline void play_sound(int32_t sound_id) {
        SoundManager::play_sound_centered(sound_id, UNUSED_FLOAT);
    }

    static inline void play_sound_validate(int32_t sound_id) {
        if (sound_id >= 0) SoundManager::play_sound(sound_id);
    }

    static inline void play_sound_positioned_validate(int32_t sound_id, const float& position) {
        if (sound_id >= 0) SoundManager::play_sound_positioned(sound_id, position);
    }

    inline void cleanup() { // Might be destructor?
        this->__wait_and_close_handles();
        this->free_bgm_format_file();
        this->release_smb_array();
        this->free_all_sound_effect_files();
        if (this->csound_manager_ptr) {
            KillTimer(this->timer_hwnd, 1);
            this->__sub_476B40();
            this->dsound = NULL; // Isn't this supposed to be released though?
            this->sound_buffer_ptr->Stop();
            SAFE_RELEASE(this->sound_buffer_ptr);
            SAFE_DELETE(this->__unknown_sme_ptr_5704);
            SAFE_DELETE(this->csound_manager_ptr); // This destructor got inlined
            this->free_first_sma_array();
        }
    }
};
#pragma region // SoundManager Validation
ValidateFieldOffset32(0x0, SoundManager, dsound);
ValidateFieldOffset32(0x4, SoundManager, sound_buffer_ptr);
ValidateFieldOffset32(0x8, SoundManager, timer_hwnd);
ValidateFieldOffset32(0xC, SoundManager, csound_manager_ptr);
ValidateFieldOffset32(0x10, SoundManager, sound_thread_id);
ValidateFieldOffset32(0x14, SoundManager, __handle_14);
ValidateFieldOffset32(0x1C, SoundManager, active_sound_ids);
ValidateFieldOffset32(0x4C, SoundManager, active_sound_id_counts);
ValidateFieldOffset32(0x7C, SoundManager, __unknown_smf_array_7C);
ValidateFieldOffset32(0x187C, SoundManager, bgm_format_offsets);
ValidateFieldOffset32(0x18BC, SoundManager, __unknown_sma_ptr_array_18BC);
ValidateFieldOffset32(0x18FC, SoundManager, __unknown_sma_ptr_array_18FC);
ValidateFieldOffset32(0x193C, SoundManager, __bgm_format_14_array_193C);
ValidateFieldOffset32(0x197C, SoundManager, __dword_197C);
ValidateFieldOffset32(0x1980, SoundManager, bgm_format_file);
ValidateFieldOffset32(0x1A84, SoundManager, __unknown_smb_array_1A84);
ValidateFieldOffset32(0x2264, SoundManager, sound_effect_files);
ValidateFieldOffset32(0x2384, SoundManager, __text_buffer_2384);
ValidateFieldOffset32(0x2484, SoundManager, __unknown_smd_array_2484);
ValidateFieldOffset32(0x5604, SoundManager, thbgm_filename);
ValidateFieldOffset32(0x5704, SoundManager, __unknown_sme_ptr_5704);
ValidateFieldOffset32(0x570C, SoundManager, __handle_570C);
ValidateFieldOffset32(0x5714, SoundManager, file_pointer_offset);
ValidateFieldOffset32(0x5718, SoundManager, __thread_5718);
ValidateFieldOffset32(0x571C, SoundManager, __handle_571C);
ValidateFieldOffset32(0x5720, SoundManager, __thread_id_5720);
ValidateFieldOffset32(0x5724, SoundManager, __dword_5724);
ValidateFieldOffset32(0x5728, SoundManager, main_window_hwnd);
ValidateFieldOffset32(0x572C, SoundManager, __dword_572C);
ValidateFieldOffset32(0x5730, SoundManager, __dword_5730);
ValidateFieldOffset32(0x5734, SoundManager, __dword_5734);
ValidateFieldOffset32(0x5738, SoundManager, __dword_5738);
ValidateStructSize32(0x573C, SoundManager);
#pragma endregion

extern "C" {
    // 0x56AD80
    extern SoundManager SOUND_MANAGER asm("_SOUND_MANAGER");
}



// 0x453640
dllexport gnu_noinline ZUNResult fastcall Supervisor::on_registration(void* self) {
    if (THDAT_ARCFILE.__sub_46EB80()) {
        char ver_file_name[64];
        int32_t ver_file_size;
        sprintf(ver_file_name, "th18_%.4x%c.ver", 100, 'a');
        void* ver_file = read_file_to_buffer(ver_file_name, &ver_file_size, false);
        SUPERVISOR.ver_file_buffer = ver_file;
        if (!ver_file) {
            LOG_BUFFER.write_error(JpEnStr("", "error : Wrong data version\r\n"));
        }
    }
    else {
        LOG_BUFFER.write_error(JpEnStr("", "error : data file does not exist\r\n"));
    }
    GAME_SPEED.value = 1.0f;
    SUPERVISOR.background_color = PackD3DCOLOR(255, 0, 0, 0);
    SUPERVISOR.__initialize_cameras();
    SUPERVISOR.__camera2_sub_454F50();
    DWORD time = timeGetTime();
    SUPERVISOR.initial_rng_seed = time;
    REPLAY_RNG.value = time;
    RNG.value = time;
    DWORD thread_id;
    SOUND_MANAGER.__handle_571C = CreateThread(
        NULL,
        0,
        &SoundManager::load_sound_effects,
        &SOUND_MANAGER,
        0,
        &thread_id
    );
    // TODO
}

// 0x4767B0
dllexport gnu_noinline DWORD WINAPI SoundManager::load_sound_effects(void* self) {
    int32_t i = 0;
    do {
        if (SOUND_MANAGER.__dword_5724 == 2) {
            return 1;
        }
        void* sound_effect_file = read_file_to_buffer(SoundEffectFilenames[i], NULL, false);
        SOUND_MANAGER.sound_effect_files[i] = sound_effect_file;
        if (!sound_effect_file) {
            LOG_BUFFER.write(JpEnStr("", "error : Sound File cannot read Check data %s\r\n"), SoundEffectFilenames[i]);
            return 1;
        }
    } while (++i < countof(SoundEffectFilenames));
    while (SOUND_MANAGER.__dword_5724 == 0) {
        Sleep(1);
    }
    return 0;
}

// 0x4776F0
dllexport gnu_noinline ZUNResult thiscall SoundManagerUnknownB::__sub_4776F0(const char* filename) {
    if (SOUND_MANAGER.csound_manager_ptr) {
        SAFE_RELEASE(this->sound_buffer);
        SoundManagerUnknownB* unknown_b_ptr = SOUND_MANAGER.__unknown_smb_array_1A84;
        int32_t idk = this->__int_C;
        int32_t current_filename_index = this->data->filename_index;
        for (int32_t i = 0; i < idk; ++i, ++unknown_b_ptr) {
            if (unknown_b_ptr->data->filename_index == current_filename_index) {
                SOUND_MANAGER.dsound->DuplicateSoundBuffer(SOUND_MANAGER.__unknown_smb_array_1A84[i].sound_buffer, &this->sound_buffer);
                return ZUN_SUCCESS;
            }
        }
        while (!SOUND_MANAGER.sound_effect_files[this->data->filename_index]) {
            Sleep(10);
        }
        const char* error_text;
        if (WavFile* sound_file = (WavFile*)SOUND_MANAGER.sound_effect_files[this->data->filename_index]) {
            if (!strncmp(sound_file->header.riff_text, "RIFF", sizeof(sound_file->header.riff_text))) {
                error_text = JpEnStr("Wav ファイルじゃない %s\r\n", "Not a Wav file %s\r\n");
                goto OtherError;
            }
            int32_t file_size = sound_file->header.remaining_file_size;
            if (!strncmp(sound_file->header.wave_text, "WAVE", sizeof(sound_file->header.wave_text))) {
                goto MalformedWaveError;
            }
            int32_t data_size = file_size - sizeof(WavFileHeader);
            WAVEFORMATEX* wav_format_ptr = (WAVEFORMATEX*)get_wav_chunk_data(sound_file->chunks, "fmt ", &file_size, data_size);
            if (!wav_format_ptr) {
                goto MalformedWaveError;
            }
            WAVEFORMATEX wav_format = *wav_format_ptr;
            unsigned char* sound_data_ptr = get_wav_chunk_data(sound_file->chunks, "data", &file_size, data_size);
            if (!sound_data_ptr) {
                goto MalformedWaveError;
            }
            DSBUFFERDESC wav_buffer_desc = {
                .dwSize = sizeof(DSBUFFERDESC),
                .dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE,
                .dwBufferBytes = (DWORD)file_size,
                .dwReserved = 0,
                .lpwfxFormat = &wav_format,
                .guid3DAlgorithm = DS3DALG_DEFAULT
            };
            if (SUCCEEDED(SOUND_MANAGER.dsound->CreateSoundBuffer(&wav_buffer_desc, &this->sound_buffer, NULL))) {
                LPVOID buffer_contents_ptr_A, buffer_contents_ptr_B;
                DWORD buffer_contents_size_A, buffer_contents_size_B;
                if (SUCCEEDED(this->sound_buffer->Lock(0, file_size, &buffer_contents_ptr_A, &buffer_contents_size_A, &buffer_contents_ptr_B, &buffer_contents_size_B, 0))) {
                    memcpy(buffer_contents_ptr_A, sound_data_ptr, buffer_contents_size_A);
                    if (buffer_contents_size_B) {
                        memcpy(buffer_contents_ptr_B, sound_data_ptr + buffer_contents_size_A, buffer_contents_size_B);
                    }
                    this->sound_buffer->Unlock(buffer_contents_ptr_A, buffer_contents_size_A, buffer_contents_ptr_B, buffer_contents_size_B);
                    SAFE_FREE(SOUND_MANAGER.sound_effect_files[this->data->filename_index]);
                    DebugLogger::__debug_log_stub_10("Create Sound Buffer %s\n", filename);
                }
            }
        }
        else {
MalformedWaveError:
            error_text = JpEnStr("Wav ファイルじゃない? %s\r\n", "Isn't it a Wav file? %s\r\n");
OtherError:
            LOG_BUFFER.write(error_text, filename);
            SAFE_FREE(SOUND_MANAGER.sound_effect_files[this->data->filename_index]);
            return ZUN_ERROR;
        }
    }
    return ZUN_SUCCESS;
}

// 0x476320
dllexport gnu_noinline int32_t SoundManager::__wait_and_close_handles() {
    if (SOUND_MANAGER.__thread_5718) {
        SOUND_MANAGER.__dword_5724 = SOUND_MANAGER.__dword_5724 == 0 ? 1 : SOUND_MANAGER.__dword_5724;
        while (WaitForSingleObject(SOUND_MANAGER.__thread_5718, 100) == WAIT_TIMEOUT) {
            Sleep(1);
        }
        while (WaitForSingleObject(SOUND_MANAGER.__handle_571C, 100) == WAIT_TIMEOUT) {
            Sleep(1);
        }
        CloseHandle(SOUND_MANAGER.__thread_5718);
        CloseHandle(SOUND_MANAGER.__handle_571C);
        SOUND_MANAGER.__thread_5718 = NULL;
        SOUND_MANAGER.__handle_571C = NULL;
    }
    return 0;
}

// 0x4763D0
dllexport gnu_noinline DWORD WINAPI SoundManager::sound_thread_func(void* self) {
    SOUND_MANAGER.__sub_476410(SOUND_MANAGER.main_window_hwnd);
    while (!SOUND_MANAGER.__dword_5724) {
        Sleep(1);
    }
    SOUND_MANAGER.__dword_572C = 1;
    return 0;
}

// 0x476410
dllexport gnu_noinline ZUNResult SoundManager::__sub_476410(HWND window_hwnd_arg) {
    HWND window_hwnd = window_hwnd_arg;
    this->copy_sound_data();
    memset(this->active_sound_ids, -1, sizeof(this->active_sound_ids));
    CSoundManager* csound_manager_ptr = new CSoundManager();
    this->csound_manager_ptr = csound_manager_ptr;
    if (SUCCEEDED(csound_manager_ptr->Initialize(window_hwnd, DSSCL_PRIORITY))) {
        csound_manager_ptr->SetPrimaryBufferFormat(2, 44100, 16);
        LPDIRECTSOUND8 dsound = this->csound_manager_ptr->GetDirectSound();
        this->dsound = dsound;
        LPDIRECTSOUNDBUFFER* buffer_ptr_ptr = &this->sound_buffer_ptr;
        WAVEFORMATEX sound_format = {
            .wFormatTag = WAVE_FORMAT_1S08,
            .nChannels = 2,
            .nSamplesPerSec = 44100,
            .nAvgBytesPerSec = 176400,
            .nBlockAlign = 4,
            .wBitsPerSample = 16,
            .cbSize = 0
        };
        DSBUFFERDESC buffer_desc = {
            .dwSize = sizeof(DSBUFFERDESC),
            .dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE,
            .dwBufferBytes = 0x8000,
            .dwReserved = 0,
            .lpwfxFormat = &sound_format,
            .guid3DAlgorithm = DS3DALG_DEFAULT
        };
        if (FAILED(dsound->CreateSoundBuffer(&buffer_desc, buffer_ptr_ptr, NULL))) {
            return ZUN_ERROR;
        }
        LPVOID buffer_contents_ptr_A;
        DWORD buffer_contents_size_A;
        LPVOID buffer_contents_ptr_B;
        DWORD buffer_contents_size_B;
        if (FAILED((*buffer_ptr_ptr)->Lock(0, 0x8000, &buffer_contents_ptr_A, &buffer_contents_size_A, &buffer_contents_ptr_B, &buffer_contents_size_B, 0))) {
            return ZUN_ERROR;
        }
        memset(buffer_contents_ptr_A, 0, buffer_contents_size_A);
        (*buffer_ptr_ptr)->Unlock(buffer_contents_ptr_A, buffer_contents_size_A, buffer_contents_ptr_B, buffer_contents_size_B);
        (*buffer_ptr_ptr)->Play(0, 0, DSBPLAY_LOOPING);
        this->__dword_5730 = 100;
        this->__dword_5734 = 100;
        SetTimer(window_hwnd, 0, 250, NULL);
        this->timer_hwnd = window_hwnd;
        window_hwnd = NULL;
        SoundManagerUnknownB* unknown_b_ptr = this->__unknown_smb_array_1A84;
        SoundData* sound_data = SOUND_DATA;
        for (size_t i = 0; sound_data < array_end_addr(SOUND_DATA); ++i, ++sound_data, ++unknown_b_ptr) {
            if (SOUND_MANAGER.__dword_5724 == 2) {
                return ZUN_ERROR;
            }
            if (ZUN_FAILED(unknown_b_ptr->__sub_4776F0(SOUND_EFFECT_FILENAMES[sound_data->filename_index]))) {
                LOG_BUFFER.write(
                    JpEnStr("error : Sound ファイルが読み込めない データを確認 %s\r\n", "error : Sound File cannot read Check data %s\r\n")
                    , SOUND_EFFECT_FILENAMES[SOUND_DATA[i].filename_index]
                );
                return ZUN_ERROR;
            }
        }
        LOG_BUFFER.write(JpEnStr("DirectSound は正常に初期化されました\r\n", "DirectSound has been successfully initialized\r\n"));
        return ZUN_SUCCESS;
    } else {
        LOG_BUFFER.write(JpEnStr("DirectSound オブジェクトの初期化が失敗したよ\r\n", "Initialization of DirectSound object failed\r\n"));
        if (this->csound_manager_ptr) {
            SAFE_DELETE(this->csound_manager_ptr);
        }
        return ZUN_ERROR;
    }
}

// 0x476B40
dllexport gnu_noinline void thiscall SoundManager::__sub_476B40() {
    if (SoundManagerUnknownE* sme_ptr = this->__unknown_sme_ptr_5704) {
        sme_ptr->__sub_48A620(true);
        if (this->__handle_14) {
            do {
                PostThreadMessageA(this->sound_thread_id, WM_QUIT, 0, 0);
            } while (WaitForSingleObject(this->__handle_14, 256));
            CloseHandle(this->__handle_14);
            CloseHandle(this->__handle_570C);
            this->__handle_14 = 0;
        }
    }
    SAFE_DELETE(this->__unknown_sme_ptr_5704);
}

// 0x476BE0
dllexport gnu_noinline void stdcall SoundManager::play_sound_centered(int32_t sound_id, float) {
    int32_t idk = SOUND_DATA[sound_id].__short_A;
    nounroll for (size_t i = 0; i < countof(SOUND_MANAGER.active_sound_ids); ++i) {
        int32_t active_sound_id = SOUND_MANAGER.active_sound_ids[i];
        if (active_sound_id < 0) {
            SOUND_MANAGER.active_sound_ids[i] = sound_id;
            SOUND_MANAGER.__unknown_smf_array_7C[i].__int_array_0[0] = 0;
            SOUND_MANAGER.active_sound_id_counts[i] = 1;
            SOUND_MANAGER.__unknown_smb_array_1A84[i].__int_4 = idk;
            return;
        }
        if (active_sound_id == sound_id) {
            int32_t active_sound_id_count = SOUND_MANAGER.active_sound_id_counts[i];
            int32_t* active_sound_id_count_ptr = &SOUND_MANAGER.active_sound_id_counts[i];
            if (active_sound_id_count < 60 && active_sound_id_count >= 0) {
                SOUND_MANAGER.__unknown_smf_array_7C[i].__int_array_0[active_sound_id_count] = 0;
                ++*active_sound_id_count_ptr;
            }
            return;
        }
    }
}

// 0x476C70
dllexport gnu_noinline void vectorcall SoundManager::play_sound_positioned(int32_t sound_id, float position) {
    int32_t idk = SOUND_DATA[sound_id].__short_A;
    int32_t idk2 = (int32_t)(position * 1000.0f / 192.0f);
    nounroll for (size_t i = 0; i < countof(SOUND_MANAGER.active_sound_ids); ++i) {
        int32_t active_sound_id = SOUND_MANAGER.active_sound_ids[i];
        if (active_sound_id < 0) {
            SOUND_MANAGER.active_sound_ids[i] = sound_id;
            SOUND_MANAGER.__unknown_smf_array_7C[i].__int_array_0[0] = idk2;
            SOUND_MANAGER.active_sound_id_counts[i] = 1;
            SOUND_MANAGER.__unknown_smb_array_1A84[i].__int_4 = idk;
            return;
        }
        if (active_sound_id == sound_id) {
            int32_t active_sound_id_count = SOUND_MANAGER.active_sound_id_counts[i];
            int32_t* active_sound_id_count_ptr = &SOUND_MANAGER.active_sound_id_counts[i];
            if (active_sound_id_count < 60 && active_sound_id_count >= 0) {
                SOUND_MANAGER.__unknown_smf_array_7C[i].__int_array_0[active_sound_id_count] = idk2;
                ++*active_sound_id_count_ptr;
            }
            return;
        }
    }
}

enum ReplayMode {
    __replay_recording = 0,
    __replay_playback = 1,
    __replay_mode_2 = 2
};

static inline bool is_replay();

typedef struct GameThread GameThread;
extern "C" {
    // 0x4CF2E4
    extern GameThread* GAME_THREAD_PTR asm("_GAME_THREAD_PTR");
}

// size: 0xD8
struct GameThread : ZUNTask {
    //ZUNTask base; // 0x0
    Timer __timer_C; // 0xC
    int32_t stage_number; // 0x20
    int32_t chapter; // 0x24
    Config config; // 0x28
    union {
        uint32_t flags; // 0xB0
        struct {
            uint32_t __unknown_flag_A : 1; // 1
            uint32_t __unknown_flag_B : 1; // 2
            uint32_t skip_flag : 1; // 3
            uint32_t __unknown_flag_H : 1; // 4
            uint32_t __unknown_flag_I : 1; // 5
            uint32_t __unknown_flag_L : 1; // 6
            uint32_t __unknown_flag_M : 1; // 7
            uint32_t __unknown_flag_J : 1; // 8
            uint32_t __unknown_flag_D : 1; // 9
            uint32_t : 1; // 10
            uint32_t __unknown_flag_C : 1; // 11
            uint32_t __unknown_flag_K : 1; // 12
            uint32_t : 2; // 13-14
            uint32_t __unknown_flag_E : 1; // 15
            uint32_t : 1; // 16
            uint32_t __unknown_flag_F : 1; // 17
            uint32_t __unknown_flag_G : 1; // 18
        };
    };
    unknown_fields(0x4); // 0xB4
    int __int_B8; // 0xB8
    unknown_fields(0x14); // 0xBC
    ReplayMode replay_mode; // 0xD0
    int __int_D4; // 0xD4
    // 0xD8

    inline void zero_contents() {
        zero_this();
    }

    inline GameThread() {
        this->zero_contents();
    }

    // 0x439E80
    dllexport gnu_noinline void thiscall set_chapter(int32_t chapter) asm_symbol_rel(0x439E80) {
        GAME_MANAGER.globals.chapter = chapter;
        GAME_THREAD_PTR->chapter = chapter;
    }

    // 0x443860
    dllexport gnu_noinline UpdateFuncRet thiscall on_tick() asm_symbol_rel(0x443860);

    // 0x443D70
    dllexport gnu_noinline static UpdateFuncRet fastcall on_tick(void* ptr) asm_symbol_rel(0x443D70) {
        return ((GameThread*)ptr)->on_tick();
    }

    // 0x443D80
    dllexport gnu_noinline static UpdateFuncRet fastcall on_draw(void* ptr) asm_symbol_rel(0x443D80);

private:
    inline unsigned thread_start_impl();
public:
    // 0x4424E0
    dllexport gnu_noinline unsigned thiscall thread_start() {
        return GAME_THREAD_PTR->thread_start_impl();
    }

    // 0x4432B0
    dllexport static unsigned cdecl thread_start_stub(void*) {
        return GAME_THREAD_PTR->thread_start();
    }

    // 0x443E60
    dllexport gnu_noinline ZUNResult thiscall __sub_443E60() asm_symbol_rel(0x443E60) {

    }

    // 0x4443C0
    dllexport gnu_noinline ZUNResult thiscall __sub_4443C0() asm_symbol_rel(0x4443C0) {

    }

    // 0x4437B0
    dllexport gnu_noinline static GameThread* fastcall allocate(ReplayMode mode) asm_symbol_rel(0x4437B0) {
        GameThread* game_thread = new GameThread();
        WINDOW_DATA.__int_20D0 = 0;
        SUPERVISOR.d3d_device->EvictManagedResources();
        game_thread->skip_flag = true;
        GAME_THREAD_PTR = game_thread;
        game_thread->replay_mode = mode;
        SUPERVISOR.__start_thread_A94((_beginthreadex_proc_type)&thread_start_stub);
        return game_thread;
    }
};

typedef int32_t (*volatile fastcall AnmOnFunc)(AnmVM*);
typedef int32_t (*volatile fastcall AnmOnFuncArg)(AnmVM*, int32_t);

extern inline const AnmOnFunc ANM_ON_WAIT_FUNCS[];
//extern inline const AnmOnFunc ANM_ON_TICK_FUNCS[];
//extern inline const AnmOnFunc ANM_ON_DRAW_FUNCS[];
//extern inline const AnmOnFunc ANM_ON_DESTROY_FUNCS[];
//extern inline const AnmOnFuncArg ANM_ON_INTERRUPT_FUNCS[];
extern inline const AnmOnFunc ANM_ON_COPY_A_FUNCS[];
extern inline const AnmOnFunc ANM_ON_COPY_B_FUNCS[];
//extern inline const AnmOnFuncArg ANM_ON_SPRITE_LOOKUP_FUNCS[];
extern "C" {
    extern AnmOnFunc ANM_ON_TICK_FUNCS[] asm("_ANM_ON_TICK_FUNCS");
    extern AnmOnFunc ANM_ON_DESTROY_FUNCS[] asm("_ANM_ON_DESTROY_FUNCS");
    extern AnmOnFuncArg ANM_ON_INTERRUPT_FUNCS[] asm("_ANM_ON_INTERRUPT_FUNCS");
    extern AnmOnFuncArg ANM_ON_SPRITE_LOOKUP_FUNCS[] asm("_ANM_ON_SPRITE_LOOKUP_FUNCS");
    extern AnmOnFunc ANM_ON_DRAW_FUNCS[] asm("_ANM_ON_DRAW_FUNCS");
}

// size: 0x8
struct AnmInstruction {
    int16_t opcode; // 0x0
    uint16_t offset_to_next; // 0x2
    int16_t time; // 0x4
    uint16_t param_mask; // 0x6
    unsigned char args[]; // 0x8
};

enum AnmVMCreationListType {
    WorldListBack = 0,
    WorldListFront = 1,
    UiListBack = 2,
    UiListFront = 3,
};
union AnmVMCreationFlags {
    uint32_t raw;
    struct {
        uint32_t __unknown_flag_A : 1; // 1
        uint32_t list_type : 2; // 2-3
        uint32_t __unknown_flag_B : 1; // 4
    };
};

// 0x0
static inline constexpr AnmVMCreationFlags WORLD_LIST_BACK = { .list_type = WorldListBack };
// 0x2
static inline constexpr AnmVMCreationFlags WORLD_LIST_FRONT = { .list_type = WorldListFront };
// 0x4
static inline constexpr AnmVMCreationFlags UI_LIST_BACK = { .list_type = UiListBack };
// 0x6
static inline constexpr AnmVMCreationFlags UI_LIST_FRONT = { .list_type = UiListFront };

enum AnmVMState {
    Normal = 0,
    MarkedForDelete,
    Deleted
};

enum AnmBlendMode : uint8_t {

};

enum AnmUVMode : uint8_t {
    Wrap = 0,
    Clamp = 1,
    Mirror = 2
};

// size: 0x60C
struct AnmVM {
    struct AnmContextVars {
        int32_t int_vars[4]; // 0x0
        float float_vars[4]; // 0x10
        Float3 __float3_20; // 0x20
        int32_t counter_vars[2]; // 0x2C
        float rand_scale; // 0x34
        float rand_angle_scale; // 0x38
        int32_t rand_int_range; // 0x3C
    };
    struct AnmVMData {
        Timer interrupt_return_time; // 0x0
        int32_t interrupt_return_offset; // 0x14
        uint32_t layer; // 0x18
        int32_t slot; // 0x1C
        int32_t slot2; // 0x20
        int32_t sprite_id; // 0x24
        int32_t script_id; // 0x28
        int32_t current_instruction_offset; // 0x2C
        Float3 position; // 0x30
        Float3 rotation; // 0x3C
        Float3 angular_velocity; // 0x48
        Float2 scale; // 0x54
        Float2 scale2; // 0x5C
        Float2 scale_delta; // 0x64
        Float2 uv_scale; // 0x6C
        Float2 sprite_size; // 0x74
        Float2 uv_scroll; // 0x7C
        Float3 anchor_offset; // 0x84
        ZUNInterp<Float3> position_interp; // 0x90
        ZUNInterp<RGB> color_interp; // 0xE8
        ZUNInterp<int32_t> alpha_interp; // 0x140
        ZUNInterp<Float3> rotation_interp; // 0x170
        ZUNInterp<ZUNAngle> angular_velocity_interp; // 0x1C8
        ZUNInterp<Float2> scale_interp; // 0x1F8
        ZUNInterp<Float2> scale2_interp; // 0x23C
        ZUNInterp<Float2> uv_scale_interp; // 0x280
        ZUNInterp<RGB> color2_interp; // 0x2C4
        ZUNInterp<int32_t> alpha2_interp; // 0x31C
        ZUNInterp<float> u_scroll_speed_interp; // 0x34C
        ZUNInterp<float> v_scroll_speed_interp; // 0x37C
        Float2 sprite_uv_quad[4]; // 0x3AC
        Float2 uv_scroll_speed; // 0x3CC
        D3DMATRIXZ __matrix_3D4; // 0x3D4
        D3DMATRIXZ __matrix_414; // 0x414
        D3DMATRIXZ __matrix_454; // 0x454
        int32_t run_interrupt; // 0x494
        int32_t __last_sprite_set_time; // 0x498
        unknown_fields(0x4); // 0x49C
        uint16_t script_id2; // 0x4A0
        unknown_fields(0x2); // 0x4A2
        AnmContextVars current_context_vars; // 0x4A4
        Float3 __position_2; // 0x4E4
        Float3 __render_quad[4]; // 0x4F0
        AnmVMCreationFlags creation_flags; // 0x520
        D3DCOLOR color1; // 0x524
        D3DCOLOR color2; // 0x528
        D3DCOLOR mixed_inherited_color; // 0x52C
        uint8_t font_width; // 0x530
        uint8_t font_height; // 0x531
        probably_padding_bytes(0x2); // 0x532
        union {
            uint32_t flags_low; // 0x534
            struct {
                uint32_t visible : 1; // 1
                uint32_t __unknown_flag_J : 1; // 2
                uint32_t __z_rotation : 1; // 3
                uint32_t __scale_related_A : 1; // 4
                uint32_t : 1; // 5
                uint32_t blend_mode : 4; // 6-9
                uint32_t : 1; // 10
                uint32_t position_mode : 1; // 11
                uint32_t mirror_x : 1; // 12
                uint32_t mirror_y : 1; // 13
                uint32_t : 1; // 14
                uint32_t disable_z_write : 1; // 15
                uint32_t : 1; // 16
                uint32_t __unknown_std_flag_A : 1; // 17
                uint32_t color_mode : 3; // 18-20
                uint32_t : 1; // 21
                uint32_t __unknown_flag_O : 1; // 22
                uint32_t x_anchor_mode : 2; // 23-24
                uint32_t y_anchor_mode : 2; // 25-26
                uint32_t type : 5; // 27-31
                uint32_t : 1; // 32
            };
        };
        union {
            uint32_t flags_high; // 0x538
            struct {
                uint32_t v_scroll_mode : 2; // 1-2
                uint32_t u_scroll_mode : 2; // 3-4
                uint32_t rotation_mode : 3; // 5-7
                uint32_t __vm_state : 2; // 8-9
                uint32_t auto_rotate : 1; // 10
                uint32_t __unknown_flag_T : 1; // 11
                uint32_t slowdown_immune : 1; // 12
                uint32_t use_animation_rng : 1; // 13
                uint32_t nearest_neighbor : 1; // 14
                uint32_t : 1; // 15
                uint32_t __unknown_flag_Q : 1; // 16
                uint32_t __unknown_field_B : 2; // 17-18
                uint32_t __use_root_vm_custom_slowdown : 1; // 19
                uint32_t : 1; // 20
                uint32_t origin_mode : 2; // 21-22
                uint32_t resolution_mode : 3; // 23-25
                uint32_t inherit_rotation : 1; // 26
                uint32_t __unknown_flag_U : 1; // 27
                uint32_t colorize_children : 1; // 28
                uint32_t __unknown_flag_F : 1; // 29
                uint32_t : 1; // 30
                uint32_t enable_camera_fade : 1; // 31
                uint32_t : 1; // 32
            };
        };
        Float2 __float2_53C; // 0x53C
        // 0x544
    };
    static_assert(sizeof(AnmVMData) == 0x544);
    struct AnmVMController {
        AnmID id; // 0x0 (0x544)
        uint32_t fast_id; // 0x4 (0x548)
        Timer script_time; // 0x8 (0x54C)
        Timer __timer_1C; // 0x1C (0x560)
        ZUNList<AnmVM> global_list_node; // 0x30 (0x574)
        ZUNList<AnmVM> child_list_node; // 0x40 (0x584)
        ZUNList<AnmVM> child_list; // 0x50 (0x594)
        ZUNList<AnmVM> destroy_list_node; // 0x60 (0x5A4)
        AnmVM* next_in_layer; // 0x70 (0x5B4)
        AnmVM* prev_in_layer; // 0x74 (0x5B8)
        AnmVM* __root_vm; // 0x78 (0x5BC)
        AnmVM* parent; // 0x7C (0x5C0)
        float slowdown; // 0x80 (0x5C4)
        void* special_data; // 0x84 (0x5C8)
        uint32_t special_data_size; // 0x88 (0x5CC)
        uint32_t on_wait_index; // 0x8C (0x5D0)
        uint32_t on_tick_index; // 0x90 (0x5D4)
        uint32_t on_draw_index; // 0x94 (0x5D8)
        uint32_t on_destroy_index; // 0x98 (0x5DC)
        uint32_t on_interrupt_index; // 0x9C (0x5E0)
        uint32_t on_copy_A_index; // 0xA0 (0x5E4)
        uint32_t on_copy_B_index; // 0xA4 (0x5E8)
        uint32_t on_sprite_lookup_index; // 0xA8 (0x5EC)
        Float3 position; // 0xAC (0x5F0)
        void* associated_entity; // 0xB8 (0x5FC)
        Float3 __rotation_related; // 0xBC (0x600)

        inline void zero_contents() {
            zero_this();
        }
    };
    ValidateStructSize32(0xC8, AnmVMController);

    AnmVMData data; // 0x0
    AnmVMController controller; // 0x544
    // 0x60C

    inline void zero_contents() {
        zero_this();
    }

    AnmVM() {
        this->zero_contents();
        this->data.sprite_id = -1;
        this->data.current_instruction_offset = -1;
    }

    // 0x4894D0
    dllexport gnu_noinline AnmLoaded* thiscall get_anm_loaded() asm_symbol_rel(0x4894D0);

    template <typename L>
    inline AnmVM* search_roots(L&& lambda) {
        AnmVM* search = this;
        while ((search = search->controller.__root_vm) && !lambda(search));
        return search;
    }
    
    // 0x4066B0
    dllexport gnu_noinline void thiscall set_layer(int32_t layer) asm_symbol_rel(0x4066B0) {
        // IDFK how this one was originally structured
        this->data.layer = layer;
        switch (layer) {
            default:
                this->data.origin_mode = 0;
                break;
            case 3 ... 19:
                this->data.origin_mode = 1;
                break;
            case 20 ... 23:
                this->data.origin_mode = 2;
                break;
        }
        switch (layer) {
            case 20 ... 32: case 37 ... 45:
                this->data.resolution_mode = 1;
                break;
        }
    }

    // 0x405BB0
    dllexport gnu_noinline float vectorcall get_root_vm_custom_slowdown() asm_symbol_rel(0x405BB0) {
        return this->search_roots([](AnmVM* vm) static_lambda {
            return vm->data.__use_root_vm_custom_slowdown;
        })->controller.slowdown;
    }

    // 0x405AD0
    dllexport gnu_noinline void thiscall set_position_interp_bezier(int32_t end_time, Float3* initial_pos, Float3* bezier1, Float3* final_pos, Float3* bezier2) asm_symbol_rel(0x405AD0) {
        this->data.position_interp.end_time = end_time;
        this->data.position_interp.mode = 8;
        this->data.position_interp.initial_value = *initial_pos;
        this->data.position_interp.final_value = *final_pos;
        this->data.position_interp.bezier1 = *bezier1;
        this->data.position_interp.bezier2 = *bezier2;
        this->data.position_interp.time.reset();
    }

    // 0x405CE0
    dllexport gnu_noinline float vectorcall get_z_rotation() asm_symbol_rel(0x405CE0) {
        return this->data.rotation.z;
    }

    // 0x406630
    dllexport gnu_noinline void thiscall initialize_alpha_interp(int32_t end_time, int32_t mode, uint8_t initial_alpha, uint8_t final_alpha) asm_symbol_rel(0x406630) {
        this->data.alpha_interp.end_time = end_time;
        this->data.alpha_interp.mode = mode;
        this->data.alpha_interp.initial_value = initial_alpha;
        this->data.alpha_interp.final_value = final_alpha;
        this->data.alpha_interp.bezier1 = 0;
        this->data.alpha_interp.bezier2 = 0;
        this->data.alpha_interp.time.reset();
    }

    // 0x4890B0
    dllexport gnu_noinline AnmVM* thiscall __wtf_child_list_jank_A(int32_t script, uint32_t arg2) asm_symbol_rel(0x4890B0) {
        for (
            ZUNList<AnmVM>* node = &this->controller.child_list;
            node;
            node = node->next
        ) {
            AnmVM* vm = node->data;
            if (vm && vm != this) {
                if (vm->data.script_id2 == script || script == -1) {
                    if (!arg2) {
                        return vm;
                    }
                    --arg2;
                }
                if (vm->controller.child_list.next) {
                    if (AnmVM* vm2 = vm->__wtf_child_list_jank_A(script, arg2)) {
                        return vm2;
                    }
                }
                if (
                    this->data.script_id2 == (uint16_t)-2 &&
                    !node->next
                ) {
                    return node->data;
                }
            }
        }
        return NULL;
    }

    inline float get_custom_slowdown() {
        AnmVM* root_vm = this->controller.__root_vm;
        if (root_vm && this->data.__use_root_vm_custom_slowdown) {
            return this->get_root_vm_custom_slowdown();
        } else {
            return this->controller.slowdown;
        }
    }

    inline void set_custom_slowdown(float slowdown_value) {
        this->controller.slowdown = slowdown_value;
    }

    inline void clear_all() {
        Float3 controller_position = this->controller.position;
        uint32_t vm_fast_id = this->controller.fast_id;
        int32_t vm_layer = this->data.layer;
        this->zero_contents();
        this->controller.position = controller_position;
        this->controller.fast_id = vm_fast_id;
        this->data.layer = vm_layer;
    }

    inline void clear_controller() {
        Float3 controller_position = this->controller.position;
        int32_t vm_layer = this->data.layer;
        uint32_t vm_fast_id = this->controller.fast_id;
        this->controller.zero_contents();
        this->controller.fast_id = vm_fast_id;
        this->data.layer = vm_layer;
        this->controller.position = controller_position;
    }
    
    // 0x406730
    dllexport gnu_noinline void set_controller_position(Float3* position) asm_symbol_rel(0x406730) {
        this->controller.position = *position;
    }

    // 0x41B430
    dllexport gnu_noinline void reset() asm_symbol_rel(0x41B430) {
        this->clear_all();
        this->data.scale = { 1.0f, 1.0f };
        this->data.scale2 = { 1.0f, 1.0f };
        this->data.uv_scale = { 1.0f, 1.0f };
        this->data.color1 = PackD3DCOLOR(255, 255, 255, 255);
        this->data.__matrix_3D4.set_identity();
        this->controller.script_time.default_values();
        this->controller.__timer_1C.default_values();
        this->data.position_interp.end_time = 0;
        //this->data.color_interp.end_time = 0;
        this->data.alpha_interp.end_time = 0;
        this->data.rotation_interp.end_time = 0;
        this->data.angular_velocity_interp.end_time = 0;
        //this->data.scale_interp.end_time = 0;
        this->data.scale2_interp.end_time = 0;
        this->data.uv_scale_interp.end_time = 0;
        this->data.color2_interp.end_time = 0;
        this->data.alpha2_interp.end_time = 0;
        this->data.u_scroll_speed_interp.end_time = 0;
        this->data.v_scroll_speed_interp.end_time = 0;
        this->data.__unknown_field_B = 1;
        this->data.current_context_vars.rand_scale = 1.0f;
        this->data.current_context_vars.rand_angle_scale = PI_f;
        this->data.current_context_vars.rand_int_range = UINT16_MAX + 1;
        // Technically memset 0 isn't guaranteed to assign NULL to pointers
        // according to official C/C++ standards. Instead literal 0 is given
        // special meanings in the context of pointers.
        // Assigning 0 to a pointer will initialize it with the platform-dependent
        // null pointer value, which could be 0x55555555 or some BS on hypothetical
        // exotic platforms. Memset doesn't do this, instead copying the bytes
        // verbatum with no guarantees about pointer contents.
        //
        // TLDR: Even if you memset something to 0, the standard doesn't guarantee
        // pointers will be NULL. ZUN is *technically* following best practice here!
        //
        // It's still stupid though.
        this->controller.__root_vm = NULL;
        this->controller.parent = NULL;
        this->controller.global_list_node.initialize_with(this);
        this->controller.child_list_node.initialize_with(this);
        this->controller.child_list.initialize_with(this);
        this->controller.destroy_list_node.initialize_with(this);
    }

    // 0x41B6F0
    dllexport gnu_noinline void cleanup() asm_symbol_rel(0x41B6F0) {
        if (void* special_data = this->controller.special_data) {
            free(special_data);
        }
        this->controller.special_data = NULL;
        this->controller.special_data_size = 0;
        this->controller.id = 0;
        this->data.current_instruction_offset = -1;
        for (
            AnmVM* vm = this;
            vm->controller.next_in_layer;
            // vm = vm->controller.next_in_layer
        ) {
            vm->controller.id = 0;
        }
    }

    // 0x41B750
    inline ~AnmVM() {
        this->cleanup();
    }

#define declare_on_func(func_name, index_var, func_table, ...) \
    inline int32_t func_name(MACRO_FIRST_EVAL(__VA_ARGS__) MACRO_SECOND_EVAL(__VA_ARGS__,)) { \
        if (uint32_t index = this->controller.index_var) { \
            return (func_table[index])(this __VA_OPT__(,) MACRO_SECOND_EVAL(__VA_ARGS__,)); \
        } else return 0; \
    }

    declare_on_func(run_on_wait, on_wait_index, ANM_ON_WAIT_FUNCS);
    declare_on_func(run_on_tick, on_tick_index, ANM_ON_TICK_FUNCS);
    declare_on_func(run_on_draw, on_draw_index, ANM_ON_DRAW_FUNCS);
    declare_on_func(run_on_destroy, on_destroy_index, ANM_ON_DESTROY_FUNCS);
    declare_on_func(run_on_interrupt, on_interrupt_index, ANM_ON_INTERRUPT_FUNCS, int32_t, interrupt);
    declare_on_func(run_on_copy_A, on_copy_A_index, ANM_ON_COPY_A_FUNCS);
    declare_on_func(run_on_copy_B, on_copy_B_index, ANM_ON_COPY_B_FUNCS);
    declare_on_func(run_on_sprite_lookup, on_sprite_lookup_index, ANM_ON_SPRITE_LOOKUP_FUNCS, int32_t, sprite);

#undef declare_on_func

    inline void interrupt_and_run(int32_t interrupt) {
        this->run_on_interrupt(interrupt);
        this->run_anm();
    }

    // 0x406AD0
    dllexport gnu_noinline static int fastcall __on_create_1(AnmVM* vm, void* arg) asm_symbol_rel(0x406AD0) {

    }

    // 0x404FC0
    dllexport gnu_noinline static int fastcall __on_create_2(AnmVM* vm, void* arg) asm_symbol_rel(0x404FC0) {

    }

    // 0x405D70
    dllexport gnu_noinline static int fastcall __on_create_3(AnmVM* vm, void* arg) asm_symbol_rel(0x405D70) {

    }

    // 0x4062B0
    dllexport gnu_noinline static int fastcall __on_create_4(AnmVM* vm, void* arg) asm_symbol_rel(0x4062B0) {

    }

    // 0x407590
    dllexport gnu_noinline static int fastcall __on_create_5(AnmVM* vm, void* arg) asm_symbol_rel(0x407590) {

    }
    
    // 0x488FA0
    dllexport void thiscall set_sprite(int32_t sprite_id) asm_symbol_rel(0x488FA0);

    // 0x412F20
    dllexport inline void thiscall interrupt(int32_t interrupt_index) asm_symbol_rel(0x412F20) {
        this->run_on_interrupt(interrupt_index);
        this->data.run_interrupt = interrupt_index;
    }

    inline AnmInstruction* get_current_instruction();

    // 0x47D8B0
    dllexport gnu_noinline void vectorcall set_scale_delta(float x, float y) asm_symbol_rel(0x47D8B0) {
        this->data.__unknown_flag_U = true;
        this->data.scale_delta.x = x;
        this->data.scale_delta.y = y;
    }
    // 0x47D8D0
    dllexport gnu_noinline void vectorcall set_angular_velocity(float x, float y, float z) asm_symbol_rel(0x47D8D0) {
        this->data.__unknown_flag_U = true;
        this->data.angular_velocity.x = x;
        this->data.angular_velocity.y = y;
        this->data.angular_velocity.z = z;
    }
    
    // 0x43A250
    dllexport float vectorcall get_scaled_sprite_x_size() asm_symbol_rel(0x43A250) {
        return this->data.sprite_size.x * this->data.scale.x;
    }
    
    // 0x43A260
    dllexport float vectorcall get_scaled_sprite_y_size() asm_symbol_rel(0x43A260) {
        return this->data.sprite_size.y * this->data.scale.y;
    }

    // 0x478580
    dllexport gnu_noinline int32_t run_anm() asm_symbol_rel(0x478580);

    // 0x488EF0
    dllexport void __unknown_tree_set_J() asm_symbol_rel(0x488EF0) {
        this->data.__unknown_flag_J = true;
        this->controller.child_list.as_head().for_each([](AnmVM* vm) gnu_always_inline static_lambda {
            clang_noinline vm->__unknown_tree_set_J();
        });
    }

    // 0x488F20
    dllexport void __unknown_tree_clear_J() asm_symbol_rel(0x488F20) {
        this->data.__unknown_flag_J = false;
        this->controller.child_list.as_head().for_each([](AnmVM* vm) gnu_always_inline static_lambda {
            clang_noinline vm->__unknown_tree_clear_J();
        });
    }
    
    // 0x4892E0
    dllexport gnu_noinline void vectorcall set_z_rotation(float value) asm_symbol_rel(0x4892E0) {
        this->data.__z_rotation = true;
        this->data.rotation.z = value;
    }
};
ValidateStructSize32(0x60C, AnmVM);

using AnmVMOnCreateFunc = decltype(AnmVM::__on_create_1);

// size: 0x624
struct FastAnmVM : AnmVM {
    ZUNList<FastAnmVM> fast_node; // 0x60C
    bool alive; // 0x61C
    probably_padding_bytes(0x3); // 0x61D
    uint32_t fast_id; // 0x620
};

typedef struct AnmEntry AnmEntry;

// size: 0x10
struct AnmTexture {
    ZUNMagic magic; // 0x0 (THTX)
    uint16_t __zero; // 0x4
    uint16_t format; // 0x6
    int16_t width; // 0x8
    int16_t height; // 0xA
    uint32_t num_bytes; // 0xC
    unsigned char data[]; // 0x10
};
#pragma region // AnmTexture Validation
ValidateFieldOffset32(0x0, AnmTexture, magic);
ValidateFieldOffset32(0x4, AnmTexture, __zero);
ValidateFieldOffset32(0x6, AnmTexture, format);
ValidateFieldOffset32(0x8, AnmTexture, width);
ValidateFieldOffset32(0xA, AnmTexture, height);
ValidateFieldOffset32(0xC, AnmTexture, num_bytes);
ValidateFieldOffset32(0x10, AnmTexture, data);
ValidateStructSize32(0x10, AnmTexture);
#pragma endregion

// size: 0x18
struct AnmImage {
    LPDIRECT3DTEXTURE9 d3d_texture; // 0x0
    void* file; // 0x4
    int32_t file_size; // 0x8
    uint32_t bytes_per_pixel; // 0xC
    AnmEntry* entry; // 0x10
    union {
        uint32_t flags; // 0x14
        struct {
            uint32_t __unknown_flag_A : 1;
            uint32_t : 31;
        };
    };
    // 0x18
};
#pragma region // AnmImage Validation
ValidateFieldOffset32(0x0, AnmImage, d3d_texture);
ValidateFieldOffset32(0x4, AnmImage, file);
ValidateFieldOffset32(0x8, AnmImage, file_size);
ValidateFieldOffset32(0xC, AnmImage, bytes_per_pixel);
ValidateFieldOffset32(0x10, AnmImage, entry);
ValidateFieldOffset32(0x14, AnmImage, flags);
ValidateStructSize32(0x18, AnmImage);
#pragma endregion

// size: 0x44
struct AnmSprite {
    unknown_fields(0x1C); // 0x0
    float __float_1C; // 0x1C
    float __float_20; // 0x20
    float __uv_related_24[4]; // 0x24
    float __size_y; // 0x34
    float __size_x; // 0x38
    float __float_3C; // 0x3C
    float __float_40; // 0x40
    // 0x44
};
#pragma region // AnmSprite Validation
ValidateFieldOffset32(0x1C, AnmSprite, __float_1C);
ValidateFieldOffset32(0x20, AnmSprite, __float_20);
ValidateFieldOffset32(0x24, AnmSprite, __uv_related_24);
ValidateFieldOffset32(0x34, AnmSprite, __size_y);
ValidateFieldOffset32(0x38, AnmSprite, __size_x);
ValidateFieldOffset32(0x3C, AnmSprite, __float_3C);
ValidateFieldOffset32(0x40, AnmSprite, __float_40);
ValidateStructSize32(0x44, AnmSprite);
#pragma endregion

// size: 0x40+
struct AnmEntry {
    uint32_t version; // 0x0
    uint16_t sprite_count; // 0x4
    uint16_t script_count; // 0x6
    uint16_t __word_8; // 0x8
    uint16_t width; // 0xA
    uint16_t height; // 0xC
    uint16_t format; // 0xE
    uint32_t image_path_offset; // 0x10
    int16_t offset_x; // 0x14
    int16_t offset_y; // 0x16
    uint32_t memory_priority; // 0x18
    uint32_t texture_data_offset; // 0x1C
    bool has_data; // 0x20
    unknown_fields(0x1); // 0x21
    bool low_res_scale; // 0x22
    unknown_fields(0x1); // 0x23
    uint32_t offset_to_next; // 0x24
    padding_bytes(0x18); // 0x28
    unsigned char data[]; // 0x40
};
#pragma region // AnmEntry Validation
ValidateFieldOffset32(0x0, AnmEntry, version);
ValidateFieldOffset32(0x4, AnmEntry, sprite_count);
ValidateFieldOffset32(0x6, AnmEntry, script_count);
ValidateFieldOffset32(0x8, AnmEntry, __word_8);
ValidateFieldOffset32(0xA, AnmEntry, width);
ValidateFieldOffset32(0xC, AnmEntry, height);
ValidateFieldOffset32(0xE, AnmEntry, format);
ValidateFieldOffset32(0x10, AnmEntry, image_path_offset);
ValidateFieldOffset32(0x14, AnmEntry, offset_x);
ValidateFieldOffset32(0x16, AnmEntry, offset_y);
ValidateFieldOffset32(0x18, AnmEntry, memory_priority);
ValidateFieldOffset32(0x1C, AnmEntry, texture_data_offset);
ValidateFieldOffset32(0x20, AnmEntry, has_data);
ValidateFieldOffset32(0x22, AnmEntry, low_res_scale);
ValidateFieldOffset32(0x24, AnmEntry, offset_to_next);
ValidateFieldOffset32(0x40, AnmEntry, data);
ValidateStructSize32(0x40, AnmEntry);
#pragma endregion

inline const AnmOnFunc ANM_ON_WAIT_FUNCS[] = { NULL, NULL };
//inline const AnmOnFunc ANM_ON_TICK_FUNCS[] = { NULL, NULL };
//inline const AnmOnFunc ANM_ON_DRAW_FUNCS[] = { NULL, NULL };
//inline const AnmOnFunc ANM_ON_DESTROY_FUNCS[] = { NULL, NULL };
//inline const AnmOnFuncArg ANM_ON_INTERRUPT_FUNCS[] = { NULL, NULL };
inline const AnmOnFunc ANM_ON_COPY_A_FUNCS[] = { NULL, NULL };
inline const AnmOnFunc ANM_ON_COPY_B_FUNCS[] = { NULL, NULL };
//inline const AnmOnFuncArg ANM_ON_SPRITE_LOOKUP_FUNCS[] = { NULL, NULL };

enum AnmListEnd {
    AnmListBack,
    AnmListFront
};

// 0x438D40
dllexport AnmLoaded* stdcall anm_file_lookup(int32_t file_index);

// size: 0x13C
struct AnmLoaded {
    int32_t slot_index; // 0x0
    char anm_path[MAX_PATH]; // 0x4
    void* anm_file; // 0x108
    AnmVM (*__vm_array)[]; // 0x10C
    int32_t entry_count; // 0x110
    int32_t script_count; // 0x114
    int32_t sprite_count; // 0x118
    AnmSprite* sprites; // 0x11C
    AnmInstruction* (*scripts)[]; // 0x120
    AnmImage* images; // 0x124
    int32_t __load_wait; // 0x128
    unknown_fields(0x4); // 0x12C
    int32_t total_image_sizes; // 0x130
    int32_t __counter_134; // 0x134
    unknown_fields(0x4); // 0x138
    // 0x13C

    inline void zero_contents() {
        zero_this();
    }

    inline AnmLoaded() {
        this->zero_contents();
    }

    // 0x47D8F0
    dllexport gnu_noinline AnmInstruction* thiscall get_script(int32_t index) asm_symbol_rel(0x47D8F0) {
        return (*this->scripts)[index];
    }

    // 0x477B00
    dllexport gnu_noinline ZUNResult thiscall set_sprite(AnmVM* vm, int32_t sprite_id) asm_symbol_rel(0x477B00) {
        if (!this->anm_file) {
            return ZUN_ERROR;
        }
        vm->data.sprite_id = sprite_id;
        AnmSprite* sprite = &this->sprites[sprite_id];
        vm->data.sprite_uv_quad[2].x = sprite->__uv_related_24[0];
        vm->data.sprite_uv_quad[0].x = sprite->__uv_related_24[0];
        vm->data.sprite_uv_quad[3].x = sprite->__uv_related_24[1];
        vm->data.sprite_uv_quad[1].x = sprite->__uv_related_24[1];
        vm->data.sprite_uv_quad[1].y = sprite->__uv_related_24[2];
        vm->data.sprite_uv_quad[0].y = sprite->__uv_related_24[2];
        vm->data.sprite_uv_quad[3].y = sprite->__uv_related_24[3];
        vm->data.sprite_uv_quad[2].y = sprite->__uv_related_24[3];
        
        float sizeX = sprite->__size_x;
        vm->data.sprite_size.x = sizeX;
        float sizeY = sprite->__size_y;
        vm->data.sprite_size.y = sizeY;

        constexpr float scale_factor = 1.0f / 256.0f;
        vm->data.__matrix_3D4.set_scaled(
            sizeX * scale_factor,
            sizeY * scale_factor
        );
        vm->data.__matrix_454.set_scaled(
            sizeX / sprite->__float_20 * sprite->__float_3C,
            sizeY / sprite->__float_1C * sprite->__float_40
        );
        vm->data.__matrix_414 = vm->data.__matrix_3D4;

        return ZUN_SUCCESS;
    }

    // Definitely exists, found at 0x40CEF0 in DDC
    inline ZUNResult thiscall __set_initial_sprite(AnmVM* vm, int32_t sprite_id) {
        vm->reset();
        vm->data.slot2 = this->slot_index;
        return this->set_sprite(vm, sprite_id);
    }

    inline void __prepare_vm_data(AnmVM* vm, int32_t script_id);

    inline void __prepare_vm(AnmVM* vm, int32_t script_id);

    // 0x477D60
    dllexport void __sub_477D60(AnmVM* vm, int32_t sprite_id) asm_symbol_rel(0x477D60);

    // 0x407420
    dllexport gnu_noinline void __copy_data_to_vm(AnmVM* vm, int32_t index) asm_symbol_rel(0x407420) {
        vm->clear_controller();
        vm->controller.global_list_node.initialize_with(vm);
        vm->controller.child_list_node.initialize_with(vm);
        vm->controller.child_list.initialize_with(vm);
        rep_movsd(&vm->data, &(*this->__vm_array)[index].data, sizeof(AnmVM::AnmVMData) / sizeof(DWORD));
        vm->controller.__timer_1C.reset();
        vm->controller.script_time.reset();
    }

    // 0x41F920
    dllexport gnu_noinline int32_t __copy_data_to_vm_and_run(AnmVM* vm, int32_t index) asm_symbol_rel(0x41F920) {
        this->__copy_data_to_vm(vm, index);
        vm->controller.parent = NULL;
        vm->controller.__root_vm = NULL;
        return vm->run_anm();
    }

    // 0x4894E0
    dllexport gnu_noinline void thiscall __copy_data_to_vm_unknown_A(AnmVM* vm) asm_symbol_rel(0x4894E0) {
        Float3 position = vm->data.position;
        this->__copy_data_to_vm(vm, 7);
        vm->data.position = position;
        vm->controller.__timer_1C.reset();
        vm->controller.script_time.reset();
    }

private:
    // 0x488770
    dllexport AnmID& thiscall instantiate_vm(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer, AnmVMCreationFlags flags, AnmVM** raw_out) asm_symbol_rel(0x488770);
public:
    inline AnmID instantiate_vm(int32_t script_index, Float3* position, float z_rotation, int32_t layer, AnmVMCreationFlags flags, AnmVM** raw_out) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm(dummy, script_index, position, z_rotation, layer, flags, raw_out);
    }

    inline AnmID instantiate_vm_to_world_list_back(int32_t script_index) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_back(dummy, script_index, -1, nullptr);
    }

private:
    inline AnmID& thiscall instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position);
public:
    inline AnmID instantiate_vm_to_world_list_back(int32_t script_index, Float3* position) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_back(dummy, script_index, position);
    }

private:
    inline AnmID& thiscall instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position, int32_t layer);
public:
    inline AnmID instantiate_vm_to_world_list_back(int32_t script_index, Float3* position, int32_t layer) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_back(dummy, script_index, position, layer);
    }

private:
    // 0x405BF0
    dllexport AnmID& thiscall instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, int32_t layer, AnmVM** raw_out) asm_symbol_rel(0x405BF0);
public:
    inline AnmID instantiate_vm_to_world_list_back(int32_t script_index, int32_t layer, AnmVM** raw_out) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_back(dummy, script_index, layer, raw_out);
    }

private:
    inline AnmID& thiscall instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer);
public:
    inline AnmID instantiate_vm_to_world_list_back(int32_t script_index, Float3* position, float z_rotation, int32_t layer) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_back(dummy, script_index, position, z_rotation, layer);
    }

private:
    // 0x413180
    dllexport AnmID& thiscall instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer, AnmVM** raw_out) asm_symbol_rel(0x413180);
public:
    inline AnmID instantiate_vm_to_world_list_back(int32_t script_index, Float3* position, float z_rotation, int32_t layer, AnmVM** raw_out) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_back(dummy, script_index, position, z_rotation, layer, raw_out);
    }

private:
    // 0x43A030
    dllexport AnmID& thiscall instantiate_vm_to_world_list_front(AnmID& out, int32_t script_index, int32_t layer, UNUSED_ARG(AnmVMCreationFlags flags)) asm_symbol_rel(0x43A030);
public:
    inline AnmID instantiate_vm_to_world_list_front(int32_t script_index, int32_t layer, AnmVMCreationFlags flags = GARBAGE_ARG(AnmVMCreationFlags)) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_front(dummy, script_index, layer, flags);
    }

private:
    // 0x43A110
    dllexport AnmID& thiscall instantiate_vm_to_world_list_front(AnmID& out, int32_t script_index, Float3* position, float z_rotation, UNUSED_ARG(int32_t layer), UNUSED_ARG(AnmVMCreationFlags flags)) asm_symbol_rel(0x43A110);
public:
    inline AnmID instantiate_vm_to_world_list_front(int32_t script_index, Float3* position, float z_rotation, int32_t layer = -1, AnmVMCreationFlags flags = {}) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_world_list_front(dummy, script_index, position, z_rotation, layer, flags);
    }

private:
    // 0x416A10
    dllexport AnmID& thiscall instantiate_vm_to_ui_list_back(AnmID& out, int32_t script_index, UNUSED_ARG(AnmVMCreationFlags flags)) asm_symbol_rel(0x416A10);
public:
    inline AnmID instantiate_vm_to_ui_list_back(int32_t script_index) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_ui_list_back(dummy, script_index, GARBAGE_ARG(AnmVMCreationFlags));
    }

private:
    // 0x409670
    dllexport AnmID& thiscall instantiate_vm_to_ui_list_back(AnmID& out, int32_t script_index, Float3* position, UNUSED_ARG(AnmVMCreationFlags flags)) asm_symbol_rel(0x409670);
public:
    inline AnmID instantiate_vm_to_ui_list_back(int32_t script_index, Float3* position) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_ui_list_back(dummy, script_index, position, GARBAGE_ARG(AnmVMCreationFlags));
    }

private:
    // 0x409590
    dllexport AnmID& thiscall instantiate_vm_to_ui_list_front(AnmID& out, int32_t script_index, Float3* position, UNUSED_ARG(AnmVMCreationFlags flags)) asm_symbol_rel(0x409590);
public:
    inline AnmID instantiate_vm_to_ui_list_front(int32_t script_index, Float3* position) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_vm_to_ui_list_front(dummy, script_index, position, GARBAGE_ARG(AnmVMCreationFlags));
    }

private:
    // 0x4888F0
    dllexport AnmID& thiscall instantiate_child_vm(AnmID& out, int32_t script_index, AnmVM* parent, AnmVMCreationFlags flags) asm_symbol_rel(0x4888F0);
public:
    inline AnmID instantiate_child_vm(int32_t script_index, AnmVM* parent, AnmVMCreationFlags flags) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_child_vm(dummy, script_index, parent, flags);
    }

private:
    // 0x488A40
    dllexport AnmID& thiscall instantiate_orphan_vm_to_world_list_back(AnmID& out, int32_t script_index, AnmVM* parent, UNUSED_ARG(AnmVMCreationFlags flags)) asm_symbol_rel(0x488A40);
public:
    inline AnmID instantiate_orphan_vm_to_world_list_back(int32_t script_index, AnmVM* parent, AnmVMCreationFlags flags = {}) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_orphan_vm_to_world_list_back(dummy, script_index, parent, flags);
    }

    // 0x4865B0
    dllexport gnu_noinline ZUNResult thiscall preload(const char* filename) asm_symbol_rel(0x4865B0) {
        char path[MAX_PATH + 1];
        sprintf(path, "%s", filename);
        void* anm_file = read_file_to_buffer(path, NULL, false);
        if (!anm_file) {
            return ZUN_ERROR;
        }
        this->anm_file = anm_file;
        byteloop_strcpy(this->anm_path, filename);
        AnmEntry* current_entry = (AnmEntry*)anm_file;
        int32_t entry_count = 1;
        int32_t total_script_count = current_entry->script_count;
        int32_t total_sprite_count = current_entry->sprite_count;
        while (uint32_t offset_to_next = current_entry->offset_to_next) {
            current_entry = based_pointer(current_entry, offset_to_next);
            ++entry_count;
            total_script_count += current_entry->script_count;
            total_sprite_count += current_entry->sprite_count;
        }
        this->entry_count = entry_count;

        size_t image_array_size = sizeof(AnmImage) * entry_count;
        AnmImage* images = (AnmImage*)malloc(image_array_size);
        this->images = images;
        memset(images, 0, image_array_size);

        this->sprites = (AnmSprite*)malloc(sizeof(AnmSprite[total_sprite_count]));
        this->scripts = (AnmInstruction*(*)[])malloc(sizeof(AnmInstruction*[total_script_count]));
        this->script_count = total_script_count;
        this->sprite_count = total_sprite_count;

        current_entry = (AnmEntry*)anm_file;
        
        for (int32_t entry_index = 0;;) {
            if (expect(!current_entry, false)) {
                LOG_BUFFER.write_error(JpEnStr("", "Can't load animation. data is lost or corrupted\r\n"));
                break;
            }
            if (expect(current_entry->version != 8, false)) {
                LOG_BUFFER.write_error(JpEnStr("", "different version of animation\r\n"));
                break;
            }
            if (!current_entry->has_data) {
                const char* image_filename = based_pointer<const char>(current_entry, current_entry->image_path_offset);
                if (image_filename[0] != '@') {
                    char image_path[MAX_PATH];
                    sprintf(image_path, "%s", image_filename);
                    int32_t image_file_size;
                    void* image_file = read_file_to_buffer(image_path, &image_file_size, true);
                    if (expect(!image_file, false)) {
                        LOG_BUFFER.write_error(JpEnStr("", "Unable to load texture %s. data is lost or corrupted\r\n"), image_filename);
                        break;
                    }
                    this->images[entry_index].file_size = image_file_size;
                    this->images[entry_index].file = image_file;
                }
            }
            ++entry_index;
            size_t offset_to_next = current_entry->offset_to_next;
            if (expect(!offset_to_next, false)) {
                size_t vm_array_size = sizeof(AnmVM) * total_script_count;
                auto vm_array = (AnmVM(*)[])malloc(vm_array_size);
                this->__vm_array = vm_array;
                memset(vm_array, 0, vm_array_size);
                break;
            }
            current_entry = based_pointer(current_entry, offset_to_next);
        }
        return ZUN_SUCCESS;
    }
};

// size: 0x14
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE (0x44)
struct PrimitiveVertex {
    Float4 position; // 0x0
    D3DCOLOR diffuse; // 0x10
    // 0x14

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
};
// size: 0x14
// D3DFVF_XYZ | D3DFVF_TEX1 (0x102)
struct UnknownVertexA {
    Float3 position; // 0x0
    Float2 texture_uv; // 0xC
    // 0x14

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZ | D3DFVF_TEX1;
};
// size: 0x18
// D3DFVF_XYZRHW | D3DFVF_TEX1 (0x104)
struct SpriteVertexB {
    Float4 position; // 0x0
    Float2 texture_uv; // 0x10
    // 0x18

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_TEX1;
};
// size: 0x18
// D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 (0x142)
struct SpriteVertexC {
    Float3 position; // 0x0
    D3DCOLOR diffuse; // 0xC
    Float2 texture_uv; // 0x10
    // 0x18

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};
// size: 0x1C
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 (0x144)
struct SpriteVertex {
    Float4 position; // 0x0
    D3DCOLOR diffuse; // 0x10
    Float2 texture_uv; // 0x14
    // 0x1C

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};
// size: 0x10
// D3DFVF_XYZ | D3DFVF_DIFFUSE (0x42)
struct UnknownVertexB {
    Float3 position; // 0x0
    D3DCOLOR diffuse; // 0xC
    // 0x10

    static constexpr DWORD FVF_TYPE = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

extern "C" {
    // 0x51F65C
    extern AnmManager* ANM_MANAGER_PTR asm("_ANM_MANAGER_PTR");
    // 0x5704C0
    extern SpriteVertexB SPRITE_VERTEX_BUFFER_B[4] asm("_SPRITE_VERTEX_BUFFER_B");
    // 0x570520
    extern SpriteVertex SPRITE_VERTEX_BUFFER_A[4] asm("_SPRITE_VERTEX_BUFFER_A");
    // 0x570590
    extern SpriteVertexC SPRITE_VERTEX_BUFFER_C[4] asm("_SPRITE_VERTEX_BUFFER_C");
}

enum AnmFileIndex {
    TEXT_ANM_INDEX = 0,
    SIG_ANM_INDEX = 1,
    ASCII_ANM_INDEX = 2,
    STAGE_ANM_INDEX_A = 3,
    STAGE_ANM_INDEX_B = 4,
    FRONT_ANM_INDEX = 5,
    STAGE_LOGO_ANM_INDEX = 6,
    BULLET_ANM_INDEX = 7,
    EFFECT_ANM_INDEX = 8,
    PLAYER_ANM_INDEX = 9,

    TITLE_ANM_INDEX = 16,
    TITLEV_ANM_INDEX = 17,

    HELP_ANM_INDEX = 19,

    TROPHY_ANM_INDEX = 27,

    ABILITY_ANM_INDEX = 30,
    ABCARD_ANM_INDEX = 31,
    ABMENU_ANM_INDEX = 32,


    ENUM_MAX_VALUE_DECLARE(CmdType)
};

// 0x4B4920
static inline constexpr int D3DFORMAT_SIZES_TABLE[] = {
    4,
    sizeof(PixelA8R8G8B8), // 4
    sizeof(PixelA1R5G5B5), // 2
    sizeof(PixelR5G6B5), // 2
    sizeof(PixelR8G8B8), // 3
    sizeof(PixelA4R4G4B4), // 2
    sizeof(PixelA8R3G3B2), // 2
    sizeof(PixelA8), // 1
    sizeof(PixelR3G3B2) // 1
};

// 0x4B4944
static inline constexpr D3DFORMAT D3DFORMAT_TABLE[] = {
    D3DFMT_UNKNOWN,
    D3DFMT_A8R8G8B8,
    D3DFMT_A1R5G5B5,
    D3DFMT_R5G6B5,
    D3DFMT_R8G8B8,
    D3DFMT_A4R4G4B4,
    D3DFMT_A8R3G3B2,
    D3DFMT_A8,
    D3DFMT_R3G3B2
};

// size: 0x39724B8
struct AnmManager {

#define WORLD_LAYER_A_COUNT 24
#define WORLD_LAYER_B_COUNT 13
#define WORLD_LAYER_COUNT (WORLD_LAYER_A_COUNT + WORLD_LAYER_B_COUNT)
#define UI_LAYER_COUNT 9

    ZUNThreadB __thread_0; // 0x0
    unknown_fields(0x4); // 0x1C
    int32_t __int_20; // 0x20
    unknown_fields(0x24); // 0x24
    int __dword_48; // 0x48
    unknown_fields(0x24); // 0x4C
    int __dword_70; // 0x70
    unknown_fields(0x24); // 0x74
    int __dword_98; // 0x98
    unknown_fields(0x24); // 0x9C
    int __dword_C0; // 0xC0
    int __dword_C4; // 0xC4
    int __dword_C8; // 0xC8
    int __dword_CC; // 0xCC
    Int2 __int2_D0; // 0xD0
    Float2 __float2_D8; // 0xD8
    unknown_fields(0x4); // 0xE0
    AnmVM __vm_E4; // 0xE4
    ZUNListEnds<AnmVM> world_list; // 0x6F0
    ZUNListEnds<AnmVM> ui_list; // 0x6F8
    FastAnmVM fast_array[uint_width_max(ANM_FAST_ID_BITS)]; // 0x700
    FastAnmVM fast_array_end; // 0x31200DC
    int32_t next_snapshot_fast_id; // 0x3120700
    int32_t next_snapshot_discriminator; // 0x3120704
    ZUNListHead<AnmVM> snapshot_list_head; // 0x3120708
    ZUNListHead<FastAnmVM> free_list_head; // 0x3120718
    unknown_fields(0x4); // 0x3120728
    AnmLoaded* loaded_anm_files[33]; // 0x312072C
    D3DMATRIXZ __matrix_31207B0; // 0x31207B0
    AnmVM __vm_31207F0; // 0x31207F0
    unknown_fields(0x4); // 0x3120DFC
    D3DCOLOR current_texture_blend_color; // 0x3120E00
    int32_t __index_3120E04; // 0x3120E04
    AnmBlendMode current_blend_mode; // 0x3120E08
    char __byte_3120E09; // 0x3120E09
    int8_t __sbyte_3120E0A; // 0x3120E0A
    char __byte_3120E0B; // 0x3120E0B
    char __byte_3120E0C; // 0x3120E0C
    unknown_fields(0x1); // 0x3120E0D
    bool currently_using_point_filtering; // 0x3120E0E
    bool currently_using_modulate_op; // 0x3120E0F
    AnmUVMode current_u_sample_mode; // 0x3120E10
    AnmUVMode current_v_sample_mode; // 0x3120E11
    probably_padding_bytes(0x2); // 0x3120E12
    int __current_dword_3120E14; // 0x3120E14
    LPDIRECT3DVERTEXBUFFER9 __d3d_vertex_buffer_3120E18; // 0x3120E18
    UnknownVertexA __vertex_array_3120E1C[4]; // 0x3120E1C
    int32_t unrendered_sprite_count; // 0x3120E6C
    SpriteVertex sprite_vertex_data[0x40000]; // 0x3120E70
    SpriteVertex* sprite_write_cursor; // 0x3820E70
    SpriteVertex* sprite_render_cursor; // 0x3820E74
    int32_t unrendered_primitive_count; // 0x3820E78
    PrimitiveVertex primitive_vertex_data[0x10000]; // 0x3820E7C
    PrimitiveVertex* primitive_write_cursor; // 0x3960E7C
    PrimitiveVertex* primitive_render_cursor; // 0x3960E80
    AnmVM layer_heads[WORLD_LAYER_COUNT + UI_LAYER_COUNT]; // 0x3960E84
    uint32_t prev_slow_id; // 0x39724AC
    D3DCOLOR __color_39724B0; // 0x39724B0
    unknown_fields(0x4); // 0x39724B4
    // 0x39724B8

    inline void zero_contents() {
        zero_this();
    }

    inline static void set_modulate_op();

    // 0x47E6E0
    dllexport gnu_noinline static void reset_vertex_buffers() asm_symbol_rel(0x47E6E0) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        anm_manager->unrendered_sprite_count = 0;
        anm_manager->sprite_write_cursor = anm_manager->sprite_vertex_data;
        anm_manager->sprite_render_cursor = anm_manager->sprite_vertex_data;
        anm_manager->unrendered_primitive_count = 0;
        anm_manager->primitive_write_cursor = anm_manager->primitive_vertex_data;
        anm_manager->primitive_render_cursor = anm_manager->primitive_vertex_data;
    }

    // 0x47E730
    dllexport gnu_noinline void thiscall flush_sprites() asm_symbol_rel(0x47E730) {
        if (this->unrendered_sprite_count) {
            this->set_modulate_op();
            SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
            SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            SUPERVISOR.d3d_device->SetFVF(SpriteVertex::FVF_TYPE);
            SUPERVISOR.d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, this->unrendered_sprite_count * 2, this->sprite_render_cursor, sizeof(SpriteVertex));
            this->sprite_render_cursor = this->sprite_write_cursor;
            ++this->__dword_CC;
            this->unrendered_sprite_count = 0;
        }
    }

    // 0x481210
    dllexport gnu_noinline int32_t thiscall draw_vm(AnmVM* vm) asm_symbol_rel(0x481210) {

    }

    // 0x488260
    dllexport gnu_noinline UpdateFuncRet thiscall render_layer(uint32_t layer_index) asm_symbol_rel(0x488260) {
        CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
        {
            this->world_list.for_each_safeB([&, this](AnmVM* vm) {
                if (!vm->data.__vm_state) {
                    uint32_t vm_layer = vm->data.layer;
                    vm_layer = (vm_layer - WORLD_LAYER_COUNT) >= UI_LAYER_COUNT ? vm_layer : vm_layer - WORLD_LAYER_B_COUNT;
                    if (vm_layer == layer_index) {
                        this->draw_vm(vm);
                        ++vm->data.position_interp.end_time;
                    }
                }
            });
            this->ui_list.for_each_safeB([=](AnmVM* vm) {
                if (!vm->data.__vm_state) {
                    int32_t vm_layer = vm->data.layer;
                    if ((uint32_t)(vm_layer - WORLD_LAYER_A_COUNT) < UI_LAYER_COUNT) {
                        vm_layer += WORLD_LAYER_B_COUNT;
                    }
                    else if (
                        vm_layer < WORLD_LAYER_COUNT ||
                        vm_layer >= (WORLD_LAYER_COUNT + UI_LAYER_COUNT)
                    ) {
                        vm_layer = 39;
                    }
                    if (vm_layer == layer_index) {
                        this->draw_vm(vm);
                        ++vm->data.position_interp.end_time;
                    }
                }
            });
        }
        CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
        return UpdateFuncNext;
    }

    // 0x488220
    dllexport static gnu_noinline UpdateFuncRet fastcall pre_tick_world(void* self) asm_symbol_rel(0x488220) {
    }
    // 0x488250
    dllexport static gnu_noinline UpdateFuncRet fastcall pre_tick_ui(void* self) asm_symbol_rel(0x488250) {
    }

#define simple_draw_layer(address, index) \
    /* // address */ dllexport static gnu_noinline UpdateFuncRet fastcall MACRO_CAT(draw_layer_, index)(void* self) asm_symbol_rel(address) { return ((AnmManager*)self)->render_layer(index); }

    /* // 0x487670 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_0(void* self) asm_symbol_rel(0x487670) { return ((AnmManager*)self)->render_layer(0); }
    /* // 0x487680 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_1(void* self) asm_symbol_rel(0x487680) { return ((AnmManager*)self)->render_layer(1); }
    /* // 0x487690 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_2(void* self) asm_symbol_rel(0x487690) { return ((AnmManager*)self)->render_layer(2); }
    /* // 0x4876A0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_4(void* self) asm_symbol_rel(0x4876A0) { return ((AnmManager*)self)->render_layer(4); }
    // 0x4876B0
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_3(void* self) asm_symbol_rel(0x4876B0) {
        Supervisor::__sub_454950(&SUPERVISOR.cameras[3]);
        SUPERVISOR.set_camera_by_index_disable_fog(3);
        return ((AnmManager*)self)->render_layer(3);
    }
    /* // 0x487770 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_5(void* self) asm_symbol_rel(0x487770) { return ((AnmManager*)self)->render_layer(5); }
    /* // 0x487780 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_6(void* self) asm_symbol_rel(0x487780) { return ((AnmManager*)self)->render_layer(6); }
    /* // 0x487790 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_7(void* self) asm_symbol_rel(0x487790) { return ((AnmManager*)self)->render_layer(7); }
    /* // 0x4877A0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_8(void* self) asm_symbol_rel(0x4877A0) { return ((AnmManager*)self)->render_layer(8); }
    /* // 0x4877B0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_9(void* self) asm_symbol_rel(0x4877B0) { return ((AnmManager*)self)->render_layer(9); }
    /* // 0x4877C0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_10(void* self) asm_symbol_rel(0x4877C0) { return ((AnmManager*)self)->render_layer(10); }
    /* // 0x4877D0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_11(void* self) asm_symbol_rel(0x4877D0) { return ((AnmManager*)self)->render_layer(11); }
    /* // 0x4877E0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_13(void* self) asm_symbol_rel(0x4877E0) { return ((AnmManager*)self)->render_layer(13); }
    /* // 0x4877F0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_14(void* self) asm_symbol_rel(0x4877F0) { return ((AnmManager*)self)->render_layer(14); }
    /* // 0x487800 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_15(void* self) asm_symbol_rel(0x487800) { return ((AnmManager*)self)->render_layer(15); }
    /* // 0x487810 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_16(void* self) asm_symbol_rel(0x487810) { return ((AnmManager*)self)->render_layer(16); }
    /* // 0x487820 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_17(void* self) asm_symbol_rel(0x487820) { return ((AnmManager*)self)->render_layer(17); }
    /* // 0x487830 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_18(void* self) asm_symbol_rel(0x487830) { return ((AnmManager*)self)->render_layer(18); }
    /* // 0x487840 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_12(void* self) asm_symbol_rel(0x487840) { return ((AnmManager*)self)->render_layer(12); }
    /* // 0x487850 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_19(void* self) asm_symbol_rel(0x487850) { return ((AnmManager*)self)->render_layer(19); }
    /* // 0x487860 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_21(void* self) asm_symbol_rel(0x487860) { return ((AnmManager*)self)->render_layer(21); }
    /* // 0x487870 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_22(void* self) asm_symbol_rel(0x487870) { return ((AnmManager*)self)->render_layer(22); }
    /* // 0x487880 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_31(void* self) asm_symbol_rel(0x487880) { return ((AnmManager*)self)->render_layer(31); }
    /* // 0x487890 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_32(void* self) asm_symbol_rel(0x487890) { return ((AnmManager*)self)->render_layer(32); }
    /* // 0x4878A0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_30(void* self) asm_symbol_rel(0x4878A0) { return ((AnmManager*)self)->render_layer(30); }
    /* // 0x4878B0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_26(void* self) asm_symbol_rel(0x4878B0) { return ((AnmManager*)self)->render_layer(26); }
    /* // 0x4878C0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_27(void* self) asm_symbol_rel(0x4878C0) { return ((AnmManager*)self)->render_layer(27); }
    /* // 0x4878D0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_25(void* self) asm_symbol_rel(0x4878D0) { return ((AnmManager*)self)->render_layer(25); }
    /* // 0x4878E0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_38(void* self) asm_symbol_rel(0x4878E0) { return ((AnmManager*)self)->render_layer(38); }
    /* // 0x4878F0 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_39(void* self) asm_symbol_rel(0x4878F0) { return ((AnmManager*)self)->render_layer(39); }
    /* // 0x487900 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_40(void* self) asm_symbol_rel(0x487900) { return ((AnmManager*)self)->render_layer(40); }
    /* // 0x487910 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_43(void* self) asm_symbol_rel(0x487910) { return ((AnmManager*)self)->render_layer(43); }
    /* // 0x487920 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_44(void* self) asm_symbol_rel(0x487920) { return ((AnmManager*)self)->render_layer(44); }
    /* // 0x487930 */ dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_45(void* self) asm_symbol_rel(0x487930) { return ((AnmManager*)self)->render_layer(45); }
    // 0x487940
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_20(void* self) asm_symbol_rel(0x487940) {
        SUPERVISOR.set_camera_by_index_disable_zwrite(1);
        return ((AnmManager*)self)->render_layer(20);
    }
    // 0x487A10
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_23(void* self) asm_symbol_rel(0x487A10) {
        SUPERVISOR.set_camera_by_index_disable_zwrite(1);
        return ((AnmManager*)self)->render_layer(23);
    }
    // 0x487AE0
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_24(void* self) asm_symbol_rel(0x487AE0) {
        SUPERVISOR.set_camera_by_index_disable_zwrite(2);
        AnmManager* anm_manager = (AnmManager*)self;
        anm_manager->__int2_D0.x = 0;
        anm_manager->__int2_D0.y = 0;
        return anm_manager->render_layer(24);
    }
    // 0x487BC0
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_28(void* self) asm_symbol_rel(0x487BC0) {
        AnmManager* anm_manager = (AnmManager*)self;
        SUPERVISOR.set_camera2_alt();
        UpdateFuncRet ret = anm_manager->render_layer(28);
        SUPERVISOR.set_camera_by_index(2);
        return ret;
    }
    // 0x487C50
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_29(void* self) asm_symbol_rel(0x487C50) {
        AnmManager* anm_manager = (AnmManager*)self;
        SUPERVISOR.set_camera2_alt();
        UpdateFuncRet ret = anm_manager->render_layer(29);
        SUPERVISOR.set_camera_by_index(2);
        return ret;
    }
    // 0x487CE0
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_37(void* self) asm_symbol_rel(0x487CE0) {
        SUPERVISOR.set_camera_by_index_disable_zwrite(2);
        return ((AnmManager*)self)->render_layer(37);
    }
    // 0x487DB0
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_41A(void* self) asm_symbol_rel(0x487DB0) {
        AnmManager* anm_manager = (AnmManager*)self;
        SUPERVISOR.set_camera2_alt();
        UpdateFuncRet ret = anm_manager->render_layer(41);
        SUPERVISOR.set_camera_by_index(2);
        return ret;
    }
    // Was this supposed to be 42?
    // 0x487E40
    dllexport static gnu_noinline UpdateFuncRet fastcall draw_layer_41B(void* self) asm_symbol_rel(0x487E40) {
        AnmManager* anm_manager = (AnmManager*)self;
        SUPERVISOR.set_camera2_alt();
        UpdateFuncRet ret = anm_manager->render_layer(41);
        SUPERVISOR.set_camera_by_index(2);
        return ret;
    }

    // 0x483820
    dllexport gnu_noinline AnmManager() : prev_slow_id(0) {
        this->zero_contents();
        SPRITE_VERTEX_BUFFER_B[3].position.w = 1.0f;
        SPRITE_VERTEX_BUFFER_B[2].position.w = 1.0f;
        SPRITE_VERTEX_BUFFER_B[1].position.w = 1.0f;
        SPRITE_VERTEX_BUFFER_B[0].position.w = 1.0f;
        
        SPRITE_VERTEX_BUFFER_B[0].texture_uv = { 0.0f, 0.0f };
        SPRITE_VERTEX_BUFFER_B[1].texture_uv = { 1.0f, 0.0f };
        SPRITE_VERTEX_BUFFER_B[2].texture_uv = { 0.0f, 1.0f };
        SPRITE_VERTEX_BUFFER_B[3].texture_uv = { 1.0f, 1.0f };
        
        SPRITE_VERTEX_BUFFER_A[3].position.w = 1.0f;
        SPRITE_VERTEX_BUFFER_A[2].position.w = 1.0f;
        SPRITE_VERTEX_BUFFER_A[1].position.w = 1.0f;
        SPRITE_VERTEX_BUFFER_A[0].position.w = 1.0f;
        
        SPRITE_VERTEX_BUFFER_A[0].texture_uv = { 0.0f, 0.0f };
        SPRITE_VERTEX_BUFFER_A[1].texture_uv = { 1.0f, 0.0f };
        SPRITE_VERTEX_BUFFER_A[2].texture_uv = { 0.0f, 1.0f };
        SPRITE_VERTEX_BUFFER_A[3].texture_uv = { 1.0f, 1.0f };
        
        this->__d3d_vertex_buffer_3120E18 = NULL;
        this->__index_3120E04 = -1;
        this->current_blend_mode = (AnmBlendMode)0;
        this->__byte_3120E09 = 0;
        this->__sbyte_3120E0A = 0;
        this->__byte_3120E0B = 0;
        this->current_texture_blend_color = PackD3DCOLOR(0, 0, 0, 0);
        this->__byte_3120E0C = -1;
        this->__int_20 = -1;
        this->__dword_48 = -1;
        this->__dword_70 = -1;
        this->__dword_98 = -1;
        ZUNListHead<FastAnmVM>* free_list_head_ptr = &this->free_list_head;
        free_list_head_ptr->initialize_with((FastAnmVM*)&free_list_head_ptr);
        FastAnmVM* fast_array_ptr = this->fast_array;
        int32_t i = 0;
        do {
            fast_array_ptr->reset();
            fast_array_ptr->controller.fast_id = i;
            fast_array_ptr->alive = false;
            fast_array_ptr->fast_id = i;
            fast_array_ptr->fast_node.initialize_with(fast_array_ptr);
            free_list_head_ptr->append(&fast_array_ptr->fast_node);
            ++i;
            ++fast_array_ptr;
        } while (i < countof(this->fast_array));
        this->next_snapshot_fast_id = 0;
        UpdateFunc* update_func;
        update_func = new UpdateFunc(&AnmManager::pre_tick_world, true, this);
        UpdateFuncRegistry::register_on_tick(update_func, 34);
        update_func = new UpdateFunc(&AnmManager::pre_tick_ui, true, this);
        UpdateFuncRegistry::register_on_tick(update_func, 11);
        update_func = new UpdateFunc(&AnmManager::draw_layer_0, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 5);
        update_func = new UpdateFunc(&AnmManager::draw_layer_1, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 7);
        update_func = new UpdateFunc(&AnmManager::draw_layer_2, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 9);
        update_func = new UpdateFunc(&AnmManager::draw_layer_4, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 11);
        update_func = new UpdateFunc(&AnmManager::draw_layer_3, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 10);
        update_func = new UpdateFunc(&AnmManager::draw_layer_5, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 13);
        update_func = new UpdateFunc(&AnmManager::draw_layer_6, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 16);
        update_func = new UpdateFunc(&AnmManager::draw_layer_7, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 18);
        update_func = new UpdateFunc(&AnmManager::draw_layer_8, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 20);
        update_func = new UpdateFunc(&AnmManager::draw_layer_9, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 21);
        update_func = new UpdateFunc(&AnmManager::draw_layer_10, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 22);
        update_func = new UpdateFunc(&AnmManager::draw_layer_11, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 24);
        update_func = new UpdateFunc(&AnmManager::draw_layer_12, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 27);
        update_func = new UpdateFunc(&AnmManager::draw_layer_13, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 28);
        update_func = new UpdateFunc(&AnmManager::draw_layer_14, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 31);
        update_func = new UpdateFunc(&AnmManager::draw_layer_15, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 32);
        update_func = new UpdateFunc(&AnmManager::draw_layer_16, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 35);
        update_func = new UpdateFunc(&AnmManager::draw_layer_17, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 37);
        update_func = new UpdateFunc(&AnmManager::draw_layer_18, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 40);
        update_func = new UpdateFunc(&AnmManager::draw_layer_19, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 43);
        update_func = new UpdateFunc(&AnmManager::draw_layer_20, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 46);
        update_func = new UpdateFunc(&AnmManager::draw_layer_21, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 47);
        update_func = new UpdateFunc(&AnmManager::draw_layer_22, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 54);
        update_func = new UpdateFunc(&AnmManager::draw_layer_23, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 56);
        update_func = new UpdateFunc(&AnmManager::draw_layer_26, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 64);
        update_func = new UpdateFunc(&AnmManager::draw_layer_27, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 66);
        update_func = new UpdateFunc(&AnmManager::draw_layer_30, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 84);
        update_func = new UpdateFunc(&AnmManager::draw_layer_31, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 86);
        update_func = new UpdateFunc(&AnmManager::draw_layer_32, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 89);
        update_func = new UpdateFunc(&AnmManager::draw_layer_25, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 61);
        update_func = new UpdateFunc(&AnmManager::draw_layer_24, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 60);
        update_func = new UpdateFunc(&AnmManager::draw_layer_28, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 69);
        update_func = new UpdateFunc(&AnmManager::draw_layer_29, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 71);
        update_func = new UpdateFunc(&AnmManager::draw_layer_37, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 57);
        update_func = new UpdateFunc(&AnmManager::draw_layer_38, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 62);
        update_func = new UpdateFunc(&AnmManager::draw_layer_39, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 65);
        update_func = new UpdateFunc(&AnmManager::draw_layer_40, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 67);
        update_func = new UpdateFunc(&AnmManager::draw_layer_41A, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 68);
        update_func = new UpdateFunc(&AnmManager::draw_layer_41B, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 70);
        update_func = new UpdateFunc(&AnmManager::draw_layer_43, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 85);
        update_func = new UpdateFunc(&AnmManager::draw_layer_44, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 87);
        update_func = new UpdateFunc(&AnmManager::draw_layer_45, true, this);
        UpdateFuncRegistry::register_on_draw(update_func, 90);
        SUPERVISOR.d3d_device->SetVertexShader(NULL);
        this->next_snapshot_discriminator = 0;
    }


    // 0x4867E0
    dllexport gnu_noinline AnmLoaded* thiscall create_anm_loaded(int32_t file_index, const char* filename) asm_symbol_rel(0x4867E0) {
        DebugLogger::__debug_log_stub_6("::preloadAnim : %s\n", filename);
        if (file_index < countof(this->loaded_anm_files)) {
            LOG_BUFFER.write_error("Not enough space to store textures\r\n");
            return NULL;
        }
        AnmLoaded* anm_loaded = new AnmLoaded();
        this->loaded_anm_files[file_index] = anm_loaded;
        anm_loaded->slot_index = file_index;
        if (ZUN_FAILED(anm_loaded->preload(filename))) {
            delete anm_loaded;
            return NULL;
        }
        return anm_loaded;
    }

    // 0x486880
    dllexport static gnu_noinline AnmLoaded* stdcall preload_anm(int32_t file_index, const char* filename) asm_symbol_rel(0x486880) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        if (anm_manager->loaded_anm_files[file_index]) {
            DebugLogger::__debug_log_stub_6("::preloadAnim already : %s\n", filename);
            return anm_manager->loaded_anm_files[file_index];
        }
        AnmLoaded* anm_loaded = anm_manager->create_anm_loaded(file_index, filename);
        if (expect(anm_loaded != NULL, false)) {
            anm_loaded->__load_wait = 1;
            while (anm_loaded->__load_wait && !SUPERVISOR.__unknown_bitfield_A) {
                Sleep(1);
            }
            DebugLogger::__debug_log_stub_6("::preloadAnimEnd : %s\n", filename);
        }
        return anm_loaded;
    }

    // 0x4858E0
    dllexport gnu_noinline void thiscall __screw_with_texture_bits(LPDIRECT3DTEXTURE9 d3d_texture) asm_symbol_rel(0x4858E0) {
        LPDIRECT3DSURFACE9 surface = NULL;
        d3d_texture->GetSurfaceLevel(0, &surface);
        D3DSURFACE_DESC surface_desc;
        surface->GetDesc(&surface_desc);
        D3DLOCKED_RECT rect;
        surface->LockRect(&rect, NULL, 0);
        // Syntax highlighting doesn't understand lambda templates. :/
        auto process_pixels = [&](auto dummy_pixel) {
            using PixelT = decltype(dummy_pixel);
            for (uint32_t y = 0; y < surface_desc.Height; ++y) {
                PixelT* pixel = based_pointer<PixelT>(rect.pBits, rect.Pitch * y);
                for (uint32_t x = 0; x < surface_desc.Width; ++x) {
                    if (!pixel[0].a) {
                        uint32_t count = 0;
                        uint32_t R = 0, G = 0, B = 0;
                        if (x > 0 && pixel[-1].a) {
                            R = pixel[-1].r; G = pixel[-1].g; B = pixel[-1].b;
                            count = 1;
                        }
                        if (x < surface_desc.Width - 1 && pixel[1].a) {
                            R += pixel[1].r; G += pixel[1].g; B += pixel[1].b;
                            ++count;
                        }
                        if (y > 0) {
                            PixelT* prev_row_pixel = pixel - rect.Pitch / sizeof(PixelT);
                            if (prev_row_pixel->a) {
                                R += prev_row_pixel->r; G += prev_row_pixel->g; B += prev_row_pixel->b;
                                ++count;
                            }
                        }
                        if (y < surface_desc.Height - 1) {
                            PixelT* next_row_pixel = pixel - rect.Pitch / sizeof(PixelT);
                            if (next_row_pixel->a) {
                                R += next_row_pixel->r; G += next_row_pixel->g; B += next_row_pixel->b;
                                ++count;
                            }
                        }
                        if (count > 1) {
                            R /= count; G /= count; B /= count;
                        }
                        if constexpr (PixelT::a_is_bitfield) {
                            pixel[0] = (PixelT){
                                .r = R, .g = G, .b = B, .a = 0
                            };
                        } else {
                            PixelT temp = pixel[0];
                            temp.r = R; temp.g = G; temp.b = B;
                            pixel[0] = temp;
                        }
                    }
                    ++pixel;
                }
            }
        };
        switch (surface_desc.Format) {
            case D3DFMT_UNKNOWN: case D3DFMT_A8R8G8B8:
                process_pixels((PixelA8R8G8B8){});
                break;
            case D3DFMT_A1R5G5B5:
                process_pixels((PixelA1R5G5B5){});
                break;
            case D3DFMT_A4R4G4B4:
                process_pixels((PixelA4R4G4B4){});
                break;
            case D3DFMT_A8R3G3B2:
                process_pixels((PixelA8R3G3B2){});
                break;
        }
        surface->UnlockRect();
        surface->Release();
    }

    // 0x486140
    dllexport gnu_noinline int32_t thiscall __sub_486140(AnmImage* image, uint32_t format_index, uint32_t entry_index, int32_t width, int32_t height, int32_t offset_x, int32_t offset_y) asm_symbol_rel(0x486140);

    // 0x486BC0
    dllexport gnu_noinline int32_t thiscall __sub_486BC0(AnmLoaded* anm_loaded, uint32_t entry_index, uint32_t sprite_count, uint32_t script_count, AnmEntry* anm_entry) asm_symbol_rel(0x486BC0);

    // 0x486920
    dllexport gnu_noinline AnmLoaded* thiscall __sub_486920(AnmLoaded* anm_loaded) asm_symbol_rel(0x486920) {
        AnmEntry* anm_entry_ptr = (AnmEntry*)anm_loaded->anm_file;
        int32_t wait_index = 0;
        int32_t load_wait = anm_loaded->__load_wait;
        uint32_t script_count = 0;
        uint32_t sprite_count = 0;
        uint32_t entry_index = 0;
        BOOL idk = false;
        do {
            if (load_wait - 1 == wait_index) {
                if (this->__sub_486BC0(anm_loaded, entry_index, sprite_count, script_count, anm_entry_ptr) < 0) {
                    anm_loaded->__load_wait = 0;
                    return NULL;
                }
                idk = true;
            }
            sprite_count += anm_entry_ptr->sprite_count;
            script_count += anm_entry_ptr->script_count;
            ++entry_index;
            if (uint32_t offset_to_next = anm_entry_ptr->offset_to_next) {
                load_wait = anm_loaded->__load_wait;
                ++wait_index;
                anm_entry_ptr = based_pointer(anm_entry_ptr, offset_to_next);
            } else {
                for (int32_t i = 0; i < anm_loaded->script_count; ++i) {
                    (*anm_loaded->__vm_array)[i].reset();
                    anm_loaded->__prepare_vm(&(*anm_loaded->__vm_array)[i], i);
                    (*anm_loaded->__vm_array)[i].controller.script_time.set(-1);
                    (*anm_loaded->__vm_array)[i].controller.__timer_1C.set(-1);
                    (*anm_loaded->__vm_array)[i].run_anm();
                }
                anm_loaded->__load_wait = 0;
                return anm_loaded;
            }
        } while (load_wait != wait_index && idk == false);
        anm_loaded->__load_wait = load_wait + 1;
        return anm_loaded;
    }

    inline void unlink_node_from_list_ends(ZUNList<AnmVM>* node) {
        node->unlink_from(this->world_list);
        node->unlink_from(this->ui_list);
    }

    // 0x488B40
    dllexport gnu_noinline AnmVM* get_vm_with_id(const AnmID& vm_id) asm_symbol_rel(0x488B40) {
        if (vm_id) {
            uint32_t fast_id = vm_id.fast_id;
            if (fast_id == INVALID_FAST_ID) {
                auto id_match = [=](AnmVM* vm){
                    return vm->controller.id == vm_id;
                };
                if (AnmVM* ret = this->world_list.find_if(id_match)) return ret;
                if (AnmVM* ret = this->ui_list.find_if(id_match)) return ret;
            }
            else {
                FastAnmVM& fast_vm = this->fast_array[fast_id];
                if (fast_vm.alive && fast_vm.controller.id == vm_id) {
                    return &this->fast_array[fast_id];
                }
            }
        }
        return NULL;
    }

    // 0x488BE0
    dllexport static void interrupt_tree(const AnmID& id, int32_t interrupt_index) asm_symbol_rel(0x488BE0) {
        if (AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(id)) {
            vm->interrupt(interrupt_index);
            vm->controller.child_list.as_head().for_each([=](AnmVM* vm) {
                vm->interrupt(interrupt_index);
            });
        }
    }

    // 0x488C60
    dllexport static void interrupt_and_run_tree(const AnmID& id, int32_t interrupt_index) asm_symbol_rel(0x488C60) {
        if (AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(id)) {
            vm->interrupt(interrupt_index);
            vm->run_anm();
            vm->controller.child_list.as_head().for_each([=](AnmVM*& vm) {
                vm->interrupt(interrupt_index);
                vm->run_anm();
            });
        }
    }

    // 0x488D50
    dllexport void thiscall mark_tree_for_delete(AnmVM* vm) asm_symbol_rel(0x488D50) {
        if (vm && !vm->data.__unknown_flag_F) {
            vm->data.__vm_state = AnmVMState::MarkedForDelete;
            vm->controller.child_list.as_head().for_each([=](AnmVM* vm) gnu_always_inline {
                clang_noinline this->mark_tree_for_delete(vm);
            });
        }
    }

    inline void mark_tree_id_for_delete(AnmID& id) {
        this->mark_tree_for_delete(this->get_vm_with_id(id));
    }

    // 0x488CF0
    dllexport static gnu_noinline void stdcall mark_tree_id_for_delete(const AnmID& id) asm_symbol_rel(0x488CF0) {
        ANM_MANAGER_PTR->mark_tree_id_for_delete((AnmID&)id);
    }

    // 0x489320
    dllexport static AnmVM* allocate_new_vm() asm_symbol_rel(0x489320) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        if (ZUNList<FastAnmVM>* free_node = anm_manager->free_list_head.next) {
            FastAnmVM* vm = free_node->data;
            vm->fast_node.unlink();
            vm->controller.fast_id = vm->fast_id;
            vm->alive = true;
            vm->controller.__root_vm = NULL;
            vm->controller.parent = NULL;
            vm->controller.global_list_node.initialize_with(vm);
            vm->controller.child_list_node.initialize_with(vm);
            vm->controller.child_list.initialize_with(vm);
            return vm;
        } else {
            AnmVM* vm = new AnmVM;
            vm->zero_contents();
            vm->data.sprite_id = -1;
            vm->data.current_instruction_offset = -1;
            vm->reset();
            vm->controller.fast_id = INVALID_FAST_ID;
            return vm;
        }
    }

    // 0x488110
    dllexport gnu_noinline void thiscall __recursive_remove(AnmVM* vm, ZUNList<AnmVM>* list_node) asm_symbol_rel(0x488110) {
        vm->controller.child_list.as_head().for_each_safe([=](AnmVM* current_vm) gnu_always_inline {
            clang_noinline this->__recursive_remove(current_vm, list_node);
        });
        if (vm->data.__vm_state != AnmVMState::Deleted) {
            ZUNList<AnmVM>* current_node = &vm->controller.destroy_list_node;
            current_node->initialize_with(vm);
            list_node->append(current_node);
        }
        vm->data.__vm_state = AnmVMState::Deleted;
        vm->controller.__root_vm = NULL;
        vm->controller.parent = NULL;
        vm->controller.next_in_layer = NULL;
        vm->controller.prev_in_layer = NULL;
        vm->controller.child_list_node.unlink();
    }

    // 0x4885D0
    dllexport int32_t thiscall destroy_possibly_managed_vm(AnmVM* vm) asm_symbol_rel(0x4885D0) {
        this->unlink_node_from_list_ends(&vm->controller.global_list_node);
        vm->run_on_destroy();
        vm->controller.global_list_node.unlink();
        vm->controller.destroy_list_node.unlink();
        vm->controller.__root_vm = NULL;
        vm->controller.parent = NULL;
        if (vm >= this->fast_array && vm < array_end_addr(this->fast_array)) {
            FastAnmVM* fast_vm = &this->fast_array[vm->controller.fast_id];
            fast_vm->alive = false;
            this->free_list_head.append(&fast_vm->fast_node);
            vm->cleanup();
            vm->controller.id = 0;
            return 0;
        }
        else {
            vm->cleanup();
            delete vm;
            return 0;
        }
    }

    // 0x487ED0
    dllexport int32_t thiscall on_tick_world() asm_symbol_rel(0x487ED0) {
        CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
        {
            ZUNListHead<AnmVM> destroy_list;
            destroy_list.initialize_with(NULL);

            AnmVM* layer_lists[WORLD_LAYER_COUNT];
            int32_t i = 0;
            AnmVM* layer_heads = &this->layer_heads[0];
            NoUnroll do {
                //layer_lists[i] = &layer_heads[i];
                layer_heads[i].controller.next_in_layer = NULL;
            } while (++i < WORLD_LAYER_COUNT);
            this->world_list.for_each([&, this](AnmVM* vm) {
                vm->controller.next_in_layer = NULL;
                vm->controller.prev_in_layer = NULL;
                switch (vm->data.__vm_state) {
                    case AnmVMState::Normal:
                        if (vm->run_anm()) {
                    case AnmVMState::MarkedForDelete:
                            this->__recursive_remove(vm, &destroy_list);
                        }
                }
            });
            destroy_list.for_each([this](AnmVM* vm) {
                this->destroy_possibly_managed_vm(vm);
            });
        }
        CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
        return 1;
    }

    // 0x487FE0
    dllexport int32_t thiscall on_tick_ui() asm_symbol_rel(0x487FE0) {
        CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
        {
            ZUNListHead<AnmVM> destroy_list;
            destroy_list.initialize_with(NULL);

            //AnmVM* layer_lists[UI_LAYER_COUNT];
            int32_t i = 0;
            AnmVM* layer_heads = &this->layer_heads[WORLD_LAYER_COUNT];
            do {
                //layer_lists[i] = &layer_heads[i];
                layer_heads[i].controller.next_in_layer = NULL;
            } while (++i < UI_LAYER_COUNT);
            this->ui_list.for_each([&, this](AnmVM* vm) {
                vm->controller.next_in_layer = NULL;
                vm->controller.prev_in_layer = NULL;
                switch (vm->data.__vm_state) {
                    case AnmVMState::Normal:
                        if (vm->run_anm()) {
                    case AnmVMState::MarkedForDelete:
                            this->__recursive_remove(vm, &destroy_list);
                        }
                }
            });
            destroy_list.for_each([this](AnmVM* vm) {
                this->destroy_possibly_managed_vm(vm);
            });
        }
        CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
        return 1;
    }

    forceinline void assign_next_id_to_ref(AnmID& out, AnmVM* vm) {
        UBitInt(ANM_SLOW_ID_BITS) slow_id = this->prev_slow_id + 1;
        this->prev_slow_id = slow_id;
        if (!slow_id) {
            this->prev_slow_id = ++slow_id;
        }
        AnmID new_id;
        new_id.fast_id = vm->controller.fast_id;
        new_id.slow_id = slow_id;
        out = vm->controller.id = new_id;
    }

private:
    // 0x488350
    // This function never uses a pointer for the first arg
    // even though the return value is typically used instead,
    // so I can only assume it must be a reference type.
    dllexport static gnu_noinline AnmID& stdcall add_vm_to_world_list_back(AnmID& out, AnmVM* vm) asm_symbol_rel(0x488350) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        ZUNList<AnmVM>* world_node = &vm->controller.global_list_node;
        world_node->initialize_with(vm);
        anm_manager->world_list.append(world_node);
        anm_manager->assign_next_id_to_ref(out, vm);
        return out;
    }
public:
    static inline AnmID add_vm_to_world_list_back(AnmVM* vm) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return add_vm_to_world_list_back(dummy, vm);
    }

private:
    // 0x488400
    dllexport static gnu_noinline AnmID& stdcall add_vm_to_world_list_front(AnmID& out, AnmVM* vm) asm_symbol_rel(0x488400) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        ZUNList<AnmVM>* world_node = &vm->controller.global_list_node;
        world_node->initialize_with(vm);
        anm_manager->world_list.prepend(world_node);
        anm_manager->assign_next_id_to_ref(out, vm);
        return out;
    }
public:
    static inline AnmID add_vm_to_world_list_front(AnmVM* vm) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return add_vm_to_world_list_front(dummy, vm);
    }

private:
    // 0x488490
    dllexport static gnu_noinline AnmID& stdcall add_vm_to_ui_list_back(AnmID& out, AnmVM* vm) asm_symbol_rel(0x488490) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        ZUNList<AnmVM>* world_node = &vm->controller.global_list_node;
        world_node->initialize_with(vm);
        anm_manager->ui_list.append(world_node);
        anm_manager->assign_next_id_to_ref(out, vm);
        return out;
    }
public:
    static inline AnmID add_vm_to_ui_list_back(AnmVM* vm) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return add_vm_to_ui_list_back(dummy, vm);
    }

private:
    // 0x488540
    dllexport static gnu_noinline AnmID& stdcall add_vm_to_ui_list_front(AnmID& out, AnmVM* vm) asm_symbol_rel(0x488540) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        ZUNList<AnmVM>* world_node = &vm->controller.global_list_node;
        world_node->initialize_with(vm);
        anm_manager->ui_list.prepend(world_node);
        anm_manager->assign_next_id_to_ref(out, vm);
        return out;
    }
public:
    static inline AnmID add_vm_to_ui_list_front(AnmVM* vm) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return add_vm_to_ui_list_front(dummy, vm);
    }

    // 0x488DA0
    dllexport gnu_noinline void thiscall __set_state1_for_all_vms_from_loaded(AnmLoaded* anm_loaded) asm_symbol_rel(0x488DA0) {
        if (anm_loaded) {
            auto set_state1_if_slot_matches = [=](AnmVM* vm) {
                int32_t slot = anm_loaded->slot_index;
                if (
                    vm->data.slot == slot ||
                    vm->data.slot2 == slot
                ) {
                    vm->data.__vm_state = 1;
                }
            };
            this->world_list.for_each_safe(set_state1_if_slot_matches);
            this->ui_list.for_each_safe(set_state1_if_slot_matches);
        }
    }

    inline void __set_state1_for_all_vms_from_loaded_slot(int32_t slot) {
        this->__set_state1_for_all_vms_from_loaded(this->loaded_anm_files[slot]);
    }
};
#pragma region // AnmManager Validation
ValidateFieldOffset32(0x0, AnmManager, __thread_0);
ValidateFieldOffset32(0x20, AnmManager, __int_20);
ValidateFieldOffset32(0x48, AnmManager, __dword_48);
ValidateFieldOffset32(0x70, AnmManager, __dword_70);
ValidateFieldOffset32(0x98, AnmManager, __dword_98);
ValidateFieldOffset32(0xC0, AnmManager, __dword_C0);
ValidateFieldOffset32(0xC4, AnmManager, __dword_C4);
ValidateFieldOffset32(0xC8, AnmManager, __dword_C8);
ValidateFieldOffset32(0xCC, AnmManager, __dword_CC);
ValidateFieldOffset32(0xD0, AnmManager, __int2_D0);
ValidateFieldOffset32(0xD8, AnmManager, __float2_D8);
ValidateFieldOffset32(0xE4, AnmManager, __vm_E4);
ValidateFieldOffset32(0x6F0, AnmManager, world_list);
ValidateFieldOffset32(0x6F8, AnmManager, ui_list);
ValidateFieldOffset32(0x700, AnmManager, fast_array);
ValidateFieldOffset32(0x31200DC, AnmManager, fast_array_end);
ValidateFieldOffset32(0x3120700, AnmManager, next_snapshot_fast_id);
ValidateFieldOffset32(0x3120704, AnmManager, next_snapshot_discriminator);
ValidateFieldOffset32(0x3120708, AnmManager, snapshot_list_head);
ValidateFieldOffset32(0x3120718, AnmManager, free_list_head);
ValidateFieldOffset32(0x312072C, AnmManager, loaded_anm_files);
ValidateFieldOffset32(0x31207B0, AnmManager, __matrix_31207B0);
ValidateFieldOffset32(0x31207F0, AnmManager, __vm_31207F0);
ValidateFieldOffset32(0x3120E00, AnmManager, current_texture_blend_color);
ValidateFieldOffset32(0x3120E04, AnmManager, __index_3120E04);
ValidateFieldOffset32(0x3120E08, AnmManager, current_blend_mode);
ValidateFieldOffset32(0x3120E09, AnmManager, __byte_3120E09);
ValidateFieldOffset32(0x3120E0A, AnmManager, __sbyte_3120E0A);
ValidateFieldOffset32(0x3120E0B, AnmManager, __byte_3120E0B);
ValidateFieldOffset32(0x3120E0C, AnmManager, __byte_3120E0C);
ValidateFieldOffset32(0x3120E0E, AnmManager, currently_using_point_filtering);
ValidateFieldOffset32(0x3120E0F, AnmManager, currently_using_modulate_op);
ValidateFieldOffset32(0x3120E10, AnmManager, current_u_sample_mode);
ValidateFieldOffset32(0x3120E11, AnmManager, current_v_sample_mode);
ValidateFieldOffset32(0x3120E14, AnmManager, __current_dword_3120E14);
ValidateFieldOffset32(0x3120E18, AnmManager, __d3d_vertex_buffer_3120E18);
ValidateFieldOffset32(0x3120E1C, AnmManager, __vertex_array_3120E1C);
ValidateFieldOffset32(0x3120E6C, AnmManager, unrendered_sprite_count);
ValidateFieldOffset32(0x3120E70, AnmManager, sprite_vertex_data);
ValidateFieldOffset32(0x3820E70, AnmManager, sprite_write_cursor);
ValidateFieldOffset32(0x3820E74, AnmManager, sprite_render_cursor);
ValidateFieldOffset32(0x3820E78, AnmManager, unrendered_primitive_count);
ValidateFieldOffset32(0x3820E7C, AnmManager, primitive_vertex_data);
ValidateFieldOffset32(0x3960E7C, AnmManager, primitive_write_cursor);
ValidateFieldOffset32(0x3960E80, AnmManager, primitive_render_cursor);
ValidateFieldOffset32(0x3960E84, AnmManager, layer_heads);
ValidateFieldOffset32(0x39724AC, AnmManager, prev_slow_id);
ValidateFieldOffset32(0x39724B0, AnmManager, __color_39724B0);
ValidateStructSize32(0x39724B8, AnmManager);
#pragma endregion

// 0x443D80
dllexport gnu_noinline UpdateFuncRet fastcall GameThread::on_draw(void* ptr) {
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    anm_manager->__dword_C4 = 0;
    anm_manager->__dword_C8 = 0;
    anm_manager->__dword_C0 = 0;
    anm_manager->__dword_CC = 0;
    return UpdateFuncNext;
}

// 0x441ED0
dllexport gnu_noinline void thiscall Gui::__display_stage_logo() {
    if (SUPERVISOR.gamemode_switch != 8 && GAME_MANAGER.__unknown_flag_E) {
        GUI_PTR->stage_logo_anm->instantiate_vm_to_world_list_back(0);
    }
}

// 0x488FA0
dllexport void thiscall AnmVM::set_sprite(int32_t sprite_id) {
    if (AnmLoaded* anm_loaded = ANM_MANAGER_PTR->loaded_anm_files[this->data.slot]) {
        anm_loaded->set_sprite(this, sprite_id);
    }
}

// 0x4894D0
dllexport gnu_noinline AnmLoaded* thiscall AnmVM::get_anm_loaded() {
    return ANM_MANAGER_PTR->loaded_anm_files[this->data.slot];
}

// size: 0x20
struct EffectData {
    int16_t __effect_anm_file_index; // 0x0
    int16_t __script_id; // 0x2
    AnmVMOnCreateFunc* on_create_func; // 0x4
    int32_t on_tick_index; // 0x8
    int32_t on_draw_index; // 0xC
    int32_t on_destroy_index; // 0x10
    int32_t on_interrupt_index; // 0x14
    int32_t on_copyA_index; // 0x18
    int32_t on_copyB_index; // 0x1C
    // 0x20
};
#pragma region // EffectData Validation
ValidateFieldOffset32(0x0, EffectData, __effect_anm_file_index);
ValidateFieldOffset32(0x2, EffectData, __script_id);
ValidateFieldOffset32(0x4, EffectData, on_create_func);
ValidateFieldOffset32(0x8, EffectData, on_tick_index);
ValidateFieldOffset32(0xC, EffectData, on_draw_index);
ValidateFieldOffset32(0x10, EffectData, on_destroy_index);
ValidateFieldOffset32(0x14, EffectData, on_interrupt_index);
ValidateFieldOffset32(0x18, EffectData, on_copyA_index);
ValidateFieldOffset32(0x1C, EffectData, on_copyB_index);
ValidateStructSize32(0x20, EffectData);
#pragma endregion

extern "C" {
    // 0x4CCBF8
    extern EffectData EFFECT_DATA_TABLE[5] asm("_EFFECT_DATA_TABLE");
}

typedef struct Effect Effect;

struct Effect {

};
#pragma region // Effect Validation
ValidateFieldOffset32(0x39724B0, AnmManager, __color_39724B0);
ValidateStructSize32(0x39724B8, AnmManager);
#pragma endregion

typedef struct EffectManager EffectManager;
extern "C" {
    // 0x4CF2C8
    extern EffectManager* EFFECT_MANAGER_PTR asm("_EFFECT_MANAGER_PTR");
}

static inline constexpr int32_t MAX_EFFECTS = 0x400;

// size: 0x2040
struct EffectManager : ZUNTask {
    // ZUNTask base; // 0x0
    union {
        AnmLoaded* anm_loaded_array[2]; // 0xC
        struct {
            AnmLoaded* effect_anm; // 0xC
            AnmLoaded* bullet_anm; // 0x10
        };
    };
    unknown_fields(0x4); // 0x14
    int32_t slot_index; // 0x18
    AnmID vm_slots[MAX_EFFECTS]; // 0x1C
    int32_t __slot_indexB; // 0x101C
    AnmID __vm_slotsB[MAX_EFFECTS]; // 0x1020
    ZUNThreadB __thread_2020; // 0x2020
    BOOL __done_loading; // 0x203C
    // 0x2040

    inline void zero_contents() {
        zero_this();
    }

    inline EffectManager() {
        this->zero_contents();
        this->__unknown_task_flag_A = true;
    }

    // 0x42AF30
    dllexport gnu_noinline static UpdateFuncRet fastcall on_tick(void* ptr) {
        EffectManager* self = (EffectManager*)ptr;
        for (size_t i = 0; i < MAX_EFFECTS; ++i) {
            (void)self->vm_slots[i].get_vm_ptr();
        }
        return UpdateFuncNext;
    }

    // 0x42AF60
    dllexport gnu_noinline static UpdateFuncRet fastcall on_draw(void* ptr) {
        return UpdateFuncNext;
    }

    // 0x42AC70
    dllexport gnu_noinline EffectManager* allocate() {
        EffectManager* effect_manager = new EffectManager();
        EFFECT_MANAGER_PTR = effect_manager;
        if (
            (effect_manager->bullet_anm = ANM_MANAGER_PTR->preload_anm(7, "bullet.anm")) &&
            (effect_manager->effect_anm = ANM_MANAGER_PTR->preload_anm(8, "effect.anm"))
        ) {
            effect_manager->__done_loading = 1;
        }
        else {
            LOG_BUFFER.write(JpEnStr("", "data is corrupted\r\n"));
        }

        UpdateFunc* update_func = new UpdateFunc(&on_tick, false, effect_manager);
        UpdateFuncRegistry::register_on_tick(update_func, 32);
        effect_manager->on_tick_func = update_func;
        update_func = new UpdateFunc(&on_draw, false, effect_manager);
        UpdateFuncRegistry::register_on_draw(update_func, 39);
        effect_manager->on_draw_func = update_func;
        effect_manager->enable_funcs();
        return effect_manager;
    }

    // 0x422D00
    dllexport gnu_noinline int32_t thiscall find_available_slot() asm_symbol_rel(0x422D00) {
        for (int32_t i = 0; i < MAX_EFFECTS; ++i) {
            int32_t cur_slot = this->slot_index;
            int32_t next_slot = cur_slot + 1;
            this->slot_index = next_slot % MAX_EFFECTS;
            if (
                !this->vm_slots[next_slot] ||
                !this->vm_slots[next_slot].get_vm_ptr()
            ) {
                return cur_slot;
            }
        }
        return -1;
    }

    // 0x42D5D0
    dllexport int32_t thiscall fill_available_slot(AnmID id) asm_symbol_rel(0x42D5D0) {
        EffectManager* effect_manager = EFFECT_MANAGER_PTR;

        int32_t slot = effect_manager->find_available_slot();
        if (slot != -1 && slot < MAX_EFFECTS) {
            effect_manager->vm_slots[slot] = id;
            return slot | 0x80000000;
        }
        return 0;
    }

private:
    // 0x422DD0
    dllexport gnu_noinline static AnmID& stdcall __get_slot_vm_id(AnmID& out, int32_t slot) {
        uint32_t masked_slot = slot % (MAX_EFFECTS * 2u);
        if (
            masked_slot < MAX_EFFECTS &&
            slot < 0
        ) {
            out = EFFECT_MANAGER_PTR->vm_slots[masked_slot];
        }
        else {
            out = 0;
        }
        return out;
    }
public:
    inline static AnmID __get_slot_vm_id(int32_t slot) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return __get_slot_vm_id(dummy, slot);
    }

private:
    // 0x42AF70
    dllexport gnu_noinline AnmID& thiscall instantiate_effect_vm_to_world_list_back(AnmID& out, int32_t type, void* on_create_arg, AnmVM* vm) asm_symbol_rel(0x42AF70) {
        out = 0;
        EffectData& effect_data = EFFECT_DATA_TABLE[type];
        int32_t script = effect_data.__script_id;
        if (script >= 0) {
            if (!vm) {
                out = this->anm_loaded_array[effect_data.__effect_anm_file_index]->instantiate_vm_to_world_list_back(script, -1, (AnmVM**)vm);
                vm = out.get_vm_ptr();
            }
            if (AnmVMOnCreateFunc* create_func = effect_data.on_create_func) {
                create_func(vm, on_create_arg);
            }
            vm->controller.on_tick_index = effect_data.on_tick_index;
            vm->controller.on_draw_index = effect_data.on_draw_index;
            vm->controller.on_destroy_index = effect_data.on_destroy_index;
            vm->controller.on_interrupt_index = effect_data.on_interrupt_index;
            vm->controller.on_copy_A_index = effect_data.on_copyA_index;
            vm->controller.on_copy_B_index = effect_data.on_copyB_index;
        }
        return out;
    }
public:
    inline AnmID instantiate_effect_vm_to_world_list_back(int32_t type, void* on_create_arg, AnmVM* vm) {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return this->instantiate_effect_vm_to_world_list_back(dummy, type, on_create_arg, vm);
    }

private:
    // 0x42B0A0
    dllexport gnu_noinline static AnmID& thiscall instantiate_effect_vm_to_ui_list_back(AnmID& out, UNUSED_ARG(int32_t type), UNUSED_ARG(void* on_create_arg), UNUSED_ARG(AnmVM* vm)) {
        EffectManager* effect_manager = EFFECT_MANAGER_PTR;

        out = 0;
        int32_t script = EFFECT_DATA_TABLE[0].__script_id;
        if (script >= 0) {
            out = effect_manager->anm_loaded_array[EFFECT_DATA_TABLE[0].__effect_anm_file_index]->instantiate_vm_to_ui_list_back(script);
            AnmVM* vm = out.get_vm_ptr();
            if (AnmVMOnCreateFunc* create_func = EFFECT_DATA_TABLE[0].on_create_func) {
                create_func(vm, NULL);
            }
            vm->controller.on_tick_index = EFFECT_DATA_TABLE[0].on_tick_index;
            vm->controller.on_draw_index = EFFECT_DATA_TABLE[0].on_draw_index;
            vm->controller.on_destroy_index = EFFECT_DATA_TABLE[0].on_destroy_index;
            vm->controller.on_interrupt_index = EFFECT_DATA_TABLE[0].on_interrupt_index;
            vm->controller.on_copy_A_index = EFFECT_DATA_TABLE[0].on_copyA_index;
            vm->controller.on_copy_B_index = EFFECT_DATA_TABLE[0].on_copyB_index;
        }
        return out;
    }
public:
    inline static AnmID instantiate_effect0_vm_to_ui_list_back() {
        AnmID dummy{ GARBAGE_VALUE(int) };
        return instantiate_effect_vm_to_ui_list_back(dummy, UNUSED_DWORD, GARBAGE_VALUE(void*), GARBAGE_VALUE(AnmVM*));
    }

    // 0x422D70
    dllexport gnu_noinline static int32_t stdcall create_effect(int32_t type, Float3* position, int) {
        EffectManager* effect_manager = EFFECT_MANAGER_PTR;
        int32_t slot = effect_manager->find_available_slot();
        if (slot != -1 && slot < MAX_EFFECTS) {
            effect_manager->vm_slots[slot] = effect_manager->instantiate_effect_vm_to_world_list_back(type, position, NULL);
        }
        return 0;
    }
};
#pragma region // EffectManager Validation
ValidateFieldOffset32(0x0, EffectManager, task_flags);
ValidateFieldOffset32(0x4, EffectManager, on_tick_func);
ValidateFieldOffset32(0x8, EffectManager, on_draw_func);
ValidateFieldOffset32(0xC, EffectManager, effect_anm);
ValidateFieldOffset32(0x10, EffectManager, bullet_anm);
ValidateFieldOffset32(0x18, EffectManager, slot_index);
ValidateFieldOffset32(0x1C, EffectManager, vm_slots);
ValidateFieldOffset32(0x101C, EffectManager, __slot_indexB);
ValidateFieldOffset32(0x1020, EffectManager, __vm_slotsB);
ValidateFieldOffset32(0x2020, EffectManager, __thread_2020);
ValidateFieldOffset32(0x203C, EffectManager, __done_loading);
ValidateStructSize32(0x2040, EffectManager);
#pragma endregion

inline void StageCamera::__copy_int2_FC_to_anm_manager() {
    if (AnmManager* anm_manager = ANM_MANAGER_PTR) {
        anm_manager->__int2_D0.x = this->__int2_FC.x;
        anm_manager->__int2_D0.y = this->__int2_FC.y;
    }
}

// 0x454760
dllexport HRESULT thiscall Supervisor::d3d_enable_fog() {
    if (this->fog_enabled != TRUE) {
        ANM_MANAGER_PTR->flush_sprites();
        this->fog_enabled = TRUE;
        return this->d3d_device->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }
    return D3D_OK;
}

// 0x4547A0
dllexport HRESULT thiscall Supervisor::d3d_disable_fog() {
    if (this->fog_enabled != FALSE) {
        ANM_MANAGER_PTR->flush_sprites();
        this->fog_enabled = FALSE;
        return this->d3d_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    return D3D_OK;
}

// 0x4547E0
dllexport HRESULT thiscall Supervisor::d3d_enable_zwrite() {
    if (this->zwrite_enabled != TRUE) {
        ANM_MANAGER_PTR->flush_sprites();
        this->zwrite_enabled = TRUE;
        return this->d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    }
    return D3D_OK;
}

// 0x454820
dllexport HRESULT thiscall Supervisor::d3d_disable_zwrite() {
    if (this->zwrite_enabled != FALSE) {
        ANM_MANAGER_PTR->flush_sprites();
        this->zwrite_enabled = FALSE;
        return this->d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
    return D3D_OK;
}

// 0x41B330
dllexport void thiscall Supervisor::set_camera_by_index(uint32_t index) {
    StageCamera* camera = &this->cameras[index];
    this->current_camera_ptr = camera;
    this->__sub_4548E0(camera);
    this->d3d_device->SetViewport(&this->current_camera_ptr->__viewport_10C);
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    anm_manager->__float2_D8.x = this->current_camera_ptr->__int2_104.x;
    anm_manager->__float2_D8.y = this->current_camera_ptr->__int2_104.y;
    this->current_camera_index = index;
}
// 0x41B3B0
dllexport gnu_noinline void stdcall Supervisor::set_camera2_alt(uint32_t) {
    StageCamera* camera = &SUPERVISOR.cameras[2];
    SUPERVISOR.current_camera_ptr = camera;
    SUPERVISOR.__sub_4548E0(camera);
    SUPERVISOR.d3d_device->SetViewport(&SUPERVISOR.current_camera_ptr->__viewport_124);
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    anm_manager->__float2_D8.x = SUPERVISOR.current_camera_ptr->__int2_104.x;
    anm_manager->__float2_D8.y = SUPERVISOR.current_camera_ptr->__int2_104.y;
    SUPERVISOR.current_camera_index = 2;
}

inline void thiscall Supervisor::set_camera_by_index_disable_fog(uint32_t index) {
    StageCamera* camera = &this->cameras[index];
    this->current_camera_ptr = camera;
    this->__sub_4548E0(camera);
    this->d3d_device->SetViewport(&this->current_camera_ptr->__viewport_10C);
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    anm_manager->__float2_D8.x = this->current_camera_ptr->__int2_104.x;
    anm_manager->__float2_D8.y = this->current_camera_ptr->__int2_104.y;
    this->current_camera_index = index;
    if (this->zwrite_enabled != FALSE) {
        anm_manager->flush_sprites();
        this->zwrite_enabled = FALSE;
        this->d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
}

inline void thiscall Supervisor::set_camera_by_index_disable_zwrite(uint32_t index) {
    StageCamera* camera = &this->cameras[index];
    this->current_camera_ptr = camera;
    this->__sub_4548E0(camera);
    this->d3d_device->SetViewport(&this->current_camera_ptr->__viewport_10C);
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    anm_manager->__float2_D8.x = this->current_camera_ptr->__int2_104.x;
    anm_manager->__float2_D8.y = this->current_camera_ptr->__int2_104.y;
    this->current_camera_index = index;
    if (this->fog_enabled != FALSE) {
        anm_manager->flush_sprites();
        this->fog_enabled = FALSE;
        this->d3d_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    anm_manager = ANM_MANAGER_PTR;
    anm_manager->flush_sprites();
    this->d3d_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
}

inline void AnmManager::set_modulate_op() {
    if (!ANM_MANAGER_PTR->currently_using_modulate_op) {
        SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        ANM_MANAGER_PTR->currently_using_modulate_op = true;
    }
}

// 0x4548E0
dllexport gnu_noinline void stdcall Supervisor::__sub_4548E0(StageCamera* camera) {
    if (AnmManager* anm_manager = ANM_MANAGER_PTR) {
        anm_manager->flush_sprites();
    }
    SUPERVISOR.d3d_device->SetTransform(D3DTS_VIEW, &camera->view_matrix);
    SUPERVISOR.d3d_device->SetTransform(D3DTS_PROJECTION, &camera->projection_matrix);
    camera->__copy_int2_FC_to_anm_manager();
}

#pragma region // AnmLoaded instantiate funcs
// 0x488770
dllexport AnmID& thiscall AnmLoaded::instantiate_vm(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer, AnmVMCreationFlags flags, AnmVM** raw_out) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        if (raw_out) {
            *raw_out = vm;
        }
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        if (layer >= 0) {
            vm->data.layer = layer;
            if (layer < 23) {
                vm->data.origin_mode = 1;
            }
        }
        if (!position && !flags.__unknown_flag_B) {
            vm->controller.position = { 0.0f, 0.0f, 0.0f };
        }
        else if (position) {
            if (flags.__unknown_flag_A) {
                vm->controller.position = *position + (Float2){ 320.0f, 16.0f };
            } else {
                vm->controller.position = *position;
            }
        }
        vm->data.rotation.z = z_rotation;
        vm->run_anm();
        vm->data.creation_flags = flags;
        out = 0;
        switch (flags.list_type) {
            case UiListFront:
                out = AnmManager::add_vm_to_ui_list_front(vm);
                vm->data.__unknown_field_B = 0;
                break;
            case UiListBack:
                out = AnmManager::add_vm_to_ui_list_back(vm);
                vm->data.__unknown_field_B = 0;
                break;
            case WorldListFront:
                out = AnmManager::add_vm_to_world_list_front(vm);
                break;
            case WorldListBack:
                out = AnmManager::add_vm_to_world_list_back(vm);
                break;
        }
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

inline AnmID& thiscall AnmLoaded::instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        vm->controller.position.safe_copy(position);
        vm->data.rotation.z = 0.0f;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_BACK;
        out = AnmManager::add_vm_to_world_list_back(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

inline AnmID& thiscall AnmLoaded::instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position, int32_t layer) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        if (layer >= 0) {
            vm->data.layer = layer;
            if (layer < 23) {
                vm->data.origin_mode = 1;
            }
        }
        vm->controller.position.safe_copy(position);
        vm->data.rotation.z = 0.0f;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_BACK;
        out = AnmManager::add_vm_to_world_list_back(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x405BF0
dllexport AnmID& thiscall AnmLoaded::instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, int32_t layer, AnmVM** raw_out) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        if (raw_out) {
            *raw_out = vm;
        }
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        if (layer >= 0) {
            vm->data.layer = layer;
            if (layer < 23) {
                vm->data.origin_mode = 1;
            }
        }
        vm->controller.position = (Float3){ 0.0f, 0.0f, 0.0f };
        vm->data.rotation.z = 0.0f;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_BACK;
        out = 0;
        out = AnmManager::add_vm_to_world_list_back(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

inline AnmID& thiscall AnmLoaded::instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        vm->data.use_animation_rng = true;
        if (layer >= 0) {
            vm->data.layer = layer;
            if (layer < 23) {
                vm->data.origin_mode = 1;
            }
        }
        vm->controller.position.safe_copy(position);
        vm->data.rotation.z = z_rotation;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_BACK;
        out = 0;
        out = AnmManager::add_vm_to_world_list_back(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x413180
dllexport AnmID& thiscall AnmLoaded::instantiate_vm_to_world_list_back(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer, AnmVM** raw_out) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        if (raw_out) {
            *raw_out = vm;
        }
        vm->data.use_animation_rng = true;
        if (layer >= 0) {
            vm->data.layer = layer;
            if (layer < 23) {
                vm->data.origin_mode = 1;
            }
        }
        vm->controller.position.safe_copy(position);
        vm->data.rotation.z = z_rotation;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_BACK;
        out = 0;
        out = AnmManager::add_vm_to_world_list_back(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x43A030
dllexport AnmID& thiscall AnmLoaded::instantiate_vm_to_world_list_front(AnmID& out, int32_t script_index, int32_t layer, UNUSED_ARG(AnmVMCreationFlags flags)) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        if (layer >= 0) {
            vm->data.layer = layer;
            if (layer < 23) {
                vm->data.origin_mode = 1;
            }
        }
        vm->controller.position = (Float3){ 0.0f, 0.0f, 0.0f };
        vm->data.rotation.z = 0.0f;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_FRONT;
        out = 0;
        out = AnmManager::add_vm_to_world_list_front(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x43A110
dllexport AnmID& thiscall AnmLoaded::instantiate_vm_to_world_list_front(AnmID& out, int32_t script_index, Float3* position, float z_rotation, UNUSED_ARG(int32_t layer), UNUSED_ARG(AnmVMCreationFlags flags)) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        vm->controller.position.safe_copy(position);
        vm->data.rotation.z = z_rotation;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_FRONT;
        out = 0;
        out = AnmManager::add_vm_to_world_list_front(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x416A10
dllexport AnmID& thiscall AnmLoaded::instantiate_vm_to_ui_list_back(AnmID& out, int32_t script_index, UNUSED_ARG(AnmVMCreationFlags flags)) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        vm->controller.position = (Float3){ 0.0f, 0.0f, 0.0f };
        vm->data.rotation.z = 0.0f;
        vm->run_anm();
        vm->data.creation_flags = UI_LIST_BACK;
        out = 0;
        out = AnmManager::add_vm_to_ui_list_back(vm);
        vm->data.__unknown_field_B = 0;
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x409670
dllexport AnmID& thiscall AnmLoaded::instantiate_vm_to_ui_list_back(AnmID& out, int32_t script_index, Float3* position, UNUSED_ARG(AnmVMCreationFlags flags)) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        vm->controller.position.safe_copy(position);
        vm->data.rotation.z = 0.0f;
        vm->run_anm();
        vm->data.creation_flags = UI_LIST_BACK;
        out = 0;
        out = AnmManager::add_vm_to_ui_list_back(vm);
        vm->data.__unknown_field_B = 0;
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x409590
dllexport AnmID& thiscall AnmLoaded::instantiate_vm_to_ui_list_front(AnmID& out, int32_t script_index, Float3* position, UNUSED_ARG(AnmVMCreationFlags flags)) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        vm->controller.position.safe_copy(position);
        vm->data.rotation.z = 0.0f;
        vm->run_anm();
        vm->data.creation_flags = UI_LIST_FRONT;
        out = 0;
        out = AnmManager::add_vm_to_ui_list_front(vm);
        vm->data.__unknown_field_B = 0;
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x4888F0
dllexport AnmID& thiscall AnmLoaded::instantiate_child_vm(AnmID& out, int32_t script_index, AnmVM* parent, AnmVMCreationFlags flags) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        vm->data.use_animation_rng = true;
        vm->data.layer = parent->data.layer;
        vm->controller.position = (Float3){ 0.0f, 0.0f, 0.0f };
        this->__copy_data_to_vm(vm, script_index);
        vm->data.colorize_children ^= parent->data.colorize_children;
        vm->controller.parent = parent;
        vm->controller.__root_vm = parent->controller.__root_vm ? : parent;
        vm->run_anm();
        vm->data.creation_flags = flags;
        out = 0;
        switch (flags.list_type) {
            case UiListFront:
                out = AnmManager::add_vm_to_ui_list_front(vm);
                break;
            case UiListBack:
                out = AnmManager::add_vm_to_ui_list_back(vm);
                break;
            case WorldListFront:
                out = AnmManager::add_vm_to_world_list_front(vm);
                break;
            case WorldListBack:
                out = AnmManager::add_vm_to_world_list_back(vm);
                break;
        }
        parent->controller.child_list.append(&vm->controller.child_list_node);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}

// 0x488A40
dllexport AnmID& thiscall AnmLoaded::instantiate_orphan_vm_to_world_list_back(AnmID& out, int32_t script_index, AnmVM* parent, UNUSED_ARG(AnmVMCreationFlags flags)) {
    CRITICAL_SECTION_MANAGER.enter_section(AnmList_CS);
    {
        this->__counter_134++;
        AnmVM* vm = AnmManager::allocate_new_vm();
        this->__copy_data_to_vm(vm, script_index);
        vm->data.use_animation_rng = true;
        vm->data.layer = parent->data.layer;
        vm->data.colorize_children ^= parent->data.colorize_children;
        vm->controller.position = parent->controller.position;
        vm->data.rotation = parent->data.rotation;
        vm->data.__position_2 = parent->data.position;
        vm->run_anm();
        vm->data.creation_flags = WORLD_LIST_BACK;
        out = 0;
        out = AnmManager::add_vm_to_world_list_back(vm);
    }
    CRITICAL_SECTION_MANAGER.leave_section(AnmList_CS);
    return out;
}
#pragma endregion

inline AnmInstruction* AnmVM::get_current_instruction() {
    int32_t anm_slot = this->data.slot;
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    int32_t script_index = this->data.script_id;
    AnmInstruction* current_instruction = (*anm_manager->loaded_anm_files[anm_slot]->scripts)[script_index];
    return IndexInstr(this->data.current_instruction_offset);
}

// 0x488E30
dllexport AnmVM* AnmID::get_vm_ptr() {
    AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(*this);
    if (!vm) {
        this->full = 0;
    }
    return vm;
}

inline int32_t AnmID::run_vm() {
    AnmManager* anm_manager = ANM_MANAGER_PTR;
    AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(*this);
    if (!vm) {
        this->full = 0;
        vm = &anm_manager->__vm_E4;
    }
    return vm->run_anm();
}

// 0x488E50
dllexport void AnmID::interrupt_tree(int32_t interrupt_index) {
    AnmManager::interrupt_tree(*this, interrupt_index);
}

inline void AnmID::__unknown_tree_set_J(AnmManager* anm_manager) {
    if (AnmVM* vm = anm_manager->get_vm_with_id(*this)) {
        vm->data.__unknown_flag_J = true;
        vm->controller.child_list.as_head().for_each([](AnmVM* vm) static_lambda {
            vm->__unknown_tree_set_J();
        });
    }
}

// 0x488E70
dllexport void AnmID::__unknown_tree_set_J() {
    this->__unknown_tree_set_J(ANM_MANAGER_PTR);
}

// 0x488EB0
dllexport void AnmID::__unknown_tree_clear_J() {
    if (AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(*this)) {
        vm->data.__unknown_flag_J = false;
        vm->controller.child_list.as_head().for_each([](AnmVM* vm) static_lambda {
            vm->__unknown_tree_clear_J();
        });
    }
}

inline void AnmID::mark_tree_for_delete(AnmManager* anm_manager) {
    anm_manager->mark_tree_id_for_delete(*this);
    this->full = NULL;
}

// 0x488F50
dllexport void AnmID::mark_tree_for_delete() {
    AnmManager::mark_tree_id_for_delete(*this);
    this->full = NULL;
}

inline void AnmID::set_position(Float3* position) {
    if (AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(*this)) {
        vm->data.position.set(*position);
    }
}

// 0x488F70
dllexport void thiscall AnmID::set_controller_position(Float3* position) {
    if (AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(*this)) {
        vm->controller.position.set(*position);
    }
}

// 0x488FD0
dllexport void thiscall AnmID::__sub_488FD0(int32_t script) {
    AnmManager* anm_manager_ptr = ANM_MANAGER_PTR;
    if (AnmVM* vm = anm_manager_ptr->get_vm_with_id(*this)) {
        AnmLoaded* anm_loaded = anm_manager_ptr->loaded_anm_files[vm->data.slot];
        vm->data.visible = false;
        *this = anm_loaded->instantiate_vm_to_world_list_back(script);
        vm->data.slot = anm_loaded->slot_index;
        vm->data.slot2 = anm_loaded->slot_index;
    }
}

// 0x4892F0
dllexport void thiscall AnmID::set_color1(D3DCOLOR color) {
    AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(*this);
    if (!vm) {
        this->full = 0;
        return;
    }
    vm->data.color1 = color;
}

// 0x489230
dllexport gnu_noinline AnmVM* thiscall AnmID::__wtf_child_list_jank_A(int32_t script, uint32_t arg2) {
    AnmVM* vm = ANM_MANAGER_PTR->get_vm_with_id(*this);
    if (!vm) {
        this->full = 0;
        return NULL;
    }
    return vm->__wtf_child_list_jank_A(script, arg2);
}

inline void AnmLoaded::__prepare_vm_data(AnmVM* vm, int32_t script_id) {
    vm->data.script_id2 = script_id;
    vm->data.slot = this->slot_index;
    vm->data.slot2 = this->slot_index;
    vm->data.mirror_x = false;
    vm->data.mirror_y = false;
    vm->data.__unknown_field_B = 2;
    vm->data.script_id = script_id;
    vm->data.current_instruction_offset = 0;
    vm->controller.__timer_1C.reset();
    vm->controller.script_time.reset();
    vm->data.visible = false;
}

inline void AnmLoaded::__prepare_vm(AnmVM* vm, int32_t script_id) {
    if ((*this->scripts)[script_id]) {
        vm->reset();
        this->__prepare_vm_data(vm, script_id);
    } else {
        vm->zero_contents();
    }
}

dllexport void AnmLoaded::__sub_477D60(AnmVM* vm, int32_t script_id) {
    if ((*this->scripts)[script_id] && !this->__load_wait) {
        this->__prepare_vm_data(vm, script_id);
        vm->run_anm();
        ANM_MANAGER_PTR->__dword_C0++;
        if (vm->data.__unknown_field_B == 2) {
            vm->data.__unknown_field_B = 1;
        }
    } else {
        vm->zero_contents();
    }
}

// 0x478580
dllexport gnu_noinline int32_t AnmVM::run_anm() {
    /*
    float previous_gamespeed = GAME_SPEED.value;
    if (this->data.slowdown_immune) {
        GAME_SPEED.value = 1.0f;
    }
    if (this->get_custom_slowdown() > 0.0f) {
        float new_gamespeed = previous_gamespeed - (this->get_custom_slowdown() * previous_gamespeed);
        GAME_SPEED.value = new_gamespeed;
        if (0.0f > new_gamespeed) {
            GAME_SPEED.value = 0.0f;
        }
    }
    if (this->run_on_tick()) {
        GAME_SPEED.set(previous_gamespeed);
        return 1;
    }
    if (this->data.current_instruction_offset < 0 || this->data.__unknown_flag_O) {
        GAME_SPEED.set(previous_gamespeed);
        return 0;
    }
    this->controller.__timer_1C.increment_anm();
    AnmInstruction* current_instruction;
    if (this->data.run_interrupt) {
        current_instruction = this->get_current_instruction();
        goto RunInterrupt;
    }
    if (this->data.__unknown_field_B == 1) if (GameThread* game_thread = GAME_THREAD_PTR; !game_thread->__unknown_flag_B) {
        GAME_SPEED.set(previous_gamespeed);
        return 0;
    }
    //for (;;) {
        current_instruction = this->get_current_instruction();
        //if (!(current_instruction->time <= this->controller.script_time.current)) break;
        switch (current_instruction->opcode) {

        }
    //}
RunInterrupt:
    */
    use_var(this);
    return rand();
    //return 0;
}

//template<size_t initial_size, size_t batch_size>
//struct AnmVMIDHack {
//    AnmVM** id_array = (AnmVM**)calloc(initial_size, sizeof(AnmVM*));
//    size_t array_size = initial_size;
//    size_t previous_index = 0;
//
//    dllexport gnu_noinline AnmVM** allocate_anm_id() {
//        AnmVM** id_array = this->id_array;
//        AnmVM** new_id = &id_array[this->previous_index];
//        AnmVM** starting_id = new_id;
//        AnmVM** array_end_id = &id_array[this->array_size];
//        do {
//            if (unpredictable(!*new_id)) {
//                this->previous_index = new_id - id_array;
//                return new_id;
//            }
//            ++new_id;
//            if (expect(new_id == array_end_id, false)) {
//                new_id = id_array;
//            }
//        } while (expect(new_id != starting_id, true));
//        size_t prev_size = this->array_size;
//        size_t new_size = prev_size + batch_size;
//        auto new_array = (AnmVM**)realloc(id_array, sizeof(AnmVM*) * new_size);
//        if (expect(new_array != NULL, true)) {
//            this->id_array = new_array;
//            this->array_size = new_size;
//            this->previous_index = prev_size;
//            return (AnmVM**)__builtin_memset(&new_array[prev_size], 0, sizeof(AnmVM*) * batch_size);
//        }
//        return NULL;
//    }
//};
//
//dllexport AnmVMIDHack<0x8000, 0x100> anm_hack;

// 0x4573F0
dllexport gnu_noinline BOOL thiscall Globals::__add_power(int32_t value) {
    int32_t& A = this->current_power;
    int32_t B = this->max_power;
    if (A >= B) {
        return false;
    }
    A += value;
    if (A > B) {
        A = B;
        Gui* gui = GUI_PTR;
        AnmManager::mark_tree_id_for_delete(gui->__anm_id_BC);
        gui->__anm_id_BC = 0;
        gui->__anm_id_BC = gui->__anm_loaded_2C0->instantiate_vm_to_world_list_back(33);
    }
    int32_t C = (A - value) / this->power_per_level;
    int32_t D = A / this->power_per_level;
    return C != D;
}

// 0x4420E0
dllexport gnu_noinline void thiscall Gui::__update_bomb_ui(int32_t bomb_count, int32_t bomb_fragments, int32_t bomb_max) {
    AnmVM* vm = this->__anm_vm_ptr_array_68[0];
    if (vm) {
        float A = (7 - bomb_max) * 28.0f;
        Float3 B(A, A, 0.0f);
        int32_t C = 0;
        vm->controller.position = B;
        this->__anm_vm_ptr_array_68[1]->controller.position = B;
        this->__anm_vm_ptr_array_68[2]->controller.position = B;
        this->__anm_vm_ptr_array_68[3]->controller.position = B;
        this->__anm_vm_ptr_array_68[4]->controller.position = B;
        this->__anm_vm_ptr_array_68[5]->controller.position = B;
        this->__anm_vm_ptr_array_68[6]->controller.position = B;
        if (bomb_count > 0) {
            C = bomb_count;
            AnmVM** vm_ptrs = &this->__anm_vm_ptr_array_68[0];
            do {
                vm_ptrs[bomb_count]->interrupt(2);
            } while (--bomb_count);
        }
        if (C < bomb_max) {
            // 0x4B6600
            static constexpr int32_t UnknownAnmInterruptTableA[] = { 0, 1, 2 };
            this->__anm_vm_ptr_array_68[0]->interrupt(UnknownAnmInterruptTableA[bomb_fragments]);
            if (++C < bomb_max) {
                bomb_count = bomb_max - C;
                AnmVM** vm_ptrs = &this->__anm_vm_ptr_array_68[0];
                do {
                    vm_ptrs[bomb_count]->interrupt(3);
                } while (--bomb_count);
            }
        }
        if (C < countof(this->__anm_vm_ptr_array_68)) {
            bomb_count = countof(this->__anm_vm_ptr_array_68) - C;
            AnmVM** vm_ptrs = &this->__anm_vm_ptr_array_68[0];
            do {
                vm_ptrs[bomb_count]->interrupt(5);
            } while (--bomb_count);
        }
    }
}

// 0x43A730
dllexport gnu_noinline ZUNResult thiscall Gui::__sub_43A730() {
    AnmLoaded* stage_logo_anm = AnmManager::preload_anm(STAGE_LOGO_ANM_INDEX, STAGE_DATA_PTR->logo_anm_filename);
    this->stage_logo_anm = stage_logo_anm;
    if (!stage_logo_anm) goto corrupted_data_error;
    if (void* cached_msg_file = CACHED_MSG_FILE_PTR) {
        this->msg_file = cached_msg_file;
        DebugLogger::__debug_log_stub_8("%s load Skip\n", STAGE_DATA_PTR->msg_filenames[GAME_MANAGER.globals.character + GAME_MANAGER.globals.shottype]);
        CACHED_MSG_FILE_PTR = NULL;
    } else {
        void* msg_file = read_file_from_dat(STAGE_DATA_PTR->msg_filenames[GAME_MANAGER.globals.character + GAME_MANAGER.globals.shottype]);
        this->msg_file = msg_file;
        if (!msg_file) goto corrupted_data_error;
    }
    this->__timer_140.reset();
    this->__int_13C = -1;
    this->__score = GAME_MANAGER.globals.score;
    this->spell_timer_seconds = -1;
    this->spell_timer_hundredths = -1;
    return ZUN_SUCCESS;
corrupted_data_error:
    LOG_BUFFER.write(JpEnStr("", "data is corrupted\r\n"));
    return ZUN_ERROR;
}

typedef struct AsciiManager AsciiManager;

// size: 0x138
struct AsciiString {
    char text[0x100]; // 0x0
    Float3 position; // 0x100
    D3DCOLOR color; // 0x10C
    Float2 scale; // 0x110
    int __dword_118; // 0x118
    int __dword_11C; // 0x11C
    int font_id; // 0x120
    BOOL enable_shadows; // 0x124
    uint32_t group; // 0x128
    int32_t duration; // 0x12C
    int __horizontal_positioning_mode; // 0x130
    int __vertical_positioning_mode; // 0x134
    // 0x138
};
#pragma region // AsciiString Validation
ValidateFieldOffset32(0x0, AsciiString, text);
ValidateFieldOffset32(0x100, AsciiString, position);
ValidateFieldOffset32(0x10C, AsciiString, color);
ValidateFieldOffset32(0x110, AsciiString, scale);
ValidateFieldOffset32(0x120, AsciiString, font_id);
ValidateFieldOffset32(0x124, AsciiString, enable_shadows);
ValidateFieldOffset32(0x128, AsciiString, group);
ValidateFieldOffset32(0x12C, AsciiString, duration);
ValidateFieldOffset32(0x130, AsciiString, __horizontal_positioning_mode);
ValidateFieldOffset32(0x134, AsciiString, __vertical_positioning_mode);
ValidateStructSize32(0x138, AsciiString);
#pragma endregion

extern "C" {
    // 0x4CF2AC
    extern AsciiManager* ASCII_MANAGER_PTR asm("_ASCII_MANAGER_PTR");
}

#if INCLUDE_PATCH_CODE
dllexport uint32_t score_upper[3] = { rand(), rand(), rand() };
#endif

// size: 0x19278
struct AsciiManager : ZUNTask {
    // ZUNTask base; // 0x0
    AnmVM __vm_C; // 0xC
    AnmVM __vm_618; // 0x618
    AsciiString strings[320]; // 0xC24
    int32_t string_count; // 0x19224
    D3DCOLOR color; // 0x19228
    D3DCOLOR color2; // 0x1922C
    Float2 scale; // 0x19230
    int __dword_19238; // 0x19238
    int __dword_1923C; // 0x1923C
    BOOL enable_shadows; // 0x19240
    int font_id; // 0x19244
    uint32_t group; // 0x19248
    int32_t duration; // 0x1924C
    int __horizontal_positioning_mode; // 0x19250
    int __vertical_positioning_mode; // 0x19254
    int __character_spacing_for_font_0; // 0x19258
    int frame_count; // 0x1925C
    AnmLoaded* ascii_anm; // 0x19260
    AnmID __vm_id_19264; // 0x19264
    AnmID __vm_id_19268; // 0x19268
    UpdateFunc* on_draw_func_group_1; // 0x1926C
    UpdateFunc* on_draw_func_group_2; // 0x19270
    UpdateFunc* on_draw_func_group_3; // 0x19274
    // 0x19278

    inline void zero_contents() {
        zero_this();
    }

    inline AsciiManager() {
        this->zero_contents();
        this->__unknown_task_flag_A = true;
        this->color = PackD3DCOLOR(255, 255, 255, 255);
        this->color2 = PackD3DCOLOR(255, 0, 0, 0);
        this->scale.x = 1.0f;
        this->scale.y = 1.0f;
        this->__dword_1923C = 0;
        this->__character_spacing_for_font_0 = 9;
        this->__horizontal_positioning_mode = 1;
        this->__vertical_positioning_mode = 1;
    }

    // This functions is disgusting
    // 0x41A2B0
    dllexport gnu_noinline void thiscall draw_string(AsciiString* string) asm_symbol_rel(0x41A2B0) {
        use_var(string);
    }

    // 0x41A180
    dllexport gnu_noinline UpdateFuncRet thiscall draw_group(uint32_t group) asm_symbol_rel(0x41A180) {
        for (int32_t i = 0; i < this->string_count; ++i) {
            if (this->strings[i].group == group) {
                this->draw_string(&this->strings[i]);
            }
        }
        ANM_MANAGER_PTR->flush_sprites();
        SUPERVISOR.set_camera_by_index(2);
        return UpdateFuncNext;
    }

    // Yes, the original code was something terrible like this
    // 0x4198B0
    dllexport static UpdateFuncRet fastcall on_tick(void* ptr) {
        AsciiManager* self = (AsciiManager*)ptr;
        int32_t live_index = 0;
        AsciiString* cur_string = self->strings;
        AsciiString* live_string = cur_string;
        for (int32_t cur_index = 0; cur_index < self->string_count; ++cur_index) {
            if (--self->strings[cur_index].duration >= 0) {
                if (cur_index != live_index) {
                    *live_string++ = *cur_string;
                }
                ++live_index;
            }
            ++cur_string;
        }
        ++self->frame_count;
        self->string_count = live_index;
        return UpdateFuncNext;
    }

    // 0x419940
    dllexport static UpdateFuncRet fastcall on_draw_group_0(void* ptr) {
        return ((AsciiManager*)ptr)->draw_group(0);
    }

    // 0x419950
    dllexport static UpdateFuncRet fastcall on_draw_group_1(void* ptr) {
        AsciiManager* self = (AsciiManager*)ptr;
        SUPERVISOR.set_camera_by_index(0);
        ANM_MANAGER_PTR->flush_sprites();
        self->__vm_C.data.origin_mode = 2;
        self->draw_group(1);
        self->__vm_C.data.origin_mode = 0;
        SUPERVISOR.set_camera_by_index(2);
        return UpdateFuncNext;
    }

    // 0x419A80
    dllexport static UpdateFuncRet fastcall on_draw_group_2(void* ptr) {
        AsciiManager* self = (AsciiManager*)ptr;
        SUPERVISOR.set_camera2_alt();
        UpdateFuncRet ret = self->draw_group(2);
        SUPERVISOR.set_camera_by_index(2);
        return ret;
    }

    // 0x419B10
    dllexport static UpdateFuncRet fastcall on_draw_group_3(void* ptr) {
        AsciiManager* self = (AsciiManager*)ptr;
        SUPERVISOR.set_camera_by_index(2);
        return self->draw_group(3);
    }

    // 0x419B90
    dllexport gnu_noinline void thiscall add_string(Float3* position, const char* str) asm_symbol_rel(0x419B90) {
        int32_t next_index = this->string_count;
        if (next_index < countof(this->strings)) {
            AsciiString& string = this->strings[next_index];
            this->string_count = next_index + 1;

            byteloop_strcpy(string.text, str);
            string.position = *position;
            string.position *= WINDOW_DATA.__float_2070;
            string.group = this->group;
            string.color = this->color;
            string.scale = this->scale; // Copied as integers
            string.__dword_11C = this->__dword_19238;
            string.font_id = this->font_id;
            string.enable_shadows = this->enable_shadows;
            string.duration = this->duration;
            string.__horizontal_positioning_mode = this->__horizontal_positioning_mode;
            string.__vertical_positioning_mode = this->__vertical_positioning_mode;
        }
    }

    // 0x419CB0
    dllexport gnu_noinline void cdecl printf(Float3* position, const char* format, ...) asm_symbol_rel(0x419CB0) {
        va_list va;
        va_start(va, format);
        char buffer[0x100];
        vsprintf(buffer, format, va);
        //va_end(va);
        switch (this->font_id) {
            case 10: {
                D3DCOLOR prev_color = this->color;
                this->color = this->color2;
                this->font_id = 11;
                this->add_string(position, buffer);
                this->font_id = 10;
                this->color = prev_color;
                break;
            }
            case 7: {
                D3DCOLOR prev_color = this->color;
                this->color = this->color2;
                this->font_id = 9;
                this->add_string(position, buffer);
                this->font_id = 7;
                this->color = prev_color;
                break;
            }
            case 6: {
                D3DCOLOR prev_color = this->color;
                this->color = this->color2;
                this->font_id = 8;
                this->add_string(position, buffer);
                this->font_id = 6;
                this->color = prev_color;
                break;
            }
        }
        this->add_string(position, buffer);
    }

private:
    inline void print_number_impl(Float3* position, uint32_t number) {
        char buffer[0x100];
        if (number < 1000) {
            sprintf(buffer, "%d", number);
        }
        else {
            uint32_t hundreds = number % 1000;
            uint32_t thousands = number / 1000;
            if (number < 1000000) {
                sprintf(buffer, "%d,%.3d", thousands, hundreds);
            }
            else {
                thousands %= 1000;
                uint32_t millions = (number / 1000000) % 1000;
                if (number < 1000000000) {
                    sprintf(buffer, "%d,%.3d,%.3d", millions, thousands, hundreds);
                }
                else {
                    uint32_t billions = (number / 1000000000) % 1000;
                    sprintf(buffer, "%d,%.3d,%.3d,%.3d", billions, millions, thousands, hundreds);
                }
            }
        }
        switch (this->font_id) {
            case 10: {
                D3DCOLOR prev_color = this->color;
                this->color = this->color2;
                this->font_id = 11;
                this->add_string(position, buffer);
                this->font_id = 10;
                this->color = prev_color;
                break;
            }
        }
        this->add_string(position, buffer);
    }
public:
    // 0x419DD0
    dllexport gnu_noinline static void stdcall print_number(Float3* position, uint32_t number) asm_symbol_rel(0x419DD0) {
        ASCII_MANAGER_PTR->print_number_impl(position, number);
    }
    
private:
    inline void print_score_impl(Float3* position, uint32_t score, uint32_t continues) {
        char buffer[0x100];
        if (score < 100) {
            sprintf(buffer, "%d", score * 10 + continues);
        }
        else {
            uint32_t hundreds = score % 100;
            uint32_t thousands = score / 100;
            if (score < 100000) {
                sprintf(buffer, "%d,%.2d%d", thousands, hundreds, continues);
            }
            else {
                thousands %= 1000;
                uint32_t millions = (score / 100000) % 1000;
                if (score < 10000000) {
                    sprintf(buffer, "%d,%.3d,%.2d%d", millions, thousands, hundreds, continues);
                }
                else {
                    uint32_t billions = (score / 100000000) % 1000;
                    sprintf(buffer, "%d,%.3d,%.3d,%.2d%d", billions, millions, thousands, hundreds, continues);
                }
            }
        }
        switch (this->font_id) {
            case 10: {
                D3DCOLOR prev_color = this->color;
                this->color = this->color2;
                this->font_id = 11;
                this->add_string(position, buffer);
                this->font_id = 10;
                this->color = prev_color;
                break;
            }
            case 7: {
                D3DCOLOR prev_color = this->color;
                this->color = this->color2;
                this->font_id = 9;
                this->add_string(position, buffer);
                this->font_id = 7;
                this->color = prev_color;
                break;
            }
            case 6: {
                D3DCOLOR prev_color = this->color;
                this->color = this->color2;
                this->font_id = 8;
                this->add_string(position, buffer);
                this->font_id = 6;
                this->color = prev_color;
                break;
            }
        }
        this->add_string(position, buffer);
    }
public:
    // 0x419F30
    dllexport gnu_noinline static void stdcall print_score(Float3* position, uint32_t score, uint32_t continues) asm_symbol_rel(0x419F30) {
        ASCII_MANAGER_PTR->print_score_impl(position, score, continues);
    }

#if INCLUDE_PATCH_CODE
    dllexport gnu_noinline static void stdcall print_score_bigger(Float3* position, uint32_t score, uint32_t continues) {
        uint64_t big_score = score | (uint64_t)score_upper[1 + (*(uint32_t*)&position->y == 0x42800000)] << 32;

        char buffer[32];
        char* buffer_write = &buffer[31];
        *buffer_write = '\0';
        *--buffer_write = '0' + continues;
        uint8_t comma_counter = 2;

        while (big_score) {
            uint8_t digit = big_score % 10;
            big_score /= 10;
            *--buffer_write = '0' + digit;
            if (!--comma_counter && big_score) {
                comma_counter = 3;
                *--buffer_write = ',';
            }
        }

        AsciiManager* ascii_manager = ASCII_MANAGER_PTR;

        switch (ascii_manager->font_id) {
            case 10: {
                D3DCOLOR prev_color = ascii_manager->color;
                ascii_manager->color = ascii_manager->color2;
                ascii_manager->font_id = 11;
                ascii_manager->add_string(position, buffer_write);
                ascii_manager->font_id = 10;
                ascii_manager->color = prev_color;
                break;
            }
            case 7: {
                D3DCOLOR prev_color = ascii_manager->color;
                ascii_manager->color = ascii_manager->color2;
                ascii_manager->font_id = 9;
                ascii_manager->add_string(position, buffer_write);
                ascii_manager->font_id = 7;
                ascii_manager->color = prev_color;
                break;
            }
            case 6: {
                D3DCOLOR prev_color = ascii_manager->color;
                ascii_manager->color = ascii_manager->color2;
                ascii_manager->font_id = 8;
                ascii_manager->add_string(position, buffer_write);
                ascii_manager->font_id = 6;
                ascii_manager->color = prev_color;
                break;
            }
        }
        ascii_manager->add_string(position, buffer_write);
    }
#endif

    // 0x41A110
    dllexport gnu_noinline void cdecl debugf(Float3* position, const char* format, ...) asm_symbol_rel(0x41A110) {
        va_list va;
        va_start(va, format);
        char buffer[0x100];
        vsprintf(buffer, format, va);
        //va_end(va);
        this->add_string(position, buffer);
        this->strings[this->string_count - 1].font_id = 1;
    }

    // 0x419390
    dllexport gnu_noinline static AsciiManager* allocate() {
        AsciiManager* ascii_manager = new AsciiManager();

        const char* ascii_filename;
        if (WINDOW_DATA.__float_2070 <= 1.1f) {
            ascii_filename = "ascii.anm";
        }
        else if (WINDOW_DATA.__float_2070 <= 1.6f) {
            ascii_filename = "ascii_960.anm";
        }
        else {
            ascii_filename = "ascii1280.anm";
        }
        AnmLoaded* ascii_anm = ANM_MANAGER_PTR->preload_anm(2, ascii_filename);
        ascii_manager->ascii_anm = ascii_anm;
        if (!ascii_anm) {
            LOG_BUFFER.write(JpEnStr("", "data is corrupted\r\n"));
            delete ascii_manager;
            return NULL;
        }
        UpdateFunc* update_func = new UpdateFunc(&on_tick, false, ascii_manager);
        UpdateFuncRegistry::register_on_tick(update_func, 5);
        ascii_manager->on_tick_func = update_func;
        update_func = new UpdateFunc(&on_draw_group_0, false, ascii_manager);
        UpdateFuncRegistry::register_on_draw(update_func, 88);
        ascii_manager->on_draw_func = update_func;
        update_func = new UpdateFunc(&on_draw_group_1, false, ascii_manager);
        UpdateFuncRegistry::register_on_draw(update_func, 55);
        ascii_manager->on_draw_func_group_1 = update_func;
        update_func = new UpdateFunc(&on_draw_group_2, false, ascii_manager);
        UpdateFuncRegistry::register_on_draw(update_func, 72);
        ascii_manager->on_draw_func_group_2 = update_func;
        update_func = new UpdateFunc(&on_draw_group_3, false, ascii_manager);
        UpdateFuncRegistry::register_on_draw(update_func, 63);
        ascii_manager->on_draw_func_group_3 = update_func;

        ascii_manager->ascii_anm->__set_initial_sprite(&ascii_manager->__vm_C, 0);
        ascii_manager->ascii_anm->__set_initial_sprite(&ascii_manager->__vm_618, 98);
        return ascii_manager;
    }

private:
    inline void set_defaults_impl() {
        this->color = PackD3DCOLOR(255, 255, 255, 255);
        this->color2 = PackD3DCOLOR(255, 0, 0, 0);
        this->__dword_1923C = 0;
        this->__character_spacing_for_font_0 = 9;
        this->scale.x = 1.0f;
        this->scale.y = 1.0f;
        this->enable_shadows = FALSE;
        this->font_id = 0;
        this->group = 0;
        this->duration = 0;
        this->__horizontal_positioning_mode = 1;
        this->__vertical_positioning_mode = 1;
    }

public:
    // 0x416AE0
    dllexport gnu_noinline static void set_defaults() {
        ASCII_MANAGER_PTR->set_defaults_impl();
    }

    // 0x42CB60
    dllexport gnu_noinline static void __interrupt_1_and_wipe_vm_id_19268() {
        AnmID& anm_id = ASCII_MANAGER_PTR->__vm_id_19268;
        anm_id.interrupt_tree(1);
        anm_id = 0;
    }

    // 0x42CB90
    dllexport gnu_noinline static void vectorcall __instantiate_vm_id_19268(float x, float y) {
        AsciiManager* ascii_manager_ptr = ASCII_MANAGER_PTR;
        Float3 position = { x + x, y + y, 0.0f };
        if (!ascii_manager_ptr->__vm_id_19268) {
            ascii_manager_ptr->__vm_id_19268 = ascii_manager_ptr->ascii_anm->instantiate_vm_to_world_list_back(17, &position);
        }
    }
};
#pragma region // AsciiManager Validation
ValidateFieldOffset32(0x0, AsciiManager, task_flags);
ValidateFieldOffset32(0x4, AsciiManager, on_tick_func);
ValidateFieldOffset32(0x8, AsciiManager, on_draw_func);
ValidateFieldOffset32(0xC, AsciiManager, __vm_C);
ValidateFieldOffset32(0x618, AsciiManager, __vm_618);
ValidateFieldOffset32(0xC24, AsciiManager, strings);
ValidateFieldOffset32(0x19224, AsciiManager, string_count);
ValidateFieldOffset32(0x19228, AsciiManager, color);
ValidateFieldOffset32(0x1922C, AsciiManager, color2);
ValidateFieldOffset32(0x19230, AsciiManager, scale);
ValidateFieldOffset32(0x19240, AsciiManager, enable_shadows);
ValidateFieldOffset32(0x19244, AsciiManager, font_id);
ValidateFieldOffset32(0x19248, AsciiManager, group);
ValidateFieldOffset32(0x1924C, AsciiManager, duration);
ValidateFieldOffset32(0x19250, AsciiManager, __horizontal_positioning_mode);
ValidateFieldOffset32(0x19254, AsciiManager, __vertical_positioning_mode);
ValidateFieldOffset32(0x19258, AsciiManager, __character_spacing_for_font_0);
ValidateFieldOffset32(0x1925C, AsciiManager, frame_count);
ValidateFieldOffset32(0x19260, AsciiManager, ascii_anm);
ValidateFieldOffset32(0x19264, AsciiManager, __vm_id_19264);
ValidateFieldOffset32(0x19268, AsciiManager, __vm_id_19268);
ValidateFieldOffset32(0x1926C, AsciiManager, on_draw_func_group_1);
ValidateFieldOffset32(0x19270, AsciiManager, on_draw_func_group_2);
ValidateFieldOffset32(0x19274, AsciiManager, on_draw_func_group_3);
ValidateStructSize32(0x19278, AsciiManager);
#pragma endregion



typedef struct ShtFile ShtFile;
extern "C" {
    // 0x570920
    extern ShtFile* CACHED_SHT_FILE_PTR asm("_CACHED_SHT_FILE_PTR");
}

// size: 
struct ShtFile {
    unknown_fields(0x2); // 0x0
    uint16_t __word_2; // 0x2
    unknown_fields(0xDC); // 0x4
    void* __ptr_array_E0[]; // 0xE0


};

typedef struct Player Player;
extern "C" {
    // 0x4CF410
    extern Player* PLAYER_PTR asm("_PLAYER_PTR");
}

enum MovementDirection : int32_t {
    MovementNone = 0,
    MovementUp = 1,
    MovementDown = 2,
    MovementLeft = 3,
    MovementRight = 4,
    MovementUpLeft = 5,
    MovementUpRight = 6,
    MovementDownLeft = 7,
    MovementDownRight = 8
};

enum CollisionResult : int32_t {
    NoCollision = 0,
    DeathCollision = 1,
    GrazeCollision = 2
};

// size: 0x9C
struct PlayerDamageSource {
    union {
        uint32_t flags; // 0x0
    };
    float __float_4; // 0x4
    float __float_8; // 0x8
    float __float_C; // 0xC
    ZUNAngle __angle_10; // 0x10
    float __float_14; // 0x14
    float __float_18; // 0x18
    MotionData motion; // 0x1C
    Timer __timer_60; // 0x60
    int32_t __int_74; // 0x74
    int __int_78; // 0x78
    int __int_7C; // 0x7C
    int32_t __int_80; // 0x80
    EnemyID __enemy_id_84; // 0x84
    int32_t __int_88; // 0x88
    int __int_8C; // 0x8C
    int32_t __player_bullet_index; // 0x90
    EnemyID __enemy_id_94; // 0x94
    int32_t __unknown_func_index; // 0x98
    // 0x9C
};

// size: 0xF0
struct PlayerOption {
    unknown_fields(0x54); // 0x0
    uint32_t position_x; // 0x54
    uint32_t position_y; // 0x58
    unknown_fields(0x94); // 0x5C
    // 0xF0
};
// size: 0xF8
struct PlayerBullet {
    union {
        uint32_t flags; // 0x0
        struct {
            uint32_t __unknown_flag_A : 1;
        };
    };
    unknown_fields(0x4); // 0x0
    AnmID __vm_id_8; // 0x8
    Timer __timer_C; // 0xC
    MotionData motion; // 0x20
    unknown_fields(0x28); // 0x64
    int state; // 0x8C
    unknown_fields(0xC); // 0x90
    int __int_9C; // 0x9C
    unknown_fields(0xC); // 0xA0
    int __dword_AC; // 0xAC
    int32_t damage_source_index; // 0xB0
    unknown_fields(0x38); // 0xB4
    void* __func_ptr_EC; // 0xEC
    unknown_fields(0x4); // 0xF0
    void* __func_ptr_F4; // 0xF4
    // 0xF8
};

// size: 0x47308
struct PlayerData {
    Float3 position; // 0x0, 0x620
    Int2 internal_position; // 0xC, 0x62C
    Timer __timer_14; // 0x14, 0x634
    Timer __timer_28; // 0x28, 0x648
    Timer __timer_3C; // 0x3C, 0x65C
    PlayerOption options[4]; // 0x50, 0x670
    PlayerOption equipment[12]; // 0x410, 0xA30
    PlayerBullet bullets[0x200]; // 0xF50, 0x1570
    int32_t last_created_damage_source_index; // 0x1FF50, 0x20570
    PlayerDamageSource damage_sources[0x401]; // 0x1FF54, 0x20574
    PlayerDamageSource __damage_source_46FF0; // 0x46FF0, 0x47610
    int32_t state; // 0x4708C, 0x476AC
    AnmID __vm_id_47090; // 0x47090, 0x476B0
    AnmID __vm_id_47094; // 0x47094, 0x476B4
    Timer __timer_47098; // 0x47098, 0x476B8
    BOOL focused; // 0x470AC, 0x476CC
    Timer shoot_key_short_timer; // 0x470B0, 0x476D0
    Timer shoot_key_long_timer; // 0x470C4, 0x476E4
    int power; // 0x470D8, 0x476F8
    int __dword_array_470DC[30]; // 0x470DC, 0x476FC
    Timer __timer_47154; // 0x47154, 0x47774
    Timer __timer_47168; // 0x47168, 0x47788
    union {
        uint32_t flags; // 0x4717C, 0x4779C
        struct {
            uint32_t __unknown_flag_A : 1;
            uint32_t __unknown_flag_C : 1;
            uint32_t : 1;
            uint32_t __unknown_flag_B : 1;
            uint32_t __unknown_flag_F : 1;
            uint32_t __unknown_flag_E : 1;
            uint32_t __unknown_flag_G : 1;
            uint32_t __unknown_field_A : 2;
            uint32_t __unknown_flag_D : 1;
        };
    };
    Timer __timer_477A0; // 0x47180, 0x477A0
    int32_t __unfocused_linear_speed; // 0x47194, 0x477B4
    int32_t __focused_linear_speed; // 0x47198, 0x477B8
    int32_t __unfocused_diagonal_speed; // 0x4719C, 0x477BC
    int32_t __focused_diagonal_speed; // 0x471A0, 0x477C0
    Float3 velocity; // 0x471A4, 0x477C4
    Float3 __last_movement_velocity; // 0x471B0, 0x477D0
    Int3 __internal_velocity; // 0x471BC, 0x477DC
    int32_t __int_471C4; // 0x471C4, 0x477E4
    int32_t __int_471C8; // 0x471C8, 0x477E8
    float __speed_modifier; // 0x471CC, 0x477EC
    Float3 __base_axis_speed; // 0x471D0, 0x477F0
    unknown_fields(0x4); // 0x471DC, 0x477FC
    Int2 previous_positions[33]; // 0x471E0, 0x47800
    int32_t num_deathbomb_frames; // 0x472E8, 0x47908
    float __float_472EC; // 0x472EC, 0x4790C
    float __float_472F0; // 0x472F0, 0x47910
    Timer __timer_472F4; // 0x472F4, 0x47914
    // 0x47308, 0x47928

private:
    // 0x45D5E0
    dllexport gnu_noinline void thiscall __sub_45D5E0(int) {

    }
public:
    inline void __sub_45D5E0() {
        return this->__sub_45D5E0(UNUSED_DWORD);
    }
};

// size: 0x479D4
struct Player : ZUNTask {
    // ZUNTask base; // 0x0
    AnmLoaded* player_anm; // 0xC
    AnmLoaded* __player_anm_copy; // 0x10
    AnmVM __vm_14; // 0x14
    PlayerData data; // 0x620
    Float3 __float3_47928; // 0x47928
    Int2 __base_movement_velocity; // 0x47934
    MovementDirection movement_direction; // 0x4793C
    void* __ptr_47940; // 0x47940 (Sht file?)
    int __dword_47944; // 0x47944
    char __byte_47948; // 0x47948
    unknown_fields(0x3); // 0x47949
    ZUNInterp<float> scale_interp; // 0x4794C
    float scale; // 0x4797C
    float damage_multiplier; // 0x47980
    int __int_47984; // 0x47984
    float __item_attract_speed; // 0x47988
    float item_collect_radius; // 0x4798C
    float item_attract_radius_focused; // 0x47990
    float item_attract_radius_unfocused; // 0x47994
    float poc_height; // 0x47998
    float __float_4799C; // 0x4799C
    float __float_479A0; // 0x479A0
    Float2 __float2_479A4; // 0x479A4
    float __float_479AC; // 0x479AC
    Float2 __float2_479B0; // 0x479B0
    float __float_479B8; // 0x479B8
    unknown_fields(0x10); // 0x479BC
    float __float_479CC; // 0x479CC
    // 0x479D0

    inline void zero_contents() {
        zero_this();
    }

    inline Player() {
        this->zero_contents();
    }

    // 0x407D50
    dllexport gnu_noinline Float3* thiscall get_position() asm_symbol_rel(0x407D50) {
        return &this->data.position;
    }

    // 0x4099D0
    dllexport static gnu_noinline void stdcall __set_data_timer_47154(int32_t time) asm_symbol_rel(0x4099D0) {
        PLAYER_PTR->data.__timer_47154.initialize_and_set(time);
    }

    // 0x45CBA0
    dllexport static float angle_to_player_from_point(Float3* position) asm_symbol_rel(0x45CBA0) {
        Player* player = PLAYER_PTR;
        float y_pos = player->data.position.y - position->y;
        float x_pos = player->data.position.x - position->x;
        if (y_pos == 0.0f && x_pos == 0.0f) {
            return HALF_PI_f;
        } else {
            clang_forceinline return zatan2f(y_pos, x_pos);
        }
    }

    // 0x45D3A0
    dllexport gnu_noinline void thiscall die() asm_symbol_rel(0x45D3A0) {
        if (!this->data.__unknown_flag_A) {
            SoundManager::play_sound(2);
        }
        // TODO
    }

private:
    // Implementation may not actually use Float2 judging by how the compiler is using the stack
    inline CollisionResult __check_collision_rotated_rectangle_impl(Float2* position, float angle, float width, float length, BOOL graze_only) {
        Float2 A = (this->data.position - *position).rotate_around_origin(-angle);
        BOOL focused = this->data.focused;
        Float2 B;
        if (focused) {
            B = this->__float2_479B0;
        } else {
            B = this->__float2_479A4;
        }
        B *= 16.0f;
        Float2 min = A - B;
        Float2 max = A + B;
        if (
            !(min.x > length) &&
            !(min.y > width * 0.5f) &&
            !(max.x > 0.0f) &&
            !(max.y > -width * 0.5f)
        ) {
            if (focused) {
                B = this->__float2_479B0;
            } else {
                B = this->__float2_479A4;
            }
            min = A - B;
            max = A + B;
            if (
                !(min.x > length) &&
                !(min.y > width * 0.5f) &&
                !(max.x > 0.0f) &&
                !(max.y > -width * 0.5f)
            ) {
                if (!Gui::msg_is_active()) {
                    if (graze_only) {
                        return GrazeCollision;
                    }
                    switch (this->data.state) {
                        case 2: case 3: case 4:
                            return NoCollision;
                    }
                    if (this->data.__timer_47154 > 0) {
                        return NoCollision;
                    }
                    this->die();
                    return DeathCollision;
                }
            } else {
                return GrazeCollision;
            }
        }
        return NoCollision;
    }

    /*
    inline CollisionResult __check_collision_rotated_rectangle_impl(Float2* position, float angle, float width, float length, BOOL graze_only) {
        angle = -angle;
        float x = this->data.position.x - position->x;
        float y = this->data.position.y - position->y;
        float y_unit = zsinf(angle);
        float x_unit = zcosf(angle);
        BOOL focused = this->data.focused;
        float AX = (x_unit * x) - (y_unit * y);
        float AY = (x_unit * y) + (y_unit * x);
        float BX;
        float BY;
        if (focused) {
            BX = this->__float2_479B0.x;
            BY = this->__float2_479B0.y;
        } else {
            BX = this->__float2_479A4.x;
            BY = this->__float2_479A4.y;
        }
        BX *= 16.0f;
        BY *= 16.0f;
        float x_min = AX - BX;
        float x_max = AX + BX;
        float y_min = AY - BY;
        float y_max = AY + BY;
        if (
            !(x_min > length) &&
            !(y_min > width * 0.5f) &&
            !(x_max > 0.0f) &&
            !(y_max > -width * 0.5f)
        ) {
            if (focused) {
                BX = this->__float2_479B0.x;
                BY = this->__float2_479B0.y;
            } else {
                BX = this->__float2_479A4.x;
                BY = this->__float2_479A4.y;
            }
            x_min = AX - BX;
            x_max = AX + BX;
            y_min = AY - BY;
            y_max = AY + BY;
            if (
                !(x_min > length) &&
                !(y_min > width * 0.5f) &&
                !(x_max > 0.0f) &&
                !(y_max > -width * 0.5f)
            ) {
                if (!Gui::msg_is_active()) {
                    if (graze_only) {
                        return GrazeCollision;
                    }
                    switch (this->data.state) {
                        case 2: case 3: case 4:
                            return NoCollision;
                    }
                    if (this->data.__timer_47154 > 0) {
                        return NoCollision;
                    }
                    this->die();
                    return DeathCollision;
                }
            } else {
                return GrazeCollision;
            }
        }
        return NoCollision;
    }
    */

    // 0x45CEA0
    dllexport gnu_noinline static CollisionResult vectorcall __check_collision_rotated_rectangle(int, int, float, float, Float2* position, float angle, float width, float length, BOOL graze_only) asm_symbol_rel(0x45CEA0) {
        return PLAYER_PTR->__check_collision_rotated_rectangle_impl(position, angle, width, length, graze_only);
    }
public:
    static inline CollisionResult __check_collision_rotated_rectangle(Float2* position, float angle, float width, float length, BOOL graze_only) {
        return Player::__check_collision_rotated_rectangle(UNUSED_DWORD, UNUSED_DWORD, UNUSED_FLOAT, UNUSED_FLOAT, position, angle, width, length, graze_only);
    }

private:
    inline void do_graze_impl(Float2* position) {
        GAME_MANAGER.globals.add_graze();

        Float3 graze_position = *position + this->data.position * 0.5f;

        EFFECT_MANAGER_PTR->effect_anm->instantiate_vm_to_world_list_back(24, &graze_position);

        SOUND_MANAGER.play_sound_positioned(42, position->x);
    }
public:
    // 0x45E160
    dllexport gnu_noinline static void do_graze(Float2* position) asm_symbol_rel(0x45E160) {
        return PLAYER_PTR->do_graze_impl(position);
    }

private:
    inline CollisionResult check_collision_circle_impl(Float2* position, float radius, BOOL graze_only) {
        float distance_squared = this->data.position.distance_squared(position);
        float player_radius;
        if (this->data.focused) {
            player_radius = this->__float_479A0;
        } else {
            player_radius = this->__float_4799C;
        }
        if (this->data.__unknown_flag_F) {
            player_radius *= this->scale * 3.6f;
        }
        float radius_squared = radius * radius;
        if (player_radius * player_radius + radius_squared <= distance_squared) {
            radius /= 2.5f;
            float A = __max(radius, 40.0f) + player_radius;
            if (A * A + radius_squared <= distance_squared) {
                return NoCollision;
            }
            return GrazeCollision;
        }
        if (Gui::msg_is_active()) {
            return NoCollision;
        }
        if (graze_only) {
            return GrazeCollision;
        }
        switch (this->data.state) {
            case 2: case 3: case 4:
                return NoCollision;
        }
        if (this->data.__timer_47154 <= 0) {
            this->die();
        }
        return DeathCollision;
    }

    // 0x45CD90
    dllexport static gnu_noinline CollisionResult vectorcall check_collision_circle(int, int, float, float, Float2* position, float radius, BOOL graze_only) asm_symbol_rel(0x45CD90) {
        return PLAYER_PTR->check_collision_circle_impl(position, radius, graze_only);
    }

public:
    static inline CollisionResult check_collision_circle(Float2* position, float radius, BOOL graze_only) {
        return check_collision_circle(UNUSED_DWORD, UNUSED_DWORD, UNUSED_FLOAT, UNUSED_FLOAT, position, radius, graze_only);
    }

    // 0x45A7A0
    dllexport gnu_noinline ZUNResult thiscall initialize() {
        // TODO
        return ZUN_SUCCESS;
    }

    // 0x45AF20
    dllexport gnu_noinline static Player* allocate() {
        Player* player = new Player();
        PLAYER_PTR = player;
        if (ZUN_FAILED(player->initialize())) {
            delete player;
            return NULL;
        }
        return player;
    }
};

typedef struct PopupManager PopupManager;
extern "C" {
    // 0x4CF420
    extern PopupManager* POPUP_MANAGER_PTR asm("_POPUP_MANAGER_PTR");
}

// size: 0x48
struct Popup {
    uint8_t digits[12]; // 0x0
    Float3 position; // 0xC
    float __float_18; // 0x18
    D3DCOLOR color; // 0x1C
    Timer __timer_20; // 0x20
    unknown_fields(0x8); // 0x34
    bool alive; // 0x3C
    uint8_t digit_count; // 0x3D
    unknown_fields(0xA); // 0x3E
    // 0x48
};
#pragma region // Popup Validation

#pragma endregion

// size: 0x1044
struct PopupManager : ZUNTask {
    // ZUNTask base; // 0x0
    AnmLoaded* ascii_anm; // 0xC
    int32_t __index_10; // 0x10
    unknown_fields(0x4); // 0x14
    AnmVM __vm_18; // 0x18
    Popup popups[18]; // 0x624
    // 0xCE4

    inline void zero_contents() {
        zero_this();
    }

    inline PopupManager() {
        this->zero_contents();
        this->__unknown_task_flag_A = true;
    }

    // 0x464020
    dllexport gnu_noinline UpdateFuncRet thiscall on_tick() asm_symbol_rel(0x464020) {

        return UpdateFuncNext;
    }

    // 0x4645D0
    dllexport gnu_noinline static UpdateFuncRet fastcall on_tick(void* ptr) {
        return ((PopupManager*)ptr)->on_tick();
    }

    // 0x464180
    dllexport gnu_noinline UpdateFuncRet thiscall on_draw() asm_symbol_rel(0x464180) {
        Popup* popup = this->popups;
        SUPERVISOR.d3d_disable_fog();

        for (size_t i = 0; i < 13; ++i) {

        }

        return UpdateFuncNext;
    }

    // 0x4645E0
    dllexport gnu_noinline static UpdateFuncRet fastcall on_draw(void* ptr) {
        return ((PopupManager*)ptr)->on_draw();
    }

    // 0x463DD0
    dllexport gnu_noinline PopupManager* allocate() {
        PopupManager* popup_manager = new PopupManager();
        POPUP_MANAGER_PTR = popup_manager;
        popup_manager->ascii_anm = ASCII_MANAGER_PTR->ascii_anm;

        UpdateFunc* update_func = new UpdateFunc(&PopupManager::on_tick, false, popup_manager);
        UpdateFuncRegistry::register_on_tick(update_func, 21);
        popup_manager->on_tick_func = update_func;
        update_func = new UpdateFunc(&PopupManager::on_draw, false, popup_manager);
        UpdateFuncRegistry::register_on_draw(update_func, 48);
        popup_manager->on_draw_func = update_func;

        AnmLoaded* ascii_anm = popup_manager->ascii_anm;
        popup_manager->__vm_18.reset();
        ascii_anm->set_sprite(&popup_manager->__vm_18, 289);
        popup_manager->__vm_18.data.origin_mode = 2;
        popup_manager->__vm_18.data.resolution_mode = 3;
        return popup_manager;
    }

    // 0x4645F0
    dllexport gnu_noinline static void stdcall create_popup(Float3* position, int32_t value, D3DCOLOR color) {
        PopupManager* popup_manager = POPUP_MANAGER_PTR;

        int32_t index = popup_manager->__index_10;
        if (index >= 10) {
            popup_manager->__index_10 = 0;
            index = 0;
        }

        Popup* popup = &popup_manager->popups[index];

        popup->alive = true;

        int32_t digit_count = 0;
        if (value >= 0) {
            while (value > 0) {
                int32_t next_value = value / 10;
                uint8_t digit = next_value;
                popup->digits[digit_count++] = value - digit * 10;
                value = next_value;
            }
            if (!digit_count) {
                popup->digits[digit_count++] = 0;
            }
        }
        else {
            popup->digits[digit_count++] = 10; // ???
        }
        popup->digit_count = digit_count;
        popup->color = color;
        popup->__timer_20.reset();
        popup->position = *position;
        popup->__float_18 = 1.0f;

        ++popup_manager->__index_10;
    }
};
#pragma region // PopupManager Validation

#pragma endregion

typedef struct AbilityManager AbilityManager;
typedef struct AbilityTextData AbilityTextData;
typedef struct AbilityMenu AbilityMenu;

typedef struct EnemyManager EnemyManager;

extern "C" {
    // 0x4CF298
    extern AbilityManager* ABILITY_MANAGER_PTR asm("_ABILITY_MANAGER_PTR");
    // 0x4CF29C
    extern AbilityTextData* ABILITY_TEXT_DATA_PTR asm("_ABILITY_TEXT_DATA_PTR");
    // 0x4CF2A0
    extern AbilityMenu* ABILITY_MENU_PTR asm("_ABILITY_MENU_PTR");
    // 0x4CF2D4
    extern int32_t UNKNOWN_COUNTER_A asm("_UNKNOWN_COUNTER_A");

    // 0x4CF2D0
    extern EnemyManager* ENEMY_MANAGER_PTR asm("_ENEMY_MANAGER_PTR");
}

static inline bool enemies_are_alive();
static inline float ability_manager_get_float_C58();
static inline AnmLoaded* ability_manager_get_ability_anm();

// size: 0x54
struct CardBase {
    // void* vtable; // 0x0
    int32_t id; // 0x4
    int32_t __array_index; // 0x8
    ZUNList<CardBase> list_node; // 0xC
    AnmID effect_vm_id; // 0x1C
    Timer __timer_20; // 0x20
    Timer __timer_34; // 0x34
    int32_t recharge_time; // 0x48
    const CardData* data; // 0x4C
    union {
        uint32_t flags; // 0x50
        struct {
            uint32_t __unknown_flag_F : 1; // 1
            uint32_t __unknown_flag_B : 1; // 2
            uint32_t __unknown_flag_C : 1; // 3
            uint32_t __is_active_card : 1; // 4
            uint32_t : 1; // 5
            uint32_t __unknown_flag_A : 1; // 6
            uint32_t __is_equipment_card : 1; // 7
            uint32_t : 1; // 8
        };
    };
    // 0x54

    inline void zero_contents() {
        zero_this();
    }

    inline CardBase() {
        this->zero_contents();
        this->id = -1;
        this->list_node.initialize_with(this);
    }

    // Method 0
    // 0x413010
    dllexport gnu_noinline virtual ZUNResult thiscall initializeA() {
        return ZUN_SUCCESS;
    }
    // Method 4
    // 0x413020
    dllexport gnu_noinline virtual ZUNResult thiscall initializeB() {
        return ZUN_SUCCESS;
    }
    // Method 8
    // 0x413030
    dllexport gnu_noinline virtual int thiscall on_activate() {
        return 0;
    }
    // Method C
    // 0x413040
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb(int) {
        return 0;
    }
    // Method 10
    // 0x413050
    dllexport gnu_noinline virtual int thiscall on_player_death_pre_deathbomb() {
        return 0;
    }
    // Method 14
    // 0x413060
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb_frame_2() {
        return 0;
    }
    // Method 18
    // 0x413070
    dllexport gnu_noinline virtual int thiscall on_power_level_change() {
        return 0;
    }
    // Method 1C
    // 0x413080
    dllexport gnu_noinline virtual int thiscall on_shoot(int, int) {
        return 0;
    }
    // Method 20
    // 0x413090
    dllexport gnu_noinline virtual int thiscall on_load() {
        return 0;
    }
    // Method 24
    // 0x4130A0
    dllexport gnu_noinline virtual int thiscall on_tick() {
        return 0;
    }
    // Method 28
    // 0x4130B0
    dllexport gnu_noinline virtual int thiscall __on_bullet_init(int) {
        return 0;
    }
    // Method 2C
    // 0x4130C0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        return 0;
    }
    // Method 30
    // 0x4130D0
    dllexport gnu_noinline virtual int thiscall recharge(int, int) {
        return 0;
    }
    // Method 34
    // 0x4130E0
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
    }
    // Method 38
    // 0x4130F0
    dllexport gnu_noinline virtual int thiscall __set_replay_state(int32_t value) {
        this->__timer_34.set(value);
        return 0;
    }
    // Method 3C
    // 0x413130
    dllexport gnu_noinline virtual int32_t thiscall __get_replay_state() {
        return this->__timer_34.current;
    }
    // Method 40
    // 0x413140
    dllexport gnu_noinline virtual bool thiscall __get_unknown_flag_A() {
        return this->__unknown_flag_A;
    }
    // Method 44
    // 0x413050
    dllexport gnu_noinline virtual int thiscall on_anm_id_assigned_to_hud(AnmID id) {
        return 0;
    }
    // Method 48
    // 0x413060
    dllexport gnu_noinline virtual int thiscall on_draw() {
        return 0;
    }
    // Method 4C
    // 0x413070
    dllexport gnu_noinline virtual void thiscall __method_4C() {
    }
    // Method 50
    // 0x412ED0
    virtual ~CardBase() {
        this->list_node.unlink();
    }

    // 0x408BF0
    dllexport int thiscall __dec_timer_34_during_gameplay() asm_symbol_rel(0x408BF0) {
        if (!GUI_PTR->msg_is_active() && enemies_are_alive()) {
            if (this->__timer_34 > 0) {
                this->__timer_34--;
            }
        }
        return 0;
    }

    inline void __inc_timer_20_during_gameplay() {
        if (!GUI_PTR->msg_is_active() && enemies_are_alive()) {
            ++this->__timer_20;
        }
    }
};

// size: 0x58
struct CardKoishi : CardBase {
    // CardBase base; // 0x0
    int32_t power; // 0x54
    // 0x58

    static inline constexpr CardId ID = KOISHI_CARD; // 27

    inline CardKoishi() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }

    // Method C
    // 0x40D4E0
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb(int) {
        this->power = GAME_MANAGER.globals.current_power;
        return 0;
    }
    // Method 14
    // 0x40D4F0
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb_frame_2() {
        this->power -= 50;
        int32_t new_power = this->power;
        if (new_power < 100) {
            this->power = 100;
            new_power = 100;
        }
        int32_t max_power = GAME_MANAGER.globals.max_power;
        if (new_power > max_power) {
            GAME_MANAGER.globals.current_power = max_power;
        }
        else {
            if (new_power < GAME_MANAGER.globals.power_per_level) {
                new_power = GAME_MANAGER.globals.power_per_level;
            }
            GAME_MANAGER.globals.current_power = new_power;
        }
        return 0;
    }
    // Method 2C
    // 0x40D4C0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        /*
        if (EnemyManager* enemy_manager = ENEMY_MANAGER_PTR) {
            enemy_manager->__int_164 = 1;
        }
        */
        return 0;
    }
    // Method 34
    // 0x40D4A0
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__method_4C();
    }
    // Method 4C
    // 0x40D4B0
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
    }
};

struct CardKaguya : CardBase {
    static inline constexpr CardId ID = DEAD_SPELL_CARD; // 31

    inline CardKaguya() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }

    // Method 14
    // 0x40D2E0
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb_frame_2() {
        //ITEM_MANAGER_PTR->spawn_item(7, )
    }
};

// size: 0x58
struct CardMamizou : CardBase {
    // CardBase base; // 0x0
    int32_t power; // 0x54
    // 0x58

    static inline constexpr CardId ID = POWERMAX_CARD; // 32

    inline CardMamizou() {
        this->__unknown_flag_B = true;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }

    // Method 4
    // 0x40D390
    dllexport gnu_noinline virtual int thiscall __method_4() {
        if (GAME_THREAD_PTR && this->__unknown_flag_B) {
            BOOL A = GAME_MANAGER.globals.__add_power(100);
            this->__unknown_flag_B = false;
            if (A) {
                if (Player* player = PLAYER_PTR) {
                    player->data.__sub_45D5E0();
                    POPUP_MANAGER_PTR->create_popup(&PLAYER_PTR->data.position, -1, PackD3DCOLOR(64, 255, 255, 255));
                    SOUND_MANAGER.play_sound_positioned(13, PLAYER_PTR->data.position.x);
                }
            }
        }
        return 0;
    }
    // Method C
    // 0x40D400
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb(int) {
        this->power = GAME_MANAGER.globals.current_power;
        return 0;
    }
    // Method 14
    // 0x40D410
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb_frame_2() {
        int32_t A = GAME_MANAGER.globals.power_per_level;
        int32_t new_power = this->power;
        
        int32_t B = A * 3;
        if (new_power > B) {
            this->power = B;
            new_power = B;
            A = GAME_MANAGER.globals.power_per_level;
        }

        int32_t max_power = GAME_MANAGER.globals.max_power;
        if (new_power > max_power) {
            GAME_MANAGER.globals.current_power = max_power;
        } else {
            if (new_power < A) {
                new_power = A;
            }
            GAME_MANAGER.globals.current_power = new_power;
        }
        return 0;
    }
    // Method 34
    // 0x40D380
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        if (SUPERVISOR.__int_804) {
            this->__method_4();
        }
    }
    // Method 4C
    // 0x40D370
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = true;
    }
};

struct CardYuyuko : CardBase {
    static inline constexpr CardId ID = YUYUKO_CARD; // 33

    inline CardYuyuko() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }

    // Method 2C
    // 0x40D5A0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        /*
        if (BulletManager* bullet_manager = BULLET_MANAGER_PTR) {
            bullet_manager->__unknown_counter_flag = 1;
        }
        */
        return 0;
    }
    // Method 34
    // 0x40D580
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__method_4C();
    }
    // Method 4C
    // 0x40D590
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
    }
};
struct CardYachie : CardBase {
    static inline constexpr CardId ID = MONEY_CARD; // 34

    inline CardYachie() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }

    // Method 30
    // 0x40D630
    dllexport gnu_noinline virtual int thiscall recharge(int, int) {
        float A = REPLAY_RNG.rand_float_signed() * PI_f;
        // TODO
        return 0;
    }
    // Method 34
    // 0x40D610
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__method_4C();
    }
    // Method 4C
    // 0x40D620
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
    }
};

// size: 0x64
struct CardShikiEiki : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    AnmID __vm_id_58; // 0x58
    void* __ptr_5C; // 0x5C PlayerDamageSource*?
    unknown_fields(0x4); // 0x60
    // 0x64

    static inline constexpr CardId ID = ROKUMON_CARD; // 35

    inline CardShikiEiki() {
        this->__vm_id_58 = NULL;
        this->__ptr_5C = NULL;
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }

    // 0x40D840
    dllexport gnu_noinline int thiscall __sub_40D840() asm_symbol_rel(0x40D840) {
        ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(41, &PLAYER_PTR->data.position);
        SOUND_MANAGER.play_sound(44);
        // TODO
        //SPELLCARD_PTR->__sub_409AD0();
        GAME_MANAGER.globals.current_money -= 200;
        PLAYER_PTR->data.__timer_47154.set(60);
        // ++ENEMY_MANAGER_PTR->player_bomb_count;
        // ENEMY_MANAGER_PTR->can_capture_spell = FALSE;
        // TODO
        this->__timer_20.reset();
        this->__int_54 = 1;
        return 0;
    }
    // Method C
    // 0x40DA10
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb(int) {
        if (GAME_MANAGER.globals.current_money >= 200) {
            this->__sub_40D840();
            PLAYER_PTR->data.__timer_14.set(60);
            PLAYER_PTR->data.state = 1;
            return 1;
        }
        return 0;
    }
    // Method 24
    // 0x40DA80
    dllexport gnu_noinline virtual int thiscall on_tick() {
        switch (this->__int_54) {
            case 1:
                // TODO
                if (this->__timer_20 >= 60) {
                    this->__int_54 = 0;
                    BOMB_PTR->__dword_A0 = 0;
                }
                break;
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x40D800
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__method_4C();
    }
    // Method 4C
    // 0x40D810
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__vm_id_58.mark_tree_for_delete();
        this->__int_54 = 0;
        this->__ptr_5C = NULL;
    }
};


struct CardMike : CardBase {
    static inline constexpr CardId ID = MANEKI_CARD; // 38

    inline CardMike() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }
};
struct CardTakane : CardBase {
    static inline constexpr CardId ID = YAMAWARO_CARD; // 39

    inline CardTakane() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }
};
struct CardSannyo : CardBase {
    static inline constexpr CardId ID = KISERU_CARD; // 40

    inline CardSannyo() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }
};

// size: 0x68
struct CardShinmyoumaru : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    int __dword_64; // 0x64
    // 0x68

    static inline constexpr CardId ID = KOZUCHI_CARD; // 42

    inline CardShinmyoumaru() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 2400;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x40F0F0
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            this->position = PLAYER_PTR->data.position;
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(30, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            SOUND_MANAGER.play_sound_positioned(30, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            this->__dword_64 = 0;
        }
        return 0;
    }
    // Method C
    // 0x40F0D0
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb(int) {
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
        return 0;
    }
    // Method 2C
    // 0x40EED0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 3:
                this->__unknown_flag_A = true;
                if (this->__timer_20 < 10) {
                    float A = (float)this->__timer_20 * 4.0f + 30.0f;
                    // TODO
                }
                if (this->__timer_20 > 40) {
                    this->__int_54 = 0;
                    this->effect_vm_id.mark_tree_for_delete();
                }
                break;
            case 2:
                this->__unknown_flag_A = true;
                if (this->__timer_20 > 10) {
                    this->__int_54 = 3;
                    // TODO, something inlined from sound manager
                    SOUND_MANAGER.play_sound_positioned(41, this->position.x);
                    this->__timer_20.reset();
                    this->position.y -= 84.0f;
                }
                break;
            case 1:
                this->position = PLAYER_PTR->data.position;
                this->position.y -= 16.0f;
                this->effect_vm_id.set_controller_position(&this->position);
                if (this->__timer_20 > 20) {
                    this->__int_54 = 2;
                    this->__timer_20.reset();
                }
                break;
            case 0:
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x40EE00
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x40EE50
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};

// size: 0x74
struct CardTenshi : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    Float3 __float3_64; // 0x64
    int __dword_70; // 0x70
    // 0x74

    static inline constexpr CardId ID = KANAME_CARD; // 43

    inline CardTenshi() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 3600;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x40EBF0
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            this->position = PLAYER_PTR->data.position;
            this->position.y -= 100.0f;
            this->__float3_64 = this->position;
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(28, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            SOUND_MANAGER.play_sound_positioned(77, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            this->__unknown_flag_A = true;
            this->__dword_70 = 0;
        }
        return 0;
    }
    // Method 2C
    // 0x40E8C0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 2:
                this->__unknown_flag_A = false;
                if (this->__timer_20 > 8) {
                    this->__int_54 = 0;
                    this->effect_vm_id.mark_tree_for_delete();
                }
                break;
            case 1:
                if (this->__timer_20 >= 30) {
                    this->__unknown_flag_A = false;
                }
                this->__float3_64 = PLAYER_PTR->data.position;
                this->__float3_64.y -= 80.0f;
                static constexpr float scale = 1.0f / 25.0f;
                this->position += (this->__float3_64 - this->position) * scale;
                this->effect_vm_id.set_controller_position(&this->position);
                if (this->__timer_20 > 1800) {
                    this->effect_vm_id.interrupt_tree(1);
                    this->__int_54 = 2;
                    this->__timer_20.reset();
                    SOUND_MANAGER.play_sound_positioned(41, this->position.x);
                }
                break;
            case 0:
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        if (this->__int_54 == 1) {
            // TODO
        }
        this->__inc_timer_20_during_gameplay();
        return 0;
    }
    // Method 34
    // 0x40E7F0
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x40E840
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};

// size: 0xC4
struct CardClownpiece : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    ZUNInterp<Float3> position_interp; // 0x64
    void* __ptr_BC; // 0xBC PlayerDamageSource*?
    float __float_C0; // 0xC0
    // 0xC4

    static inline constexpr CardId ID = MOON_CARD; // 44

    inline CardClownpiece() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 2700;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x40E040
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            Float3 A = this->position_interp.initial_value = this->position = PLAYER_PTR->data.position;
            A.y -= 400.0f;
            this->position_interp.mode = 1;
            this->position_interp.end_time = 120;
            this->position_interp.final_value = A;
            this->position_interp.time.reset();
            // TODO
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(45, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            this->__float_C0 = 0;
            SOUND_MANAGER.play_sound_positioned(77, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
        }
        return 0;
    }
    // Method 2C
    // 0x40DCE0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 2:
                this->__unknown_flag_A = true;
                this->position = this->position_interp.step();
                this->position.y += this->__float_C0;
                this->effect_vm_id.set_controller_position(&this->position);
                if (this->__timer_20 > 40) {
                    this->__int_54 = 0;
                    this->effect_vm_id.mark_tree_for_delete();
                }
                break;
            case 1:
                this->__unknown_flag_A = true;
                this->position = this->position_interp.step();
                this->position.y += this->__float_C0;
                this->effect_vm_id.set_controller_position(&this->position);
                if (this->__timer_20 > 80) {
                    this->__int_54 = 2;
                    this->effect_vm_id.interrupt_tree(1);
                    this->__timer_20.reset();

                }
                break;
            case 0:
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        if (this->__int_54) {
            // TODO
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x40DC10
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x40DC60
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};

// size: 0x68
struct CardMiko : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    void* __ptr_64; // 0x64 PlayerDamageSource*?
    // 0x68

    static inline constexpr CardId ID = MIKOFLASH_CARD; // 45

    inline CardMiko() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 1800;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x40E5C0
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            this->position = PLAYER_PTR->data.position;
            // TODO
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(47, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            SOUND_MANAGER.play_sound_positioned(77, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
        }
        return 0;
    }
    // Method 2C
    // 0x40E3D0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 1:
                this->__unknown_flag_A = true;
                this->position = PLAYER_PTR->data.position;
                this->effect_vm_id.set_controller_position(&PLAYER_PTR->data.position);
                if (this->__timer_20 > 130) {
                    this->__int_54 = 0;
                    this->effect_vm_id.mark_tree_for_delete();
                }
                if (
                    (uint32_t)this->__timer_20.current - 40 <= 79 &&
                    this->__timer_20.is_multiple_of(8)
                ) {
                    // TODO
                }
                break;
            case 0:
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x40E300
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x40E350
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};

// size: 0x68
struct CardRemilia : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    int __dword_64; // 0x64
    // 0x68

    static inline constexpr CardId ID = VAMPIRE_CARD; // 46

    inline CardRemilia() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 1200;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x40F670
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            this->position = PLAYER_PTR->data.position;
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(38, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            SOUND_MANAGER.play_sound_positioned(30, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            this->__dword_64 = 0;
        }
        return 0;
    }
    // Method C
    // 0x40F650
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb(int) {
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
        return 0;
    }
    // Method 2C
    // 0x40F3A0
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 2:
                this->position = PLAYER_PTR->data.position;
                this->effect_vm_id.set_controller_position(&PLAYER_PTR->data.position);
                if (this->__timer_20 < 50) {
                    Float3 A = this->position;
                    float B;
                    if (this->__timer_20 < 10) {
                        B = (float)this->__timer_20 * 90.0f / 10.0f;
                    } else {
                        B = 90.0f;
                    }
                    A.y -= B * 0.5f;
                    // TODO
                }
                if (this->__timer_20 > 60) {
                    this->__int_54 = 0;
                    this->effect_vm_id.mark_tree_for_delete();
                }
                break;
            case 1:
                this->position = PLAYER_PTR->data.position;
                this->effect_vm_id.set_controller_position(&PLAYER_PTR->data.position);
                if (this->__timer_20 > 30) {
                    this->__int_54 = 2;
                    this->__timer_20.reset();
                    SOUND_MANAGER.play_sound_positioned(27, this->position.x);
                }
                break;
            case 0:
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x40F2D0
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x40F320
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};

// size: 0x6C
struct CardUtsuho : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    int __dword_64; // 0x64
    void* __ptr_68; // 0x68 PlayerDamageSource*?
    // 0x6C

    static inline constexpr CardId ID = SUN_CARD; // 47

    inline CardUtsuho() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 18000;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x40FB60
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            //SPELLCARD_PTR->__sub_409AD0();
            // TODO
            this->position = PLAYER_PTR->data.position;
            // TODO
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(50, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            SOUND_MANAGER.play_sound_positioned(49, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            this->__dword_64 = 0;
        }
        return 0;
    }
    // Method C
    // 0x40FB40
    dllexport gnu_noinline virtual int thiscall on_player_death_after_deathbomb(int) {
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
        return 0;
    }
    // Method 2C
    // 0x40F920
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 2:
                this->__unknown_flag_A = true;
                if (this->__timer_20 > 60) {
                    this->__int_54 = 0;
                    this->effect_vm_id.mark_tree_for_delete();
                }
                break;
            case 1: {
                this->__unknown_flag_A = true;
                /*
                if (SPELLCARD_PTR) {
                    Spellcard::__sub_409AD0();
                }
                */
                if (GUI_PTR->msg_vm) {
                    this->__int_54 = 2;
                    this->__timer_20.reset();
                    this->effect_vm_id.interrupt_tree(1);
                }
                // TODO
                float A = this->effect_vm_id.get_vm_ptr()->data.scale.x * 96.0f;
                if (this->__timer_20 > 600) {
                    this->__int_54 = 2;
                    this->__timer_20.reset();
                    SOUND_MANAGER.play_sound_positioned(27, this->position.x);
                }
                if (
                    this->__timer_20.is_multiple_of(6) &&
                    GAME_MANAGER.globals.subtract_power(1)
                ) {
                    PLAYER_PTR->data.__sub_45D5E0();
                }
                break;
            }
            case 0:
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x40F850
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x40F8A0
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};

// size: 0x58
struct CardLilyWhite : CardBase {
    // CardBase base; // 0x0
    int32_t __int_54; // 0x54
    // 0x58

    static inline constexpr CardId ID = LILY_CARD; // 48

    inline CardLilyWhite() {
        this->recharge_time = 7200;
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_34.reset();
        this->__int_54 = 0;
    }

    // Method 8
    // 0x40FE70
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (this->recharge_time <= 0) {
            if (EnemyManager* enemy_manager = ENEMY_MANAGER_PTR) {
                // TODO, needs full enemy manager access
                this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
                ++this->__int_54;
                this->__unknown_flag_A = true;
            }
        }
        return 0;
    }
    // Method 2C
    // 0x40FE20
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        this->__dec_timer_34_during_gameplay();
        if (this->__timer_34 < this->recharge_time - 8) {
            this->__unknown_flag_A = false;
        }
        return 0;
    }
    // Method 34
    // 0x40FDD0
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
    }
    // Method 38
    // 0x40FFA0
    dllexport gnu_noinline virtual int thiscall __set_replay_state(int32_t value) {
        this->__timer_34.set((uint16_t)value);
        this->__int_54 = value >> 16;
        return 0;
    }
    // Method 3C
    // 0x40FFE0
    dllexport gnu_noinline virtual int32_t thiscall __get_replay_state() {
        return (uint16_t)this->__timer_34.current | this->__int_54 << 16;
    }
    // Method 4C
    // 0x40FDE0
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_34.reset();
        this->__int_54 = 0;
    }
};

// size: 0x68
struct CardRaiko : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    int __dword_64; // 0x64
    // 0x68

    static inline constexpr CardId ID = BASSDRUM_CARD; // 49

    inline CardRaiko() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 600;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x410250
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            this->position = PLAYER_PTR->data.position;
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(58, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            SOUND_MANAGER.play_sound_positioned(51, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            this->__dword_64 = 0;
        }
        return 0;
    }
    // Method 2C
    // 0x410110
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 1:
                this->__unknown_flag_A = true;
                // TODO
                if (this->__timer_20 >= 30) {
                    this->__int_54 = 0;
                    this->__timer_20.reset();
                    this->effect_vm_id.mark_tree_for_delete();
                }
                break;
            case 0:
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x410040
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x410090
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};

// size: 0x68
struct CardSumireko : CardBase {
    // CardBase base; // 0x0
    int __int_54; // 0x54
    Float3 position; // 0x58
    int __dword_64; // 0x64
    // 0x68

    static inline constexpr CardId ID = PSYCHO_CARD; // 50

    inline CardSumireko() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_20.reset();
        this->recharge_time = 1500;
        this->__int_54 = 0;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x410780
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (!this->__int_54 && this->recharge_time <= 0) {
            this->position = PLAYER_PTR->data.position;
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(60, &this->position, 13);
            this->__int_54 = 1;
            this->__timer_20.reset();
            SOUND_MANAGER.play_sound_positioned(51, this->position.x);
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            this->__dword_64 = 0;
        }
        return 0;
    }
    // Method 2C
    // 0x410500
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        switch (this->__int_54) {
            case 1:
                this->effect_vm_id.set_controller_position(&this->position);
                this->__unknown_flag_A = true;
                // TODO
                if (this->__timer_20 >= 120) {
                    this->__int_54 = 0;
                    this->__timer_20.reset();
                    this->effect_vm_id.mark_tree_for_delete();
                }
                break;
            case 0: 
                this->__unknown_flag_A = false;
                this->__dec_timer_34_during_gameplay();
                break;
        }
        ++this->__timer_20;
        return 0;
    }
    // Method 34
    // 0x410430
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->effect_vm_id.mark_tree_for_delete();
    }
    // Method 4C
    // 0x410480
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_20.reset();
        this->__int_54 = 0;
        this->__timer_34.reset();
        this->effect_vm_id.mark_tree_for_delete();
    }
};


struct CardMagtama : CardBase {
    static inline constexpr CardId ID = MAGATAMA_CARD; // 51

    inline CardMagtama() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }
};
struct CardTsukasa : CardBase {
    static inline constexpr CardId ID = CYLINDER_CARD; // 52

    inline CardTsukasa() {
        this->recharge_time = 1200;
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x410E60
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (this->__timer_34 <= 0) {
            if (GAME_MANAGER.globals.current_power < GAME_MANAGER.globals.power_per_level * 2) {
                SOUND_MANAGER.play_sound(16);
                return 0;
            }
            BombBase* bomb_ptr = BOMB_PTR;
            if (!bomb_ptr->bomb_active && !bomb_ptr->__dword_A0) {
                bomb_ptr->bomb_active = TRUE;
                bomb_ptr->__timer_34.reset();
                // TODO
                SOUND_MANAGER.play_sound_positioned(44, PLAYER_PTR->data.position.x);
                bomb_ptr->activate();
                UNKNOWN_COUNTER_A = 0;
                // ENEMY_MANAGER_PTR->can_capture_spell = FALSE;
                GAME_MANAGER.globals.subtract_power(GAME_MANAGER.globals.power_per_level);
                PLAYER_PTR->data.__sub_45D5E0();
                this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            }
        }
        return 0;
    }
    // Method 2C
    // 0x410E10
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        this->__dec_timer_34_during_gameplay();
        if (this->__timer_34 < this->recharge_time - 8) {
            this->__unknown_flag_A = false;
        }
        return 0;
    }
    // Method 34
    // 0x410DD0
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
    }
    // Method 4C
    // 0x410DE0
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_34.reset();
    }
};
struct CardMegumu : CardBase {
    static inline constexpr CardId ID = RICEBALL_CARD; // 53

    inline CardMegumu() {
        this->recharge_time = 5400;
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = false;
        this->__is_active_card = true;
        this->__is_equipment_card = false;
        this->__timer_34.reset();
    }

    // Method 8
    // 0x410BD0
    dllexport gnu_noinline virtual int thiscall on_activate() {
        if (this->__timer_34 < 0) {
            if (GAME_MANAGER.globals.current_power >= GAME_MANAGER.globals.max_power) {
                GAME_MANAGER.globals.add_bomb_fragment();
                SOUND_MANAGER.play_sound_positioned(13, PLAYER_PTR->data.position.x);
            }
            else {
                if (GAME_MANAGER.globals.__add_power(100)) {
                    PLAYER_PTR->data.__sub_45D5E0();
                    POPUP_MANAGER_PTR->create_popup(&PLAYER_PTR->data.position, -1, PackD3DCOLOR(64, 255, 255, 255));
                    SOUND_MANAGER.play_sound_positioned(13, PLAYER_PTR->data.position.x);
                }
            }
            this->__timer_34.set(this->recharge_time * ability_manager_get_float_C58());
            this->effect_vm_id = ability_manager_get_ability_anm()->instantiate_vm_to_world_list_back(66, &PLAYER_PTR->data.position, 13);
        }
        return 0;
    }
    // Method 2C
    // 0x410A90
    dllexport gnu_noinline virtual int thiscall __on_tick_2() {
        this->__dec_timer_34_during_gameplay();
        if (this->__timer_34 < this->recharge_time - 8) {
            this->__unknown_flag_A = false;
        }
        this->effect_vm_id.set_controller_position(&PLAYER_PTR->data.position);
        // TODO
        return 0;
    }
    // Method 34
    // 0x410A50
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__unknown_flag_B = false;
    }
    // Method 4C
    // 0x410A60
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__timer_34.reset();
    }
};

// size: 0x5C
struct CardMomoyo : CardBase {
    // CardBase base; // 0x0
    int32_t __int_54; // 0x54
    AnmID __vm_id_58; // 0x58
    // 0x5C

    static inline constexpr CardId ID = MUKADE_CARD; // 54

    inline CardMomoyo() {
        this->__int_54 = 0;
        UNKNOWN_COUNTER_A = 0;
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }

    // Method 28
    // 0x411010
    dllexport gnu_noinline virtual int thiscall __on_bullet_init(int) {
        // TODO
    }
    // Method 2C
    // 0x411000
    dllexport gnu_noinline virtual int thiscall __on_tick_2() { // pointless override
        return 0;
    }
    // Method 34
    // 0x410FE0
    dllexport gnu_noinline virtual void thiscall __on_load_2() {
        this->__method_4C();
    }
    // Method 38
    // 0x411350
    dllexport gnu_noinline virtual int thiscall __set_replay_state(int32_t value) {
        this->__int_54 = value;
        return 0;
    }
    // Method 3C
    // 0x411360
    dllexport gnu_noinline virtual int32_t thiscall __get_replay_state() {
        return this->__int_54;
    }
    // Method 44
    // 0x413050
    dllexport gnu_noinline virtual int thiscall on_anm_id_assigned_to_hud(uint32_t id) {
        this->__vm_id_58 = id;
        return 0;
    }
    // Method 48
    // 0x4110A0
    dllexport gnu_noinline virtual int thiscall on_draw() {
        // TODO
        return 0;
    }
    // Method 4C
    // 0x410FF0
    dllexport gnu_noinline virtual void thiscall __method_4C() {
        this->__unknown_flag_B = false;
        this->__int_54 = 0;
    }
};

struct CardNull : CardBase {
    static inline constexpr CardId ID = NULL_CARD; // 56

    inline CardNull() {
        this->__unknown_flag_B = false;
        this->__unknown_flag_C = true;
        this->__is_active_card = false;
        this->__is_equipment_card = false;
    }
};

// size: 0x1C0
struct CardText {
    char lines[7][64]; // 0x0
    // 0x1C0
};

// size: 0x63E0
struct AbilityTextData {
    CardText description_text[CARD_COUNT]; // 0x0
    AnmID __vm_id_array_63C0[7]; // 0x63C0
    AnmID __vm_id_63DC; // 0x63DC
    // 0x63E0

    inline void zero_contents() {
        zero_this();
    }

    // 0x4132B0
    dllexport static void delete_vms() asm_symbol_rel(0x4132B0) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        AbilityTextData* ability_text_data = ABILITY_TEXT_DATA_PTR;
        size_t i = countof(ability_text_data->__vm_id_array_63C0);
        AnmID* current_vm_id = ability_text_data->__vm_id_array_63C0;
        do {
            current_vm_id++->mark_tree_for_delete(anm_manager);
        } while (--i);
        ability_text_data->__vm_id_63DC.mark_tree_for_delete(anm_manager);
    }

    // 0x416940
    dllexport void thiscall __sub_416940(int arg1, int arg2) asm_symbol_rel(0x416940) {

    }

    // 0x416C10
    dllexport static void stdcall __sub_416C10(int arg1) asm_symbol_rel(0x416C10) {
        AnmManager* anm_manager = ANM_MANAGER_PTR;
        AbilityTextData* ability_text_data = ABILITY_TEXT_DATA_PTR;
        size_t i = countof(ability_text_data->__vm_id_array_63C0);
        AnmID* current_vm_id = ability_text_data->__vm_id_array_63C0;
        do {
            current_vm_id++->__unknown_tree_set_J(anm_manager);
        } while (--i);
        ability_text_data->__sub_416940(arg1, 0);
    }

    static forceinline char* ability_txt_skip_to_end_of_line(const char* str, int32_t& count) {
        char c = *str;
        while (
            c != '\n' && c != '\r' && c != '\f'
        ) {
            c = *str++;
            --count;
        }
        return (char*)str;
    }
    static forceinline char* ability_txt_skip_end_of_line_chars(const char* str, int32_t& count) {
        char c;
        while (
            ((c = *str) == '\n' || c == '\r' || c == '\f') &&
            count != 0
        ) {
            ++str;
            --count;
        }
        return (char*)str;
    }

    // 0x413580
    dllexport static gnu_noinline char* fastcall ability_txt_skip_to_next_line(const char* str, int32_t& count) asm_symbol_rel(0x413580) {
        str = ability_txt_skip_to_end_of_line(str, count);
        if (count != 0) {
            str = ability_txt_skip_end_of_line_chars(str, count);
        }
        return (char*)str;
    }

    // 0x4135D0
    dllexport static gnu_noinline char* fastcall ability_txt_copy_rest_of_line_to_buffer(char* out_str, char* str, int32_t& count) asm_symbol_rel(0x4135D0) {
        out_str[0] = '\0';
        char* end_of_line = str;
        if (*str != '\n') {
            end_of_line = ability_txt_skip_to_end_of_line(str, count);
        }
        if (count != 0) {
            *end_of_line = '\0';
            byteloop_strcpy(out_str, str);
            end_of_line = ability_txt_skip_end_of_line_chars(++end_of_line, count);
        }
        return end_of_line;
    }

    // 0x4160B0
    dllexport gnu_noinline void thiscall initialize() asm_symbol_rel(0x4160B0) {
        char buffer[0x100];
        int32_t ability_txt_count;
        void* ability_txt_file = read_file_to_buffer("ability.txt", &ability_txt_count, false);
        char* ability_txt = (char*)ability_txt_file;
        while (ability_txt_count > 0) {
            switch (char c = *ability_txt++) {
                case '\\':
                    goto break_all;
                case '@': stupid_label: {
                    --ability_txt_count;
                    ability_txt = ability_txt_copy_rest_of_line_to_buffer(buffer, ability_txt, ability_txt_count);
                    auto& matched_card = find_in_card_data([=](const CardData& card) {
                        return !strcmp_asm(card.name, buffer);
                    });
                    const CardId& matched_id = matched_card.id;
                    if (ability_txt_count > 0) {
                        for (int32_t i = 0; i < countof(CardText::lines); ++i) {
                            while ((c = *ability_txt) == '#') {
                                clang_forceinline ability_txt = ability_txt_skip_to_next_line(ability_txt, ability_txt_count);
                                if (ability_txt_count <= 0) {
                                    goto break_all;
                                }
                            }
                            if (c == '@') {
                                goto stupid_label;
                            }
                            ability_txt = ability_txt_copy_rest_of_line_to_buffer(buffer, ability_txt, ability_txt_count);
                            byteloop_strcpy(this->description_text[matched_id].lines[i], buffer);
                            if (ability_txt_count <= 0) {
                                goto break_all;
                            }
                        }
                    }
                    break;
                }
                case '#': default:
                    ability_txt = ability_txt_skip_to_next_line(ability_txt, ability_txt_count);
                    break;
            }
        }
    break_all:
        SAFE_FREE(ability_txt_file);
    }

    inline AbilityTextData() {
        this->zero_contents();
        this->initialize();
    }
};
#pragma region // AbilityTextData Validation
ValidateFieldOffset32(0x0, AbilityTextData, description_text);
ValidateFieldOffset32(0x63C0, AbilityTextData, __vm_id_array_63C0);
ValidateFieldOffset32(0x63DC, AbilityTextData, __vm_id_63DC);
ValidateStructSize32(0x63E0, AbilityTextData);
#pragma endregion

// 0xD70
struct AbilityManager : ZUNTask {
    // ZUNTask base; // 0x0
    AnmLoaded* ability_anm; // 0xC
    AnmLoaded* abcard_anm; // 0x10
    AnmLoaded* abmenu_anm; // 0x14
    ZUNListHead<CardBase> card_list; // 0x18
    int32_t card_count; // 0x28
    int32_t active_card_count; // 0x2C
    int32_t equipment_card_count; // 0x30
    int32_t passive_card_count; // 0x34
    CardBase* selected_active_card; // 0x38
    AnmID __anm_id_3C; // 0x3C
    unknown_fields(0xC); // 0x40
    AnmID __anm_id_4C; // 0x4C
    unknown_fields(0x4); // 0x50
    union {
        uint32_t flags; // 0x54
        struct {
            uint32_t __unknown_flag_A : 1; // 1
        };
    };
    AnmID __anm_id_array_58[0x100]; // 0x58
    AnmID __anm_id_array_458[0x100]; // 0x458
    CardBase* __card_array_858[0x100]; // 0x858
    float __float_C58; // 0xC58
    int32_t __int_C5C; // 0xC5C
    BOOL __ability_data_loaded; // 0xC60
    BOOL __created_ability_data; // 0xC64
    ZUNThread __thread_C68; // 0xC68
    int __int_array_C84[CARD_COUNT - 1]; // 0xC84
    unknown_fields(0xC); // 0xD64
    // 0xD70

    inline void zero_contents() {
        zero_this();
    }

    inline AbilityManager() : __unknown_flag_A(false) {
        this->zero_contents();
        this->__unknown_task_flag_A = true;
    }

    inline void load_files() {
        if (
            (this->ability_anm = AnmManager::preload_anm(ABILITY_ANM_INDEX, "ability.anm")) &&
            (this->abcard_anm = AnmManager::preload_anm(ABCARD_ANM_INDEX, "abcard.anm")) &&
            (this->abmenu_anm = AnmManager::preload_anm(ABMENU_ANM_INDEX, "abmenu.anm"))
        ) {
            this->__created_ability_data = false;
            if (!ABILITY_TEXT_DATA_PTR) {
                this->__created_ability_data = true;
                ABILITY_TEXT_DATA_PTR = new AbilityTextData();
            }
            this->__ability_data_loaded = true;
        }
        LOG_BUFFER.write(JpEnStr("", "data is corrupted\r\n"));
    }

    inline void wait_for_ability_data_to_load() {
        while (!this->__ability_data_loaded) {
            Sleep(1);
        }
    }

    // 0x408DE0
    dllexport gnu_noinline void thiscall create_card_list_for_hud(Float3* position_ptr, int32_t count, int card_type, BOOL arg4) {
        Float3 position = *position_ptr;
        ZUNList<CardBase>* node = this->card_list.next;
        float offset_per_card = 80.0f; // LOCAL 1
        if (count) {
            float count_f = count;
            float offset = count_f * offset_per_card; // EBP+C
            if (offset > 304.0f) {
                offset = 304.0f;
                offset_per_card = 304.0f / count_f;
            }
            offset *= 0.5f;
            offset += position.x;
            position.x = offset;
            if (node) {
                size_t i = 0;
                size_t j = 0;
                node->for_each([&](CardBase* card) {
                    const CardData* data = card->data;
                    switch (card_type) {
                        case 0:
                            if (!card->__is_active_card) {
                                goto skip_card;
                            }
                            break;
                        case 1:
                            if (!card->__is_equipment_card) {
                                goto skip_card;
                            }
                            break;
                        default:
                            if (
                                (card->__is_active_card | card->__is_equipment_card) ||
                                !data->__render_passive_in_hud
                            ) {
                                goto skip_card;
                            }
                            break;
                    }
                    AnmID anm_id = this->abcard_anm->instantiate_vm_to_ui_list_back(5, &position);
                    this->__anm_id_array_58[i] = anm_id;
                    anm_id.run_vm();

                    this->__anm_id_array_58[i].__wtf_child_list_jank_A(6, 0)->set_sprite(data->sprite_small);
                    this->__anm_id_array_58[i].__wtf_child_list_jank_A(7, 0)->set_sprite(data->sprite_small);

                    card->on_anm_id_assigned_to_hud(this->__anm_id_array_58[i]);

                    if (arg4) {
                        this->__anm_id_array_58[i].interrupt_tree(4);
                    }

                    if (card_type == 0) {
                        this->__anm_id_array_458[j] = this->__anm_id_array_58[i];
                        this->__card_array_858[j] = card;
                        ++j;
                    }

                    offset -= offset_per_card;
                    position.x = offset;

                skip_card:
                    ++i;
                });
            }
        }
    }

    // 0x408D00
    dllexport gnu_noinline void thiscall create_all_card_lists_for_hud(BOOL arg1) {
        for (size_t i = 0; i < countof(this->__anm_id_array_58); ++i) {
            this->__anm_id_array_58[i].mark_tree_for_delete();
        }
        if (GAME_THREAD_PTR) {
            Float3 position = {};
            this->create_card_list_for_hud(&position, this->active_card_count, 0, arg1);
            position.y = 130.0f;
            this->create_card_list_for_hud(&position, this->equipment_card_count, 1, arg1);
            position.z = 260.0f;
            this->create_card_list_for_hud(&position, this->passive_card_count, 2, arg1);
        }
    }

    inline ZUNResult initialize_new_card(CardBase* card, int32_t id, uint32_t flags) {
        card->id = id;
        card->__array_index = this->card_count + 1;
        BOOL flagF = flags & 1;
        card->__unknown_flag_F = flagF;
        if (!flagF) {
            if (ZUN_FAILED(card->initializeA())) {
                return ZUN_ERROR;
            }
        }
        if (flags & 2) {
            if (ZUN_FAILED(card->initializeB())) {
                return ZUN_ERROR;
            }
        }
        this->__int_array_C84[id] = 1;
        this->card_list.append_to_list_end(&card->list_node);
        if (card->__is_active_card) {
            this->selected_active_card = card;
        }
        if (card->__is_active_card) {
            ++this->active_card_count;
        } else if (card->__is_equipment_card) {
            ++this->equipment_card_count;
        } else {
            ++this->passive_card_count;
        }
        card->data = &find_id_in_card_data(card->id);

        if (flags == 1) {
            this->create_all_card_lists_for_hud(false);
        }

        ++this->card_count;

        return ZUN_SUCCESS;
    }

    // 0x411460
    dllexport gnu_noinline int32_t thiscall allocate_new_card(int32_t id, uint32_t flags) {
        if (this->card_count < 0x100) {
            CardBase* card;
            switch (id) {
                case NULL_CARD: // 56
                    card = new CardNull();
                    break;
                // TODO
                default:
                    return -1;
            }
            if (card) {
                if (ZUN_FAILED(this->initialize_new_card(card, id, flags))) {
                    delete card;
                }
                return this->card_count;
            }
        }
        return -1;
    }

    // 0x407DA0
    dllexport gnu_noinline void thiscall __sub_407DA0(BOOL arg1) {
        this->card_list.delete_each_data();
        this->card_list.initialize_with((CardBase*)this);
        this->card_count = 0;
        this->active_card_count = 0;
        this->equipment_card_count = 0;
        this->passive_card_count = 0;
        this->__float_C58 = 1.0f;
        this->selected_active_card = NULL;
        ANM_MANAGER_PTR->__set_state1_for_all_vms_from_loaded_slot(30);
        this->__anm_id_3C.mark_tree_for_delete();
        ANM_MANAGER_PTR->__set_state1_for_all_vms_from_loaded_slot(31);

        for (size_t i = 0; i < countof(this->__anm_id_array_58); ++i) {
            this->__anm_id_array_58[i].mark_tree_for_delete();
        }

        if (arg1) {
            // Do these REP STOS memsets correspond to std::array = {}?
            memset(this->__int_array_C84, 0, sizeof(this->__int_array_C84));

            ScorefileManager* scorefile_manager = SCOREFILE_MANAGER_PTR;
            for (
                int32_t i = 0;
                i < scorefile_manager->primary_file.__sectionA.__int_array_1C0[this->__int_C5C];
                ++i, scorefile_manager = SCOREFILE_MANAGER_PTR
            ) {
                int32_t card_id = scorefile_manager->primary_file.__sectionA.__card_ids_150[this->__int_C5C][i];
                this->allocate_new_card(card_id, 1);
            }
        }
    }

    // 0x408640
    dllexport gnu_noinline UpdateFuncRet thiscall on_tick() asm_symbol_rel(0x408640) {

    }

    // 0x408A90
    dllexport static gnu_noinline UpdateFuncRet fastcall pre_on_tick(void* self) asm_symbol_rel(0x408A90) {
        if (ABILITY_SHOP_PTR) {
            return UpdateFuncNext;
        }
        return ((AbilityManager*)self)->on_tick();
    }

    // 0x408AB0
    dllexport static gnu_noinline UpdateFuncRet fastcall on_draw(void* self) asm_symbol_rel(0x408AB0) {
        ((AbilityManager*)self)->card_list.for_each([](CardBase* card) static_lambda {

        });
        return UpdateFuncNext;
    }

    // 0x4082B0
    dllexport static gnu_noinline AbilityManager* create() asm_symbol_rel(0x4082B0) {
        AbilityManager* ability_manager = new AbilityManager();
        ABILITY_MANAGER_PTR = ability_manager;
        ability_manager->card_list.initialize_with((CardBase*)ability_manager);
        ability_manager->load_files();
        UpdateFunc* on_tick = new UpdateFunc(&AbilityManager::pre_on_tick, false, ability_manager);
        UpdateFuncRegistry::register_on_tick(on_tick, 22);
        ability_manager->on_tick_func = on_tick;
        UpdateFunc* on_draw = new UpdateFunc(&AbilityManager::on_draw, false, ability_manager);
        UpdateFuncRegistry::register_on_draw(on_draw, 51);
        ability_manager->on_draw_func = on_draw;
        return ability_manager;
    }

    // 0x408BA0
    dllexport static gnu_noinline int __sub_408BA0(int32_t* ptr) asm_symbol_rel(0x408BA0) {

    }

#if INCLUDE_PATCH_CODE
    size_t vectorcall dont_worry_bravi_it_only_took_me_a_year(const char *restrict count_name) {
        const CardData *restrict card_data_ptr = CARD_DATA_TABLE;
        do {
            if (byteloop_strmatch(card_data_ptr->name, count_name)) {
                CardId count_id = card_data_ptr->id;
                return this->card_list.count_if([=](CardBase* card) {
                    return card->id == count_id;
                });
            }
        } while (++card_data_ptr != array_end_addr(CARD_DATA_TABLE));
        return 0;
    }
#endif

    // 0x412FE0
    dllexport gnu_noinline static BOOL stdcall card_equipped(int32_t id) {
        return (bool)ABILITY_MANAGER_PTR->card_list.find_if([=](CardBase* card) {
            return card->id == id;
        });
    }

private:
    inline int32_t equipped_cards_get_ids_impl(int32_t* card_ids_out) {
        memset(card_ids_out, 0, sizeof(int32_t[256]));
        int32_t card_count = 0;
        this->card_list.do_while([&](CardBase* card) {
            ++card_count;
            *card_ids_out++ = card->id;
            return card_count < 255;
        });
        *card_ids_out = -1;
        CardBase* active_card = this->selected_active_card;
        if (!active_card) {
            return -1;
        }
        return active_card->id;
    }

    inline int __equipped_cards_get_replay_states_impl(int32_t* times_out) {
        this->card_list.for_each([&](CardBase* card) {
            *times_out++ = card->__get_replay_state();
        });
        *times_out = -1;
        return 0;
    }

    inline int equipped_cards_run_on_load_impl() {
        int ret = 0;
        this->card_list.for_each([&](CardBase* card) {
            ret |= card->on_load();
        });
        return ret;
    }

    // 0x418C70
    dllexport gnu_noinline static int32_t stdcall equipped_cards_get_ids(int32_t* card_ids_out, int) {
        return ABILITY_MANAGER_PTR->equipped_cards_get_ids_impl(card_ids_out);
    }
public:

    // Wrapper to get rid of dummy arg when calling
    inline int32_t equipped_cards_get_ids(int32_t* card_ids_out) {
        return equipped_cards_get_ids(card_ids_out, UNUSED_DWORD);
    }

    // 0x408BA0
    dllexport gnu_noinline static int stdcall __equipped_cards_get_replay_states(int32_t* times_out) {
        return ABILITY_MANAGER_PTR->__equipped_cards_get_replay_states_impl(times_out);
    }

    // 0x408AD0
    dllexport gnu_noinline static int equipped_cards_run_on_load() {
        return ABILITY_MANAGER_PTR->equipped_cards_run_on_load_impl();
    }
};
#pragma region // AbilityManager Validation
ValidateFieldOffset32(0x0, AbilityManager, task_flags);
ValidateFieldOffset32(0x4, AbilityManager, on_tick_func);
ValidateFieldOffset32(0x8, AbilityManager, on_draw_func);
ValidateFieldOffset32(0xC, AbilityManager, ability_anm);
ValidateFieldOffset32(0x10, AbilityManager, abcard_anm);
ValidateFieldOffset32(0x14, AbilityManager, abmenu_anm);
ValidateFieldOffset32(0x18, AbilityManager, card_list);
ValidateFieldOffset32(0x28, AbilityManager, card_count);
ValidateFieldOffset32(0x2C, AbilityManager, active_card_count);
ValidateFieldOffset32(0x30, AbilityManager, equipment_card_count);
ValidateFieldOffset32(0x34, AbilityManager, passive_card_count);
ValidateFieldOffset32(0x38, AbilityManager, selected_active_card);
ValidateFieldOffset32(0x3C, AbilityManager, __anm_id_3C);
ValidateFieldOffset32(0x4C, AbilityManager, __anm_id_4C);
ValidateFieldOffset32(0x54, AbilityManager, flags);
ValidateFieldOffset32(0x58, AbilityManager, __anm_id_array_58);
ValidateFieldOffset32(0x458, AbilityManager, __anm_id_array_458);
ValidateFieldOffset32(0x858, AbilityManager, __card_array_858);
ValidateFieldOffset32(0xC58, AbilityManager, __float_C58);
ValidateFieldOffset32(0xC5C, AbilityManager, __int_C5C);
ValidateFieldOffset32(0xC60, AbilityManager, __ability_data_loaded);
ValidateFieldOffset32(0xC64, AbilityManager, __created_ability_data);
ValidateFieldOffset32(0xC68, AbilityManager, __thread_C68);
ValidateFieldOffset32(0xC84, AbilityManager, __int_array_C84);
ValidateStructSize32(0xD70, AbilityManager);
#pragma endregion

static inline float ability_manager_get_float_C58() {
    return ABILITY_MANAGER_PTR->__float_C58;
}
static inline AnmLoaded* ability_manager_get_ability_anm() {
    return ABILITY_MANAGER_PTR->ability_anm;
}

// size: 0xE3C
struct AbilityShop : ZUNTask {
    // ZUNTask base; // 0x0
    MenuSelect __menu_select_C; // 0xC
    // 0xE4

    // 0x417CC0
    dllexport gnu_noinline UpdateFuncRet thiscall on_tick() asm_symbol_rel(0x417CC0) {

    }

    // 0x418C20
    dllexport gnu_noinline static UpdateFuncRet fastcall on_tick__jump(void* ptr) {
        return ((AbilityShop*)ptr)->on_tick();
    }

    // 0x418990
    dllexport gnu_noinline UpdateFuncRet thiscall on_draw() asm_symbol_rel(0x418990) {

    }

    // 0x418C30
    dllexport gnu_noinline static UpdateFuncRet fastcall on_draw__jump(void* ptr) {
        return ((AbilityShop*)ptr)->on_draw();
    }

    // 0x4171B0
    dllexport gnu_noinline ZUNResult thiscall initialize(Float3* arg1) asm_symbol_rel(0x4171B0) {

    }
};
#pragma region // AbilityShop Validation

#pragma endregion

#if INCLUDE_PATCH_CODE
dllexport gnu_noinline void count_cards_of_type(EnemyData* self) {
    int32_t* out;
    clang_forceinline out = self->get_int_ptr_arg();
    EclInstruction* current_instruction;
    __asm__(
        "movl -0x57C(%%ebp), %[current_instruction]"
        : asm_arg("=r", current_instruction)
    );
    *out = ABILITY_MANAGER_PTR->dont_worry_bravi_it_only_took_me_a_year(StringArg(4));
}

dllexport gnu_noinline void count_cards_of_type2(EnemyData* self) {
    EclInstruction* current_instruction;
    __asm__(
        "movl -0x57C(%%ebp), %[current_instruction]"
        : asm_arg("=r", current_instruction)
    );
    const char* count_name = StringArg(4);
    
    int32_t count = 0;
    for (
        const CardData *restrict card_data_ptr = CARD_DATA_TABLE;
        card_data_ptr != array_end_addr(CARD_DATA_TABLE);
        ++card_data_ptr
    ) {
        if (byteloop_strmatch(card_data_ptr->name, count_name)) {
            CardId count_id = card_data_ptr->id;
            count = ABILITY_MANAGER_PTR->card_list.count_if([=](CardBase* card) {
                return card->id == count_id;
            });
            break;
        }
    }
    clang_forceinline *self->get_int_ptr_arg() = count;
}
#endif

dllexport gnu_noinline void stdcall log_vprintf(const char* format, va_list va) {
    vprintf(format, va);
}

dllexport gnu_noinline void fastcall thcrap_print(EclContext* self_in, int, const EclInstruction* current_instruction) {
    EclContext* self;
    __asm__(
        ""
        : "=D"(self)
        : "0"(self_in)
    );
    
    const EclValue* call_arg_formats = based_pointer<EclValue>(StringArg(4), IntArg(0));
    size_t param_count = current_instruction->param_count;
    //uint8_t* print_args = (uint8_t*)__builtin_alloca(sizeof(double[param_count - 1]));
    //uint8_t* print_args_write = print_args;
    double print_args_mem[bitsof(current_instruction->param_mask)];
    uint8_t* print_args = (uint8_t*)print_args_mem;
    for (size_t i = 1; i < param_count; ++i) {
        const EclValue& call_arg = call_arg_formats[i - 1];
        switch (call_arg.type) {
            case 'f': case 'g':
                *(double*)print_args = (long double)self->parse_float_as_arg_pop(i, call_arg.value.real, (EclInstruction*)current_instruction);
                print_args += sizeof(double);
                //*(double*)print_args_write = (long double)context->parse_float_as_arg_pop(i, call_arg.value.real, (EclInstruction*)current_instruction);
                //print_args_write += sizeof(double);
                break;
            default:
                *(int32_t*)print_args = self->parse_int_as_arg_pop(i, call_arg.value.integer, (EclInstruction*)current_instruction);
                print_args += sizeof(int32_t);
                //*(int32_t*)print_args_write = context->parse_int_as_arg_pop(i, call_arg.value.integer, (EclInstruction*)current_instruction);
                //print_args_write += sizeof(int32_t);
                break;
        }
    }
    log_vprintf(StringArg(4), (va_list)print_args_mem);
    //free(print_args);
}

// 0x425B40
dllexport gnu_noinline float vectorcall Float3::__bullet_effect_angle_jank(float angle, float arg2, float arg3) {
    if (!(arg2 <= -999990.0f)) {
        if (arg2 >= 999990.0f && arg2 < 1999990.0f) {
            return arg3 + Player::angle_to_player_from_point(this);
        }
        if (arg2 >= 2999990.0f && arg2 < 3999990.0f) {
            return REPLAY_RNG.rand_float_signed_range(arg3) + Player::angle_to_player_from_point(this);
        }
        if (arg2 >= 3999990.0f && arg2 < 4999990.0f) {
            return REPLAY_RNG.rand_float_signed_range(arg3) + angle;
        }
        if (arg2 >= 4999990.0f) {
            return get_boss_by_index(0)->data.current_motion.position.angle_to(this);
        }
        return arg2;
    }
    return angle;
}

// I completely forget if this was going to be EnemyInitData or something else
struct EnemyAllocationData {
    
};

typedef int32_t(fastcall UnknownSFunc)(int, int);

// size: 0x20
struct UnknownS {
    int16_t __sshort_0; // 0x0
    int16_t __sshort_2; // 0x2
    UnknownSFunc* __func_4; // 0x4
    uint32_t on_tick_index; // 0x8
    uint32_t on_draw_index; // 0xC
    uint32_t on_destroy_index; // 0x10
    uint32_t on_interrupt_index; // 0x14
    uint32_t on_copy_A_index; // 0x18
    uint32_t on_copy_B_index; // 0x1C
    // 0x20
};

extern "C" {
    // 0x5217D0
    extern Float3 UNKNOWN_FLOAT3_A asm("_UNKNOWN_FLOAT3_A");
    // 0x5217DC
    extern Float3 UNKNOWN_FLOAT3_B asm("_UNKNOWN_FLOAT3_B");
    // 0x56AD78
    extern Float2 UNKNOWN_FLOAT2_A asm("_UNKNOWN_FLOAT2_A");
}

// size: 0x1A0
struct EnemyManager : ZUNTask {
    //ZUNTask base; // 0x0
    int32_t int_vars[4]; // 0xC
    float float_vars[8]; // 0x1C
    int32_t player_death_count; // 0x3C
    int32_t player_bomb_count; // 0x40
    int32_t can_capture_spell; // 0x44
    int32_t boss_ids[4]; // 0x48
    unknown_fields(0x30); // 0x58
    union {
        uint32_t flags; // 0x88
        struct {
            uint32_t __unknown_flag_A : 1; // 1
        };
    };
    int32_t enemy_limit; // 0x8C
    int32_t next_enemy_id; // 0x90
    int32_t prev_enemy_id; // 0x94
    Timer __timer_98; // 0x98
    int32_t __int_AC; // 0xAC
    int32_t __int_B0; // 0xB0
    unknown_fields(0xB0); // 0xB4
    int __int_164; // 0x164
    AnmLoaded* enemy_anms[8]; // 0x168
    EnemyController* enemy_controller; // 0x188
    ZUNListEnds<Enemy> enemy_list; // 0x18C
    ZUNList<Enemy>* __unknown_enemy_list; // 0x194
    int32_t enemy_count; // 0x198
    ZUNAngle __angle_19C; // 0x19C
    // 0x1A0

    inline EnemyManager() {
        EnemyController* enemy_controller = new EnemyController();
        this->enemy_controller = enemy_controller;

    }

    forceinline bool is_below_enemy_limit() {
        return this->enemy_count < this->enemy_limit;
    }

    // 0x42D7D0
    dllexport void allocate_new_enemy(const char* sub_name, EnemyInitData* data, int32_t = UNUSED_DWORD) asm_symbol_rel(0x42D7D0) {
        Enemy* enemy = new Enemy(sub_name);
        enemy->data.motion.absolute.position = data->position;
        enemy->data.score = data->score;
        enemy->data.life.current = data->life;
        enemy->data.life.maximum = data->life;
        enemy->data.life.remaining_current_attack = data->life;
        enemy->data.drops.main_id = data->item_drop;
        enemy->data.mirrored = data->mirrored;
        enemy->context.difficulty_mask = 1 << GAME_MANAGER.globals.difficulty;
        __builtin_memcpy(enemy->data.int_vars, data->int_vars, sizeof(enemy->data.int_vars));
        __builtin_memcpy(enemy->data.float_vars, data->float_vars, sizeof(enemy->data.float_vars));
        enemy->data.invulnerable_timer.set(1);
        enemy->data.__basic_anm_update = data->__basic_anm_update;
        enemy->data.kill_id = 0;
        enemy->parent_id = data->parent_id;
        if (data->life >= 1000) {
            enemy->data.__unknown_flag_O = true;
        }
        enemy->data.chapter = GAME_MANAGER.globals.chapter;

    }

    // 0x42D440
    dllexport static gnu_noinline int32_t count_killable_enemies() asm_symbol_rel(0x42D440) {
        ENEMY_MANAGER_PTR->enemy_list.count_if_not([](Enemy* enemy) {
            return !enemy->data.has_active_hitbox() || enemy->data.is_invulnerable();
            //return enemy->data.disable_hitbox || enemy->data.invincible || enemy->data.intangible || enemy->data.invulnerable_timer > 0;
        });
    }

    // 0x42D490
    dllexport gnu_noinline void set_boss(int32_t index, Enemy* enemy) asm_symbol_rel(0x42D490) {
        EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
        if (enemy) {
            enemy_manager->boss_ids[index] = enemy->id;
        } else {
            enemy_manager->boss_ids[index] = 0;
        }
    }

    // 0x42D4D0
    dllexport gnu_noinline void __set_unkown_flag_A(bool value) asm_symbol_rel(0x42D4D0) {
        ENEMY_MANAGER_PTR->__unknown_flag_A = value;
    }

    forceinline Enemy* get_enemy_by_id_impl(int32_t enemy_id) {
        if (!enemy_id) {
            return NULL;
        } else {
            return this->enemy_list.find_if([=](Enemy* enemy) {
                return enemy->id == enemy_id;
            });
        }
    }

    // 0x42D500
    dllexport gnu_noinline Enemy* get_enemy_by_id(int32_t enemy_id) asm_symbol_rel(0x42D500) {
        return ENEMY_MANAGER_PTR->get_enemy_by_id_impl(enemy_id);
    }

    // 0x4237F0
    dllexport gnu_noinline Enemy* get_boss_by_index(int32_t index) asm_symbol_rel(0x4237F0) {
        EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
        return enemy_manager->get_enemy_by_id_impl(enemy_manager->boss_ids[index]);
    }

    // 0x409990
    dllexport gnu_noinline BOOL enemy_exists_with_id(int32_t enemy_id) asm_symbol_rel(0x409990) {
        if (enemy_id) {
            return (bool)ENEMY_MANAGER_PTR->enemy_list.find_if([=](Enemy* enemy) {
                return enemy->id == enemy_id;
            });
        }
        return false;
    }

    // 0x42D540
    dllexport gnu_noinline void set_enemy_limit(int32_t limit) asm_symbol_rel(0x42D540) {
        ENEMY_MANAGER_PTR->enemy_limit = limit;
    }

    inline AnmLoaded* anm_file_lookup(int32_t file_index) asm_symbol_rel(0x438D40);

private:
    inline EnemyID& __get_id_of_nearest_enemy_in_radius(EnemyID& out, Float2* position, float radius) {
        Enemy* found = NULL;
        float radius_squared = radius * radius;
        this->enemy_list.for_each_safe([&](Enemy* enemy) {
            if (!(
                enemy->data.disable_hitbox | enemy->data.intangible |
                enemy->data.__basic_anm_update | enemy->data.homing_disable
            )) {
                float distance_squared = position->distance_squared(&enemy->data.current_motion.position);
                if (distance_squared < radius_squared) {
                    found = enemy;
                    radius_squared = distance_squared;
                }
            }
        });
        if (found) {
            out = found->id;
            return out;
        }
        out = 0;
        return out;
    }
    // 0x438CB0
    dllexport gnu_noinline static EnemyID& vectorcall __get_id_of_nearest_enemy_in_radius(int, int, EnemyID& out, Float2* position, float, float, float, float radius) {
        return ENEMY_MANAGER_PTR->__get_id_of_nearest_enemy_in_radius(out, position, radius);
    }
public:
    inline static EnemyID __get_id_of_nearest_enemy_in_radius(Float2* position, float radius) {
        EnemyID temp;
        return __get_id_of_nearest_enemy_in_radius(UNUSED_DWORD, UNUSED_DWORD, temp, position, UNUSED_FLOAT, UNUSED_FLOAT, UNUSED_FLOAT, radius);
    }

    // 0x430710
    dllexport gnu_noinline static void kill_all() {
        EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
        enemy_manager->enemy_list.for_each_safe([=](Enemy* enemy) {
            enemy->try_kill();
        });
        enemy_manager->__timer_98++;
    }

    // 0x430B20
    dllexport gnu_noinline static void kill_all_no_callbacks() {
        EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
        enemy_manager->enemy_list.for_each_safe([=](Enemy* enemy) {
            enemy->try_kill<true>();
        });
        enemy_manager->__timer_98++;
    }

    // 0x430910
    dllexport gnu_noinline static void stdcall kill_all_by_kill_id(int32_t kill_id) {
        EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
        enemy_manager->enemy_list.for_each_safe([=](Enemy* enemy) {
            enemy->try_kill_by_kill_id(kill_id);
        });
        enemy_manager->__timer_98++;
    }
};
#pragma region // EnemyManager Validation
ValidateFieldOffset32(0x0, EnemyManager, task_flags);
ValidateFieldOffset32(0x4, EnemyManager, on_tick_func);
ValidateFieldOffset32(0x8, EnemyManager, on_draw_func);
ValidateFieldOffset32(0xC, EnemyManager, int_vars);
ValidateFieldOffset32(0x1C, EnemyManager, float_vars);
ValidateFieldOffset32(0x3C, EnemyManager, player_death_count);
ValidateFieldOffset32(0x40, EnemyManager, player_bomb_count);
ValidateFieldOffset32(0x44, EnemyManager, can_capture_spell);
ValidateFieldOffset32(0x48, EnemyManager, boss_ids);
ValidateFieldOffset32(0x88, EnemyManager, flags);
ValidateFieldOffset32(0x8C, EnemyManager, enemy_limit);
ValidateFieldOffset32(0x90, EnemyManager, next_enemy_id);
ValidateFieldOffset32(0x94, EnemyManager, prev_enemy_id);
ValidateFieldOffset32(0x98, EnemyManager, __timer_98);
ValidateFieldOffset32(0xAC, EnemyManager, __int_AC);
ValidateFieldOffset32(0xB0, EnemyManager, __int_B0);
ValidateFieldOffset32(0x164, EnemyManager, __int_164);
ValidateFieldOffset32(0x168, EnemyManager, enemy_anms);
ValidateFieldOffset32(0x188, EnemyManager, enemy_controller);
ValidateFieldOffset32(0x18C, EnemyManager, enemy_list);
ValidateFieldOffset32(0x194, EnemyManager, __unknown_enemy_list);
ValidateFieldOffset32(0x198, EnemyManager, enemy_count);
ValidateFieldOffset32(0x19C, EnemyManager, __angle_19C);
ValidateStructSize32(0x1A0, EnemyManager);
#pragma endregion

// 0x42E0C0
dllexport gnu_noinline Enemy* thiscall EnemyID::get_enemy_ptr() {
    int32_t raw_id = this->raw;
    if (!raw_id) {
        return NULL;
    }
    return ENEMY_MANAGER_PTR->get_enemy_by_id_impl(raw_id);
}

static inline bool enemies_are_alive() {
    if (EnemyManager* enemy_manager = ENEMY_MANAGER_PTR) {
        return enemy_manager->enemy_count != 0;
    }
    return false;
}

inline void EnemyData::set_anm_vm_slowdowns(const float& slowdown_value) {
    for (size_t i = 0; i < countof(this->anm_vms); ++i) {
        this->anm_vms[i].get_vm_ptr()->set_custom_slowdown(slowdown_value);
    }
}

inline void EnemyData::initialize(Enemy* enemy) {
    this->vm = enemy;
    this->position_interp.absolute.end_time = 0;
    this->position_interp.relative.end_time = 0;
    this->angle_interp_absolute.end_time = 0;
    this->angle_interp_relative.end_time = 0;
    this->speed_interp_absolute.end_time = 0;
    this->speed_interp_relative.end_time = 0;
    this->orbit_radius_interp.absolute.end_time = 0;
    this->orbit_radius_interp.relative.end_time = 0;
    this->ellipse_interp.absolute.end_time = 0;
    this->ellipse_interp.relative.end_time = 0;
    this->hit_sound = -1;
    this->death_anm_script = 0;
    this->current_motion.zero_contents();
    this->motion.absolute.zero_contents();
    this->motion.relative.zero_contents();
    this->hitbox_size = { 24.0f, 24.0f };
    this->collision_size = { 24.0f, 24.0f };
    this->hitbox_rotation = 0.0f;
    this->boss_id = -1;
    this->global_list_node.initialize_with(enemy);
    this->drops.initialize();
    this->ecl_time.initialize_and_reset();
    this->boss_timer.initialize_and_reset();
    this->invulnerable_timer.initialize_and_reset();
    this->no_collision_timer.initialize_and_reset();
    this->anm_base_layer = 1;
}

// 0x42E100
dllexport gnu_noinline Enemy::Enemy(const char* sub_name) {
    this->initialize_vm();
    this->data.initialize(this);
    EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
    this->__on_kill_func = NULL;
    this->data.death_callback_sub[0] = '\0';
    this->id = enemy_manager->next_enemy_id;
    int32_t id = enemy_manager->next_enemy_id;
    enemy_manager->prev_enemy_id = id;
    enemy_manager->next_enemy_id = ++id;
    if (!id) {
        enemy_manager->next_enemy_id = ++id;
    }
    this->data.slowdown = 0.0f;
    this->controller = enemy_manager->enemy_controller;
    this->locate_sub((char*)sub_name);
    this->current_context->location.instruction_offset = 0;
    this->current_context->time = 0.0f;
    this->data.life.initialize();
    this->data.initialize_callbacks();
    this->data.initialize_anm_vm_slots();
    this->data.bomb_damage_multiplier = 1.0f;
    this->data.chapter_spawn_weight = 0;
}

// 0x4237F0
dllexport gnu_noinline Enemy* get_boss_by_index(int32_t boss_index) {
    EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
    int32_t boss_id = enemy_manager->boss_ids[boss_index];
    if (!boss_id) {
        return NULL;
    } else {
        enemy_manager->enemy_list.find_if([=](Enemy* enemy) {
            return enemy->id == boss_id;
        });
    }
}

// 0x42D220
dllexport gnu_noinline Enemy::~Enemy() {
    EnemyManager* enemy_manager = ENEMY_MANAGER_PTR;
    this->data.global_list_node.unlink_from_head(enemy_manager->enemy_list.head);
    this->data.global_list_node.unlink_from_tail(enemy_manager->enemy_list.tail);
    this->data.global_list_node.unlink_from_head(enemy_manager->__unknown_enemy_list);
    this->data.global_list_node.unlink();
    if (this->data.__unknown_flag_P) {
        --enemy_manager->enemy_count;
        // Probably a nested function here...
        if (this->data.__unknown_flag_P) {
            if (this->data.is_boss) {
                enemy_manager->boss_ids[this->data.boss_id] = 0;
            }
            AnmManager* anm_manager = ANM_MANAGER_PTR;
            for (size_t i = 0; i < countof(this->data.anm_vms); ++i) {
                anm_manager->mark_tree_id_for_delete(this->data.anm_vms[i]);
                this->data.anm_vms[i] = 0;
            }
        }
    }
    if (void* fog_ptr = this->data.fog.fog_ptr) {
        // __sub_41BE50(fog_ptr);
    }
    this->data.fog.fog_ptr = NULL;
}

// 0x430240
dllexport gnu_noinline int thiscall Enemy::kill() {

    ENEMY_MANAGER_PTR->__angle_19C = this->data.kill_effects<true>();

    this->data.drops.spawn_items(&this->data.current_motion.position);

    char* death_callback_sub = this->data.death_callback_sub;
    if (death_callback_sub[0]) {
        this->reinitialize_vm_with_sub(death_callback_sub);
        this->run_ecl(0.0f);
        death_callback_sub[0] = '\0';
    }

    if (auto* on_kill_func = this->__on_kill_func) {
        on_kill_func(this);
    }

    return 1;
}

typedef struct Stage Stage;
extern "C" {
    // 0x4CF2B4
    extern Stage* STAGE_PTR asm("_STAGE_PTR");
    // 0x4CF2B8
    extern Stage* STAGE_B_PTR asm("_STAGE_B_PTR");
}

// size: 0x30
struct StdDistortion {
    unknown_fields(0x2C); // 0x0
    int32_t enabled; // 0x2C
    // 0x30
};

// size: 0x90
struct StdHeader {
    int16_t entry_count; // 0x0
    int16_t face_count; // 0x2
    uint32_t faces_offset; // 0x4
    uint32_t script_offset; // 0x8
    unknown_fields(0x4); // 0xC
    char anm_name[128]; // 0x10
    // 0x90
};

// size: 0x1C
struct StdQuad {
    int16_t type; // 0x0
    int16_t offset_to_next; // 0x2
    uint16_t anm_script; // 0x4
    int16_t face_vm_index; // 0x6
    Float3 position; // 0x8
    Float2 size; // 0x14
    // 0x1C
};

// size: 0x1C+
struct StdEntry {
    int16_t id; // 0x0
    int8_t layer; // 0x2
    union {
        uint8_t flags; // 0x3
        struct {

        };
    };
    Float3 position; // 0x4
    Float3 size; // 0x10
    StdQuad quads[0]; // 0x1C
};

// size: 0x10
struct StdFace {
    int16_t entry_id; // 0x0
    union {
        uint16_t flags; // 0x2
        struct {

        };
    };
    Float3 position; // 0x4
    // 0x10
};

// size: 0x8+
struct StdInstruction {
    int32_t time; // 0x0
    int16_t opcode; // 0x4
    int16_t size; // 0x6
    unsigned char args[]; // 0x8
};

// It's been too long since I looked at stage stuff,
// is this actually the whole thing?
// size: 0x3444
struct StdVM {
    Timer script_time; // 0x0
    int32_t current_instruction_offset; // 0x14
    int32_t shaking_mode; // 0x18
    Timer shaking_timer; // 0x1C
    Timer __shaking_6_timer; // 0x30
    ZUNInterp<Float3> camera_facing_interp; // 0x44
    ZUNInterp<Float3> camera_position_interp; // 0x9C
    ZUNInterp<Float3> camera_rotation_interp; // 0xF4
    ZUNInterp<StageSky> sky_data_interp; // 0x14C
    ZUNInterp<float> camera_fov_interp; // 0x1F4
    StageCamera camera; // 0x224
    Stage* full_stage; // 0x388
    AnmVM vms[8]; // 0x38C
    int32_t anm_layers[8]; // 0x33EC
    float draw_distance_squared; // 0x340C
    StdDistortion distortion; // 0x3410
    D3DCOLOR __color_3440; // 0x3440
    // 0x3444
};
#pragma region // StdVM Validation
ValidateFieldOffset32(0x0, StdVM, script_time);
ValidateFieldOffset32(0x14, StdVM, current_instruction_offset);
ValidateFieldOffset32(0x18, StdVM, shaking_mode);
ValidateFieldOffset32(0x1C, StdVM, shaking_timer);
ValidateFieldOffset32(0x30, StdVM, __shaking_6_timer);
ValidateFieldOffset32(0x44, StdVM, camera_facing_interp);
ValidateFieldOffset32(0x9C, StdVM, camera_position_interp);
ValidateFieldOffset32(0xF4, StdVM, camera_rotation_interp);
ValidateFieldOffset32(0x14C, StdVM, sky_data_interp);
ValidateFieldOffset32(0x1F4, StdVM, camera_fov_interp);
ValidateFieldOffset32(0x224, StdVM, camera);
ValidateFieldOffset32(0x388, StdVM, full_stage);
ValidateFieldOffset32(0x38C, StdVM, vms);
ValidateFieldOffset32(0x33EC, StdVM, anm_layers);
ValidateFieldOffset32(0x340C, StdVM, draw_distance_squared);
ValidateFieldOffset32(0x3410, StdVM, distortion);
ValidateFieldOffset32(0x3440, StdVM, __color_3440);
ValidateStructSize32(0x3444, StdVM);
#pragma endregion

// size: 0x3A40
struct Stage : ZUNTask {
    // ZUNTask base; // 0x0
    StdVM std_vm; // 0xC
    AnmVM* face_vms; // 0x3450
    StdHeader* std_file; // 0x3454
    StdEntry* entries; // 0x3458
    StdFace* faces; // 0x345C
    StdInstruction* script; // 0x3460
    AnmLoaded* stage_anm; // 0x3464
    // 0x3464

    union {
        uint32_t flags; // 0x3474
        struct {
            uint32_t : 2; // 1-2
            uint32_t __unknown_flag_B : 1; // 3
            uint32_t __unknown_flag_A : 1; // 4
        };
    };

    inline void zero_contents() {
        zero_this();
    }

    inline Stage() {
        this->zero_contents();
        this->__unknown_task_flag_A = true;
    }

    // 0x41B850
    dllexport gnu_noinline ZUNResult thiscall initialize(const char* std_filename) asm_symbol_rel(0x41B850) {

    }

    // 0x41BF90
    dllexport gnu_noinline static Stage* fastcall allocate(const char* std_filename) asm_symbol_rel(0x41BF90) {
        int A = 0;
        Stage* stage = new Stage();
        A = -1;
        if (ZUN_FAILED(stage->initialize(std_filename))) {
            delete stage;
            return NULL;
        }
        return stage;
    }
};
#pragma region // Stage Validation
ValidateFieldOffset32(0x0, Stage, task_flags);
ValidateFieldOffset32(0x4, Stage, on_tick_func);
ValidateFieldOffset32(0x8, Stage, on_draw_func);
ValidateStructSize32(0x3444, StdVM);
#pragma endregion

typedef struct Spellcard Spellcard;
extern "C" {
    // 0x4CF2C0
    extern Spellcard* SPELLCARD_PTR asm("_SPELLCARD_PTR");
}

// size: 0xC0
struct Spellcard : ZUNTask {
    // ZUNTask base; // 0x0
    AnmID __vm_id_C; // 0xC
    AnmID __vm_id_10; // 0x10
    AnmID __vm_id_14; // 0x14
    AnmID __vm_id_18; // 0x18
    AnmID __vm_id_1C; // 0x1C
    Timer __timer_20; // 0x20
    char name[64]; // 0x34
    int32_t id; // 0x74
    union {
        uint32_t flags; // 0x78
        struct {
            uint32_t __unknown_flag_A : 1; // 1
            uint32_t __unknown_flag_B : 1; // 2
            uint32_t : 1; // 3
            uint32_t __timeout_spell : 1; // 4
            uint32_t __unknown_flag_C : 1; // 5
            uint32_t __unknown_flag_E : 1; // 6
            uint32_t __unknown_flag_F : 1; // 7
            uint32_t __unknown_flag_D : 1; // 8
            uint32_t __unknown_flag_H: 1; // 9
            uint32_t __unknown_flag_G : 1; // 10
        };
    };
    int32_t __bonus_A; // 0x7C
    int32_t __bonus_B; // 0x80
    int32_t time; // 0x84
    unknown_fields(0x4); // 0x88
    int __int_8C; // 0x8C
    unknown_fields(0x1C); // 0x90
    Float3 __float3_AC; // 0xAC
    unknown_fields(0x8); // 0xB8
    // 0xC0

    inline void zero_contents() {
        zero_this();
    }

    inline Spellcard() {
        this->zero_contents();
    }

    // 0x409B10
    dllexport gnu_noinline BOOL thiscall __get_flag_A() asm_symbol_rel(0x409B10) {
        return this->__unknown_flag_A;
    }

    // 0x42D640
    dllexport gnu_noinline BOOL thiscall __get_flag_B() asm_symbol_rel(0x42D640) {
        return this->__unknown_flag_B;
    }

    // 0x42D650
    dllexport gnu_noinline void thiscall __set_timeout_flag() asm_symbol_rel(0x42D650) {
        SPELLCARD_PTR->__timeout_spell = true;
    }

    // 0x42D610
    dllexport gnu_noinline void thiscall __set_flag_H(int32_t state) asm_symbol_rel(0x42D610) {
        SPELLCARD_PTR->__unknown_flag_H = state;
    }

    inline bool __inline_sub_A() {
        return this->__unknown_flag_A && this->__timer_20 >= 60;
    }

    inline bool __inline_sub_B() {
        return this->__unknown_flag_A & this->__timeout_spell;
    }

    inline bool __inline_sub_C() {
        return this->__unknown_flag_A & this->__unknown_flag_E;
    }

    // 0x42D670
    dllexport gnu_noinline void thiscall __sub_42D670() asm_symbol_rel(0x42D670) {
        Spellcard* spellcard = SPELLCARD_PTR;
        spellcard->__unknown_flag_C = true;
        spellcard->__vm_id_1C.mark_tree_for_delete();
    }

    inline void __inline_sub_409AD0() {
        if (this->__unknown_flag_A) {
            if (this->__timer_20 >= 60) {
                this->__bonus_A = 0;
                this->__unknown_flag_B = false;
                this->__unknown_flag_E = false;
            } else if (BOMB_PTR->bomb_active == TRUE) {
                this->__unknown_flag_E = true;
            }
        }
    }

    // 0x409AD0
    dllexport gnu_noinline static void __sub_409AD0() asm_symbol_rel(0x409AD0) {
        SPELLCARD_PTR->__inline_sub_409AD0();
    }

    // 0x42A320
    dllexport gnu_noinline void thiscall start_spell(int32_t id, const char* name, int32_t time, int32_t mode) asm_symbol_rel(0x42A320) {
        __asm FINIT

        this->__timer_20.reset();
        this->id = id;
        byteloop_strcpy(this->name, name);
        this->__unknown_flag_A = true;
        this->__unknown_flag_B = true;
        this->__timeout_spell = false;
        this->__unknown_flag_C = false;
        this->__unknown_flag_D = false;
        if (!is_replay()) {
            byteloop_strcpy(SCOREFILE_MANAGER_PTR->primary_file.shottypes[GAME_MANAGER.globals.shottype_index()].spells[id].name, name);

        }
        Gui* gui = GUI_PTR;
        gui->__anm_vm_84->interrupt_and_run(2);
        gui->__anm_vm_88->interrupt_and_run(2);

        this->__int_8C = 1;
        this->__unknown_flag_E = false;
        this->__unknown_flag_F = false;

        this->__vm_id_10 = ASCII_MANAGER_PTR->ascii_anm->instantiate_vm_to_world_list_back(0);
        this->__vm_id_14 = SUPERVISOR.text_anm->instantiate_vm_to_world_list_back(2);
        this->__vm_id_18 = ASCII_MANAGER_PTR->ascii_anm->instantiate_vm_to_world_list_back(1);
        AnmVM* vm = this->__vm_id_14.get_vm_ptr();
        // TODO

        SOUND_MANAGER.play_sound(33);
        this->__vm_id_1C = EFFECT_MANAGER_PTR->effect_anm->instantiate_vm_to_world_list_back(13);

        Enemy* boss;
        clang_forceinline boss = ENEMY_MANAGER_PTR->get_boss_by_index(0);
        this->__float3_AC = boss->data.current_motion.position;
        this->__vm_id_1C.set_controller_position(&boss->data.current_motion.position);

        this->__vm_id_1C.__wtf_child_list_jank_A(11, 0)->data.current_context_vars.int_vars[2] = time;
        this->__vm_id_1C.__wtf_child_list_jank_A(13, 0)->data.current_context_vars.int_vars[2] = time;

        this->time = time;

        // ZUN didn't mark this as const,
        // so it gets written onto the
        // stack for no good reason.
        // 
        // Trying to run this code on overdrive
        // will end up reading the stack cookie
        // as the bonus.
        int32_t bonus_table[] = {
            500000,
            1000000,
            1500000,
            2000000,
            1000000
        };

        int32_t bonus = bonus_table[GAME_MANAGER.globals.difficulty] * GAME_MANAGER.globals.current_stage;
        this->__bonus_A = bonus;
        this->__bonus_B = bonus;
        if (bonus >= 1000000000) {
            this->__bonus_B = 1000000000;
        }

        EFFECT_MANAGER_PTR->effect_anm->instantiate_vm_to_world_list_back(20);

        auto& anm_sourceA = STAGE_DATA_PTR->innner[mode];
        anm_file_lookup(anm_sourceA.__spell_anm_indexA)->instantiate_vm_to_world_list_back(anm_sourceA.__spell_anm_scriptA);
        auto& anm_sourceB = STAGE_DATA_PTR->innner[mode];
        this->__unknown_flag_G = anm_sourceB.__spell_flag_state;
        anm_file_lookup(anm_sourceB.__spell_anm_indexB)->instantiate_vm_to_world_list_back(anm_sourceB.__spell_anm_scriptB);
        
        GAME_MANAGER.globals.__unknown_flag_A = false;
    }

    // 0x42A780
    dllexport gnu_noinline void thiscall end_spell() {
        if (this->__unknown_flag_A) {
            // TODO, sets some stage flag
            this->__vm_id_10.interrupt_tree(1);
            this->__vm_id_14.interrupt_tree(1);
            this->__vm_id_18.interrupt_tree(1);
            this->__unknown_flag_A = false;
            this->__vm_id_C.mark_tree_for_delete();
            this->__unknown_flag_E = false;
            //GUI_PTR->__sub_429C30();
            this->__vm_id_1C.mark_tree_for_delete();

            if (this->__unknown_flag_B) {
                GAME_MANAGER.add_to_score(this->__bonus_A);
                //GUI_PTR->__sub_441A50(0, this->__bonus_A);
                if (!is_replay()) {
                    // TODO: update scorefile
                }
                SOUND_MANAGER.play_sound(46);
            }
            else {
                Gui* gui = GUI_PTR;
                gui->__anm_id_B8.mark_tree_for_delete();
                gui->__anm_id_B8 = gui->__anm_loaded_2C0->instantiate_vm_to_world_list_back(50);
                gui->__int_134 = 1;
                gui->__anm_id_110 = gui->__anm_loaded_2C0->instantiate_vm_to_world_list_back(84);
            }

            if (this->__timeout_spell) {
                SOUND_MANAGER.play_sound(69);
            }
        }
    }

    // this probably actually takes an Enemy*, but
    // the memory access pattern when recompiling
    // would be wrong for that.
    inline bool __enemy_is_low_health(EnemyData* enemy_data, int32_t threshold_low, int32_t threshold_high) {
        if (this->__inline_sub_C()) {
            return false;
        }
        int32_t life_remaining = enemy_data->enemy()->data.life.remaining_current_attack;

        if (this->__unknown_flag_A) {
            return life_remaining < threshold_low;
        } else {
            return life_remaining < threshold_high;
        }
    }

    static inline Spellcard* allocate() {
        Spellcard* spellcard = new Spellcard();

        return spellcard;
    }
};
#pragma region // Spellcard Validation
ValidateFieldOffset32(0x0, Spellcard, task_flags);
ValidateFieldOffset32(0x4, Spellcard, on_tick_func);
ValidateFieldOffset32(0x8, Spellcard, on_draw_func);
ValidateFieldOffset32(0xC, Spellcard, __vm_id_C);
ValidateFieldOffset32(0x10, Spellcard, __vm_id_10);
ValidateFieldOffset32(0x14, Spellcard, __vm_id_14);
ValidateFieldOffset32(0x18, Spellcard, __vm_id_18);
ValidateFieldOffset32(0x1C, Spellcard, __vm_id_1C);
ValidateFieldOffset32(0x20, Spellcard, __timer_20);
ValidateFieldOffset32(0x34, Spellcard, name);
ValidateFieldOffset32(0x74, Spellcard, id);
ValidateFieldOffset32(0x78, Spellcard, flags);
ValidateFieldOffset32(0x7C, Spellcard, __bonus_A);
ValidateFieldOffset32(0x80, Spellcard, __bonus_B);
ValidateFieldOffset32(0x84, Spellcard, time);
ValidateFieldOffset32(0x8C, Spellcard, __int_8C);
ValidateFieldOffset32(0xAC, Spellcard, __float3_AC);
ValidateStructSize32(0xC0, Spellcard);
#pragma endregion

// size: 0x8
struct ItemSpriteData {
    int32_t __id_0; // 0x0
    int32_t __id_4; // 0x4
    // 0x8
};

enum ItemID : int32_t {
    InvalidItem = 0,
    PowerItem = 1,
    PointItem = 2,
    BigPowerItem = 3,
    LifeFragmentItem = 4,
    LifeItem = 5,
    BombFragmentItem = 6,
    BombItem = 7,
    FItem = 8,
    Piv5Item = 9,
    Piv10Item = 10,
    Piv20Item = 11,
    Piv30Item = 12,
    Piv40Item = 13,
    Piv50Item = 14,
    Item15 = 15,
    Item16 = 16,
    Item17 = 17,
    Item18 = 18,
    Item19 = 19
};

// size: 0xC94
struct Item {
    ZUNList<Item> __list_node_0; // 0x0
    AnmVM __vm_10; // 0x10
    AnmVM __vm_61C; // 0x61C
    AnmID __vm_id_C28; // 0xC28
    Float3 position; // 0xC2C
    Float3 velocity; // 0xC38
    float speed; // 0xC44
    ZUNAngle angle; // 0xC48
    Timer __timer_C4C; // 0xC4C
    unknown_fields(0x14); // 0xC60
    uint32_t state; // 0xC74
    ItemID id; // 0xC78
    int __dword_C7C; // 0xC7C
    float __float_C80; // 0xC80
    int32_t __int_C84; // 0xC84
    unknown_fields(0x4); // 0xC88
    int __dword_C8C; // 0xC8C
    int32_t sound_id; // 0xC90
    // 0xC94

    // 0x4472B0
    dllexport gnu_noinline int thiscall __sub_4472B0() asm_symbol_rel(0x4472B0) {
        switch (this->id) {
            case LifeFragmentItem: // 4
            case BombFragmentItem: // 6
            case Item15: // 15
            case LifeItem: // 5
            case BombItem: // 7
            case Item16: // 16
            case Item17: // 17
            case Item19: // 19
            case Item18: // 18

            case PointItem: // 2
                break;
        }
        return 0;
    }
};
#pragma region // Item Validation
ValidateFieldOffset32(0x0, Item, __list_node_0);
ValidateFieldOffset32(0x10, Item, __vm_10);
ValidateFieldOffset32(0x61C, Item, __vm_61C);
ValidateFieldOffset32(0xC28, Item, __vm_id_C28);
ValidateFieldOffset32(0xC2C, Item, position);
ValidateFieldOffset32(0xC38, Item, velocity);
ValidateFieldOffset32(0xC44, Item, speed);
ValidateFieldOffset32(0xC48, Item, angle);
ValidateFieldOffset32(0xC4C, Item, __timer_C4C);
ValidateFieldOffset32(0xC74, Item, state);
ValidateFieldOffset32(0xC78, Item, id);
ValidateFieldOffset32(0xC7C, Item, __dword_C7C);
ValidateFieldOffset32(0xC80, Item, __float_C80);
ValidateFieldOffset32(0xC84, Item, __int_C84);
ValidateFieldOffset32(0xC8C, Item, __dword_C8C);
ValidateFieldOffset32(0xC90, Item, sound_id);
ValidateStructSize32(0xC94, Item);
#pragma endregion

typedef struct ItemManager ItemManager;
extern "C" {
    // 0x4CF2EC
    dllexport extern ItemManager* ITEM_MANAGER_PTR asm("_ITEM_MANAGER_PTR");
}
// size: 0xE6BB28
struct ItemManager : ZUNTask {
    //ZUNTask base; // 0x0
    unknown_fields(0x8); // 0xC
    Item items[600]; // 0x14
    Item cancel_items[0x1000]; // 0x1D7AF4
    ZUNListHead<Item> items_freelist; // 0xE6BAF4
    ZUNListHead<Item> cancel_items_freelist; // 0xE6BB04
    float slowdown_factor; // 0xE6BB14
    int32_t items_onscreen; // 0xE6BB18
    int32_t item_count; // 0xE6BB1C
    unknown_fields(0x8); // 0xE6BB20
    // 0xE6BB28

private:
    // 0x446F40
    dllexport gnu_noinline Item* spawn_item(int32_t item_id, Float3* position, float, float angle, float speed, int32_t arg6) asm_symbol_rel(0x446F40) {
        if (item_id > 20) {
            return NULL;
        }
        ++this->item_count;
        switch (item_id) {
            default: // normal items

            case Piv5Item: // 9
            case Piv10Item: // 10
            case Piv20Item: // 11
            case Piv30Item: // 12
            case Piv40Item: // 13
            case Piv50Item: // 14

        }
        use_var(item_id);
        use_var(position);
        use_var(angle);
        use_var(speed);
        return (Item*)(position + (intptr_t)(item_id + angle + speed));
    }
public:
    inline Item* spawn_item(int32_t item_id, Float3* position, float angle, float speed, int32_t arg6) {
        return this->spawn_item(item_id, position, UNUSED_FLOAT, angle, speed, arg6);
    }
};
#pragma region // ItemManager Validation
ValidateFieldOffset32(0x0, ItemManager, task_flags);
ValidateFieldOffset32(0x4, ItemManager, on_tick_func);
ValidateFieldOffset32(0x8, ItemManager, on_draw_func);
ValidateFieldOffset32(0x14, ItemManager, items);
ValidateFieldOffset32(0x1D7AF4, ItemManager, cancel_items);
ValidateFieldOffset32(0xE6BAF4, ItemManager, items_freelist);
ValidateFieldOffset32(0xE6BB04, ItemManager, cancel_items_freelist);
ValidateFieldOffset32(0xE6BB14, ItemManager, slowdown_factor);
ValidateFieldOffset32(0xE6BB18, ItemManager, items_onscreen);
ValidateFieldOffset32(0xE6BB1C, ItemManager, item_count);
ValidateStructSize32(0xE6BB28, ItemManager);
#pragma endregion

// 0x430510
dllexport gnu_noinline void EnemyDrops::spawn_extra_items(Float3* position) {

}

// 0x42D0C0
dllexport void EnemyDrops::spawn_items(Float3* position) {
    int32_t main_drop = this->main_id;
    if (main_drop) {
        ITEM_MANAGER_PTR->spawn_item(main_drop, position, -HALF_PI_f, 2.2f, 0);
    }
    this->spawn_extra_items(position);
    this->main_id = 0;
}

// size: 0x10
struct BulletSpriteColorData {
    int32_t sprite_id; // 0x0
    int32_t spawn_effect_id; // 0x4
    int32_t cancel_effect_id; // 0x8
    int32_t cancel_script; // 0xC
    // 0x10
};
// size: 0x118
struct BulletSpriteData {
    int32_t anm_script; // 0x0
    BulletSpriteColorData color_data[16]; // 0x4
    float hitbox_size; // 0x104
    int32_t layer; // 0x108
    int __int_10C; // 0x10C
    int __int_110; // 0x110
    int32_t __anm_script_114; // 0x114
    // 0x118
};

extern "C" {
    // 0x4C5F8C
    extern BulletSpriteData BULLET_SPRITE_DATA[48] asm("_BULLET_SPRITE_DATA");
    // 0x4B36F0
    extern int32_t BULLET_IDK_DATA[8] asm("_BULLET_IDK_DATA");
}


typedef struct BulletManager BulletManager;
typedef struct LaserManager LaserManager;
extern "C" {
    // 0x4CF2BC
    extern BulletManager* BULLET_MANAGER_PTR asm("_BULLET_MANAGER_PTR");
    // 0x4CF3F4
    extern LaserManager* LASER_MANAGER_PTR asm("_LASER_MANAGER_PTR");
}

enum BulletEffectType : uint32_t {
    EX_NONE         = 0x00000000, // 0      0
    EX_DIST         = 0x00000001, // 1      1
    EX_ANIM         = 0x00000002, // 2      2
    EX_ACCEL        = 0x00000004, // 3      4
    EX_ANGLE_ACCEL  = 0x00000008, // 4      8
    EX_ANGLE        = 0x00000010, // 5      16
    // EX_NOP_A     = 0x00000020, // 6      32
    EX_BOUNCE       = 0x00000040, // 7      64
    EX_INVULN       = 0x00000080, // 8      128
    EX_OFFSCREEN    = 0x00000100, // 9      256
    EX_SETSPRITE    = 0x00000200, // 10     512
    EX_DELETE       = 0x00000400, // 11     1024
    EX_PLAYSOUND    = 0x00000800, // 12     2048
    EX_WRAP         = 0x00001000, // 13     4096
    EX_SHOOT        = 0x00002000, // 14     8192
    // EX_NOP_B     = 0x00004000, // 15     16384
    EX_REACT        = 0x00008000, // 16     32768
    EX_LOOP         = 0x00010000, // 17     65536
    EX_MOVE         = 0x00020000, // 18     131072
    EX_VEL          = 0x00040000, // 19     262144
    EX_VELADD       = 0x00080000, // 20     524288
    EX_BRIGHT       = 0x00100000, // 21     1048576
    EX_VELTIME      = 0x00200000, // 22     2097152
    EX_SIZE         = 0x00400000, // 23     4194304
    EX_SAVEANGLE    = 0x00800000, // 24     8388608
    EX_ENEMY        = 0x01000000, // 25     16777216
    EX_LAYER        = 0x02000000, // 26     33554432
    EX_DELAY        = 0x04000000, // 27     67108864
    EX_LASER        = 0x08000000, // 28     134217728
    // EX_NOP_C     = 0x10000000, // 29     268435456
    EX_HITBOX       = 0x20000000, // 30     536870912
    EX_HOMING       = 0x40000000, // 31     1073741824
    EX_WAIT         = 0x80000000, // 32     -2147483648
};

// size: 0x48
struct BulletEffectData {
    Timer timer; // 0x0
    union { // 0x14
        float speed;
        float acceleration;
    };
    union { // 0x18
        float angle;
        float angular_velocity;
    };
    union { // 0x1C
        Float3 position;
        Float3 size;
    };
    union { // 0x28
        Float3 velocity;
        Float3 acceleration_vec;
        Float3 target;
    };
    union { // 0x34
        int32_t duration;
        int32_t __offscreen_unknown;
    };
    union { // 0x38
        int32_t max_count;
        int32_t mode;
    };
    union { // 0x3C
        int32_t count;
        int32_t flags;
    };
    int32_t type; // 0x40
    int32_t __int_44; // 0x44
    // 0x48

    inline BulletEffectData() {}
};

// size: 0xFA0
struct Bullet {
    ZUNList<Bullet> __list_0; // 0x0
    ZUNList<Bullet> __list_10; // 0x10
    union {
        uint32_t flags; // 0x20
        struct {
            uint32_t : 2;
            uint32_t grazed : 1;
            uint32_t : 1;
            uint32_t __unknown_flag_A : 1;
            uint32_t : 1;
            uint32_t __unknown_flag_B : 1;
            uint32_t : 2;
            uint32_t disable_cancel_items : 1;
        };
    };
    int32_t invulnerable_time; // 0x24
    AnmVM vm; // 0x28
    AnmID __anm_tree_id; // 0x634
    Float3 position; // 0x638
    Float3 velocity; // 0x644
    float speed; // 0x650
    ZUNAngle angle; // 0x654
    float hitbox_radius; // 0x658
    float __hitbox_radius_copy; // 0x65C
    int32_t bullet_manager_index; // 0x660
    int32_t __ex_func_a; // 0x664
    unknown_fields(0x8); // 0x668
    int __dword_670; // 0x670
    int32_t __cancel_script_id; // 0x674
    unknown_fields(0x4); // 0x678
    int32_t effect_index; // 0x67C
    int32_t effect_loop_index; // 0x680
    uint32_t active_effects; // 0x684
    uint32_t initial_effects; // 0x688
    Bullet* next_in_layer; // 0x68C
    unknown_fields(0x4); // 0x690
    int32_t transform_sound; // 0x694
    int32_t layer; // 0x698
    BulletEffectArgs effects[24]; // 0x69C
    BulletEffectData effect_speedup; // 0xABC
    BulletEffectData effect_accel; // 0xB04
    BulletEffectData effect_angle_accel; // 0xB4C
    BulletEffectData effect_angle; // 0xB94
    BulletEffectData effect_bounce; // 0xBDC
    BulletEffectData effect_wait; // 0xC24
    BulletEffectData effect_wrap; // 0xC6C
    BulletEffectData effect_homing; // 0xCB4
    BulletEffectData effect_move; // 0xCFC
    BulletEffectData effect_veladd; // 0xD44
    BulletEffectData effect_veltime; // 0xD8C
    BulletEffectData effect_offscreen; // 0xDD4
    BulletEffectData effect_saveangle; // 0xE1C
    BulletEffectData effect_delay; // 0xE64
    unknown_fields(0x4); // 0xEAC
    ZUNInterp<Float3> effect_move_interp; // 0xEB0
    ZUNInterp<float> scale_interp; // 0xF08
    float scale; // 0xF38
    Timer __timer_F3C; // 0xF3C
    Timer __timer_F50; // 0xF50
    unknown_fields(0x4); // 0xF64
    uint16_t state; // 0xF68
    probably_padding_bytes(0x2); // 0xF6A
    Timer __timer_F6C; // 0xF6C
    Timer __timer_F80; // 0xF80
    BulletSpriteData* sprite_data; // 0xF94
    int16_t sprite; // 0xF98
    int16_t color; // 0xF9A
    unknown_fields(0x4); // 0xF9C
    // 0xFA0

    // 0x428E90
    dllexport int32_t cancel(int) asm_symbol_rel(0x428E90) {
        this->vm.run_on_interrupt(1);
        this->vm.run_anm();
        AnmManager::interrupt_tree(this->__anm_tree_id, 1);
        if (!this->disable_cancel_items) {
            // item spawning stuff
        }
        this->state = 4;
        this->position += this->velocity * GAME_SPEED.value * 0.5f;
        this->__timer_F6C.reset();
        return 0;
    }

    // 0x425C60
    dllexport void run_effects() asm_symbol_rel(0x425C60);
};

enum LaserType {
    LineLaser = 0,
    InfiniteLaser = 1,
    CurvyLaser = 2,
    BeamLaser = 3
};

typedef struct LaserData LaserData;
// size: 0x788
struct LaserData {
    // void* vtable; // 0x0
    //LaserData* next_piss; // 0x4
    //LaserData* prev; // 0x8
    ZUNEmbeddedListR<LaserData> embedded_node; // 0x4
    union {
        uint32_t __flags_C; // 0xC
        struct {
            uint32_t __unknown_flag_A : 1;
            uint32_t __unknown_field_A : 2;
            uint32_t __unknown_flag_B : 1;
        };
    };
    int32_t state; // 0x10
    int32_t type; // 0x14
    Timer __timer_18; // 0x18
    Timer graze_timer; // 0x2C
    Timer __timer_40; // 0x40
    Float3 position; // 0x54
    Float3 velocity; // 0x60
    float angle; // 0x6C
    float length; // 0x70
    float width; // 0x74
    float speed; // 0x78
    float __float_7C; // 0x7C
    int32_t id; // 0x80
    BulletEffectData effect_speedup; // 0x84
    BulletEffectData effect_accel; // 0xCC
    BulletEffectData effect_angle_accel; // 0x114
    BulletEffectData effect_angle; // 0x15C
    BulletEffectData effect_bounce; // 0x1A4
    BulletEffectData effect_wait; // 0x1EC
    BulletEffectData effect_wrap; // 0x234
    BulletEffectData effect_unused[17]; // 0x27C
    int32_t effect_index; // 0x744
    uint32_t active_effects; // 0x748
    unknown_fields(0x8); // 0x74C
    Timer __timer_754; // 0x754
    Timer offscreen_timer; // 0x768
    int32_t invulnerable_time; // 0x77C
    int32_t sprite; // 0x780
    int32_t color; // 0x784
    uint8_t derived_class[0]; // 0x788

    inline void zero_contents() {
        zero_this();
    }

    // 0x448170
    dllexport gnu_noinline LaserData() {
        this->zero_contents();
        this->__timer_18.reset();
    }

private:
    // 0x42D6A0
    dllexport gnu_noinline void vectorcall set_angle(float, float angle) asm_symbol_rel(0x42D6A0) {
        this->angle = angle;
    }
public:
    inline void set_angle(float angle) {
        return this->set_angle(UNUSED_FLOAT, angle);
    }

private:
    // 0x42D6B0
    dllexport gnu_noinline void vectorcall set_width(float, float width) asm_symbol_rel(0x42D6B0) {
        this->width = width;
    }
public:
    inline void set_width(float width) {
        return this->set_width(UNUSED_FLOAT, width);
    }

private:
    // 0x42D6C0
    dllexport gnu_noinline void vectorcall set_speed(float, float speed) asm_symbol_rel(0x42D6C0) {
        this->speed = speed;
    }
public:
    inline void set_speed(float speed) {
        return this->set_speed(UNUSED_FLOAT, speed);
    }

    // 0x42D6D0
    dllexport gnu_noinline void thiscall set_id(int32_t id = 0) asm_symbol_rel(0x42D6D0) {
        this->id = 0;
    }

    // 0x42D700
    dllexport gnu_noinline void thiscall set_position(Float3* position) asm_symbol_rel(0x42D700) {
        // why zun, don't do this
        *(Float3*)&this->derived_class[0] = this->position = *position;
    }

    // 0x42D730
    // Line:        __angle_C, __length_related, length
    // Infinite:    velocity
    // Curve:       __angle_C, width, __speed1
    // Beam:        velocity
    dllexport gnu_noinline void thiscall set_velocity(Float3* set_velocity) asm_symbol_rel(0x42D730) {
        *(Float3*)&this->derived_class[0xC] = *set_velocity;
    }

private:
    // 0x42D750
    dllexport gnu_noinline void vectorcall set_angular_velocity(float, float angular_velocity) {
        *(float*)&this->derived_class[0x1C] = angular_velocity;
    }
public:
    // Line:        width
    // Infinite:    angular_velocity
    // Curve:       color (incorrect type)
    // Beam:        angular_velocity
    inline void set_angular_velocity(float angular_velocity) {
        return this->set_angular_velocity(UNUSED_FLOAT, angular_velocity);
    }

#pragma region // LaserData method stubs

    // 0x447FC0
    // Method 0x0
    dllexport virtual gnu_noinline void thiscall __method_0(float magnitude, Float3* arg2) asm_symbol_rel(0x447FC0) {}
    // 0x447FD0
    // Method 0x4
    dllexport virtual gnu_noinline void thiscall run_effects() asm_symbol_rel(0x447FD0) {}
    // 0x447FE0
    // Method 0x8
    dllexport virtual gnu_noinline void thiscall __method_8(int) asm_symbol_rel(0x447FE0) {}
    // 0x447FF0
    // Method 0xC
    dllexport virtual gnu_noinline int thiscall initialize(void*) asm_symbol_rel(0x447FF0) {
        return 0;
    }
    // 0x448000
    // Method 0x10
    dllexport virtual gnu_noinline int thiscall on_tick() asm_symbol_rel(0x448000) {
        return 0;
    }
    // 0x448010
    // Method 0x14
    dllexport virtual gnu_noinline int thiscall on_draw() asm_symbol_rel(0x448010) {
        return 0;
    }
    // 0x448020
    // Method 0x18
    dllexport virtual gnu_noinline int thiscall __method_18() asm_symbol_rel(0x448020) {
        this->embedded_node.unlink();
        return 0;
    }
    // 0x448040
    // Method 0x1C
    dllexport virtual gnu_noinline int thiscall __method_1C(int, int, int, int, int, int) asm_symbol_rel(0x448040) {
        return 0;
    }
    // 0x448050
    // Method 0x20
    dllexport virtual gnu_noinline int thiscall cancel_as_bomb_rectangle(Float3* arg1, Float3* arg2, float arg3, int arg4, int32_t arg5) asm_symbol_rel(0x448050) {
        return 0;
    }
    // 0x448060
    // Method 0x24
    dllexport virtual gnu_noinline int thiscall cancel_as_bomb_circle(Float3* arg1, float arg2, int arg3, int arg4) asm_symbol_rel(0x448060) {
        return 0;
    }
    // 0x448070
    // Method 0x28
    dllexport virtual gnu_noinline int thiscall cancel(int32_t arg1, int32_t arg2) asm_symbol_rel(0x448070) {
        return 0;
    }
    // 0x448080
    // Method 0x2C
    dllexport virtual gnu_noinline int thiscall __method_2C(int, int, int, int) asm_symbol_rel(0x448080) {
        return 0;
    }
    // 0x448090
    // Method 0x30
    dllexport virtual gnu_noinline int thiscall __method_30(Float2* arg1, float arg2) asm_symbol_rel(0x448090) {
        return 0;
    }
    // 0x4480A0
    // Method 0x34
    dllexport virtual gnu_noinline int thiscall check_graze_or_kill(BOOL arg1) asm_symbol_rel(0x4480A0) {
        return 0;
    }
    // 0x4480B0
    // Method 0x38
    dllexport virtual gnu_noinline int thiscall __method_38() asm_symbol_rel(0x4480B0) {
        return 0;
    }
    // 0x4480C0
    // Method 0x3C
    dllexport virtual gnu_noinline int thiscall __method_3C() asm_symbol_rel(0x4480C0) {
        return 0;
    }
    // 0x4480D0
    // Method 0x40
    dllexport virtual gnu_noinline int thiscall __method_40() asm_symbol_rel(0x4480D0) {
        return 0;
    }
    // 0x4480E0
    // Method 0x44
    dllexport virtual gnu_noinline int thiscall __method_44() asm_symbol_rel(0x4480E0) {
        return 0;
    }
    // 0x4480F0
    // Method 0x48
    dllexport virtual gnu_noinline int thiscall __method_48() asm_symbol_rel(0x4480F0) {
        return 0;
    }
    // 0x448100
    // Method 0x4C
    dllexport virtual gnu_noinline int thiscall __method_4C() asm_symbol_rel(0x448100) {
        return 0;
    }
    // 0x448110
    // Method 0x50
    dllexport virtual gnu_noinline int thiscall __method_50() asm_symbol_rel(0x448110) {
        return 0;
    }
    // 0x448120
    // Method 0x54
    dllexport virtual gnu_noinline int thiscall __method_54() asm_symbol_rel(0x448120) {
        return 0;
    }
    // 0x448130
    // Method 0x58
    dllexport virtual gnu_noinline int thiscall __method_58() asm_symbol_rel(0x448130) {
        return 0;
    }
    // 0x448140
    // Method 0x5C
    dllexport virtual gnu_noinline int thiscall __method_5C() asm_symbol_rel(0x448140) {
        return 0;
    }
    // 0x448150
    // Method 0x60
    dllexport virtual gnu_noinline int thiscall __method_60() asm_symbol_rel(0x448150) {
        return 0;
    }
    // 0x448160
    // Method 0x64
    dllexport virtual gnu_noinline LaserData* thiscall duplicate() asm_symbol_rel(0x448160) {
        return NULL;
    }
    // 0x4481E0
    // Method 0x68
    virtual ~LaserData() {
    }

#pragma endregion // LaserData method stubs

    inline int on_tick_common() {
        if (this->__unknown_flag_B) {
            this->check_graze_or_kill(TRUE);
        }
        else {
            if (this->on_tick()) {
                return 1;
            }
            ++this->__timer_18;
            this->__unknown_flag_A = true;
        }
        return 0;
    }
};

// size: 0x460
struct LaserLineParams {
    Float3 position; // 0x0, 0x788
    float __angle_C; // 0xC, 0x794
    float __length_related; // 0x10, 0x798 (max length?)
    float length; // 0x14, 0x79C
    float __float_18; // 0x18, 0x7A0
    float width; // 0x1C, 0x7A4
    float __speed_1; // 0x20, 0x7A8
    int32_t sprite; // 0x24, 0x7AC
    int32_t color; // 0x28, 0x7B0
    float distance; // 0x2C, 0x7B4
    int32_t effect_index; // 0x30, 0x7B8
    union {
        uint32_t flags; // 0x34, 0x7BC
        struct {
            uint32_t __unknown_flag_A : 1; // 1
            uint32_t __unknown_flag_B : 1; // 2
        };
    };
    BulletEffectArgs effects[BULLET_EFFECT_MAX]; // 0x38, 0x7C0
    int32_t shot_sound; // 0x458, 0xBE0
    int32_t transform_sound; // 0x45C, 0xBE4
    // 0x460, 0xBE8

    inline void zero_contents() {
        zero_this();
    }

    inline LaserLineParams() {
        this->zero_contents();
    }

    inline LaserLineParams(int) {
    }
};

// size: 0x1E0C
struct LaserLine : LaserData {
    LaserLineParams params; // 0x788
    AnmVM __vm_BE8; // 0xBE8
    AnmVM __vm_11F4; // 0x11F4
    AnmVM __vm_1800; // 0x1800
    // 0x1E0C

    // 0x4482C0
    dllexport gnu_noinline LaserLine() {
    }

    // 0x448220
    // Method 0x0
    dllexport virtual gnu_noinline void thiscall __method_0(float magnitude, Float3* arg2) override asm_symbol_rel(0x448220) {
        arg2->make_from_vector(this->angle, magnitude);
        *arg2 += this->position;
    }

    // 0x4494F0
    // Method 0x4
    dllexport virtual gnu_noinline void thiscall run_effects() override asm_symbol_rel(0x4494F0);

    // 0x4490D0
    // Method 0xC
    dllexport virtual gnu_noinline int thiscall initialize(void* data) override asm_symbol_rel(0x4490D0);

    // 0x44A4F0
    // Method 0x10
    dllexport virtual gnu_noinline int thiscall on_tick() override asm_symbol_rel(0x44A4F0) {
        return 0;
    }

    // 0x44AB60
    // Method 0x14
    dllexport virtual gnu_noinline int thiscall on_draw() override asm_symbol_rel(0x44AB60) {
        this->__vm_BE8.data.position = this->position;
        this->__vm_BE8.data.rotation.z = reduce_angle_add(this->angle, HALF_PI_f);
        this->__vm_BE8.data.__z_rotation = true;
        ANM_MANAGER_PTR->draw_vm(&this->__vm_BE8);
        this->__vm_1800.data.position.make_from_vector(this->angle, this->length);
        this->__vm_1800.data.position.x += this->position.x;
        this->__vm_1800.data.position.y += this->position.y;
        this->__vm_1800.data.position.z = 0.0f;
        ANM_MANAGER_PTR->draw_vm(&this->__vm_1800);
        if (this->__float_7C == 0.0f) {
            this->__vm_11F4.data.position = this->position;
            ANM_MANAGER_PTR->draw_vm(&this->__vm_11F4);
        }
        return 0;
    }

    // 0x44AC90
    // Method 0x18
    dllexport virtual gnu_noinline int thiscall __method_18() override asm_symbol_rel(0x44AC90) {
        return 0;
    }

    // 0x44B5B0
    // Method 0x1C
    dllexport virtual gnu_noinline int thiscall __method_1C(int, int, int, int, int, int) override asm_symbol_rel(0x44B5B0) {
        // TODO
    }

    // 0x44ACA0
    // Method 0x20
    dllexport virtual gnu_noinline int thiscall cancel_as_bomb_rectangle(Float3* arg1, Float3* arg2, float arg3, int arg4, int32_t arg5) override asm_symbol_rel(0x44ACA0) {
        // TODO
    }

    // 0x44BD00
    // Method 0x24
    dllexport virtual gnu_noinline int thiscall cancel_as_bomb_circle(Float3* arg1, float arg2, int arg3, int arg4) override asm_symbol_rel(0x44BD00) {
        // TODO
    }

    // 0x44C420
    // Method 0x28
    dllexport virtual gnu_noinline int thiscall cancel(int, int) override asm_symbol_rel(0x44C420) {
        // TODO
    }

    // 0x448210
    // Method 0x2C
    dllexport virtual gnu_noinline int thiscall __method_2C(int, int, int, int) override asm_symbol_rel(0x448210) {
        return 0;
    }

    // 0x44C790
    // Method 0x30
    dllexport virtual gnu_noinline int thiscall __method_30(Float2* arg1, float arg2) override asm_symbol_rel(0x44C790) {
        float x = arg1->x - this->position.x;
        float y = arg1->y - this->position.y;
        float angle = -this->angle;
        float y_unit = zsinf(angle);
        float x_unit = zcosf(angle);
        float A = (x_unit * x) - (y_unit * y);
        float B = (x_unit * y) + (y_unit * x);
        if (
            !(A - arg2 > this->length) &&
            !(B - arg2 > this->width * 0.5) &&
            !(A + arg2 < 0.0f) &&
            !(B + arg2 < -this->width * 0.5)
        ) {
            return 2;
        }
        return 0;
    }

    // 0x44A950
    // Method 0x34
    dllexport virtual gnu_noinline int thiscall check_graze_or_kill(BOOL graze_only) override asm_symbol_rel(0x44A950) {
        float length = this->length;
        if (length > 16.0f) {
            float width = this->width;
            if (width > 3.0f) {
                Float3 position;
                if (!this->params.__unknown_flag_B) {
                    position.make_from_vector(this->angle, length / 10.0f);
                    position += this->position;
                } else {
                    position = this->position;
                }
                if (!this->params.__unknown_flag_B) {
                    length *= 4.0f;
                    length /= 5.0f;
                }
                if (width < 32.0f) {
                    width *= 0.5f;
                } else {
                    width -= (width + 16.0f) * 0.5f;
                }
                int32_t result = Player::__check_collision_rotated_rectangle(&position, this->angle, width, length, graze_only);
                if (result == 1) {
                    Float3 B(32.0f, 32.0f, 0.0f);
                    this->cancel_as_bomb_rectangle(&PLAYER_PTR->data.position, &B, UNUSED_DWORD, 0, result);
                    return 0;
                }
                if (result == 2) {
                    if (this->graze_timer.is_multiple_of(3)) {
                        float C = this->angle;
                        float D;
                        float E;
                        __sub_403BC0(&D, &E, position.x, position.y, C, this->position.x, this->position.y, reduce_angle(C + HALF_PI_f));
                        Float2 F(D, E);
                        Player::do_graze(&F);
                        this->graze_timer.increment();
                    }
                }
            }
        }
        return 0;
    }

    // 0x44A370
    // Method 0x3C
    dllexport virtual gnu_noinline int thiscall __method_3C() override asm_symbol_rel(0x44A370) {
        // TODO
    }

    // 0x44A1F0
    // Method 0x44
    dllexport virtual gnu_noinline int thiscall __method_44() override asm_symbol_rel(0x44A1F0) {
        // TODO
    }

    // 0x449E00
    // Method 0x50
    dllexport virtual gnu_noinline int thiscall __method_50() override asm_symbol_rel(0x449E00) {
        // TODO
    }

    // 0x448280
    // Method 0x64
    dllexport virtual gnu_noinline LaserData* thiscall duplicate() override asm_symbol_rel(0x448280) {
        LaserLine* new_laser = new LaserLine();
        memcpy(new_laser, this, sizeof(LaserLine));
        return new_laser;
    }
};

// size: 0x480
struct LaserInfiniteParams {
    Float3 position; // 0x0, 0x788
    Float3 velocity; // 0xC, 0x794
    float __angle_18; // 0x18, 0x7A0
    float angular_velocity; // 0x1C, 0x7A4
    float length; // 0x20, 0x7A8
    float __float_24; // 0x24, 0x7AC
    float width; // 0x28, 0x7B0
    float __speed_1; // 0x2C, 0x7B4
    int32_t start_time; // 0x30, 0x7B8
    int32_t expand_time; // 0x34, 0x7BC
    int32_t duration; // 0x38, 0x7C0
    int32_t stop_time; // 0x3C, 0x7C4
    int32_t shot_sound; // 0x40, 0x7C8
    int32_t transform_sound; // 0x44, 0x7CC
    int32_t laser_id; // 0x48, 0x7D0
    float distance; // 0x4C, 0x7D4
    int32_t __start_ex; // 0x50, 0x7D8
    int32_t sprite; // 0x54, 0x7DC
    int32_t color; // 0x58, 0x7E0
    union {
        uint32_t flags; // 0x5C, 0x7E4
        struct {
            uint32_t : 1; // 1
            uint32_t __unknown_flag_A : 1; // 2
        };
    };
    BulletEffectArgs effects[BULLET_EFFECT_MAX]; // 0x60, 0x7E8
    // 0x480, 0xC08

    inline void zero_contents() {
        zero_this();
    }

    inline LaserInfiniteParams() {
        this->zero_contents();
    }

    inline LaserInfiniteParams(int) {
    }
};

// size: 0x1824
struct LaserInfinite : LaserData {
    LaserInfiniteParams params; // 0x788
    int __dword_C08; // 0xC08
    AnmVM __vm_C0C; // 0xC0C
    AnmVM __vm_1218; // 0x1218
    // 0x1824

    inline LaserInfinite() {
    }
};

// size: 0x460
struct LaserCurveParams {
    Float3 position; // 0x0, 0x788
    float __angle_C; // 0xC, 0x794
    float width; // 0x10, 0x798
    float __speed_1; // 0x14, 0x79C
    int32_t sprite; // 0x18, 0x7A0
    int32_t color; // 0x1C, 0x7A4
    int32_t curve_length; // 0x20, 0x7A8
    float distance; // 0x24, 0x7AC
    union {
        uint32_t flags; // 0x28, 0x7B0
        struct {

        };
    };
    BulletEffectArgs effects[BULLET_EFFECT_MAX]; // 0x2C, 0x7B4
    int32_t shot_sound; // 0x44C, 0xBD4
    int32_t transform_sound; // 0x450, 0xBD8
    unknown_fields(0xC); // 0x454, 0xBDC
    // 0x460, 0xBE8

    inline void zero_contents() {
        zero_this();
    }

    inline LaserCurveParams() {
        this->zero_contents();
    }
};

// size: 0x1844
struct LaserCurve : LaserData {

    // size: 0x20
    struct LaserCurveNode {
        Float3 position; // 0x0
        Float3 velocity; // 0xC
        float angle; // 0x18
        float speed; // 0x1C
        // 0x20
    };

    // size: 0x3C
    struct LaserCurveNodeEx {
        LaserCurveNodeEx* next; // 0x0
        LaserCurveNodeEx* prev; // 0x4
        float __float_8; // 0x8
        float __float_C; // 0xC
        int32_t move_type; // 0x10
        Float3 __float3_14; // 0x14
        Float3 __float3_20; // 0x20
        float angle; // 0x2C
        float speed; // 0x30
        float __float_34; // 0x34
        float __float_38; // 0x38
        // 0x3C
    };

    LaserCurveParams params; // 0x788
    AnmVM __vm_BE8; // 0xBE8
    AnmVM __vm_11F4; // 0x11F4
    LaserCurveNode* nodes; // 0x1800
    SpriteVertex* vertices; // 0x1804
    LaserCurveNodeEx node_ex; // 0x1808
    // 0x1844
};

// size: 0x45C
struct LaserBeamParams {
    Float3 position; // 0x0
    Float3 velocity; // 0xC
    float __angle_18; // 0x18
    float angular_velocity; // 0x1C
    float __float_20; // 0x20
    float __float_24; // 0x24
    int32_t laser_id; // 0x28
    int32_t color; // 0x2C
    float distance; // 0x30
    int32_t start_time; // 0x34
    unknown_fields(0x4); // 0x38
    BulletEffectArgs effects[24]; // 0x3C
    // 0x45C

    inline void zero_contents() {
        zero_this();
    }

    inline LaserBeamParams() {
        this->zero_contents();
    }
};

// size: 0x21F4
struct LaserBeam : LaserData {

    LaserBeamParams params; // 0x788
    AnmVM __vm_BE4; // 0xBE4
    unknown_fields(0x1004); // 0x11F0
    // 0x21F4

    inline LaserBeam() {
    }
};

// size: 0x10
struct BulletIters {
    ZUNList<Bullet>* current[2]; // 0x0
    ZUNList<Bullet>* next[2]; // 0x8
    // 0x10
};

// size: 0x7A41F8
struct BulletManager : ZUNTask {
    // ZUNTask base; // 0x0
    unknown_fields(0x4); // 0xC
    Bullet* __bullet_cache_pointers[12]; // 0x10
    int32_t __int_40; // 0x40 (ECL variable -9898)
    float player_protect_radius; // 0x44
    Float2 __bounce_bounds; // 0x48
    unknown_fields(0x8); // 0x50
    int __graze_array[20]; // 0x58
    int __int_A8; // 0xA8
    ZUNList<void> __unknown_list_AC; // 0xAC
    ZUNListHead<Bullet> bullet_tick_list; // 0xBC
    unknown_fields(0x20); // 0xCC
    Bullet dummy_bullet; // 0xEC
    Bullet bullets[2000]; // 0x108C
    int32_t anm_ids[2001]; // 0x7A228C
    int32_t __cancel_counter; // 0x7A41D0
    int32_t __int_7A41D4; // 0x7A41D4
    //ZUNList<Bullet>* bullet_iter_current; // 0x7A41D8
    //ZUNList<Bullet>* bullet_iter_current2; // 0x7A41DC
    //ZUNList<Bullet>* bullet_iter_next; // 0x7A41E0
    //ZUNList<Bullet>* bullet_iter_next2; // 0x7A41E4
    BulletIters bullet_iter; // 0x7A41D8
    int32_t __cancel_counter2; // 0x7A41E8
    AnmLoaded* bullet_anm; // 0x7A41EC
    int32_t __unknown_counter_flag; // 0x7A41F0
    int32_t __unknown_counter; // 0x7A41F4
    // 0x7A41F8
    
    // 0x409940
    dllexport gnu_noinline Bullet* thiscall start_bullet_iter(uint32_t index) asm_symbol_rel(0x409940) {
        ZUNList<Bullet>* node = this->bullet_tick_list.next;
        this->bullet_iter.current[index] = node;
        if (node) {
            this->bullet_iter.next[index] = node->next;
            return node->data;
        }
        else {
            this->bullet_iter.next[index] = NULL;
            return NULL;
        }
    }

    // 0x42CC70
    dllexport gnu_noinline uint32_t thiscall __count_graze_array() asm_symbol_rel(0x42CC70) {
        // MSVC loops this 2 at a time?
        uint32_t count = 0;
        for (size_t i = 0; i < countof(this->__graze_array); ++i) {
            count += this->__graze_array[i];
        }
        return count;
    }

private:
    // 0x42CCA0
    dllexport gnu_noinline void vectorcall set_player_protect_radius(float, float radius) asm_symbol_rel(0x42CCA0) {
        BULLET_MANAGER_PTR->player_protect_radius = radius;
    }
public:
    inline void set_player_protect_radius(float radius) {
        return this->set_player_protect_radius(UNUSED_FLOAT, radius);
    }

private:
    // 0x4237E0
    dllexport gnu_noinline void vectorcall __set_bounce_bounds(float, float width, float height) {
        this->__bounce_bounds.x = width;
        this->__bounce_bounds.y = height;
    }
public:
    inline void __set_bounce_bounds(float width, float height) {
        return this->__set_bounce_bounds(UNUSED_FLOAT, width, height);
    }

    // 0x4297A0
    dllexport gnu_noinline void thiscall cancel_all(int32_t = UNUSED_DWORD) asm_symbol_rel(0x4297A0) {
        for (int32_t i = countof(this->bullets); i; --i) {

        }
    }

    // 0x424FE0
    dllexport gnu_noinline int32_t thiscall shoot_one_bullet(ShooterData* shooter, int count1, int count2, float angle_to_player) asm_symbol_rel(0x424FE0) {

    }
    
    // 0x427810
    dllexport gnu_noinline int32_t thiscall shoot_bullets(ShooterData* shooter) asm_symbol_rel(0x427810) {
        BulletManager* bullet_manager = BULLET_MANAGER_PTR;
        float angle_to_player = Player::angle_to_player_from_point(&shooter->position);
        for (int32_t i = 0; i < shooter->count2; ++i) {
            for (int32_t j = 0; j < shooter->count1; ++j) {
                int32_t result = bullet_manager->shoot_one_bullet(shooter, j, i, angle_to_player);
                if (result != 0 && result == 1) goto break_outer;
            }
        }
    break_outer:
        if (shooter->play_sound_on_spawn) {
            SOUND_MANAGER.play_sound_positioned(shooter->shoot_sound, shooter->position.x);
        }
        return 0;
    }
    
    // 0x424C50
    dllexport gnu_noinline int32_t thiscall on_tick__impl() asm_symbol_rel(0x424C50) {
        for (size_t i = countof(this->__graze_array) - 1; i; --i) {
            this->__graze_array[i] = this->__graze_array[i - 1];
        }
        this->__graze_array[0] = 0;
        
        Bullet* cur_bullet = this->start_bullet_iter(0);

        this->__int_40 = 0;
        this->__bullet_cache_pointers[5] = NULL;
        this->__bullet_cache_pointers[4] = NULL;
        this->__bullet_cache_pointers[3] = NULL;
        this->__bullet_cache_pointers[2] = NULL;
        this->__bullet_cache_pointers[1] = NULL;
        this->__bullet_cache_pointers[0] = NULL;
        this->__bullet_cache_pointers[11] = NULL;
        this->__bullet_cache_pointers[10] = NULL;
        this->__bullet_cache_pointers[9] = NULL;
        this->__bullet_cache_pointers[8] = NULL;
        this->__bullet_cache_pointers[7] = NULL;
        this->__bullet_cache_pointers[6] = NULL;

        while (cur_bullet) {

        }

        return 1;
    }
    
    // 0x424E70
    dllexport gnu_noinline int32_t thiscall on_tick() asm_symbol_rel(0x424E70) {
        GameThread* game_thread_ptr = GAME_THREAD_PTR;
        if (
            (game_thread_ptr && (game_thread_ptr->__unknown_flag_A | game_thread_ptr->skip_flag)) ||
            ABILITY_SHOP_PTR
        ) {
            return 1;
        }
        return this->on_tick__impl();
    }
};

static inline constexpr int32_t MAX_LASER_COUNT = 512;

// size: 0x7C4
struct LaserManager : ZUNTask {
    // ZUNTask base; // 0x0
    LaserData dummy_laser; // 0xC
    LaserData* list_tail; // 0x794
    int32_t laser_count; // 0x798
    int32_t __prev_laser_id; // 0x79C
    Float3 __float3_7A0; // 0x7A0
    Float3 __float3_7AC; // 0x7AC
    AnmLoaded* bullet_anm; // 0x7B8
    int32_t __int_7BC; // 0x7BC
    LaserData* __current_stupid_iter; // 0x7C0
    // 0x7C4

    inline void zero_contents() {
        zero_this();
    }

    inline ZUNEmbeddedListRHead<LaserData>& list_head() {
        return *(ZUNEmbeddedListRHead<LaserData>*)&this->dummy_laser.embedded_node;
    }

    inline LaserManager() {
        this->zero_contents();
        this->__prev_laser_id = 0x10000;
    }

    inline void __increment_laser_id() {
        ++this->__prev_laser_id;
        this->__prev_laser_id = std::max(this->__prev_laser_id, 0x10000);
    }

    inline void allocate_new_line_laser(LaserLineParams* data) {
        if (this->laser_count < MAX_LASER_COUNT) {
            this->__increment_laser_id();
            LaserLine* laser = new LaserLine();
            laser->id = this->__prev_laser_id;
            this->list_tail->embedded_node.append(laser);
            ++this->laser_count;
            this->list_tail = laser;
            laser->initialize(data);
        }
    }

    // 0x448920
    dllexport gnu_noinline int32_t thiscall allocate_new_laser(int32_t laser_type, void* data) asm_symbol_rel(0x448920) {
        LaserManager* laser_manager = LASER_MANAGER_PTR;
        if (this->laser_count >= MAX_LASER_COUNT) {
            return 0;
        }
        laser_manager->__increment_laser_id();
        switch (laser_type) {
            case LineLaser: {
                LaserLine* laser = new LaserLine();
                laser->id = laser_manager->__prev_laser_id;
                laser_manager->list_tail->embedded_node.append(laser);
                ++laser_manager->laser_count;
                laser_manager->list_tail = laser;
                laser->initialize(data);
                break;
            }
            case InfiniteLaser: {
                LaserInfinite* laser = new LaserInfinite();
                laser->id = laser_manager->__prev_laser_id;
                laser_manager->list_tail->embedded_node.append(laser);
                ++laser_manager->laser_count;
                laser_manager->list_tail = laser;
                laser->initialize(data);
                break;
            }
            case BeamLaser: {
                LaserBeam* laser = new LaserBeam();
                laser->id = laser_manager->__prev_laser_id;
                laser_manager->list_tail->embedded_node.append(laser);
                ++laser_manager->laser_count;
                laser_manager->list_tail = laser;
                laser->initialize(data);
                break;
            }
            case CurvyLaser: {
                LaserCurve* laser = new LaserCurve();
                laser->id = laser_manager->__prev_laser_id;
                laser_manager->list_tail->embedded_node.append(laser);
                ++laser_manager->laser_count;
                laser_manager->list_tail = laser;
                laser->initialize(data);
                break;
            }
        }
        return laser_manager->__prev_laser_id;
    }

    // 0x42D780
    dllexport gnu_noinline LaserData* thiscall get_laser_with_id(int32_t id, int32_t arg2 = 0) {
        LaserManager* laser_manager = LASER_MANAGER_PTR;
        if (id) {
            return laser_manager->list_head().find_if([=](LaserData* laser) {
                return laser->id == id;
            });
        }
        return NULL;
    }

    // 0x442480
    dllexport gnu_noinline void thiscall destroy_all() asm_symbol_rel(0x442480) {
        this->list_head().for_each_safe([](LaserData* laser) {
            laser->__method_18();
            laser->embedded_node.unlink();
            delete laser;
        });
        this->laser_count = 0;
        this->list_tail = &this->dummy_laser;
    }

    // 0x449090
    dllexport gnu_noinline int thiscall cancel_all(int32_t arg1, int32_t arg2 = UNUSED_DWORD) asm_symbol_rel(0x449090) {
        LASER_MANAGER_PTR->list_head().for_each_safe([=](LaserData* laser) {
            if (laser->state != 1) {
                laser->cancel(arg1, 0);
            }
        });
        return 1;
    }

    // 0x448FA0
    dllexport gnu_noinline int thiscall clear_all() asm_symbol_rel(0x448FA0) {
        LASER_MANAGER_PTR->list_head().for_each_safe([](LaserData* laser) {
            laser->__timer_754.reset();
            laser->invulnerable_time = 0;
            laser->cancel(1, 0);
        });
        return 0;
    }

private:
    // 0x448F10
    dllexport gnu_noinline static int vectorcall cancel_in_rectangle(int, int, float, float, float, Float3* arg1, Float3* arg2, float arg3, int arg4 = UNUSED_DWORD, int arg5 = 0) {
        LaserManager* laser_manager = LASER_MANAGER_PTR;
        int ret = 0;
        laser_manager->__float3_7A0 = *arg1;
        laser_manager->__float3_7AC = *arg2;
        laser_manager->list_head().for_each_safe([&](LaserData* laser) {
            if (
                laser->state != 1 &&
                laser->__unknown_flag_A
            ) {
                ret += laser->cancel_as_bomb_rectangle(arg1, arg2, arg3, 0, arg5);
            }
        });
        return ret;
    }
public:
    inline static int cancel_in_rectangle(Float3* arg1, Float3* arg2, float arg3, int arg4 = UNUSED_DWORD, int arg5 = 0) {
        return cancel_in_rectangle(UNUSED_DWORD, UNUSED_DWORD, UNUSED_FLOAT, UNUSED_FLOAT, UNUSED_FLOAT, arg1, arg2, arg3, arg4, arg5);
    }

private:
    // 0x449010
    dllexport gnu_noinline static int vectorcall cancel_in_radius(int, int, float, float, Float3* arg1, float arg2, int arg3, int arg4) {
        LaserManager* laser_manager = LASER_MANAGER_PTR;
        int ret = 0;
        laser_manager->__float3_7A0 = *arg1;
        laser_manager->list_head().for_each_safe([&](LaserData* laser) {
            if (laser->state != 1) {
                ret += laser->cancel_as_bomb_circle(arg1, arg2, arg3, arg4);
            }
        });
        return ret;
    }
public:
    inline static int cancel_in_radius(Float3* arg1, float arg2, int arg3, int arg4) {
        return cancel_in_radius(UNUSED_DWORD, UNUSED_DWORD, UNUSED_FLOAT, UNUSED_FLOAT, arg1, arg2, arg3, arg4);
    }

    // 0x448760
    dllexport gnu_noinline UpdateFuncRet thiscall on_tick() {
        this->list_head().for_each_safe([=](LaserData* laser) {
            if (
                (laser->__unknown_field_A != 0 && ++laser->__unknown_field_A >= 2) ||
                laser->state == 1 ||
                laser->on_tick_common() != 0
            ) {
                laser->__method_18();
                --this->laser_count;
                laser->embedded_node.unlink();
                if (this->list_tail == laser) {
                    this->list_tail = laser->embedded_node.prev;
                }
                delete laser;
            }
        });
        return UpdateFuncNext;
    }

    // 0x448870
    dllexport gnu_noinline static UpdateFuncRet fastcall on_tick(void* ptr) {
        LaserManager* laser_manager = (LaserManager*)ptr;
        if (
            !(GAME_THREAD_PTR->skip_flag | GAME_THREAD_PTR->__unknown_flag_A) &&
            !GAME_THREAD_PTR->__unknown_flag_C
        ) {
            if (GAME_THREAD_PTR->__unknown_flag_B) {
                float prev_game_speed = GAME_SPEED.value;
                GAME_SPEED.value = 0.0f;
                UpdateFuncRet ret = laser_manager->on_tick();
                GAME_SPEED.value = prev_game_speed;
                return ret;
            }
            else {
                if (!ABILITY_SHOP_PTR) {
                    return laser_manager->on_tick();
                }
            }
        }
        return UpdateFuncNext;
    }

    // 0x4488E0
    dllexport gnu_noinline static UpdateFuncRet fastcall on_draw(void* ptr) {
        LaserManager* laser_manager = (LaserManager*)ptr;
        if (!GAME_THREAD_PTR->skip_flag) {
            laser_manager->list_head().for_each_safe([](LaserData* laser) {
                if (laser->state != 1) {
                    laser->on_draw();
                }
            });
        }
        return UpdateFuncNext;
    }

    static inline LaserManager* allocate() {
        LaserManager* laser_manager = new LaserManager();
        LASER_MANAGER_PTR = laser_manager;
        laser_manager->bullet_anm = ANM_MANAGER_PTR->preload_anm(7, "bullet.anm");
        if (!laser_manager->bullet_anm) {
            LOG_BUFFER.write(JpEnStr("", "Enemy bullet data not found. Data is corrupted\r\n"));
            delete laser_manager;
            return NULL;
        }

        UpdateFunc* update_func = new UpdateFunc(&on_tick, false, laser_manager);
        UpdateFuncRegistry::register_on_tick(update_func, 28);
        laser_manager->on_tick_func = update_func;
        update_func = new UpdateFunc(&on_draw, false, laser_manager);
        UpdateFuncRegistry::register_on_draw(update_func, 36);

        laser_manager->list_tail = &laser_manager->dummy_laser;

        return laser_manager;
    }

    template <typename L>
    static inline void for_each_laser_but_stupid(const L& lambda) {
        LaserManager* laser_manager = LASER_MANAGER_PTR;
        LaserData* data = laser_manager->list_head().next;
        laser_manager->__current_stupid_iter = data;
        if (data) {
            do {
                lambda(data);
                laser_manager = LASER_MANAGER_PTR;
                data = laser_manager->__current_stupid_iter;
                if (!data) break;
                data = data->embedded_node.next;
                laser_manager->__current_stupid_iter = data;
            } while (data);
        }
    }

    dllexport static inline void __set_flag_B_on_all_lasers() {
        for_each_laser_but_stupid([](LaserData* laser) {
            laser->__unknown_flag_B = true;
        });
    }

    dllexport static inline void __clear_flag_B_on_all_lasers() {
        for_each_laser_but_stupid([](LaserData* laser) {
            laser->__unknown_flag_B = false;
        });
    }
};

// 0x4490D0
// Method 0xC
dllexport gnu_noinline int thiscall LaserLine::initialize(void* data) {
    memcpy(&this->params, data, sizeof(LaserLineParams));
    this->sprite = this->params.sprite;
    this->color = this->params.color;
    this->state = 2;
    this->type = 0;
    AnmVM* vm = &this->__vm_BE8;
    vm->reset();
    LASER_MANAGER_PTR->bullet_anm->__sub_477D60(vm, BULLET_SPRITE_DATA[this->sprite].color_data[0].sprite_id);
    vm->run_on_interrupt(2);
    vm->run_anm();
    vm->data.blend_mode = 1;
    vm->data.x_anchor_mode = 0;
    vm->data.y_anchor_mode = 2;
    vm->data.type = 1;
    vm->data.origin_mode = 1;

    vm = &this->__vm_11F4;
    clang_forceinline LASER_MANAGER_PTR->bullet_anm->__copy_data_to_vm_and_run(vm, this->params.color + 56);
    vm->run_on_interrupt(2);
    vm->run_anm();
    vm->data.blend_mode = 1;
    vm->data.type = 1;
    vm->data.origin_mode = 1;

    if (this->sprite > 17 && this->sprite != 38) {
        vm = &this->__vm_1800;
        clang_forceinline LASER_MANAGER_PTR->bullet_anm->__copy_data_to_vm_and_run(vm, this->params.color + 83);
    } else {
        vm = &this->__vm_1800;
        clang_forceinline LASER_MANAGER_PTR->bullet_anm->__copy_data_to_vm_and_run(vm, this->params.color + 91);
        vm->data.blend_mode = 1;
    }
    vm->data.origin_mode = 1;
    this->__timer_754.set(30);
    this->offscreen_timer.set(3);
    SoundManager::play_sound_positioned_validate(this->params.shot_sound, 0.0f);
    this->graze_timer.reset();
    this->__timer_40.reset();
    // TODO: FINISH THIS
}

#pragma push_macro("IntArg")
#pragma push_macro("ShortArg")
#pragma push_macro("WordArg")
#pragma push_macro("FloatArg")

#undef IntArg
#undef ShortArg
#undef WordArg
#undef FloatArg
#define IntArg(index) (current_effect.int_values[index])
#define ShortArg(index) (*(int16_t*)&IntArg(index))
#define WordArg(index) (*(uint16_t*)&IntArg(index))
#define FloatArg(index) (current_effect.float_values[index])
#define IntArgEx(index) (((index < 4) + &current_effect)->int_values[(index) % 4])
#define FloatArgEx(index) (((index < 4) + &current_effect)->float_values[(index) % 4])

// 0x4494F0
// Method 0x4
dllexport gnu_noinline void thiscall LaserLine::run_effects() {
    int32_t& effect_index = this->effect_index;
    while (effect_index < countof(this->params.effects)) {
        BulletEffectArgs& current_effect = this->params.effects[effect_index];
        BulletEffectType current_type = current_effect.type;
        if (
            current_type == EX_NONE ||
            !current_effect.async && this->active_effects != EX_NONE
        ) {
            return;
        }
        switch (current_type) {
            case EX_DIST: {
                this->active_effects |= EX_DIST;
                BulletEffectData& effect_data = this->effect_speedup;
                effect_data.timer.reset();
                ++effect_index;
                continue;
            }
            case EX_ACCEL: {
                this->active_effects |= EX_ACCEL;
                BulletEffectData& effect_data = this->effect_accel;
                effect_data.acceleration = FloatArg(0);
                float angle_arg = FloatArg(1);
                if (angle_arg <= -999.0f) {
                    angle_arg = this->angle;
                }
                else if (angle_arg >= 990.0f) {
                    angle_arg = Player::angle_to_player_from_point(&this->position);
                }
                effect_data.angle = angle_arg;
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                effect_data.acceleration_vec.make_from_vector(effect_data.angle, effect_data.acceleration);
                if (this->effect_index != 0) {
                    SoundManager::play_sound_validate(this->params.transform_sound);
                }
                break;
            }
            case EX_ANGLE_ACCEL: {
                this->active_effects |= EX_ANGLE_ACCEL;
                BulletEffectData& effect_data = this->effect_angle_accel;
                effect_data.acceleration = FloatArg(0);
                effect_data.angle = FloatArg(1);
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                if (this->effect_index != 0) {
                    SoundManager::play_sound_validate(this->params.transform_sound);
                }
                break;
            }
            case EX_ANGLE: {
                this->active_effects |= current_type;
                BulletEffectData& effect_data = this->effect_angle;
                effect_data.angle = FloatArg(0);
                float speed_arg = FloatArg(1);
                if (!(speed_arg > -999990.0f)) {
                    speed_arg = this->speed;
                }
                effect_data.speed = speed_arg;
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                effect_data.max_count = IntArg(1);
                effect_data.count = 0;
                effect_data.type = IntArg(2);
                ++effect_index;
                continue;
            }
            case EX_BOUNCE: { // looks more like EX_LOOP...?
                if (IntArg(0) > 0) {
                    this->active_effects |= current_type;
                    BulletEffectData& effect_data = this->effect_bounce;
                    float speed_arg = FloatArg(0);
                    if (!(speed_arg >= 0.0f)) {
                        speed_arg = this->speed;
                    }
                    effect_data.speed = speed_arg;
                    effect_data.max_count = --IntArg(0);
                    effect_data.duration = 0;
                    effect_data.count = IntArg(1);
                    ++effect_index;
                    continue;
                }
                break;
            }
            case EX_SETSPRITE: {
                AnmVM* vm = &this->__vm_BE8;
                int32_t sprite_id = BULLET_SPRITE_DATA[IntArg(0)].color_data[0].sprite_id + IntArg(1);
                clang_forceinline BULLET_MANAGER_PTR->bullet_anm->__copy_data_to_vm_and_run(vm, sprite_id);
                ++effect_index;
                continue;
            }
            case EX_OFFSCREEN: {
                this->offscreen_timer.set(IntArg(0));
                ++effect_index;
                continue;
            }
            case EX_INVULN: {
                this->invulnerable_time = IntArg(0);
                ++effect_index;
                continue;
            }
            case EX_DELETE: {
                this->state = 3;
                ++effect_index;
                continue;
            }
        }
        ++effect_index;
    }
}

// 0x438D40
dllexport AnmLoaded* stdcall anm_file_lookup(int32_t file_index) {
    if (file_index == 7) {
        return STAGE_PTR->stage_anm;
    } else if (file_index == 0) {
        return BULLET_MANAGER_PTR->bullet_anm;
    } else if (file_index == 1) {
        return EFFECT_MANAGER_PTR->effect_anm;
    } else {
        return ENEMY_MANAGER_PTR->enemy_anms[file_index];
    }
}


inline AnmLoaded* EnemyManager::anm_file_lookup(int32_t file_index) {
    return ::anm_file_lookup(file_index);
}

dllexport void Bullet::run_effects() {
    int32_t& effect_index = this->effect_index;
    while (effect_index < countof(this->effects)) {
        BulletEffectArgs& current_effect = this->effects[effect_index];
        BulletEffectType current_type = current_effect.type;
        if (
            current_type == EX_NONE ||
            !current_effect.async && (this->active_effects & EX_OFFSCREEN) != EX_NONE ||
            !(this->active_effects & current_type)
        ) {
            return;
        }

        switch (current_type) {
            case EX_ANIM: {
                int32_t interrupt_index = 7 + ShortArg(0);
                this->vm.run_on_interrupt(interrupt_index);
                this->vm.data.run_interrupt = interrupt_index;
                this->vm.interrupt(7 + ShortArg(0));
                this->state = 2;
                this->position -= this->velocity * 4.0f;
                break;
            }
            case EX_DIST: {
                this->active_effects |= EX_DIST;
                BulletEffectData& effect_data = this->effect_speedup;
                effect_data.timer.reset();
                effect_data.velocity.z = 0.0f;
                break;
            }
            case EX_ACCEL: {
                this->active_effects |= EX_ACCEL;
                BulletEffectData& effect_data = this->effect_accel;
                effect_data.acceleration = FloatArg(0);
                effect_data.angle = this->position.__bullet_effect_angle_jank(this->angle, FloatArg(1), FloatArg(2));
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                effect_data.acceleration_vec.make_from_vector(effect_data.angle, effect_data.acceleration);
                if (this->effect_index != 0) {
                    SoundManager::play_sound_validate(this->transform_sound);
                }
                break;
            }
            case EX_ANGLE_ACCEL: {
                this->active_effects |= EX_ANGLE_ACCEL;
                BulletEffectData& effect_data = this->effect_angle_accel;
                effect_data.acceleration = FloatArg(0);
                effect_data.angle = FloatArg(1);
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                if (this->effect_index != 0) {
                    SoundManager::play_sound_validate(this->transform_sound);
                }
                break;
            }
            case EX_ANGLE: {
                this->active_effects |= current_type;
                BulletEffectData& effect_data = this->effect_angle;
                float speed_arg = FloatArg(1);
                if (!(speed_arg > -999990.0f)) {
                    speed_arg = this->speed;
                }
                effect_data.speed = speed_arg;
                float angle_arg = this->position.__bullet_effect_angle_jank(this->angle, FloatArg(0), FloatArg(2));
                switch (IntArg(2)) {
                    case 2:
                        effect_data.angle = reduce_angle(angle_arg + Player::angle_to_player_from_point(&this->effect_saveangle.position));
                        break;
                    case 3:
                        effect_data.angle = reduce_angle(this->effect_saveangle.angle + angle_arg);
                        break;
                    case 0: case 1: case 4:
                        effect_data.angle = angle_arg;
                        break;
                    case 5: case 6:
                        effect_data.angle = REPLAY_RNG.rand_float_signed_range(FloatArg(0));
                        break;
                    case 7:
                        angle_arg = FloatArg(0);
                        if (angle_arg <= -999990.0f) {
                            angle_arg = this->angle;
                        }
                        if (angle_arg >= 990.0f) {
                            angle_arg = Player::angle_to_player_from_point(&this->position);
                        }
                        effect_data.angle = reduce_angle(angle_arg);
                        effect_data.speed = this->speed + REPLAY_RNG.rand_float_signed_range(FloatArg(1));
                        break;
                }
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                effect_data.max_count = IntArg(1);
                effect_data.count = 0;
                effect_data.type = IntArg(2);
                effect_data.__int_44 = IntArg(3);
                break;
            }
            case EX_BOUNCE: {
                this->active_effects |= current_type;
                BulletEffectData& effect_data = this->effect_bounce;
                effect_data.speed = FloatArg(0);
                if (IntArg(1) & 0x20) {
                    effect_data.size = { FloatArg(1), FloatArg(2) };
                } else {
                    effect_data.size = { 384.0f, 448.0f };
                }
                effect_data.max_count = IntArg(0);
                effect_data.duration = 0;
                effect_data.type = IntArg(1);
                break;
            }
            case EX_INVULN:
                this->invulnerable_time = IntArg(0);
                break;
            case EX_OFFSCREEN: {
                this->active_effects |= EX_OFFSCREEN;
                BulletEffectData& effect_data = this->effect_offscreen;
                effect_data.timer.set(IntArg(0));
                effect_data.__offscreen_unknown = IntArg(1);
                break;
            }
            case EX_PLAYSOUND:
                SoundManager::play_sound_positioned(IntArg(0), this->position.x);
                break;
            case EX_DELETE:
                if (IntArg(0) == true) {
                    this->__cancel_script_id = -1;
                }
                this->cancel(0);
                break;
            case EX_SETSPRITE: {
                this->sprite = IntArg(0);
                this->color = WordArg(1) & INT16_MAX;
                BulletSpriteData& bullet_sprite_data = BULLET_SPRITE_DATA[IntArg(0)];
                this->hitbox_radius = bullet_sprite_data.hitbox_size;
                this->__hitbox_radius_copy = bullet_sprite_data.hitbox_size;
                this->layer = bullet_sprite_data.layer;
                this->vm.reset();
                this->vm.controller.on_sprite_lookup_index = 1;
                this->vm.controller.associated_entity = this;
                BULLET_MANAGER_PTR->bullet_anm->__sub_477D60(&this->vm, BULLET_SPRITE_DATA[IntArg(0)].color_data[0].sprite_id);
                this->vm.data.origin_mode = 1;
                AnmManager::mark_tree_id_for_delete(this->__anm_tree_id);
                this->__anm_tree_id = 0;
                if (int32_t script = BULLET_SPRITE_DATA[IntArg(0)].__anm_script_114) {
                    this->__anm_tree_id = BULLET_MANAGER_PTR->bullet_anm->instantiate_vm_to_world_list_back(script, &this->position);
                }
                switch (BULLET_SPRITE_DATA[this->sprite].__int_10C) {
                    case 0:
                        this->__cancel_script_id = this->color * 2 + 4;
                        break;
                    case 1:
                        this->__cancel_script_id = BULLET_IDK_DATA[this->color];
                        break;
                    case 2:
                        this->__cancel_script_id = -1;
                        this->__unknown_flag_A = true;
                        break;
                    case 3:
                        this->__cancel_script_id = 16;
                        break;
                    case 4:
                        this->__cancel_script_id = 6;
                        break;
                    case 5:
                        this->__cancel_script_id = 12;
                        break;
                    case 6:
                        this->__cancel_script_id = BULLET_SPRITE_DATA[this->sprite].color_data[this->color + 1].sprite_id;
                        this->__unknown_flag_A = true;
                        break;
                    case 7:
                        this->__cancel_script_id = 260;
                        this->__unknown_flag_A = true;
                        break;
                    case 8:
                        this->__cancel_script_id = 263;
                        this->__unknown_flag_A = true;
                        break;
                    case 9:
                        this->__cancel_script_id = 266;
                        this->__unknown_flag_A = true;
                        break;
                    case 10:
                        this->__cancel_script_id = 275;
                        this->__unknown_flag_A = true;
                        break;
                }
                if (IntArg(2) & 0x8000) {
                    this->vm.interrupt(2);
                }
                break;
            }
            case EX_WRAP: {
                this->active_effects |= EX_WRAP;
                BulletEffectData& effect_data = this->effect_wrap;
                effect_data.max_count = IntArg(0);
                effect_data.duration = 0;
                effect_data.flags = IntArg(1);
                break;
            }
            case EX_REACT:
                this->__ex_func_a = IntArg(0);
                ++effect_index;
                continue;
            case EX_SHOOT: {
                ShooterData bullet_shooter(0);
                bullet_shooter.position = this->position;
                bullet_shooter.aim_mode = WordArg(0);
                bullet_shooter.start_transform = IntArg(1);
                bullet_shooter.count1 = WordArg(2);
                bullet_shooter.transform_sound = -1;
                bullet_shooter.count2 = WordArg(3);
                ZUNAngle angle;
                float angle_arg = FloatArg(0);
                if (angle_arg <= -999990.0f) {
                    angle = this->angle;
                }
                else if (999990.0f < angle_arg && angle_arg < 1999990.0f) {
                    // Somehow this can end up only running half of an angle reduce? WTF?
                    angle = Player::angle_to_player_from_point(&this->position);
                }
                else {
                    angle = angle_arg;
                }
                bullet_shooter.angle1 = angle;
                bullet_shooter.angle2 = FloatArg(1);
                float speed_arg = FloatArg(2);
                if (speed_arg <= -999990.0f) {
                    speed_arg = this->speed;
                }
                bullet_shooter.speed1 = speed_arg;
                BOOL cancel_current_bullet = IntArgEx(6);
                bullet_shooter.type = IntArgEx(4);
                bullet_shooter.color = IntArgEx(5);
                bullet_shooter.speed2 = FloatArg(3);
                bullet_shooter.flags = 0;
                memcpy(bullet_shooter.effects, this->effects, sizeof(bullet_shooter.effects));
                BULLET_MANAGER_PTR->shoot_bullets(&bullet_shooter);
                ++effect_index;
                if (cancel_current_bullet) {
                    this->cancel(0);
                }
                continue;
            }
            case EX_LOOP: {
                int32_t new_loop_index = IntArg(1);
                int32_t current_loop_index;
                if (new_loop_index <= 0) goto default_loop;
                current_loop_index = this->effect_loop_index;
                switch (current_loop_index) {
                    case 0:
                        this->effect_loop_index = new_loop_index;
                        effect_index = IntArg(0);
                        continue;
                    case 1:
                        this->effect_loop_index = 0;
                        break;
                    default:
                        this->effect_loop_index = --current_loop_index;
                    default_loop:
                        effect_index = IntArg(0);
                        continue;
                }
                break;
            }
            case EX_VELADD: {
                this->active_effects |= EX_VELADD;
                BulletEffectData& effect_data = this->effect_veladd;
                effect_data.velocity.make_from_vector(FloatArg(0), FloatArg(1));
                effect_data.velocity.z = 0.0f;
                effect_data.angle = FloatArg(0);
                effect_data.speed = FloatArg(1);
                effect_data.duration = IntArg(0);
                effect_data.timer.reset();
                break;
            }
            case EX_VEL: {
                float angle_arg = FloatArg(0);
                if (angle_arg >= 990.0f) {
                    angle_arg -= 999.0f;
                    angle_arg += Player::angle_to_player_from_point(&this->position);
                    this->angle = reduce_angle(angle_arg);
                }
                else if (angle_arg >= -990.0f) {
                    this->angle = angle_arg;
                }
                float speed_arg = FloatArg(1);
                if (speed_arg >= -990.0) {
                    this->speed = speed_arg;
                } else {
                    speed_arg = this->speed;
                }
                this->velocity.make_from_vector(this->angle, speed_arg);
                break;
            }
            case EX_MOVE: {
                this->active_effects |= EX_MOVE;
                BulletEffectData& effect_data = this->effect_move;
                effect_data.target = { FloatArg(0), FloatArg(1) };
                if (IntArg(1) & 0x100) {
                    effect_data.target += this->position;
                }
                effect_data.speed = this->speed;
                effect_data.target.z = 0.0f;
                effect_data.duration = IntArg(0);
                effect_data.mode = IntArg(1);
                effect_data.timer.reset();
                this->effect_move_interp.initial_value = this->position;
                this->effect_move_interp.final_value = effect_data.target;
                this->effect_move_interp.bezier1 = UNKNOWN_FLOAT3_B;
                this->effect_move_interp.bezier2 = UNKNOWN_FLOAT3_B;
                this->effect_move_interp.end_time = IntArg(0);
                this->effect_move_interp.mode = IntArg(1);
                this->effect_move_interp.time.reset();
                break;
            }
            case EX_BRIGHT:
                switch (IntArg(0)) {
                    case 2:
                        this->vm.data.blend_mode = 2;
                        break;
                    case 1:
                        this->vm.data.blend_mode = 1;
                        break;
                    default:
                        this->vm.data.blend_mode = 0;
                        break;
                }
                break;
            case EX_SAVEANGLE:
                this->effect_saveangle.position = this->position;
                //this->effect_saveangle.position.copy(this->position);
                this->effect_saveangle.angle = this->angle;
                this->effect_saveangle.speed = this->speed;
                ++effect_index;
                continue;
            case EX_SIZE: {
                this->active_effects |= EX_SIZE;
                this->scale_interp.initial_value = FloatArg(0);
                this->scale_interp.final_value = FloatArg(1);
                this->scale_interp.bezier1 = 0.0f;
                this->scale_interp.bezier2 = 0.0f;
                this->scale_interp.end_time = IntArg(0);
                this->scale_interp.mode = IntArg(1);
                this->scale_interp.time.reset();
                this->__unknown_flag_B = true;
                break;
            }
            case EX_VELTIME: {
                this->active_effects |= EX_VELTIME;
                BulletEffectData& effect_data = this->effect_veltime;
                effect_data.speed = (FloatArg(0) - this->speed) / IntArg(0);
                effect_data.angle = this->position.__bullet_effect_angle_jank(this->angle, FloatArg(1), FloatArg(2));
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                effect_data.velocity.make_from_vector(effect_data.angle, effect_data.speed);
                if (this->effect_index != 0) {
                    SoundManager::play_sound_validate(this->transform_sound);
                }
                break;
            }
            case EX_ENEMY: {
                EnemyInitData enemy_data;
                enemy_data.position = this->position;
                enemy_data.int_vars[0] = IntArg(0); // I0 = etEx a
                enemy_data.int_vars[1] = IntArg(1); // I1 = etEx b
                enemy_data.int_vars[2] = IntArg(2); // I2 = etEx c
                enemy_data.int_vars[3] = IntArg(3); // I3 = etEx d
                enemy_data.mirrored = false;
                enemy_data.__basic_anm_update = false;
                enemy_data.float_vars[4] = 0.0f;
                enemy_data.float_vars[5] = 0.0f;
                enemy_data.float_vars[6] = 0.0f;
                enemy_data.float_vars[7] = 0.0f;
                enemy_data.parent_id = 0;
                enemy_data.life = 10000;
                enemy_data.score = 0;
                enemy_data.item_drop = 0;
                enemy_data.float_vars[0] = FloatArg(0); // F0 = etEx r
                enemy_data.float_vars[1] = FloatArg(1); // F1 = etEx s
                enemy_data.float_vars[2] = FloatArg(2); // F2 = etEx m
                enemy_data.float_vars[3] = FloatArg(3); // F3 = etEx n
                ENEMY_MANAGER_PTR->allocate_new_enemy(current_effect.string, &enemy_data, UNUSED_DWORD);
                break;
            }
            case EX_LASER: {
                switch (IntArg(0)) {
                    case 1: {
                        LaserInfiniteParams laser_params(0); // 0xA8
                        laser_params.__speed_1 = 8.0f;
                        laser_params.velocity.x = 0.0f;
                        laser_params.velocity.y = 0.0f;
                        laser_params.velocity.z = 0.0f;
                        laser_params.__angle_18 = 0.0f;
                        laser_params.angular_velocity = 0.0f;
                        laser_params.length = 0.0f;
                        laser_params.__float_24 = 0.0f;
                        laser_params.width = 0.0f;
                        laser_params.start_time = 0;
                        laser_params.expand_time = 0;
                        laser_params.duration = 0;
                        laser_params.stop_time = 0;
                        laser_params.shot_sound = 0;
                        laser_params.transform_sound = 0;
                        laser_params.laser_id = 0;
                        laser_params.distance = 0.0f;
                        __builtin_memcpy(laser_params.effects, this->effects, sizeof(laser_params.effects));
                        int32_t flags = IntArg(3);
                        laser_params.position = this->position;
                        laser_params.flags = (flags & 0xFD) | 0x2;
                        laser_params.sprite = IntArg(1);
                        laser_params.color = IntArg(2);
                        laser_params.__start_ex = (uint8_t)(flags >> 8);
                        ZUNAngle angle;
                        float angle_arg = FloatArg(0);
                        if (angle_arg <= -999990.0f) {
                            angle = this->angle;
                        } else if (999990.0f < angle_arg && angle_arg < 1999990.0f) {
                            // Somehow this can end up only running half of an angle reduce? WTF?
                            clang_forceinline angle = Player::angle_to_player_from_point(&this->position);
                        } else {
                            angle = angle_arg;
                        }
                        laser_params.__angle_18 = angle;
                        float speed_arg = FloatArg(1);
                        if (speed_arg <= -999990.0f) {
                            speed_arg = this->speed;
                        }
                        ++effect_index;
                        laser_params.__float_24 = FloatArg(2);
                        laser_params.start_time = IntArgEx(4);
                        laser_params.expand_time = IntArgEx(5);
                        laser_params.duration = IntArgEx(6);
                        laser_params.length = FloatArg(3);
                        laser_params.stop_time = IntArgEx(7);
                        laser_params.width = FloatArgEx(4);
                        laser_params.shot_sound = 18;
                        laser_params.transform_sound = -1;
                        laser_params.distance = FloatArgEx(5);
                        LASER_MANAGER_PTR->allocate_new_laser(1, &laser_params);
                        ++effect_index;
                        if (flags & 0x10000) {
                            this->cancel(0);
                        }
                        continue;
                    }
                    case 0: {
                        LaserLineParams laser_params(0); // 0x528
                        laser_params.flags = 0;
                        laser_params.__angle_C = 0.0f;
                        laser_params.__length_related = 0.0f;
                        laser_params.length = 0.0f;
                        laser_params.__float_18 = 0.0f;
                        laser_params.width = 0.0f;
                        laser_params.__speed_1 = 0.0f;
                        laser_params.distance = 0.0f;
                        laser_params.effect_index = 0;
                        laser_params.shot_sound = 0;
                        laser_params.transform_sound = 0;
                        __builtin_memcpy(laser_params.effects, this->effects, sizeof(laser_params.effects));
                        BOOL cancel_current_bullet = IntArg(3);
                        laser_params.position = this->position;
                        laser_params.sprite = IntArg(1);
                        laser_params.color = IntArg(2);
                        ZUNAngle angle;
                        float angle_arg = FloatArg(0);
                        if (angle_arg <= -999990.0f) {
                            angle = this->angle;
                        } else if (999990.0f < angle_arg && angle_arg < 1999990.0f) {
                            // Somehow this can end up only running half of an angle reduce? WTF?
                            clang_forceinline angle = Player::angle_to_player_from_point(&this->position);
                        } else {
                            angle = angle_arg;
                        }
                        laser_params.__angle_C = angle;
                        float speed_arg = FloatArg(1);
                        if (speed_arg <= -999990.0f) {
                            speed_arg = this->speed;
                        }
                        ++effect_index;
                        laser_params.__unknown_flag_A = true;
                        laser_params.__speed_1 = speed_arg;
                        laser_params.length = FloatArg(2);
                        laser_params.__length_related = FloatArg(3);
                        laser_params.__float_18 = FloatArgEx(4);
                        laser_params.shot_sound = IntArgEx(4);
                        laser_params.width = FloatArgEx(5);
                        laser_params.transform_sound = IntArgEx(5);
                        laser_params.distance = FloatArgEx(6);
                        laser_params.effect_index = IntArgEx(6);
                        LASER_MANAGER_PTR->allocate_new_laser(0, &laser_params);
                        ++effect_index;
                        if (cancel_current_bullet) {
                            this->cancel(0);
                        }
                        continue;
                    }
                }
                continue;
            }
            case EX_DELAY:
                if (IntArg(0) <= 0) {
                    ++effect_index;
                    continue;
                }
                this->active_effects |= EX_DELAY;
                this->effect_delay.timer.set(IntArg(0));
                break;
            case EX_LAYER:
                this->layer = IntArg(0);
                ++effect_index;
                continue;
            case EX_HITBOX: {
                float hitbox_size = FloatArg(0);
                if (0.0f > hitbox_size) {
                    hitbox_size = BULLET_SPRITE_DATA[this->sprite].hitbox_size;
                }
                this->__hitbox_radius_copy = hitbox_size;
                this->hitbox_radius = hitbox_size;
                break;
            }
            case EX_WAIT:
                if (IntArg(0) <= 0) {
                    ++effect_index;
                    continue;
                }
                this->active_effects |= EX_WAIT;
                this->effect_wait.timer.set(IntArg(0));
                break;
            case EX_HOMING: {
                this->active_effects |= EX_HOMING;
                BulletEffectData& effect_data = this->effect_homing;
                effect_data.speed = FloatArg(0);
                effect_data.angle = FloatArg(1);
                effect_data.target.x = FloatArg(2);
                effect_data.timer.reset();
                effect_data.duration = IntArg(0);
                break;
            }
            default:
                break;
        }
        ++effect_index;
    }
}

#pragma pop_macro("IntArg")
#pragma pop_macro("ShortArg")
#pragma pop_macro("WordArg")
#pragma pop_macro("FloatArg")

forceinline ZUNResult thiscall EnemyData::update() {
    if (ZUN_FAILED(this->__move())) {
        return ZUN_ERROR;
    }
    if (ZUN_FAILED(this->vm->run_ecl(this->ecl_time.get_scale_unsafe()))) {
        return ZUN_ERROR;
    }
    if (auto func_set_func = this->func_set_func) {
        if (ZUN_FAILED(func_set_func(this))) {
            return ZUN_ERROR;
        }
    }
    return ZUN_SUCCESS;
}

inline const char* Enemy::check_timer_callbacks() {
    for (size_t i = 0; i < MAX_CALLBACKS; ++i) {
        if (this->data.callbacks[i].life >= 0) {
            int32_t time_threshold = this->data.callbacks[i].time;
            if (time_threshold > 0) {
                if (this->data.is_boss) {
                    GUI_PTR->update_spell_timer(time_threshold - this->data.ecl_time);
                }
                if (this->data.ecl_time >= this->data.callbacks[i].time) {
                    this->data.life.current = this->data.callbacks[i].life;
                    this->data.callbacks[i].life = -1;
                    this->data.ecl_time.reset();
                    this->data.__unknown_flag_L = true;

                    Spellcard* spellcard = SPELLCARD_PTR;
                    if (!spellcard->__timeout_spell) {
                        spellcard->__sub_409AD0();
                        ENEMY_MANAGER_PTR->can_capture_spell = false;
                    }
                    else if (spellcard->__inline_sub_B()) {
                        this->data.__unknown_flag_L = false;
                        GAME_MANAGER.globals.__int_90 += this->data.chapter_spawn_weight;
                    }
                    this->data.chapter_spawn_weight = 0;

                    return this->data.callbacks[i].time_sub;
                }
                break;
            }
        }
    }
    return NULL;
}

inline const char* Enemy::check_life_callbacks() {
    int32_t life = this->data.life.current;
    for (size_t i = 0; i < MAX_CALLBACKS; ++i) {
        int32_t life_threshold = this->data.callbacks[i].life;
        if (life_threshold >= 0) {
            this->data.life.remaining_current_attack = life - life_threshold;
            this->data.life.starting_value_for_next_attack = this->data.callbacks[i].life;
            life_threshold = this->data.callbacks[i].life;
            if (life <= life_threshold) {
                this->data.add_spawn_weight_to_chapter_destroy();
                this->data.callbacks[i].life = -1;
                this->data.ecl_time.reset();
                this->data.__unknown_flag_L = false;
                return this->data.callbacks[i].life_sub;
            }
            break;
        }
    }
    return NULL;
}

// 0x42ED40
dllexport gnu_noinline ZUNResult thiscall EnemyData::__update_state() {
    BombBase* bomb_ptr = BOMB_PTR;
    if (this->bomb_shield) {
        if (bomb_ptr->bomb_active && !this->bomb_shield_active) {
            this->anm_vms[0].__sub_488FD0(this->current_anm_script = this->bombshield_on_anm);
            this->disable_hitbox = true;
            this->bomb_shield_active = true;
        } else if (this->bomb_shield_active) {
            this->anm_vms[0].__sub_488FD0(this->current_anm_script = this->bombshield_off_anm);
            this->disable_hitbox = false;
            this->bomb_shield_active = false;
        }
    } else if (this->bomb_shield_active) {
        this->anm_vms[0].__sub_488FD0(this->current_anm_script = this->bombshield_off_anm);
        this->disable_hitbox = false;
        this->bomb_shield_active = false;
    }
    if (this->delete_as_bullet) {
        Float3* damage_pos = &this->position_of_last_damage_source_to_hit;
        int32_t A = 0;
        Float2* hitbox_size = &this->hitbox_size;
        Float3* position = &this->current_motion.position;
        EnemyID id = this->enemy()->id;
        float radius;
        float rotation;
        if (!this->rectangular_hitbox) {
            rotation = 0.0f;
            radius = this->hitbox_size.x * 0.5f;
        } else {
            rotation = this->hitbox_rotation;
            radius = 0.0f;
        }
        if (
            HitboxManager::enm_compute_damage_sources(position, hitbox_size, rotation, radius, &A, damage_pos, true, id) != 0 &&
            A != 0
            ) {
            if (this->enemy()->kill()) {
                return ZUN_SUCCESS2; // triggers the fail condition
            }
        }
    }
    if (const char* timer_callback_to_run = this->enemy()->check_timer_callbacks()) {
        this->ecl_time.reset();
        this->reinitialize_vm_with_sub(timer_callback_to_run);
        if (ZUN_FAILED(this->run_ecl())) {
            return ZUN_ERROR;
        }
    }

    int32_t B = 0;
    this->__damaged_this_frame = false;
    if (this->has_active_hitbox()) {
        float radius = this->hitbox_size.x;
        Float2* hitbox_size = &this->hitbox_size;
        int32_t damage = 0;
        if (radius > 0.0f) {
            Float3* damage_pos = &this->position_of_last_damage_source_to_hit;
            Float3* position = &this->current_motion.position;
            EnemyID id = this->enemy()->id;
            float rotation;
            if (!this->rectangular_hitbox) {
                rotation = 0.0f;
                radius = this->hitbox_size.x * 0.5f;
            } else {
                rotation = this->hitbox_rotation;
                radius = 0.0f;
            }
            damage = HitboxManager::enm_compute_damage_sources(position, hitbox_size, rotation, radius, &B, damage_pos, false, id);
            damage *= PLAYER_PTR->damage_multiplier;
        }
        if (auto* extra_damage_func = this->extra_damage_func) {
            damage += extra_damage_func(this, damage);
        }
        int32_t more_damage = this->__int_50D4;
        if (more_damage > 0) {
            damage += more_damage;
        }
        switch (PLAYER_PTR->data.state) {
            case 0: case 2:
                damage /= 5;
        }
        if (GUI_PTR->msg_vm_active()) {
            damage = 0;
        }
        if (damage > 0) {
            if (B) {
                int32_t life = this->life.current;
                if (damage >= life) {
                    ENEMY_MANAGER_PTR->__int_AC += (damage - life) / 4 + life;
                }
                else {
                    ENEMY_MANAGER_PTR->__int_AC += damage;
                }
            }
            else {
                ENEMY_MANAGER_PTR->__int_B0 += damage;
            }
        }
        if (BOMB_PTR->bomb_active == TRUE) {
            float bomb_multiplier = this->bomb_damage_multiplier;
            if (bomb_multiplier < 1.0f) {
                if (damage && bomb_multiplier <= 0.0f) {
                    SOUND_MANAGER.play_sound_positioned(36, this->current_motion.position.x);
                }
                damage *= bomb_multiplier;
            }
        }
        if (damage) {
            ++UNKNOWN_COUNTER_A;
            if (SPELLCARD_PTR->__inline_sub_C()) {
                damage /= 60;
            }
            this->life.take_damage(damage, this->is_invulnerable());
            this->__timer_5118.set(30);

            if (const char* life_callback_to_run = this->enemy()->check_life_callbacks()) {
                this->ecl_time.set(0);
                this->reinitialize_vm_with_sub(life_callback_to_run);
                if (ZUN_FAILED(this->run_ecl())) {
                    return ZUN_ERROR;
                }
            }

            if (this->life.current <= 0 & ~this->no_delete) {
                if (this->enemy()->kill()) {
                    return ZUN_SUCCESS2; // triggers the fail condition
                }
            }
            this->__damaged_this_frame = true;
        }
    }

    if (
        this->life.__unknown_flag_A &&
        this->enemy()->kill()
    ) {
        return ZUN_SUCCESS2; // triggers the fail condition
    }

    if (
        this->has_active_collision() &&
        this->__basic_anm_update &&
        (!ENEMY_MANAGER_PTR->__int_164 || this->is_boss)
    ) {
        if (auto* extra_hitbox_func = this->extra_hitbox_func) {
            extra_hitbox_func(this);
        }
        else {
            CollisionResult result;
            if (!this->rectangular_hitbox) {
                result = PLAYER_PTR->check_collision_circle(&this->current_motion.position, this->collision_size.x, FALSE);
            }
            else {
                AnmVM* main_vm = this->anm_vms[0].get_vm_ptr();
                float X = 0.0f;
                float Y = this->collision_size.y * 0.5f;
                float Z = 0.0f;
                if (main_vm) {
                    float angle = reduce_angle<NoInline>(main_vm->data.rotation.z + HALF_PI_f);
                    // This looks a lot like rotate_around_origin,
                    // but the register usage doesn't look like
                    // it was using actual Float2/Float3
                    float y_unit = zsinf(angle);
                    float x_unit = zcosf(angle);
                    X = (x_unit * X) - (y_unit * Y);
                    Y = (x_unit * Y) + (y_unit * X);
                }
                float rotation = this->hitbox_rotation;
                float width = this->collision_size.y;
                Float3 position;
                position.x = this->current_motion.position.x + X;
                position.y = this->current_motion.position.y + Y;
                position.z = this->current_motion.position.z + Z;
                float length = this->collision_size.x;
                result = PLAYER_PTR->__check_collision_rotated_rectangle(&position, rotation, width, length, FALSE);
            }
            if (
                this->grazable &&
                result == GrazeCollision &&
                this->ecl_time.is_multiple_of(6)
            ) {
                PLAYER_PTR->do_graze(&PLAYER_PTR->data.position);
            }
        }
    }

    if (AnmVM* main_vm = this->anm_vms[0].get_vm_ptr()) {
        if (!this->__int_50E4) {
            if (this->__anm_related_flag_A) {
                if (this->ecl_time.is_multiple_of(4)) {
                    main_vm->data.color2 = PackD3DCOLOR(255, 255, 0, 255);
                    main_vm->data.color_mode = 1;
                }
                else {
                    main_vm->data.color_mode = 0;
                }
            }
            if (
                this->__damaged_this_frame &&
                !this->slowdown_immune // ???
            ) {
                main_vm->data.color2 = PackD3DCOLOR(255, 0, 0, 255);
                main_vm->data.color_mode = 1;
                int32_t hit_sound = this->hit_sound;
                this->__int_50E4 = 4;
                if (hit_sound < 0) {
                    int32_t health_remaining;
                    if (
                        !(this->__unknown_flag_O | this->is_boss) || // make this another helper function
                        !SPELLCARD_PTR->__enemy_is_low_health(this, 200, 900)
                    ) {
                        SOUND_MANAGER.play_sound_positioned(34, this->current_motion.position.x);
                    }
                    else {
                        SOUND_MANAGER.play_sound_positioned(35, this->current_motion.position.x);
                    }
                }
                else {
                    SOUND_MANAGER.play_sound_positioned(hit_sound, this->current_motion.position.x);
                }
            }
            else {
                if (this->ecl_time.is_multiple_of(4)) {
                    if (
                        (this->__unknown_flag_O | this->is_boss) &&
                        SPELLCARD_PTR->__enemy_is_low_health(this, 100, 500)
                    ) {
                        main_vm->data.color2 = PackD3DCOLOR(255, 0, 0, 255);
                        main_vm->data.color_mode = 1;
                    }
                }
                else {
                    main_vm->data.color_mode = 0;
                }
            }
        }
        else {
            main_vm->data.color_mode = 0;
            --this->__int_50E4;
        }
    }

    if (this->__timer_5118 > 0) {
        this->__timer_5118--;
    }
    return ZUN_SUCCESS;
}

// 0x42FF80
dllexport gnu_noinline ZUNResult thiscall EnemyData::on_tick() {
    if (!this->__unknown_flag_A) {
        this->__unknown_flag_A = true;
        if (ZUN_FAILED(this->update())) {
            return ZUN_ERROR;
        }
        if (ZUN_FAILED(this->__update_state())) {
            return ZUN_ERROR;
        }
        this->__update_fog();
        if (!this->__basic_anm_update) {
            for (size_t i = 0; i < 16; ++i) {
                if (AnmVM* vm = this->anm_vms[i].get_vm_ptr()) {
                    Float3 new_position = this->current_motion.position + this->anm_positions[i];
                    int32_t vm_index = this->anm_vm_indices[i];
                    if (vm_index >= 0) {
                        if (AnmVM* vm2 = this->anm_vms[vm_index].get_vm_ptr()) {
                            new_position += vm2->data.position;
                        }
                    }
                    vm->set_controller_position(&new_position);
                    if (vm->data.auto_rotate) {
                        float movement_angle = this->current_motion.axis_velocity.direction();
                        clang_forceinline vm->set_z_rotation(movement_angle);
                        this->hitbox_rotation = movement_angle;
                    }
                }
            }
        } else {
            for (size_t i = 0; i < 16; ++i) {
                this->anm_vms[i].set_controller_position(&this->current_motion.position);
            }
        }
        this->drops.limited_timer++;
        if (this->invulnerable_timer > 0) {
            this->invulnerable_timer--;
        }
        if (this->no_collision_timer > 0) {
            this->no_collision_timer--;
        }
        this->boss_timer++;
        this->ecl_time++;
    }
    return ZUN_SUCCESS;
}

// 0x4394A0
dllexport gnu_noinline int32_t fastcall EnemyData::extra_damage_func2(EnemyData* enemy_data, int32_t prev_damage) {
    Float3 position = enemy_data->current_motion.position;
    int32_t dummy = 0; // 
    int32_t new_damageA = 0; // EBX
    int32_t new_damageB = 0; // EDI
    if (AnmVM* anm_vm = enemy_data->anm_vms[1].get_vm_ptr()) {
        position.y += 24.0f;
        Float2 hurtbox = {
            anm_vm->data.scale.x * 192.0f,
            anm_vm->data.scale.y * 32.0f
        };
        new_damageB = HitboxManager::enm_compute_damage_sources(&position, &hurtbox, anm_vm->data.rotation.z, 0.0f, &dummy, NULL, false, enemy_data->enemy()->id);
        position.y += 32.0f;
        new_damageA = HitboxManager::enm_compute_damage_sources(&position, NULL, 0.0f, 48.0f, &dummy, NULL, false, enemy_data->enemy()->id);
    }
    return new_damageA + new_damageB + prev_damage;
}

inline void EclContext::step_float_interps() {
    nounroll for (size_t i = 0; i < countof(this->float_interps); ++i) {
        if (this->float_interps[i].end_time) {
            EclInstruction* current_instruction = this->get_instruction(this->float_interp_locations[i].sub_index, this->float_interp_locations[i].instruction_offset);
            float* value_write;
            if (current_instruction->param_mask & 2) {
                float value = FloatArg(1);
                int32_t value_as_int = value;
                if (value >= 0.0f) {
                    value_write = &this->stack.ref_offset<float>(this->float_interp_stack_offsets[i]);
                } else {
                    value_write = this->vm->get_float_ptr(value_as_int);
                }
            } else {
                value_write = NULL;
            }
            *value_write = this->float_interps[i].step();
        }
    }
}

// 0x4369E0
dllexport gnu_noinline int32_t thiscall EnemyData::ecl_enm_create() {
    using namespace Ecl;

    if (ENEMY_MANAGER_PTR->is_below_enemy_limit()) {
        EclInstruction* current_instruction = this->vm->current_context->get_current_instruction();
        int32_t sub_name_length = IntArg(0) + sizeof(IntArg(0));
        int32_t enm_args_offset = sub_name_length / sizeof(DWORD);

        EnemyInitData init_data;
        init_data.position = {};
        init_data.mirrored = false;
        init_data.__basic_anm_update = false;
        
        init_data.position.x = this->parse_float_as_arg(1, FloatArg(enm_args_offset));
        init_data.position.y = this->parse_float_as_arg(2, FloatArg(enm_args_offset + 1));

        // This section looks *really* weird...
        uint32_t opcode = current_instruction->opcode;
        if (
            opcode == enemy_create_rel ||
            opcode == enemy_create_rel_stage ||
            opcode == __enemy_create_rel_2 ||
            opcode == enemy_create_rel_stage_mirror ||
            opcode == enemy_create_rel_mirror
        ) {
            init_data.position.x += this->current_motion.position.x;
            init_data.position.y += this->current_motion.position.y;
        }
        if (
            opcode == enemy_create_rel_stage_mirror ||
            opcode == enemy_create_abs_stage_mirror ||
            opcode == enemy_create_rel_mirror ||
            opcode == enemy_create_abs_mirror
        ) {
            init_data.mirrored = true;
        }
        if (this->get_mirror_flag()) {
            init_data.position.x *= -1.0f;
            init_data.mirrored ^= 1;
        }

        init_data.life = this->parse_int_as_arg(3, IntArg(enm_args_offset + 2));
        init_data.score = this->parse_int_as_arg(4, IntArg(enm_args_offset + 3));
        init_data.item_drop = this->parse_int_as_arg(5, IntArg(enm_args_offset + 4));
        __builtin_memcpy(init_data.int_vars, this->int_vars, sizeof(this->int_vars));
        __builtin_memcpy(init_data.float_vars, this->float_vars, sizeof(this->float_vars));
        init_data.parent_id = this->enemy()->id;
        ENEMY_MANAGER_PTR->allocate_new_enemy(StringArg(4), &init_data);
    }
    return 0;
}

// 0x436BB0
dllexport gnu_noinline void thiscall EnemyData::anm_set_slot_impl() {
    int32_t slot = this->get_int_arg(0);
    int32_t sprite = this->get_int_arg(1);
    AnmID& id = this->anm_vms[slot];
    id.mark_tree_for_delete();
    if (sprite >= 0) {
        sprite = this->get_int_arg(1);
        id = ENEMY_MANAGER_PTR->anm_file_lookup(this->anm_source_index)->instantiate_vm_to_world_list_front(sprite, this->anm_base_layer + 7);
        if (slot == 0) {
            this->anm_slot_0_script = this->get_int_arg(1);
            this->anm_slot_0_source_index = this->anm_source_index;
        }
        AnmVM* vm = id.get_vm_ptr();
        if (slot == 0) {
            this->final_sprite_size.x = vm->get_scaled_sprite_y_size();
            this->final_sprite_size.y = vm->get_scaled_sprite_x_size();
        }
        if (this->intangible) {
            id.__unknown_tree_clear_J();
        }
    }
}

inline void thiscall EnemyData::anm_set_slot_main_impl() {
    int32_t slot = this->get_int_arg(0);
    int32_t sprite = this->get_int_arg(1);
    AnmID& id = this->anm_vms[slot];
    id.mark_tree_for_delete();
    id = ENEMY_MANAGER_PTR->anm_file_lookup(this->anm_source_index)->instantiate_vm_to_world_list_front(sprite, this->anm_base_layer + 7);
    AnmVM* vm = id.get_vm_ptr();
    if (slot == 0) {
        this->final_sprite_size.x = vm->get_scaled_sprite_y_size();
        this->final_sprite_size.y = vm->get_scaled_sprite_x_size();
    }
    if (this->intangible) {
        id.__unknown_tree_clear_J();
    }
    if (slot == 0) {
        this->__unknown_flag_I = true;
        this->current_anm_script = sprite;
        this->anm_slot_0_script = sprite;
        this->current_anm_pose = 0;
        this->anm_slot_0_source_index = this->anm_source_index;
    }
}

inline void thiscall EnemyData::anm_play_main_impl() {
    this->anm_vms[0].mark_tree_for_delete();
    this->anm_vms[0] = ENEMY_MANAGER_PTR->anm_file_lookup(this->anm_source_index)->instantiate_vm_to_world_list_front(this->current_anm_script, this->anm_base_layer + 7);
    this->__unknown_flag_I = false;
    this->anm_slot_0_script = this->current_anm_script;
    this->current_anm_pose = 0;
    this->anm_slot_0_source_index = this->anm_source_index;
}

inline void thiscall EnemyData::anm_play_attack_impl() {
    int32_t slot = this->get_int_arg(0);
    this->anm_vms[slot].mark_tree_for_delete();
    this->anm_vms[slot] = ENEMY_MANAGER_PTR->anm_file_lookup(this->anm_source_index)->instantiate_vm_to_world_list_front(this->current_anm_script + 5, this->anm_base_layer + 7);
    AnmVM* vm = this->anm_vms[slot].get_vm_ptr();
    if (slot == 0) {
        this->final_sprite_size.x = vm->get_scaled_sprite_y_size();
        this->final_sprite_size.y = vm->get_scaled_sprite_x_size();
    }
    if (this->intangible) {
        this->anm_vms[slot].__unknown_tree_clear_J();
    }
}

inline void thiscall EnemyData::anm_play_attack_ex_impl() {
    int32_t slot = this->get_int_arg(0);
    int32_t attack_index = this->get_int_arg(1);
    AnmID& id = this->anm_vms[slot];
    id.mark_tree_for_delete();
    int32_t sprite = this->current_anm_script;
    if (attack_index >= 0) {
        sprite += attack_index + 5;
    }
    id = ENEMY_MANAGER_PTR->anm_file_lookup(this->anm_source_index)->instantiate_vm_to_world_list_front(sprite, this->anm_base_layer + 7);
    AnmVM* vm = this->anm_vms[slot].get_vm_ptr();
    if (slot == 0) {
        this->final_sprite_size.x = vm->get_scaled_sprite_y_size();
        this->final_sprite_size.y = vm->get_scaled_sprite_x_size();
    }
    if (this->intangible) {
        this->anm_vms[slot].__unknown_tree_clear_J();
    }
}

template <bool sanity_check_script>
inline float EnemyData::kill_effects() {
    SOUND_MANAGER.play_sound_positioned_validate(this->death_sound, this->current_motion.position.x);

    float angle = -HALF_PI_f;

    static constexpr float A = 1.0f / 25.0f;
    if (!(this->position_of_last_damage_source_to_hit.distance_squared(&this->current_motion.position) < A)) {
        angle = this->current_motion.position.angle_to(&this->position_of_last_damage_source_to_hit);
    }

    int32_t death_script = this->death_anm_script;
    if (sanity_check_script && death_script >= 0) {
        AnmID death_anm = ENEMY_MANAGER_PTR->anm_file_lookup(this->death_anm_index)->instantiate_vm_to_world_list_back(death_script, &this->current_motion.position, angle, 3);
        EFFECT_MANAGER_PTR->fill_available_slot(death_anm);
    }

    return angle;
}

// 0x48B030
dllexport gnu_noinline ZUNResult thiscall EclContext::call(EclContext* new_context, int32_t va_index, int32_t) {
    EclInstruction* current_instruction = this->get_current_instruction(); // EBP-8 (LOCAL.2)
    int32_t va_offset = IntArg(0) + sizeof(int32_t[va_index]); // EBP-10 (LOCAL.4)
    int32_t stack_pointer = new_context->stack.pointer; // EBP-14 (LOCAL.5)
    int32_t new_stack_pointer; // EBP-C (LOCAL.3)
    if (!stack_pointer) {
        new_stack_pointer = sizeof(int32_t[5]);
        new_context->stack.push(0);
    } else {
        new_stack_pointer = stack_pointer + sizeof(int32_t[4]);
    }
    if (++va_index < current_instruction->param_count) {
        uint8_t* stack_write_ptr = &new_context->stack.raw[new_stack_pointer];
        const EclRawValue* call_args_ptr = based_pointer<EclRawValue>(StringArg(4), va_offset);
        // Yes, all this weird pointer arithmetic seems to be in the original code.
        // There's too much BS for it to be simple.
        do {
            switch (call_args_ptr->type) {
                default:
                {
                    int32_t value = this->parse_int_as_arg_pop(va_index, IntArg((&call_args_ptr->integer - &IntArg(0)) + 1));
                    if (call_args_ptr->type2 == 'f') {
                        *(float*)stack_write_ptr = value;
                    } else {
                        *(int32_t*)stack_write_ptr = value;
                    }
                    break;
                }
                case 'f': case 'g':
                {
                    float value = this->parse_float_as_arg_pop(va_index, FloatArg((&call_args_ptr->real - &FloatArg(0)) + 1));
                    if (call_args_ptr->type2 == 'f') {
                        *(float*)stack_write_ptr = value;
                    } else {
                        *(int32_t*)stack_write_ptr = value;
                    }
                    break;
                }
            }
            call_args_ptr += 2;
            stack_write_ptr += sizeof(int32_t);
        } while (++va_index < current_instruction->param_count);
    }
    int32_t current_stack_pointer = new_context->stack.pointer; // EBP-14 (LOCAL.5)
    if (stack_pointer) {
        int32_t idk_what = new_context->stack.pop<int32_t>();
        new_context->stack.pointer = stack_pointer;
        new_context->stack.write_temp(-1, idk_what);
        new_context->stack.write_temp(-2, current_stack_pointer);
        new_context->stack.push(this->time);
        new_context->stack.push(this->location.instruction_offset);
        new_context->stack.push(this->location.sub_index);
    } else {
        new_context->stack.pointer = sizeof(int32_t);
        new_context->stack.write_temp(0, current_stack_pointer);
        new_context->stack.push(-1);
        new_context->stack.push(-1);
        new_context->stack.push(-1);
    }
    new_context->stack.pointer += sizeof(int32_t);
    std::swap(new_context, this->vm->current_context);
    const char* sub_name = StringArg(4);
    clang_forceinline this->vm->set_context_to_sub(sub_name);
    if (this->get_current_instruction()) {
        this->vm->current_context = new_context;
        return ZUN_SUCCESS;
    } else {
        DebugLogger::__debug_log_stub_5(JpEnStr("", " error : undefined function name %s\n"), sub_name);
        this->location.reset();
        return ZUN_ERROR;
    }
}

// 0x48B3A0
dllexport gnu_noinline ZUNResult vectorcall EclContext::low_ecl_run(float, float current_gamespeed) {
    using namespace Ecl;

    EclInstruction* current_instruction = this->get_current_instruction(); // ESP+8
    if (expect(current_instruction == NULL, false)) {
        return ZUN_ERROR;
    }
    float& current_time = this->time;
    while (current_time <= (float)current_instruction->time) {
        if (current_instruction->difficulty_mask & this->difficulty_mask) {
            int32_t opcode = current_instruction->opcode;
            switch (opcode) {
                case ret: // 10
                    if (ZUN_SUCCEEDED(this->stack.leave_frame())) {
                        this->location.sub_index = this->stack.pop<int32_t>();
                        this->location.instruction_offset = this->stack.pop<int32_t>();
                        this->time = this->stack.pop<float>();
                        this->stack.pointer = this->stack.pop<int32_t>();
                        current_instruction = this->get_current_instruction();
                        if (this->location.instruction_offset >= 0) {
                            break;
                        }
                    }
                case enemy_delete: // 1
                delete_enemy:
                    this->location.reset();
                    return ZUN_ERROR;
                case async_call: // 15
                    this->vm->new_async(-1, 0);
                    goto skip_stack_adjust;
                case async_stop_all: // 21
                    this->vm->context_list.as_head().for_each_safeB([](EclContext* context) {
                        context->location.reset();
                    });
                    goto skip_stack_adjust;
                case async_call_id: { // 16
                    int32_t string_len = IntArg(0);
                    int32_t dword_len = (string_len + sizeof(int32_t)) / sizeof(int32_t);
                    int32_t id = this->parse_int_as_arg_pop(1, IntArg(dword_len));
                    this->vm->new_async(id, 1);
                    goto skip_stack_adjust;
                }
                case async_stop_id: { // 17
                    int32_t id = this->get_int_arg(0);
                    auto async_id_matches = [=](EclContext* context) {
                        return context->async_id == id;
                    };
                    if (EclContext* context = this->vm->context_list.find_if(async_id_matches)) {
                        context->location.instruction_offset = -1;
                    }
                    break;
                }
                case __async_unknown_flag_set: { // 18
                    int32_t id = this->get_int_arg(0);
                    auto async_id_matches = [=](EclContext* context) {
                        return context->async_id == id;
                    };
                    if (EclContext* context = this->vm->context_list.find_if(async_id_matches)) {
                        context->__unknown_flag_A = true;
                    }
                    break;
                }
                case __async_unknown_flag_clear: { // 19
                    int32_t id = this->get_int_arg(0);
                    auto async_id_matches = [=](EclContext* context) {
                        return context->async_id == id;
                    };
                    if (EclContext* context = this->vm->context_list.find_if(async_id_matches)) {
                        context->__unknown_flag_A = false;
                    }
                    break;
                }
                case __async_unknown_value: { // 20
                    int32_t id = this->get_int_arg(0);
                    auto async_id_matches = [=](EclContext* context) {
                        return context->async_id == id;
                    };
                    if (EclContext* context = this->vm->context_list.find_if(async_id_matches)) {
                        context->__int_101C = this->get_int_arg(1);
                    }
                    break;
                }
                case Opcode::call: // 11
                    current_instruction->stack_adjust = 0; // Evil hack by ZUN
                    if (ZUN_FAILED(this->call(this, 0))) {
                        goto delete_enemy;
                    }
                    current_instruction = this->get_current_instruction();
                    continue;
                case jump_neq: // 14
                    if (this->stack.pop_cast<int32_t>()) {
                        goto jump;
                    }
                    break;
                case jump_equ: // 13
                    if (this->stack.pop_cast<int32_t>()) {
                        break;
                    }
                case jump: // 12
                jump:
                    this->time = IntArg(1);
                    this->location.instruction_offset += IntArg(0);
                    IndexInstr(IntArg(0));
                    continue;
                case ecl_time_sub: // 23
                    this->time -= this->get_int_arg(0);
                    break;
                case ecl_time_sub_float: // 24
                    this->time -= this->get_float_arg(0);
                    break;
                case frame_enter: { // 40
                    int32_t locals_size = this->get_int_arg(0);
                    this->stack.enter_frame(locals_size);
                    break;
                }
                case frame_leave: // 41
                    this->stack.leave_frame();
                    break;
                case push_int: // 42
                    this->stack.push(this->get_int_arg_pop(0));
                    goto skip_stack_adjust;
                case push_float: // 44
                    this->stack.push(this->get_float_arg_pop(0));
                    goto skip_stack_adjust;
                case pop_int: { // 43
                    int32_t* write = this->get_int_ptr_arg(0);
                    *write = this->stack.pop_cast<int32_t>();
                    goto skip_stack_adjust;
                }
                case pop_float: { // 45
                    float* write = this->get_float_ptr_arg(0);
                    *write = this->stack.pop_cast<float>();
                    goto skip_stack_adjust;
                }
                case math_int_add: // 50
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs + rhs;
                    });
                    goto skip_stack_adjust;
                case math_int_sub: // 52
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs - rhs;
                    });
                    goto skip_stack_adjust;
                case math_int_mul: // 54
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs * rhs;
                    });
                    goto skip_stack_adjust;
                case math_int_div: // 56
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs / rhs;
                    });
                    goto skip_stack_adjust;
                case math_int_mod: // 58
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs % rhs;
                    });
                    goto skip_stack_adjust;
                case math_float_add: // 51
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs + rhs;
                    });
                    goto skip_stack_adjust;
                case math_float_sub: // 53
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs - rhs;
                    });
                    goto skip_stack_adjust;
                case math_float_mul: // 55
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs * rhs;
                    });
                    goto skip_stack_adjust;
                case math_float_div: // 57
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs / rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_int_equ: // 59
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs == rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_int_neq: // 61
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs != rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_int_les: // 63
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs < rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_int_leq: // 65
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs <= rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_int_gre: // 67
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs > rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_int_geq: // 69
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs >= rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_int_not: // 71
                    this->stack.unary_op([](int32_t value) {
                        return !value;
                    });
                    goto skip_stack_adjust;
                case cmp_float_equ: // 60
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs == rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_float_neq: // 62
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs != rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_float_les: // 64
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs < rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_float_leq: // 66
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs <= rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_float_gre: // 68
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs > rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_float_geq: // 70
                    this->stack.binary_op([](float lhs, float rhs) {
                        return lhs >= rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_float_not: // 72
                    this->stack.unary_op([](float value) {
                        return value != 0.0f;
                    });
                    goto skip_stack_adjust;
                case cmp_or: // 73
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs || rhs;
                    });
                    goto skip_stack_adjust;
                case cmp_and: // 74
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs && rhs;
                    });
                    goto skip_stack_adjust;
                case math_bit_xor: // 75
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs ^ rhs;
                    });
                    goto skip_stack_adjust;
                case math_bit_or: // 76
                    this->stack.binary_op([](int32_t lhs, int32_t rhs) {
                        return lhs | rhs;
                    });
                    goto skip_stack_adjust;
                case math_int_neg: // 83
                    this->stack.unary_op([](int32_t value) {
                        return -value;
                    });
                    goto skip_stack_adjust;
                case math_float_neg: // 84
                    this->stack.unary_op([](float value) {
                        return -value;
                    });
                    goto skip_stack_adjust;
                case math_post_dec: { // 78
                    int32_t value = this->get_int_arg(0);
                    int32_t* write = this->get_int_ptr_arg(0);
                    *write = value - 1;
                    this->stack.push(value);
                    goto skip_stack_adjust;
                }
                case math_sin: // 79
                    this->stack.unary_op([](float value) {
                        return zsin(value);
                    });
                    goto skip_stack_adjust;
                case math_sqrt: // 88
                    this->stack.unary_op([](float value) {
                        return zsqrt(value);
                    });
                    goto skip_stack_adjust;
                case math_cos: // 80
                    this->stack.unary_op([](float value) {
                        return zcos(value);
                    });
                    goto skip_stack_adjust;
                case math_circle_pos: { // 81
                    float angle = reduce_angle(this->get_float_arg(2));
                    float radius = this->get_float_arg(3);
                    Float2 position;
                    position.make_from_vector(angle, radius);
                    float* x_write = this->get_float_ptr_arg(0);
                    *x_write = position.x;
                    float* y_write = this->get_float_ptr_arg(1);
                    *y_write = position.y;
                    break;
                }
                case math_hypot_squared: { // 85
                    Float2 size;
                    size.x = this->get_float_arg(1);
                    size.y = this->get_float_arg(2);
                    float* write = this->get_float_ptr_arg(0);
                    *write = size.hypot_squared();
                    break;
                }
                case math_hypot: { // 86
                    Float2 size;
                    size.x = this->get_float_arg(1);
                    size.y = this->get_float_arg(2);
                    float* write = this->get_float_ptr_arg(0);
                    *write = size.hypot();
                    break;
                }
                case math_reduce_angle: { // 82
                    float angle = reduce_angle(this->get_float_arg(0));
                    float* write = this->get_float_ptr_arg(0);
                    *write = angle;
                    break;
                }
                case math_line_angle: { // 87
                    Float2 lhs;
                    Float2 rhs;
                    lhs.x = this->get_float_arg(1);
                    lhs.y = this->get_float_arg(2);
                    rhs.x = this->get_float_arg(3);
                    rhs.y = this->get_float_arg(4);
                    float angle = rhs.angle_to(lhs);
                    float* write = this->get_float_ptr_arg(0);
                    *write = angle;
                    break;
                }
                case math_angle_diff: { // 89
                    float angle1 = this->get_float_arg(1);
                    float angle2 = this->get_float_arg(2);
                    float diff = reduced_angle_diff(angle2, angle1);
                    float* write = this->get_float_ptr_arg(0);
                    *write = diff;
                    break;
                }
                case math_point_rotate: { // 90
                    Float2 position;
                    position.x = this->get_float_arg(2);
                    position.y = this->get_float_arg(3);
                    float angle = reduce_angle(this->get_float_arg(4));
                    position = position.rotate_around_origin(angle);
                    float* x_write = this->get_float_ptr_arg(0);
                    *x_write = position.x;
                    float* y_write = this->get_float_ptr_arg(1);
                    *y_write = position.y;
                    break;
                }
                case math_float_interp: { // 91
                    int32_t slot = this->get_int_arg(0);
                    this->float_interp_locations[slot] = this->location;
                    this->float_interps[slot].step(); // why?
                    this->float_interps[slot].end_time = this->get_int_arg(2);
                    this->float_interps[slot].mode = this->get_int_arg(3);
                    float initial_value = this->get_float_arg(4);
                    float final_value = this->get_float_arg(5);
                    this->float_interps[slot].initial_value = initial_value;
                    this->float_interps[slot].final_value = final_value;
                    float* interp_write = this->get_float_ptr_arg(1);
                    *interp_write = initial_value;
                    this->float_interps[slot].time.reset();
                    this->float_interp_stack_offsets[slot] = this->stack.base;
                    break;
                }
                case math_float_interp_bezier: { // 92
                    int32_t slot = this->get_int_arg(0);
                    this->float_interp_locations[slot] = this->location;
                    this->float_interps[slot].step(); // why?
                    this->float_interps[slot].end_time = this->get_int_arg(2);
                    this->float_interps[slot].mode = this->get_int_arg(3);
                    float initial_value = this->get_float_arg(4);
                    float final_value = this->get_float_arg(5);
                    this->float_interps[slot].initial_value = initial_value;
                    this->float_interps[slot].final_value = final_value;
                    float* interp_write = this->get_float_ptr_arg(1);
                    *interp_write = initial_value;
                    float bezier1 = this->get_float_arg(6);
                    float bezier2 = this->get_float_arg(7);
                    this->float_interps[slot].bezier1 = bezier1;
                    this->float_interps[slot].bezier2 = bezier2;
                    this->float_interps[slot].time.reset();
                    this->float_interp_stack_offsets[slot] = this->stack.base;
                    break;
                }
                case math_circle_pos_rand: { // 93
                    float min_radius = this->get_float_arg(0);
                    float max_radius = this->get_float_arg(1);
                    float angle = REPLAY_RNG.rand_float_signed() * PI_f;
                    float percent = REPLAY_RNG.rand_float_signed();
                    float radius = lerp(min_radius, max_radius, percent);
                    Float2 position;
                    position.make_from_vector(angle, radius);
                    float* x_write = this->get_float_ptr_arg(0);
                    *x_write = position.x;
                    float* y_write = this->get_float_ptr_arg(1);
                    *y_write = position.y;
                    break;
                }
                case math_ellipse_pos: { // 94
                    float angle = this->get_float_arg(2);
                    float offset = this->get_float_arg(4);
                    angle = reduce_angle(angle - offset);
                    float radius = this->get_float_arg(3);
                    Float2 position;
                    position.make_from_vector(angle, radius);
                    float ellipse_factor = this->get_float_arg(5);
                    position.x *= ellipse_factor;
                    position.rotate_around_origin(offset);
                    float* x_write = this->get_float_ptr_arg(0);
                    *x_write = position.x;
                    float* y_write = this->get_float_ptr_arg(1);
                    *y_write = position.y;
                    break;
                }
                default:
                    switch (this->vm->high_ecl_run()) {
                        case -1:
                            goto step_interps;
                        case 0:
                            break;
                        case 1:
                            goto skip_stack_adjust;
                    }
                    break;
                case nop:
                case __debug_unknown_A:
                case debug_print:
                case __debug_unknown_B:
                    // These don't go to the default case
                    break;
            }
            if (uint8_t stack_adjust = current_instruction->stack_adjust) {
                this->stack.pointer -= stack_adjust;
            }
        }
    skip_stack_adjust:
        this->location.instruction_offset += current_instruction->offset_to_next;
        IndexInstr(current_instruction->offset_to_next);
    }
    current_time += current_gamespeed;
step_interps:
    this->step_float_interps();
    return ZUN_SUCCESS;
}

// 0x430D40
dllexport gnu_noinline int32_t thiscall EnemyData::high_ecl_run() {
    using namespace Ecl;

    // this // EBP-570, EDI
    EclVM** vm_ptr = &this->vm; // EBP-578, ESI
    EclInstruction* current_instruction = this->vm->current_context->get_current_instruction(); // EBP-57C
    int16_t opcode = current_instruction->opcode; // EBP-56C

    switch (opcode) {
        case enemy_create_rel_stage: case enemy_create_abs_stage: // 309, 310
        case enemy_create_rel_stage_mirror: case enemy_create_abs_stage_mirror: // 311, 312
        {
            clang_forceinline if (ENEMY_MANAGER_PTR->get_boss_by_index(0)) {
        case enemy_create_rel: case enemy_create_abs: // 300, 301
        case enemy_create_rel_mirror: case enemy_create_abs_mirror: // 304, 305
        case __enemy_create_rel_2: // 321
                // TODO
            }
            break;
        }
        case anm_source: // 302
            this->anm_source_index = this->get_int_arg(0);
            break;
        case enemy_flag_mirror: // 424
            this->mirrored = this->get_int_arg(0);
            break;
        case __enemy_manager_flag_unknown_A: // 631
            ENEMY_MANAGER_PTR->__unknown_flag_A = this->get_int_arg(0);
            break;
        case anm_layer_base: // 552
            this->anm_base_layer = this->get_int_arg(0);
            break;
        case enemy_damage_sound: // 553
            this->hit_sound = this->get_int_arg(0);
            break;
        case score_add: { // 638
            int32_t score_bonus = this->get_int_arg(0);
            GAME_MANAGER.add_to_score(score_bonus);
            POPUP_MANAGER_PTR->create_popup(&this->current_motion.position, score_bonus, PackD3DCOLOR(255, 255, 255, 255));
            break;
        }
        case anm_set_slot: // 303
            this->anm_set_slot_impl();
            break;
        case anm_move_position_slot: { // 320
            int32_t slot = this->get_int_arg(0);
            this->anm_positions[slot].x = this->get_float_arg(1);
            this->anm_positions[slot].y = this->get_float_arg(2);
            this->anm_positions[slot].z = 0.0f;
            break;
        }
        // TODO
        case enemy_kill: // 566
            if (this->enemy()->kill()) {
                return 1;
            }
            break;
        case anm_death_effects: // 323
            this->death_anm_index = this->get_int_arg(0);
            this->death_anm_script = this->get_int_arg(1);
            break;
        case __enemy_kill_effect_create: // 561
            this->kill_effects<false>();
            break;
        case __anm_set_slot_anchor_index: // 322
            this->anm_vm_indices[this->get_int_arg(0)] = this->get_int_arg(1);
            break;
        case __stage_logo: // 554
            GUI_PTR->__display_stage_logo();
            break;
        case anm_create_rel_front_rotated: { // 338
            float Y = this->get_float_arg(3);
            float X = this->get_float_arg(2);
            Float3 position = this->current_motion.position + Float3(X, Y);
            float rotation = this->get_float_arg(4);
            int32_t script = this->get_int_arg(1);
            AnmID id = ENEMY_MANAGER_PTR->anm_file_lookup(this->get_int_arg(0))->instantiate_vm(
                script, &position, rotation, -1, WORLD_LIST_FRONT, NULL
            );
            EFFECT_MANAGER_PTR->fill_available_slot(id);
            break;
        }
        case anm_create_front: { // 307
            int32_t script = this->get_int_arg(1);
            AnmID id = ENEMY_MANAGER_PTR->anm_file_lookup(this->get_int_arg(0))->instantiate_vm(
                script, &this->current_motion.position, 0.0f, -1, WORLD_LIST_FRONT, NULL
            );
            EFFECT_MANAGER_PTR->fill_available_slot(id);
            break;
        }
        case anm_create_back: { // 314
            int32_t script = this->get_int_arg(1);
            AnmID id = ENEMY_MANAGER_PTR->anm_file_lookup(this->get_int_arg(0))->instantiate_vm(
                script, &this->current_motion.position, 0.0f, -1, WORLD_LIST_BACK, NULL
            );
            EFFECT_MANAGER_PTR->fill_available_slot(id);
            break;
        }
        case anm_create_zero_front: { // 308
            int32_t anm_index = this->get_int_arg(0);
            int32_t script = this->get_int_arg(1);
            AnmID id = ENEMY_MANAGER_PTR->anm_file_lookup(anm_index)->instantiate_vm(
                script, NULL, 0.0f, -1, WORLD_LIST_FRONT, NULL
            );
            EFFECT_MANAGER_PTR->fill_available_slot(id);
            break;
        }
        case __anm_create_zero_front_and_run: { // 339
            int32_t anm_index = this->get_int_arg(0);
            int32_t script = this->get_int_arg(1);
            AnmID id = ENEMY_MANAGER_PTR->anm_file_lookup(anm_index)->instantiate_vm(
                script, NULL, 0.0f, -1, WORLD_LIST_FRONT, NULL
            );
            EFFECT_MANAGER_PTR->fill_available_slot(id);
            AnmVM* vm = id.get_vm_ptr();
            for (int32_t i = 0; i < this->get_int_arg(2); ++i) {
                vm->run_anm();
            }
            break;
        }
        case anm_create_front_rotated: { // 315
            AnmID id;
            id = ENEMY_MANAGER_PTR->anm_file_lookup(this->get_int_arg(0))->instantiate_vm_to_world_list_front(this->get_int_arg(1), &this->get_position(), 0.0f);
            AnmVM* vm = id.get_vm_ptr();
            if (vm) {
                vm->set_controller_position(&this->get_position());
                vm->set_z_rotation(this->get_float_arg(2));
            }
            EFFECT_MANAGER_PTR->fill_available_slot(id);
            break;
        }
        case __effect_create_special: // 334
            EFFECT_MANAGER_PTR->create_effect(this->get_int_arg(0), &this->get_position(), 0);
            break;
        case anm_set_slot_main: // 306
            this->anm_set_slot_main_impl();
            break;
        case anm_play_main: // 318
            this->anm_play_main_impl();
            break;
        case anm_play_attack: // 313
            this->anm_play_attack_impl();
            break;
        case anm_play_attack_ex: // 316
            this->anm_play_attack_ex_impl();
            break;
        case anm_interrupt_slot: { // 317
            int32_t slot = this->get_int_arg(0);
            int16_t interrupt = this->get_int_arg(1);
            this->anm_vms[slot].interrupt_tree(interrupt);
            break;
        }
        case move_stop: { // 427
            MotionData& motion_rel = this->motion.relative;
            MotionData& motion_abs = this->motion.absolute;
            motion_abs.get_position() += motion_rel.get_position();
            motion_rel.position = UNKNOWN_FLOAT3_B;
            motion_rel.set_speed(0.0f);
            motion_abs.set_speed(0.0f);
            motion_rel.set_orbit_origin(UNKNOWN_FLOAT3_B);
            motion_abs.set_orbit_origin(UNKNOWN_FLOAT3_B);
            motion_rel.set_axis_velocity_mode();
            motion_abs.set_axis_velocity_mode();
            this->position_interp.absolute.reset_end_time();
            this->position_interp.relative.reset_end_time();
            this->angle_interp_absolute.reset_end_time();
            this->angle_interp_relative.reset_end_time();
            this->speed_interp_absolute.reset_end_time();
            this->speed_interp_relative.reset_end_time();
            this->orbit_radius_interp.absolute.reset_end_time();
            this->orbit_radius_interp.relative.reset_end_time();
            this->ellipse_interp.absolute.reset_end_time();
            this->ellipse_interp.relative.reset_end_time();
            break;
        }
        case move_position_abs: case move_position_rel: { // 400, 402
            MotionData& motion = (opcode != move_position_abs) ? this->motion.relative : this->motion.absolute;
            float X = this->get_float_arg(0);
            float Y = this->get_float_arg(1);
            if (X > -999999.0) {
                motion.set_position_x(X);
            }
            if (Y > -999999.0) {
                motion.set_position_y(Y);
            }
            motion.set_axis_velocity_mode();
            this->current_motion.get_position() = this->motion.relative.get_position() + this->motion.absolute.get_position();
            this->update_current_motion();
            break;
        }
        case move_position_add_abs: case move_position_add_rel: { // 416 417
            MotionData& motion = (opcode == move_position_abs) ? this->motion.absolute : this->motion.relative;
            float X = this->get_float_arg(0);
            float Y = this->get_float_arg(1);
            float Z = this->get_float_arg(2);
            motion.get_position().x += X;
            motion.get_position().y += Y;
            motion.get_position().z += Z;
            this->update_current_motion();
            break;
        }
        case move_position_abs_interp: case move_position_rel_interp: // 401, 403
        case __move_position_offset_abs_interp: case __move_position_offset_rel_interp: // 436, 437
        {
            MotionData& motion = (opcode != move_position_abs_interp && opcode != __move_position_offset_abs_interp) ? this->motion.relative : this->motion.absolute;
            ZUNInterpEx<Float3>& position_interp = (opcode != move_position_abs_interp && opcode != __move_position_offset_abs_interp) ? this->position_interp.relative : this->position_interp.absolute;
            float X = this->get_float_arg(2);
            float Y = this->get_float_arg(3);
            if (this->get_int_arg(0) <= 0) {
                position_interp.reset_end_time();
                break;
            }
            if (opcode == __move_position_offset_abs_interp || opcode == __move_position_offset_rel_interp) {
                if (!this->get_mirror_flag()) {
                    X = motion.get_position_x() + X;
                } else {
                    X = motion.get_position_x() - X;
                }
                Y = motion.get_position_y() + Y;
            }
            position_interp.set_end_time(this->get_int_arg(0));
            position_interp.set_bezier1(UNKNOWN_FLOAT3_B);
            position_interp.set_bezier2(UNKNOWN_FLOAT3_B);
            position_interp.set_combined_mode(this->get_int_arg(1));
            position_interp.set_initial_value(motion.get_position());
            if (!(Y > -999999.0f)) {
                Y = motion.get_position_y();
            }
            if (!(X > -999999.0f)) {
                X = motion.get_position_x();
            }
            position_interp.set_final_value(Float3(X, Y, 0.0f));
            position_interp.reset_timer();
            motion.set_axis_velocity_mode();
            break;
        }
        case move_axis_interp_abs: case move_axis_interp_rel: // 434, 435
        case __move_axis_offset_abs_interp: case __move_axis_offset_rel_interp: // 438, 439
        {
            MotionData& motion = (opcode != move_axis_interp_abs && opcode != __move_axis_offset_abs_interp) ? this->motion.relative : this->motion.absolute;
            ZUNInterpEx<Float3>& position_interp = (opcode != move_axis_interp_abs && opcode != __move_axis_offset_abs_interp) ? this->position_interp.relative : this->position_interp.absolute;
            float X = this->get_float_arg(3);
            float Y = this->get_float_arg(4);
            if (this->get_int_arg(0) <= 0) {
                position_interp.reset_end_time();
                break;
            }
            if (opcode == __move_axis_offset_abs_interp || opcode == __move_axis_offset_rel_interp) {
                if (!this->get_mirror_flag()) {
                    X = motion.get_position_x() + X;
                } else {
                    X = motion.get_position_x() - X;
                }
                Y = motion.get_position_y() + Y;
            }
            position_interp.set_end_time(this->get_int_arg(0));
            position_interp.set_bezier1(UNKNOWN_FLOAT3_B);
            position_interp.set_bezier2(UNKNOWN_FLOAT3_B);
            position_interp.set_axis_mode(0, this->get_int_arg(1));
            position_interp.set_axis_mode(1, this->get_int_arg(2));
            position_interp.set_initial_value(motion.get_position());
            if (!(Y > -999999.0f)) {
                Y = motion.get_position_y();
            }
            if (!(X > -999999.0f)) {
                X = motion.get_position_x();
            }
            position_interp.set_final_value(Float3(X, Y, 0.0f));
            position_interp.reset_timer();
            motion.set_axis_velocity_mode();
            break;
        }
        case move_bezier_abs: case move_bezier_rel: { // 425, 426
            MotionData& motion = (opcode == move_bezier_abs) ? this->motion.absolute : this->motion.relative;
            ZUNInterpEx<Float3>& position_interp = (opcode == move_bezier_abs) ? this->position_interp.absolute : this->position_interp.relative;
            float X = this->get_float_arg(3);
            float Y = this->get_float_arg(4);
            float Z = 0.0f;
            Float3 bezier1 = {};
            Float3 bezier2 = {};
            bezier1.x = this->get_float_arg(1);
            bezier1.y = this->get_float_arg(2);
            bezier1.z = 0.0f;
            bezier2.x = this->get_float_arg(5);
            bezier2.y = this->get_float_arg(6);
            bezier2.z = 0.0f;
            position_interp.set_end_time(this->get_int_arg(0));
            position_interp.set_bezier1(bezier1);
            position_interp.set_bezier2(bezier2);
            position_interp.set_combined_mode(Bezier);
            position_interp.set_initial_value(motion.get_position());
            if (!(Y > -999999.0)) {
                Y = motion.get_position_y();
            }
            if (!(X > -999999.0)) {
                X = motion.get_position_x();
            }
            position_interp.set_final_value(Float3(X, Y, Z));
            position_interp.reset_timer();
            motion.set_axis_velocity_mode();
            break;
        }
        case move_velocity_abs: case move_velocity_rel: // 404, 406
        case move_velocity_no_mirror_abs: case move_velocity_no_mirror_rel: // 428, 430
        {
            MotionData& motion = (opcode != move_velocity_abs && opcode != move_velocity_no_mirror_abs) ? this->motion.relative : this->motion.absolute;
            float angle = this->get_float_arg(0);
            float speed = this->get_float_arg(1);
            if (angle > -999999.0) {
                if (this->get_mirror_flag() && (opcode == move_velocity_abs || opcode == move_velocity_rel)) {
                    angle = reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f));
                }
                motion.set_angle(angle);
            }
            if (speed > -999999.0) {
                motion.set_speed(speed);
            }
            motion.set_axis_velocity_mode();
            break;
        }
        case move_velocity_abs_interp: case move_velocity_rel_interp: // 405, 407
        case move_velocity_no_mirror_abs_interp: case move_velocity_no_mirror_rel_interp: // 429, 431
        {
            MotionData& motion = (opcode != move_velocity_abs_interp && opcode != move_velocity_no_mirror_abs_interp) ? this->motion.relative : this->motion.absolute;
            ZUNInterp<float>& angle_interp = (opcode != move_velocity_abs_interp && opcode != move_velocity_no_mirror_abs_interp) ? this->angle_interp_relative : this->angle_interp_absolute;
            ZUNInterp<float>& speed_interp = (opcode != move_velocity_abs_interp && opcode != move_velocity_no_mirror_abs_interp) ? this->speed_interp_relative : this->speed_interp_absolute;
            float angle = this->get_float_arg(2);
            float speed = this->get_float_arg(3);
            Float2 final_val = {};
            if (this->get_int_arg(0) <= 0) {
                angle_interp.reset_end_time();
                speed_interp.reset_end_time();
                break;
            }
            int32_t interp_mode = this->get_int_arg(1);
            if (interp_mode != ConstantVelocity) {
                if (angle > -999999.0) {
                    if (this->get_mirror_flag() && (opcode == move_velocity_abs_interp || opcode == move_velocity_rel_interp)) {
                        final_val.x = reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f));
                    } else {
                        final_val.x = angle;
                    }
                } else {
                    final_val.x = motion.get_angle();
                }
                if (speed > -999999.0) {
                    final_val.y = speed;
                } else {
                    final_val.y = motion.get_speed();
                }
            } else {
                if (angle > -999999.0) {
                    if (this->get_mirror_flag() && (opcode == move_velocity_abs_interp || opcode == move_velocity_rel_interp)) {
                        final_val.x = -angle;
                    } else {
                        final_val.x = angle;
                    }
                } else {
                    final_val.x = 0.0f;
                }
                if (speed > -999999.0) {
                    final_val.y = speed;
                } else {
                    final_val.y = 0.0f;
                }
            }
            Float2 initial_val = Float2(motion.get_angle(), motion.get_speed());
            if (fabsf(initial_val.x - final_val.x) >= PI_f) {
                if (final_val.x > initial_val.x) {
                    initial_val.x += TWO_PI_f;
                } else {
                    final_val.x += TWO_PI_f;
                }
            }
            angle_interp.initialize(this->get_int_arg(0), interp_mode, initial_val.x, final_val.x);
            speed_interp.initialize(this->get_int_arg(0), interp_mode, initial_val.y, final_val.y);
            motion.set_axis_velocity_mode();
            break;
        }
        case move_angle_abs: case move_angle_rel: // 440, 442
        {
            MotionData& motion = (opcode == move_angle_abs) ? this->motion.absolute : this->motion.relative;
            float angle = this->get_float_arg(0);
            if (this->get_mirror_flag()) {
                motion.set_angle(reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f)));
                motion.set_axis_velocity_mode();
            } else {
                motion.set_angle(angle);
                motion.set_axis_velocity_mode();
            }
            break;
        }
        case move_angle_abs_interp: case move_angle_rel_interp: { // 441, 443
            MotionData& motion = (opcode == move_angle_abs) ? this->motion.absolute : this->motion.relative;
            ZUNInterp<float>& angle_interp = (opcode == move_angle_abs) ? this->angle_interp_absolute : this->angle_interp_relative;
            float angle = this->get_float_arg(2);
            if (this->get_int_arg(0) <= 0) {
                angle_interp.reset_end_time();
                break;
            }
            int32_t interp_mode = this->get_int_arg(1);
            if (interp_mode != ConstantVelocity) {
                if (angle > -999999.0) {
                    if (this->get_mirror_flag() && (opcode == move_angle_abs_interp || opcode == move_angle_rel_interp)) {
                        angle = reduce_angle(HALF_PI_f - reduce_angle(angle - HALF_PI_f));
                    }
                } else {
                    angle = motion.get_angle();
                }
            } else {
                if (angle > -999999.0) {
                    if (this->get_mirror_flag() && (opcode == move_angle_abs_interp || opcode == move_angle_rel_interp)) {
                        angle = -angle;
                    }
                } else {
                    angle = 0.0f;
                }
            }
            float current_angle = motion.get_angle();
            angle = reduced_angle_diff(current_angle, angle) + current_angle;
            angle_interp.initialize(this->get_int_arg(0), interp_mode, current_angle, angle);
            motion.set_axis_velocity_mode();
            break;
        }
        case move_speed_abs: case move_speed_rel: { // 444, 446
            MotionData& motion = (opcode == move_speed_abs) ? this->motion.absolute : this->motion.relative;
            float speed = this->get_float_arg(0);
            if (speed > -999999.0) {
                motion.set_speed(speed);
            }
            motion.set_axis_velocity_mode();
            break;
        }
        case move_speed_abs_interp: case move_speed_rel_interp: { // 445, 447
            MotionData& motion = (opcode == move_speed_abs) ? this->motion.absolute : this->motion.relative;
            ZUNInterp<float>& speed_interp = (opcode == move_speed_abs) ? this->speed_interp_absolute : this->speed_interp_relative;
            float speed = this->get_float_arg(2);
            float current_speed = motion.get_speed();
            if (this->get_int_arg(0) <= 0) {
                speed_interp.reset_end_time();
                break;
            }
            int32_t interp_mode = this->get_int_arg(1);
            if (interp_mode != ConstantVelocity) {
                if (!(speed > -999999.0)) {
                    speed = motion.get_speed();
                }
            } else {
                if (!(speed > -999999.0)) {
                    speed = 0.0f;
                }
            }
            speed_interp.initialize(this->get_int_arg(0), interp_mode, current_speed, speed);
            motion.set_axis_velocity_mode();
            break;
        }
        case move_orbit_abs: case move_orbit_rel: { // 408, 410
            MotionData& motion = (opcode == move_orbit_abs) ? this->motion.absolute : this->motion.relative;
            float angle = this->get_float_arg(0);
            float speed = this->get_float_arg(1);
            float orbit_radius = this->get_float_arg(2);
            float orbit_velocity = this->get_float_arg(3);
            if (motion.mode_is_orbit()) {
                motion.set_orbit_origin(motion.get_position());
            }
            if (angle > -999999.0) {
                motion.set_angle(angle);
            }
            if (speed > -999999.0) {
                motion.set_speed(speed);
            }
            if (orbit_radius > -999999.0) {
                motion.set_orbit_radius(orbit_radius);
            }
            if (orbit_velocity > -999999.0) {
                motion.set_orbit_velocity(orbit_velocity);
            }
            motion.set_orbit_mode();
            motion.update2();
            this->update_current_motion();
            break;
        }
        case move_orbit_abs_interp: case move_orbit_rel_interp: { // 409, 411
            MotionData& motion = (opcode == move_orbit_abs_interp) ? this->motion.absolute : this->motion.relative;
            ZUNInterp<float>& speed_interp = (opcode == move_orbit_abs_interp) ? this->speed_interp_absolute : this->speed_interp_relative;
            ZUNInterp<Float2>& orbit_radius_interp = (opcode == move_orbit_abs_interp) ? this->orbit_radius_interp.absolute : this->orbit_radius_interp.relative;
            float speed = this->get_float_arg(2);
            float orbit_radius = this->get_float_arg(3);
            float orbit_velocity = this->get_float_arg(4);
            if (!(speed > -999999.0)) {
                speed = motion.get_speed();
            }
            Float2 final_speed = Float2(0.0f, speed);
            Float2 initial_speed = Float2(0.0f, motion.get_speed());
            if (!(orbit_velocity > -999999.0)) {
                orbit_velocity = motion.get_orbit_velocity();
            }
            if (!(orbit_radius > -999999.0)) {
                orbit_radius = motion.get_orbit_radius();
            }
            Float2 final_orbit = Float2(orbit_radius, orbit_velocity);
            Float2 initial_orbit = Float2(motion.get_orbit_radius(), motion.get_orbit_velocity());
            int32_t end_time = this->get_int_arg(0);
            int32_t interp_mode = this->get_int_arg(1);
            if (end_time <= 0) {
                speed_interp.reset_end_time();
                orbit_radius_interp.reset_end_time();
                break;
            }
            speed_interp.initialize(end_time, interp_mode, initial_speed.y, final_speed.y);
            orbit_radius_interp.initialize_bezier(end_time, interp_mode, initial_orbit, final_orbit, UNKNOWN_FLOAT2_A, UNKNOWN_FLOAT2_A);
            motion.set_orbit_mode();
            motion.update2();
            this->update_current_motion();
            break;
        }
        case move_origin_abs: case move_origin_rel: { // 418, 419
            MotionData& motion = (opcode == move_origin_abs) ? this->motion.absolute : this->motion.relative;
            float X = this->get_float_arg(0);
            float Y = this->get_float_arg(1);
            if (X > -999999.0) {
                motion.set_orbit_origin_x(X);
            }
            if (Y > -999999.0) {
                motion.set_orbit_origin_y(Y);
            }
            motion.update2();
            this->update_current_motion();
            // break; // Nice one ZUN
        }
        case move_ellipse_abs: case move_ellipse_rel: { // 420, 422
            MotionData& motion = (opcode == move_ellipse_abs) ? this->motion.absolute : this->motion.relative;
            float angle = this->get_float_arg(0);
            float speed = this->get_float_arg(1);
            float orbit_radius = this->get_float_arg(2);
            float orbit_velocity = this->get_float_arg(3);
            float ellipse_angle = this->get_float_arg(4);
            float ellipse_ratio = this->get_float_arg(5);
            if (motion.mode_is_orbit()) {
                motion.set_orbit_origin(motion.get_position());
            }
            if (angle > -999999.0) {
                motion.set_angle(angle);
            }
            if (speed > -999999.0) {
                motion.set_speed(speed);
            }
            if (orbit_radius > -999999.0) {
                motion.set_orbit_radius(orbit_radius);
            }
            if (orbit_velocity > -999999.0) {
                motion.set_orbit_velocity(orbit_velocity);
            }
            if (ellipse_angle > -999999.0) {
                motion.set_ellipse_angle(ellipse_angle);
            }
            if (ellipse_ratio > -999999.0) {
                motion.set_ellipse_ratio(ellipse_ratio);
            }
            motion.set_orbit_mode();
            motion.set_ellipse_mode();
            motion.update2();
            this->update_current_motion();
            break;
        }
        case move_ellipse_abs_interp: case move_ellipse_rel_interp: { // 421, 423
            MotionData& motion = (opcode == move_ellipse_abs_interp) ? this->motion.absolute : this->motion.relative;
            ZUNInterp<float>& speed_interp = (opcode == move_ellipse_abs_interp) ? this->speed_interp_absolute : this->speed_interp_relative;
            ZUNInterp<Float2>& orbit_radius_interp = (opcode == move_ellipse_abs_interp) ? this->orbit_radius_interp.absolute : this->orbit_radius_interp.relative;
            ZUNInterp<Float2>& ellipse_interp = (opcode == move_ellipse_abs_interp) ? this->ellipse_interp.absolute : this->ellipse_interp.relative;
            float speed = this->get_float_arg(2);
            float orbit_radius = this->get_float_arg(3);
            float orbit_velocity = this->get_float_arg(4);
            float ellipse_angle = this->get_float_arg(5);
            float ellipse_ratio = this->get_float_arg(6);
            if (!(speed > -999999.0)) {
                speed = motion.get_speed();
            }
            Float2 final_speed = Float2(0.0f, speed);
            Float2 initial_speed = Float2(0.0f, motion.get_speed());
            if (!(orbit_velocity > -999999.0)) {
                orbit_velocity = motion.get_orbit_velocity();
            }
            if (!(orbit_radius > -999999.0)) {
                orbit_radius = motion.get_orbit_radius();
            }
            Float2 final_orbit = Float2(orbit_radius, orbit_velocity);
            Float2 initial_orbit = Float2(motion.get_orbit_radius(), motion.get_orbit_velocity());
            if (!(ellipse_ratio > -999999.0)) {
                ellipse_ratio = motion.get_ellipse_ratio();
            }
            if (!(ellipse_angle > -999999.0)) {
                ellipse_angle = motion.get_ellipse_angle();
            }
            Float2 final_ellipse = Float2(ellipse_angle, ellipse_ratio);
            Float2 initial_ellipse = Float2(motion.get_ellipse_angle(), motion.get_ellipse_ratio());
            int32_t end_time = this->get_int_arg(0);
            int32_t interp_mode = this->get_int_arg(1);
            if (end_time <= 0) {
                speed_interp.reset_end_time();
                orbit_radius_interp.reset_end_time();
                ellipse_interp.reset_end_time();
                break;
            }
            speed_interp.initialize(end_time, interp_mode, initial_speed.y, final_speed.y);
            orbit_radius_interp.initialize_bezier(end_time, interp_mode, initial_orbit, final_orbit, UNKNOWN_FLOAT2_A, UNKNOWN_FLOAT2_A);
            ellipse_interp.initialize_bezier(end_time, interp_mode, initial_ellipse, final_ellipse, UNKNOWN_FLOAT2_A, UNKNOWN_FLOAT2_A);
            motion.set_orbit_origin(motion.get_position());
            motion.set_orbit_mode();
            motion.set_ellipse_mode();
            motion.update2();
            this->update_current_motion();
            break;
        }
        case move_to_boss0_abs: // 414
            this->motion.absolute.set_positionB(&ENEMY_MANAGER_PTR->get_boss_by_index(0)->get_current_motion()->position);
            break;
        case move_to_boss0_rel: // 415
            this->motion.relative.set_positionB(&ENEMY_MANAGER_PTR->get_boss_by_index(0)->get_current_motion()->position);
            break;
        case move_to_enemy_id_abs: // 432
            this->motion.absolute.set_positionB(&ENEMY_MANAGER_PTR->get_enemy_by_id(this->get_int_arg(0))->get_current_motion()->position);
            break;
        case move_to_enemy_id_rel: // 433
            this->motion.relative.set_positionB(&ENEMY_MANAGER_PTR->get_enemy_by_id(this->get_int_arg(0))->get_current_motion()->position);
            break;
        case move_bounds_set: // 504
            this->move_bounds_enable = true;
            this->move_bounds_center.x = this->get_float_arg(0);
            this->move_bounds_center.y = this->get_float_arg(1);
            this->move_bounds_size.x = this->get_float_arg(2);
            this->move_bounds_size.y = this->get_float_arg(3);
            break;
        case move_bounds_disable: // 505
            this->move_bounds_enable = false;
            break;
        case player_protect_range: // 526
            this->player_protect_radius = this->get_float_arg(0);
            break;
        case move_rand_interp_abs: case move_rand_interp_rel: { // 412, 413
            // nooooo this one sucks, I'll do it later
            break;
        }
        case enemy_lifebar_color: { // 527
            D3DCOLOR color = this->get_int_arg(2);
            float hp_position = this->get_float_arg(1);
            int32_t slot = this->get_int_arg(0);
            hp_position /= this->get_life_data().get_maximum();
            GUI_PTR->set_lifebar_marker(this->boss_id, slot, color, hp_position);
            break;
        }
        case ex_ins_repeat: { // 632
            int32_t index = this->get_int_arg(0);
            this->__is_func_set_2 = FALSE;
            //this->func_set_func = ECL_FUNC_CALL_TABLE[index];
            break;
        }
        case __ex_ins_repeat2: { // 639
            int32_t index = this->get_int_arg(0);
            this->__is_func_set_2 = TRUE;
            //this->func_set_func = ECL_FUNC_CALL_TABLE[index];
            break;
        }
        case enemy_damage_ex: { // 633
            int32_t index = this->get_int_arg(0);
            this->extra_damage_func = EXTRA_DAMAGE_FUNC_TABLE[index];
            break;
        }
        case enemy_hitbox_ex: { // 634
            int32_t index = this->get_int_arg(0);
            this->extra_hitbox_func = EXTRA_HITBOX_FUNC_TABLE[index];
            break;
        }
        case ex_ins_call: { // 637
            int32_t index = this->get_int_arg(0);
            //ECL_FUNC_CALL_TABLE[index](this);
            break;
        }
        case enemy_set_hitbox: // 500
            this->hitbox_size.x = this->get_float_arg(0);
            this->hitbox_size.y = this->get_float_arg(1);
            if (!this->chapter_spawn_weight) {
                this->chapter_spawn_weight = 1;
                GAME_MANAGER.globals.__add_to_int_8C(1);
            }
            break;
        case enemy_set_collision: // 501
            this->collision_size.x = this->get_float_arg(0);
            this->collision_size.y = this->get_float_arg(1);
            break;
        case __enemy_set_chapter_spawn_weight: { // 569
            int32_t weight;
            switch (this->chapter_spawn_weight) {
                case 0:
                    weight = this->get_int_arg(0);
                    this->chapter_spawn_weight = weight;
                    if (weight > 0) {
                        GAME_MANAGER.globals.__add_to_int_8C(weight);
                    }
                    break;
                case 1:
                    weight = this->get_int_arg(0);
                    this->chapter_spawn_weight = weight;
                    GAME_MANAGER.globals.__add_to_int_8C(weight - 1);
                    break;
            }
            break;
        }
        case __enemy_add_spawn_weight_to_destroy: // 570
            this->add_spawn_weight_to_chapter_destroy<false>();
            break;
        case enemy_flag_hitbox_shape: // 563
            this->rectangular_hitbox = this->get_int_arg(0);
            break;
        case enemy_set_hitbox_rotation: // 564
            this->hitbox_rotation = this->get_float_arg(0);
            break;
        case enemy_bomb_shield_multiplier: // 565
            this->bomb_damage_multiplier = this->get_float_arg(0);
            break;
        case enemy_flags_set: // 502
            this->flags_low |= this->get_int_arg(0);
            if (this->intangible) {
                for (size_t i = 0; i < 16; ++i) {
                    this->anm_vms[i].__unknown_tree_clear_J();
                }
            }
            break;
        case enemy_flags_clear: // 503
            this->flags_low &= ~this->get_int_arg(0);
            if (this->intangible) {
                for (size_t i = 0; i < 16; ++i) {
                    this->anm_vms[i].__unknown_tree_set_J();
                }
            }
            break;
        case item_bonus_slots_reset: // 506
            this->drops.reset_extra_drops();
            break;
        case item_bonus_slot_set: { // 507
            int32_t slot = this->get_int_arg(0);
            int32_t item = this->get_int_arg(1);
            this->drops.set_extra_drop(slot, item);
            break;
        }
        case item_timed_bonus_slot_set: { // 573
            int32_t slot = this->get_int_arg(0);
            int32_t item = this->get_int_arg(1);
            this->drops.set_timed_extra_drop(slot, item);
            break;
        }
        case item_drop_area: { // 508
            float height = this->get_float_arg(1);
            float width = this->get_float_arg(0);
            this->drops.set_area(width, height);
            break;
        }
        case drop_item_rewards: // 509
            if (GAME_MANAGER.__unknown_field_A_is_2()) {
        case drop_item_rewards_force: // 562
                this->drops.spawn_items(&this->get_position());
            }
            break;
        case item_reward_set: { // 510
            int32_t item = this->get_int_arg(0);
            this->drops.set_main_drop(item);
            break;
        }
        case enemy_life_set: { // 511
            EnemyLife& life = this->life;
            int32_t hp = this->get_int_arg(0);
            life.set_current(hp);
            life.set_maximum(life.get_current());
            life.set_current_remaining(life.get_current());
            if (this->is_boss) {
                this->__unknown_flag_O = true;
            }
            life.set_current_scaled(life.get_current());
            break;
        }
        case __enemy_life_set_current: { // 572
            int32_t hp = this->get_int_arg(0);
            this->life.set_current(hp);
            this->life.set_current_scaled(this->life.get_current());
            break;
        }
        case boss_set: {
            int32_t index = this->get_int_arg(0);
            ENEMY_MANAGER_PTR->__set_unkown_flag_A(false);
            if (index < 0) {
                if (this->is_boss) {
                    ENEMY_MANAGER_PTR->set_boss(this->boss_id, NULL);
                }
                this->is_boss = false;
            }
            else {
                this->is_boss = true;
                ENEMY_MANAGER_PTR->set_boss(index, this->enemy());
                this->boss_id = index;
            }
            break;
        }
        case enemy_invincible_timer: { // 515
            int32_t duration = this->get_int_arg(0);
            this->invulnerable_timer.set(duration);
            break;
        }
        case enemy_no_collision_timer: { // 541
            int32_t duration = this->get_int_arg(0);
            this->no_collision_timer.set(duration);
            break;
        }
        case enemy_kill_all_id: { // 551
            int32_t kill_id = this->get_int_arg(0);
            ENEMY_MANAGER_PTR->kill_all_by_kill_id(kill_id);
            break;
        }
        case enemy_kill_all_no_callbacks: // 571
            ENEMY_MANAGER_PTR->kill_all_no_callbacks();
            break;
        case set_int_difficulty: { // 535
            int32_t easy = this->get_int_arg(1);
            int32_t normal = this->get_int_arg(2);
            int32_t hard = this->get_int_arg(3);
            int32_t lunatic = this->get_int_arg(4);
            switch (GAME_MANAGER.get_difficulty()) {
                case EASY:
                    *this->get_int_ptr_arg(0) = easy;
                    break;
                case NORMAL:
                    *this->get_int_ptr_arg(0) = normal;
                    break;
                case HARD:
                    *this->get_int_ptr_arg(0) = hard;
                    break;
                default:
                    *this->get_int_ptr_arg(0) = lunatic;
                    break;
            }
            break;
        }
        case set_float_difficulty: { // 536
            float easy = this->get_float_arg(1);
            float normal = this->get_float_arg(2);
            float hard = this->get_float_arg(3);
            float lunatic = this->get_float_arg(4);
            switch (GAME_MANAGER.get_difficulty()) {
                case EASY:
                    *this->get_float_ptr_arg(0) = easy;
                    break;
                case NORMAL:
                    *this->get_float_ptr_arg(0) = normal;
                    break;
                case HARD:
                    *this->get_float_ptr_arg(0) = hard;
                    break;
                default:
                    *this->get_float_ptr_arg(0) = lunatic;
                    break;
            }
            break;
        }
        case shooter_reset: { // 600
            int32_t slot = this->get_int_arg(0);
            this->shooters[slot].zero_contents();
            this->shooters[slot].angle1 = 0.0f;
            this->shooters[slot].speed1 = 2.0f;
            this->shooters[slot].count1 = 1;
            this->shooters[slot].count2 = 1;
            this->shooters[slot].shoot_sound = 21;
            this->shooters[slot].transform_sound = 38;
            this->shooters[slot].flags = 0x23; // TODO: convert to bitfields
            this->shooter_offsets[slot].x = 0.0f;
            this->shooter_offsets[slot].y = 0.0f;
            this->shooter_origins[slot].x = 0.0f;
            this->shooter_origins[slot].y = 0.0f;
            this->shooter_origins[slot].z = 0.0f;
            this->bullet_effect_indices[slot] = 0;
            break;
        }
        case shooter_copy: { // 614
            int32_t dst = this->get_int_arg(0);
            int32_t src = this->get_int_arg(1);
            this->shooters[dst] = this->shooters[src];
            this->shooter_offsets[dst] = this->shooter_offsets[src];
            this->shooter_origins[dst] = this->shooter_origins[src];
            break;
        }
        case shoot_now: { // 601
            int32_t slot = this->get_int_arg(0);
            if (this->shooter_origins[slot].z > 0.9f) {
                this->shooters[slot].position.set(this->shooter_offsets[slot].as2() + this->shooter_origins[slot].as2());
            } else {
                this->shooters[slot].position = this->current_motion.get_position() + this->shooter_offsets[slot];
            }
            BULLET_MANAGER_PTR->set_player_protect_radius(this->player_protect_radius);
            BULLET_MANAGER_PTR->shoot_bullets(&this->shooters[slot]);
            BULLET_MANAGER_PTR->set_player_protect_radius(0.0f);
            break;
        }
        case bullet_sprite: { // 602
            int32_t slot = this->get_int_arg(0);
            int32_t type = this->get_int_arg(1);
            this->shooters[slot].type = type;
            int32_t color = this->get_int_arg(2);
            this->shooters[slot].color = color;
            break;
        }
        case shoot_offset: { // 603
            int32_t slot = this->get_int_arg(0);
            float x = this->get_float_arg(1);
            this->shooter_offsets[slot].x = x;
            float y = this->get_float_arg(2);
            this->shooter_offsets[slot].y = y;
            break;
        }
        case shoot_offset_circle: { // 626
            int32_t slot = this->get_int_arg(0);
            float angle = this->get_float_arg(1);
            float radius = this->get_float_arg(2);
            Float3 offset;
            offset.make_from_vector(angle, radius);
            this->shooter_offsets[slot].x = offset.x;
            this->shooter_offsets[slot].y = offset.y;
            break;
        }
        case shoot_spawn_distance: { // 627
            int32_t slot = this->get_int_arg(0);
            float distance = this->get_float_arg(1);
            this->shooters[slot].distance = distance;
            break;
        }
        case __shoot_origin: { // 628
            int32_t slot = this->get_int_arg(0);
            float x = this->get_float_arg(1);
            this->shooter_origins[slot].x = x;
            float y = this->get_float_arg(2);
            this->shooter_origins[slot].y = y;
            if (this->shooter_origins[slot].x < -990.0f) {
                this->shooter_origins[slot].z = 0.0f;
            } else {
                this->shooter_origins[slot].z = 1.0f;
            }
            break;
        }
        case shoot_angle: { // 604
            int32_t slot = this->get_int_arg(0);
            float angle1 = reduce_angle(this->get_float_arg(1));
            this->shooters[slot].angle1 = angle1;
            float angle2 = reduce_angle(this->get_float_arg(2));
            this->shooters[slot].angle2 = angle2;
            break;
        }
        case bullet_speed: { // 605
            int32_t slot = this->get_int_arg(0);
            float speed1 = this->get_float_arg(1);
            this->shooters[slot].speed1 = speed1;
            float speed2 = this->get_float_arg(2);
            this->shooters[slot].speed2 = speed2;
            break;
        }
        case bullet_count: { // 606
            int32_t slot = this->get_int_arg(0);
            int32_t count1 = this->get_int_arg(1);
            this->shooters[slot].count1 = count1;
            int32_t count2 = this->get_int_arg(2);
            this->shooters[slot].count2 = count2;
            break;
        }
        case bullet_speed_difficulty: { // 624
            int32_t slot = this->get_int_arg(0);
            int32_t arg_index;
            if (GAME_MANAGER.get_difficulty() == EASY) {
                arg_index = 1;
            } else if (GAME_MANAGER.get_difficulty() == NORMAL) {
                arg_index = 2;
            } else if (GAME_MANAGER.get_difficulty() == HARD) {
                arg_index = 3;
            } else {
                arg_index = 4;
            }
            float speed1 = this->get_float_arg(arg_index);
            this->shooters[slot].speed1 = speed1;
            if (GAME_MANAGER.get_difficulty() == EASY) {
                arg_index = 5;
            } else if (GAME_MANAGER.get_difficulty() == NORMAL) {
                arg_index = 6;
            } else if (GAME_MANAGER.get_difficulty() == HARD) {
                arg_index = 7;
            } else {
                arg_index = 8;
            }
            float speed2 = this->get_float_arg(arg_index);
            this->shooters[slot].speed2 = speed2;
            break;
        }
        case bullet_count_difficulty: { // 625
            int32_t slot = this->get_int_arg(0);
            int32_t arg_index;
            if (GAME_MANAGER.get_difficulty() == EASY) {
                arg_index = 1;
            } else if (GAME_MANAGER.get_difficulty() == NORMAL) {
                arg_index = 2;
            } else if (GAME_MANAGER.get_difficulty() == HARD) {
                arg_index = 3;
            } else {
                arg_index = 4;
            }
            int32_t count1 = this->get_int_arg(arg_index);
            this->shooters[slot].count1 = count1;
            if (GAME_MANAGER.get_difficulty() == EASY) {
                arg_index = 5;
            } else if (GAME_MANAGER.get_difficulty() == NORMAL) {
                arg_index = 6;
            } else if (GAME_MANAGER.get_difficulty() == HARD) {
                arg_index = 7;
            } else {
                arg_index = 8;
            }
            int32_t count2 = this->get_int_arg(arg_index);
            this->shooters[slot].count2 = count2;
            break;
        }
        case bullet_speed_rank_3: { // 617
            int32_t slot = this->get_int_arg(0);
            int32_t arg1_index;
            int32_t arg2_index;
            if (GAME_MANAGER.globals.get_rank() >= 512) {
                arg1_index = 5;
                arg2_index = 6;
            }
            else if (GAME_MANAGER.globals.get_rank() >= -512) {
                arg1_index = 3;
                arg2_index = 4;
            }
            else {
                arg1_index = 1;
                arg2_index = 2;
            }
            float speed1 = this->get_float_arg(arg1_index);
            this->shooters[slot].speed1 = speed1;
            float speed2 = this->get_float_arg(arg2_index);
            this->shooters[slot].speed2 = speed2;
            break;
        }
        case bullet_speed_rank_5: { // 618
            int32_t slot = this->get_int_arg(0);
            int32_t arg1_index;
            int32_t arg2_index;
            if (GAME_MANAGER.globals.get_rank() >= 600) {
                arg1_index = 9;
                arg2_index = 10;
            }
            else if (GAME_MANAGER.globals.get_rank() >= 200) {
                arg1_index = 7;
                arg2_index = 8;
            }
            else if (GAME_MANAGER.globals.get_rank() >= -200) {
                arg1_index = 5;
                arg2_index = 6;
            }
            else if (GAME_MANAGER.globals.get_rank() >= -600) {
                arg1_index = 3;
                arg2_index = 4;
            }
            else {
                arg1_index = 1;
                arg2_index = 2;
            }
            float speed1 = this->get_float_arg(arg1_index);
            this->shooters[slot].speed1 = speed1;
            float speed2 = this->get_float_arg(arg2_index);
            this->shooters[slot].speed2 = speed2;
            break;
        }
        case bullet_speed_rank_lerp: { // 619
            int32_t slot = this->get_int_arg(0);
            float speed1_low = this->get_float_arg(1);
            float speed2_low = this->get_float_arg(2);
            float speed1_high = this->get_float_arg(3);
            float speed2_high = this->get_float_arg(4);
            this->shooters[slot].speed1 = GAME_MANAGER.globals.rank_lerp(speed1_low, speed1_high);
            this->shooters[slot].speed2 = GAME_MANAGER.globals.rank_lerp(speed2_low, speed2_high);
            break;
        }
        case bullet_count_rank_3: { // 620
            int32_t slot = this->get_int_arg(0);
            int32_t arg1_index;
            int32_t arg2_index;
            if (GAME_MANAGER.globals.get_rank() >= 512) {
                arg1_index = 5;
                arg2_index = 6;
            }
            else if (GAME_MANAGER.globals.get_rank() >= -512) {
                arg1_index = 3;
                arg2_index = 4;
            }
            else {
                arg1_index = 1;
                arg2_index = 2;
            }
            int32_t count1 = this->get_int_arg(arg1_index);
            this->shooters[slot].count1 = count1;
            int32_t count2 = this->get_int_arg(arg2_index);
            this->shooters[slot].count2 = count2;
            break;
        }
        case bullet_count_rank_5: { // 621
            int32_t slot = this->get_int_arg(0);
            int32_t arg1_index;
            int32_t arg2_index;
            if (GAME_MANAGER.globals.get_rank() >= 600) {
                arg1_index = 9;
                arg2_index = 10;
            }
            else if (GAME_MANAGER.globals.get_rank() >= 200) {
                arg1_index = 7;
                arg2_index = 8;
            }
            else if (GAME_MANAGER.globals.get_rank() >= -200) {
                arg1_index = 5;
                arg2_index = 6;
            }
            else if (GAME_MANAGER.globals.get_rank() >= -600) {
                arg1_index = 3;
                arg2_index = 4;
            }
            else {
                arg1_index = 1;
                arg2_index = 2;
            }
            int32_t count1 = this->get_int_arg(arg1_index);
            this->shooters[slot].count1 = count1;
            int32_t count2 = this->get_int_arg(arg2_index);
            this->shooters[slot].count2 = count2;
            break;
        }
        case bullet_count_rank_lerp: { // 619
            int32_t slot = this->get_int_arg(0);
            int32_t count1_low = this->get_int_arg(1);
            int32_t count2_low = this->get_int_arg(2);
            int32_t count1_high = this->get_int_arg(3);
            int32_t count2_high = this->get_int_arg(4);
            this->shooters[slot].count1 = GAME_MANAGER.globals.rank_lerp(count1_low, count1_high);
            this->shooters[slot].count2 = GAME_MANAGER.globals.rank_lerp(count2_low, count2_high);
            break;
        }
        case shoot_aim_mode: { // 607
            int32_t slot = this->get_int_arg(0);
            int32_t mode = this->get_int_arg(1);
            this->shooters[slot].aim_mode = mode;
            break;
        }
        case bullet_sound: { // 608
            int32_t slot = this->get_int_arg(0);
            int32_t shoot_sound = this->get_int_arg(1);
            this->shooters[slot].shoot_sound = shoot_sound;
            int32_t transform_sound = this->get_int_arg(2);
            this->shooters[slot].transform_sound = transform_sound;
            break;
        }
        case bullet_effects: case bullet_effects_ex: // 609, 610
        case bullet_effects_add: case bullet_effects_add_ex: // 611, 612
        {
            // BUG: this is the argument used to select
            // a shooter, not the effect index.
            int32_t shooter_slot = this->get_int_arg(0);
            if (shooter_slot >= BULLET_EFFECT_MAX || shooter_slot < 0) {
                DebugLogger::__debug_log_stub_3("error et_ex over \n");
                shooter_slot = 0;
            }

            opcode = current_instruction->opcode;

            int32_t effect_arg_index; // ESI
            if (
                opcode != bullet_effects_add && // 611
                opcode != bullet_effects_add_ex // 612
            ) {
                effect_arg_index = 2;
            } else {
                effect_arg_index = 1;
            }

            int32_t effect_slot;
            if (
                opcode != bullet_effects_add && // 611
                opcode != bullet_effects_add_ex // 612
            ) {
                effect_slot = this->get_int_arg(1);
            } else {
                effect_slot = this->bullet_effect_indices[shooter_slot];
            }

            int32_t async = this->get_int_arg(effect_arg_index);
            ++effect_arg_index;
            this->shooters[shooter_slot].effects[effect_slot].async = async;

            int32_t type = this->get_int_arg(effect_arg_index);
            ++effect_arg_index;
            this->shooters[shooter_slot].effects[effect_slot].type = (BulletEffectType)type;

            int32_t A = this->get_int_arg(effect_arg_index);
            ++effect_arg_index;
            this->shooters[shooter_slot].effects[effect_slot].int_values[0] = A;
            int32_t B = this->get_int_arg(effect_arg_index);
            ++effect_arg_index;
            this->shooters[shooter_slot].effects[effect_slot].int_values[1] = B;

            opcode = current_instruction->opcode;
            if (
                opcode == bullet_effects_ex || // 610
                opcode == bullet_effects_add_ex // 612
            ) {
                int32_t C = this->get_int_arg(effect_arg_index);
                ++effect_arg_index;
                this->shooters[shooter_slot].effects[effect_slot].int_values[2] = C;
                int32_t D = this->get_int_arg(effect_arg_index);
                ++effect_arg_index;
                this->shooters[shooter_slot].effects[effect_slot].int_values[2] = D;
            }

            float R = this->get_float_arg(effect_arg_index);
            ++effect_arg_index;
            this->shooters[shooter_slot].effects[effect_slot].float_values[0] = R;
            float S = this->get_float_arg(effect_arg_index);
            ++effect_arg_index;
            this->shooters[shooter_slot].effects[effect_slot].float_values[1] = S;

            opcode = current_instruction->opcode;
            if (
                opcode == bullet_effects_ex || // 610
                opcode == bullet_effects_add_ex // 612
            ) {
                float M = this->get_float_arg(effect_arg_index);
                ++effect_arg_index;
                this->shooters[shooter_slot].effects[effect_slot].float_values[2] = M;
                float N = this->get_float_arg(effect_arg_index);
                ++effect_arg_index;
                this->shooters[shooter_slot].effects[effect_slot].float_values[3] = N;
            }

            opcode = current_instruction->opcode;
            if (
                opcode != bullet_effects_add && // 611
                opcode != bullet_effects_add_ex // 612
            ) {
                this->bullet_effect_indices[shooter_slot] = effect_slot + 1;
            } else {
                ++this->bullet_effect_indices[shooter_slot];
            }
            break;
        }
        case __bullet_effects_prev: { // 641
            int32_t slot = this->get_int_arg(0);
            if (--this->bullet_effect_indices[slot] < 0) {
                this->bullet_effect_indices[slot] = 0;
            }
            break;
        }
        case bullet_effects_set_string: { // 640
            int32_t shooter_slot = this->get_int_arg(0);
            int32_t effect_slot = this->get_int_arg(1);
            this->shooters[shooter_slot].effects[effect_slot].string = StringArg(2);
            break;
        }
        case laser_size_data: { // 700
            int32_t slot = this->get_int_arg(0);
            float length = this->get_float_arg(1);
            this->shooters[slot].position.x = length;
            float B = this->get_float_arg(2);
            this->shooters[slot].position.y = B;
            float C = this->get_float_arg(3);
            this->shooters[slot].position.z = C;
            float width = this->get_float_arg(4);
            this->shooters[slot].width = width;
            break;
        }
        case laser_timing_data: { // 701
            int32_t slot = this->get_int_arg(0);
            int32_t start_time = this->get_int_arg(1);
            this->shooters[slot].start_time = start_time;
            int32_t A = this->get_int_arg(2);
            this->shooters[slot].__transform_A = A;
            int32_t duration = this->get_int_arg(3);
            this->shooters[slot].duration = duration;
            int32_t B = this->get_int_arg(4);
            this->shooters[slot].__transform_B = B;
            int32_t __laser_flags = this->get_int_arg(5);
            this->shooters[slot].__laser_flags = __laser_flags;
            break;
        }
        case callback_ex: { // 514
            const char* callback_sub = StringArg(4);
            int32_t chapter;
            if (
                GAME_MANAGER.__unknown_field_A_is_2() &&
                this->is_boss
            ) {
                // This section is very strange.
                // The logic seems to be 100% redudant,
                // so maybe it's a leftover structure
                // from a previous game?
                callback_sub = "BossDead";
                if (this->boss_id == 0) {
                    if (
                        GAME_MANAGER.get_current_stage() == 7 &&
                        GAME_MANAGER.get_chapter() < 41
                    ) {
                        callback_sub = "MBossDead";
                    }
                }
                else {
                    if (
                        GAME_MANAGER.get_current_stage() == 7 &&
                        GAME_MANAGER.get_chapter() < 41
                    ) {
                        callback_sub = "MBossDead";
                    }
                }

                int32_t time_duration = this->get_int_arg(2);
                int32_t callback_index = this->get_int_arg(0);
                this->enemy()->set_callback(callback_index, 0, time_duration, callback_sub);
                callback_index = this->get_int_arg(0);
                this->enemy()->set_callback_time_sub_name(callback_index, "BossEscape");
            }
            else {
                int32_t time_duration = this->get_int_arg(2);
                int32_t life_threshold = this->get_int_arg(1);
                int32_t callback_index = this->get_int_arg(0);
                this->enemy()->set_callback(callback_index, life_threshold, time_duration, callback_sub);
            }
            break;
        }
        case timer_callback_sub: { // 521
            const char* callback_sub = StringArg(1);
            int32_t callback_index = this->get_int_arg(0);
            this->enemy()->set_callback_time_sub_name(callback_index, callback_sub);
            break;
        }
        case Opcode::death_callback_sub: { // 556
            const char* callback_sub = StringArg(0);
            byteloop_strcpy(this->death_callback_sub, callback_sub);
            break;
        }
        case bullet_cancel: // 613
            BULLET_MANAGER_PTR->cancel_all(0);
            LASER_MANAGER_PTR->cancel_all(1, 0);
            break;
        case effect_sound: { // 516
            Float3* position = &this->get_position();
            int32_t sound = this->get_int_arg(0);
            SOUND_MANAGER.play_sound_positioned(sound, position->x);
            break;
        }
        case effect_screen_shake: { // 517
            // TODO
            break;
        }
        case msg_read: { // 518
            // TODO
        }
            [[fallthrough]];
        case enemy_kill_all: // 525
            ENEMY_MANAGER_PTR->kill_all();
            break;
        case msg_wait: // 519
            if (
                GUI_PTR->msg_vm_active() &&
                !GUI_PTR->__get_enemy_appear_counter()
            ) {
                return -1;
            }
            break;
        case boss_wait: // 520
            if (
                ENEMY_MANAGER_PTR->get_boss_by_index(0) ||
                ENEMY_MANAGER_PTR->get_boss_by_index(1) ||
                ENEMY_MANAGER_PTR->get_boss_by_index(2)
            ) {
                return -1;
            }
            break;
        case enemy_flag_mirror2: // 558
            this->mirrored = this->get_int_arg(0);
            break;
        case spellcard_start: case spellcard_start_2: // 522, 528
        case spellcard_start_difficulty: case spellcard_start_difficulty_1: case spellcard_start_difficulty_2: // 537, 538, 539
        {
            // TODO
            break;
        }
        case boss_timer_clear: // 513
            this->ecl_time.reset(); // This is definitely the right offset...
            break;
        case __enemy_flag_armored: // 568
            this->life.set_spell(this->get_int_arg(0));
            break;
        case spellcard_end: // 523
            if (SPELLCARD_PTR->__get_flag_A() && SPELLCARD_PTR->__get_flag_B()) {
                ITEM_MANAGER_PTR->spawn_item(LifeFragmentItem, &this->get_position(), -HALF_PI_f, 2.2f, 60);
            }
            SPELLCARD_PTR->end_spell();
            this->life.set_spell(false);
            break;
        case spellcard_flag_timeout_set: // 542
            SPELLCARD_PTR->__set_timeout_flag();
            break;
        case __spellcard_flag_anm_unknown: // 543
            SPELLCARD_PTR->__sub_42D670();
            break;
        case __spellcard_flag_unknown_A: { // 567
            int32_t state = this->get_int_arg(0);
            SPELLCARD_PTR->__set_flag_H(state);
            break;
        }
        case enemy_flag_homing_disable: // 544
            this->homing_disable = this->get_int_arg(0);
            break;
        case laser_line_create: { // 702
            LaserLineParams params;

            int32_t slot = this->get_int_arg(0);
            memcpy(params.effects, this->shooters[slot].effects, sizeof(BulletEffectArgs[BULLET_EFFECT_MAX]));

            if (this->shooter_origins[slot].z > 0.9f) {
                params.position.set(this->shooter_offsets[slot].as2() + this->shooter_origins[slot].as2());
            } else {
                params.position = this->current_motion.get_position() + this->shooter_offsets[slot];
            }
            params.sprite = this->shooters[slot].type;
            params.color = this->shooters[slot].color;
            params.__angle_C = reduce_angle(this->shooters[slot].angle1);
            params.__speed_1 = this->shooters[slot].speed1;
            params.length = this->shooters[slot].position.x;
            params.flags = this->shooters[slot].__laser_flags | 1;
            params.__length_related = this->shooters[slot].position.y;
            params.shot_sound = this->shooters[slot].shoot_sound;
            params.__float_18 = this->shooters[slot].position.z;
            params.transform_sound = this->shooters[slot].transform_sound;
            params.width = this->shooters[slot].width;
            params.distance = this->shooters[slot].distance;
            LASER_MANAGER_PTR->allocate_new_laser(LineLaser, &params);
            break;
        }
        case laser_infinite_create: { // 703
            LaserInfiniteParams params;

            int32_t slot = this->get_int_arg(0);
            memcpy(params.effects, this->shooters[slot].effects, sizeof(BulletEffectArgs[BULLET_EFFECT_MAX]));

            if (this->shooter_origins[slot].z > 0.9f) {
                params.position.set(this->shooter_offsets[slot].as2() + this->shooter_origins[slot].as2());
            } else {
                params.position = this->current_motion.get_position() + this->shooter_offsets[slot];
            }
            // TODO

            params.laser_id = this->get_int_arg(1);
            LASER_MANAGER_PTR->allocate_new_laser(InfiniteLaser, &params);
            break;
        }
        case laser_beam_create: { // 713
            LaserBeamParams params;

            int32_t slot = this->get_int_arg(0);
            memcpy(params.effects, this->shooters[slot].effects, sizeof(BulletEffectArgs[BULLET_EFFECT_MAX]));

            if (this->shooter_origins[slot].z > 0.9f) {
                params.position.set(this->shooter_offsets[slot].as2() + this->shooter_origins[slot].as2());
            } else {
                params.position = this->current_motion.get_position() + this->shooter_offsets[slot];
            }
            params.color = this->shooters[slot].color;
            params.__angle_18 = reduce_angle(this->shooters[slot].angle1);
            params.__float_20 = this->shooters[slot].position.z;
            params.start_time = this->shooters[slot].start_time;
            params.distance = this->shooters[slot].distance;
            params.laser_id = this->get_int_arg(1);
            LASER_MANAGER_PTR->allocate_new_laser(BeamLaser, &params);
            break;
        }
        case laser_curve_create: { // 711
            LaserCurveParams params;

            int32_t slot = this->get_int_arg(0);
            memcpy(params.effects, this->shooters[slot].effects, sizeof(BulletEffectArgs[BULLET_EFFECT_MAX]));

            if (this->shooter_origins[slot].z > 0.9f) {
                params.position.set(this->shooter_offsets[slot].as2() + this->shooter_origins[slot].as2());
            } else {
                params.position = this->current_motion.get_position() + this->shooter_offsets[slot];
            }
            // TODO

            LASER_MANAGER_PTR->allocate_new_laser(CurvyLaser, &params);
            break;
        }
        case laser_clear: // 710
            while (LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0))) {
                laser->cancel(0, 0);
                laser->set_id(0);
            }
            break;
        case laser_offset: { // 704
            LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0));
            Float3 position;
            position.x = this->get_float_arg(1);
            position.y = this->get_float_arg(2);
            if (laser) {
                position.z = 0.0f;
                laser->set_position(&position);
            }
            break;
        }
        case laser_target: { // 705
            LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0));
            Float3 velocity;
            velocity.x = this->get_float_arg(1);
            velocity.y = this->get_float_arg(2);
            if (laser) {
                velocity.z = 0.0f;
                laser->set_velocity(&velocity);
            }
            break;
        }
        case laser_speed: // 706
            if (LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0))) {
                float speed = this->get_float_arg(1);
                laser->set_speed(speed);
            }
            break;
        case laser_width: // 707
            if (LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0))) {
                float width = this->get_float_arg(1);
                laser->set_width(width);
            }
            break;
        case laser_angle: // 708
            if (LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0))) {
                float angle = this->get_float_arg(1);
                laser->set_angle(angle);
            }
            break;
        case laser_rotate: // 709
            if (LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0))) {
                float angular_velocity = this->get_float_arg(1);
                laser->set_angular_velocity(angular_velocity);
            }
            break;
        case __laser_beam_flag_unknown_A: // 714
            if (LaserData* laser = LASER_MANAGER_PTR->get_laser_with_id(this->get_int_arg(0))) {
                int32_t state = this->get_int_arg(1);
                laser->__method_8(state);
            }
            break;
        case bullet_cancel_radius: { // 615
            float radius = this->get_float_arg(0);
            //BULLET_MANAGER_PTR->canel_radius(&this->get_position(), radius, 1, 0);
            LASER_MANAGER_PTR->cancel_in_radius(&this->get_position(), radius, 1, 1);
            break;
        }
        case __bullet_cancel_weak_rectangle: { // 712
            Float3 size;
            size.x = this->get_float_arg(0);
            size.y = this->get_float_arg(1);
            float rotation = this->anm_vms[0].get_vm_ptr()->get_z_rotation();
            //BULLET_MANAGER_PTR->canel_bullets_in_rectangle_as_bomb(&this->get_position(), &size, rotation, 1, 0);
            break;
        }
        case bullet_clear_radius: { // 615
            float radius = this->get_float_arg(0);
            //BULLET_MANAGER_PTR->canel_radius(&this->get_position(), radius, 0, 0);
            LASER_MANAGER_PTR->cancel_in_radius(&this->get_position(), radius, 0, 1);
            break;
        }
        case bullet_cancel_weak_radius: { // 635
            float radius = this->get_float_arg(0);
            //BULLET_MANAGER_PTR->canel_radius_as_bomb(&this->get_position(), radius, 1, 99999, 0);
            LASER_MANAGER_PTR->cancel_in_radius(&this->get_position(), radius, 1, 1);
            break;
        }
        case bullet_clear_weak_radius: { // 636
            float radius = this->get_float_arg(0);
            //BULLET_MANAGER_PTR->canel_radius_as_bomb(&this->get_position(), radius, 0, 99999, 0);
            LASER_MANAGER_PTR->cancel_in_radius(&this->get_position(), radius, 0, 1);
            break;
        }
        case chapter_set: { // 524
            int32_t chapter = this->get_int_arg(0);
            GAME_THREAD_PTR->set_chapter(chapter);
            this->chapter = chapter;
            this->chapter_spawn_weight = 0;
            break;
        }
        case set_float_rank_3: // 529
            if (GAME_MANAGER.globals.get_rank() >= 512) {
                *this->get_float_ptr_arg(0) = this->get_float_arg(2);
            }
            // BUG: Middle case is optimized out as impossible,
            // likely because of a missing negative sign.
            else if (GAME_MANAGER.globals.get_rank() >= 512) {
                *this->get_float_ptr_arg(0) = this->get_float_arg(1);
            }
            else {
                *this->get_float_ptr_arg(0) = this->get_float_arg(0);
            }
            break;
        case set_float_rank_5: // 530
            if (GAME_MANAGER.globals.get_rank() >= 600) {
                *this->get_float_ptr_arg(0) = this->get_float_arg(4);
            }
            else if (GAME_MANAGER.globals.get_rank() >= 200) {
                *this->get_float_ptr_arg(0) = this->get_float_arg(3);
            }
            else if (GAME_MANAGER.globals.get_rank() >= -200) {
                *this->get_float_ptr_arg(0) = this->get_float_arg(2);
            }
            else if (GAME_MANAGER.globals.get_rank() >= -400) {
                *this->get_float_ptr_arg(0) = this->get_float_arg(1);
            }
            else {
                *this->get_float_ptr_arg(0) = this->get_float_arg(0);
            }
            break;
        case math_float_rank_lerp: { // 531
            float low = this->get_float_arg(1);
            float high = this->get_float_arg(2);
            *this->get_float_ptr_arg(0) = GAME_MANAGER.globals.rank_lerp(low, high);
            break;
        }
        case set_int_rank_3: // 532
            if (GAME_MANAGER.globals.get_rank() >= 512) {
                *this->get_int_ptr_arg(0) = this->get_int_arg(2);
            }
            // BUG: Middle case is optimized out as impossible,
            // likely because of a missing negative sign.
            else if (GAME_MANAGER.globals.get_rank() >= 512) {
                *this->get_int_ptr_arg(0) = this->get_int_arg(1);
            }
            else {
                *this->get_int_ptr_arg(0) = this->get_int_arg(0);
            }
            break;
        case set_int_rank_5: // 533
            if (GAME_MANAGER.globals.get_rank() >= 600) {
                *this->get_int_ptr_arg(0) = this->get_int_arg(4);
            }
            else if (GAME_MANAGER.globals.get_rank() >= 200) {
                *this->get_int_ptr_arg(0) = this->get_int_arg(3);
            }
            else if (GAME_MANAGER.globals.get_rank() >= -200) {
                *this->get_int_ptr_arg(0) = this->get_int_arg(2);
            }
            else if (GAME_MANAGER.globals.get_rank() >= -400) {
                *this->get_int_ptr_arg(0) = this->get_int_arg(1);
            }
            else {
                *this->get_int_ptr_arg(0) = this->get_int_arg(0);
            }
            break;
        case math_int_rank_lerp: { // 534
            int32_t low = this->get_int_arg(1);
            int32_t high = this->get_int_arg(2);
            *this->get_int_ptr_arg(0) = GAME_MANAGER.globals.rank_lerp(low, high);
            break;
        }
        case boss_set_life_count: { // 540
            int32_t lives = this->get_int_arg(0);
            GUI_PTR->__set_field_170(lives);
            break;
        }
        case laser_clear_all: // 545
            LASER_MANAGER_PTR->clear_all();
            break;
        case enemy_bomb_shield: { // 546
            this->bomb_shield = this->get_int_arg(0);
            int32_t anm_script = this->get_int_arg(1);
            this->disable_hitbox = false;
            this->bomb_shield_active = false;
            this->bombshield_on_anm = anm_script;
            this->bombshield_off_anm = this->current_anm_script;
            break;
        }
        case enemy_limit_set: { // 559
            int32_t limit = this->get_int_arg(0);
            ENEMY_MANAGER_PTR->set_enemy_limit(limit);
            break;
        }
        case game_speed_set: { // 547
            float speed = this->get_float_arg(0);
            GAME_SPEED.set(speed);
            break;
        }
        case set_float_angle_to_player_from_point: { // 623
            float x_pos = PLAYER_PTR->get_position()->x - this->get_float_arg(0);
            float y_pos = PLAYER_PTR->get_position()->y - this->get_float_arg(1);
            *this->get_float_ptr_arg(0) = zatan2f(y_pos, x_pos);
            break;
        }
        case ecl_time_sub_difficulty: { // 548
            int32_t easy = this->get_int_arg(0);
            int32_t normal = this->get_int_arg(1);
            int32_t hard = this->get_int_arg(2);
            int32_t lunatic = this->get_int_arg(3);
            if (GAME_MANAGER.get_difficulty() == EASY) {
                this->enemy()->subtract_time_float(easy);
            } else if (GAME_MANAGER.get_difficulty() == NORMAL) {
                this->enemy()->subtract_time_float(normal);
            } else if (GAME_MANAGER.get_difficulty() == HARD) {
                this->enemy()->subtract_time_float(hard);
            } else {
                this->enemy()->subtract_time_float(lunatic);
            }
            break;
        }
        case enemy_fog_spawn: { // 629
            // TODO
            break;
        }
        case std_interrupt: { // 630
            int32_t interrupt = this->get_int_arg(0);
            // STAGE_PTR->std_interrupt(interrupt);
            break;
        }
        case std_fog_interp: { // 557
            // TODO
            break;
        }
        case __enemy_flag_unknown_A: // 549
            this->__anm_related_flag_A = this->get_int_arg(0);
            break;
        case enemy_set_kill_id: // 550
            this->kill_id = this->get_int_arg(0);
            break;
        case enemy_id_exists: { // 555
            int32_t* write = this->get_int_ptr_arg(0);
            int32_t id = this->get_int_arg(1);
            *write = ENEMY_MANAGER_PTR->enemy_exists_with_id(id);
            break;
        }
        case enemy_id_get_position_crash: { // 801
            int32_t id = this->get_int_arg(2);
            Enemy* enemy = ENEMY_MANAGER_PTR->get_enemy_by_id(id);
            *this->get_float_ptr_arg(0) = enemy->get_current_motion()->position.x;
            *this->get_float_ptr_arg(1) = enemy->get_current_motion()->position.y;
            break;
        }
        case boss_callback: { // 802
            int32_t slot = this->get_int_arg(0);
            for (int32_t i = 0; i < 4; ++i) {
                Enemy* boss = ENEMY_MANAGER_PTR->get_boss_by_index(i);
                if (
                    i != this->boss_id &&
                    boss != NULL &&
                    boss->get_data().callbacks[slot].life >= 0 &&
                    boss->get_data().callbacks[slot].life_sub // BUG: This is checking if the array exists
                ) {
                    // this is definitely an inlined call to
                    // reinitialize_vm_with_sub of some sort,
                    // but getting the get_data() call to match is
                    // pain so I'm leaving it as-is
                    boss->cleanup_vm();
                    boss->initialize_vm();
                    boss->set_context_to_sub(boss->get_data().callbacks[slot].life_sub);
                }
            }
            break;
        }
        case enemy_id_change_sub: { // 800
            int32_t id = this->get_int_arg(0);
            if (Enemy* enemy = ENEMY_MANAGER_PTR->get_enemy_by_id(id)) {
                enemy->reinitialize_vm_with_sub(StringArg(1));
                //enemy->cleanup_vm();
                //enemy->initialize_vm();
                //enemy->set_context_to_sub(StringArg(1));
            }
            break;
        }
        case enemy_id_get_position: { // 324
            int32_t id = this->get_int_arg(2);
            if (Enemy* enemy = ENEMY_MANAGER_PTR->get_enemy_by_id(id)) {
                *this->get_float_ptr_arg(0) = enemy->get_current_motion()->position.x;
                *this->get_float_ptr_arg(1) = enemy->get_current_motion()->position.y;
            } else {
                *this->get_float_ptr_arg(0) = this->get_current_motion().position.x;
                *this->get_float_ptr_arg(1) = this->get_current_motion().position.y;
            }
            break;
        }
        case bullet_bounce_bounds_set: { // 560
            float height = this->get_float_arg(1);
            float width = this->get_float_arg(0);
            BULLET_MANAGER_PTR->__set_bounce_bounds(width, height);
            break;
        }
        case __enemy_id_set_flag_unknown_B: { // 340
            int32_t id = this->get_int_arg(0);
            if (Enemy* enemy = ENEMY_MANAGER_PTR->get_enemy_by_id(id)) {
                enemy->__set_unknown_flag_B();
            }
            break;
        }
        case __globals_flag_unknown_A: { // 1001
            int32_t state = this->get_int_arg(0);
            GAME_MANAGER.globals.__set_unknown_flag_A(state);
            break;
        }
    }
    return 0;
}

// 0x4B4144
static inline constexpr const char* TWO_LETTER_DIFFICULTY_NAMES[] = {
    "E ",
    "N ",
    "H ",
    "L ",
    "EX",
    "OD"
};

// 0x4B4160
static inline constexpr const char* THREE_LETTER_STAGE_NAMES[] = {
    "tst",
    "St1",
    "St2",
    "St3",
    "St4",
    "St5",
    "St6",
    "Ex ",
    "All",
    "ExA"
};

// 0x4B4190
static inline constexpr const char* SEVEN_LETTER_STAGE_NAMES[] = {
    "test   ",
    "Stage 1",
    "Stage 2",
    "Stage 3",
    "Stage 4",
    "Stage 5",
    "Stage 6",
    "Extra  ",
    "Clear  ",
    "ExClear"
};

// 0x4B41C0
static inline constexpr const char* SEVEN_LETTER_DIFFICULTY_NAMES[] = {
    "Easy   ",
    "Normal ",
    "Hard   ",
    "Lunatic",
    "Extra  ",
    "O.D.   ",
    "r" // This is definitely part of the table. Why?
};

// 0x4B41E0
static inline constexpr const char* SIX_LETTER_SHOTTYPE_NAMES[] = {
    "Reimu ",
    "Marisa",
    "Sakuya",
    "Sanae "
};

static inline constexpr uint32_t REPLAY_MAGIC = PackUInt('t', '1', '8', 'r');
static inline constexpr uint32_t REPLAY_USER_MAGIC = PackUInt('U', 'S', 'E', 'R');
static inline constexpr uint16_t REPLAY_VERSION_NUMBER = 6; // I'm just guessing at this

// size: 0x24
struct ReplayHeader {
    ZUNMagic magic; // 0x0
    uint16_t __version; // 0x4
    unknown_fields(0x6); // 0x6
    uint32_t file_size; // 0xC
    int __int_10; // 0x10
    unknown_fields(0x8); // 0x14
    uint32_t compressed_size; // 0x1C
    uint32_t uncompressed_size; // 0x20
    // 0x24

    inline void zero_contents() {
        zero_this_inline();
    }

    inline ReplayHeader(uint32_t magic, uint16_t version) {
        this->zero_contents();
        this->magic.as_uint = magic;
        this->__version = version;
        this->__int_10 = 256;
    }
};
#pragma region // ReplayHeader Verification
ValidateFieldOffset32(0x0, ReplayHeader, magic);
ValidateFieldOffset32(0x4, ReplayHeader, __version);
ValidateFieldOffset32(0x10, ReplayHeader, __int_10);
ValidateFieldOffset32(0x1C, ReplayHeader, compressed_size);
ValidateFieldOffset32(0x20, ReplayHeader, uncompressed_size);
ValidateStructSize32(0x24, ReplayHeader);
#pragma endregion

// size: 0xC+
struct ReplayUserData {
    ZUNMagic magic; // 0x0
    int32_t size; // 0x4
    uint8_t __byte_8; // 0x8
    probably_padding_bytes(3); // 0x9
    char text[]; // 0xC
};
#pragma region // ReplayUserData Verification
ValidateFieldOffset32(0x0, ReplayUserData, magic);
ValidateFieldOffset32(0x4, ReplayUserData, size);
ValidateFieldOffset32(0x8, ReplayUserData, __byte_8);
ValidateFieldOffset32(0xC, ReplayUserData, text);
ValidateStructSize32(0xC, ReplayUserData);
#pragma endregion

// size: 0x6
struct ReplayFrameInput {
    uint16_t current; // 0x0
    uint16_t rising_edge; // 0x2
    uint16_t falling_edge; // 0x4
    // 0x6

    inline bool is_end() const {
        return this->current == 0xFFFF && this->rising_edge == 0xFFFF && this->falling_edge == 0xFFFF;
    }
};
#pragma region // ReplayFrameInput Verification
ValidateFieldOffset32(0x0, ReplayFrameInput, current);
ValidateFieldOffset32(0x2, ReplayFrameInput, rising_edge);
ValidateFieldOffset32(0x4, ReplayFrameInput, falling_edge);
ValidateStructSize32(0x6, ReplayFrameInput);
#pragma endregion

static inline constexpr ReplayFrameInput REPLAY_INPUT_END = { 0xFFFF, 0xFFFF, 0xFFFF };

// size: 0x126C+
struct ReplayGamestate {
    int16_t stage_number; // 0x0
    uint16_t rng; // 0x2
    uint32_t input_count; // 0x4
    uint32_t extra_size; // 0x8
    int __dword_C; // 0xC
    int __dword_10; // 0x10
    unknown_fields(0x4); // 0x14
    int __dword_array_18[20]; // 0x18
    Globals globals; // 0x68
    int32_t cards_owned[0x100]; // 0x164
    int32_t card_cooldowns[0x100]; // 0x564
    int32_t card_selected; // 0x964
    Globals __globalsB; // 0x968
    int32_t __cards_ownedB[0x100]; // 0xA64
    int32_t __card_cooldownsB[0x100]; // 0xE64
    int32_t __card_selectedB; // 0x1264
    union {
        uint32_t flags; // 0x1268
        struct {
            uint32_t __unknown_flag_A : 1;
        };
    };
    unsigned char extra[]; // 0x126C

    inline void zero_contents() {
        zero_this();
    }

    inline ReplayGamestate() {
        this->zero_contents();
    }
};
#pragma region // ReplayGamestate Verification
ValidateFieldOffset32(0x0, ReplayGamestate, stage_number);
ValidateFieldOffset32(0x2, ReplayGamestate, rng);
ValidateFieldOffset32(0x4, ReplayGamestate, input_count);
ValidateFieldOffset32(0x8, ReplayGamestate, extra_size);
ValidateFieldOffset32(0xC, ReplayGamestate, __dword_C);
ValidateFieldOffset32(0x10, ReplayGamestate, __dword_10);
ValidateFieldOffset32(0x18, ReplayGamestate, __dword_array_18);
ValidateFieldOffset32(0x68, ReplayGamestate, globals);
ValidateFieldOffset32(0x164, ReplayGamestate, cards_owned);
ValidateFieldOffset32(0x564, ReplayGamestate, card_cooldowns);
ValidateFieldOffset32(0x964, ReplayGamestate, card_selected);
ValidateFieldOffset32(0x968, ReplayGamestate, __globalsB);
ValidateFieldOffset32(0xA64, ReplayGamestate, __cards_ownedB);
ValidateFieldOffset32(0xE64, ReplayGamestate, __card_cooldownsB);
ValidateFieldOffset32(0x1264, ReplayGamestate, __card_selectedB);
ValidateFieldOffset32(0x1268, ReplayGamestate, flags);
ValidateFieldOffset32(0x126C, ReplayGamestate, extra);
ValidateStructSize32(0x126C, ReplayGamestate);
#pragma endregion

// size: 0xC8
struct ReplayInfo {
    char name[9]; // 0x0
    unknown_fields(0x1); // 0x9
    union {
        uint8_t flags; // 0xA
        struct {
            uint8_t practice_mode : 1;
            uint8_t __unknown_flag_A : 1;
        };
    };
    unknown_fields(0x5); // 0xB
    time_t time; // 0x10 
    int32_t score; // 0x18
    Config config; // 0x1C
    float slowdown_rate; // 0xA4
    int32_t stage_count; // 0xA8
    int32_t character; // 0xAC
    int32_t shottype; // 0xB0
    int32_t difficulty; // 0xB4
    int32_t __end_stage; // 0xB8
    int32_t __int_BC; // 0xBC
    int32_t __ecl_var_9907; // 0xC0
    unknown_fields(0x4); // 0xC4
    // 0xC8

    inline void zero_contents() {
        zero_this();
    }

    inline ReplayInfo() {
        this->zero_contents();
    }

    inline int32_t shottype_index() const {
        return this->character + this->shottype * SHOTTYPES_PER_CHARACTER;
    }

private:
    // 0x457D80
    dllexport gnu_noinline void stdcall __print_in_menuA(int arg1, Float3* position, ReplayInfo* info) {
        tm* time = localtime(&info->time);

        ++arg1;
        if (!info->__unknown_flag_A) {
            ASCII_MANAGER_PTR->printf(position,
                "No.%.2d %s %.2d/%.2d/%.2d %s %s %s",
                arg1, info->name,
                time->tm_year % 100, time->tm_mon + 1, time->tm_mday,
                SIX_LETTER_SHOTTYPE_NAMES[info->shottype_index()],
                TWO_LETTER_DIFFICULTY_NAMES[info->difficulty],
                THREE_LETTER_STAGE_NAMES[info->__end_stage]
            );
        }
        else {
            ASCII_MANAGER_PTR->printf(position,
                "No.%.2d %s %.2d/%.2d/%.2d %s Sp %3d",
                arg1, info->name,
                time->tm_year % 100, time->tm_mon + 1, time->tm_mday,
                SIX_LETTER_SHOTTYPE_NAMES[info->shottype_index()],
                info->__ecl_var_9907 + 1
            );
        }
    }
public:

    inline void __print_in_menuA(int arg1, Float3* position) {
        return ReplayInfo::__print_in_menuA(arg1, position, this);
    }
};
#pragma region // ReplayFrameInput Verification
ValidateFieldOffset32(0x0, ReplayInfo, name);
ValidateFieldOffset32(0xA, ReplayInfo, flags);
ValidateFieldOffset32(0x10, ReplayInfo, time);
ValidateFieldOffset32(0x18, ReplayInfo, score);
ValidateFieldOffset32(0x1C, ReplayInfo, config);
ValidateFieldOffset32(0xA4, ReplayInfo, slowdown_rate);
ValidateFieldOffset32(0xA8, ReplayInfo, stage_count);
ValidateFieldOffset32(0xAC, ReplayInfo, character);
ValidateFieldOffset32(0xB0, ReplayInfo, shottype);
ValidateFieldOffset32(0xB4, ReplayInfo, difficulty);
ValidateFieldOffset32(0xB8, ReplayInfo, __end_stage);
ValidateFieldOffset32(0xBC, ReplayInfo, __int_BC);
ValidateFieldOffset32(0xC0, ReplayInfo, __ecl_var_9907);
ValidateStructSize32(0xC8, ReplayInfo);
#pragma endregion

// size: 0x28
struct ReplayStageData {
    ReplayFrameInput* input_start; // 0x0
    ReplayFrameInput* inputs_current; // 0x4
    uint8_t* fps_counts_start; // 0x8
    uint8_t* fps_counts_current; // 0xC
    ReplayGamestate* gamestate_start; // 0x10
    int32_t current_frame; // 0x14
    ZUNList<ReplayStageData> list_node; // 0x18
    // 0x28

    inline void zero_contents() {
        zero_this_inline();
    }

    // 0x4631F0
    ReplayStageData() {
        this->zero_contents();
        this->fps_counts_current = this->fps_counts_start;
        this->list_node.initialize_with(this);
    }

    // 0x463220
    ~ReplayStageData() {
        this->list_node.unlink();
    }
};
#pragma region // ReplayStageData Verification
ValidateFieldOffset32(0x0, ReplayStageData, input_start);
ValidateFieldOffset32(0x4, ReplayStageData, inputs_current);
ValidateFieldOffset32(0x8, ReplayStageData, fps_counts_start);
ValidateFieldOffset32(0xC, ReplayStageData, fps_counts_current);
ValidateFieldOffset32(0x10, ReplayStageData, gamestate_start);
ValidateFieldOffset32(0x14, ReplayStageData, current_frame);
ValidateFieldOffset32(0x18, ReplayStageData, list_node);
ValidateStructSize32(0x28, ReplayStageData);
#pragma endregion

// size: 0x18B4
struct ReplayChunk {
    ReplayFrameInput inputs[900]; // 0x0
    ReplayFrameInput* next_input_write_pos; // 0x1518
    uint8_t fps_counts[900]; // 0x151C
    uint8_t* next_fps_count_write_pos; // 0x18A0
    ZUNList<ReplayChunk> list_node; // 0x18A4
    // 0x18B4

    inline void zero_contents() {
        zero_this();
    }

    ReplayChunk() {
        this->zero_contents();
        this->next_input_write_pos = this->inputs;
        this->next_fps_count_write_pos = this->fps_counts;
        this->list_node.initialize_with(this);

    }

    inline void cleanup() {
        this->list_node.unlink();
    }

    // 0x463060
    dllexport bool write_input(uint16_t current, uint16_t rising_edge, uint16_t falling_edge) asm_symbol_rel(0x463060) {
        this->next_input_write_pos->current = current;
        this->next_input_write_pos->rising_edge = rising_edge;
        this->next_input_write_pos->falling_edge = falling_edge;
        ++this->next_input_write_pos;
        return this->input_buffer_full();
    }

    inline bool write_input(const ReplayFrameInput& input) {
        return this->write_input(input.current, input.rising_edge, input.falling_edge);
    }

    inline ptrdiff_t input_count() {
        return this->next_input_write_pos - this->inputs;
    }

    inline size_t input_size() {
        return this->input_count() * sizeof(ReplayFrameInput);
    }

    // Fun fact, if this actually overflows then the pointer is completely invalid anyway
    inline bool input_buffer_full() {
        return this->input_count() >= countof(this->inputs);
    }

    inline ptrdiff_t fps_count() {
        return this->next_fps_count_write_pos - this->fps_counts;
    }

    inline size_t fps_size() {
        return this->fps_count() * sizeof(uint8_t);
    }
};
#pragma region // ReplayChunk Verification
ValidateFieldOffset32(0x0, ReplayChunk, inputs);
ValidateFieldOffset32(0x1518, ReplayChunk, next_input_write_pos);
ValidateFieldOffset32(0x151C, ReplayChunk, fps_counts);
ValidateFieldOffset32(0x18A0, ReplayChunk, next_fps_count_write_pos);
ValidateFieldOffset32(0x18A4, ReplayChunk, list_node);
ValidateStructSize32(0x18B4, ReplayChunk);
#pragma endregion

typedef struct ReplayManager ReplayManager;
extern "C" {
    // 0x4CF40C
    extern ReplayManager* REPLAY_MANAGER_PTR asm("_REPLAY_MANAGER_PTR");
}

// size: 0x31C
struct ReplayManager : ZUNTask {
    // ZUNTask base; // 0x0
    ReplayMode mode; // 0xC
    int __dword_10; // 0x10
    ReplayHeader* header; // 0x14
    ReplayInfo* info; // 0x18
    ReplayGamestate* game_states[STAGE_COUNT]; // 0x1C
    ZUNListHead<ReplayChunk> chunk_lists[STAGE_COUNT]; // 0x3C
    ZUNList<ReplayChunk>* current_chunk_node; // 0xBC
    int __chunk_count; // 0xC0
    ReplayStageData stage_data[STAGE_COUNT]; // 0xC4
    void* file_buffer; // 0x204
    uint8_t __byte_208; // 0x208
    probably_padding_bytes(3); // 0x209
    int32_t __int_20C; // 0x20C
    UpdateFunc* on_tick_func_B; // 0x210
    int32_t stage_number; // 0x214
    union {
        uint32_t flags; // 0x218
        struct {
            uint32_t __unknown_flag_B : 1;
            uint32_t __unknown_flag_A : 1;
        };
    };
    char file_path[0x100]; // 0x21C
    // 0x31C

    inline void zero_contents() {
        zero_this();
    }

    ReplayManager() {
        this->zero_contents();
    }

    dllexport static UpdateFuncRet fastcall on_tick_A1(void* ptr) {
        ReplayManager* self = (ReplayManager*)ptr;
        if (GAME_THREAD_PTR) {
            INPUT_STATES[0].inputs_previous = INPUT_STATES[0].inputs_current;
            INPUT_STATES[0].inputs_current = (uint16_t)INPUT_STATES[0].hardware_inputs_current;
            __update_input0();
            if (
                SUPERVISOR.config.__unknown_flag_C &&
                INPUT_STATES[0].check_inputs(BUTTON_SHOOT) &&
                INPUT_STATES[0].inputs_held[BUTTON_SHOOT_INDEX] >= 10
            ) {
                INPUT_STATES[0].inputs_current |= BUTTON_FOCUS;
            }
            if (self->__int_20C >= 0 && !ABILITY_SHOP_PTR) {
                if (!(self->__int_20C % 30)) {
                    float fps_count_f = FPS_COUNTER_PTR->__float_30 + 0.5f;
                    int32_t fps_count = fps_count_f < 256.0f ? fps_count_f : 255;
                    ReplayChunk* cur_chunk = self->current_chunk_node->data;
                    *cur_chunk->next_fps_count_write_pos++ = fps_count;
                }
                if (self->current_chunk_node->data->write_input(INPUT_STATES[0].inputs_current, INPUT_STATES[0].inputs_rising_edge, INPUT_STATES[0].inputs_falling_edge)) {
                    self->current_chunk_node = self->allocate_chunk(self->stage_number);
                }
                ++self->__int_20C;
            }
        }
        return UpdateFuncNext;
    }

    dllexport static UpdateFuncRet fastcall on_tick_A2(void* ptr) {
        ReplayManager* self = (ReplayManager*)ptr;
        if (GAME_THREAD_PTR && !self->__unknown_flag_A) {
            if (self->stage_data[self->stage_number].current_frame < 0) {
                INPUT_STATES[0].inputs_current = 0;
                INPUT_STATES[0].inputs_rising_edge = 0;
                INPUT_STATES[0].inputs_falling_edge = 0;
            }
            else if (!ABILITY_SHOP_PTR) {
                if (self->stage_number >= 0) {
                    INPUT_STATES[0].inputs_previous = INPUT_STATES[0].inputs_current;
                    if (self->stage_data[self->stage_number].current_frame < self->stage_data[self->stage_number].gamestate_start->input_count) {
                        if (self->stage_data[self->stage_number].inputs_current->is_end()) {
                            INPUT_STATES[0].inputs_current = 0;
                            INPUT_STATES[0].inputs_rising_edge = 0;
                            INPUT_STATES[0].inputs_falling_edge = 0;
                            // some mess with the pause screen, probably just replay end
                            self->stage_number = -1;
                        }
                        else {
                            INPUT_STATES[0].inputs_current = self->stage_data[self->stage_number].inputs_current->current;
                            INPUT_STATES[0].inputs_rising_edge = self->stage_data[self->stage_number].inputs_current->rising_edge;
                            INPUT_STATES[0].inputs_falling_edge = self->stage_data[self->stage_number].inputs_current->falling_edge;
                            __update_input0();
                            self->__byte_208 = *self->stage_data[self->stage_number].fps_counts_current;
                            ++self->stage_data[self->stage_number].inputs_current;
                            if (!(self->__int_20C % 30)) {
                                ++self->stage_data[self->stage_number].fps_counts_current;
                            }
                        }
                    }
                    else {
                        INPUT_STATES[0].inputs_current = 0;
                        INPUT_STATES[0].inputs_rising_edge = 0;
                        INPUT_STATES[0].inputs_falling_edge = 0;
                    }
                    ++self->stage_data[self->stage_number].current_frame;
                    ++self->__int_20C;
                }
                else {
                    INPUT_STATES[0].inputs_current = 0;
                    INPUT_STATES[0].inputs_rising_edge = 0;
                    INPUT_STATES[0].inputs_falling_edge = 0;
                    ++self->__int_20C;
                }
            }
        }
        return UpdateFuncNext;
    }

    // 0x461DB0
    dllexport gnu_noinline UpdateFuncRet thiscall on_tick_B_impl() asm_symbol_rel(0x461DB0) {
        if (
            GAME_THREAD_PTR && this->mode == __replay_playback
        ) {
            if (!INPUT_STATES[0].check_hardware_inputs(BUTTON_SHOOT | BUTTON_DOWN)) {
                if (!this->__dword_10) {
                    SOUND_MANAGER.__unknown_sme_ptr_5704->__sub_48AF10(GAME_MANAGER.globals.__counter_14 / 60.0);
                }
                this->__dword_10 = 0;
            }
            else {
                if (this->__int_20C % 8) {
                    return UpdateFuncRestartTick;
                }
            }
        }
        return UpdateFuncNext;
    }

    // 0x462C30
    dllexport static UpdateFuncRet fastcall on_tick_B(void* ptr) asm_symbol_rel(0x462C30) {
        GameThread* game_thread_ptr = GAME_THREAD_PTR;
        if (game_thread_ptr && game_thread_ptr->skip_flag) {
            return UpdateFuncNext;
        }
        return ((ReplayManager*)ptr)->on_tick_B_impl();
    }

    // 0x462C50
    dllexport static UpdateFuncRet fastcall on_draw(void* ptr) asm_symbol_rel(0x462C50) {
        ReplayManager* self = (ReplayManager*)ptr;

        GameThread* game_thread_ptr = GAME_THREAD_PTR;
        if (game_thread_ptr && !game_thread_ptr->skip_flag) {
            switch (self->mode) {
                case __replay_recording:
                    break;
                case __replay_playback:
                    Float3 position = {
                        383.0f, 450.0f, 0.0f
                    };
                    float A = self->__byte_208;
                    D3DCOLOR color;
                    if (A < 30.0f) { // IDK why these were ever floats
                        color = PackD3DCOLOR(255, 80, 80, 255);
                    } else if (A < 50.0f) {
                        color = PackD3DCOLOR(255, 160, 160, 255);
                    } else {
                        color = PackD3DCOLOR(255, 255, 255, 255);
                    }
                    AsciiManager* ascii_manager = ASCII_MANAGER_PTR;
                    ascii_manager->color = color;
                    ascii_manager->printf(&position, "%3d", self->__byte_208);
                    ASCII_MANAGER_PTR->color = PackD3DCOLOR(255, 255, 255, 255);
            }
        }
        return UpdateFuncNext;
    }

    // 0x463180
    dllexport gnu_noinline void thiscall delete_chunk_list(int32_t stage_number) asm_symbol_rel(0x463180) {
        this->chunk_lists[stage_number].for_each_safe([](ReplayChunk* chunk) {
            chunk->cleanup();
            delete chunk;
        });
    }

    // 0x4630E0
    dllexport gnu_noinline ZUNList<ReplayChunk>* thiscall allocate_chunk(int32_t stage_number) asm_symbol_rel(0x4630E0) {
        ReplayChunk* chunk = new ReplayChunk();
        this->chunk_lists[stage_number].append_to_list_end(&chunk->list_node);
        ++this->__chunk_count;
        return &chunk->list_node;
    }

    // 0x4615E0
    dllexport gnu_noinline ZUNResult thiscall initialize(ReplayMode mode, const char* path) asm_symbol_rel(0x4615E0) {
        this->mode = mode;
        switch (mode) {
            case __replay_recording: {
                REPLAY_MANAGER_PTR = this;
                this->delete_chunk_list(GAME_MANAGER.globals.current_stage);
                this->current_chunk_node = this->allocate_chunk(GAME_MANAGER.globals.current_stage);
                this->header = new ReplayHeader(REPLAY_MAGIC, REPLAY_VERSION_NUMBER);
                this->info = new ReplayInfo();
                this->game_states[GAME_MANAGER.globals.current_stage] = new ReplayGamestate();
                ReplayGamestate* game_state = this->game_states[GAME_MANAGER.globals.current_stage];
                this->info->character = GAME_MANAGER.globals.character;
                this->info->shottype = GAME_MANAGER.globals.shottype;
                this->info->difficulty = GAME_MANAGER.globals.difficulty;
                this->info->practice_mode = GAME_MANAGER.__unknown_field_A & 1;
                this->info->__unknown_flag_A = GAME_MANAGER.__unknown_field_A_is_2();
                this->info->__ecl_var_9907 = GAME_MANAGER.globals.__ecl_var_9907;
                if (GameThread* game_thread_ptr = GAME_THREAD_PTR) {
                    this->info->config = game_thread_ptr->config;
                }
                game_state->stage_number = GAME_MANAGER.globals.current_stage;
                game_state->rng = REPLAY_RNG.value;
                REPLAY_RNG.index = 0;
                game_state->__unknown_flag_A = SUPERVISOR.__int_804 & 1;
                if (SUPERVISOR.__int_804) {
                    game_state->__dword_C = 0;
                    game_state->__dword_10 = 0;
                }
                game_state->globals = GAME_MANAGER.globals;

                size_t bs_value = 0;
                for (size_t i = 0; i < countof(game_state->__dword_array_18); ++i) {
                    game_state->__dword_array_18[i] = bs_value;
                    bs_value -= 0x21522153; // This is a suspiciously repetitve constant...
                }

                this->info->__int_BC = GAME_MANAGER.globals.continues;

                UpdateFunc* update_func = new UpdateFunc(&on_tick_A1, false, this);
                UpdateFuncRegistry::register_on_tick(update_func, 17);
                this->on_tick_func = update_func;
                update_func = new UpdateFunc(&on_tick_B, false, this);
                UpdateFuncRegistry::register_on_tick(update_func, 35);
                this->on_tick_func_B = update_func;
                update_func = new UpdateFunc(&on_draw, false, this);
                UpdateFuncRegistry::register_on_draw(update_func, 77);
                this->on_draw_func = update_func;

                this->stage_number = GAME_MANAGER.globals.current_stage;
                this->__int_20C = -1;
                return ZUN_SUCCESS;
            }
            case __replay_playback: {
                if (ZUN_SUCCEEDED(this->__load_from_path(path))) {
                    GAME_THREAD_PTR->config = this->info->config;
                    ReplayStageData& cur_stage_data = this->stage_data[GAME_MANAGER.globals.current_stage];
                    ReplayGamestate* game_state = cur_stage_data.gamestate_start;
                    cur_stage_data.inputs_current = cur_stage_data.input_start;
                    cur_stage_data.fps_counts_current = cur_stage_data.fps_counts_start;
                    cur_stage_data.current_frame = -1;
                    GAME_MANAGER.globals.character = this->info->character;
                    GAME_MANAGER.globals.shottype = this->info->shottype;
                    GAME_MANAGER.globals.difficulty = this->info->difficulty;
                    REPLAY_RNG.value = game_state->rng;
                    REPLAY_RNG.index = 0;
                    GAME_MANAGER.globals = game_state->globals;

                    int32_t A = GAME_MANAGER.globals.__ecl_var_9907;
                    if (A >= 0) {
                        if (GAME_MANAGER.globals.__unknown_field_A != 2) {
                            A = -1;
                        }
                        GAME_MANAGER.globals.__unknown_field_A = 2;
                    } else {
                        if (GAME_MANAGER.globals.__unknown_field_A != 2) {
                            A = -1;
                        }
                        GAME_MANAGER.globals.__unknown_field_A = 0;
                    }
                    GAME_MANAGER.globals.__ecl_var_9907 = A;

                    UpdateFunc* update_func = new UpdateFunc(&on_tick_A2, false, this);
                    UpdateFuncRegistry::register_on_tick(update_func, 17);
                    this->on_tick_func = update_func;
                    update_func = new UpdateFunc(&on_tick_B, false, this);
                    UpdateFuncRegistry::register_on_tick(update_func, 35);
                    this->on_tick_func_B = update_func;
                    update_func = new UpdateFunc(&on_draw, false, this);
                    UpdateFuncRegistry::register_on_draw(update_func, 77);
                    this->on_draw_func = update_func;

                    this->stage_number = -1;
                    return ZUN_SUCCESS;
                }
                break;
            }
            case __replay_mode_2: {
                if (ZUN_SUCCEEDED(this->__load_from_path(path))) {
                    return ZUN_SUCCESS;
                }
                break;
            }
        }
        return ZUN_ERROR;
    }

    // 0x462680
    dllexport gnu_noinline ZUNResult thiscall __load_from_path(const char* path) asm_symbol_rel(0x462680);

    // 0x461E90
    dllexport gnu_noinline ZUNResult thiscall __write_to_path(const char* path, const char* name, bool arg3, bool arg4) asm_symbol_rel(0x461E90);

    // 0x461CF0
    dllexport static gnu_noinline ReplayManager* fastcall __make_manager_for_path(const char* path) asm_symbol_rel(0x461CF0) {
        int32_t intA = 0; // WTF is this?
        ReplayManager* replay_manager = new ReplayManager();
        intA = -1;
        replay_manager->mode = __replay_mode_2;
        if (ZUN_FAILED(replay_manager->__load_from_path(path))) {
            delete replay_manager;
            return NULL;
        }
        return replay_manager;
    }
};
#pragma region // ReplayManager Verification
ValidateFieldOffset32(0x0, ReplayManager, task_flags);
ValidateFieldOffset32(0x4, ReplayManager, on_tick_func);
ValidateFieldOffset32(0x8, ReplayManager, on_draw_func);
ValidateFieldOffset32(0xC, ReplayManager, mode);
ValidateFieldOffset32(0x10, ReplayManager, __dword_10);
ValidateFieldOffset32(0x14, ReplayManager, header);
ValidateFieldOffset32(0x18, ReplayManager, info);
ValidateFieldOffset32(0x1C, ReplayManager, game_states);
ValidateFieldOffset32(0x3C, ReplayManager, chunk_lists);
ValidateFieldOffset32(0xBC, ReplayManager, current_chunk_node);
ValidateFieldOffset32(0xC0, ReplayManager, __chunk_count);
ValidateFieldOffset32(0xC4, ReplayManager, stage_data);
ValidateFieldOffset32(0x204, ReplayManager, file_buffer);
ValidateFieldOffset32(0x208, ReplayManager, __byte_208);
ValidateFieldOffset32(0x20C, ReplayManager, __int_20C);
ValidateFieldOffset32(0x210, ReplayManager, on_tick_func_B);
ValidateFieldOffset32(0x214, ReplayManager, stage_number);
ValidateFieldOffset32(0x218, ReplayManager, flags);
ValidateFieldOffset32(0x21C, ReplayManager, file_path);
ValidateStructSize32(0x31C, ReplayManager);
#pragma endregion

static inline bool is_replay() {
    return REPLAY_MANAGER_PTR->mode == __replay_playback;
}

typedef struct PauseMenu PauseMenu;
extern "C" {
    // 0x4CF40C
    extern PauseMenu* PAUSE_MENU_PTR asm("_PAUSE_MENU_PTR");
}

// size: 0x3F8
struct PauseMenu : ZUNTask {
    //ZUNTask base; // 0x0
    Timer __timer_C; // 0xC
    Timer __timer_20; // 0x20
    MenuSelect __menu_select_34; // 0x34
    MenuSelect __menu_select_10C; // 0x10C
    AnmID __vm_id_1E4; // 0x1E4
    AnmID __vm_id_1E8; // 0x1E8
    int __dword_1EC; // 0x1EC
    int __dword_1F0; // 0x1F0
    int __dword_1F4; // 0x1F4
    int __dword_1F8; // 0x1F8
    int __dword_1FC; // 0x1FC
    unknown_fields(0x8); // 0x200
    int __dword_208; // 0x208
    ReplayManager* __replay_manager_array_20C[25]; // 0x20C
    unknown_fields(0x70); // 0x270
    float __float_2E0; // 0x2E0
    unknown_fields(0x10C); // 0x2E4
    union {
        uint32_t __flags_3F0; // 0x3F0
        struct {
            uint32_t __unknown_bitfield_A : 2;
            uint32_t __unknown_flag_B : 1;
        };
    };
    AnmLoaded* __anm_loaded_3F4; // 0x3F4
    // 0x3F8


    inline void zero_contents() {
        zero_this();
    }

    inline PauseMenu() {
        this->zero_contents();
        this->__unknown_task_flag_A = true;
    }
};
#pragma region // PauseMenu Verification
ValidateFieldOffset32(0x0, PauseMenu, task_flags);
ValidateFieldOffset32(0x4, PauseMenu, on_tick_func);
ValidateFieldOffset32(0x8, PauseMenu, on_draw_func);
ValidateFieldOffset32(0xC, PauseMenu, __timer_C);
ValidateFieldOffset32(0x20, PauseMenu, __timer_20);
ValidateFieldOffset32(0x34, PauseMenu, __menu_select_34);
ValidateFieldOffset32(0x10C, PauseMenu, __menu_select_10C);
ValidateFieldOffset32(0x1E4, PauseMenu, __vm_id_1E4);
ValidateFieldOffset32(0x1E8, PauseMenu, __vm_id_1E8);
ValidateFieldOffset32(0x1EC, PauseMenu, __dword_1EC);
ValidateFieldOffset32(0x1F0, PauseMenu, __dword_1F0);
ValidateFieldOffset32(0x1F4, PauseMenu, __dword_1F4);
ValidateFieldOffset32(0x1F8, PauseMenu, __dword_1F8);
ValidateFieldOffset32(0x1FC, PauseMenu, __dword_1FC);
ValidateFieldOffset32(0x208, PauseMenu, __dword_208);
ValidateFieldOffset32(0x20C, PauseMenu, __replay_manager_array_20C);
ValidateFieldOffset32(0x2E0, PauseMenu, __float_2E0);
ValidateFieldOffset32(0x3F0, PauseMenu, __flags_3F0);
ValidateFieldOffset32(0x3F4, PauseMenu, __anm_loaded_3F4);
ValidateStructSize32(0x3F8, PauseMenu);
#pragma endregion

// 0x401860
dllexport gnu_noinline uint32_t get_hardware_xinput() asm_symbol_rel(0x401860);
dllexport gnu_noinline uint32_t get_hardware_xinput() {
    return INPUT_STATES[0].get_xinput(0);
}

// 0x401650
dllexport gnu_noinline uint32_t get_joypad_input(uint32_t buttons) asm_symbol_rel(0x401650);
dllexport gnu_noinline uint32_t get_joypad_input(uint32_t buttons) {
    return INPUT_STATES[0].get_joypad(buttons);
}

// 0x401C50
dllexport gnu_noinline uint32_t get_hardware_inputs() asm_symbol_rel(0x401C50);
dllexport gnu_noinline uint32_t get_hardware_inputs() {
    uint32_t buttons = get_hardware_xinput();
    if (WINDOW_DATA.window_active) {
        buttons = INPUT_STATES[0].get_keyboard(buttons);
    }
    buttons = get_joypad_input(buttons);
    INPUT_STATES[0].hardware_inputs_previous = INPUT_STATES[0].hardware_inputs_current;
    INPUT_STATES[0].hardware_inputs_current = buttons;
    INPUT_STATES[0].__update_hardware_input();
    return buttons;
}

// 0x486140
dllexport gnu_noinline int32_t thiscall AnmManager::__sub_486140(AnmImage* image, uint32_t format_index, uint32_t entry_index, int32_t width, int32_t height, int32_t offset_x, int32_t offset_y) {
    image->__unknown_flag_A = false;
    LPDIRECT3DTEXTURE9 texture;
    if (D3DXCreateTextureFromFileInMemoryEx(
        SUPERVISOR.d3d_device,
        image->file,
        image->file_size,
        0, 0, 0, 0,
        D3DFORMAT_TABLE[format_index],
        D3DPOOL_MANAGED,
        D3DX_FILTER_NONE,
        D3DX_DEFAULT,
        0,
        NULL,
        NULL,
        &texture
    ) != D3D_OK) {
        return ZUN_ERROR;
    }
    LPDIRECT3DSURFACE9 surface;
    texture->GetSurfaceLevel(0, &surface);
    D3DSURFACE_DESC surface_desc;
    surface->GetDesc(&surface_desc);
    this->__screw_with_texture_bits(texture);
    float floatA = WINDOW_DATA.__float_2070;
    if (
        surface_desc.Width == width && surface_desc.Height == height &&
        (!image->entry->low_res_scale || !(floatA < 2.0f))
    ) {
        image->d3d_texture = texture;
        SAFE_RELEASE(surface);
    } else {
        uint32_t right = width;
        uint32_t bottom = height;
        if (width + offset_x > surface_desc.Width) {
            right = surface_desc.Width - offset_x;
        }
        if (height + offset_y > surface_desc.Height) {
            bottom = surface_desc.Height - offset_y;
        }
        if (image->entry->low_res_scale && floatA < 2.0f) {
            width = (float)width * floatA * 0.5f;
            height = (float)height * floatA * 0.5f;
        }
        D3DXCreateTexture(
            SUPERVISOR.d3d_device,
            width, height,
            0, 0,
            D3DFORMAT_TABLE[format_index], D3DPOOL_MANAGED,
            &image->d3d_texture
        );
        LPDIRECT3DSURFACE9 surface2;
        image->d3d_texture->GetSurfaceLevel(0, &surface2);
        RECT src_rect;
        src_rect.left = offset_x;
        src_rect.right = offset_x + right;
        src_rect.top = offset_y;
        src_rect.bottom = offset_y + bottom;
        DWORD filter = !image->entry->low_res_scale || !(WINDOW_DATA.__float_2070 < 2.0f) ? D3DX_FILTER_NONE : D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR_U | D3DX_FILTER_MIRROR_V;
        D3DXLoadSurfaceFromSurface(
            surface2, NULL, NULL,
            surface, NULL, &src_rect, filter, 0
        );
        SAFE_RELEASE(surface);
        SAFE_RELEASE(surface2);
        SAFE_RELEASE(texture);
    }
    this->__screw_with_texture_bits(image->d3d_texture);
    uint32_t bytes_per_pixel = D3DFORMAT_SIZES_TABLE[format_index];
    image->bytes_per_pixel = bytes_per_pixel;
    return bytes_per_pixel * width * height;
}

// 0x486390
dllexport gnu_noinline int32_t stdcall __sub_486390(AnmImage* image, AnmTexture* texture, uint32_t format_index, uint32_t width, uint32_t height) asm_symbol_rel(0x486390);
dllexport gnu_noinline int32_t stdcall __sub_486390(AnmImage* image, AnmTexture* texture, uint32_t format_index, uint32_t width, uint32_t height) {
    image->__unknown_flag_A = false;
    AnmEntry* entry = image->entry;
    int32_t texture_width = texture->width;
    int32_t texture_height = texture->height;
    RECT rectA = {
        .left = 0,
        .top = 0,
        .right = texture_width,
        .bottom = texture_width
    };
    RECT rectB = {
        .left = 0,
        .top = 0,
        .right = texture_width,
        .bottom = texture_width
    };
    float floatA = WINDOW_DATA.__float_2070;
    if (entry->low_res_scale && floatA < 2.0f) {
        rectB.right = (float)texture_width * floatA * 0.5f;
    }
    // TODO
}

// 0x486560
dllexport gnu_noinline int32_t stdcall __create_render_target_texture(AnmImage* image, uint32_t width, uint32_t height) asm_symbol_rel(0x486560);
dllexport gnu_noinline int32_t stdcall __create_render_target_texture(AnmImage* image, uint32_t width, uint32_t height) {
    SUPERVISOR.d3d_device->CreateTexture(
        width, height,
        1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        &image->d3d_texture, NULL
    );
    image->bytes_per_pixel = (SUPERVISOR.present_parameters.BackBufferFormat == D3DFMT_A8R8G8B8) * 2 + 2;
    return 0;
}

static inline int32_t stdcall __create_normal_texture(AnmImage* image, uint32_t format_index, uint32_t width, uint32_t height) {
    image->__unknown_flag_A = false;
    D3DXCreateTexture(
        SUPERVISOR.d3d_device,
        width, height,
        1, 0,
        D3DFORMAT_TABLE[format_index], D3DPOOL_MANAGED,
        &image->d3d_texture
    );
    uint32_t bytes_per_pixel = D3DFORMAT_SIZES_TABLE[format_index];
    image->bytes_per_pixel = bytes_per_pixel;
    return bytes_per_pixel * width * height;
}

// 0x486BC0
dllexport gnu_noinline int32_t thiscall AnmManager::__sub_486BC0(AnmLoaded* anm_loaded, uint32_t entry_index, uint32_t sprite_count, uint32_t script_count, AnmEntry* entry) {
    if (!entry) {
        LOG_BUFFER.write_error(JpEnStr("", "Can't load animation. data is lost or corrupted\r\n"));
        return -1;
    }
    if (entry->version != 8) {
        LOG_BUFFER.write_error(JpEnStr("", "different version of animation\r\n"));
        return -1;
    }
    anm_loaded->images[entry_index].entry = entry;
    int32_t image_size;
    if (!entry->has_data) {
        const char* image_filename = based_pointer<const char>(entry, entry->image_path_offset);
        if (image_filename[0] == '@') {
            if (image_filename[1] == 'R') {
                entry->width = WINDOW_DATA.__int_2050;
                entry->height = WINDOW_DATA.__int_2054;
                __create_render_target_texture(&anm_loaded->images[entry_index], WINDOW_DATA.__int_2050, WINDOW_DATA.__int_2054);
                goto skip_adding_image_size;
            }
            image_size = __create_normal_texture(&anm_loaded->images[entry_index], entry->format, entry->width, entry->height);
        } else {
            image_size = this->__sub_486140(&anm_loaded->images[entry_index], entry->format, entry_index, entry->width, entry->height, entry->offset_x, entry->offset_y);
            if (image_size < 0) {
                LOG_BUFFER.write_error(JpEnStr("", "Unable to create texture %s. data is lost or corrupted\r\n"), image_filename);
                return -1;
            }
        }
    } else {
        image_size = __sub_486390(&anm_loaded->images[entry_index], based_pointer<AnmTexture>(entry, entry->texture_data_offset), entry->format, entry->width, entry->height);
        if (image_size < 0) {
            LOG_BUFFER.write_error(JpEnStr("", "Unable to create texture. data is lost or corrupted\r\n"));
            return -1;
        }
    }
    // TODO
skip_adding_image_size:
}

// 0x454B20
dllexport gnu_noinline void thiscall Supervisor::__initialize_cameras() {
    this->cameras[2].position.x = 0.0f;
    this->cameras[2].position.y = 0.0f;
    this->cameras[2].position.z = 0.0f;
    this->cameras[2].facing.x = 0.0f;
    this->cameras[2].facing.y = 0.0f;
    this->cameras[2].rotation.x = 0.0f;
    this->cameras[2].rotation.y = 1.0f;
    StageCamera* camera2 = &this->cameras[2];
    camera2->viewport.X = 0;
    camera2->viewport.Y = 0;
    camera2->fov = 0.5235988f;
    float dumb_local = 0.0f;
    camera2->facing.z = dumb_local;
    dumb_local = 0.0f;
    camera2->rotation.z = dumb_local;
    camera2->viewport.Width = WINDOW_DATA.__int_2050;
    camera2->viewport.Height = WINDOW_DATA.__int_2054;
    camera2->__shaking_float3_A.x = 0.0f;
    camera2->__shaking_float3_A.y = 0.0f;
    camera2->viewport.MinZ = 0.0f;
    camera2->viewport.MaxZ = 1.0f;
    camera2->camera_index = 2;
    dumb_local = 0.0f;
    camera2->__shaking_float3_A.z = dumb_local;
    camera2->__shaking_float3_B.x = 0.0f;
    camera2->__shaking_float3_B.y = 0.0f;
    dumb_local = 0.0f;
    camera2->__shaking_float3_B.z = dumb_local;
    camera2->window_resolution.x = WINDOW_DATA.__int_2050;
    camera2->window_resolution.y = WINDOW_DATA.__int_2054;
    camera2->__viewport_10C = camera2->viewport;
    camera2->__int2_104.x = 0;
    camera2->__int2_104.y = 0;
    camera2->__viewport_10C.X = 0;
    camera2->__viewport_10C.Y = 0;
    camera2->__viewport_10C.Width = WINDOW_DATA.__int_2050;
    camera2->__viewport_10C.Height = WINDOW_DATA.__int_2054;
    camera2->__viewport_124.X = WINDOW_DATA.__float_2070 * 32.0f;
    camera2->__viewport_124.Y = WINDOW_DATA.__float_2070 * 16.0f;
    camera2->__viewport_124.Width = WINDOW_DATA.__float_2070 * 384.0f;
    camera2->__viewport_124.Height = WINDOW_DATA.__float_2070 * 448.0f;
    this->__sub_454950(camera2);
    StageCamera* camera0 = &this->cameras[0];
    *camera0 = *camera2;
    camera0->camera_index = 0;
    camera0->viewport.X = WINDOW_DATA.__float_2070 * 32.0f;
    camera0->viewport.Y = WINDOW_DATA.__float_2070 * 16.0f;
    camera0->viewport.Width = WINDOW_DATA.__float_2070 * 384.0f;
    camera0->viewport.Height = WINDOW_DATA.__float_2070 * 448.0f;
    camera0->__int2_104.x = 0;
    camera0->__int2_104.y = 0;
    camera0->__viewport_10C = camera0->viewport;
    this->__sub_454950(camera0);
    StageCamera* camera1 = &this->cameras[1];
    *camera1 = *camera0;
    camera1->camera_index = 1;
    camera1->viewport.X = WINDOW_DATA.__float_2070 * 128.0f;
    camera1->viewport.Y = WINDOW_DATA.__float_2070 * 16.0f;
    camera1->viewport.Width = WINDOW_DATA.__float_2070 * 384.0f;
    camera1->viewport.Height = WINDOW_DATA.__float_2070 * 448.0f;
    camera1->__int2_104.x = 0;
    camera1->__int2_104.y = 0;
    camera1->__viewport_10C = camera1->viewport;
    this->__sub_454950(camera1);
    StageCamera* camera3 = &this->cameras[3];
    *camera3 = *camera0;
    camera3->camera_index = 3;
    camera3->viewport.X = ((float)WINDOW_DATA.__int_2050 - 408.0f) * 0.5f;
    camera3->viewport.Width = 408;
    camera3->viewport.Width = 472;
    camera3->__int2_104.x = 0;
    camera3->__int2_104.y = 0;
    camera3->viewport.Y = ((float)WINDOW_DATA.__int_2054 - 472.0f) * 0.5f;
    camera3->__viewport_10C = camera3->viewport;
    this->__sub_454950(camera3);
    WINDOW_DATA.__int_2084 = WINDOW_DATA.__int_2050 / 2;
    WINDOW_DATA.__int_2088 = WINDOW_DATA.__float_2070 * 16.0f;
}

// 0x454F50
dllexport gnu_noinline void Supervisor::__camera2_sub_454F50() {
    int32_t intA = WINDOW_DATA.__int_2050;
    int32_t intB = (float)(WINDOW_DATA.__int_2068 - intA) * 0.5f;
    float floatA = WINDOW_DATA.__float_2070;
    SUPERVISOR.cameras[2].__int2_104.x = intB;
    int32_t intC = WINDOW_DATA.__int_2054;
    int32_t intD = (float)(WINDOW_DATA.__int_206C - intC) * 0.5f;
    SUPERVISOR.cameras[2].__int2_104.y = intD;
    SUPERVISOR.cameras[2].__viewport_10C = SUPERVISOR.cameras[2].viewport;
    SUPERVISOR.cameras[2].__viewport_10C.X = intB;
    SUPERVISOR.cameras[2].__viewport_10C.Y = intD;
    SUPERVISOR.cameras[2].__viewport_10C.Width = intA;
    SUPERVISOR.cameras[2].__viewport_10C.Height = intC;
    SUPERVISOR.cameras[2].__viewport_124.X = floatA * -32.0f;
    SUPERVISOR.cameras[2].__viewport_124.Y = floatA * -16.0f;
    SUPERVISOR.cameras[2].__viewport_124.Width = floatA * 384.0f;
    SUPERVISOR.cameras[2].__viewport_124.Width = floatA * 448.0f;
}

// 0x462680
dllexport gnu_noinline ZUNResult thiscall ReplayManager::__load_from_path(const char* path) {
    char buffer[0x1000];
    void* replay_data;
    ReplayHeader* header;
    byteloop_strcpy(this->file_path, path);
    if (!GAME_MANAGER.__unknown_flag_C) {
        chdir(WINDOW_DATA.appdata_path);
        sprintf(buffer, "replay/%s", path);
        if (zun_file_exists(buffer)) {
            if (!__zun_open_file(buffer)) {
                header = (ReplayHeader*)__zun_read_file(sizeof(ReplayHeader));
                this->header = header;
                if (
                    header->magic.as_int == REPLAY_MAGIC &&
                    header->__version == REPLAY_VERSION_NUMBER
                ) {
                    replay_data = __zun_read_file(header->file_size);
                    __zun_close_file();
                    header = this->header;
                    goto valid_replay;
                }
            }
            __zun_close_file();
        }
        chdir(WINDOW_DATA.exe_path);
        return ZUN_ERROR;
    }
    else {
        void* file = read_file_to_buffer(path, (int32_t*)&replay_data, false);
        header = (ReplayHeader*)file;
        this->header = header;
        replay_data = based_pointer(file, sizeof(ReplayHeader));
    }
valid_replay:
    this->file_buffer = malloc(header->uncompressed_size);
    __decrypt_buffer(replay_data, this->header->compressed_size, 0x5C, 0xE1, 0x400, this->header->compressed_size);
    __decrypt_buffer(replay_data, this->header->compressed_size, 0x7D, 0x3A, 0x100, this->header->compressed_size);
    __decompress_buffer(replay_data, this->header->compressed_size, this->file_buffer, this->header->uncompressed_size);
    void* file = this->file_buffer;
    this->info = (ReplayInfo*)file;
    ReplayGamestate* gamestate_ptr = based_pointer<ReplayGamestate>(file, sizeof(ReplayInfo));
    for (int32_t i = 0; ; ++i) {
        int32_t stage_count = this->info->stage_count;
        stage_count = stage_count >= STAGE_COUNT ? 6 : stage_count;
        if (i >= stage_count) {
            break;
        }
        ReplayFrameInput* inputs = (ReplayFrameInput*)&gamestate_ptr->extra[0];
        this->stage_data[gamestate_ptr->stage_number].gamestate_start = gamestate_ptr;
        this->stage_data[gamestate_ptr->stage_number].input_start = inputs;
        this->stage_data[gamestate_ptr->stage_number].fps_counts_start = (uint8_t*)(gamestate_ptr->input_count * sizeof(ReplayFrameInput));
        gamestate_ptr = based_pointer(gamestate_ptr + 1, gamestate_ptr->extra_size);
    }
    if (
        !GAME_MANAGER.__unknown_flag_C &&
        replay_data
    ) {
        free(replay_data);
    }
    chdir(WINDOW_DATA.exe_path);
    return ZUN_SUCCESS;
}

// 0x461E90
dllexport gnu_noinline ZUNResult thiscall ReplayManager::__write_to_path(const char* filename, const char* name, bool arg3, bool arg4) {
    ReplayManager* self = REPLAY_MANAGER_PTR;

    int32_t stage_end = 0; // EBP-128
    int32_t stage_start = 0; // EBP-120

    byteloop_strcpy(self->info->name, name);
    for (
        int32_t i = strlen(name);
        i < countof(self->info->name) - 1;
        ++i
    ) {
        self->info->name[i] = ' ';
    }

    if (self->__unknown_flag_B && arg4) {
        ReplayChunk* cur_chunk = self->current_chunk_node->data;
        if (cur_chunk->write_input(REPLAY_INPUT_END)) {
            self->current_chunk_node = self->allocate_chunk(self->stage_number);
        }
    }

    char path[MAX_PATH];
    sprintf(path, "replay/%s", filename);

    size_t alloc_size = sizeof(ReplayInfo);
    size_t stage_count = 0;

    for (int32_t i = 0; i < STAGE_COUNT; ++i) {
        if (ReplayGamestate* game_state = self->game_states[i]) {
            stage_end = i;
            if (!stage_start) {
                stage_start = i; // This only works if 0 isn't a valid index, so why are the arrays 8...?
            }

            if (!self->__unknown_flag_B) {
                game_state->extra_size = 0;
            }
            alloc_size += sizeof(ReplayGamestate);
            self->chunk_lists[i].for_each([&](ReplayChunk* chunk) {
                size_t input_size = chunk->input_size();
                alloc_size += input_size;
                alloc_size += chunk->fps_size();
                if (!self->__unknown_flag_B) {
                    self->game_states[i]->extra_size += input_size + chunk->fps_size();
                    self->game_states[i]->input_count += chunk->input_count();
                }
            });
            ++stage_count;
        }
    }

    self->info->stage_count = stage_count;
    self->info->score = GAME_MANAGER.globals.score;
    self->info->slowdown_rate = FPS_COUNTER_PTR->calc_slowdown_rate();

    uint8_t* buffer = (uint8_t*)malloc(alloc_size);
    *(ReplayInfo*)buffer = *self->info;

    size_t offset = sizeof(ReplayInfo);
    for (int32_t i = 0; i < STAGE_COUNT; ++i) {
        if (ReplayGamestate* game_state = self->game_states[i]) {
            memcpy(&buffer[offset], game_state, sizeof(ReplayGamestate));
            offset += sizeof(ReplayGamestate);
            self->chunk_lists[i].for_each([&](ReplayChunk* chunk) {
                memcpy(&buffer[offset], chunk->inputs, chunk->input_size());
                offset += chunk->input_size();
            });
            self->chunk_lists[i].for_each([&](ReplayChunk* chunk) {
                memcpy(&buffer[offset], chunk->fps_counts, chunk->fps_size());
                offset += chunk->fps_size();
            });
        }
    }

    int32_t compressed_size;
    uint8_t* compressed_buffer = (uint8_t*)__compress_buffer(buffer, offset, &compressed_size);
    free(buffer);
    __crypt_buffer(compressed_buffer, compressed_size, 0x7D, 0x3A, 0x100, compressed_size);
    __crypt_buffer(compressed_buffer, compressed_size, 0x5C, 0xE1, 0x400, compressed_size);

    self->header->uncompressed_size = offset;
    self->header->compressed_size = compressed_size;
    self->header->file_size = compressed_size + sizeof(ReplayHeader);

    chdir(WINDOW_DATA.appdata_path);
    __zun_open_new_file(path);
    if (__zun_write_file(self->header, sizeof(ReplayHeader)) != ZUN_ERROR) {
        __zun_write_file(compressed_buffer, compressed_size);
    }

    if (compressed_buffer) {
        free(compressed_buffer);
    }

    // oh why zun
    char* user_data_buffer = (char*)malloc(UINT16_MAX);
    memset(user_data_buffer, 0, UINT16_MAX);

    ReplayUserData* user_data = (ReplayUserData*)user_data_buffer;

    user_data->magic.as_uint = REPLAY_USER_MAGIC;
    char* user_write = user_data->text;

    user_write += sprintf(user_write, 
        JpEnStr("", "%s replay file information\r\n"),
        JpEnStr("", "Oriental Rainbow Cave")
    );
    user_write += sprintf(user_write, "Version %s\r\n", "1.00a");
    user_write += sprintf(user_write, "Name %s\r\n", self->info->name);

    tm* time = localtime(&self->info->time);

    user_write += sprintf(user_write, "Date %.2d/%.2d/%.2d %.2d:%.2d\r\n", time->tm_year % 100, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min);

    user_write += sprintf(user_write, "Chara %s\r\n", SIX_LETTER_SHOTTYPE_NAMES[self->info->shottype_index()]);

    user_write += sprintf(user_write, "Rank %s\r\n", SEVEN_LETTER_DIFFICULTY_NAMES[self->info->difficulty]);

    if (self->info->__end_stage > 7) {
        if (stage_start == 7) {
            user_write += sprintf(user_write, "Extra Stage Clear\r\n");
        }
        else {
            user_write += sprintf(user_write, "Stage All Clear\r\n");
        }
    }
    else {
        if (stage_start == stage_end) {
            if (stage_start == 7) {
                user_write += sprintf(user_write, "Extra Stage\r\n");
            }
            else {
                user_write += sprintf(user_write, "Stage %d\r\n", stage_start);
            }
        }
        else {
            user_write += sprintf(user_write, "Stage %d - %d\r\n", stage_start, stage_end);
        }
    }

    user_write += sprintf(user_write, "Score %d\r\n", self->info->score);

    user_write += sprintf(user_write, "Slow Rate %2.2f\r\n", self->info->slowdown_rate);

    ++user_write;

    int32_t user_size_align = (user_write - user_data_buffer) % 4;
    if (!user_size_align) {
        user_write += 4 - user_size_align;
    }
    int32_t user_size = user_write - user_data_buffer;
    user_data->size = user_size;

    __zun_write_file(user_data_buffer, user_size);

    memset(user_data_buffer, 0, UINT16_MAX);
    user_data->magic.as_uint = REPLAY_USER_MAGIC;
    user_data->__byte_8 = 1;

    user_write = user_data->text;

    user_write += sprintf(user_write,
        JpEnStr("", "can write comments")
    );

    ++user_write;

    user_size_align = (user_write - user_data_buffer) % 4;
    if (!user_size_align) {
        user_write += 4 - user_size_align;
    }
    user_size = user_write - user_data_buffer;
    user_data->size = user_size;

    __zun_write_file(user_data_buffer, user_size);

    free(user_data_buffer);

    __zun_close_file();

    chdir(WINDOW_DATA.exe_path);

    self->__unknown_flag_B = true;

    return ZUN_SUCCESS;
}

// 0x4543A0
dllexport gnu_noinline ZUNResult thiscall Supervisor::load_config_file(int) {
    SUPERVISOR.config.initialize();
    chdir(WINDOW_DATA.appdata_path);
    int32_t file_size;
    void* config_file = read_file_to_buffer("th18.cfg", &file_size, true);
    chdir(WINDOW_DATA.exe_path);
    if (!config_file) {
        LOG_BUFFER.write(JpEnStr("コンフィグデータが見つからないので初期化しました\r\n", "I initialized because the configuration data was not found\r\n"));
        SUPERVISOR.config.initialize();
    }
    else {
        SUPERVISOR.config = *(Config*)config_file;
        free(config_file);
        if (
            SUPERVISOR.config.__ubyte_44 < 2 &&
            SUPERVISOR.config.__ubyte_45 < 3 &&
            SUPERVISOR.config.__ubyte_46 < 2 &&
            SUPERVISOR.config.__ubyte_47 < 10 &&
            SUPERVISOR.config.__ubyte_48 < 3 &&
            SUPERVISOR.config.__ubyte_49 < 3 &&
            SUPERVISOR.config.__dword_0 == 0x180002 &&
            file_size == 0x88
        ) {
            INPUT_STATES[0].xinput_mapping   = SUPERVISOR.config.xinput_mapping;
            INPUT_STATES[0].joypad_mapping   = SUPERVISOR.config.joypad_mapping;
            INPUT_STATES[0].keyboard_mapping = SUPERVISOR.config.keyboard_mapping;
        }
        else {
            LOG_BUFFER.write(JpEnStr("コンフィグデータが異常でしたので再初期化しました\r\n", "The configuration data was abnormal, so it was reinitialized\r\n"));
            SUPERVISOR.config.initialize();
        }
    }
    this->disable_vsync = false;
    if (this->config.disable_fog) {
        LOG_BUFFER.write(JpEnStr("フォグの使用を抑制します\r\n", "suppress the use of fog\r\n"));
    }
    if (this->present_parameters.Windowed) {
        LOG_BUFFER.write(JpEnStr("ウィンドウモードで起動します\r\n", "start in windowed mode\r\n"));
    }
    if (this->config.reference_rasterizer) {
        LOG_BUFFER.write(JpEnStr("リファレンスラスタライザを強制します\r\n", "force reference rasterizer\r\n"));
    }
    if (this->config.disable_direct_input) {
        LOG_BUFFER.write(JpEnStr("パッド、キーボードの入力に DirectInput を使用しません\r\n", "Does not use DirectInput for pad and keyboard input\r\n"));
    }
    if (this->config.preload_bgm) {
        LOG_BUFFER.write(JpEnStr("ＢＧＭをメモリに読み込みます\r\n", "Load BGM into memory\r\n"));
    }
    if (this->config.disable_vsync) {
        LOG_BUFFER.write(JpEnStr("垂直同期を取りません\r\n", "no vertical sync\r\n"));
        SUPERVISOR.disable_vsync = true;
    }
    if (this->config.__disable_locale_detection) {
        LOG_BUFFER.write(JpEnStr("文字描画の環境を自動検出しません\r\n", "Do not auto-detect character drawing environment\r\n"));
    }
    chdir(WINDOW_DATA.appdata_path);
    if (ZUN_FAILED(__zun_create_new_file_from_buffer("th18.cfg", &SUPERVISOR.config, sizeof(Config)))) {
        LOG_BUFFER.write_error(JpEnStr("ファイルが書き出せません %s\r\n", "Unable to write file %s\r\n"), "th18.cfg");
        LOG_BUFFER.write_error(JpEnStr("フォルダが書込み禁止属性になっているか、ディスクがいっぱいいっぱいになってませんか？\r\n", "Is the folder write-protected or is the disk full?\r\n"));
        chdir(WINDOW_DATA.exe_path);
        return ZUN_ERROR;
    }
    else {
        chdir(WINDOW_DATA.exe_path);
        return ZUN_SUCCESS;
    }
}

// 0x473390
dllexport gnu_noinline double vectorcall get_runtime() asm_symbol_rel(0x473390);
dllexport gnu_noinline double vectorcall get_runtime() {
    CRITICAL_SECTION_MANAGER.enter_section(Menu_CS);
    double current_time;
    double prev_time;
    if (WINDOW_DATA.performance_counter_frequency.QuadPart) {
        LARGE_INTEGER current_qpc_value;
        QueryPerformanceCounter(&current_qpc_value);
        current_time = (double)(current_qpc_value.QuadPart - WINDOW_DATA.startup_qpc_value.QuadPart) / (double)WINDOW_DATA.performance_counter_frequency.QuadPart;
        prev_time = WINDOW_DATA.__double_20B0;
        if (current_time < prev_time) {
            WINDOW_DATA.__double_20B0 = prev_time = current_time;
        }
        CRITICAL_SECTION_MANAGER.leave_section(Menu_CS);
        return current_time - WINDOW_DATA.__double_20B0;
    } else {
        current_time = timeGetTime();
        prev_time = WINDOW_DATA.__double_20B0;
        if (current_time < prev_time) {
            WINDOW_DATA.__double_20B0 = prev_time = current_time;
        }
        current_time = (current_time - prev_time * 1000.0) / 1000.0;
        CRITICAL_SECTION_MANAGER.leave_section(Menu_CS);
        return current_time;
    }
}

// 0x443860
dllexport gnu_noinline UpdateFuncRet thiscall GameThread::on_tick() {
    if (SUPERVISOR.__unknown_bitfield_A != 0) {
        return UpdateFuncDeleteCurrentThenNext;
    }

    if (this->__unknown_flag_E) {
        if (this->__unknown_flag_I) {
            return UpdateFuncEnd1;
        }

        int A = ++this->__int_D4;

        ReplayMode replay_mode = this->replay_mode;
        if (replay_mode != __replay_recording) {
            if (A == 120) {
                // PAUSE_MENU_PTR->__sub_4588F0();
                replay_mode = this->replay_mode;
            }
        }
        if (replay_mode == __replay_recording) {
            if (this->__int_D4 == 180) {
                // ScreenEffect::create(5, 0xC8, 0, 0, 0, 0x59);
                replay_mode = this->replay_mode;
            }
        }
        if (replay_mode == __replay_recording) {
            if (this->__int_D4 > 380) {
                // ABILITY_MANAGER_PTR->__sub_4074C0();
                ABILITY_MANAGER_PTR->__sub_407DA0(true);

                int32_t mode;
                if (GAME_MANAGER.globals.difficulty != EXTRA) {
                    mode = (SUPERVISOR.__unknown_bitfield_A & 1) ? 2 : 15;
                } else {
                    mode = (SUPERVISOR.__unknown_bitfield_A & 1) ? 2 : 16;
                }
                SUPERVISOR.gamemode_switch = mode;
            }
        }
    }

    Gui* gui = GUI_PTR;
    if (
        !gui->__unknown_flag_A ||
        gui->__timer_198 >= 120
        ) {
        if (!this->__unknown_flag_I) {
            return UpdateFuncNext;
        }

        // These both allocate the "main" enemy
        switch (this->__timer_C.current) {
            case 0:
                if (ZUN_FAILED(this->__sub_443E60())) {
                    return UpdateFuncNext;
                }
                break;
            case 30:
                this->__sub_4443C0();
                break;
        }
    }

    Stage* stageB = STAGE_B_PTR;
    if (
        stageB != NULL &&
        stageB->__unknown_flag_A &&
        stageB != NULL
    ) {
        delete stageB;
    }

    WINDOW_DATA.__int_20D0 = this->__timer_C == 5 ? 2 : WINDOW_DATA.__int_20D0;

    if (this->skip_flag) {
        this->__unknown_flag_J = true;
        return UpdateFuncNext;
    }

    if (!GAME_MANAGER.__unknown_flag_E) {
        if (
            INPUT_STATES[0].check_hardware_inputs(BUTTON_SHOOT | BUTTON_BOMB | BUTTON_PAUSE | BUTTON_ENTER) ||
            (this->__unknown_flag_I | this->__unknown_flag_L | this->__unknown_flag_M)
        ) {
            SUPERVISOR.gamemode_switch = (SUPERVISOR.__unknown_bitfield_A & 1) ? 2 : 4;
        }

        switch (this->__timer_C.current) {
            case 3540:
                // ScreenEffect::create(5, 0x3C, 0, 0, 0, 0x5B);
                break;
            case 3600:
                SUPERVISOR.gamemode_switch = (SUPERVISOR.__unknown_bitfield_A & 1) ? 2 : 4;
                break;
        }
    }

#if INCLUDE_PATCH_CODE
    gui = GUI_PTR;
    uint64_t score = GAME_MANAGER.globals.score | (uint64_t)score_upper[0] << 32;
    uint64_t displayed_score = gui->__score | (uint64_t)score_upper[2] << 32;
    if (score != displayed_score) {
        uint64_t score_diff = score - displayed_score;
        uint64_t score_diff_B = score_diff / 32;

        uint32_t score_diff_C = __max(__min(score_diff_B, 578910), 1);

        if (gui->__int_15C < score_diff_C) {
            gui->__int_15C = score_diff_C;
        } else {
            score_diff_C = gui->__int_15C;
        }

        if (score_diff_C > score_diff) {
            gui->__int_15C = score_diff;
        } else {
            score_diff = score_diff_C;
        }
        displayed_score += score_diff;
        gui->__score = displayed_score;
        score_upper[2] = displayed_score >> 32;

        if (displayed_score >= score) {
            gui->__int_15C = 0;
        }
    }
    uint64_t high_score = GAME_MANAGER.__high_score | (uint64_t)score_upper[1] << 32;
    if (high_score < displayed_score) {
        int32_t continues_local = GAME_MANAGER.globals.continues;
        GAME_MANAGER.__unknown_flag_C = true;
        GAME_MANAGER.__high_score = displayed_score;
        score_upper[1] = displayed_score >> 32;
        GAME_MANAGER.__high_score_continues = continues_local;
    }
#else
    gui = GUI_PTR;
    uint32_t score = GAME_MANAGER.globals.score;
    uint32_t displayed_score = gui->__score;
    if (score != displayed_score) {
        uint32_t score_diff = score - displayed_score;
        uint32_t score_diff_B = score_diff / 32;

        int32_t score_diff_C = __max(__min(score_diff_B, 578910), 1);

        if (gui->__int_15C < score_diff_C) {
            gui->__int_15C = score_diff_C;
            score = GAME_MANAGER.globals.score;
        } else {
            score_diff_C = gui->__int_15C;
        }

        int32_t score_diff_D = score - displayed_score;

        if (score_diff_C > score_diff_D) {
            gui->__int_15C = score_diff_D;
            score_diff_C = score_diff_D;
        }
        displayed_score += score_diff_C;
        gui->__score = displayed_score;

        if (displayed_score >= (int32_t)GAME_MANAGER.globals.score) {
            gui->__int_15C = 0;
        }
    }
    if ((int32_t)GAME_MANAGER.__high_score < displayed_score) {
        int32_t continues = GAME_MANAGER.globals.continues;
        GAME_MANAGER.__unknown_flag_C = true;
        GAME_MANAGER.__high_score = displayed_score;
        GAME_MANAGER.__high_score_continues = continues;
    }
#endif
}

// All values are stored / 100

// 0x4B666C
static const int32_t MIN_POINT_ITEM_VALUES_TABLE[DIFFICULTY_COUNT] = {
    10000,
    10000,
    10000,
    10000,
    10000,
    100000
};
// 0x4B6654
static const int32_t MAX_POINT_ITEM_VALUES_TABLE[DIFFICULTY_COUNT] = {
    200000,
    500000,
    700000,
    1000000,
    500000,
    1000000
};

// 0x4B663C
static const int32_t CONTINUE_CREDITS_TABLE[DIFFICULTY_COUNT] = {
    5, 5, 5, 5,
    0, 0
};

inline unsigned GameThread::thread_start_impl() {
    GameThread* game_thread = GAME_THREAD_PTR;
    
    this->skip_flag = true;

    __asm FINIT

    while (ANM_MANAGER_PTR->__int_20 >= 0) {
        if (SUPERVISOR.__unknown_bitfield_A) {
            goto thread_start_important_label;
        }
        Sleep(1);
    }

    if (!SUPERVISOR.__int_804) {
        Sleep(60);
    }

    SUPERVISOR.__arcade_vm_ptr_A->interrupt_and_run(2);

    GAME_SPEED.value = 1.0f;
    GAME_THREAD_PTR->__unknown_flag_E = false;
    GAME_MANAGER.globals.__counter_C = 0;
    GAME_MANAGER.globals.__counter_10 = 0;
    GAME_MANAGER.globals.__counter_14 = 0;

    ABILITY_MANAGER_PTR->wait_for_ability_data_to_load();

    if (GAME_THREAD_PTR->replay_mode == __replay_recording) {
        SCOREFILE_MANAGER_PTR->primary_file.shottypes[GAME_MANAGER.globals.shottype_index()]
            .practice[GAME_MANAGER.globals.difficulty][GAME_MANAGER.globals.current_stage]
                .unlocked = true;
    }

    if (SUPERVISOR.__int_804) {
        int32_t difficulty = GAME_MANAGER.globals.difficulty;
        if (GAME_MANAGER.globals.current_stage == 7) {
            difficulty = __max(difficulty, LUNATIC);
            GAME_MANAGER.globals.difficulty = difficulty;
        }

        switch (GAME_MANAGER.__unknown_field_A) {
            case 2: {
                ScorefileSpellcard& spell = SCOREFILE_MANAGER_PTR->primary_file.shottypes[GAME_MANAGER.globals.shottype_index()]
                                                .spells[GAME_MANAGER.globals.__ecl_var_9907];
                GAME_MANAGER.__high_score = spell.spell_practice_score;
                GAME_MANAGER.__high_score_continues = 0;
                break;
            }
            case 1: {
                ScorefileStagePractice& practice = SCOREFILE_MANAGER_PTR->primary_file.shottypes[GAME_MANAGER.globals.shottype_index()]
                                                       .practice[GAME_MANAGER.globals.difficulty][GAME_MANAGER.globals.current_stage];
                GAME_MANAGER.__high_score = practice.score;
                GAME_MANAGER.__high_score_continues = 0;
                break;
            }
            default: {
                ScorefileRecord& record = SCOREFILE_MANAGER_PTR->primary_file.shottypes[GAME_MANAGER.globals.shottype_index()]
                                              .records[GAME_MANAGER.globals.difficulty][0];
                GAME_MANAGER.__high_score = record.score;
                GAME_MANAGER.__high_score_continues = record.continues;
                break;
            }
        }

        GAME_MANAGER.globals.graze_in_stage = 0;

        int32_t continues = GAME_MANAGER.globals.continues;
        continues = !GAME_MANAGER.__unknown_flag_D ? 0 : GAME_MANAGER.globals.continues;
        GAME_MANAGER.globals.continues = continues;

        GAME_MANAGER.globals.score = 0;

        int32_t starting_bombs = __min(GAME_MANAGER.globals.bomb_stock_max, 3);

        GAME_MANAGER.globals.money_collected_in_game = 0;
        GAME_MANAGER.globals.current_money = 0;
        GAME_MANAGER.globals.point_items_collected_in_game = 0;
        GAME_MANAGER.globals.current_power = 0;
        GAME_MANAGER.globals.power_per_level = 1;
        GAME_MANAGER.globals.bomb_stocks_for_new_life = 3;
        GAME_MANAGER.globals.bomb_stocks = starting_bombs;

        // inlined version of standalone __update_bomb_ui
        Gui* gui = GUI_PTR;
        if (gui) {
            gui->__update_bomb_ui(starting_bombs, GAME_MANAGER.globals.bomb_fragments, GAME_MANAGER.globals.bomb_stock_max);
        }

        GAME_MANAGER.globals.bomb_fragments = 0;
        GAME_MANAGER.globals.life_fragments = 0;
        GAME_MANAGER.globals.lives_added = 0;
        GAME_MANAGER.globals.miss_count_in_game = 0;
        GAME_MANAGER.globals.__dword_94 = 0;

        GAME_MANAGER.globals.__timer_C4.reset();
        GAME_MANAGER.globals.__timer_D8.reset();

        int32_t min_item_value = MIN_POINT_ITEM_VALUES_TABLE[difficulty] * 100;
        int32_t max_item_value = MAX_POINT_ITEM_VALUES_TABLE[difficulty] * 100;

        GAME_MANAGER.globals.min_point_item_value = min_item_value;
        GAME_MANAGER.globals.__dword_EC = 0;
        GAME_MANAGER.globals.__dword_F0 = 0;
        GAME_MANAGER.globals.__dword_F4 = 0;
        GAME_MANAGER.globals.max_point_item_value = max_item_value;

        // why are you like this ZUN
        int32_t item_value = min_item_value / 100.0f * 100.0f;

        if (item_value < max_item_value) {
            item_value = __min(item_value, GAME_MANAGER.globals.min_point_item_value);
        } else {
            item_value = max_item_value;
        }
        GAME_MANAGER.globals.point_item_value = item_value;

        GAME_MANAGER.continue_credits = CONTINUE_CREDITS_TABLE[difficulty];
        GAME_MANAGER.globals.life_stock_max = 7;

        switch (GAME_MANAGER.__unknown_field_A) {
            case 2:
                GAME_MANAGER.globals.bomb_stock_max = 7;
                GAME_MANAGER.globals.life_stocks = 0;
                GAME_MANAGER.globals.bomb_stocks = 0;
                if (gui) {
                    gui->__update_bomb_ui(0, 0, 7);
                }
                break;
            case 0:
                GAME_MANAGER.globals.bomb_stock_max = 7;
                GAME_MANAGER.globals.life_stocks = 2;
                GAME_MANAGER.globals.bomb_stocks = 3;
                if (gui) {
                    gui->__update_bomb_ui(3, 0, 7);
                }
                break;
            default: {
                int32_t life_override = GAME_MANAGER.__int_C;
                if (!life_override) {
                    GAME_MANAGER.globals.bomb_stock_max = 7;
                    GAME_MANAGER.globals.life_stocks = 7;
                } else {
                    GAME_MANAGER.globals.life_stocks = life_override - 1;
                }
            }
        }

        if (!Player::allocate()) {
            goto thread_start_important_label;
        }

        if (GAME_MANAGER.__unknown_field_A == 2) {
            clang_forceinline GAME_MANAGER.globals.set_power(GAME_MANAGER.globals.power_per_level * 4);
        }
        else {
            int32_t power;
            int32_t stage = GAME_MANAGER.globals.current_stage;
            if (stage <= 1) {
                power = GAME_MANAGER.globals.power_per_level;
            }
            else if (stage == 7) {
                power = GAME_MANAGER.globals.power_per_level;
            }
            else {
                power = GAME_MANAGER.globals.power_per_level * 4;
            }
            GAME_MANAGER.globals.set_power(power);
        }

        GAME_MANAGER.__unknown_flag_C = false;

        if (
            game_thread->replay_mode == __replay_recording &&
            ACHIEVEMENT_MODE_STATE < 0
        ) {
            SCOREFILE_MANAGER_PTR->primary_file.shottypes[GAME_MANAGER.globals.shottype_index()].__add_play_count();
        }

        if (
            GAME_MANAGER.globals.current_stage == 7 &&
            !ABILITY_MANAGER_PTR->card_equipped(MAGATAMA2_CARD)
        ) {
            ABILITY_MANAGER_PTR->allocate_new_card(MAGATAMA2_CARD, 0);
            //ABILITY_MANAGER_PTR->__sub_4094C0();
        }
        UNKNOWN_COUNTER_A = 0;
    }
    else {
        GAME_MANAGER.__high_score = __max(GAME_MANAGER.globals.score, GAME_MANAGER.__high_score);
    }
thread_start_important_label:
}

// 0x4726A0
dllexport gnu_noinline ZUNResult WindowData::__save_properties_and_configure_paths() {
    SystemParametersInfoA(SPI_GETSCREENSAVEACTIVE, 0, &WINDOW_DATA.screen_saver_active, 0);
    SystemParametersInfoA(SPI_GETLOWPOWERACTIVE, 0, &WINDOW_DATA.screen_saver_low_power_active, 0);
    SystemParametersInfoA(SPI_GETPOWEROFFACTIVE, 0, &WINDOW_DATA.screen_saver_power_off_active, 0);
    SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETLOWPOWERACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETPOWEROFFACTIVE, 0, NULL, SPIF_SENDCHANGE);
    QueryPerformanceFrequency(&WINDOW_DATA.performance_counter_frequency);
    QueryPerformanceCounter(&WINDOW_DATA.startup_qpc_value);
    char* appdata_path = this->appdata_path;
    GetEnvironmentVariableA("APPDATA", appdata_path, sizeof(this->appdata_path));
    if (appdata_path[0] != '\0') {
        byteloop_strcat(appdata_path, "\\ShanghaiAlice");
        if (!chdir(appdata_path)) {
            if (!mkdir(appdata_path)) return ZUN_ERROR;
            if (!chdir(appdata_path)) return ZUN_ERROR;
        }
        byteloop_strcat(appdata_path, "\\th18");
        if (!chdir(appdata_path)) {
            if (!mkdir(appdata_path)) return ZUN_ERROR;
            if (!chdir(appdata_path)) return ZUN_ERROR;
        }
        byteloop_strcat(appdata_path, "\\");
        if (!chdir("replay")) {
            if (!mkdir("replay")) return ZUN_ERROR;
        }
        if (!chdir("snapshot")) {
            if (!mkdir("snapshot")) return ZUN_ERROR;
        }
        DebugLogger::__debug_log_stub_11("%d\n", appdata_path);
    }
    char* exe_path = this->exe_path;
    if (GetModuleFileNameA(NULL, exe_path, sizeof(this->exe_path)) != ERROR_SUCCESS) {
        if (char* final_slash = strrchr(exe_path, '\\')) {
            *final_slash = '\0';
        }
        DebugLogger::__debug_log_stub_11("%d\n", exe_path);
    }
    if (!chdir(exe_path)) return ZUN_ERROR;
    return ZUN_SUCCESS;
}

template<typename L>
inline int32_t WindowData::update_window_common(const L& lambda) {
    ANM_MANAGER_PTR->flush_sprites();
    SUPERVISOR.set_camera_by_index(2);

    switch (UPDATE_FUNC_REGISTRY_PTR->run_all_on_tick()) {
        case 0:
            SUPERVISOR.__thread_A94.stop_and_cleanup();
            return 1;
        case -1:
            SUPERVISOR.__thread_A94.stop_and_cleanup();
            return 2;
    }

    ++this->__sbyte_1C;
    if (SUPERVISOR.config.__ubyte_48 < this->__sbyte_1C) {
        SUPERVISOR.d3d_device->BeginScene();
        ANM_MANAGER_PTR->reset_vertex_buffers();
        SUPERVISOR.fog_enabled = 0xFF;
        ANM_MANAGER_PTR->flush_sprites();
        SUPERVISOR.fog_enabled = FALSE;
        SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
        UPDATE_FUNC_REGISTRY_PTR->run_all_on_draw();
        ANM_MANAGER_PTR->flush_sprites();
        SUPERVISOR.d3d_device->SetTexture(0, NULL);
        SUPERVISOR.d3d_device->EndScene();
        this->__sbyte_1C = 0;

        lambda();
    }
    SUPERVISOR.__double_B50 = get_runtime() - this->__double_2098;
    return 0;
}

// 0x4740D0
dllexport gnu_noinline void __set_default_d3d_states() asm_symbol_rel(0x4740D0);
dllexport gnu_noinline void __set_default_d3d_states() {
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_ALPHAREF, 0x01);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGENABLE, TRUE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGDENSITY, bitcast<DWORD>(1.0f));
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGCOLOR, PackD3DCOLOR(255, 160, 160, 160));
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGSTART, bitcast<DWORD>(1000.0f));
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_FOGEND, bitcast<DWORD>(5000.0f));
    SUPERVISOR.d3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    SUPERVISOR.d3d_device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0 | D3DTSS_TCI_PASSTHRU);
    SUPERVISOR.d3d_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    SUPERVISOR.d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    SUPERVISOR.d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    SUPERVISOR.d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
    SUPERVISOR.d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    SUPERVISOR.d3d_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    if (AnmManager* anm_manager = ANM_MANAGER_PTR) {
        anm_manager->current_blend_mode = (AnmBlendMode)11;
        anm_manager->__byte_3120E09 = -1;
        anm_manager->__sbyte_3120E0A = -1;
        anm_manager->__index_3120E04 = -1;
        anm_manager->__byte_3120E0C = -1;
    }
}

// 0x472DD0
dllexport gnu_noinline int32_t thiscall WindowData::update_window__normal_version() {
    double A = get_runtime();
    double B = this->__double_20A0;
    this->__double_2098 = A;
    if (A < B) {
        this->__double_20A8 = A;
        B = A;
    } else {
        B = this->__double_20A8;
    }
    this->__double_20A0 = B;
    A -= B;
    A *= 1000.0;
    if (A >= 1.5) {
        Sleep(1);
    }

    if (this->__double_2098 > this->__double_20A8) {
        A = this->__double_2098;
        do {
            B = this->__double_20A8;
            B += 1.0 / 60.0;
            this->__double_20A8 = B;
        } while (A < B);

        return this->update_window_common([=]() {
            this->__sub_4731B0();
            if (SUCCEEDED(SUPERVISOR.d3d_device->Present(NULL, NULL, NULL, NULL))) {
                SUPERVISOR.__release_rendering_surfaces();
                // ANM_MANAGER_PTR->__sub_471120();
                SUPERVISOR.d3d_device->Reset(&SUPERVISOR.present_parameters);
                // ANM_MANAGER_PTR->__sub_4711B0();
                // SUPERVISOR.__sub_455EC0();
                __set_default_d3d_states();
                SUPERVISOR.__int_818 = 2;
                // FPS_COUNTER_PTR->__sub_4728A0();
            }
        });
    }
    return 0;
}

// 0x472FD0
dllexport gnu_noinline int32_t thiscall WindowData::update_window__alt_version() {
    return this->update_window_common([=]() {

        double A = get_runtime();
        this->__double_2098 = A;

        if (SUPERVISOR.config.__byte_4D == 1) {
            double B = A;
            double C = this->__double_20B8;
            B -= C;
            if (
                B < 1.0 / 60.0 &&
                B > 0.0
            ) {
                C += 1.0 / 60.0;
                C -= A;
                C *= 1000.0;
                C -= 3.5;
                int32_t D = C;
                if (D > 0) {
                    Sleep(D);
                }
            }

            this->__double_20C0 = get_runtime();

            this->__sub_4731B0();
            if (SUCCEEDED(SUPERVISOR.d3d_device->Present(NULL, NULL, NULL, NULL))) {
                SUPERVISOR.__release_rendering_surfaces();
                // ANM_MANAGER_PTR->__sub_471120();
                SUPERVISOR.d3d_device->Reset(&SUPERVISOR.present_parameters);
                // ANM_MANAGER_PTR->__sub_4711B0();
                // SUPERVISOR.__sub_455EC0();
                __set_default_d3d_states();
                SUPERVISOR.__int_818 = 2;
                this->__double_20B8 = get_runtime();
                // FPS_COUNTER_PTR->__sub_4728A0();
            }
        }
    });
}

inline int32_t thiscall WindowData::update_window__alt_version2() {
    double A = this->__double_2098;
    double B = this->__double_20A8;
    while (B < A) {
        B += 1.0 / 60.0;
    }
    this->__double_20A8 = B;

    return this->update_window_common([=]() {
        this->__sub_472B50();
    });
}

// 0x472B50
dllexport gnu_noinline void thiscall WindowData::__sub_472B50() {

}

// 0x4731B0
dllexport gnu_noinline void thiscall WindowData::__sub_4731B0() {

}

// 0x472280
dllexport gnu_noinline LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_ACTIVATEAPP: // 0x1C
            if ((WINDOW_DATA.window_active = wParam)) {
                WINDOW_DATA.__dword_14 = 0;
                SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
            }
            else {
                WINDOW_DATA.__dword_14 = 1;
                SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
            }
            break;
        case WM_SIZE: // 0x5
            if (WINDOW_DATA.__unknown_flag_A && wParam == SIZE_MAXIMIZED) {
                WINDOW_DATA.__unknown_flag_B = true;
                switch (WINDOW_DATA.__unknown_bitfield_A) {
                    case 3:
                        WINDOW_DATA.__unknown_bitfield_A = 0;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                    case 4:
                        WINDOW_DATA.__unknown_bitfield_A = 1;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                    case 5: case 6:
                        WINDOW_DATA.__unknown_bitfield_A = 2;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                }
            }
            break;
        case WM_CLOSE: // 0x10
            SUPERVISOR.__unknown_bitfield_A = 1;
            return TRUE; // Documentation says to return 0?
        case WM_SETCURSOR: // 0x20
            if (!SUPERVISOR.present_parameters.Windowed) {
                if (WINDOW_DATA.__dword_14) {
                    SetCursor(LoadCursorA(NULL, IDC_ARROW));
                    while (ShowCursor(true) < 0);
                    return TRUE;
                }
                else {
                    while (ShowCursor(false) >= 0);
                    SetCursor(NULL);
                    return TRUE;
                }
            }
            else {
                SetCursor(LoadCursorA(NULL, IDC_ARROW));
                ShowCursor(true);
                return TRUE;
            }
            break;
        case WM_SYSCOMMAND: // 0x112
            switch (wParam) {
                case SC_MOUSEMENU: // 0xF090
                case SC_KEYMENU: // 0xF100
                    return TRUE;
            }
            break;
        case WM_SYSKEYDOWN: // 0x104
            if (wParam == VK_RETURN) {
                WINDOW_DATA.__unknown_flag_B = true;
                switch (WINDOW_DATA.__unknown_bitfield_A) {
                    case 3:
                        WINDOW_DATA.__unknown_bitfield_A = 0;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                    case 4:
                        WINDOW_DATA.__unknown_bitfield_A = 1;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                    case 5: case 6: case 7:
                        WINDOW_DATA.__unknown_bitfield_A = 2;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                    case 2:
                        switch (SUPERVISOR.config.__ubyte_4E) {
                            case 4:
                                WINDOW_DATA.__unknown_bitfield_A = 7;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                            case 3:
                                WINDOW_DATA.__unknown_bitfield_A = 6;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                            default:
                                WINDOW_DATA.__unknown_bitfield_A = 5;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                        }
                    case 8: case 9:
                        switch (SUPERVISOR.config.__ubyte_4E) {
                            case 4:
                                WINDOW_DATA.__unknown_bitfield_A = 7;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                            case 3:
                                WINDOW_DATA.__unknown_bitfield_A = 6;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                            case 2: case 5:
                                WINDOW_DATA.__unknown_bitfield_A = 5;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                            case 0:
                                WINDOW_DATA.__unknown_bitfield_A = 3;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                            case 1:
                                WINDOW_DATA.__unknown_bitfield_A = 4;
                                return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                        }
                        break;
                    case 0:
                        WINDOW_DATA.__unknown_bitfield_A = 3;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                    case 1:
                        WINDOW_DATA.__unknown_bitfield_A = 4;
                        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                }
            }
            break;
        case WM_DISPLAYCHANGE: // 0x7E
            if (!SUPERVISOR.present_parameters.Windowed) {
                Sleep(3000);
            }
            WINDOW_DATA.__dword_18 = 300;
            break;
    }
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// 0x4734E0
dllexport gnu_noinline void WindowData::__sub_4734E0(int arg1) {
    float floatA;
    int32_t height;
    int32_t width;
    switch (int32_t intA = WINDOW_DATA.__unknown_bitfield_A) {
        case 7:
            WINDOW_DATA.__float_2070 = floatA = 2.0f;
            WINDOW_DATA.window_width = 2560;
            WINDOW_DATA.window_height = 1920;
            break;
        case 6:
            WINDOW_DATA.__float_2070 = floatA = 2.0f;
            WINDOW_DATA.window_width = 1920;
            WINDOW_DATA.window_height = 1440;
            break;
        default:
            WINDOW_DATA.__float_2070 = floatA = 1.0f;
            WINDOW_DATA.window_width = 640;
            WINDOW_DATA.window_height = 480;
            break;
        case 1: case 4:
            WINDOW_DATA.__float_2070 = floatA = 1.5f;
            WINDOW_DATA.window_width = 960;
            WINDOW_DATA.window_height = 720;
            break;
        case 2: case 5:
            WINDOW_DATA.__float_2070 = floatA = 2.0f;
            WINDOW_DATA.window_width = 1280;
            WINDOW_DATA.window_height = 960;
            break;
        case 8: case 9:
            if (arg1) {
                height = WINDOW_DATA.__dword_2064;
                width = WINDOW_DATA.__dword_2060;
                WINDOW_DATA.window_width = width;
                WINDOW_DATA.window_height = height;
                if (width >= 2560 && height >= 1920) {
                    WINDOW_DATA.__float_2070 = floatA = 2.0f;
                    SUPERVISOR.config.__ubyte_4E = 4;
                }
                else if (width >= 1920 && height >= 1440) {
                    WINDOW_DATA.__float_2070 = floatA = 2.0f;
                    SUPERVISOR.config.__ubyte_4E = 3;
                }
                else if (width >= 1280 && height >= 960) {
                    WINDOW_DATA.__float_2070 = floatA = 2.0f;
                    SUPERVISOR.config.__ubyte_4E = 2;
                }
                else if (width >= 960 && height >= 720) {
                    WINDOW_DATA.__float_2070 = floatA = 1.5f;
                    SUPERVISOR.config.__ubyte_4E = 1;
                }
                else {
                    floatA = 1.0f;
                    SUPERVISOR.config.__ubyte_4E = 0;
                    WINDOW_DATA.__float_2070 = floatA;
                }
            } else {
                floatA = WINDOW_DATA.__float_2070;
            }
            // Screw this crap
    }
    height = floatA * 480.0f;
    width = floatA * 640.0f;
    WINDOW_DATA.__int_2054 = height;
    WINDOW_DATA.__int_2050 = width;
    this->__int_2074 = (int32_t)(width - 384.0f) / 2;
    this->__int_2078 = (int32_t)(WINDOW_DATA.__int_2054 - 448.0f) / 2;
}

// 0x473890
dllexport gnu_noinline BOOL WindowData::__create_window(HINSTANCE instance) {
    WNDCLASSA class_def;
    LCID user_locale = GetUserDefaultLCID();
    class_def.style = 0;
    class_def.hIcon = NULL;
    class_def.lpszMenuName = NULL;
    class_def.cbClsExtra = 0;
    class_def.cbWndExtra = 0;
    const char* window_title = user_locale == 0x411
        ? "東方虹龍洞　～ Unconnected Marketeers ver 1.00a"
        : "TH18 - Unconnected Marketeers ver 1.00a";
    class_def.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    class_def.hCursor = LoadCursorA(NULL, IDC_ARROW);
    class_def.hInstance = instance;
    class_def.lpfnWndProc = &WndProc;
    this->window_active = true;
    this->__dword_14 = 0;
    class_def.lpszClassName = "BASE";
    RegisterClassA(&class_def);
    this->__unknown_bitfield_A = SUPERVISOR.config.__ubyte_47;
    SUPERVISOR.present_parameters.Windowed = this->__unknown_bitfield_A > 12;
    if (SUPERVISOR.config.__ubyte_48 && SUPERVISOR.config.__byte_4D == 2) {
        this->__unknown_flag_C = true;
    } else {
        this->__unknown_flag_C = false;
    }
    this->__dword_array_20D4[0] = 15;
    this->__dword_array_20D4[1] = 15;
    this->__dword_array_20D4[2] = 0;
    this->__dword_array_20D4[3] = 12;
    this->__dword_array_20D4[4] = 12;
    this->__dword_array_20D4[5] = 0;
    this->__dword_array_20D4[6] = 12;
    this->__dword_array_20D4[7] = 12;
    this->__dword_array_20D4[8] = 0;
    this->__dword_array_20D4[9] = 8;
    this->__dword_array_20D4[10] = 8;
    this->__dword_array_20D4[11] = 0;
    this->__int_20D0 = 0;
    this->__sub_4734E0(true);
    if (!SUPERVISOR.present_parameters.Windowed) {
        this->window = CreateWindowExA(
            0,
            "BASE",
            window_title,
            WS_VISIBLE | WS_POPUP,
            0,
            0,
            this->window_width,
            this->window_height,
            NULL,
            NULL,
            instance,
            NULL
        );
    }
    else if (WINDOW_DATA.__unknown_bitfield_A != 8 && WINDOW_DATA.__unknown_bitfield_A != 9) {
        RECT rect;
        rect.right = this->window_width;
        rect.bottom = this->window_height;
        rect.left = 0;
        rect.top = 0;
        AdjustWindowRectEx(
            &rect,
            WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION | WS_VISIBLE,
            FALSE,
            0
        );
        this->window = CreateWindowExA(
            0,
            "BASE",
            window_title,
            WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION | WS_VISIBLE,
            SUPERVISOR.config.window_x,
            SUPERVISOR.config.window_y,
            rect.left - rect.right,
            rect.top - rect.bottom,
            NULL,
            NULL,
            instance,
            NULL
        );
    }
    else {
        HWND window_handle = CreateWindowExA(
            WS_EX_APPWINDOW,
            "BASE",
            window_title,
            WS_OVERLAPPED | WS_VISIBLE,
            0,
            0,
            this->window_width,
            this->window_height,
            NULL,
            NULL,
            instance,
            NULL
        );
        this->window = window_handle;
        SetWindowLongA(
            window_handle,
            GWL_STYLE,
            WS_OVERLAPPED
        );
        ShowWindow(
            this->window,
            SW_SHOW
        );
    }
    GetWindowRect(
        this->window,
        &SUPERVISOR.window_rect
    );
    SUPERVISOR.main_window_handle = this->window;
    return this->window == NULL;
}

struct MutexData {
    HANDLE handle; // 0x0
};
extern "C" {
    // 0x570C98
    extern MutexData MUTEX_DATA asm("_MUTEX_DATA");
}

#include <ShObjIdl.h>

// 0x474400
dllexport gnu_noinline ZUNResult __make_mutex_and_test_path() asm_symbol_rel(0x474400);
dllexport gnu_noinline ZUNResult __make_mutex_and_test_path() {
    MUTEX_DATA.handle = CreateMutexA(NULL, TRUE, "th18 App");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        LOG_BUFFER.write_error(JpEnStr("二つは起動できません\r\n", "Two cannot be started\r\n"));
        return ZUN_ERROR;
    }
    STARTUPINFOA startup_info = { sizeof(STARTUPINFOA) };
    char filename_buffer[MAX_PATH + 1];
    char path_buffer[MAX_PATH + 1];
    GetModuleFileNameA(NULL, filename_buffer, sizeof(filename_buffer));
    GetConsoleTitleA(path_buffer, sizeof(path_buffer));
    GetStartupInfoA(&startup_info);
    if (startup_info.lpTitle) {
        char* file_extension_str = strrchr(startup_info.lpTitle, '.');
        if (zun_file_exists(startup_info.lpTitle) && file_extension_str) {
            if (!stricmp(file_extension_str, ".lnk")) {
                do {
                    if (CoInitialize(NULL) != S_OK) {
                        LOG_BUFFER.write_error(JpEnStr("CoInitialize 初期化失敗\r\n", "CoInitialize initialization failure\r\n"));
                    }
                    IShellLinkA* shortcut_interface;
                    if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (LPVOID*)&shortcut_interface))) {
                        IPersistFile* persist_file_interface;
                        if (SUCCEEDED(shortcut_interface->QueryInterface(IID_IPersistFile, (LPVOID*)&persist_file_interface))) {
                            wchar_t* conversion_array = new wchar_t[MAX_PATH];
                            WIN32_FIND_DATAA find_data;
                            memset_force(&find_data, 0, sizeof(WIN32_FIND_DATAA));
                            MultiByteToWideChar(CP_ACP, 0, startup_info.lpTitle, -1, conversion_array, MAX_PATH);
                            if (SUCCEEDED(persist_file_interface->Load(conversion_array, 0))) {
                                shortcut_interface->GetPath(path_buffer, MAX_PATH, &find_data, 0);
                            }
                            delete[] conversion_array;
                            persist_file_interface->Release();
                        }
                        shortcut_interface->Release();
                    }
                    CoUninitialize();
                } while (!stricmp(strrchr(path_buffer, '.'), ".lnk"));
            }
            else {
                byteloop_strcpy(path_buffer, startup_info.lpTitle);
            }
            WINDOW_DATA.__bool_30 = strcmp_asm(path_buffer, filename_buffer) ? true : WINDOW_DATA.__bool_30;
        }
        SUPERVISOR.__unknown_flag_A = false;
    }
    else {
        SUPERVISOR.__unknown_flag_A = true;
    }
    return MUTEX_DATA.handle != NULL ? ZUN_SUCCESS : ZUN_ERROR;
}


#ifdef CHEAT_THE_LOADER

dllexport volatile char backing_memory[0x200000];

struct StaticCtorsDtors {

#define static_construct(global) new(&global) decltype(global)()

#define original_addr(addr) ((void*)((uintptr_t)original_game + ((uintptr_t)(addr) - 0x400000)))

    template<typename T>
    static inline void copy_from_original_game(T& value, HMODULE original_game) {
        __builtin_memcpy(&value, original_addr(&value), sizeof(value));
    }

    StaticCtorsDtors() {
        HMODULE original_game = LoadLibraryExA("F:\\Touhou_Stuff_2\\disassembly_stuff\\18\\crack\\th18.exe.unvlv.exe", NULL, 0);

        copy_from_original_game(SOUND_DATA, original_game);

        static_construct(LOG_BUFFER);
        static_construct(SOUND_MANAGER);
    }
    ~StaticCtorsDtors() {

    }
};

static StaticCtorsDtors fake_static_data;

#endif

extern "C" {
    // 0x5705F0
    extern JOYCAPSA JOYCAPS_GLOBAL asm("_JOYCAPS");
}

// 0x4B4280
static int ResolutionDialogButtonIDs[] = {
    0xD3,
    0xD2,
    0xD1,
    0xD8,
    0xD7,
    0xD6,
    0xD5,
    0xD4,
    0xCF,
    0xD0
};

// 0x4B7FF0
static int ResolutionDialogButtonIDsB[] = {
    0xCF,
    0xD0,
    0xD1,
    0xD2,
    0xD3,
    0xD4,
    0xD5,
    0xD6,
    0xD7,
    0xD8
};

// 0x4B7FBC
static uint8_t ResolutionConfigValues[] = {
    0, 0, 0, 0, 1, 0, 0, 0, 2, 0
};

// 0x4747D0
dllexport gnu_noinline void set_selected_resolution() asm_symbol_rel(0x4747D0);
dllexport gnu_noinline void set_selected_resolution() {
    if (IsDlgButtonChecked(WINDOW_DATA.resolution_dialogue, 0xCA) == BST_CHECKED) {
        SUPERVISOR.config.__unknown_flag_A = true;
    } else {
        SUPERVISOR.config.__unknown_flag_A = false;
    }
    for (int32_t i = 0; i < countof(ResolutionDialogButtonIDs); ++i) {
        if (IsDlgButtonChecked(WINDOW_DATA.resolution_dialogue, ResolutionDialogButtonIDs[i]) == BST_CHECKED) {
            SUPERVISOR.config.__ubyte_47 = i;
            SUPERVISOR.config.__ubyte_4E = ResolutionConfigValues[i];
            return;
        }
    }
}

// 0x4746C0
dllexport gnu_noinline INT_PTR CALLBACK ResolutionDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND: // 0x111
            if (LOWORD(wParam) == 0xCE) {
                return 0;
            }
            set_selected_resolution();
            DestroyWindow(WINDOW_DATA.resolution_dialogue);
            WINDOW_DATA.resolution_dialogue = NULL;
            break;
        case WM_INITDIALOG: // 0x110
            if (SUPERVISOR.config.__unknown_flag_A) {
                SendMessageA(GetDlgItem(hWnd, 0xCA), BM_SETCHECK, BST_CHECKED, 0);
            }
            SendMessageA(GetDlgItem(hWnd, ResolutionDialogButtonIDs[SUPERVISOR.config.__ubyte_47]), BM_SETCHECK, BST_CHECKED, 0);
            WINDOW_DATA.__unknown_bitfield_B = 2;
            break;
        case WM_CLOSE: // 0x10
            if (WINDOW_DATA.__unknown_bitfield_B == 2) {
                WINDOW_DATA.__unknown_bitfield_B = 1;
            }
            DestroyWindow(WINDOW_DATA.resolution_dialogue);
            WINDOW_DATA.resolution_dialogue = NULL;
            return TRUE;
    }
    return FALSE;
}

// 0x474850
dllexport gnu_noinline void process_resolution_dialog() asm_symbol_rel(0x474850);
dllexport gnu_noinline void process_resolution_dialog() {
    BOOL disabled_buttons[10] = {};
    if (!SUPERVISOR.config.__unknown_flag_A) {
        BYTE keyboard[256];
        GetKeyboardState(keyboard);
        if (keyboard[VK_SHIFT] & 0x80) {
            return;
        }
    }
    LCID user_locale = GetUserDefaultLCID();
    WINDOW_DATA.resolution_dialogue = CreateDialogParamA(WINDOW_DATA.current_instance, MAKEINTRESOURCEA(user_locale == 0x411 ? 0xCB : 0xCC), 0, &ResolutionDlgProc, 0);
    DEVMODEA dev_mode;
    // ZUN not following the documentation again by leaving dev_mode uninitialized...
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dev_mode);
    if (dev_mode.dmPelsWidth <= 2560 || dev_mode.dmPelsHeight <= 1920) {
        disabled_buttons[5] = true;
        EnableWindow(GetDlgItem(WINDOW_DATA.resolution_dialogue, 0xD4), FALSE);
    }
    if (dev_mode.dmPelsWidth <= 1920 && dev_mode.dmPelsHeight <= 1440) {
        disabled_buttons[6] = true;
        EnableWindow(GetDlgItem(WINDOW_DATA.resolution_dialogue, 0xD5), FALSE);
    }
    if (dev_mode.dmPelsWidth <= 1280 && dev_mode.dmPelsHeight <= 960) {
        disabled_buttons[7] = true;
        EnableWindow(GetDlgItem(WINDOW_DATA.resolution_dialogue, 0xD6), FALSE);
        disabled_buttons[2] = true;
        EnableWindow(GetDlgItem(WINDOW_DATA.resolution_dialogue, 0xD1), FALSE);
    }
    if (dev_mode.dmPelsWidth <= 960 && dev_mode.dmPelsHeight <= 720) {
        disabled_buttons[8] = true;
        EnableWindow(GetDlgItem(WINDOW_DATA.resolution_dialogue, 0xD7), FALSE);
        disabled_buttons[3] = true;
        EnableWindow(GetDlgItem(WINDOW_DATA.resolution_dialogue, 0xD2), FALSE);
    }
    int32_t button_index = 0;
    for (int32_t i = 0; i < countof(ResolutionDialogButtonIDsB); ++i) {
        if (IsDlgButtonChecked(WINDOW_DATA.resolution_dialogue, ResolutionDialogButtonIDsB[i]) == BST_CHECKED) {
            button_index = i;
            break;
        }
    }
    ShowWindow(WINDOW_DATA.resolution_dialogue, SW_SHOW);
    for (;;) {
        JOYINFOEX joy_info{};
        joy_info.dwSize = sizeof(JOYINFOEX);
        joy_info.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(JOYSTICKID1, &joy_info) == JOYERR_NOERROR) {
            // TODO: bother writing out the joystick code at 0x474AC0
        }
        MSG message;
        if (PeekMessageA(&message, NULL, WM_NULL, WM_NULL, PM_REMOVE)) {
            if (!IsDialogMessageA(WINDOW_DATA.resolution_dialogue, &message)) {
                continue;
            }
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        if (INPUT_STATES[0].check_hardware_inputs_no_repeat(BUTTON_ENTER | BUTTON_SHOOT)) {
            break;
        }
        if (
            INPUT_STATES[0].check_hardware_inputs_repeating(BUTTON_DOWN) &&
            button_index < 9
        ) {
            while (disabled_buttons[++button_index]);
            CheckRadioButton(WINDOW_DATA.resolution_dialogue, 0xCF, 0xD8, ResolutionDialogButtonIDsB[button_index]);
        }
        if (
            INPUT_STATES[0].check_hardware_inputs_repeating(BUTTON_UP) &&
            button_index > 0
        ) {
            while (disabled_buttons[--button_index]);
            CheckRadioButton(WINDOW_DATA.resolution_dialogue, 0xCF, 0xD8, ResolutionDialogButtonIDsB[button_index]);
        }
        if (!WINDOW_DATA.resolution_dialogue) {
            return;
        }
        Sleep(6);
    }
    set_selected_resolution();
    WINDOW_DATA.__unknown_bitfield_B = 0;
}

/*
struct D3DPRESENT_PARAMETERS {
    UINT                BackBufferWidth; // 0x0 EBP-54
    UINT                BackBufferHeight; // 0x4 EBP-50
    D3DFORMAT           BackBufferFormat; // 0x8 EBP-4C
    UINT                BackBufferCount; // 0xC EBP-48
    D3DMULTISAMPLE_TYPE MultiSampleType; // 0x10 EBP-44
    DWORD               MultiSampleQuality; // 0x14 EBP-40
    D3DSWAPEFFECT       SwapEffect; // 0x18 EBP-3C
    HWND                hDeviceWindow; // 0x1C EBP-38
    BOOL                Windowed; // 0x20 EBP-34
    BOOL                EnableAutoDepthStencil; // 0x24 EBP-30
    D3DFORMAT           AutoDepthStencilFormat; // 0x28 EBP-2C
    DWORD               Flags; // 0x2C EBP-28
    UINT                FullScreen_RefreshRateInHz; // 0x30 EBP-24
    UINT                PresentationInterval; // 0x34 EBP-20
    // 0x38
};
*/

struct SimpleInt2 {
    int32_t x;
    int32_t y;
};

// 0x4B8020
static const SimpleInt2 RESOLUTIONS[] = {
    { .x = 640, .y = 480 },
    { .x = 960, .y = 720 },
    { .x = 1024, .y = 768 },
    { .x = 1152, .y = 864 },
    { .x = 1280, .y = 960 },
    { .x = 1280, .y = 1024 },
    { .x = 1600, .y = 1024 },
    { .x = 1600, .y = 1200 },
    { .x = 1920, .y = 1080 },
    { .x = 1920, .y = 1200 },
    { .x = 1920, .y = 1440 },
    { .x = 2048, .y = 1152 },
    { .x = 2048, .y = 1536 },
    { .x = 2560, .y = 1440 },
    { .x = 2560, .y = 1600 },
    { .x = 3840, .y = 2160 }
};

// 0x473B20
dllexport gnu_noinline ZUNResult fastcall __sub_473B20(BOOL arg1) asm_symbol_rel(0x473B20);
dllexport gnu_noinline ZUNResult fastcall __sub_473B20(BOOL arg1) {
    D3DPRESENT_PARAMETERS present_parameters = SUPERVISOR.present_parameters;
    BOOL is_second_iteration = false;
    if (SUPERVISOR.config.__byte_4D == 3) {
        WINDOW_DATA.__unknown_flag_C = false;
        present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    } else {
        present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    }
    for (;;) {
        size_t i = 0;
        do {
            if (
                i == 0 ||
                (WINDOW_DATA.__unknown_bitfield_A != 8 && WINDOW_DATA.__unknown_bitfield_A != 9)
            ) {
                present_parameters.BackBufferWidth = WINDOW_DATA.__int_2068;
                present_parameters.BackBufferHeight = WINDOW_DATA.__int_206C;
            } else {
                int32_t width = RESOLUTIONS[i].x;
                if (width < WINDOW_DATA.__int_2050) continue;
                int32_t height = RESOLUTIONS[i].y;
                if (height < WINDOW_DATA.__int_2054) continue;
                present_parameters.BackBufferWidth = width;
                present_parameters.BackBufferHeight = height;
            }
            if (!arg1) {
                SUPERVISOR.__unknown_flag_D = false;
                if (!SUPERVISOR.config.reference_rasterizer) {
                    if (SUPERVISOR.d3d->CreateDevice(
                        D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL,
                        WINDOW_DATA.window,
                        D3DCREATE_HARDWARE_VERTEXPROCESSING,
                        &present_parameters,
                        &SUPERVISOR.d3d_device
                    ) == D3D_OK) {
                        break;
                    }
                    if (SUPERVISOR.d3d->CreateDevice(
                        D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL,
                        WINDOW_DATA.window,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                        &present_parameters,
                        &SUPERVISOR.d3d_device
                    ) == D3D_OK) {
                        break;
                    }
                }
                if (SUPERVISOR.d3d->CreateDevice(
                    D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_REF,
                    WINDOW_DATA.window,
                    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                    &present_parameters,
                    &SUPERVISOR.d3d_device
                ) == D3D_OK) {
                    break;
                }
                SAFE_RELEASE(SUPERVISOR.d3d_device);
            }
            else {
                if (SUPERVISOR.d3d_device->Reset(&present_parameters) == D3D_OK) {
                    goto reset_success;
                }
            }
        } while (++i < countof(RESOLUTIONS));
        SUPERVISOR.__unknown_flag_D = true;
reset_success:
        HDC hdc = GetDC(WINDOW_DATA.window);
        GetDeviceCaps(hdc, VREFRESH);
        ReleaseDC(WINDOW_DATA.window, hdc);
        if (i < countof(RESOLUTIONS)) {
            break;
        }
        if (is_second_iteration) {
            if (!arg1) {
                LOG_BUFFER.write_error(JpEnStr("Direct3D の初期化に失敗、この解像度には対応してません\r\n", "Direct3D initialization failed, this resolution is not supported\r\n"));
                SAFE_RELEASE(SUPERVISOR.d3d);
            }
            return ZUN_ERROR;
        }
        is_second_iteration = true;
    }
    HDC hdc = GetDC(WINDOW_DATA.window);
    int32_t refresh_rate = GetDeviceCaps(hdc, VREFRESH);
    ReleaseDC(WINDOW_DATA.window, hdc);
    if (refresh_rate != 60) {
        WINDOW_DATA.__unknown_flag_C = false;
        present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }
    SUPERVISOR.present_parameters = present_parameters;
    if (SUPERVISOR.d3d_device->Reset(&present_parameters) == D3D_OK) {
        LOG_BUFFER.write(
            JpEnStr("解像度 %d %dで起動します\r\n", "Booting at resolution %d %d\r\n")
            , present_parameters.BackBufferWidth, present_parameters.BackBufferHeight
        );
    }
    return ZUN_SUCCESS;
}

// 0x473DF0
dllexport gnu_noinline int32_t __initialize_d3d() asm_symbol_rel(0x473DF0);
dllexport gnu_noinline int32_t __initialize_d3d() {
    D3DPRESENT_PARAMETERS present_parameters;
    present_parameters.BackBufferWidth = 0;
    present_parameters.BackBufferHeight = 0;
    present_parameters.BackBufferFormat = D3DFMT_UNKNOWN;
    present_parameters.BackBufferCount = 0;
    present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    present_parameters.MultiSampleQuality = 0;
    present_parameters.hDeviceWindow = NULL;
    present_parameters.Windowed = false;
    present_parameters.FullScreen_RefreshRateInHz = 0;
    present_parameters.PresentationInterval = 0;
    D3DDISPLAYMODE display_mode;
    SUPERVISOR.d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode);
    UINT refresh_rate = display_mode.RefreshRate;
    D3DFORMAT d3d_format = display_mode.Format == D3DFMT_X8R8G8B8 ? D3DFMT_A8R8G8B8 : display_mode.Format;
    display_mode.Format = d3d_format;
    SUPERVISOR.display_mode = display_mode;
    if (SUPERVISOR.present_parameters.Windowed && refresh_rate != 60) {
        WINDOW_DATA.__unknown_flag_C = false;
    }
    BOOL boolA = true;
    SUPERVISOR.disable_vsync = WINDOW_DATA.__bool_30 ? boolA : SUPERVISOR.disable_vsync;
    if (!SUPERVISOR.present_parameters.Windowed) {
        if (SUPERVISOR.config.__ubyte_44 == 255) {
            d3d_format = D3DFMT_A8R8G8B8;
            LOG_BUFFER.write(JpEnStr("初回起動、画面を 32Bits で初期化しました\r\n", "First boot, screen initialized with 32Bits\r\n"));
        } else {
            d3d_format = !SUPERVISOR.config.__ubyte_44 ? D3DFMT_R5G6B5 : D3DFMT_A8R8G8B8;
            present_parameters.BackBufferFormat = d3d_format;
        }
        if (!SUPERVISOR.disable_vsync) {
            // IDK how this might've originally looked, so comma jank
            if (
                WINDOW_DATA.__unknown_flag_C ||
                ((present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE), SUPERVISOR.config.__byte_4D != 3)
            ) {
                present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
            }
            present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        }
        else {
            present_parameters.FullScreen_RefreshRateInHz = 0;
            present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
            present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        }
    }
    else {
        present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3d_format = d3d_format == D3DFMT_X8R8G8B8 ? D3DFMT_A8R8G8B8 : d3d_format;
        present_parameters.BackBufferFormat = d3d_format;
        if (SUPERVISOR.config.__byte_4D != 3 && refresh_rate == 60) {
            if (WINDOW_DATA.__unknown_flag_C) {
                present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
            }
        }
        else {
            WINDOW_DATA.__unknown_flag_C = false;
            present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        }
        present_parameters.Windowed = true;
    }
    SUPERVISOR.__unknown_flag_C = true;
    present_parameters.EnableAutoDepthStencil = true;
    present_parameters.AutoDepthStencilFormat = D3DFMT_D16;
    present_parameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    SUPERVISOR.__dword_824 = 1;
    SUPERVISOR.present_parameters = present_parameters;
    if (ZUN_SUCCEEDED(__sub_473B20(false))) {
        return boolA;
    }
    SUPERVISOR.d3d_device->GetDeviceCaps(&SUPERVISOR.d3dcaps);
    if (!(SUPERVISOR.d3dcaps.TextureOpCaps & D3DTEXOPCAPS_ADD)) {
        LOG_BUFFER.write(JpEnStr("D3DTEXOPCAPS_ADD をサポートしていません、色加算エミュレートモードで動作します\r\n", "Does not support D3DTEXOPCAPS_ADD, works in color addition emulation mode\r\n"));
    }
    if (SUPERVISOR.d3dcaps.MaxTextureWidth < 256) { // Is this a wrong number or a bad translation?
        LOG_BUFFER.write(JpEnStr("512 以上のテクスチャをサポートしていません。殆どの絵がボケて表示されます。\r\n", "Doesn't support larger than 512 textures. Most of the pictures are blurry.\r\n"));
    }
    if (SUPERVISOR.d3d->CheckDeviceFormat(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        d3d_format,
        0,
        D3DRTYPE_TEXTURE,
        D3DFMT_A8R8G8B8
    ) == D3D_OK) {
        SUPERVISOR.__unknown_flag_E = true;
    }
    else {
        SUPERVISOR.__unknown_flag_E = false;
        SUPERVISOR.config.__unknown_flag_B = true;
        LOG_BUFFER.write(JpEnStr("", "Does not support D3DFMT_A8R8G8B8, works in reduced color mode\r\n"));
    }
    WINDOW_DATA.__dword_8 = 0;
    __set_default_d3d_states();
    SUPERVISOR.__dword_828 = 0;
    return 0;
}


template <typename T>
inline T* ole_strstr(T* lhs, const std::remove_const_t<T>* rhs) {
    if constexpr (std::is_same_v<std::remove_pointer_t<T>, char>) {
        return strstr(lhs, rhs);
    } else if constexpr (std::is_same_v<std::remove_pointer_t<T>, wchar_t>) {
        return wcsstr(lhs, rhs);
    }
}

template <typename T, typename ... FormatArgs>
inline int ole_sscanf(const T* buffer, const T* format, FormatArgs... args) {
    if constexpr (std::is_same_v<T, char>) {
        return sscanf(buffer, format, args...);
    } else if constexpr (std::is_same_v<T, wchar_t>) {
        return swscanf(buffer, format, args...);
    }
}

// 0x474CD0
dllexport gnu_noinline BOOL fastcall __joystick_enum_devices_callback(const GUID* joystick_guid) asm_symbol_rel(0x474CD0);
dllexport gnu_noinline BOOL fastcall __joystick_enum_devices_callback(const GUID* joystick_guid) {
    IWbemLocator* wbem_locator = NULL;
    IEnumWbemClassObject* wbem_enum = NULL;
    IWbemClassObject* wbem_objects[20] = {};
    IWbemServices* wbem_services = NULL;
    ULONG wbem_object_count = 0;
    bool com_was_initialized = SUCCEEDED(CoInitialize(NULL));
    BSTR bstrA, bstrB;
    BSTR bstrC = NULL;
    BOOL found_joystick;
    if (
        SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&wbem_locator)) &&
        wbem_locator != NULL &&
        (bstrA = SysAllocString(OLESTR("\\\\.\\root\\cimv2")))
    ) {
        if (
            (bstrB = SysAllocString(OLESTR("Win32_PNPEntity"))) &&
            (bstrC = SysAllocString(OLESTR("DeviceID"))) &&
            SUCCEEDED(wbem_locator->ConnectServer(bstrA, NULL, NULL, NULL, 0, NULL, NULL, &wbem_services)) &&
            wbem_services != NULL &&
            CoSetProxyBlanket(wbem_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE), SUCCEEDED(wbem_services->CreateInstanceEnum(bstrB, 0, NULL, &wbem_enum)) &&
            wbem_enum != NULL
        ) {
            while (SUCCEEDED(wbem_enum->Next(10000, countof(wbem_objects), wbem_objects, &wbem_object_count))) {
                if (!wbem_object_count) {
                    // ole_fail_D
                    found_joystick = false;
                    SysFreeString(bstrA);
                    SysFreeString(bstrC);
                    goto ole_break;
                }
                for (ULONG i = 0; i < wbem_object_count; ++i) {
                    VARIANT device_id;
                    if (
                        SUCCEEDED(wbem_objects[i]->Get(bstrC, 0, &device_id, NULL, NULL)) &&
                        V_VT(&device_id) == VT_BSTR &&
                        V_BSTR(&device_id) &&
                        ole_strstr(V_BSTR(&device_id), OLESTR("IG_"))
                    ) {
                        unsigned int product_id = 0;
                        unsigned int vendor_id = 0;
                        if (BSTR vid_str = ole_strstr(V_BSTR(&device_id), OLESTR("VID_"))) {
                            vendor_id = ole_sscanf(vid_str, OLESTR("VID_%4X"), &vendor_id) == 1 ? vendor_id : 0;
                        }
                        if (BSTR pid_str = ole_strstr(V_BSTR(&device_id), OLESTR("PID_"))) {
                            product_id = ole_sscanf(pid_str, OLESTR("PID_%4X"), &product_id) == 1 ? product_id : 0;
                        }
                        if (MAKELONG(vendor_id, product_id) == joystick_guid->Data1) {
                            found_joystick = true;
                            SysFreeString(bstrA);
                            SysFreeString(bstrC);
                            goto ole_break;
                        }
                    }
                    SAFE_RELEASE(wbem_objects[i]);
                }
            }
            // ole_fail_C
            found_joystick = false;
            SysFreeString(bstrA);
            SysFreeString(bstrC);
        } else {
            // ole_fail_B
            found_joystick = false;
            SysFreeString(bstrA);
            if (bstrC) {
                SysFreeString(bstrC);
            }
        }
ole_break:
        if (bstrB) {
            SysFreeString(bstrB);
        }
    }
    for (ULONG i = 0; i < wbem_object_count; ++i) {
        SAFE_RELEASE(wbem_objects[i]);
    }
    SAFE_RELEASE(wbem_enum);
    SAFE_RELEASE(wbem_locator);
    SAFE_RELEASE(wbem_services);
    if (com_was_initialized) {
        CoUninitialize();
    }
    return found_joystick;
}

// 0x475230
dllexport gnu_noinline BOOL PASCAL EnumDevicesCallback(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvref) asm_symbol_rel(0x475230);
dllexport gnu_noinline BOOL PASCAL EnumDevicesCallback(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvref) {
    if (!__joystick_enum_devices_callback(&lpddi->guidProduct)) {
        if (!SUPERVISOR.joypad_devices[0]) {
            SUPERVISOR.dinput->CreateDevice(
                lpddi->guidInstance,
                &SUPERVISOR.joypad_devices[0],
                NULL
            );
        }
        else {
            return DIENUM_STOP;
        }
    }
    return DIENUM_CONTINUE;
}

// 0x475280
dllexport gnu_noinline BOOL PASCAL EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef) asm_symbol_rel(0x475280);
dllexport gnu_noinline BOOL PASCAL EnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef) {
    DWORD joypad_type = lpddoi->dwType;
    if (joypad_type & DIDFT_AXIS) {
        DIPROPRANGE range;
        range.diph.dwObj = joypad_type;
        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        range.diph.dwHow = DIPH_BYID;
        range.lMin = JOYPAD_MIN_RANGE;
        range.lMax = JOYPAD_MAX_RANGE;
        if (SUCCEEDED(SUPERVISOR.joypad_devices[JOYPAD_INDEX]->SetProperty(DIPROP_RANGE, (LPCDIPROPHEADER)&range))) {
            return DIENUM_STOP;
        }
    }
    return DIENUM_CONTINUE;
}

// 0x475050
dllexport gnu_noinline ZUNResult __initialize_dinput() asm_symbol_rel(0x475050);
dllexport gnu_noinline ZUNResult __initialize_dinput() {
    if (SUCCEEDED(DirectInput8Create(
        WINDOW_DATA.current_instance,
        DIRECTINPUT_VERSION, // 0x800
        IID_IDirectInput8A,
        (LPVOID*)&SUPERVISOR.dinput,
        NULL
    ))) {
        if (SUCCEEDED(SUPERVISOR.dinput->CreateDevice(
            GUID_SysKeyboard,
            &SUPERVISOR.keyboard_device,
            NULL
        ))) {
            goto dinput_init_success;
        }
        SAFE_RELEASE(SUPERVISOR.dinput);
    }
    SUPERVISOR.dinput = NULL;
    LOG_BUFFER.write(JpEnStr("DirectInput が使用できません\r\n", "DirectInput not available\r\n"));
    return ZUN_ERROR;
dinput_init_success:
    if (FAILED(SUPERVISOR.keyboard_device->SetDataFormat(&c_dfDIKeyboard))) {
        SAFE_RELEASE(SUPERVISOR.keyboard_device);
        SAFE_RELEASE(SUPERVISOR.dinput);
        LOG_BUFFER.write(JpEnStr("DirectInput SetDataFormat が使用できません\r\n", "DirectInput SetDataFormat not available\r\n"));
        return ZUN_ERROR;
    }
    if (FAILED(SUPERVISOR.keyboard_device->SetCooperativeLevel(WINDOW_DATA.window, DISCL_NOWINKEY))) {
        SAFE_RELEASE(SUPERVISOR.keyboard_device);
        SAFE_RELEASE(SUPERVISOR.dinput);
        LOG_BUFFER.write(JpEnStr("DirectInput SetCooperativeLevel が使用できません\r\n", "DirectInput SetCooperativeLevel not available\r\n"));
        return ZUN_ERROR;
    }
    SUPERVISOR.keyboard_device->Acquire();
    LOG_BUFFER.write(JpEnStr("DirectInput は正常に初期化されました\r\n", "DirectInput successfully initialized\r\n"));
    SUPERVISOR.dinput->EnumDevices(
        DI8DEVCLASS_GAMECTRL,
        &EnumDevicesCallback,
        NULL,
        DIEDFL_ATTACHEDONLY
    );
    if (LPDIRECTINPUTDEVICE8 joypad_device = SUPERVISOR.joypad_devices[0]) {
        joypad_device->SetDataFormat(&c_dfDIJoystick2);
        SUPERVISOR.joypad_devices[0]->SetCooperativeLevel(WINDOW_DATA.window, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
        SUPERVISOR.__joypad_caps.dwSize = sizeof(DIDEVCAPS);
        SUPERVISOR.joypad_devices[0]->GetCapabilities(&SUPERVISOR.__joypad_caps);
        JOYPAD_INDEX = 0;
        SUPERVISOR.joypad_devices[0]->EnumObjects(
            &EnumDeviceObjectsCallback,
            NULL,
            DIDFT_ALL
        );
        LOG_BUFFER.write(JpEnStr("有効なパッドを発見しました\r\n", "found a valid pad\r\n"));
    }
    return ZUN_SUCCESS;
}

extern "C" {

// 0x471270
gnu_noinline int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
    HINSTANCE current_instance = hInstance;
    int local_dword_18 = 0;
    WINDOW_DATA.current_instance = hInstance;
    WINDOW_DATA.__dword_18 = 0;
    timeBeginPeriod(1);
    CRITICAL_SECTION_MANAGER.initialize();
    DEBUG_LOG_PTR = new DebugLogger();
    DEBUG_LOG_PTR->__dword_0 = 0;
    LOG_BUFFER.write(JpEnStr("東方動作記録 --------------------------------------------- \r\n", "Touhou Operation Record --------------------------------------------- \r\n"));
    if (__make_mutex_and_test_path() != ZUN_ERROR) {
        SUPERVISOR.current_instance = current_instance;
        if (ZUN_SUCCEEDED(WINDOW_DATA.__save_properties_and_configure_paths())) {
            goto winmain_load_config;
        }
        LOG_BUFFER.write_error(JpEnStr("セーブデータが保存できません\r\n", "Save data cannot be saved\r\n"));
    }
winmain_important_label:
    SOUND_MANAGER.__dword_5724 = 2;
    SOUND_MANAGER.cleanup();
    SAFE_DELETE(ANM_MANAGER_PTR);
    SAFE_RELEASE(SUPERVISOR.__surface_1AC);
    SAFE_RELEASE(SUPERVISOR.__surface_1B0);
    SAFE_RELEASE(SUPERVISOR.back_buffer);
    SAFE_RELEASE(SUPERVISOR.d3d_device);
    SAFE_RELEASE(SUPERVISOR.d3d);
    if (HWND window_handle = WINDOW_DATA.window) {
        ShowWindow(window_handle, SW_HIDE);
        MoveWindow(WINDOW_DATA.window, 0, 0, 0, 0, FALSE);
        DestroyWindow(WINDOW_DATA.window);
        WINDOW_DATA.window = NULL;
    }
    while (ShowCursor(true) < 0);
    if (local_dword_18 == 2) {
        LOG_BUFFER.buffer_write = LOG_BUFFER.buffer;
        LOG_BUFFER.buffer[0] = '\0';
        LOG_BUFFER.write(JpEnStr("再起動を要するオプションが変更されたので再起動します\r\n", "An option that requires a reboot has been changed and will be rebooted\r\n"));
    }
    if (!SUPERVISOR.present_parameters.Windowed) {
        WINNLSEnableIME(NULL, true);
    }
    MSG message;
    uint32_t message_count = 60;
    do {
        if (PeekMessageA(&message, NULL, WM_NULL, WM_NULL, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    } while (--message_count);
    SUPERVISOR.__unknown_bitfield_A = 0;
winmain_after_config_loaded:
    IDirect3D9* d3d_ptr = Direct3DCreate9(D3D_SDK_VERSION); // version 32
    SUPERVISOR.d3d = d3d_ptr;
    if (d3d_ptr) {
        goto winmain_d3d_create_success;
    }
    else {
        LOG_BUFFER.write_error(JpEnStr("Direct3D オブジェクトは何故か作成出来なかった\r\n", "Direct3D object could not be created for some reason\r\n"));
        goto winmain_important_label;
    }
    unreachable;
winmain_load_config:
    if (ZUN_FAILED(SUPERVISOR.load_config_file(UNUSED_DWORD))) {
        goto winmain_important_label;
    }
    JOYINFOEX joy_info;
    joy_info.dwSize = sizeof(JOYINFOEX);
    joy_info.dwFlags = JOY_RETURNALL;
    if (
        joyGetPosEx(JOYSTICKID1, &joy_info) == JOYERR_NOERROR &&
        joyGetPosEx(JOYSTICKID2, &joy_info) == JOYERR_NOERROR
    ) {
        SUPERVISOR.__unknown_flag_B = false;
    } else {
        joyGetDevCapsA(JOYSTICKID1, &JOYCAPS_GLOBAL, sizeof(JOYCAPSA));
        SUPERVISOR.__unknown_flag_B = true;
    }
    BYTE keyboard[256];
    GetKeyboardState(keyboard);
    for (int32_t i = 0; i < countof(keyboard); ++i) {
        keyboard[i] &= 0x7F;
    }
    SetKeyboardState(keyboard);
    process_resolution_dialog();
    if (WINDOW_DATA.__unknown_bitfield_B) {
        goto winmain_important_label;
    }
    WINDOW_DATA.__unknown_bitfield_A = SUPERVISOR.config.__ubyte_47;
    char module_name[8]; 
    if (GetModuleFileNameA(NULL, module_name, MAX_PATH + 1)) { // But why is the size wrong...?
        int32_t exe_file_size;
        if (void* module_file_buffer = read_file_to_buffer(module_name, &exe_file_size, true)) {
            int32_t* module_file_read = (int32_t*)module_file_buffer;
            int32_t checksum_low = 0;
            int32_t checksum_high = 0;
            int32_t file_size_in_dwords = exe_file_size / 4;
            int32_t summed_dword_count = 0;
            if (file_size_in_dwords >= 2) {
                int32_t checksum_iters = (uint32_t)(file_size_in_dwords - 2) / 2 - 1;
                summed_dword_count = checksum_iters * 2;
                do {
                    checksum_low += module_file_read[0];
                    checksum_high += module_file_read[1];
                    module_file_read += 2;
                } while (--checksum_iters);
            }
            int32_t checksum = summed_dword_count < file_size_in_dwords ? *module_file_read : 0;
            free(module_file_buffer);
            SUPERVISOR.game_exe_checksum = checksum;
            SUPERVISOR.game_exe_file_size = exe_file_size;
        }
    }
    goto winmain_after_config_loaded;
winmain_d3d_create_success:
    DEVMODEA dev_mode;
    dev_mode.dmPelsWidth = 0;
    // ZUN not following the documentation again by leaving dev_mode uninitialized...
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dev_mode);
    DWORD window_width = dev_mode.dmPelsWidth;
    DWORD window_height = dev_mode.dmPelsHeight;
    WINDOW_DATA.__dword_2060 = window_width;
    WINDOW_DATA.window_width = window_width;
    WINDOW_DATA.__dword_2064 = window_height;
    WINDOW_DATA.window_height = window_height;
    if (WINDOW_DATA.__create_window(current_instance)) {
        goto winmain_important_label;
    }
    __initialize_dinput();
    UNKNOWN_THREAD_A.stop_and_cleanup();
    SOUND_MANAGER.zero_contents();
    SOUND_MANAGER.__thread_5718 = CreateThread(
        NULL,
        0,
        &SoundManager::sound_thread_func,
        &SOUND_MANAGER,
        0,
        &SOUND_MANAGER.__thread_id_5720
    );
    if (__initialize_d3d()) {
        goto winmain_important_label;
    }
    UPDATE_FUNC_REGISTRY_PTR = new UpdateFuncRegistry();
    ANM_MANAGER_PTR = new AnmManager();
    if (SUPERVISOR.present_parameters.Windowed) {
        WINNLSEnableIME(NULL, false);
        while (ShowCursor(false) >= 0);
        SetCursor(NULL);
    }
    WINDOW_DATA.__double_20B0 = 0.0;
    double runtimeA = get_runtime();
    WINDOW_DATA.__double_20A8 = runtimeA;
    WINDOW_DATA.__double_2098 = runtimeA;
    WINDOW_DATA.__double_20A0 = runtimeA;
    runtimeA = get_runtime();
    WINDOW_DATA.__double_20B8 = runtimeA;
    WINDOW_DATA.__double_20C0 = runtimeA;
    SetForegroundWindow(WINDOW_DATA.window);
    ZUNResult result = SUPERVISOR.initialize();
    if (result == ZUN_SUCCESS) {
        WINDOW_DATA.__unknown_flag_A = true;
        local_dword_18 = 0;
        WINDOW_DATA.__sbyte_1C = -4;
        while (!WINDOW_DATA.__dword_8) {
            if (PeekMessageA(&message, NULL, WM_NULL, WM_NULL, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
            else {
                switch (SUPERVISOR.d3d_device->TestCooperativeLevel()) {
                    case D3D_OK:
                        if (!WINDOW_DATA.__unknown_flag_B) {
                            if (WINDOW_DATA.__unknown_flag_C) {
                                local_dword_18 = WINDOW_DATA.update_window__alt_version2();
                            }
                            else if (
                                SUPERVISOR.present_parameters.PresentationInterval == 1 &&
                                SUPERVISOR.config.__ubyte_48 == 0
                            ) {
                                local_dword_18 = WINDOW_DATA.update_window__alt_version();
                            }
                            else {
                                local_dword_18 = WINDOW_DATA.update_window__normal_version();
                            }
                            if (local_dword_18 != 0) {
                                goto loop_break;
                            }
                            SUPERVISOR.__unknown_flag_F = false;
                            break;
                        }
                    case D3DERR_DEVICENOTRESET:
                        WINDOW_DATA.__counter_2044 = 10;
                        if (!WINDOW_DATA.__unknown_flag_B) {
                            D3DFORMAT format;
                            if (WINDOW_DATA.__unknown_bitfield_A <= 2) {
                                GetWindowRect(WINDOW_DATA.window, &SUPERVISOR.window_rect);
                                SUPERVISOR.present_parameters.Windowed = FALSE;
                                format = SUPERVISOR.config.__ubyte_44 ? D3DFMT_R5G6B5 : D3DFMT_A8R8G8B8;
                            }
                            else {
                                format = SUPERVISOR.display_mode.Format;
                                SUPERVISOR.present_parameters.FullScreen_RefreshRateInHz = 0;
                                SUPERVISOR.present_parameters.Windowed = TRUE;
                            }
                            SUPERVISOR.present_parameters.BackBufferFormat = format;
                            WINDOW_DATA.__sub_4734E0(0);
                        }
                        SUPERVISOR.__release_rendering_surfaces();
                        // ANM_MANAGER_PTR->__sub_4771120();
                        if (ZUN_FAILED(__sub_473B20(TRUE))) {
                            goto loop_break;
                        }
                        __set_default_d3d_states();
                        // ANM_MANAGER_PTR->__sub_4711B0();
                        SUPERVISOR.__unknown_flag_F = true;
                        SUPERVISOR.__int_818 = 3;
                        if (WINDOW_DATA.__unknown_flag_B) {

                        }
                }
            }
        }
loop_break:;
    }
    else if (result == ZUN_ERROR) {
        local_dword_18 = -1;
    }
    else {
        local_dword_18 = 2;
    }

}

}