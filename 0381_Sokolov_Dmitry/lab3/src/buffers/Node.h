#ifndef LAB3_NODE_H
#define LAB3_NODE_H

#include <memory>


template<typename T>
struct Node {
    const T data;
    std::shared_ptr<Node<T>> next;

    T get_data() {
        return data;
    }

    Node() = default;

    explicit Node(T const& _data): data(_data), next(std::make_shared<Node<T>>()) {};

};


#endif //LAB3_NODE_H
