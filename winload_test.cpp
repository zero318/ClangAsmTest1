#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "zero/util.h"
#include "zero/custom_intrin.h"

#include "x86_msrs.h"

#include "F:/My Hacking Stuff Expansion/windows/ntoskrnl_structs.h"

typedef uintptr_t EFI_STATUS;

#define EFI_FALSE                   0
#define EFI_TRUE                    1

#define EFI_SUCCESS                 0x0000000000000000
#define EFI_ERR                     0x8000000000000000
#define EFI_LOAD_ERROR              (EFI_ERR | 0x0000000000000001)
#define EFI_INVALID_PARAMETER       (EFI_ERR | 0x0000000000000002)
#define EFI_UNSUPPORTED             (EFI_ERR | 0x0000000000000003)
#define EFI_BAD_BUFFER_SIZE         (EFI_ERR | 0x0000000000000004)
#define EFI_BUFFER_TOO_SMALL        (EFI_ERR | 0x0000000000000005)
#define EFI_NOT_READY               (EFI_ERR | 0x0000000000000006)
#define EFI_DEVICE_ERROR            (EFI_ERR | 0x0000000000000007)
#define EFI_WRITE_PROTECTED         (EFI_ERR | 0x0000000000000008)
#define EFI_OUT_OF_RESOURCES        (EFI_ERR | 0x0000000000000009)
#define EFI_VOLUME_CORRUPTED        (EFI_ERR | 0x000000000000000a)
#define EFI_VOLUME_FULL             (EFI_ERR | 0x000000000000000b)
#define EFI_NO_MEDIA                (EFI_ERR | 0x000000000000000c)
#define EFI_MEDIA_CHANGED           (EFI_ERR | 0x000000000000000d)
#define EFI_NOT_FOUND               (EFI_ERR | 0x000000000000000e)
#define EFI_ACCESS_DENIED           (EFI_ERR | 0x000000000000000f)
#define EFI_NO_RESPONSE             (EFI_ERR | 0x0000000000000010)
#define EFI_NO_MAPPING              (EFI_ERR | 0x0000000000000011)
#define EFI_TIMEOUT                 (EFI_ERR | 0x0000000000000012)
#define EFI_NOT_STARTED             (EFI_ERR | 0x0000000000000013)
#define EFI_ALREADY_STARTED         (EFI_ERR | 0x0000000000000014)
#define EFI_ABORTED                 (EFI_ERR | 0x0000000000000015)
#define EFI_ICMP_ERROR              (EFI_ERR | 0x0000000000000016)
#define EFI_TFTP_ERROR              (EFI_ERR | 0x0000000000000017)
#define EFI_PROTOCOL_ERROR          (EFI_ERR | 0x0000000000000018)
#define EFI_INCOMPATIBLE_VERSION    (EFI_ERR | 0x0000000000000019)
#define EFI_SECURITY_VIOLATION      (EFI_ERR | 0x000000000000001a)
#define EFI_CRC_ERROR               (EFI_ERR | 0x000000000000001b)
#define EFI_END_OF_MEDIA            (EFI_ERR | 0x000000000000001c)
#define EFI_END_OF_FILE             (EFI_ERR | 0x000000000000001f)
#define EFI_INVALID_LANGUAGE        (EFI_ERR | 0x0000000000000020)
#define EFI_COMPROMISED_DATA        (EFI_ERR | 0x0000000000000021)
#define EFI_IP_ADDRESS_CONFLICT     (EFI_ERR | 0x0000000000000022)
#define EFI_HTTP_ERROR              (EFI_ERR | 0x0000000000000023)

#define EFI_ERROR(status) ((status) != EFI_SUCCESS)

typedef void* EFI_HANDLE;
typedef void* EFI_EVENT;

typedef uint64_t EFI_PHYSICAL_ADDRESS;
typedef uint64_t EFI_VIRTUAL_ADDRESS;

typedef int32_t NTSTATUS;

typedef size_t EFI_TPL;

struct alignas(8) EFI_GUID {
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint8_t Data4[8];
};

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#ifdef __cplusplus
#define OptionalPtr = NULL
#else
#define OptionalPtr
#endif

#define EFIAPI

// size: 0x18
struct EFI_TABLE_HEADER {
	uint64_t Signature; // 0x0
	union {
		uint32_t Revision; // 0x8
		struct {
			uint16_t MinorRevision; // 0x8
			uint16_t MajorRevision; // 0xA
		};
	};
	uint32_t HeaderSize; // 0xC
	uint32_t CRC32; // 0x10
	uint32_t Reserved; // 0x14
	// 0x18
};

// size: 0x4
struct EFI_INPUT_KEY {
	uint16_t ScanCode; // 0x0
	char16_t UnicodeChar; // 0x2
	// 0x4
};

struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_INPUT_RESET)(
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This,
	bool ExtendedVerification
);
typedef EFI_STATUS(EFIAPI *EFI_INPUT_READ_KEY)(
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This,
	EFI_INPUT_KEY* Key
);
typedef EFI_STATUS(EFIAPI *EFI_WAIT_FOR_EVENT)(
	size_t NumberOfEvents,
	EFI_EVENT* Event,
	size_t* Index
);

// size: 0x18
struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	EFI_INPUT_RESET Reset; // 0x0
	EFI_INPUT_READ_KEY ReadKeyStroke; // 0x8
	EFI_EVENT WaitForKey; // 0x10
	// 0x18
};

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_TEXT_RESET)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	bool ExtendedVerification
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_STRING)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	char16_t* String
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_TEST_STRING)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	char16_t* String
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_QUERY_MODE)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	size_t ModeNumber,
	size_t* Columns,
	size_t* Rows
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_MODE)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	size_t ModeNumber
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	size_t Attribute
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	size_t Column,
	size_t Row
);
typedef EFI_STATUS(EFIAPI *EFI_TEXT_ENABLE_CURSOR)(
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	bool Visible
);

// size: 0x14
struct SIMPLE_TEXT_OUTPUT_MODE {
	int32_t MaxMode; // 0x0
	// Current Settings
	int32_t Mode; // 0x4
	int32_t Attribute; // 0x8
	int32_t CursorColumn; // 0xC
	int32_t CursorRow; // 0x10
	int32_t CursorVisible; // 0x14
	// 0x18
};

// size: 0x50
struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	EFI_TEXT_RESET Reset; // 0x0
	EFI_TEXT_STRING OutputString; // 0x8
	EFI_TEXT_TEST_STRING TestString; // 0x10
	EFI_TEXT_QUERY_MODE QueryMode; // 0x18
	EFI_TEXT_SET_MODE SetMode; // 0x20
	EFI_TEXT_SET_ATTRIBUTE SetAttribute; // 0x28
	EFI_TEXT_CLEAR_SCREEN ClearScreen; // 0x30
	EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition; // 0x38
	EFI_TEXT_ENABLE_CURSOR EnableCursor; // 0x40
	SIMPLE_TEXT_OUTPUT_MODE* Mode; // 0x48
	// 0x50
};

// size: 0x10
struct EFI_TIME {
	uint16_t Year; // 0x0
	uint8_t Month; // 0x2
	uint8_t Day; // 0x3
	uint8_t Hour; // 0x4
	uint8_t Minute; // 0x5
	uint8_t Second; // 0x6
	uint8_t Pad1; // 0x7
	uint32_t Nanosecond; // 0x8
	int16_t TimeZone; // 0xC
	uint8_t Daylight; // 0xE
	uint8_t Pad2; // 0xF
	// 0x10
};

// size: 0xC
struct EFI_TIME_CAPABILITIES {
	uint32_t Resolution; // 0x0
	uint32_t Accuracy; // 0x4
	bool SetsToZero; // 0x8
	// 0x9
};

typedef EFI_STATUS(EFIAPI *EFI_GET_TIME)(
	EFI_TIME* Time,
	EFI_TIME_CAPABILITIES* Capabilities // Optional
);
typedef EFI_STATUS(EFIAPI *EFI_SET_TIME)(
	EFI_TIME* Time
);
typedef EFI_STATUS(EFIAPI *EFI_GET_WAKEUP_TIME)(
	bool* Enabled,
	bool* Pending,
	EFI_TIME* Time
);
typedef EFI_STATUS(EFIAPI *EFI_SET_WAKEUP_TIME)(
	bool Enable,
	EFI_TIME* Time // Optional
);

// size: 0x28
struct EFI_MEMORY_DESCRIPTOR {
	uint32_t Type; // 0x0
	// 0x4
	EFI_PHYSICAL_ADDRESS PhysicalStart; // 0x8
	EFI_VIRTUAL_ADDRESS VirtualStart; // 0x10
	uint64_t NumberOfPages; // 0x18
	uint64_t Attribute; // 0x20
	// 0x28
};

typedef EFI_STATUS(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(
	size_t MemoryMapSize,
	size_t DescriptorSize,
	uint32_t DescriptorVersion,
	EFI_MEMORY_DESCRIPTOR* VirtualMap
);
typedef EFI_STATUS(EFIAPI *EFI_CONVERT_POINTER)(
	size_t DebugDisposition,
	void** Address
);

typedef EFI_STATUS(EFIAPI *EFI_GET_VARIABLE)(
	char16_t* VariableName,
	EFI_GUID* VendorGuid,
	uint32_t* Attributes, // Optional
	size_t* DataSize,
	void* Data // Optional
);
typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_VARIBALE_NAME)(
	size_t* VariableNameSize,
	char16_t* VariableName,
	EFI_GUID* VendorGuid
);
typedef EFI_STATUS(EFIAPI *EFI_SET_VARIABLE)(
	char16_t* VariableName,
	EFI_GUID* VendorGuid,
	uint32_t Attributes,
	size_t DataSize,
	void* Data
);

typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(
	uint32_t* Highcount
);

enum EFI_RESET_TYPE {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
};

typedef void(gnu_attr(noreturn) EFIAPI *EFI_RESET_SYSTEM)(
	EFI_RESET_TYPE ResetType,
	EFI_STATUS ResetStatus,
	size_t DataSize,
	void* ResetData // Optional
);

// size: 0x20
struct EFI_CAPSULE_HEADER {
	EFI_GUID CapsuleGuid; // 0x0
	uint32_t HeaderSize; // 0x10
	uint32_t Flags; // 0x14
	uint32_t CapsuleImageSize; // 0x18
	// 0x1C
};

typedef EFI_STATUS(EFIAPI *EFI_UPDATE_CAPSULE)(
	EFI_CAPSULE_HEADER** CapsuleHeaderArray,
	size_t CapsuleCount,
	EFI_PHYSICAL_ADDRESS ScatterGatherList // Optional
);
typedef EFI_STATUS(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(
	EFI_CAPSULE_HEADER** CapsuleHeaderArray,
	size_t CapsuleCount,
	uint64_t* MaximumCapsuleSize,
	EFI_RESET_TYPE* ResetType
);

typedef EFI_STATUS(EFIAPI *EFI_QUERY_VARIABLE_INFO)(
	uint32_t Attributes,
	uint64_t* MaximumVariableStorageSize,
	uint64_t* RemainingVariableStorageSize,
	uint64_t* MaximumVariableSize
);

// size: 0x88
struct EFI_RUNTIME_SERVICES {
	EFI_TABLE_HEADER Hdr; // 0x0

	//
	// Time Services
	//
	EFI_GET_TIME GetTime; // 0x18
	EFI_SET_TIME SetTime; // 0x20
	EFI_GET_WAKEUP_TIME GetWakeupTime; // 0x28
	EFI_SET_WAKEUP_TIME SetWakeupTime; // 0x30

	//
	// Virtual Memory Services
	//
	EFI_SET_VIRTUAL_ADDRESS_MAP SetVirtualAddressMap; // 0x38
	EFI_CONVERT_POINTER ConvertPointer; // 0x40

	//
	// Variable Services
	//
	EFI_GET_VARIABLE GetVariable; // 0x48
	EFI_GET_NEXT_VARIBALE_NAME GetNextVariableName; // 0x50
	EFI_SET_VARIABLE SetVariable; // 0x58

	//
	// Miscellaneous Services
	//
	EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount; // 0x60
	EFI_RESET_SYSTEM ResetSystem; // 0x68

	//
	// UEFI 2.0 Capsule Services
	//
	EFI_UPDATE_CAPSULE UpdateCapsule; // 0x70
	EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities; // 0x78

	//
	// Miscellaneous UEFI 2.0 Service
	//
	EFI_QUERY_VARIABLE_INFO QueryVariableInfo; // 0x80
	// 0x88
};

typedef EFI_TPL(EFIAPI *EFI_RAISE_TPL)(
	EFI_TPL NewTpl
);
typedef void(EFIAPI *EFI_RESTORE_TPL)(
	EFI_TPL OldTpl
);

enum EFI_ALLOCATE_TYPE {
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddress,
	MaxAllocateType
};
enum EFI_MEMORY_TYPE {
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,
	EfiBootServicesData,
	EfiRuntimeServicesCode,
	EfiRuntimeServicesData,
	EfiConventionalMemory,
	EfiUnusableMemory,
	EfiACPIReclaimMemory,
	EfiACPIMemoryNVS,
	EfiMemoryMappedIO,
	EfiMemoryMappedIOPortSpace,
	EfiPalCode,
	EfiPersistentMemory,
	EfiUnacceptedMemoryType,
	EfiMaxMemoryType
};
typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_PAGES)(
	EFI_ALLOCATE_TYPE Type,
	EFI_MEMORY_TYPE MemoryType,
	size_t Pages,
	EFI_PHYSICAL_ADDRESS* Memory
);
typedef EFI_STATUS(EFIAPI *EFI_FREE_PAGES)(
	EFI_PHYSICAL_ADDRESS Memory,
	size_t Pages
);
typedef EFI_STATUS(EFIAPI *EFI_GET_MEMORY_MAP)(
	size_t* MemoryMapSize,
	EFI_MEMORY_DESCRIPTOR* MemoryMap,
	size_t* MapKey,
	size_t* DescriptorSize,
	uint32_t* DescriptorVersion
);
typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_POOL)(
	EFI_MEMORY_TYPE PoolType,
	size_t Size,
	void** Buffer
);
typedef EFI_STATUS(EFIAPI *EFI_FREE_POOL)(
	void* Buffer
);

typedef void(EFIAPI *EFI_EVENT_NOTIFY)(
	EFI_EVENT Event,
	void* Context
);
typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT)(
	uint32_t Type,
	EFI_TPL NotifyTpl,
	EFI_EVENT_NOTIFY NotifyFunction, // Optional
	void* NotifyContext, // Optional
	EFI_EVENT* Event
);
typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT_EX)(
	uint32_t Type,
	EFI_TPL NotifyTpl,
	EFI_EVENT_NOTIFY NotifyFunction, // Optional
	const void* NotifyContext, // Optional
	const EFI_GUID* EventGroup, // Optional
	EFI_EVENT* Event
);


enum EFI_TIMER_DELAY {
	TimerCancel,
	TimerPeriodic,
	TimerRelative
};
typedef EFI_STATUS(EFIAPI *EFI_SET_TIMER)(
	EFI_EVENT Event,
	EFI_TIMER_DELAY Type,
	uint64_t TriggerTime
);
typedef EFI_STATUS(EFIAPI *EFI_SIGNAL_EVENT)(
	EFI_EVENT Event
);
typedef EFI_STATUS(EFIAPI *EFI_CLOSE_EVENT)(
	EFI_EVENT Event
);
typedef EFI_STATUS(EFIAPI *EFI_CHECK_EVENT)(
	EFI_EVENT Event
);

enum EFI_INTERFACE_TYPE {
	EFI_NATIVE_INTERFACE
};
typedef EFI_STATUS(EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE)(
	EFI_HANDLE* Handle,
	EFI_GUID* Protocol,
	EFI_INTERFACE_TYPE InterfaceType,
	void* Interface
);
typedef EFI_STATUS(EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE)(
	EFI_HANDLE Handle,
	EFI_GUID* Protocol,
	void* OldInterface,
	void* NewInterface
);
typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE)(
	EFI_HANDLE Handle,
	EFI_GUID* Protocol,
	void* Interface
);
typedef EFI_STATUS(EFIAPI *EFI_HANDLE_PROTOCOL)(
	EFI_HANDLE Handle,
	EFI_GUID* Protocol,
	void** Interface
);
typedef EFI_STATUS(EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY)(
	EFI_GUID* Protocol,
	EFI_EVENT Event,
	void** Registration
);
enum EFI_LOCATE_SEARCH_TYPE {
	AllHandles,
	ByRegisterNotify,
	ByProtocol
};
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE)(
	EFI_LOCATE_SEARCH_TYPE SearchType,
	EFI_GUID* Protocol, // Optional
	void* SearchKey, // Optional
	size_t* BufferSize,
	EFI_HANDLE* Buffer
);

// size: 0x4
struct EFI_DEVICE_PATH_PROTOCOL {
	uint8_t Type; // 0x0
	uint8_t SubType; // 0x1
	uint8_t Length[2]; // 0x2
	// 0x4
};

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_DEVICE_PATH)(
	EFI_GUID* Protocol,
	EFI_DEVICE_PATH_PROTOCOL** DevicePath,
	EFI_HANDLE* Device
);
typedef EFI_STATUS(EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE)(
	EFI_GUID* Guid,
	void* Table
);


typedef EFI_STATUS(EFIAPI *EFI_IMAGE_LOAD)(
	bool BootPolicy,
	EFI_HANDLE ParentImageHandle,
	EFI_DEVICE_PATH_PROTOCOL* DevicePath, // Optional
	void* SourceBuffer, // Optional
	size_t SourceSize,
	EFI_HANDLE* ImageHandle
);
typedef EFI_STATUS(EFIAPI *EFI_IMAGE_START)(
	EFI_HANDLE ImageHandle,
	size_t* ExitDataSize,
	char16_t** ExitData // Optional
);
typedef EFI_STATUS(EFIAPI *EFI_EXIT)(
	EFI_HANDLE ImageHandle,
	EFI_STATUS ExitStatus,
	size_t ExitDataSize,
	char16_t* ExitData // Optional
);
typedef EFI_STATUS(EFIAPI *EFI_IMAGE_UNLOAD)(
	EFI_HANDLE ImageHandle
);
typedef EFI_STATUS(EFIAPI *EFI_EXIT_BOOT_SERVICES)(
	EFI_HANDLE ImageHandle,
	size_t MapKey
);

typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT)(
	uint64_t* Count
);
typedef EFI_STATUS(EFIAPI *EFI_STALL)(
	size_t Microseconds
);
typedef EFI_STATUS(EFIAPI *EFI_SET_WATCHDOG_TIMER)(
	size_t Timeout,
	uint64_t WatchdogCode,
	size_t DataSize,
	char16_t* WatchdogData // Optional
);

typedef EFI_STATUS(EFIAPI *EFI_CONNECT_CONTROLLER)(
	EFI_HANDLE ControllerHandle,
	EFI_HANDLE* DriverImageHandle, // Optional
	EFI_DEVICE_PATH_PROTOCOL* RemainingDevicePath, // Optional
	bool Recursive
);
typedef EFI_STATUS(EFIAPI *EFI_DISCONNECT_CONTROLLER)(
	EFI_HANDLE ControllerHandle,
	EFI_HANDLE DriverImageHandle, // Optional
	EFI_HANDLE ChildHandle // Optional
);

typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL)(
	EFI_HANDLE Handle,
	EFI_GUID* Protocol,
	void** Interface, // Optional
	EFI_HANDLE AgentHandle,
	EFI_HANDLE ControllerHandle,
	uint32_t Attributes
);
typedef EFI_STATUS(EFIAPI *EFI_CLOSE_PROTOCOL)(
	EFI_HANDLE Handle,
	EFI_GUID* Protocol,
	EFI_HANDLE AgentHandle,
	EFI_HANDLE ControllerHandle
);

// size: 0x18
struct EFI_OPEN_PROTOCOL_INFORMATION_ENTRY {
	EFI_HANDLE AgentHandle; // 0x0
	EFI_HANDLE ControllerHandle; // 0x8
	uint32_t Attributes; // 0x10
	uint32_t OpenCount; // 0x14
	// 0x18
};

typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION)(
	EFI_HANDLE Handle,
	EFI_GUID* Protocol,
	EFI_OPEN_PROTOCOL_INFORMATION_ENTRY** EntryBuffer,
	size_t* EntryCount
);

typedef EFI_STATUS(EFIAPI *EFI_PROTOCOLS_PER_HANDLE)(
	EFI_HANDLE Handle,
	EFI_GUID*** ProtocolBuffer,
	size_t* ProtocolBufferCount
);
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(
	EFI_LOCATE_SEARCH_TYPE SearchType,
	EFI_GUID* Protocol, // Optional
	void* SearchKey, // Optional
	size_t* NoHandles,
	EFI_HANDLE** Buffer
);
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL)(
	EFI_GUID* Protocol,
	void* Registration, // Optional
	void** Interface
);
typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
	EFI_HANDLE Handle,
	...
);

typedef EFI_STATUS(EFIAPI *EFI_CALCULATE_CRC32)(
	void* Data,
	size_t DataSize,
	uint32_t* Crc32
);

typedef void(EFIAPI *EFI_COPY_MEM)(
	void* Destination,
	void* Source,
	size_t Length
);
typedef void(EFIAPI *EFI_SET_MEM)(
	void* Buffer,
	size_t Size,
	uint8_t Value
);

// size: 0x178
struct EFI_BOOT_SERVICES {
	EFI_TABLE_HEADER Hdr; // 0x0

	//
	// Task Priority Services
	//
	EFI_RAISE_TPL RaiseTPL; // 0x18
	EFI_RESTORE_TPL RestoreTPL; // 0x20

	//
	// Memory Services
	//
	EFI_ALLOCATE_PAGES AllocatePages; // 0x28
	EFI_FREE_PAGES FreePages; // 0x30
	EFI_GET_MEMORY_MAP GetMemoryMap; // 0x38
	EFI_ALLOCATE_POOL AllocatePool; // 0x40
	EFI_FREE_POOL FreePool; // 0x48

	//
	// Event & Timer Services
	//
	EFI_CREATE_EVENT CreateEvent; // 0x50
	EFI_SET_TIMER SetTimer; // 0x58
	EFI_WAIT_FOR_EVENT WaitForEvent; // 0x60
	EFI_SIGNAL_EVENT SignalEvent; // 0x68
	EFI_CLOSE_EVENT CloseEvent; // 0x70
	EFI_CHECK_EVENT CheckEvent; // 0x78

	//
	// Protocol Handler Services
	//
	EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface; // 0x80
	EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface; // 0x88
	EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface; // 0x90
	EFI_HANDLE_PROTOCOL HandleProtocol; // 0x98
	void* Reserved; // 0xA0
	EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify; // 0xA8
	EFI_LOCATE_HANDLE LocateHandle; // 0xB0
	EFI_LOCATE_DEVICE_PATH LocateDevicePath; // 0xB8
	EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable; // 0xC0

	//
	// Image Services
	//
	EFI_IMAGE_LOAD LoadImage; // 0xC8
	EFI_IMAGE_START StartImage; // 0xD0
	EFI_EXIT Exit; // 0xD8
	EFI_IMAGE_UNLOAD UnloadImage; // 0xE0
	EFI_EXIT_BOOT_SERVICES ExitBootServices; // 0xE8

	//
	// Miscellaneous Services
	//
	EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount; // 0xF0
	EFI_STALL Stall; // 0xF8
	EFI_SET_WATCHDOG_TIMER SetWatchdogTimer; // 0x100

	//
	// Driver Support Services
	//
	EFI_CONNECT_CONTROLLER ConnectController; // 0x108
	EFI_DISCONNECT_CONTROLLER DisconnectController; // 0x110

	//
	// Open and Close Protocol Services
	//
	EFI_OPEN_PROTOCOL OpenProtocol; // 0x118
	EFI_CLOSE_PROTOCOL CloseProtocol; // 0x120
	EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation; // 0x128

	//
	// Library Services
	//
	EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle; // 0x130
	EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer; // 0x138
	EFI_LOCATE_PROTOCOL LocateProtocol; // 0x140
	EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES InstallMultipleProtocolInterfaces; // 0x148
	EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces; // 0x150

	//
	// 32-bit CRC Services
	//
	EFI_CALCULATE_CRC32 CalculateCrc32; // 0x158

	//
	// Miscellaneous Services
	//
	EFI_COPY_MEM CopyMem; // 0x160
	EFI_SET_MEM SetMem; // 0x168
	EFI_CREATE_EVENT_EX CreateEventEx; // 0x170
	// 0x178
};

// size: 0x18
struct EFI_CONFIGURATION_TABLE {
	EFI_GUID VendorGuid; // 0x0
	void* VendorTable; // 0x10
	// 0x18
};

// size: 0x78
struct EFI_SYSTEM_TABLE {
	EFI_TABLE_HEADER Hdr; // 0x0
	char16_t* FirmwareVendor; // 0x18
	uint32_t FirmwareRevision; // 0x20
	// 0x24
	EFI_HANDLE ConsoleInHandle; // 0x28
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn; // 0x30
	EFI_HANDLE ConsoleOutHandle; // 0x38
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut; // 0x40
	EFI_HANDLE StandardErrorHandle; // 0x48
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr; // 0x50
	EFI_RUNTIME_SERVICES* RuntimeServices; // 0x58
	EFI_BOOT_SERVICES* BootServices; // 0x60
	size_t NumberOfTableEntries; // 0x68
	EFI_CONFIGURATION_TABLE* ConfigurationTable; // 0x70
	// 0x78
};

#define BL_RETURN_ARGUMENTS_VERSION 1

#define BL_LIBRARY_FLAG_NO_DISPLAY                      0x01
#define BL_LIBRARY_FLAG_REINITIALIZE                    0x02
#define BL_LIBRARY_FLAG_REINITIALIZE_ALL                0x04
#define BL_LIBRARY_FLAG_ZERO_HEAP_ALLOCATIONS_ON_FREE   0x10
#define BL_LIBRARY_FLAG_INITIALIZATION_COMPLETED        0x20
#define BL_LIBRARY_FLAG_NO_GRAPHICS_CONSOLE             0x800

// size: 0x38
struct BOOT_APPLICATION_PARAMETER_BLOCK {
	union {
		uint32_t Signature[2]; // 0x0
		uint64_t Signature_full; // 0x0
	};
	uint32_t Version; // 0x8
	uint32_t Size; // 0xC
	uint32_t ImageType; // 0x10
	uint32_t MemoryTranslationType; // 0x14
	uint64_t ImageBase; // 0x18
	uint32_t ImageSize; // 0x20
	uint32_t MemoryDataOffset; // 0x24
	uint32_t AppEntryOffset; // 0x28
	uint32_t BootDeviceOffset; // 0x2C
	uint32_t FirmwareParametersOffset; // 0x30
	uint32_t ReturnArgumentsOffset; // 0x34
	// 0x38
};
#pragma region // BOOT_APPLICATION_PARAMETER_BLOCK Validation
ValidateFieldOffset(0x0, BOOT_APPLICATION_PARAMETER_BLOCK, Signature);
ValidateFieldOffset(0x0, BOOT_APPLICATION_PARAMETER_BLOCK, Signature);
#pragma endregion

union BL_LIBRARY_PARAMETERS_FLAGS {
	uint32_t raw;
	struct {
		uint32_t : 1;
		uint32_t __unknown_flag_a : 1;
		uint32_t __unknown_flag_b : 1;
		uint32_t : 1;
		uint32_t __unknown_flag_c : 1;
	};
};

enum BL_TRANSLATION_TYPE : uint32_t {
	BlNone = 0,
	BlVirtual = 1,
	BlPae = 2,
	BlMax = 3
};

// size: 0x28
struct BL_LIBRARY_PARAMETERS {
	union {
		BL_LIBRARY_PARAMETERS_FLAGS LibraryFlags; // 0x0
		struct {
			uint32_t : 1;
			uint32_t __unknown_flag_a : 1;
			uint32_t __unknown_flag_b : 1;
			uint32_t : 1;
			uint32_t __unknown_flag_c : 1;
		};
	};
	BL_TRANSLATION_TYPE TranslationType; // 0x4
	uint32_t MinimumAllocationCount; // 0x8
	uint32_t MinimumHeapSize; // 0xC
	uint32_t HeapAllocationAttributes; // 0x10
	// 0x14
	wchar_t* ApplicationBaseDirectory; // 0x18
	uint32_t DescriptorCount; // 0x20
	// 0x24
};

struct UnknownStructB {
	unsigned char padding[0x34]; // 0x0
	uint32_t __uint_34; // 0x34
	// 0x38
};

union BL_RETURN_ARGUMENTS_FLAGS {
	uint32_t raw;
	struct {

	};
};

struct BL_RETURN_ARGUMENTS {
	uint32_t Version; // 0x0
	NTSTATUS Status; // 0x4
	union {
		BL_RETURN_ARGUMENTS_FLAGS Flags;
		struct {

		};
	};
	// 0xC
};

dllexport gnu_noinline NTSTATUS OslpMain(
	BL_RETURN_ARGUMENTS_FLAGS* ReturnFlags
) {
	use_var(ReturnFlags);
	return rand();
}

dllexport gnu_noinline NTSTATUS BlInitializeLibrary(
	EFI_HANDLE ImageHandle,
	BL_LIBRARY_PARAMETERS* LibraryParameters
) {
	use_var(ImageHandle);
	use_var(LibraryParameters);
	return rand();
}

dllexport gnu_noinline void BlDestroyLibrary(
	void
) {

}

dllexport gnu_noinline NTSTATUS OslMain(
	BOOT_APPLICATION_PARAMETER_BLOCK* BootParameters
) {
	BL_LIBRARY_PARAMETERS LibraryParameters_a;
	BL_LIBRARY_PARAMETERS LibraryParameters_b;
	BL_RETURN_ARGUMENTS* return_agruments_ptr = (BL_RETURN_ARGUMENTS*)BootParameters->ReturnArgumentsOffset;
	LibraryParameters_a.HeapAllocationAttributes = 0;
	LibraryParameters_a.ApplicationBaseDirectory = NULL;
	LibraryParameters_a.DescriptorCount = 0;
	return_agruments_ptr = based_pointer<BL_RETURN_ARGUMENTS>(BootParameters, return_agruments_ptr);
	return_agruments_ptr->Version = BL_RETURN_ARGUMENTS_VERSION;
	LibraryParameters_a.LibraryFlags.raw = 0x14;
	LibraryParameters_a.TranslationType = BlVirtual;
	LibraryParameters_a.MinimumAllocationCount = 1024;
	LibraryParameters_a.MinimumHeapSize = 2 * 1024 * 1024;
	memcpy(&LibraryParameters_b, &LibraryParameters_a, sizeof(BL_LIBRARY_PARAMETERS));
	LibraryParameters_b.TranslationType = BlVirtual;
	LibraryParameters_b.MinimumHeapSize = 2 * 1024 * 1024;
	LibraryParameters_b.MinimumAllocationCount = 1024;
	NTSTATUS return_status = BlInitializeLibrary(BootParameters, &LibraryParameters_b);
	if (NT_SUCCESS(return_status)) {
		return_agruments_ptr->Status = return_status;
	} else {
		BL_RETURN_ARGUMENTS_FLAGS flags;
		return_status = OslpMain(&flags);
		return_agruments_ptr->Flags = flags;
		return_agruments_ptr->Status = return_status;
		BlDestroyLibrary();
	}
	return return_status;
}

static const EFI_GUID UNKNOWN_GUID_A = {
	.Data1 = 0x5B1B31A1,
	.Data2 = 0x9562,
	.Data3 = 0x11D2,
	.Data4 = { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B }
};
static const EFI_GUID UNKNOWN_GUID_B = {
	.Data1 = 0x09576E91,
	.Data2 = 0x6D3F,
	.Data3 = 0x11D2,
	.Data4 = { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B }
};

dllexport gnu_noinline EFI_STATUS __sub_10007A8C(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable,
	void* Arg3
) {

}

#define STATUS_INVALID_PARAMETER 0xC000000D

dllexport gnu_noinline EFI_STATUS efi_main(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
) {
	if (__sub_10007A8C(ImageHandle, SystemTable, NULL)) {
		return STATUS_INVALID_PARAMETER;
	}
}