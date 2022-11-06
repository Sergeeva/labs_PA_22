#include "main.h"

int main() {
    Logger::printCPlusPlusVersion();

    size_t iterations_count_in_each_thread = 30;
    size_t threads_count_for_adding = 8;
    size_t threads_count_for_consumers_and_producers = 8;

    LockFreeQueue<MatricesHolder> input_queue("INPUT");
    LockFreeQueue<Matrix*> output_queue("OUTPUT");

    std::vector<std::thread> threads;
    auto start = Time::now();

    for (size_t i = 0; i < threads_count_for_consumers_and_producers; ++i) {
        threads.emplace_back(
                enter_matrices_with_repetitions,
                iterations_count_in_each_thread,
                std::ref(input_queue)
        );
        threads.emplace_back(
                calculate_matrix_with_repetitions,
                iterations_count_in_each_thread,
                std::ref(input_queue),
                std::ref(output_queue),
                threads_count_for_adding
        );
        threads.emplace_back(
                print_matrix_with_repetitions,
                iterations_count_in_each_thread,
                std::ref(output_queue)
        );
    }

    for (auto& thread: threads) {
        thread.join();
    }

    auto elapsed_time = Time::get_elapsed_time_in_microseconds(start);
    Logger::printThreadIdWithText("Elapsed time: " + std::to_string(elapsed_time));
    return 0;
}

void enter_matrices_with_repetitions(size_t iterations_count, LockFreeQueue<MatricesHolder>& queue) {
    for (size_t i = 0; i < iterations_count; ++i) {
        enter_matrices(queue);
    }
}

void enter_matrices(LockFreeQueue<MatricesHolder>& queue) {
    std::ifstream input(filenames::kMatrixInputFilename);

    auto first_matrix = new Matrix();
    auto second_matrix = new Matrix();

    input >> *first_matrix >> *second_matrix;

    queue.push(std::make_pair(first_matrix, second_matrix));
    Logger::printThreadIdWithText("Pair of matrices pushed to the `INPUT` queue");
}

void calculate_matrix_with_repetitions(
        size_t iterations_count,
        LockFreeQueue<MatricesHolder>& input_queue,
        LockFreeQueue<Matrix*>& output_queue,
        size_t threads_count
) {
    for (size_t i = 0; i < iterations_count; ++i) {
        calculate_matrix(input_queue, output_queue, threads_count);
    }
}

void calculate_matrix(
        LockFreeQueue<MatricesHolder>& input_queue,
        LockFreeQueue<Matrix*>& output_queue,
        size_t threads_count
) {
    auto [first_matrix, second_matrix] = input_queue.pop();
    Logger::printThreadIdWithText("Pair of matrices got from the `INPUT` queue");
    std::unique_ptr<Matrix> first_matrix_u(first_matrix);
    std::unique_ptr<Matrix> second_matrix_u(second_matrix);

    auto summed_matrix = first_matrix_u->parallel_sum(*second_matrix_u, threads_count);
    output_queue.push(new Matrix(summed_matrix));

    Logger::printThreadIdWithText("Summed matrix pushed to the `OUTPUT` queue");
}

void print_matrix_with_repetitions(size_t iterations_count, LockFreeQueue<Matrix*>& queue) {
    for (size_t i = 0; i < iterations_count; ++i) {
        print_matrix(queue);
    }
}

void print_matrix(LockFreeQueue<Matrix*>& queue) {
    std::ofstream output(filenames::kMatrixOutputFilename);
    std::unique_ptr<Matrix> element(queue.pop());

    Logger::printThreadIdWithText("Summed matrix got from the `OUTPUT` queue");

    output << *element;
}