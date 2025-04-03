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
#include <tuple>
#include <array>
#include <string>
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

#define ENABLE_SSL 1

#define ENABLE_SSL_FOR_TWITCH 0

#define ENABLE_YOUTUBE 1

#define PRINT_ERROR_MESSAGES 1

#define PRINT_MESSAGES 1

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

template <typename T, typename S, typename ... StrsT> requires(std::is_same_v<S, std::basic_string<T>> || std::is_same_v<S, std::basic_string_view<T>>)
static inline T* build_str(T* buffer, const S& str, StrsT&&... next_strs) {
    if constexpr (sizeof...(StrsT)) {
        return build_str(str_copy(buffer, str.data(), str.length()), std::forward<StrsT>(next_strs)...);
    } else {
        return str_copy(buffer, str.data(), str.length());
    }
}
template <typename T, typename ... StrsT>
static inline T* build_str(T* buffer, const T* str, size_t len, StrsT&&... next_strs) {
    if constexpr (sizeof...(StrsT)) {
        return build_str(str_copy(buffer, str, len), std::forward<StrsT>(next_strs)...);
    } else {
        return str_copy(buffer, str, len);
    }
}
template <typename T, size_t len, typename ... StrsT> requires(!std::is_integral_v<std::tuple_element_t<0, std::tuple<StrsT...,void>>>)
static inline T* build_str(T* buffer, const T(&str)[len], StrsT&&... next_strs) {
    if constexpr (sizeof...(StrsT)) {
        return build_str(str_copy(buffer, str, len - 1), std::forward<StrsT>(next_strs)...);
    } else {
        return str_copy(buffer, str, len - 1);
    }
}

template <typename T, typename S, typename ... StrsT> requires(std::is_same_v<S, std::basic_string<T>> || std::is_same_v<S, std::basic_string_view<T>>)
static inline size_t calc_str_len(size_t length, const S& str, StrsT&&... next_strs) {
    if constexpr (sizeof...(StrsT)) {
        return calc_str_len<T>(length + str.length(), std::forward<StrsT>(next_strs)...);
    } else {
        return length + str.length();
    }
}
template <typename T, size_t len, typename ... StrsT> requires(!std::is_integral_v<std::tuple_element_t<0, std::tuple<StrsT..., void>>>)
static inline size_t calc_str_len(size_t length, const T(&str)[len], StrsT&&... next_strs) {
    if constexpr (sizeof...(StrsT)) {
        return calc_str_len<T>(length + len - 1, std::forward<StrsT>(next_strs)...);
    } else {
        return length + len - 1;
    }
}

template <typename T, typename ... StrsT>
static inline auto build_new_str_impl(StrsT&&... strs) {
    size_t length = calc_str_len<T>(0, std::forward<StrsT>(strs)...);
    T* buffer = (T*)malloc(length * sizeof(T) + sizeof(T));
    buffer[length] = (T)0;
    build_str(buffer, std::forward<StrsT>(strs)...);
    return std::make_pair(buffer, length);
}

template <typename T, typename ... StrsT>
static inline auto build_new_str(StrsT&&... strs) {
    return build_new_str_impl<T>(std::forward<std::conditional_t<!std::is_pointer_v<std::remove_reference_t<StrsT>>, StrsT, std::basic_string_view<T>>>(strs)...);
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
    // as a single memory block. (GCC doesn't like this)
    struct {
        // Buffers that never need to be accessed simultaneously
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

static inline constexpr const wchar_t twitch_server_name[] = L"irc.chat.twitch.tv";
#if ENABLE_SSL && ENABLE_SSL_FOR_TWITCH
static inline constexpr const wchar_t twitch_port_str[] = L"6697";
#else
static inline constexpr const wchar_t twitch_port_str[] = L"6667";
#endif
#if ENABLE_YOUTUBE
static inline constexpr const wchar_t googleapis_server_name_w[] = L"www.googleapis.com";
static inline constexpr const char googleapis_server_name[] = "www.googleapis.com";
static inline constexpr const wchar_t googleapis_port_str[] = L"443";
#endif

static inline constexpr const char get_request_header_part_1[] = "GET ";
static inline constexpr const char get_request_header_part_2[] = " HTTP/1.1\r\n";
static inline constexpr const char get_request_header_part_3[] = "\r\n";

template<typename S>
struct SocketBase {
    SOCKET sock = INVALID_SOCKET; // 0x0
    // room for 3 bytes
protected:

    static inline constexpr ADDRINFOW addr_hint = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    void initialize(SOCKET sock, const wchar_t* server_name) {
        this->sock = sock;
        ((S*)this)->initialize_secondary(sock, server_name);
    }

    inline void initialize_secondary(SOCKET sock, const wchar_t* server_name) {}

public:
    void close() {
        if (this->sock != INVALID_SOCKET) {
            closesocket(this->sock);
            this->sock = INVALID_SOCKET;
            ((S*)this)->close_secondary();
        }
    }

protected:
    inline void close_secondary() {}

public:
    
    bool connect(const wchar_t* server_name, const wchar_t* port_str) {
        // GetAddrInfoW just crashes for some reason when trying to
        // resolve a URL on Windows 7, so the DNS resolution needs
        // to be done manually.
        // THIS IS DUMB
        
        PDNS_RECORDW records;
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
                        this->initialize(sock, server_name);
                        return true;
                    }
                    /*
                    else {
                        int error = WSAGetLastError();
                        printf("%d\n", error);
                    }
                    */
                    closesocket(sock);
                }
            } while ((addrs = addrs->ai_next));
            FreeAddrInfoW(addrs);
        }
        return false;
    }

protected:
    inline size_t receive_impl(void* buffer, size_t recv_length) const {
        int recv_ret = ::recv(this->sock, (char*)buffer, recv_length, 0);
        if (expect(recv_ret <= 0, false)) {
            //return 0;
            recv_ret = 0;
        }
        return recv_ret;
    }

public:
    size_t receive(void* buffer, size_t recv_length) const {
        return ((const S*)this)->receive_impl(buffer, recv_length);
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

protected:
    inline size_t send_impl(const void* buffer, size_t send_length) const {

        size_t sent = 0;
        while (sent != send_length) {
            int send_ret = ::send(this->sock, &((char*)buffer)[sent], send_length - sent, 0);
            if (expect(send_ret <= 0, false)) {
                return 0;
            }
            sent += send_ret;
        }

        return sent;
    }

public:
    size_t send(const void* buffer, size_t send_length) const {
        return ((const S*)this)->send_impl(buffer, send_length);
    }

    template <typename T, size_t len>
    inline bool send_text(const T(&str)[len]) const {
        return this->send(&str[0], sizeof(str) - sizeof(T));
    }

    template <typename T>
    inline bool send(const T& buffer) const {
        return this->send(&buffer, sizeof(buffer));
    }

    bool send_http_get(const char* path = "/", const char* extra_header_data = "") const {
        auto [request, request_length] = build_new_str<char>(get_request_header_part_1, path, get_request_header_part_2, extra_header_data, get_request_header_part_3);
        bool ret = this->send(request, request_length);
        free(request);
        return ret;
    }
};

template<bool enable_ssl = false>
struct Socket;

template<>
struct Socket<false> : SocketBase<Socket<false>> {
};

#if ENABLE_SSL
template<>
struct Socket<true> : SocketBase<Socket<true>> {
    friend SocketBase<Socket<true>>;

protected:
    bool tls_in_use = false;
    SecPkgContext_StreamSizes tls_sizes;

    // Who knows WTF windows does with these, so
    // don't make any weird promises to the compiler
    mutable CredHandle tls_handle;
    mutable CtxtHandle tls_context;

    
    static inline constexpr SCHANNEL_CRED tls_cred = {
        .dwVersion = SCHANNEL_CRED_VERSION,
        .grbitEnabledProtocols = SP_PROT_TLS1_2,  // allow only TLS v1.2
        .dwFlags = SCH_USE_STRONG_CRYPTO |          // use only strong crypto alogorithms
                   SCH_CRED_AUTO_CRED_VALIDATION |  // automatically validate server certificate
                   SCH_CRED_NO_DEFAULT_CREDS     // no client certificate authentication
    };

    inline void initialize_secondary(SOCKET sock, const wchar_t* server_name) {
        this->tls_in_use = this->initialize_tls(server_name);
    }

    inline void close_secondary() {
        if (this->tls_in_use) {
            DeleteSecurityContext(&this->tls_context);
            FreeCredentialsHandle(&this->tls_handle);
        }
    }

    
    // Adapted from this: https://github.com/odzhan/shells/blob/master/s6/tls.c
    bool initialize_tls(const wchar_t* server_name) {
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

    inline size_t receive_impl(void* buffer, size_t recv_length) const {
        uint8_t* recv_buffer = (uint8_t*)buffer;
        if (expect(!this->tls_in_use, false)) {
            int recv_ret = ::recv(this->sock, (char*)recv_buffer, recv_length, 0);
            if (expect(recv_ret <= 0, false)) {
                //return 0;
                recv_ret = 0;
            }
            return recv_ret;
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
    }

    inline size_t send_impl(const void* buffer, size_t send_length) const {

        uint8_t* send_buffer = (uint8_t*)buffer;

        size_t total_sent_length = 0;

        while (send_length) {
            char* current_send_buffer = (char*)send_buffer;
            size_t current_buffer_read_length = send_length;
            size_t current_send_length = current_buffer_read_length;
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
            size_t sent = 0;
            while (sent != current_send_length) {
                int send_ret = ::send(this->sock, &current_send_buffer[sent], current_send_length - sent, 0);
                if (expect(send_ret <= 0, false)) {
                    return 0;
                }
                sent += send_ret;
                total_sent_length += send_ret;
            }
            send_buffer += current_buffer_read_length;
            send_length -= current_buffer_read_length;
        }
        return total_sent_length;
    }
};
#endif

#define LOCK_ALL_MESSAGES 1

static uint32_t filled_message_slots;
static uint32_t recent_message_write_index;
static _Atomic(uint8_t) recent_message_lock;

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
        if constexpr (recent_message_max_count * recent_message_max_length <= 1024) {
            // memmove makes better code here than memcpy
            // because the memory addresses are read in order
            __builtin_memmove(recent_messages[index], message.data(), recent_message_max_length);
        } else {
            __builtin_memcpy(recent_messages[index], message.data(), recent_message_max_length);
        }
        filled_message_slots |= 1 << index;
        recent_message_write_index = index + 1 & recent_message_wrap_mask;
    }
}

#define PARSE_TAGS 0

#if !ENABLE_SSL_FOR_TWITCH
static Socket<false> twitch_socket;
#else
static Socket<true> twitch_socket;
#endif

static _Atomic(int8_t) vote_thread_stop;

template<typename L1, typename L2>
void parse_twitch_chat(const L1& pre_message, const L2& parse) noexcept {
    do {

        pre_message();

        if (size_t data_length = twitch_socket.receive(receive_buffer, receive_buffer_size)) {
            //receive_buffer[data_length] = '\0';
            //printf("%.*s\n", data_length, receive_buffer);
            //continue;

            size_t start_of_message = 0;
            for (size_t i = 0; i < data_length; ++i) {
                // All messages are terminated with \r\n,
                // so it's always going to be safe to read
                // 1 extra character past a null terminator
                // when formatted this way.
                if (expect(receive_buffer[i] == '\r', false)) {
                    receive_buffer[i] = '\0'; // Add a null terminator so that strstr doesn't explode

                    char* message = (char*)&receive_buffer[start_of_message];
                    size_t message_length = i - start_of_message;

                    if (
                        message_length >= sizeof("PING :tmi.twitch.tv") - 1 &&
                        strstr(message, "PING :tmi.twitch.tv")
                    ) {
                        // This is probably wrong but the other twitch
                        // clients do the same thing so whatever
                        twitch_socket.send_text("PONG :tmi.twitch.tv\r\n");
                    }
                    else if (strstr(message, "PRIVMSG")) {
                        // Twitch's example parser sucks
#if PARSE_TAGS
                        // 1 points to the start of tags
                        size_t j = message[0] == '@';
                        if (j) while (message[j++] != ' ');
#else
                        size_t j = 0;
#endif
                        size_t k = j++;
                        // j now points to the ':' at the start of a message
                        while (message[++k] != '!'); // find the end of the username
                        message[k] = '\0'; // add a null terminator to the username
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
    } while (!vote_thread_stop);

    twitch_socket.close();
}

struct HashedString {
    size_t hash;

    inline constexpr HashedString(const std::string_view& str) : hash(std::hash<std::string_view>{}(str)) {}

    inline constexpr bool operator==(const HashedString& val) const {
        return this->hash == val.hash;
    }
};

static _Atomic(uint8_t) reset_votes;

static _Atomic(uint8_t) enable_voting;

static inline constexpr uint32_t valid_votes_default = 0b00001111;
static _Atomic(uint32_t) valid_votes;

#if USE_VECTOR_FOR_NAME_HASHES
static std::vector<HashedString> already_voted;
static inline bool record_vote_username(const std::string_view& username) {
    HashedString new_hash = HashedString(username);
    for (auto& hash : already_voted) {
        if (hash == new_hash) {
            return false;
        }
    }
    already_voted.push_back(new_hash);
    return true;
}
#else
template<>
struct std::hash<HashedString> {
    size_t operator()(const HashedString& str) const {
        return str.hash;
    }
};
static std::unordered_set<HashedString> already_voted;
static inline bool record_vote_username(const std::string_view& username) {
    return already_voted.emplace(username).second;
}
#endif

alignas(16) static size_t vote_totals[8];

struct zthread : std::thread {

    using std::thread::thread;

    ~zthread() {
        if (this->joinable()) {
            vote_thread_stop = true;
            this->join();
        }
    }
};
static zthread vote_thread;

void __fastcall twitch_voting(bool state) {
    enable_voting = state;
}

void __fastcall stop_twitch_thread() {
    vote_thread.~zthread();
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

bool __fastcall start_twitch_thread(const char* channel_name) {
    if (expect(enable(), true)) {
        if (expect(twitch_socket.connect(twitch_server_name, twitch_port_str), true)) {
#if PARSE_TAGS
            socket.send_text("CAP REQ :"
#if PARSE_TAGS
                             "twitch.tv/tags "
#endif
                             "\n"
            );
#endif
            // Ripped these from https://github.com/xenova/twitch-chat-irc/tree/master
            // This should probably be updated with something else?
            twitch_socket.send_text("PASS SCHMOOPIIE\n");
            twitch_socket.send_text("NICK justinfan67420\n");

            //u_long mode = 1;
            //auto yeetus2 = ioctlsocket(socket.sock, FIONBIO, &mode);
            DWORD timeout = 1000;
            setsockopt(twitch_socket.sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

            size_t data_length = twitch_socket.receive(receive_buffer, receive_buffer_size);
            //receive_buffer[data_length] = '\0';
            //printf("%s", receive_buffer);

            if (expect(
                data_length != 0 &&
                // Check for the success string to try and end string search
                // faster than scanning the whole buffer for the fail message
                strstr((char*)receive_buffer, "Welcome, GLHF!") != NULL
            , true)) {

                // Reuse the receive buffer to store the join message
                __builtin_memcpy(receive_buffer, "JOIN #", 6);
                size_t channel_length = str_len(channel_name);

                uint8_t* channel_name_write = &receive_buffer[6];
                for (size_t i = 0; i < channel_length; ++i) {
                    *channel_name_write++ = channel_name[i];
                }
                *channel_name_write++ = '\n';

                twitch_socket.send(receive_buffer, channel_name_write - &receive_buffer[0]);

                data_length = twitch_socket.receive(receive_buffer, receive_buffer_size);
                //receive_buffer[data_length] = '\0';
                //printf("%s", receive_buffer);

                if (expect(
                    data_length != 0 &&
                    // Check if the channel is valid
                    strstr((char*)receive_buffer, "JOIN") != NULL
                , true)) {

                    // Ignore the other join messages, we don't care
                    while ((data_length = twitch_socket.receive(receive_buffer, receive_buffer_size)));

                    // Initialize all the static variables
                    valid_votes = valid_votes_default;
                    __builtin_memset(vote_totals, 0, sizeof(vote_totals));
                    reset_votes = false;
                    enable_voting = false;
                    filled_message_slots = 0;
                    recent_message_write_index = 0;
                    recent_message_lock = false;
                    vote_thread_stop = false;
                    already_voted.clear();

                    new (&vote_thread) zthread(
                        []() {
                            parse_twitch_chat(
                                // Pre-message function
                                []() {
                                    if (expect(reset_votes, false)) {
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
                                    if (
                                        vote < 8 &&
                                        enable_voting
                                    ) {
                                        // Message is an attempt at voting and
                                        // shouldn't count towards recent messages
                                        if (
                                            (valid_votes & 1 << vote) &&
                                            record_vote_username(username)
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
                    return true;
                }
#if PRINT_ERROR_MESSAGES
                else {
                    printf("Error: Couldn't connect to channel \"%s\"\n", channel_name);
                }
#endif
            }
#if PRINT_ERROR_MESSAGES
            else {
                printf("Error: Twitch login failed\n");
            }
#endif
            // Something went wrong, so close the socket
            twitch_socket.close();
        }
#if PRINT_ERROR_MESSAGES
        else {
            printf("Error: Couldn't connect to twitch\n");
        }
#endif
    }
    return false;
}

#if ENABLE_YOUTUBE
#include "..\zero_private\youtube_api_key.hpp"

static Socket<true> youtube_socket;

bool __fastcall start_youtube_thread(const char* channel_id) {
    if (expect(enable(), true)) {
        if (expect(youtube_socket.connect(googleapis_server_name_w, googleapis_port_str), true)) {
            auto [path, length] = build_new_str<char>("/youtube/v3/search?part=snippet&channelId=", channel_id, "&eventType=live&type=video&key=", youtube_api_key);
            auto [extra_headers, extra_length] = build_new_str<char>("Host:", googleapis_server_name, "\r\n");
            youtube_socket.send_http_get(path, extra_headers);
            free(path);
            free(extra_headers);

            size_t data_length = twitch_socket.receive(receive_buffer, receive_buffer_size);

            receive_buffer[data_length] = '\0';
            printf("%s", receive_buffer);

            youtube_socket.close();
        } else {
            printf("connection failed\n");
        }
    }
    return true;
}
#endif