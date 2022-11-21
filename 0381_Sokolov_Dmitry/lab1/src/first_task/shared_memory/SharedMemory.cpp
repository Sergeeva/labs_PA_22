#include "SharedMemory.h"

SharedMemory::SharedMemory() : size(MemoryConfig::memory_size), key(MemoryConfig::memory_key), mode(MemoryConfig::flag)
{
    this->make_shared();
}

SharedMemory::SharedMemory(int _size, int _key, int _mode) : size(_size), key(_key), mode(_mode) {
    this->make_shared();
}

void SharedMemory::make_shared() {
    auto id = shmget(this->key, this->size, this->mode);

    if (id == -1) {
        throw std::runtime_error("Couldn't allocate shared memory");
    }

    this->block = shmat(id, nullptr, 0);

}

int *SharedMemory::get_shared() {
    return (int *) (this->block);
}

int *SharedMemory::write_to_shared(const Matrix &matrix, int *ptr) {

    if (ptr == nullptr) {
        ptr = this->get_shared();
    }

    int rows = matrix.get_rows();
    int columns = matrix.get_columns();

    *ptr = rows;
    ++ptr;

    *ptr = columns;
    ++ptr;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            *ptr = matrix.get(i, j);
            ++ptr;
        }
    }

//    std::cout << "finished writing!" << std::endl;

    return ptr;
}

std::pair<Matrix, int *> SharedMemory::read_from_shared(int *ptr) {

    if (ptr == nullptr) {
        ptr = this->get_shared();
    }

    int rows = *ptr;
    ptr++;

    int columns = *ptr;
    ptr++;

    auto result = Matrix(rows, columns);

    for (int i = 0; i < result.get_rows(); ++i) {
        for (int j = 0; j < result.get_columns(); ++j) {

            result[i][j] = *ptr;

            ++ptr;
        }
    }

//    std::cout << "finished reading!" << std::endl;

    return std::make_pair(result, ptr);
}
