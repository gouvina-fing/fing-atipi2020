#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

// Global dependencies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Local dependencies
#include <models/imageModel.h>
#include <handlers/fileHandler.h>
#include <const.h>

// Read image file 'path' and cast it to image 'img' 
short read_image(const std::string path, ImageModel &img);

// Write image 'img' to file 'path'
short write_image(const std::string path, ImageModel img);

// Copy image 'img_in' into image 'img_out' 
short copy_image(ImageModel img_in, ImageModel &img_out);

#endif