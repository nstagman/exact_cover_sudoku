#include <iostream>
#include <fstream>
// #include <string>
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


int main(){
    Matrix solver;
    char buf[1024];
    int solution[81];
    int puzzle[81];
    char pstr[81];

    std::fstream fs;
    fs.open("puzzle.txt", std::fstream::in | std::fstream::out);
    std::FILE* puzzle_file = std::fopen("puzzles.txt", "r+");
    while(std::fgets(buf, 1024, puzzle_file) != 0){
        for(int i=0; i<81; i++){
            puzzle[i] = buf[i] - '0';
        }
        solve_puzzle(&solver, puzzle, solution);
        for(int i=0; i<81; i++){
            pstr[i] = solution[i] + '0';
        }
        std::fputs(" ", puzzle_file);
        std::fputs(pstr, puzzle_file);
        std::fputs("\n", puzzle_file);
        
    }
    return 0;
}