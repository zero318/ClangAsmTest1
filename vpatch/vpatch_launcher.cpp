#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <shlwapi.h>

#include "common.h"

#include "..\zero\util.h"
#include "..\zero\custom_intrin.h"

void error(const char* str) {
    puts(str);
    MessageBoxA(NULL, str, "vpatch error", MB_OK | MB_ICONHAND | MB_DEFBUTTON1 | MB_APPLMODAL);
}

typedef InitializeReturnCode __stdcall initialize_func(const char*);

int main(int argc, char* argv[]) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, countof(path));
    PathRemoveFileSpecA(path);
    SetCurrentDirectoryA(path);
    WIN32_FIND_DATAA find_data;
    HANDLE vpatch_dll_file = FindFirstFileA("*vpatch*.dll", &find_data);
    if (vpatch_dll_file == INVALID_HANDLE_VALUE) {
        error(JpEnStr("vpatch dll�����݂��܂���", "vpatch dll does not exist"));
        return -1;
    }
    (void)strlen(find_data.cFileName);
    FindClose(vpatch_dll_file);
    HMODULE vpatch_dll_module = LoadLibraryA(find_data.cFileName);
    if (!vpatch_dll_module) {
        error(JpEnStr("vpatch dll�����[�h�ł��܂���ł���", "Failed to load vpatch dll"));
        return -2;
    }
    initialize_func* init_func = (initialize_func*)GetProcAddress(vpatch_dll_module, "_Initialize@4");
    if (!init_func) {
        error(JpEnStr("vpatch dll���̊֐���������܂���ł���", "Function in vpatch dll was not found"));
        return -3;
    }
    char path2[MAX_PATH];
    const char* config_path = NULL;
    if (argc == 2) {
        PathSearchAndQualifyA(argv[1], path2, countof(path2));
        config_path = path2;
    }
    switch (init_func(config_path)) {
        default:
            error(JpEnStr("���m�̃G���[���������܂����Q�[�����N���ł��܂���", "An unknown error has occurred. Unable to start"));
            return -4;
        case INIT_INJECT_FAILED:
            error(JpEnStr("�Q�[���̎��s�t�@�C���ւ�DLL Injection�Ɏ��s���܂���", "DLL injection to game executable file failed"));
            return -4;
        case INIT_PROCESS_FAILED:
            error(JpEnStr("�Q�[���̎��s�t�@�C����������܂���ł���", "Game executable file not found"));
            return -4;
        case INIT_SUCCESS:
            return 0;
    }
}