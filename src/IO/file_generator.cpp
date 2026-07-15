#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "FileIO.hpp"

void generateMassivefile(const std::string& filepath, size_t targetsizeMB)
{
	std::fstream file(filepath, std::ios::out);

	if (!file.is_open())
	{
		std::cerr << "Errro: couldn't open the file...\n";
		return;
	}

	size_t targetBytes = targetsizeMB * 1024 * 1024;
	size_t currentBytes = 0;

	std::string rowData = "User is Writing: 1001, 4587.89, active\n";

	size_t rowSize = rowData.size();

	while (currentBytes < targetBytes)
	{
		file << rowData;
		currentBytes += rowSize;
	}

	std::cout << "Generated: " << targetsizeMB << " MB file...\n";
	file.close();

}