#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

#include "CPUTimer.h"
#include "ImageStuff.h"

using namespace std;

#define REPS 	129
#define NUM_THREADS 128


template <typename T>
void FlipHRawImageFunc(const T* inputImage, T* outputImage, int width, int height) 
{
	int w, h;
	for (h = 0; h < height; ++h) 
	{
		for (w = 0; w < width; ++w) 
		{
			outputImage[h * width + w] = inputImage[h * width + (width - 1 - w)];
		}
	}
}


template <typename T>
void FlipVRawImageFunc(const T* inputImage, T* outputImage, int width, int height) 
{
	int w, h;
	for (h = 0; h < height; ++h)
	{
		for (w = 0; w < width; ++w)
		{
			outputImage[h * width + w] = inputImage[(height - 1 - h) * width + w];
		}
	}
}


int main()
{

	string ImagePath = "C://Users//ming//Desktop//testdata//BlackMarble_5760_3240_uint16.raw";
	string ImageSavePath = "C://Users//ming//Desktop//testdata//BlackMarble_5760_3240_uint16_output.raw";

	const int width = 5760;
	const int height = 3240;
	size_t dataSize = static_cast<size_t>(width) * height;

	// read image
	const uint16_t* rawData = readRawData<uint16_t>(ImagePath, width, height);

	if (!rawData) {
		cerr << "Failed to read raw data.\n";
		return 1;
	}
	else 
	{
		cout << "Successfully read raw data.\n";
	}

	double TotalCPUTime = 0;
	double PerImageTime = 0;
	double PerImage_onePixelTime = 0;

	//FlipMode flipmode = FlipMode::Horizontal;
	FlipMode flipmode = FlipMode::Horizontal;

	uint16_t* result = new uint16_t[dataSize];

	CPUTimer timer;

	int iter;
	switch (flipmode)
	{
	case FlipMode::Horizontal:
		for (iter = 0; iter < REPS; ++iter)
		{
			FlipHRawImageFunc(rawData, result, width, height);
		}
		std::cout << "Horizontal flip" << endl;
		break;
	case FlipMode::Vertical:
		for(iter = 0; iter < REPS; ++iter)
		{
			FlipVRawImageFunc(rawData, result, width, height);
		}
		cout << "Vertical flip" << endl;
		break;
	}

	TotalCPUTime = timer.elapsed();
	std::cout << "Total execution CPU time: " << TotalCPUTime << " ms" << std::endl;

	PerImageTime = TotalCPUTime / (double)REPS;
	cout << "Per execution time: " << PerImageTime << "ms" << endl;
	PerImage_onePixelTime = PerImageTime * 1000 * 1000 / (double)(width * height);
	cout << PerImage_onePixelTime << "ns per pixel" << endl;


	saveRawData<uint16_t>(ImageSavePath, result, width, height);

	// free memory
	delete[] rawData;
	delete[] result;

	return 0;
}
