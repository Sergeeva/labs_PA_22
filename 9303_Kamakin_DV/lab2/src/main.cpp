#include <filesystem>
#include "../headers/Starter.h"
#include "spdlog/spdlog.h"
#include "../headers/Configuration.h"

void init_logger() {
    spdlog::set_pattern("[%H:%M:%S %z] [thread %t] %v");
    spdlog::set_level(spdlog::level::debug);
}

int main() {
    init_logger();
    Starter().run();
    return 0;
}
