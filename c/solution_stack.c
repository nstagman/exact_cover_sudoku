#include <stdlib.h>
#include "solution_stack.h"


solution_stack* create_stack(){
    solution_stack* stack = malloc(sizeof(solution_stack));
    stack->head  = NULL;
    stack->count = 0;
    return stack;
}

void push_stack(solution_stack* stack, void* data){
    solution_node* node = malloc(sizeof(solution_node));
    node->data = data;
    node->next = stack->head;
    stack->head = node;
    stack->count++;
}

void pop_stack(solution_stack* stack){
    if(stack->count < 1) { return; }
    solution_node* tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp);
    stack->count--;
}

void delete_stack(solution_stack* stack){
    if(stack->head != NULL){
        solution_node* itr  = stack->head;
        solution_node* prev = itr;
        while(itr->next != NULL){
            itr = itr->next;
            free(prev);
            prev = itr;
        }
        free(itr);
    }
    free(stack);
}