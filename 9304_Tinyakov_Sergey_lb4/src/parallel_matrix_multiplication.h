#ifndef PARALLEL_MATRIX_MULTIPLICATION_H
#define PARALLEL_MATRIX_MULTIPLICATION_H

#include "matrix.h"

void pmm_by_element(Matrix *first, Matrix *second, Matrix *result, int thread_num);

void pmm_by_blocks(Matrix *first, Matrix *second, Matrix *result, int thread_num, long block_size);

void strassen_pmm(Matrix *first, Matrix *second, Matrix *result, int thread_num, long block_size);

#endif // PARALLEL_MATRIX_MULTIPLICATION
