#pragma once

#include <atomic>

#include "matrix.h"

struct Node {
	Node(Matrix* const data = nullptr) : next(nullptr), _data(data) {}

	Matrix* _data;
	std::atomic<Node*> next;

};