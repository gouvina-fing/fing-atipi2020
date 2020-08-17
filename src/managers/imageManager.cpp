// Local dependencies
#include <managers/imageManager.h>

// Read image file 'path' and cast it to image 'img' 
short read_image(const std::string path, ImageModel &img){

    // Result code
    short code = OK;

    // Check if file exists
    code = check_file(path);
    if (code != OK) {
        return code;
    }

    // Try to read file header
    short width, height;
    code = read_PGM_header(path, width, height);
    if (code != OK) {
        return code;
    }

    // Try to read file content
    unsigned char** matrix = new unsigned char*[height];
    for (int i = 0; i < height; i++) matrix[i] = new unsigned char[width];
    code = read_PGM_content(path, width, height, matrix);
    if (code != OK) {
        return code;
    }

    img = { width, height, matrix, false };
    return OK;
}

// Write image 'img' to file 'path'
short write_image(const std::string path, ImageModel img){

    // Aux variables
    short code = OK;

    // Remove image if it already exists
    remove(path.c_str());

    // Write PGM image content
    code = write_PGM(path, img.getWidth(), img.getHeight(), img.getMatrix());
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
    img_out.setEmpty(false);
    return OK;
}
