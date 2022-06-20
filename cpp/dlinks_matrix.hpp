#pragma once
#include <iostream>

//Toroidally linked matrix for solving sudoku puzzles via algorithm x
//Optimized to only work with standard 9x9 puzzles

typedef struct node Node;
typedef struct column Column;

struct node{
    int row, col, count;
    Node* up, *down, *left, *right;
};

struct column{
    int node_ptr;
    Node nodes[10];
};

Column cols[324];
Node* solution_stack[81];
int solution_ptr;
bool solved;
Node counts[11];


//selects next available node in specified column
//updates node attributes, updates node.up and node.up.down ptrs
//returns pointer to selected node
Node* insert(int row, int col){
    int ptr = cols[col].node_ptr;
    Node* insert = &cols[col].nodes[ptr];
    insert->row = row;
    insert->col = col;
    insert->up = &cols[col].nodes[ptr-1];
    insert->up->down = insert;
    ++cols[col].node_ptr;
    ++cols[col].nodes[0].count;
    return insert;
}

//return the column containing the minimum uncovered nodes
//if all columns are covered - return NULL
Node* select_min_column(){
    for(int i=0; i<10; ++i){
        if(counts[i].right != &counts[i]) { return counts[i].right; }
    }
    return 0;
}

//cover a column of node n for dancing links algorithm
void cover(Node* n){
    Node* col_node = &cols[n->col].nodes[0];
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
            Node* cn = &cols[horiz_itr->col].nodes[0];
            --cn->count;
            //if column cn is not covered - push it into new list
            if(cn->right->left == cn){
                cn->right->left = cn->left;
                cn->left->right = cn->right;
                cn->right = counts[cn->count].right;
                cn->left = &counts[cn->count];
                counts[cn->count].right->left = cn;
                counts[cn->count].right = cn;
            }
        }
    }
}

//uncover a column of node n for dancing links algorithm
void uncover(Node* n){
    Node* col_node = &cols[n->col].nodes[0];
    //relink left and right neighbors of col to col
    col_node->right = counts[col_node->count].right;
    col_node->left = &counts[col_node->count];
    counts[col_node->count].right->left = col_node;
    counts[col_node->count].right = col_node;
    //iterate through each 1 in col bottom to top
    for(Node* vert_itr=col_node->up; vert_itr!=col_node; vert_itr=vert_itr->up){
        //iterate through row right to left
        //for each Node in this row, relink top and bottom neighbors and increment count of that column
        for(Node* horiz_itr=vert_itr->left; horiz_itr!=vert_itr; horiz_itr=horiz_itr->left){
            horiz_itr->up->down = horiz_itr;
            horiz_itr->down->up = horiz_itr;
            Node* cn = &cols[horiz_itr->col].nodes[0];
            ++cn->count;
            //if column cn is not covered - push it into new list
            if(cn->right->left == cn){
                cn->right->left = cn->left;
                cn->left->right = cn->right;
                cn->right = counts[cn->count].right;
                cn->left = &counts[cn->count];
                counts[cn->count].right->left = cn;
                counts[cn->count].right = cn;
            }
        }
    }
}

//search the toroidal matrix structure for an exact cover
//returns true if exact cover is found, false otherwise
//solutions_stack will contain all rows making up the solution
bool alg_x_rec_search(){
    Node* selected_col = select_min_column();
    //if no column in matrix, then a solution has been found
    if(selected_col == 0) { return solved = true; }
    //if selected column has 0 Nodes, then this branch has failed
    if(selected_col->count < 1) { return false; }

    Node* vert_itr=selected_col->down;
    Node* horiz_itr;
    //iterate down from selected column head
    while(vert_itr != selected_col){
        //add selected row to solutions LIFO
        solution_stack[solution_ptr] = vert_itr;
        ++solution_ptr;

        horiz_itr = vert_itr;
        //iterate right from vertical iterator, cover each column
        do{
            cover(horiz_itr);
        }while((horiz_itr = horiz_itr->right) != vert_itr);

        //search this matrix again after covering
        //if solution found on this branch, leave loop and stop searching
        if(alg_x_rec_search()) { return true; }

        //solution not found on this iteration's branch, need to revert changes to matrix
        //remove row from solutions, then uncover columns from this iteration
        --solution_ptr;
        horiz_itr = vert_itr->left;
        //iterate left from the last column that was covered, uncover each column
        do{
            uncover(horiz_itr);
        }while((horiz_itr = horiz_itr->left) != vert_itr->left);

        vert_itr = vert_itr->down;
    }
    return solved;
}

//iterative implementation of the search
[[gnu::always_inline]] bool alg_x_itr_search(int start_sols){
    Node* selected_col, *vert_itr, *horiz_itr;

    //select initial column to begin the search
    if((selected_col = select_min_column())->count < 1) { return false; }
    vert_itr = selected_col->down;

    for(;;){
        //select current row as partial solution and cover
        solution_stack[solution_ptr++] = vert_itr;
        horiz_itr = vert_itr;
        do{
            cover(horiz_itr);
        }while((horiz_itr = horiz_itr->right) != vert_itr);

        //select next column and check for solution, and branch failure
        if((selected_col = select_min_column()) == 0) { return true; }
        if(selected_col->count > 0){
            vert_itr = selected_col->down;
            continue;
        }

        //uncover last partial solution
        do{
            if(--solution_ptr < start_sols) { return false; }

            vert_itr = solution_stack[solution_ptr];
            horiz_itr = vert_itr->left;
            do{
                uncover(horiz_itr);
            }while((horiz_itr = horiz_itr->left) != vert_itr->left);
            vert_itr = vert_itr->down;
        //if next column is a header, then continue to uncover
        }while(vert_itr == &cols[vert_itr->up->col].nodes[0]);
    }
}

void quick_cover(Node* n){
    int c = n->col;
    Node* col_node = &cols[c].nodes[0];
    col_node->count = 1000;
    //iterate through each Node in col top to bottom
    Node* vert_itr = col_node->down;
    while(vert_itr != NULL && vert_itr->col == c){
        //iterate through row left to right
        //for each Node in this row, unlink top and bottom neighbors and reduce count of that column
        for(Node* horiz_itr=vert_itr->right; horiz_itr!=vert_itr; horiz_itr=horiz_itr->right){
            if(horiz_itr->down != NULL && horiz_itr->down->up == horiz_itr){
                horiz_itr->up->down = horiz_itr->down;
                horiz_itr->down->up = horiz_itr->up;
            }
            else{
                --cols[horiz_itr->col].node_ptr;
            }
            --cols[horiz_itr->col].nodes[0].count;
        }
        vert_itr = vert_itr->down;
    }
}

//convert int array representing puzzle into constraint matrix for algorithm x
//inputs: int array - this implementation only works on a standard 9x9 puzzle
bool solve_puzzle(char* sudoku_list){
    //hardcoded lookup tables for the 4 constraints of a 9x9 sudoku
    int one_c[] = { 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,20,20,20,20,20,20,20,20,20,21,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,22,23,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,32,32,32,32,32,32,32,32,32,33,33,33,33,33,33,33,33,33,34,34,34,34,34,34,34,34,34,35,35,35,35,35,35,35,35,35,36,36,36,36,36,36,36,36,36,37,37,37,37,37,37,37,37,37,38,38,38,38,38,38,38,38,38,39,39,39,39,39,39,39,39,39,40,40,40,40,40,40,40,40,40,41,41,41,41,41,41,41,41,41,42,42,42,42,42,42,42,42,42,43,43,43,43,43,43,43,43,43,44,44,44,44,44,44,44,44,44,45,45,45,45,45,45,45,45,45,46,46,46,46,46,46,46,46,46,47,47,47,47,47,47,47,47,47,48,48,48,48,48,48,48,48,48,49,49,49,49,49,49,49,49,49,50,50,50,50,50,50,50,50,50,51,51,51,51,51,51,51,51,51,52,52,52,52,52,52,52,52,52,53,53,53,53,53,53,53,53,53,54,54,54,54,54,54,54,54,54,55,55,55,55,55,55,55,55,55,56,56,56,56,56,56,56,56,56,57,57,57,57,57,57,57,57,57,58,58,58,58,58,58,58,58,58,59,59,59,59,59,59,59,59,59,60,60,60,60,60,60,60,60,60,61,61,61,61,61,61,61,61,61,62,62,62,62,62,62,62,62,62,63,63,63,63,63,63,63,63,63,64,64,64,64,64,64,64,64,64,65,65,65,65,65,65,65,65,65,66,66,66,66,66,66,66,66,66,67,67,67,67,67,67,67,67,67,68,68,68,68,68,68,68,68,68,69,69,69,69,69,69,69,69,69,70,70,70,70,70,70,70,70,70,71,71,71,71,71,71,71,71,71,72,72,72,72,72,72,72,72,72,73,73,73,73,73,73,73,73,73,74,74,74,74,74,74,74,74,74,75,75,75,75,75,75,75,75,75,76,76,76,76,76,76,76,76,76,77,77,77,77,77,77,77,77,77,78,78,78,78,78,78,78,78,78,79,79,79,79,79,79,79,79,79,80,80,80,80,80,80,80,80,80,};
    int row_c[] = { 81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161,153,154,155,156,157,158,159,160,161};
    int col_c[] = { 162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242};
    int box_c[] = { 243,244,245,246,247,248,249,250,251,243,244,245,246,247,248,249,250,251,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,252,253,254,255,256,257,258,259,260,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,261,262,263,264,265,266,267,268,269,261,262,263,264,265,266,267,268,269,243,244,245,246,247,248,249,250,251,243,244,245,246,247,248,249,250,251,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,252,253,254,255,256,257,258,259,260,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,261,262,263,264,265,266,267,268,269,261,262,263,264,265,266,267,268,269,243,244,245,246,247,248,249,250,251,243,244,245,246,247,248,249,250,251,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,252,253,254,255,256,257,258,259,260,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,261,262,263,264,265,266,267,268,269,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,270,271,272,273,274,275,276,277,278,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,279,280,281,282,283,284,285,286,287,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,288,289,290,291,292,293,294,295,296,288,289,290,291,292,293,294,295,296,270,271,272,273,274,275,276,277,278,270,271,272,273,274,275,276,277,278,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,279,280,281,282,283,284,285,286,287,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,288,289,290,291,292,293,294,295,296,288,289,290,291,292,293,294,295,296,270,271,272,273,274,275,276,277,278,270,271,272,273,274,275,276,277,278,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,279,280,281,282,283,284,285,286,287,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,288,289,290,291,292,293,294,295,296,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,297,298,299,300,301,302,303,304,305,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,306,307,308,309,310,311,312,313,314,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,315,316,317,318,319,320,321,322,323,315,316,317,318,319,320,321,322,323,297,298,299,300,301,302,303,304,305,297,298,299,300,301,302,303,304,305,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,306,307,308,309,310,311,312,313,314,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,315,316,317,318,319,320,321,322,323,315,316,317,318,319,320,321,322,323,297,298,299,300,301,302,303,304,305,297,298,299,300,301,302,303,304,305,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,306,307,308,309,310,311,312,313,314,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,315,316,317,318,319,320,321,322,323,315,316,317,318,319,320,321,322,323};

    //initialize matrix
    for(int i=0; i<324; ++i){
        cols[i].nodes[0].count = 0;
        cols[i].node_ptr = 1;
    }
    solved = false;
    solution_ptr = 0;
    for(int i=0; i<11; ++i){
        counts[i].right = &counts[i];
        counts[i].left = &counts[i];
    }

    //iterate through sudoku list
    int row = 0;
    Node* r1, *r2, *r3, *r4;
    for(int i=0; i<81; ++i){
        //if no value assigned to cell, populate all rows representing all possible candidate values for cell
        if(sudoku_list[i] - '0' == 0){
            for(int j=0; j<9; ++j){
                row = i*9+j;
                r1 = insert(row, one_c[row]);
                r2 = insert(row, row_c[row]);
                r3 = insert(row, col_c[row]);
                r4 = insert(row, box_c[row]);
                r1->right = r2; r2->right = r3; r3->right = r4; r4->right = r1;
                r4->left = r3; r3->left = r2; r2->left = r1; r1->left = r4;
            }
        }
        //otherwise only populate the row representing the known assigned value
        else{
            row = i*9+sudoku_list[i]-1-'0';
            r1 = insert(row, one_c[row]);
            r2 = insert(row, row_c[row]);
            r3 = insert(row, col_c[row]);
            r4 = insert(row, box_c[row]);
            r1->right = r2; r2->right = r3; r3->right = r4; r4->right = r1;
            r4->left = r3; r3->left = r2; r2->left = r1; r1->left = r4;
        }
    }

    // for(int i=0; i<solution_ptr; ++i){
    //     Node* sol_node = solution_stack[i];
    //     Node* sol_itr = sol_node;
    //     do{
    //         quick_cover(sol_itr);
    //     }while((sol_itr = sol_itr->right) != sol_node);
    // }

    // for(Node* covered_col=counts[10].right; covered_col!=&counts[10]; covered_col=covered_col->right){
    //     quick_cover(covered_col);
    // }


    //finalize toroidal structure of columns
    for(int i=0; i<324; ++i){
        int ptr = cols[i].node_ptr - 1;
        Node* cn = &cols[i].nodes[0];
        cn->up = &cols[i].nodes[ptr];
        cols[i].nodes[ptr].down = cn;

        if(cols[i].nodes[0].count > 9) { continue; }
        cn->left = &counts[cn->count];
        cn->right = counts[cn->count].right;
        counts[cn->count].right->left = cn;
        counts[cn->count].right = cn;
    }

    return alg_x_itr_search(solution_ptr);
    // return alg_x_rec_search();
}