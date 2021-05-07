from linked_matrix import *


matrix = DL_Matrix(4,4)


# for n in matrix.rows[2].itr_up():
#     print(n.row)
# for n in matrix.root.itr_down():
#     print(n.row)
# for n in matrix.root.itr_right():
#     print(n.col)
# for n in matrix.root.itr_left():
#     print(n.col)

matrix.insert_node(1,2)
matrix.insert_node(0,1)
matrix.insert_node(0,0)
matrix.insert_node(2,0)
matrix.insert_node(3,1)
matrix.insert_node(3,3)

matrix.print_matrix()
print(matrix.select_min_col().col)