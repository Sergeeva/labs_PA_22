#include "Mandelbrot.h"

Mandelbrot::Mandelbrot(unsigned int width, unsigned int height, unsigned int iterations, float scale, float offsetX, float offsetY): width_(width), height_(height), iterations_(iterations), scale_(scale), offsetX_(offsetX), offsetY_(offsetY){
    result_.resize(width*height);
}

cl_device_id createDevice(){
    cl_platform_id platform;
    cl_device_id device;
    cl_int err = 0;
    err |= clGetPlatformIDs(1, &platform, nullptr);
    err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, nullptr);
    if (err) std::cout << "Device error - " << err << std::endl;
    return device;
}

std::string getProgramText(){
    std::ifstream kernelcode("./Mandelbrot.cl");
    return std::string(std::istreambuf_iterator<char>(kernelcode), std::istreambuf_iterator<char>());
}

cl_program buildProgram(cl_context context, cl_device_id device){
    cl_int err = 0;
    std::string src = getProgramText();
    const char* srcText = src.data();
    size_t srcLength = src.size();
    cl_program program = clCreateProgramWithSource(context, 1, &srcText, &srcLength, nullptr);
    err |= clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    if (err) std::cout << "Build error - " << err << std::endl;
    return program;
}

void Mandelbrot::vectorToPPM(std::string filename) {
    std::ofstream file(filename, std::ios::binary);
    file << "P3\n" << width_ << ' ' << height_ << ' ' << 255 << std::endl;
    for (unsigned int i = 0; i < width_ * height_; i++){
        file << (255 - result_[i]) << ' ' << (255 - result_[i]) << ' ' << (255 - (result_[i] * 15) % 256) << std::endl;
    }
    file.close();
}

void Mandelbrot::mandelbrot(){
    if (!ocl_init())
        return;
    cl_int err = 0;
    cl_device_id device = createDevice();
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, nullptr);
    cl_program program = buildProgram(context, device);
    cl_kernel kernel = clCreateKernel(program, "get_point", &err);
    if (err) std::cout << "Kernel error - " << err << std::endl;
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    if (err) std::cout << "Queue error - " << err << std::endl;
    cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uint) * width_ * height_, nullptr, &err);
    if (err) std::cout << "Buffer error - " << err << std::endl;

    clSetKernelArg(kernel, 0, sizeof(float), &offsetX_);
    clSetKernelArg(kernel, 1, sizeof(float), &offsetY_);
    clSetKernelArg(kernel, 2, sizeof(float), &scale_);
    clSetKernelArg(kernel, 3, sizeof(unsigned int), &iterations_);
    clSetKernelArg(kernel, 4, sizeof(int), &width_);
    clSetKernelArg(kernel, 5, sizeof(int), &height_);
    clSetKernelArg(kernel, 6, sizeof(cl_mem), &buffer);

    size_t local_size[2] = {256, 1};
    size_t global_size[2] = {(width_+local_size[0]-1)/local_size[0]*local_size[0],(height_+local_size[1]-1)/local_size[1]*local_size[1]};

    auto start = std::chrono::high_resolution_clock::now();

    clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global_size, local_size, 0, nullptr, nullptr);
    clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(cl_uint) * width_ * height_, result_.data(), 0, nullptr, nullptr);
    clFlush(queue);
    clFinish(queue);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Duration (GPU) - " << duration.count() << "ms" << std::endl;

    clReleaseKernel(kernel);
    clReleaseMemObject(buffer);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);

    vectorToPPM("./GPU.ppm");
}