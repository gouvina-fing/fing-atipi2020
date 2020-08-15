// Dependencies
#include "denoiser.h"
#include <math.h>
#include <algorithm>
#include <cstdio> // TODO: Remove this (printf)

// TODO: Should this be in denoiser.h?
#define M 256

// TODO: Should this be in denoiser.h?
struct HistogramData {
    short total_sum;
    short occurrences[M];
};

// Allocate memory for all auxiliary variables and for the output image to be written
void allocate_memory(short ***A, short ***contexts, struct HistogramData **histograms, unsigned char ***img_out, ImageModel img_in, short histograms_length) {
    short height = img_in.getHeight();
    short width = img_in.getWidth();

    // Context matrix and auxiliary matrix
    *A = new short*[height];
    *contexts = new short*[height];
    *img_out = new unsigned char*[height];
    for (short i = 0; i < height; ++i) {
        (*A)[i] = new short[width];
        (*contexts)[i] = new short[width];
        (*img_out)[i] = new unsigned char[width];
    }

    *histograms = new HistogramData[histograms_length];
}

// Initialize auxiliarty context structure
void initialize_auxiliary_contexts(short **A, ImageModel img_in) {
    short height = img_in.getHeight();
    short width = img_in.getWidth();
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
        }
    }
}

// Initialize auxiliary histogram structure
void initialize_histograms(struct HistogramData *histograms, short histograms_length) {
    for (short i = 0; i < histograms_length; ++i) {
        histograms[i].total_sum = 0;
        for (short j = 0; j < M; ++j)
            histograms[i].occurrences[j] = 0;
    }
}

// Preprocess the image, gathering empirical data from the context that surrounds each pixel and recording its cuantization
void preprocessing(short **A, short **contexts, struct HistogramData **histograms, ImageModel img_in, ImageModel context_img_in, short histograms_length, short k) {
    short height = img_in.getHeight();
    short width = img_in.getWidth();

    short current_element, current_aux_context;
    unsigned char **matrix_in = img_in.getMatrix();
    unsigned char **matrix_in_context = context_img_in.getMatrix();

    // Get aux data from the context (sum the value of each pixel to the masks that cover it)
    for (short i = 0; i < height; ++i) {
        for (short j = 0; j < width; ++j) {
            current_element = matrix_in_context[i][j];
            for(short mask_i = i - 1; (mask_i < i + 2) && (mask_i >= 0) && (mask_i < height); ++mask_i) {
                for(short mask_j = j - 1; (mask_j < j + 2) && (mask_j >= 0) && (mask_j < width); ++mask_j) {
                    A[mask_i][mask_j] += current_element;
                }
            }
        }
    }
    
    // Compute A and use it to compute the context for each pixel
    bool aux_bit_condition;
    short divisor_aux, context;
    short binary_aux;
    
    for (short i = 0; i < height; ++i) {
        for (short j = 0; j < width; ++j) {
            binary_aux = histograms_length/2; // 2^(4+k-1)
            current_aux_context = floorf((A[i][j]/8) + 0.5);
            A[i][j] = current_aux_context;

            // Compute f(C) (Histogram Table)
            context = 0;

            // F1: North
            if (i == 0)
                aux_bit_condition = (128 > current_aux_context);
            else
                aux_bit_condition = (matrix_in_context[i-1][j] > current_aux_context);

            if (aux_bit_condition)
                context += binary_aux; // binary_aux = 2^(k+3)

            binary_aux /= 2;

            // F2: East
            if (j == width - 1)
                aux_bit_condition = (128 > current_aux_context);
            else
                aux_bit_condition = (matrix_in_context[i][j+1] > current_aux_context);
            
            if (aux_bit_condition) {
                context += binary_aux; // binary_aux = 2^(k+2)
            }
            
            binary_aux /= 2;

            // F3: South
            if (i == height - 1)
                aux_bit_condition = (128 > current_aux_context);
            else
                aux_bit_condition = (matrix_in_context[i+1][j] > current_aux_context);
            
            if (aux_bit_condition)
                context += binary_aux; // binary_aux = 2^(k+1)
            
            binary_aux /= 2;

            // F4: West
            if (j == 0)
                aux_bit_condition = (128 > current_aux_context);
            else
                aux_bit_condition = (matrix_in_context[i][j-1] > current_aux_context);
            
            if (aux_bit_condition)
                context += binary_aux; // binary_aux = 2^(k)
            
            binary_aux /= 2;

            // F5 .. F4+k
            context += floorf(A[i][j]/pow(2, 8-k));

            // Save result
            contexts[i][j] = context;

            // Record that this context appeared (In the original input image)
            current_element = matrix_in[i][j];
            (*histograms)[context].total_sum += 1;
            (*histograms)[context].occurrences[current_element] += 1;
        }
    }
}

// Execute DUDE
void denoise(short **contexts, struct HistogramData *histograms, ImageModel img_in, short histograms_length, float delta, unsigned char **matrix_out) {
    short height = img_in.getHeight();
    short width = img_in.getWidth();
    unsigned char **matrix_in = img_in.getMatrix();

    float partial_delta_coef = delta/(2*(1-delta));
    float inner_delta_coef = (delta/2)*(M-1);
    // The sum "n" was previously computed for each context and is equal to: histograms[current_context].total_sum
    
    short current_element, current_context;
    float delta_coef;
    unsigned char denoised_pixel;
    int sum;

    for (short i = 0; i < height; ++i) {
        for (short j = 0; j < width; ++j) {
            current_element = matrix_in[i][j];
            current_context = contexts[i][j];

            if((current_element == 0) || (current_element == M-1)) {
                delta_coef = partial_delta_coef/histograms[current_context].occurrences[current_element];
                sum = 0;

                if(current_element == 0) {
                    for (short k = 0; k < M; ++k) // sum to M-1
                        sum += k*histograms[current_context].occurrences[k];
                    denoised_pixel = delta_coef*(sum - histograms[current_context].total_sum*inner_delta_coef);
                } else { // current_element == M-1
                    for (short k = 0; k < M; ++k) // sum to M-1
                        sum += (M - 1 - k)*histograms[current_context].occurrences[k];
                    denoised_pixel = delta_coef*(-sum + histograms[current_context].total_sum*inner_delta_coef) + M -1;
                }
            } else { // Leave the pixel unchanged (as S&P noise couldn't have affected it)
                denoised_pixel = current_element;
            }

            matrix_out[i][j] = fmax(0, fmin(M-1, denoised_pixel));
        }
    }
}

// Free all memory that was created for auxiliary structures
// TODO: FIXME: Esto explota cuando la imagen es un cuadrado vertical (mas alto que ancho)
void free_memory(short ***A, short ***contexts, struct HistogramData **histograms, ImageModel img_in) {
    // Context matrix and auxiliary matrix
    for (short i = 0; i < img_in.getHeight(); ++i) {
        delete [] (*A)[i];
        delete [] (*contexts)[i];
    } 
    delete [] (*A);
    delete [] (*contexts);

    // Histogram table
    delete [] (*histograms);
}

// TODO: Make it return an int? I don't think there is any specific error we can catch and return an error code.
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
 |    - f(C) is used as index in the conditional histogram's table (there are 2^(4+k) possible contexts for each pixel value)
 |    - After that it applies the previously calculated formula for denoising a S&P channel.
 |          using "the number of a pixel on a specific context" as "the number of occurences (n_i)"
 |
 |  Args:
 |    - delta (float): Probability that pixel sample goes to 0 or 255 (uniformly)
 |    - k (short): Parameter for context quantization, taking values from [0,8] 
 |    - img_in (ImageModel): Model containing a 2D matrix storing a grayscale image to be denoised.
 |    - img_prefiltered (ImageModel):
 |          - Empty ImageModel if prefilter is not desired.
 |          - Model containing a 2D matrix storing a grayscale image if prefiltering and iteration are taken into account.
 |    - img_out (ImageModel): Model containing a 2D matrix that will store the result of DUDE
 |  
 |  Returns (int):
 |    - 0 if successful, error code otherwise
 |
 |  Raises:
 |    - 
*/
void dude(float delta, short k, ImageModel img_in, ImageModel img_prefiltered, ImageModel &img_out) {
    short **A;
    unsigned char **matrix_out;
    short **contexts;
    struct HistogramData *histograms;

    short histograms_length = pow(2, 4+k);

    allocate_memory(&A, &contexts, &histograms, &matrix_out, img_in, histograms_length);
    initialize_auxiliary_contexts(A, img_in);
    initialize_histograms(histograms, histograms_length);

    // Step 1: Calculate empirical distribution for each context
    if(img_prefiltered.getEmpty()) {
        // If no prefiltered image is provided we compute C from the input image
        preprocessing(A, contexts, &histograms, img_in, img_in, histograms_length, k);
        
    } else {
        // If a prefiltered image is provided we compute C from it, (histograms are still accounted comparing with the input one)
        preprocessing(A, contexts, &histograms, img_in, img_prefiltered, histograms_length, k);
    }

    // Step 2: Estimate each symbol by the calculated MAP response to its context
    denoise(contexts, histograms, img_in, histograms_length, delta, matrix_out);

    img_out.setHeight(img_in.getHeight());
    img_out.setWidth(img_in.getWidth());
    img_out.setMatrix(matrix_out);

    // free_memory(&A, &contexts, &histograms, img_in);
}
