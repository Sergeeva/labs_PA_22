cmake_minimum_required(VERSION 3.5)

project(lb2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lb2 main.cpp args/args.cpp args/args.h matrix/matrix.cpp matrix/matrix.h sync-buffer/syncbuffer.cpp sync-buffer/syncbuffer.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lb2 PRIVATE Threads::Threads)
