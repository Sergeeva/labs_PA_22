//
// Created by Виктор Борисовский on 20.12.2022.
//

#define STRASSEN_MIN_MATRIX_SIZE 128
#define THREADS_NUMBER 4

#include <fstream>
#include <memory>
#include <utility>
#include <vector>
#include <random>
#include <iostream>
#include <thread>
#include <future>
#include "Matrix.h"

typedef std::vector<std::vector<int>> matrix_type;

Matrix::Matrix(int height, int width, matrix_type matrix) : matrix(std::move(matrix)),
                                                                              width(width),
                                                                              height(height) {}

Matrix::Matrix() : Matrix(16, 16) {
    createRandomMatrix();
}

void Matrix::createRandomMatrix() {
    std::srand(std::time(nullptr));
    std::random_device rd; // obtain a createRandomMatrix number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-1000, 1000); // define the range

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix[i][j] = distr(gen);
        }
    }
}

Matrix::Matrix(int height, int width)
        : matrix(matrix_type(height, std::vector<int>(width))), width(width), height(height) {}


Matrix Matrix::operator+(const Matrix &right) const {
    Matrix summedMatrix = Matrix(*this);

    for (int i = 0; i < summedMatrix.height; i++) {
        for (int j = 0; j < summedMatrix.width; j++) {
            summedMatrix.matrix[i][j] += right.matrix[i][j];
        }
    }

    return summedMatrix;
}

Matrix Matrix::operator-(const Matrix &right) const {
    auto subedMatrix = Matrix(*this);

    for (int i = 0; i < subedMatrix.height; ++i) {
        for (int j = 0; j < subedMatrix.width; ++j) {
            subedMatrix.matrix[i][j] -= right.matrix[i][j];
        }
    }

    return subedMatrix;
}

Matrix Matrix::strassenMultiplyMatrices(const Matrix &right, int maxDepth, int currentDepth) const {
    if (width <= STRASSEN_MIN_MATRIX_SIZE) {
        return parallelSimpleMultiplyMatrices(right, THREADS_NUMBER);
    } else if (currentDepth < maxDepth) {
        return strassenMultiplyRecursionParallel(*this, right, maxDepth, currentDepth);
    }

    return strassenMultiplyRecursion(*this, right, maxDepth, currentDepth);
}

void calculateP(const Matrix &left,
                const Matrix &right,
                int maxD,
                int depth,
                Matrix &resultMatrix,
                std::vector<std::thread> &threads) {

    std::thread thread(
            [](const Matrix l, const Matrix r, Matrix &result, int maxDepth, int curDepth) {
                result = l.strassenMultiplyMatrices(r, maxDepth, curDepth);
            },
            left, right, std::ref(resultMatrix), maxD, depth);

    threads.emplace_back(std::move(thread));
}

Matrix Matrix::strassenMultiplyRecursionParallel(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth) {
    currentDepth++;

    std::vector<std::thread> threads;

    Matrix p1;
    calculateP(l.calculateBlock(0, 0) + l.calculateBlock(1, 1), r.calculateBlock(0, 0) + r.calculateBlock(1, 1), maxDepth, currentDepth,
               p1, threads);

    Matrix p2;
    calculateP(l.calculateBlock(1, 0) + l.calculateBlock(1, 1), r.calculateBlock(0, 0), maxDepth, currentDepth, p2, threads);

    Matrix p3;
    calculateP(l.calculateBlock(0, 0), r.calculateBlock(0, 1) - r.calculateBlock(1, 1), maxDepth, currentDepth, p3, threads);

    Matrix p4;
    calculateP(l.calculateBlock(1, 1), r.calculateBlock(1, 0) - r.calculateBlock(0, 0), maxDepth, currentDepth, p4, threads);

    Matrix p5;
    calculateP(l.calculateBlock(0, 0) + l.calculateBlock(0, 1), r.calculateBlock(1, 1), maxDepth, currentDepth, p5, threads);

    Matrix p6;
    calculateP(l.calculateBlock(1, 0) - l.calculateBlock(0, 0), r.calculateBlock(0, 0) + r.calculateBlock(0, 1), maxDepth, currentDepth,
               p6,
               threads);

    Matrix p7;
    calculateP(l.calculateBlock(0, 1) - l.calculateBlock(1, 1), r.calculateBlock(1, 0) + r.calculateBlock(1, 1), maxDepth, currentDepth,
               p7, threads);


    for (auto &thread: threads) {
        thread.join();
    }


    Matrix c21 = p2 + p4;
    Matrix c12 = p3 + p5;
    Matrix c22 = p1 - p2 + p3 + p6;
    Matrix c11 = p1 + p4 - p5 + p7;


    return merge2x2Matrices(c11, c12, c21, c22);
}

Matrix Matrix::strassenMultiplyRecursion(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth) {
    currentDepth++;

    Matrix p1 = (l.calculateBlock(0, 0) + l.calculateBlock(1, 1)).strassenMultiplyMatrices(
            r.calculateBlock(0, 0) + r.calculateBlock(1, 1),
            maxDepth, currentDepth);
    Matrix p2 = (l.calculateBlock(1, 0) + l.calculateBlock(1, 1)).strassenMultiplyMatrices(r.calculateBlock(0, 0),
                                                                                           maxDepth, currentDepth);
    Matrix p3 = l.calculateBlock(0, 0).strassenMultiplyMatrices(r.calculateBlock(0, 1) - r.calculateBlock(1, 1),
                                                                maxDepth, currentDepth);
    Matrix p4 = l.calculateBlock(1, 1).strassenMultiplyMatrices(r.calculateBlock(1, 0) - r.calculateBlock(0, 0),
                                                                maxDepth, currentDepth);
    Matrix p5 = (l.calculateBlock(0, 0) + l.calculateBlock(0, 1)).strassenMultiplyMatrices(r.calculateBlock(1, 1),
                                                                                           maxDepth, currentDepth);
    Matrix p6 = (l.calculateBlock(1, 0) - l.calculateBlock(0, 0)).strassenMultiplyMatrices(
            r.calculateBlock(0, 0) + r.calculateBlock(0, 1),
            maxDepth, currentDepth);
    Matrix p7 = (l.calculateBlock(0, 1) - l.calculateBlock(1, 1)).strassenMultiplyMatrices(
            r.calculateBlock(1, 0) + r.calculateBlock(1, 1),
            maxDepth, currentDepth);

    Matrix c11 = p1 + p4 - p5 + p7;
    Matrix c12 = p3 + p5;
    Matrix c21 = p2 + p4;
    Matrix c22 = p1 - p2 + p3 + p6;

    return merge2x2Matrices(c11, c12, c21, c22);
}

void Matrix::multiplyPart(int start, int end, const Matrix &left, const Matrix &right, Matrix &resultMatrix) {
    for (int c = start; c < end; c++) {
        auto row = c % resultMatrix.height;
        auto column = c / resultMatrix.width;
        auto result = 0;

        for (int i = 0; i < std::min(resultMatrix.height, resultMatrix.width); ++i) {
            auto first = left.matrix[row][i];
            auto second = right.matrix[i][column];
            result += first * second;
        }

        resultMatrix.matrix[row][column] = result;
    }
}

Matrix Matrix::parallelSimpleMultiplyMatrices(const Matrix &right, int threadsNumber) const {
    auto result = Matrix(height, right.width);
    std::vector<std::thread> threads;

    auto elemsNumber = height * width;
    auto operationsNumber = elemsNumber / threadsNumber;
    auto remainderOperationsNumber = elemsNumber % threadsNumber;

    for (auto i = 0; i < threadsNumber; i++) {
        int start;
        int end;

        if (i == 0) {
            start = operationsNumber * i;
            end = (operationsNumber * (i + 1)) + remainderOperationsNumber;
        } else {
            start = operationsNumber * i + remainderOperationsNumber;
            end = (operationsNumber * (i + 1)) + remainderOperationsNumber;
        }

        std::thread thread(multiplyPart, start, end, std::ref(*this), std::ref(right), std::ref(result));
        threads.emplace_back(std::move(thread));
    }

    for (auto &thread: threads) {
        thread.join();
    }

    return result;
}

Matrix Matrix::calculateBlock(int row, int column) const {
    Matrix block(height / 2, width / 2);
    for (int h = 0; h < block.height; ++h) {
        for (int w = 0; w < block.width; ++w) {
            block.matrix[h][w] = matrix[h + row * block.height][w + column * block.width];
        }
    }

    return block;
}

Matrix Matrix::merge2x2Matrices(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22) {
    std::vector<std::vector<int>> merged = c11.matrix;

    for (int i = 0; i < merged.size(); i++) {
        merged[i].insert(merged[i].end(), c12.matrix[i].begin(), c12.matrix[i].end());
    }

    merged.insert(merged.end(), c21.matrix.begin(), c21.matrix.end());

    int height_middle = c11.height;
    for (int i = height_middle; i < merged.size(); i++) {
        int c22_index = i - height_middle;
        merged[i].insert(merged[i].end(), c22.matrix[c22_index].begin(), c22.matrix[c22_index].end());
    }

    auto width = c11.width + c12.width;
    auto height = c11.height + c21.height;

    return Matrix(height, width, merged);
}

void Matrix::printMatrix(std::ostream &ostream) const {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            ostream << matrix[i][j] << " ";
        }

        ostream << "\n";
    }

    ostream << "\n";
}
