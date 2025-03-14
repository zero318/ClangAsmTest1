#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "8086_cpu.h"

#include "..\bee_movie_script.h"

#define REALSEG(seg) ((seg)>>4)
#define BYTES16(value) (uint8_t)((value) & 0xFF), (uint8_t)((value) >> 8 & 0xFF)
#define FADDR16_BYTES(seg, addr) BYTES16(addr), BYTES16(seg)

#define ZBIOS_BASE 0x50000
#define ZBIOS_SEGMENT REALSEG(ZBIOS_BASE)
#define CALL_PUT_CHAR 0x9A, FADDR16_BYTES(ZBIOS_SEGMENT, 0x0000)
#define CALL_PUT_STRN 0x9A, FADDR16_BYTES(ZBIOS_SEGMENT, 0x0005)
#define CALL_GET_CHAR 0x9A, FADDR16_BYTES(ZBIOS_SEGMENT, 0x0018)

static inline constexpr uint8_t zbios[] = {
    // void put_char(char c)
    // Args:
    // c: AL
    // Clobbers:
    // DX
    0xBA, 0x38, 0x7C,	// MOV DX, 31800
    0xEE,				// OUT DX, AL
// skip_put_strn:
    0xCB,				// RETF
    // void put_strn(const char* str, uint16_t len)
    // Args:
    // str: DS: [SI]
    // len: CX
    // Clobbers:
    // AL, DX, SI
    0xE3, 0xFD,			// JCXZ skip_put_strn
    0xBA, 0x38, 0x7C,	// MOV DX, 31800
    0x30, 0xC0,			// XOR AL, AL
    0xD0, 0xF0,			// SETMO AL, 1
    0x78, 0x03,			// JS put_strn8086
    0xF3, 0x6E,			// REP OUTSB
    0xCB,				// RETF
// put_strn8086:
    0xAC,				// LODSB
    0xEE,				// OUT DX, AL
    0xE2, 0xFC,			// LOOP put_strn8086
    0xCB,				// RETF
    // char get_char()
    // Clobbers:
    // DX
    0xBA, 0x38, 0x7C,	// MOV DX, 31800
    0xEC,				// IN AL, DX
    0xCB				// RETF
};

static inline constexpr uint8_t zbios_test[] = {
    CALL_GET_CHAR,
    0x3C, 'b',
    0x74, 0x0D,
    0xB9, 0x0A, 0x00,
    0xBE, 0x00, 0x50,
    CALL_PUT_STRN,
    0xEB, 0xFE,
    0xB8, BYTES16(REALSEG(0x6000)),
    0x8E, 0xD8,
    0xB9, BYTES16(sizeof(BEE_MOVIE_SCRIPT) - 1),
    0x31, 0xF6,
    CALL_PUT_STRN,
    0xEB, 0xFE,
};

// Assuming init/reset state:
// Real mode, all registers 0, interrupts disabled
// Memory may not be 0
// Sets the IDT to 0 (any interrupt will triple fault)
// Sets up this GDT at 0x10000:
// 0000 = NULL descriptor (LGDT buffer)
// 0008 = 16 bit TSS at base 0xF000
// 0010 = 16 bit ring 0 code segment at base 0x0000
// 0018 = ring 0 data segment at base 0x0000
// 0020 = 16 bit ring 3 code segment at base 0x1000
// 0028 = ring 3 data segment at base 0x1000
// 0030 = 32 bit ring 3 code segment at base 0x00002000
// 0038 = ring 3 data segment at base 0x00002000
static inline constexpr uint8_t protected_mode_testA[] = {
    0xBE, 0x00, 0x10,               // MOV SI, 0x1000
    0x8E, 0xDE,                     // MOV DS, SI
    0x8E, 0xC6,                     // MOV ES, SI
    // Memset
    0xB1, 0x1F,                     // MOV CL, 0x20
    0xF3, 0xAB,                     // REP STOSW
    // Fake IDT
    0x0F, 0x01, 0x0D,               // SIDT [DI]
    0xAB,                           // STOSW
    0x0F, 0x01, 0x5D, 0xFE,         // LIDT [DI-0x02]
    0x4A,                           // DEC DX
    0xB1, 0x08,                     // MOV CL, 8
// limit_loop:
    0x83, 0xEF, 0x08,               // SUB DI, 8
    0x89, 0x15,                     // MOV WORD [DI], DX
    0xE2, 0xF9,                     // LOOP limit_loop
    // GDT Base (0x10000)
    0x41,                           // INC CX
    0x88, 0x4D, 0x04,               // MOV BYTE [DI+0x04], CL
    0x41,                           // INC CX
    // TSS 16 Base (0xF000)
    0xB7, 0xF0,                     // MOV BH, 0xF0
    0x88, 0x7D, 0x0A,               // MOV BYTE [DI+0x0A], BH
    0xC6, 0x45, 0x0D, 0x81,         // MOV BYTE [DI+0x0D], 0x81
    // Ring 0 Code 16 (0x0)
    0xC6, 0x45, 0x15, 0x9A,         // MOV BYTE [DI+0x15], 0x9A
    // Ring 0 Data 16 (0x0)
    0xC6, 0x45, 0x1D, 0x92,         // MOV BYTE [DI+0x1D], 0x92
    // Ring 3
    0xB0, 0xFA,                     // MOV AL, 0xFA
    0x89, 0xF5,                     // MOV BP, SI
// ring3_loop:
    // Ring 3 Code
    0x89, 0x6D, 0x22,               // MOV WORD [DI+0x22], BP
    0x89, 0x45, 0x25,               // MOV WORD [DI+0x25], AX
    // Ring 3 Data
    0x89, 0x6D, 0x2A,               // MOV WORD [DI+0x2A], BP
    0x24, 0xF7,                     // AND AL, 0xF7
    0x89, 0x45, 0x2D,               // MOV WORD [DI+0x2D], AX
    0x0D, 0x08, 0xCF,               // OR AX, 0xCF08
    0xD1, 0xE5,                     // SHL BP, 1
    0x83, 0xC7, 0x10,               // ADD DI, 0x10
    0xE2, 0xE8,                     // LOOP ring3_loop
    // Load GDT
    0x0F, 0x01, 0x55, 0xF0,         // LGDT [DI-0x10]
    0x0F, 0x01, 0xE0,               // SMSW AX
    0x40,                           // INC AX
    0x0F, 0x01, 0xF0,               // LMSW AX
    0xEA, 0x00, 0x50, 0x10, 0x00    // JMP FAR 0x0010:0x5000
};

static inline constexpr uint8_t protected_mode_testB[] = {
    //0xE6, 0x69, // nice
    //0xEB, 0xFC
    //0xEB, 0xFE
    0x8C, 0xC8,                         // MOV AX, CS
    0x83, 0xC0, 0x08,                   // ADD AX, 8
    0x8E, 0xD8,                         // MOV DS, AX
    0x8E, 0xC0,                         // MOV ES, AX
    0x8E, 0xD0,                         // MOV SS, AX
    0x66, 0xBC, 0x00, 0x00, 0x02, 0x00, // MOV ESP, 0x20000
    0xC8, 0x20, 0x00, 0x00,             // ENTER 0x20, 0
    0x50,                               // PUSH AX
    0x6A, 0x01,                         // PUSH 1
    0x66, 0x5A,                         // POP EDX
    0xC9,                               // LEAVE
    0x66, 0xC8, 0xFE, 0xFF, 0x00,       // ENTER 0x20, 0
    0x58,                               // POP AX
    0x58,                               // POP AX
    0x66, 0x52,                         // PUSH EDX
    0x66, 0xC9,                         // LEAVE
    0xCC,                               // INT3
    0xEB, 0xFE
};

static inline constexpr uint8_t protected_mode_test_jump[] = {
    0xBC, 0x00, 0x10,               // MOV SP, 0x1000
    0xEA, 0x00, 0x00, 0x00, 0x04    // JMP FAR 0x4000:0x0000
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

struct DebugByteDevice : PortByteDevice {
    virtual bool in_byte(uint8_t& value, uint32_t port) {
        switch (port) {
            case 31800:
                value = getchar();
                return true;
        }
        return false;
    }
    virtual bool out_byte(uint32_t port, uint8_t value) {
        switch (port) {
            case 31800:
                putchar(value);
                return true;
            case 31801:
                //switch (value) {
                    //case 0:
                //}
        }
        return false;
    }
};
struct DebugWordDevice : PortWordDevice {
    virtual bool in_word(uint16_t& value, uint32_t port) {
        return false;
    }
    virtual bool out_word(uint32_t port, uint16_t value) {
        return false;
    }
};

DumbBS startup_device;
DebugByteDevice debug_byte_device;

int main(int argc, char* argv[]) {
    //load_bios_rom();

    z86_mem_write(ZBIOS_BASE, zbios);
    z86_mem_write(0xFFFF0, protected_mode_test_jump);
    //z86_mem_write(0x4000, zbios_test);
    z86_mem_write(0x5000, "yo what up");
    //z86_mem_write(0x6000, BEE_MOVIE_SCRIPT);
    static constexpr uint8_t split_float_test[] = { 0x00, 0x80, 0x3F };
    z86_mem_write(0x0000, split_float_test);
    z86_mem_write(0x04000, protected_mode_testA);
    z86_mem_write(0x05000, protected_mode_testB);

    z86_add_byte_device(&startup_device);
    z86_add_byte_device(&debug_byte_device);

    z86_execute();

    return 0;
}