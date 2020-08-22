#ifndef DENOISER_H
#define DENOISER_H

#include <models/imageModel.h>

#define M 256

struct HistogramData {
    short total_sum;
    short occurrences[M];
};

/*
 |  Process a grayscale image applying the DUDE algorithm, and storing the obtained result in img_out
 |
 |  Preconditions: This implementation of DUDE assumes that:
 |    - Images are grayscale 8bpp, taking values from [0,255]
 |    - The noise that affects the images comes from a Salt & Pepper channel (with parameter delta)
 |    
 |  The denoiser has the following peculiarities:
 |    - Every pixel outside the border of an image has 128 as its value.
 |    - A 3x3 context is used (the 8 pixels that are immediate neighboors to the one in the center)
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
*/
void dude(float delta, short k, ImageModel img_in, ImageModel img_prefiltered, ImageModel &img_out);

#endif
