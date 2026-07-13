#include "ImageGrayscale.hpp"
#include <vector>
#include <thread>

void grayscale_multithreaded(Image& img, size_t numThreads)
{
	int totalPixels = img.width * img.height;

	size_t chunk = totalPixels / numThreads;

	std::vector<std::thread> threads;
	threads.reserve(numThreads);

	for (size_t t = 0; t < numThreads; t++)
	{
		size_t startpixel = chunk * t;
		size_t endpixel = (t == numThreads - 1) ? totalPixels : (startpixel + chunk);

		threads.emplace_back([&,startpixel, endpixel]() {
			
			for (size_t i = startpixel; i < endpixel; i += img.channels)
			{

			}
						
		});
	}

}