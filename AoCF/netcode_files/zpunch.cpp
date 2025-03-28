#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"

#if __INTELLISENSE__
#define _HAS_CXX17 1
#define _HAS_CXX20 0
#endif

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

#include <windows.h>

#include <WinDNS.h>

#include "util.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Dnsapi")

#define MACRO_EVAL(...) __VA_ARGS__
#define MACRO_VOID(...)

#if __INTELLISENSE__
// Attempt to make intellisense shut up about C++ features it doesn't understand...
#define INTELLISENSE_TYPENAME typename
#define requires(...) MACRO_EVAL(MACRO_VOID(__VA_ARGS__))
#define concept static inline constexpr bool
#define static_lambda
#define constexpr_lambda
#define consteval_lambda
#else
#define INTELLISENSE_TYPENAME
#define requires(...) requires (__VA_ARGS__)
#define static_lambda static
#define constexpr_lambda constexpr
#define consteval_lambda consteval
#endif

// Configuration values

static constexpr const wchar_t relay_host[] = L"delthas.fr";
static constexpr const wchar_t relay_port[] = L"14763";

static constexpr size_t RELAY_DELAY = 500;

// Networking utility functions

struct AnySockaddr {
    sockaddr_storage storage;
    size_t length;

    AnySockaddr() = default;

    AnySockaddr(sockaddr* addr, size_t length) : length(length) {
        memcpy(&this->storage, addr, length);
    }

    operator sockaddr*() const {
        return (sockaddr*)&this->storage;
    }
};

IP6_ADDRESS map_ip_to_ipv6(const sockaddr* addr) {
    switch (addr->sa_family) {
        case AF_INET:
            return (IP6_ADDRESS) {
                .IP6Dword = {
                    0x00000000, 0x00000000, 0xFFFF0000, *(const IP4_ADDRESS*)&((const sockaddr_in*)addr)->sin_addr
                }
            };
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

bool ips_match(const sockaddr* lhs,  const sockaddr* rhs) {
    IP6_ADDRESS lhs_addr = map_ip_to_ipv6(lhs);
    IP6_ADDRESS rhs_addr = map_ip_to_ipv6(rhs);
    return !memcmp(&lhs_addr, &rhs_addr, sizeof(IP6_ADDRESS));
}

template <typename T> requires(!std::is_pointer_v<T>)
int sendto(SOCKET s, const T& data, int flags, const AnySockaddr& to) {
    return sendto(s, (const char*)&data, sizeof(T), flags, to, to.length);
}

template <typename T>
int sendto(SOCKET s, const T* data, int flags, const AnySockaddr& to) {
    return sendto(s, (const char*)data, sizeof(T), flags, to, to.length);
}

int getsockname(SOCKET s, AnySockaddr& addr) {
    addr.length = sizeof(addr.storage);
    return getsockname(s, addr, (int*)&addr.length);
}

static AnySockaddr relay_addr = {};

#include "znet.h"

// Synchronization utilities

struct SpinLock {
    std::atomic<bool> flag;

    inline constexpr SpinLock() : flag(false) {}
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    inline void lock() {
        while (this->flag.exchange(true, std::memory_order_acquire));
        std::atomic_thread_fence(std::memory_order_acquire);
    }
    inline bool try_lock() {
        bool ret = this->flag.exchange(true, std::memory_order_acquire);
        std::atomic_thread_fence(std::memory_order_acquire);
        return ret ^ true;
    }
    inline void unlock() {
        std::atomic_thread_fence(std::memory_order_release);
        this->flag.store(false, std::memory_order_release);
    }
};

// Thread type

#include "zthread.h"

// Socket types and map management

struct MappingData {

};

struct SocketData {
    SpinLock modification_mutex;
    std::atomic<bool> delete_flag;
    uint16_t local_port;
    std::vector<MappingData> mappings;
};

static std::unordered_map<SOCKET, SocketData*> sockets;
static std::shared_mutex map_mutex;

using shared_mutex_lock = std::shared_lock<std::shared_mutex>;
using unique_mutex_lock = std::unique_lock<std::shared_mutex>;

template <typename L>
static inline void shared_socket_data(SOCKET socket, const L& lambda) {
    shared_mutex_lock lock(map_mutex);
    auto iter = sockets.find(socket);
    if (iter != sockets.end()) {
        SocketData* socket_data = iter->second;
        lambda(socket_data);
    }
}

template <typename L>
static inline void exclusive_socket_data(SOCKET socket, const L& lambda) {
    shared_mutex_lock lock(map_mutex);
    auto iter = sockets.find(socket);
    if (iter != sockets.end()) {
        SocketData* socket_data = iter->second;
        std::lock_guard lock(socket_data->modification_mutex);
        lambda(socket_data);
    }
}

static inline void insert_socket_data(SOCKET socket) {
    unique_mutex_lock lock(map_mutex);
    auto [iter, success] = sockets.try_emplace(socket, NULL);
    if (success) {
        iter->second = new SocketData();
    }
}

static inline void remove_socket_data(SOCKET socket) {
    shared_mutex_lock lock(map_mutex);
    auto iter = sockets.find(socket);
    if (iter != sockets.end()) {
        iter->second->delete_flag = true;
    }
}

// Patch functions

// Adds any new sockets to the tracked socket list
SOCKET WSAAPI my_WSASocketW(
    int af, int type, int protocol,
    LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags
) {
    SOCKET socket = WSASocketW(af, type, protocol, lpProtocolInfo, g, dwFlags);
    if (
        socket != INVALID_SOCKET &&
        af == AF_INET
    ) {
        insert_socket_data(socket);
    }
    return socket;
}

// Removes sockets from the tracked socket list
int WSAAPI my_closesocket(
    SOCKET s
) {
    remove_socket_data(s);
    return closesocket(s);
}

int WSAAPI my_bind(
    SOCKET s,
    const sockaddr* name,
    int namelen
) {
    int r = bind(s, name, namelen);
    if (!r) {
        exclusive_socket_data(s,
            [=](SocketData* socket_data) {
                AnySockaddr local_addr;
                if (!getsockname(s, local_addr)) {
                    socket_data->local_port = get_port(local_addr);
                }
            }
        );
    }
    return r;
}

int WSAAPI my_WSASendTo(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags, // 0
    const sockaddr* lpTo,
    int iTolen,
    LPWSAOVERLAPPED lpOverlapped, // NULL
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine // NULL
) {
    SocketData* socket_data = get_socket_data(s);
    if (!socket_data) {
        return WSASendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iTolen, lpOverlapped, lpCompletionRoutine);
    }
    
}

// 

static std::atomic<uint8_t> relay_thread_stop = { false };

static Socket<false> relay_socket;

static zthread relay_thread;

void relay() {
    shared_mutex_lock lock(map_mutex);
    for (auto [socket, socket_data] : sockets) {
        if (socket_data->delete_flag) {
            delete socket_data;
            continue;
        }

        sendto(socket, socket_data->port, 0, relay_addr);


    }
}

void autopunch_init() {

    if (expect(enable_winsock(), true)) {
        if (expect(relay_socket.connect_udp(relay_host, relay_port), true)) {
            relay_thread_stop = false;
            new (&relay_thread) zthread(
                []() {
                    relay_thread_stop = true;
                },
                []() {
                    do {
                        relay();
                        Sleep(RELAY_DELAY);
                    } while (!relay_thread_stop);
                    relay_socket.close();
                }
            );
        }
    }
}

void autopunch_cleanup() {
    relay_thread.~zthread();
}