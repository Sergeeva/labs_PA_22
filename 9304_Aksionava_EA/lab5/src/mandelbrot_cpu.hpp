#include <vector>
#include "common.hpp"

#ifndef MANDELBROT_CPU_HPP
#define MANDELBROT_CPU_HPP

#define RESULT_CPU_FILE_PATH "./mandelbrot_cpu.ppm"

void run_cpu();

void mandelbrot_cpu(
        unsigned int* results,
        unsigned int height = HEIGHT,
        unsigned int width = WIDTH,
        int iterations = ITERATIONS
);

#endif
