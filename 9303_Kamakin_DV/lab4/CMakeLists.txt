cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 23)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")

add_executable(untitled src/Starter.cpp headers/Starter.h headers/Worker.h src/main.cpp src/InputReader.cpp headers/InputReader.h src/ParallelMultiplier.cpp headers/ParallelMultiplier.h src/OutputWriter.cpp headers/OutputWriter.h headers/Runnable.h src/Matrix.cpp headers/Matrix.h src/Configuration.cpp headers/Configuration.h src/MatricesHolder.cpp headers/MatricesHolder.h headers/SynchronizedQueue.h src/MultiplyingThread.cpp headers/MultiplyingThread.h headers/Log.h src/Log.cpp src/Configurator.cpp headers/Configurator.h src/SimpleMatricesMultiplier.cpp headers/SimpleMatricesMultiplier.h headers/MatricesMultiplier.h src/StrassenMatricesMultiplier.cpp headers/StrassenMatricesMultiplier.h src/MatrixHelper.cpp)
