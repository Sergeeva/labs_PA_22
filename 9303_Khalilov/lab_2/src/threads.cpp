#include "SafeQueue.cpp"
#include <thread>
using namespace std;

void generate( SafeQueue<std::pair<MATRIX::Matrix<int>, MATRIX::Matrix<int>>> &_queue, int count, int n, int m)
{

    for (size_t i = 0; i < count; i++)
    {
        auto pair_mat = make_pair(MATRIX::generate(n, m), MATRIX::generate(n, m));
        _queue.push(pair_mat, "FILL");
    }
}

void mat_sum(
    SafeQueue<std::pair<MATRIX::Matrix<int>, MATRIX::Matrix<int>>> &operands_queue,
    SafeQueue<MATRIX::Matrix<int>> &results_queue,
    int count)
{
    for (size_t i = 0; i < count; i++)
    {
        auto operand = operands_queue.pop("SUM");
        auto result = operand.first + operand.second;
        results_queue.push(result, "SUM");
    }
}

void write(SafeQueue<MATRIX::Matrix<int>> &results_queue, int count)
{
    for (size_t i = 0; i < count; i++)
    {
        cout << "WRITE[" + to_string(i + 1) + "] ------- \n";
        auto result = results_queue.pop();
        MATRIX::write(result, "results/result__" + to_string(i + 1) + "__.txt");
    }
}

int main(int argc, const char **argv)
{

    SafeQueue<std::pair<MATRIX::Matrix<int>, MATRIX::Matrix<int>>> operands_queue("operands", 10); 
    SafeQueue<MATRIX::Matrix<int>> results_queue("results", 10);

    int count = 20, n = 10, m = 10;

    std::vector<thread> threads;
    threads.push_back(thread(generate, std::ref(operands_queue), count, n, m));
    threads.push_back(thread(mat_sum, std::ref(operands_queue), std::ref(results_queue), count));
    threads.push_back(thread(write, std::ref(results_queue), count));

    for (auto &_thread : threads)
    {
        _thread.join();
    }

    return 0;
}