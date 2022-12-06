#include "opencl_mandelbrot.h"
#include "serial_mandelbrot.h"
#include "svpng.h"

#include <iostream>

template<typename T>
std::chrono::microseconds output_mandelbrot(T algorithm, std::string &file_name) {
    unsigned int image_width = 1920;
    unsigned int image_height = 1200;
    unsigned int iterations = 256;

    auto image = new unsigned char[image_height * image_width * 4];

    std::cout << "Image height: " << image_height << ", width: " << image_width << ", iterations: " << iterations
              << std::endl;

    auto duration = algorithm(image, image_height, image_width, iterations);

    auto file = fopen(file_name.c_str(), "wb");
    svpng(file, image_width, image_height, image, 1);
    fclose(file);

    delete[] image;

    return duration;
}

int main() {
    auto opencl_filename = std::string("../opencl_mandelbrot.png");
    auto serial_filename = std::string("../serial_mandelbrot.png");

    auto opencl_duration = output_mandelbrot(opencl_mandelbrot, opencl_filename);

    std::cout << "OpenCL duration: " << opencl_duration.count() << " microseconds" << std::endl;

    auto serial_duration = output_mandelbrot(serial_mandelbrot, serial_filename);

    std::cout << "Serial duration: " << serial_duration.count() << " microseconds" << std::endl;

    return 0;
}
