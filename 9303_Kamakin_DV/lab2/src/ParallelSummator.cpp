#include <spdlog/spdlog.h>
#include "../headers/ParallelSummator.h"
#include "../headers/SumTask.h"
#include "../headers/SummatorThread.h"

void ParallelSummator::run() {
    spdlog::info("ParallelSummator started");

    SynchronizedQueue<SumTask> tasks;
    std::atomic<int> processed_batches;
    auto threads = initialize_summators(tasks, processed_batches);

    for (auto i = 0; i < iterations_count; i++) {
        spdlog::info("ParallelSummator iteration №{}", i);

        auto holder = matrices_queue.pop();
        auto result = sum(holder, tasks, threads, processed_batches);

        spdlog::info("ParallelSummator pushing result");

        sums_queue.push(result);
    }

    spdlog::info("ParallelSummator stopping workers");

    tasks.close();

    for (auto &thread: threads) {
        thread.join();
    }

    spdlog::info("ParallelSummator work done");
}

std::vector<SummatorThread> ParallelSummator::initialize_summators(SynchronizedQueue<SumTask> &tasks,
                                                                   std::atomic<int> &processed_batches) {
    spdlog::info("ParallelSummator initializing summators");
    std::vector<SummatorThread> threads;
    threads.reserve(thread_count);

    for (auto i = 0; i < thread_count; i++) {
        threads.emplace_back(SummatorThread(i, processed_batches, tasks));
        threads.back().run();
    }

    spdlog::info("ParallelSummator summators initialized");
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

    spdlog::info("ParallelSummator start creating tasks");

    for (auto i = 0; i < total_size; i += batch_size) {
        auto task = SumTask(i, batch_size, total_size, sum_result, holder);

        tasks.push(task);
    }

    spdlog::info("ParallelSummator created {} tasks", batches_count);

    auto actual = processed_batches.load();

    while (actual != batches_count) {
        spdlog::info("ParallelSummator actual processed size: {}", actual);
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