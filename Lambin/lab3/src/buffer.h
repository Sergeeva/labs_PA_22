#ifndef BUFFER_H
#define BUFFER_H

#include "node.h"

template<typename T>
class Buffer {
public:
    Buffer();
    ~Buffer() = default;
    void push(T);
    T pop();

private:
    std::shared_ptr<Node<T>> top;

};

template<typename T>
Buffer<T>::Buffer() {
    top = nullptr;
}

template<typename T>
void Buffer<T>::push(T data) {
    std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(data);
    std::shared_ptr<Node<T>> curNode = top;
    newNode->next = top;

    while (!std::atomic_compare_exchange_weak(&top, &curNode, newNode)) {
        curNode = top;
        newNode->next = top;
    }
}

template<typename T>
T Buffer<T>::pop() {
    std::shared_ptr<Node<T>> oldNode = top;

    while (!oldNode || !std::atomic_compare_exchange_weak(&top, &oldNode, oldNode->next)) {
        oldNode = top;
    }

    return oldNode->data();
}

#endif //BUFFER_H
