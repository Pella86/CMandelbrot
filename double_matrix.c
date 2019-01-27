#include <stdio.h>
#include <stdlib.h>

#include "double_matrix.h"
#include "math.h"

void d_init_matrix(t_doubleMatrix* m, int w, int h){
    m->width = w;
    m->height = h;
    m->arr = (double*) calloc((w*h), sizeof(double));
}

int d_calc_index(t_doubleMatrix m, int i, int j){
    return m.width * j + i;
}

void d_set(t_doubleMatrix* m, int i, int j, double value){
    t_doubleMatrix mat = *m;
    int index = d_calc_index(mat, i, j);
    m->arr[index] = value;
}

double d_get(t_doubleMatrix m, int i, int j){
    return m.arr[d_calc_index(m, i, j)];
}

void d_free_matrix(t_doubleMatrix* m){
    free(m->arr);
}


void d_print_matrix(t_doubleMatrix m){
    int i, j;

    for(j = 0; j < m.height; j++){
        for(i = 0; i < m.width; i++){

            printf("%.2f", d_get(m, i, j));
            printf(" ");
        }
        printf("\n");
    }
}

double f_max_mat(t_doubleMatrix m){
    double max = d_get(m, 0, 0);
    int i;

    for(i = 1; i < m.height * m.width; i++){
        double cur = m.arr[i];
        max = fmax(cur, max);

    }
    return max;
}

double f_min_mat(t_doubleMatrix m){
    double min = d_get(m, 0, 0);
    int i;

    for(i = 1; i < m.height * m.width; i++){
        double cur = m.arr[i];
        min = fmin(cur, min);

    }
    return min;
}

void d_normalize(t_doubleMatrix* mat){
    // find max
    // find min
    double min_mat = f_min_mat(*mat);
    double max_mat = f_max_mat(*mat);

    int i, j;

    for(j = 0; j < mat->height; j++){
        for(i = 0; i < mat->width; i++){
            if(min_mat != max_mat){
                double getv = d_get(*mat, i, j);
                double normv = (getv - min_mat) / (max_mat - min_mat);
                d_set(mat, i, j, normv);
            }
        }
    }
}
