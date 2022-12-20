#include <vector>
#include <iostream>
#include <fstream>

#ifndef MANDELBROT_CPU_HPP
#define MANDELBROT_CPU_HPP

void start_cpu(int width, int height, int coeff, int iterations);

void mandelbrot_cpu(unsigned int* results, unsigned int height, unsigned int width, int iterations, int coeff);
void save_cpu_result(const unsigned int* result, const std::string& name, int width, int height);

#endif