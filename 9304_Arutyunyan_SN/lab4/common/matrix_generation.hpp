#pragma once

#include <vector>
#include <random>
#include <iostream>

#include "matrix.hpp"

namespace {

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution distrib(0, 100);

int GenerateNumber() {
    return distrib(gen);
}

}

template <typename T>
MatrixPtr<T> GenerateMatrix(std::size_t N, std::size_t M) {
    MatrixPtr<T> matrix = std::make_shared<Matrix<T>>(N, M);
    
    for (auto& row : *matrix) {
        for (std::size_t i = 0; i < M; ++i) {
            row[i] = GenerateNumber();
        }
    }

    return matrix;
}

template <typename T, typename U = T>
MatrixPtrPair<T, U> GenerateMatrixPair(std::size_t N, std::size_t M) {
    return std::make_pair(GenerateMatrix<T>(N, M), GenerateMatrix<T>(N, M));
}
