#if __INTELLISENSE__
#undef _HAS_CXX20
#define _HAS_CXX20 0
#endif

#include <stdint.h>
#include <stdlib.h>
//#include <stdio.h>
#include <utility>
#include <string.h>

#include <type_traits>
#include <codecvt>
#include <string>

// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

template <typename T>
static inline size_t uint32_to_hex_strbuf(uint32_t value, T* text_buffer) {
    uint32_t temp = value;
    size_t digit_offset = temp ? (31 - __builtin_ia32_lzcnt_u32(value)) >> 2 : 0;
    size_t ret = digit_offset + 1;
    do {
        uint16_t digit = temp & 0xF;
        temp >>= 4;
        text_buffer[digit_offset] = (digit < 10 ? (T)'0' : (T)('A' - 10)) + digit;
    } while (digit_offset--);
    return ret;
}

#undef WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Psapi.h>

#include "jansson.h"
#include "json5pp.hpp"

#include <intrin.h>
//#include <DbgHelp.h>

template <typename T>
[[nodiscard]] auto read_file_to_buffer(const T* path) {
    long file_size = 0;
    uint8_t* buffer = NULL;
    FILE* file;
    if constexpr (std::is_same_v<T, char>) {
        file = fopen(path, "rb");
    } else if constexpr (std::is_same_v<T, wchar_t>) {
        file = _wfopen(path, L"rb");
    }
    if (file) {
        _fseek_nolock(file, 0, SEEK_END);
        file_size = _ftell_nolock(file);
        buffer = (uint8_t*)_mm_malloc(file_size, 16);
        if (buffer) {
            _fseek_nolock(file, 0, SEEK_SET);
            _fread_nolock(buffer, file_size, 1, file);
        }
        _fclose_nolock(file);
    }
    return std::make_pair(buffer, (size_t)file_size);
}

extern uint32_t __fastcall crc32_impl(const uint8_t* buf, size_t len);

static json_t* label_json = NULL;

#pragma comment (lib, "_dbghelp.lib")

BOOL __stdcall SymInitializeW(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess);
BOOL __stdcall SymCleanup(HANDLE hProcess);
DWORD64 __stdcall SymLoadModuleExW(
    HANDLE hProcess,
    HANDLE hFile,
    PCWSTR ImageName,
    PCWSTR ModuleName,
    DWORD64 BaseOfDll,
    DWORD DllSize,
    void* Data,
    DWORD Flags
);
BOOL __stdcall SymAddSymbolW(
    HANDLE  hProcess,
    ULONG64 BaseOfDll,
    PCWSTR  Name,
    DWORD64 Address,
    DWORD   Size,
    DWORD   Flags
);

using SymInitializeW_t = decltype(SymInitializeW);
using SymCleanup_t = decltype(SymCleanup);
using SymLoadModuleExW_t = decltype(SymLoadModuleExW);
using SymAddSymbolW_t = decltype(SymAddSymbolW);

SymCleanup_t* cleanup;
SymLoadModuleExW_t* load_module;
SymAddSymbolW_t* add_symbol;

extern "C" {

BOOL __stdcall SymInitializeW_shim(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess) {
    HMODULE real_dbg_help = GetModuleHandleA("_dbghelp.dll");

    cleanup = (SymCleanup_t*)GetProcAddress(real_dbg_help, "SymCleanup");
    load_module = (SymLoadModuleExW_t*)GetProcAddress(real_dbg_help, "SymLoadModuleExW");
    add_symbol = (SymAddSymbolW_t*)GetProcAddress(real_dbg_help, "SymAddSymbolW");


    label_json = json_load_file("_label_lookup.json", 0, NULL);

    auto init = (SymInitializeW_t*)GetProcAddress(real_dbg_help, "SymInitializeW");
    [[clang::must_tail]] return init(hProcess, UserSearchPath, fInvadeProcess);
}

BOOL __stdcall SymCleanup_shim(HANDLE hProcess) {
    json_decref(label_json);
    //return SymCleanup(hProcess);
    return cleanup(hProcess);
}

#define json_object_foreach_fast(object, key, value) \
    for (void* iter = json_object_iter(object); \
		 iter ? (value = json_object_iter_value(iter)), (key = json_object_iter_key(iter)), 1 : 0; \
		 iter = json_object_iter_next(object, iter))

#define json_array_foreach_scoped(ind_type, ind, arr, val) \
	for(ind_type ind = 0, ind ## _max = json_array_size(arr); \
		ind < ind ## _max ? (val = json_array_get(arr, ind)), 1 : 0; \
		ind++)


// hProcess, 0, img, mod, baseAddr, size, nullptr, 0
DWORD64 __stdcall SymLoadModuleExW_shim(
    HANDLE hProcess,
    HANDLE hFile,
    PCWSTR ImageName,
    PCWSTR ModuleName,
    DWORD64 BaseOfDll,
    DWORD DllSize,
    void* Data,
    DWORD Flags
) {
    //DWORD64 ret = SymLoadModuleExW(hProcess, hFile, ImageName, ModuleName, BaseOfDll, DllSize, Data, Flags);
    DWORD64 ret = load_module(hProcess, hFile, ImageName, ModuleName, BaseOfDll, DllSize, Data, Flags);
    if (ret) {
        /*
        if (!__builtin_wcscmp(ModuleName, L"th19.exe")) {
            add_symbol(hProcess, BaseOfDll, L"_sin", BaseOfDll + 0x7A070, 0x43, 0);
            add_symbol(hProcess, BaseOfDll, L"__sin_pentium4", BaseOfDll + 0x80C48, 0x1AF, 0);
            add_symbol(hProcess, BaseOfDll, L"_cos", BaseOfDll + 0x7A0C0, 0x43, 0);
            add_symbol(hProcess, BaseOfDll, L"__cos_pentium4", BaseOfDll + 0x80F18, 0x191, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmManager::render_layer(int layer_index)", BaseOfDll + 0xCC5A0, 0x267, 0);
            add_symbol(hProcess, BaseOfDll, L"CPUHitInf::__check_colliders", BaseOfDll + 0xF8F60, 0x5CF, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmListEnds::on_tick_world", BaseOfDll + 0xCCE00, 0x98, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmVM::run_anm", BaseOfDll + 0xBD660, 0x38B7, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmVM::get_root_vm_custom_slowdown", BaseOfDll + 0x49D0, 0x31, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmVM::__get_root_vm_controller_rotation", BaseOfDll + 0x4AE0, 0x90, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmManager::draw_vm(AnmVM* vm)", BaseOfDll + 0xC50F0, 0xDCA, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmVM::__step_interps", BaseOfDll + 0xBC9A0, 0x218, 0);
            add_symbol(hProcess, BaseOfDll, L"__sub_r126990", BaseOfDll + 0x126990, 0x710, 0);
            add_symbol(hProcess, BaseOfDll, L"AnmManager::__sub_rC9380", BaseOfDll + 0xC9380, 0xA12, 0);
        }
        */
        //__asm__ volatile (".byte 0xEB, 0xFE");
        
        auto utf8_str = wstring_to_utf8(ModuleName);
        json_t* module_versions = json_object_get(label_json, utf8_str.data());
        if (__builtin_expect(module_versions != NULL, false)) {
            auto [buffer, size] = read_file_to_buffer(ImageName);
            if (buffer) {
                uint32_t crc32 = crc32_impl(buffer, size);
                _mm_free(buffer);

                char crc32_buf[8];

                size_t length = uint32_to_hex_strbuf(crc32, crc32_buf);

                json_t* filename = json_object_getn(module_versions, crc32_buf, length);
                if (filename) {
                    std::string u8str = "extra_labels/";
                    u8str.append(json_string_value(filename), json_string_length(filename));

                    auto [label_buffer, label_size] = read_file_to_buffer(u8str.data());
                    if (label_buffer) {
                        try {
                            json5pp::value json5 = json5pp::parse5(label_buffer, label_size);
                            for (const auto& [key, value] : json5.as_object()) {
                                std::wstring name = utf8_to_wstring(key);
                                const auto& values = value.as_array();
                                add_symbol(
                                    hProcess,
                                    BaseOfDll,
                                    name.data(),
                                    BaseOfDll + values[0].as_integer(),
                                    values[1].as_integer(),
                                    0
                                );
                            }
                        }
                        catch (json5pp::syntax_error e) {
                        }
                        _mm_free(label_buffer);
                    }
                    
                    //json5pp::parse5()

                    /*
                    json_error_t err;
                    json_t* exe_labels = json_load_file(u8str.data(), 0, &err);
                    if (__builtin_expect(exe_labels != NULL, true)) {
                        json_t* value;
                        json_object_foreach_fast(exe_labels, u8str, value) {
                            
                        }
                    }
                    */

                    //SymAddSymbolW
                    /*
                    add_symbol(hProcess, BaseOfDll, L"_sin", BaseOfDll + 0x7A070, 0x43, 0);
                    add_symbol(hProcess, BaseOfDll, L"__sin_pentium4", BaseOfDll + 0x80C48, 0x1AF, 0);
                    add_symbol(hProcess, BaseOfDll, L"_cos", BaseOfDll + 0x7A0C0, 0x43, 0);
                    add_symbol(hProcess, BaseOfDll, L"__cos_pentium4", BaseOfDll + 0x80F18, 0x191, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmManager::render_layer(int layer_index)", BaseOfDll + 0xCC5A0, 0x267, 0);
                    add_symbol(hProcess, BaseOfDll, L"CPUHitInf::__check_colliders", BaseOfDll + 0xF8F60, 0x5CF, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmListEnds::on_tick_world", BaseOfDll + 0xCCE00, 0x98, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmVM::run_anm", BaseOfDll + 0xBD660, 0x38B7, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmVM::get_root_vm_custom_slowdown", BaseOfDll + 0x49D0, 0x31, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmVM::__get_root_vm_controller_rotation", BaseOfDll + 0x4AE0, 0x90, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmManager::draw_vm(AnmVM* vm)", BaseOfDll + 0xC50F0, 0xDCA, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmVM::__step_interps", BaseOfDll + 0xBC9A0, 0x218, 0);
                    add_symbol(hProcess, BaseOfDll, L"__sub_r126990", BaseOfDll + 0x126990, 0x710, 0);
                    add_symbol(hProcess, BaseOfDll, L"AnmManager::__sub_rC9380", BaseOfDll + 0xC9380, 0xA12, 0);
                    */
                }
            }
        }
        
    }
    return ret;
}

}