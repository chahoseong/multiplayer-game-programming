#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

#include "RoboMath.h"

class GameObject;

inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision)
{
	return static_cast<int>((inNumber - inMin) / inPrecision);
}

inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision)
{
	return static_cast<float>(inNumber) * inPrecision + inMin;
}

class OutputMemoryBitStream
{
public:
	OutputMemoryBitStream();
	~OutputMemoryBitStream();

	void WriteBits(uint8_t inData, uint32_t inBitCount);
	void WriteBits(const void* inData, uint32_t inBitCount);

	const char* GetBufferPtr() const;
	uint32_t GetBitLength() const;
	uint32_t GetByteLength() const;

	void WriteBytes(const void* inData, uint32_t inByteCount);

	template <typename T>
	void Write(T inData, uint32_t inBitCount = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic_v<T> ||
			std::is_enum_v<T>, "Generic Write only supports primitive data types");
		WriteBits(&inData, inBitCount);
	}

	void Write(bool inData);
	void Write(const Vector3& inVector);
	void Write(const Quaternion& inQuat);

	void Write(const std::string& inString);

private:
	void ReallocBuffer(uint32_t inNewBitCapacity);

private:
	char* buffer;
	uint32_t bitHead;
	uint32_t bitCapacity;
};

class InputMemoryBitStream
{
public:
	InputMemoryBitStream(char* inBuffer, uint32_t inBitCount);
	InputMemoryBitStream(const InputMemoryBitStream& inOther);
	~InputMemoryBitStream();

	const char* GetBufferPtr() const;
	uint32_t GetRemainingBitCount() const;

	void ReadBits(uint8_t& outData, uint32_t inBitCount);
	void ReadBits(void* outData, uint32_t inBitCount);
	void ReadBytes(void* outData, uint32_t inByteCount);

	template <typename T>
	void Read(T& inData, uint32_t inBitCount = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>,
			"Generic Read only supports primitive data types");
		ReadBits(&inData, inBitCount);
	}

	void Read(uint32_t& outData, uint32_t inBitCount = 32);
	void Read(int& outData, uint32_t inBitCount = 32);
	void Read(float& outData);

	void Read(uint16_t& outData, uint32_t inBitCount = 16);
	void Read(int16_t& outData, uint32_t inBitCount = 16);

	void Read(uint8_t& outData, uint32_t inBitCount = 8);
	void Read(bool& outData);

	void Read(Quaternion& outQuat);

	void ResetToCapacity(uint32_t inByteCapacity);

	void Read(std::string& outString);

	void Read(Vector3& outVector);

private:
	char* buffer;
	uint32_t bitHead;
	uint32_t bitCapacity;
	bool isBufferOwner;
};