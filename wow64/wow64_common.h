#ifndef WOW64_COMMON_H
#define WOW64_COMMON_H 1

#define WOW64_DECLARE_OVERLOADS 1

#ifdef COMPILE_WOW64
#define WOW64_EXPORT dllexport
#else
#define WOW64_EXPORT
#endif

#ifdef COMPILE_WOW64_CPU
#define WOW64CPU_EXPORT dllexport
#else
#define WOW64CPU_EXPORT
#endif

#ifdef COMPILE_WOW64_WIN
#define WOW64WIN_EXPORT dllexport
#else
#define WOW64WIN_EXPORT
#endif

#define UNUSED_ARG(...) __VA_ARGS__

struct CPUCONTEXT {
	int __dword_0;
	CONTEXTX<32> context32;
	union {
		uint32_t flags;
		struct {
			uint32_t return_with_iret : 1;
		};
	};
};

struct WOW64INFO {
	uint32_t NativeSystemPageSize; // 0x0
	union {
		uint32_t CpuFlags; // 0x4
		struct {

		};
	};
	union {
		uint32_t Wow64ExecuteFlags; // 0x8
		struct {
			uint32_t : 14;
			uint32_t __unknown_flag_A : 1;
		};
	};
	unknown_fields(0x4);
	uint16_t NativeMachineType; // 0x10
	uint16_t EmulatedMachineType; // 0x12
	// 0x14
};

struct WOW64_TEMP_LIST_HEADER {
	WOW64_TEMP_LIST_HEADER* prev; // 0x0
	WOW64_TEMP_LIST_HEADER* next; // 0x8
};

template<typename T>
struct WOW64_TEMP_LIST {
	WOW64_TEMP_LIST_HEADER header;
	T data;
};

static constexpr size_t TEMP_DATA_BUFFER_SIZE = 2048;

struct WOW64_TEMP_DATA {
	WOW64_TEMP_DATA* __self_ptr_0; // 0x0
	uint8_t* write_ptr; // 0x8
	uint8_t buffer[TEMP_DATA_BUFFER_SIZE]; // 0x10
	// 0x810

	void* allocate(size_t size, WOW64_TEMP_LIST<WOW64_TEMP_DATA>* temp_list);
};

typedef struct WOW64_CALLBACK_DATA WOW64_CALLBACK_DATA;

struct WOW64_CALLBACK_DATA {
	JUMP_BUFFERX<64> jump_buffer; // 0x0
	WOW64_CALLBACK_DATA* prev_callback_data; // 0x100
	// 0x108
};

#define CURRENT_PROCESS_HANDLE ((HANDLE)~(uintptr_t)0)
#define CURRENT_THREAD_HANDLE ((HANDLE)~(uintptr_t)1)
#define CURRENT_HEAP_HANDLE (peb->ProcessHeap)

enum Wow64TLSSlot {
    WOW64_TLS_STACKPTR64 = 0, // 0x1480
    WOW64_TLS_CPURESERVED = 1, // 0x1488
    WOW64_TLS_INCPUSIMULATION = 2, // 0x1490
    WOW64_TLS_TEMPLIST = 3, // 0x1498
    WOW64_TLS_EXCEPTIONADDR = 4, // 0x14A0
    WOW64_TLS_USERCALLBACKDATA = 5, // 0x14A8
    WOW64_TLS_EXTENDED_FLOAT = 6, // 0x14B0
    WOW64_TLS_APCLIST = 7, // 0x14B8
    WOW64_TLS_FILESYSREDIR = 8, // 0x14C0
    WOW64_TLS_LASTWOWCALL = 9, // 0x14C8
    WOW64_TLS_WOW64INFO = 10, // 0x14D0
    WOW64_TLS_INITIAL_TEB32 = 11, // 0x14D8
	WOW64_TLS_PERFDATA = 12, // 0x14E0
	WOW64_TLS_DEBUGGER_COMM = 13, // 0x14E8
	WOW64_TLS_INVALID_STARTUP_CONTEXT = 14, // 0x14F0
	WOW64_TLS_SLIST_FAULT = 15, // 0x14F8
	WOW64_TLS_UNWIND_NATIVE_STACK = 16, // 0x1500
	WOW64_TLS_APC_WRAPPER = 17, // 0x1508
	WOW64_TLS_IN_SUSPEND_THREAD = 18, // 0x1510
};

#define SAVED_RSP_FOR_TEB(teb_arg) (*(void**)&(teb_arg)->TlsSlots[WOW64_TLS_STACKPTR64])
#define CPUCONTEXT_FOR_TEB(teb_arg) (*(CPUCONTEXT**)&(teb_arg)->TlsSlots[WOW64_TLS_CPURESERVED])
#define TEMPDATA_FOR_TEB(teb_arg) (*(WOW64_TEMP_LIST<WOW64_TEMP_DATA>**)&(teb_arg)->TlsSlots[WOW64_TLS_TEMPLIST])
#define USERCALLBACKDATA_FOR_REB(teb_arg) (*(WOW64_CALLBACK_DATA**)&(teb_arg)->TlsSlots[WOW64_TLS_USERCALLBACKDATA])
#define WOW64INFO_FOR_TEB(teb_arg) (*(WOW64INFO**)&(teb_arg)->TlsSlots[WOW64_TLS_WOW64INFO])

#define TEB_SAVED_RSP (*(void* GS_RELATIVE*)&teb->TlsSlots[WOW64_TLS_STACKPTR64])
#define TEB_CPUCONTEXT (*(CPUCONTEXT* GS_RELATIVE*)&teb->TlsSlots[WOW64_TLS_CPURESERVED])
#define TEB_TEMPDATA (*(WOW64_TEMP_LIST<WOW64_TEMP_DATA>* GS_RELATIVE*)&teb->TlsSlots[WOW64_TLS_TEMPLIST])
#define TEB_USERCALLBACKDATA (*(WOW64_CALLBACK_DATA* GS_RELATIVE*)&teb->TlsSlots[WOW64_TLS_USERCALLBACKDATA])
#define TEB_WOW64INFO (*(WOW64INFO* GS_RELATIVE*)&teb->TlsSlots[WOW64_TLS_WOW64INFO])

enum WOW64_FLUSH_REASON : int32_t {
	WOW64_FLUSH_FORCE,
	WOW64_FLUSH_FREE,
	WOW64_FLUSH_ALLOC,
	WOW64_FLUSH_PROTECT
};

#endif