cmake_minimum_required(VERSION 3.16)

project(lab5)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(libs)
configure_file(mandelbrot.cl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

add_executable(
        ${PROJECT_NAME}
        main.cpp
        mandelbrot.cpp
        mandelbrot.hpp
        mandelbrot_cpu.cpp
        mandelbrot_cpu.hpp
)

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)
