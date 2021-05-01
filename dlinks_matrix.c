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
    Node* itr = mx->rows[row];
    Node* start = itr;
    while(itr->right != start && itr->right->col < col){
        itr = itr->right;
    }
    //if node at this position already exists, reassign value and leave
    if(itr->right->col == col){
        itr->right->value = value;
        return;
    }
    //reassign left and right pointers
    new_node->right = itr->right;
    new_node->left = itr;
    itr->right = new_node;
    new_node->right->left = new_node;

    //iterate through column to find correct placement of node in column
    itr = mx->cols[col];
    start = itr;
    while(itr->down != start && itr->down->row < row){
        itr = itr->down;
    }
    //reassign up and down pointers
    new_node->down = itr->down;
    new_node->up = itr;
    itr->down = new_node;
    new_node->down->up = new_node;

    mx->rows[row]->count++;
    mx->cols[col]->count++;

}
 
//Remove node at row, col
//If no node exists at this position, do nothing
void remove_node(Matrix* mx, int row, int col){
    assert(row >= 0 && col >= 0 && row < mx->num_rows && col < mx->num_cols);

    //iterate through row to find node to remove
    Node* itr = mx->rows[row];
    Node* start = itr;
    while(itr->right != start && itr->right->col <= col){
        itr = itr->right;
    }
    //if node doesn't exist, leave
    if(itr->col != col){
        printf("Node doesn't exist\n");
        return;
    }
    //reassign pointers
    itr->left->right = itr->right;
    itr->right->left = itr->left;
    itr->up->down = itr->down;
    itr->down->up = itr->up;

    mx->rows[row]->count--;
    mx->cols[col]->count--;
    free(itr);
}

void print_matrix(Matrix* mx){
    Node* itr, *start;
    int i, j;
    for(i=0; i<mx->num_rows; i++){
        itr = mx->rows[i];
        start = itr;
        for(j=0; j<mx->num_cols; j++){
            if(itr->right == start || itr->right->col > j){
                printf("0 ");
            }
            else{
                printf("%d ", itr->right->value);
                itr = itr->right;
            }
        }
        printf("\n");
    }
    printf("\n");
}

void delete_matrix(Matrix* mx){
    int i;
    Node* itr, *start;
    for(i=0; i<mx->num_cols; i++){
        free(mx->cols[i]);
    }
    for(i=0; i<mx->num_rows; i++){
        itr = mx->rows[i]->right;
        start = mx->rows[i]->right;
        while(itr->right != start){
            itr = itr->right;
            free(itr->left);
        }
        free(itr);
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
    //instantiate array of row header nodes
    mx->rows[0] = create_node(mx, 0, -1, 2, -1, 0);
    mx->rows[0]->right = mx->rows[0];
    mx->rows[0]->left = mx->rows[0];
    for(int i=1; i<mx->num_rows; i++){
        Node* node = create_node(mx, i, -1, 2, -1, 0);
        node->up = mx->rows[i-1];
        mx->rows[i-1]->down = node;
        node->right = node;
        node->left = node;
        mx->rows[i] = node;
    }
    mx->rows[mx->num_rows-1]->down = mx->rows[0];
    mx->rows[0]->up = mx->rows[mx->num_rows-1];

    //instantiate array of column header nodes
    mx->cols[0] = create_node(mx, -1, 0, 2, -1, 0);
    mx->cols[0]->down = mx->cols[0];
    mx->cols[0]->up = mx->cols[0];
    for(int i=1; i<mx->num_cols; i++){
        Node* node = create_node(mx, -1, i, 2, -1, 0);
        node->left = mx->cols[i-1];
        mx->cols[i-1]->right = node;
        node->down = node;
        node->up = node;
        mx->cols[i] = node;
    }
    mx->cols[mx->num_cols-1]->right = mx->cols[0];
    mx->cols[0]->left = mx->cols[mx->num_cols-1];

    mx->row_head = mx->rows[0];
    mx->col_head = mx->cols[0];
}