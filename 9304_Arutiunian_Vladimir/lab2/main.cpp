#include "main.h"

int main() {
    size_t iterations_count = 15;
    size_t queue_max_size = 3;
    size_t threads_count_for_adding = 8;

    BlockingQueue<std::pair<Matrix, Matrix>> input_queue("INPUT", queue_max_size);
    BlockingQueue<Matrix> output_queue("OUTPUT", queue_max_size);


    std::thread input_thread(
            enter_matrices_with_repetitions,
            iterations_count,
            std::ref(input_queue)
    );
    std::thread calculate_thread(
            calculate_matrix_with_repetitions,
            iterations_count,
            std::ref(input_queue),
            std::ref(output_queue),
            threads_count_for_adding
    );
    std::thread output_thread(
            print_matrix_with_repetitions,
            iterations_count,
            std::ref(output_queue)
    );

    input_thread.join();
    calculate_thread.join();
    output_thread.join();

    return 0;
}


void enter_matrices_with_repetitions(size_t iterations_count, BlockingQueue<std::pair<Matrix, Matrix>>& queue) {
    for (size_t i = 0; i < iterations_count; ++i) {
        enter_matrices(queue);
    }
}

void enter_matrices(BlockingQueue<std::pair<Matrix, Matrix>>& queue) {
    std::ifstream input(filenames::kMatrixInputFilename);

    Matrix first_matrix;
    Matrix second_matrix;

    input >> first_matrix >> second_matrix;

    queue.push(std::make_pair(first_matrix, second_matrix));
}

void calculate_matrix_with_repetitions(
        size_t iterations_count,
        BlockingQueue<std::pair<Matrix, Matrix>>& input_queue,
        BlockingQueue<Matrix>& output_queue,
        size_t threads_count
) {
    for (size_t i = 0; i < iterations_count; ++i) {
        calculate_matrix(input_queue, output_queue, threads_count);
    }
}

void calculate_matrix(
        BlockingQueue<std::pair<Matrix, Matrix>>& input_queue,
        BlockingQueue<Matrix>& output_queue,
        size_t threads_count
) {
    auto [first_matrix, second_matrix] = input_queue.pop();
    auto summed_matrix = first_matrix.parallelSum(second_matrix, threads_count);
    output_queue.push(summed_matrix);
}

void print_matrix_with_repetitions(
        size_t iterations_count,
        BlockingQueue<Matrix>& queue
) {
    for (size_t i = 0; i < iterations_count; ++i) {
        print_matrix(queue);
    }
}


void print_matrix(BlockingQueue<Matrix>& queue) {
    std::ofstream output(filenames::kMatrixOutputFilename);
    output << queue.pop();
}
