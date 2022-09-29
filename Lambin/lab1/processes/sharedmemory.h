#ifndef PROCESSES_SHAREDMEMORY_H
#define PROCESSES_SHAREDMEMORY_H

#include "definitions.h"

class SharedMemory {
public:
    SharedMemory(process);
    ~SharedMemory();
    bool checkMemory() const;
    void setProcessStatus(process, status);
    status getProcessStatus(process) const;
    void crash();
    bool isCrash() const;
    void setValue(double, matrix, int, int);
    double getValue(matrix, int, int) const;
    int getMatrixHeight(matrix) const;
    int getMatrixWidth(matrix) const;

private:
    struct MemoryBlock {
        Matrix<double> matrixA;
        Matrix<double> matrixB;
        Matrix<double> matrixC;
        status readingProcessStatus = START;
        status calculationProcessStatus = START;
        status writingProcessStatus = START;
        bool isCrash = false;
    };
    process currentProcess;
    int semaphoreID;
    int memoryBlockID;
    MemoryBlock *memoryBlock;

};

#endif //PROCESSES_SHAREDMEMORY_H
