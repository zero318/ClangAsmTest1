# CMakeList.txt : CMake project for ClangAsmTest1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ClangAsmTest1")

add_executable(${PROJECT_NAME})
add_executable(EoSDTest1)
add_executable(PCBTest1)
add_executable(INTest1)
add_executable(MoFTest1)
add_executable(UMTest1)
add_executable(FourOfAKind1)
add_executable(tts_test1)
add_executable(print_copypasta)
add_executable(ntoskrnl_tests)
add_executable(winload_tests)
add_library(CSoundLib STATIC)

set(programs
${PROJECT_NAME}
EoSDTest1
PCBTest1
INTest1
MoFTest1
UMTest1
FourOfAKind1
CSoundLib
tts_test1
print_copypasta
ntoskrnl_tests
winload_tests
)

enable_language(ASM-ATT)
add_library(AsmTemp OBJECT
	"asm_test.asm"
)

target_compile_options(AsmTemp PRIVATE
	-march=bdver3
)

#set(CMAKE_CXX_STANDARD 20)

#add_library(DummyDLL SHARED
#	"DummyDLL/dummy_dll.cpp"
#)
#target_link_options(DummyDLL PRIVATE
#	/fixed
#	/base:0x400000
#	/DLL
#)

if(${CLANG_TYPE} STREQUAL "CLANG-CL")
set(SLASH_CLANG_ARG "/clang:")
else()
set(SLASH_CLANG_ARG "")
endif()

foreach(EXE ${programs})

#set_target_properties(${EXE} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

if(IS_X64)
target_compile_definitions(${EXE} PRIVATE
	IS_X64
)
endif()

#if(MSVC)
# Deal with MSVC warnings
target_compile_definitions(${EXE} PRIVATE
	# SHUT UP ABOUT MEMCPY BEING UNSECURE I DON'T CARE
	_CRT_SECURE_NO_DEPRECATE
	# POSIX functions are nice
	_CRT_NONSTDC_NO_DEPRECATE
	# Allow POSIX functions without leading underscores everywhere
	_CRT_DECLARE_NONSTDC_NAMES
	_ENABLE_EXTENDED_ALIGNED_STORAGE
	__clang_major__=16
	__analysis_assume=__assume
	_Analysis_assume_=__assume
	_HAS_EXCEPTIONS=0
)
if(IS_X64)
target_compile_definitions(${EXE} PRIVATE
	_STD_ATOMIC_ALWAYS_USE_CMPXCHG16B=1
)
endif()
#endif()

# Library related defines
target_compile_definitions(${EXE} PRIVATE
	# Omit some extraneous BS from Windows.h	
	WIN32_LEAN_AND_MEAN
	# Define PI and other constants in math.h
	_USE_MATH_DEFINES
	# Default CRT rand implementation sucks balls
	_CRT_RAND_S
	# More float types are cool
	__STDC_WANT_IEC_60559_TYPES_EXT__
	ENABLE_RESTRICTED
	NOMINMAX
	_FORCENAMELESSUNION
	#__CLANG_STDATOMIC_H
	#_CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY
	#ZERO_TRAP_AND_EMULATE
	_Float16=float
	__bf16=float
	__AVX512FP16INTRIN_H
	__AVX512VLFP16INTRIN_H
	__AVX512BF16INTRIN_H
	__AVX512VLBF16INTRIN_H
	__AVXNECONVERTINTRIN_H
)

# Language Standards
target_compile_options(${EXE} PRIVATE
	# Enable C23 Features
	#/std:c++latest
	"SHELL:-Xclang -std=c2x"
	# Enable C++20 Features
	"SHELL:-Xclang -std=c++2b"

	#"SHELL:-Xclang -fmodules"
)
target_compile_definitions(${EXE} PRIVATE
	#__STDC_VERSION__=202000L
	#__cplusplus=202101L
)

# CPU Feature flags
target_compile_options(${EXE} PRIVATE
	#"-mno-avx"
	#"-mno-bmi"
	#"-msse4.1"
	#"-mno-sse4.1"
	#"-mno-ssse3"
	#"-mno-sse3"

	#"-mno-xop"
	#"-mno-tbm"
	#"-mno-sse4a"
	#"-mno-fma4"
	#"-mno-fma"
	#"SHELL:-Xclang -mprefer-vector-width=128"
)

# Warning Suppression
target_compile_options(${EXE} PRIVATE
	-Wno-c99-designator
	-Wno-c++11-narrowing
	-Wno-c++17-extensions
	-Wno-c++2b-extensions
	-Wno-unused-function
	-Wno-unused-const-variable
	-Wno-unused-variable
	-Wno-unused-but-set-variable
	-Wno-unused-label
	-Wno-missing-braces
	#-Winvalid-token-paste
	-Wno-global-isel
	-Wno-logical-op-parentheses
	-Wno-bitwise-op-parentheses
	-Wno-bitfield-width
	-Wno-pointer-to-int-cast
	-Wno-int-to-pointer-cast
	-Wno-format-security
	-Wno-shift-op-parentheses
	-Wno-gnu-alignof-expression
	-Wno-microsoft-goto
)

# Output Flags
target_compile_options(${EXE} PRIVATE
	# Emit extra info in generated assembly
	${SLASH_CLANG_ARG}-fverbose-asm
	#"SHELL:-Xclang -v"
)

# Compatibility Flags
target_compile_options(${EXE} PRIVATE
	# Allow GNU keywords
	"SHELL:-Xclang -fgnu-keywords"

	# Processor Features to Allow
	-march=bdver3
	#"SHELL:-Xclang -mtune=bdver3"

	# Attempt at forcing x87 80-bit long doubles
	"SHELL:-Xclang -mlong-double-80"
	#${SLASH_CLANG_ARG}-mlong-double-80

	# Allow nested functions
	#${SLASH_CLANG_ARG}-fnested-functions
	#"SHELL:-Xclang -fnested-functions"

	# Enable int128_t because why not
	"SHELL:-Xclang -fforce-enable-int128"
	${SLASH_CLANG_ARG}-ffixed-point

	${SLASH_CLANG_ARG}-fenable-matrix

	# Inline assembly dialect
	#/clang:-masm=intel
)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	# Optimization Flags
	if(SIZE_OPTIMIZE)
	target_compile_options(${EXE} PRIVATE
		# Optimization level
		"SHELL:-Xclang -Oz"
		${SLASH_CLANG_ARG}-falign-functions=16
		${SLASH_CLANG_ARG}-finline-functions
	)
	else()
	target_compile_options(${EXE} PRIVATE
		# Optimization level
		"SHELL:-Xclang -O3"
	)
	endif()
	target_compile_options(${EXE} PRIVATE
		${SLASH_CLANG_ARG}-fglobal-isel

		# Treat signed integer overflow as two’s complement
		${SLASH_CLANG_ARG}-fwrapv

		# Omit frame pointers
		#"SHELL:-Xclang -fomit-frame-pointer"
		# Omit frame pointer setup for leaf functions
		#-momit-leaf-frame-pointer

		#${SLASH_CLANG_ARG}-fstack-reuse=all

		#"SHELL:-Xclang -fstack-reuse=all"
		${SLASH_CLANG_ARG}-foptimize-sibling-calls

		# Try to skip constructors writing to memory
		${SLASH_CLANG_ARG}-felide-constructors

		# Merge constant values
		-fmerge-all-constants
		# Merge globals
		"SHELL:-Xclang -mglobal-merge"
		
		#"SHELL:-Xclang -mmemops"

		# Enable loop vectorization
		#"SHELL:-Xclang -fvectorize"

		# Return small structs in registers
		"SHELL:-Xclang -freg-struct-return"

		#-fpcc-struct-return

		# Only allocate as many bytes as an enum needs
		#"SHELL:-Xclang -fshort-enums"

		# Unroll loops
		"SHELL:-Xclang -funroll-loops"
		"SHELL:-Xclang -freroll-loops"

		#-falign-loops
	)
	if(MSVC)
		target_compile_options(${EXE} PRIVATE
			# Enable more vectorization
			/Qvec
		)
	endif()
endif()

# Security Optimization Flags
target_compile_options(${EXE} PRIVATE
	#-fdelete-null-pointer-checks
	#-fno-delete-null-pointer-checks
	
	# Disable more stack checking
	#"SHELL:-Xclang -fno-stack-protector"
	${SLASH_CLANG_ARG}-fno-stack-protector
)
if(MSVC)
	target_compile_options(${EXE} PRIVATE
		# Disable buffer checking
		/GS-

		# Disable stack cookies
		/Gs-

		#/clang:-fno-exceptions
		/EHsc

		# Don't use TLS for function scope statics
		/Zc:threadSafeInit-
	)
endif()

target_compile_options(${EXE} PRIVATE
	#-flto=thin
	-flto=full
)

target_compile_options(${EXE} PRIVATE
	${SLASH_CLANG_ARG}-fvectorize
	${SLASH_CLANG_ARG}-ftree-slp-vectorize

	${SLASH_CLANG_ARG}-fno-fine-grained-bitfield-accesses

	#"SHELL:-mllvm -slp-vectorize-hor-store=true"
	#"SHELL:-mllvm -slp-threshold=-1"

	#"SHELL:-mllvm -enable-interleaved-mem-accesses=true"
	#"SHELL:-mllvm -enable-masked-interleaved-mem-accesses=true"
	#"SHELL:-mllvm -enable-loop-distribution"

	#"SHELL:-mllvm -disable-licm-promotion=true"

	# Enable screwy register allocations
	#"SHELL:-mllvm -enable-ipra=true"
	# ???
	#"SHELL:-mllvm -disable-ssc=true"
	#"SHELL:-mllvm -optimize-regalloc=true"
	#"SHELL:-mllvm -enable-merge-functions=true"
	#"SHELL:-mllvm -force-hardware-loops=true"
	"SHELL:-mllvm -extra-vectorizer-passes=true"
	#"SHELL:-mllvm -enable-split-machine-functions=true"


	"SHELL:-mllvm -x86-cmov-converter-force-mem-operand=false"
	#"SHELL:-mllvm -x86-use-fsrm-for-memcpy=true"
	#"SHELL:-mllvm -x86-use-vzeroupper=false"
	#"SHELL:-mllvm -disable-x86-lea-opt=true"
	#"SHELL:-Xclang -fslp-vectorize"
)

target_compile_options(${EXE} PRIVATE
	#-menable-unsafe-fp-math
	#-funsafe-math-optimizations
	#-freciprocal-math
	#-fapprox-func
	#-frounding-math

	#"SHELL:-Xclang -malign-branch-boundary=16"
	${SLASH_CLANG_ARG}-malign-branch-boundary=16

	#-fvirtual-function-elimination
	"SHELL:-Xclang -fdata-sections"
	"SHELL:-Xclang -ffunction-sections"
	
	"SHELL:-Xclang -ffp-contract=fast"
	"SHELL:-Xclang -ffast-math"

	#"SHELL:-Xclang -fassume-sane-operator-new"
	${SLASH_CLANG_ARG}-fnew-infallible
	#-fsized-deallocation

	${SLASH_CLANG_ARG}-mstack-probe-size=93284238
	${SLASH_CLANG_ARG}-ffinite-loops

	#-fhonor-infinities
	#-fhonor-nans
	#-fsigned-zeros

	${SLASH_CLANG_ARG}-flax-vector-conversions=all

	${SLASH_CLANG_ARG}-fstrict-flex-arrays=1

	#${SLASH_CLANG_ARG}-fthreadsafe-statics

	${SLASH_CLANG_ARG}-mno-ms-bitfields
	#"SHELL:-Xclang -mno-ms-bitfields"
)

target_link_options(${EXE} PRIVATE
	"LINKER:--allow-multiple-definition"
)

target_compile_options(${EXE} PRIVATE
	#"SHELL:-fuse-ld=lld-link"
	#"-fuse-ld=\"F:\\Program Files (x86)\\LLVM\\bin\\ld.lld.exe\""
)

#set(CMAKE_LINKER "F:\\Program Files (x86)\\LLVM\\bin\\ld.lld.exe")

#include_directories("F:\\Program Files (x86)\\LLVM\\lib\\clang\\12.0.0\\include")
#include_directories("F:\\Program Files\\LLVM\\lib\\clang\\13.0.0\\include")
#include_directories("F:\\Program Files\\LLVM\\lib\\clang\\14.0.0\\include")
#include_directories(SYSTEM "F:\\Program Files\\LLVM\\lib\\clang\\15.0.2\\include")
include_directories(BEFORE SYSTEM "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\zero_include")
include_directories(SYSTEM "F:\\Program Files\\LLVM\\lib\\clang\\16\\include")
include_directories(SYSTEM "C:\\WinDDK\\7600.16385.1\\inc\\ddk")
include_directories(SYSTEM "F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT")
include_directories(SYSTEM "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Include")
include_directories(SYSTEM "C:\\Program Files (x86)\\Microsoft Speech SDK\\Include")
if(NOT MSVC)
include_directories(AFTER SYSTEM
	"F:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.29.30133\\ATLMFC\\include"
	"F:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.29.30133\\include"
	"C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.8\\include\\um"
	"C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.20262.0\\ucrt"
	"C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.20262.0\\shared"
	"C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.20262.0\\um"
	"C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.20262.0\\winrt"
	"C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.20262.0\\cppwinrt"
)
endif()
#add_library(NT "C:\\Windows\\SysWOW64\\ntdll.dll")

#target_link_libraries(${EXE} NT)
#target_link_libraries(${EXE} "ntdll.lib" "msvcrt.lib")
#target_link_libraries(${EXE} "C:\\WinDDK\\7600.16385.1\\lib\\wxp\\i386\\ntdll.lib")
target_link_libraries(${EXE} "C:\\WinDDK\\7600.16385.1\\lib\\wxp\\i386\\winmm.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\dsound.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\dinput8.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\dxguid.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\d3d9.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\d3dx9.lib")
#target_link_libraries(${EXE} DummyDLL)

# Add source to this project's executable.
target_sources(${EXE} PRIVATE
	#"ClangAsmTest1.cpp"
	#"ClangAsmTest1.h"

	#"util.h"
	"zero/force_macros.h"
	"zero/util.h"
	"zero/util.cpp"
	#"zero/ExternalGlobal.h"
	"zero/custom_intrin.h"
	"zero/thcrap_util.h"
	"zero/LinkerCheeseValues.h"
	"zero/BoundingBox.h"
	#"zero/trap_and_emulate.cpp"
	"zero/func_traits.h"
	"zero/compiler_support.h"
)
if (STATIC_CSOUND)
target_sources(${EXE} PRIVATE
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional\\SDKSound.cpp"
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional\\SDKwavefile.cpp"
)
set_source_files_properties("F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional\\SDKSound.cpp" PROPERTIES COMPILE_FLAGS "-DUNICODE")
set_source_files_properties("F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional\\SDKwavefile.cpp" PROPERTIES COMPILE_FLAGS "-DUNICODE")
target_include_directories(${EXE} SYSTEM PRIVATE
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Core"
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional"
)
target_compile_definitions(CSoundLib PRIVATE
	UNICODE
)
endif()

endforeach()


#add_library(NT STATIC IMPORTED)
#SET_PROPERTY(TARGET NT PROPERTY IMPORTED_LOCATION "C:\\WinDDK\\7600.16385.1\\lib\\win7\\amd64\\ntoskrnl.lib")
#target_link_libraries(${PROJECT_NAME} NT)

if(NOT STATIC_CSOUND)
target_sources(CSoundLib PRIVATE
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional\\SDKSound.cpp"
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional\\SDKwavefile.cpp"
)
target_include_directories(CSoundLib SYSTEM PRIVATE
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Core"
	"F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT\\Optional"
)
target_compile_definitions(CSoundLib PRIVATE
	UNICODE
)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
	#-fforce-emit-vtables
)
target_sources(${PROJECT_NAME} PRIVATE
	#"ClangAsmTest1.cpp"
	#"ClangAsmTest1.h"
	#"util.h"
	#"NewEoSDECL.cpp"
	#"zero/util.h"
	#"zero/util.cpp"
	#"ExternalGlobal.h"
	#"RNG_helpers.h"
	#"custom_intrin.h"
	#$<TARGET_OBJECTS:AsmTemp>
    #"zero/LinkerCheeseValues.h"
	#"zero/BoundingBox.h"
	
	#"NewUMECL.cpp"
	#"UM_bullet_ex.cpp"
	
	#"NewMoFECL.cpp"
	"low_mem_alloc.cpp"
	"low_mem_alloc.h"
	"custom_printf.cpp"

	"ClangAsmTest1.cpp"
	"ClangAsmTest1.h"
	"windows_crap.h"
	#"windows_jank.cpp"
	"unicode_test.cpp"
	"rand_float_test.cpp"
	"reduce_angle_fast.h"
	"pe_test.cpp"
	#"inject_test.cpp"
	"olly_dbg_cmd_dump.cpp"
	"sha256_thcrap.cpp"
	"sha256_new.cpp"
	"bson_test.cpp"
	"zson_test.cpp"
	"zero/city_hash.h"
	"zero/city_hash.cpp"
	"frame_pointer_test.cpp"
	"json_test.cpp"
	#"json5_test.cpp"
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
	USE_AMD_FEATURES
)

add_library(ntoskrnl_test_asm OBJECT
	"windows_kernel_test.asm"
)
target_sources(ntoskrnl_tests PRIVATE
	"dummy_main.cpp"
	$<TARGET_OBJECTS:ntoskrnl_test_asm>
	"x86_msrs.h"
	"F:/My Hacking Stuff Expansion/windows/ntoskrnl_structs.h"
	"windows_kernel_test.cpp"
)
if(IS_X64)
target_compile_options(ntoskrnl_tests PRIVATE
	-march=x86-64
	-mprfchw
	-msahf
)
else()
target_compile_options(ntoskrnl_tests PRIVATE
	-march=pentium4
)
endif()


#add_library(winload_test_asm OBJECT
	#"windows_kernel_test.asm"
#)
target_sources(winload_tests PRIVATE
	"dummy_main.cpp"
	#$<TARGET_OBJECTS:winload_test_asm>
	"x86_msrs.h"
	"F:/My Hacking Stuff Expansion/windows/ntoskrnl_structs.h"
	"winload_test.cpp"
)
if(IS_X64)
target_compile_options(winload_tests PRIVATE
	-march=x86-64
	-mprfchw
	-msahf
)
else()
target_compile_options(winload_tests PRIVATE
	-march=pentium4
)
endif()

add_library(EoSDAsm OBJECT
	"asm_test.asm"
)
target_compile_options(EoSDAsm PRIVATE
	-march=bdver3
)
target_sources(EoSDTest1 PRIVATE
	"dummy_main.cpp"
	$<TARGET_OBJECTS:EoSDAsm>
	"EoSD/NewEoSDECL.cpp"
	"x87_math_helpers.h"
	"x87_math_helpers.cpp"
)
target_compile_options(EoSDTest1 PRIVATE
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
)

#add_library(PCBAsm OBJECT
	#"asm_test.asm"
#)
#target_compile_options(PCBAsm PRIVATE
	#-march=bdver3
#)
target_sources(PCBTest1 PRIVATE
	"dummy_main.cpp"
	#$<TARGET_OBJECTS:PCBAsm>
	"PCB/pcb_enemy_dump.cpp"
)
target_compile_options(PCBTest1 PRIVATE
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
)

#add_library(INAsm OBJECT
	#"asm_test.asm"
#)
#target_compile_options(INAsm PRIVATE
	#-march=bdver3
#)
target_sources(INTest1 PRIVATE
	"dummy_main.cpp"
	#$<TARGET_OBJECTS:INAsm>
	"IN/test.cpp"
)
target_compile_options(INTest1 PRIVATE
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
)

#add_library(MoFAsm OBJECT
	#"asm_test.asm"
#)
#target_compile_options(MoFAsm PRIVATE
	#-march=bdver3
#)
target_sources(MoFTest1 PRIVATE
	"dummy_main.cpp"
	#$<TARGET_OBJECTS:MoFAsm>
	"MoF/NewMoFECL.cpp"
)
target_compile_options(MoFTest1 PRIVATE
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
)

add_library(UMAsm OBJECT
	"UM/globals.asm"
)
target_compile_options(UMAsm PRIVATE
	-march=bdver3
)
target_sources(UMTest1 PRIVATE
	#"dummy_main.cpp"
	$<TARGET_OBJECTS:UMAsm>
	"UM/UM_bullet_ex.cpp"
	"zero/libm_test.h"
	"zero/libm_test.cpp"
)
target_compile_definitions(UMTest1 PRIVATE
	NO_ASM_SYMBOLS
	#ZUNListIdkDefault=true
)
target_compile_options(UMTest1 PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	-Rpass=.*vectorize.*
	-Rpass-missed=.*vectorize.*
	-Rpass-analysis=.*vectorize.*
)
if(MSVC)
	target_compile_options(UMTest1 PRIVATE
		/Gv
		#/MT
	)
endif()
target_link_options(UMTest1 PRIVATE
	"LINKER:/subsystem:windows"
	"LINKER:/fixed"
	"LINKER:/base:0x400000"
	#/VERBOSE:LIB
)

target_link_libraries(UMTest1 "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\dsound.lib")
if(NOT STATIC_CSOUND)
target_link_libraries(UMTest1 CSoundLib)
endif()


add_library(FourOfAKindAsm OBJECT
	"FoaK/asm_defs.asm"
)
target_sources(FourOfAKind1 PRIVATE
	"dummy_main.cpp"
	"FoaK/patch_funcs.cpp"
	$<TARGET_OBJECTS:FourOfAKindAsm>
)
target_compile_options(FourOfAKind1 PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-sse3"
)

target_sources(tts_test1 PRIVATE
	"tts_test.cpp"
)

target_sources(print_copypasta PRIVATE
	"copypasta_printer.cpp"
)
target_compile_options(print_copypasta PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)

#target_link_libraries(UMTest1 "F:\\Touhou_Stuff_2\\disassembly_stuff\\18\\crack\\th18_fake_dll.dll")

#set_source_files_properties(NewMoFECL.cpp PROPERTIES COMPILE_FLAGS "-mno-sse -Xclang -menable-unsafe-fp-math")

#add_dependencies(AsmTemp ${PROJECT_NAME})

set_source_files_properties(x87_math_helpers.h PROPERTIES COMPILE_FLAGS "-mno-sse -mno-sse2")
set_source_files_properties(x87_math_helpers.cpp PROPERTIES COMPILE_FLAGS "-mno-sse -mno-sse2")
set_source_files_properties(frame_pointer_test.cpp PROPERTIES COMPILE_FLAGS ${SLASH_CLANG_ARG}-fno-omit-frame-pointer)
#set_source_files_properties(RNG_helpers.h PROPERTIES COMPILE_FLAGS "-mno-sse -mno-sse2")

# TODO: Add tests and install targets if needed.
