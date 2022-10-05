cmake_minimum_required(VERSION 3.23)
project(PAFirstLab)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory("./firstTask")
add_subdirectory("./secondTask")
add_subdirectory("./thirdTask")

add_executable(PAFirstLab utils.h utils.cpp)
