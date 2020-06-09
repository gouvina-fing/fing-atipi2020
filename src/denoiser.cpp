// Dependencies
#include "denoiser.h"

// Methods

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
 |    - f(C) is used as index
 |
 |  Args:
 |    - delta (float): Probability that pixel sample goes to 0 or 255 (uniformly)
 |    - k (int): Parameter for context quantization, taking values from [0,8] 
 |    - img_in (float**): 2D matrix storing a grayscale image to be denoised.
 |    - img_prefiltered (float**):
 |          - NULL if prefilter is not desired.
 |          - 2D matrix storing a grayscale image if prefiltering and iteration are taken into account.
 |    - img_out (float**): 2D matrix that will store the result of DUDE
 |  
 |  Returns (int):
 |    - 0 if successful, error code otherwise
 |
 |  Raises:
 |    - 
*/
void dude(float delta, int k, float** img_in, float** img_prefiltered, float** img_out) {
    img_out = img_in;

    // Step 1: Calculate empirical distribution for each context


    // Step 2: Calculate the MAP response for each context


    // Step 3: Estimate each symbol by the MAP response to its context

}
