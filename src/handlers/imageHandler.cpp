// Local dependencies
#include <handlers/imageHandler.h>

// AUX FUNCTIONS

// Read content from file 'path' and get PGM header, saving dimensions in 'width' and 'height'
short read_PGM_header(const std::string path, short &width, short &height){
    
    std::ifstream file(path);
    if (file.is_open()) {
        
        // Aux string for reading lines
        std::string line;

        // Line 1: Check PGM start
        std::getline(file, line);
        if (line.compare("P5") != 0) {
            file.close();
            return ERROR_FILE_BAD_HEADER;
        }

        // Line 2: Check if comment
        // If there is, read another line and check for dimensions
        // If not, check for dimensions in this line
        std::getline(file, line);
        while(line.at(0) == '#') std::getline(file, line);
        short w, h;
        std::istringstream iss(line);
        if (!(iss >> w >> h)) {
            file.close();
            return ERROR_FILE_BAD_DIMENSIONS;
        }

        // Line 3: Check PGM bpp
        std::getline(file, line);
        if (line.compare("255") != 0) {
            file.close();
            return ERROR_FILE_BAD_BPP;
        }

        width = w;
        height = h;
        
    } else {
        file.close();
        return ERROR_FILE_DAMAGED;
    }

    file.close();
    return OK;
}

// Read content from file 'path' and get PGM content, saving it in 'matrix'
short read_PGM_content(const std::string path, short width, short height, unsigned char** matrix){
    
    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        
        // Aux string for reading lines
        std::string line;

        // Line 1,2,3: Ignored (4 if there is comment)
        std::getline(file, line);
        std::getline(file, line);
        while(line.at(0) == '#') std::getline(file, line);
        std::getline(file, line);

        char c_aux = ' ';

        // Loop over matrix, reading bytes and casting them to float
        for (short i = 0; i < height; i++) {
            for (short j = 0; j < width; j++) {
                file.read(reinterpret_cast<char*>(&c_aux), sizeof(char));
                matrix[i][j] = static_cast<unsigned char>(c_aux);
            }
        }
        
    } else {
        file.close();
        return ERROR_FILE_DAMAGED;
    }

    file.close();
    return OK;
}

// Create and write in file 'path' a PGM header with dimensions 'width' and 'height' and a binary representation of 'matrix'
short write_PGM(const std::string path, short width, short height, unsigned char** matrix) {

    std::ofstream file(path, std::ios_base::binary);
    if (file.is_open()) {

        // Line 1: Check PGM start
        file << "P5" << std::endl;

        // Line 2: Check PGM image size
        file << width << " " << height << std::endl;

        // Line 3: Check PGM bpp
        file << 255 << std::endl;

        // Loop over matrix and write values as char (binary)        
        for (short i = 0; i < height; i++) {
            for (short j = 0; j < width; j++) {
                file << matrix[i][j];
            }
        }

    } else {
        file.close();
        return ERROR_FILE_DAMAGED;
    }

    file.close();
    return OK;
}

// MAIN FUNCTIONS

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
