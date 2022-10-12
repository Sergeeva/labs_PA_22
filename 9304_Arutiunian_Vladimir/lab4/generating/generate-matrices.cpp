#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void print_matrix(std::ofstream& output, size_t height, size_t width);

int main() {
    size_t power = 10;
    size_t first_matrix_height = 1 << power;
    size_t first_matrix_width = 1 << power;
    size_t second_matrix_width = 1 << power;
    size_t second_matrix_height = first_matrix_width;

    std::string filename = "generated-matrices.txt";
    std::ofstream output(filename);
    std::vector<std::vector<int>> matrix;

    print_matrix(output, first_matrix_height, first_matrix_width);
    print_matrix(output, second_matrix_height, second_matrix_width);

    std::cout << "Check matrices in ./" + filename;
    return 0;
}

void print_matrix(std::ofstream& output, size_t height, size_t width) {
    output << height << ' ' << width << '\n';
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            output << (i + 1) * (j + 1) % 17;
            if (j < width - 1) {
                output << ' ';
            }
        }
        output << '\n';
    }
    output << '\n';
}