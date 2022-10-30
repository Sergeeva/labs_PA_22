#ifndef LAB1_PA_LOCKFREEQUEUE_H
#define LAB1_PA_LOCKFREEQUEUE_H


template<typename T>
class LockFreeQueue {

private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node *> next;

        Node() : data({}),
                 next(nullptr) {}

        explicit Node(T elem) : data(std::make_shared<T>(elem)),
                       next(nullptr) {}
    };

    std::atomic<Node *> head;
    std::atomic<Node *> tail;

public:
    LockFreeQueue() {
        std::atomic<Node *> dummy = new Node();
        head = dummy.load();
        tail = dummy.load();
    }

    void produce(T element) {
        auto newTail = new Node(element);

        while (true) {
            auto current_tail = tail.load();
            Node *null = nullptr;

            if (current_tail->next.compare_exchange_weak(null, newTail)) {
                tail.compare_exchange_weak(current_tail, newTail);
                break;
            } else {
                tail.compare_exchange_weak(current_tail, current_tail->next);
            }
        }
    }

    std::shared_ptr<T> consume() {
        while (true) {
            auto current_head = head.load();
            auto current_tail = tail.load();
            auto next_head = current_head->next.load();

            if (current_head == current_tail && next_head) {
                tail.compare_exchange_strong(current_tail, next_head);
            } else if (next_head) {
                auto result = next_head->data;
                if (head.compare_exchange_strong(current_head, next_head)) {
                    return result;
                }
            }
        }
    }
};


#endif //LAB1_PA_LOCKFREEQUEUE_H
