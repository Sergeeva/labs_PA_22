#include "../utils.h"

void firstProcess() {
    int pid = fork();

    if (pid == 0) { // child process
        int sharedMemoryId = getSharedMemoryId();
        int *sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

        Matrix firstMatrix;
        std::thread firstThread([&]() {
            firstMatrix = generateMatrix(N, M);
        });

        Matrix  secondMatrix;
        std::thread secondThread([&]() {
            secondMatrix = generateMatrix(N, M);
        });

        firstThread.join();
        secondThread.join();

        sharedMemoryPointer = writeMatrixToSharedMemory(sharedMemoryPointer, firstMatrix);
        writeMatrixToSharedMemory(sharedMemoryPointer, secondMatrix);


        std::ofstream firstMatrixFile("firstMatrix.txt");
        printMatrix(firstMatrix, firstMatrixFile);
        firstMatrixFile.close();

        std::ofstream secondMatrixFile("secondMatrix.txt");
        printMatrix(secondMatrix, secondMatrixFile);
        secondMatrixFile.close();

        std::cout << "The first process completed successfully! PID: " << getpid() << "\n\n";

        exit(0);
    } else if (pid == -1) {
        std::cout << "Fork error!";
        exit(1);
    } else { // parent process
        wait(&pid);
    }
}

void secondProcess() {
    int pid = fork();

    if (pid == 0) { // child process
        int sharedMemoryId = getSharedMemoryId();
        int *sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

        Matrix firstMatrix;
        sharedMemoryPointer = readMatrixFromSharedMemory(sharedMemoryPointer, firstMatrix);

        Matrix  secondMatrix;
        readMatrixFromSharedMemory(sharedMemoryPointer, secondMatrix);

        int height = firstMatrix.size();
        int width = firstMatrix[0].size();
        Matrix resultMatrix(height, std::vector<int>(width));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                resultMatrix[i][j] = firstMatrix[i][j] + secondMatrix[i][j];
            }
        }

        sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

        writeMatrixToSharedMemory(sharedMemoryPointer, resultMatrix);

        std::cout << "The second process completed successfully! PID: " << getpid() << "\n\n";

        exit(0);
    } else if (pid == -1) {
        std::cout << "Fork error!";
        exit(1);
    } else { // parent process
        wait(&pid);
    }
}

void thirdProcess() {
    int pid = fork();

    if (pid == 0) { // child process
        int sharedMemoryId = getSharedMemoryId();
        int *sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

        Matrix resultMatrix;
        readMatrixFromSharedMemory(sharedMemoryPointer, resultMatrix);

        std::ofstream resultMatrixFile("result.txt");
        printMatrix(resultMatrix, resultMatrixFile);
        resultMatrixFile.close();

        std::cout << "The third process completed successfully! PID: " << getpid() << "\n\n";

        exit(0);
    } else if (pid == -1) {
        std::cout << "Fork error!";
        exit(1);
    } else { // parent process
        wait(&pid);
    }
}

int main() {
    firstProcess();
    secondProcess();
    thirdProcess();

    return 0;
}
