#include "UDPSocket.h"
#include "SocketUtil.h"
#include "StringUtils.h"

#include <WS2tcpip.h>
#include "TCPSocket.h"

UDPSocket::UDPSocket(SOCKET inSocket)
	: sock(inSocket)
{

}

UDPSocket::~UDPSocket()
{
	closesocket(sock);
}

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
	int error = bind(sock, &inBindAddress.sockAddr, static_cast<int>(inBindAddress.GetSize()));
	if (error != 0) {
		SocketUtil::ReportError("UDPSocket::Bind");
		return SocketUtil::GetLastError();
	}

	return NO_ERROR;
}

int UDPSocket::SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress)
{
	int byteSentCount = sendto(
		sock,
		static_cast<const char*>(inToSend),
		inLength,
		0,
		&inToAddress.sockAddr,
		static_cast<int>(inToAddress.GetSize())
	);

	if (byteSentCount <= 0) {
		SocketUtil::ReportError("UDPSocket::SendTo");
		return -SocketUtil::GetLastError();
	}

	return byteSentCount;
}

int UDPSocket::ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress)
{
	socklen_t fromLength = static_cast<socklen_t>(outFromAddress.GetSize());
	int readByteCount = recvfrom(
		sock,
		static_cast<char*>(inToReceive),
		inMaxLength,
		0,
		&outFromAddress.sockAddr,
		&fromLength
	);

	if (readByteCount >= 0) {
		return readByteCount;
	}

	int error = SocketUtil::GetLastError();
	if (error == WSAEWOULDBLOCK) {
		return 0;
	}
	else if (error == WSAECONNRESET) {
		LOG("Connection reset from %s", outFromAddress.ToString().c_str());
		return -WSAECONNRESET;
	}
	else {
		SocketUtil::ReportError("UDPSocket::ReceiveFrom");
		return -error;
	}
}