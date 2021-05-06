#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solve.h"


int main(){
    int size = 9;
    int solution[size*size];
    int puzzle[] =
    {0,4,3,0,0,7,8,0,0,
     1,0,0,0,0,0,0,0,4,
     0,6,8,0,0,0,0,0,0,
     0,0,0,0,3,1,0,8,6,
     0,0,9,2,0,5,1,0,0,
     4,8,0,7,9,0,0,0,0,
     0,0,0,0,0,0,6,5,0,
     2,0,0,0,0,0,0,0,8,
     0,0,5,8,0,0,2,1,0};
    
    bool solved = solve_puzzle(puzzle, size, solution);
    
    if(solved){
        for(int i=0; i<size*size; i++){
            if(i % size == 0) { printf("\n"); }
            printf("%d ", solution[i]);
        }
        printf("\n");
    }

    return 0;
}