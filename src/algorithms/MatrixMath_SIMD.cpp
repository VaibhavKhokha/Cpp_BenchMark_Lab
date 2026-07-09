#include "MatrixMath.hpp"
#include <immintrin.h> // intel intrinsics header
#include <vector>

template<>
void multiplyMatrices_SIMD(const std::vector<int>& firstMatrix, const std::vector<int>& secondMatrix, std::vector<int>& resultMatrix, size_t dimension)
{
	size_t N = dimension;

	for (size_t i = 0; i < N; i++)
	{
		for (size_t k = 0; k < N; k++)
		{
			int a_ik = firstMatrix[i * N + k];

			//cloning a_ik 8 times into a 256-bit register //broadcasting
			__m256i vecA = _mm256_set1_epi32(a_ik);

			for (size_t j = 0; j < N; j+=8)
			{
				__m256i vecB = _mm256_loadu_si256((__m256i*) & secondMatrix[k * N + j]);

				__m256i resultVec = _mm256_loadu_si256((__m256i*) & resultMatrix[i*N +j]);

				__m256i temp = _mm256_mullo_epi32(vecA, vecB);

				resultVec = _mm256_add_epi32(resultVec, temp);

				_mm256_storeu_si256((__m256i*)&resultMatrix[i*N + j], resultVec);

			}

		}
	}
}
