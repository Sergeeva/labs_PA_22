#include "../headers/OutputWriter.h"
#include "../headers/Summator.h"
#include "../headers/InputReader.h"
#include "../headers/WorkerFactory.h"

OutputWriter WorkerFactory::create_output_writer() {
    return {};
}

Summator WorkerFactory::create_summator() {
    return {};
}

InputReader WorkerFactory::create_input_reader() {
    return {};
}

