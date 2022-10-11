#include <thread>
#include <sstream>
#include "../headers/Log.h"

void Log::info(const std::string &message) {
    std::stringstream stream;
    stream << std::this_thread::get_id();
    auto thread_id_str = stream.str();

    std::cout << ("[Thread " + thread_id_str + "] " + message + " \n");
}