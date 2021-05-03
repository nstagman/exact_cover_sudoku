#ifndef LIFO_H
#define LIFO_H

typedef struct lifo_node lifo_node;
typedef struct lifo lifo;

lifo* create_stack(void);
void push_stack(lifo*, void*);
void pop_stack(lifo*);
lifo_node* peek_stack(lifo*);
void delete_stack(lifo*);

struct lifo_node {
    void* data;
    struct lifo_node* next;
};

struct lifo {
    lifo_node* head;
    int count;
};

#endif