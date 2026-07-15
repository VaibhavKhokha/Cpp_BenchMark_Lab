#include "FileIO.hpp"
#include <fstream>
#include <iostream>

size_t parseFile_baseline(const std::string& filepath)
{
	std::ifstream file(filepath);

	if (!file.is_open())
	{
		std::cerr << "File couldn't be opened...\n";
		return 0;
	}

	size_t lineCount = 0;

	std::string Line; //buffer

	while (std::getline(file, Line))
	{
		lineCount++;
	}

	return lineCount;

}