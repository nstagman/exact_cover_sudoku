#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solve.h"

//return column index for the one value per cell constraint given row and size of puzzle
static inline int one_constraint(int row, int size){
    return row/size;
}
//return column index for the unique value per row constraint given row and size of puzzle
static inline int row_constraint(int row, int size){
    return (size*size) + size*(row/(size*size)) + (row%size);
}
//return column index for the unique value per col constraint given row and size of puzzle
static inline int col_constraint(int row, int size){
    return 2*(size*size) + (row % (size*size));
}
//return column index for the unique value per box constraint given row and size of puzzle
static inline int box_constraint(int row, int size){
    return 3*(size*size) + (row/((int)sqrt(size)*size*size))*(size*(int)sqrt(size)) + ((row/((int)sqrt(size)*size)) % (int)sqrt(size))*size + (row%size);
}

//convert int array representing puzzle into constraint matrix for algorithm x
Matrix* sud2matrix(int* sudoku_list, int size){
    assert((int)sqrt(size)*(int)sqrt(size) == size); //only perfect square puzzles are supported
    int num_rows = size*size*size;
    int num_cols = size*size*4;
    int num_cells = size*size;
    Matrix* matrix = create_matrix(num_rows, num_cols);

    int row = 0;
    //iterate through puzzle list
    for(int i=0; i<num_cells; i++){
        //if no value assigned to cell, populate all rows representing candidate values for cell
        if(sudoku_list[i] == 0){
            for(int j=0; j<size; j++){
                row = i*size+j;
                insert_node(matrix, row, one_constraint(row, size), 1);
                insert_node(matrix, row, row_constraint(row, size), 1);
                insert_node(matrix, row, col_constraint(row, size), 1);
                insert_node(matrix, row, box_constraint(row, size), 1);
            }
        }
        //otherwise only populate the row represnting the known assigned value
        else{
            row = i*size+sudoku_list[i]-1;
            insert_node(matrix, row, one_constraint(row, size), 1);
            insert_node(matrix, row, row_constraint(row, size), 1);
            insert_node(matrix, row, col_constraint(row, size), 1);
            insert_node(matrix, row, box_constraint(row, size), 1);
        }
    }
    return matrix;
}

void decode_solution(lifo* solution_stack, int size, int* decoded){
    int index, value;
    for(lifo_node* itr=solution_stack->head; itr!=NULL; itr=itr->next){
        index = itr->data->row / size;
        value = (itr->data->row % size) + 1;
        decoded[index] = value;
    }
}

//solve a soduku puzzle represented by an int array
//returns true if solution is found, false otherwise
bool solve_puzzle(int* puzzle, int size, int* solution){
    Matrix* puzzle_matrix = sud2matrix(puzzle, size);
    lifo* solution_stack = create_stack();
    bool found = alg_x_search(puzzle_matrix, solution_stack);
    if(found) { decode_solution(solution_stack, size, solution); }
    
    delete_matrix(puzzle_matrix);
    delete_stack(solution_stack);
    return found;
}
