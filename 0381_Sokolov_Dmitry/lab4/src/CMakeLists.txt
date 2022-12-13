cmake_minimum_required(VERSION 3.24)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp matrix/Matrix.cpp matrix/MatrixHandler.cpp utilities/Generator.cpp utilities/session_timer.cpp config/Config.cpp matrix/multiplication/Strassen.cpp matrix/multiplication/Strassen.h)
