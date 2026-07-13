#include "Sorting.hpp"

#include <algorithm>
#include <random>
#include <type_traits>

template<typename T>
std::vector<T> generateRandomArray(size_t size)
{
	std::vector<T> arr(size);

	std::mt19937 rng(1337); //seed
	
	if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> dist(1, 1000000);
		for (size_t i = 0; i < size; i++)
		{
			arr[i] = dist(rng);
		}
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		std::uniform_real_distribution<T> dist(1.0, 1000000.0);
		for (size_t i = 0; i < size; i++)
		{
			arr[i] = dist(rng);
		}
	}


	return arr;
}


template<typename T>
void sort_Baseline(std::vector<T>& arr)
{
	std::sort(arr.begin(), arr.end());
}


template std::vector<int> generateRandomArray<int>(size_t size);
template void sort_Baseline(std::vector<int>& arr);

template std::vector<float> generateRandomArray<float>(size_t size);
template void sort_Baseline(std::vector<float>& arr);
