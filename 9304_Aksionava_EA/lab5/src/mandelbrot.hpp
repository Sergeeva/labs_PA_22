#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include <libclew/ocl_init.h>
#include <OpenCL/CL.h>
#include <vector>
#include "common.hpp"


#define DIMENSION 2
#define KERNEL_NAME "mandelbrot"
#define CL_PROGRAM_FILE_PATH "./mandelbrot.cl"
#define RESULT_FILE_PATH "./mandelbrot.ppm"

void run();

void mandelbrot(
        cl_uint* result,
        float mag = COEFFICIENT,
        int width = WIDTH,
        int height = HEIGHT,
        int iterations = ITERATIONS
);

#endif
