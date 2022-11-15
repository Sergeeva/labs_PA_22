#include <iostream>
#include <fstream>
#include "../headers/OutputWriter.h"

void OutputWriter::run() {
    Log::info("Start OutputWriter");

    std::ofstream matrix_output_file(output_file);

    for (auto i = 0; i < iterations_count; i++) {
        Log::info("OutputWriter iteration #" + std::to_string(i));

        auto matrix_pointer = sums_queue.pop();

        if (matrix_pointer) {
            print_result(matrix_output_file, *matrix_pointer);
        }
    }

    matrix_output_file.close();

    Log::info("OutputWriter work done");
}

void OutputWriter::print_result(std::ofstream &file, Matrix &matrix) {
    file << matrix.get_height() << ' ' << matrix.get_width() << '\n';

    for (auto y = 0; y < matrix.get_height(); y++) {
        for (auto x = 0; x < matrix.get_width(); x++) {
            file << matrix.get(y, x);

            if (x != matrix.get_width() - 1) {
                file << ' ';
            }
        }

        file << '\n';
    }
}