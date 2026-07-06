#include "VectorMath.hpp"
#include <iostream>

template <typename T>
void addVectors_prealloc(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& result)
{
	if (a.size() != b.size())
	{
		std::cout << "size of vectors adding are not equal..\n";
		return;
	}

	for (int i = 0; i < a.size(); i++)
	{
		result[i] = a[i] + b[i];
	}
}


template void addVectors_prealloc<int>(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result);
template void addVectors_prealloc<float>(const std::vector<float>& a, const std::vector<float>& b, std::vector<float>& result);
template void addVectors_prealloc<double>(const std::vector<double>& a, const std::vector<double>& b, std::vector<double>& result);
