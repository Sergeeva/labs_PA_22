#ifndef PROCESSES_READINGPROCESS_H
#define PROCESSES_READINGPROCESS_H

#include "definitions.h"
#include "sharedmemory.h"

class ReadingProcess {
public:
    ReadingProcess(char *, char *);
    ~ReadingProcess();
    void run();

private:
    void readMatrix();

    SharedMemory *sharedMemory;
    char *inputFileName[2];
    int fileCounter;

};

#endif //PROCESSES_READINGPROCESS_H
