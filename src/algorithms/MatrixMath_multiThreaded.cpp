#include "MatrixMath.hpp"
#include <thread>

template<typename T>
void multiplyMatrices_multiThreaded(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension, size_t numThreads)
{
	size_t N = dimension;

	size_t chunk = N / numThreads; // no of rows each thread gets..

	std::vector<std::thread> threads;

	for (size_t t = 0; t < numThreads; t++)
	{
		size_t startRow = t * chunk;

		size_t endRow = (t == numThreads - 1) ? N : (startRow + chunk);

		threads.emplace_back([&, startRow, endRow]() {

			for (size_t i = startRow; i < endRow; i++)
			{
				for (size_t k = 0; k < N; k++)
				{
					T a_ik = firstMatrix[i * N + k];

					for (size_t j = 0; j < N; j++)
					{
						resultMatrix[i * N + j] += a_ik * secondMatrix[k * N + j];
					}
				}
			}

		});
		
	}

	for (auto& th : threads)
	{
		th.join();
	}
}


template void multiplyMatrices_multiThreaded(const std::vector<int>& firstMatrix, const std::vector<int>& secondMatrix, std::vector<int>& resultMatrix, size_t dimension, size_t numThreads);
template void multiplyMatrices_multiThreaded(const std::vector<float>& firstMatrix, const std::vector<float>& secondMatrix, std::vector<float>& resultMatrix, size_t dimension, size_t numThreads);
template void multiplyMatrices_multiThreaded(const std::vector<double>& firstMatrix, const std::vector<double>& secondMatrix, std::vector<double>& resultMatrix, size_t dimension, size_t numThreads);
