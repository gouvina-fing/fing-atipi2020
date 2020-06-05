// Local dependencies
#include <managers/imageManager.h>

// Read image file 'path' and cast it to image 'img' 
short read_image(const std::string path, ImageModel img){

    // Result code
    short code = OK;

    // Check if file exists
    code = checkFile(path);
    if (code != OK) {
        return code;
    }

    // Try to read file content
    short width, height;
    code = readPGMHeader(path, width, height);
    if (code != OK) {
        return code;
    }
    // std::cout << width << " " << height << '\n';

    img = { width, height, nullptr };
    return OK;
}

// Write image 'img' to file 'path'
short write_image(const std::string path, ImageModel img){
    return OK;
}

// Copy image 'img_in' into image 'img_out' 
short copy_image(ImageModel img_in, ImageModel img_out){
    img_out = { img_in.getWidth(), img_in.getHeight(), img_in.getMatrix()};
    return OK;
}
