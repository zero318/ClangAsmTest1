#if !_HAS_CXX17
#undef _HAS_CXX17
#define _HAS_CXX17 1
#endif
#if !_HAS_CXX20
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif

#if !defined(JSON_USE_UTF8) && !defined(JSON_USE_UTF16)
#error Define either JSON_USE_UTF8 or JSON_USE_UTF16!
#endif

#include <stdlib.h>
#include <iostream>
#include <codecvt>
#include <type_traits>
#include <string>
#include <string_view>
#include <sstream>
#include <variant>
#include <vector>
#include <map>
#include "expected.h"

#if JSON_USE_UTF8
#define JC(c) u8##c
#define JV(s) u8##s##sv
using jstring = std::u8string;
using jstring_view = std::u8string_view;
using jchar = char8_t;
#elif JSON_USE_UTF16
#define JC(c) u##c
#define JV(s) u##s##sv
using jstring = std::u16string;
using jstring_view = std::u16string_view;
using jchar = char16_t;
#endif

using namespace std::literals::string_view_literals;

struct JsonNull {};
struct JsonTrue : std::true_type {};
struct JsonFalse : std::false_type {};
using JsonString = jstring;
using JsonNumber = double;
struct JsonArray;
struct JsonObject;
using JsonValue = std::variant<
    JsonNull,
    JsonTrue,
    JsonFalse,
    JsonNumber,
    JsonString,
    JsonArray,
    JsonObject
>;
struct JsonArray : std::vector<JsonValue> {};
using JsonKey = jstring;
struct JsonObject : std::map<JsonKey, JsonValue> {};


#if JSON_USE_UTF16
template<typename char_type = char>
static inline auto utf16string_to_string(const std::u16string& str) {
    if constexpr (std::is_same_v<char_type, char>) {
        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
        return converter.to_bytes(str);
    } else if constexpr (std::is_same_v<char_type, wchar_t>) {
        std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10FFFF, std::codecvt_mode::little_endian>, wchar_t> converter;
        return converter.from_bytes((const char*)&str[0], (const char*)(&str[0] + str.size()));
    } else {
        static_assert(std::is_same_v<char_type, char>);
    }
}

template<typename char_type>
static inline auto string_to_utf16string(const std::basic_string<char_type, std::char_traits<char_type>, std::allocator<char_type>>& str) {
    if constexpr (std::is_same_v<char_type, wchar_t>) {
        return str;
    } else {
        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
        return converter.from_bytes(str);
    }
}
#endif

template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream> || std::is_same_v<T, std::stringstream> || std::is_same_v<T, std::wstringstream>)
T& operator<<(T& os, const JsonValue& json_value) {
    std::visit([&](auto&& arg) {
        os << arg;
    }, json_value);
    return os;
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream> || std::is_same_v<T, std::stringstream> || std::is_same_v<T, std::wstringstream>)
T& operator<<(T& os, const JsonNull& json_null) {
    return os << "null";
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream> || std::is_same_v<T, std::stringstream> || std::is_same_v<T, std::wstringstream>)
T& operator<<(T& os, const JsonTrue& json_true) {
    return os << "true";
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream> || std::is_same_v<T, std::stringstream> || std::is_same_v<T, std::wstringstream>)
T& operator<<(T& os, const JsonFalse& json_false) {
    return os << "false";
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream> || std::is_same_v<T, std::stringstream> || std::is_same_v<T, std::wstringstream>)
T& operator<<(T& os, const JsonString& json_string) {
#if JSON_USE_UTF8
    return os << '"' << (const char*)json_string.c_str() << '"';
#elif JSON_USE_UTF16
    return os << '"' << utf16string_to_string<typename T::char_type>(json_string) << '"';
#endif
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream> || std::is_same_v<T, std::stringstream> || std::is_same_v<T, std::wstringstream>)
T& operator<<(T& os, const JsonArray& json_array) {
    os << '[';
    size_t size = json_array.size();
    for (size_t i = 0; i < size;) {
        os << json_array[i];
        if (++i < size) os << ',';
    }
    return os << ']';
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream> || std::is_same_v<T, std::stringstream> || std::is_same_v<T, std::wstringstream>)
T& operator<<(T& os, const JsonObject& json_object) {
    os << '{';
    for (
        auto iter = json_object.cbegin();
        iter != json_object.cend();
    ) {
#if JSON_USE_UTF8
        os << '"' << (const char*)iter->first.c_str() << "\":" << iter->second;
#elif JSON_USE_UTF16
        os << '"' << utf16string_to_string<typename T::char_type>(iter->first) << "\":" << iter->second;
#endif
        if (++iter != json_object.cend()) os << ',';
    }
    os << '}';
    return os;
}

enum class JsonValueError {
    InvalidParsingCharacter,
    UnexpectedCharacter,
    InvalidString,
    InvalidValue,
    InvalidObjectKey,
    OuterElementNotObject
};
enum class JsonStringError {
    InvalidCharacter,
    UnexpectedCharacter,
    InvalidEscapeSequence,
    InvalidUnescapedCharacter
};

using JsonOptionalValue = std::expected<JsonValue, JsonValueError>;
using JsonOptionalString = std::expected<JsonString, JsonStringError>;
using JsonOptionalObject = std::expected<JsonObject, JsonValueError>;

static inline const jchar* skip_whitespace(const jchar* str) {
    for (;; ++str) {
        switch (*str) {
            case JC(' '): case JC('\t'): case JC('\n'): case JC('\r'):
                continue;
            [[likely]] default:
                return str;
        }
    }
}

static inline const jchar* skip_numeric(const jchar* str) {
    for (;; ++str) {
        switch (*str) {
            case JC('0'): case JC('1'): case JC('2'): case JC('3'): case JC('4'): case JC('5'): case JC('6'): case JC('7'): case JC('8'): case JC('9'):
                continue;
        }
        return str;
    }
}

JsonOptionalString parse_json_string(const jchar*& raw_json) {
    jstring str = JC("");
    const jchar* str_segment = raw_json;
    size_t str_segment_length = 0;
    for (;;) {
        switch (jchar c = str_segment[str_segment_length]) {
            default:
                if (c <= JC('\x1F')) [[unlikely]] {
                    return std::unexpected{ JsonStringError::InvalidUnescapedCharacter };
                }
                ++str_segment_length;
                continue;
            case JC('\\'): {
                size_t escape_length = 2;
                switch (c = str_segment[str_segment_length + 1]) {
                    [[unlikely]] default:
                        return std::unexpected{ JsonStringError::InvalidEscapeSequence };
                    case JC('u'):
                        c = JC('\0');
                        for (size_t i = 0; i < 4; ++i) {
                            switch (jchar c2 = str_segment[str_segment_length + 2 + i]) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonStringError::InvalidEscapeSequence };
                                case JC('0'): case JC('1'): case JC('2'): case JC('3'): case JC('4'): case JC('5'): case JC('6'): case JC('7'): case JC('8'): case JC('9'):
                                    c |= c2 - JC('0') << 12 - i * 4;
                                    break;
                                case JC('a'): case JC('b'): case JC('c'): case JC('d'): case JC('e'): case JC('f'):
                                    c |= c2 - (JC('a') - 10) << 12 - i * 4;
                                    break;
                                case JC('A'): case JC('B'): case JC('C'): case JC('D'): case JC('E'): case JC('F'):
                                    c |= c2 - (JC('A') - 10) << 12 - i * 4;
                                    break;
                            }
                        }
                        escape_length = 6;
                        break;
                    case JC('b'): c = JC('\b'); break;
                    case JC('f'): c = JC('\f'); break;
                    case JC('n'): c = JC('\n'); break;
                    case JC('r'): c = JC('\r'); break;
                    case JC('t'): c = JC('\t'); break;
                    case JC('"'): case JC('\\'): case JC('/'):
                }
                str.append(str_segment, str_segment_length);
                str += c;
                str_segment += str_segment_length + escape_length;
                str_segment_length = 0;
                continue;
            }
            case JC('"'):
                raw_json = str_segment + str_segment_length + 1;
                return str.append(str_segment, str_segment_length);
        }
    }
}

template<bool skip_initial_whitespace = true>
JsonOptionalString parse_json_key(const jchar*& raw_json) {
    const jchar* raw_json_read = raw_json;
    if constexpr (skip_initial_whitespace) {
        raw_json_read = skip_whitespace(raw_json_read);
    }
    if (*raw_json_read++ == JC('"')) [[likely]] {
        if (auto str = parse_json_string(raw_json_read)) [[likely]] {
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read == JC(':')) [[likely]] {
                raw_json = raw_json_read + 1;
                return str;
            }
        } else [[unlikely]] {
            return str;
        }
    }
    return std::unexpected{ JsonStringError::UnexpectedCharacter };
}

template<bool skip_initial_whitespace = true>
JsonOptionalValue parse_json_value(const jchar*& raw_json) {
    const jchar* raw_json_read = raw_json;
    if constexpr (skip_initial_whitespace) {
        raw_json_read = skip_whitespace(raw_json_read);
    }
    const jchar* numeric_str_start;
    switch (*raw_json_read++) {
        case JC('t'):
            if (jstring_view(raw_json_read, 3) == JV("rue")) [[likely]] {
                raw_json = raw_json_read + 3;
                return JsonTrue{};
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case JC('f'):
            if (jstring_view(raw_json_read, 4) == JC("alse")) [[likely]] {
                raw_json = raw_json_read + 4;
                return JsonFalse{};
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case JC('n'):
            if (jstring_view(raw_json_read, 3) == JC("ull")) [[likely]] {
                raw_json = raw_json_read + 3;
                return JsonNull{};
            }
        [[unlikely]] default:
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case JC('{'): {
            JsonObject obj;
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read != JC('}')) {
                for (;;) {
                    if (auto key = parse_json_key<false>(raw_json_read)) [[likely]] {
                        if (auto obj_element = parse_json_value(raw_json_read)) [[likely]] {
                            obj[key.value()] = obj_element.value();
                            raw_json_read = skip_whitespace(raw_json_read);
                            switch (*raw_json_read++) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonValueError::UnexpectedCharacter };
                                case JC('}'):
                                    goto return_object;
                                case JC(','):
                                    raw_json_read = skip_whitespace(raw_json_read);
                                    continue;
                            }
                        } else [[unlikely]] {
                            return std::unexpected{ JsonValueError::InvalidValue };
                        }
                    } else [[unlikely]] {
                        return std::unexpected{ JsonValueError::InvalidObjectKey };
                    }
                }
            } else {
                ++raw_json_read;
            }
return_object:
            raw_json = raw_json_read;
            return obj;
        }
        case JC('['): {
            JsonArray arr;
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read != JC(']')) {
                for (;;) {
                    if (auto arr_element = parse_json_value<false>(raw_json_read)) [[likely]] {
                        arr.push_back(arr_element.value());
                        raw_json_read = skip_whitespace(raw_json_read);
                        switch (*raw_json_read++) {
                            [[unlikely]] default:
                                return std::unexpected{ JsonValueError::UnexpectedCharacter };
                            case JC(']'):
                                goto return_array;
                            case JC(','):
                                raw_json_read = skip_whitespace(raw_json_read);
                                continue;
                        }
                    } else [[unlikely]] {
                        return std::unexpected{ JsonValueError::InvalidValue };
                    }
                }
            } else {
                ++raw_json_read;
            }
return_array:
            raw_json = raw_json_read;
            return arr;
        }
        case JC('"'):
            if (auto str = parse_json_string(raw_json_read)) [[likely]] {
                raw_json = raw_json_read;
                return str.value();
            } else [[unlikely]] {
                return std::unexpected{ JsonValueError::InvalidString };
            }
        case JC('-'):
            numeric_str_start = raw_json_read - 1;
            switch (*raw_json_read++) {
                [[unlikely]] default:
                    return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                case JC('0'):
                    goto numeric_decimal_check;
                case JC('1'): case JC('2'): case JC('3'): case JC('4'): case JC('5'): case JC('6'): case JC('7'): case JC('8'): case JC('9'):
            }
            break;
        case JC('0'):
            numeric_str_start = raw_json_read - 1;
            goto numeric_decimal_check;
        case JC('1'): case JC('2'): case JC('3'): case JC('4'): case JC('5'): case JC('6'): case JC('7'): case JC('8'): case JC('9'):
            numeric_str_start = raw_json_read - 1;
            break;
    }
    raw_json_read = skip_numeric(raw_json_read);
numeric_decimal_check:
    jchar c = *raw_json_read;
    if (c == JC('.')) {
        switch (*++raw_json_read) {
            [[unlikely]] default:
                return std::unexpected{ JsonValueError::InvalidParsingCharacter };
            case JC('0'): case JC('1'): case JC('2'): case JC('3'): case JC('4'): case JC('5'): case JC('6'): case JC('7'): case JC('8'): case JC('9'):
                raw_json_read = skip_numeric(raw_json_read + 1);
                c = *raw_json_read;
        }
    }
    switch (c) {
        case JC('e'): case JC('E'):
            switch (*++raw_json_read) {
                [[unlikely]] default:
                    return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                case JC('+'): case JC('-'):
                    switch (*++raw_json_read) {
                        [[unlikely]] default:
                            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                        case JC('0'): case JC('1'): case JC('2'): case JC('3'): case JC('4'): case JC('5'): case JC('6'): case JC('7'): case JC('8'): case JC('9'):
                    }
                case JC('0'): case JC('1'): case JC('2'): case JC('3'): case JC('4'): case JC('5'): case JC('6'): case JC('7'): case JC('8'): case JC('9'):
                    raw_json_read = skip_numeric(raw_json_read + 1);
            }
    }
#if JSON_USE_UTF8
    return strtod((const char*)raw_json_read, NULL);
#elif JSON_USE_UTF16
    size_t temp_length = raw_json_read - numeric_str_start;
    char temp[temp_length + 1];
    temp[temp_length] = '\0';
    for (size_t i = 0; i < temp_length; ++i) {
        temp[i] = numeric_str_start[i];
    }
    raw_json = raw_json_read;
    return strtod(temp, NULL);
#endif
}

static JsonOptionalObject parse_json_text(const jchar* raw_json) {
    if (auto value = parse_json_value(raw_json)) [[likely]] {
        raw_json = skip_whitespace(raw_json);
        if (*raw_json == JC('\0')) [[likely]] {
            if (auto* ret_ptr = std::get_if<JsonObject>(&value.value())) [[likely]] {
                return *ret_ptr;
            } else [[unlikely]] {
                return std::unexpected{JsonValueError::OuterElementNotObject};
            }
        } else [[unlikely]] {
            return std::unexpected{JsonValueError::UnexpectedCharacter};
        }
    } else [[unlikely]] {
        return std::unexpected{value.error()};
    }
}