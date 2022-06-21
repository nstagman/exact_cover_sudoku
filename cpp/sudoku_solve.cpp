#include <stdio.h>
#include "dlinks_matrix.hpp"


//accepts up to 2 arguments: 
//  1 - file of puzzles, puzzles are a new-line delimited string of numbers with 0 representing empty boxes
//      defaults to "puzzles.txt"
//  2 - output file, solution will be written to the output file in the same format as the input file
//      defaults to "solutions.txt"
int main(int argc, char *argv[]){
    char solution[81];
    char puzzle[83];
    FILE* puzzle_file;
    FILE* solutions_file;

    if(argc > 1) { puzzle_file = fopen(argv[1], "r"); }
    else { puzzle_file = fopen("puzzles.txt", "r"); }
    if(argc > 2) { solutions_file = fopen(argv[2], "w"); }
    else { solutions_file = fopen("solutions.txt", "w"); }

    while(fgets(puzzle, 83, puzzle_file) != 0){
        puzzle[81] = 0;

        if(solve_puzzle(puzzle)) {
            int index, value;
            for(int i=0; i<81; i++){
                index = solution_stack[i]->row / 9;
                value = (solution_stack[i]->row % 9) + '1';
                solution[index] = value;
            }
        }
        
        fputs(puzzle, solutions_file);
        fputs(",", solutions_file);
        fputs(solution, solutions_file);
        fputs("\n", solutions_file);
    }
    return 0;
}