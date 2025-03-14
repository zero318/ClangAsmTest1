#include <conio.h>
#include <Windows.h>

#include "zero/util.h"


//#define CALL_PUT_CHAR "CALL QWORD PTR [RIP+put_char_ptr] \n"
#define CALL_PUT_CHAR \
        "MOV EAX, R15D \n" \
        "CALL RAX \n"

//#define PUT_CHAR_ADDR(regq, regd) "MOV " MACRO_STR(regq) ", QWORD PTR [RIP+put_char_ptr] \n"
#define PUT_CHAR_ADDR(regq, regd) "MOV " MACRO_STR(regd) ", R15D \n"

//#define CALL_SET_CURSOR "CALL QWORD PTR [RIP+set_cursor_ptr] \n"
#define CALL_SET_CURSOR \
        "MOV EAX, R13D \n" \
        "CALL RAX \n"
//#define SET_CURSOR_ADDR(regq, regd) "MOV " MACRO_STR(regq) ", QWORD PTR [RIP+set_cursor_ptr] \n"
#define SET_CURSOR_ADDR(regq, regd) "MOV " MACRO_STR(regd) ", R13D \n"

//#define CALL_KBHIT "CALL QWORD PTR [RIP+kbhit_ptr] \n"
#define CALL_KBHIT \
        "MOV RAX, R13 \n" \
        "SHR RAX, 32 \n" \
        "CALL RAX"
//#define KBHIT_ADDR(regq, regd) "MOV " MACRO_STR(regq) ", QWORD PTR [RIP+kbhit_ptr] \n"
#define KBHIT_ADDR(regq, regd) \
        "MOV " MACRO_STR(regq) ", R13 \n" \
        "SHR " MACRO_STR(regq) ", 32 \n"

//#define CALL_GETCH "CALL QWORD PTR [RIP+getch_ptr] \n"
#define CALL_GETCH \
        "MOV RAX, R15 \n" \
        "SHR RAX, 32 \n" \
        "CALL RAX \n"
//#define GETCH_ADDR(regq, regd) "MOV " MACRO_STR(regq) ", QWORD_PTR [RIP+getch_ptr] \n"
#define GETCH_ADDR(regq, regd) \
        "MOV " MACRO_STR(regq) ", R15 \n" \
        "SHR " MACRO_STR(regq) ", 32 \n"
        

#define PRINT_ECX \
        CALL_PUT_CHAR

#define PRINT_ECX_RBP \
        "CALL RBP \n"

#define PRINT_LINE \
        "MOV ECX, '\n' \n" \
        PRINT_ECX

#define PRINT_LINE_RBP \
        "MOV ECX, '\n' \n" \
        PRINT_ECX_RBP

//#define CLEAR_SCREEN \
        "MOV EDI, 300 \n" \
    "0: \n" \
        PRINT_LINE \
        "DEC EDI \n" \
        "JNZ 0b \n"

#define RESET_CURSOR \
        "MOV RCX, R12 \n" \
        "XOR EDX, EDX \n" \
        CALL_SET_CURSOR
#define RESET_CURSOR_RBP \
        "MOV RCX, R12 \n" \
        "XOR EDX, EDX \n" \
        "CALL RBP \n"

#define SET_CURSOR_TO_EBP \
        "MOV ECX, 0xAAAAAAAB \n" \
        "IMUL RCX, RBP \n" \
        "SHR RCX, 33 \n" \
        "IMUL EAX, ECX, 0xFFFD \n" \
        "LEA EDX, [RBP+RAX+0x10000] \n" \
        "MOV RCX, R12 \n" \
        CALL_SET_CURSOR
#define SET_CURSOR_TO_EBP_BIG \
        "MOV ECX, 0xAAAAAAAB \n" \
        "IMUL RCX, RBP \n" \
        "SHR RCX, 33 \n" \
        "IMUL EAX, ECX, 0x1FFFA \n" \
        "LEA EDX, [RBP*2+RAX+0x10000] \n" \
        "MOV RCX, R12 \n" \
        CALL_SET_CURSOR
        
#define PRINT_XMM7(length) \
        "MOV EDI, " MACRO_STR(length) "\n" \
    "0: \n" \
        "VMOVD ECX, XMM7 \n" \
        CALL_PUT_CHAR \
        "VPSRLDQ XMM7, XMM7, 1 \n" \
        "DEC EDI \n" \
        "JNZ 0b \n"
#define PRINT_EBX(length) \
        "MOV EDI, " MACRO_STR(length) "\n" \
    "0: \n" \
        "MOVZX ECX, BL \n" \
        CALL_PUT_CHAR \
        "SHR EBX, 8 \n" \
        "DEC EDI \n" \
        "JNZ 0b \n"
#define PRINT_RBX(length) \
        "MOV EDI, " MACRO_STR(length) " \n" \
    "0: \n" \
        "MOVZX ECX, BL \n" \
        CALL_PUT_CHAR \
        "SHR RBX, 8 \n" \
        "DEC EDI \n" \
        "JNZ 0b \n"
#define PRINT_BLANKS(length) \
        "MOV EDI, " MACRO_STR(length) " \n" \
    "0: \n" \
        "XOR ECX, ECX \n" \
        CALL_PUT_CHAR \
        "DEC EDI \n" \
        "JNZ 0b \n"

#define PRINT_XMM7_RBP(length) \
        "MOV EDI, " MACRO_STR(length) "\n" \
    "0: \n" \
        "VMOVD ECX, XMM7 \n" \
        "CALL RBP \n" \
        "VPSRLDQ XMM7, XMM7, 1 \n" \
        "DEC EDI \n" \
        "JNZ 0b \n"
#define PRINT_EBX_RBP(length) \
        "MOV EDI, " MACRO_STR(length) "\n" \
    "0: \n" \
        "MOVZX ECX, BL \n" \
        "CALL RBP \n" \
        "SHR EBX, 8 \n" \
        "DEC EDI \n" \
        "JNZ 0b \n"
#define PRINT_RBX_RBP(length) \
        "MOV EDI, " MACRO_STR(length) " \n" \
    "0: \n" \
        "MOVZX ECX, BL \n" \
        "CALL RBP \n" \
        "SHR RBX, 8 \n" \
        "DEC EDI \n" \
        "JNZ 0b \n"
#define PRINT_BLANKS_RBP(length) \
        "MOV EDI, " MACRO_STR(length) " \n" \
    "0: \n" \
        "XOR ECX, ECX \n" \
        "CALL RBP \n" \
        "DEC EDI \n" \
        "JNZ 0b \n"


// Outputs:
// EBX = character
#define GET_KEYW \
        KBHIT_ADDR(RBX, EBX) \
    "0: \n" \
        "CALL RBX \n" \
        "TEST AL, AL \n" \
        "JZ 0b \n" \
        GETCH_ADDR(RDI, EDI) \
        "CALL RDI \n" \
        "XCHG RAX, RBX \n" \
        "CALL RAX \n" \
        "TEST AL, AL \n" \
        "JZ 0f \n" \
        "CALL RDI \n" \
        "MOV BH, AL \n" \
    "0: \n"

#define GET_KEYA(regq, regd) \
        KBHIT_ADDR(RDI, EDI) \
        GETCH_ADDR(regq, regd) \
    "0: \n" \
        "CALL RDI \n" \
        "TEST AL, AL \n" \
        "JZ 0b \n" \
        "CALL " MACRO_STR(regq) "\n" \
        "MOVZX EBX, AL \n" \
        "CALL RDI \n" \
        "TEST AL, AL \n" \
        "JNZ 0b \n"

// Right = 4DE0
// Left = 4BE0
// Down = 50E0
// Up = 48E0

#define LEFT_KEY 0x4BE0
#define UP_KEY 0x48E0
#define RIGHT_KEY 0x4DE0
#define DOWN_KEY 0x50E0

#define PACK_BYTES16(b0, b1) \
MACRO_STR(b1) "<<8|" MACRO_STR(b0)
#define PACK_WORDS32(w0, w1) \
MACRO_STR(w1) "<<16|" MACRO_STR(w0)
#define PACK_BYTES32(b0, b1, b2, b3) \
MACRO_STR(b3) "<<24|" MACRO_STR(b2) "<<16|" MACRO_STR(b1) "<<8|" MACRO_STR(b0)
#define PACK_DWORDS64(d0, d1) \
MACRO_STR(d1) "<<32|" MACRO_STR(d0)
#define PACK_WORDS64(w0, w1, w2, w3) \
MACRO_STR(w3) "<<48|" MACRO_STR(w2) "<<32|" MACRO_STR(w1) "<<16|" MACRO_STR(w0)
#define PACK_BYTES64(b0, b1, b2, b3, b4, b5, b6, b7) \
MACRO_STR(b7) "<<56|" MACRO_STR(b6) "<<48|" MACRO_STR(b5) "<<40|" MACRO_STR(b4) "<<32|" MACRO_STR(b3) "<<24|" MACRO_STR(b2) "<<16|" MACRO_STR(b1) "<<8|" MACRO_STR(b0)

#define SET_CURSOR(x, y) \
        "MOV RCX, R12 \n" \
        "MOV EDX," PACK_WORDS32(x, y) "\n" \
        CALL_SET_CURSOR

#define SET_CURSOR_RBP(x, y) \
        "MOV RCX, R12 \n" \
        "MOV EDX," PACK_WORDS32(x, y) "\n" \
        "CALL RBP \n"

#define P(...) "%{" MACRO_STRV(__VA_ARGS__) "%}"

#define PACK_R13_DWORDS(d0, d1) \
        ".byte 0x49, 0xBD \n" \
        ".int " MACRO_STR(d0) "\n" \
        ".int " MACRO_STR(d1) "\n"

#define PACK_R15_DWORDS(d0, d1) \
        ".byte 0x49, 0xBF \n" \
        ".int " MACRO_STR(d0) "\n" \
        ".int " MACRO_STR(d1) "\n"

/*
extern "C" {
    auto* kbhit_ptr asm("kbhit_ptr") = _kbhit;
    auto* getch_ptr asm("getch_ptr") = _getch;
    auto* put_char_ptr asm("put_char_ptr") = putchar;
    auto* get_std_handle_ptr asm("get_std_handle_ptr") = GetStdHandle;
    auto* set_cursor_ptr asm("set_cursor_ptr") = SetConsoleCursorPosition;
}
*/

#define INCLUDE_AI_MODE 1

preserve_none void tic_tac_bs() {
    __asm__ volatile (
        /*
            RBX   = scratch register
            RBP   = cursor position
            RSI   = player markers, turn counter, mode flag
            RDI	  = scratch register, loop counter
            R12   = console handle
            R13   = func pointers
            R14   = player number text
            R15   = func pointers
            XMM6  = board state
            XMM7  = scratch register
            XMM8  = win permutations A
            XMM9  = win permutations B
            XMM10 = input key lookup
            XMM11 = board print permutations A
            XMM12 = board print permutations B
            XMM13 = stupid index array
            XMM14 = player 2 win check
            XMM15 = player 1 win check
        */
        ".intel_syntax \n"
        ".code64 \n"
        "SUB RSP, 0x28 \n"
        "MOV ECX, %c[stdout_id] \n"
        "MOV EAX, OFFSET GetStdHandle \n"
        "CALL RAX \n"
        "MOV R12, RAX \n"
        /// Prep constants
        // function pointers
        PACK_R13_DWORDS(SetConsoleCursorPosition, _kbhit)
        PACK_R15_DWORDS(putchar, _getch)
        // player win checks
        "MOV RAX," PACK_BYTES64('X', 'X', 'X', 0, 'O', 'O', 'O', 0) "\n"
        "VMOVQ XMM15, RAX \n"
        "VPSHUFD XMM14, XMM15, 0x55 \n"
#if __AVX512F__ && __AVX512VL__
        "VPBROADCASTD XMM15, XMM15 \n"
#else
        "VPSHUFD XMM15, XMM15, 0 \n"
#endif
        // win permutations
        "MOV R8," PACK_BYTES64(0, 1, 2, 0x80, 3, 4, 5, 0x80) "\n"
        "VMOVQ XMM8, R8 \n"
        "MOV R9," PACK_BYTES64(6, 7, 8, 0x80, 0, 3, 6, 0x80) "\n"
        "VPINSRQ XMM8, XMM8, R9, 1 \n"
        "MOV R10," PACK_BYTES64(1, 4, 7, 0x80, 2, 5, 8, 0x80) "\n"
        "VMOVQ XMM9, R10 \n"
        "MOV R11," PACK_BYTES64(0, 4, 8, 0x80, 2, 4, 6, 0x80) "\n"
        "VPINSRQ XMM9, XMM9, R11, 1 \n"
        // print permutations
        "MOV RAX," PACK_BYTES64(0, 12, 1, 12, 2, 13, 14, 15) "\n"
        "VMOVQ XMM11, RAX \n"
        "MOV RDX," PACK_BYTES64(14, 15, 14, 13, 3, 12, 4, 12) "\n"
        "VPINSRQ XMM11, XMM11, RDX, 1 \n"
        "MOV R8," PACK_BYTES64(5, 13, 14, 15, 14, 15, 14, 13) "\n"
        "VMOVQ XMM12, R8 \n"
        "MOV R9," PACK_BYTES64(6, 12, 7, 12, 8, 11, 0x80, 0x80) "\n"
        "VPINSRQ XMM12, XMM12, R9, 1 \n"
        // board grid
        "MOV RCX," PACK_BYTES64(7, 0xB3, '\n', 0xC4, 0xC5, 0, 0, 0) "\n"
        "VMOVQ XMM6, RCX \n"
        // turn text
        "MOV R14," PACK_BYTES64(0, ' ', 'r', 'e', 'y', 'a', 'l', 'P') "\n"
        // key tables
        "MOV R8," PACK_WORDS64(RIGHT_KEY, DOWN_KEY, LEFT_KEY, UP_KEY) "\n"
        "VMOVQ XMM10, R8 \n"
        "MOV R9," PACK_WORDS64(' ', '\n', 'x', 'o') "\n"
        "VPINSRQ XMM10, XMM10, R9, 1 \n"
#if !(__AVX512F__ && __AVX512VL__ && __AVX512BW__)
        // indexing mask
        "MOV RAX," PACK_BYTES64(1, 2, 3, 4, 5, 6, 7, 8) "\n"
        "VMOVQ XMM13, RAX \n"
        "MOV EDX, 9 \n"
        "VPINSRB XMM13, XMM13, EDX, 8 \n"
#endif
    "1: \n" // restart_game
        // player markers, turn counter
        "MOV ESI," PACK_BYTES32('X', 'O', 0, 0x70) "\n"
#if INCLUDE_AI_MODE
        PUT_CHAR_ADDR(RBP, EBP)
        "MOV RBX," PACK_BYTES64('P', 'l', 'a', 'y', ' ', 'a', 'g', 'a') "\n"
        PRINT_RBX_RBP(8)
        "MOV RBX," PACK_BYTES64('i', 'n', 's', 't', ' ', ' ', '\n', 't') "\n"
        PRINT_RBX_RBP(8)
        "MOV RBX," PACK_BYTES64('h', 'e', ' ', 'c', 'o', 'm', 'p', 'u') "\n"
        PRINT_RBX_RBP(8)
        "MOV RBX," PACK_BYTES64('t', 'e', 'r', '?', ' ', ' ', '\n', 'Y') "\n"
        PRINT_RBX_RBP(8)
        "MOV ECX, '/' \n"
        PRINT_ECX_RBP
        "MOV ECX, 'N' \n"
        PRINT_ECX_RBP
    "2: \n" // get_ai_selection
        GET_KEYA(RBP, EBP)
        "CMP BL, 'n' \n"
        "JE 2f \n" // player_vs
        "CMP BL, 'y' \n"
        "JNE 2b \n" // get_ai_selection
        "OR ESI, 0x8000000 \n"
    "2: \n" // player_vs
        SET_CURSOR_ADDR(RBP, EBP)
        SET_CURSOR_RBP(5, 1)
        PRINT_BLANKS(8)
        RESET_CURSOR_RBP
#endif
        // cursor position
        "XOR EBP, EBP \n"
        // board reset
        "VPSLLDQ XMM6, XMM6, 11 \n"
        // player number
        "MOV R14B, '1' \n"
    "2: \n" // start_of_turn
        "MOV RBX, R14 \n"
        "BSWAP RBX \n"
        PRINT_RBX(8)
        "MOV RBX," PACK_BYTES64('\'', 's', ' ', 't', 'u', 'r', 'n', '\n') "\n"
        PRINT_RBX(8)
        "VPSHUFB XMM7, XMM6, XMM11 \n"
        PRINT_XMM7(16)
        "VPSHUFB XMM7, XMM6, XMM12 \n"
        PRINT_XMM7(13)
    "3: \n" // move_cursor
        SET_CURSOR_TO_EBP_BIG
    "4: \n" // is_bad_input
#if INCLUDE_AI_MODE
        "CMP R14B, '1' \n"
        "JE 5f \n" // get_input_key
        "TEST ESI, 0x8000000 \n"
        "JNZ 9f \n" // ai_move_selection
#endif
    "5: \n" // get_input_key
        GET_KEYW
        // CF = character is special
        // ECX = special character index
        "VMOVD XMM0, EBX \n"
        "VPCMPISTRI XMM0, XMM10, 1 \n"
        "JNC 5b \n" // get_input_key
        // TODO: Check if VPCMPEQW can work
        "CMP ECX, 4 \n"
        "JAE 8f \n" // is_selection_input
        "CMP ECX, 2 \n"
        "JE 5f \n" // left
        "JA 6f \n" // up
        "TEST ECX, ECX \n"
        "JNZ 7f \n" // down
        "MOV EAX, 0xDB \n"
        "BT EAX, EBP \n"
        "ADC EBP, 0 \n"
        "JMP 3b \n" // move_cursor
    "7: \n" // down
        "MOV EAX, 0x3F \n"
        "BT EAX, EBP \n"
        "SBB EDX, EDX \n"
        "AND EDX, 3 \n"
        "ADD EBP, EDX \n"
        "JMP 3b \n" // move_cursor
    "5: \n" // left
        "MOV EAX, 0x1B6 \n"
        "BT EAX, EBP \n"
        "SBB EBP, 0 \n"
        "JMP 3b \n" // move_cursor
    "6: \n" // up
        "MOV EAX, 0x1F8 \n"
        "BT EAX, EBP \n"
        "SBB EDX, EDX \n"
        "AND EDX, 3 \n"
        "SUB EBP, EDX \n"
        "JMP 3b \n" // move_cursor
#if INCLUDE_AI_MODE
    "9: \n" // ai_move_selection
#if __RDRND__ || __RDSEED__
        "XOR EBP, EBP \n"
    "9: \n"
#if __RDRND__
        "RDRAND BP \n"
#else
        "RDSEED BP \n"
#endif
        "JNC 9b" // ai_move_selection
#else
        "RDTSC \n"
        "MOVZX EBP, AX \n"
#endif
        "IMUL ECX, EBP, 0xE38F \n"
        "SHR ECX, 19 \n"
        "LEA EDX, [RCX*8+RCX] \n"
        "SUB EBP, EDX \n"
#endif
    "8: \n" // is_selection_input
#if __AVX512F__ && __AVX512VL__ && __AVX512BW__
        "MOV EAX, 1 \n"
#if __BMI2__
        "SHLX EAX, EAX, EBP \n"
#else
        "MOV ECX, EBP \n"
        "SHL EAX, CL \n"
#endif
        "KMOVD K1, EAX \n"
        "VPMOVM2B XMM0, K1 \n"
        "VPTEST XMM6, XMM0 \n"
        "JNZ 4b \n" // is_bad_input
        "VPBROADCASTB XMM6" P(K1) ", ESI \n"
#else
        "LEA EAX, [RBP+1] \n"
        "VMOVD XMM2, ESI \n"
        "VPXOR XMM3, XMM3, XMM3 \n"
        "VPSHUFB XMM2, XMM2, XMM3 \n"
        "VMOVD XMM1, EAX \n"
        "PCMPISTRM XMM1, XMM13, 0x40 \n"
        "VPTEST XMM6, XMM0 \n"
        "JNZ 4b \n" // is_bad_input
        "VPBLENDVB XMM6, XMM6, XMM2, XMM0 \n"
#endif
        "ROL SI, 8 \n"

        RESET_CURSOR

        "VPSHUFB XMM0, XMM6, XMM8 \n"
        "VPSHUFB XMM1, XMM6, XMM9 \n"
#if __AVX2__
        "VINSERTI128 YMM0, YMM0, XMM1, 1 \n"
        "VINSERTI128 YMM14, YMM14, XMM14, 1 \n"
#if __AVX512F__ && __AVX512VL__
        "VPCMPEQD K0, YMM0, YMM14 \n"
        "KTESTW K0, K0 \n"
#else
        "VPCMPEQD YMM2, YMM0, YMM14 \n"
        "VPTEST YMM2, YMM2 \n"
#endif
#else
        "VPCMPEQD XMM2, XMM0, XMM14 \n"
        "VPTEST XMM2, XMM2 \n"
        "JNZ 4f \n" // player_wins
        "VPCMPEQD XMM4, XMM1, XMM14 \n"
        "VPTEST XMM4, XMM4 \n"
#endif
        "JNZ 4f \n" // player_wins
#if __AVX2__
        "VINSERTI128 YMM15, YMM15, XMM15, 1 \n"
#if __AVX512F__ && __AVX512VL__
        "VPCMPEQD K0, YMM0, YMM15 \n"
        "KTESTW K0, K0 \n"
#else
        "VPCMPEQD YMM3, YMM0, YMM15 \n"
        "VPTEST YMM3, YMM3 \n"
#endif
#else
        "VPCMPEQD XMM3, XMM0, XMM15 \n"
        "VPTEST XMM3, XMM3 \n"
        "JNZ 4f \n" // player_wins
        "VPCMPEQD XMM5, XMM1, XMM15 \n"
        "VPTEST XMM5, XMM5 \n"
#endif
        "JNZ 4f \n" // player_wins
        "ADD ESI, 0x10000000 \n"
        "MOV RBX," PACK_BYTES64(' ', ' ', 'N', 'o', 'b', 'o', 'd', 'y') "\n"
        "JC 5f \n" // draw
        "XOR R14B, 3 \n"
        "JMP 2b \n" // start_of_turn
    "4: \n" // player_wins
        "MOV RBX, R14 \n"
        "BSWAP RBX \n"
    "5: \n" // draw
        PUT_CHAR_ADDR(RBP, EBP)
        PRINT_RBX_RBP(8)
        "MOV RBX, " PACK_BYTES64(' ', 'w', 'i', 'n', 's', '!', ' ', '\n') "\n"
        PRINT_RBX_RBP(8)
        "MOV RBX, " PACK_BYTES64('P', 'l', 'a', 'y', ' ', 'a', 'g', 'a') "\n"
        PRINT_RBX_RBP(8)
        "MOV RBX, " PACK_BYTES64('i', 'n', '?', ' ', 'Y', '/', 'N', '\n') "\n"
        PRINT_RBX_RBP(8)
        "VPSHUFB XMM7, XMM6, XMM11 \n"
        PRINT_XMM7_RBP(16)
        "VPSHUFB XMM7, XMM6, XMM12 \n"
        PRINT_XMM7_RBP(14)
        "VPSRLDQ XMM7, XMM13, 12 \n"
    "4: \n" // is_bad_restart_input
        GET_KEYA(RSI, ESI)
        "CMP BL, 'n' \n"
        "JE 1f \n" // end_game
        "CMP BL, 'y' \n"
        "JNE 4b \n" // is_bad_restart_input
#if !INCLUDE_AI_MODE
        "MOV ECX, '\r' \n"
        PRINT_ECX_RBP
        PRINT_BLANKS_RBP(5)
        SET_CURSOR(5, 1)
        PRINT_BLANKS_RBP(10)
#else
        SET_CURSOR(0, 2)
        "MOV EBX, 5 \n"
    "2: \n" // screen_clear_loop
        PRINT_BLANKS_RBP(5)
        PRINT_LINE_RBP
        "DEC EBX \n"
        "JNZ 2b \n" // screen_clear_loop
#endif
        RESET_CURSOR
        "VPSRLDQ XMM6, XMM6, 11 \n"
        "JMP 1b \n" // restart_game
    "1: \n" // end_game
        "ADD RSP, 0x28 \n"
        "RET \n"
        :
        //: asm_arg("m", kbhit_ptr), asm_arg("m", put_char_ptr), asm_arg("m", getch_ptr), asm_arg("m", get_std_handle_ptr), asm_arg("m", set_cursor_ptr), [stdout_id]"i"(STD_OUTPUT_HANDLE)
        : [stdout_id]"i"(STD_OUTPUT_HANDLE)
    );
}

int main(int argc, char* argv[]) {
    tic_tac_bs();
    return 0;
}