#include <iostream>
#include "mandelbrot.hpp"

size_t align(size_t x, size_t y) {
    size_t blocksCount = (x + y - 1) / y;
    return blocksCount * y;
}

inline void log(cl_int err, const char *name) {
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << name << std::endl;
}

std::vector<cl_uint> mandelbrot(float coefficient, cl_int height, cl_int width, cl_int iterations) {
    cl_int err;

    std::vector<cl::Platform> platformList;
    cl::Platform::get(&platformList);
    log(!platformList.empty() ? CL_SUCCESS : -1, "cl::Platform::get");

    cl_context_properties cprops[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) (platformList[0])(), 0};

    cl::Context context(CL_DEVICE_TYPE_DEFAULT, cprops, nullptr, nullptr, &err);
    log(err, "Context::Context()");

    std::vector<cl::Device> devices;
    devices = context.getInfo<CL_CONTEXT_DEVICES>();
    log(!devices.empty() ? CL_SUCCESS : -1, "devices.size() > 0");

    cl::CommandQueue queue(context, devices[0], 0, &err);
    log(err, "CommandQueue::CommandQueue()");

    std::vector<cl_uint> result;
    result.resize(height * width);
    cl::Buffer buffer(context,
                      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
                      sizeof(cl_uint) * height * width,
                      result.data(),
                      &err);
    log(err, "Buffer::Buffer()");

    std::ifstream file(CL_PROGRAM_FILE_PATH);
    log(file.is_open() ? CL_SUCCESS : -1, CL_PROGRAM_FILE_PATH);

    std::string code(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
    cl::Program::Sources source(1, std::make_pair(code.c_str(), code.length() + 1));

    cl::Program program(context, source);
    err = program.build(devices, "");
    log(err, "Program::build()");

    cl::Kernel kernel(program, KERNEL_FUNCTION_NAME, &err);
    log(err, "Kernel::Kernel()");

    err = kernel.setArg(0, coefficient);
    log(err, "Kernel::setArg() 0");
    err = kernel.setArg(1, height);
    log(err, "Kernel::setArg() 1");
    err = kernel.setArg(2, width);
    log(err, "Kernel::setArg() 2");
    err = kernel.setArg(3, iterations);
    log(err, "Kernel::setArg() 3");
    err = kernel.setArg(4, buffer);
    log(err, "Kernel::setArg() 4");

    auto horizontalWorkGroupHeight = 1;
    auto horizontalWorkGroupWidth = 256;

    cl::Event event;
    err = queue.enqueueNDRangeKernel(kernel,
                                     cl::NullRange,
                                     cl::NDRange(align(width, horizontalWorkGroupWidth),
                                                 align(height, horizontalWorkGroupHeight)),
                                     cl::NDRange(horizontalWorkGroupWidth, horizontalWorkGroupHeight),
                                     nullptr,
                                     &event);
    log(err, "CommandQueue::enqueueNDRangeKernel()");

    event.wait();

    err = queue.enqueueReadBuffer(buffer, CL_TRUE, 0, sizeof(cl_uint) * height * width, result.data());
    log(err, "CommandQueue::enqueueReadBuffer()");

    return result;
}
