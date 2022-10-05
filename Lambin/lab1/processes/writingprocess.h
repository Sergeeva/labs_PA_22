#ifndef PROCESSES_WRITINGPROCESS_H
#define PROCESSES_WRITINGPROCESS_H

#include "definitions.h"
#include "sharedmemory.h"

class WritingProcess {
public:
    WritingProcess(char *);
    ~WritingProcess();
    void run();

private:
    void writeMatrix();

    SharedMemory *sharedMemory;
    char *outputFileName;

};

#endif //PROCESSES_WRITINGPROCESS_H
