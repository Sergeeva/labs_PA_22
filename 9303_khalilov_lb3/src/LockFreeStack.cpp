#include "utils.cpp"

using namespace std;

template <typename T>
class LockFreeStack
{
private:
    struct node
    {
        shared_ptr<T> data;
        shared_ptr<node> next;
        node(T const &data_) : data(make_shared<T>(data_))
        {
        }
    };
    shared_ptr<node> head;

public:
    void push(T const &data)
    {
        shared_ptr<node> const new_node = make_shared<node>(node(data));
        new_node->next = atomic_load(&head);

        while (!atomic_compare_exchange_weak(&head,
                                             &new_node->next, new_node))
            ;
    }

    shared_ptr<T> pop()
    {
        shared_ptr<node> old_head = atomic_load(&head);

        while (old_head && !atomic_compare_exchange_weak(
                               &head,
                               &old_head,
                               atomic_load(&old_head->next)))
            ;

        if (old_head)
        {
            atomic_store(&old_head->next, shared_ptr<node>());
            return old_head->data;
        }
        return shared_ptr<T>();
    }

    ~LockFreeStack()
    {
        while (pop())
            ;
    }
};
