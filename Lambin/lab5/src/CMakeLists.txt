cmake_minimum_required(VERSION 3.16)

add_subdirectory(libs)

project(lab5)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab5 main.cpp mandelbrot.cpp mandelbrot.h)
target_link_libraries(lab5 libclew)
