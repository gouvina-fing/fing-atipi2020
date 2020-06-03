// Local dependencies
#include <handlers/fileHandler.h>

// Check if file 'path' exists
bool checkFile(const char * path) { 
    std::ifstream infile(path);
    return infile.good();
}
