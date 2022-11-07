#ifndef DATA_H
#define DATA_H

#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

#include "list.h"

struct GenerationThreadInfo{
    List *gen_queue;
    int matrix_height, matrix_width;
    unsigned long iter_count;
    atomic_bool *is_running;
};

struct SummatorMainThreadInfo{
    List *gen_queue, *writer_queue;
    int matrix_height, matrix_width, number_of_threads;
    atomic_bool *is_running, *sum_is_running;
};

struct SummatorThreadInfo{
    int *matrix1, *matrix2;
    long start_index, end_index;
    pthread_cond_t *wait, *end_sum;
    int number_of_threads;
    int *waiters;
    char *cur_iter;
    pthread_mutex_t *mutex;
    atomic_bool *is_running;
};

struct WriterThreadInfo{
    List *writer_queue;
    int matrix_height, matrix_width;
    char *filename;
    atomic_bool *is_running;
};

typedef struct GenerationThreadInfo GenerationThreadInfo;

typedef struct SummatorMainThreadInfo SummatorMainThreadInfo;

typedef struct SummatorThreadInfo SummatorThreadInfo;

typedef struct WriterThreadInfo WriterThreadInfo;

#endif // DATA_H
