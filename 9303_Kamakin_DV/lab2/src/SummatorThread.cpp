#include "../headers/SummatorThread.h"


void SummatorThread::run() {
    thread = std::thread([this]() { start(); });
}

void SummatorThread::join() {
    thread.join();
}

void SummatorThread::start() {
    spdlog::info("SummatorThread №{} starting, is need to work: {}", id, is_need_to_work);

    try {
        while (true) {
            tasks.pop().run();

            processed_batches++;
            processed_batches.notify_one();
        }
    } catch (std::runtime_error &e) {
        spdlog::error(e.what());
    }

    spdlog::info("SummatorThread №{} has stopped", id);
}

