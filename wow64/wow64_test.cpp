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

naked uint8_t Wow64pSetJmp(JUMP_BUFFERX<64>* jump_buffer) {
    __asm {
        XOR RAX, RAX
        MOV QWORD PTR[RCX + 0x8], RBX
        MOV QWORD PTR[RCX + 0x18], RBP
        MOV QWORD PTR[RCX + 0x20], RSI
        MOV QWORD PTR[RCX + 0x28], RDI
        MOV QWORD PTR[RCX + 0x30], R12
        MOV QWORD PTR[RCX + 0x38], R13
        MOV QWORD PTR[RCX + 0x40], R14
        MOV QWORD PTR[RCX + 0x48], R15
        LEA R8, [RSP + 0x8]
        MOV QWORD PTR[RCX + 0x10], R8
        MOV R8, QWORD PTR[RSP]
        MOV QWORD PTR[RCX + 0x50], R8
        STMXCSR DWORD PTR[RCX + 0x58]
        MOVAPS XMMWORD PTR[RCX + 0x60], XMM6
        MOVAPS XMMWORD PTR[RCX + 0x70], XMM7
        MOVAPS XMMWORD PTR[RCX + 0x80], XMM8
        MOVAPS XMMWORD PTR[RCX + 0x90], XMM9
        MOVAPS XMMWORD PTR[RCX + 0xA0], XMM10
        MOVAPS XMMWORD PTR[RCX + 0xB0], XMM11
        MOVAPS XMMWORD PTR[RCX + 0xC0], XMM12
        MOVAPS XMMWORD PTR[RCX + 0xD0], XMM13
        MOVAPS XMMWORD PTR[RCX + 0xE0], XMM14
        MOVAPS XMMWORD PTR[RCX + 0xF0], XMM15
        RET
    }
}

[[noreturn]] naked void Wow64pLongJmp(JUMP_BUFFERX<64>* jump_buffer) {
    __asm {
        MOV RBX, QWORD PTR[RCX + 0x8]
        MOV RSI, QWORD PTR[RCX + 0x20]
        MOV RDI, QWORD PTR[RCX + 0x28]
        MOV R12, QWORD PTR[RCX + 0x30]
        MOV R13, QWORD PTR[RCX + 0x38]
        MOV R14, QWORD PTR[RCX + 0x40]
        MOV R15, QWORD PTR[RCX + 0x48]
        LDMXCSR DWORD PTR[RCX + 0x58]
        MOVAPS XMM6, XMMWORD PTR[RCX + 0x60]
        MOVAPS XMM7, XMMWORD PTR[RCX + 0x70]
        MOVAPS XMM8, XMMWORD PTR[RCX + 0x80]
        MOVAPS XMM9, XMMWORD PTR[RCX + 0x90]
        MOVAPS XMM10, XMMWORD PTR[RCX + 0xA0]
        MOVAPS XMM11, XMMWORD PTR[RCX + 0xB0]
        MOVAPS XMM12, XMMWORD PTR[RCX + 0xC0]
        MOVAPS XMM13, XMMWORD PTR[RCX + 0xD0]
        MOVAPS XMM14, XMMWORD PTR[RCX + 0xE0]
        MOVAPS XMM15, XMMWORD PTR[RCX + 0xF0]
        MOV RDX, QWORD PTR[RCX + 0x50]
        MOV RBP, QWORD PTR[RCX + 0x18]
        MOV RSP, QWORD PTR[RCX + 0x10]
        MOV RAX, 1
        JMP RDX
    }
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

// Export #10
WOW64_EXPORT NTSTATUS Wow64KiUserCallbackDispatcher(WOW64_CALLBACK_DATA* callback_data) {
    NTSTATUS ret;
    callback_data->prev_callback_data = TEB_USERCALLBACKDATA;
    TEB_USERCALLBACKDATA = callback_data;
    WOW64_TEMP_LIST<WOW64_TEMP_DATA>* temp_data = TEB_TEMPDATA;
    if (!Wow64pSetJmp(&callback_data->jump_buffer)) {
        CONTEXTX<32> context32;
        CpuGetContext(CURRENT_THREAD_HANDLE, CURRENT_PROCESS_HANDLE, (TEBX<64>*)teb->NtTib.Self, &context32);
        // TODO
    }
    else {
        // TODO
    }
    return ret;
}

// Export #11
WOW64_EXPORT void Wow64LdrpInitialize(CONTEXTX<64>* context) {

}

// Export #12
//Wow64LogPrint

static OBJECT_ATTRIBUTES key_attributes;

// Export #13
WOW64_EXPORT NTSTATUS Wow64OpenConfigKey(HANDLE* key, ACCESS_MASK desired_access) {
    return NtOpenKey(key, desired_access, &key_attributes);
}

// Export #14
WOW64_EXPORT void Wow64PrepareForDebuggerAttach() {
    return CpuPrepareForDebuggerAttach();
}

// Export #15
WOW64_EXPORT void Wow64PrepareForException(EXCEPTION_RECORDX<64>* exception_record, CONTEXTX<64>* context) {
    uintptr_t saved_rsp = (uintptr_t)TEB_SAVED_RSP;
    if (saved_rsp) {
        uint32_t cur_esp = esp_reg;
        uint32_t esp_diff = (uint32_t)context->Rsp - cur_esp;
        saved_rsp = AlignDownToMultipleOf2(saved_rsp - esp_diff, 16);
        __builtin_memcpy((void*)saved_rsp, (void*)cur_esp, esp_diff);
        cur_esp = esp_reg;
        exception_record = (EXCEPTION_RECORDX<64>*)((uint32_t)exception_record - cur_esp + saved_rsp);
        context = (CONTEXTX<64>*)((uint32_t)context - cur_esp + saved_rsp);
        rsp_reg = saved_rsp;
    }
    EXCEPTION_POINTERSX<64> exception_pointers;
    exception_pointers.ExceptionRecord = exception_record;
    exception_pointers.ContextRecord = context;
    CpuResetToConsistentState(&exception_pointers);
    register uintptr_t r11 asm("r11") = (uintptr_t)teb->NtTib.Self;
    __asm__(""::"r"(r11));
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