#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"

#if __INTELLISENSE__
#define _HAS_CXX17 1
#define _HAS_CXX20 0
#endif

// These types aren't even used in the code
// but clang is warning about them anyway
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#define USE_VECTOR_FOR_NAME_HASHES 0

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <utility>
#include <array>
#include <string_view>
#if USE_VECTOR_FOR_NAME_HASHES
#include <vector>
#else
#include <unordered_set>
#endif

#include <thread>

#if __INTELLISENSE__
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif

#include <bit>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#define ENABLE_SSL 0

#define PRINT_ERROR_MESSAGES 1

#define PRINT_MESSAGES 0

#if ENABLE_SSL
#include <algorithm>
#define SECURITY_WIN32
#include <security.h>
#include <schannel.h>
#include <sspi.h>
#endif

#include <Windows.h>

#include <WinDNS.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Dnsapi")
#if ENABLE_SSL
#pragma comment (lib, "secur32.lib")
#endif

#include "..\zero_private\youtube_api_key.hpp"

#include "jansson.h"


//static constexpr char youtube_channel_id[] = "UCpu91i5feE6jcj1T7zHw1KQ";
//static constexpr wchar_t youtube_channel_id_w[] = L"UCpu91i5feE6jcj1T7zHw1KQ";
static constexpr char youtube_channel_id[] = "UCk9RA3G-aVQXvp7-Q4Ac9kQ";
static constexpr wchar_t youtube_channel_id_w[] = L"UCk9RA3G-aVQXvp7-Q4Ac9kQ";



