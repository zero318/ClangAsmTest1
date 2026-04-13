#include "zero/util.h"
#include "zero/custom_intrin.h"

struct x87EnvTest {
	FCW fcw;
	uint16_t pad0;
	FSW fsw;
	uint16_t pad1;
	uint16_t tag_word;
	uint16_t pad2;
	uint32_t fip;
	uint16_t fcs;
	uint16_t fop;
	uint32_t fdp;
	uint16_t fds;
	uint16_t pad3;
};

dllexport gnu_noinline void mmx_jank_test() {
	__asm {
		EMMS
		FLDZ
		FLDZ
		FLDZ
		FLDZ
		FLDZ
		FLDZ
		FLDZ
		FLDZ
		FNINIT
		FNCLEX
	}

	struct {
		union {
			float tempf[4];
			int32_t tempi[4];
		};
		x87EnvTest env[4];
	} stack;

	memset(&stack, 0, sizeof(stack));

	__asm {
		FLD1
		FST stack.tempf
	}

	store_x87_env(&stack.env[0]);

	__asm {
		XORPS XMM0, XMM0
		CVTPI2PS XMM0, stack.tempi
		FST stack.tempf + 4
	}

	store_x87_env(&stack.env[1]);

	__asm {
		CVTPI2PS XMM0, MM0
		FST stack.tempf + 8
	}

	store_x87_env(&stack.env[2]);

	__asm {
		MOVDQ2Q MM0, XMM0
		FST stack.tempf + 12
	}

	store_x87_env(&stack.env[3]);

	__asm {
		EMMS
	}

	static constexpr const char* fpu_tag_strings[] = {
		"Valid",
		"Zero ",
		"Other",
		"Empty"
	};

#define TagWordString(i, r) \
		fpu_tag_strings[((uint32_t)stack.env[i].tag_word >> ((r) * 2)) & 0b11]

	printf(
		"x87 base:\n"
		"-TOS: %u\n"
		"-TAG: %04X, %s,%s,%s,%s,%s,%s,%s,%s\n"
		"-VAL: %08X\n"
		"MMX mem->SSE:\n"
		"-TOS: %u\n"
		"-TAG: %04X, %s,%s,%s,%s,%s,%s,%s,%s\n"
		"-VAL: %08X\n"
		"MMX reg->SSE:\n"
		"-TOS: %u\n"
		"-TAG: %04X, %s,%s,%s,%s,%s,%s,%s,%s\n"
		"-VAL: %08X\n"
		"SSE -> MMX:\n"
		"-TOS: %u\n"
		"-TAG: %04X, %s,%s,%s,%s,%s,%s,%s,%s\n"
		"-VAL: %08X\n"
		, stack.env[0].fsw.stack_top
		, stack.env[0].tag_word, TagWordString(0, 0), TagWordString(0, 1), TagWordString(0, 2), TagWordString(0, 3), TagWordString(0, 4), TagWordString(0, 5), TagWordString(0, 6), TagWordString(0, 7)
		, stack.tempi[0]
		, stack.env[1].fsw.stack_top
		, stack.env[1].tag_word, TagWordString(1, 0), TagWordString(1, 1), TagWordString(1, 2), TagWordString(1, 3), TagWordString(1, 4), TagWordString(1, 5), TagWordString(1, 6), TagWordString(1, 7)
		, stack.tempi[1]
		, stack.env[2].fsw.stack_top
		, stack.env[2].tag_word, TagWordString(2, 0), TagWordString(2, 1), TagWordString(2, 2), TagWordString(2, 3), TagWordString(2, 4), TagWordString(2, 5), TagWordString(2, 6), TagWordString(2, 7)
		, stack.tempi[2]
		, stack.env[3].fsw.stack_top
		, stack.env[3].tag_word, TagWordString(3, 0), TagWordString(3, 1), TagWordString(3, 2), TagWordString(3, 3), TagWordString(3, 4), TagWordString(3, 5), TagWordString(3, 6), TagWordString(3, 7)
		, stack.tempi[3]
	);
}

int main() {
	mmx_jank_test();
	return 0;
}