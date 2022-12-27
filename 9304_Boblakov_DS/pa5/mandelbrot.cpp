#include "mandelbrot.h"

cl_device_id GPU::createDevice() {
    cl_platform_id platform;
    cl_device_id device;
    cl_int error = 0;

    error |= clGetPlatformIDs(1, &platform, nullptr);
    error |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (error == CL_DEVICE_NOT_FOUND) {
        error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, nullptr);
    }

    reportError(error, __FUNCTION__);
    return device;
}

cl_context GPU::createContext(cl_device_id device) {
    cl_int error = 0;

    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &error);

    reportError(error, __FUNCTION__);
    return context;
}

cl_program GPU::buildProgram(cl_context context, cl_device_id device) {
    cl_int error = 0;

    std::string src = getProgramText();
    const char *srcText = src.data();
    size_t srcLength = src.size();

    cl_program program = clCreateProgramWithSource(context, 1, &srcText, &srcLength, &error);
    error |= clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

    reportError(error, __FUNCTION__);
    return program;
}


std::string GPU::getProgramText() {
    std::ifstream file("../mandelbrot.cl");
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open kernel file");
    }
    std::string fileText = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
    return fileText;
}

cl_kernel GPU::createKernel(cl_program program) {
    cl_int error = 0;

    cl_kernel kernel = clCreateKernel(program, "draw", &error);

    reportError(error, __FUNCTION__);
    return kernel;
}

cl_command_queue GPU::createQueue(cl_context context, cl_device_id device) {
    cl_int error = 0;

    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error);

    reportError(error, __FUNCTION__);
    return queue;
}

cl_mem GPU::createBuffer(cl_context context, size_t size) {
    cl_int error = 0;

    cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size, nullptr, nullptr);

    reportError(error, __FUNCTION__);
    return buffer;
}

void GPU::invokeKernel(cl_kernel kernel, cl_command_queue queue, cl_mem buffer, cl_uint *result,
                       float x, float y, float scale, int width, int height, float iterations) {
    cl_int error = 0;
    error |= clSetKernelArg(kernel, 0, sizeof(float), &x);
    error |= clSetKernelArg(kernel, 1, sizeof(float), &y);
    error |= clSetKernelArg(kernel, 2, sizeof(int), &width);
    error |= clSetKernelArg(kernel, 3, sizeof(int), &height);
    error |= clSetKernelArg(kernel, 4, sizeof(float), &scale);
    error |= clSetKernelArg(kernel, 5, sizeof(float), &iterations);
    error |= clSetKernelArg(kernel, 6, sizeof(int), &width);
    error |= clSetKernelArg(kernel, 7, sizeof(cl_mem), &buffer);

    size_t local_size[2] = {32, 1};
    size_t global_size[2] = {align(width, local_size[0]), align(height, local_size[1])};

    error |= clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global_size, local_size, 0, nullptr, nullptr);
    error |= clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(int) * width * height, result, 0, nullptr, nullptr);

    reportError(error, __FUNCTION__);
    clFinish(queue);
}

size_t GPU::align(int x, size_t size) {
    return (x + size - 1) / size * size;
}

void GPU::save(const cl_uint *pixels, int width, int height, int which=0) {
    std::string name ="resultGPU.ppm";
    if (which == 1){
        name = "resultCPU.ppm";
    }
    std::ofstream file(name, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open result file");
    }
    file << "P6\n" << width << " " << height << "\n255\n";
    for (int y = 0; y < height; y++) {
        const cl_uint *line = pixels + width * y;
        for (int x = 0; x < width; x++) {
            file.write((const char *) (line + x), 3);
        }
    }
    file.close();
}

void GPU::reportError(cl_int status, const std::string function) {
    if (status == CL_SUCCESS) {
        return;
    }
    std::ostringstream stream;
    stream << status;
    std::string message = "OpenCL error code " + stream.str() + " at GPU::" + function + "()";
    throw std::runtime_error(message);
}
