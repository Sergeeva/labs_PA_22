#include "mandelbrot.hpp"

size_t align(size_t x, size_t y) {
    size_t remainder = x % y;
    return (remainder == 0) ? x : x - remainder + y;
}

cl_uint get_platforms_count() {
    cl_uint platforms_count = 0;
    clGetPlatformIDs(0, nullptr, &platforms_count);

    return platforms_count;
}

cl_platform_id get_first_platform() {

    auto platforms_count = get_platforms_count();

    if (platforms_count == 0) {
        throw std::runtime_error("No OpenCL platform was found");
    }

    cl_platform_id platform;

    clGetPlatformIDs(platforms_count, &platform, nullptr);

    return platform;
}

cl_uint get_devices_count(cl_platform_id platform) {
    cl_uint devices_count = 0;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &devices_count);

    return devices_count;
}

cl_device_id get_first_device() {

    auto platform = get_first_platform();

    auto devices_count = get_devices_count(platform);

    if (devices_count == 0) {
        throw std::runtime_error("No device for platform was found");
    }

    cl_int error_code = 0;
    cl_device_id device;

    error_code |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (error_code == CL_DEVICE_NOT_FOUND)
        error_code = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, nullptr);


    if (error_code != 0) throw std::runtime_error("Error while getting device");

    return device;
}

std::string read_cl_program_text() {
    std::string cl_program_text;
    std::getline(std::ifstream(CL_PROGRAM_FILE_PATH), cl_program_text, '\0');

    return cl_program_text;
}

cl_program build_cl_program(const cl_context context, const cl_device_id device) {
    cl_int error;

    auto source = read_cl_program_text();
    auto cl_program_text = source.c_str();
    auto cl_program_text_length = source.length();
    auto options = "";

    auto program = clCreateProgramWithSource(context, 1, &cl_program_text, &cl_program_text_length, &error);

    error = clBuildProgram(program, 1, &device, options, nullptr, nullptr);

    if (error == CL_BUILD_PROGRAM_FAILURE) {
        throw std::runtime_error("Failed to build program");
    }

    return program;
}

void mandelbrot(cl_uint* result, float mag, int width, int height, int iterations) {

    cl_int error;

    cl_device_id device = get_first_device();
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &error);

    cl_program program = build_cl_program(context, device);
    cl_kernel kernel = clCreateKernel(program, KERNEL_NAME, &error);
    cl_mem buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uint) * WIDTH * HEIGHT, nullptr, nullptr);

    clSetKernelArg(kernel, 0, sizeof(float), &mag);
    clSetKernelArg(kernel, 1, sizeof(int), &iterations);
    clSetKernelArg(kernel, 2, sizeof(cl_int), &width);
    clSetKernelArg(kernel, 3, sizeof(cl_int), &height);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &buff);

    size_t local_size[2] = { 256, 1 };
    size_t global_size[2] = { align(width, local_size[0]),align(height, local_size[1]) };

    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error);

    clock_t start_time = clock();
    clEnqueueNDRangeKernel(queue, kernel, DIMENSION, nullptr, global_size, local_size, 0, nullptr, nullptr);
    clEnqueueReadBuffer(queue, buff, CL_TRUE, 0, sizeof(cl_uint) * width * height, result, 0, nullptr, nullptr);

    clFlush(queue);
    clFinish(queue);
    printf("Time taken: %.6f\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);

    clReleaseKernel(kernel);
    clReleaseMemObject(buff);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);
}



void run() {

    if (!ocl_init()) throw;

    std::vector<cl_uint> result(WIDTH * HEIGHT);
    mandelbrot(result.data());
    save_result(result.data(), RESULT_FILE_PATH);
}
