#include "SocketAddress.h"
#include "StringUtils.h"

#include <WS2tcpip.h>

SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
	GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr)
{
	memcpy(&sockAddr, &inSockAddr, sizeof(sockaddr));
}

size_t SocketAddress::GetSize() const
{
	return sizeof(sockaddr);
}

std::string SocketAddress::ToString() const
{
	const sockaddr_in* s = GetAsSockAddrIn();
	char destinationBuffer[128];
	InetNtopA(
		s->sin_family,
		const_cast<in_addr*>(&s->sin_addr),
		destinationBuffer,
		sizeof(destinationBuffer)
	);
	return StringUtils::Sprintf("%s:%d", destinationBuffer, ntohs(s->sin_port));
}

sockaddr_in* SocketAddress::GetAsSockAddrIn()
{
	return reinterpret_cast<sockaddr_in*>(&sockAddr);
}

const sockaddr_in* SocketAddress::GetAsSockAddrIn() const
{
	return reinterpret_cast<const sockaddr_in*>(&sockAddr);
}
