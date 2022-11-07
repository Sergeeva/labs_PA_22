#ifndef UNTITLED_SUMTASK_H
#define UNTITLED_SUMTASK_H

#include "MatricesHolder.h"
#include "Matrix.h"
#include "Runnable.h"

class SumTask : public Runnable {

private:
    int offset;
    int batch_size;
    int total_size;
    Matrix &target;
    MatricesHolder &holder;

public:
    SumTask(int offset,
            int batch_size,
            int total_size,
            Matrix &target,
            MatricesHolder &holder) : offset(offset),
                                      batch_size(batch_size),
                                      total_size(total_size),
                                      target(target),
                                      holder(holder) {}

    void run();

};


#endif //UNTITLED_SUMTASK_H
