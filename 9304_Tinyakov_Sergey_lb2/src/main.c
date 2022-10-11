#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#include "data.h"
#include "generation.h"
#include "summator.h"
#include "writer.h"

#define MAX_QUEUE_SIZE 10
#define MATRIX_HEIGHT 3
#define MATRIX_WIDTH 3
#define SUM_THREADS 3
#define ITER_COUNT 100
#define FILENAME NULL

int main(int argc, char **argv){
    long height = MATRIX_HEIGHT, width = MATRIX_WIDTH, iterations = ITER_COUNT;
    unsigned int queue_size = MAX_QUEUE_SIZE;
    int sum_threads_number = SUM_THREADS;
    char *filename = FILENAME;
    if (argc < 7){
        puts("Usage: thread_sum <matrix_height> <matrix_width> <iterations_number> <thread_number> <max_queue_size> <filename>");
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
    atomic_bool is_running = 1, sum_is_running = 1;
    List *gen_queue = create_list(queue_size), *writer_queue = create_list(queue_size);
    GenerationThreadInfo gen_info = {gen_queue, height, width, iterations, &is_running};
    SummatorMainThreadInfo sum_info = {gen_queue, writer_queue, height, width, sum_threads_number, &is_running, &sum_is_running};
    WriterThreadInfo writer_info = {writer_queue, height, width, filename, &sum_is_running};
    pthread_t gen, sum, writer;
    pthread_create(&gen, NULL, generation_thread, &gen_info);
    pthread_create(&sum, NULL, summator_main_thread, &sum_info);
    pthread_create(&writer, NULL, writer_thread, &writer_info);
    pthread_join(gen, NULL);
    pthread_join(sum, NULL);
    pthread_join(writer, NULL);
    return 0;
}
