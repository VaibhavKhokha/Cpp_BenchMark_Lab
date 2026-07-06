#include <iostream>
#include "Timer.hpp"
#include "VectorMath.hpp"
#include "BenchMarkReporter.hpp"


int main()
{
	Timer timer_baseline;
	Timer timer_reserving;
	Timer timer_prealloc;
	BenchMarkReporter reporter;

	std::vector<int> a(1000000, 1);
	std::vector<int> b(1000000, 2);
	std::vector<int> result(1000000, 0);



	timer_baseline.Start();
	auto result_baseline = addVectors_baseline(a, b);
	timer_baseline.Stop();

	double duration_baseline = timer_baseline.elapsedMicroseconds();
	reporter.addRecords("VectorAdd_Baseline", a.size(), duration_baseline);



	timer_reserving.Start();
	auto result_reserving = addVectors_reserving(a, b);
	timer_reserving.Stop();

	double duration_reserving = timer_reserving.elapsedMicroseconds();
	reporter.addRecords("VectorAdd_reserving", a.size(), duration_reserving);
	


	timer_prealloc.Start();
	addVectors_prealloc(a, b, result);
	timer_prealloc.Stop();

	double duration_prealloc = timer_prealloc.elapsedMicroseconds();
	reporter.addRecords("VectorAdd_prealloc", a.size(), duration_prealloc);



	std::cout << "Time elapsed for baseline: " << duration_baseline << " microseconds\n";

	std::cout << "Time elapsed for prealloc: " << duration_prealloc << " microseconds\n";

	std::cout << "Time elapsed for reserving: " << duration_reserving << " microseconds\n";

	reporter.saveToCsv("C:/Users/Vaibhav Khokha/Desktop/Cpp_BenchMark_Lab/results/results.csv");
}