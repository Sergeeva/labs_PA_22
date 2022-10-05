#ifndef UNTITLED_SHAREDMEMORY_H
#define UNTITLED_SHAREDMEMORY_H

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

enum class WriteStatus {
    WRITING = -1,
    EMPTY = 1
};

class SharedMemory {

private:
    boost::interprocess::shared_memory_object shared_memory;
    boost::interprocess::mapped_region region;
    long offset = 1;

    SharedMemory &set_flag(WriteStatus status);

    int get_flag();

public:
    explicit SharedMemory(boost::interprocess::mode_t mode, const std::string &memory_name);

    SharedMemory &resize(long size);

    SharedMemory &map_region(boost::interprocess::mode_t mode);

    void write(int elem);

    SharedMemory &clear();

    void lock();

    void unlock();

    void wait();

    int read();

};


#endif //UNTITLED_SHAREDMEMORY_H
