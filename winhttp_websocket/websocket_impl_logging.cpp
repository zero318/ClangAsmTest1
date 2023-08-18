#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <utility>
#include <tuple>

#include <Windows.h>
#include <winternl.h>
#include <Rpc.h>
#include <winhttp.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#define SECURITY_WIN32
#include <security.h>
#include <schannel.h>
#include <sspi.h>

#include <windns.h>

#include "../zero/util.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Dnsapi")
#pragma comment (lib, "secur32.lib")

// Use this when the int3 is intended to
// be a stop rather than a debug break
#define halt_and_catch_fire() __asm__("int3")

/*
Clang doesn't inline string functions from string.h, only memory functions
*/

template <typename T>
static inline constexpr size_t str_len(const T* str) {
	size_t len = 0;
	while (*str++) ++len;
	return len;
}

// Returns end pointer for simplifying concat operations
template <typename T>
static inline T* str_copy(T* dst, const T* src, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		dst[i] = src[i];
	}
	return dst + count;
}

template <typename T, typename ... StrsT>
static inline T* build_str(T* buffer, const T* str, size_t len, StrsT&&... next_strs) {
	if constexpr (sizeof...(StrsT)) {
		return build_str(str_copy(buffer, str, len), std::forward<StrsT>(next_strs)...);
	} else {
		return str_copy(buffer, str, len);
	}
}
template <typename T, size_t len, typename ... StrsT>
requires(!sizeof...(StrsT) || std::is_pointer_v<std::decay_t<std::tuple_element_t<0, std::tuple<StrsT...>>>>)
static inline T* build_str(T* buffer, const T(&str)[len], StrsT&&... next_strs) {
	if constexpr (sizeof...(StrsT)) {
		return build_str(str_copy(buffer, str, len), std::forward<StrsT>(next_strs)...);
	} else {
		return str_copy(buffer, str, len);
	}
}

template <typename T>
static inline size_t uint16_to_strbuf(uint16_t value, T* text_buffer) {
	size_t digit_offset;
	switch (value) {
		case 0 ... 9:
			digit_offset = 0;
			break;
		case 10 ... 99:
			digit_offset = 1;
			break;
		case 100 ... 999:
			digit_offset = 2;
			break;
		case 1000 ... 9999:
			digit_offset = 3;
			break;
		default:
			digit_offset = 4;
			break;
	}
	size_t ret = digit_offset + 1;
	do {
		uint16_t digit = value % 10;
		value /= 10;
		text_buffer[digit_offset] = ((T)'0') + digit;
	} while (digit_offset--);
	return ret;
}

template <typename T>
static inline T* allocate() {
	return (T*)malloc(sizeof(T));
}

template <typename T>
static inline T* allocate(size_t count) {
	return (T*)malloc(sizeof(T[count]));
}

template <typename T1, typename T2>
static inline T1* allocate(size_t count) {
	return (T1*)malloc(sizeof(T1) + sizeof(T2[count]));
}

dllexport BOOL WINAPI WinHttpCloseHandle(HINTERNET hInternet) asm("_WinHttpCloseHandle");
dllexport HINTERNET WINAPI WinHttpConnect(HINTERNET hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved) asm("_WinHttpConnect");
dllexport DWORD WINAPI WinHttpWebSocketClose(HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength) asm("_WinHttpWebSocketClose");
dllexport DWORD WINAPI WinHttpWebSocketReceive(HINTERNET hWebSocket, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType) asm("_WinHttpWebSocketReceive");
dllexport DWORD WINAPI WinHttpWebSocketSend(HINTERNET hWebSocket, WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType, PVOID pvBuffer, DWORD dwBufferLength) asm("_WinHttpWebSocketSend");
dllexport HINTERNET WINAPI WinHttpWebSocketCompleteUpgrade(HINTERNET hRequest, DWORD_PTR pContext) asm("_WinHttpWebSocketCompleteUpgrade");
dllexport BOOL WINAPI WinHttpReceiveResponse(HINTERNET hRequest, LPVOID lpReserved) asm("_WinHttpReceiveResponse");
dllexport BOOL WINAPI WinHttpSendRequest(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext) asm("_WinHttpSendRequest");
dllexport HINTERNET WINAPI WinHttpOpenRequest(HINTERNET hConnect, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, LPCWSTR pwszVersion, LPCWSTR pwszReferrer, LPCWSTR FAR* ppwszAcceptTypes, DWORD dwFlags) asm("_WinHttpOpenRequest");
dllexport BOOL WINAPI WinHttpSetOption(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength) asm("_WinHttpSetOption");
dllexport HINTERNET WINAPI WinHttpOpen(LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags) asm("_WinHttpOpen");

// Replacement version of getaddrinfo that doesn't crash
INT WSAAPI getaddrinfo_replacement(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints, PADDRINFOA* ppResult) {
	char addr_buff[48];
	if (pNodeName) {
		for (const char* node_name_scan = pNodeName; char c = *node_name_scan; ++node_name_scan) {
			switch (c) {
				case '.': case '0' ... '9':
					continue;
			}
			bool restrict_ip_type = false;
			WORD dns_type = DNS_TYPE_AAAA;
			if (pHints) {
				switch (pHints->ai_family) {
					case AF_INET:
						dns_type = DNS_TYPE_A;
						[[fallthrough]];
					case AF_INET6:
						restrict_ip_type = true;
						break;
				}
			}
			PDNS_RECORDA records;
			if (expect(DnsQuery_A(pNodeName, dns_type, !restrict_ip_type ? DNS_QUERY_DUAL_ADDR : 0, NULL, (PDNS_RECORD*)&records, NULL) == 0, true)) {
				PDNS_RECORDA cur_record = records;
				do {
					void* ipv4;
					switch (cur_record->wType) {
						[[unlikely]] default:
							halt_and_catch_fire();
						case DNS_TYPE_CNAME:
							break;
						case DNS_TYPE_AAAA:
							if (
								restrict_ip_type ||
								!(cur_record->Data.AAAA.Ip6Address.IP6Dword[0] == 0 &&
								  cur_record->Data.AAAA.Ip6Address.IP6Dword[1] == 0 &&
								  cur_record->Data.AAAA.Ip6Address.IP6Dword[2] == 0xFFFF0000)
							) {
								inet_ntop(AF_INET6, &cur_record->Data.AAAA.Ip6Address, addr_buff, countof(addr_buff));
								goto found_addr;
							} else if (1) {
								ipv4 = &cur_record->Data.AAAA.Ip6Address.IP6Dword[3];
							} else {
						case DNS_TYPE_A:
							ipv4 = &cur_record->Data.A.IpAddress;
							}
							inet_ntop(AF_INET, ipv4, addr_buff, countof(addr_buff));
							goto found_addr;
					}
				} while ((cur_record = cur_record->pNext));
				DnsFree(records, DnsFreeRecordList);
			}
			return EAI_FAIL;
		found_addr:
			DnsFree(records, DnsFreeRecordList);
			pNodeName = addr_buff;
		}
	}
	return getaddrinfo(pNodeName, pServiceName, pHints, ppResult);
}


struct StaticFlags {
	bool initialized_winsock;
};

static inline constinit StaticFlags flags{
	.initialized_winsock = false
};

#define not_null_hinternet ((HINTERNET)1)

// This is a static variable so that stack space
// doesn't need to be allocated for it every single time
static WSADATA idgaf_about_winsock_version;

/*
static inline constexpr const wchar_t request_header_part_1[] = L"GET ";
static inline constexpr const wchar_t request_header_part_2[] = L" HTTP/1.1\r\n"
																L"Connection: Upgrade\r\n"
																L"Upgrade: websocket\r\n"
																L"User-Agent: ";
static inline constexpr const wchar_t request_header_part_3[] = L"\r\n"
																L"Sec-WebSocket-Key: ";
static inline constexpr const wchar_t request_header_part_4[] = L"\r\n"
																L"Sec-WebSocket-Version: 13\r\n"
																L"Host: ";
static inline constexpr const wchar_t request_header_part_5[] = L":";
static inline constexpr const wchar_t request_header_part_6[] = L"\r\n";
static inline constexpr const wchar_t request_header_part_7[] = L"\r\n\r\n";
*/

// These parts are in the open request func
static inline constexpr const wchar_t request_header_part_1[] = { L'G',L'E',L'T',L' ' };
static inline constexpr const wchar_t request_header_part_2[] = { L' ',L'H',L'T',L'T',L'P',L'/',L'1',L'.',L'1',L'\r',L'\n',
																  L'C',L'o',L'n',L'n',L'e',L'c',L't',L'i',L'o',L'n',L':',L' ',L'U',L'p',L'g',L'r',L'a',L'd',L'e',L'\r',L'\n',
																  L'U',L'p',L'g',L'r',L'a',L'd',L'e',L':',L' ',L'w',L'e',L'b',L's',L'o',L'c',L'k',L'e',L't',L'\r',L'\n',
																  L'U',L's',L'e',L'r',L'-',L'A',L'g',L'e',L'n',L't',L':',L' '
};
static inline constexpr const wchar_t request_header_part_3[] = { L'\r',L'\n',
																  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'K',L'e',L'y',L':',L' '
};
static inline constexpr const wchar_t request_header_part_4[] = { L'\r',L'\n',
																  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'V',L'e',L'r',L's',L'i',L'o',L'n',L':',L' ',L'1',L'3',L'\r',L'\n',
																  L'H',L'o',L's',L't',L':',L' '
};
static inline constexpr const wchar_t request_header_part_5[] = { L':' };
static inline constexpr const wchar_t request_header_part_6[] = { L'\r',L'\n' };
// This part is in the send request func
static inline constexpr const wchar_t request_header_part_7[] = { L'\r',L'\n',L'\r',L'\n' };

#define RESPECT_HTTPS 1

static inline constexpr size_t receive_buffer_size = UINT16_MAX + 1;

#if RESPECT_HTTPS
#define TLS_MAX_PACKET_SIZE (16384+512) // payload + extra over head for header/mac/padding (probably an overestimate)
#endif

typedef struct UserAgentData UserAgentData;
typedef struct SocketData SocketData;
typedef struct RequestData RequestData;
typedef struct WebSocketData WebSocketData;

enum StructType {
	GenericDataType,
	SocketDataType,
};

template <StructType _data_type = GenericDataType>
struct FreeData {

	StructType data_type; // 0x0

	inline constexpr FreeData() : data_type(_data_type) {}

	inline void cleanup() {
		switch (this->data_type) {
			case SocketDataType:
				((SocketData*)this)->~SocketData();
				break;
		}
	}
};
ValidateStructSize32(0x4, FreeData<>);

struct UserAgentData : FreeData<GenericDataType> {
	size_t user_agent_str_len; // 0x4
	wchar_t user_agent_str[]; // 0x8

	inline constexpr operator HINTERNET() const {
		return (HINTERNET)this;
	}

	inline UserAgentData(const wchar_t* str, size_t str_len) : user_agent_str_len(str_len) {
		str_copy(this->user_agent_str, str, str_len);
	}

	inline ~UserAgentData() = default;

	static inline UserAgentData* allocate(const wchar_t* str) {
		size_t user_agent_str_len = str_len(str);
		return new (::allocate<UserAgentData, wchar_t>(user_agent_str_len)) UserAgentData(str, user_agent_str_len);
	}
};

static inline constexpr size_t nonce_key_encode_bits = 144;

struct SocketData : FreeData<SocketDataType> {

	static inline constexpr size_t port_str_buf_len = 8;

	SOCKET sock; // 0x4
	wchar_t* server_str; // 0x8
	size_t server_str_len; // 0xC
	wchar_t port_str[port_str_buf_len]; // 0x10
	wchar_t nonce_key_str[24]; // 0x20
	union { // 0x50
		uint8_t nonce_key[16];
		uint32_t nonce_key_dwords[5];
		UBitInt(nonce_key_encode_bits) nonce_key_wide;
	};
	const wchar_t* user_agent_str; // 0x64
	size_t user_agent_str_len; // 0x68
	size_t port_str_len; // 0x6C
	bool upgraded_to_websocket; // 0x70
	bool generated_nonce_key; // 0x71
#if RESPECT_HTTPS
	bool tls_in_use; // 0x72
	bool tls_initialized; // 0x73
	SecPkgContext_StreamSizes tls_sizes; // 0x74
	// uint64_t __unused_qword_A; // 0x88
	CredHandle tls_handle; // 0x90
	CtxtHandle tls_context; // 0xA0
	//cache_align char packet_buffer[TLS_MAX_PACKET_SIZE];
#endif

	inline constexpr operator HINTERNET() const {
		return (HINTERNET)this;
	}

	inline SocketData(SOCKET sock, const wchar_t* _server_str, const wchar_t* port_str, size_t port_str_len, UserAgentData* user_agent_data)
		: sock(sock), upgraded_to_websocket(false), generated_nonce_key(false), port_str_len(port_str_len),
#if RESPECT_HTTPS
		tls_in_use(false), tls_initialized(false),
#endif
		user_agent_str(user_agent_data->user_agent_str), user_agent_str_len(user_agent_data->user_agent_str_len)
	{
		this->nonce_key_dwords[4] = 0;
		str_copy(this->port_str, port_str, port_str_buf_len);
		size_t server_str_len = str_len(_server_str);
		this->server_str_len = server_str_len;
		wchar_t* server_str = ::allocate<wchar_t>(server_str_len);
		this->server_str = server_str;
		str_copy(server_str, _server_str, server_str_len);
	}

	inline ~SocketData() {
		free(this->server_str);
		// TODO: Close socket like a sane person.
		// For some reason closing the socket here seems to cause requests to fail?
		// The game might be calling close a bit too fast, so try moving this
		// to part of the receive code since that is called from an async thread.
		closesocket(this->sock);
	}

	static inline SocketData* allocate(SOCKET sock, const wchar_t* server_str, const wchar_t* port_str, size_t port_str_len, UserAgentData* user_agent_data) {
		return new (::allocate<SocketData>()) SocketData(sock, server_str, port_str, port_str_len, user_agent_data);
	}

	static inline SocketData* allocate(SOCKET sock, const wchar_t* server_str, const wchar_t* port_str, size_t port_str_len, HINTERNET user_agent_data) {
		return allocate(sock, server_str, port_str, port_str_len, (UserAgentData*)user_agent_data);
	}
};

struct RequestData : FreeData<GenericDataType> {
	SocketData* socket; // 0x4
	size_t request_str_len; // 0x8
	wchar_t request_str[]; // 0xC

	inline constexpr operator HINTERNET() const {
		return (HINTERNET)this;
	}

	inline RequestData(SocketData* socket_data, size_t request_str_len)
		: socket(socket_data), request_str_len(request_str_len)
	{
		// Copying the data into the string is still done outside of this
		// TODO: More parameter packs?
	}

	inline ~RequestData() = default;

	static inline RequestData* allocate(SocketData* socket_data, size_t request_str_len) {
		return new (::allocate<RequestData, wchar_t>(request_str_len)) RequestData(socket_data, request_str_len);
	}

	static inline RequestData* allocate(HINTERNET socket_data, size_t request_str_len) {
		return allocate((SocketData*)socket_data, request_str_len);
	}
};

// This is only a separate struct so that the
// pointer can be blindly freed in WinHttpCloseHandle
struct WebSocketData : FreeData<GenericDataType> {
	SocketData* socket; // 0x4

	inline constexpr operator HINTERNET() const {
		return (HINTERNET)this;
	}

	inline WebSocketData(SocketData* socket_data) : socket(socket_data) {}

	inline ~WebSocketData() = default;

	static inline WebSocketData* allocate(SocketData* socket_data) {
		return new (::allocate<WebSocketData>()) WebSocketData(socket_data);
	}

	static inline WebSocketData* allocate(RequestData* request_data) {
		return allocate(request_data->socket);
	}

	static inline WebSocketData* allocate(HINTERNET request_data) {
		return allocate((RequestData*)request_data);
	}
};

static FILE* websocket_log = NULL;
static FILE* winsock_log = NULL;

/*

th19 info: WinHttpOpen(L"Photon", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0)

*/
dllexport HINTERNET WINAPI WinHttpOpen(LPCWSTR pszAgentW, DWORD, LPCWSTR, LPCWSTR, DWORD) {
	if (expect(!flags.initialized_winsock, false)) {
		flags.initialized_winsock = !WSAStartup(WINSOCK_VERSION, &idgaf_about_winsock_version);

		/*
		
		getaddrinfo still just crashes on Windows 7,
		so use this one-time setup code to patch
		the IAT with a replacement function

		*/
		PEB* peb = *(PEB* FS_RELATIVE*)0x30;
		IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)peb->Reserved3[1];
		IMAGE_NT_HEADERS* nt_header = based_pointer<IMAGE_NT_HEADERS>(dos_header, dos_header->e_lfanew);
		if (nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) {
			IMAGE_IMPORT_DESCRIPTOR* import_directory_entry = based_pointer<IMAGE_IMPORT_DESCRIPTOR>(dos_header, nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
			for (DWORD name_offset; name_offset = import_directory_entry->Name; ++import_directory_entry) {
				if (!stricmp(based_pointer<char>(dos_header, name_offset), "WS2_32.dll")) {
					IMAGE_THUNK_DATA* thunk = based_pointer<IMAGE_THUNK_DATA>(dos_header, import_directory_entry->FirstThunk);
					if (DWORD ordinal_first_thunk_offset = import_directory_entry->OriginalFirstThunk) {
						for (
							IMAGE_THUNK_DATA* ordinal_thunk = based_pointer<IMAGE_THUNK_DATA>(dos_header, ordinal_first_thunk_offset);
							ordinal_thunk->u1.Function;
							++ordinal_thunk, ++thunk
						) {
							if (!(ordinal_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)) {
								IMAGE_IMPORT_BY_NAME* import_by_name = based_pointer<IMAGE_IMPORT_BY_NAME>(dos_header, ordinal_thunk->u1.AddressOfData);
								if (import_by_name->Name[0] == '\0') {
									break;
								}
								if (!stricmp(import_by_name->Name, "getaddrinfo")) {
									goto thunk_detour;
								}
							} else if (thunk->u1.Function == (DWORD)&getaddrinfo) {
								goto thunk_detour;
							}
						}
					} else {
						for (
							;
							thunk->u1.Function;
							++thunk
						) {
							if ((void*)thunk->u1.Function == (void*)&getaddrinfo) {
thunk_detour:
								DWORD old_protect;
								VirtualProtect(&thunk->u1.Function, sizeof(thunk->u1.Function), PAGE_READWRITE, &old_protect);
								thunk->u1.Function = (uintptr_t)&getaddrinfo_replacement;
								VirtualProtect(&thunk->u1.Function, sizeof(thunk->u1.Function), old_protect, &old_protect);
								break;
							}
						}
					}
					break;
				}
			}
		}
		websocket_log = fopen("websocket_log.txt", "w");
		winsock_log = fopen("winsock_log.txt", "w");
	}
	if (expect(flags.initialized_winsock, true)) {
		return UserAgentData::allocate(pszAgentW);
	}
	SetLastError(ERROR_WINHTTP_INTERNAL_ERROR);
	return NULL;
}

static inline constexpr ADDRINFOW addr_hint = {
	.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
	.ai_family = AF_UNSPEC,
	.ai_socktype = SOCK_STREAM,
	.ai_protocol = IPPROTO_TCP
};

/*

th19 info: WinHttpConnect(this->http_open_handle, this->server_name.get_pointer(), this->port, 0)

This function seems to actually work :D

*/
dllexport HINTERNET WINAPI WinHttpConnect(HINTERNET user_agent_data_handle, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD) {

	// GetAddrInfoW just crashes for some reason when trying to
	// resolve a URL, so the DNS resolution needs to be done manually.
	// THIS IS DUMB

	PDNS_RECORDW records;
	wchar_t addr_buff[48];
	if (expect(DnsQuery_W(pswzServerName, DNS_TYPE_AAAA, DNS_QUERY_DUAL_ADDR, NULL, (PDNS_RECORD*)&records, NULL) == 0, true)) {
		PDNS_RECORDW cur_record = records;
		do {
			void* ipv4;
			switch (cur_record->wType) {
				[[unlikely]] default:
					halt_and_catch_fire();
				case DNS_TYPE_CNAME:
					break;
				case DNS_TYPE_AAAA:
					if (
						!(cur_record->Data.AAAA.Ip6Address.IP6Dword[0] == 0 &&
						cur_record->Data.AAAA.Ip6Address.IP6Dword[1] == 0 &&
						cur_record->Data.AAAA.Ip6Address.IP6Dword[2] == 0xFFFF0000)
					) {
						InetNtopW(AF_INET6, &cur_record->Data.AAAA.Ip6Address, addr_buff, countof(addr_buff));
						goto found_addr;
					} else if (1) {
						ipv4 = &cur_record->Data.AAAA.Ip6Address.IP6Dword[3];
					} else {
				case DNS_TYPE_A:
						ipv4 = &cur_record->Data.A.IpAddress;
					}
					InetNtopW(AF_INET, ipv4, addr_buff, countof(addr_buff));
					goto found_addr;
			}
		} while ((cur_record = cur_record->pNext));
		DnsFree(records, DnsFreeRecordList);
	}
ret_error:
	SetLastError(ERROR_WINHTTP_INTERNAL_ERROR);
	return NULL;
found_addr:
	DnsFree(records, DnsFreeRecordList);

#if !RESPECT_HTTPS
	if (expect(nServerPort == 19092 || nServerPort == 19093, true)) {
		nServerPort -= 10000;
	}
#endif
	wchar_t port_buffer[SocketData::port_str_buf_len] = {}; // Buffer slightly oversized for efficient XMM copy
	size_t port_str_len = uint16_to_strbuf(nServerPort, port_buffer);
	PADDRINFOW addrs;
	// GetAddrInfoW sucks
	if (expect(!GetAddrInfoW(addr_buff, port_buffer, &addr_hint, &addrs), true)) {
		do {
			SOCKET sock = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
			if (expect(sock != INVALID_SOCKET, true)) {
				if (expect(connect(sock, addrs->ai_addr, (int)addrs->ai_addrlen) != SOCKET_ERROR, true)) {
					FreeAddrInfoW(addrs);
					return SocketData::allocate(sock, pswzServerName, port_buffer, port_str_len, user_agent_data_handle);
				}
				closesocket(sock);
			}
		} while ((addrs = addrs->ai_next));
		FreeAddrInfoW(addrs);
	}
	goto ret_error;
}

#if RESPECT_HTTPS
static inline constexpr SCHANNEL_CRED tls_cred = {
	.dwVersion = SCHANNEL_CRED_VERSION,
	.dwFlags = SCH_USE_STRONG_CRYPTO |          // use only strong crypto alogorithms
			   SCH_CRED_AUTO_CRED_VALIDATION |  // automatically validate server certificate
			   SCH_CRED_NO_DEFAULT_CREDS,     // no client certificate authentication
	.grbitEnabledProtocols = SP_PROT_TLS1_2,  // allow only TLS v1.2
};


// Version 1 copied from this: https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d
// Version 1 sucks and didn't work
// 
// Version 2 copied from this: https://github.com/odzhan/shells/blob/master/s6/tls.c
static gnu_noinline bool fastcall initialize_tls(SocketData* socket_data) {
	if (expect(AcquireCredentialsHandleW(NULL, UNISP_NAME_W, SECPKG_CRED_OUTBOUND, NULL, (void*)&tls_cred, NULL, NULL, &socket_data->tls_handle, NULL) != SEC_E_OK, false)) {
		closesocket(socket_data->sock);
		return false;
	}

	cache_align char packet_buffer[TLS_MAX_PACKET_SIZE];

#define USE_IMPL_NUM 2

#if USE_IMPL_NUM == 2
	SecBuffer outbuffers[1] = { 0 };
	outbuffers[0].BufferType = SECBUFFER_TOKEN;
	outbuffers[0].pvBuffer = NULL;
	outbuffers[0].cbBuffer = 0;

	SecBufferDesc outdesc = { SECBUFFER_VERSION, ARRAYSIZE(outbuffers), outbuffers };

	DWORD flags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM | ISC_REQ_MANUAL_CRED_VALIDATION;

	SECURITY_STATUS sec = InitializeSecurityContextW(&socket_data->tls_handle,
														NULL,
														socket_data->server_str,
														flags,
														0,
														0,
														NULL,
														0,
														&socket_data->tls_context,
														&outdesc,
														&flags,
														NULL);
	if (sec == SEC_I_CONTINUE_NEEDED) {
		{
			char* buffer = (char*)outbuffers[0].pvBuffer;
			size_t size = outbuffers[0].cbBuffer;
			while (size != 0) {
				auto send_ret = send(socket_data->sock, buffer, size, 0);
				if (expect(send_ret <= 0, false)) {
					// failed with disconnect or error
					goto tls_fail;
				}
				size -= send_ret;
				buffer += send_ret;
			}
			FreeContextBuffer(outbuffers[0].pvBuffer);
		}
			

		size_t received = 0;
		SecBuffer inbuffers[2] = {};
		do {
			if (sec == SEC_E_INCOMPLETE_MESSAGE) {
				int recv_ret = recv(socket_data->sock, &packet_buffer[received], sizeof(packet_buffer) - received, 0);
				if (expect(recv_ret <= 0, false)) {
					// failed with disconnect or error
					goto tls_fail;
				}
				received += recv_ret;
			}

			inbuffers[0].BufferType = SECBUFFER_TOKEN;
			inbuffers[0].pvBuffer = packet_buffer;
			inbuffers[0].cbBuffer = received;
			inbuffers[1].BufferType = SECBUFFER_EMPTY;
			inbuffers[1].pvBuffer = NULL;
			inbuffers[1].cbBuffer = 0;

			outbuffers[0].BufferType = SECBUFFER_TOKEN;
			outbuffers[0].pvBuffer = NULL;
			outbuffers[0].cbBuffer = 0;

			SecBufferDesc indesc = { SECBUFFER_VERSION, ARRAYSIZE(inbuffers), inbuffers };
			outdesc = { SECBUFFER_VERSION, ARRAYSIZE(outbuffers), outbuffers };

			flags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM | ISC_REQ_MANUAL_CRED_VALIDATION;

			sec = InitializeSecurityContextW(&socket_data->tls_handle,
												&socket_data->tls_context,
												NULL,
												flags,
												0,
												0,
												&indesc,
												0,
												NULL,
												&outdesc,
												&flags,
												NULL);

			if (sec == SEC_E_OK || sec == SEC_I_CONTINUE_NEEDED) {
				char* buffer = (char*)outbuffers[0].pvBuffer;
				size_t size = outbuffers[0].cbBuffer;
				if (buffer) {
					while (size != 0) {
						auto send_ret = send(socket_data->sock, buffer, size, 0);
						if (expect(send_ret <= 0, false)) {
							// failed with disconnect or error
							FreeContextBuffer(outbuffers[0].pvBuffer);
							goto tls_fail;
						}
						size -= send_ret;
						buffer += send_ret;
					}
					FreeContextBuffer(outbuffers[0].pvBuffer);
				}
			}
			if (sec == SEC_E_INCOMPLETE_MESSAGE) {
				continue;
			}
			if (sec == SEC_E_OK) {
				QueryContextAttributesW(&socket_data->tls_context, SECPKG_ATTR_STREAM_SIZES, &socket_data->tls_sizes);
				return true;
			}
			if (FAILED(sec)) {
				break;
			}
			if (inbuffers[1].BufferType == SECBUFFER_EXTRA) {
				__builtin_memmove(packet_buffer, &packet_buffer[received - inbuffers[1].cbBuffer], inbuffers[1].cbBuffer);
				received = inbuffers[1].cbBuffer;
			} else {
				received = 0;
			}
		} while (sec == SEC_I_CONTINUE_NEEDED || sec == SEC_E_INCOMPLETE_MESSAGE || sec == SEC_I_INCOMPLETE_CREDENTIALS);
tls_fail:
		DeleteSecurityContext(&socket_data->tls_context);
	}
	FreeCredentialsHandle(&socket_data->tls_handle);
	closesocket(socket_data->sock);
	return false;
	
#elif USE_IMPL_NUM == 1
	
	CtxtHandle* tls_context_ptr = NULL;
	size_t received = 0;
	size_t total_received = 0;
	for (;;) {
		SecBuffer inbuffers[2] = { 0 };
		inbuffers[0].BufferType = SECBUFFER_TOKEN;
		inbuffers[0].pvBuffer = packet_buffer;
		inbuffers[0].cbBuffer = 0;
		inbuffers[1].BufferType = SECBUFFER_EMPTY;
		inbuffers[1].pvBuffer = NULL;
		inbuffers[1].cbBuffer = 0;

		SecBuffer outbuffers[1] = { 0 };
		outbuffers[0].BufferType = SECBUFFER_TOKEN;
		//outbuffers[0].pvBuffer = socket_data->send_packet_buffer;
		outbuffers[0].pvBuffer = NULL;
		outbuffers[0].cbBuffer = 0;

		SecBufferDesc indesc = { SECBUFFER_VERSION, ARRAYSIZE(inbuffers), inbuffers };
		SecBufferDesc outdesc = { SECBUFFER_VERSION, ARRAYSIZE(outbuffers), outbuffers };

		DWORD flags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;

		SECURITY_STATUS sec = InitializeSecurityContextW(&socket_data->tls_handle,
														 tls_context_ptr,
														 tls_context_ptr ? NULL : socket_data->server_str,
														 flags,
														 0,
														 0,
														 tls_context_ptr ? &indesc : NULL,
														 0,
														 tls_context_ptr ? NULL : &socket_data->tls_context,
														 &outdesc,
														 &flags,
														 NULL);
		tls_context_ptr = &socket_data->tls_context;
		if (inbuffers[1].BufferType == SECBUFFER_EXTRA) {
			__builtin_memmove(packet_buffer, &packet_buffer[received - inbuffers[1].cbBuffer], inbuffers[1].cbBuffer);
			received = inbuffers[1].cbBuffer;
		} else {
			received = 0;
		}
		switch (sec) {
			case SEC_E_OK: tls_success:
				QueryContextAttributesW(tls_context_ptr, SECPKG_ATTR_STREAM_SIZES, &socket_data->tls_sizes);
				return true;
			default: case SEC_I_INCOMPLETE_CREDENTIALS: tls_fail:
				if (tls_context_ptr) {
					DeleteSecurityContext(tls_context_ptr);
				}
				FreeCredentialsHandle(&socket_data->tls_handle);
				closesocket(socket_data->sock);
				return false;
			case SEC_I_CONTINUE_NEEDED: {
				// need to send data to server
				char* buffer = (char*)outbuffers[0].pvBuffer;
				size_t size = outbuffers[0].cbBuffer;

				while (size != 0) {
					auto send_ret = send(socket_data->sock, buffer, size, 0);
					if (expect(send_ret <= 0, false)) {
						// failed with disconnect or error
						FreeContextBuffer(outbuffers[0].pvBuffer);
						goto tls_fail;
					}
					size -= send_ret;
					buffer += send_ret;
				}
				FreeContextBuffer(outbuffers[0].pvBuffer);
				if (expect(size != 0, false)) {
					// failed to fully send data to server
					goto tls_fail;
				}
			}
				[[fallthrough]];
			case SEC_E_INCOMPLETE_MESSAGE: {
				if (expect(received == TLS_MAX_PACKET_SIZE, false)) {
					// failed with too much data
					goto tls_fail;
				}
				// AUTHENTICATION HANGS HERE
				// WHY AAAAAAAAAAAAAAA
				int recv_ret = recv(socket_data->sock, &packet_buffer[received], sizeof(packet_buffer) - received, 0);
				if (expect(recv_ret <= 0, false)) {
					// failed with disconnect or error
					goto tls_fail;
				}
				received += recv_ret;
				total_received += recv_ret;
				continue;
			}
		}
	}
#endif
}
#endif

static inline size_t write_to_socket(SocketData* socket_data, uint8_t* send_buffer, size_t send_length) {
	fprintf(winsock_log, "[SEND %19llu]: ", rdtsc());
	for (size_t i = 0; i < send_length; ++i) {
		fprintf(winsock_log, "%.2hhX", send_buffer[i]);
	}
	fputc('\n', winsock_log);
	fflush(winsock_log);

	size_t total_sent_length = 0;
#if RESPECT_HTTPS
	cache_align char packet_buffer[receive_buffer_size];
	while (send_length) {
#endif
		char* current_send_buffer = (char*)send_buffer;
		size_t current_buffer_read_length = send_length;
		size_t current_send_length = current_buffer_read_length;
#if RESPECT_HTTPS
		SecBuffer buffers[3];
		if (expect(socket_data->tls_in_use, true)) {
			current_send_buffer = packet_buffer;
			if (current_buffer_read_length > socket_data->tls_sizes.cbMaximumMessage) {
				current_buffer_read_length = socket_data->tls_sizes.cbMaximumMessage;
			}
			buffers[0].BufferType = SECBUFFER_STREAM_HEADER;
			buffers[0].pvBuffer = current_send_buffer;
			buffers[0].cbBuffer = socket_data->tls_sizes.cbHeader;
			buffers[1].BufferType = SECBUFFER_DATA;
			buffers[1].pvBuffer = &current_send_buffer[socket_data->tls_sizes.cbHeader];
			buffers[1].cbBuffer = current_buffer_read_length;
			buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;
			buffers[2].pvBuffer = &current_send_buffer[socket_data->tls_sizes.cbHeader + current_buffer_read_length];
			buffers[2].cbBuffer = socket_data->tls_sizes.cbTrailer;
			__builtin_memcpy(buffers[1].pvBuffer, send_buffer, current_buffer_read_length);
			SecBufferDesc desc = { SECBUFFER_VERSION, ARRAYSIZE(buffers), buffers };
			if (expect(EncryptMessage(&socket_data->tls_context, 0, &desc, 0) != SEC_E_OK, false)) {
				// this should not happen, but just in case check it
				return false;
			}
			current_send_length = buffers[0].cbBuffer + buffers[1].cbBuffer + buffers[2].cbBuffer;
		}

#endif
		size_t sent = 0;
		while (sent != current_send_length) {
			int send_ret = send(socket_data->sock, &current_send_buffer[sent], current_send_length - sent, 0);
			if (expect(send_ret <= 0, false)) {
				return 0;
			}
			sent += send_ret;
			total_sent_length += send_ret;
		}
#if RESPECT_HTTPS
		send_buffer += current_buffer_read_length;
		send_length -= current_buffer_read_length;
	}
#endif
	return total_sent_length;
}

// TODO: use this instead of direct recv calls to simplify TLS support?
static inline size_t read_from_socket(SocketData* socket_data, uint8_t* recv_buffer, size_t recv_length) {
#if RESPECT_HTTPS
	if (expect(!socket_data->tls_in_use, false)) {
#endif
		int recv_ret = recv(socket_data->sock, (char*)recv_buffer, recv_length, 0);
		if (expect(recv_ret <= 0, false)) {
			return 0;
		}
		return recv_ret;
#if RESPECT_HTTPS
	}
	size_t received_size = 0;
	size_t total_received = 0;

	uint8_t* initial_recv_buffer = recv_buffer;
	auto log_recv = [=](size_t length) {
		fprintf(winsock_log, "[RECV %19llu]: ", rdtsc());
		for (size_t i = 0; i < length; ++i) {
			fprintf(winsock_log, "%.2hhX", initial_recv_buffer[i]);
		}
		fputc('\n', winsock_log);
		fflush(winsock_log);
	};

	cache_align char packet_buffer[receive_buffer_size];
	while (recv_length) {

		if (total_received) {
			// Why?
			return total_received;
		}

		if (expect(received_size == sizeof(packet_buffer), false)) {
			// Is this even a possible error?
			return 0;
		}
		int recv_ret = recv(socket_data->sock, &packet_buffer[received_size], receive_buffer_size - received_size, 0);
		if (expect(recv_ret <= 0, false)) {
			return 0;
		}
		received_size += recv_ret;

		while (received_size) {

			SecBuffer buffers[4];
			buffers[0].BufferType = SECBUFFER_DATA;
			buffers[0].pvBuffer = packet_buffer;
			buffers[0].cbBuffer = received_size;
			buffers[1].BufferType = SECBUFFER_EMPTY;
			buffers[2].BufferType = SECBUFFER_EMPTY;
			buffers[3].BufferType = SECBUFFER_EMPTY;

			SecBufferDesc desc = { SECBUFFER_VERSION, ARRAYSIZE(buffers), buffers };

			SECURITY_STATUS sec = DecryptMessage(&socket_data->tls_context, &desc, 0, NULL);
			switch (sec) {
				case SEC_E_OK:
				{
					char* decrypted_data = (char*)buffers[1].pvBuffer;
					size_t decrypt_data_length = buffers[1].cbBuffer;
					size_t used_length = received_size - (buffers[3].BufferType == SECBUFFER_EXTRA ? buffers[3].cbBuffer : 0);

					size_t current_write_length;
					do {
						current_write_length = __min(recv_length, decrypt_data_length);
						__builtin_memcpy(recv_buffer, decrypted_data, current_write_length);
						recv_buffer += current_write_length;
						recv_length -= current_write_length;
						total_received += current_write_length;
						decrypt_data_length -= current_write_length;
						decrypted_data += current_write_length;
					} while (current_write_length != decrypt_data_length);
					__builtin_memmove(packet_buffer, &packet_buffer[used_length], received_size - used_length);
					received_size -= used_length;
					continue;
				}
				case SEC_I_CONTEXT_EXPIRED:
					log_recv(total_received);
					return total_received;
				default:
					return 0;
				case SEC_E_INCOMPLETE_MESSAGE:
					break;
			}
			break;
		}
	}
	log_recv(total_received);
	return total_received;
#endif
}


/*

th19 info: WinHttpOpenRequest(this->http_connect_handle, L"GET", this->__string_F0.get_pointer(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, this->use_https ? WINHTTP_FLAG_SECURE : 0)

Ignoring some parameters again since they always have the same value

*/
static inline constexpr wchar_t base64_lookup_str[] = {
	L'A',L'B',L'C',L'D',L'E',L'F',L'G',L'H',L'I',L'J',L'K',L'L',L'M',L'N',L'O',L'P',L'Q',L'R',L'S',L'T',L'U',L'V',L'W',L'X',L'Y',L'Z',
	L'a',L'b',L'c',L'd',L'e',L'f',L'g',L'h',L'i',L'j',L'k',L'l',L'm',L'n',L'o',L'p',L'q',L'r',L's',L't',L'u',L'v',L'w',L'x',L'y',L'z',
	L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'+',L'/'
};
dllexport HINTERNET WINAPI WinHttpOpenRequest(HINTERNET socket_data_handle, LPCWSTR, LPCWSTR pwszObjectName, LPCWSTR, LPCWSTR, LPCWSTR FAR*, DWORD dwFlags) {

	SocketData* socket_data = (SocketData*)socket_data_handle;

#if RESPECT_HTTPS
	if (!socket_data->tls_in_use && dwFlags) {
		socket_data->tls_in_use = true;
		if (expect(!socket_data->tls_initialized, false)) {
			if (expect(!initialize_tls(socket_data), false)) {
				return NULL;
			}
			socket_data->tls_initialized = true;
		}
	}
#endif

	if (expect(!socket_data->generated_nonce_key, false)) {
		socket_data->generated_nonce_key = true;

		// TODO: better RNG
		(void)rand_s(&socket_data->nonce_key_dwords[0]);
		(void)rand_s(&socket_data->nonce_key_dwords[1]);
		(void)rand_s(&socket_data->nonce_key_dwords[2]);
		(void)rand_s(&socket_data->nonce_key_dwords[3]);

		size_t str_write = 0;
		// This loop gets optimized to remove the last two
		// iterations anyway, so I don't feel like tweaking it
		for (size_t i = 0; i < nonce_key_encode_bits; i += 6) {
			socket_data->nonce_key_str[str_write++] = base64_lookup_str[socket_data->nonce_key_wide >> i & 0b111111];
		}
		socket_data->nonce_key_str[22] = L'=';
		socket_data->nonce_key_str[23] = L'=';
	}

	size_t path_len = str_len(pwszObjectName);

	size_t request_len = countof(request_header_part_1) + path_len +
						 countof(request_header_part_2) + socket_data->user_agent_str_len +
						 countof(request_header_part_3) + countof(socket_data->nonce_key_str) +
						 countof(request_header_part_4) + socket_data->server_str_len +
						 countof(request_header_part_5) + socket_data->port_str_len +
						 countof(request_header_part_6);
	RequestData* request_data = RequestData::allocate(socket_data, request_len);
	build_str(request_data->request_str,
			  request_header_part_1,
			  pwszObjectName, path_len,
			  request_header_part_2,
			  socket_data->user_agent_str, socket_data->user_agent_str_len,
			  request_header_part_3,
			  socket_data->nonce_key_str,
			  request_header_part_4,
			  socket_data->server_str, socket_data->server_str_len,
			  request_header_part_5,
			  socket_data->port_str, socket_data->port_str_len,
			  request_header_part_6
	);
	return request_data;
}

/*

th19 info: WinHttpSetOption(http_request_handle, WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, NULL, 0)

Already handled as part of WinHttpOpenRequest, so kinda a NOP?

*/
dllexport BOOL WINAPI WinHttpSetOption(HINTERNET, DWORD, LPVOID, DWORD) {
	return TRUE;
}

/*

th19 info:

For some reason the game doesn't set the length of the extra headers, but I guess it works anyway? Weird

*/
dllexport BOOL WINAPI WinHttpSendRequest(HINTERNET request_data_handle, LPCWSTR lpszHeaders, DWORD, LPVOID, DWORD, DWORD, DWORD_PTR) {


	size_t extra_header_length = str_len(lpszHeaders);

	const RequestData* request_data = (const RequestData*)request_data_handle;

	size_t full_request_length = request_data->request_str_len +
								 extra_header_length +
								 countof(request_header_part_7);

	wchar_t* full_request = allocate<wchar_t>(full_request_length);
	build_str(full_request,
			  request_data->request_str, request_data->request_str_len,
			  lpszHeaders, extra_header_length,
			  request_header_part_7
	);

	// HORRIBLE HACK
	// I don't want to bother converting ascii-only-UTF16
	// to UTF8 the proper way until I know everything else works.
	uint8_t* evil_garbage = allocate<uint8_t>(full_request_length);
	for (size_t i = 0; i < full_request_length; ++i) {
		wchar_t jank = full_request[i];
		evil_garbage[i] = (uint8_t)jank;
		if (expect(jank != (uint8_t)jank, false)) {
			halt_and_catch_fire(); // crap it's not ascii
			unreachable;
		}
	}

	SocketData* socket_data = request_data->socket;
	SOCKET sock = socket_data->sock;

	char* send_buffer = (char*)evil_garbage;
	size_t send_length = full_request_length;

	if (expect(!write_to_socket(socket_data, (uint8_t*)send_buffer, send_length), false)) {
		free(evil_garbage);
		free(full_request);
		SetLastError(ERROR_WINHTTP_INTERNAL_ERROR);
		return false;
	}
	free(evil_garbage);
	free(full_request);
	return true;
}



/*

th19 info: WinHttpReceiveResponse(this->http_request_handle, NULL)

*/
dllexport BOOL WINAPI WinHttpReceiveResponse(HINTERNET request_data_handle, LPVOID) {

	const RequestData* request_data = (const RequestData*)request_data_handle;

	uint8_t receive_buffer[receive_buffer_size];

	if (expect(!read_from_socket(request_data->socket, receive_buffer, receive_buffer_size), false)) {
		SetLastError(ERROR_WINHTTP_INTERNAL_ERROR);
		return false;
	}

	return true;
}


/*

th19 info: WinHttpWebSocketCompleteUpgrade(this->http_request_handle, NULL)

*/
dllexport HINTERNET WINAPI WinHttpWebSocketCompleteUpgrade(HINTERNET request_data_handle, DWORD_PTR) {
	return WebSocketData::allocate(request_data_handle);
}

enum WebSocketOpcode {
	ContinuationFrame = 0,
	TextFrame = 1,
	BinaryFrame = 2,
	ConnectionClose = 8,
	// Is ping/pong handling part of WinHTTP?
	// Or is the application supposed to do that itself? Crap, IDK
	// Update: Looks like Photon doesn't send any ping/pong frames anyway
	Ping = 9,
	Pong = 10
};
enum WebSocketFrameLength : uint8_t {
	MediumFrame = 126,
	BigFrame = 127
};

union WebSocketFrameBig {
	struct {
		struct {
			uint8_t opcode : 4;
			uint8_t reserved : 3;
			uint8_t final_fragment : 1;
		};
		struct {
			uint8_t payload_length_dummy : 7;
			uint8_t masked_data : 1;
		};
		uint64_t payload_length packed_field;
	};
	UBitInt(80) header_with_length;
};
ValidateFieldOffset32(0x2, WebSocketFrameBig, payload_length);

union WebSocketFrameMedium {
	struct {
		struct {
			uint8_t opcode : 4;
			uint8_t reserved : 3;
			uint8_t final_fragment : 1;
		};
		struct {
			uint8_t payload_length_dummy : 7;
			uint8_t masked_data : 1;
		};
		uint16_t payload_length;
	};
	uint32_t header_with_length;
};
ValidateFieldOffset32(0x2, WebSocketFrameMedium, payload_length);

union WebSocketFrameSmall {
	struct {
		struct {
			uint8_t opcode : 4;
			uint8_t reserved : 3;
			uint8_t final_fragment : 1;
		};
		struct {
			uint8_t payload_length : 7;
			uint8_t masked_data : 1;
		};
	};
	uint16_t header_with_length;
};

struct WebSocketFrame {

	inline WebSocketFrameSmall* as_small_frame() {
		return (WebSocketFrameSmall*)this;
	}
	inline WebSocketFrameMedium* as_medium_frame() {
		return (WebSocketFrameMedium*)this;
	}
	inline WebSocketFrameBig* as_big_frame() {
		return (WebSocketFrameBig*)this;
	}

	inline DWORD recv_data(SocketData* socket_data, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType) {
		

		
		uint8_t* data = (uint8_t*)this;

		size_t recv_ret = read_from_socket(socket_data, data, receive_buffer_size);
		if (expect(!recv_ret, false)) {
			return ERROR_INVALID_OPERATION;
		}

		if (expect(recv_ret == receive_buffer_size, false)) {
			// IDK what the proper way to size these
			// buffers is, so just die if it's too big
			halt_and_catch_fire();
		}
		WebSocketFrameSmall* frame = this->as_small_frame();
		if (expect(!frame->final_fragment, false)) {
			// Do I have to implement fragmented frames?
			halt_and_catch_fire();
		}

		uint64_t data_length;
		switch (uint8_t payload_length = frame->payload_length) {
			default:
				data_length = payload_length;
				data += sizeof(WebSocketFrameSmall);
				break;
			case MediumFrame:
				data_length = __builtin_bswap16(this->as_medium_frame()->payload_length);
				data += sizeof(WebSocketFrameMedium);
				break;
			case BigFrame:
				data_length = __builtin_bswap64(this->as_big_frame()->payload_length);
				data += sizeof(WebSocketFrameBig);
				break;
		}
		if (expect(data_length > dwBufferLength, false)) {
			// Docs don't say what happens when the buffer
			// is too small, so I made something up
			return ERROR_INVALID_OPERATION;
		}

		uint32_t mask;
		bool is_masked = frame->masked_data;
		if (is_masked) {
			mask = *(unaligned uint32_t*)data;
			data += sizeof(uint32_t);
			for (size_t i = 0; i < data_length; i += 4) {
				*(uint32_t*)&data[i] ^= mask;
			}
		}

		__builtin_memcpy(pvBuffer, data, data_length);

		*pdwBytesRead = data_length;

		fprintf(websocket_log, "[RECV %19llu]: ", rdtsc());
		for (size_t i = 0; i < data_length; ++i) {
			fprintf(websocket_log, "%.2hhX", data[i]);
		}
		fputc('\n', websocket_log);
		fflush(websocket_log);

		*peBufferType = WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE;

		return NO_ERROR;
	}
};

dllexport DWORD WINAPI WinHttpWebSocketReceive(HINTERNET hWebSocket, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType) {
	
	uint8_t receive_buffer[receive_buffer_size];
	WebSocketFrame* frame = (WebSocketFrame*)receive_buffer;

	WebSocketData* web_socket_data = (WebSocketData*)hWebSocket;
	SocketData* socket_data = web_socket_data->socket;

	return frame->recv_data(socket_data, pvBuffer, dwBufferLength, pdwBytesRead, peBufferType);
}


/*

th19 info: WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE

*/
dllexport DWORD WINAPI WinHttpWebSocketSend(HINTERNET hWebSocket, WINHTTP_WEB_SOCKET_BUFFER_TYPE, PVOID pvBuffer, DWORD dwBufferLength) {
	size_t frame_size = sizeof(uint32_t) + dwBufferLength; // mask_size
	uint8_t header_length;
	switch (dwBufferLength) {
		case 0 ... 125:
			header_length = dwBufferLength;
			frame_size += sizeof(WebSocketFrameSmall);
			break;
		case 126 ... UINT16_MAX:
			header_length = 126;
			frame_size += sizeof(WebSocketFrameMedium);
			break;
		default:
			header_length = 127;
			frame_size += sizeof(WebSocketFrameBig);
			break;
	}
	uint8_t frame[frame_size];
	WebSocketFrameSmall* frame_header = (WebSocketFrameSmall*)frame;
	frame_header->final_fragment = true;
	frame_header->reserved = 0;
	frame_header->opcode = BinaryFrame;
	frame_header->masked_data = true;
	frame_header->payload_length = header_length;
	uint8_t* frame_data = frame + sizeof(WebSocketFrameSmall);
	switch (header_length) {
		case 126:
			*(uint16_t*)frame_data = __builtin_bswap16((uint16_t)dwBufferLength);
			frame_data += sizeof(uint16_t);
			break;
		case 127:
			*(uint64_t*)frame_data = __builtin_bswap64((uint64_t)dwBufferLength);
			frame_data += sizeof(uint64_t);
			break;
	}
	// Yes, I hardcoded the masking key as 0
	// No, I don't give a crap about it
	*(uint32_t*)frame_data = 0;
	frame_data += sizeof(uint32_t);
	__builtin_memcpy(frame_data, pvBuffer, dwBufferLength);

	fprintf(websocket_log, "[SEND %19llu]: ", rdtsc());
	for (size_t i = 0; i < dwBufferLength; ++i) {
		fprintf(websocket_log, "%.2hhX", ((uint8_t*)pvBuffer)[i]);
	}
	fputc('\n', websocket_log);
	fflush(websocket_log);

	WebSocketData* web_socket_data = (WebSocketData*)hWebSocket;

	if (expect(!write_to_socket(web_socket_data->socket, (uint8_t*)frame, frame_size), false)) {
		return ERROR_INVALID_OPERATION;
	}

	return NO_ERROR;
}

struct WebSocketFrameClose {
	WebSocketFrameSmall header;
	uint32_t key;
};

static inline constexpr WebSocketFrameClose close_frame = {
	.header = {
		.final_fragment = true,
		.reserved = 0,
		.opcode = ConnectionClose,
		.masked_data = true,
		.payload_length = 0
	},
	.key = 0
};

/*

th19 info: WinHttpWebSocketClose(this->websocket, 1000, NULL, NULL);

Just ignore the reason since it doesn't matter

*/
dllexport DWORD WINAPI WinHttpWebSocketClose(HINTERNET hWebSocket, USHORT, PVOID, DWORD) {
	if (WebSocketData* web_socket_data = (WebSocketData*)hWebSocket) {
		SocketData* socket_data = web_socket_data->socket;

		if (expect(!write_to_socket(socket_data, (uint8_t*)&close_frame, sizeof(close_frame)), false)) {
			return ERROR_INVALID_OPERATION;
		}

#if RESPECT_HTTPS
		if (expect(socket_data->tls_in_use, true)) {
			DWORD type = SCHANNEL_SHUTDOWN;

			SecBuffer inbuffers[1];
			inbuffers[0].BufferType = SECBUFFER_TOKEN;
			inbuffers[0].pvBuffer = &type;
			inbuffers[0].cbBuffer = sizeof(DWORD);

			SecBufferDesc indesc = { SECBUFFER_VERSION, ARRAYSIZE(inbuffers), inbuffers };
			ApplyControlToken(&socket_data->tls_context, &indesc);

			SecBuffer outbuffers[1];
			outbuffers[0].BufferType = SECBUFFER_TOKEN;

			SecBufferDesc outdesc = { SECBUFFER_VERSION, ARRAYSIZE(outbuffers), outbuffers };

			DWORD flags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;
			SECURITY_STATUS sec = InitializeSecurityContextW(&socket_data->tls_handle,
															 &socket_data->tls_context,
															 NULL,
															 flags,
															 0,
															 0,
															 &outdesc,
															 0,
															 NULL,
															 &outdesc,
															 &flags,
															 NULL);
			if (expect(sec == SEC_E_OK, true)) {
				char* buffer = (char*)outbuffers[0].pvBuffer;
				size_t size = outbuffers[0].cbBuffer;
				while (size != 0) {
					int send_ret = send(socket_data->sock, buffer, size, 0);
					if (expect(send_ret <= 0, false)) {
						// ignore any failures socket will be closed anyway
						break;
					}
					buffer += send_ret;
					size -= send_ret;
				}
				FreeContextBuffer(outbuffers[0].pvBuffer);
			}
			shutdown(socket_data->sock, SD_BOTH);
			DeleteSecurityContext(&socket_data->tls_context);
			FreeCredentialsHandle(&socket_data->tls_handle);
		}
#endif
	}
	return NO_ERROR;
}

/*

th19 info: Just called as a basic destructor for handle values

*/
dllexport BOOL WINAPI WinHttpCloseHandle(HINTERNET hInternet) {
	if (FreeData<>* free_data = (FreeData<>*)hInternet) {
		free_data->cleanup();
		free(free_data);
	}
	return TRUE;
}