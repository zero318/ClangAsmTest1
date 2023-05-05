#pragma once
#include <stdint.h>

extern "C" {
	extern uint8_t RawLinkerZero[] asm("_LinkerZero");
	extern uint8_t RawLinkerOne[] asm("_LinkerOne");
	extern uint8_t RawLinkerTwo[] asm("_LinkerTwo");
	extern uint8_t RawLinkerThree[] asm("_LinkerThree");
	extern uint8_t RawLinkerFour[] asm("_LinkerFour");
	extern uint8_t RawLinkerFive[] asm("_LinkerFive");
	extern uint8_t RawLinkerSix[] asm("_LinkerSix");
	extern uint8_t RawLinkerSeven[] asm("_LinkerSeven");
}

#define LinkerZero ((uintptr_t)RawLinkerZero)
#define LinkerOne ((uintptr_t)RawLinkerOne)
#define LinkerTwo ((uintptr_t)RawLinkerTwo)
#define LinkerThree ((uintptr_t)RawLinkerThree)
#define LinkerFour ((uintptr_t)RawLinkerFour)
#define LinkerFive ((uintptr_t)RawLinkerFive)
#define LinkerSix ((uintptr_t)RawLinkerSix)
#define LinkerSeven ((uintptr_t)RawLinkerSeven)