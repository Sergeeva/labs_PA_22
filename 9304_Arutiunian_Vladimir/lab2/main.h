#ifndef LABS_PA_22_LAB2_MAIN_H
#define LABS_PA_22_LAB2_MAIN_H

#include <fstream>
#include <thread>

#include "blocking-queue/blocking-queue.h"
#include "constant/filenames.h"
#include "matrix/matrix.h"

void enter_matrices_with_repetitions(size_t iterations_count, BlockingQueue<std::pair<Matrix, Matrix>>& queue);

void enter_matrices(BlockingQueue<std::pair<Matrix, Matrix>>& queue);

void calculate_matrix_with_repetitions(
        size_t iterations_count,
        BlockingQueue<std::pair<Matrix, Matrix>>& input_queue,
        BlockingQueue<Matrix>& output_queue,
        size_t threads_count
);

void calculate_matrix(
        BlockingQueue<std::pair<Matrix, Matrix>>& input_queue,
        BlockingQueue<Matrix>& output_queue,
        size_t threads_count
);

void print_matrix_with_repetitions(
        size_t iterations_count,
        BlockingQueue<Matrix>& queue
);

void print_matrix(BlockingQueue<Matrix>& queue);

#endif //LABS_PA_22_LAB2_MAIN_H
