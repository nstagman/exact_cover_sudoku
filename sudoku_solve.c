#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solve.h"


Matrix* sud2matrix(int* sudoku_list, int size){
    Matrix* matrix = create_matrix(size, size);
    return matrix;
}