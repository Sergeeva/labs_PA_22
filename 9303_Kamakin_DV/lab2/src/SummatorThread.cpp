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

    try {
        while (true) {
            tasks.pop().run();

            processed_batches++;
            processed_batches.notify_one();
        }
    } catch (std::runtime_error &e) {
        Log::info(e.what());
    }

    Log::info("SummatorThread has stopped, #" + std::to_string(id));
}

