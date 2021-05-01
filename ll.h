#ifndef LL_H
#define LL_H

typedef struct ll_node ll_node;
typedef struct l_list l_list;

l_list* create_list(void);
void list_append(l_list*, void*);
void list_pop(l_list*);
void delete_list(l_list*);

struct ll_node {
    void* data;
    struct ll_node* next;
    struct ll_node* prev;
};

struct l_list {
    ll_node* head;
    ll_node* tail;
    int num_nodes;
};

#endif