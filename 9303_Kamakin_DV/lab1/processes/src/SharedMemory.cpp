#include <spdlog/spdlog.h>
#include "../headers/SharedMemory.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

SharedMemory::SharedMemory(boost::interprocess::mode_t mode, const std::string &memory_name) {
    using namespace boost::interprocess;
    spdlog::info("Open memory: {}", memory_name);
    shared_memory = shared_memory_object(open_or_create, memory_name.c_str(), mode);
}

SharedMemory &SharedMemory::map_region(boost::interprocess::mode_t mode) {
    spdlog::info("Map region, mode: {}", mode);
    region = boost::interprocess::mapped_region(SharedMemory::shared_memory, mode);
    return *this;
}

SharedMemory &SharedMemory::resize(long size) {
    spdlog::info("Resize shared memory: {}", size);
    shared_memory.truncate(size);
    return *this;
}

SharedMemory &SharedMemory::clear() {
    spdlog::info("Memory cleared");
    std::memset(region.get_address(), 0, region.get_size());
    return *this;
}

void SharedMemory::write(int elem) {
    *(static_cast<int *>(region.get_address()) + offset++) = elem;
}

int SharedMemory::read() {
    return *(static_cast<int *>(region.get_address()) + offset++);
}

SharedMemory &SharedMemory::set_flag(WriteStatus status) {
    auto result = static_cast<int>(status);

    spdlog::info("Set flag: {}", result);

    *(static_cast<int *>(region.get_address())) = result;

    spdlog::info("Flag set", result);
    return *this;
}

int SharedMemory::get_flag() {
    return *(static_cast<int *>(region.get_address()));
}

void SharedMemory::lock() {
    set_flag(WriteStatus::WRITING);
}

void SharedMemory::unlock() {
    set_flag(WriteStatus::EMPTY);
}

void SharedMemory::wait() {
    while (get_flag() != static_cast<int>(WriteStatus::EMPTY)) {
        spdlog::info("Waiting");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    spdlog::info("Done waiting");
}

