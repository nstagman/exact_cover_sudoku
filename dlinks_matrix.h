#ifndef DLINKS_MATRIX_H
#define DLINKS_MATRIX_H


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
    Node* rows, *cols;
    int num_rows, num_cols;
};

#endif