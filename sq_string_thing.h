#include <algorithm>
#include <utility>
#include <type_traits>

template<typename C, size_t N = 1>
struct StringLiteralBase {

    static inline constexpr size_t length = N - 1;

    C buf[N];
    constexpr StringLiteralBase(const C (&str)[N]) {
        for (size_t i = 0; i < length; ++i) this->buf[i] = str[i];
        this->buf[length] = (C)'\0';
    }

    constexpr StringLiteralBase(C c) : buf{c, (C)'\0'} {}

    template<typename T> requires(std::is_integral_v<T> && !std::is_same_v<T, C>)
    constexpr StringLiteralBase(T value) : buf{} {
        size_t digit_offset = 0;
        C* str = this->buf;
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                *str++ = (C)'-';
            } else {
                value = -value;
            }
            for (T temp = value; temp < -9; temp /= 10) ++digit_offset;
            do {
                str[digit_offset] = (C)'0' - value % 10;
                value /= 10;
            } while (digit_offset--);
        } else {
            for (T temp = value; temp > 9; temp /= 10) ++digit_offset;
            do {
                str[digit_offset] = (C)'0' + value % 10;
                value /= 10;
            } while (digit_offset--);
        }
    }

    constexpr operator const C*() const {
        return this->buf;
    }

    template<typename T> requires(std::is_integral_v<T> && !std::is_same_v<T, C>)
    static inline constexpr size_t length_for_value(T value) {
        size_t length = 1;
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                ++length;
            } else {
                value = -value;
            }
            for (;value < -9; value /= 10) ++length;
        } else {
            for (;value > 9; value /= 10) ++length;
        }
        return length;
    }
    template<size_t L>
    static inline constexpr size_t length_for_value(const C(&value)[L]) {
        size_t length = 0;
        while (value[length]) ++length;
        return length;
    }
    static inline constexpr size_t length_for_value(C value) {
        return 1;
    }
    template<size_t L>
    static inline constexpr size_t length_for_value(const StringLiteralBase<C, L>& value) {
        size_t length = 0;
        while (value[length]) ++length;
        return length;
    }

    template<typename T, typename ... Args>
    static inline constexpr void make_literal_impl(C* str, T&& value, Args&&... args) {
        size_t length = length_for_value(value);
        for (size_t i = 0; i < length; ++i) *str++ = value[i];
        if constexpr (sizeof...(Args) != 0) {
            make_literal_impl(str, std::forward<Args>(args)...);
        }
    }
};

template<size_t N = 1>
struct StringLiteral : StringLiteralBase<char, N> {
    using StringLiteralBase<char, N>::StringLiteralBase;
};

template<size_t N = 1>
struct WStringLiteral : StringLiteralBase<wchar_t, N> {
    using StringLiteralBase<wchar_t, N>::StringLiteralBase;
};

template<size_t N>
StringLiteral(const char(&str)[N]) -> StringLiteral<N>;
template<size_t N>
WStringLiteral(const wchar_t(&str)[N]) -> WStringLiteral<N>;

template<typename T> requires(std::is_integral_v<T> && !std::is_same_v<T, char>)
StringLiteral(T value) -> StringLiteral<std::numeric_limits<T>::digits10 + 2>;
template<typename T> requires(std::is_integral_v<T> && !std::is_same_v<T, wchar_t>)
WStringLiteral(T value) -> WStringLiteral<std::numeric_limits<T>::digits10 + 2>;

StringLiteral(char c) -> StringLiteral<2>;
WStringLiteral(wchar_t c) -> WStringLiteral<2>;

template<StringLiteral... strs, size_t N = 1 + (StringLiteral<>::length_for_value(strs) + ...)> requires(sizeof...(strs) != 0)
static inline constexpr StringLiteral<N> make_literal() {
    char chars[N]{};
    StringLiteral<>::make_literal_impl(chars, std::forward<const decltype(strs)>(strs)...);
    return chars;
}

template<WStringLiteral... strs, size_t N = 1 + (WStringLiteral<>::length_for_value(strs) + ...)> requires(sizeof...(strs) != 0)
static inline constexpr WStringLiteral<N> make_wliteral() {
    wchar_t chars[N]{};
    WStringLiteral<>::make_literal_impl(chars, std::forward<const decltype(strs)>(strs)...);
    return chars;
}

#ifdef SQUNICODE
#define make_sqliteral(...) make_wliteral<__VA_ARGS__>()
#else
#define make_sqliteral(...) make_literal<__VA_ARGS__>()
#endif