#include <fstream>
#include <memory>
#include "Matrix.h"

Matrix::Matrix(int height, int width, std::vector<std::vector<int>> matrix) : matrix(std::move(matrix)),
                                                                              width(width),
                                                                              height(height) {}

Matrix::Matrix() : Matrix(2000, 2000) {
    random();
}

void Matrix::random() {
    std::srand(std::time(nullptr));
    for (int i = 0; i < height * width; i++) {
        matrix[i / width][i % width] = (std::rand() % 100);
    }
}

Matrix::Matrix(int height, int width)
        : matrix(std::vector<std::vector<int>>(height, std::vector<int>(width))), width(width), height(height) {}


Matrix Matrix::operator+(const Matrix &right) const {
    auto newMatrix = Matrix(*this);

    for (int i = 0; i < newMatrix.height; ++i) {
        for (int j = 0; j < newMatrix.width; ++j) {
            newMatrix.matrix[i][j] += right.matrix[i][j];
        }
    }

    return newMatrix;
}

Matrix Matrix::operator-(const Matrix &right) const {
    auto newMatrix = Matrix(*this);

    for (int i = 0; i < newMatrix.height; ++i) {
        for (int j = 0; j < newMatrix.width; ++j) {
            newMatrix.matrix[i][j] -= right.matrix[i][j];
        }
    }

    return newMatrix;
}

Matrix Matrix::parallelSum(const Matrix &right, const int threadsCount) const {
    int size = right.width * right.height;
    if (threadsCount <= 0 || threadsCount >= size) {
        throw std::invalid_argument("Invalid threads count");
    }

    int elementsPerThread = (size / threadsCount) + (size % threadsCount > 0 ? 1 : 0);
    auto result = Matrix(height, width);
    std::vector<std::thread> threads;
    for (int i = 0; i < size; i += elementsPerThread) {
        std::thread thread(sumPart, std::ref(*this), std::ref(right), i, elementsPerThread, std::ref(result));
        threads.emplace_back(std::move(thread));
    }

    for (auto &thread: threads) {
        thread.join();
    }

    return result;
}

Matrix Matrix::strassenMultiply(const Matrix &right, int maxDepth, int currentDepth) const {
    if (width <= STRASSEN_MIN_MATRIX_SIZE) {
        return parallelMultiply(right, THREADS_COUNT);
    } else if (currentDepth < maxDepth) {
        return strassenMultiplyInnerParallel(*this, right, maxDepth, currentDepth);
    }

    return strassenMultiplyInner(*this, right, maxDepth, currentDepth);
}

void getP(const Matrix &left,
          const Matrix &right,
          int maxD,
          int depth,
          std::promise<Matrix> &promise,
          std::vector<std::thread> &threads) {

    std::thread thread(
            [](const Matrix l, const Matrix r, int maxDepth, int curDepth, std::promise<Matrix> &resultPromise) {
                return resultPromise.set_value(l.strassenMultiply(r, maxDepth, curDepth));
            },
            left, right, maxD, depth, std::ref(promise));

    threads.emplace_back(std::move(thread));
}

Matrix Matrix::strassenMultiplyInnerParallel(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth) {
    currentDepth++;

    std::vector<std::thread> threads;

    std::promise<Matrix> p1Promise;
    auto p1 = p1Promise.get_future();
    getP(l.getBlock(0, 0) + l.getBlock(1, 1), r.getBlock(0, 0) + r.getBlock(1, 1), maxDepth, currentDepth,
         p1Promise, threads);

    std::promise<Matrix> p2Promise;
    auto p2 = p2Promise.get_future();
    getP(l.getBlock(1, 0) + l.getBlock(1, 1), r.getBlock(0, 0), maxDepth, currentDepth, p2Promise, threads);

    std::promise<Matrix> p3Promise;
    auto p3 = p3Promise.get_future();
    getP(l.getBlock(0, 0), r.getBlock(0, 1) - r.getBlock(1, 1), maxDepth, currentDepth, p3Promise, threads);

    std::promise<Matrix> p4Promise;
    auto p4 = p4Promise.get_future();
    getP(l.getBlock(1, 1), r.getBlock(1, 0) - r.getBlock(0, 0), maxDepth, currentDepth, p4Promise, threads);

    std::promise<Matrix> p5Promise;
    auto p5 = p5Promise.get_future();
    getP(l.getBlock(0, 0) + l.getBlock(0, 1), r.getBlock(1, 1), maxDepth, currentDepth, p5Promise, threads);

    std::promise<Matrix> p6Promise;
    auto p6 = p6Promise.get_future();
    getP(l.getBlock(1, 0) - l.getBlock(0, 0), r.getBlock(0, 0) + r.getBlock(0, 1), maxDepth, currentDepth, p6Promise,
         threads);

    std::promise<Matrix> p7Promise;
    auto p7 = p7Promise.get_future();
    getP(l.getBlock(0, 1) - l.getBlock(1, 1), r.getBlock(1, 0) + r.getBlock(1, 1), maxDepth, currentDepth,
         p7Promise, threads);


    auto realP1 = p1.get();
    auto realP2 = p2.get();
    auto realP3 = p3.get();
    auto realP4 = p4.get();
    auto realP5 = p5.get();
    auto realP6 = p6.get();
    auto realP7 = p7.get();
    auto c21 = realP2 + realP4;
    auto c12 = realP3 + realP5;
    auto c22 = realP1 - realP2 + realP3 + realP6;
    auto c11 = realP1 + realP4 - realP5 + realP7;

    for (auto &thread: threads) {
        thread.join();
    }

    return merge2x2(c11, c12, c21, c22);
}

Matrix Matrix::strassenMultiplyInner(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth) {
    currentDepth++;

    Matrix p1 = (l.getBlock(0, 0) + l.getBlock(1, 1)).strassenMultiply(r.getBlock(0, 0) + r.getBlock(1, 1),
                                                                       maxDepth, currentDepth);
    Matrix p2 = (l.getBlock(1, 0) + l.getBlock(1, 1)).strassenMultiply(r.getBlock(0, 0), maxDepth, currentDepth);
    Matrix p3 = l.getBlock(0, 0).strassenMultiply(r.getBlock(0, 1) - r.getBlock(1, 1), maxDepth, currentDepth);
    Matrix p4 = l.getBlock(1, 1).strassenMultiply(r.getBlock(1, 0) - r.getBlock(0, 0), maxDepth, currentDepth);
    Matrix p5 = (l.getBlock(0, 0) + l.getBlock(0, 1)).strassenMultiply(r.getBlock(1, 1), maxDepth, currentDepth);
    Matrix p6 = (l.getBlock(1, 0) - l.getBlock(0, 0)).strassenMultiply(r.getBlock(0, 0) + r.getBlock(0, 1),
                                                                       maxDepth, currentDepth);
    Matrix p7 = (l.getBlock(0, 1) - l.getBlock(1, 1)).strassenMultiply(r.getBlock(1, 0) + r.getBlock(1, 1),
                                                                       maxDepth, currentDepth);

    auto c11 = p1 + p4 - p5 + p7;
    auto c12 = p3 + p5;
    auto c21 = p2 + p4;
    auto c22 = p1 - p2 + p3 + p6;

    return merge2x2(c11, c12, c21, c22);
}

void Matrix::sumPart(const Matrix &left, const Matrix &right, int start, int length, Matrix &result) {
    int size = result.width * result.height;
    for (int i = start; i < start + length && i < size; ++i) {
        int h = i / result.width;
        int w = i % result.width;
        result.matrix[h][w] = left.matrix[h][w] + right.matrix[h][w];
    }
}

std::istream &operator>>(std::istream &in, Matrix &matrix) {
    in >> matrix.height;
    in >> matrix.width;

    matrix.matrix.resize(matrix.height);

    for (int i = 0; i < matrix.height; ++i) {
        matrix.matrix[i].resize(matrix.width);
        for (int j = 0; j < matrix.width; ++j) {
            in >> matrix.matrix[i][j];
        }
    }

    return in;
}

std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
    out << matrix.height << ' ';
    out << matrix.width << '\n';

    for (int i = 0; i < matrix.height; ++i) {
        for (int j = 0; j < matrix.width; ++j) {
            out << matrix.matrix[i][j];
            if (j < matrix.width - 1) {
                out << ' ';
            }
        }
        out << '\n';
    }

    return out;
}

void Matrix::multiplyPart(int start, int end, const Matrix &left, const Matrix &right, Matrix &resultMatrix) {
    for (int op = start; op < end; ++op) {
        auto row = op % resultMatrix.height;
        auto column = op / resultMatrix.width;
        auto result = 0;

        for (int i = 0; i < std::min(resultMatrix.height, resultMatrix.width); ++i) {
            auto first = left.matrix[row][i];
            auto second = right.matrix[i][column];
            result += first * second;
        }

        resultMatrix.matrix[row][column] = result;
    }
}

Matrix Matrix::parallelMultiply(const Matrix &right, int threadsCount) const {
    if (width != right.height) {
        throw std::invalid_argument("Invalid sizes. Left width: " + std::to_string(width) + ". Right height: " +
                                    std::to_string(right.height));
    }

    auto result = Matrix(height, right.width);
    std::vector<std::thread> threads;

    auto elementsCount = height * width;
    auto operationsCount = elementsCount / threadsCount;
    auto restOperations = elementsCount % threadsCount;

    for (auto i = 0; i < threadsCount; i++) {
        int start;
        int end;

        if (i == 0) {
            start = operationsCount * i;
            end = (operationsCount * (i + 1)) + restOperations;
        } else {
            start = operationsCount * i + restOperations;
            end = (operationsCount * (i + 1)) + restOperations;
        }

        std::thread thread(multiplyPart, start, end, std::ref(*this), std::ref(right), std::ref(result));
        threads.emplace_back(std::move(thread));
    }

    for (auto &thread: threads) {
        thread.join();
    }

    return result;
}

Matrix Matrix::getBlock(int y, int x) const {
    Matrix block(height / 2, width / 2);
    for (int h = 0; h < block.height; ++h) {
        for (int w = 0; w < block.width; ++w) {
            block.matrix[h][w] = matrix[h + y * block.height][w + x * block.width];
        }
    }

    return block;
}

Matrix Matrix::merge2x2(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22) {
    std::vector<std::vector<int>> merged = c11.matrix;

    for (size_t i = 0; i < merged.size(); ++i) {
        merged[i].insert(merged[i].end(), c12.matrix[i].begin(), c12.matrix[i].end());
    }

    merged.insert(merged.end(), c21.matrix.begin(), c21.matrix.end());

    size_t height_middle = c11.height;
    for (size_t i = height_middle; i < merged.size(); ++i) {
        size_t c22_index = i - height_middle;
        merged[i].insert(merged[i].end(), c22.matrix[c22_index].begin(), c22.matrix[c22_index].end());
    }

    auto width = c11.width + c12.width;
    auto height = c11.height + c21.height;

    return {height, width, merged};
}
