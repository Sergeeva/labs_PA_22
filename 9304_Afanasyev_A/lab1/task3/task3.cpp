#include "task3.h"

int main() {
    log("Task3 started");
    int minThreads = 1;
    int maxThreads = 100;
    for (int i = minThreads; i <= maxThreads; i += (i < 10 ? 1 : 10)) {
        long long ms = start(i);
        log("Threads count: " + std::to_string(i) + ". Total ms: " + std::to_string(ms) + "\n");
    }
    return 0;
}

long long start(int threadsCount) {
    auto firstMatrix = Matrix();
    auto secondMatrix = Matrix();
    std::thread readThread(Matrix::readTwoMatrices, std::ref(firstMatrix), std::ref(secondMatrix));

    std::promise<Matrix> resultPromise;
    auto resultFuture = resultPromise.get_future();

    readThread.join();

    auto start = std::chrono::steady_clock::now();
    std::thread sumThread(sum, std::ref(firstMatrix), std::ref(secondMatrix), std::ref(resultPromise),
                          threadsCount);

    auto result = resultFuture.get();
    auto end = std::chrono::steady_clock::now();

    std::thread printThread(Matrix::writeMatrix, std::ref(result));
    sumThread.join();
    printThread.join();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void sum(const Matrix &firstMatrix, const Matrix &secondMatrix, std::promise<Matrix> &resultPromise, int threadsCount) {
    auto result = firstMatrix.parallelSum(secondMatrix, threadsCount);
    log("Got result of sum.");

    resultPromise.set_value(result);
}
