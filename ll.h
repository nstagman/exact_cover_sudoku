#ifndef LL_H
#define LL_H

typedef struct n l_node;
typedef struct list List;

List* create_list(void);
void list_append(List*, void*);
void list_pop(List*);
void delete_list(List*);

struct n {
    void* data;
    struct n* next;
    struct n* prev;
};

struct list {
    l_node* head;
    l_node* tail;
    int num_nodes;
};

#endif