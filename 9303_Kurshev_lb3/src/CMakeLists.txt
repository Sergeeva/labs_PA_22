cmake_minimum_required(VERSION 3.16.3)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(lab3 main.cpp Sources/Matrix.cpp Headers/Matrix.h Sources/Buffer.cpp Headers/Buffer.h)
