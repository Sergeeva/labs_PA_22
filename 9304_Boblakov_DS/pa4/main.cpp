
#include "utils.h"

typedef std::vector<std::vector<int>> Matrix;

const int m_width = 2048;
const int m_height = 2048;

const int STRASSEN_MIN_MATRIX_SIZE = 64;
const int THREADS_COUNT = 1;

Matrix splitMatix(Matrix m1, int y, int x);

Matrix unitedMatrix(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22);

Matrix strassenMultiply(const Matrix &left, const Matrix &right, int maxDepth, int currentDepth = 0);

Matrix sum(Matrix m1, Matrix m2) {

    Matrix res_m(m1.size(), std::vector<int>(m1[0].size()));

    for (int i = 0; i < m1.size(); i++) {
        for (int j = 0; j < m1[i].size(); j++) {
            res_m[i][j] = m1[i][j] + m2[i][j];
        }
    }

    return res_m;
}

Matrix sub(Matrix m1, Matrix m2) {

    Matrix res_m(m1.size(), std::vector<int>(m1[0].size()));

    for (int i = 0; i < m1.size(); i++) {
        for (int j = 0; j < m1[i].size(); j++) {
            res_m[i][j] = m1[i][j] - m2[i][j];
        }
    }

    return res_m;
}

Matrix strassenOne(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth) {
    currentDepth++;

    Matrix p1 = strassenMultiply((sum(splitMatix(l, 0, 0), splitMatix(l, 1, 1))), sum(splitMatix(r, 0, 0), splitMatix(r, 1, 1)),
                                 maxDepth, currentDepth);
    Matrix p2 = strassenMultiply(sum(splitMatix(l, 1, 0), splitMatix(l, 1, 1)), splitMatix(r, 0, 0), maxDepth, currentDepth);
    Matrix p3 = strassenMultiply(splitMatix(l, 0, 0), sub(splitMatix(r, 0, 1), splitMatix(r, 1, 1)), maxDepth, currentDepth);
    Matrix p4 = strassenMultiply(splitMatix(l, 1, 1), sub(splitMatix(r, 1, 0), splitMatix(r, 0, 0)), maxDepth, currentDepth);
    Matrix p5 = strassenMultiply((sum(splitMatix(l, 0, 0), splitMatix(l, 0, 1))), splitMatix(r, 1, 1), maxDepth,
                                 currentDepth);
    Matrix p6 = strassenMultiply((sub(splitMatix(l, 1, 0), splitMatix(l, 0, 0))), sum(splitMatix(r, 0, 0), splitMatix(r, 0, 1)),
                                 maxDepth, currentDepth);
    Matrix p7 = strassenMultiply((sub(splitMatix(l, 0, 1), splitMatix(l, 1, 1))), sum(splitMatix(r, 1, 0), splitMatix(r, 1, 1)),
                                 maxDepth, currentDepth);

    auto c11 = sum(sub(sum(p1, p4), p5), p7);
    auto c12 = sum(p3, p5);
    auto c21 = sum(p2, p4);
    auto c22 = sum(sum(sub(p1, p2), p3), p6);

    return unitedMatrix(c11, c12, c21, c22);
}

void multiplyPart(int start, int end, const Matrix &left, const Matrix &right, Matrix &resultMatrix) {
    for (int op = start; op < end; ++op) {
        auto row = op % resultMatrix.size();
        auto column = op / resultMatrix[0].size();
        auto result = 0;

        for (int i = 0; i < std::min(resultMatrix.size(), resultMatrix[0].size()); ++i) {
            auto first = left[row][i];
            auto second = right[i][column];
            result += first * second;
        }

        resultMatrix[row][column] = result;
    }
}

Matrix parallelMultiply(const Matrix &left, const Matrix &right, int threads_count) {

    auto result = generateZeroM(left.size(), right[0].size());
    std::vector<std::thread> threads;

    auto elementsCount = left.size() * left[0].size();
    auto operationsCount = elementsCount / threads_count;
    auto restOperations = elementsCount % threads_count;

    for (auto i = 0; i < threads_count; i++) {
        int start;
        int end;

        if (i == 0) {
            start = operationsCount * i;
            end = (operationsCount * (i + 1)) + restOperations;
        } else {
            start = operationsCount * i + restOperations;
            end = (operationsCount * (i + 1)) + restOperations;
        }

        std::thread thread(multiplyPart, start, end, std::ref(left), std::ref(right), std::ref(result));
        threads.emplace_back(std::move(thread));
    }

    for (auto &thread: threads) {
        thread.join();
    }

//    printMatrix(result);
    return result;
}

void callPromise(const Matrix &left, const Matrix &right, int maxD, int depth, std::promise<Matrix> &promise, std::vector<std::thread> &threads) {

    std::thread thread([](const Matrix l, const Matrix r, int maxDepth, int curDepth, std::promise<Matrix> &resultPromise) {
                return resultPromise.set_value(strassenMultiply(l, r, maxDepth, curDepth));
            },
            left, right, maxD, depth, std::ref(promise));

    threads.emplace_back(std::move(thread));
}

Matrix splitMatix(Matrix m1, int y, int x) {
    Matrix block = generateZeroM(m1.size() / 2, m1[0].size() / 2);
    for (int h = 0; h < block.size(); ++h) {
        for (int w = 0; w < block[h].size(); ++w) {
            block[h][w] = m1[h + y * block[0].size()][w + x * block.size()];
        }
    }

    return block;
}

Matrix unitedMatrix(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22) {
    std::vector<std::vector<int>> merged;
    for (int i = 0; i < c11.size(); i++)
        merged.push_back(c11[i]);

    for (size_t i = 0; i < merged.size(); ++i) {
        merged[i].insert(merged[i].end(), c12[i].begin(), c12[i].end());
    }

    merged.insert(merged.end(), c21.begin(), c21.end());

    size_t height_middle = c11.size();
    for (size_t i = height_middle; i < merged.size(); ++i) {
        size_t c22_index = i - height_middle;
        merged[i].insert(merged[i].end(), c22[c22_index].begin(), c22[c22_index].end());
    }

    auto width = c11[0].size() + c12[0].size();
    auto height = c11.size() + c21.size();

    return merged;
}

Matrix strassenParallel(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth) {
    currentDepth++;

    std::vector<std::thread> threads;

    std::promise<Matrix> p1Promise;
    auto p1 = p1Promise.get_future();
    callPromise(sum(splitMatix(l, 0, 0), splitMatix(l, 1, 1)), sum(splitMatix(r, 0, 0), splitMatix(r, 1, 1)), maxDepth,
                currentDepth,
                p1Promise, threads);

    std::promise<Matrix> p2Promise;
    auto p2 = p2Promise.get_future();
    callPromise(sum(splitMatix(l, 1, 0), splitMatix(l, 1, 1)), splitMatix(r, 0, 0), maxDepth, currentDepth, p2Promise,
                threads);

    std::promise<Matrix> p3Promise;
    auto p3 = p3Promise.get_future();
    callPromise(splitMatix(l, 0, 0), sub(splitMatix(r, 0, 1), splitMatix(r, 1, 1)), maxDepth, currentDepth, p3Promise,
                threads);

    std::promise<Matrix> p4Promise;
    auto p4 = p4Promise.get_future();
    callPromise(splitMatix(l, 1, 1), sub(splitMatix(r, 1, 0), splitMatix(r, 0, 0)), maxDepth, currentDepth, p4Promise,
                threads);

    std::promise<Matrix> p5Promise;
    auto p5 = p5Promise.get_future();
    callPromise(sum(splitMatix(l, 0, 0), splitMatix(l, 0, 1)), splitMatix(r, 1, 1), maxDepth, currentDepth, p5Promise,
                threads);

    std::promise<Matrix> p6Promise;
    auto p6 = p6Promise.get_future();
    callPromise(sub(splitMatix(l, 1, 0), splitMatix(l, 0, 0)), sum(splitMatix(r, 0, 0), splitMatix(r, 0, 1)), maxDepth,
                currentDepth,
                p6Promise,
                threads);

    std::promise<Matrix> p7Promise;
    auto p7 = p7Promise.get_future();
    callPromise(sub(splitMatix(l, 0, 1), splitMatix(l, 1, 1)), sum(splitMatix(r, 1, 0), splitMatix(r, 1, 1)), maxDepth,
                currentDepth,
                p7Promise, threads);


    auto realP1 = p1.get();
    auto realP2 = p2.get();
    auto realP3 = p3.get();
    auto realP4 = p4.get();
    auto realP5 = p5.get();
    auto realP6 = p6.get();
    auto realP7 = p7.get();
    auto c21 = sum(realP2, realP4);
    auto c12 = sum(realP3, realP5);
    auto c22 = sum(sum(sub(realP1, realP2), realP3), realP6);
    auto c11 = sum(sub(sum(realP1, realP4), realP5), realP7);

    for (auto &thread: threads) {
        thread.join();
    }

    return unitedMatrix(c11, c12, c21, c22);
}

Matrix strassenMultiply(const Matrix &left, const Matrix &right, int maxDepth, int currentDepth) {
    if (left.size() <= STRASSEN_MIN_MATRIX_SIZE) {
        return parallelMultiply(left, right, THREADS_COUNT);
    } else if (currentDepth < maxDepth) {
        return strassenParallel(left, right, maxDepth, currentDepth);
    }

    return strassenOne(left, right, maxDepth, currentDepth);
}


int main() {

    int threads_count = THREADS_COUNT;
    std::cout << "Enter threads count\n";
    std::cin >> threads_count;

    int strassenDepth = 1;
    std::cout << "Enter strassen depth\n";
    std::cin >> strassenDepth;

    std::vector<std::thread> threads;

    Matrix m1 = generateMatrix(m_width, m_height);
    Matrix m2 = generateMatrix(m_width, m_height);
//    printMatrix(m1);
//    printMatrix(m2);

    auto start1 = std::chrono::steady_clock::now();
    parallelMultiply(m1, m2, threads_count);
    auto end1 = std::chrono::steady_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    std::cout << "Trivial mult time: " << (double) duration1.count() / 1000000 << "\n";


    auto start2 = std::chrono::steady_clock::now();
    strassenMultiply(m1, m2, threads_count);
    auto end2 = std::chrono::steady_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    std::cout << "Strassen mult time: " << (double) duration2.count() / 1000000 << "\n";

    return 0;
}

