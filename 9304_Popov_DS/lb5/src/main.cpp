#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <CL/cl.hpp>

#include "matrix.hpp"

constexpr char CL_FILE_PATH[] = "./matrix_kernel.cl";
constexpr unsigned MATRIX_SIDE = 1'024;
#define DURATION_TIME

#ifdef DURATION_TIME
#include <chrono>
std::chrono::_V2::steady_clock::time_point begin;
std::chrono::_V2::steady_clock::time_point end;
std::chrono::milliseconds duration;
#endif



int main()
{
    MatrixSpace::Matrix firstMatrix(MATRIX_SIDE);
    MatrixSpace::Matrix secondMatrix(MATRIX_SIDE);
    MatrixSpace::Matrix resultMatrix(MATRIX_SIDE);


    MatrixSpace::generateMatrix(firstMatrix);
    MatrixSpace::generateMatrix(secondMatrix);

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

    if (program.build(allDevices) != CL_SUCCESS)
    {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice) << "\n";
        return 1;
    }

    const unsigned matrixSize = pow(MATRIX_SIDE, 2);

    cl::Buffer firstMatrixBuffer(context,  CL_MEM_READ_WRITE, sizeof(int) * matrixSize);
    cl::Buffer secondMatrixBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * matrixSize);
    cl::Buffer thirdMatrixBuffer(context,  CL_MEM_READ_WRITE, sizeof(int) * matrixSize);

    cl::CommandQueue queue(context, defaultDevice);

#ifdef DURATION_TIME
    begin = std::chrono::steady_clock::now();
#endif

    queue.enqueueWriteBuffer(firstMatrixBuffer,
                            CL_TRUE,
                            0,
                            sizeof(int) * matrixSize,
                            firstMatrix._data.data());

    queue.enqueueWriteBuffer(secondMatrixBuffer,
                            CL_TRUE,
                            0,
                            sizeof(int) * matrixSize,
                            secondMatrix._data.data());


    cl::Kernel kernel(program, "multiplication");

    kernel.setArg(0, firstMatrixBuffer);
    kernel.setArg(1, secondMatrixBuffer);
    kernel.setArg(2, thirdMatrixBuffer);
    kernel.setArg(3, MATRIX_SIDE);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(matrixSize), cl::NullRange);

    queue.enqueueReadBuffer(thirdMatrixBuffer,
                            CL_TRUE,
                            0,
                            sizeof(int) * matrixSize,
                            resultMatrix._data.data());

#ifdef DURATION_TIME
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Multithread solution: " << duration.count() << " ms\n";
#endif

    return 0;
}