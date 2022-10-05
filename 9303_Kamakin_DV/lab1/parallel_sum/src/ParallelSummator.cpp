#include <spdlog/spdlog.h>
#include "../headers/ParallelSummator.h"

void ParallelSummator::run() {
    spdlog::info("Start ParallelSummator");

    auto sum_result = sum();

    spdlog::info("ParallelSummator work done, set future");

    result.set_value(sum_result);
}

Matrix ParallelSummator::sum() {
    if (holder.get_first().get_height() != holder.get_second().get_height() ||
        holder.get_first().get_width() != holder.get_second().get_width() ||
        thread_count <= 0) {
        throw std::runtime_error(
                "Invalid input: matrices must have same dimensions and thread count greater than zero");
    }

    auto height = holder.get_first().get_height();
    auto width = holder.get_first().get_width();
    auto sum_result = Matrix(height, width);

    auto total_size = height * width;
    auto batch_size = calculate_batch_size(total_size);

    spdlog::info("Thread count: {}, batch size: {}", thread_count, batch_size);

    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    for (auto i = 0; i < total_size; i += batch_size) {
        threads.emplace_back(std::move(std::thread(ParallelSummator::sum_thread,
                                                   i,
                                                   batch_size,
                                                   total_size,
                                                   std::ref(sum_result),
                                                   std::ref(holder))));
    }

    for (auto &thread: threads) {
        thread.join();
    }

    return sum_result;
}

void ParallelSummator::sum_thread(int offset, int batch_size, int total_size, Matrix &target,
                                  MatricesHolder &holder) {
    spdlog::info("Thread started, current batch: {}", offset);

    for (int i = offset; i < offset + batch_size && i < total_size; i++) {
        int height = i / target.get_width();
        int width = i % target.get_width();
        int sum = holder.get_first().get(height, width) + holder.get_second().get(height, width);

        target.set(height, width, sum);
    }

    spdlog::info("Thread finished");
}

int ParallelSummator::calculate_batch_size(int total_size) const {
    return (total_size / thread_count) + (total_size % thread_count > 0 ? 1 : 0);
}