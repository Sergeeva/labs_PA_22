cmake_minimum_required(VERSION 3.23)
project(labs_PA_22_lab4)

set(CMAKE_CXX_STANDARD 17)

configure_file(input.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(labs_PA_22_lab4
        constant/filenames.h
        matrix/matrix.h
        matrix/matrix.cpp
        logger/logger.cpp
        logger/logger.h
        time/Time.cpp
        time/Time.h
        main.cpp
        main.h)