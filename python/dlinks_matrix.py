from __future__ import annotations
from typing import Generator, Union


# Toroidally Linked Matrix
class DL_Matrix:
    def __init__(self, num_rows: int, num_cols: int) -> None:
        self.root: Node       = Node(self, -1, -1)
        self.rows: list[Node] = [Node(self, row=i, col=-1) for i in range(num_rows)]
        self.cols: list[Node] = [Node(self, row=-1, col=i, count=0) for i in range(num_cols)]
        self.solved: bool = False
        # set up links for row and column headers
        for i, node in enumerate(self.rows):
            node.right = node
            node.left  = node
            node.down  = self.rows[i+1] if i < len(self.rows)-1 else self.root
            node.up    = self.rows[i-1] if i > 0 else self.root
        for i, node in enumerate(self.cols):
            node.up    = node
            node.down  = node
            node.right = self.cols[i+1] if i < len(self.cols)-1 else self.root
            node.left  = self.cols[i-1] if i > 0 else self.root
        # set root links to row and column headers
        self.root.right = self.cols[0]
        self.root.left  = self.cols[-1]
        self.root.down  = self.rows[0]
        self.root.up    = self.rows[-1]

    # returns the column header of column with minimum number of Nodes
    def select_min_col(self) -> Node:
        if self.is_empty(): return self.root
        min_node  = self.root.right
        min_count = self.root.right.count
        for col in self.root.itr_right():
            if col.count < min_count:
                min_node  = col
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
        for col_itr in col.itr_down():
            # iterate though each node in row of next_down left to right
            for row_itr in col_itr.itr_right():
                # unlink top and bottom neighbors of each node reduce count in col header
                row_itr.up.down = row_itr.down
                row_itr.down.up = row_itr.up
                row_itr.get_col().count -= 1

    # uncover a column of a node for dancing links algorithm x
    @staticmethod
    def uncover(node: Node) -> None:
        col = node.get_col()
        # iterate through each node in column bottom to top
        for col_itr in col.itr_up():
            # iterate through each node in row of next_up right to left
            for row_itr in col_itr.itr_left():
                # relink top and bottom neighbors of each node and increment count in col header
                row_itr.up.down = row_itr
                row_itr.down.up = row_itr
                row_itr.get_col().count += 1
        # relink left and right neighbors of column header
        col.right.left = col
        col.left.right = col
    
    # search matrix for exact cover, return list of rows as solution if one exists
    def alg_x_search(self) -> list[int]:
        solutions = [] # list of solutions
        
        # recursive search algorithm returns true if solution is found
        def search() -> bool:
            if self.is_empty(): # if matrix is empty exact cover exists
                self.solved = True
                return True

            # select col with least number of nodes
            selected_col = self.select_min_col()
            # if selected col has zero nodes, then this branch has failed
            if selected_col.count < 1 : return False

            # iterate down from selected column
            for col_itr in selected_col.itr_down():
                solutions.append(col_itr.row) # add current row to solutions
                # iterate right through the current solution row
                for sol_node in col_itr.itr_right(excl=False):
                    if sol_node.col >= 0: DL_Matrix.cover(sol_node)

                # search again after covering
                # if solution is found on this branch, leave loop and stop searching
                if search(): break

                # solution not found on this branch, need to uncover columns from this iteration
                solutions.pop() # remove current row from solutions
                # iterate left through the current solution row
                for sol_node in col_itr.left.itr_left(excl=False):
                    if sol_node.col >= 0: DL_Matrix.uncover(sol_node)
            return self.solved

        search()
        return solutions

    # inserts Node into Matrix at row, col
    # if node already exists at row, col - do nothing
    def insert_node(self, row: int, col: int) -> None:
        assert(row >=0 and col >= 0 and row < len(self.rows) and col < len(self.cols))
        # create node to insert
        new_node: Node = Node(self, row, col)
        
        # iterate through the row to find correct placement of new_node in row
        n = self.root
        for n in self.rows[row].itr_right(excl=False):
            if n.right.col == -1 or n.right.col > col: break
        if n.col == col: return # if node already exists, leave
        # reassign left and right pointers
        new_node.right      = n.right
        new_node.left       = n
        new_node.right.left = new_node
        n.right             = new_node

        # iterate through the column to find correct placement of new_node in col
        for n in self.cols[col].itr_down(excl=False):
            if n.down.row == -1 or n.down.row > row: break
        # reassign up and down pointers
        new_node.down    = n.down
        new_node.up      = n
        new_node.down.up = new_node
        n.down           = new_node
        self.cols[col].count += 1

    # returns True if matrix is empty, False otherwise
    def is_empty(self) -> bool:
        return self.root.right == self.root

    # prints Matrix in tabular form
    def print_matrix(self) -> None:
        uln = '\033[4m'
        res = '\033[0m'
        print('n: ', end='')
        # print counts for each column
        for col in self.root.itr_right():
            print(col.count, end=' ')
        print()
        print(uln + "R| ", end='')
        for col in self.root.itr_right():
            print(col.col, end=' ')
        print(res)
        for row in self.root.itr_down():
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
    def __init__(self, matrix: DL_Matrix, row: int, col: int, count: int=1,
                 up: Union[Node,None]=None, down: Union[Node,None]=None,
                 left: Union[Node,None]=None, right: Union[Node,None]=None) -> None:
        self.row    = row
        self.col    = col
        self.count  = count
        self.matrix = matrix
        self.up     = up or self
        self.down   = down or self
        self.left   = left or self
        self.right  = right or self
    
    # iterate full circe from node moving up
    # if excl is True, this node will not be yielded
    def itr_up(self, excl: bool=True) -> Generator[Node,None,None]:
        itr = self
        if not excl: yield itr
        while itr.up != self:
            itr = itr.up
            yield itr

    # iterate full circle from node moving down
    # if excl is True, this node will not be yielded
    def itr_down(self, excl: bool=True) -> Generator[Node,None,None]:
        itr = self
        if not excl: yield itr
        while itr.down != self:
            itr = itr.down
            yield itr

    # iterate full circle from node moving left
    # if excl is True, this node will not be yielded
    def itr_left(self, excl: bool=True) -> Generator[Node,None,None]:
        itr = self
        if not excl: yield itr
        while itr.left != self:
            itr = itr.left
            yield itr

    # iterate full circle from node moving right
    # if excl is True, this node will not be yielded
    def itr_right(self, excl: bool=True) -> Generator[Node,None,None]:
        itr = self
        if not excl: yield itr
        while itr.right != self:
            itr = itr.right
            yield itr

    # return column header of this node
    def get_col(self) -> Node:
        if self.col == -1: return self.matrix.root
        return self.matrix.cols[self.col]

    # returns whether node is in a covered column or not
    def col_is_covered(self) -> bool:
        return not(self.get_col().right.left == self.get_col())
