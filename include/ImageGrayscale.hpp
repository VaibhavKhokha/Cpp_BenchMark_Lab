#pragma  once
#include "ImageUtils.hpp"

void grayscale_baseline(Image& img);
void grayscale_optimized(Image& img);
void grayscale_multithreaded(Image& img, size_t numThreads);