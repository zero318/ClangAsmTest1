#pragma once
#ifndef VPATCH_COMMON_H
#define VPATCH_COMMON_H 1

#pragma comment(lib, "shlwapi.lib")

#define ENGLISH_STRINGS 0

#if ENGLISH_STRINGS
#define JpEnStr(jstring, estring) estring
#else
#define JpEnStr(jstring, estring) jstring
#endif

#define countof(array_type) (sizeof(array_type) / sizeof(array_type[0]))

enum InitializeReturnCode {
	INIT_SUCCESS = 0,
	INIT_PROCESS_FAILED = 1,
	INIT_INJECT_FAILED = 2,
	INIT_UNKNOWN_ERROR = 3,
};

#endif