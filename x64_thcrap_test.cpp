#include <stdint.h>
#include <stdlib.h>

#include "windows.h"

__declspec(dllexport, noinline) void say_hi() {
	MessageBoxA(NULL, "hi from x64", "thcrap_inject_test", 0);
}
__declspec(dllexport, noinline) void say_bye() {
	MessageBoxA(NULL, "injection_failed", "thcrap_inject_test", 0);
}

int main(int argc, char* argv[]) {
	say_bye();
	return 0;
}