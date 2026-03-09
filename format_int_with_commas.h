#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <array>
#include <limits>
#include <type_traits>
#include <bit>

#define bitsof(type) (sizeof(type) * CHAR_BIT)

template<typename T>
struct FormatIntWithCommas {
    static constexpr size_t DIGITS_MAX = std::numeric_limits<T>::digits10 + 1;
    static constexpr size_t COMMAS_MAX = DIGITS_MAX / 3;
    static constexpr size_t BUFFER_SIZE = DIGITS_MAX + COMMAS_MAX + 1 + std::is_signed_v<T>;
    using U = std::make_unsigned_t<T>;
    static constexpr auto POW10_TABLE = [] consteval {
        std::array<U, DIGITS_MAX> table = {};
        for (U i = 1; i <= DIGITS_MAX; ++i) table[i - 1] = i * 10u;
        return table;
    }();

    char buffer[BUFFER_SIZE];

    inline constexpr const char* print(T value) {
        bool negative = false;
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                negative = true;
                value = -value;
                this->buffer[0] = '-';
            }
        }
        U valueu = value;

        // Fast integer log10+1 to get the number of digits
        uint32_t guess = ((bitsof(U) - 1) - std::countl_zero<U>(valueu | 1)) * 77u >> 8;
        uint32_t digit_count = guess + (value >= POW10_TABLE[guess]);

        uint32_t commas = 0;
        if constexpr (COMMAS_MAX) {
            if (digit_count) commas = (digit_count - 1) / 3;
        }
        uint32_t write_offset = digit_count + commas + negative;
        this->buffer[write_offset--] = '\0';
        if constexpr (COMMAS_MAX) {
            while (commas--) {
                if consteval {
                    uint8_t digit = valueu % 10u; valueu /= 10u;
                    this->buffer[write_offset--] = '0' | digit;
                    digit = valueu % 10u; valueu /= 10u;
                    this->buffer[write_offset--] = '0' | digit;
                    digit = valueu % 10u; valueu /= 10u;
                    this->buffer[write_offset--] = '0' | digit;
                    this->buffer[write_offset--] = ',';
                } else {
                    uint8_t digit = valueu % 10u; valueu /= 10u;
                    uint32_t digits = digit << 24;
                    digit = valueu % 10u; valueu /= 10u;
                    digits |= digit << 16;
                    digit = valueu % 10u; valueu /= 10u;
                    digits |= digit << 8;
                    *(uint32_t*)&this->buffer[write_offset - 3] = digits | '000,'; // 0x3030302C if you don't like the multichar literal
                    write_offset -= 4;
                }
            }
        }
        do {
            uint8_t digit = valueu % 10u; valueu /= 10u;
            this->buffer[write_offset] = '0' | digit;
        } while (write_offset-- != negative);
        return this->buffer;
    }
    inline constexpr FormatIntWithCommas() = default;
    inline constexpr FormatIntWithCommas(T value) {
        if consteval {
            for (size_t i = 0; i != BUFFER_SIZE; ++i) this->buffer[i] = '\0';
        }
        this->print(value);
    }
    inline constexpr operator const char*() const {
        return this->buffer;
    }
};

#define FormatIntWithCommas(number) FormatIntWithCommas(number).buffer