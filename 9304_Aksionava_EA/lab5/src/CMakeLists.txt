cmake_minimum_required(VERSION 3.23)

project(open_cl_mandelbrot)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs)
configure_file(mandelbrot.cl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)


add_executable(
        ${PROJECT_NAME}
        main.cpp
        common.hpp
        mandelbrot.cpp
        mandelbrot.hpp
        mandelbrot_cpu.cpp
        mandelbrot_cpu.hpp
        common.cpp)

target_link_libraries(${PROJECT_NAME} libclew)
