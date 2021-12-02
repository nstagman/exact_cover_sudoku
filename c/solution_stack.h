#ifndef LIFO_H
#define LIFO_H

//Simple linked node based stack for holding solution to algorithm x

typedef struct _solution_node solution_node;
typedef struct _solution_stack solution_stack;

struct _solution_node {
    void* data;
    solution_node* next;
};

struct _solution_stack {
    solution_node* head;
    int count;
};

solution_stack* create_stack();
void push_stack(solution_stack* stack, void* data);
void pop_stack(solution_stack* stack);
void delete_stack(solution_stack* stack);

#endif