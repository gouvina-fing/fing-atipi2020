// Local dependencies
#include <handlers/fileHandler.h>

// Check if file 'path' exists
short checkFile(const std::string path) { 
    std::ifstream file(path);
    if (file.good()) return OK;
    return ERROR_FILE_UNKNOWN;
}

// Read content from file 'path' and get PGM header, saving dimensions in 'width' and 'height'
short readPGMHeader(const std::string path, short &width, short &height){
    
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

        // TODO: HACK: Remove this for comments
        std::getline(file, line);

        // Line 2: Check PGM image size
        std::getline(file, line);
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
short readPGMContent(const std::string path, short width, short height, unsigned char** matrix){
    
    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        
        // Aux string for reading lines
        std::string line;

        // Line 1,2,3: Ignored
        std::getline(file, line);
        std::getline(file, line);
        std::getline(file, line);

        // Loop over matrix, reading bytes and casting them to float
        char c_aux = ' ';
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

// Create and write in file 'path' a PGM header with dimensions 'width' and 'height'
short writePGMHeader(const std::string path, short width, short height) {
    
    std::ofstream file(path);
    if (file.is_open()) {

        // Line 1: Check PGM start
        file << "P5" << std::endl;

        // Line 2: Check PGM image size
        file << std::to_string(width) << " " << std::to_string(height) << std::endl;

        // Line 3: Check PGM bpp
        file << "255" << std::endl;
        
    } else {
        file.close();
        return ERROR_FILE_DAMAGED;
    }

    file.close();
    return OK;
}

// Write in file 'path' a binary representation of 'matrix'
short writePGMContent(const std::string path, short width, short height, unsigned char** matrix) {

    std::ofstream file(path, std::ios_base::app);
    if (file.is_open()) {

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
