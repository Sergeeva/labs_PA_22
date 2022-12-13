#ifndef COMMON_HPP
#define COMMON_HPP

#include <fstream>
#include <string>

#define HEIGHT 1200
#define WIDTH 1920
#define ITERATIONS 256
#define COEFFICIENT 5

void save_result(const unsigned int* result, const std::string& name, int width = WIDTH, int height = HEIGHT);

#endif
