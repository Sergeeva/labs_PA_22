#include "utils.cpp"

using namespace std;

template <typename T>
class LockFreeStack
{
private:
    struct node
    {
        shared_ptr<T> data;
        shared_ptr<node> next = nullptr;
        explicit node(T const &data_) : data(make_shared<T>(data_))
        {
        }
    };
    shared_ptr<node> head;

public:
    LockFreeStack()
    {
        head = nullptr;
    }

    void push(T const &data)
    {
        shared_ptr<node> new_node = make_shared<node>(node(data));
        shared_ptr<node> old_head;

        while (!atomic_compare_exchange_weak(&head, &old_head, new_node))
        {
            old_head = head;
            new_node->next = old_head;
        };
    }

    shared_ptr<T> pop()
    {
        shared_ptr<node> old_head;
        
        while (!old_head || !atomic_compare_exchange_weak(&head, &old_head, old_head->next))
        {
            old_head = head;
        };

        return old_head->data;
    }

    ~LockFreeStack()
    {
        while (pop())
            ;
    }
};
