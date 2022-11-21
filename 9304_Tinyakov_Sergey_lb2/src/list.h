#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <pthread.h>

struct Node{
    int *data;
    struct Node *next;
};

struct List{
    unsigned int size, max_size;
    struct Node *head, *tail;
    pthread_mutex_t mutex;
    pthread_cond_t data_cond, not_full;
};

typedef struct Node Node;
typedef struct List List;

List* create_list(unsigned int max_size);

void destroy_list(List *list);

void list_add_to_tail(List *list, Node *node);

Node* list_delete_from_head(List *list);

int list_push(List *list, int *data);

int* list_pop(List *list);

unsigned int list_cur_size(List *list);

#endif // LIST_H
