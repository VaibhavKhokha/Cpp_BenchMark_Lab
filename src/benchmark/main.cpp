#include <iostream>
#include "Timer.hpp"
#include "VectorMath.hpp"


int main()
{
	Timer timer_baseline;
	Timer timer_optimized;

	std::vector<int> a(1000000, 1);
	std::vector<int> b(1000000, 2);

	timer_baseline.Start();
	auto result_baseline = addVectors_baseline(a, b);
	timer_baseline.Stop();

	double duration_baseline = timer_baseline.elapsedMicroseconds();

	timer_optimized.Start();
	auto result_optimized = addVectors_optimized(a, b);
	timer_optimized.Stop();

	double duration_optimized = timer_optimized.elapsedMicroseconds();

	std::cout << "Time elapsed for baseline: " << duration_baseline << " microseconds\n";

	std::cout << "Time elapsed for optimized: " << duration_optimized << " microseconds\n";

}