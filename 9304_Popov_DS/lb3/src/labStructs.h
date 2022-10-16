#pragma once

#include <vector>
#include <random>
#include <cstdint>
#include <atomic>
#include <memory>

#include <iostream>


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


// template <class T>
class CustomQueue 
{
public:
    CustomQueue()
    {
        head = new Node;
        tail.store(head);
    }

    void push(Matrix* const newElement)
    {
        const std::atomic<Node*> newTail = new Node(newElement);
        Node* null = nullptr;

        while (true)
        {
            Node* oldTail = tail.load();

            if (oldTail->next.compare_exchange_weak(null, newTail))
            {
                tail.compare_exchange_weak(oldTail, newTail);
                return;
            }
            else
                tail.compare_exchange_weak(oldTail, oldTail->next);

        }   
    }

    Matrix* pop()
    {
        while (true)
        {
            Node* oldHead = head.load();
            Node* oldTail = tail.load();
            Node* nextHead = oldHead->next.load();

            if (oldHead == oldTail)
            {
                if (nextHead == nullptr)
                    throw std::out_of_range("Queue is empty!\n");
                else
                    tail.compare_exchange_weak(oldTail, nextHead);
            }
            else
            {
                Matrix* result = nextHead->_data;

                if (head.compare_exchange_weak(oldHead, nextHead))
                    return result;
            }
        }
    }

private:
    struct Node
    {
        Node(Matrix* const data = nullptr) : _data(data), next(nullptr) { }

        Matrix* _data;
        std::atomic<Node*> next;
    };
    std::atomic<Node*> head {nullptr};
    std::atomic<Node*> tail {nullptr};
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