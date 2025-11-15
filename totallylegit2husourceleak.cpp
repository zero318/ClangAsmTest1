/*

Linker flags:

/LARGEADDRESSAWARE:NO
/DYNAMICBASE:NO
/HIGHENTROPYVA:NO
/ENTRY:yeet
/BASE:0x690000
/SECTION:LeSanae,RWE

*/

#include "windows_structs.h"

// Trying to avoid depending on my util header so this is
// at least *closer* to compliling as a single file...
#undef naked
#undef regcall
#define asm_arg(constraint, name) \
[name] constraint (name)
#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

#pragma code_seg("LeSanae")
#pragma data_seg("LeSanae")
#pragma const_seg("LeSanae")
#pragma bss_seg("LeSanae")

__attribute__((used, retain, naked)) void BEES1() {
	__asm__(
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
	);
}

#define STRING_BUFFER (0x690000 + 0x1000 + 0x35859F)
#define FUNC_BUFFER (0x690000 + 0x1000 + 0x35859F + 0x25 + 0x35859F)

// Unfinished version that would've XOR'd the first few bytes
// of the function as the string buffer. Too lazy tho
/*
__attribute__((used, retain, naked, regcall)) void yeet2() {
	__asm__(
		INTEL_64_DIRECTIVE
		".byte 0x65, 0x67, 0x48, 0xA1, 0x60, 0, 0, 0 \n"
		"MOV R8, [RAX+0x18] \n"
		"MOV R9, [R8+0x10] \n"
		"ADD R8, 0x10 \n"
		//"MOV R10, 0x80000000 \n"
		//"MOV R11, 0x80000000 \n"
		".byte 0x48, 0xBA \n"
		".byte 0x00, 'n'^0x00, 0x00, 'r'^0x60, 0xA1, 'e'^0x48, 0x67, 'k'^0x65 \n"
		".byte 0x49, 0xBB \n"
		".byte 0x10, '2'^0xC0, 0x83, '3'^0x49, 0x18, 'l'^0x40, 0x8B, 'e'^0x4C \n"
		//".byte 0x49, 0xBC \n"
		//".byte 0x00, 'l'^0xBA, 0x49, 'd'^0x08, 0x8B, '.'^0x4D \n"
		"MOV EBX, " MACRO_STR(STRING_BUFFER) " \n"
		"MOV ECX, 13 \n"
		"XOR [RBX+RCX-1], AL \n"
	);
}
*/

__attribute__((used, retain, naked)) void string_buffer() {
	__asm {
		__asm _emit 'k' __asm _emit 0
		__asm _emit 'e' __asm _emit 0
		__asm _emit 'r' __asm _emit 0
		__asm _emit 'n' __asm _emit 0
		__asm _emit 'e' __asm _emit 0
		__asm _emit 'l' __asm _emit 0
		__asm _emit '3' __asm _emit 0
		__asm _emit '2' __asm _emit 0
		__asm _emit '.' __asm _emit 0
		__asm _emit 'd' __asm _emit 0
		__asm _emit 'l' __asm _emit 0
		__asm _emit 'l' __asm _emit 0
		__asm _emit 0 __asm _emit 0
		__asm _emit 'Q' __asm _emit 'w'
		__asm _emit '4' __asm _emit 'w'
		__asm _emit '9' __asm _emit 'W'
		__asm _emit 'g' __asm _emit 'X'
		__asm _emit 'c' __asm _emit 'Q'
		__asm _emit 0
	}
}

__attribute__((used, retain, naked)) void BEES2() {
	__asm__(
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
	);
}

// Dummy arguments to make clang think it can clobber all the registers
__attribute__((regcall)) void yeet(int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L) {
	//const auto* module_list = &teb64->ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList;
	const PEB* peb_ptr;
	__asm__ volatile (
		//".byte 0x65, 0x67, 0x48, 0xA1, 0x60, 0, 0, 0 \n" // MOV RAX, GS:[SMALL ABS 0x60]
		".byte 0x65, 0x48, 0x8B, 0x05, 0xF5, 0xE4, 0x2B, 0xFF \n" // MOV RAX, GS:[REL 0x60]
		: "=a"(peb_ptr)
	);
	const auto* module_list = &peb_ptr->Ldr->InLoadOrderModuleList;
	__asm__ volatile ("":"+r"(module_list));

	const LDR_DATA_TABLE_ENTRY* module_data = module_list->Flink;
	
	// Without forcing R10 clang tries to use RAX, which causes
	// the second XOR loop to use RBX and that generates unwinds
	register uint64_t STRING asm("r10") = (uint32_t)STRING_BUFFER;
	__asm__ volatile ("":"+r"(STRING));
	do {
		if (repe_cmpsw((const wchar_t*)module_data->BaseDllName.Buffer, (PTR32Z<const wchar_t>)STRING, countof(L"kernel32.dll"))) {
			{
				/*
				uint128_t mask = PackUInt64('k'^'C', 'r', 'e'^'e', 'a', 'r'^'t', 'e', 'n'^'P', 'r') | (uint128_t)PackUInt64('e'^'o', 'c', 'l'^'e', 's', '3'^'s', 'A', '2') << 64;
				uint64_t i = 0;
				nounroll do {
					((uint8_t*)STRING)[i] ^= (uint8_t)mask;
					mask >>= 8;
					i = (uint32_t)(i + 1);
				} while ((uint32_t)i != countof("CreateProcessA"));
				*/
				uint64_t mask_low;
				uint64_t mask_high;
				__asm__ volatile (
					"MOV %[high], %[mask_high] \n"
					"MOV %[low], %[mask_low] \n"
					: asm_arg("=d", mask_low), asm_arg("=r", mask_high)
					: [low]"i"(PackUInt64('2', 'A', '3' ^ 's', 's', 'l' ^ 'e', 'c', 'e' ^ 'o', 'r'))
					, [high]"i"(PackUInt64('n' ^ 'P', 'e', 'r' ^ 't', 'a', 'e' ^ 'e', 'r', 'k' ^ 'C'))
				);
				uint8_t i = countof("CreateProcessA");
				__asm__ volatile (
				"1: \n"
					"XORB %b[mask_low], -1(%q[STRING],%%RCX) \n"
					"SHRD $8, %[mask_high], %[mask_low] \n"
					"SHR $8, %[mask_high] \n"
					"LOOP 1b"
					: asm_arg("+d", mask_low), asm_arg("+r", mask_high), "+c"(i)
					: asm_arg("r", STRING)
				);
			}
			uint32_t kernel32 = *(uint32_t*)&module_data->DllBase;
			uint32_t nt_header = kernel32 + ((IMAGE_DOS_HEADER*)kernel32)->e_lfanew;
			uint32_t exports = kernel32 + ((IMAGE_NT_HEADERS64*)nt_header)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
			uint32_t names = kernel32 + ((IMAGE_EXPORT_DIRECTORY*)exports)->AddressOfNames;
			uint64_t name_count = ((IMAGE_EXPORT_DIRECTORY*)exports)->NumberOfNames;
			if ((uint32_t)name_count) {
				// If the variable isn't u64 then clang will pointlessly zero extend
				// after doing a DEC sometimes, but this pattern avoids it
				name_count = (uint32_t)(name_count - 1);
				do {
					uint32_t export_name = kernel32 + ((uint32_t*)names)[name_count];
					if (repe_cmpsb((const char*)export_name, (PTR32Z<const char>)STRING, countof("CreateProcessA"))) {
						uint32_t ordinals = kernel32 + ((IMAGE_EXPORT_DIRECTORY*)exports)->AddressOfNameOrdinals;
						uint32_t ordinal = ((uint16_t*)ordinals)[name_count];
						uint32_t addresses = kernel32 + ((IMAGE_EXPORT_DIRECTORY*)exports)->AddressOfFunctions;
						uint32_t create_process = kernel32 + ((uint32_t*)addresses)[ordinal];
						{
							/*
							UBitInt(208) mask =
								PackUInt64('C'^'e', 'r'^'x', 'e'^'p', 'a'^'l', 't'^'o', 'e'^'r', 'P'^'e', 'r'^'r') |
								(uint128_t)PackUInt64('o'^' ', 'c'^'h', 'e'^'t', 's'^'t', 's'^'p', 'A'^':', '/', '/') << 64 |
								(UBitInt(208))PackUInt64('.'^'y', 'o', 'd'^'u', 't', 'l'^'u', '.', 'l'^'b', 'e') << 128 |
								(UBitInt(208))PackUInt64('/', 'd') << 192;
							uint64_t i = 0;
							nounroll do {
								((uint8_t*)STRING)[i] ^= (uint8_t)mask;
								mask >>= 8;
								i = (uint32_t)(i + 1);
							} while ((uint32_t)i != countof("explorer http://youtu.be/"));
							*/
							uint64_t maskA;
							uint64_t maskB;
							uint64_t maskC;
							uint16_t maskD;
							__asm__ volatile (
								"MOV %[D], %[maskD] \n"
								"MOV %[C], %[maskC] \n"
								"MOV %[B], %[maskB] \n"
								"MOV %[A], %[maskA] \n"
								: asm_arg("=q", maskA), asm_arg("=r", maskB), asm_arg("=r", maskC), asm_arg("=d", maskD)
								: [A]"i"(PackUInt64('d', '/', 'e', 'l'^'b', '.', 'l'^'u', 't', 'd'^'u'))
								, [B]"i"(PackUInt64('o', '.'^'y', '/', '/', 'A'^':', 's'^'p', 's'^'t', 'e'^'t'))
								, [C]"i"(PackUInt64('c'^'h', 'o'^' ', 'r'^'r', 'P'^'e', 'e'^'r', 't'^'o', 'a'^'l', 'e'^'p'))
								, [D]"i"(PackUInt16('r'^'x', 'C'^'e'))
							);
							uint32_t i = countof("explorer http://youtu.be/");
							__asm__ volatile (
							"1: \n"
								"XORB %b[maskA], -1(%q[STRING],%%RCX) \n"
								"SHRD $8, %[maskB], %[maskA] \n"
								"SHRD $8, %[maskC], %[maskB] \n"
								"SHRD $8, %q[maskD], %[maskC] \n"
								"MOVZX %h[maskD], %k[maskD] \n"
								"LOOP 1b"
								: asm_arg("+q", maskA), asm_arg("+r", maskB), asm_arg("+r", maskC), asm_arg("+d", maskD), "+c"(i)
								: asm_arg("r", STRING)
							);
						}
						*(uint8_t*)yeet = sizeof(STARTUPINFOA);
						rep_stosb((uint8_t*)(FUNC_BUFFER + 1), 0, sizeof(STARTUPINFOA) - 1 + sizeof(PROCESS_INFORMATION));
						//((decltype(&CreateProcessA))create_process)(NULL, (char*)STRING_BUFFER, NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOA)FUNC_BUFFER, (LPPROCESS_INFORMATION)(FUNC_BUFFER + sizeof(STARTUPINFOA)));
						
						__asm__ volatile(
							"PUSH %[pi] \n"
							"PUSH %[si] \n"
							"PUSH $0 \n"
							"PUSH $0 \n"
							"PUSH $0 \n"
							"PUSH $0 \n"
							"SUB $0x20, %%RSP \n"
							"MOV %[cmd], %%EDX \n"
							"XOR %%ECX, %%ECX \n"
							"XOR %%R8D, %%R8D \n"
							"XOR %%R9D, %%R9D \n"
							"CALLQ *%[func] \n"
							"ADD $0x50, %%RSP \n"
							:
							: [func]"R"((uint64_t)create_process), [cmd]"i"(STRING_BUFFER), [si]"i"(FUNC_BUFFER), [pi]"i"(FUNC_BUFFER + sizeof(STARTUPINFOA))
						);
						
						return;
					}
				} while ((name_count = (uint32_t)(name_count - 1)));
			}
		name_zero:
			return;
		}
		module_data = module_data->InLoadOrderLinks.Flink;
	} while ((void*)module_data != (void*)module_list);
	
	return;
}

__attribute__((used, retain, naked)) void BEES3() {
	__asm__(
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
#include "bee_movie_inline_asm.h"
//#include "bee_movie_inline_asm.h"
//#include "bee_movie_inline_asm.h"
	);
}