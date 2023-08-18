
#include <stdlib.h>
#include <stdint.h>

#include "zero/util.h"

// 2 bits
enum x86BitMode : uint8_t {
    SixteenBit   = 0b00,
    ThirtyTwoBit = 0b01,
    SixtyFourBit = 0b10,
};

// 2 bits
enum x86RingLevel : uint8_t {
    Ring0 = 0b00,
    Ring1 = 0b01,
    Ring2 = 0b10,
    Ring3 = 0b11,
};

// 3 bits
enum x86OperatingMode : uint8_t {
    RealMode          = 0b000,
    v86Mode           = 0b001,
    ProtectedMode     = 0b010,
    CompatibilityMode = 0b110,
    LongMode          = 0b100,
};

// 3 bits
enum x86OpcodeEncoding : uint8_t {
    Legacy   = 0b000, // Maps 0-3
    APX      = 0b001, // Maps 0-1
    VEX2     = 0b010, // Map 1
    VEX3     = 0b011, // Maps 0-31
    XOP      = 0b100, // Maps 8-31
    EVEX     = 0b101, // Maps 0-7
    MVEX     = 0b110, // Maps 0-15
    Larrabee = 0b111, // Why
};

// 5 bits
enum x86OpcodeMap : uint8_t {
    LegacyMap0 = 0x0, // One byte opcodes (and weird VEX jank for 1st gen Xeon Phi)
    LegacyMap1 = 0x1, // Two byte opcodes 0x0F
    LegacyMap2 = 0x2, // Three byte opcodes 0x0F38
    LegacyMap3 = 0x3, // Three byte opcodes 0x0F3A
    APXMap4    = 0x4, // APX Legacy EVEX map
    HalfMap5   = 0x5, // Cursed half precision map A
    HalfMap6   = 0x6, // Cursed half precision map B
    XopMap8    = 0x8, // Xop map imm8
    XopMap9    = 0x9, // Xop map
    XopMap10   = 0xA, // XOP map imm32
    // Would it ever make sense to handle groups as fake maps?
};

union x86OpcodeIndex {
    uint32_t raw;
    struct {
        uint8_t opcode;
        uint8_t encoding : 3;
        uint8_t mode : 3;
        uint8_t bits : 2;
        uint8_t map : 5;
    };
};