#include "Runnable.h"
#include "SynchronizedQueue.h"
#include "MatricesHolder.h"
#include <future>

#ifndef UNTITLED_MATRICESMULTIPLIER_H
#define UNTITLED_MATRICESMULTIPLIER_H

class MatricesMultiplier {

public:

    virtual std::promise<Matrix *> *multiply(MatricesHolder *matrices,
                                             SynchronizedQueue<Runnable> *task_consumer) = 0;

    virtual ~MatricesMultiplier() = default;

};

#endif //UNTITLED_MATRICESMULTIPLIER_H

