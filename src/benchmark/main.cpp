#include <iostream>
#include <cstdlib>
#include <thread>

#include "Timer.hpp"
#include "VectorMath.hpp"
#include "BenchMarkReporter.hpp"
#include "MatrixMath.hpp"
#include "MemoryArena.hpp"
#include "ImageUtils.hpp"
#include "ImageGrayscale.hpp"
#include "Sorting.hpp"
#include "FileIO.hpp"

void runVectorBenchmarks()
{
	Timer timer;
	BenchMarkReporter reporter;

	std::vector<size_t> testSizes = { 1000, 10000, 100000, 1000000, 10000000 };

	for (size_t size : testSizes)
	{
		std::vector<int> a(size, 1);
		std::vector<int> b(size, 2);
		std::vector<int> result(size, 0);

		// Baseline
		timer.Start();
		auto result_baseline = addVectors_baseline(a, b);
		timer.Stop();

		double duration_baseline = timer.elapsedMicroseconds();
		reporter.addRecords("VectorAdd_Baseline", size, duration_baseline);

		//Reserving memory in vector
		timer.Start();
		auto result_reserving = addVectors_reserving(a, b);
		timer.Stop();

		double duration_reserving = timer.elapsedMicroseconds();
		reporter.addRecords("VectorAdd_Reserving", size, duration_reserving);

		//using preallocated vector and just putting values in it
		timer.Start();
		addVectors_prealloc(a, b, result);
		timer.Stop();

		double duration_prealloc = timer.elapsedMicroseconds();
		reporter.addRecords("VectorAdd_Prealloc", size, duration_prealloc);


	}

	std::cout << "Saving to csv...\n";
	reporter.saveToCsv("../../../results/Vector/vector_results.csv");

	std::cout << "Visualizing..\n";
	std::system("python \"../../../scripts/results_vector_plot.py\"");

	std::cout << "Vector BenchMarking completed\n";
}

void runMatrixBenchmarks()
{
	Timer timer;
	BenchMarkReporter reporter;

	std::vector<size_t> testSizes = { 250, 500, 750, 1000, 1500 };

	for (size_t N : testSizes)
	{
		size_t tileSize = 64;
		std::vector<int> matrixA(N * N, 4);
		std::vector<int> matrixB(N * N, 2);
		std::vector<int> result(N * N, 0);

		timer.Start();
		multiplyMatrices_baseline(matrixA, matrixB, result, N);
		timer.Stop();

		double duration_baseline = timer.elapsedMicroseconds();
		reporter.addRecords("MultiplyMatrices_Baseline", N * N, duration_baseline);


		// clearing out result vector for next implementation
		std::fill(result.begin(), result.end(), 0);

		timer.Start();
		multiplyMatrices_loopReordered(matrixA, matrixB, result, N);
		timer.Stop();

		double duration_loopReordered = timer.elapsedMicroseconds();
		reporter.addRecords("MultiplyMatrices_LoopReordered", N * N, duration_loopReordered);


		// clearing out results again
		std::fill(result.begin(), result.end(), 0);

		timer.Start();
		multiplyMatrices_cacheTiling(matrixA, matrixB, result, N, tileSize);
		timer.Stop();

		double duration_cacheTiling = timer.elapsedMicroseconds();
		reporter.addRecords("MultiplyMatrices_cacheTiling", N * N, duration_cacheTiling);

	}

	std::cout << "Saving to Csv...\n";
	reporter.saveToCsv("../../../results/Matrix/Matrix_results.csv");

	std::cout << "Visualizing..\n";
	std::system("python \"../../../scripts/results_matrix_plot.py\"");

	std::cout << "Matrix BenchMarking completed\n";

}

void runThreadingBenchmarks()
{
	Timer timer;
	BenchMarkReporter reporter;

	size_t numThreads = std::thread::hardware_concurrency();
	std::cout << "Threads: " << numThreads << std::endl;

	std::vector<size_t> testSizes = { 250, 500, 750, 1000, 1500, 2000 };

	for (size_t N : testSizes)
	{
		std::vector<int> matrixA(N * N, 2);
		std::vector<int> matrixB(N * N, 4);
		std::vector<int> result(N * N, 0);

		timer.Start();
		multiplyMatrices_multiThreaded(matrixA, matrixB, result, N, numThreads);
		timer.Stop();

		double duration_multiThreaded = timer.elapsedMicroseconds();

		reporter.addRecords("MultiplyMatrices_MultiThreading", N*N, duration_multiThreaded);

	}


	std::cout << "Saving to Csv...\n";
	reporter.saveToCsv("../../../results/Matrix/Threading_results.csv");

	std::cout << "Visualizing..\n";
	std::system("python \"../../../scripts/results_threading_plot.py\"");

	std::cout << "Threading BenchMarking completed\n";

	
}

void runSIMDBenchmarks()
{
	Timer timer;

	BenchMarkReporter reporter;

	//perfect multiple of 8 due to usage of AVX2 registers
	std::vector<size_t> testSizes = { 256, 512, 768, 1000, 1536, 2000};
	size_t numThreads = std::thread::hardware_concurrency();

	for (size_t N : testSizes)
	{
		std::vector<int> matrixA(N * N, 2);
		std::vector<int> matrixB(N * N, 4);
		std::vector<int> result(N * N, 0);

		timer.Start();
		multiplyMatrices_SIMD(matrixA, matrixB, result, N);
		timer.Stop();

		double duration_SIMD = timer.elapsedMicroseconds();
		reporter.addRecords("MatrixMultiply_SIMD_1Core", N*N, duration_SIMD);


		std::fill(result.begin(), result.end(), 0);



		timer.Start();
		multiplyMatrices_multiThreaded_SIMD(matrixA, matrixB, result, N, numThreads);
		timer.Stop();

		double duration_multiThreaded_SIMD = timer.elapsedMicroseconds();
		reporter.addRecords("MatrixMultiply_SIMD_MultiThreaded", N*N, duration_multiThreaded_SIMD);
	}

	std::cout << "Saving to CSV...\n";
	reporter.saveToCsv("../../../results/Matrix/SIMD_results.csv");

	std::cout << "Visualizing...\n";
	std::system("python \"../../../scripts/results_SIMD_plot.py\"");

	std::cout << "SIMD BenchMarking completed\n";

	
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

		reporter.addRecords("MatrixMultiply_MemoryArena_SIMD", N * N, duration_Arena);

		arena.reset();

	}

	std::cout << "Saving to CSV...\n";
	reporter.saveToCsv("../../../results/Matrix/Memory_Arena_results.csv");

	std::cout << "Visualizing...\n";
	std::system("python \"../../../scripts/results_Arena_plot.py\"");

	std::cout << "Memory Arena Benchmarking competed..\n";
}


void MatrixMathCompiledPlotting_BestAlgo()
{
	std::cout << "<=================================================>\n";
	std::cout << "==Visualizing Compiled Matrix Math Plot For Best Algo==\n";
	std::cout << "<=================================================>\n";

	std::system("python \"../../../scripts/results_MatrixMath_Final_plot_BestAlgo.py\"");
}

void CompiledPlotting()
{
	std::cout << "<=================================================>\n";
	std::cout << "=================Compiled Plotting================\n";
	std::cout << "<=================================================>\n";

	std::system("python \"../../../scripts/Compiled_Plotting.py\"");
}

void runImageBenchmarks()
{
	Timer timer;
	BenchMarkReporter reporter;

	Image img = loadImage("../../../assets/test_image.png");

	timer.Start();
	grayscale_baseline(img);
	timer.Stop();

	double duration_baseline = timer.elapsedMicroseconds();

	saveImage("../../../assets/output_baseline.png", img);
	reporter.addRecords("ImageGrayscaling_Baseline", img.height * img.width, duration_baseline);

	img = loadImage("../../../assets/test_image.png");

	timer.Start();
	grayscale_optimized(img);
	timer.Stop();

	double duration_optimized = timer.elapsedMicroseconds();

	saveImage("../../../assets/output_optimized.png", img);
	reporter.addRecords("ImageGrayscaling_Optimized", img.height * img.width, duration_optimized);

	img = loadImage("../../../assets/test_image.png");

	timer.Start();
	grayscale_multithreaded(img, std::thread::hardware_concurrency());
	timer.Stop();

	double duration_multiThreaded = timer.elapsedMicroseconds();
	
	saveImage("../../../assets/output_multiThreaded.png", img);
	reporter.addRecords("ImageGrayscaling_MultiThreaded", img.height * img.width, duration_multiThreaded);

	freeImage(img);
	
	std::cout << "Saving to CSV...\n";
	reporter.saveToCsv("../../../results/Image/ImageGrayscaling_results.csv");

	std::cout << "Visualizing...\n";
	std::system("python \"../../../scripts/results_ImageGrayscaling.py\"");
	
	std::cout << "Image Grayscaling Benchmarking completed...\n";

}

void runSortingBenchmarks()
{
	Timer timer;

	BenchMarkReporter reporter;

	size_t size = 100000000;

	std::vector<int> array = generateRandomArray<int>(size);

	timer.Start();
	sort_Baseline(array);
	timer.Stop();

	double duration_baseline = timer.elapsedMicroseconds();

	reporter.addRecords("Sorting_Baseline", size, duration_baseline);


	array = generateRandomArray<int>(size);

	timer.Start();
	sort_Radix(array);
	timer.Stop();

	double duration_Radix = timer.elapsedMicroseconds();

	reporter.addRecords("Radix_Sort", size, duration_Radix);

	std::cout << "Saving to CSV...\n";
	reporter.saveToCsv("../../../results/Sorting/Sorting_results.csv");

	std::cout << "Visualizing...\n";
	std::system("python \"../../../scripts/results_Sorting_plot.py\"");

	std::cout << "Sorting Benchmarking completed...\n";

}

void generateMassivefile()
{
	std::string filepath = "../../../assets/Massive_data.txt";

	size_t size = 1024; //MB

	generateMassivefile(filepath, size);

}

void runIOBenchmarks()
{
	Timer timer;
	BenchMarkReporter reporter;

	std::string filepath = "../../../assets/Massive_data.txt";

	timer.Start();
	size_t totalLines = parseFile_baseline(filepath);
	timer.Stop();

	double duration_baseline = timer.elapsedMicroseconds();

	reporter.addRecords("ParseFile_Baseline", 1024 * 1024 * 1024, duration_baseline);


	timer.Start();
	totalLines = parseFile_mmap(filepath);
	timer.Stop();

	double duration_mmap = timer.elapsedMicroseconds();

	reporter.addRecords("ParseFile_MMAP", 1024*1024*1024, duration_mmap);

	std::cout << "Saving to CSV...\n";
	reporter.saveToCsv("../../../results/IO/IO_results.csv");

	std::cout << "Visualizing...\n";
	std::system("python \"../../../scripts/results_io_plot.py\"");

	std::cout << "IO Benchmarking completed...\n";

}

int main()
{
	std::cout << "C++ Benchmark Lab initiated..\n";

	runVectorBenchmarks();
	runMatrixBenchmarks();
	runThreadingBenchmarks();
	runSIMDBenchmarks();
	runArenaBenchmarks();
	MatrixMathCompiledPlotting_BestAlgo();
	CompiledPlotting();
	runImageBenchmarks();
	runSortingBenchmarks();
	runIOBenchmarks();

	std::cout << "Benchmarking Finished..\n";

}