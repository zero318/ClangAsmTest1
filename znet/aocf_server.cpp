#if __INTELLISENSE__
#if !_HAS_CXX17
#undef _HAS_CXX17
#define _HAS_CXX17 1
#endif

#define starts_with(...) _Starts_with(__VA_ARGS__)
#define ends_with(...) _Starts_with(__VA_ARGS__)
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include <utility>
#include <unordered_map>
#include <deque>
#include <queue>
#include <atomic>
#include <mutex>
#include <shared_mutex>

#include <string>
#include <string_view>
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

#include <conio.h>

#include <Windows.h>

#include "znet.h"
#include "zthread.h"

using namespace zero::net;
using namespace zero::thread;

#include "common.h"

enum ArgIndex {
	PROGRAM_NAME_ARG = 0,
	PORT_ARG = 1,
	MIN_REQUIRED_ARGS
};

static constexpr size_t TCP_BUFFER_SIZE = 4096;
static constexpr size_t TCP_SLEEP_TIME = 10;
static constexpr size_t UDP_BUFFER_SIZE = 64;
static constexpr size_t UDP_SLEEP_TIME = 10;

static constexpr size_t UDP_PORT_TIMEOUT = 500;
static constexpr size_t UDP_PORT_ITER_DELAY = 10;

static constexpr size_t UDP_PORT_ITERS = UDP_PORT_TIMEOUT / UDP_PORT_ITER_DELAY;

static constexpr size_t CLIENT_SETUP_TIMEOUT = 1000;
static constexpr size_t PONG_TIMEOUT = 10000;

static constexpr size_t MAX_NICKNAME_LENGTH = 32;

enum RoomType : uint8_t {
	INVALID_ROOM,
	FREE_ROOM,
	NOVICE_ROOM,
	VETERAN_ROOM
};

struct UserData {
	std::atomic<bool> delete_flag = { false }; // 0x0
	RoomType room_type = INVALID_ROOM; // 0x1
	std::atomic<uint16_t> external_port = { 0 }; // 0x2
	SocketTCP socket; // 0x4
	std::string_view nickname; // 0x8
	sockaddr_any address = {}; // 0x10

	UserData(std::string_view nick, SocketTCP socket)
		: socket(socket), nickname(nick)
	{

	}

	~UserData() {
		if (void* name = (void*)this->nickname.data()) {
			free(name);
			// still trying to track down that invalid free...
			memset(&this->nickname, 0, sizeof(this->nickname));
		}
	}

	void send_message_from(std::string_view nick, RoomType room, std::string_view source, std::string_view message) {
		
		if (
			this->room_type == room &&
			this->nickname != nick
		) {
			char message_buf[source.length() + 1 + message.length() + 1];
			int send_length = sprintf(message_buf,
				"%s %s\n",
				source.data(), message.data()
			);
			printf("SEND:%s", message_buf);
			this->socket.send(message_buf, send_length);
		}
	}
};

static std::unordered_map<std::string_view, UserData*> user_map;
static std::shared_mutex user_mutex;

using shared_mutex_lock = std::shared_lock<std::shared_mutex>;
using unique_mutex_lock = std::unique_lock<std::shared_mutex>;

template <typename L>
static inline void shared_user_data(std::string_view user, const L& lambda) {
	shared_mutex_lock lock(user_mutex);

	auto iter = user_map.find(user);
	if (iter != user_map.end()) {
		UserData* user_data = iter->second;
		lambda(user_data);
	}
}

template <typename L>
static inline void exclusive_user_data(std::string_view user, const L& lambda) {
	unique_mutex_lock lock(user_mutex);

	auto iter = user_map.find(user);
	if (iter != user_map.end()) {
		UserData* user_data = iter->second;
		lambda(user_data);
	}
}

template <typename L>
static inline void shared_find_user_data(const L& lambda) {
	shared_mutex_lock lock(user_mutex);

	for (auto iter : user_map) {
		UserData* user_data = iter.second;
		if (lambda(user_data)) {
			break;
		}
	}
}

static inline bool insert_user_data(std::string_view user, SocketTCP socket) {
	unique_mutex_lock lock(user_mutex);

	if (!user_map.contains(user)) {
		char* user_ptr = (char*)malloc(user.length() + 1);
		user_ptr[user.length()] = '\0';
		memcpy(user_ptr, user.data(), user.length());

		std::string_view user_full{ user_ptr, user.length() };
		auto [iter, success] = user_map.try_emplace(user_full, nullptr);
		if (success) {
			//printf("New user: %s\n", user_full.data());
			iter->second = new UserData(user_full, socket);
			return true;
		}
		else {
			free(user_ptr);
		}
	}
	return false;
}

static inline void remove_user_data(std::string_view user) {
	shared_mutex_lock lock(user_mutex);

	auto iter = user_map.find(user);
	if (iter != user_map.end()) {
		iter->second->delete_flag = true;
	}
}

template<typename L>
static inline void shared_iter_user_data(const L& lambda) {
	shared_mutex_lock lock(user_mutex);

	for (auto user_data : user_map) {
		lambda(user_data.second);
	}
}

template <typename L>
static inline void exclusive_iter_user_data(const L& lambda) {
	unique_mutex_lock lock(user_mutex);

	auto user_iter = user_map.begin();
	while (user_iter != user_map.end()) {
		auto cur_iter = user_iter++;
		if (!lambda(cur_iter->second)) {
			UserData* temp = cur_iter->second;
			user_map.erase(cur_iter);
			delete temp;
		}
	}
}

static inline constexpr char REQUEST_LOGIN_REPLY[] = "E\n";

static inline constexpr char NICK_IN_USE_REPLY[] = ": 433 E\n";
static inline constexpr char REQUEST_JOIN_REPLY[] = ": 422 E\n";

static inline constexpr char JOIN_FREE_REPLY[] = "! JOIN #th155_Free\n\0\0\0\0\0\0\0\0\0\0\0\0";
static inline constexpr size_t JOIN_FREE_REPLY_LEN = 19;

static inline constexpr char JOIN_NOVICE_REPLY[] = "! JOIN #th155_Novice\n\0\0\0\0\0\0\0\0\0\0";
static inline constexpr size_t JOIN_NOVICE_REPLY_LEN = 21;

static inline constexpr char JOIN_VETERAN_REPLY[] = "! JOIN #th155_Veteran\n\0\0\0\0\0\0\0\0\0";
static inline constexpr size_t JOIN_VETERAN_REPLY_LEN = 22;

static inline constexpr std::string_view JOIN_FREE_VIEW = "JOIN #th155_Free Free"sv;
static inline constexpr std::string_view JOIN_NOVICE_VIEW = "JOIN #th155_Novice Novice"sv;
static inline constexpr std::string_view JOIN_VETERAN_VIEW = "JOIN #th155_Veteran Veteran"sv;

static inline constexpr char PING_MESSAGE[] = "PING\n";
static inline constexpr std::string_view PONG_REPLY = "PONG 0"sv;

static inline constexpr char ERROR_MESSAGE[] = "ERROR\n";

static inline constexpr std::string_view REQUEST_MATCH_COMMAND_VIEW = "REQUEST_MATCH "sv;
static inline constexpr std::string_view WELCOME_COMMAND_VIEW = "WELCOME "sv;

static inline constexpr std::string_view PASSWORD_VIEW = "PASS kzxmckfqbpqieh8rw<rczuturKfnsjxhauhybttboiuuzmWdmnt5mnlczpythaxf"sv;

int main(int argc, char* argv[]) {
	if (argc < MIN_REQUIRED_ARGS) {
		error_exit("aocf_server.exe <tcp_port> <udp_port>");
	}

	uint16_t port;
	if (!strbuf_to_uint16(argv[PORT_ARG], port)) {
		error_exit("Invalid port");
	}

	if (!enable_winsock()) {
		error_exit("Count not initialize winsock");
	}

	if (SocketTCP tcp_socket = SocketTCP::create()) {
		if (tcp_socket.bind(port)) {
			if (SocketUDP udp_socket = SocketUDP::create()) {
				if (udp_socket.bind(port)) {
					tcp_socket.set_blocking_state(false);

					tcp_socket.listen(50);

					zjthread stupid_keyboard_thread([](const std::atomic<uint8_t>& stop) {
						wait_for_keyboard();
					});

					zjthread udp_listen_thread([=](const std::atomic<uint8_t>& stop) {
						do {
							char text[MAX_NICKNAME_LENGTH + 1];

							sockaddr_any peer_addr;
							if (size_t receive_length = udp_socket.receive_text(text, peer_addr)) {

								uint16_t peer_port = get_port(peer_addr);

								printf("UDP :%s=%u\n", text, peer_port);

								std::string_view peer_nickname_view{ text, receive_length };

								shared_user_data(peer_nickname_view, [peer_port](UserData* user_data) {
									user_data->external_port = peer_port;
								});

								//printf("UDP port set end\n");
							}

							Sleep(UDP_SLEEP_TIME);
						} while (!stop);
						//printf("!!!!! UDP THREAD EXIT !!!!!\n");
					});

					do {
						if (SocketTCP response_socket = tcp_socket.accept()) {
							printf("Accepted connection!\n");
							std::thread([response_socket]() mutable {

								response_socket.set_blocking_state(true);

								response_socket.set_receive_timeout(CLIENT_SETUP_TIMEOUT);
								response_socket.send_text(REQUEST_LOGIN_REPLY);

								char text[TCP_BUFFER_SIZE];
								std::queue<std::string_view> messages;

								char name_buffer[MAX_NICKNAME_LENGTH + 1 + std::max(32u, MAX_ADDR_BUFF_SIZE)]{ ':' };
						
								auto receive_message = [&, response_socket](std::string_view& out) -> bool {
							
									if (messages.empty()) {
										if (size_t length = response_socket.receive_text(text)) {
											char* text_start = text;
											char* text_read = text;
											do {
												char c = *text_read;
												switch (c) {
													case '\r': case '\n':
														*text_read = '\0';
													case '\0':
														if (size_t message_length = text_read - text_start) {
															//printf("MSG :%s\n", text_start);
															messages.emplace(text_start, message_length);
														}
														text_start = text_read + 1;
												}
												++text_read;
											} while (--length);
											if (messages.empty()) {
												return false;
											}
										}
										else {
											return false;
										}
									}
									out = messages.front();
									//printf("GOT :%s\n", out.data());
									messages.pop();
									return true;
								};

								std::string_view nick_msg_view;
								if (receive_message(nick_msg_view) && nick_msg_view.starts_with("NICK "sv)) {
									//printf("%s\n", nick_msg_view.data());
									memcpy(name_buffer + 1, nick_msg_view.data() + 5, nick_msg_view.length() - 4);

									std::string_view nick_view{ name_buffer + 1, nick_msg_view.length() - 5 };

									std::string_view pass_msg_view;
									if (receive_message(pass_msg_view) && pass_msg_view == PASSWORD_VIEW) {
										//printf("%s\n", pass_msg_view.data());

										std::string_view user_msg_view;
										if (receive_message(user_msg_view) && user_msg_view.starts_with("USER "sv)) {
											//printf("%s\n", user_msg_view.data());
									
											for (;;) {
												//printf("NICK:%.*s\n", nick_view.length(), nick_view.data());
												if (!insert_user_data(nick_view, response_socket)) {
													response_socket.send_text(NICK_IN_USE_REPLY);

													if (receive_message(nick_msg_view) && nick_msg_view.starts_with("NICK "sv)) {
														memcpy(name_buffer + 1, nick_msg_view.data() + 5, nick_msg_view.length() - 1);
														nick_view = { name_buffer + 1, nick_msg_view.length() - 5 };
														//printf("%s", textA);
														continue;
													}
													goto disconnect;
												}
												break;
											}

											response_socket.send_text(REQUEST_JOIN_REPLY);

											response_socket.set_receive_timeout(5000);

											std::string_view join_view;
											if (receive_message(join_view)) {
												//printf("%s\n", join_view.data());

												RoomType room_type;

												const char* join_response_str;
												size_t join_response_length;
												if (join_view == JOIN_FREE_VIEW) {
													join_response_str = JOIN_FREE_REPLY;
													join_response_length = JOIN_FREE_REPLY_LEN;
													room_type = FREE_ROOM;
												}
												else if (join_view == JOIN_NOVICE_VIEW) {
													join_response_str = JOIN_NOVICE_REPLY;
													join_response_length = JOIN_NOVICE_REPLY_LEN;
													room_type = NOVICE_ROOM;
												}
												else if (join_view == JOIN_VETERAN_VIEW) {
													join_response_str = JOIN_VETERAN_REPLY;
													join_response_length = JOIN_VETERAN_REPLY_LEN;
													room_type = VETERAN_ROOM;
												}
												else {
													remove_user_data(nick_view);
													goto disconnect;
												}

												sockaddr_any address;
												response_socket.get_peer_addr(address);

												shared_user_data(nick_view, [&](UserData* user_data) {
													user_data->room_type = room_type;
													user_data->address = address;
												});

												memcpy(name_buffer + 1 + nick_view.length(), join_response_str, 32);

												//printf("%s", name_buffer);
												response_socket.send(name_buffer, 1 + nick_view.length() + join_response_length);

												name_buffer[1 + nick_view.length() + 0] = '!';
												name_buffer[1 + nick_view.length() + 1] = '@';

												response_socket.set_receive_timeout(PONG_TIMEOUT);

												char* ip_buffer = name_buffer + 1 + nick_view.length() + 2;
												ip_buffer[sprint_sockaddr(address, ip_buffer)] = '\0';
												//printf("NAME:%s", name_buffer);
												std::string_view user_view{ name_buffer };

												std::string_view message_view;
												do {
													while (receive_message(message_view)) {
														printf("GOT :%s\n", message_view.data());
														printf("FROM:%s\n", nick_view.data());

														char replacement_message[message_view.length() + 32];

														if (message_view.starts_with("PRIVMSG "sv)) {
															if (const char* command_start = (const char*)memchr(message_view.data(), ':', message_view.length())) {
																++command_start;

																size_t prefix_length = command_start - message_view.data();
																std::string_view command_view{ command_start, message_view.length() - prefix_length };

																auto make_replacement_command = [&]<typename L>(const L& lambda) {
																	if (const char* nickname_end = (const char*)memchr(command_view.data(), ' ', command_view.length())) {
																		prefix_length += nickname_end - command_view.data() + 1;
																		for (size_t i = 0; i < UDP_PORT_ITERS; ++i) {
																			Sleep(UDP_PORT_ITER_DELAY);
																			uint16_t external_port = 0;
																			shared_user_data(nick_view, [&](UserData* user_data) {
																				external_port = user_data->external_port;
																			});
																			if (external_port != 0) {
																				lambda(external_port, prefix_length);
																				break;
																			}
																		}
																	}
																};

																if (command_view.starts_with(REQUEST_MATCH_COMMAND_VIEW)) {
																	prefix_length += REQUEST_MATCH_COMMAND_VIEW.length();
																	command_view.remove_prefix(REQUEST_MATCH_COMMAND_VIEW.length());
																	//printf("REPA:%s\n", command_view.data());

																	make_replacement_command([&](uint16_t external_port, size_t copy_length) {
																		int replacement_length = sprintf(replacement_message,
																			"%.*s%u"
																			, (int)copy_length, message_view.data()
																			, external_port
																		);
																		message_view = std::string_view(replacement_message, replacement_length);
																		//printf("REPB:%s\n", replacement_message);
																	});
																}
																else if (command_view.starts_with(WELCOME_COMMAND_VIEW)) {
																	prefix_length += WELCOME_COMMAND_VIEW.length();
																	command_view.remove_prefix(WELCOME_COMMAND_VIEW.length());
																	//printf("REPC:%s\n", command_view.data());


																	make_replacement_command([&](uint16_t external_port, size_t copy_length) {
																		int replacement_length = sprintf(replacement_message,
																			"%.*s%u %u"
																			, (int)copy_length, message_view.data()
																			, external_port, external_port
																		);
																		message_view = std::string_view(replacement_message, replacement_length);
																		//printf("REPD:%s\n", replacement_message);
																	});
																}
															}
														}

														shared_iter_user_data([=](UserData* user_data) {
															user_data->send_message_from(nick_view, room_type, user_view, message_view);
														});
													}
													response_socket.send_text(PING_MESSAGE);
												} while (receive_message(message_view) && message_view == PONG_REPLY);

												remove_user_data(nick_view);

												printf("SUCCEEDED, disconnecting...\n");
												goto success;
											}
										}
									}
								}
							disconnect:
								printf("FAILED, disconnecting...\n");
							success:
								response_socket.close();
							}).detach();
						}
						Sleep(TCP_SLEEP_TIME);
						exclusive_iter_user_data([](UserData* user_data) -> bool {
							return !user_data->delete_flag.load(std::memory_order_relaxed);
						});
					} while (!stupid_keyboard_thread.stopped());
					udp_listen_thread.stop();
				}
				udp_socket.close();
			}
		}
		tcp_socket.close();
	}

	disable_winsock();

	return EXIT_SUCCESS;
}