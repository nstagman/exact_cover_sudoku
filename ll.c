#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ll.h"


l_list* create_list(){
    l_list* list = malloc(sizeof(l_list));
    list->head = NULL;
    list->tail = NULL;
    list->num_nodes = 0;
    return list;
}

void list_append(l_list* list, void* data){
    ll_node* node = malloc(sizeof(ll_node));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    list->num_nodes++;
    if(list->num_nodes == 1){
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
}

void list_pop(l_list* list){
    assert(list->head != NULL && list->tail != NULL);
    if(list->num_nodes == 1){
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
    }
    else{
        ll_node* n = list->tail;
        list->tail->prev->next = NULL;
        list->tail = list->tail->prev;
        free(n);
    }
    list->num_nodes--;
}

void delete_list(l_list* list){
    if(list->head != NULL){
        ll_node* itr = list->head;
        while(itr->next != NULL){
            itr = itr->next;
            free(itr->prev);
        }
        free(itr);
    }
    free(list);
}