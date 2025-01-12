#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "8086_cpu.h"

static inline constexpr uint8_t protected_mode_testA[] = {
	0x48,
	0x40,
	0x66, 0x48,
	0xBB, 0x00, 0x10,
	0x89, 0x07,
	0x89, 0x5F, 0x02,
	0x89, 0x6F, 0x04,
	0xBF, 0x00, 0x20,
	0x89, 0x47, 0x08,
	0x89, 0x7F, 0x0A,
	0xC7, 0x47, 0x0C, 0x00, 0x81,
	0xBE, 0x00, 0x50,
	0x89, 0x47, 0x10,
	0x89, 0x77, 0x12,
	0xC7, 0x47, 0x14, 0x00, 0x9A,
	0x89, 0x47, 0x18,
	0x89, 0x77, 0x1A,
	0xC7, 0x47, 0x1C, 0x00, 0x92,
	0xBE, 0x00, 0x60,
	0x89, 0x47, 0x20,
	0x89, 0x77, 0x22,
	0xC7, 0x47, 0x24, 0x00, 0xFA,
	0x89, 0x47, 0x28,
	0x89, 0x77, 0x2A,
	0xC7, 0x47, 0x2C, 0x00, 0xF2,
	0x0F, 0x01, 0x17,
	0x0F, 0x01, 0xE0,
	0x83, 0xC8, 0x01,
	0x0F, 0x01, 0xF0,
	0x90,
	0xEA, 0x00, 0x00, 0x10, 0x00
};

static inline constexpr uint8_t protected_mode_testB[] = {
	0xE6, 0x69, // nice
	0xEB, 0xFC
};

static inline constexpr uint8_t protected_mode_test_jump[] = {
	0xEA, 0x00, 0x00, 0x00, 0x04
};

size_t file_size(FILE* file) {
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	rewind(file);
	return size;
}

void load_bios_rom() {
	//if (FILE* bios = fopen("F:\\MyCodeLibs\\_BIOS.ROM", "rb")) {
	if (FILE* bios = fopen("F:\\MyCodeLibs\\NEC PC-9801 (unknown model) [ROM]\\bios.rom", "rb")) {
		z86_file_read(0xE8000, bios, file_size(bios));
		fclose(bios);
	}
}

void reload_bios_rom() {
	//if (FILE* bios = fopen("F:\\MyCodeLibs\\_BIOS.ROM", "rb")) {
	if (FILE* bios = fopen("F:\\MyCodeLibs\\NEC PC-9801 (unknown model) [ROM]\\bios.rom", "rb")) {
		size_t size = file_size(bios) - 0x10000;
		fseek(bios, 0x10000, SEEK_SET);
		z86_file_read(0xF8000, bios, size);
		fclose(bios);
	}
}

void load_itf_rom() {
	if (FILE* itf_rom = fopen("F:\\MyCodeLibs\\NEC PC-9801 (unknown model) [ROM]\\itf.rom", "rb")) {
		z86_file_read(0xF8000, itf_rom, file_size(itf_rom));
		fclose(itf_rom);
	}
}

// F:\MyCodeLibs\NEC PC-9801 (unknown model) [ROM]
struct DumbBS : PortByteDevice {
	virtual bool in_byte(uint8_t& value, uint32_t port) {
		switch (port) {
			case 0x35: // One of the system ports
				value = 0x80;
				return true;
			case 0x439:  // DMA related
				value = 4;
				return true;
		}
		return false;
	}
	virtual bool out_byte(uint32_t port, uint8_t value) {
		switch (port) {
			case 0x43D: // Some sort of bank switching port
				switch (value) {
					default:
						return false;
					case 0x10:
						load_itf_rom();
						break;
					case 0x12:
						reload_bios_rom();
						break;
				}
				return true;
		}
		return false;
	}
};

DumbBS startup_device;

int main(int argc, char* argv[]) {
	//load_bios_rom();

	z86_mem_write(0xFFFF0, protected_mode_test_jump);
	z86_mem_write(0x04000, protected_mode_testA);
	z86_mem_write(0x05000, protected_mode_testB);

	z86_add_byte_device(&startup_device);

	z86_execute();

	return 0;
}