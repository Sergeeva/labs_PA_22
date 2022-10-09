cmake_minimum_required(VERSION 3.23)
project(labs_PA_22_lab2)

set(SOURCE_DIR ".")

configure_file(${SOURCE_DIR}/input.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(CMAKE_CXX_STANDARD 17)

include_directories(${SOURCE_DIR})

add_executable(labs_PA_22
        ${SOURCE_DIR}/blocking-queue/blocking-queue.h
        ${SOURCE_DIR}/blocking-queue/blocking-queue.tpp
        ${SOURCE_DIR}/constant/filenames.h
        ${SOURCE_DIR}/matrix/matrix.h
        ${SOURCE_DIR}/matrix/matrix.cpp
        ${SOURCE_DIR}/main.cpp
        main.h)