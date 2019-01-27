#ifndef INT_MATRIX_H_INCLUDED
#define INT_MATRIX_H_INCLUDED


typedef struct intMatrix{
    int width;
    int height;
    int* arr;
} t_intMatrix;


void i_init_matrix(t_intMatrix* m, int w, int h);
int i_calc_index(t_intMatrix m, int i, int j);
void i_set(t_intMatrix* m, int i, int j, int value);
int i_get(t_intMatrix m, int i, int j);
void i_print_matrix(t_intMatrix m);
void i_free_matrix(t_intMatrix* m);


#endif // INT_MATRIX_H_INCLUDED
