#include <spdlog/spdlog.h>
#include <iostream>
#include "../headers/Summator.h"
#include "../headers/MatrixReader.h"
#include "../headers/SharedMemory.h"
#include "../headers/Configuration.h"

void Summator::run() {
    spdlog::info("Start Summator");

    auto result = sum(SharedMemory(boost::interprocess::mode_t::read_only, Configuration::two_matrices_memory)
                              .map_region(boost::interprocess::mode_t::read_only));

    write_result(SharedMemory(boost::interprocess::mode_t::read_write, Configuration::result_matrix_memory)
                         .map_region(boost::interprocess::mode_t::read_write),
                 result);

}

void Summator::write_result(SharedMemory &memory, Matrix &result) {
    memory.lock();

    spdlog::info("Summator writing result, height {}, width {}", result.get_height(), result.get_width());

    memory.write(result.get_height());
    memory.write(result.get_width());

    for (auto y = 0; y < result.get_height(); y++) {
        for (auto x = 0; x < result.get_width(); x++) {
            memory.write(result.get(y, x));
        }
    }

    spdlog::info("Summator wrote result matrix");

    memory.unlock();
}

Matrix Summator::sum(SharedMemory &memory) {
    auto reader = MatrixReader();
    auto elemSupplier = [&memory]() { return memory.read(); };

    memory.wait();

    spdlog::info("Summator start reading matrices");

    auto first = reader.read_from(elemSupplier);
    auto second = reader.read_from(elemSupplier);

    spdlog::info("Summator matrices read, total size (height, width): first({},{}), second({},{})",
                 first.get_height(), first.get_width(), second.get_height(), second.get_width());

    return first + second;
}