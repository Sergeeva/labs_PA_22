//
// Created by Виктор Борисовский on 20.12.2022.
//

#ifndef PAFOURTHLAB_MATRIX_H
#define PAFOURTHLAB_MATRIX_H

#include <vector>


class Matrix {
    std::vector<std::vector<int>> matrix;
    int width = 0;
    int height = 0;

    static void multiplyPart(int start, int end, const Matrix &left, const Matrix &right, Matrix &resultMatrix);

    static Matrix strassenMultiplyRecursionParallel(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth);

    static Matrix strassenMultiplyRecursion(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth);

    static Matrix merge2x2Matrices(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22);

public:

    Matrix();
    Matrix calculateBlock(int row, int column) const;

    Matrix(int height, int width);

    void createRandomMatrix();

    Matrix(const Matrix &matrix) = default;

    ~Matrix() = default;

    Matrix(int height, int width, std::vector<std::vector<int>> matrix);

    Matrix parallelSimpleMultiplyMatrices(const Matrix &right, int threadsNumber) const;

    Matrix strassenMultiplyMatrices(const Matrix &right, int maxDepth, int currentDepth = 0) const;

    Matrix operator+(const Matrix &right) const;

    Matrix operator-(const Matrix &right) const;

    void printMatrix(std::ostream &ostream) const;

};


#endif //PAFOURTHLAB_MATRIX_H
