#pragma once

#include <iostream>
#include <fstream>


enum class FlipMode {
    Horizontal,
    Vertical
};


template <typename T>
T* readRawData(const std::string& filename, int width, int height) {
    // 打开二进制文件
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return nullptr;
    }

    // 计算数据总数
    size_t dataSize = static_cast<size_t>(width) * height;

    // 分配内存
    T* data = new T[dataSize];

    // 读取数据
    file.read(reinterpret_cast<char*>(data), dataSize * sizeof(T));

    // 检查是否成功读取了预期的数据
    if (file.gcount() != dataSize * sizeof(T)) {
        std::cerr << "Failed to read the expected amount of data from file.\n";
        delete[] data;  // 释放分配的内存
        return nullptr;
    }

    // 关闭文件
    file.close();

    return data;
}


template <typename T>
void saveRawData(const std::string& filename, const T* data, int width, int height) {
    // 打开二进制文件进行写入
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // 写入数据
    file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(width) * height * sizeof(T));

    // 检查是否成功写入了数据
    if (!file.good()) {
        std::cerr << "Failed to write data to file.\n";
    }

    // 关闭文件
    file.close();
}