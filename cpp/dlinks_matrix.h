#pragma once
#include <iostream>

typedef struct node Node;
typedef struct column Column;
typedef struct matrix Matrix;


struct node{
    int row, col, count;
    Node* up, *down, *left, *right;
};

struct column{
    int node_ptr;
    Node header;
    Node nodes[9];
};

struct matrix{
    Node root;
    Column cols[324];
    int solution_stack[81];
    int solution_ptr;
    bool solved;

    void init_matrix(){
        root.right = &cols[0].header;
        root.left = &cols[323].header;
        cols[0].header.right = &cols[1].header;
        cols[0].header.left = &root;
        cols[0].header.count = 0;
        cols[0].node_ptr = 0;
        cols[323].header.right = &root;
        cols[323].header.left = &cols[322].header;
        cols[323].header.count = 0;
        cols[323].node_ptr = 0;
        for(int i=1; i<323; i++){
            cols[i].header.right = &cols[i+1].header;
            cols[i].header.left = &cols[i-1].header;
            cols[i].header.count = 0;
            cols[i].node_ptr = 0;
        }
        solved = false;
        solution_ptr = 0;
    }

    Node* insert(int row, int col){
        int ptr = cols[col].node_ptr;
        Node* insert = &cols[col].nodes[ptr];
        insert->row = row;
        insert->col = col;
        if(ptr == 0) {
            insert->up = &cols[col].header;
            cols[col].header.down = insert;
        }
        else {
            insert->up = &cols[col].nodes[ptr-1];
            insert->up->down = insert;
        }
        cols[col].node_ptr++;
        cols[col].header.count++;
        return insert;
    }

    void insert_finalize(){
        for(int i=0; i<324; i++){
            int ptr = cols[i].node_ptr - 1;
            cols[i].header.up = &cols[i].nodes[ptr];
            cols[i].nodes[ptr].down = &cols[i].header;
        }
    }

    //return column header of column with least number of Nodes in matrix
    Node* select_min_column(){
        Node* itr = root.right;
        Node* min_node = itr;
        int min_count  = itr->count;
        while(itr->right != &root){
            itr = itr->right;
            if(itr->count < min_count){
                min_node  = itr;
                min_count = itr->count;
                if(min_count < 2) { break; }
            }
        }
        return min_node;
    }

    //cover a column of node n for dancing links algorithm
    void cover(Node* n){
        Node* col_node = &cols[n->col].header;
        //unlink left and right neighbors of col from col
        col_node->right->left = col_node->left;
        col_node->left->right = col_node->right;
        //iterate through each Node in col top to bottom
        for(Node* vert_itr=col_node->down; vert_itr!=col_node; vert_itr=vert_itr->down){
            //iterate through row left to right
            //for each Node in this row, unlink top and bottom neighbors and reduce count of that column
            for(Node* horiz_itr=vert_itr->right; horiz_itr!=vert_itr; horiz_itr=horiz_itr->right){
                horiz_itr->up->down = horiz_itr->down;
                horiz_itr->down->up = horiz_itr->up;
                cols[horiz_itr->col].header.count--;
            }
        }
    }

    //uncover a column of node n for dancing links algorithm
    void uncover(Node* n){
        Node* col_node = &cols[n->col].header;
        //iterate through each 1 in col bottom to top
        for(Node* vert_itr=col_node->up; vert_itr!=col_node; vert_itr=vert_itr->up){
            //iterate through row right to left
            //for each Node in this row, relink top and bottom neighbors and increment count of that column
            for(Node* horiz_itr=vert_itr->left; horiz_itr!=vert_itr; horiz_itr=horiz_itr->left){
                horiz_itr->up->down = horiz_itr;
                horiz_itr->down->up = horiz_itr;
                cols[horiz_itr->col].header.count++;
            }
        }
        //relink left and right neighbors of col to col
        col_node->right->left = col_node;
        col_node->left->right = col_node;
    }

    //search the toroidal matrix structure for an exact cover
    //returns true if exact cover is found, false otherwise
    //solutions LIFO will contain all rows making up the solution
    bool alg_x_search(){
        //if matrix is empty then an exact cover exists, return true
        if(root.right == &root) { return solved = true; }
        //select the column with least number of Nodes
        Node* selected_col = select_min_column();
        //if selected column has 0 Nodes, then this branch has failed
        if(selected_col->count < 1) { return false; }

        Node* vert_itr=selected_col->down;
        Node* horiz_itr;
        //iterate down from selected column head
        while(vert_itr != selected_col && !solved){
            //add selected row to solutions LIFO
            solution_stack[solution_ptr] = vert_itr->row;
            solution_ptr++;

            horiz_itr = vert_itr;
            //iterate right from vertical iterator, cover each column
            do{
                cover(horiz_itr);
            }while((horiz_itr = horiz_itr->right) != vert_itr);

            //search this matrix again after covering
            //if solution found on this branch, leave loop and stop searching
            if(alg_x_search()) { break; }

            //solution not found on this iteration's branch, need to revert changes to matrix
            //remove row from solutions, then uncover columns from this iteration
            solution_ptr--;
            horiz_itr = vert_itr->left;
            //iterate left from the last column that was covered, uncover each column
            do{
                uncover(horiz_itr);
            }while((horiz_itr = horiz_itr->left) != vert_itr->left);

            vert_itr = vert_itr->down;
        }
        return solved;
    }
};