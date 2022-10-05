#ifndef UNTITLED_STARTER_H
#define UNTITLED_STARTER_H

#include <future>
#include "Runnable.h"
#include "MatricesHolder.h"

class Starter : public Runnable {

private:
    static void sum(MatricesHolder &holder, std::promise<Matrix> result_future, int thread_count);

    static long long run_and_mark_time(int thread_count, const std::string &file_name);

    static void analyse_file(const std::string &file_name);

public:

    void run() final;

};


#endif //UNTITLED_STARTER_H
