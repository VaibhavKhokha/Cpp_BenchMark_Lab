#include "MatrixMath.hpp"
#include <immintrin.h>
#include <thread>
#include <vector>

template<>
void multiplyMatrices_MemoryArena_SIMD(const int* firstMatrix, const int* secondMatrix, int* resultMatrix, size_t dimension, size_t numThreads)
{
	size_t N = dimension;

	size_t chunk = N / numThreads;

	std::vector<std::thread> threads;

	for (int t = 0; t< numThreads; t++)
	{
		size_t startRow = t * chunk;

		size_t endRow = (t == numThreads-1) ? N : (startRow + chunk);

		threads.emplace_back([&, startRow, endRow]() {
			for (size_t i = startRow; i < endRow; i++)
			{
				for (size_t k = 0; k < N; k++)
				{
					int a_ik = firstMatrix[i * N + k];

					__m256i vecA = _mm256_set1_epi32(a_ik);

					for (size_t j = 0; j < N; j += 8)
					{
						__m256i vecB = _mm256_loadu_si256((__m256i*) & secondMatrix[k * N + j]);
						__m256i result = _mm256_loadu_si256((__m256i*)& resultMatrix[i * N + j]);

						__m256i temp = _mm256_mullo_epi32(vecA, vecB);

						result = _mm256_add_epi32(result, temp);
						_mm256_storeu_si256((__m256i*)& resultMatrix[i * N + j], result);
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


void multiplyMatrices_MemoryArena_SIMD(const int* firstMatrix, const int* secondMatrix, int* resultMatrix, size_t dimension, size_t numThreads);