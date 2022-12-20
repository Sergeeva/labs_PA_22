#pragma once

#include <CL/cl.hpp>
#include <vector>
#include <fstream>

#define CL_PROGRAM_FILE_PATH "mandelbrot.cl"
#define KERNEL_FUNCTION_NAME "mandelbrot"

std::vector<cl_uint> mandelbrot(float coefficient, int height, int width, int iterations);
