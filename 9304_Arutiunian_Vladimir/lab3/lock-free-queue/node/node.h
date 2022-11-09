#ifndef LABS_PA_22_LAB3_NODE_H
#define LABS_PA_22_LAB3_NODE_H

#include <memory>

template<class T>
class Node {
public:
    T value;
    std::atomic<Node<T>*> next = nullptr;

    Node() = default;

    explicit Node(const T& value_);
};

#include "node.tpp"

#endif //LABS_PA_22_LAB3_NODE_H
