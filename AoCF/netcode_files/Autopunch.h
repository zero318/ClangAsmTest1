#pragma once
#ifndef AUTOPUNCH_H
#define AUTOPUNCH_H 1

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#include <ws2tcpip.h>
#include "log.h"

int WSAAPI my_WSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, sockaddr* lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int WSAAPI my_WSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const sockaddr* lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int WSAAPI my_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int WSAAPI my_bind(SOCKET s, const sockaddr* name, int namelen);
int WINAPI my_closesocket(SOCKET s);

void autopunch_init();
void autopunch_cleanup();
#endif