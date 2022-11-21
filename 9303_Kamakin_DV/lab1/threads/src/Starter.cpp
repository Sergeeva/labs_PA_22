#include <future>
#include <spdlog/spdlog.h>
#include "../headers/Starter.h"
#include "../headers/Worker.h"
#include "../headers/OutputWriter.h"
#include "../headers/Summator.h"
#include "../headers/InputReader.h"

void Starter::run() {
    auto first = Matrix();
    auto second = Matrix();
    auto holder = MatricesHolder(first, second);
    auto sum_result = std::promise<Matrix>();
    auto sum_future = sum_result.get_future();

    auto thread = std::thread([&holder]() { InputReader(holder).run(); });
    thread.join();

    thread = std::thread(Starter::sum, std::ref(holder), std::move(sum_result));
    thread.join();

    auto result = sum_future.get();

    thread = std::thread([&result]() { OutputWriter(result).run(); });
    thread.join();

    spdlog::info("Program finished successfully");
}

void Starter::sum(MatricesHolder &holder, std::promise<Matrix> result_future) {
    Summator(holder, std::move(result_future)).run();
}

