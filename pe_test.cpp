#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "zero/util.h"
#include "zero/custom_intrin.h"

#include <utility>
#include <bit>

#include <Windows.h>

#define if_exists(type) []()consteval->bool{__if_exists(type){return true;} return false;}()
/*
template<typename T>
[[nodiscard]] static inline gnu_attr(malloc) T* malloc_array(size_t count) {
	return (T*)malloc(sizeof(T[count]));
}
*/

template<typename T = void>
[[nodiscard]] gnu_noinline gnu_attr(malloc) T* read_file_to_buffer(const char* path) {
	T* ret = NULL;
	if (FILE* file = fopen(path, "rb")) {
		_fseek_nolock(file, 0, SEEK_END);
#ifdef _M_IX86
		int32_t file_size = _ftell_nolock(file);
#else
		int64_t file_size = _ftelli64_nolock(file);
#endif
		if ((ret = (T*)malloc(file_size))) {
			_fseek_nolock(file, 0, SEEK_SET);
			_fread_nolock(ret, file_size, 1, file);
		}
		_fclose_nolock(file);
	}
	return ret;
}

static constexpr size_t sizeof_ptr = sizeof(void *__ptr32);

#define PARAGRAPH_SIZE 16
#define DOS_HEADER_PAGE_SIZE 512

struct DOS_RELOCATION {
	uint16_t offset; // 0x0
	uint16_t segment; // 0x2
	// 0x4
};

enum X86Bits : uint8_t {
	ThirtyTwoBitX86,
	SixtyFourBitX86,
	SixteenBitX86,
	Invalid,

	ENUM_MAX_VALUE_DECLARE(X86Bits)
};

template<auto value>
static inline constexpr auto constexpr_value() {
	return value;
}

enum PEType : uint8_t {
	PE32 = ThirtyTwoBitX86,
	PE64 = SixtyFourBitX86,
	PEROM = X86Bits::Invalid
};

struct COFFSection {
	void* data; // 0x0
	uint32_t size; // 0x8
	// 0xC
};
struct DataDirectory {
	void* data; // 0x0
	uint32_t size; // 0x8
	uint8_t type; // 0xC
	// 0xD
};
struct SectionEntry {
	void* raw; // 0x0
	void* data; // 0x8
	unsigned char (*name)[8]; // 0x10
	uint32_t size_after_loading; // 0x18
	uint32_t offset_after_loading; // 0x1C
	uint32_t size_in_file; // 0x20
	uint32_t offset_in_file; // 0x24
	union {
		struct SECTION_FLAGS {
			uint32_t type_dsect : 1;					// 0x00000001
			//uint32_t tls_scaled : 1;
			uint32_t type_noload : 1;					// 0x00000002
			uint32_t type_group : 1;					// 0x00000004 Apparently related to 16-bit stuff?
			uint32_t link_no_padding : 1;				// 0x00000008
			uint32_t type_copy : 1;						// 0x00000010 Meaning unknown
			uint32_t contains_code : 1;					// 0x00000020
			uint32_t contains_initialized_data : 1;		// 0x00000040
			uint32_t contains_uninitialized_data : 1;	// 0x00000080
			uint32_t link_other : 1;					// 0x00000100 Meaning unknown
			uint32_t link_info : 1;						// 0x00000200
			uint32_t type_over : 1;						// 0x00000400 Related to DOS-like overlays
			uint32_t link_remove : 1;					// 0x00000800
			uint32_t link_comdat : 1;					// 0x00001000
			uint32_t __unknown_flag_A : 1;				// 0x00002000 LOOK THIS UP
			// uint32_t mem_protected : 1;
			uint32_t no_defer_spec_exec : 1;			// 0x00004000
			uint32_t mem_fardata : 1;					// 0x00008000
			uint32_t mem_sysheap : 1;					// 0x00010000
			uint32_t mem_purgeable : 1;					// 0x00020000
			//uint32_t mem_16_bit : 1;
			uint32_t mem_locked : 1;					// 0x00040000
			uint32_t mem_preload : 1;					// 0x00080000
			uint32_t alignment : 4;						// 0x00F00000
			uint32_t link_extended_relocations : 1;		// 0x01000000
			uint32_t mem_discardable : 1;				// 0x02000000
			uint32_t mem_not_cached : 1;				// 0x04000000
			uint32_t mem_not_paged : 1;					// 0x08000000
			uint32_t mem_shared : 1;					// 0x10000000
			uint32_t execute : 1;						// 0x20000000
			uint32_t read : 1;							// 0x40000000
			uint32_t write : 1;							// 0x80000000
		} flags;
		UByteIntType<sizeof(SECTION_FLAGS)> raw_flags; // 0x28
	};
	uint16_t number_of_relocations; // 0x2C
	uint16_t number_of_line_numbers; // 0x2E
	union {
		struct SECTION_METADATA {
			bool misaligned_size_in_file : 1; // 0x01
			bool misaligned_offset_in_file : 1; // 0x02
		} metadata;
		UByteIntType<sizeof(SECTION_METADATA)> raw_metadata; // 0x30
	};
	// 0x31
};
//ValidateStructSize()
struct RichHeaderEntry {
	uint16_t build;
	uint16_t type;
	uint32_t count;
};

// Rich header crap based on this: https://github.com/dishather/richprint

struct ExecutableFile {
	union {
		//uint8_t flags; // 0x0
		struct EXE_FLAGS {
			X86Bits bit_count : constexpr_value<std::bit_width<size_t>(ENUM_MAX_VALUE(X86Bits))>();
			bool has_rich_header : 1;
		} flags;
		UByteIntType<sizeof(EXE_FLAGS)> raw_flags;
	};
	// 0x1
	uint16_t optional_header_size; // 0x2
	union {
		uint16_t file_header_flags; // 0x4
		struct {
			uint16_t relocs_stripped : 1;				// 0x0001
			uint16_t executable_image : 1;				// 0x0002
			uint16_t coff_line_numbers_stripped : 1;	// 0x0004
			uint16_t coff_local_symbols_stripped : 1;	// 0x0008
			uint16_t aggressively_trim_ws : 1;			// 0x0010
			uint16_t large_address_aware : 1;			// 0x0020
			uint16_t machine_is_16_bit : 1;				// 0x0040 LOOK THIS UP
			uint16_t little_endian : 1;					// 0x0080
			uint16_t machine_is_32_bit : 1;				// 0x0100 What does this do?
			uint16_t debug_info_stripped : 1;			// 0x0200
			uint16_t removable_run_from_swap : 1;		// 0x0400
			uint16_t network_run_from_swap : 1;			// 0x0800
			uint16_t system_file : 1;					// 0x1000
			uint16_t dll_file : 1;						// 0x2000
			uint16_t prefer_single_core : 1;			// 0x4000
			uint16_t big_endian : 1;					// 0x8000
		};
	};
	union {
		uint16_t optional_header_flags; // 0x6
		struct {
			// Apparently these are set by the borland linker?
			uint16_t library_process_init : 1;			// 0x0001
			uint16_t library_process_term : 1;			// 0x0002
			uint16_t library_thread_init : 1;			// 0x0004
			uint16_t library_thread_term : 1;			// 0x0008
			// Normal flags
			uint16_t __unknown_flag_B : 1;				// 0x0010
			uint16_t high_entropy_address_space : 1;	// 0x0020
			uint16_t code_integrity : 1;				// 0x0040
			uint16_t relocatable : 1;					// 0x0080
			uint16_t nx_compatible : 1;					// 0x0100
			uint16_t no_isolation : 1;					// 0x0200
			uint16_t no_seh : 1;						// 0x0400
			uint16_t no_bind : 1;						// 0x0800
			uint16_t require_appcontainer : 1;			// 0x1000
			uint16_t wdm_driver : 1;					// 0x2000
			uint16_t control_flow_guard : 1;			// 0x4000
			uint16_t terminal_server_aware : 1;			// 0x8000
		};
	};
	uint16_t section_count; // 0x8
	union {
		uint16_t present_directories; // 0xA
		struct {
			uint16_t export_directory : 1;
			uint16_t import_directory : 1;
			uint16_t resource_directory : 1;
			uint16_t exception_directory : 1;
			uint16_t security_directory : 1;
			uint16_t relocation_table : 1;
			uint16_t debug_directory : 1;
			uint16_t architecture_specific : 1;
			uint16_t global_ptr : 1; // Apparently MIPS related
			uint16_t tls : 1;
			uint16_t load_config_directory : 1;
			uint16_t bound_import_directory : 1;
			uint16_t import_address_table : 1;
			uint16_t delay_load_imports : 1;
			uint16_t com_descriptor : 1;
			uint16_t __unknown_directory_A : 1;
		};
	};
	uint32_t data_directory_count; // 0xC
	SectionEntry* sections; // 0x10
	DataDirectory* data_directories; // 0x18
	uint32_t dos_stub_offset; // 0x20
	uint32_t dos_stub_size; // 0x24
	uint32_t rich_header_key; // 0x28
	union {
		uint32_t loader_flags; // 0x2C
		struct {
			uint32_t com_plus_image : 1;
			uint32_t : 23;
			uint32_t system_global : 1; // Global subsections apply across TS sessions.
			uint32_t : 7;
		};
	};
	size_t rich_header_size; // 0x30
	RichHeaderEntry* rich_header_data; // 0x38
	// 0x40
};

//static constexpr auto wkejrbkwjrb = __builtin_offsetof(ExecutableFile, section_count);

dllexport ExecutableFile exe_data;

static constexpr const char* error_string_table[] = {
	"Unsupported executable type!",
	"Unrecognized executable type!",
	"Invalid NT header offset!",
	"Unsupported NT signature!",
	"Unrecognized NT signature!",
	"Unsupported machine type!",
	"Machine type does not match pe type!",
	"Unrecognized pe type!",
	"DOS files not supported!"
};
enum ERROR_INDICES : size_t {
	UNSUPPORTED_EXECUTABLE_TYPE,
	UNRECOGNIZED_EXECUTABLE_TYPE,
	INVALID_NT_HEADER_OFFSET,
	UNSUPPORTED_NT_SIGNATURE,
	UNRECOGNIZED_NT_SIGNATURE,
	UNSUPPORTED_MACHINE_TYPE,
	MACHINE_TYPE_DOES_NOT_MATCH_PE_TYPE,
	UNRECOGNIZED_PE_TYPE,
	DOS_NOT_SUPPORTED,

	ENUM_MAX_VALUE_DECLARE(ERROR_INDICES)
};

#define NORETURN_ERRORS 1

#if NORETURN_ERRORS
[[noreturn]] static inline void print_error(size_t index) {
	puts(error_string_table[index]);
	exit(EXIT_FAILURE);
}
#define MakeErrorFunc(index) \
static gnu_noinline void MACRO_CAT(goto_error_,index)() { \
	print_error(index); \
}
#else
static inline void print_error(size_t index) {
	puts(error_string_table[index]);
}
#define MakeErrorFunc(index) \
static gnu_noinline void MACRO_CAT(goto_error_,index)(const char* = GARBAGE_ARG(const char*)) { \
	print_error(index); \
}
MakeErrorFunc(UNSUPPORTED_EXECUTABLE_TYPE)
MakeErrorFunc(UNRECOGNIZED_EXECUTABLE_TYPE)
MakeErrorFunc(INVALID_NT_HEADER_OFFSET)
MakeErrorFunc(UNSUPPORTED_NT_SIGNATURE)
MakeErrorFunc(UNRECOGNIZED_NT_SIGNATURE)
MakeErrorFunc(UNSUPPORTED_MACHINE_TYPE)
MakeErrorFunc(MACHINE_TYPE_DOES_NOT_MATCH_PE_TYPE)
MakeErrorFunc(UNRECOGNIZED_PE_TYPE)
MakeErrorFunc(DOS_NOT_SUPPORTED)
#endif

using error_index_t = UBitIntType<std::bit_width<size_t>(ENUM_MAX_VALUE(ERROR_INDICES))>;



static inline constexpr size_t kwjebrkwjerb32 = __builtin_offsetof(IMAGE_NT_HEADERS32, OptionalHeader.Magic);
static inline constexpr size_t kwjebrkwjerb64 = __builtin_offsetof(IMAGE_NT_HEADERS64, OptionalHeader.Magic);
static inline constexpr size_t kwjebrkwjerb564 = __builtin_offsetof(IMAGE_NT_HEADERS64, OptionalHeader.AddressOfEntryPoint);

static inline constexpr size_t kwjebrkwje = __builtin_offsetof(IMAGE_DOS_HEADER, e_lfanew);

template<typename T1, typename T2>
static forceinline T1 fast_max(T1 value, T2 max) {
	if (value > (T1)max) {
		value = (T1)max;
	}
	assume(value <= (T1)max);
	return value;
}
template<typename T1, typename T2>
static forceinline T1 fast_min(T2 min, T1 value) {
	if (value < (T1)min) {
		value = (T1)min;
	}
	assume(value >= (T1)min);
	return value;
}
template<typename T1, typename T2>
static forceinline T1 fast_clamp(const T2& min_value, T1 value, const T2& max_value) {
	T1 ret = value;
	if (ret > (T1)max_value) {
		ret = (T1)max_value;
	}
	if (ret < (T1)min_value) {
		ret = (T1)min_value;
	}
	assume(ret >= (T1)min_value && ret <= (T1)max_value);
	return ret;
}

static constexpr auto kwjbrkjwbr = __builtin_offsetof(IMAGE_NT_HEADERS64, OptionalHeader.Subsystem);

// Defining a ".mixcrt" will make WoW64 treat a program as if it were for the posix subsystem?
// WoW64 has an entire logging subsystem that could be hooked...?

//#define DIRECT_ERROR_STRINGS 1
#define DIRECT_ERROR_CALLS 1

gnu_noinline void open_exe(const char* path) {

#if DIRECT_ERROR_CALLS
#if NORETURN_ERRORS
#define GOTO_ERROR(index) print_error(index);
#else
#define GOTO_ERROR(index) [[clang::musttail]] return MACRO_CAT(goto_error_,index)()
#endif
#elif DIRECT_ERROR_STRINGS
#define GOTO_ERROR(index) if (error_string = error_string_table[index]; 1) goto ErrorLabel; else
#else
#define GOTO_ERROR(index) if (error_index = (index); 1) goto ErrorLabel; else
#endif

	const char* error_string;
	error_index_t error_index;
	union {
		struct PARSE_FLAGS {
			uint8_t is_32_bit : 1;
			uint8_t no_rich_header : 1;
			uint8_t found_rich_header : 1;
		} flags;
		UByteIntType<sizeof(PARSE_FLAGS)> raw_flags;
	} parse_data;
	//parse_data.raw_flags = 0;

	if (auto* file = read_file_to_buffer(path)) {
		IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)file;
		switch (dos_header->e_magic) {
			case IMAGE_OS2_SIGNATURE: case IMAGE_VXD_SIGNATURE:
				GOTO_ERROR(UNSUPPORTED_EXECUTABLE_TYPE);
			default:
				GOTO_ERROR(UNRECOGNIZED_EXECUTABLE_TYPE);
			case IMAGE_DOS_SIGNATURE:

				uint16_t dos_pages = dos_header->e_cp;
				uint16_t dos_last_bytes_of_page = dos_header->e_cblp;
				uint32_t dos_file_size = (dos_pages - (bool)dos_last_bytes_of_page) * DOS_HEADER_PAGE_SIZE + dos_last_bytes_of_page;
				uint32_t dos_header_size = dos_header->e_cparhdr * PARAGRAPH_SIZE; // Apparently this is supposed to be even?
				exe_data.dos_stub_offset = dos_header_size;
				exe_data.dos_stub_size = dos_file_size - dos_header_size;

				uint32_t dos_relocations_offset = dos_header->e_lfarlc;
				uint32_t dos_relocations_size = sizeof(DOS_RELOCATION[dos_header->e_crlc]);

				int32_t nt_header_offset_raw = dos_header->e_lfanew;
				switch (nt_header_offset_raw) {
					case INT32_MIN ... -1:
						GOTO_ERROR(INVALID_NT_HEADER_OFFSET);
					case 0:
						GOTO_ERROR(DOS_NOT_SUPPORTED); // yet
				}
				uint32_t nt_header_offset = nt_header_offset_raw; // VALIDATE WHETHER THERE ARE ALIGNMENT REQUIREMENTS

				IMAGE_NT_HEADERS* nt_header = based_pointer<IMAGE_NT_HEADERS>(file, nt_header_offset);

				// This doesn't *need* to be aligned, but MSVC will
				// only output it that way and idgaf otherwise
				uint32_t* rich_header_scan = (uint32_t*)AlignPtrDownToMultipleOf2(nt_header, sizeof(uint32_t));
				
				parse_data.flags.found_rich_header = false;
				parse_data.flags.no_rich_header = dos_file_size == nt_header_offset || rich_header_scan <= (uint32_t*)&dos_header->e_lfanew;
				if (!parse_data.flags.no_rich_header) {
					while (--rich_header_scan > (uint32_t*)&dos_header->e_lfanew) {
						if (*rich_header_scan == PackUInt('R', 'i', 'c', 'h')) {
							exe_data.flags.has_rich_header = parse_data.flags.found_rich_header = true;
							uint32_t xor_key = exe_data.rich_header_key = rich_header_scan[1];
							// Why does XOR decryption look so *nasty* to do efficiently?
							vec<uint32_t, 4> wide_xor_key = { xor_key, xor_key, xor_key, xor_key };
							vec<uint32_t, 4> wide_rich_end = { PackUInt('D', 'a', 'n', 'S') , 0, 0, 0 };
							uint32_t* rich_header_data_end = rich_header_scan;
							rich_header_scan -= 2;
							for (
								vec<uint32_t, 4> temp;
								(temp = (*(unaligned vec<uint32_t, 4>*)(rich_header_scan -= 2) ^ wide_xor_key) != wide_rich_end), temp[0] && temp[1] && temp[2] && temp[3];
							);
							rich_header_scan += 4;
							size_t rich_header_size = exe_data.rich_header_size = sizeof(uint32_t[rich_header_data_end - rich_header_scan]);
							uint32_t* rich_header_data = malloc_array<uint32_t>(rich_header_size);
							exe_data.rich_header_data = (RichHeaderEntry*)rich_header_data;
							novectorize while (rich_header_scan < rich_header_data_end) {
								*rich_header_data++ = *rich_header_scan++ ^ xor_key;
							}
							break;
						}
					}
				}
				
				auto full_nt_header_signature = nt_header->Signature;
				switch ((uint16_t)full_nt_header_signature) { // Truncate to 16 bytes to properly detect non-PE values...
					case IMAGE_OS2_SIGNATURE: // NE
					case IMAGE_VXD_SIGNATURE: // LE
						// Supposedly LX might also be a thing?
						GOTO_ERROR(UNSUPPORTED_NT_SIGNATURE);
					case IMAGE_NT_SIGNATURE: // PE
						if (full_nt_header_signature != IMAGE_NT_SIGNATURE) { // ...then recheck the value actually has two 0 bytes when it's inevitably PE
							// PEED is apparently also a thing? WTF is that
					default:
							GOTO_ERROR(UNRECOGNIZED_NT_SIGNATURE);
						}
						X86Bits exe_bit_depth;
						switch (nt_header->FileHeader.Machine) {
							default:
								GOTO_ERROR(UNSUPPORTED_MACHINE_TYPE);
							case IMAGE_FILE_MACHINE_I386:
								exe_bit_depth = ThirtyTwoBitX86;
								parse_data.flags.is_32_bit = true;
								break;
							case IMAGE_FILE_MACHINE_AMD64:
								exe_bit_depth = SixtyFourBitX86;
								parse_data.flags.is_32_bit = false;
								break;
						}
						PEType pe_type;
						switch (nt_header->OptionalHeader.Magic) {
							default:
								GOTO_ERROR(UNRECOGNIZED_PE_TYPE);
							case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
								pe_type = PE32;
								break;
							case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
								pe_type = PE64;
								break;
							case IMAGE_ROM_OPTIONAL_HDR_MAGIC:
								pe_type = PEROM;
								break;
						}
						if (exe_bit_depth != pe_type) GOTO_ERROR(MACHINE_TYPE_DOES_NOT_MATCH_PE_TYPE);
						exe_data.flags.bit_count = exe_bit_depth;

#define READ_OPTIONAL_HEADER_FIELD(field) (parse_data.flags.is_32_bit ? ((IMAGE_NT_HEADERS32*)nt_header)->OptionalHeader.field : ((IMAGE_NT_HEADERS64*)nt_header)->OptionalHeader.field)

						IMAGE_DATA_DIRECTORY* raw_data_directories = READ_OPTIONAL_HEADER_FIELD(DataDirectory);
						uint16_t directory_mask = 0;
						auto data_directory_count = fast_clamp(0, READ_OPTIONAL_HEADER_FIELD(NumberOfRvaAndSizes), IMAGE_NUMBEROF_DIRECTORY_ENTRIES);
						for (size_t i = 0; i < data_directory_count; ++i) {
							bool has_directory = *(uint64_t*)&raw_data_directories[i];
							directory_mask |= has_directory << i;
						}
						exe_data.present_directories = directory_mask;
						size_t allocated_data_directories = exe_data.data_directory_count = std::popcount(directory_mask);
						DataDirectory* data_directories = exe_data.data_directories = malloc_array<DataDirectory>(allocated_data_directories);
						size_t parsed_directories = 0;
						for (size_t i = 0; i < data_directory_count; ++i) {
							if (directory_mask & 1 << i) {
								DataDirectory& current_directory = data_directories[parsed_directories++];
								current_directory.data = based_pointer(file, raw_data_directories[i].VirtualAddress);
								current_directory.size = raw_data_directories[i].Size;
								current_directory.type = i;
							}
						}
						
						size_t section_count = exe_data.section_count = nt_header->FileHeader.NumberOfSections;
						//exe_data.sections = (COFFSmalloc(sizeof(COFFSection[section_count]));
						SectionEntry* sections = exe_data.sections = malloc_array<SectionEntry>(section_count);
						IMAGE_SECTION_HEADER* raw_sections = based_pointer<IMAGE_SECTION_HEADER>(&nt_header->OptionalHeader, nt_header->FileHeader.SizeOfOptionalHeader);
						uint32_t file_alignment = READ_OPTIONAL_HEADER_FIELD(FileAlignment);
						if (!std::has_single_bit(file_alignment)) {
							file_alignment = 1;
						}
						assume(std::has_single_bit(file_alignment));
						uint32_t file_alignment_mask = tzmskz(file_alignment);
						for (size_t i = 0; i < section_count; ++i) {
							IMAGE_SECTION_HEADER& current_raw_section = raw_sections[i];
							SectionEntry& current_section = sections[i];
							current_section.raw = &current_raw_section;
							current_section.name = &current_raw_section.Name;
							current_section.size_after_loading = current_raw_section.Misc.VirtualSize;
							current_section.offset_after_loading = current_raw_section.VirtualAddress;
							current_section.raw_flags = current_raw_section.Characteristics;
							uint32_t offset_in_file = current_section.offset_in_file = current_raw_section.PointerToRawData;
							current_section.data = based_pointer(file, offset_in_file);
							current_section.metadata.misaligned_offset_in_file = offset_in_file & file_alignment_mask;
							uint32_t size_in_file = current_section.size_in_file = current_raw_section.SizeOfRawData;
							current_section.metadata.misaligned_size_in_file = size_in_file & file_alignment_mask;
						}
				};
		}
	}
	return;
ErrorLabel:
#if !DIRECT_ERROR_STRINGS
	error_string = error_string_table[error_index];
#endif
	puts(error_string);
	//print_error(error_index);
	return;
}

void open_exe_wrapper(const char* path) {
	return open_exe(path);
}
