#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>

#include "matrix.h"
#include "matrix_operations.h"
#include "parallel_matrix_multiplication.h"

#define NS 1000000000L
#define MATRIX_HEIGHT 4
#define MATRIX_WIDTH 4
#define THREADS_NUM 3
#define FILENAME NULL
#define BLOCK_SIZE 1
#define TYPE_ELEMENTS 1
#define TYPE_BLOCKS 2
#define TYPE_STRASSEN 3

int main(int argc, char **argv){
    long height = MATRIX_HEIGHT, width = MATRIX_WIDTH;
    int threads_number = THREADS_NUM;
    char *filename = FILENAME;
    long block_size = BLOCK_SIZE;
    long type = TYPE_ELEMENTS;
    if (argc < 7){
        puts("Usage: pmm <matrix_height> <matrix_width> <thread_number> <filename> <type> <block_size>");
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
    if(argc >= 4){
        threads_number = (int)strtol(argv[3], NULL, 10);
        if (threads_number < 1){
            printf("Incorrect threads_number %s\n", argv[3]);
            return 1;
        }
    }
    if(argc >= 5){
        filename = argv[4];
    }
    if(argc >= 6){
        if(!strcmp(argv[5], "blocks")){
            type = TYPE_BLOCKS;
        }else if(!strcmp(argv[5], "strassen")){
            type = TYPE_STRASSEN;
        }else if(strcmp(argv[5], "elements")){
            printf("Unknown type %s\n", argv[5]);
            return 1;
        }
    }
    if(argc >= 7){
        block_size = strtol(argv[6], NULL, 10);
        if (block_size < 1){
            printf("Incorrect block_size %s\n", argv[6]);
            return 1;
        }
    }
    int ret = 0;
    srand(time(NULL));
    Matrix *matrix1 = generate_matrix(height, width), *matrix2 = generate_matrix(width, height), *matrix3 = generate_matrix(height, height);
    if(!matrix1 || !matrix2 || !matrix3){
        ret = 1;
        goto main_end;
    }
    fill_matrix_with_values(matrix3, 0);
    struct timespec start_time, end_time;
    char *type_str;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    switch(type){
        case TYPE_ELEMENTS:
            pmm_by_element(matrix1, matrix2, matrix3, threads_number);
            type_str = "elements";
            break;
        case TYPE_BLOCKS:
            pmm_by_blocks(matrix1, matrix2, matrix3, threads_number, block_size);
            type_str = "blocks";
            break;
        case TYPE_STRASSEN:
            strassen_pmm(matrix1, matrix2, matrix3, threads_number, block_size);
            type_str = "strassen";
            break;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
    write_matrix(matrix3, filename);
    long long time_diff = (end_time.tv_sec - start_time.tv_sec) * NS + (end_time.tv_nsec - start_time.tv_nsec);
    printf("Matrix size: %dx%d; Type %s; Threads: %d; Block size: %d; Measured time: %ld\n", height, width, type_str, threads_number, block_size, time_diff);
main_end:
    free_matrix(matrix1);
    free_matrix(matrix2);
    free_matrix(matrix3);
    return ret;
}
