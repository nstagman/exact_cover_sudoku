#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku_solve.h"


int main(){
    struct timespec start, end;
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
    
    clock_gettime(CLOCK_REALTIME, &start);
    bool solved = solve_puzzle(puzzle, size, solution);
    clock_gettime(CLOCK_REALTIME, &end);
    double t = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1000000000.0;
    
    printf("Puzzle:\n");
    print_puzzle(puzzle, size);
    if(solved){
        printf("\nSolution Found In: %f Seconds\n", t);
        print_puzzle(solution, size);
    }
    else{
        printf("No Solution Found\n");
    }

    return 0;
}