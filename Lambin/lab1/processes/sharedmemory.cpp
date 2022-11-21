#include "sharedmemory.h"

SharedMemory::SharedMemory(process proc) : currentProcess(proc) {
    memoryBlockID = shmget(SHMID, sizeof(MemoryBlock), PERMS | IPC_CREAT);
    memoryBlock = (MemoryBlock *) shmat(memoryBlockID, nullptr, 0);
}

SharedMemory::~SharedMemory() {
    shmdt(memoryBlock);
    if (currentProcess == READ) {
        if (shmctl(memoryBlockID, IPC_RMID, nullptr) < 0) {
            std::cerr << "Shared memory error: unable to remove memory block\n";
        }
    }
}

bool SharedMemory::checkMemory() const {
    return memoryBlockID >= 0 && memoryBlock != nullptr;
}

void SharedMemory::setProcessStatus(process proc, status stat) {
    if (proc != READ && proc != CALCULATE && proc != WRITE) {
        throw std::runtime_error("Shared memory error: wrong process name");
    }

    switch (proc) {
        case READ:
            memoryBlock->readingProcessStatus = stat;
            break;
        case CALCULATE:
            memoryBlock->calculationProcessStatus = stat;
            break;
        case WRITE:
            memoryBlock->writingProcessStatus = stat;
            break;
    }
}

status SharedMemory::getProcessStatus(process proc) const {
    if (proc != READ && proc != CALCULATE && proc != WRITE) {
        throw std::runtime_error("Shared memory error: wrong process name");
    }

    switch (proc) {
        case READ:
            return memoryBlock->readingProcessStatus;
        case CALCULATE:
            return memoryBlock->calculationProcessStatus;
        case WRITE:
            return memoryBlock->writingProcessStatus;
    }
}

void SharedMemory::crash() {
    memoryBlock->isCrash = true;
}

bool SharedMemory::isCrash() const {
    return memoryBlock->isCrash;
}

void SharedMemory::setValue(double value, matrix matrixName, int x, int y) {
    if (x < 0 || x >= MAXSIZE || y < 0 || y >= MAXSIZE) {
        throw std::runtime_error("Shared memory error: wrong coordinates");
    }

    Matrix<double> *matrixPointer;
    switch (matrixName) {
        case A:
            matrixPointer = &memoryBlock->matrixA;
            break;
        case B:
            matrixPointer = &memoryBlock->matrixB;
            break;
        case C:
            matrixPointer = &memoryBlock->matrixC;
            break;
    }

    matrixPointer->data[y][x] = value;
    if (x >= matrixPointer->width) {
        matrixPointer->width = x + 1;
    }
    if (y >= matrixPointer->height) {
        matrixPointer->height = y + 1;
    }
}

double SharedMemory::getValue(matrix matrixName, int x, int y) const {
    if (x < 0 || x >= MAXSIZE || y < 0 || y >= MAXSIZE) {
        throw std::runtime_error("Shared memory error: wrong coordinates");
    }

    Matrix<double> *matrixPointer;
    switch (matrixName) {
        case A:
            matrixPointer = &memoryBlock->matrixA;
            break;
        case B:
            matrixPointer = &memoryBlock->matrixB;
            break;
        case C:
            matrixPointer = &memoryBlock->matrixC;
            break;
    }

    return matrixPointer->data[y][x];
}

int SharedMemory::getMatrixHeight(matrix matrixName) const {
    if (matrixName != A && matrixName != B && matrixName != C) {
        throw std::runtime_error("Shared memory error: wrong matrix name");
    }

    switch (matrixName) {
        case A:
            return memoryBlock->matrixA.height;
        case B:
            return memoryBlock->matrixB.height;
        case C:
            return memoryBlock->matrixC.height;
    }
}

int SharedMemory::getMatrixWidth(matrix matrixName) const {
    if (matrixName != A && matrixName != B && matrixName != C) {
        throw std::runtime_error("Shared memory error: wrong matrix name");
    }

    switch (matrixName) {
        case A:
            return memoryBlock->matrixA.width;
        case B:
            return memoryBlock->matrixB.width;
        case C:
            return memoryBlock->matrixC.width;
    }
}
