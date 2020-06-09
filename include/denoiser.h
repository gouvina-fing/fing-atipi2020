#ifndef DENOISER_H
#define DENOISER_H

// DUDE algorithm with input 'delta', 'k', 'img_in', 'img_prefiltered, and output 'img_out'
void dude(float delta, int k, float** img_in, float** img_prefiltered, float** img_out);

#endif
