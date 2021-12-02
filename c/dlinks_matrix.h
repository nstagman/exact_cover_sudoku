#ifndef DLINKS_MATRIX_H
#define DLINKS_MATRIX_H
#include <stdbool.h>
#include "solution_stack.h"

#define uln "\033[4m"
#define res "\033[0m"

//Toroidal Matrix structure used for holding the constraints of an exact cover problem

typedef struct _matrix Matrix;
typedef struct _node Node;

//Data node for sparse matrix
struct _node{
    int row, col, value, type, count;
    Node* up, *down, *left, *right;
    Matrix* matrix;
};

//Toroidally linked sparse matrix
struct _matrix{
    Node** rows, **cols;
    Node* root;
    int num_rows, num_cols;
    solution_stack* solution;
    bool solved;
};

Matrix* create_matrix(int num_rows, int num_cols);
void insert_node(Matrix* mx, int row, int col, int value);
void remove_node(Matrix* mx, int row, int col);
void print_matrix(Matrix* mx);
void delete_matrix(Matrix* mx);
bool alg_x_search(Matrix* mx);

static inline bool matrix_is_empty(Matrix* matrix){
    return matrix->root->right == matrix->root;
}

static inline Node* column_of(Node* node){
    if(node->col==-1) { return node->matrix->root; }
    return node->matrix->cols[node->col];
}

static inline bool column_is_covered(Node* node){
    return !(column_of(node)->right->left == column_of(node));
}

#endif