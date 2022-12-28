#ifndef PARALLEL_ALGORITHMS_LAB5_OPENCL_MANDELBROT_H
#define PARALLEL_ALGORITHMS_LAB5_OPENCL_MANDELBROT_H

#include <string>
#include <vector>
#include <algorithm>
#include <vector>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

#include <CL/cl.h>
#include <libclew/ocl_init.h>

constexpr size_t ONE_ENTRY = 1;
constexpr size_t ZERO_SIZE = 0;
constexpr size_t ZERO_CODE = 0;
constexpr size_t ZERO_OFFSET = 0;
constexpr size_t WORK_DIMENSION = 2;
constexpr size_t HORIZONTAL_WORK_GROUP_WIDTH = 256;
constexpr size_t HORIZONTAL_WORK_GROUP_HEIGHT = 1;

std::vector<unsigned int> compute_opencl_mandelbrot(
        int height,
        int width,
        float ratio,
        int iterations_limit
);

#endif //PARALLEL_ALGORITHMS_LAB5_OPENCL_MANDELBROT_H
