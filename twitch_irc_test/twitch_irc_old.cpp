#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"

#define _CRT_RAND_S 1

#define _HAS_CXX17 1
#define _HAS_CXX20 0

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <utility>
#include <tuple>
#include <string>
#include <string_view>

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

//#define ENABLE_SSL 1
//#define ENABLE_WEBSOCKET 1

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Dnsapi")
#if ENABLE_SSL
#pragma comment (lib, "secur32.lib")
#endif

#define MACRO_VOID(...)
#define MACRO_EVAL(...) __VA_ARGS__

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

#define expect __builtin_expect

#define CACHE_LINE_SIZE 64
#define cache_align alignas(CACHE_LINE_SIZE)

#define halt_and_catch_fire() __asm__("int3")

template <size_t bit_count>
using SBitIntType = std::conditional_t<bit_count <= 8, int8_t,
                    std::conditional_t<bit_count <= 16, int16_t,
                    std::conditional_t<bit_count <= 32, int32_t,
                    std::conditional_t<bit_count <= 64, int64_t,
                    void>>>>;
template <size_t bit_count>
using UBitIntType = std::conditional_t<bit_count <= 8, uint8_t,
                    std::conditional_t<bit_count <= 16, uint16_t,
                    std::conditional_t<bit_count <= 32, uint32_t,
                    std::conditional_t<bit_count <= 64, uint64_t,
                    void>>>>;

template <size_t byte_count>
using SByteIntType = SBitIntType<byte_count * CHAR_BIT>;
template <size_t byte_count>
using UByteIntType = UBitIntType<byte_count * CHAR_BIT>;

#if __INTELLISENSE__
#define requires(...) MACRO_EVAL(MACRO_VOID(__VA_ARGS__))
#define SBitInt(bit_count) SBitIntType<bit_count>
#define UBitInt(bit_count) UBitIntType<bit_count>
#define _BitInt(bit_count) SBitInt(bit_count)
#else
#define requires(...) requires (__VA_ARGS__)
#define SBitInt(bit_count) signed _BitInt(bit_count)
#define UBitInt(bit_count) unsigned _BitInt(bit_count)
#endif

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

static inline constexpr size_t uint16_text_length(uint16_t value) {
    switch (value) {
        case 0 ... 9:
            return 1;
        case 10 ... 99:
            return 2;
        case 100 ... 999:
            return 3;
        case 1000 ... 9999:
            return 4;
        default:
            return 5;
    }
}

template <typename T>
static inline constexpr size_t uint16_to_strbuf(uint16_t value, T* text_buffer) {
    size_t digit_offset = uint16_text_length(value) - 1;
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

#define USE_SINGLE_RECEIVE_BUFFER 1

bool winsock_initialized = false;

static inline constexpr size_t receive_buffer_size = UINT16_MAX + 1;

#if ENABLE_SSL
#define TLS_MAX_PACKET_SIZE (16384+512) // payload + extra over head for header/mac/padding (probably an overestimate)
#endif


//static constexpr wchar_t user_agent_str[] = L"TH18CHAOS";

__declspec(dllexport) bool enable() {
    if (expect(!winsock_initialized, false)) {
        WSADATA idgaf_about_winsock_version;
        winsock_initialized = !WSAStartup(WINSOCK_VERSION, &idgaf_about_winsock_version);
    }
    return winsock_initialized;
}


#if ENABLE_WEBSOCKET
static constexpr wchar_t server_name[] = L"irc-ws.chat.twitch.tv";
#if ENABLE_SSL
static constexpr INTERNET_PORT server_port = 443;
static constexpr wchar_t port_str[] = L"443";

static inline constexpr const wchar_t request_header_part_1[] = { L'G',L'E',L'T',L' ',L'/',L' ',L'H',L'T',L'T',L'P',L'/',L'1',L'.',L'1',L'\r',L'\n',
                                                                  L'C',L'o',L'n',L'n',L'e',L'c',L't',L'i',L'o',L'n',L':',L'U',L'p',L'g',L'r',L'a',L'd',L'e',L'\r',L'\n',
                                                                  L'U',L'p',L'g',L'r',L'a',L'd',L'e',L':',L'w',L'e',L'b',L's',L'o',L'c',L'k',L'e',L't',L'\r',L'\n',
                                                                  L'U',L's',L'e',L'r',L'-',L'A',L'g',L'e',L'n',L't',L':',
                                                                  L'T',L'H',L'1',L'8',L'C',L'H',L'A',L'O',L'S',L'\r',L'\n', // User Agent
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'P',L'r',L'o',L't',L'o',L'c',L'o',L'l',L':',L'i',L'r',L'c',L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'K',L'e',L'y',L':'
};
static inline constexpr const wchar_t request_header_part_2[] = { L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'V',L'e',L'r',L's',L'i',L'o',L'n',L':',L'1',L'3',L'\r',L'\n',
                                                                  L'H',L'o',L's',L't',L':',
                                                                  L'i',L'r',L'c',L'-',L'w',L's',L'.',L'c',L'h',L'a',L't',L'.',L't',L'w',L'i',L't',L'c',L'h',L'.',L't',L'v',L':',L'4',L'4',L'3',L'\r',L'\n', // server:port
                                                                  L'\r',L'\n',L'\r',L'\n'
};
#else
static constexpr INTERNET_PORT server_port = 80;
static constexpr wchar_t port_str[] = L"80";

static inline constexpr const wchar_t request_header_part_1[] = { L'G',L'E',L'T',L' ',L'/',L' ',L'H',L'T',L'T',L'P',L'/',L'1',L'.',L'1',L'\r',L'\n',
                                                                  L'C',L'o',L'n',L'n',L'e',L'c',L't',L'i',L'o',L'n',L':',L'U',L'p',L'g',L'r',L'a',L'd',L'e',L'\r',L'\n',
                                                                  L'U',L'p',L'g',L'r',L'a',L'd',L'e',L':',L'w',L'e',L'b',L's',L'o',L'c',L'k',L'e',L't',L'\r',L'\n',
                                                                  L'U',L's',L'e',L'r',L'-',L'A',L'g',L'e',L'n',L't',L':',
                                                                  L'T',L'H',L'1',L'8',L'C',L'H',L'A',L'O',L'S',L'\r',L'\n', // User Agent
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'P',L'r',L'o',L't',L'o',L'c',L'o',L'l',L':',L'i',L'r',L'c',L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'K',L'e',L'y',L':'
};
static inline constexpr const wchar_t request_header_part_2[] = { L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'V',L'e',L'r',L's',L'i',L'o',L'n',L':',L'1',L'3',L'\r',L'\n',
                                                                  L'H',L'o',L's',L't',L':',
                                                                  L'i',L'r',L'c',L'-',L'w',L's',L'.',L'c',L'h',L'a',L't',L'.',L't',L'w',L'i',L't',L'c',L'h',L'.',L't',L'v',L':',L'8',L'0',L'\r',L'\n', // server:port
                                                                  L'\r',L'\n',L'\r',L'\n'
};
#endif
#else
static constexpr wchar_t server_name[] = L"irc.chat.twitch.tv";
#if ENABLE_SSL
static constexpr INTERNET_PORT server_port = 6697;
static constexpr wchar_t port_str[] = L"6697";

static inline constexpr const wchar_t request_header_part_1[] = { L'G',L'E',L'T',L' ',L'/',L' ',L'H',L'T',L'T',L'P',L'/',L'1',L'.',L'1',L'\r',L'\n',
                                                                  L'C',L'o',L'n',L'n',L'e',L'c',L't',L'i',L'o',L'n',L':',L'U',L'p',L'g',L'r',L'a',L'd',L'e',L'\r',L'\n',
                                                                  L'U',L'p',L'g',L'r',L'a',L'd',L'e',L':',L'w',L'e',L'b',L's',L'o',L'c',L'k',L'e',L't',L'\r',L'\n',
                                                                  L'U',L's',L'e',L'r',L'-',L'A',L'g',L'e',L'n',L't',L':',
                                                                  L'T',L'H',L'1',L'8',L'C',L'H',L'A',L'O',L'S',L'\r',L'\n', // User Agent
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'P',L'r',L'o',L't',L'o',L'c',L'o',L'l',L':',L'i',L'r',L'c',L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'K',L'e',L'y',L':'
};
static inline constexpr const wchar_t request_header_part_2[] = { L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'V',L'e',L'r',L's',L'i',L'o',L'n',L':',L'1',L'3',L'\r',L'\n',
                                                                  L'H',L'o',L's',L't',L':',
                                                                  L'i',L'r',L'c',L'.',L'c',L'h',L'a',L't',L'.',L't',L'w',L'i',L't',L'c',L'h',L'.',L't',L'v',L':',L'6',L'6',L'9',L'7',L'\r',L'\n', // server:port
                                                                  L'\r',L'\n',L'\r',L'\n'
};
#else
static constexpr INTERNET_PORT server_port = 6667;
static constexpr wchar_t port_str[] = L"6667";

static inline constexpr const wchar_t request_header_part_1[] = { L'G',L'E',L'T',L' ',L'/',L' ',L'H',L'T',L'T',L'P',L'/',L'1',L'.',L'1',L'\r',L'\n',
                                                                  L'C',L'o',L'n',L'n',L'e',L'c',L't',L'i',L'o',L'n',L':',L'U',L'p',L'g',L'r',L'a',L'd',L'e',L'\r',L'\n',
                                                                  L'U',L's',L'e',L'r',L'-',L'A',L'g',L'e',L'n',L't',L':',
                                                                  L'T',L'H',L'1',L'8',L'C',L'H',L'A',L'O',L'S',L'\r',L'\n', // User Agent
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'P',L'r',L'o',L't',L'o',L'c',L'o',L'l',L':',L'i',L'r',L'c',L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'K',L'e',L'y',L':'
};
static inline constexpr const wchar_t request_header_part_2[] = { L'\r',L'\n',
                                                                  L'S',L'e',L'c',L'-',L'W',L'e',L'b',L'S',L'o',L'c',L'k',L'e',L't',L'-',L'V',L'e',L'r',L's',L'i',L'o',L'n',L':',L'1',L'3',L'\r',L'\n',
                                                                  L'H',L'o',L's',L't',L':',
                                                                  L'i',L'r',L'c',L'.',L'c',L'h',L'a',L't',L'.',L't',L'w',L'i',L't',L'c',L'h',L'.',L't',L'v',L':',L'6',L'6',L'6',L'7',L'\r',L'\n', // server:port
                                                                  L'\r',L'\n',L'\r',L'\n'
};
#endif
#endif

static inline constexpr wchar_t base64_lookup_str[] = {
    L'A',L'B',L'C',L'D',L'E',L'F',L'G',L'H',L'I',L'J',L'K',L'L',L'M',L'N',L'O',L'P',L'Q',L'R',L'S',L'T',L'U',L'V',L'W',L'X',L'Y',L'Z',
    L'a',L'b',L'c',L'd',L'e',L'f',L'g',L'h',L'i',L'j',L'k',L'l',L'm',L'n',L'o',L'p',L'q',L'r',L's',L't',L'u',L'v',L'w',L'x',L'y',L'z',
    L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'+',L'/'
};

struct alignas(16) Socket {

protected:
    static inline constexpr size_t nonce_key_encode_bits = 144;
    static inline constexpr size_t port_str_buf_len = 8;
public:

    wchar_t nonce_key_str[24]; // 0x0
    union { // 0x30
        uint8_t nonce_key[16];
        uint32_t nonce_key_dwords[5];
        UBitInt(nonce_key_encode_bits) nonce_key_wide;
    };
    SOCKET sock = INVALID_SOCKET; // 0x40
    bool upgraded_to_websocket; // 0x44
    bool generated_nonce_key; // 0x45
#if ENABLE_SSL
    bool tls_in_use; // 0x46
    bool tls_initialized; // 0x47
    SecPkgContext_StreamSizes tls_sizes; // 0x48
    CredHandle tls_handle;
    CtxtHandle tls_context;
#endif

    static inline constexpr ADDRINFOW addr_hint = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

#if ENABLE_SSL
    static inline constexpr SCHANNEL_CRED tls_cred = {
        .dwVersion = SCHANNEL_CRED_VERSION,
        .grbitEnabledProtocols = SP_PROT_TLS1_2,  // allow only TLS v1.2
        .dwFlags = SCH_USE_STRONG_CRYPTO |          // use only strong crypto alogorithms
                   SCH_CRED_AUTO_CRED_VALIDATION |  // automatically validate server certificate
                   SCH_CRED_NO_DEFAULT_CREDS     // no client certificate authentication
    };
#endif

protected:
    void initialize(SOCKET sock) {
        this->sock = sock;
        this->upgraded_to_websocket = false;
        this->generated_nonce_key = false;
#if ENABLE_SSL
        this->tls_in_use = false;
        this->tls_initialized = false;
#endif
    }
public:

    ~Socket() {
        if (this->sock != INVALID_SOCKET) {
            closesocket(this->sock);
        }
    }

    bool connect(/*const wchar_t* server_name, INTERNET_PORT server_port*/) {
        // GetAddrInfoW just crashes for some reason when trying to
        // resolve a URL on Windows 7, so the DNS resolution needs
        // to be done manually.
        // THIS IS DUMB

        PDNS_RECORDW records;
        wchar_t addr_buff[48];
        if (expect(DnsQuery_W(server_name, DNS_TYPE_AAAA, DNS_QUERY_DUAL_ADDR, NULL, (PDNS_RECORD*)&records, NULL) == 0, true)) {
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
        return false;

found_addr:
        DnsFree(records, DnsFreeRecordList);

        //wchar_t port_str[port_str_buf_len] = {}; // Buffer slightly oversized for efficient XMM copy
        //size_t port_str_len = uint16_to_strbuf(server_port, port_str);
    
        PADDRINFOW addrs;
        // GetAddrInfoW sucks
        if (expect(!GetAddrInfoW(addr_buff, port_str, &addr_hint, &addrs), true)) {
            do {
                SOCKET sock = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
                if (expect(sock != INVALID_SOCKET, true)) {
                    if (expect(::connect(sock, addrs->ai_addr, (int)addrs->ai_addrlen) != SOCKET_ERROR, true)) {
                        FreeAddrInfoW(addrs);
                        this->initialize(sock);
                        return true;
                    }
                    closesocket(sock);
                }
            } while ((addrs = addrs->ai_next));
            FreeAddrInfoW(addrs);
        }
        return false;
    }

#if ENABLE_SSL
    // Adapted from this: https://github.com/odzhan/shells/blob/master/s6/tls.c
    bool initialize_tls() {
        if (expect(AcquireCredentialsHandleW(NULL, (LPWSTR)UNISP_NAME_W, SECPKG_CRED_OUTBOUND, NULL, (void*)&tls_cred, NULL, NULL, &this->tls_handle, NULL) == SEC_E_OK, true)) {

            cache_align char packet_buffer[TLS_MAX_PACKET_SIZE];

            SecBuffer outbuffers[1] = { 0 };
            outbuffers[0].BufferType = SECBUFFER_TOKEN;
            outbuffers[0].pvBuffer = NULL;
            outbuffers[0].cbBuffer = 0;

            SecBufferDesc outdesc = { SECBUFFER_VERSION, ARRAYSIZE(outbuffers), outbuffers };

            DWORD flags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM | ISC_REQ_MANUAL_CRED_VALIDATION;

            SECURITY_STATUS sec = InitializeSecurityContextW(&this->tls_handle,
                                                             NULL,
                                                             (SEC_WCHAR*)server_name,
                                                             flags,
                                                             0,
                                                             0,
                                                             NULL,
                                                             0,
                                                             &this->tls_context,
                                                             &outdesc,
                                                             &flags,
                                                             NULL);
            if (sec == SEC_I_CONTINUE_NEEDED) {
                {
                    char* buffer = (char*)outbuffers[0].pvBuffer;
                    size_t size = outbuffers[0].cbBuffer;
                    while (size != 0) {
                        auto send_ret = ::send(this->sock, buffer, size, 0);
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
                        int recv_ret = ::recv(this->sock, &packet_buffer[received], sizeof(packet_buffer) - received, 0);
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

                    sec = InitializeSecurityContextW(&this->tls_handle,
                                                     &this->tls_context,
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
                                auto send_ret = ::send(this->sock, buffer, size, 0);
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
                        QueryContextAttributesW(&this->tls_context, SECPKG_ATTR_STREAM_SIZES, &this->tls_sizes);
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
                DeleteSecurityContext(&this->tls_context);
            }
            FreeCredentialsHandle(&this->tls_handle);
        }
        closesocket(this->sock);
        this->sock = INVALID_SOCKET;
        return false;
    }
#endif

    // TODO: use this instead of direct recv calls to simplify TLS support?
    size_t receive(uint8_t* recv_buffer, size_t recv_length) {
#if ENABLE_SSL
        if (expect(!this->tls_in_use, false)) {
#endif
            int recv_ret = ::recv(this->sock, (char*)recv_buffer, recv_length, 0);
            if (expect(recv_ret <= 0, false)) {
                return 0;
            }
            return recv_ret;
#if ENABLE_SSL
        }
        size_t received_size = 0;
        size_t total_received = 0;

        uint8_t* initial_recv_buffer = recv_buffer;

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
            int recv_ret = ::recv(this->sock, &packet_buffer[received_size], receive_buffer_size - received_size, 0);
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

                SECURITY_STATUS sec = DecryptMessage(&this->tls_context, &desc, 0, NULL);
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
                        return total_received;
                    default:
                        return 0;
                    case SEC_E_INCOMPLETE_MESSAGE:
                        break;
                }
                break;
            }
        }
        return total_received;
#endif
    }

    template<typename T, size_t len>
    size_t receive(const T(&buffer)[len]) {
        return this->receive((uint8_t*)&buffer, sizeof(buffer));
    }

    size_t send(void* buffer, size_t send_length) {

        uint8_t* send_buffer = (uint8_t*)buffer;

        size_t total_sent_length = 0;
#if ENABLE_SSL
        cache_align char packet_buffer[receive_buffer_size];
        while (send_length) {
#endif
            char* current_send_buffer = (char*)send_buffer;
            size_t current_buffer_read_length = send_length;
            size_t current_send_length = current_buffer_read_length;
#if ENABLE_SSL
            SecBuffer buffers[3];
            if (expect(this->tls_in_use, true)) {
                current_send_buffer = packet_buffer;
                if (current_buffer_read_length > this->tls_sizes.cbMaximumMessage) {
                    current_buffer_read_length = this->tls_sizes.cbMaximumMessage;
                }
                buffers[0].BufferType = SECBUFFER_STREAM_HEADER;
                buffers[0].pvBuffer = current_send_buffer;
                buffers[0].cbBuffer = this->tls_sizes.cbHeader;
                buffers[1].BufferType = SECBUFFER_DATA;
                buffers[1].pvBuffer = &current_send_buffer[this->tls_sizes.cbHeader];
                buffers[1].cbBuffer = current_buffer_read_length;
                buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;
                buffers[2].pvBuffer = &current_send_buffer[this->tls_sizes.cbHeader + current_buffer_read_length];
                buffers[2].cbBuffer = this->tls_sizes.cbTrailer;
                __builtin_memcpy(buffers[1].pvBuffer, send_buffer, current_buffer_read_length);
                SecBufferDesc desc = { SECBUFFER_VERSION, ARRAYSIZE(buffers), buffers };
                if (expect(EncryptMessage(&this->tls_context, 0, &desc, 0) != SEC_E_OK, false)) {
                    // this should not happen, but just in case check it
                    return false;
                }
                current_send_length = buffers[0].cbBuffer + buffers[1].cbBuffer + buffers[2].cbBuffer;
            }
#endif
            size_t sent = 0;
            while (sent != current_send_length) {
                int send_ret = ::send(this->sock, &current_send_buffer[sent], current_send_length - sent, 0);
                if (expect(send_ret <= 0, false)) {
                    return 0;
                }
                sent += send_ret;
                total_sent_length += send_ret;
            }
#if ENABLE_SSL
            send_buffer += current_buffer_read_length;
            send_length -= current_buffer_read_length;
        }
#endif
        return total_sent_length;
    }

    template <typename T, size_t len>
    inline bool send(const T(&str)[len]) {
        return this->send((void*)&str, sizeof(str) - sizeof(T));
    }

    template <typename L>
    bool send_and_parse_reply(void* buffer, size_t buffer_length, const L& parse) {
        if (this->send(buffer, buffer_length)) {
            uint8_t receive_buffer[receive_buffer_size] = {};
            if (size_t data_length = this->receive(receive_buffer, sizeof(receive_buffer))) {
                return parse(receive_buffer, data_length);
            }
        }
        return false;
    }

    template <typename T, size_t len, typename L>
    inline bool send_and_parse_reply(const T(&str)[len], const L& parse) {
        return this->send_and_parse_reply((void*)&str, sizeof(str) - sizeof(T), parse);
    }

protected:
    bool upgrade_to_websocket() {
#if ENABLE_SSL
        if (!this->tls_in_use) {
            this->tls_in_use = true;
            if (expect(!this->tls_initialized, false)) {
                if (expect(!this->initialize_tls(), false)) {
                    return false;
                }
                this->tls_initialized = true;
            }
        }
#endif

        if (expect(!this->generated_nonce_key, false)) {
            this->generated_nonce_key = true;

            // TODO: better RNG
            (void)rand_s(&this->nonce_key_dwords[0]);
            (void)rand_s(&this->nonce_key_dwords[1]);
            (void)rand_s(&this->nonce_key_dwords[2]);
            (void)rand_s(&this->nonce_key_dwords[3]);

            size_t str_write = 0;
            // This loop gets optimized to remove the last two
            // iterations anyway, so I don't feel like tweaking it
            for (size_t i = 0; i < nonce_key_encode_bits; i += 6) {
                this->nonce_key_str[str_write++] = base64_lookup_str[this->nonce_key_wide >> i & 0b111111];
            }
            this->nonce_key_str[22] = L'=';
            this->nonce_key_str[23] = L'=';
        }

        size_t request_len = countof(request_header_part_1) + countof(this->nonce_key_str) +
                             countof(request_header_part_2);

        wchar_t* request_str = ::allocate<wchar_t>(request_len);

        build_str(request_str,
                  request_header_part_1,
                  this->nonce_key_str,
                  request_header_part_2
        );

        // HORRIBLE HACK
        // I don't want to bother converting ascii-only-UTF16
        // to UTF8 the proper way until I know everything else works.
        uint8_t* evil_garbage = (uint8_t*)request_str;
        for (size_t i = 0; i < request_len; ++i) {
            wchar_t jank = request_str[i];
            evil_garbage[i] = (uint8_t)jank;
            if (expect(jank != (uint8_t)jank, false)) {
                halt_and_catch_fire(); // crap it's not ascii
                __builtin_unreachable();
            }
        }

        if (expect(!this->send(evil_garbage, request_len), false)) {
            free(request_str);
            return false;
        }
        free(request_str);

        uint8_t receive_buffer[receive_buffer_size];

        return this->receive(receive_buffer, receive_buffer_size);
    }
};

#pragma region websocket
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
        uint64_t payload_length __attribute__((packed));
    };
    UBitInt(80) header_with_length;
};

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

struct WebSocket : Socket {

    ~WebSocket() {
        if (this->sock != INVALID_SOCKET) {
            if (expect(((Socket*)this)->send((uint8_t*)&close_frame, sizeof(close_frame)), true)) {
#if ENABLE_SSL
                if (expect(this->tls_in_use, true)) {
                    DWORD type = SCHANNEL_SHUTDOWN;

                    SecBuffer inbuffers[1];
                    inbuffers[0].BufferType = SECBUFFER_TOKEN;
                    inbuffers[0].pvBuffer = &type;
                    inbuffers[0].cbBuffer = sizeof(DWORD);

                    SecBufferDesc indesc = { SECBUFFER_VERSION, ARRAYSIZE(inbuffers), inbuffers };
                    ApplyControlToken(&this->tls_context, &indesc);

                    SecBuffer outbuffers[1];
                    outbuffers[0].BufferType = SECBUFFER_TOKEN;

                    SecBufferDesc outdesc = { SECBUFFER_VERSION, ARRAYSIZE(outbuffers), outbuffers };

                    DWORD flags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;
                    SECURITY_STATUS sec = InitializeSecurityContextW(&this->tls_handle,
                                                                     &this->tls_context,
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
                            int send_ret = ::send(this->sock, buffer, size, 0);
                            if (expect(send_ret <= 0, false)) {
                                // ignore any failures socket will be closed anyway
                                break;
                            }
                            buffer += send_ret;
                            size -= send_ret;
                        }
                        FreeContextBuffer(outbuffers[0].pvBuffer);
                    }
                    shutdown(this->sock, SD_BOTH);
                    DeleteSecurityContext(&this->tls_context);
                    FreeCredentialsHandle(&this->tls_handle);
                }
#endif
            }
        }
    }

    bool connect(/*const wchar_t* server_name, INTERNET_PORT server_port*/) {
        return ((Socket*)this)->connect() && this->upgrade_to_websocket();
    }

    size_t receive(void* buffer, size_t buffer_length) {
#if !USE_SINGLE_RECEIVE_BUFFER
        uint8_t receive_buffer[receive_buffer_size];
#else
        uint8_t* receive_buffer = (uint8_t*)buffer;
        buffer_length = receive_buffer_size;
#endif

        size_t recv_ret = ((Socket*)this)->receive(receive_buffer, receive_buffer_size);
        if (expect(!recv_ret, false)) {
            return 0;
        }

        if (expect(recv_ret == receive_buffer_size, false)) {
            // IDK what the proper way to size these
            // buffers is, so just die if it's too big
            halt_and_catch_fire();
        }
        WebSocketFrameSmall* frame = (WebSocketFrameSmall*)receive_buffer;
        if (expect(!frame->final_fragment, false)) {
            // Do I have to implement fragmented frames?
            halt_and_catch_fire();
        }

        uint8_t* data = receive_buffer;

        uint64_t data_length;
        switch (uint8_t payload_length = frame->payload_length) {
            default:
                data_length = payload_length;
                data += sizeof(WebSocketFrameSmall);
                break;
            case MediumFrame:
                data_length = __builtin_bswap16(((WebSocketFrameMedium*)receive_buffer)->payload_length);
                data += sizeof(WebSocketFrameMedium);
                break;
            case BigFrame:
                data_length = __builtin_bswap64(((WebSocketFrameBig*)receive_buffer)->payload_length);
                data += sizeof(WebSocketFrameBig);
                break;
        }
        if (expect(data_length > buffer_length, false)) {
            // Docs don't say what happens when the buffer
            // is too small, so I made something up
            return 0;
        }

        uint32_t mask;
        bool is_masked = frame->masked_data;
        if (is_masked) {
            mask = *(__unaligned uint32_t*)data;
            data += sizeof(uint32_t);
            for (size_t i = 0; i < data_length; i += 4) {
                *(uint32_t*)&data[i] ^= mask;
            }
        }

#if !USE_SINGLE_RECEIVE_BUFFER
        __builtin_memcpy(buffer, data, data_length);
#endif

        return data_length;
    }

    template<typename T, size_t len>
    size_t receive(const T(&buffer)[len]) {
        return this->receive(&buffer, sizeof(buffer));
    }

    bool send(void* buffer, size_t buffer_length) {
        size_t frame_size = sizeof(uint32_t) + buffer_length; // mask_size
        uint8_t header_length;
        switch (buffer_length) {
            case 0 ... 125:
                header_length = buffer_length;
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
        frame_header->opcode = TextFrame;
        frame_header->masked_data = true;
        frame_header->payload_length = header_length;
        uint8_t* frame_data = frame + sizeof(WebSocketFrameSmall);
        switch (header_length) {
            case 126:
                *(uint16_t*)frame_data = __builtin_bswap16((uint16_t)buffer_length);
                frame_data += sizeof(uint16_t);
                break;
            case 127:
                *(uint64_t*)frame_data = __builtin_bswap64((uint64_t)buffer_length);
                frame_data += sizeof(uint64_t);
                break;
        }
        // Yes, I hardcoded the masking key as 0
        // No, I don't give a crap about it
        *(uint32_t*)frame_data = 0;
        frame_data += sizeof(uint32_t);
        __builtin_memcpy(frame_data, buffer, buffer_length);

        return ((Socket*)this)->send((uint8_t*)frame, frame_size);
    }

    template <typename T, size_t len>
    inline bool send(const T(&str)[len]) {
        return this->send((void*)&str, sizeof(str) - sizeof(T));
    }

    template <typename L>
    bool send_and_parse_reply(void* buffer, size_t buffer_length, const L& parse) {
        if (this->send(buffer, buffer_length)) {
            uint8_t receive_buffer[receive_buffer_size] = {};
            if (size_t data_length = this->receive(receive_buffer, sizeof(receive_buffer))) {
                return parse(receive_buffer, data_length);
            }
        }
        return false;
    }

    template <typename T, size_t len, typename L>
    inline bool send_and_parse_reply(const T(&str)[len], const L& parse) {
        return this->send_and_parse_reply((void*)&str, sizeof(str) - sizeof(T), parse);
    }
};
#pragma endregion


bool stop_twitch_parsing = false;

template<typename L>
bool parse_twitch_chat(const char* channel_id, const L& parse) {
    if (enable()) {
        Socket socket;
        if (socket.connect()) {
            // Ripped these from https://github.com/xenova/twitch-chat-irc/tree/master
            // This should probably be updated with something else?
            socket.send("PASS SCHMOOPIIE\r\n");
            socket.send("NICK justinfan67420\r\n");

            uint8_t receive_buffer[receive_buffer_size + 1];

            // Ignore the default login message
            socket.receive(receive_buffer, receive_buffer_size);

            __builtin_memcpy(receive_buffer, "JOIN #", 6);
            size_t channel_length = str_len(channel_id);

            uint8_t* channel_name_write = &receive_buffer[6];
            for (size_t i = 0; i < channel_length; ++i) {
                *channel_name_write++ = channel_id[i];
            }
            *channel_name_write++ = '\r';
            *channel_name_write++ = '\n';

            socket.send(receive_buffer, channel_name_write - &receive_buffer[0]);
            
            // Ignore the channel join messages
            socket.receive(receive_buffer, receive_buffer_size);
            socket.receive(receive_buffer, receive_buffer_size);

            //u_long mode = 1;
            //ioctlsocket(socket.sock, FIONBIO, &mode);

            while (!stop_twitch_parsing) {
                if (size_t data_length = socket.receive(receive_buffer, receive_buffer_size)) {
                    receive_buffer[data_length] = '\0';
                    char* message = (char*)receive_buffer;
                    if (
                        data_length >= sizeof("PING :tmi.twitch.tv") - 1 &&
                        strstr(message, "PING :tmi.twitch.tv")
                    ) {
                        socket.send("PONG :tmi.twitch.tv\r\n");
                        continue;
                    }
                    if (strstr(message, "PRIVMSG")) {
                        size_t i = 0;
                        while (message[++i] != '!'); // find the end of the username
                        message[i] = '\0';
                        std::string_view username = std::string_view(&message[1], i - 1); // offset by 1 to remove the initial :
                        while (message[++i] != ':'); // find the start of the message text
                        size_t j = i++;
                        for (char c; (c = message[++j]) != '\r' && c != '\n';); // find the end of the message text
                        message[j] = '\0';
                        std::string_view message_text = std::string_view(&message[i], j - i);
                        
                        parse(username, message_text);
                    }
                    
                }
            }
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    return !parse_twitch_chat(
        "jollywangcore",
        [](std::string_view& username, std::string_view& message) {
            //printf("%.*s", message_length, message);
            printf("%s", message.data());
        }
    );
}