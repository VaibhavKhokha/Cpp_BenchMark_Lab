#pragma once
#include <cstddef>
#include <cstdlib>
#include <iostream>

class MemoryArena
{
private:
	char* buffer;
	size_t capacity;
	size_t offset;

public:
	MemoryArena(size_t size);

	~MemoryArena();

	void* allocate(size_t bytes);

	void reset();
};