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
#include <wchar.h>

#include <vector>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <atomic>
#include <utility>
#include <type_traits>
#include <bit>
#include <atomic>

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

#include "..\zero\util.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Dnsapi")
#if ENABLE_SSL
#pragma comment (lib, "secur32.lib")
#endif

namespace zero {

namespace util {
template <typename T>
using SIForSize = std::conditional_t<sizeof(T) == sizeof(int8_t), int8_t,
                  std::conditional_t<sizeof(T) == sizeof(int16_t), int16_t,
                  std::conditional_t<sizeof(T) == sizeof(int32_t), int32_t,
                  std::conditional_t<sizeof(T) == sizeof(int64_t), int64_t,
                  void>>>>;

template <typename T>
using UIForSize = std::conditional_t<sizeof(T) == sizeof(uint8_t), uint8_t,
                  std::conditional_t<sizeof(T) == sizeof(uint16_t), uint16_t,
                  std::conditional_t<sizeof(T) == sizeof(uint32_t), uint32_t,
                  std::conditional_t<sizeof(T) == sizeof(uint64_t), uint64_t,
                  void>>>>;

template <typename T>
static constexpr UIForSize<T> bit_cast_from_size(T value) {
    return std::bit_cast<UIForSize<T>>(value);
}

template <typename T>
static constexpr T bit_cast_to_size(UIForSize<T> value) {
    return std::bit_cast<T>(value);
}

template <typename T>
static constexpr T bswap(T value) {
    auto temp = bit_cast_from_size<T>(value);
    if constexpr (sizeof(T) == sizeof(uint16_t)) {
        temp = __builtin_bswap16(temp);
    }
    else if constexpr (sizeof(T) == sizeof(uint32_t)) {
        temp = __builtin_bswap32(temp);
    }
    else if constexpr (sizeof(T) == sizeof(uint64_t)) {
        temp = __builtin_bswap64(temp);
    }
    return bit_cast_to_size<T>(temp);
}

}

namespace net {

enum SocketType : uint8_t {
    TCP,
    UDP
};



static inline constexpr IP6_ADDRESS map_ipv4_to_ipv6(const IP4_ADDRESS& addr) {
    return (IP6_ADDRESS) {
        .IP6Dword = {
            0x00000000, 0x00000000, 0xFFFF0000, addr
        }
    };
}

static inline constexpr IP6_ADDRESS map_ip_to_ipv6(const sockaddr* addr) {
    switch (addr->sa_family) {
        case AF_INET:
            return map_ipv4_to_ipv6(*(const IP4_ADDRESS*)&((const sockaddr_in*)addr)->sin_addr);
        case AF_INET6:
            return *(const IP6_ADDRESS*)&((const sockaddr_in6*)addr)->sin6_addr;
        [[unlikely]] default:
            return {};
    }
}

uint16_t get_port(const sockaddr* addr) {
    switch (addr->sa_family) {
        case AF_INET:
            return ((const sockaddr_in*)addr)->sin_port;
        case AF_INET6:
            return ((const sockaddr_in6*)addr)->sin6_port;
        [[unlikely]] default:
            return 0;
    }
}

void print_ipv4(IP4_ADDRESS addr) {
    union {
        uint32_t temp;
        uint8_t as_bytes[4];
    };
    temp = addr;
    printf(
        "v4: %u.%u.%u.%u"
        , as_bytes[0], as_bytes[1], as_bytes[2], as_bytes[3]
    );
}

void print_ipv6(IP6_ADDRESS addr) {
    printf(
        "v6: %0hX:%0hX:%0hX:%0hX:%0hX:%0hX:%0hX:%0hX"
        , __builtin_bswap16(addr.IP6Word[0]), __builtin_bswap16(addr.IP6Word[1])
        , __builtin_bswap16(addr.IP6Word[2]), __builtin_bswap16(addr.IP6Word[3])
        , __builtin_bswap16(addr.IP6Word[4]), __builtin_bswap16(addr.IP6Word[5])
        , __builtin_bswap16(addr.IP6Word[6]), __builtin_bswap16(addr.IP6Word[7])
    );
    if (
        addr.IP6Dword[0] == 0 &&
        addr.IP6Dword[1] == 0 &&
        addr.IP6Dword[2] == 0xFFFF0000
    ) {
        printf(" ");
        print_ipv4(addr.IP6Dword[3]);
    }
}

void print_sockaddr(const sockaddr* addr) {
    switch (addr->sa_family) {
        case AF_INET:
            print_ipv4(*(IP4_ADDRESS*)&((const sockaddr_in*)addr)->sin_addr);
            break;
        case AF_INET6:
            print_ipv6(*(IP6_ADDRESS*)&((const sockaddr_in6*)addr)->sin6_addr);
            break;
    }
}

struct sockaddr_any {
    sockaddr_storage storage;
    size_t length;

    sockaddr_any() = default;

    sockaddr_any(const sockaddr* addr, size_t length) : length(length) {
        memcpy(&this->storage, addr, length);
    }

    void initialize(const sockaddr* addr, size_t length) {
        memcpy(&this->storage, addr, this->length = length);
    }

    void initialize_v4_to_v6(const sockaddr* addr) {
        this->length = sizeof(sockaddr_in6);
        const sockaddr_in* addr4 = (const sockaddr_in*)addr;
        sockaddr_in6* addr6 = (sockaddr_in6*)&this->storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = addr4->sin_port;
        IP6_ADDRESS temp = map_ipv4_to_ipv6(*(IP4_ADDRESS*)&addr4->sin_addr);
        addr6->sin6_addr = *(IN6_ADDR*)&temp;
    }

    operator sockaddr*() const {
        return (sockaddr*)&this->storage;
    }
};

static inline DNS_STATUS WINAPI dns_query(PCSTR pszName, WORD wType, DWORD Options, PVOID pExtra, PDNS_RECORDA* ppQueryResults, PVOID* pReserved) {
    return ::DnsQuery_A(pszName, wType, Options, pExtra, (PDNS_RECORD*)ppQueryResults, pReserved);
}
static inline DNS_STATUS WINAPI dns_query(PCWSTR pszName, WORD wType, DWORD Options, PVOID pExtra, PDNS_RECORDW* ppQueryResults, PVOID* pReserved) {
    return ::DnsQuery_W(pszName, wType, Options, pExtra, (PDNS_RECORD*)ppQueryResults, pReserved);
}
static inline const wchar_t* inet_ntop(int Family, const void* pAddr, wchar_t* pStringBuf, size_t StringBufSize) {
    return ::InetNtopW(Family, pAddr, pStringBuf, StringBufSize);
}
template <size_t N>
static inline const char* inet_ntop(int Family, const void* pAddr, char(&str)[N]) {
    return ::inet_ntop(Family, pAddr, str, N);
}
template <size_t N>
static inline const wchar_t* inet_ntop(int Family, const void* pAddr, wchar_t(&str)[N]) {
    return ::InetNtopW(Family, pAddr, str, N);
}
static inline int inet_pton(int Family, const wchar_t* pszAddrString, void* pAddrBuf) {
    return ::InetPtonW(Family, pszAddrString, pAddrBuf);
}

static inline int getaddrinfo(const wchar_t* pNodeName, const wchar_t* pServiceName, const ADDRINFOW* pHints, PADDRINFOW* ppResult) {
    return ::GetAddrInfoW(pNodeName, pServiceName, pHints, ppResult);
}
static inline void freeaddrinfo(PADDRINFOW pAddrInfo) {
    return ::FreeAddrInfoW(pAddrInfo);
}

static inline constexpr size_t MAX_ADDR_BUFF_SIZE = (std::max)(INET_ADDRSTRLEN, INET6_ADDRSTRLEN);

template <typename T, size_t N> requires(N >= MAX_ADDR_BUFF_SIZE)
static bool resolve_address(const T* server_name, T(&addr_buff)[N]) {
    using PDNS_RECORD = std::conditional_t<std::is_same_v<T, wchar_t>, PDNS_RECORDW, PDNS_RECORDA>;
    
    // GetAddrInfoW just crashes for some reason when trying to
    // resolve a URL on Windows 7, so the DNS resolution needs
    // to be done manually.
    // THIS IS DUMB

    PDNS_RECORD records;
    if (expect(dns_query(server_name, DNS_TYPE_AAAA, DNS_QUERY_DUAL_ADDR, NULL, &records, NULL) == 0, true)) {
        PDNS_RECORD cur_record = records;
        do {
            switch (cur_record->wType) {
                [[unlikely]] default:
                case DNS_TYPE_CNAME:
                    break;
                case DNS_TYPE_AAAA:
                    inet_ntop(AF_INET6, &cur_record->Data.AAAA.Ip6Address, addr_buff);
                    goto found_addr;
                case DNS_TYPE_A:
                    IP6_ADDRESS ipv6 = map_ipv4_to_ipv6(cur_record->Data.A.IpAddress);
                    inet_ntop(AF_INET6, &ipv6, addr_buff);
                    goto found_addr;
            }
        } while ((cur_record = cur_record->pNext));
        ::DnsFree(records, DnsFreeRecordList);
    }
    return false;

found_addr:
    ::DnsFree(records, DnsFreeRecordList);
    return true;
}

template<SocketType>
static inline SOCKET socket(int af);

template<>
inline SOCKET socket<TCP>(int af) {
    return ::socket(af, SOCK_STREAM, IPPROTO_TCP);
}

template<>
inline SOCKET socket<UDP>(int af) {
    return ::socket(af, SOCK_DGRAM, IPPROTO_UDP);
}

template<typename T>
static inline int setsockopt(SOCKET s, int level, int optname, const T& optval) {
    return ::setsockopt(s, level, optname, (const char*)&optval, sizeof(optval));
}

template<typename T>
static inline int getsockopt(SOCKET s, int level, int optname, T& optval) {
    int optlen = sizeof(T);
    return ::getsockopt(s, level, optname, (char*)&optval, &optlen);
}

template<typename T>
static inline int connect(SOCKET s, const T& addr) {
    return ::connect(s, (const sockaddr*)&addr, sizeof(addr));
}

static inline int connect(SOCKET s, const sockaddr_any& addr) {
    return ::connect(s, addr, addr.length);
}

template<typename T>
static inline int bind(SOCKET s, const T& addr) {
    return ::bind(s, (const sockaddr*)&addr, sizeof(addr));
}

static inline int bind(SOCKET s, const sockaddr_any& addr) {
    return ::bind(s, addr, addr.length);
}

template<typename T> requires(!std::is_pointer_v<T>)
static inline int send(SOCKET s, const T& data, int flags) {
    return ::send(s, (const char*)&data, sizeof(T), flags);
}

template<typename T>
static inline int send(SOCKET s, const T* data, int flags) {
    return ::send(s, (const char*)data, sizeof(T), flags);
}

template<typename T> requires(!std::is_pointer_v<T>)
static inline int sendto(SOCKET s, const T& data, int flags, const sockaddr* to, int tolen) {
    return ::sendto(s, (const char*)&data, sizeof(T), flags, to, tolen);
}

template<typename T>
static inline int sendto(SOCKET s, const T* data, int flags, const sockaddr* to, int tolen) {
    return ::sendto(s, (const char*)data, sizeof(T), flags, to, tolen);
}

template<typename T> requires(!std::is_pointer_v<T>)
static inline int recv(SOCKET s, T& data, int flags) {
    return ::recv(s, (char*)&data, sizeof(T), flags);
}

template<typename T>
static inline int recv(SOCKET s, T* data, int flags) {
    return ::recv(s, (char*)data, sizeof(T), flags);
}

template<typename T> requires(!std::is_pointer_v<T>)
static inline int recvfrom(SOCKET s, T& data, int flags, sockaddr* from, int* fromlen) {
    return ::recvfrom(s, (char*)&data, sizeof(T), flags, from, fromlen);
}

template<typename T>
static inline int recvfrom(SOCKET s, T* data, int flags, sockaddr* from, int* fromlen) {
    return ::recvfrom(s, (char*)data, sizeof(T), flags, from, fromlen);
}

static inline SOCKET accept(SOCKET s, sockaddr_any& addr) {
    addr.length = sizeof(addr.storage);
    return ::accept(s, addr, (int*)&addr.length);
}

static inline int getpeername(SOCKET s, sockaddr_any& addr) {
    addr.length = sizeof(addr.storage);
    return ::getpeername(s, addr, (int*)&addr.length);
}

static inline int getsockname(SOCKET s, sockaddr_any& addr) {
    addr.length = sizeof(addr.storage);
    return ::getsockname(s, addr, (int*)&addr.length);
}

template<typename T> requires(!std::is_pointer_v<T>)
static inline int sendto(SOCKET s, const T& data, int flags, const sockaddr_any& to) {
    return ::sendto(s, (const char*)&data, sizeof(T), flags, to, to.length);
}

template<typename T>
static inline int sendto(SOCKET s, const T* data, int flags, const sockaddr_any& to) {
    return ::sendto(s, (const char*)data, sizeof(T), flags, to, to.length);
}

static inline int sendto(SOCKET s, const char* buf, int len, int flags, const sockaddr_any& to) {
    return ::sendto(s, buf, len, flags, to, to.length);
}

template<typename T> requires(!std::is_pointer_v<T>)
static inline int recvfrom(SOCKET s, T& data, int flags, sockaddr_any& from) {
    from.length = sizeof(from.storage);
    return ::recvfrom(s, (char*)&data, sizeof(T), flags, from, (int*)&from.length);
}

template<typename T>
static inline int recvfrom(SOCKET s, T* data, int flags, sockaddr_any& from) {
    from.length = sizeof(from.storage);
    return ::recvfrom(s, (char*)data, sizeof(T), flags, from, (int*)&from.length);
}

static inline int recvfrom(SOCKET s, char* buf, int len, int flags, sockaddr_any& from) {
    from.length = sizeof(from.storage);
    return ::recvfrom(s, buf, len, flags, from, (int*)&from.length);
}

template<typename T>
static inline constexpr T hton(T value) {
    if constexpr (std::endian::native == std::endian::little) {
        return util::bswap(value);
    }
    else if constexpr (std::endian::native == std::endian::big) {
        return value;
    }
    else {
        static_assert(false);
    }
}

template<typename T>
static inline constexpr T ntoh(T value) {
    if constexpr (std::endian::native == std::endian::little) {
        return util::bswap(value);
    }
    else if constexpr (std::endian::native == std::endian::big) {
        return value;
    }
    else {
        static_assert(false);
    }
}

template<typename S>
struct SocketBase {
    SOCKET sock = INVALID_SOCKET; // 0x0

    inline operator bool() const {
        return this->sock != INVALID_SOCKET;
    }

protected:

    static inline constexpr ADDRINFOA tcp_addr_hint_a = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    static inline constexpr ADDRINFOA udp_addr_hint_a = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_DGRAM,
        .ai_protocol = IPPROTO_UDP
    };

    static inline constexpr ADDRINFOW tcp_addr_hint_w = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    static inline constexpr ADDRINFOW udp_addr_hint_w = {
        .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_DGRAM,
        .ai_protocol = IPPROTO_UDP
    };

public:

    template<SocketType socket_type>
    bool initialize() {
        SOCKET sock = zero::net::socket<socket_type>(AF_INET6);
        if (expect(sock != INVALID_SOCKET, true)) {
            this->sock = sock;
            zero::net::setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, FALSE);
            return true;
        }
        return false;
    }

protected:

    inline void close_secondary() {}

public:

    void close() {
        SOCKET sock = this->sock;
        if (sock != INVALID_SOCKET) {
            ::closesocket(sock);
            this->sock = INVALID_SOCKET;
            ((S*)this)->close_secondary();
        }
    }

protected:

    inline bool connect_secondary(const char* server_name) {
        return true;
    }

    inline bool connect_secondary(const wchar_t* server_name) {
        return true;
    }

    template<SocketType socket_type, typename T>
    bool connect_ipv6_impl(const T* server_name, const T* port_str, const T* ip) const {

        using ADDRINFO = std::conditional_t<std::is_same_v<T, wchar_t>, ADDRINFOW, ADDRINFOA>;

        const ADDRINFO* addr_hint;
        if constexpr (std::is_same_v<T, wchar_t>) {
            if constexpr (socket_type == TCP) {
                addr_hint = &tcp_addr_hint_w;
            } else if constexpr (socket_type == UDP) {
                addr_hint = &udp_addr_hint_w;
            }
        } else {
            if constexpr (socket_type == TCP) {
                addr_hint = &tcp_addr_hint_a;
            } else if constexpr (socket_type == UDP) {
                addr_hint = &udp_addr_hint_a;
            }
        }

        ADDRINFO* addrs;
        // GetAddrInfoW sucks
        if (expect(!getaddrinfo(ip, port_str, addr_hint, &addrs), true)) {
            ADDRINFO* cur_addr = addrs;
            do {
                switch (cur_addr->ai_family) {
                    case AF_INET: case AF_INET6:
                        if (expect(::connect(this->sock, (const sockaddr*)cur_addr->ai_addr, cur_addr->ai_addrlen) == 0, true)) {
                            freeaddrinfo(addrs);
                            return ((S*)this)->connect_secondary(server_name);
                        } //else {
                            //printf("Error: %d\n", WSAGetLastError());
                        //}
                }
                //print_sockaddr(full_sock);
            } while ((cur_addr = cur_addr->ai_next));
            freeaddrinfo(addrs);
        }
        return false;
    }

    template<SocketType socket_type, typename T>
    bool connect_ipv4_impl(const T* server_name, const T* port_str, const T* ip) const {
        T addr_buff[MAX_ADDR_BUFF_SIZE];
        IP6_ADDRESS ipv6 = (IP6_ADDRESS){
            .IP6Dword = {
                0x00000000, 0x00000000, 0xFFFF0000, 0
            }
        };
        if constexpr (std::is_same_v<T, wchar_t>) {
            inet_pton(AF_INET, ip, &ipv6.IP6Dword[3]);
        } else {
            ::inet_pton(AF_INET, ip, &ipv6.IP6Dword[3]);
        }
        inet_ntop(AF_INET6, &ipv6, addr_buff);
        return this->connect_ipv6_impl<socket_type>(server_name, port_str, addr_buff);
    }

public:

    bool connect(const sockaddr_any& addr) const {
        return zero::net::connect(this->sock, addr) == 0;
    }

    template<SocketType socket_type>
    bool connect_ipv4(const char* ip, const char* port_str) const {
        return this->connect_ipv4_impl<socket_type, char>(NULL, port_str, ip);
    }

    template<SocketType socket_type>
    bool connect_ipv4(const wchar_t* ip, const wchar_t* port_str) const {
        return this->connect_ipv4_impl<socket_type, wchar_t>(NULL, port_str, ip);
    }

    template<SocketType socket_type>
    bool connect_ipv6(const char* ip, const char* port_str) const {
        return this->connect_ipv6_impl<socket_type, char>(NULL, port_str, ip);
    }

    template<SocketType socket_type>
    bool connect_ipv6(const wchar_t* ip, const wchar_t* port_str) const {
        return this->connect_ipv6_impl<socket_type, wchar_t>(NULL, port_str, ip);
    }

    template<SocketType socket_type>
    bool connect_ip(const char* ip, const char* port_str) const {
        if (strchr(ip, '.')) {
            return this->connect_ipv4<socket_type>(ip, port_str);
        } else {
            return this->connect_ipv6<socket_type>(ip, port_str);
        }
    }

    template<SocketType socket_type>
    bool connect_ip(const wchar_t* ip, const wchar_t* port_str) const {
        if (wcschr(ip, '.') {
            return this->connect_ipv4<socket_type>(ip, port_str);
        } else {
            return this->connect_ipv6<socket_type>(ip, port_str);
        }
    }

    template<SocketType socket_type>
    bool connect(const char* server_name, const char* port_str) const {
        char addr_buff[MAX_ADDR_BUFF_SIZE];
        if (zero::net::resolve_address(server_name, addr_buff)) {
            return this->connect_ipv6_impl<socket_type>(server_name, port_str, addr_buff);
        }
        return false;
    }

    template<SocketType socket_type>
    bool connect(const wchar_t* server_name, const wchar_t* port_str) const {
        wchar_t addr_buff[MAX_ADDR_BUFF_SIZE];
        if (zero::net::resolve_address(server_name, addr_buff)) {
            return this->connect_ipv6_impl<socket_type>(server_name, port_str, addr_buff);
        }
        return false;
    }

    bool bind(const IP6_ADDRESS& ip, uint16_t local_port) const {
        sockaddr_in6 local = {
            .sin6_family = AF_INET6,
            .sin6_port = hton(local_port),
            .sin6_addr = *(const IN6_ADDR*)&ip
        };
        return zero::net::bind(this->sock, local) == 0;
    }

    bool bind(const IP4_ADDRESS& ip, uint16_t local_port) const {
        return this->bind(map_ipv4_to_ipv6(ip), local_port);
    }

    bool bind(uint16_t local_port) const {
        return this->bind(IN6ADDR_ANY_INIT, local_port);
    }

    bool listen(int limit) const {
        return ::listen(this->sock, limit) == 0;
    }

    template<typename T = S>
    T accept(sockaddr_any& addr) const {
        return { zero::net::accept(this->sock, addr) };
    }

    template<typename T = S>
    T accept() const {
        return { ::accept(this->sock, NULL, NULL) };
    }

    bool get_local_addr(sockaddr_any& addr) const {
        return zero::net::getsockname(this->sock, addr) == 0;
    }

protected:
    inline size_t receive_impl(void* buffer, size_t recv_length) const {
        int recv_ret = ::recv(this->sock, (char*)buffer, recv_length, 0);
        if (expect(recv_ret <= 0, false)) {
            recv_ret = 0;
        }
        return recv_ret;
    }

    inline size_t receive_from_impl(void* buffer, size_t recv_length, sockaddr_any& addr) const {
        addr.length = sizeof(addr.storage);
        int recv_ret = ::recvfrom(this->sock, (char*)buffer, recv_length, 0, addr, (int*)&addr.length);
        if (expect(recv_ret <= 0, false)) {
            recv_ret = 0;
        }
        return recv_ret;
    }

public:

    size_t receive(void* buffer, size_t recv_length) const {
        return ((const S*)this)->receive_impl(buffer, recv_length);
    }

    size_t receive(void* buffer, size_t recv_length, sockaddr_any& addr) const {
        return ((const S*)this)->receive_from_impl(buffer, recv_length, addr);
    }

    template <typename T, size_t len>
    size_t receive_text(T(&buffer)[len]) const {
        size_t ret = this->receive(buffer, sizeof(buffer) - sizeof(T));
        buffer[ret] = (T)0;
        return ret;
    }

    template <typename T, size_t len>
    size_t receive_text(T(&buffer)[len], sockaddr_any& addr) const {
        size_t ret = this->receive(buffer, sizeof(buffer) - sizeof(T), addr);
        buffer[ret] = (T)0;
        return ret;
    }

    template <typename T>
    size_t receive(T& buffer) const {
        return this->receive(&buffer, sizeof(buffer));
    }

    template <typename T>
    size_t receive(T& buffer, sockaddr_any& addr) const {
        return this->receive(&buffer, sizeof(buffer), addr);
    }

    bool wait_for_incoming_data(size_t timeout) const {
        timeval time;
        time.tv_sec = timeout / 1000;
        time.tv_usec = timeout % 1000;

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(this->sock, &fds);

        return ::select(0, &fds, NULL, NULL, &time) > 0;
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

    inline size_t send_to_impl(const void* buffer, size_t send_length, const sockaddr_any& addr) const {
        size_t sent = 0;
        while (sent != send_length) {
            int send_ret = zero::net::sendto(this->sock, &((char*)buffer)[sent], send_length - sent, 0, addr);
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

    size_t send(const void* buffer, size_t send_length, const sockaddr_any& addr) const {
        return ((const S*)this)->send_to_impl(buffer, send_length, addr);
    }

    template <typename T, size_t len>
    inline bool send_text(const T(&str)[len]) const {
        return this->send(&str[0], sizeof(str) - sizeof(T));
    }

    template <typename T, size_t len>
    inline bool send_text(const T(&str)[len], const sockaddr_any& addr) const {
        return this->send(&str[0], sizeof(str) - sizeof(T), addr);
    }

    template <typename T, size_t len>
    inline bool send_text(const T(&str)[len], size_t length) const {
        return this->send(&str[0], length);
    }

    template <typename T, size_t len>
    inline bool send_text(const T(&str)[len], size_t length, const sockaddr_any& addr) const {
        return this->send(&str[0], length, addr);
    }

    template <typename T>
    inline bool send(const T& buffer) const {
        return this->send(&buffer, sizeof(T));
    }

    template <typename T>
    inline bool sent(const T& buffer, const sockaddr_any& addr) const {
        return this->send(&buffer, sizeof(T), addr)
    }

    bool get_receive_timeout(size_t& timeout) const {
        uint32_t timeout_dword = 0;
        bool ret = zero::net::getsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, timeout_dword) == 0;
        timeout = timeout_dword;
        return ret;
    }

    bool set_receive_timeout(size_t timeout) const {
        uint32_t timeout_dword = (uint32_t)timeout;
        return zero::net::setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, timeout_dword);
    }

    bool get_send_timeout(size_t& timeout) const {
        uint32_t timeout_dword = 0;
        bool ret = zero::net::getsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO, timeout_dword) == 0;
        timeout = timeout_dword;
        return ret;
    }

    bool set_send_timeout(size_t timeout) const {
        uint32_t timeout_dword = (uint32_t)timeout;
        return zero::net::setsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO, timeout_dword);
    }

    bool set_blocking_state(bool state) const {
        u_long blocking_state = !state;
        return ::ioctlsocket(this->sock, FIONBIO, &blocking_state);
    }
};

struct Socket : SocketBase<Socket> {

    template<SocketType socket_type>
    static inline Socket create() {
        Socket socket;
        socket.initialize<socket_type>();
        return socket;
    }
};

struct SocketTCP : Socket {
    template<typename T>
    T accept(sockaddr_any& addr) const = delete;

    template<typename T>
    T accept() const = delete;

    SocketTCP accept(sockaddr_any& addr) const {
        return this->Socket::accept<SocketTCP>(addr);
    }

    SocketTCP accept() const {
        return this->Socket::accept<SocketTCP>();
    }

    template<SocketType socket_type = TCP>
    inline bool initialize() {
        return this->Socket::initialize<socket_type>();
    }

    inline bool connect(const sockaddr_any& addr) const {
        return this->Socket::connect(addr);
    }

    template<SocketType socket_type = TCP>
    inline bool connect_ipv4(const char* ip, const char* port_str) const {
        return this->Socket::connect_ipv4<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect_ipv4(const wchar_t* ip, const wchar_t* port_str) const {
        return this->Socket::connect_ipv4<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect_ipv6(const char* ip, const char* port_str) const {
        return this->Socket::connect_ipv6<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect_ipv6(const wchar_t* ip, const wchar_t* port_str) const {
        return this->Socket::connect_ipv6<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect_ip(const char* ip, const char* port_str) const {
        return this->Socket::connect_ip<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect_ip(const wchar_t* ip, const wchar_t* port_str) const {
        return this->Socket::connect_ip<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect(const char* server_name, const char* port_str) const {
        return this->Socket::connect<socket_type>(server_name, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect(const wchar_t* server_name, const wchar_t* port_str) const {
        return this->Socket::connect<socket_type>(server_name, port_str);
    }

    template<SocketType socket_type = TCP>
    static inline SocketTCP create() {
        SocketTCP socket;
        socket.initialize<socket_type>();
        return socket;
    }
};

template<>
bool SocketTCP::initialize<UDP>() = delete;

template<>
inline bool SocketTCP::connect_ipv4<UDP>(const char* ip, const char* port_str) const = delete;

template<>
inline bool SocketTCP::connect_ipv4<UDP>(const wchar_t* ip, const wchar_t* port_str) const = delete;

template<>
inline bool SocketTCP::connect_ipv6<UDP>(const char* ip, const char* port_str) const = delete;

template<>
inline bool SocketTCP::connect_ipv6<UDP>(const wchar_t* ip, const wchar_t* port_str) const = delete;

template<>
inline bool SocketTCP::connect_ip<UDP>(const char* ip, const char* port_str) const = delete;

template<>
inline bool SocketTCP::connect_ip<UDP>(const wchar_t* ip, const wchar_t* port_str) const = delete;

template<>
inline bool SocketTCP::connect<UDP>(const char* server_name, const char* port_str) const = delete;

template<>
inline bool SocketTCP::connect<UDP>(const wchar_t* server_name, const wchar_t* port_str) const = delete;

template<>
inline SocketTCP SocketTCP::create<UDP>() = delete;

struct SocketUDP : Socket {
    template<SocketType socket_type = UDP>
    inline bool initialize() {
        return this->Socket::initialize<socket_type>();
    }

    inline bool connect(const sockaddr_any& addr) const {
        return this->Socket::connect(addr);
    }

    template<SocketType socket_type = UDP>
    inline bool connect_ipv4(const char* ip, const char* port_str) const {
        return this->Socket::connect_ipv4<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = UDP>
    inline bool connect_ipv4(const wchar_t* ip, const wchar_t* port_str) const {
        return this->Socket::connect_ipv4<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = UDP>
    inline bool connect_ipv6(const char* ip, const char* port_str) const {
        return this->Socket::connect_ipv6<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = UDP>
    inline bool connect_ipv6(const wchar_t* ip, const wchar_t* port_str) const {
        return this->Socket::connect_ipv6<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = UDP>
    inline bool connect_ip(const char* ip, const char* port_str) const {
        return this->Socket::connect_ip<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = UDP>
    inline bool connect_ip(const wchar_t* ip, const wchar_t* port_str) const {
        return this->Socket::connect_ip<socket_type>(ip, port_str);
    }

    template<SocketType socket_type = UDP>
    inline bool connect(const char* server_name, const char* port_str) const {
        return this->Socket::connect<socket_type>(server_name, port_str);
    }

    template<SocketType socket_type = UDP>
    inline bool connect(const wchar_t* server_name, const wchar_t* port_str) const {
        return this->Socket::connect<socket_type>(server_name, port_str);
    }

    template<SocketType socket_type = UDP>
    static inline SocketUDP create() {
        SocketUDP socket;
        socket.initialize<socket_type>();
        return socket;
    }
};

template<>
bool SocketUDP::initialize<TCP>() = delete;

template<>
inline bool SocketUDP::connect_ipv4<TCP>(const char* ip, const char* port_str) const = delete;

template<>
inline bool SocketUDP::connect_ipv4<TCP>(const wchar_t* ip, const wchar_t* port_str) const = delete;

template<>
inline bool SocketUDP::connect_ipv6<TCP>(const char* ip, const char* port_str) const = delete;

template<>
inline bool SocketUDP::connect_ipv6<TCP>(const wchar_t* ip, const wchar_t* port_str) const = delete;

template<>
inline bool SocketUDP::connect_ip<TCP>(const char* ip, const char* port_str) const = delete;

template<>
inline bool SocketUDP::connect_ip<TCP>(const wchar_t* ip, const wchar_t* port_str) const = delete;

template<>
inline bool SocketUDP::connect<TCP>(const char* server_name, const char* port_str) const = delete;

template<>
inline bool SocketUDP::connect<TCP>(const wchar_t* server_name, const wchar_t* port_str) const = delete;

template<>
inline SocketUDP SocketUDP::create<TCP>() = delete;

#if ENABLE_SSL
struct SocketSSL : SocketBase<SocketSSL> {

    template<SocketType socket_type = TCP>
    inline bool initialize() {
        return this->SocketBase<SocketSSL>::initialize<socket_type>();
    }

    inline bool connect(const sockaddr_any& addr) const {
        return this->Socket::connect(addr);
    }

    template<SocketType socket_type = TCP>
    inline bool connect_ipv4(const char* ip, const char* port_str) const = delete;

    template<SocketType socket_type = TCP>
    inline bool connect_ipv4(const wchar_t* ip, const wchar_t* port_str) const = delete;

    template<SocketType socket_type = TCP>
    inline bool connect_ipv6(const char* ip, const char* port_str) const = delete;

    template<SocketType socket_type = TCP>
    inline bool connect_ipv6(const wchar_t* ip, const wchar_t* port_str) const = delete;

    template<SocketType socket_type = TCP>
    inline bool connect_ip(const char* ip, const char* port_str) const = delete;

    template<SocketType socket_type = TCP>
    inline bool connect_ip(const wchar_t* ip, const wchar_t* port_str) const = delete;

    template<SocketType socket_type = TCP>
    inline bool connect(const char* server_name, const char* port_str) const {
        return this->SocketBase<SocketSSL>::connect<socket_type>(server_name, port_str);
    }

    template<SocketType socket_type = TCP>
    inline bool connect(const wchar_t* server_name, const wchar_t* port_str) const {
        return this->SocketBase<SocketSSL>::connect<socket_type>(server_name, port_str);
    }

    template<SocketType socket_type = TCP>
    static inline SocketSSL create() {
        SocketSSL socket;
        socket.initialize<socket_type>();
        return socket;
    }

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

    inline bool connect_secondary(const char* server_name) {
        return this->tls_in_use = this->initialize_tls(server_name);
    }

    inline bool connect_secondary(const wchar_t* server_name) {
        return this->tls_in_use = this->initialize_tls(server_name);
    }

    inline void close_secondary() {
        if (this->tls_in_use) {
            ::DeleteSecurityContext(&this->tls_context);
            ::FreeCredentialsHandle(&this->tls_handle);
        }
    }
    
    // Adapted from this: https://github.com/odzhan/shells/blob/master/s6/tls.c
    template<typename T>
    bool initialize_tls(const T* server_name) {

        SECURITY_STATUS status;
        if constexpr (std::is_same_v<T, wchar_t>) {
            status = ::AcquireCredentialsHandleW(NULL, (LPWSTR)UNISP_NAME_W, SECPKG_CRED_OUTBOUND, NULL, (void*)&tls_cred, NULL, NULL, &this->tls_handle, NULL);
        } else {
            status = ::AcquireCredentialsHandleA(NULL, (LPSTR)UNISP_NAME_A, SECPKG_CRED_OUTBOUND, NULL, (void*)&tls_cred, NULL, NULL, &this->tls_handle, NULL);
        }

        if (expect(status == SEC_E_OK, true)) {

            SecBuffer outbuffers[1] = { 0 };
            outbuffers[0].BufferType = SECBUFFER_TOKEN;
            outbuffers[0].pvBuffer = NULL;
            outbuffers[0].cbBuffer = 0;

            SecBufferDesc outdesc = { SECBUFFER_VERSION, ARRAYSIZE(outbuffers), outbuffers };

            DWORD flags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM | ISC_REQ_MANUAL_CRED_VALIDATION;

            SECURITY_STATUS sec;
            if constexpr (std::is_same_v<T, wchar_t>) {
                sec = ::InitializeSecurityContextW(&this->tls_handle,
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
            } else {
                sec = ::InitializeSecurityContextA(&this->tls_handle,
                                                   NULL,
                                                   (SEC_CHAR*)server_name,
                                                   flags,
                                                   0,
                                                   0,
                                                   NULL,
                                                   0,
                                                   &this->tls_context,
                                                   &outdesc,
                                                   &flags,
                                                   NULL);
            }

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
                    ::FreeContextBuffer(outbuffers[0].pvBuffer);
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

                    if constexpr (std::is_same_v<T, wchar_t>) {
                        sec = ::InitializeSecurityContextW(&this->tls_handle,
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
                    } else {
                        sec = ::InitializeSecurityContextA(&this->tls_handle,
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
                    }

                    if (sec == SEC_E_OK || sec == SEC_I_CONTINUE_NEEDED) {
                        char* buffer = (char*)outbuffers[0].pvBuffer;
                        size_t size = outbuffers[0].cbBuffer;
                        if (buffer) {
                            while (size != 0) {
                                auto send_ret = ::send(this->sock, buffer, size, 0);
                                if (expect(send_ret <= 0, false)) {
                                    // failed with disconnect or error
                                    ::FreeContextBuffer(outbuffers[0].pvBuffer);
                                    goto tls_fail;
                                }
                                size -= send_ret;
                                buffer += send_ret;
                            }
                            ::FreeContextBuffer(outbuffers[0].pvBuffer);
                        }
                    }
                    if (sec == SEC_E_INCOMPLETE_MESSAGE) {
                        continue;
                    }
                    if (sec == SEC_E_OK) {
                        if constexpr (std::is_same_v<T, wchar_t>) {
                            ::QueryContextAttributesW(&this->tls_context, SECPKG_ATTR_STREAM_SIZES, &this->tls_sizes);
                        } else {
                            ::QueryContextAttributesA(&this->tls_context, SECPKG_ATTR_STREAM_SIZES, &this->tls_sizes);
                        }
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
                ::DeleteSecurityContext(&this->tls_context);
            }
            ::FreeCredentialsHandle(&this->tls_handle);
        }
        ::closesocket(this->sock);
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

                SECURITY_STATUS sec = ::DecryptMessage(&this->tls_context, &desc, 0, NULL);
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
                if (expect(::EncryptMessage(&this->tls_context, 0, &desc, 0) != SEC_E_OK, false)) {
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

template<>
bool SocketSSL::initialize<UDP>() = delete;

template<>
inline bool SocketSSL::connect<UDP>(const char* server_name, const char* port_str) const = delete;

template<>
inline bool SocketSSL::connect<UDP>(const wchar_t* server_name, const wchar_t* port_str) const = delete;

template<>
inline SocketSSL SocketSSL::create<UDP>() = delete;
#endif

static inline std::atomic<uint8_t> winsock_initialized = { false };

static inline bool enable_winsock() {
    WSADATA idgaf_about_winsock_version;
    if (expect(!winsock_initialized.exchange(true), false)) {
        winsock_initialized = ::WSAStartup(WINSOCK_VERSION, &idgaf_about_winsock_version) == 0;
    }
    return winsock_initialized;
}

static inline void disable_winsock() {
    if (expect(winsock_initialized.exchange(false), true)) {
        ::WSACleanup();
    }
}



}

}

#endif