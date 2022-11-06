#include <iostream>

template<class T>
LockFreeQueue<T>::LockFreeQueue(std::string_view name): name(name) {
    auto fake_node = new Node<T>();
    head = fake_node;
    tail = fake_node;
}

template<class T>
LockFreeQueue<T>::~LockFreeQueue() {
    delete head;
}

template<class T>
void LockFreeQueue<T>::push(const T& value) {
    auto new_node = new Node(value);
    bool is_new_node_not_added = true;

    while (is_new_node_not_added) {
        Node<T>* local_tail = tail.load();
        Node<T>* local_tail_next = local_tail->next.load();

        if (local_tail_next == nullptr) {
            if (local_tail->next.compare_exchange_weak(local_tail_next, new_node)) {
                tail.compare_exchange_strong(local_tail, new_node);
                is_new_node_not_added = false;
            }
        } else {
            tail.compare_exchange_weak(local_tail, local_tail_next);
        }
    }
}

template<class T>
T LockFreeQueue<T>::pop() {
    T returning_value;
    bool is_node_not_got = true;

    while (is_node_not_got) {
        auto local_head = head.load();
        auto local_head_next = local_head->next.load();
        auto local_tail = tail.load();

        if (local_head_next != nullptr) {
            if (local_head == local_tail) {
                tail.compare_exchange_strong(local_tail, local_head_next);
            } else {
                returning_value = local_head_next->value;
                if (head.compare_exchange_weak(local_head, local_head_next)) {
                    is_node_not_got = false;
                }
            }
        }
    }

    return returning_value;
}
