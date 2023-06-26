#pragma clang diagnostic ignored "-Wc++17-extensions"
#include <stdlib.h>
#include <stdint.h>

#include <Windows.h>

#define musttail [[clang::musttail]]

// Clang does this silly thing where it'll
// pointlessly move stack arguments back and
// forth when making a tail call, so just
// manually jump to the correct address
// when you know it's safe.
#define jump_to_mem_addr(var) __asm__("jmp* %0"::"m"(var)); __builtin_unreachable()

#if __INTELLISENSE__
// Intellisense doesn't know what
// constinit is when using clang
#define constinit constexpr
#endif

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

static DWORD WINAPI XInputGetStateShim(DWORD dwUserIndex, void* pState);
static DWORD WINAPI XInputSetStateShim(DWORD dwUserIndex, void* pVibration);
static DWORD WINAPI XInputGetCapabilitiesShim(DWORD dwUserIndex, DWORD dwFlags, void* pCapabilities);
static void WINAPI XInputEnableShim(BOOL enable);
static DWORD WINAPI XInputGetBatteryInformationShim(DWORD dwUserIndex, BYTE devType, void* pBatteryInformation);
static DWORD WINAPI XInputGetKeystrokeShim(DWORD dwUserIndex, DWORD dwReserved, void* pKeystroke);
static DWORD WINAPI XInputGetAudioDeviceIdsShim(DWORD  dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount);

#ifndef __x86_64__
static void WINAPI HaltAndCatchFire1V(uintptr_t) {
	__asm__("int3");
}
static uintptr_t WINAPI HaltAndCatchFire2O(uintptr_t, uintptr_t) {
	__asm__("int3");
	return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire3O(uintptr_t, uintptr_t, uintptr_t) {
	__asm__("int3");
	return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire5O(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	__asm__("int3");
	return ERROR_INVALID_FUNCTION;
}
#else
static void WINAPI HaltAndCatchFireV() {
	__asm__("int3");
}
static uintptr_t WINAPI HaltAndCatchFireO() {
	__asm__("int3");
	return ERROR_INVALID_FUNCTION;
}
#define HaltAndCatchFire1V HaltAndCatchFireV
#define HaltAndCatchFire2O HaltAndCatchFireO
#define HaltAndCatchFire3O HaltAndCatchFireO
#define HaltAndCatchFire5O HaltAndCatchFireO
#endif

typedef decltype(&XInputGetStateShim) XInputGetStatePtr;
typedef decltype(&XInputSetStateShim) XInputSetStatePtr;
typedef decltype(&XInputGetCapabilitiesShim) XInputGetCapabilitiesPtr;
typedef decltype(&XInputEnableShim) XInputEnablePtr;
typedef decltype(&XInputGetBatteryInformationShim) XInputGetBatteryInformationPtr;
typedef decltype(&XInputGetKeystrokeShim) XInputGetKeystrokePtr;
typedef decltype(&XInputGetAudioDeviceIdsShim) XInputGetAudioDeviceIdsPtr;

static XInputGetStatePtr XInputGetStateResolved = &XInputGetStateShim;
static XInputSetStatePtr XInputSetStateResolved = &XInputSetStateShim;
static XInputGetCapabilitiesPtr XInputGetCapabilitiesResolved = &XInputGetCapabilitiesShim;
static XInputEnablePtr XInputEnableResolved = &XInputEnableShim;
static XInputGetBatteryInformationPtr XInputGetBatteryInformationResolved = &XInputGetBatteryInformationShim;
static XInputGetKeystrokePtr XInputGetKeystrokeResolved = &XInputGetKeystrokeShim;
static XInputGetAudioDeviceIdsPtr XInputGetAudioDeviceIdsResolved = &XInputGetAudioDeviceIdsShim;

static inline const void* *const xinput_func_ptrs[] = {
	(const void**const)&XInputGetStateResolved,
	(const void**const)&XInputSetStateResolved,
	(const void**const)&XInputGetCapabilitiesResolved,
	(const void**const)&XInputEnableResolved,
	(const void**const)&XInputGetBatteryInformationResolved,
	(const void**const)&XInputGetKeystrokeResolved,
	(const void**const)&XInputGetAudioDeviceIdsResolved
};

static inline constexpr const wchar_t *const xinput_dlls[] = {
	L"\\xinput1_4.dll",
	L"\\xinput1_3.dll",
	L"\\XInput9_1_0.dll"
};

static inline constexpr size_t longest_dll_name = [] constexpr {
	size_t max_length = 0;
	for (size_t i = 0; i < countof(xinput_dlls); ++i) {
		size_t j = 0;
		while (xinput_dlls[i][j]) ++j;
		if (j > max_length) {
			max_length = j;
		}
	}
	return max_length;
}();

static inline constinit const char *const xinput_funcs[] = {
	"XInputGetState",
	"XInputSetState",
	"XInputGetCapabilities",
	"XInputEnable",
	"XInputGetBatteryInformation",
	"XInputGetKeystroke",
	"XInputGetAudioDeviceIds"
};

static inline constinit const void *const xinput_halt_funcs[] = {
	(void*)&HaltAndCatchFire2O,
	(void*)&HaltAndCatchFire2O,
	(void*)&HaltAndCatchFire3O,
	(void*)&HaltAndCatchFire1V,
	(void*)&HaltAndCatchFire3O,
	(void*)&HaltAndCatchFire3O,
	(void*)&HaltAndCatchFire5O
};

static wchar_t path_buffer[MAX_PATH + longest_dll_name];

static void resolve_xinput_funcs() {
	size_t system_dir_size = GetSystemDirectoryW(path_buffer, countof(path_buffer));
	if (system_dir_size <= MAX_PATH) {
		for (size_t i = 0; i < countof(xinput_dlls); ++i) {
			for (
				size_t j = 0;
				(path_buffer[system_dir_size + j] = xinput_dlls[i][j]);
				++j
			);
			if (HMODULE xinput_lib = LoadLibraryW(path_buffer)) {
				for (size_t j = 0; j < countof(xinput_funcs); ++j) {
					*xinput_func_ptrs[j] = (void*)GetProcAddress(xinput_lib, xinput_funcs[j]) ? : xinput_halt_funcs[j];
				}
				return;
			}
		}
	}
	for (size_t i = 0; i < countof(xinput_funcs); ++i) {
		*xinput_func_ptrs[i] = xinput_halt_funcs[i];
	}
}

static DWORD WINAPI XInputGetStateShim(DWORD dwUserIndex, void* pState) {
	resolve_xinput_funcs();
#ifndef __x86_64__
	jump_to_mem_addr(XInputGetStateResolved);
#else
	musttail return XInputGetStateResolved(dwUserIndex, pState);
#endif
}

static DWORD WINAPI XInputSetStateShim(DWORD dwUserIndex, void* pVibration) {
	resolve_xinput_funcs();
#ifndef __x86_64__
	jump_to_mem_addr(XInputSetStateResolved);
#else
	musttail return XInputSetStateResolved(dwUserIndex, pVibration);
#endif
}

static DWORD WINAPI XInputGetCapabilitiesShim(DWORD dwUserIndex, DWORD dwFlags, void* pCapabilities) {
	resolve_xinput_funcs();
#ifndef __x86_64__
	jump_to_mem_addr(XInputGetCapabilitiesResolved);
#else
	musttail return XInputGetCapabilitiesResolved(dwUserIndex, dwFlags, pCapabilities);
#endif
}

static void WINAPI XInputEnableShim(BOOL enable) {
	resolve_xinput_funcs();
#ifndef __x86_64__
	jump_to_mem_addr(XInputEnableResolved);
#else
	musttail return XInputEnableResolved(enable);
#endif
}

static DWORD WINAPI XInputGetBatteryInformationShim(DWORD dwUserIndex, BYTE devType, void* pBatteryInformation) {
	resolve_xinput_funcs();
#ifndef __x86_64__
	jump_to_mem_addr(XInputGetBatteryInformationResolved);
#else
	musttail return XInputGetBatteryInformationResolved(dwUserIndex, devType, pBatteryInformation);
#endif
}

static DWORD WINAPI XInputGetKeystrokeShim(DWORD dwUserIndex, DWORD dwReserved, void* pKeystroke) {
	resolve_xinput_funcs();
#ifndef __x86_64__
	jump_to_mem_addr(XInputGetKeystrokeResolved);
#else
	musttail return XInputGetKeystrokeResolved(dwUserIndex, dwReserved, pKeystroke);
#endif
}

static DWORD WINAPI XInputGetAudioDeviceIdsShim(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount) {
	resolve_xinput_funcs();
#ifndef __x86_64__
	jump_to_mem_addr(XInputGetAudioDeviceIdsResolved);
#else
	musttail return XInputGetAudioDeviceIdsResolved(dwUserIndex, pRenderDeviceId, pRenderCount, pCaptureDeviceId, pCaptureCount);
#endif
}

extern "C" {

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	return TRUE;
}

DWORD WINAPI XInputGetState(DWORD dwUserIndex, void* pState) {
	jump_to_mem_addr(XInputGetStateResolved);
}

DWORD WINAPI XInputSetState(DWORD dwUserIndex, void* pVibration) {
	jump_to_mem_addr(XInputSetStateResolved);
}

DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, void* pCapabilities) {
	jump_to_mem_addr(XInputGetCapabilitiesResolved);
}

void WINAPI XInputEnable(BOOL enable) {
	jump_to_mem_addr(XInputEnableResolved);
}

DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, void* pBatteryInformation) {
	jump_to_mem_addr(XInputGetBatteryInformationResolved);
}

DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, void* pKeystroke) {
	jump_to_mem_addr(XInputGetKeystrokeResolved);
}

DWORD WINAPI XInputGetAudioDeviceIds(DWORD  dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount) {
	jump_to_mem_addr(XInputGetAudioDeviceIdsResolved);
}

}