#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

#include <stdlib.h>
#include <stdint.h>
#include <utility>

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

#define unused gnu_attr(unused)

#define static_str_len(str) (countof(str)-1)

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

template <typename T1, typename T2>
static inline T1* build_str(T1* buffer, T2* str, size_t len) {
	return str_copy(buffer, str, len);
}
template <typename T1, typename T2, typename ... StrsT>
static inline T1* build_str(T1* buffer, T2* str, size_t len, StrsT&&... next_views) {
	return build_str(str_copy(buffer, str, len), std::forward<StrsT>(next_views)...);
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

template<typename T>
static inline T* allocate() {
	return (T*)malloc(sizeof(T));
}

template<typename T>
static inline T* allocate(size_t count) {
	return (T*)malloc(sizeof(T[count]));
}

template<typename T1, typename T2>
static inline T1* allocate(size_t count) {
	return (T1*)malloc(sizeof(T1) + sizeof(T2[count]));
}

// Status: DONE
dllexport BOOL WINAPI WinHttpCloseHandle(HINTERNET hInternet) asm("_WinHttpCloseHandle");
// Status: 
dllexport HINTERNET WINAPI WinHttpConnect(HINTERNET hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved) asm("_WinHttpConnect");
// Status: 
dllexport DWORD WINAPI WinHttpWebSocketClose(HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength) asm("_WinHttpWebSocketClose");
// Status: 
dllexport DWORD WINAPI WinHttpWebSocketReceive(HINTERNET hWebSocket, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType) asm("_WinHttpWebSocketReceive");
// Status: 
dllexport DWORD WINAPI WinHttpWebSocketSend(HINTERNET hWebSocket, WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType, PVOID pvBuffer, DWORD dwBufferLength) asm("_WinHttpWebSocketSend");
// Status: 
dllexport HINTERNET WINAPI WinHttpWebSocketCompleteUpgrade(HINTERNET hRequest, DWORD_PTR pContext) asm("_WinHttpWebSocketCompleteUpgrade");
// Status: 
dllexport BOOL WINAPI WinHttpReceiveResponse(HINTERNET hRequest, LPVOID lpReserved) asm("_WinHttpReceiveResponse");
// Status: 
dllexport BOOL WINAPI WinHttpSendRequest(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext) asm("_WinHttpSendRequest");
// Status: IDK what I'm doing
dllexport HINTERNET WINAPI WinHttpOpenRequest(HINTERNET hConnect, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, LPCWSTR pwszVersion, LPCWSTR pwszReferrer, LPCWSTR FAR* ppwszAcceptTypes, DWORD dwFlags) asm("_WinHttpOpenRequest");
// Status: Maybe a NOP
dllexport BOOL WINAPI WinHttpSetOption(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength) asm("_WinHttpSetOption");
// Status: DONE(?)
dllexport HINTERNET WINAPI WinHttpOpen(LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags) asm("_WinHttpOpen");


struct StaticFlags {
	/*
	union {
		uint32_t flags;
		struct {
		};
	};
	*/
	bool initialized_winsock;
};

static inline constinit StaticFlags flags{
	.initialized_winsock = false
};

#define not_null_hinternet ((HINTERNET)1)

// This is a static variable so that stack space
// doesn't need to be allocated for it every single time
static WSADATA idgaf_about_winsock_version;

#define USE_STRING_BUILDER 1

#define USE_FACTORIES 1

//#define NULL_TERMINATE_STRINGS 1

#if NULL_TERMINATE_STRINGS
static inline constexpr bool include_null_terminator = true;
#else
static inline constexpr bool include_null_terminator = false;
#endif
static inline constexpr const wchar_t null_string[] = L"";


//#define RESPECT_HTTPS 1

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
		return new (::allocate<UserAgentData, wchar_t>(user_agent_str_len + include_null_terminator)) UserAgentData(str, user_agent_str_len + include_null_terminator);
	}
};
ValidateFieldOffset32(0x4, UserAgentData, user_agent_str_len);

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
	CredHandle tls_handle;
	CtxtHandle tls_context;
	SecPkgContext_StreamSizes tls_sizes;
	cache_align char recv_packet_buffer[TLS_MAX_PACKET_SIZE];
	//cache_align char send_packet_buffer[49152]; // Supposedly this buffer needs to be big?
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
		wchar_t* server_str = ::allocate<wchar_t>(server_str_len + include_null_terminator);
		this->server_str = server_str;
		str_copy(server_str, _server_str, server_str_len + include_null_terminator);
	}

	inline ~SocketData() {
		free(this->server_str);
		// TODO: Close socket like a sane person
		//closesocket(this->sock);
	}

	static inline SocketData* allocate(SOCKET sock, const wchar_t* server_str, const wchar_t* port_str, size_t port_str_len, UserAgentData* user_agent_data) {
		return new (::allocate<SocketData>()) SocketData(sock, server_str, port_str, port_str_len, user_agent_data);
	}

	static inline SocketData* allocate(SOCKET sock, const wchar_t* server_str, const wchar_t* port_str, size_t port_str_len, HINTERNET user_agent_data) {
		return allocate(sock, server_str, port_str, port_str_len, (UserAgentData*)user_agent_data);
	}
};


// These parts are in the open request func
static inline constexpr const wchar_t request_header_part_1[] =	L"GET ";
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
// This part is in the send request func
static inline constexpr const wchar_t request_header_part_7[] = L"\r\n\r\n";

struct RequestData : FreeData<GenericDataType> {
	SocketData* socket; // 0x4
	size_t request_str_len; // 0x8
	wchar_t request_str[]; // 0xC

	inline constexpr operator HINTERNET() const {
		return (HINTERNET)this;
	}

	inline RequestData(SocketData* socket_data, size_t request_str_len)
		: socket(socket_data), request_str_len(request_str_len)
	{}

	inline ~RequestData() = default;

	static inline RequestData* allocate(SocketData* socket_data, size_t request_str_len) {
		return new (::allocate<RequestData, wchar_t>(request_str_len + include_null_terminator)) RequestData(socket_data, request_str_len + include_null_terminator);
	}

	static inline RequestData* allocate(HINTERNET socket_data, size_t request_str_len) {
		return allocate((SocketData*)socket_data, request_str_len);
	}
};

// This is only a separate allocation so that I don't
// need to keep track of which pointer is which when
// freeing them in WinHttpCloseHandle
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

/*

th19 info: WinHttpOpen(L"Photon", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0)

*/
dllexport HINTERNET WINAPI WinHttpOpen(LPCWSTR pszAgentW, DWORD, LPCWSTR, LPCWSTR, DWORD) {
	if (expect(!flags.initialized_winsock, false)) {
		flags.initialized_winsock = !WSAStartup(WINSOCK_VERSION, &idgaf_about_winsock_version);
	}
	if (expect(flags.initialized_winsock, true)) {
#if USE_FACTORIES
		return UserAgentData::allocate(pszAgentW);
#else
		size_t agent_str_len = str_len(pszAgentW);
		//UserAgentData* user_agent_data = (UserAgentData*)malloc(sizeof(UserAgentData) + sizeof(wchar_t[agent_str_len + include_null_terminator]));
		UserAgentData* user_agent_data = allocate<UserAgentData, wchar_t>(agent_str_len + include_null_terminator);
		user_agent_data->data_type = GenericDataType;
		user_agent_data->user_agent_str_len = agent_str_len;
		str_copy(user_agent_data->user_agent_str, pszAgentW, agent_str_len + include_null_terminator);
		return user_agent_data;
#endif
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
				default:
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
	//if (expect(!GetAddrInfoW(pswzServerName, port_buffer, &addr_hint, &addrs), true)) {
	if (expect(!GetAddrInfoW(addr_buff, port_buffer, &addr_hint, &addrs), true)) {
		do {
			SOCKET sock = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
			if (expect(sock != INVALID_SOCKET, true)) {
				if (expect(connect(sock, addrs->ai_addr, (int)addrs->ai_addrlen) != SOCKET_ERROR, true)) {
					FreeAddrInfoW(addrs);
#if USE_FACTORIES
					return SocketData::allocate(sock, pswzServerName, port_buffer, port_str_len, user_agent_data_handle);
#else
					SocketData* socket_data = (SocketData*)malloc(sizeof(SocketData));
					//SocketData* socket_data = allocate<SocketData>();
					socket_data->data_type = SocketDataType;
					//socket_data->inner_free_count = 1;
					socket_data->nonce_key_dwords[4] = 0;
					socket_data->upgraded_to_websocket = false;
					socket_data->generated_nonce_key = false;
#if RESPECT_HTTPS
					socket_data->tls_in_use = false;
					socket_data->tls_initialized = false;
#endif
					socket_data->port_str_len = port_str_len;
					socket_data->sock = sock;
					str_copy(socket_data->port_str, port_buffer, countof(port_buffer));
					size_t server_str_len = str_len(pswzServerName);
					socket_data->server_str_len = server_str_len;
					wchar_t* server_str = (wchar_t*)malloc(sizeof(wchar_t[server_str_len + include_null_terminator]));
					//wchar_t* server_str = allocate<wchar_t>(server_str_len + include_null_terminator);
					socket_data->server_str = server_str;
					str_copy(server_str, pswzServerName, server_str_len + include_null_terminator);
					UserAgentData* user_agent_data = (UserAgentData*)user_agent_data_handle;
					socket_data->user_agent_str = user_agent_data->user_agent_str;
					socket_data->user_agent_str_len = user_agent_data->user_agent_str_len;
					return socket_data;
#endif
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
	.dwFlags = SCH_USE_STRONG_CRYPTO          // use only strong crypto alogorithms
				| SCH_CRED_AUTO_CRED_VALIDATION  // automatically validate server certificate
				| SCH_CRED_NO_DEFAULT_CREDS,     // no client certificate authentication
	.grbitEnabledProtocols = SP_PROT_TLS1_2,  // allow only TLS v1.2
};

// Shamelessly copied from this random gist: https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d
static gnu_noinline bool fastcall initialize_tls(SocketDataPtr socket_data) {
	if (expect(AcquireCredentialsHandleW(NULL, UNISP_NAME_W, SECPKG_CRED_OUTBOUND, NULL, (void*)&tls_cred, NULL, NULL, &socket_data->tls_handle, NULL) != SEC_E_OK, false)) {
		closesocket(socket_data->sock);
		return false;
	}

	CtxtHandle* tls_context_ptr = NULL;
	unsigned long received = 0;
	for (;;) {
		SecBuffer inbuffers[2] = { 0 };
		inbuffers[0].BufferType = SECBUFFER_TOKEN;
		inbuffers[0].pvBuffer = socket_data->recv_packet_buffer;
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
			__builtin_memmove(socket_data->recv_packet_buffer, &socket_data->recv_packet_buffer[received - inbuffers[1].cbBuffer], inbuffers[1].cbBuffer);
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
				unsigned long size = outbuffers[0].cbBuffer;

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
				int recv_ret = recv(socket_data->sock, &socket_data->recv_packet_buffer[received], sizeof(socket_data->recv_packet_buffer) - received, 0);
				if (expect(recv_ret <= 0, false)) {
					// failed with disconnect or error
					goto tls_fail;
				}
				received += recv_ret;
				continue;
			}
		}
	}

}
#endif

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

	size_t request_len = static_str_len(request_header_part_1) + path_len +
						 static_str_len(request_header_part_2) + socket_data->user_agent_str_len +
						 static_str_len(request_header_part_3) + countof(socket_data->nonce_key_str) +
						 static_str_len(request_header_part_4) + socket_data->server_str_len +
						 static_str_len(request_header_part_5) + socket_data->port_str_len +
						 static_str_len(request_header_part_6);
#if USE_FACTORIES
	RequestData* request_data = RequestData::allocate(socket_data, request_len);
#else
	RequestData* request_data = (RequestData*)malloc(sizeof(RequestData) + sizeof(wchar_t[request_len + include_null_terminator]));
	//RequestData* request_data = allocate<RequestData, wchar_t>(request_len + include_null_terminator);
	request_data->data_type = GenericDataType;
	request_data->socket = socket_data;
	request_data->request_str_len = request_len;
#endif
	wchar_t* request_str_write = request_data->request_str;

	// I swear this isn't as bad as it looks
#if USE_STRING_BUILDER
	build_str(request_str_write,
			  request_header_part_1, static_str_len(request_header_part_1),
			  pwszObjectName, path_len,
			  request_header_part_2, static_str_len(request_header_part_2),
			  socket_data->user_agent_str, socket_data->user_agent_str_len,
			  request_header_part_3, static_str_len(request_header_part_3),
			  socket_data->nonce_key_str, countof(socket_data->nonce_key_str),
			  request_header_part_4, static_str_len(request_header_part_4),
			  socket_data->server_str, socket_data->server_str_len,
			  request_header_part_5, static_str_len(request_header_part_5),
			  socket_data->port_str, socket_data->port_str_len,
			  request_header_part_6, static_str_len(request_header_part_6)
#if NULL_TERMINATE_STRINGS
			  , null_string, countof(null_string)
#endif
	);
#else
	if constexpr (include_null_terminator) {
		request_str_write[request_len] = L'\0';
	}
	request_str_write = str_copy(request_str_write, request_header_part_1, static_str_len(request_header_part_1));
	request_str_write = str_copy(request_str_write, pwszObjectName, path_len);
	request_str_write = str_copy(request_str_write, request_header_part_2, static_str_len(request_header_part_2));
	request_str_write = str_copy(request_str_write, socket_data->user_agent_str, socket_data->user_agent_str_len);
	request_str_write = str_copy(request_str_write, request_header_part_3, static_str_len(request_header_part_3));
	request_str_write = str_copy(request_str_write, socket_data->nonce_key_str, countof(socket_data->nonce_key_str));
	request_str_write = str_copy(request_str_write, request_header_part_4, static_str_len(request_header_part_4));
	request_str_write = str_copy(request_str_write, socket_data->server_str, socket_data->server_str_len);
	request_str_write = str_copy(request_str_write, request_header_part_5, static_str_len(request_header_part_5));
	request_str_write = str_copy(request_str_write, socket_data->port_str, socket_data->port_str_len);
	request_str_write = str_copy(request_str_write, request_header_part_6, static_str_len(request_header_part_6));
#endif
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

static inline constexpr size_t receive_buffer_size = UINT16_MAX + 1;

static char receive_buffer[receive_buffer_size];

dllexport BOOL WINAPI WinHttpSendRequest(HINTERNET request_data_handle, LPCWSTR lpszHeaders, DWORD, LPVOID, DWORD, DWORD, DWORD_PTR) {


	size_t extra_header_length = str_len(lpszHeaders);

	const RequestData* request_data = (const RequestData*)request_data_handle;

	size_t full_request_length = request_data->request_str_len +
								 extra_header_length + static_str_len(request_header_part_7);

	wchar_t* full_request = allocate<wchar_t>(full_request_length + include_null_terminator);
	wchar_t* full_request_write = full_request;
#if USE_STRING_BUILDER
	build_str(full_request,
			  (wchar_t*)request_data->request_str, request_data->request_str_len,
			  lpszHeaders, extra_header_length,
			  request_header_part_7, static_str_len(request_header_part_7)
#if NULL_TERMINATE_STRINGS
			  , null_string, countof(null_string)
#endif
	);
#else
	if constexpr (include_null_terminator) {
		full_request_write[full_request_length] = L'\0';
	}
	full_request_write = str_copy(full_request_write, request_data->request_str, request_data->request_str_len);
	full_request_write = str_copy(full_request_write, lpszHeaders, extra_header_length);
	full_request_write = str_copy(full_request_write, request_header_part_7, static_str_len(request_header_part_7));
#endif

	// HORRIBLE HACK
	// I don't want to bother converting ascii-only-UTF16
	// to UTF8 the proper way until I know everything else works.
	uint8_t* evil_garbage = (uint8_t*)malloc(sizeof(uint8_t[full_request_length + include_null_terminator]));
	//uint8_t* evil_garbage = allocate<uint8_t>(full_request_length + include_null_terminator);
	for (size_t i = 0; i < (full_request_length + include_null_terminator); ++i) {
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

	//bool func_success = true;

#if RESPECT_HTTPS
	while (send_length) {
#endif
		char* current_send_buffer = send_buffer;
		size_t current_send_length = send_length;
#if RESPECT_HTTPS
		SecBuffer buffers[3];
		if (socket_data->tls_in_use) {
			current_send_buffer = socket_data->recv_packet_buffer;
			if (send_length > socket_data->tls_sizes.cbMaximumMessage) {
				send_length = socket_data->tls_sizes.cbMaximumMessage;
			}
			buffers[0].BufferType = SECBUFFER_STREAM_HEADER;
			buffers[0].pvBuffer = current_send_buffer;
			buffers[0].cbBuffer = socket_data->tls_sizes.cbHeader;
			buffers[1].BufferType = SECBUFFER_DATA;
			buffers[1].pvBuffer = &current_send_buffer[socket_data->tls_sizes.cbHeader];
			buffers[1].cbBuffer = current_send_length;
			buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;
			buffers[2].pvBuffer = &current_send_buffer[socket_data->tls_sizes.cbHeader + current_send_length];
			buffers[2].cbBuffer = socket_data->tls_sizes.cbTrailer;
			__builtin_memcpy(buffers[1].pvBuffer, send_buffer, current_send_length);
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
			int send_ret = send(sock, &current_send_buffer[sent], current_send_length - sent, 0);
			if (expect(send_ret <= 0, false)) {
				free(evil_garbage);
				free(full_request);
				SetLastError(ERROR_WINHTTP_INTERNAL_ERROR);
				return false;
			}
			sent += send_ret;
		}
#if RESPECT_HTTPS
		send_buffer += current_send_length;
		send_length -= current_send_length;
	}
#endif

	free(evil_garbage);
	free(full_request);

	/*
	do {
		auto ret = recv(sock, receive_buffer, countof(receive_buffer), 0);
		if (ret > 0) {
			//printf("Received: %d\n", ret);
			
			//__asm__("int3");
			break;
		} else if (ret == 0) {
			printf("Disconnected\n");
			halt_and_catch_fire();
			break;
		} else {
			printf("Error: %d\n", ret);
			halt_and_catch_fire();
		}
	} while (1);
	*/
	

	return true;
}



/*

th19 info: WinHttpReceiveResponse(this->http_request_handle, NULL)

*/
dllexport BOOL WINAPI WinHttpReceiveResponse(HINTERNET request_data_handle, LPVOID) {

	const RequestData* request_data = (const RequestData*)request_data_handle;

	uint8_t receive_buffer[receive_buffer_size];
	int recv_ret = recv(request_data->socket->sock, (char*)receive_buffer, receive_buffer_size, 0);
	if (expect(recv_ret <= 0, false)) {
		SetLastError(ERROR_WINHTTP_INTERNAL_ERROR);
		return false;
	}

	return true;
}


/*

th19 info: WinHttpWebSocketCompleteUpgrade(this->http_request_handle, NULL)

*/
dllexport HINTERNET WINAPI WinHttpWebSocketCompleteUpgrade(HINTERNET request_data_handle, DWORD_PTR) {
#if USE_FACTORIES
	return WebSocketData::allocate(request_data_handle);
#else
	const RequestData* request_data = (const RequestData*)request_data_handle;

	WebSocketData* web_socket_data = (WebSocketData*)malloc(sizeof(WebSocketData));
	//WebSocketData* web_socket_data = allocate<WebSocketData>();
	web_socket_data->data_type = GenericDataType;
	web_socket_data->socket = request_data->socket;

	return web_socket_data;
#endif
}

enum WebSocketOpcode {
	ContinuationFrame = 0,
	TextFrame = 1,
	BinaryFrame = 2,
	ConnectionClose = 8,
	// Is ping/pong handling part of WinHTTP?
	// Or is the application supposed to do that itself? Crap, IDK
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
		int recv_ret = recv(socket_data->sock, (char*)data, receive_buffer_size, 0);
		if (expect(recv_ret <= 0, false)) {
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

	WebSocketData* web_socket_data = (WebSocketData*)hWebSocket;
	SocketData* socket_data = web_socket_data->socket;

	int send_ret = send(socket_data->sock, (char*)frame, frame_size, 0);
	if (expect(send_ret <= 0, false)) {
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

		int send_ret = send(socket_data->sock, (char*)&close_frame, sizeof(close_frame), 0);
		if (expect(send_ret <= 0, false)) {
			return ERROR_INVALID_OPERATION;
		}
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