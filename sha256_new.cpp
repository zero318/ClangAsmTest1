
#include <stdlib.h>
#include <stdint.h>
#include "zero/util.h"
#include "zero/custom_intrin.h"

#include <limits>
#include <bit>

union SHA256_HASH {
	uint32_t dwords[8];
	uint64_t qwords[4];
	uint8_t bytes[32];
	struct {
		__m128i lower_simd;
		__m128i upper_simd;
	};
};

static constexpr SHA256_HASH initial_state = {
	0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
	0x510E527f, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
};

static constexpr uint32_t k_raw[] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static constexpr vec<uint32_t, 4> k[] = {
	{0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5},{0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5},
	{0xd807aa98,0x12835b01,0x243185be,0x550c7dc3},{0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174},
	{0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc},{0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da},
	{0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7},{0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967},
	{0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13},{0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85},
	{0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3},{0xd192e819,0xd6990624,0xf40e3585,0x106aa070},
	{0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5},{0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3},
	{0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208},{0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2}
};

static inline constexpr auto lwjkbkj = alignof(k);

static inline gnu_attr(assume_aligned(16)) constexpr const vec<uint32_t, 4> *restrict get_k() {
	return k;
}

dllexport gnu_attr(target("no-sse3,no-bmi")) vec<uint32_t, 4> vectorcall add_xmm_A(vec<uint32_t, 4> vec, size_t index) {
	return vec + k[index];
}
dllexport gnu_attr(target("no-sse3,no-bmi")) vec<uint32_t, 4> vectorcall add_xmm_B(vec<uint32_t, 4> vec, size_t index) {
	return vec + get_k()[index];
}

dllexport gnu_attr(target("no-sse3,no-bmi")) inline vec<uint32_t, 4> vectorcall byteswap_xmm(vec<uint32_t, 4> input) {
	//auto temp = *(vec<uint8_t, 16>*)&input;
	auto temp = bitcast<vec<uint8_t, 16>>(input);
	temp = shufflevec(temp, temp, 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);
	return bitcast<vec<uint32_t, 4>>(temp);
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline vec<uint32_t, 4> vectorcall byteswap_xmm2(vec<uint32_t, 4> input) {
	//auto temp = *(vec<uint8_t, 16>*)&input;
	auto temp = bitcast<vec<uint8_t, 16>>(input);
	temp = shufflevec(temp, temp, 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);
	return bitcast<vec<uint32_t, 4>>(temp);
}

#define ror(value, amount) ((value) >> (amount) | (value) << 32 - (amount))
#define ror64(value, amount) ((value) >> (amount) | (value) << 64 - (amount))

template<typename T> requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
static gnu_attr(target("no-sse3,no-bmi")) inline constexpr T ror_func(const T& value, size_t amount) {
	return value >> amount | value << bitsof(T) - amount;
}

/*
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t EP0(uint32_t value) {
	return ror(value, 2) ^ ror(value, 13) ^ ror(value, 22);
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t EP1(uint32_t value) {
	return ror(value, 6) ^ ror(value, 11) ^ ror(value, 25);
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t MAJ(uint32_t valueA, uint32_t valueB, uint32_t valueC) {
	return valueA & valueB ^ valueA & valueC ^ valueB & valueC;
}
*/
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t SIG0(uint32_t value) {
	return ror(value, 7) ^ ror(value, 18) ^ (value >> 3);
}

dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t EP0(uint32_t value) {
	return ror_func(ror_func(ror_func(value, 9) ^ value, 11) ^ value, 2);
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t EP1(uint32_t value) {
	return ror_func(ror_func(ror_func(value, 14) ^ value, 5) ^ value, 6);
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t CH(uint32_t valueA, uint32_t valueB, uint32_t valueC) {
	return valueA & valueB ^ ~valueA & valueC;
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t CH_B(uint32_t valueA, uint32_t valueB, uint32_t valueC) {
	return (valueB ^ valueC) & valueA ^ valueC;
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t MAJ(uint32_t valueA, uint32_t valueB, uint32_t valueC) {
	return (valueA | valueC) & valueB | valueA & valueC;
}


dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr vec<uint32_t, 4> vectorcall SIG0(vec<uint32_t, 4> value) {
	return ror(value, 7) ^ ror(value, 18) ^ (value >> 3);
}

dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr uint32_t SIG1(uint32_t value) {
	return ror(value, 17) ^ ror(value, 19) ^ (value >> 10);
}
dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr vec<uint32_t, 4> vectorcall SIG1_vec(vec<uint32_t, 4> value) {
	return ror(value, 17) ^ ror(value, 19) ^ (value >> 10);
}

dllexport gnu_attr(target("no-sse3,no-bmi")) inline constexpr vec<uint32_t, 4> vectorcall SIG1_half_vec(vec<uint32_t, 4> value) {
	vec<uint64_t, 2> temp = bitcast<vec<uint64_t, 2>>(value);
	return value >> 10 ^ bitcast<vec<uint32_t, 4>>(temp >> 17 ^ temp >> 19);
}

dllexport gnu_attr(target("no-sse3,no-bmi")) inline vec<uint32_t, 4> vectorcall SIG1_low_half(vec<uint32_t, 4> value) {
	vec<uint64_t, 2> temp = bitcast<vec<uint64_t, 2>>(value);
	value = value >> 10 ^ bitcast<vec<uint32_t, 4>>(temp >> 17 ^ temp >> 19);
	temp = bitcast<vec<uint64_t, 2>>(shufflevec(value, value, 0, 2, -1, -1));
	temp[1] = 0;
	return bitcast<vec<uint32_t, 4>>(temp);
}

dllexport gnu_attr(target("no-sse3,no-bmi")) inline vec<uint32_t, 4> vectorcall SIG1_high_half(vec<uint32_t, 4> value) {
	vec<uint64_t, 2> temp = bitcast<vec<uint64_t, 2>>(value);
	value = value >> 10 ^ bitcast<vec<uint32_t, 4>>(temp >> 17 ^ temp >> 19);
	temp = bitcast<vec<uint64_t, 2>>(shufflevec(value, value, -1, -1, 0, 2));
	temp[0] = 0;
	return bitcast<vec<uint32_t, 4>>(temp);
}

gnu_noinline void test_funcs() {
	for (size_t i = 0; i < 10000; ++i) {
		//if (SIG0(i) != SIG0_B(i)) {
			__asm int3
		//}
	}
	__asm int3
}

dllexport gnu_attr(target("no-sse3,no-bmi")) vec<uint32_t, 4> vectorcall movq_test(vec<uint32_t, 4> A, vec<uint32_t, 4> B) {
	vec<uint64_t, 2> temp = bitcast<vec<uint64_t, 2>>(shufflevec(B, B, 0, 2, -1, -1));
	temp[1] = 0;
	return A + bitcast<vec<uint32_t, 4>>(temp);
}

dllexport gnu_attr(target("no-sse3,no-bmi")) vec<uint32_t, 4> vectorcall movq_test2(vec<uint32_t, 4> A, vec<uint32_t, 4> B) {
	vec<uint64_t, 2> temp = bitcast<vec<uint64_t, 2>>(shufflevec(B, B, -1, -1, 0, 2));
	temp[0] = 0;
	return A + bitcast<vec<uint32_t, 4>>(temp);
}

dllexport gnu_attr(target("no-sse3,no-bmi")) inline vec<uint32_t, 4> vectorcall ror_xmm(vec<uint32_t, 4> input, uint8_t shift) {
	auto temp = input;
	input >>= shift;
	auto shift2 = 32 - shift;
	temp[0] <<= shift2;
	temp[1] <<= shift2;
	temp[2] <<= shift2;
	temp[3] <<= shift2;
	return input | temp;
}

dllexport int32_t sha256_out_state[8];
dllexport gnu_attr(target("no-sse3,no-bmi")) void vectorcall sha256_calc_correct(SHA256_HASH *restrict state, uint8_t *restrict data_arr, size_t length) {
	size_t blocks = length / 64;
	int32_t remaining_bytes = length % 64;
	uint32_t a = 0x6A09E667;
	uint32_t b = 0xBB67AE85;
	uint32_t c = 0x3C6EF372;
	uint32_t d = 0xA54FF53A;
	uint32_t e = 0x510E527f;
	uint32_t f = 0x9B05688C;
	uint32_t g = 0x1F83D9AB;
	uint32_t h = 0x5BE0CD19;

	uint32_t m[64];

	//int32_t out_state[8];
	memset((void*)sha256_out_state, 0, sizeof(int32_t[8]));
	while (true) {
		a = sha256_out_state[0] += a;
		b = sha256_out_state[1] += b;
		c = sha256_out_state[2] += c;
		d = sha256_out_state[3] += d;
		e = sha256_out_state[4] += e;
		f = sha256_out_state[5] += f;
		g = sha256_out_state[6] += g;
		h = sha256_out_state[7] += h;
		if (expect((ssize_t)blocks <= 0, false)) {
			if (expect((ssize_t)blocks < 0, true)) break;
			switch (remaining_bytes) {
				default:
					data_arr[remaining_bytes] = 128;
					memcpy(m, data_arr, sizeof(uint32_t[16]));
					if (remaining_bytes >= 56) {
						remaining_bytes = -remaining_bytes;
						break;
					} else if (false) {
				case INT32_MIN ... - 1:
						memset(m, 0, sizeof(uint32_t[14]));
					}
					uint64_t byte_length = (uint64_t)length * 8;
					m[15] = _byteswap_ulong((uint32_t)byte_length);
					m[14] = _byteswap_ulong((uint32_t)(byte_length >> 32));
					--blocks;
			}
		} else {
			memcpy(m, data_arr, sizeof(uint32_t[16]));
			data_arr += sizeof(uint32_t[16]);
			--blocks;
		}
		for (size_t i = 0; i < 16; ++i) {
			m[i] = _byteswap_ulong(m[i]);
		}
		for (size_t i = 16; i < 64; ++i) {
			m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
		}
		for (size_t i = 0; i < 64; i += 8) {
			// Round 0

			d += h += EP1(e) + CH(e, f, g) + k_raw[i] + m[i];
			h += EP0(a) + MAJ(a, b, c);

			// Round 1

			c += g += EP1(d) + CH(d, e, f) + k_raw[i+1] + m[i+1];
			g += EP0(h) + MAJ(h, a, b);

			// Round 2

			b += f += EP1(c) + CH(c, d, e) + k_raw[i+2] + m[i+2];
			f += EP0(g) + MAJ(g, h, a);

			// Round 3

			a += e += EP1(b) + CH(b, c, d) + k_raw[i+3] + m[i+3];
			e += EP0(f) + MAJ(f, g, h);

			// Round 4

			h += d += EP1(a) + CH(a, b, c) + k_raw[i+4] + m[i+4];
			d += EP0(e) + MAJ(e, f, g);

			// Round 5

			g += c += EP1(h) + CH(h, a, b) + k_raw[i+5] + m[i+5];
			c += EP0(d) + MAJ(d, e, f);

			// Round 6

			f += b += EP1(g) + CH(g, h, a) + k_raw[i+6] + m[i+6];
			b += EP0(c) + MAJ(c, d, e);

			// Round 3

			e += a += EP1(f) + CH(f, g, h) + k_raw[i+7] + m[i+7];
			a += EP0(b) + MAJ(b, c, d);
		}
	}
	memcpy(state, (void*)sha256_out_state, sizeof(sha256_out_state));
}

//volatile int32_t out_state[8];
dllexport gnu_attr(target("no-sse3,no-bmi")) void vectorcall sha256_calc_new3(SHA256_HASH *restrict state, uint8_t *restrict data_arr, size_t length) {
	size_t blocks = length / 64;
	int32_t remaining_bytes = length % 64;

	uint32_t a = 0x6A09E667;
	uint32_t b = 0xBB67AE85;
	uint32_t c = 0x3C6EF372;
	uint32_t d = 0xA54FF53A;
	uint32_t e = 0x510E527f;
	uint32_t f = 0x9B05688C;
	uint32_t g = 0x1F83D9AB;
	uint32_t h = 0x5BE0CD19;

	vec<uint32_t, 4> data0, data1, data2, data3;

	sha256_out_state[0] = 0;
	sha256_out_state[1] = 0;
	sha256_out_state[2] = 0;
	sha256_out_state[3] = 0;
	sha256_out_state[4] = 0;
	sha256_out_state[5] = 0;
	sha256_out_state[6] = 0;
	sha256_out_state[7] = 0;

	while (true) {
		a = sha256_out_state[0] += a;
		b = sha256_out_state[1] += b;
		c = sha256_out_state[2] += c;
		d = sha256_out_state[3] += d;
		e = sha256_out_state[4] += e;
		f = sha256_out_state[5] += f;
		g = sha256_out_state[6] += g;
		h = sha256_out_state[7] += h;
		if (expect((ssize_t)blocks <= 0, false)) {
			if (expect((ssize_t)blocks < 0, true)) break;
			switch (remaining_bytes) {
				default:
					data_arr[remaining_bytes] = 128;
					data0 = ((unaligned vec<uint32_t, 4>*)data_arr)[0];
					data1 = ((unaligned vec<uint32_t, 4>*)data_arr)[1];
					data2 = ((unaligned vec<uint32_t, 4>*)data_arr)[2];
					data3 = ((unaligned vec<uint32_t, 4>*)data_arr)[3];
					if (remaining_bytes >= 56) {
						remaining_bytes = -remaining_bytes;
						break;
					}
					else if (false) {
				case INT32_MIN ... -1:
						data0 = { 0,0,0,0 };
						data1 = { 0,0,0,0 };
						data2 = { 0,0,0,0 };
						data3 = { 0,0,0,0 };
					}
					uint64_t byte_length = (uint64_t)length * 8;
					data3[3] = _byteswap_ulong((uint32_t)byte_length);
					data3[2] = _byteswap_ulong((uint32_t)(byte_length >> 32));
					blocks--;
			}
		} else {
			data0 = ((unaligned vec<uint32_t, 4>*)data_arr)[0];
			data1 = ((unaligned vec<uint32_t, 4>*)data_arr)[1];
			data2 = ((unaligned vec<uint32_t, 4>*)data_arr)[2];
			data3 = ((unaligned vec<uint32_t, 4>*)data_arr)[3];
			data_arr += sizeof(vec<uint32_t, 4>[4]);
			--blocks;
		}
		//__asm__ volatile ("" : asm_arg("+x", data0));
		
		data0 = byteswap_xmm2(data0);
		data1 = byteswap_xmm2(data1);
		data2 = byteswap_xmm2(data2);
		data3 = byteswap_xmm2(data3);
		
		vec<uint32_t, 4> FER;
		uint32_t uint_temp;

		for (size_t j = 0; j < 12; j += 4) {

			vec<uint32_t, 4> vec_temp;

			// Four Rounds 0

			FER = data0 + k[j + 0];

			vec_temp = shufflevec(data1, data0, 5, 6, 7, 0);
			data0 += shufflevec(data3, data2, 5, 6, 7, 0);
			
			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);

			data0 += SIG0(vec_temp);
			vec_temp = shufflevec(data3, data3, 2, 2, 3, 3);

			c += g += MAJ(d, e, f) + EP0(d) + FER[1];
			g += EP1(h) + CH(h, a, b);

			data0 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data0, data0, 0, 0, 1, 1);

			b += f += MAJ(c, d, e) + EP0(c) + FER[2];
			f += EP1(g) + CH(g, h, a);

			data0 += SIG1_high_half(vec_temp);

			a += e += MAJ(b, c, d) + EP0(b) + FER[3];
			e += EP1(f) + CH(f, g, h);

			// Four Rounds 1

			FER = data1 + k[j + 1];

			vec_temp = shufflevec(data2, data1, 5, 6, 7, 0);
			data1 += shufflevec(data0, data3, 5, 6, 7, 0);

			h += d += MAJ(a, b, c) + EP0(a) + FER[0];
			d += EP1(e) + CH(e, f, g);

			data1 += SIG0(vec_temp);
			vec_temp = shufflevec(data0, data0, 2, 2, 3, 3);

			g += c += MAJ(h, a, b) + EP0(h) + FER[1];
			c += EP1(d) + CH(d, e, f);

			data1 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data1, data1, 0, 0, 1, 1);

			f += b += MAJ(g, h, a) + EP0(g) + FER[2];
			b += EP1(c) + CH(c, d, e);

			data1 += SIG1_high_half(vec_temp);

			e += a += MAJ(f, g, h) + EP0(f) + FER[3];
			a += EP1(b) + CH(b, c, d);

			// Four Rounds 2

			FER = data2 + k[j + 2];

			vec_temp = shufflevec(data3, data2, 5, 6, 7, 0);
			data2 += shufflevec(data1, data0, 5, 6, 7, 0);

			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);

			data2 += SIG0(vec_temp);
			vec_temp = shufflevec(data1, data1, 2, 2, 3, 3);

			c += g += MAJ(d, e, f) + EP0(d) + FER[1];
			g += EP1(h) + CH(h, a, b);

			data2 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data2, data2, 0, 0, 1, 1);

			b += f += MAJ(c, d, e) + EP0(c) + FER[2];
			f += EP1(g) + CH(g, h, a);

			data2 += SIG1_high_half(vec_temp);

			a += e += MAJ(b, c, d) + EP0(b) + FER[3];
			e += EP1(f) + CH(f, g, h);

			// Four Rounds 3

			FER = data3 + k[j + 3];

			vec_temp = shufflevec(data0, data3, 5, 6, 7, 0);
			data3 += shufflevec(data2, data1, 5, 6, 7, 0);

			h += d += MAJ(a, b, c) + EP0(a) + FER[0];
			d += EP1(e) + CH(e, f, g);

			data3 += SIG0(vec_temp);
			vec_temp = shufflevec(data2, data2, 2, 2, 3, 3);

			g += c += MAJ(h, a, b) + EP0(h) + FER[1];
			c += EP1(d) + CH(d, e, f);

			data3 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data3, data3, 0, 0, 1, 1);

			f += b += MAJ(g, h, a) + EP0(g) + FER[2];
			b += EP1(c) + CH(c, d, e);

			data3 += SIG1_high_half(vec_temp);

			e += a += MAJ(f, g, h) + EP0(f) + FER[3];
			a += EP1(b) + CH(b, c, d);
		}
		for (size_t j = 12; j < 16; j += 2) {
			FER = data0 + k[j + 0];

			// Round 0

			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);

			// Round 1

			c += g += MAJ(d, e, f) + EP0(d) + FER[1];
			g += EP1(h) + CH(h, a, b);

			// Round 2

			b += f += MAJ(c, d, e) + EP0(c) + FER[2];
			f += EP1(g) + CH(g, h, a);

			// Round 3

			a += e += MAJ(b, c, d) + EP0(b) + FER[3];
			e += EP1(f) + CH(f, g, h);

			FER = data1 + k[j + 1];

			// Round 0

			h += d += MAJ(a, b, c) + EP0(a) + FER[0];
			d += EP1(e) + CH(e, f, g);

			// Round 1

			g += c += MAJ(h, a, b) + EP0(h) + FER[1];
			c += EP1(d) + CH(d, e, f);

			// Round 2

			f += b += MAJ(g, h, a) + EP0(g) + FER[2];
			b += EP1(c) + CH(c, d, e);

			// Round 3

			e += a += MAJ(f, g, h) + EP0(f) + FER[3];
			a += EP1(b) + CH(b, c, d);

			data0 = data2;
			data1 = data3;
		}
	}
	memcpy(state, (void*)sha256_out_state, sizeof(sha256_out_state));
}

dllexport gnu_attr(target("no-sse3,no-bmi")) void vectorcall sha256_calc_new2(SHA256_HASH *restrict state, uint8_t *restrict data_arr, size_t length) {
	size_t blocks = length / 64;
	int32_t remaining_bytes = length & 63;

	uint32_t a = 0x6A09E667;
	uint32_t b = 0xBB67AE85;
	uint32_t c = 0x3C6EF372;
	uint32_t d = 0xA54FF53A;
	uint32_t e = 0x510E527f;
	uint32_t f = 0x9B05688C;
	uint32_t g = 0x1F83D9AB;
	uint32_t h = 0x5BE0CD19;

#define ROTATE_SCALARS \
	uint_temp = h; \
	h = g; \
	g = f; \
	f = e; \
	e = d; \
	d = c; \
	c = b; \
	b = a; \
	a = uint_temp;


#define ROTATE_VECS \
	vec_temp = data0; \
	data0 = data1; \
	data1 = data2; \
	data2 = data3; \
	data3 = vec_temp;

	vec<uint32_t, 4> data0, data1, data2, data3;

	int32_t out_state[8];
	memset(&out_state, 0, sizeof(out_state));

	while (true) {
		out_state[0] += a;
		out_state[1] += b;
		out_state[2] += c;
		out_state[3] += d;
		out_state[4] += e;
		out_state[5] += f;
		out_state[6] += g;
		out_state[7] += h;
		if (expect((ssize_t)blocks <= 0, false)) {
			if (expect((ssize_t)blocks < 0, true)) break;
			switch (remaining_bytes) {
				default:
					data_arr[remaining_bytes] = 128;
					data0 = ((unaligned vec<uint32_t, 4>*)data_arr)[0];
					data1 = ((unaligned vec<uint32_t, 4>*)data_arr)[1];
					data2 = ((unaligned vec<uint32_t, 4>*)data_arr)[2];
					data3 = ((unaligned vec<uint32_t, 4>*)data_arr)[3];
					if (remaining_bytes >= 56) {
						remaining_bytes = -remaining_bytes;
						break;
					}
					else if (false) {
				case INT32_MIN ... -1:
						data0 = { 0,0,0,0 };
						data1 = { 0,0,0,0 };
						data2 = { 0,0,0,0 };
						data3 = { 0,0,0,0 };
					}
					uint64_t byte_length = (uint64_t)length * 8;
					data3[3] = _byteswap_ulong((uint32_t)byte_length);
					data3[2] = _byteswap_ulong((uint32_t)(byte_length >> 32));
					blocks--;
			}
		} else {
			data0 = ((unaligned vec<uint32_t, 4>*)data_arr)[0];
			data1 = ((unaligned vec<uint32_t, 4>*)data_arr)[1];
			data2 = ((unaligned vec<uint32_t, 4>*)data_arr)[2];
			data3 = ((unaligned vec<uint32_t, 4>*)data_arr)[3];
			data_arr += sizeof(vec<uint32_t, 4>[4]);
			--blocks;
		}
		//__asm__ volatile ("" : asm_arg("+x", data0));
		
		data0 = byteswap_xmm2(data0);
		data1 = byteswap_xmm2(data1);
		data2 = byteswap_xmm2(data2);
		data3 = byteswap_xmm2(data3);
		
		vec<uint32_t, 4> FER;
		uint32_t uint_temp;

		for (size_t j = 0; j < 12; j += 4) {

			vec<uint32_t, 4> vec_temp;

			// Four Rounds 0

			FER = data0 + k[j + 0];

			vec_temp = shufflevec(data1, data0, 5, 6, 7, 0);
			data0 += shufflevec(data3, data2, 5, 6, 7, 0);
			
			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG0(vec_temp);
			vec_temp = shufflevec(data3, data3, 2, 2, 3, 3);

			d += h += MAJ(e, f, g) + EP0(e) + FER[1];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data0, data0, 0, 0, 1, 1);

			d += h += MAJ(e, f, g) + EP0(e) + FER[2];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_high_half(vec_temp);

			d += h += MAJ(e, f, g) + EP0(e) + FER[3];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			ROTATE_VECS;

			// Four Rounds 1

			FER = data0 + k[j + 1];

			vec_temp = shufflevec(data1, data0, 5, 6, 7, 0);
			data0 += shufflevec(data3, data2, 5, 6, 7, 0);

			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG0(vec_temp);
			vec_temp = shufflevec(data3, data3, 2, 2, 3, 3);

			d += h += MAJ(e, f, g) + EP0(e) + FER[1];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data0, data0, 0, 0, 1, 1);

			d += h += MAJ(e, f, g) + EP0(e) + FER[2];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_high_half(vec_temp);

			d += h += MAJ(e, f, g) + EP0(e) + FER[3];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			ROTATE_VECS;

			// Four Rounds 2

			FER = data0 + k[j + 2];

			vec_temp = shufflevec(data1, data0, 5, 6, 7, 0);
			data0 += shufflevec(data3, data2, 5, 6, 7, 0);

			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG0(vec_temp);
			vec_temp = shufflevec(data3, data3, 2, 2, 3, 3);

			d += h += MAJ(e, f, g) + EP0(e) + FER[1];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data0, data0, 0, 0, 1, 1);

			d += h += MAJ(e, f, g) + EP0(e) + FER[2];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_high_half(vec_temp);

			d += h += MAJ(e, f, g) + EP0(e) + FER[3];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			ROTATE_VECS;

			// Four Rounds 3

			FER = data0 + k[j + 3];

			vec_temp = shufflevec(data1, data0, 5, 6, 7, 0);
			data0 += shufflevec(data3, data2, 5, 6, 7, 0);

			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG0(vec_temp);
			vec_temp = shufflevec(data3, data3, 2, 2, 3, 3);

			d += h += MAJ(e, f, g) + EP0(e) + FER[1];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_low_half(vec_temp);
			vec_temp = shufflevec(data0, data0, 0, 0, 1, 1);

			d += h += MAJ(e, f, g) + EP0(e) + FER[2];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 += SIG1_high_half(vec_temp);

			d += h += MAJ(e, f, g) + EP0(e) + FER[3];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			ROTATE_VECS;
		}
		for (size_t j = 12; j < 16; j += 2) {
			FER = data0 + k[j + 0];

			// Round 0

			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			// Round 1

			d += h += MAJ(e, f, g) + EP0(e) + FER[1];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			// Round 2

			d += h += MAJ(e, f, g) + EP0(e) + FER[2];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			// Round 3

			d += h += MAJ(e, f, g) + EP0(e) + FER[3];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			FER = data1 + k[j + 1];

			// Round 0

			d += h += MAJ(e, f, g) + EP0(e) + FER[0];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			// Round 1

			d += h += MAJ(e, f, g) + EP0(e) + FER[1];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			// Round 2

			d += h += MAJ(e, f, g) + EP0(e) + FER[2];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			// Round 3

			d += h += MAJ(e, f, g) + EP0(e) + FER[3];
			h += EP1(a) + CH(a, b, c);
			ROTATE_SCALARS;

			data0 = data2;
			data1 = data3;
		}
	}
	state->dwords[0] = out_state[0];
	state->dwords[1] = out_state[1];
	state->dwords[2] = out_state[2];
	state->dwords[3] = out_state[3];
	state->dwords[4] = out_state[4];
	state->dwords[5] = out_state[5];
	state->dwords[6] = out_state[6];
	state->dwords[7] = out_state[7];
}


dllexport gnu_attr(target("no-sse3,no-bmi")) void vectorcall sha256_calc_new(SHA256_HASH *restrict state, uint8_t *restrict data_arr, size_t length) {
	size_t blocks = length / 64;
	int32_t remaining_bytes = length % 64;

	//SHA256_HASH state = initial_state;
	//uint32_t a, b, c, d, e, f, g, h;

	//SHA256_HASH state;
	//memset(&state, 0, sizeof(state));

	uint32_t a = 0x6A09E667;
	uint32_t b = 0xBB67AE85;
	uint32_t c = 0x3C6EF372;
	uint32_t d = 0xA54FF53A;
	uint32_t e = 0x510E527f;
	uint32_t f = 0x9B05688C;
	uint32_t g = 0x1F83D9AB;
	uint32_t h = 0x5BE0CD19;

	vec<uint32_t, 4> data0, data1, data2, data3;

	uint32_t out_state[8];
	memset(&out_state, 0, sizeof(out_state));

	while (true) {
		out_state[0] += a;
		out_state[1] += b;
		out_state[2] += c;
		out_state[3] += d;
		out_state[4] += e;
		out_state[5] += f;
		out_state[6] += g;
		out_state[7] += h;
		if (expect((ssize_t)blocks <= 0, false)) {
			if (expect((ssize_t)blocks < 0, true)) break;
			switch (remaining_bytes) {
				default:
					data_arr[remaining_bytes] = 128;
					data0 = ((unaligned vec<uint32_t, 4>*)data_arr)[0];
					data1 = ((unaligned vec<uint32_t, 4>*)data_arr)[1];
					data2 = ((unaligned vec<uint32_t, 4>*)data_arr)[2];
					data3 = ((unaligned vec<uint32_t, 4>*)data_arr)[3];
					if (remaining_bytes >= 56) {
						remaining_bytes = -remaining_bytes;
						break;
					}
					else if (false) {
				case INT32_MIN ... -1:
						data0 = { 0,0,0,0 };
						data1 = { 0,0,0,0 };
						data2 = { 0,0,0,0 };
						data3 = { 0,0,0,0 };
					}
					uint64_t byte_length = (uint64_t)length * 8;
					data3[3] = _byteswap_ulong((uint32_t)byte_length);
					data3[2] = _byteswap_ulong((uint32_t)(byte_length >> 32));
					blocks--;
			}
		} else {
			data0 = ((unaligned vec<uint32_t, 4>*)data_arr)[0];
			data1 = ((unaligned vec<uint32_t, 4>*)data_arr)[1];
			data2 = ((unaligned vec<uint32_t, 4>*)data_arr)[2];
			data3 = ((unaligned vec<uint32_t, 4>*)data_arr)[3];
			data_arr += sizeof(vec<uint32_t, 4>[4]);
			--blocks;
		}
		//__asm__ volatile ("" : asm_arg("+x", data0));
		
		data0 = byteswap_xmm(data0);
		data1 = byteswap_xmm(data1);
		data2 = byteswap_xmm(data2);
		data3 = byteswap_xmm(data3);
		
		vec<uint32_t, 4> FER;
		uint32_t uintA, uintB, uintC, uintD, uint_temp;

#define DO_ROUND(FER_index) \
	uintA = ror(e, 14) ^ e; \
	uintB = ror(a, 9) ^ a; \
	uintA = ror(uintA, 5) ^ e; \
	uintC = (f ^ g) & e; \
	uintB = ror(uintB, 11) ^ a; \
	uintD = (a | c) & b; \
	h += (uintC ^ g) + ror(uintA, 6) + FER[FER_index]; \
	d += h; \
	h += ror(uintB, 2); \
	h += uintD | (a & c); \
	ROTATE_SCALARS; \

		for (size_t j = 0; j < 3; ++j) {

			vec<uint32_t, 4> vecA, vecB, vecC, vecD, vecE, vecF, vecG, vec_temp;

			/*vecA = shufflevec(data3, data2, 5, 6, 7, 0) + data0;
			uintA = ror(e, 14) ^ e;
			uintB = ror(a, 9) ^ a;
			uintA = ror(uintA, 5) ^ e;
			uintC = (f ^ g) & e;
			uintB = ror(uintB, 11) ^ a;
			vecB = shufflevec(data1, data0, 5, 6, 7, 0);
			uintD = (a | c) & b;
			h += uintC ^ g) + ror(uintA, 6) + FER[0];
			d += h;
			h += ror(uintB, 2);
			vecC = ror(vecB, 7);
			h += uintD | (a & c);*/

			//((e>>6)^(e>>11)^(e>>25)); // S1
			//((a >> 2) ^ (a >> 13) ^ (a >> 22)); // S0
			//(((f ^ g) & e) ^ g); // CH
			//(((a | c) & b) | (a & c)); // MAJ

#define FOUR_ROUNDS_AND_SCHED(FER_inc) \
	FER = data0 + k[j*4+FER_inc]; \
	\
	vecA = shufflevec(data3, data2, 5, 6, 7, 0) + data0; \
	vecB = shufflevec(data1, data0, 5, 6, 7, 0); \
	vecC = ror(vecB, 7); \
	\
	DO_ROUND(0); \
	\
	vecC ^= vecB << 14; \
	vecC ^= vecB >> 18; \
	vecC ^= vecB >> 3; \
	vecA += vecC; \
	vecD = shufflevec(data3, data3, 2, 2, 3, 3); \
	\
	DO_ROUND(1); \
	\
	vecE = bitcast<vec<uint64_t, 2>>(vecD); \
	vecD >>= 10; \
	vecD ^= bitcast<vec<uint32_t, 4>>(vecE >> 17 ^ vecE >> 19); \
	vecA[0] += vecD[0]; \
	vecA[1] += vecD[2]; \
	vecF = shufflevec(vecA, vecA, 0, 0, 1, 1); \
	\
	DO_ROUND(2); \
	\
	vecG = bitcast<vec<uint64_t, 2>>(vecF); \
	vecF >>= 10; \
	vecF ^= bitcast<vec<uint32_t, 4>>(vecG >> 17 ^ vecG >> 19); \
	data0 = shufflevec(vecA, vecF, 0, 1, 4, 6); \
	\
	DO_ROUND(3); \
	ROTATE_VECS; \

			FOUR_ROUNDS_AND_SCHED(0);
			FOUR_ROUNDS_AND_SCHED(1);
			FOUR_ROUNDS_AND_SCHED(2);
			FOUR_ROUNDS_AND_SCHED(3);
		}
		for (size_t j = 0; j < 2; ++j) {
			FER = data0 + k[12 + j * 2 + 0];

			DO_ROUND(0);
			DO_ROUND(1);
			DO_ROUND(2);
			DO_ROUND(3);

			FER = data1 + k[12 + j * 2 + 1];

			DO_ROUND(0);
			DO_ROUND(1);
			DO_ROUND(2);
			DO_ROUND(3);

			data0 = data2;
			data1 = data3;
		}
	}
	memcpy(state, out_state, sizeof(out_state));
}

inline void hash(uint8_t* buffer, size_t size) {
	SHA256_HASH hash;
	clang_noinline sha256_calc_correct(&hash, buffer, size);
	char hash_str[65];
	for (size_t i = 0; i < 8; ++i) {
		sprintf(hash_str + (i * 8), "%08lx", _byteswap_ulong(hash.dwords[i]));
	}
	printf("%s\n", hash_str);
}

char yeetus_text[128];

void sha256_test2() {
	memset(yeetus_text, 0, sizeof(yeetus_text));
	memcpy(yeetus_text, "yeetus", sizeof("yeetus"));
	hash((uint8_t*)yeetus_text, 6);
}

void sha256_test(const char* filename) {
	//test_funcs();
	if (FILE* file = fopen(filename, "rb")) {
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		rewind(file);
		size_t full_size = AlignUpToMultipleOf2(size + 1, 64);
		if (uint8_t* buffer = malloc_array<uint8_t>(full_size)) {
			fread(buffer, size, 1, file);
			memset(&buffer[size], 0, full_size - size);
			hash(buffer, size);
		}
		fclose(file);
	}
}