#include "StringUtils.h"

#include <cstdarg>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::string StringUtils::Sprintf(const char* inFormat, ...)
{
	static char temp[4096];

	va_list args;
	va_start(args, inFormat);

	_vsnprintf_s(temp, 4096, 4096, inFormat, args);

	return std::string(temp);
}

void StringUtils::Log(const char* inFormat, ...)
{
	static char temp[4096];

	va_list args;
	va_start(args, inFormat);

	_vsnprintf_s(temp, 4096, 4096, inFormat, args);

	OutputDebugStringA(temp);
	OutputDebugStringA("\n");
}
