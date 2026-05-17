#pragma once

#ifndef SHIFT_JIS_H
#define SHIFT_JIS_H 1

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <array>
#include <string_view>

#include "utf8_to_sjis.h"

#if __cpp_char8_t < 201811L
#define char8_t unsigned char
#endif

template<typename C, size_t N>
struct StringLiteralBase {
    alignas(alignof(void*)) const std::array<C, N> buf;

    inline constexpr StringLiteralBase(const C(&str)[N]) : buf(std::to_array(str)) {}

    inline constexpr StringLiteralBase(const std::array<C, N>& str) : buf(str) {}

    inline constexpr const C* data() const {
        return &this->buf[0];
    }

    inline constexpr size_t size() const {
        return N;
    }

    inline constexpr size_t length() const {
        return N - 1;
    }

    inline constexpr const C& operator[](size_t index) const {
        return this->buf[index];
    }
};

template<size_t N>
struct StringLiteral : StringLiteralBase<char, N> {
    using StringLiteralBase<char, N>::StringLiteralBase;
};
template<size_t N>
struct U8StringLiteral : StringLiteralBase<char8_t, N> {
    using StringLiteralBase<char8_t, N>::StringLiteralBase;
};

template<size_t N>
StringLiteral(const char(&)[N])->StringLiteral<N>;
template<size_t N>
U8StringLiteral(const char8_t(&)[N])->U8StringLiteral<N>;

template<size_t N>
StringLiteral(const std::array<char, N>&)->StringLiteral<N>;
template<size_t N>
U8StringLiteral(const std::array<char8_t, N>&)->U8StringLiteral<N>;

template<auto str>
struct StringPool {
    static inline constexpr auto data() {
        return str.data();
    }
    static inline constexpr size_t size() {
        return str.size();
    }
    static inline constexpr size_t length() {
        return str.length();
    }
    static inline constexpr auto operator[](size_t i) {
        return str[i];
    }
};

template<typename C>
static inline constexpr size_t utf8_to_sjis_buffer_length(const C* str, size_t N) {
    size_t length = 0;

    do {
        ++length; // At least one byte no matter what
        switch (uint8_t c = (uint8_t)*str++) {
            case 0b00000000 ... 0b01111111:
                // Single byte character in both UTF8 / SJIS
                continue;
            case 0b11000000 ... 0b11011111:
                str += 1;
                N -= 1;
                break;
            case 0b11100000 ... 0b11101111: {
                uint32_t codepoint = (c & 0b1111) << 12;
                codepoint |= ((uint8_t)*str++ & 0x3F) << 6;
                codepoint |= ((uint8_t)*str++ & 0x3F);
                N -= 2;
                switch (codepoint) {
                    default: // Two byte character
                        ++length;
                    case U'｡' ... U'ﾟ': // Single byte character in SJIS
                        break;
                }
                break;
            }
            case 0b11110000 ... 0b11110111:
                str += 3;
                N -= 3;
                break;
        }
    } while (--N);

    return length;
}

template<typename C>
static inline constexpr void utf8_to_sjis(char* out, const C* str, size_t N) {
    do {
        uint32_t codepoint = 0;
        switch (uint8_t c = (uint8_t)*str++) {
            case 0b00000000 ... 0b01111111:
                *out++ = (char)c; // Single byte character in both UTF8 / SJIS
                continue;
            case 0b11000000 ... 0b11011111:
                N -= 1;
                codepoint = (c & 0b11111) << 6;
                codepoint |= (uint8_t)*str++ & 0x3F;
                break;
            case 0b11100000 ... 0b11101111:
                N -= 2;
                codepoint = (c & 0b1111) << 12;
                codepoint |= ((uint8_t)*str++ & 0x3F) << 6;
                codepoint |= ((uint8_t)*str++ & 0x3F);
                switch (codepoint) {
                    case U'｡' ... U'ﾟ': // Single byte character in SJIS
                        *out++ = (char)(codepoint - (uint32_t)U'｡' + 0x81);
                        continue;
                }
                break;
            case 0b11110000 ... 0b11110111:
                N -= 3;
                codepoint = (c & 0b111) << 18;
                codepoint |= ((uint8_t)*str++ & 0x3F) << 12;
                codepoint |= ((uint8_t)*str++ & 0x3F) << 6;
                codepoint |= ((uint8_t)*str++ & 0x3F);
                break;
        }

        uint16_t sjis = UTF_TO_SJIS[UTF_TO_SJIS_INDEX(codepoint)];
        *out++ = (char)(uint8_t)sjis;
        *out++ = (char)(uint8_t)(sjis >> 8);
    } while (--N);
}

template<StringLiteral ustr>
static inline constexpr const char* operator""_sjis() {
    using S = StringPool<[](){
        std::array<char, utf8_to_sjis_buffer_length(ustr.data(), ustr.size())> str = {};
        utf8_to_sjis(&str[0], ustr.data(), ustr.size());
        return StringLiteral(str);
    }()>;
    return S::data();
}
template<U8StringLiteral ustr>
static inline constexpr const char* operator""_sjis() {
    using S = StringPool<[](){
        std::array<char, utf8_to_sjis_buffer_length(ustr.data(), ustr.size())> str = {};
        utf8_to_sjis(&str[0], ustr.data(), ustr.size());
        return StringLiteral(str);
    }()>;
    return S::data();
}
template<StringLiteral ustr>
static inline constexpr std::string_view operator""_sjis_sv() {
    using S = StringPool<[](){
        std::array<char, utf8_to_sjis_buffer_length(ustr.data(), ustr.size())> str = {};
        utf8_to_sjis(&str[0], ustr.data(), ustr.size());
        return StringLiteral(str);
    }()>;
    return std::string_view(S::data(), S::length());
}
template<U8StringLiteral ustr>
static inline constexpr std::string_view operator""_sjis_sv() {
    using S = StringPool<[](){
        std::array<char, utf8_to_sjis_buffer_length(ustr.data(), ustr.size())> str = {};
        utf8_to_sjis(&str[0], ustr.data(), ustr.size());
        return StringLiteral(str);
    }()>;
    return std::string_view(S::data(), S::length());
}

#if __cpp_char8_t < 201811L
#undef char8_t
#endif

#endif