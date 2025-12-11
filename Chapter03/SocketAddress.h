#pragma once

#include <cstdint>
#include <memory>

#include <WinSock2.h>

class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& inSockAddr);

	size_t GetSize() const;

private:
	sockaddr_in* GetAsSockAddrIn();

private:
	sockaddr sockAddr;
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;