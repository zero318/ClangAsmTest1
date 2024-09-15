// size: 0x8
struct AsyncTcpSSLClientInnerA {
    int __dword_0; // 0x0
    int __dword_4; // 0x4
    // 0x8
};

struct AsyncTcpSSLClientInnerB {

};

// size: 0x298
struct AsyncTcpSSLClient {
    // void* vftable; // 0x0
    // 0x4
    int __int_8; // 0x8
    SOCKET socket; // 0xC

    uint32_t __uint_114; // 0x114
    SOCKET __socket_array_118[64]; // 0x118
    AsyncTcpSSLClientInnerA* __innerA_ptr_218; // 0x218
    char __byte_21C; // 0x21C
    uint32_t __time_220; // 0x220
    // 0x224
    long long __longlong_228; // 0x228
    long long __longlong_230; // 0x230
    std::string server_string; // 0x238
    std::string port_string; // 0x250
    std::mutex __mutex_268; // 0x268
    // 0x298

    AsyncTcpSSLClient() {
        this->__int_8 = 0;
        this->__time_220 = 0;
        this->__innerA_ptr_218 = new AsyncTcpSSLClientInnerA();
        this->__innerA_ptr_218->__dword_0 = 0;
        this->__innerA_ptr_218->__dword_4 = 0;
        this->__longlong_230 = 10;
    }

    // Method 0
    // Rx1FF30
    virtual ~AsyncTcpSSLClient() {
        if (this->__innerA_ptr_218) {
            delete this->__innerA_ptr_218;
        }
    }

    // Method 4
    // Rx20560
    virtual int __method_4() {
        return this->__int_8;
    }

    // Method 8
    // Rx1FF70
    virtual int close() {
        if (!this->__int_8) {
            return 0;
        }
        if (this->__int_8 == 1) {
            this->__int_8 = 4;
            return 0;
        }
        this->__time_220 = timeGetTime();
        this->__int_8 = 3;
        if (this->__innerA_ptr_218) {

        }
        closesocket(this->socket);
        this->__int_8 = 0;
        return 0;
    }

    // Method C
    // Rx15B0
    virtual void __method_C() {
    }

    // Rx20060
    void __sub_r20060(const char* server, const char* port, bool arg3) {

        if (this->__int_8 != 0) {
            this->close();
        }
        this->__int_8 = 1;
        this->__byte_21C = 0;
        this->server_string = server;
        this->port_string = port;
    }

    // Rx20160
    void __sub_r20160(const char* server, const char* port) {
        // lock mutex
        server = this->server_string.c_str();
        port = this->port_string.c_str();
        this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
        hostent* host = gethostbyname(server);
        if (!host) {
            this->close();
            // unlock mutex
            return;
        }
        sockaddr_in addr = {};

        addr.sin_family = AF_INET;
        addr.sin_port = htons(atoi(port));
        ((uint8_t*)&addr.sin_addr)[0] = host->h_addr[0];
        ((uint8_t*)&addr.sin_addr)[1] = host->h_addr[1];
        ((uint8_t*)&addr.sin_addr)[2] = host->h_addr[2];
        ((uint8_t*)&addr.sin_addr)[3] = host->h_addr[3];
        if (FAILED(connect(this->socket, (const sockaddr*)&addr, sizeof(sockaddr_in)))) {
            this->close();
            // unlock mutex
            return;
        }

        // this loop looks broken...
        this->__uint_114 = 0;
        do {
            size_t i = 0;
            for (; i < this->__uint_114; ++i) {
                if (this->__socket_array_118[i] == this->socket) {
                    break;
                }
            }
            if (
                i != this->__uint_114 &&
                this->__uint_114 < countof(this->__socket_array_118)
            ) {
                this->__socket_array_118[i] = this->socket;
                ++this->__uint_114;
            }
        } while (false);

        // this->__longlong_228 = ??? + this->__longlong_230;

        this->__byte_21C = 0;

        // SSL init


    }

    // Rx207F0
    void __sub_r207F0(long long arg1) {
        this->__longlong_230 = arg1;
    }
};

// size: 0x8
struct AsyncLobbyClientInnerA {
    int __dword_0; // 0x0
    int __dword_4; // 0x4
    // 0x8
};

// size: 0x8
struct AsyncLobbyClientInnerB {
    int __dword_0; // 0x0
    int __dword_4; // 0x4
    // 0x8
};

static std::string HOST_STRING;
static std::string MATCH_USER_DATA_STRING;

static inline constexpr int RPL_ENDOFMOTD = 376;
static inline constexpr int ERR_NOMOTD = 422;
static inline constexpr int ERR_NICKNAMEINUSE = 433;

struct StatParams {
    int peer_state; // 0x0
    int __param_2; // 0x4
    int __time_8; // 0x8
    int __param_3; // 0xC
    // 0x10
    std::string nick_string; // 0x14
    std::string host_string; // 0x2C
    int __int_44; // 0x44
    // 0x48
};

// size: 0x4F8
struct AsyncLobbyClient : AsyncTcpSSLClient {
    // AsyncTcpSSLClient base; // 0x0
    // 0x298
    AsyncLobbyClientInnerA __innerA_298; // 0x298
    CRITICAL_SECTION_SMALL __crit_section_2A0; // 0x2A0
    AsyncLobbyClientInnerB __innerB_2B8; // 0x2B8
    CRITICAL_SECTION_SMALL __crit_section_2C0; // 0x2C0
    std::string __string_2D8; // 0x2D8
    std::string lobby_prefix; // 0x2F0
    std::string current_nick; // 0x308
    std::string __string_320; // 0x320
    std::string current_channel; // 0x338
    std::string __lobby_nameA; // 0x350
    std::string password; // 0x368
    std::string __lobby_nameB; // 0x380
    std::string version_signature; // 0x398
    std::string user_data_str; // 0x3B0
    std::string match_host; // 0x3C8
    std::string __match_user_data_str; // 0x3E0 opponent port?
    uint32_t external_port; // 0x3F8
    int __dword_3FC; // 0x3FC
    int __int_400; // 0x400
    int __int_404; // 0x404
    int __dword_408; // 0x408
    AsyncLobbyClientInnerB __innerB_40C; // 0x40C
    int __dword_414; // 0x414
    uint32_t __time_418; // 0x418
    int __dword_41C; // 0x41C
    uint8_t __byte_420; // 0x420
    // 0x421
    std::string __string_424; // 0x424 opponent nick?
    char __byte_43C; // 0x43C
    // 0x43D
    std::string __recv_str; // 0x440
    //AsyncLobbyClientInnerC __innerC_458; // 0x458
    std::queue<std::string> __queue_458; // 0x458
    // 0x470

    int __int_4EC; // 0x4EC
    int32_t max_nick_length; // 0x4F0
    // 0x4F4

    AsyncLobbyClient() {
        this->lobby_prefix = "irc_";
        this->version_signature = "-";
        this->external_port = 10800;
        this->set_lobby_user_state(0);
        this->__dword_3FC = 0;
        this->__dword_41C = 0;
        this->__byte_420 = 0;
        this->__time_418 = 0;
        this->__dword_414 = 0;
        this->__byte_43C = 0;
        this->max_nick_length = 10;
        this->set_strike_factor(1000, 1000);
    }

    // Method 8
    virtual int close() {
        this->AsyncTcpSSLClient::close();
        this->current_nick = "";
        this->__string_320 = "";
        this->current_channel = "";
        this->password = "";
        this->__lobby_nameA = "";
        this->__lobby_nameB = "";
        this->match_host = "";
        this->__match_user_data_str = "";
        // more random crap

        this->set_lobby_user_state(0);
    }

    // Method 10
    // Rx4740
    virtual int get_lobby_user_state() {
        if (this->__int_8 != 2) {
            return 0;
        }
        if (
            this->__int_4EC == 100 &&
            this->__byte_420 != 0
        ) {
            return 102;
        }
        return this->__int_4EC;
    }

    // Method 14
    // Rx4790
    virtual void set_lobby_user_state(int arg) {
        this->__int_4EC = arg;
        this->__dword_408 = 0;
        this->__time_418 = 0;
        if (arg == 100) {
            this->__dword_3FC = 0;
            this->__dword_41C = 0;
            this->__byte_420 = 0;
        }
        if (arg == 200) {
            this->match_host = "";
            this->__match_user_data_str = "";
            this->__dword_414 = 0;
        }
    }

    // Method 18
    // Rx4830
    virtual void set_strike_factor(int arg1, int arg2) {
        this->__int_400 = arg1;
        this->__int_404 = arg2;
    }

    // Rx4720
    void set_external_port(uint32_t port) {
        this->external_port = port;
    }

    // Rx46F0
    void set_prefix(const char* str) {
        if (!str) {
            str = "irc_";
        }
        this->lobby_prefix = str;
    }

    // Rx4860
    const char* get_match_host() {
        HOST_STRING = this->match_host;
        this->match_host = "";
    }

    // Rx49A0
    const char* get_current_channel() {
        if (
            this->current_channel[0] == '#' ||
            this->current_channel[0] == '@'
        ) {
            return this->current_channel.c_str() + this->lobby_prefix.length() + 1;
        }
        else {
            return this->current_channel.c_str();
        }
    }

    // Rx4A10
    const char* get_current_nick() {
        return this->current_nick.c_str() + this->lobby_prefix.length();
    }

    // Rx4A40
    int count_other_user() {

    }

    // Rx4B70
    void join_channel() {

    }

    // Rx4670
    void set_max_nick_length(int length) {
        this->max_nick_length = length;
        if (this->max_nick_length > 32) {
            this->max_nick_length = 32;
        }
        if (this->max_nick_length < 9) {
            this->max_nick_length = 9;
        }
    }

    // Rx46C0
    void set_version_sig(const char* str) {
        this->version_signature = str;
    }

    // Rx8A50
    void set_user_data(const char* str) {
        this->user_data_str = str;
    }

    // Rx4900
    void get_match_user_data() {
        MATCH_USER_DATA_STRING = this->__match_user_data_str;
        this->__match_user_data_str = "";
    }

    // Rx53B0
    int connect(const char* server, const char* port, const char* password, const char* lobby_nameA, const char* lobby_nameB) {
        if (this->__int_8 != 0) {
            this->close();
        }
        this->current_nick = "";
        this->__string_320 = "";
        this->password = password;
        this->current_channel = "";
        this->__lobby_nameA = lobby_nameA;
        this->__lobby_nameB = lobby_nameB;
        this->match_host = "";
        this->__match_user_data_str = "";
        this->__byte_43C = 0;
        this->set_lobby_user_state(0);
        // crit section things

        if (
            this->__lobby_nameA[0] != '#' &&
            this->__lobby_nameA[0] != '@'
        ) {

        }
        else {

        }

        if (
            this->__lobby_nameA[0] != '#' &&
            this->__lobby_nameA[0] != '@'
        ) {

        } else {

        }

        this->__sub_r207F0(600);

        this->__sub_r20060(server, port, true);
    }
	
    // Rx69C0
    void __recv() {

        char bufferA[4096];
        char bufferB[4096];
        while (this->__int_8 == 2) {
            memset(bufferA, 0, 4095);

            int recv_length = recv(this->socket, bufferA, 4095, 0); // real call is more complicated
            if (recv_length >= sizeof(bufferA)) {
                this->close();
            }
            if (recv_length > 0) {
                memcpy(bufferB, bufferA, recv_length);
                if (recv_length > sizeof(bufferB)) {
                    // abort or something?
                }
                bufferB[recv_length] = '\0';
                this->__recv_str += bufferB;

                while (true) {
                    size_t i = 0; // LOCAL 2069
                    const char* recv_read = this->__recv_str.c_str(); // LOCAL 2068

                    while (
                        *recv_read != '\0' &&
                        *recv_read != '\n'
                    ) {
                        ++i;
                        ++recv_read;
                    }

                    if (*recv_read == '\n') {
                        std::string message = this->__recv_str.substr(0, i);
                        this->__recv_str = this->__recv_str.substr(i + 1);
                        if (isprint(*message.c_str())) {
                            this->__queue_458.push(message);
                        }
                        this->__parse_recv();
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }

	// Rx6C40
	void __parse_recv() {
        if (this->__int_8 == 2) {
            char* buffer = new char[655535]; // This looks like an extra 5 as a typo

            char*& buffer_ref = buffer; // LOCAL 81

            memset(buffer, 0, 65535);

            while (this->__queue_458.size() > 0) {

                std::string local11{ this->__queue_458.front() };
                this->__queue_458.pop();

                memcpy(buffer_ref, local11.c_str(), local11.length());

                if (this->__string_320 == "") {
                    // send NICK/PASS/USER
                }

                char* buffer_ptrA = buffer_ref; // local82
                char* buffer_ptrB = buffer_ref; // local80

                while (*buffer_ptrA != '\0') {
                    if (
                        *buffer_ptrB == '\r' ||
                        *buffer_ptrB == '\n' ||
                        *buffer_ptrB == '\0'
                    ) {
                        *buffer_ptrB = '\0';
                        this->__parse_message(buffer_ptrB);
                        buffer_ptrA = buffer_ptrB + 1;
                    }
                    ++buffer_ptrA;


                    if (this->__int_8 != 2) {
                        delete buffer_ref; // clean up LOCAL 81
                        return;
                    }
                }
                if (this->__int_8 != 2) {
                    delete buffer_ref; // clean up LOCAL 81
                    return;
                }
            }
            delete buffer_ref; // clean up LOCAL 81
        }
	}

    // Rx7230
    void __parse_message(const char* message) {
        if (!message || !message[0]) {
            return;
        }
        if (this->__int_8 != 2) {
            return;
        }

        std::vector<std::string> message_parts;

        // make stringstream from message
        for (;;) {
            // call getline? and append strings to vector
        }

        if (
            message_parts.size() > 3 &&
            message_parts[0].length() > 0 &&
            *message_parts[0].c_str() == ':'
        ) {
            int code = atoi(message_parts[1].c_str());
            switch (code) {
                case ERR_NICKNAMEINUSE: {
                    // std::string nick_str = "NICK " + ???
                    // send str
                    break;
                }
                case RPL_ENDOFMOTD: case ERR_NOMOTD: {
                    if (this->__lobby_nameA != "") {
                        std::string join_str = "JOIN " + this->__lobby_nameA + (this->__lobby_nameB == "" ? ""s : " "s + this->__lobby_nameB) + "\r\n";
                        // send str

                        // ???
                    }
                    break;
                }
                default:
                    if (code >= 400 && code < 500) {
                        this->__byte_43C = 1;
                        this->__int_8 = 4;
                        return;
                    }
                    break;
            }
        }

        std::string message_nick; // LOCAL 10
        std::string message_host; // LOCAL 16
        if (message_parts.size() > 0) {
            message_nick = message_parts[0];
            size_t offset = message_nick.find("!");
            if (offset != std::string::npos) {
                message_nick = message_nick.substr(message_nick[0] == ':', offset);
            }
            else {
                message_nick = "";
            }
            message_host = message_parts[0];
            offset = message_host.find("@");
            if (offset != std::string::npos) {
                message_host = message_host.substr(offset + 1, message_host.length());
            }
            else {
                message_host = "";
            }
        }

        if (
            message_parts.size() > 2 &&
            message_parts[1] == "JOIN" &&
            message_nick == this->current_nick
        ) {
            this->current_channel = message_parts[2];
            this->__lobby_nameA = "";

            if (this->current_channel[0] == ':') {
                this->current_channel = this->current_channel.substr(1, this->current_channel.length());
            }
        }

        if (
            message_parts.size() > 2 &&
            message_parts[0].find("!") == std::string::npos &&
            message_parts[0].find("@") == std::string::npos &&
            message_parts[1] == "MODE" &&
            message_parts[2] == this->current_channel
        ) {
            std::string mode_str = "MODE" + this->current_channel + " +snt" + (this->__lobby_nameB == "" ? ""s : "+k " + this->__lobby_nameB) + "\r\n";
            // send str
        }

        if (
            message_parts.size() > 0 &&
            message_parts[0] == "PING"
        ) {
            // send "PONG 0\r\n"
        }

        if (
            message_parts.size() > 0 &&
            message_parts[0] == "ERROR"
        ) {
            this->__int_8 = 4;
            return;
        }

        if (
            message_parts.size() > 2 &&
            message_parts[1] == "PRIVMSG" &&
            (
                message_parts[2] == this->current_channel ||
                message_parts[2] == ":" + this->current_channel
            )
        ) {
            std::string temp{ message };

            size_t offset = 0;
            offset += message_parts[0].length() + 1;
            offset += message_parts[1].length() + 1;
            offset += message_parts[2].length() + 1;

            if (temp[offset] == ':') {
                ++offset;
            }

            temp = temp.substr(offset, temp.length());

            this->__parse_privmsg(message_nick.c_str(), message_host.c_str(), temp.c_str());
        }
    }

    // Rx804A
    void __parse_privmsg(const char* nick, const char* host, const char* message) {
        if (!nick || !nick[0]) {
            return;
        }
        if (!host || !host[0]) {
            return;
        }
        if (!message || !message[0]) {
            return;
        }

        std::vector<std::string> message_parts;

        // make stringstream from message
        for (;;) {
            // call getline? and append strings to vector
        }

        if (
            message_parts.size() > 4 &&
            message_parts[0] == "STAT" &&
            message_parts[4] == this->version_signature
        ) {
            StatParams stat_params; // LOCAL 540
            stat_params.peer_state = atoi(message_parts[1].c_str());
            stat_params.__param_2 = atoi(message_parts[2].c_str());
            stat_params.__param_3 = atoi(message_parts[3].c_str());
            stat_params.__time_8 = timeGetTime();
            stat_params.nick_string = nick;
            stat_params.host_string = host;
            stat_params.__int_44 = 1000;
            if (message_parts.size() > 5) {
                stat_params.__int_44 = atoi(message_parts[5].c_str());
            }
        }

        if (
            this->get_lobby_user_state() == 100 &&
            message_parts.size() > 2 &&
            message_parts[0] == "REQUEST_MATCH" &&
            message_parts[1] == this->current_nick
        ) {
            int current_time = timeGetTime();
            int time_since_last = current_time - this->__time_418;
            if (!this->__byte_420) {

                char buffer[2048] = "";

                sprintf(buffer,
                    "PRIVMSG %s : WELCOME %s %d %s\r\n",
                    this->current_channel.c_str(), nick, this->external_port, this->user_data_str.c_str()
                );

                // send buffer

                this->__time_418 = current_time;
                this->__byte_420 = 1;

                this->__string_424 = nick;
                this->__match_user_data_str = message_parts[2];
            }
        }

        if (
            this->get_lobby_user_state() == 200 &&
            this->match_host != "" &&
            message_parts.size() > 3 &&
            message_parts[0] == "WELCOME" &&
            message_parts[1] == this->current_nick
        ) {
            this->match_host = std::string(host) + ":" + message_parts[2];
            this->__match_user_data_str = message_parts[3];
            this->set_lobby_user_state(0);
            this->__string_2D8 = host;
        }

        if (
            this->get_lobby_user_state() == 100 &&
            this->match_host != "" &&
            message_parts.size() > 3 &&
            message_parts[0] == "WELCOME" &&
            message_parts[1] == this->current_nick
        ) {
            this->match_host = std::string(host) + ":" + message_parts[2];
            this->__match_user_data_str = message_parts[3];
            this->__string_2D8 = host;
        }
    }

    // Rx5B00
    void __sub_r5B00() {
        while (this->__int_8 == 2) {
            if (this->current_channel != "") {

                int state = this->get_lobby_user_state();

                // a whole bunch of conditions

                std::string message;
                message += "PRIVMSG";
                message += " " + this->current_channel;
                message += " :STAT";
                message += " "s + std::to_string(state);
                message += " "s + std::to_string(0); // IDK what this value actually is

            }
            else {
                Sleep(50);
            }
        }
    }
};