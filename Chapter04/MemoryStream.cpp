#include "MemoryStream.h"
#include "LinkingContext.h"

OutputMemoryStream::OutputMemoryStream()
{
	ReallocBuffer(32);
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{
	buffer = static_cast<char*>(std::realloc(buffer, inNewLength));
	capacity = inNewLength;
}

OutputMemoryStream::~OutputMemoryStream()
{
	std::free(buffer);
}

const char* OutputMemoryStream::GetBufferPtr() const
{
	return buffer;
}

uint32_t OutputMemoryStream::GetLength() const
{
	return head;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
{
	uint32_t newHead = head + static_cast<uint32_t>(inByteCount);
	if (newHead > capacity) {
		ReallocBuffer(std::max(capacity * 2, newHead));
	}

	std::memcpy(buffer + head, inData, inByteCount);

	head = newHead;
}

void OutputMemoryStream::Write(const std::vector<int>& inIntVector)
{
	size_t elementCount = inIntVector.size();
	Write(elementCount);
	Write(inIntVector.data(), elementCount * sizeof(int));
}

void OutputMemoryStream::Write(const std::string& inString)
{
	size_t elementCount = inString.size();
	Write(elementCount);
	Write(inString.data(), elementCount * sizeof(std::string::value_type));
}

void OutputMemoryStream::Write(const GameObject* inGameObject)
{
	uint32_t networkId = linkingContext->GetNetworkId(const_cast<GameObject*>(inGameObject));
	Write(networkId);
}

InputMemoryStream::InputMemoryStream(char* inBuffer, uint32_t inByteCount)
	: buffer(buffer), capacity(inByteCount), head(0)
{
}

InputMemoryStream::~InputMemoryStream()
{
	std::free(buffer);
}

uint32_t InputMemoryStream::GetRemainingDataSize() const
{
	return capacity - head;
}

void InputMemoryStream::Read(void* outData, uint32_t inByteCount)
{
	uint32_t newHead = head + inByteCount;
	if (newHead > capacity) {
		// no data to read
		return;
	}

	std::memcpy(outData, buffer + head, inByteCount);

	head = newHead;
}

void InputMemoryStream::Read(GameObject*& outGameObject)
{
	uint32_t networkId;
	Read(networkId);
	outGameObject = linkingContext->GetGameObject(networkId);
}

