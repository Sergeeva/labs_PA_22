#include "task.h"

int main() {
    int iterations = 1000;
    std::cout << "Enter iterations count\n";
    std::cin >> iterations;

    BlockingStack<Matrix> sourceStack;
    BlockingStack<Matrix> resultStack;

    std::thread consumer1{popAndSumAndPushResult, std::ref(sourceStack), std::ref(resultStack), iterations};
    std::thread consumer3{popAndSumAndPushResult, std::ref(sourceStack), std::ref(resultStack), iterations};
    std::thread consumer2{popAndSumAndPushResult, std::ref(sourceStack), std::ref(resultStack), iterations};

    std::thread producer1{pushMatrix, std::ref(sourceStack), iterations};
    std::thread producer2{pushMatrix, std::ref(sourceStack), iterations};
    std::thread producer3{pushMatrix, std::ref(sourceStack), iterations};

    std::thread writer{popAndWrite, std::ref(resultStack), iterations * 3};

    log("All threads started");

    consumer1.join();
    consumer2.join();
    consumer3.join();

    producer1.join();
    producer2.join();
    producer3.join();

    writer.join();

    log("All threads finished");

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
        stack.push(Matrix::randomMatrix(20, 100, 1000));
        stack.push(Matrix::randomMatrix(20, 100, 1000));
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
