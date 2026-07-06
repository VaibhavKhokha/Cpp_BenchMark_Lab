#include <iostream>
#include <cstdlib>

#include "Timer.hpp"
#include "VectorMath.hpp"
#include "BenchMarkReporter.hpp"


int main()
{
	Timer timer_baseline;
	Timer timer_reserving;
	Timer timer_prealloc;
	BenchMarkReporter reporter;

	std::vector<size_t> testSizes = {1000, 10000, 100000, 1000000, 10000000};

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
	reporter.saveToCsv("../../../results/results.csv");

	std::cout << "Visualizing..\n";
	std::system("python \"../../../scripts/results_plot.py\"");

	std::cout << "Completed..\n";
}