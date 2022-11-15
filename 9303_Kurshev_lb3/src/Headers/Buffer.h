#ifndef LAB2_BUFFER_H
#define LAB2_BUFFER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "./Matrix.h"

class Node{
    private:
        const Matrix node_data;
    public:
        std::shared_ptr<Node> next = nullptr;
        Node(Matrix matrix_data);
        ~Node() = default;
        Matrix get_data();
};

class Buffer{
    private:
        std::shared_ptr<Node> head;
    public:
        Buffer();
        ~Buffer() = default;
        Matrix pop();
        void push(Matrix& matrix);
};





#endif
