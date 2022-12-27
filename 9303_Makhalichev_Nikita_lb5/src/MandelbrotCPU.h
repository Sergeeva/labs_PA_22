#ifndef MANDELBROTCPU_H
#define MANDELBROTCPU_H

#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>

class MandelbrotCPU {
private:
    int width_;
    int height_;
    unsigned int iterations_;
    float scale_;
    float offsetX_;
    float offsetY_;
    std::vector<unsigned int> result_;
    void vectorToPPM(std::string filename);
public:
    MandelbrotCPU(int width, int height, unsigned int iterations, float scale, float offsetX, float offsetY);
    void mandelbrot();
};

#endif
