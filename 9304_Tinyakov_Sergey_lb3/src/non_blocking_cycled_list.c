#include "non_blocking_cycled_list.h"

int __alloc_nodes(NonBlockingCycledList *list){
    NonBlockingCycledListNode *new_node, *cur;
    list->head = malloc(sizeof(NonBlockingCycledListNode));
    if(!list->head) return 1;
    cur = (NonBlockingCycledListNode*) list->head;
    cur->id = 0;
    atomic_flag_clear(&cur->is_add);
    atomic_flag_test_and_set(&cur->is_del);
    for(unsigned int i = 1; i < 2 * list->max_size; i++){
        new_node = malloc(sizeof(NonBlockingCycledListNode));
        if(!new_node) goto __alloc_node_error;
        new_node->id = i;
        atomic_flag_clear(&new_node->is_add);
        atomic_flag_test_and_set(&new_node->is_del);
        cur->next = new_node;
        cur = cur->next;
    }
    cur->next = list->head;
    list->tail = list->head;
    return 0;
__alloc_node_error:
    while(list->head){
        new_node = ((NonBlockingCycledListNode*) list->head)->next;
        free(list->head);
        list->head = new_node;
    }
    return 1;
}

NonBlockingCycledList* create_non_blocking_cycled_list(int max_size){
    NonBlockingCycledList *list = malloc(sizeof(NonBlockingCycledList));
    if (list != NULL){
        list->max_size = max_size;
        list->push_size = list->pop_size = 0;
        list->head = list->tail = NULL;
        if (pthread_mutex_init(&list->mutex, NULL)){
            free(list);
            list = NULL;
        }
        if (pthread_cond_init(&list->data_cond, NULL)){
            free(list);
            list = NULL;
        }
        if (pthread_cond_init(&list->not_full, NULL)){
            free(list);
            list = NULL;
        }
        if (__alloc_nodes(list)){
            free(list);
            list = NULL;
        }
    }
    return list;
}

void destroy_non_blocking_cycled_list(NonBlockingCycledList *list){
    NonBlockingCycledListNode *node = list->head, *tmp;
    do{
        tmp = node->next;
        free(node);
        node = tmp;
    }while(node != list->head);
    free(list);
}

void __update_pointer(atomic_ullong *pointer, NonBlockingCycledListNode *new_pointer, unsigned int max_size){
    NonBlockingCycledListNode *node;
    do{
        node = (NonBlockingCycledListNode*) atomic_load(pointer);
        if (new_pointer->id < max_size && new_pointer->id <= node->id)
            break; // someone updated the tail to far away
        if (new_pointer->id >= max_size && (node->id < max_size || new_pointer->id <= node->id))
            break; // someone updated the tail to far away
    }while(!atomic_compare_exchange_strong(pointer, &node, new_pointer));
}

void __non_blocking_cycled_list_add_to_tail(NonBlockingCycledList *list, int *data){
    NonBlockingCycledListNode *node = atomic_load(&list->tail);
    while(1){
        if(!atomic_flag_test_and_set(&node->is_add)){
            // Add data to this node
            break;
        }
        node = node->next;
    };
    node->data = data;
    __update_pointer(&list->tail, node->next, list->max_size);
    atomic_flag_clear(&node->is_del);
}

int* __non_blocking_cycled_list_delete_from_head(NonBlockingCycledList *list){
    NonBlockingCycledListNode *node = atomic_load(&list->head);
    while(1){
        if(!atomic_flag_test_and_set(&node->is_del))
            break; // read data from this node
        node = node->next;
    }
    int *data = node->data;
    __update_pointer(&list->head, node->next, list->max_size);
    atomic_flag_clear(&node->is_add);
    return data;
}

int non_blocking_cycled_list_push(NonBlockingCycledList *list, int *data){
    while(atomic_fetch_add(&list->push_size, 1) >= list->max_size){
        if (atomic_fetch_add(&list->push_size, -1) <= list->max_size)
            continue;
        pthread_mutex_lock(&list->mutex);
        pthread_cond_wait(&list->not_full, &list->mutex);
        pthread_mutex_unlock(&list->mutex);
    }
    __non_blocking_cycled_list_add_to_tail(list, data);
    atomic_fetch_add(&list->pop_size, 1);
    pthread_cond_signal(&list->data_cond);
}

int* non_blocking_cycled_list_pop(NonBlockingCycledList *list){
    while(atomic_fetch_add(&list->pop_size, -1) <= 0){
        if (atomic_fetch_add(&list->pop_size, 1) >= 0)
            continue;
        pthread_mutex_lock(&list->mutex);
        pthread_cond_wait(&list->data_cond, &list->mutex);
        pthread_mutex_unlock(&list->mutex);
    }
    int *data = __non_blocking_cycled_list_delete_from_head(list);
    atomic_fetch_add(&list->push_size, -1);
    pthread_cond_signal(&list->not_full);
    return data;
}
