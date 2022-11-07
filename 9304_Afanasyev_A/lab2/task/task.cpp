#include "task.h"

int main() {
    int iterations = 10;
    std::cout << "Enter iterations count\n";
    std::cin >> iterations;

    std::mutex fileMutex;
    std::ofstream outfile;
    outfile.open("output.txt");

    log("File opened");

    BlockingQueue<Matrix> sourceQueue(10);
    BlockingQueue<Matrix> resultQueue(10);

    std::thread consumer{popAndSumAndPushResult, std::ref(sourceQueue), std::ref(resultQueue), iterations};
    std::thread producer{pushMatrix, std::ref(sourceQueue), iterations};
    std::thread writer{popAndWrite, std::ref(resultQueue), std::ref(outfile), std::ref(fileMutex), iterations};

    log("All thread started");

    producer.join();
    consumer.join();
    writer.join();

    return 0;
}

void popAndSumAndPushResult(BlockingQueue<Matrix> &queue, BlockingQueue<Matrix> &resultQueue, int iterations) {
    for (int i = 0; i < iterations; i++) {
        Matrix matrix1 = queue.pop();
        Matrix matrix2 = queue.pop();
        resultQueue.push(matrix1 + matrix2);
        resultQueue.push(matrix1.parallelSum(matrix2, iterations));
        log("Got result");
    }
}

void pushMatrix(BlockingQueue<Matrix> &queue, int iterations) {
    for (int i = 0; i < iterations * 2; i++) {
        Matrix matrix{};
        queue.push(matrix);
        log("Pushed new matrix");
    }
}

void popAndWrite(BlockingQueue<Matrix> &queue, std::ofstream &file, std::mutex &fileMutex, int iterations) {
    for (int i = 0; i < iterations * 2; i++) {
        Matrix matrix = queue.pop();
        std::unique_lock<std::mutex> lock(fileMutex);
        file << matrix;
        log("Matrix written into file");
    }
}
