#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solve.h"


//return column index for the one value per cell constraint given row and dimensioin of puzzle
static inline int one_constraint(int row, int dim){
    return row/dim;
}
//return column index for the unique value per row constraint given row and dimensioin of puzzle
static inline int row_constraint(int row, int dim){
    return (dim*dim) + dim*(row/(dim*dim)) + (row%dim);
}
//return column index for the unique value per col constraint given row and dimensioin of puzzle
static inline int col_constraint(int row, int dim){
    return 2*(dim*dim) + (row % (dim*dim));
}
//return column index for the unique value per box constraint given row and dimensioin of puzzle
static inline int box_constraint(int row, int dim){
    return 3*(dim*dim) + (row/((int)sqrt(dim)*dim*dim))*(dim*(int)sqrt(dim)) + ((row/((int)sqrt(dim)*dim)) % (int)sqrt(dim))*dim + (row%dim);
}

//convert int array representing puzzle into constraint matrix for algorithm x
//inputs: int array and dimension of puzzle. (dim==9 for standard 9x9 puzzle)
Matrix* sud2matrix(int* sudoku_list, int dim){
    assert((int)sqrt(dim)*(int)sqrt(dim) == dim); //only perfect square puzzles are supported
    int num_rows = dim*dim*dim;
    int num_cols = dim*dim*4;
    int num_cells = dim*dim;
    Matrix* matrix = create_matrix(num_rows, num_cols);

    int row = 0;
    //iterate through puzzle list
    for(int i=0; i<num_cells; i++){
        //if no value assigned to cell, populate all rows representing all possible candidate values for cell
        if(sudoku_list[i] == 0){
            for(int j=0; j<dim; j++){
                row = i*dim+j;
                insert_node(matrix, row, one_constraint(row, dim), 1);
                insert_node(matrix, row, row_constraint(row, dim), 1);
                insert_node(matrix, row, col_constraint(row, dim), 1);
                insert_node(matrix, row, box_constraint(row, dim), 1);
            }
        }
        //otherwise only populate the row representing the known assigned value
        else{
            row = i*dim+sudoku_list[i]-1;
            insert_node(matrix, row, one_constraint(row, dim), 1);
            insert_node(matrix, row, row_constraint(row, dim), 1);
            insert_node(matrix, row, col_constraint(row, dim), 1);
            insert_node(matrix, row, box_constraint(row, dim), 1);
        }
    }
    return matrix;
}

//decodes the rows of constraint matrix stored in solution_stack and
//populates solved_puzzle int array with a representation of the completed puzzle
void decode_solution(lifo* solution_stack, int dim, int* solved_puzzle){
    int index, value;
    for(lifo_node* itr=solution_stack->head; itr!=NULL; itr=itr->next){
        index = itr->data->row / dim;
        value = (itr->data->row % dim) + 1;
        solved_puzzle[index] = value;
    }
}

//solve a soduku puzzle represented by an int array
//returns true if solution is found, false otherwise
//inputs: int array puzzle - original puzzle
//        int dim - dimension of puzzle (dim==9 for standard 9x9 sudoku)
//        int array solution - contains completed puzzle if solution is found
bool solve_puzzle(int* puzzle, int dim, int* solution){
    Matrix* puzzle_matrix = sud2matrix(puzzle, dim);
    lifo* solution_stack = create_stack();
    bool found = alg_x_search(puzzle_matrix, solution_stack);
    if(found) { decode_solution(solution_stack, dim, solution); }

    delete_matrix(puzzle_matrix);
    delete_stack(solution_stack);
    return found;
}

//prints sudoku puzzle
void print_puzzle(int* solution, int dim){
    for(int i=0; i<dim; i++) { printf(uln"   "); }
    printf("     "res"");
    for(int i=0; i<dim*dim; i++){
        if((i % (int)sqrt(dim) == 0 && i != 0)) { printf("|"); }
        if(i/dim % (int)sqrt(dim) == (int)sqrt(dim)-1) { printf(uln""); }
        if(i % dim == 0) { printf("\n|"); }
        printf("%3d"res, solution[i]);
    }
    printf("|\n");
}
