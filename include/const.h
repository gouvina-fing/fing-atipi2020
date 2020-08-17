#ifndef CONST_H
#define CONST_H

// Global dependencies
#include <string>

// Function return codes
const unsigned short OK = 0;
// Main errors
const unsigned short ERROR_MAIN_BAD_FORMATTING = 1;
// File errors
const unsigned short ERROR_FILE_UNKNOWN = 2;
const unsigned short ERROR_FILE_DAMAGED = 3;
const unsigned short ERROR_FILE_BAD_HEADER = 4;
const unsigned short ERROR_FILE_BAD_DIMENSIONS = 5;
const unsigned short ERROR_FILE_BAD_BPP = 6;
const unsigned short ERROR_DIRECTORY = 7;

// Function return messages
// Main errors
const std::string ERROR_MAIN_BAD_FORMATTING_MSG = "(Error, Main) - Bad formatted call for main program.\n";
// File errors
const std::string ERROR_FILE_UNKNOWN_MSG = "(Error, FileHandler) - Cannot find file.\n";
const std::string ERROR_FILE_DAMAGED_MSG = "(Error, FileHandler) - Cannot open file.\n";
const std::string ERROR_FILE_BAD_HEADER_MSG = "(Error, FileHandler) - Bad formatted file, incorrect PGM header\n";
const std::string ERROR_FILE_BAD_DIMENSIONS_MSG = "(Error, FileHandler) - Bad formatted file, incorrect PGM dimensions\n";
const std::string ERROR_FILE_BAD_BPP_MSG = "(Error, FileHandler) - Bad formatted file, incorrect PGM bpp\n";

#endif