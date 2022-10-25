#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <string.h>

#include "list.h"
#include "non_blocking_list.h"
#include "non_blocking_cycled_list.h"

#define NS 1000000000L
#define DATA_SIZE 3
#define MAX_RAND 1000
#define THREADS_NUMBER 10
#define MAX_QUEUE_SIZE 100
#define ITER_COUNT 1000

typedef struct Pipeline{
    void *queue;
    atomic_int *check_p, *check_c;
    atomic_int producer_iter, consumer_iter;
    int iters;
    void* (*create_func)(unsigned int);
    void (*destroy_func)(void*);
    void (*push_func)(void*, int*);
    int* (*pop_func)(void*);
} Pipeline;

typedef struct ThreadInfo{
    Pipeline *pipeline;
    int id;
} ThreadInfo;


void* __create_blocking_list(unsigned int size){
    return (void*) create_list(size);
}

void __destroy_blocking_list(void *list){
    destroy_list((List*) list);
}

void __blocking_list_push(void *list, int *data){
    list_push((List*) list, data);
}

int* __blocking_list_pop(void *list){
    return list_pop((List*) list);
}

void* __create_non_blocking_list(unsigned int size){
    return (void*) create_non_blocking_list(size);
}

void __destroy_non_blocking_list(void *list){
    destroy_non_blocking_list((NonBlockingList*) list);
}

void __non_blocking_list_push(void *list, int *data){
    non_blocking_list_push((NonBlockingList*) list, data);
}

int* __non_blocking_list_pop(void *list){
    return non_blocking_list_pop((NonBlockingList*) list);
}

void* __create_non_blocking_cycled_list(unsigned int size){
    return (void*) create_non_blocking_cycled_list(size);
}

void __destroy_non_blocking_cycled_list(void *list){
    destroy_non_blocking_cycled_list((NonBlockingCycledList*) list);
}

void __non_blocking_cycled_list_push(void *list, int *data){
    non_blocking_cycled_list_push((NonBlockingCycledList*) list, data);
}

int* __non_blocking_cycled_list_pop(void *list){
    return non_blocking_cycled_list_pop((NonBlockingCycledList*) list);
}


void *thread_producer(void *arg){
    ThreadInfo *info = (ThreadInfo*) arg;
    Pipeline *pipeline = info->pipeline;
    while(atomic_fetch_add(&pipeline->producer_iter, 1) < pipeline->iters){
        int *data = NULL;
        while(!data) data = malloc(sizeof(int) * DATA_SIZE);
        for(int i = 0; i < DATA_SIZE; i++){
            data[i] = rand() % MAX_RAND;
            atomic_fetch_add(pipeline->check_p + data[i], 1);
        }
        //non_blocking_list_push(pipeline->queue, data);
        (*pipeline->push_func)(pipeline->queue, data);
    }
    printf("Producer %d is finished\n", info->id);
}


void *thread_consumer(void *arg){
    ThreadInfo *info = (ThreadInfo*) arg;
    Pipeline *pipeline = info->pipeline;
    int k;
    while(atomic_fetch_add(&pipeline->consumer_iter, 1) < pipeline->iters){
        //int *data = non_blocking_list_pop(pipeline->queue);
        int *data = (*pipeline->pop_func)(pipeline->queue);
        for(int i = 0; i < DATA_SIZE; i++){
            atomic_fetch_add(pipeline->check_c + data[i], 1);
        }
        free(data);
    }
    printf("Consumer %d is finished\n", info->id);
}

int main(int argc, char **argv){
    int ret;
    long iter_count = ITER_COUNT, threads_number = THREADS_NUMBER;
    Pipeline pipeline;
    pipeline.create_func = __create_blocking_list;
    pipeline.destroy_func = __destroy_blocking_list;
    pipeline.push_func = __blocking_list_push;
    pipeline.pop_func = __blocking_list_pop;
    if (argc < 4){
        puts("Usage: test_time <type> <threads_count> <iterations_number>");
    }
    if(argc >= 2){
        if(!strcmp(argv[1], "non_blocking")){
            pipeline.create_func = __create_non_blocking_list;
            pipeline.destroy_func = __destroy_non_blocking_list;
            pipeline.push_func = __non_blocking_list_push;
            pipeline.pop_func = __non_blocking_list_pop;
        }else if(!strcmp(argv[1], "non_blocking_cycled")){
            pipeline.create_func = __create_non_blocking_cycled_list;
            pipeline.destroy_func = __destroy_non_blocking_cycled_list;
            pipeline.push_func = __non_blocking_cycled_list_push;
            pipeline.pop_func = __non_blocking_cycled_list_pop;
        }else if (strcmp(argv[1], "blocking")){
            printf("Unknown type %s\n", argv[1]);
            return 1;
        }
    }
    if(argc >= 3){
        threads_number = strtol(argv[2], NULL, 10);
        if (threads_number < 1){
            printf("Incorrect threads_number %s\n", argv[2]);
            return 1;
        }
    }
    if(argc >= 4){
        iter_count = strtol(argv[3], NULL, 10);
        if (iter_count < 1){
            printf("Incorrect iter_count number %s\n", argv[3]);
            return 1;
        }
    }
    pipeline.check_c = malloc(sizeof(atomic_int) * MAX_RAND);
    if (!pipeline.check_c){
        puts("Cannot alloc memory for check_c");
        ret = 1;
        goto end;
    }
    pipeline.check_p = malloc(sizeof(atomic_int) * MAX_RAND);
    if (!pipeline.check_p){
        puts("Cannot alloc memory for check_p");
        ret = 1;
        goto end;
    }
    pipeline.queue = (*pipeline.create_func)(MAX_QUEUE_SIZE);
    if (!pipeline.queue){
        puts("Cannot alloc memory for queue");
        ret = 1;
        goto end;
    }
    pipeline.iters = ITER_COUNT;
    pipeline.consumer_iter = 0;
    pipeline.producer_iter = 0;
    pthread_t *consumers = malloc(sizeof(pthread_t) * threads_number), *producers = malloc(sizeof(pthread_t) * threads_number);
    ThreadInfo *tinfo_c = malloc(sizeof(ThreadInfo) * threads_number), *tinfo_p = malloc(sizeof(ThreadInfo) * threads_number);
    if(!(consumers && producers && tinfo_c && tinfo_p)){
        puts("Cannot alloc memory for threads");
        ret = 1;
        goto end;
    }
    srand(time(NULL));
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
    for(int i = 0; i < threads_number; i++){
        tinfo_c[i].id = i;
        tinfo_c[i].pipeline = &pipeline;
        pthread_create(&consumers[i], NULL, thread_consumer, &tinfo_c[i]);
    }
    for(int i = 0; i < threads_number; i++){
        tinfo_p[i].id = i;
        tinfo_p[i].pipeline = &pipeline;
        pthread_create(&producers[i], NULL, thread_producer, &tinfo_p[i]);
    }
    for(int i = 0; i < threads_number; i++){
        pthread_join(consumers[i], NULL);
        pthread_join(producers[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
    for(int i = 0; i < MAX_RAND; i++){
        if (pipeline.check_c[i] != pipeline.check_p[i])
            printf("Value %d is not equal in check_c and check_p\n", i);
    }
    long long time_diff = (end_time.tv_sec - start_time.tv_sec) * NS + (end_time.tv_nsec - start_time.tv_nsec);
    printf("Type %s; Threads: %d; Iterations: %d; Measured time: %ld\n", argv[1], threads_number, iter_count, time_diff);
end:
    if(producers != NULL)
        free(producers);
    if(consumers != NULL)
        free(consumers);
    if(tinfo_c != NULL)
        free(tinfo_c);
    if(tinfo_p!= NULL)
        free(tinfo_p);
    if(pipeline.check_c != NULL)
        free(pipeline.check_c);
    if(pipeline.check_p != NULL)
        free(pipeline.check_p);
    if(pipeline.queue != NULL)
        (*pipeline.destroy_func)(pipeline.queue);
    return ret;
}
