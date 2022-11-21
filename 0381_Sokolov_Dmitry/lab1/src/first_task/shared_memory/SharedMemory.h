#ifndef PROJECT_SHAREDMEMORY_H
#define PROJECT_SHAREDMEMORY_H

#include <utility>

#include "../../configuration/config.h"
#include "../../matrices/Matrix.h"


class SharedMemory {
    const int size = MemoryConfig::memory_size;
    const int key = MemoryConfig::memory_key;
    const int mode = MemoryConfig::flag;

    void *block;

protected:
    void make_shared();

    int *get_shared();

public:
    SharedMemory();

    explicit SharedMemory(int _size, int key, int flag);

    int *write_to_shared(const Matrix &matrix, int *ptr = nullptr);

    std::pair<Matrix, int *> read_from_shared(int *ptr = nullptr);

};


#endif //PROJECT_SHAREDMEMORY_H
