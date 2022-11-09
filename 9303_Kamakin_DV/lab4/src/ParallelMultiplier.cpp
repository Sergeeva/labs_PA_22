#include "../headers/ParallelMultiplier.h"

void ParallelMultiplier::run() {
    Log::info("ParallelMultiplier started");

    SynchronizedQueue<Runnable> tasks;
    auto threads = initialize_threads(tasks);

    for (auto i = 0; i < iterations_count; i++) {
        Log::info("ParallelMultiplier iteration #" + std::to_string(i));

        auto holder_pointer = matrices_queue.pop();

        if (holder_pointer) {
            Log::info("ParallelMultiplier got matrices");

            auto start = std::chrono::steady_clock::now();

            auto future_matrix = multiplier.multiply(holder_pointer.get(), &tasks);

            Log::info("ParallelMultiplier waiting for tasks to be done");

            auto result = future_matrix->get_future().get();

            auto end = std::chrono::steady_clock::now();

            Log::info("ParallelMultiplier pushing result, algorithm took " +
                      std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                                             .count()) + " ms");


            sums_queue.push(result);
        }
    }

    Log::info("ParallelMultiplier stopping workers");

    tasks.close();

    for (auto &thread: threads) {
        thread.join();
    }

    Log::info("ParallelMultiplier work done");
}

std::vector<MultiplyingThread> ParallelMultiplier::initialize_threads(SynchronizedQueue<Runnable> &tasks) const {
    Log::info("ParallelMultiplier initializing threads, count: " + std::to_string(thread_count));
    std::vector<MultiplyingThread> threads;
    threads.reserve(thread_count);

    for (auto i = 0; i < thread_count; i++) {
        threads.emplace_back(MultiplyingThread(i, tasks));
        threads.back().run();
    }

    Log::info("ParallelMultiplier threads initialized");
    return threads;
}
