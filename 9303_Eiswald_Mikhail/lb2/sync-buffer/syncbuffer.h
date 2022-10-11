#ifndef SYNCBUFFER_H
#define SYNCBUFFER_H

#include <condition_variable>
#include <mutex>

#include "../matrix/matrix.h"

using namespace std;

class SyncBuffer {
    mutex buffer_mutex;
    size_t size, read_index, write_index;
    Matrix* data;
    bool empty;
    condition_variable able_to_read, able_to_write;
public:
    SyncBuffer(size_t size = 10);
    void push(const Matrix& matrix);
    Matrix pop();
};

#endif // SYNCBUFFER_H
