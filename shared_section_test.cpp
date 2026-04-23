#include <windows.h>

#define NEG_ONE_SIZET (~(SIZE_T)0)

#pragma comment(linker, "/SECTION:.data,RWS")

static _Atomic SIZE_T shared_value = NEG_ONE_SIZET;

static char message_buffer[] = "New run detected! Total count:                    0\n";

#define STARTUP_MSG "Waiting for program to be run again. Total count: 0\n"

int entrypoint() {
	if (!++shared_value) {
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

		WriteConsoleA(out, STARTUP_MSG, sizeof(STARTUP_MSG) - 1, NULL, NULL);

		SIZE_T prev_value = 0;

		for (;;) {
			SIZE_T new_value;
			while ((new_value = shared_value) == prev_value) Sleep(1);
			prev_value = new_value;

			SIZE_T i = 50;
			do {
				message_buffer[i--] = new_value % 10 + '0';
				new_value /= 10;
			} while (new_value);

			WriteConsoleA(out, message_buffer, sizeof(message_buffer) - 1, NULL, NULL);
		}
	}
	return 0;
}