//
// Created by Елизавета Москаленко on 11.10.2022.
//

#include "Buffer.h"
using namespace std;

void Buffer::produce(MatrixPair matrixs) {
    std::unique_lock<std::mutex> uniqueLock(mtx);
    not_full.wait(uniqueLock, [this]() {
        return this->buffer.size() < BUFFER_CAPACITY;
    });
    buffer.emplace_back(matrixs);
    uniqueLock.unlock();
    // буффер не пустой. один поток может обработать
    not_empty.notify_one();
}

MatrixPair Buffer::consume() {
    std::unique_lock<std::mutex> uniqueLock(mtx);
    // ждем, если буффер пустой
    not_empty.wait(uniqueLock, [this]() {
        return buffer.size() > 0;
    });
    MatrixPair matrixs = buffer.front();
    buffer.erase(buffer.begin());
    uniqueLock.unlock();
    not_full.notify_one();
    return matrixs;
}