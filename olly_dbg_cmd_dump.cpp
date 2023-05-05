#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "zero/util.h"
#include "zero/custom_intrin.h"

#include <utility>
#include <bit>

#include <Windows.h>

enum CmdType : uint32_t {
	DT_CMD = 0x00,
	DT_MOV = 0x01,
	DT_MOVC = 0x02,
	DT_SETC = 0x03,
	DT_TEST = 0x04,
	DT_STRING = 0x05,
	DT_JMP = 0x06,
	DT_JMPFAR = 0x07,
	DT_JMC = 0x08,
	DT_JMCX = 0x09,
	DT_PUSH = 0x0A,
	DT_POP = 0x0B,
	DT_CALL = 0x0C,
	DT_CALLFAR = 0x0D,
	DT_INT = 0x0E,
	DT_RET = 0x0F,
	DT_RETFAR = 0x10,
	DT_FPU = 0x11,
	DT_MMX = 0x12,
	DT_3DNOW = 0x13,
	DT_SSE = 0x14,
	DT_IO = 0x15,
	DT_SYS = 0x16,
	DT_PRIVILEGED = 0x17,
	DT_AVX = 0x18,
	DT_XOP = 0x19,
	DT_UNUSED1 = 0x1A,
	DT_UNUSED2 = 0x1B,
	DT_DATA = 0x1C,
	DT_PSEUDO = 0x1D,
	DT_PREFIX = 0x1E,
	DT_BAD = 0x1F,

	ENUM_MAX_VALUE_DECLARE(CmdType)
};
static inline constexpr size_t CmdType_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(CmdType));
static_assert(CmdType_Bits == 5);

enum PrefixRequirements : uint32_t {
	DP_NOMUST = 0x00,
	DP_MUST66 = 0x01,
	DP_MUSTF2 = 0x02,
	DP_MUSTF3 = 0x03,
	DP_MUSTNONE = 0x04,
	DP_NEEDF2 = 0x05,
	DP_NEEDF3 = 0x06,
	DP_NOREP = 0x07,
	DP_MUSTREP = 0x08,
	DP_MUSTREPE = 0x09,
	DP_MUSTREPNE = 0x0A,
	DP_UNUSED1 = 0x0B,
	DP_UNUSED2 = 0x0C,
	DP_UNUSED3 = 0x0D,
	DP_UNUSED4 = 0x0E,
	DP_UNUSED5 = 0x0F,

	ENUM_MAX_VALUE_DECLARE(PrefixRequirements)
};
static inline constexpr size_t PrefixRequirements_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(PrefixRequirements));
static_assert(PrefixRequirements_Bits == 4);

enum FlagUpdates : uint32_t {
	DF_NOFLAGS = 0x0,
	DF_ALLFLAGS = 0x1,
	DF_FLAGZ = 0x2,
	DF_FLAGC = 0x3,
	DF_FLAGSCO = 0x4,
	DF_FLAGD = 0x5,
	DF_FLAGSZPC = 0x6,
	D_NOCFLAG = 0x7,

	ENUM_MAX_VALUE_DECLARE(FlagUpdates)
};
static inline constexpr size_t FlagUpdates_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(FlagUpdates));
static_assert(FlagUpdates_Bits == 3);

enum X87Updates : uint32_t {
	D7_FPUSAME = 0x0,
	D7_FPUPOP = 0x1,
	D7_FPUPOP2 = 0x2,
	D7_FPUPUSH = 0x3,

	ENUM_MAX_VALUE_DECLARE(X87Updates)
};
static inline constexpr size_t X87Updates_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(X87Updates));
static_assert(X87Updates_Bits == 2);

enum UnusualFlags : uint32_t {
	DU_NORMAL = 0x0,
	DU_RARE = 0x1,
	DU_SUSPICIOUS = 0x2,
	DU_UNDOC = 0x3,

	ENUM_MAX_VALUE_DECLARE(UnusualFlags)
};
static inline constexpr size_t UnusualFlags_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(UnusualFlags));
static_assert(UnusualFlags_Bits == 2);

enum ZeroFlagDecode : uint32_t {
	DXZ_NO_PREFERENCE = 0x0,
	DXZ_PREFER_JE = 0x1,
	DXZ_PREFER_JZ = 0x2,
	DXZ_UNUSED1 = 0x3,

	ENUM_MAX_VALUE_DECLARE(ZeroFlagDecode)
};
static inline constexpr size_t ZeroFlagDecode_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(ZeroFlagDecode));
static_assert(ZeroFlagDecode_Bits == 2);

enum CarryFlagDecode : uint32_t {
	DXC_NO_PREFERENCE = 0x0,
	DXC_PREFER_JB = 0x1,
	DXC_PREFER_JC = 0x2,
	DXC_UNUSED1 = 0x3,

	ENUM_MAX_VALUE_DECLARE(CarryFlagDecode)
};
static inline constexpr size_t CarryFlagDecode_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(CarryFlagDecode));
static_assert(CarryFlagDecode_Bits == 2);

enum VexLength : uint32_t {
	DXV_LSHORT = 0x0,
	DXV_LBOTH = 0x1,
	DXV_LLONG = 0x2,
	DXV_IGNOREL = 0x3,

	ENUM_MAX_VALUE_DECLARE(VexLength)
};
static inline constexpr size_t VexLength_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(VexLength));
static_assert(VexLength_Bits == 2);

enum VexWideRequirement : uint32_t {
	DXW_NONE = 0x0,
	DXW_REQUIRE_0 = 0x1,
	DXW_REQUIRE_1 = 0x2,
	DXW_UNUSED1 = 0x3,

	ENUM_MAX_VALUE_DECLARE(VexWideRequirement)
};
static inline constexpr size_t VexWideRequirement_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(VexWideRequirement));
static_assert(VexWideRequirement_Bits == 2);

enum VexOpcodePage : uint32_t {
	DXP_LEAD_0F = 0x0,
	DXP_LEAD_0F38 = 0x1,
	DXP_LEAD_0F3A = 0x2,
	DXP_UNUSED1 = 0x3,
	DXP_UNUSED2 = 0x4,
	DXP_UNUSED3 = 0x5,
	DXP_UNUSED4 = 0x6,
	DXP_UNUSED5 = 0x7,

	ENUM_MAX_VALUE_DECLARE(VexOpcodePage)
};
static inline constexpr size_t VexOpcodePage_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(VexOpcodePage));
static_assert(VexOpcodePage_Bits == 3);

enum CmdTypeEx32 : uint32_t {
	DXT32_DEFAULT = 0x00,
	DXT32_ADD = 0x1,
	DXT32_SUB = 0x2,
	DXT32_LEA = 0x3,
	DXT32_NOP = 0x4
};

enum CmdTypeEx64 : uint32_t {
	DXT64_DEFAULT = 0x00,
	DXT64_NOP = 0x01
};

enum ArgType32 : uint32_t {
	BT32_NONE = 0x00,
	BT32_AL = 0x01,
	BT32_AH = 0x02,
	BT32_AX = 0x03,
	BT32_CL = 0x04,
	BT32_CX = 0x05,
	BT32_DX = 0x06,
	BT32_DXPORT = 0x07,
	BT32_EAX = 0x08,
	BT32_EBX = 0x09,
	BT32_ECX = 0x0A,
	BT32_EDX = 0x0B,
	BT32_ACC = 0x0C,
	BT32_STRCNT = 0x0D,
	BT32_DXEDX = 0x0E,
	BT32_BPEBP = 0x0F,
	BT32_REG = 0x10,
	BT32_REG16 = 0x11,
	BT32_REG32 = 0x12,
	BT32_REGCMD = 0x13,
	BT32_REGCMD8 = 0x14,
	BT32_ANYREG = 0x15,
	BT32_INT = 0x16,
	BT32_INT8 = 0x17,
	BT32_INT16 = 0x18,
	BT32_INT32 = 0x19,
	BT32_INT1632 = 0x1A,
	BT32_INT64 = 0x1B,
	BT32_INT128 = 0x1C,
	BT32_IMMINT = 0x1D,
	BT32_INTPAIR = 0x1E,
	BT32_SEGOFFS = 0x1F,
	BT32_STRDEST = 0x20,
	BT32_STRDEST8 = 0x21,
	BT32_STRSRC = 0x22,
	BT32_STRSRC8 = 0x23,
	BT32_XLATMEM = 0x24,
	BT32_EAXMEM = 0x25,
	BT32_LONGDATA = 0x26,
	BT32_ANYMEM = 0x27,
	BT32_STKTOP = 0x28,
	BT32_STKTOPFAR = 0x29,
	BT32_STKTOPEFL = 0x2A,
	BT32_STKTOPA = 0x2B,
	BT32_PUSH = 0x2C,
	BT32_PUSHRET = 0x2D,
	BT32_PUSHRETF = 0x2E,
	BT32_PUSHA = 0x2F,
	BT32_EBPMEM = 0x30,
	BT32_SEG = 0x31,
	BT32_SEGNOCS = 0x32,
	BT32_SEGCS = 0x33,
	BT32_SEGDS = 0x34,
	BT32_SEGES = 0x35,
	BT32_SEGFS = 0x36,
	BT32_SEGGS = 0x37,
	BT32_SEGSS = 0x38,
	BT32_ST = 0x39,
	BT32_ST0 = 0x3A,
	BT32_ST1 = 0x3B,
	BT32_FLOAT32 = 0x3C,
	BT32_FLOAT64 = 0x3D,
	BT32_FLOAT80 = 0x3E,
	BT32_BCD = 0x3F,
	BT32_MREG8x8 = 0x40,
	BT32_MMX8x8 = 0x41,
	BT32_MMX8x8DI = 0x42,
	BT32_MREG16x4 = 0x43,
	BT32_MMX16x4 = 0x44,
	BT32_MREG32x2 = 0x45,
	BT32_MMX32x2 = 0x46,
	BT32_MREG64 = 0x47,
	BT32_MMX64 = 0x48,
	BT32_3DREG = 0x49,
	BT32_3DNOW = 0x4A,
	BT32_XMM0I32x4 = 0x4B,
	BT32_XMM0I64x2 = 0x4C,
	BT32_XMM0I8x16 = 0x4D,
	BT32_SREGF32x4 = 0x4E,
	BT32_SREGF32L = 0x4F,
	BT32_SREGF32x2L = 0x50,
	BT32_SSEF32x4 = 0x51,
	BT32_SSEF32L = 0x52,
	BT32_SSEF32x2L = 0x53,
	BT32_SREGF64x2 = 0x54,
	BT32_SREGF64L = 0x55,
	BT32_SSEF64x2 = 0x56,
	BT32_SSEF64L = 0x57,
	BT32_SREGI8x16 = 0x58,
	BT32_SSEI8x16 = 0x59,
	BT32_SSEI8x16DI = 0x5A,
	BT32_SSEI8x8L = 0x5B,
	BT32_SSEI8x4L = 0x5C,
	BT32_SSEI8x2L = 0x5D,
	BT32_SREGI16x8 = 0x5E,
	BT32_SSEI16x8 = 0x5F,
	BT32_SSEI16x4L = 0x60,
	BT32_SSEI16x2L = 0x61,
	BT32_SREGI32x4 = 0x62,
	BT32_SREGI32L = 0x63,
	BT32_SREGI32x2L = 0x64,
	BT32_SSEI32x4 = 0x65,
	BT32_SSEI32x2L = 0x66,
	BT32_SREGI64x2 = 0x67,
	BT32_SSEI64x2 = 0x68,
	BT32_SREGI64L = 0x69,
	BT32_EFL = 0x6A,
	BT32_FLAGS8 = 0x6B,
	BT32_OFFSET = 0x6C,
	BT32_BYTEOFFS = 0x6D,
	BT32_FARCONST = 0x6E,
	BT32_DESCR = 0x6F,
	BT32_1 = 0x70,
	BT32_CONST8 = 0x71,
	BT32_CONST8_2 = 0x72,
	BT32_CONST16 = 0x73,
	BT32_CONST = 0x74,
	BT32_CONSTL = 0x75,
	BT32_SXTCONST = 0x76,
	BT32_CR = 0x77,
	BT32_CR0 = 0x78,
	BT32_DR = 0x79,
	// x64 added one somewhere...
	BT32_FST = 0x7A,
	BT32_FCW = 0x7B,
	BT32_MXCSR = 0x7C,
	BT32_SVEXF32x4 = 0x7D,
	BT32_SVEXF32L = 0x7E,
	BT32_SVEXF64x2 = 0x7F,
	BT32_SVEXF64L = 0x80,
	BT32_SVEXI8x16 = 0x81,
	BT32_SVEXI16x8 = 0x82,
	BT32_SVEXI32x4 = 0x83,
	BT32_SVEXI64x2 = 0x84,
	BT32_SIMMI8x16 = 0x85,
	// My own additional types...
	BT32_REG32VEX = 0x86,
	BT32_CONST32 = 0x87
};
enum ArgType64 : uint32_t {
	BT64_NONE = 0x00,
	BT64_AL = 0x01,
	BT64_AX = 0x02,
	BT64_EAX = 0x03,
	BT64_RAX = 0x04,
	BT64_ACC = 0x05,
	BT64_DXEDX = 0x06,
	BT64_AH = 0x07,
	BT64_CL = 0x08,
	BT64_ECX = 0x09,
	BT64_RCX = 0x0A,
	BT64_DXPORT = 0x0B,
	BT64_EDX = 0x0C,
	BT64_RDX = 0x0D,
	BT64_EBX = 0x0E,
	BT64_RBX = 0x0F,
	BT64_STRCNT = 0x10,
	BT64_BPRBP = 0x11,
	BT64_REG = 0x12,
	BT64_REG3264 = 0x13,
	BT64_REGCMD = 0x14,
	BT64_REGCMD8 = 0x15,
	BT64_ANYREG = 0x16,
	BT64_INT = 0x17,
	BT64_INT8 = 0x18,
	BT64_INT16 = 0x19,
	BT64_INT32 = 0x1A,
	BT64_INT1632 = 0x1B, // Unused?
	BT64_INT64 = 0x1C,
	BT64_INT128 = 0x1D,
	BT64_IMMINT = 0x1E,
	BT64_SEGOFFS = 0x1F,
	BT64_STRDEST = 0x20,
	BT64_STRDEST8 = 0x21,
	BT64_STRSRC = 0x22,
	BT64_STRSRC8 = 0x23,
	BT64_XLATMEM = 0x24,
	BT64_EAXMEM = 0x25, // ACCMEM
	BT64_LONGDATA = 0x26,
	BT64_ANYMEM = 0x27,
	BT64_STKTOP = 0x28,
	BT64_STKTOPFAR = 0x29,
	BT64_STKTOPEFL = 0x2A,
	// BT64_STKTOPA = 0x2B,
	BT64_PUSH = 0x2C,
	BT64_PUSHRET = 0x2D,
	// BT64_PUSHRETF = 0x2E,
	// BT64_PUSHA = 0x2F,
	BT64_RBPMEM = 0x30,
	BT64_SEG = 0x31,
	BT64_SEGNOCS = 0x32,
	// BT64_SEGCS = 0x33,
	// BT64_SEGDS = 0x34,
	// BT64_SEGES = 0x35,
	BT64_SEGFS = 0x36,
	BT64_SEGGS = 0x37,
	BT64_SEGSS = 0x38,
	BT64_ST = 0x39,
	BT64_ST0 = 0x3A,
	BT64_ST1 = 0x3B,
	BT64_FLOAT32 = 0x3C,
	BT64_FLOAT64 = 0x3D,
	BT64_FLOAT80 = 0x3E,
	BT64_BCD = 0x3F,
	BT64_MREG8x8 = 0x40,
	BT64_MMX8x8 = 0x41,
	BT64_MMX8x8DI = 0x42,
	BT64_MREG16x4 = 0x43,
	BT64_MMX16x4 = 0x44,
	BT64_MREG32x2 = 0x45,
	BT64_MMX32x2 = 0x46,
	BT64_MREG64 = 0x47,
	BT64_MMX64 = 0x48,
	BT64_3DREG = 0x49,
	BT64_3DNOW = 0x4A,
	BT64_XMM0I32x4 = 0x4B,
	BT64_XMM0I64x2 = 0x4C,
	BT64_XMM0I8x16 = 0x4D,
	BT64_SREGF32x4 = 0x4E,
	BT64_SREGF32L = 0x4F,
	BT64_SREGF32x2L = 0x50,
	BT64_SSEF32x4 = 0x51,
	BT64_SSEF32L = 0x52,
	BT64_SSEF32x2L = 0x53,
	BT64_SREGF64x2 = 0x54,
	BT64_SREGF64L = 0x55,
	BT64_SSEF64x2 = 0x56,
	BT64_SSEF64L = 0x57,
	BT64_SREGI8x16 = 0x58,
	BT64_SSEI8x16 = 0x59,
	BT64_SSEI8x16DI = 0x5A,
	BT64_SSEI8x8L = 0x5B,
	BT64_SSEI8x4L = 0x5C,
	BT64_SSEI8x2L = 0x5D,
	BT64_SREGI16x8 = 0x5E,
	BT64_SSEI16x8 = 0x5F,
	BT64_SSEI16x4L = 0x60,
	BT64_SSEI16x2L = 0x61,
	BT64_SREGI32x4 = 0x62,
	BT64_SREGI32L = 0x63,
	BT64_SREGI32x2L = 0x64,
	BT64_SSEI32x4 = 0x65,
	BT64_SSEI32x2L = 0x66,
	BT64_SREGI64x2 = 0x67,
	BT64_SSEI64x2 = 0x68,
	BT64_SREGI64L = 0x69,
	BT64_EFL = 0x6A,
	BT64_FLAGS8 = 0x6B,
	BT64_OFFSET = 0x6C, // Sign Extend
	BT64_BYTEOFFS = 0x6D, // Sign Extend
	// BT64_FARCONST = 0x6E,
	BT64_DESCR = 0x6F,
	BT64_1 = 0x70,
	BT64_CONST8 = 0x71, // Zero Extend
	BT64_CONST8_2 = 0x72, // Zero Extend
	BT64_CONST16 = 0x73, // Zero Extend
	BT64_CONST = 0x74, // Sign Extend
	BT64_CONSTL = 0x75, // No Extend
	// ???
	BT64_SXTCONST = 0x77, // Sign Extend
	BT64_CR = 0x78,
	BT64_CR0 = 0x79,
	BT64_DR = 0x7A,
	BT64_FST = 0x7B,
	BT64_FCW = 0x7C,
	BT64_MXCSR = 0x7D,
	BT64_SVEXF32x4 = 0x7E,
	BT64_SVEXF32L = 0x7F,
	BT64_SVEXF64x2 = 0x80,
	BT64_SVEXF64L = 0x81,
	BT64_SVEXI8x16 = 0x82,
	BT64_SVEXI16x8 = 0x83,
	BT64_SVEXI32x4 = 0x84,
	BT64_SVEXI64x2 = 0x85,
	BT64_SIMMI8x16 = 0x86,
	// My own additional types...

};


enum ArgModifier32 : uint32_t {
	BM32_NONSPEC = 0x0,
	BM32_UNSIGNED = 0x1,
	BM32_SIGNED = 0x2,
	BM32_BINARY = 0x3,
	BM32_BITCNT = 0x4,
	BM32_SHIFTCNT = 0x5,
	BM32_COUNT = 0x6,
	BM32_NOADDR = 0x7,
	BM32_JMPCALL = 0x8,
	BM32_JMPCALLFAR = 0x9,
	BM32_STACKINC = 0xA,
	BM32_PORT = 0xB,
	BM32_UNUSED1 = 0xC,
	BM32_UNUSED2 = 0xD,
	BM32_UNUSED3 = 0xE,
	BM32_UNUSED4 = 0xF,

	ENUM_MAX_VALUE_DECLARE(ArgModifier32)
};
static inline constexpr size_t ArgModifier32_Bits = std::bit_width<size_t>(ENUM_MAX_VALUE(ArgModifier32));
static_assert(ArgModifier32_Bits == 4);

// 0x4
union BinCmdArg {
	uint32_t raw; // 0x0
	struct {
		ArgType32 type			: 8; // 0x000000FF
		uint32_t				: 8; // 0x0000FF00
		ArgModifier32 modifier	: 4; // 0x000F0000
		uint32_t				: 12;
	};
	struct {
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool : 1;
		bool memory_version				: 1; // 0x00100000
		bool register_version			: 1; // 0x00200000
		bool memory_only				: 1; // 0x00400000
		bool register_only				: 1; // 0x00800000
		bool _identifier(32bit_only)	: 1; // 0x01000000
		bool no_esp						: 1; // 0x02000000
		bool __unknown_flag_J			: 1; // 0x04000000
		bool always_show_size			: 1; // 0x08000000
		bool old_contents_not_used		: 1; // 0x10000000
		bool modified_with_old_contents	: 1; // 0x20000000
		bool pseudo_operand				: 1; // 0x40000000
		bool ignore_selector_offset		: 1; // 0x80000000
	};
	// 0x4
};

// size: 0x34
struct BinCmd32 {
	PTR32Z<wchar_t> name; // 0x0
	union {
		uint32_t cmdtype; // 0x4
		PTR32Z<> cmdtype_hex;
		struct {
			CmdType type							: 5; // 0x0000001F
			uint32_t								: 7;
			PrefixRequirements prefix_requirements	: 4; // 0x0000F000
			uint32_t								: 4;
			FlagUpdates flags_updated				: 3; // 0x00700000
			X87Updates x87_updates					: 2; // 0x01800000
			uint32_t								: 5;
			UnusualFlags							: 2; // 0xC0000000
		};
		struct {
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool size01								: 1; // 0x00000020
			bool postbyte							: 1; // 0x00000040
			bool longform							: 1; // 0x00000080
			bool requires_16_bit_data				: 1; // 0x00000100
			bool requires_32_bit_data				: 1; // 0x00000200
			bool requires_16_bit_address			: 1; // 0x00000400
			bool requires_32_bit_address			: 1; // 0x00000800
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool lockable							: 1; // 0x00010000
			bool branch_hint						: 1; // 0x00020000
			bool mod_must_be_memory					: 1; // 0x00040000
			bool mod_must_be_register				: 1; // 0x00080000
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool changes_stack_pointer				: 1; // 0x02000000
			bool nonstandard_hla					: 1; // 0x04000000
			bool contains_wildcard					: 1; // 0x08000000
			bool is_conditional						: 1; // 0x10000000
			bool uses_carry_flag					: 1; // 0x20000000
			bool									: 1;
			bool									: 1;
		};
		struct {
			uint32_t								: 8;
			uint32_t data_address_size_mask			: 4; // 0x00000F00
		};
		struct {
			uint32_t								: 8;
			uint32_t required_data_size				: 2; // 0x00000300
			uint32_t required_address_size			: 2; // 0x00000C00
		};
	};
	union {
		uint32_t exttype; // 0x8
		PTR32Z<> exttype_hex;
		struct {
			ZeroFlagDecode zero_flag_decode			: 2; // 0x00000003
			CarryFlagDecode carry_flag_decode		: 2; // 0x0000000C
			uint32_t								: 5;
			VexLength vex_length					: 2; // 0x00000600
			uint32_t								: 1;
			VexWideRequirement vex_w_requirement	: 2; // 0x00003000
			uint32_t								: 2;
			VexOpcodePage vex_opcode_page			: 3; // 0x00070000
			uint32_t								: 5;
			CmdTypeEx32 cmdtype_ex					: 8; // 0xFF000000
		};
		struct {
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool near_return						: 1; // 0x00000010
			bool __unknown_flag_A					: 1; // 0x00000020
			bool __unknown_flag_B					: 1; // 0x00000040
			bool __unknown_flag_C					: 1; // 0x00000080
			bool vex								: 1; // 0x00000100
			bool									: 1;
			bool									: 1;
			bool no_vreg							: 1; // 0x00000800
			bool									: 1;
			bool									: 1;
			bool __unknown_flag_D					: 1; // 0x00004000
			bool __unknown_flag_E					: 1; // 0x00008000
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool __unknown_flag_F					: 1; // 0x00080000
			bool __unknown_flag_G					: 1; // 0x00100000
			bool __unknown_flag_H					: 1; // 0x00200000
			bool __unknown_flag_I					: 1; // 0x00400000
			bool wonky_trap							: 1; // 0x00800000
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
		};
	};
	uint32_t main_opcode_length; // 0xC
	union {
		uint32_t opcode_mask; // 0x10
		uint8_t opcode_mask_array[4];
		PTR32Z<> opcode_mask_hex;
		struct {
			uint8_t opcode_mask_byte_1;
			uint8_t opcode_mask_byte_2;
			uint8_t opcode_mask_byte_3;
			uint8_t opcode_mask_byte_4;
		};

	};
	union {
		uint32_t opcode_bytes; // 0x14
		uint8_t opcode_bytes_array[4];
		PTR32Z<> opcode_bytes_hex;
		struct {
			uint8_t opcode_byte_1;
			uint8_t opcode_byte_2;
			uint8_t opcode_byte_3;
			uint8_t opcode_byte_4;
		};
	};
	union {
		uint32_t post_opcode_bytes; // 0x18
		uint8_t post_opcode_bytes_array[4];
		PTR32Z<> post_opcode_bytes_hex;
		struct {
			uint8_t post_opcode_byte_1;
			uint8_t post_opcode_byte_2;
			uint8_t post_opcode_byte_3;
			uint8_t post_opcode_byte_4;
		};
	};
	union {
		BinCmdArg arg_array[4]; // 0x1C
		struct {
			BinCmdArg arg_1; // 0x1C
			BinCmdArg arg_2; // 0x20
			BinCmdArg arg_3; // 0x24
			BinCmdArg arg_4; // 0x28
		};
	};
	PTR32Z<> trace; // 0x2C
	PTR32Z<> emu; // 0x30
	// 0x34
};
// size: 0x40
struct BinCmd64 {
	PTR64<wchar_t> name; // 0x0
	union {
		uint32_t cmdtype; // 0x8
		PTR32Z<> cmdtype_hex;
		struct {
			CmdType type							: 5; // 0x0000001F
			uint32_t								: 7;
			PrefixRequirements prefix_requirements	: 4; // 0x0000F000
			uint32_t								: 4;
			FlagUpdates flags_updated				: 3; // 0x00700000
			X87Updates x87_updates					: 2; // 0x01800000
			uint32_t								: 5;
			UnusualFlags							: 2; // 0xC0000000
		};
		struct {
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool size01								: 1; // 0x00000020
			bool postbyte							: 1; // 0x00000040
			bool longform							: 1; // 0x00000080
			bool requires_16_bit_data				: 1; // 0x00000100
			bool requires_32_bit_data				: 1; // 0x00000200
			bool requires_16_bit_address			: 1; // 0x00000400
			bool requires_32_bit_address			: 1; // 0x00000800
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool lockable							: 1; // 0x00010000
			bool branch_hint						: 1; // 0x00020000
			bool mod_must_be_memory					: 1; // 0x00040000
			bool mod_must_be_register				: 1; // 0x00080000
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool changes_stack_pointer				: 1; // 0x02000000
			bool nonstandard_hla					: 1; // 0x04000000
			bool contains_wildcard					: 1; // 0x08000000
			bool is_conditional						: 1; // 0x10000000
			bool uses_carry_flag					: 1; // 0x20000000
			bool									: 1;
			bool									: 1;
		};
		struct {
			uint32_t								: 8;
			uint32_t data_address_size_mask			: 4; // 0x00000F00
		};
		struct {
			uint32_t								: 8;
			uint32_t required_data_size				: 2; // 0x00000300
			uint32_t required_address_size			: 2; // 0x00000C00
		};
	};
	union {
		uint32_t exttype; // 0xC
		PTR32Z<> exttype_hex;
		struct {
			ZeroFlagDecode zero_flag_decode : 2; // 0x00000003
			CarryFlagDecode carry_flag_decode : 2; // 0x0000000C
			uint32_t : 5;
			VexLength vex_length : 2; // 0x00000600
			uint32_t : 1;
			VexWideRequirement vex_w_requirement : 2; // 0x00003000
			uint32_t : 2;
			VexOpcodePage vex_opcode_page : 3; // 0x00070000
			uint32_t : 5;
			CmdTypeEx64 cmdtype_ex : 8; // 0xFF000000
		};
		struct {
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool near_return : 1; // 0x00000010
			bool __unknown_flag_A : 1; // 0x00000020
			bool __unknown_flag_B : 1; // 0x00000040
			bool __unknown_flag_C : 1; // 0x00000080
			bool vex : 1; // 0x00000100
			bool : 1;
			bool : 1;
			bool no_vreg : 1; // 0x00000800
			bool : 1;
			bool : 1;
			bool __unknown_flag_D : 1; // 0x00004000
			bool __unknown_flag_E : 1; // 0x00008000
			bool : 1;
			bool : 1;
			bool : 1;
			bool __unknown_flag_F : 1; // 0x00080000
			bool __unknown_flag_G : 1; // 0x00100000
			bool __unknown_flag_H : 1; // 0x00200000
			bool __unknown_flag_I : 1; // 0x00400000
			bool wonky_trap : 1; // 0x00800000
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
			bool : 1;
		};
	};
	uint32_t main_opcode_length; // 0x10
	union {
		uint32_t opcode_mask; // 0x14
		uint8_t opcode_mask_array[4];
		PTR32Z<> opcode_mask_hex;
		struct {
			uint8_t opcode_mask_byte_1;
			uint8_t opcode_mask_byte_2;
			uint8_t opcode_mask_byte_3;
			uint8_t opcode_mask_byte_4;
		};

	};
	union {
		uint32_t opcode_bytes; // 0x18
		uint8_t opcode_bytes_array[4];
		PTR32Z<> opcode_bytes_hex;
		struct {
			uint8_t opcode_byte_1;
			uint8_t opcode_byte_2;
			uint8_t opcode_byte_3;
			uint8_t opcode_byte_4;
		};
	};
	union {
		uint32_t post_opcode_bytes; // 0x1C
		uint8_t post_opcode_bytes_array[4];
		PTR32Z<> post_opcode_bytes_hex;
		struct {
			uint8_t post_opcode_byte_1;
			uint8_t post_opcode_byte_2;
			uint8_t post_opcode_byte_3;
			uint8_t post_opcode_byte_4;
		};
	};
	union {
		BinCmdArg arg_array[4]; // 0x20
		struct {
			BinCmdArg arg_1; // 0x20
			BinCmdArg arg_2; // 0x24
			BinCmdArg arg_3; // 0x28
			BinCmdArg arg_4; // 0x2C
		};
	};
	PTR64<> trace; // 0x30
	PTR64<> emu; // 0x38
	// 0x40
};

#pragma region // BinCmd Validation
ValidateFieldOffset(0x0, BinCmd64, name);
ValidateFieldOffset(0x8, BinCmd64, cmdtype);
ValidateFieldOffset(0xC, BinCmd64, exttype);
ValidateFieldOffset(0x10, BinCmd64, main_opcode_length);
ValidateFieldOffset(0x14, BinCmd64, opcode_mask);
ValidateFieldOffset(0x18, BinCmd64, opcode_bytes);
ValidateFieldOffset(0x1C, BinCmd64, post_opcode_bytes);
ValidateFieldOffset(0x20, BinCmd64, arg_array);
ValidateFieldOffset(0x30, BinCmd64, trace);
ValidateFieldOffset(0x38, BinCmd64, emu);
ValidateStructSize(0x40, BinCmd64);

ValidateFieldOffset(0x0, BinCmd32, name);
ValidateFieldOffset(0x4, BinCmd32, cmdtype);
ValidateFieldOffset(0x8, BinCmd32, exttype);
ValidateFieldOffset(0xC, BinCmd32, main_opcode_length);
ValidateFieldOffset(0x10, BinCmd32, opcode_mask);
ValidateFieldOffset(0x14, BinCmd32, opcode_bytes);
ValidateFieldOffset(0x18, BinCmd32, post_opcode_bytes);
ValidateFieldOffset(0x1C, BinCmd32, arg_array);
ValidateFieldOffset(0x2C, BinCmd32, trace);
ValidateFieldOffset(0x30, BinCmd32, emu);
ValidateStructSize(0x34, BinCmd32);
#pragma endregion

gnu_noinline void dump_ollydbg_instructions32(const wchar_t* path, const char* output_path) {
	if (FILE* output_file = fopen(output_path, "w")) {
		output_file = freopen(output_path, "a", output_file);
		if (HMODULE ollydbg_handle = LoadLibraryExW(path, NULL, LOAD_LIBRARY_AS_DATAFILE)) {
			void* ollydbg32_base = based_pointer<void>(ollydbg_handle, -1);
			BinCmd32* instruction_data = based_pointer<BinCmd32>(ollydbg32_base, 0xF6D3C);
			size_t instruction_count = 0;
			for (
				BinCmd32* instruction_data_reader = instruction_data;
				instruction_data_reader->name;
				++instruction_data_reader
			) ++instruction_count;
			BinCmd32* instruction_array = (BinCmd32*)malloc(sizeof(BinCmd32[instruction_count]));
			memcpy(instruction_array, instruction_data, sizeof(BinCmd32[instruction_count]));
			for (size_t i = 0; i < instruction_count; ++i) {
				instruction_array[i].name = based_pointer<wchar_t>(instruction_array[i].name, -0x400C00 + (uintptr_t)ollydbg32_base);
				__builtin_dump_struct(&instruction_array[i], fprintf, output_file);
			}
			FreeLibrary(ollydbg_handle);
		}
		fclose(output_file);
	}
}
gnu_noinline void dump_ollydbg_instructions64(const wchar_t* path, const char* output_path) {
	if (FILE* output_file = fopen(output_path, "w")) {
		output_file = freopen(output_path, "a", output_file);
		if (HMODULE ollydbg_handle = LoadLibraryExW(path, NULL, LOAD_LIBRARY_AS_IMAGE_RESOURCE)) {
			for (
				auto instruction_data = based_pointer<BinCmd64>(ollydbg_handle, 0x39B070 - 2);
				instruction_data->name;
				++instruction_data
			) {
				__builtin_dump_struct(instruction_data, fprintf, output_file);
			}
			FreeLibrary(ollydbg_handle);
		}
		fclose(output_file);
	}
}


// size: 0x34
struct BinCmd32New {
	PTR32Z<wchar_t> name; // 0x0
	union {
		uint32_t cmdtype; // 0x4
		PTR32Z<> cmdtype_hex;
		struct {
			CmdType type							: 5; // 0x0000001F
			uint32_t								: 7;
			PrefixRequirements prefix_requirements	: 4; // 0x0000F000
			uint32_t								: 4;
			FlagUpdates flags_updated				: 3; // 0x00700000
			X87Updates x87_updates					: 2; // 0x01800000
			uint32_t								: 5;
			UnusualFlags							: 2; // 0xC0000000
		};
		struct {
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool size01								: 1; // 0x00000020
			bool postbyte							: 1; // 0x00000040
			bool longform							: 1; // 0x00000080
			bool requires_16_bit_data				: 1; // 0x00000100
			bool requires_32_bit_data				: 1; // 0x00000200
			bool requires_16_bit_address			: 1; // 0x00000400
			bool requires_32_bit_address			: 1; // 0x00000800
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool lockable							: 1; // 0x00010000
			bool branch_hint						: 1; // 0x00020000
			bool mod_must_be_memory					: 1; // 0x00040000
			bool mod_must_be_register				: 1; // 0x00080000
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool changes_stack_pointer				: 1; // 0x02000000
			bool nonstandard_hla					: 1; // 0x04000000
			bool contains_wildcard					: 1; // 0x08000000
			bool is_conditional						: 1; // 0x10000000
			bool uses_carry_flag					: 1; // 0x20000000
			bool									: 1;
			bool									: 1;
		};
		struct {
			uint32_t								: 8;
			uint32_t data_address_size_mask			: 4; // 0x00000F00
		};
		struct {
			uint32_t								: 8;
			uint32_t required_data_size				: 2; // 0x00000300
			uint32_t required_address_size			: 2; // 0x00000C00
		};
	};
	union {
		uint32_t exttype; // 0x8
		PTR32Z<> exttype_hex;
		struct {
			ZeroFlagDecode zero_flag_decode			: 2; // 0x00000003
			CarryFlagDecode carry_flag_decode		: 2; // 0x0000000C
			uint32_t								: 5;
			VexLength vex_length					: 2; // 0x00000600
			uint32_t								: 1;
			VexWideRequirement vex_w_requirement	: 2; // 0x00003000
			uint32_t								: 2;
			VexOpcodePage vex_opcode_page			: 3; // 0x00070000
			uint32_t								: 5;
			CmdTypeEx32 cmdtype_ex					: 8; // 0xFF000000
		};
		struct {
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool near_return						: 1; // 0x00000010
			bool __unknown_flag_A					: 1; // 0x00000020
			bool __unknown_flag_B					: 1; // 0x00000040
			bool __unknown_flag_C					: 1; // 0x00000080
			bool vex								: 1; // 0x00000100
			bool									: 1;
			bool									: 1;
			bool no_vreg							: 1; // 0x00000800
			bool									: 1;
			bool									: 1;
			bool __unknown_flag_D					: 1; // 0x00004000
			bool __unknown_flag_E					: 1; // 0x00008000
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool __unknown_flag_F					: 1; // 0x00080000
			bool __unknown_flag_G					: 1; // 0x00100000
			bool __unknown_flag_H					: 1; // 0x00200000
			bool __unknown_flag_I					: 1; // 0x00400000
			bool wonky_trap							: 1; // 0x00800000
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
			bool									: 1;
		};
	};
	union {
		uint32_t old_main_opcode_length; // 0xC
		struct {
			uint32_t main_opcode_length : 4;
		};
	};
	union {
		uint32_t opcode_mask; // 0x10
		uint8_t opcode_mask_array[4];
		PTR32Z<> opcode_mask_hex;
		struct {
			uint8_t opcode_mask_byte_1;
			uint8_t opcode_mask_byte_2;
			uint8_t opcode_mask_byte_3;
			uint8_t opcode_mask_byte_4;
		};
	};
	union {
		uint32_t opcode_bytes; // 0x14
		uint8_t opcode_bytes_array[4];
		PTR32Z<> opcode_bytes_hex;
		struct {
			uint8_t opcode_byte_1;
			uint8_t opcode_byte_2;
			uint8_t opcode_byte_3;
			uint8_t opcode_byte_4;
		};
	};
	union {
		uint32_t old_post_opcode_bytes; // 0x18
		struct {
			uint8_t post_opcode_byte; // 0x18
		};
	};
	union {
		BinCmdArg arg_array[4]; // 0x1C
		struct {
			BinCmdArg arg_1; // 0x1C
			BinCmdArg arg_2; // 0x20
			BinCmdArg arg_3; // 0x24
			BinCmdArg arg_4; // 0x28
		};
	};
	PTR32Z<> trace; // 0x2C
	PTR32Z<> emu; // 0x30
	// 0x34
};


struct RelocEntry {
	uint16_t offset : 12;
	uint16_t type : 4;
};
struct RelocBlock {
	PTR32Z<> page_rva; // 0x0
	uint32_t block_size; // 0x4
	RelocEntry entries[]; // 0x8
};

gnu_noinline void dump_ollydbg_relocs32(const wchar_t* path, const char* output_path) {
	if (FILE* output_file = fopen(output_path, "w")) {
		output_file = freopen(output_path, "a", output_file);
		if (HMODULE ollydbg_handle = LoadLibraryExW(path, NULL, LOAD_LIBRARY_AS_DATAFILE)) {
			void* ollydbg32_base = based_pointer<void>(ollydbg_handle, -1);
			RelocBlock* reloc_data = based_pointer<RelocBlock>(ollydbg32_base, 0x275E00);
			RelocBlock* end_of_reloc_data = based_pointer(reloc_data, 0x11400);
			size_t image_base = 0x400000;
			size_t block_size;
			size_t block_index = 0;
			void* bincmd_start = based_pointer((void*)image_base, 0xF793C);
			void* bincmd_end = based_pointer((void*)image_base, 0x109498);
			for (
				;
				reloc_data < end_of_reloc_data;
				(++block_index), (reloc_data = based_pointer(reloc_data, block_size))
			) {
				block_size = reloc_data->block_size;
				if (!block_size) break;

				void* page_start = based_pointer(reloc_data->page_rva, image_base);
				void* page_end = based_pointer(page_start, 4096);
				if (
					page_start < bincmd_end && bincmd_start <= page_end
				) {
					size_t entry_count = (block_size - sizeof(RelocBlock)) / sizeof(RelocEntry);
					size_t entries_in_range = 0;
					size_t entries_that_matter = entry_count;
					for (size_t i = 0; i < entry_count; ++i) {
						if (reloc_data->entries[i].type != IMAGE_REL_BASED_ABSOLUTE) {
							void* reloc_addr = based_pointer(page_start, reloc_data->entries[i].offset);
							entries_in_range += (uintptr_t(reloc_addr) - uintptr_t(bincmd_start)) < (uintptr_t(reloc_addr) - uintptr_t(bincmd_end));
						} else {
							--entries_that_matter;
						}
					}
					if (entries_in_range) {
						fprintf(output_file,
							"Block Index: %zu\n"
							"CmdName Entries: %zu/%zu (%zu)\n"
							, block_index
							, entries_in_range, entries_that_matter, entry_count
						);
						__builtin_dump_struct(reloc_data, fprintf, output_file);
						if (entries_in_range != entries_that_matter) {
							for (size_t i = 0; i < entry_count; ++i) {
								void* reloc_addr = based_pointer(page_start, reloc_data->entries[i].offset);
								if ((uintptr_t(reloc_addr) - uintptr_t(bincmd_start)) < (uintptr_t(reloc_addr) - uintptr_t(bincmd_end))) {
									fprintf(output_file, "Entry: %zu\n", i);
									__builtin_dump_struct(&reloc_data->entries[i], fprintf, output_file);
								}
							}
						}
					}
				}
			}
			FreeLibrary(ollydbg_handle);
		}
		fclose(output_file);
	}
}

struct Emu32 {
	uint32_t operand[4]; // 0x0
	uint32_t opsize; // 0x10
	PTR32Z<> memaddr; // 0x14
	uint32_t memsize; // 0x18
	uint32_t memdata; // 0x1C
	// 0x20
};
ValidateFieldOffset(0x0, Emu32, operand);
ValidateFieldOffset(0x10, Emu32, opsize);
ValidateFieldOffset(0x14, Emu32, memaddr);
ValidateFieldOffset(0x18, Emu32, memsize);
ValidateFieldOffset(0x1C, Emu32, memdata);
ValidateStructSize(0x20, Emu32);

struct MemField32 {
	PTR32Z<> addr; // 0x0
	uint32_t size; // 0x4
	uint8_t data[16]; // 0x8
	// 0x18
};
ValidateFieldOffset(0x0, MemField32, addr);
ValidateFieldOffset(0x4, MemField32, size);
ValidateFieldOffset(0x8, MemField32, data);
ValidateStructSize(0x18, MemField32);

union EFlagsLow {
	uint8_t raw; // 0x0
	struct {
		bool carry : 1;
		bool : 1;
		bool parity : 1;
		bool : 1;
		bool arithmetic : 1;
		bool : 1;
		bool zero : 1;
		bool sign : 1;
	};
};
union EFlagsHigh {
	uint8_t raw; // 0x0
	struct {
		bool trap : 1;
		bool interrupt : 1;
		bool direction : 1;
		bool overflow : 1;
	};
};
union EFlagsWord {
	uint16_t raw; // 0x0
	struct {
		EFlagsLow low; // 0x0
		EFlagsHigh high; // 0x1
	};
	struct {
		bool carry : 1;
		bool : 1;
		bool parity : 1;
		bool : 1;
		bool arithmetic : 1;
		bool : 1;
		bool zero : 1;
		bool sign : 1;
		bool trap : 1;
		bool interrupt : 1;
		bool direction : 1;
		bool overflow : 1;
	};
};
union EFlags {
	uint32_t raw; // 0x0
	EFlagsWord word; // 0x0
	struct {
		uint8_t low_byte; // 0x0
		uint8_t high_byte; // 0x1
	};
	struct {
		EFlagsLow low; // 0x0
		EFlagsHigh high; // 0x1
	};
	struct {
		bool carry : 1;
		bool : 1;
		bool parity : 1;
		bool : 1;
		bool arithmetic : 1;
		bool : 1;
		bool zero : 1;
		bool sign : 1;
		bool trap : 1;
		bool interrupt : 1;
		bool direction : 1;
		bool overflow : 1;
	};
};

struct RegData32 {
	union {
		uint32_t status; // 0x0
		struct {
			bool update_context : 1;
			bool user_modified_regs : 1;
			bool sse_valid : 1;
			bool sse_modified : 1;
			bool last_error_valid : 1;
			bool last_error_modified : 1;
			bool memory_valid : 1;
			bool update_drs : 1;
		};
	};
	uint32_t thread_id; // 0x4
	union {
		uint32_t eip; // 0x8
		uint16_t ip; // 0x8
	};
	uint32_t eax; // 0xC
	uint32_t ecx; // 0x10
	uint32_t edx; // 0x14
	uint32_t ebx; // 0x18
	uint32_t esp; // 0x1C
	uint32_t ebp; // 0x20
	uint32_t esi; // 0x24
	uint32_t edi; // 0x28
	EFlags flags; // 0x2C
	uint32_t es; // 0x30
	uint32_t cs; // 0x34
	uint32_t ss; // 0x38
	uint32_t ds; // 0x3C
	uint32_t fs; // 0x40
	uint32_t gs; // 0x44
	uint32_t es_base; // 0x48
	uint32_t cs_base; // 0x4C
	uint32_t ss_base; // 0x50
	uint32_t ds_base; // 0x54
	uint32_t fs_base; // 0x58
	uint32_t gs_base; // 0x5C
	uint32_t es_limit; // 0x60
	uint32_t cs_limit; // 0x64
	uint32_t ss_limit; // 0x68
	uint32_t ds_limit; // 0x6C
	uint32_t fs_limit; // 0x70
	uint32_t gs_limit; // 0x74
	bool es_big; // 0x78
	bool cs_big; // 0x79
	bool ss_big; // 0x7A
	bool ds_big; // 0x7B
	bool fs_big; // 0x7C
	bool gs_big; // 0x7D
	bool : 8; // 0x7E
	bool : 8; // 0x7F
	int32_t stack_top_index; // 0x80

};
ValidateFieldOffset(0x0, RegData32, status);
ValidateFieldOffset(0x4, RegData32, thread_id);
ValidateFieldOffset(0x8, RegData32, eip);
ValidateFieldOffset(0xC, RegData32, eax);
ValidateFieldOffset(0x10, RegData32, ecx);
ValidateFieldOffset(0x14, RegData32, edx);
ValidateFieldOffset(0x18, RegData32, ebx);
ValidateFieldOffset(0x1C, RegData32, esp);
ValidateFieldOffset(0x20, RegData32, ebp);
ValidateFieldOffset(0x24, RegData32, esi);
ValidateFieldOffset(0x28, RegData32, edi);
ValidateFieldOffset(0x2C, RegData32, flags);
ValidateFieldOffset(0x30, RegData32, es);
ValidateFieldOffset(0x34, RegData32, cs);
ValidateFieldOffset(0x38, RegData32, ss);
ValidateFieldOffset(0x3C, RegData32, ds);
ValidateFieldOffset(0x40, RegData32, fs);
ValidateFieldOffset(0x44, RegData32, gs);
ValidateFieldOffset(0x48, RegData32, es_base);
ValidateFieldOffset(0x4C, RegData32, cs_base);
ValidateFieldOffset(0x50, RegData32, ss_base);
ValidateFieldOffset(0x54, RegData32, ds_base);
ValidateFieldOffset(0x58, RegData32, fs_base);
ValidateFieldOffset(0x5C, RegData32, gs_base);
ValidateFieldOffset(0x60, RegData32, es_limit);
ValidateFieldOffset(0x64, RegData32, cs_limit);
ValidateFieldOffset(0x68, RegData32, ss_limit);
ValidateFieldOffset(0x6C, RegData32, ds_limit);
ValidateFieldOffset(0x70, RegData32, fs_limit);
ValidateFieldOffset(0x74, RegData32, gs_limit);
ValidateFieldOffset(0x78, RegData32, es_big);
ValidateFieldOffset(0x79, RegData32, cs_big);
ValidateFieldOffset(0x7A, RegData32, ss_big);
ValidateFieldOffset(0x7B, RegData32, ds_big);
ValidateFieldOffset(0x7C, RegData32, fs_big);
ValidateFieldOffset(0x7D, RegData32, gs_big);

#define USE_FAST_FULL_FLAGS 1
#define USE_FAST_SHORT_FLAGS 1
//#define USE_INTRINSIC_FLAGS 1

#ifndef  __x86_64__
dllexport void regcall EmulateNOP(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
}
dllexport void regcall EmulateMOV_LEA_MOVZX(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	emu->operand[0] = emu->operand[1];
}
dllexport void regcall EmulateMOVSXB(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	emu->operand[0] = int32_t(int8_t(emu->operand[1]));
}
dllexport void regcall EmulateMOVSXW(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	emu->operand[0] = int32_t(int16_t(emu->operand[1]));
}
dllexport void regcall EmulateLODS(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	emu->operand[0] = emu->operand[1];
	int32_t data_size = emu->opsize;
	regs->esi += expect_chance(!regs->flags.direction, true, 1.0) ? data_size : -data_size;
}
dllexport void regcall EmulateSTOS(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	emu->operand[0] = emu->operand[1];
	int32_t data_size = emu->opsize;
	regs->edi += expect_chance(!regs->flags.direction, true, 1.0) ? data_size : -data_size;
}
dllexport void regcall EmulateADD(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opB = emu->operand[1];
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] += opB;
			break;
		default:
			*(uint16_t*)&emu->operand[0] += opB;
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] += opB;
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(overflow_flag)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
/*
dllexport void regcall EmulateADC_old(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlags flags = regs->flags;
	uint32_t opB = emu->operand[1];
	bool carry;
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			__asm__(
				""
				: "=@ccc"(carry)
				: "0"(flags.carry)
			);
			emu->operand[0] += opB + carry;
			break;
		default:
			__asm__(
				""
				: "=@ccc"(carry)
				: "0"(flags.carry)
			);
			*(uint16_t*)&emu->operand[0] += opB + carry;
			break;
		case 1:
			__asm__(
				""
				: "=@ccc"(carry)
				: "0"(flags.carry)
			);
			*(uint8_t*)&emu->operand[0] += opB + carry;
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(flags)
	);
	flags.low_byte = low_flags;
	regs->flags = flags;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
*/
dllexport void regcall EmulateADC(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlagsLow low_flags = regs->flags.low;
	uint32_t opB = emu->operand[1];
	uint32_t opsize = emu->opsize;
	bool zero;
	__asm__ volatile (
		"cmp %[dword_op_size], %[opsize] \n"
		"rorb %[shift_count], %[flags]"
		: [flags]"+r"(low_flags.raw), asm_flags(z, zero)
		: [shift_count]"i"(1), [dword_op_size]"i"(4), asm_arg("c", opsize)
	);
	bool carry;
	if (expect_chance(zero, true, 1.0)) {
		__asm__(
			""
			: "=@ccc"(carry)
			: "0"(carry)
		);
		emu->operand[0] += opB + carry;
end_dword:
	} else {
		uint32_t opsizeB = opsize - 2;
		__asm__ goto (
			JPCXZ "%l[word_label]"
			:
			: asm_arg("c", opsizeB)
			:
			: word_label
		);
		__asm__(
			""
			: "=@ccc"(carry)
			: "0"(carry)
		);
		*(uint8_t*)&emu->operand[0] += opB + carry;
		goto end_dword;
word_label:
		__asm__(
			""
			: "=@ccc"(carry)
			: "0"(carry)
		);
		*(uint16_t*)&emu->operand[0] += opB + carry;
		goto end_dword;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t new_low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(new_low_flags), "=@cco"(overflow_flag)
	);
	regs->flags.low_byte = new_low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags] "=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateSUB(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opB = emu->operand[1];
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] -= opB;
			break;
		default:
			*(uint16_t*)&emu->operand[0] -= opB;
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] -= opB;
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(overflow_flag)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
/*
dllexport void regcall EmulateSBB_old(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlags flags = regs->flags;
	uint32_t opB = emu->operand[1] + flags.carry;
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] -= opB;
			break;
		default:
			*(uint16_t*)&emu->operand[0] -= opB;
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] -= opB;
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(flags.overflow)
	);
	flags.low_byte = low_flags;
	regs->flags = flags;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
*/
dllexport void regcall EmulateSBB(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlagsLow low_flags = regs->flags.low;
	uint32_t opB = emu->operand[1];
	uint32_t opsize = emu->opsize;
	bool zero;
	__asm__ volatile (
		"cmp %[dword_op_size], %[opsize] \n"
		"rorb %[shift_count], %[flags]"
		: [flags]"+r"(low_flags.raw), asm_flags(z, zero)
		: [shift_count]"i"(1), [dword_op_size]"i"(4), asm_arg("c", opsize)
	);
	bool carry;
	if (expect_chance(zero, true, 1.0)) {
		__asm__(
			""
			: "=@ccc"(carry)
			: "0"(carry)
		);
		emu->operand[0] -= opB + carry;
end_dword:
	} else {
		uint32_t opsizeB = opsize - 2;
		__asm__ goto (
			JPCXZ "%l[word_label]"
			:
			: asm_arg("c", opsizeB)
			:
			: word_label
		);
		__asm__(
			""
			: "=@ccc"(carry)
			: "0"(carry)
		);
		*(uint8_t*)&emu->operand[0] -= opB + carry;
		goto end_dword;
word_label:
		__asm__(
			""
			: "=@ccc"(carry)
			: "0"(carry)
		);
		*(uint16_t*)&emu->operand[0] -= opB + carry;
		goto end_dword;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t new_low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(new_low_flags), "=@cco"(overflow_flag)
	);
	regs->flags.low_byte = new_low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags] "=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateAND(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opB = emu->operand[1];
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] &= opB;
			break;
		default:
			*(uint16_t*)&emu->operand[0] &= opB;
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] &= opB;
			break;
	}
#if USE_FAST_SHORT_FLAGS
	register uint8_t low_flags asm("ah");
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = false;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateOR(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opB = emu->operand[1];
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] |= opB;
			break;
		default:
			*(uint16_t*)&emu->operand[0] |= opB;
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] |= opB;
			break;
	}
#if USE_FAST_SHORT_FLAGS
	register uint8_t low_flags asm("ah");
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = false;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateXOR(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opB = emu->operand[1];
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] ^= opB;
			break;
		default:
			*(uint16_t*)&emu->operand[0] ^= opB;
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] ^= opB;
			break;
	}
#if USE_FAST_SHORT_FLAGS
	register uint8_t low_flags asm("ah");
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = false;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateTEST(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opA = emu->operand[0];
	uint32_t opB = emu->operand[1];
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			__asm__ volatile(
				"testl %1,%0"
				: "+r"(opA), "+r"(opB)
				: 
				: "cc"
			);
			break;
		default:
			__asm__ volatile(
				"testw %w1,%w0"
				: "+r"(opA), "+r"(opB)
				:
				: "cc"
			);
			break;
		case 1:
			__asm__ volatile(
				"testb %b1,%b0"
				: "+r"(opA), "+r"(opB)
				:
				: "cc"
			);
			break;
	}
#if USE_FAST_SHORT_FLAGS
	register uint8_t low_flags asm("ah");
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = false;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateCMP(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opA = emu->operand[0];
	uint32_t opB = emu->operand[1];
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			__asm__ volatile(
				"cmpl %1,%0"
				: "+r"(opA), "+r"(opB)
				: 
				: "cc"
			);
			break;
		default:
			__asm__ volatile(
				"cmpw %w1,%w0"
				: "+r"(opA), "+r"(opB)
				:
				: "cc"
			);
			break;
		case 1:
			__asm__ volatile(
				"cmpb %b1,%b0"
				: "+r"(opA), "+r"(opB)
				:
				: "cc"
			);
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(overflow_flag)
		);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags]"=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateINC(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			++emu->operand[0];
			break;
		default:
			++*(uint16_t*)&emu->operand[0];
			break;
		case 1:
			++*(uint8_t*)&emu->operand[0];
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(overflow_flag)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags] "=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateDEC(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			--emu->operand[0];
			break;
		default:
			--*(uint16_t*)&emu->operand[0];
			break;
		case 1:
			--*(uint8_t*)&emu->operand[0];
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(overflow_flag)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags] "=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateNEG(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] = -emu->operand[0];
			break;
		default:
			*(uint16_t*)&emu->operand[0] = -*(uint16_t*)&emu->operand[0];
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] = -*(uint8_t*)&emu->operand[0];
			break;
	}
#if USE_FAST_FULL_FLAGS
	register uint8_t low_flags asm("ah");
	bool overflow_flag;
	__asm__ volatile (
		"lahf"
		: "=r"(low_flags), "=@cco"(overflow_flag)
	);
	regs->flags.low_byte = low_flags;
	regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
	regs->flags.raw = __readeflags();
#else
	__asm__ volatile (
		"pushfd \n"
		"pop %[flags]"
		: [flags] "=m"(regs->flags)
	);
#endif
}
dllexport void regcall EmulateNOT(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	switch (expect_chance(emu->opsize, 4, 0.75)) {
		case 4:
			emu->operand[0] = ~emu->operand[0];
			break;
		default:
			*(uint16_t*)&emu->operand[0] = ~*(uint16_t*)&emu->operand[0];
			break;
		case 1:
			*(uint8_t*)&emu->operand[0] = ~*(uint8_t*)&emu->operand[0];
			break;
	}
}
dllexport void regcall EmulateJMP(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opA = emu->operand[0];
	if (expect_chance(emu->opsize == 2, true, 0.0)) {
		opA = (uint16_t)opA;
	}
	regs->eip = opA;
}
dllexport void regcall EmulateJZ_JE(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (regs->flags.zero) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJNZ_JNE(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (!regs->flags.zero) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJA(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (!regs->flags.carry && !regs->flags.zero) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJNC_JAE(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (!regs->flags.carry) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJC_JB(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (regs->flags.carry) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJBE(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (regs->flags.carry || regs->flags.zero) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJG(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlagsWord flags = regs->flags.word;
	if (!flags.zero && flags.sign == flags.overflow) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJGE(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlagsWord flags = regs->flags.word;
	if (flags.sign == flags.overflow) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJL(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlagsWord flags = regs->flags.word;
	if (flags.sign != flags.overflow) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJLE(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	EFlagsWord flags = regs->flags.word;
	if (flags.zero || flags.sign != flags.overflow) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJO(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (regs->flags.overflow) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJNO(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (!regs->flags.overflow) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJPO(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (!regs->flags.parity) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJPE(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (regs->flags.parity) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJS(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (regs->flags.sign) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
dllexport void regcall EmulateJNS(uint32_t a, uint32_t b, uint32_t c, Emu32 *restrict emu, RegData32 *restrict regs) {
	if (!regs->flags.sign) {
		clang_noinline gnu_attr(musttail) return EmulateJMP(a, b, c, emu, regs);
	}
}
/*
dllexport void regcall EmulateSHL_old(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint8_t shift_count = emu->operand[1];
	if (expect_chance(shift_count != 0, true, 1.0)) {
		EFlagsWord flags = regs->flags.word;
		__asm__(
			"shrb %[initial_shift_count], %%ah \n"
			"rorb $1, %%ah \n"
			"movb %%al, %%ah \n"
			"sahf"
			:
			: "a"(flags), [initial_shift_count]"i"(3)
			: clobber_list("ecx")
		);
		switch (expect_chance(emu->opsize, 4, 0.75)) {
			case 4:
				emu->operand[0] <<= shift_count;
				break;
			default:
				*(uint16_t*)&emu->operand[0] <<= shift_count;
				break;
			case 1:
				*(uint8_t*)&emu->operand[0] <<= shift_count;
				break;
		}
		
#if USE_FAST_FULL_FLAGS
		register uint8_t low_flags asm("ah");
		bool overflow_flag;
		__asm__ volatile (
			"lahf"
			: "=r"(low_flags), "=@cco"(overflow_flag)
			);
		regs->flags.low_byte = low_flags;
		regs->flags.overflow = overflow_flag;
#elif USE_INTRINSIC_FLAGS
		regs->flags.raw = __readeflags();
#else
		__asm__ volatile (
			"pushfd \n"
			"pop %[flags]"
			: [flags] "=m"(regs->flags)
			);
#endif
	}
}
*/
dllexport void regcall EmulateSHL(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	union ShiftCrap {
		uint32_t raw;
		struct {
			uint8_t shift_count;
			uint8_t flags_temp;
		};
	};
	register ShiftCrap temp asm("ecx");
	temp.raw = (uint8_t)emu->operand[1];
	temp.shift_count &= 0b11111;
	if (expect_chance(temp.shift_count != 0, true, 1.0)) {
		EFlagsWord flags = regs->flags.word;
		std::swap(flags.low.raw, flags.high.raw);
		__asm__ volatile (
			"mov %%al, %%ch \n"
			"andb $8, %%al"
			: "+a"(flags.raw), "+c"(temp)
		);
		switch (expect_chance(emu->opsize, 4, 0.75)) {
			case 4:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] <<= temp.shift_count;
				break;
			default:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				*(uint16_t*)&emu->operand[0] <<= temp.shift_count;
				break;
			case 1:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				*(uint8_t*)&emu->operand[0] <<= temp.shift_count;
				break;
		}
		
#if USE_FAST_FULL_FLAGS
		register uint8_t low_flags asm("ah");
		register uint8_t old_flags_temp asm("ch") = temp.flags_temp;
		bool overflow_flag;
		__asm__ volatile (
			"lahf"
			: "+c"(old_flags_temp), "=r"(low_flags), "=@cco"(overflow_flag)
		);
		regs->flags.low_byte = low_flags;
		regs->flags.high_byte = overflow_flag << 3 | (old_flags_temp & 0xF7);
#elif USE_INTRINSIC_FLAGS
		regs->flags.raw = __readeflags();
#else
		__asm__ volatile (
			"pushfd \n"
			"pop %[flags]"
			: [flags] "=m"(regs->flags)
		);
#endif
	}
}
dllexport void regcall EmulateSHR(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	union ShiftCrap {
		uint32_t raw;
		struct {
			uint8_t shift_count;
			uint8_t flags_temp;
		};
	};
	register ShiftCrap temp asm("ecx");
	temp.raw = (uint8_t)emu->operand[1];
	temp.shift_count &= 0b11111;
	if (expect_chance(temp.shift_count != 0, true, 1.0)) {
		EFlagsWord flags = regs->flags.word;
		std::swap(flags.low.raw, flags.high.raw);
		__asm__ volatile (
			"mov %%al, %%ch \n"
			"andb $8, %%al"
			: "+a"(flags.raw), "+c"(temp)
		);
		switch (expect_chance(emu->opsize, 4, 0.75)) {
			case 4:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] >>= temp.shift_count;
				break;
			default:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				*(uint16_t*)&emu->operand[0] >>= temp.shift_count;
				break;
			case 1:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				*(uint8_t*)&emu->operand[0] >>= temp.shift_count;
				break;
		}
		
#if USE_FAST_FULL_FLAGS
		register uint8_t low_flags asm("ah");
		register uint8_t old_flags_temp asm("ch") = temp.flags_temp;
		bool overflow_flag;
		__asm__ volatile (
			"lahf"
			: "+c"(old_flags_temp), "=r"(low_flags), "=@cco"(overflow_flag)
		);
		regs->flags.low_byte = low_flags;
		regs->flags.high_byte = overflow_flag << 3 | (old_flags_temp & 0xF7);
#elif USE_INTRINSIC_FLAGS
		regs->flags.raw = __readeflags();
#else
		__asm__ volatile (
			"pushfd \n"
			"pop %[flags]"
			: [flags] "=m"(regs->flags)
		);
#endif
	}
}
dllexport void regcall EmulateSAR(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	union ShiftCrap {
		uint32_t raw;
		struct {
			uint8_t shift_count;
			uint8_t flags_temp;
		};
	};
	register ShiftCrap temp asm("ecx");
	temp.raw = (uint8_t)emu->operand[1];
	temp.shift_count &= 0b11111;
	if (expect_chance(temp.shift_count != 0, true, 1.0)) {
		EFlagsWord flags = regs->flags.word;
		std::swap(flags.low.raw, flags.high.raw);
		__asm__ volatile (
			"mov %%al, %%ch \n"
			"andb $8, %%al"
			: "+a"(flags.raw), "+c"(temp)
		);
		switch (expect_chance(emu->opsize, 4, 0.75)) {
			case 4:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				*(int32_t*)&emu->operand[0] >>= temp.shift_count;
				break;
			default:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				*(int16_t*)&emu->operand[0] >>= temp.shift_count;
				break;
			case 1:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				*(int8_t*)&emu->operand[0] >>= temp.shift_count;
				break;
		}
		
#if USE_FAST_FULL_FLAGS
		register uint8_t low_flags asm("ah");
		register uint8_t old_flags_temp asm("ch") = temp.flags_temp;
		bool overflow_flag;
		__asm__ volatile (
			"lahf"
			: "+c"(old_flags_temp), "=r"(low_flags), "=@cco"(overflow_flag)
		);
		regs->flags.low_byte = low_flags;
		regs->flags.high_byte = overflow_flag << 3 | (old_flags_temp & 0xF7);
#elif USE_INTRINSIC_FLAGS
		regs->flags.raw = __readeflags();
#else
		__asm__ volatile (
			"pushfd \n"
			"pop %[flags]"
			: [flags] "=m"(regs->flags)
		);
#endif
	}
}
dllexport void regcall EmulateROR(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	union ShiftCrap {
		uint32_t raw;
		struct {
			uint8_t shift_count;
			uint8_t flags_temp;
		};
	};
	register ShiftCrap temp asm("ecx");
	temp.raw = (uint8_t)emu->operand[1];
	temp.shift_count &= 0b11111;
	if (expect_chance(temp.shift_count != 0, true, 1.0)) {
		EFlagsWord flags = regs->flags.word;
		std::swap(flags.low.raw, flags.high.raw);
		__asm__ volatile (
			"mov %%al, %%ch \n"
			"andb $8, %%al"
			: "+a"(flags.raw), "+c"(temp)
		);
		switch (expect_chance(emu->opsize, 4, 0.75)) {
			case 4:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] = __builtin_rotateright32(emu->operand[0], temp.shift_count);
				break;
			default:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] = __builtin_rotateright16(emu->operand[0], temp.shift_count);
				break;
			case 1:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] = __builtin_rotateright8(emu->operand[0], temp.shift_count);
				break;
		}
		
#if USE_FAST_FULL_FLAGS
		register uint8_t low_flags asm("ah");
		register uint8_t old_flags_temp asm("ch") = temp.flags_temp;
		bool overflow_flag;
		__asm__ volatile (
			"lahf"
			: "+c"(old_flags_temp), "=r"(low_flags), "=@cco"(overflow_flag)
		);
		regs->flags.low_byte = low_flags;
		regs->flags.high_byte = overflow_flag << 3 | (old_flags_temp & 0xF7);
#elif USE_INTRINSIC_FLAGS
		regs->flags.raw = __readeflags();
#else
		__asm__ volatile (
			"pushfd \n"
			"pop %[flags]"
			: [flags] "=m"(regs->flags)
		);
#endif
	}
}
dllexport void regcall EmulateROL(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	union ShiftCrap {
		uint32_t raw;
		struct {
			uint8_t shift_count;
			uint8_t flags_temp;
		};
	};
	register ShiftCrap temp asm("ecx");
	temp.raw = (uint8_t)emu->operand[1];
	temp.shift_count &= 0b11111;
	if (expect_chance(temp.shift_count != 0, true, 1.0)) {
		EFlagsWord flags = regs->flags.word;
		std::swap(flags.low.raw, flags.high.raw);
		__asm__ volatile (
			"mov %%al, %%ch \n"
			"andb $8, %%al"
			: "+a"(flags.raw), "+c"(temp)
		);
		switch (expect_chance(emu->opsize, 4, 0.75)) {
			case 4:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] = __builtin_rotateleft32(emu->operand[0], temp.shift_count);
				break;
			default:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] = __builtin_rotateleft16(emu->operand[0], temp.shift_count);
				break;
			case 1:
				__asm__ volatile(
					"cmpb %[overflow_const], %%al \n"
					"sahf"
					: 
					: "a"(flags.raw), [overflow_const]"i"(-120)
				);
				emu->operand[0] = __builtin_rotateleft8(emu->operand[0], temp.shift_count);
				break;
		}
		
#if USE_FAST_FULL_FLAGS
		register uint8_t low_flags asm("ah");
		register uint8_t old_flags_temp asm("ch") = temp.flags_temp;
		bool overflow_flag;
		__asm__ volatile (
			"lahf"
			: "+c"(old_flags_temp), "=r"(low_flags), "=@cco"(overflow_flag)
		);
		regs->flags.low_byte = low_flags;
		regs->flags.high_byte = overflow_flag << 3 | (old_flags_temp & 0xF7);
#elif USE_INTRINSIC_FLAGS
		regs->flags.raw = __readeflags();
#else
		__asm__ volatile (
			"pushfd \n"
			"pop %[flags]"
			: [flags] "=m"(regs->flags)
		);
#endif
	}
}
dllexport void regcall EmulateRCR(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	union ShiftCrap {
		uint32_t raw;
		struct {
			uint8_t shift_count;
			uint8_t flags_temp;
		};
	};
	register ShiftCrap temp asm("ecx");
	temp.raw = (uint8_t)emu->operand[1];
	temp.shift_count &= 0b11111;
	if (expect_chance(temp.shift_count != 0, true, 1.0)) {
		EFlagsWord flags = regs->flags.word;
		std::swap(flags.low.raw, flags.high.raw);
		__asm__ volatile (
			"mov %%al, %%ch \n"
			"andb $8, %%al"
			: "+a"(flags.raw), "+c"(temp)
		);
		uint32_t op = emu->operand[0];
		if (expect_chance(temp.shift_count == 1, true, 1.0)) {
			switch (expect_chance(emu->opsize, 4, 0.75)) {
				case 4:
					__asm__ volatile(
						"cmpb %[overflow_const], %%al \n"
						"sahf \n"
						"rcrl $1, %k[op]"
						: asm_arg("+b", op)
						: "a"(flags.raw), [overflow_const]"i"(-120)
					);
					break;
				default:
					__asm__ volatile(
						"cmpb %[overflow_const], %%al \n"
						"sahf \n"
						"rcrw $1, %w[op]"
						: asm_arg("+b", op)
						: "a"(flags.raw), [overflow_const]"i"(-120)
					);
					break;
				case 1:
					__asm__ volatile(
						"cmpb %[overflow_const], %%al \n"
						"sahf \n"
						"rcrb $1, %b[op]"
						: asm_arg("+b", op)
						: "a"(flags.raw), [overflow_const]"i"(-120)
					);
					break;
			}
		}
		else {
			switch (expect_chance(emu->opsize, 4, 0.75)) {
				case 4:
					__asm__ volatile(
						"cmpb %[overflow_const], %%al \n"
						"sahf \n"
						"rcrl %%cl, %k[op]"
						: asm_arg("+b", op)
						: "a"(flags.raw), [overflow_const]"i"(-120)
					);
					break;
				default:
					__asm__ volatile(
						"cmpb %[overflow_const], %%al \n"
						"sahf \n"
						"rcrw %%cl, %w[op]"
						: asm_arg("+b", op)
						: "a"(flags.raw), [overflow_const]"i"(-120)
					);
					break;
				case 1:
					__asm__ volatile(
						"cmpb %[overflow_const], %%al \n"
						"sahf \n"
						"rcrb %%cl, %b[op]"
						: asm_arg("+b", op)
						: "a"(flags.raw), [overflow_const]"i"(-120)
					);
					break;
			}
		}
		emu->operand[0] = op;
#if USE_FAST_FULL_FLAGS
		register uint8_t low_flags asm("ah");
		register uint8_t old_flags_temp asm("ch") = temp.flags_temp;
		bool overflow_flag;
		__asm__ volatile (
			"lahf"
			: "+c"(old_flags_temp), "=r"(low_flags), "=@cco"(overflow_flag)
		);
		regs->flags.low_byte = low_flags;
		regs->flags.high_byte = overflow_flag << 3 | (old_flags_temp & 0xF7);
#elif USE_INTRINSIC_FLAGS
		regs->flags.raw = __readeflags();
#else
		__asm__ volatile (
			"pushfd \n"
			"pop %[flags]"
			: [flags] "=m"(regs->flags)
		);
#endif
	}
}
dllexport void regcall EmulatePUSH(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opA = emu->operand[0];
	uint32_t opsize = emu->memsize = emu->opsize;
	if (expect_chance(opsize != 2, true, 1.0)) {
		emu->memdata = opA;
	} else {
		*(uint16_t*)&emu->memdata = opA;
	}
	emu->memaddr = (void*)(regs->esp -= opsize);
}
dllexport void regcall EmulatePOP(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t opB = emu->operand[1];
	uint32_t opsize = emu->memsize = emu->opsize;
	if (expect_chance(opsize != 2, true, 1.0)) {
		emu->operand[0] = opB;
	} else {
		*(uint16_t*)&emu->operand[0] = opB;
	}
	regs->esp += opsize;
}
dllexport void regcall EmulateCALL(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t eip = regs->eip;
	uint32_t opA = emu->operand[0];
	uint32_t opsize = emu->memsize = emu->opsize;
	if (expect_chance(opsize != 2, true, 1.0)) {
		emu->memdata = eip;
		regs->eip = opA;
	} else {
		*(uint16_t*)&emu->memdata = eip;
		regs->eip = (uint16_t)opA;
	}
	emu->memaddr = (void*)(regs->esp -= opsize);
}
dllexport void regcall EmulateRET(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t addr = emu->operand[0];
	uint32_t opsize = emu->memsize = emu->opsize;
	if (expect_chance(opsize != 2, true, 1.0)) {
		regs->eip = addr;
	} else {
		regs->eip = (uint16_t)addr;
	}
	regs->esp += opsize;
}
dllexport void regcall EmulateRETImm(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	uint32_t addr = emu->operand[0];
	uint32_t opsize = emu->memsize = emu->opsize;
	if (expect_chance(opsize != 2, true, 1.0)) {
		regs->eip = addr;
	} else {
		regs->eip = (uint16_t)addr;
	}
	regs->esp += opsize + emu->operand[1];
}
dllexport void regcall EmulateCLD(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	regs->flags.direction = false;
}
dllexport void regcall EmulateCLC(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	regs->flags.carry = false;
}
dllexport void regcall EmulateCMC(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	regs->flags.carry = !regs->flags.carry;
}
dllexport void regcall EmulateSTC(uint32_t, uint32_t, uint32_t, Emu32 *restrict emu, RegData32 *restrict regs) {
	regs->flags.carry = true;
}
#endif