
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <Windows.h>

#include "common.h"
#include "znet.h"

using namespace zero::net;

struct sockaddr_un {
	sa_family_t sun_family;
	char sun_path[108];
};

int main(int argc, char* argv[]) {
	if (argc < 3) {
		error_exit("unix_socket_test.exe <path> <data>");
	}

	size_t path_len = strlen(argv[1]);
	if (path_len > 107) {
		error_exit("Socket path is too long!");
	}
	size_t data_len = strlen(argv[2]);
	if (data_len > 127) {
		error_exit("Data is too long for test receiver!");
	}

	if (!enable_winsock()) {
		error_exit("Count not initialize winsock");
	}

	SOCKET sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		error_exit("Failed to create unix socket");
	}

	sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, argv[1], path_len + 1);

	if (!connect(sock, (const sockaddr*)&addr, sizeof(sockaddr_un))) {
		if (::send(sock, argv[2], data_len + 1, 0)) {
			printf("Sent successfully!");
		} else {
			printf("Send failed");
		}
	} else {
		printf("Connection failed");
	}

	closesocket(sock);

	disable_winsock();

	return EXIT_SUCCESS;
}

/*
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fputs("unix_socket_host <path>", stderr);
		exit(EXIT_FAILURE);
	}

	size_t path_len = strlen(argv[1]);
	if (path_len > 107) {
		fputs("Socket path is too long!", stderr);
		exit(EXIT_FAILURE);
	}

	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1) {
		fputs("Failed to create unix socket", stderr);
		exit(EXIT_FAILURE);
	}

	sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, argv[1], path_len + 1);

	if (!connect(sock, (const sockaddr*)&addr, sizeof(sockaddr_un))) {
		char buffer[128];
		if (recv(sock, buffer, sizeof(buffer), 0) > 0) {
			printf("Received \"%s\"", buffer);
		} else {
			printf("Recv failed");
		}
	} else {
		printf("Connection failed");
	}

	close(sock);

	return EXIT_SUCCESS;
}
*/