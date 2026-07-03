#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>

using namespace std::literals;

#include "zero/util.h"

template<typename C, size_t N>
struct StringLiteralBase {
    alignas(16) const std::array<C, N> buf;

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
StringLiteral(const char(&)[N])->StringLiteral<N>;

template<size_t N>
StringLiteral(const std::array<char, N>&)->StringLiteral<N>;

template<auto str>
struct StringPool {
    static inline constexpr auto data() {
#if __INTELLISENSE__
        return (const char*)NULL;
#else
        return str.data();
#endif
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

#if __INTELLISENSE__
static inline constexpr const char* operator""_str(const char*) {
    return StringPool<0>::data();
}
#else
template<char ... str>
static inline constexpr const char* operator""_str() {
    return StringPool<StringLiteral(str...)>::data();
}
#endif

struct DOS_HEADER {
    uint16_t e_magic;                     // Magic number
    uint16_t e_cblp;                      // Bytes on last page of file
    uint16_t e_cp;                        // Pages in file
    uint16_t e_crlc;                      // Relocations
    uint16_t e_cparhdr;                   // Size of header in paragraphs
    uint16_t e_minalloc;                  // Minimum extra paragraphs needed
    uint16_t e_maxalloc;                  // Maximum extra paragraphs needed
    uint16_t e_ss;                        // Initial (relative) SS value
    uint16_t e_sp;                        // Initial SP value
    uint16_t e_csum;                      // Checksum
    uint16_t e_ip;                        // Initial IP value
    uint16_t e_cs;                        // Initial (relative) CS value
    uint16_t e_lfarlc;                    // File address of relocation table
    uint16_t e_ovno;                      // Overlay number
    uint16_t e_res[4];                    // Reserved words
    uint16_t e_oemid;                     // OEM identifier (for e_oeminfo)
    uint16_t e_oeminfo;                   // OEM information; e_oemid specific
    uint16_t e_res2[10];                  // Reserved words
    int32_t e_lfanew;                     // File address of new exe header
};

[[noreturn]] static void error_exit(const char* message) {
    fputs(message, stderr);
    exit(EXIT_FAILURE);
}

template <typename T = void>
[[nodiscard]] inline auto read_file_to_buffer(const char* path) {
    long file_size = 0;
    T* buffer = NULL;
    if (FILE* file = fopen(path, "rb")) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        if ((buffer = (T*)malloc(file_size))) {
            rewind(file);
            fread(buffer, file_size, 1, file);
        }
        fclose(file);
    }
    return std::make_pair(buffer, file_size);
}

template<typename T, size_t N>
struct BSearchListBase {
    using type = T;
    static inline constexpr size_t length = N;

    static inline constexpr size_t element_size = sizeof(T);

    const std::array<T, N> data;

    inline constexpr BSearchListBase(const T(&arr)[N]) : data(std::to_array(arr)) {}

    inline BSearchListBase() = default;
    inline constexpr BSearchListBase(const BSearchListBase&) = default;
    inline constexpr BSearchListBase(BSearchListBase&&) = default;
    inline constexpr BSearchListBase& operator=(const BSearchListBase&) = default;
    inline constexpr BSearchListBase& operator=(BSearchListBase&&) = default;

    template<typename L>
    constexpr ssize_t find(const T& val, const L& lambda) const {
        size_t left = 0;
        size_t right = N - 1;
        do {
            size_t i = left + ((right - left) >> 1);
            auto cmp_val = lambda(val, this->data[i]);
            if (!cmp_val) {
                return i;
            } else if (cmp_val < 0) {
                right = i - 1;
            } else {
                left = i + 1;
            }
        } while (left != right);
        return -1;
    }
};

template<typename T, size_t N>
struct BSearchList : BSearchListBase<T, N> {
    using BSearchListBase<T,N>::BSearchListBase;
};

template<size_t N>
struct BSearchList<const char*,N> : BSearchListBase<const char*, N> {
    using T = const char*;
    using super = BSearchListBase<T, N>;
    using BSearchListBase<T, N>::BSearchListBase;

    /*
    constexpr ssize_t find(const char* val) const {
        return this->super::find(val, [](const char* lhs, const char* rhs) {
            return byteloop_strcmp(lhs, rhs);
        });
    }
    */

    constexpr ssize_t find(const char* val) const {
        size_t left = 0;
        size_t right = N - 1;
        size_t i = N - 1 >> 1;
        for (;;) {
            const char* str = this->data[i];

            size_t j = ~(size_t)0;
            for (;;) {
                ++j;
                uint8_t carry;
                uint8_t c = val[j];
                if (__builtin_unpredictable(
                    __builtin_subcb(c, str[j], 0, &carry)
                )) {
                    if (__builtin_unpredictable(
                        carry
                    )) {
                        if (__builtin_expect(
                            left == i - 1
                            , false
                        )) {
                            goto end;
                        }
                        i = left + ((i - 1) - left >> 1);
                        right = i - 1;
                    } else {
                        if (__builtin_expect(
                            i + 1 == right
                            , false
                        )) {
                            goto end;
                        }
                        i = (i + 1) + (right - (i + 1) >> 1);
                        left = i + 1;
                    }
                    break;
                }
                if (__builtin_expect(
                    !c
                    , false
                )) {
                    return i;
                }
            }
        }
    end:
        __asm__("");
        return -1;
    }

    /*
    gnu_noinline
    constexpr ssize_t find(const char* val) const {
        size_t left = 0;
        size_t right = N - 1;

        size_t i = (N - 1) >> 1;
        for (;;) {
            const char* str = this->data[i];

            uint8_t c, carry;
            size_t j = ~(size_t)0;
            for (;;) {
                ++j;
                if (__builtin_subcb(
                    (c = val[j]),
                    str[j],
                    0,
                    &carry
                )) {
                    if (carry) {
                        if (left == i - 1) goto end;
                        i = left + (((i - 1) - left) >> 1);
                        right = i - 1;
                    } else {
                        if (i + 1 == right) goto end;
                        i = (i + 1) + ((right - (i + 1)) >> 1);
                        left = i + 1;
                    }
                    break;
                }
                if (!c) return i;
            }
        }
    end:
        return -1;
    }
    */


    //gnu_noinline
    //constexpr ssize_t find(const char* val) const {
    //    size_t left = 0;
    //    size_t right = N - 1;

    //    size_t i = (N - 1) >> 1;
    //    for(;;) {
    //        const char* str = this->data[i];
    //        
    //        uint8_t c, carry;
    //        size_t j = ~(size_t)0;
    //        for (;;) {
    //            ++j;
    //            if (__builtin_subcb(
    //                (c = val[j]),
    //                str[j],
    //                0,
    //                &carry
    //            )) {
    //                if (carry) {
    //                    /*
    //                    right = i - 1;
    //                    if (left == right) return -1;
    //                    i = left + ((right - left) >> 1);
    //                    */
    //                    if (left == i - 1) goto end;
    //                    i = left + (((i - 1) - left) >> 1);
    //                    right = i - 1;
    //                } else {
    //                    /*
    //                    left = i + 1;
    //                    if (left == right) return -1;
    //                    i = left + ((right - left) >> 1);
    //                    */
    //                    if (i + 1 == right) goto end;
    //                    i = (i + 1) + ((right - (i + 1)) >> 1);
    //                    left = i + 1;
    //                }
    //                break;
    //                /*
    //                if (carry) {
    //                    right = i - 1;
    //                } else {
    //                    left = i + 1;
    //                }
    //                if (left == right) {
    //                    return -1;
    //                }
    //                i = left + ((right - left) >> 1);
    //                break;
    //                */
    //            }
    //            if (!c) return i;
    //        }

    //        /*
    //        for (
    //            size_t j = 0;
    //            !__builtin_subcb(
    //                (c = ((const volatile uint8_t * restrict)val)[j]),
    //                ((const volatile uint8_t *restrict)str)[j],
    //                0,
    //                &carry
    //            );
    //            ++j
    //        ) {
    //            if (!c) return i;
    //        }
    //        */
    //        /*
    //        if (carry) {
    //            right = i - 1;
    //        } else {
    //            left = i + 1;
    //        }
    //        */

    //        /*
    //        if (carry) {
    //            --i;
    //            if (left == i) return -1;
    //            right = i;
    //            i = left + ((right - left) >> 1);
    //        } else {
    //            ++i;
    //            if (i == right) return -1;
    //            left = i;
    //            i = left + ((right - left) >> 1);
    //        }
    //        */


    //        /*
    //        if (!cmp_val) {
    //            return i;
    //        } else if (cmp_val < 0) {
    //            right = i - 1;
    //        } else {
    //            left = i + 1;
    //        }
    //        */
    //        /*
    //        if (left == right) {
    //            return -1;
    //        }
    //        i = left + ((right - left) >> 1);
    //        */
    //    }
    //    //while (left != right);
    //    //return -1;
    //end:
    //    return -1;
    //}
};

template<typename T, size_t N>
BSearchList(const T(&)[N])->BSearchList<T, N>;

#define STR(name, val) \
alignas(16) static inline constexpr char name[] = val

/*
STR(A_STR, "a");
STR(B_STR, "b");
STR(C_STR, "c");
STR(D_STR, "d");
*/

const auto pingas = BSearchList({ "a"_str, "b"_str, "c"_str, "d"_str });


int main(int argc, char* argv[]) {

    return pingas.find("c");


    if (argc <= 1) {
        error_exit(
            "Usage:\n"
            "zasm input_path [output_filename]\n"
        );
    }

    auto tokenize = [](char*& string) -> char* {
        char* string_start = string;
        char* string_read = string_start;

        for (;;) {
            switch (*string_read) {
                case '\0':
                    return NULL;
                case '|':
                    *string_read = '\0';
                    string = string_read + 1;
                    return string_start;
                default:
                    ++string_read;
            }
        }
    };


	return 0;
}