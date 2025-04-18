#include "twitch_irc.hpp"

#include <stdio.h>

#include <conio.h>

#include <Windows.h>

inline char wait_for_keyboard(size_t delay) {
    while (!_kbhit()) Sleep(delay);
    return _getch();
}

__declspec(noinline) void test_parsing() {
    while (1) {
        //wait_for_keyboard();

        switch (wait_for_keyboard(100)) {
            case 'l':
                if (lock_recent_messages()) {
                    printf("Locking messages\n");
                }
                continue;
            case 'u':
                unlock_recent_messages();
                printf("Unlocking messages\n");
                continue;
            case 'g':
                if (const char* message = get_recent_message()) {
                    printf("%s\n", message);
                } else {
                    printf("No more messages\n");
                }
                continue;
            case 'p':
                parse_all_recent_messages(
                    [](const char* message) {
                        printf("%s\n", message);
                        return true;
                    }
                );
                continue;
            case 'q':
                break;
            case 'v': {
                auto votes = get_final_votes();
                if (voting_is_high_numbers()) {
                    printf(
                        "1: %zu\n"
                        "2: %zu\n"
                        "3: %zu\n"
                        "4: %zu\n"
                        , votes[0]
                        , votes[1]
                        , votes[2]
                        , votes[3]
                    );
                } else {
                    printf(
                        "5: %zu\n"
                        "6: %zu\n"
                        "7: %zu\n"
                        "8: %zu\n"
                        , votes[0]
                        , votes[1]
                        , votes[2]
                        , votes[3]
                    );
                }
                continue;
            }
            default:
                continue;
        }
        break;
    }
}

extern bool __fastcall start_youtube_thread(const char* channel_id);

int main(int argc, char* argv[]) {

    //start_youtube_thread("UCV6mNrW8CrmWtcxWfQXy11g");
    //return 0;

    //if (argc > 1) {
        //run_twitch_voting(argv[1]);
        if (start_twitch_thread("bravidunno")) {

            twitch_voting(true);

            test_parsing();

            stop_twitch_thread();
        }
        else {
            wait_for_keyboard(100);
        }
        return 0;
    //}
    return 1;
}