#include <stdlib.h>
#include <stdio.h>
#include "algx.h"

//return column header of column with least number of 1's in matrix
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

//cover a column of node n in manner described for dancing links algorithm
void cover(Node* n){
    Node* col = column_of(n);
    //unlink left and right neighbors of col from col
    col->right->left = col->left;
    col->left->right = col->right;
    //iterate through each 1 in col col top to bottom
    for(Node* vert_itr=col->down; vert_itr!=col; vert_itr=vert_itr->down){
        //iterate through row left to right
        //for each 1 in this row, unlink top and bottom neighbors and reduce count of that column
        for(Node* horiz_itr=vert_itr->right; horiz_itr!=vert_itr; horiz_itr=horiz_itr->right){
            horiz_itr->up->down = horiz_itr->down;
            horiz_itr->down->up = horiz_itr->up;
            column_of(horiz_itr)->count--;
        }
    }
}

//uncover a column of node n in manner described for dancing links algorithm
void uncover(Node* n){
    Node* col=column_of(n);
    //iterate through each 1 in col bottom to top
    for(Node* vert_itr=col->up; vert_itr!=col; vert_itr=vert_itr->up){
        //iterate through row right to left
        //for each 1 in this row, relink top and bottom neighbors and increment count of that column
        for(Node* horiz_itr=vert_itr->left; horiz_itr!=vert_itr; horiz_itr=horiz_itr->left){
            horiz_itr->up->down = horiz_itr;
            horiz_itr->down->up = horiz_itr;
            column_of(horiz_itr)->count++;
        }
    }
    //relink left and right neighbors of col to col
    col->right->left = col;
    col->left->right = col;
}

//search the toroidal matrix structure for an exact cover
//returns true if exact cover is found, false otherwise
//solutions LIFO will contain all rows making up the solution
bool alg_x_search(Matrix* matrix, lifo* solutions){
    //if matrix is empty then an exact cover exists, return true
    if(matrix_is_empty(matrix)) {
        matrix->solved = true;
        return true;
    }
    //select the column with least number of 1's
    Node* selected_col = select_column(matrix);
    //if selected column has 0 ones, then this branch has failed
    if(selected_col->count < 1) { return false; }

    Node* vert_itr=selected_col->down;
    Node* horiz_itr;
    //iterate down from selected column head
    while(vert_itr != selected_col && !matrix->solved){
        //add selected row to solutions LIFO
        push_stack(solutions, matrix->rows[vert_itr->row]);

        horiz_itr = vert_itr;
        //iterate right from vertical iterator, cover each column
        do{
            if(horiz_itr->col >= 0){ cover(horiz_itr); } //skip column of row headers
        }while((horiz_itr = horiz_itr->right) != vert_itr);

        //search this matrix again after covering
        //if solution not found on this branch, pop row from solutions LIFO
        if(!alg_x_search(matrix, solutions)) { pop_stack(solutions); }

        horiz_itr = vert_itr->left;
        //iterate left from the last column that was covered, uncover each column
        do{
            if(horiz_itr->col >= 0){ uncover(horiz_itr); } //skip column of row headers
        }while((horiz_itr = horiz_itr->left) != vert_itr->left);

        vert_itr = vert_itr->down;
    }
    return matrix->solved;
}



// printf("n: ");
// for(Node* itr=matrix->root->right; itr!=matrix->root; itr=itr->right){
//     printf("%d ", itr->count);
// }
// printf("\n");
// print_matrix(matrix);

// bool alg_x_search(Matrix* matrix, lifo* solutions){
//     //if matrix is empty then an exact cover exists, return true
//     if(matrix_is_empty(matrix)) {
//         matrix->solved = true;
//         return true;
//     }
//     //select the column with least number of 1's
//     Node* selected_col = select_column(matrix);
//     //if selected column has 0 ones, then this branch has failed
//     if(selected_col->count < 1) { return false; }

//     Node* horiz_itr, *vert_itr=selected_col->down;
//     //iterate down from selected column head
//     while(vert_itr != selected_col && !matrix->solved){
//         //add selected row to solutions LIFO
//         push_stack(solutions, matrix->rows[vert_itr->row]);

//         horiz_itr = vert_itr;
//         //iterate right from vertical iterator, cover each column
//         do{
//             if(horiz_itr->col >= 0){ cover(horiz_itr); } //skip column of row headers
//         }while((horiz_itr = horiz_itr->right) != vert_itr);

//         //search this matrix again after covering
//         //if solution not found on this branch, pop row from solutions LIFO
//         if(!alg_x_search(matrix, solutions)) { pop_stack(solutions); }

//         horiz_itr = vert_itr->left;
//         //iterate left from the last column that was covered, uncover each column
//         do{
//             if(horiz_itr->col >= 0){ uncover(horiz_itr); } //skip column of row headers
//         }while((horiz_itr = horiz_itr->left) != vert_itr->left);

//         vert_itr = vert_itr->down;
//     }
//     return matrix->solved;
// }