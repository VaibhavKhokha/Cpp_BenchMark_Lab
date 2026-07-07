#include "MatrixMath.hpp"

template <typename T>
void multiplyMatrices_loopReordered(const std::vector<T>& firstMatrix, const std::vector<T>& secondMatrix, std::vector<T>& resultMatrix, size_t dimension)
{
	size_t N = dimension;

	for (size_t i = 0; i < N; i++)
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
}


template void multiplyMatrices_loopReordered(const std::vector<int>& firstMatrix, const std::vector<int>& secondMatrix, std::vector<int>& resultMatrix, size_t dimension);
template void multiplyMatrices_loopReordered(const std::vector<float>& firstMatrix, const std::vector<float>& secondMatrix, std::vector<float>& resultMatrix, size_t dimension);
template void multiplyMatrices_loopReordered(const std::vector<double>& firstMatrix, const std::vector<double>& secondMatrix, std::vector<double>& resultMatrix, size_t dimension);

