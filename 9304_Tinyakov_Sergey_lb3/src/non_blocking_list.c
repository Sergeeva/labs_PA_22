#include "non_blocking_list.h"

NonBlockingList* create_non_blocking_list(int max_size){
    NonBlockingList *list = malloc(sizeof(NonBlockingList));
    if (list != NULL){
        list->max_size = max_size;
        list->push_size = list->pop_size = 0;
        list->head.counter = 0;
        list->head.pointer = list->tail = NULL;
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
    }
    return list;
}

void destroy_non_blocking_list(NonBlockingList *list){
    if(atomic_load(&list->head).pointer == NULL && atomic_load(&list->tail) != NULL){
        free(list->tail);
    }
    NonBlockingListNode *cur = atomic_load(&list->head).pointer, *new_cur;
    while(cur){
        new_cur = cur->next.pointer;
        free(cur);
        cur = new_cur;
    }
    free(list);
}

NodePointer __get_pointer(_Atomic NodePointer *node_pointer){
    _Atomic NodePointer pointer, old_pointer;
    do{
        old_pointer = atomic_load(node_pointer);
        pointer = old_pointer;
        pointer.counter++;
    }while(!atomic_compare_exchange_strong(node_pointer, &old_pointer, pointer));
    return pointer;
}

NodePointer __set_pointer(_Atomic NodePointer *node_pointer, NodePointer new_node_pointer, NonBlockingListNode* old_pointer){
    NodePointer old_node_pointer;
    do{
        old_node_pointer = atomic_load(node_pointer);
        if (old_node_pointer.pointer != old_pointer)
            break; // the head is already changed
    }while(!atomic_compare_exchange_strong(node_pointer, &old_node_pointer, new_node_pointer));
    return old_node_pointer;
}

NodePointer __set_pointer_always(_Atomic NodePointer *node_pointer, NodePointer new_node_pointer){
    NodePointer old_node_pointer;
    do{
        old_node_pointer = atomic_load(node_pointer);
    }while(!atomic_compare_exchange_strong(node_pointer, &old_node_pointer, new_node_pointer));
    return old_node_pointer;
}

PassedCounter __inc_and_set_counter(_Atomic PassedCounter *counter, uint32_t thread_count){
    PassedCounter old_counter, new_counter;
    do{
        old_counter = new_counter = atomic_load(counter);
        new_counter.passed++;
        if (thread_count != 0) new_counter.thread_count = thread_count;
    }while(!atomic_compare_exchange_strong(counter, &old_counter, new_counter));
    return new_counter;
}

void __inc_and_check_counter(NonBlockingListNode *node, uint32_t thread_count){
    PassedCounter node_counter = __inc_and_set_counter(&node->passed, thread_count);
    if (node_counter.passed == node_counter.thread_count)
        free(node);
}

void __add_to_tail(NonBlockingList *list, int *data){
    NodePointer head, new_head, new_next, old_next;
    PassedCounter node_counter;
    NonBlockingListNode *old_tail, *node = malloc(sizeof(NonBlockingListNode));
    if (!node) return;
    node->data = data;
    node->next.pointer = NULL;
    node->next.counter = 0;
    node->passed.passed = 0;
    node->passed.thread_count = 0;
    node->is_del = 0;
    new_head.counter = 0;
    new_head.pointer = node;
    new_next.counter = 0;
    new_next.pointer = node;
    do{
        old_tail = (NonBlockingListNode*) atomic_load(&list->tail);
    }while(!atomic_compare_exchange_strong(&list->tail, &old_tail, node));
    if (!old_tail){
        atomic_store(&list->head, new_head);
        return;
    }
    do{
        old_next = atomic_load(&old_tail->next);
    }while(!atomic_compare_exchange_strong(&old_tail->next, &old_next, new_next));
    if (old_next.counter != 0)
    {
        // Someone had already readed the NULL
        head = __set_pointer_always(&list->head, new_head);
        // If head.pointer is NULL -- someone already cleared the head before us, do not set counter and use old_tail
        __inc_and_check_counter(head.pointer ? head.pointer : old_tail, head.pointer ? head.counter + 1 : 0);
    }
}

int* __delete_from_head(NonBlockingList *list){
    NodePointer pointer, new_pointer = {.counter = 0, .pointer = NULL}, tail, next, new_tail;
    NonBlockingListNode *node, new_head;
    int *data;
    int add;
    PassedCounter counter;
    while (1){
        do{
            pointer = __get_pointer(&list->head);
        }while(pointer.pointer == NULL);
        node = pointer.pointer;
        if (!atomic_flag_test_and_set(&node->is_del)){
            // I am the first
            data = node->data;
            next = __get_pointer(&node->next);
            add = next.pointer ? 0 : 1;
            new_pointer.pointer = next.pointer;
            pointer = __set_pointer(&list->head, new_pointer, node);
            __inc_and_check_counter(node, pointer.pointer == node ? pointer.counter + add : 0);
            return data;
        }else{
            __inc_and_check_counter(node, 0);
        }
    }
}

int non_blocking_list_push(NonBlockingList *list, int *data){
    while(atomic_fetch_add(&list->push_size, 1) >= list->max_size){
        if (atomic_fetch_add(&list->push_size, -1) <= list->max_size)
            continue;
        pthread_mutex_lock(&list->mutex);
        pthread_cond_wait(&list->not_full, &list->mutex);
        pthread_mutex_unlock(&list->mutex);
    }
    __add_to_tail(list, data);
    atomic_fetch_add(&list->pop_size, 1);
    pthread_cond_signal(&list->data_cond);
}

int* non_blocking_list_pop(NonBlockingList *list){
    while(atomic_fetch_add(&list->pop_size, -1) <= 0){
        if (atomic_fetch_add(&list->pop_size, 1) >= 0)
            continue;
        pthread_mutex_lock(&list->mutex);
        pthread_cond_wait(&list->data_cond, &list->mutex);
        pthread_mutex_unlock(&list->mutex);
    }
    int *data = __delete_from_head(list);
    atomic_fetch_add(&list->push_size, -1);
    pthread_cond_signal(&list->not_full);
    return data;
}
