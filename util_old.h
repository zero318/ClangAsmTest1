#pragma once

#pragma clang diagnostic ignored "-Wc++11-narrowing"
#pragma clang diagnostic ignored "-Wc++17-extensions"

typedef double max_align_t;

#include <stdint.h>
#include <type_traits>

/*
#define USE_ALL_FEATURES
#define USE_AMD_FEATURES

#ifndef __SSE__
#define __SSE__ 1
#endif
#ifndef __SSE2__
#define __SSE2__ 1
#endif
#ifndef __SSSE3__
#define __SSSE3__ 1
#endif
#ifndef __SSE3__
#define __SSE3__ 1
#endif
#ifndef __SSE4_1__
#define __SSE4_1__ 1
#endif

#ifdef USE_ALL_FEATURES
#ifndef __SSE4_2__
#define __SSE4_2__ 1
#endif
#ifndef __AVX__
#define __AVX__ 1
#endif
#ifndef __BMI__
#define __BMI__ 1
#endif
#ifndef __FMA__
#define __FMA__ 1
#endif
#ifndef __LZCNT__
#define __LZCNT__ 1
#endif
#ifndef __POPCNT__
#define __POPCNT__ 1
#endif
#endif

#ifdef USE_AMD_FEATURES
#ifndef __FMA4__
#define __FMA4__ 1
#endif
#ifndef __SSE4A__
#define __SSE4A__ 1
#endif
#ifndef __XOP__
#define __XOP__ 1
#endif
#ifndef __TBM__
#define __TBM__ 1
#endif
#endif

#ifndef __building_module
#define __building_module(e) 1
#endif
#include <intrin.h>

#define _MACRO_CAT(arg1, arg2) arg1 ## arg2
#define MACRO_CAT(arg1, arg2) _MACRO_CAT(arg1, arg2)
#define _MACRO_STR(arg) #arg
#define MACRO_STR(arg) _MACRO_STR(arg)
#define MACRO_EVAL(...) __VA_ARGS__

#define CounterCat(name) \
MACRO_CAT(name, __COUNTER__)

*/

enum RegName_t {
	None,
	EAX,
	ECX,
	EDX,
	EBX,
	EBP,
	ESP,
	ESI,
	EDI,
	AX,
	CX,
	DX,
	BX,
	BP,
	SP,
	SI,
	DI,
	AL,
	AH,
	CL,
	CH,
	DL,
	DH,
	BL,
	BH,
	SS,
	CS,
	DS,
	ES,
	FS,
	GS,
	ST0,
	ST1,
	ST2,
	ST3,
	ST4,
	ST5,
	ST6,
	ST7,
	MM0,
	MM1,
	MM2,
	MM3,
	MM4,
	MM5,
	MM6,
	MM7,
	XMM0,
	XMM1,
	XMM2,
	XMM3,
	XMM4,
	XMM5,
	XMM6,
	XMM7,
	YMM0,
	YMM1,
	YMM2,
	YMM3,
	YMM4,
	YMM5,
	YMM6,
	YMM7
};

#define AsmCall(val) \
asm( \
	".byte 0xE8 \n" \
	"Laddr: \n" \
	".long Laddr - 4 - " MACRO_STR(val) \
)

#define __packed

//#include "zero/ExternalGlobal.h"

//using namespace ExternalGlobalVars;