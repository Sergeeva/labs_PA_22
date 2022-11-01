#include "SafeQueue.cpp"
#include <thread>
using namespace std;

void generate(SafeQueue<MATRIX::Matrix<int>> &_queue, int count, int n, int m)
{

    for (size_t i = 0; i < count; i++)
    {
        MATRIX::Matrix<int> matrix = MATRIX::generate(n, m);
        _queue.push(matrix, "FILL");
    }
}

void mat_sum(
    SafeQueue<MATRIX::Matrix<int>> &first_operands_queue,
    SafeQueue<MATRIX::Matrix<int>> &second_operands_queue,
    SafeQueue<MATRIX::Matrix<int>> &results_queue,
    int count)
{
    for (size_t i = 0; i < count; i++)
    {
        auto first_operand = first_operands_queue.pop("SUM");
        auto second_operand = second_operands_queue.pop("SUM");
        auto result = first_operand + second_operand;

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

    SafeQueue<MATRIX::Matrix<int>> first_operands_queue("A_operands", 10), second_operands_queue("B_operands", 10), results_queue("results", 10);

    int count = 20, n = 10, m = 10;

    std::vector<thread> threads;
    threads.push_back(thread(generate, std::ref(first_operands_queue), count, n, m));
    threads.push_back(thread(generate, std::ref(second_operands_queue), count, n, m));
    threads.push_back(thread(mat_sum, std::ref(first_operands_queue), std::ref(second_operands_queue), std::ref(results_queue), count));
    threads.push_back(thread(write, std::ref(results_queue), count));

    for (auto &_thread : threads)
    {
        _thread.join();
    }

    return 0;
}