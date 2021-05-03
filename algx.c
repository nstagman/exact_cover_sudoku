#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include "algx.h"


bool matrix_is_empty(Matrix* matrix){
    return matrix->root->right == matrix->root;
}

Node* select_column(Matrix* matrix){
    if(matrix_is_empty(matrix)){ return matrix->root; }
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

void cover(Node* n){
    Node* vert_itr, *horiz_itr, *col=n->matrix->cols[n->col];
    col->right->left = col->left;
    col->left->right = col->right;
    for(vert_itr=col->down; vert_itr!=col; vert_itr=vert_itr->down){
        for(horiz_itr=vert_itr->right; horiz_itr!=vert_itr; horiz_itr=horiz_itr->right){
            horiz_itr->up->down = horiz_itr->down;
            horiz_itr->down->up = horiz_itr->up;
        }
    }
}

void uncover(Node* n){
    Node* vert_itr, *horiz_itr, *col=n->matrix->cols[n->col];
    for(vert_itr=col->up; vert_itr!=col; vert_itr=vert_itr->up){
        for(horiz_itr=vert_itr->left; horiz_itr!=vert_itr; horiz_itr=horiz_itr->left){
            horiz_itr->up->down = horiz_itr;
            horiz_itr->down->up = horiz_itr;
        }
    }
    col->right->left = col;
    col->left->right = col;
}

void alg_x_search(Matrix matrix){
    if(matrix_is_empty(&matrix)){ return; }
    Node* col = select_column(&matrix);
    printf("col: %d, count: %d\n", col->col, col->count);
}
