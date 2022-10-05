cmake_minimum_required(VERSION 3.23)
project(untitled)

if (NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
endif ()


set(CMAKE_CXX_STANDARD 23)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")

add_executable(untitled src/main.cpp src/Starter.cpp headers/Starter.h headers/Worker.h src/InputReader.cpp headers/InputReader.h src/Summator.cpp headers/Summator.h src/OutputWriter.cpp headers/OutputWriter.h headers/Runnable.h src/Matrix.cpp headers/Matrix.h src/Configuration.cpp headers/Configuration.h src/MatricesHolder.cpp headers/MatricesHolder.h)
target_link_libraries(untitled PRIVATE spdlog::spdlog_header_only)
