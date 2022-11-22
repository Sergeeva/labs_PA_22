cmake_minimum_required(VERSION 3.16)

project(lab4)

add_executable(
        lab4
        task/task.cpp
        matrix/Matrix.cpp
        util/util.cpp
)

find_package(Threads REQUIRED)

target_link_libraries(lab4 PRIVATE Threads::Threads)
