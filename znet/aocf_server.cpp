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

#define ZNET_IPV6_MODE ZNET_DISABLE_IPV6

#include "znet.h"
#include "zthread.h"

using namespace zero::net;
using namespace zero::thread;

#include "common.h"

static inline char wait_for_keyboard(size_t delay) {
	while (!_kbhit()) Sleep(delay);
	return _getch();
}

enum PacketType : uint8_t {
	// Original packets
	PACKET_TYPE_0 = 0,
	PACKET_TYPE_1 = 1,
	PACKET_TYPE_2 = 2,
	PACKET_TYPE_3 = 3,
	PACKET_TYPE_4 = 4,
	PACKET_TYPE_5 = 5,
	PACKET_TYPE_6 = 6,
	PACKET_TYPE_7 = 7,
	PACKET_TYPE_8 = 8,
	PACKET_TYPE_9 = 9,
	PACKET_TYPE_10 = 10,
	PACKET_TYPE_11 = 11,
	PACKET_TYPE_12 = 12,
	PACKET_TYPE_13 = 13,
	PACKET_TYPE_14 = 14,
	PACKET_TYPE_15 = 15,
	PACKET_TYPE_16 = 16,
	PACKET_TYPE_17 = 17,
	PACKET_TYPE_18 = 18,
	PACKET_TYPE_19 = 19,

	// Custom packets
	PACKET_TYPE_LOBBY_NAME = 0x80,
	PACKET_TYPE_PUNCH_PING = 0x81,
	PACKET_TYPE_PUNCH_WAIT = 0x82,
	PACKET_TYPE_PUNCH_CONNECT = 0x83,
	PACKET_TYPE_PUNCH_PEER = 0x84,
	PACKET_TYPE_PUNCH = 0x85,
	PACKET_TYPE_PUNCH_SELF = 0x86, // Same format as PACKET_TYPE_PUNCH_PEER
	PACKET_TYPE_IPV6_TEST = 0x88,
};

struct PacketLayout {
	PacketType type; // 0x0
	unsigned char data[]; // 0x1
};

// size: 0x8+
struct PacketLobbyName {
	PacketType type; // 0x0
	// 0x1
	size_t length; // 0x4
	alignas(4) char name[]; // 0x8
};

// size: 0x1
struct PacketPunchPing {
	PacketType type; // 0x0
	// 0x1
};

// size: 0x1
struct PacketPunch {
	PacketType type; // 0x0
	// 0x1
};

static constexpr PacketPunchPing PUNCH_PING_PACKET = {
	.type = PACKET_TYPE_PUNCH_PING
};

static constexpr PacketPunch PUNCH_PACKET = {
	.type = PACKET_TYPE_PUNCH
};

static constexpr uint8_t LOCAL_IS_IPV6_MASK = 0b01;
static constexpr uint8_t DEST_IS_IPV6_MASK = 0b10;

static constexpr size_t IP_BYTE_SIZE = (std::max)(sizeof(IP4_ADDRESS), sizeof(IP6_ADDRESS));

// size: 0x4+
struct PacketPunchWait {
	PacketType type; // 0x0
	uint8_t is_ipv6; // 0x1
	uint16_t local_port; // 0x2
	alignas(4) unsigned char ip[sizeof(IP6_ADDRESS)]; // 0x4

	ipaddr_any local_ip() const {
		return ipaddr_any(this->is_ipv6 & LOCAL_IS_IPV6_MASK, this->local_port, this->ip);
	}
};

// size: 0x8+
struct PacketPunchConnect {
	PacketType type; // 0x0
	uint8_t is_ipv6; // 0x1
	uint16_t local_port; // 0x2
	uint16_t dest_port; // 0x4
	// 0x6
	alignas(4) unsigned char local_ip_buf[sizeof(IP6_ADDRESS)]; // 0x8
	alignas(4) unsigned char dest_ip_buf[sizeof(IP6_ADDRESS)]; // 0x18
	// 0x28

	ipaddr_any local_ip() const {
		return ipaddr_any(this->is_ipv6 & LOCAL_IS_IPV6_MASK, this->local_port, this->local_ip_buf);
	}

	ipaddr_any dest_ip() const {
		return ipaddr_any(this->is_ipv6 & DEST_IS_IPV6_MASK, this->dest_port, this->dest_ip_buf);
	}
};

// size: 0x4
struct PacketPunchPeer {
	PacketType type; // 0x0
	uint8_t is_ipv6; // 0x1
	uint16_t remote_port; // 0x2
	alignas(4) unsigned char ip[sizeof(IP6_ADDRESS)]; // 0x4

	PacketPunchPeer() = default;

	PacketPunchPeer(bool is_ipv6, uint16_t port, const void* ip)
		: type(PACKET_TYPE_PUNCH), is_ipv6(is_ipv6), remote_port(port)
	{
		if (is_ipv6) {
			*(IP6_ADDRESS*)this->ip = *(IP6_ADDRESS*)ip;
		} else {
			*(IP4_ADDRESS*)this->ip = *(IP4_ADDRESS*)ip;
		}
	}
};

struct PacketPunchSelf {
	PacketType type; // 0x0
	uint8_t is_ipv6; // 0x1
	uint16_t remote_port; // 0x2
	alignas(4) unsigned char ip[sizeof(IP6_ADDRESS)]; // 0x4

	PacketPunchSelf() = default;

	PacketPunchSelf(bool is_ipv6, uint16_t port, const void* ip)
		: type(PACKET_TYPE_PUNCH_SELF), is_ipv6(is_ipv6), remote_port(port)
	{
		if (is_ipv6) {
			*(IP6_ADDRESS*)this->ip = *(IP6_ADDRESS*)ip;
		} else {
			*(IP4_ADDRESS*)this->ip = *(IP4_ADDRESS*)ip;
		}
	}
};

// size: 0x10
struct PacketIPv6Test {
	PacketType type; // 0x0
	unsigned char padding[7]; // 0x1
	alignas(8) LARGE_INTEGER tsc; // 0x8
	// 0x10
};

enum ArgIndex {
	PROGRAM_NAME_ARG = 0,
	PORT_ARG = 1,
	MIN_REQUIRED_ARGS
};

static constexpr size_t TCP_BUFFER_SIZE = 4096;
static constexpr size_t TCP_SLEEP_TIME = 0.01_secms;
static constexpr size_t UDP_BUFFER_SIZE = 64;
static constexpr size_t UDP_SLEEP_TIME = 0.01_secms;

static constexpr size_t UDP_PORT_TIMEOUT = 0.5_secms;
static constexpr size_t UDP_PORT_ITER_DELAY = 0.01_secms;

static constexpr size_t UDP_PORT_ITERS = UDP_PORT_TIMEOUT / UDP_PORT_ITER_DELAY;

static constexpr size_t CLIENT_SETUP_TIMEOUT = 1_secms;
static constexpr size_t JOIN_SETUP_TIMEOUT = 5_secms;
static constexpr size_t PONG_TIMEOUT = 10_secms;

static constexpr size_t MAX_NICKNAME_LENGTH = 32;


static constexpr size_t PACKET_TYPE_LOBBY_NAME_MAX_SIZE = sizeof(PacketLobbyName) + MAX_NICKNAME_LENGTH;
static constexpr size_t PACKET_TYPE_PUNCH_WAIT_MAX_SIZE = sizeof(PacketPunchWait) + IP_BYTE_SIZE;
static constexpr size_t PACKET_TYPE_PUNCH_CONNECT_MAX_SIZE = sizeof(PacketPunchConnect) + IP_BYTE_SIZE * 2;
static constexpr size_t PACKET_TYPE_IPV6_TEST_MAX_SIZE = sizeof(PacketIPv6Test);

static constexpr size_t PACKET_TYPE_PUNCH_PEER_MAX_SIZE = sizeof(PacketPunchPeer) + IP_BYTE_SIZE;
static constexpr size_t PACKET_TYPE_PUNCH_PEER_IPV4_SIZE = sizeof(PacketPunchPeer) + sizeof(IP4_ADDRESS);
static constexpr size_t PACKET_TYPE_PUNCH_PEER_IPV6_SIZE = sizeof(PacketPunchPeer) + sizeof(IP6_ADDRESS);

static constexpr size_t MAX_SERVER_UDP_PACKET = (std::max)({
													PACKET_TYPE_LOBBY_NAME_MAX_SIZE,
													PACKET_TYPE_PUNCH_WAIT_MAX_SIZE,
													PACKET_TYPE_PUNCH_CONNECT_MAX_SIZE,
													PACKET_TYPE_PUNCH_PEER_MAX_SIZE,
													PACKET_TYPE_IPV6_TEST_MAX_SIZE
												});

enum RoomType : uint8_t {
	FREE_ROOM,
	NOVICE_ROOM,
	VETERAN_ROOM,
	INVALID_ROOM = UINT8_MAX
};

static std::atomic<uint32_t> USER_COUNT[3] = {};

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
		this->socket.close();
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
			if (!this->socket.send(message_buf, send_length)) {
				printf("SEND FAIL %u (%d)\n", WSAGetLastError(), this->socket.sock);
			}
		}
	}

	void send_message(std::string_view nick, RoomType room, std::string_view message) {
		if (
			this->room_type == room //&&
			//this->nickname != nick
		) {
			printf("SEND:%.*s", message.length(), message.data());
			if (!this->socket.send(message.data(), message.length())) {
				printf("SEND FAIL %u (%d)\n", WSAGetLastError(), this->socket.sock);
			}
		}
	}
};

static constexpr size_t PUNCH_DATA_TIMEOUT = 20_secms;
static constexpr size_t PUNCH_DATA_FULL_PING_TIMER = PUNCH_DATA_TIMEOUT / UDP_SLEEP_TIME;
static constexpr size_t PUNCH_DATA_PING_TIMER_SEND_THRESHOLD = PUNCH_DATA_FULL_PING_TIMER / 2;
static constexpr size_t PUNCH_DATA_PING_SEND_REPEATS = 5;
static constexpr size_t PUNCH_DATA_PING_SEND_INTERVAL = PUNCH_DATA_PING_TIMER_SEND_THRESHOLD / PUNCH_DATA_PING_SEND_REPEATS;

static_assert(PUNCH_DATA_FULL_PING_TIMER < UINT16_MAX);

struct PunchData {
	bool alive;
	uint16_t ping_timer;
	ipaddr_any local_addr;
	sockaddr_any remote_addr;

	PunchData(const ipaddr_any& local_addr, const sockaddr_any& remote_addr)
		: alive(true), ping_timer(PUNCH_DATA_FULL_PING_TIMER),
		local_addr(local_addr), remote_addr(remote_addr)
	{}

	void reset_ping_timer() {
		this->ping_timer = PUNCH_DATA_FULL_PING_TIMER;
	}

	bool tick_ping_timer() {
		//printf("ping timer: %u\n", this->ping_timer);
		return --this->ping_timer;
	}

	bool ping_timer_send() const {
		return
			this->ping_timer <= PUNCH_DATA_PING_TIMER_SEND_THRESHOLD &&
			!(this->ping_timer % PUNCH_DATA_PING_SEND_INTERVAL);
	}
};

static std::unordered_map<std::string_view, UserData*> user_map;
static std::shared_mutex user_mutex;

static std::vector<PunchData> punch_data;

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

static inline void insert_punch_data(const ipaddr_any& local_addr, const sockaddr_any& remote_addr) {
	for (auto& punch_data : punch_data) {
		if (!punch_data.alive) {
			punch_data.alive = true;
			punch_data.ping_timer = PUNCH_DATA_FULL_PING_TIMER;
			punch_data.local_addr = local_addr;
			punch_data.remote_addr = remote_addr;
			return;
		}
	}
	punch_data.emplace_back(local_addr, remote_addr);
}

template <typename L>
static inline void find_punch_data(const L& lambda) {
	for (auto& punch_data : punch_data) {
		if (lambda(punch_data)) {
			break;
		}
	}
}

template <typename L>
static inline void iter_punch_data(const L& lambda) {
	for (auto& punch_data : punch_data) {
		lambda(punch_data);
	}
}

static inline constexpr char REQUEST_LOGIN_REPLY[] = "E\n";

static inline constexpr char NICK_IN_USE_REPLY[] = ": 433 E\n";
static inline constexpr char REQUEST_JOIN_REPLY[] = ": 422 E\n";

//static inline constexpr char JOIN_FREE_REPLY[] = "! JOIN #th155_Free\n\0\0\0\0\0\0\0\0\0\0\0\0";
static inline constexpr char JOIN_FREE_REPLY[] = "! JOIN #th155_Free %u\n";
static inline constexpr size_t JOIN_FREE_REPLY_LEN = 19;

//static inline constexpr char JOIN_NOVICE_REPLY[] = "! JOIN #th155_Novice\n\0\0\0\0\0\0\0\0\0\0";
static inline constexpr char JOIN_NOVICE_REPLY[] = "! JOIN #th155_Novice %u\n";
static inline constexpr size_t JOIN_NOVICE_REPLY_LEN = 21;

//static inline constexpr char JOIN_VETERAN_REPLY[] = "! JOIN #th155_Veteran\n\0\0\0\0\0\0\0\0\0";
static inline constexpr char JOIN_VETERAN_REPLY[] = "! JOIN #th155_Veteran %u\n";
static inline constexpr size_t JOIN_VETERAN_REPLY_LEN = 22;

static inline constexpr std::string_view JOIN_FREE_VIEW = "JOIN #th155_Free Free"sv;
static inline constexpr std::string_view JOIN_NOVICE_VIEW = "JOIN #th155_Novice Novice"sv;
static inline constexpr std::string_view JOIN_VETERAN_VIEW = "JOIN #th155_Veteran Veteran"sv;

#define CALC_JOIN_USERS_BUFFER(max_val) sizeof("! JOIN #th155_Veteran " MACRO_STR(max_val) "\n")

static inline constexpr size_t JOIN_AND_USER_COUNT_BUFFER_SIZE = CALC_JOIN_USERS_BUFFER(4294967295);

static inline constexpr char USER_COUNT_REPLY[] = "USERS ";
static inline constexpr size_t USER_COUNT_REPLY_LEN = sizeof(USER_COUNT_REPLY) - 1;

static inline constexpr char PING_MESSAGE[] = "PING\n";
static inline constexpr std::string_view PONG_REPLY = "PONG 0"sv;

static inline constexpr char ERROR_MESSAGE[] = "ERROR\n";

static inline constexpr std::string_view STAT_COMMAND_VIEW = "STAT"sv;
static inline constexpr std::string_view REQUEST_MATCH_COMMAND_VIEW = "REQUEST_MATCH "sv;
static inline constexpr std::string_view WELCOME_COMMAND_VIEW = "WELCOME "sv;

static inline constexpr std::string_view PASSWORD_VIEW = "PASS kzxmckfqbpqieh8rw<rczuturKfnsjxhauhybttboiuuzmWdmnt5mnlczpythaxf"sv;

int main(int argc, char* argv[]) {
	if (argc < MIN_REQUIRED_ARGS) {
		error_exit("aocf_server.exe <port>");
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
						wait_for_keyboard(1_secms);
					});

					udp_socket.set_blocking_state(false);
					//udp_socket.set_receive_timeout(UDP_SLEEP_TIME);

					zjthread udp_listen_thread([=](const std::atomic<uint8_t>& stop) {
						do {
							unsigned char buffer[MAX_SERVER_UDP_PACKET];

							sockaddr_any peer_addr;
							if (size_t receive_length = udp_socket.receive(buffer, peer_addr)) {

								PacketLayout* raw_packet = (PacketLayout*)buffer;
								printf("UDP: %zu type %zu\n", receive_length, raw_packet->type);
								switch (raw_packet->type) {
									if (0) {
									case PACKET_TYPE_LOBBY_NAME:

										//udp_socket.send(PUNCH_PACKET, peer_addr);

										PacketLobbyName* packet = (PacketLobbyName*)raw_packet;

										uint16_t peer_port = get_port(peer_addr);

										printf("NAMEPORT: %.*s = %u ", packet->length, packet->name, peer_port);
										print_sockaddr(peer_addr);
										printf("\n");

										std::string_view peer_nickname_view{ packet->name, packet->length };

										shared_user_data(peer_nickname_view, [peer_port](UserData* user_data) {
											user_data->external_port = peer_port;
										});
										//printf("UDP port set end\n");
									} else {
									case PACKET_TYPE_PUNCH_WAIT:
										PacketPunchWait* packet = (PacketPunchWait*)raw_packet;
										insert_punch_data(packet->local_ip(), peer_addr);
									}
									{
										PacketPunchSelf packet;
										bool is_ipv6 = peer_addr.storage.ss_family == AF_INET6;
										new (&packet) PacketPunchSelf(is_ipv6, get_port(peer_addr), peer_addr.get_ip_ptr());
										udp_socket.send(packet, peer_addr);
										break;
									}
									case PACKET_TYPE_PUNCH_PING: {
										printf("PUNCH PING ");
										print_sockaddr(peer_addr);
										printf("\n");
										find_punch_data([&](PunchData& punch_data) {
											if (
												punch_data.alive &&
												ports_match(punch_data.remote_addr, peer_addr) &&
												ips_match(punch_data.remote_addr, peer_addr)
											) {
												punch_data.reset_ping_timer();
												return true;
											}
											return false;
										});
										break;
									}
									/*
									case PACKET_TYPE_PUNCH_CONNECT: {
										PacketPunchConnect* packet = (PacketPunchConnect*)raw_packet;
										ipaddr_any dest_addr = packet->dest_ip();
										find_punch_data([&](PunchData& punch_data) {
											if (
												punch_data.alive &&
												ports_match(punch_data.local_addr, dest_addr) &&
												ips_match(punch_data.remote_addr, dest_addr)
											) {
												PacketPunchPeer packet;

												// TODO: Attempt to map between v4 and v6 for compatibility

												bool is_ipv6 = peer_addr.storage.ss_family == AF_INET6;
												new (&packet) PacketPunchPeer(is_ipv6, get_port(peer_addr), peer_addr.get_ip_ptr());
												udp_socket.send(packet, punch_data.remote_addr);
												is_ipv6 = punch_data.remote_addr.storage.ss_family == AF_INET6;
												new (&packet) PacketPunchPeer(is_ipv6, get_port(punch_data.remote_addr), punch_data.remote_addr.get_ip_ptr());
												udp_socket.send(packet, peer_addr);
												return true;
											}
											return false;
										});
										break;
									}
									*/
									case PACKET_TYPE_IPV6_TEST: {
										//printf("IPv6 test received\n");
										udp_socket.send(*(PacketIPv6Test*)buffer, peer_addr);
										break;
									}
								}
							}
							iter_punch_data([&](PunchData& punch_data) {
								if (punch_data.alive) {
									if (punch_data.tick_ping_timer()) {
										if (punch_data.ping_timer_send()) {
											//printf("Sending punch ping...\n");
											udp_socket.send(PUNCH_PING_PACKET, punch_data.remote_addr);
										}
									}
									else {
										printf("Killed punch client\n");
										punch_data.alive = false;
									}
								}
							});
							Sleep(UDP_SLEEP_TIME);
						} while (!stop);
						printf("!!!!! UDP THREAD EXIT !!!!!\n");
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

											response_socket.set_receive_timeout(JOIN_SETUP_TIMEOUT);

											std::string_view join_view;
											if (receive_message(join_view)) {
												//printf("%s\n", join_view.data());

												RoomType room_type;

												const char* join_response_str;
												char join_and_users_buffer[JOIN_AND_USER_COUNT_BUFFER_SIZE];
												size_t join_response_length;
												if (join_view == JOIN_FREE_VIEW) {
													join_response_str = JOIN_FREE_REPLY;
													//join_response_length = JOIN_FREE_REPLY_LEN;
													room_type = FREE_ROOM;
												}
												else if (join_view == JOIN_NOVICE_VIEW) {
													join_response_str = JOIN_NOVICE_REPLY;
													//join_response_length = JOIN_NOVICE_REPLY_LEN;
													room_type = NOVICE_ROOM;
												}
												else if (join_view == JOIN_VETERAN_VIEW) {
													join_response_str = JOIN_VETERAN_REPLY;
													//join_response_length = JOIN_VETERAN_REPLY_LEN;
													room_type = VETERAN_ROOM;
												}
												else {
													remove_user_data(nick_view);
													goto disconnect;
												}

												join_response_length = sprintf(join_and_users_buffer, join_response_str, USER_COUNT[room_type].load());

												sockaddr_any address;
												response_socket.get_peer_addr(address);

												shared_user_data(nick_view, [&](UserData* user_data) {
													user_data->room_type = room_type;
													user_data->address = address;
												});

												memcpy(name_buffer + 1 + nick_view.length(), join_and_users_buffer, JOIN_AND_USER_COUNT_BUFFER_SIZE);

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
												bool is_waiting = false;
												memcpy(join_and_users_buffer, USER_COUNT_REPLY, USER_COUNT_REPLY_LEN);
												do {
													while (receive_message(message_view)) {
														printf(
															"GOT :%s\n"
															"FROM:%s\n"
															, message_view.data()
															, nick_view.data()
														);

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

																if (command_view.starts_with(STAT_COMMAND_VIEW)) {
																	if (!is_waiting) {
																		size_t user_count_len = sprintf(&join_and_users_buffer[USER_COUNT_REPLY_LEN], "%u\n", ++USER_COUNT[room_type]) + USER_COUNT_REPLY_LEN;
																		std::string_view user_count_view{ join_and_users_buffer, user_count_len };
																		shared_iter_user_data([=](UserData* user_data) {
																			user_data->send_message(nick_view, room_type, user_count_view);
																		});
																	}
																	is_waiting = true;
																}
																else if (command_view.starts_with(REQUEST_MATCH_COMMAND_VIEW)) {
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
																	if (is_waiting) {
																		size_t user_count_len = sprintf(&join_and_users_buffer[USER_COUNT_REPLY_LEN], "%u\n", --USER_COUNT[room_type]) + USER_COUNT_REPLY_LEN;
																		std::string_view user_count_view{ join_and_users_buffer, user_count_len };
																		shared_iter_user_data([=](UserData* user_data) {
																			user_data->send_message(nick_view, room_type, user_count_view);
																		});
																	}
																	is_waiting = false;

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

													// If a ping needs to be sent, assume the user has stopped waiting
													if (is_waiting) {
														size_t user_count_len = sprintf(&join_and_users_buffer[USER_COUNT_REPLY_LEN], "%u\n", --USER_COUNT[room_type]) + USER_COUNT_REPLY_LEN;
														std::string_view user_count_view{ join_and_users_buffer, user_count_len };
														shared_iter_user_data([=](UserData* user_data) {
															user_data->send_message(nick_view, room_type, user_count_view);
														});
													}
													is_waiting = false;
												} while (receive_message(message_view) && message_view == PONG_REPLY);

												printf("SUCCEEDED, disconnecting...\n");
											}
											remove_user_data(nick_view);
											goto success;
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