#ifndef PAFIRSTLAB_UTILS_H
#define PAFIRSTLAB_UTILS_H

#include <iostream>
#include <sys/shm.h>
#include <vector>
#include <random>
#include <unistd.h>
#include <thread>
#include <fstream>

#define N 15
#define M 15

typedef std::vector<std::vector<int>> Matrix;

Matrix generateMatrix(int width, int height) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-1000, 1000); // define the range

    Matrix matrix(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix[i][j] = distr(gen);
        }
    }

    return matrix;
}

void printMatrix(Matrix matrix, std::ostream &ostream) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            ostream << matrix[i][j] << " ";
        }

        ostream << "\n";
    }

    ostream << "\n\n";
}

#endif //PAFIRSTLAB_UTILS_H
