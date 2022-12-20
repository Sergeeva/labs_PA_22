#ifndef MANDELBROT_CPU_H
#define MANDEKBROT_CPU_H

void mandelbrotCPU(float* results,
           unsigned int width, unsigned int height,
           float fromX, float fromY,
           float sizeX, float sizeY,
           unsigned int iters, bool smoothing);

#endif
