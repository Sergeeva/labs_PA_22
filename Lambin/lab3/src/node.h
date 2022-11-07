#ifndef NODE_H
#define NODE_H

#include "definition.h"

template<typename T>
class Node {
public:
    Node(T);
    ~Node() = default;
    T data() const;
    std::shared_ptr<Node> next;

private:
    std::shared_ptr<T> _data;

};

template<typename T>
Node<T>::Node(T data) {
    _data = std::make_shared<T>(data);
}

template<typename T>
T Node<T>::data() const {
    return *_data;
}

#endif //NODE_H
