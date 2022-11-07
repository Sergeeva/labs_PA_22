cmake_minimum_required(VERSION 3.23)
project(lab1_pa)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1_pa main.cpp matrixFunc.cpp matrixFunc.h fileWriter.cpp fileWriter.h processes.cpp processes.h threads.cpp threads.h)
