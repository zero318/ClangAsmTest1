#if ZERO_TRAP_AND_EMULATE

#include <stdint.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>

#include <float.h>

#include "util.h"
//#include "zero/BoundingBox.h"
#undef _WIN64
//#undef WIN32_LEAN_AND_MEAN
#include "Windows.h"

LONG WINAPI exception_filter(LPEXCEPTION_POINTERS lpEI);

#define ENABLE_CUSTOM_INSTRUCTIONS 1
#define ENABLE_OFFBRAND_INSTRUCTIONS 1
#define ENABLE_AMD_INSTRUCTIONS 1
//#define ENABLE_ANCIENT_BS1 1
#define ENABLE_ANCIENT_BS2 1
//#define ENABLE_ANCIENT_BS3 1
//#define ENABLE_L10M 1
//#define ENABLE_K10M 1
//#define ENABLE_MVEX 1
//#define ENABLE_EVEX 1

static bool waiting_on_trap_flag = []() {
	AddVectoredExceptionHandler(0, exception_filter);
	return false;
}();

union ModRM {
	uint8_t raw;
	struct {
		uint8_t m : 3;
		uint8_t r : 3;
		uint8_t mod : 2;
	};
};

LONG WINAPI exception_filter(LPEXCEPTION_POINTERS lpEI) {
	/*
	if (IsDebuggerPresent()) {
		return EXCEPTION_CONTINUE_SEARCH;
	}
	*/
	LPEXCEPTION_RECORD lpER = lpEI->ExceptionRecord;
	switch (lpER->ExceptionCode) {
		default:
			return EXCEPTION_CONTINUE_SEARCH;
		case STATUS_ILLEGAL_INSTRUCTION:
	}

	return EXCEPTION_CONTINUE_EXECUTION;
	PCONTEXT context = lpEI->ContextRecord;
	uint8_t* rip = (uint8_t*)context->Rip;
	struct {
		bool data_size : 1;
		bool address_size : 1;
		bool lock : 1;
		bool repne : 1;
		bool rep : 1;
		bool rex : 1;
	} prefixes{};
	while (true) {
		switch (uint8_t first_byte = *rip++) {
			case 0x66: // data_size
				prefixes.data_size = true;
				continue;
			case 0x67: // address_size
				prefixes.address_size = true;
				continue;
			case 0xF0: // lock
				prefixes.lock = true;
				continue;
			case 0xF2: // repne
				prefixes.repne = true;
				continue;
			case 0xF3: // rep
				prefixes.rep = true;
				continue;

			case 0x06: // PUSH ES
			case 0x07: // POP ES
			case 0x0E: // PUSH CS
			case 0x0F:
				switch (uint8_t second_byte = *rip++) {
#if ENABLE_ANCIENT_BS1
					case 0x04: // SAVEALL
#endif
					case 0x0A: // CL1INVMB
					case 0x0D: // PREFETCH, PREFETCHW, PREFETCHWT1
					case 0x0E: // FEMMS
					case 0x0F: // 3DNow!
					case 0x34: // SYSENTER
					case 0x35: // SYSEXIT
					case 0x38:
						switch (uint8_t third_byte = *rip++) {

						}
					case 0x3A:
						switch (uint8_t third_byte = *rip++) {

						}
#if ENABLE_AMD_INSTRUCTIONS
					case 0x78: // EXTRQ(datasize), INSERTQ(repne)
					case 0x79: // EXTRQ(datasize), INSERTQ(repne)
#endif
#if ENABLE_ANCIENT_BS2
					case 0xA6: // XBTS
					case 0xA7: // IBTS
#endif
#if ENABLE_ANCIENT_BS3
					case 0xB8: // JMPE
#endif
				}
			case 0x16: // PUSH SS
			case 0x17: // POP SS
			case 0x1E: // PUSH DS
			case 0x1F: // POP DS
			case 0x27: // DAA
#if ENABLE_AMD_INSTRUCTIONS
			case 0x2B: // MOVNTSS(rep), MOVNTSD(repne)

#endif
			case 0x2F: // DAS
			case 0x37: // AAA
			case 0x3F: // AAS
			case 0x60: // PUSHA
			case 0x61: // POPA
#if ENABLE_L10M || ENABLE_MVEX || ENABLE_EVEX
			case 0x62: // L10M, MVEX, EVEX
#else
			case 0x62: // BOUND
#endif
			case 0x82: // Group 2
			case 0x9A: // CALL FAR imm
			case 0xCE: // INTO
			case 0xD4: // AAM
			case 0xD5: // AAD
#if ENABLE_L10M
			case 0xD6: // L10M
#else
			case 0xD6: // SALC
#endif
			case 0xEA: // JMP FAR imm

			case 0xD9:
				switch (uint8_t second_byte = *rip++) {
#if ENABLE_CUSTOM_INSTRUCTIONS
					case 0x00 ... 0xBF:
						ModRM as_mod_rm{ second_byte };
						switch (as_mod_rm.r) {
							case 1: // FCOMI m32
						}
#endif
#if ENABLE_OFFBRAND_INSTRUCTIONS
					case 0xE6: // FTSTP
#endif
				}
			case 0xDA:
				switch (uint8_t second_byte = *rip++) {
#if ENABLE_CUSTOM_INSTRUCTIONS
					case 0xE0 ... 0xE7: // FMOV ST(0), ST(i)
#endif
				}
			case 0xDB:
				switch (uint8_t second_byte = *rip++) {
#if ENABLE_CUSTOM_INSTRUCTIONS
					case 0x00 ... 0xBF:
						ModRM as_mod_rm{ second_byte };
						switch (as_mod_rm.r) {
							case 4: // FCOMI m64
							case 6: // FST m80
						}
#endif
#if ENABLE_OFFBRAND_INSTRUCTIONS
					case 0xFC: // FRINT2
#endif
				}
			case 0xDD:
				switch (uint8_t second_byte = *rip++) {
#if ENABLE_CUSTOM_INSTRUCTIONS
					case 0x00 ... 0xBF:
						ModRM as_mod_rm{ second_byte };
						switch (as_mod_rm.r) {
							case 5: // FIST i64
						}
#endif
#if ENABLE_OFFBRAND_INSTRUCTIONS
					case 0xFC: // FRICHOP
#endif
				}
			case 0xDF:
				switch (uint8_t second_byte = *rip++) {
#if ENABLE_OFFBRAND_INSTRUCTIONS
					case 0xFC: // FRINEAR
#endif
				}
		}
	}
}
#endif