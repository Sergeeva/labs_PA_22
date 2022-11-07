#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>

#include "data.h"
#include "generation.h"
#include "summator.h"
#include "writer.h"
#include "list.h"
#include "non_blocking_list.h"

#define NS 1000000000L
#define MAX_QUEUE_SIZE 10
#define MATRIX_HEIGHT 3
#define MATRIX_WIDTH 3
#define SUM_THREADS 3
#define ITER_COUNT 100
#define FILENAME NULL

void* __create_queue(unsigned int size){
    return (void*) create_list(size);
}

void __destroy_queue(void *list){
    destroy_list((List*) list);
}

void __queue_push(void *list, int *data){
    list_push((List*) list, data);
}

int* __queue_pop(void *list){
    return list_pop((List*) list);
}

void* __create_non_blocking_queue(unsigned int size){
    return (void*) create_non_blocking_list(size);
}

void __destroy_non_blocking__queue(void *list){
    destroy_non_blocking_list((NonBlockingList*) list);
}

void __non_blocking_queue_push(void *list, int *data){
    non_blocking_list_push((NonBlockingList*) list, data);
}

int* __non_blocking_queue_pop(void *list){
    return non_blocking_list_pop((NonBlockingList*) list);
}

int main(int argc, char **argv){
    long height = MATRIX_HEIGHT, width = MATRIX_WIDTH, iterations = ITER_COUNT;
    unsigned int queue_size = MAX_QUEUE_SIZE;
    int sum_threads_number = SUM_THREADS;
    char *filename = FILENAME;
    void* (*queue_create_func)(unsigned int) = __create_queue;
    void (*queue_destroy_func)(void*) = __destroy_queue;
    void (*queue_push_func)(void*, int*) = __queue_push;
    int* (*queue_pop_func)(void*) = __queue_pop;
    if (argc < 8){
        puts("Usage: thread_sum <matrix_height> <matrix_width> <iterations_number> <thread_number> <max_queue_size> <filename> <type>");
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
        iterations = strtol(argv[3], NULL, 10);
        if (iterations < 1){
            printf("Incorrect iterations number %s\n", argv[3]);
            return 1;
        }
    }
    if(argc >= 5){
        sum_threads_number = (int)strtol(argv[4], NULL, 10);
        if (sum_threads_number < 1){
            printf("Incorrect sum_threads_number %s\n", argv[4]);
            return 1;
        }
    }
    if(argc >= 6){
        queue_size = (unsigned int)strtol(argv[5], NULL, 10);
        if (queue_size < 1){
            printf("Incorrect queue_size %s\n", argv[5]);
            return 1;
        }
    }
    if(argc >= 7){
        filename = argv[6];
    }
    if(argc >= 8){
        if(!strcmp(argv[7], "non_blocking")){
            queue_create_func = __create_non_blocking_queue;
            queue_destroy_func = __destroy_non_blocking__queue;
            queue_push_func = __non_blocking_queue_push;
            queue_pop_func = __non_blocking_queue_pop;
        }else if(strcmp(argv[7], "blocking")){
            printf("Unknown type %s\n", argv[7]);
            return 1;
        }
    }
    atomic_bool is_running = 1, sum_is_running = 1;
    void *gen_queue = (*queue_create_func)(queue_size), *writer_queue = (*queue_create_func)(queue_size);
    GenerationThreadInfo gen_info = {gen_queue, queue_push_func, height, width, iterations, &is_running};
    SummatorMainThreadInfo sum_info = {gen_queue, writer_queue, queue_push_func, queue_pop_func, height, width, sum_threads_number, iterations, &is_running, &sum_is_running};
    WriterThreadInfo writer_info = {writer_queue, queue_pop_func, height, width, iterations, filename, &sum_is_running};
    pthread_t gen, sum, writer;
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    pthread_create(&gen, NULL, generation_thread, &gen_info);
    pthread_create(&sum, NULL, summator_main_thread, &sum_info);
    pthread_create(&writer, NULL, writer_thread, &writer_info);
    pthread_join(gen, NULL);
    pthread_join(sum, NULL);
    pthread_join(writer, NULL);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
    long long time_diff = (end_time.tv_sec - start_time.tv_sec) * NS + (end_time.tv_nsec - start_time.tv_nsec);
    printf("Queue size: %d; Type: %s; Measured time: %ld\n", queue_size, argv[7], time_diff);
    (*queue_destroy_func)(gen_queue);
    (*queue_destroy_func)(writer_queue);
    return 0;
}
