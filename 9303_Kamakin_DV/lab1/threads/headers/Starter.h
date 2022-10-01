#ifndef UNTITLED_STARTER_H
#define UNTITLED_STARTER_H

#include <future>
#include "Runnable.h"
#include "MatricesHolder.h"

class Starter : public Runnable {

private:
    static void sum(MatricesHolder &holder, std::promise<Matrix> result_future);

public:

    void run() final;

};


#endif //UNTITLED_STARTER_H
