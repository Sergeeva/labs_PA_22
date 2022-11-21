#include <filesystem>
#include "../headers/Starter.h"
#include "spdlog/spdlog.h"
#include "../headers/SharedMemory.h"
#include "../headers/Configuration.h"

void init_logger() {
    spdlog::set_pattern("[%H:%M:%S %z] [process %P] [thread %t] %v");
}

void resize_shared_memory() {
    auto file_size = std::filesystem::file_size(Configuration::input_file_name);

    SharedMemory(boost::interprocess::mode_t::read_write, Configuration::result_matrix_memory)
            .resize(file_size)
            .map_region(boost::interprocess::mode_t::read_write)
            .clear();

    SharedMemory(boost::interprocess::mode_t::read_write, Configuration::two_matrices_memory)
            .resize(file_size * 1.5)
            .map_region(boost::interprocess::mode_t::read_write)
            .clear();
}

int main() {
    init_logger();
    resize_shared_memory();
    Starter().run();
    return 0;
}
