cmake_minimum_required(VERSION 3.24)
project(pa4)

set(CMAKE_CXX_STANDARD 20)
SET(FLAGS "-pthread")
SET(CMAKE_CXX_FLAGS  "-pthread")
add_executable(pa4 main.cpp)
