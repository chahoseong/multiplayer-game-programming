#include "MemoryBitStream.h"

OutputMemoryBitStream::OutputMemoryBitStream()
	: bitHead(0), buffer(nullptr)
{
	ReallocBuffer(1500 * 8);
}

OutputMemoryBitStream::~OutputMemoryBitStream()
{
	std::free(buffer);
}

void OutputMemoryBitStream::WriteBits(uint8_t inData, uint32_t inBitCount)
{
	uint32_t newBitHead = bitHead + inBitCount;

	if (newBitHead > bitCapacity) {
		ReallocBuffer(std::max(bitCapacity * 2, newBitHead));
	}

	uint32_t byteOffset = bitHead >> 3;
	uint32_t bitOffset = bitHead & 0x7; // 2진수 111
	
	uint8_t currentMask = ~(0xff << bitOffset);
	buffer[byteOffset] = (buffer[byteOffset] & currentMask) | (inData << bitOffset);

	uint32_t bitsFreeThisByte = 8 - bitOffset;

	if (bitsFreeThisByte < inBitCount) {
		buffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	bitHead = newBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, uint32_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);

	while (inBitCount > 8) {
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0) {
		WriteBits(*srcByte, inBitCount);
	}
}

const char* OutputMemoryBitStream::GetBufferPtr() const
{
	return buffer;
}

uint32_t OutputMemoryBitStream::GetBitLength() const
{
	return bitHead;
}

uint32_t OutputMemoryBitStream::GetByteLength() const
{
	return (bitHead + 7) >> 3;
}

void OutputMemoryBitStream::WriteBytes(const void* inData, uint32_t inByteCount)
{
	WriteBits(inData, inByteCount << 3);
}

void OutputMemoryBitStream::Write(bool inData)
{
	WriteBits(&inData, 1);
}

void OutputMemoryBitStream::Write(const Vector3& inVector)
{
	Write(inVector.mX);
	Write(inVector.mY);
	Write(inVector.mZ);
}

void OutputMemoryBitStream::Write(const Quaternion& inQuat)
{
	float precision = (2.0f / 65535.0f);
	Write(ConvertToFixed(inQuat.mX, -1.0f, precision), 16);
	Write(ConvertToFixed(inQuat.mY, -1.0f, precision), 16);
	Write(ConvertToFixed(inQuat.mZ, -1.0f, precision), 16);
	Write(inQuat.mW < 0);
}

void OutputMemoryBitStream::Write(const std::string& inString)
{
	uint32_t elementCount = static_cast<uint32_t>(inString.size());
	Write(elementCount);
	for (const auto& element : inString) {
		Write(element);
	}
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitCapacity)
{
	if (!buffer) {
		buffer = static_cast<char*>(std::malloc(inNewBitCapacity >> 3));
		memset(buffer, 0, inNewBitCapacity >> 8);
	}
	else {
		char* tempBuffer = static_cast<char*>(std::malloc(inNewBitCapacity >> 3));
		memset(tempBuffer, 0, inNewBitCapacity >> 3);
		memcpy(tempBuffer, buffer, bitCapacity >> 3);
		std::free(buffer);
		buffer = tempBuffer;
	}

	bitCapacity = inNewBitCapacity;
}

InputMemoryBitStream::InputMemoryBitStream(char* inBuffer, uint32_t inBitCount)
	: buffer(inBuffer), bitCapacity(inBitCount), bitHead(0), isBufferOwner(false)
{
}

InputMemoryBitStream::InputMemoryBitStream(const InputMemoryBitStream& inOther)
	: bitCapacity(inOther.bitCapacity),
	bitHead(inOther.bitHead),
	isBufferOwner(true)
{
	int byteCount = bitCapacity / 8;
	buffer = static_cast<char*>(std::malloc(byteCount));
	memcpy(buffer, inOther.buffer, byteCount);
}

InputMemoryBitStream::~InputMemoryBitStream()
{
	if (isBufferOwner) {
		std::free(buffer);
	}
}

const char* InputMemoryBitStream::GetBufferPtr() const
{
	return buffer;
}

uint32_t InputMemoryBitStream::GetRemainingBitCount() const
{
	return bitCapacity - bitHead;
}

void InputMemoryBitStream::ReadBits(uint8_t& outData, uint32_t inBitCount)
{
	uint32_t byteOffset = bitHead >> 3;
	uint32_t bitOffset = bitHead & 0x7;

	outData = static_cast<uint8_t>(buffer[byteOffset]) >> bitOffset;

	uint32_t bitsFreeThisByte = 8 - bitOffset;

	if (bitsFreeThisByte < inBitCount) {
		outData |= static_cast<uint8_t>(buffer[byteOffset + 1]) << bitsFreeThisByte;
	}

	outData &= (~(0x00ff << inBitCount));

	bitHead += inBitCount;
}

void InputMemoryBitStream::ReadBits(void* outData, uint32_t inBitCount)
{
	uint8_t* destByte = reinterpret_cast<uint8_t*>(outData);

	while (inBitCount > 8) {
		ReadBits(*destByte, 8);
		++destByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0) {
		ReadBits(*destByte, inBitCount);
	}
}

void InputMemoryBitStream::ReadBytes(void* outData, uint32_t inByteCount)
{
	ReadBits(outData, inByteCount << 3);
}

void InputMemoryBitStream::Read(uint32_t& outData, uint32_t inBitCount)
{
	ReadBits(&outData, inBitCount);
}

void InputMemoryBitStream::Read(int& outData, uint32_t inBitCount)
{
	ReadBits(&outData, inBitCount);
}

void InputMemoryBitStream::Read(float& outData)
{
	ReadBits(&outData, 32);
}

void InputMemoryBitStream::Read(uint16_t& outData, uint32_t inBitCount)
{
	ReadBits(&outData, inBitCount);
}

void InputMemoryBitStream::Read(int16_t& outData, uint32_t inBitCount)
{
	ReadBits(&outData, inBitCount);
}

void InputMemoryBitStream::Read(uint8_t& outData, uint32_t inBitCount)
{
	ReadBits(&outData, inBitCount);
}

void InputMemoryBitStream::Read(bool& outData)
{
	ReadBits(&outData, 1);
}

void InputMemoryBitStream::Read(Quaternion& outQuat)
{
	float precision = (2.0f / 65535.0f);

	uint32_t f = 0;

	Read(f, 16);
	outQuat.mX = ConvertFromFixed(f, -1.0f, precision);
	Read(f, 16);
	outQuat.mY = ConvertFromFixed(f, -1.0f, precision);
	Read(f, 16);
	outQuat.mZ = ConvertFromFixed(f, -1.0f, precision);

	outQuat.mW = std::sqrtf(1.0f - (outQuat.mX * outQuat.mX +
									outQuat.mY * outQuat.mY +
									outQuat.mZ * outQuat.mZ));

	bool isNegative;
	Read(isNegative);

	if (isNegative) {
		outQuat.mW *= -1;
	}
}

void InputMemoryBitStream::ResetToCapacity(uint32_t inByteCapacity)
{
	bitCapacity = inByteCapacity << 3;
	bitHead = 0;
}

void InputMemoryBitStream::Read(std::string& outString)
{
	uint32_t elementCount;
	Read(elementCount);
	outString.resize(elementCount);
	for (auto& element : outString) {
		Read(element);
	}
}

void InputMemoryBitStream::Read(Vector3& outVector)
{
	Read(outVector.mX);
	Read(outVector.mY);
	Read(outVector.mZ);
}

