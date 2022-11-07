#include "../utils.h"

Matrix sumMatrices(Matrix firstMatrix, Matrix secondMatrix) {
    Matrix resultMatrix(firstMatrix.size(), std::vector<int>(firstMatrix[0].size()));

    for (int i = 0; i < firstMatrix.size(); i++) {
        for (int j = 0; j < firstMatrix[i].size(); j++) {
            resultMatrix[i][j] = firstMatrix[i][j] + secondMatrix[i][j];
        }
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

    std::ofstream firstMatrixFile("./firstMatrix");
    printMatrix(firstMatrix, firstMatrixFile);
    firstMatrixFile.close();

    std::ofstream secondMatrixFile("./secondMatrix");
    printMatrix(secondMatrix, secondMatrixFile);
    secondMatrixFile.close();


    Matrix resultMatrix;
    std::thread sumMatricesThread([&]() {
        resultMatrix = sumMatrices(firstMatrix, secondMatrix);

        std::cout << "Matrices successfully summed! Thread: " << std::this_thread::get_id() << "\n\n";
    });

    sumMatricesThread.join();

    std::thread saveMatrixInFileThread([&]() {
        std::ofstream resultMatrixFile("result.txt");
        printMatrix(resultMatrix, resultMatrixFile);
        resultMatrixFile.close();

        std::cout << "Matrices successfully saved! Thread: " << std::this_thread::get_id() << "\n\n";
    });

    saveMatrixInFileThread.join();

    return 0;
}