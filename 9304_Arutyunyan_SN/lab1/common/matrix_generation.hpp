#pragma once

#include <vector>
#include <random>
#include <iostream>

#include "matrix.hpp"

namespace {

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution distrib(0, 1000);

int GenerateNumber() {
    return distrib(gen);
}

}

Matrix GenerateMatrix(std::size_t N, std::size_t M) {
    Matrix matrix(N, Row(M));
    
    for (auto& row : matrix) {
        for (std::size_t i = 0; i < M; ++i) {
            row[i] = GenerateNumber();
        }
    }

    return matrix;
}

MatrixPair GenerateMatrixPair(std::size_t N, std::size_t M) {
    return std::make_pair(GenerateMatrix(N, M), GenerateMatrix(N, M));
}
