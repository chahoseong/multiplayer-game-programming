#include "TCPSocket.h"
#include "SocketUtil.h"
#include <WS2tcpip.h>

TCPSocket::TCPSocket(SOCKET inSocket)
	: sock(inSocket)
{
}

TCPSocket::~TCPSocket()
{
	closesocket(sock);
}

int TCPSocket::Connect(const SocketAddress& inAddress)
{
	int err = connect(sock, &inAddress.sockAddr, static_cast<int>(inAddress.GetSize()));
	if (err < 0) {
		SocketUtil::ReportError("TCPSocket::Connect");
		return -SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int TCPSocket::Bind(const SocketAddress& inToAddress)
{
	int err = bind(sock, &inToAddress.sockAddr, static_cast<int>(inToAddress.GetSize()));
	if (err != 0) {
		SocketUtil::ReportError("TCPSocket::Bind");
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int TCPSocket::Listen(int inBacklog)
{
	int err = listen(sock, inBacklog);
	if (err < 0) {
		SocketUtil::ReportError("TCPSocket::Listen");
		return -SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress& inFromAddress)
{
	socklen_t length = static_cast<int>(inFromAddress.GetSize());
	SOCKET newSocket = accept(sock, &inFromAddress.sockAddr, &length);

	if (newSocket != INVALID_SOCKET) {
		return TCPSocketPtr(new TCPSocket(newSocket));
	}
	else {
		SocketUtil::ReportError("TCPSocket::Accept");
		return nullptr;
	}
}

int32_t TCPSocket::Send(const void* inData, size_t inLen)
{
	int bytesSentCount = send(sock, static_cast<const char*>(inData), static_cast<int>(inLen), 0);
	if (bytesSentCount < 0) {
		SocketUtil::ReportError("TCPSocket::Send");
		return -SocketUtil::GetLastError();
	}
	return bytesSentCount;
}

int32_t TCPSocket::Receive(void* inData, size_t inLen)
{
	int bytesReceivedCount = recv(sock, static_cast<char*>(inData), static_cast<int>(inLen), 0);
	if (bytesReceivedCount < 0) {
		SocketUtil::ReportError("TCPSocket::Receive");
		return -SocketUtil::GetLastError();
	}
	return bytesReceivedCount;
}