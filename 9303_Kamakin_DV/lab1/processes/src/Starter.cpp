#include <csignal>
#include <functional>
#include <stdexcept>
#include <future>
#include <spdlog/spdlog.h>
#include "../headers/Starter.h"
#include "../headers/Worker.h"
#include "../headers/OutputWriter.h"
#include "../headers/Summator.h"
#include "../headers/WorkerFactory.h"

enum class ForkResult {
    FAIL = -1,
    CHILD = 0
};

void Starter::run() {
    using namespace WorkerFactory;
    fork_and_run<>(create_input_reader);
    fork_and_run<>(create_summator);
    fork_and_run<>(create_output_writer);

    std::promise<void>().get_future().wait();
}

template<class WorkerFactory>
void Starter::fork_and_run(WorkerFactory factory) const {
    auto forked = static_cast<ForkResult>(fork());

    switch (forked) {
        case ForkResult::CHILD:
            factory().run();

            spdlog::info("Job done, exiting");

            exit(0);

        case ForkResult::FAIL:
            throw std::runtime_error("Failed to fork child");
    }
}

