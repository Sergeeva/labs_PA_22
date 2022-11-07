cmake_minimum_required(VERSION 3.23)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(lab3 main.cpp threads.cpp threads.h buffer.h node.h definition.h)
