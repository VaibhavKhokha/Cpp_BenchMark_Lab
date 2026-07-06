#include "VectorMath.hpp"

template <typename T>
std::vector<T> addVectors_reserving(const std::vector<T>& firstVec, const std::vector<T>& secondVec)
{
	size_t size = firstVec.size();

	std::vector<T> result;
	result.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		result.emplace_back(firstVec[i] + secondVec[i]);
	}

	return result;

}


template std::vector<int> addVectors_reserving<int>(const std::vector<int>& a, const std::vector<int>& b);
template std::vector<float> addVectors_reserving<float>(const std::vector<float>& a, const std::vector<float>& b);
template std::vector<double> addVectors_reserving<double>(const std::vector<double>& a, const std::vector<double>& b);
