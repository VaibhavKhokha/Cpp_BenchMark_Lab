#pragma once
#include <vector>

template<typename T>
std::vector<T> generateRandomArray(size_t size);

template<typename T>
void sort_Baseline(std::vector<T>& arr);

template<typename T>
void sort_Radix(std::vector<T>& arr);
