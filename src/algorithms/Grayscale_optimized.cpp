#include "ImageGrayscale.hpp"

void grayscale_optimized(Image& img)
{
	int width = img.width;
	int height = img.height;
	int channels = img.channels;

	for (int i = 0; i < (width * height * channels); i += img.channels)
	{
		uint8_t r = img.data[i];
		uint8_t g = img.data[i + 1];
		uint8_t b = img.data[i + 2];

		uint8_t grey = (r * 77 + g * 150 + b * 29) >> 8; //bit shifted to 256;

		img.data[i] = grey;
		img.data[i + 1] = grey;
		img.data[i + 2] = grey;
	}
}