#include "threads.h"

Threads::Threads(int size, char *fileName1, char *fileName2) : matrixSize(size) {
    matrix1 = Matrix<int>(matrixSize);
    matrix2 = Matrix<int>(matrixSize);
    multiplicationResultFileName = fileName1;
    strassenResultFileName = fileName2;
}

void Threads::run() {
    matrix1.fill();
    matrix2.fill();

    auto primaryMultiplicationStart = std::chrono::steady_clock::now();
    Matrix<int> resultMatrix = matrix1 * matrix2;
    auto primaryMultiplicationEnd = std::chrono::steady_clock::now();
    write(multiplicationResultFileName, resultMatrix);

    auto strassenMultiplicationStart = std::chrono::steady_clock::now();
    resultMatrix = strassenAlgorithm();
    auto strassenMultiplicationEnd = std::chrono::steady_clock::now();
    write(strassenResultFileName, resultMatrix);

    std::chrono::duration<double> primaryDiff = primaryMultiplicationEnd - primaryMultiplicationStart;
    std::chrono::duration<double> strassenDiff = strassenMultiplicationEnd - strassenMultiplicationStart;
    std::cout << "Time of the parallel matrices multiplication algorithm: " << primaryDiff.count() << '\n'
        << "Time of Strassen\'s parallel matrices multiplication algorithm: " << strassenDiff.count() << '\n';
}

Matrix<int> Threads::strassenAlgorithm() {
    if (matrix1.size() != matrix2.size()) {
        throw std::runtime_error("Matrix error: different sizes of matrices");
    }

    int newMatrixSize = pow(2, ceil(std::log(matrixSize) / std::log(2)));
    Matrix<int> newMatrix1(newMatrixSize);
    Matrix<int> newMatrix2(newMatrixSize);
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            newMatrix1[i][j] = matrix1[i][j];
            newMatrix2[i][j] = matrix2[i][j];
        }
    }

    Matrix<int> multiplicationMatrix = recursionStrassen(newMatrix1, newMatrix2, 0);
    if (multiplicationMatrix.size() == matrixSize) {
        return multiplicationMatrix;
    } else {
        Matrix<int> result(matrixSize);
        for (int i = 0; i < matrixSize; i++) {
            for (int j = 0; j < matrixSize; j++) {
                result[i][j] = multiplicationMatrix[i][j];
            }
        }
        return result;
    }
}

Matrix<int> Threads::recursionStrassen(Matrix<int> matrix1, Matrix<int> matrix2, int depth) {
    if (matrix1.size() <= 64) {
        return matrix1 * matrix2;
    }

    auto [a11, a12, a21, a22] = matrix1.split();
    auto [b11, b12, b21, b22] = matrix2.split();

    std::vector<Matrix<int>> matrixP(7);
    if (depth <= 1) {
        std::vector<std::thread> threads(7);
        threads[0] = std::thread([&] (Matrix<int> mat1, Matrix<int> mat2) -> void {
            matrixP[0] = recursionStrassen(mat1, mat2, depth + 1);
        }, a11 + a22, b11 + b22);
        threads[1] = std::thread([&] (Matrix<int> mat1, Matrix<int> mat2) -> void {
            matrixP[1] = recursionStrassen(mat1, mat2, depth + 1);
        }, a21 + a22, b11);
        threads[2] = std::thread([&] (Matrix<int> mat1, Matrix<int> mat2) -> void {
            matrixP[2] = recursionStrassen(mat1, mat2, depth + 1);
        }, a11, b12 - b22);
        threads[3] = std::thread([&] (Matrix<int> mat1, Matrix<int> mat2) -> void {
            matrixP[3] = recursionStrassen(mat1, mat2, depth + 1);
        }, a22, b21 - b11);
        threads[4] = std::thread([&] (Matrix<int> mat1, Matrix<int> mat2) -> void {
            matrixP[4] = recursionStrassen(mat1, mat2, depth + 1);
        }, a11 + a12, b22);
        threads[5] = std::thread([&] (Matrix<int> mat1, Matrix<int> mat2) -> void {
            matrixP[5] = recursionStrassen(mat1, mat2, depth + 1);
        }, a21 - a11, b11 + b12);
        threads[6] = std::thread([&] (Matrix<int> mat1, Matrix<int> mat2) -> void {
            matrixP[6] = recursionStrassen(mat1, mat2, depth + 1);
        }, a12 - a22, b21 + b22);
        for (int i = 0; i < 7; i++) {
            threads[i].join();
        }
    } else {
        matrixP[0] = recursionStrassen(a11 + a22, b11 + b22, depth + 1);
        matrixP[1] = recursionStrassen(a21 + a22, b11, depth + 1);
        matrixP[2] = recursionStrassen(a11, b12 - b22, depth + 1);
        matrixP[3] = recursionStrassen(a22, b21 - b11, depth + 1);
        matrixP[4] = recursionStrassen(a11 + a12, b22, depth + 1);
        matrixP[5] = recursionStrassen(a21 - a11, b11 + b12, depth + 1);
        matrixP[6] = recursionStrassen(a12 - a22, b21 + b22, depth + 1);
    }

    Matrix<int> c11 = matrixP[0] + matrixP[3] + matrixP[6] - matrixP[4];
    Matrix<int> c12 = matrixP[2] + matrixP[4];
    Matrix<int> c21 = matrixP[1] + matrixP[3];
    Matrix<int> c22 = matrixP[0] - matrixP[1] + matrixP[2] + matrixP[5];;

    return Matrix<int>::join(c11, c12, c21, c22);
}

void Threads::write(char *fileName, Matrix<int> &resultMatrix) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Writing error: invalid file");
    }

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            file << std::setw(3) << matrix1[i][j] << ' ';
        }
        file << (i == 0 ? "\t*\t" : "\t\t");
        for (int j = 0; j < matrixSize; j++) {
            file << std::setw(3) << matrix2[i][j] << ' ';
        }
        file << (i == 0 ? "\t=\t" : "\t\t");
        for (int j = 0; j < matrixSize; j++) {
            file << std::setw(7) << resultMatrix[i][j] << ' ';
        }
        file << '\n';
    }

    file.close();
}
