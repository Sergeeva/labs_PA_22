#include <spdlog/spdlog.h>
#include <fstream>
#include "../headers/InputReader.h"

void InputReader::run() {
    spdlog::info("Start InputReader");

    read();
}

void InputReader::read_matrix(std::ifstream &source_file, Matrix &matrix) {
    int height;
    int width;

    source_file >> height;
    source_file >> width;

    spdlog::info("read_matrix: reading from file height: {}, width: {}", height, width);

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

void InputReader::read() const {
    std::ifstream input_file(file_name);

    read_matrix(input_file, holder.get_first());
    read_matrix(input_file, holder.get_second());

    spdlog::info("InputReader wrote matrices");

    input_file.close();
}
