#ifndef WOW64_CPU_H
#define WOW64_CPU_H 1

// Export #1
void CpuFlushInstructionCache(HANDLE ProcessHandle, void* BaseAddress, uint32_t NumberOfBytesToFlush, WOW64_FLUSH_REASON Reason);

// Export #2
NTSTATUS CpuGetContext(HANDLE ThreadHandle, UNUSED_ARG(HANDLE ProcessHandle), UNUSED_ARG(TEBX<64>* teb_ptr), CONTEXTX<32>* context_out);

// Export #3
PTR32Z<> CpuGetStackPointer();

// Export #4
void CpuInitializeStartupContext(HANDLE ProcessHandle, int, TEBX<64>* teb_ptr, CPUCONTEXT* context_out, const CONTEXTX<32>* context_in);

#ifndef COMPILE_WOW64_CPU
// Export #5
static inline void CpuNotifyAffinityChange() {
}
#endif

// Export #6
static inline void CpuNotifyAfterFork() {
}

// Export #7
static inline void CpuNotifyBeforeFork() {
}

// Export #8
static inline void CpuNotifyDllLoad(wchar_t* DllName, void* DllBase, uint32_t DllSize) {
}

// Export #9
static inline void CpuNotifyDllUnload(void* DllBase) {
}

// Export #10
static inline void CpuPrepareForDebuggerAttach() {
}

// Export #11
static inline bool CpuProcessDebugEvent() {
	return false;
}

// Export #12
NTSTATUS CpuProcessInit(UNUSED_ARG(wchar_t* ImageName), size_t* cpu_size);

#ifndef COMPILE_WOW64_CPU
// Export #13
static inline NTSTATUS CpuProcessTerm(UNUSED_ARG(HANDLE ProcessHandle)) {
	return 0;
}
#endif

// Export #14
static inline void CpuResetFloatingPoint() {
}

// Export #15
void CpuResetToConsistentState(EXCEPTION_POINTERSX<64>* exception_pointers);

// Export #16
NTSTATUS CpuSetContext(HANDLE ThreadHandle, UNUSED_ARG(HANDLE ProcessHandle), UNUSED_ARG(TEBX<64>* teb_ptr), const CONTEXTX<32>* context_in);

// Export #17
void CpuSetInstructionPointer(PTR32Z<> ptr);

// Export #18
void CpuSetStackPointer(PTR32Z<> ptr);

// Export #19
void CpuSimulate();

// Export #20
NTSTATUS CpuSuspendLocalThread();

// Export #21
static inline NTSTATUS CpuSuspendThread(HANDLE ThreadHandle, HANDLE ProcessHandle, uint32_t* arg4) {
	return 0;
}

// Export #22
NTSTATUS CpuThreadInit(CPUCONTEXT* virt_context);

// Export #23
static inline NTSTATUS CpuThreadTerm() {
	return 0;
}

#endif