cmake_minimum_required(VERSION 3.23)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(lab2 main.cpp threads.cpp threads.h buffer.h definition.h)