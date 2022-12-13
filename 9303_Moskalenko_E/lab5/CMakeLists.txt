cmake_minimum_required(VERSION 3.1)

project(mandelbrot)

find_package(OpenCL REQUIRED)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} src/main.cpp src/opencl/errors/save_cl.h src/opencl/errors/save_cl.cpp src/opencl/mandelbrot.cl src/opencl/opencl_mandelbrot.cpp src/opencl/opencl_mandelbrot.h src/one_thread/one_thread_mandelbrot.cpp src/one_thread/one_thread_mandelbrot.h src/svpng/svpng.h src/svpng/svpng.cpp src/one_thread/Color.cpp src/one_thread/Color.h src/params.h)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)
