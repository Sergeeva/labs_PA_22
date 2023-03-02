#pragma once

#include <queue>
#include <condition_variable>
#include <atomic>
#include <mutex>

#include "node.h"


class Queue {
public:
	Queue() {
		head = new Node;
		tail.store(head);
	}

	std::atomic<Node*> head{};
	std::atomic<Node*> tail{};
    std::mutex mutex;
    std::atomic<int> count = 0;
};