#include "BenchMarkReporter.hpp"
#include <fstream>
#include <iostream>

void BenchMarkReporter::addRecords(const std::string& name, size_t size, double time)
{
	m_Records.emplace_back(name, size, time);
}

void BenchMarkReporter::saveToCsv(const std::string& filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "Error: unable to open file...\n";
		return;
	}

	file << "Algorithm, InputSize, TimeMicroseconds\n";

	for (const auto& [name, size, time] : m_Records)
	{
		file << name;
		file << ",";
		file << size;
		file << ",";
		file << time;
		file << "\n";
	}

	file.close();
}