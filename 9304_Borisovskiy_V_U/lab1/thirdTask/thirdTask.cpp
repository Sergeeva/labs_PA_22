#include "../utils.h"

void sumVectorsOfMatrices(Matrix &firstMatrix, Matrix &secondMatrix, Matrix &resultMatrix, int startIndex, int elementsCount) {
    for (int i = startIndex; i < startIndex + elementsCount; i++) {
        int height = i / firstMatrix[0].size();
        int width = i % firstMatrix[0].size();

        resultMatrix[height][width] = firstMatrix[height][width] + secondMatrix[height][width];
    }
}

Matrix sumMatrices(Matrix firstMatrix, Matrix secondMatrix, int threadsCount) {
    int elemsCount = firstMatrix.size() * firstMatrix[0].size();

    if (threadsCount <= 0 || threadsCount > elemsCount) {
        std::cout << "Incorrect thread count!";
        exit(1);
    }

    std::vector<std::thread> threads;
    threads.reserve(threadsCount);

    int step = elemsCount / threadsCount;

    Matrix resultMatrix(firstMatrix.size(), std::vector<int>(firstMatrix[0].size()));
    for (int i = 0; i < threadsCount; i++) {
        int startIndex = i * step;

        if (i == threadsCount - 1) {
            step = elemsCount - (step * i);
        }

        std::thread sumThread(sumVectorsOfMatrices, std::ref(firstMatrix), std::ref(secondMatrix), std::ref(resultMatrix), startIndex, step);
        threads.push_back(std::move(sumThread));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return resultMatrix;
}

int main() {
    Matrix firstMatrix;
    std::thread generateFirstMatrixThread([&]() {
        firstMatrix = generateMatrix(N, M);

        std::cout << "First matrix successfully generated! Thread: " << std::this_thread::get_id() << "\n\n";
    });

    generateFirstMatrixThread.join();

    Matrix secondMatrix;
    std::thread generateSecondMatrixThread([&]() {
        secondMatrix = generateMatrix(N, M);

        std::cout << "Second matrix successfully generated! Thread: " << std::this_thread::get_id() << "\n\n";
    });

    generateSecondMatrixThread.join();

    std::ofstream firstMatrixFile("firstMatrix.txt");
    printMatrix(firstMatrix, firstMatrixFile);
    firstMatrixFile.close();

    std::ofstream secondMatrixFile("secondMatrix.txt");
    printMatrix(secondMatrix, secondMatrixFile);
    secondMatrixFile.close();


    Matrix resultMatrix;


    auto begin = std::chrono::steady_clock::now();
    std::thread sumMatricesThread([&]() {
        resultMatrix = sumMatrices(firstMatrix, secondMatrix, 1);

        std::cout << "Matrices successfully summed! Thread: " << std::this_thread::get_id() << "\n";
    });

    sumMatricesThread.join();

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * 1.0 / 1000000 << " seconds" << "\n\n";



    std::thread saveMatrixInFileThread([&]() {
        std::ofstream resultMatrixFile("result.txt");
        printMatrix(resultMatrix, resultMatrixFile);
        resultMatrixFile.close();

        std::cout << "Matrix successfully saved! Thread: " << std::this_thread::get_id() << "\n\n";
    });
    saveMatrixInFileThread.join();

    return 0;
}