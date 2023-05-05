extern "C" {

__declspec(dllexport) volatile char address_space_bloat[0x200000];

__declspec(dllexport) void __stdcall dummy_func() {

}

__declspec(dllexport) int __stdcall DllMain(void* hinstDLL, unsigned long fdwReason, void* lpReserved) {
    return 1;
}

}