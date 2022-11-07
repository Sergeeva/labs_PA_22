#include "summator.h"

void *summator_main_thread(void *arg){
    SummatorMainThreadInfo *info = (SummatorMainThreadInfo*) arg;
    long msize = info->matrix_height * info->matrix_width;
    long cur_iter_count = 0;
    while(1){
        if(cur_iter_count >= info->iter_count) break;
        int *new_data = (*info->queue_pop_func)(info->gen_queue);
        if (new_data == NULL) continue;
        sum_elements(new_data, new_data + msize, 0, 2 * msize);
        (*info->queue_push_func)(info->writer_queue, new_data);
        cur_iter_count++;
    }
    atomic_flag_test_and_set(&info->sum_is_running);
}
