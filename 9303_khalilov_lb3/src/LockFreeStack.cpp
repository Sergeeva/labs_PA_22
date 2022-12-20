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
    shared_ptr<node> fake = nullptr;

    int worker_comunt = 0;

public:
    void
    increment()
    {
        ++worker_comunt;
    }
    void decrement()
    {
        --worker_comunt;
    }

    LockFreeStack()
    {
        head = fake;
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
        cout << "pop"
             << "\n";

        while (!old_head || !atomic_compare_exchange_weak(&head, &old_head, old_head->next))
        {
            old_head = head;
            if (worker_comunt <= 1 && !head)
            {
                return shared_ptr<T>();
            }
        };
        return old_head->data;
    }

    ~LockFreeStack()
    {
        while (pop())
            ;
    }
};
