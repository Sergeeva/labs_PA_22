#include <thread>
#include "../headers/MultiplyingThread.h"

void MultiplyingThread::run() {
    thread = std::thread([this]() { start(); });
}

void MultiplyingThread::join() {
    thread.join();
}

void MultiplyingThread::start() {
    Log::info("MultiplyingThread started, #" + std::to_string(id));

    auto task_pointer = tasks.pop();

    while (task_pointer) {
        task_pointer->run();

        task_pointer = tasks.pop();
    }

    Log::info("MultiplyingThread has stopped, #" + std::to_string(id));
}

