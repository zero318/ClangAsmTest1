#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include <utility>
#include <tuple>

#include <immintrin.h>

typedef __int128_t int128_t;
typedef __uint128_t uint128_t;

#if __has_builtin(__builtin_align_down)
#define AlignDownToMultipleOf2(val, mul) (__builtin_align_down((val),(mul)))
#else
#define AlignDownToMultipleOf2(val, mul) ((val)&-(mul))
#endif
#if __has_builtin(__builtin_align_up)
#define AlignUpToMultipleOf2(val, mul) (__builtin_align_up((val),(mul)))
#else
#define AlignUpToMultipleOf2(val, mul) AlignDownToMultipleOf2((val)+(mul)-1,(mul))
#endif

#define bitsof(type) \
(sizeof(type) * CHAR_BIT)

template<typename T>
static inline constexpr T rotl(T x, size_t n) {
    return x << n | x >> bitsof(T) - n;
}
template<typename T>
static inline constexpr T rotr(T x, size_t n) {
    return x >> n | x << bitsof(T) - n;
}

template <typename T = void>
[[nodiscard]] inline auto read_file_to_aligned_buffer(const char* path, size_t align) {
    long file_size = 0;
    long aligned_size = 0;
    T* buffer = NULL;
    if (FILE* file = fopen(path, "rb")) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        if (file_size != -1l) {
            aligned_size = AlignUpToMultipleOf2(file_size, align);
            if ((buffer = (T*)_mm_malloc(aligned_size, align))) {
                long diff = aligned_size - file_size;
                if (diff > 0) {
                    memset(&((unsigned char*)buffer)[file_size], 0, diff);
                }
                rewind(file);
                fread(buffer, file_size, 1, file);
            }
        }
        fclose(file);
    }
    return std::make_tuple(buffer, (unsigned long)file_size, (unsigned long)aligned_size);
}

struct EncryptHeader {
    char magic[4];
    uint16_t version;
    uint8_t alignment;
    uint8_t buffer_extra;
    uint64_t file_size;
};
static_assert(sizeof(EncryptHeader) == 16);

static const unsigned char start_mask[16] = {
    'L', 'e', 'S', 'a', 'n', 'a', 'e', '\0',
    0xA4, 0x55, // 42069
    0x01, 0x3E, // 318
    'y', 'e', 'e', 't'
};

void encrypt_file(const char* in_path, const char* out_path) {
    
    FILE* out = fopen(out_path, "wb");
    if (out) {
        auto [buffer, size, buffer_size] = read_file_to_aligned_buffer<uint8_t>(in_path, 16);
        if (buffer) {
            
            uint8_t buffer_extra = buffer_size - size;
            
            EncryptHeader header = {
                .magic = { 'Z', 'E', 'N', 'C' },
                .version = 1,
                .alignment = 16,
                .buffer_extra = buffer_extra,
                .file_size = size
            };
            
            fwrite(&header, sizeof(EncryptHeader), 1, out);
            
            uint128_t mask_init = *(uint128_t*)&start_mask;
            uint128_t xor_mask_init = rotl(mask_init, (buffer_extra + 1) * 6);
            uint128_t xor_accel_init = rotr(mask_init, (buffer_extra + 1) * 7);

            __m128i xor_mask = _mm_loadu_si128((__m128i*)&xor_mask_init);
            __m128i xor_accel = _mm_loadu_si128((__m128i*)&xor_accel_init);

            size_t iters = buffer_size >> 4;
            __m128i* buffer_data = (__m128i*)buffer;
            for (
                size_t i = 0;
                i != iters;
                ++i, ++buffer_data
            ) {
                __m128i data = _mm_load_si128(buffer_data);
                data = _mm_xor_si128(data, xor_mask);
                fwrite(&data, sizeof(__m128i), 1, out);

                xor_mask = _mm_add_epi8(xor_mask, xor_accel);
                xor_mask = _mm_sub_epi16(xor_mask, xor_accel);
                xor_mask = _mm_add_epi32(xor_mask, xor_accel);
            }
            
            _mm_free(buffer);
        }
        fclose(out);
    }
}

template <typename T = void>
[[nodiscard]] auto decrypt_file_to_buffer(const char* path) {
    EncryptHeader header;
    header.file_size = 0;
    uint8_t* buffer = NULL;
    if (FILE* file = fopen(path, "rb")) {
        fread(&header, sizeof(EncryptHeader), 1, file);
        if (
            (header.magic[0] == 'Z' & header.magic[1] == 'E' & header.magic[2] == 'N' & header.magic[3] == 'C') &&
            header.version == 1 &&
            header.alignment >= 16 &&
            !(header.alignment & header.alignment - 1)
        ) {
            size_t buffer_size = header.buffer_extra + header.file_size;
            if (
                !(buffer_size & header.alignment - 1) &&
                (buffer = (uint8_t*)_mm_malloc(buffer_size, header.alignment))
            ) {
                fread(buffer, buffer_size, 1, file);
                    
                uint128_t mask_init = *(uint128_t*)&start_mask;
                uint128_t xor_mask_init = rotl(mask_init, (header.buffer_extra + 1) * 6);
                uint128_t xor_accel_init = rotr(mask_init, (header.buffer_extra + 1) * 7);

                __m128i xor_mask = _mm_loadu_si128((__m128i*)&xor_mask_init);
                __m128i xor_accel = _mm_loadu_si128((__m128i*)&xor_accel_init);

                size_t iters = buffer_size >> 4;
                __m128i* buffer_data = (__m128i*)buffer;
                for (
                    size_t i = 0;
                    i != iters;
                    ++i, ++buffer_data
                ) {
                    __m128i data = _mm_load_si128(buffer_data);
                    data = _mm_xor_si128(data, xor_mask);
                    _mm_store_si128(buffer_data, data);

                    xor_mask = _mm_add_epi8(xor_mask, xor_accel);
                    xor_mask = _mm_sub_epi16(xor_mask, xor_accel);
                    xor_mask = _mm_add_epi32(xor_mask, xor_accel);
                }
            }
        }
        fclose(file);
    }
    return std::make_pair((T*)buffer, header.file_size);
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        switch (*argv[1]) {
            case 'e':
                if (argc > 3) {
                    encrypt_file(argv[2], argv[3]);
                }
                else {
                    size_t length = strlen(argv[2]);
                    char* out_path = (char*)malloc(length + 5);
                    memcpy(out_path, argv[2], length);
                    memcpy(&out_path[length], ".zenc", sizeof(".zenc"));
                    encrypt_file(argv[2], out_path);
                    free(out_path);
                }
                break;
            case 'd':
                if (argc > 3) {
                    auto [buffer, file_size] = decrypt_file_to_buffer(argv[2]);
                    if (buffer) {
                        if (FILE* out = fopen(argv[3], "wb")) {
                            fwrite(buffer, file_size, 1, out);
                            fclose(out);
                        }
                        _mm_free(buffer);
                    }
                }
                break;
        }
    }
    return 0;
}