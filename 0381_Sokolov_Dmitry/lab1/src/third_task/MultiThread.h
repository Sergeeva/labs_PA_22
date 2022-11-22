#ifndef PROJECT_MULTITHREAD_H
#define PROJECT_MULTITHREAD_H

#include <thread>
#include <future>
#include <functional>

#include "../configuration/config.h"
#include "../utility/DataHandler.h"
#include "../utility/session_timer.h"

class MultiThread {

    bool logs;

    DataHandler data = DataHandler();

    std::promise<Matrix> promised;

    int thread_number = PThreadsConfig::min_threads;
    int max_threads = PThreadsConfig::max_threads;;

    std::thread init;
    std::thread out;

    SessionTimer timer = SessionTimer();

protected:

    void input(Matrix &A, Matrix &B, bool remake);
    void thread_input(Matrix& A, Matrix&  B, bool remake);

    void queue();
    std::future<Matrix> get_queued();

    void sum(Matrix& A, Matrix& B);

    void output(Matrix &matrix);
    void thread_output();

    void log_thread();

public:
    MultiThread() = default;

    void set_threads(int thread_number);

    void log_mode(bool write_logs);

    void execute(bool remake = false);

    long long int timed_execute(bool remake = false);

};


#endif //PROJECT_MULTITHREAD_H
