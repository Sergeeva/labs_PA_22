#ifndef UNTITLED_SYNCHRONIZEDQUEUE_H
#define UNTITLED_SYNCHRONIZEDQUEUE_H

#include <queue>
#include <mutex>
#include <atomic>
#include <memory>
#include <optional>
#include <condition_variable>
#include "Log.h"


template<typename T>
class SynchronizedQueue {

private:

    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node *> next;

        explicit Node(T const &data) : data(std::make_shared<T>(data)), next(nullptr) {}

        Node() : data(std::shared_ptr<T>()), next(nullptr) {}
    };

    std::atomic<Node *> tail = new Node();
    std::atomic<Node *> head;
    std::atomic<bool> is_closed;

public:

    void push(T const &data) {
        Log::info("Pushing element to the queue");

        auto new_node = new Node(data);
        auto old_tail = tail.load();

        Node *null = nullptr;

        while (!old_tail->next.compare_exchange_weak(null, new_node)) {
            Log::info("Busy waiting to push");
            old_tail = tail.load();
        }

        if (head.compare_exchange_weak(null, new_node)) {
            Log::info("Replaced null head to the tail");
        }

        if (tail.compare_exchange_weak(old_tail, new_node)) {
            Log::info("Tail updated");
        }
    }

    std::shared_ptr<T> pop() {
        Log::info("Pop element");
        auto old_head = head.load();

        while (!is_closed && (!old_head || !head.compare_exchange_weak(old_head, old_head->next))) {
            old_head = head.load();
        }

        Log::info("Got an element");

        return old_head ? old_head->data : std::shared_ptr<T>{};
    }

    void close() {
        is_closed = true;
    }

};

//template<typename T>
//class SynchronizedQueue {
//
//private:
//
//    std::queue<std::shared_ptr<T>> queue;
//    std::mutex mutex;
//    std::atomic<bool> is_closed;
//    std::condition_variable has_elements;
//
//public:
//
//    std::shared_ptr<T> pop() {
//        Log::info("Attempt to acquire lock");
//
//        std::unique_lock<std::mutex> lock(mutex);
//
//        Log::info("Start waiting for objects in queue");
//
//        has_elements.wait(lock, [this]() {
//            return is_closed || queue.size() != 0;
//        });
//
//        Log::info("Done waiting for objects in queue");
//
//        if (queue.size() == 0) {
//            Log::info("Found closed queue");
//            return std::shared_ptr<T>{};
//        } else {
//            auto elem = queue.front();
//            queue.pop();
//
//            has_elements.notify_one();
//
//            Log::info("Element was removed from the queue");
//
//            return elem;
//        }
//    }
//
//    void push(T const &elem) {
//        Log::info("Acquiring lock to push");
//
//        std::scoped_lock<std::mutex> lock(mutex);
//
//        Log::info("Acquired lock to push");
//
//        queue.push(std::make_shared<T>(elem));
//
//        Log::info("Element was successfully pushed to the queue");
//
//        has_elements.notify_one();
//    }
//
//    void close() {
//        is_closed = true;
//        has_elements.notify_all();
//    }
//
//};

#endif
