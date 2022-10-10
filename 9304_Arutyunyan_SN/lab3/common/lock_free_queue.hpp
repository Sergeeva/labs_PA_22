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
    node(T value) : value(std::move(value)) {}

    T value;
    std::shared_ptr<node> next = nullptr;
};

template <typename T>
class lock_free_queue {
public:
    void push(T value) {
        auto new_node = std::make_shared<node<T>>(std::move(value));

        // если можно сделать set без лишних переменных, то выглядеть будет красивее
        std::shared_ptr<node<T>> first = nullptr;
        std::shared_ptr<node<T>> last = nullptr;
        // а) если список пустой, то head нужно установить на node б) если список непустой, то head не трогаем
        std::atomic_compare_exchange_weak(&head, &first, new_node);

        // переводим указатель на хвост в актуальное состояние
        while (!std::atomic_compare_exchange_weak(&tail, &last, new_node));
        // на этом этапе last - предыдущий от последнего, node - последний для текущего треда
        if (last) last->next = new_node;
        ++_size;
    }

    T front() const {
        auto first = head;
        while (!head || std::atomic_exchange(&first, head) != first) {}
        return first->value;
    }

    void pop() {
        auto first = head;
        auto head_next = (first == nullptr) ? nullptr : first->next;
        // если head == first, то переводим head на следующий элемент. иначе first указывает на актуальную версию head
        while (!head || !std::atomic_compare_exchange_weak(&head, &first, head_next)) {
            head_next = (first == nullptr) ? nullptr : first->next;
        }

        --_size;
    }

    auto size() const { return _size.load(); }

    void print() const {
        auto current = head;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << "\n";
    }

    std::shared_ptr<node<T>> head = nullptr;
    std::shared_ptr<node<T>> tail = nullptr;
    std::atomic<int> _size{0};
};
