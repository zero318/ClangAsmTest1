#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"
#include "ClangAsmTest1.h"


#define POINTER_64 __ptr64

#if IS_X64
#include <iostream>
#endif

#include <stdint.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <string.h>
#include <float.h>

#include <Bits.h>

#include <array>
#include <thread>

#include "zero/util.h"
//#include "zero/BoundingBox.h"
#undef _WIN64
//#undef WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <wincrypt.h>
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
#include <new>

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
#define TEST_BOUNDS 5.795216e+19f
	
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

#if !IS_X64

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
			//value -= _mm_round_sd((__m128d){value}, (__m128d){ value }, _MM_FROUND_NO_EXC | _MM_FROUND_TO_NEAREST_INT)[0];
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

#endif

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
dllexport gnu_used struct dummy_name { \
	inline operator usreg_t() { \
		return bitcast<usreg_t>(this); \
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
		__asm__ volatile ("":"+t"(ld_value));
		*/
		long double ld_value;
		uint64_t yeet = counter;
		__asm__ volatile (
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
		__asm__ volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom3_end_time = rdtsc();

	custom4_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		int64_t yeet;
		__asm__ volatile (
			"BT $0x3F, %[counter] \n"
			"SBB %[yeet], %[yeet] \n"
			: asm_arg("=r", yeet)
			: asm_arg("r", counter)
		);
		ld_value += MAGIC_VALUE_A_ARRAY2[1 + yeet];
		__asm__ volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom4_end_time = rdtsc();

	custom5_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		int64_t yeet = counter;
		__asm__ volatile (
			"ADD %[yeet], %[yeet] \n"
			"SBB %[yeet], %[yeet] \n"
			: asm_arg("+r", yeet)
		);
		ld_value += MAGIC_VALUE_A_ARRAY2[1 + yeet];
		__asm__ volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom5_end_time = rdtsc();

	custom6_start_time = rdtsc();
	counter = 0;
	do {
		uint64_t yeet = counter;
		long double ld_value;
		__asm__ volatile (
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
#include <inttypes.h>

//dllexport int64_t old_float_int;
dllexport float uint_max_float = bitcast<float>(0x4F800000u);
dllexport float uint_max_floatB = bitcast<float>(0x2F800000u);
dllexport double uint_max_doubleB = bitcast<double>(0x3DF0000000000000u);
dllexport double uint_convert_double = bitcast<double>(0x41F0000000000000u);
dllexport double uint_convert_array[2] = {
	0.0,
	bitcast<double>(0x41F0000000000000u)
};
static constexpr FCW single_precision_fcw = {
	.invalid_operation_mask = 1,
	.denormal_operand_mask = 1,
	.divide_by_zero_mask = 1,
	.overflow_mask = 1,
	.underflow_mask = 1,
	.precision_mask = 1,
	.precision = SinglePrecision,
	.rounding = RoundToNearest
};
static constexpr FCW double_precision_fcw = {
	.invalid_operation_mask = 1,
	.denormal_operand_mask = 1,
	.divide_by_zero_mask = 1,
	.overflow_mask = 1,
	.underflow_mask = 1,
	.precision_mask = 1,
	.precision = DoublePrecision,
	.rounding = RoundToNearest
};
static constexpr FCW extended_precision_fcw = {
	.invalid_operation_mask = 1,
	.denormal_operand_mask = 1,
	.divide_by_zero_mask = 1,
	.overflow_mask = 1,
	.underflow_mask = 1,
	.precision_mask = 1,
	.precision = ExtendedPrecision,
	.rounding = RoundToNearest
};

//#include <complex.h>

struct Rng {
	uint16_t value; // 0x0
	uint32_t index; // 0x4
	// 0x8

	static uint16_t calculate_step_1(uint16_t value) {
		return (value ^ 0x9630) - 0x6553;
	}
	static uint16_t calculate_step_2(uint16_t value) {
		return ((value & 0xC000) >> 14) + (value << 2) & 0xFFFF;
	}

	static long double regcall calculate_float_old(const int64_t* value) {
#pragma float_control(precise, on)
#pragma clang fp reassociate(off)
		//long double temp = *value;
		long double temp;
		__asm__ volatile (
			"fildll %[value]"
			: asm_arg("=t", temp)
			: [value] "m"(*value)
		);
		return temp / uint_max_float;
	}
	static forceinline long double regcall calculate_float_newA(const int32_t* value) {
#pragma float_control(precise, on)
#pragma clang fp reassociate(off)
		//long double temp = *value;
		long double temp;
		__asm__ volatile (
			"fildl %[value]"
			: asm_arg("=t", temp)
			: [value]"m"(*value)
		);
		if (*value < 0) {
			//temp += uint_max_float;
			__asm__ volatile (
				"fadds %[uint_max_float]"
				: asm_arg("+t", temp)
				: asm_arg("m", uint_max_float)
			);
		}
		return temp * uint_max_floatB;
	}
	static forceinline long double regcall calculate_float_newB(const int32_t* value) {
#pragma float_control(precise, on)
#pragma clang fp reassociate(off)
		//long double temp = *value;
		long double temp;
		__asm__ volatile (
			"fildl %[value]"
			: asm_arg("=t", temp)
			: [value] "m"(*value)
		);
		if (*value < 0) {
			//temp += uint_max_float;
			__asm__ volatile (
				"fadds %[uint_max_float]"
				: asm_arg("+t", temp)
				: asm_arg("m", uint_max_float)
			);
		}
		return temp * uint_max_doubleB;
	}
	static forceinline float vectorcall calculate_float_newC(uint32_t value) {
		vec<int32_t, 2> temp_vecA = { value, 0 };
		vec<double, 2> temp_vecB = convertvec(temp_vecA, vec<double, 2>);
		temp_vecB[0] += uint_convert_array[value >> 31];
		vec<float, 2> temp_vecC = convertvec(temp_vecB, vec<float, 2>);
		return temp_vecC[0] * uint_max_floatB;
	}
	/*
	static long double regcall calculate_double_float_oldA(const uint64_t& value) {
#pragma float_control(precise, on)
		long double double_precision = value;
		return double_precision / uint_max_float;
	}
	static long double regcall calculate_extended_float_old(const uint64_t& value) {
#pragma float_control(precise, on)
		long double extended_precision = value;
		return extended_precision / uint_max_float;
	}
	*/

	void log_all_values(FILE* file) {
		fputs(
			"InternalIndex,"
			"NextU16,NextU16_UDoALG,"
			"NextU32_Old,NextU32_New,"
			"NextPI32_Old,NextPI32_New,"
			"NextFloat24_Old,NextFloat53_Old," // EoSD, PCB, IN, StB
			"NextFloat24_NewA,NextFloat53_NewA," // PoFV, MoF, UB
			"NextFloat24_NewB,NextFloat54_NewB,NextFloat64_NewB," // SA, UFO, DS, GFW, TD
			"NextFloat_NewC\n" // Everything else
			, file
		);
		this->value = 0;
		this->index = 0;
		FCW original_fcw = current_fcw();
		do {
			uint16_t high_halfA = this->calculate_step_1(this->value);
			uint16_t high_halfB = this->calculate_step_2(high_halfA);
			uint16_t low_halfA = this->calculate_step_1(high_halfB);
			uint16_t low_halfB = this->calculate_step_2(low_halfA);

			uint16_t next_ushort_new = high_halfA;
			uint16_t next_ushort_old = high_halfB;
			uint32_t next_uint_old = ((uint32_t)high_halfB << 16) | low_halfB;
			uint32_t next_uint_new = ((uint32_t)high_halfA << 16) | low_halfA;


			this->value = next_ushort_old;

			int64_t old_float_int = next_uint_old;
			int32_t new_float_int = next_uint_new;

			float single_precision_newC = calculate_float_newC(new_float_int);

			load_fcw(single_precision_fcw);
			long double single_precision_old = calculate_float_old(&old_float_int);
			long double single_precision_newA = calculate_float_newA((int32_t*)&old_float_int);
			long double single_precision_newB = calculate_float_newB((int32_t*)&new_float_int);
			load_fcw(double_precision_fcw);
			long double double_precision_old = calculate_float_old(&old_float_int);
			long double double_precision_newA = calculate_float_newA((int32_t*)&old_float_int);
			long double double_precision_newB = calculate_float_newB((int32_t*)&new_float_int);
			load_fcw(extended_precision_fcw);
			long double extended_precision_newB = calculate_float_newB((int32_t*)&new_float_int);
			load_fcw(original_fcw);

			
			fprintf(file, "%" PRIu32 ",%" PRIu16 ",%" PRIu16 ",%" PRIu32 ",%" PRIu32 ",%" PRIu32 ",%" PRIu32 ",%." MACRO_STR(__DBL_DECIMAL_DIG__) "f,%." MACRO_STR(__DBL_DECIMAL_DIG__) "f,%." MACRO_STR(__DBL_DECIMAL_DIG__) "f,%." MACRO_STR(__DBL_DECIMAL_DIG__) "f,%." MACRO_STR(__DBL_DECIMAL_DIG__) "f,%." MACRO_STR(__DBL_DECIMAL_DIG__) "f,%." MACRO_STR(__DBL_DECIMAL_DIG__) "f,%." MACRO_STR(__DBL_DECIMAL_DIG__) "f\n"
					, this->index++
					, next_ushort_old, next_ushort_new
					, next_uint_old, next_uint_new
					, next_uint_old & 0x7FFFFFFF, next_uint_new & 0x7FFFFFFF
					, (double)single_precision_old, (double)double_precision_old
					, (double)single_precision_newA, (double)double_precision_newA
					, (double)single_precision_newB, (double)double_precision_newB, (double)extended_precision_newB
					, (double)single_precision_newC
			);
		} while (this->value);
	}

	uint16_t get_next_ushort() {
		//uint16_t temp = (this->value ^ 0x9630) - 0x6553;
		//return ((temp & 0xC000) >> 14) + (temp << 2) & 0xFFFF;
		return this->calculate_step_2(this->calculate_step_1(this->value));
	}

	uint32_t get_next_uint_old() {
		uint16_t high_half = this->get_next_ushort();
		uint16_t temp = (high_half ^ 0x9630) - 0x6553;
		uint16_t low_half = ((temp & 0xC000) >> 14) + (temp << 2) & 0xFFFF;
		return ((uint32_t)high_half << 16) | low_half;
	}

	uint32_t get_next_uint_new() {
		uint16_t high_half = (this->value ^ 0x9630) - 0x6553;
		uint16_t tempA = ((high_half & 0xC000) >> 14) + (high_half << 2) & 0xFFFF;
		uint16_t low_half = (tempA ^ 0x9630) - 0x6553;
		uint16_t tempB = ((low_half & 0xC000) >> 14) + (low_half << 2) & 0xFFFF;
		return ((uint32_t)high_half << 16) | low_half;
	}

	uint32_t rand_uint_half_old() {
		uint16_t high_half = this->rand_ushort();
		return ((uint32_t)high_half << 16) | this->get_next_ushort();
	}
	uint32_t rand_uint_half_new() {
		++this->index;
		uint16_t high_half = (this->value ^ 0x9630) - 0x6553;
		uint16_t tempA = ((high_half & 0xC000) >> 14) + (high_half << 2) & 0xFFFF;
		this->value = tempA;
		uint16_t low_half = (tempA ^ 0x9630) - 0x6553;
		uint16_t tempB = ((low_half & 0xC000) >> 14) + (low_half << 2) & 0xFFFF;
		return ((uint32_t)high_half << 16) | low_half;
	}

	uint16_t rand_ushort() {
		++this->index;
		return this->value = this->get_next_ushort();
	}
	uint32_t rand_uint_old() {
		uint16_t high_half = this->rand_ushort();
		return ((uint32_t)high_half << 16) | this->rand_ushort();
	}
	uint32_t rand_uint_new() {
		this->index += 2;
		uint16_t high_half = (this->value ^ 0x9630) - 0x6553;
		uint16_t temp = ((high_half & 0xC000) >> 14) + (high_half << 2) & 0xFFFF;
		uint16_t low_half = (temp ^ 0x9630) - 0x6553;
		this->value = ((low_half & 0xC000) >> 14) + (low_half << 2) & 0xFFFF;
		return ((uint32_t)high_half << 16) | low_half;
	}
};

//#include <timeapi.h>
#pragma comment (lib, "Winmm.lib")

dllexport gnu_noinline void test_zun_rng() {

	Rng template_unseeded_rng;
	template_unseeded_rng.value = 0;
	template_unseeded_rng.index = 0;

	FILE* rng_fileA = fopen("rng_sequence16.csv", "w");

	fputs("InternalIndex,Value\n", rng_fileA);

	Rng test_rng = template_unseeded_rng;
	do {
		uint16_t value = test_rng.rand_ushort();
		fprintf(rng_fileA, "%" PRIu32 ",%" PRIu16 "\n"
				, test_rng.index, value
		);
	} while (test_rng.value);
	printf("Sequence length: %" PRIu32 "\n", test_rng.index);
	fclose(rng_fileA);

	
	FILE* rng_fileB = fopen("rng_sequence32_old.csv", "w");
	fputs("Index,Value\n", rng_fileB);

	test_rng = template_unseeded_rng;
	do {
		uint32_t value = test_rng.rand_uint_half_old();
		fprintf(rng_fileB, "%" PRIu32 ",%" PRIu32 "\n"
				, test_rng.index, value
		);
	} while (test_rng.value);
	fclose(rng_fileB);
	
}

dllexport gnu_noinline void test_zun_rng2() {

	FILE* rng_file = fopen("rng_sequence.csv", "w");
	Rng rng;
	rng.log_all_values(rng_file);
	_fclose_nolock(rng_file);
}

/*

template<bool locked = false, typename T>
inline bool bts_mem32(T& data, size_t index) {
	bool ret;
	if constexpr (locked) {
		__asm__ volatile (
			"lock bts %[index], %[data]"
			: asm_arg("+m", data), asm_flags(c, ret)
			: asm_arg("ir", index)
			: "memory"
			);
	} else {
		__asm__ volatile (
			"bts %[index], %[data]"
			: asm_arg("+m", data), asm_flags(c, ret)
			: asm_arg("ir", index)
			: "memory"
			);
	}
	return ret;
}

std::atomic_uint32_t write_count{ 0 };
std::atomic_uint32_t write_success_count{ 0 };
std::atomic_uint32_t read_count{ 0 };
std::atomic_uint32_t read_success_count{ 0 };

template<typename T, size_t max_index> requires(max_index <= 64)
struct cache_align CrappyLockedQueue {

	static inline constexpr size_t mask_bits = std::max(max_index, sizeof(uint32_t) * CHAR_BIT);
	using MaskT = UBitIntType<mask_bits>;
	static inline constexpr MaskT initial_mask = ~(MaskT)0 >> (bitsof(MaskT) - max_index);

	MaskT written_mask = initial_mask;
	std::atomic_uint32_t lock{};
	uint8_t current_write_index = 0;
	uint8_t current_read_index = 0;
	T values[max_index];

	inline void acquire_lock() {
		while (__builtin_expect(bts_mem32<true>(this->lock, 0), false));
	}
	inline void release_lock() {
		this->lock = 0;
	}

	gnu_noinline bool push_value(T value) {
		this->acquire_lock();
		//++write_count;
		//compare_array[compare_array_index++] = value;
		bool ret;
		if (MaskT current_mask = this->written_mask) {
			//++write_success_count;
			size_t prev_write_index = this->current_write_index;
			MaskT temp_mask = __rorq(current_mask, prev_write_index);
			MaskT cleared_bit_mask = temp_mask & -temp_mask;
			MaskT new_mask = temp_mask & temp_mask - 1;
			this->written_mask = __rolq(new_mask, prev_write_index);
			size_t current_write_index = std::countr_zero<MaskT>(cleared_bit_mask) + prev_write_index + 1;
			if (current_write_index >= max_index) {
				current_write_index -= max_index;
			}
			this->current_write_index = current_write_index;
			this->values[current_write_index - 1] = value;
			ret = true;
		} else {
			ret = false;
		}
		this->release_lock();
		return ret;
	}
	gnu_noinline std::pair<T, bool> pop_value() {
		this->acquire_lock();
		//++read_count;
		std::pair<T, bool> ret;
		MaskT current_mask = this->written_mask;
		if (current_mask != initial_mask) {
			//++read_success_count;
			size_t prev_read_index = this->current_read_index;
			MaskT temp_mask = __rorq(this->written_mask, prev_read_index);
			MaskT set_bit_mask = ~temp_mask & temp_mask + 1;
			MaskT new_mask = temp_mask | temp_mask + 1;
			this->written_mask = __rolq(new_mask, prev_read_index);
			size_t current_read_index = std::countr_zero<MaskT>(set_bit_mask) + prev_read_index + 1;
			if (current_read_index >= max_index) {
				current_read_index -= max_index;
			}
			this->current_read_index = current_read_index;
			ret.first = this->values[current_read_index - 1];
			ret.second = true;
		} else {
			ret.second = false;
		}
		this->release_lock();
		return ret;
	}
};

extern bool bs_dll64();
dllexport void wasted_wtf_is_this();
dllexport void wasted_wtf_is_this2();
dllexport void wasted_wtf_is_this3();
extern void test_json_parse(const char16_t* raw_json);
extern void test_json5_parse(const wchar_t* raw_json);
dllexport volatile float wjhrjwrv;

std::atomic_uint32_t yeetus_value{ 0 };
std::atomic_uint32_t compare_array_index{ 0 };

static inline constexpr uint32_t thread_count = 1;
static inline constexpr uint32_t thread_iters = 16;

static inline uint32_t compare_array[thread_iters * thread_count];

CrappyLockedQueue<uint32_t, 4> test_jank2{};

*/


inline vec<float, 4> vectorcall read_float3(void* addr) {
	//vec<float, 4> ret = ;
	//ret[3] = 0.0f;
	//vec<float, 4> ret = { 0, 0, 0, 0 };
	//return shufflevec((vec<float, 4>) * (unaligned vec<float, 4>*)addr, ret, 0, 1, 2, 7);
	float* addr_read = (float*)addr;
	return (vec<float, 4>) { addr_read[0], addr_read[1], addr_read[2], 0.0f };
}

inline void vectorcall write_float3(void* addr, vec<float, 4> value) {
	//auto* write_addr = (unaligned vec<float, 4>*)addr;
	//*write_addr = shufflevec(value, (vec<float, 4>)*write_addr, 0, 1, 2, 7);
	float* addr_write = (float*)addr;
	addr_write[0] = value[0];
	addr_write[1] = value[1];
	addr_write[2] = value[2];
}

inline float vectorcall horizontal_add(vec<float, 4> value) {
	//__m128 t1 = _mm_movehl_ps(value, value);
	//__m128 t2 = _mm_add_ps(value, t1);
	//__m128 t3 = _mm_shuffle_ps(t2, t2, 1);
	//__m128 t4 = _mm_add_ss(t2, t3);
	//return _mm_cvtss_f32(t4);
	return value[0] + value[1] + value[2] + value[3];
}

inline float vectorcall dot_product(vec<float, 4> A, vec<float, 4> B) {
	return horizontal_add(A * B);
}

inline float vectorcall float3_length(vec<float, 4> value) {
	return __builtin_sqrtf(dot_product(value, value));
}

dllexport gnu_noinline float* stdcall normalize_float3(float* in, float* out) {
	vec<float, 4> value = read_float3(in);
	value /= float3_length(value);
	write_float3(out, value);
	return out;
}

struct Float3A {
	float x = 0.0f;
	float y = 0.7867742f;
	float z = 0.6172410f;
};
struct Float3B {
	float x = 0.0f;
	float y = 377.1200f;
	float z = 296.1600f;
};
dllexport Float3A float3a;
dllexport Float3B float3b;

constexpr auto wjherwejrv = PROCESS_ALL_ACCESS;

using PPS_CREATE_INFO = void*;
using PPS_ATTRIBUTE_LIST = void*;

NTSTATUS
NTAPI
NtCreateUserProcess(
	_Out_ PHANDLE ProcessHandle,
	_Out_ PHANDLE ThreadHandle,
	_In_ ACCESS_MASK ProcessDesiredAccess,
	_In_ ACCESS_MASK ThreadDesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ProcessObjectAttributes,
	_In_opt_ POBJECT_ATTRIBUTES ThreadObjectAttributes,
	_In_ ULONG ProcessFlags,
	_In_ ULONG ThreadFlags,
	_In_ PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
	_Inout_ PPS_CREATE_INFO CreateInfo,
	_In_ PPS_ATTRIBUTE_LIST AttributeList
);

using NtCreateUserProcessPtr = decltype(&NtCreateUserProcess);

#include "windows_structs.h"

#include "decent_rng.h"

static DecentRNG RNG;

dllexport gnu_noinline void generate_rtti_labels32(const wchar_t* path, const char* output_path, const char* input_path);

dllexport forceinline uint64_t query_performance_counter_fast() {
	uint64_t tick = rdtsc_serialize();
	//tick += USER_SHARED_DATAR().TscQpcBias;
	tick >>= USER_SHARED_DATAR().TscQpcShift;
	return tick;
}

#include <bit>

inline uint32_t __fastcall ilog2(uint32_t value) {
	__assume(value != 0);
	return std::bit_width(value) - 1;
}

dllexport gnu_noinline int32_t __fastcall old_isqrt(int32_t s) {
	auto error = s;
	uint32_t acc = 1;
	while (error >= 0) {
		error -= acc;
		acc += 2;
	}
	auto root_plus_1 = acc >> 1;
	if (root_plus_1 * root_plus_1 - root_plus_1 + 1 > 2) {
		return root_plus_1 - 1;
	}
	return root_plus_1;
}

dllexport gnu_noinline uint16_t __fastcall old_isqrt2(int32_t s) {
	int32_t error = s;
	uint32_t acc = 1;
	while (error >= 0) {
		error -= acc;
		acc += 2;
	}
	uint32_t root_plus_1 = acc >> 1;
	return root_plus_1 - (root_plus_1 * root_plus_1 - root_plus_1 + 1 > 2);
}

dllexport gnu_noinline uint16_t __fastcall old_isqrt3(int32_t s) {
	if (s > 0) {
		int32_t error = s;
		uint32_t acc = 1;
		while (error >= 0) {
			error -= acc;
			acc += 2;
		}
		uint32_t root_plus_1 = acc >> 1;
		return root_plus_1 - (root_plus_1 * root_plus_1 - root_plus_1 + 1 > 2);
	} else {
		return 0;
	}
}

dllexport gnu_noinline int32_t __fastcall isqrt(int32_t s) {
	if (s <= 0) {
		return 0;
	}
	auto error = s;
	decltype(s) root = 0;

	std::make_unsigned<decltype(s)>::type d = (1 << ((sizeof(s) * 8) - 2));
	while (d > s) {
		d >>= 2;
	}

	while (d != 0) {
		if (error >= (root + d)) {
			error -= (root + d);
			root = ((root >> 1) + d);
		} else {
			root >>= 1;
		}
		d >>= 2;
	}

	if (error > root) {
		return root + 1;
	}
	return root;
}

dllexport gnu_noinline uint16_t __fastcall isqrt2(uint32_t s) {

	uint32_t root = 0;
	if (s) {
		uint32_t error = s;

		uint32_t shift_count = ilog2(s) & ~1;
		uint32_t d = 1 << shift_count;

		while (d) {
			uint32_t root_pd = root + d;
			root >>= 1;
			if (error >= root_pd) {
				error -= root_pd;
				root += d;
			}
			d >>= 2;
		}
		root += error > root;
	}
	return root;
}

// Based on https://math.stackexchange.com/questions/2469446/what-is-a-fast-algorithm-for-finding-the-integer-square-root
dllexport gnu_noinline uint16_t __fastcall fast_sqrt(uint32_t x) {
	if (x > 0) {
		uint8_t y_log2 = ilog2(x) >> 1;
		uint32_t y = 1 << y_log2;
		uint32_t diff = x - y * y;
		// Perform lerp between powers of four
		y += diff / 3 >> y_log2;
		// The estimate is probably too low, refine it upward
		y += (x / y - y) >> 1;
		if (x < y * y) {
			y += (x / y - y) >> 1;
			// The estimate may still be 1 too high
			y -= x < y * y;
		}
		return y;
	}
	return 0;
}

dllexport gnu_noinline uint16_t __fastcall fast_sqrt_prev(uint32_t x) {
	if (x > 1) {
		uint8_t y_log2 = ilog2(x) >> 1;
		uint_fast16_t y = 1 << y_log2;
		uint32_t diff = x - y * y;
		// Perform lerp between powers of four
		y += diff / 3 >> y_log2;
		// The estimate is probably too low, refine it upward
		y += (x / y - y) >> 1;
		if (x < y * y) {
			y += (x / y - y) >> 1;
			// The estimate may still be 1 too high
			y -= x < y * y;
		}
		return y;
	}
	return x;
}

dllexport gnu_noinline uint16_t __fastcall fast_sqrt2(uint32_t x) {
	
	if (x >= 64) {
		uint8_t y_log2 = ilog2(x) >> 1;
		uint_fast16_t y = 1 << y_log2;
		uint32_t diff = x - y * y;

		// Perform lerp between powers of four
		y += diff / 3 >> y_log2;
		// The estimate is probably too low, refine it upward
		y += (x / y - y) >> 1;
		// The estimate may be too high. If so, refine it downward
		if (x < y * y) {
			y += (x / y - y) >> 1;
			// The estimate may still be 1 too high
			y -= x < y * y;
		}
		return y;
	} else {
		cache_align static constexpr uint8_t low_value_lookup[] = {
			0, 1, 1, 1, 2, 2, 2, 2,
			2, 3, 3, 3, 3, 3, 3, 3,
			4, 4, 4, 4, 4, 4, 4, 4,
			4, 5, 5, 5, 5, 5, 5, 5,
			5, 5, 5, 5, 6, 6, 6, 6,
			6, 6, 6, 6, 6, 6, 6, 6,
			6, 7, 7, 7, 7, 7, 7, 7,
			7, 7, 7, 7, 7, 7, 7, 7
		};
		return low_value_lookup[x];
	}
}

dllexport gnu_noinline uint16_t __fastcall fast_sqrt3(int32_t value) {
	
	if (value > 120) {
		uint32_t x = value;
		uint8_t y_log2 = ilog2(x) >> 1;
		uint_fast16_t y = 1 << y_log2;
		uint32_t diff = x - y * y;

		// Perform lerp between powers of four
		y += diff / 3 >> y_log2;
		// The estimate is probably too low, refine it upward
		y += (x / y - y) >> 1;
		// The estimate may be too high. If so, refine it downward
		if (x < y * y) {
			y += (x / y - y) >> 1;
			// The estimate may still be 1 too high
			y -= x < y * y;
		}
		return y;
	} else if (value > 0) {
		uint32_t acc = 1;
		do {
			value -= acc;
			acc += 2;
		} while (value >= 0);
		uint32_t y = acc >> 1;
		return y - (y * y - y + 1 > 2);
	} else {
		return 0;
	}
}

dllexport gnu_noinline uint16_t __fastcall fast_sqrt4(uint32_t x) {
	if (x > 1) {
		uint8_t y_log2 = ilog2(x) >> 1;
		uint32_t y = 1 << y_log2;
		uint32_t diff = x - y * y;
		// Perform lerp between powers of four
		y += diff / 3 >> y_log2;
		// The estimate is probably too low, refine it upward
		y += (x / y - y) >> 1;
		if (x < y * y) {
			y += (x / y - y) >> 1;
			// The estimate may still be 1 too high
			y -= x < y * y;
		}
		return y;
	}
	return x;
}

cache_align static constexpr uint8_t java_sqrt_table[] = {
	 0,    16,  22,  27,  32,  35,  39,  42,  45,  48,  50,  53,  55,  57,
	 59,   61,  64,  65,  67,  69,  71,  73,  75,  76,  78,  80,  81,  83,
	 84,   86,  87,  89,  90,  91,  93,  94,  96,  97,  98,  99, 101, 102,
	 103, 104, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 118,
	 119, 120, 121, 122, 123, 124, 125, 126, 128, 128, 129, 130, 131, 132,
	 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145,
	 146, 147, 148, 149, 150, 150, 151, 152, 153, 154, 155, 155, 156, 157,
	 158, 159, 160, 160, 161, 162, 163, 163, 164, 165, 166, 167, 167, 168,
	 169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176, 177, 178, 178,
	 179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 187, 187, 188,
	 189, 189, 190, 191, 192, 192, 193, 193, 194, 195, 195, 196, 197, 197,
	 198, 199, 199, 200, 201, 201, 202, 203, 203, 204, 204, 205, 206, 206,
	 207, 208, 208, 209, 209, 210, 211, 211, 212, 212, 213, 214, 214, 215,
	 215, 216, 217, 217, 218, 218, 219, 219, 220, 221, 221, 222, 222, 223,
	 224, 224, 225, 225, 226, 226, 227, 227, 228, 229, 229, 230, 230, 231,
	 231, 232, 232, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238,
	 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246,
	 246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253,
	 253, 254, 254, 255
};

dllexport gnu_noinline uint16_t __fastcall java_sqrt_test(uint32_t x) {

	

	uint32_t xn;

	switch (x) {
		default:
			unreachable;
		case 0x00000000 ... 0x000000FF: // 32, 31, 30, 29, 28, 27, 26, 25, 24
			xn = java_sqrt_table[x] >> 4; // 0, 0
			goto no_adjust;
		case 0x00000100 ... 0x000003FF: // 23, 22
			xn = (java_sqrt_table[x >> 2] >> 3) + 1; // 2, 1
			goto single_adjust;
		case 0x00000400 ... 0x00000FFF: // 21, 20
			xn = (java_sqrt_table[x >> 4] >> 2) + 1; // 4, 2
			goto single_adjust;
		case 0x00001000 ... 0x00003FFF: // 19, 18
			xn = (java_sqrt_table[x >> 6] >> 1) + 1; // 6, 3
			goto single_adjust;
		case 0x00004000 ... 0x0000FFFF: // 17, 16
			xn = java_sqrt_table[x >> 8] + 1; // 8, 4
			goto single_adjust;
		case 0x00010000 ... 0x0003FFFF: // 15, 14
			xn = java_sqrt_table[x >> 10] << 1; // 10, 5
			goto double_adjust;
		case 0x00040000 ... 0x000FFFFF: // 13, 12
			xn = java_sqrt_table[x >> 12] << 2; // 12, 6
			goto double_adjust;
		case 0x00100000 ... 0x003FFFFF: // 11, 10
			xn = java_sqrt_table[x >> 14] << 3; // 14, 7
			goto double_adjust;
		case 0x00400000 ... 0x00FFFFFF: // 9, 8
			xn = java_sqrt_table[x >> 16] << 4; // 16, 8
			goto double_adjust;
		case 0x01000000 ... 0x03FFFFFF: // 7, 6
			xn = java_sqrt_table[x >> 18] << 5; // 18, 9
			goto triple_adjust;
		case 0x04000000 ... 0x0FFFFFFF: // 5, 4
			xn = java_sqrt_table[x >> 20] << 6; // 20, 10
			goto triple_adjust;
		case 0x10000000 ... 0x3FFFFFFF: // 3, 2
			xn = java_sqrt_table[x >> 22] << 7; // 22, 11
			goto triple_adjust;
		case 0x40000000 ... 0xFFFFFFFF: // 1, 0
			xn = java_sqrt_table[x >> 24] << 8; // 24, 12
			goto triple_adjust;
	}

triple_adjust:
	xn = (xn + 1 + (x / xn)) >> 1;
double_adjust:
	xn = (xn + 1 + (x / xn)) >> 1;
single_adjust:
	xn -= x < xn * xn;
no_adjust:
	return xn;
}



dllexport gnu_noinline uint16_t __fastcall new_fast_sqrt(uint32_t x) {

	uint8_t shift_count = std::bit_width(x >> 7) >> 1;

	uint32_t y = java_sqrt_table[x >> shift_count * 2];
	y = (y << shift_count) >> 4;
	y += 0b11110 >> shift_count & 1;

	switch (shift_count) {
		default:
			unreachable;
			//__asm int3
		case 9: case 10: case 11: case 12:
			y = y + x / y >> 1;
		case 5: case 6: case 7: case 8:
			y = y + x / y >> 1;
		case 1: case 2: case 3: case 4:
			y -= x < y * y;
		case 0:
			return y;
	}
}

dllexport gnu_noinline uint16_t __fastcall new_fast_sqrt2(uint32_t x) {

	uint8_t shift_count = std::bit_width(x >> 7) >> 1;

	uint32_t y = java_sqrt_table[x >> shift_count * 2];
	y = (y << shift_count) >> 4;
	y += 0b11110 >> shift_count & 1;

	if (shift_count > 0) {
		if (shift_count > 4) {
			if (shift_count > 8) {
				y = y + x / y >> 1;
			}
			y = y + x / y >> 1;
		}
		y -= x < y * y;
	}
	return y;
}

dllexport gnu_noinline uint16_t __fastcall new_fast_sqrt3(uint32_t x) {

	uint8_t shift_count = std::bit_width(x >> 7) >> 1;

	uint32_t y = java_sqrt_table[x >> shift_count * 2];
	y = (uint16_t)__rold(y, shift_count - 4);
	y += 0b11110 >> shift_count & 1;

	if (x > 0xFF) {
		if (x > 0xFFFF) {
			if (x > 0xFFFFFF) {
				y = y + x / y >> 1;
			}
			y = y + x / y >> 1;
		}
		y -= x < y * y;
	}
	return y;
}

dllexport gnu_noinline uint16_t __fastcall float_sqrt(int32_t x) {
	if (x > 0) {
		return __builtin_sqrt(x);
	} else {
		return 0;
	}
}

dllexport gnu_noinline void wwhrvwjhrvjwhr() {
	__asm__ volatile (
		".byte 0x66, 0x66, 0x0F, 0x1F, 0x00"
	);
}

dllexport gnu_noinline void kjwbkwjrbwkjr() {
	__asm__ volatile (
		".byte 0x66, 0x2E, 0x83, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00"
	);
}


dllexport gnu_noinline void time_fix_test(const char* str, char* buffer) {
	uint8_t str_chars[8];
	__builtin_memcpy(str_chars, str, 8);
	for (size_t i = 0; i < countof(str_chars); ++i) {
		str_chars[i] -= '0';
	}
	tm time = {};
	time.tm_mon = str_chars[1] + str_chars[0] * 10;
	time.tm_mday = str_chars[4] + str_chars[3] * 10;
	time.tm_year = str_chars[7] + str_chars[6] * 10 + 2000;
	strftime(buffer, 9, "", &time);
}

dllexport gnu_noinline void __fastcall time_fix_test2(uint8_t* str) {
	vec<uint16_t, 8> str_chars = { str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7] };
	str_chars[5] = str[3];
	//str_chars = shufflevec(str_chars, str_chars, 0, 1, -1, -1, 3, 4, 6, 7);
	str_chars += (vec<uint16_t, 8>){ -'0', -'0', -'0', -'0', -'0', -'0', -'0', -'0' };
	const vec<uint16_t, 8> multipliers = { 1, 10, 0, 0, 10, 1, 1, 10 };
	vec<uint32_t, 4> time_out = (vec<uint32_t, 4>)_mm_madd_epi16((__m128i)str_chars, (__m128i)multipliers);
	tm time = {};
	time.tm_mon = time_out[0];
	time.tm_mday = time_out[2];
	time.tm_year = time_out[3];
	strftime((char*)str, 9, "", &time);
}

#if !IS_X64
dllexport gnu_noinline void __fastcall time_fix_test3(uint8_t* str) {
	tm time = {};
	time.tm_mon = aad_math(PackUInt16(str[1], str[0]) - PackUInt16('0', '0'));
	time.tm_mday = aad_math(PackUInt16(str[4], str[3]) - PackUInt16('0', '0'));
	time.tm_year = aad_math(PackUInt16(str[7], str[6]) - PackUInt16('0', '0')) + 2000;
	strftime((char*)str, 9, "", &time);
}
#endif

dllexport int64_t stdcall alldiv_fast(int64_t divisor, int64_t dividend) {
	return dividend / divisor;
}
dllexport int64_t stdcall allrem_fast(int64_t divisor, int64_t dividend) {
	return dividend % divisor;
}
dllexport uint64_t stdcall aulldiv_fast(uint64_t divisor, uint64_t dividend) {
	return dividend / divisor;
}
dllexport uint64_t stdcall aullrem_fast(uint64_t divisor, uint64_t dividend) {
	return dividend % divisor;
}
dllexport auto regparm(3) alldvrm_fast(int, int, int, int64_t divisor, int64_t dividend) {
	int64_t quotient = dividend / divisor;
	int64_t remainder = dividend % divisor;
	vec<int64_t, 2> temp = { quotient , remainder };
	return temp;
}
dllexport auto regparm(3) aulldvrm_fast(int, int, int, uint64_t divisor, uint64_t dividend) {
	uint64_t quotient = dividend / divisor;
	uint64_t remainder = dividend % divisor;
	vec<uint64_t, 2> temp = { quotient , remainder };
	return temp;
}
dllexport int64_t stdcall allmul_fast(int64_t lhs, int64_t rhs) {
	return lhs * rhs;
}
dllexport uint64_t stdcall aullmul_fast(uint64_t lhs, uint64_t rhs) {
	return lhs * rhs;
}
dllexport int64_t regparm(3) allshl_fast(int64_t value, uint8_t shift) {
	if (expect_chance(value < 64, true, 1.0)) {
		return value << shift;
	} else {
		return 0;
	}
}
dllexport int64_t regparm(3) allshr_fast(int64_t value, uint8_t shift) {
	if (expect_chance(value < 64, true, 1.0)) {
		return value >> shift;
	} else {
		return value < 0 ? -1ll : 0ll;
	}
}
dllexport uint64_t regparm(3) aullshr_fast(uint64_t value, uint8_t shift) {
	if (expect_chance(value < 64, true, 1.0)) {
		return value >> shift;
	} else {
		return 0;
	}
}

gnu_noinline void test_sqrt_thing() {

#define include_old_sqrt 0

#define include_crap_roots 1

#define include_known_worse 0

#define test_single_value 0

#if test_single_value
#define test_val_declare volatile int yeet = test_sqrt_value
#define test_val yeet
#else
#define test_val_declare
#define test_val i
#endif

#define test_sqrt_value 121

#if include_crap_roots
	uint64_t start_time_A = rdtsc_serialize();
	{
		int32_t i = 1;
		do {
			test_val_declare;
			__asm__(
				""
				:
				: "X"(float_sqrt(test_val))
			);
		} while (++i > 0);
	}
	uint64_t end_time_A = rdtsc_serialize();

#if include_old_sqrt
	uint64_t start_time_B = rdtsc_serialize();
	{
		int32_t i = 1;
		do {
			test_val_declare;
			__asm__(
				""
				:
				: "X"(old_isqrt3(test_val))
			);
		} while (++i > 0);
	}
	uint64_t end_time_B = rdtsc_serialize();
#endif

#if include_known_worse
	uint64_t start_time_C = rdtsc_serialize();
	{
		int32_t i = 1;
		do {
			test_val_declare;
			__asm__(
				""
				:
				: "X"(isqrt(test_val))
			);
		} while (++i > 0);
	}
	uint64_t end_time_C = rdtsc_serialize();
#endif

	uint64_t start_time_D = rdtsc_serialize();
	{
		int32_t i = 1;
		do {
			test_val_declare;
			__asm__(
				""
				:
				: "X"(isqrt2(test_val))
			);
		} while (++i > 0);
	}
	uint64_t end_time_D = rdtsc_serialize();
#endif

	uint64_t start_time_E = rdtsc_serialize();
	{
		int32_t i = 1;
		do {
			test_val_declare;
			__asm__(
				""
				:
				: "X"(fast_sqrt(test_val))
			);
		} while (++i > 0);
	}
	uint64_t end_time_E = rdtsc_serialize();

#if include_crap_roots
	uint64_t start_time_F = rdtsc_serialize();
	{
		int32_t i = 1;
		do {
			test_val_declare;
			__asm__(
				""
				:
				: "X"(fast_sqrt2(test_val))
			);
		} while (++i > 0);
	}
	uint64_t end_time_F = rdtsc_serialize();
#endif
	
	
	uint64_t start_time_G = rdtsc_serialize();
	{
		int32_t i = 1;
		do {
			test_val_declare;
			__asm__(
				""
				:
				: "X"(fast_sqrt_prev(test_val))
			);
		} while (++i > 0);
	}
	uint64_t end_time_G = rdtsc_serialize();
	
	

	printf(
#if include_crap_roots
		"Double Precision sqrt:  %llu\n"
#if include_old_sqrt
		"Old isqrt:              %llu\n"
#else
		"Old isqrt:              lol no\n"
#endif
#if include_known_worse
		"ReC98 isqrt:            %llu\n"
#endif
		"ReC98 isqrt Optimized:  %llu\n"
#endif
		"Test isqrt:             %llu\n"
#if include_crap_roots
		"Test isqrt + lookup:    %llu\n"
#endif
		"Test isqrt old:         %llu\n"
#if include_crap_roots
		, end_time_A - start_time_A
#if include_old_sqrt
		, end_time_B - start_time_B
#endif
#if include_known_worse
		, end_time_C - start_time_C
#endif
		, end_time_D - start_time_D
#endif
		, end_time_E - start_time_E
#if include_crap_roots
		, end_time_F - start_time_F
#endif
		, end_time_G - start_time_G
	);
}

#define _HAS_CXX17 1
//#include <filesystem>
#include <string>
#include <optional>
//#include <string_view>

//static inline constexpr size_t kjwebkwrb = sizeof_template_impl<std::wstring>();

bool dumb_x64_hack() {
	//std::string
}

#if !IS_X64
#pragma comment (lib, "onecore.lib")
#endif

void thcrap_inject_test();

void x64_inject_test();

dllexport void say_hi() {
	MessageBoxA(NULL, "hi from x64", "thcrap_inject_test", 0);
}
dllexport void say_bye() {
	MessageBoxA(NULL, "injection_failed", "thcrap_inject_test", 0);
}

void make_shift_jis_conversion_table();

gnu_noinline void segment_jank_test();

gnu_noinline void atomic_timing_test();

gnu_noinline void loop_timing_test();

extern bool dns_connect(const wchar_t* server_name, const wchar_t* port_str);

#include <conio.h>

dllexport void tic_tac_bs();

uint32_t derpy_rng_value = rand();
dllexport uint32_t derp_rng() {
	uint64_t temp = 0xBC8Full * derpy_rng_value;
	//temp %= INT32_MAX;
	uint32_t ret;
	//_udiv64(temp, INT32_MAX, &ret);
	//return ret;
	derpy_rng_value = ret;
	return ret;
}

//constexpr auto wkrjbb = __builtin_offsetof(CONTEXT, LastBranchToRip);

#ifndef STATUS_NOT_IMPLEMENTED
#define STATUS_NOT_IMPLEMENTED				((DWORD)0xC0000002L)
#endif
#ifndef STATUS_INVALID_LOCK_SEQUENCE
#define STATUS_INVALID_LOCK_SEQUENCE		((DWORD)0xC000001EL)
#endif
#ifndef STATUS_BAD_STACK
#define STATUS_BAD_STACK					((DWORD)0xC0000028L)
#endif
#ifndef STATUS_INVALID_UNWIND_TARGET
#define STATUS_INVALID_UNWIND_TARGET		((DWORD)0xC0000029L)
#endif
#ifndef STATUS_BAD_FUNCTION_TABLE
#define STATUS_BAD_FUNCTION_TABLE			((DWORD)0xC00000FFL)
#endif
#ifndef STATUS_DATATYPE_MISALIGNMENT_ERROR
#define STATUS_DATATYPE_MISALIGNMENT_ERROR	((DWORD)0xC00002C5L)
#endif
#ifndef STATUS_HEAP_CORRUPTION
#define STATUS_HEAP_CORRUPTION				((DWORD)0xC0000374L)
#endif

//#include <ehdata.h>

// These defines are from internal CRT header ehdata.h
// MSVC++ EH exception number
#ifndef EH_EXCEPTION_NUMBER
#define EH_EXCEPTION_NUMBER 0xE06D7363 // ('msc' | 0xE0000000)
#endif
#ifndef EH_MAGIC_NUMBER1
#define EH_MAGIC_NUMBER1 0x19930520
#endif

#if !__x86_64__
//#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll32.lib")
#else
#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")
#endif

NTSTATUS NTAPI NtGetContextThread(HANDLE ThreadHandle, PCONTEXT pContext);

using NtGetContextThread_t = decltype(NtGetContextThread);

NtGetContextThread_t* NtGetContextThread_ptr;

static uint64_t last_branch_to;
static uint64_t last_branch_from;
static uint64_t last_exception_to;
static uint64_t last_exception_from;

LONG WINAPI log_branch_records(LPEXCEPTION_POINTERS lpEI) {
#if __x86_64__
	last_branch_to = lpEI->ContextRecord->LastBranchToRip;
	last_branch_from = lpEI->ContextRecord->LastBranchFromRip;
	last_exception_to = lpEI->ContextRecord->LastExceptionToRip;
	last_exception_from = lpEI->ContextRecord->LastExceptionFromRip;

	lpEI->ContextRecord->Rip += 7;
#else
	CONTEXTX<64> x64_context;
	x64_context.ContextFlags = 0x0010001F;
	NtGetContextThread_ptr(GetCurrentThread(), (PCONTEXT)&x64_context);

	last_branch_to = x64_context.LastBranchToRip;
	last_branch_from = x64_context.LastBranchFromRip;
	last_exception_to = x64_context.LastExceptionToRip;
	last_exception_from = x64_context.LastExceptionFromRip;

	lpEI->ContextRecord->Eip += 7;
#endif

	return EXCEPTION_CONTINUE_EXECUTION;
}

gnu_noinline void debug_test_func() {
bs_label_testA:
	__asm__ volatile(
		"jmp 0f \n"
		"int3 \n"
	"0: \n"
		"nop"
	);
bs_label_testB:
	__asm {
		_emit 0x8B
		_emit 0x04
		_emit 0x25
		_emit 0
		_emit 0
		_emit 0
		_emit 0
	}

	printf(
		"BranchTo:   %llX\n"
		"BranchFrom: %llX\n"
		"ExceptTo:   %llX\n"
		"ExceptFrom: %llX\n"
		"LabelTestA: %zX\n"
		"LabelTestB: %zX\n"
		, last_branch_to
		, last_branch_from
		, last_exception_to
		, last_exception_from
		, &&bs_label_testA
		, &&bs_label_testB
	);
}

template <bool is_rex>
uint8_t get_random_shift_count(uint8_t count) {
	unsigned int random;
	rand_s(&random);
	if constexpr (!is_rex) {
		return (random & 7) << 5 | count;
	} else {
		return (random & 3) << 6 | count;
	}
}

template <typename T>
gnu_noinline uint8_t* replace_imm_with_bs(T value, uint8_t* code);

uint8_t giga_buffer[65500];

template <typename T>
void print_imm_with_bs(T value) {
	printf("\n.byte ");

	uint8_t* code_ptr = replace_imm_with_bs(value, giga_buffer);
	for (size_t i = 0; i < code_ptr - giga_buffer; ++i) {
		printf("0x%02X,", giga_buffer[i]);
	}
}


struct WorstZUNRng {
	uint32_t seed;
	uint32_t count;

	dllexport gnu_noinline uint32_t __thiscall this_is_awful() {
		uint32_t immediate4;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0x0F,0x77,0xDB,0xE2,0xDF,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xD9,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xD8,0xE3,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE2,0x0F,0x77,0xDD,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0x0F,0x77,0xDB,0xE2,0xD9,0xED,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xDF,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xD9,0xDC,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xD9,0xF7,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDF,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xD9,0xDA,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE2,0x0F,0x77,0xD9,0xD8,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE2,0x0F,0x77,0xD9,0xD9,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xDB,0xE0,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xD9,0xE5,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xDF,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xD9,0xE9,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE3,0xD9,0xDB,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xDF,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDD,0xC3,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xD9,0xDF,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xDD,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xDB,0xE4,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xD9,0xEA,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xDD,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xDF,0xC3,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE2,0x0F,0x77,0xDD,0xC0,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xDD,0xC3,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0x0F,0x77,0xDB,0xE2,0xD9,0xDB,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0x0F,0x77,0xDB,0xE2,0xDF,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xDF,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xD9,0xE5,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xD9,0xDF,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0x89,0xD0,0xD9,0x2C,0x24,0x5A"
			: "=a"(immediate4)
			:
			: "edx"
		);
		uint32_t immediate2A;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0xDB,0xE3,0xDF,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xD8,0xD1,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDF,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE2,0x0F,0x77,0xD9,0xEB,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDF,0xC1,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE2,0x0F,0x77,0xD9,0xDC,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xDF,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xD9,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xDB,0xE4,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xDB,0xE1,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xDF,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xD9,0xF7,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDB,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE2,0x0F,0x77,0xDD,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xD9,0xDE,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xD9,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0x0F,0x77,0xDB,0xE2,0xDD,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xD9,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xD9,0xD0,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xDD,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xDB,0xE4,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xDD,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE2,0x0F,0x77,0xD9,0xE9,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xDB,0xE0,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDF,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xDB,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xD9,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xD9,0xF6,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0x0F,0x77,0xDB,0xE2,0xD9,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE2,0x0F,0x77,0xD9,0xDC,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE2,0x0F,0x77,0xD9,0xED,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0x0F,0x77,0xDB,0xE2,0xDF,0xC7,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0x89,0xD0,0xD9,0x2C,0x24,0x5A"
			: "=a"(immediate2A)
			:
			: "edx"
		);
		*based_pointer<uint32_t>(this, immediate4) += immediate2A;
		uint32_t rng_const1;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0x0F,0x77,0xDB,0xE2,0xDF,0xC3,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE2,0x0F,0x77,0xDD,0xC7,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xD9,0xD0,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE2,0x0F,0x77,0xDF,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xDC,0xE1,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0x0F,0x77,0xDB,0xE2,0xDB,0xF1,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDD,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xD9,0xE9,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xDB,0xE1,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0x0F,0x77,0xDB,0xE2,0xD8,0x1C,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xDE,0x24,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE3,0xDF,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDA,0xC0,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDD,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0x0F,0x77,0xDB,0xE2,0xD9,0xD9,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xDF,0xD8,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDC,0xD3,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE3,0xDD,0xEA,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xDE,0xF2,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDC,0xFD,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDE,0xD3,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xDE,0xF0,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDE,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xD8,0xEE,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xDA,0xC8,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xDB,0xC8,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0x0F,0x77,0xDB,0xE2,0xDF,0xDC,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xDE,0x0C,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0x0F,0x77,0xDB,0xE2,0xDC,0xED,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE2,0x0F,0x77,0xDC,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0x0F,0x77,0xDB,0xE2,0xDF,0xF1,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0x0F,0x77,0xDB,0xE2,0xDD,0xED,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0x89,0xD0,0xD9,0x2C,0x24,0x5A"
			: "=a"(rng_const1)
			:
			: "edx"
		);
		rng_const1 ^= this->seed;
		uint32_t rng_const2;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0x0F,0x77,0xDB,0xE2,0xD8,0xC7,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xD9,0xE5,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xDB,0xC0,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE2,0x0F,0x77,0xD8,0xF2,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xD9,0xEE,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE2,0x0F,0x77,0xDF,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xD9,0xEA,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDA,0xDB,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE2,0x0F,0x77,0xDB,0xE0,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xDA,0xDC,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE3,0xDF,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xDA,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xDC,0xE8,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xD9,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE2,0x0F,0x77,0xD9,0xE5,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0x0F,0x77,0xDB,0xE2,0xDC,0xE3,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xD8,0xCD,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xD8,0xF6,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDB,0xC7,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xD9,0xCD,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xD8,0xFB,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDE,0xD4,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE2,0x0F,0x77,0xDA,0xD8,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE3,0xDA,0xCF,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE2,0x0F,0x77,0xDD,0xE5,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0x0F,0x77,0xDB,0xE2,0xDE,0xCE,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE2,0x0F,0x77,0xDE,0xCA,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xD8,0xC7,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xD8,0xCF,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE2,0x0F,0x77,0xD8,0xE2,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDF,0xE8,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xD8,0xD3,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x89,0xD0,0xD9,0x2C,0x24,0x5A"
			: "=a"(rng_const2)
			:
			: "edx"
		);
		rng_const1 += rng_const2;
		uint8_t shift_const1;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0xDB,0xE3,0xDB,0xE0,0xDF,0xE0,0xC1,0xC8,0x41,0xDB,0xE3,0xDE,0xF3,0xDF,0xE0,0xC1,0xC8,0xC1,0xDB,0xE2,0x0F,0x77,0xDD,0xC3,0xDF,0xE0,0xC1,0xC8,0xA1,0xDB,0xE3,0xD9,0xE5,0xDF,0xE0,0xC1,0xC8,0x21,0xDB,0xE3,0xDF,0xC1,0xDF,0xE0,0xC1,0xC8,0xE1,0xDB,0xE2,0x0F,0x77,0xD9,0xF7,0xDF,0xE0,0xC1,0xC8,0xA1,0x0F,0x77,0xDB,0xE2,0xDF,0xC3,0xDF,0xE0,0xC1,0xC8,0xE1,0x0F,0x77,0xDB,0xE2,0xD9,0xDA,0xDF,0xE0,0xC1,0xC8,0xE1,0xC1,0xF8,0x98,0xD9,0x2C,0x24,0x5A"
			: "=a"(shift_const1)
			:
			: "edx"
		);
		uint32_t temp1 = rng_const1;
		*(uint16_t*)&temp1 = __rolw(temp1, shift_const1);
		uint32_t rng_const3;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0xDB,0xE2,0x0F,0x77,0xDF,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xDD,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xD9,0xEB,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xDF,0xC0,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE2,0x0F,0x77,0xDC,0xC6,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0x0F,0x77,0xDB,0xE2,0xDA,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xD9,0xED,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE2,0x0F,0x77,0xDF,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xD9,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE2,0x0F,0x77,0xDB,0xC0,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xDB,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xD9,0xD9,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xD9,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xD9,0xDE,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE2,0x0F,0x77,0xDB,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE2,0x0F,0x77,0xDE,0x04,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xDC,0xC1,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xD9,0xCF,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDE,0xF6,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xD8,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDC,0xF5,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xDF,0xD9,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xDC,0xF6,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0x0F,0x77,0xDB,0xE2,0xDA,0x34,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xDF,0xEA,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDF,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE2,0x0F,0x77,0xDC,0xD4,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE3,0xDA,0xCF,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xD8,0xC9,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDC,0xF5,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE3,0xDB,0xC0,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xDB,0xEF,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0x89,0xD0,0xD9,0x2C,0x24,0x5A"
			: "=a"(rng_const3)
			:
			: "edx"
		);
		temp1 ^= rng_const3;
		uint32_t rng_const4;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0xDB,0xE2,0x0F,0x77,0xD9,0xCE,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0xDB,0xE3,0xD9,0xEC,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE2,0x0F,0x77,0xDF,0xCF,0xDF,0xE0,0x0F,0xAC,0xC2,0xE1,0x0F,0x77,0xDB,0xE2,0xD8,0xC1,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0x0F,0x77,0xDB,0xE2,0xDF,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDD,0xD6,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE2,0x0F,0x77,0xDF,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDB,0xD7,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xDF,0xC7,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE2,0x0F,0x77,0xDC,0xFD,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDF,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xDE,0x2C,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDC,0xC2,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE3,0xD9,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xDF,0xC5,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE3,0xDA,0x34,0x24,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDB,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0x0F,0x77,0xDB,0xE2,0xDD,0xEF,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0xDB,0xE3,0xDA,0xC9,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0x0F,0x77,0xDB,0xE2,0xDB,0xC0,0xDF,0xE0,0x0F,0xAC,0xC2,0x61,0xDB,0xE3,0xDE,0xE2,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xDB,0xC4,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0xDB,0xE2,0x0F,0x77,0xDB,0xDD,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0xDB,0xE2,0x0F,0x77,0xD8,0xDF,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0x0F,0x77,0xDB,0xE2,0xD9,0xE1,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDC,0xD2,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0x0F,0x77,0xDB,0xE2,0xDD,0xEA,0xDF,0xE0,0x0F,0xAC,0xC2,0xA1,0xDB,0xE3,0xDF,0xCF,0xDF,0xE0,0x0F,0xAC,0xC2,0x41,0x0F,0x77,0xDB,0xE2,0xDB,0xC1,0xDF,0xE0,0x0F,0xAC,0xC2,0x01,0x0F,0x77,0xDB,0xE2,0xDD,0xD5,0xDF,0xE0,0x0F,0xAC,0xC2,0x81,0xDB,0xE2,0x0F,0x77,0xDC,0xCE,0xDF,0xE0,0x0F,0xAC,0xC2,0xC1,0xDB,0xE3,0xD8,0xEB,0xDF,0xE0,0x0F,0xAC,0xC2,0x21,0x89,0xD0,0xD9,0x2C,0x24,0x5A"
			: "=a"(rng_const4)
			:
			: "edx"
		);
		temp1 += rng_const4;
		uint8_t shift_const2;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0xDB,0xE2,0x0F,0x77,0xD9,0xED,0xDF,0xE0,0xC1,0xC8,0x61,0xDB,0xE2,0x0F,0x77,0xDB,0xEE,0xDF,0xE0,0xC1,0xC8,0xC1,0x0F,0x77,0xDB,0xE2,0xDF,0xC5,0xDF,0xE0,0xC1,0xC8,0x81,0xDB,0xE2,0x0F,0x77,0xDF,0xC2,0xDF,0xE0,0xC1,0xC8,0x81,0xDB,0xE2,0x0F,0x77,0xDF,0xC6,0xDF,0xE0,0xC1,0xC8,0x61,0xDB,0xE2,0x0F,0x77,0xDF,0xDA,0xDF,0xE0,0xC1,0xC8,0x81,0xDB,0xE3,0xD8,0x2C,0x24,0xDF,0xE0,0xC1,0xC8,0xC1,0xDB,0xE3,0xD8,0xFE,0xDF,0xE0,0xC1,0xC8,0x21,0xC1,0xF8,0x58,0xD9,0x2C,0x24,0x5A"
			: "=a"(shift_const2)
			:
			: "edx"
		);
		*(uint16_t*)&this->seed = __rolw(temp1, shift_const2);
		uint8_t shift_const3;
		__asm__(
			".byte 0x0F,0xA0,0xD9,0x3C,0x24,0x0F,0x77,0xDB,0xE2,0xDF,0xC5,0xDF,0xE0,0xC1,0xC8,0xE1,0xDB,0xE3,0xD9,0xF6,0xDF,0xE0,0xC1,0xC8,0xA1,0xDB,0xE3,0xDD,0xC7,0xDF,0xE0,0xC1,0xC8,0xE1,0xDB,0xE3,0xDD,0xC0,0xDF,0xE0,0xC1,0xC8,0xC1,0xDB,0xE3,0xDE,0xC8,0xDF,0xE0,0xC1,0xC8,0x81,0xDB,0xE2,0x0F,0x77,0xDB,0x04,0x24,0xDF,0xE0,0xC1,0xC8,0x81,0xDB,0xE2,0x0F,0x77,0xDC,0xDB,0xDF,0xE0,0xC1,0xC8,0x41,0xDB,0xE2,0x0F,0x77,0xD9,0xEB,0xDF,0xE0,0xC1,0xC8,0x21,0xC1,0xF8,0x58,0xD9,0x2C,0x24,0x5A"
			: "=a"(shift_const3)
			:
			: "edx"
		);
		return (uint16_t)temp1 | rng_const1 << shift_const3;
	}
};

struct ZUNRng {
	uint16_t value; // 0x0
	uint32_t index; // 0x4
	// 0x8

	// 0x402740
	dllexport gnu_noinline uint32_t thiscall rand_uint() {
		uint16_t tempA, tempB;
		{
			this->index += 2;
			tempA = (this->value ^ 0x9630) - 0x6553;
			tempB = ((tempA & 0xC000) >> 14) + (tempA << 2) & 0xFFFF;
			tempB = (tempB ^ 0x9630) - 0x6553;
			this->value = ((tempB & 0xC000) >> 14) + (tempB << 2) & 0xFFFF;
		}
		return (tempA << 16) | tempB;
	}
};

gnu_noinline void test_token_parsing();

gnu_noinline void vec3_testing() {

	
	//_aligned_malloc()


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
		__asm__ volatile ("":"+t"(ld_value));
		*/
		long double ld_value;
		uint64_t yeet = counter;
		__asm__ volatile (
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
		__asm__ volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom3_end_time = rdtsc();

	custom4_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		int64_t yeet;
		__asm__ volatile (
			"BT $0x3F, %[counter] \n"
			"SBB %[yeet], %[yeet] \n"
			: asm_arg("=r", yeet)
			: asm_arg("r", counter)
			);
		ld_value += MAGIC_VALUE_A_ARRAY2[1 + yeet];
		__asm__ volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom4_end_time = rdtsc();

	custom5_start_time = rdtsc();
	counter = 0;
	do {
		long double ld_value = (int64_t)counter;
		int64_t yeet = counter;
		__asm__ volatile (
			"ADD %[yeet], %[yeet] \n"
			"SBB %[yeet], %[yeet] \n"
			: asm_arg("+r", yeet)
			);
		ld_value += MAGIC_VALUE_A_ARRAY2[1 + yeet];
		__asm__ volatile ("":"+t"(ld_value));
	} while (!add_overflow(counter, counter_add));
	custom5_end_time = rdtsc();

	custom6_start_time = rdtsc();
	counter = 0;
	do {
		uint64_t yeet = counter;
		long double ld_value;
		__asm__ volatile (
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

int stdcall main(int argc, char* argv[]) {

	//test_token_parsing();
	return 0;


	uint16_t timestamp = rdtsc();

	ZUNRng zun_rng = {
		.value = timestamp,
		.index = 0
	};
	WorstZUNRng terrible_zun_rng = {
		.seed = timestamp,
		.count = 0
	};

	return zun_rng.rand_uint() == terrible_zun_rng.this_is_awful();

	/*
	print_imm_with_bs(4);
	print_imm_with_bs(2);
	print_imm_with_bs(0xFFFF9630);
	print_imm_with_bs(0xFFFF9AAD);
	print_imm_with_bs(get_random_shift_count<false>(2));
	print_imm_with_bs(0xFFFF9630);
	print_imm_with_bs(0xFFFF9AAD);
	print_imm_with_bs(get_random_shift_count<false>(2));
	print_imm_with_bs(get_random_shift_count<false>(16));
	*/
	return 0;


	AddVectoredExceptionHandler(0, log_branch_records);

	NtGetContextThread_ptr = (NtGetContextThread_t*)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtGetContextThread");

	CONTEXT context;
	context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(GetCurrentThread(), &context);

	printf("Testing without DR7 bits... %zX\n", context.Dr7);
	debug_test_func();


	context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	context.Dr7 |= 0x100;

	SetThreadContext(GetCurrentThread(), &context);

	context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(GetCurrentThread(), &context);

	if (context.Dr7 & 0x100) {

		printf("\nTesting with DR7 bits... %zX\n", context.Dr7);
		debug_test_func();

	}
	else {
		printf("\nSetting DR7 failed %zX", context.Dr7);
	}

	return 0;

	printf("test\n");
	printf("\e[2J\e[3J");
	return 0;

	//dns_connect(L"waluigistacostand.ddns.net", L"42069");

	atomic_timing_test();

	//segment_jank_test();
	return 0;

	clang_noinline say_bye();

	return 0;

	/*
	printf("Available: %s\n", bool_str(IsApiSetImplemented("api-ms-win-eventing-classicprovider-l1-1-0")));

	HMODULE module = LoadLibraryA("api-ms-win-eventing-classicprovider-l1-1-0.dll");

	if (module) {
		for (size_t i = 64;; i += 64) {
			wchar_t buf[i];
			size_t buf_written = GetModuleFileNameW(module, buf, i);
			if (buf_written < i) {
				wprintf(L"Path: %s\n", buf);
				break;
			}
		}
	} else {
		printf("Path: Invalid handle\n");
	}
	*/

	//thcrap_inject_test();
	x64_inject_test();

	return 0;

	test_zun_rng2();
	return 0;
	//std::filesystem::path test = std::filesystem::path(yeet);

	//std::string piss;
	

	//volatile int yeet78 = new_fast_sqrt(0x00000001);

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	Sleep(5000);

	//std::wstring_view yeetu;
	//std::wstring bjkjbq = std::wstring(yeetu);

	/*
	for (int32_t i = 1; i >= 0; ++i) {
		//uint32_t test = fast_sqrt4(i);
		uint32_t test = fast_sqrt(i);
		//uint32_t rec98 = isqrt3(i);
		uint32_t rec98 = old_isqrt(i);
		if (test != rec98) {
			volatile int piss = fast_sqrt(i);
			__asm int3
		}
	}
	return 0;
	*/
	

	//LARGE_INTEGER freq;
	//QueryPerformanceFrequency(&freq);

	test_sqrt_thing();

	//generate_rtti_labels32(L"F:\\Touhou_Stuff_2\\disassembly_stuff\\19\\th19\\th19.exe", ".\\yeetus_out.txt", ".\\yeetus_in.txt");
	return 0;
	//return RNG.rand<int>();

	HMODULE ntdll_handle = GetModuleHandleA("ntdll.dll");
	auto NtCreateUserProcess_func = (NtCreateUserProcessPtr)GetProcAddress(ntdll_handle, "NtCreateUserProcess");

	HANDLE yeet = INVALID_HANDLE_VALUE;
	NtCreateUserProcess_func(
		&yeet,
		&yeet,
		PROCESS_ALL_ACCESS,
		THREAD_ALL_ACCESS,
		NULL,
		NULL,
		0x00020000UL,
		0,
		NULL,
		NULL,
		NULL
	);

	
	
	STARTUPINFOA si{
		.cb = sizeof(si)
	};
	PROCESS_INFORMATION pi{};
	CreateProcessA(
		"F:\\Touhou_Stuff_2\\disassembly_stuff\\8\\eiya - Copy\\th08.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		"F:\\Touhou_Stuff_2\\disassembly_stuff\\8\\eiya - Copy\\",
		&si,
		&pi
	);

	//normalize_float3(&float3a.x, &float3b.x);
	//std::thread threads[thread_count];
	//uint32_t local_compare_array_index = 0;
	//nounroll for (size_t i = 0; i < thread_iters; ++i) {
	//	for (auto& thread : threads) {
	//		thread = std::thread([]() gnu_noinline {
	//			uint32_t number_to_add = yeetus_value++;
	//			//while (yeetus_value != thread_count);
	//			test_jank2.push_value(number_to_add);
	//		});
	//	}
	//	for (size_t i = 0; i < thread_count; ++i) {
	//		auto&& [value, success] = test_jank2.pop_value();
	//		if (success) {
	//			printf("%u, %u\n", value, compare_array[local_compare_array_index++]);
	//		}
	//	}
	//	for (auto& thread : threads) {
	//		thread.join();
	//	};
	//	yeetus_value = 0;
	//}
	//printf("Writes: %u (%u)\n"
	//	   "Reads:  %u (%u)\n"
	//	   , write_count.load(), write_success_count.load()
	//	   , read_count.load(), read_success_count.load()
	//);
	//__builtin_dump_struct(&test_jank2, printf);
	
	//test_zun_rng2();
	return 0;

	//clang_noinline wjhrjwrv = __builtin_sqrt(-32984.2349f);
	//test_json_parse(u"{ \"ye\\u0065tus\" : true, \"pingas\":{}, \"yeet\": [ 1, 0.1, -123.456e+78, \"\", null ] }");
	//puts("");
	//test_json5_parse(L"{ \"ye\\u0065tus\" : true, \"pingas\":{}, \"yeet\": [ 01, 0.1, -123.456e+78, \"\", null, ], /* multi comment \n*/ unquoted : \'n\\x65w\\\nline\', }");

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

	//bs_dll64();
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
		//total += test_loop(1000000);
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

//typedef void json_t;

#if !IS_X64

#include "jansson.h"

struct _x86_reg_t {
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

typedef int(__cdecl *BreakpointFunc_t)(_x86_reg_t *regs, void *bp_info);

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

	dllexport int cdecl breakpoint_load_test(_x86_reg_t *regs, json_t *bp_info) asm("breakpoint_load_test");
	dllexport int cdecl breakpoint_load_test(_x86_reg_t *regs, json_t *bp_info) {
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

#endif

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
//#define atomic_signal_fence ____atomic_signal_fence
//#define atomic_thread_fence ____atomic_thread_fence
//#include <stdatomic.h>
//#undef atomic_signal_fence
//#undef atomic_thread_fence

// Just compile already
//#define atomic_compare_exchange_strong(...) (__VA_ARGS__)
#define atomic_compare_exchange_strong(arg1, arg2, arg3) ((void)arg1, (void)arg2, arg3)
//#define atomic_fetch_add(...) (__VA_ARGS__)
#define atomic_fetch_add(arg1, arg2) ((void)arg1, arg2)

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

/*
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
*/

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
struct _x86_reg_t2 {
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
struct _x86_reg_t2 {
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

#if !IS_X64
dllexport gnu_noinline gnu_attr(noduplicate) size_t cdecl breakpoint_process(void* bp, size_t addr_index, _x86_reg_t* regs) {
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
	_x86_reg_t* regs = (_x86_reg_t*)rsp_reg;

}
#endif

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
	//auto RtlInitUnicodeString_func = (decltype(&RtlInitUnicodeString)GetProcAddress(ntdll_handle, "RtlInitUnicodeString");
	//RtlInitUnicodeString_func(&device_name, L"\\Device\\PhysicalMemory");
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
		access.Trustee.ptstrName = (char*)"CURRENT_USER";

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
/*
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
*/

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
dllexport gnu_attr(target("no-sse")) void ArchSetGdtEntry(KGDTENTRY* gdt_base, uint32_t offset, uint64_t base, size_t limit, KGDTTYPE type, uint32_t dpl, SetGetEntryFlags flags) {
	//KGDTENTRY64* entry = based_pointer<KGDTENTRY64>(gdt_base, AlignDownToMultipleOf2(offset, alignof(KGDTENTRY)));
	KGDTENTRY64* entry = (KGDTENTRY64*)(uintptr_t(gdt_base) + AlignDownToMultipleOf2(offset, alignof(KGDTENTRY)));
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
dllexport gnu_attr(target("no-sse")) void ArchSetGdtEntryNew(KGDTENTRY* gdt_base, uint32_t offset, uint64_t base, size_t limit, KGDTTYPE type, uint32_t dpl, uint8_t flags) {
	//KGDTENTRY64* entry = based_pointer<KGDTENTRY64>(gdt_base, AlignDownToMultipleOf2(offset, alignof(KGDTENTRY)));
	KGDTENTRY64* entry = (KGDTENTRY64*)(uintptr_t(gdt_base) + AlignDownToMultipleOf2(offset, alignof(KGDTENTRY)));
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

dllexport gnu_attr(target("no-sse")) void ArchSetGdtEntryNew2(KGDTENTRY* gdt_base, uint32_t offset, uint64_t base, size_t limit, KGDTTYPE type, uint32_t dpl, uint8_t flags) {
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

struct ServiceDescriptor {
	uint32_t arg_count : 4;
	uint32_t offset : 28;
};

dllexport void KeCompactServiceTable(uint64_t *restrict address_table, const uint8_t *restrict argument_table, uint32_t limit, bool is_win2k_table) {
	ServiceDescriptor *restrict table_output = (ServiceDescriptor *restrict)address_table;
	for (size_t i = 0; i < limit; ++i) {
		table_output[i] = {
			.arg_count = (uint32_t)(argument_table[i] >> 2),
			.offset = (uint32_t)(address_table[i] - (uintptr_t)address_table)
		};
	}
	if (is_win2k_table) {
		for (size_t i = 0; i < limit; ++i) {
			((uint8_t*)&table_output[limit])[i] = ((uint8_t*)&address_table[limit])[i];
		}
	}
}

dllexport void KeCompactServiceTable2(uint64_t *restrict address_table, const uint8_t *restrict argument_table, size_t limit, bool is_win2k_table) {
	if ((uint32_t)limit) {
		uint32_t *restrict table_output = (uint32_t * restrict)address_table;
		novectorize for (size_t i = 0; i < limit; ++i) {
			table_output[i] = ((address_table[i] - (uintptr_t)address_table) << 4) | (argument_table[i] >> 2);
		}
		if (is_win2k_table) {
			uint8_t *restrict status_vec_out = (uint8_t *restrict)&table_output[limit];
			uint8_t *restrict status_vec_in = (uint8_t *restrict)&address_table[limit];
			novectorize do {
				*status_vec_out++ = *status_vec_in++;
			} while (--limit);
		}
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
/*
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
*/
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

/*
#define thcrap_value2(type, name, value) \
struct { \
	inline operator type() { \
		static const unsigned char dummy[1] = {}; \
		return bitcast<float>(&dummy[0]);\
	} \
} name asm(value)
*/

#define thcrap_value2(type, name, value) \
dllexport gnu_used struct dummy_name { \
	inline operator type() { \
		return bitcast<type>(this); \
	} \
} name asm(value)

thcrap_expr(test_val, <option:value_test> * 7);

dllexport uint32_t get_option_test_A() {
	return test_val;
}

/*
#include <winhttp.h>

typedef void cdecl PhotonAsyncFunc(void* arg);

struct PhotonAsync {
	PhotonAsyncFunc* func; // 0x0
	void* arg; // 0x4
	// 0x8

	// Rx39ED0
	dllexport static gnu_noinline DWORD WINAPI run(void* arg) {
		PhotonAsync* async = (PhotonAsync*)arg;
		async->func(async->arg);
		free(async);
		return 0;
	}

	// Rx39E50
	dllexport static gnu_noinline void cdecl create(PhotonAsyncFunc* func, void* arg) {
		if (void* func_ptr = func) {
			PhotonAsync* new_async = (PhotonAsync*)malloc(sizeof(PhotonAsync));
			new_async->func = func;
			new_async->arg = arg;
			CloseHandle(CreateThread(NULL, 0, &PhotonAsync::run, new_async, 0, NULL));
		}
	}
};



struct PhotonStruct {


	struct PhotonString {
		unknown_fields(0x4); // 0x0
		LPWSTR data; // 0x4

		// Rx29510
		gnu_noinline LPWSTR get_pointer() {
			return this->data;
		}
	};


	
	// void* vtable; // 0x0
	unknown_fields(0x4); // 0x4
	int __dword_8; // 0x8
	unknown_fields(0x98); // 0xC
	char __byte_A0; // 0xA0
	unknown_fields(0x23); // 0xA1
	PhotonString* __something_that_generates_the_websocket_header_and_ill_pretend_its_a_string; // 0xC8
	HINTERNET http_open_handle; // 0xCC
	HINTERNET http_connect_handle; // 0xD0
	HINTERNET http_request_handle; // 0xD4
	HINTERNET web_socket_handle; // 0xD8
	PhotonString server_name; // 0xDC
	unknown_fields(0x8); // 0xE4
	INTERNET_PORT port; // 0xEC
	probably_padding_bytes(0x2); // 0xEE
	PhotonString __string_F0; // 0xF0
	unknown_fields(0x8); // 0xF8
	bool use_https; // 0x100
	// 0x100

	dllexport static gnu_noinline void cdecl __sub_EFFE0(void* self) {

	}

	dllexport void thiscall web_test() {
		HINTERNET http_open_handle = WinHttpOpen(L"Photon", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		this->http_open_handle = http_open_handle;
		if (!http_open_handle) {
connection_error:
			printf("Connection error: %d", GetLastError());
			return;
		}
		HINTERNET http_connect_handle = WinHttpConnect(this->http_open_handle, this->server_name.get_pointer(), this->port, 0);
		this->http_connect_handle = http_connect_handle;
		if (!http_open_handle) goto connection_error;
		HINTERNET http_request_handle = WinHttpOpenRequest(this->http_connect_handle, L"GET", this->__string_F0.get_pointer(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, this->use_https ? WINHTTP_FLAG_SECURE : 0);
		this->http_request_handle = http_request_handle;
		if (!http_request_handle) goto connection_error;
		if (!WinHttpSetOption(http_request_handle, WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, NULL, 0)) goto connection_error;


		if (!WinHttpSendRequest(
				this->http_request_handle,
				this->__something_that_generates_the_websocket_header_and_ill_pretend_its_a_string->get_pointer(),
				0,
				WINHTTP_NO_REQUEST_DATA,
				0,
				0,
				NULL
		)) goto connection_error;
		if (!WinHttpReceiveResponse(this->http_request_handle, NULL)) goto connection_error;
		HINTERNET web_socket_handle = WinHttpWebSocketCompleteUpgrade(this->http_request_handle, NULL);
		if (!web_socket_handle) goto connection_error;
		printf("Succesfully upgraded to websocket protocol"); // Nice typo ExitGames
		this->__byte_A0 = 1;
		this->__dword_8 = 5;
		PhotonAsync::create(&PhotonStruct::__sub_EFFE0, this);

	}
};
*/

struct DllImportT {
	const char* dll_name;
	size_t max_import_index;
};

dllexport DllImportT dll_array[69];

dllexport void* iat_array[69];

dllexport const char* func_strings[69];

dllexport void __cdecl make_iat_test() {
	using LoadLibraryA_t = decltype(LoadLibraryA);
	LoadLibraryA_t* load_library_a = (LoadLibraryA_t*)th_GetProcAddress(th_GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	size_t iat_index = 0;
	for (
		DllImportT* dll_iter = dll_array;
		const char* dll_name = dll_iter->dll_name;
		++dll_iter
	) {
		HMODULE dll_handle = th_GetModuleHandleA(dll_name);
		if (!dll_handle) {
			dll_handle = load_library_a(dll_name);
		}
		size_t max_iat_index = dll_iter->max_import_index;
		do {
			iat_array[iat_index] = (void*)th_GetProcAddress(dll_handle, func_strings[iat_index]);
		} while (++iat_index != max_iat_index);
	}
}

dllexport void __cdecl make_iat_test3() {
	using LoadLibraryA_t = decltype(LoadLibraryA);
	LoadLibraryA_t* load_library_a = (LoadLibraryA_t*)th_GetProcAddress(th_GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	size_t iat_index = 0;
	DllImportT* dll_iter = dll_array;
	const char* dll_name = dll_iter->dll_name;
	do {
		HMODULE dll_handle = th_GetModuleHandleA(dll_name);
		if (!dll_handle) {
			dll_handle = load_library_a(dll_name);
		}
		for (
			size_t max_iat_index = dll_iter->max_import_index;
			iat_index != max_iat_index;
			++iat_index
		) {
			iat_array[iat_index] = (void*)th_GetProcAddress(dll_handle, func_strings[iat_index]);
		}
	} while ((dll_name = (++dll_iter)->dll_name));
}

struct DllImportT2 {
	const char *const dll_name;
	const size_t import_count;
	const char *const func_strings[];
};

dllexport void __cdecl make_iat_test2() {
	using LoadLibraryA_t = decltype(LoadLibraryA);
	LoadLibraryA_t* load_library_a = (LoadLibraryA_t*)th_GetProcAddress(th_GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	size_t iat_index = 0;
	for (
		const DllImportT2* dll_iter = (const DllImportT2*)dll_array;
		const char* dll_name = dll_iter->dll_name;

	) {
		HMODULE dll_handle = th_GetModuleHandleA(dll_name);
		if (!dll_handle) {
			dll_handle = load_library_a(dll_name);
		}
		for (size_t i = dll_iter->import_count; i; --i) {
			iat_array[iat_index] = (void*)th_GetProcAddress(dll_handle, dll_iter->func_strings[iat_index]);
			++iat_index;
		}
	}
}

dllexport std::pair<uint8_t*, size_t> wkjbrwkrjb() {
	return { nullptr, 0 };
}

dllexport size_t regcall str_to_num_test(const char* str, size_t base) {
	//auto [num, _] = strtonum<size_t, 0, true, true, true>(str, base);

	return 0;
}

static constexpr const char checkbox_str[] = "checkbox";


#if !IS_X64
/*
{
	"state_buffer": "expression",
	"ascii_manager": "expression",
	"x_pos": "expression",
	"y_pos": "expression",
	"title": "text",
}
*/
#define THCRAP_API __declspec(dllimport)
#define THCRAP_EXPORT_API
#include <assert.h>
#include <string>
#include <string_view>

#pragma push_macro("dllexport")
#pragma push_macro("ms_abi")
#pragma push_macro("cdecl")
#pragma push_macro("stdcall")
#pragma push_macro("fastcall")
#pragma push_macro("regcall")
#pragma push_macro("regparm")
#pragma push_macro("vectorcall")
#pragma push_macro("thiscall")
#pragma push_macro("naked")
#undef dllexport
#undef ms_abi
#undef cdecl
#undef stdcall
#undef fastcall
#undef regcall
#undef regparm
#undef vectorcall
#undef thiscall
#undef naked


#include "F:\\Users\\zero318\\Source\\Repos\\thcrap\\thcrap\\src\\compiler_support.h"

extern "C" {

#undef TH_NODISCARD_REASON
#define TH_NODISCARD_REASON(...)
#define THCRAP_INTERNAL_API

#undef TH_VECTORCALL
#define TH_VECTORCALL __vectorcall
#undef TH_FASTCALL
#define TH_FASTCALL __fastcall

#define _PEB _TH_PEB
#define PEB TH_PEB
#define PPEB TH_PPEB
#define _CLIENT_ID _TH_CLIENT_ID
#define CLIENT_ID TH_CLIENT_ID
#define _UNICODE_STRING _TH_UNICODE_STRING
#define UNICODE_STRING TH_UNICODE_STRING
#define PUNICODE_STRING TH_PUNICODE_STRING
#define _TEB _TH_TEB
#define TEB TH_TEB
#define PTEB TH_PTEB
#define _CURDIR _TH_CURDIR
#define CURDIR TH_CURDIR
#define _GDI_TEB_BATCH _TH_GDI_TEB_BATCH
#define GDI_TEB_BATCH TH_GDI_TEB_BATCH
#define _KSYSTEM_TIME _TH_KSYSTEM_TIME
#define KSYSTEM_TIME TH_KSYSTEM_TIME
#define _KUSER_SHARED_DATA _TH_KUSER_SHARED_DATA
#define KUSER_SHARED_DATA TH_KUSER_SHARED_DATA
#define _PROCESS_BASIC_INFORMATION _TH_PROCESS_BASIC_INFORMATION
#define PROCESS_BASIC_INFORMATION TH_PROCESS_BASIC_INFORMATION
#define _RTL_USER_PROCESS_PARAMETERS _TH_RTL_USER_PROCESS_PARAMETERS
#define RTL_USER_PROCESS_PARAMETERS TH_RTL_USER_PROCESS_PARAMETERS
#define PROCESSINFOCLASS TH_PROCESSINFOCLASS
#define ProcessBasicInformation TH_ProcessBasicInformation
#define ProcessWow64Information TH_ProcessWow64Information
#define ProcessImageFileName TH_ProcessImageFileName
#define NTAPI __stdcall
#define NtQueryInformationProcess TH_NtQueryInformationProcess
#define NtProductWinNt TH_NtProductWinNt
#define NtProductLanManNt TH_NtProductLanManNt
#define NtProductServer TH_NtProductServer
#define NT_PRODUCT_TYPE TH_NT_PRODUCT_TYPE
#define StandardDesign TH_StandardDesign
#define NEC98x86 TH_NEC98x86
#define EndAlternatives TH_EndAlternatives
#define ALTERNATIVE_ARCHITECTURE_TYPE TH_ALTERNATIVE_ARCHITECTURE_TYPE
#define TH_FORCEINLINE
#define SpinLock TH_SpinLock
#include "F:\\Users\\zero318\\Source\\Repos\\thcrap\\thcrap\\src\\util.h"
#undef _PEB
#undef PEB
#undef PPEB
#undef _CLIENT_ID
#undef CLIENT_ID
#undef _UNICODE_STRING
#undef UNICODE_STRING
#undef PUNICODE_STRING
#undef _TEB
#undef TEB
#undef PTEB
#undef _CURDIR
#undef CURDIR
#undef _GDI_TEB_BATCH
#undef GDT_TEB_BATCH
#undef _KSYSTEM_TIME
#undef KSYSTEM_TIME
#undef _KUSER_SHARED_DATA
#undef KUSER_SHARED_DATA
#undef _PROCESS_BASIC_INFORMATION
#undef PROCESS_BASIC_INFORMATION
#undef _RTL_USER_PROCESS_PARAMETERS
#undef RTL_USER_PROCESS_PARAMETERS
#undef PROCESSINFOCLASS
#undef ProcessBasicInformation
#undef ProcessWow64Information
#undef ProcessImageFileName
#undef NtQueryInformationProcess
#undef NtProductWinNt
#undef NtProductLanManNt
#undef NtProductServer
#undef NT_PRODUCT_TYPE
#undef StandardDesign
#undef NEC98x86
#undef EndAlternatives
#undef ALTERNATIVE_ARCHITECTURE_TYPE
#undef SpinLock
#include "F:\\Users\\zero318\\Source\\Repos\\thcrap\\thcrap\\src\\jansson_ex.h"
#define LongDouble80 long double
#include "F:\\Users\\zero318\\Source\\Repos\\thcrap\\thcrap\\src\\expression.h"
#include "F:\\Users\\zero318\\Source\\Repos\\thcrap\\thcrap\\src\\patchfile.h"

}
#pragma pop_macro("naked")
#pragma pop_macro("thiscall")
#pragma pop_macro("vectorcall")
#pragma pop_macro("regparm")
#pragma pop_macro("regcall")
#pragma pop_macro("fastcall")
#pragma pop_macro("stdcall")
#pragma pop_macro("cdecl")
#pragma pop_macro("ms_abi")
#pragma pop_macro("dllexport")

struct MenuState {
	size_t selection;
};

struct AsciiManager19 {
	unknown_fields(0x1977C);
	DWORD text_color;
};

dllexport gnu_noinline void cdecl ascii_draw_text(AsciiManager19* ascii_manager, float* position, const char* format, ...) {
	use_var(ascii_manager);
	use_var(position);
	va_list va;
	va_start(va, format);
	vprintf(format, va);
	va_end(va);
}

struct Input {
	uint8_t select : 1;
	uint8_t back : 1;
	uint8_t : 3;
	uint8_t up : 1;
	uint8_t down : 1;
	uint8_t left : 1;
	uint8_t right : 1;
};

enum MenuItemStyle : size_t {
					// Menu Title
	PrintText,		// -Label
	Checkbox,		// *Label: [X]
	HSpinnerNumber,	// *Label: <  1>
	HSpinnerText,	// *Label: <Yeet>
	WriteValue,		// *Quit
	InvalidStyle,
};

// Tells thcrap to execute the original
// bytes overwritten by the breakpoint
// call after returning
#define BREAKPOINT_EXEC_CAVE 1

// These would be defined as per-game
// options of some sort in the codecave
#define INPUT_ADDR 0xDEADBEEF
#define ASCII_MANAGER_PTR_ADDR 0xDEADBEEF

dllexport int cdecl menu_on_tick(x86_reg_t* regs, json_t* bp_info) {
	MenuState* menu_state = (MenuState*)json_object_get_eval_addr_default(bp_info, "state_buffer", 0, JEVAL_DEFAULT);
	if (!menu_state) {
		return BREAKPOINT_EXEC_CAVE;
	}
	json_t* menu_items = json_object_get(bp_info, "items");
	if (!json_is_array(menu_items)) {
		return BREAKPOINT_EXEC_CAVE;
	}
	Input input = *(Input*)INPUT_ADDR;

	size_t array_max = json_array_size(menu_items) - 1;
	size_t starting_index = menu_state->selection;
	size_t selected_index = starting_index;

	MenuItemStyle style;
	json_t* menu_item;
	if (input.up && !input.down) {
		do {
			if (--selected_index == SIZE_MAX) {
				selected_index = array_max;
			}
			menu_item = json_array_get(menu_items, selected_index);
		} while (
			selected_index != starting_index &&
			(json_object_get_eval_bool_default(menu_item, "hidden", false, JEVAL_DEFAULT) ||
			 PrintText == (style = (MenuItemStyle)json_object_get_eval_int_default(menu_item, "style", PrintText, JEVAL_DEFAULT)))
		);
	} else if (input.down) {
		do {
			if (++selected_index > array_max) {
				selected_index = 0;
			}
		} while (
			selected_index != starting_index &&
			(json_object_get_eval_bool_default(menu_item, "hidden", false, JEVAL_DEFAULT) ||
			 PrintText == (style = (MenuItemStyle)json_object_get_eval_int_default(menu_item, "style", PrintText, JEVAL_DEFAULT)))
		);
	}
	menu_state->selection = selected_index;

	switch (style) {
		case WriteValue:
			if (input.select) {
				size_t* val_ptr = (size_t*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				*val_ptr = json_object_get_eval_addr_default(menu_item, "val", 0, JEVAL_DEFAULT);
			}
			break;
		case Checkbox: 
			if (input.select || (input.left != input.right)) {
				bool* val_ptr = (bool*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				*val_ptr ^= true;
			}
			break;
		case HSpinnerNumber:
			if (input.left != input.right) {
				int32_t change = input.left ? 1 : -1;
				const char* type_str = json_object_get_string(menu_item, "type");
				if (!type_str) goto is_bad;
				patch_value_type_t value_type = patch_parse_type(type_str);
				switch (value_type) {
					case PVT_BYTE ... PVT_SDWORD:
						break;
					default:
						goto is_bad;
				}
				void* val_ptr = (void*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				size_t value;
				switch (value_type--) {
					default:
						unreachable;
					case PVT_BYTE: case PVT_SBYTE:
						*(int8_t*)val_ptr += change;
						break;
					case PVT_WORD: case PVT_SWORD:
						*(int16_t*)val_ptr += change;
						break;
					case PVT_DWORD: case PVT_SDWORD:
						*(int32_t*)val_ptr += change;
						break;
				}
			}
			break;
		case HSpinnerText:
			if (input.left != input.right) {
				json_t* text_array = json_object_get(menu_item, "text");
				if (!json_is_array(text_array)) goto is_bad;
				size_t text_array_max = json_array_size(text_array) - 1;
				size_t* val_ptr = (size_t*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				if (input.left) {
					if (++*val_ptr > text_array_max) {
						*val_ptr = 0;
					}
				} else {
					if (--*val_ptr == SIZE_MAX) {
						*val_ptr = text_array_max;
					}
				}
			}
			break;
		default:
			break;
		is_bad:
			json_object_set_new_nocheck(menu_item, "hidden", json_true());
			break;
	}
	return BREAKPOINT_EXEC_CAVE;
}

dllexport int cdecl menu_on_draw(x86_reg_t* regs, json_t* bp_info) {
	MenuState* menu_state = (MenuState*)json_object_get_eval_addr_default(bp_info, "state_buffer", NULL, JEVAL_DEFAULT);
	if (!menu_state) {
		return BREAKPOINT_EXEC_CAVE;
	}
	AsciiManager19* ascii_manager = *(AsciiManager19**)ASCII_MANAGER_PTR_ADDR;
	float position[4];
	position[0] = (float)(long double)json_object_get_eval_real_default(bp_info, "x_pos", 0.0, JEVAL_DEFAULT);
	position[1] = (float)(long double)json_object_get_eval_real_default(bp_info, "y_pos", 0.0, JEVAL_DEFAULT);
	position[2] = 0.0f;
	position[3] = (float)json_object_get_eval_real_default(bp_info, "y_spacing", 1.0, JEVAL_DEFAULT);

	auto draw_text = [ascii_manager, &position]<typename ... Args>(const char* format, Args... args) gnu_always_inline {
		ascii_draw_text(ascii_manager, position, format, args...);
		position[1] += position[3];
	};

	if (const char* title_str = json_object_get_string(bp_info, "title")) {
		draw_text("%s", title_str);
	}
	json_t* menu_items = json_object_get(bp_info, "items");
	if (!json_is_array(menu_items)) {
		return BREAKPOINT_EXEC_CAVE;
	}

	json_t* menu_item;
	json_array_foreach_scoped(size_t, i, menu_items, menu_item) {
		if (json_object_get_eval_bool_default(menu_item, "hidden", false, JEVAL_DEFAULT)) {
			continue;
		}
		const char* label_str = json_object_get_string(menu_item, "label");
		if (!label_str) {
			label_str = "";
		}
		MenuItemStyle action = (MenuItemStyle)json_object_get_eval_int_default(menu_item, "style", PrintText, JEVAL_DEFAULT);

		uint32_t selection_characters = menu_state->selection == i ? PackInt('*', '<', '>') : PackInt(' ', ' ', ' ');
		switch (action) {
			case PrintText: {
				draw_text("-%s", label_str);
				break;
			}
			case WriteValue: {
				draw_text("%c%s", selection_characters, label_str);
				break;
			}
			case Checkbox: {
				bool* val_ptr = (bool*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				draw_text("%c%s: [%c]", selection_characters, label_str, *val_ptr ? 'X' : ' ');
				break;
			}
			case HSpinnerNumber: {
				const char* type_str = json_object_get_string(menu_item, "type");
				if (!type_str) goto is_bad;
				patch_value_type_t value_type = patch_parse_type(type_str);
				switch (value_type) {
					case PVT_BYTE ... PVT_SDWORD:
						break;
					default:
						goto is_bad;
				}
				void* val_ptr = (void*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				size_t value;
				switch (value_type--) {
					default:
						unreachable;
					case PVT_BYTE: case PVT_SBYTE:
						value = *(uint8_t*)val_ptr;
						break;
					case PVT_WORD: case PVT_SWORD:
						value = *(uint16_t*)val_ptr;
						break;
					case PVT_DWORD: case PVT_SDWORD:
						value = *(uint32_t*)val_ptr;
						break;
				}
				static constexpr const char* format_table[] = {
					"%c%s: %c%3" PRIu8 "%c", "%c%s: %c%4" PRIi8 "%c",
					"%c%s: %c%5" PRIu16 "%c", "%c%s: %c%6" PRIi16 "%c",
					"%c%s: %c%10" PRIu32 "%c", "%c%s: %c%11" PRIi32 "%c"
				};
				draw_text(format_table[value_type], selection_characters, label_str, selection_characters >> 8, value, selection_characters >> 16);
				break;
			}
			case HSpinnerText: {
				json_t* text_array = json_object_get(menu_item, "text");
				if (!json_is_array(text_array)) goto is_bad;
				size_t* val_ptr = (size_t*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				json_t* text_value = json_array_get(text_array, *val_ptr);
				if (!json_is_string(text_value)) goto is_bad;
				draw_text("%c%s: %c%s%c", selection_characters, label_str, selection_characters >> 8, json_string_value(text_value), selection_characters >> 16);
				break;
			}
			default:
				unreachable;
			is_bad:
				json_object_set_new_nocheck(menu_item, "hidden", json_true());
				break;
		}
	}
	return BREAKPOINT_EXEC_CAVE;
}

#endif

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

/*
enum ReturnCodes {
	INJECT_INIT_FALSE = -1, // Initialization function returned false
	INJECT_SUCCESS = 0,
	INJECT_DLL_FALSE = 1, // DllMain returned FALSE
	INJECT_DLL_ERROR = 2, // LoadLibrary failed
	INJECT_INIT_ERROR = 3, // Initialization function not found
};
*/

uint8_t inject_func[] = {
	0x53,             //   PUSH EBX
	0x6A, 0x00,       //   PUSH 0
	0x6A, 0x00,       //   PUSH 0
	0x68, 0, 0, 0, 0, //   PUSH dll_name
	0xE8, 0, 0, 0, 0, //   CALL LoadLibraryExW
	0x85, 0xC0,       //   TEST EAX, EAX
	0x74, 0x30,       //   JZ load_library_fail
	0x89, 0xC3,       //   MOV EBX, EAX
	0x68, 0, 0, 0, 0, //   PUSH init_func_name
	0x50,             //   PUSH EAX
	0xE8, 0, 0, 0, 0, //   CALL GetProcAddress
	0x85, 0xC0,       //   TEST EAX, EAX
	0x74, 0x32,       //   JZ get_init_func_fail
	0x68, 0, 0, 0, 0, //   PUSH init_func_param
	0xFF, 0xD0,       //   CALL EAX
	0x89, 0xD9,       //   MOV ECX, EBX
	0x89, 0xC3,       //   MOV EBX, EAX
	0x85, 0xC0,       //   TEST EAX, EAX
	0x74, 0x09,       //   JZ init_func_success
	0x83, 0xCB, 0xFF, //   OR EBX, -1
					  // free_library:
	0x51,             //   PUSH ECX
	0xE8, 0, 0, 0, 0, //   CALL FreeLibrary
					  // exit_thread:
	0x53,             //   PUSH EBX
	0xE8, 0, 0, 0, 0, //   CALL ExitThread
	0xCC,             //   INT3
					  // load_library_fail:
	0x31, 0xDB,       //   XOR EBX, EBX
					  //   CMP DWORD PTR FS:[LastErrorValue], STATUS_DLL_INIT_FAILED
	0x64, 0x81, 0x3D, 0x34, 0x00, 0x00, 0x00, 0x42, 0x01, 0x00, 0xC0,
	0x0F, 0x95, 0xC3, //   SETNE BL
	0x43,             //   INC EBX
	0xEB, 0xE6,       //   JMP exit_thread
					  // get_init_func_fail:
	0x89, 0xD9,       //   MOV ECX, EBX
					  //   MOV EBX, 3
	0xBB, 0x03, 0x00, 0x00, 0x00,
	0xEB, 0xD7,       //   JMP free_library
	0xCC
};

const size_t inject_func_length = sizeof(inject_func);
const size_t dll_name = 6;
const size_t load_library = 11;
const size_t init_func_name = 22;
const size_t get_proc_address = 28;
const size_t init_func_param = 37;
const size_t free_library = 56;
const size_t exit_thread = 62;

enum ReturnCodes {
	INJECT_INIT_FALSE = -1, // Initialization function returned false
	INJECT_SUCCESS = 0,
	INJECT_DLL_FALSE = 1, // DllMain returned FALSE
	INJECT_DLL_ERROR = 2, // LoadLibrary failed
	INJECT_INIT_ERROR = 3, // Initialization function not found,
	INJECT_ALLOC_FAIL = 4, // One of the virtual memory functions failed
	INJECT_THREAD_FAIL = 5, // CreateRemoteThread failed
};

dllexport int inject(HANDLE process, const wchar_t* dll_str, const char* init_func, int32_t param) {
	size_t dll_name_bytes = (wcslen(dll_str) + 1) * sizeof(wchar_t);
	size_t init_func_bytes = strlen(init_func) + 1;

	uint8_t* inject_buffer = (uint8_t*)VirtualAllocEx(process, NULL, inject_func_length + dll_name_bytes + init_func_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!inject_buffer) {
		return INJECT_ALLOC_FAIL;
	}

	*(uint8_t**)&inject_func[dll_name] = &inject_buffer[inject_func_length];
	*(int32_t*)&inject_func[load_library] = (uint8_t*)&LoadLibraryExW - &inject_buffer[load_library + sizeof(int32_t)];
	*(uint8_t**)&inject_func[init_func_name] = &inject_buffer[inject_func_length + dll_name_bytes];
	*(int32_t*)&inject_func[get_proc_address] = (uint8_t*)&GetProcAddress - &inject_buffer[get_proc_address + sizeof(int32_t)];
	*(int32_t*)&inject_func[init_func_param] = param;
	*(int32_t*)&inject_func[free_library] = (uint8_t*)&FreeLibrary - &inject_buffer[free_library + sizeof(int32_t)];
	*(int32_t*)&inject_func[exit_thread] = (uint8_t*)&ExitThread - &inject_buffer[exit_thread + sizeof(int32_t)];

	DWORD exit_code = INJECT_ALLOC_FAIL;
	if (
		WriteProcessMemory(process, inject_buffer, inject_func, inject_func_length, NULL) &&
		WriteProcessMemory(process, inject_buffer + inject_func_length, dll_str, dll_name_bytes, NULL) &&
		WriteProcessMemory(process, inject_buffer + inject_func_length + dll_name_bytes, init_func, init_func_bytes, NULL) &&
		FlushInstructionCache(process, inject_buffer, inject_func_length)
	) {
		exit_code = INJECT_THREAD_FAIL;
		HANDLE thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)inject_buffer, 0, 0, NULL);
		if (thread) {
			WaitForSingleObject(thread, INFINITE);
			GetExitCodeThread(thread, &exit_code);
			CloseHandle(thread);
		}
	}

	VirtualFreeEx(process, inject_buffer, 0, MEM_RELEASE);

	return (int)exit_code;
}

static const uint8_t infinite_loop[] = {
				// inf:
	0xEB, 0xFE, //   JMP inf
};

void thcrap_inject_test() {

	STARTUPINFOW si = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION pi = {};

	if (CreateProcessW(
		L"F:/Touhou_Stuff_2/disassembly_stuff/7/youmu - Copy2/th07.exe",
		NULL,
		NULL,
		NULL,
		TRUE,
		CREATE_SUSPENDED,
		NULL,
		L"F:/Touhou_Stuff_2/disassembly_stuff/7/youmu - Copy2/",
		&si,
		&pi
	)) {

		CONTEXT context = {};
		context.ContextFlags = CONTEXT_INTEGER | CONTEXT_SEGMENTS;
		GetThreadContext(pi.hThread, &context);

		uintptr_t addr;
		//if (!is_running_on_wine) {
			//addr = context.Eax;
		//} else {
			LDT_ENTRY entry;
			GetThreadSelectorEntry(pi.hThread, context.SegFs, &entry);
			addr = entry.BaseLow | entry.HighWord.Bits.BaseMid << 16 | entry.HighWord.Bits.BaseHi << 24;
			ReadProcessMemory(pi.hProcess, (LPCVOID)(addr + 0x30), &addr, sizeof(uintptr_t), NULL);
			ReadProcessMemory(pi.hProcess, (LPCVOID)(addr + 0x8), &addr, sizeof(uintptr_t), NULL);
			DWORD offset;
			ReadProcessMemory(pi.hProcess, (LPCVOID)(addr + 0x3C), &offset, sizeof(DWORD), NULL);
			ReadProcessMemory(pi.hProcess, (LPCVOID)(addr + offset + 0x28), &offset, sizeof(DWORD), NULL);
			addr += offset;
		//}
		;
		DWORD protection;
		VirtualProtectEx(pi.hProcess, (LPVOID)addr, sizeof(infinite_loop), PAGE_EXECUTE_READWRITE, &protection);
		uint8_t old_bytes[sizeof(infinite_loop)];
		ReadProcessMemory(pi.hProcess, (LPCVOID)addr, old_bytes, sizeof(old_bytes), NULL);
		WriteProcessMemory(pi.hProcess, (LPVOID)addr, infinite_loop, sizeof(infinite_loop), NULL);
		FlushInstructionCache(pi.hProcess, (LPCVOID)addr, sizeof(infinite_loop));

		context.ContextFlags = CONTEXT_CONTROL;
		do {
			ResumeThread(pi.hThread);
			Sleep(10);
			SuspendThread(pi.hThread);
			GetThreadContext(pi.hThread, &context);
#if IS_X64
		} while (addr != context.Rip);
#else
		} while (addr != context.Eip);
#endif

		WriteProcessMemory(pi.hProcess, (LPVOID)addr, old_bytes, sizeof(old_bytes), NULL);
		VirtualProtectEx(pi.hProcess, (LPVOID)addr, sizeof(old_bytes), protection, &protection);
		FlushInstructionCache(pi.hProcess, (LPCVOID)addr, sizeof(old_bytes));

		SetCurrentDirectoryW(L"F:/Touhou_Stuff_2/_thtk-bin-new/__thcrap/bin/");
		int code = inject(pi.hProcess, L"F:\\Touhou_Stuff_2\\_thtk-bin-new\\__thcrap\\bin\\thcrap.dll", "thcrap_init", 0x7FFE0100);
		printf("%i", code);
		if (code == 0) {
			ResumeThread(pi.hThread);
		} else {
			TerminateProcess(pi.hProcess, 0);
		}
	}
}

uintptr_t arg_val;

typedef int(__fastcall *init_func_t)(uintptr_t*);

dllexport gnu_noinline HMODULE stdcall LoadLibraryExW_fake(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags) {
	use_var(lpLibFileName);
	use_var(hFile);
	use_var(dwFlags);
	return (HMODULE)rand();
	//return LoadLibraryExW(lpLibFileName, hFile, dwFlags);
}
dllexport gnu_noinline FARPROC stdcall GetProcAddres_fake(HMODULE hModule, LPCSTR lpProcName) {
	use_var(hModule);
	use_var(lpProcName);
	return (FARPROC)rand();
	//return GetProcAddress(hModule, lpProcName);
}
dllexport gnu_noinline noreturn void stdcall ExitThread_fake(DWORD dwExitCode) {
	use_var(dwExitCode);
	//ExitThread(dwExitCode);
}
dllexport gnu_noinline BOOL stdcall FreeLibrary_fake(HMODULE hLibModule) {
	use_var(hLibModule);
	return (BOOL)rand();
	//return FreeLibrary(hLibModule);
}

dllexport int inject_lib() {
	HMODULE handle = LoadLibraryExW_fake(L"dummy.dll", NULL, 0);
	HMODULE handleB = handle;
	__asm__ volatile ("":"+b"(handleB));
	HMODULE handle2;
	if (expect(handle == NULL, false)) {
		handleB = (HMODULE)(teb32->LastErrorValue == STATUS_DLL_INIT_FAILED ? INJECT_DLL_FALSE : INJECT_DLL_ERROR);
		goto exit_label;
	}

	init_func_t init_func = (init_func_t)GetProcAddres_fake(handle, "dummy");
	if (expect(init_func == NULL, false)) {
		handle = handleB;
		__asm__ volatile ("":"+c"(handle));
		handleB = (HMODULE)INJECT_INIT_ERROR;
		goto library_label;
	}

	int exit_code = init_func(&arg_val);
	handle = handleB;
	__asm__ volatile ("":"+c"(handle));
	handleB = (HMODULE)exit_code;
	if (exit_code != EXIT_SUCCESS) {
		handleB = (HMODULE)INJECT_INIT_FALSE;
		__asm__ volatile ("":"+b"(handleB));
	library_label:
		__asm__ volatile ("":"+c"(handle));
		FreeLibrary_fake(handle);
	}


exit_label:
	__asm__ volatile ("":::);
	clang_noinline ExitThread_fake((DWORD)handleB);
}

static inline constexpr wchar_t dummy_dll_name[] = L"F:/Users/zero318/Source/Repos/ClangAsmTest1/out/build/x64-Clang-Release-Debug/x64_inject_test.dll";

/*
#if IS_X64
#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")
#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\out\\build\\x64-Clang-Release-Debug\\x64_inject_test.lib")

extern "C" {
void config_wait(HANDLE id);
void* wait_for_address();
}

void x64_inject_test() {

	STARTUPINFOW si = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION pi = {};

	if (CreateProcessW(L"C:/Windows/notepad.exe", NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
		//BOOL is_wow64 = false;
		//if (auto is_wow64_func = (decltype(&IsWow64Process))GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process")) {
			//is_wow64_func(pi.hProcess, &is_wow64);
		//}

		PROCESS_BASIC_INFORMATIONX<64> process_info;
		ULONG idgaf;
		NtQueryInformationProcess(pi.hProcess, ProcessBasicInformation, &process_info, sizeof(process_info), &idgaf);

		uintptr_t module_base;
		ReadProcessMemory(pi.hProcess, &process_info.PebBaseAddress->ImageBaseAddress, &module_base, sizeof(module_base), NULL);
		DWORD offset;
		ReadProcessMemory(pi.hProcess, (LPCVOID)(module_base + 0x3C), &offset, sizeof(DWORD), NULL);
		ReadProcessMemory(pi.hProcess, (LPCVOID)(module_base + offset + 0x28), &offset, sizeof(DWORD), NULL);

		uintptr_t entrypoint = module_base + offset;

		DWORD protection;
		VirtualProtectEx(pi.hProcess, (LPVOID)entrypoint, sizeof(infinite_loop), PAGE_EXECUTE_READWRITE, &protection);
		uint8_t old_bytes[sizeof(infinite_loop)];
		ReadProcessMemory(pi.hProcess, (LPCVOID)entrypoint, old_bytes, sizeof(old_bytes), NULL);
		WriteProcessMemory(pi.hProcess, (LPVOID)entrypoint, infinite_loop, sizeof(infinite_loop), NULL);
		FlushInstructionCache(pi.hProcess, (LPCVOID)entrypoint, sizeof(infinite_loop));

		CONTEXTX<64> context = {};
		context.ContextFlags = CONTEXT_CONTROL;
		do {
			ResumeThread(pi.hThread);
			Sleep(10);
			SuspendThread(pi.hThread);
			GetThreadContext(pi.hThread, (CONTEXT*)&context);
		} while (entrypoint != context.Rip);

		WriteProcessMemory(pi.hProcess, (LPVOID)entrypoint, old_bytes, sizeof(old_bytes), NULL);
		VirtualProtectEx(pi.hProcess, (LPVOID)entrypoint, sizeof(old_bytes), protection, &protection);
		FlushInstructionCache(pi.hProcess, (LPCVOID)entrypoint, sizeof(old_bytes));

		void* inject_buffer = VirtualAllocEx(pi.hProcess, NULL, sizeof(dummy_dll_name), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		WriteProcessMemory(pi.hProcess, inject_buffer, dummy_dll_name, sizeof(dummy_dll_name), NULL);

		config_wait((HANDLE)process_info.UniqueProcessId);

		void* init_address = NULL;

		HANDLE thread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&LoadLibraryW, inject_buffer, 0, NULL);
		if (thread) {
			init_address = wait_for_address();
			WaitForSingleObject(thread, INFINITE);
			CloseHandle(thread);
		}

		VirtualFreeEx(pi.hProcess, inject_buffer, 0, MEM_RELEASE);

		if (init_address) {
			thread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)init_address, (LPVOID)module_base, 0, NULL);
			if (thread) {
				WaitForSingleObject(thread, INFINITE);
				CloseHandle(thread);
			}
		}

		ResumeThread(pi.hThread);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);


		//CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&LoadLibraryA, )
	}
}
#endif
*/

#include <atomic>

//#define _HAS_CXX20 1
//#include <bit>

// C++ Concurrency Named Requirements:
// BasicLockable
// Lockable
// Mutex
struct dllexport SpinLock {
	std::atomic<bool> flag;

	inline constexpr SpinLock() : flag(false) {}
	SpinLock(const SpinLock&) = delete;
	SpinLock& operator=(const SpinLock&) = delete;

	inline void lock() {
		while (this->flag.exchange(true, std::memory_order_acquire));
		std::atomic_thread_fence(std::memory_order_acquire);
	}
	inline bool try_lock() {
		bool ret = this->flag.exchange(true, std::memory_order_acquire);
		std::atomic_thread_fence(std::memory_order_acquire);
		return ret ^ true;
	}
	inline void unlock() {
		std::atomic_thread_fence(std::memory_order_release);
		this->flag.store(false, std::memory_order_release);
	}
};

template<typename T = size_t>
struct dllexport SpinQueue {
	std::atomic<T> data;

	inline constexpr SpinQueue() : data(0) {}
	SpinQueue(const SpinQueue&) = delete;
	SpinQueue& operator=(const SpinQueue&) = delete;

	inline void lock() {
		if (T ticket = this->data.fetch_add(1, std::memory_order_acquire)) {
			do {
			} while (this->data.load(std::memory_order_relaxed) != ticket);
		}
		std::atomic_thread_fence(std::memory_order_acquire);
	}
	inline void unlock() {
		std::atomic_thread_fence(std::memory_order_release);
		this->data.fetch_sub(1, std::memory_order_release);
	}
};

template struct SpinQueue<size_t>;

void make_shift_jis_conversion_table() {
	wchar_t table[0x10000][2] = {};

	std::string output = "";
	for (size_t i = 0; i < 0x10000; ++i) {
		if (i % 256 == 0) {
			output += '\n';
		}
		int length = 0;
		if (i < 0x80 || (i & 0x80)) {
			length = MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, (LPCCH)&i, 1 + (i > 255), table[i], 2);
		}
		output += '(';
		if (length > 0) {
			output += std::to_string((uint16_t)table[i][0]);
		}
		if (length > 1) {
			output += ',' + std::to_string((uint16_t)table[i][1]);
		}
		output += ')';
	}
	if (FILE* dump = fopen("char_dump.txt", "wb")) {
		fwrite(output.data(), 1, output.length(), dump);
		fclose(dump);
	}
}

//#define FAR_CALL_IMM(seg, addr, ret, ...) __asm__ volatile ("lcall %[Seg],%[Addr]":ret: [Seg]"i"(seg), [Addr]"i"(addr) __VA_OPT__(,) __VA_ARGS__)
//#define FAR_JUMP_IMM(seg, addr, ret, ...) __asm__ volatile ("ljmp %[Seg],%[Addr]":ret: [Seg]"i"(seg), [Addr]"i"(addr) __VA_OPT__(,) __VA_ARGS__)

gnu_noinline void segment_jank_test() {

	constexpr uint64_t loop_iters = 1000000000ULL;
	constexpr uint64_t counter_add = UINT64_MAX / loop_iters;

	struct Dummy {
		void* addr;
		uint16_t seg;
	};

	uint64_t counter;

	const uint32_t ss_segment = 0x2B;

	builtin_start_time = rdtsc();
	counter = 0;
	do {
		write_ss(ss_segment);
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	builtin_end_time = rdtsc();


	custom1_start_time = rdtsc();
	counter = 0;
	do {
		if (store_ss() != ss_segment) {
			write_ss(ss_segment);
		}
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	custom1_end_time = rdtsc();

	static Dummy lss_test{
		.addr = (void*)esp_reg,
		.seg = ss_segment
	};

	custom2_start_time = rdtsc();
	counter = 0;
	do {
		__asm__ volatile (
			INTEL_SYNTAX_DIRECTIVE
			"LSS ESP, FWORD PTR [%V[addr]]"
			:
			: [addr]"r"(&lss_test)
		);
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	custom2_end_time = rdtsc();

	/*
	custom4_start_time = rdtsc();
	counter = 0;
	do {
		__asm__ volatile goto (
			".byte 0x9A \n"
			".int %c[far_ret_i] \n"
			".word %c[Seg]"
			:
			: [Seg]"i"(0x33), [far_ret_i]"i"(&&far_ret)
			:
			: far_ret, piss
		);
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	custom4_end_time = rdtsc();

	static const Dummy far_jump{
		.addr = &&jump_back_ret,
		.seg = 0x23
	};
	
	custom5_start_time = rdtsc();
	counter = 0;
	do {
		__asm__ volatile goto (
			".byte 0xEA \n"
			".int %c[far_jump_back_i] \n"
			".word %c[Seg] \n"
			".byte 0xCC"
			:
			: [Seg]"i"(0x33), [far_jump_back_i]"i"(&&far_jump_back)
			:
			: far_jump_back
		);
	jump_back_ret:;
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	custom5_end_time = rdtsc();
	*/

	printf(
		"SS Always Time:  %llu\n"
		"SS Cond Time:    %llu\n"
		"LSS Always Time: %llu\n"
		//"Far Call Time:   %llu\n"
		//"Far Jump2 Time:  %llu\n"
		//"Far IRET Time:   %llu\n"
		, builtin_end_time - builtin_start_time
		, custom1_end_time - custom1_start_time
		, custom2_end_time - custom2_start_time
		//, custom3_end_time - custom3_start_time
		//, custom4_end_time - custom4_start_time
	);

	return;

	/*
piss:
	__asm__ volatile(
		".align 16, 0xCC \n"
	);
far_ret:
	__asm__ volatile(
		"lret \n"
		".align 16, 0xCC \n"
	);
far_jump_back:
	__asm__ volatile(
		ATT_64_DIRECTIVE
		"ljmp *%[far_jump] \n"
		".align 16, 0xCC \n"
		:
		: [far_jump]"m"(far_jump)
	);
	*/
}

std::atomic<uint8_t> atomic_test;

uint64_t exchange_start_time, exchange_end_time;
uint64_t cmpexchange_start_time, cmpexchange_end_time;
uint64_t bts_start_time, bts_end_time;
gnu_noinline void atomic_timing_test() {

	constexpr uint64_t loop_iters = 1000000000ULL;
	constexpr uint64_t counter_add = UINT64_MAX / loop_iters;

	uint64_t counter;

	exchange_start_time = rdtsc();
	counter = 0;
	do {
		if (!atomic_test.exchange(true)) {
			//printf("test");
			__asm__ volatile ("nop":);
		}
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	exchange_end_time = rdtsc();

	cmpexchange_start_time = rdtsc();
	counter = 0;
	do {
		uint8_t expected = false;
		if (atomic_test.compare_exchange_strong(expected, true)) {
			//printf("test");
			__asm__ volatile ("nop":);
		}
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	cmpexchange_end_time = rdtsc();

	bts_start_time = rdtsc();
	counter = 0;
	do {
		int carry_flag;
		__asm__(
			"lock btsl %[bit_offset], %[atomic_test]"
			: asm_arg("+m", atomic_test), asm_flags(c, carry_flag)
			: [bit_offset] "Ns"(0)
		);
		if (!carry_flag) {
			//printf("test");
			__asm__ volatile ("nop":);
		}
	} while (!__builtin_add_overflow(counter, counter_add, &counter));
	bts_end_time = rdtsc();

	printf(
		"XCHG:    %llu\n"
		"CMPXCHG: %llu\n"
		"BTS:     %llu\n"
		, exchange_end_time - exchange_start_time
		, cmpexchange_end_time - cmpexchange_start_time
		, bts_end_time - bts_start_time
	);
}

uint64_t subjnz_start_time, subjnz_end_time;
uint64_t decjnz_start_time, decjnz_end_time;
uint64_t loop_start_time, loop_end_time;
gnu_noinline void loop_timing_test() {

	uint32_t xor_shift_state;
	while (!(xor_shift_state = rdtsc()));
	auto xor_shift = [&]() {
		uint32_t val = xor_shift_state;
		val ^= val << 13;
		val ^= val >> 17;
		val ^= val << 5;
		xor_shift_state = val;
		return val;
	};

	//uint64_t initial_tsc = rdtsc();
	//while (rdtsc() - initial_tsc < 0x400000000);
	//__asm INT3

	subjnz_start_time = rdtsc();
	uint32_t sub_counter = 0x10000000;
	/*
	__asm__ volatile (
		".align 16 \n"
	"0: \n"
		"SUB $1, %[counter] \n"
		"JNZ 0b \n"
		: [counter]"+c"(sub_counter)
	);
	*/
sub_loop:
	__asm__ volatile goto (
		"SUB $1, %[counter] \n"
		"JZ 0f \n"
		"TEST %[x], %[x] \n"
		"JNZ %l3 \n"
	"0: \n"
		: [counter] "+c"(sub_counter)
		: [x]"r"(xor_shift())
		:
		: sub_loop
	);
	subjnz_end_time = rdtsc();

	decjnz_start_time = rdtsc();
	sub_counter = 0x10000000;
	/*
	__asm__ volatile (
		".align 16 \n"
	"0: \n"
		"DEC %[counter] \n"
		"JNZ 0b \n"
		: [counter]"+c"(sub_counter)
	);
	*/
dec_loop:
	__asm__ volatile goto (
		"DEC %[counter] \n"
		"JZ 0f \n"
		"TEST %[x], %[x] \n"
		"JNZ %l3 \n"
	"0: \n"
		: [counter]"+c"(sub_counter)
		: [x]"r"(xor_shift())
		:
		: dec_loop
	);
	decjnz_end_time = rdtsc();

	loop_start_time = rdtsc();
	sub_counter = 0x10000000;
	/*
	__asm__ volatile (
		".align 16 \n"
	"0: \n"
		".byte 0x67 \n"
		"LOOP 0b \n"
		: [counter]"+c"(sub_counter)
	);
	*/
loop_loop:
	__asm__ volatile goto (
		//"TEST %[x], %[x] \n"
		"LOOPNZ %l3 \n"
		: [counter] "+c"(sub_counter)
		: [x]"r"(xor_shift())
		:
		: loop_loop
	);
	loop_end_time = rdtsc();

	printf(
		"SUB/JNZ: %llu\n"
		"DEC/JNZ: %llu\n"
		"LOOP:    %llu\n"
		, subjnz_end_time - subjnz_start_time
		, decjnz_end_time - decjnz_start_time
		, loop_end_time - loop_start_time
	);
}

#if CUSTOM_CLANG
dllexport int read_es_ptr(ESPTR<int> ptr) {
	return *ptr;
}
#endif

//#include "win_syscalls/syscalls7.h"
//#include "win_syscalls/common.h"

NTSTATUS NtQueryVirtualMemoryX(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);

dllexport decltype(NtQueryVirtualMemoryX)* NtQueryVirtualMemory;

ULONG NTAPI BaseSetLastNtErrorX(NTSTATUS Status) asm("BaseSetLastNtError");

dllexport decltype(BaseSetLastNtErrorX)* BaseSetLastNtError;

static ULONG OfferVirtualMemoryInternal(
	IN	PVOID			VirtualAddress,
	IN	SIZE_T			Size,
	IN	OFFER_PRIORITY	Priority,
	IN	BOOL			DiscardMemory
) {
	NTSTATUS Status;
	MEMORY_BASIC_INFORMATION BasicInformation;
	PVOID VirtualAllocResult;
	ULONG OldProtect;

	//
	// Parameter validation.
	//

	if (
		expect(
		(!VirtualAddress || !Size) ||
		// The virtual address must be page-aligned.
		((ULONG_PTR)VirtualAddress & 0xFFF) ||
		// The size must be a multiple of the page size.
		(Size & 0xFFF), false)
	) {
		return ERROR_INVALID_PARAMETER;
	}

	

	//
	// Check to see if the memory region provided is valid.
	// The entire region must be readable, writable, and committed.
	//

	Status = NtQueryVirtualMemory(
		NtCurrentProcess(),
		VirtualAddress,
		MemoryBasicInformation,
		&BasicInformation,
		sizeof(BasicInformation),
		NULL);

	if (expect(!NT_SUCCESS(Status), false)) {
		return BaseSetLastNtError(Status);
	}

	if (expect(BasicInformation.RegionSize < Size ||
		BasicInformation.Protect != PAGE_READWRITE ||
		BasicInformation.State != MEM_COMMIT, false)
	) {
		// STATUS_INVALID_PAGE_PROTECTION
		return BaseSetLastNtError(0xC0000045);
	}

	//
	// Tell the kernel that we won't be needing the contents of this memory
	// anymore.
	//
	VirtualAllocResult = VirtualAlloc(
		VirtualAddress,
		Size,
		MEM_RESET,
		PAGE_READWRITE);

	if (expect(VirtualAllocResult != VirtualAddress, false)) {
		return GetLastError();
	}

	if (DiscardMemory) {
		VirtualUnlock(VirtualAddress, Size);
	} else {
		// If OfferVirtualMemory was called, then make those pages
		// inaccessible.
		VirtualProtect(VirtualAddress, Size, PAGE_NOACCESS, &OldProtect);
	}

	return ERROR_SUCCESS;
}

dllexport ULONG WINAPI DiscardVirtualMemory(
	IN	PVOID	VirtualAddress,
	IN	SIZE_T	Size
) {
	return OfferVirtualMemoryInternal(VirtualAddress, Size, VmOfferPriorityVeryLow, TRUE);
}

/*
#include <Shlobj.h>

void wtf_windows() {
	FOLDERID_LocalAppData
}
*/

uint8_t* emit_IE_gen(uint8_t* dest) {
	unsigned int random;
	rand_s(&random);
	random %= 402;
	switch (random) {
		default: unreachable;
		case 0 ... 63:
			*dest++ = 0xD8;
			*dest++ = 0xC0 + random;
			break;
		case 64 ... 79:
			*dest++ = 0xD9;
			*dest++ = 0xC0 + (random - 64);
			break;
		case 80: case 81:
			*dest++ = 0xD9;
			*dest++ = 0xE0 + (random - 80);
			break;
		case 82 ... 87:
			*dest++ = 0xD9;
			*dest++ = 0xF0 + (random - 82);
			break;
		case 88 ... 95:
			*dest++ = 0xD9;
			*dest++ = 0xF8 + (random - 88);
			break;
		case 96 ... 127:
			*dest++ = 0xDA;
			*dest++ = 0xC0 + (random - 96);
			break;
		case 128:
			*dest++ = 0xDA;
			*dest++ = 0xE9;
			break;
		case 129 ... 160:
			*dest++ = 0xDB;
			*dest++ = 0xC0 + (random - 129);
			break;
		case 161 ... 176:
			*dest++ = 0xDB;
			*dest++ = 0xE8 + (random - 161);
			break;
		case 177 ... 240:
			*dest++ = 0xDC;
			*dest++ = 0xC0 + (random - 177);
			break;
		case 241 ... 280:
			*dest++ = 0xDD;
			*dest++ = 0xC8 + (random - 241);
			break;
		case 281 ... 304:
			*dest++ = 0xDE;
			*dest++ = 0xC0 + (random - 281);
			break;
		case 305:
			*dest++ = 0xDE;
			*dest++ = 0xD9;
			break;
		case 306 ... 337:
			*dest++ = 0xDE;
			*dest++ = 0xE0 + (random - 306);
			break;
		case 338 ... 361:
			*dest++ = 0xDF;
			*dest++ = 0xC8 + (random - 338);
			break;
		case 362 ... 377:
			*dest++ = 0xDF;
			*dest++ = 0xE8 + (random - 362);
			break;
		case 378 ... 385:
			*dest++ = 0xD8;
			*dest++ = 0x04 + (random - 378 << 3);
			*dest++ = 0x24;
			break;
		case 386 ... 393:
			*dest++ = 0xDA;
			*dest++ = 0x04 + (random - 386 << 3);
			*dest++ = 0x24;
			break;
		case 394 ... 401:
			*dest++ = 0xDE;
			*dest++ = 0x04 + (random - 394 << 3);
			*dest++ = 0x24;
			break;
	}
	return dest;
}

uint8_t* emit_IE_nop(uint8_t* dest) {
	unsigned int random;
	rand_s(&random);
	random %= 41;
	switch (random) {
		default: unreachable;
		case 0:
			*dest++ = 0xD9;
			*dest++ = 0xD0;
			break;
		case 1 ... 8:
			*dest++ = 0xD9;
			*dest++ = 0xD8 + (random - 1);
			break;
		case 9:
			*dest++ = 0xD9;
			*dest++ = 0xE5;
			break;
		case 10 ... 16:
			*dest++ = 0xD9;
			*dest++ = 0xE8 + (random - 10);
			break;
		case 17: case 18:
			*dest++ = 0xD9;
			*dest++ = 0xF6 + (random - 17);
			break;
		case 19: case 20:
			*dest++ = 0xDB;
			*dest++ = 0xE0 + (random - 19);
			break;
		case 21:
			*dest++ = 0xDB;
			*dest++ = 0xE4;
			break;
		case 22 ... 29:
			*dest++ = 0xDD;
			*dest++ = 0xC0 + (random - 22);
			break;
		case 30 ... 37:
			*dest++ = 0xDF;
			*dest++ = 0xC0 + (random - 30);
			break;
		case 38:
			*dest++ = 0xD9;
			*dest++ = 0x04;
			*dest++ = 0x24;
			break;
		case 39:
			*dest++ = 0xDB;
			*dest++ = 0x04;
			*dest++ = 0x24;
			break;
		case 40:
			*dest++ = 0xDF;
			*dest++ = 0x04;
			*dest++ = 0x24;
			break;
	}
	return dest;
}

uint8_t* emit_fp_exception_clear(uint8_t* code) {
	unsigned int random;
	rand_s(&random);
	if (random & 1) {
		*code++ = 0xDB;
		*code++ = 0xE3;
	}
	else {
		if (random & 2) {
			*code++ = 0xDB;
			*code++ = 0xE2;
			*code++ = 0x0F;
			*code++ = 0x77;
		}
		else {
			*code++ = 0x0F;
			*code++ = 0x77;
			*code++ = 0xDB;
			*code++ = 0xE2;
		}
	}
	return code;
}

uint8_t get_random_rexw() {
	unsigned int random;
	rand_s(&random);
	return (random & 1) << 1 | 0x48;
}

template <typename T>
gnu_noinline uint8_t* replace_imm_with_bs(T value, uint8_t* code) {
	*code++ = 0x0F; *code++ = 0xA0; // PUSH FS
	*code++ = 0xD9; *code++ = 0x3C; *code++ = 0x24; // FSTCW [ESP]
	for (size_t i = 0; i < sizeof(T) * CHAR_BIT; ++i) {
		code = emit_fp_exception_clear(code);
		if (value >> i & 1) {
			code = emit_IE_gen(code);
		}
		else {
			code = emit_IE_nop(code);
		}
		*code++ = 0xDF; *code++ = 0xE0; // FSTSW AX
		if constexpr (sizeof(T) <= sizeof(int16_t)) {
			*code++ = 0xC1; *code++ = 0xC8; *code++ = get_random_shift_count<false>(0x01); // ROR EAX, 1
		}
		else if constexpr (sizeof(T) == sizeof(int32_t)) {
			*code++ = 0x0F; *code++ = 0xAC; *code++ = 0xC2; *code++ = get_random_shift_count<false>(0x01); // SHRD EDX, EAX, 1
		}
		else if constexpr (sizeof(T) == sizeof(int64_t)) {
			*code++ = get_random_rexw(); *code++ = 0x0F; *code++ = 0xAC; *code++ = 0xC2; *code++ = get_random_shift_count<true>(0x01); // SHRD RDX, RAX, 1
		}
	}
	if constexpr (sizeof(T) == sizeof(int8_t)) {
		*code++ = 0xC1; *code++ = 0xF8; *code++ = get_random_shift_count<false>(0x18); // SAR EAX, 24
	}
	else if constexpr (sizeof(T) == sizeof(int16_t)) {
		*code++ = 0xC1; *code++ = 0xF8; *code++ = get_random_shift_count<false>(0x10); // SAR EAX, 16
	}
	else if constexpr (sizeof(T) == sizeof(int32_t)) {
		*code++ = 0x89; *code++ = 0xD0; // MOV EAX, EDX
	}
	else if constexpr (sizeof(T) == sizeof(int64_t)) {
		*code++ = get_random_rexw(); *code++ = 0x89; *code++ = 0xD0; // MOV RAX, RDX
	}
	*code++ = 0xD9; *code++ = 0x2C; *code++ = 0x24; // FLDCW [ESP]
	*code++ = 0x5A; // POP EDX
	return code;
}

#include <mmsystem.h>

struct JankSound {

};

struct JankSoundManager {
	HWAVEOUT wave_out;


	// I just ripped this out of the normal UM code
	static inline constexpr WAVEFORMATEX WAVE_FORMAT = {
		.wFormatTag = WAVE_FORMAT_1S08,
		.nChannels = 2,
		.nSamplesPerSec = 44100,
		.nAvgBytesPerSec = 176400,
		.nBlockAlign = 4,
		.wBitsPerSample = 16,
		.cbSize = 0
	};

	bool initialize() {
		if (MMSYSERR_NOERROR == waveOutOpen(&this->wave_out, WAVE_MAPPER, &WAVE_FORMAT, NULL, NULL, CALLBACK_NULL)) {

		}
		return false;
	}
};

JankSoundManager JANK_SOUND_MANAGER;

//using Token = std::string;

enum TokenType : uint8_t {
	QuotationStart = '(',
	QuotationEnd = ')',
	Swap = '~',
	Duplicate = '+',
	Nuke = '-',
	Ungroup = '<',
	Group = '>',
	Merge = ','
};

static inline constexpr size_t TOKEN_TYPES = 8;

enum ParseResult {
	ErrorParse,
	NormalParse,
	IrreducibleParse
};

struct Token {
	size_t length;
	char buffer[];

	bool is_parenthesized() const {
		return this->buffer[0] == '(' && this->buffer[this->length - 1] == ')';
	}
};

struct TokenList {
	size_t count;
	Token* temp;
	Token* tokens[];
};

void cleanup_tokens(const TokenList*volatile list) {
	volatile size_t list_length = list->count;
	for (volatile size_t i = 0; i < list_length; ++i) {
		free(list->tokens[i]);
	}
	if (Token* temp = list->temp) {
		free(temp);
	}
}

Token* allocate_token(const char* start, size_t length) {
	if (Token* token = (Token*)malloc(sizeof(Token) + length)) {
		token->length = length;
		memcpy(token->buffer, start, length);
		return token;
	}
	return NULL;
}

#define USE_STACK_LIMIT 0

#if !_WIN32
#undef USE_STACK_LIMIT
#define USE_STACK_LIMIT 1
#endif

#if USE_STACK_LIMIT
#define STACK_LIMIT_ARG , size_t depth = 0
#define STACK_LIMIT_NEXT , depth + 1
#else
#define STACK_LIMIT_ARG
#define STACK_LIMIT_NEXT
#endif

static inline constexpr size_t MAX_DEPTH = 1000;

gnu_noinline ParseResult tokenize_inner(TokenList*& master_token_list, TokenList*& master_source_list STACK_LIMIT_ARG) {

#if USE_STACK_LIMIT
	if (depth == MAX_DEPTH) {
		return ErrorParse;
	}
#endif

	TokenList* source_list = master_source_list;
	Token* source_token = source_list->tokens[source_list->count - 1];
	size_t length = source_token->length;
	const char* string = source_token->buffer;

	{
		// Early pass to avoid recursion
		// into invalid patterns
		size_t paren_depth = 0;
		for (size_t i = 0; i < length; ++i) {
			switch (string[i]) {
				case QuotationStart:
					++paren_depth;
					break;
				case QuotationEnd:
					if (!paren_depth--) return ErrorParse;
					break;
			}
		}
		if (paren_depth) return ErrorParse;
	}

	TokenList* token_list = master_token_list;

#if !USE_STACK_LIMIT
	// Evil hack to make sure the stack
	// won't overflow in the middle of a
	// heap management libcall
	__asm TEST BYTE PTR [ESP-0x1000], 0
#endif

	for (size_t i = 0; i < length; ++i) {
		switch (string[i]) {
			case QuotationStart: {
				size_t paren_depth = 1;
				for (size_t j = i + 1; j < length; ++j) {
					switch (string[j]) {
						case QuotationStart:
							++paren_depth;
							break;
						case QuotationEnd:
							if (!--paren_depth) {
								if (Token* new_token = allocate_token(&string[i], (j - i) + 1)) {
									size_t prev_count = token_list->count;
									token_list->temp = new_token;
									if (TokenList* new_list = (TokenList*)realloc(token_list, sizeof(TokenList) + sizeof(Token*) * (prev_count + 1))) {
										master_token_list = token_list = new_list;
										token_list->temp = NULL;
										token_list->tokens[prev_count] = new_token;
										token_list->count = prev_count + 1;
										i = j;
										goto next_char;
									}
									token_list->temp = NULL;
									free(new_token);
								}
								cleanup_tokens(token_list);
								free(token_list);
								master_token_list = NULL;
								return ErrorParse;
							}
							break;
					}
				}
			}
			case QuotationEnd:
				cleanup_tokens(token_list);
				free(token_list);
				master_token_list = NULL;
				return ErrorParse;
			case Swap:
				switch (size_t count = token_list->count) {
					default:
						std::swap(token_list->tokens[count - 1], token_list->tokens[count - 2]);
						continue;
					case 1:
						free(token_list->tokens[0]);
					case 0:
						free(token_list);
						master_token_list = NULL;
						return ErrorParse;
				}
			case Duplicate:
				if (size_t prev_count = token_list->count) {
					Token* token = token_list->tokens[prev_count - 1];
					if (Token* new_token = allocate_token(token->buffer, token->length)) {
						token_list->temp = new_token;
						if (TokenList* new_list = (TokenList*)realloc(token_list, sizeof(TokenList) + sizeof(Token*) * (prev_count + 1))) {
							master_token_list = token_list = new_list;
							token_list->temp = NULL;
							token_list->tokens[prev_count] = new_token;
							token_list->count = prev_count + 1;
							continue;
						}
						token_list->temp = NULL;
						free(new_token);
					}
					cleanup_tokens(token_list);
				}
				free(token_list);
				master_token_list = NULL;
				return ErrorParse;
			case Nuke:
				if (size_t count = token_list->count) {
					--count;
					free(token_list->tokens[count]);
					token_list->count = count;
					continue;
				}
				free(token_list);
				master_token_list = NULL;
				return ErrorParse;
			case Ungroup:
				if (size_t count = token_list->count) {
					Token* token = token_list->tokens[count - 1];
					if (!token->is_parenthesized()) {
						goto irreducible;
					}

					size_t source_count = source_list->count;
					if (TokenList* new_list = (TokenList*)realloc(source_list, sizeof(TokenList) + sizeof(Token*) * (source_count + 1))) {
						master_source_list = source_list = new_list;
						token_list->count = count - 1;
						source_list->temp = NULL;
						source_list->tokens[source_count] = token;
						source_list->count = source_count + 1;

						size_t old_size = token->length;
						token->length -= 2;
						size_t new_size = old_size - 2;
						for (size_t i = 0; i < new_size; ++i) {
							token->buffer[i] = token->buffer[i + 1];
						}

						switch (ParseResult result = tokenize_inner(master_token_list, master_source_list STACK_LIMIT_NEXT)) {
							default: // ErrorParse
								// no cleanup here because the inner call
								// will already have freed everything
								return result;
							case NormalParse:
								token_list = master_token_list;
								source_list = master_source_list;
								continue;
							case IrreducibleParse:
								token_list = master_token_list;
								source_list = master_source_list;
								++i;
								goto irreducible;
						}
					}
					cleanup_tokens(token_list);
				}
				free(token_list);
				master_token_list = NULL;
				return ErrorParse;
			case Group:
				if (size_t count = token_list->count) {
					Token* token = token_list->tokens[count - 1];
					size_t old_size = token->length;
					if (Token* bigger_token = (Token*)realloc(token, sizeof(Token) + old_size + 2)) {
						token_list->tokens[count - 1] = bigger_token;
						bigger_token->length += 2;
						size_t write_offset = old_size + 1;
						bigger_token->buffer[write_offset] = ')';
						while (--write_offset) {
							bigger_token->buffer[write_offset] = bigger_token->buffer[write_offset - 1];
						}
						bigger_token->buffer[0] = '(';
						continue;
					}
					cleanup_tokens(token_list);
				}
				free(token_list);
				master_token_list = NULL;
				return ErrorParse;
			case Merge:
				switch (size_t count = token_list->count) {
					default: {
						Token* left = token_list->tokens[count - 2];
						Token* right = token_list->tokens[count - 1];
						if (!left->is_parenthesized() || !right->is_parenthesized()) {
							goto irreducible;
						}
						size_t old_size = left->length;
						size_t expansion = right->length;
						if (Token* bigger_left = (Token*)realloc(left, sizeof(Token) + old_size + expansion - 2)) {
							token_list->tokens[count - 2] = bigger_left;
							bigger_left->length += expansion - 2;
							memcpy(bigger_left->buffer + old_size - 1, right->buffer + 1, expansion - 1);
							free(right);
							token_list->count = count - 1;
							continue;
						}
						cleanup_tokens(token_list);
						break;
					}
					case 1:
						free(token_list->tokens[0]);
					case 0:;
				}
				free(token_list);
				master_token_list = NULL;
				return ErrorParse;
			default:
				goto irreducible;
		}
	irreducible:
		if (!(length - i)) {
			return IrreducibleParse;
		}
		if (Token* new_token = allocate_token(&string[i], length - i)) {
			size_t prev_count = token_list->count;
			token_list->temp = new_token;
			if (TokenList* new_list = (TokenList*)realloc(token_list, sizeof(TokenList) + sizeof(Token*) * (prev_count + 1))) {
				master_token_list = token_list = new_list;
				token_list->temp = NULL;
				token_list->tokens[prev_count] = new_token;
				token_list->count = prev_count + 1;
				return IrreducibleParse;
			}
			token_list->temp = NULL;
			free(new_token);
		}
		cleanup_tokens(token_list);
		free(token_list);
		master_token_list = NULL;
		return ErrorParse;
	next_char:;
	}
	return NormalParse;
}

/*
gnu_noinline ParseResult tokenize_inner(std::vector<Token>& tokens, std::string_view string) {
	const char* str = string.data();

	size_t length = string.length();

	{
		// Early pass to avoid recursion
		// into invalid patterns
		size_t paren_depth = 0;
		for (size_t i = 0; i < length; ++i) {
			switch (str[i]) {
				case QuotationStart:
					++paren_depth;
					break;
				case QuotationEnd:
					if (!paren_depth--) return ErrorParse;
					break;
			}
		}
		if (paren_depth) {
			return ErrorParse;
		}
	}

	for (size_t i = 0; i < length; ++i) {
		switch (string[i]) {
			case QuotationStart: {
				size_t paren_depth = 1;
				for (size_t j = i + 1; j < length; ++j) {
					switch (str[j]) {
						case QuotationStart:
							++paren_depth;
							break;
						case QuotationEnd:
							if (!--paren_depth) {
								tokens.emplace_back(&str[i], (j - i) + 1);
								i = j;
								goto next_char;
							}
							break;
					}
				}
			}
			case QuotationEnd:
				return ErrorParse;
			case Swap:
				if (tokens.size() <= 1) return ErrorParse;
				std::swap(tokens.back(), tokens.end()[-2]);
				break;
			case Duplicate:
				if (tokens.size() == 0) return ErrorParse;
				tokens.reserve(tokens.size() + 1);
				tokens.push_back(tokens.back());
				break;
			case Nuke:
				if (tokens.size() == 0) return ErrorParse;
				tokens.pop_back();
				break;
			case Ungroup: {
				if (tokens.size() == 0) return ErrorParse;
				if (tokens.back().starts_with(QuotationStart) && tokens.back().ends_with(QuotationEnd)) {
					Token prev_token = tokens.back();
					tokens.pop_back();
					std::string_view token_view = prev_token;
					token_view.remove_prefix(1);
					token_view.remove_suffix(1);
					switch (tokenize_inner(tokens, token_view)) {
						default:
							unreachable;
						case NormalParse:
							break;
						case IrreducibleParse:
							if (i + 1 != length) {
								tokens.reserve(tokens.size() + 1);
								tokens.emplace_back(&str[i + 1], length - (i + 1));
							}
							return IrreducibleParse;
						case ErrorParse:
							return ErrorParse;
					}
					break;
				}
			}
			default:
				tokens.emplace_back(&str[i], length - i);
				return IrreducibleParse;
			case Group:
				if (tokens.size() == 0) return ErrorParse;
				tokens.back().insert(0, 1, '(').push_back(')');
				break;
			case Merge:
				if (tokens.size() <= 1) return ErrorParse;
				if (
					tokens.back().starts_with(QuotationStart) && tokens.back().ends_with(QuotationEnd) &&
					tokens.end()[-2].starts_with(QuotationStart) && tokens.end()[-2].ends_with(QuotationEnd)
				) {
					Token prev_token = tokens.back();
					tokens.pop_back();
					tokens.back().pop_back();
					tokens.back().append(prev_token.data() + 1, prev_token.size() - 1);
					break;
				}
				return ErrorParse;
		}
	next_char:;
	}
	return NormalParse;
}

//std::optional<Token>
std::pair<Token, bool> tokenize_whatever(std::string_view string) {
	std::vector<Token> tokens;
	if (tokenize_inner(tokens, string)) {
		Token ret;
		for (auto& token : tokens) {
			ret += token;
		}
		//return ret;
		return { std::move(ret), false };
	}
	return { {}, true };
}
*/

Token* tokenize_whatever(const char* string, size_t length) {
	
	if (Token* source_token = allocate_token(string, length)) {
		if (TokenList* master_token_list = (TokenList*)malloc(sizeof(TokenList))) {
			master_token_list->count = 0;
			master_token_list->temp = NULL;
			if (TokenList* master_source_list = (TokenList*)malloc(sizeof(TokenList) + sizeof(Token*))) {
				master_source_list->count = 1;
				master_source_list->temp = NULL;
				master_source_list->tokens[0] = source_token;

				Token* ret = NULL;
#if !USE_STACK_LIMIT
				__try {
#endif
					if (tokenize_inner(master_token_list, master_source_list) != ErrorParse) {
						if (size_t token_count = master_token_list->count) {
							size_t buffer_size = 0;
							for (size_t i = 0; i < token_count; ++i) {
								buffer_size += master_token_list->tokens[i]->length;
							}
							if (Token* return_token = (Token*)malloc(sizeof(Token) + buffer_size)) {
								ret = return_token;
								return_token->length = buffer_size;
								char* token_write = return_token->buffer;
								for (size_t i = 0; i < token_count; ++i) {
									Token* token = master_token_list->tokens[i];
									size_t length = token->length;
									token_write = (char*)memcpy(token_write, token->buffer, length);
									token_write += length;
								}
							}
						}
					}
#if !USE_STACK_LIMIT
				}
				__except (
					GetExceptionCode() == STATUS_STACK_OVERFLOW ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH
				) {
					// At least fix the stack but otherwise ignore it
					_resetstkoflw();
					if (ret) {
						free(ret);
						ret = NULL;
					}
				}
#endif
				if (master_token_list) {
					cleanup_tokens(master_token_list);
					free(master_token_list);
				}
				if (master_source_list) {
					cleanup_tokens(master_source_list);
					free(master_source_list);
				}

				return ret;
			}
			free(master_token_list);
		}
		free(source_token);
	}
	return NULL;
}
/*
std::pair<Token, bool> tokenize_whatever_safeB(std::string_view string) {
	try {
		return tokenize_whatever(string);
	}
	catch (...) {
		//Sleep(30);
	}
	return { {}, true };
}

std::pair<Token, bool> tokenize_whatever_safe(std::string_view string) {
	__try {
		return tokenize_whatever_safeB(string);
	}
	__except (
		GetExceptionCode() == STATUS_STACK_OVERFLOW ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH
	) {
		// At least fix the stack but otherwise ignore it
		_resetstkoflw();
	}
	return { {}, true };
}
*/
static constexpr uint8_t token_chars[256] = {
	[0] = QuotationStart,
	[QuotationStart] = QuotationEnd,
	[QuotationEnd] = Swap,
	[Swap] = Duplicate,
	[Duplicate] = Nuke,
	[Nuke] = Ungroup,
	[Ungroup] = Group,
	[Group] = Merge,
	[Merge] = 0,
};

gnu_noinline void find_shortest_patterns(std::string_view prefix, std::string_view known_good) {
	char buffer[prefix.length() + known_good.length() + 1];
	buffer[prefix.length() + known_good.length()] = '\0';
	memcpy(buffer, prefix.data(), prefix.length());
	char* test = &buffer[prefix.length()];
	memcpy(test, known_good.data(), known_good.length());

	const char *volatile buffer_str = buffer;
	
	Token* correct = tokenize_whatever(buffer, prefix.length() + known_good.length());
	if (!correct) {
		printf("Parse error in known good pattern");
		return;
	}

	memset(test, 0, known_good.length());

#if !USE_STACK_LIMIT
	ULONG stack_handler_size = 1024;
	SetThreadStackGuarantee(&stack_handler_size);
#endif

	std::vector<std::string> matches;
	size_t min_length = known_good.length();

	const char *volatile test_str = test;

	/*
		Filter patterns:
		Nops:		+- ~~ >< >- )- )<
		Invalid:	()
					Start with )
					End with ( ) )+ )> ), )~
		Dupes:		+~	 = +
					>-	 = -
					,-	 = --
					~--	 = --
					~,-  = -- (subcase of ,-)
					+>~  = >+<
					+>~> = >+
					>+<> = >+
	*/

	volatile size_t test_length = 1;
	for (;;) {
	next_permutation:
		size_t i = 0;
		for (;;) {
			uint8_t next = token_chars[test[i]];
			if (next) {
				switch (next) {
					case QuotationStart:
						// Skip patterns that end with an
						// open/empty group since those are
						// always invalid.
						if (
							i + 1 == test_length ||
							test[i + 1] == QuotationEnd
						) {
							next = QuotationEnd;
						} //else {
							break;
						//}
						// fallthrough is intentional
						/*
					case QuotationEnd:
						for (
							size_t j = i;
							j && test[j - 1] == QuotationStart;
							--j
						) {
							test[j - 1] = QuotationEnd;
						}
						break;
						*/
					case Swap:
						// Double swap *should* be a nop
						if (test[i + 1] != Swap) {
							break;
						}
						next = Duplicate;
						// fallthrough is intentional
					case Duplicate:
						if (test[i + 1] != Nuke) {
							break;
						}
						next = Nuke;
						break;
					case Group:
						if (test[i + 1] == Ungroup) {
							next = Merge;
						}
				}
				test[i] = next;
				break;
			} else {
				test[i++] = QuotationStart;
				if (i == min_length) {
					goto end_testing;
				}
				test_length += i == test_length;
			}
		}

		for (size_t j = 0; j < test_length - 1; ++j) {
			if (test[j] == '(' & test[j + 1] == ')') {
				goto next_permutation;
			}
		}

		// Left off at ~,+(,++<,)+<
		
		if (Token* token = tokenize_whatever(buffer, prefix.length() + test_length)) {
			const char* volatile str_data = token->buffer;
			size_t length = token->length;
			if (
				length == correct->length &&
				!memcmp(token->buffer, correct->buffer, length)
			) {
				if (test_length < min_length) {
					min_length = test_length;
					matches.clear();
				}
				matches.emplace_back(test, test_length);
			}
			else {
				free(token);
			}
		}
	}
end_testing:

	for (auto& match : matches) {
		printf("%s\n", match.data());
	}
}

gnu_noinline void test_token_parsing() {
	//Token test = tokenize_whatever("((B)(A)+-)<~,");
	//Token test = tokenize_whatever("(A)<<");
	//Token test = tokenize_whatever("(+<)+<");
	//printf("%s", test.c_str());

	/*
	auto [test, error] = tokenize_whatever("(B)(A)>~>,+(>~,),~(>,),,<");
	if (!error) {
		printf("%s", test.c_str());
	}
	*/

	find_shortest_patterns("(B)(A)", ">~>,+(>~,),~(>,),,<");
	//find_shortest_patterns("(A)", "+-+");
}


dllexport __attribute__((noinline)) void* cdecl read_file_to_buffer_test(char* filename, int* file_size, bool IsNotInDat) {
	static volatile auto wkjrbkr = filename;
	static volatile auto webewq = file_size;
	static volatile auto wkjeb = IsNotInDat;
	return (void*)rand();
}

dllexport void* cdecl text_file_strlen_fix(char* filename, int* file_size, bool IsNotInDat) {
	uint8_t* buffer = (uint8_t*)read_file_to_buffer_test(filename, file_size, IsNotInDat);
	if (buffer) {
		size_t size = _msize(buffer);
		if (uint8_t* new_buffer = (uint8_t*)realloc(buffer, size + 1)) {
			buffer = new_buffer;
			buffer[size] = '\0';
		}
	}
	return buffer;
}

#define LZSS_DICTSIZE 0x2000
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_MIN_MATCH 3

// Based on thtk's thlzss.c and https://fgiesen.wordpress.com/2018/02/20/reading-bits-in-far-too-many-ways-part-2/
dllexport uint8_t* cdecl unlzss_fast(uint8_t* __restrict src, size_t src_len, uint8_t* __restrict dst, size_t dst_len) {
	if (!dst) {
		dst = (uint8_t*)malloc(dst_len);
		if (!dst)
			return nullptr;
	}

	size_t src_pos = 0;
	size_t dst_pos = 0;
	uint32_t bit_buf = 0;
	uint8_t bit_count = 0;

	auto refill = [&](uint8_t count) __attribute__((always_inline)) {
		while (bit_count < count) {
			bit_buf |= (uint32_t)(__builtin_expect(src_pos == src_len, false) ? 0 : src[src_pos++]) << (24 - bit_count);
			bit_count += 8;
		}
	};
	auto get_bits = [&](uint8_t count) __attribute__((always_inline)) {
		uint32_t ret = bit_buf >> (32 - count);
		bit_buf <<= count;
		bit_count -= count;
		return ret;
	};

	while (true) {
		refill(1 + 13 + 4);
		if (get_bits(1)) {
			uint8_t c = get_bits(8);
			dst[dst_pos++] = c;
		} else {
			uint32_t match_offset = get_bits(13);
			if (__builtin_expect(!match_offset, false))
				break;

			uint32_t match_len = get_bits(4) + LZSS_MIN_MATCH;
			for (uint32_t i = 0; i < match_len; i++) {
				uint32_t dict_offset = (match_offset + i - 1) & LZSS_DICTSIZE_MASK;
				uint8_t c = dst[(dst_pos & ~LZSS_DICTSIZE_MASK) + dict_offset - (dict_offset >= (dst_pos & LZSS_DICTSIZE_MASK) ? LZSS_DICTSIZE : 0)];
				dst[dst_pos++] = c;
			}
		}
	}

	return dst;
}