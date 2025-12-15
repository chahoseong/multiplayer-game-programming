#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <WinSock2.h>

class SocketAddress
{
	friend class UDPSocket;
	friend class TCPSocket;

public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& inSockAddr);

	size_t GetSize() const;
	std::string ToString() const;

private:
	sockaddr_in* GetAsSockAddrIn();
	const sockaddr_in* GetAsSockAddrIn() const;

private:
	sockaddr sockAddr;
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;