#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include <libclew/ocl_init.h>
#include <CL/cl.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

void start(int width, int height, int coeff, int iterations);
void mandelbrot(cl_uint* result, float coeff, int width, int height, int iterations);
void save_result(const unsigned int* result, const std::string& name, int width, int height);

#endif
