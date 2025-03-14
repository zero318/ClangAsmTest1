
__declspec(dllexport) char text[] = "You are winner";

int yeet() {
	return (int)text;
}


#if CUSTOM_CLANG
#define CS_RELATIVE __attribute__((address_space(259)))
#define DS_RELATIVE __attribute__((address_space(260)))
#define ES_RELATIVE __attribute__((address_space(261)))
#endif

#if CUSTOM_CLANG
__declspec(dllexport) int __fastcall read_es_ptr(int ES_RELATIVE* ptr) {
	return *ptr;
}

register unsigned short FS asm("fs");

__declspec(dllexport) void write_fs_zero() {
	FS = 0;
}

__declspec(dllexport) void write_fs_imm8() {
	FS = 69;
}

__declspec(dllexport) void write_es_imm16() {
	FS = 420;
}

__declspec(dllexport) void __fastcall write_fs_short(unsigned short value) {
	FS = value;
}

__declspec(dllexport) void __fastcall write_fs_int(unsigned int value) {
	FS = value;
}

__declspec(dllexport) void __fastcall write_fs_long(unsigned long long value) {
	FS = value;
}

__declspec(dllexport) void __fastcall write_fs_short_mem(unsigned short* ptr) {
	FS = *ptr;
}

__declspec(dllexport) void __fastcall write_fs_int_mem(unsigned int* ptr) {
	FS = *ptr;
}

__declspec(dllexport) void __fastcall write_fs_long_mem(unsigned long long* ptr) {
	FS = *ptr;
}

__declspec(dllexport) unsigned short read_fs_short() {
	return FS;
}

__declspec(dllexport) unsigned int read_fs_int() {
	return FS;
}

__declspec(dllexport) unsigned long long read_fs_long() {
	return FS;
}

__declspec(dllexport) void __fastcall read_fs_short_ptr(unsigned short* ptr) {
	*ptr = FS;
}

__declspec(dllexport) void __fastcall read_fs_int_ptr(unsigned int* ptr) {
	*ptr = FS;
}

__declspec(dllexport) void __fastcall read_fs_long_ptr(unsigned long long* ptr) {
	*ptr = FS;
}

__declspec(dllexport) volatile unsigned short short_vol = 0;
__declspec(dllexport) __declspec(noinline) void __stdcall short_stack_func(
#if __x86_64__
	int, int, int, int,
#endif
	unsigned short value
) {
	short_vol = value;
}

__declspec(dllexport) volatile unsigned int int_vol = 0;
__declspec(dllexport) __declspec(noinline) void __stdcall int_stack_func(
#if __x86_64__
	int, int, int, int,
#endif
	unsigned int value
) {
	int_vol = value;
}

__declspec(dllexport) volatile unsigned long long long_vol = 0;
__declspec(dllexport) __declspec(noinline) void __stdcall long_stack_func(
#if __x86_64__
	int, int, int, int,
#endif
	unsigned long long value
) {
	long_vol = value;
}

__declspec(dllexport) void push_fs_short() {
	short_stack_func(
#if __x86_64__
		0, 0, 0, 0,
#endif
		FS
	);
}

__declspec(dllexport) void push_fs_int() {
	int_stack_func(
#if __x86_64__
		0, 0, 0, 0,
#endif
		FS
	);
}

__declspec(dllexport) void push_fs_long() {
	long_stack_func(
#if __x86_64__
		0, 0, 0, 0,
#endif
		FS
	);
}

#endif
