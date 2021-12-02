#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "dlinks_matrix.h"


//return column index for the one value per cell constraint given row and dimension of puzzle
static inline int one_constraint(int row, int dim){
    return row/dim;
}
//return column index for the unique value per row constraint given row and dimension of puzzle
static inline int row_constraint(int row, int dim){
    return (dim*dim) + dim*(row/(dim*dim)) + (row%dim);
}
//return column index for the unique value per col constraint given row and dimension of puzzle
static inline int col_constraint(int row, int dim){
    return 2*(dim*dim) + (row % (dim*dim));
}
//return column index for the unique value per box constraint given row and dimension of puzzle
static inline int box_constraint(int row, int dim){
    return 3*(dim*dim) + (row/((int)sqrt(dim)*dim*dim))*(dim*(int)sqrt(dim)) +
           ((row/((int)sqrt(dim)*dim)) % (int)sqrt(dim))*dim + (row%dim);
}

//convert int array representing puzzle into constraint matrix for algorithm x
//inputs: int array and dimension of puzzle. (dim==9 for standard 9x9 puzzle)
Matrix* puzzle_to_matrix(int* sudoku_list, int dim){
    assert((int)sqrt(dim)*(int)sqrt(dim) == dim); //only perfect square puzzles are supported
    int num_rows  = dim*dim*dim;
    int num_cols  = dim*dim*4;
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

//solve a soduku puzzle represented by an int array
//returns true if solution is found, false otherwise
//inputs: int array puzzle - original puzzle
//        int dim - dimension of puzzle (dim==9 for standard 9x9 sudoku)
//        int array solution - contains completed puzzle if solution is found
bool solve_puzzle(int* puzzle, int dim, int* solution){
    Matrix* matrix = puzzle_to_matrix(puzzle, dim);
    bool found = alg_x_search(matrix);
    if(found) { //decode solution stored in matrix.solution
        int index, value;
        for(solution_node* itr=matrix->solution->head; itr!=NULL; itr=itr->next){
            index = ((Node*)itr->data)->row / dim;
            value = (((Node*)itr->data)->row % dim) + 1;
            solution[index] = value;
        }
    }
    delete_matrix(matrix);
    return found;
}

//prints int array in sudoku puzzle format
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

int main(){
    int dimension = 16;
    int solution[dimension*dimension];
    //hardcoded puzzle example
    int puzzle[] =
    {15, 0, 0,13, 0,14,11, 8, 3, 0, 0, 1, 4, 0, 0, 7,
      3, 8, 0, 0,15, 2, 6, 0, 0, 0, 0, 0, 1, 0,14, 0,
      6, 0, 1, 0,10, 7, 0,12,15, 0, 5, 0, 2, 0,11, 3,
     11, 0, 0, 0, 0, 3, 0, 0, 4, 0,13,14, 0, 0, 0, 6,
      8, 3,10,15, 0,16,13, 0, 7, 0, 0, 6,14, 5, 0,12,
      0, 0, 0, 1, 0, 0,14, 0,13, 0, 0,15, 6, 7, 0, 0,
      7, 2,12, 6, 8, 4, 0,10, 5, 9, 0,16, 0,11, 1,15,
      0,13,14, 0, 7, 6, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
      0, 5,11, 0,14,13, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
      1, 7, 2,10,12, 8, 0, 4, 6, 3, 0, 5, 0,14, 9,13,
      0, 0, 0, 3, 0, 0,10, 0, 9, 0, 0, 4,12,16, 0, 0,
     14, 4, 9, 8, 0,15, 2, 0,12, 0, 0,11, 7, 6, 0,10,
     12, 0, 0, 0, 0,10, 0, 0,16, 0, 9,13, 0, 0, 0, 2,
     10, 0,13, 0, 4, 1, 0, 2,14, 0, 6, 0, 9, 0, 8,11,
      2,15, 0, 0, 6, 9, 7, 0, 0, 0, 0, 0,10, 0,13, 0,
      9, 0, 0,14, 0,12, 8, 3, 1, 0, 0, 2, 5, 0, 0,16};
    
    bool solved = solve_puzzle(puzzle, dimension, solution);
    printf("Puzzle:\n");
    print_puzzle(puzzle, dimension);
    if(solved){
        printf("Solution:\n");
        print_puzzle(solution, dimension);
    }
    else{
        printf("No Solution Found\n");
    }

    return 0;
}