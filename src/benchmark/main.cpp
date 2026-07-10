#include <iostream>
#include <cstdlib>
#include <thread>

#include "Timer.hpp"
#include "VectorMath.hpp"
#include "BenchMarkReporter.hpp"
#include "MatrixMath.hpp"
#include "MemoryArena.hpp"

void runVectorBenchmarks()
{
	Timer timer_baseline;
	Timer timer_reserving;
	Timer timer_prealloc;
	BenchMarkReporter reporter;

	std::vector<size_t> testSizes = { 1000, 10000, 100000, 1000000, 10000000 };

	for (size_t size : testSizes)
	{
		std::vector<int> a(size, 1);
		std::vector<int> b(size, 2);
		std::vector<int> result(size, 0);

		// Baseline
		timer_baseline.Start();
		auto result_baseline = addVectors_baseline(a, b);
		timer_baseline.Stop();

		double duration_baseline = timer_baseline.elapsedMicroseconds();
		reporter.addRecords("VectorAdd_Baseline", size, duration_baseline);

		//Reserving memory in vector
		timer_reserving.Start();
		auto result_reserving = addVectors_reserving(a, b);
		timer_reserving.Stop();

		double duration_reserving = timer_reserving.elapsedMicroseconds();
		reporter.addRecords("VectorAdd_Reserving", size, duration_reserving);

		//using preallocated vector and just putting values in it
		timer_prealloc.Start();
		addVectors_prealloc(a, b, result);
		timer_prealloc.Stop();

		double duration_prealloc = timer_prealloc.elapsedMicroseconds();
		reporter.addRecords("VectorAdd_Prealloc", size, duration_prealloc);


	}

	std::cout << "Saving to csv...\n";
	reporter.saveToCsv("../../../results/vector_results.csv");

	std::cout << "Visualizing..\n";
	std::system("python \"../../../scripts/results_vector_plot.py\"");

	std::cout << "Vector BenchMarking completed\n";
}

void runMatrixBenchmarks()
{
	Timer timer_baseline;
	Timer timer_loopReordered;
	Timer timer_cacheTilling;
	BenchMarkReporter reporter;

	std::vector<size_t> testSizes = { 250, 500, 750, 1000, 1500 };

	for (size_t N : testSizes)
	{
		size_t tileSize = 64;
		std::vector<int> matrixA(N * N, 4);
		std::vector<int> matrixB(N * N, 2);
		std::vector<int> result(N * N, 0);

		timer_baseline.Start();
		multiplyMatrices_baseline(matrixA, matrixB, result, N);
		timer_baseline.Stop();

		double duration_baseline = timer_baseline.elapsedMicroseconds();
		reporter.addRecords("MultiplyMatrices_Baseline", N * N, duration_baseline);


		// clearing out result vector for next implementation
		std::fill(result.begin(), result.end(), 0);

		timer_loopReordered.Start();
		multiplyMatrices_loopReordered(matrixA, matrixB, result, N);
		timer_loopReordered.Stop();

		double duration_loopReordered = timer_loopReordered.elapsedMicroseconds();
		reporter.addRecords("MultiplyMatrices_LoopReordered", N * N, duration_loopReordered);


		// clearing out results again
		std::fill(result.begin(), result.end(), 0);

		timer_cacheTilling.Start();
		multiplyMatrices_cacheTiling(matrixA, matrixB, result, N, tileSize);
		timer_cacheTilling.Stop();

		double duration_cacheTiling = timer_cacheTilling.elapsedMicroseconds();
		reporter.addRecords("MultiplyMatrices_cacheTiling", N * N, duration_cacheTiling);

	}

	std::cout << "Saving to Csv...\n";
	reporter.saveToCsv("../../../results/Matrix_results.csv");

	std::cout << "Visualizing..\n";
	std::system("python \"../../../scripts/results_matrix_plot.py\"");

	std::cout << "Matrix BenchMarking completed\n";

}

void runThreadingBenchmarks()
{
	Timer timer_multiThreaded;
	BenchMarkReporter reporter;

	size_t numThreads = std::thread::hardware_concurrency();
	std::cout << "Threads: " << numThreads << std::endl;

	std::vector<size_t> testSizes = { 250, 500, 750, 1000, 1500, 2000 };

	for (size_t N : testSizes)
	{
		std::vector<int> matrixA(N * N, 2);
		std::vector<int> matrixB(N * N, 4);
		std::vector<int> result(N * N, 0);

		timer_multiThreaded.Start();
		multiplyMatrices_multiThreaded(matrixA, matrixB, result, N, numThreads);
		timer_multiThreaded.Stop();

		double duration_multiThreaded = timer_multiThreaded.elapsedMicroseconds();

		reporter.addRecords("MultiplyMatrices_MultiThreading", N*N, duration_multiThreaded);

	}


	std::cout << "Saving to Csv...\n";
	reporter.saveToCsv("../../../results/Threading_results.csv");

	std::cout << "Visualizing..\n";
	std::system("python \"../../../scripts/results_threading_plot.py\"");

	std::cout << "Threading BenchMarking completed\n";

	
}

void runSIMDBenchmarks()
{
	Timer timer_SIMD;
	Timer timer_multiThreaded_SIMD;
	BenchMarkReporter reporter;

	//perfect multiple of 8 due to usage of AVX2 registers
	std::vector<size_t> testSizes = { 256, 512, 768, 1000, 1536, 2000};
	size_t numThreads = std::thread::hardware_concurrency();

	for (size_t N : testSizes)
	{
		std::vector<int> matrixA(N * N, 2);
		std::vector<int> matrixB(N * N, 4);
		std::vector<int> result(N * N, 0);

		timer_SIMD.Start();
		multiplyMatrices_SIMD(matrixA, matrixB, result, N);
		timer_SIMD.Stop();

		double duration_SIMD = timer_SIMD.elapsedMicroseconds();
		reporter.addRecords("MatrixMultiply_SIMD_1Core", N*N, duration_SIMD);


		std::fill(result.begin(), result.end(), 0);



		timer_multiThreaded_SIMD.Start();
		multiplyMatrices_multiThreaded_SIMD(matrixA, matrixB, result, N, numThreads);
		timer_multiThreaded_SIMD.Stop();

		double duration_multiThreaded_SIMD = timer_multiThreaded_SIMD.elapsedMicroseconds();
		reporter.addRecords("MatrixMultiply_SIMD_MultiThreaded", N*N, duration_multiThreaded_SIMD);
	}

	std::cout << "Saving to CSV...\n";
	reporter.saveToCsv("../../../results/SIMD_results.csv");

	std::cout << "Visualizing...\n";
	std::system("python \"../../../scripts/results_SIMD_plot.py\"");

	std::cout << "SIMD BenchMarking completed\n";

	
}

void MatrixMathCompiledPlotting()
{
	std::cout << "<=================================================>\n";
	std::cout << "======Visualizing Compiled Matrix Math Plot========\n";
	std::cout << "<=================================================>\n";

	std::system("python \"../../../scripts/results_MatrixMath_Final_plot.py\"");
}

void runArenaBenchmarks()
{
	//500 MB memory allocated
	size_t memory = 500 * 1024 * 1024;
	MemoryArena arena(memory);

	Timer timer_Arena;
	BenchMarkReporter reporter;

	std::vector<size_t> testSizes = { 256, 512, 1024, 2048 };
	size_t numThreads = std::thread::hardware_concurrency();

	for (size_t N : testSizes)
	{
		int* matrixA = static_cast<int*>(arena.allocate(N * N * sizeof(int))); // as it is int --> 4bytes
		std::fill_n(matrixA, N * N, 2);

		int* matrixB = static_cast<int*>(arena.allocate(N * N * sizeof(int)));
		std::fill_n(matrixB, N * N, 4);

		int* result = static_cast<int*>(arena.allocate(N * N * sizeof(int)));
		std::fill_n(result, N * N, 0);

		timer_Arena.Start();
		multiplyMatrices_MemoryArena_SIMD(matrixA, matrixB, result, N, numThreads);
		timer_Arena.Stop();

		double duration_Arena = timer_Arena.elapsedMicroseconds();

		reporter.addRecords("MatrixMultiply_MemoryArena_SIMD", N*N, duration_Arena);

		arena.reset();

	}

	std::cout << "Saving to CSV...\n";
	reporter.saveToCsv("../../../results/Memory_Arena_results.csv");

	std::cout << "Visualizing...\n";
	std::system("python \"../../../scripts/results_Arena_plot.py\"");
}

int main()
{
	runArenaBenchmarks();

}