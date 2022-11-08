#ifndef QUEUE_H
#define QUEUE_H

#include <atomic>

using namespace std;

#define cas(checked, expected, value) \
    checked.compare_exchange_weak(expected, value, \
    memory_order_relaxed, memory_order_relaxed)

template<typename T>
class Queue {
    size_t size;
    atomic<size_t> read_index, write_index;
    atomic<T*>* data;
public:
    Queue(size_t size = 10): size(size) {
        data = (atomic<T*>*) new atomic<T*>[size];
        for(int i = 0; i < size; ++i) {
            data[i].store(nullptr, memory_order_relaxed);
        }
        read_index.store(0, memory_order_relaxed);
        write_index.store(0, memory_order_relaxed);
    }

    void enq(T* v) {
        for(;;) {
            size_t l_index = write_index.load(memory_order_relaxed);
            if(data[l_index] == nullptr) {
                if(cas(write_index, l_index, (l_index + 1) % size)) {
                    data[l_index].store(v, memory_order_relaxed);
                    return;
                }
            }
        }
    }

    T* deq() {
        T* v_ptr;
        for(;;) {
            size_t l_index = read_index.load(memory_order_relaxed);
            if(data[l_index] != nullptr) {
                if(cas(read_index, l_index, (l_index + 1) % size)) {
                    v_ptr = data[l_index].load(memory_order_relaxed);
                    data[l_index].store(nullptr, memory_order_relaxed);
                    return v_ptr;
                }
            }
        }
    }
};

#endif
