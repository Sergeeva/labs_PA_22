cmake_minimum_required(VERSION 3.23)
project(Lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab3 main.cpp matrix/Matrix.cpp matrix/Matrix.h config/Config.cpp config/Config.h utilities/Generator.cpp utilities/Generator.h matrix/MatrixHandler.cpp matrix/MatrixHandler.h buffers/Node.h buffers/Treiber_stack.h utilities/session_timer.cpp utilities/session_timer.h buffers/BQueue.h)


