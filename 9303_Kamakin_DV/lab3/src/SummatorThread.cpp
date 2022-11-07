#include <thread>
#include "../headers/SummatorThread.h"

void SummatorThread::run() {
    thread = std::thread([this]() { start(); });
}

void SummatorThread::join() {
    thread.join();
}

void SummatorThread::start() {
    Log::info("SummatorThread started, #" + std::to_string(id));

    auto task_pointer = tasks.pop();

    while (task_pointer) {
        task_pointer->run();
        processed_batches++;
        processed_batches.notify_one();

        task_pointer = tasks.pop();
    }

    Log::info("SummatorThread has stopped, #" + std::to_string(id));
}

