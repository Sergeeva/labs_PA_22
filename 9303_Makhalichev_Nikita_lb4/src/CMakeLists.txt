cmake_minimum_required(VERSION 3.23)
project(lb4)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lm")

add_executable(lb3 Sources/main.cpp Sources/Buffer.cpp Sources/Matrix.cpp Headers/Buffer.h Headers/Matrix.h)
