#include "task.h"

int main() {
    int iterationsCount = 1000;
    std::cout << "Enter iterationsCount count\n";
    std::cin >> iterationsCount;

    int threadsCount = 10;
    std::cout << "Enter producers / consumers count\n";
    std::cin >> threadsCount;

    auto start = std::chrono::steady_clock::now();

    BlockingStack<Matrix> sourceStack;
    BlockingStack<Matrix> resultStack;

    std::vector<std::thread> consumers;
    for (int i = 0; i < threadsCount; ++i) {
        std::thread consumer{popAndSumAndPushResult, std::ref(sourceStack), std::ref(resultStack), iterationsCount};
        consumers.emplace_back(std::move(consumer));
    }

    std::vector<std::thread> producers;
    for (int i = 0; i < threadsCount; ++i) {
        std::thread producer{pushMatrix, std::ref(sourceStack), iterationsCount};
        producers.emplace_back(std::move(producer));
    }

    std::thread writer{popAndWrite, std::ref(resultStack), iterationsCount * threadsCount};

    log("All threadsCount started");

    for (auto &thread: consumers) {
        thread.join();
    }

    for (auto &thread: producers) {
        thread.join();
    }

    writer.join();

    log("All threadsCount finished");

    auto end = std::chrono::steady_clock::now();
    log("Duration: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()));

    return 0;
}

void popAndSumAndPushResult(BlockingStack<Matrix> &stack, BlockingStack<Matrix> &resultStack, int iterations) {
    for (int i = 0; i < iterations; i++) {
        Matrix matrix1 = stack.pop();
        Matrix matrix2 = stack.pop();
        resultStack.push(matrix1 + matrix2);
    }

    log("Finish summing");
}

void pushMatrix(BlockingStack<Matrix> &stack, int iterations) {
    for (int i = 0; i < iterations; i++) {
        stack.push(Matrix::randomMatrix(100, 100, 1000));
        stack.push(Matrix::randomMatrix(100, 100, 1000));
    }

    log("Finish generating");
}

void popAndWrite(BlockingStack<Matrix> &resultStack, int iterations) {
    std::ofstream outfile;
    outfile.open("output.txt");
    log("File opened");

    for (int i = 0; i < iterations; i++) {
        Matrix matrix = resultStack.pop();
        outfile << matrix;
    }

    log("Finish writing");
}
