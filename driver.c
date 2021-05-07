#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku_solve.h"


int main(){
    struct timespec start, end;
    int size = 16;
    int solution[size*size];
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
    // int puzzle[] =
    // {0,9,0,2,0,0,0,0,0,
    //  8,2,3,0,0,5,0,0,0,
    //  0,4,0,0,3,0,0,0,0,
    //  3,0,0,6,0,0,0,0,8,
    //  4,0,0,0,7,0,0,0,6,
    //  7,0,0,0,0,9,0,0,4,
    //  0,0,0,0,2,0,0,5,0,
    //  0,0,0,1,0,0,7,8,3,
    //  0,0,0,0,0,8,0,1,0};
    
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