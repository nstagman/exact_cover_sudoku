from math import sqrt
import dlinks_matrix as dlm


# --- Constraints for a sudoku puzzle ---
# One value per cell
# Unique value per row
# Unique value per col
# Unique value per box
# ---------------------------------------
# These functions return the column of the matrix to be populated for a constraint when given
# a specified row of the matrix and the dimension of the sudoku puzzle
def _one_constraint(row: int, dim:int) -> int:
    return row//dim
def _row_constraint(row:int, dim:int) -> int:
    return dim**2 + dim*(row//(dim**2)) + row % dim
def _col_constraint(row:int, dim:int) -> int:
    return 2*(dim**2) + (row % (dim**2))
def _box_constraint(row:int, dim:int) -> int:
    return int(3*(dim**2) + (row//(sqrt(dim)*dim**2))*(dim*sqrt(dim)) + ((row//(sqrt(dim)*dim)) % sqrt(dim))*dim + (row % dim))
constraint_list = [_one_constraint, _row_constraint, _col_constraint, _box_constraint]

# convert list of ints representing the puzzle to a dancing link matrix
def _list_2_matrix(puzzle: list[int], dim: int) -> dlm.DL_Matrix:
    num_rows = dim**3
    num_cols = (dim**2)*len(constraint_list)
    matrix: dlm.DL_Matrix = dlm.DL_Matrix(num_rows, num_cols)
    #iterate through puzzle
    for i, cell in enumerate(puzzle):
        if cell == 0: # if cell is unassigned
            # populate all rows representing cadidate values for this cell
            for j in range(dim):
                row = i*dim+j
                for constraint in constraint_list:
                    matrix.insert_node(row, constraint(row, dim))
        else: # if cell is assigned
            # populate the row representing the assigned value for this cell
            row = i*dim+cell-1
            for constraint in constraint_list:
                    matrix.insert_node(row, constraint(row, dim))
    return matrix

# takes list of ints representing a sudoku puzzle
# returns a list of ints representing the solution if one is found
def solve_puzzle(puzzle: list[int]) -> list[int]:
    dim = int(sqrt(len(puzzle)))
    assert(int(dim+0.5)**2 == len(puzzle)) # only perfect square puzzles are supported
    solution_list = _list_2_matrix(puzzle, dim).alg_x_search()
    if not solution_list: return []
    solved_puzzle = [0] * (dim**2)
    for node in solution_list:
        solved_puzzle[node.row // dim] = (node.row % dim) + 1
    return solved_puzzle

# prints a list of ints as a sudoku puzzle
def print_puzzle(puzzle: list[int]) -> None:
    uln = '\033[4m'
    res = '\033[0m'
    dim = int(sqrt(len(puzzle)))
    assert(int(dim+0.5)**2 == len(puzzle)) # only perfect square puzzles are supported
    for i in range(dim): print(uln + '   ', end='')
    print('     ' + res, end='')
    for i, cell in enumerate(puzzle):
        if i % int(sqrt(dim)) == 0 and i != 0: print("|", end='')
        if i//dim % int(sqrt(dim)) == int(sqrt(dim)) -1: print(uln + "", end='')
        if i % dim == 0: print('\n|', end='')
        print(f"{cell:3d}", end='')
        print(res, end='')
    print("|")


if __name__ == '__main__':

    def main():
        with open('puzzles.txt', 'rb') as puzzle_file, open('solutions.txt', 'wb') as solution_file:
            while puzzle_str := puzzle_file.read(82).rstrip(b'\n'):
                puzzle = [c - 0x30 for c in puzzle_str]
                solution = solve_puzzle(puzzle)
                solution_str = bytes([i + 0x30 for i in solution]) + b'\n'
                solution_file.write(puzzle_str + b',' + solution_str)
        
    main()
