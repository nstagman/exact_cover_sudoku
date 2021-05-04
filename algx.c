#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "algx.h"


Node* select_column(Matrix* matrix){
    if(matrix_is_empty(matrix)) { return matrix->root; }
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
    Node* vert_itr, *horiz_itr, *col=column_of(n);
    if(col==n->matrix->root){
        n->up->down = n->down;
        n->down->up = n->up;
        return;
    }
    col->right->left = col->left;
    col->left->right = col->right;
    for(vert_itr=col->down; vert_itr!=col; vert_itr=vert_itr->down){
        for(horiz_itr=vert_itr->right; horiz_itr!=vert_itr; horiz_itr=horiz_itr->right){
            horiz_itr->up->down = horiz_itr->down;
            horiz_itr->down->up = horiz_itr->up;
            // column_of(horiz_itr)->count--;
        }
    }
}

void uncover(Node* n){
    Node* vert_itr, *horiz_itr, *col=column_of(n);
    if(col==n->matrix->root){
        n->up->down = n;
        n->down->up = n;
        return;
    }
    for(vert_itr=col->up; vert_itr!=col; vert_itr=vert_itr->up){
        for(horiz_itr=vert_itr->left; horiz_itr!=vert_itr; horiz_itr=horiz_itr->left){
            horiz_itr->up->down = horiz_itr;
            horiz_itr->down->up = horiz_itr;
            // column_of(horiz_itr)->count++;
        }
    }
    col->right->left = col;
    col->left->right = col;
}

bool alg_x_search(Matrix* matrix){
    if(matrix_is_empty(matrix)) { return true; }
    static int i = 0;
    Node* selected_col = select_column(matrix);
    if(selected_col->count < 1) { return false; }
    
    printf("LEVELS: %d\n", i++);
    printf("selected: %d\n", selected_col->col);
    Node* itr;
    for(itr=matrix->root->right; itr!=matrix->root; itr=itr->right){
        printf("%d ", itr->count);
    }
    printf("\n");
    print_matrix(matrix);

    Node* vert_itr = selected_col->down;
    Node* horiz_itr;
    while(vert_itr != selected_col){
        //push list
        cover(vert_itr);
        // horiz_itr = vert_itr;
        // do{
        //     cover(horiz_itr);
        //     horiz_itr = horiz_itr->right;
        //     print_matrix(matrix);
        // }while(horiz_itr != vert_itr);

        if(alg_x_search(matrix)) {
            break; 
        }
        //pop list
        uncover(vert_itr);
        // horiz_itr = vert_itr;
        // do{
        //     uncover(horiz_itr);
        //     horiz_itr = horiz_itr->right;
        // }while(horiz_itr != vert_itr);
        vert_itr = vert_itr->down;
    }
    return false;
}
