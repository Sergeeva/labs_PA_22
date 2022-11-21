cmake_minimum_required(VERSION 3.23)
project(processes)

set(CMAKE_CXX_STANDARD 17)

add_executable(processes main.cpp
        sharedmemory.cpp sharedmemory.h
        readingprocess.cpp readingprocess.h
        calculationprocess.cpp calculationprocess.h
        writingprocess.cpp writingprocess.h
        definitions.h)
