#include <atomic>
#include <memory>

template <typename T>
struct node {
    T data;
    std::shared_ptr<node<T>> next;

    node(T data) : data(std::move(data)) { }
};

template <typename T>
class lock_free_stack {
public:
    std::shared_ptr<node<T>> head = nullptr;

    void push(T val) {
        auto current_head = head;
        auto new_node = std::make_shared<node<T>>(std::move(val));

        do {
            new_node->next = current_head;
        } while (!std::atomic_compare_exchange_weak(&head, &current_head, new_node));
    }

    T pop() {
        auto last = head;
        while (!last || !std::atomic_compare_exchange_weak(&head, &last, last->next)) {
            last = head;
        }

        return last->data;
    }

    void print() const {
        auto current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};
