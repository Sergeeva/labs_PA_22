#include "calculationprocess.h"

CalculationProcess::CalculationProcess() {
    sharedMemory = new SharedMemory(CALCULATE);
}

CalculationProcess::~CalculationProcess() {
    delete sharedMemory;
}

void CalculationProcess::run() {
    if (!sharedMemory->checkMemory()) {
        std::cerr << "Shared memory error: unable to allocate memory\n";
        return;
    }

    sharedMemory->setProcessStatus(CALCULATE, RUN);
//    std::cout << "Calculation process start\n";

    try {
        calculateMatrix();
    } catch (std::runtime_error &exception) {
        sharedMemory->crash();
        std::cerr << exception.what() << '\n';
    }

    sharedMemory->setProcessStatus(CALCULATE, TERMINATE);
//    std::cout << "Calculation process terminate\n";
}

void CalculationProcess::calculateMatrix() {
    while (sharedMemory->getProcessStatus(READ) < RUN) {
        if (sharedMemory->isCrash()) {
            return;
        }
    }

    int y = 0;
    while (sharedMemory->getProcessStatus(READ) != TERMINATE || sharedMemory->getMatrixHeight(B) > y) {
        if (sharedMemory->getMatrixHeight(B) > y) {
            for (int x = 0; x < sharedMemory->getMatrixWidth(A); x++) {
                double sum = sharedMemory->getValue(A, x, y) + sharedMemory->getValue(B, x, y);
                sharedMemory->setValue(sum, C, x, y);
            }
        }
        y++;
    }
}
