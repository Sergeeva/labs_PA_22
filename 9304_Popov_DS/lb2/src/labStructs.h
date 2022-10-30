#pragma once

#include <queue>
#include <vector>
#include <random>
#include <cstdint>
#include <condition_variable>
#include <atomic>



namespace labStruct
{

using uint = uint32_t;

class Matrix 
{
public:
    explicit Matrix(const uint& row, const uint& column)
    {
        const uint size =  row * column;
        firstMatrix.resize(size);
        secondMatrix.resize(size);
    }

    std::vector<uint> firstMatrix;
    std::vector<uint> secondMatrix;
};


template <class T>
class CustomQueue 
{
public:
    void push(T element)
    {
        std::lock_guard<std::mutex> ul(queueMutex);
        customQueue.push(std::move(element));
        queueCount.fetch_add(1);
        queueBell.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> ul(queueMutex);

        queueBell.wait(ul, [this](){ return queueCount.load() > 0; });

        queueCount.fetch_add(-1);

        auto fronElement = customQueue.front();
        customQueue.pop();

        return fronElement;
    }

private:
    std::condition_variable queueBell;
    std::atomic<uint> queueCount {0};
    std::queue<T> customQueue;
    std::mutex queueMutex;
};


namespace random
{

std::random_device device;
std::mt19937 randomGenerator;
std::uniform_int_distribution<uint> range(0, 49);

uint getRandomNumber()
{
    return range(randomGenerator);
}

}

}