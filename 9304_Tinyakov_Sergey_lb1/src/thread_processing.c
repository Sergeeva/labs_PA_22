#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "read_write.h"
#include "matrix_operations.h"
#include "data.h"

#define N_THREADS 4
#define EPS 0.0001

void* reader_func(void* arg){
    ThreadReaderMatrix *m = (ThreadReaderMatrix*) arg;
    if (read_matrix(m->filename, m->mode, m->data)){
        printf("[Reader %d] Cannot read matrix from file %s\n", m->id, m->filename);
        m->flag = 0;
    }else{
        m->flag = 0xFF;
    }
    return NULL;
}


void* summator_func(void* arg){
    ThreadSummatorMatrix *m = (ThreadSummatorMatrix*) arg;
    sum_elements(m->data1, m->data2, m->start_index, m->end_index);
    return NULL;
}

void* writer_func(void* arg){
    ThreadWriterMatrix *m = (ThreadWriterMatrix*) arg;
    if (write_matrix(m->filename, m->mode, m->data, m->height, m->width)){
        printf("[Writer] Cannot write matrix to the file %s\n", m->filename);
    }
    return NULL;
}


int main(int argc, char** argv){
    if (argc < 4){
        puts("Usage: thread_sum <input_filename1> <input_filename2> <output_filename1> <thread_number> <mode>\nMode: bin (default) or text");
        return 1;
    }
    char mode = 0;
    long n;
    if(argc < 5){
        puts("thread_number is not provided. Using one thread by default");
        n = 1;
    }else{
        n = strtol(argv[4], NULL, 10);
        if (n < 1){
            printf("Incorrect thread_number %s\n", argv[4]);
            return 1;
        }
    }
    if(argc < 6){
        printf("No mode is provided. Using bin mode by default\n");
        mode = BIN_MODE;
    }else if(strcmp(argv[5], "bin") == 0){
        printf("Using bin mode\n");
        mode = BIN_MODE;
    }else if(strcmp(argv[5], "text") == 0){
        printf("Using text mode\n");
        mode = TEXT_MODE;
    }else{
        printf("Unknown mode. Avaiable modes are 'bin' and 'text'\n");
        return 1;
    }
    // create time structures
    #ifdef TIME_OUTPUT
    struct timespec global_start, global_end, sum_start, sum_end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &global_start);
    #endif
    // Read sizes of matrices
    long int height1, width1, height2, width2;
    if (read_size_from_file(argv[1], mode, &height1, &width1)){
        printf("Cannot read size from file %s\n", argv[1]);
        return 1;
    }
    if (read_size_from_file(argv[2], mode, &height2, &width2)){
        printf("Cannot read size from file %s\n", argv[2]);
        return 1;
    }
    if (height1 != height2 || width1 != width2){
        puts("Matrices have different dimensions!");
        return 1;
    }
    int ret = 0;
    long int msize = height1 * width1;
    // alloc memory
    ThreadReaderMatrix m1 = {0, mode, 1, NULL, NULL}, m2 = {0, mode, 2, NULL, NULL};
    m1.data = (int*) malloc(msize * sizeof(int));
    if (m1.data == NULL){
        printf("Cannot allocate memory for matrix1 with size %ld\n", msize);
        ret = 1;
        goto end_main;
    }
    m2.data = (int*) malloc(msize * sizeof(int));
    if (m2.data == NULL){
        printf("Cannot allocate memory for matrix2 with size %ld\n", msize);
        ret = 1;
        goto end_main;
    }
    m1.filename = argv[1];
    m2.filename = argv[2];

    // alloc memory for n threads
    if (n > msize){
        n = msize;
        printf("Number of threads is greater than matrix size. Thread number reduced to %ld\n", msize);
    }
    pthread_t *summators = malloc(n * sizeof(pthread_t));
    if (summators == NULL){
        printf("Cannot allocate memory for %d summators\n", n);
        ret = 1;
        goto end_main;
    }

    // alloc memory for structures
    ThreadSummatorMatrix *sum_structs = malloc(n * sizeof(ThreadSummatorMatrix));
    if (sum_structs == NULL){
        printf("Cannot allocate memory for %d ThreadSummatorMatrix\n", n);
        ret = 1;
        goto end_main;
    }

    // read matrices
    pthread_t reader1, reader2;
    if(pthread_create(&reader1, NULL, reader_func, &m1)){
        puts("[Main] Cannot create reader 1\n");
        ret = 1;
    }
    if(pthread_create(&reader2, NULL, reader_func, &m2)){
        puts("[Main] Cannot create reader 2\n");
        ret = 1;
    }
    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);
    if (ret){
        puts("Ret");
        goto end_main;
    }
    if (m1.flag != 0xFF || m2.flag != 0xFF){
        puts("[Main] Matrices are not readed");
        ret = 1;
        goto end_main;
    }

    // sum matrices
    #ifdef TIME_OUTPUT
    clock_gettime(CLOCK_MONOTONIC_RAW, &sum_start);
    #endif
    int ssize = ((float)msize / n) - msize < EPS ? msize / n : msize / n + 1;
    for(int i = 0; i < n; i++){
        sum_structs[i].data1 = m1.data;
        sum_structs[i].data2 = m2.data;
        sum_structs[i].start_index = i * ssize;
        sum_structs[i].end_index = (i + 1) * ssize;
        if (sum_structs[i].end_index > msize){
            sum_structs[i].end_index = msize;
        }
        if (pthread_create(&summators[i], NULL, summator_func, &sum_structs[i])){
            printf("[Main] Cannot create summator %d\n", i);
            for(int j = 0; j < i; j++){
                pthread_join(summators[j], NULL);
            }
            ret = 1;
            goto end_main;
        }
    }
    for(int i = 0; i < n; i++){
        pthread_join(summators[i], NULL);
    }
    #ifdef TIME_OUTPUT
    clock_gettime(CLOCK_MONOTONIC_RAW, &sum_end);
    #endif

    // write matrix, use reader1 as writer
    ThreadWriterMatrix wm = {mode, height1, width1, argv[3], m1.data};
    if (pthread_create(&reader1, NULL, writer_func, &wm)){
        puts("[Main] Cannot create writer thread");
        ret = 1;
        goto end_main;
    }
    pthread_join(reader1, NULL);

end_main:
    if (m1.data != NULL){
        free(m1.data);
    }
    if (m2.data != NULL){
        free(m2.data);
    }
    if (summators != NULL){
        free(summators);
    }
    if (sum_structs != NULL){
        free(sum_structs);
    }
    #ifdef TIME_OUTPUT
    if (ret == 0){
        clock_gettime(CLOCK_MONOTONIC_RAW, &global_end);
        long global_time = (global_end.tv_sec - global_start.tv_sec) * 1000000 + (global_end.tv_nsec - global_start.tv_nsec) / 1000;
        long sum_time = (sum_end.tv_sec - sum_start.tv_sec) * 1000000 + (sum_end.tv_nsec - sum_start.tv_nsec) / 1000;
        printf("Matrix %ldx%ld with %ld threads: global time=%ld; sum time = %ld\n", height1, width1, n, global_time, sum_time);
    }
    #endif
    return ret;
}
