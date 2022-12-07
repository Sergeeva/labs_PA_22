#include <OpenCL/cl.h>

#include "errors/save_cl.h"
#include "opencl_mandelbrot.h"
#include "../svpng/svpng.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include <fstream>
#include "util.h"

std::string read_file(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file) {
        throw std::runtime_error("Failed to read file: " + file_name);
    }
    std::string str;

    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());
    return str;
}

cl_uint get_platforms_count() {
    cl_uint platformsCount = 0;
    CL_RUN_COMMAND(clGetPlatformIDs(0, nullptr, &platformsCount))
    return platformsCount;
}

cl_platform_id get_platform(cl_uint platform_id) {
    std::vector<cl_platform_id> platforms(1);
    CL_RUN_COMMAND(clGetPlatformIDs(1, platforms.data(), nullptr))
    return platforms[0];
}

std::vector<unsigned char> get_device_name(cl_device_id device) {
    size_t param_size = 0;
    CL_RUN_COMMAND(clGetDeviceInfo(device, CL_DEVICE_NAME, param_size, nullptr, &param_size))

    std::vector<unsigned char> dev_name(param_size, 0);
    CL_RUN_COMMAND(clGetDeviceInfo(device, CL_DEVICE_NAME, param_size, dev_name.data(), &param_size))

    return dev_name;
}

cl_device_id get_first_device() {
    auto platformsCount = get_platforms_count();

    if (platformsCount == 0) {
        throw std::runtime_error("OpenCL platforms not found");
    }
    cl_uint devicesCount = 0;
    CL_RUN_COMMAND(clGetDeviceIDs(get_platform(1), CL_DEVICE_TYPE_CPU, 0, nullptr, &devicesCount))

    if (devicesCount == 0) {
        throw std::runtime_error("OpenCL devices not found");
    }

    auto firstPlatform = get_platform(1);

    std::vector<cl_device_id> devices(2);
    CL_RUN_COMMAND(clGetDeviceIDs(firstPlatform, CL_DEVICE_TYPE_CPU, 1, devices.data(), 0))
    return devices[0];
}

cl_program build(const cl_context context, const cl_device_id device, const std::string &source) {
    cl_int error;
    auto sourceStr = source.c_str();
    auto sourceLength = source.length();
    cl_program program = clCreateProgramWithSource(context, 1, &sourceStr, &sourceLength, &error);
    // создаёт объект программы из исходного кода для данного контекста (первый аргумент) и загружает туда исходный код

    error = clBuildProgram(program, 1, &device, "", nullptr, nullptr);

    if (error == CL_BUILD_PROGRAM_FAILURE) {
        throw std::runtime_error("Failed to build program!");
    }

    return program;
}

std::string get_device_type(cl_device_id device) {
    size_t param_size = 0;
    cl_device_type device_type;
    CL_RUN_COMMAND(clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type), &device_type, &param_size))
    switch (device_type) {
        case CL_DEVICE_TYPE_CPU:
            return "CPU";
        case CL_DEVICE_TYPE_GPU:
            return "GPU";
        default:
            return "UNKNOWN TYPE";
    }
}

cl_mem create_image(cl_context context, size_t height, size_t width) {
    cl_image_format format;
    auto error = 0;
    format.image_channel_order = CL_RGBA;
    format.image_channel_data_type = CL_UNSIGNED_INT8;
    auto cl_img = clCreateImage2D(context, CL_MEM_WRITE_ONLY,
                                                         &format, width, height, 0, nullptr, &error);
    CL_CHECK_CODE(error)

    return cl_img;
}

std::chrono::milliseconds opencl_mandelbrot(std::string &output_filename,
                                            size_t image_height, size_t image_width,
                                            size_t iterations) {
    auto first_device = get_first_device();

    std::cout << "OpenCL device: " << get_device_name(first_device).data() << std::endl;
    std::cout << "Device type: " << get_device_type(first_device) << std::endl;

    size_t global[2] = {image_width, image_height};
    size_t local[2] = {1, 1};
    auto result = new uint8_t[image_width * image_height * 4];

    cl_int error = 0;
    auto context = clCreateContext(nullptr, 1, &first_device, nullptr, nullptr, &error);

    auto queue = clCreateCommandQueue(context, first_device, 0, &error); // очередь команд

    auto filename = std::string("../src/opencl/mandelbrot.cl");
    auto source = read_file(filename);
    auto program = build(context, first_device, source);

    auto kernel = clCreateKernel(program, "mandelbrot", &error);

    auto imagePng = create_image(context, image_height, image_width);
    // создаем изображение

    CL_RUN_COMMAND(clSetKernelArg(kernel, 0, sizeof(imagePng), &imagePng));
    CL_RUN_COMMAND(clSetKernelArg(kernel, 1, sizeof(uint), &iterations));

    auto start = std::chrono::steady_clock::now();
    CL_RUN_COMMAND(clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global, local, 0, nullptr, nullptr));

    size_t origin[3] = {0, 0, 0};
    size_t depth[3] = {image_width, image_height, 1};

    CL_RUN_COMMAND(clEnqueueReadImage(queue, imagePng, CL_TRUE, origin, depth, 0, 0, result, 0, nullptr, nullptr));

    CL_RUN_COMMAND(clFlush(queue)); // очищаем очередь команд
    CL_RUN_COMMAND(clFinish(queue)); //  // завершаем выполнение всех команд в очереди
    auto end = std::chrono::steady_clock::now();
    auto file = fopen(output_filename.c_str(), "w+b");
    svpng(file, image_width, image_height, result, 1);
    fclose(file);

    delete[] result;

    CL_RUN_COMMAND(clReleaseKernel(kernel)); // удаляем ядро
    CL_RUN_COMMAND(clReleaseProgram(program)); // удаляем программу OpenCL
    CL_RUN_COMMAND(clReleaseMemObject(imagePng));  // отчищаем OpenCL буфер изображения
    CL_RUN_COMMAND(clReleaseCommandQueue(queue)); // удаляем очередь команд
    CL_RUN_COMMAND(clReleaseContext(context));  // удаляем контекст OpenCL

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}
