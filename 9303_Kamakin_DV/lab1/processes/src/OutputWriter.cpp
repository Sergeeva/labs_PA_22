#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include "../headers/OutputWriter.h"
#include "../headers/MatrixReader.h"
#include "../headers/Configuration.h"

void OutputWriter::run() {
    spdlog::info("Start OutputWriter");

    print_result(SharedMemory(boost::interprocess::mode_t::read_only, Configuration::result_matrix_memory)
                         .map_region(boost::interprocess::mode_t::read_only));
}

void OutputWriter::print_result(SharedMemory &memory) {
    memory.wait();

    auto result = MatrixReader().read_from([&memory]() { return memory.read(); });

    std::ofstream matrix_output_file(Configuration::output_file_name);

    matrix_output_file << result.get_height() << ' ' << result.get_width() << '\n';

    for (auto y = 0; y < result.get_height(); y++) {
        for (auto x = 0; x < result.get_width(); x++) {
            matrix_output_file << result.get(y, x);

            if (x != result.get_width() - 1) {
                matrix_output_file << ' ';
            }
        }
        matrix_output_file << '\n';
    }

    matrix_output_file.close();
}