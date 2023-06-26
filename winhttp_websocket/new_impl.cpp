

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>

#include <boost/certify/extensions.hpp>
#include <boost/certify/https_verification.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

#define HINTERNET void*
#define WINHTTP_WEB_SOCKET_BUFFER_TYPE int
#define INTERNET_PORT unsigned short

#define dllexport __declspec(dllexport)

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

struct WTF {
	net::io_context ioc;
	ssl::context ssl_ctx{ ssl::context::tlsv12_client };
	tcp::resolver resolver{ ioc };
	websocket::stream<tcp::socket> ws{ ioc };

	WTF() {
		ssl_ctx.set_verify_mode(ssl::context::verify_peer |
								ssl::context::verify_fail_if_no_peer_cert);
		ssl_ctx.set_default_verify_paths();
		boost::certify::enable_native_https_server_verification(ssl_ctx);
	}
};

dllexport HINTERNET WINAPI WinHttpOpen(LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD) {
	WTF* socket = new WTF();

}

dllexport HINTERNET WINAPI WinHttpConnect(HINTERNET, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD) {

}

dllexport HINTERNET WINAPI WinHttpOpenRequest(HINTERNET hConnect, LPCWSTR, LPCWSTR pwszObjectName, LPCWSTR, LPCWSTR, LPCWSTR FAR*, DWORD) {

}

dllexport BOOL WINAPI WinHttpSetOption(HINTERNET, DWORD, LPVOID, DWORD) {

}

dllexport BOOL WINAPI WinHttpSendRequest(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD, LPVOID, DWORD, DWORD, DWORD_PTR) {

}

dllexport BOOL WINAPI WinHttpReceiveResponse(HINTERNET hRequest, LPVOID lpReserved) {

}

dllexport HINTERNET WINAPI WinHttpWebSocketCompleteUpgrade(HINTERNET hRequest, DWORD_PTR pContext) {

}

dllexport DWORD WINAPI WinHttpWebSocketReceive(HINTERNET hWebSocket, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType) {

}

dllexport DWORD WINAPI WinHttpWebSocketSend(HINTERNET hWebSocket, WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType, PVOID pvBuffer, DWORD dwBufferLength) {

}

dllexport DWORD WINAPI WinHttpWebSocketClose(HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength) {

}

dllexport BOOL WINAPI WinHttpCloseHandle(HINTERNET hInternet) {
	
}