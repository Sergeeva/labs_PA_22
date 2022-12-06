#include "matrix_operations.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

Matrix* generate_matrix(long row_count, long col_count){
    long matrix_size = row_count * col_count;
    Matrix *matrix = malloc(sizeof(Matrix));
    if(matrix == NULL) return NULL;
    matrix->data = malloc(sizeof(int) * matrix_size);
    if(matrix->data == NULL){
        free(matrix);
        return NULL;
    }
    for(long i = 0; i < matrix_size; i++){
       matrix->data[i] = rand() % (2 * MAX_ABS_VALUE) - MAX_ABS_VALUE; 
    }
    matrix->row_size = row_count;
    matrix->col_size = col_count;
    return matrix;
}

Matrix* create_matrix(long row_count, long col_count, char fill_value){
    long matrix_size = row_count * col_count;
    Matrix *matrix = malloc(sizeof(Matrix));
    if(matrix == NULL) return NULL;
    matrix->data = malloc(sizeof(int) * matrix_size);
    if(matrix->data == NULL){
        free(matrix);
        return NULL;
    }
    memset(matrix->data, fill_value, sizeof(int) * matrix_size);
    matrix->row_size = row_count;
    matrix->col_size = col_count;
    return matrix;
}

void write_matrix(Matrix *matrix, char *filename){
    FILE *f;
    if (filename == NULL){
        f = stdout;
    }else{
        f = fopen(filename, "w");
        if (f == NULL){
            return;
        }
    }
    fputs("Matrix:\n", f);
    for(long i = 0; i < matrix->row_size; i++){
        for(long j = 0; j < matrix->col_size; j++){
            fprintf(f, "%d ", matrix->data[i * matrix->col_size + j]);
        }
        fprintf(f, "\n");
    }
    if (filename != NULL){
        fclose(f);
    }
}

void free_matrix(Matrix *matrix){
    if(matrix == NULL) return;
    free(matrix->data);
    free(matrix);
}

void fill_matrix_with_values(Matrix *matrix, int value){
    for(long i = 0; i < matrix->col_size * matrix->row_size; i++){
        matrix->data[i] = value;
    }
}

int multiply_one_element(Matrix *first, Matrix *second, int i, int j){
    int res = 0;
    for(int k = 0; k < first->col_size; k++){
        res += first->data[i * first->col_size + k] * second->data[k * second->col_size + j];
    }
    return res;
}

void multiply_one_block(int *block_a, int *block_b, int *res_block, long block_size){
    for(long i = 0; i < block_size; i++){
        for(long j = 0; j < block_size; j++){
            for(long k = 0; k < block_size; k++){
                res_block[i * block_size + j] += block_a[i * block_size + k] * block_b[k * block_size + j];
            }
        }
    }
}

void multiply_matrices(Matrix *first, Matrix *second, Matrix *result){
    for(long i = 0; i < first->row_size; i++){
        for(long j = 0; j < second->col_size; j++){
            for(long k = 0; k < first->col_size; k++){
                result->data[i * result->col_size + j] += first->data[i * first->col_size + k] * second->data[k * second->col_size + j];
            }
        }
    }
}

void sum_matrices(Matrix *first, Matrix *second, Matrix *result, char sign){
    sign = sign < 0 ? -1 : 1;
    for(long i = 0; i < first->row_size * first->col_size; i++){
        result->data[i] = first->data[i] + sign * second->data[i];
    }
}
