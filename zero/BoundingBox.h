#pragma once

//#include <intrin.h>

// Based off of this:
// https://gamedev.stackexchange.com/a/167846

#ifndef DECLARED_FLOAT_STUCTS
#define DECLARED_FLOAT_STUCTS 1

#include "zun_types.h"

#endif


static vec<float, 4> bounding_sign_mask = { 0.0f, 0.0f, -0.0f, -0.0f };
static vec<float, 4> bounding_half_maskA = { 0.5f, 0.5f, -0.5f, -0.5f };
static vec<float, 4> bounding_half_maskB = { -0.5f, -0.5f, 0.5f, 0.5f };

struct BoundingBoxDest {
	vec<float, 4> box;

	dllexport inline BoundingBoxDest& make_from_pos_size(const Float3& position, const Float3& size) {
		vec<float, 4> position_box = {
			position.x, position.y,
			position.x, position.y
		};
		vec<float, 4> size_box = {
			size.x, size.y,
			size.x, size.y
		};
		size_box *= bounding_half_maskA;
		position_box += size_box;
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxDest& make_from_pos_size(const Float2& position, const Float2& size) {
		vec<float, 4> position_box = {
			position.x, position.y,
			position.x, position.y
		};
		vec<float, 4> size_box = {
			size.x, size.y,
			size.x, size.y
		};
		size_box *= bounding_half_maskA;
		position_box += size_box;
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxDest& make_from_min_max(const Float3& max, const Float3& min) {
		vec<float, 4> position_box = bitcast<vec<float, 4>>((vec<double, 2>) {
			*(double*)&max,
			*(double*)&min
		});
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxDest& make_from_min_max(const Float2& max, const Float2& min) {
		vec<float, 4> position_box = bitcast<vec<float, 4>>((vec<double, 2>) {
			*(double*)&max,
			*(double*)&min
		});
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxDest& make_from_max(const Float2& max) {
		vec<float, 4> position_box = bitcast<vec<float, 4>>((vec<double, 2>) {
			*(double*)&max,
			0.0
		});
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxDest& make_from_raw_pos_size(float X, float Y, float W, float H) {
		vec<float, 4> position_box = {
			X, Y,
			X, Y
		};
		vec<float, 4> size_box = {
			W, H,
			W, H
		};
		size_box *= bounding_half_maskA;
		position_box += size_box;
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxDest& make_from_raw_min_max(float max_X, float max_Y, float min_X, float min_Y) {
		vec<float, 4> position_box = {
			max_X, max_Y,
			min_X, min_Y
		};
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}
};

struct BoundingBoxSource {
	vec<float, 4> box;

	dllexport inline BoundingBoxSource& make_from_pos_size(const Float3& position, const Float3& size) {
		vec<float, 4> position_box = {
			position.x, position.y,
			position.x, position.y
		};
		vec<float, 4> size_box = {
			size.x, size.y,
			size.x, size.y
		};
		size_box *= bounding_half_maskB;
		position_box += size_box;
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxSource& make_from_pos_size(const Float2& position, const Float2& size) {
		vec<float, 4> position_box = {
			position.x, position.y,
			position.x, position.y
		};
		vec<float, 4> size_box = {
			size.x, size.y,
			size.x, size.y
		};
		size_box *= bounding_half_maskB;
		position_box += size_box;
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxSource& make_from_min_max(const Float3& max, const Float3& min) {
		vec<float, 4> position_box = bitcast<vec<float, 4>>((vec<double, 2>) {
			*(double*)&min,
			*(double*)&max
		});
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxSource& make_from_min_max(const Float2& max, const Float2& min) {
		vec<float, 4> position_box = bitcast<vec<float, 4>>((vec<double, 2>) {
			*(double*)&min,
			*(double*)&max
		});
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxSource& make_from_raw_pos_size(float X, float Y, float W, float H) {
		vec<float, 4> position_box = {
			X, Y,
			X, Y
		};
		vec<float, 4> size_box = {
			W, H,
			W, H
		};
		size_box *= bounding_half_maskB;
		position_box += size_box;
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline BoundingBoxSource& make_from_raw_min_max(float max_X, float max_Y, float min_X, float min_Y) {
		vec<float, 4> position_box = {
			min_X, min_Y,
			max_X, max_Y
		};
		position_box = _mm_xor_ps(position_box, bounding_sign_mask);
		this->box = position_box;
		return *this;
	}

	dllexport inline bool test_intersect(const BoundingBoxDest& box2) {
		auto val = this->box > box2.box;
#ifdef __AVX__
		return _mm_testz_ps(val, val);
#else
		return _mm_testz_si128((__m128i)val, (__m128i)val);
#endif
		//return !_mm_movemask_ps(this->box > box2.box);
	}

	dllexport inline bool test_intersect_exclusive(const BoundingBoxDest& box2) {
		auto val = this->box >= box2.box;
#ifdef __AVX__
		return _mm_testz_ps(val, val);
#else
		return _mm_testz_si128((__m128i)val, (__m128i)val);
#endif
		//return !_mm_movemask_ps(this->box > box2.box);
	}

	dllexport inline bool test_not_intersect(const BoundingBoxDest& box2) {
		auto val = this->box > box2.box;
#ifdef __AVX__
		return !_mm_testz_ps(val, val);
#else
		return !_mm_testz_si128((__m128i)val, (__m128i)val);
#endif
		//return _mm_testz_si128((__m128i)val, (__m128i)val);
		//return _mm_movemask_ps(this->box > box2.box);
		//return !_mm_movemask_ps(this->box < box2.box);
		//return _mm_movemask_ps(this->box < box2.box) != 0;
	}

	dllexport inline bool test_not_intersect_exclusive(const BoundingBoxDest& box2) {
		auto val = this->box >= box2.box;
#ifdef __AVX__
		return !_mm_testz_ps(val, val);
#else
		return !_mm_testz_si128((__m128i)val, (__m128i)val);
#endif
		//return _mm_testz_si128((__m128i)val, (__m128i)val);
		//return _mm_movemask_ps(this->box > box2.box);
		//return !_mm_movemask_ps(this->box < box2.box);
		//return _mm_movemask_ps(this->box < box2.box) != 0;
	}
};