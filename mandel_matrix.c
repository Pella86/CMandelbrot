#include "mandel_matrix.h"

#include <stdio.h>
#include <stdlib.h>

#include "custom_complex.h"


void m_init_matrix(t_mandelMatrix* m, int w, int h){
    m->width = w;
    m->height = h;
    m->arr = (t_MandelResult*) calloc((w*h), sizeof(t_MandelResult));
}

int m_calc_index(t_mandelMatrix m, int i, int j){
    return m.width * j + i;
}

void m_set(t_mandelMatrix* m, int i, int j, t_MandelResult value){
    int index = m_calc_index(*m, i, j);
    m->arr[index] = value;
}

t_MandelResult m_get(t_mandelMatrix m, int i, int j){
    return m.arr[m_calc_index(m, i, j)];
}

void m_free_matrix(t_mandelMatrix* m){
    free(m->arr);
}

void print_MandelResult(t_MandelResult result){
    char* in_set = (result.in_set)? "true" : "false";
    printf("%d %s ", result.iterations, in_set);
    print_complex(result.value);
}

void m_print_matrix(t_mandelMatrix m){
    int i, j;

    for(j = 0; j < m.height; j++){
        for(i = 0; i < m.width; i++){

            print_MandelResult(m_get(m, i, j));
            printf(" ");
        }
        printf("\n");
    }
}

