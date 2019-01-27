#ifndef DOUBLE_MATRIX_H_INCLUDED
#define DOUBLE_MATRIX_H_INCLUDED

typedef struct doubleMatrix{
    int width;
    int height;
    double* arr;
} t_doubleMatrix;


void d_init_matrix(t_doubleMatrix* m, int w, int h);
int d_calc_index(t_doubleMatrix m, int i, int j);
void d_set(t_doubleMatrix* m, int i, int j, double value);
double d_get(t_doubleMatrix m, int i, int j);
void d_print_matrix(t_doubleMatrix m);
void d_normalize(t_doubleMatrix* mat);
void d_free_matrix(t_doubleMatrix* m);

#endif // DOUBLE_MATRIX_H_INCLUDED
