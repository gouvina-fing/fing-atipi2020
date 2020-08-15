// Local dependencies
#include <handlers/fileHandler.h>

// Check if file 'path' exists
short checkFile(const std::string path) { 
    std::ifstream file(path);
    if (file.good()) return OK;
    return ERROR_FILE_UNKNOWN;
}

// Get file name as string from 'path'
std::string getFileName(const std::string path) { 
    
    std::stringstream test(path);
    std::string segment;

    std::vector<std::string> seglist;
    std::vector<std::string>::iterator it;

    while(std::getline(test, segment, '/')) {
        it = seglist.begin();
        seglist.insert(it, segment);
    }

    return seglist.front();
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
short readPGMContent(const std::string path, short width, short height, unsigned char** matrix){
    
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
                // file.read(reinterpret_cast<unsigned char*>(&matrix[i][j]), 1);
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
short writePGM(const std::string path, short width, short height, unsigned char** matrix) {

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
                // file.write(reinterpret_cast<const unsigned unsigned char*>(&matrix[i][j]), 1);
            }
        }

    } else {
        file.close();
        return ERROR_FILE_DAMAGED;
    }

    file.close();
    return OK;
}
