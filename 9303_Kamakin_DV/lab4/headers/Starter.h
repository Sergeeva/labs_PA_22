#ifndef UNTITLED_STARTER_H
#define UNTITLED_STARTER_H

#include <future>
#include <utility>
#include "Runnable.h"
#include "MatricesHolder.h"
#include "SynchronizedQueue.h"
#include "Configuration.h"

class Starter : public Runnable {

private:

    std::shared_ptr<Configuration> configuration;

    void multiply(SynchronizedQueue<MatricesHolder> &matrices_queue, SynchronizedQueue<Matrix> &sums_queue);

    static void join_all(std::vector<std::thread> &threads);

    void execute();

public:

    void run() final;

    explicit Starter(std::shared_ptr<Configuration> configuration) : configuration(std::move(configuration)) {}

};


#endif //UNTITLED_STARTER_H
