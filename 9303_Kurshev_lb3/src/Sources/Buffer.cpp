#include "../Headers/Buffer.h"

Buffer::Buffer(){
    head = nullptr;
}

void Buffer::push(Matrix& matrix){
    std::shared_ptr<Node> new_node = std::make_shared<Node>(matrix);
    std::shared_ptr<Node> first_element;
    while(!std::atomic_compare_exchange_weak(&head, &first_element, new_node)){
        first_element = head;
        new_node->next = first_element;
    }
}

Matrix Buffer::pop(){
    std::shared_ptr<Node> first_element;
    while(!first_element || !std::atomic_compare_exchange_weak(&head, &first_element, first_element->next))
        first_element = head;

    return first_element->get_data();
}

Node::Node(Matrix data): node_data(data) {}

Matrix Node::get_data() {
    return node_data;
}
