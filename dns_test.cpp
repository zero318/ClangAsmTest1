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
#include <string.h>
#include <stdio.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#include <Windows.h>

#include <WinDNS.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Dnsapi")

#include "zero/util.h"

#define halt_and_catch_fire() __asm__("int3")

wchar_t addr_buff[48];

/*
static inline constexpr ADDRINFOW addr_hint = {
    .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
    .ai_family = AF_INET,
    .ai_socktype = SOCK_DGRAM,
    .ai_protocol = IPPROTO_UDP
};
*/

static inline constexpr ADDRINFOW addr_hint = {
    .ai_flags = AI_NUMERICHOST | AI_NUMERICSERV,
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = IPPROTO_TCP
};

void print_ipv4(IP4_ADDRESS addr) {
    union {
        uint32_t temp;
        uint8_t as_bytes[4];
    };
    temp = addr;
    printf(
        "v4: %u.%u.%u.%u\n"
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
    } else {
        printf("\n");
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

bool dns_connect(const wchar_t* server_name, const wchar_t* port_str) {
    WSADATA idgaf_about_winsock_version;
    WSAStartup(WINSOCK_VERSION, &idgaf_about_winsock_version);

    printf("Dns:\n");

    // GetAddrInfoW just crashes for some reason when trying to
    // resolve a URL on Windows 7, so the DNS resolution needs
    // to be done manually.
    // THIS IS DUMB

    PDNS_RECORDW records;
    if (DnsQuery_W(server_name, DNS_TYPE_AAAA, DNS_QUERY_DUAL_ADDR, NULL, (PDNS_RECORD*)&records, NULL) == 0) {
    //if (DnsQuery_W(server_name, DNS_TYPE_A, 0, NULL, (PDNS_RECORD*)&records, NULL) == 0) {
        PDNS_RECORDW cur_record = records;
        do {
            void* ipv4;
            switch (cur_record->wType) {
                [[unlikely]] default:
                    halt_and_catch_fire();
                case DNS_TYPE_CNAME:
                    break;
                case DNS_TYPE_AAAA:
                    print_ipv6(cur_record->Data.AAAA.Ip6Address);
                    if (
                        !(cur_record->Data.AAAA.Ip6Address.IP6Dword[0] == 0 &&
                          cur_record->Data.AAAA.Ip6Address.IP6Dword[1] == 0 &&
                          cur_record->Data.AAAA.Ip6Address.IP6Dword[2] == 0xFFFF0000)
                    ) {
                        InetNtopW(AF_INET6, &cur_record->Data.AAAA.Ip6Address, addr_buff, countof(addr_buff));
                        //goto found_addr;
                        break;
                    } else if (1) {
                        ipv4 = &cur_record->Data.AAAA.Ip6Address.IP6Dword[3];
                    } else {
                case DNS_TYPE_A:
                        print_ipv4(cur_record->Data.A.IpAddress);
                        ipv4 = &cur_record->Data.A.IpAddress;
                    }
                    InetNtopW(AF_INET, ipv4, addr_buff, countof(addr_buff));
                    //goto found_addr;
            }
            wprintf(L"%s\n", addr_buff);
        } while ((cur_record = cur_record->pNext));
        DnsFree(records, DnsFreeRecordList);
    }
    //return false;

found_addr:
    //DnsFree(records, DnsFreeRecordList);

    /*
    PADDRINFOW addrs;
    // GetAddrInfoW sucks
    if (!GetAddrInfoW(addr_buff, port_str, &addr_hint, &addrs)) {
        do {
            SOCKET sock = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
            if (sock != INVALID_SOCKET) {
                if (::connect(sock, addrs->ai_addr, (int)addrs->ai_addrlen) != SOCKET_ERROR) {
                    FreeAddrInfoW(addrs);
                    //this->initialize(sock, server_name);
                    return true;
                }
                
                //else {
                    //int error = WSAGetLastError();
                    //printf("%d\n", error);
                //}
                
                closesocket(sock);
            }
        } while ((addrs = addrs->ai_next));
        FreeAddrInfoW(addrs);
    }
    */
    //return false;
    printf("Sockaddrs:\n");

    PADDRINFOW addrs;
    // GetAddrInfoW sucks
    if (!GetAddrInfoW(addr_buff, port_str, &addr_hint, &addrs)) {
        PADDRINFOW addrs_iter = addrs;
        do {
            //switch (addrs->ai_family) {
                //case AF_INET:
                //case AF_INET6:
                    //new (out) SavedSockaddr(addrs->ai_addr, addrs->ai_addrlen);
                    print_sockaddr(addrs_iter->ai_addr);
                    //FreeAddrInfoW(addrs);
                    //return true;
            //}
        } while ((addrs_iter = addrs_iter->ai_next));
        FreeAddrInfoW(addrs);
    } else {
        printf("crap");
    }
    return false;
}