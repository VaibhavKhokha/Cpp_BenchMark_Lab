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
			
			for (size_t pixel = startpixel; pixel < endpixel; pixel++)
			{
				size_t byteIndex = pixel * img.channels;

				uint8_t r = img.data[byteIndex];
				uint8_t g = img.data[byteIndex + 1];
				uint8_t b = img.data[byteIndex + 2];

				uint8_t grey = (r * 77 + g * 150 + b * 29) >> 8;

				img.data[byteIndex] = grey;
				img.data[byteIndex + 1] = grey;
				img.data[byteIndex + 2] = grey;
			}
						
		});
	}


	for (auto& th : threads)
	{
		th.join();
	}

}