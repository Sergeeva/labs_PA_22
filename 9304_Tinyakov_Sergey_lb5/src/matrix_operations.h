#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "matrix.h"

#define MAX_ABS_VALUE (100)

Matrix* generate_matrix(long row_count, long col_count);

Matrix* create_matrix(long row_count, long col_count, char fill_value);

void write_matrix(Matrix *matrix, char *filename);

void free_matrix(Matrix *matrix);

void fill_matrix_with_values(Matrix *matrix, int value);

int multiply_one_element(Matrix *first, Matrix *second, int i, int j);

void multiply_one_block(int *block_a, int *block_b, int *res_block, long block_size);

void multiply_matrices(Matrix *first, Matrix *second, Matrix *result);

void sum_matrices(Matrix *first, Matrix *second, Matrix *result, char sign);

#endif
