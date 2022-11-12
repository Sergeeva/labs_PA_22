#pragma once

#include <atomic>
#include <iostream>
#include <memory>

// template <typename T>
// class node_ptr;

// template <typename T>
// struct node {
//     T data;
//     node* next;
//     std::atomic<int> ref_counter{0};

//     node(T data) : data(std::move(data)) {}
// };

// template <typename T>
// class node_ptr {
//     std::atomic<node<T>*> _node{nullptr};

// public:
//     node_ptr() = default;
//     node_ptr(T data) : _node(new node<T>(std::move(data))) {
//         ++_node.load()->ref_counter;
//     }
//     node_ptr(const node_ptr& other) : _node(other._node.load()) {
//         if (_node.load()) ++_node.load()->ref_counter;
//     }
//     node_ptr(node_ptr&& other) : _node(std::move(other._node.load())) {
//         other._node.exchange(nullptr);
//     }

//     node_ptr<T>& operator=(const node_ptr& other) {
//         decrement_ref_counter();
//         _node.store(other._node.load());
//         return *this;
//     }
//     node_ptr<T>& operator=(node_ptr&& other) {
//         decrement_ref_counter();
//         _node.store(other._node.load());
//         other.decrement_ref_counter();
//         return *this;
//     }

//     void decrement_ref_counter() {
//         if (_node.load() && _node.load()->ref_counter.fetch_sub(1) == 0) {
//             delete _node.load();
//             _node.store(nullptr);
//         }
//     }

//     ~node_ptr() {
//         decrement_ref_counter();
//     }

//     int use_count() const {
//         return _node.load() ? _node.load()->ref_counter.load() : 0;
//     }

//     bool has_value() const {
//         return _node.load();
//     }

//     bool cas(node_ptr& expected, const node_ptr& actual) {
//         return _node.compare_exchange_weak(expected._node.load(), actual._node.load());
//     }

//     bool operator==(const node_ptr& other) const {
//         return _node.load() == other._node.load();
//     }

//     node<T>& operator*() {
//         return *_node.load();
//     }

//     const node<T>& operator*() const {
//         return *_node.load();
//     }

//     node<T>* operator->() {
//         return _node.load();
//     }

//     node<T>* const operator->() const {
//         return _node.load();
//     }
// };

template <typename T>
struct node {
    node() = default;
    node(T value) : value(std::move(value)) {}

    T value{};
    std::shared_ptr<node> next = nullptr;
};

template <typename T>
class lock_free_queue {
public:
    lock_free_queue() {
        head = std::make_shared<node<T>>();
        tail = head;
    }

    void push(T value) {
        auto new_node = std::make_shared<node<T>>(std::move(value));
        auto last = head;

        while (!std::atomic_compare_exchange_weak(&tail, &last, new_node));

        last->next = new_node;
        ++_size;
    }

    T front() const {
        auto first = head->next;
        auto head_next = head->next;
        while (!head_next || !std::atomic_compare_exchange_weak(&head_next, &first, first)) {
            head_next = head->next;
        }
        return first->value;
    }

    void pop() {
        auto dummy_next = head->next;
        std::shared_ptr<node<T>> sub = nullptr;

        // активно ожидаем
        while (std::atomic_compare_exchange_weak(&dummy_next, &sub, head->next));

        // здесь dummy_next указывает на первый настоящий элемент, и при этом он не nullptr
        auto prev_last_node = dummy_next->next;
        std::atomic_store(&head->next, prev_last_node);
        // если prev_last_node == nullptr, то tail нужно сдвинуть на head
        std::atomic_compare_exchange_weak(&tail, &dummy_next, head);

        --_size;
    }

    auto size() const { return _size.load(); }

    void print() const {
        auto current = head->next;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << "\n";
    }

    std::shared_ptr<node<T>> head;
    std::shared_ptr<node<T>> tail;
    std::atomic<int> _size{0};
};
