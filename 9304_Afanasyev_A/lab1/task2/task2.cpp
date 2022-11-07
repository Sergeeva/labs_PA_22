#include "task2.h"

int main() {
    log("Task2 started");
    auto firstMatrix = Matrix();
    auto secondMatrix = Matrix();

    std::thread inputThread(Matrix::readTwoMatrices, std::ref(firstMatrix), std::ref(secondMatrix));

    std::promise<Matrix> resultPromise;
    auto resultFuture = resultPromise.get_future();

    inputThread.join();

    std::thread sumThread(sum, std::ref(firstMatrix), std::ref(secondMatrix), std::ref(resultPromise));
    auto result = resultFuture.get();

    std::thread printThread(Matrix::writeMatrix, std::ref(result));

    sumThread.join();
    printThread.join();
    return 0;
}

void sum(const Matrix &firstMatrix, const Matrix &secondMatrix, std::promise<Matrix> &resultPromise) {
    auto result = firstMatrix + secondMatrix;
    log("Got result of sum");

    resultPromise.set_value(result);
}
