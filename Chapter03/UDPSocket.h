#pragma once

#include "SocketAddress.h"

class UDPSocket
{
	friend class SocketUtil;

public:
	~UDPSocket();

	int Bind(const SocketAddress& inBindAddress);
	int SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress);
	int ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress);

private:
	UDPSocket(SOCKET inSocket);

private:
	SOCKET sock;
};

using UDPSocketPtr = std::shared_ptr<UDPSocket>;