#include "VectorMath.hpp"
#include <stdexcept>

template <typename T>
void addVectors_prealloc(const std::vector<T>& firstVec, const std::vector<T>& secondVec, std::vector<T>& result)
{
	if (firstVec.size() != secondVec.size())
	{
		throw std::invalid_argument("Vector inputs should be of the same size...");
	}

	for (size_t i = 0; i < firstVec.size(); i++)
	{
		result[i] = firstVec[i] + secondVec[i];
	}
}


template void addVectors_prealloc<int>(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result);
template void addVectors_prealloc<float>(const std::vector<float>& a, const std::vector<float>& b, std::vector<float>& result);
template void addVectors_prealloc<double>(const std::vector<double>& a, const std::vector<double>& b, std::vector<double>& result);
