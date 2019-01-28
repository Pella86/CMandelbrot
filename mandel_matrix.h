#ifndef MANDEL_MATRIX_H_INCLUDED
#define MANDEL_MATRIX_H_INCLUDED

#include "custom_complex.h"

typedef enum { false, true } bool;

typedef struct MandelResult{
    int iterations;
    bool in_set;
    t_complex value;
} t_MandelResult;

typedef struct mandelMatrix{
    int width;
    int height;
    t_MandelResult* arr;
} t_mandelMatrix;


void m_init_matrix(t_mandelMatrix* m, int w, int h);
int m_calc_index(t_mandelMatrix m, int i, int j);
void m_set(t_mandelMatrix* m, int i, int j, t_MandelResult value);
t_MandelResult m_get(t_mandelMatrix m, int i, int j);
void m_print_matrix(t_mandelMatrix m);
void m_normalize(t_mandelMatrix* mat);
void m_free_matrix(t_mandelMatrix* m);


#endif // MANDEL_MATRIX_H_INCLUDED
