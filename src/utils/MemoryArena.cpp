#include "MemoryArena.hpp"

MemoryArena::MemoryArena(size_t size)
	: capacity(size), offset(0)
{
	buffer = static_cast<char*>(std::malloc(size));

	if (buffer == nullptr)
	{
		throw std::bad_alloc();
	}
}

MemoryArena::~MemoryArena()
{
	std::free(buffer);
}

void* MemoryArena::allocate(size_t bytes)
{
	if (offset + bytes > capacity)
	{
		throw std::bad_alloc();
	}

	void* currentPtr = buffer + offset;

	offset += bytes;

	return currentPtr;
}

void MemoryArena::reset()
{
	offset = 0;
}