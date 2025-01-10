#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "8086_cpu.h"

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
	load_bios_rom();

	z86_add_byte_device(&startup_device);

	z86_execute();

	return 0;
}