cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 23)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")

add_executable(untitled src/main.cpp src/Starter.cpp headers/Starter.h headers/Worker.h src/InputReader.cpp headers/InputReader.h src/ParallelSummator.cpp headers/ParallelSummator.h src/OutputWriter.cpp headers/OutputWriter.h headers/Runnable.h src/Matrix.cpp headers/Matrix.h src/Configuration.cpp headers/Configuration.h src/MatricesHolder.cpp headers/MatricesHolder.h headers/SynchronizedQueue.h src/SummatorThread.cpp headers/SummatorThread.h src/SumTask.cpp headers/SumTask.h headers/Log.h src/Log.cpp)
