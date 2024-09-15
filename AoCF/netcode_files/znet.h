#pragma once

#ifndef ZNET_H
#define ZNET_H 1

#define NOMINMAX 1

// These types aren't even used in the code
// but clang is warning about them anyway
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <vector>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <atomic>
#include <utility>
#include <type_traits>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#define ENABLE_SSL 0

#if ENABLE_SSL
#define SECURITY_WIN32
#include <security.h>
#include <schannel.h>
#include <sspi.h>
#endif

#include <windows.h>

#include <WinDNS.h>

#include "util.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Dnsapi")
#if ENABLE_SSL
#pragma comment (lib, "secur32.lib")
#endif

enum SocketType {
    TcpSocket,
    UdpSocket
};

template<typename S>
struct SocketBase {
    SOCKET sock = INVALID_SOCKET; // 0x0

protected:

    static inline constexpr size_t MAX_ADDR_BUFF_SIZE = (std::max)(INET_ADDRSTRLEN, INET6_ADDRSTRLEN);

    static inline constexpr ADDRINFOW tcp_addr_hint = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    static inline constexpr ADDRINFOW udp_addr_hint = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_DGRAM,
        .ai_protocol = IPPROTO_UDP
    };

    //declare_has_func(initialize_secondary);
    //declare_has_func(close_secondary);

    inline bool initialize_secondary(SOCKET sock, const wchar_t* server_name) {
        return true;
    }

    bool initialize(SOCKET sock, const wchar_t* server_name) {
        this->sock = sock;
        return ((S*)this)->initialize_secondary(sock, server_name);
    }

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

    template<auto& addr_hint>
    bool connect(const wchar_t* server_name, const wchar_t* port_str) {
        // GetAddrInfoW just crashes for some reason when trying to
        // resolve a URL on Windows 7, so the DNS resolution needs
        // to be done manually.
        // THIS IS DUMB

        wchar_t addr_buff[MAX_ADDR_BUFF_SIZE];
        PDNS_RECORDW records;
        if (expect(DnsQuery_W(server_name, DNS_TYPE_AAAA, DNS_QUERY_DUAL_ADDR, NULL, (PDNS_RECORD*)&records, NULL) == 0, true)) {
            PDNS_RECORDW cur_record = records;
            do {
                void* ipv4;
                switch (cur_record->wType) {
                    [[unlikely]] default:
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
        if (expect(!GetAddrInfoW(addr_buff, port_str, addr_hint, &addrs), true)) {
            PADDRINFOW cur_addr = addrs;
            do {
                switch (cur_addr->ai_family) {
                    case AF_INET:
                    case AF_INET6:
                        //new (out) AnySockaddr(addrs->ai_addr, addrs->ai_addrlen);
                        //FreeAddrInfoW(addrs);
                        SOCKET sock = socket(cur_addr->ai_family, cur_addr->ai_socktype, cur_addr->ai_protocol);
                        if (expect(sock != INVALID_SOCKET, true)) {
                            if (expect(::connect(sock, cur_addr->ai_addr, (int)cur_addr->ai_addrlen) != SOCKET_ERROR, true)) {
                                FreeAddrInfoW(addrs);
                                return this->initialize(sock, server_name);
                            }
                        }
                }
            } while ((cur_addr = cur_addr->ai_next));
            FreeAddrInfoW(addrs);
        }
        return false;
    }

public:

    bool connect_tcp(const wchar_t* server_name, const wchar_t* port_str) {
        return this->connect<tcp_addr_hint>(server_name, port_str);
    }

    bool connect_udp(const wchar_t* server_name, const wchar_t* port_str) {
        return this->connect<udp_addr_hint>(server_name, port_str);
    }

protected:
    inline size_t receive_impl(void* buffer, size_t recv_length) const {
        int recv_ret = ::recv(this->sock, (char*)buffer, recv_length, 0);
        if (expect(recv_ret <= 0, false)) {
            recv_ret = 0;
        }
        return recv_ret;
    }

public:

    size_t receive(void* buffer, size_t recv_length) const {
        return ((const S*)this)->receive_impl(buffer, recv_length);
    }

    template <typename T, size_t len>
    size_t receive_text(T(&buffer)[len]) const {
        size_t ret = this->receive(buffer, sizeof(buffer) - sizeof(T));
        buffer[ret] = (T)0;
        return ret;
    }

    template <typename T>
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
};


#if !ENABLE_SSL
template<bool ssl_dummy = false>
struct Socket : SocketBase<Socket<false>> {};
#else
template<bool enable_ssl = false>
struct Socket;

template<>
struct Socket<false> : SocketBase<Socket<false>> {};

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

    static inline constexpr size_t PACKET_BUFFER_SIZE = UINT16_MAX + 1;
    
    static inline constexpr SCHANNEL_CRED tls_cred = {
        .dwVersion = SCHANNEL_CRED_VERSION,
        .grbitEnabledProtocols = SP_PROT_TLS1_2,  // allow only TLS v1.2
        .dwFlags = SCH_USE_STRONG_CRYPTO |          // use only strong crypto alogorithms
                   SCH_CRED_AUTO_CRED_VALIDATION |  // automatically validate server certificate
                   SCH_CRED_NO_DEFAULT_CREDS     // no client certificate authentication
    };

    inline bool initialize_secondary(SOCKET sock, const wchar_t* server_name) {
        return this->tls_in_use = this->initialize_tls(server_name);
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

                char packet_buffer[PACKET_BUFFER_SIZE];

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
                        memmove(packet_buffer, &packet_buffer[received - inbuffers[1].cbBuffer], inbuffers[1].cbBuffer);
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
                recv_ret = 0;
            }
            return recv_ret;
        }
        size_t received_size = 0;
        size_t total_received = 0;

        uint8_t* initial_recv_buffer = recv_buffer;
        
        char packet_buffer[PACKET_BUFFER_SIZE];

        while (recv_length) {

            if (total_received) {
                // Why?
                return total_received;
            }

            if (expect(received_size == sizeof(packet_buffer), false)) {
                // Is this even a possible error?
                return 0;
            }
            int recv_ret = ::recv(this->sock, &packet_buffer[received_size], sizeof(packet_buffer) - received_size, 0);
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
                            memcpy(recv_buffer, decrypted_data, current_write_length);
                            recv_buffer += current_write_length;
                            recv_length -= current_write_length;
                            total_received += current_write_length;
                            decrypt_data_length -= current_write_length;
                            decrypted_data += current_write_length;
                        } while (current_write_length != decrypt_data_length);
                        memmove(packet_buffer, &packet_buffer[used_length], received_size - used_length);
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
        
        char packet_buffer[PACKET_BUFFER_SIZE];

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
                memcpy(buffers[1].pvBuffer, send_buffer, current_buffer_read_length);
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

static inline bool winsock_initialized = false;

static inline bool enable_winsock() {
    WSADATA idgaf_about_winsock_version;
    if (expect(!winsock_initialized, false)) {
        winsock_initialized = !WSAStartup(WINSOCK_VERSION, &idgaf_about_winsock_version);
    }
    return winsock_initialized;
}

#endif