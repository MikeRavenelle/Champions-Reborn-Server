#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

constexpr int kSendFlags = 0;

namespace Platform
{
	inline bool NetworkStartup()
	{
		WSADATA wsaData;
		return WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0;
	}

	inline int LastSocketError() { return WSAGetLastError(); }
	inline bool IsWouldBlock( int err ) { return err == WSAEWOULDBLOCK; }
}
#else
#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

using SOCKET = int;
using SOCKADDR = sockaddr;

constexpr int kSendFlags = MSG_NOSIGNAL;
constexpr SOCKET INVALID_SOCKET = -1;
constexpr int SOCKET_ERROR = -1;

inline int closesocket( SOCKET s ) { return close( s ); }

namespace Platform
{
	inline bool NetworkStartup() { return true; }
	inline int LastSocketError() { return errno; }
	inline bool IsWouldBlock( int err ) { return err == EWOULDBLOCK || err == EAGAIN; }
}
#endif
