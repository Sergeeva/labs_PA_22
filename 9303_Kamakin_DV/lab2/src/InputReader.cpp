#include <spdlog/spdlog.h>
#include <fstream>
#include "../headers/InputReader.h"

void InputReader::run() {
    spdlog::info("InputReader started");

    std::ifstream input_file(file_name);

    for (auto i = 0; i < iterations_count; i++) {
        spdlog::info("InputReader iteration №{}", i);

        auto holder = MatricesHolder();

        read_matrix(input_file, holder.get_first());
        read_matrix(input_file, holder.get_second());

        spdlog::info("InputReader read matrices");

        queue.push(holder);

        input_file.seekg(0, std::ios::beg);
    }

    input_file.close();
}

void InputReader::read_matrix(std::ifstream &source_file, Matrix &matrix) {
    int height;
    int width;

    source_file >> height;
    source_file >> width;

    matrix.set_height(height);
    matrix.set_width(width);

    int elem;

    for (auto y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            source_file >> elem;

            matrix.set(y, x, elem);
        }
    }
}
