#ifndef PROCESSES_CALCULATIONPROCESS_H
#define PROCESSES_CALCULATIONPROCESS_H

#include "definitions.h"
#include "sharedmemory.h"

class CalculationProcess {
public:
    CalculationProcess();
    ~CalculationProcess();
    void run();

private:
    void calculateMatrix();

    SharedMemory *sharedMemory;

};

#endif //PROCESSES_CALCULATIONPROCESS_H
