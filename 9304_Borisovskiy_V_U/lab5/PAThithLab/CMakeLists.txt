cmake_minimum_required(VERSION 3.23)
project(PAThithLab)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs)

configure_file(mandelbrot.cl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(${PROJECT_NAME}
        main.cpp
        cpu_mandelbrot.cpp
        cpu_mandelbrot.h
        )

target_link_libraries(${PROJECT_NAME} libclew)
