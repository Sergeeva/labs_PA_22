cmake_minimum_required(VERSION 3.1)

project(enumDevices LANGUAGES CXX)

add_subdirectory(libs)

set(CMAKE_CXX_STANDARD 11)

add_executable(enumDevices src/main.cpp src/libpng_boilerplate_code.cpp src/libpng_boilerplate_code.h src/mandelbrot_cpu.cpp src/mandelbrot_cpu.h)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "lb5")
target_link_libraries(enumDevices libclew png)
