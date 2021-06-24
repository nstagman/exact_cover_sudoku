#ifndef LIFO_H
#define LIFO_H
#include "dlinks_matrix.h"

typedef struct lifo_node lifo_node;
typedef struct lifo lifo;

lifo* create_stack(void);
void push_stack(lifo*, Node*);
void pop_stack(lifo*);
lifo_node* peek_stack(lifo*);
void delete_stack(lifo*);

struct lifo_node {
    Node* data;
    struct lifo_node* next;
};

struct lifo {
    lifo_node* head;
    int count;
};

#endif