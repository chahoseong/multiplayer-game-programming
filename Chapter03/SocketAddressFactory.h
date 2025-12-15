#pragma once

#include "SocketAddress.h"
#include <string>

class SocketAddressFactory
{
public:
	static SocketAddressPtr CreateIPv4FromString(const std::string& inString);
};