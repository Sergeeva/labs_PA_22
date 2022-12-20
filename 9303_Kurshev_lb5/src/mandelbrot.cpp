#include "mandelbrot.hpp"

size_t align(size_t x, size_t y) {
    size_t remainder = x % y;
    return (remainder == 0) ? x : x - remainder + y;
}

cl_device_id get_device(){
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, nullptr);

    cl_int error = 0;
    cl_device_id device;
    error |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (error == CL_DEVICE_NOT_FOUND)
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, nullptr);
    return device;

}

cl_context create_context(cl_device_id device) {
    cl_int error = 0;
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &error);
    return context;
}

cl_command_queue create_queue(cl_context context, cl_device_id device) {
    cl_int error = 0;
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error);
    return queue;
}

std::string get_program(){
    std::ifstream file("./mandelbrot.cl");
    std::string file_text = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
    return file_text;
}

cl_program build(const cl_context context, const cl_device_id device) {
    cl_int error;

    std::string src = get_program();
    const char *src_pointer = src.data();
    size_t src_length = src.size();

    cl_program program = clCreateProgramWithSource(context, 1, &src_pointer, &src_length, &error);
    error = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    return program;
}

cl_kernel create_kernel(cl_program program){
    cl_int error = 0;
    cl_kernel kernel = clCreateKernel(program, "mandelbrot", &error);
    return kernel;
}

cl_mem create_buffer(cl_context context, size_t size){
    cl_int error = 0;
    cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size, nullptr, &error);
    return buffer;
}

void start_kernel(cl_kernel kernel, cl_command_queue queue, cl_mem buffer, cl_uint *result, float coeff, int iterations, int width, int height){
    clSetKernelArg(kernel, 0, sizeof(float), &coeff);
    clSetKernelArg(kernel, 1, sizeof(int), &iterations);
    clSetKernelArg(kernel, 2, sizeof(cl_int), &width);
    clSetKernelArg(kernel, 3, sizeof(cl_int), &height);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &buffer);

    size_t local_size[2] = { 256, 1 };
    size_t global_size[2] = { align(width, local_size[0]),align(height, local_size[1]) };



    clock_t start_time = clock();
    clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global_size, local_size, 0, nullptr, nullptr);
    clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(cl_uint) * width * height, result, 0, nullptr, nullptr);

    clFlush(queue);
    clFinish(queue);
    std::cout << "Time - " << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
}

void release(cl_kernel kernel, cl_mem buffer, cl_command_queue queue, cl_program program, cl_context context){
    clReleaseKernel(kernel);
    clReleaseMemObject(buffer);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);
}

void mandelbrot(cl_uint* result, float coeff, int width, int height, int iterations) {
    cl_device_id device = get_device();
    cl_context context = create_context(device);
    cl_program program = build(context, device);
    cl_kernel kernel = create_kernel(program);
    cl_mem buffer = create_buffer(context, sizeof(cl_uint) * width * height);
    cl_command_queue queue = create_queue(context, device);
    start_kernel(kernel, queue, buffer, result, coeff, iterations, width, height);
    release(kernel, buffer, queue, program, context);

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
    file.close();
}

void start() {
    int wight = 1920;
    int height = 1080;
    int coeff = 5;
    int iterations = 256;

    if (!ocl_init()) throw;

    std::vector<cl_uint> result(wight * height);
    mandelbrot(result.data(), coeff, wight, height, iterations);
    save_result(result.data(), "./result.ppm", wight, height);
}
