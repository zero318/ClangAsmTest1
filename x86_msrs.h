#pragma once
#ifndef X86_MSRS_H
#define X86_MSRS_H 1

#include "zero/util.h"

struct MSR_TIME_STAMP_COUNTER : MSR<0x10, MSR_TIME_STAMP_COUNTER> {
	uint64_t tsc;
};

struct MSR_IA32_FEATURE_CONTROL : MSR<0x3A, MSR_IA32_FEATURE_CONTROL> {
	union {
		uint64_t raw;
		struct {
			uint64_t lock : 1; // 0 (Prevents further modification)
			uint64_t EnableVMXInsideSMX : 1; // 1
			uint64_t EnableVMXOutsideSMX : 1; // 2
			uint64_t : 5; // 3-7
			
		};
	};
};

struct MSR_IA32_SPEC_CTRL : MSR<0x48, MSR_IA32_SPEC_CTRL> {
	union {
		uint64_t raw;
		struct {
			uint64_t IBRS : 1; // 0
			uint64_t STIBP : 1; // 1
			uint64_t SSBD : 1; // 2 (Speculative Store Bypass Disable)
			uint64_t : 4; // 3-6
			uint64_t PSFD : 1; // 7 (Predictive Store Forwarding Disable)
			uint64_t : 56; // 8-63
		};
	};
};

struct MSR_IA32_PRED_CMD : MSR<0x49, MSR_IA32_PRED_CMD> {
	union {
		uint64_t raw;
		struct {
			uint64_t IBPB : 1; // 0
			uint64_t : 63; // 1-63
		};
	};
};

struct MSR_IA32_PMC0 : MSR<0xC1, MSR_IA32_PMC0> {
	uint64_t value;
};

struct MSR_IA32_PMC1 : MSR<0xC2, MSR_IA32_PMC1> {
	uint64_t value;
};

struct MSR_IA32_PMC2 : MSR<0xC3, MSR_IA32_PMC2> {
	uint64_t value;
};

struct MSR_IA32_PMC3 : MSR<0xC4, MSR_IA32_PMC3> {
	uint64_t value;
};

struct MSR_IA32_PMC4 : MSR<0xC5, MSR_IA32_PMC4> {
	uint64_t value;
};

struct MSR_IA32_PMC5 : MSR<0xC6, MSR_IA32_PMC5> {
	uint64_t value;
};

struct MSR_IA32_PMC6 : MSR<0xC7, MSR_IA32_PMC6> {
	uint64_t value;
};

struct MSR_IA32_PMC7 : MSR<0xC8, MSR_IA32_PMC7> {
	uint64_t value;
};

struct MSR_IA32_ARCH_CAPABILITIES : MSR<0x10A, MSR_IA32_ARCH_CAPABILITIES> {
	union {
		uint64_t raw;
		struct {
			uint64_t RDCL_NO : 1; // 0
			uint64_t IBRS_ALL : 1; // 1
			uint64_t RSBA : 1; // 2
			uint64_t SKIP_L1DFL_VMENTRY : 1; // 3
			uint64_t SSB_NO : 1; // 4
			uint64_t MDS_NO : 1; // 5
			uint64_t IFPCMC_NO : 1; // 6
			uint64_t TSX_CTL : 1; // 7
			uint64_t TAA_NO : 1; // 8
			uint64_t : 55; // 9-63
		};
	};
};

struct MSR_IA32_TSX_CTRL : MSR<0x122, MSR_IA32_TSX_CTRL> {
	union {
		uint64_t raw;
		struct {
			uint64_t RTM_DISABLE : 1; // 0
			uint64_t TSX_CPUID_CLEAR : 1; // 1
			uint64_t : 62; // 2-63
		};
	};
};

struct MSR_SYSENTER_CS : MSR<0x174, MSR_SYSENTER_CS> {
	union {
		uint64_t raw;
		struct {
			seg_t cs_selector; // 0x0
			uint16_t unused; // 0x2
			// 0x4
		};
	};
};

struct MSR_SYSENTER_RSP : MSR<0x175, MSR_SYSENTER_RSP> {
	uint64_t value;
};

struct MSR_SYSENTER_RIP : MSR<0x176, MSR_SYSENTER_RIP> {
	uint64_t address;
};

struct MSR_IA32_PERFEVTSEL1 : MSR<0x187, MSR_IA32_PERFEVTSEL1> {
	uint64_t value;
};

struct MSR_IA32_PERFEVTSEL2 : MSR<0x188, MSR_IA32_PERFEVTSEL2> {
	uint64_t value;
};

struct MSR_IA32_PERFEVTSEL3 : MSR<0x189, MSR_IA32_PERFEVTSEL3> {
	uint64_t value;
};

struct MSR_IA32_MISC_ENABLE : MSR<0x1A0, MSR_IA32_MISC_ENABLE> {
	union {
		uint64_t raw;
		struct {
			uint64_t FastStringsEnable : 1; // 0
			uint64_t : 1; // 1
			// Finally found docs on this in the main manual volume 1.
			// When set to 1 this sets FOP for all non-control x87 instructions
			// and not just ones that produce unmasked exceptions
			uint64_t EnableFOpcode : 1; // 2 (Used as an example for MSR format, but then not documented in the msr list) [Not Architectural]
			uint64_t ATCCEnable : 1; // 3 (Automatic Thermal Control Circuit Enable)
			uint64_t SplitLockDisable : 1; // 4 [Not Architectural]
			uint64_t : 1; // 5
			uint64_t L3CacheDisable : 1; // 6 [Not Architectural]
			uint64_t PerformanceMonitoringAvailable : 1; // 7
			uint64_t SuppressLockEnable : 1; // 8 [Not Architectural]
			uint64_t HardwarePrefetchDisable : 1; // 9 [Not Architectural]
			uint64_t FERRReportingEnable : 1; // 10 (FERRMultiplexingEnable) [Not Architectural]
			uint64_t BTSUnsupported : 1; // 11
			uint64_t PEBSUnsupported : 1; // 12
			uint64_t ThermalMonitor2 : 1; // 13 [Not Architectural]
			uint64_t : 2; // 14-15
			uint64_t EnhancedSpeedStepEnable : 1; // 16
			uint64_t : 1; // 17
			uint64_t MONITOREnable : 1; // 18
			uint64_t AdjacentCacheLinePrefetchDisable : 1; // 19 [Not Architectural]
			uint64_t EnhancedSpeedStepSelectLock : 1; // 20 [Not Architectural]
			uint64_t : 1; // 21
			uint64_t LimitCPUIDMax : 1; // 22
			uint64_t xTPRMessageDisable : 1; // 23
			uint64_t L1DataCacheContextMode : 1; // 24 [Not Architectural]
			uint64_t : 2; // 25-26
			uint64_t __unknown_windows_set : 1; // 27
			uint64_t : 6; // 28-33
			uint64_t CPUID_NX_Disable : 1; // 34
			uint64_t : 2; // 35-36
			uint64_t DCUPrefetcherDisable : 1; // 37 [Not Architectural]
			uint64_t TurboModeDisable : 1; // 38 (IDADisable) [Not Architectural]
			uint64_t IPPrefetcherDisable : 1; // 39 [Not Architectural]
			uint64_t : 24; // 40-63
		};
	};
};

struct MSR_DBG_CTL : MSR<0x1D9, MSR_DBG_CTL> {
	union {
		uint64_t raw;
		struct {
			uint64_t LBR : 1; // 0
			uint64_t BTF : 1; // 1
			uint64_t PB : 4; // 2-5
			uint64_t TR : 1; // 6
			uint64_t BTS : 1; // 7
			uint64_t BTINT : 1; // 8
			uint64_t BTS_OFF_OS : 1; // 9
			uint64_t BTS_OFF_USR : 1; // 10
			uint64_t FREEZE_LBRS_ON_PMI : 1; // 11
			uint64_t FREEZE_PERFMON_ON_PMI : 1; // 12
			uint64_t ENABLE_UNCORE_PMI : 1; // 13
			uint64_t FREEZE_WHILE_SMM : 1; // 14
			uint64_t RTM_DEBUG : 1; // 15
			uint64_t : 48; // 16-63
		};
	};
};

// Instr_Retired.Any
struct MSR_IA32_FIXED_CTR0 : MSR<0x309, MSR_IA32_FIXED_CTR0> {
	uint64_t count;
};

// CPU_CLK_Unhalted.Core
struct MSR_IA32_FIXED_CTR1 : MSR<0x309, MSR_IA32_FIXED_CTR1> {
	uint64_t count;
};

// CPU_CLK_Unhalted.Ref
struct MSR_IA32_FIXED_CTR2 : MSR<0x309, MSR_IA32_FIXED_CTR2> {
	uint64_t count;
};

struct MSR_IA32_PERF_CAPABILITIES : MSR<0x345, MSR_IA32_PERF_CAPABILITIES> {
	union {
		uint64_t raw;
		struct {
			uint64_t LBRFormat : 6; // 0-5
			uint64_t PEBSTrap : 1; // 6
			uint64_t PEBSSaveArchRegs : 1; // 7
			uint64_t PEBSRecordFormat : 4; // 8-11
			uint64_t SMMFreeze : 1; // 12
			uint64_t FullWidthPMC : 1; // 13
			uint64_t : 1; // 14
			uint64_t PerformanceMetricsAvailable : 1; // 15
			uint64_t PEBSOutputToPT : 1; // 16
			uint64_t : 47; // 17-63
		};
	};
};

struct MSR_IA32_FIXED_CTR_CTRL : MSR<0x38D, MSR_IA32_FIXED_CTR_CTRL> {
	union {
		uint64_t raw;
		struct {
			uint64_t CTR0KernelEnable : 1; // 0
			uint64_t CTR0UserEnable : 1; // 1
			uint64_t CTR0AllSharedCores : 1; // 2
			uint64_t CTR0PMIEnable : 1; // 3
			uint64_t CTR1KernelEnable : 1; // 4
			uint64_t CTR1UserEnable : 1; // 5
			uint64_t CTR1AllSharedCores : 1; // 6
			uint64_t CTR1PMIEnable : 1; // 7
			uint64_t CTR2KernelEnable : 1; // 8
			uint64_t CTR2UserEnable : 1; // 9
			uint64_t CTR2AllSharedCores : 1; // 10
			uint64_t CTR2PMIEnable : 1; // 11
			uint64_t : 52; // 12-63
		};
	};
};

struct MSR_IA32_BNDCFGS : MSR<0xD90, MSR_IA32_BNDCFGS> {
	union {
		uint64_t raw;
		struct {
			uint64_t EnableSupervisorMPX : 1; // 0
			uint64_t BNDPRESERVE : 1; // 1
			uint64_t : 10; // 2-11
			uint64_t BoundDirectoryAddress : 52; // 12-63
		};
	};
};

struct MSR_IA32_XSS : MSR<0xDA0, MSR_IA32_XSS> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 8; // 0-7
			uint64_t TracePacketConfigurationState : 1; // 8
			uint64_t : 2; // 9-10
			uint64_t CET_U_State : 1; // 11
			uint64_t CET_S_State : 1; // 12
			uint64_t HDC_State : 1; // 13
			uint64_t : 50; // 14-63
		};
	};
};

struct MSR_UNKNOWN_A : MSR<0x400000F0, MSR_UNKNOWN_A> {
	uint64_t raw;
};

struct MSR_EFER : MSR<0xC0000080, MSR_EFER> {
	union {
		uint64_t raw;
		struct {
			uint64_t SyscallEnable : 1; // 0
			uint64_t DataPrefetchEnable : 1; // 1 [K6]
			uint64_t EWBEControl : 2; // 2-3 [K6]
			uint64_t L2Disable : 1; // 4 [K6]
			uint64_t : 3; // 5-7
			uint64_t LongModeEnable : 1; // 8
			uint64_t : 1; // 9
			uint64_t LongModeActive : 1; // 10
			uint64_t NXEnable : 1; // 11
			uint64_t SVMEnable : 1; // 12
			uint64_t LongModeSegmentLimit : 1; // 13
			uint64_t FastFXSEnable : 1; // 14
			uint64_t TCE : 1; // 15
			uint64_t : 1; // 16
			uint64_t MCOM : 1; // 17
			uint64_t : 46; // 18-63
		};
	};
};

struct MSR_STAR : public MSR<0xC0000081, MSR_STAR> {
	union {
		struct {
			PTR32<> address; // 0x0
			seg_t call_segment; // 0x4
			seg_t ret_segment; // 0x6
		};
		uint64_t raw;
	};
	
};

struct MSR_LSTAR : MSR<0xC0000082, MSR_LSTAR> {
	PTR64<> address;
};

struct MSR_CSTAR : MSR<0xC0000083, MSR_CSTAR> {
	PTR64<> address;
};

struct MSR_SYSCALL_MASK : MSR<0xC0000084, MSR_SYSCALL_MASK> {
	uint64_t mask;
};

struct MSR_FS_BASE : MSR<0xC0000100, MSR_FS_BASE> {
	PTR64<> address;
};

struct MSR_GS_BASE : MSR<0xC0000101, MSR_GS_BASE> {
	PTR64<> address;
};

struct MSR_KERNEL_GS_BASE : MSR<0xC0000102, MSR_KERNEL_GS_BASE> {
	PTR64<> address;
};

struct MSR_TSC_AUX : MSR<0xC0000103, MSR_TSC_AUX> {
	union {
		uint64_t raw;
		struct {
			uint32_t value;
		};
	};
};

struct MSR_LWP_CFG : MSR<0xC0000105, MSR_LWP_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 1; // 0
			uint64_t LwpVAL : 1; // 1 (LWPVAL instruction support)
			uint64_t LwpIRE : 1; // 2 (instructions retired event support)
			uint64_t LwpBRE : 1; // 3 (branch retired event support)
			uint64_t LwpDME : 1; // 4 (DC miss event support)
			uint64_t LwpCNH : 1; // 5 (core clocks not halted event support)
			uint64_t LwpRNH : 1; // 6 (core reference clocks not halted event support)
			uint64_t : 22; // 7-28
			uint64_t LwpCont : 1; // 29 (sampling in continuous mode) [15h-Steamroller]
			uint64_t LwpPTSC : 1; // 30 (performance time stamp counter in event record) [15h-Steamroller]
			uint64_t LwpInt : 1; // 31 (interrupt on threshold overflow)
			uint64_t LwpCoreId : 8; // 32-39
			uint64_t LwpVector : 8; // 40-47 (threshold interrupt vector)
			uint64_t : 16; // 48-63
		};
	};
};

struct MSR_LWP_CBADDR : MSR<0xC0000106, MSR_LWP_CBADDR> {
	uint64_t address;
};

struct MSR_HWCR : MSR<0xC0010015, MSR_HWCR> {
	union {
		uint64_t raw;
		struct {
			uint64_t SMM_lock : 1; // 0
			uint64_t SlowSFENCEEnable : 1; // 1 [10h, 11h, 12h]
			uint64_t : 1; // 2
			uint64_t TLBCacheDisable : 1; // 3
			uint64_t INVDWBINVD : 1; // 4 (Convert INVD to WBINVD)
			uint64_t : 1; // 5
			uint64_t FlushFilterDisable : 1; // 6
			uint64_t AllowFerrOnNe : 1; // 7
			uint64_t IgnneEm : 1; // 8 (Enable emulation of IGNNE port)
			uint64_t MonMwaitDisable : 1; // 9
			uint64_t MonMwaitUserEnable : 1; // 10
			uint64_t LimitCPUIDMax : 1; // 11 [10h, 11h]
			uint64_t HltXSpCycEn : 1; // 12 (halt-exit special bus cycle enable) [10h, 11h, 15h]
			uint64_t SmiSpCycDisable : 1; // 13 (SMI special bus cycle disable)
			uint64_t RsmSpCycDisable : 1; // 14 (RSM special bus cycle disable)
			uint64_t SSEDisable : 1; // 15 [10h, 11h, 12h, 14h]
			uint64_t : 1; // 16
			uint64_t Wrap32Disable : 1; // 17
			uint64_t McStatusWrEnable : 1; // 18 (machine check status write enable)
			uint64_t : 1; // 19
			uint64_t IoCfgGpFault : 1; // 20
			uint64_t LockTscToCurrentP0 : 1; // 21 (MisAlignSseDisable [10h, 11h, 12h, 14h, 16h])
			uint64_t : 1; // 22
			uint64_t ForceUsRdWrSzPrb : 1; // 23 (force probes for upstream RdSized and WrSized) [10h, 11h, 12h, 14h, 15h, 16h]
			uint64_t TscFreqSel : 1; // 24
			uint64_t CpbDis : 1; // 25 (core performance boost disable)
			uint64_t EffFreqCntMwait : 1; // 26 (effective frequency counting during mwait)
			uint64_t EffFreqReadOnlyLock : 1; // 27
			uint64_t SmuLock : 1; // 28 [15h-Excavator]
			uint64_t CSEnable : 1; // 29 (connected standby enable) [15h-Steameroller, 15h-Excavator]
			uint64_t IRPerfEnable : 1; // 30
			uint64_t : 33; // 31-63
		};
	};
};

struct MSR_CPUID_NAME0 : MSR<0xC0010030, MSR_CPUID_NAME0> {
	char name[64];
};
struct MSR_CPUID_NAME1 : MSR<0xC0010031, MSR_CPUID_NAME1> {
	char name[64];
};
struct MSR_CPUID_NAME2 : MSR<0xC0010032, MSR_CPUID_NAME2> {
	char name[64];
};
struct MSR_CPUID_NAME3 : MSR<0xC0010033, MSR_CPUID_NAME3> {
	char name[64];
};
struct MSR_CPUID_NAME4 : MSR<0xC0010034, MSR_CPUID_NAME4> {
	char name[64];
};
struct MSR_CPUID_NAME5 : MSR<0xC0010035, MSR_CPUID_NAME5> {
	char name[64];
};

struct MSR_OSVW_ID_LENGTH : MSR<0xC0010140, MSR_OSVW_ID_LENGTH> {
	union {
		uint64_t raw;
		struct {
			uint16_t length; // 0
		};
	};
};

struct MSR_OSVW_STATUS : MSR<0xC0010141, MSR_OSVW_STATUS> {
	uint64_t raw;
};

struct MSR_AMD_ERRATA_A : MSR<0xC0011000, MSR_AMD_ERRATA_A> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 15; // 0-14
			uint64_t __errata_15h_727 : 1; // 15 (something with cc6 resume) [15h]
			uint64_t __errata_15h_608 : 1; // 16 (something with p-state and interrupts) [15h]
			uint64_t __errata_15h_671 : 1; // 17 (something with misaligned debug breakpoints in DR7 crossing cache lines during a TLB miss) [15h]
		};
	};
};

// Enable/Disable Bits of CPUID[7,0] EBX/EAX
struct MSR_AMD_CPUID7_FEATURES_EX : MSR<0xC0011002, MSR_AMD_CPUID7_FEATURES_EX> {
	union {
		uint64_t raw;
		struct {
			uint32_t ebx_features;
			uint32_t eax_features;
		};
	};
};

// Enable/Disable Bits of CPUID[1] ECX/EDX
struct MSR_AMD_CPUID1_FEATURES : MSR<0xC0011004, MSR_AMD_CPUID1_FEATURES> {
	union {
		uint64_t raw;
		struct {
			uint32_t ecx_features;
			uint32_t edx_features;
		};
	};
};

// Enable/Disable Bits of CPUID[80000001] ECX/EDX
struct MSR_AMD_CPUID1EX_FEATURES : MSR<0xC0011005, MSR_AMD_CPUID1EX_FEATURES> {
	union {
		uint64_t raw;
		struct {
			uint32_t ecx_features;
			uint32_t edx_features;
		};
	};
};

// Load-Store Config 3
struct MSR_AMD_LS_CFG3 : MSR<0xC001101C, MSR_AMD_LS_CFG3> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 7; // 0-6
			uint64_t StreamStoreDisable : 1; // 7 [15h-Excavator]
			uint64_t : 12; // 8-19
			uint64_t DisSpecTlbRld : 1; // 20 (Disable speculative ITLB reloads) [15h-Excavator]
			uint64_t : 2; // 21-22
			uint64_t DisHwPf : 1; // 23 (Disable hardware prefetches) [15h-Excavator]
			uint64_t : 1; // 24
			uint64_t DisPfHwForSw : 1; // 25 (Disable hardware prefetches for software prefetches) [15h-Excavator]
			uint64_t : 38; // 26-63
		};
	};
};

// Load-Store Config
struct MSR_AMD_LS_CFG : MSR<0xC0011020, MSR_AMD_LS_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 4; // 0-3
			uint64_t __errata_17h_1033 : 1; // 4 (something about locked instructions)
			uint64_t : 3; // 5-7
			uint64_t __errata_0Fh_10h_12h_670 : 1; // 8 (something with segment loads)
			uint64_t : 1; // 9
			uint64_t __SSBD_Disable_17h : 1; // 10 (Disable SSBD For Family 17h) [17h]
			uint64_t : 4; // 11-14
			uint64_t __errata_16h_793 : 1; // 15 (something about wc memory and locked instructions)
			uint64_t : 2; // 16-17
			uint64_t __errata_14h_560 : 1; // 18 (something with non-cacheable SSE ops)
			uint64_t : 5; // 19-23
			uint64_t WcPlusEnable : 1; // 24 (write combining plus enable) [16h]
			uint64_t DIS_LS2_SQUISH : 1; // 25 (Disable something with L2 cache, potentially for read-modify-writes crossing pages) [14h, 16h]
			uint64_t DisHitCurPageOpt : 1; // 26 (Disable current table-walk page hit optimization) [16h]
			uint64_t : 1; // 27
			uint64_t StreamStoreDisable : 1; // 28 [14h, 15h-Bulldozer, 15h-Piledriver, 15h-Steamroller]
			uint64_t : 4; // 29-32
			uint64_t __SSBD_Disable_16h : 1; // 33 (Disable SSBD For Family 16h) [16h]
			uint64_t LsCfgDisAgenPick : 1; // 34 (load execution is delayed such that two dependent loads will not be observed during speculation before an early redirect pipeline flush occurs) [Zen2]
			uint64_t : 1; // 35
			uint64_t __errata_14h_530 : 1; // 36 (something with read ordering)
			uint64_t : 8; // 37-44
			uint64_t : 1; // 45
			uint64_t : 5; // 46-50
			uint64_t LsWcFastStringEnable : 1; // 51 (Enable string optimization on destination memtype of WC) [16h]
			uint64_t LsWcPlusFastStringEnable : 1; // 52 (Enable string optimization on destination memtype of WC+) [16h]
			uint64_t : 1; // 53
			uint64_t __SSBD_Disable_15h : 1; // 54 (Disable SSBD For Family 15h) [15h]
			uint64_t : 2; // 55-56
			uint64_t __errata_17h_1095 : 1; // 57 (something about read ordering of locked instructions)
			uint64_t : 6; // 58-63
		};
	};
};

// Instruction Cache Config
struct MSR_AMD_IC_CFG : MSR<0xC0011021, MSR_AMD_IC_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 3; // 0-2
			uint64_t __errata_14h_688 : 1; // 3 (something about branch prediction)
			uint64_t : 5; // 4-8
			uint64_t DisSpecTlbRld : 1; // 9 (Disable speculative ITLB reloads) [10h, 11h, 12h, 14h, 15h, 16h]
			uint64_t : 1; // 10
			uint64_t DIS_SEQ_PREFETCH : 1; // 11 (disable IC sequential prefetch)
			uint64_t : 2; // 12-13
			uint64_t DIS_IND : 1; // 14 (Disable indirect branch predictor) [10h, 11h, 14h]
			uint64_t : 11; // 15-25
			uint64_t DIS_WIDEREAD_PWR_SAVE : 1; // 26 (Disable wide read power mgmt feature for read accesses to IC data array) [16h]
			uint64_t : 12; // 27-38
			uint64_t DisLoopPredictor : 1; // 39 (Disable loop predictor) [15h-Piledriver, 15h-Steamroller]
			uint64_t : 24; // 40-63
		};
		struct {
			uint64_t : 1; // 0
			uint64_t DisIcWayFilter : 4; // 1-4 (disable IC way access filter) [15h-Piledriver]
		};
	};
};

// Data Cache Config
struct MSR_AMD_DC_CFG : MSR<0xC0011022, MSR_AMD_DC_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 3; // 0-2
			uint64_t DIS_CNV_WC_SSO : 1; // 3 (something about streaming stores and WC memory?)
			uint64_t DisSpecTlbRld : 1; // 4 (Disable speculative DTLB reloads) [10h, 12h, 15h-Bulldozer, 15h-Piledriver, 15h-Steamroller]
			uint64_t : 3; // 5-7
			uint64_t DIS_CLR_WBTOL2_SMC_HIT : 1; // 8 (Disable self modifying code checking logic) [10h, 11h, 12h]
			uint64_t : 1; // 9
			uint64_t DIS_SMC_CHK_BUF : 1; // 10 (disable the SelfModifyingCode check buffer for streaming stores)
			uint64_t : 2; // 11-12
			uint64_t DisHwPf : 1; // 13 (Disable hardware prefetches) [10h, 11h, 12h, 14h, 15h-Bulldozer, 15h-Piledriver, 15h-Steamroller, 16h]
			uint64_t : 1; // 14
			uint64_t DisPfHwForSw : 1; // 15 (Disable hardware prefetches for software prefetches) [11h, 15h-Bulldozer, 15h-Piledriver, 15h-Steamroller]
			uint64_t : 8; // 16-23
			uint64_t __errata_10h_261 : 1; // 24 [10h]
			uint64_t : 9; // 25-33
			uint64_t REQ_CTR : 2; // 34-35 (Hardware Prefetch Request Limit) [10h]
			uint64_t : 6; // 36-41
			uint64_t __errata_10h_326_A : 1; // 42 [10h]
			uint64_t __errata_10h_326_B : 1; // 43 [10h]
			uint64_t : 3; // 44-46
			uint64_t __errata_10h_383 : 1; // 47 [10h]
			uint64_t : 16; // 25-63
		};
		struct {
			uint64_t : 4; // 0-3
			uint64_t DisSpecTlbWalk : 1; // 4 (Disable speculative DTLB reloads) [16h]
		};
	};
};

// Bus Unit Config
// Combined Unit Config
struct MSR_AMD_BU_CFG : MSR<0xC0011023, MSR_AMD_BU_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 1; // 0
			uint64_t __errata_0Fh_10h_298 : 1; // 1 [0Fh, 10h]
			uint64_t : 8; // 2-9
			uint64_t DcacheAggressivePriority : 1; // 10 (Prioritize Data Cache Over Instruction Cache) [15h]
			uint64_t : 4; // 11-14
			uint64_t __errata_15h_727 : 1; // 15 [15h]
			uint64_t __errata_15h_608 : 1; // 16 [15h]
			uint64_t __errata_15h_671 : 1; // 17 [15h]
			uint64_t : 3; // 18-20
			uint64_t __errata_0Fh_10h_254 : 1; // 21 [0Fh, 10h]
			uint64_t : 1; // 22
			uint64_t __errata_10h_309 : 1; // 23 [10h]
			uint64_t : 9; // 24-32
			uint64_t __errata_147 : 1; // 33 (something about lock)
			uint64_t WbinvdFlushClean : 1; // 34 (Flush all L2 lines on CC6 entry, including clean lines) [15h-Steamroller]
			uint64_t : 10; // 35-44
			uint64_t WbPfSmcChkDis : 1; // 45
			uint64_t : 2; // 46-47
			uint64_t WbEnhWsbDisable : 1; // 48 (disable multi-stream write combining) [10h, 11h]
			uint64_t ProcFeedbackEn : 1; // 49 (processor feedback interface enable) [15h-Steamroller, 15h-Excavator]
			uint64_t : 1; // 50
			uint64_t EarlyNbDataReturnEnable : 1; // 51 (faster data return from NB to L1 caches if NB returns an entire block of data in contiguous cycles) [15h-Excavator]
			uint64_t : 12; // 52-63
		};
		struct {
			uint64_t : 19; // 0-18
			uint64_t L2FirstLockedWay : 4; // 19-22 (first L2 way locked) [15h]
			uint64_t L2WayLock : 1; // 23 (L2 way lock enable) [15h]
		};
	};
};

// Table Walker Config
struct MSR_AMD_TW_CFG : MSR<0xC0011023, MSR_AMD_TW_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 49; // 0-48
			uint64_t TwCfgCombineCr0Cd : 1; // 49 (combine CR0_CD for both threads of a core) [17h]
			uint64_t : 14; // 50-63
		};
	};
};

// Floating Point Config
struct MSR_AMD_FP_CFG : MSR<0xC0011028, MSR_AMD_FP_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 4; // 0-3
			uint64_t __errata_17h_1049 : 1; // 4 (something about FCOMI/FCMOV)
			uint64_t : 3; // 5-7
			uint64_t DiDtCfg : 8; // 8-15 [16hB]
			uint64_t DiDtMode : 1; // 16 [15h]
			uint64_t : 1; // 17
			uint64_t DiDtCfg0 : 5; // 18-22 [15h]
			uint64_t : 2; // 23-24
			uint64_t DiDtCfg2 : 2; // 25-26 [15h]
			uint64_t DiDtCfg1 : 8; // 27-34 [15h]
			uint64_t : 5; // 35-39
			uint64_t DiDtCfg3 : 1; // 40 [15h-Bulldozer, 15h-Steamroller, 15h-Excavator]
			uint64_t DiDtCfg5 : 1; // 41 [15h-Piledriver, 15h-Steamroller, 15h-Excavator]
			uint64_t DiDtCfg4 : 3; // 42-44 [15h-Piledriver, 15h-Steamroller, 15h-Excavator]
			uint64_t : 19; // 45-64
		};
	};
};

// Decode Config
struct MSR_AMD_DE_CFG : MSR<0xC0011029, MSR_AMD_DE_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t __errata_10h_12h_721 : 1; // 0
			uint64_t LFenceDispatchSerializingEnable : 1; // 1 [10h, 12h, 14h, 15h, 16h, 17h]
			uint64_t PrefetchntaDisable : 1; // 2 [15h]
			uint64_t Prefetcht0Disable : 1; // 3 [15h]
			uint64_t Prefetcht1Disable : 1; // 4 [15h]
			uint64_t Prefetcht2Disable : 1; // 5 [15h]
			uint64_t PrefetchwDisable : 1; // 6 [15h]
			uint64_t PrefetchDisable : 1; // 7 [15h]
			uint64_t : 2; // 8-9
			uint64_t ResyncPredSingleDispDis : 1; // 10 (Disable the single dispatch of loads that hit in the resync predictor) [15h]
			uint64_t : 2; // 11-12
			uint64_t __errata_17h_1021 : 1; // 13 (something about load store ordering) [17h]
			uint64_t : 9; // 14-22
			uint64_t ClflushSerialize : 1; // 23
			uint64_t : 7; // 24-30
			uint64_t __errata_12h_665 : 1; // 31
			uint64_t : 32; // 32-63
		};
	};
};

// Bus Unit Config 2
// Combined Unit Config 2
struct MSR_AMD_BU_CFG2 : MSR<0xC001102A, MSR_AMD_BU_CFG2> {
	union {
		uint64_t raw;
		struct {
			uint64_t L2UpsizeERT : 5; // 0-4 (L2 upsize evict rate threshold) [15h-Steamroller, 15h-Excavator]
			uint64_t : 1; // 5
			uint64_t ThrottleNbInterface : 2; // 6-7 [10h, 15h]
			uint64_t SpecNbReqDisable : 1; // 8 (Disables speculative NB requests) [15h-Bulldozer, 15h-Piledriver, 15h-Steamroller]
			uint64_t L2UpsizeCSWT : 1; // 9 (L2 upsize context switch warmup threshold) [15h-Steamroller, 15h-Excavator]
			uint64_t VicResyncChkEnable : 1; // 10 (Generate an internal probe to NB for nonshared victims. Required for the Monitor/MWait) [15h]
			uint64_t __errata_15h_503 : 1; // 11 (something about APIC task priority) [15h]
			uint64_t : 2; // 12-13
			uint64_t : 1; // 14
			uint64_t ClLinesToNbDisable : 1; // 15 [10h]
			uint64_t : 2; // 16-17
			uint64_t L2UpsizeCSWTEx : 4; // 18-21 (L2 upsize context switch warmup threshold) [15h-Steamroller, 15h-Excavator]
			uint64_t : 2; // 22-23
			uint64_t WcPlusDisable : 1; // 24 (Disable WC+ Memory) [10h]
			uint64_t : 1; // 25
			uint64_t : 3; // 26-28
			uint64_t Smash1GPages : 1; // 29 (1G pages are broken up and installed in the TLB as 2M pages) [10h]
			uint64_t : 5; // 30-34
			uint64_t IcDisSpecTlbWr : 1; // 35 (Speculative writes to the ITLB by the TLB reloader are disabled)
			uint64_t ThrottleNbInterfaceEx : 2; // 36-37 [10h-D+, 15h]
			uint64_t : 4; // 38-41
			uint64_t ProbeFilterSupEnable : 1; // 42 (Enable Probe Filter) [10h-D+, 15h-Bulldozer, 15h-Piledriver, 15h-Steamroller]
			uint64_t : 8; // 43-49
			uint64_t RdMmExtCfgQwEnable : 1; // 50 (read mmio extended config quadword enable) [10h-C+, 12h, 15h]
			uint64_t : 5; // 51-55
			uint64_t L2ClkGatingEn : 1; // 56 (enable L2 clock gating) [12h]
			uint64_t L2HystCnt : 2; // 57-58 (L2 clock gating hysteresis) [12h]
			uint64_t : 6; // 59-63
		};
		struct {
			uint64_t : 15; // 0-14
			uint64_t ClLinesToL2Disable : 1; // 15 (Clean victims and clean forced victims are not sent to the L2) [16h]
			uint64_t : 36; // 16-51
			uint64_t L2UpsizeCUCT : 5; // 52-56 (L2 upsize detector Commited Micro-op Counter Threshold) [15h-Steamroller, 15h-Excavator]
		};
	};
};


// Bus Unit Config 3
// Combined Unit Config 3
struct MSR_AMD_BU_CFG3 : MSR<0xC001102B, MSR_AMD_BU_CFG3> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 3; // 0-2
			uint64_t PfcL1TrainDisable : 1; // 3 (L1 prefetch training disabled) [15h]
			uint64_t : 12; // 4-15
			uint64_t PfcRegionDisable : 1; // 16 (Region prefetch generation disabled) [15h]
			uint64_t PfcStrideDisable : 1; // 17 (Stride prefetch generation disabled) [15h]
			uint64_t PfcDis : 1; // 18 (Prefetcher disabled) [15h]
			uint64_t : 1; // 19
			uint64_t PfcStrideMul : 2; // 20-21 (number of stride offsets that are prefetched + 3) [15h-Piledriver, 15h-Steamroller]
			uint64_t PfcDoubleStride : 1; // 22 (Prefetch N and N+1 offsets ahead of a stride miss instead of just N) [15h-Piledriver, 15h-Steamroller]
			uint64_t : 26; // 23-41
			uint64_t PwcDisableWalkerSharing : 1; // 42 (Page table walker sharing is disabled) [15h]
			uint64_t : 6; // 43-48
			uint64_t CombineCr0Cd : 1; // 49 (combine CR0[CD] for both cores of a compute unit) [15h]
			uint64_t AsidIncrScaleFactorOld : 1; // 50 (toggles between 16/64 per ASID replace) [15h-Bulldozer]
			uint64_t AsidDecrScaleFactor : 2; // 51-52 (toggles between 16/32/64/128 per ASID replace) [15h]
			uint64_t : 2; // 53-54
			uint64_t AsidIncrScaleFactor : 2; // 55-56 (toggles between 16/32/64/128 per ASID replace) [15h-Steamroller, 15h-Excavator]
			uint64_t PcidIncrScaleFactor : 2; // 57-58 (toggles between 16/32/64/128 per PCID replace) [15h-Steamroller, 15h-Excavator]
			uint64_t PcidDecrScaleFactor : 2; // 59-60 (toggles between 16/32/64/128 per PCID replace) [15h-Steamroller, 15h-Excavator]
			uint64_t : 3; // 61-63
		};
	};
};

// Execution Unit Config
struct MSR_AMD_EX_CFG : MSR<0xC001102C, MSR_AMD_EX_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 51; // 0-50
			uint64_t __errata_17h_1100 : 1; // 51 (something about fused branch page faults)
			uint64_t : 2; // 52-53
			uint64_t LateSbzResync : 1; // 54 (Force an SBZ (Shift-By-Zero) resync to wait if the op has a store component which has not yet completed) [15h-Piledriver]
			uint64_t : 9; // 55-64
		};
	};
};

// Load-Store Config 2
struct MSR_AMD_LS_CFG2 : MSR<0xC001102D, MSR_AMD_LS_CFG2> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 12; // 0-11
			uint64_t ForceBusLockDisable : 1; // 12 (Disable widget that forces cacheable locks to be bus locks) [15h-Bulldozer]
			uint64_t : 1; // 13
			uint64_t ForceSmcCheckFlwStDisable : 1; // 14 (Do not perform a self modifying code check when a cache probe hits a store that has not retired) [15h]
			uint64_t : 8; // 15-22
			uint64_t DisScbThreshold : 1; // 23 (Disable SCB threshold and force deallocation) [15h]
			uint64_t : 10; // 24-33
			uint64_t __errata_17h_1091 : 1; // 34 (something about loads/stores crossing 4k boundaries)
			uint64_t : 29; // 35-63
		};
	};
};

// Combined Unit Prefetch Throttling Config
struct MSR_AMD_CU_PFTCFG : MSR<0xC001102F, MSR_AMD_CU_PFTCFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t CaptureThreshold : 6; // 0-5 (prefetch throttling capture threshold) [15h-Steamroller, 15h-Excavator]
			uint64_t TrackThreshold : 6; // 6-11 (prefetch throttling tracking threshold) [15h-Steamroller, 15h-Excavator]
			uint64_t AccThresh0 : 6; // 12-17 (accuracy threshold level 0) [15h-Steamroller, 15h-Excavator]
			uint64_t AccThresh1 : 6; // 18-23 (accuracy threshold level 1) [15h-Steamroller, 15h-Excavator]
			uint64_t AccThresh2 : 6; // 24-29 (accuracy threshold level 2) [15h-Steamroller, 15h-Excavator]
			uint64_t ThrottleLevel : 6; // 30-35 (throttling level) [15h-Steamroller, 15h-Excavator]
			uint64_t PrefetchThrottlingEnable : 1; // 36 [15h-Steamroller, 15h-Excavator]
			uint64_t : 27; // 37-63
		};
	};
};

// L2I Configuration
struct MSR_AMD_L2I_CFG : MSR<0xC00110A0, MSR_AMD_L2I_CFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 3; // 0-2
			uint64_t CacheIcAttrDisable : 1; // 3 (Disable inserting IC attributes into the L2) [16h]
			uint64_t : 3; // 4-6
			uint64_t PrefetcherDisable : 1; // 7 (Disable the L2 prefetcher) [16h]
			uint64_t : 11; // 8-18
			uint64_t McaToMstCoreEnable : 1; // 19 (route machine check events to master Core) [16h]
			uint64_t L2RinserDisable : 1; // 20 [16h]
			uint64_t L2ScrubberDisable : 1; // 21 [16h] (Do not search for and repair single bit errors in the background) [16h]
			uint64_t : 18; // 22-39
			uint64_t ThrottleNbInterface : 4; // 40-43 (NB Interface Request Throttling) [16h]
			uint64_t PbDisObeysThrottleNb : 1; // 44 (probes disobey NB throttling) [16h]
			uint64_t L2ScrubberInterval : 12; // 45-56 (L2 background scrubber interval) [16h]
			uint64_t : 7; // 57-63
		};
	};
};

// Combined Unit Contention Blocking Buffer Control
struct MSR_AMD_CU_CBBCFG : MSR<0xC00110A1, MSR_AMD_CU_CBBCFG> {
	union {
		uint64_t raw;
		struct {
			uint64_t MasterEnable : 1; // 0 (Enable CBB feature) [15h-Excavator]
			uint64_t CBBCoreBlockingEnable : 1; // 1 (Allows CBB entries to block core requests in addition to probes) [15h-Excavator]
			uint64_t CBBCLockHandlingEnable : 1; // 2 (Enables the CBB features that treat cache locks differently than other request types) [15h-Excavator]
			uint64_t : 1; // 3
			uint64_t AllocCBBForLsFill : 1; // 4 (Allows CBB entries to be allocated by LS FILL requests) [15h-Excavator]
			uint64_t AllocCBBForWccFill : 1; // 5 (Allows CBB entries to be allocated by WCC FILL requests) [15h-Excavator]
			uint64_t AllocCBBForPFLock : 1; // 6 (Allows CBB entries to be allocated by PFLOCK requests) [15h-Excavator]
			uint64_t AllocCBBForCLOCK : 1; // 7 (Allows CBB entries to be allocated by CLOCK requests) [15h-Excavator]
			uint64_t AllocCBBForExclusive : 1; // 8 (Allows CBB entries to be allocated on M/Muw/E data returns) [15h-Excavator]
			uint64_t AllocCBBForShared : 1; // 9 (Allows CBB entries to be allocated on O/S data returns) [15h-Excavator]
			uint64_t TimeoutValueForCBBAlloc : 3; // 10-12 (toggles between 32/64/96/128/160/192/224/256 cycles) [15h-Excavator]
			uint64_t TimeoutValueForCBBCLock : 3; // 13-15 (toggles between 32/64/96/128/160/192/224/256 cycles) [15h-Excavator]
			uint64_t : 48; // 16-63
		};
	};
};

// Decode Config 2
struct MSR_AMD_DE_CFG2 : MSR<0xC00110E3, MSR_AMD_DE_CFG2> {
	union {
		uint64_t raw;
		struct {
			uint64_t : 1; // 0
			uint64_t SuppressBPOnNonBr : 1; // 1 (branch prediction information on non-branch instructions is ignored) [Zen2]
			uint64_t : 62; // 2-63
		};
	};
};

#endif