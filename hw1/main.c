#include "pgm.h"

int main(int argc, char *argv[]){

    char filename[24], kernel_type[12];
    int kernel_size = 0;
    float sigma = 0.0;
    int choice = 0;

    printf("Please, enter the filename: ");
    scanf("%s", filename);

    if (argc > 1){
        strcpy(filename, argv[1]);
    }

    PGM *pgm = pgm_read(filename);
    //pgm_show_matrix(pgm, pgm->width, pgm->height);

    printf("\nPlease, enter the kernel type\n (gauss, laplacian, sobelx, sobely, sobelxy, sobel): ");
    scanf("%s", kernel_type);


    if (strcmp(kernel_type, "gauss") == 0){
        printf("\nPlease, enter the kernel size and sigma value: ");
        scanf("%d %f", &kernel_size, &sigma);
        float kernel[kernel_size][kernel_size];
        create_gaussian_filter(kernel_size, kernel, sigma);
        PGM *gaussed = convolution(pgm, "False", kernel_size, kernel);
        pgm_write(gaussed, "test_gaussed.pgm");
        pgm_free(gaussed);
    }
    else if(strcmp(kernel_type, "laplacian") == 0){
        printf("\nPlease, enter the kernel choice: \n1 = {0, -1, 0}, {-1, 4, -1}, {0, -1, 0}\n2 = {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}\n\nChoice: ");
        scanf("%d", &choice);

        float kernel_1[3][3] = {{0, -1, 0}, 
                               {-1, 4, -1}, 
                               {0, -1, 0}};

        float kernel_2[3][3] = {{-1, -1, -1}, 
                               {-1, 8, -1}, 
                               {-1, -1, -1}};

        if (choice == 1){
            PGM *laplacianed = convolution(pgm, "False", 3, kernel_1);
            pgm_write(laplacianed, "test_laplacianed_by_k1.pgm");
        }    
        else if (choice == 2){
            PGM *laplacianed = convolution(pgm, "False", 3, kernel_2);
            pgm_write(laplacianed, "test_laplacianed_by_k2.pgm");
        }
        else{
            printf("ERROR: Provided choice is not exist");
            exit(EXIT_FAILURE);
        }
    }

    else if(strcmp(kernel_type, "sobelx") == 0){
        PGM *sobelx = sobel_filter(pgm, "False", "sobelx");
        pgm_write(sobelx, "test_sobel_x.pgm");
    }
    else if(strcmp(kernel_type, "sobely") == 0){
        PGM *sobely = sobel_filter(pgm, "False", "sobely");
        pgm_write(sobely, "test_sobel_y.pgm");
    }
    else if(strcmp(kernel_type, "sobelxy") == 0){
        
    }
    else if(strcmp(kernel_type, "sobel") == 0){
        PGM *sobelx = sobel_filter(pgm, "False", "sobelx");
        PGM *sobely = sobel_filter(pgm, "False", "sobely");
        PGM *sobelxy = sobel_filter(pgm, "False", "sobelxy");
        pgm_write(sobelx, "test_sobel_x.pgm");
        pgm_write(sobely, "test_sobel_y.pgm");
        pgm_write(sobelxy, "test_sobel_xy.pgm");
        pgm_free(sobelx);
        pgm_free(sobely);
        pgm_free(sobelxy);
    }
    else{
            printf("ERROR: Provided choice is not exist");
            exit(EXIT_FAILURE);
    }
    
    

    pgm_free(pgm);

    return 0;
}