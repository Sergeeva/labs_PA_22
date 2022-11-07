#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include "./Matrix.h"


class Node{
public:
    std::shared_ptr<Matrix> matrix;
    std::shared_ptr<Node> next;
    Node();
    Node(std::shared_ptr<Matrix> matrix);
};

class Buffer{
public:
    Buffer();
    void produce(Matrix matrix);
    std::shared_ptr<Matrix> consume();
private:
    std::shared_ptr<Node> head_;
};

#endif