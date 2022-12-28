cmake_minimum_required(VERSION 3.23)
project(parallel_algorithms_lab5)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs)

configure_file(mandelbrot.cl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(${PROJECT_NAME}
        main.cpp
        sequential_mandelbrot.cpp
        sequential_mandelbrot.h
        time/Time.cpp
        time/Time.h
        opencl_mandelbrot.cpp
        opencl_mandelbrot.h)

target_link_libraries(${PROJECT_NAME} libclew)
