#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dlinks_matrix.h"

void init_matrix(Matrix*);
Node* create_node(Matrix*, int, int, int, int, int);


//Returns initialized Matrix struct of size [num_rows x num_cols]
Matrix* create_matrix(int num_rows, int num_cols){
    Matrix* mx = malloc(sizeof(Matrix));
    mx->rows = malloc(sizeof(Node*) * num_rows);
    mx->cols = malloc(sizeof(Node*) * num_cols);
    mx->num_rows = num_rows;
    mx->num_cols = num_cols;
    init_matrix(mx);
    return mx;
}

//Insert node into matrix at row, col
//If a node already exists at this position, change value
void insert_node(Matrix* mx, int row, int col, int value){
    assert(row >= 0 && col >= 0 && row < mx->num_rows && col < mx->num_cols);
    
    //ceate new node
    Node* new_node = create_node(mx, row, col, 1, value, -1);

    //iterate through row to find correct placement of node in row
    Node* itr_n = mx->rows[row];
    while(itr_n->right != NULL && itr_n->right->col < col){
        itr_n = itr_n->right;
    }
    if(itr_n->right != NULL && itr_n->right->col == col){
        itr_n->right->value = value;
        return;
    }
    //reassign left and right pointers
    new_node->right = itr_n->right;
    new_node->left = itr_n;
    itr_n->right = new_node;
    if(new_node->right != NULL){
        new_node->right->left = new_node;
    }

    //iterate through column to find correct placement of node in column
    itr_n = mx->cols[col];
    while(itr_n->down != NULL && itr_n->down->row < row){
        itr_n = itr_n->down;
    }
    //reassign up and down pointers
    new_node->down = itr_n->down;
    new_node->up = itr_n;
    itr_n->down = new_node;
    if(new_node->down != NULL){
        new_node->down->up = new_node;
    }

    mx->rows[row]->count++;
    mx->cols[col]->count++;

}

//Remove node at row, col
//If no node exists at this position, do nothing
void remove_node(Matrix* mx, int row, int col){
    assert(row >= 0 && col >= 0 && row < mx->num_rows && col < mx->num_cols);

    //iterate through row to find node to remove
    Node* itr_n = mx->rows[row];
    while(itr_n->right != NULL && itr_n->right->col <= col){
        itr_n = itr_n->right;
    }
    //if node doesn't exist, quit
    if(itr_n->col != col){
        printf("Node doesn't exist\n");
        return;
    }
    //reassign left and right pointers
    itr_n->left->right = itr_n->right;
    if(itr_n->right != NULL){
        itr_n->right->left = itr_n->left;
    }
    //reassign up and down pointers
    itr_n->up->down = itr_n->down;
    if(itr_n->down != NULL){
        itr_n->down->up = itr_n->up;
    }

    mx->rows[row]->count--;
    mx->cols[col]->count--;
    free(itr_n);
}

void print_matrix(Matrix* mx){
    Node* itr_n;
    int i, j;
    for(i=0; i<mx->num_rows; i++){
        itr_n = mx->rows[i];
        for(j=0; j<mx->num_cols; j++){
            if(itr_n->right == NULL || itr_n->right->col > j){
                printf("0 ");
            }
            else{
                printf("%d ", itr_n->right->value);
                itr_n = itr_n->right;
            }
        }
        printf("\n");
    }
}

void delete_matrix(Matrix* mx){
    int i;
    Node* itr_n;
    for(i=0; i<mx->num_cols; i++){
        free(mx->cols[i]);
    }
    for(i=0; i<mx->num_rows; i++){
        itr_n = mx->rows[i];
        while(itr_n->right != NULL){
            itr_n = itr_n->right;
            free(itr_n->left);
        }
        free(itr_n);
    }
    free(mx->rows);
    free(mx->cols);
    free(mx);
}

Node* create_node(Matrix* mx, int row, int col, int type, int value, int count){
    Node* node = malloc(sizeof(Node));
    node->row=row;
    node->col=col;
    node->type=type;
    node->value=value;
    node->count=count;
    node->up=NULL;
    node->down=NULL;
    node->left=NULL;
    node->right=NULL;
    node->matrix=mx;
    return node;
}

//Initialize the Row and Column headers of the Matrix
void init_matrix(Matrix* mx){
    mx->rows[0] = create_node(mx, 0, -1, 2, -1, 0);
    //instantiate array of row header nodes
    for(int i=1; i<mx->num_rows; i++){
        Node* node = create_node(mx, i, -1, 2, -1, 0);
        node->up = mx->rows[i-1];
        mx->rows[i-1]->down = node;
        mx->rows[i] = node;
    }

    mx->cols[0] = create_node(mx, -1, 0, 2, -1, 0);
    //instantiate array of column header nodes
    for(int i=1; i<mx->num_cols; i++){
        Node* node = create_node(mx, -1, i, 2, -1, 0);
        node->left = mx->cols[i-1];
        mx->cols[i-1]->right = node;
        mx->cols[i] = node;
    }
    mx->row_head = mx->rows[0];
    mx->col_head = mx->cols[0];
}