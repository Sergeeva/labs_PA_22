#include "mandelbrot_cpu.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>

constexpr std::size_t WIDTH = 1920;
constexpr std::size_t HEIGHT = 1080;
const std::string FILE_PATH = "mandelbrot_cpu.ppm";

void Save(const std::vector<unsigned int>& result, std::size_t height, std::size_t width) {
    std::ofstream file(FILE_PATH, std::ios::binary);
    file << "P6\n" << width << " " << height << "\n255\n";
    for (int y = 0; y < height; ++y) {
        const unsigned int *line = result.data() + width * y;
        for (int x = 0; x < width; ++x) {
            file.write((const char *) (line + x), 3);
        }
    }
}

int main() {
    {
        auto startTime = clock();
        std::vector<unsigned int> result = mandelbrotCpu(5, HEIGHT, WIDTH, 256);
        std::cout << "Time taken: " << (clock() - startTime) / CLOCKS_PER_SEC << std::endl;
        Save(result, HEIGHT, WIDTH);
    }

    return 0;
}