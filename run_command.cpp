#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <Windows.h>

template<typename T>
T* find_end_of_arg(T* str) {
	switch (T p = *str) {
		case (T)'\0':
			return str;
		case (T)'"':
			for (;;) {
				switch (T c = *++str) {
					case (T)'"':
						if (p != (T)'\\') {
					case (T)'\0':
							return str;
						}
					default:
						p = c;
				}
			}
		default:
			for (;;) {
				switch (T c = *++str) {
					case (T)' ': case (T)'\0':
						return str;
				}
			}
	}
}

template<typename T>
T* find_next_arg(T* str) {
	if (*str != (T)'\0') {
		bool has_space = false;
		for (;;) {
			switch (T c = *++str) {
				case (T)'\0':
					return NULL;
				default:
					if (has_space) {
						return str;
					}
				case (T)' ':
					has_space = true;
			}
		}
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	if (wchar_t* args_start = find_next_arg(find_end_of_arg(GetCommandLineW()))) {
		_wsystem(args_start);
	}
	return EXIT_SUCCESS;
}