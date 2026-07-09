#pragma once
#include <vector>

template<typename T>
void multiplyMatrices_baseline(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension);

template<typename T>
void multiplyMatrices_loopReordered(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension);

template<typename T>
void multiplyMatrices_cacheTiling(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension, size_t tileSize);

template<typename T>
void multiplyMatrices_multiThreaded(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension, size_t numThreads);

template<typename T>
void multiplyMatrices_SIMD(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension);

template<typename T>
void multiplyMatrices_multiThreaded_SIMD(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension, size_t numThreads);
