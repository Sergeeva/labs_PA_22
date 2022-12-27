cmake_minimum_required(VERSION 3.24)
project(pa5)
add_subdirectory(libs)

set(CMAKE_CXX_STANDARD 20)

#add_executable(pa5 main.cpp)


add_executable(pa5 main.cpp mandelbrot.cpp mandelbrot.h)
target_link_libraries(pa5 libclew)
