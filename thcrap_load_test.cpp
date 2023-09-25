#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define bool_str(cond) ((cond) ? "true" : "false")

//#define UseWinMain

static const char runconfig_filename[] = "F:/Touhou_Stuff_2/_thtk-bin-new/__thcrap/config/en.js";

static const char thcrap_runconfig_format[] = "{\"binhacks\":{\"fix_trophy\":{\"addr\":\"Rx106B88\",\"code\":\"[Rx207910]\",\"enable\":%s}}}";

#define screensaver_commandline "\"F:/Touhou_Stuff_2/disassembly_stuff/19/th19/th19.exe\""

static const char screensaver_name[] = "F:\\Touhou_Stuff_2\\disassembly_stuff\\19\\th19\\th19.exe";
static char command_line[32767] = screensaver_commandline;

static STARTUPINFO si = { sizeof(STARTUPINFO) };
static PROCESS_INFORMATION pi;

#ifndef UseWinMain
int main(int argc, char *argv[]) {
	/*
	if (argc != 1) {
		char* command_line_write = &command_line[sizeof(screensaver_commandline) - 1];
		for (int i = 1; i != argc; ++i) {
			*command_line_write = ' ';
			const char* argv_read = argv[i];
			while (*++command_line_write = *argv_read++);
		}
	}
	*/
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	if (*lpCmdLine) {
		char* command_line_write = &command_line[sizeof(screensaver_commandline) - 1];
		*command_line_write++ = ' ';
		while (*command_line_write++ = *lpCmdLine++);
	}
#endif

	SetCurrentDirectoryA("F:/Touhou_Stuff_2/_thtk-bin-new/__thcrap/bin/");
	HMODULE thcrap_handle = LoadLibraryA("thcrap.dll");

#ifndef UseWinMain
	int ret = EXIT_SUCCESS;
#endif
	if (CreateProcessA(
		screensaver_name,
		command_line,
		NULL,
		NULL,
		TRUE,
		CREATE_SUSPENDED,
		NULL,
		"F:/Touhou_Stuff_2/disassembly_stuff/19/th19",
		&si,
		&pi
	)) {
		sprintf(command_line, thcrap_runconfig_format, bool_str(argc > 1));
		if (((int(__cdecl*)(HANDLE hProcess, const char *run_cfg_fn))GetProcAddress(thcrap_handle, "thcrap_inject_into_running"))(
			pi.hProcess,
			command_line
		) != -1) {
			ResumeThread(pi.hThread);
		} else {
			MessageBox(NULL, "Error injecting thcrap!", "Error!", 0);
#ifndef UseWinMain
			TerminateProcess(pi.hProcess, ret = EXIT_FAILURE);
#else
			TerminateProcess(pi.hProcess, EXIT_FAILURE);
#endif

		}
	}
#ifndef UseWinMain
	return ret;
#else
	return 0;
#endif
}