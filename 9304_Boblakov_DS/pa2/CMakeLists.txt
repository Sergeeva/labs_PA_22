cmake_minimum_required(VERSION 3.23)
project(pa2)

set(CMAKE_CXX_STANDARD 20)

SET(FLAGS "-pthread")
SET(CMAKE_CXX_FLAGS  "-pthread")

add_executable(pa2 main.cpp utils.h)
