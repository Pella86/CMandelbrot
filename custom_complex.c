#include "custom_complex.h"

#include <math.h>
#include <stdio.h>



void print_complex(t_complex c){
    printf("(%.2f:%.2f)", c.real, c.imag);
}

t_complex add(t_complex a, t_complex b){
    t_complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

t_complex mul(t_complex a, t_complex b){
    t_complex result;

    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

double squared_module(t_complex a){
    return a.real * a.real + a.imag * a.imag;
}

double module(t_complex a){
    return sqrt(squared_module(a));
}

double phase(t_complex a){
    return atan2(a.imag, a.real);
}


//void build_complex_string(char * dst, t_complex c){
//    strcat(dst, "(");
//
//    char real[10];
//    sprintf(real, "%.2f", c.real);
//    strcat(dst, real);
//
//    strcat(dst, ":");
//
//    char imag[10];
//    sprintf(imag, "%.2f", c.imag);
//    strcat(dst, imag);
//    strcat(dst, ")");
//}
