#include "summator.h"

#define EPS 0.0001

void *summator_main_thread(void *arg){
    SummatorMainThreadInfo *info = (SummatorMainThreadInfo*) arg;
    long msize = info->matrix_height * info->matrix_width;
    long ssize = ((float)msize / info->number_of_threads) - msize < EPS ? msize / info->number_of_threads : msize / info->number_of_threads + 1;
    int current_waiters = 0;
    char cur_iter = 0;
    SummatorThreadInfo *infos = malloc(info->number_of_threads * sizeof(SummatorThreadInfo));
    pthread_t *summators = malloc(info->number_of_threads * sizeof(pthread_t));
    pthread_cond_t wait_for_sum, waiters;
    pthread_mutex_t mutex;
    pthread_cond_init(&wait_for_sum, NULL);
    pthread_cond_init(&waiters, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < info->number_of_threads; i++){
        infos[i].start_index = i * ssize;
        infos[i].end_index = (i + 1) * ssize;
        if (infos[i].end_index > msize){
            infos[i].end_index = msize;
        }
        infos[i].wait = &waiters;
        infos[i].end_sum = &wait_for_sum;
        infos[i].number_of_threads = info->number_of_threads;
        infos[i].waiters = &current_waiters;
        infos[i].mutex = &mutex;
        infos[i].is_running = info->sum_is_running;
        infos[i].cur_iter = &cur_iter;
        pthread_create(&summators[i], NULL, summator_thread, &infos[i]);
    }
    pthread_cond_wait(&wait_for_sum, &mutex);
    pthread_mutex_unlock(&mutex);
    while(1){
        if (list_cur_size(info->gen_queue) == 0 && !atomic_load(info->is_running)){
            break;
        }
        int *new_data = list_pop(info->gen_queue);
        if (new_data == NULL) continue;
        for(int i = 0; i < info->number_of_threads; i++){
            infos[i].matrix1 = new_data;
            infos[i].matrix2 = new_data + msize;
        }
        pthread_mutex_lock(&mutex);
        current_waiters = 0;
        cur_iter = !cur_iter;
        pthread_cond_broadcast(&waiters);
        pthread_cond_wait(&wait_for_sum, &mutex);
        pthread_mutex_unlock(&mutex);
        list_push(info->writer_queue, new_data);
    }
    atomic_store(info->sum_is_running, 0);
    current_waiters = 0;
    cur_iter = !cur_iter;
    pthread_cond_broadcast(&waiters);
    for(int i = 0; i < info->number_of_threads; i++){
        pthread_join(summators[i], NULL);
    }
}

void *summator_thread(void *arg){
    SummatorThreadInfo *info = (SummatorThreadInfo*) arg;
    char prev_iter;
    while(1){
        pthread_mutex_lock(info->mutex);
        prev_iter = *info->cur_iter;
        if (++(*info->waiters) >= info->number_of_threads){
            pthread_cond_signal(info->end_sum);
            while(prev_iter == *info->cur_iter) pthread_cond_wait(info->wait, info->mutex);
        }else{
            while(prev_iter == *info->cur_iter) pthread_cond_wait(info->wait, info->mutex);
        }
        pthread_mutex_unlock(info->mutex);
        if (!atomic_load(info->is_running)) break;
        sum_elements(info->matrix1, info->matrix2, info->start_index, info->end_index);
    }
}
