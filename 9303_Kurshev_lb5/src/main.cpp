#include "mandelbrot.hpp"
#include "mandelbrot_cpu.hpp"

int main() {
    int width = 3840;
    int height = 2160;
    int coeff = 5;
    int iterations = 256;
    start(width, height, coeff, iterations);
    start_cpu(width, height, coeff, iterations);
    return 0;
}
