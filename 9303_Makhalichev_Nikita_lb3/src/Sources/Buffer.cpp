#include "../Headers/Buffer.h"


Node::Node() : matrix(nullptr){}

Node::Node(std::shared_ptr<Matrix> matrix) : matrix(matrix){}


Buffer::Buffer(){
    this->head_ = nullptr;
}

void Buffer::produce(Matrix matrix){
    std::shared_ptr<Node> node = this->head_;
    std::shared_ptr<Node> newHead = std::make_shared<Node>(Node(std::make_shared<Matrix>(matrix)));
    do {
        newHead->next = node;
    } while (!std::atomic_compare_exchange_weak(&this->head_, &node, newHead));

}

std::shared_ptr<Matrix> Buffer::consume(){
    std::shared_ptr<Node> node = this->head_;
    while (!(node && std::atomic_compare_exchange_weak(&this->head_, &node, node->next)))
        node = this->head_;
    return node->matrix;
}