#include <stdio.h>
#include <stdlib.h>
#include "algx.h"
#include "lifo.h"

int main(){
    
    Matrix* matrix = create_matrix(6,7);
    lifo* stack = create_stack();
    
    insert_node(matrix, 0, 0, 1);
    insert_node(matrix, 0, 3, 1);
    insert_node(matrix, 0, 6, 1);
    insert_node(matrix, 1, 0, 1);
    insert_node(matrix, 1, 3, 1);
    insert_node(matrix, 2, 3, 1);
    insert_node(matrix, 2, 4, 1);
    insert_node(matrix, 2, 6, 1);
    insert_node(matrix, 3, 2, 1);
    insert_node(matrix, 3, 4, 1);
    insert_node(matrix, 3, 5, 1);
    insert_node(matrix, 4, 1, 1);
    insert_node(matrix, 4, 2, 1);
    insert_node(matrix, 4, 5, 1);
    insert_node(matrix, 4, 6, 1);
    insert_node(matrix, 5, 1, 1);
    insert_node(matrix, 5, 6, 1);

    alg_x_search(matrix, stack);
    printf("Solution: ");
    for(lifo_node* itr=stack->head; itr!=NULL; itr=itr->next){
        printf("%c, ", itr->data->row+65);
    }
    printf("\n");
    print_matrix(matrix);

    delete_matrix(matrix);
    delete_stack(stack);

    return 0;
}