#include "youtube_chat.hpp"

#include <stdio.h>

#include <conio.h>

#include <Windows.h>

inline char wait_for_keyboard() {
    while (!_kbhit());
    return _getch();
}

int main(int argc, char* argv[]) {
    //if (argc > 1) {
        //run_twitch_voting(argv[1]);
    //}
    return 1;
}