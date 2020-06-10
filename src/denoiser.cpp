// Dependencies
#include "denoiser.h"
#include <math.h>

// Methods
void allocate_memory(float ***A, short height, short width) {
    *A = (float**) malloc(height*sizeof(float*));
    for (int i = 0; i < height; ++i) (*A)[i] = (float*) malloc(width*sizeof(float));

    for (short i = 0; i < height; ++i) {
        for (short j = 0; j < width; ++j) {
            // Base value
            *A[i][j] = 0;

            // Pre-Fill values for borders outside of the images

            // If it's in the north or south border it collides with 3 
            if ((i == 0) || (i == height -1)) {
                *A[i][j] += 384;
            }

            // If it's in the east or west border it collides with 3 
            if((j == 0) || (j == width -1)) {
                *A[i][j] += 384;
            }

            // If it's in a corner it collides with 5 (and above we added an extra collision)
            if ((i == 0) && (j == 0) || (i == 0) && (j == width -1) || (i == height -1) && (j == 0) || (i == height -1) && (j == width -1) ) {
                *A[i][j] -= 128;
            }
        }
    }
}

void free_memory(float ***A, short height) {
    for (int i = 0; i < height; ++i) free((*A)[i]);
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
 |    - k (int): Parameter for context quantization, taking values from [0,8] 
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
void dude(float delta, int k, ImageModel img_in, ImageModel img_prefiltered, ImageModel img_out) {
    img_out = img_in;

    // Preprocessing
    float **A;

    matrix_in = img_in.getMatrix();
    allocate_memory(&A, img_in.getHeigh(), img_in.getWidth());

    // Get data from the context
    for (short i = 0; i < img_in.getHeight(); ++i) {
        for (short j = 0; j < img_in.getWidth(); ++j) {
            for(short mask_i = i - 1; (mask_i < i + 2) && (mask_i >= 0) && (mask_i < img_in.getHeigh()); ++mask_i) {
                for(short mask_j = j - 1; (mask_j < j + 2) && (mask_j >= 0) && (mask_j < img_in.getWidth()); ++mask_j) {
                    A[mask_i][mask_j] += matrix_in[i][j];
                }
            }
        }
    }

    for (short i = 0; i < img_in.getHeight(); ++i) {
        for (short j = 0; j < img_in.getWidth(); ++j) {
            A[i][j] /= 8;
            A[i][j] += 0.5;
            A[i][j] = floorf(A[i][j]);
        }
    }

    // Step 1: Calculate empirical distribution for each context


    // Step 2: Calculate the MAP response for each context


    // Step 3: Estimate each symbol by the MAP response to its context

}
