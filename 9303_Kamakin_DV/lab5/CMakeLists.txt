cmake_minimum_required(VERSION 3.1)

project(mandelbrot)

find_package(OpenCL REQUIRED)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} src/main.cpp src/save_cl.h src/save_cl.cpp src/util.cpp src/util.h src/opencl_mandelbrot.cpp src/opencl_mandelbrot.h src/serial_mandelbrot.cpp src/serial_mandelbrot.h src/svpng.h src/svpng.cpp)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)
