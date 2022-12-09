#ifndef PARALLEL_ALGORITHMS_LAB5_SEQUENTIAL_MANDELBROT_H
#define PARALLEL_ALGORITHMS_LAB5_SEQUENTIAL_MANDELBROT_H

#include <string>
#include <vector>

std::vector<unsigned int> compute_sequential_mandelbrot(
        int height,
        int width,
        float ratio,
        int iterations_limit
);

#endif //PARALLEL_ALGORITHMS_LAB5_SEQUENTIAL_MANDELBROT_H
