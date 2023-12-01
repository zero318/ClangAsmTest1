#include "util.h"
#include <utility>
#include <algorithm>
#include <cmath>

struct dllexport gnu_attr(trivial_abi) Float2 {
    float x;
    float y;

    inline Float2() {}
    inline constexpr Float2(const float x, const float y) : x(x), y(y) {}
    inline constexpr Float2(const float x) : Float2(x, 0.0f) {}

    inline constexpr Float2 regcall operator+() const {
        return *this;
    }
    inline constexpr Float2 regcall operator-() const {
        return { -this->x, -this->y };
    }
    inline constexpr Float2 regcall operator+(const Float2& rhs) const {
        return { this->x + rhs.x, this->y + rhs.y };
    }
    inline constexpr Float2 regcall operator+(const float rhs) const {
        return { this->x + rhs, this->y + rhs };
    }
    inline constexpr Float2 regcall operator-(const Float2& rhs) const {
        return { this->x - rhs.x, this->y - rhs.y };
    }
    inline constexpr Float2 regcall operator-(const float rhs) const {
        return { this->x - rhs, this->y - rhs };
    }
    inline constexpr Float2 regcall operator*(const Float2& rhs) const {
        return { this->x * rhs.x, this->y * rhs.y };
    }
    inline constexpr Float2 regcall operator*(const float rhs) const {
        return { this->x * rhs, this->y * rhs };
    }
    inline constexpr Float2 regcall operator/(const Float2& rhs) const {
        return { this->x / rhs.x, this->y / rhs.y };
    }
    inline constexpr Float2 regcall operator/(const float rhs) const {
        return { this->x / rhs, this->y / rhs };
    }

    inline constexpr Float2& regcall operator+=(const Float2& rhs) {
        return *this = *this + rhs;
    }
    inline constexpr Float2& regcall operator+=(const float rhs) {
        this->x += rhs; this->y += rhs;
        return *this;
    }
    inline constexpr Float2& regcall operator-=(const Float2& rhs) {
        return *this = *this - rhs;
    }
    inline constexpr Float2& regcall operator-=(const float rhs) {
        this->x -= rhs; this->y -= rhs;
        return *this;
    }
    inline constexpr Float2& regcall operator*=(const Float2& rhs) {
        return *this = *this * rhs;
    }
    inline constexpr Float2& regcall operator*=(const float rhs) {
        this->x *= rhs; this->y *= rhs;
        return *this;
    }
    inline constexpr Float2& regcall operator/=(const Float2& rhs) {
        return *this = *this / rhs;
    }
    inline constexpr Float2& regcall operator/=(const float rhs) {
        this->x /= rhs; this->y /= rhs;
        return *this;
    }

    inline constexpr bool regcall operator==(const Float2& rhs) const {
        return this->x == rhs.x & this->y == rhs.y;
    }
    inline constexpr bool regcall operator!=(const Float2& rhs) const {
        return this->x != rhs.x & this->y != rhs.y;
    }
    inline constexpr bool regcall operator<(const Float2& rhs) const {
        return this->x < rhs.x & this->y < rhs.y;
    }
    inline constexpr bool regcall operator<=(const Float2& rhs) const {
        return this->x <= rhs.x & this->y <= rhs.y;
    }
    inline constexpr bool regcall operator>(const Float2& rhs) const {
        return this->x > rhs.x & this->y > rhs.y;
    }
    inline constexpr bool regcall operator>=(const Float2& rhs) const {
        return this->x >= rhs.x & this->y >= rhs.y;
    }

    inline constexpr Float2 regcall max(const Float2& rhs) const {
        return { std::max(this->x, rhs.x), std::max(this->y, rhs.y) };
    }
    inline constexpr Float2 regcall min(const Float2& rhs) const {
        return { std::min(this->x, rhs.x), std::min(this->y, rhs.y) };
    }

    inline constexpr Float2 regcall abs() const {
        return { std::fabs(this->x), std::fabs(this->y) };
    }
    inline constexpr Float2 regcall ceil() const {
        return { std::ceilf(this->x), std::ceilf(this->y) };
    }
    inline constexpr Float2 regcall floor() const {
        return { std::floorf(this->x), std::floorf(this->y) };
    }
    inline constexpr Float2 regcall trunc() const {
        return { std::truncf(this->x), std::truncf(this->y) };
    }
    inline constexpr Float2 regcall round() const {
        return { std::roundf(this->x), std::roundf(this->y) };
    }
    inline constexpr Float2 regcall sqrt() const {
        return { std::sqrtf(this->x), std::sqrtf(this->y) };
    }
    inline constexpr Float2 vectorcall sin() const {
        return { std::sinf(this->x), std::sinf(this->y) };
    }
    inline constexpr Float2 vectorcall cos() const {
        return { std::cosf(this->x), std::cosf(this->y) };
    }

};

struct alignas(16) dllexport gnu_attr(trivial_abi) Float3 : Float2 {
    float z;
    float w;

    inline Float3() {}
protected:
    inline constexpr Float3(const float x, const float y, const float z, const float w) : Float2(x, y), z(z), w(w) {}
    inline constexpr Float3(const Float2& val, const float z, const float w) : Float2(val), z(z), w(w) {}
public:
    inline constexpr Float3(const float x, const float y, const float z) : Float3(x, y, z, 0.0f) {}
    inline constexpr Float3(const float x, const float y) : Float3(x, y, 0.0f, 0.0f) {}
    inline constexpr Float3(const float x) : Float3(x, 0.0f, 0.0f, 0.0f) {}
    inline constexpr Float3(const Float2& val, const float z) : Float3(val, z, 0.0f) {}
    inline constexpr Float3(const Float2& val) : Float3(val, 0.0f, 0.0f) {}

    inline constexpr Float3 regcall operator+() const {
        return *this;
    }
    inline constexpr Float3 regcall operator-() const {
        return { -this->x, -this->y, -this->z, -this->w };
    }
    inline constexpr Float3 regcall operator+(const Float3& rhs) const {
        return { this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w };
    }
    inline constexpr Float3 regcall operator+(const Float2& rhs) const {
        return { this->x + rhs.x, this->y + rhs.y, this->z, this->w };
    }
    inline constexpr Float3 regcall operator+(const float rhs) const {
        return { this->x + rhs, this->y + rhs, this->z + rhs };
    }
    inline constexpr Float3 regcall operator-(const Float3& rhs) const {
        return { this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w };
    }
    inline constexpr Float3 regcall operator-(const Float2& rhs) const {
        return { this->x - rhs.x, this->y - rhs.y, this->z, this->w };
    }
    inline constexpr Float3 regcall operator-(const float rhs) const {
        return { this->x - rhs, this->y - rhs, this->z - rhs };
    }
    inline constexpr Float3 regcall operator*(const Float3& rhs) const {
        return { this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w };
    }
    inline constexpr Float3 regcall operator*(const Float2& rhs) const {
        return { this->x * rhs.x, this->y * rhs.y, this->z, this->w };
    }
    inline constexpr Float3 regcall operator*(const float rhs) const {
        return { this->x * rhs, this->y * rhs, this->z * rhs };
    }
    inline constexpr Float3 vectorcall operator/(const Float3& rhs_in) const {
        float lhs[4] = { this->x, this->y, this->z, this->w };
        float rhs[4] = { rhs_in.x, rhs_in.y, rhs_in.z, rhs_in.w };
        lhs[0] /= rhs[0]; lhs[1] /= rhs[1]; lhs[2] /= rhs[2]; lhs[3] /= rhs[3];
        return { lhs[0], lhs[1], lhs[2], 0.0f };
    }
    inline constexpr Float3 regcall operator/(const Float2& rhs) const {
        return { this->x / rhs.x, this->y / rhs.y, this->z, this->w };
    }
    inline constexpr Float3 regcall operator/(const float rhs) const {
        return { this->x / rhs, this->y / rhs, this->z / rhs };
    }

    inline constexpr Float3& regcall operator+=(const Float3& rhs) {
        return *this = *this + rhs;
    }
    inline constexpr Float3& regcall operator+=(const Float2& rhs) {
        *static_cast<Float2*>(this) += rhs;
        return *this;
    }
    inline constexpr Float3& regcall operator+=(const float rhs) {
        this->x += rhs; this->y += rhs; this->z += rhs;
        return *this;
    }
    inline constexpr Float3& regcall operator-=(const Float3& rhs) {
        return *this = *this - rhs;
    }
    inline constexpr Float3& regcall operator-=(const Float2& rhs) {
        *static_cast<Float2*>(this) -= rhs;
        return *this;
    }
    inline constexpr Float3& regcall operator-=(const float rhs) {
        this->x -= rhs; this->y -= rhs; this->z -= rhs;
        return *this;
    }
    inline constexpr Float3& regcall operator*=(const Float3& rhs) {
        return *this = *this * rhs;
    }
    inline constexpr Float3& regcall operator*=(const Float2& rhs) {
        *static_cast<Float2*>(this) += rhs;
        return *this;
    }
    inline constexpr Float3& regcall operator*=(const float rhs) {
        this->x *= rhs; this->y *= rhs; this->z *= rhs;
        return *this;
    }
    inline constexpr Float3& regcall operator/=(const Float3& rhs) {
        this->x /= rhs.x; this->y /= rhs.y; this->z /= rhs.z;
        return *this;
    }
    inline constexpr Float3& regcall operator/=(const Float2& rhs) {
        *static_cast<Float2*>(this) /= rhs;
        return *this;
    }
    inline constexpr Float3& regcall operator/=(const float rhs) {
        this->x /= rhs; this->y /= rhs; this->z /= rhs;
        return *this;
    }

    inline constexpr bool regcall operator==(const Float3& rhs) const {
        return this->x == rhs.x & this->y == rhs.y & this->z == rhs.z;
    }
    inline constexpr bool regcall operator!=(const Float3& rhs) const {
        return this->x != rhs.x & this->y != rhs.y & this->z != rhs.z;
    }
    inline constexpr bool regcall operator<(const Float3& rhs) const {
        return this->x < rhs.x & this->y < rhs.y & this->z < rhs.z;
    }
    inline constexpr bool regcall operator<=(const Float3& rhs) const {
        return this->x <= rhs.x & this->y <= rhs.y & this->z <= rhs.z;
    }
    inline constexpr bool regcall operator>(const Float3& rhs) const {
        return this->x > rhs.x & this->y > rhs.y & this->z > rhs.z;
    }
    inline constexpr bool regcall operator>=(const Float3& rhs) const {
        return this->x >= rhs.x & this->y >= rhs.y & this->z >= rhs.z;
    }

    inline constexpr Float3 regcall max(const Float3& rhs) const {
        return { std::max(this->x, rhs.x), std::max(this->y, rhs.y), std::max(this->z, rhs.z), std::max(this->w, rhs.w) };
    }
    inline constexpr Float3 regcall max(const Float2& rhs) const {
        return { std::max(this->x, rhs.x), std::max(this->y, rhs.y), this->z, this->w };
    }
    inline constexpr Float3 regcall min(const Float3& rhs) const {
        return { std::min(this->x, rhs.x), std::min(this->y, rhs.y), std::min(this->z, rhs.z), std::min(this->w, rhs.w) };
    }
    inline constexpr Float3 regcall min(const Float2& rhs) const {
        return { std::min(this->x, rhs.x), std::min(this->y, rhs.y), this->z, this->w };
    }

    inline constexpr Float3 regcall abs() const {
        return { std::fabs(this->x), std::fabs(this->y), std::fabsf(this->z), std::fabsf(this->w) };
    }
    inline constexpr Float3 regcall ceil() const {
        return { std::ceilf(this->x), std::ceilf(this->y), std::ceilf(this->z), std::ceilf(this->w) };
    }
    inline constexpr Float3 regcall floor() const {
        return { std::floorf(this->x), std::floorf(this->y), std::floorf(this->z), std::floorf(this->w) };
    }
    inline constexpr Float3 regcall trunc() const {
        return { std::truncf(this->x), std::truncf(this->y), std::truncf(this->z), std::truncf(this->w) };
    }
    inline constexpr Float3 regcall round() const {
        return { std::roundf(this->x), std::roundf(this->y), std::roundf(this->z), std::roundf(this->w) };
    }
    inline constexpr Float3 regcall sqrt() const {
        return { std::sqrtf(this->x), std::sqrtf(this->y), std::sqrtf(this->z), std::sqrtf(this->w) };
    }
    inline constexpr Float3 vectorcall sin() const {
        return { std::sinf(this->x), std::sinf(this->y), std::sinf(this->z), std::sinf(this->w) };
    }
    inline constexpr Float3 vectorcall cos() const {
        return { std::cosf(this->x), std::cosf(this->y), std::cosf(this->z), std::cosf(this->w) };
    }
};

struct alignas(16) dllexport gnu_attr(trivial_abi) Float4 : Float3 {

    inline Float4() {}
    inline constexpr Float4(const float x, const float y, const float z, const float w) : Float3(x, y, z, w) {}
    inline constexpr Float4(const float x, const float y, const float z) : Float3(x, y, z, 0.0f) {}
    inline constexpr Float4(const float x, const float y) : Float3(x, y, 0.0f, 0.0f) {}
    inline constexpr Float4(const float x) : Float3(x, 0.0f, 0.0f, 0.0f) {}
    inline constexpr Float4(const Float3& val, const float w) : Float3(val.x, val.y, val.z, w) {}
    inline constexpr Float4(const Float3& val) : Float3(val) {}
    inline constexpr Float4(const Float2& val, const float z, const float w) : Float3(val, z, w) {}
    inline constexpr Float4(const Float2& val, const float z) : Float3(val, z, 0.0f) {}
    inline constexpr Float4(const Float2& val) : Float3(val, 0.0f, 0.0f) {}

    inline constexpr Float4 regcall operator+() const {
        return *this;
    }
    inline constexpr Float4 regcall operator-() const {
        return { -this->x, -this->y, -this->z, -this->w };
    }
    inline constexpr Float4 regcall operator+(const Float4& rhs) const {
        return { this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w };
    }
    inline constexpr Float4 regcall operator+(const Float3& rhs) const {
        return { this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w };
    }
    inline constexpr Float4 regcall operator+(const Float2& rhs) const {
        return { this->x + rhs.x, this->y + rhs.y, this->z, this->w };
    }
    inline constexpr Float4 regcall operator+(const float rhs) const {
        return { this->x + rhs, this->y + rhs, this->z + rhs, this->w + rhs };
    }
    inline constexpr Float4 regcall operator-(const Float4& rhs) const {
        return { this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w };
    }
    inline constexpr Float4 regcall operator-(const Float3& rhs) const {
        return { this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w };
    }
    inline constexpr Float4 regcall operator-(const Float2& rhs) const {
        return { this->x - rhs.x, this->y - rhs.y, this->z, this->w };
    }
    inline constexpr Float4 regcall operator-(const float rhs) const {
        return { this->x - rhs, this->y - rhs, this->z - rhs, this->w - rhs };
    }
    inline constexpr Float4 regcall operator*(const Float4& rhs) const {
        return { this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w };
    }
    inline constexpr Float4 regcall operator*(const Float3& rhs) const {
        return { this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w };
    }
    inline constexpr Float4 regcall operator*(const Float2& rhs) const {
        return { this->x * rhs.x, this->y * rhs.y, this->z, this->w };
    }
    inline constexpr Float4 regcall operator*(const float rhs) const {
        return { this->x * rhs, this->y * rhs, this->z * rhs, this->w * rhs };
    }
    inline constexpr Float4 regcall operator/(const Float4& rhs) const {
        return { this->x / rhs.x, this->y / rhs.y, this->z / rhs.z, this->w / rhs.w };
    }
    inline constexpr Float4 vectorcall operator/(const Float3& rhs_in) const {
        float lhs[4] = { this->x, this->y, this->z, this->w };
        float rhs[4] = { rhs_in.x, rhs_in.y, rhs_in.z, rhs_in.w };
        lhs[0] /= rhs[0]; lhs[1] /= rhs[1]; lhs[2] /= rhs[2]; lhs[3] /= rhs[3];
        return { lhs[0], lhs[1], lhs[2], this->w };
    }
    inline constexpr Float4 regcall operator/(const Float2& rhs) const {
        return { this->x / rhs.x, this->y / rhs.y, this->z, this->w };
    }
    inline constexpr Float4 regcall operator/(const float rhs) const {
        return { this->x / rhs, this->y / rhs, this->z / rhs, this->w / rhs };
    }

    inline constexpr Float4& regcall operator+=(const Float4& rhs) {
        return *this = *this + rhs;
    }
    inline constexpr Float4& regcall operator+=(const Float3& rhs) {
        *static_cast<Float3*>(this) += rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator+=(const Float2& rhs) {
        *static_cast<Float2*>(this) += rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator+=(const float rhs) {
        this->x += rhs; this->y += rhs; this->z += rhs; this->w += rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator-=(const Float4& rhs) {
        return *this = *this - rhs;
    }
    inline constexpr Float4& regcall operator-=(const Float3& rhs) {
        *static_cast<Float3*>(this) -= rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator-=(const Float2& rhs) {
        *static_cast<Float2*>(this) -= rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator-=(const float rhs) {
        this->x -= rhs; this->y -= rhs; this->z -= rhs; this->w -= rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator*=(const Float4& rhs) {
        return *this = *this * rhs;
    }
    inline constexpr Float4& regcall operator*=(const Float3& rhs) {
        *static_cast<Float3*>(this) *= rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator*=(const Float2& rhs) {
        *static_cast<Float2*>(this) *= rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator*=(const float rhs) {
        this->x *= rhs; this->y *= rhs; this->z *= rhs; this->w *= rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator/=(const Float4& rhs) {
        return *this = *this / rhs;
    }
    inline constexpr Float4& regcall operator/=(const Float3& rhs) {
        *static_cast<Float3*>(this) /= rhs;
        return *this;
    }
    inline constexpr Float2& regcall operator/=(const Float2& rhs) {
        *static_cast<Float2*>(this) /= rhs;
        return *this;
    }
    inline constexpr Float4& regcall operator/=(const float rhs) {
        this->x /= rhs; this->y /= rhs; this->z /= rhs; this->w /= rhs;
        return *this;
    }

    inline constexpr bool regcall operator==(const Float4& rhs) const {
        return this->x == rhs.x & this->y == rhs.y & this->z == rhs.z & this->w == rhs.w;
    }
    inline constexpr bool regcall operator!=(const Float4& rhs) const {
        return this->x != rhs.x & this->y != rhs.y & this->z != rhs.z & this->w != rhs.w;
    }
    inline constexpr bool regcall operator<(const Float4& rhs) const {
        return this->x < rhs.x & this->y < rhs.y & this->z < rhs.z & this->w < rhs.w;
    }
    inline constexpr bool regcall operator<=(const Float4& rhs) const {
        return this->x <= rhs.x & this->y <= rhs.y & this->z <= rhs.z & this->w <= rhs.w;
    }
    inline constexpr bool regcall operator>(const Float4& rhs) const {
        return this->x > rhs.x & this->y > rhs.y & this->z > rhs.z & this->w > rhs.w;
    }
    inline constexpr bool regcall operator>=(const Float4& rhs) const {
        return this->x >= rhs.x & this->y >= rhs.y & this->z >= rhs.z & this->w >= rhs.w;
    }

    inline constexpr Float4 regcall max(const Float4& rhs) const {
        return { std::max(this->x, rhs.x), std::max(this->y, rhs.y), std::max(this->z, rhs.z), std::max(this->w, rhs.w) };
    }
    inline constexpr Float4 regcall max(const Float3& rhs) const {
        return { std::max(this->x, rhs.x), std::max(this->y, rhs.y), std::max(this->z, rhs.z), this->w };
    }
    inline constexpr Float4 regcall max(const Float2& rhs) const {
        return { std::max(this->x, rhs.x), std::max(this->y, rhs.y), this->z, this->w };
    }
    inline constexpr Float4 regcall min(const Float4& rhs) const {
        return { std::min(this->x, rhs.x), std::min(this->y, rhs.y), std::min(this->z, rhs.z), std::min(this->w, rhs.w) };
    }
    inline constexpr Float4 regcall min(const Float3& rhs) const {
        return { std::min(this->x, rhs.x), std::min(this->y, rhs.y), std::min(this->z, rhs.z), this->w };
    }
    inline constexpr Float4 regcall min(const Float2& rhs) const {
        return { std::min(this->x, rhs.x), std::min(this->y, rhs.y), this->z, this->w };
    }

    inline constexpr Float4 regcall abs() const {
        return { std::fabs(this->x), std::fabs(this->y), std::fabsf(this->z), std::fabsf(this->w) };
    }
    inline constexpr Float4 regcall ceil() const {
        return { std::ceilf(this->x), std::ceilf(this->y), std::ceilf(this->z), std::ceilf(this->w) };
    }
    inline constexpr Float4 regcall floor() const {
        return { std::floorf(this->x), std::floorf(this->y), std::floorf(this->z), std::floorf(this->w) };
    }
    inline constexpr Float4 regcall trunc() const {
        return { std::truncf(this->x), std::truncf(this->y), std::truncf(this->z), std::truncf(this->w) };
    }
    inline constexpr Float4 regcall round() const {
        return { std::roundf(this->x), std::roundf(this->y), std::roundf(this->z), std::roundf(this->w) };
    }
    inline constexpr Float4 regcall sqrt() const {
        return { std::sqrtf(this->x), std::sqrtf(this->y), std::sqrtf(this->z), std::sqrtf(this->w) };
    }
    inline constexpr Float4 vectorcall sin() const {
        return { std::sinf(this->x), std::sinf(this->y), std::sinf(this->z), std::sinf(this->w) };
    }
    inline constexpr Float4 vectorcall cos() const {
        return { std::cosf(this->x), std::cosf(this->y), std::cosf(this->z), std::cosf(this->w) };
    }
};