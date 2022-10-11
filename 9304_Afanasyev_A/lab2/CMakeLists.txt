cmake_minimum_required(VERSION 3.16)

project(lab2)

add_executable(lab2
        task/task.cpp
        matrix/Matrix.cpp
        util/util.cpp
        blocking-queue/BlockingQueue.h)

find_package(Threads REQUIRED)

target_link_libraries(lab2 PRIVATE Threads::Threads)
