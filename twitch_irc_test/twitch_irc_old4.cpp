#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"

#if __INTELLISENSE__
#define _HAS_CXX17 1
#define _HAS_CXX20 0
#endif

// These types aren't even used in the code
// but clang is warning about them anyway
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <utility>
#include <tuple>
#include <string>
#include <array>
#include <string_view>
#include <unordered_set>
#include <type_traits>
#include <chrono>
using namespace std::literals;
using namespace std::literals::chrono_literals;

#include <thread>

#if __INTELLISENSE__
#undef _HAS_CXX20
#define _HAS_CXX20 1
#endif

#include <bit>

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

//#define ENABLE_SSL 1

#if ENABLE_SSL
#define SECURITY_WIN32
#include <security.h>
#include <schannel.h>
#include <sspi.h>
#endif

#include <WinDNS.h>

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

#if __INTELLISENSE__
#define requires(...) MACRO_EVAL(MACRO_VOID(__VA_ARGS__))
#else
#define requires(...) requires (__VA_ARGS__)
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

static bool winsock_initialized = false;

static inline constexpr size_t receive_buffer_size = UINT16_MAX + 1;

static inline constexpr size_t recent_message_max_count = 8;
static inline constexpr size_t recent_message_max_length = 128;
static_assert(std::has_single_bit(recent_message_max_count));
static inline constexpr uint32_t recent_message_wrap_mask = recent_message_max_count - 1;

// Static buffers

// "Members of an anonymous union are injected in the enclosing scope"
static union {
    // Annonymous struct to allow grouping everything together
    // as a single memory block
    struct {
        // Buffers that never need to be accesses simultaneously
        union {
            cache_align uint8_t receive_buffer[receive_buffer_size];
            cache_align WSADATA idgaf_about_winsock_version;
            cache_align wchar_t addr_buff[48];
        };
        // This is placed after the receive buffer in memory
        // so that copy operations can be done without caring
        // how long a message actually is
        cache_align char recent_messages[recent_message_max_count][recent_message_max_length];
#if ENABLE_SSL
        cache_align char packet_buffer[receive_buffer_size];
#endif
    };
};

static inline bool enable() {
    if (expect(!winsock_initialized, false)) {
        winsock_initialized = !WSAStartup(WINSOCK_VERSION, &idgaf_about_winsock_version);
    }
    return winsock_initialized;
}

static constexpr wchar_t server_name[] = L"irc.chat.twitch.tv";
#if ENABLE_SSL
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

static inline constexpr wchar_t base64_lookup_str[] = {
    L'A',L'B',L'C',L'D',L'E',L'F',L'G',L'H',L'I',L'J',L'K',L'L',L'M',L'N',L'O',L'P',L'Q',L'R',L'S',L'T',L'U',L'V',L'W',L'X',L'Y',L'Z',
    L'a',L'b',L'c',L'd',L'e',L'f',L'g',L'h',L'i',L'j',L'k',L'l',L'm',L'n',L'o',L'p',L'q',L'r',L's',L't',L'u',L'v',L'w',L'x',L'y',L'z',
    L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'+',L'/'
};

struct Socket {

    SOCKET sock = INVALID_SOCKET; // 0x0
    // room for 3 bytes
protected:
#if ENABLE_SSL
    bool tls_in_use;
    SecPkgContext_StreamSizes tls_sizes;

    // Who knows WTF windows does with these, so
    // don't make any weird promises to the compiler
    mutable CredHandle tls_handle;
    mutable CtxtHandle tls_context;
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

    void initialize(SOCKET sock) {
        this->sock = sock;
#if ENABLE_SSL
        this->tls_in_use = this->initialize_tls();
#endif
    }
public:

    void close() {
        if (this->sock != INVALID_SOCKET) {
            closesocket(this->sock);
            this->sock = INVALID_SOCKET;
        }
    }

    ~Socket() {
        this->close();
    }

    bool connect() {
        // GetAddrInfoW just crashes for some reason when trying to
        // resolve a URL on Windows 7, so the DNS resolution needs
        // to be done manually.
        // THIS IS DUMB

        PDNS_RECORDW records;
        //wchar_t addr_buff[48];
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

protected:
#if ENABLE_SSL
    // Adapted from this: https://github.com/odzhan/shells/blob/master/s6/tls.c
    bool initialize_tls() {
        if (expect(AcquireCredentialsHandleW(NULL, (LPWSTR)UNISP_NAME_W, SECPKG_CRED_OUTBOUND, NULL, (void*)&tls_cred, NULL, NULL, &this->tls_handle, NULL) == SEC_E_OK, true)) {

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
public:

    size_t receive(void* buffer, size_t recv_length) const {
        uint8_t* recv_buffer = (uint8_t*)buffer;
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
                            current_write_length = (std::min)(recv_length, decrypt_data_length);
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
    size_t receive_text(T(&buffer)[len]) const {
        size_t ret = this->receive(buffer, sizeof(buffer) - sizeof(T));
        buffer[ret] = '\0';
        return ret;
    }

    template<typename T>
    size_t receive(T& buffer) const {
        return this->receive(&buffer, sizeof(buffer));
    }

    size_t send(const void* buffer, size_t send_length) const {

        uint8_t* send_buffer = (uint8_t*)buffer;

        size_t total_sent_length = 0;
#if ENABLE_SSL
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
    inline bool send_text(const T(&str)[len]) const {
        return this->send(&str[0], sizeof(str) - sizeof(T));
    }

    template <typename T>
    inline bool send(const T& buffer) const {
        return this->send(&buffer, sizeof(buffer));
    }
};

#define LOCK_ALL_MESSAGES 1

static uint32_t filled_message_slots = 0;
static uint32_t recent_message_write_index = 0;
static _Atomic(uint8_t) recent_message_lock = false;

static inline constexpr bool btr(uint32_t& value, uint32_t index) {
    // Clang doesn't seem to understand the flag
    // output of BTR, but this is close enough
    uint32_t temp = value;
    value &= ~(1 << index);
    return value != temp;
}

bool __fastcall lock_recent_messages() {
    if (expect(filled_message_slots != 0, true)) {
        recent_message_lock = true;
        return true;
    }
    return false;
}

const char* __fastcall get_recent_message() {
    uint32_t filled_slots = filled_message_slots;
    if (expect(filled_slots != 0, true)) {
        uint32_t index = recent_message_write_index;
        for (;;) {
            index = index - 1 & recent_message_wrap_mask;
            if (expect(btr(filled_slots, index), true)) {
                filled_message_slots = filled_slots;
                recent_message_write_index = index;
                return recent_messages[index];
            }
        }
        __builtin_unreachable();
    }
    return NULL;
}

void __fastcall unlock_recent_messages() {
    recent_message_lock = false;
}

void __fastcall write_recent_message(const std::string_view& message) {
    if (expect(
        message.length() < recent_message_max_length &&
        !recent_message_lock
    , true)) {
        uint32_t index = recent_message_write_index;
        // memmove makes better code here than memcpy
        __builtin_memmove(recent_messages[index], message.data(), recent_message_max_length);
        filled_message_slots |= 1 << index;
        recent_message_write_index = index + 1 & recent_message_wrap_mask;
    }
}

#define PARSE_TAGS 0

template<typename L1, typename L2>
void parse_twitch_chat(std::stop_token& stoken, const char* channel_id, const L1& pre_message, const L2& parse) {
    if (enable()) {
        Socket socket;
        if (socket.connect()) {
#if PARSE_TAGS
            socket.send_text("CAP REQ :"
#if PARSE_TAGS
                            "twitch.tv/tags "
#endif
                            "\r\n"
            );
#endif
            // Ripped these from https://github.com/xenova/twitch-chat-irc/tree/master
            // This should probably be updated with something else?
            socket.send_text("PASS SCHMOOPIIE\r\n");
            socket.send_text("NICK justinfan67420\r\n");

            //u_long mode = 1;
            //auto yeetus2 = ioctlsocket(socket.sock, FIONBIO, &mode);
            DWORD timeout = 250;
            auto yeetus = setsockopt(socket.sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

            size_t debug;

            // Ignore the default login message
            debug = socket.receive(receive_buffer, receive_buffer_size);

            //printf("%.*s", debug, receive_buffer);

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
            debug = socket.receive(receive_buffer, receive_buffer_size);
            //printf("%.*s", debug, receive_buffer);
            debug = socket.receive(receive_buffer, receive_buffer_size);
            //printf("%.*s", debug, receive_buffer);

            //printf("Start\n");

            do {

                pre_message();

                if (size_t data_length = socket.receive(receive_buffer, receive_buffer_size)) {
                    //receive_buffer[data_length] = '\0';
                    //printf("%.*s\n", data_length, receive_buffer);
                    //continue;

                    size_t start_of_message = 0;
                    for (size_t i = 0; i < data_length; ++i) {
                        // All messages are terminated with \r\n,
                        // so it's always going to be safe to read
                        // 1 extra character past a null terminator
                        // when formatted this way.
                        if (receive_buffer[i] == '\r') {
                            receive_buffer[i] = '\0'; // Add a null terminator so that strstr doesn't explode

                            char* message = (char*)&receive_buffer[start_of_message];
                            size_t message_length = i - start_of_message;

                            if (
                                message_length >= sizeof("PING :tmi.twitch.tv") - 1 &&
                                strstr(message, "PING :tmi.twitch.tv")
                            ) {
                                // This is probably wrong but the other twitch
                                // clients do the same thing so whatever
                                socket.send_text("PONG :tmi.twitch.tv\r\n");
                            }
                            else if (strstr(message, "PRIVMSG")) {
                                // Twitch's example parser sucks
#if PARSE_TAGS
                                size_t j = message[0] == '@';
                                if (j) while (message[j++] != ' ');
#else
                                size_t j = 0;
#endif
                                size_t k = j++;
                                while (message[++k] != '!'); // find the end of the username
                                message[k] = '\0';
                                std::string_view username = std::string_view(&message[j], k - j); // offset by 1 to remove the initial :
                                while (message[++k] != ':'); // find the start of the message text
                                size_t l = k++;
                                while (message[++l] != '\0'); // find the end of the message text (always delimitted by \r\n)
                                std::string_view message_text = std::string_view(&message[k], l - k);

                                parse(username, message_text);
                            }

                            start_of_message = i += 2;
                        }
                    }
                }
            } while (!stoken.stop_requested());
        }
    }
}


struct HashedString {
    size_t hash;

    inline constexpr HashedString(const std::string_view& str) : hash(std::hash<std::string_view>{}(str)) {}

    inline constexpr bool operator==(const HashedString& val) const {
        return this->hash == val.hash;
    }
};

template<>
struct std::hash<HashedString> {
    size_t operator()(const HashedString& str) const {
        return str.hash;
    }
};

static _Atomic(bool) reset_votes;

static _Atomic(uint32_t) valid_votes = 0b00001111;

static std::unordered_set<HashedString> already_voted;

alignas(16) static size_t vote_totals[8];

static std::jthread vote_thread;

#define PRINT_MESSAGES 0

void __fastcall stop_twitch_voting() {
    vote_thread.~jthread();
}

bool __fastcall voting_is_high_numbers() {
    return (int8_t)valid_votes < 0;
}

std::array<size_t, 4> __fastcall get_votes() {
    size_t ret[4];
    if (!voting_is_high_numbers()) {
        __builtin_memcpy(ret, &vote_totals[0], sizeof(size_t[4]));
        __builtin_memset(&vote_totals[0], 0, sizeof(size_t[4]));
    } else {
        __builtin_memcpy(ret, &vote_totals[4], sizeof(size_t[4]));
        __builtin_memset(&vote_totals[4], 0, sizeof(size_t[4]));
    }
    return { ret[0], ret[1], ret[2], ret[3] };
}

std::array<size_t, 4> __fastcall get_final_votes() {
    
    uint32_t temp = std::rotl((uint8_t)valid_votes, 4);
    valid_votes = temp;
    reset_votes = true;

    size_t ret[4];
    if ((int8_t)temp < 0) {
        __builtin_memcpy(ret, &vote_totals[0], sizeof(size_t[4]));
        __builtin_memset(&vote_totals[0], 0, sizeof(size_t[4]));
    } else {
        __builtin_memcpy(ret, &vote_totals[4], sizeof(size_t[4]));
        __builtin_memset(&vote_totals[4], 0, sizeof(size_t[4]));
    }
    return { ret[0], ret[1], ret[2], ret[3] };
}

void __fastcall start_twitch_voting(const char* channel_name) {
    vote_thread = std::jthread(
        [=](std::stop_token stoken) __attribute__((stdcall)) {
            parse_twitch_chat(
                stoken,
                channel_name,
                // Pre-message function
                []() {
                    if (reset_votes) {
                        reset_votes = false;
                        already_voted.clear();
                    }
                },
                // Message parsing function
                [](const std::string_view& username, const std::string_view& message) {
#if PRINT_MESSAGES
                    printf("%s: %s\n", username.data(), message.data());
#endif

                    uint32_t vote = *(uint16_t*)message.data() - '1';
                    if (vote < 8) {
                        // Message is an attempt at voting and
                        // shouldn't count towards recent messages
                        if (
                            (valid_votes & 1 << vote) &&
                            already_voted.emplace(username).second
                        ) {
                            ++vote_totals[vote];
                        }
                    } else if (message.length() > 1) { // Ignore other single character messages
                        write_recent_message(message);
                    }
                }
            );
        }
    );
}