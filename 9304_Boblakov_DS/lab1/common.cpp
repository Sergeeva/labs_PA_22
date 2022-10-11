#include "common.h"
//
//std::random_device rd;
//std::mt19937 gen(rd());
//std::uniform_int_distribution<> distr(-1000, 1000);




int getSharedMemoryId() {
    const int key = 1234;
    const int size = 1024;

    int sharedMemoryId = shmget(key, size, 0644 | IPC_CREAT);

    if (sharedMemoryId == 1) {
        std::cout << "Shared memory allocation error!";
        exit(EXIT_FAILURE);
    }

    return sharedMemoryId;
}