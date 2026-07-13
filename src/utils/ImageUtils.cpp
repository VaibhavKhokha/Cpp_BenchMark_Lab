#include "ImageUtils.hpp"
#include <iostream>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image loadImage(const std::string& filepath)
{
	Image img;

	img.data = stbi_load(filepath.c_str(), &img.width, &img.height, &img.channels, 0);

	if (img.data == nullptr)
	{
		throw std::runtime_error("Failed to load image: " + filepath);
	}

	return img;

}

void saveImage(const std::string& filepath, const Image& img)
{
	int stride = img.width * img.channels; //bytes in a single row
	int success = stbi_write_png(filepath.c_str(), img.width, img.height,img.channels, img.data, stride);

	if (!success)
	{
		throw std::runtime_error("Failed to save image: " + filepath);
	}

}


void freeImage(Image& img)
{
	if (img.data != nullptr)
	{
		stbi_image_free(img.data);
		img.data = nullptr;
	}
}