#ifndef DLINKS_MATRIX_H
#define DLINKS_MATRIX_H
#include <stdbool.h>

#define uln "\033[4m"
#define res "\033[0m"

typedef struct matrix Matrix;
typedef struct node Node;

Matrix* create_matrix(int, int);
void insert_node(Matrix*, int, int, int);
void remove_node(Matrix*, int, int);
void print_matrix(Matrix*);
void delete_matrix(Matrix*);

struct node{
    int row, col, value, type, count;
    struct node* up, *down, *left, *right;
    struct matrix* matrix;
};

struct matrix{
    Node** rows, **cols;
    Node* root;
    int num_rows, num_cols;
    bool solved;
};

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