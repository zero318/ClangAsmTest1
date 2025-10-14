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
//#include <winternl.h>


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

#ifdef NTAPI
#undef NTAPI
#endif
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

using NTSTATUSX = int32_t;
__if_not_exists(NTSTATUS) {
    using NTSTATUS = NTSTATUSX;
}

#ifndef STATUS_UNSUCCESSFUL
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif
#ifndef STATUS_NOT_IMPLEMENTED
#define STATUS_NOT_IMPLEMENTED ((NTSTATUS)0xC0000002L)
#endif
#ifndef STATUS_INVALID_INFO_CLASS
#define STATUS_INVALID_INFO_CLASS ((NTSTATUS)0xC0000003L)
#endif
#ifndef STATUS_INFO_LENGTH_MISMATCH
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#endif
#ifndef STATUS_ACCESS_VIOLATION
#define STATUS_ACCESS_VIOLATION ((NTSTATUS)0xC0000005L)
#endif
#ifndef STATUS_INVALID_PARAMETER
#define STATUS_INVALID_PARAMETER ((NTSTATUS)0xC000000DL)
#endif
#ifndef STATUS_NO_MEMORY
#define STATUS_NO_MEMORY ((NTSTATUS)0xC0000017L)
#endif
#ifndef STATUS_INVALID_SYSTEM_SERVICE
#define STATUS_INVALID_SYSTEM_SERVICE ((NTSTATUS)0xC000001CL)
#endif
#ifndef STATUS_ILLEGAL_INSTRUCTION
#define STATUS_ILLEGAL_INSTRUCTION ((NTSTATUS)0xC000001DL)
#endif
#ifndef STATUS_ACCESS_DENIED
#define STATUS_ACCESS_DENIED ((NTSTATUS)0xC0000022L)
#endif
#ifndef STATUS_BUFFER_TOO_SMALL
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023L)
#endif
#ifndef STATUS_NONCONTINUABLE_EXCEPTION
#define STATUS_NONCONTINUABLE_EXCEPTION ((NTSTATUS)0xC0000025L)
#endif
#ifndef STATUS_UNWIND
#define STATUS_UNWIND ((NTSTATUS)0xC0000027L)
#endif

using UNKNOWN_TYPE = void;
using ANY_TYPE = void;

using KPRIORITYX = int32_t;
__if_not_exists(KPRIORITY) {
    using KPRIORITY = KPRIORITYX;
}

template<size_t bits = native_bits>
using KAFFINITYX = size_tx<bits>;
ValidateStructSize(0x4, KAFFINITYX<32>);
ValidateStructAlignment(0x4, KAFFINITYX<32>);
ValidateStructSize(0x8, KAFFINITYX<64>);
ValidateStructAlignment(0x8, KAFFINITYX<64>);

__if_not_exists(KAFFINITY) {
    using KAFFINITY = KAFFINITYX<>;
}

using KIRQLX = uint8_t;
__if_not_exists(KIRQL) {
    using KIRQL = KIRQLX;
}

template<size_t bits = native_bits>
using HANDLEX = PTRZX<bits>;
ValidateStructSize(0x4, HANDLEX<32>);
ValidateStructAlignment(0x4, HANDLEX<32>);
ValidateStructSize(0x8, HANDLEX<64>);
ValidateStructAlignment(0x8, HANDLEX<64>);

template<size_t bits = native_bits>
using PHANDLEX = PTRZX<bits, HANDLEX<bits>>;

__if_not_exists(HANDLE) {
    using HANDLE = HANDLEX<>;
}
__if_not_exists(PHANDLE) {
    using PHANDLE = PHANDLEX<>;
}

using RTL_ATOMX = uint16_t;

__if_not_exists(RTL_ATOM) {
    using RTL_ATOM = RTL_ATOMX;
}
__if_not_exists(PRTL_ATOM) {
    using PRTL_ATOM = RTL_ATOM*;
}

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
using int64_tx = typename winstruct_alignment_impl<bits>::int64_tx;
ValidateStructSize(0x8, int64_tx<32>);
ValidateStructAlignment(0x4, int64_tx<32>);
ValidateStructSize(0x8, int64_tx<64>);
ValidateStructAlignment(0x8, int64_tx<64>);

template<size_t bits = native_bits>
using uint64_tx = typename winstruct_alignment_impl<bits>::uint64_tx;
ValidateStructSize(0x8, uint64_tx<32>);
ValidateStructAlignment(0x4, uint64_tx<32>);
ValidateStructSize(0x8, uint64_tx<64>);
ValidateStructAlignment(0x8, uint64_tx<64>);

template<size_t bits = native_bits>
struct BaseWinTypes;

template<>
struct BaseWinTypes<16> {
    typedef int16_t         BOOL, near* PBOOL, far* LPBOOL;
    typedef uint8_t         BOOLEAN, near* PBOOLEAN, far* LPBOOLEAN;

    typedef char            CHAR, near* PCHAR, far* LPCHAR;
    typedef unsigned char   UCHAR, near* PUCHAR, far* LPUCHAR;

    typedef wchar_t         WCHAR, near* PWCHAR, far* LPWCHAR;

    typedef uint8_t         BYTE, near* PBYTE, far* LPBYTE;
    typedef uint16_t        WORD, near* PWORD, far* LPWORD;
    typedef uint32_t        DWORD, near* PDWORD, far* LPDWORD;
    typedef uint64_tx<32>   DWORDLONG, near* PDWORDLONG, far* LPDWORDLONG;

    typedef int16_t         SHORT, near* PSHORT, far* LPSHORT;
    typedef uint16_t        USHORT, near* PUSHORT, far* LPUSHORT;
    typedef int16_t         INT, near* PINT, far* LPINT;
    typedef uint16_t        UINT, near* PUINT, far* LPUINT;
    typedef int32_t         LONG, near* PLONG, far* LPLONG;
    typedef uint32_t        ULONG, near* PULONG, far* LPULONG;
    typedef int64_tx<32>    LONGLONG, near* PLONGLONG, far* LPLONGLONG;
    typedef uint64_tx<32>   ULONGLONG, near* PULONGLONG, far* LPULONGLONG;

    typedef int8_t          INT8, near* PINT8, far* LPINT8;
    typedef int16_t         INT16, near* PINT16, far* LPINT16;
    typedef int32_t         INT32, near* PINT32, far* LPINT32;
    typedef int64_tx<32>    INT64, near* PINT64, far* LPINT64;
    typedef uint8_t         UINT8, near* PUINT8, far* LPUINT8;
    typedef uint16_t        UINT16, near* PUINT16, far* LPUINT16;
    typedef uint32_t        UINT32, near* PUINT32, far* LPUINT32;
    typedef uint64_tx<32>   UINT64, near* PUINT64, far* LPUINT64;

    typedef int16_t         HALF_PTR, near* PHALF_PTR, far* LPHALF_PTR;
    typedef uint16_t        UHALF_PTR, near* PUHALF_PTR, far* LPUHALF_PTR;
    typedef intptr_tx<32>   INT_PTR, near* PINT_PTR, far* LPINT_PTR;
    typedef uintptr_tx<32>  UINT_PTR, near* PUINT_PTR, far* LPUINT_PTR;
    typedef intptr_tx<32>   LONG_PTR, near* PLONG_PTR, far* LPLONG_PTR;
    typedef uintptr_tx<32>  ULONG_PTR, near* PULONG_PTR, far* LPULONG_PTR;
    typedef uintptr_tx<32>  DWORD_PTR, near* PDWORD_PTR, far* LPDWORD_PTR;

    typedef size_tx<32>     SIZE_T, near* PSIZE_T, far* LPSIZE_T;
    typedef ssize_tx<32>    SSIZE_T, near* PSSIZE_T, far* LPSSIZE_T;
};
template<>
struct BaseWinTypes<32> {
    typedef int16_t         BOOL, near* PBOOL, far* LPBOOL;
    typedef uint8_t         BOOLEAN, near* PBOOLEAN, far* LPBOOLEAN;

    typedef char            CHAR, near* PCHAR, far* LPCHAR;
    typedef unsigned char   UCHAR, near* PUCHAR, far* LPUCHAR;

    typedef wchar_t         WCHAR, near* PWCHAR, far* LPWCHAR;

    typedef uint8_t         BYTE, near* PBYTE, far* LPBYTE;
    typedef uint16_t        WORD, near* PWORD, far* LPWORD;
    typedef uint32_t        DWORD, near* PDWORD, far* LPDWORD;
    typedef uint64_tx<32>   DWORDLONG, near* PDWORDLONG, far* LPDWORDLONG;

    typedef int16_t         SHORT, near* PSHORT, far* LPSHORT;
    typedef uint16_t        USHORT, near* PUSHORT, far* LPUSHORT;
    typedef int32_t         INT, near* PINT, far* LPINT;
    typedef uint32_t        UINT, near* PUINT, far* LPUINT;
    typedef int32_t         LONG, near* PLONG, far* LPLONG;
    typedef uint32_t        ULONG, near* PULONG, far* LPULONG;
    typedef int64_tx<32>    LONGLONG, near* PLONGLONG, far* LPLONGLONG;
    typedef uint64_tx<32>   ULONGLONG, near* PULONGLONG, far* LPULONGLONG;

    typedef int8_t          INT8, near* PINT8, far* LPINT8;
    typedef int16_t         INT16, near* PINT16, far* LPINT16;
    typedef int32_t         INT32, near* PINT32, far* LPINT32;
    typedef int64_tx<32>    INT64, near* PINT64, far* LPINT64;
    typedef uint8_t         UINT8, near* PUINT8, far* LPUINT8;
    typedef uint16_t        UINT16, near* PUINT16, far* LPUINT16;
    typedef uint32_t        UINT32, near* PUINT32, far* LPUINT32;
    typedef uint64_tx<32>   UINT64, near* PUINT64, far* LPUINT64;

    typedef int16_t         HALF_PTR, near* PHALF_PTR, far* LPHALF_PTR;
    typedef uint16_t        UHALF_PTR, near* PUHALF_PTR, far* LPUHALF_PTR;
    typedef intptr_tx<32>   INT_PTR, near* PINT_PTR, far* LPINT_PTR;
    typedef uintptr_tx<32>  UINT_PTR, near* PUINT_PTR, far* LPUINT_PTR;
    typedef intptr_tx<32>   LONG_PTR, near* PLONG_PTR, far* LPLONG_PTR;
    typedef uintptr_tx<32>  ULONG_PTR, near* PULONG_PTR, far* LPULONG_PTR;
    typedef uintptr_tx<32>  DWORD_PTR, near* PDWORD_PTR, far* LPDWORD_PTR;

    typedef size_tx<32>     SIZE_T, near* PSIZE_T, far* LPSIZE_T;
    typedef ssize_tx<32>    SSIZE_T, near* PSSIZE_T, far* LPSSIZE_T;
};
template<>
struct BaseWinTypes<64> {
    typedef int16_t         BOOL, near* PBOOL, far* LPBOOL;
    typedef uint8_t         BOOLEAN, near* PBOOLEAN, far* LPBOOLEAN;

    typedef char            CHAR, near* PCHAR, far* LPCHAR;
    typedef unsigned char   UCHAR, near* PUCHAR, far* LPUCHAR;

    typedef wchar_t         WCHAR, near* PWCHAR, far* LPWCHAR;

    typedef uint8_t         BYTE, near* PBYTE, far* LPBYTE;
    typedef uint16_t        WORD, near* PWORD, far* LPWORD;
    typedef uint32_t        DWORD, near* PDWORD, far* LPDWORD;
    typedef uint64_tx<64>   DWORDLONG, near* PDWORDLONG, far* LPDWORDLONG;

    typedef int16_t         SHORT, near* PSHORT, far* LPSHORT;
    typedef uint16_t        USHORT, near* PUSHORT, far* LPUSHORT;
    typedef int32_t         INT, near* PINT, far* LPINT;
    typedef uint32_t        UINT, near* PUINT, far* LPUINT;
    typedef int32_t         LONG, near* PLONG, far* LPLONG;
    typedef uint32_t        ULONG, near* PULONG, far* LPULONG;
    typedef int64_tx<64>    LONGLONG, near* PLONGLONG, far* LPLONGLONG;
    typedef uint64_tx<64>   ULONGLONG, near* PULONGLONG, far* LPULONGLONG;

    typedef int8_t          INT8, near* PINT8, far* LPINT8;
    typedef int16_t         INT16, near* PINT16, far* LPINT16;
    typedef int32_t         INT32, near* PINT32, far* LPINT32;
    typedef int64_tx<64>    INT64, near* PINT64, far* LPINT64;
    typedef uint8_t         UINT8, near* PUINT8, far* LPUINT8;
    typedef uint16_t        UINT16, near* PUINT16, far* LPUINT16;
    typedef uint32_t        UINT32, near* PUINT32, far* LPUINT32;
    typedef uint64_tx<64>   UINT64, near* PUINT64, far* LPUINT64;

    typedef int16_t         HALF_PTR, near* PHALF_PTR, far* LPHALF_PTR;
    typedef uint16_t        UHALF_PTR, near* PUHALF_PTR, far* LPUHALF_PTR;
    typedef intptr_tx<64>   INT_PTR, near* PINT_PTR, far* LPINT_PTR;
    typedef uintptr_tx<64>  UINT_PTR, near* PUINT_PTR, far* LPUINT_PTR;
    typedef intptr_tx<64>   LONG_PTR, near* PLONG_PTR, far* LPLONG_PTR;
    typedef uintptr_tx<64>  ULONG_PTR, near* PULONG_PTR, far* LPULONG_PTR;
    typedef uintptr_tx<64>  DWORD_PTR, near* PDWORD_PTR, far* LPDWORD_PTR;

    typedef size_tx<64>     SIZE_T, near* PSIZE_T, far* LPSIZE_T;
    typedef ssize_tx<64>    SSIZE_T, near* PSSIZE_T, far* LPSSIZE_T;
};

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
using PLARGE_INTEGERX = PTRZX<bits, LARGE_INTEGERX<bits>>;
template<size_t bits = native_bits>
using LPLARGE_INTEGERX = LPTRZX<bits, LARGE_INTEGERX<bits>>;

__if_not_exists(LARGE_INTEGER) {
    using LARGE_INTEGER = LARGE_INTEGERX<>;
}
__if_not_exists(PLARGE_INTEGER) {
    using PLARGE_INTEGER = PLARGE_INTEGERX<>;
}
__if_not_exists(LPLARGE_INTEGER) {
    using LPLARGE_INTEGER = LPLARGE_INTEGERX<>;
}

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
using PULARGE_INTEGERX = PTRZX<bits, ULARGE_INTEGERX<bits>>;
template<size_t bits = native_bits>
using LPULARGE_INTEGERX = LPTRZX<bits, ULARGE_INTEGERX<bits>>;

__if_not_exists(ULARGE_INTEGER) {
    using ULARGE_INTEGER = ULARGE_INTEGERX<>;
}
__if_not_exists(PULARGE_INTEGER) {
    using PULARGE_INTEGER = PULARGE_INTEGERX<>;
}
__if_not_exists(LPULARGE_INTEGER) {
    using LPULARGE_INTEGER = LPULARGE_INTEGERX<>;
}

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

__if_not_exists(KSYSTEM_TIME) {
    using KSYSTEM_TIME = KSYSTEM_TIMEX<>;
}

enum OBJECT_WAIT_TYPEX : int32_t {
    WaitAllObjectX = 0,
    WaitAnyObjectX = 1
};
__if_not_exists(OBJECT_WAIT_TYPE) {
    using OBJECT_WAIT_TYPE = OBJECT_WAIT_TYPEX;
}
__if_not_exists(WaitAllObject) {
    static constexpr auto WaitAllObject = (OBJECT_WAIT_TYPE)WaitAllObjectX;
}
__if_not_exists(WaitAnyObject) {
    static constexpr auto WaitAnyObject = (OBJECT_WAIT_TYPE)WaitAnyObjectX;
}

enum THREADINFOCLASSX : int32_t {
    ThreadIsIoPendingX = 16,
    ThreadNameInformationX = 38
};
__if_not_exists(THREADINFOCLASS) {
    using THREADINFOCLASS = THREADINFOCLASSX;
}
__if_not_exists(ThreadIsIoPending) {
    static constexpr auto ThreadIsIoPending = (THREADINFOCLASS)ThreadIsIoPendingX;
}
__if_not_exists(ThreadNameInformation) {
    static constexpr auto ThreadNameInformation = (THREADINFOCLASS)ThreadNameInformationX;
}

enum PROCESSINFOCLASSX : int32_t {
    ProcessBasicInformationX = 0,
    ProcessQuotaLimitsX = 1,
    ProcessIoCountersX = 2,
    ProcessVmCountersX = 3,
    ProcessTimesX = 4,
    ProcessBasePriorityX = 5,
    ProcessRaisePriorityX = 6,
    ProcessDebugPortX = 7,
    ProcessExceptionPortX = 8,
    ProcessAccessTokenX = 9,
    ProcessLdtInformationX = 10,
    ProcessLdtSizeX = 11,
    ProcessDefaultHardErrorModeX = 12,
    ProcessIoPortHandlersX = 13,
    ProcessPooledUsageAndLimitsX = 14,
    ProcessWorkingSetWatchX = 15,
    ProcessUserModeIOPLX = 16,
    ProcessEnableAlignmentFaultFixupX = 17,
    ProcessPriorityClassX = 18,
    ProcessWx86InformationX = 19,
    ProcessHandleCountX = 20,
    ProcessAffinityMaskX = 21,
    ProcessPriorityBoostX = 22,
    ProcessDeviceMapX = 23,
    ProcessSessionInformationX = 24,
    ProcessForegroundInformationX = 25,
    ProcessWow64InformationX = 26,
    ProcessImageFileNameX = 27,
    ProcessLUIDDeviceMapsEnabledX = 28,
    ProcessBreakOnTerminationX = 29,
    ProcessDebugObjectHandleX = 30,
    ProcessDebugFlagsX = 31,
    ProcessHandleTracingX = 32,
    ProcessIoPriorityX = 33,
    ProcessExecuteFlagsX = 34,
    ProcessTelemetryIdInformationX = 64,
    ProcessSubsystemInformationX = 75
};
__if_not_exists(PROCESSINFOCLASS) {
    using PROCESSINFOCLASS = PROCESSINFOCLASSX;
}
__if_not_exists(ProcessBasicInformation) {
    static constexpr auto ProcessBasicInformation = (PROCESSINFOCLASS)ProcessBasicInformationX;
}
__if_not_exists(ProcessQuotaLimits) {
    static constexpr auto ProcessQuotaLimits = (PROCESSINFOCLASS)ProcessQuotaLimitsX;
}
__if_not_exists(ProcessIoCounters) {
    static constexpr auto ProcessIoCounters = (PROCESSINFOCLASS)ProcessIoCountersX;
}
__if_not_exists(ProcessVmCounters) {
    static constexpr auto ProcessVmCounters = (PROCESSINFOCLASS)ProcessVmCountersX;
}
__if_not_exists(ProcessTimes) {
    static constexpr auto ProcessTimes = (PROCESSINFOCLASS)ProcessTimesX;
}
__if_not_exists(ProcessBasePriority) {
    static constexpr auto ProcessBasePriority = (PROCESSINFOCLASS)ProcessBasePriorityX;
}
__if_not_exists(ProcessRaisePriority) {
    static constexpr auto ProcessRaisePriority = (PROCESSINFOCLASS)ProcessRaisePriorityX;
}
__if_not_exists(ProcessDebugPort) {
    static constexpr auto ProcessDebugPort = (PROCESSINFOCLASS)ProcessDebugPortX;
}
__if_not_exists(ProcessExceptionPort) {
    static constexpr auto ProcessExceptionPort = (PROCESSINFOCLASS)ProcessExceptionPortX;
}
__if_not_exists(ProcessAccessToken) {
    static constexpr auto ProcessAccessToken = (PROCESSINFOCLASS)ProcessAccessTokenX;
}
__if_not_exists(ProcessLdtInformation) {
    static constexpr auto ProcessLdtInformation = (PROCESSINFOCLASS)ProcessLdtInformationX;
}
__if_not_exists(ProcessLdtSize) {
    static constexpr auto ProcessLdtSize = (PROCESSINFOCLASS)ProcessLdtSizeX;
}
__if_not_exists(ProcessDefaultHardErrorMode) {
    static constexpr auto ProcessDefaultHardErrorMode = (PROCESSINFOCLASS)ProcessDefaultHardErrorModeX;
}
__if_not_exists(ProcessIoPortHandlers) {
    static constexpr auto ProcessIoPortHandlers = (PROCESSINFOCLASS)ProcessIoPortHandlersX;
}
__if_not_exists(ProcessPooledUsageAndLimits) {
    static constexpr auto ProcessPooledUsageAndLimits = (PROCESSINFOCLASS)ProcessPooledUsageAndLimitsX;
}
__if_not_exists(ProcessWorkingSetWatch) {
    static constexpr auto ProcessWorkingSetWatch = (PROCESSINFOCLASS)ProcessWorkingSetWatchX;
}
__if_not_exists(ProcessUserModeIOPL) {
    static constexpr auto ProcessUserModeIOPL = (PROCESSINFOCLASS)ProcessUserModeIOPLX;
}
__if_not_exists(ProcessEnableAlignmentFaultFixup) {
    static constexpr auto ProcessEnableAlignmentFaultFixup = (PROCESSINFOCLASS)ProcessEnableAlignmentFaultFixupX;
}
__if_not_exists(ProcessPriorityClass) {
    static constexpr auto ProcessPriorityClass = (PROCESSINFOCLASS)ProcessPriorityClassX;
}
__if_not_exists(ProcessWx86Information) {
    static constexpr auto ProcessWx86Information = (PROCESSINFOCLASS)ProcessWx86InformationX;
}
__if_not_exists(ProcessHandleCount) {
    static constexpr auto ProcessHandleCount = (PROCESSINFOCLASS)ProcessHandleCountX;
}
__if_not_exists(ProcessAffinityMask) {
    static constexpr auto ProcessAffinityMask = (PROCESSINFOCLASS)ProcessAffinityMaskX;
}
__if_not_exists(ProcessPriorityBoost) {
    static constexpr auto ProcessPriorityBoost = (PROCESSINFOCLASS)ProcessPriorityBoostX;
}
__if_not_exists(ProcessDeviceMap) {
    static constexpr auto ProcessDeviceMap = (PROCESSINFOCLASS)ProcessDeviceMapX;
}
__if_not_exists(ProcessSessionInformation) {
    static constexpr auto ProcessSessionInformation = (PROCESSINFOCLASS)ProcessSessionInformationX;
}
__if_not_exists(ProcessForegroundInformation) {
    static constexpr auto ProcessForegroundInformation = (PROCESSINFOCLASS)ProcessForegroundInformationX;
}
__if_not_exists(ProcessWow64Information) {
    static constexpr auto ProcessWow64Information = (PROCESSINFOCLASS)ProcessWow64InformationX;
}
__if_not_exists(ProcessImageFileName) {
    static constexpr auto ProcessImageFileName = (PROCESSINFOCLASS)ProcessImageFileNameX;
}
__if_not_exists(ProcessLUIDDeviceMapsEnabled) {
    static constexpr auto ProcessLUIDDeviceMapsEnabled = (PROCESSINFOCLASS)ProcessLUIDDeviceMapsEnabledX;
}
__if_not_exists(ProcessBreakOnTermination) {
    static constexpr auto ProcessBreakOnTermination = (PROCESSINFOCLASS)ProcessBreakOnTerminationX;
}
__if_not_exists(ProcessDebugObjectHandle) {
    static constexpr auto ProcessDebugObjectHandle = (PROCESSINFOCLASS)ProcessDebugObjectHandleX;
}
__if_not_exists(ProcessDebugFlags) {
    static constexpr auto ProcessDebugFlags = (PROCESSINFOCLASS)ProcessDebugFlagsX;
}
__if_not_exists(ProcessHandleTracing) {
    static constexpr auto ProcessHandleTracing = (PROCESSINFOCLASS)ProcessHandleTracingX;
}
__if_not_exists(ProcessIoPriority) {
    static constexpr auto ProcessIoPriority = (PROCESSINFOCLASS)ProcessIoPriorityX;
}
__if_not_exists(ProcessExecuteFlags) {
    static constexpr auto ProcessExecuteFlags = (PROCESSINFOCLASS)ProcessExecuteFlagsX;
}
__if_not_exists(ProcessTelemetryIdInformation) {
    static constexpr auto ProcessTelemetryIdInformation = (PROCESSINFOCLASS)ProcessTelemetryIdInformationX;
}
__if_not_exists(ProcessSubsystemInformation) {
    static constexpr auto ProcessSubsystemInformation = (PROCESSINFOCLASS)ProcessSubsystemInformationX;
}

template<size_t bits = native_bits>
struct PROCESS_EXECUTE_FLAGSX {
    union {
        uint32_t raw; // 0x0, 0x0
        struct {
            uint32_t MEM_EXECUTE_OPTION_ENABLE : 1; // 1
            uint32_t MEM_EXECUTE_OPTION_DISABLE : 1; // 2
            uint32_t MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION : 1; // 3
            uint32_t MEM_EXECUTE_OPTION_PERMANENT : 1; // 4
            uint32_t MEM_EXECUTE_OPTION_EXECUTE_DISPATCH_ENABLE : 1; // 5
            uint32_t MEM_EXECUTE_OPTION_IMAGE_DISPATCH_ENABLE : 1; // 6
        };
    };
    // 0x4, 0x4
};
ValidateStructSize(0x4, PROCESS_EXECUTE_FLAGSX<>);
ValidateStructAlignment(0x4, PROCESS_EXECUTE_FLAGSX<>);

__if_not_exists(PROCESS_EXECUTE_FLAGS) {
    using PROCESS_EXECUTE_FLAGS = PROCESS_EXECUTE_FLAGSX<>;
}

static constexpr uint32_t THREADSTATE_GETTHREADINFO = 0;
static constexpr uint32_t THREADSTATE_INSENDMESSAGE = 1;
static constexpr uint32_t THREADSTATE_FOCUSWINDOW = 2;
static constexpr uint32_t THREADSTATE_ACTIVEWINDOW = 3;
static constexpr uint32_t THREADSTATE_CAPTUREWINDOW = 4;
static constexpr uint32_t THREADSTATE_PROGMANWINDOW = 5;
static constexpr uint32_t THREADSTATE_TASKMANWINDOW = 6;
static constexpr uint32_t THREADSTATE_GETMESSAGETIME = 7;
static constexpr uint32_t THREADSTATE_GETINPUTSTATE = 8;
static constexpr uint32_t THREADSTATE_UPTIMELASTREAD = 9;
static constexpr uint32_t THREADSTATE_FOREGROUNDTHREAD = 10;
static constexpr uint32_t THREADSTATE_GETCURSOR = 11;

enum SYSTEM_INFORMATION_CLASSX : int32_t {
    SystemBasicInformationX = 0,
    SystemProcessorInformationX = 1,
    SystemPerformanceInformationX = 2,
    SystemTimeOfDayInformationX = 3,
    SystemProcessInformationX = 5,
    SystemProcessorPerformanceInformationX = 8,
    SystemInterruptInformationX = 23,
    SystemExceptionInformationX = 33,
    SystemRegistryQuotaInformationX = 37,
    SystemLookasideInformationX = 45,
    SystemCodeIntegrityInformationX = 103,
    SystemPolicyInformationX = 134,
};
__if_not_exists(SYSTEM_INFORMATION_CLASS) {
    using SYSTEM_INFORMATION_CLASS = SYSTEM_INFORMATION_CLASSX;
}
__if_not_exists(SystemBasicInformation) {
    static constexpr auto SystemBasicInformation = (SYSTEM_INFORMATION_CLASS)SystemBasicInformationX;
}
__if_not_exists(SystemProcessorInformation) {
    static constexpr auto SystemProcessorInformation = (SYSTEM_INFORMATION_CLASS)SystemProcessorInformationX;
}
__if_not_exists(SystemPerformanceInformation) {
    static constexpr auto SystemPerformanceInformation = (SYSTEM_INFORMATION_CLASS)SystemPerformanceInformationX;
}
__if_not_exists(SystemTimeOfDayInformation) {
    static constexpr auto SystemTimeOfDayInformation = (SYSTEM_INFORMATION_CLASS)SystemTimeOfDayInformationX;
}
__if_not_exists(SystemProcessInformation) {
    static constexpr auto SystemProcessInformation = (SYSTEM_INFORMATION_CLASS)SystemProcessInformationX;
}
__if_not_exists(SystemProcessorPerformanceInformation) {
    static constexpr auto SystemProcessorPerformanceInformation = (SYSTEM_INFORMATION_CLASS)SystemProcessorPerformanceInformationX;
}
__if_not_exists(SystemInterruptInformation) {
    static constexpr auto SystemInterruptInformation = (SYSTEM_INFORMATION_CLASS)SystemInterruptInformationX;
}
__if_not_exists(SystemExceptionInformation) {
    static constexpr auto SystemExceptionInformation = (SYSTEM_INFORMATION_CLASS)SystemExceptionInformationX;
}
__if_not_exists(SystemRegistryQuotaInformation) {
    static constexpr auto SystemRegistryQuotaInformation = (SYSTEM_INFORMATION_CLASS)SystemRegistryQuotaInformationX;
}
__if_not_exists(SystemLookasideInformation) {
    static constexpr auto SystemLookasideInformation = (SYSTEM_INFORMATION_CLASS)SystemLookasideInformationX;
}
__if_not_exists(SystemCodeIntegrityInformation) {
    static constexpr auto SystemCodeIntegrityInformation = (SYSTEM_INFORMATION_CLASS)SystemCodeIntegrityInformationX;
}
__if_not_exists(SystemPolicyInformation) {
    static constexpr auto SystemPolicyInformation = (SYSTEM_INFORMATION_CLASS)SystemPolicyInformationX;
}

enum MEMORY_INFORMATION_CLASSX : int32_t {
    MemoryBasicInformationX = 0
};
__if_not_exists(MEMORY_INFORMATION_CLASS) {
    using MEMORY_INFORMATION_CLASS = MEMORY_INFORMATION_CLASSX;
}
__if_not_exists(MemoryBasicInformation) {
    static constexpr auto MemoryBasicInformation = (MEMORY_INFORMATION_CLASS)MemoryBasicInformationX;
}

enum KEY_INFORMATION_CLASSX : int32_t {
    KeyBasicInformationX = 0,
    KeyNodeInformationX = 1,
    KeyFullInformationX = 2,
    KeyNameInformationX = 3,
    KeyCachedInformationX = 4,
    KeyFlagsInformationX = 5,
    KeyVirtualizationInformationX = 6,
    KeyHandleTagsInformationX = 7,
    KeyTrustInformationX = 8,
    KeyLayerInformationX = 9
};
__if_not_exists(KEY_INFORMATION_CLASS) {
    using KEY_INFORMATION_CLASS = KEY_INFORMATION_CLASSX;
}
__if_not_exists(KeyBasicInformation) {
    static constexpr auto KeyBasicInformation = (KEY_INFORMATION_CLASS)KeyBasicInformationX;
}
__if_not_exists(KeyNodeInformation) {
    static constexpr auto KeyNodeInformation = (KEY_INFORMATION_CLASS)KeyNodeInformationX;
}
__if_not_exists(KeyFullInformation) {
    static constexpr auto KeyFullInformation = (KEY_INFORMATION_CLASS)KeyFullInformationX;
}
__if_not_exists(KeyNameInformation) {
    static constexpr auto KeyNameInformation = (KEY_INFORMATION_CLASS)KeyNameInformationX;
}
__if_not_exists(KeyCachedInformation) {
    static constexpr auto KeyCachedInformation = (KEY_INFORMATION_CLASS)KeyCachedInformationX;
}
__if_not_exists(KeyFlagsInformation) {
    static constexpr auto KeyFlagsInformation = (KEY_INFORMATION_CLASS)KeyFlagsInformationX;
}
__if_not_exists(KeyVirtualizationInformation) {
    static constexpr auto KeyVirtualizationInformation = (KEY_INFORMATION_CLASS)KeyVirtualizationInformationX;
}
__if_not_exists(KeyHandleTagsInformation) {
    static constexpr auto KeyHandleTagsInformation = (KEY_INFORMATION_CLASS)KeyHandleTagsInformationX;
}
__if_not_exists(KeyTrustInformation) {
    static constexpr auto KeyTrustInformation = (KEY_INFORMATION_CLASS)KeyTrustInformationX;
}
__if_not_exists(KeyLayerInformation) {
    static constexpr auto KeyLayerInformation = (KEY_INFORMATION_CLASS)KeyLayerInformationX;
}

enum KEY_VALUE_INFORMATION_CLASSX : int32_t {
    KeyValueBasicInformationX = 0,
    KeyValueFullInformationX = 1,
    KeyValuePartialInformationX = 2,
    KeyValueFullInformationAlign64X = 3,
    KeyValuePartialInformationAlign64X = 4,
    KeyValueLayerInformationX = 5
};
__if_not_exists(KEY_VALUE_INFORMATION_CLASS) {
    using KEY_VALUE_INFORMATION_CLASS = KEY_VALUE_INFORMATION_CLASSX;
}
__if_not_exists(KeyValueBasicInformation) {
    static constexpr auto KeyValueBasicInformation = (KEY_VALUE_INFORMATION_CLASS)KeyValueBasicInformationX;
}
__if_not_exists(KeyValueFullInformation) {
    static constexpr auto KeyValueFullInformation = (KEY_VALUE_INFORMATION_CLASS)KeyValueFullInformationX;
}
__if_not_exists(KeyValuePartialInformation) {
    static constexpr auto KeyValuePartialInformation = (KEY_VALUE_INFORMATION_CLASS)KeyValuePartialInformationX;
}
__if_not_exists(KeyValueFullInformationAlign64) {
    static constexpr auto KeyValueFullInformationAlign64 = (KEY_VALUE_INFORMATION_CLASS)KeyValueFullInformationAlign64X;
}
__if_not_exists(KeyValuePartialInformationAlign64) {
    static constexpr auto KeyValuePartialInformationAlign64 = (KEY_VALUE_INFORMATION_CLASS)KeyValuePartialInformationAlign64X;
}
__if_not_exists(KeyValueLayerInformation) {
    static constexpr auto KeyValueLayerInformation = (KEY_VALUE_INFORMATION_CLASS)KeyValueLayerInformationX;
}

enum KEY_SET_INFORMATION_CLASSX : int32_t {
    KeyWriteTimeInformationX = 0,
    KeyWow64FlagsInformationX = 1,
    KeyControlFlagsInformationX = 2,
    KeySetVirtualizationInformationX = 3,
    KeySetDebugInformationX = 4,
    KeySetHandleTagsInformationX = 5,
    KeySetLayerInformationX = 6
};
__if_not_exists(KEY_SET_INFORMATION_CLASS) {
    using KEY_SET_INFORMATION_CLASS = KEY_SET_INFORMATION_CLASSX;
}
__if_not_exists(KeyWriteTimeInformation) {
    static constexpr auto KeyWriteTimeInformation = (KEY_SET_INFORMATION_CLASS)KeyWriteTimeInformationX;
}
__if_not_exists(KeyWow64FlagsInformation) {
    static constexpr auto KeyWow64FlagsInformation = (KEY_SET_INFORMATION_CLASS)KeyWow64FlagsInformationX;
}
__if_not_exists(KeyControlFlagsInformation) {
    static constexpr auto KeyControlFlagsInformation = (KEY_SET_INFORMATION_CLASS)KeyControlFlagsInformationX;
}
__if_not_exists(KeySetVirtualizationInformation) {
    static constexpr auto KeySetVirtualizationInformation = (KEY_SET_INFORMATION_CLASS)KeySetVirtualizationInformationX;
}
__if_not_exists(KeySetDebugInformation) {
    static constexpr auto KeySetDebugInformation = (KEY_SET_INFORMATION_CLASS)KeySetDebugInformationX;
}
__if_not_exists(KeySetHandleTagsInformation) {
    static constexpr auto KeySetHandleTagsInformation = (KEY_SET_INFORMATION_CLASS)KeySetHandleTagsInformationX;
}
__if_not_exists(KeySetLayerInformation) {
    static constexpr auto KeySetLayerInformation = (KEY_SET_INFORMATION_CLASS)KeySetLayerInformationX;
}

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

__if_not_exists(STRING) {
    using STRING = STRINGX<>;
}
__if_not_exists(PSTRING) {
    using PSTRING = STRINGX<>*;
}
__if_not_exists(LPSTRING) {
    using LPSTRING = STRINGX<>*;
}

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

__if_not_exists(UNICODE_STRING) {
    using UNICODE_STRING = UNICODE_STRINGX<>;
}
__if_not_exists(PUNICODE_STRING) {
    using PUNICODE_STRING = UNICODE_STRINGX<>*;
}
__if_not_exists(LPUNICODE_STRING) {
    using LPUNICODE_STRING = UNICODE_STRINGX<>*;
}

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

__if_not_exists(LIST_ENTRY) {
    using LIST_ENTRY = LIST_ENTRYX<>;
}

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

__if_not_exists(ASSEMBLY_STORAGE_MAP_ENTRY) {
    using ASSEMBLY_STORAGE_MAP_ENTRY = ASSEMBLY_STORAGE_MAP_ENTRYX<>;
}

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

__if_not_exists(ASSEMBLY_STORAGE_MAP) {
    using ASSEMBLY_STORAGE_MAP = ASSEMBLY_STORAGE_MAPX<>;
}

/*========================================
    ACTIVATION CONTEXTS
========================================*/
#pragma region // ACTIVATION CONTEXT definitions

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATAX {
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
ValidateStructSize(0x20, ACTIVATION_CONTEXT_DATAX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATAX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA) {
    using ACTIVATION_CONTEXT_DATA = ACTIVATION_CONTEXT_DATAX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_TOC_HEADERX {
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
ValidateStructSize(0x10, ACTIVATION_CONTEXT_DATA_TOC_HEADERX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_TOC_HEADERX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_TOC_HEADER) {
    using ACTIVATION_CONTEXT_DATA_TOC_HEADER = ACTIVATION_CONTEXT_DATA_TOC_HEADERX<>;
}

// No actual extensions?
template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADERX {
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
ValidateStructSize(0x10, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADERX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADERX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADER) {
    using ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADER = ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_HEADERX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_TOC_ENTRYX {
    uint32_t Id; // 0x0, 0x0
    uint32_t Offset; // 0x4, 0x4
    uint32_t Length; // 0x8, 0x8
    uint32_t Format; // 0xC, 0xC
    // 0x10, 0x10
};
ValidateStructSize(0x10, ACTIVATION_CONTEXT_DATA_TOC_ENTRYX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_TOC_ENTRYX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_TOC_ENTRY) {
    using ACTIVATION_CONTEXT_DATA_TOC_ENTRY = ACTIVATION_CONTEXT_DATA_TOC_ENTRYX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRYX {
    GUID ExtensionGuid; // 0x0, 0x0
    uint32_t TocOffset; // 0x10, 0x10
    uint32_t Length; // 0x14, 0x14
    // 0x18, 0x18
};
ValidateStructSize(0x18, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRYX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRYX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRY) {
    using ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRY = ACTIVATION_CONTEXT_DATA_EXTENDED_TOC_ENTRYX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADERX {
    uint32_t HeaderSize; // 0x0, 0x0
    uint32_t HashAlgorithm; // 0x4, 0x4
    uint32_t EntryCount; // 0x8, 0x8
    uint32_t FirstEntryOffset; // 0xC, 0xC
    uint32_t AssemblyInformationSectionOffset; // 0x10, 0x10
    // 0x14, 0x14
};
ValidateStructSize(0x14, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADERX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADERX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER) {
    using ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADER = ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_HEADERX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRYX {
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
ValidateStructSize(0x18, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRYX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRYX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY) {
    using ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRY = ACTIVATION_CONTEXT_DATA_ASSEMBLY_ROSTER_ENTRYX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_STRING_SECTION_HEADERX {
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
ValidateStructSize(0x2C, ACTIVATION_CONTEXT_STRING_SECTION_HEADERX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_STRING_SECTION_HEADERX<>);

__if_not_exists(ACTIVATION_CONTEXT_STRING_SECTION_HEADER) {
    using ACTIVATION_CONTEXT_STRING_SECTION_HEADER = ACTIVATION_CONTEXT_STRING_SECTION_HEADERX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_GUID_SECTION_HEADERX {
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
ValidateStructSize(0x28, ACTIVATION_CONTEXT_GUID_SECTION_HEADERX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_GUID_SECTION_HEADERX<>);

__if_not_exists(ACTIVATION_CONTEXT_GUID_SECTION_HEADER) {
    using ACTIVATION_CONTEXT_GUID_SECTION_HEADER = ACTIVATION_CONTEXT_GUID_SECTION_HEADERX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_SECTION_HASH_TABLEX {
    uint32_t BucketTableEntryCount; // 0x0, 0x0
    uint32_t BucketTableOffset; // 0x4, 0x4
    // 0x8, 0x8
};
ValidateStructSize(0x8, ACTIVATION_CONTEXT_SECTION_HASH_TABLEX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_SECTION_HASH_TABLEX<>);

__if_not_exists(ACTIVATION_CONTEXT_SECTION_HASH_TABLE) {
    using ACTIVATION_CONTEXT_SECTION_HASH_TABLE = ACTIVATION_CONTEXT_SECTION_HASH_TABLEX<>;
}

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_STRING_SECTION_HASH_TABLEX = ACTIVATION_CONTEXT_SECTION_HASH_TABLEX<bits>;

__if_not_exists(ACTIVATION_CONTEXT_STRING_SECTION_HASH_TABLE) {
    using ACTIVATION_CONTEXT_STRING_SECTION_HASH_TABLE = ACTIVATION_CONTEXT_STRING_SECTION_HASH_TABLEX<>;
}

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_GUID_SECTION_HASH_TABLEX = ACTIVATION_CONTEXT_SECTION_HASH_TABLEX<bits>;

__if_not_exists(ACTIVATION_CONTEXT_GUID_SECTION_HASH_TABLE) {
    using ACTIVATION_CONTEXT_GUID_SECTION_HASH_TABLE = ACTIVATION_CONTEXT_GUID_SECTION_HASH_TABLEX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX {
    uint32_t ChainCount; // 0x0, 0x0
    uint32_t ChainOffset; // 0x4, 0x4
    // 0x8, 0x8
};
ValidateStructSize(0x8, ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX<>);

__if_not_exists(ACTIVATION_CONTEXT_SECTION_HASH_BUCKET) {
    using ACTIVATION_CONTEXT_SECTION_HASH_BUCKET = ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX<>;
}

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_STRING_SECTION_HASH_BUCKETX = ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX<bits>;

__if_not_exists(ACTIVATION_CONTEXT_STRING_SECTION_HASH_BUCKET) {
    using ACTIVATION_CONTEXT_STRING_SECTION_HASH_BUCKET = ACTIVATION_CONTEXT_STRING_SECTION_HASH_BUCKETX<>;
}

template<size_t bits = native_bits>
using ACTIVATION_CONTEXT_GUID_SECTION_HASH_BUCKETX = ACTIVATION_CONTEXT_SECTION_HASH_BUCKETX<bits>;

__if_not_exists(ACTIVATION_CONTEXT_GUID_SECTION_HASH_BUCKET) {
    using ACTIVATION_CONTEXT_GUID_SECTION_HASH_BUCKET = ACTIVATION_CONTEXT_GUID_SECTION_HASH_BUCKETX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_STRING_SECTION_ENTRYX {
    uint32_t PseudoKey; // 0x0, 0x0
    uint32_t KeyOffset; // 0x4, 0x4
    uint32_t KeyLength; // 0x8, 0x8
    uint32_t Offset; // 0xC, 0xC
    uint32_t Length; // 0x10, 0x10
    uint32_t AssemblyRosterIndex; // 0x14, 0x14
    // 0x18, 0x18
};
ValidateStructSize(0x18, ACTIVATION_CONTEXT_STRING_SECTION_ENTRYX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_STRING_SECTION_ENTRYX<>);

__if_not_exists(ACTIVATION_CONTEXT_STRING_SECTION_ENTRY) {
    using ACTIVATION_CONTEXT_STRING_SECTION_ENTRY = ACTIVATION_CONTEXT_STRING_SECTION_ENTRYX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_GUID_SECTION_ENTRYX {
    GUID Guid; // 0x0, 0x0
    uint32_t Offset; // 0x10, 0x10
    uint32_t Length; // 0x14, 0x14
    uint32_t AssemblyRosterIndex; // 0x18, 0x18
    // 0x1C, 0x1C
};
ValidateStructSize(0x1C, ACTIVATION_CONTEXT_GUID_SECTION_ENTRYX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_GUID_SECTION_ENTRYX<>);

__if_not_exists(ACTIVATION_CONTEXT_GUID_SECTION_ENTRY) {
    using ACTIVATION_CONTEXT_GUID_SECTION_ENTRY = ACTIVATION_CONTEXT_GUID_SECTION_ENTRYX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATIONX {
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
ValidateStructSize(0x34, ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATION) {
    using ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATION = ACTIVATION_CONTEXT_DATA_ASSEMBLY_GLOBAL_INFORMATIONX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATIONX {
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
ValidateStructSize(0x64, ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION) {
    using ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATION = ACTIVATION_CONTEXT_DATA_ASSEMBLY_INFORMATIONX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_DLL_REDIRECTIONX {
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
ValidateStructSize(0x14, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION) {
    using ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION = ACTIVATION_CONTEXT_DATA_DLL_REDIRECTIONX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENTX {
    uint32_t Length; // 0x0, 0x0
    uint32_t Offset; // 0x4, 0x4
    // 0x8, 0x8
};
ValidateStructSize(0x8, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENTX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENTX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT) {
    using ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENT = ACTIVATION_CONTEXT_DATA_DLL_REDIRECTION_PATH_SEGMENTX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTIONX {
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
ValidateStructSize(0x18, ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION) {
    using ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTION = ACTIVATION_CONTEXT_DATA_WINDOW_CLASS_REDIRECTIONX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTIONX {
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
ValidateStructSize(0x64, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION) {
    using ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION = ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTIONX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIMX {
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
ValidateStructSize(0x2C, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIMX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIMX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM) {
    using ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIM = ACTIVATION_CONTEXT_DATA_COM_SERVER_REDIRECTION_SHIMX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTIONX {
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
ValidateStructSize(0x44, ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION) {
    using ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTION = ACTIVATION_CONTEXT_DATA_COM_INTERFACE_REDIRECTIONX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_CLR_SURROGATEX {
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
ValidateStructSize(0x28, ACTIVATION_CONTEXT_DATA_CLR_SURROGATEX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_CLR_SURROGATEX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_CLR_SURROGATE) {
    using ACTIVATION_CONTEXT_DATA_CLR_SURROGATE = ACTIVATION_CONTEXT_DATA_CLR_SURROGATEX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSIONX {
    uint16_t Major; // 0x0, 0x0
    uint16_t Minor; // 0x2, 0x2
    // 0x4, 0x4
};
ValidateStructSize(0x4, ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSIONX<>);
ValidateStructAlignment(0x2, ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSION) {
    using ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSION = ACTIVATION_CONTEXT_DATA_TYPE_LIBRARY_VERSIONX<>;
} 

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600X {
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
ValidateStructSize(0x1C, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600X<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600X<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600) {
    using ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600 = ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION_2600X<>;
}

// TODO: inherit?
template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTIONX {
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
ValidateStructSize(0x20, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION) {
    using ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTION = ACTIVATION_CONTEXT_DATA_COM_TYPE_LIBRARY_REDIRECTIONX<>;
}

template<size_t bits = native_bits>
struct ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTIONX {
    uint32_t Size; // 0x0, 0x0
    union {
        uint32_t Flags; // 0x4, 0x4
        struct {
        };
    };
    uint32_t ConfiguredClsidOffset; // 0x8, 0x8
    // 0xC, 0xC
};
ValidateStructSize(0xC, ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTIONX<>);
ValidateStructAlignment(0x4, ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTIONX<>);

__if_not_exists(ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION) {
    using ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTION = ACTIVATION_CONTEXT_DATA_COM_PROGID_REDIRECTIONX<>;
}

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


__if_not_exists(ACTIVATION_CONTEXT_NOTIFY_ROUTINE) {
    using ACTIVATION_CONTEXT_NOTIFY_ROUTINE = ACTIVATION_CONTEXT_NOTIFY_ROUTINEX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT) {
    using ACTIVATION_CONTEXT = ACTIVATION_CONTEXTX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_WRAPPED) {
    using ACTIVATION_CONTEXT_WRAPPED = ACTIVATION_CONTEXT_WRAPPEDX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_BASIC_INFORMATION) {
    using ACTIVATION_CONTEXT_BASIC_INFORMATION = ACTIVATION_CONTEXT_BASIC_INFORMATIONX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION) {
    using ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION = ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATIONX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_DETAILED_INFORMATION) {
    using ACTIVATION_CONTEXT_DETAILED_INFORMATION = ACTIVATION_CONTEXT_DETAILED_INFORMATIONX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600) {
    using ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600 = ACTIVATION_CONTEXT_SECTION_KEYED_DATA_2600X<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATA) {
    using ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATA = ACTIVATION_CONTEXT_SECTION_KEYED_DATA_ASSEMBLY_METADATAX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_SECTION_KEYED_DATA) {
    using ACTIVATION_CONTEXT_SECTION_KEYED_DATA = ACTIVATION_CONTEXT_SECTION_KEYED_DATAX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_ASSEMBLY_DATA) {
    using ACTIVATION_CONTEXT_ASSEMBLY_DATA = ACTIVATION_CONTEXT_ASSEMBLY_DATAX<>;
}

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

__if_not_exists(RTL_ACTIVATION_CONTEXT_STACK_FRAME) {
    using RTL_ACTIVATION_CONTEXT_STACK_FRAME = RTL_ACTIVATION_CONTEXT_STACK_FRAMEX<>;
}

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

__if_not_exists(RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME) {
    using RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME = RTL_HEAP_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_STACK_FRAMELIST) {
    using ACTIVATION_CONTEXT_STACK_FRAMELIST = ACTIVATION_CONTEXT_STACK_FRAMELISTX<>;
}

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

__if_not_exists(RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASIC) {
    using RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASIC = RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_BASICX<>;
}

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

__if_not_exists(RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDED) {
    using RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDED = RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX<>;
}

template<size_t bits = native_bits>
using RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX = RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDEDX<bits>;

__if_not_exists(RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME) {
    using RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME = RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAMEX<>;
}

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

__if_not_exists(ACTIVATION_CONTEXT_STACK) {
    using ACTIVATION_CONTEXT_STACK = ACTIVATION_CONTEXT_STACKX<>;
}

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

__if_not_exists(LDR_DATA_TABLE_ENTRY) {
    using LDR_DATA_TABLE_ENTRY = LDR_DATA_TABLE_ENTRYX<>;
}

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

__if_not_exists(PEB_LDR_DATA) {
    using PEB_LDR_DATA = PEB_LDR_DATAX<>;
}

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

__if_not_exists(CURDIR) {
    using CURDIR = CURDIRX<>;
}

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

__if_not_exists(RTL_DRIVE_LETTER_CURDIR) {
    using RTL_DRIVE_LETTER_CURDIR = RTL_DRIVE_LETTER_CURDIRX<>;
}

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
using PRTL_USER_PROCESS_PARAMETERSX = PTRZX<bits, RTL_USER_PROCESS_PARAMETERSX<bits>>;

__if_not_exists(RTL_USER_PROCESS_PARAMETERS) {
    using RTL_USER_PROCESS_PARAMETERS = RTL_USER_PROCESS_PARAMETERSX<>;
}
__if_not_exists(PRTL_USER_PROCESS_PARAMETERS) {
    using PRTL_USER_PROCESS_PARAMETERS = PRTL_USER_PROCESS_PARAMETERSX<>;
}

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

__if_not_exists(RTL_CRITICAL_SECTION_DEBUG) {
    using RTL_CRITICAL_SECTION_DEBUG = RTL_CRITICAL_SECTION_DEBUGX<>;
}

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

__if_not_exists(RTL_CRITICAL_SECTION) {
    using RTL_CRITICAL_SECTION = RTL_CRITICAL_SECTIONX<>;
}

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

__if_not_exists(GDI_HANDLE_BUFFER) {
    using GDI_HANDLE_BUFFER = GDI_HANDLE_BUFFERX<>;
}

template<size_t bits = native_bits>
static inline constexpr size_t GDI_HANDLE_BUFFERX_SIZE = GDI_HANDLE_BUFFERX_base<bits>::buffer_size;

//using PPS_POST_PROCESS_INIT_ROUTINE = void(void);

/*========================================
    PEB
========================================*/

using PS_POST_PROCESS_INIT_ROUTINEX = void NTAPI(void);

__if_not_exists(PPS_POST_PROCESS_INIT_ROUTINE) {
    using PPS_POST_PROCESS_INIT_ROUTINE = PTRZX<native_bits, PS_POST_PROCESS_INIT_ROUTINEX>;
}

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
    PTRZX<bits, PS_POST_PROCESS_INIT_ROUTINEX> PostProcessInitRoutine; // 0x14C, 0x230
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

__if_not_exists(PEB) {
    using PEB = PEBX<>;
}

/*========================================
    CONTEXT
========================================*/

struct FSAVE_AREAX {
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
ValidateStructSize(0x6C, FSAVE_AREAX);
ValidateStructAlignment(0x4, FSAVE_AREAX);

__if_not_exists(FSAVE_AREA) {
    using FSAVE_AREA = FSAVE_AREAX;
}

template<size_t bits = native_bits>
struct FLOATING_SAVE_AREAX : FSAVE_AREA {
    uint32_t Cr0NpxState; // 0x6C, 0x6C
    // 0x70, 0x70
};
ValidateStructSize(0x70, FLOATING_SAVE_AREAX<32>);
ValidateStructAlignment(0x4, FLOATING_SAVE_AREAX<32>);
ValidateStructSize(0x70, FLOATING_SAVE_AREAX<64>);
ValidateStructAlignment(0x4, FLOATING_SAVE_AREAX<64>);

__if_not_exists(FLOATING_SAVE_AREA) {
    using FLOATING_SAVE_AREA = FLOATING_SAVE_AREAX<>;
}

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
    union {
        uint64_t ErrorOffset; // 0x8
        struct {
            uint32_t : 32;
            union {
                uint16_t ErrorSelector; // 0xC
                uint32_t ErrorSelector_wide;
            };
        };
    };
    union {
        uint64_t DataOffset; // 0x10
        struct {
            uint32_t : 32;
            union {
                uint16_t DataSelector; // 0x14
                uint32_t DataSelector_wide;
            };
        };
    };
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

__if_not_exists(FXSAVE_FORMAT) {
    using FXSAVE_FORMAT = FXSAVE_FORMATX<>;
}

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

__if_not_exists(XSAVE_AREA_HEADER) {
    using XSAVE_AREA_HEADER = XSAVE_AREA_HEADERX<>;
}

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

__if_not_exists(XSAVE_FORMAT) {
    using XSAVE_FORMAT = XSAVE_FORMATX<>;
}

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

__if_not_exists(XSAVE_AREA) {
    using XSAVE_AREA = XSAVE_AREAX<>;
}

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

__if_not_exists(XSTATE_CONTEXT) {
    using XSTATE_CONTEXT = XSTATE_CONTEXTX<>;
}

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

__if_not_exists(CONTEXT) {
    using CONTEXT = CONTEXTX<>;
}

static inline constexpr uint32_t CONTEXTX_CONTROL               = 0x00000001L; // (x32, x64) SS:SP, CS:IP, FLAGS, BP
static inline constexpr uint32_t CONTEXTX_INTEGER               = 0x00000002L; // (x32, x64) AX, BX, CX, DX, SI, DI
static inline constexpr uint32_t CONTEXTX_SEGMENTS              = 0x00000004L; // (x32, x64) DS, ES, FS, GS
static inline constexpr uint32_t CONTEXTX_FLOATING_POINT        = 0x00000008L; // (x32, x64) 387 state
static inline constexpr uint32_t CONTEXTX_DEBUG_REGISTERS       = 0x00000010L; // (x32, x64) DB 0-3,6,7
static inline constexpr uint32_t CONTEXTX_EXTENDED_REGISTERS	= 0x00000020L; // (x32, x64) cpu specific extensions
static inline constexpr uint32_t CONTEXTX_XSTATE                = 0x00000040L; // (x32, x64)
static inline constexpr uint32_t CONTEXTX_KERNEL_CET            = 0x00000080L; // (x64)
static inline constexpr uint32_t CONTEXTX_32                    = 0x00010000L; // (x32)
static inline constexpr uint32_t CONTEXTX_64                    = 0x00100000L; // (x64)
static inline constexpr uint32_t CONTEXTX_ARM                   = 0x00200000L; // (arm)
static inline constexpr uint32_t CONTEXTX_ARM64                 = 0x00400000L; // (arm64)
static inline constexpr uint32_t CONTEXTX_KERNEL_DEBUGGER       = 0x04000000L; // (xbox)
static inline constexpr uint32_t CONTEXTX_EXCEPTION_ACTIVE      = 0x08000000L; // (x32, x64)
static inline constexpr uint32_t CONTEXTX_SERVICE_ACTIVE        = 0x10000000L; // (x32, x64)
static inline constexpr uint32_t CONTEXTX_UNWOUND_TO_CALL       = 0x20000000L; // (x64)
static inline constexpr uint32_t CONTEXTX_EXCEPTION_REQUEST     = 0x40000000L; // (x32, x64)
static inline constexpr uint32_t CONTEXTX_EXCEPTION_REPORTING   = 0x80000000L; // (x32, x64)

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

__if_not_exists(XSTATE_CONFIGURATION) {
    using XSTATE_CONFIGURATION = XSTATE_CONFIGURATIONX<>;
}

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

__if_not_exists(EXCEPTION_RECORD) {
    using EXCEPTION_RECORD = EXCEPTION_RECORDX<>;
}

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
using EXCEPTION_HANDLER_FUNCX = EXCEPTION_DISPOSITION(EXCEPTION_RECORDX<bits>,
                                                      PTRZX<bits>,
                                                      PTRZX<bits, CONTEXTX<bits, T>>,
                                                      PTRZX<bits>
                                                      );

__if_not_exists(EXCEPTION_HANDLER_FUNC) {
    using EXCEPTION_HANDLER_FUNC = EXCEPTION_HANDLER_FUNCX<>;
}

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

__if_not_exists(EXCEPTION_REGISTRATION_RECORD) {
    using EXCEPTION_REGISTRATION_RECORD = EXCEPTION_REGISTRATION_RECORDX<>;
}

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct EXCEPTION_POINTERSX {
    PTRZX<bits, EXCEPTION_RECORDX<bits>> ExceptionRecord; // 0x0, 0x0
    PTRZX<bits, CONTEXTX<bits, T>> ContextRecord; // 0x4, 0x8
    // 0x8, 0x10
};
ValidateStructSize(0x8, EXCEPTION_POINTERSX<32>);
ValidateStructAlignment(0x4, EXCEPTION_POINTERSX<32>);
ValidateStructSize(0x10, EXCEPTION_POINTERSX<64>);
ValidateStructAlignment(0x8, EXCEPTION_POINTERSX<64>);

__if_not_exists(EXCEPTION_POINTERS) {
    using EXCEPTION_POINTERS = EXCEPTION_POINTERSX<>;
}

template<size_t bits = native_bits>
struct OBJECT_ATTRIBUTESX {
    uint32_t Length; // 0x0, 0x0
    __x64_padding(0x4);
    PTRZX<bits> RootDirectory; // 0x4, 0x8
    PTRZX<bits, UNICODE_STRINGX<bits>> ObjectName; // 0x8, 0x10
    uint32_t Attributes; // 0xC, 0x18
    __x64_padding(0x4);
    PTRZX<bits> SecurityDescriptor; // 0x10, 0x20
    PTRZX<bits> SecurityQualityOfService; // 0x14, 0x28
    // 0x18, 0x30
};
ValidateStructSize(0x18, OBJECT_ATTRIBUTESX<32>);
ValidateStructAlignment(0x4, OBJECT_ATTRIBUTESX<32>);
ValidateStructSize(0x30, OBJECT_ATTRIBUTESX<64>);
ValidateStructAlignment(0x8, OBJECT_ATTRIBUTESX<64>);

template<size_t bits = native_bits>
using POBJECT_ATTRIBUTESX = PTRZX<bits, OBJECT_ATTRIBUTESX<bits>>;

__if_not_exists(OBJECT_ATTRIBUTES) {
    using OBJECT_ATTRIBUTES = OBJECT_ATTRIBUTESX<>;
}
__if_not_exists(POBJECT_ATTRIBUTES) {
    using POBJECT_ATTRIBUTES = POBJECT_ATTRIBUTESX<>;
}

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

__if_not_exists(NT_TIB) {
    using NT_TIB = NT_TIBX<>;
}

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

template<size_t bits = native_bits>
using PCLIENT_IDX = PTRZX<bits, CLIENT_IDX<bits>>;

__if_not_exists(CLIENT_ID) {
    using CLIENT_ID = CLIENT_IDX<>;
}
__if_not_exists(PCLIENT_ID) {
    using PCLIENT_ID = PCLIENT_IDX<>;
}

template<size_t bits = native_bits>
struct LPC_MESSAGEX {
    uint16_t DataLength; // 0x0, 0x0
    uint16_t Length; // 0x2, 0x2
    uint16_t MessageType; // 0x4, 0x4
    uint16_t DataInfoOffset; // 0x6, 0x6
    CLIENT_IDX<bits> ClientId; // 0x8, 0x8
    uint32_t MessageId; // 0x10, 0x18
    uint32_t CallbackId; // 0x14, 0x1C
    // 0x18, 0x20
};
ValidateStructSize(0x18, LPC_MESSAGEX<32>);
ValidateStructAlignment(0x4, LPC_MESSAGEX<32>);
ValidateStructSize(0x20, LPC_MESSAGEX<64>);
ValidateStructAlignment(0x8, LPC_MESSAGEX<64>);

__if_not_exists(LPC_MESSAGE) {
    using LPC_MESSAGE = LPC_MESSAGEX<>;
}

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

__if_not_exists(GDI_TEB_BATCH) {
    using GDI_TEB_BATCH = GDI_TEB_BATCHX<>;
}

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

__if_not_exists(TEB_ACTIVE_FRAME_CONTEXT) {
    using TEB_ACTIVE_FRAME_CONTEXT = TEB_ACTIVE_FRAME_CONTEXTX<>;
}

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

__if_not_exists(TEB_ACTIVE_FRAME_CONTEXT_EX) {
    using TEB_ACTIVE_FRAME_CONTEXT_EX = TEB_ACTIVE_FRAME_CONTEXT_EXX<>;
}

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

__if_not_exists(TEB_ACTIVE_FRAME) {
    using TEB_ACTIVE_FRAME = TEB_ACTIVE_FRAMEX<>;
}

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

__if_not_exists(TEB_ACTIVE_FRAME_EX) {
    using TEB_ACTIVE_FRAME_EX = TEB_ACTIVE_FRAME_EXX<>;
}

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct WX86TIBX {
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
ValidateStructSize(0x40, WX86TIBX<>);
ValidateStructAlignment(0x4, WX86TIBX<>);

__if_not_exists(WX86TIB) {
    using WX86TIB = WX86TIBX<>;
}

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
        uint8_t arbitrary_offset[0];
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
        uint8_t arbitrary_offset[0];
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

__if_not_exists(TEB) {
    using TEB = TEBX<>;
}

static inline constexpr auto teb32 = (TEBX<32> FS_RELATIVE*)0;
static inline constexpr auto teb64 = (TEBX<64> GS_RELATIVE*)0;
static inline constexpr auto cteb32 = (const TEBX<32> FS_RELATIVE*)0;
static inline constexpr auto cteb64 = (const TEBX<64> GS_RELATIVE*)0;

#if NATIVE_BITS == 32
static inline constexpr auto teb = teb32;
static inline constexpr auto cteb = cteb32;
#else
static inline constexpr auto teb = teb64;
static inline constexpr auto cteb = cteb64;
#endif

template<size_t bits = native_bits>
struct INITIAL_TEBX {
    struct {
        PTRZX<bits> OldStackBase; // 0x0, 0x0
        PTRZX<bits> OldStackLimit; // 0x4, 0x8
    } OldInitialTeb;
    PTRZX<bits> StackBase; // 0x8, 0x10
    PTRZX<bits> StackLimit; // 0xC, 0x18
    PTRZX<bits> StackAllocationBase; // 0x10, 0x20
    // 0x14, 0x28
};

template<size_t bits = native_bits>
using PINITIAL_TEBX = PTRZX<bits, INITIAL_TEBX<bits>>;

__if_not_exists(INITIAL_TEB) {
    using INITIAL_TEB = INITIAL_TEBX<>;
}
__if_not_exists(PINITIAL_TEB) {
    using PINITIAL_TEB = PINITIAL_TEBX<>;
}

#define peb32 (teb32->ProcessEnvironmentBlock)
#define peb64 (teb64->ProcessEnvironmentBlock)

#if NATIVE_BITS == 32
#define peb peb32
#else
#define peb peb64
#endif

template<size_t bits = native_bits, typename T = void>
static inline void write_teb_offset(size_t offset, const T& value) {
    if constexpr (bits == 32) {
        *(T FS_RELATIVE*)&teb32->arbitrary_offset[offset] = value;
    } else if constexpr (bits == 64) {
        *(T GS_RELATIVE*)&teb64->arbitrary_offset[offset] = value;
    }
}

template<typename T, size_t bits = native_bits>
static inline T read_teb_offset(size_t offset) {
    if constexpr (bits == 32) {
        return *(T FS_RELATIVE*)&teb32->arbitrary_offset[offset];
    } else if constexpr (bits == 64) {
        return *(T GS_RELATIVE*)&teb64->arbitrary_offset[offset];
    }
}

/*========================================
    Syscall types
========================================*/

template<size_t bits = native_bits>
struct SYSTEM_BASIC_INFORMATIONX {
    uint32_t Reserved; // 0x0, 0x0 (This field is just set to 0)
    uint32_t TimerResolution; // 0x4, 0x4
    uint32_t PageSize; // 0x8, 0x8
    uint32_t NumberOfPhysicalPages; // 0xC, 0xC
    uint32_t LowestPhysicalPageNumber; // 0x10, 0x10
    uint32_t HighestPhysicalPageNumber; // 0x14, 0x14
    uint32_t AllocationGranularity; // 0x18, 0x18
    __x64_padding(0x4);
    uintptr_tx<bits> MinimumUserModeAddress; // 0x1C, 0x20
    uintptr_tx<bits> MaximumUserModeAddress; // 0x20, 0x28
    KAFFINITYX<bits> ActiveProcessorsAffinityMask; // 0x24, 0x30
    int8_t NumberOfProcessors; // 0x28, 0x38
    __padding(0x3);
    __x64_padding(0x4);
    // 0x2C, 0x40
};
ValidateStructSize(0x2C, SYSTEM_BASIC_INFORMATIONX<32>);
ValidateStructAlignment(0x4, SYSTEM_BASIC_INFORMATIONX<32>);
ValidateStructSize(0x40, SYSTEM_BASIC_INFORMATIONX<64>);
ValidateStructAlignment(0x8, SYSTEM_BASIC_INFORMATIONX<64>);

__if_not_exists(SYSTEM_BASIC_INFORMATION) {
    using SYSTEM_BASIC_INFORMATION = SYSTEM_BASIC_INFORMATIONX<>;
}

template<size_t bits = native_bits>
struct SYSTEM_PROCESSOR_FEATURES_INFORMATIONX;

template<>
struct SYSTEM_PROCESSOR_FEATURES_INFORMATIONX<32> {
    union {
        uint64_tx<32> ProcessorFeatureBits; // 0x0, 0x0
        struct {
            uint64_tx<32> VME : 1; // 1
            uint64_tx<32> TSC : 1; // 2
            uint64_tx<32> VME_PSE_PGE : 1; // 3
            uint64_tx<32> CMOV : 1; // 4
            uint64_tx<32> PGE : 1; // 5
            uint64_tx<32> PSE : 1; // 6
            uint64_tx<32> MTRR : 1; // 7
            uint64_tx<32> CX8 : 1; // 8
            uint64_tx<32> MMX : 1; // 9
            uint64_tx<32> __unknown_intel_errata_fix : 1; // 10
            uint64_tx<32> PAT : 1; // 11
            uint64_tx<32> FXSR : 1; // 12
            uint64_tx<32> SEP : 1; // 13
            uint64_tx<32> SSE : 1; // 14
            uint64_tx<32> _3DNow : 1; // 15
            uint64_tx<32> __amd_mtrr : 1; // 16
            uint64_tx<32> SSE2 : 1; // 17
            uint64_tx<32> DS : 1; // 18
            uint64_tx<32> CLFSH : 1; // 19
            uint64_tx<32> SSE3 : 1; // 20
            uint64_tx<32> __unknown_cpuid_bit : 1; // 21
            uint64_tx<32> ACNT2 : 1; // 22 No idea what this is
            uint64_tx<32> __unknown_A : 1; // 23
            uint64_tx<32> __unknown_B : 1; // 24
            uint64_tx<32> SMEP : 1; // 25
            uint64_tx<32> RDRAND : 1; // 26
            uint64_tx<32> NP : 1; // 27
            uint64_tx<32> __unknown_C : 1; // 28
            uint64_tx<32> __unused_A : 1; // 29
            uint64_tx<32> NX : 1; // 30
            uint64_tx<32> __unused_B : 1; // 31
            uint64_tx<32> __unused_C : 1; // 32
            uint64_tx<32> RDTSCP : 1; // 33
            uint64_tx<32> CLFLUSHOPT : 1; // 34
            uint64_tx<32> HDC : 1; // 35
        };
    };
    uint64_tx<32> Reserved[3]; // 0x8, 0x8
    // 0x20, 0x20
};
template<>
struct SYSTEM_PROCESSOR_FEATURES_INFORMATIONX<64> {
    union {
        uint64_tx<64> ProcessorFeatureBits; // 0x0, 0x0
        struct {
            uint64_tx<64> SMEP : 1; // 1
            uint64_tx<64> TSC : 1; // 2
            uint64_tx<64> VME_PSE_PGE : 1; // 3
            uint64_tx<64> CMOV : 1; // 4
            uint64_tx<64> PGE : 1; // 5
            uint64_tx<64> PSE : 1; // 6
            uint64_tx<64> MTRR : 1; // 7
            uint64_tx<64> CX8 : 1; // 8
            uint64_tx<64> MMX : 1; // 9
            uint64_tx<64> DS : 1; // 10
            uint64_tx<64> PAT : 1; // 11
            uint64_tx<64> FXSR : 1; // 12
            uint64_tx<64> SEP : 1; // 13
            uint64_tx<64> SSE : 1; // 14
            uint64_tx<64> _3DNow : 1; // 15
            uint64_tx<64> __unused_A : 1; // 16
            uint64_tx<64> SSE2 : 1; // 17
            uint64_tx<64> LBR : 1; // 18
            uint64_tx<64> __unused_B : 1; // 19
            uint64_tx<64> SSE3 : 1; // 20
            uint64_tx<64> CMPXCHG16B : 1; // 21
            uint64_tx<64> __unknown_cpuid_bit : 1; // 22
            uint64_tx<64> ACNT2 : 1; // 23 No idea what this is
            uint64_tx<64> __unknown_A : 1; // 24
            uint64_tx<64> __unknown_B : 1; // 25
            uint64_tx<64> __unused_C : 1; // 26
            uint64_tx<64> NP : 1; // 27
            uint64_tx<64> __unknown_C : 1; // 28
            uint64_tx<64> FSGSBASE : 1; // 29
            uint64_tx<64> NX : 1; // 30
            uint64_tx<64> __unused_D : 1; // 31
            uint64_tx<64> __unused_E : 1; // 32
            uint64_tx<64> RDRAND : 1; // 33
            uint64_tx<64> __unused_F : 1; // 34
            uint64_tx<64> RDTSCP : 1; // 35
            uint64_tx<64> __unused_G : 1; // 36
            uint64_tx<64> __unused_H : 1; // 37
            uint64_tx<64> gigabyte_pages : 1; // 38
        };
    };
    uint64_tx<64> Reserved[3]; // 0x8, 0x8
    // 0x20, 0x20
};
ValidateStructSize(0x20, SYSTEM_PROCESSOR_FEATURES_INFORMATIONX<32>);
//ValidateStructAlignment(0x4, SYSTEM_PROCESSOR_FEATURES_INFORMATIONX<32>);
ValidateStructSize(0x20, SYSTEM_PROCESSOR_FEATURES_INFORMATIONX<64>);
ValidateStructAlignment(0x8, SYSTEM_PROCESSOR_FEATURES_INFORMATIONX<64>);

__if_not_exists(SYSTEM_PROCESSOR_FEATURES_INFORMATION) {
    using SYSTEM_PROCESSOR_FEATURES_INFORMATION = SYSTEM_PROCESSOR_FEATURES_INFORMATIONX<>;
}

template<size_t bits = native_bits>
struct SYSTEM_PROCESSOR_INFORMATIONX;

template<>
struct SYSTEM_PROCESSOR_INFORMATIONX<32> {
    uint16_t ProcessorArchitecture; // 0x0, 0x0
    uint16_t ProcessorLevel; // 0x2, 0x2
    uint16_t ProcessorRevision; // 0x4, 0x4
    uint16_t MaximumProcessors; // 0x6, 0x6
    union {
        uint32_t ProcessorFeatureBits; // 0x8, 0x8
        struct {
            uint32_t VME : 1; // 1
            uint32_t TSC : 1; // 2
            uint32_t VME_PSE_PGE : 1; // 3
            uint32_t CMOV : 1; // 4
            uint32_t PGE : 1; // 5
            uint32_t PSE : 1; // 6
            uint32_t MTRR : 1; // 7
            uint32_t CX8 : 1; // 8
            uint32_t MMX : 1; // 9
            uint32_t __unknown_intel_errata_fix : 1; // 10
            uint32_t PAT : 1; // 11
            uint32_t FXSR : 1; // 12
            uint32_t SEP : 1; // 13
            uint32_t SSE : 1; // 14
            uint32_t _3DNow : 1; // 15
            uint32_t __amd_mtrr : 1; // 16
            uint32_t SSE2 : 1; // 17
            uint32_t DS : 1; // 18
            uint32_t CLFSH : 1; // 19
            uint32_t SSE3 : 1; // 20
            uint32_t __unknown_cpuid_bit : 1; // 21
            uint32_t ACNT2 : 1; // 22 No idea what this is
            uint32_t __unknown_A : 1; // 23
            uint32_t __unknown_B : 1; // 24
            uint32_t SMEP : 1; // 25
            uint32_t RDRAND : 1; // 26
            uint32_t NP : 1; // 27
            uint32_t __unknown_C : 1; // 28
            uint32_t __unused_A : 1; // 29
            uint32_t NX : 1; // 30
            uint32_t __NX_unknown_A : 1; // 31
            uint32_t __NX_unknown_B : 1; // 32
        };
    };
    // 0xC, 0xC
};
template<>
struct SYSTEM_PROCESSOR_INFORMATIONX<64> {
    uint16_t ProcessorArchitecture; // 0x0, 0x0
    uint16_t ProcessorLevel; // 0x2, 0x2
    uint16_t ProcessorRevision; // 0x4, 0x4
    uint16_t MaximumProcessors; // 0x6, 0x6
    union {
        uint32_t ProcessorFeatureBits; // 0x8, 0x8
        struct {
            uint32_t SMEP : 1; // 1
            uint32_t TSC : 1; // 2
            uint32_t VME_PSE_PGE : 1; // 3
            uint32_t CMOV : 1; // 4
            uint32_t PGE : 1; // 5
            uint32_t PSE : 1; // 6
            uint32_t MTRR : 1; // 7
            uint32_t CX8 : 1; // 8
            uint32_t MMX : 1; // 9
            uint32_t DS : 1; // 10
            uint32_t PAT : 1; // 11
            uint32_t FXSR : 1; // 12
            uint32_t SEP : 1; // 13
            uint32_t SSE : 1; // 14
            uint32_t _3DNow : 1; // 15
            uint32_t __unused_A : 1; // 16
            uint32_t SSE2 : 1; // 17
            uint32_t LBR : 1; // 18
            uint32_t __unused_B : 1; // 19
            uint32_t SSE3 : 1; // 20
            uint32_t CMPXCHG16B : 1; // 21
            uint32_t __unknown_cpuid_bit : 1; // 22
            uint32_t ACNT2 : 1; // 23 No idea what this is
            uint32_t __unknown_A : 1; // 24
            uint32_t __unknown_B : 1; // 25
            uint32_t __unused_C : 1; // 26
            uint32_t NP : 1; // 27
            uint32_t __unknown_C : 1; // 28
            uint32_t FSGSBASE : 1; // 29
            uint32_t NX : 1; // 30
            uint32_t __NX_unknown_A : 1; // 31
            uint32_t __NX_unknown_B : 1; // 32
        };
    };
    // 0xC, 0xC
};
ValidateStructSize(0xC, SYSTEM_PROCESSOR_INFORMATIONX<32>);
ValidateStructAlignment(0x4, SYSTEM_PROCESSOR_INFORMATIONX<32>);
ValidateStructSize(0xC, SYSTEM_PROCESSOR_INFORMATIONX<64>);
ValidateStructAlignment(0x4, SYSTEM_PROCESSOR_INFORMATIONX<64>);

__if_not_exists(SYSTEM_PROCESSOR_INFORMATION) {
    using SYSTEM_PROCESSOR_INFORMATION = SYSTEM_PROCESSOR_INFORMATIONX<>;
}

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

__if_not_exists(PROCESS_BASIC_INFORMATION) {
    using PROCESS_BASIC_INFORMATION = PROCESS_BASIC_INFORMATIONX<>;
}

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

__if_not_exists(THREAD_BASIC_INFORMATION) {
    using THREAD_BASIC_INFORMATION = THREAD_BASIC_INFORMATIONX<>;
}

template<size_t bits = native_bits>
struct MEMORY_BASIC_INFORMATIONX;

template<>
struct MEMORY_BASIC_INFORMATIONX<32> {
    PTRZX<32> BaseAddress; // 0x0
    PTRZX<32> AllocationBase; // 0x4
    uint32_t AllocationProtect; // 0x8
    size_tx<32> RegionSize; // 0xC
    uint32_t State; // 0x10
    uint32_t Protect; // 0x14
    uint32_t Type; // 0x18
    // 0x1C
};
template<>
struct MEMORY_BASIC_INFORMATIONX<64> {
    PTRZX<64> BaseAddress; // 0x0
    PTRZX<64> AllocationBase; // 0x8
    uint32_t AllocationProtect; // 0x10
    uint16_t PartitionId; // 0x14
    __x64_padding(0x2);
    size_tx<64> RegionSize; // 0x18
    uint32_t State; // 0x20
    uint32_t Protect; // 0x24
    uint32_t Type; // 0x28
    __x64_padding(0x4);
    // 0x30
};
ValidateStructSize(0x1C, MEMORY_BASIC_INFORMATIONX<32>);
ValidateStructAlignment(0x4, MEMORY_BASIC_INFORMATIONX<32>);
ValidateStructSize(0x30, MEMORY_BASIC_INFORMATIONX<64>);
ValidateStructAlignment(0x8, MEMORY_BASIC_INFORMATIONX<64>);

__if_not_exists(MEMORY_BASIC_INFORMATION) {
    using MEMORY_BASIC_INFORMATION = MEMORY_BASIC_INFORMATIONX<>;
}

template<size_t N = 0, size_t bits = native_bits>
struct KEY_VALUE_BASIC_INFORMATIONX {
    uint32_t TitleIndex; // 0x0, 0x0
    uint32_t Type; // 0x4, 0x4
    uint32_t NameLength; // 0x8, 0x8
    wchar_t Name[N]; // 0xC, 0xC
};
ValidateStructSize(0xC, KEY_VALUE_BASIC_INFORMATIONX<>);
ValidateStructAlignment(0x4, KEY_VALUE_BASIC_INFORMATIONX<>);

__if_not_exists(KEY_VALUE_BASIC_INFORMATION) {
    using KEY_VALUE_BASIC_INFORMATION = KEY_VALUE_BASIC_INFORMATIONX<>;
}

template<size_t N = 0, size_t bits = native_bits>
struct KEY_VALUE_FULL_INFORMATIONX {
    uint32_t TitleIndex; // 0x0, 0x0
    uint32_t Type; // 0x4, 0x4
    uint32_t DataOffset; // 0x8, 0x8
    uint32_t DataLength; // 0xC, 0xC
    uint32_t NameLength; // 0x10, 0x10
    wchar_t Name[N]; // 0x14, 0x14
};
ValidateStructSize(0x14, KEY_VALUE_FULL_INFORMATIONX<>);
ValidateStructAlignment(0x4, KEY_VALUE_FULL_INFORMATIONX<>);

__if_not_exists(KEY_VALUE_FULL_INFORMATION) {
    using KEY_VALUE_FULL_INFORMATION = KEY_VALUE_FULL_INFORMATIONX<>;
}

template<size_t N = 0, size_t bits = native_bits>
struct KEY_VALUE_PARTIAL_INFORMATIONX {
    uint32_t TitleIndex; // 0x0, 0x0
    uint32_t Type; // 0x4, 0x4
    uint32_t DataLength; // 0x8, 0x8
    uint8_t Data[N]; // 0xC, 0xC
};
ValidateStructSize(0xC, KEY_VALUE_PARTIAL_INFORMATIONX<>);
ValidateStructAlignment(0x4, KEY_VALUE_PARTIAL_INFORMATIONX<>);

__if_not_exists(KEY_VALUE_PARTIAL_INFORMATION) {
    using KEY_VALUE_PARTIAL_INFORMATION = KEY_VALUE_PARTIAL_INFORMATIONX<>;
}

template<size_t N = 0, size_t bits = native_bits>
struct KEY_BASIC_INFORMATIONX {
    LARGE_INTEGERX<bits> LastWriteTime; // 0x0, 0x0
    uint32_t TitleIndex; // 0x8, 0x8
    uint32_t NameLength; // 0xC, 0xC
    wchar_t Name[N]; // 0x10, 0x10
};
ValidateStructSize(0x10, KEY_BASIC_INFORMATIONX<0, 32>);
ValidateStructAlignment(0x4, KEY_BASIC_INFORMATIONX<0, 32>);
ValidateStructSize(0x10, KEY_BASIC_INFORMATIONX<0, 64>);
ValidateStructAlignment(0x8, KEY_BASIC_INFORMATIONX<0, 64>);

__if_not_exists(KEY_BASIC_INFORMATION) {
    using KEY_BASIC_INFORMATION = KEY_BASIC_INFORMATIONX<>;
}

template<size_t N = 0, size_t bits = native_bits>
struct KEY_NODE_INFORMATIONX {
    LARGE_INTEGERX<bits> LastWriteTime; // 0x0, 0x0
    uint32_t TitleIndex; // 0x8, 0x8
    uint32_t ClassOffset; // 0xC, 0xC
    uint32_t ClassLength; // 0x10, 0x10
    uint32_t NameLength; // 0x14, 0x14
    wchar_t Name[N]; // 0x18, 0x18
};
ValidateStructSize(0x18, KEY_NODE_INFORMATIONX<0, 32>);
ValidateStructAlignment(0x4, KEY_NODE_INFORMATIONX<0, 32>);
ValidateStructSize(0x18, KEY_NODE_INFORMATIONX<0, 64>);
ValidateStructAlignment(0x8, KEY_NODE_INFORMATIONX<0, 64>);

__if_not_exists(KEY_NODE_INFORMATION) {
    using KEY_NODE_INFORMATION = KEY_NODE_INFORMATIONX<>;
}

template<size_t N = 0, size_t bits = native_bits>
struct KEY_FULL_INFORMATIONX {
    LARGE_INTEGERX<bits> LastWriteTime; // 0x0, 0x0
    uint32_t TitleIndex; // 0x8, 0x8
    uint32_t ClassOffset; // 0xC, 0xC
    uint32_t ClassLength; // 0x10, 0x10
    uint32_t SubKeys; // 0x14, 0x14
    uint32_t MaxNameLen; // 0x18, 0x18
    uint32_t MaxClassLen; // 0x1C, 0x1C
    uint32_t Values; // 0x20, 0x20
    uint32_t MaxValueNameLen; // 0x24, 0x24
    uint32_t MaxValueDataLen; // 0x28, 0x28
    wchar_t Class[N]; // 0x2C, 0x2C
};
ValidateStructSize(0x2C, KEY_FULL_INFORMATIONX<0, 32>);
ValidateStructAlignment(0x4, KEY_FULL_INFORMATIONX<0, 32>);
ValidateStructSize(0x30, KEY_FULL_INFORMATIONX<0, 64>);
ValidateStructAlignment(0x8, KEY_FULL_INFORMATIONX<0, 64>);

__if_not_exists(KEY_NODE_INFORMATION) {
    using KEY_NODE_INFORMATION = KEY_NODE_INFORMATIONX<>;
}

template<size_t N = 0, size_t bits = native_bits>
struct KEY_NAME_INFORMATIONX {
    uint32_t NameLength; // 0x0, 0x0
    wchar_t Name[N]; // 0x4, 0x4
};
ValidateStructSize(0x4, KEY_NAME_INFORMATIONX<>);
ValidateStructAlignment(0x4, KEY_NAME_INFORMATIONX<>);

__if_not_exists(KEY_NAME_INFORMATION) {
    using KEY_NAME_INFORMATION = KEY_NAME_INFORMATIONX<>;
}

template<size_t bits = native_bits>
struct KEY_CACHED_INFORMATIONX {
    LARGE_INTEGERX<bits> LastWriteTime; // 0x0, 0x0
    uint32_t TitleIndex; // 0x8, 0x8
    uint32_t SubKeys; // 0xC, 0xC
    uint32_t MaxNameLen; // 0x10, 0x10
    uint32_t Values; // 0x14, 0x14
    uint32_t MaxValueNameLen; // 0x18, 0x18
    uint32_t MaxValueDataLen; // 0x1C, 0x1C
    uint32_t NameLength; // 0x20, 0x20
    __x64_padding(0x4);
    // 0x24, 0x28
};
ValidateStructSize(0x24, KEY_CACHED_INFORMATIONX<32>);
ValidateStructAlignment(0x4, KEY_CACHED_INFORMATIONX<32>);
ValidateStructSize(0x28, KEY_CACHED_INFORMATIONX<64>);
ValidateStructAlignment(0x8, KEY_CACHED_INFORMATIONX<64>);

__if_not_exists(KEY_CACHED_INFORMATION) {
    using KEY_CACHED_INFORMATION = KEY_CACHED_INFORMATIONX<>;
}

template<size_t bits = native_bits>
struct KEY_WRITE_TIME_INFORMATIONX {
    LARGE_INTEGERX<bits> LastWriteTime; // 0x0, 0x0
    // 0x8, 0x8
};
ValidateStructSize(0x8, KEY_WRITE_TIME_INFORMATIONX<32>);
ValidateStructAlignment(0x4, KEY_WRITE_TIME_INFORMATIONX<32>);
ValidateStructSize(0x8, KEY_WRITE_TIME_INFORMATIONX<64>);
ValidateStructAlignment(0x8, KEY_WRITE_TIME_INFORMATIONX<64>);

__if_not_exists(KEY_WRITE_TIME_INFORMATION) {
    using KEY_WRITE_TIME_INFORMATION = KEY_WRITE_TIME_INFORMATIONX<>;
}

/*========================================
    User Shared Data Types
========================================*/

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

__if_not_exists(FIBER) {
    using FIBER = FIBERX<>;
}

enum PS_CREATE_STATEX : int32_t {
    PsCreateInitialStateX = 0,
    PsCreateFailOnFileOpenX = 1,
    PsCreateFailOnSectionOpenX = 2,
    PsCreateFailExeFormatX = 3,
    PsCreateFailMachineMismatchX = 4,
    PsCreateFailExeNameX = 5,
    PsCreateSuccessX = 6,
    PsCreateMaximumStatesX = 7
};
__if_not_exists(PS_CREATE_STATE) {
    using PS_CREATE_STATE = PS_CREATE_STATEX;
}
__if_not_exists(PsCreateInitialState) {
    static constexpr auto PsCreateInitialState = (PS_CREATE_STATE)PsCreateInitialStateX;
}
__if_not_exists(PsCreateFailOnFileOpen) {
    static constexpr auto PsCreateFailOnFileOpen = (PS_CREATE_STATE)PsCreateFailOnFileOpenX;
}
__if_not_exists(PsCreateFailOnSectionOpen) {
    static constexpr auto PsCreateFailOnSectionOpen = (PS_CREATE_STATE)PsCreateFailOnSectionOpenX;
}
__if_not_exists(PsCreateFailExeFormat) {
    static constexpr auto PsCreateFailExeFormat = (PS_CREATE_STATE)PsCreateFailExeFormatX;
}
__if_not_exists(PsCreateFailMachineMismatch) {
    static constexpr auto PsCreateFailMachineMismatch = (PS_CREATE_STATE)PsCreateFailMachineMismatchX;
}
__if_not_exists(PsCreateFailExeName) {
    static constexpr auto PsCreateFailExeName = (PS_CREATE_STATE)PsCreateFailExeNameX;
}
__if_not_exists(PsCreateSuccess) {
    static constexpr auto PsCreateSuccess = (PS_CREATE_STATE)PsCreateSuccessX;
}
__if_not_exists(PsCreateMaximumStates) {
    static constexpr auto PsCreateMaximumStates = (PS_CREATE_STATE)PsCreateMaximumStatesX;
}

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

__if_not_exists(PS_CREATE_INFO) {
    using PS_CREATE_INFO = PS_CREATE_INFOX<>;
}

enum NT_PRODUCT_TYPEX : int32_t {
    NtProductWinNtX = 1,
    NtProductLanManNtX = 2,
    NtProductServerX = 3
};
__if_not_exists(NT_PRODUCT_TYPE) {
    using NT_PRODUCT_TYPE = NT_PRODUCT_TYPEX;
}
__if_not_exists(NtProductWinNt) {
    static constexpr auto NtProductWinNt = (NT_PRODUCT_TYPE)NtProductWinNtX;
}
__if_not_exists(NtProductLanManNt) {
    static constexpr auto NtProductLanManNt = (NT_PRODUCT_TYPE)NtProductLanManNtX;
}
__if_not_exists(NtProductServer) {
    static constexpr auto NtProductServer = (NT_PRODUCT_TYPE)NtProductServerX;
}

enum ALTERNATIVE_ARCHITECTURE_TYPEX : int32_t {
    StandardDesignX = 0,
    NEC98x86X = 1,
    EndAlternativesX = 2
};
__if_not_exists(ALTERNATIVE_ARCHITECTURE_TYPE) {
    using ALTERNATIVE_ARCHITECTURE_TYPE = ALTERNATIVE_ARCHITECTURE_TYPEX;
}
__if_not_exists(StandardDesign) {
    static constexpr auto StandardDesign = (ALTERNATIVE_ARCHITECTURE_TYPE)StandardDesignX;
}
__if_not_exists(NEC98x86) {
    static constexpr auto NEC98x86 = (ALTERNATIVE_ARCHITECTURE_TYPE)NEC98x86X;
}
__if_not_exists(EndAlternatives) {
    static constexpr auto EndAlternatives = (ALTERNATIVE_ARCHITECTURE_TYPE)EndAlternativesX;
}

struct ETW_UMGL_KEYX {
    uint8_t LoggerId; // 0x0
    union {
        uint8_t Flags; // 0x1
        struct {
        };
    };
    // 0x2
};
ValidateStructSize(0x2, ETW_UMGL_KEYX);
ValidateStructAlignment(0x1, ETW_UMGL_KEYX);

__if_not_exists(ETW_UMGL_KEY) {
    using ETW_UMGL_KEY = ETW_UMGL_KEYX;
}

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

__if_not_exists(KUSER_SHARED_DATA) {
    using KUSER_SHARED_DATA = KUSER_SHARED_DATAX<>;
}

extern "C" {
    extern const KUSER_SHARED_DATAX<32> USER_SHARED_DATA32 asm("_USER_SHARED_DATA32");
    extern const KUSER_SHARED_DATAX<64> USER_SHARED_DATA64 asm("_USER_SHARED_DATA64");
    extern const KUSER_SHARED_DATAX<> USER_SHARED_DATA asm("_USER_SHARED_DATA");
}

#define _USER_SHARED_DATAR(bits) (*(const KUSER_SHARED_DATAX<bits>*)0x7FFE0000)
#define USER_SHARED_DATAR(...) _USER_SHARED_DATAR(MACRO_DEFAULT_ARG(native_bits,__VA_ARGS__))

//static inline const KUSER_SHARED_DATAX<>& USER_SHARED_DATAR = *(const KUSER_SHARED_DATAX<>*)0x7FFE0000;

/*========================================
    Other Stuff idk
========================================*/

struct PROCESS_PRIORITY_CLASSX {
    BOOLEAN Foreground; // 0x0
    UCHAR PriorityClass; // 0x1
    // 0x2
};
ValidateStructSize(0x2, PROCESS_PRIORITY_CLASSX);

template<size_t bits = native_bits>
using PPROCESS_PRIORITY_CLASSX = PTRZX<bits, PROCESS_PRIORITY_CLASSX>;

__if_not_exists(PROCESS_PRIORITY_CLASS) {
    using PROCESS_PRIORITY_CLASS = PROCESS_PRIORITY_CLASSX;
}
__if_not_exists(PPROCESS_PRIORITY_CLASS) {
    using PPROCESS_PRIORITY_CLASS = PPROCESS_PRIORITY_CLASSX<>;
}

template<size_t bits = native_bits>
struct RTL_RELATIVE_NAMEX {
    UNICODE_STRINGX<bits> RelativeName;
    HANDLEX<bits> ContainingDirectory;
    PTRZX<bits> CurDirRef;
};
template<size_t bits = native_bits>
using PRTL_RELATIVE_NAMEX = PTRZX<bits, RTL_RELATIVE_NAMEX<bits>>;

__if_not_exists(RTL_RELATIVE_NAME) {
    using RTL_RELATIVE_NAME = RTL_RELATIVE_NAMEX<>;
}
__if_not_exists(PRTL_RELATIVE_NAME) {
    using PRTL_RELATIVE_NAME = PRTL_RELATIVE_NAMEX<>;
}

template<size_t bits = native_bits>
struct IO_STATUS_BLOCKX {
    union {
        NTSTATUS Status;
        PTRZX<bits> Pointer;
    };
    uintptr_tx<bits> Information;
};
template<size_t bits = native_bits>
using PIO_STATUS_BLOCKX = PTRZX<bits, IO_STATUS_BLOCKX<bits>>;

__if_not_exists(IO_STATUS_BLOCK) {
    using IO_STATUS_BLOCK = IO_STATUS_BLOCKX<>;
}
__if_not_exists(PIO_STATUS_BLOCK) {
    using PIO_STATUS_BLOCK = PIO_STATUS_BLOCKX<>;
}

template<size_t bits = native_bits>
struct SECTION_IMAGE_INFORMATIONX {
    PTRZX<bits> TransferAddress; // 0x0, 0x0
    uint32_t ZeroBits; // 0x04, 0x8
    __x64_padding(0x4);
    uintptr_tx<bits> MaximumStackSize; // 0x8, 0x10
    uintptr_tx<bits> CommittedStackSize; // 0xC, 0x18
    uint32_t SubSystemType; // 0x10, 0x20
    union {
        struct {
            uint16_t SubSystemMinorVersion; // 0x14, 0x24
            uint16_t SubSystemMajorVersion; // 0x16, 0x26
        };
        uint32_t SubSystemVersion; // 0x14, 0x24
    };
    uint32_t GpValue; // 0x18, 0x28
    uint16_t ImageCharacteristics; // 0x1C, 0x2C
    uint16_t DllCharacteristics; // 0x1E, 0x2E
    uint16_t Machine; // 0x20, 0x30
    uint8_t ImageContainsCode; // 0x22, 0x32
    union {
        uint8_t ImageFlags; // 0x23, 0x33
        struct {
            uint8_t ComPlusNativeReady : 1;
            uint8_t ComPlusILOnly : 1;
            uint8_t ImageDynamicallyRelocated : 1;
            uint8_t ImageMappedFlat : 1;
            uint8_t BaseBelow4gb : 1;
            uint8_t ComPlusPrefer32bit : 1;
        };
    };
    uint32_t LoaderFlags; // 0x24, 0x34
    uint32_t ImageFileSize; // 0x28, 0x38
    uint32_t CheckSum; // 0x2C, 0x3C
    // 0x30, 0x40
};

template<size_t bits = native_bits>
using PSECTION_IMAGE_INFORMATIONX = PTRZX<bits, SECTION_IMAGE_INFORMATIONX<bits>>;

__if_not_exists(SECTION_IMAGE_INFORMATION) {
    using SECTION_IMAGE_INFORMATION = SECTION_IMAGE_INFORMATIONX<>;
}
__if_not_exists(PSECTION_IMAGE_INFORMATION) {
    using PSECTION_IMAGE_INFORMATION = PSECTION_IMAGE_INFORMATIONX<>;
}

enum SECTION_INFORMATION_CLASSX : int32_t {
    SectionBasicInformationX = 0,
    SectionImageInformationX = 1,
    SectionRelocationInformationX = 2,
    SectionOriginalBaseInformationX = 3,
    SectionInternalImageInformationX = 4
};
__if_not_exists(SECTION_INFORMATION_CLASS) {
    using SECTION_INFORMATION_CLASS = SECTION_INFORMATION_CLASSX;
}
__if_not_exists(SectionBasicInformation) {
    static constexpr auto SectionBasicInformation = (SECTION_INFORMATION_CLASS)SectionBasicInformationX;
}
__if_not_exists(SectionImageInformation) {
    static constexpr auto SectionImageInformation = (SECTION_INFORMATION_CLASS)SectionImageInformationX;
}
__if_not_exists(SectionRelocationInformation) {
    static constexpr auto SectionRelocationInformation = (SECTION_INFORMATION_CLASS)SectionRelocationInformationX;
}
__if_not_exists(SectionOriginalBaseInformation) {
    static constexpr auto SectionOriginalBaseInformation = (SECTION_INFORMATION_CLASS)SectionOriginalBaseInformationX;
}
__if_not_exists(SectionInternalImageInformation) {
    static constexpr auto SectionInternalImageInformation = (SECTION_INFORMATION_CLASS)SectionInternalImageInformationX;
}

template<size_t bits = native_bits>
struct RTL_USER_PROCESS_INFORMATIONX {
    uint32_t Size; // 0x0, 0x0
    __x64_padding(0x4);
    HANDLEX<bits> ProcessHandle; // 0x4, 0x8
    HANDLEX<bits> ThreadHandle; // 0x8, 0x10
    CLIENT_IDX<bits> ClientId; // 0xC, 0x18
    SECTION_IMAGE_INFORMATIONX<bits> ImageInformation; // 0x14, 0x28
};

template<size_t bits = native_bits>
using PRTL_USER_PROCESS_INFORMATIONX = PTRZX<bits, RTL_USER_PROCESS_INFORMATIONX<bits>>;

__if_not_exists(RTL_USER_PROCESS_INFORMATION) {
    using RTL_USER_PROCESS_INFORMATION = RTL_USER_PROCESS_INFORMATIONX<>;
}
__if_not_exists(PRTL_USER_PROCESS_INFORMATION) {
    using PRTL_USER_PROCESS_INFORMATION = PRTL_USER_PROCESS_INFORMATIONX<>;
}

/*========================================
    CSR Stuff
========================================*/

template<size_t bits = native_bits>
struct PORT_MESSAGEX {
    uint32_t u1; // 0x0, 0x0
    uint32_t u2; // 0x4, 0x4
    union {
        CLIENT_IDX<bits> ClientId; // 0x8, 0x8
        float DoNotUseThisField;
    };
    uint32_t MessageId; // 0x10, 0x18
    union {
        uint32_t ClientViewSize; // 0x14, 0x1C
        uint32_t CallbackId;
    };
    __x64_padding(0x4);
    // 0x18, 0x20
};

template<size_t bits = native_bits>
using PPORT_MESSAGEX = PTRZX<bits, PORT_MESSAGEX<>>;

__if_not_exists(PORT_MESSAGE) {
    using PORT_MESSAGE = PORT_MESSAGEX<>;
}
__if_not_exists(PPORT_MESSAGE) {
    using PPORT_MESSAGE = PPORT_MESSAGEX<>;
}

template<size_t bits = native_bits>
struct BASE_MSG_SXS_STREAMX {
    uint8_t FileType; // 0x0, 0x0
    uint8_t PathType; // 0x1, 0x1
    uint8_t HandleType; // 0x2, 0x2
    __padding(0x1);
    __x64_padding(0x4);
    UNICODE_STRINGX<bits> Path; // 0x4, 0x8
    HANDLEX<bits> FileHandle; // 0xC, 0x18
    HANDLEX<bits> Handle; // 0x10, 0x20
    uint64_tx<bits> Offset ide_packed_field; // 0x14, 0x28
    size_t Size; // 0x1C, 0x30
    // 0x20, 0x38
};
ValidateStructSize(0x20, BASE_MSG_SXS_STREAMX<32>);
ValidateStructAlignment(0x4, BASE_MSG_SXS_STREAMX<32>);
ValidateStructSize(0x38, BASE_MSG_SXS_STREAMX<64>);
ValidateStructAlignment(0x8, BASE_MSG_SXS_STREAMX<64>);

template<size_t bits = native_bits>
using PBASE_MSG_SXS_STREAMX = PTRZX<bits, BASE_MSG_SXS_STREAMX<bits>>;

__if_not_exists(BASE_MSG_SXS_STREAM) {
    using BASE_MSG_SXS_STREAM = BASE_MSG_SXS_STREAMX<>;
}
__if_not_exists(PBASE_MSG_SXS_STREAM) {
    using PBASE_MSG_SXS_STREAM = PBASE_MSG_SXS_STREAMX<>;
}

template<size_t bits = native_bits>
struct BASE_SXS_CREATEPROCESS_MSGX {
    uint32_t Flags; // 0x0, 0x0
    __x64_padding(0x4);
    BASE_MSG_SXS_STREAMX<bits> Manifest; // 0x4, 0x8
    BASE_MSG_SXS_STREAMX<bits> Policy; // 0x24, 0x40
    UNICODE_STRINGX<bits> AssemblyDirectory; // 0x44, 0x78
    // 0x4C, 0x88
};
ValidateStructSize(0x4C, BASE_SXS_CREATEPROCESS_MSGX<32>);
ValidateStructAlignment(0x4, BASE_SXS_CREATEPROCESS_MSGX<32>);
ValidateStructSize(0x88, BASE_SXS_CREATEPROCESS_MSGX<64>);
ValidateStructAlignment(0x8, BASE_SXS_CREATEPROCESS_MSGX<64>);

template<size_t bits = native_bits>
using PBASE_SXS_CREATEPROCESS_MSGX = PTRZX<bits, BASE_SXS_CREATEPROCESS_MSGX<bits>>;

__if_not_exists(BASE_SXS_CREATEPROCESS_MSG) {
    using BASE_SXS_CREATEPROCESS_MSG = BASE_SXS_CREATEPROCESS_MSGX<>;
}
__if_not_exists(PBASE_SXS_CREATEPROCESS_MSG) {
    using PBASE_SXS_CREATEPROCESS_MSG = PBASE_SXS_CREATEPROCESS_MSGX<>;
}

template<size_t bits = native_bits>
struct BASE_CREATEPROCESS_MSGX {
    HANDLEX<bits> ProcessHandle; // 0x0, 0x0
    HANDLEX<bits> ThreadHandle; // 0x4, 0x8
    CLIENT_IDX<bits> ClientId;  // 0x8, 0x10
    uint32_t CreationFlags; // 0x10, 0x20
    uint32_t VdmBinaryType; // 0x14, 0x24
    uint32_t VdmTask; // 0x18, 0x28
    __x64_padding(0x4);
    HANDLEX<bits> hVDM; // 0x1C, 0x30
    BASE_SXS_CREATEPROCESS_MSGX<bits> Sxs; // 0x20, 0x38
    uint64_tx<bits> Peb ide_packed_field; // 0x6C, 0xC0
    uintptr_tx<bits> RealPeb; // 0x74, 0xC8
    uint16_t ProcessorArchitecture; // 0x78, 0xD0
    __padding(0x2);
    __x64_padding(0x4);
    // 0x7C, 0xD8
};
ValidateStructSize(0x7C, BASE_CREATEPROCESS_MSGX<32>);
ValidateStructAlignment(0x4, BASE_CREATEPROCESS_MSGX<32>);
ValidateStructSize(0xD8, BASE_CREATEPROCESS_MSGX<64>);
ValidateStructAlignment(0x8, BASE_CREATEPROCESS_MSGX<64>);

template<size_t bits = native_bits>
using PBASE_CREATEPROCESS_MSGX = PTRZX<bits, BASE_CREATEPROCESS_MSGX<bits>>;

__if_not_exists(BASE_CREATEPROCESS_MSG) {
    using BASE_CREATEPROCESS_MSG = BASE_CREATEPROCESS_MSGX<>;
}
__if_not_exists(PBASE_CREATEPROCESS_MSG) {
    using PBASE_CREATEPROCESS_MSG = PBASE_CREATEPROCESS_MSGX<>;
}

template<size_t bits = native_bits>
struct CSR_CAPTURE_HEADERX {
    uint32_t Length; // 0x0, 0x0
    __x64_padding(0x4);
    PTRZX<bits, CSR_CAPTURE_HEADERX<bits>> RelatedCaptureBuffer; // 0x4, 0x8
    uint32_t CountMessagePointers; // 0x8, 0x10
    __x64_padding(0x4);
    PTRZX<bits, char> FreeSpace; // 0xC, 0x18
    uintptr_tx<bits> MessagePointerOffsets[1]; // 0x10, 0x20
    // 0x14, 0x28
};

template<size_t bits = native_bits>
using PCSR_CAPTURE_HEADERX = PTRZX<bits, CSR_CAPTURE_HEADERX<bits>>;

__if_not_exists(CSR_CAPTURE_HEADER) {
    using CSR_CAPTURE_HEADER = CSR_CAPTURE_HEADERX<>;
}
__if_not_exists(PCSR_CAPTURE_HEADER) {
    using PCSR_CAPTURE_HEADER = PCSR_CAPTURE_HEADERX<>;
}

using CSR_API_NUMBERX = uint32_t;

__if_not_exists(CSR_API_NUMBER) {
    using CSR_API_NUMBER = CSR_API_NUMBERX;
}

#ifdef CreateProcess
#pragma push_macro("CreateProcess")
#undef CreateProcess
#endif

template<size_t bits = native_bits>
struct BASE_API_MSGX {
    PORT_MESSAGEX<bits> h; // 0x0, 0x0
    PCSR_CAPTURE_HEADERX<bits> CaptureBuffer; // 0x18, 0x20
    CSR_API_NUMBER ApiNumber; // 0x1C, 0x28
    uint32_t ReturnValue; // 0x20, 0x2C
    uint32_t Reserved; // 0x24, 0x30
    __x64_padding(0x4);
    union {
        BASE_CREATEPROCESS_MSGX<bits> CreateProcess; // 0x28, 0x38
    } u;
};

#pragma pop_macro("CreateProcess")

template<size_t bits = native_bits>
using PBASE_API_MSGX = PTRZX<bits, BASE_API_MSGX<bits>>;

__if_not_exists(BASE_API_MSG) {
    using BASE_API_MSG = BASE_API_MSGX<>;
}
__if_not_exists(PBASE_API_MSG) {
    using PBASE_API_MSG = PBASE_API_MSGX<>;
}

enum BASESRV_API_NUMBERX : int32_t {
    BasepCreateProcessX = 0,
    BasepCreateThreadX
};
__if_not_exists(BASESRV_API_NUMBER) {
    using BASESRV_API_NUMBER = BASESRV_API_NUMBERX;
}
__if_not_exists(BasepCreateProcess) {
    static constexpr auto BasepCreateProcess = (BASESRV_API_NUMBER)BasepCreateProcessX;
}
__if_not_exists(BasepCreateThread) {
    static constexpr auto BasepCreateThread = (BASESRV_API_NUMBER)BasepCreateThreadX;
}

/*========================================
    Kernel Mode Stuff
========================================*/

template<size_t bits = native_bits>
struct DBGKD_GET_VERSION64X {
    uint16_t MajorVersion; // 0x0
    uint16_t MinorVersion; // 0x2
    uint8_t ProtocolVersion; // 0x4
    uint8_t KdSecondaryVersion; // 0x5
    union {
        uint16_t Flags; // 0x6
        struct {
        };
    };
    uint16_t MachineType; // 0x8
    uint8_t MaxPacketType; // 0xA
    uint8_t MaxStateChange; // 0xB
    uint8_t MaxManipulate; // 0xC
    uint8_t Simulation; // 0xD
    __padding(0x2); // 0xE
    uint64_tx<bits> KernBase; // 0x10
    uint64_tx<bits> PsLoadedModuleList; // 0x18
    uint64_tx<bits> DebuggerDataList; // 0x20
    // 0x28
};
ValidateStructSize(0x28, DBGKD_GET_VERSION64X<32>);
//ValidateStructAlignment(0x4, DBGKD_GET_VERSION64X<32>);
ValidateStructSize(0x28, DBGKD_GET_VERSION64X<64>);
ValidateStructAlignment(0x8, DBGKD_GET_VERSION64X<64>);

__if_not_exists(DBGKD_GET_VERSION64) {
    using DBGKD_GET_VERSION64 = DBGKD_GET_VERSION64X<>;
}

template<size_t bits = native_bits>
struct KIDTENTRYX;

template<>
struct alignas(8) KIDTENTRYX<32> {
    uint16_t OffsetLow; // 0x0
    uint16_t Selector; // 0x2
    union {
        uint16_t flags; // 0x4
        struct {
            uint16_t IstIndex : 3;
            uint16_t : 5;
            uint16_t Type : 5;
            uint16_t Dpl : 2;
            uint16_t Present : 1;
        };
    };
    uint16_t OffsetMiddle; // 0x6
    // 0x8
};
ValidateStructSize(0x8, KIDTENTRYX<32>);
ValidateStructAlignment(0x8, KIDTENTRYX<32>);

template<>
struct alignas(16) KIDTENTRYX<64> {
    uint16_t OffsetLow; // 0x0
    uint16_t Selector; // 0x2
    union {
        uint16_t flags; // 0x4
        struct {
            uint16_t IstIndex : 3;
            uint16_t : 5;
            uint16_t Type : 5;
            uint16_t Dpl : 2;
            uint16_t Present : 1;
        };
    };
    uint16_t OffsetMiddle; // 0x6
    uint32_t OffsetHigh; // 0x8
    __x64_padding(0x4); // 0xC
    // 0x10
};
ValidateStructSize(0x10, KIDTENTRYX<64>);
ValidateStructAlignment(0x10, KIDTENTRYX<64>);

__if_not_exists(KIDTENTRY) {
    using KIDTENTRY = KIDTENTRYX<32>;
}
__if_not_exists(KIDTENTRY64) {
    using KIDTENTRY64 = KIDTENTRYX<64>;
}

template<size_t bits = native_bits>
struct KGDTENTRYX;

template<>
struct alignas(8) KGDTENTRYX<32> {
    uint16_t LimitLow; // 0x0
    uint16_t BaseLow; // 0x2
    uint8_t BaseMiddle; // 0x4
    union {
        uint8_t Flags1; // 0x5
        struct {
            uint8_t Type : 5;
            uint8_t Dpl : 2;
            uint8_t Present : 1;
        };
    };
    union {
        uint8_t Flags2; // 0x6
        struct {
            uint8_t LimitHigh : 4;
            uint8_t System : 1;
            uint8_t LongMode : 1;
            uint8_t DefaultBig : 1;
            uint8_t Granularity : 1;
        };
    };
    uint8_t BaseHigh; // 0x7
    // 0x8
};
ValidateStructSize(0x8, KGDTENTRYX<32>);
ValidateStructAlignment(0x8, KGDTENTRYX<32>);

template<>
struct alignas(8) KGDTENTRYX<64> {
    uint16_t LimitLow; // 0x0
    uint16_t BaseLow; // 0x2
    uint8_t BaseMiddle; // 0x4
    union {
        uint8_t Flags1; // 0x5
        struct {
            uint8_t Type : 5;
            uint8_t Dpl : 2;
            uint8_t Present : 1;
        };
    };
    union {
        uint8_t Flags2; // 0x6
        struct {
            uint8_t LimitHigh : 4;
            uint8_t System : 1;
            uint8_t LongMode : 1;
            uint8_t DefaultBig : 1;
            uint8_t Granularity : 1;
        };
    };
    uint8_t BaseHigh; // 0x7
    uint32_t BaseUpper; // 0x8
    uint32_t MustBeZero; // 0xC
    // 0x10
};
ValidateStructSize(0x10, KGDTENTRYX<64>);
ValidateStructAlignment(0x8, KGDTENTRYX<64>);

__if_not_exists(KGDTENTRY) {
    using KGDTENTRY = KGDTENTRYX<32>;
}
__if_not_exists(KGDTENTRY64) {
    using KGDTENTRY64 = KGDTENTRYX<64>;
}

struct KiIoAccessMapX {
    uint8_t DirectionMap[32]; // 0x0
    uint8_t IoMap[0x2004]; // 0x20
    // 0x2024
};
ValidateStructSize(0x2024, KiIoAccessMapX);
ValidateStructAlignment(0x1, KiIoAccessMapX);

template<size_t bits = native_bits>
struct KTSSX;

template<>
struct KTSSX<32> {
    alignas(4) uint16_t Backlink; // 0x0
    uint32_t Esp0; // 0x4
    alignas(4) uint16_t Ss0; // 0x8
    uint32_t Esp1; // 0xC
    alignas(4) uint16_t Ss1; // 0x10
    uint32_t Esp2; // 0x14
    alignas(4) uint16_t Ss2; // 0x18
    uint32_t CR3; // 0x1C
    uint32_t Eip; // 0x20
    uint32_t EFlags; // 0x24
    uint32_t Eax; // 0x28
    uint32_t Ecx; // 0x2C
    uint32_t Edx; // 0x30
    uint32_t Ebx; // 0x34
    uint32_t Esp; // 0x38
    uint32_t Ebp; // 0x3C
    uint32_t Esi; // 0x40
    uint32_t Edi; // 0x44
    alignas(4) uint16_t Es; // 0x48
    alignas(4) uint16_t Cs; // 0x4C
    alignas(4) uint16_t Ss; // 0x50
    alignas(4) uint16_t Ds; // 0x54
    alignas(4) uint16_t Fs; // 0x58
    alignas(4) uint16_t Gs; // 0x5C
    alignas(4) uint16_t LDT; // 0x60
    union {
        uint16_t Flags; // 0x64
        struct {
            uint16_t Trap : 1;
        };
    };
    uint16_t IoMapBase; // 0x66
    KiIoAccessMapX IoMaps; // 0x68
    uint8_t IntDirectionMap[32]; // 0x208C
    // 0x20AC
};
ValidateStructSize(0x20AC, KTSSX<32>);
ValidateStructAlignment(0x4, KTSSX<32>);

template<>
struct KTSSX<64> {
    uint32_t Reserved0; // 0x0
    uint64_t Rsp0 packed_field; // 0x4
    uint64_t Rsp1 packed_field; // 0xC
    uint64_t Rsp2 packed_field; // 0x14
    uint64_t Ist[8] packed_field; // 0x1C
    uint64_t Reserved1 packed_field; // 0x5C
    uint16_t Reserved2; // 0x64
    uint16_t IoMapBase; // 0x66
    // 0x68
};
ValidateStructSize(0x68, KTSSX<64>);
ValidateStructAlignment(0x4, KTSSX<64>);

__if_not_exists(KTSS) {
    using KTSS = KTSSX<32>;
}
__if_not_exists(KTSS64) {
    using KTSS64 = KTSSX<64>;
}

template<size_t bits = native_bits>
struct KSPIN_LOCK_QUEUEX {
    volatile PTRZX<bits, KSPIN_LOCK_QUEUEX<bits>> Next; // 0x0, 0x0
    volatile PTRZX<bits, uint32_t> Lock; // 0x4, 0x8
    // 0x8, 0x10
};
ValidateStructSize(0x8, KSPIN_LOCK_QUEUEX<32>);
ValidateStructAlignment(0x4, KSPIN_LOCK_QUEUEX<32>);
ValidateStructSize(0x10, KSPIN_LOCK_QUEUEX<64>);
ValidateStructAlignment(0x8, KSPIN_LOCK_QUEUEX<64>);

__if_not_exists(KSPIN_LOCK_QUEUE) {
    using KSPIN_LOCK_QUEUE = KSPIN_LOCK_QUEUEX<>;
}

template<size_t bits = native_bits>
struct KTHREADX {
};

template<size_t bits = native_bits>
struct KPRCBX;

template<>
struct KPRCBX<32> {
    uint16_t MinorVersion; // 0x0
    uint16_t MajorVersion; // 0x2
    PTR32Z<KTHREADX<32>> CurrentThread; // 0x4
    PTR32Z<KTHREADX<32>> NextThread; // 0x8
    PTR32Z<KTHREADX<32>> IdleThread; // 0xC
    uint8_t LegacyNumber; // 0x10
    uint8_t NestingLevel; // 0x11
    uint16_t BuildType; // 0x12
};

template<>
struct KPRCBX<64> {
    uint32_t MxCsr; // 0x0
    uint8_t LegacyNumber; // 0x4
    uint8_t NestingLevel; // 0x5
    bool InterruptRequest; // 0x6
    bool IdleHalt; // 0x7

};

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct KPCRX;

template<size_t bits = native_bits, typename T = FXSAVE_DEFAULT_PADDING>
struct KPCRX_base;

template<typename T>
struct KPCRX_base<32, T> {
    union {
        NT_TIBX<32, T> NtTib; // 0x0
        uint8_t arbitrary_offset[0]; // 0x0
        struct {
            uint32_t : 32;
            uint32_t : 32;
            // Begin Windows 10+
            uint32_t MxCsr; // 0x8
            // End Windows 10+
            PTR32Z<KTSSX<32>> TssCopy; // 0xC
            uint32_t ContextSwitches; // 0x10
            KAFFINITYX<32> SetMemberCopy; // 0x14
            PTR32Z<TEBX<32>> Used_Self; // 0x18
        };
    };
    PTR32Z<KPCRX<32, T>> Self; // 0x1C
    PTR32Z<KPRCBX<32>> Prcb; // 0x20
    KIRQLX Irql; // 0x24
    __x86_padding(0x3); // 0x25 (do not use, sometimes written as 32 bits)
    uint32_t IRR; // 0x28
    uint32_t IrrActive; // 0x2C
    uint32_t IDR; // 0x30
    PTR32Z<DBGKD_GET_VERSION64X<32>> KdVersionBlock; // 0x34
    PTR32Z<KIDTENTRYX<32>> IDT; // 0x38
    PTR32Z<KGDTENTRYX<32>> GDT; // 0x3C
    PTR32Z<KTSSX<32>> TSS; // 0x40
    uint16_t MajorVersion; // 0x44
    uint16_t MinorVersion; // 0x46
    KAFFINITYX<32> SetMember; // 0x48
    uint32_t StallScaleFactor; // 0x4C
    // Begin Windows XP-
    uint8_t DebugActive; // 0x50
    // End Windows XP-
    uint8_t Number; // 0x51
    uint8_t Spare0; // 0x52
    uint8_t SecondLevelCacheAssociativity; // 0x53
    uint8_t VdmAlert; // 0x54
    __x86_padding(0x3); // 0x55
    uint32_t KernelReserved[14]; // 0x58
    uint32_t SecondLevelCacheSize; // 0x90
    uint32_t HalReserved[16]; // 0x94
    uint32_t InterruptMode; // 0xD4
    uint8_t Spare1; // 0xD8
    __x86_padding(0x3); // 0xD9
    uint32_t KernelReserved2[17]; // 0xDC
    // 0x120
};
ValidateStructSize(0x120, KPCRX_base<32>);
ValidateStructAlignment(0x4, KPCRX_base<32>);

template<typename T>
struct alignas(64) KPCRX_base<64, T> {
    union {
        NT_TIBX<64, T> NtTib; // 0x0
        uint8_t arbitrary_offset[0]; // 0x0
        struct {
            PTR64Z<KGDTENTRYX<64>> GdtBase; // 0x0
            PTR64Z<KTSSX<64>> TssBase; // 0x8
            uint64_t UserRsp; // 0x10
            PTR64Z<KPCRX<64, T>> Self; // 0x18
            PTR64Z<KPRCBX<64>> CurrentPrcb; // 0x20
            PTR64Z<KSPIN_LOCK_QUEUEX<64>> LockArray; // 0x28
            PTR64Z<TEBX<64>> Used_Self; // 0x30
        };
    };
    PTR64Z<KIDTENTRYX<64>> IdtBase; // 0x38
    uint64_t Unused[2]; // 0x40
    KIRQLX Irql; // 0x50
    uint8_t SecondLevelCacheAssociativity; // 0x51
    uint8_t ObsoleteNumber; // 0x52
    uint8_t Fill0; // 0x53
    uint32_t Unused0[3]; // 0x54
    uint16_t MajorVersion; // 0x60
    uint16_t MinorVersion; // 0x62
    uint32_t StallScaleFactor; // 0x64
    PTR64Z<> Unused1[3]; // 0x68
    uint32_t KernelReserved[15]; // 0x80
    uint32_t SecondLevelCacheSize; // 0xBC
    uint32_t HalReserved[16]; // 0xC0
    uint32_t Unused2; // 0x100
    PTR64Z<> KdVersionBlock; // 0x108
    PTR64Z<> Unused3; // 0x110
    uint32_t PcrAlign1[24]; // 0x118
    // 0x180
};
ValidateStructSize(0x180, KPCRX_base<64>);
ValidateStructAlignment(64, KPCRX_base<64>);

template<typename T>
struct KPCRX<32, T> : KPCRX_base<32, T> {
    KPRCBX<32> PrcbData;
};

template<typename T>
struct KPCRX<64, T> : KPCRX_base<64, T> {
    KPRCBX<64> Prcb;
};

__if_not_exists(KPCR) {
    using KPCR = KPCRX<>;
}

/*========================================
    Functions
========================================*/

//__if_not_exists(RtlInitUnicodeString) {
template<size_t bits>
static constexpr void RtlInitUnicodeString(UNICODE_STRINGX<bits>* unicode_string, const wchar_t* string) {
    if (expect(string != NULL, true)) {
        size_t length = byteloop_wcslen_raw(string);
        if (length >= 0x7FFC) length = 0x7FFC;
        unicode_string->Length = length;
        unicode_string->MaximumLength = length + 2;
        unicode_string->Buffer = (PTRZX<bits, wchar_t>)string;
    } else {
        __builtin_memset(unicode_string, 0, sizeof(*unicode_string));
    }
}
//}

#endif