#ifndef MANDELBROT_ONE_THREAD_MANDELBROT_H
#define MANDELBROT_ONE_THREAD_MANDELBROT_H

#include <string>

std::chrono::milliseconds one_thread_algorithm(unsigned char *results,
                                               size_t height,
                                               size_t width,
                                               size_t iterations);

std::chrono::milliseconds one_thread_mandelbrot(std::string &output_filename,
                                                size_t image_height, size_t image_width,
                                                size_t iterations);

#endif
