#pragma once
#include <vector>

template<typename T>
void multiplyMatrices_baseline(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension);

template<typename T>
void multiplyMatrices_loopReordered(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension);
