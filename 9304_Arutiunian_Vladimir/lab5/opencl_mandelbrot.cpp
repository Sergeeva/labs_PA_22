#include "opencl_mandelbrot.h"

#define OCL_SAFE_CALL(expr) report_error(expr, __FILE__, __LINE__)

template<typename T>
std::string to_string(T value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

void report_error(cl_int err, const std::string& filename, int line) {
    if (CL_SUCCESS != err) {
        throw std::runtime_error(
                "OpenCL error code " + to_string(err) + " encountered at " + filename + ":" + to_string(line)
        );
    }
}

size_t align(size_t x, size_t y) {
    const size_t blocks_count = (x + y - 1) / y;
    return blocks_count * y;
}

void execute(
        cl_kernel kernel,
        cl_command_queue command_queue,
        cl_mem buffer,
        cl_uint* result_pointer,
        float ratio,
        cl_int width,
        cl_int height,
        cl_int iterations_limit
) {
    int argument_index = 0;
    OCL_SAFE_CALL(clSetKernelArg(kernel, argument_index++, sizeof(cl_int), &height));
    OCL_SAFE_CALL(clSetKernelArg(kernel, argument_index++, sizeof(cl_int), &width));
    OCL_SAFE_CALL(clSetKernelArg(kernel, argument_index++, sizeof(float), &ratio));
    OCL_SAFE_CALL(clSetKernelArg(kernel, argument_index++, sizeof(cl_int), &iterations_limit));
    OCL_SAFE_CALL(clSetKernelArg(kernel, argument_index++, sizeof(cl_mem), &buffer));

    std::array<size_t, 2> local_size = {HORIZONTAL_WORK_GROUP_WIDTH, HORIZONTAL_WORK_GROUP_HEIGHT};
    std::array<size_t, 2> global_size = {align(width, local_size[0]), align(height, local_size[1])};

    OCL_SAFE_CALL(
            clEnqueueNDRangeKernel(
                    command_queue,
                    kernel,
                    WORK_DIMENSION,
                    nullptr,
                    global_size.data(),
                    local_size.data(),
                    ZERO_SIZE,
                    nullptr,
                    nullptr
            )
    );

    const size_t buffer_data_size = sizeof(cl_uint) * width * height;
    OCL_SAFE_CALL(
            clEnqueueReadBuffer(
                    command_queue,
                    buffer,
                    CL_TRUE,
                    ZERO_OFFSET,
                    buffer_data_size,
                    result_pointer,
                    ZERO_SIZE,
                    nullptr,
                    nullptr
            )
    );

    clFinish(command_queue);
}

cl_device_id create_device() {
    cl_platform_id platform;
    OCL_SAFE_CALL(clGetPlatformIDs(ONE_ENTRY, &platform, nullptr));

    cl_device_id dev;
    OCL_SAFE_CALL(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, ONE_ENTRY, &dev, nullptr));

    return dev;
}

std::string get_program_code_as_string(const std::string& cl_filename) {
    std::ifstream stream(cl_filename);
    return to_string(stream.rdbuf());
}

cl_program build_program(cl_context ctx, cl_device_id device, const std::string& cl_filename) {
    int error_code;

    std::string code = get_program_code_as_string(cl_filename);
    const char* code_pointer = code.c_str();
    size_t code_size = code.size();

    cl_program program = clCreateProgramWithSource(ctx, ONE_ENTRY, &code_pointer, &code_size, &error_code);
    OCL_SAFE_CALL(error_code);

    error_code = clBuildProgram(program, ZERO_SIZE, nullptr, nullptr, nullptr, nullptr);

    if (CL_BUILD_PROGRAM_FAILURE == error_code) {
        size_t log_size;
        OCL_SAFE_CALL(clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, ZERO_SIZE, nullptr, &log_size));

        std::vector<unsigned char> message(log_size);
        OCL_SAFE_CALL(clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, message.data(), nullptr));

        throw std::runtime_error("Build program failure: " + std::string(message.begin(), message.end()));
    }

    return program;
}

void assertOclInitialized() {
    if (!ocl_init()) {
        throw std::runtime_error("OpenCL not initialized");
    }
}

std::vector<unsigned int> compute_opencl_mandelbrot(
        int height,
        int width,
        float ratio,
        int iterations_limit
) {
    assertOclInitialized();

    const auto cl_filename = "mandelbrot.cl";
    const auto entry_point_function_name = "compute_mandelbrot";

    cl_int error_code;
    cl_device_id device = create_device();
    cl_context context = clCreateContext(nullptr, ONE_ENTRY, &device, nullptr, nullptr, &error_code);

    cl_program program = build_program(context, device, cl_filename);

    cl_kernel kernel = clCreateKernel(program, entry_point_function_name, &error_code);
    OCL_SAFE_CALL(error_code);

    cl_command_queue command_queue = clCreateCommandQueue(context, device, ZERO_CODE, &error_code);
    OCL_SAFE_CALL(error_code);

    const size_t buffer_size = sizeof(cl_uint) * width * height;
    cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, buffer_size, nullptr, &error_code);
    OCL_SAFE_CALL(error_code);

    std::vector<unsigned int> pixels_buffer(width * height);
    execute(kernel, command_queue, buffer, pixels_buffer.data(), ratio, width, height, iterations_limit);

    clReleaseKernel(kernel);
    clReleaseMemObject(buffer);
    clReleaseCommandQueue(command_queue);
    clReleaseProgram(program);
    clReleaseContext(context);

    return pixels_buffer;
}
