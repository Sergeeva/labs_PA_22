#include <libclew/ocl_init.h>
#include "mandelbrot.h"

std::vector<unsigned int> mandelbrotCpu(float coefficient, int height, int width, int iterations) {
    std::vector<unsigned int> results;
    results.resize(height * width);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            float x0 = coefficient * (((float) i - (float) width / 2) / (float) width);
            float y0 = coefficient * (((float) j - (float) height / 2) / (float) height);
            float x = x0;
            float y = y0;

            int iter = 0;

            while (x * x + y * y <= 4.0 && iter < iterations) {
                float x_next = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = x_next;
                iter++;
            }
            results[j * width + i] = iter | (iter << 8) | (iter << 16) | (0xff << 24);
        }
    }
    return results;
}

int main() {
    if (!ocl_init()) {
        return -1;
    }

    static const int width = 1920, height = 1080, iterations = 50;

    cl_device_id device;
    cl_context context;
    cl_program program;
    cl_kernel kernel;
    cl_command_queue queue;
    cl_mem buffer;

    device = GPU::createDevice();
    context = GPU::createContext(device);
    program = GPU::buildProgram(context, device);
    kernel = GPU::createKernel(program);
    queue = GPU::createQueue(context, device);
    buffer = GPU::createBuffer(context, sizeof(cl_uint) * width * height);

    std::vector<cl_uint> pixelsGPU(width * height);
    clock_t startTime = clock();
    GPU::invokeKernel(kernel, queue, buffer, pixelsGPU.data(), -0.5f, 0, 5.0f, width, height, iterations);
    printf("GPU Time taken: %.6f\n", (double) (clock() - startTime) / CLOCKS_PER_SEC);
    GPU::save(pixelsGPU.data(), width, height, 0);

    startTime = clock();
    std::vector<unsigned int> pixelsCPU = mandelbrotCpu(3, width, height, iterations);
    printf("CPU Time taken: %.6f\n", (double) (clock() - startTime) / CLOCKS_PER_SEC);
    GPU::save(pixelsCPU.data(), height, width, 1);
}
