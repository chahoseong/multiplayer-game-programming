#pragma once

#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

#include "ByteSwap.h"

class GameObject;
class LinkingContext;

class OutputMemoryStream
{
public:
	OutputMemoryStream();
	~OutputMemoryStream();

	const char* GetBufferPtr() const;
	uint32_t GetLength() const;

	void Write(const void* inData, size_t inByteCount);

	template <typename T>
	void Write(T inData)
	{
		static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic Write only supports primitive data types");

		if (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS) {
			Write(&inData, sizeof(inData));
		}
		else {
			T swappedData = ByteSwap(inData);
			Write(&swappedData, sizeof(swappedData));
		}
	}

	void Write(const std::vector<int>& inIntVector);

	template <typename T>
	void Write(const std::vector<T>& inVector)
	{
		uint32_t elementCount = inVector.size();
		Write(elementCount);
		for (const T& element : inVector) {
			Write(element);
		}
	}

	void Write(const std::string& inString);

	void Write(const GameObject* inGameObject);

private:
	void ReallocBuffer(uint32_t inNewLength);

private:
	char* buffer;
	uint32_t head;
	uint32_t capacity;
	LinkingContext* linkingContext;
};

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount);
	~InputMemoryStream();

	uint32_t GetRemainingDataSize() const;
	
	void Read(void* outData, uint32_t inByteCount);

	template <typename T>
	void Read(T& outData)
	{
		static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic Read only supports primitive data types");
		Read(&outData, sizeof(outData));
	}

	template <typename T>
	void Read(std::vector<T>& outVector)
	{
		size_t elementCount;
		Read(elementCount);
		outVector.resize(elementCount);
		for (const T& element : outVector) {
			Read(element);
		}
	}

	void Read(GameObject*& outGameObject);

private:
	char* buffer;
	uint32_t head;
	uint32_t capacity;
	LinkingContext* linkingContext;
};