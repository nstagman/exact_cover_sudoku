#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "algx.h"


Node* select_column(Matrix* matrix){
    Node* itr = matrix->col_head;
    Node* min_node = itr;
    int min_count = itr->count;
    while(itr->right != NULL){
        itr = itr->right;
        if(itr->count < min_count){
            min_node = itr;
        }
    }
    return min_node;
}

void alg_x(Matrix matrix){
    if(matrix.col_head == NULL){
        return;
    }
    Node* col = select_column(&matrix);


}