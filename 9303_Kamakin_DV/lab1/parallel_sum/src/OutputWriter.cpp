#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include "../headers/OutputWriter.h"
#include "../headers/Configuration.h"

void OutputWriter::run() {
    spdlog::info("Start OutputWriter");

    print_result();
}

void OutputWriter::print_result() {
    spdlog::info("OutputWriter got matrix");

    std::ofstream matrix_output_file(Configuration::output_file_name);

    matrix_output_file << target_matrix.get_height() << ' ' << target_matrix.get_width() << '\n';

    for (auto y = 0; y < target_matrix.get_height(); y++) {
        for (auto x = 0; x < target_matrix.get_width(); x++) {
            matrix_output_file << target_matrix.get(y, x);

            if (x != target_matrix.get_width() - 1) {
                matrix_output_file << ' ';
            }
        }
        matrix_output_file << '\n';
    }

    matrix_output_file.close();
}