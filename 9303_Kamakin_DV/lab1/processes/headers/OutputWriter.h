#ifndef UNTITLED_OUTPUTWRITER_H
#define UNTITLED_OUTPUTWRITER_H

#include "Runnable.h"
#include "Worker.h"
#include "SharedMemory.h"

class OutputWriter : public Worker {

private:
    void print_result(SharedMemory &memory);

public:
    void run() final;

};


#endif //UNTITLED_OUTPUTWRITER_H
