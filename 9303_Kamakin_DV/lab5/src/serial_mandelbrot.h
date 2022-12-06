#ifndef MANDELBROT_SERIAL_MANDELBROT_H
#define MANDELBROT_SERIAL_MANDELBROT_H

#include <string>

std::chrono::microseconds serial_mandelbrot(unsigned char *results,
                                           unsigned int height,
                                           unsigned int width,
                                           unsigned int iterations);

#endif
