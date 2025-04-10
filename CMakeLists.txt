# CMakeList.txt : CMake project for ClangAsmTest1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#set(CMAKE_CXX_STANDARD 23)

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
add_executable(winhttp_test3)
add_executable(smallest_pe_file)
add_executable(ghidra_launch)
add_executable(crappy_launcher)
add_executable(udoalg_test)
add_executable(3d_flying_objects)
add_executable(discord_test)
add_executable(AoCFTest1)
add_executable(loaddll)
add_executable(force_load_pe)
add_executable(znet_client_test)
add_executable(znet_server_test)
add_executable(znet_p2p_test)
add_executable(make_embed)
add_executable(aocf_server)
add_executable(run_command)
add_executable(aocf_plugin_patch)
add_executable(unix_socket_test)
add_executable(z8086_old)
add_executable(z86_emulator)
add_executable(ttt_no_mem)
add_executable(libm_testing)
add_executable(vpatch)
add_executable(heroes_calculator)
add_library(z8086 SHARED)
add_library(XInput1_4 SHARED)
add_library(winhttp SHARED)
add_library(winhttp_websocket SHARED)
add_library(CSoundLib STATIC)
add_library(wow64 SHARED)
add_library(wow64cpu SHARED)
add_library(wow64win SHARED)
add_library(parakit_inject SHARED)
add_library(python_test SHARED)
add_library(glaux STATIC)
add_library(glcommon STATIC)
#add_library(twitch_irc_test SHARED)
add_executable(twitch_irc_test)
add_executable(youtube_chat_test)
add_library(x64_inject_test SHARED)
add_executable(x64_injector)
add_library(aocf_netcode SHARED)
add_library(Netplay SHARED)
add_library(dbghelp SHARED)
add_library(_dbghelp SHARED)
add_library(vpatch_dll SHARED)

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
XInput1_4
winhttp
winhttp_websocket
winhttp_test3
wow64
wow64cpu
wow64win
parakit_inject
smallest_pe_file
ghidra_launch
crappy_launcher
udoalg_test
python_test
glaux
glcommon
3d_flying_objects
discord_test
twitch_irc_test
youtube_chat_test
AoCFTest1
loaddll
x64_inject_test
x64_injector
force_load_pe
aocf_netcode
Netplay
znet_client_test
znet_server_test
znet_p2p_test
make_embed
aocf_server
run_command
aocf_plugin_patch
unix_socket_test
z8086_old
z86_emulator
z8086
ttt_no_mem
libm_testing
dbghelp
_dbghelp
vpatch
vpatch_dll
heroes_calculator
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
set(COMPILE_DLL "/DLL")
set(MSLINK_ARG "")
set(NOSTDLIB_LINK "-nostartfiles -nodefaultlib")
else()
set(SLASH_CLANG_ARG "")
set(COMPILE_DLL "-shared")
set(MSLINK_ARG "-Wl")
set(NOSTDLIB_LINK "")
endif()

foreach(EXE ${programs})

#set_target_properties(${EXE} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

if(IS_X64)
target_compile_definitions(${EXE} PRIVATE
	IS_X64
)
endif()

if(${CLANG_TYPE} STREQUAL "CLANG-CL")
target_compile_definitions(${EXE} PRIVATE
	CLANG_CL=1
)
else()
target_compile_definitions(${EXE} PRIVATE
	CLANG_CL=0
)
endif()

if(CUSTOM_CLANG)
target_compile_definitions(${EXE} PRIVATE
	CUSTOM_CLANG=1
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
	#__clang_major__=17
	__analysis_assume=__assume
	_Analysis_assume_=__assume
	_HAS_EXCEPTIONS=0
	_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

	# Clang loves complaining about deprecated
	# things that I'm not even using
	_WINSOCK_DEPRECATED_NO_WARNINGS

	# IDK why this started not getting defined, but this fixes it
	POINTER_64=__ptr64
	# For some reason cdecl keeps getting itself undef'ed and this prevents it
	DOSWIN32=1
	_LDSUPPORT=1

	DONT_COMPILE_REGISTER_ASSUME_JANK=1
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

	#_MSVC_LANG=202003L # Hopefully this is fine?
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
	"-mno-avx"
	"-mno-bmi"
	#"-msse4.1"
	"-mno-sse4.1"
	"-mno-ssse3"
	"-mno-sse3"
	"-mno-lzcnt"

	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-fma"
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
	-Wno-missing-declarations
	-Wno-initializer-overrides
	-Wno-microsoft-template
	-Wno-deprecated-this-capture
	-Wno-microsoft-extra-qualification
	#-Wno-dynamic-class-memaccess
	-Wno-inline-namespace-reopened-noninline
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
	target_compile_definitions(${EXE} PRIVATE
		SIZE_OPTIMIZE
	)
	else()
	target_compile_options(${EXE} PRIVATE
		# Optimization level
		"SHELL:-Xclang -O3"
	)
	endif()
	target_compile_options(${EXE} PRIVATE
		#${SLASH_CLANG_ARG}-fglobal-isel

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
		#"SHELL:-Xclang -freroll-loops"

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
	${SLASH_CLANG_ARG}-fcf-protection=none
	${SLASH_CLANG_ARG}-fno-sanitize-cfi-cross-dso
	#${SLASH_CLANG_ARG}-mguard=none
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

#if(${CLANG_TYPE} STREQUAL "CLANG-CL")
#else()
target_compile_options(${EXE} PRIVATE
	#-flto=thin
	#-flto=full
)
#endif()

target_compile_options(${EXE} PRIVATE
	${SLASH_CLANG_ARG}-fvectorize
	${SLASH_CLANG_ARG}-ftree-slp-vectorize

	${SLASH_CLANG_ARG}-fno-fine-grained-bitfield-accesses
	#"SHELL:-Xclang -fslp-vectorize"

	#"SHELL:-mllvm -slp-vectorize-hor-store=true"
	#"SHELL:-mllvm -slp-threshold=-1"

	# Recognize more loop things I guess?
	"SHELL:-mllvm -lv-strided-pointer-ivs=true"

	# Prefer padding instructions instead
	# of emitting NOPs? Maybe?
	"SHELL:-mllvm -x86-pad-for-align=true"
	"SHELL:-mllvm -x86-pad-for-branch-align=true"

	#"SHELL:-mllvm -enable-interleaved-mem-accesses=true"
	#"SHELL:-mllvm -enable-masked-interleaved-mem-accesses=true"
	#"SHELL:-mllvm -enable-loop-distribute"
	#"SHELL:-mllvm -optimize-regalloc=true"
	#"SHELL:-mllvm -enable-merge-functions=true"
	#"SHELL:-mllvm -force-hardware-loops=true"
	"SHELL:-mllvm -extra-vectorizer-passes=true"
	"SHELL:-mllvm -aggressive-ext-opt"

	# Screw with these at some point to fix that stupid
	# "move tons of LEA above a switch" nonsense
	#"SHELL:-mllvm -disable-licm-promotion=true"
	#"SHELL:-mllvm -disable-machine-licm=true"
	#"SHELL:-mllvm -disable-postra-machine-licm=true"
	#"SHELL:-mllvm -disable-constant-hoisting=true"

	# Enable screwy register allocations
	#"SHELL:-mllvm -enable-ipra=true"
	
	# Disable Stack Slot Coloring
	#"SHELL:-mllvm -disable-ssc=true"

	# Enable splitting cold blocks into independent things
	#"SHELL:-mllvm -enable-split-machine-functions=true"

	# Require memory CMOVs be converted to branches
	"SHELL:-mllvm -x86-cmov-converter-force-mem-operand=false"
	
	# Convert all CMOVs to branches
	#"SHELL:-mllvm -x86-cmov-converter-force-all=true"

	# Disable SSE domain reassignment
	#"SHELL:-mllvm -disable-x86-domain-reassignment=true"

	# Disable converting byte/word instructions to dword
	#"SHELL:-mllvm -fixup-byte-word-insts=false"

	# Something about relaxing atomic loads/stores
	#"SHELL:-mllvm -x86-experimental-unordered-atomic-isel=true"

	# Screw hardening nonsense
	"SHELL:-mllvm -x86-lvi-load-no-cbranch=true"

	# Disable removing some LEA patterns
	#"SHELL:-mllvm -disable-x86-lea-opt=true"

	# Generate REP MOV for memcpy when fast
	#"SHELL:-mllvm -x86-use-fsrm-for-memcpy=true"

	# Enable early if conversion
	#"SHELL:-mllvm -x86-early-ifcvt=true"

	# Disable generating VZEROUPPER for AVX/SSE transitions
	#"SHELL:-mllvm -x86-use-vzeroupper=false"

	# Not sure, but it sounds good
	#"SHELL:-mllvm -mergefunc-use-aliases=true"

	# Assume that nested loops won't
	# overflow their index if merged
	"SHELL:-mllvm -loop-flatten-assume-no-overflow=true"

	# Allow converting switches
	# to lookup tables
	"SHELL:-mllvm -switch-to-lookup=true"

	# Allow converting switches
	# to integer ranges
	#"SHELL:-mllvm -switch-range-to-icmp=true"

	# ???
	#"SHELL:-mllvm -keep-loops=false"

	# Why wouldn't this be on?
	"SHELL:-mllvm -enable-knowledge-retention=true"

	# Funky icache things
	#"SHELL:-mllvm -enable-ext-tsp-block-placement=true"
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
	${SLASH_CLANG_ARG}-fno-math-errno

	#"SHELL:-Xclang -fassume-sane-operator-new"
	${SLASH_CLANG_ARG}-fnew-infallible
	#-fsized-deallocation

	#${SLASH_CLANG_ARG}-mstack-probe-size=93284238
	${SLASH_CLANG_ARG}-mstack-probe-size=1024
	${SLASH_CLANG_ARG}-ffinite-loops

	#-fhonor-infinities
	#-fhonor-nans
	#-fsigned-zeros

	${SLASH_CLANG_ARG}-flax-vector-conversions=all

	${SLASH_CLANG_ARG}-fstrict-flex-arrays=1

	#${SLASH_CLANG_ARG}-fthreadsafe-statics

	${SLASH_CLANG_ARG}-mno-ms-bitfields
	#"SHELL:-Xclang -mno-ms-bitfields"

	#"-fno-exceptions"
)

target_link_options(${EXE} PRIVATE
	#"LINKER:--allow-multiple-definition"
)

target_compile_options(${EXE} PRIVATE
	#"SHELL:-fuse-ld=lld-link"
	#"SHELL:-fuse-ld=link"
	#"-fuse-ld=\"F:\\Program Files (x86)\\LLVM\\bin\\ld.lld.exe\""
)

#set(CMAKE_LINKER "F:\\Program Files (x86)\\LLVM\\bin\\ld.lld.exe")

#include_directories("F:\\Program Files (x86)\\LLVM\\lib\\clang\\12.0.0\\include")
#include_directories("F:\\Program Files\\LLVM\\lib\\clang\\13.0.0\\include")
#include_directories("F:\\Program Files\\LLVM\\lib\\clang\\14.0.0\\include")
#include_directories(SYSTEM "F:\\Program Files\\LLVM\\lib\\clang\\15.0.2\\include")
include_directories(BEFORE SYSTEM "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\zero_include")
#include_directories(SYSTEM "F:\\Program Files\\LLVM\\lib\\clang\\16\\include")
include_directories(SYSTEM "F:\\Program Files\\LLVM\\lib\\clang\\17\\include")
include_directories(SYSTEM "C:\\WinDDK\\7600.16385.1\\inc\\ddk")
include_directories(SYSTEM "F:\\MyCodeLibs\\SoundSample\\Sample\\DXUT")
include_directories(SYSTEM "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Include")
include_directories(SYSTEM "F:\\My Programs Expansion\\DirectX_SDKs\\8.0a\\include")
include_directories(SYSTEM "C:\\Program Files (x86)\\Microsoft Speech SDK\\Include")
include_directories(SYSTEM "F:\\MyCodeLibs\\boost\\install\\boost_1_82_0\\boost_1_82_0\\")
#if(NOT MSVC)
include_directories(AFTER SYSTEM
	"F:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.29.30133\\ATLMFC\\include"
	"F:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.29.30133\\include"
	"C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.8\\include\\um"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.20262.0\\ucrt"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.20262.0\\shared"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.20262.0\\um"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.20262.0\\winrt"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.20262.0\\cppwinrt"
)
#endif()
include_directories(AFTER SYSTEM
	"F:\\Users\\zero318\\Source\\Repos\\thcrap\\libs\\external_deps\\include"
)
#add_library(NT "C:\\Windows\\SysWOW64\\ntdll.dll")

#target_link_libraries(${EXE} NT)
#target_link_libraries(${EXE} "ntdll.lib" "msvcrt.lib")
#target_link_libraries(${EXE} "msvcrt.lib")
#target_link_libraries(${EXE} "C:\\WinDDK\\7600.16385.1\\lib\\wxp\\i386\\ntdll.lib")
if (IS_X64)
#target_link_libraries(${EXE} "C:\\WinDDK\\7600.16385.1\\lib\\win7\\amd64\\winmm.lib")
target_link_directories(${EXE} PRIVATE "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.20262.0\\ucrt\\x64")
target_link_directories(${EXE} PRIVATE "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Lib\\x64")
#target_link_options(${EXE} PRIVATE "/libpath:\"C/Program Files (x86)/Windows Kits/10/Lib/10.0.20262.0/ucrt/x64/\"")
target_link_libraries(${EXE} "F:\\Program Files\\LLVM\\lib\\clang\\17\\lib\\windows\\clang_rt.builtins-x86_64.lib")
else()
#target_link_libraries(${EXE} "F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\ntdll32.lib")
if (NOT NO_STDLIB)
target_link_libraries(${EXE} "C:\\WinDDK\\7600.16385.1\\lib\\wxp\\i386\\winmm.lib")
endif()
target_link_directories(${EXE} PRIVATE "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.20262.0\\ucrt\\x86")
target_link_directories(${EXE} PRIVATE "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.20262.0\\um\\x86")
endif()

if (NOT NO_STDLIB)
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\dsound.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\dinput8.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\dxguid.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\d3d9.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\Hopefully_Current\\Lib\\x86\\d3dx9.lib")
target_link_libraries(${EXE} "F:\\My Programs Expansion\\DirectX_SDKs\\8.0a\\lib\\d3d8.lib")
#target_link_libraries(${EXE} DummyDLL)
endif()

# Add source to this project's executable.
target_sources(${EXE} PRIVATE
	#"ClangAsmTest1.cpp"
	#"ClangAsmTest1.h"

	#"util.h"
	"zero/force_macros.h"
	"zero/util.h"
	#"zero/util.cpp"
	"zero/zun.h"
	"zero/zun_types.h"
	"zero/zun_math.h"
	#"zero/ExternalGlobal.h"
	"zero/custom_intrin.h"
	"zero/thcrap_util.h"
	"zero/LinkerCheeseValues.h"
	"zero/BoundingBox.h"
	#"zero/trap_and_emulate.cpp"
	"zero/func_traits.h"
	"zero/compiler_support.h"
	"zun/util.h"
)
if (NOT NO_STDLIB)
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

add_library(windows_test_asm OBJECT
	"windows_labels.asm"
)
target_link_libraries(${PROJECT_NAME} "F:\\Users\\zero318\\Source\\Repos\\thcrap\\bin\\bin\\jansson.lib")
target_link_libraries(${PROJECT_NAME} "F:\\Users\\zero318\\Source\\Repos\\thcrap\\bin\\bin\\thcrap.lib")
target_compile_options(${PROJECT_NAME} PRIVATE
	#-fforce-emit-vtables
	"-mno-sse3"
	"-mno-avx"
	"-mno-bmi"
	"-mno-lzcnt"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
)
target_include_directories(${PROJECT_NAME} PRIVATE 
	"F:\\MyCodeLibs\\asmlib"
)
target_sources(${PROJECT_NAME} PRIVATE
	$<TARGET_OBJECTS:windows_test_asm>
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
	#"sha256_thcrap.cpp"
	#"sha256_new.cpp"
	#"bson_test.cpp"
	#"zson_test.cpp"
	#"zero/city_hash.h"
	#"zero/city_hash.cpp"
	"frame_pointer_test.cpp"
	#"json_test.cpp"
	#"json5_test.cpp"
	#"decent_rng.h"
	"rtti_label_gen.cpp"
	#"zero/vec_lib.cpp"
	"dns_test.cpp"
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
	"UM/new_header_test.cpp"
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
	"-mno-sse3"
	#-Rpass=.*vectorize.*
	#-Rpass-missed=.*vectorize.*
	#-Rpass-analysis=.*vectorize.*
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

target_sources(udoalg_test PRIVATE
	#"dummy_main.cpp"
	"UDoaLG/udoalg_test.cpp"
)
target_compile_options(udoalg_test PRIVATE
	#"-mno-avx"
	#"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	-Rpass=.*vectorize.*
	-Rpass-missed=.*vectorize.*
	-Rpass-analysis=.*vectorize.*
)

target_sources(AoCFTest1 PRIVATE
	"dummy_main.cpp"
	"AoCF/aocf_test.cpp"
)


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

target_sources(smallest_pe_file PRIVATE
	"why.cpp"
)
target_compile_definitions(smallest_pe_file PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(smallest_pe_file PRIVATE
	/ENTRY:yeet
	"/STUB:F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\empty_dos_stub.exe"
	/emittoolversioninfo:no
	"LINKER:/fixed"
	-nodefaultlib
)

target_sources(XInput1_4 PRIVATE
	"xinput_dummy/stop_complaining_about_xinput_1_4.cpp"
	"stop_complaining_about_xinput_1_4.def"
)
target_compile_options(XInput1_4 PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(XInput1_4 PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(XInput1_4 PRIVATE
	${MSLINK_ARG}/DEF:stop_complaining_about_xinput_1_4.def
	${MSLINK_ARG}/DLL
	${MSLINK_ARG}/NOENTRY
	#${MSLINK_ARG}-nodefaultlib
	${NOSTDLIB_LINK}
	#-nodefaultlibs
	#-nostdlib
)

target_sources(winhttp PRIVATE
	"winhttp_dummy/stop_breaking_websockets.cpp"
	"stop_breaking_websockets.def"
)
target_compile_options(winhttp PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(winhttp PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(winhttp PRIVATE
	/DEF:stop_breaking_websockets.def
	/DLL
	/NOENTRY
	-nodefaultlib
)

if(IS_X64)
target_sources(winhttp_websocket PRIVATE
	"winhttp_websocket/websocket_impl.cpp"
	"winhttp_websocket.def"
)
else()
target_sources(winhttp_websocket PRIVATE
	#"winhttp_websocket/websocket_impl.cpp"
	"winhttp_websocket/websocket_impl_logging.cpp"
	#"winhttp_websocket/new_impl.cpp"
	"winhttp_websocket.def"
)
endif()
target_compile_options(winhttp_websocket PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(winhttp_websocket PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(winhttp_websocket PRIVATE
	${MSLINK_ARG}"/DEF:winhttp_websocket.def"
	${COMPILE_DLL}
)

target_sources(twitch_irc_test PRIVATE
	"twitch_irc_test/twitch_irc.cpp"
	"twitch_irc_test/twitch_irc_test.cpp"
)
target_compile_options(twitch_irc_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
#target_compile_definitions(twitch_irc_test PRIVATE
	#STOP_LINKING_DEFAULT_LIB_PLZ
#)
#target_link_options(twitch_irc_test PRIVATE
	#/DLL
#)
target_sources(youtube_chat_test PRIVATE
	"twitch_irc_test/youtube_test.cpp"
)
target_compile_options(youtube_chat_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_link_libraries(youtube_chat_test "F:\\Users\\zero318\\Source\\Repos\\thcrap\\bin\\bin\\jansson.lib")

target_sources(winhttp_test3 PRIVATE
	"winhttp_test3/test.cpp"
)


target_sources(wow64 PRIVATE
	"wow64/wow64_test.cpp"
	"wow64/wow64cpu_test.cpp"
	"wow64/wow64win_test.cpp"
	"wow64.def"
)
target_compile_options(wow64 PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(wow64 PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
	COMPILE_WOW64
)
target_link_options(wow64 PRIVATE
	/DEF:wow64.def	
	${COMPILE_DLL}
	#${MSLINK_ARG}/NOENTRY
	${NOSTDLIB_LINK}
	-force:unresolved
)

target_sources(wow64cpu PRIVATE
	"wow64/wow64_test.cpp"
	"wow64/wow64cpu_test.cpp"
	"wow64/wow64win_test.cpp"
	"wow64cpu.def"
)
target_compile_options(wow64cpu PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
	#-mx32
)
target_compile_definitions(wow64cpu PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
	COMPILE_WOW64_CPU
)
target_link_options(wow64cpu PRIVATE
	/DEF:wow64cpu.def
	${COMPILE_DLL}
	#${MSLINK_ARG}/NOENTRY
	${NOSTDLIB_LINK}
	-force:unresolved
)

target_sources(wow64win PRIVATE
	"wow64/wow64_test.cpp"
	"wow64/wow64cpu_test.cpp"
	"wow64/wow64win_test.cpp"
	"wow64win.def"
)
target_compile_options(wow64win PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(wow64win PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
	COMPILE_WOW64_WIN
)
target_link_options(wow64win PRIVATE
	/DEF:wow64win.def
	${COMPILE_DLL}
	#${MSLINK_ARG}/NOENTRY
	${NOSTDLIB_LINK}
	-force:unresolved
)

target_sources(parakit_inject PRIVATE
	"parakit/test.cpp"
)
target_compile_options(parakit_inject PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(parakit_inject PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(parakit_inject PRIVATE
	/DLL
)

target_sources(ghidra_launch PRIVATE
	"ghidra_launch/ghidra_launch.cpp"
	"ghidra_launch/ghidra_launch.rc"
)

target_sources(crappy_launcher PRIVATE
	"thcrap_load_test.cpp"
)

target_link_directories(python_test PRIVATE 
"C:\\Users\\zero318\\AppData\\Local\\Programs\\Python\\Python37-32\\libs\\"
)
target_sources(python_test PRIVATE
	"python_test/test.cpp"
)
target_compile_options(python_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_link_options(python_test PRIVATE
	/DLL
)

target_sources(glaux PRIVATE
	"screensavers\\GLAUX\\FONT.C"
	"screensavers\\GLAUX\\GLAUX.C"
	"screensavers\\GLAUX\\IMAGE.C"
	"screensavers\\GLAUX\\SHAPES.C"
	"screensavers\\GLAUX\\TEAPOT.C"
	"screensavers\\GLAUX\\TK.C"
	"screensavers\\GLAUX\\TKDIB.C"
	"screensavers\\GLAUX\\TKFONT.C"
	"screensavers\\GLAUX\\TKIMAGE.C"
	"screensavers\\GLAUX\\VECT3D.C"
	"screensavers\\GLAUX\\XFORM.C"
)
target_compile_options(glaux PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(glaux PRIVATE
	_WINSVC_=1
	#_M_IX86=1
	DONT_COMPILE_REGISTER_ASSUME_JANK=1
)
target_include_directories(glaux PRIVATE
	"F:\\My Programming Stuff Expansion\\nt40_win95_sdk1996\\MSTOOLS\\INCLUDE"
)

target_sources(glcommon PRIVATE
	$<TARGET_OBJECTS:glaux>
	"screensavers\\COMMON\\UTIL.CXX"
	"screensavers\\COMMON\\COLOR.C"
	"screensavers\\COMMON\\DIALOG.C"
	"screensavers\\COMMON\\FASTDIB.C"
	"screensavers\\COMMON\\MATERIAL.C"
	"screensavers\\COMMON\\MATH.C"
	"screensavers\\COMMON\\SSA8.C"
	"screensavers\\COMMON\\SSDIB.C"
	"screensavers\\COMMON\\SSIMAGE.C"
	"screensavers\\COMMON\\TEXTURE.C"
	"screensavers\\COMMON\\CLEAR.CXX"
	"screensavers\\COMMON\\DLGDRAW.CXX"
	"screensavers\\COMMON\\GLSCRNSV.CXX"
	"screensavers\\COMMON\\PALETTE.CXX"
	"screensavers\\COMMON\\SCRNSAVE.CXX"
	"screensavers\\COMMON\\SSINIT.CXX"
	"screensavers\\COMMON\\SSUTIL.CXX"
	"screensavers\\COMMON\\SSWINDOW.CXX"
	"screensavers\\COMMON\\SSWPROC.CXX"
	"screensavers\\COMMON\\SSCOMMON.RC"
)
target_compile_options(glcommon PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(glcommon PRIVATE
	_WINSVC_=1
	#_M_IX86=1
	DONT_COMPILE_REGISTER_ASSUME_JANK=1
)
target_include_directories(glcommon PRIVATE
	"F:\\My Programming Stuff Expansion\\nt40_win95_sdk1996\\MSTOOLS\\INCLUDE"
)


target_sources(3d_flying_objects PRIVATE
	#$<TARGET_OBJECTS:glaux>
	#$<TARGET_OBJECTS:glcommon>
	"screensavers\\GLAUX\\FONT.C"
	"screensavers\\GLAUX\\GLAUX.C"
	"screensavers\\GLAUX\\IMAGE.C"
	"screensavers\\GLAUX\\SHAPES.C"
	"screensavers\\GLAUX\\TEAPOT.C"
	"screensavers\\GLAUX\\TK.C"
	"screensavers\\GLAUX\\TKDIB.C"
	"screensavers\\GLAUX\\TKFONT.C"
	"screensavers\\GLAUX\\TKIMAGE.C"
	"screensavers\\GLAUX\\VECT3D.C"
	"screensavers\\GLAUX\\XFORM.C"
	"screensavers\\COMMON\\UTIL.CXX"
	"screensavers\\COMMON\\COLOR.C"
	"screensavers\\COMMON\\DIALOG.C"
	"screensavers\\COMMON\\FASTDIB.C"
	"screensavers\\COMMON\\MATERIAL.C"
	"screensavers\\COMMON\\MATH.C"
	"screensavers\\COMMON\\SSA8.C"
	"screensavers\\COMMON\\SSDIB.C"
	"screensavers\\COMMON\\SSIMAGE.C"
	"screensavers\\COMMON\\TEXTURE.C"
	"screensavers\\COMMON\\CLEAR.CXX"
	"screensavers\\COMMON\\DLGDRAW.CXX"
	"screensavers\\COMMON\\GLSCRNSV.CXX"
	"screensavers\\COMMON\\PALETTE.CXX"
	"screensavers\\COMMON\\SCRNSAVE.CXX"
	"screensavers\\COMMON\\SSINIT.CXX"
	"screensavers\\COMMON\\SSUTIL.CXX"
	"screensavers\\COMMON\\SSWINDOW.CXX"
	"screensavers\\COMMON\\SSWPROC.CXX"
	"screensavers\\COMMON\\SSCOMMON.RC"
	"screensavers/3DFO/GENDROP.CXX"
	"screensavers\\3DFO\\GENEXPLD.CXX"
	"screensavers/3DFO/GENLEM.CXX"
	"screensavers/3DFO/GENSTRIP.CXX"
	"screensavers/3DFO/GENTEX.CXX"
	"screensavers/3DFO/GENWIN.CXX"
	"screensavers/3DFO/MESH.CXX"
	"screensavers\\3DFO\\SS3DFO.CXX"
	"screensavers\\3DFO\\SS3DFO.RC"
	"screensavers\\3DFO\\GLSAVER.DEF"
)
target_compile_options(3d_flying_objects PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(3d_flying_objects PRIVATE
	_WINSVC_=1
	#_M_IX86=1
	#UNICODE=1
	#_UNICODE=1
	#_INC_SCRNSAVE=1
	#IDS_DESCRIPTION=1
)
target_include_directories(3d_flying_objects AFTER PRIVATE
	"screensavers\\COMMON"
	"F:\\My Programming Stuff Expansion\\nt40_win95_sdk1996\\MSTOOLS\\INCLUDE"
)
target_link_libraries(3d_flying_objects 
	opengl32.lib
	glu32.lib
	#"F:\\Users\\zero318\\Source\\Repos\\ClangAsmTest1\\screensavers\\GLAUX.LIB"
	comdlg32.lib
	comctl32.lib
	scrnsave.lib
)

target_sources(discord_test PRIVATE
	"winhttp_websocket/websocket_impl.cpp"
	"discord_test/discord_test.cpp"
	#"json_test.cpp"
)
target_compile_definitions(discord_test PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)

target_sources(loaddll PRIVATE
	"loaddll/loaddll.cpp"
)
target_compile_options(loaddll PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(loaddll PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(loaddll PRIVATE
	/ENTRY:yeet
	${NOSTDLIB_LINK}
)
target_sources(force_load_pe PRIVATE
	"loaddll/force_load_pe.cpp"
)
target_compile_options(force_load_pe PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(force_load_pe PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(force_load_pe PRIVATE
	/ENTRY:yeet
	${NOSTDLIB_LINK}
)

target_sources(x64_inject_test PRIVATE
	"x64_inject_test.cpp"
)
target_compile_options(x64_inject_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(x64_inject_test PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(x64_inject_test PRIVATE
	${COMPILE_DLL}
	${NOSTDLIB_LINK}
	#"LINKER:/base:0x7FFD95940000"
)
target_sources(x64_injector PRIVATE
	"x64_injector.cpp"
)
target_compile_options(x64_injector PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(x64_injector PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(x64_injector PRIVATE
	/ENTRY:entry	
	${NOSTDLIB_LINK}
)

target_sources(aocf_netcode PRIVATE
	"AoCF/netcode_files/main.cpp"
	"AoCF/netcode_files/AllocMan.cpp"
	"AoCF/netcode_files/AllocMan.h"
	"AoCF/netcode_files/log.cpp"
	"AoCF/netcode_files/log.h"
	"AoCF/netcode_files/PatchUtils.cpp"
	"AoCF/netcode_files/PatchUtils.h"
	"AoCF/netcode_files/util.h"
	"AoCF/netcode_files/zpunch.cpp"
)
target_compile_options(aocf_netcode PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(aocf_netcode PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)

target_sources(Netplay PRIVATE
	"AoCF/netplay_patch/netplay.cpp"
)
target_compile_options(Netplay PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(Netplay PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_link_options(Netplay PRIVATE
	${COMPILE_DLL}
	/ENTRY:DllMain
	${NOSTDLIB_LINK}
)

target_sources(znet_client_test PRIVATE
	"znet/client_test.cpp"
)
target_compile_options(znet_client_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)

target_sources(znet_server_test PRIVATE
	"znet/server_test.cpp"
)
target_compile_options(znet_server_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)

target_sources(znet_p2p_test PRIVATE
	"znet/p2p_test.cpp"
)
target_compile_options(znet_p2p_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)

target_sources(aocf_server PRIVATE
	"znet/aocf_server.cpp"
)
target_compile_options(aocf_server PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)

target_sources(make_embed PRIVATE
	"make_embed.cpp"
)

target_sources(run_command PRIVATE 
	"run_command.cpp"
)

target_sources(aocf_plugin_patch PRIVATE
	"AoCF/plugin_patch.cpp"
)
target_compile_options(aocf_plugin_patch PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)
target_compile_definitions(aocf_plugin_patch PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)

target_sources(unix_socket_test PRIVATE
	"znet/unix_socket_test.cpp"
)
target_compile_options(unix_socket_test PRIVATE
	"-mno-avx"
	"-mno-bmi"
	"-mno-xop"
	"-mno-tbm"
	"-mno-sse4a"
	"-mno-fma4"
	"-mno-sse3"
)

target_sources(z8086_old PRIVATE
	"8086_cpu.cpp"
	"dummy_main.cpp"
)
target_compile_definitions(z8086_old PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)

target_sources(z86_emulator PRIVATE
	"x86_emulators/core_manager.cpp"
	"x86_emulators/8086_cpu.cpp"
)
target_compile_definitions(z86_emulator PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
if(IS_X64)
target_compile_options(z86_emulator PRIVATE
	"-fno-pic"
	"-fno-pie"
	"-msahf"
)
endif()

target_sources(z8086 PRIVATE
	"x86_emulators/8086_cpu.cpp"
	"dummy_dllmain.cpp"
)
target_link_options(z8086 PRIVATE
	${COMPILE_DLL}
	/ENTRY:DllMain
)
target_compile_definitions(z8086 PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)

target_sources(ttt_no_mem PRIVATE
	"ttt_no_mem.cpp"
)
target_compile_definitions(ttt_no_mem PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
)
target_compile_options(ttt_no_mem PRIVATE
	"-fno-pic"
	"-fno-pie"
	"-msahf"
)
target_link_options(ttt_no_mem PRIVATE
	/LARGEADDRESSAWARE:NO
)

target_sources(libm_testing PRIVATE
	"libm/sin_cos.cpp"
	"libm/trandisp.cpp"
)
target_compile_definitions(libm_testing PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
	_HAS_CXX17=1
	_HAS_CXX20=1
)
target_compile_options(libm_testing PRIVATE
	#"-mavx"
	#"-mbmi"
	#"-mxop"
	#"-mtbm"
	#"-msse4a"
	#"-mfma4"
	#"-msse3"
	#"-mssse3"
	#"-msse4.1"
	#"-msse4.2"
	#"-mlzcnt"
)

target_sources(dbghelp PRIVATE
	"dbghelp/dbghelp.cpp"
	"dbghelp/dbghelp.def"
	"dbghelp/crc32.cpp"
	"dummy_dllmain.cpp"
)
target_compile_options(dbghelp PRIVATE
	"-mavx"
	"-mbmi"
	"-msse4.1"
	"-mssse3"
	"-msse3"
	"-mlzcnt"
	"-mxop"
	"-mtbm"
	"-msse4a"
	"-mfma4"
	"-mfma"
)
target_compile_definitions(dbghelp PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
	_HAS_CXX17=1
	_HAS_CXX20=1
)
target_link_options(dbghelp PRIVATE
	#${MSLINK_ARG}/DEF:dbghelp.def
	#${COMPILE_DLL}
	#/ENTRY:DllMain
	#${NOSTDLIB_LINK}
)
target_link_libraries(dbghelp 
	_dbghelp.lib
	"F:\\Users\\zero318\\Source\\Repos\\thcrap\\bin\\bin\\jansson.lib"
	libcmt.lib
)

target_sources(_dbghelp PRIVATE
	"dbghelp/dbghelp_init_stub.cpp"
	"dbghelp/dbghelp_stub.def"
	"dummy_dllmain.cpp"
)
target_compile_definitions(_dbghelp PRIVATE
	STOP_LINKING_DEFAULT_LIB_PLZ
	_HAS_CXX17=1
	_HAS_CXX20=1
)
target_link_options(_dbghelp PRIVATE
	${COMPILE_DLL}
	/ENTRY:DllMain
	${NOSTDLIB_LINK}
)

target_sources(vpatch PRIVATE
	"vpatch/vpatch_launcher.cpp"
)
target_compile_options(vpatch PRIVATE
	"-mno-sse"
	"-mno-sse2"
)
target_sources(vpatch_dll PRIVATE
	"vpatch/vpatch_dll.cpp"
)
target_compile_options(vpatch_dll PRIVATE
	"-mno-sse"
	"-mno-sse2"
)

target_sources(heroes_calculator PRIVATE
	"heroes_calculator.cpp"
)

#target_link_libraries(UMTest1 "F:\\Touhou_Stuff_2\\disassembly_stuff\\18\\crack\\th18_fake_dll.dll")

#set_source_files_properties(NewMoFECL.cpp PROPERTIES COMPILE_FLAGS "-mno-sse -Xclang -menable-unsafe-fp-math")

#add_dependencies(AsmTemp ${PROJECT_NAME})

set_source_files_properties(x87_math_helpers.h PROPERTIES COMPILE_FLAGS "-mno-sse -mno-sse2")
set_source_files_properties(x87_math_helpers.cpp PROPERTIES COMPILE_FLAGS "-mno-sse -mno-sse2")
set_source_files_properties(frame_pointer_test.cpp PROPERTIES COMPILE_FLAGS ${SLASH_CLANG_ARG}-fno-omit-frame-pointer)
#set_source_files_properties(RNG_helpers.h PROPERTIES COMPILE_FLAGS "-mno-sse -mno-sse2")

# TODO: Add tests and install targets if needed.
