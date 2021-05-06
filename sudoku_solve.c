#include <stdio.h>
#include <stdlib.h>
#include "algx.h"
#include "sudoku_solve.h"


static inline int one_constraint(int row, int size){
    return row/size;
}

static inline int row_constraint(int row, int size){
    return (size*size) + size*(row/(size*size)) + (row % size);
}

static inline int col_constraint(int row, int size){
    return 2*(size*size) + (row % (size*size));
}

static inline int box_constraint(int row, int size){
    return 3*(size*size) + (row % size);
}

Matrix* sud2matrix(int* sudoku_list, int size){
    //assert size is perfect square
    int num_rows = size*size*size;
    int num_cols = size*size*4;
    int num_cells = size*size;
    Matrix* matrix = create_matrix(num_rows, num_cols);

    int row = 0;
    for(int i=0; i<num_cells; i++){
        if(sudoku_list[i] == 0){
            for(int j=0; j<size; j++){
                row = i*size+j;
                insert_node(matrix, row, one_constraint(row, size), 1);
                insert_node(matrix, row, row_constraint(row, size), 1);
                insert_node(matrix, row, col_constraint(row, size), 1);
                insert_node(matrix, row, box_constraint(row, size), 1);
            }
        }
        else{
            row = i*size+sudoku_list[i]+1;
            insert_node(matrix, row, one_constraint(row, size), 1);
            insert_node(matrix, row, row_constraint(row, size), 1);
            insert_node(matrix, row, col_constraint(row, size), 1);
            insert_node(matrix, row, box_constraint(row, size), 1);
        }
    }

    return matrix;
}