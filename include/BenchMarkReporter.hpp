#pragma once
#include <vector>
#include <string>

struct BenchMarkRecord
{
	std::string algorithmName;
	size_t inputsize;
	double timeMicroseconds;
};

class BenchMarkReporter
{
private:
	std::vector<BenchMarkRecord> m_Records;

public:
	void addRecords(const std::string& name, size_t size, double time);
	void saveToCsv(const std::string& filename);

};