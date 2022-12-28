cmake_minimum_required(VERSION 3.16)

project(lab5)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs)

add_executable(${PROJECT_NAME} main.cpp mandelbrot.cpp mandelbrot.hpp mandelbrot_cpu.cpp mandelbrot_cpu.hpp)

target_link_libraries(${PROJECT_NAME} libclew)
