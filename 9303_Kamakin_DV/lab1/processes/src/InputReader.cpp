#include <spdlog/spdlog.h>
#include <fstream>
#include "../headers/InputReader.h"

void InputReader::run() {
    spdlog::info("Start InputReader");

    read_to(SharedMemory(boost::interprocess::mode_t::read_write, Configuration::two_matrices_memory)
                    .map_region(boost::interprocess::mode_t::read_write));
}

template<class ElemConsumer>
void InputReader::read_matrix(std::ifstream &source_file, const ElemConsumer &consumer) const {
    int height;
    int width;

    source_file >> height;
    source_file >> width;

    spdlog::info("read_matrix: reading from file height: {}, width: {}", height, width);

    consumer(height);
    consumer(width);

    int elem;

    for (auto y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            source_file >> elem;

            consumer(elem);
        }
    }
}

void InputReader::read_to(SharedMemory &shared_memory) const {
    spdlog::info("read_to: reading from {}", Configuration::input_file_name);

    std::ifstream input_file(Configuration::input_file_name);

    using namespace boost::interprocess;
    auto consumer = [&shared_memory](int elem) { shared_memory.write(elem); };

    shared_memory.lock();

    read_matrix(input_file, consumer);
    read_matrix(input_file, consumer);

    shared_memory.unlock();

    input_file.close();
}
