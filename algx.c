#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "algx.h"


int matrix_empty(Matrix* matrix){
    return matrix->root->right == matrix->root;
}

Node* select_column(Matrix* matrix){
    if(matrix_empty(matrix)){ return matrix->root; }
    Node* itr = matrix->root->right;
    Node* min_node = itr;
    int min_count = itr->count;
    while(itr->right != matrix->root){
        itr = itr->right;
        if(itr->count < min_count){
            min_node = itr;
            min_count = itr->count;
        }
    }
    return min_node;
}

void alg_x_search(Matrix matrix){
    if(matrix_empty(&matrix)){ return; }
    Node* col = select_column(&matrix);
    printf("col: %d, count: %d\n", col->col, col->count);
}
