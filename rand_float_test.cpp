#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <tuple>
#include <bit>
#include <array>

#define _USE_MATH_DEFINES 1
#include <math.h>

#include "zero/util.h"
#include "zero/custom_intrin.h"

#include <x86intrin.h>

static double U32_FLOAT_CVT[2] = {
	0.0,
	4294967296.0
};
static constexpr float F_PI = M_PI;

static constexpr float F32_DIVU32MAXP1 = bitcast<float>(0x2F800000); //0x1.0p-32f;
static constexpr float F32_DIVI32MAXP1 = bitcast<float>(0x30000000); // 0x1.0p-31f;
static constexpr float F32_ONE = bitcast<float>(0x3F800000); // 1.0f;
static constexpr float F32_I32MAXDIVPI = bitcast<float>(0x4E22F983);
static constexpr float F32_PI = bitcast<float>(0x40490FDB);

//static_assert(F32_U32MAXDIVPI == test);

//#ifdef _MSC_VER
//#include <intrin.h>
//#else
//#include <immintrin.h>
//#endif

struct MatchData {
	size_t match_count;
	size_t cycle_size;
};

//static std::set<uint16_t> RNG_WORD_SET; // INT16_MAX + 1
//static std::set<uint32_t> RNG_DWORD_SET_A; // UINT16_MAX + 1
//static std::set<uint32_t> RNG_DWORD_SET_B; // UINT16_MAX + 1
//static std::array<uint16_t, INT16_MAX + 1> RNG_WORD_ARRAY;
//static std::array<uint32_t, UINT16_MAX + 1> RNG_DWORD_ARRAY_A;
//static std::array<uint32_t, UINT16_MAX + 1> RNG_DWORD_ARRAY_B;

#define RNG_WORD_COUNT (INT16_MAX + 1)
#define RNG_DWORD_COUNT (UINT16_MAX + 1)
static union {
	uint16_t as_array[RNG_WORD_COUNT];
	vec<uint16_t, 16> as_vec[sizeof(uint16_t[RNG_WORD_COUNT]) / sizeof(vec<uint16_t, 16>)];
} RNG_WORDS;
static union {
	uint32_t as_array[RNG_DWORD_COUNT];
	vec<uint32_t, 8> as_vec[sizeof(uint32_t[RNG_DWORD_COUNT]) / sizeof(vec<uint32_t, 8>)];
} RNG_DWORDS_A;
static union {
	uint32_t as_array[RNG_DWORD_COUNT];
	vec<uint32_t, 8> as_vec[sizeof(uint32_t[RNG_DWORD_COUNT]) / sizeof(vec<uint32_t, 8>)];
} RNG_DWORDS_B;

template<typename T, size_t size>
static constexpr std::array<T, size> init_array(T init_value) {
	std::array<T, size> values;
	for (size_t i = 0; i < size; ++i) values[i] = init_value;
	return values;
}


static constexpr float yeetusA = (float)UINT32_MAX / (2.0f * F_PI);
static constexpr float yeetusA2 = (float)UINT32_MAX / (2.0f * F_PI);
static constexpr float yeetusB = (float)INT32_MAX / F_PI;
//static_assert(yeetusA == yeetusB);

struct Rng {
	uint16_t rng_value;
	uint32_t rng_index;

	static uint16_t run_rng(uint16_t value) {
		uint16_t temp = (value ^ 0x9630) - 0x6553;
		return ((temp & 0xC000) >> 14) + (temp << 2) & 0xFFFF;
	}

	uint16_t rand_ushort() {
		this->rng_index++;
		uint16_t temp = (this->rng_value ^ 0x9630) - 0x6553;
		this->rng_value = ((temp & 0xC000) >> 14) + (temp << 2) & 0xFFFF;
		return this->rng_value;
	}

	uint32_t rand_uint() {
		this->rng_index += 2;
		uint16_t tempA = (this->rng_value ^ 0x9630) - 0x6553;
		uint16_t tempB = ((tempA & 0xC000) >> 14) + (tempA << 2) & 0xFFFF;
		tempB = (tempB ^ 0x9630) - 0x6553;
		this->rng_value = ((tempB & 0xC000) >> 14) + (tempB << 2) & 0xFFFF;
		return (tempA << 16) | tempB;
	}

//#define UseZUNVer

	void more_rng_cycles() {
		static uint8_t cycle_seen[UINT16_MAX + 1] = {};
		static volatile uint8_t cycle_seen_end;
		uint8_t index = 0;
		for (
			uint_fast16_t i = 0;
			i < countof(cycle_seen);
			++i
		) {
			uint16_t rng_value = i;
			if (!cycle_seen[rng_value]) {
				++index;
				do {
					cycle_seen[rng_value] = index;
					rng_value = this->run_rng(rng_value);
				} while (!cycle_seen[rng_value]);
			}
		}
		index = 0;
		for (
			uint_fast16_t i = 0;
			i < countof(cycle_seen);
			++i
		) {
			index += !cycle_seen[i];
		}
		printf(
			"RNG Cycles: %hhu\n"
			, index
		);
	}

	void rng_testing_crap() {
		this->rng_value = 0;
		for (size_t i = 0; i < countof(RNG_WORDS.as_array); ++i) {
			RNG_WORDS.as_array[i] = this->rand_ushort();
		}
		this->rng_value = 0;
		for (size_t i = 0; i < countof(RNG_DWORDS_A.as_array); ++i) {
			RNG_DWORDS_A.as_array[i] = this->rand_uint();
		}
		this->rng_value = 1;
		//const vec<uint32_t, 8, true> zero_vec = { 0, 0, 0, 0, 0, 0, 0, 0 };
		//uint64_t start_time = rdtsc();
		for (size_t i = 0; i < countof(RNG_DWORDS_B.as_array); ++i) {
			const uint32_t value = this->rand_uint();
			RNG_DWORDS_B.as_array[i] = value;
#define UseVector
#ifndef UseVector
			for (
				size_t j = 0;
				j < countof(RNG_DWORDS_A.as_array);
				++j
			) {
				if (RNG_DWORDS_A.as_array[j] == value) {
					printf(
						"Value match: A[%zu] == B[%zu] (%u)\n"
						, j, i, value
					);
					break;
				}
			}
#else
			const vec<uint32_t, 8> value_vec = { value, value, value, value, value, value, value, value };
			for (
				size_t j = 0;
				j < countof(RNG_DWORDS_A.as_vec);
				++j
			) {
				if (uint32_t mask = _mm256_movemask_ps(_mm256_cmp_ps(value_vec, RNG_DWORDS_A.as_vec[j], _CMP_NEQ_UQ));
					expect(mask != 0b11111111, false)
				) {
					mask = ~mask;
					uint32_t k = j * 8 + __builtin_ia32_tzcnt_u32(mask);
					printf(
						"Value match: A[%u] == B[%zu] (%u == %u)\n"
						, k, i, RNG_DWORDS_A.as_array[k], value
					);
					break;
				}
			}
#endif
		}
		//uint64_t end_time = rdtsc();
		//printf("Time: %llu\n", end_time - start_time);
	}

	void fill_rng_tables() {
		this->rng_value = 0;
		std::vector<uint16_t> rng_word_vec;
		for (size_t i = 0;; ++i) {
			uint16_t rand_value = this->rand_ushort();
			for (uint16_t value : rng_word_vec) {
				if (value == rand_value) goto BreakA;
			}
			//printf("%hu,", rand_value);
			//RNG_WORD_ARRAY[i] = rand_value;
			//RNG_WORD_SET.insert(rand_value);
			rng_word_vec.push_back(rand_value);
		}
BreakA:
		//puts("");
		this->rng_value = 0;
		std::vector<uint32_t> rng_dword_vec_A;
		for (size_t i = 0;;++i) {
			uint32_t rand_value = this->rand_uint();
			for (uint32_t value : rng_dword_vec_A) {
				if (value == rand_value) goto BreakB;
			}
			//printf("%u,", rand_value);
			//RNG_DWORD_ARRAY_A[i] = rand_value;
			//RNG_DWORD_SET_A.insert(rand_value);
			rng_dword_vec_A.push_back(rand_value);
		}
BreakB:
		//puts("");
		this->rng_value = 1;
		std::vector<uint32_t> rng_dword_vec_B;
		for (size_t i = 0;; ++i) {
			uint32_t rand_value = this->rand_uint();
			for (uint32_t value : rng_dword_vec_B) {
				if (value == rand_value) goto BreakB;
			}
			//printf("%u,", rand_value);
			//RNG_DWORD_ARRAY_B[i] = rand_value;
			//RNG_DWORD_SET_B.insert(rand_value);
			rng_dword_vec_B.push_back(rand_value);
		}
BreakC:
		//puts("");


		/*for (
			size_t i = 0, i_max = rng_dword_vec_A.size();
			i < i_max;
			++i
		) {
			uint32_t value = rng_dword_vec_A[i];
			for (
				size_t j = 0, j_max = rng_dword_vec_B.size();
				j < j_max;
				++j
			) {
				if (rng_dword_vec_B[j] == value) {
					printf(
						"Value match: A[%zu] == B[%zu] (%u)\n"
						, i, j, value
					);
					break;
				}
			}
		}*/
	};

	size_t calc_cycle_size() {
		std::vector<uint32_t> value_vec;
		for (;;) {
			uint32_t rand_value = this->rand_uint();
			for (
				size_t i = 0, i_max = value_vec.size();
				i < i_max;
				++i
			) {
				if (value_vec[i] == rand_value) return i_max;
			}
			value_vec.push_back(rand_value);
		}
		return -1;
	}

	MatchData rand_angle_test() {
		//std::unordered_map<uint32_t, bool> seen_values;
		std::unordered_set<uint32_t> seen_values;
		uint32_t rand_value_A;
		size_t match_count = 0;
		while ((rand_value_A = this->rand_uint()), !seen_values.contains(rand_value_A)) {
#ifndef UseZUNVer
			int32_t rand_value_B = (int32_t)rand_value_A;
			float rand_angle, rand_angle_2;
			{
				double tempA = (double)rand_value_B;
				tempA += U32_FLOAT_CVT[rand_value_B < 0];
				float tempB = (float)tempA;
				rand_angle = rand_angle_2 = tempB;
			}
			rand_angle = rand_angle * F32_DIVI32MAXP1;
			rand_angle = rand_angle - F32_ONE;
			rand_angle = rand_angle * F32_PI;
			rand_angle_2 = rand_angle_2 / F32_I32MAXDIVPI;
			rand_angle_2 = rand_angle_2 - F32_PI;
#else
			float rand_angle, rand_angle_2;
			rand_angle = rand_angle_2 = rand_value_A;
			rand_angle = (rand_angle / INT32_MAX - 1.0f) * F_PI;
			rand_angle_2 = rand_angle_2 / (INT32_MAX / F_PI) - F_PI;
#endif
			//bool match = rand_angle == rand_angle_2;
//extern unsigned char _subborrow_u32 (unsigned char c_in, unsigned int a, unsigned int b, unsigned int * out);
			bool match;
			{
				uint32_t rand_angle_uint = bitcast<uint32_t>(rand_angle);
				uint32_t rand_angle_2_uint = bitcast<uint32_t>(rand_angle_2);
				if (_subborrow_u32(0, rand_angle_uint, rand_angle_2_uint, &rand_angle_uint)) rand_angle_uint = -rand_angle_uint;
				match = rand_angle_uint < 5;
			}

			//bool match = (rand_angle < rand_angle_2 ? rand_angle_2 - rand_angle : rand_angle - rand_angle_2) <= 0.001f;
			match_count += match;
			/*
			if (!match) {
				printf("\n"
					"rand_angle:\t%08X (%f)\n"
					"rand_angle_2:\t%08X (%f)\n"
					, std::bit_cast<uint32_t>(rand_angle), rand_angle
					, std::bit_cast<uint32_t>(rand_angle_2), rand_angle_2
				);
			}
			*/
			//seen_values[rand_value_A] = match;
			seen_values.emplace(rand_value_A);
		}
		return { match_count, seen_values.size() };
	}
};

static Rng RNG = { 0, 0 };

#include <algorithm>
#include <ranges>

void run_float_jank_test() {
	uint32_t rng_value;
	uint32_t mismatched_cycles = 0;

	
	//RNG.fill_rng_tables();
	//RNG.rng_testing_crap();
	RNG.more_rng_cycles();
	
	/*
	printf(
		"RNG Cycles:\n"
		"WORD:    %zu values\n"
		"DWORD_A: %zu values\n"
		"DWORD_B: %zu values\n"
		, RNG_WORD_SET.size()
		, RNG_DWORD_SET_A.size()
		, RNG_DWORD_SET_B.size()
	);
	*/


	return;

#define batch_number 0
#define batch_size 2

	//constexpr uint32_t start_value = (1 << batch_number * batch_size) - 1;
	//constexpr uint32_t end_value = 1 << (batch_number + 1) * batch_size;
	constexpr uint32_t start_value = 0;
	//constexpr uint32_t end_value = UINT16_MAX + 1;
	constexpr uint32_t end_value = 1;
	constexpr uint32_t cycle_count = end_value - start_value;
	printf(
		"Checking %u cycles with seeds %u-%u...\n"
		, cycle_count, start_value, end_value - 1
	);
	for (
		rng_value = start_value;
		rng_value < end_value;
		++rng_value
	) {
		RNG.rng_value = rng_value;
		printf(
			"Cycle %04X has size %zu\n"
			, rng_value, RNG.calc_cycle_size()
		);
		/*
		MatchData match_data = RNG.rand_angle_test();
		size_t mismatch_count = match_data.cycle_size - match_data.match_count;
		bool mismatch_cycle = mismatch_count;
		mismatched_cycles += mismatch_cycle;
		printf(
			!mismatch_cycle
			? "Cycle %04hX    match! %zu / %zu different\n"
			: "Cycle %04hX mismatch! %zu / %zu different\n"
			, (uint16_t)rng_value, mismatch_count, match_data.cycle_size
		);
		*/
	}
	printf(
		"%u match fails\n"
		, cycle_count - mismatched_cycles
	);
}
