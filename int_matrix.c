#include <stdio.h>
#include <stdlib.h>

#include "int_matrix.h"

void i_init_matrix(t_intMatrix* m, int w, int h){
    m->width = w;
    m->height = h;
    m->arr = (int*) calloc((w*h), sizeof(int));
}

int i_calc_index(t_intMatrix m, int i, int j){
    return m.width * j + i;
}

void i_set(t_intMatrix* m, int i, int j, int value){
    t_intMatrix mat = *m;
    int index = i_calc_index(mat, i, j);
    m->arr[index] = value;
}

int i_get(t_intMatrix m, int i, int j){
    return m.arr[i_calc_index(m, i, j)];
}

void i_free_matrix(t_intMatrix* m){
    free(m->arr);
}


void i_print_matrix(t_intMatrix m){
    int i, j;

    for(j = 0; j < m.height; j++){
        for(i = 0; i < m.width; i++){

            printf("%d", i_get(m, i, j));
            printf(" ");
        }
        printf("\n");
    }
}
