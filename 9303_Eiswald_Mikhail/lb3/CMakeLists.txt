cmake_minimum_required(VERSION 3.5)

project(lb3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lb3 main.cpp args/args.cpp args/args.h
    matrix/matrix.cpp matrix/matrix.h queue/queue.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lb3 PRIVATE Threads::Threads)
