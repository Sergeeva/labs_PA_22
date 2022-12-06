#include <OpenCL/cl.h>

#include "save_cl.h"
#include "opencl_mandelbrot.h"
#include "util.h"
#include "svpng.h"

#include <iostream>
#include <stdexcept>
#include <vector>

cl_uint get_platforms_count() {
    cl_uint platformsCount = 0;
    CL_SAVE_EXECUTE(clGetPlatformIDs(0, nullptr, &platformsCount))
    return platformsCount;
}

std::vector<cl_platform_id> get_platforms(cl_uint platforms_count) {
    std::vector<cl_platform_id> platforms(platforms_count);
    CL_SAVE_EXECUTE(clGetPlatformIDs(platforms_count, platforms.data(), nullptr))

    return platforms;
}

std::vector<unsigned char> get_device_name(cl_device_id device) {
    size_t param_size = 0;
    CL_SAVE_EXECUTE(clGetDeviceInfo(device, CL_DEVICE_NAME, param_size, nullptr, &param_size))

    std::vector<unsigned char> dev_name(param_size, 0);
    CL_SAVE_EXECUTE(clGetDeviceInfo(device, CL_DEVICE_NAME, param_size, dev_name.data(), &param_size))

    return dev_name;
}

cl_uint get_devices_count(cl_platform_id platform) {
    cl_uint devicesCount = 0;
    CL_SAVE_EXECUTE(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &devicesCount))
    return devicesCount;
}

std::vector<cl_device_id> get_devices(cl_platform_id platform, cl_uint devices_count) {
    std::vector<cl_device_id> devices(devices_count);
    CL_SAVE_EXECUTE(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, devices.data(), &devices_count))
    return devices;
}

cl_device_id get_first_device(cl_platform_id platform) {
    auto devicesCount = get_devices_count(platform);

    if (devicesCount == 0) {
        throw std::runtime_error("OpenCL devices not found");
    }

    return get_devices(platform, devicesCount)[0];
}

cl_device_id get_first_device() {
    auto platformsCount = get_platforms_count();

    if (platformsCount == 0) {
        throw std::runtime_error("OpenCL platforms not found");
    }

    return get_first_device(get_platforms(platformsCount)[0]);
}

cl_program build(const cl_context context, const cl_device_id device, const std::string &source) {
    cl_int error;
    auto sourceStr = source.c_str();
    auto sourceLength = source.length();
    cl_program program = clCreateProgramWithSource(context, 1, &sourceStr, &sourceLength, &error);
    CL_CHECK_CODE(error)

    const char *options = "";
    error = clBuildProgram(program, 1, &device, options, nullptr, nullptr);

    if (error == CL_BUILD_PROGRAM_FAILURE) {
        size_t messageSize;
        CL_SAVE_EXECUTE(clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &messageSize))

        std::vector<unsigned char> message(messageSize);
        CL_SAVE_EXECUTE(clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, messageSize, message.data(),
                                              nullptr))

        throw std::runtime_error("Failed to build program: " + std::string(message.begin(), message.end()));
    }

    return program;
}

std::string get_device_type(cl_device_id device) {
    size_t param_size = 0;
    cl_device_type dev_type;
    CL_SAVE_EXECUTE(clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type), &dev_type, &param_size))

    switch (dev_type) {
        case CL_DEVICE_TYPE_CPU:
            return "CPU";
        case CL_DEVICE_TYPE_GPU:
            return "GPU";
        default:
            return "UNKNOWN";
    }
}

cl_mem createImage(cl_context context, size_t height, size_t width) {
    cl_image_format format;
    auto error = 0;
    format.image_channel_order = CL_RGBA;
    format.image_channel_data_type = CL_UNSIGNED_INT8;

    cl_image_desc desc;
    desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    desc.image_width = width;
    desc.image_height = height;
    desc.image_depth = 0;
    desc.image_array_size = 0;
    desc.image_row_pitch = 0;
    desc.image_slice_pitch = 0;
    desc.num_mip_levels = 0;
    desc.num_samples = 0;
    desc.buffer = nullptr;

    auto cl_img = clCreateImage(context, CL_MEM_WRITE_ONLY, &format, &desc, nullptr, &error);
    CL_CHECK_CODE(error)

    return cl_img;
}

std::chrono::microseconds opencl_mandelbrot(unsigned char *image,
                                            uint image_height, uint image_width,
                                            uint iterations) {
    auto first_device = get_first_device();

    std::cout << "OpenCL device to use: " << get_device_name(first_device).data() << std::endl;
    std::cout << "Device type: " << get_device_type(first_device) << std::endl;

    auto error = 0;
    auto context = clCreateContext(nullptr, 1, &first_device, nullptr, nullptr, &error);
    CL_CHECK_CODE(error)

    auto queue = clCreateCommandQueue(context, first_device, 0, &error);
    CL_CHECK_CODE(error)

    auto filename = std::string("../src/mandelbrot.cl");
    auto source = read_file(filename);
    auto program = build(context, first_device, source);

    auto kernel = clCreateKernel(program, "mandelbrot", &error);
    CL_CHECK_CODE(error)

    auto opencl_image = createImage(context, image_height, image_width);

    CL_SAVE_EXECUTE(clSetKernelArg(kernel, 0, sizeof(opencl_image), &opencl_image))
    CL_SAVE_EXECUTE(clSetKernelArg(kernel, 1, sizeof(uint), &iterations))

    size_t global[2] = {image_width, image_height};
    size_t local[2] = {16, 16};

    auto start = std::chrono::steady_clock::now();
    CL_SAVE_EXECUTE(clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global, local, 0, nullptr, nullptr))

    size_t origin[3] = {0, 0, 0};
    size_t depth[3] = {image_width, image_height, 1};
    CL_SAVE_EXECUTE(clEnqueueReadImage(queue, opencl_image, CL_TRUE,
                                       origin, depth, 0, 0, image,
                                       0, nullptr, nullptr))

    CL_SAVE_EXECUTE(clFlush(queue))
    CL_SAVE_EXECUTE(clFinish(queue))
    auto end = std::chrono::steady_clock::now();

    CL_SAVE_EXECUTE(clReleaseKernel(kernel))
    CL_SAVE_EXECUTE(clReleaseProgram(program))
    CL_SAVE_EXECUTE(clReleaseMemObject(opencl_image))
    CL_SAVE_EXECUTE(clReleaseCommandQueue(queue))
    CL_SAVE_EXECUTE(clReleaseContext(context))

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}
