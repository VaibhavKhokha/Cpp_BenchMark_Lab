#pragma once

#include <cstdint>
#include <string>

struct Image
{
	uint8_t* data = nullptr;
	int width = 0;
	int height = 0;
	int channels = 0;
};

Image loadImage(const std::string& filename);
void saveImage(const std::string& filepath, const Image& img);
void freeImage(Image& img);
