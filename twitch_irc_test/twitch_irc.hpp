#pragma once
#ifndef TWITCH_IRC_H
#define TWITCH_IRC_H 1

#include <stdlib.h>
#include <array>

// Starts the thread for polling twitch chat from the specified channel name
bool __fastcall start_twitch_thread(const char* channel_name);

// Stops the thread for polling twitch char if it is currently running
void __fastcall stop_twitch_thread();

// Control whether messages are parsed as votes (defaults to false)
void __fastcall twitch_voting(bool state);

// Queries whether the numbers 1-4 or 5-8 are the current vote options
bool __fastcall voting_is_high_numbers();

// Gets the current vote totals
std::array<size_t, 4> __fastcall get_votes();

// Finalizes the current vote totals and resets to the other set of numbers
std::array<size_t, 4> __fastcall get_final_votes();

// Lock the recent message list so that messages can be parsed.
bool __fastcall lock_recent_messages();

// Unlock the recent message list so that new messages can be added.
void __fastcall unlock_recent_messages();

// Retrieve the most recent non-vote message that hasn't already been retreived.
// Only safe to call when messages are locked.
const char* __fastcall get_recent_message();

template<typename L>
static inline bool parse_recent_message(const L& func) {
	if (lock_recent_messages()) {
		func(get_recent_message());
		unlock_recent_messages();
		return true;
	}
	return false;
}

template<typename L>
static inline bool parse_all_recent_messages(const L& func) {
	if (lock_recent_messages()) {
		while (const char* message = get_recent_message()) {
			if (!func(message)) break;
		}
		unlock_recent_messages();
		return true;
	}
	return false;
}

#endif