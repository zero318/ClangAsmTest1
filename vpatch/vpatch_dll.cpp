#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <windows.h>
#include <shlwapi.h>

#include "common.h"

#include "..\zero\util.h"
#include "..\zero\custom_intrin.h"

struct Config {
    BOOL initialized; // 0x0
    BOOL enabled; // 0x4
    int32_t x; // 0x8
    int32_t y; // 0xC
    int32_t width; // 0x10
    int32_t height; // 0x14
    BOOL title_bar; // 0x18
    BOOL always_on_top; // 0x1C
    BOOL ask_window_mode; // 0x20
    int32_t vsync; // 0x24
    int32_t blt_prepare_time; // 0x28
    int32_t sleep_type; // 0x2C
    uint32_t auto_blt_prepare_time; // 0x30
    uint32_t game_fps; // 0x34
    int32_t replay_skip_fps; // 0x38
    int32_t replay_slow_fps; // 0x3C
    BOOL calc_fps; // 0x40
    BOOL always_blt; // 0x44
    BOOL allow_short_delay; // 0x48
    BOOL bug_fix_cherry; // 0x4C
    BOOL bug_fix_th10_power3; // 0x50
    BOOL bug_fix_th12_shadow; // 0x54
    BOOL bug_fix_get_device_state; // 0x58
    BOOL fpu_patch; // 0x5C
    BOOL d3d_multithread; // 0x60
    BOOL hook_directinput; // 0x64
    int32_t process_priority; // 0x68
    uint32_t process_affinity_mask; // 0x6C
    BOOL lock_back_buffer; // 0x70
    // 0x74

    void read_from_file(const char* path) {
        this->initialized = TRUE;
        this->enabled = GetPrivateProfileIntA("Window", "enabled", FALSE, path);
        this->x = GetPrivateProfileIntA("Window", "X", INT32_MIN, path);
        this->y = GetPrivateProfileIntA("Window", "Y", INT32_MIN, path);
        this->width = GetPrivateProfileIntA("Window", "Width", 640, path);
        this->height = GetPrivateProfileIntA("Window", "Height", 480, path);
        this->title_bar = GetPrivateProfileIntA("Window", "TitleBar", TRUE, path);
        this->always_on_top = GetPrivateProfileIntA("Window", "AlwaysOnTop", FALSE, path);
        this->ask_window_mode = GetPrivateProfileIntA("Window", "AskWindowMode", FALSE, path);
        this->vsync = GetPrivateProfileIntA("Option", "Vsync", 0, path);
        this->blt_prepare_time = GetPrivateProfileIntA("Option", "BltPrepareTime", 4, path);
        this->sleep_type = GetPrivateProfileIntA("Option", "SleepType", 1, path);
        this->auto_blt_prepare_time = GetPrivateProfileIntA("Option", "AutoBltPrepareTime", 1, path);
        this->game_fps = GetPrivateProfileIntA("Option", "GameFPS", 60, path);
        this->replay_skip_fps = GetPrivateProfileIntA("Option", "ReplaySkipFPS", 240, path);
        this->replay_slow_fps = GetPrivateProfileIntA("Option", "ReplaySlowFPS", 30, path);
        this->calc_fps = GetPrivateProfileIntA("Option", "CalcFPS", TRUE, path);
        this->always_blt = GetPrivateProfileIntA("Option", "AlwaysBlt", FALSE, path);
        this->allow_short_delay = GetPrivateProfileIntA("Option", "AllowShortDelay", FALSE, path);
        this->bug_fix_cherry = GetPrivateProfileIntA("Option", "BugFixCherry", TRUE, path);
        this->bug_fix_th10_power3 = GetPrivateProfileIntA("Option", "BugFixTh10Power3", FALSE, path);
        this->bug_fix_th12_shadow = GetPrivateProfileIntA("Option", "BugFixTh12Shadow", FALSE, path);
        this->bug_fix_get_device_state = GetPrivateProfileIntA("Option", "BugFixGetDeviceState", TRUE, path);
        this->fpu_patch = GetPrivateProfileIntA("Option", "FpuPatch", FALSE, path);
        this->d3d_multithread = GetPrivateProfileIntA("Option", "D3DMultiThread", FALSE, path);
        this->hook_directinput = GetPrivateProfileIntA("Option", "HookDirectInput", FALSE, path);
        this->process_priority = GetPrivateProfileIntA("Option", "ProcessPriority", 0, path);
        this->process_affinity_mask = GetPrivateProfileIntA("Option", "ProcessAffinityMask", 0, path);
        this->lock_back_buffer = GetPrivateProfileIntA("Option", "LockBackBuffer", FALSE, path);

        if (this->game_fps < 60) {
            this->game_fps = 60;
        }
        if (this->blt_prepare_time < 0) {
            this->auto_blt_prepare_time = abs(this->blt_prepare_time);
            if (!this->auto_blt_prepare_time) {
                this->auto_blt_prepare_time = 1;
            }
            if (this->auto_blt_prepare_time > 1000) {
                this->auto_blt_prepare_time = 1000;
            }
            this->blt_prepare_time = 4;
        }
        else {
            if (this->blt_prepare_time > 1000) {
                this->blt_prepare_time = 1000;
            }
        }
        if (this->always_blt) {
            this->bug_fix_get_device_state = TRUE;
        }
    }

    void initialize(const char* config_path) {
        char path[MAX_PATH];
        if (!this->initialized) {
            if (!config_path) {
                GetModuleFileNameA(NULL, path, countof(path));
                PathRemoveFileSpecA(path);
                PathAddBackslashA(path);
                strcat(path, "vpatch.ini");
                config_path = path;
            }
            this->read_from_file(config_path);
        }
    }
};


#pragma section("shared",read,write,shared)
#pragma comment(linker, "/SECTION:shared,RWS")
__declspec(allocate("shared"))
Config CONFIG;

struct UnknownA {
    LARGE_INTEGER qpc_freq; // 0x0
    double __double_8; // 0x8
    double __double_10; // 0x10
    double __double_18; // 0x18
    double __double_20; // 0x20
    double __double_28; // 0x28
    int64_t __long_array_30[32]; // 0x30
    int __dword_130; // 0x130
    // 0x134

    UnknownA() {
        QueryPerformanceFrequency(&this->qpc_freq);
    }
};

static UnknownA UNKNOWN_A;

struct UnknownB {
    int __dword_0; // 0x0
    int __int_4; // 0x4
    int __dword_8; // 0x8
    int __dword_C; // 0xC
    int32_t __int_10; // 0x10
    HANDLE __handle_14; // 0x14
    int __dword_18; // 0x18
    // 0x1C
    int64_t __qword_20; // 0x20
    int64_t __qword_28; // 0x28
    int64_t __qword_30; // 0x30
    int64_t __qword_38; // 0x38
    int64_t __qword_array_40[512]; // 0x40
    int __dword_1040; // 0x1040
    int __int_1044; // 0x1044
    int64_t __qword_1048; // 0x1048
    int __int_1050; // 0x1050
    // 0x1054
    int64_t __qword_1058; // 0x1058
    int64_t __qword_1060; // 0x1060
    int64_t __qword_array_1068[64]; // 0x1068
    int64_t __qword_array_1268[64]; // 0x1268
    int __dword_1468; // 0x1468
    // 0x146C
    int64_t __qword_1470; // 0x1470
    int64_t __qword_1478; // 0x1478
    int64_t __qword_1480; // 0x1480
    int __dword_1488; // 0x1488
    // 0x148C
    int64_t __qword_1490; // 0x1490
    int64_t __qword_1498; // 0x1498
    int __int_14A0; // 0x14A0
    int __int_14A4; // 0x14A4
    int __int_14A8; // 0x14A8
    // 0x14AC
    int __dword_14B0; // 0x14B0
    int __dword_14B4; // 0x14B4
    int __dword_14B8; // 0x14B8
    int __dword_14BC; // 0x14BC
    int64_t __qword_14C0; // 0x14C0
    int __dword_14C8; // 0x14C8
    int __dword_14CC; // 0x14CC
    int __dword_14D0; // 0x14D0
    int __dword_14D4; // 0x14D4
    int __dword_14D8; // 0x14D8
    int __dword_14DC; // 0x14DC
    int __dword_14E0; // 0x14E0
    CRITICAL_SECTION __critical_section_14E4; // 0x14E4

    int __dword_1500; // 0x1500
    int __dword_1504; // 0x1504
    // 0x150C

    int __dword_1510; // 0x1510
    // 0x1514
    BOOL sse2; // 0x1518
    int __dword_151C; // 0x151C
    // 0x1520

    UnknownB() {
        this->__dword_0 = 0;
        this->__int_4 = 60;
        this->__dword_8 = 0;
        this->__dword_C = 0;
        this->__int_10 = -1;
        this->__handle_14 = NULL;
        this->__dword_18 = 0;
        this->__qword_20 = 0;
        this->__qword_28 = 0;
        this->__qword_30 = 0;
        this->__qword_38 = 0;
        this->__qword_array_40[0] = 0;
        for (size_t i = 0; i < countof(this->__qword_array_40) - 1; ++i) {
            this->__qword_array_40[i + 1] = this->__qword_array_40[i];
        }
        this->__dword_1040 = 0;
        this->__int_1044 = 1;
        this->__qword_1048 = 0;
        this->__int_1050 = 500;
        this->__qword_1058 = 0;
        this->__qword_1060 = 0;
        this->__qword_array_1068[0] = 0;
        for (size_t i = 0; i < countof(this->__qword_array_1068) - 1; ++i) {
            this->__qword_array_1068[i + 1] = this->__qword_array_1068[i];
        }
        this->__qword_array_1268[0] = 0;
        for (size_t i = 0; i < countof(this->__qword_array_1268) - 1; ++i) {
            this->__qword_array_1268[i + 1] = this->__qword_array_1268[i];
        }
        this->__dword_1468 = 0;
        this->__qword_1470 = 0;
        this->__qword_1478 = 0;
        this->__qword_1480 = 0;
        this->__dword_1488 = 0;
        this->__qword_1490 = 0;
        this->__qword_1498 = 0;
        this->__dword_14B8 = 0;
        this->__dword_14BC = 0;
        this->__qword_14C0 = 0;
        this->__dword_14C8 = 0;
        this->__dword_14CC = 0;
        this->__dword_14D0 = 0;
        this->__dword_14D4 = 0;
        this->__dword_14D8 = 0;
        this->__dword_14DC = 0;
        this->__dword_14E0 = 0;
        InitializeCriticalSectionAndSpinCount(&this->__critical_section_14E4, 1000);
        this->__dword_1500 = 0;
        this->__dword_1504 = 0;
        this->__dword_1510 = 0;
        this->sse2 = IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE);
    }

    ~UnknownB() {
        DeleteCriticalSection(&this->__critical_section_14E4);
    }
};

// Rx18AB0
static UnknownB UNKNOWN_B;

__attribute__((used)) double __stdcall muldiv(double A, double B, double C) {
    double ret = 0.0;
    if (UNKNOWN_B.sse2) {
        /*
        __asm__(
            "MOVSD %[A], %%XMM0 \n"
            "MULSD %[B], %%XMM0 \n"
            "DIVSD %[C], %%XMM0 \n"
            "MOVSD %%XMM0, %[ret]"
            : [ret]"=m"(ret)
            : [A]"m"(A), [B]"m"(B), [C]"m"(C)
        );
        */
    }
    else {
        FCW old_fcw = store_x87_cw();
        load_x87_cw(FCW{ 0x27F });
        ret = A * B / C;
        load_x87_cw(old_fcw);
    }
    return ret;
}

// Rx18AA8
static HINSTANCE INSTANCE;

static const char EXE_NAME[] = "th10.exe";
static constexpr size_t EXE_NAME_LENGTH = countof(EXE_NAME) - 1;

#define WINMAIN_CALL_ADDR 0x45395C
#define WINMAIN_FUNC_ADDR 0x438AD0

void mem_write(void* dst, void* src, size_t size) {
    DWORD old_protect;
    VirtualProtect(dst, size, PAGE_EXECUTE_WRITECOPY, &old_protect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, old_protect, &old_protect);
    FlushInstructionCache(GetCurrentProcess(), dst, size);
}

template <typename T, typename R>
static inline void patch_rel32(T target, R replacement) {
    int32_t rel32 = (uintptr_t)replacement - (uintptr_t)target - 4;
    mem_write((void*)target, &rel32, sizeof(int32_t));
}

void setupA1() {

}

void setupA() {
    CONFIG.initialize(NULL);

    int32_t priority = CONFIG.process_priority;
    if (priority) {
        priority += 2;
        if (priority <= 0) {
            priority = 0;
        }
        if (priority >= 4) {
            priority = 4;
        }
        DWORD priority_lookup[] = {
            IDLE_PRIORITY_CLASS,
            BELOW_NORMAL_PRIORITY_CLASS,
            NORMAL_PRIORITY_CLASS,
            ABOVE_NORMAL_PRIORITY_CLASS,
            HIGH_PRIORITY_CLASS
        };
        SetPriorityClass(GetCurrentProcess(), priority_lookup[priority]);
    }

    DWORD_PTR new_affinity_mask = CONFIG.process_affinity_mask;
    if (new_affinity_mask) {
        DWORD_PTR process_affinity_mask;
        DWORD_PTR system_affinity_mask;
        if (GetProcessAffinityMask(GetCurrentProcess(), &process_affinity_mask, &system_affinity_mask)) {
            new_affinity_mask &= process_affinity_mask;
            if (new_affinity_mask) {
                SetProcessAffinityMask(GetCurrentProcess(), new_affinity_mask);
            }
        }
    }

    setupA1();
}

void winmain_setup() {
    setupA();

}

typedef int WINAPI WinMain_t(HINSTANCE, HINSTANCE, PSTR, int);

int WINAPI WinMain_hook(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
    winmain_setup();
    return ((WinMain_t*)WINMAIN_FUNC_ADDR)(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}

void dllmain_setup() {
    char path[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, path, countof(path));
    if (length && length >= EXE_NAME_LENGTH && !_stricmp(&path[length - EXE_NAME_LENGTH], EXE_NAME)) {
        patch_rel32(WINMAIN_CALL_ADDR, WinMain_hook);
    }
}

HANDLE create_process(HANDLE* thread_out) {
    char path[256];
    DWORD length = GetModuleFileNameA((HMODULE)INSTANCE, path, countof(path));
    if (!length) {
        path[0] = '\0';
    }
    PathRemoveFileSpecA(path);
    PathAppendA(path, EXE_NAME);

    PROCESS_INFORMATION pi;
    STARTUPINFOA si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    if (!CreateProcessA(path, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
        return 0;
    }
    *thread_out = pi.hThread;
    return pi.hProcess;
}

extern "C" {
    dllexport InitializeReturnCode __stdcall Initialize(const char* config_path) {
        HANDLE main_thread;
        HANDLE process = create_process(&main_thread);
        if (!process) {
            return INIT_PROCESS_FAILED;
        }
        HMODULE self = (HMODULE)INSTANCE;
        if (!self) {
            return INIT_UNKNOWN_ERROR;
        }
        char path[MAX_PATH];
        DWORD length = GetModuleFileNameA(self, path, countof(path));
        if (!length) {
            return INIT_UNKNOWN_ERROR;
        }
        CONFIG.initialize(config_path);
        void* path_page = VirtualAllocEx(process, NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(process, path_page, path, MAX_PATH, NULL);

        HANDLE remote_thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32"), "LoadLibraryA"), path_page, CREATE_SUSPENDED, NULL);
        if (!remote_thread) {
            return INIT_INJECT_FAILED;
        }
        ResumeThread(remote_thread);
        WaitForSingleObject(remote_thread, INFINITE);
        DWORD exit_code = 0;
        GetExitCodeThread(remote_thread, &exit_code);
        CloseHandle(remote_thread);
        VirtualFreeEx(process, path_page, MAX_PATH, MEM_RELEASE);
        ResumeThread(main_thread);
        CloseHandle(main_thread);
        CloseHandle(process);
        return exit_code ? INIT_SUCCESS : INIT_INJECT_FAILED;
    }

    BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
        if (fdwReason == DLL_PROCESS_ATTACH) {
            INSTANCE = hinstDLL;
            dllmain_setup();
        }
        return TRUE;
    }
}