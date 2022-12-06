#include <fstream>
#include "util.h"
#include <sstream>

std::string read_file(const std::string &file_name) {
    std::ifstream file(file_name);

    if (!file) {
        throw std::runtime_error("Failed to read file: " + file_name);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}