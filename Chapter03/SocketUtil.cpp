#include "SocketUtil.h"
#include "StringUtils.h"

#include <WinSock2.h>

void SocketUtil::ReportError(const char* inOperationDesc)
{
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);

	LOG("Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}

int SocketUtil::Select(const std::vector<TCPSocketPtr>* inReadSet, std::vector<TCPSocketPtr>* outReadSet, const std::vector<TCPSocketPtr>* inWriteSet, std::vector<TCPSocketPtr>* outWriteSet, const std::vector<TCPSocketPtr>* inExceptSet, std::vector<TCPSocketPtr>* outExceptSet)
{
	fd_set read, write, except;

	int nfds = 0;

	fd_set* readPtr = FillSetFromVector(read, inReadSet, nfds);
	fd_set* writePtr = FillSetFromVector(write, inWriteSet, nfds);
	fd_set* exceptPtr = FillSetFromVector(except, inExceptSet, nfds);

	int toRet = select(nfds + 1, readPtr, writePtr, exceptPtr, nullptr);
	
	if (toRet > 0) {
		FillVectorFromSet(outReadSet, inReadSet, read);
		FillVectorFromSet(outWriteSet, inWriteSet, write);
		FillVectorFromSet(outExceptSet, inExceptSet, except);
	}

	return toRet;
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	SOCKET sock = socket(static_cast<int>(inFamily), SOCK_DGRAM, IPPROTO_UDP);
	if (sock != INVALID_SOCKET) {
		return UDPSocketPtr(new UDPSocket(sock));
	}
	else {
		ReportError("SocketUtil::CreateUDPSocket");
		return nullptr;
	}
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	SOCKET sock = socket(static_cast<int>(inFamily), SOCK_STREAM, IPPROTO_TCP);
	if (sock != INVALID_SOCKET) {
		return TCPSocketPtr(new TCPSocket(sock));
	}
	else {
		ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const std::vector<TCPSocketPtr>* inSockets, int& ioNaxNfds)
{
	if (inSockets) {
		FD_ZERO(&outSet);

		for (const TCPSocketPtr& socket : *inSockets) {
			FD_SET(socket->sock, &outSet);
		}
		return &outSet;
	}
	else {
		return nullptr;
	}
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets, const std::vector<TCPSocketPtr>* inSockets, const fd_set& inSet)
{
	if (inSockets && outSockets) {
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets) {
			if (FD_ISSET(socket->sock, &inSet)) {
				outSockets->push_back(socket);
			}
		}
	}
}
