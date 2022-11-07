#include <future>
#include <spdlog/spdlog.h>
#include "../headers/Starter.h"
#include "../headers/Worker.h"
#include "../headers/OutputWriter.h"
#include "../headers/ParallelSummator.h"
#include "../headers/InputReader.h"

void Starter::run() {
    analyse_file(Configuration::small_matrix_file_name);
    analyse_file(Configuration::big_matrix_file_name);

    spdlog::info("Program finished successfully");
}

void Starter::analyse_file(const std::string &file_name) {
    for (auto i = 1; i < Configuration::thread_count; i++) {
        auto time = run_and_mark_time(i, file_name);
        spdlog::info("Summing file {} with {} threads took {} ms", file_name, i, time);
    }
}

long long Starter::run_and_mark_time(int thread_count, const std::string &file_name) {
    auto first = Matrix();
    auto second = Matrix();
    auto holder = MatricesHolder(first, second);
    auto sum_result = std::promise<Matrix>();
    auto sum_future = sum_result.get_future();

    auto thread = std::thread([&holder, &file_name]() { InputReader(holder, file_name).run(); });
    thread.join();

    auto start = std::chrono::steady_clock::now();
    thread = std::thread(Starter::sum, std::ref(holder), std::move(sum_result), thread_count);
    thread.join();

    auto result = sum_future.get();
    auto end = std::chrono::steady_clock::now();

    thread = std::thread([&result]() { OutputWriter(result).run(); });
    thread.join();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void Starter::sum(MatricesHolder &holder, std::promise<Matrix> result_future, int thread_count) {
    ParallelSummator(holder, std::move(result_future), thread_count).run();
}

