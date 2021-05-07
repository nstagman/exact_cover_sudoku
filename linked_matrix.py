from __future__ import annotations


@staticmethod
def cover(node: Node) -> None:
    col = node.get_col()
    col.right.left = col.left
    col.left.right = col.right
    for d in col.itr_down(excl=True):
        for r in d.itr_right(excl=True):
            r.up.down = r.down
            r.down.up = r.up
            r.get_col().count -= 1

@staticmethod
def uncover(node: Node) -> None:
    col = node.get_col()
    for u in col.itr_up(excl=True):
        for l in u.itr_left(excl=True):
            l.up.down = l
            l.down.up = l
            l.get_col().count += 1
    col.right.left = col
    col.left.right = col


# Toroidally Linked Matrix
class DL_Matrix:
    def __init__(self, num_rows:int, num_cols:int) -> None:
        self.root: Node = Node(self, -1, -1)
        self.rows: list[Node] = [Node(self, row=i, col=-1) for i in range(num_rows)]
        self.cols: list[Node] = [Node(self, row=-1, col=i, count=0) for i in range(num_cols)]
        self.solved: bool = False

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
        self.root.right = self.cols[0]
        self.root.left = self.cols[-1]
        self.root.down = self.rows[0]
        self.root.up = self.rows[-1]

    # Returns True if matrix is empty, False Otherwise
    def is_empty(self):
        return self.root.right == self.root

    # Returns the header of column with minimum number of 1's
    def select_min_col(self) -> Node:
        if self.is_empty(): return self.root
        min_node = self.root.right
        min_count = self.root.right.count
        for col in self.root.itr_right(excl=True):
            if col.count < min_count:
                min_node = col
                min_count = col.count
        return min_node

    # Inserts Node into Matrix at row, col
    # If node already exists at row, col: do nothing
    def insert_node(self, row:int, col:int) -> None:
        assert(row >=0 and col >= 0 and row < len(self.rows) and col < len(self.cols))

        new_node: Node = Node(self, row, col)
        for n in self.rows[row].itr_right():
            if n.right.col == -1 or n.right.col > col:
                break
        if n.col == col:
            return
        new_node.right = n.right
        new_node.left = n
        n.right = new_node
        new_node.right.left = new_node

        for n in self.cols[col].itr_down():
            if n.down.row == -1 or n.down.row > row:
                break
        new_node.down = n.down
        new_node.up = n
        n.down = new_node
        new_node.down.up = new_node

        self.cols[col].count += 1

    # Prints Matrix in tabular form
    def print_matrix(self):
        print("\033[4mR| ", end='')
        for col in self.root.itr_right(excl=True):
            print(col.col, end=' ')
        print("\033[0m")
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
    def __init__(self, matrix:DL_Matrix, row:int, col:int, count:int=-1, up:Node=None, down:Node=None, left:Node=None, right:Node=None) -> None:
        self.row: int = row
        self.col: int = col
        self.count: int = count
        self.matrix: DL_Matrix = matrix
        self.up: Node = up
        self.down: Node = down
        self.left: Node = left
        self.right: Node = right
    
    # iterate full circe from node moving up
    # If excl is True, this node will not be yielded
    def itr_up(self, excl=False):
        start, itr = self, self if not excl else self.up
        yield itr
        while itr.up != start:
            itr = itr.up
            yield itr

    # iterate full circle from node moving down
    # If excl is True, this node will not be yielded
    def itr_down(self, excl=False):
        start, itr = self, self if not excl else self.down
        yield itr
        while itr.down != start:
            itr = itr.down
            yield itr

    # iterate full circle from node moving left
    # If excl is True, this node will not be yielded
    def itr_left(self, excl=False):
        start, itr = self, self if not excl else self.left
        yield itr
        while itr.left != start:
            itr = itr.left
            yield itr

    # iterate full circle from node moving right
    # If excl is True, this node will not be yielded
    def itr_right(self, excl=False):
        start, itr = self, self if not excl else self.right
        yield itr
        while itr.right != start:
            itr = itr.right
            yield itr

    # return column header of this node
    def get_col(self) -> Node:
        return self.matrix.cols[self.col]

    # returns whether node is in a covered column or not
    def col_is_covered(self) -> bool:
        return not(self.get_col().right.left == self.get_col())
