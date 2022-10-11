#include "matrix_operations.h"

void sum_elements(int* first_matrix, int* second_matrix, long start_index, long end_index){
    for(long i = start_index; i < end_index; i++){
        first_matrix[i] += second_matrix[i];
    }
}
