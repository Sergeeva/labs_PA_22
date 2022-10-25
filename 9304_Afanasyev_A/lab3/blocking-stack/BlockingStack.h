#pragma once

#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
struct Node {

    const T value;
    std::shared_ptr<Node<T>> next = nullptr;

    explicit Node(T value) : value(std::move(value)) {}

    Node() = delete;
};

template<typename T>
class BlockingStack {

    std::shared_ptr<Node<T>> head{};

public:
    T pop() {
        std::shared_ptr<Node<T>> first{};

        do {
            first = head;
        } while (!first || !std::atomic_compare_exchange_weak(&head, &first, first->next));

        return first->value;
    }

    void push(const T &value) {
        auto newNode = std::make_shared<Node<T>>(std::move(value));

        std::shared_ptr<Node<T>> first{};
        do {
            first = head;
            newNode->next = first;
        } while (!std::atomic_compare_exchange_weak(&head, &first, newNode));
    }
};
