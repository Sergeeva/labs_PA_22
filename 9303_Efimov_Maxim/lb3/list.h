 
#ifndef LIST_H
#define LIST_H

#include <memory>
#include "matrix.h"


class Node{
public:
    std::shared_ptr<Matrix> matrix;
    std::shared_ptr<Node> next;
    Node();
    Node(std::shared_ptr<Matrix> matrix);
};

class List{
public:
    List();
    void produce(Matrix matrix);
    std::shared_ptr<Matrix> consume();
private:
    std::shared_ptr<Node> head;
};

#endif