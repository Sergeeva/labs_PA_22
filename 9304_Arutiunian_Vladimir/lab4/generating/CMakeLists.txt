cmake_minimum_required(VERSION 3.23)
project(labs_PA_22_lab4)

set(SOURCE_DIR ".")

set(CMAKE_CXX_STANDARD 17)

include_directories(${SOURCE_DIR})

add_executable(labs_PA_22_lab4_generating
        ${SOURCE_DIR}/generate-matrices.cpp)