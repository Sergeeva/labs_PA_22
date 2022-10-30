#ifndef NON_BLOCKING_CYCLED_LIST_H
#define NON_BLOCKING_CYCLED_LIST_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct NonBlockingCycledListNode{
    int *data;
    struct NonBlockingCycledListNode *next;
    int id;
    atomic_bool is_del, is_add;
} NonBlockingCycledListNode;

typedef struct NonBlockingCycledList{
    atomic_int push_size, pop_size;
    int max_size;
    atomic_ullong head, tail;
    pthread_mutex_t mutex;
    pthread_cond_t data_cond, not_full;
} NonBlockingCycledList;

NonBlockingCycledList* create_non_blocking_cycled_list(int max_size);
void destroy_non_blocking_cycled_list(NonBlockingCycledList *list);
int non_blocking_cycled_list_push(NonBlockingCycledList *list, int *data);
int* non_blocking_cycled_list_pop(NonBlockingCycledList *list);

#endif // NON_BLOCKING_CYCLED_LIST_H
