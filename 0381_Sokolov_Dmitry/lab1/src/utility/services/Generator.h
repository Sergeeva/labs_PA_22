#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <vector>
#include <random>

#include "../../configuration/config.h"


class Generator {
    int ceiling = Config::generation_ceiling;

public:
    Generator() = default;

    explicit Generator(int ceiling);

    void change_ceiling(int value);

    std::vector<std::vector<int>> generate(int rows, int columns, unsigned int seed = 101);
};


#endif //PROJECT_GENERATOR_H
