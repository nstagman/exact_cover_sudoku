#include <stdlib.h>
#include "lifo.h"


lifo* create_stack(){
    lifo* stack = malloc(sizeof(lifo));
    stack->head = NULL;
    stack->count = 0;
    return stack;
}

void push_stack(lifo* stack, Node* data){
    lifo_node* node = malloc(sizeof(lifo_node));
    node->data = data;
    node->next = stack->head;
    stack->head = node;
    stack->count++;
}

void pop_stack(lifo* stack){
    if(stack->count < 1) { return; }
    lifo_node* tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp);
    stack->count--;
}

lifo_node* peek_stack(lifo* stack){
    return stack->head;
}

void delete_stack(lifo* stack){
    if(stack->head != NULL){
        lifo_node* itr = stack->head;
        lifo_node* prev = itr;
        while(itr->next != NULL){
            itr = itr->next;
            free(prev);
            prev = itr;
        }
        free(itr);
    }
    free(stack);
}