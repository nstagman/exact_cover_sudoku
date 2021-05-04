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
    col->right->left = col->left;
    col->left->right = col->right;
    for(vert_itr=col->down; vert_itr!=col; vert_itr=vert_itr->down){
        for(horiz_itr=vert_itr->right; horiz_itr!=vert_itr; horiz_itr=horiz_itr->right){
            horiz_itr->up->down = horiz_itr->down;
            horiz_itr->down->up = horiz_itr->up;
            column_of(horiz_itr)->count--;
        }
    }
}

void uncover(Node* n){
    Node* vert_itr, *horiz_itr, *col=column_of(n);
    for(vert_itr=col->up; vert_itr!=col; vert_itr=vert_itr->up){
        for(horiz_itr=vert_itr->left; horiz_itr!=vert_itr; horiz_itr=horiz_itr->left){
            horiz_itr->up->down = horiz_itr;
            horiz_itr->down->up = horiz_itr;
            column_of(horiz_itr)->count++;
        }
    }
    col->right->left = col;
    col->left->right = col;
}

bool alg_x_search(Matrix* matrix, lifo* solutions){
    if(matrix_is_empty(matrix)) {
        printf("FOUND\n");
        matrix->solved = true;
        return true;
    }
    static int i = 0;
    Node* selected_col = select_column(matrix);
    if(selected_col->count < 1) {
        // printf("fail\n");
        // printf("LEVEL: %d\n", i++);
        // printf("selected: %d\n", selected_col->col);
        // printf("count: %d\n", selected_col->col);
        // printf("n: ");
        // for(Node* itr=matrix->root->right; itr!=matrix->root; itr=itr->right){
        //     printf("%d ", itr->count);
        // }
        // printf("\n");
        // print_matrix(matrix);
        return false;
    }

    int j = 0;
    i++;
    Node* vert_itr = selected_col->down;
    Node* horiz_itr;
    while(vert_itr != selected_col && !matrix->solved){
        push_stack(solutions, matrix->rows[vert_itr->row]);
        horiz_itr = vert_itr;
        do{
            if(horiz_itr->col >= 0){
                cover(horiz_itr); 
            }
            horiz_itr = horiz_itr->right;
            printf("n: ");
            for(Node* itr=matrix->root->right; itr!=matrix->root; itr=itr->right){
                printf("%d ", itr->count);
            }
            printf("\n");
            print_matrix(matrix);
        }while(horiz_itr != vert_itr);
        printf("OUT\n");

        // printf("LEVEL: %d\n", i++);
        // printf("Iteration: %d\n", j);
        // printf("selected: %c\n", vert_itr->row+65);
        // printf("n: ");
        // for(Node* itr=matrix->root->right; itr!=matrix->root; itr=itr->right){
        //     printf("%d ", itr->count);
        // }
        // printf("\n");
        // print_matrix(matrix);

        if(!alg_x_search(matrix, solutions)) {
            pop_stack(solutions);
            // break; 
        }
        i--;
        // pop_stack(solutions);
        horiz_itr = vert_itr;
        do{
            if(horiz_itr->col >= 0){
                uncover(horiz_itr);
            }
            horiz_itr = horiz_itr->left;
        }while(horiz_itr != vert_itr);
        printf("AFTER UNCOVER\n");
        printf("n: ");
            for(Node* itr=matrix->root->right; itr!=matrix->root; itr=itr->right){
                printf("%d ", itr->count);
            }
            printf("\n");
            print_matrix(matrix);

        vert_itr = vert_itr->down;
        j++;
    }
    return matrix->solved;
}
