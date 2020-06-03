#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

// Dependencies
#include <models/imageModel.h>

// Read image file 'path' and cast it to image 'img' 
bool read_image(const char * path, ImageModel img);

// Write image 'img' to file 'path'
bool write_image(const char * path, ImageModel img);

// Copy image 'img_in' into image 'img_out' 
bool copy_image(ImageModel img_in, ImageModel img_out);

#endif