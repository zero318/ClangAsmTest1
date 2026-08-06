#ifndef STATUS_NOT_IMPLEMENTED
#define STATUS_NOT_IMPLEMENTED				((DWORD)0xC0000002L)
#endif
#ifndef STATUS_INVALID_LOCK_SEQUENCE
#define STATUS_INVALID_LOCK_SEQUENCE		((DWORD)0xC000001EL)
#endif
#ifndef STATUS_BAD_STACK
#define STATUS_BAD_STACK					((DWORD)0xC0000028L)
#endif
#ifndef STATUS_INVALID_UNWIND_TARGET
#define STATUS_INVALID_UNWIND_TARGET		((DWORD)0xC0000029L)
#endif
#ifndef STATUS_BAD_FUNCTION_TABLE
#define STATUS_BAD_FUNCTION_TABLE			((DWORD)0xC00000FFL)
#endif
#ifndef STATUS_DATATYPE_MISALIGNMENT_ERROR
#define STATUS_DATATYPE_MISALIGNMENT_ERROR	((DWORD)0xC00002C5L)
#endif
#ifndef STATUS_HEAP_CORRUPTION
#define STATUS_HEAP_CORRUPTION				((DWORD)0xC0000374L)
#endif

#define EH_EXCEPTION_NUMBER 0xE06D7363

static constexpr const char* EMOTE_CAPTIONS[] = {
	":ReisenWorried:",
	":NotLikeAlice:",
	":tannedcirno:",
	":KeikiPout:",
	":ReisenDying:",
	":ClownpieceSweat:",
	":ReimuWat:",
	":KogasaGun:"
};

inline int Zmbox(const char* message, const char* caption, UINT type) {
	if (!caption) {
		unsigned int random;
		rand_s(&random);
		caption = EMOTE_CAPTIONS[random % countof(EMOTE_CAPTIONS)];
	}
	return MessageBoxA(NULL, message, caption, type);
}
inline int Zmboxf(const char* format, const char* caption, UINT type, ...) {
	va_list va, va2;
	va_start(va, type);
	va_copy(va2, va);
	int length = vsnprintf(NULL, 0, format, va);
	va_end(va);
	char buffer[length + 1];
	vsprintf(buffer, format, va2);
	va_end(va2);
	return Zmbox(buffer, caption, type);
}

gnu_noinline void make_sure_file_exists(const char* path) {
	while (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) {
		switch (Zmboxf("Count not find %s", NULL, MB_TOPMOST | MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_DEFBUTTON2, path)) {
			case IDRETRY:
				break;
			case IDIGNORE: // if you insist
				return;
			case IDABORT:
				ExitProcess(-1);
		}
	}
}

LONG WINAPI exception_filter(LPEXCEPTION_POINTERS lpEI) {
	LPEXCEPTION_RECORD lpER = lpEI->ExceptionRecord;

	switch (lpER->ExceptionCode) {
		case EXCEPTION_BREAKPOINT:
			if (!IsDebuggerPresent()) {
		case STATUS_ACCESS_VIOLATION:
		case STATUS_ILLEGAL_INSTRUCTION:
		case STATUS_INTEGER_DIVIDE_BY_ZERO:
		case EH_EXCEPTION_NUMBER:

		case STATUS_NOT_IMPLEMENTED:
		case STATUS_INVALID_LOCK_SEQUENCE:
		case STATUS_ARRAY_BOUNDS_EXCEEDED:
		case STATUS_PRIVILEGED_INSTRUCTION:
		case STATUS_DATATYPE_MISALIGNMENT_ERROR:
		case STATUS_ASSERTION_FAILURE:

		case STATUS_FLOAT_INVALID_OPERATION:
		case STATUS_FLOAT_OVERFLOW:
		case STATUS_FLOAT_STACK_CHECK:
		case STATUS_FLOAT_UNDERFLOW:
		case STATUS_FLOAT_MULTIPLE_FAULTS:
		case STATUS_FLOAT_MULTIPLE_TRAPS:

		case STATUS_STACK_OVERFLOW:
		case STATUS_STACK_BUFFER_OVERRUN:
		case STATUS_BAD_STACK:
		case STATUS_INVALID_UNWIND_TARGET:
		case STATUS_BAD_FUNCTION_TABLE:
		case STATUS_HEAP_CORRUPTION:
			Zmboxf(
				"Your dodging is ass. Session terminated.\n"
				"\n"
				"Exception %08X at 0x%p",
				"Yes this is actually a crash handler",
				MB_OK | MB_ICONSTOP | MB_TASKMODAL,
				lpER->ExceptionCode, lpER->ExceptionAddress
			);
			}
			[[fallthrough]];
		default:
			return EXCEPTION_CONTINUE_SEARCH;
	}
}