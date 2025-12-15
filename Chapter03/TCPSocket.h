#pragma once

#include "SocketAddress.h"
#include <memory>

class TCPSocket
{
	friend class SocketUtil;

public:
	~TCPSocket();

	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBacklog = 32);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress);
	int32_t Send(const void* inData, size_t inLen);
	int32_t Receive(void* inData, size_t inLen);

private:
	TCPSocket(SOCKET inSocket);

private:
	SOCKET sock;
};

using TCPSocketPtr = std::shared_ptr<TCPSocket>;