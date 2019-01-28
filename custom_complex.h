#ifndef CUSTOM_COMPLEX_H_INCLUDED
#define CUSTOM_COMPLEX_H_INCLUDED

typedef struct complex{
    double real, imag;
} t_complex;

void print_complex(t_complex c);
t_complex add(t_complex a, t_complex b);
t_complex mul(t_complex a, t_complex b);
double squared_module(t_complex a);
double module(t_complex a);
double phase(t_complex a);

#endif // CUSTOM_COMPLEX_H_INCLUDED
