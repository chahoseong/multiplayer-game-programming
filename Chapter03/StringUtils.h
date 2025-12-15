#pragma once

#include <string>

namespace StringUtils
{
	std::string Sprintf(const char* inFormat, ...);
	void Log(const char* inFormat, ...);
}

#define LOG(...) StringUtils::Log(__VA_ARGS__);