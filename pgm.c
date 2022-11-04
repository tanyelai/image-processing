#include "pgm.h"

/**
 * @brief Check if pgm format is P2 or P5 and return the reading mode
 * 
 * @param filename 
 * @return char* 
 */
char *check_pgm_type(char *filename){
    char type[3];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    fscanf(fp, "%2s", type);
    fclose(fp);
    if (strcmp(type, "P2") == 0)
        return "r\0";
    
    else if (strcmp(type, "P5") == 0)
        return "rb";
    
    return NULL;
}

/**
 * @brief Create a new pgm struct to hold the image data and return pointer to it
 *        Struct is allocated on the heap
 *        Calloc is used to allocate memory because Calloc initializes the memory to 0 for padding if needed
 * 
 * @param width 
 * @param height 
 * @param max_val 
 * @param type 
 * @return PGM* 
 */
PGM *pgm_create(int width, int height, int max_val, char *type){
    PGM *pgm = (PGM *)malloc(sizeof(PGM));
    pgm->width = width;
    pgm->height = height;
    pgm->max_val = max_val;
    strcpy(pgm->type, type);
    pgm->matrix = (unsigned char **)calloc(height, sizeof(unsigned char *));

    if (pgm->matrix == NULL){
        free(pgm);
        fprintf(stderr, "Error: pgm_create() failed to allocate memory for pgm->data\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < height; i++)
        pgm->matrix[i] = (unsigned char *)calloc(width, sizeof(unsigned char));
    
    fprintf(stdout, "pgm_create() created a PGM image with width %d, height %d, max_val %d, type %s\n", width, height, max_val, type);
    return pgm;
}

/**
 * @brief Read a pgm image from a file and return a pointer to the image struct
 *        Check for PGM type is P2, reading mode is r and if PGM type is P5, reading mode is rb
 *        if PGM type is P2, data orientation is like this: data|whitespace|data|whitespace
 *          so fscanf reads data|whitespace and then data to fill the image struct
 *        if PGM type is P5, we can read the data with fread line by line 
 *        Single pixel data type is unsigned char (max value is 255)
 * 
 * @param filename 
 * @return PGM* 
 */
PGM *pgm_read(char *filename){
    char type[3];
    int max_val;
    int width, height, i, j;
    char reading_format[3];

    memcpy(reading_format, check_pgm_type(filename), 2);

    FILE *ptr = fopen(filename, reading_format);
    if (ptr == NULL){
        fprintf(stderr, "ERROR: image file cannot be read by pgm_read() %s\n", filename);
        exit(EXIT_FAILURE);
    }

    skip_comments(ptr);
    fscanf(ptr, "%s\n", type);
    skip_comments(ptr);
    fscanf(ptr, "%d %d\n", &width, &height);
    skip_comments(ptr);
    fscanf(ptr, "%d\n", &max_val);

    PGM *pgm = pgm_create(width, height, max_val, type);

    if (pgm->type[1] == '2'){
       for (i = 0; i < pgm->height; i++){
            for (j = 0; j < pgm->width; j++)
                fscanf(ptr, "%hhu ", &pgm->matrix[i][j]);
        }
    }
    else if (pgm->type[1] == '5'){
        for (i = 0; i < pgm->height; i++){
            fread(pgm->matrix[i], sizeof(unsigned char), pgm->width, ptr);
            if (ferror(ptr)){
                fprintf(stderr, "ERROR: pgm_read() failed to read from file %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }
    }
    else
        fprintf(stderr, "ERROR: Provided format is not readable\n");

    fclose(ptr);
    return pgm;
}
/**
 * @brief There can be comments in the file, so we need to skip them
 *        Comments are lines starting with #
 *        Read char and check if it is #
 *        If it is #, skip the line
 *        If it is not #, seek back to the beginning of the line 
 * 
 * @param ptr 
 */
void skip_comments(FILE *ptr){
    int ch;
    char line[100];
    while ((ch = fgetc(ptr)) != EOF && isspace(ch)){};
    if (ch == '#'){
        fgets(line, sizeof(line), ptr);
        skip_comments(ptr);
    }
    else fseek(ptr, -1, SEEK_CUR);
}

/**
 * @brief The function is useful for understanding if matrix is read properly
 *        
 * 
 * @param pgm 
 * @param width 
 * @param heigth 
 */
void pgm_show_matrix(PGM *pgm, int width, int height){
	int i, j;
	for(i=0; i<pgm->height; i++){
		for(j=0; j<pgm->width; j++)
			printf("%d ", pgm->matrix[i][j]);
		printf("\n");
	}
}



/**
 * @brief Write a pgm image to a file
 *        Check for PGM type is P2, writing mode is w and if PGM type is P5, writing mode is wb
 *        if PGM type is P2, data orientation is like this: data|whitespace|data|whitespace
 *        so fprintf writes data|whitespace and then data to the file
 *        if PGM type is P5, we can write the data with fwrite line by line
 *        Single pixel data type is unsigned char (max value is 255)
 * 
 * @param pgm 
 * @param filename 
 */
void pgm_write(PGM *pgm, char *filename){
    char reading_format[3];
    int i, j;
    if (strcmp(pgm->type, "P2") == 0)
        strcpy(reading_format, "w\0");
    else if (strcmp(pgm->type, "P5") == 0)
        strcpy(reading_format, "wb");
    else{
        fprintf(stderr, "Error: Unknown format\n");
        exit(EXIT_FAILURE);
    }

    FILE *ptr = fopen(filename, reading_format);
    if (ptr == NULL){
        fprintf(stderr, "Error: pgm_write() failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(ptr, "%s\n", pgm->type);
    fprintf(ptr, "%d %d\n", pgm->width, pgm->height);
    fprintf(ptr, "%d\n", pgm->max_val);

    
    if(pgm->type[1] == '2'){
        for (i = 0; i < pgm->height; i++){
            for (j = 0; j < pgm->width; j++)
                fprintf(ptr, "%hhu ", pgm->matrix[i][j]);
            fprintf(ptr, "\n");
        }
    }
    else if(pgm->type[1] == '5'){
        for (i = 0; i < pgm->height; i++){
            fwrite(pgm->matrix[i], sizeof(unsigned char), pgm->width, ptr);
            if (ferror(ptr)){
                fprintf(stderr, "Error: pgm_write() failed to write to file %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(ptr);
}


/**
 * @brief Free the memory allocated for the image struct
 * 
 * @param pgm 
 */
void pgm_free(PGM *pgm){
    for (int i = 0; i < pgm->height; i++)
        free(pgm->matrix[i]);
    
    free(pgm->matrix);
    free(pgm);
    fprintf(stdout, "pgm_free() freed the PGM image\n");
}

/**
 * @brief Apply method of sobel filter function that provides horizontal and vertical
 *        filtering in the desired direction.
 *        main.c gets input param "choice" to determine which filter will be applied.
 *        Available choices "SOBEL_X": horizontal filter
 *                          "SOBEL_Y": vertical filter
 *                          "SOBEL_XY": both horizontal and vertical filter
 * 
 * @param img 
 * @param padding 
 * @param choice 
 * @return PGM* 
 */

PGM *sobel_filter(PGM *img, char *padding, char *choice){
    PGM *filtered, *sobel_x, *sobel_y;
    int k, i, j, m, n;

    double x_max = DBL_MIN;
    double x_min = DBL_MAX;
    double y_max = DBL_MIN;
    double y_min = DBL_MAX;
    double x_sum, y_sum;

    short x_sobel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    short y_sobel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    double **temp_x = (double **)malloc(sizeof(double *) * img->height - 2);
    double **temp_y = (double **)malloc(sizeof(double *) * img->height - 2);

    for (i = 0; i < img->height - 1; i++){
        temp_x[i] = (double *)malloc(sizeof(double) * img->width - 2);
        temp_y[i] = (double *)malloc(sizeof(double) * img->width - 2);
    }

    if (strcmp(padding, "True") == 0){
        filtered = pgm_create(img->width, img->height, img->max_val, img->type);
        sobel_x = pgm_create(img->width, img->height, img->max_val, img->type);
        sobel_y = pgm_create(img->width, img->height, img->max_val, img->type);
        k = 1;
    }
    else{
        filtered = pgm_create(img->width - 2, img->height - 2, img->max_val, img->type);
        sobel_x = pgm_create(img->width - 2, img->height - 2, img->max_val, img->type);
        sobel_y = pgm_create(img->width - 2, img->height - 2, img->max_val, img->type);
        k = 0;
    }

    for (i = 0; i < img->height - 2; i++){
        for (j = 0; j < img->width - 2; j++){
            x_sum = 0;
            y_sum = 0;
            for (m = 0; m < 3; m++){
                for (n = 0; n < 3; n++){
                    x_sum += (double)img->matrix[i + m][j + n] * x_sobel[m][n];
                    y_sum += (double)img->matrix[i + m][j + n] * y_sobel[m][n];
                }
            }
            temp_x[i][j] = x_sum;
            temp_y[i][j] = y_sum;

            if (x_sum > x_max)
                x_max = x_sum;
            else if (x_sum < x_min)
                x_min = x_sum;
            
            if (y_sum > y_max)
                y_max = y_sum;
            else if (y_sum < y_min)
                y_min = y_sum;   
        }
    }

    /* Sobel x and y directions are calculated in the same loop */
    for (i = 0; i < img->width - 2; i++){
        for (j = 0; j < img->height - 2; j++){
            sobel_x->matrix[i + k][j + k] = (unsigned char)(temp_x[i][j] - x_min) * 255 / (x_max - x_min);
            sobel_y->matrix[i + k][j + k] = (unsigned char)(temp_y[i][j] - y_min) * 255 / (y_max - y_min);
        }
    }

    /* Sobel XY */
    for (i = 0; i < img->width - 2; i++){
        for (j = 0; j < img->height - 2; j++)
            filtered->matrix[i + k][j + k] = (unsigned char)(sqrt(pow(sobel_x->matrix[i + k][j + k], 2) + pow(sobel_y->matrix[i + k][j + k], 2)));
    }

    for (i = 0; i < img->height - 1; i++){
        free(temp_x[i]);
        free(temp_y[i]);
    }

    free(temp_x);
    free(temp_y);

    /*return filtered image, determined by input at main function*/
    if (strcmp(choice, "SOBEL_X") == 0){
        pgm_free(sobel_y);
        pgm_free(filtered);
        return sobel_x;
    }
    else if(strcmp(choice, "SOBEL_Y") == 0){
        pgm_free(sobel_x);
        pgm_free(filtered);
        return sobel_y;
    }
    else{
        pgm_free(sobel_x);
        pgm_free(sobel_y);
        return filtered;
    }
}



/**
 * @brief Create a gaussian filter object
 * 
 * @param kernel_size 
 * @param kernel 
 * @param sigma 
 */
void create_gaussian_filter(int kernel_size, float kernel[kernel_size][kernel_size], float sigma){
	int i, j, X[kernel_size][kernel_size], Y[kernel_size][kernel_size], dif = (kernel_size-1)/2;

	// INITIALIZATION OF X-Y
	for(i=0; i<kernel_size; i++){		
		for(j=0; j<kernel_size; j++){
			X[i][j] = j-dif;
			Y[i][j] = i-dif;
		}
	}

	// CREATE GAUSSIAN KERNEL
	for(i=0; i<kernel_size; i++){
		for(j=0; j<kernel_size; j++){
			kernel[i][j] = exp(-(X[i][j] * X[i][j] + Y[i][j] * Y[i][j]) / (2 * sigma * sigma));
		}
	}

float normalization_factor = kernel[0][0];

	for(i=0; i<kernel_size; i++){
		for(j=0; j<kernel_size; j++)
			kernel[i][j] = round(kernel[i][j] / (normalization_factor));
	}

}


/**
 * @brief convolution algorithm to apply different filters dynamically
 * 
 * @param img 
 * @param padding 
 * @param kernel_size 
 * @param kernel 
 * @return PGM* 
 */
PGM *convolution(PGM *img, char *padding, int kernel_size, float kernel[][kernel_size]){

    int k, i, j, ki, kj;
    int sum;
    int size = kernel_size - 1;
    PGM *scaled_img;

    if (strcmp(padding, "True") == 0){
        scaled_img = pgm_create(img->width, img->height, img->max_val, img->type);
        k = size / 2;
    }
    else{
        scaled_img = pgm_create(img->width - 2, img->height - 2, img->max_val, img->type);
        k = 0;
    }

    int **filtered = (int **)malloc(img->width * sizeof(int *));
    for (i = 0; i < img->width; i++)
        filtered[i] = (int *)malloc(img->height * sizeof(int));
    


    // init filtered matrix
    for (i = 0; i < img->width - kernel_size + 1; i++){
        for (j = 0; j < img->height - kernel_size + 1; j++){
            sum = 0;
            for (ki = 0; ki < kernel_size; ki++){
                for (kj = 0; kj < kernel_size; kj++){
                    sum += img->matrix[i+ki][j+kj] * kernel[ki][kj];
                }
            }
            filtered[i + k][j + k] = round((float)sum / (kernel_size * kernel_size));
        }
    }


    // int array is used to be able to hold 255+ values which are created by filter
    // then we scale the 255+ values to 0-255 and scaled_img will be holding new
    // scaled image.
    int min = filtered[0][0], max = filtered[0][0];
    
    for(i=0; i<img->width - kernel_size + 1; i++){
		for(j=0; j<img->height - kernel_size + 1; j++){
			if(filtered[i][j] < min)
				min = filtered[i][j];
			else if(filtered[i][j] > max)
				max = filtered[i][j];
		}
	}
	printf("\nMIN: %d\nMAX: %d\n", min, max);
	
    int dif = max-min;
	for(i=0; i < img->width; i++)
		for(j=0; j < img->height; j++)
			filtered[i][j] =  (int) (((float) (filtered[i][j] - min) / dif) * (255-min)) + min;


    for(i = 0; i < img->width - kernel_size + 1; i++)
		for(j = 0; j < img->height - kernel_size + 1; j++)
			scaled_img->matrix[i][j] = (unsigned char) filtered[i][j];


    return scaled_img;
}

