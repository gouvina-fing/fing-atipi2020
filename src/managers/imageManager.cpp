// Local dependencies
#include <models/imageModel.h>
#include <managers/imageManager.h>
#include <handlers/fileHandler.h>

// Read image file 'path' and cast it to image 'img' 
bool read_image(const char * path, ImageModel img){

    // Check if file exists
    if (!checkFile(path)) return false;
  
    img = { 0, 0, nullptr };
    return true;
}

// Write image 'img' to file 'path'
bool write_image(const char * path, ImageModel img){
    return true;
}

// Copy image 'img_in' into image 'img_out' 
bool copy_image(ImageModel img_in, ImageModel img_out){
    img_out = { img_in.getWidth(), img_in.getHeight(), img_in.getMatrix()};
    return true;
}
