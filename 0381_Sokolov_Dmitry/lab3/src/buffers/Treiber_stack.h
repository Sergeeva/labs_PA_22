#ifndef LAB3_TREIBER_STACK_H
#define LAB3_TREIBER_STACK_H

#include "./Node.h"


template <typename T>
class TRStack {


public:

    std::shared_ptr<Node<T>> head = nullptr;

    TRStack() = default;

    void push(T const& data) {

        auto h = this->head;

        std::shared_ptr<Node<T>> insert = std::make_shared<Node<T>>(Node<T>(data));

        do {
            insert->next = h;
        } while (!(std::atomic_compare_exchange_weak(&this->head, &h, insert)));
    }

    T pop() {
        auto last = head;

        while(!(last && std::atomic_compare_exchange_weak(&this->head, &last, last->next))) {
            last = head;
        }

        return last->get_data();
    }
};

#endif //LAB3_TREIBER_STACK_H
