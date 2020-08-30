#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// Global dependencies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

// Local dependencies
#include <const.h>

// Check if file 'path' exists
short check_file(const std::string path);

// Get file name as string from path
std::string get_file_name(const std::string path);

// Create directory specified in 'path'
short add_directory(const std::string path);

// Remove directory specified in 'path'
short remove_directory(const std::string path);

// Create and write in file 'path' a CSV table contained in text
short write_CSV(const std::string path, const std::string text);

#endif