#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include <limits>
template <typename T>
static inline constexpr auto decimal_digits_bound() {
    return std::numeric_limits<T>::digits10 + 1;
}

#include <utility>
#include <atomic>

#include <conio.h>

#include <windows.h>
#include <Mmsystem.h>

#include "znet.h"
#include "zthread.h"

#pragma comment (lib, "winmm.lib")

using namespace zero::net;
using namespace zero::thread;

#include "common.h"

#define size_t_neg_one (~(size_t)0)


void clear_screen(char fill = ' ') {
    // why are you like this windows
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    COORD tl = { 0,0 };
    FillConsoleOutputCharacterA(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

// TODO: The first character can't be backspaced and only the first character detects ESC
template<size_t N>
size_t getsn_with_esc(char(&buf)[N]) {
    if (_kbhit()) {
        switch (char c = _getch()) {
            default:
                buf[0] = c;
                putchar(c);
                return 1 + getsn((char(&)[N - 1])buf[1]);
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

// TODO: Make this respond to ESC somehow
template<size_t N>
uint16_t get_port_from_input(char(&port_str_buffer)[N]) {


    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(console, &s);

    COORD original_pos = s.dwCursorPosition;

    uint16_t port;

    printf("Select a port to use: ");

    for (;;) {
        getsn(port_str_buffer);

        if (strbuf_to_uint16(port_str_buffer, port)) {
            break;
        }

        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacterA(console, ' ', cells, original_pos, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, original_pos, &written);
        SetConsoleCursorPosition(console, original_pos);

        printf("Select a VALID port to use: ");
    }

    return port;
}

// TODO: Add more memes and make the RNG better
void run_meme(size_t index = size_t_neg_one) {

    typedef void meme_func();
    static constexpr meme_func *const meme_table[] = {
        [](){
            system("explorer \"https://youtu.be/dQw4w9WgXcQ\"");
        },
        [](){
            system("explorer \"https://youtu.be/TZNQEYto97c\"");
        },
        [](){
            static constexpr uint8_t don_squirrel[] = {
#include "don_squirrel.h"
            };
            sndPlaySound((const char*)don_squirrel, SND_ASYNC | SND_MEMORY);
        },
        []() {
            static constexpr uint8_t metal_pipe[] = {
#include "obnoxiously_loud_metal_pipe.h"
            };
            sndPlaySound((const char*)metal_pipe, SND_ASYNC | SND_MEMORY);
        }
    };

    if (index >= countof(meme_table)) {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);
        //index = current_time.QuadPart % countof(meme_table);
        srand(current_time.QuadPart);
        index = rand() % countof(meme_table);
    }
    
    meme_table[index]();
}

void meme_menu() {

    for (;;) {
        clear_screen();

        printf(
            "YOU FOUND THE SECRET MEME MENU\n"
            "0. rickroll\n"
            "1. base boosted coconut mall\n"
            "2. Don squirrel\n"
            "3. Obnoxiously loud metal pipe\n"
            "4. Return to main menu\n"
        );

        switch (char c = wait_for_keyboard()) {
            case '4': case VK_ESCAPE:
                return;
            case '0':
            case '1':
            case '2':
            case '3':
                run_meme(c - '0');
        }
    }

}

static constexpr uint8_t konami_code[] = {
    224, 72, 224, 72,
    224, 80, 224, 80,
    224, 75, 224, 77,
    224, 75, 224, 77,
    97, 98, 13
};

static const uint8_t* current_konami_code = konami_code;

inline void konami_code_reset() {
    current_konami_code = konami_code;
}

bool konami_code_check(uint8_t c) {
    if (*current_konami_code == c) {
        if (++current_konami_code == &konami_code[countof(konami_code)]) {
            konami_code_reset();
            return true;
        }
    } else {
        konami_code_reset();
    }
    return false;
}

#define EXTRA_DEBUG_TEXT 0

static inline constexpr char LOGIN_TEXT[] = "yo whaddup";
static inline constexpr char LOGIN_ACCEPT[] = "you are winner";
static inline constexpr char LOGIN_DENY[] = "WHAT ARE YOU DOING IN MY SWAMP";
static inline constexpr char LOGOUT_TEXT[] = "\4";

template <size_t N1, size_t N2>
void main_net_func(const SocketUDP& socket, char(&buf1)[N1], char(&buf2)[N2]) {

    socket.set_receive_timeout(10);

    zjthread send_thread([&](const std::atomic<uint8_t>& stop) {
        while (!stop) {
            if (size_t message_length = getsn_with_esc(buf1)) {
                if (message_length == size_t_neg_one) {
                    socket.send_text(LOGOUT_TEXT);
                    printf("Disconnecting, returning to menu...\n");
                    Sleep(3000);
                    break;
                }
#if !EXTRA_DEBUG_TEXT
                socket.send_text(buf1, message_length);
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
                //socket.wait_for_incoming_data(60000) &&
                socket.receive_text(buf2)
            ) {
                if (!strcmp(buf2, LOGOUT_TEXT)) {
                    printf("Peer disconnected, returning to menu...\n");
                    Sleep(3000);
                    break;
                }
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

int main(int argc, char* argv[]) {

    for (;;) {
        clear_screen();

        printf(
            "ZNET BASIC P2P TEST PROGRAM:\n"
            "1. Connect to peer\n"
            "2. Wait for connection\n"
            "3. Quit\n"
            "4. ???\n"
        );

        
        uint16_t port;
        char text_buffer[512];
        char text_buffer2[512];
        sockaddr_any peer_addr;

        switch (char c = wait_for_keyboard()) {
            default:
                if (konami_code_check(c)) {
                    meme_menu();
                }
                break;
            case '1': {
                konami_code_reset();

                printf("Enter an address to connect to: ");

                getsn(text_buffer);

                port = get_port_from_input(text_buffer2);

                enable_winsock();


                if (SocketUDP socket = SocketUDP::create()) {
                    if (
                        socket.connect(text_buffer, text_buffer2) ||
                        socket.connect_ip(text_buffer, text_buffer2)
                    ) {
                        socket.set_receive_timeout(60000);

                        socket.send_text(LOGIN_TEXT);

                        if (
                            socket.receive_text(text_buffer)
                        ) {
                            if (!strcmp(text_buffer, LOGIN_ACCEPT)) {
                                printf("Connection accepted!\n");

                                main_net_func(socket, text_buffer, text_buffer2);
                            }
                            else if (!strcmp(text_buffer, LOGIN_DENY)) {
                                printf("Connection attempt denied by peer\n");
                                Sleep(3000);
                            }
                        } else {
                            printf("Connection attempt timed out\n");
                            Sleep(3000);
                        }
                    }
                    socket.close();
                }
                break;
            }
            case '2': {
                konami_code_reset();

                port = get_port_from_input(text_buffer2);

                enable_winsock();

                if (SocketUDP socket = SocketUDP::create()) {
                    if (socket.bind(port)) {

                        socket.set_blocking_state(false);

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

                        printf("Waiting for connection attempts...\n");

                        do {
                            if (
                                socket.receive_text(text_buffer, peer_addr)
                            ) {
                                if (!strcmp(text_buffer, LOGIN_TEXT)) {
                                    printf("Connection requested from ");
                                    print_sockaddr(peer_addr);
                                    printf(
                                        "\n"
                                        "Accept or deny?\n"
                                        "1. Accept\n"
                                        "2. Deny\n"
                                    );
                                    for (;;) {
                                        switch (char c = wait_for_keyboard()) {
                                            case '2': case VK_ESCAPE:
                                                socket.send_text(LOGIN_DENY, peer_addr);
                                                printf(
                                                    "Denied connection\n"
                                                );
                                                goto stop_connection;
                                            case '1':
                                                socket.send_text(LOGIN_ACCEPT, peer_addr);
                                                printf(
                                                    "Accepted connection\n"
                                                );
                                                stupid_keyboard_thread.stop();

                                                if (socket.connect(peer_addr)) {

                                                    socket.set_blocking_state(true);

                                                    main_net_func(socket, text_buffer, text_buffer2);
                                                } else {
                                                    printf("Connection failed\n");
                                                }
                                                goto stop_connection;
                                        }
                                    }
                                stop_connection:;
                                }
                            }
                            Sleep(10);
                        } while (!stupid_keyboard_thread.stopped());
                    }
                    socket.close();
                }

                break;
            }
            case '3': case VK_ESCAPE:
                goto quit;
            case '4':
                konami_code_reset();
                run_meme();
                break;
        }
    }
    
quit:

    disable_winsock();

	return EXIT_SUCCESS;
}