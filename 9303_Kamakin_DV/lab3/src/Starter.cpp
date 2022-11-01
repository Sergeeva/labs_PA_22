#include <future>
#include "../headers/Starter.h"
#include "../headers/Worker.h"
#include "../headers/OutputWriter.h"
#include "../headers/ParallelSummator.h"
#include "../headers/InputReader.h"

void Starter::run() {
    Log::info("Starter work begin");

    auto start = std::chrono::steady_clock::now();

    run(Configuration::small_matrix_file_name, 5, 1, 5, 5, 5);

    auto end = std::chrono::steady_clock::now();
    Log::info("Duration: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()));

    Log::info("Program finished successfully");
}

void Starter::run(const std::string &file_name, int readers_count, int summators_count, int writers_count,
                  int iterations_count,
                  int summator_thread_count) {
    auto matrices_queue = SynchronizedQueue<MatricesHolder>();
    auto sums_queue = SynchronizedQueue<Matrix>();

    std::vector<std::thread> readers;
    std::vector<std::thread> summators;
    std::vector<std::thread> writers;

    for (auto i = 0; i < readers_count; i++) {
        auto reader = std::thread([&matrices_queue, &file_name, &iterations_count]() {
            InputReader(matrices_queue, file_name, iterations_count).run();
        });
        readers.emplace_back(std::move(reader));
    }

    for (auto i = 0; i < summators_count; i++) {
        auto summator = std::thread(Starter::sum, std::ref(matrices_queue), std::ref(sums_queue), iterations_count,
                                    summator_thread_count);
        summators.emplace_back(std::move(summator));
    }

    for (auto i = 0; i < writers_count; i++) {
        auto output_writer = std::thread([&sums_queue, &iterations_count]() {
            OutputWriter(sums_queue, iterations_count).run();
        });
        writers.emplace_back(std::move(output_writer));
    }

    join_all(readers);
    matrices_queue.close();

    join_all(summators);
    sums_queue.close();

    join_all(writers);
}

void Starter::join_all(std::vector<std::thread> &threads) {
    for (auto &thread: threads) {
        thread.join();
    }
}

void Starter::sum(SynchronizedQueue<MatricesHolder> &matrices_queue,
                  SynchronizedQueue<Matrix> &sums_queue,
                  int iterations_count,
                  int thread_count) {
    ParallelSummator(matrices_queue, sums_queue, thread_count, iterations_count).run();
}

