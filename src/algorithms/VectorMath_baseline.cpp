#include "VectorMath.hpp"
#include <stdexcept>

template <typename T>
std::vector<T> addVectors_baseline(const std::vector<T>& firstVec, const std::vector<T>& secondVec)
{
	if (firstVec.size() != secondVec.size())
	{
		throw std::invalid_argument("Vector inputs should be of the same size...");
	}

	size_t size = firstVec.size();

	std::vector<T> result(size, 0);

	for (size_t i = 0; i < size; i++)
	{
		result[i] = firstVec[i] + secondVec[i];
	}

	return result;

}


template std::vector<int> addVectors_baseline<int>(const std::vector<int>& a, const std::vector<int>& b);
template std::vector<float> addVectors_baseline<float>(const std::vector<float>& a, const std::vector<float>& b);
template std::vector<double> addVectors_baseline<double>(const std::vector<double>& a, const std::vector<double>& b);
