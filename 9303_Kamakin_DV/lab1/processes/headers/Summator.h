#ifndef UNTITLED_SUMMATOR_H
#define UNTITLED_SUMMATOR_H

#include "Worker.h"
#include "SharedMemory.h"
#include "Matrix.h"

class Summator : public Worker {

public:

    void run() final;

    Matrix sum(SharedMemory &memory);

    void write_result(SharedMemory &memory, Matrix &result);

};


#endif //UNTITLED_SUMMATOR_H
