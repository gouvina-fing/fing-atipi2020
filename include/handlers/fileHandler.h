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

// Read content from file 'path' and save it in 'result'
short readPGMHeader(const std::string path, short &width, short &height);

#endif