#include <spdlog/spdlog.h>
#include "../headers/Summator.h"

void Summator::run() {
    spdlog::info("Start Summator");

    auto sum_result = sum();

    spdlog::info("Summator work done, set future");

    result.set_value(sum_result);
}

Matrix Summator::sum() {
    auto first = holder.get_first();
    auto second = holder.get_second();

    return first + second;
}