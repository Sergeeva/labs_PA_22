#ifndef PA_LAB2_GENERATOR_H
#define PA_LAB2_GENERATOR_H

#include <vector>
#include <random>

#include "../config/config.h"


class Generator {
    const int ceiling = Config::generation_ceiling;

public:
    Generator() = default;

    explicit Generator(int ceiling);

//    void change_ceiling(int value);

    std::vector<std::vector<int>> generate(int rows, int columns, unsigned int seed = 101) const;
};


#endif //PA_LAB2_GENERATOR_H
