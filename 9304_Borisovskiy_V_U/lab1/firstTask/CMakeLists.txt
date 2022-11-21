cmake_minimum_required(VERSION 3.23)
project(PAFirstLab)

set(CMAKE_CXX_STANDARD 20)

include_directories("./")

add_executable(FirstTask ../firstTask/firstTask.cpp ../utils.h)
