#if !_HAS_CXX17
#undef _HAS_CXX17
#define _HAS_CXX17 1
#endif
#if !_HAS_CXX20
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif

#include <stdlib.h>
#include <iostream>
#include <codecvt>
#include <type_traits>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <map>
#include "zero/expected.h"

using namespace std::literals::string_view_literals;

struct JsonNull {};
struct JsonTrue : std::true_type {};
struct JsonFalse : std::false_type {};
using JsonString = std::u16string;
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
using JsonKey = std::u16string;
struct JsonObject : std::map<JsonKey, JsonValue> {};

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

template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream>)
T& operator<<(T& os, const JsonValue& json_value) {
    std::visit([&](auto&& arg) {
        os << arg;
    }, json_value);
    return os;
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream>)
T& operator<<(T& os, const JsonNull& json_null) {
    return os << "null";
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream>)
T& operator<<(T& os, const JsonTrue& json_true) {
    return os << "true";
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream>)
T& operator<<(T& os, const JsonFalse& json_false) {
    return os << "false";
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream>)
T& operator<<(T& os, const JsonString& json_string) {
    return os << '"' << utf16string_to_string<typename T::char_type>(json_string) << '"';
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream>)
T& operator<<(T& os, const JsonArray& json_array) {
    os << '[';
    size_t size = json_array.size();
    for (size_t i = 0; i < size;) {
        os << json_array[i];
        if (++i < size) os << ',';
    }
    return os << ']';
}
template<typename T> requires(std::is_same_v<T, std::ostream> || std::is_same_v<T, std::wostream>)
T& operator<<(T& os, const JsonObject& json_object) {
    os << '{';
    for (
        auto iter = json_object.cbegin();
        iter != json_object.cend();
    ) {
        os << '"' << utf16string_to_string<typename T::char_type>(iter->first) << "\":" << iter->second;
        if (++iter != json_object.cend()) os << ',';
    }
    return os << '}';
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

static inline const char16_t* skip_whitespace(const char16_t* str) {
    for (;; ++str) {
        switch (*str) {
            case u' ': case u'\t': case u'\n': case u'\r':
                continue;
            [[likely]] default:
                return str;
        }
    }
}

static inline const char16_t* skip_numeric(const char16_t* str) {
    for (;; ++str) {
        switch (*str) {
            case u'0': case u'1': case u'2': case u'3': case u'4': case u'5': case u'6': case u'7': case u'8': case u'9':
                continue;
        }
        return str;
    }
}

JsonOptionalString parse_json_string(const char16_t*& raw_json) {
    std::u16string str = u"";
    const char16_t* str_segment = raw_json;
    size_t str_segment_length = 0;
    for (;;) {
        switch (char16_t c = str_segment[str_segment_length]) {
            default:
                if (c <= u'\x1F') [[unlikely]] {
                    return std::unexpected{ JsonStringError::InvalidUnescapedCharacter };
                }
                ++str_segment_length;
                continue;
            case u'\\': {
                size_t escape_length = 2;
                switch (c = str_segment[str_segment_length + 1]) {
                    [[unlikely]] default:
                        return std::unexpected{ JsonStringError::InvalidEscapeSequence };
                    case u'u':
                        c = u'\0';
                        for (size_t i = 0; i < 4; ++i) {
                            switch (char16_t c2 = str_segment[str_segment_length + 2 + i]) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonStringError::InvalidEscapeSequence };
                                case u'0': case u'1': case u'2': case u'3': case u'4': case u'5': case u'6': case u'7': case u'8': case u'9':
                                    c |= c2 - u'0' << 12 - i * 4;
                                    break;
                                case u'a': case u'b': case u'c': case u'd': case u'e': case u'f':
                                    c |= c2 - (u'a' - 10) << 12 - i * 4;
                                    break;
                                case u'A': case u'B': case u'C': case u'D': case u'E': case u'F':
                                    c |= c2 - (u'A' - 10) << 12 - i * 4;
                                    break;
                            }
                        }
                        escape_length = 6;
                        break;
                    case u'b': c = u'\b'; break;
                    case u'f': c = u'\f'; break;
                    case u'n': c = u'\n'; break;
                    case u'r': c = u'\r'; break;
                    case u't': c = u'\t'; break;
                    case u'"': case u'\\': case u'/':
                }
                str.append(str_segment, str_segment_length);
                str += c;
                str_segment += str_segment_length + escape_length;
                str_segment_length = 0;
                continue;
            }
            case u'"':
                raw_json = str_segment + str_segment_length + 1;
                return str.append(str_segment, str_segment_length);
        }
    }
}

template<bool skip_initial_whitespace = true>
JsonOptionalString parse_json_key(const char16_t*& raw_json) {
    const char16_t* raw_json_read = raw_json;
    if constexpr (skip_initial_whitespace) {
        raw_json_read = skip_whitespace(raw_json_read);
    }
    if (*raw_json_read++ == u'"') [[likely]] {
        if (auto str = parse_json_string(raw_json_read)) [[likely]] {
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read == u':') [[likely]] {
                raw_json = raw_json_read + 1;
                return str;
            }
        } else [[unlikely]] {
            return str;
        }
    }
    return std::unexpected{ JsonStringError::UnexpectedCharacter };
}

JsonOptionalValue parse_json_value(const char16_t*& raw_json) {
    const char16_t* raw_json_read = skip_whitespace(raw_json);
    const char16_t* numeric_str_start;
    switch (*raw_json_read++) {
        case u't':
            if (std::u16string_view(raw_json_read, 3) == u"rue"sv) [[likely]] {
                raw_json = raw_json_read + 3;
                return JsonTrue{};
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case u'f':
            if (std::u16string_view(raw_json_read, 4) == u"alse"sv) [[likely]] {
                raw_json = raw_json_read + 4;
                return JsonFalse{};
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case u'n':
            if (std::u16string_view(raw_json_read, 3) == u"ull"sv) [[likely]] {
                raw_json = raw_json_read + 3;
                return JsonNull{};
            }
        [[unlikely]] default:
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case u'{': {
            JsonObject obj;
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read != u'}') {
                for (;;) {
                    if (auto key = parse_json_key<false>(raw_json_read)) [[likely]] {
                        if (auto obj_element = parse_json_value(raw_json_read)) [[likely]] {
                            obj[key.value()] = obj_element.value();
                            raw_json_read = skip_whitespace(raw_json_read);
                            switch (*raw_json_read++) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonValueError::UnexpectedCharacter };
                                case u'}':
                                    goto return_object;
                                case u',':
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
        case u'[': {
            JsonArray arr;
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read != u']') {
                for (;;) {
                    if (auto arr_element = parse_json_value(raw_json_read)) [[likely]] {
                        arr.push_back(arr_element.value());
                        raw_json_read = skip_whitespace(raw_json_read);
                        switch (*raw_json_read++) {
                            [[unlikely]] default:
                                return std::unexpected{ JsonValueError::UnexpectedCharacter };
                            case u']':
                                goto return_array;
                            case u',':
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
        case u'"':
            if (auto str = parse_json_string(raw_json_read)) [[likely]] {
                raw_json = raw_json_read;
                return str.value();
            } else [[unlikely]] {
                return std::unexpected{ JsonValueError::InvalidString };
            }
        case u'-':
            numeric_str_start = raw_json_read - 1;
            switch (*raw_json_read++) {
                [[unlikely]] default:
                    return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                case u'0':
                    goto numeric_decimal_check;
                case u'1': case u'2': case u'3': case u'4': case u'5': case u'6': case u'7': case u'8': case u'9':
            }
            break;
        case u'0':
            numeric_str_start = raw_json_read - 1;
            goto numeric_decimal_check;
        case u'1': case u'2': case u'3': case u'4': case u'5': case u'6': case u'7': case u'8': case u'9':
            numeric_str_start = raw_json_read - 1;
            break;
    }
    raw_json_read = skip_numeric(raw_json_read);
numeric_decimal_check:
    char16_t c = *raw_json_read;
    if (c == u'.') {
        switch (*++raw_json_read) {
            [[unlikely]] default:
                return std::unexpected{ JsonValueError::InvalidParsingCharacter };
            case u'0': case u'1': case u'2': case u'3': case u'4': case u'5': case u'6': case u'7': case u'8': case u'9':
                raw_json_read = skip_numeric(raw_json_read + 1);
                c = *raw_json_read;
        }
    }
    switch (c) {
        case u'e': case u'E':
            switch (*++raw_json_read) {
                [[unlikely]] default:
                    return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                case u'+': case u'-':
                    switch (*++raw_json_read) {
                        [[unlikely]] default:
                            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                        case u'0': case u'1': case u'2': case u'3': case u'4': case u'5': case u'6': case u'7': case u'8': case u'9':
                    }
                case u'0': case u'1': case u'2': case u'3': case u'4': case u'5': case u'6': case u'7': case u'8': case u'9':
                    raw_json_read = skip_numeric(raw_json_read + 1);
            }
    }
    size_t temp_length = raw_json_read - numeric_str_start;
    char temp[temp_length + 1];
    temp[temp_length] = '\0';
    for (size_t i = 0; i < temp_length; ++i) {
        temp[i] = numeric_str_start[i];
    }
    raw_json = raw_json_read;
    return strtod(temp, NULL);
}

JsonOptionalObject parse_json_text(const char16_t* raw_json) {
    if (auto value = parse_json_value(raw_json)) [[likely]] {
        raw_json = skip_whitespace(raw_json);
        if (*raw_json == u'\0') [[likely]] {
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

__declspec(noinline) void test_json_parse(const char16_t* raw_json) {
    if (auto value = parse_json_text(raw_json)) {
        std::wcout << value.value();
    } else {
        std::wcout << (size_t)value.error();
    }
}
