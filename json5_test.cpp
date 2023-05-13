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
#if !__INTELLISENSE__
#include "zero/expected.h"
#endif

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
            // Explicitly mentioned in the spec
            case L' ': case L'\t': case L'\v': case L'\f': case L'\n': case L'\r':
            [[unlikely]] case L'\u00A0': // '\N{NO-BREAK SPACE}'
            [[unlikely]] case L'\u2028': // '\N{LINE SEPARATOR}'
            [[unlikely]] case L'\u2029': // '\N{PARAGRAPH SEPARATOR}'
            [[unlikely]] case L'\uFEFF': // '\N{ZERO WIDTH NO-BREAK SPACE}' (Also byte order mark)
            // Unicode Zs Category
            [[unlikely]] case L'\u1680': // '\N{OGHAM SPACE MARK}'
            [[unlikely]] case L'\u2000': // '\N{EN QUAD}'
            [[unlikely]] case L'\u2001': // '\N{EM QUAD}'
            [[unlikely]] case L'\u2002': // '\N{EN SPACE}'
            [[unlikely]] case L'\u2003': // '\N{EM SPACE}'
            [[unlikely]] case L'\u2004': // '\N{THREE-PER-EM SPACE}'
            [[unlikely]] case L'\u2005': // '\N{FOUR-PER-EM SPACE}'
            [[unlikely]] case L'\u2006': // '\N{SIX-PER-EM SPACE}'
            [[unlikely]] case L'\u2007': // '\N{FIGURE SPACE}'
            [[unlikely]] case L'\u2008': // '\N{PUNCTUATION SPACE}'
            [[unlikely]] case L'\u2009': // '\N{THIN SPACE}'
            [[unlikely]] case L'\u200A': // '\N{HAIR SPACE}'
            [[unlikely]] case L'\u202F': // '\N{NARROW NO-BREAK SPACE}'
            [[unlikely]] case L'\u205F': // '\N{MEDIUM MATHEMATICAL SPACE}'
            [[unlikely]] case L'\u3000': // '\N{IDEOGRAPHIC SPACE}'
                continue;
            case L'/':
                switch (str[1]) {
                    [[likely]] case L'/': // Single line comment
                        for (str += 2;; ++str) {
                            switch (*str) {
                                case L'\r':
                                    if (str[1] == L'\n') {
                                        str += 2;
                                        goto single_line_comment_continue_outer_loop;
                                    }
                                case L'\n':
                                [[unlikely]] case L'\u2028': // '\N{LINE SEPARATOR}'
                                [[unlikely]] case L'\u2029': // '\N{PARAGRAPH SEPARATOR}'
                                    str += 1;
                                    goto single_line_comment_continue_outer_loop;
                                default:
                                    continue;
                            }
                        }
single_line_comment_continue_outer_loop:
                        continue;
                    [[likely]] case L'*': // Multi line comment
                        for (str += 2;; ++str) {
                            switch (*str) {
                                case L'*':
                                    if (str[1] == L'/') {
                                        str += 2;
                                        goto multi_line_comment_continue_outer_loop;
                                    }
                                default:
                                    continue;
                            }
                        }
multi_line_comment_continue_outer_loop:
                        continue;
                    [[unlikely]] default:
                        break;
                }
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

static inline const wchar_t* skip_hexadecimal(const wchar_t* str) {
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

JsonOptionalString parse_json5_string(const wchar_t*& raw_json, wchar_t terminator) {
    std::wstring str = L"";
    const wchar_t* str_segment = raw_json;
    size_t str_segment_length = 0;
    for (;;) {
        switch (wchar_t c = str_segment[str_segment_length]) {
            default:
                if (c == terminator) {
                    raw_json = str_segment + str_segment_length + 1;
                    return str.append(str_segment, str_segment_length);
                }
                if (c >= L'\0' && c <= L'\x1F') [[unlikely]] {
                    return std::unexpected{ JsonStringError::InvalidUnescapedCharacter };
                }
                ++str_segment_length;
                continue;
            case L'\\':
                size_t escape_length = 2;
                switch (c = str_segment[str_segment_length + 1]) {
                    case L'\r':
                        if (str_segment[str_segment_length + 2] == L'\n') {
                            escape_length = 3;
                        }
                    case L'\n':
                    [[unlikely]] case L'\u2028': // '\N{LINE SEPARATOR}'
                    [[unlikely]] case L'\u2029': // '\N{PARAGRAPH SEPARATOR}'
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
    }
}

JsonOptionalString parse_json5_string(const wchar_t*& raw_json) {
    std::wstring str = L"";
    const wchar_t* str_segment = raw_json;
    size_t str_segment_length = 0;
    for (;;) {
        switch (wchar_t c = str_segment[str_segment_length]) {
            // Explicitly mentioned in the spec
            case L' ': case L'\t': case L'\v': case L'\f': case L'\n': case L'\r': case ':':
            [[unlikely]] case L'\u00A0': // '\N{NO-BREAK SPACE}'
            [[unlikely]] case L'\u2028': // '\N{LINE SEPARATOR}'
            [[unlikely]] case L'\u2029': // '\N{PARAGRAPH SEPARATOR}'
            [[unlikely]] case L'\uFEFF': // '\N{ZERO WIDTH NO-BREAK SPACE}' (Also byte order mark)
            // Unicode Zs Category
            [[unlikely]] case L'\u1680': // '\N{OGHAM SPACE MARK}'
            [[unlikely]] case L'\u2000': // '\N{EN QUAD}'
            [[unlikely]] case L'\u2001': // '\N{EM QUAD}'
            [[unlikely]] case L'\u2002': // '\N{EN SPACE}'
            [[unlikely]] case L'\u2003': // '\N{EM SPACE}'
            [[unlikely]] case L'\u2004': // '\N{THREE-PER-EM SPACE}'
            [[unlikely]] case L'\u2005': // '\N{FOUR-PER-EM SPACE}'
            [[unlikely]] case L'\u2006': // '\N{SIX-PER-EM SPACE}'
            [[unlikely]] case L'\u2007': // '\N{FIGURE SPACE}'
            [[unlikely]] case L'\u2008': // '\N{PUNCTUATION SPACE}'
            [[unlikely]] case L'\u2009': // '\N{THIN SPACE}'
            [[unlikely]] case L'\u200A': // '\N{HAIR SPACE}'
            [[unlikely]] case L'\u202F': // '\N{NARROW NO-BREAK SPACE}'
            [[unlikely]] case L'\u205F': // '\N{MEDIUM MATHEMATICAL SPACE}'
            [[unlikely]] case L'\u3000': // '\N{IDEOGRAPHIC SPACE}'
                raw_json = str_segment + str_segment_length;
                return str.append(str_segment, str_segment_length);
            [[likely]] default:
                if (c >= L'\0' && c <= L'\x1F') [[unlikely]] {
                    return std::unexpected{ JsonStringError::InvalidUnescapedCharacter };
                }
                ++str_segment_length;
                continue;
            case L'\\':
                return std::unexpected{ JsonStringError::InvalidEscapeSequence };
        }
    }
}

JsonOptionalString parse_json5_key(const wchar_t*& raw_json, bool skip_initial_whitespace = true) {
    const wchar_t* raw_json_read = skip_initial_whitespace ? skip_whitespace(raw_json) : raw_json;
    JsonOptionalString str;
    switch (wchar_t c = *raw_json_read) {
        case L'"': case L'\'':
            ++raw_json_read;
            str = parse_json5_string(raw_json_read, c);
            break;
        default:
            str = parse_json5_string(raw_json_read);
            break;
    }
    if (str) [[likely]] {
        raw_json_read = skip_whitespace(raw_json_read);
        if (*raw_json_read == L':') [[likely]] {
            raw_json = raw_json_read + 1;
            return str;
        }
    } else [[unlikely]] {
        return str;
    }
    return std::unexpected{ JsonStringError::UnexpectedCharacter };
}

JsonOptionalValue parse_json5_value(const wchar_t*& raw_json, bool skip_initial_whitespace = true) {
    const wchar_t* raw_json_read = skip_initial_whitespace ? skip_whitespace(raw_json) : raw_json;
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
                    if (auto key = parse_json5_key(raw_json_read, false)) [[likely]] {
                        if (auto obj_element = parse_json5_value(raw_json_read)) [[likely]] {
                            obj[key.value()] = obj_element.value();
                            raw_json_read = skip_whitespace(raw_json_read);
                            switch (*raw_json_read++) {
                                [[unlikely]] default:
                                    return std::unexpected{ JsonValueError::UnexpectedCharacter };
                                case L',':
                                    raw_json_read = skip_whitespace(raw_json_read);
                                    if (*raw_json_read == L'}') {
                                        ++raw_json_read;
                                case L'}':
                                        goto return_object;
                                    }
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
                    if (auto arr_element = parse_json5_value(raw_json_read, false)) [[likely]] {
                        arr.push_back(arr_element.value());
                        raw_json_read = skip_whitespace(raw_json_read);
                        switch (*raw_json_read++) {
                            [[unlikely]] default:
                                return std::unexpected{ JsonValueError::UnexpectedCharacter };
                            case L',':
                                raw_json_read = skip_whitespace(raw_json_read);
                                if (*raw_json_read == L']') {
                                    ++raw_json_read;
                            case L']':
                                    goto return_array;
                                }
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
            if (auto str = parse_json5_string(raw_json_read, c)) [[likely]] {
                raw_json = raw_json_read;
                return str.value();
            } else [[unlikely]] {
                return std::unexpected{ JsonValueError::InvalidString };
            }
        case L'N':
            numeric_str_start = raw_json_read - 1;
nan_check:
            if (std::wstring_view(raw_json_read, 2) == L"aN"sv) [[likely]] {
                break;
            }
            return std::unexpected{ JsonValueError::InvalidParsingCharacter };
        case L'I':
            numeric_str_start = raw_json_read - 1;
inf_check:
            if (std::wstring_view(raw_json_read, 7) == L"nfinity"sv) [[likely]] {
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
                    //goto return_double;
                case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
            }
            break;
        case L'0':
            numeric_str_start = raw_json_read - 1;
zero_check:
            switch (c = *raw_json_read) {
                case L'x': case L'X':
                    raw_json_read = skip_hexadecimal(raw_json_read + 1);
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
