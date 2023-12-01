#pragma clang diagnostic ignored "-Wc++17-extensions"
#pragma clang diagnostic ignored "-Wc++20-extensions"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// Something got undefined in my cmake file or some
// crap and I'd rather just define it here than fix it
#define POINTER_64 __ptr64

#include <stdlib.h>
#include <stdint.h>
#include <utility>
#include <tuple>

#include <Windows.h>
#include <winternl.h>
#include <Rpc.h>
#include <winhttp.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#define SECURITY_WIN32
#include <security.h>
#include <schannel.h>
#include <sspi.h>

#include <windns.h>

#include <string.h>

#define JSON_USE_UTF8 1
#include "../zero/json.h"

#include "../zero/util.h"

DWORD WINAPI WinHttpWebSocketClose(HINTERNET hWebSocket, USHORT, PVOID, DWORD);

BOOL WINAPI WinHttpCloseHandle(HINTERNET hInternet) asm("_WinHttpCloseHandle");
HINTERNET WINAPI WinHttpConnect(HINTERNET hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved) asm("_WinHttpConnect");
DWORD WINAPI WinHttpWebSocketClose(HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength) asm("_WinHttpWebSocketClose");
DWORD WINAPI WinHttpWebSocketReceive(HINTERNET hWebSocket, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType) asm("_WinHttpWebSocketReceive");
DWORD WINAPI WinHttpWebSocketSend(HINTERNET hWebSocket, WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType, PVOID pvBuffer, DWORD dwBufferLength) asm("_WinHttpWebSocketSend");
HINTERNET WINAPI WinHttpWebSocketCompleteUpgrade(HINTERNET hRequest, DWORD_PTR pContext) asm("_WinHttpWebSocketCompleteUpgrade");
BOOL WINAPI WinHttpReceiveResponse(HINTERNET hRequest, LPVOID lpReserved) asm("_WinHttpReceiveResponse");
BOOL WINAPI WinHttpSendRequest(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext) asm("_WinHttpSendRequest");
HINTERNET WINAPI WinHttpOpenRequest(HINTERNET hConnect, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, LPCWSTR pwszVersion, LPCWSTR pwszReferrer, LPCWSTR FAR* ppwszAcceptTypes, DWORD dwFlags) asm("_WinHttpOpenRequest");
BOOL WINAPI WinHttpSetOption(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength) asm("_WinHttpSetOption");
HINTERNET WINAPI WinHttpOpen(LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags) asm("_WinHttpOpen");


static constexpr const char* appId = "1174872221370548245";

// I took this from the buffer size libuv uses for named pipes; I suspect ours would usually be much
// smaller.
constexpr size_t MaxRpcFrameSize = 64 * 1024;

struct BaseConnection {

    bool isOpen{ false };
    HANDLE pipe{ INVALID_HANDLE_VALUE };

    ~BaseConnection() {
        this->Close();
    }
    bool Open() {
        wchar_t pipeName[]{ L"\\\\?\\pipe\\discord-ipc-0" };
        const size_t pipeDigit = sizeof(pipeName) / sizeof(wchar_t) - 2;
        pipeName[pipeDigit] = L'0';
        for (;;) {
            this->pipe = ::CreateFileW(
                pipeName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
            if (this->pipe != INVALID_HANDLE_VALUE) {
                this->isOpen = true;
                return true;
            }

            auto lastError = GetLastError();
            if (lastError == ERROR_FILE_NOT_FOUND) {
                if (pipeName[pipeDigit] < L'9') {
                    pipeName[pipeDigit]++;
                    continue;
                }
            } else if (lastError == ERROR_PIPE_BUSY) {
                if (!WaitNamedPipeW(pipeName, 10000)) {
                    return false;
                }
                continue;
            }
            return false;
        }
    }
    bool Close() {
        ::CloseHandle(this->pipe);
        this->pipe = INVALID_HANDLE_VALUE;
        this->isOpen = false;
        return true;
    }
    bool Write(const void* data, size_t length) {
        if (length == 0) {
            return true;
        }
        if (this->pipe == INVALID_HANDLE_VALUE || !data) {
            return false;
        }
        const DWORD bytesLength = (DWORD)length;
        DWORD bytesWritten = 0;
        return ::WriteFile(this->pipe, data, bytesLength, &bytesWritten, nullptr) == TRUE &&
            bytesWritten == bytesLength;
    }
    bool Read(void* data, size_t length) {
        if (this->pipe == INVALID_HANDLE_VALUE || !data) {
            return false;
        }
        DWORD bytesAvailable = 0;
        if (::PeekNamedPipe(this->pipe, nullptr, 0, nullptr, &bytesAvailable, nullptr)) {
            if (bytesAvailable >= length) {
                DWORD bytesToRead = (DWORD)length;
                DWORD bytesRead = 0;
                if (::ReadFile(this->pipe, data, bytesToRead, &bytesRead, nullptr) == TRUE) {
                    return true;
                } else {
                    this->Close();
                }
            }
        } else {
            this->Close();
        }
        return false;
    }
};

size_t JsonWriteHandshakeObj(char* dest, size_t maxLen, int version, const char* applicationId) {
    JsonObject obj;

    obj[u8"v"] = JsonNumber(version);
    obj[u8"client_id"] = JsonString((const char8_t*)applicationId);

    std::stringstream stream;

    stream << obj;

    auto str = stream.str();

    size_t copy_count = str.copy(dest, maxLen - 1, 0);
    dest[copy_count] = '\0';

    return copy_count + 1;
}

static const int RpcVersion = 1;
struct RpcConnection {
    enum class ErrorCode : int {
        Success = 0,
        PipeClosed = 1,
        ReadCorrupt = 2,
    };

    enum class Opcode : uint32_t {
        Handshake = 0,
        Frame = 1,
        Close = 2,
        Ping = 3,
        Pong = 4,
    };

    struct MessageFrameHeader {
        Opcode opcode;
        uint32_t length;
    };

    struct MessageFrame : public MessageFrameHeader {
        char message[MaxRpcFrameSize - sizeof(MessageFrameHeader)];
    };

    enum class State : uint32_t {
        Disconnected,
        SentHandshake,
        AwaitingResponse,
        Connected,
    };

    BaseConnection connection{};
    State state{ State::Disconnected };
    void (*onConnect)(JsonObject& message) { nullptr };
    void (*onDisconnect)(int errorCode, const char* message) { nullptr };
    char appId[64]{};
    int lastErrorCode{ 0 };
    //char lastErrorMessage[256]{};
    RpcConnection::MessageFrame sendFrame;

    RpcConnection* Create(const char* applicationId) {
        strcpy(this->appId, applicationId);
        return this;
    }
    ~RpcConnection() {
        this->Close();
    }

    inline bool IsOpen() const {
        return this->state == State::Connected;
    }

    void Open() {
        if (this->state == State::Connected) {
            return;
        }

        if (this->state == State::Disconnected && !this->connection.Open()) {
            return;
        }

        if (this->state == State::SentHandshake) {
            JsonObject message;
            if (this->Read(message)) {
                if (auto* cmd = std::get_if<JsonString>(&message[u8"cmd"])) {
                    if (auto* evt = std::get_if<JsonString>(&message[u8"evt"])) {
                        if (*cmd == u8"DISPATCH" && *evt == u8"READY") {
                            this->state = State::Connected;
                            if (this->onConnect) {
                                this->onConnect(message);
                            }
                        }
                    }
                }
            }
        } else {
            this->sendFrame.opcode = Opcode::Handshake;
            this->sendFrame.length = (uint32_t)JsonWriteHandshakeObj(
                this->sendFrame.message, sizeof(this->sendFrame.message), RpcVersion, this->appId);

            if (this->connection.Write(&this->sendFrame, sizeof(MessageFrameHeader) + this->sendFrame.length)) {
                this->state = State::SentHandshake;
            } else {
                this->Close();
            }
        }
    }

    void Close() {
        if (this->onDisconnect && (this->state == State::Connected || this->state == State::SentHandshake)) {
            this->onDisconnect(this->lastErrorCode, "" /*this->lastErrorMessage*/);
        }
        //this->connection->Close();
        this->state = State::Disconnected;
    }

    bool Write(const void* data, size_t length) {
        this->sendFrame.opcode = Opcode::Frame;
        memcpy(this->sendFrame.message, data, length);
        this->sendFrame.length = (uint32_t)length;
        if (!this->connection.Write(&this->sendFrame, sizeof(MessageFrameHeader) + length)) {
            this->Close();
            return false;
        }
        return true;
    }

    bool Read(JsonObject& message) {
        if (this->state != State::Connected && this->state != State::SentHandshake) {
            return false;
        }
        MessageFrame readFrame;
        for (;;) {
            bool didRead = this->connection.Read(&readFrame, sizeof(MessageFrameHeader));
            if (!didRead) {
                if (!this->connection.isOpen) {
                    this->lastErrorCode = (int)ErrorCode::PipeClosed;
                    //strcpy(this->lastErrorMessage, "Pipe closed");
                    this->Close();
                }
                return false;
            }

            if (readFrame.length > 0) {
                didRead = this->connection.Read(readFrame.message, readFrame.length);
                if (!didRead) {
                    this->lastErrorCode = (int)ErrorCode::ReadCorrupt;
                    //strcpy(this->lastErrorMessage, "Partial data in frame");
                    this->Close();
                    return false;
                }
                readFrame.message[readFrame.length] = 0;
            }

            switch (readFrame.opcode) {
                case Opcode::Close: {
                    if (auto parsed_message = parse_json_text((char8_t*)readFrame.message)) {
                        message = parsed_message.value();
                        this->lastErrorCode = std::get<JsonNumber>(message[u8"code"]);
                        //strcpy(this->lastErrorMessage, GetStrMember(&message, "message", ""));
                    }
                    this->Close();
                    return false;
                }
                case Opcode::Frame: {
                    if (auto parsed_message = parse_json_text((char8_t*)readFrame.message)) {
                        message = parsed_message.value();
                    }
                    return true;
                }
                case Opcode::Ping:
                    readFrame.opcode = Opcode::Pong;
                    if (!this->connection.Write(&readFrame, sizeof(MessageFrameHeader) + readFrame.length)) {
                        this->Close();
                    }
                    break;
                case Opcode::Pong:
                    break;
                case Opcode::Handshake:
                default:
                    // something bad happened
                    this->lastErrorCode = (int)ErrorCode::ReadCorrupt;
                    //strcpy(this->lastErrorMessage, "Bad ipc frame");
                    this->Close();
                    return false;
            }
        }
    }
};

RpcConnection connection;


void initialize_discord() {
    connection.Create(appId);
}

int main(int argc, char* argv[]) {

}