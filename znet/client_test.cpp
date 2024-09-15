#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include <conio.h>

#include <windows.h>

#include "znet.h"
#include "zthread.h"

using namespace zero::net;
using namespace zero::thread;

#include "common.h"

enum ArgIndex {
	PROGRAM_NAME_ARG = 0,
	SOCKET_TYPE_ARG = 1,
	IP_ARG = 2,
	PORT_ARG = 3,
	MIN_REQUIRED_ARGS
};

int main(int argc, char* argv[]) {

#if NDEBUG
#define socket_type_arg argv[SOCKET_TYPE_ARG]
#define ip_arg argv[IP_ARG]
#define port_arg argv[PORT_ARG]

	if (argc < MIN_REQUIRED_ARGS) {
		error_exit("znet_client_test.exe <ip> <port>");
	}

#else
#define socket_type_arg "TCP"
#define ip_arg "waluigistacostand.ddns.net"
#define port_arg "42069"
#endif

	uint16_t port;
	if (!strbuf_to_uint16(port_arg, port)) {
		error_exit("Invalid port");
	}

	if (!enable_winsock()) {
		error_exit("Count not initialize winsock");
	}

	if (!strcmp(socket_type_arg, "TCP")) {
		if (SocketTCP socket = SocketTCP::create()) {
			if (socket.connect(ip_arg, port_arg)) {

				printf("Transmitting TCP on port %hu...\n", port);

				char message[512];
				for (;;) {
					size_t message_length = getsn(message);
					if (!strcmp(message, "stop")) {
						break;
					}
					socket.send(message, message_length);

					socket.receive_text(message);
					printf("Server response: %s\n", message);
				}

			} else {
				printf("Connection failed!\n");
			}
			socket.close();
		}
	}
	else if (!strcmp(socket_type_arg, "UDP")) {
		if (SocketUDP socket = SocketUDP::create()) {
			if (socket.connect(ip_arg, port_arg)) {

				printf("Transmitting UDP on port %hu...\n", port);

				char message[512];
				for (;;) {
					size_t message_length = getsn(message);
					if (!strcmp(message, "stop")) {
						break;
					}
					socket.send(message, message_length);

					socket.receive_text(message);
					printf("Server response: %s\n", message);
				}

			} else {
				printf("Connection failed!\n");
			}
			socket.close();
		}
	}
	else if (!strcmp(socket_type_arg, "ICMP")) {
		if (SocketICMP socket = SocketICMP::create()) {
			//sizeof(IPHeader)
		}
	}

	disable_winsock();

	return EXIT_SUCCESS;
}