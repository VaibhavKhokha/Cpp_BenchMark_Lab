#include "MatrixMath.hpp"
#include <algorithm>

template<typename T>
void multiplyMatrices_cacheTiling(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension, size_t tileSize)
{
	size_t N = dimension;

	for (size_t ii = 0; ii < N; ii += tileSize)
	{
		for (size_t kk = 0; kk < N; kk += tileSize)
		{
			for (size_t jj = 0; jj < N; jj += tileSize)
			{
				for (size_t i = ii; i < std::min(ii + tileSize, N); i++)
				{
					for (size_t k = kk; k < std::min(kk + tileSize, N); k++)
					{

						T a_ik = firstMatrix[i * N + k];

						for (size_t j = jj; j < std::min(jj + tileSize, N); j++)
						{
							resultMatrix[i * N + j] += a_ik * secondMatrix[k * N + j];
						}
					}
				}
			}
		}
	}

}

template void multiplyMatrices_cacheTiling(const std::vector<int>& firstMatrix, const std::vector<int>& secondMatrix, std::vector<int>& resultMatrix, size_t dimension, size_t tileSize);
template void multiplyMatrices_cacheTiling(const std::vector<float>& firstMatrix, const std::vector<float>& secondMatrix, std::vector<float>& resultMatrix, size_t dimension, size_t tileSize);
template void multiplyMatrices_cacheTiling(const std::vector<double>& firstMatrix, const std::vector<double>& secondMatrix, std::vector<double>& resultMatrix, size_t dimension, size_t tileSize);
