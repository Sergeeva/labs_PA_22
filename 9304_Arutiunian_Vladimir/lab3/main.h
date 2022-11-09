#ifndef LABS_PA_22_LAB3_MAIN_H
#define LABS_PA_22_LAB3_MAIN_H

#include <fstream>
#include <thread>

#include "lock-free-queue/lock-free-queue.h"
#include "constant/filenames.h"
#include "matrix/matrix.h"
#include "logger/logger.h"
#include "time/Time.h"

using MatricesHolder = std::pair<Matrix*, Matrix*>;

void enter_matrices_with_repetitions(size_t iterations_count, LockFreeQueue<MatricesHolder>& queue);

void enter_matrices(LockFreeQueue<MatricesHolder>& queue);

void calculate_matrix_with_repetitions(
        size_t iterations_count,
        LockFreeQueue<MatricesHolder>& input_queue,
        LockFreeQueue<Matrix*>& output_queue,
        size_t threads_count
);

void calculate_matrix(
        LockFreeQueue<MatricesHolder>& input_queue,
        LockFreeQueue<Matrix*>& output_queue,
        size_t threads_count
);

void print_matrix_with_repetitions(
        size_t iterations_count,
        LockFreeQueue<Matrix*>& queue
);

void print_matrix(LockFreeQueue<Matrix*>& queue);

#endif //LABS_PA_22_LAB3_MAIN_H
