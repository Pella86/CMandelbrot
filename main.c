#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "complex_matrix.h"
#include "int_matrix.h"
#include "double_matrix.h"

/*
    MANDELBROT
*/

t_complex calc_mandelbrot(t_complex c){
    int max_iter = 25;
    t_complex z = {0, 0};

    int iter = 0;
    while(iter < max_iter){
        z = add(mul(z, z), c);

        if(squared_module(z) > 4){
            return z;
        }
        iter++;
    }
    return z;
}


void print_mandelbrot(t_cMatrix m){
    int i, j;

    for(j = 0; j < m.height; j++){
        for(i = 0; i < m.width; i++){
            t_complex r;
            r = c_get(m, i, j);

            if(squared_module(r) > 4.0){
                printf("*");
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void print_mandelbrot_double(t_doubleMatrix m){
    int i, j;

    for(j = 0; j < m.height; j++){
        for(i = 0; i < m.width; i++){
            double r = d_get(m, i, j);

            if( r == 0.0){
                printf("*");
            }
            else{
                printf("-");
            }
        }
        printf("\n");
    }
}

void print_mandelbrot_int(t_intMatrix m){
    int i, j;

    for(j = 0; j < m.height; j++){
        for(i = 0; i < m.width; i++){
            int r = i_get(m, i, j);

            if( r == 0){
                printf("*");
            }
            else{
                printf(".");
            }
        }
        printf("\n");
    }
}

/*
    LINSPACE
*/

typedef struct Linspace{
    double minv;
    double maxv;
    int steps;
} t_linspace;

void lin_init(t_linspace* ls, int minv, int maxv, int steps){
    ls->minv = minv;
    ls->maxv = maxv;
    ls->steps = steps;
}

double lin_get(t_linspace ls, int n){
    return (ls.maxv - ls.minv) / ls.steps * n + ls.minv;
}

/*******************************************************************************
* INPUT MATRIX
*******************************************************************************/


void create_input_matrix(t_cMatrix* imat, t_linspace lsx, t_linspace lsy){
    int i, j;
    for(i = 0; i < imat->width; i++){
        for(j = 0; j < imat->height; j++){
            t_complex c;
            c.real = lin_get(lsx, i);
            c.imag = lin_get(lsy, j);
            c_set(imat, i, j, c);
        }
    }
}

t_cMatrix generate_input_matrix(int width, int height, double xmin, double xmax, double ymin, double ymax){
    // generate the matrix
    t_cMatrix cmat;
    t_cMatrix* ref_cmat = &cmat;
    c_init_matrix(ref_cmat, width, height);


    t_linspace lsx;
    t_linspace* ref_lsx = &lsx;
    lin_init(ref_lsx, xmin, xmax, width);

    t_linspace lsy;
    t_linspace* ref_lsy = & lsy;
    lin_init(ref_lsy, ymin, ymax, height);

    create_input_matrix(ref_cmat, lsx, lsy);
    return cmat;
}





// normalizzare matrice

// salvare sia matrice che matrice colore?
// matrice colore, matrice composta da int -> rappresentano colore
// lunghezza = colormap depth
// lut, prenderla da python, array di int
// double index trassformato in colore


/*******************************************************************************
* Colormap
*******************************************************************************/


int* load_colormap(char* filename, int color_depth){
    FILE* f = fopen(filename, "rb");

    int* rcontent = malloc(color_depth * sizeof(int));
    fread(rcontent, sizeof(int), color_depth, f);

    return rcontent;
}

/*******************************************************************************
* Generate mandelbrot
*******************************************************************************/

void populate_mandel_solution(t_cMatrix* mandel_sol, t_cMatrix imat){
    int i, j;
    for(i = 0; i < imat.width; i++){
        for(j = 0; j < imat.height; j++){
            t_complex z = calc_mandelbrot(c_get(imat, i, j));
            c_set(mandel_sol, i, j, z);
        }
    }
}


// converts the mandelbrot in a real number
t_doubleMatrix evaluate_mandelbrot(t_cMatrix mandel_sol){
    int i, j;

    t_doubleMatrix real_sol;
    t_doubleMatrix* ptr_real_sol = & real_sol;
    d_init_matrix(ptr_real_sol, mandel_sol.width, mandel_sol.height);

    for(i = 0; i < mandel_sol.width; i++){
        for(j = 0; j < mandel_sol.height; j++){
            t_complex z = c_get(mandel_sol, i, j);
            // insert here the different possible calculations
            double sqm = squared_module(z);
            double item;

            // inside mandelbrot
            if(sqm <= 4){
                item = 0;
            }
            else{
                item = sqrt(sqm);
            }
            d_set(ptr_real_sol, i, j, item);
        }
    }

    // normalize the matrix!!!
    d_normalize(ptr_real_sol);

    return real_sol;
}

t_intMatrix convert_to_colormap(t_doubleMatrix real_sol, char* cmp_filename, int color_depth){
    int i, j;
    t_intMatrix color_mat;
    t_intMatrix* ptr_color_mat = &color_mat;
    i_init_matrix(ptr_color_mat, real_sol.width, real_sol.height);

    int* colormap = load_colormap(cmp_filename, color_depth);

    for(i = 0; i < real_sol.width; i++){
        for(j = 0; j < real_sol.height; j++){
            // take the value
            double v = d_get(real_sol, i, j);
            // check if is 0 to 1
            assert(v >= 0 && v <= 1);

            int color;
            // get the number from the colormap and assing it to the matrix
            if(v == 0){
                color = 0;
            }
            else{
                double dcolor_index = v * (color_depth - 1);
                int color_index = (int) dcolor_index;
                color = colormap[color_index];
            }

            i_set(ptr_color_mat, i, j, color);
        }
    }
    return color_mat;

}

void create_mandel_file(char* path, t_intMatrix color_mat){
    FILE* f = fopen(path, "wb");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    printf("writing to file...\n");
    fwrite(color_mat.arr, color_mat.height * color_mat.width, sizeof(int), f);
}

void generate_mandelbrot(int width, int height,
                         double xmin, double xmax,
                         double ymin, double ymax,
                         int color_depth, char* colormap_filename){

    printf("calculating...\n");
    // generate the input matrix

    t_cMatrix imat = generate_input_matrix(width, height, xmin, xmax, ymin, ymax);

    // create the solution
    t_cMatrix mand_sol;
    t_cMatrix* mandel_sol = &mand_sol;
    c_init_matrix(mandel_sol, width, height);

    populate_mandel_solution(mandel_sol, imat);

    //print_mandelbrot(mand_sol);

    // construct a double matrix
    t_doubleMatrix real_sol = evaluate_mandelbrot(mand_sol);

    //print_mandelbrot_double(real_sol);

    // contruct a int matrix based on the value
    t_intMatrix color_mat = convert_to_colormap(real_sol, colormap_filename, color_depth);

    //print_mandelbrot_int(color_mat);

    //write to file
    create_mandel_file("test_file.mlo", color_mat);

    // free stuff
    c_free_matrix(&imat);
    c_free_matrix(mandel_sol);
    d_free_matrix(&real_sol);
    i_free_matrix(&color_mat);

}

// write the different modes
// iteration, phase, module, real, imag

// write the inputs from command line int* argc, char** argv


int main()
{
    int width = 640;
    int height = 480;

    int color_depth = 128;
    char* cmap_filename = "hot_lut.cmp";

    double xmin = -2., xmax = 1.5, ymin = -1.25, ymax = 1.25;
    generate_mandelbrot(width, height, xmin, xmax, ymin, ymax, color_depth, cmap_filename);

    char* vb_str = "vb e' frocio";
    printf("%d, %c, %x\n", vb_str[0], vb_str[0], vb_str[0]);
    int a, b, c;
    a = build_int(vb_str);
    b = build_int(vb_str + 4);
    c = build_int(vb_str + 8);
    printf("%d\n", c);



    return 0;
}
