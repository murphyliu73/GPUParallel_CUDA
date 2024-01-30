#include <iostream>
#include <thread>
#include <chrono>
#include <thread>

#include "CPUTimer.h"
#include "ImageStuff.h"

#define REPS 129

template <typename T>
void FlipHRawImageFunc_Parallel(const T* inputImage, T* outputImage, const int width, const int height, int start, int end) {
	// 确保输入有效
	if (inputImage == nullptr || outputImage == nullptr || start >= end || start < 0 || end > height) {
		return;
	}

	for (int h = start; h < end; ++h) {
		for (int w = 0; w < width; ++w) {
			outputImage[h * width + (width - 1 - w)] = inputImage[h * width + w];
		}
	}
}


template <typename T>
void FlipImageMultiThreaded(const T* inputImage, T* outputImage, int width, int height, int numThreads) {
	// define number of threads
	if (numThreads < 1) {
		numThreads = std::thread::hardware_concurrency();
	}
	std::vector<std::thread> threads;

	// 计算每个线程要处理的行数
	int rowsPerThread = height / numThreads;

	for (int i = 0; i < numThreads; ++i) {
		int start = i * rowsPerThread;
		int end = (i == numThreads - 1) ? height : (i + 1) * rowsPerThread; // 确保最后一个线程处理所有剩余的行

		threads.emplace_back(FlipHRawImageFunc_Parallel<T>, inputImage, outputImage, width, height, start, end);
	}

	// 等待所有线程完成
	for (auto& thread : threads) {
		thread.join();
	}
}


int main()
{
	string ImagePath = "C://Users//ming//Desktop//testdata//BlackMarble_5760_3240_uint16.raw";
	string ImageSavePath = "C://Users//ming//Desktop//testdata//BlackMarble_5760_3240_uint16_output_cpu_parallel.raw";

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

	// allocate memory for output image
	uint16_t* outputData = new uint16_t[dataSize];


	CPUTimer timer;
	timer.reset();

	// flip image
	int num_threads = std::thread::hardware_concurrency() / 2;
	std::cout << "1/2 Number of threads: " << num_threads << std::endl;

	int iter;
	for (iter = 0; iter < REPS; ++iter)
	{
		FlipImageMultiThreaded(rawData, outputData, width, height, num_threads);
	}
	
	double TotalCPUSerialTime = 0;
	double PerImageTime = 0;
	double PerImage_onePixelTime = 0;

	//std::cout << "CPU serial flip took " << timer.elapsed() << " ms.\n";
	TotalCPUSerialTime = timer.elapsed();
	std::cout << "Total CPU serial execution time: " << TotalCPUSerialTime << " ms" << std::endl;

	PerImageTime = TotalCPUSerialTime / (double)REPS;
	cout << "Per execution time: " << PerImageTime << "ms" << endl;
	PerImage_onePixelTime = PerImageTime * 1000 * 1000 / (double)(width * height);
	cout << PerImage_onePixelTime << "ns per pixel" << endl;

	// save flipped image
	saveRawData(ImageSavePath, outputData, width, height);

	// free memory
	delete[] rawData;
	delete[] outputData;

	return 0;
}