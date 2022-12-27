#include "Mandelbrot.h"
#include "MandelbrotCPU.h"

int main(int argc, char *argv[]) {
    int width, height;
    unsigned int iterations;
    float scale, offsetX, offsetY;
    if (argc != 7){
        std::cout << "Wrong arguments" << std::endl;
        return 0;
    }
    width = atoi(argv[1]);
    height = atoi(argv[2]);
    iterations = atoi(argv[3]);
    scale = atof(argv[4]);
    offsetX = atof(argv[5]);
    offsetY = atof(argv[6]);
    MandelbrotCPU cpu = MandelbrotCPU(width, height, iterations, scale, offsetX, offsetY);
    cpu.mandelbrot();
    Mandelbrot gpu = Mandelbrot(width, height, iterations, scale, offsetX, offsetY);
    gpu.mandelbrot();
    return 0;
}