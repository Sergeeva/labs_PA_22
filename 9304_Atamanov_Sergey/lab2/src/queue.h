#pragma once

#include <queue>
#include <condition_variable>
#include <atomic>
#include <mutex>

template <typename T>
class Queue {
public:

    explicit Queue(){}

    void push(T val){
        std::lock_guard<std::mutex> lk(queueMutex);
        queue.push(std::move(val));
        cvQueue.notify_all();
    }
    std::shared_ptr<T> pop(){
        std::unique_lock<std::mutex> lk(queueMutex);
        cvQueue.wait(lk, [this] {return !queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(std::move(queue.front())));
        queue.pop();
        return res;
    }

	std::condition_variable cvQueue;
	std::atomic<int> count = 0;
	std::queue<T> queue;
	std::mutex queueMutex;
    std::mutex mutex;
};