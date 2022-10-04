#include <iostream>
#include <fstream>
#include <vector>

void writeMatrix(std::ofstream& output, int height, int width);

int main() {
    int height = 20;
    int width = 40;

    std::ofstream output("input.txt");
    std::vector<std::vector<int>> matrix;

    writeMatrix(output, height, width);
    writeMatrix(output, height, width);

    output.close();
    return 0;
}

void writeMatrix(std::ofstream& output, int height, int width) {
    output << height << ' ' << width << '\n';
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            output << i * j;
            if (j < width - 1) {
                output << ' ';
            }
        }
        output << '\n';
    }
}
