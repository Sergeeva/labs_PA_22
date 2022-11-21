#ifndef UNTITLED_STARTER_H
#define UNTITLED_STARTER_H

#include "Runnable.h"

class Starter : public Runnable {

private:

    template<class WorkerFactory>
    void fork_and_run(WorkerFactory factory) const;

public:

    void run() final;

};


#endif //UNTITLED_STARTER_H
