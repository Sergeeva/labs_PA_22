#include "opencl/opencl_mandelbrot.h"
#include "one_thread/one_thread_mandelbrot.h"
#include "params.h"

#include <iostream>

void openCl_alg() {
    auto opencl_filename = std::string("../opencl_mandelbrot.png");
    auto opencl_duration = opencl_mandelbrot(opencl_filename, image_height, image_width, iterations);
    std::cout << "OpenCL : " << opencl_duration.count() << " milliseconds" << std::endl;
}

void one_thread_alg() {
    auto serial_filename = std::string("../one_thread_mandelbrot.png");
    auto serial_duration = one_thread_mandelbrot(serial_filename, image_height, image_width, iterations);
    std::cout << "One thread algorithm took: " << serial_duration.count() << " milliseconds" << std::endl;
}

int main() {
    std::cout << "Image params: height: " << image_height << ", width: " << image_width << " iterations: " << iterations
              << std::endl;
    openCl_alg();
    one_thread_alg();
    return 0;
}
