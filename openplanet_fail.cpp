NTSYSAPI NTSTATUS NtProtectVirtualMemory(HANDLE ProcessHandle, PVOID* BaseAddress, PSIZE_T NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection);
NTSYSAPI NTSTATUS NtWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, SIZE_T NumberOfBytesToWrite, PSIZE_T NumberOfBytesWritten);
NTSYSAPI NTSTATUS NtOpenSection(PHANDLE SectionHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes);
NTSYSAPI NTSTATUS NtMapViewOfSection(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType, ULONG Win32Protect);

using NtProtectVirtualMemory_t = decltype(NtProtectVirtualMemory);
using NtWriteVirtualMemory_t = decltype(NtWriteVirtualMemory);
using NtOpenSection_t = decltype(NtOpenSection);
using NtMapViewOfSection_t = decltype(NtMapViewOfSection);

NtProtectVirtualMemory_t* NtProtectVirtualMemory_ptr;
NtWriteVirtualMemory_t* NtWriteVirtualMemory_ptr;

void logging(int arg1, const char* format, ...);

bool file_exists(const char* path);

size_t patch_memory(
    void* dst,
    void* src,
    size_t count
) {
    NTSTATUS status;
    SIZE_T ret = 0;
    if (!NtProtectVirtualMemory_ptr) {
        // This gets a handle to kxnt.dll, which
        // for the purposes of this function just
        // has a bunch of forwarding exports to the
        // real ntdll.
        HMODULE ntdll = LoadLibraryA("ntdll.dll");
        NtOpenSection_t* NtOpenSection_ptr = (NtOpenSection_t*)GetProcAddress(ntdll, "NtOpenSection");
        NtMapViewOfSection_t* NtMapViewOfSection_ptr = (NtMapViewOfSection_t*)GetProcAddress(ntdll, "NtMapViewOfSection");
        NtWriteVirtualMemory_ptr = (NtWriteVirtualMemory_t*)GetProcAddress(ntdll, "NtWriteVirtualMemory");
        UNICODE_STRING str;
        RtlInitUnicodeString(&str, L"\\KnownDlls\\ntdll.dll");
        OBJECT_ATTRIBUTES attr;
        attr.Length = sizeof(OBJECT_ATTRIBUTES);
        attr.ObjectName = &str;
        attr.RootDirectory = NULL;
        attr.SecurityDescriptor = NULL;
        attr.SecurityQualityOfService = NULL;
        HANDLE section;
        status = NtOpenSection_ptr(&section, 0xC, &attr);
        if (SUCCEEDED(status)) {
            PVOID view_base = NULL;
            SIZE_T view_size = 0;
            NtMapViewOfSection_ptr(section, GetCurrentProcess(), &view_base, 0, 0, NULL, &view_size, ViewShare, 0, PAGE_EXECUTE_READ);
            uint8_t* temp = (uint8_t*)GetProcAddress(ntdll, "NtProtectVirtualMemory");
            // This ends up calculating a completely wrong address because of the previous
            // handle from LoadLibraryA not matching the module that GetProcAddress found
            // NtProtectVirtualMemory in.
            NtProtectVirtualMemory_ptr = (NtProtectVirtualMemory_t*)((uintptr_t)view_base + ((uintptr_t)temp - (uintptr_t)ntdll));
        }
        else {
            logging_func(
                3,
                status == STATUS_OBJECT_PATH_NOT_FOUND
                    ? "Unable to open ntdll section (error %08X, this can happen when running under Wine on Linux)"
                    : "Unable to open ntdll section (error %08X)"
            );
            if (file_exists("ntdll_o.dll")) {
                DeleteFileA("ntdll_o.dll");
            }
            CopyFileA("C:\\windows\\system32\\ntdll.dll", "ntdll_o.dll", TRUE);
            HMODULE ntdll_o = LoadLibraryA("ntdll_o.dll");
            if (!ntdll_o || ntdll_o == ntdll) {
                logging(3, "Unable to open ntdll_o library (please report!)");
                if (ntdll_o) {
                    FreeLibrary(ntdll_o);
                }
                DeleteFileA("ntdll_o.dll");
                return 0;
            }
            NtProtectVirtualMemory_ptr = (NtProtectVirtualMemory_t*)GetProcAddress(ntdll_o, "NtProtectVirtualMemory");
            logging_func(3, "Alternative ntdll load was successful");
        }
    }
    PVOID protect_base = dst;
    SIZE_T protect_size = count;
    ULONG old_protection;
    status = NtProtectVirtualMemory_ptr(GetCurrentProcess(), &protect_base, &protect_size, PAGE_EXECUTE_READWRITE, &old_protection);
    if (FAILED(status)) {
        logging_func(3, "Unable to protect memory (error 0x%08X)", status);
        return 0;
    }
    status = NtWriteVirtualMemory_ptr(GetCurrentProcess(), dst, src, count, &ret);
    if (FAILED(status)) {
        logging_func(3, "Unable to patch memory (error 0x%08X)", status);
        return 0;
    }
    NtProtectVirtualMemory_ptr(GetCurrentProcess(), &protect_base, &protect_size, old_protection, &old_protection);
    return ret;
}