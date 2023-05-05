#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"
#include "ClangAsmTest1.h"

#if IS_X64
#include <iostream>
#endif

#include <stdint.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>

#include <float.h>

#include <Bits.h>

#include <array>

#include "zero/util.h"
//#include "zero/BoundingBox.h"
#undef _WIN64
//#undef WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <joystickapi.h>

#include <stdnoreturn.h>
#include <winternl.h>

#define ThcrapAbsAddr(val) \
void CounterCat(placeholder_func)(void) asm(MACRO_STR(val));

#define thcrap_value(name, value) \
static const unsigned char name[1] asm(value) = {}

//#define thcrap_string(name, thcrap_name, string) \
//static const char name[] asm(thcrap_name) = string

static volatile int32_t test_value asm("<codecave:test_value>") = 1;

void TestFunc2(void) asm("<codecave:TestFunc2>");
void TestFunc2(void) {
	test_value = 0;
}

//thcrap_string(pointer_format_str, "<option:pointer_format_str>", "%p");

void TestFunc(void) asm("<codecave:TestFunc>");
void TestFunc(void) {
	char small_buffer[64];

	//sprintf(small_buffer, pointer_format_str, &TestFunc2);

	thcrap_value(expr_1, "(<option:test> * 5)");
	//sprintf(small_buffer, pointer_format_str, expr_1);
}

#include <math.h>

static inline constexpr float PI_f = M_PI;
static inline constexpr float HALF_PI_f = M_PI_2;
static inline constexpr float TWO_PI_f = 2.0f * M_PI;
static inline constexpr float NPI_f = -M_PI;
static inline constexpr float NHALF_PI_f = -M_PI_2;
static inline constexpr const float NTWO_PI_f = -2.0f * M_PI;
static inline constexpr float RECIP_TWO_PI_f = 1.0f / (2.0f * M_PI);

static inline constexpr double TWO_PI_d = 2.0 * M_PI;

//#if !__has_builtin(__builtin_bit_cast)
//#define __builtin_bit_cast(type, value) ((type)value)
//#endif
//#define bit_cast __builtin_bit_cast
//#define bitcast __builtin_bit_cast

float reduce_angle_old(float angle) {
	int32_t count = 0;
	while (angle > PI_f) {
		angle -= TWO_PI_f;
		if (count++ > 16) break;
	}
	while (angle < NPI_f) {
		angle += TWO_PI_f;
		if (count++ > 16) break;
	}
	return angle;
}

//extern dllexport float cdecl reduce_angle4(float value);
//extern dllexport float cdecl reduce_angle_old(float angle);

float reduce_angle_correct(float value) {
	if (!value) return value;
	float temp = __builtin_remainderf(value, TWO_PI_f);
	return temp;
	//return temp ? temp : __builtin_signbit(temp) ? NPI_f : PI_f;
}

float reduce_angle_cheese(float value) {
	float ret = value;
	float no;
	__asm__(
		"FLDS %1"
		: "=t"(no)
		: "m"(TWO_PI_f)
	);
	__asm__(
		"FPREM1 \n"
		: "+t"(ret)
		: "u"(no)
	);
	/*__asm__(
		"FLDS %2 \n"
		"FLDS %1 \n"
		"FPREM1 \n"
		"FSTPS %0 \n"
		"FFREEP %%st(0) \n"
		: "=m"(ret)
		: "m"(value), "m"(TWO_PI_f)
	);*/
	return ret;
}

gnu_noinline float reduce_angle_new(float value) {
	
	auto temp = _mm_set_ss(value);
	temp[0] *= RECIP_TWO_PI_f;
	//temp = _mm_round_ss(temp, temp, 8);
	return value + temp[0] * NTWO_PI_f;

	//return value + __builtin_nearbyintf(value * RECIP_TWO_PI_f) * NTWO_PI_f;
	//return value + __builtin_roundf(value * RECIP_TWO_PI_f + 0.5f) * NTWO_PI_f;
	//return value + __builtin_rintf(value * RECIP_TWO_PI_f) * NTWO_PI_f;

	//float recip = value * RECIP_TWO_PI_f;
	//_mm_round_ss(recip, recip, 8);

	

	//return value + __builtin_roundf(value * RECIP_TWO_PI_f) * NTWO_PI_f;
	//return value + __builtin_truncf(value * RECIP_TWO_PI_f) * NTWO_PI_f;
	
	//
	//constexpr float recip = 1.0f / PI_f;
	//float temp = value * recip;
	//temp -= __builtin_truncf(value);
	//float garbage;
	//temp = __builtin_modff(temp, &garbage);

	//return temp * TWO_PI_f;
	//return temp * PI_f;
}

/*
enum MatchTypes {
	NoMatch =			0b0000,
	LeftRight =			0b0001,
	TopBottom =			0b0010,
	EnglishReading =	0b0100,
	JapanReading =		0b1000
};
MatchTypes dllexport gnu_noinline parse_letter_array(const char* letters, uint32_t height, uint32_t width, uint32_t match_size) {
	int8_t ret = NoMatch;
	auto& better_letters = (const char[height][width])*letters;
	const char* last_letter = &better_letters[height - 1][width - 1];
	for (uint32_t j = 0; j < height; ++j) {
		const char* last_row_char = &better_letters[j][width - 1];
		for (uint32_t i = 0; i < width; ++i) {
			const char* checked_letter = &better_letters[j][i];
			if (!(ret & EnglishReading)) {
				bool EnglishReadingOrder = true;
				for (uint32_t k = 1; k < match_size; ++k) {
					const char* next_letter = &checked_letter[k];
					if (next_letter > last_letter) break;
					EnglishReadingOrder &= checked_letter[k - 1] < *next_letter;
				}
				ret |= EnglishReadingOrder ? EnglishReading : NoMatch;
			}
			if (!(ret & LeftRight)) {
				if (last_row_char - checked_letter >= match_size) {
					bool LeftRightOrder = true;
					for (uint32_t k = 1; k < match_size; ++k) {
						const char* next_letter = &checked_letter[k];
						if (next_letter > last_letter) break;
						LeftRightOrder &= checked_letter[k - 1] < *next_letter;
					}
					ret |= LeftRightOrder ? LeftRight : NoMatch;
				}
			}
		}
	}
	return (MatchTypes)ret;
}
*/

//#define GS_RELATIVE __attribute__((address_space(256)))
//#define FS_RELATIVE __attribute__((address_space(257)))
//#define SS_RELATIVE __attribute__((address_space(258)))
dllexport int32_t cdecl evil_x64_hook(int32_t(*function)(int32_t, int32_t, ...), uint32_t dummy_segment, ...) {
	uint32_t x64_code_segment = *(uint16_t*)(uintptr_t)(*(uint32_t FS_RELATIVE*)(0xC0) + 5);
	__asm__(
		"MOVL %[x64_segment], 8(%%ESP) \n"
		"LCALL *4(%%ESP) \n"
		"RET \n"
		:
		: [function_addr]"m"(function), [x64_segment]"r"(x64_code_segment), [stack_segment]"m"(dummy_segment)
	);
}

#define BoolStr(val) \
((val) ? "TRUE" : "FALSE")

template <typename T1, typename T2, typename T3>
gnu_noinline void test_float_func(const T1 divisor, T2 correct_func, T3 testing_func) {
	size_t success = 0;
	size_t total = 0;

	union FloatBits {
		uint32_t int_bits;
		float float_bits;
	};

	/*FloatBits value = {
		.int_bits = 0xFF7FFFFF
	};*/

	size_t test_add = 0;

//#define _TEST_BOUNDS 0x60490FDB
//#define TEST_BOUNDS bitcast(float, _TEST_BOUNDS)
#define TEST_BOUNDS 5.795216e+19
	
	vec<float, 1> value2;
	value2[0] = -TEST_BOUNDS;
	//value2[0] = -INFINITY;
	//value2[0] = -TEST_BOUNDS;
	vec<uint32_t, 1> inc_dec = { 1 };

	//while (value.int_bits-- != 0x80000000) {
	//	// Negative floats
	//	++total;
	//	success += correct_func(value.float_bits, divisor) == testing_func(value.float_bits, divisor);
	//};
	while (value2[0] != bitcast<float>(0x80000000)) {
		*(vec<uint32_t, 1>*)&value2 -= inc_dec;
		// Negative floats
		++total;
		//success += correct_func(value2[0], divisor) == testing_func(value2[0], divisor);
		__asm__(
			"UCOMISS %[correct], %[testing] \n"
			"SETE %b[test_add]"
			: [test_add]"+b"(test_add)
			: [correct]"mxt"(correct_func(value2[0], divisor)), [testing]"x"(testing_func(value2[0], divisor))
		);
		success += test_add;
	}


	//{
	//	value.int_bits = 0;
	//	// Zero
	//	++total;
	//	success += correct_func(value.float_bits, divisor) == testing_func(value.float_bits, divisor);
	//}
	{
		//value2 = { 0.0f, 0.0f, 0.0f, 0.0f };
		value2 = { 0.0f };
		// Zero
		++total;
		//success += correct_func(value2[0], divisor) == testing_func(value2[0], divisor);
		__asm__(
			"UCOMISS %[correct], %[testing] \n"
			"SETE %b[test_add]"
			: [test_add]"+b"(test_add)
			: [correct]"mxt"(correct_func(value2[0], divisor)), [testing]"x"(testing_func(value2[0], divisor))
		);
		success += test_add;
	}


	//while (value.int_bits++ != 0x7F800000) {
	//	// Positive floats
	//	++total;
	//	success += correct_func(value.float_bits, divisor) == testing_func(value.float_bits, divisor);
	//}
	//while (value2[0] != bitcast(float, 0x7F800000)) {
	while (value2[0] != TEST_BOUNDS) {
	//while (value2[0] != TEST_BOUNDS) {
		*(vec<uint32_t, 1>*)&value2 += inc_dec;
		// Positive floats
		++total;
		//success += correct_func(value2[0], divisor) == testing_func(value2[0], divisor);
		__asm__(
			"UCOMISS %[correct], %[testing] \n"
			"SETE %b[test_add]"
			: [test_add]"+b"(test_add)
			: [correct]"mxt"(correct_func(value2[0], divisor)), [testing]"x"(testing_func(value2[0], divisor))
		);
		success += test_add;
	}
	printf(
		"Float jank test\n"
		"Valid:\t4278190080\n"
		"Total:\t%zu\n"
		"Pass:\t%zu\n"
		, total
		, success
	);
}

auto correct_remainderf = [](float value, float divisor) -> float {
	return __builtin_remquof(value, divisor, NULL);
	//return __builtin_remainderf(value, divisor);
};


auto correct_remainderf2 = [](float value, double divisor) -> float {
	return __builtin_remquo(value, divisor, NULL);
	//return __builtin_remainderf(value, divisor);
};

auto correct_remainderf3 = [](float value, float divisor) -> float {
	/*__asm__(
		"FLDS %[divisor] \n"
		"FLDS %[value] \n"
		: "=t"(ret)
		: [value]"m"(value), [divisor]"m"(divisor)
	);*/
	__asm__(
		"FPREM1"
		: "+t"(value)
		: "u"(divisor)
	);
	return value;
};

auto correct_remainderf4 = [](float value, double divisor) -> float {
	/*__asm__(
		"FLDS %[divisor] \n"
		"FLDS %[value] \n"
		: "=t"(ret)
		: [value]"m"(value), [divisor]"m"(divisor)
	);*/
	__asm__(
		"FPREM1"
		: "+t"(value)
		: "u"(divisor)
	);
	return value;
};

auto test_remainderf = [](float value, float divisor) -> float {
	float temp = value * (1.0f / divisor);
	temp -= __builtin_truncf(temp);
	return temp * divisor;
};

auto test_remainderf2 = [](float value, double divisor) -> float {
	double temp = (double)value * (1.0f / divisor);
	temp -= __builtin_trunc(temp);
	return temp * divisor;
};

auto test_remainderf3 = [](float value, float divisor) -> float {
	vec<float, 4> temp = { value * (1.0f / divisor) };
	vec<float, 4> temp2 = _mm_round_ss(temp, temp, 0x8);
	temp[0] -= temp2[0];
	return temp[0] * divisor;
};

auto test_remainderf4 = [](float value, double divisor) -> float {
	vec<double, 2> temp = { (double)value * (1.0 / divisor) };
	vec<double, 2> temp2 = _mm_round_sd(temp, temp, 0x8);
	temp[0] -= temp2[0];
	return temp[0] * divisor;
};

static inline constexpr float minimum_negative_float_value = -0x1.FFFFFEp127f;
static inline constexpr float maximum_negative_float_value = -0x0.0p0f;
static inline constexpr float minimum_positive_float_value = 0x0.0p0f;
static inline constexpr float maximum_positive_float_value = 0x1.FFFFFEp127f;

static inline constexpr size_t max_float_test_iters = 4278190080;

static constexpr const x86FloatPrecision precision_table[] = {
	SinglePrecision,
	DoublePrecision,
	ExtendedPrecision
};
static constexpr const x86FloatRounding rounding_table[] = {
	RoundToNearest,
	RoundDown,
	RoundUp,
	RoundTowardsZero
};

static constexpr FCW fcw_table[countof(precision_table)][countof(rounding_table)] = {
	{ 0x007F, 0x047F, 0x087F, 0x0C7F },
	{ 0x027F, 0x067F, 0x0A7F, 0x0E7F },
	{ 0x037F, 0x077F, 0x0B7F, 0x0F7F }
};
static constexpr MXCSR mxcsr_table[countof(rounding_table)] = {
	0x00001F80, 0x00003F80, 0x00005F80, 0x00007F80
};

template<typename T, typename L1, typename L2>
dllexport gnu_noinline void vectorcall ultimate_float_reduce_test(T& success_counts, long double divisor, L1 funcA, L2 funcB) {
	FCW initial_fcw;
	store_fcw(initial_fcw);
	MXCSR initial_mxcsr;
	store_mxcsr(initial_mxcsr);
	nounroll for (size_t i = 0; i < countof(rounding_table); ++i) {
		load_mxcsr(mxcsr_table[i]);
		nounroll for (size_t j = 0; j < countof(precision_table); ++j) {
			load_fcw(fcw_table[j][i]);
			size_t& current_success_count = success_counts[j][i];
			for (
				uint32_t value = bitcast<uint32_t>(minimum_negative_float_value);
				value >= bitcast<uint32_t>(maximum_negative_float_value);
				--value
			) {
				union {
					uint32_t as_int;
					float as_float;
				} temp_value;
				temp_value.as_int = value;
				__asm__ volatile (""::"m"(temp_value), "r"(value));
				uint32_t resultA = funcA(temp_value.as_float, divisor);
				uint32_t resultB = funcB((double)temp_value.as_float, divisor);
				bool carry;
				__asm__ volatile (
					"subl %[resultA], %[resultB] \n"
					"negl %[resultB] \n"
					: asm_flags(c, carry)
					: asm_arg("m", resultA), asm_arg("r", resultB)
				);
				current_success_count -= carry;
			}
			for (
				uint32_t value = bitcast<uint32_t>(minimum_positive_float_value);
				value <= bitcast<uint32_t>(maximum_positive_float_value);
				++value
			) {
				union {
					uint32_t as_int;
					float as_float;
				} temp_value;
				temp_value.as_int = value;
				__asm__ volatile (""::"m"(temp_value), "r"(value));
				uint32_t resultA = funcA(temp_value.as_float, divisor);
				uint32_t resultB = funcB((double)temp_value.as_float, divisor);
				bool carry;
				__asm__ volatile (
					"subl %[resultA], %[resultB] \n"
					"negl %[resultB] \n"
					: asm_flags(c, carry)
					: asm_arg("m", resultA), asm_arg("r", resultB)
				);
				current_success_count -= carry;
			}
		}
	}
	load_fcw(initial_fcw);
	load_mxcsr(initial_mxcsr);
}

dllexport gnu_noinline void run_ultimate_float_reduce_test() {

	//std::array<size_t, (countof(rounding_table) * countof(precision_table))> success_counts;
	//success_counts.fill(max_float_test_iters);
	size_t success_counts[countof(precision_table)][countof(rounding_table)];
	for (size_t i = 0; i < countof(rounding_table); ++i) {
		for (size_t j = 0; j < countof(precision_table); ++j) {
			success_counts[j][i] = max_float_test_iters;
		}
	}

	ultimate_float_reduce_test(
		success_counts,
		(long double)M_PI,
		[](long double dividend, long double divisor) -> uint32_t {
			__asm__ volatile (
				"fprem1"
				: "+t"(dividend)
				: "u"(divisor)
			);
			float ret = dividend;
			return bitcast<uint32_t>(ret);
		},
		[](double dividend, double divisor) -> uint32_t {
			double value = dividend / (divisor * 2.0);
			value -= _mm_round_sd((__m128d){value}, (__m128d){ value }, _MM_FROUND_NO_EXC | _MM_FROUND_TO_NEAREST_INT)[0];
			float ret = value * (divisor * 2.0);
			return bitcast<uint32_t>(ret);
		}
	);
	printf(
		"Single Precision Float Reduction Tests:\n"
		"\n"
		"ToNearest, Single:   %10zu/4278190080\n"
		"ToNearest, Double:   %10zu/4278190080\n"
		"ToNearest, Extended: %10zu/4278190080\n"
		"Down, Single:        %10zu/4278190080\n"
		"Down, Double:        %10zu/4278190080\n"
		"Down, Extended:      %10zu/4278190080\n"
		"Up, Single:          %10zu/4278190080\n"
		"Up, Double:          %10zu/4278190080\n"
		"Up, Extended:        %10zu/4278190080\n"
		"ToZero, Single:      %10zu/4278190080\n"
		"ToZero, Double:      %10zu/4278190080\n"
		"ToZero, Extended:    %10zu/4278190080\n"
		, success_counts[SinglePrecision][RoundToNearest]
		, success_counts[DoublePrecision-1][RoundToNearest]
		, success_counts[ExtendedPrecision-1][RoundToNearest]
		, success_counts[SinglePrecision][RoundDown]
		, success_counts[DoublePrecision-1][RoundDown]
		, success_counts[ExtendedPrecision-1][RoundDown]
		, success_counts[SinglePrecision][RoundUp]
		, success_counts[DoublePrecision-1][RoundUp]
		, success_counts[ExtendedPrecision-1][RoundUp]
		, success_counts[SinglePrecision][RoundTowardsZero]
		, success_counts[DoublePrecision-1][RoundTowardsZero]
		, success_counts[ExtendedPrecision-1][RoundTowardsZero]
	);
}

#include "windows_crap.h"

extern
//[[clang::weak_import]]
__kernel_entry NTSTATUS
NTAPI
NtSetInformationProcess(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESSINFOCLASS ProcessInformationClass,
	_In_ PVOID ProcessInformation,
	_In_ ULONG ProcessInformationLength
) asm("_NtSetInformationProcess@16");

dllexport gnu_noinline uint64_t test_loop(size_t count);

extern void run_float_jank_test();

typedef
_Function_class_(RTL_UMS_SCHEDULER_ENTRY_POINT)
VOID
NTAPI
RTL_UMS_SCHEDULER_ENTRY_POINT(
	_In_ RTL_UMS_SCHEDULER_REASON Reason,
	_In_ ULONG_PTR ActivationPayload,
	_In_ PVOID SchedulerParam
);



DWORD stdcall plz_work(LPVOID) {
	while (true) {
		printf("hi");
	}
	UmsThreadYield(NULL);
	//__asm int3;
}

void make_thread() {

}

static struct alignas(64) yeeeeeee {
	char big_honking_context[0x2000];
} context_dump;

void stdcall ums_test(RTL_UMS_SCHEDULER_REASON Reason, ULONG_PTR ActivationPayload, PVOID SchedulerParam) {

	/*volatile uint16_t segment = 0;
	uint32_t limit = 0;
	uint32_t data = 0;
	uint8_t valid = false;
	size_t valid_count = 0;
	do {
		__asm__(
			"LARL %3, %2 \n"
			"LSLL %3, %0 \n"
			"SETZ %1 \n"
			: "=r"(limit), "=r"(valid), "=r"(data)
			: "r"(segment)
		);
		if (valid) {
			++valid_count;
			LDT_ENTRY ldt;
			valid = GetThreadSelectorEntry(GetCurrentThread(), segment, &ldt);
			uint32_t base = ldt.BaseLow | ldt.HighWord.Bytes.BaseMid << 16 | ldt.HighWord.Bytes.BaseHi << 24;
			uint32_t limit2 = ldt.LimitLow | ldt.HighWord.Bits.LimitHi << 16;
			if (ldt.HighWord.Bits.Granularity) limit2 = limit2 << 12 | 0xFFF;
			printf(
				"Valid Segment:\t%hX\tBase:\t%X\tLimit:\t%X(%X)\t\n"
				, segment, base, limit, limit2
			);
		}
	} while (++segment != 0);
	return;*/

	static PUMS_CONTEXT context;
	/*
	switch (Reason) {
		case UmsSchedulerStartup:
		{
			printf("start\n");
			CreateUmsThreadContext(&context);
			PUMS_COMPLETION_LIST list;
			CreateUmsCompletionList(&list);
			size_t size;
			InitializeProcThreadAttributeList(NULL, 1, 0, (PSIZE_T)&size);
			uint8_t lol_idk[size];
			auto attr_list = (LPPROC_THREAD_ATTRIBUTE_LIST)&lol_idk;
			InitializeProcThreadAttributeList(attr_list, 1, 0, (PSIZE_T)&size);
			UMS_CREATE_THREAD_ATTRIBUTES attr = {
				.UmsVersion = UMS_VERSION,
				.UmsContext = &context,
				//.UmsContext = (PUMS_CONTEXT)&context_dump,
				.UmsCompletionList = list,
			};
			UpdateProcThreadAttribute(attr_list, 0, PROC_THREAD_ATTRIBUTE_UMS_THREAD, &attr, sizeof(attr), NULL, NULL);
			CreateRemoteThreadEx(GetCurrentProcess(), NULL, 0, &plz_work, NULL, 0, attr_list, NULL);
		}
		case UmsSchedulerThreadBlocked:
		case UmsSchedulerThreadYield:
			ExecuteUmsThread(&context);
	}
	*/
	return;
	//ExecuteUmsThread()
	//int yeet;
	//UmsThreadYield(&yeet);
}

enum THREAD_UMS_INFORMATION_COMMAND {
	UmsInformationCommandInvalid = 0,
	UmsInformationCommandAttach = 1,
	UmsInformationCommandDetach = 2,
	UmsInformationCommandQuery = 3,
};


//#include "BSTTest.h"


//#include <ntifs.h>

extern
__checkReturn
__drv_allocatesMem(Mem)
__kernel_entry //NTSYSCALLAPI
NTSTATUS
NTAPI
NtAllocateVirtualMemory(
	__in HANDLE ProcessHandle,
	__inout PVOID *BaseAddress,
	__in ULONG_PTR ZeroBits,
	__inout PSIZE_T RegionSize,
	__in ULONG AllocationType,
	__in ULONG Protect
) asm("NtAllocateVirtualMemory");

extern
__kernel_entry //NTSYSCALLAPI
NTSTATUS
NTAPI
NtFreeVirtualMemory(
	__in HANDLE ProcessHandle,
	__inout __drv_freesMem(Mem) PVOID *BaseAddress,
	__inout PSIZE_T RegionSize,
	__in ULONG FreeType
) asm("NtFreeVirtualMemory");

extern
__checkReturn
__drv_allocatesMem(Mem)
__kernel_entry //NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateProcess(
	HANDLE* ProcessHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	HANDLE ParentProcess,
	BOOLEAN InheritObjectTable,
	HANDLE SectionHandle,
	HANDLE DebugPort,
	HANDLE ExceptionPort
) asm("NtCreateProcess");


dllexport volatile void* shared_buffer;
dllexport void access_file_mapping() {
	char mapping_name[] = "EoSDTst";
	HANDLE shared_mapping;
	
	if (!(shared_mapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, mapping_name))) {
		shared_mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 64, mapping_name);
	}
	shared_buffer = MapViewOfFile(shared_mapping, FILE_MAP_ALL_ACCESS, 0, 0, 64);
}

//#include <functional>

template <bool value, sfinae_enable(value)>
struct sfinae_enable_struct {};

template <auto Func, typename... Fargs, sfinae_enable(std::is_invocable_v<typeof(Func), Fargs...>)>
dllexport gnu_used auto call_func(Fargs... args) {
	return std::invoke(Func, args...);
}

struct CallFunc {
	template <auto Func, typename... Fargs, sfinae_enable(std::is_invocable_v<typeof(Func), Fargs...>)>
	dllexport gnu_used gnu_noinline static auto call(Fargs... args) asm_symbol_rel(test) {
		return std::invoke(Func, args...);
	}
};


using testing_t = decltype(get_func_types_of(&GetModuleFileNameA));

#ifndef __x86_64__
#define thcrap_builtin(func_name, ...) \
extern "C++" { \
using MACRO_CAT(func_name,T) = typeof(MACRO_DEFAULT_ARG(func_name,__VA_ARGS__)); \
using MACRO_CAT(func_name,Ptr) = MACRO_CAT(func_name,T)*; \
dllexport gnu_used struct unique_name(dummy) { \
private: \
	using F = decltype(MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)); \
	using FTypes = decltype(get_func_types_of(std::declval<F>())); \
	using FArgs = FTypes::Types; \
	using FRet = FTypes::R; \
	template <typename> \
	struct CallFunc {}; \
	template <typename... Args> \
	struct CallFunc<Types<Args...>> { \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(cdecl*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto cdecl call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(stdcall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto stdcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(fastcall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto fastcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(thiscall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto thiscall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(vectorcall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto vectorcall call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(cdecl*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto cdecl addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(stdcall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto stdcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(fastcall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto fastcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(thiscall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto thiscall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(vectorcall*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto vectorcall addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
	}; \
public: \
	inline operator F*() { \
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	inline F* operator&() {\
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	template <typename... Args> \
	requires std::is_invocable_v<F*, Args...> \
	inline auto operator()(Args... args) { \
		return CallFunc<FArgs>::call(args...); \
	} \
} MACRO_CAT(th_,func_name); \
}
#else
#define thcrap_builtin(func_name, ...) \
extern "C++" { \
using MACRO_CAT(func_name,T) = typeof(MACRO_DEFAULT_ARG(func_name,__VA_ARGS__)); \
using MACRO_CAT(func_name,Ptr) = MACRO_CAT(func_name,T)*; \
dllexport gnu_used struct unique_name(dummy) { \
private: \
	using F = decltype(MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)); \
	using FTypes = decltype(get_func_types_of(std::declval<F>())); \
	using FArgs = FTypes::Types; \
	using FRet = FTypes::R; \
	template <typename> \
	struct CallFunc {}; \
	template <typename... Args> \
	struct CallFunc<Types<Args...>> { \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto call(Args... args) asm_symbol_rel(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
		template <typename... Fargs> \
		requires std::is_same_v<FRet(*)(Args...), F*> \
		dllexport gnu_used static gnu_noinline auto addr(Args... args) asm_symbol_abs(MACRO_CAT(th_,func_name)) { \
			gnu_attr(musttail) return MACRO_DEFAULT_ARG(func_name, __VA_ARGS__)(args...); \
		} \
	}; \
public: \
	inline operator F*() { \
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	inline F* operator&() {\
		return (F*)&CallFunc<FArgs>::addr; \
	} \
	template <typename... Args> \
	requires std::is_invocable_v<F*, Args...> \
	inline auto operator()(Args... args) { \
		return CallFunc<FArgs>::call(args...); \
	} \
} MACRO_CAT(th_,func_name); \
}
#endif

#define thcrap_import(func_name, ...) \
using MACRO_CAT(func_name,Ptr) = decltype(&MACRO_DEFAULT_ARG(func_name,__VA_ARGS__)); \
dllexport gnu_used MACRO_CAT(func_name,Ptr) MACRO_CAT(func_name,_ptr) asm_symbol_abs(option:MACRO_CAT(func_name,_ptr))

#define thcrap_string(option_name, text) \
dllexport char option_name[] asm_symbol_abs(option:option_name) = text

#define thcrap_wstring(option_name, text) \
dllexport wchar_t option_name[] asm_symbol_abs(option:option_name) = text

#define thcrap_utf8_string(option_name, text) \
dllexport char8_t option_name[] asm_symbol_abs(option:option_name) = text

#define thcrap_expr(expr_name, value) \
dllexport gnu_used struct { \
	inline operator usreg_t() { \
		return (usreg_t)this; \
	} \
} expr_name asm_symbol_expr(value)

dllexport DWORD prev_time asm_symbol_abs(codecave:eosd_framelimiter_fix_data);
dllexport double supervisor_frame_time asm_symbol_abs(0x6C6BF8);

static constexpr double frame_ms = bitcast<double>(0x4030AAAAAAAAAAABull);

dllexport uint8_t supervisor_config_fullscreen_setting asm_symbol_abs(0x6C6E4A);
dllexport uint8_t supervisor_config_frameskip_setting asm_symbol_abs(0x6C6E4B);
dllexport union unique_name(dummy) {
	uint32_t raw;
	struct {
		uint32_t : 3;
		uint32_t force_redraw_A : 1;
		uint32_t force_redraw_B : 1;
		uint32_t : 2;
		uint32_t force_60_fps : 1;
	};
} supervisor_config_flags asm_symbol_abs(0x6C6E60);

enum SyncType {
	Vsync = 0,
	Async = 1
};
dllexport SyncType supervisor_sync_type asm_symbol_abs(0x6C6EB8);
dllexport float supervisor_slowdown asm_symbol_abs(0x6C6EC4);

dllexport gnu_noinline void render_windw() asm_symbol_rel(0x420B50);
dllexport gnu_noinline void render_windw() {
	assume_all_registers_volatile();
}

#include <timeapi.h>
struct Window {
	HWND main_window; // 0x0
	int __dword_4; // 0x4
	BOOL disable_redraw; // 0x8
	int __dword_C; // 0xC
	uint8_t frames_skipped; // 0x10

	dllexport gnu_noinline int32_t thiscall do_tick() {
		assume_all_registers_volatile();
		if (this->disable_redraw) {
			return 0;
		}
		if (!this->frames_skipped) {
			goto EnterWhenZero;
		}
		while (supervisor_config_frameskip_setting > this->frames_skipped) {
			assume_all_registers_volatile();
			++this->frames_skipped;
EnterWhenZero:
			assume_all_registers_volatile();
			uint8_t fullscreen_setting = supervisor_config_fullscreen_setting;
			if ((fullscreen_setting || !(supervisor_config_flags.force_60_fps && supervisor_sync_type == Async)) && this->frames_skipped != 0) {
				supervisor_slowdown = 1.0f;
				//timeBeginPeriod(1);
				DWORD time_value_int;
				//time_value_int = (double)timeGetTime();
				//timeEndPeriod(1);
				DWORD prev_time_local = prev_time;
				prev_time = time_value_int;
				DWORD current_time;
				if (__builtin_sub_overflow(time_value_int, prev_time_local, &current_time)) {
					current_time = time_value_int;
				}
				double time_value = (double)current_time;
				double frame_time = supervisor_frame_time;
				if (time_value < frame_time) {
					frame_time = time_value;
				}
				supervisor_frame_time = frame_time;
				time_value = __builtin_fabs(time_value - frame_time);
				if (time_value < frame_ms) {
					goto SkipJank;
				}
				do {
					frame_time += frame_ms;
					time_value -= frame_ms;
				} while (time_value >= frame_ms);
				supervisor_frame_time = frame_time;
			}
			else {
SkipJank:
				if (!(fullscreen_setting || !(supervisor_config_flags.force_60_fps && supervisor_sync_type == Async))) {
					return 0;
				}
				if (supervisor_config_frameskip_setting >= this->frames_skipped) {
					render_windw();
				}
			}
		}
		render_windw();
		
		/*
		timeBeginPeriod(1);
		DWORD time_value = timeGetTime();
		timeEndPeriod(1);
		DWORD prev_time_local = prev_time;
		prev_time = time_value;
		//time_value -= prev_time_local;
		DWORD current_time;
		if (__builtin_sub_overflow(time_value, prev_time_local, &current_time)) {
			current_time = time_value;
		}
		double current_time_double = current_time;
		double frame_time_local = supervisor_frame_time;
		if (current_time_double < frame_time_local) {
			frame_time_local = current_time_double;
		}
		current_time_double = __builtin_fabs(current_time_double - frame_time_local);
		while (current_time_double >= frame_ms) {
			frame_time_local += frame_ms;
			current_time_double -= frame_ms;
		}
		*/
		return 0;
	}
};

//#define __CLANG_STDATOMIC_H 1
#include <atomic>

template <typename T>
union CustomSyncType {
	T as_normal;
	std::atomic<T> as_atomic;
	volatile T as_volatile;
	volatile std::atomic<T> as_volatile_atomic;
};



typedef struct ScorefileCATK ScorefileCATK;
struct ScorefileCATK {
	char magic_CATK[0x4];  // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	int32_t highscore;  // 0xc
	short __word_10;  // 0x10
	uint8_t spell_name_hash;  // 0x12
	uint8_t most_recent_capture_shottype;  // 0x13
	unknown_fields(0x4);  // 0x14
	char spell_name[36];  // 0x18
	short attempts;  // 0x3c
	short captures;  // 0x3e
};  // 0x40

typedef struct ScorefileCLRD ScorefileCLRD;
struct ScorefileCLRD {
	char magic_CLRD[0x4];  // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	uint8_t clear_counts[0x5];  // 0xc
	uint8_t continue_counts[0x5];  // 0x11
	uint8_t character_shot_id;  // 0x16
	probably_padding_bytes(0x1);  // 0x17
};  // 0x18

typedef struct ScorefilePSCR ScorefilePSCR;
struct ScorefilePSCR {
	char magic_PSCR[0x4];  // 0x0
	uint16_t size_1;  // 0x4
	uint16_t size_2;  // 0x6
	uint8_t __byte_8;  // 0x8
	probably_padding_bytes(0x3);  // 0x9
	int32_t score;  // 0xc
	uint8_t character;  // 0x10
	uint8_t difficulty;  // 0x11
	uint8_t stage;  // 0x12
	probably_padding_bytes(0x1);  // 0x13
};  // 0x14

struct EoSDMultiSharedData {
	CustomSyncType<uint8_t> next_index;
	//CustomSyncType<int8_t> scorefile_lock;
	//padding_bytes(0xD);
	//ScorefileCATK catk_data[0x40];
	//ScorefileCLRD clrd_data[0x4];
	//ScorefilePSCR pscr_data[0x60];
};

#include <XInput.h>

extern "C" {


thcrap_string(eosd_multi_score_dat_str, "multiscore.dat");
thcrap_string(eosd_multi_mapping_name, "EoSDMulti");

thcrap_string(kernel32_dll_str, "kernel32.dll");
thcrap_string(user32_dll_str, "USER32.dll");
thcrap_string(win32_utf8_dll_str, "win32_utf8.dll");
thcrap_string(ntdll_dll_str, "ntdll.dll");


thcrap_string(OpenFileMappingA_str, "OpenFileMappingA");
thcrap_string(CreateFileMappingA_str, "CreateFileMappingA");
thcrap_string(MapViewOfFile_str, "MapViewOfFile");
thcrap_string(UnmapViewOfFile_str, "UnmapViewOfFile");
thcrap_string(Sleep_str, "Sleep");
thcrap_string(LoadLibraryA_str, "LoadLibraryA");
thcrap_string(XInputGetState_str, "XInputGetState");
thcrap_string(GetKeyState_str, "GetKeyState");

thcrap_string(thcrap_loader_str, "\\bin\\thcrap_loader.exe");

thcrap_string(eosd_multi_title_n_str, " \"{\\\"title\\\":\\\"The Embodiment of Scarlet Devil Normal\\\"}\"");
thcrap_string(eosd_multi_title_h_str, " \"{\\\"title\\\":\\\"The Embodiment of Scarlet Devil Hard\\\"}\"");
thcrap_string(eosd_multi_title_l_str, " \"{\\\"title\\\":\\\"The Embodiment of Scarlet Devil Lunatic\\\"}\"");

thcrap_string(eosd_multi_title_new_e_str, "The Embodiment of Scarlet Devil Easy");
thcrap_string(eosd_multi_title_new_n_str, "The Embodiment of Scarlet Devil Normal");
thcrap_string(eosd_multi_title_new_h_str, "The Embodiment of Scarlet Devil Hard");
thcrap_string(eosd_multi_title_new_l_str, "The Embodiment of Scarlet Devil Lunatic");

thcrap_string(xinput_1_4_dll_str, "xinput1_4.dll");
thcrap_string(xinput_1_3_dll_str, "xinput1_3.dll");
thcrap_string(xinput9_1_0_dll_str, "XInput9_1_0.dll");

dllexport char* eosd_multi_title_strs[] asm_symbol_expr(<codecave:eosd_multi_constant_pool> + 0x80) = {
	eosd_multi_title_n_str,
	eosd_multi_title_h_str,
	eosd_multi_title_l_str,
};
dllexport char* eosd_multi_xinput_lib_names[] asm_symbol_expr(<codecave:eosd_multi_constant_pool> +0x8C) = {
	xinput_1_4_dll_str,
	xinput_1_3_dll_str,
	xinput9_1_0_dll_str
};
dllexport char derp[] asm_symbol_expr(<codecave:eosd_multi_constant_pool> + 0x98) = "derp";

//thcrap_string(OpenProcess_str, "OpenProcess");
thcrap_string(CloseHandle_str, "CloseHandle");
//thcrap_string(GetModuleFileNameW_str, "GetModuleFilenameW");
//thcrap_string(GetModuleFileNameA_str, "GetModuleFilenameA");
//thcrap_string(GetModuleFileNameU_str, "GetModuleFilenameU");
//thcrap_string(GetCurrentProcess_str, "GetCurrentProcess");
//thcrap_string(CreateProcessW_str, "CreateProcessW");
thcrap_string(CreateProcessA_str, "CreateProcessA");
thcrap_string(GetFileAttributesW_str, "GetFileAttributesW");
thcrap_string(GetCurrentDirectoryW_str, "GetCurrentDirectoryW");
thcrap_string(CreateDirectoryA_str, "CreateDirectoryA");
thcrap_string(ExitProcess_str, "ExitProcess");

thcrap_string(eosd_multi_score_dat_filename_E_str, "multiscoreE.dat");
thcrap_string(eosd_multi_score_dat_filename_N_str, "multiscoreN.dat");
thcrap_string(eosd_multi_score_dat_filename_H_str, "multiscoreH.dat");
thcrap_string(eosd_multi_score_dat_filename_L_str, "multiscoreL.dat");
thcrap_string(eosd_multi_log_filename_E_str, "./logE.txt");
thcrap_string(eosd_multi_log_filename_N_str, "./logN.txt");
thcrap_string(eosd_multi_log_filename_H_str, "./logH.txt");
thcrap_string(eosd_multi_log_filename_L_str, "./logL.txt");
thcrap_string(eosd_multi_replay_filename_E_str, "./multireplay/th6E_%2d.rpy");
thcrap_string(eosd_multi_replay_filename_N_str, "./multireplay/th6N_%2d.rpy");
thcrap_string(eosd_multi_replay_filename_H_str, "./multireplay/th6H_%2d.rpy");
thcrap_string(eosd_multi_replay_filename_L_str, "./multireplay/th6L_%2d.rpy");
thcrap_string(eosd_multi_default_replay_filename_E_str, "multireplay/th6E_00.rpy");
thcrap_string(eosd_multi_default_replay_filename_N_str, "multireplay/th6N_00.rpy");
thcrap_string(eosd_multi_default_replay_filename_H_str, "multireplay/th6H_00.rpy");
thcrap_string(eosd_multi_default_replay_filename_L_str, "multireplay/th6L_00.rpy");
thcrap_string(eosd_multi_replay_ud_filename_E_str, "th6E_ud????.rpy");
thcrap_string(eosd_multi_replay_ud_filename_N_str, "th6N_ud????.rpy");
thcrap_string(eosd_multi_replay_ud_filename_H_str, "th6H_ud????.rpy");
thcrap_string(eosd_multi_replay_ud_filename_L_str, "th6L_ud????.rpy");
thcrap_string(eosd_multi_replay_folder_str, "./multireplay/");

dllexport vec<uint32_t, 4> per_index_data_arrayA[] asm_symbol_expr(<codecave:eosd_multi_constant_pool> + 0x20) = {
	{ (uint32_t)&eosd_multi_log_filename_E_str, (uint32_t)&eosd_multi_replay_filename_E_str, (uint32_t)&eosd_multi_default_replay_filename_E_str, (uint32_t)&eosd_multi_replay_ud_filename_E_str },
	{ (uint32_t)&eosd_multi_log_filename_N_str, (uint32_t)&eosd_multi_replay_filename_N_str, (uint32_t)&eosd_multi_default_replay_filename_N_str, (uint32_t)&eosd_multi_replay_ud_filename_N_str },
	{ (uint32_t)&eosd_multi_log_filename_H_str, (uint32_t)&eosd_multi_replay_filename_H_str, (uint32_t)&eosd_multi_default_replay_filename_H_str, (uint32_t)&eosd_multi_replay_ud_filename_H_str },
	{ (uint32_t)&eosd_multi_log_filename_L_str, (uint32_t)&eosd_multi_replay_filename_L_str, (uint32_t)&eosd_multi_default_replay_filename_L_str, (uint32_t)&eosd_multi_replay_ud_filename_L_str },
};

dllexport const char* per_index_data_arrayB[] asm_symbol_expr(<codecave:eosd_multi_constant_pool> + 0x60) = {
	eosd_multi_score_dat_filename_E_str,
	eosd_multi_score_dat_filename_N_str,
	eosd_multi_score_dat_filename_H_str,
	eosd_multi_score_dat_filename_L_str,
};
//dllexport const char* xinput_lib_names[] asm_symbol_expr()

thcrap_import(OpenFileMappingA);
thcrap_import(CreateFileMappingA);
thcrap_import(MapViewOfFile);
thcrap_import(UnmapViewOfFile);
thcrap_import(Sleep);
thcrap_import(CreateProcessA);
thcrap_import(CloseHandle);
thcrap_import(LoadLibraryA);
thcrap_import(XInputGetState);
thcrap_import(GetKeyState);
//thcrap_import(GetFileAttributesW);
//thcrap_import(GetCurrentDirectoryW);
thcrap_import(CreateDirectoryA);
thcrap_import(ExitProcess);
//thcrap_import(GetModuleFileNameW);
//thcrap_import(GetModuleFileNameA);
//thcrap_import(GetCurrentProcess);
//thcrap_import(CreateProcessW);
//thcrap_import(GetModuleFileNameU, GetModuleFileNameA);


dllexport gnu_noinline const char* runconfig_game_get() asm_symbol_rel(runconfig_game_get);
dllexport gnu_noinline const char* runconfig_game_get() {
	return (const char*)(uintptr_t)rand();
}
dllexport gnu_noinline const char* runconfig_thcrap_dir_get() asm_symbol_rel(runconfig_thcrap_dir_get);
dllexport gnu_noinline const char* runconfig_thcrap_dir_get() {
	return (const char*)(uintptr_t)rand();
}
dllexport gnu_noinline const char* runconfig_runcfg_fn_get() asm_symbol_rel(runconfig_runcfg_fn_get);
dllexport gnu_noinline const char* runconfig_runcfg_fn_get() {
	return (const char*)(uintptr_t)rand();
}

dllexport gnu_noinline int log_mboxf(const char* caption, const UINT type, const char* format, ...) asm_symbol_rel(log_mboxf);
dllexport gnu_noinline int log_mboxf(const char* caption, const UINT type, const char* format, ...) {
	use_var(caption);
	use_var(type);
	va_list va;
	va_start(va, format);
	int ret = vsnprintf(NULL, 0, format, va_use(va));
	va_end(va);
	return ret + rand();
}

thcrap_builtin(malloc);
thcrap_builtin(free);
thcrap_builtin(strlen);
thcrap_builtin(memcpy);
thcrap_builtin(strcpy);
thcrap_builtin(GetModuleHandleA);
thcrap_builtin(GetProcAddress);

//static constexpr auto catk_offset = offsetof(EoSDMultiSharedData, catk_data);
//static constexpr auto clrd_offset = offsetof(EoSDMultiSharedData, clrd_data);
//static constexpr auto pscr_offset = offsetof(EoSDMultiSharedData, pscr_data);


dllexport volatile EoSDMultiSharedData* shared_data_ptr asm_symbol_abs(option:eosd_multi_shared_data_ptr);
dllexport HANDLE shared_mapping_handle asm_symbol_abs(option:eosd_multi_shared_data_handle_ptr);
dllexport uint8_t instance_index asm_symbol_abs(option:eosd_multi_instance_index_ptr);
dllexport HMODULE xinput_handle asm_symbol_abs(option:eosd_multi_xinput_handle_ptr);
dllexport const char* score_dat_filename asm_symbol_expr(<codecave:eosd_multi_per_instance_data> + 0xC);
//dllexport void*(cdecl*load_scorefile_func_ptr)(const char*) asm_symbol_expr(<codecave:eosd_multi_per_instance_data> + 0xC);
dllexport const char* logfile_filename asm_symbol_expr(<codecave:eosd_multi_per_instance_data> + 0x10);
dllexport const char* replay_filename asm_symbol_abs(option:eosd_multi_replay_filename_ptr);
dllexport const char* replay_default_filename asm_symbol_abs(option:eosd_multi_replay_default_filename_ptr);
dllexport const char* replay_ud_filename asm_symbol_abs(option:eosd_multi_replay_ud_filename_ptr);

struct EoSDMultiFuncs {
	dllexport static gnu_noinline void cdecl eosd_multi_setup_patch_init() asm_symbol_rel(codecave:eosd_multi_setup_patch_init);
	dllexport static gnu_noinline void cdecl eosd_multi_setup_patch_init2() asm_symbol_rel(codecave:eosd_multi_setup_patch_init2);
	dllexport static gnu_noinline void cdecl eosd_multi_setup_patch_init3() asm_symbol_rel(codecave:eosd_multi_setup_patch_init3);
	dllexport static gnu_noinline void cdecl eosd_multi_setup_patch_init4() asm_symbol_rel(codecave:eosd_multi_setup_patch_init4);

	EoSDMultiFuncs() {
		//EoSDMultiFuncs::eosd_multi_setup_patch_init();
	}
};

dllexport EoSDMultiFuncs EoSDFuncs;
dllexport gnu_noinline void cdecl EoSDMultiFuncs::eosd_multi_setup_patch_init() {
	HMODULE kernel_handle = th_GetModuleHandleA(kernel32_dll_str);
#ifndef __x86_64__
	GetProcAddressPtr get_proc_address_shim;
	asm(
		"MOVL %1, %[get_proc_address_shim]"
		: asm_arg("=r", get_proc_address_shim)
		: "i"(&th_GetProcAddress)
	);
#else
	GetProcAddressPtr get_proc_address_shim = &th_GetProcAddress;
#endif
	
	OpenFileMappingA_ptr = (OpenFileMappingAPtr)get_proc_address_shim(kernel_handle, OpenFileMappingA_str);
	CreateFileMappingA_ptr = (CreateFileMappingAPtr)get_proc_address_shim(kernel_handle, CreateFileMappingA_str);
	MapViewOfFile_ptr = (MapViewOfFilePtr)get_proc_address_shim(kernel_handle, MapViewOfFile_str);
	UnmapViewOfFile_ptr = (UnmapViewOfFilePtr)get_proc_address_shim(kernel_handle, UnmapViewOfFile_str);
	Sleep_ptr = (SleepPtr)get_proc_address_shim(kernel_handle, Sleep_str);
}


struct EoSDMultiUniqueData {
	dllexport static gnu_noinline void cdecl eosd_multi_setup_patch_post_init() asm_symbol_rel(codecave:eosd_multi_setup_patch_post_init);

	dllexport static gnu_noinline void cdecl eosd_multi_shutdown_patch_exit() asm_symbol_rel(codecave:eosd_multi_shutdown_patch_exit);

	EoSDMultiUniqueData() {
		//EoSDMultiUniqueData::eosd_multi_setup_patch_post_init();
	}

	~EoSDMultiUniqueData() {
		//EoSDMultiUniqueData::eosd_multi_shutdown_patch_exit();
	}
};

dllexport EoSDMultiUniqueData EoSDUniqueData;

dllexport gnu_noinline void cdecl EoSDMultiUniqueData::eosd_multi_setup_patch_post_init() {
	HANDLE shared_mapping;
	uint8_t mapping_created = false;
	if (expect((shared_mapping = OpenFileMappingA_ptr(FILE_MAP_ALL_ACCESS, FALSE, eosd_multi_mapping_name)) == NULL, false)) {
		shared_mapping = CreateFileMappingA_ptr(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(EoSDMultiSharedData), eosd_multi_mapping_name);
		mapping_created = true;
	}
	shared_mapping_handle = shared_mapping;
	auto* shared_data = (EoSDMultiSharedData*)MapViewOfFile_ptr(shared_mapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(EoSDMultiSharedData));
	shared_data_ptr = shared_data;
	auto& next_index = shared_data->next_index;
	uint32_t index_value;
	if (mapping_created) {
		//shared_data->running_count = 1;
		//shared_data->next_index = 0;
		next_index.as_atomic = index_value = 1;
	}
	else {
		instance_index = index_value = next_index.as_atomic.fetch_add(1);
	}
	for (;index_value != 4; index_value = next_index.as_atomic) {
		Sleep_ptr(0);
	}
}



dllexport gnu_noinline void cdecl EoSDMultiFuncs::eosd_multi_setup_patch_init2() {
	HMODULE kernel_handle = th_GetModuleHandleA(kernel32_dll_str);
#ifndef __x86_64__
	GetProcAddressPtr get_proc_address_shim;
	asm(
		"MOVL %1, %[get_proc_address_shim]"
		: asm_arg("=r", get_proc_address_shim)
		: "i"(&th_GetProcAddress)
	);
#else
	GetProcAddressPtr get_proc_address_shim = &th_GetProcAddress;
#endif
	UnmapViewOfFile_ptr = (UnmapViewOfFilePtr)get_proc_address_shim(kernel_handle, UnmapViewOfFile_str);

	HANDLE shared_mapping;
	if (expect((shared_mapping = ((OpenFileMappingAPtr)get_proc_address_shim(kernel_handle, OpenFileMappingA_str))(FILE_MAP_ALL_ACCESS, FALSE, eosd_multi_mapping_name)) == NULL, false)) {
		shared_mapping = ((CreateFileMappingAPtr)get_proc_address_shim(kernel_handle, CreateFileMappingA_str))(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(EoSDMultiSharedData), eosd_multi_mapping_name);
	}
	shared_mapping_handle = shared_mapping;
	HANDLE mapping_open_handle;
	asm("MOV %[shared_mapping], %[mapping_open_handle]":asm_arg("=D", mapping_open_handle) : asm_arg("a", shared_mapping));
	auto* shared_data = (EoSDMultiSharedData*)((MapViewOfFilePtr)get_proc_address_shim(kernel_handle, MapViewOfFile_str))(mapping_open_handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(EoSDMultiSharedData));
	shared_data_ptr = shared_data;
	auto sleep_func = (SleepPtr)get_proc_address_shim(kernel_handle, Sleep_str);
	Sleep_ptr = sleep_func;
	auto& next_index = shared_data->next_index;
	auto mapping_index = instance_index = next_index.as_atomic.fetch_add(1);
	*(vec<uint32_t, 4>*)&logfile_filename = per_index_data_arrayA[mapping_index];
	do {
		sleep_func(0);
	} while (expect(next_index.as_normal < 4, true));
}

/*
dllexport gnu_noinline void* cdecl __load_score_dat(const char* scorefile_name) asm_symbol_rel(0x42B0D9);
dllexport gnu_noinline void* cdecl __load_score_dat(const char* scorefile_name) {
	use_var(scorefile_name);
	assume_all_registers_volatile();
	return (void*)scorefile_name;
}

dllexport gnu_noinline void cdecl __free_scorefile(void* file) asm_symbol_rel(0x42B7DC);
dllexport gnu_noinline void cdecl __free_scorefile(void* file) {
	use_var(file);
	assume_all_registers_volatile();
}

dllexport gnu_noinline void* cdecl load_scorefile_sub(const char* scorefile_name) asm_symbol_abs(codecave:eosd_multi_load_scorefile_sub);
dllexport gnu_noinline void* cdecl load_scorefile_sub(const char* scorefile_name) {
	auto& scorefile_lock = shared_data_ptr->scorefile_lock.as_atomic;
	++scorefile_lock;
	do {
		Sleep_ptr(0);
	} while (scorefile_lock >= 0);
	void*& return_address = *(void**)_AddressOfReturnAddress();
	void* new_return_address;
	switch ((uintptr_t)return_address) {
		case 0x41BCE6: new_return_address = (void*)0x41BD9F; break;
		case 0x43A5CA: new_return_address = (void*)0x43A5F4; break;
		default: unreachable;
	}
	return_address = new_return_address;
	++scorefile_lock;
	while (scorefile_lock != 0) {
		Sleep_ptr(0);
	}
	return GARBAGE_ARG(void*);
}

dllexport gnu_noinline void* cdecl load_scorefile_main(const char* scorefile_name) asm_symbol_abs(codecave:eosd_multi_load_scorefile_main);
dllexport gnu_noinline void* cdecl load_scorefile_main(const char* scorefile_name) {
	auto& scorefile_lock = shared_data_ptr->scorefile_lock.as_atomic;
	while (scorefile_lock != 3) {
		Sleep_ptr(0);
	}
	gnu_attr(musttail) return __load_score_dat(scorefile_name);
}

dllexport gnu_noinline void cdecl free_scorefile(void* file) {
	__free_scorefile(file);
	auto& scorefile_lock = shared_data_ptr->scorefile_lock.as_atomic;
	//scorefile_lock = -scorefile_lock;
	asm("lock negb %[scorefile_lock]" : asm_arg("+m", scorefile_lock));
	do {
		Sleep_ptr(0);
	} while (scorefile_lock != 0);
}
*/

dllexport gnu_noinline void fastcall spawn_children_processes(HMODULE kernel_handle, GetProcAddressPtr get_proc_address_shim) asm_symbol_rel(codecave:eosd_multi_spawn_children_processes);
dllexport gnu_noinline void fastcall spawn_children_processes(HMODULE kernel_handle, GetProcAddressPtr get_proc_address_shim) {
	const char* thcrap_dir = runconfig_thcrap_dir_get();
	const char* runconfig_filename = runconfig_runcfg_fn_get();
	const char* runconfig_game_id = runconfig_game_get();
	size_t thcrap_dir_len = th_strlen(thcrap_dir);
	size_t runconfig_filename_len = th_strlen(runconfig_filename);
	size_t runconfig_game_id_len = th_strlen(runconfig_game_id);
	size_t total_size = thcrap_dir_len + sizeof(thcrap_loader_str) + runconfig_filename_len + runconfig_game_id_len + 6 + sizeof(eosd_multi_title_l_str);
	char* command_line_writer = (char*)th_malloc(total_size);
	char* command_line = command_line_writer;
	*command_line_writer++ = '"';
	th_memcpy(command_line_writer, thcrap_dir, thcrap_dir_len);
	command_line_writer += thcrap_dir_len;
	th_memcpy(command_line_writer, thcrap_loader_str, sizeof(thcrap_loader_str) - 1);
	command_line_writer += sizeof(thcrap_loader_str) - 1;
	*command_line_writer++ = '"';
	*command_line_writer++ = ' ';
	*command_line_writer++ = '"';
	th_memcpy(command_line_writer, runconfig_filename, runconfig_filename_len);
	command_line_writer += runconfig_filename_len;
	*command_line_writer++ = '"';
	*command_line_writer++ = ' ';
	th_memcpy(command_line_writer, runconfig_game_id, runconfig_game_id_len);
	command_line_writer += runconfig_game_id_len;
	auto* create_process_a = (CreateProcessAPtr)get_proc_address_shim(kernel_handle, CreateProcessA_str);
	auto* close_handle = (CloseHandlePtr)get_proc_address_shim(kernel_handle, CloseHandle_str);

	nounroll for (size_t i = 0; i < 3; ++i) {
		th_strcpy(command_line_writer, eosd_multi_title_strs[i]);
		struct {
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
		} combo_info;
		__builtin_memset(&combo_info, 0, sizeof(combo_info));
		combo_info.si.cb = sizeof(STARTUPINFOA);
		create_process_a(NULL, command_line, NULL, NULL, FALSE, 0, NULL, NULL, &combo_info.si, &combo_info.pi);
		close_handle(combo_info.pi.hProcess);
		close_handle(combo_info.pi.hThread);
	}
	th_free(command_line);
}

dllexport gnu_noinline void cdecl EoSDMultiFuncs::eosd_multi_setup_patch_init3() {
	HMODULE kernel_handle = th_GetModuleHandleA(kernel32_dll_str);
#ifndef __x86_64__
	GetProcAddressPtr get_proc_address_shim;
	asm(
		"MOVL %1, %[get_proc_address_shim]"
		: asm_arg("=r", get_proc_address_shim)
		: "i"(&th_GetProcAddress)
	);
#else
	GetProcAddressPtr get_proc_address_shim = &th_GetProcAddress;
#endif
	UnmapViewOfFile_ptr = (UnmapViewOfFilePtr)get_proc_address_shim(kernel_handle, UnmapViewOfFile_str);
	HANDLE shared_mapping;
	if (expect((shared_mapping = ((OpenFileMappingAPtr)get_proc_address_shim(kernel_handle, OpenFileMappingA_str))(FILE_MAP_ALL_ACCESS, FALSE, eosd_multi_mapping_name)) == NULL, false)) {
		shared_mapping = ((CreateFileMappingAPtr)get_proc_address_shim(kernel_handle, CreateFileMappingA_str))(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(EoSDMultiSharedData), eosd_multi_mapping_name);
	}
	auto* shared_data = (EoSDMultiSharedData*)((MapViewOfFilePtr)get_proc_address_shim(kernel_handle, MapViewOfFile_str))(shared_mapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(EoSDMultiSharedData));
	shared_data_ptr = shared_data;
	auto sleep_func = (SleepPtr)get_proc_address_shim(kernel_handle, Sleep_str);
	Sleep_ptr = sleep_func;
	auto& next_index = shared_data->next_index;
	auto mapping_index = instance_index = next_index.as_atomic.fetch_add(1);
	if (expect(!mapping_index, false)) {
		spawn_children_processes(kernel_handle, get_proc_address_shim);
	}
	do {
		sleep_func(0);
	} while (expect(next_index.as_normal < 4, true));
	LoadLibraryAPtr load_library_a_ptr = (LoadLibraryAPtr)get_proc_address_shim(kernel_handle, LoadLibraryA_str);
	nounroll for (size_t i = 0; i < countof(eosd_multi_xinput_lib_names); ++i) {
		if (HMODULE xinput_lib = load_library_a_ptr(eosd_multi_xinput_lib_names[i])) {
			XInputGetState_ptr = (XInputGetStatePtr)get_proc_address_shim(xinput_lib, XInputGetState_str);
			break;
		}
	}
	GetKeyState_ptr = (GetKeyStatePtr)get_proc_address_shim(th_GetModuleHandleA(user32_dll_str), GetKeyState_str);
}

dllexport gnu_noinline void fastcall spawn_children_processes2(HMODULE kernel_handle, GetProcAddressPtr get_proc_address_shim) asm_symbol_rel(codecave:eosd_multi_spawn_children_processes2);
dllexport gnu_noinline void fastcall spawn_children_processes2(HMODULE kernel_handle, GetProcAddressPtr get_proc_address_shim) {
	((CreateDirectoryAPtr)get_proc_address_shim(kernel_handle, CreateDirectoryA_str))(eosd_multi_replay_folder_str, NULL);
	const char* thcrap_dir = runconfig_thcrap_dir_get();
	const char* runconfig_filename = runconfig_runcfg_fn_get();
	const char* runconfig_game_id = runconfig_game_get();
	size_t thcrap_dir_len = th_strlen(thcrap_dir);
	size_t runconfig_filename_len = th_strlen(runconfig_filename);
	size_t runconfig_game_id_len = th_strlen(runconfig_game_id);
	size_t total_size = thcrap_dir_len + sizeof(thcrap_loader_str) + runconfig_filename_len + runconfig_game_id_len + 7;
	char* command_line_writer = (char*)th_malloc(total_size);
	char* command_line = command_line_writer;
	*command_line_writer++ = '"';
	th_memcpy(command_line_writer, thcrap_dir, thcrap_dir_len);
	command_line_writer += thcrap_dir_len;
	th_memcpy(command_line_writer, thcrap_loader_str, sizeof(thcrap_loader_str) - 1);
	command_line_writer += sizeof(thcrap_loader_str) - 1;
	*command_line_writer++ = '"';
	*command_line_writer++ = ' ';
	*command_line_writer++ = '"';
	th_memcpy(command_line_writer, runconfig_filename, runconfig_filename_len);
	command_line_writer += runconfig_filename_len;
	*command_line_writer++ = '"';
	*command_line_writer++ = ' ';
	th_memcpy(command_line_writer, runconfig_game_id, runconfig_game_id_len);
	command_line_writer += runconfig_game_id_len;
	*command_line_writer++ = '\0';
	auto* create_process_a = (CreateProcessAPtr)get_proc_address_shim(kernel_handle, CreateProcessA_str);
	auto* close_handle = (CloseHandlePtr)get_proc_address_shim(kernel_handle, CloseHandle_str);

	nounroll for (size_t i = 0; i < 3; ++i) {
		struct {
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
		} combo_info = {
			.si.cb = sizeof(STARTUPINFOA)
		};
		//__builtin_memset(&combo_info, 0, sizeof(combo_info));
		//combo_info.si.cb = sizeof(STARTUPINFOA);
		create_process_a(NULL, command_line, NULL, NULL, FALSE, 0, NULL, NULL, &combo_info.si, &combo_info.pi);
		close_handle(combo_info.pi.hProcess);
		close_handle(combo_info.pi.hThread);
	}
	th_free(command_line);
}


thcrap_string(eosd_multi_invalid_options_caption_str, "Four of a Kind Option Error");
thcrap_string(eosd_multi_invalid_options_format_str, "Invalid options specified!");
dllexport gnu_noinline noreturn void eosd_multi_invalid_option_protection_patch_init() {
	log_mboxf(eosd_multi_invalid_options_caption_str, MB_OK, eosd_multi_invalid_options_format_str);
	((ExitProcessPtr)th_GetProcAddress(th_GetModuleHandleA(kernel32_dll_str), ExitProcess_str))(EXIT_FAILURE);
}

dllexport gnu_noinline void cdecl EoSDMultiFuncs::eosd_multi_setup_patch_init4() {
	HMODULE kernel_handle = th_GetModuleHandleA(kernel32_dll_str);
#ifndef __x86_64__
	GetProcAddressPtr get_proc_address_shim;
	asm(
		"MOVL %1, %[get_proc_address_shim]"
		: asm_arg("=r", get_proc_address_shim)
		: "i"(&th_GetProcAddress)
	);
#else
	GetProcAddressPtr get_proc_address_shim = &th_GetProcAddress;
#endif
	UnmapViewOfFile_ptr = (UnmapViewOfFilePtr)get_proc_address_shim(kernel_handle, UnmapViewOfFile_str);
	HANDLE shared_mapping;
	if (expect((shared_mapping = ((OpenFileMappingAPtr)get_proc_address_shim(kernel_handle, OpenFileMappingA_str))(FILE_MAP_ALL_ACCESS, FALSE, eosd_multi_mapping_name)) == NULL, false)) {
		shared_mapping = ((CreateFileMappingAPtr)get_proc_address_shim(kernel_handle, CreateFileMappingA_str))(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(EoSDMultiSharedData), eosd_multi_mapping_name);
	}
	auto* shared_data = (EoSDMultiSharedData*)((MapViewOfFilePtr)get_proc_address_shim(kernel_handle, MapViewOfFile_str))(shared_mapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(EoSDMultiSharedData));
	shared_data_ptr = shared_data;
	auto sleep_func = (SleepPtr)get_proc_address_shim(kernel_handle, Sleep_str);
	Sleep_ptr = sleep_func;
	auto& next_index = shared_data->next_index;
	auto mapping_index = instance_index = next_index.as_atomic.fetch_add(1);
	if (expect(!mapping_index, false)) {
		//spawn_children_processes2(kernel_handle, get_proc_address_shim);
		((CreateDirectoryAPtr)get_proc_address_shim(kernel_handle, CreateDirectoryA_str))(eosd_multi_replay_folder_str, NULL);
		const char* thcrap_dir = runconfig_thcrap_dir_get();
		const char* runconfig_filename = runconfig_runcfg_fn_get();
		const char* runconfig_game_id = runconfig_game_get();
		size_t thcrap_dir_len = th_strlen(thcrap_dir);
		size_t runconfig_filename_len = th_strlen(runconfig_filename);
		size_t runconfig_game_id_len = th_strlen(runconfig_game_id);
		size_t total_size = thcrap_dir_len + sizeof(thcrap_loader_str) + runconfig_filename_len + runconfig_game_id_len + 7;
		char* command_line_writer = (char*)th_malloc(total_size);
		char* command_line = command_line_writer;
		*command_line_writer++ = '"';
		th_memcpy(command_line_writer, thcrap_dir, thcrap_dir_len);
		command_line_writer += thcrap_dir_len;
		th_memcpy(command_line_writer, thcrap_loader_str, sizeof(thcrap_loader_str) - 1);
		command_line_writer += sizeof(thcrap_loader_str) - 1;
		*command_line_writer++ = '"';
		*command_line_writer++ = ' ';
		*command_line_writer++ = '"';
		th_memcpy(command_line_writer, runconfig_filename, runconfig_filename_len);
		command_line_writer += runconfig_filename_len;
		*command_line_writer++ = '"';
		*command_line_writer++ = ' ';
		th_memcpy(command_line_writer, runconfig_game_id, runconfig_game_id_len);
		command_line_writer += runconfig_game_id_len;
		*command_line_writer++ = '\0';
		auto* create_process_a = (CreateProcessAPtr)get_proc_address_shim(kernel_handle, CreateProcessA_str);
		auto* close_handle = (CloseHandlePtr)get_proc_address_shim(kernel_handle, CloseHandle_str);

		nounroll for (size_t i = 0; i < 3; ++i) {
			struct {
				STARTUPINFOA si;
				PROCESS_INFORMATION pi;
			} combo_info;
			__builtin_memset(&combo_info, 0, sizeof(combo_info));
			combo_info.si.cb = sizeof(STARTUPINFOA);
			create_process_a(NULL, command_line, NULL, NULL, FALSE, 0, NULL, NULL, &combo_info.si, &combo_info.pi);
			close_handle(combo_info.pi.hProcess);
			close_handle(combo_info.pi.hThread);
		}
		th_free(command_line);
	}
	do {
		sleep_func(0);
	} while (expect(next_index.as_normal < 4, true));
	LoadLibraryAPtr load_library_a_ptr = (LoadLibraryAPtr)get_proc_address_shim(kernel_handle, LoadLibraryA_str);
	nounroll for (size_t i = 0; i < countof(eosd_multi_xinput_lib_names); ++i) {
		if (HMODULE xinput_lib = load_library_a_ptr(eosd_multi_xinput_lib_names[i])) {
			XInputGetState_ptr = (XInputGetStatePtr)get_proc_address_shim(xinput_lib, XInputGetState_str);
			break;
		}
	}
	GetKeyState_ptr = (GetKeyStatePtr)get_proc_address_shim(th_GetModuleHandleA(user32_dll_str), GetKeyState_str);
}

union XInputButtons {
	uint16_t buttons;
	struct {
		uint16_t dpad : 4;
		uint16_t start : 1;
		uint16_t back : 1;
		uint16_t thumbs : 2;
		uint16_t shoulders : 2;
		uint16_t : 2;
		uint16_t face_buttons : 4;
	};
};
union JoyInfoButtons {
	uint32_t buttons;
	struct {
		uint32_t face_buttons : 4;
		uint32_t shoulders : 2;
		uint32_t back : 1;
		uint32_t start : 1;
		uint32_t thumbs : 2;
		uint32_t dpad : 4;
	};
};

dllexport uint32_t joycap_axes_xmin asm_symbol_abs(0x69D784);
dllexport uint32_t joycap_axes_xmax asm_symbol_abs(0x69D788);
dllexport uint32_t joycap_axes_ymin asm_symbol_abs(0x69D78C);
dllexport uint32_t joycap_axes_ymax asm_symbol_abs(0x69D790);

dllexport gnu_noinline uint16_t eosd_multi_fix_joycaps() {
	//*(unaligned vec<uint32_t, 4>*)&joycap_axes_xmin = (vec<uint32_t, 4>){ 0, UINT16_MAX, 0, UINT16_MAX };
	joycap_axes_xmin = 0;
	joycap_axes_xmax = UINT16_MAX;
	joycap_axes_ymin = 0;
	joycap_axes_ymax = UINT16_MAX;
	return 0;
}

dllexport gnu_noinline uint32_t stdcall eosd_multi_process_xinput(DWORD index, JOYINFOEX* joy_info) {
	XINPUT_STATE xinput_state;
	DWORD status;
	for (size_t i = 0; i < 4; ++i) {
		status = XInputGetState_ptr(i, &xinput_state);
		if (expect(status == ERROR_SUCCESS, true)) {
			XInputButtons& x_buttons = (XInputButtons&)xinput_state.Gamepad.wButtons;
			JoyInfoButtons joy_buttons;
			joy_buttons.buttons = 0;
#define copy_button(name) joy_buttons.name = x_buttons.name
			copy_button(face_buttons);
			copy_button(shoulders);
			copy_button(back);
			copy_button(start);
			copy_button(thumbs);
			copy_button(dpad);
			joy_info->dwButtons = joy_buttons.buttons;
			//joy_info->dwXpos = (int32_t)xinput_state.Gamepad.sThumbLX + 32768;
			//joy_info->dwYpos = ~(int32_t)xinput_state.Gamepad.sThumbLY + 32768;
			
			joy_info->dwXpos = (uint16_t)xinput_state.Gamepad.sThumbLX ^ 0x8000;
			joy_info->dwYpos = (uint16_t)xinput_state.Gamepad.sThumbLY ^ 0x7FFF;

			//uint32_t temp = *(uint32_t*)&xinput_state.Gamepad.sThumbLX;
			//temp ^= 0x7FFF8000;

			//*(uint32_t*)&xinput_state.Gamepad.sThumbLX ^= 0x7FFF8000;
			//*(unaligned vec<uint32_t, 2>*)&joy_info->dwXpos = convertvec(*(unaligned vec<uint16_t, 2>*)&xinput_state.Gamepad.sThumbLX, vec<uint32_t, 2>);

			//*(uint32_t*)&xinput_state.Gamepad.sThumbLX ^= 0x7FFF8000;

			//vec<uint16_t, 2> temp = *(unaligned vec<uint16_t, 2>*)&xinput_state.Gamepad.sThumbLX;
			//temp ^= (vec<uint16_t, 2>){ 0x8000, 0x7FFF };
			//*(unaligned vec<uint32_t, 2>*)&joy_info->dwXpos = convertvec(temp, vec<uint32_t, 2>);


			/*vec<int16_t, 2> temp = *(unaligned vec<int16_t, 2>*)&xinput_state.Gamepad.sThumbLX;
			vec<uint16_t, 2> temp2 = convertvec(temp2, vec<uint16_t, 2>);
			temp += 32768;
			temp[1] = ~temp[1];
			vec<uint16_t, 2> temp2 = bitcast(vec<uint16_t, 2>, temp);
			*(unaligned vec<uint32_t, 2>*)&joy_info->dwXpos = convertvec(temp2, vec<uint32_t, 2>);*/


			return 0;
		}
	}
	return status;
}

dllexport gnu_noinline void cdecl EoSDMultiUniqueData::eosd_multi_shutdown_patch_exit() {
	auto* shared_data = shared_data_ptr;
	auto& atomic_index = shared_data->next_index.as_atomic;
	auto index_value = atomic_index.fetch_sub(1);
	do {
		Sleep_ptr(1);
	} while (expect(atomic_index, true));
	UnmapViewOfFile_ptr((void*)shared_data);
	//CloseHandle_ptr(EoSDUniqueData.shared_mapping_handle);
}

}

dllexport wchar_t dll_fn[] = L"yeet";
dllexport char func_name[] = "yeetus";
dllexport int param;
dllexport noreturn void test() {
	HMODULE h = LoadLibraryExW(dll_fn, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (expect(!h, false)) {
		ExitThread(1);
	}
	auto* p = (void(*)(void*))GetProcAddress(h, func_name);
	if (expect(!p, false)) {
		FreeLibraryAndExitThread(h, 2);
	}
	p(&param);
	ExitThread(0);
}

static inline constexpr float_t MAGIC_VALUE_A = 0x1.0p64;
static inline constexpr float_t MAGIC_VALUE_B = 0x1.0p63;

static inline constexpr float_t MAGIC_VALUE_A_ARRAY[] = {
	0.0,
	MAGIC_VALUE_A
};
static inline constexpr float_t MAGIC_VALUE_A_ARRAY2[] = {
	MAGIC_VALUE_A,
	0.0
};

static uint64_t builtin_start_time;
static uint64_t builtin_end_time;
static uint64_t custom1_start_time;
static uint64_t custom1_end_time;
static uint64_t custom2_start_time;
static uint64_t custom2_end_time;
static uint64_t custom3_start_time;
static uint64_t custom3_end_time;
static uint64_t custom4_start_time;
static uint64_t custom4_end_time;
static uint64_t custom5_start_time;
static uint64_t custom5_end_time;
static uint64_t custom6_start_time;
static uint64_t custom6_end_time;

template<typename T>
forceinline bool inc_overflow(T& value) {
	bool ret;
	asm volatile (
		"ADD $1, %[value]"
		: asm_arg("+r", value), asm_flags(c, ret)
	);
	return ret;
}

template<typename T1, typename T2>
forceinline bool add_overflow(T1& value, const T2 add) {
	bool ret;
	asm volatile (
		"ADD %[add], %[value]"
		: asm_arg("+r", value), asm_flags(c, ret)
		: asm_arg("Ker", add)
	);
	return ret;
}

dllexport gnu_noinline void long_double_test() {
	
	constexpr uint64_t loop_iters = 1000000000ULL;
	constexpr uint64_t counter_add = UINT64_MAX / loop_iters;
	
	uint64_t counter;
	
	builtin_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = counter;
		__asm volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	builtin_end_time = rdtsc();

	custom1_start_time = rdtsc();
	counter = 0;
	do {
		/*
		long double ld_value = (int64_t)counter;
		if (counter >> 63) {
			ld_value += MAGIC_VALUE_A;
		}
		__asm volatile ("":"+t"(ld_value));
		*/
		long double ld_value;
		uint64_t yeet = counter;
		__asm volatile (
			"fildq %[yeet] \n"
			//"fldt st \n"
			".byte 0xD9, 0xC0 \n"
			"fadds %[MAGIC_VALUE_A] \n"
			"bt $0x3F, %[counter] \n"
			//"fcmovnb st(1), st \n"
			".byte 0xDB, 0xC1 \n"
			//"fstpt st(1) \n"
			".byte 0xDD, 0xD9 \n"
			: asm_arg("=t", ld_value)
			: asm_arg("m", yeet), asm_arg("r", counter), asm_arg("m", MAGIC_VALUE_A)
		);
	} while (!add_overflow(counter, counter_add));
	custom1_end_time = rdtsc();

	custom2_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		ld_value += MAGIC_VALUE_A_ARRAY[counter >> 63];
		__asm volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom2_end_time = rdtsc();

	custom3_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		//ld_value += MAGIC_VALUE_A_ARRAY[__bextri_u64(counter, 0x13F)];
		__asm volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom3_end_time = rdtsc();

	custom4_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		int64_t yeet;
		__asm volatile (
			"BT $0x3F, %[counter] \n"
			"SBB %[yeet], %[yeet] \n"
			: asm_arg("=r", yeet)
			: asm_arg("r", counter)
		);
		ld_value += MAGIC_VALUE_A_ARRAY2[1 + yeet];
		__asm volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom4_end_time = rdtsc();

	custom5_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		int64_t yeet = counter;
		__asm volatile (
			"ADD %[yeet], %[yeet] \n"
			"SBB %[yeet], %[yeet] \n"
			: asm_arg("+r", yeet)
		);
		ld_value += MAGIC_VALUE_A_ARRAY2[1 + yeet];
		__asm volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom5_end_time = rdtsc();

	custom6_start_time = rdtsc();
	counter = 0;
	do {
		uint64_t yeet = counter;
		long double ld_value;
		__asm volatile (
			"FILDQ %[yeet] \n"
			"TEST %[counter], %[counter] \n"
			"JNS 1f \n"
			"FADDS %[MAGIC_VALUE_A] \n"
		"1: \n"
			: asm_arg("=t", ld_value)
			: asm_arg("m", yeet), asm_arg("r", counter), asm_arg("m", MAGIC_VALUE_A)
		);
	} while (!add_overflow(counter, counter_add));
	custom6_end_time = rdtsc();

	printf(
		"Builtin Time: %llu\n"
		"Custom1 Time: %llu\n"
		"Custom2 Time: %llu\n"
		"Custom3 Time: %llu\n"
		"Custom4 Time: %llu\n"
		"Custom5 Time: %llu\n"
		"Custom6 Time: %llu\n"
		, builtin_end_time - builtin_start_time
		, custom1_end_time - custom1_start_time
		, custom2_end_time - custom2_start_time
		, custom3_end_time - custom3_start_time
		, custom4_end_time - custom4_start_time
		, custom5_end_time - custom5_start_time
		, custom6_end_time - custom6_start_time
	);
}

dllexport void fill_gdts();

extern gnu_noinline void yeetus_memory();

extern void sha256_test(const char* filename);
extern void sha256_test2();

extern "C" {
	extern inline uint32_t get_ebp_test();
}

dllexport uint32_t ebp_test_frame_jank() {
	return *(uint32_t*)(get_ebp_test() + 0xC);
}

using IsWow64ProcessPtrT = decltype(&IsWow64Process);
dllexport HANDLE GetNTDLL64() {
	BOOL is_x64 = false;
	IsWow64Process((HANDLE)-1, &is_x64);
	HANDLE ret = INVALID_HANDLE_VALUE;
	if (is_x64) {
		PVOID old;
		Wow64DisableWow64FsRedirection(&old);
		wchar_t buffer[MAX_PATH];
		GetSystemDirectoryW(buffer, MAX_PATH);
		wcscat(buffer, L"\\ntdll.dll");
		ret = GetModuleHandleW(buffer);
		Wow64RevertWow64FsRedirection(old);
	}
	return ret;
}

dllexport HMODULE GetNTDLL64_2() {
	BOOL is_x64 = false;
	IsWow64Process((HANDLE)-1, &is_x64);
	HMODULE ret = (HMODULE)INVALID_HANDLE_VALUE;
	if (is_x64) {
		if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, *(LPCSTR FS_RELATIVE*)0xC0, &ret)) {
			ret = (HMODULE)INVALID_HANDLE_VALUE;
		}
	}
	return ret;
}

/*
dllexport class BaseClassA {
public:
	int A = rand();
	dllexport gnu_noinline virtual int testA() = 0;
	dllexport gnu_noinline virtual int testB() {
		return printf("%d", rand());
	}
	dllexport gnu_noinline virtual ~BaseClassA() {
		printf("%d", rand());
	}
};
dllexport class BaseClassB {
public:
	int A = rand();
	dllexport gnu_noinline virtual int testA() {
		return rand() * 5;
	}
	dllexport gnu_noinline virtual ~BaseClassB() {
		printf("%d", rand());
	}
};
dllexport class BaseClassC : public BaseClassA, public BaseClassB {
public:
	int A = rand();
	dllexport gnu_noinline int testA() {
		return rand();
	}
	dllexport gnu_noinline virtual ~BaseClassC() {
		printf("%d", rand());
	}
	dllexport gnu_noinline virtual int testB() {
		return rand();
	}
};
dllexport class BaseClassD : public BaseClassB, public BaseClassA {
public:
	int A = rand();
	dllexport gnu_noinline int testA() {
		return rand();
	}
	dllexport gnu_noinline virtual ~BaseClassD() {
		printf("%d", rand());
	}
	dllexport gnu_noinline virtual int testB() {
		return rand();
	}
};

dllexport gnu_noinline int force_vtable(BaseClassB* yeet) {
	return printf("%d", yeet->testA());
}
*/

extern bool bs_dll64();
dllexport void wasted_wtf_is_this();
dllexport void wasted_wtf_is_this2();
dllexport void wasted_wtf_is_this3();
extern void test_json_parse(const char16_t* raw_json);
int main(int argc, char* argv[]) {
	//test_json_parse(u"{ \"ye\\u0065tus\" : true, \"pingas\":{}, \"yeet\": [ 01, 0.1, -123.456e+78, \"\", null ] }");
	/*
	BaseClassC* temp = new BaseClassC();
	printf("%d", temp->testA());
#ifdef _M_IX86
	LoadLibraryA("F:\\My Programs Expansion\\__code_tools\\ClassDumper2\\DX11\\ClassDumper2.dll");
#else
	LoadLibraryA("F:\\My Programs Expansion\\__code_tools\\ClassDumper2\\DX11\\ClassDumper2_64.dll");
#endif
	infinite_loop();
	force_vtable(dynamic_cast<BaseClassB*>(temp));
	*/
	//return 0;

	bs_dll64();
	//run_ultimate_float_reduce_test();
	return 0;
	//goto print_seg_bases;

	//clang_noinline sha256_test("F:/Touhou_Stuff_2/disassembly_stuff/8/eiya - Copy/th08.exe");
	//clang_noinline sha256_test2();
	//fill_gdts();
	//yeetus_memory();
	//return 0;

	extern void dump_ollydbg_instructions32(const wchar_t* path, const char* output_path);
	extern void dump_ollydbg_instructions64(const wchar_t* path, const char* output_path);
	extern void dump_ollydbg_relocs32(const wchar_t* path, const char* output_path);
	dump_ollydbg_instructions32(
		L"F:\\My Programs Expansion\\WindowsTools\\odbg201\\OllyDbg.exe",
		"F:\\My Programs Expansion\\WindowsTools\\odbg201\\instructions_dump.txt"
	);
	dump_ollydbg_instructions64(
		L"F:\\My Programs Expansion\\WindowsTools\\odbg64\\OllyDbgTest.exe",
		"F:\\My Programs Expansion\\WindowsTools\\odbg64\\instructions_dump.txt"
	);
	dump_ollydbg_relocs32(
		L"F:\\My Programs Expansion\\WindowsTools\\odbg201\\OllyDbg.exe",
		"F:\\My Programs Expansion\\WindowsTools\\odbg201\\relocs.txt"
	);
	return 0;

	extern void open_exe_wrapper(const char* path);

	//open_exe_wrapper("F:\\My Programs Expansion\\WindowsTools\\odbg64\\OllyDbgTest.exe");
	open_exe_wrapper("F:\\Touhou_Stuff_2\\disassembly_stuff\\6\\kouma - Copy (1)\\th06.exe");

	extern void string_test();
	extern void string_test_old2();
	extern void string_test_old();
	extern void string_test_older();
	//string_test();

	extern void tts_test();
	extern void tts_test_new();
	//tts_test();
	//tts_test_new();
	//Sleep(3000);
	//long_double_test();
	return 0;

print_seg_bases:
	printf(
		"Operating Mode: %zu bit\n"
		"Instructions Enabled: %s\n"
		"FSBASE: 0x%llX\n"
		"GSBASE: 0x%llX\n"
		, sizeof(void*) * CHAR_BIT
		, bool_str(IsProcessorFeaturePresent(PF_RDWRFSGSBASE_AVAILABLE))
		, (uint64_t)rdfsbase64()
		, (uint64_t)rdgsbase64()
	);
	return 0;

	HWND console_window = GetConsoleWindow();
	size_t i = 0;
	for (;;) {
		RECT position;
		GetWindowRect(console_window, &position);

		if (i & 1) {
			std::swap(position.left, position.right);
			std::swap(position.top, position.bottom);
		}

		int32_t x_positions[2] = { position.left, position.right };
		int32_t y_positions[2] = { position.top, position.bottom };

		int32_t x0 = x_positions[0];
		int32_t smallest_x = x0;
		int32_t x1 = x_positions[1];
		if (x1 < smallest_x) smallest_x = x1;
		++smallest_x;
		uint32_t smallest_x_diff = UINT32_MAX;
		//if ((x0 -= smallest_x) && x0 < smallest_x_diff) smallest_x_diff = x0;
		//if ((x1 -= smallest_x) && x1 < smallest_x_diff) smallest_x_diff = x1;
		if ((uint32_t)(x0 -= smallest_x) < smallest_x_diff) smallest_x_diff = x0;
		if ((uint32_t)(x1 -= smallest_x) < smallest_x_diff) smallest_x_diff = x1;
		x_positions[0] = smallest_x;
		smallest_x += smallest_x_diff;
		x_positions[1] = smallest_x;

		int32_t y0 = y_positions[0];
		int32_t smallest_y = y0;
		int32_t y1 = y_positions[1];
		if (y1 < smallest_y) smallest_y = y1;
		++smallest_y;
		uint32_t smallest_y_diff = UINT32_MAX;
		//if ((y0 -= smallest_y) && y0 < smallest_y_diff) smallest_y_diff = y0;
		//if ((y1 -= smallest_y) && y1 < smallest_y_diff) smallest_y_diff = y1;
		if ((uint32_t)(y0 -= smallest_y) < smallest_y_diff) smallest_y_diff = y0;
		if ((uint32_t)(y1 -= smallest_y) < smallest_y_diff) smallest_y_diff = y1;
		y_positions[0] = smallest_y;
		smallest_y += smallest_y_diff;
		y_positions[1] = smallest_y;

		printf(
			"\t\tTick:\t%zu\n"
			"\t\tX:\t%ld\t%ld\n"
			"\t\tY:\t%ld\t%ld\n"
			"\t\tX2:\t%d\t%d\n"
			"\t\tY2:\t%d\t%d\n"
			, i++
			, position.left, position.right
			, position.top, position.bottom
			, x_positions[0], x_positions[1]
			, y_positions[0], y_positions[1]
		);
		Sleep(2000);
	}

	//size_t count = joyGetNumDevs();

	//for (size_t x = 0; x )
	

	/*for (size_t i = 0; i < countof(eosd_multi_xinput_lib_names); ++i) {
		const char* lib_name = eosd_multi_xinput_lib_names[i];
		if (HMODULE xinput_lib = LoadLibraryA(lib_name)) {
			if (auto* XInputGetState_func = (XInputGetStatePtr)GetProcAddress(xinput_lib, "XInputGetState")) {
				XINPUT_STATE input_state;
				while (true) {
					if (XInputGetState_func(0, &input_state) == ERROR_SUCCESS) {
						printf("XInputGetState succeeded!\n");
						if (GetKeyState(VK_SCROLL) & 1) {
							__builtin_dump_struct(&input_state, &printf);
						}
					} else {
						printf("XInputGetState failed!\n");
					}
					Sleep(1);
				}
			} else {
				printf("XInputGetState not found in %s!\n", lib_name);
			}
		}
		else {
			printf("%s not found!\n", lib_name);
		}
	}*/
	return 0;
}

int pingas() {
#define LOOP_COUNT 1000000
	uint64_t total = 0;
	//for (size_t i = 0; i < LOOP_COUNT; ++i) {
		total += test_loop(1000000);
	//}
	//total /= LOOP_COUNT;
	printf("Average: %llu", total);

	//BST_test();
	return 0;

	HANDLE self_handle = GetCurrentProcess();
	ULONG Iopl = 3;

	extern void AddSeTcbPrivilege(const char* FQname);

	//AddSeTcbPrivilege("zero318");

	return (int)NtSetInformationProcess(self_handle, (PROCESSINFOCLASS)ProcessUserModeIOPL, &Iopl, sizeof(Iopl));
	return 0;
	

	//test_float_func(TWO_PI_f, correct_remainderf3, test_remainderf);
	//test_float_func(TWO_PI_d, correct_remainderf4, test_remainderf2);
	//test_float_func(TWO_PI_f, correct_remainderf3, test_remainderf3);
	test_float_func(TWO_PI_d, correct_remainderf4, test_remainderf4);
	//test_float_func(TWO_PI_f, correct_remainderf2, test_remainderf3);
	return 0;
	

	/*volatile uint16_t segment = 0;
	uint32_t limit = 0;
	uint32_t data = 0;
	uint8_t valid = false;
	size_t valid_count = 0;
	do {
		__asm__(
			"LARL %3, %2 \n"
			"LSLL %3, %0 \n"
			"SETZ %1 \n"
			: "=r"(limit), "=r"(valid), "=r"(data)
			: "r"(segment)
		);
		if (valid) {
			++valid_count;
			LDT_ENTRY ldt;
			valid = GetThreadSelectorEntry(GetCurrentThread(), segment, &ldt);
			uint32_t base = ldt.BaseLow | ldt.HighWord.Bytes.BaseMid << 16 | ldt.HighWord.Bytes.BaseHi << 24;
			uint32_t limit2 = ldt.LimitLow | ldt.HighWord.Bits.LimitHi << 16;
			if (ldt.HighWord.Bits.Granularity) limit2 = limit2 << 12 | 0xFFF;
			printf(
				"Valid Segment:\t%hX\tBase:\t%X\tLimit:\t%X(%X)\t\n"
				, segment, base, limit, limit2
			);
		}
	} while (++segment != 0);
	return 0;*/

	/*const char test_charsA[2][5] = {
		{'A', 'B', 'C', 'A', 'B'},
		{'A', 'B', 'C', 'A', 'B'},
	};
	const char test_charsB[2][5] = {
		{'Z', 'X', 'C', 'A', 'B'},
		{'C', 'A', 'A', 'A', 'A'},
	};

	auto Result1 = parse_letter_array(&test_charsA[0][0], 2, 5, 3);
	auto Result2 = parse_letter_array(&test_charsB[0][0], 2, 5, 3);
	printf(
		"%X\t%X"
		, Result1, Result2
	);
	return 0;*/

	/*BoundingBoxSource src_box;
	src_box.make_from_raw_pos_size(5.0f, 5.0f, 10.0f, 10.0f);
	//src_box.make_from_raw_pos_size(0.0f, 0.0f, 0.0f, 0.0f);

	float min_x = 0.0f;
	float max_x = 10.0f;
	float min_y = 0.0f;
	float max_y = 10.0f;

	printf(
		"%f\t%f\t%f\t%f\n"
		, src_box.box[0], src_box.box[1], src_box.box[2], src_box.box[3]
	);
	size_t test_count = 0;
	size_t intersect_count = 0;
	size_t not_intersect_count = 0;
	size_t correct_intersect_count = 0;
	size_t correct_not_intersect_count = 0;
	for (float XX = -10.0f; XX < 10.0f; XX += 0.001f) {
		for (float YY = -10.0f; YY < 10.0f; YY += 0.001f) {
			BoundingBoxDest dst_box;
			dst_box.make_from_raw_pos_size(XX, YY, 2.0f, 2.0f);
			//dst_box.make_from_raw_pos_size(0.0f, 0.0f, 0.0f, 0.0f);

			float rect_min_x = XX - 1.0f;
			float rect_max_x = XX + 1.0f;
			float rect_min_y = YY - 1.0f;
			float rect_max_y = YY + 1.0f;
			float rect_min_x = 0.0f;
			float rect_max_x = 0.0f;
			float rect_min_y = 0.0f;
			float rect_max_y = 0.0f;
			correct_intersect_count += rect_min_x <= max_x && rect_max_x >= min_x && rect_min_y <= max_y && rect_max_y >= min_y;
			correct_not_intersect_count += rect_min_x > max_x || rect_max_x < min_x || rect_min_y > max_y || rect_max_y < min_y;
			//correct_intersect_count += rect_min_x < max_x && rect_max_x > min_x && rect_min_y < max_y && rect_max_y > min_y;
			//correct_not_intersect_count += rect_min_x >= max_x || rect_max_x <= min_x || rect_min_y >= max_y || rect_max_y <= min_y;

			++test_count;
			bool intersect = src_box.test_intersect(dst_box);
			intersect_count += intersect;
			bool not_intersect = src_box.test_not_intersect(dst_box);
			not_intersect_count += not_intersect;
			printf(
				"%f\t%f\t%f\t%f\t%s\t%s\n"
				, dst_box.box[0], dst_box.box[1], dst_box.box[2], dst_box.box[3]
				, BoolStr(intersect)
				, BoolStr(not_intersect)
			);
			
		}
	}
	printf(
		"Total tests:\t\t%zu\n"
		"Intersects:\t\t%zu/%zu\n"
		"Not Intersects:\t\t%zu/%zu\n"
		"Sum Success:\t\t%s\n"
		"Match Success:\t\t%s\n"
		, test_count
		, intersect_count, correct_intersect_count
		, not_intersect_count, correct_not_intersect_count
		, BoolStr(test_count == intersect_count + not_intersect_count)
		, BoolStr(intersect_count == correct_intersect_count && not_intersect_count == correct_not_intersect_count)
	);
	return 1;*/


	for (float ii = -100.0f; ii < 100.0f; ++ii) {
		float correct_reduce = reduce_angle_correct(ii);
		float old_reduce = reduce_angle_old(ii);
		float new_reduce = reduce_angle_cheese(ii);
		float newer_reduce = reduce_angle_new(ii);
		printf("%f\t%f\t%f\t%f\t%f\n", ii, correct_reduce, old_reduce, new_reduce, newer_reduce);
	}
	return 0;

	bool prev_correct = false;
	size_t correct_span = 0;
	float recent_correct_start;
	size_t longest_correct_span = 0;
	float longest_correct_start = 0.0f;
	float longest_correct_end = 0.0f;

	__v4si value = { (int)0xFF7FFFFF, 0, 0, 0 };
	__v4si inc_dec = { 1, 0, 0, 0 };
	__v4sf current_vec;
	//__v4sf previous_vec = bitcast(__v4sf, value);
	float current_val;
	//float previous_val;
	//size_t success = 0;
	//size_t total = 0;

	auto lambda_func = [&](float angle) {
		float correct_reduce = reduce_angle_correct(angle);
		//float old_reduce = reduce_angle_old(angle);
		float new_reduce = reduce_angle_cheese(angle);
		//float old_diff = __max(correct_reduce, old_reduce) - __min(correct_reduce, old_reduce);
		float new_diff = __max(correct_reduce, new_reduce) - __min(correct_reduce, new_reduce);
		//float diff_diff = __max(new_reduce, old_reduce) - __min(new_reduce, old_reduce);
		if (new_diff < 0.00001f) {
			++correct_span;
			if (!prev_correct) {
				prev_correct = true;
				recent_correct_start = angle;
			}
		} else if (prev_correct) {
			prev_correct = false;
			if (correct_span > longest_correct_span) {
				longest_correct_span = correct_span;
				longest_correct_start = recent_correct_start;
				longest_correct_end = angle;
			}
			correct_span = 0;
		}
	};

	/*for (float ii = -100.0f; ii < 100.0f; ++ii) {
		float correct_reduce = reduce_angle_correct(ii);
		float old_reduce = reduce_angle_old(ii);
		float new_reduce = reduce_angle4(ii);
		printf("%f\t%f\t%f\t%f\n", ii, correct_reduce, old_reduce, new_reduce);
	}
	return 0;*/

	while (bitcast<__v4sf>(value)[0]) {
		value -= inc_dec;
		current_vec = bitcast<__v4sf>(value);
		current_val = current_vec[0];

		// Negative floats
		lambda_func(current_val);

		/*previous_val = previous_vec[0];
		success += current_val > previous_val;
		previous_vec = current_vec;
		++total;*/
	};
	//previous_vec = current_vec;
	value = { 0, 0, 0, 0 };
	current_vec = bitcast<__v4sf>(value);
	current_val = current_vec[0];

	// Zero
	lambda_func(current_val);

	/*previous_val = previous_vec[0];
	success += current_val == previous_val;
	++total;
	previous_vec = current_vec;*/

	float max = bitcast<float>(0x7F800000);
	while (bitcast<__v4sf>(value)[0] != max) {
		value += inc_dec;
		current_vec = bitcast<__v4sf>(value);
		current_val = current_vec[0];

		// Positive floats
		lambda_func(current_val);

		/*previous_val = previous_vec[0];
		success += current_val > previous_val;
		previous_vec = current_vec;
		++total;*/
	}
	/*printf(
		"Float jank test\n"
		"Valid:\t4278190080\n"
		"Total:\t%u\n"
		"Pass:\t%u\n"
		, total
		, success
	);*/
	printf(
		"\n"
		"Reduce Angle Test\n"
		"Longest Correct Span:\n"
		"Length:\t%zu\n"
		"Start:\t%f\n"
		"End:\t%f\n"
		, longest_correct_span
		, longest_correct_start
		, longest_correct_end
	);
	return 0;
}

//uint8_t* SomeGlobalStaticBufferThatMakesMeSad = 0x4D2D5C;
//#define SomeGlobalStaticBufferThatMakesMeSad ((uint8_t*)0x4D2D5C)
/*
#ifdef _M_IX86
extern "C" {
	extern uint8_t SomeGlobalStaticBufferThatMakesMeSad[0x7FFF];
}

__declspec(dllexport) void* encryption_thing(
	uint8_t* compressed_data,
	int32_t compressed_size,
	uint8_t* output,
	int32_t decompressed_size
) {
	uint8_t trash_counter = 0b10000000; // 0x80
	int32_t sum = 0; // Yes, it calculates this and then does *absolutely nothing* with it
	bool output_is_locally_allocated = false;
	if (compressed_size <= 0 || decompressed_size <= 0) {
		//goto EarlyRet;
	}
	if (!output) {
		output = (uint8_t*)malloc(decompressed_size);
		if (!output) {
			//goto EarlyRet;
		}
		output_is_locally_allocated = true;
	}
	uint8_t* current_in_data = compressed_data;
	uint8_t* current_out_data = output;
	int32_t global_buffer_index = 1;

	uint8_t data;
	auto decrypt_thing_idk = [&](uint32_t local_trash_counter_value) {
		int32_t local_trash_counter = local_trash_counter_value; // 0x4000
		int32_t ret = 0;
#pragma clang loop unroll(disable)
		do {
			if (trash_counter == 0b10000000) { // 0x80
				data = *current_in_data;
				if ((current_in_data - compressed_data) >= compressed_size) {
					data = 0;
				} else {
					++current_in_data;
				}
				sum += data;
			}
			if (trash_counter & data) {
				ret |= local_trash_counter;
			}
			local_trash_counter >>= 1;
			if (!(trash_counter >>= 1)) {
				trash_counter = 0b10000000; // 0x80
			}
		} while (local_trash_counter);
		return ret;
	};

#pragma clang loop unroll(disable)
	while (1) {
#pragma clang loop unroll(disable)
		while (1) {
			if (trash_counter == 0b10000000) { // 0x80
				data = *current_in_data;
				if ((current_in_data - compressed_data) >= compressed_size) {
					data = 0;
				} else {
					++current_in_data;
				}
				sum += data;
			}
			data &= trash_counter; // I don't even know
			if (!(trash_counter >>= 1)) {
				trash_counter = 0b10000000; // 0x80
			}
			if (!data) {
				break;
			}
			if ((current_out_data - output) >= decompressed_size) {
				//goto Error;
			}
			uint8_t mask_out_A = decrypt_thing_idk(0b10000000);
			*current_out_data++ = mask_out_A;
			SomeGlobalStaticBufferThatMakesMeSad[global_buffer_index] = mask_out_A;
			global_buffer_index = (global_buffer_index + 1) & 0x7FFF;
		}
		int32_t mask_out_B = decrypt_thing_idk(0b100000000000000); // 0x4000
		if (!mask_out_B) {
			break;
		}
		int32_t mask_out_C = decrypt_thing_idk(0b1000); // 0x8
		mask_out_C += 2;
		if (mask_out_C > 0) {
			int32_t i = 0;
#pragma clang loop unroll(disable)
			do {
				uint8_t data = SomeGlobalStaticBufferThatMakesMeSad[(mask_out_B + i) & 0x7FFF];
				if ((current_out_data - output) >= decompressed_size) {
					//goto Error;
				}
				*output++ = data;
				SomeGlobalStaticBufferThatMakesMeSad[global_buffer_index] = data;
				global_buffer_index = (global_buffer_index + 1) & 0x7FFF;
			} while (++i <= mask_out_C);
		}
	}
	// trash_counter must equal 0b10000000 or 0 to break the loop
	while (trash_counter != 0b10000000 && (trash_counter >>= 1));
	uint8_t* original_out_data = output;
	int32_t original_out_size = decompressed_size;
	if ((current_out_data - original_out_data) != original_out_size) {
		goto Error;
	}
	return output;
Error:
	__noop("err in archive");
	if (output_is_locally_allocated && output != NULL) {
		free(output);
	}
EarlyRet:
	return 0;
}
#endif
*/

typedef void json_t;

dllexport gnu_noinline json_t* cdecl json_object_get(const json_t *object, const char *key) {
	use_variable(object);
	use_variable(key);
	assume_all_registers_volatile();
	return NULL;
}

struct x86_reg_t {
	uint32_t eflags;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t retaddr;
};

typedef int(__cdecl *BreakpointFunc_t)(x86_reg_t *regs, void *bp_info);

struct ModuleData {
	void* base_addr;
	size_t size;
	char* name;
	void* json_data;
};

struct ModuleDataHolder {
	ModuleData* data_array;
	size_t array_size;
};

static ModuleDataHolder MODULE_DATA = { 0,0 };

dllexport gnu_noinline auto thcrap_realloc(void* mem, size_t size) asm("{[th_realloc]}");
dllexport gnu_noinline auto thcrap_realloc(void* mem, size_t size) {
	return realloc(mem, size);
}

dllexport gnu_noinline auto thcrap_strdup(const char* str) asm("{[th_strdup]}");
dllexport gnu_noinline auto thcrap_strdup(const char* str) {
	return strdup(str);
}

extern "C" {

	dllexport int cdecl breakpoint_load_test(x86_reg_t *regs, void *bp_info) asm("breakpoint_load_test");
	dllexport int cdecl breakpoint_load_test(x86_reg_t *regs, void *bp_info) {
		size_t next_index;
		size_t byte_offset = MODULE_DATA.array_size;
		ModuleData* new_module = MODULE_DATA.data_array = (ModuleData*)thcrap_realloc(
			MODULE_DATA.data_array,
			MODULE_DATA.array_size += sizeof(ModuleData)
		);
		new_module = (ModuleData*)((uintptr_t)new_module + byte_offset);
		new_module->base_addr = (void*)regs->esi;
		new_module->size = *(size_t*)(regs->ebx + regs->edi + 0x50);
		new_module->json_data = bp_info;
		const char* module_name = thcrap_strdup((char*)(regs->ebp + 0x14));
		new_module->name = (char*)module_name;
		if (json_t* module_stage = json_object_get(bp_info, module_name)) {

		}
		return 1;
	}

	dllexport void cdecl module_data_patch_exit(void*) asm("module_data_patch_exit");
	dllexport void cdecl module_data_patch_exit(void*) {
		ModuleData* data_array = MODULE_DATA.data_array;
		for (
			size_t array_count = MODULE_DATA.array_size / sizeof(ModuleData);
			array_count;
			--array_count, ++data_array
		) {
			free(data_array->name);
		}
		free(data_array);
	}

}

extern "C" {
	static SRWLOCK srwl = SRWLOCK_INIT;

	dllexport void stdcall AcquireSRWLExclusive(SRWLOCK* SRWLock) {
		AcquireSRWLockExclusive(SRWLock);
	}
	dllexport void stdcall AcquireSRWLShared(SRWLOCK* SRWLock) {
		AcquireSRWLockShared(SRWLock);
	}
	dllexport void stdcall InitializeSRWL(SRWLOCK* SRWLock) {
		InitializeSRWLock(SRWLock);
	}
	dllexport void stdcall ReleaseSRWLExclusive(SRWLOCK* SRWLock) {
		ReleaseSRWLockExclusive(SRWLock);
	}
	dllexport void stdcall ReleaseSRWLShared(SRWLOCK* SRWLock) {
		ReleaseSRWLockShared(SRWLock);
	}
	dllexport BOOLEAN stdcall TryAcquireSRWLExclusive(SRWLOCK* SRWLock) {
		return TryAcquireSRWLockExclusive(SRWLock);
	}
	dllexport BOOLEAN stdcall TryAcquireSRWLShard(SRWLOCK* SRWLock) {
		return TryAcquireSRWLockShared(SRWLock);
	}
}

//#include <stdatomic.h>
//#include "zero/custom_intrin.h"
#define atomic_signal_fence ____atomic_signal_fence
#define atomic_thread_fence ____atomic_thread_fence
#include <stdatomic.h>
#undef atomic_signal_fence
#undef atomic_thread_fence

#define SRWLockSpinCount 1024

struct alignas(16) SyncItem {
	SyncItem *restrict back;
	SyncItem *restrict notify;
	SyncItem *restrict next;
	union {
		uintptr_t value;
		struct {
			uintptr_t flags : 4;
			uintptr_t share_count : 28;
		};
		struct {
			uint8_t flag1 : 1;
			uint8_t flag2 : 1;
			uint8_t optimize : 1;
		};
	};
};

union TestLockBits {
	uintptr_t value;
	struct {
		uint8_t busy : 1; // 0b00001
		uint8_t already_waiting : 1; // 0b00010
		uint8_t released : 1; // 0b00100
		uint8_t mixed : 1; // 0b01000
		uint8_t shared_only : 1; // 0b10000
	};
	struct {
		uintptr_t : 4;
		uintptr_t share_count : 28;
	};
};
#define EXTRACT_ADDR(s) ((s) & (~ 0xf))

union TestLock {
	_Atomic uintptr_t value;
	uintptr_t non_atomic_value;
};

/*
extern
//[[clang::weak_import]]
__kernel_entry NTSTATUS
NTAPI
NtWaitForKeyedEvent(
	IN HANDLE               KeyedEventHandle,
	IN PVOID                Key,
	IN BOOLEAN              Alertable,
	IN PLARGE_INTEGER       Timeout OPTIONAL) asm("_NtWaitForKeyedEvent@16");
*/
//NTSYSAPI
//[[clang::weak_import]]
HANDLE GlobalKeyedEventHandle asm("GlobalKeyedEventHandle");

extern "C" {
	//static bool has_multiple_cores = *(DWORD*)(*(DWORD*)(*(DWORD FS_RELATIVE*)(0x18) + 0x30) + 0x64) != 1;
	static bool has_multiple_cores = true;
	inline void backoff(size_t* count) {
		size_t local_count = *count;
		if (local_count) {
			if (local_count < 0x1FFF) {
				local_count *= 2;
				*count = local_count;
			}
		} else if (expect(has_multiple_cores, true)) {
			local_count = 64;
			*count = local_count;
		} else {
			return;
		}
		local_count += (local_count - 1) & (size_t)_rdtsc();
		for (size_t i = 0; i < local_count; ++i) {
			_mm_pause();
			//RtlCaptureContext()
		}
	}

	gnu_noinline void OptimizeLockList(TestLock* test_lock, TestLockBits new_lock) {

	}

	dllexport void stdcall InitializeTestLock(TestLock* test_lock) {
		test_lock->non_atomic_value = 0;
	}
	dllexport void stdcall AcquireTestLockExclusive(TestLock* test_lock) {
		// OR variant
		//TestLockBits prev_lock = { atomic_fetch_or(&test_lock->value, 1) };

		// CMPXCHG variant
		//TestLockBits prev_lock = { test_lock->value };
		//TestLockBits new_lock = { prev_lock.value | 1 };
		//prev_lock.value = atomic_compare_exchange_weak(&test_lock->value, &prev_lock.value, new_lock.value);

		TestLockBits prev_lock = { test_lock->non_atomic_value };
		TestLockBits new_lock = prev_lock;
		new_lock.busy = true;
		atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value);
		if (expect(prev_lock.busy, false)) {
		//if (expect(prev_lock.value != 0, false)) {
			//TestLockBits new_lock;
			size_t backoff_delay = 0;
			SyncItem sync;
			while (1) {
				if (prev_lock.busy) {
					sync.flags = 3;
					sync.next = NULL;
					// if (RtlpWaitCouldDeadlock()) // oh crap, idk
					new_lock.value = (uintptr_t)&sync + 0b11;
					if (prev_lock.already_waiting) {
						sync.notify = NULL;
						sync.share_count = 0;
						sync.back = (SyncItem*)EXTRACT_ADDR(prev_lock.value);
						new_lock.mixed = prev_lock.mixed;
						new_lock.released = true;
						if (!prev_lock.released) {
							sync.optimize = true;
						}
					} else {
						sync.notify = &sync;
						new_lock.mixed = (sync.share_count = prev_lock.share_count) > 1;
					}
					if (!atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value)) {
						backoff(&backoff_delay);
						continue;
					}
					if (sync.optimize) {
						OptimizeLockList(test_lock, new_lock);
					}
					for (size_t i = SRWLockSpinCount; i > 0; --i) {
						//flag (bit1) can be reset by release-lock operation in other thread
						if (!sync.flag2) {
							break;
						}
						_mm_pause();
					}
					if (!sync.flag2) {
						//NtWaitForKeyedEvent(GlobalKeyedEventHandle, &sync, false, NULL);
					}
				} else {
					if (atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, prev_lock.value + 1)) {
						return;
					}
					backoff(&backoff_delay);
					prev_lock.value = test_lock->value;
				}
			}
			
		}
	}
	dllexport void stdcall AcquireTestLockShared(TestLock* test_lock) {
		TestLockBits prev_lock = { .value = 0 };
		TestLockBits new_lock = prev_lock;
		new_lock.busy = true;
		new_lock.shared_only = true;
		atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value);
	}

	gnu_noinline void WakeupTestLock(TestLock* test_lock, TestLockBits new_lock) {

	}

	static __forceinline bool bit_test_reset32(uintptr_t& value, const uintptr_t bit_offset) {
		int carry_flag;
		__asm__(
			"BTR %[bit_offset], %[value]"
			: asm_arg("+r", value), asm_flags(c, carry_flag)
			: asm_arg("I", bit_offset)
		);
		return carry_flag;
	}

	dllexport void stdcall ReleaseTestLockExclusive(TestLock* test_lock) {
		TestLockBits prev_lock = { atomic_fetch_add(&test_lock->value, -1) };
		//if (expect(bit_test_reset32(prev_lock.value, 0), false)) {
			__ud2(); // halt and catch fire becuase the lock broke
		//}
		if (expect(prev_lock.already_waiting && !prev_lock.released, false)) {
			TestLockBits new_lock = prev_lock;
			new_lock.released = true;
			if (expect(atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value), true)) {
				WakeupTestLock(test_lock, new_lock);
			}
		}
	}
	dllexport void stdcall ReleaseTestLockShared(TestLock* test_lock) {
		TestLockBits new_lock = { .value = 0 };
		TestLockBits prev_lock = new_lock;
		prev_lock.busy = true;
		prev_lock.shared_only = true;
		if (expect_chance(!atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value), true, 0.4)) {
			while (1) {
				if (prev_lock.already_waiting) {
					if (prev_lock.mixed) {

					}
					while (1) {
						new_lock = prev_lock;
						new_lock.busy = false;
						new_lock.mixed = false;
						if (prev_lock.released) {
							if (atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value)) {
								return;
							}
						} else {
							new_lock.released = true;
							if (atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value)) {
								WakeupTestLock(test_lock, new_lock);
								return;
							}
						}
					}
				}
			}
		}
	}

	dllexport BOOLEAN stdcall TryAcquireTestLockExclusive(TestLock* test_lock) {
		return !_interlockedbittestandset((volatile long*)&test_lock->value, 0);

		//TestLockBits prev_lock = { test_lock->non_atomic_value };
		//TestLockBits new_lock = prev_lock;
		//new_lock.busy = true;
		//atomic_compare_exchange_strong(&test_lock->value, &prev_lock.value, new_lock.value);

		//uint8_t prev_value = atomic_exchange(&test_lock->value, 1);
		//uint8_t prev_value = atomic_fetch_or(&test_lock->value, 1);
		//prev_value ? 0 : prev
		//return 
		//return !atomic_flag_test_and_set(&test_lock->lock_flag);
	}
}

dllexport void test_while1(size_t count) {
	uint64_t start_time = rdtsc();

	uint64_t first = 0;
	if (count) {
		size_t start_count = count;
		uint64_t second = 1;
		do {
			__asm__(
				"XADD %[first], %[second]"
				: asm_arg("+r", first), asm_arg("+r", second)
			);
		} while (--count);
		count = start_count;
	}
	uint64_t end_time = rdtsc();
	printf("Number[%zu]: %llu in %llu", count, first, end_time - start_time);
}
dllexport void test_while2(size_t count) {
	size_t start_count = count;
	uint64_t start_time = rdtsc();

	uint64_t first = 0, second = 1;
	while (count--) {
		__asm__(
			"XADD %[first], %[second]"
			: asm_arg("+r", first), asm_arg("+r", second)
		);
	}
	uint64_t end_time = rdtsc();
	printf("Number[%zu]: %llu in %llu", start_count, first, end_time - start_time);
}


static char text_buffer[65] = {};

dllexport gnu_noinline uint64_t test_loop(size_t count) {
	uint64_t start_time = rdtsc();

	uint64_t first;
	__asm__ goto (
		"XOR %k[first], %k[first] \n"
		JPCXZ" %l[skip_loop]"
		: asm_arg("=r", first)
		: asm_arg("c", count)
		:
		: skip_loop
	);
	{
		size_t start_count = count;
		uint64_t second = 1;
		__asm__ volatile (
			".align 16 \n"
			"loop_label: \n"

			"XADD %[first], %[second] \n"

			//"XCHG %[first], %[second] \n"
			//"ADD %[first], %[second] \n"

			"LOOP loop_label"
			: asm_arg("+r", first), asm_arg("+r", second), asm_arg("+c", count)
		);
		count = start_count;
	}
skip_loop:
	uint64_t end_time = rdtsc() - start_time;
	//printf("Number[%zu]: %llu in %llu", count, first, end_time);
	return end_time;
}

bool save_area_is_xsave;
size_t save_area_size;
dllexport void calculate_save_format() {
	uint32_t dummy;
	uint32_t max_standard_page;
	uint32_t features;
	get_cpuid(0, max_standard_page, dummy, dummy, dummy);
	if ((save_area_is_xsave = (expect(max_standard_page >= 0xD, true) &&
							  (get_cpuid(1, dummy, dummy, features, dummy), expect(~features & 0xC000000, true))))
	) {
		uint32_t max_xsave_size;
		get_cpuid_ex(0xD, 0, dummy, dummy, max_xsave_size, dummy);
		save_area_size = max_xsave_size;
	} else {
		save_area_size = 512;
	}
}

#ifdef _M_IX86
struct x86_reg_t2 {
	uint32_t eflags;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t ret_addr;
};
#else
struct x86_reg_t2 {
	uint64_t rflags;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rbp;
	uint64_t rsp;
	uint64_t rbx;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rax;
	uint64_t ret_addr;
};
#endif
union fpu_reg {
	long double as_fp;
	uint8_t raw_bytes[16];
};
struct fenv_area_t {
	uint16_t cw;
	uint16_t paddingA;
	uint16_t sw;
	uint16_t paddingB;
	uint16_t tw;
	uint16_t paddingC;
	uint32_t fip;
	uint16_t fcs;
	uint16_t fop;
	uint32_t fdp;
	uint16_t fds;
	uint16_t paddingD;
};
struct alignas(64) fxsave_area_t {
	uint16_t fcw;
	uint16_t fsw;
	uint8_t ftw;
	uint8_t paddingA;
	uint16_t fop;
#ifndef _M_IX86
	uint64_t fip;
	uint64_t fdp;
#else
	uint32_t fip;
	uint16_t fcs;
	uint16_t paddingB;
	uint32_t fdp;
	uint16_t fds;
	uint16_t paddingC;
#endif
	uint32_t mxcsr;
	uint32_t mxcsr_mask;
	fpu_reg st0;
	fpu_reg st1;
	fpu_reg st2;
	fpu_reg st3;
	fpu_reg st4;
	fpu_reg st5;
	fpu_reg st6;
	fpu_reg st7;
	uint8_t xmm0[16];
	uint8_t xmm1[16];
	uint8_t xmm2[16];
	uint8_t xmm3[16];
	uint8_t xmm4[16];
	uint8_t xmm5[16];
	uint8_t xmm6[16];
	uint8_t xmm7[16];
#ifndef _M_IX86
	uint8_t xmm8[16];
	uint8_t xmm9[16];
	uint8_t xmm10[16];
	uint8_t xmm11[16];
	uint8_t xmm12[16];
	uint8_t xmm13[16];
	uint8_t xmm14[16];
	uint8_t xmm15[16];
#else
	uint8_t paddingL[128];
#endif
	uint8_t reserved[48];
	union {
		struct {
			uint16_t cw;
			uint16_t paddingM;
			uint16_t sw;
			uint16_t paddingN;
			uint16_t tw;
			uint16_t paddingO;
			uint32_t fip;
			uint16_t fcs;
			uint16_t fop;
			uint32_t fdp;
			uint16_t fds;
			uint16_t paddingP;
		} original_fenv;
#ifndef _M_IX86
		struct {
			uint16_t paddingQ[6];
			uint64_t fs_base;
			uint64_t gs_base;
		};
#endif
	};
	uint32_t temp_mxcsr;
	uint16_t es;
	uint16_t cs;
	uint16_t ss;
	uint16_t ds;
	uint16_t fs;
	uint16_t gs;
	uint8_t pingas[];
};
static_assert(sizeof(fxsave_area_t) <= 512);

//#define AlignUpToMultipleOf2(val, mul) (((val) + (mul) - 1) & -(mul))

dllexport gnu_noinline gnu_attr(noduplicate) size_t cdecl breakpoint_process(void* bp, size_t addr_index, x86_reg_t* regs) {
	use_var(bp);
	use_var(addr_index);
	use_var(regs);
	assume_all_registers_volatile();
	return rand();
}

dllexport void bp_entry() {
	asm(
		//"pusha \n"
		"pushf \n"
	);
	clear_dir_flag();
	x86_reg_t* regs = (x86_reg_t*)rsp_reg;

}

dllexport void test_wrfsbase() {
	uint64_t addr = rdfsbase64();
	printf("%llu", addr);
}

//#include <wdm.h>
#include <winternl.h>
#include <AclAPI.h>

#define NtCurrentProcess() ( (HANDLE)(LONG_PTR) -1 )  

extern
NTSTATUS
NTAPI
NtOpenSection(
	__out PHANDLE SectionHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes
);

typedef enum _SECTION_INHERIT {
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT;

extern
NTSTATUS
NTAPI
NtMapViewOfSection(
	__in HANDLE SectionHandle,
	__in HANDLE ProcessHandle,
	__inout PVOID *BaseAddress,
	__in ULONG_PTR ZeroBits,
	__in SIZE_T CommitSize,
	__inout_opt PLARGE_INTEGER SectionOffset,
	__inout PSIZE_T ViewSize,
	__in SECTION_INHERIT InheritDisposition,
	__in ULONG AllocationType,
	__in ULONG Win32Protect
);

extern
NTSTATUS
NTAPI
NtUnmapViewOfSection(
	__in HANDLE ProcessHandle,
	__in_opt PVOID BaseAddress
);

extern
VOID
NTAPI
RtlInitUnicodeString(
	__out PUNICODE_STRING DestinationString,
	__in_z_opt __drv_aliasesMem PCWSTR SourceString
);

#define PAGE_SIZE 0x1000

struct GDTR {
	uint16_t limit gnu_packed;
	uint64_t base gnu_packed;
} gnu_packed;
ValidateFieldOffset(0x0, GDTR, limit);
ValidateFieldOffset(0x2, GDTR, base);

gnu_noinline void yeetus_memory() {
	
	GDTR GDT_data;
	__asm__ volatile(
		"SGDT %[GDT_data]"
		: asm_arg("=m", GDT_data)
	);

	HMODULE ntdll_handle = GetModuleHandleA("ntdll.dll");
	auto NtOpenSection_func = (decltype(&NtOpenSection))GetProcAddress(ntdll_handle, "NtOpenSection");
	
	UNICODE_STRING device_name;
	auto RtlInitUnicodeString_func = (decltype(&RtlInitUnicodeString))GetProcAddress(ntdll_handle, "RtlInitUnicodeString");
	RtlInitUnicodeString_func(&device_name, L"\\Device\\PhysicalMemory");
	HANDLE section_handle;
	OBJECT_ATTRIBUTES object_attributes;
	InitializeObjectAttributes(&object_attributes, &device_name, OBJ_CASE_INSENSITIVE, NULL, NULL);
	NTSTATUS status = NtOpenSection_func(&section_handle, WRITE_DAC | READ_CONTROL, &object_attributes);
	if (NT_SUCCESS(status)) {


		EXPLICIT_ACCESS access{};
		PACL old_dacl = NULL;
		PVOID security;
		GetSecurityInfo(section_handle, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &old_dacl, NULL, &security);

		access.grfAccessPermissions = SECTION_ALL_ACCESS;
		access.grfAccessMode = GRANT_ACCESS;
		access.grfInheritance = NO_INHERITANCE;
		access.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
		access.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
		access.Trustee.TrusteeType = TRUSTEE_IS_USER;
		access.Trustee.ptstrName = "CURRENT_USER";

		PACL new_dacl = NULL;
		SetEntriesInAcl(1, &access, old_dacl, &new_dacl);

		SetSecurityInfo(section_handle, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, new_dacl, NULL);

		auto NtClose_func = (decltype(&NtClose))GetProcAddress(ntdll_handle, "NtClose");
		NtClose_func(section_handle);

		if (NT_SUCCESS(NtOpenSection_func(&section_handle, SECTION_MAP_READ | SECTION_MAP_WRITE, &object_attributes))) {
			auto NtMapViewOfSection_func = (decltype(&NtMapViewOfSection))GetProcAddress(ntdll_handle, "NtMapViewOfSection");
			PVOID mapped_address = NULL;
			LARGE_INTEGER mapped_offset;
			mapped_offset.QuadPart = GDT_data.base;
			SIZE_T mapped_size;

			auto NtUnmapViewOfSection_func = (decltype(&NtUnmapViewOfSection))GetProcAddress(ntdll_handle, "NtUnmapViewOfSection");
			
			if (NT_SUCCESS(NtMapViewOfSection_func(section_handle, NtCurrentProcess(), &mapped_address, 0, PAGE_SIZE, &mapped_offset, &mapped_size, ViewShare, 0, PAGE_READONLY))) {
				NtUnmapViewOfSection_func(NtCurrentProcess(), mapped_address);
			}
			NtClose_func(section_handle);
		}
	}
}

// size: 
struct alignas(8) KGDTENTRY {
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
#pragma region // KGDTENTRY Validation
ValidateFieldOffset(0x0, KGDTENTRY, LimitLow);
ValidateFieldOffset(0x2, KGDTENTRY, BaseLow);
ValidateFieldOffset(0x4, KGDTENTRY, BaseMiddle);
ValidateFieldOffset(0x5, KGDTENTRY, Flags1);
ValidateFieldOffset(0x6, KGDTENTRY, Flags2);
ValidateFieldOffset(0x7, KGDTENTRY, BaseHigh);
ValidateStructAlignment(0x8, KGDTENTRY);
#pragma endregion

// size: 
struct alignas(16) KGDTENTRY64 {
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
#pragma region // KGDTENTRY64 Validation
ValidateFieldOffset(0x0, KGDTENTRY64, LimitLow);
ValidateFieldOffset(0x2, KGDTENTRY64, BaseLow);
ValidateFieldOffset(0x4, KGDTENTRY64, BaseMiddle);
ValidateFieldOffset(0x5, KGDTENTRY64, Flags1);
ValidateFieldOffset(0x6, KGDTENTRY64, Flags2);
ValidateFieldOffset(0x7, KGDTENTRY64, BaseHigh);
ValidateFieldOffset(0x8, KGDTENTRY64, BaseUpper);
ValidateFieldOffset(0xC, KGDTENTRY64, MustBeZero);
ValidateStructSize(0x10, KGDTENTRY64);
ValidateStructAlignment(0x10, KGDTENTRY64);
#pragma endregion

// size: 0x68
struct KTSS64 {
	unused_bytes(4); // 0x0
	uint64_t Rsp0 gnu_packed; // 0x4
	uint64_t Rsp1 gnu_packed; // 0xC
	uint64_t Rsp2 gnu_packed; // 0x14
	uint64_t Ist[8] gnu_packed; // 0x1C
	unused_bytes(0xA); // 0x5C
	uint16_t IoMapBase; // 0x66
	// 0x68
};
#pragma region // KTSS64 Validation
ValidateFieldOffset(0x4, KTSS64, Rsp0);
ValidateFieldOffset(0xC, KTSS64, Rsp1);
ValidateFieldOffset(0x14, KTSS64, Rsp2);
ValidateFieldOffset(0x1C, KTSS64, Ist);
ValidateFieldOffset(0x66, KTSS64, IoMapBase);
ValidateStructSize(0x68, KTSS64);
#pragma endregion

// size: 0x10
struct alignas(16) KIDTENTRY64 {
	uint16_t OffsetLow; // 0x0
	uint16_t Selector; // 0x2
	union {
		uint16_t flags; // 0x4
		struct {
			uint16_t IstIndex : 3;
			uint16_t : 5;
			uint16_t Type : 5; // (Shouldn't this be 4 bits?)
			uint16_t Dpl : 2;
			uint16_t Present : 1;
		};
	};
	uint16_t OffsetMiddle; // 0x6
	uint32_t OffsetHigh; // 0x8
	padding_bytes(4); // 0xC
	// 0x10
};
#pragma region // KIDTENTRY64
ValidateFieldOffset(0x0, KIDTENTRY64, OffsetLow);
ValidateFieldOffset(0x2, KIDTENTRY64, Selector);
ValidateFieldOffset(0x4, KIDTENTRY64, flags);
ValidateFieldOffset(0x6, KIDTENTRY64, OffsetMiddle);
ValidateFieldOffset(0x8, KIDTENTRY64, OffsetHigh);
ValidateStructSize(0x10, KIDTENTRY64);
ValidateStructAlignment(0x10, KIDTENTRY64);
#pragma endregion

//#define PAGE_SIZE 4096

struct SetGetEntryFlags {
	uint8_t LongMode : 1;
	uint8_t is_32_bit : 1;
};
enum KGDTTYPE {
	Reserved0 = 0x0,
	TSS_16_Available = 0x1,
	LDT = 0x2,
	TSS_16_Busy = 0x3,
	Call_Gate_16 = 0x4,
	Task_Gate = 0x5,
	Interrupt_Gate_16 = 0x6,
	Trap_Gate_16 = 0x7,
	Reserved8 = 0x8,
	TSS_Available = 0x9,
	ReservedA = 0xA,
	TSS_Busy = 0xB,
	Call_Gate = 0xC,
	ReservedD = 0xD,
	Interrupt_Gate = 0xE,
	Trap_Gate = 0xF,
	Read_Only_Data = 0x10,
	Read_Only_Data_Accessed = 0x11,
	Read_Write_Data = 0x12,
	Read_Write_Data_Accessed = 0x13,
	Read_Only_Data_Expand_Down = 0x14,
	Read_Only_Data_Expand_Down_Accessed = 0x15,
	Read_Write_Data_Expand_Down = 0x16,
	Read_Write_Data_Expand_Down_Accessed = 0x17,
	Execute_Only_Code = 0x18,
	Execute_Only_Code_Accessed = 0x19,
	Execute_Read_Code = 0x1A,
	Execute_Read_Code_Accessed = 0x1B,
	Execute_Only_Code_Conforming = 0x1C,
	Execute_Only_Code_Conforming_Accessed = 0x1D,
	Execute_Read_Code_Conforming = 0x1E,
	Execute_Read_Code_Conforming_Accessed = 0x1F
};

template<bool include_long_check = true>
dllexport gnu_attr(target("no-sse")) void ArchSetGdtEntry(KGDTENTRY* gdt_base, uint32_t offset, uintptr_t base, size_t limit, KGDTTYPE type, uint32_t dpl, SetGetEntryFlags flags) {
	KGDTENTRY64* entry = based_pointer<KGDTENTRY64>(gdt_base, AlignDownToMultipleOf2(offset, alignof(KGDTENTRY)));
	if ((entry->Granularity = limit > PAGE_SIZE << 12)) {
		limit >>= 12;
	}
	entry->LimitHigh = 0;
	entry->LimitLow = limit;
	entry->BaseLow = base;
	entry->LimitHigh = limit >> bitsof(entry->LimitLow);
	entry->BaseMiddle = base >> bitsof(entry->BaseLow);
	entry->BaseHigh = base >> bitsof(entry->BaseLow) + bitsof(entry->BaseMiddle);
	entry->LongMode = flags.LongMode;
	if ((include_long_check && flags.LongMode) || type == TSS_Available) {
		entry->BaseUpper = base >> bitsof(entry->BaseLow) + bitsof(entry->BaseMiddle) + bitsof(entry->BaseHigh);
		entry->MustBeZero = 0;
	}
	entry->Present = (bool)type;
	entry->Dpl = dpl;
	entry->Type = type;
	entry->DefaultBig = flags.is_32_bit;
	entry->System = 0;
}

#define FLAGS_NONE 0x0
#define FLAGS_IS_LONG_MODE 0x1
#define FLAGS_IS_32_BIT 0x2
#define FLAGS_ALL 0x3

#define KERNEL_MODE 0x0
#define DRIVER_MODE 0x1
#define SECURE_USER_MODE 0x2
#define USER_MODE 0x3

template<bool include_long_check = false, bool force_single_reg = false>
dllexport gnu_attr(target("no-sse")) void ArchSetGdtEntryNew(KGDTENTRY* gdt_base, uint32_t offset, uintptr_t base, size_t limit, KGDTTYPE type, uint32_t dpl, uint8_t flags) {
	KGDTENTRY64* entry = based_pointer<KGDTENTRY64>(gdt_base, AlignDownToMultipleOf2(offset, alignof(KGDTENTRY)));
	if constexpr (force_single_reg) {
		__asm__ volatile ("" : asm_arg("+r", entry));
	}

	uint32_t base_temp = base;
	if ((include_long_check && (flags & FLAGS_IS_LONG_MODE)) || type == TSS_Available) {
		*(uint64_t*)&entry->BaseUpper = base >> (bitsof(entry->BaseLow) + bitsof(entry->BaseMiddle) + bitsof(entry->BaseHigh));
	}

	uint16_t temp = 0;
	uint32_t limit_temp = limit;

	temp |= flags & FLAGS_ALL; // DefaultBig, LongMode
	temp <<= 1; // System
	temp <<= 4; // LimitHigh
	temp <<= 1;
	temp |= (bool)type; // Present
	temp <<= 2;
	temp |= dpl & 0x3; // Dpl
	temp <<= 5;
	temp |= type & 0x1F; // Type
	
	entry->BaseLow = base_temp;
	base_temp >>= bitsof(entry->BaseLow);
	entry->BaseMiddle = base_temp;
	base_temp >>= bitsof(entry->BaseMiddle);
	entry->BaseHigh = base_temp;
	
	if (limit > PAGE_SIZE << 12) {
		limit_temp >>= 12;
		temp |= 0x8000; // Granularity
	}
	entry->LimitLow = limit_temp;
	//temp |= limit_temp >> bitsof(entry->LimitLow);
	temp |= limit_temp >> 8 & 0xF00;
	entry->Flags1 = temp;
	entry->Flags2 = temp >> 8;
}

dllexport gnu_attr(target("no-sse")) void ArchSetGdtEntryNew2(KGDTENTRY* gdt_base, uint32_t offset, uintptr_t base, size_t limit, KGDTTYPE type, uint32_t dpl, uint8_t flags) {
	return ArchSetGdtEntryNew<true, true>(gdt_base, offset, base, limit, type, dpl, flags);
}

// Inline versions

dllexport gnu_noinline gnu_attr(target("no-sse")) void ArchBuildKernelGdt(KGDTENTRY* gdt_base, uintptr_t tss_base) {
	ArchSetGdtEntry(gdt_base,	0x00,	0,			0,			Reserved0,					KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// Null
	ArchSetGdtEntry(gdt_base,	0x10,	0,			0,			Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = true, .is_32_bit = false });	// 64-bit Kernel Code
	ArchSetGdtEntry(gdt_base,	0x18,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// Kernel Data (Maybe unused?)
	ArchSetGdtEntry(gdt_base,	0x40,	tss_base,	0x67,		TSS_Available,				KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// TSS
	ArchSetGdtEntry(gdt_base,	0x30,	0,			0,			Execute_Read_Code,			USER_MODE,		{ .LongMode = true, .is_32_bit = false });	// 64-bit User Code
	ArchSetGdtEntry(gdt_base,	0x20,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// 32-bit User Code
	ArchSetGdtEntry(gdt_base,	0x28,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// User Data
	ArchSetGdtEntry(gdt_base,	0x50,	0,			0xFFF,		Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// TLS
	ArchSetGdtEntry(gdt_base,	0x60,	0,			0xFFFFFFFF,	Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// 32-bit Kernel Code
}

dllexport gnu_noinline gnu_attr(target("no-sse")) void ArchBuildKernelGdtNew(KGDTENTRY* gdt_base, uintptr_t tss_base) {
	ArchSetGdtEntry(gdt_base,	0x00,	0,			0,			Reserved0,					KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// Null
	// 0x08 is available
	ArchSetGdtEntry(gdt_base,	0x10,	0,			0,			Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = true, .is_32_bit = false });	// 64-bit Kernel Code
	ArchSetGdtEntry(gdt_base,	0x18,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// Kernel Data (Maybe unused?)
	ArchSetGdtEntry(gdt_base,	0x20,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// 32-bit User Code
	ArchSetGdtEntry(gdt_base,	0x28,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// User Data
	ArchSetGdtEntry(gdt_base,	0x30,	0,			0,			Execute_Read_Code,			USER_MODE,		{ .LongMode = true, .is_32_bit = false });	// 64-bit User Code
	// 0x38 is available
	ArchSetGdtEntry(gdt_base,	0x40,	tss_base,	0x67,		TSS_Available,				KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// TSS
	// 0x48 is the upper TSS
	ArchSetGdtEntry(gdt_base,	0x50,	0,			0xFFF,		Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// TLS
	// 0x58 is available
	ArchSetGdtEntry(gdt_base,	0x60,	0,			0xFFFFFFFF,	Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// 32-bit Kernel Code
	// 0x68 is available
	// 0x70 is the LDT
	// 0x78 is the upper LDT
	
	ArchSetGdtEntry(gdt_base,	0x08,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		{ .LongMode = false, .is_32_bit = false });	// 16-bit User Code
	ArchSetGdtEntry(gdt_base,	0x38,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = false });	// 16-bit User Data
	ArchSetGdtEntry(gdt_base,	0x58,	(uintptr_t)gdt_base,	0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,	{ .LongMode = false, .is_32_bit = true });	// User Mode GDT Access
}

dllexport gnu_noinline gnu_attr(target("no-sse")) void ArchBuildKernelGdtNew2(KGDTENTRY* gdt_base, uintptr_t tss_base) {
	ArchSetGdtEntryNew(gdt_base,	0x00,	0,			0,			Reserved0,					KERNEL_MODE,	FLAGS_NONE);	// Null
	// 0x08 is available
	ArchSetGdtEntryNew(gdt_base,	0x10,	0,			0,			Execute_Read_Code,			KERNEL_MODE,	FLAGS_IS_LONG_MODE);	// 64-bit Kernel Code
	ArchSetGdtEntryNew(gdt_base,	0x18,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	KERNEL_MODE,	FLAGS_IS_32_BIT);	// Kernel Data (Maybe unused?)
	ArchSetGdtEntryNew(gdt_base,	0x20,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		FLAGS_IS_32_BIT);	// 32-bit User Code
	ArchSetGdtEntryNew(gdt_base,	0x28,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		FLAGS_IS_32_BIT);	// User Data
	ArchSetGdtEntryNew(gdt_base,	0x30,	0,			0,			Execute_Read_Code,			USER_MODE,		FLAGS_IS_LONG_MODE);	// 64-bit User Code
	// 0x38 is available
	ArchSetGdtEntryNew(gdt_base,	0x40,	tss_base,	0x67,		TSS_Available,				KERNEL_MODE,	FLAGS_NONE);	// TSS
	// 0x48 is the upper TSS
	ArchSetGdtEntryNew(gdt_base,	0x50,	0,			0xFFF,		Read_Write_Data_Accessed,	USER_MODE,		FLAGS_IS_32_BIT);	// TLS
	// 0x58 is available
	ArchSetGdtEntryNew(gdt_base,	0x60,	0,			0xFFFFFFFF,	Execute_Read_Code,			KERNEL_MODE,	FLAGS_IS_32_BIT);	// 32-bit Kernel Code
	// 0x68 is available
	// 0x70 is the LDT
	// 0x78 is the upper LDT
	
	ArchSetGdtEntryNew(gdt_base,	0x08,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		FLAGS_NONE);	// 16-bit User Code
	ArchSetGdtEntryNew(gdt_base,	0x38,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		FLAGS_NONE);	// 16-bit User Data
	ArchSetGdtEntryNew(gdt_base,	0x58,	(uintptr_t)gdt_base,	0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,	FLAGS_IS_32_BIT);	// User Mode GDT Access
}

// No inline versions

dllexport gnu_noinline gnu_attr(target("no-sse")) void ArchBuildKernelGdt_noinline(KGDTENTRY* gdt_base, uintptr_t tss_base) {
	clang_noinline ArchSetGdtEntry(gdt_base,	0x00,	0,			0,			Reserved0,					KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// Null
	clang_noinline ArchSetGdtEntry(gdt_base,	0x10,	0,			0,			Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = true, .is_32_bit = false });	// 64-bit Kernel Code
	clang_noinline ArchSetGdtEntry(gdt_base,	0x18,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// Kernel Data (Maybe unused?)
	clang_noinline ArchSetGdtEntry(gdt_base,	0x40,	tss_base,	0x67,		TSS_Available,				KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// TSS
	clang_noinline ArchSetGdtEntry(gdt_base,	0x30,	0,			0,			Execute_Read_Code,			USER_MODE,		{ .LongMode = true, .is_32_bit = false });	// 64-bit User Code
	clang_noinline ArchSetGdtEntry(gdt_base,	0x20,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// 32-bit User Code
	clang_noinline ArchSetGdtEntry(gdt_base,	0x28,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// User Data
	clang_noinline ArchSetGdtEntry(gdt_base,	0x50,	0,			0xFFF,		Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// TLS
	clang_noinline ArchSetGdtEntry(gdt_base,	0x60,	0,			0xFFFFFFFF,	Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// 32-bit Kernel Code
}

dllexport gnu_noinline gnu_attr(target("no-sse")) void ArchBuildKernelGdtNew_noinline(KGDTENTRY* gdt_base, uintptr_t tss_base) {
	clang_noinline ArchSetGdtEntry(gdt_base,	0x00,	0,			0,			Reserved0,					KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// Null
	// 0x08 is available
	clang_noinline ArchSetGdtEntry(gdt_base,	0x10,	0,			0,			Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = true, .is_32_bit = false });	// 64-bit Kernel Code
	clang_noinline ArchSetGdtEntry(gdt_base,	0x18,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// Kernel Data (Maybe unused?)
	clang_noinline ArchSetGdtEntry(gdt_base,	0x20,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// 32-bit User Code
	clang_noinline ArchSetGdtEntry(gdt_base,	0x28,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// User Data
	clang_noinline ArchSetGdtEntry(gdt_base,	0x30,	0,			0,			Execute_Read_Code,			USER_MODE,		{ .LongMode = true, .is_32_bit = false });	// 64-bit User Code
	// 0x38 is available
	clang_noinline ArchSetGdtEntry(gdt_base,	0x40,	tss_base,	0x67,		TSS_Available,				KERNEL_MODE,	{ .LongMode = false, .is_32_bit = false });	// TSS
	// 0x48 is the upper TSS
	clang_noinline ArchSetGdtEntry(gdt_base,	0x50,	0,			0xFFF,		Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = true });	// TLS
	// 0x58 is available
	clang_noinline ArchSetGdtEntry(gdt_base,	0x60,	0,			0xFFFFFFFF,	Execute_Read_Code,			KERNEL_MODE,	{ .LongMode = false, .is_32_bit = true });	// 32-bit Kernel Code
	// 0x68 is available
	// 0x70 is the LDT
	// 0x78 is the upper LDT
	
	clang_noinline ArchSetGdtEntry(gdt_base,	0x08,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		{ .LongMode = false, .is_32_bit = false });	// 16-bit User Code
	clang_noinline ArchSetGdtEntry(gdt_base,	0x38,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		{ .LongMode = false, .is_32_bit = false });	// 16-bit User Data
	clang_noinline ArchSetGdtEntry(gdt_base,	0x58,	(uintptr_t)gdt_base,	0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,	{ .LongMode = false, .is_32_bit = true });	// User Mode GDT Access
}

dllexport gnu_noinline gnu_attr(target("no-sse")) void ArchBuildKernelGdtNew2_noinline(KGDTENTRY* gdt_base, uintptr_t tss_base) {
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x00,	0,			0,			Reserved0,					KERNEL_MODE,	FLAGS_NONE);	// Null
	// 0x08 is available
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x10,	0,			0,			Execute_Read_Code,			KERNEL_MODE,	FLAGS_IS_LONG_MODE);	// 64-bit Kernel Code
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x18,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	KERNEL_MODE,	FLAGS_IS_32_BIT);	// Kernel Data (Maybe unused?)
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x20,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		FLAGS_IS_32_BIT);	// 32-bit User Code
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x28,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		FLAGS_IS_32_BIT);	// User Data
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x30,	0,			0,			Execute_Read_Code,			USER_MODE,		FLAGS_IS_LONG_MODE);	// 64-bit User Code
	// 0x38 is available
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x40,	tss_base,	0x67,		TSS_Available,				KERNEL_MODE,	FLAGS_NONE);	// TSS
	// 0x48 is the upper TSS
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x50,	0,			0xFFF,		Read_Write_Data_Accessed,	USER_MODE,		FLAGS_IS_32_BIT);	// TLS
	// 0x58 is available
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x60,	0,			0xFFFFFFFF,	Execute_Read_Code,			KERNEL_MODE,	FLAGS_IS_32_BIT);	// 32-bit Kernel Code
	// 0x68 is available
	// 0x70 is the LDT
	// 0x78 is the upper LDT
	
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x08,	0,			0xFFFFFFFF,	Execute_Read_Code,			USER_MODE,		FLAGS_NONE);	// 16-bit User Code
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x38,	0,			0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,		FLAGS_NONE);	// 16-bit User Data
	clang_noinline ArchSetGdtEntryNew(gdt_base,	0x58,	(uintptr_t)gdt_base,	0xFFFFFFFF,	Read_Write_Data_Accessed,	USER_MODE,	FLAGS_IS_32_BIT);	// User Mode GDT Access
}

dllexport KGDTENTRY static_gdt_A[16];
dllexport KGDTENTRY static_gdt_B[16];
dllexport KGDTENTRY static_gdt_C[16];

dllexport void fill_gdts() {
	//memset(static_gdt_A, 0, sizeof(static_gdt_A));
	//memset(static_gdt_B, 0, sizeof(static_gdt_B));
	//memset(static_gdt_C, 0, sizeof(static_gdt_C));
	memset(static_gdt_A, 0, sizeof(KGDTENTRY[16]));
	ArchBuildKernelGdtNew2(static_gdt_A, 0xDEADBEEF);
	memcpy(static_gdt_C, static_gdt_A, sizeof(KGDTENTRY[16]));

	memset(static_gdt_A, 0, sizeof(KGDTENTRY[16]));
	ArchBuildKernelGdtNew(static_gdt_A, 0xDEADBEEF);
	memcpy(static_gdt_B, static_gdt_A, sizeof(KGDTENTRY[16]));

	ArchBuildKernelGdt(static_gdt_A, 0xDEADBEEF);
	if (!memcmp(static_gdt_B, static_gdt_C, sizeof(KGDTENTRY[16]))) {
		__asm int 3
	}
	__asm _emit 0xCC
}

// size: 0x20000
struct UnknownBootStruct {
	unknown_bytes(0x1000); // 0x0
	KIDTENTRY64 idt[0x100]; // 0x1000
	KTSS64 tss; // 0x2000
	unknown_bytes(0xF98); // 0x2068
	KGDTENTRY gdt[0x10]; // 0x3000
	unknown_bytes(0xF80); // 0x3080
	unknown_bytes(0x2000); // 0x4000
	uint64_t tss_stack_0[0xC00]; // 0x6000
	uint64_t kernel_stack[0xC00]; // 0xC000
	uint64_t interrupt_stack_1[0x400]; // 0x12000
	uint64_t interrupt_stack_3[0x400]; // 0x14000
	uint64_t interrupt_stack_2[0x400]; // 0x16000
	uint64_t interrupt_stack_4[0x400]; // 0x18000
	unknown_bytes(0x6000); // 0x1A000
	// 0x20000
};
#pragma region // UnknownBootStruct Validation
ValidateFieldOffset(0x1000, UnknownBootStruct, idt);
ValidateFieldOffset(0x2000, UnknownBootStruct, tss);
ValidateFieldOffset(0x3000, UnknownBootStruct, gdt);
ValidateFieldOffset(0x6000, UnknownBootStruct, tss_stack_0);
ValidateFieldOffset(0xC000, UnknownBootStruct, kernel_stack);
ValidateFieldOffset(0x12000, UnknownBootStruct, interrupt_stack_1);
ValidateFieldOffset(0x14000, UnknownBootStruct, interrupt_stack_3);
ValidateFieldOffset(0x16000, UnknownBootStruct, interrupt_stack_2);
ValidateFieldOffset(0x18000, UnknownBootStruct, interrupt_stack_4);
ValidateStructSize(0x20000, UnknownBootStruct);
#pragma endregion

static constexpr auto wkrbwkjrb = offsetof(UnknownBootStruct, idt);
static constexpr auto wkrbwkjrb2 = sizeof(UnknownBootStruct::kernel_stack);

dllexport void OslArchpKernelSetupPhase0(void* arg1) {
	if (UnknownBootStruct* boot_struct = (UnknownBootStruct*)malloc(0x20000)) {
		memset(boot_struct, 0, 0x1A000);
		//boot_struct->tss.Rsp0
	}
}

union SHA256_HASH {
	struct {
		vec<uint32_t, 4> lower_dwords;
		vec<uint32_t, 4> upper_dwords;
	};
	vec<uint32_t, 8> dwords;
	vec<uint64_t, 4> qwords;
	vec<uint8_t, 32> bytes;
};

#ifdef __SHA__

dllexport gnu_attr(target("sha")) void sha256_intrinsic(SHA256_HASH* state, const uint8_t data[], size_t length) {
	//state->
	//state->bytes[]
	SHA256_HASH local_state = *state;
	vec<uint32_t, 8> temp = shufflevec(local_state.dwords, local_state.dwords, 0, 1, 4, 5, 2, 3, 6, 7);
	
	while (length >= 64) {
		vec<uint32_t, 8> save = temp;
		vec<uint32_t, 4> STATE0 = shufflevec(temp, temp, 0, 1, 2, 3);
		vec<uint32_t, 4> STATE1 = shufflevec(temp, temp, 4, 5, 6, 7);

		vec<uint8_t, 32> MSG_IN = *(unaligned vec<uint8_t, 32>*)data;
		MSG_IN = shufflevec(MSG_IN, MSG_IN, 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 19, 18, 17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28);
		vec<uint32_t, 8> MSG = *(vec<uint32_t, 8>*)&MSG_IN;
		MSG += { 0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5 };
		vec<uint32_t, 4> MSG0 = shufflevec(MSG, MSG, 0, 1, 2, 3);
		vec<uint32_t, 4> MSG1 = shufflevec(MSG, MSG, 4, 5, 6, 7);
		STATE1 = _mm_sha256rnds2_epu32((__m128i)STATE1, (__m128i)STATE0, (__m128i)MSG0);
		STATE0 = _mm_sha256rnds2_epu32((__m128i)STATE0, (__m128i)STATE1, (__m128i)shufflevec(MSG0, MSG0, 2, 3, 0, 0));
		STATE1 = _mm_sha256rnds2_epu32((__m128i)STATE1, (__m128i)STATE0, (__m128i)MSG1);
		STATE0 = _mm_sha256rnds2_epu32((__m128i)STATE0, (__m128i)STATE1, (__m128i)shufflevec(MSG1, MSG1, 2, 3, 0, 0));

		MSG_IN = *(unaligned vec<uint8_t, 32>*)(data + 32);
		MSG_IN = shufflevec(MSG_IN, MSG_IN, 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 19, 18, 17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28);
		MSG = *(vec<uint32_t, 8>*)&MSG_IN;
		MSG += { 0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174 };
		vec<uint32_t, 4> MSG2 = shufflevec(MSG, MSG, 0, 1, 2, 3);
		vec<uint32_t, 4> MSG3 = shufflevec(MSG, MSG, 4, 5, 6, 7);
		STATE1 = _mm_sha256rnds2_epu32((__m128i)STATE1, (__m128i)STATE0, (__m128i)MSG2);
		STATE0 = _mm_sha256rnds2_epu32((__m128i)STATE0, (__m128i)STATE1, (__m128i)shufflevec(MSG2, MSG2, 2, 3, 0, 0));
		STATE1 = _mm_sha256rnds2_epu32((__m128i)STATE1, (__m128i)STATE0, (__m128i)MSG3);
		STATE0 = _mm_sha256rnds2_epu32((__m128i)STATE0, (__m128i)STATE1, (__m128i)shufflevec(MSG3, MSG3, 2, 3, 0, 0));
		
		MSG0 = _mm_sha256msg1_epu32((__m128i)MSG0, (__m128i)MSG1);
		MSG0 += shufflevec(MSG3, MSG2, 1, 2, 3, 4);
		MSG0 = _mm_sha256msg2_epu32((__m128i)MSG0, (__m128i)MSG3);
		MSG2 = _mm_sha256msg1_epu32((__m128i)MSG2, (__m128i)MSG3);

		//MSG_IN = *(unaligned vec<uint8_t, 32>*)(data + 64);
		//MSG_IN = shufflevec(MSG_IN, MSG_IN, 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 19, 18, 17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28);
		//MSG = *(vec<uint32_t, 8>*) & MSG_IN;
		//MSG += { 0xE49B69C1, 0xEFBE4786, 0xFC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA };


		temp = shufflevec(STATE0, STATE1, 0, 1, 2, 3, 4, 5, 6, 7);
		length += 64;
	}

	state->dwords = temp;
}

#endif

inline void sha256_sse2_step() {

}

dllexport gnu_attr(target("no-sse3")) void transpose_to_di4_sse2(
	unaligned const vec<uint32_t, 4> *restrict s0,
	unaligned const vec<uint32_t, 4> *restrict s1,
	unaligned const vec<uint32_t, 4> *restrict s2,
	unaligned const vec<uint32_t, 4> *restrict s3,
	vec<uint32_t, 4> *restrict out
) {
	nounroll for (
		size_t i = 0, j = 0;
		i < 16;
		i += 4, ++j
	) {
		vec<uint32_t, 4> t0 = shufflevec(s0[j], s1[j], 0, 4, 1, 5); // s0[0], s1[0], s0[1], s1[1]
		vec<uint32_t, 4> t1 = shufflevec(s2[j], s3[j], 0, 4, 1, 5); // s2[0], s3[0], s2[1], s3[1]
		vec<uint32_t, 4> t2 = shufflevec(s0[j], s1[j], 2, 6, 3, 7); // s0[2], s1[2], s0[3], s1[3]
		vec<uint32_t, 4> t3 = shufflevec(s2[j], s3[j], 2, 6, 3, 7); // s2[2], s3[2], s2[3], s3[3]
		out[i + 0] = shufflevec(t0, t1, 0, 1, 4, 5); // s0[0], s1[0], s2[0], s3[0]
		out[i + 1] = shufflevec(t0, t1, 2, 3, 6, 7); // s0[1], s1[1], s2[1], s3[1]
		out[i + 2] = shufflevec(t2, t3, 0, 1, 4, 5); // s0[2], s1[2], s2[2], s3[2]
		out[i + 3] = shufflevec(t2, t3, 2, 3, 6, 7); // s0[3], s1[3], s2[3], s3[3]
	}
}

dllexport gnu_attr(target("no-sse3")) void transpose_to_di4_sse2_new(
	unaligned const vec<uint32_t, 4> *restrict s0,
	unaligned const vec<uint32_t, 4> *restrict s1,
	unaligned const vec<uint32_t, 4> *restrict s2,
	unaligned const vec<uint32_t, 4> *restrict s3,
	vec<uint32_t, 4> *restrict out
) {
	nounroll for (
		size_t i = 0, j = 0;
		i < 16;
		i += 4, ++j
	) {
		out[i + 0] = { s0[j][0], s1[j][0], s2[j][0], s3[j][0] };
		out[i + 1] = { s0[j][1], s1[j][1], s2[j][1], s3[j][1] };
		out[i + 2] = { s0[j][2], s1[j][2], s2[j][2], s3[j][2] };
		out[i + 3] = { s0[j][3], s1[j][3], s2[j][3], s3[j][3] };
	}
}

dllexport gnu_attr(target("no-sse3")) void transpose_to_di4_sse2_new2(
	unaligned const vec<uint32_t, 4> *restrict s0,
	unaligned const vec<uint32_t, 4> *restrict s1,
	unaligned const vec<uint32_t, 4> *restrict s2,
	unaligned const vec<uint32_t, 4> *restrict s3,
	vec<uint32_t, 4> *restrict out
) {
	nounroll for (
		size_t i = 0, j = 0;
		i < 16;
		i += 4, ++j
	) {
		vec<uint32_t, 4> t0 = { s0[j][0], s1[j][0], s0[j][1], s1[j][1] };
		vec<uint32_t, 4> t2 = { s0[j][2], s1[j][2], s0[j][3], s1[j][3] };
		vec<uint32_t, 4> t1 = { s2[j][0], s3[j][0], s2[j][1], s3[j][1] };
		vec<uint32_t, 4> t3 = { s2[j][2], s3[j][2], s2[j][3], s3[j][3] };
		out[i + 0] = { t0[0], t0[1], t1[0], t1[1] };
		out[i + 1] = { t0[2], t0[3], t1[2], t1[3] };
		out[i + 2] = { t2[0], t2[1], t3[0], t3[1] };
		out[i + 3] = { t2[2], t2[3], t3[2], t3[3] };
	}
}

dllexport gnu_attr(target("no-sse3")) void transpose_to_di4_sse2_new3(
	unaligned const vec<uint32_t, 4> *restrict s0,
	unaligned const vec<uint32_t, 4> *restrict s1,
	unaligned const vec<uint32_t, 4> *restrict s2,
	unaligned const vec<uint32_t, 4> *restrict s3,
	vec<uint32_t, 4> *restrict out
) {
	nounroll for (
		size_t i = 0, j = 0;
		i < 16;
		i += 4, ++j
	) {
		vec<uint32_t, 4> t0, t1, t2, t3, t4;

		t0 = s0[j];
		t2 = s1[j];
		t4 = t0;
		t0 = shufflevec(t0, t2, 0, 4, 1, 5);
		t2 = shufflevec(t2, t4, 6, 2, 7, 3);

		t1 = s2[j];
		t3 = s3[j];
		t4 = t1;
		t1 = shufflevec(t1, t3, 0, 4, 1, 5);
		t3 = shufflevec(t3, t4, 6, 2, 7, 3);

		t4 = t0;
		t0 = shufflevec(t0, t1, 0, 1, 4, 5);
		t1 = shufflevec(t1, t4, 6, 7, 2, 3);

		t4 = t2;
		t2 = shufflevec(t2, t3, 0, 1, 4, 5);
		t3 = shufflevec(t3, t4, 6, 7, 2, 3);

		out[i + 0] = t0;
		out[i + 1] = t1;
		out[i + 2] = t2;
		out[i + 3] = t3;
	}
}

dllexport gnu_attr(target("no-sse3")) void transpose_to_di4_sse2_new4(
	unaligned const vec<uint32_t, 4> *restrict s0,
	unaligned const vec<uint32_t, 4> *restrict s1,
	unaligned const vec<uint32_t, 4> *restrict s2,
	unaligned const vec<uint32_t, 4> *restrict s3,
	vec<uint32_t, 4> *restrict out
) {
	nounroll for (
		size_t i = 0, j = 0;
		i < 16;
		i += 4, ++j
	) {
		vec<uint32_t, 4> t0, t1, t2, t3, t4;

		t0 = s0[j]; // s0[0], s0[1], s0[2], s0[3]
		t2 = s1[j]; // s1[0], s1[1], s1[2], s1[3]
		t1 = s2[j]; // s2[0], s2[1], s2[2], s2[3]
		t3 = s3[j]; // s3[0], s3[1], s3[2], s3[3]

		t4 = t0; // s0[0], s0[1], s0[2], s0[3]
		t0 = shufflevec(t0, t2, 0, 4, 1, 5); // s0[0], s1[0], s0[1], s1[1]
		t2 = shufflevec(t2, t4, 6, 2, 7, 3); // s0[2], s1[2], s0[3], s1[3]

		t4 = t1; // s2[0], s2[1], s2[2], s2[3]
		t1 = shufflevec(t1, t3, 0, 4, 1, 5); // s2[0], s3[0], s2[1], s3[1]
		t3 = shufflevec(t3, t4, 6, 2, 7, 3); // s2[2], s3[2], s2[3], s3[3]

		t4 = t0; // s0[0], s1[0], s0[1], s1[1]
		t0 = shufflevec(t0, t1, 0, 1, 4, 5); // s0[0], s1[0], s2[0], s3[0]
		t1 = shufflevec(t1, t4, 6, 7, 2, 3); // s0[1], s1[1], s2[1], s3[1]

		t4 = t2; // s0[2], s1[2], s0[3], s1[3]
		t2 = shufflevec(t2, t3, 0, 1, 4, 5); // s0[2], s1[2], s2[2], s3[2]
		t3 = shufflevec(t3, t4, 6, 7, 2, 3); // s0[3], s1[3], s2[3], s3[3]

		out[i + 0] = t0; // s0[0], s1[0], s2[0], s3[0]
		out[i + 1] = t1; // s0[1], s1[1], s2[1], s3[1]
		out[i + 2] = t2; // s0[2], s1[2], s2[2], s3[2]
		out[i + 3] = t3; // s0[3], s1[3], s2[3], s3[3]
	}
}


gnu_attr(target("no-sse3")) void sha256_sse2(SHA256_HASH* state, const uint8_t data[], size_t length) {
	//vec<uint32_t, 4> STATE0 = state->lower_dwords;
	//vec<uint32_t, 4> STATE1 = state->upper_dwords;

	vec<uint32_t, 4> m[4];

	for (size_t i = 0; i < 4; ++i) {
		vec<uint8_t, 16> bytes = ((unaligned vec<uint8_t, 16>*)data)[i];
		m[i] = (__m128)shufflevec(bytes, bytes, 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);
	}
	for (size_t i = 16; i < 64; ++i) {
		size_t j = i / 4;
		size_t k = i % 4;
		
	}
}

#include <vector>
using BigInteger = std::vector<usreg_t>;
dllexport bool fastcall add_big_int(BigInteger& total, const BigInteger& value) {
	size_t total_length = total.size();
	size_t value_length = value.size();
	size_t i = 0;
	if (expect_chance(total_length && value_length, true, 1.0)) {
		usreg_t* total_base = total.data();
		const usreg_t* value_base = value.data();
		total_length -= value_length;
		usreg_t temp;
		__asm__ volatile (
			"   clc \n"
			".align 16 \n"
			"1: \n"
			"   mov (%[value_base],%[i],%c[ptr_mul]), %[temp] \n"
			"   adc %[temp], (%[total_base],%[i],%c[ptr_mul]) \n"
			"   lea 1(%[i]), %[i] \n"
			"   loop 1b \n"
			"   mov %[total_length], %[value_length] \n"
			JPCXZ "2f \n"
			".align 16 \n"
			"1: \n"
#if _M_IX86
			"   adcl $0, (%[total_base],%[i],%c[ptr_mul]) \n"
#else
			"   adcq $0, (%[total_base],%[i],%c[ptr_mul]) \n"
#endif
			"   lea 1(%[i]), %[i] \n"
			"   loop 1b \n"
			"2: \n"
			: asm_arg("+r", i), asm_arg("=r", temp), asm_arg("+c", value_length), asm_flags(c, i)
			: asm_arg("r", total_length), asm_arg("r", value_base), asm_arg("r", total_base), [ptr_mul]"i"(sizeof(void*))
		);
	}
	return i;
}

dllexport gnu_noinline int stdcall __sub_4CADC0(const char* lookup, int Arg2) asm_symbol_rel(0x4CADC0);
dllexport gnu_noinline int stdcall __sub_4CADC0(const char* lookup, int Arg2) {
	use_var(lookup);
	use_var(Arg2);
	return rand();
}

struct SaveManager {
	unsigned char dummy[0x31]; // 0x0
	bool autosave_enabled; // 0x31
};

extern "C" {
	dllexport const char* overwrite_message asm_symbol_abs(0x5F97B8) = "MEMCARD_OVERWRITE_WARNING_PC";
}

dllexport gnu_noinline bool fastcall new_overwrite_warning(SaveManager* save_manager) {
	int selection = 3;
	if (!expect(save_manager->autosave_enabled, true)) {
		selection = __sub_4CADC0(overwrite_message, 4);
	}
	return expect(selection == 3, true);
}

#if IS_X64

dllexport void wasted_wtf_is_this() {
	size_t n, m, k;
	std::cin >> n;
	std::cin >> m;
	std::cin >> k;
	if (!n || !m || k >= n || k >= m) return;
	size_t a[m][n];
	for (size_t y = 0; y < m; ++y) {
		for (size_t x = 0; x < n; ++x) {
			std::cin >> a[y][x];
		}
	}
	n -= k;
	for (size_t y = 0; y < m; ++y) {
		for (size_t x = 0; x <= n; ++x) {
			size_t temp = 0;
			for (size_t i = 0; i < k; ++i) {
				temp += a[y][x + i];
			}
			a[y][x] = temp;
		}
	}
	m -= k;
	for (size_t y = 0; y <= m; ++y) {
		for (size_t x = 0; x <= n; ++x) {
			size_t temp = 0;
			for (size_t i = 0; i < k; ++i) {
				temp += a[y + i][x];
			}
			a[y][x] = temp;
		}
	}
	size_t ret = SIZE_MAX;
	size_t ret_x;
	size_t ret_y;
	for (size_t y = 0; y <= m; ++y) {
		for (size_t x = 0; x <= n; ++x) {
			size_t temp = a[y][x];
			if (temp < ret) {
				ret_x = x;
				ret_y = y;
				ret = temp;
			}
		}
	}
	if (ret != SIZE_MAX) {
		printf(
			"Sum: %zu\n"
			"X:   %zu\n"
			"Y:   %zu"
			, ret
			, ret_x
			, ret_y
		);
	}
}

dllexport void wasted_wtf_is_this2() {
	size_t n, m, k;
	std::cin >> n;
	std::cin >> m;
	std::cin >> k;
	if (!n || !m || !k || k >= n || k >= m) return;
	size_t pf[m][n];
	for (size_t y = 0; y < m; ++y) {
		for (size_t x = 0; x < n; ++x) {
			size_t temp;
			std::cin >> temp;
			if (y) temp += pf[y - 1][x];
			if (x) temp += pf[y][x - 1];
			if (y && x) temp -= pf[y - 1][x - 1];
			pf[y][x] = temp;
		}
	}
	--k;
	n -= k;
	m -= k;
	size_t ret = SIZE_MAX;
	size_t ret_x;
	size_t ret_y;
	for (size_t y = 0; y < m; ++y) {
		for (size_t x = 0; x < n; ++x) {
			size_t temp = pf[y + k][x + k];
			if (y) temp -= pf[y - 1][x + k];
			if (x) temp -= pf[y + k][x - 1];
			if (y && x) temp += pf[y - 1][x - 1];
			if (temp < ret) {
				ret_x = x;
				ret_y = y;
				ret = temp;
			}
		}
	}
	if (ret != SIZE_MAX) {
		printf(
			"Sum: %zu\n"
			"X:   %zu\n"
			"Y:   %zu"
			, ret
			, ret_x
			, ret_y
		);
	}
}

dllexport void wasted_wtf_is_this3() {
	size_t n, m, k;
	std::cin >> n;
	std::cin >> m;
	std::cin >> k;
	if (!n || !m || !k || k >= n || k >= m) return;
	size_t pf[m + 1][n + 1];
	
	// Zero out the edges of the array
	for (size_t x = 0; x <= n; ++x) pf[0][x] = 0;
	for (size_t y = 1; y <= m; ++y) pf[y][0] = 0;
	
	// Read all the data in
	for (size_t y = 0; y < m; ++y) {
		for (size_t x = 0; x < n; ++x) {
			size_t temp;
			std::cin >> temp;
			pf[y + 1][x + 1] = temp + pf[y][x + 1] + pf[y + 1][x] - pf[y][x];
		}
	}
	
	// Calculate the smallest sum
	m -= k;
	n -= k;
	size_t ret = SIZE_MAX;
	size_t ret_x;
	size_t ret_y;
	for (size_t y = 0; y <= m; ++y) {
		for (size_t x = 0; x <= n; ++x) {
			size_t temp = pf[y + k][x + k] + pf[y][x] - pf[y][x + k] - pf[y + k][x];
			if (temp < ret) {
				ret = temp;
				ret_x = x;
				ret_y = y;
			}
		}
	}
	
	// Print the answer
	if (ret != SIZE_MAX) {
		printf(
			"Sum: %zu\n"
			"X:   %zu\n"
			"Y:   %zu"
			, ret
			, ret_x
			, ret_y
		);
	}
}

#endif