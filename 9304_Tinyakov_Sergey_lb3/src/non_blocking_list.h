#ifndef NON_BLOCKING_LIST_H
#define NON_BLOCKING_LIST_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct NodePointer{
    struct NonBlockingListNode *pointer;
    uint32_t counter;
} NodePointer;

typedef struct PassedCounter{
    uint32_t passed;
    uint32_t thread_count;
} PassedCounter;

typedef struct NonBlockingListNode{
    _Atomic NodePointer next;
    int *data;
    _Atomic PassedCounter passed;
    atomic_bool is_del;
} NonBlockingListNode;

typedef struct NonBlockingList{
    atomic_int push_size, pop_size;
    int max_size;
    _Atomic NodePointer head;
    atomic_ullong tail;
    pthread_mutex_t mutex;
    pthread_cond_t data_cond, not_full;
} NonBlockingList;

NonBlockingList* create_non_blocking_list(int max_size);
void destroy_non_blocking_list(NonBlockingList *list);
int non_blocking_list_push(NonBlockingList *list, int *data);
int* non_blocking_list_pop(NonBlockingList *list);

#endif // NON_BLOCKING_LIST_H
