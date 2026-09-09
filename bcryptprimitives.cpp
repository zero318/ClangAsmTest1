#include "stdint.h"

#include "windows.h"

#pragma comment(lib, "AdvAPI32.Lib")
extern "C" {
	__declspec(dllimport) BOOLEAN RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength) asm("SystemFunction036");
}


#define musttail [[clang::musttail]]

// Clang does this silly thing where it'll
// pointlessly move stack arguments back and
// forth when making a tail call, so just
// manually jump to the correct address
// when you know it's safe.
#define jump_to_mem_addr(var) __asm__("jmp* %0"::"m"(var)); __builtin_unreachable()

#if __INTELLISENSE__
// Intellisense doesn't know what
// constinit is when using clang
#define constinit constexpr
#endif

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

uint64_t GetAsymmetricEncryptionInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
uint64_t GetCipherInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
uint64_t GetHashInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
uint64_t GetRngInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
uint64_t GetSecretAgreementInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
uint64_t GetSignatureInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d);

using ShimPtr = uint64_t(uint64_t a, uint64_t b, uint64_t c, uint64_t d);

static uint64_t HaltAndCatchFire(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	__asm__("int3");
}

static ShimPtr* GetAsymmetricEncryptionInterfaceResolved = GetAsymmetricEncryptionInterfaceShim;
static ShimPtr* GetCipherInterfaceResolved = GetCipherInterfaceShim;
static ShimPtr* GetHashInterfaceResolved = GetHashInterfaceShim;
static ShimPtr* GetRngInterfaceResolved = GetRngInterfaceShim;
static ShimPtr* GetSecretAgreementInterfaceResolved = GetSecretAgreementInterfaceShim;
static ShimPtr* GetSignatureInterfaceResolved = GetSignatureInterfaceShim;

static inline const void* *const func_ptrs[] = {
	(const void**const)&GetAsymmetricEncryptionInterfaceResolved,
	(const void**const)&GetCipherInterfaceResolved,
	(const void**const)&GetHashInterfaceResolved,
	(const void**const)&GetRngInterfaceResolved,
	(const void**const)&GetSecretAgreementInterfaceResolved,
	(const void**const)&GetSignatureInterfaceResolved
};

static inline const char *const func_names[] = {
	"GetAsymmetricEncryptionInterface",
	"GetCipherInterface",
	"GetHashInterface",
	"GetRngInterface",
	"GetSecretAgreementInterface",
	"GetSignatureInterface"
};

static void resolve_funcs() {
	if (HMODULE lib = LoadLibraryW(L"C:\\Windows\\System32\\bcryptprimitives.dll")) {
		for (size_t i = 0; i < countof(func_ptrs); ++i) {
			*func_ptrs[i] = (void*)GetProcAddress(lib, func_names[i]) ? : (const void *const)HaltAndCatchFire;
		}
	} else {
		for (size_t i = 0; i < countof(func_ptrs); ++i) {
			*func_ptrs[i] = (const void *const)HaltAndCatchFire;
		}
	}
}

uint64_t GetAsymmetricEncryptionInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	resolve_funcs();
	musttail return GetAsymmetricEncryptionInterfaceResolved(a, b, c, d);
}
uint64_t GetCipherInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	resolve_funcs();
	musttail return GetCipherInterfaceResolved(a, b, c, d);
}
uint64_t GetHashInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	resolve_funcs();
	musttail return GetHashInterfaceResolved(a, b, c, d);
}
uint64_t GetRngInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	resolve_funcs();
	musttail return GetRngInterfaceResolved(a, b, c, d);
}
uint64_t GetSecretAgreementInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	resolve_funcs();
	musttail return GetSecretAgreementInterfaceResolved(a, b, c, d);
}
uint64_t GetSignatureInterfaceShim(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	resolve_funcs();
	musttail return GetSignatureInterfaceResolved(a, b, c, d);
}

extern "C" {
	BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
		return TRUE;
	}
	uint64_t GetAsymmetricEncryptionInterface(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
		jump_to_mem_addr(GetAsymmetricEncryptionInterfaceResolved);
	}
	uint64_t GetCipherInterface(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
		jump_to_mem_addr(GetCipherInterfaceResolved);
	}
	uint64_t GetHashInterface(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
		jump_to_mem_addr(GetHashInterfaceResolved);
	}
	uint64_t GetRngInterface(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
		jump_to_mem_addr(GetRngInterfaceResolved);
	}
	uint64_t GetSecretAgreementInterface(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
		jump_to_mem_addr(GetSecretAgreementInterfaceResolved);
	}
	uint64_t GetSignatureInterface(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
		jump_to_mem_addr(GetSignatureInterfaceResolved);
	}
	BOOL ProcessPrng(PBYTE pbData, SIZE_T cbData) {
		return RtlGenRandom(pbData, cbData);
	}
}