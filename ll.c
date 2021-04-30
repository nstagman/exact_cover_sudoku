#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ll.h"


List* create_list(){
    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->num_nodes = 0;
    return list;
}

void list_append(List* list, void* data){
    l_node* node = malloc(sizeof(l_node));
    node->data = data;
    list->num_nodes++;
    if(list->num_nodes == 1){
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->prev->next = node;
    node->prev = list->tail;
    node->next = NULL;
    list->tail = node;
}

void list_pop(List* list){
    assert(list->head != NULL && list->tail != NULL);
    if(list->num_nodes == 1){
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
    }
    else{
        l_node* n = list->tail;
        list->tail->prev->next = NULL;
        list->tail = list->tail->prev;
        free(n);
    }
    list->num_nodes--;
}

void delete_list(List* list){
    if(list->head != NULL){
        l_node* itr = list->head;
        l_node* prev = list->head;
        while(itr->next != NULL){
            itr = itr->next;
            free(prev);
            prev = itr;
        }
        free(itr);
    }
    free(list);
}