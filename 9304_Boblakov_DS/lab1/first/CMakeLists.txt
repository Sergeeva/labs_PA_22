cmake_minimum_required(VERSION 3.23)
project(lab1)

set(CMAKE_CXX_STANDARD 23)

include_directories("./")
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(first ../first/main.cpp ../common.h)