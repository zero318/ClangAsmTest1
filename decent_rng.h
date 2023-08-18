#ifndef DECENT_RNG_H
#define DECENT_RNG_H 1
#pragma once

#include <type_traits>
#include <limits>
#include <limits.h>

/*
#include <asmlibran.h>

#if __x86_64__
#pragma comment(lib, "F:\\MyCodeLibs\\asmlib\\libacof64.lib")
#else
#pragma comment(lib, "F:\\MyCodeLibs\\asmlib\\libacof32.lib")
#endif

class DecentRNG {
	CRandomSFMTA0 generator;

	template<typename T, std::enable_if_t<std::is_same_v<T, bool>, bool> = true>
	inline T rand_impl() {
		return this->generator.BRandom() & 1;
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	inline T rand_impl() {
		if constexpr (std::is_same_v<T, long double> && sizeof(long double) > sizeof(double)) {
			return this->generator.RandomL();
		} else {
			return this->generator.Random();
		}
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, bool> = true>
	inline T rand_impl() {
		if constexpr (sizeof(T) <= sizeof(uint32_t)) {
			return this->generator.BRandom();
		} else if constexpr (sizeof(T) <= sizeof(uint64_t)) {
			return ((uint64_t)this->generator.BRandom() << 32) | this->generator.BRandom();
		} else {
			static_assert(sizeof(T) > sizeof(uint64_t), "Integer type too big to randomize!");
		}
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	inline T rand_range_impl(T max) {
		return this->rand_impl<T>() * max;
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, bool> = true>
	inline T rand_range_impl(T max) {
		if constexpr (std::is_signed_v<T>) {
			if constexpr (sizeof(T) <= sizeof(int)) {
				return this->generator.IRandomX(0, max);
			}
			else if (max < std::numeric_limits<int>::max()) {
				return this->generator.IRandomX(0, max);
			} else {

			}
		} //else if ()
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	inline T rand_range_impl(T min, T max) {
		return min + this->rand_impl<T>() * (max - min);
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, bool> = true>
	inline T rand_range_impl(T min, T max) {
		if constexpr (sizeof(T) <= sizeof(int) && std::is_signed_v<T>) {
			return this->generator.IRandom(min, max);
		} else if constexpr (sizeof(T) <= sizeof(int) && std::is_unsigned_v<T>) {
			constexpr auto min_value = std::numeric_limits<T>::min;
			this->generator.IRandom()
		}
		
	}

public:
	inline DecentRNG(int32_t initial_seed) : generator(initial_seed) {}
	inline DecentRNG() : DecentRNG(__rdtsc()) {}

	inline void seed(int32_t seed) {
		return this->generator.RandomInit(seed);
	}

	// Integer types: Any value
	// Float types: Value between 0 and 1
	// Bool: 0 or 1
	template<typename T>
	T rand() {
		return this->rand_impl<T>();
	}

	// Integer/Float types: Value between [min] and [max]
	// Bool: Invalid
	template<typename T>
	T rand_range(T min, T max) {
		return this->rand_range_impl(min, max);
	}

	// Integer/Float types: Value between 0 and [max]
	// Bool: Invalid
	template<typename T>
	T rand_range(T max) {
		return this->rand_range_impl(max);
	}
};
*/

template<size_t bytes>
struct DecentRNGImpl {

	static inline constexpr size_t bit_width = bytes * CHAR_BIT;
	static inline constexpr size_t half_bit_width = bit_width / 2;
	// IDK WTF these are for
	static inline constexpr size_t bit_pos_left = bit_width / 3 + 1;
	static inline constexpr size_t bit_pos_start = bit_pos_left * 2;

	using CounterT = unsigned _BitInt(bit_width);

	using CounterHackT = std::conditional_t<bytes == sizeof(uint8_t), uint8_t,
						 std::conditional_t<bytes < sizeof(uint32_t), uint16_t,
						 std::conditional_t<bytes < sizeof(uint64_t), uint32_t,
						 uintptr_t>>>;

	using BigDivWidthT = unsigned _BitInt(sizeof(uintptr_t) * CHAR_BIT * 2);

	static inline constexpr BigDivWidthT make_div_jank() {
		BigDivWidthT big_value = ~(BigDivWidthT)0;
		auto temp = big_value / (bit_width - 1);
		return temp * (bit_width - 1);
	}

	static inline constexpr BigDivWidthT div_jank = make_div_jank();

	static inline constexpr CounterT high_bit = ((CounterT)1) << bit_width - 1;
	static inline constexpr CounterT bit_mask = ~(~((CounterT)0) << half_bit_width - 1);

	template<bool div_is_too_thicc = (sizeof(CounterT) > sizeof(BigDivWidthT))>
	struct DivWrapper;

	template<>
	struct DivWrapper<false> {
		static inline constexpr uint8_t get_shift_mask(CounterT value) {
			CounterT temp = value + ((value & bit_mask) << 1);
			return (BigDivWidthT)temp % (bit_width - 1u) | 1u;
		}
	};


	template<>
	struct DivWrapper<true> {
		static __declspec(noinline) uint8_t get_shift_mask(CounterT value) {
			CounterT temp = value + ((value & bit_mask) << 1);
			while (temp > div_jank) temp -= div_jank;
			return (BigDivWidthT)temp % (bit_width - 1u) | 1u;
		}
	};

	CounterT current_count = 0;
	CounterT value;
	CounterT prev_value;
	union {
		uint32_t flags;
		struct {
			uint32_t start_flag : 1;
			uint32_t left_flag : 1;
		};
	};

	static inline constexpr bool get_bit(const CounterT& value, size_t index) {
		return value >> index & 1;
	}
	static inline constexpr CounterT shift_left_in(CounterT value, bool bit) {
		return value << 1 | bit;
	}
	static inline constexpr CounterT shift_right_in(CounterT value, bool bit) {
		return value >> 1 | (bit ? high_bit : 0);
	}


	static constexpr CounterT bit_diff(CounterT value, bool left, bool start_bit) {
		CounterT ret;
		if (left) {
			ret = shift_left_in(value, start_bit);
		} else {
			ret = shift_right_in(value, start_bit);
		}
		return value ^ ret;
	}
	CounterT generate_innerA(CounterT value, uint32_t bit) {
		bool left = get_bit(value, (bit_pos_left + bit) % bit_width) ^ this->left_flag ^ true;
		bool start_bit = get_bit(value, (bit_pos_start + bit) % bit_width) ^ this->start_flag ^ true;
		this->left_flag = left;
		this->start_flag = start_bit;
		return this->prev_value = bit_diff(value, left, start_bit) ^ value ^ ~this->prev_value;
	}

	inline CounterT generateA(CounterT value, CounterT count) {
		return (generate_innerA(value, 0) << 1) * (generate_innerA(count, 1) << 1) ^ generate_innerA(value, 2);
	}

	static inline constexpr CounterT generate_innerB(CounterT value) {
		uint8_t shift = DivWrapper<>::get_shift_mask(value);
		value ^= shift_right_in(value, value & 1);
		value ^= value >> half_bit_width;
		value ^= value << half_bit_width;
		__builtin_assume(shift < bit_width);
		return value ^ (value << bit_width - shift | value >> shift);
	}
	static inline constexpr CounterT generateB(CounterT value, CounterT count) {
		CounterT temp = (~value << 1) * (generate_innerB(~value) << 1);
		return generate_innerB(value) ^ temp ^ generate_innerB(temp) ^ generate_innerB(~generate_innerB(count));
	}

	template<typename T> //requires(sizeof(T) <= bytes)
	__declspec(noinline) T __thiscall randA() {
		CounterT new_count;
		bool wrap = __builtin_add_overflow(this->current_count, 1u, &new_count);
		this->current_count = new_count;
		if (__builtin_expect(wrap, false)) {
			*(CounterHackT*)&new_count = 1;
		}
		return this->value = this->generateA(this->value, new_count);
	}
	template<typename T> //requires(sizeof(T) <= bytes)
	__declspec(noinline) T __thiscall randB() {
		CounterT new_count;
		bool wrap = __builtin_add_overflow(this->current_count, 1u, &new_count);
		this->current_count = new_count;
		if (__builtin_expect(wrap, false)) {
			*(CounterHackT*)&new_count = 1;
		}
		return this->value = this->generateB(this->value, new_count);
	}
};

struct DecentRNG {

	

	inline DecentRNG(uint64_t initial_seed) {}
	inline DecentRNG() : DecentRNG(__rdtsc()) {}

	

	inline void __vectorcall seed(uint64_t seed) {
		
	}

	template<typename T>
	T __vectorcall rand() {

	}
};

#endif