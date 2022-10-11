#include <future>
#include <spdlog/spdlog.h>
#include "../headers/Starter.h"
#include "../headers/Worker.h"
#include "../headers/OutputWriter.h"
#include "../headers/ParallelSummator.h"
#include "../headers/InputReader.h"

void Starter::run() {
    run(Configuration::small_matrix_file_name, 5, 2);

    spdlog::info("Program finished successfully");
}

void Starter::run(const std::string &file_name, int iterations_count, int thread_count) {
    auto matrices_queue = SynchronizedQueue<MatricesHolder>();
    auto sums_queue = SynchronizedQueue<Matrix>();

    auto reader = std::thread([&matrices_queue, &file_name, &iterations_count]() {
        InputReader(matrices_queue, file_name, iterations_count).run();
    });

    auto summator = std::thread(Starter::sum, std::ref(matrices_queue), std::ref(sums_queue), iterations_count,
                                thread_count);

    auto output_writer = std::thread([&sums_queue, &iterations_count]() {
        OutputWriter(sums_queue, iterations_count).run();
    });

    reader.join();
    summator.join();
    output_writer.join();
}

void Starter::sum(SynchronizedQueue<MatricesHolder> &matrices_queue,
                  SynchronizedQueue<Matrix> &sums_queue,
                  int iterations_count,
                  int thread_count) {
    ParallelSummator(matrices_queue, sums_queue, thread_count, iterations_count).run();
}

