#include "../headers/SimpleMatricesMultiplier.h"
#include "./MatrixHelper.cpp"


void SimpleMatricesMultiplier::SimpleMultiplyTask::run() {
    for (int op = start_index; op < end_index; ++op) {
        auto height = target->get_height();
        auto width = target->get_width();

        auto row = op % height;
        auto col = op / width;
        auto result = 0;

        for (int i = 0; i < std::min(height, width); ++i) {
            auto first = holder->get_first().get(row, i);
            auto second = holder->get_second().get(i, col);
            result += first * second;
        }

        target->set(row, col, result);
    }

    (*processed_tasks)++;

    if (processed_tasks->load() >= total_tasks) {
        promise->set_value(target);
    }
}

std::promise<Matrix *> *
SimpleMatricesMultiplier::multiply(MatricesHolder *matrices, SynchronizedQueue<Runnable> *task_consumer) {
    auto height = matrices->get_first().get_height();
    auto width = matrices->get_first().get_width();

    auto n_elements = height * width;
    auto n_operations = n_elements / thread_count;
    auto rest_operations = n_elements % thread_count;
    auto promise = new std::promise<Matrix *>();
    auto result = new Matrix(height, width);
    auto processed_tasks = new std::atomic<int>(0);

    for (auto i = 0; i < thread_count; i++) {
        int start_index;
        int end_index;

        if (i == 0) {
            start_index = n_operations * i;
            end_index = (n_operations * (i + 1)) + rest_operations;
        } else {
            start_index = n_operations * i + rest_operations;
            end_index = (n_operations * (i + 1)) + rest_operations;
        }

        task_consumer->push(new SimpleMultiplyTask(start_index, end_index, result, matrices, processed_tasks, promise,
                                                   thread_count));
    }

    return promise;
}
