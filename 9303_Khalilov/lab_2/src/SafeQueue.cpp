#include <queue>
#include <mutex>
#include <condition_variable>
#include "./utils.cpp"
#include <thread>

template <typename T>
class SafeQueue
{
private:
    int max_size;
    int current_index;
    mutex __mutex;
    queue<T> __queue;
    condition_variable hes_producer, hes_consumer;
    string name;

public:
    SafeQueue(string name, int max_size = 10) : name(name), max_size(max_size)
    {
    }


    void push(T &value, string opr = "")
    {

        this->showState("PUSH");
        unique_lock<mutex> lock(this->__mutex);

        this->hes_producer.wait(lock, [&]()
                          { return this->isNotFull(); });
        lock.unlock();
        this->__queue.push(value);
        this->hes_consumer.notify_one();

    }

    void showState(string opr)
    {
        cout << "[" + opr + "][" + to_string(this->__queue.size()) + "/" + to_string(max_size) + "] -- (" + this->name + ") -- \n";
    }

    T pop(string opr = "")
    {

        this->showState("POP ");
        unique_lock<mutex> lock(this->__mutex);
        this->hes_consumer.wait(lock, [&]()
                          { return !this->isEmpty(); });

        auto first_item = this->__queue.front();
        this->__queue.pop();
        lock.unlock();
        this->hes_producer.notify_one();
        return first_item;
    }

    bool isNotFull() 
    {
        return this->__queue.size() < this->max_size;
    }

    bool isEmpty()
    {
        return this->__queue.empty();
    }
};