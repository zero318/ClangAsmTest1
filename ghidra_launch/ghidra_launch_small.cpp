#include "Windows.h"

#pragma code_seg(".rdata")
#pragma data_seg(".rdata")
#pragma const_seg(".rdata")
#pragma bss_seg(".rdata")

__declspec(code_seg(".rdata"), guard(nocf))
BOOL launch() {
	STARTUPINFOA si = {};
	PROCESS_INFORMATION pi;
	return CreateProcessA(NULL, (char*)"launch_ghidra.bat", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}

/*
#include "..\windows_structs.h"

//#pragma section("launch",read,write,execute)


//__declspec(allocate(".text"))
//static const char launch_str[] = "ghidraRun.bat";

__declspec(code_seg("launch"), guard(nocf))
__attribute__((used, retain)) naked void launch_str() {
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
		__asm _emit 'C'
		__asm _emit 'r'
		__asm _emit 'e'
		__asm _emit 'a'
		__asm _emit 't'
		__asm _emit 'e'
		__asm _emit 'P'
		__asm _emit 'r'
		__asm _emit 'o'
		__asm _emit 'c'
		__asm _emit 'e'
		__asm _emit 's'
		__asm _emit 's'
		__asm _emit 'A'
		__asm _emit 0
		__asm _emit 'l'
		__asm _emit 'a'
		__asm _emit 'u'
		__asm _emit 'n'
		__asm _emit 'c'
		__asm _emit 'h'
		__asm _emit '_'
		__asm _emit 'g'
		__asm _emit 'h'
		__asm _emit 'i'
		__asm _emit 'd'
		__asm _emit 'r'
		__asm _emit 'a'
		__asm _emit '.'
		__asm _emit 'b'
		__asm _emit 'a'
		__asm _emit 't'
		__asm _emit 0
	}
}

#define KERNEL32_DLL_WSTR (const wchar_t*)(0x690000 + 0x1000)
#define CREATEPROCESSA_STR (const char*)(0x690000 + 0x1000 + sizeof(L"kernel32.dll"))
#define GHIDRARUN_BAT_STR (const char*)(0x690000 + 0x1000 + sizeof(L"kernel32.dll") + sizeof("CreateProcessA"))

__declspec(code_seg("launch"), guard(nocf))
regcall void launch(int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L) {
	//const auto* module_list = &teb64->ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList;
	const PEB* peb_ptr = peb64;
	const auto* module_list = &peb_ptr->Ldr->InLoadOrderModuleList;
	__asm__ volatile ("":"+r"(module_list));

	const LDR_DATA_TABLE_ENTRY* module_data = module_list->Flink;
	
	// Without forcing R10 clang tries to use RAX, which causes
	// the second XOR loop to use RBX and that generates unwinds
	register uint64_t STRING asm("r10") = (uint32_t)KERNEL32_DLL_WSTR;
	__asm__ volatile ("":"+r"(STRING));
	do {
		if (repe_cmpsw((const wchar_t*)module_data->BaseDllName.Buffer, (PTR32Z<const wchar_t>)STRING, countof(L"kernel32.dll"))) {
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
					STRING = (uint32_t)CREATEPROCESSA_STR;
					__asm__ volatile ("":"+r"(STRING));
					if (repe_cmpsb((const char*)export_name, (PTR32Z<const char>)STRING, countof("CreateProcessA"))) {
						uint32_t ordinals = kernel32 + ((IMAGE_EXPORT_DIRECTORY*)exports)->AddressOfNameOrdinals;
						uint32_t ordinal = ((uint16_t*)ordinals)[name_count];
						uint32_t addresses = kernel32 + ((IMAGE_EXPORT_DIRECTORY*)exports)->AddressOfFunctions;
						uint32_t create_process = kernel32 + ((uint32_t*)addresses)[ordinal];
						STARTUPINFOA si;
						rep_stosb(&si, 0, sizeof(STARTUPINFOA));
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
							: [func]"R"((uint64_t)create_process), [cmd]"i"(GHIDRARUN_BAT_STR), [si]"r"(&si), [pi]"i"((uint32_t)KERNEL32_DLL_WSTR)
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
*/