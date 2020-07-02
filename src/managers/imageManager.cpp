// Local dependencies
#include <managers/imageManager.h>

// Read image file 'path' and cast it to image 'img' 
short read_image(const std::string path, ImageModel &img){

    // Result code
    short code = OK;

    // Check if file exists
    code = checkFile(path);
    if (code != OK) {
        return code;
    }

    // Try to read file header
    short width, height;
    code = readPGMHeader(path, width, height);
    if (code != OK) {
        return code;
    }

    // Try to read file content
    char** matrix = new char*[height];
    for (int i = 0; i < height; i++) matrix[i] = new char[width];
    code = readPGMContent(path, width, height, matrix);
    if (code != OK) {
        return code;
    }

    img = { width, height, matrix };
    return OK;
}

// Write image 'img' to file 'path'
short write_image(const std::string path, ImageModel img){

    // Aux variables
    short code = OK;

    // Remove image if it already exists
    remove(path.c_str());

    // Write PGM image content
    code = writePGM(path, img.getWidth(), img.getHeight(), img.getMatrix());
    if (code != OK) {
        return code;
    }

    return OK;
}

// Copy image 'img_in' into image 'img_out' 
short copy_image(ImageModel img_in, ImageModel &img_out){
    img_out.setWidth(img_in.getWidth());
    img_out.setHeight(img_in.getHeight());
    img_out.setMatrix(img_in.getMatrix());
    return OK;
}
