#include <future>
#include "../headers/Starter.h"
#include "../headers/Worker.h"
#include "../headers/OutputWriter.h"
#include "../headers/ParallelMultiplier.h"
#include "../headers/InputReader.h"

void Starter::run() {
    Log::info("Starter work begin");

    execute();
}

void Starter::execute() {
    auto matrices_queue = SynchronizedQueue<MatricesHolder>();
    auto multiply_queue = SynchronizedQueue<Matrix>();

    std::vector<std::thread> readers;
    std::vector<std::thread> multipliers;
    std::vector<std::thread> writers;

    auto iterations = configuration->get_iterations_count();
    auto input_file = configuration->get_input_file_name();
    auto output_file = configuration->get_output_file_name();

    for (auto i = 0; i < configuration->get_readers_count(); i++) {
        auto reader = std::thread([&matrices_queue, &input_file, &iterations]() {
            InputReader(matrices_queue, input_file, iterations).run();
        });
        readers.emplace_back(std::move(reader));
    }

    for (auto i = 0; i < configuration->get_multipliers_count(); i++) {
        auto multiplier = std::thread([&matrices_queue, &multiply_queue, this]() {
            multiply(matrices_queue, multiply_queue);
        });
        multipliers.emplace_back(std::move(multiplier));
    }

    for (auto i = 0; i < configuration->get_writers_count(); i++) {
        auto output_writer = std::thread([&multiply_queue, &iterations, &output_file]() {
            OutputWriter(multiply_queue, iterations, output_file).run();
        });
        writers.emplace_back(std::move(output_writer));
    }

    join_all(readers);
    matrices_queue.close();

    join_all(multipliers);
    multiply_queue.close();

    join_all(writers);
}

void Starter::join_all(std::vector<std::thread> &threads) {
    for (auto &thread: threads) {
        thread.join();
    }
}

void Starter::multiply(SynchronizedQueue<MatricesHolder> &matrices_queue,
                       SynchronizedQueue<Matrix> &sums_queue) {
    ParallelMultiplier(matrices_queue, sums_queue,
                       *configuration->get_multiplier(),
                       configuration->get_workers_count(),
                       configuration->get_iterations_count()).run();
}

