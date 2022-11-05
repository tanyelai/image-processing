#ifndef PGM_H
#define PGM_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

// PGM data structure
typedef struct{
    char type[3]; // When type[2] is selected, the error "trace trap" occures.
    int width;
    int height;
    int max_val;
    unsigned char **matrix;
} PGM;

// PGM file format read and write
char *check_pgm_type(char *filename);
PGM *pgm_read(char *filename);
void skip_comments(FILE *fp);
void pgm_show_matrix(PGM *pgm, int width, int height);
PGM *pgm_create(int width, int height, int max_val, char *type);
void pgm_write(PGM *pgm, char *filename);
void pgm_free(PGM *pgm);

// external functions
PGM *sobel_filter(PGM *img, char *padding, char *choice);
void create_gaussian_filter(int kernel_size, float kernel[kernel_size][kernel_size], float sigma);
PGM *convolution(PGM *img, char *padding, int kernel_size, float kernel[][kernel_size]);


#endif //PGM_H