#include "generation.h"

void *generation_thread(void *arg){
    GenerationThreadInfo *info = (GenerationThreadInfo*) arg;
    long msize2x = info->matrix_height * info->matrix_width * 2;
    srand(time(NULL));
    for(unsigned long i = 0; i < info->iter_count; i++){
        int *matrix2x = NULL;
        while(matrix2x == NULL){
            matrix2x = malloc(msize2x * sizeof(int));
        }
        for(long k = 0; k < msize2x; k++){
            matrix2x[k] = rand() % (2 * MAX_ABS_VALUE) - MAX_ABS_VALUE;
        }
        (*info->queue_push_func)(info->gen_queue, matrix2x);
    }
    atomic_store(info->is_running, 0);
}
