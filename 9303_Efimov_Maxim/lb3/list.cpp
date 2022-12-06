#include "list.h"


Node::Node(){matrix = nullptr;}

Node::Node(std::shared_ptr<Matrix> matrix){this->matrix = matrix;}


List::List(){
    this->head = nullptr;
}


void List::produce(Matrix matrix){
    std::shared_ptr<Node> node = this->head;
    std::shared_ptr<Node> newHead = std::make_shared<Node>(Node(std::make_shared<Matrix>(matrix)));
    do {
        newHead->next = node;
    } while (!std::atomic_compare_exchange_strong(&this->head, &node, newHead));

}

std::shared_ptr<Matrix> List::consume(){
    std::shared_ptr<Node> node = this->head;
    while (!(node && std::atomic_compare_exchange_strong(&this->head, &node, node->next)))
        node = this->head;
    return node->matrix;
} 
