#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "complex_matrix.h"
#include "int_matrix.h"
#include "double_matrix.h"
#include "mandel_matrix.h"
#include "custom_complex.h"

/*******************************************************************************
* MANDELBROT
*******************************************************************************/

t_MandelResult calc_mandelbrot(t_complex c, int max_iter){
    t_MandelResult result;
    result.in_set = true;

    t_complex z = {0, 0};
    int iter = 0;
    while(iter < max_iter){
        if(squared_module(z) > 4){
            result.in_set = false;
            break;
        }

        z = add(mul(z, z), c);
        iter++;
    }

    result.iterations = iter;
    result.value = z;
    return result;
}

/*******************************************************************************
* LINSPACE
*******************************************************************************/

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

/*******************************************************************************
* Colormap
*******************************************************************************/


int* load_colormap(char* filename, int color_depth){
    FILE* f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Error opening colormap file!\n");
        exit(1);
    }
    int* rcontent = malloc(color_depth * sizeof(int));
    fread(rcontent, sizeof(int), color_depth, f);

    return rcontent;
}

/*******************************************************************************
* Generate mandelbrot
*******************************************************************************/

void populate_mandel_solution(t_mandelMatrix* mandel_sol, t_cMatrix imat, int max_iter){
    int i, j;
    for(i = 0; i < imat.width; i++){
        for(j = 0; j < imat.height; j++){
            t_MandelResult m = calc_mandelbrot(c_get(imat, i, j), max_iter);
            m_set(mandel_sol, i, j, m);
        }
    }
}

typedef enum Modes{
    m_iteration = 0,
    m_module,
    m_real,
    m_imag,
    m_phase,
}t_modes;

// converts the mandelbrot in a real number
t_doubleMatrix evaluate_mandelbrot(t_mandelMatrix mandel_sol, t_modes mode){
    int i, j;

    t_doubleMatrix real_sol;
    t_doubleMatrix* ptr_real_sol = & real_sol;
    d_init_matrix(ptr_real_sol, mandel_sol.width, mandel_sol.height);

    for(i = 0; i < mandel_sol.width; i++){
        for(j = 0; j < mandel_sol.height; j++){

            t_MandelResult result = m_get(mandel_sol, i, j);
            double ev_res;
            switch(mode){
                case m_iteration:
                    ev_res = (double) result.iterations;
                    break;
                case m_module:
                    ev_res = module(result.value);
                    break;
                case m_real:
                    ev_res = result.value.real;
                    break;
                case m_imag:
                    ev_res = result.value.imag;
                    break;
                case m_phase:
                    ev_res = phase(result.value);
                    break;
                default:
                    ev_res = 0;
            }

            d_set(ptr_real_sol, i, j, ev_res);
        }
    }

    // normalize the matrix!!!
    d_normalize(ptr_real_sol);

    return real_sol;
}

t_intMatrix convert_to_colormap(t_doubleMatrix real_sol, t_mandelMatrix mandel, char* cmp_filename, int color_depth){
    int i, j;
    t_intMatrix color_mat;
    t_intMatrix* ptr_color_mat = &color_mat;
    i_init_matrix(ptr_color_mat, real_sol.width, real_sol.height);

    int* colormap = load_colormap(cmp_filename, color_depth);

    for(i = 0; i < real_sol.width; i++){
        for(j = 0; j < real_sol.height; j++){
            t_MandelResult m = m_get(mandel, i, j);
            if(m.in_set){
                i_set(ptr_color_mat, i, j, 0);
            }
            else{
                int color;
                double v = d_get(real_sol, i, j);
                double dcolor_index = v * (color_depth - 1);
                int color_index = (int) dcolor_index;
                color = colormap[color_index];
                i_set(ptr_color_mat, i, j, color);
            }
        }
    }
    return color_mat;

}

void create_mandel_file(char* path, t_intMatrix color_mat){
    FILE* f = fopen(path, "wb");
    if (f == NULL) {
        printf("Error opening file to be saved!\n");
        exit(1);
    }
    printf("writing to file...\n");
    fwrite(color_mat.arr, color_mat.height * color_mat.width, sizeof(int), f);
}

void generate_mandelbrot(int width, int height,
                         double xmin, double xmax,
                         double ymin, double ymax,
                         int max_iter, t_modes mode,
                         int color_depth, char* colormap_filename,
                         char* destination){

    printf("calculating...\n");
    // generate the input matrix

    t_cMatrix imat = generate_input_matrix(width, height, xmin, xmax, ymin, ymax);

    // create the solution
    t_mandelMatrix mand_sol;
    t_mandelMatrix* mandel_sol = &mand_sol;
    m_init_matrix(mandel_sol, width, height);

    populate_mandel_solution(mandel_sol, imat, max_iter);

    //print_mandelbrot(mand_sol);

    // construct a double matrix
    t_doubleMatrix real_sol = evaluate_mandelbrot(mand_sol, mode);

    //print_mandelbrot_double(real_sol);

    // contruct a int matrix based on the value
    t_intMatrix color_mat = convert_to_colormap(real_sol, mand_sol, colormap_filename, color_depth);

    //print_mandelbrot_int(color_mat);

    //write to file
    create_mandel_file(destination, color_mat);

    // free stuff
    c_free_matrix(&imat);
    m_free_matrix(&mand_sol);
    d_free_matrix(&real_sol);
    i_free_matrix(&color_mat);

}

// write the inputs from command line int* argc, char** argv




int main(int argc, char* argv[])
{
    assert(argc == 12);

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    double xmin = atof(argv[3]);
    double xmax = atof(argv[4]);
    double ymin = atof(argv[5]);
    double ymax = atof(argv[6]);

    int max_iter = atoi(argv[7]);

    char* arg_mode = argv[8];
    char* modes_str[] = {"iteration", "module", "real", "imag", "phase"};
    size_t modes_size = 5;
    t_modes mode;

    int i;
    for(i = 0; i < modes_size; i++){
        if(strcmp(arg_mode, modes_str[i]) == 0){
            mode = (t_modes)i;
            break;
        }
    }

    int color_depth = atoi(argv[9]);
    char* cmap_filename = argv[10];
    char* destination = argv[11];

    generate_mandelbrot(width, height, xmin, xmax, ymin, ymax, max_iter, mode, color_depth, cmap_filename, destination);



    return 0;
}


