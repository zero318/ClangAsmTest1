#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>

#include "../zero/util.h"

#include "../windows_structs.h"

#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")

#include "..\win_syscalls\syscalls7.h"

#include "wow64_common.h"

#include "wow64cpu.h"

extern "C" {
dll_import NTSYSAPI NTSTATUS LdrDisableThreadCalloutsForDll(PVOID DllHandle);
dll_import NTSYSAPI PVOID RtlAllocateHeap(PVOID HeapHandle, ULONG Flags, SIZE_T Size);
dll_import NTSYSAPI BOOL RtlFreeHeap(PVOID HeapHandle, ULONG Flags, PVOID BaseAddress);
dll_import NTSTATUS LdrQueryImageFileExecutionOptions(PUNICODE_STRING lpImageFile, PCWSTR lpszOption, ULONG dwType, PVOID lpData, ULONG cbData, ULONG* lpcbData);
}

static inline void* heap_alloc(size_t size, HANDLE heap_handle = CURRENT_HEAP_HANDLE) {
    return RtlAllocateHeap(heap_handle, 0, size);
}
static inline BOOL heap_free(void* ptr, HANDLE heap_handle = CURRENT_HEAP_HANDLE) {
    return RtlFreeHeap(heap_handle, 0, ptr);
}


void Wow64ApcRoutine(uintptr_t ApcContext, uintptr_t Arg2, uintptr_t Arg3);

void ThunkContext32TO64(CONTEXTX<32>* Context32, CONTEXTX<64>* Context64, uintptr_t StackBase) {

}


NTSTATUS Wow64pThunkProcessParameters(PEBX<32>* Peb32, PEBX<64>* Peb) {
    //_AddressOfReturnAddress()
}

NTSTATUS ProcessInit(size_t* pCpuThreadSize) {

}

void* Wow64AllocateTempFromHeap(size_t size, WOW64_TEMP_LIST<WOW64_TEMP_DATA>* temp_list) {
    size += sizeof(WOW64_TEMP_LIST_HEADER);
    WOW64_TEMP_LIST<uint8_t[0]>* new_alloc = (WOW64_TEMP_LIST<uint8_t[0]>*)heap_alloc(size);
    if (expect(new_alloc != NULL, true)) {
        auto old_next = temp_list->header.next;
        temp_list->header.next = (WOW64_TEMP_LIST_HEADER*)new_alloc;
        old_next->next = &new_alloc->header;
        new_alloc->header.prev = old_next;
        new_alloc->header.next = (WOW64_TEMP_LIST_HEADER*)&temp_list->header.next; // This seems wrong...
        return &new_alloc->data;
    } else {
        EXCEPTION_RECORDX<64> exception;
        exception.ExceptionCode = STATUS_NO_MEMORY;
        exception.ExceptionFlags = 0;
        exception.ExceptionRecord = NULL;
        exception.NumberParameters = 1;
        exception.ExceptionInformation[0] = size;
        RtlRaiseException((PEXCEPTION_RECORD)&exception);
        unreachable;
    }
}

// Do these do anything?
static size_t TotalHeapHits;
static size_t LocalThreadHeapHits;

void* WOW64_TEMP_DATA::allocate(size_t size, WOW64_TEMP_LIST<WOW64_TEMP_DATA>* temp_list) {
    uint8_t* write_ptr = this->write_ptr;
    if (expect(write_ptr >= &this->buffer[size], true)) {
        //++LocalThreadHeapHits;
        return this->write_ptr = write_ptr - size;
    } else {
        return Wow64AllocateTempFromHeap(size, temp_list);
    }
}

bool Wow64pCheckForSafeExecution(void* argA, void* argB, void* argC, bool skip_first_two_checks) {
    MEMORY_BASIC_INFORMATION memory_info;
    void* base_address;
    if (expect(
        (
            skip_first_two_checks || (
                SUCCEEDED(NtQueryVirtualMemory(CURRENT_PROCESS_HANDLE, argA, MemoryBasicInformation, &memory_info, sizeof(memory_info), NULL)) && (
                    (base_address = memory_info.AllocationBase),
                    SUCCEEDED(NtQueryVirtualMemory(CURRENT_PROCESS_HANDLE, argB, MemoryBasicInformation, &memory_info, sizeof(memory_info), NULL))
                ) &&
                base_address == memory_info.AllocationBase
            )
        ) &&
        SUCCEEDED(NtQueryVirtualMemory(CURRENT_PROCESS_HANDLE, argC, MemoryBasicInformation, &memory_info, sizeof(memory_info), NULL)) &&
        (memory_info.Type & MEM_IMAGE)
    , true)) {
        return true;
    }
    return false;
}

extern "C" {

// Export #1
WOW64_EXPORT void Wow64AllocThreadHeap(TEBX<64>* teb, WOW64_TEMP_LIST<WOW64_TEMP_DATA>* temp_data) {
    WOW64_TEMP_LIST<WOW64_TEMP_DATA>* old_temp_data = TEMPDATA_FOR_TEB(teb);
    TEMPDATA_FOR_TEB(teb) = temp_data;
    temp_data->header.prev = &old_temp_data->header;
    temp_data->data.__self_ptr_0 = &temp_data->data;
    temp_data->data.write_ptr = &temp_data->data.buffer[TEMP_DATA_BUFFER_SIZE];
}

// Export #2
WOW64_EXPORT PVOID Wow64AllocateHeap(size_t Size) {
    return heap_alloc(Size);
}

// Export #3
WOW64_EXPORT void* Wow64AllocateTemp(size_t Size) {
    void* ret = NULL;
    if (expect(Size != 0, true)) {
        //++TotalHeapHits;
        WOW64_TEMP_LIST<WOW64_TEMP_DATA>* temp_data = TEB_TEMPDATA;
        ret = temp_data->data.allocate(AlignUpToMultipleOf2(Size, 16), temp_data);
    }
    return ret;
}

// Export #4
WOW64_EXPORT void Wow64ApcRoutine() {

    // TODO
    auto esp = CpuGetStackPointer();

    CpuGetContext(CURRENT_THREAD_HANDLE, CURRENT_PROCESS_HANDLE, (TEBX<64>*)teb->NtTib.Self, NULL);
}

// Export #5
WOW64_EXPORT bool Wow64CheckIfNXEnabled() {
    SYSTEM_PROCESSOR_INFORMATION processor_info;
    if (
        expect(SUCCEEDED(NtQuerySystemInformation(SystemProcessorInformation, &processor_info, sizeof(processor_info), NULL)), true) &&
        processor_info.__NX_unknown_A
    ) {
        PROCESS_EXECUTE_FLAGS flags;
        if (expect(
            SUCCEEDED(NtQueryInformationProcess(CURRENT_PROCESS_HANDLE, ProcessExecuteFlags, &flags, sizeof(flags), NULL)) &&
            !flags.MEM_EXECUTE_OPTION_DISABLE && (
                processor_info.__NX_unknown_B ||
                flags.MEM_EXECUTE_OPTION_ENABLE
            )
        , true)) {
            return true;
        }
    }
    return false;
}

// Export #6
WOW64_EXPORT void Wow64EmulateAtlThunk() {
    // TODO
}

// Export #7
WOW64_EXPORT BOOL Wow64FreeHeap(void* ptr) {
    return heap_free(ptr);
}

// Export #8
WOW64_EXPORT void Wow64FreeThreadHeap(TEBX<64>* teb_ptr) {
    WOW64_TEMP_LIST<WOW64_TEMP_DATA>* temp_data = TEMPDATA_FOR_TEB(teb_ptr);
    WOW64_TEMP_LIST_HEADER* current = (WOW64_TEMP_LIST_HEADER*)temp_data;
    HANDLE heap = CURRENT_HEAP_HANDLE;
    for (;;) {
        WOW64_TEMP_LIST_HEADER* next = current->next;
        WOW64_TEMP_LIST_HEADER* prev = next->prev;
        temp_data->header.next = prev;
        prev->next = (WOW64_TEMP_LIST_HEADER*)&temp_data->header.next;
        if (expect(next == (WOW64_TEMP_LIST_HEADER*)&temp_data->header.next, false)) {
            break;
        }
        heap_free(next, heap);
        current = prev;
    }
    TEMPDATA_FOR_TEB(teb_ptr) = (WOW64_TEMP_LIST<WOW64_TEMP_DATA>*)temp_data->header.prev;
}

// Export #9
WOW64_EXPORT NTSTATUS Wow64GetWow64ImageOption(HANDLE key, const wchar_t* option, uint32_t type, void* data, uint32_t data_length) {
    NTSTATUS ret;
    auto process_parameters = peb->ProcessParameters;
    if (expect(process_parameters != NULL, true)) {
        if (expect(SUCCEEDED(ret = LdrQueryImageFileExecutionOptions(&process_parameters->ImagePathName, option, type, data, data_length, NULL)), true)) {
            return ret;
        }
    }
    if (expect(key != NULL, true)) {
        UNICODE_STRING string;
        RtlInitUnicodeString(&string, option);
        KEY_VALUE_PARTIAL_INFORMATIONX<520> value_info;
        ULONG return_length;
        if (expect(
            SUCCEEDED(ret = NtQueryValueKey(key, &string, KeyValuePartialInformation, &value_info, sizeof(value_info), &return_length)) &&
            value_info.Type == type &&
            value_info.DataLength <= data_length
        , true)) {
            __builtin_memcpy(data, value_info.Data, value_info.DataLength);
        }
    }
    return ret;
}

#ifdef COMPILE_WOW64
//BOOL APIENTRY DllMain(HMODULE hDll, DWORD ulReasonForCall, LPVOID) {
BOOL APIENTRY _DllMainCRTStartup(HMODULE hDll, DWORD ulReasonForCall, LPVOID) {
    if (expect(ulReasonForCall == DLL_PROCESS_ATTACH, true)) {
        LdrDisableThreadCalloutsForDll(hDll);
    }
    return TRUE;
}
#endif

}