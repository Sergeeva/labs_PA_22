#ifndef LAB3_TREIBER_STACK_H
#define LAB3_TREIBER_STACK_H

#include "./Node.h"


template <typename T>
class TRStack {

    std::shared_ptr<Node<T>> head = nullptr;

    std::atomic<int> size = 0;

public:

    int get_size() {
        return size;
    }

    TRStack() = default;

    void push(T const& data) {

        auto h = this->head;

        std::shared_ptr<Node<T>> insert = std::make_shared<Node<T>>(Node<T>(data));

        do {
            insert->next = h;
        } while (!(std::atomic_compare_exchange_weak(&this->head, &h, insert)));
        size.fetch_add(1);
    }

    T pop() {
        auto last = head;

        while(!(last && std::atomic_compare_exchange_weak(&this->head, &last, last->next))) {
            last = head;
        }

        size.fetch_sub(1);
        return last->get_data();
    }
};

#endif //LAB3_TREIBER_STACK_H
