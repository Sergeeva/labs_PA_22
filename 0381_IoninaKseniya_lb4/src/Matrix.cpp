#include <fstream>
#include <memory>
#include "Matrix.h"
using namespace std;

Matrix::Matrix(int height, int width, vector<vector<int>> matrix) : matrix(move(matrix)), width(width), height(height) {}

void Matrix::random() {

    srand(time(nullptr));

    for (int i = 0; i < height * width; i++) 
    {
        matrix[i / width][i % width] = (rand() % 100);
    }
}

Matrix::Matrix(int height, int width): matrix(vector<vector<int>>(height, vector<int>(width))), width(width), height(height) {}


Matrix Matrix::operator+(const Matrix &right) const {

    auto newMatrix = Matrix(*this);

    for (int i = 0; i < newMatrix.height; ++i) 
    {
        for (int j = 0; j < newMatrix.width; ++j) 
        {
            newMatrix.matrix[i][j] += right.matrix[i][j];
        }
    }

    return newMatrix;
}

Matrix Matrix::operator-(const Matrix &right) const {

    auto newMatrix = Matrix(*this);

    for (int i = 0; i < newMatrix.height; ++i) 
    {
        for (int j = 0; j < newMatrix.width; ++j) 
        {
            newMatrix.matrix[i][j] -= right.matrix[i][j];
        }
    }

    return newMatrix;
}

void Matrix::sumPart(const Matrix &left, const Matrix &right, int start, int length, Matrix &result) {
    int size = result.width * result.height;

    for (int i = start; i < start + length && i < size; ++i) 
    {
        int h = i / result.width;
        int w = i % result.width;
        result.matrix[h][w] = left.matrix[h][w] + right.matrix[h][w];
    }
}

Matrix Matrix::parallelSum(const Matrix &right, const int threadsCount) const {

    int size = right.width * right.height;

    if (threadsCount <= 0 || threadsCount >= size) 
    {
        throw invalid_argument("Недопустимое количество потоков");
    }

    int elementsPerThread = (size / threadsCount) + (size % threadsCount > 0 ? 1 : 0);

    auto result = Matrix(height, width);

    vector<thread> threads;

    for (int i = 0; i < size; i += elementsPerThread) 
    {
        thread thread(sumPart, ref(*this), ref(right), i, elementsPerThread, ref(result));
        threads.emplace_back(move(thread));
    }

    for (auto &thread: threads) 
    {
        thread.join();
    }

    return result;
}

Matrix Matrix::getBlock(int y, int x) const 
{
    Matrix block(height / 2, width / 2);

    for (int h = 0; h < block.height; ++h) 
    {
        for (int w = 0; w < block.width; ++w) 
        {
            block.matrix[h][w] = matrix[h + y * block.height][w + x * block.width];
        }
    }

    return block;
}


Matrix Matrix::strassenMultiply(const Matrix &right, int max_depth, int curr_depth) const 
{
   
    if (width <= 128) 
    {

        return parallelMultiply(right, THREADS_COUNT);

    } 

    else if (curr_depth < max_depth) 
    {

        return strassenMultiply_rec_parallel(*this, right, max_depth, curr_depth);

    }

    return strassenMultiply_rec(*this, right, max_depth, curr_depth);
}

void get_pr(const Matrix &left, const Matrix &right, int maxD, int depth, promise<Matrix> &promise, vector<thread> &threads) {
        thread thread(

            [](const Matrix l, const Matrix r, int max_depth, int curDepth, std::promise<Matrix> &resultPromise) {
                return resultPromise.set_value(l.strassenMultiply(r, max_depth, curDepth));
            },

            left, right, maxD, depth, ref(promise));

    threads.emplace_back(move(thread));
}

Matrix Matrix::strassenMultiply_rec_parallel(const Matrix &l, const Matrix &r, int max_depth, int curr_depth) {

    curr_depth++;

    vector<thread> threads;

    promise<Matrix> promise_p1;
    auto p1 = promise_p1.get_future();
    get_pr(l.getBlock(0, 0) + l.getBlock(1, 1), r.getBlock(0, 0) + r.getBlock(1, 1), max_depth, curr_depth, promise_p1, threads);

    promise<Matrix> promise_p2;
    auto p2 = promise_p2.get_future();
    get_pr(l.getBlock(1, 0) + l.getBlock(1, 1), r.getBlock(0, 0), max_depth, curr_depth, promise_p2, threads);

    promise<Matrix> promise_p3;
    auto p3 = promise_p3.get_future();
    get_pr(l.getBlock(0, 0), r.getBlock(0, 1) - r.getBlock(1, 1), max_depth, curr_depth, promise_p3, threads);

    promise<Matrix> promise_p4;
    auto p4 = promise_p4.get_future();
    get_pr(l.getBlock(1, 1), r.getBlock(1, 0) - r.getBlock(0, 0), max_depth, curr_depth, promise_p4, threads);

    promise<Matrix> promise_p5;
    auto p5 = promise_p5.get_future();
    get_pr(l.getBlock(0, 0) + l.getBlock(0, 1), r.getBlock(1, 1), max_depth, curr_depth, promise_p5, threads);

    promise<Matrix> promise_p6;
    auto p6 = promise_p6.get_future();
    get_pr(l.getBlock(1, 0) - l.getBlock(0, 0), r.getBlock(0, 0) + r.getBlock(0, 1), max_depth, curr_depth, promise_p6, threads);

    promise<Matrix> promise_p7;
    auto p7 = promise_p7.get_future();
    get_pr(l.getBlock(0, 1) - l.getBlock(1, 1), r.getBlock(1, 0) + r.getBlock(1, 1), max_depth, curr_depth, promise_p7, threads);

    auto p1_m = p1.get();
    auto p2_m = p2.get();
    auto p3_m = p3.get();
    auto p4_m = p4.get();
    auto p5_m = p5.get();
    auto p6_m = p6.get();
    auto p7_m = p7.get();

    auto a21 = p2_m + p4_m;
    auto a12 = p3_m + p5_m;
    auto a22 = p1_m - p2_m + p3_m + p6_m;
    auto a11 = p1_m + p4_m - p5_m + p7_m;

    for (auto &thread: threads) 
    {
        thread.join();
    }

    return together(a11, a12, a21, a22);
}

Matrix Matrix::strassenMultiply_rec(const Matrix &l, const Matrix &r, int max_depth, int curr_depth) {

    curr_depth++;

    Matrix p1 = (l.getBlock(0, 0) + l.getBlock(1, 1)).strassenMultiply(r.getBlock(0, 0) + r.getBlock(1, 1), max_depth, curr_depth);
    Matrix p2 = (l.getBlock(1, 0) + l.getBlock(1, 1)).strassenMultiply(r.getBlock(0, 0), max_depth, curr_depth);
    Matrix p3 = l.getBlock(0, 0).strassenMultiply(r.getBlock(0, 1) - r.getBlock(1, 1), max_depth, curr_depth);
    Matrix p4 = l.getBlock(1, 1).strassenMultiply(r.getBlock(1, 0) - r.getBlock(0, 0), max_depth, curr_depth);
    Matrix p5 = (l.getBlock(0, 0) + l.getBlock(0, 1)).strassenMultiply(r.getBlock(1, 1), max_depth, curr_depth);
    Matrix p6 = (l.getBlock(1, 0) - l.getBlock(0, 0)).strassenMultiply(r.getBlock(0, 0) + r.getBlock(0, 1), max_depth, curr_depth);
    Matrix p7 = (l.getBlock(0, 1) - l.getBlock(1, 1)).strassenMultiply(r.getBlock(1, 0) + r.getBlock(1, 1), max_depth, curr_depth);

    auto a11 = p1 + p4 - p5 + p7;
    auto a12 = p3 + p5;
    auto a21 = p2 + p4;
    auto a22 = p1 - p2 + p3 + p6;

    return together(a11, a12, a21, a22);
}



istream &operator>>(istream &in, Matrix &matrix) {
    in >> matrix.height;
    in >> matrix.width;

    matrix.matrix.resize(matrix.height);

    for (int i = 0; i < matrix.height; ++i) 
    {
        matrix.matrix[i].resize(matrix.width);
        for (int j = 0; j < matrix.width; ++j) 
        {
            in >> matrix.matrix[i][j];
        }
    }

    return in;
}

ostream &operator<<(ostream &out, const Matrix &matrix) {
    out << matrix.height << ' ';
    out << matrix.width << '\n';

    for (int i = 0; i < matrix.height; ++i) 
    {
        for (int j = 0; j < matrix.width; ++j) 
        {
            out << matrix.matrix[i][j];
            if (j < matrix.width - 1) {
                out << ' ';
            }
        }
        out << '\n';
    }

    return out;
}

void Matrix::multPart(int start, int end, const Matrix &left, const Matrix &right, Matrix &resultMatrix) {

    for (int i = start; i < end; ++i) {

        auto row = i % resultMatrix.height;
        auto column = i / resultMatrix.width;
        auto result = 0;

        for (int i = 0; i < min(resultMatrix.height, resultMatrix.width); ++i) 
        {
            auto first = left.matrix[row][i];
            auto second = right.matrix[i][column];

            result += first * second;
        }

        resultMatrix.matrix[row][column] = result;
    }
}

Matrix Matrix::parallelMultiply(const Matrix &right, int threadsCount) const 
{
    if (width != right.height) {
        throw invalid_argument("Неверные размеры. Ширина слева: " + to_string(width) + ". Ширина справа: " + to_string(right.height));
    }

    auto result = Matrix(height, right.width);
    vector<thread> threads;

    auto elementsCount = height * width;

    auto operationsCount = elementsCount / threadsCount;

    auto restOperations = elementsCount % threadsCount;

    for (auto i = 0; i < threadsCount; i++) 
    {
        int start;
        int end;

        if (i == 0) 
        {
            start = operationsCount * i;
            end = (operationsCount * (i + 1)) + restOperations;
        } 
        
        else 
        {
            start = operationsCount * i + restOperations;
            end = (operationsCount * (i + 1)) + restOperations;
        }

        thread thread(multPart, start, end, ref(*this), ref(right), ref(result));
        threads.emplace_back(move(thread));
    }

    for (auto &thread: threads) {
        thread.join();
    }

    return result;
}



Matrix Matrix::together(const Matrix &a11, const Matrix &a12, const Matrix &a21, const Matrix &a22) {

    vector<vector<int>> merged = a11.matrix;//

    for (size_t i = 0; i < merged.size(); ++i) //вставляем столбцы
    {
        merged[i].insert(merged[i].end(), a12.matrix[i].begin(), a12.matrix[i].end());
    }

//   for (size_t i = 0; i < merged.size(); ++i) 
//     {
//         merged.push_back(merged[i].end(), a21.matrix[i].begin(), a21.matrix[i].end());
//     }

     merged.insert(merged.end(), a21.matrix.begin(), a21.matrix.end()); //вставляем строки

    size_t height_middle = a11.height;//**

//от height_middle до n/2
    for (size_t i = height_middle; i < merged.size(); ++i) 
    //начинаем с конца а11 
    {
        size_t a22_index = i - height_middle;

        merged[i].insert(merged[i].end(), a22.matrix[a22_index].begin(), a22.matrix[a22_index].end());
    }

    auto width = a11.width + a12.width;
    auto height = a11.height + a21.height;

    return {height, width, merged};
}