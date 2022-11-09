cmake_minimum_required(VERSION 3.23)
project(labs_PA_22_lab3)

set(CMAKE_CXX_STANDARD 17)

configure_file(
        input.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(labs_PA_22
        constant/filenames.h
        lock-free-queue/lock-free-queue.tpp
        lock-free-queue/lock-free-queue.h
        lock-free-queue/node/node.tpp
        lock-free-queue/node/node.h
        matrix/matrix.cpp
        matrix/matrix.h
        logger/logger.cpp
        logger/logger.h
        time/Time.cpp
        time/Time.h
        main.cpp
        main.h)
