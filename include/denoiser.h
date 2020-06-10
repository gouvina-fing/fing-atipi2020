#ifndef DENOISER_H
#define DENOISER_H

#include <models/imageModel.h>

// DUDE algorithm with input 'delta', 'k', 'img_in', 'img_prefiltered, and output 'img_out'
void dude(float delta, int k, ImageModel img_in, ImageModel img_prefiltered, ImageModel img_out);

#endif
