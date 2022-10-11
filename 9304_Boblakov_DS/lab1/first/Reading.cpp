#include "Reading.h"

void process_read(){

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