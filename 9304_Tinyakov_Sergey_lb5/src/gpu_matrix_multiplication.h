#include "matrix.h"

int compile_kernel();

int run_kernel();

void clear_kernel();

int write_kernel_data(Matrix *matrix1, Matrix *matrix2);

int set_kernel_args(Matrix *matrix1, Matrix *matrix2, Matrix *result);

int read_kernel_data(Matrix *result);
