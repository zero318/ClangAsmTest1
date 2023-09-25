#ifndef WINDOWS_STRUCTS_H
#define WINDOWS_STRUCTS_H
#pragma once

#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

#define PARAGRAPH_SIZE 16
#define PAGE_SIZE 0x1000

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>
#include <winternl.h>


#include "zero/util.h"
#include "zero/custom_intrin.h"

#define __x86_padding(bytes)
#define __x64_padding(bytes)
#define __padding(bytes)

#if __INTELLISENSE__
#define ide_packed_field
#define ide_gnu_packed
#else
#define ide_packed_field packed_field
#define ide_gnu_packed gnu_packed
#endif

#define DUMMY_X_TYPE(type) \
template<size_t bits = native_bits> \
using MACRO_CAT(type,X) = type

#if NATIVE_BITS == 32
#define NTAPI stdcall
#else
#define NTAPI
#endif

#define MakeWinVer(major, minor, build, revision) (UINT64_C(major) * 1000000000000ull + UINT64_C(minor) * 10000000000ull + UINT64_C(build) * 100000ull + UINT64_C(revision))

enum WindowsVersion : uint64_t {
	Win_NT_310		= MakeWinVer(3, 10, 511, 1),
	Win_NT_310_SP3	= MakeWinVer(3, 10, 528, 1),
	Win_NT_350		= MakeWinVer(3, 50, 800, 1),
	Win_NT_350_SP3	= MakeWinVer(3, 50, 807, 1),
	Win_NT_351		= MakeWinVer(3, 51, 1025, 1),
	Win_NT_351_SP3	= MakeWinVer(3, 51, 1057, 3),
	Win_NT_351_SP4	= MakeWinVer(3, 51, 1057, 5),
	Win_NT_351_SP5	= MakeWinVer(3, 51, 1057, 6),
	Win_NT_40		= MakeWinVer(4, 0, 1381, 1),
	Win_NT_40_SP2	= MakeWinVer(4, 0, 1381, 3),
	Win_NT_40_SP3	= MakeWinVer(4, 0, 1381, 4),
	Win_NT_40_SP4	= MakeWinVer(4, 0, 1381, 133),
	Win_NT_40_SP5	= MakeWinVer(4, 0, 1381, 204),
	Win_NT_40_SP6	= MakeWinVer(4, 0, 1381, 335),
	Win_2000		= MakeWinVer(5, 0, 2195, 1),
	Win_2000_SP1	= MakeWinVer(5, 0, 2195, 1620),
	Win_2000_SP3	= MakeWinVer(5, 0, 2195, 5438),
	Win_2000_SP4	= MakeWinVer(5, 0, 2195, 6717),
	Win_XP			= MakeWinVer(5, 1, 2600, 0),
	Win_XP_SP1		= MakeWinVer(5, 1, 2600, 1106),
	Win_XP_SP2		= MakeWinVer(5, 1, 2600, 2180),
	Win_XP_SP3		= MakeWinVer(5, 1, 2600, 5512),
	Win_Srv2003		= MakeWinVer(5, 2, 3790, 0),
	Win_Srv2003_SP1	= MakeWinVer(5, 2, 3790, 1830),
	Win_Srv2003_SP2	= MakeWinVer(5, 2, 3790, 3959),
	Win_Vista		= MakeWinVer(6, 0, 6000, 16386),
	Win_Vista_SP1	= MakeWinVer(6, 0, 6001, 18000),
	Win_Vista_SP2	= MakeWinVer(6, 0, 6002, 18005),
	Win_7			= MakeWinVer(6, 1, 7600, 16385),
	Win_7_SP1		= MakeWinVer(6, 1, 7601, 17514),
	Win_8			= MakeWinVer(6, 2, 9200, 16384),
	Win_81			= MakeWinVer(6, 2, 9600, 16384),
	Win_81_SP1		= MakeWinVer(6, 2, 9600, 17031),
	Win_10			= MakeWinVer(10, 0, 10240, 16384),
	Win_10_1507		= Win_10,
	Win_10_1511		= MakeWinVer(10, 0, 10586, 0),
	Win_10_1607		= MakeWinVer(10, 0, 14393, 0),
	Win_10_1703		= MakeWinVer(10, 0, 15063, 0),
	Win_10_1709		= MakeWinVer(10, 0, 16299, 15),
	Win_10_1803		= MakeWinVer(10, 0, 17134, 1),
	Win_10_1809		= MakeWinVer(10, 0, 17763, 107),
	Win_10_1903		= MakeWinVer(10, 0, 18362, 1),
	Win_10_1909		= MakeWinVer(10, 0, 18362, 418),
	Win_10_2004		= MakeWinVer(10, 0, 19041, 208),
	Win_10_20H2		= MakeWinVer(10, 0, 19042, 0),
	Win_10_21H1		= MakeWinVer(10, 0, 19043, 0),
	Win_10_21H2		= MakeWinVer(10, 0, 19044, 0),
	Win_10_22H2		= MakeWinVer(10, 0, 19045, 0),
	Win_11			= MakeWinVer(10, 0, 22000, 0),
	Win_11_21H2		= Win_11,
	Win_11_22H2		= MakeWinVer(10, 0, 22621, 1848)
};

#undef MakeWinVer

using UNKNOWN_TYPE = void;
using ANY_TYPE = void;

//__if_not_exists(KPRIORITY) {
	//using KPRIORITY = int32_t;
//}

template<size_t bits = native_bits>
using KAFFINITYX = size_tx<bits>;
ValidateStructSize(0x4, KAFFINITYX<32>);
ValidateStructAlignment(0x4, KAFFINITYX<32>);
ValidateStructSize(0x8, KAFFINITYX<64>);
ValidateStructAlignment(0x8, KAFFINITYX<64>);

template<size_t bits = native_bits>
using HANDLEX = PTRZX<bits>;
ValidateStructSize(0x4, HANDLEX<32>);
ValidateStructAlignment(0x4, HANDLEX<32>);
ValidateStructSize(0x8, HANDLEX<64>);
ValidateStructAlignment(0x8, HANDLEX<64>);

struct LongDouble80_base {
	uint16_t raw[5];

	inline constexpr LongDouble80_base& operator=(long double value) {
		*(long double*)this = value;
		return *this;
	}

	inline constexpr operator long double() const {
		return *(long double*)this;
	}
};
struct PackedLongDouble80 : LongDouble80_base {};
struct alignas(16) AlignedLongDouble80 : LongDouble80_base {};
struct PaddedLongDouble80 : LongDouble80_base {
	uint16_t padding[3];
};

using __m128_unaligned gnu_aligned(4) = __m128;
ValidateStructAlignment(0x4, __m128_unaligned);

template<size_t bits = native_bits>
struct winstruct_alignment_impl;

template<>
struct winstruct_alignment_impl<32> {
	using int64_tx gnu_aligned(alignof(int32_t)) = int64_t;
	using uint64_tx gnu_aligned(alignof(uint32_t)) = uint64_t;
	struct LongDouble80 : LongDouble80_base {};
};
template<>
struct winstruct_alignment_impl<64> {
	using int64_tx gnu_aligned(alignof(int64_t)) = int64_t;
	using uint64_tx gnu_aligned(alignof(uint64_t)) = int64_t;
	struct alignas(16) LongDouble80 : LongDouble80_base {};
};

template<size_t bits = native_bits>
using int64_tx = winstruct_alignment_impl<bits>::int64_tx;
ValidateStructSize(0x8, int64_tx<32>);
ValidateStructAlignment(0x4, int64_tx<32>);
ValidateStructSize(0x8, int64_tx<64>);
ValidateStructAlignment(0x8, int64_tx<64>);


template<size_t bits = native_bits>
using uint64_tx = winstruct_alignment_impl<bits>::uint64_tx;
ValidateStructSize(0x8, uint64_tx<32>);
ValidateStructAlignment(0x4, uint64_tx<32>);
ValidateStructSize(0x8, uint64_tx<64>);
ValidateStructAlignment(0x8, uint64_tx<64>);

template<size_t bits = native_bits>
union LARGE_INTEGERX {
	struct {
		uint32_t LowPart; // 0x0, 0x0
		int32_t HighPart; // 0x4, 0x4
	};
	struct {
		uint32_t LowPart; // 0x0, 0x0
		int32_t HighPart; // 0x4, 0x4
	} u;
	int64_tx<bits> QuadPart ide_packed_field; // 0x0, 0x0
	// 0x8, 0x8
};
ValidateStructSize(0x8, LARGE_INTEGERX<32>);
ValidateStructAlignment(0x4, LARGE_INTEGERX<32>);
ValidateStructSize(0x8, LARGE_INTEGERX<64>);
ValidateStructAlignment(0x8, LARGE_INTEGERX<64>);

template<size_t bits = native_bits>
union ULARGE_INTEGERX {
	struct {
		uint32_t LowPart; // 0x0, 0x0
		uint32_t HighPart; // 0x4, 0x4
	};
	struct {
		uint32_t LowPart; // 0x0, 0x0
		uint32_t HighPart; // 0x4, 0x4
	} u;
	uint64_tx<bits> QuadPart ide_packed_field; // 0x0, 0x0
	// 0x8, 0x8
};
ValidateStructSize(0x8, ULARGE_INTEGERX<32>);
ValidateStructAlignment(0x4, ULARGE_INTEGERX<32>);
ValidateStructSize(0x8, ULARGE_INTEGERX<64>);
ValidateStructAlignment(0x8, ULARGE_INTEGERX<64>);

template<size_t bits = native_bits>
struct KSYSTEM_TIMEX {
	union {
		struct {
			uint32_t LowPart; // 0x0, 0x0
			int32_t High1Time; // 0x4, 0x4
		};
		int64_tx<32> QuadPart ide_packed_field; // 0x0, 0x0
		_Atomic int64_tx<32> AtomicQuadPart ide_packed_field; // 0x0, 0x0
	};
	int32_t High2Time; // 0x8, 0x8
	// 0xC, 0xC

private:
	template<typename T>
	static inline int64_t read_impl(T self) {
		if constexpr (bits == 64) {
			return self->AtomicQuadPart;
		} else {
			LARGE_INTEGERX<32> ret;
			do {
				ret.QuadPart = this->QuadPart;
			} while (ret.HighPart == this->High2Time);
			return ret.QuadPart;
		}
	}
public:

	inline int64_t read() const {
		return read_impl(this);
	}

	inline int64_t read() const volatile {
		return read_impl(this);
	}
};
ValidateStructSize(0xC, KSYSTEM_TIMEX<32>);
ValidateStructAlignment(0x4, KSYSTEM_TIMEX<32>);
ValidateStructSize(0xC, KSYSTEM_TIMEX<64>);
ValidateStructAlignment(0x4, KSYSTEM_TIMEX<64>);

#if !__INTELLISENSE__
//template<size_t bits = native_bits>
//using LongDouble80 gnu_aligned(bits != 64 ? 2 : 16) = long double;
template<size_t bits = native_bits>
using LongDouble80 = winstruct_alignment_impl<bits>::LongDouble80;
using AlignedLongDouble = AlignedLongDouble80;
using PackedLongDouble = PackedLongDouble80;
using PaddedLongDouble = PaddedLongDouble80;
#else
template<size_t bits>
union LongDouble80;
template<>
union gnu_packed gnu_aligned(2) LongDouble80<32> {
	long double value;
	uint8_t dummy[10];
};
template<>
union gnu_aligned(16) LongDouble80<64> {
	long double value;
};
union gnu_aligned(16) AlignedLongDouble {
	long double value;
};
union gnu_packed gnu_aligned(2) PackedLongDouble {
	long double value;
	uint8_t dummy[10];
};
using PaddedLongDouble = PaddedLongDouble80;
#endif
ValidateStructSize(0xA, LongDouble80<32>);
ValidateStructAlignment(0x2, LongDouble80<32>);
ValidateStructSize(0x10, LongDouble80<64>);
ValidateStructAlignment(0x10, LongDouble80<64>);
//ValidateStructSize(0x10, AlignedLongDouble);
ValidateStructAlignment(0x10, AlignedLongDouble);
ValidateStructSize(0xA, PackedLongDouble);
ValidateStructAlignment(0x2, PackedLongDouble);

template<size_t bits = native_bits>
struct STRINGX {
	uint16_t Length; // 0x0, 0x0
	uint16_t MaximumLength; // 0x2, 0x2
	__x64_padding(0x4);
	PTRZX<bits, char> Buffer; // 0x4, 0x8
	// 0x8, 0x10
};
ValidateStructSize(0x8, STRINGX<32>);
ValidateStructAlignment(0x4, STRINGX<32>);
ValidateStructSize(0x10, STRINGX<64>);
ValidateStructAlignment(0x8, STRINGX<64>);

template<size_t bits = native_bits>
struct UNICODE_STRINGX {
	uint16_t Length; // 0x0, 0x0
	uint16_t MaximumLength; // 0x2, 0x2
	__x64_padding(0x4);
	PTRZX<bits, wchar_t> Buffer; // 0x4, 0x8
	// 0x8, 0x10
};
ValidateStructSize(0x8, UNICODE_STRINGX<32>);
ValidateStructAlignment(0x4, UNICODE_STRINGX<32>);
ValidateStructSize(0x10, UNICODE_STRINGX<64>);
ValidateStructAlignment(0x8, UNICODE_STRINGX<64>);

template<size_t bits = native_bits, typename T = void>
struct LIST_ENTRYX {
	PTRZX<bits, T> Flink; // 0x0, 0x0
	PTRZX<bits, T> Blink; // 0x4, 0x8
	// 0x8, 0x10
};
ValidateStructSize(0x8, LIST_ENTRYX<32>);
ValidateStructAlignment(0x4, LIST_ENTRYX<32>);
ValidateStructSize(0x10, LIST_ENTRYX<64>);
ValidateStructAlignment(0x8, LIST_ENTRYX<64>);

template<size_t bits = native_bits>
struct ASSEMBLY_STORAGE_MAP_ENTRYX {
	union {
		uint32_t Flags; // 0x0, 0x0
		struct {
		};
	};
	__x64_padding(0x4);
	UNICODE_STRINGX<bits> DosPath; // 0x4, 0x8
	HANDLEX<bits> Handle; // 0xC, 0x18
	// 0x10, 0x20
};
ValidateStructSize(0x10, ASSEMBLY_STORAGE_MAP_ENTRYX<32>);
ValidateStructAlignment(0x4, ASSEMBLY_STORAGE_MAP_ENTRYX<32>);
ValidateStructSize(0x20, ASSEMBLY_STORAGE_MAP_ENTRYX<64>);
ValidateStructAlignment(0x8, ASSEMBLY_STORAGE_MAP_ENTRYX<64>);

template<size_t bits = native_bits>
struct ASSEMBLY_STORAGE_MAPX {
	union {
		uint32_t Flags; // 0x0, 0x0
		struct {
		};
	};
	uint32_t AssemblyCount; // 0x4, 0x4
	PTRZX<bits, ASSEMBLY_STORAGE_MAP_ENTRYX<bits>[]> AssemblyArray; // 0x8, 0x8
	// 0xC, 0x10
};
ValidateStructSize(0xC, ASSEMBLY_STORAGE_MAPX<32>);
ValidateStructAlignment(0x4, ASSEMBLY_STORAGE_MAPX<32>);
ValidateStructSize(0x10, ASSEMBLY_STORAGE_MAPX<64>);
ValidateStructAlignment(0x8, ASSEMBLY_STORAGE_MAPX<64>);

/*========================================
	ACTIVATION CONTEXTS
========================================*/
#pragma region // ACTIVATION CONTEXT definitions

struct ACTIVATION_CONTEXT_DATA {
	uint32_t Magic; // 0x0, 0x0
	uint32_t HeaderSize; // 0x4, 0x4
	uint32_t FormatVersion; // 0x8, 0x8
	uint32_t TotalSize; // 0xC, 0xC
	uint32_t DefaultTocOffset; // 0x10, 0x10
	uint32_t ExtendedTocOffset; // 0x14, 0x14
	uint32_t AssemblyRosterOffset; // 0x18, 0x18
	union {
		uint32_t Flags; // 0x1C, 0x1C
		struct {
		};
	};
	// 0x20, 0x20
};
ValidateStructSize(0x20, ACTIVATION_CONTEXT_DATA);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATAX = ACTIVATION_CONTEXT_DATA;

struct ACTIVATION_CONTEXT_DATA_TOC_HEADER {
	uint32_t HeaderSize; // 0x0, 0x0
	uint32_t EntryCount; // 0x4, 0x4
	uint32_t FirstEntryOffset; // 0x8, 0x8
	union {
		uint32_t Flags; // 0xC, 0xC
		struct {
		};
	};
	// 0x10, 0x10
};
ValidateStructSize(0x10, ACTIVATION_CONTEXT_DATA_TOC_HEADER);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_TOC_HEADER);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_TOC_HEADERX = ACTIVATION_CONTEXT_DATA_TOC_HEADER;

// No actual extensions?
struct ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADER {
	uint32_t HeaderSize; // 0x0, 0x0
	uint32_t EntryCount; // 0x4, 0x4
	uint32_t FirstEntryOffset; // 0x8, 0x8
	union {
		uint32_t Flags; // 0xC, 0xC
		struct {
		};
	};
	// 0x10, 0x10
};
ValidateStructSize(0x10, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADER);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADER);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADERX = ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADER;

struct ACTIVATION_CONTEXT_DATA_TOC_ENTRY {
	uint32_t Id; // 0x0, 0x0
	uint32_t Offset; // 0x4, 0x4
	uint32_t Length; // 0x8, 0x8
	uint32_t Format; // 0xC, 0xC
	// 0x10, 0x10
};
ValidateStructSize(0x10, ACTIVATION_CONTEXT_DATA_TOC_ENTRY);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_TOC_ENTRY);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_TOC_ENTRYX = ACTIVATION_CONTEXT_DATA_TOC_ENTRY;

struct ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRY {
	GUID ExtensionGuid; // 0x0, 0x0
	uint32_t TocOffset; // 0x10, 0x10
	uint32_t Length; // 0x14, 0x14
	// 0x18, 0x18
};
ValidateStructSize(0x18, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRY);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRY);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRYX = ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRY;

struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER {
	uint32_t HeaderSize; // 0x0, 0x0
	uint32_t HashAlgorithm; // 0x4, 0x4
	uint32_t EntryCount; // 0x8, 0x8
	uint32_t FirstEntryOffset; // 0xC, 0xC
	uint32_t AssemblyInformationSectionOffset; // 0x10, 0x10
	// 0x14, 0x14
};
ValidateStructSize(0x14, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADERX = ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER;

struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY {
	union {
		uint32_t Flags; // 0x0, 0x0
		struct {
		};
	};
	uint32_t PseudoKey; // 0x4, 0x4
	uint32_t AssemblyNameOffset; // 0x8, 0x8
	uint32_t AssemblyNameLength; // 0xC, 0xC
	uint32_t AssemblyInformationOffset; // 0x10, 0x10
	uint32_t AssemblyInformationLength; // 0x14, 0x14
	// 0x18, 0x18
};
ValidateStructSize(0x18, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRYX = ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY;

struct ACTIVATION_CONTEXT_STRING_SECTION_HEADER {
	uint32_t Magic; // 0x0, 0x0
	uint32_t HeaderSize; // 0x4, 0x4
	uint32_t FormatVersion; // 0x8, 0x8
	uint32_t DataFormatVersion; // 0xC, 0xC
	union {
		uint32_t Flags; // 0x10, 0x10
		struct {
		};
	};
	uint32_t ElementCount; // 0x14, 0x14
	uint32_t ElementListOffset; // 0x18, 0x18
	uint32_t HashAlgorithm; // 0x1C, 0x1C
	uint32_t SearchStructureOffset; // 0x20, 0x20
	uint32_t UserDataOffset; // 0x24, 0x24
	uint32_t UserDataSize; // 0x28, 0x28
	// 0x2C, 0x2C
};
ValidateStructSize(0x2C, ACTIVATION_CONTEXT_STRING_SECTION_HEADER);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_STRING_SECTION_HEADER);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_STRING_SECTION_HEADERX = ACTIVATION_CONTEXT_STRING_SECTION_HEADER;

struct ACTIVATION_CONTEXT_GUID_SECTION_HEADER {
	uint32_t Magic; // 0x0, 0x0
	uint32_t HeaderSize; // 0x4, 0x4
	uint32_t FormatVersion; // 0x8, 0x8
	uint32_t DataFormatVersion; // 0xC, 0xC
	union {
		uint32_t Flags; // 0x10, 0x10
		struct {
		};
	};
	uint32_t ElementCount; // 0x14, 0x14
	uint32_t ElementListOffset; // 0x18, 0x18
	uint32_t SearchStructureOffset; // 0x1C, 0x1C
	uint32_t UserDataOffset; // 0x20, 0x20
	uint32_t UserDataSize; // 0x24, 0x24
	// 0x28, 0x28
};
ValidateStructSize(0x28, ACTIVATION_CONTEXT_GUID_SECTION_HEADER);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_GUID_SECTION_HEADER);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_GUID_SECTION_HEADERX = ACTIVATION_CONTEXT_GUID_SECTION_HEADER;

struct ACTIVATION_CONTEXT_SECTION_HASH_TABLE {
	uint32_t BucketTableEntryCount; // 0x0, 0x0
	uint32_t BucketTableOffset; // 0x4, 0x4
	// 0x8, 0x8
};
ValidateStructSize(0x8, ACTIVATION_CONTEXT_SECTION_HASH_TABLE);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_SECTION_HASH_TABLE);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_SECTION_HASH_TABLEX = ACTIVATION_CONTEXT_SECTION_HASH_TABLE;

using ACTIVATION_CONTEXT_STRING_SECTION_HASH_TABLE = ACTIVATION_CONTEXT_SECTION_HASH_TABLE;
using ACTIVATION_CONTEXT_GUID_SECTION_HASH_TABLE = ACTIVATION_CONTEXT_SECTION_HASH_TABLE;

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_STRING_SECTION_HASH_TABLEX = ACTIVATION_CONTEXT_SECTION_HASH_TABLEX<bits>;
template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_GUID_SECTION_HASH_TABLEX = ACTIVATION_CONTEXT_SECTION_HASH_TABLEX<bits>;

struct ACTIVATION_CONTEXT_SECTION_HASH_BUCKET {
	uint32_t ChainCount; // 0x0, 0x0
	uint32_t ChainOffset; // 0x4, 0x4
	// 0x8, 0x8
};
ValidateStructSize(0x8, ACTIVATION_CONTEXT_SECTION_HASH_BUCKET);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_SECTION_HASH_BUCKET);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX = ACTIVATION_CONTEXT_SECTION_HASH_BUCKET;

using ACTIVATION_CONTEXT_STRING_SECTION_HASH_BUCKET = ACTIVATION_CONTEXT_SECTION_HASH_BUCKET;
using ACTIVATION_CONTEXT_GUID_SECTION_HASH_BUCKET = ACTIVATION_CONTEXT_SECTION_HASH_BUCKET;

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_STRING_SECTION_HASH_BUCKETX = ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX<bits>;
template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_GUID_SECTION_HASH_BUCKETX = ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX<bits>;

struct ACTIVATION_CONTEXT_STRING_SECTION_ENTRY {
	uint32_t PseudoKey; // 0x0, 0x0
	uint32_t KeyOffset; // 0x4, 0x4
	uint32_t KeyLength; // 0x8, 0x8
	uint32_t Offset; // 0xC, 0xC
	uint32_t Length; // 0x10, 0x10
	uint32_t AssemblyRosterIndex; // 0x14, 0x14
	// 0x18, 0x18
};
ValidateStructSize(0x18, ACTIVATION_CONTEXT_STRING_SECTION_ENTRY);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_STRING_SECTION_ENTRY);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_STRING_SECTION_ENTRYX = ACTIVATION_CONTEXT_STRING_SECTION_ENTRY;

struct ACTIVATION_CONTEXT_GUID_SECTION_ENTRY {
	GUID Guid; // 0x0, 0x0
	uint32_t Offset; // 0x10, 0x10
	uint32_t Length; // 0x14, 0x14
	uint32_t AssemblyRosterIndex; // 0x18, 0x18
	// 0x1C, 0x1C
};
ValidateStructSize(0x1C, ACTIVATION_CONTEXT_GUID_SECTION_ENTRY);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_GUID_SECTION_ENTRY);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_GUID_SECTION_ENTRYX = ACTIVATION_CONTEXT_GUID_SECTION_ENTRY;

struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	GUID PolicyCoherencyGuid; // 0x8, 0x8
	GUID PolicyOverrideGuid; // 0x18, 0x18
	uint32_t ApplicationDirectoryPathType; // 0x28, 0x28
	uint32_t ApplicationDirectoryLength; // 0x2C, 0x2C
	uint32_t ApplicationDirectoryOffset; // 0x30, 0x30
	// 0x34, 0x34
};
ValidateStructSize(0x34, ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATIONX = ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATION;

struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t EncodedAssemblyIdentityLength; // 0x8, 0x8
	uint32_t EncodedAssemblyIdentityOffset; // 0xC, 0xC
	uint32_t ManifestPathType; // 0x10, 0x10
	uint32_t ManifestPathLength; // 0x14, 0x14
	uint32_t ManifestPathOffset; // 0x18, 0x18
	LARGE_INTEGERX<32> ManifestLastWriteTime; // 0x1C, 0x1C
	uint32_t PolicyPathType; // 0x24, 0x24
	uint32_t PolicyPathLength; // 0x28, 0x28
	uint32_t PolicyPathOffset; // 0x2C, 0x2C
	LARGE_INTEGERX<32> PolicyLastWriteTime; // 0x30, 0x30
	uint32_t MetadataSatelliteRosterIndex; // 0x38, 0x38
	uint32_t Unused2; // 0x3C, 0x3C
	uint32_t ManifestVersionMajor; // 0x40, 0x40
	uint32_t ManifestVersionMinor; // 0x44, 0x44
	uint32_t PolicyVersionMajor; // 0x48, 0x48
	uint32_t PolicyVersionMinor; // 0x4C, 0x4C
	uint32_t AssemblyDirectoryNameLength; // 0x50, 0x50
	uint32_t AssemblyDirectoryNameOffset; // 0x54, 0x54
	uint32_t NumOfFilesInAssembly; // 0x58, 0x58
	uint32_t LanguageLength; // 0x5C, 0x5C
	uint32_t LanguageOffset; // 0x60, 0x60
	// 0x64, 0x64
};
ValidateStructSize(0x64, ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATIONX = ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION;

struct ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t TotalPathLength; // 0x8, 0x8
	uint32_t PathSegmentCount; // 0xC, 0xC
	uint32_t PathSegmentOffset; // 0x10, 0x10
	// 0x14, 0x14
};
ValidateStructSize(0x14, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_DLL_REDIRECTIONX = ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION;

struct ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT {
	uint32_t Length; // 0x0, 0x0
	uint32_t Offset; // 0x4, 0x4
	// 0x8, 0x8
};
ValidateStructSize(0x8, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENTX = ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT;

struct ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t VersionSpecificClassNameLength; // 0x8, 0x8
	uint32_t VersionSpecificClassNameOffset; // 0xC, 0xC
	uint32_t DllNameLength; // 0x10, 0x10
	uint32_t DllNameOffset; // 0x14, 0x14
	// 0x18, 0x18
};
ValidateStructSize(0x18, ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTIONX = ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION;

struct ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t ThreadingModel; // 0x8, 0x8
	GUID ReferenceClsid; // 0xC, 0xC
	GUID ConfiguredClsid; // 0x1C, 0x1C
	GUID ImplementedClsid; // 0x2C, 0x2C
	GUID TypeLibraryId; // 0x3C, 0x3C
	uint32_t ModuleLength; // 0x4C, 0x4C
	uint32_t ModuleOffset; // 0x50, 0x50
	uint32_t ProgIdLength; // 0x54, 0x54
	uint32_t ProgIdOffset; // 0x58, 0x58
	uint32_t ShimDataLength; // 0x5C, 0x5C
	uint32_t ShimDataOffset; // 0x60, 0x60
	// 0x64, 0x64
};
ValidateStructSize(0x64, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTIONX = ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION;

struct ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t Type; // 0x8, 0x8
	uint32_t ModuleLength; // 0xC, 0xC
	uint32_t ModuleOffset; // 0x10, 0x10
	uint32_t TypeLength; // 0x14, 0x14
	uint32_t TypeOffset; // 0x18, 0x18
	uint32_t ShimVersionLength; // 0x1C, 0x1C
	uint32_t ShimVersionOffset; // 0x20, 0x20
	uint32_t DataLength; // 0x24, 0x24
	uint32_t DataOffset; // 0x28, 0x28
	// 0x2C, 0x2C
};
ValidateStructSize(0x2C, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIMX = ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM;

struct ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	GUID ProxyStubClsid32; // 0x8, 0x8
	uint32_t NumMethods; // 0x18, 0x18
	GUID TypeLibraryId; // 0x1C, 0x1C
	GUID BaseInterface; // 0x2C, 0x2C
	uint32_t NameLength; // 0x3C, 0x3C
	uint32_t NameOffset; // 0x40, 0x40
	// 0x44, 0x44
};
ValidateStructSize(0x44, ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTIONX = ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION;

struct ACTIVATION_CONTEXT_DATA_CLR_SURROGATE {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	GUID SurrogateIdent; // 0x8, 0x8
	uint32_t VersionOffset; // 0x18, 0x18
	uint32_t VersionLength; // 0x1C, 0x1C
	uint32_t TypeNameOffset; // 0x20, 0x20
	uint32_t TypeNameLength; // 0x24, 0x24
	// 0x28, 0x28
};
ValidateStructSize(0x28, ACTIVATION_CONTEXT_DATA_CLR_SURROGATE);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_CLR_SURROGATE);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_CLR_SURROGATEX = ACTIVATION_CONTEXT_DATA_CLR_SURROGATE;

struct ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSION {
	uint16_t Major; // 0x0, 0x0
	uint16_t Minor; // 0x2, 0x2
	// 0x4, 0x4
};
ValidateStructSize(0x4, ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSION);
ValidateStructAlignment(0x2, ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSIONX = ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSION;

struct ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600 {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t NameLength; // 0x8, 0x8
	uint32_t NameOffset; // 0xC, 0xC
	uint16_t ResourceId; // 0x10, 0x10
	union {
		uint16_t LibraryFlags; // 0x12, 0x12
		struct {
		};
	};
	uint32_t HelpDirLength; // 0x14, 0x14
	uint32_t HelpDirOffset; // 0x18, 0x18
	// 0x1C, 0x1C
};
ValidateStructSize(0x1C, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600X = ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600;

// TODO: inherit?
struct ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t NameLength; // 0x8, 0x8
	uint32_t NameOffset; // 0xC, 0xC
	uint16_t ResourceId; // 0x10, 0x10
	union {
		uint16_t LibraryFlags; // 0x12, 0x12
		struct {
		};
	};
	uint32_t HelpDirLength; // 0x14, 0x14
	uint32_t HelpDirOffset; // 0x18, 0x18
	ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSION Version; // 0x1C, 0x1C
	// 0x20, 0x20
};
ValidateStructSize(0x20, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTIONX = ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION;

struct ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	uint32_t ConfiguredClsidOffset; // 0x8, 0x8
	// 0xC, 0xC
};
ValidateStructSize(0xC, ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION);

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTIONX = ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION;

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXTX;

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_NOTIFY_ROUTINEX = void NTAPI (uint32_t NotificationType,
											   PTRZX<bits, ACTIVATION_CONTEXTX<bits>> ActivationContext,
											   PTRZX<bits, const void> ActivationContextData,
											   PTRZX<bits, UNKNOWN_TYPE> NotificationContext,
											   PTRZX<bits, UNKNOWN_TYPE> NotificationData,
											   PTRZX<bits, bool> DisableThisNotification
											   );


#define ACTCTX_RELEASE_STACK_DEPTH (4)
#define ACTCTX_RELEASE_STACK_SLOTS (4)
template<size_t bits /*= native_bits*/>
struct ACTIVATION_CONTEXTX {
	int32_t RefCount; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	LIST_ENTRYX<bits, UNKNOWN_TYPE> Links; // 0x8, 0x8
	PTRZX<bits, const ACTIVATION_CONTEXT_DATA> ActivationContextData; // 0x10, 0x18
	PTRZX<bits, ACTIVATION_CONTEXT_NOTIFY_ROUTINEX<bits>> NotificationRoutine; // 0x14, 0x20
	PTRZX<bits, UNKNOWN_TYPE> NotificationContext; // 0x18, 0x28
	uint32_t SentNotifications[8]; // 0x1C, 0x30
	uint32_t DisabledNotifications[8]; // 0x3C, 0x50
	ASSEMBLY_STORAGE_MAPX<bits> StorageMap; // 0x5C, 0x70
	ASSEMBLY_STORAGE_MAP_ENTRYX<bits> InlineStorageMapEntries[32]; // 0x68, 0x80
	uint32_t StackTraceIndex; // 0x268, 0x480
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> StackTraces[ACTCTX_RELEASE_STACK_SLOTS][ACTCTX_RELEASE_STACK_DEPTH]; // 0x26C, 0x488
	// 0x2AC, 0x508
};
ValidateStructSize(0x2AC, ACTIVATION_CONTEXTX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXTX<32>);
ValidateStructSize(0x508, ACTIVATION_CONTEXTX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXTX<64>);

//template<size_t bits = native_bits>
//static inline constexpr PTRSX<bits, ACTIVATION_CONTEXTX<bits>> INVALID_ACTIVATION_CONTEXTX = __builtin_bit_cast(PTRSX<bits, ACTIVATION_CONTEXTX<bits>>, -1);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_WRAPPEDX {
	PTRZX<bits, UNKNOWN_TYPE> MagicMarker; // 0x0, 0x0
	ACTIVATION_CONTEXTX<bits> ActivationContext; // 0x4, 0x8
	// 0x2B0, 0x510
};
ValidateStructSize(0x2B0, ACTIVATION_CONTEXT_WRAPPEDX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_WRAPPEDX<32>);
ValidateStructSize(0x510, ACTIVATION_CONTEXT_WRAPPEDX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_WRAPPEDX<64>);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_BASIC_INFORMATIONX {
	union {
		PTRZX<bits, ACTIVATION_CONTEXTX<bits>> ActivationContext; // 0x0, 0x0
		HANDLEX<bits> hActCtx; // 0x0, 0x0
	};
	union {
		uint32_t Flags; // 0x4, 0x8
		uint32_t dwFlags; // 0x4, 0x8
		struct {
		};
	};
	__x64_padding(0x4);
	// 0x8, 0x10
};
ValidateStructSize(0x8, ACTIVATION_CONTEXT_BASIC_INFORMATIONX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_BASIC_INFORMATIONX<32>);
ValidateStructSize(0x10, ACTIVATION_CONTEXT_BASIC_INFORMATIONX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_BASIC_INFORMATIONX<64>);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATIONX {
	union {
		uint32_t ulFlags; // 0x0, 0x0
		struct {
		};
	};
	uint32_t ulEncodedAssemblyIdentityLength; // 0x4, 0x4
	uint32_t ulManifestPathType; // 0x8, 0x8
	uint32_t ulManifestPathLength; // 0xC, 0xC
	LARGE_INTEGERX<bits> liManifestLastWriteTime; // 0x10, 0x10 (FILETIME)
	uint32_t ulPolicyPathType; // 0x18, 0x18
	uint32_t ulPolicyPathLength; // 0x1C, 0x1C
	LARGE_INTEGERX<bits> liPolicyLastWriteTime; // 0x20, 0x20 (FILETIME)
	uint32_t ulMetadataSatelliteRosterIndex; // 0x28, 0x28
	uint32_t ulManifestVersionMajor; // 0x2C, 0x2C
	uint32_t ulManifestVersionMinor; // 0x30, 0x30
	uint32_t ulPolicyVersionMajor; // 0x34, 0x34
	uint32_t ulPolicyVersionMinor; // 0x38, 0x38
	uint32_t ulAssemblyDirectoryNameLength; // 0x3C, 0x3C
	PTRZX<bits, const wchar_t> lpAssemblyEncodedAssemblyIdentity; // 0x40, 0x40
	PTRZX<bits, const wchar_t> lpAssemblyManifestPath; // 0x44, 0x48
	PTRZX<bits, const wchar_t> lpAssemblyPolicyPath; // 0x48, 0x50
	PTRZX<bits, const wchar_t> lpAssemblyDirectoryName; // 0x4C, 0x58
	uint32_t ulFileCount; // 0x50, 0x60
	__x64_padding(0x4);
	// 0x54, 0x68
};
ValidateStructSize(0x54, ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATIONX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATIONX<32>);
ValidateStructSize(0x68, ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATIONX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATIONX<64>);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DETAILED_INFORMATIONX {
	union {
		uint32_t dwFlags; // 0x0, 0x0
		struct {
		};
	};
	uint32_t ulFormatVersion; // 0x4, 0x4
	uint32_t ulAssemblyCount; // 0x8, 0x8
	uint32_t ulRootManifestPathType; // 0xC, 0xC
	uint32_t ulRootManifestPathChars; // 0x10, 0x10
	uint32_t ulRootConfigurationPathType; // 0x14, 0x14
	uint32_t ulRootConfigurationPathChars; // 0x18, 0x18
	uint32_t ulAppDirPathType; // 0x1C, 0x1C
	uint32_t ulAppDirPathChars; // 0x20, 0x20
	__x64_padding(0x4);
	PTRZX<bits, const wchar_t> lpRootManifestPath; // 0x24, 0x28
	PTRZX<bits, const wchar_t> lpRootConfigurationPath; // 0x28, 0x30
	PTRZX<bits, const wchar_t> lpAppDirPath; // 0x2C, 0x38
	// 0x30, 0x40
};
ValidateStructSize(0x30, ACTIVATION_CONTEXT_DETAILED_INFORMATIONX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DETAILED_INFORMATIONX<32>);
ValidateStructSize(0x40, ACTIVATION_CONTEXT_DETAILED_INFORMATIONX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_DETAILED_INFORMATIONX<64>);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600X {
	uint32_t Size; // 0x0, 0x0
	uint32_t DataFormatVersion; // 0x4, 0x4
	PTRZX<bits, UNKNOWN_TYPE> Data; // 0x8, 0x8
	uint32_t Length; // 0xC, 0x10
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> SectionGlobalData; // 0x10, 0x18
	uint32_t SectionGlobalDataLength; // 0x14, 0x20
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> SectionBase; // 0x18, 0x28
	uint32_t SectionTotalLength; // 0x1C, 0x30
	__x64_padding(0x4);
	PTRZX<bits, ACTIVATION_CONTEXTX<bits>> ActivationContext; // 0x20, 0x38
	uint32_t AssemblyRosterIndex; // 0x24, 0x40
	union {
		uint32_t Flags; // 0x28, 0x44
		struct {
		};
	};
	// 0x2C, 0x48
};
ValidateStructSize(0x2C, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600X<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600X<32>);
ValidateStructSize(0x48, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600X<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600X<64>);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATAX {
	PTRZX<bits, ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION> Information; // 0x0, 0x0
	PTRZX<bits, UNKNOWN_TYPE> SectionBase; // 0x4, 0x8
	uint32_t SectionLength; // 0x8, 0x10
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> SectionGlobalDataBase; // 0xC, 0x18
	uint32_t SectionGlobalDataLength; // 0x10, 0x20
	__x64_padding(0x4);
	// 0x14, 0x28
};
ValidateStructSize(0x14, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATAX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATAX<32>);
ValidateStructSize(0x28, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATAX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATAX<64>);

// TODO: Inherit from ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600X?
template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_SECTION_KEYED_DATAX {
	uint32_t Size; // 0x0, 0x0
	uint32_t DataFormatVersion; // 0x4, 0x4
	PTRZX<bits, UNKNOWN_TYPE> Data; // 0x8, 0x8
	uint32_t Length; // 0xC, 0x10
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> SectionGlobalData; // 0x10, 0x18
	uint32_t SectionGlobalDataLength; // 0x14, 0x20
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> SectionBase; // 0x18, 0x28
	uint32_t SectionTotalLength; // 0x1C, 0x30
	__x64_padding(0x4);
	PTRZX<bits, ACTIVATION_CONTEXTX<bits>> ActivationContext; // 0x20, 0x38
	uint32_t AssemblyRosterIndex; // 0x24, 0x40
	union {
		uint32_t Flags; // 0x28, 0x44
		struct {
		};
	};
	ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATAX<bits> AssemblyMetadata; // 0x2C, 0x48
	// 0x40, 0x70
};
ValidateStructSize(0x40, ACTIVATION_CONTEXT_SECTION_KEYED_DATAX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_SECTION_KEYED_DATAX<32>);
ValidateStructSize(0x70, ACTIVATION_CONTEXT_SECTION_KEYED_DATAX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_SECTION_KEYED_DATAX<64>);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_ASSEMBLY_DATAX {
	uint32_t Size; // 0x0, 0x0
	union {
		uint32_t Flags; // 0x4, 0x4
		struct {
		};
	};
	PTRZX<bits, const wchar_t> AssemblyName; // 0x8, 0x8
	uint32_t AssemblyNameLength; // 0xC, 0x10
	uint32_t HashAlgorithm; // 0x10, 0x14
	uint32_t PseudoKey; // 0x14, 0x18
	__x64_padding(0x4);
	// 0x18, 0x20
};
ValidateStructSize(0x18, ACTIVATION_CONTEXT_ASSEMBLY_DATAX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_ASSEMBLY_DATAX<32>);
ValidateStructSize(0x20, ACTIVATION_CONTEXT_ASSEMBLY_DATAX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_ASSEMBLY_DATAX<64>);

template<size_t bits = native_bits>
struct RTL_ACTIVATION_CONTEXT_STACK_FRAMEX {
	PTRZX<bits, RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<bits>> Previous; // 0x0, 0x0
	PTRZX<bits, ACTIVATION_CONTEXTX<bits>> ActivationContext; // 0x4, 0x8
	union {
		uint32_t Flags; // 0x8, 0x10
		struct {
		};
	};
	__x64_padding(0x4);
	// 0xC, 0x18
};
ValidateStructSize(0xC, RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<32>);
ValidateStructAlignment(0x4, RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<32>);
ValidateStructSize(0x18, RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<64>);
ValidateStructAlignment(0x8, RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<64>);

template<size_t bits = native_bits>
struct RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX {
	RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<bits> Frame; // 0x0, 0x0
	uintptr_tx<bits> Cookie; // 0xC, 0x18
	PTRZX<bits, UNKNOWN_TYPE> ActivationStackBackTrace[8]; // 0x10, 0x20
	// 0x30, 0x60
};
ValidateStructSize(0x30, RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX<32>);
ValidateStructAlignment(0x4, RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX<32>);
ValidateStructSize(0x60, RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX<64>);
ValidateStructAlignment(0x8, RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX<64>);

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_STACK_FRAMELISTX {
	uint32_t Magic; // 0x0, 0x0
	uint32_t FramesInUse; // 0x4, 0x4
	LIST_ENTRYX<bits, UNKNOWN_TYPE> Links; // 0x8, 0x8
	union {
		uint32_t Flags; // 0x10, 0x18
		struct {
		};
	};
	uint32_t NotFramesInUse; // 0x14, 0x1C
	RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX<bits> Frames[32]; // 0x18, 0x20
	// 0x618, 0xC20
};
ValidateStructSize(0x618, ACTIVATION_CONTEXT_STACK_FRAMELISTX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_STACK_FRAMELISTX<32>);
ValidateStructSize(0xC20, ACTIVATION_CONTEXT_STACK_FRAMELISTX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_STACK_FRAMELISTX<64>);

template<size_t bits = native_bits>
struct RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASICX {
	size_tx<bits> Size; // 0x0, 0x0
	uint32_t Format; // 0x4, 0x8
	__x64_padding(0x4);
	RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<bits> Frame; // 0x8, 0x10
	// 0x14, 0x28
};
ValidateStructSize(0x14, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASICX<32>);
ValidateStructAlignment(0x4, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASICX<32>);
ValidateStructSize(0x28, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASICX<64>);
ValidateStructAlignment(0x8, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASICX<64>);

// TODO: inherit
template<size_t bits = native_bits>
struct RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX {
	size_tx<bits> Size; // 0x0, 0x0
	uint32_t Format; // 0x4, 0x8
	__x64_padding(0x4);
	RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<bits> Frame; // 0x8, 0x10
	PTRZX<bits> Extra1; // 0x14, 0x28
	PTRZX<bits> Extra2; // 0x18, 0x30
	PTRZX<bits> Extra3; // 0x1C, 0x38
	PTRZX<bits> Extra4; // 0x20, 0x40
	// 0x24, 0x48
};
ValidateStructSize(0x24, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX<32>);
ValidateStructAlignment(0x4, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX<32>);
ValidateStructSize(0x48, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX<64>);
ValidateStructAlignment(0x8, RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX<64>);

template<size_t bits = native_bits>
using RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX = RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX<bits>;

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_STACKX {
	union {
		uint32_t Flags; // 0x0, 0x0
		struct {
		};
	};
	uint32_t NextCookieSequenceNumber; // 0x4, 0x4
	PTRZX<bits, RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<bits>> ActiveFrame; // 0x8, 0x8
	LIST_ENTRYX<bits, UNKNOWN_TYPE> FrameListCache; // 0xC, 0x10
	// 0x14, 0x20
};
ValidateStructSize(0x14, ACTIVATION_CONTEXT_STACKX<32>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_STACKX<32>);
ValidateStructSize(0x20, ACTIVATION_CONTEXT_STACKX<64>);
ValidateStructAlignment(0x8, ACTIVATION_CONTEXT_STACKX<64>);

#pragma endregion

template<size_t bits = native_bits>
struct LDR_DATA_TABLE_ENTRYX {
	LIST_ENTRYX<bits, LDR_DATA_TABLE_ENTRYX<bits>> InLoadOrderLinks; // 0x0, 0x0
	LIST_ENTRYX<bits, LDR_DATA_TABLE_ENTRYX<bits>> InMemoryOrderLinks; // 0x8, 0x10
	LIST_ENTRYX<bits, LDR_DATA_TABLE_ENTRYX<bits>> InInitializationOrderLinks; // 0x10, 0x20
	PTRZX<bits> DllBase; // 0x18, 0x30
	PTRZX<bits> EntryPoint; // 0x1C, 0x38
	uint32_t SizeOfImage; // 0x20, 0x40
	__x64_padding(0x4);
	UNICODE_STRINGX<bits> FullDllName; // 0x24, 0x48
	UNICODE_STRINGX<bits> BaseDllName; // 0x2C, 0x58
	union {
		uint32_t Flags; // 0x34, 0x68
		struct {
		};
	};
	uint16_t LoadCount; // 0x38, 0x6C
	uint16_t TlsIndex; // 0x3A, 0x6E
	union {
		LIST_ENTRYX<bits, UNKNOWN_TYPE> HashLinks; // 0x3C, 0x70
		struct {
			PTRNZ<bits> SectionPointer; // 0x3C, 0x70
			uint32_t CheckSum; // 0x40, 0x78
			__x64_padding(0x4);
		};
	};
	union {
		struct {
			uint32_t TimeDateStamp; // 0x44, 0x80
			__x64_padding(0x4);
		};
		PTRNZ<bits> LoadedImports; // 0x44, 0x80
	};
	PTRZX<bits, ACTIVATION_CONTEXTX<bits>> EntryPointActivationContext; // 0x48, 0x88
	PTRZX<bits, UNKNOWN_TYPE> PatchInformation; // 0x4C, 0x90
	LIST_ENTRYX<bits, UNKNOWN_TYPE> ForwarderLinks; // 0x50, 0x98
	LIST_ENTRYX<bits, UNKNOWN_TYPE> ServiceTagLinks; // 0x54, 0xA8
	LIST_ENTRYX<bits, UNKNOWN_TYPE> StaticLinks; // 0x58, 0xB8
	PTRZX<bits, UNKNOWN_TYPE> ContextInformation; // 0x5C, 0xC8
	uintptr_tx<bits> OriginalBase; // 0x60, 0xD0
	LARGE_INTEGERX<bits> LoadTime; // 0x70, 0xD8
	// 0x78, 0xE0
};
ValidateStructSize(0x78, LDR_DATA_TABLE_ENTRYX<32>);
ValidateStructAlignment(0x4, LDR_DATA_TABLE_ENTRYX<32>);
ValidateStructSize(0xE0, LDR_DATA_TABLE_ENTRYX<64>);
ValidateStructAlignment(0x8, LDR_DATA_TABLE_ENTRYX<64>);

template<size_t bits = native_bits>
struct PEB_LDR_DATAX {
	uint32_t Length; // 0x0, 0x0
	bool Initialized; // 0x4, 0x4
	__padding(0x3); // 0x5, 0x5
	PTRZX<bits, UNKNOWN_TYPE> SsHandle; // 0x8, 0x8
	LIST_ENTRYX<bits, LDR_DATA_TABLE_ENTRYX<bits>> InLoadOrderModuleList; // 0xC, 0x10
	LIST_ENTRYX<bits, LDR_DATA_TABLE_ENTRYX<bits>> InMemoryOrderModuleList; // 0x14, 0x20
	LIST_ENTRYX<bits, LDR_DATA_TABLE_ENTRYX<bits>> InInitializationOrderModuleList; // 0x1C, 0x30
	PTRZX<bits> EntryInProgress; // 0x24, 0x40
	bool ShutdownInProgress; // 0x28, 0x48
	__padding(0x3); // 0x29, 0x48
	__x64_padding(0x4);
	PTRZX<bits> ShutdownThreadId; // 0x2C, 0x50
	// 0x30, 0x58
};
ValidateStructSize(0x30, PEB_LDR_DATAX<32>);
ValidateStructAlignment(0x4, PEB_LDR_DATAX<32>);
ValidateStructSize(0x58, PEB_LDR_DATAX<64>);
ValidateStructAlignment(0x8, PEB_LDR_DATAX<64>);

template<size_t bits = native_bits>
struct CURDIRX {
	UNICODE_STRINGX<bits> DosPath; // 0x0, 0x0
	HANDLEX<bits> Handle; // 0x8, 0x10
	// 0xC, 0x18
};
ValidateStructSize(0xC, CURDIRX<32>);
ValidateStructAlignment(0x4, CURDIRX<32>);
ValidateStructSize(0x18, CURDIRX<64>);
ValidateStructAlignment(0x8, CURDIRX<64>);

template<size_t bits = native_bits>
struct RTL_DRIVE_LETTER_CURDIRX {
	uint16_t Flags; // 0x0, 0x0
	uint16_t Length; // 0x2, 0x2
	uint32_t TimeStamp; // 0x4, 0x4
	STRINGX<bits> DosPath; // 0x8, 0x8
	// 0x10, 0x18
};
ValidateStructSize(0x10, RTL_DRIVE_LETTER_CURDIRX<32>);
ValidateStructAlignment(0x4, RTL_DRIVE_LETTER_CURDIRX<32>);
ValidateStructSize(0x18, RTL_DRIVE_LETTER_CURDIRX<64>);
ValidateStructAlignment(0x8, RTL_DRIVE_LETTER_CURDIRX<64>);

template<size_t bits = native_bits>
struct RTL_USER_PROCESS_PARAMETERSX {
	uint32_t MaximumLength; // 0x0, 0x0
	uint32_t Length; // 0x4, 0x4
	union {
		uint32_t Flags; // 0x8, 0x8
		struct {
		};
	};
	union {
		uint32_t DebugFlags; // 0xC, 0xC
		struct {
		};
	};
	HANDLEX<bits> ConsoleHandle; // 0x10, 0x10
	union {
		uint32_t ConsoleFlags; // 0x14, 0x18
		struct {
		};
	};
	__x64_padding(0x4);
	HANDLEX<bits> StandardInput; // 0x18, 0x20
	HANDLEX<bits> StandardOutput; // 0x1C, 0x28
	HANDLEX<bits> StandardError; // 0x20, 0x30
	CURDIRX<bits> CurrentDirectory; // 0x24, 0x38
	UNICODE_STRINGX<bits> DllPath; // 0x30, 0x50
	UNICODE_STRINGX<bits> ImagePathName; // 0x38, 0x60
	UNICODE_STRINGX<bits> CommandLine; // 0x40, 0x70
	PTRZX<bits> Environment; // 0x48, 0x80
	uint32_t StartingX; // 0x4C, 0x88
	uint32_t StartingY; // 0x50, 0x8C
	uint32_t CountX; // 0x54, 0x90
	uint32_t CountY; // 0x58, 0x94
	uint32_t CountCharsX; // 0x5C, 0x98
	uint32_t CountCharsY; // 0x60, 0x9C
	uint32_t FillAttribute; // 0x64, 0xA0
	union {
		uint32_t WindowFlags; // 0x68, 0xA4
		struct {
		};
	};
	union {
		uint32_t ShowWindowFlags; // 0x6C, 0xA8
		struct {
		};
	};
	__x64_padding(0x4);
	UNICODE_STRINGX<bits> WindowTitle; // 0x70, 0xB0
	UNICODE_STRINGX<bits> DesktopInfo; // 0x78, 0xC0
	UNICODE_STRINGX<bits> ShellInfo; // 0x80, 0xD0
	UNICODE_STRINGX<bits> RuntimeData; // 0x88, 0xE0
	RTL_DRIVE_LETTER_CURDIRX<bits> CurrentDirectories[32]; // 0x90, 0xF0
	volatile uintptr_tx<bits> EnvironmentSize; // 0x290, 0x3F0
	volatile uintptr_tx<bits> EnvironmentVersion; // 0x294, 0x3F8
	// 0x298, 0x400
};
ValidateStructSize(0x298, RTL_USER_PROCESS_PARAMETERSX<32>);
ValidateStructAlignment(0x4, RTL_USER_PROCESS_PARAMETERSX<32>);
ValidateStructSize(0x400, RTL_USER_PROCESS_PARAMETERSX<64>);
ValidateStructAlignment(0x8, RTL_USER_PROCESS_PARAMETERSX<64>);

template<size_t bits = native_bits>
struct RTL_CRITICAL_SECTIONX;

template<size_t bits = native_bits>
struct RTL_CRITICAL_SECTION_DEBUGX {
	uint16_t Type; // 0x0, 0x0
	uint16_t CreatorBackTraceIndex; // 0x2, 0x2
	__x64_padding(0x4);
	PTRZX<bits, RTL_CRITICAL_SECTIONX<bits>> CriticalSection; // 0x4, 0x8
	LIST_ENTRYX<bits> ProcessLocksList; // 0x8, 0x10
	uint32_t EntryCount; // 0x10, 0x20
	uint32_t ContentionCount; // 0x14, 0x24
	union {
		uint32_t Flags; // 0x18, 0x28
		struct {
		};
	};
	uint16_t CreatorBackTraceIndexHigh; // 0x1C, 0x2C
	uint16_t SpareWORD; // 0x1E, 0x2E
	// 0x20, 0x30
};
ValidateStructSize(0x20, RTL_CRITICAL_SECTION_DEBUGX<32>);
ValidateStructAlignment(0x4, RTL_CRITICAL_SECTION_DEBUGX<32>);
ValidateStructSize(0x30, RTL_CRITICAL_SECTION_DEBUGX<64>);
ValidateStructAlignment(0x8, RTL_CRITICAL_SECTION_DEBUGX<64>);

template<size_t bits /*= native_bits*/>
struct RTL_CRITICAL_SECTIONX {
	PTRZX<bits, RTL_CRITICAL_SECTION_DEBUGX<bits>> DebugInfo; // 0x0, 0x0
	int32_t LockCount; // 0x4, 0x8
	int32_t RecursionCount; // 0x8, 0xC
	PTRZX<bits, UNKNOWN_TYPE> OwningThread; // 0xC, 0x10
	PTRZX<bits, UNKNOWN_TYPE> LockSemaphore; // 0x10, 0x18
	size_tx<bits> SpinCount; // 0x14, 0x20
	// 0x18, 0x28
};
ValidateStructSize(0x18, RTL_CRITICAL_SECTIONX<32>);
ValidateStructAlignment(0x4, RTL_CRITICAL_SECTIONX<32>);
ValidateStructSize(0x28, RTL_CRITICAL_SECTIONX<64>);
ValidateStructAlignment(0x8, RTL_CRITICAL_SECTIONX<64>);


union NtGlobalFlag_t {
	uint32_t raw; // 0x0, 0x0
	struct {
		uint32_t stop_on_exception : 1; // FLG_STOP_ON_EXCEPTION
		uint32_t show_loader_snaps : 1; // FLG_SHOW_LDR_SNAPS
		uint32_t debug_initial_command : 1; // FLG_DEBUG_INITIAL_COMMAND
		uint32_t stop_on_hung_gui : 1; // FLG_STOP_ON_HUNG_GUI
		uint32_t enable_heap_tail_check : 1; // FLG_HEAP_ENABLE_TAIL_CHECK
		uint32_t enable_heap_free_check : 1; // FLG_HEAP_ENABLE_FREE_CHECK
		uint32_t enable_heap_parameter_validation : 1; // FLG_HEAP_VALIDATE_PARAMETERS
		uint32_t enable_all_heap_validations : 1; // FLG_HEAP_VALIDATE_ALL
		uint32_t enable_application_verifier : 1; // FLG_APPLICATION_VERIFIER
		uint32_t enable_silent_process_exit_monitoring : 1; // FLG_MONITOR_SILENT_PROCESS_EXIT
		uint32_t enable_pool_tagging : 1; // FLG_POOL_ENABLE_TAGGING
		uint32_t enable_heap_tagging : 1; // FLG_HEAP_ENABLE_TAGGING
		uint32_t create_user_mode_stack_trace_database : 1; // FLG_USER_STACK_TRACE_DB
		uint32_t create_kernel_mode_stack_trace_database : 1; // FLG_KERNEL_STACK_TRACE_DB
		uint32_t maintain_object_type_list : 1; // FLG_MAINTAIN_OBJECT_TYPELIST
		uint32_t enable_heap_tagging_by_dll : 1; // FLG_HEAP_ENABLE_TAG_BY_DLL
		uint32_t disable_stack_extension : 1; // FLG_DISABLE_STACK_EXTENSION
		uint32_t enable_win32_subsystem_debug : 1; // FLG_ENABLE_CSRDEBUG
		uint32_t enable_kernel_debugger_symbol_loading : 1; // FLG_ENABLE_KDEBUG_SYMBOL_LOAD
		uint32_t disable_kernel_stack_paging : 1; // FLG_DISABLE_PAGE_KERNEL_STACKS
		uint32_t enable_system_critical_breaks : 1; // FLG_ENABLE_SYSTEM_CRIT_BREAKS
		uint32_t disable_heap_coalesce_on_free : 1; // FLG_HEAP_DISABLE_COALESCING
		uint32_t enable_handle_close_exception : 1; // FLG_ENABLE_CLOSE_EXCEPTIONS
		uint32_t enable_exception_logging : 1; // FLG_ENABLE_EXCEPTION_LOGGING
		uint32_t enable_object_handle_type_tagging : 1; // FLG_ENABLE_HANDLE_TYPE_TAGGING
		uint32_t enable_page_heap_verification : 1; // FLG_HEAP_PAGE_ALLOCS
		uint32_t debug_initial_command_ex : 1; // FLG_DEBUG_INITIAL_COMMAND_EX
		uint32_t buffer_dbgprint_output : 1; // FLG_DISABLE_DBGPRINT
		uint32_t early_critical_section_event_creation : 1; // FLG_CRITSEC_EVENT_CREATION
		uint32_t stop_on_unhandled_user_mode_exception : 1; // FLG_STOP_ON_UNHANDLED_EXCEPTION
		uint32_t enable_bad_handle_detection : 1; // FLG_ENABLE_HANDLE_EXCEPTIONS
		uint32_t disable_protected_dll_verification : 1; // FLG_DISABLE_PROTDLLS
	};
	// 0x4, 0x4
};
ValidateStructSize(0x4, NtGlobalFlag_t);
ValidateStructAlignment(0x4, NtGlobalFlag_t);

template<size_t bits = native_bits>
using NtGlobalFlag_tx = NtGlobalFlag_t;

template<size_t bits = native_bits>
union AppCompatFlags_tx {
	ULARGE_INTEGERX<bits> raw; // 0x0, 0x0
	struct {
		uint64_tx<bits> get_short_path_name_old_behavior : 1; // KACF_OLDGETSHORTPATHNAME
		uint64_tx<bits> get_version_is_a_lie : 1; // KACF_VERSIONLIE
		uint64_tx<bits> : 1;
		uint64_tx<bits> get_disk_free_space_2g_friendly : 1; // KACF_GETDISKFREESPACE
		uint64_tx<bits> __get_temp_path_jank : 1; // KACF_GETTEMPPATH
		uint64_tx<bits> __some_dcom_weirdness_idk : 1; // KACF_FTMFROMCURRENTAPT
		uint64_tx<bits> __do_not_notify_binding_changes : 1; // KACF_DISALLOWORBINDINGCHANGES
		uint64_tx<bits> ole32_validates_pointers : 1; // KACF_OLE32VALIDATEPTRS
		uint64_tx<bits> disable_cicero : 1; // KACF_DISABLECICERO
		uint64_tx<bits> ole32_enable_async_doc_file : 1; // KACF_OLE32ENABLEASYNCDOCFILE
	};
} ide_gnu_packed;
ValidateStructSize(0x8, AppCompatFlags_tx<32>);
ValidateStructAlignment(0x4, AppCompatFlags_tx<32>);
ValidateStructSize(0x8, AppCompatFlags_tx<64>);
ValidateStructAlignment(0x8, AppCompatFlags_tx<64>);

template<size_t bits>
struct GDI_HANDLE_BUFFERX_base;

template<>
struct GDI_HANDLE_BUFFERX_base<32> {
	static inline constexpr size_t buffer_size = 34;
	using type = uint32_t[buffer_size];
};

template<>
struct GDI_HANDLE_BUFFERX_base<64> {
	static inline constexpr size_t buffer_size = 60;
	using type = uint32_t[buffer_size];
};

template<size_t bits = native_bits>
using GDI_HANDLE_BUFFERX = GDI_HANDLE_BUFFERX_base<bits>::type;

template<size_t bits = native_bits>
static inline constexpr size_t GDI_HANDLE_BUFFERX_SIZE = GDI_HANDLE_BUFFERX_base<bits>::buffer_size;

//using PPS_POST_PROCESS_INIT_ROUTINE = void(void);

/*========================================
	PEB
========================================*/

//#define FLS_MAXIMUM_AVAILABLE 128
#define TLS_MINIMUM_AVAILABLE 64
#define TLS_EXPANSION_SLOTS   1024

template<size_t bits = native_bits>
struct PEBX {
	bool InheritedAddressSpace; // 0x0
	bool ReadImageFileExecOptions; // 0x1
	bool BeingDebugged; // 0x2
	union {
		uint8_t BitField; // 0x3
		struct {
			uint8_t ImageUsesLargePages : 1;
			uint8_t IsProtectedProcess : 1;
			uint8_t IsLegacyProcess : 1;
			uint8_t IsImageDynamicallyRelocated : 1;
			uint8_t SkipPatchingUser32Forwarders : 1;
			uint8_t SpareBits : 3;
		};
	};
	__x64_padding(0x4);
	HANDLEX<bits> Mutant; // 0x4, 0x8
	PTRZX<bits> ImageBaseAddress; // 0x8, 0x10
	PTRZX<bits, PEB_LDR_DATAX<bits>> Ldr; // 0xC, 0x18 (Why did I have this as a normal pointer and not PTRZX...?)
	PTRZX<bits, RTL_USER_PROCESS_PARAMETERSX<bits>> ProcessParameters; // 0x10, 0x20
	PTRZX<bits> SubSystemData; // 0x14, 0x28
	HANDLEX<bits> ProcessHeap; // 0x18, 0x30
	PTRZX<bits, RTL_CRITICAL_SECTIONX<bits>> FastPebLock; // 0x1C, 0x38
	PTRZX<bits, UNKNOWN_TYPE> AtlThunkSListPtr; // 0x20, 0x40
	PTRZX<bits, UNKNOWN_TYPE> IFEOKey; // 0x24, 0x48
	union {
		uint32_t CrossProcessFlags; // 0x28, 0x50
		struct {
			uint32_t ProcessInJob : 1;
			uint32_t ProcessInitializing : 1;
			uint32_t ProcessUsingVEH : 1;
			uint32_t ProcessUsingVCH : 1;
			uint32_t ProcessUsingFTH : 1;
			uint32_t ReservedBits0 : 27;
		};
	};
	__x64_padding(0x4);
	union {
		PTRZX<bits, UNKNOWN_TYPE> KernelCallbackTable; // 0x2C, 0x58
		PTRZX<bits, UNKNOWN_TYPE> UserSharedInfoPtr; // 0x2C, 0x58
	};
	uint32_t SystemReserved[1]; // 0x30, 0x60
	uint32_t AtlThunkSListPtr32; // 0x34, 0x64
	PTRZX<bits, UNKNOWN_TYPE> ApiSetMap; // 0x38, 0x68
	uint32_t TlsExpansionCounter; // 0x3C, 0x70
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> TlsBitmap; // 0x40, 0x78
	uint32_t TlsBitmapBits[2]; // 0x44, 0x80
	PTRZX<bits, UNKNOWN_TYPE> ReadOnlySharedMemoryBase; // 0x4C, 0x88
	PTRZX<bits, UNKNOWN_TYPE> HotpatchInformation; // 0x50, 0x90
	PTRZX<bits, PTRZX<bits, UNKNOWN_TYPE>> ReadOnlyStaticServerData; // 0x54, 0x98
	PTRZX<bits, UNKNOWN_TYPE> AnsiCodePageData; // 0x58, 0xA0
	PTRZX<bits, UNKNOWN_TYPE> OemCodePageData; // 0x5C, 0xA8
	PTRZX<bits, UNKNOWN_TYPE> UnicodeCaseTableData; // 0x60, 0xB0
	uint32_t NumberOfProcessors; // 0x64, 0xB8
	NtGlobalFlag_tx<bits> NtGlobalFlag; // 0x68, 0xBC
	__x86_padding(0x4);
	LARGE_INTEGERX<64> CriticalSectionTimeout; // 0x70, 0xC0
	size_tx<bits> HeapSegmentReserve; // 0x78, 0xC8
	size_tx<bits> HeapSegmentCommit; // 0x7C, 0xD0
	size_tx<bits> HeapDeCommitTotalFreeThreshold; // 0x80, 0xD8
	size_tx<bits> HeapDeCommitFreeBlockThreshold; // 0x84, 0xE0
	uint32_t NumberOfHeaps; // 0x88, 0xE8
	uint32_t MaximumNumberOfHeaps; // 0x8C, 0xEC
	PTRZX<bits, PTRZX<bits, UNKNOWN_TYPE>> ProcessHeaps; // 0x90, 0xF0
	PTRZX<bits, UNKNOWN_TYPE> GdiSharedHandleTable; // 0x94, 0xF8
	PTRZX<bits, UNKNOWN_TYPE> ProcessStarterHelper; // 0x98, 0x100
	uint32_t GdiDCAttributeList; // 0x9C, 0x108
	__x64_padding(0x4);
	PTRZX<bits, RTL_CRITICAL_SECTIONX<bits>> LoaderLock; // 0xA0, 0x110
	uint32_t OSMajorVersion; // 0xA4, 0x118
	uint32_t OSMinorVersion; // 0xA8, 0x11C
	uint16_t OSBuildNumber; // 0xAC, 0x120
	union {
		uint16_t OSCSDVersion; // 0xAE, 0x122
		struct {
			uint8_t OSCSDMajorVersion; // 0xAE, 0x122
			uint8_t OSCSDMinorVersion; // 0xAF, 0x123
		};
	};
	uint32_t OSPlatformId; // 0xB0, 0x124
	uint32_t ImageSubsystem; // 0xB4, 0x128
	uint32_t ImageSubsystemMajorVersion; // 0xB8, 0x12C
	uint32_t ImageSubsystemMinorVersion; // 0xBC, 0x130
	__x64_padding(0x4);
	KAFFINITYX<bits> ActiveProcessAffinityMask; // 0xC0, 0x138
	GDI_HANDLE_BUFFERX<bits> GdiHandleBuffer; // 0xC4, 0x140
	PTRZX<bits, PPS_POST_PROCESS_INIT_ROUTINE> PostProcessInitRoutine; // 0x14C, 0x230
	PTRZX<bits, UNKNOWN_TYPE> TlsExpansionBitmap; // 0x150, 0x238
	uint32_t TlsExpansionBitmapBits[0x20]; // 0x154, 0x240
	uint32_t SessionId; // 0x1D4, 0x2C0
	__x64_padding(0x4);
	AppCompatFlags_tx<bits> AppCompatFlags; // 0x1D8, 0x2C8
	AppCompatFlags_tx<bits> AppCompatFlagsUser; // 0x1E0, 0x2D0
	PTRZX<bits, UNKNOWN_TYPE> pShimData; // 0x1E8, 0x2D8
	PTRZX<bits, UNKNOWN_TYPE> AppCompatInfo; // 0x1EC, 0x2E0
	UNICODE_STRINGX<bits> CSDVersion; // 0x1F0, 0x2E8
	PTRZX<bits, const ACTIVATION_CONTEXT_DATAX<bits>> ActivationContextData; // 0x1F8, 0x2F8
	PTRZX<bits, ASSEMBLY_STORAGE_MAPX<bits>> ProcessAssemblyStorageMap; // 0x1FC, 0x300
	PTRZX<bits, const ACTIVATION_CONTEXT_DATAX<bits>> SystemDefaultActivationContextData; // 0x200, 0x308
	PTRZX<bits, ASSEMBLY_STORAGE_MAPX<bits>> SystemAssemblyStorageMap; // 0x204, 0x310
	size_tx<bits> MinimumStackCommit; // 0x208, 0x318
	PTRZX<bits, PTRZX<bits, UNKNOWN_TYPE>> FlsCallback; // 0x20C, 0x320
	LIST_ENTRYX<bits, UNKNOWN_TYPE> FlsListHead; // 0x210, 0x328
	PTRZX<bits, UNKNOWN_TYPE> FlsBitmap; // 0x218, 0x338
	uint32_t FlsBitmapBits[4]; // 0x21C, 0x340
	uint32_t FlsHighIndex; // 0x22C, 0x350
	PTRZX<bits, UNKNOWN_TYPE> WerRegistrationData; // 0x230, 0x358
	PTRZX<bits, UNKNOWN_TYPE> WerShipAssertPtr; // 0x234, 0x360
	PTRZX<bits, UNKNOWN_TYPE> pContextData; // 0x238, 0x368
	PTRZX<bits, UNKNOWN_TYPE> pImageHeaderHash; // 0x23C, 0x370
	union {
		uint32_t TracingFlags; // 0x240, 0x378
		struct {
			uint32_t HeapTracingEnabled : 1;
			uint32_t CritSecTracingEnabled : 1;
			uint32_t SpareTracingBits : 30;
		};
	};
	__padding(0x4);
	// 0x248, 0x380
};
ValidateStructSize(0x248, PEBX<32>);
ValidateStructAlignment(0x8, PEBX<32>);
ValidateStructSize(0x380, PEBX<64>);
ValidateStructAlignment(0x8, PEBX<64>);

/*========================================
	CONTEXT
========================================*/

struct FSAVE_AREA {
	FCWW ControlWord; // 0x0, 0x0
	FSWW StatusWord; // 0x4, 0x4
	uint32_t TagWord; // 0x8, 0x8
	uint32_t ErrorOffset; // 0xC, 0xC
	uint16_t ErrorSelector; // 0x10, 0x10
	uint16_t ErrorOpcode; // 0x12, 0x12
	uint32_t DataOffset; // 0x14, 0x14
	uint32_t DataSelector; // 0x18, 0x18
	PackedLongDouble RegisterArea[8]; // 0x1C, 0x1C
	// 0x6C, 0x6C
};
ValidateStructSize(0x6C, FSAVE_AREA);
ValidateStructAlignment(0x4, FSAVE_AREA);

template<size_t bits = native_bits>
struct FLOATING_SAVE_AREAX : FSAVE_AREA {
	uint32_t Cr0NpxState; // 0x6C, 0x6C
	// 0x70, 0x70
};
ValidateStructSize(0x70, FLOATING_SAVE_AREAX<32>);
ValidateStructAlignment(0x4, FLOATING_SAVE_AREAX<32>);
ValidateStructSize(0x70, FLOATING_SAVE_AREAX<64>);
ValidateStructAlignment(0x4, FLOATING_SAVE_AREAX<64>);

struct FXSAVE_DEFAULT_PADDING {
	unused_bytes(48); // 0x0
	// 0x30
};
ValidateStructSize(0x30, FXSAVE_DEFAULT_PADDING);

template<size_t bits, typename T = FXSAVE_DEFAULT_PADDING>
struct FXSAVE_FORMATX_base;

template<typename T> requires(sizeof(T) <= 0x30)
struct FXSAVE_FORMATX_base<32, T> {
	FCW ControlWord; // 0x0
	FSW StatusWord; // 0x2
	uint8_t TagWord; // 0x4
	__x86_padding(0x1);
	uint16_t ErrorOpcode; // 0x6
	uint32_t ErrorOffset; // 0x8
	uint16_t ErrorSelector; // 0xC
	__x86_padding(0x2);
	uint32_t DataOffset; // 0x10
	uint16_t DataSelector; // 0x4
	__x86_padding(0x2);
	MXCSR MxCsr; // 0x18
	MXCSR MxCsr_Mask; // 0x1C
	PaddedLongDouble FloatRegisters[8]; // 0x20
	__m128_unaligned XmmRegisters[16]; // 0xA0
	reserved_bytes(0x30); // 0x1A0
	T UserArea; // 0x1D0
	unsigned char end_padding[(std::max)((size_t)0, 0x30 - sizeof(T))];
	// 0x200
};
template<typename T> requires(sizeof(T) <= 0x30)
struct FXSAVE_FORMATX_base<64, T> {
	FCW ControlWord; // 0x0
	FSW StatusWord; // 0x2
	uint8_t TagWord; // 0x4
	__x64_padding(0x1);
	uint16_t ErrorOpcode; // 0x6
	uint64_t ErrorOffset; // 0x8
	uint64_t DataOffset; // 0x10
	MXCSR MxCsr; // 0x18
	MXCSR MxCsr_Mask; // 0x1C
	PaddedLongDouble FloatRegisters[8]; // 0x20
	__m128_unaligned XmmRegisters[16]; // 0xA0
	reserved_bytes(0x30); // 0x1A0
	T UserArea; // 0x1D0
	unsigned char end_padding[(std::max)((size_t)0, 0x30 - sizeof(T))];
	// 0x200
};

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING, bool align = true> requires(sizeof(T) <= 0x30)
struct FXSAVE_FORMATX;

template<size_t bits, typename T>
struct FXSAVE_FORMATX<bits, T, false> : FXSAVE_FORMATX_base<bits, T> {};

template<size_t bits, typename T>
struct alignas(16) FXSAVE_FORMATX<bits, T, true> : FXSAVE_FORMATX_base<bits, T> {};

ValidateStructSize(0x200, FXSAVE_FORMATX<32>);
ValidateStructAlignment(0x10, FXSAVE_FORMATX<32>);
ValidateStructSize(0x200, FXSAVE_FORMATX<64>);
ValidateStructAlignment(0x10, FXSAVE_FORMATX<64>);

template<size_t bits = native_bits>
struct XSAVE_AREA_HEADERX {
	uint64_t Mask; // 0x0, 0x0
	uint64_t CompactionMask; // 0x8, 0x8
	uint64_t Reserved2[6]; // 0x10, 0x10
	// 0x40, 0x40
};
ValidateStructSize(0x40, XSAVE_AREA_HEADERX<32>);
ValidateStructAlignment(0x8, XSAVE_AREA_HEADERX<32>);
ValidateStructSize(0x40, XSAVE_AREA_HEADERX<64>);
ValidateStructAlignment(0x8, XSAVE_AREA_HEADERX<64>);

template<size_t bits, typename T = FXSAVE_DEFAULT_PADDING, bool align = true>
struct XSAVE_FORMATX;

template<size_t bits, typename T>
struct XSAVE_FORMATX<bits, T, false> : FXSAVE_FORMATX<bits, T, false> {};

template<size_t bits, typename T>
struct alignas(64) XSAVE_FORMATX<bits, T, true> : FXSAVE_FORMATX<bits, T, true> {};

ValidateStructSize(0x200, XSAVE_FORMATX<32>);
ValidateStructAlignment(0x40, XSAVE_FORMATX<32>);
ValidateStructSize(0x200, XSAVE_FORMATX<64>);
ValidateStructAlignment(0x40, XSAVE_FORMATX<64>);

template<size_t bits, typename T = FXSAVE_DEFAULT_PADDING, bool align = true>
struct XSAVE_AREAX_base {
	XSAVE_FORMATX<bits, T, align> LegacyState; // 0x0, 0x0
	XSAVE_AREA_HEADERX<bits> Header; // 0x200, 0x200
};

template<size_t bits, typename T = FXSAVE_DEFAULT_PADDING, bool align = true>
struct XSAVE_AREAX;

template<size_t bits, typename T>
struct XSAVE_AREAX<bits, T, false> : XSAVE_AREAX_base<bits, T, false> {
	unsigned char extra[]; // 0x240, 0x240
};

template<size_t bits, typename T>
struct alignas(64) XSAVE_AREAX<bits, T, true> : XSAVE_AREAX_base<bits, T, true> {
	unsigned char extra[]; // 0x240, 0x240
};

ValidateStructSize(0x240, XSAVE_AREAX<32>);
ValidateStructAlignment(0x40, XSAVE_AREAX<32>);
ValidateStructSize(0x240, XSAVE_AREAX<64>);
ValidateStructAlignment(0x40, XSAVE_AREAX<64>);

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING, bool align = true>
struct XSTATE_CONTEXTX {
	uint64_t Mask; // 0x0, 0x0
	uint32_t Length; // 0x8, 0x8
	uint32_t Reserved1; // 0xC, 0xC
	PTRZX<64, XSAVE_AREAX<bits, T, align>> Area; // 0x10, 0x10
	PTRZX<64, void> Buffer; // 0x18, 0x18
	// 0x20, 0x20
};
ValidateStructSize(0x20, XSTATE_CONTEXTX<32>);
ValidateStructAlignment(0x8, XSTATE_CONTEXTX<32>);
ValidateStructSize(0x20, XSTATE_CONTEXTX<64>);
ValidateStructAlignment(0x8, XSTATE_CONTEXTX<64>);

template<size_t bits, typename T = FXSAVE_DEFAULT_PADDING>
struct CONTEXTX;

template<typename T>
struct CONTEXTX<32, T> {
	uint32_t ContextFlags; // 0x0
	uint32_t Dr0; // 0x4
	uint32_t Dr1; // 0x8
	uint32_t Dr2; // 0xC
	uint32_t Dr3; // 0x10
	uint32_t Dr6; // 0x14
	uint32_t Dr7; // 0x18
	FLOATING_SAVE_AREAX<32> FloatSave; // 0x1C
	uint32_t SegGs; // 0x8C
	uint32_t SegFs; // 0x90
	uint32_t SegEs; // 0x94
	uint32_t SegDs; // 0x98
	uint32_t Edi; // 0x9C
	uint32_t Esi; // 0xA0
	uint32_t Ebx; // 0xA4
	uint32_t Edx; // 0xA8
	uint32_t Ecx; // 0xAC
	uint32_t Eax; // 0xB0
	uint32_t Ebp; // 0xB4
	uint32_t Eip; // 0xB8
	uint32_t SegCs; // 0xBC
	uint32_t EFlags; // 0xC0
	uint32_t Esp; // 0xC4
	uint32_t SegSs; // 0xC8
	XSAVE_FORMATX<32, T, false> ExtendedRegisters; // 0xCC
	// 0x2CC
};
template<typename T>
struct alignas(16) CONTEXTX<64, T> {
	uint64_t P1Home; // 0x0
	uint64_t P2Home; // 0x8
	uint64_t P3Home; // 0x10
	uint64_t P4Home; // 0x18
	uint64_t P5Home; // 0x20
	uint64_t P6Home; // 0x28
	uint32_t ContextFlags; // 0x30
	MXCSR MxCsr; // 0x34
	uint16_t SegCs; // 0x38
	uint16_t SegDs; // 0x3A
	uint16_t SegEs; // 0x3C
	uint16_t SegFs; // 0x3E
	uint16_t SegGs; // 0x40
	uint16_t SegSs; // 0x42
	uint32_t EFlags; // 0x44
	uint64_t Dr0; // 0x48
	uint64_t Dr1; // 0x50
	uint64_t Dr2; // 0x58
	uint64_t Dr3; // 0x60
	uint64_t Dr6; // 0x68
	uint64_t Dr7; // 0x70
	uint64_t Rax; // 0x78
	uint64_t Rcx; // 0x80
	uint64_t Rdx; // 0x88
	uint64_t Rbx; // 0x90
	uint64_t Rsp; // 0x98
	uint64_t Rbp; // 0xA0
	uint64_t Rsi; // 0xA8
	uint64_t Rdi; // 0xB0
	uint64_t R8; // 0xB8
	uint64_t R9; // 0xC0
	uint64_t R10; // 0xC8
	uint64_t R11; // 0xD0
	uint64_t R12; // 0xD8
	uint64_t R13; // 0xE0
	uint64_t R14; // 0xE8
	uint64_t R15; // 0xF0
	uint64_t Rip; // 0xF8
	XSAVE_FORMATX<64, T, false> FltSave; // 0x100
	__m128 VectorRegister[26]; // 0x300
	uint64_t VectorControl; // 0x4A0
	uint64_t DebugControl; // 0x4A8
	uint64_t LastBranchToRip; // 0x4B0
	uint64_t LastBranchFromRip; // 0x4B8
	uint64_t LastExceptionToRip; // 0x4C0
	uint64_t LastExceptionFromRip; // 0x4C8
	// 0x4D0
};
ValidateStructSize(0x2CC, CONTEXTX<32>);
ValidateStructAlignment(0x4, CONTEXTX<32>);
ValidateStructSize(0x4D0, CONTEXTX<64>);
ValidateStructAlignment(0x10, CONTEXTX<64>);

template<size_t bits = native_bits>
struct XSTATE_CONFIGURATIONX {
	uint64_tx<bits> EnabledFeatures ide_packed_field; // 0x0
	uint32_t Size; // 0x8
	union {
		uint32_t ControlFlags; // 0xC
		struct {
			uint32_t OptimizedSave : 1;
		};
	};
	XSTATE_FEATURE Features[64]; // 0x10
	// 0x210
};
ValidateStructSize(0x210, XSTATE_CONFIGURATIONX<32>);
ValidateStructAlignment(0x4, XSTATE_CONFIGURATIONX<32>);
ValidateStructSize(0x210, XSTATE_CONFIGURATIONX<64>);
ValidateStructAlignment(0x8, XSTATE_CONFIGURATIONX<64>);

template<size_t bits = native_bits>
struct EXCEPTION_RECORDX {
	int32_t ExceptionCode; // 0x0, 0x0
	union {
		uint32_t ExceptionFlags; // 0x4, 0x4
		struct {
		};
	};
	PTRZX<bits, EXCEPTION_RECORDX<bits>> ExceptionRecord; // 0x8, 0x8
	PTRZX<bits> ExceptionAddress; // 0xC, 0x10
	uint32_t NumberParameters; // 0x10, 0x18
	__x64_padding(0x4);
	uintptr_tx<bits> ExceptionInformation[15]; // 0x14, 0x20
	// 0x50, 0x98
};
ValidateStructSize(0x50, EXCEPTION_RECORDX<32>);
ValidateStructAlignment(0x4, EXCEPTION_RECORDX<32>);
ValidateStructSize(0x98, EXCEPTION_RECORDX<64>);
ValidateStructAlignment(0x8, EXCEPTION_RECORDX<64>);

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
using EXCEPTION_HANDLER_FUNCX = EXCEPTION_DISPOSITION(EXCEPTION_RECORDX<bits>,
													  PTRZX<bits>,
													  PTRZX<bits, CONTEXTX<bits, T>>,
													  PTRZX<bits>
													  );

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct EXCEPTION_REGISTRATION_RECORDX {
	PTRZX<bits, EXCEPTION_REGISTRATION_RECORDX<bits, T>> Next; // 0x0, 0x0
	PTRZX<bits, EXCEPTION_HANDLER_FUNCX<bits, T>> Handler; // 0x4, 0x8
	// 0x8, 0x10
};
ValidateStructSize(0x8, EXCEPTION_REGISTRATION_RECORDX<32>);
ValidateStructAlignment(0x4, EXCEPTION_REGISTRATION_RECORDX<32>);
ValidateStructSize(0x10, EXCEPTION_REGISTRATION_RECORDX<64>);
ValidateStructAlignment(0x8, EXCEPTION_REGISTRATION_RECORDX<64>);

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct NT_TIBX {
	PTRZX<bits, EXCEPTION_REGISTRATION_RECORDX<bits, T>> ExceptionList; // 0x0, 0x0
	PTRZX<bits> StackBase; // 0x4, 0x8
	PTRZX<bits> StackLimit; // 0x8, 0x10
	PTRZX<bits> SubSystemTib; // 0xC, 0x18
	union {
		PTRZX<bits> FiberData; // 0x10, 0x20
		struct {
			uint32_t Verison; // 0x10, 0x20
			__x64_padding(0x4);
		};
	};
	PTRZX<bits> ArbitraryUserPointer; // 0x14, 0x28
	PTRZX<bits, NT_TIBX<bits, T>> Self; // 0x18, 0x30
	// 0x1C, 0x38
};
ValidateStructSize(0x1C, NT_TIBX<32>);
ValidateStructAlignment(0x4, NT_TIBX<32>);
ValidateStructSize(0x38, NT_TIBX<64>);
ValidateStructAlignment(0x8, NT_TIBX<64>);

template<size_t bits = native_bits>
struct CLIENT_IDX {
	HANDLEX<bits> UniqueProcess; // 0x0, 0x0
	HANDLEX<bits> UniqueThread; // 0x4, 0x8
	// 0x8, 0x10
};
ValidateStructSize(0x8, CLIENT_IDX<32>);
ValidateStructAlignment(0x4, CLIENT_IDX<32>);
ValidateStructSize(0x10, CLIENT_IDX<64>);
ValidateStructAlignment(0x8, CLIENT_IDX<64>);

#define GDI_BATCH_BUFFER_SIZE 310
template<size_t bits = native_bits>
struct GDI_TEB_BATCHX {
	uint32_t Offset; // 0x0, 0x0
	__x64_padding(0x4);
	PTRZX<bits, UNKNOWN_TYPE> HDC; // 0x4, 0x8
	uint32_t Buffer[GDI_BATCH_BUFFER_SIZE]; // 0x8, 0x10
	// 0x4E0, 0x4E8
};
ValidateStructSize(0x4E0, GDI_TEB_BATCHX<32>);
ValidateStructAlignment(0x4, GDI_TEB_BATCHX<32>);
ValidateStructSize(0x4E8, GDI_TEB_BATCHX<64>);
ValidateStructAlignment(0x8, GDI_TEB_BATCHX<64>);

template<size_t bits = native_bits>
struct TEB_ACTIVE_FRAME_CONTEXTX {
	union {
		uint32_t Flags; // 0x0, 0x0
		struct {
		};
	};
	__x64_padding(0x4);
	PTRZX<bits, const char> FrameName; // 0x4, 0x8
	// 0x8, 0x10
};
ValidateStructSize(0x8, TEB_ACTIVE_FRAME_CONTEXTX<32>);
ValidateStructAlignment(0x4, TEB_ACTIVE_FRAME_CONTEXTX<32>);
ValidateStructSize(0x10, TEB_ACTIVE_FRAME_CONTEXTX<64>);
ValidateStructAlignment(0x8, TEB_ACTIVE_FRAME_CONTEXTX<64>);

template<size_t bits = native_bits>
struct TEB_ACTIVE_FRAME_CONTEXT_EXX {
	TEB_ACTIVE_FRAME_CONTEXTX<bits> BasicContext; // 0x0, 0x0
	PTRZX<bits, const char> SourceLocation; // 0x8, 0x10
	// 0xC, 0x18
};
ValidateStructSize(0xC, TEB_ACTIVE_FRAME_CONTEXT_EXX<32>);
ValidateStructAlignment(0x4, TEB_ACTIVE_FRAME_CONTEXT_EXX<32>);
ValidateStructSize(0x18, TEB_ACTIVE_FRAME_CONTEXT_EXX<64>);
ValidateStructAlignment(0x8, TEB_ACTIVE_FRAME_CONTEXT_EXX<64>);

template<size_t bits = native_bits>
struct TEB_ACTIVE_FRAMEX {
	union {
		uint32_t Flags; // 0x0, 0x0
		struct {
		};
	};
	__x64_padding(0x4);
	PTRZX<bits, TEB_ACTIVE_FRAMEX<bits>> Previous; // 0x4, 0x8
	PTRZX<bits, const TEB_ACTIVE_FRAME_CONTEXTX<bits>> Context; // 0x8, 0x10
	// 0xC, 0x18
};
ValidateStructSize(0xC, TEB_ACTIVE_FRAMEX<32>);
ValidateStructAlignment(0x4, TEB_ACTIVE_FRAMEX<32>);
ValidateStructSize(0x18, TEB_ACTIVE_FRAMEX<64>);
ValidateStructAlignment(0x8, TEB_ACTIVE_FRAMEX<64>);

template<size_t bits = native_bits>
struct TEB_ACTIVE_FRAME_EXX {
	TEB_ACTIVE_FRAMEX<bits> BasicFrame; // 0x0, 0x0
	PTRZX<bits> ExtensionIdentifier; // 0xC, 0x18
	// 0x10, 0x20
};
ValidateStructSize(0x10, TEB_ACTIVE_FRAME_EXX<32>);
ValidateStructAlignment(0x4, TEB_ACTIVE_FRAME_EXX<32>);
ValidateStructSize(0x20, TEB_ACTIVE_FRAME_EXX<64>);
ValidateStructAlignment(0x8, TEB_ACTIVE_FRAME_EXX<64>);

template<typename T = FXSAVE_DEFAULT_PADDING>
struct WX86TIB {
	uint32_t Size; // 0x0, 0x0
	uint32_t InitialiPc; // 0x4, 0x4
	PTRZX<32> StackBase; // 0x8, 0x8
	PTRZX<32> StackLimit;  // 0xC, 0xC
	PTRZX<32> DeallocationStack; // 0x10, 0x10
	union {
		uint32_t LogFlags; // 0x14, 0x14
		struct {
		};
	};
	uint32_t InitialSp; // 0x18, 0x18
	uint8_t SimulationCount; // 0x1C, 0x1C
	bool InCpuSimulation; // 0x1D, 0x1D
	bool EmulateInitialPc; // 0x1E, 0x1E
	bool Initialized; // 0x1F, 0x1F
	PTRZX<32, EXCEPTION_REGISTRATION_RECORDX<32, T>> ExceptionList; // 0x20, 0x20
	PTRZX<32, UNKNOWN_TYPE> CpuContext; // 0x24, 0x24
	PTRZX<32, CONTEXTX<32, T>> InitialExceptionContext; // 0x28, 0x28
	PTRZX<32, UNKNOWN_TYPE> pCallersRIID; // 0x2C, 0x2C
	PTRZX<32, UNKNOWN_TYPE> pCallersUnknown; // 0x30, 0x30
	union {
		uint32_t Flags; // 0x34, 0x34
		struct {
		};
	};
	PTRZX<32, UNKNOWN_TYPE> SelfRegDllName; // 0x38, 0x38
	PTRZX<32, UNKNOWN_TYPE> SelfRegDllHandle; // 0x3C, 0x3C
	// 0x40, 0x40
};
ValidateStructSize(0x40, WX86TIB<>);
ValidateStructAlignment(0x4, WX86TIB<>);

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
using WX86TIBX = WX86TIB<T>;

/*========================================
	TEB
========================================*/

#define STATIC_UNICODE_BUFFER_LENGTH 261
#define WIN32_CLIENT_INFO_LENGTH 62
template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct TEBX;

template<typename T>
struct TEBX<32, T> {
	union {
		NT_TIBX<32, T> NtTib; // 0x0
	};
	PTRZX<32, UNKNOWN_TYPE> EnvironmentPointer; // 0x1C
	CLIENT_IDX<32> ClientId; // 0x20
	PTRZX<32, UNKNOWN_TYPE> ActiveRpcHandle; // 0x28
	PTRZX<32, UNKNOWN_TYPE> ThreadLocalStoragePointer; // 0x2C
	PTRZX<32, PEBX<32>> ProcessEnvironmentBlock; // 0x30
	uint32_t LastErrorValue; // 0x34
	uint32_t CountOfOwnedCriticalSections; // 0x38
	PTRZX<32, UNKNOWN_TYPE> CsrClientThread; // 0x3C
	PTRZX<32, UNKNOWN_TYPE> Win32ThreadInfo; // 0x40 (PtiCurrent)
	uint32_t User32Reserved[26]; // 0x44 (user32.dll items)
	uint32_t UserReserved[5]; // 0xAC (Winsrv SwitchStack)
	union {
		PTRZX<32, UNKNOWN_TYPE> WOW32Reserved; // 0xC0
	};
	LCID CurrentLocale; // 0xC4
	uint32_t FpSoftwareStatusRegister; // 0xC8
	PTRZX<32, UNKNOWN_TYPE> SystemReserved1[54]; // 0xCC (Used by FP emulator)
	NTSTATUS ExceptionCode; // 0x1A4
	PTRZX<32, ACTIVATION_CONTEXT_STACKX<32>> ActivationContextStackPointer; // 0x1A8
	uint8_t SpareBytes1[36]; // 0x1C
	uint32_t TxFsContext; // 0x1D0
	GDI_TEB_BATCHX<32> GdiTebBatch; // 0x1D4
	CLIENT_IDX<32> RealClientId; // 0x6B4
	HANDLEX<32> GdiCachedProcessHandle; // 0x6BC
	uint32_t GdiClientPID; // 0x6C0
	uint32_t GdiClientTID; // 0x6C4
	PTRZX<32, UNKNOWN_TYPE> GdiThreadLocalInfo; // 0x6C8
	uintptr_tx<32> Win32ClientInfo[WIN32_CLIENT_INFO_LENGTH]; // 0x6CC
	PTRZX<32, UNKNOWN_TYPE> glDispatchTable[233]; // 0x7C4
	uintptr_tx<32> glReserved1[29]; // 0xB68
	PTRZX<32, UNKNOWN_TYPE> glReserved2; // 0xBDC
	PTRZX<32, UNKNOWN_TYPE> glSectionInfo; // 0xBE0
	PTRZX<32, UNKNOWN_TYPE> glSection; // 0xBE4
	PTRZX<32, UNKNOWN_TYPE> glTable; // 0xBE8
	PTRZX<32, UNKNOWN_TYPE> glCurrentRC; // 0xBEC
	PTRZX<32, UNKNOWN_TYPE> glContext; // 0xBF0
	uint32_t LastStatusValue; // 0xBF4
	UNICODE_STRINGX<32> StaticUnicodeString; // 0xBF8
	wchar_t StaticUnicodeBuffer[STATIC_UNICODE_BUFFER_LENGTH]; // 0xC00
	PTRZX<32, UNKNOWN_TYPE> DeallocationStack; // 0xE0C
	PTRZX<32, UNKNOWN_TYPE> TlsSlots[TLS_MINIMUM_AVAILABLE]; // 0xE10
	LIST_ENTRYX<32, UNKNOWN_TYPE> TlsLinks; // 0xF10
	PTRZX<32, WX86TIBX<32, T>> Vdm; // 0xF18
	PTRZX<32, UNKNOWN_TYPE> ReservedForNtRpc; // 0xF1C
	HANDLEX<32> DbgSsReserved[2]; // 0xF20
	uint32_t HardErrorMode; // 0xF28
	PTRZX<32, UNKNOWN_TYPE> Instrumentation[9]; // 0xF2C
	GUID ActivityId; // 0xF50
	PTRZX<32, UNKNOWN_TYPE> SubProcessTag; // 0xF60
	PTRZX<32, UNKNOWN_TYPE> EtwLocalData; // 0xF64
	PTRZX<32, UNKNOWN_TYPE> EtwTraceData; // 0xF68
	PTRZX<32, UNKNOWN_TYPE> WinSockData; // 0xF6C
	uint32_t GdiBatchCount; // 0xF70
	union {
		PROCESSOR_NUMBER CurrentIdealProcessor; // 0xF74
		uint32_t IdealProcessorValue; // 0xF74
		struct {
			uint8_t ReservedPad0; // 0xF74
			uint8_t ReservedPad1; // 0xF75
			uint8_t ReservedPad2; // 0xF76
			uint8_t IdealProcessor; // 0xF77
		};
	};
	uint32_t GuaranteedStackBytes; // 0xF78
	PTRZX<32, UNKNOWN_TYPE> ReservedForPerf; // 0xF7C
	PTRZX<32, UNKNOWN_TYPE> ReservedForOle; // 0xF80
	uint32_t WaitingOnLoaderLock; // 0xF84
	PTRZX<32, UNKNOWN_TYPE> SavedPriorityState; // 0xF88
	uintptr_tx<32> SoftPatchPtr1; // 0xF8C
	PTRZX<32, UNKNOWN_TYPE> ThreadPoolData; // 0xF90
	PTRZX<32, PTRZX<32, UNKNOWN_TYPE>> TlsExpansionSlots; // 0xF94
	uint32_t MuiGeneration; // 0xF98
	uint32_t IsImpersonating; // 0xF9C
	PTRZX<32, UNKNOWN_TYPE> NlsCache; // 0xFA0
	PTRZX<32, UNKNOWN_TYPE> pShimData; // 0xFA4
	uint32_t HeapVirtualAffinity; // 0xFA8
	HANDLEX<32> CurrentTransactionHandle; // 0xFAC
	PTRZX<32, TEB_ACTIVE_FRAMEX<32>> ActiveFrame; // 0xFB0
	PTRZX<32, UNKNOWN_TYPE> FlsData; // 0xFB4
	PTRZX<32, UNKNOWN_TYPE> PreferredLanguages; // 0xFB8
	PTRZX<32, UNKNOWN_TYPE> UserPrefLanguages; // 0xFBC
	PTRZX<32, UNKNOWN_TYPE> MergedPrefLanguages; // 0xFC0
	uint32_t MuiImpersonation; // 0xFC4
	union {
		volatile uint16_t CrossTebFlags; // 0xFC8
		struct {
			uint16_t SpareCrossTebBits : 16;
		};
	};
	union {
		uint16_t SameTebFlags; // 0xFCA
		struct {
			uint16_t SafeThunkCall : 1;
			uint16_t InDebugPrint : 1;
			uint16_t HasFiberData : 1;
			uint16_t SkipThreadAttach : 1;
			uint16_t WerInShipAssertCode : 1;
			uint16_t RanProcessInit : 1;
			uint16_t ClonedThread : 1;
			uint16_t SuppressDebugMsg : 1;
			uint16_t DisableUserStackWalk : 1;
			uint16_t RtlExceptionAttached : 1;
			uint16_t InitialThread : 1;
			uint16_t SpareSameTebBits : 5;
		};
	};
	PTRZX<32, UNKNOWN_TYPE> TxnScopeEnterCallback; // 0xFCC
	PTRZX<32, UNKNOWN_TYPE> TxnScopeExitCallback; // 0xFD0
	PTRZX<32, UNKNOWN_TYPE> TxnScopeContext; // 0xFD4
	uint32_t LockCount; // 0xFD8
	uint32_t SpareUlong0; // 0xFDC
	PTRZX<32, UNKNOWN_TYPE> ResourceRetValue; // 0xFE0
	// 0xFE4
};

template<typename T>
struct TEBX<64, T> {
	union {
		NT_TIBX<64, T> NtTib; // 0x0
		struct {
			PTRZX<32, TEBX<32, T>> Wow64Teb32; // 0x0
		};
	};
	PTRZX<64, UNKNOWN_TYPE> EnvironmentPointer; // 0x38
	CLIENT_IDX<64> ClientId; // 0x40
	PTRZX<64, UNKNOWN_TYPE> ActiveRpcHandle; // 0x50
	PTRZX<64, UNKNOWN_TYPE> ThreadLocalStoragePointer; // 0x58
	PTRZX<64, PEBX<64>> ProcessEnvironmentBlock; // 0x60
	uint32_t LastErrorValue; // 0x68
	uint32_t CountOfOwnedCriticalSections; // 0x6C
	PTRZX<64, UNKNOWN_TYPE> CsrClientThread; // 0x70
	PTRZX<64, UNKNOWN_TYPE> Win32ThreadInfo; // 0x78 (PtiCurrent)
	uint32_t User32Reserved[26]; // 0x80 (user32.dll items)
	uint32_t UserReserved[5]; // 0xE8 (Winsrv SwitchStack)
	__x64_padding(0x4);
	PTRZX<64, UNKNOWN_TYPE> WOW32Reserved; // 0x100
	LCID CurrentLocale; // 0x108
	uint32_t FpSoftwareStatusRegister; // 0x10C
	PTRZX<64, UNKNOWN_TYPE> SystemReserved1[54]; // 0x110 (Used by FP emulator)
	NTSTATUS ExceptionCode; // 0x2C0
	__x64_padding(0x4);
	PTRZX<64, ACTIVATION_CONTEXT_STACKX<64>> ActivationContextStackPointer; // 0x2C8
	UCHAR SpareBytes1[24]; // 0x2D0
	uint32_t TxFsContext; // 0x2E8
	__x64_padding(0x4);
	GDI_TEB_BATCHX<64> GdiTebBatch; // 0x2F0
	CLIENT_IDX<64> RealClientId; // 0x7D8
	HANDLEX<64> GdiCachedProcessHandle; // 0x7E8
	uint32_t GdiClientPID; // 0x7F0
	uint32_t GdiClientTID; // 0x7F4
	PTRZX<64, UNKNOWN_TYPE> GdiThreadLocalInfo; // 0x7F8
	uintptr_tx<64> Win32ClientInfo[WIN32_CLIENT_INFO_LENGTH]; // 0x800
	PTRZX<64, UNKNOWN_TYPE> glDispatchTable[233]; // 0x9F0
	uintptr_tx<64> glReserved1[29]; // 0x1138
	PTRZX<64, UNKNOWN_TYPE> glReserved2; // 0x1220
	PTRZX<64, UNKNOWN_TYPE> glSectionInfo; // 0x1228
	PTRZX<64, UNKNOWN_TYPE> glSection; // 0x1230
	PTRZX<64, UNKNOWN_TYPE> glTable; // 0x1238
	PTRZX<64, UNKNOWN_TYPE> glCurrentRC; // 0x1240
	PTRZX<64, UNKNOWN_TYPE> glContext; // 0x1248
	uint32_t LastStatusValue; // 0x1250
	__x64_padding(0x4);
	UNICODE_STRINGX<64> StaticUnicodeString; // 0x1258
	wchar_t StaticUnicodeBuffer[STATIC_UNICODE_BUFFER_LENGTH]; // 0x1268
	__x64_padding(0x6);
	PTRZX<64, UNKNOWN_TYPE> DeallocationStack; // 0x1478
	PTRZX<64, UNKNOWN_TYPE> TlsSlots[TLS_MINIMUM_AVAILABLE]; // 0x1480
	LIST_ENTRYX<64, UNKNOWN_TYPE> TlsLinks; // 0x1680
	PTRZX<64, WX86TIBX<64, T>> Vdm; // 0x1690
	PTRZX<64, UNKNOWN_TYPE> ReservedForNtRpc; // 0x1698
	HANDLEX<64> DbgSsReserved[2]; // 0x16A0
	uint32_t HardErrorMode; // 0x16B0
	__x64_padding(0x4);
	PTRZX<64, UNKNOWN_TYPE> Instrumentation[11]; // 0x16B8
	GUID ActivityId; // 0x1710
	PTRZX<64, UNKNOWN_TYPE> SubProcessTag; // 0x1720
	PTRZX<64, UNKNOWN_TYPE> EtwLocalData; // 0x1728
	PTRZX<64, UNKNOWN_TYPE> EtwTraceData; // 0x1730
	PTRZX<64, UNKNOWN_TYPE> WinSockData; // 0x1738
	uint32_t GdiBatchCount; // 0x1740
	union {
		PROCESSOR_NUMBER CurrentIdealProcessor; // 0x1744
		uint32_t IdealProcessorValue; // 0x1744
		struct {
			uint8_t ReservedPad0; // 0x1744
			uint8_t ReservedPad1; // 0x1745
			uint8_t ReservedPad2; // 0x1746
			uint8_t IdealProcessor; // 0x1747
		};
	};
	uint32_t GuaranteedStackBytes; // 0x1748
	__x64_padding(0x4);
	PTRZX<64, UNKNOWN_TYPE> ReservedForPerf; // 0x1750
	PTRZX<64, UNKNOWN_TYPE> ReservedForOle; // 0x1758
	uint32_t WaitingOnLoaderLock; // 0x1760
	__x64_padding(0x4);
	PTRZX<64, UNKNOWN_TYPE> SavedPriorityState; // 0x1768
	uintptr_tx<64> SoftPatchPtr1; // 0x1770
	PTRZX<64, UNKNOWN_TYPE> ThreadPoolData; // 0x1778
	PTRZX<64, PTRZX<64, UNKNOWN_TYPE>> TlsExpansionSlots; // 0x1780
	PTRZX<64, UNKNOWN_TYPE> DeallocationBStore; // 0x1788
	PTRZX<64, UNKNOWN_TYPE> BStoreLimit; // 0x1790
	uint32_t MuiGeneration; // 0x1798
	uint32_t IsImpersonating; // 0x179C
	PTRZX<64, UNKNOWN_TYPE> NlsCache; // 0x17A0
	PTRZX<64, UNKNOWN_TYPE> pShimData; // 0x17A8
	uint32_t HeapVirtualAffinity; // 0x17B0
	__x64_padding(0x4); // 0x17B4
	HANDLEX<64> CurrentTransactionHandle; // 0x17B8
	PTRZX<64, TEB_ACTIVE_FRAMEX<64>> ActiveFrame; // 0x17C0
	PTRZX<64, UNKNOWN_TYPE> FlsData; // 0x17C8
	PTRZX<64, UNKNOWN_TYPE> PreferredLanguages; // 0x17D0
	PTRZX<64, UNKNOWN_TYPE> UserPrefLanguages; // 0x17D8
	PTRZX<64, UNKNOWN_TYPE> MergedPrefLanguages; // 0x17E0
	uint32_t MuiImpersonation; // 0x17E8
	union {
		volatile uint16_t CrossTebFlags; // 0x17EC
		struct {
			uint16_t SpareCrossTebBits : 16;
		};
	};
	union {
		uint16_t SameTebFlags; // 0x17EE
		struct {
			uint16_t SafeThunkCall : 1;
			uint16_t InDebugPrint : 1;
			uint16_t HasFiberData : 1;
			uint16_t SkipThreadAttach : 1;
			uint16_t WerInShipAssertCode : 1;
			uint16_t RanProcessInit : 1;
			uint16_t ClonedThread : 1;
			uint16_t SuppressDebugMsg : 1;
			uint16_t DisableUserStackWalk : 1;
			uint16_t RtlExceptionAttached : 1;
			uint16_t InitialThread : 1;
			uint16_t SpareSameTebBits : 5;
		};
	};
	PTRZX<64, UNKNOWN_TYPE> TxnScopeEnterCallback; // 0x17F0
	PTRZX<64, UNKNOWN_TYPE> TxnScopeExitCallback; // 0x17F8
	PTRZX<64, UNKNOWN_TYPE> TxnScopeContext; // 0x1800
	uint32_t LockCount; // 0x1808
	uint32_t SpareUlong0; // 0x180C
	PTRZX<64, UNKNOWN_TYPE> ResourceRetValue; // 0x1810
	// 0x1818
};
ValidateStructSize(0xFE4, TEBX<32>);
ValidateStructAlignment(0x4, TEBX<32>);
ValidateStructSize(0x1818, TEBX<64>);
ValidateStructAlignment(0x8, TEBX<64>);


static inline constexpr auto teb32 = (TEBX<32> FS_RELATIVE*)0;
static inline constexpr auto teb64 = (TEBX<64> GS_RELATIVE*)0;

template<size_t bits = native_bits>
struct PROCESS_BASIC_INFORMATIONX {
	NTSTATUS ExitStatus; // 0x0, 0x0
	__x64_padding(0x4);
	PTRZX<bits, PEBX<bits>> PebBaseAddress; // 0x4, 0x8
	uintptr_tx<bits> AffinityMask; // 0x8, 0x10
	KPRIORITY BasePriority; // 0xC, 0x18
	__x64_padding(0x4);
	uintptr_tx<bits> UniqueProcessId; // 0x10, 0x20
	uintptr_tx<bits> InheritedFromUniqueProcessId; // 0x14, 0x28
	// 0x18, 0x30
};
ValidateStructSize(0x18, PROCESS_BASIC_INFORMATIONX<32>);
ValidateStructAlignment(0x4, PROCESS_BASIC_INFORMATIONX<32>);
ValidateStructSize(0x30, PROCESS_BASIC_INFORMATIONX<64>);
ValidateStructAlignment(0x8, PROCESS_BASIC_INFORMATIONX<64>);

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct THREAD_BASIC_INFORMATIONX {
	NTSTATUS ExitStatus; // 0x0, 0x0
	__x64_padding(0x4);
	PTRZX<bits, TEBX<bits, T>> TebBaseAddress; // 0x4, 0x8
	CLIENT_IDX<bits> ClientId; // 0x8, 0x10
	uintptr_tx<bits> AffinityMask; // 0x10, 0x20
	KPRIORITY Priority; // 0x14, 0x28
	int32_t BasePriority; // 0x18, 0x2C
	// 0x1C, 0x30
};
ValidateStructSize(0x1C, THREAD_BASIC_INFORMATIONX<32>);
ValidateStructAlignment(0x4, THREAD_BASIC_INFORMATIONX<32>);
ValidateStructSize(0x30, THREAD_BASIC_INFORMATIONX<64>);
ValidateStructAlignment(0x8, THREAD_BASIC_INFORMATIONX<64>);

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct FIBERX {
	PTRZX<bits, EXCEPTION_REGISTRATION_RECORDX<bits, T>> ExceptionList; // 0x0, 0x0
	PTRZX<bits> StackBase; // 0x4, 0x8
	PTRZX<bits> StackLimit; // 0x8, 0x10
	PTRZX<bits, UNKNOWN_TYPE> DeallocationStack; // 0xC, 0x18
	CONTEXTX<bits, T> FiberContext; // 0x10, 0x20
	PTRZX<bits, WX86TIBX<bits, T>> Wx86Tib; // 0x2DC, 0x4F0
	PTRZX<bits, UNKNOWN_TYPE> FlsData; // 0x2E0, 0x4F8
	// 0x2E4, 0x500
};
// TODO: Stop fixing asserts by commenting them out
ValidateStructSize(0x2E4, FIBERX<32>);
ValidateStructAlignment(0x4, FIBERX<32>);
ValidateStructSize(0x500, FIBERX<64>);
ValidateStructAlignment(0x10, FIBERX<64>);

enum PS_CREATE_STATEX : int32_t {
	PsCreateInitialState = 0,
	PsCreateFailOnFileOpen = 1,
	PsCreateFailOnSectionOpen = 2,
	PsCreateFailExeFormat = 3,
	PsCreateFailMachineMismatch = 4,
	PsCreateFailExeName = 5,
	PsCreateSuccess = 6,
	PsCreateMaximumStates = 7
};

template<size_t bits = native_bits>
struct PS_CREATE_INFOX {
	uintptr_tx<bits> Size; // 0x0, 0x0
	PS_CREATE_STATEX State; // 0x4, 0x8
	__x64_padding(0x4);
	union {
		struct {
			union {
				uint32_t InitFlags; // 0x8, 0x10
				struct {
					uint8_t WriteOutputOnExit : 1;
					uint8_t DetectManifest : 1;
					uint8_t SpareBits1 : 6;
					uint8_t IFEOKeyState : 2;
					uint8_t SpareBits2 : 6;
					uint16_t ProhibitedImageCharacteristics : 16;
				};
			};
			ACCESS_MASK AdditionalFileAccess; // 0xC, 0x14
			// 0x10, 0x18
		} InitState;
		struct {
			HANDLEX<bits> FileHandle; // 0x8, 0x10
			// 0xC, 0x18
		} FailSection;
		struct {
			uint16_t DllCharacteristics; // 0x8, 0x10
			// 0xA, 0x12
		} ExeFormat;
		struct {
			HANDLEX<bits> IFEOKey; // 0x8, 0x10
			__x86_padding(0x4);
			// 0x10, 0x18
		} ExeName;
		struct {
			union {
				uint32_t OutputFlags; // 0x8, 0x10
				struct {
					uint8_t ProtectedProcess : 1;
					uint8_t AddressSpaceOverride : 1;
					uint8_t DevOverrideEnabled : 1;
					uint8_t ManifestDetected : 1;
					uint8_t SpareBits1 : 4;
					uint8_t SpareBits2 : 8;
					uint16_t SpareBits3 : 16;
				};
			};
			__x64_padding(0x4);
			HANDLEX<bits> FileHandle; // 0xC, 0x18
			HANDLEX<bits> SectionHandle; // 0x10, 0x20
			__x86_padding(0x4);
			PTR64Z<RTL_USER_PROCESS_PARAMETERSX<bits>> UserProcessParametersNative; // 0x18, 0x28
			PTR32Z<RTL_USER_PROCESS_PARAMETERSX<32>> UserProcessParametersWow64; // 0x20, 0x30
			union {
				uint32_t CurrentParameterFlags; // 0x24, 0x34
				struct {
				};
			};
			PTR64Z<PEBX<bits>> PebAddressNative; // 0x28, 0x38
			PTR32Z<PEBX<32>> PebAddressWow64; // 0x30, 0x40
			__padding(0x4);
			PTR64Z<UNKNOWN_TYPE> ManifestAddress; // 0x38, 0x48
			uint32_t ManifestSize; // 0x40, 0x50
			__padding(0x4);
			// 0x48, 0x58
		} SuccessState;
	};
	// 0x48, 0x58
};
ValidateStructSize(0x48, PS_CREATE_INFOX<32>);
ValidateStructAlignment(0x8, PS_CREATE_INFOX<32>);
ValidateStructSize(0x58, PS_CREATE_INFOX<64>);
ValidateStructAlignment(0x8, PS_CREATE_INFOX<64>);

enum NT_PRODUCT_TYPE : int32_t {
	NtProductWinNt = 1,
	NtProductLanManNt = 2,
	NtProductServer = 3
};

enum ALTERNATIVE_ARCHITECTURE_TYPE : int32_t {
	StandardDesign = 0,
	NEC98x86 = 1,
	EndAlternatives = 2
};

struct ETW_UMGL_KEY {
	uint8_t LoggerId; // 0x0
	union {
		uint8_t Flags; // 0x1
		struct {
		};
	};
	// 0x2
};
ValidateStructSize(0x2, ETW_UMGL_KEY);
ValidateStructAlignment(0x1, ETW_UMGL_KEY);

/*
    KUSER_SHARED_DATA Padding list:
    4 bytes at 0x0 (since Srv2003)
    1 byte at 0x269
    2 bytes at 0x2EE
    4 bytes at 0x2F4
    16 bytes at 0x310
    4 bytes at 0x334
    8 bytes at 0x3A8
    4 bytes at 0x3DC
*/

template<size_t bits = native_bits>
struct KUSER_SHARED_DATAX_base {
	volatile uint32_t TickCountLowDeprecated; // 0x0
	uint32_t TickCountMultiplier; // 0x4
	volatile KSYSTEM_TIMEX<bits> InterruptTime; // 0x8
	volatile KSYSTEM_TIMEX<bits> SystemTime; // 0x14
	volatile KSYSTEM_TIMEX<bits> TimeZoneBias; // 0x20
	uint16_t ImageNumberLow; // 0x2C
	uint16_t ImageNumberHigh; // 0x2E
	wchar_t NtSystemRoot[MAX_PATH]; // 0x30
	uint32_t MaxStackTraceDepth; // 0x238
	uint32_t CryptoExponent; // 0x23C
	uint32_t TimeZoneId; // 0x240
	uint32_t LargePageMinimum; // 0x244

	// Begin Windows 8+
	uint32_t AitSamplingValueNew; // 0x248
	uint32_t AppCompatFlagNew; // 0x24C
	uint64_t RNGSeedVersion; // 0x250
	uint32_t GlobalValidationRunLevel; // 0x258
	volatile int32_t TimeZoneBiasStamp; // 0x25C
	// End Windows 8+

	// Begin Windows 10+
	uint32_t NtBuildNumber; // 0x260
	// End Windows 10+
	
	NT_PRODUCT_TYPE NtProductType; // 0x264
	bool ProductTypeIsValid; // 0x268

	// Begin Windows 8+
	bool Reserved0[1]; // 0x269
	uint16_t NativeProcessorArchitecture; // 0x26A
	// End Windows 8+
	
	uint32_t NtMajorVersion; // 0x26C
	uint32_t NtMinorVersion; // 0x270
	bool ProcessorFeatures[0x40]; // 0x274

	// Begin Windows NT 4 specific (Still have values in later versions, but are almost useless with only 32 bits each)
	uint32_t MmHighestUserAddress; // 0x2B4
	uint32_t MmSystemRangeStart; // 0x2B8
	// End Windows NT 4 specific

	volatile uint32_t TimeSlip; // 0x2BC, 0x2BC
	ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture; // 0x2C0

	// Begin Windows 10+
	uint32_t BootId; // 0x2C4
	// End Windows 10+

	LARGE_INTEGERX<bits> SystemExpirationDate; // 0x2C8
	uint32_t SuiteMask; // 0x2D0
	uint8_t KdDebuggerEnabled; // 0x2D4 (Supposedly this is two bit flags?)
	union {
		uint8_t MitigationPolicies; // 0x2D5
		struct {
			uint8_t NXSupportPolicy : 2;
			// Begin Windows 8+
			uint8_t SEHValidationPolicy : 2;
			uint8_t CurDirDevicesSkippedForDlls : 2;
			uint8_t Reserved : 2;
			// End Windows 8+
		};
	};

	// Begin Windows 10+
	uint16_t CyclesPerYield; // 0x2D6
	// End Windows 10+

	volatile uint32_t ActiveConsoleId; // 0x2D8
	volatile uint32_t DismountCount; // 0x2DC
	uint32_t ComPlusPackage; // 0x2E0
	uint32_t LastSystemRITEventTickCount; // 0x2E4
	uint32_t NumberOfPhysicalPages; // 0x2E8
	bool SafeBootMode; // 0x2EC

	union {
		// Begin Windows 7 specific
		uint8_t TscQpcData; // 0x2ED
		struct {
			uint8_t TscQpcEnabled : 1;
			uint8_t TscQpcSpareFlag : 1;
			uint8_t TscQpcShift : 6;
		};
		// End Windows 7
		// Begin Windows 10+
		uint8_t VirtualizationFlags; // 0x2ED
		// End Windows 10+
	};

	uint8_t Reserved12[2]; // 0x2EE
	union {
		uint32_t SharedDataFlags; // 0x2F0
		struct {
			uint32_t DbgErrorPortPresent : 1;
			uint32_t DbgElevationEnabled : 1;
			uint32_t DbgVirtEnabled : 1;
			uint32_t DbgInstallerDetectEnabled : 1;
			uint32_t DbgSystemDllRelocated : 1; // (DbgLkgEnabled)
			uint32_t DbgDynProcessorEnabled : 1;
			uint32_t DbgSEHValidationEnabled : 1; // (DbgConsoleBrokerEnabled)
			// Begin Windows 8+
			uint32_t DbgSecureBootEnabled : 1;
			// End Windows 8+
			// Begin Windows 10+
			uint32_t DbgMultiSessionSku : 1;
			uint32_t DbgMultiUsersInSessionSku : 1;
			uint32_t DbgStateSeparationEnabled : 1;
			// End Windows 10+
			uint32_t SpareBits : 21;
		};
	};
	uint32_t DataFlagsPad[1]; // 0x2F4

	uint64_tx<bits> TestRetInstruction; // 0x2F8
	uint32_t SystemCall; // 0x300
	uint32_t SystemCallReturn; // 0x304

	// Begin Windows 10+
	uint32_t SystemCallNew; // 0x308
	union {
		uint32_t AllFlags; // 0x30C
		struct {
			uint32_t Win32Process : 1;
			uint32_t Sgx2Enclave : 1;
			uint32_t VbsBasicEnclave : 1;
			uint32_t SpareBits : 29;
		};
	} UserCetAvailableEnvironments;
	// End Windows 10+

	uint64_tx<bits> SystemCallPad[2]; // 0x310
	union {
		volatile KSYSTEM_TIMEX<bits> TickCount; // 0x320
		volatile uint64_tx<bits> TickCountQuad; // 0x320
		struct {
			uint32_t ReservedTickCountOverlay[3]; // 0x320
			uint32_t TickCountPad[1]; // 0x32C
			// 0x330
		};
	};
	uint32_t Cookie; // 0x330
	uint32_t CookiePad[1]; // 0x334
	int64_tx<bits> ConsoleSessionForegroundProcessId; // 0x338
	uint32_t Wow64SharedInformation[16]; // 0x340
	ETW_UMGL_KEY UserModeGlobalLogger[16]; // 0x380
	uint32_t ImageFileExecutionOptions; // 0x3A0
	uint32_t LangGenerationCount; // 0x3A4
	uint64_tx<bits> Reserved5; // 0x3A8
	volatile uint64_tx<bits> InterruptTimeBias; // 0x3B0
	union {
		volatile uint64_tx<bits> TscQpcBias; // 0x3B8
		volatile uint64_tx<bits> QpcBias; // 0x3B8
	};
	volatile uint32_t ActiveProcessorCount; // 0x3C0
	volatile uint16_t ActiveGroupCount; // 0x3C4
	// Begin Windows 8+
	union {
		uint16_t TscQpcDataNew; // 0x3C6
		uint16_t QpcData; // 0x3C6
		struct {
			union {
				uint8_t TscQpcEnabledNew; // 0x3C6
				uint8_t QpcFlags; // 0x3C6
				struct {
					uint8_t QpcBypassEnabled : 1;
					uint8_t QpcBypassUseHVPage : 1;
					uint8_t QpcBypassDisable32Bit : 1;
					uint8_t : 1;
					uint8_t QpcBypassUseMfence : 1;
					uint8_t QpcBypassUseLfence : 1;
					uint8_t QpcBypassA73Errata : 1;
					uint8_t QpcBypassUseRdtscp : 1;
				};
			};
			union {
				uint8_t TscQpcShiftNew; // 0x3C7
				uint8_t QpcShift; // 0x3C7
			};
		};
	};
	// End Windows 8+

	volatile uint32_t AitSamplingValue; // 0x3C8
	volatile uint32_t AppCompatFlag; // 0x3CC
	uintptr_tx<64> SystemDllNativeRelocation; // 0x3D0
	uintptr_tx<32> SystemDllWowRelocation; // 0x3D8
	uint32_t XStatePad[1]; // 0x3DC
	XSTATE_CONFIGURATIONX<bits> XState; // 0x3E0
	// 0x5F0
};
ValidateStructSize(0x5F0, KUSER_SHARED_DATAX_base<32>);
ValidateStructAlignment(0x8, KUSER_SHARED_DATAX_base<32>);
ValidateStructSize(0x5F0, KUSER_SHARED_DATAX_base<64>);
ValidateStructAlignment(0x8, KUSER_SHARED_DATAX_base<64>);

template<size_t bits = native_bits>
struct alignas(PAGE_SIZE) KUSER_SHARED_DATAX : KUSER_SHARED_DATAX_base<bits> {};


extern "C" {
	extern const KUSER_SHARED_DATAX<32> USER_SHARED_DATA32 asm("_USER_SHARED_DATA32");
	extern const KUSER_SHARED_DATAX<64> USER_SHARED_DATA64 asm("_USER_SHARED_DATA64");
	extern const KUSER_SHARED_DATAX<> USER_SHARED_DATA asm("_USER_SHARED_DATA");
}

#define USER_SHARED_DATAR(bits) (*(const KUSER_SHARED_DATAX<bits>*)0x7FFE0000)

//static inline const KUSER_SHARED_DATAX<>& USER_SHARED_DATAR = *(const KUSER_SHARED_DATAX<>*)0x7FFE0000;

#endif