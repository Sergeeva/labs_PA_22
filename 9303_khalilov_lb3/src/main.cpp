#include "LockFreeStack.cpp"
#include <chrono>
using namespace std;
using namespace std::chrono;

void generate(LockFreeStack<pair<MATRIX::Matrix<int>, MATRIX::Matrix<int>>> &_queue, int count, int n, int m)
{

    cout << "gen\n";
    _queue.increment();
    for (size_t i = 0; i < count; i++)
    {
        cout << "GEN[" + to_string(i + 1) + "] ------- \n";
        pair<MATRIX::Matrix<int>, MATRIX::Matrix<int>> pair_mat = make_pair(MATRIX::generate(n, m), MATRIX::generate(n, m));
        _queue.push(move(pair_mat));
    }
}

void mat_sum(
    LockFreeStack<pair<MATRIX::Matrix<int>, MATRIX::Matrix<int>>> &operands_queue,
    LockFreeStack<MATRIX::Matrix<int>> &results_queue,
    int count)
{
    cout << "sum\n";
    operands_queue.increment();
    results_queue.increment();

    for (size_t i = 0; i < count; i++)
    {
        cout << "SUM[" + to_string(i + 1) + "] ------- \n";
        auto operands = operands_queue.pop();
        if (operands)
        {
            auto result = operands->first + operands->second;
            results_queue.push(move(result));
        }
    }

    operands_queue.decrement();
    results_queue.decrement();
}

void write(LockFreeStack<MATRIX::Matrix<int>> &results_queue, int count)
{
    cout << "write\n";
    results_queue.increment();
    for (size_t i = 0; i < count; i++)
    {
        cout << "WRITE[" + to_string(i + 1) + "] ------- \n";
        auto result = results_queue.pop();
        if (result)
        {
            MATRIX::write(*result, "results/result__" + to_string(i + 1) + "__.txt");
        }
    }
    results_queue.decrement();
}

int main(int argc, const char **argv)
{

    LockFreeStack<MATRIX::Matrix<int>> results_queue;
    LockFreeStack<pair<MATRIX::Matrix<int>, MATRIX::Matrix<int>>> operands_queue;

    int count = 1, n = 10, m = 10;

    vector<thread> threads;

    threads.push_back(thread(generate, ref(operands_queue), count, n, m));

    auto start = high_resolution_clock::now();

    threads.push_back(thread(mat_sum, ref(operands_queue), ref(results_queue), count));

    auto stop = high_resolution_clock::now();

    threads.push_back(thread(write, ref(results_queue), count));

    for (auto &_thread : threads)
    {
        _thread.join();
    }

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;
}