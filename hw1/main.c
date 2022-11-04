#include "pgm.h"

int main(int argc, char *argv[]){

    char filename[30];
    //char kernel_type[16];
    int kernel_size = 0;
    float sigma = 0.0;
    int i,j;


    printf("Please, enter the filename: ");
    scanf("%s", filename);

    if (argc > 1){
        strcpy(filename, argv[1]);
    }

    PGM *pgm = pgm_read(filename);

    //printf("\nPlease, enter the kernel type: ");
    //scanf("%s", kernel_type);
    printf("\nPlease, enter the kernel size and sigma value: ");
    scanf("%d %f", &kernel_size, &sigma);

    float kernel[kernel_size][kernel_size];

    create_gaussian_filter(kernel_size, kernel, sigma);
    
    //pgm_show_matrix(pgm, pgm->width, pgm->height);

    //PGM *sobelX = sobel_filter(pgm, "True", "SOBEL_X");
    //PGM *sobelY = sobel_filter(pgm, "True", "SOBEL_Y");
   // PGM *sobelXY = sobel_filter(pgm, "True", "SOBEL_XY");

    PGM *gaussed = convolution(pgm, "Yes", kernel_size, kernel);
    pgm_write(gaussed, "test_gaussed_v2.pgm");
    // pgm_write(sobelY, "test_sobel_y.pgm");
    // pgm_write(sobelXY, "test_sobel_xy.pgm");


    printf("\n\n-------------------\nGaussian Kernel:\n\n");
	for(i=0; i<kernel_size; i++){
		for(j=0; j<kernel_size; j++){ 
			printf("%.3f ", kernel[i][j]);
		}
		printf("\n");
	}	


    // pgm_free(sobelX);
    // pgm_free(sobelY);
    // pgm_free(sobelXY);
    pgm_free(gaussed);
    pgm_free(pgm);


    return 0;
}