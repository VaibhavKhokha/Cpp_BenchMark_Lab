#include <iostream>
#include "Timer.hpp"


int main()
{
	Timer timer;

	timer.Start();

	long long sum = 0;

	for (int i = 0; i < 100000; i++)
	{
		sum += i;
	}

	timer.Stop();

	double duration = timer.elapsedMicroseconds();

	std::cout << "Time elapsed: " << duration << " microseconds\n";
}