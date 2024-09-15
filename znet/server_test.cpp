#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include <utility>

#include <conio.h>

#include <windows.h>

#include "znet.h"
#include "zthread.h"

using namespace zero::net;
using namespace zero::thread;

#define size_t_neg_one (~(size_t)0)

#include "common.h"

template<size_t N>
size_t getsn_with_esc(char(&buf)[N]) {
	if (_kbhit()) {
		switch (char c = _getch()) {
			default:
				buf[0] = c;
				putchar(c);
				return 1 + getsn_newline((char(&)[N - 1])buf[1]);
				//_ungetch(c);
				//fputc(c, stdin);
				//ungetc(c, stdin);
				//return getsn(buf);
			case VK_ESCAPE:
				return size_t_neg_one;
			case VK_RETURN:
				break;
		}
	}
	return 0;
}

enum ArgIndex {
	PROGRAM_NAME_ARG = 0,
	SOCKET_TYPE_ARG = 1,
	PORT_ARG = 2,
	MIN_REQUIRED_ARGS
};

template <size_t N1, size_t N2>
void main_net_func(const SocketTCP& socket, char(&buf1)[N1], char(&buf2)[N2]) {

    socket.set_receive_timeout(10);

    zjthread send_thread([&](const std::atomic<uint8_t>& stop) {
        while (!stop) {
            if (size_t message_length = getsn_with_esc(buf1)) {
                if (message_length == size_t_neg_one) {
                    printf("Disconnecting, returning to menu...\n");
                    Sleep(3000);
                    break;
                }
#if !EXTRA_DEBUG_TEXT
                socket.send_text(buf1, message_length + 1);
#else
                printf("Sending... %s", buf1);
                if (!socket.send_text(buf1, message_length)) {
                    printf("failed!");
                }
                putch('\n');
#endif
            } else {
                Sleep(10);
            }
        }
    });

    zjthread recv_thread([&](const std::atomic<uint8_t>& stop) {
        while (!stop) {
            if (
                socket.receive_text(buf2)
            ) {
                //if (!strcmp(buf2, LOGOUT_TEXT)) {
                    //printf("Peer disconnected, returning to menu...\n");
                    //Sleep(3000);
                    //break;
                //}
                puts(buf2);
            }
        }
    });

    while (
        !send_thread.stopped() &&
        !recv_thread.stopped()
    ) {
        Sleep(100);
    }

    send_thread.stop();
    recv_thread.stop();
}

static constexpr size_t BUFFER_SIZE = 4096;

int main(int argc, char* argv[]) {
	if (argc < MIN_REQUIRED_ARGS) {
		error_exit("znet_server_test.exe <type> <port>");
	}

	uint16_t port;
	if (!strbuf_to_uint16(argv[PORT_ARG], port)) {
		error_exit("Invalid port");
	}

	if (!enable_winsock()) {
		error_exit("Count not initialize winsock");
	}

	if (!strcmp(argv[SOCKET_TYPE_ARG], "TCP")) {
		if (SocketTCP socket = SocketTCP::create()) {
			if (socket.bind(port)) {

				printf("Hosting TCP on port %hu...\n", port);

				socket.set_blocking_state(false);

				socket.listen(10);

				zjthread stupid_keyboard_thread([](const std::atomic<uint8_t>& stop) {
					while (!stop) {
						if (_kbhit()) {
							char c = _getch();
							if (c == VK_ESCAPE) {
								break;
							}
							_ungetch(c);
						}
						Sleep(5);
					}
				});

				do {
					if (SocketTCP response_socket = socket.accept()) {
						printf("Accepted connection!\n");
						std::thread([response_socket]() mutable {

							response_socket.set_blocking_state(true);

							/*
							size_t message_count = 0;

							char text[BUFFER_SIZE];
							while (size_t receive_length = response_socket.receive_text(text)) {
								//printf("Received text: \"%s\"\n", text);
								fwrite(text, 1, receive_length, stdout);

								//size_t send_length = snprintf(text, BUFFER_SIZE, "Messages received: %zu", ++message_count);

								//response_socket.send_text(text, send_length);
							}
							response_socket.close();
							*/
							char text[BUFFER_SIZE];
							char text2[BUFFER_SIZE];
							main_net_func(response_socket, text, text2);
							response_socket.close();
						}).detach();
					}
					Sleep(10);
				} while (!stupid_keyboard_thread.stopped());
			}
			socket.close();
		}
	}
	else if (!strcmp(argv[SOCKET_TYPE_ARG], "UDP")) {
		if (SocketUDP socket = SocketUDP::create()) {
			if (socket.bind(port)) {

				printf("Hosting UDP on port %hu...\n", port);

				socket.set_blocking_state(false);

				zjthread stupid_keyboard_thread([](const std::atomic<uint8_t>& stop) {
					wait_for_keyboard();
				});

				size_t message_count = 0;

				do {
					char text[BUFFER_SIZE];

					sockaddr_any peer_addr;
					if (size_t receive_length = socket.receive_text(text, peer_addr)) {

						printf("Received text: \"%s\"\n", text);

						//size_t send_length = snprintf(text, BUFFER_SIZE, "Messages received: %zu", ++message_count);

						//socket.send_text(text, send_length, peer_addr);

					}

					Sleep(10);
				} while (!stupid_keyboard_thread.stopped());
			}
			socket.close();
		}
	}

	disable_winsock();

	return EXIT_SUCCESS;
}