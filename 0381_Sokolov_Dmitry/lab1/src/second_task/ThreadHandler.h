#ifndef PROJECT_THREADHANDLER_H
#define PROJECT_THREADHANDLER_H

#pragma once

#include <functional>
#include <thread>
#include <future>

#include "unistd.h"

#include "../configuration/config.h"
#include "../utility/DataHandler.h"

class ThreadHandler {
    DataHandler data = DataHandler();

protected:

    std::thread init;
    std::thread summator;
    std::thread print;

    std::promise<Matrix> promised;

    void log_thread();

    void queue();

    Matrix get_queued();

    void input(Matrix &A, Matrix &B, bool remake);

    virtual void sum(Matrix &A, Matrix &B);

    void output(Matrix &matrix);

    void thread_input(Matrix &A, Matrix &B, bool remake);

    void thread_sum(Matrix& A, Matrix & B);

    void thread_output();

public:
    ThreadHandler() = default;

    virtual void execute(bool remake);

};

#endif //PROJECT_THREADHANDLER_H
