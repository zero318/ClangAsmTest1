#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "custom_intrin.h"
#include <bit>
#include <concepts>
#include <string>
#include <string_view>
#include <utility>

//static constexpr inline size_t BUCKET_SIZE = 8;
//static_assert(std::has_single_bit(BUCKET_SIZE));
//static constexpr inline size_t BUCKET_SIZE_POWER_OF_2 = std::bit_width(BUCKET_SIZE - 1);

template <typename T>
using hash_type_t = std::invoke_result_t<std::hash<T>, T>;

template <typename T> requires std::regular_invocable<std::hash<T>,T>
inline auto calculate_hash(const T& key) noexcept {
	return std::hash<T>{}(key);
}

template <std::unsigned_integral T>
static inline constexpr T trash_modulo(T dividend, T divisor) {
	//if constexpr (std::is_constant_evaluated()) {
		//return dividend % divisor;
	//}
	//else {
		using U = DoubleSizeUintT<T>;
		return T(U(dividend) * U(divisor) >> bitsof(T));
	//}
}

template <typename KT, typename VT>
struct CuckooHashMap {

	using hash_t = hash_type_t<KT>;
	static inline constexpr size_t hash_vec_width = CURRENT_VEC_TRAITS::prefer_integral_width_v<hash_t> ?: 1;
	using hash_vec_t = std::conditional_t<hash_vec_width != 1, vec<hash_t, hash_vec_width>, hash_t[hash_vec_width]>;

	static inline constexpr size_t bucket_width = hash_vec_width;
	static_assert(std::has_single_bit(bucket_width));
	static constexpr inline size_t bucket_size_power_of_2 = std::bit_width(bucket_width - 1);

	using KTArg = std::conditional_t<std::is_scalar_v<KT> && sizeof(KT) <= sizeof(size_t), KT, const KT&>;
	using VTArg = std::conditional_t<std::is_scalar_v<VT> && sizeof(VT) <= sizeof(size_t), VT, const VT&>;

	struct cache_align Bucket {
		hash_t keys[bucket_width];
		VT values[bucket_width];

		inline void regcall reset_keys() {
			for (hash_t& key : keys) key = 0;
		}

		inline size_t count_invalid_keys() {
			size_t count = 0;
			hash_vec_t *restrict key_vec = (hash_vec_t *restrict)this->keys;
			for (size_t i = 0; i < hash_vec_width; ++i) count += (*key_vec)[i] != 0;
			return count;
		}

		inline size_t regcall calculate_key_mask(hash_t key_hash) const noexcept {
			hash_vec_t lookup_vec;
			for (size_t i = 0; i < hash_vec_width; ++i) lookup_vec[i] = key_hash;
			hash_vec_t lookup_result;
			hash_vec_t *restrict key_vec = (hash_vec_t *restrict)this->keys;
			for (size_t i = 0; i < hash_vec_width; ++i) lookup_result[i] = lookup_vec[i] - (*key_vec)[i];
			size_t mask = 0;
			for (size_t i = 0; i < hash_vec_width; ++i) mask |= !(bool)lookup_result[i] << i;
			return mask;
		}

		template<bool include_assumption>
		inline VT *restrict thiscall search_keys(hash_t key_hash) const noexcept {
			size_t mask = this->calculate_key_mask(key_hash);
			size_t mask_index = std::countr_zero(mask);
			if (mask_index != bitsof(mask)) {
				VT *restrict ret = (VT *restrict)&this->values[mask_index];
				if constexpr (include_assumption) {
					assume(ret != NULL);
					return ret;
				} else {
					return ret;
				}
			}
			return NULL;
		}
		dllexport inline VT *restrict thiscall search_keysA(hash_t key_hash) const noexcept asm("cuckoo_bucket_search_keysA") {
			return this->search_keys<true>(key_hash);
		}
		dllexport inline VT *restrict thiscall search_keysB(hash_t key_hash) const noexcept asm("cuckoo_bucket_search_keysB") {
			return this->search_keys<false>(key_hash);
		}

		dllexport inline bool vectorcall update_key(hash_t key_hash, VTArg value) noexcept asm("cuckoo_bucket_update_key") {
			size_t mask = this->calculate_key_mask(key_hash);
			size_t mask_index = std::countr_zero(mask);
			if (mask_index != bitsof(mask)) {
				this->values[mask_index] = value;
				return true;
			}
			return false;
		}
	};

	Bucket *restrict buckets; // 0x0
	size_t bucket_count; // 0x4
	size_t used_entries; // 0x8

	inline bool regcall is_valid_map() const noexcept {
		return this->buckets;
	}

	CuckooHashMap(size_t capacity) {
		size_t bucket_count = AlignUpToMultipleOf2(capacity, bucket_width);
		Bucket *restrict buckets = this->buckets = new Bucket[bucket_count];
		if (expect(buckets != NULL, true)) {
			this->bucket_count = bucket_count;
			this->used_entries = 0;
			for (size_t i = 0; i < bucket_count; ++i) {
				buckets[i].reset_keys();
			}
		}
	}
	~CuckooHashMap() {
		if (auto* ptr = this->buckets) delete[] ptr;
	}

	static inline hash_t regcall murmur3_final_mix(hash_t hash) noexcept {
		if constexpr (sizeof(hash_t) == sizeof(uint32_t)) {
			hash ^= hash >> 16;
			hash *= 0x85EBCA6Bu;
			hash ^= hash >> 13;
			hash *= 0xC2B2AE35u;
			hash ^= hash >> 16;
		} else  {
			hash ^= hash >> 33;
			hash *= 0xFF51AFD7ED558CCDull;
			hash ^= hash >> 33;
			hash *= 0xC4CEB9FE1A85EC53ull;
			hash ^= hash >> 33;
		}
		return hash;
	}

	static inline hash_t regcall make_hash_mix_B(hash_t hash, hash_t hash_mix_A, hash_t hash_mod_A, size_t bucket_count) noexcept {
		hash_t hash_mod_B = trash_modulo(murmur3_final_mix(hash ^ hash_mix_A), bucket_count);
		if (hash_mod_B == hash_mod_A) {
			hash_mod_B = (hash_mod_A == bucket_count - 1) ? 0 : hash_mod_A + 1;
		}
		return hash_mod_B;
	}

	dllexport inline void regcall insert(hash_t hash, VTArg value, bool is_not_reinsert) const noexcept asm("cuckoo_insert") {
		size_t bucket_count = this->bucket_count;
		hash_t hash_mix_A = murmur3_final_mix(hash);
		hash_t hash_mod_A = trash_modulo(hash_mix_A, bucket_count);
		Bucket *restrict bucket_A = &this->buckets[hash_mod_A];

		if (bucket_A->update_key(hash, value)) return;

		hash_t hash_mod_B = make_hash_mix_B(hash, hash_mix_A, hash_mod_A, bucket_count);
		Bucket *restrict bucket_B = &this->buckets[hash_mod_B];
		if (bucket_B->update_key(hash, value)) return;

		this->used_entries += is_not_reinsert;
		size_t key_offset_A = bucket_A->count_invalid_keys();
		size_t key_offset_B = bucket_B->count_invalid_keys();

		if (key_offset_A != bucket_width || key_offset_B != bucket_width) {
			Bucket *restrict fill_bucket = key_offset_A <= key_offset_B ? bucket_A : bucket_B;
			size_t fill_offset = key_offset_A <= key_offset_B ? key_offset_A : key_offset_B;
			fill_bucket->keys[fill_offset] = hash;
			fill_bucket->values[fill_offset] = value;
		}
		else {

		}
	}



	dllexport inline auto regcall get(hash_t hash) const noexcept asm("cuckoo_get") {
		size_t bucket_count = this->bucket_count;
		hash_t hash_mix_A = murmur3_final_mix(hash);
		hash_t hash_mod_A = trash_modulo(hash_mix_A, bucket_count);
		Bucket *restrict bucket_A = &this->buckets[hash_mod_A];
		VT* value = bucket_A->search_keysA(hash);
		if (value) return value;
		//hash_t hash_mix_B = murmur3_final_mix(hash ^ hash_mix_A);
		//hash_t hash_mod_B = trash_modulo(hash_mix_B, bucket_count);
		//if (hash_mod_B == hash_mod_A) {
			//hash_mod_B = (hash_mod_A == bucket_count - 1) ? 0 : hash_mod_A + 1;
		//}
		hash_t hash_mod_B = make_hash_mix_B(hash, hash_mix_A, hash_mod_A, bucket_count);
		Bucket *restrict bucket_B = &this->buckets[hash_mod_B];
		return bucket_B->search_keysA(hash);
	}

	dllexport gnu_noinline auto regcall get_outer(KTArg key) const noexcept asm("cuckoo_get_outer") {
		clang_noinline auto ret = this->get(calculate_hash(key));
		return ret;
	}
	dllexport gnu_noinline void regcall insert_outer(KTArg key, VTArg value) const noexcept asm("cuckoo_insert_outer") {
		clang_noinline this->insert(calculate_hash(key), value, true);
	}
};

dllexport CuckooHashMap<std::string_view, void*> test = CuckooHashMap<std::string_view, void*>(0);

//static inline constexpr bool vec_width = validate_impl<test.hash_vec_width, 4>();
//static_assert(vec_width, "Incorrect vector size! Size of vector is not 4");

dllexport void* yeqjrwkrvb() {
	return test.get_outer("yeetus");
}

dllexport uint128_t cdecl uint128_cdecl_test(uint128_t value) {
	return value + 1;
}
dllexport uint128_t fastcall uint128_fastcall_test(uint128_t value) {
	return value + 1;
}
dllexport uint128_t vectorcall uint128_vectorcall_test(uint128_t value) {
	return value + 1;
}
dllexport uint128_t regcall uint128_regcall_test(uint128_t value) {
	return value + 1;
}