#include "../zero/force_macros.h"
#include <stdint.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <float.h>

#include "../zero/util.h"

#include "Windows.h"
#include <joystickapi.h>
#include <stdnoreturn.h>
#include <winternl.h>
#include <atomic>
#include <XInput.h>

#include "../zero/thcrap_util.h"

// After compiling the code:
// 1. Open the exe in OllyDbg
// 2. Copy the function out of the disassembly window into Notepad++
// 3. Use the following regex to convert to codecave format
// 
// Find:
// ^[0-9A-F]+[^0-9A-F]*(?<bytes>(?:(?:[0-9A-F]+[ :](?!  ))|[0-9A-F]+)+?)(?<replace_bytes>[0-9A-F]{8})?(?<space> )(?:.*?{(?<replacement>.*)})?(?<post_bytes>[0-9A-F]+ )?.*(?<line>\r\n)?
// Replace:
// $+{bytes}(?{replacement}(?{replace_bytes}$+{replacement}:):$+{replace_bytes})$+{space}(?{post_bytes}$+{post_bytes} :)\\$+{line}

thcrap_string(kernel32_dll_str, "kernel32.dll");
thcrap_string(user32_dll_str, "USER32.dll");
thcrap_string(thcrap_loader_str, "\\bin\\thcrap_loader.exe");

thcrap_string(xinput_1_4_dll_str, "xinput1_4.dll");
thcrap_string(xinput_1_3_dll_str, "xinput1_3.dll");
thcrap_string(xinput9_1_0_dll_str, "XInput9_1_0.dll");

dllexport char* eosd_multi_xinput_lib_names[] asm_symbol_expr(<codecave:eosd_multi_constant_pool> + 0x90) = {
	xinput_1_4_dll_str,
	xinput_1_3_dll_str,
	xinput9_1_0_dll_str
};
dllexport char dummy_name asm_symbol_expr(<codecave:eosd_multi_constant_pool> +0x9C);

thcrap_string(OpenFileMappingA_str, "OpenFileMappingA");
thcrap_import(OpenFileMappingA);
thcrap_string(CreateFileMappingA_str, "CreateFileMappingA");
thcrap_import(CreateFileMappingA);
thcrap_string(MapViewOfFile_str, "MapViewOfFile");
thcrap_import(MapViewOfFile);
thcrap_string(UnmapViewOfFile_str, "UnmapViewOfFile");
thcrap_import(UnmapViewOfFile);
thcrap_string(CreateProcessA_str, "CreateProcessA");
thcrap_import(CreateProcessA);
thcrap_string(CloseHandle_str, "CloseHandle");
thcrap_import(CloseHandle);
thcrap_string(Sleep_str, "Sleep");
thcrap_import(Sleep);
thcrap_string(LoadLibraryA_str, "LoadLibraryA");
thcrap_import(LoadLibraryA);
thcrap_string(XInputGetState_str, "XInputGetState");
thcrap_import(XInputGetState);
thcrap_string(GetKeyState_str, "GetKeyState");
thcrap_import(GetKeyState);
thcrap_string(CreateDirectoryA_str, "CreateDirectoryA");
thcrap_import(CreateDirectoryA);
thcrap_string(ExitProcess_str, "ExitProcess");
thcrap_import(ExitProcess);
thcrap_string(GetWindowRect_str, "GetWindowRect");
thcrap_import(GetWindowRect);
thcrap_string(SetWindowPos_str, "SetWindowPos");
thcrap_import(SetWindowPos);

thcrap_string(eosd_multi_main_folder_str, "./FourOfAKind/");
thcrap_string(eosd_multi_replay_folder_files_str, "./FourOfAKind/replay/%s");
thcrap_string(eosd_multi_replay_folder_str, "./FourOfAKind/replay/");
thcrap_string(eosd_multi_title_E_str, "The Embodiment of Scarlet Devil Easy");
thcrap_string(eosd_multi_title_N_str, "The Embodiment of Scarlet Devil Normal");
thcrap_string(eosd_multi_title_H_str, "The Embodiment of Scarlet Devil Hard");
thcrap_string(eosd_multi_title_L_str, "The Embodiment of Scarlet Devil Lunatic");
thcrap_string(eosd_multi_score_dat_filename_E_str, "./FourOfAKind/scoreE.dat");
thcrap_string(eosd_multi_score_dat_filename_N_str, "./FourOfAKind/scoreN.dat");
thcrap_string(eosd_multi_score_dat_filename_H_str, "./FourOfAKind/scoreH.dat");
thcrap_string(eosd_multi_score_dat_filename_L_str, "./FourOfAKind/scoreL.dat");
thcrap_string(eosd_multi_log_filename_E_str, "./FourOfAKind/logE.txt");
thcrap_string(eosd_multi_log_filename_N_str, "./FourOfAKind/logN.txt");
thcrap_string(eosd_multi_log_filename_H_str, "./FourOfAKind/logH.txt");
thcrap_string(eosd_multi_log_filename_L_str, "./FourOfAKind/logL.txt");
thcrap_string(eosd_multi_replay_filename_E_str, "./FourOfAKind/replay/th6E_%2d.rpy");
thcrap_string(eosd_multi_replay_filename_N_str, "./FourOfAKind/replay/th6N_%2d.rpy");
thcrap_string(eosd_multi_replay_filename_H_str, "./FourOfAKind/replay/th6H_%2d.rpy");
thcrap_string(eosd_multi_replay_filename_L_str, "./FourOfAKind/replay/th6L_%2d.rpy");
thcrap_string(eosd_multi_replay_default_filename_E_str, "FourOfAKind/replay/th6E_00.rpy");
thcrap_string(eosd_multi_replay_default_filename_N_str, "FourOfAKind/replay/th6N_00.rpy");
thcrap_string(eosd_multi_replay_default_filename_H_str, "FourOfAKind/replay/th6H_00.rpy");
thcrap_string(eosd_multi_replay_default_filename_L_str, "FourOfAKind/replay/th6L_00.rpy");
thcrap_string(eosd_multi_replay_ud_filename_E_str, "th6E_ud????.rpy");
thcrap_string(eosd_multi_replay_ud_filename_N_str, "th6N_ud????.rpy");
thcrap_string(eosd_multi_replay_ud_filename_H_str, "th6H_ud????.rpy");
thcrap_string(eosd_multi_replay_ud_filename_L_str, "th6L_ud????.rpy");

thcrap_string(eosd_multi_vpatch_override_str, "eosd_multi_vpatch_override");
thcrap_string(eosd_multi_mapping_name, "EoSDMulti");

thcrap_string(eosd_multi_error_caption_str, "Four of a Kind Error");
thcrap_string(eosd_multi_invalid_max_count_format_str, "Max count option must be between 1 and 4!");
thcrap_string(eosd_multi_invalid_memshare_size_format_str, "Memshare size must be at least 68 bytes!");
thcrap_string(eosd_multi_main_folder_fail_format_str, "Could not create FourOfAKind folder!");
thcrap_string(eosd_multi_replay_folder_fail_format_str, "Could not create FourOfAKind/replay folder!");

// On x86, atomic reads don't need to use LOCK anyway
// and code gen gets less confused if you use the "as_normal"
// variant of this field. *Probably* still a bad idea though.
template <typename T>
union CustomSyncType {
	T as_normal;
	std::atomic<T> as_atomic;
	volatile T as_volatile;
	volatile std::atomic<T> as_volatile_atomic;
};

template <typename T, size_t size>
union CustomSyncArray {
	T as_normal[size];
	std::atomic<T> as_atomic[size];
	volatile T as_volatile[size];
	volatile std::atomic<T> as_volatile_atomic[size];
};

struct EoSDMultiSharedData {
	CustomSyncType<uint8_t> next_index;
	padding_bytes(3);
	CustomSyncType<uint8_t> read_size_index;
	padding_bytes(3);
	CustomSyncType<uint8_t> resize_index;
	padding_bytes(3);
	padding_bytes(4);
	//LONG x_positions[4];
	//LONG y_positions[4];
	//HWND window_hwnds[8];
	CustomSyncArray<LONG, 4> x_positions;
	CustomSyncArray<LONG, 4> y_positions;
	CustomSyncArray<HWND, 8> window_hwnds;
};
ValidateStructSize(0x50, EoSDMultiSharedData);

dllexport uint8_t instance_index asm_symbol_abs(option:eosd_multi_instance_index_ptr);
dllexport EoSDMultiSharedData* shared_data_ptr asm_symbol_abs(option:eosd_multi_shared_data_ptr);

static constexpr uint8_t eosd_multi_max_count = 4;

dllexport forceinline noreturn void fastcall eosd_multi_vfail_exit(const char* message, va_list va) asm_symbol_rel(codecave:eosd_multi_vfail_exit);
dllexport forceinline noreturn void fastcall eosd_multi_vfail_exit(const char* message, va_list va) {
	log_vmboxf(eosd_multi_error_caption_str, MB_OK, message, va);
	((ExitProcessPtr)th_GetProcAddress(th_GetModuleHandleA(kernel32_dll_str), ExitProcess_str))(EXIT_FAILURE);
}
dllexport gnu_noinline noreturn void regparm(1) eosd_multi_fail_exit(const char* message, ...) asm_symbol_rel(codecave:eosd_multi_fail_exit);
dllexport gnu_noinline noreturn void regparm(1) eosd_multi_fail_exit(const char* message, ...) {
	va_list va;
	va_start(va, message);
	eosd_multi_vfail_exit(message, va);
}

dllexport gnu_noinline noreturn void eosd_multi_invalid_max_count_patch_init() asm_symbol_rel(codecave:eosd_multi_invalid_max_count_patch_init);
dllexport gnu_noinline noreturn void eosd_multi_invalid_max_count_patch_init() {
	eosd_multi_fail_exit(eosd_multi_invalid_max_count_format_str);
}

dllexport gnu_noinline noreturn void eosd_multi_invalid_memshare_size_patch_init() asm_symbol_rel(codecave:eosd_multi_invalid_memshare_size_patch_init);
dllexport gnu_noinline noreturn void eosd_multi_invalid_memshare_size_patch_init() {
	eosd_multi_fail_exit(eosd_multi_invalid_memshare_size_format_str);
}

// This function only needs to run on one of the four processes,
// so it's not optimized quite as much. I just didn't care. :ZUNLUL:
dllexport gnu_noinline void fastcall spawn_children_processes(HMODULE kernel_handle, GetProcAddressPtr get_proc_address_shim) asm_symbol_rel(codecave:eosd_multi_spawn_children_processes);
dllexport gnu_noinline void fastcall spawn_children_processes(HMODULE kernel_handle, GetProcAddressPtr get_proc_address_shim) {
	const char* thcrap_dir = runconfig_thcrap_dir_get();
	const char* runconfig_filename = runconfig_runcfg_fn_get();
	const char* runconfig_game_id = runconfig_game_get();
	size_t thcrap_dir_len = th_strlen(thcrap_dir);
	size_t runconfig_filename_len = th_strlen(runconfig_filename);
	size_t runconfig_game_id_len = th_strlen(runconfig_game_id);
	size_t total_size = thcrap_dir_len + sizeof(thcrap_loader_str) + runconfig_filename_len + runconfig_game_id_len + 7; // The 7 is for the extra quotes/spaces/null
	// Would love to use a VLA for this instead of a heap
	// allocation, but that generates a call to _chkstk
	// and that isn't available as a thcrap builtin.
	char* command_line_writer = (char*)th_malloc(total_size);
	char* command_line = command_line_writer;
	*command_line_writer++ = '"';
	th_memcpy(command_line_writer, thcrap_dir, thcrap_dir_len);
	command_line_writer += thcrap_dir_len;
	th_memcpy(command_line_writer, thcrap_loader_str, sizeof(thcrap_loader_str) - 1);
	command_line_writer += sizeof(thcrap_loader_str) - 1;
	*command_line_writer++ = '"';
	*command_line_writer++ = ' ';
	*command_line_writer++ = '"';
	th_memcpy(command_line_writer, runconfig_filename, runconfig_filename_len);
	command_line_writer += runconfig_filename_len;
	*command_line_writer++ = '"';
	*command_line_writer++ = ' ';
	th_memcpy(command_line_writer, runconfig_game_id, runconfig_game_id_len);
	command_line_writer += runconfig_game_id_len;
	*command_line_writer++ = '\0';
	auto* create_process_a = (CreateProcessAPtr)get_proc_address_shim(kernel_handle, CreateProcessA_str);
	auto* close_handle = (CloseHandlePtr)get_proc_address_shim(kernel_handle, CloseHandle_str);

	nounroll for (size_t i = 0; i < (eosd_multi_max_count - 1); ++i) {
		struct {
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
		} combo_info = {
			.si.cb = sizeof(STARTUPINFOA)
		};
		create_process_a(NULL, command_line, NULL, NULL, FALSE, 0, NULL, NULL, &combo_info.si, &combo_info.pi);
		close_handle(combo_info.pi.hProcess);
		close_handle(combo_info.pi.hThread);
	}
	th_free(command_line);
}

dllexport gnu_noinline void cdecl eosd_multi_setup_patch_init() asm_symbol_rel(codecave:eosd_multi_setup_patch_init);
dllexport gnu_noinline void cdecl eosd_multi_setup_patch_init() {
	HMODULE kernel_handle = th_GetModuleHandleA(kernel32_dll_str);

	// This is using an ASM statement since I *really* didn't want to have to copy
	// that expression everywhere, but the compiler kept seeing through my attempts
	// to read th_GetProcAddress to a variable.
	GetProcAddressPtr get_proc_address_shim;
	asm(
		"MOVL %1, %[get_proc_address_shim]"
		: asm_arg("=r", get_proc_address_shim)
		: "i"(&th_GetProcAddress)
	);

	UnmapViewOfFile_ptr = (UnmapViewOfFilePtr)get_proc_address_shim(kernel_handle, UnmapViewOfFile_str);
	// This handle doesn't need to be passed to CloseHandle when
	// allocating a file mapping from the page file. This isn't
	// mentioned in the documentation, but CloseHandle fails
	// when you try
	HANDLE shared_mapping;
	if (expect((shared_mapping = ((OpenFileMappingAPtr)get_proc_address_shim(kernel_handle, OpenFileMappingA_str))(FILE_MAP_ALL_ACCESS, FALSE, eosd_multi_mapping_name)) == NULL, false)) {
		shared_mapping = ((CreateFileMappingAPtr)get_proc_address_shim(kernel_handle, CreateFileMappingA_str))(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(EoSDMultiSharedData), eosd_multi_mapping_name);
		// File mappings backed by the paging file are well defined to be
		// zero initialized (https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createfilemappinga)
		// The code takes full advantage of this, primarily for giving
		// atomic counter things a known starting value.
	}
	auto* shared_data = (EoSDMultiSharedData*)((MapViewOfFilePtr)get_proc_address_shim(kernel_handle, MapViewOfFile_str))(shared_mapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(EoSDMultiSharedData));
	shared_data_ptr = shared_data;
	auto& next_index = shared_data->next_index;
	// Assign a difficulty ID to this process, in order with easy being 0
	// The 0 ID window is used as the main controller window whenever necessary
	auto mapping_index = instance_index = next_index.as_atomic.fetch_add(1);
	if (expect(!mapping_index, false)) {
		// If this is the first of the four windows to launch,
		// take care of some extra initialization steps...
		auto* create_directory_a_ptr = (CreateDirectoryAPtr)get_proc_address_shim(kernel_handle, CreateDirectoryA_str);
		DWORD folder_success = (DWORD)create_directory_a_ptr(eosd_multi_main_folder_str, NULL);
		if (expect(!folder_success, true)) {
			folder_success = th_GetLastError() == ERROR_ALREADY_EXISTS;
		}
		if (expect(!folder_success, false)) {
			eosd_multi_fail_exit(eosd_multi_main_folder_fail_format_str);
		}
		folder_success = (DWORD)create_directory_a_ptr(eosd_multi_replay_folder_str, NULL);
		if (expect(!folder_success, true)) {
			folder_success = th_GetLastError() == ERROR_ALREADY_EXISTS;
		}
		if (expect(!folder_success, false)) {
			eosd_multi_fail_exit(eosd_multi_replay_folder_fail_format_str);
		}
		// Spawn the other 3 processes
		spawn_children_processes(kernel_handle, get_proc_address_shim);
	}
	auto* sleep_func = (SleepPtr)get_proc_address_shim(kernel_handle, Sleep_str);
	Sleep_ptr = sleep_func;
	// Wait until all of the processes have launched so that loading is synchronized
	do {
		sleep_func(0);
	} while (expect(next_index.as_normal < eosd_multi_max_count, true));
	// Load any additional DLL functions
	LoadLibraryAPtr load_library_a_ptr = (LoadLibraryAPtr)get_proc_address_shim(kernel_handle, LoadLibraryA_str);
	nounroll for (size_t i = 0; i < countof(eosd_multi_xinput_lib_names); ++i) {
		if (HMODULE xinput_lib = load_library_a_ptr(eosd_multi_xinput_lib_names[i])) {
			XInputGetState_ptr = (XInputGetStatePtr)get_proc_address_shim(xinput_lib, XInputGetState_str);
			break;
		}
	}
	HMODULE user32_handle = th_GetModuleHandleA(user32_dll_str);
	GetKeyState_ptr = (GetKeyStatePtr)get_proc_address_shim(user32_handle, GetKeyState_str);
	GetWindowRect_ptr = (GetWindowRectPtr)get_proc_address_shim(user32_handle, GetWindowRect_str);
	SetWindowPos_ptr = (SetWindowPosPtr)get_proc_address_shim(user32_handle, SetWindowPos_str);
}

dllexport gnu_noinline void cdecl eosd_multi_shutdown_patch_exit() asm_symbol_rel(codecave:eosd_multi_shutdown_patch_exit);
dllexport gnu_noinline void cdecl eosd_multi_shutdown_patch_exit() {
	auto* shared_data = shared_data_ptr;
	auto& atomic_index = shared_data->next_index.as_atomic;
	auto index_value = atomic_index.fetch_sub(1);
	// Wait until all the processes have exited
	// This might not be necessary, but it certainly feels safer
	do {
		Sleep_ptr(1);
	} while (expect(atomic_index, true));
	UnmapViewOfFile_ptr((void*)shared_data);
}

dllexport uint32_t joycap_axes[4] asm_symbol_abs(0x69D784);

// Apparently xbox controllers use different ranges
// for xinput compared to the DirectInput/joystick APIs.
// This 
dllexport gnu_noinline uint16_t eosd_multi_fix_joycaps() asm_symbol_rel(codecave:eosd_multi_fix_joycaps);
dllexport gnu_noinline uint16_t eosd_multi_fix_joycaps() {
	joycap_axes[0] = 0; // X min
	joycap_axes[1] = UINT16_MAX; // X max
	joycap_axes[2] = 0; // Y min
	joycap_axes[3] = UINT16_MAX; // Y max
	return 0;
}

union XInputButtons {
	uint16_t buttons;
	struct {
		uint16_t dpad : 4;
		uint16_t start : 1;
		uint16_t back : 1;
		uint16_t thumbs : 2;
		uint16_t shoulders : 2;
		uint16_t : 2;
		uint16_t face_buttons : 4;
	};
};
union JoyInfoButtons {
	uint32_t buttons;
	struct {
		uint32_t face_buttons : 4;
		uint32_t shoulders : 2;
		uint32_t back : 1;
		uint32_t start : 1;
		uint32_t thumbs : 2;
		uint32_t dpad : 4;
	};
};

dllexport gnu_noinline uint32_t stdcall eosd_multi_process_xinput(DWORD index, JOYINFOEX* joy_info) asm_symbol_rel(codecave:eosd_multi_process_xinput);
dllexport gnu_noinline uint32_t stdcall eosd_multi_process_xinput(DWORD index, JOYINFOEX* joy_info) {
	XINPUT_STATE xinput_state;
	DWORD status;
	// xinput has a hard limit of 4 controllers, so search all of them
	for (size_t i = 0; i < 4; ++i) {
		status = XInputGetState_ptr(i, &xinput_state);
		if (expect(status == ERROR_SUCCESS, true)) {
			XInputButtons& x_buttons = (XInputButtons&)xinput_state.Gamepad.wButtons;
			JoyInfoButtons joy_buttons;
			joy_buttons.buttons = 0;
#define copy_button(name) joy_buttons.name = x_buttons.name
			copy_button(face_buttons);
			copy_button(shoulders);
			copy_button(back);
			copy_button(start);
			copy_button(thumbs);
			copy_button(dpad);
#undef copy_button
			joy_info->dwButtons = joy_buttons.buttons;
			//joy_info->dwXpos = (int32_t)xinput_state.Gamepad.sThumbLX + 32768;
			//joy_info->dwYpos = ~(int32_t)xinput_state.Gamepad.sThumbLY + 32768;

			// This version auto-vectorizes
			joy_info->dwXpos = (uint16_t)xinput_state.Gamepad.sThumbLX ^ 0x8000;
			joy_info->dwYpos = (uint16_t)xinput_state.Gamepad.sThumbLY ^ 0x7FFF;
			return 0;
		}
	}
	return status;
}

// Don't care about any of the other fields, this is just to
// generate the right offset without pointer casting an int
struct Menu {
	dummy_fields(0x81A0);
	int32_t menu_index;
};

// This implements the skipping over window mode in the options menu
dllexport gnu_noinline Menu* fastcall eosd_multi_skip_fullscreen_option(Menu*, int32_t cursor_dir) asm_symbol_rel(codecave:eosd_multi_skip_fullscreen_option);
dllexport gnu_noinline Menu* fastcall eosd_multi_skip_fullscreen_option(Menu*, int32_t cursor_dir) {
	Menu* menu_ptr;
	// I would use an explicit register variable, but for some reason that
	// makes clang generate a push/pop sequence despite not modifying it.
	asm(
		"MOV -0xC(%%ebp), %[menu_ptr]"
		: asm_arg("=a", menu_ptr) // Does this *need* to specify EAX, or just a register?
	);
	if (menu_ptr->menu_index == 5) {
		menu_ptr->menu_index += cursor_dir;
	}
	return menu_ptr;
}

// This attempts to calculate where winodws should be spawned,
// but the math is probably wrong somehow. Unfortunately there
// doesn't seem to be an effective way of vectorizing this with
// only SSE2. :(
dllexport gnu_noinline void regcall eosd_multi_calculate_window_positions(EoSDMultiSharedData * shared_data) asm_symbol_rel(codecave:eosd_multi_calculate_window_positions);
dllexport gnu_noinline void regcall eosd_multi_calculate_window_positions(EoSDMultiSharedData * shared_data) {
	auto& x_positions = shared_data->x_positions;
	auto& y_positions = shared_data->y_positions;
	int32_t x0 = x_positions.as_normal[0];
	int32_t smallest_x = x0;
	int32_t x1 = x_positions.as_normal[1];
	if (x1 < smallest_x) smallest_x = x1;
	int32_t x2 = x_positions.as_normal[2];
	if (!x2) x2 = x1;
	if (x2 < smallest_x) smallest_x = x2;
	int32_t x3 = x_positions.as_normal[3];
	if (!x3) x3 = x2;
	if (x3 < smallest_x) smallest_x = x3;
	x_positions.as_normal[0] = smallest_x;
	++smallest_x;
	uint32_t smallest_x_diff = x0 - smallest_x;
	if ((uint32_t)(x1 -= smallest_x) < smallest_x_diff) smallest_x_diff = x1;
	if ((uint32_t)(x2 -= smallest_x) < smallest_x_diff) smallest_x_diff = x2;
	if ((uint32_t)(x3 -= smallest_x) < smallest_x_diff) smallest_x_diff = x3;
	x_positions.as_normal[1] = smallest_x += smallest_x_diff;
	x_positions.as_normal[2] = smallest_x += smallest_x_diff;
	x_positions.as_normal[3] = smallest_x += smallest_x_diff;

	int32_t y0 = y_positions.as_normal[0];
	int32_t smallest_y = y0;
	int32_t y1 = y_positions.as_normal[1];
	if (y1 < smallest_y) smallest_y = y1;
	int32_t y2 = y_positions.as_normal[2];
	if (!y2) y2 = y1;
	if (y2 < smallest_y) smallest_y = y2;
	int32_t y3 = y_positions.as_normal[3];
	if (!y3) y3 = y2;
	if (y3 < smallest_y) smallest_y = y3;
	y_positions.as_normal[0] = smallest_y;
	++smallest_y;
	uint32_t smallest_y_diff = y0 - smallest_y;
	if ((uint32_t)(y1 -= smallest_y) < smallest_y_diff) smallest_y_diff = y1;
	if ((uint32_t)(y2 -= smallest_y) < smallest_y_diff) smallest_y_diff = y2;
	if ((uint32_t)(y3 -= smallest_y) < smallest_y_diff) smallest_y_diff = y3;
	y_positions.as_normal[1] = smallest_y += smallest_y_diff;
	y_positions.as_normal[2] = smallest_y += smallest_y_diff;
	y_positions.as_normal[3] = smallest_y += smallest_y_diff;
}

// This attempts to calculate where winodws should be spawned,
// but DOES IT PROPERLY THIS TIME.
// Apparently these didn't *really* need to be atomic writes
// and the bug was just user error, but it feels less blatnatly
// unsafe so I'll leave it
dllexport uint8_t max_count asm_symbol_abs(option:eosd_multi_max_count);
forceinline void fastcall eosd_multi_calculate_window_positions_inner(EoSDMultiSharedData* shared_data) {
	auto& x_positions = shared_data->x_positions;
	auto& y_positions = shared_data->y_positions;

	// Q: Isn't it bad practice to reuse variables for unrelated things?
	// A: Yes
	//
	// Q: Then why are you doing it?
	// A: Because for somee reason clang confuses itself if I don't
	//    and it's sad to look at otherwise. :P
	int32_t v0, v1, v2, v3;
	uint32_t dv, dv2;

	v0 = x_positions.as_normal[0];
	v1 = x_positions.as_normal[1];
	if (v0 > v1) std::swap(v0, v1);
	v2 = v1;
	v3 = v1;
	if (max_count > 2) {
		v2 = x_positions.as_normal[2];
		if (v1 > v2) std::swap(v1, v2);
		if (v0 > v1) std::swap(v0, v1);
		v3 = v2;
		if (max_count > 3) {
			v3 = x_positions.as_normal[3];
			if (v2 > v3) std::swap(v2, v3);
			if (v1 > v2) std::swap(v1, v2);
			if (v0 > v1) std::swap(v0, v1);
		}
	}
	x_positions.as_atomic[0] = v0;
	dv = v1 - v0 ? : v3;
	if ((dv2 = v2 - v1)) {
		if (dv2 < dv) dv = dv2;
	}
	if (v3 -= v2) {
		if (v3 < dv) dv = v3;
	}
	v0 += dv;
	x_positions.as_atomic[1] = v0;
	v0 += dv;
	x_positions.as_atomic[2] = v0;
	v0 += dv;
	x_positions.as_atomic[3] = v0;

	v0 = y_positions.as_normal[0];
	v1 = y_positions.as_normal[1];
	if (v0 > v1) std::swap(v0, v1);
	v2 = v1;
	v3 = v1;
	if (max_count > 2) {
		v2 = y_positions.as_normal[2];
		if (v1 > v2) std::swap(v1, v2);
		if (v0 > v1) std::swap(v0, v1);
		v3 = v2;
		if (max_count > 3) {
			v3 = y_positions.as_normal[3];
			if (v2 > v3) std::swap(v2, v3);
			if (v1 > v2) std::swap(v1, v2);
			if (v0 > v1) std::swap(v0, v1);
		}
	}
	y_positions.as_atomic[0] = v0;
	dv = v1 - v0 ? : v3;
	if ((dv2 = v2 - v1)) {
		if (dv2 < dv) dv = dv2;
	}
	if (v3 -= v2) {
		if (v3 < dv) dv = v3;
	}
	v0 += dv;
	y_positions.as_atomic[1] = v0;
	v0 += dv;
	y_positions.as_atomic[2] = v0;
	v0 += dv;
	y_positions.as_atomic[3] = v0;
}

// This calling convention is the one I used in the
// generated code
dllexport gnu_noinline void regcall eosd_multi_calculate_window_positions2(int, int, int, EoSDMultiSharedData* shared_data) asm_symbol_rel(codecave:eosd_multi_calculate_window_positions2);
dllexport gnu_noinline void regcall eosd_multi_calculate_window_positions2(int, int, int, EoSDMultiSharedData* shared_data) {
	eosd_multi_calculate_window_positions_inner(shared_data);
}
// And this makes the calling function not kill itself
dllexport gnu_noinline void fastcall eosd_multi_calculate_window_positions3(EoSDMultiSharedData* shared_data) asm_symbol_rel(codecave:eosd_multi_calculate_window_positions3);
dllexport gnu_noinline void fastcall eosd_multi_calculate_window_positions3(EoSDMultiSharedData* shared_data) {
	eosd_multi_calculate_window_positions_inner(shared_data);
}

// This entire function is a bit of a hack to try and prevent
// the windows spawning on top of each other. It doesn't
// *quite* work right, but seems to succeed more often than
// just letting windows do things automatically.
// For some reason which window ends up where is still random
// though, even though I tried to put them in order...
dllexport gnu_noinline HWND regparm(1) eosd_multi_adjust_window_positions(HWND window_handle) asm_symbol_rel(codecave:eosd_multi_adjust_window_positions);
dllexport gnu_noinline HWND regparm(1) eosd_multi_adjust_window_positions(HWND window_handle) {
	RECT position_data;
	GetWindowRect_ptr(window_handle, &position_data);
	uint8_t self_index = instance_index;
	EoSDMultiSharedData* shared_data = shared_data_ptr;
	auto& window_hwnds = shared_data->window_hwnds;
	auto& x_positions = shared_data->x_positions;
	auto& y_positions = shared_data->y_positions;
	window_hwnds.as_atomic[self_index + 1] = window_handle; // Offset by 1 so that index 0 contains NULL HWND
	x_positions.as_atomic[self_index] = position_data.left;
	y_positions.as_atomic[self_index] = position_data.top;
	auto& read_size_index = shared_data->read_size_index;
	if (expect(read_size_index.as_atomic.fetch_add(1) < (eosd_multi_max_count - 1), true)) {
		// Wait until all of the windows have written their positions to the shared array
		do {
			Sleep_ptr(0);
		} while (expect(read_size_index.as_normal < eosd_multi_max_count, true));
	}
	if (expect_chance(self_index == 0, true, 0.25)) {
		//eosd_multi_calculate_window_positions(shared_data);
		eosd_multi_calculate_window_positions3(shared_data);
		// Window ID 0 doesn't need to enter the wait loop since
		// it'll be the bottom window anyway and doesn't need to
		// wait for others to reposition themselves
	}
	else {
		// Make each patch wait for their turn so that the Z order works
		// (except this is apparently doesn't make it work, but whatever)
		do {
			Sleep_ptr(0);
		} while (expect(shared_data->resize_index.as_normal != self_index, true));
	}
	SetWindowPos_ptr(
		window_handle,
		window_hwnds.as_normal[self_index],
		x_positions.as_normal[self_index],
		y_positions.as_normal[self_index],
		0,
		0,
		SWP_NOSIZE | ((self_index == 0) * SWP_NOZORDER)
	);
	// Release the next process from the previous sleep loop
	auto& resize_index = shared_data->resize_index;
	++resize_index.as_atomic;
	do {
		Sleep_ptr(0);
	} while (expect(resize_index.as_normal < eosd_multi_max_count, true));
	--read_size_index.as_atomic;
	--resize_index.as_atomic;
	return window_handle; // Need to return the value so that the game can write it into memory or whatever. Inserting this call clobbers the original field read.
}