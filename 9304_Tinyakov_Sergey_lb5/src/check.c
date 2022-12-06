#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>

#include "matrix.h"
#include "matrix_operations.h"
#include "parallel_matrix_multiplication.h"
#include "gpu_matrix_multiplication.h"

#define MATRIX_HEIGHT 4
#define MATRIX_WIDTH 4
#define THREADS_NUM 13
#define BLOCK_SIZE 16

int main(int argc, char **argv){
    long height = MATRIX_HEIGHT, width = MATRIX_WIDTH;
    int threads_number = THREADS_NUM;
    if (argc < 3){
        puts("Usage: check <matrix_height> <matrix_width>");
    }
    if(argc >= 2){
        height = strtol(argv[1], NULL, 10);
        if (height < 1){
            printf("Incorrect height %s\n", argv[1]);
            return 1;
        }
    }
    if(argc >= 3){
        width = strtol(argv[2], NULL, 10);
        if (width < 1){
            printf("Incorrect width %s\n", argv[2]);
            return 1;
        }
    }
    int ret = 0;
    srand(time(NULL));
    Matrix *matrix1 = generate_matrix(height, width), *matrix2 = generate_matrix(width, height),
           *matrix_elem = create_matrix(height, height, 0), *matrix_blocks = create_matrix(height, height, 0),
           *matrix_strassen = create_matrix(height, height, 0), *matrix_gpu = create_matrix(height, height, 0);
    if(!matrix1 || !matrix2 || !matrix_elem || !matrix_blocks){
        ret = 1;
        goto main_end;
    }

    puts("Multiplication by elements");
    pmm_by_element(matrix1, matrix2, matrix_elem, threads_number);
    puts("Multiplication by blocks");
    pmm_by_blocks(matrix1, matrix2, matrix_blocks, threads_number, BLOCK_SIZE);
    puts("Multiplication by Strassen algorithm");
    strassen_pmm(matrix1, matrix2, matrix_strassen, threads_number, BLOCK_SIZE);
    if(compile_kernel()){
        goto gpu_end;
    }
    if(set_kernel_args(matrix1, matrix2, matrix_gpu)){
        goto gpu_end;
    }
    if(write_kernel_data(matrix1, matrix2)){
        goto gpu_end;
    }
    if(run_kernel()){
        goto gpu_end;
    }
    if(read_kernel_data(matrix_gpu)){
        goto gpu_end;
    }
gpu_end:
    clear_kernel();
    for(long i = 0; i < height * height; i++){
        if(matrix_elem->data[i] != matrix_blocks->data[i] || matrix_elem->data[i] != matrix_strassen->data[i] || matrix_elem->data[i] != matrix_gpu->data[i]){
            ret = 2;
            printf("Elements under index %d are not equal\n", i);
            break;
        }
    }
    if(!ret)
        puts("Matrices are equal");
main_end:
    free_matrix(matrix1);
    free_matrix(matrix2);
    free_matrix(matrix_elem);
    free_matrix(matrix_blocks);
    free_matrix(matrix_strassen);
    return ret;
}
