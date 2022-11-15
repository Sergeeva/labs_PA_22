#include "parallel_matrix_multiplication.h"

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include "matrix_operations.h"

#define DIV_ROUNDUP(a, b) ((a) % (b) == 0 ? (a) / (b) : (a) / (b) + 1)
#define READ 0
#define WRITE 1
#define STRASSEN_POSITIVE_SUM 1
#define STRASSEN_NEGATIVE_SUM -1
#define STRASSEN_START_INDEX -1

typedef struct {
    Matrix *first;
    Matrix *second;
    Matrix *result;
    long start_index, end_index, block_size, blocks_in_col, k;
} __ThreadInfo;

typedef struct {
    Matrix **a, **b, **p;
    long index, block_size;
    atomic_int *cur_threads;
    int max_threads;
} __StrassenThreadInfo;

void* pmm_by_element_thread_func(void* arg){
    __ThreadInfo *info = (__ThreadInfo*) arg;
    for(long i = info->start_index; i < info->end_index; i++){
        info->result->data[i] = multiply_one_element(info->first, info->second, i / info->result->col_size, i % info->result->col_size);
    }
    return NULL;
}

void pmm_by_element(Matrix *first, Matrix *second, Matrix *result, int thread_num){
    __ThreadInfo *infos = malloc(sizeof(__ThreadInfo) * thread_num);
    pthread_t *threads = malloc(sizeof(pthread_t) * thread_num);
    if(infos == NULL) goto pmm_by_element_end;
    if(threads == NULL) goto pmm_by_element_end;
    long res_size = result->row_size * result->col_size, elems_per_thread = res_size / thread_num + 1;
    if(res_size % thread_num == 0) elems_per_thread--;
    for(int i = 0; i < thread_num; i++){
        infos[i].first = first;
        infos[i].second = second;
        infos[i].result = result;
        infos[i].start_index = elems_per_thread * i;
        infos[i].end_index = elems_per_thread * (i + 1);
        if(infos[i].end_index > res_size) infos[i].end_index = res_size;
    }
    for(int i = 0; i < thread_num; i++){
        int k = pthread_create(&threads[i], NULL, pmm_by_element_thread_func, (void*)&infos[i]);
    }
    for(int i = 0; i < thread_num; i++){
        pthread_join(threads[i], NULL);
    }
pmm_by_element_end:
    if(infos) free(infos);
    if(threads) free(threads);
}


void __read_write_block(int *block, Matrix *matrix, long block_index, long block_size, long blocks_in_col, int write){
    long start_row = block_index / blocks_in_col * block_size, start_col = (block_index % blocks_in_col) * block_size;
    long end_row = start_row + block_size, end_col = start_col + block_size;
    for(long i = start_row; i < end_row; i++){
        for(long j = start_col; j < end_col; j++){
            if(write){
                if(i < matrix->row_size && j < matrix->col_size)
                    matrix->data[i * matrix->col_size + j] = block[(i - start_row) * block_size + (j - start_col)];
            }else{
                block[(i - start_row) * block_size + (j - start_col)] = i >= matrix->row_size || j >= matrix->col_size ? 0 : matrix->data[i * matrix->col_size + j];
            }
        }
    }
}


void* pmm_by_blocks_thread_func(void* arg){
    __ThreadInfo *info = (__ThreadInfo*) arg;
    long block_size = info->block_size, max_k = DIV_ROUNDUP(info->first->col_size, block_size);
    int *block_a = malloc(sizeof(int) * block_size * block_size),
        *block_b = malloc(sizeof(int) * block_size * block_size),
        *res_block = malloc(sizeof(int) * block_size * block_size);
    for(long block_i = info->start_index; block_i < info->end_index; block_i++){
        memset(res_block, 0, sizeof(int) * block_size * block_size);
        long i = block_i / info->blocks_in_col, j = block_i % info->blocks_in_col;
        for(long k = 0; k < max_k; k++){
            __read_write_block(block_a, info->first, i * max_k + k, block_size, max_k, READ);
            __read_write_block(block_b, info->second, k * info->blocks_in_col + j, block_size, info->blocks_in_col, READ);
            multiply_one_block(block_a, block_b, res_block, block_size);
        }
        __read_write_block(res_block, info->result, block_i, block_size, info->blocks_in_col, WRITE);
    }
    if(block_a) free(block_a);
    if(block_b) free(block_b);
    if(res_block) free(res_block);
    return NULL;
}

void pmm_by_blocks(Matrix *first, Matrix *second, Matrix *result, int thread_num, long block_size){
    __ThreadInfo *infos = malloc(sizeof(__ThreadInfo) * thread_num);
    pthread_t *threads = malloc(sizeof(pthread_t) * thread_num);
    if(infos == NULL) goto pmm_by_blocks_end;
    if(threads == NULL) goto pmm_by_blocks_end;
    long blocks_in_row = DIV_ROUNDUP(result->row_size, block_size), blocks_in_col = DIV_ROUNDUP(result->col_size, block_size);
    long res_blocks = blocks_in_row * blocks_in_col, blocks_per_thread = DIV_ROUNDUP(res_blocks, thread_num);
    for(int i = 0; i < thread_num; i++){
        infos[i].first = first;
        infos[i].second = second;
        infos[i].result = result;
        infos[i].block_size = block_size;
        infos[i].blocks_in_col = blocks_in_col;
        infos[i].start_index = blocks_per_thread * i;
        infos[i].end_index = blocks_per_thread * (i + 1);
        if(infos[i].end_index > res_blocks) infos[i].end_index = res_blocks;
    }
    for(int i = 0; i < thread_num; i++){
        int k = pthread_create(&threads[i], NULL, pmm_by_blocks_thread_func, (void*)&infos[i]);
    }
    for(int i = 0; i < thread_num; i++){
        pthread_join(threads[i], NULL);
    }
pmm_by_blocks_end:
    if(infos) free(infos);
    if(threads) free(threads);
}

void __read_matrix_to_submatrices(Matrix *matrix, Matrix **submatrices){
    long rows = submatrices[0]->row_size, cols = submatrices[0]->col_size;
    long index, max_size = matrix->col_size * matrix->row_size;
    for(long i = 0; i < rows; i++){
        for(long j = 0; j < cols; j++){
            submatrices[0]->data[i * cols + j] = matrix->data[i * matrix->col_size + j];
            submatrices[1]->data[i * cols + j] = j + cols < matrix->col_size ? matrix->data[i * matrix->col_size + j + cols] : 0;
            submatrices[2]->data[i * cols + j] = i + rows < matrix->row_size ? matrix->data[(i + rows) * matrix->col_size + j] : 0;
            submatrices[3]->data[i * cols + j] = i + rows < matrix->row_size && j + cols < matrix->col_size ? matrix->data[(i + rows) * matrix->col_size + j + cols] : 0;
        }
    }
}

void __write_submatrices_to_matrix(Matrix *sm00, Matrix *sm01, Matrix *sm10, Matrix *sm11, Matrix *matrix){
    long rows = sm00->row_size, cols = sm00->col_size;
    long index, max_size = matrix->col_size * matrix->row_size;
    for(long i = 0; i < rows; i++){
        for(long j = 0; j < cols; j++){
            matrix->data[i * matrix->col_size + j] = sm00->data[i * cols + j];
            if(j + cols < matrix->col_size) matrix->data[i * matrix->col_size + j + cols] = sm01->data[i * cols + j];
            if(i + rows < matrix->row_size) matrix->data[(i + rows) * matrix->col_size + j] = sm10->data[i * cols + j];
            if(i + rows < matrix->row_size && j + cols < matrix->col_size) matrix->data[(i + rows) * matrix->col_size + j + cols] = sm11->data[i * cols + j];
        }
    }
}

void __strassen_sum_matrices(Matrix **a, Matrix **b, Matrix **p, int index){
    switch(index){
        case 0:
            sum_matrices(b[1], b[3], p[0], STRASSEN_NEGATIVE_SUM);
            break;
        case 1:
            sum_matrices(a[0], a[1], p[1], STRASSEN_POSITIVE_SUM);
            break;
        case 2:
            sum_matrices(a[2], a[3], p[2], STRASSEN_POSITIVE_SUM);
            break;
        case 3:
            sum_matrices(b[2], b[0], p[3], STRASSEN_NEGATIVE_SUM);
            break;
        case 4:
            sum_matrices(a[0], a[3], p[4], STRASSEN_POSITIVE_SUM);
            sum_matrices(b[0], b[3], p[7], STRASSEN_POSITIVE_SUM);
            break;
        case 5:
            sum_matrices(a[1], a[3], p[5], STRASSEN_NEGATIVE_SUM);
            sum_matrices(b[2], b[3], p[8], STRASSEN_POSITIVE_SUM);
            break;
        case 6:
            sum_matrices(a[0], a[2], p[6], STRASSEN_NEGATIVE_SUM);
            sum_matrices(b[0], b[1], p[9], STRASSEN_POSITIVE_SUM);
            break;
    }
}

int __check_matrix_size(Matrix *matrix, long block_size){
    return (matrix->row_size < block_size && matrix->col_size < block_size) || matrix->row_size == 1 || matrix->col_size == 1;
}

void* strassen_pmm_thread_func(void* arg); // forward declaration

void strassen_multiplication(Matrix *first, Matrix *second, Matrix *result, long block_size, __StrassenThreadInfo *info){
    if(__check_matrix_size(first, block_size)){
        Matrix *tmp_res = create_matrix(result->row_size, result->col_size, 0);
        multiply_matrices(first, second, tmp_res);
        memcpy(result->data, tmp_res->data, sizeof(int) * result->row_size * result->col_size);
        free_matrix(tmp_res);
        return;
    }
    long a_row = DIV_ROUNDUP(first->row_size, 2), a_col = DIV_ROUNDUP(first->col_size, 2),
         b_row = DIV_ROUNDUP(second->row_size, 2), b_col = DIV_ROUNDUP(second->col_size, 2);
    Matrix *a[4], *b[4], *p[10]; // 7 matrices for p + 3 additional matrices
    for(int i = 0; i < 10; i++){
        if(i < 4){
            a[i] = create_matrix(a_row, a_col, 0);
            b[i] = create_matrix(b_row, b_col, 0);
        }
        p[i] = create_matrix(a_row, b_col, 0);
    }
    __read_matrix_to_submatrices(first, a);
    __read_matrix_to_submatrices(second, b);
    if(info){
        __StrassenThreadInfo *infos = malloc(sizeof(__StrassenThreadInfo) * 7);
        pthread_t *threads = malloc(sizeof(pthread_t) * 6);
        for(int i = 0; i < 7; i++){
            infos[i].a = a;
            infos[i].b = b;
            infos[i].p = p;
            infos[i].block_size = block_size;
            infos[i].index = i;
            infos[i].cur_threads = info->cur_threads;
            infos[i].max_threads = info->max_threads;
            if(i < 6)
                pthread_create(&threads[i], NULL, strassen_pmm_thread_func, (void*)&infos[i]);
        }
        strassen_pmm_thread_func((void*) &infos[6]);
        for(int i = 0; i < 6; i++){
            pthread_join(threads[i], NULL);
        }
        free(infos);
        free(threads);
    }else{
        for(int i = 0; i < 7; i++){
            __strassen_sum_matrices(a, b, p, i);
        }
        // Calculate p0
        strassen_multiplication(a[0], p[0], p[0], block_size, NULL);
        // Calculate p1
        strassen_multiplication(p[1], b[3], p[1], block_size, NULL);
        // Calculate p2
        strassen_multiplication(p[2], b[0], p[2], block_size, NULL);
        // Calculate p3
        strassen_multiplication(a[3], p[3], p[3], block_size, NULL);
        // Calculate p4
        strassen_multiplication(p[4], p[7], p[4], block_size, NULL);
        // Calculate p5
        strassen_multiplication(p[5], p[8], p[5], block_size, NULL);
        // Calculate p6
        strassen_multiplication(p[6], p[9], p[6], block_size, NULL);
    }
    // p[6] -- res11
    sum_matrices(p[0], p[6], p[6], STRASSEN_NEGATIVE_SUM);
    sum_matrices(p[6], p[4], p[6], STRASSEN_POSITIVE_SUM);
    sum_matrices(p[6], p[2], p[6], STRASSEN_NEGATIVE_SUM);
    // p[2] -- res10
    sum_matrices(p[2], p[3], p[2], STRASSEN_POSITIVE_SUM);
    // p[0] -- res01
    sum_matrices(p[0], p[1], p[0], STRASSEN_POSITIVE_SUM);
    // p[4] -- res00
    sum_matrices(p[4], p[3], p[4], STRASSEN_POSITIVE_SUM);
    sum_matrices(p[4], p[1], p[4], STRASSEN_NEGATIVE_SUM);
    sum_matrices(p[4], p[5], p[4], STRASSEN_POSITIVE_SUM);
    // write to result
    __write_submatrices_to_matrix(p[4], p[0], p[2], p[6], result);
    // free matrices
    for(int i = 0; i < 10; i++){
        if(i < 4){
            free_matrix(a[i]);
            free_matrix(b[i]);
        }
        free_matrix(p[i]);
    }
}


void* strassen_pmm_thread_func(void* arg){
    __StrassenThreadInfo *info = (__StrassenThreadInfo*) arg;
    if(info->index != STRASSEN_START_INDEX)
        __strassen_sum_matrices(info->a, info->b, info->p, info->index);
    char with_threads = atomic_fetch_add(info->cur_threads, 6) < info->max_threads ? 1 : 0;
    if(!with_threads) atomic_fetch_add(info->cur_threads, -6);
    switch(info->index){
        case STRASSEN_START_INDEX:
            strassen_multiplication((Matrix*) info->a, (Matrix*) info->b, (Matrix*) info->p, info->block_size, with_threads ? info : NULL);
            break;
        case 0:
            strassen_multiplication(info->a[0], info->p[0], info->p[0], info->block_size, with_threads ? info : NULL);
            break;
        case 1:
            strassen_multiplication(info->p[1], info->b[3], info->p[1], info->block_size, with_threads ? info : NULL);
            break;
        case 2:
            strassen_multiplication(info->p[2], info->b[0], info->p[2], info->block_size, with_threads ? info : NULL);
            break;
        case 3:
            strassen_multiplication(info->a[3], info->p[3], info->p[3], info->block_size, with_threads ? info : NULL);
            break;
        case 4:
            strassen_multiplication(info->p[4], info->p[7], info->p[4], info->block_size, with_threads ? info : NULL);
            break;
        case 5:
            strassen_multiplication(info->p[5], info->p[8], info->p[5], info->block_size, with_threads ? info : NULL);
            break;
        case 6:
            strassen_multiplication(info->p[6], info->p[9], info->p[6], info->block_size, with_threads ? info : NULL);
            break;
    }
    if(with_threads) atomic_fetch_add(info->cur_threads, -6);
}

void strassen_pmm(Matrix *first, Matrix *second, Matrix *result, int thread_num, long block_size){
    __StrassenThreadInfo info;
    atomic_int threads = 0;
    info.a = (Matrix**) first;
    info.b = (Matrix**) second;
    info.p = (Matrix**) result;
    info.max_threads = thread_num;
    info.block_size = block_size;
    info.index = STRASSEN_START_INDEX;
    info.cur_threads = &threads;
    strassen_pmm_thread_func((void*)&info);
}
