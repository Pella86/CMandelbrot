#include <stdio.h>
#include <stdlib.h>

#include "complex_matrix.h"


void c_init_matrix(t_cMatrix* m, int w, int h){
    m->width = w;
    m->height = h;
    m->arr = (t_complex*) calloc((w*h), sizeof(t_complex));
}

int c_calc_index(t_cMatrix m, int i, int j){
    return m.width * j + i;
}

void c_set(t_cMatrix* m, int i, int j, t_complex value){
    t_cMatrix mat = *m;
    int index = c_calc_index(mat, i, j);
    m->arr[index] = value;
}

t_complex c_get(t_cMatrix m, int i, int j){
    return m.arr[c_calc_index(m, i, j)];
}

void c_free_matrix(t_cMatrix* m){
    free(m->arr);
}


void c_print_matrix(t_cMatrix m){
    int i, j;

    for(j = 0; j < m.height; j++){
        for(i = 0; i < m.width; i++){

            print_complex(c_get(m, i, j));
            printf(" ");
        }
        printf("\n");
    }
}
