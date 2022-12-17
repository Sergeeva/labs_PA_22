#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <CL/opencl.hpp>

#include "matrix.hpp"

constexpr char CL_FILE_PATH[] = "./matrix_kernel.cl";
constexpr unsigned MATRIX_SIZE = 10;



int main()
{
    // MatrixSpace::Matrix f(MATRIX_SIZE);
    // MatrixSpace::generateMatrix(f);
    // MatrixSpace::readMatrix(f);

    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);
    
    if(!allPlatforms.size())
    {
        std::cout << " No platforms found. Check OpenCL installation!" << std::endl;
        return 1;
    }

    cl::Platform defaultPlatform = allPlatforms[0];
    std::cout << "Using platform: " << defaultPlatform.getInfo<CL_PLATFORM_NAME>() << '\n';

    std::vector<cl::Device> allDevices;

    defaultPlatform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);

    if(!allDevices.size())
    {
        std::cout << " No devices found. Check OpenCL installation!" << std::endl;
        return 1;
    }

    cl::Device defaultDevice = allDevices[0];
    std::cout << "Using device: " << defaultDevice.getInfo<CL_DEVICE_NAME>() << '\n';

    cl::Context context(defaultDevice);

    std::string cl_program_text;
    std::getline(std::ifstream(CL_FILE_PATH), cl_program_text, '\0');

    cl::Program::Sources sources;
    sources.push_back({cl_program_text.c_str(), cl_program_text.length()});


    cl::Program program(context, sources);

    if (program.build(defaultDevice) != CL_SUCCESS)
    {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice) << "\n";
        return 1;
    }


    cl::Buffer firstMatrixBuffer(context,  CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer secondMatrixBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer thirdMatrixBuffer(context,  CL_MEM_READ_WRITE, sizeof(int) * 10);

    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

    cl::CommandQueue queue(context, defaultDevice);

    queue.enqueueWriteBuffer(firstMatrixBuffer,  CL_TRUE, 0, sizeof(int) * 10, A);
    queue.enqueueWriteBuffer(secondMatrixBuffer, CL_TRUE, 0, sizeof(int) * 10, B);


    // cl::Kernel kernel(program, "simple_add");

    auto simple_add = cl::KernelFunctor<cl::Buffer, cl::Buffer, cl::Buffer>(program, "simple_add");
    auto args = cl::EnqueueArgs(queue, cl::NullRange, cl::NDRange(10), cl::NullRange);

    simple_add(args, firstMatrixBuffer, secondMatrixBuffer, thirdMatrixBuffer);

    int C[10];
    queue.enqueueReadBuffer(thirdMatrixBuffer, CL_TRUE, 0, sizeof(int) * 10, C);

    std::cout << " result: \n";
    for (int i = 0; i < 10; i++) {
        std::cout << C[i] << " ";
    }

    return 0;
}