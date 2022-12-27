cmake_minimum_required(VERSION 3.23)

add_subdirectory(libs)

project(lb5)

set(CMAKE_CXX_STANDARD 17)

add_executable(lb5 main.cpp MandelbrotCPU.cpp MandelbrotCPU.h Mandelbrot.cpp Mandelbrot.h)

target_link_libraries(${PROJECT_NAME} libclew)
