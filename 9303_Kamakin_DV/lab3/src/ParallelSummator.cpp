#include "../headers/ParallelSummator.h"

void ParallelSummator::run() {
    Log::info("ParallelSummator started");

    SynchronizedQueue<SumTask> tasks;
    std::atomic<int> processed_batches;
    auto threads = initialize_summators(tasks, processed_batches);

    for (auto i = 0; i < iterations_count; i++) {
        Log::info("ParallelSummator iteration #" + std::to_string(i));

        auto holder_pointer = matrices_queue.pop();

        if (holder_pointer) {
            auto result = sum(*holder_pointer, tasks, threads, processed_batches);

            Log::info("ParallelSummator pushing result");

            sums_queue.push(result);
        }
    }

    Log::info("ParallelSummator stopping workers");

    tasks.close();

    for (auto &thread: threads) {
        thread.join();
    }

    Log::info("ParallelSummator work done");
}

std::vector<SummatorThread> ParallelSummator::initialize_summators(SynchronizedQueue<SumTask> &tasks,
                                                                   std::atomic<int> &processed_batches) {
    Log::info("ParallelSummator initializing summators");
    std::vector<SummatorThread> threads;
    threads.reserve(thread_count);

    for (auto i = 0; i < thread_count; i++) {
        threads.emplace_back(SummatorThread(i, processed_batches, tasks));
        threads.back().run();
    }

    Log::info("ParallelSummator summators initialized");
    return threads;
}

Matrix ParallelSummator::sum(MatricesHolder &holder,
                             SynchronizedQueue<SumTask> &tasks,
                             std::vector<SummatorThread> &threads,
                             std::atomic<int> &processed_batches) {
    auto height = holder.get_first().get_height();
    auto width = holder.get_first().get_width();
    auto sum_result = Matrix(height, width);

    auto total_size = height * width;
    auto batch_size = calculate_batch_size(total_size);
    auto batches_count = calculate_batches_count(total_size, batch_size);

    Log::info("ParallelSummator start creating tasks");

    for (auto i = 0; i < total_size; i += batch_size) {
        auto task = SumTask(i, batch_size, total_size, sum_result, holder);

        tasks.push(task);
    }

    Log::info("ParallelSummator created tasks: " + std::to_string(batches_count));

    auto actual = processed_batches.load();

    while (actual != batches_count) {
        Log::info("ParallelSummator actual processed size: " + std::to_string(actual));
        processed_batches.wait(actual);
        actual = processed_batches.load();
    }

    processed_batches = 0;

    return sum_result;
}

int ParallelSummator::calculate_batches_count(int total_size, int batch_size) const {
    return (total_size / batch_size) + (total_size % batch_size > 0 ? 1 : 0);
}

int ParallelSummator::calculate_batch_size(int total_size) const {
    return (total_size / thread_count) + (total_size % thread_count > 0 ? 1 : 0);
}