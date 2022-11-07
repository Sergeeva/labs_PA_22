#include "list.h"

List* create_list(unsigned int max_size){
    List *list = malloc(sizeof(List));
    if (list != NULL){
        list->head = list->tail = NULL;
        list->size = 0;
        list->max_size = max_size;
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

void destroy_list(List *list){
    Node *node = list->head, *next = NULL;
    while(node){
        next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

void list_add_to_tail(List *list, Node *node){
    if (list == NULL || node == NULL) return;
    if (list->tail == NULL){
        // empty list
        list->head = list->tail = node;
        node->next = NULL;
    }else{
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

Node* list_delete_from_head(List *list){
    Node *node = NULL;
    if (list == NULL || list->head == NULL) return NULL;
    node = list->head;
    list->head = list->head->next;
    if (list->head == NULL) list->tail = NULL;
    list->size--;
    return node;
}

int list_push(List *list, int *data){
    Node *node = malloc(sizeof(Node));
    if (node == NULL) return 1;
    node->data = data;
    node->next = NULL;
    pthread_mutex_lock(&list->mutex);
    while (list->size >= list->max_size){
        pthread_cond_wait(&list->not_full, &list->mutex);
    }
    list_add_to_tail(list, node);
    pthread_cond_signal(&list->data_cond);
    pthread_mutex_unlock(&list->mutex);
    return 0;
}

int* list_pop(List *list){
    pthread_mutex_lock(&list->mutex);
    while(list->size == 0){
        pthread_cond_wait(&list->data_cond, &list->mutex);
    }
    Node *node = list_delete_from_head(list);
    pthread_cond_signal(&list->not_full);
    pthread_mutex_unlock(&list->mutex);
    if (node == NULL) return NULL;
    int *data = node->data;
    free(node);
    return data;
}

unsigned int list_cur_size(List *list){
    unsigned int ret;
    pthread_mutex_lock(&list->mutex);
    ret = list->size;
    pthread_mutex_unlock(&list->mutex);
    return ret;
}
