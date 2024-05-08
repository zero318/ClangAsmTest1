#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <numeric>
#include <bit>

template <typename P, typename O>
static inline P* pointer_raw_offset(P* pointer, O offset) {
	return (P*)(((intptr_t)pointer) + offset);
}

static bool needs_endian_swap = false;

template<typename T>
struct EndianValue {
    T value;
    
    inline T read() const {
        return !needs_endian_swap ? this->value : std::byteswap(this->value);
    }
    
    inline operator T() const {
        return this->read();
    }
};

struct PatchCommand {
    EndianValue<uint64_t> offset;
    EndianValue<uint64_t> count;
    uint8_t type; // 0 = insert, !0 = remove
    char8_t data[0];
};

struct PatchFile {
    uint8_t endianness; // 0 = little, !0 = big
    EndianValue<int64_t> total_size_diff;
    EndianValue<uint64_t> command_count;
    PatchCommand commands[1];
};

uint8_t* read_file_to_buffer(const char* path, size_t* size_out) {
    void* ret = NULL;
    if (FILE* file = fopen(path, "rb")) {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);
        if ((ret = malloc(size))) {
            fread(ret, size, 1, file);
            if (size_out) {
                *size_out = size;
            }
        }
        fclose(file);
    }
    return (uint8_t*)ret;
}

void write_buffer_to_file(const char* path, const void* buffer, size_t size) {
    if (FILE* file = fopen(path, "wb")) {
        fwrite(buffer, 1, size, file);
        fflush(file);
        fclose(file);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        return 1;
    }
    char* input_file_path = argv[1];
    char* patch_file_path = argv[2];
    char* output_file_path = argv[3];
    
    size_t input_file_size;
    size_t patch_file_size;
    
    const uint8_t* input_file = read_file_to_buffer(input_file_path, &input_file_size);
    const uint8_t* patch_file_raw = read_file_to_buffer(patch_file_path, &patch_file_size);
    
    const PatchFile* patch_file = (const PatchFile*)patch_file_raw;
    
    if constexpr (std::endian::native == std::endian::little) {
        needs_endian_swap = patch_file->endianness;
    } else {
        needs_endian_swap = !patch_file->endianness;
    }
    
    size_t output_file_size = input_file_size + patch_file->total_size_diff;
    
    uint8_t* output_buffer = (uint8_t*)malloc(output_file_size);
    if (!output_buffer) {
        return 1;
    }
    
    size_t input_offset = 0;
    size_t output_offset = 0;
    const PatchCommand* current_command = patch_file->commands;
    for (
        size_t i = patch_file->command_count;
        i--;
    ) {
        size_t current_offset = current_command->offset;
        if (size_t offset_diff = current_offset - output_offset) {
            memcpy(&output_buffer[current_offset], &input_file[input_offset], offset_diff);
            current_offset += offset_diff;
            input_offset += offset_diff;
        }
        if (current_command->type == 0) {
            size_t character_count = current_command->count;
            memcpy(&output_buffer[current_offset], current_command->data, character_count);
            current_command = pointer_raw_offset(current_command, character_count);
        } else {
            input_offset += current_command->count;
        }
        ++current_command;
    }

    write_buffer_to_file(output_file_path, output_buffer, output_file_size);
    
    return 0;
}