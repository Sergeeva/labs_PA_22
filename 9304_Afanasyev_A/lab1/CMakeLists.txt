cmake_minimum_required(VERSION 3.16)

project(lab1)

configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(lab1
        #        task1/task1.cpp
        #        task2/task2.cpp
        task3/task3.cpp
        matrix/Matrix.cpp
        util/util.cpp)

find_package(Threads REQUIRED)

target_link_libraries(lab1 PRIVATE Threads::Threads)
