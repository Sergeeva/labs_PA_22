cmake_minimum_required(VERSION 3.9)
project(project)

set(CMAKE_CXX_STANDARD 17)

add_executable(project source.cpp configuration/config.h configuration/config.cpp matrices/Matrix.cpp matrices/Matrix.h utility/services/file_handler.cpp utility/services/file_handler.h utility/services/Generator.cpp utility/services/Generator.h first_task/shared_memory/SharedMemory.cpp first_task/shared_memory/SharedMemory.h first_task/ProcessHandler.cpp first_task/ProcessHandler.h  second_task/ThreadHandler.cpp second_task/ThreadHandler.h utility/DataHandler.cpp utility/DataHandler.h third_task/MultiThread.cpp third_task/MultiThread.h utility/session_timer.cpp utility/session_timer.h)