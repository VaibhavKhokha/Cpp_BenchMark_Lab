#include <algorithm>

#include "Sorting.hpp"

template<typename T>
void countingSort(std::vector<T>& arr, int shift)
{
	size_t n = arr.size();

	std::vector<T> output(n);

	int count[256] = { 0 };

	for (int i = 0; i<n; i++)
	{
		int bucket = (arr[i] >> shift) & 0xFF;

		count[bucket]++;
	}

	for (int i = 1; i < 256; i++)
	{
		count[i] += count[i - 1];
	}

	for (int i = n - 1; i >= 0; i--)
	{
		int bucket = (arr[i] >> shift) & 0xFF;

		output[count[bucket] - 1] = arr[i];

		count[bucket]--;
	}

	arr = output;
}

template<typename T>
void sort_Radix(std::vector<T>& arr)
{
	T maxVal = *(std::max_element(arr.begin(), arr.end()));

	for (int shift = 0; (maxVal >> shift) > 0; shift += 8)
	{
		countingSort(arr, shift);
	}

}


template void sort_Radix(std::vector<int>& arr);