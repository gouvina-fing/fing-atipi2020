// Dependencies
#include "denoiser.h"
#include <math.h>
#include <cstdio>

// Methods
void allocate_memory(float ***A, char ****histograms, short height, short width, short k) {
    // Auxiliary context table
    *A = new float*[height];
    for (short i = 0; i < height; ++i) (*A)[i] = new float[width];

    // Context histogram table
    *histograms = new char**[height];
    for (short i = 0; i < height; ++i) {
        (*histograms)[i] = new char*[width];
        for (short j = 0; j < width; ++j) (*histograms)[i][j] = new char[4+k];
    }
}

void initialize_data(float **A, char ***histograms, short height, short width, short k) {
    for (short i = 0; i < height; ++i) {
        for (short j = 0; j < width; ++j) {

            // Base value
            A[i][j] = 0;

            // Pre-Fill values for borders outside of the images

            // If it's in the north or south border it collides with 3 
            if ((i == 0) || (i == height -1)) {
                A[i][j] += 384;
            }

            // If it's in the east or west border it collides with 3 
            if((j == 0) || (j == width -1)) {
                A[i][j] += 384;
            }

            // If it's in a corner it collides with 5 (and above we added an extra collision)
            if (((i == 0) && (j == 0)) || ((i == 0) && (j == width -1)) || ((i == height -1) && (j == 0)) || ((i == height -1) && (j == width -1))) {
                A[i][j] -= 128;
            }

            // Initialize context histogram table
            for (short histogram = 0; histogram < 4 + k; ++histogram) {
                histograms[i][j][histogram] = 0;
            }
        }
    }
}

void free_memory(float ***A, char ****histograms, short height, short width) {
    // Auxiliary image table
    for (int i = 0; i < height; ++i) free((*A)[i]);
    free(*A);

    // Histogram table
    for (short i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) free((*histograms)[i][j]);
        free((*histograms)[i]);
    }
    free(*A);
}

/*
 |  Processses a grayscale image applying the DUDE algorithm, storing the result in img_out
 |
 |  Preconditions: This implementation of DUDE assumes that:
 |    - Images are grayscale 8bpp, taking values from [0,255]
 |    - The noise that affects the images comes from a Salt & Pepper channel (with parameter delta)
 |    
 |  The denoiser has the following peculiarities:
 |    - Every pixel outside the border of an image has 128 as its value.
 |    - It uses a 3x3 context (the 8 pixels that are immediate neighboors to the one in the center)
 |          (N,E,S,W,NW,NE,SW,SE) following the cardinal directions
 |    - A context C is quantized by extracting f(C), a binary vector of length 4+k bits (where k is a parameter in [0,8]).
 |    - f(C) is used as index in the conditional histogram's table (2^(4+k) histograms)
 |
 |  Args:
 |    - delta (float): Probability that pixel sample goes to 0 or 255 (uniformly)
 |    - k (short): Parameter for context quantization, taking values from [0,8] 
 |    - img_in (ImageModel): Model containing a 2D matrix storing a grayscale image to be denoised.
 |    - img_prefiltered (ImageModel):
 |          - NULL if prefilter is not desired.
 |          - Model containing a 2D matrix storing a grayscale image if prefiltering and iteration are taken into account.
 |    - img_out (ImageModel): Model containing a 2D matrix that will store the result of DUDE
 |  
 |  Returns (int):
 |    - 0 if successful, error code otherwise
 |
 |  Raises:
 |    - 
*/
void dude(float delta, short k, ImageModel img_in, ImageModel img_prefiltered, ImageModel img_out) {
    img_out = img_in;

    // Preprocessing
    float **A;
    char ***histograms;
    short divisor_aux = 256; // 2^8
    float current_element;

    float **matrix_in = img_in.getMatrix();
    allocate_memory(&A, &histograms, img_in.getHeight(), img_in.getWidth(), k);
    initialize_data(A, histograms, img_in.getHeight(), img_in.getWidth(), k);

    // Step 1: Calculate empirical distribution for each context

    // Get aux data from the context
    for (short i = 0; i < img_in.getHeight(); ++i) {
        for (short j = 0; j < img_in.getWidth(); ++j) {
            
            current_element = matrix_in[i][j];
            for(short mask_i = i - 1; (mask_i < i + 2) && (mask_i >= 0) && (mask_i < img_in.getHeight()); ++mask_i) {
                for(short mask_j = j - 1; (mask_j < j + 2) && (mask_j >= 0) && (mask_j < img_in.getWidth()); ++mask_j) {
                    A[mask_i][mask_j] += current_element;
                }
            }
        }
    }
    
    for (short i = 0; i < img_in.getHeight(); ++i) {
        for (short j = 0; j < img_in.getWidth(); ++j) {
            A[i][j] = floorf((A[i][j]/8) + 0.5);

            // Compute f(C) (Histogram Table)
            current_element = A[i][j];

            // F1: North
            if (i == 0)
                histograms[i][j][0] = (128 > current_element);
            else
                histograms[i][j][0] = (matrix_in[i-1][j] > current_element);

            // F2: East
            if (j == img_in.getWidth() - 1)
                histograms[i][j][1] = (128 > current_element);
            else
                histograms[i][j][1] = (matrix_in[i][j+1] > current_element);

            // F3: South
            if (i == img_in.getHeight() - 1)
                histograms[i][j][2] = (128 > current_element);
            else
                histograms[i][j][2] = (matrix_in[i+1][j] > current_element);

            // F4: West
            if (j == 0)
                histograms[i][j][3] = (128 > current_element);
            else
                histograms[i][j][3] = (matrix_in[i][j-1] > current_element);

            // F5 .. F4+k
            for (short histogram = 4; histogram < 4+k; ++histogram) {
                divisor_aux /= 2; // 2^(8-k)
                histograms[i][j][histogram] = floorf(current_element/divisor_aux);
            }

            for (short test = 0; test < 4+k; ++test) {
                printf("%i", histograms[i][j][test]);
            }
            printf("\n");

        }
    }

    // Step 2: Calculate the MAP response for each context


    // Step 3: Estimate each symbol by the MAP response to its context

    for (short i = 0; i < img_in.getHeight(); ++i) {
        for (short j = 0; j < img_in.getWidth(); ++j) {
            current_element = matrix_in[i][j];

            /*if(current_element == 0) {
                
            } else if (current_element == 255) {

            }*/
            // Else leave the pixel unchanged (as S&P noise couldn't have affected it)
        }
    }
}
