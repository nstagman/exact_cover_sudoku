#include <iostream>
#include "dlinks_matrix.h"


//solve a soduku puzzle represented by an int array
//returns true if solution is found, false otherwise
//inputs: int array puzzle - original puzzle
//        int dim - dimension of puzzle (dim==9 for standard 9x9 sudoku)
//        int array solution - contains completed puzzle if solution is found
bool solve_puzzle(Matrix* matrix, int* puzzle, int* solution){
    matrix->build_matrix(puzzle);
    bool found = matrix->alg_x_search();
    if(found) {
        int index, value;
        for(int i=0; i<81; i++){
            index = matrix->solution_stack[i] / 9;
            value = (matrix->solution_stack[i] % 9) + 1;
            solution[index] = value;
        }
    }
    return found;
}

//accepts up to 2 arguments: 
//  1 - file of puzzles, puzzles are a new-line delimited string of numbers with 0 representing empty boxes
//      defaults to "puzzles.txt"
//  2 - output file, solution will be written to the output file in the same format as the input file
//      defaults to "solutions.txt"
int main(int argc, char *argv[]){
    Matrix matrix;
    int solution[81];
    int puzzle[81];
    char buf[82];
    char pstr[81];
    std::FILE* puzzle_file;
    std::FILE* solutions_file;

    if(argc > 1) { puzzle_file = std::fopen(argv[1], "r"); }
    else { puzzle_file = std::fopen("puzzles.txt", "r"); }
    if(argc > 2) { solutions_file = std::fopen(argv[2], "w"); }
    else { solutions_file = std::fopen("solutions.txt", "w"); }

    while(std::fgets(buf, 83, puzzle_file) != 0){
        buf[81] = '\0';
        for(int i=0; i<81; i++){
            puzzle[i] = buf[i] - '0';
        }
        solve_puzzle(&matrix, puzzle, solution);
        for(int i=0; i<81; i++){
            pstr[i] = solution[i] + '0';
        }
        std::fputs(buf, solutions_file);
        std::fputs(",", solutions_file);
        std::fputs(pstr, solutions_file);
        std::fputs("\n", solutions_file);
    }
    return 0;
}