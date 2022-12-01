cmake_minimum_required(VERSION 3.23)
project(PA_lab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(PA_lab2 main.cpp matrix/Matrix.cpp matrix/Matrix.h config/Config.cpp config/Config.h utilities/Generator.cpp utilities/Generator.h matrix/MatrixHandler.cpp matrix/MatrixHandler.h buffer/BQueue.h)
