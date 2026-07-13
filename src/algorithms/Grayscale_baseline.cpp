#include "ImageGrayscale.hpp"

void grayscale_baseline(Image& img)
{
	int width = img.width;
	int height = img.height;
	int channels = img.channels;

	for (int i = 0; i < (width * height * channels); i+=img.channels)
	{
		uint8_t r = img.data[i];
		uint8_t g = img.data[i + 1];
		uint8_t b = img.data[i + 2];

		float Y = (0.299f) * r + (0.587f) * g + (0.114f) * b;

		uint8_t grey = (uint8_t)Y;

		img.data[i] = grey;
		img.data[i + 1] = grey;
		img.data[i + 2] = grey;
	}
}