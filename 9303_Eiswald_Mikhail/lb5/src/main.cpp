#include <CL/cl.h>
#include <libclew/ocl_init.h>
#include <CL/cl_ext.h>
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
#include <CL/cl_platform.h>
#include <CL/opencl.h>

#include <cstring>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <fstream>

#include "libpng_boilerplate_code.h"
#include "mandelbrot_cpu.h"

template<typename T>
std::string to_string(T value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

void reportError(cl_int err, const std::string &filename, int line) {
    if (CL_SUCCESS == err)
        return;
    std::string message = "OpenCL error code " + to_string(err) + " encountered at " + filename + ":" + to_string(line);
    throw std::runtime_error(message);
}

#define OCL_SAFE_CALL(expr) reportError(expr, __FILE__, __LINE__)

void check_return_argument(cl_int actual, const std::string &filename, int line) {
    if(actual == CL_SUCCESS) return;
    std::string message = "OpenCL error code " + to_string(actual) + " encountered at " + filename + ":" + to_string(line);
    throw std::runtime_error(message);
}

#define OCL_CHECK_PARAM(expr) check_return_argument(expr, __FILE__, __LINE__)

size_t align(size_t x, size_t y) {
    size_t remainder = x % y;
    return (remainder == 0) ? x : x - remainder + y;
}

void save_result(const unsigned int* result, const std::string& name, int width, int height) {
    std::ofstream file(name, std::ios::binary);
    file << "P6\n" << width << " " << height << "\n255\n";
    for (int y = 0; y < height; ++y) {
        const unsigned int* line = result + width * y;
        for (int x = 0; x < width; ++x) {
            file.write((const char *) (line + x), 3);
        }
    }
}

int main() {
    const int image_height = 3200;
    const int image_width = 3000;
    const int iterations = 50000;
    std::cout << "Image " << image_width << "x" << image_height << " px\n" <<
                 iterations << " repetitions\n";

    float* cpu_results = new float [image_width * image_height];

    auto start_cpu = std::chrono::high_resolution_clock::now();
    mandelbrotCPU(
                cpu_results,
                image_width, image_height,
                -2.0, -1.0, 3.25, 2.0,
                iterations,
                false
    );
    auto stop_cpu = std::chrono::high_resolution_clock::now();
    auto duration_cpu = std::chrono::duration_cast<std::chrono::milliseconds>(stop_cpu - start_cpu);
    std::cout << "Iterative version: Elapsed time: " << duration_cpu.count() << " ms\n";

    if (!ocl_init())
        throw std::runtime_error("Can't init OpenCL driver!");

    cl_uint platformsCount = 0;
    OCL_SAFE_CALL(clGetPlatformIDs(0, nullptr, &platformsCount));
    std::cout << "Number of OpenCL platforms: " << platformsCount << std::endl;

    std::vector<cl_platform_id> platforms(platformsCount);
    OCL_SAFE_CALL(clGetPlatformIDs(platformsCount, platforms.data(), nullptr));

    for (int platformIndex = 0; platformIndex < platformsCount; ++platformIndex) {
        std::cout << "Platform #" << (platformIndex + 1) << "/" << platformsCount << std::endl;
        cl_platform_id platform = platforms[platformIndex];

        size_t platformNameSize = 0;
        OCL_SAFE_CALL(clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, nullptr, &platformNameSize));
        std::vector<unsigned char> platformName(platformNameSize, 0);
        
        OCL_SAFE_CALL(clGetPlatformInfo(platform, CL_PLATFORM_NAME, platformNameSize, platformName.data(), &platformNameSize));
        std::cout << "    Platform name: " << platformName.data() << std::endl;

        size_t platformVendorSize = 0;
        OCL_SAFE_CALL(clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, 0, nullptr, &platformVendorSize));        
        
        std::vector<unsigned char> platformVendor(platformVendorSize, 0);

        OCL_SAFE_CALL(clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, platformVendorSize, platformVendor.data(), &platformVendorSize));
        std::cout << "    Platform vendor: " << platformVendor.data() << std::endl;

        cl_uint devicesCount = 0;
        OCL_SAFE_CALL(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &devicesCount));
        std::cout << "      Number of OpenCL dev-s: " << devicesCount << std::endl; 
        std::vector<cl_device_id> devices (devicesCount);
        OCL_SAFE_CALL(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, devices.data(), &devicesCount));
        
        for (int deviceIndex = 0; deviceIndex < devicesCount; ++deviceIndex) {
            std::cout << "    >> DEVICE #" << (deviceIndex + 1) << "/" << devicesCount << std::endl;
            cl_device_id device = devices[deviceIndex];
            size_t param_size = 0;
            OCL_SAFE_CALL(clGetDeviceInfo(device, CL_DEVICE_NAME, param_size, nullptr, &param_size));
            std::vector<unsigned char> dev_name (param_size, 0);
            OCL_SAFE_CALL(clGetDeviceInfo(device, CL_DEVICE_NAME, param_size, dev_name.data(), &param_size));
            std::cout << "    Device name: " << dev_name.data() << std::endl;
            cl_device_type dev_type;
            OCL_SAFE_CALL(clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type), &dev_type, &param_size));
            switch (dev_type)
            {
            case CL_DEVICE_TYPE_CPU:
                std::cout << "    Dev type :: CPU " << std::endl;
                break;
            case CL_DEVICE_TYPE_GPU:
                std::cout << "    Dev type :: GPU " << std::endl;
                break;
            default:
                std::cout << "    Dev type :: unknown " << std::endl;
                break;
            }
            /*std::cout << "Results for this device will be stored in file \"" <<
                         dev_name.data() << "\"\n";*/

            char* image_data = new char[image_height * image_width * sizeof(cl_uint)];

            int cl_error;
            auto cl_context = clCreateContext(nullptr, 1 /*device*/, &device, nullptr, nullptr, &cl_error);
            OCL_CHECK_PARAM(cl_error);
            auto cl_queue = clCreateCommandQueue(cl_context, device, 0, &cl_error);
            OCL_CHECK_PARAM(cl_error);

            const char* filename = "../OCL_test/src/cl/source.cl";
            FILE* f = fopen(filename, "rb");
            if(!f) {
                std::cout << "Failed to open function source code. Exiting...\n";
            }
            fseek(f, 0, SEEK_END);
            size_t size = ftell(f);
            char* src = new char[size];
            rewind(f);
            fread(src, sizeof(char), size, f);
            size_t src_len = strlen(src);

            auto cl_program = clCreateProgramWithSource(
                cl_context,
                1 /*piece of source code*/,
                const_cast<const char **>(&src),
                &src_len,
                &cl_error
            );
            OCL_CHECK_PARAM(cl_error);
            const char *options = "";
            //OCL_SAFE_CALL(clBuildProgram(cl_program, 1, &device, options, nullptr, nullptr));
            cl_error = clBuildProgram(cl_program, 1, &device, options, nullptr, nullptr);
            if(cl_error == CL_BUILD_PROGRAM_FAILURE) {
                size_t messageSize;
                OCL_SAFE_CALL(clGetProgramBuildInfo(cl_program, device, CL_PROGRAM_BUILD_LOG,
                                                    0, nullptr, &messageSize));
                std::vector<unsigned char> message(messageSize);
                OCL_SAFE_CALL(clGetProgramBuildInfo(cl_program, device, CL_PROGRAM_BUILD_LOG,
                                                    messageSize, message.data(), nullptr));
                std::cout << "Build failure" << std::endl;
                throw std::runtime_error(std::string(message.begin(), message.end()));
            }


            // ---
            auto cl_kernel = clCreateKernel(cl_program, "mandelbrot", &cl_error);
            OCL_CHECK_PARAM(cl_error);

            cl_mem buffer = clCreateBuffer(
                        cl_context, CL_MEM_WRITE_ONLY,
                        sizeof(cl_uint) * image_width * image_height, nullptr, nullptr
            );
            if(!buffer) {
                std::string message = "clCreateBuffer did not allocate buffer";
                throw std::runtime_error(message);
            }

            OCL_SAFE_CALL(clSetKernelArg(cl_kernel, 0, sizeof(int), &image_width));
            OCL_SAFE_CALL(clSetKernelArg(cl_kernel, 1, sizeof(int), &image_height));
            OCL_SAFE_CALL(clSetKernelArg(cl_kernel, 2, sizeof(int), &iterations));
            OCL_SAFE_CALL(clSetKernelArg(cl_kernel, 3, sizeof(cl_mem), &buffer));

            size_t local[2] = { 256, 1 };
            size_t global[2] = { align(image_width, local[0]), align(image_height, local[1]) };

            auto start = std::chrono::high_resolution_clock::now();
            OCL_SAFE_CALL(clEnqueueNDRangeKernel(
                        cl_queue, cl_kernel, 2, nullptr, global, local, 0, nullptr, nullptr
            ));
            OCL_SAFE_CALL(clEnqueueReadBuffer(
                        cl_queue, buffer, CL_TRUE, 0,
                        sizeof(cl_uint) * image_width * image_height, image_data, 0, nullptr, nullptr
            ));

            OCL_SAFE_CALL(clFlush(cl_queue));
            OCL_SAFE_CALL(clFinish(cl_queue));
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "OpenCL version: Elapsed time: " << duration.count() << " ms\n";

            OCL_SAFE_CALL(clReleaseKernel(cl_kernel));
            OCL_SAFE_CALL(clReleaseProgram(cl_program));
            OCL_SAFE_CALL(clReleaseMemObject(buffer));
            OCL_SAFE_CALL(clReleaseCommandQueue(cl_queue));
            OCL_SAFE_CALL(clReleaseContext(cl_context));

            png_bytepp row_pointers = new png_byte* [image_height];
            for(int i = 0; i < image_height; ++i) {
                row_pointers[i] = new png_byte [image_width * sizeof(cl_uint)];
                /*for(int j = 0; j < image_width; ++j) {
                    row_pointers[i][j] = image_data[i * image_width + j];
                }*/
            }
            for(int i = 0; i < 4* image_width * image_height; ++i) {
                row_pointers[i / (4*image_width)][i % (4*image_width)] = image_data[i];
            }

            Image image;
            image.width = image_width;
            image.height = image_height;
            image.row_pointers = (png_bytepp)row_pointers;
            image.color_type = PNG_COLOR_TYPE_RGBA;
            image.bit_depth = 8;
            image.write("/home/michael/Desktop/output.png");

            save_result((const uint*)image_data, std::string("/home/michael/Desktop/output.ppm"),
                        image_width, image_height);
        }
    }

    return 0;
}
