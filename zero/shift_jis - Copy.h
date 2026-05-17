#pragma once

#ifndef SHIFT_JIS_H
#define SHIFT_JIS_H 1

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <type_traits>
#include <string_view>

#if !__INTELLISENSE__
#include "utf_to_sjis.h"
#else
#define UTF_TO_SJIS_INDEX(i) ((i)-167)
static inline constexpr uint16_t UTF_TO_SJIS[1] = {};
#endif

/*
template<size_t N>
static inline constexpr uint32_t u8_rep(const char8_t(&c)[N]) {
    uint32_t raw = 0;
    for (size_t i = 0; i != N - 1; ++i) {
        raw |= (uint8_t)c[i] << i * 8;
    }
    return raw;
}

static inline constexpr uint32_t u8_rep(char32_t c) {
    uint32_t ci = (uint32_t)c;
    if (ci < 0x80) {
        return ci;
    }
    uint32_t raw = 0x8000 | (ci & 0b111111) << 8;
    if (ci < 0x800) {
        return raw | 0b11000000 | ci >> 6 & 0b11111;
    }
    raw = raw << 8 | 0x8000 | (ci & 0b111111000000) << 2;
    if (ci < 0x10000) {
        return raw | 0b11100000 | ci >> 12 & 0b1111;
    }
    return raw << 8 | 0x80F0 | (ci & 0b111111000000000000) >> 4 | ci >> 18 & 0b111;
}
*/

#if __cpp_char8_t < 201811L
#define char8_t unsigned char
#endif

template<typename C, size_t N>
struct StringLiteralBase {
    C buf[N];

    inline constexpr StringLiteralBase(const C(&str)[N]) {
        for (size_t i = 0; i != N - 1; ++i) this->buf[i] = str[i];
        this->buf[N - 1] = (C)'\0';
    }

    inline constexpr const C* data() const {
        return &this->buf[0];
    }

    inline constexpr size_t length() const {
        return N - 1;
    }

    inline constexpr C& operator[](size_t index) {
        return this->buf[index];
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
struct WStringLiteral : StringLiteralBase<wchar_t, N> {
    using StringLiteralBase<wchar_t, N>::StringLiteralBase;
};
template<size_t N>
struct U8StringLiteral : StringLiteralBase<char8_t, N> {
    using StringLiteralBase<char8_t, N>::StringLiteralBase;
};
template<size_t N>
struct U16StringLiteral : StringLiteralBase<char16_t, N> {
    using StringLiteralBase<char16_t, N>::StringLiteralBase;
};
template<size_t N>
struct U32StringLiteral : StringLiteralBase<char32_t, N> {
    using StringLiteralBase<char32_t, N>::StringLiteralBase;
};

template<size_t N>
StringLiteral(const char(&str)[N])->StringLiteral<N>;
template<size_t N>
WStringLiteral(const wchar_t(&str)[N])->WStringLiteral<N>;
template<size_t N>
U8StringLiteral(const char8_t(&str)[N])->U8StringLiteral<N>;
template<size_t N>
U16StringLiteral(const char16_t(&str)[N])->U16StringLiteral<N>;
template<size_t N>
U32StringLiteral(const char32_t(&str)[N])->U32StringLiteral<N>;

// Single byte SJIS characters:
// ASCII
// ｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ

static inline constexpr size_t buffer_length_as_sjis(const char8_t* str, size_t N) {
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
                codepoint |= ((uint8_t)*str++ & 0x111111) << 6;
                codepoint |= ((uint8_t)*str++ & 0x111111);
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

static inline constexpr void utf8_to_sjis(char* out, const char8_t* str, size_t N) {
    do {
        uint32_t codepoint = 0;
        switch (uint8_t c = (uint8_t)*str++) {
            case 0b00000000 ... 0b01111111:
                *out++ = (char)c; // Single byte character in both UTF8 / SJIS
                continue;
            case 0b11000000 ... 0b11011111:
                N -= 1;
                codepoint = (c & 0b11111) << 6;
                codepoint |= (uint8_t)*str++ & 0x111111;

                break;
            case 0b11100000 ... 0b11101111:
                N -= 2;
                codepoint = (c & 0b1111) << 12;
                codepoint |= ((uint8_t)*str++ & 0x111111) << 6;
                codepoint |= ((uint8_t)*str++ & 0x111111);
                switch (codepoint) {
                    case U'｡' ... U'ﾟ': // Single byte character in SJIS
                        *out++ = (char)(codepoint - (uint32_t)U'｡' + 0x81);
                        continue;
                }
                break;
            case 0b11110000 ... 0b11110111:
                N -= 3;
                codepoint = (c & 0b111) << 18;
                codepoint |= ((uint8_t)*str++ & 0x111111) << 12;
                codepoint |= ((uint8_t)*str++ & 0x111111) << 6;
                codepoint |= ((uint8_t)*str++ & 0x111111);
                break;
        }

        uint16_t sjis = UTF_TO_SJIS[UTF_TO_SJIS_INDEX(codepoint)];
        *out++ = (char)((uint8_t)sjis);
        *out++ = (char)((uint8_t)sjis >> 8);
        
    } while (--N);
}

template<U8StringLiteral ustr>
static inline constexpr const char* operator""_sjis() {
    static constexpr size_t SJIS_LENGTH = buffer_length_as_sjis(ustr.data(), ustr.length());
    static constexpr std::array<char, SJIS_LENGTH> STR = []() {
        std::array<char, SJIS_LENGTH> str = {};
        utf8_to_sjis(&str[0], ustr.data(), ustr.length());
        return str;
    }();
    return &STR[0];
}

#if __cpp_char8_t < 201811L
#undef char8_t
#endif

#endif