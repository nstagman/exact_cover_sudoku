from __future__ import annotations
from typing import Generator, List


uln = '\033[4m'
res = '\033[0m'

# Toroidally Linked Matrix
class DL_Matrix:
    def __init__(self, num_rows:int, num_cols:int) -> None:
        self.root: Node = Node(self, -1, -1)
        self.rows: list[Node] = [Node(self, row=i, col=-1) for i in range(num_rows)]
        self.cols: list[Node] = [Node(self, row=-1, col=i, count=0) for i in range(num_cols)]
        self.solved: bool = False
        # set up links for row and column headers
        for i, node in enumerate(self.rows):
            node.right = node
            node.left = node
            node.down = self.rows[i+1] if i < len(self.rows)-1 else self.root
            node.up = self.rows[i-1] if i > 0 else self.root
        for i, node in enumerate(self.cols):
            node.up = node
            node.down = node
            node.right = self.cols[i+1] if i < len(self.cols)-1 else self.root
            node.left = self.cols[i-1] if i > 0 else self.root
        # set root links to row and column headers
        self.root.right = self.cols[0]
        self.root.left = self.cols[-1]
        self.root.down = self.rows[0]
        self.root.up = self.rows[-1]

    # returns the column header of column with minimum number of 1's
    def select_min_col(self) -> Node:
        if self.is_empty(): return self.root
        min_node = self.root.right
        min_count = self.root.right.count
        for col in self.root.itr_right(excl=True):
            if col.count < min_count:
                min_node = col
                min_count = col.count
        return min_node

    # cover a column of a node for dancing links algorithm x
    @staticmethod
    def cover(node: Node) -> None:
        col = node.get_col()
        # unlink left and right neighbors of column header
        col.right.left = col.left
        col.left.right = col.right
        # iterate through each node in column top to bottom
        for down_itr in col.itr_down(excl=True):
            # iterate though each node in row of next_down left to right
            for right_itr in down_itr.itr_right(excl=True):
                # unlink top and bottom neighbors of each node reduce count in col header
                right_itr.up.down = right_itr.down
                right_itr.down.up = right_itr.up
                right_itr.get_col().count -= 1

    # uncover a column of a node for dancing links algorithm x
    @staticmethod
    def uncover(node: Node) -> None:
        col = node.get_col()
        # iterate through each node in column bottom to top
        for up_itr in col.itr_up(excl=True):
            # iterate through each node in row of next_up right to left
            for left_itr in up_itr.itr_left(excl=True):
                # relink top and bottom neighbors of each node and increment count in col header
                left_itr.up.down = left_itr
                left_itr.down.up = left_itr
                left_itr.get_col().count += 1
        # relink left and right neighbors of column header
        col.right.left = col
        col.left.right = col
    
    # search matrix for exact cover, return list of rows as solution if one exists
    def alg_x_search(self) -> List[Node]:
        solutions = [] # list of solutions
        
        # recursive search algorithm returns true if solution is found
        def search() -> bool:
            if self.is_empty(): # if matrix is empty exact cover exists
                self.solved = True
                return True

            # select col with least number of 1's
            selected_col = self.select_min_col()
            # if selected col has zero 1's, then this branch has failed
            if selected_col.count < 1 : return False

            # iterate down from col head
            for down_itr in selected_col.itr_down(excl=True):
                solutions.append(self.rows[down_itr.row]) # add current row to solutions
                # iterate right from current node in column and cover each
                for right_itr in down_itr.itr_right():
                    if right_itr.col >= 0: DL_Matrix.cover(right_itr)

                # search again after covering
                # if solution is found on this branch, leave loop and stop searching
                if search(): break

                # solution not found on this branch, need to uncover columns from this iteration
                solutions.pop() # remove current row from solutions
                # iterate left from the last covered column and uncover
                for left_itr in down_itr.itr_left(excl=True):
                    if left_itr.col >= 0: DL_Matrix.uncover(left_itr)
                DL_Matrix.uncover(down_itr )
            return self.solved
        
        if search(): return solutions
        else: return []

    # inserts Node into Matrix at row, col
    # if node already exists at row, col - do nothing
    def insert_node(self, row:int, col:int) -> None:
        assert(row >=0 and col >= 0 and row < len(self.rows) and col < len(self.cols))
        # create node to insert
        new_node: Node = Node(self, row, col)
        # iterate through the row to find correct placement of new_node in row
        for n in self.rows[row].itr_right():
            if n.right.col == -1 or n.right.col > col: break
        if n.col == col: return # if node already exists, leave
        # reassign left and right pointers
        new_node.right = n.right
        new_node.left = n
        n.right = new_node
        new_node.right.left = new_node
        # iterate through the column to find correct placement of new_node in col
        for n in self.cols[col].itr_down():
            if n.down.row == -1 or n.down.row > row: break
        # reassign up and down pointers
        new_node.down = n.down
        new_node.up = n
        n.down = new_node
        new_node.down.up = new_node
        self.cols[col].count += 1

    # returns True if matrix is empty, False otherwise
    def is_empty(self) -> bool:
        return self.root.right == self.root

    # prints Matrix in tabular form
    def print_matrix(self) -> None:
        print('n: ', end='')
        # print counts for each column
        for col in self.root.itr_right(excl=True):
            print(col.count, end=' ')
        print()
        print(uln + "R| ", end='')
        for col in self.root.itr_right(excl=True):
            print(col.col, end=' ')
        print(res)
        for row in self.root.itr_down(excl=True):
            print(row.row, end='| ')
            col = row
            for i in range(len(self.cols)):
                if self.cols[i].col_is_covered(): continue
                if col.right.col > i or col.right.col == -1:
                    print(0, end=' ')
                else:
                    print(1, end=' ')
                    col=col.right
            print()


# Node to be stored in toroidal linked matrix
class Node:
    def __init__(self, matrix:DL_Matrix, row:int, col:int, count:int=-1,
                 up:Node=None, down:Node=None, left:Node=None, right:Node=None) -> None:
        self.row: int = row
        self.col: int = col
        self.count: int = count
        self.matrix: DL_Matrix = matrix
        self.up: Node = up
        self.down: Node = down
        self.left: Node = left
        self.right: Node = right
    
    # iterate full circe from node moving up
    # if excl is True, this node will not be yielded
    def itr_up(self, excl=False) -> Generator[Node]:
        start, itr = self, self
        if not excl: yield itr
        while itr.up != start:
            itr = itr.up
            yield itr

    # iterate full circle from node moving down
    # if excl is True, this node will not be yielded
    def itr_down(self, excl=False) -> Generator[Node]:
        start, itr = self, self
        if not excl: yield itr
        while itr.down != start:
            itr = itr.down
            yield itr

    # iterate full circle from node moving left
    # if excl is True, this node will not be yielded
    def itr_left(self, excl=False) -> Generator[Node]:
        start, itr = self, self
        if not excl: yield itr
        while itr.left != start:
            itr = itr.left
            yield itr

    # iterate full circle from node moving right
    # if excl is True, this node will not be yielded
    def itr_right(self, excl=False) -> Generator[Node]:
        start, itr = self, self
        if not excl: yield itr
        while itr.right != start:
            itr = itr.right
            yield itr

    # return column header of this node
    def get_col(self) -> Node:
        if self.col == -1: return self.matrix.root
        return self.matrix.cols[self.col]

    # returns whether node is in a covered column or not
    def col_is_covered(self) -> bool:
        return not(self.get_col().right.left == self.get_col())
