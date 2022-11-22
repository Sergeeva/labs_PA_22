#include "Generator.h"

Generator::Generator(int ceiling) : ceiling(ceiling) {}

void Generator::change_ceiling(int value) {
    this->ceiling = value;
}

std::vector<std::vector<int>> Generator::generate(int rows, int columns, unsigned int seed) {

    std::vector<std::vector<int>> values;

    std::mt19937 engine(seed);
    std::uniform_int_distribution<> range(1, this->ceiling);

    for (int i = 0; i < rows; i++) {

        values.push_back(std::vector<int>());

        for (int j = 0; j < columns; j++) {
            values[i].push_back(range(engine));
        }
    }

    return values;
}