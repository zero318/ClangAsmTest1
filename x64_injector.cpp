#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

// Prevent "function cannot be redeclared with dllimport" nonsense
#define NtQueryInformationProcess NtQueryInformationProcess_dummy

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>

#include <atomic>

#include "../zero/util.h"

#include "../windows_structs.h"

#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll64.lib")

static inline constexpr wchar_t dummy_dll_name[] = L"x64_inject_test.dll";

#pragma comment (lib, "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\out\\build\\x64-Clang-Release\\x64_inject_test.lib")

#undef NtQueryInformationProcess

extern "C" {
	__declspec(dllimport) void config(HANDLE id);
	__declspec(dllimport) void* wait();
	__declspec(dllimport) NTSTATUS NTAPI
		NtQueryInformationProcess(
			IN HANDLE ProcessHandle,
			IN PROCESSINFOCLASS ProcessInformationClass,
			OUT PVOID ProcessInformation,
			IN ULONG ProcessInformationLength,
			OUT PULONG ReturnLength OPTIONAL
		);
}

static const uint8_t infinite_loop[] = {
				// inf:
	0xEB, 0xFE, //   JMP inf
};

bool x64_inject_test(const char* program) {

	union {
		struct {
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
		};
		struct {
			PROCESS_BASIC_INFORMATIONX<64> process_info;
			ULONG idgaf;
		};
		struct {
			uintptr_t module_base;
			DWORD offset;
		};
		struct {
			CONTEXTX<64> context;
			DWORD protection;
			uint8_t old_bytes[sizeof(infinite_loop)];
		};
	};
	si = { sizeof(STARTUPINFOA) };
	pi = {};

	if (CreateProcessA(program, NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
#if !IS_X64
		BOOL is_wow64 = true;
		if (auto is_wow64_func = (decltype(&IsWow64Process))GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process")) {
			is_wow64_func(pi.hProcess, &is_wow64);
		}
		if (!is_wow64) {
			return false;
		}
#else
		//BOOL is_wow64 = false;
		//IsWow64Process(pi.hProcess, &is_wow64);
		//if (is_wow64) {
			//return false;
		//}
#endif
		HANDLE process = pi.hProcess;
		HANDLE main_thread = pi.hThread;
		
		NtQueryInformationProcess(process, ProcessBasicInformation, &process_info, sizeof(process_info), &idgaf);

		HANDLE pid = (HANDLE)process_info.UniqueProcessId;
		ReadProcessMemory(process, &process_info.PebBaseAddress->ImageBaseAddress, &module_base, sizeof(module_base), NULL);
		ReadProcessMemory(process, (LPCVOID)(module_base + 0x3C), &offset, sizeof(DWORD), NULL);
		ReadProcessMemory(process, (LPCVOID)(module_base + offset + 0x28), &offset, sizeof(DWORD), NULL);

		uintptr_t entrypoint = module_base + offset;

		VirtualProtectEx(process, (LPVOID)entrypoint, sizeof(infinite_loop), PAGE_EXECUTE_READWRITE, &protection);
		ReadProcessMemory(process, (LPCVOID)entrypoint, old_bytes, sizeof(old_bytes), NULL);
		WriteProcessMemory(process, (LPVOID)entrypoint, infinite_loop, sizeof(infinite_loop), NULL);
		FlushInstructionCache(process, (LPCVOID)entrypoint, sizeof(infinite_loop));

		context = {};
		context.ContextFlags = CONTEXT_CONTROL;
		do {
			ResumeThread(main_thread);
			Sleep(10);
			SuspendThread(main_thread);
			GetThreadContext(main_thread, (CONTEXT*)&context);
		} while (entrypoint != context.Rip);

		WriteProcessMemory(process, (LPVOID)entrypoint, old_bytes, sizeof(old_bytes), NULL);
		VirtualProtectEx(process, (LPVOID)entrypoint, sizeof(old_bytes), protection, &protection);
		FlushInstructionCache(process, (LPCVOID)entrypoint, sizeof(old_bytes));

		void* inject_buffer = VirtualAllocEx(process, NULL, sizeof(dummy_dll_name), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		WriteProcessMemory(process, inject_buffer, dummy_dll_name, sizeof(dummy_dll_name), NULL);

		config(pid);

		void* init_address = NULL;

		HANDLE thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)&LoadLibraryW, inject_buffer, 0, NULL);
		if (thread) {
			init_address = wait();
			WaitForSingleObject(thread, INFINITE);
			CloseHandle(thread);
		}

		VirtualFreeEx(process, inject_buffer, 0, MEM_RELEASE);

		bool ret = false;
		if (init_address) {
			thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)init_address, (LPVOID)module_base, 0, NULL);
			if (thread) {
				WaitForSingleObject(thread, INFINITE);
				CloseHandle(thread);
				ret = true;
			}
		}

		ResumeThread(main_thread);

		CloseHandle(process);
		CloseHandle(main_thread);

		return ret;
	}
	return false;
}


//int main(int argc, char* argv[]) {
//int __clrcall WinMain(
int entry() {
	const char* inject_target = "C:/Windows/notepad.exe";
	//if (argc > 1) {
		//inject_target = argv[1];
	//}
	return !x64_inject_test(inject_target);
}