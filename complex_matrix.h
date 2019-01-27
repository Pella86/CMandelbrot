#ifndef COMPLEX_MATRIX_H_INCLUDED
#define COMPLEX_MATRIX_H_INCLUDED

#include "custom_complex.h"

typedef struct cMatrix{
    int width;
    int height;
    t_complex* arr;
} t_cMatrix;


void c_init_matrix(t_cMatrix* m, int w, int h);
int c_calc_index(t_cMatrix m, int i, int j);
void c_set(t_cMatrix* m, int i, int j, t_complex value);
t_complex c_get(t_cMatrix m, int i, int j);
void c_free_matrix(t_cMatrix* m);
void c_print_matrix(t_cMatrix m);



#endif // COMPLEX_MATRIX_H_INCLUDED
