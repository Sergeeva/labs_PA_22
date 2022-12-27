#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <CL/cl.h>
#include <libclew/ocl_init.h>

#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>

class Mandelbrot {
private:
    unsigned int width_;
    unsigned int height_;
    unsigned int iterations_;
    float scale_;
    float offsetX_;
    float offsetY_;
    std::vector<cl_uint> result_;
    void vectorToPPM(std::string filename);
public:
    Mandelbrot(unsigned int width, unsigned int height, unsigned int iterations, float scale, float offsetX, float offsetY);
    void mandelbrot();
};

#endif
