#include "../Headers/Buffer.h"


Node::Node() : matrix(nullptr){}

Node::Node(std::shared_ptr<Matrix> matrix) : matrix(matrix){}


Buffer::Buffer(){
    this->head_ = nullptr;
}

void Buffer::produce(Matrix matrix){
    while(true){
        std::shared_ptr<Node> node = this->head_;
        std::shared_ptr<Node> newHead = std::make_shared<Node>(Node(std::make_shared<Matrix>(matrix)));
        newHead->next = node;
        if (std::atomic_compare_exchange_weak(&this->head_, &node, newHead))
            break;
    }
}

std::shared_ptr<Matrix> Buffer::consume(){
    while(true){
        std::shared_ptr<Node> node = this->head_;
        if (node) if (std::atomic_compare_exchange_weak(&this->head_, &node, node->next))
            return node->matrix;
    }
}