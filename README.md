# Exact Cover Sudoku Solver
C, C++, and Python implementations of [Algorithm X](https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X) for solving an exact cover problem.  The C and Python versions are generic and will work with a matrix of any size. The C++ version has been designed to work specifically for 9x9 Sudoku puzzles.

Cloning and running the C and Python versions will solve a sample 16x16 puzzle.  Running the C++ version will solve all 49,151 *17-Sudoku* puzzles listed in *puzzles.txt*, and write the solutions to *solutions.txt*.  It accomplishes this in 1.3 seconds on my machine.

All 3 implementations are capable of translating an int array representing a Sudoku puzzle into a linked matrix to search for an exact cover. An int array is returned holding the solution to the puzzle if one exists.  The C++ implementation only accepts arrays of size 81 (9x9 puzzles). The other implementations can handle arrays representing any perfect square puzzle e.g. 4x4, 9x9, 16x6, 25x25, etc..

Alternatively, the C and Python implementations can be used without the Sudoku translation step to create and populate a linked matrix of any size, then search for an Exact Cover for any problem.

I have also created an [algorthm x visualizer](https://nstagman.github.io/algx_visualizer/) to view the constraint matrix as it is modified during a search.

## Exact Cover Problem
The [formal definition](https://en.wikipedia.org/wiki/Exact_cover#Formal_definition) from wikipedia: Given a set *__X__*, and a collection of subsets *__S__*, an exact cover of *__X__* is a subcollection, *__S&ast;__*, that contains each element of *__X__* exactly one time.

In the following example, *__X__* = {0,1,2,3,4,5,6} and *__S__* = {A,B,C,D,E,F} where **A**={0,3,6} **B**={0,3} **C**={3,4,6} **D**={2,4,5} **E**={1,2,5,6} **F**={1,6}. The 7 columns in the matrix represent the 7 elements of *__X__* and the 6 rows of the matrix represent the 6 subsets of *__S__*.  The first row, **A**, has a one in columns 0, 3, and 6, while the second row, **B**, has a one in columns 0 and 3, etc..

Rows = *__S__*, Cols =  *__X__* | *__S&ast;__* = {**B**,**D**,**F**}
:-:|:-:
![matrix_unsolved](https://user-images.githubusercontent.com/35941942/173413772-53a1cfff-cc8f-4b01-bcce-a57f44f3e9fa.png) | ![matri_solvedx](https://user-images.githubusercontent.com/35941942/173413784-208000c9-8d9b-4f92-8a0a-f56f23e8748c.png)

An exact cover is a set of rows that contain a one in each column exactly one time. The only exact cover for this problem is the subset {**B**,**D**,**F**}, that is, the rows **B**, **D**, and **F** have a one in each column exactly one time.

## Sudoku as an Exact Cover

A Sudoku puzzle can be represented as an exact cover problem by turning the rules of the puzzle into constraints for the binary matrix. Each constraint becomes one column of the matrix. A standard 9x9 Sudoku puzzle has four rules.  The first is that each cell in the puzzle must have a number.  This seems obvious, but this requirement needs to be added to the constraint matrix to find a solution.  The final three rules are that each row, column, and house must contain the numbers 1-9.  These numbers cannot be repeated, therefore each number must be unique in its row, column, and house.

4x4 Sudoku Constraint Matrix
:-:
![fxf_l](https://user-images.githubusercontent.com/35941942/173417887-9bfe7a4f-548d-4872-8749-75a846b26e27.png)

This image shows the constraint matrix for a 4x4 Sudoku. The first row contains the root of the matrix and the column header nodes. Every column has a special header node that tracks the number of nodes in the column and has references to the first and last node in the column. The remaining rows represent a possible candidate for the puzzle - there are 64 rows (**16 cells * 4 candidates per cell**).  Each column represents a constraint for the puzzle - there are 64 columns (**16 columns** for each of the **4 constraints**). The first 16 columns represent the value assigned constraint. Column 1 represents a value being assigned in cell 1, column 2 represents a value being assigned to cell 2, etc..  The next three constraints (columns 17-32, 33-48, 49-64), represent the row, column, and house constraints.  The first column of each set represents a 1 being assigned the to the first row, column, or house while the last column of each set represents a 4 being assigned to the fourth row, column, or house respectively.

I created the following functions to return the column number in the constraint matrix for each constraint when given the row of the matrix and the size of the puzzle; I'm sure there is some still room for simplification here. These functions will work with any perfect square puzzle e.g. 4x4, 9x9, 16x6, 25x25, etc..

<table>
<tr>
<th>Sudoku Constraint Calculators</th>
</tr>
<tr>
<td>
    
```python
def _one_constraint(row: int, dim:int) -> int:
    return row//dim
def _row_constraint(row:int, dim:int) -> int:
    return dim**2 + dim*(row//(dim**2)) + row % dim
def _col_constraint(row:int, dim:int) -> int:
    return 2*(dim**2) + (row % (dim**2))
def _box_constraint(row:int, dim:int) -> int:
    return (int(3*(dim**2)
            + (row//(sqrt(dim)*dim**2))*(dim*sqrt(dim))
            + ((row//(sqrt(dim)*dim)) % sqrt(dim))*dim
            + (row % dim)))
```
    
</td>
</tr>
</table>

## Finding an Exact Cover
Donald Knuth's [Dancing Links](https://en.wikipedia.org/wiki/Dancing_Links) methodology represents this problem with a linked binary matrix.  Each node in this matrix begins doubly linked to all 4 neighboring nodes (up, down, left, and right). A technique called **covering** is used, which is reassigning the links of a node to temporarily *remove* it from the matrix.  When backtracking is required, **uncovering** is used to *add* the node back in to the matrix.

The following are my cover and uncover functions in Python. Covering starts by *unlinking* the column header from its horizontal neighbors so they can no longer reference the covered column. We then iterate **down** from the column header and **right** through each row, unlinking the vertical neighbors of each node.  The column can longer be accessed by iterating horizontally, and the nodes can no longer be accessed by iterating vertically.  This *hides* these nodes in the matrix but still allows them to be accessible when needed.  When we need to uncover these nodes, we perform the covering steps in reverse: Iterate **up** from column header &rarr; Iterate **left** through each row &rarr; Relink the covered nodes to their neighbors.
<table>
<tr>
<th>Cover</th>
<th>Uncover</th>
</tr>
<tr>
<td>

```python
def cover(node: Node) -> None:
    col = node.get_col()
    col.right.left = col.left
    col.left.right = col.right
    for col_itr in col.itr_down():
        for row_itr in col_itr.itr_right():
            row_itr.up.down = row_itr.down
            row_itr.down.up = row_itr.up
            row_itr.get_col().count -= 1
```

</td>
<td>

```python
def uncover(node: Node) -> None:
    col = node.get_col()
    for col_itr in col.itr_up():
        for row_itr in col_itr.itr_left():
            row_itr.up.down = row_itr
            row_itr.down.up = row_itr
            row_itr.get_col().count += 1
    col.right.left = col
    col.left.right = col
```

</td>
</tr>
</table>


#### The Algorithm can be broken down into a few basic steps:

- **Check if the matrix is empty:** If the matrix is empty, the solution has been found and the current partial solution is the solution

- **Select a column:** Selecting the column with the fewest number of nodes will result in the fewest iterations.  If this column has zero nodes, then this search branch has failed.  The algorithm must backtrack and uncover the nodes that were covered during the last iteration.

- **Select partial solution from column:** Once a column with nodes is selected, one of the rows in the column is selected as a partial solution.

- **Cover partial solution:** Each node in the partial solution is covered.

- **Repeat** this algorithm on the newly covered matrix

Returning to the 6x7 matrix example from above, we can step through the algorithm and visualize the updated matrix along the way.  Column 0 is selected since it has the fewest number of nodes (2). Row 0 is then selected as a partial solution since its the first row in the column. Row 0 has nodes in columns 0, 3, and 6. All 3 of these columns need to be covered using the method shown above. The following image shows the matrix after covering the Row 0 partial solution.  

Unsolved | Select Row 0 | Row 0 Covered
:-:|:-:|:-:
![begin](https://user-images.githubusercontent.com/35941942/173422509-29c6adaf-f5c1-4b35-841a-2588c4b783d7.png) | ![r0](https://user-images.githubusercontent.com/35941942/173422343-ddca42de-f01b-49bc-a5af-39f1098f6062.png) | ![uncover](https://user-images.githubusercontent.com/35941942/173422355-e2f7d777-39bb-4f65-bed1-a8d0f076213d.png)

When searching on the next iteration, we will select column 1 since it has the least number of nodes. Column 1 has 0 nodes, which means this branch of the search has failed, Row 0 is not part of the solution, and we need to uncover the Row 0 partial solution. After uncovering we are back to the original matrix but have exhausted Row 0 as a partial solution.  Row 1 is then selected since it is the next row in column 0 (the original selected column).  The algorithm covers columns 0 and 3 &rarr; Selects row 3 as a partial solution &rarr; Covers columns 2, 4, and 5 &rarr; Selects row 5 as a partial solution &rarr; Covers columns 1 and 6.  At this point the matrix is empty, meaning the search is complete and rows 1, 3, and 5 are an exact cover of this constraint matrix.

Select Row 1 | Select Row 3 | Select Row 5 
:-:|:-:|:-:
![r1](https://user-images.githubusercontent.com/35941942/173422895-8db85e55-cf6c-4bc6-a346-ccb09d2730c1.png) | ![r3](https://user-images.githubusercontent.com/35941942/173422904-975c2ab5-5a81-426e-8c24-dd129647df26.png) | ![r5](https://user-images.githubusercontent.com/35941942/173422917-676eb4ed-28a5-4817-ab94-f2ab4ebfb101.png)

This algorithm can be seen in more detail using this [animated interactive web app](https://nstagman.github.io/algx_visualizer/).
