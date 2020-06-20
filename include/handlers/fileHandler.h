#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// Global dependencies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Local dependencies
#include <const.h>

// Check if file 'path' exists
short checkFile(const std::string path);

// Read content from file 'path' and get PGM header, saving dimensions in 'width' and 'height'
short readPGMHeader(const std::string path, short &width, short &height);

// Read content from file 'path' and get PGM content, saving it in 'matrix'
short readPGMContent(const std::string path, short width, short height, unsigned char** matrix);

// Create and write in file 'path' a PGM header with dimensions 'width' and 'height'
short writePGMHeader(const std::string path, short width, short height);

// Write in file 'path' a binary representation of 'matrix'
short writePGMContent(const std::string path, short width, short height, unsigned char** matrix);

#endif