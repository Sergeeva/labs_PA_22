#include "task1.h"

int main() {
    log("Task1 started");
    startProcessFor(readMatrices);
    startProcessFor(sumMatrices);
    startProcessFor(writeMatrices);

    return 0;
}

void startProcessFor(const std::function<void()> &toDo) {
    auto pid = fork();

    switch (pid) {
        case ForkPids::child:
            toDo();
            exit(0);

        case ForkPids::error:
            std::cout << "Got error while forking process";
            break;

        default: // parent
            wait(&pid);
    }
}

void *getSharedPointer() {
    const int size = 1024;

    auto id = shmget(1, size, 0644 | IPC_CREAT);
    if (id == -1) {
        throw std::runtime_error("Error during creating/getting shared memory");
    }

    log("Got shared memory with id: " + std::to_string(id));

    auto pointer = shmat(id, nullptr, 0);
    if (pointer == (void *) -1) {
        throw std::runtime_error("Error during attaching to shared memory");
    }

    log("Process successfully attached to shared memory.");

    return pointer;
}

void readMatrices() {
    std::ifstream matrixInputFile(Filenames::inputFilename);

    auto firstMatrix = Matrix();
    auto secondMatrix = Matrix();

    matrixInputFile >> firstMatrix;
    matrixInputFile >> secondMatrix;
    log("Matrices loaded.");

    matrixInputFile.close();

    auto sharedMemoryPointer = (int *) getSharedPointer();

    auto pointerAfterFirstMatrix = firstMatrix.writeByPtr(sharedMemoryPointer);
    secondMatrix.writeByPtr(pointerAfterFirstMatrix);
}

void sumMatrices() {
    auto sharedMemoryPointer = (int *) getSharedPointer();

    auto firstMatrixWithNextPointer = Matrix::readByPtr(sharedMemoryPointer);
    auto const &firstMatrix = firstMatrixWithNextPointer.first;

    auto secondMatrixWithNextPointer = Matrix::readByPtr(firstMatrixWithNextPointer.second);
    auto const &secondMatrix = secondMatrixWithNextPointer.first;

    auto summedMatrix = firstMatrix + secondMatrix;
    summedMatrix.writeByPtr(sharedMemoryPointer);
    log("Got result of sum.");
}

void writeMatrices() {
    auto sharedMemoryPointer = (int *) getSharedPointer();

    auto matrixWithNextPointer = Matrix::readByPtr(sharedMemoryPointer);
    auto const &matrix = matrixWithNextPointer.first;

    std::ofstream matrixOutputFile(Filenames::outputFilename);
    matrixOutputFile << matrix;
    matrixOutputFile.close();

    log("Matrices written to file: " + std::string(Filenames::outputFilename));
}
