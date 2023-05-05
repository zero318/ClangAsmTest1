#if !_HAS_CXX17
#undef _HAS_CXX17
#define _HAS_CXX17 1
#endif
#if !_HAS_CXX20
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif

#include <stdlib.h>
#include <wchar.h>
#include <iostream>
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
using JsonString = std::wstring;
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
using JsonKey = std::wstring;
struct JsonObject : std::map<JsonKey, JsonValue> {};

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
    return os << '"' << json_string << '"';
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
        os << '"' << iter->first << "\":" << iter->second;
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

static inline const wchar_t* skip_whitespace(const wchar_t* str) {
    for (;; ++str) {
        switch (*str) {
            case L'/':
            // Explicitly mentioned in the spec
            case L' ': case L'\t': case L'\v': case L'\f': case L'\n': case L'\r':
            [[unlikely]] case L'\N{NO-BREAK SPACE}':            // '\u00A0'
            [[unlikely]] case L'\N{LINE SEPARATOR}':            // '\u2028'
            [[unlikely]] case L'\N{PARAGRAPH SEPARATOR}':       // '\u2029'
            [[unlikely]] case L'\N{ZERO WIDTH NO-BREAK SPACE}': // '\uFEFF' (Also byte order mark)
            // Unicode Zs Category
            [[unlikely]] case L'\N{OGHAM SPACE MARK}':          // '\u1680'
            [[unlikely]] case L'\N{EN QUAD}':                   // '\u2000'
            [[unlikely]] case L'\N{EM QUAD}':                   // '\u2001'
            [[unlikely]] case u'\N{EN SPACE}':                  // '\u2002'
            [[unlikely]] case L'\N{EM SPACE}':                  // '\u2003'
            [[unlikely]] case L'\N{THREE-PER-EM SPACE}':        // '\u2004'
            [[unlikely]] case L'\N{FOUR-PER-EM SPACE}':         // '\u2005'
            [[unlikely]] case L'\N{SIX-PER-EM SPACE}':          // '\u2006'
            [[unlikely]] case L'\N{FIGURE SPACE}':              // '\u2007'
            [[unlikely]] case L'\N{PUNCTUATION SPACE}':         // '\u2008'
            [[unlikely]] case L'\N{THIN SPACE}':                // '\u2009'
            [[unlikely]] case L'\N{HAIR SPACE}':                // '\u200A'
            [[unlikely]] case L'\N{NARROW NO-BREAK SPACE}':     // '\u202F'
            [[unlikely]] case L'\N{MEDIUM MATHEMATICAL SPACE}': // '\u205F'
            [[unlikely]] case L'\N{IDEOGRAPHIC SPACE}':         // '\u3000'
                continue;
            [[likely]] default:
                return str;
        }
    }
}

static inline const wchar_t* skip_numeric(const wchar_t* str) {
    for (;; ++str) {
        switch (*str) {
            case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
                continue;
        }
        return str;
    }
}

static inline const wchar_t* skip_hexdecimal(const wchar_t* str) {
    for (;; ++str) {
        switch (*str) {
            case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
            case L'a': case L'b': case L'c': case L'd': case L'e': case L'f':
            case L'A': case L'B': case L'C': case L'D': case L'E': case L'F':
                continue;
        }
        return str;
    }
}

static inline const wchar_t* skip_line_terminator(const wchar_t* str) {
    switch (*str) {
        case L'\r':
            if (str[1] == L'\n') {
                return str + 2;
            }
        case L'\n':
        [[unlikely]] case L'\N{LINE SEPARATOR}':        // '\u2028'
        [[unlikely]] case L'\N{PARAGRAPH SEPARATOR}':   // '\u2029'
            return str + 1;
        default:
            return str;
    }
}

JsonOptionalString parse_json5_string(const wchar_t*& raw_json, wchar_t terminator) {
    std::wstring str = u"";
    const wchar_t* str_segment = raw_json;
    size_t str_segment_length = 0;
    for (;;) {
        switch (wchar_t c = str_segment[str_segment_length]) {
            default:
                if (c >= L'\0' && c <= L'\x1F') [[unlikely]] {
                    return std::unexpected{ JsonStringError::InvalidUnescapedCharacter };
                }
                ++str_segment_length;
                continue;
            case L'\\': {
                size_t escape_length = 2;
                switch (c = str_segment[str_segment_length + 1]) {
                    case L'\r':
                        if (str_segment[str_segment_length + 2] == L'\n') {
                            escape_length = 3;
                        }
                    case L'\n':
                    [[unlikely]] case L'\N{LINE SEPARATOR}':        // '\u2028'
                    [[unlikely]] case L'\N{PARAGRAPH SEPARATOR}':   // '\u2029'
                        str.append(str_segment, str_segment_length);
                        str_segment += str_segment_length + escape_length;
                        str_segment_length = 0;
                        continue;
                    case L'u':
                        c = L'\0';
                        for (size_t i = 0; i < 4; ++i) {
                            switch (wchar_t c2 = str_segment[str_segment_length + 2 + i]) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonStringError::InvalidEscapeSequence };
                                case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
                                    c |= c2 - L'0' << 12 - i * 4;
                                    break;
                                case L'a': case L'b': case L'c': case L'd': case L'e': case L'f':
                                    c |= c2 - (L'a' - 10) << 12 - i * 4;
                                    break;
                                case L'A': case L'B': case L'C': case L'D': case L'E': case L'F':
                                    c |= c2 - (L'A' - 10) << 12 - i * 4;
                                    break;
                            }
                        }
                        escape_length = 6;
                        break;
                    case L'x':
                        c = L'\0';
                        for (size_t i = 0; i < 2; ++i) {
                            switch (wchar_t c2 = str_segment[str_segment_length + 2 + i]) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonStringError::InvalidEscapeSequence };
                                case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
                                    c |= c2 - L'0' << 4 - i * 4;
                                    break;
                                case L'a': case L'b': case L'c': case L'd': case L'e': case L'f':
                                    c |= c2 - (L'a' - 10) << 4 - i * 4;
                                    break;
                                case L'A': case L'B': case L'C': case L'D': case L'E': case L'F':
                                    c |= c2 - (L'A' - 10) << 4 - i * 4;
                                    break;
                            }
                        }
                        escape_length = 4;
                        break;
                    case L'0':
                        switch (str_segment[str_segment_length + 2]) {
                            [[likely]] default:
                                c = L'\0';
                                goto append_to_string;
                            case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
                        }
                    case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
                        return std::unexpected{ JsonStringError::InvalidEscapeSequence };
                    case L'b': c = L'\b'; break;
                    case L'f': c = L'\f'; break;
                    case L'n': c = L'\n'; break;
                    case L'r': c = L'\r'; break;
                    case L't': c = L'\t'; break;
                    case L'v': c = L'\v'; break;
                    case L'"': case L'\'': case L'\\': case L'/': default:
                }
append_to_string:
                str.append(str_segment, str_segment_length);
                str += c;
                str_segment += str_segment_length + escape_length;
                str_segment_length = 0;
                continue;
            }
            case L'"': case L'\'':
                if (c == terminator) {
                    raw_json = str_segment + str_segment_length + 1;
                    return str.append(str_segment, str_segment_length);
                }
                ++str_segment_length;
                continue;
        }
    }
}

template<bool skip_initial_whitespace = true>
JsonOptionalString parse_json5_key(const wchar_t*& raw_json) {
    const wchar_t* raw_json_read = raw_json;
    if constexpr (skip_initial_whitespace) {
        raw_json_read = skip_whitespace(raw_json_read);
    }
    switch (wchar_t c = *raw_json_read++) {
        case L'"': case L'\'':
    }
    if ( == L'"') [[likely]] {
        if (auto str = parse_json5_string(raw_json_read)) [[likely]] {
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read == L':') [[likely]] {
                raw_json = raw_json_read + 1;
                return str;
            }
        } else [[unlikely]] {
            return str;
        }
    }
    return std::unexpected{ JsonStringError::UnexpectedCharacter };
}

JsonOptionalValue parse_json5_value(const wchar_t*& raw_json) {
    const wchar_t* raw_json_read = skip_whitespace(raw_json);
    const wchar_t* numeric_str_start;
    switch (wchar_t c = *raw_json_read++) {
        case L't':
            if (std::wstring_view(raw_json_read, 3) == L"rue"sv) [[likely]] {
                raw_json = raw_json_read + 3;
                return JsonTrue{};
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case L'f':
            if (std::wstring_view(raw_json_read, 4) == L"alse"sv) [[likely]] {
                raw_json = raw_json_read + 4;
                return JsonFalse{};
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case L'n':
            if (std::wstring_view(raw_json_read, 3) == L"ull"sv) [[likely]] {
                raw_json = raw_json_read + 3;
                return JsonNull{};
            }
        [[unlikely]] default:
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case L'{': {
            JsonObject obj;
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read != L'}') {
                for (;;) {
                    if (auto key = parse_json5_key<false>(raw_json_read)) [[likely]] {
                        if (auto obj_element = parse_json5_value(raw_json_read)) [[likely]] {
                            obj[key.value()] = obj_element.value();
                            raw_json_read = skip_whitespace(raw_json_read);
                            switch (*raw_json_read++) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonValueError::UnexpectedCharacter };
                                case L'}':
                                    goto return_object;
                                case L',':
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
        case L'[': {
            JsonArray arr;
            raw_json_read = skip_whitespace(raw_json_read);
            if (*raw_json_read != L']') {
                for (;;) {
                    if (auto arr_element = parse_json5_value(raw_json_read)) [[likely]] {
                        arr.push_back(arr_element.value());
                        raw_json_read = skip_whitespace(raw_json_read);
                        switch (*raw_json_read++) {
                            [[unlikely]] default:
                                return std::unexpected{ JsonValueError::UnexpectedCharacter };
                            case L']':
                                goto return_array;
                            case L',':
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
        case L'"': case L'\'':
            if (auto str = parse_json5_string(raw_json_read)) [[likely]] {
                raw_json = raw_json_read;
                return str.value();
            } else [[unlikely]] {
                return std::unexpected{ JsonValueError::InvalidString };
            }
        case L'N':
            numeric_str_start = raw_json_read - 1;
nan_check:
            if (std::wstring_view(raw_json_read, 2) == L"aN"sv) [[likely]] {
                //raw_json_read += 2;
                break;
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case L'I':
            numeric_str_start = raw_json_read - 1;
inf_check:
            if (std::wstring_view(raw_json_read, 7) == L"nfinity"sv) [[likely]] {
                //raw_json_read += 7;
                break;
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case L'-': case L'+':
            numeric_str_start = raw_json_read - 1;
            switch (*raw_json_read++) {
                [[unlikely]] default:
                    return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                case L'0':
                    goto zero_check;
                case L'N':
                    goto nan_check;
                case L'I':
                    goto inf_check;
                //case L'.':
                    goto return_double;
                case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
            }
            break;
        case L'0':
            numeric_str_start = raw_json_read - 1;
zero_check:
            switch (c = *raw_json_read) {
                case L'x': case L'X':
                    raw_json_read = skip_hexdecimal(raw_json_read + 1);
                    switch (*raw_json_read) {
                        [[unlikely]] case L'p': [[unlikely]] case L'P':
                            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
                    }
            }
            break;
        case L'.': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
            numeric_str_start = raw_json_read - 1;
            break;
    }
return_double:
    wchar_t* numeric_str_end = NULL;
    double value = wcstod(numeric_str_start, &numeric_str_end);
    if (numeric_str_end) {
        raw_json = numeric_str_start + (numeric_str_end - numeric_str_start);
        return value;
    } else {
        return std::unexpected{ JsonValueError::InvalidParsingCharacter };
    }
}

JsonOptionalObject parse_json5_text(const wchar_t* raw_json) {
    if (auto value = parse_json5_value(raw_json)) [[likely]] {
        raw_json = skip_whitespace(raw_json);
        if (*raw_json == L'\0') [[likely]] {
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

__declspec(noinline) void test_json5_parse(const wchar_t* raw_json) {
    if (auto value = parse_json5_text(raw_json)) {
        std::wcout << value.value();
    } else {
        std::wcout << (size_t)value.error();
    }
}
