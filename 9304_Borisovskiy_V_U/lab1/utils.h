#ifndef PAFIRSTLAB_UTILS_H
#define PAFIRSTLAB_UTILS_H

#include <iostream>
#include <sys/shm.h>
#include <vector>
#include <random>
#include <unistd.h>
#include <thread>
#include <fstream>

#define N 15
#define M 15

typedef std::vector<std::vector<int>> Matrix;

Matrix generateMatrix(int width, int height) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-1000, 1000); // define the range

    Matrix matrix(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix[i][j] = distr(gen);
        }
    }

    return matrix;
}

void printMatrix(Matrix matrix, std::ostream &ostream) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            ostream << matrix[i][j] << " ";
        }

        ostream << "\n";
    }
}

int getSharedMemoryId() {
    const int key = 1234;
    const int size = 1024;

    int sharedMemoryId = shmget(key, size, 0644 | IPC_CREAT);

    if (sharedMemoryId == 1) {
        std::cout << "Shared memory allocation error!";
        exit(1);
    }

    return sharedMemoryId;
}

void * getSharedMemoryPointer(int sharedMemoryId) {
    void * sharedMemoryPointer = shmat(sharedMemoryId, nullptr, 0);
    if (sharedMemoryPointer == (void *) -1) {
        std::cout << "Shared memory attaching error!";
        exit(1);
    }

    return sharedMemoryPointer;
}


int * writeMatrixToSharedMemory(int *sharedMemoryPointer, const Matrix &matrix) {
    *sharedMemoryPointer = matrix.size();
    sharedMemoryPointer++;

    *sharedMemoryPointer = matrix[0].size();
    sharedMemoryPointer++;

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            *sharedMemoryPointer = matrix[i][j];
            sharedMemoryPointer++;
        }
    }

    return sharedMemoryPointer;
}

int * readMatrixFromSharedMemory(int *sharedMemoryPointer, Matrix &outMatrix) {
    int height = *sharedMemoryPointer;
    sharedMemoryPointer++;

    int width = *sharedMemoryPointer;
    sharedMemoryPointer++;

    outMatrix.resize(height);

    for (int i = 0; i < height; i++) {
        outMatrix[i].resize(width);
        for (int j = 0; j < width; j++) {
            outMatrix[i][j] = *sharedMemoryPointer;
            sharedMemoryPointer++;
        }
    }

    return sharedMemoryPointer;
}

#endif //PAFIRSTLAB_UTILS_H
