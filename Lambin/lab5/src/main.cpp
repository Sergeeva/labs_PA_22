#include <libclew/ocl_init.h>
#include "mandelbrot.h"

int main() {
    if (!ocl_init()) {
        return -1;
    }

    static const int width = 1200, height = 600;
    int error = 0;

    cl_device_id device;
    cl_context context;
    cl_program program;
    cl_kernel kernel;
    cl_command_queue queue;
    cl_mem buffer;

    try {
        device = Mandelbrot::createDevice();
        context = Mandelbrot::createContext(device);
        program = Mandelbrot::buildProgram(context, device);
        kernel = Mandelbrot::createKernel(program);
        queue = Mandelbrot::createQueue(context, device);
        buffer = Mandelbrot::createBuffer(context, sizeof(cl_uint) * width * height);

        std::vector<cl_uint> pixels(width * height);
        Mandelbrot::invokeKernel(kernel, queue, buffer, pixels.data(), -0.5f, 0, 5.0f, width, height, 50);
        Mandelbrot::save(pixels.data(), width, height);
    } catch (std::runtime_error &exception) {
        std::cerr << exception.what() << "\n";
    }

    clReleaseMemObject(buffer);
    clReleaseCommandQueue(queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
}
