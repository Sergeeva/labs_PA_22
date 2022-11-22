#ifndef PROJECT_PROCESSHANDLER_H
#define PROJECT_PROCESSHANDLER_H

#include <functional>

#include <sys/wait.h>
#include <sys/shm.h>

//#include "../configuration/config.h"

#include "../utility/DataHandler.h"
#include "./shared_memory/SharedMemory.h"


class ProcessHandler {

    SharedMemory storage = SharedMemory();
    DataHandler data = DataHandler();

    bool renew = Config::remake;

public:
    ProcessHandler();

    std::function<void(void)> get_func(int id);

    void process(const std::function<void()> &func);

    void input();

    void sum();

    void output();

    void run(bool remake = Config::remake);

};


#endif //PROJECT_PROCESSHANDLER_H
